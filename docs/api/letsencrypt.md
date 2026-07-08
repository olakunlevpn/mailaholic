# Let's Encrypt API

Certificate status and ACME challenge support.

## Base URL

```
https://localhost:<port>/api/letsencrypt
```

## Authentication

Status and request endpoints require Bearer token authentication.
Challenge endpoint is public (required for ACME validation).

## Endpoints

### Get Certificate Status

Check current SSL certificate status.

```
GET /api/letsencrypt/status
```

**Headers:**
```
Authorization: Bearer <token>
```

**Success Response (200):**
```json
{
  "success": true,
  "data": {
    "hasCertificate": true,
    "isLetsEncrypt": false,
    "instructions": "To use Let's Encrypt certificates, run certbot externally and configure the certificate paths in mailaholic.ini"
  }
}
```

**Response Fields:**

| Field | Type | Description |
|-------|------|-------------|
| hasCertificate | boolean | Server has an SSL certificate |
| isLetsEncrypt | boolean | Certificate is from Let's Encrypt |
| instructions | string | Setup guidance |

---

### Request Certificate

Get instructions for obtaining a Let's Encrypt certificate.

```
POST /api/letsencrypt/request
```

**Headers:**
```
Authorization: Bearer <token>
```

**Success Response (200):**
```json
{
  "success": true,
  "data": {
    "manual": true,
    "command": "certbot certonly --webroot -w /path/to/mailaholic/data -d YOUR_DOMAIN",
    "certPath": "/etc/letsencrypt/live/YOUR_DOMAIN/fullchain.pem",
    "keyPath": "/etc/letsencrypt/live/YOUR_DOMAIN/privkey.pem"
  }
}
```

**Response Fields:**

| Field | Type | Description |
|-------|------|-------------|
| manual | boolean | Manual setup required |
| command | string | Certbot command to run |
| certPath | string | Where certificate will be saved |
| keyPath | string | Where private key will be saved |

---

### ACME Challenge

Serve ACME HTTP-01 challenge responses.

```
GET /.well-known/acme-challenge/:token
```

**No authentication required** — ACME servers must access this publicly.

**URL Parameters:**
- `token` — Challenge token from ACME server

**Success Response (200):**
```
<authorization-string>
```

Content-Type: `text/plain`

**Error Response (404):**
```
Not found
```

**Note:** Challenges are set programmatically via `LetsEncryptApi::SetChallengeToken()`.

---

## Manual Certificate Setup

Mailaholic supports Let's Encrypt through external certbot usage:

### 1. Install Certbot

**Windows:**
```cmd
winget install Certbot.Certbot
```

**Linux:**
```bash
apt install certbot
```

### 2. Request Certificate

```bash
certbot certonly --webroot \
  -w C:\Mailaholic\data \
  -d mail.example.com
```

### 3. Configure Mailaholic

Edit `mailaholic.ini`:
```ini
[SSL]
CertificateFile=C:\Certbot\live\mail.example.com\fullchain.pem
PrivateKeyFile=C:\Certbot\live\mail.example.com\privkey.pem
```

### 4. Restart Server

Restart Mailaholic to load the new certificate.

### 5. Setup Auto-Renewal

**Windows (Task Scheduler):**
```cmd
schtasks /create /sc daily /tn "Certbot Renewal" /tr "certbot renew --quiet" /st 03:00
```

**Linux (Cron):**
```bash
echo "0 3 * * * certbot renew --quiet" | crontab -
```

---

## Examples

### Check certificate status
```bash
curl https://localhost:8443/api/letsencrypt/status \
  -H "Authorization: Bearer $TOKEN" \
  -k
```

### Get setup instructions
```bash
curl -X POST https://localhost:8443/api/letsencrypt/request \
  -H "Authorization: Bearer $TOKEN" \
  -k
```

### Test ACME challenge endpoint
```bash
# This would normally be called by Let's Encrypt servers
curl http://mail.example.com/.well-known/acme-challenge/test-token
```

## Future Enhancements

Planned features (not yet implemented):
- Automatic ACME account registration
- Built-in certificate request flow
- Automatic renewal
- Certificate expiration monitoring

## See Also

- [SSL/TLS Certificates](../ssl-certificates.md) — Complete SSL setup guide
- [Installation Guide](../installation.md) — Initial setup
