# Domains API

Manage email domains.

## Base URL

```
https://localhost:<port>/api/domains
```

## Authentication

All endpoints require Bearer token authentication.

```
Authorization: Bearer <token>
```

## Endpoints

### List Domains

Get all configured domains.

```
GET /api/domains
```

**Success Response (200):**
```json
{
  "success": true,
  "data": [
    {
      "id": 1,
      "name": "example.com",
      "isActive": true
    },
    {
      "id": 2,
      "name": "company.org",
      "isActive": true
    }
  ]
}
```

---

### Create Domain

Add a new email domain.

```
POST /api/domains
```

**Request Body:**
```json
{
  "name": "newdomain.com"
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
  "error": "Missing name"
}
```

**Error Response (500):**
```json
{
  "error": "Failed to create domain"
}
```

**Notes:**
- Domain is created as active by default
- Domain name should be valid (e.g., `example.com`, not `http://example.com`)

---

### Update Domain

Modify an existing domain.

```
PUT /api/domains/:id
```

**URL Parameters:**
- `id` — Domain ID (integer)

**Request Body:**
```json
{
  "isActive": false
}
```

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
  "error": "Domain not found"
}
```

---

### Delete Domain

Remove a domain and all its accounts.

```
DELETE /api/domains/:id
```

**URL Parameters:**
- `id` — Domain ID (integer)

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
  "error": "Domain not found"
}
```

**Warning:** Deleting a domain permanently removes all accounts and emails under that domain.

---

## Examples

### List all domains
```bash
curl https://localhost:8443/api/domains \
  -H "Authorization: Bearer $TOKEN" \
  -k
```

### Create a domain
```bash
curl -X POST https://localhost:8443/api/domains \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"name":"newdomain.com"}' \
  -k
```

### Disable a domain
```bash
curl -X PUT https://localhost:8443/api/domains/1 \
  -H "Authorization: Bearer $TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"isActive":false}' \
  -k
```

### Delete a domain
```bash
curl -X DELETE https://localhost:8443/api/domains/1 \
  -H "Authorization: Bearer $TOKEN" \
  -k
```

## Domain Object

| Field | Type | Description |
|-------|------|-------------|
| id | integer | Unique domain identifier |
| name | string | Domain name (e.g., "example.com") |
| isActive | boolean | Whether domain accepts mail |

## See Also

- [Accounts API](accounts.md) — Manage accounts under a domain
- [DKIM API](dkim.md) — Configure DKIM for a domain
