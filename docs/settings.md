# Settings Reference

Complete reference for all Mailaholic configuration options.

## SMTP Settings

### Enabled
- **Type:** Boolean
- **Default:** true
- **Description:** Enable or disable the SMTP service. When disabled, the server will not accept incoming mail or allow sending.

### Max Message Size
- **Type:** Integer (bytes)
- **Default:** 20971520 (20 MB)
- **Range:** 1024 - 104857600 (1 KB - 100 MB)
- **Description:** Maximum size of a single email message including attachments. Messages exceeding this limit are rejected.

### Max Recipients
- **Type:** Integer
- **Default:** 100
- **Range:** 1 - 1000
- **Description:** Maximum number of recipients allowed in a single message. Prevents mass-mailing abuse.

## Logging Settings

### Enabled
- **Type:** Boolean
- **Default:** true
- **Description:** Master switch for all logging. When disabled, no logs are written.

### Log SMTP
- **Type:** Boolean
- **Default:** false
- **Description:** Log all SMTP protocol conversations. Useful for debugging delivery issues. Generates significant disk usage.

### Log POP3
- **Type:** Boolean
- **Default:** false
- **Description:** Log all POP3 protocol conversations. Useful for debugging client connection issues.

### Log IMAP
- **Type:** Boolean
- **Default:** false
- **Description:** Log all IMAP protocol conversations. Useful for debugging synchronization issues.

### Debug Logging
- **Type:** Boolean
- **Default:** false
- **Description:** Enable verbose debug output. Only enable when troubleshooting specific issues. High performance impact.

## Configuration File

Settings are stored in `mailaholic.ini` in the installation directory.

Example:
```ini
[Settings]
UseSMTP=1
MaxMessageSize=20971520
MaxSMTPRecipients=100
UseLogging=1
LogSMTPConversations=0
LogPOP3Conversations=0
LogIMAPConversations=0
LogDebug=0
```

## Applying Changes

Settings changed via the admin panel take effect immediately. Settings changed by editing `mailaholic.ini` require a server restart.

## See Also

- [Admin Panel](admin-panel.md) — Change settings via web interface
- [Settings API](api/settings.md) — Change settings programmatically
