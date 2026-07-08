# SSL/TLS Certificates

Mailaholic uses SSL/TLS to encrypt all email traffic. This guide covers certificate options.

## Certificate Types

### Self-Signed (Default)

During setup, Mailaholic generates a self-signed certificate automatically.

**Pros:**
- No external dependencies
- Works immediately
- No renewal required

**Cons:**
- Email clients show security warnings
- Not trusted by other mail servers
- May affect deliverability

**Use for:** Testing, internal networks, development.

### Let's Encrypt (Recommended)

Free, trusted certificates from Let's Encrypt.

**Pros:**
- Trusted by all clients and servers
- Free
- Auto-renewal possible

**Cons:**
- Requires domain validation
- Requires port 80 accessible
- 90-day renewal cycle

**Use for:** Production servers.

### Custom Certificate

Use certificates from any Certificate Authority (CA).

**Pros:**
- Extended validation options
- Longer validity periods
- Wildcard support

**Cons:**
- Cost
- Manual renewal

**Use for:** Enterprise deployments, specific compliance requirements.

## Let's Encrypt Setup

### Prerequisites

- Domain name pointing to your server
- Port 80 accessible from the internet
- Certbot installed

### Using Certbot

1. Install certbot:
```cmd
winget install Certbot.Certbot
```

2. Request certificate:
```cmd
certbot certonly --webroot -w C:\Mailaholic\data -d mail.example.com
```

3. Configure Mailaholic to use the certificate. Edit `mailaholic.ini`:
```ini
[SSL]
CertificateFile=C:\Certbot\live\mail.example.com\fullchain.pem
PrivateKeyFile=C:\Certbot\live\mail.example.com\privkey.pem
```

4. Restart Mailaholic.

### Auto-Renewal

Set up a scheduled task to renew certificates:

```cmd
schtasks /create /sc daily /tn "Certbot Renewal" /tr "certbot renew --quiet" /st 03:00
```

## Custom Certificate Setup

1. Obtain certificate and private key from your CA
2. Save files to a secure location
3. Edit `mailaholic.ini`:
```ini
[SSL]
CertificateFile=C:\Certs\mail.example.com.crt
PrivateKeyFile=C:\Certs\mail.example.com.key
```
4. Restart Mailaholic

### Certificate Requirements

- RSA 2048-bit or higher
- PEM format
- Full certificate chain (intermediate certs included)
- Matching private key

## Verifying Certificates

Test your certificate:

```cmd
openssl s_client -connect mail.example.com:993 -servername mail.example.com
```

Check certificate details:
```cmd
openssl x509 -in certificate.pem -text -noout
```

## Troubleshooting

### "Certificate not trusted"
- Using self-signed certificate
- Missing intermediate certificates
- Certificate expired

### "Hostname mismatch"
- Certificate CN/SAN doesn't match server hostname
- Request certificate for correct domain

### "Connection refused"
- Certificate file path incorrect
- Private key doesn't match certificate
- File permissions issue

## API Reference

Check certificate status via API:
```bash
curl https://localhost:PORT/api/letsencrypt/status \
  -H "Authorization: Bearer TOKEN" -k
```

See [Let's Encrypt API](api/letsencrypt.md) for details.
