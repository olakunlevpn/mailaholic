# Architecture

System design and component overview.

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                         Mailaholic                           │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐       │
│  │     SMTP     │  │     IMAP     │  │     POP3     │       │
│  │   Port 25    │  │   Port 993   │  │   Port 995   │       │
│  │   Port 587   │  │              │  │              │       │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘       │
│         │                 │                 │                │
│         └─────────────────┼─────────────────┘                │
│                           │                                  │
│                    ┌──────┴──────┐                          │
│                    │    Core     │                          │
│                    │ Application │                          │
│                    └──────┬──────┘                          │
│                           │                                  │
│         ┌─────────────────┼─────────────────┐               │
│         │                 │                 │               │
│  ┌──────┴──────┐  ┌──────┴──────┐  ┌──────┴──────┐        │
│  │  WebAdmin   │  │ Persistence │  │    Cache    │        │
│  │   (HTTPS)   │  │  (SQLite)   │  │  (Memory)   │        │
│  └─────────────┘  └─────────────┘  └─────────────┘        │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

## Components

### Protocol Servers

| Component | Port | Description |
|-----------|------|-------------|
| SMTP | 25, 587 | Receive and send email |
| IMAP | 993 | Mailbox access (TLS) |
| POP3 | 995 | Mailbox download (TLS) |

All protocols support SSL/TLS encryption.

### WebAdmin Server

Embedded HTTPS server for administration.

**Technology:**
- cpp-httplib (single-header HTTP library)
- OpenSSL for TLS
- Self-signed certificate (auto-generated)

**Features:**
- Setup wizard
- Admin panel
- REST API
- Static asset serving

### Core Application

Central orchestration layer.

**Responsibilities:**
- Service lifecycle management
- Configuration loading
- Event dispatching
- Thread management

### Persistence Layer

Database abstraction for all data storage.

**Technology:**
- SQLite (default, embedded)
- MySQL/PostgreSQL (optional, external)

**Stored Data:**
- Domains and accounts
- Messages and folders
- Rules and filters
- Settings and state

### Cache Layer

In-memory caching for performance.

**Cached:**
- Domain lookups
- Account authentication
- Session tokens
- Configuration

## Data Flow

### Incoming Email

```
External Server
      │
      ▼
  SMTP Server (Port 25)
      │
      ├── Authentication check
      ├── Spam/virus scan
      ├── Recipient validation
      │
      ▼
  Delivery Queue
      │
      ├── Local delivery → Mailbox (Persistence)
      └── Remote relay → SMTP Client
```

### Outgoing Email

```
Email Client
      │
      ▼
  SMTP Server (Port 587)
      │
      ├── Authentication required
      ├── DKIM signing
      │
      ▼
  Delivery Queue
      │
      ▼
  SMTP Client → Remote Server
```

### Web Admin Request

```
Browser
      │
      ▼
  WebAdmin HTTPS Server
      │
      ├── Static assets → AssetHandler
      │
      ├── API requests → Route handlers
      │       │
      │       ├── AuthApi (login/logout)
      │       ├── AdminApi (domains/accounts)
      │       ├── SetupApi (wizard)
      │       ├── DKIMApi (keys)
      │       └── LetsEncryptApi (certs)
      │
      ▼
  Persistence Layer → Database
```

## Namespace Organization

### MA:: (Mailaholic Core)

Core business logic and infrastructure.

```cpp
namespace MA {
    class Application;      // Main application
    class Configuration;    // Settings
    class Domain;           // Email domain
    class Account;          // Email account
    class Message;          // Email message
    // ...
}
```

### WebAdmin::

Web server and API handlers.

```cpp
namespace WebAdmin {
    class WebServer;        // HTTP server
    class AuthApi;          // Authentication
    class AdminApi;         // Admin operations
    class SetupApi;         // Setup wizard
    class DKIMApi;          // DKIM management
    class LetsEncryptApi;   // Certificate mgmt
    class AssetHandler;     // Static files
}
```

## Threading Model

```
Main Thread
    │
    ├── WebAdmin Server Thread
    │       └── Request handler threads (pooled)
    │
    ├── SMTP Server Thread
    │       └── Connection handler threads
    │
    ├── IMAP Server Thread
    │       └── Connection handler threads
    │
    ├── POP3 Server Thread
    │       └── Connection handler threads
    │
    └── Background Workers
            ├── Delivery queue processor
            ├── Cleanup tasks
            └── Scheduled jobs
```

## Security Model

### Authentication

- Passwords: SHA-256 hashed
- Sessions: 24-hour expiring tokens
- Admin levels: User (0), Domain Admin (1), Server Admin (2)

### Network Security

- All protocols support TLS
- Self-signed cert for initial setup
- Let's Encrypt support for production
- DKIM signing for outbound mail

### Input Validation

- All API inputs validated
- SQL parameterization (no injection)
- Path traversal prevention in asset handler

## File Storage

```
C:\Mailaholic\
├── Mailaholic.exe          # Main executable
├── mailaholic.ini          # Configuration
├── data\
│   ├── mailaholic.db       # SQLite database
│   └── domains\
│       └── example.com\
│           └── accounts\
│               └── user\
│                   └── messages\   # Email storage
└── logs\
    └── mailaholic.log      # Application log
```

## Extension Points

### Adding New API Endpoint

1. Add handler method to appropriate Api class
2. Register route in `RegisterRoutes()`
3. Update documentation

### Adding New Protocol

1. Create protocol handler class
2. Add server thread in Application
3. Register with Configuration

### Adding New Setting

1. Add to Configuration class
2. Add persistence in PersistentConfiguration
3. Expose via AdminApi
4. Update frontend and documentation

## See Also

- [Building from Source](building.md) — Compilation guide
- [API Reference](../api/) — Endpoint documentation
