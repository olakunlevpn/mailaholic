# Quick Start Guide

Get your email server running in 5 minutes.

## Prerequisites

- Mailaholic installed and running
- Domain name pointing to your server
- Access to your DNS settings

## Step 1: Complete Setup Wizard

When you first run Mailaholic, the setup wizard opens automatically.

1. Enter your mail server domain (e.g., `mail.yourdomain.com`)
2. Select "Personal / Small Team" for most users
3. Create your admin account (e.g., `admin@yourdomain.com`)
4. Wait for auto-configuration to complete
5. Note the DNS records displayed

## Step 2: Configure DNS

Add the DNS records shown in the wizard to your domain registrar.

Minimum required:
- **A record** — Points your mail domain to server IP
- **MX record** — Tells other servers where to deliver mail

Recommended:
- **SPF record** — Prevents email spoofing
- **DKIM record** — Signs outgoing emails
- **DMARC record** — Policy for failed authentication

## Step 3: Create Email Account

1. Open the admin panel (auto-redirects after setup)
2. Log in with your admin credentials
3. Click **Domains** in the sidebar
4. Click your domain name
5. Click **Add Account**
6. Enter email address and password
7. Click **Save**

## Step 4: Configure Email Client

Use these settings in your email client:

**Incoming Mail (IMAP)**
- Server: `mail.yourdomain.com`
- Port: `993`
- Security: `SSL/TLS`
- Username: `your@yourdomain.com`
- Password: Your account password

**Outgoing Mail (SMTP)**
- Server: `mail.yourdomain.com`
- Port: `587`
- Security: `STARTTLS`
- Username: `your@yourdomain.com`
- Password: Your account password

## Step 5: Send Test Email

1. Compose a new email in your client
2. Send to an external address (Gmail, Outlook, etc.)
3. Check if it arrives
4. Reply to verify receiving works

## Troubleshooting

**Email not sending?**
- Check firewall allows port 587 outbound
- Verify DNS MX record is correct
- Check admin panel dashboard for errors

**Email not receiving?**
- Verify port 25 is open and not blocked by ISP
- Check MX record points to correct IP
- Review server logs in admin panel

**SSL certificate errors?**
- Setup uses self-signed certificate initially
- Upgrade to Let's Encrypt for trusted certificate
- See [SSL Certificates](ssl-certificates.md)

## Next Steps

- [Admin Panel](admin-panel.md) — Full administration guide
- [DKIM Setup](dkim.md) — Improve deliverability
- [Settings Reference](settings.md) — All configuration options
