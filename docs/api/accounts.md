# Accounts API

Manage email accounts within domains.

## Base URL

```
https://localhost:<port>/api/accounts
```

## Authentication

All endpoints require Bearer token authentication.

```
Authorization: Bearer <token>
```

## Endpoints

### List Accounts

Get all accounts for a specific domain.

```
GET /api/accounts?domain_id=:id
```

**Query Parameters:**
- `domain_id` (required) — Domain ID to list accounts for

**Success Response (200):**
```json
{
  "success": true,
  "data": [
    {
      "id": 1,
      "address": "admin@example.com",
      "isActive": true,
      "adminLevel": 2
    },
    {
      "id": 2,
      "address": "user@example.com",
      "isActive": true,
      "adminLevel": 0
    }
  ]
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

### Create Account

Add a new email account.

```
POST /api/accounts
```

**Request Body:**
```json
{
  "domain_id": 1,
  "address": "newuser@example.com",
  "password": "secure-password"
}
```

**Success Response (200):**
```json
{
  "success": true,
  "data": {
    "id": 3
  }
}
```

**Error Response (400):**
```json
{
  "error": "Missing domain_id"
}
```

```json
{
  "error": "Missing address"
}
```

```json
{
  "error": "Missing password"
}
```

**Notes:**
- Password is hashed using SHA-256 before storage
- Account is created as active by default
- Admin level defaults to 0 (normal user)

---

### Update Account

Modify an existing account.

```
PUT /api/accounts/:id
```

**URL Parameters:**
- `id` — Account ID (integer)

**Request Body:**
```json
{
  "isActive": false,
  "password": "new-password"
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

**Error Response (404):**
```json
{
  "error": "Account not found"
}
```

**Notes:**
- Empty password string is ignored (password not changed)
- Cannot change email address — delete and recreate instead

---

### Delete Account

Remove an email account.

```
DELETE /api/accounts/:id
```

**URL Parameters:**
- `id` — Account ID (integer)

**Success Response (200):**
```json
{
  "success": true,
  "data": {}
}
```

**Error Response (404):**
```json
{
  "error": "Account not found"
}
```

**Warning:** Deleting an account permanently removes all emails in that mailbox.

---

## Examples

### List accounts for a domain
```bash
curl "https://localhost:8443/api/accounts?domain_id=1" \
  -H "Authorization: Bearer $TOKEN" \
  -k
```

### Create an account
```bash
curl -X POST https://localhost:8443/api/accounts \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"domain_id":1,"address":"user@example.com","password":"secret123"}' \
  -k
```

### Change password
```bash
curl -X PUT https://localhost:8443/api/accounts/2 \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"password":"newpassword456"}' \
  -k
```

### Disable an account
```bash
curl -X PUT https://localhost:8443/api/accounts/2 \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"isActive":false}' \
  -k
```

### Delete an account
```bash
curl -X DELETE https://localhost:8443/api/accounts/2 \
  -H "Authorization: Bearer $TOKEN" \
  -k
```

## Account Object

| Field | Type | Description |
|-------|------|-------------|
| id | integer | Unique account identifier |
| address | string | Full email address |
| isActive | boolean | Whether account can send/receive |
| adminLevel | integer | 0=User, 1=Domain Admin, 2=Server Admin |

## Admin Levels

| Level | Name | Permissions |
|-------|------|-------------|
| 0 | Normal User | Access own mailbox only |
| 1 | Domain Admin | Manage accounts in their domain |
| 2 | Server Admin | Full administrative access |

## See Also

- [Domains API](domains.md) — Manage domains
- [Authentication API](authentication.md) — Login with account credentials
