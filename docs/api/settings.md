# Settings API

Read and modify server configuration.

## Base URL

```
https://localhost:<port>/api/settings
```

## Authentication

All endpoints require Bearer token authentication.

```
Authorization: Bearer <token>
```

## Endpoints

### Get Settings

Retrieve all server settings.

```
GET /api/settings
```

**Success Response (200):**
```json
{
  "success": true,
  "data": {
    "smtp": {
      "enabled": true,
      "maxMessageSize": 20971520,
      "maxRecipients": 100
    },
    "logging": {
      "enabled": true,
      "logSMTP": false,
      "logPOP3": false,
      "logIMAP": false,
      "logDebug": false
    }
  }
}
```

---

### Update SMTP Settings

Modify SMTP-related settings.

```
PUT /api/settings/smtp
```

**Request Body:**
```json
{
  "enabled": true,
  "maxMessageSize": 52428800,
  "maxRecipients": 50
}
```

All fields are optional. Only provided fields are updated.

**Success Response (200):**
```json
{
  "success": true,
  "data": {}
}
```

**Field Details:**

| Field | Type | Range | Description |
|-------|------|-------|-------------|
| enabled | boolean | — | Enable/disable SMTP service |
| maxMessageSize | integer | 1024-104857600 | Max email size in bytes |
| maxRecipients | integer | 1-1000 | Max recipients per message |

---

### Update Logging Settings

Modify logging configuration.

```
PUT /api/settings/logging
```

**Request Body:**
```json
{
  "enabled": true,
  "logSMTP": true,
  "logPOP3": false,
  "logIMAP": false,
  "logDebug": false
}
```

All fields are optional. Only provided fields are updated.

**Success Response (200):**
```json
{
  "success": true,
  "data": {}
}
```

**Field Details:**

| Field | Type | Description |
|-------|------|-------------|
| enabled | boolean | Master logging switch |
| logSMTP | boolean | Log SMTP protocol conversations |
| logPOP3 | boolean | Log POP3 protocol conversations |
| logIMAP | boolean | Log IMAP protocol conversations |
| logDebug | boolean | Enable verbose debug output |

---

## Examples

### Get current settings
```bash
curl https://localhost:8443/api/settings \
  -H "Authorization: Bearer $TOKEN" \
  -k
```

### Increase max message size to 50 MB
```bash
curl -X PUT https://localhost:8443/api/settings/smtp \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"maxMessageSize":52428800}' \
  -k
```

### Enable SMTP logging for debugging
```bash
curl -X PUT https://localhost:8443/api/settings/logging \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"logSMTP":true}' \
  -k
```

### Disable all protocol logging
```bash
curl -X PUT https://localhost:8443/api/settings/logging \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"logSMTP":false,"logPOP3":false,"logIMAP":false,"logDebug":false}' \
  -k
```

## Notes

- Settings take effect immediately (no restart required)
- Settings persist across server restarts
- Invalid values are silently ignored (use GET to verify changes)

## See Also

- [Settings Reference](../settings.md) — Detailed setting descriptions
- [Admin Panel](../admin-panel.md) — Change settings via web interface
