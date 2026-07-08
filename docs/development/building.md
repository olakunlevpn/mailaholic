# Building from Source

Compile Mailaholic on Windows.

## Prerequisites

### Required Software

| Software | Version | Download |
|----------|---------|----------|
| Visual Studio 2022 | 17.x | [visualstudio.microsoft.com](https://visualstudio.microsoft.com/) |
| vcpkg | Latest | [github.com/microsoft/vcpkg](https://github.com/microsoft/vcpkg) |
| Git | 2.x | [git-scm.com](https://git-scm.com/) |

### Visual Studio Workloads

Install these workloads via Visual Studio Installer:
- Desktop development with C++
- Windows 10/11 SDK

## Setup

### 1. Clone Repository

```cmd
git clone https://github.com/olakunlevpn/mailaholic.git
cd mailaholic
```

### 2. Install vcpkg

```cmd
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
```

### 3. Install Dependencies

```cmd
C:\vcpkg\vcpkg install openssl:x64-windows
C:\vcpkg\vcpkg install boost:x64-windows
C:\vcpkg\vcpkg integrate install
```

This takes 10-30 minutes depending on your system.

## Building

### Via Visual Studio

1. Open `hmailserver/source/Server/Mailaholic/Mailaholic.sln`
2. Select **Release** | **x64** configuration
3. Build → Build Solution (Ctrl+Shift+B)

Output: `hmailserver/source/Server/Mailaholic/x64/Release/Mailaholic.exe`

### Via Command Line

```cmd
cd hmailserver\source\Server\Mailaholic
msbuild Mailaholic.sln /p:Configuration=Release /p:Platform=x64
```

### Via GitHub Actions

Push to the repository — CI builds automatically.

Download artifacts from Actions tab.

## Project Structure

```
hmailserver/
├── source/
│   ├── Server/
│   │   ├── Common/           # Shared code
│   │   │   ├── Application/  # Core application
│   │   │   ├── BO/           # Business objects
│   │   │   ├── Persistence/  # Database layer
│   │   │   ├── WebAdmin/     # Web server & APIs
│   │   │   └── ...
│   │   ├── Mailaholic/       # Main executable
│   │   ├── SMTP/             # SMTP protocol
│   │   ├── IMAP/             # IMAP protocol
│   │   └── POP3/             # POP3 protocol
│   └── WebAssets/            # Frontend files
│       ├── css/
│       ├── js/
│       └── *.html
└── docs/                     # Documentation
```

## Key Components

### WebAdmin Module

Location: `source/Server/Common/WebAdmin/`

| File | Purpose |
|------|---------|
| WebServer.cpp | HTTP server setup, SSL, routing |
| AuthApi.cpp | Authentication endpoints |
| AdminApi.cpp | Domain/account/settings CRUD |
| SetupApi.cpp | Setup wizard endpoints |
| DKIMApi.cpp | DKIM key generation |
| LetsEncryptApi.cpp | Certificate management |
| AssetHandler.cpp | Static file serving |

### Asset Embedding

Frontend assets are embedded at build time:

1. `embed-assets.ps1` reads WebAssets/
2. Generates `EmbeddedAssets.h` with base64-encoded files
3. `AssetHandler` serves from memory (no file I/O)

To update assets:
```cmd
powershell -ExecutionPolicy Bypass -File hmailserver\source\embed-assets.ps1
```

## Configuration

### Debug Build

```cmd
msbuild Mailaholic.sln /p:Configuration=Debug /p:Platform=x64
```

Debug builds include:
- Symbol files (.pdb)
- Debug assertions
- Verbose logging

### Preprocessor Defines

| Define | Effect |
|--------|--------|
| `_DEBUG` | Debug mode |
| `CPPHTTPLIB_OPENSSL_SUPPORT` | Enable HTTPS |

## Testing

### Run Locally

```cmd
cd x64\Release
Mailaholic.exe
```

Browser opens automatically to setup wizard.

### API Testing

```bash
# Get token
TOKEN=$(curl -s -X POST https://localhost:PORT/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{"email":"admin@test.com","password":"test"}' -k | jq -r '.data.token')

# Test endpoints
curl https://localhost:PORT/api/dashboard \
  -H "Authorization: Bearer $TOKEN" -k
```

## Troubleshooting

### "Cannot find openssl/ssl.h"

vcpkg not integrated. Run:
```cmd
C:\vcpkg\vcpkg integrate install
```

### Link errors (LNK2019)

Missing library. Verify vcpkg packages installed:
```cmd
C:\vcpkg\vcpkg list
```

### "Access denied" on port

Port in use or requires admin. Run as Administrator or change port.

### Asset changes not reflected

Regenerate embedded assets:
```cmd
powershell -ExecutionPolicy Bypass -File embed-assets.ps1
```
Then rebuild.

## See Also

- [Architecture](architecture.md) — System design overview
- [Installation Guide](../installation.md) — User installation
