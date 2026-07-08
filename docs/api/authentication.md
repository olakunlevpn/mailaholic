# Authentication API

All admin API endpoints require authentication via Bearer token.

## Base URL

```
https://localhost:<port>/api/auth
```

## Endpoints

### Login

Authenticate and receive a session token.

```
POST /api/auth/login
```

**Request Body:**
```json
{
  "email": "admin@example.com",
  "password": "your-password"
}
```

**Success Response (200):**
```json
{
  "success": true,
  "data": {
    "token": "a1b2c3d4e5f6..."
  }
}
```

**Error Response (401):**
```json
{
  "error": "Invalid credentials"
}
```

**Error Response (403):**
```json
{
  "error": "Not an administrator"
}
```

**Notes:**
- Only accounts with Admin Level > 0 can log in
- Token expires after 24 hours

---

### Logout

Invalidate the current session token.

```
POST /api/auth/logout
```

**Headers:**
```
Authorization: Bearer <token>
```

**Success Response (200):**
```json
{
  "success": true,
  "data": {}
}
```

---

### Check Session

Verify if the current token is valid.

```
GET /api/auth/check
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
    "valid": true,
    "userId": 1
  }
}
```

**Error Response (401):**
```json
{
  "error": "Invalid or expired token"
}
```

---

## Using Authentication

Include the token in the Authorization header for all protected endpoints:

```bash
curl https://localhost:PORT/api/domains \
  -H "Authorization: Bearer a1b2c3d4e5f6..."
```

## Token Storage

- Tokens are stored server-side in memory
- Tokens survive server restart: No (sessions cleared)
- Token format: 32-character hex string

## Security Considerations

- Always use HTTPS (self-signed cert accepted with `-k` flag)
- Tokens are single-use per session
- Failed login attempts are not rate-limited (implement at firewall level)
- Store tokens securely client-side (localStorage for web, secure storage for mobile)

## Example: Full Login Flow

```bash
# Login
TOKEN=$(curl -s -X POST https://localhost:8443/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{"email":"admin@example.com","password":"secret"}' \
  -k | jq -r '.data.token')

# Use token for subsequent requests
curl https://localhost:8443/api/dashboard \
  -H "Authorization: Bearer $TOKEN" \
  -k

# Logout when done
curl -X POST https://localhost:8443/api/auth/logout \
  -H "Authorization: Bearer $TOKEN" \
  -k
```
