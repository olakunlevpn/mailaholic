# DKIM API

Generate and manage DKIM keys for email signing.

## Base URL

```
https://localhost:<port>/api/dkim
```

## Authentication

All endpoints require Bearer token authentication.

```
Authorization: Bearer <token>
```

## Endpoints

### Get DKIM Status

Check if DKIM is configured for a domain.

```
GET /api/dkim/status?domain_id=:id
```

**Query Parameters:**
- `domain_id` (required) — Domain ID to check

**Success Response (200):**
```json
{
  "success": true,
  "data": {
    "hasKeys": true,
    "selector": "mail"
  }
}
```

**Response when no keys configured:**
```json
{
  "success": true,
  "data": {
    "hasKeys": false,
    "selector": ""
  }
}
```

**Error Response (400):**
```json
{
  "error": "Missing domain_id"
}
```

**Error Response (404):**
```json
{
  "error": "Domain not found"
}
```

---

### Generate DKIM Keys

Generate a new RSA key pair for DKIM signing.

```
POST /api/dkim/generate
```

**Request Body:**
```json
{
  "domain_id": 1,
  "selector": "mail"
}
```

| Field | Type | Required | Default | Description |
|-------|------|----------|---------|-------------|
| domain_id | integer | Yes | — | Domain to configure |
| selector | string | No | "mail" | DKIM selector name |

**Success Response (200):**
```json
{
  "success": true,
  "data": {
    "selector": "mail",
    "dnsRecord": "mail._domainkey.example.com",
    "dnsType": "TXT",
    "dnsValue": "v=DKIM1; k=rsa; p=MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA..."
  }
}
```

**Error Response (400):**
```json
{
  "error": "Missing domain_id"
}
```

**Error Response (404):**
```json
{
  "error": "Domain not found"
}
```

**Error Response (500):**
```json
{
  "error": "Failed to generate keys"
}
```

**Notes:**
- Generates 2048-bit RSA key pair
- Private key is stored in the database
- Public key is returned for DNS configuration
- Overwrites existing keys for the domain

---

## Examples

### Check DKIM status
```bash
curl "https://localhost:8443/api/dkim/status?domain_id=1" \
  -H "Authorization: Bearer $TOKEN" \
  -k
```

### Generate DKIM keys with default selector
```bash
curl -X POST https://localhost:8443/api/dkim/generate \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"domain_id":1}' \
  -k
```

### Generate DKIM keys with custom selector
```bash
curl -X POST https://localhost:8443/api/dkim/generate \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"domain_id":1,"selector":"mailaholic2024"}' \
  -k
```

### Full workflow: Generate and verify
```bash
# Generate keys
RESULT=$(curl -s -X POST https://localhost:8443/api/dkim/generate \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"domain_id":1}' \
  -k)

# Extract DNS record info
echo $RESULT | jq '.data.dnsRecord'
echo $RESULT | jq '.data.dnsValue'

# Add to DNS, then verify
dig TXT mail._domainkey.example.com +short
```

## DNS Configuration

After generating keys, add a TXT record to your DNS:

| Field | Value |
|-------|-------|
| Type | TXT |
| Name | `<selector>._domainkey.<domain>` |
| Value | The `dnsValue` from the response |

**Example:**
```
Name:  mail._domainkey.example.com
Type:  TXT
Value: v=DKIM1; k=rsa; p=MIIBIjANBgkq...
```

## Key Specifications

- Algorithm: RSA
- Key size: 2048 bits
- Format: PEM (private), Base64 (public in DNS)
- Hash: SHA-256 for signing

## See Also

- [DKIM Setup Guide](../dkim.md) — Complete DKIM configuration guide
- [Domains API](domains.md) — Domain management
