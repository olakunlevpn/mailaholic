# Installation Guide

## System Requirements

- Windows 10/11 or Windows Server 2016+
- 2 GB RAM minimum (4 GB recommended)
- 500 MB disk space
- Ports 25, 587, 993, 995 available
- Static IP address (for production use)

## Download

Download the latest release from [GitHub Releases](https://github.com/olakunlevpn/mailaholic/releases).

Or build from source — see [Building from Source](development/building.md).

## Installation Steps

### 1. Extract Files

Extract the downloaded archive to your preferred location:
```
C:\Mailaholic\
```

### 2. Run Mailaholic

Double-click `Mailaholic.exe` or run from command line:
```cmd
Mailaholic.exe
```

The web interface opens automatically in your default browser.

### 3. Complete Setup Wizard

The setup wizard guides you through initial configuration:

1. **Domain Configuration** — Enter your mail server domain (e.g., `mail.example.com`)
2. **Usage Type** — Select Personal, Organization, or High Volume
3. **Admin Account** — Create your administrator email and password
4. **Auto-Configuration** — System checks ports and generates certificates
5. **DNS Records** — Copy the displayed records to your DNS provider
6. **Complete** — Setup finishes and creates your domain/account

### 4. Configure DNS

Add these DNS records at your domain registrar:

| Type | Name | Value |
|------|------|-------|
| A | mail.example.com | YOUR_SERVER_IP |
| MX | example.com | mail.example.com (priority 10) |
| TXT | example.com | v=spf1 mx ~all |
| TXT | mail._domainkey.example.com | (DKIM key from admin panel) |
| TXT | _dmarc.example.com | v=DMARC1; p=quarantine |

### 5. Verify

Test your setup:
1. Log into the admin panel with your admin account
2. Send a test email to an external address
3. Check [mail-tester.com](https://www.mail-tester.com) for deliverability score

## Running as a Service

To run Mailaholic as a Windows service:

```cmd
sc create Mailaholic binPath= "C:\Mailaholic\Mailaholic.exe --service" start= auto
sc start Mailaholic
```

## Firewall Configuration

Allow these ports through Windows Firewall:

| Port | Protocol | Purpose |
|------|----------|---------|
| 25 | TCP | SMTP (receiving mail) |
| 587 | TCP | SMTP Submission (sending mail) |
| 993 | TCP | IMAPS (secure IMAP) |
| 995 | TCP | POP3S (secure POP3) |

```cmd
netsh advfirewall firewall add rule name="Mailaholic SMTP" dir=in action=allow protocol=TCP localport=25
netsh advfirewall firewall add rule name="Mailaholic Submission" dir=in action=allow protocol=TCP localport=587
netsh advfirewall firewall add rule name="Mailaholic IMAPS" dir=in action=allow protocol=TCP localport=993
netsh advfirewall firewall add rule name="Mailaholic POP3S" dir=in action=allow protocol=TCP localport=995
```

## Next Steps

- [Quick Start](quickstart.md) — Set up your first email account
- [Admin Panel](admin-panel.md) — Learn the administration interface
- [SSL Certificates](ssl-certificates.md) — Upgrade to Let's Encrypt
