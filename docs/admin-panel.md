# Admin Panel

The Mailaholic admin panel provides a web-based interface for managing your email server.

## Accessing the Admin Panel

After setup completes, access the admin panel at:
```
https://localhost:<port>/admin.html
```

The port is randomly assigned during startup and displayed in the console.

## Authentication

Log in with an administrator account. Administrator accounts are created:
- During initial setup (first admin)
- Via the Accounts page with Admin Level set to Server Admin

Session tokens expire after 24 hours.

## Dashboard

The dashboard displays:

| Metric | Description |
|--------|-------------|
| Domains | Total number of email domains |
| Accounts | Total email accounts across all domains |
| Server Status | Current server state (running/stopped) |

## Domains

### Viewing Domains

Click **Domains** in the sidebar to see all configured domains.

Each domain shows:
- Domain name (click to view accounts)
- Status (Active/Inactive)
- Actions (Delete)

### Adding a Domain

1. Click **Add Domain**
2. Enter domain name (e.g., `example.com`)
3. Click **Save**

The domain is created as active by default.

### Deleting a Domain

1. Click the **Delete** button next to the domain
2. Confirm deletion

**Warning:** Deleting a domain removes all accounts and emails under that domain.

## Accounts

### Viewing Accounts

1. Click **Domains**
2. Click a domain name
3. View all accounts for that domain

Each account shows:
- Email address
- Status (Active/Inactive)
- Actions (Delete)

### Adding an Account

1. Navigate to a domain's account list
2. Click **Add Account**
3. Enter full email address (e.g., `user@example.com`)
4. Enter password
5. Click **Save**

### Deleting an Account

1. Click the **Delete** button next to the account
2. Confirm deletion

## Settings

### SMTP Settings

| Setting | Description | Default |
|---------|-------------|---------|
| Enabled | SMTP service on/off | On |
| Max Message Size | Maximum email size in bytes | 20971520 (20 MB) |
| Max Recipients | Maximum recipients per message | 100 |

### Logging Settings

| Setting | Description | Default |
|---------|-------------|---------|
| Enabled | Master logging switch | On |
| Log SMTP | Log SMTP conversations | Off |
| Log POP3 | Log POP3 conversations | Off |
| Log IMAP | Log IMAP conversations | Off |
| Debug Logging | Verbose debug output | Off |

**Note:** Enable protocol logging for troubleshooting only. It generates large log files.

## Navigation

| Menu Item | Function |
|-----------|----------|
| Dashboard | Overview and statistics |
| Domains | Domain and account management |
| Settings | Server configuration |
| Logout | End admin session |

## Keyboard Shortcuts

None currently implemented.

## See Also

- [Settings Reference](settings.md) — Detailed settings documentation
- [Domains API](api/domains.md) — Programmatic domain management
- [Accounts API](api/accounts.md) — Programmatic account management
