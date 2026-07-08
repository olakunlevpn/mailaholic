# DKIM Setup

DomainKeys Identified Mail (DKIM) digitally signs outgoing emails to verify they haven't been modified and came from your server.

## Why DKIM?

- **Deliverability** — Major email providers (Gmail, Outlook) check DKIM. Missing DKIM = higher spam score.
- **Authentication** — Proves emails came from your domain, not a spammer.
- **Reputation** — Builds domain reputation over time.

## How DKIM Works

1. Server generates RSA key pair (private + public)
2. Private key stays on server, signs outgoing emails
3. Public key published in DNS as TXT record
4. Receiving servers verify signature using public key

## Generating DKIM Keys

### Via Admin Panel

Currently, generate keys via API. Admin panel UI coming soon.

### Via API

```bash
# Get auth token
TOKEN=$(curl -s -X POST https://localhost:PORT/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{"email":"admin@example.com","password":"xxx"}' -k | jq -r '.data.token')

# Generate DKIM keys for domain
curl -X POST https://localhost:PORT/api/dkim/generate \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"domain_id":1,"selector":"mail"}' -k
```

Response:
```json
{
  "success": true,
  "data": {
    "selector": "mail",
    "dnsRecord": "mail._domainkey.example.com",
    "dnsType": "TXT",
    "dnsValue": "v=DKIM1; k=rsa; p=MIIBIjANBgkq..."
  }
}
```

## DNS Configuration

Add the TXT record from the API response to your DNS:

| Type | Name | Value |
|------|------|-------|
| TXT | mail._domainkey.example.com | v=DKIM1; k=rsa; p=MIIBIjANBgkq... |

**Note:** The `p=` value is your public key. It's long — ensure you copy the entire value.

## Verifying DKIM

### Check DNS Record

```bash
dig TXT mail._domainkey.example.com +short
```

Should return your DKIM record.

### Test Email Signing

1. Send email to a Gmail address
2. Open the email in Gmail
3. Click three dots → "Show original"
4. Look for `DKIM: PASS`

### Online Tools

- [mail-tester.com](https://www.mail-tester.com) — Comprehensive email test
- [dkimvalidator.com](https://dkimvalidator.com) — DKIM-specific validation

## Selector

The selector allows multiple DKIM keys per domain. Default is `mail`.

Use different selectors for:
- Key rotation (create new key, update DNS, switch selector)
- Multiple servers sending for same domain
- Testing without affecting production

## Key Rotation

To rotate DKIM keys:

1. Generate new key with different selector:
```bash
curl -X POST https://localhost:PORT/api/dkim/generate \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"domain_id":1,"selector":"mail2"}' -k
```

2. Add new DNS record (keep old one)
3. Wait for DNS propagation (24-48 hours)
4. Update domain to use new selector
5. Remove old DNS record after 1 week

## Troubleshooting

### "DKIM: FAIL"
- DNS record not found — check record name
- Key mismatch — regenerate and update DNS
- DNS not propagated — wait 24-48 hours

### Long DNS Value
Some registrars have character limits on TXT records. Split the value:
```
"v=DKIM1; k=rsa; p=MIIBIjANBgkqhkiG9w0BAQEFAAO" "CAQ8AMIIBCgKCAQEA..."
```

### Record Not Found
- Check selector matches (e.g., `mail._domainkey` not `dkim._domainkey`)
- Verify domain has DNS zone configured
- Check for typos in record name

## API Reference

| Endpoint | Method | Description |
|----------|--------|-------------|
| /api/dkim/status | GET | Check DKIM status for domain |
| /api/dkim/generate | POST | Generate new DKIM keys |

See [DKIM API](api/dkim.md) for details.
