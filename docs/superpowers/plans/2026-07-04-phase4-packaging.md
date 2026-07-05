# Phase 4 — Packaging & Extended CI

**Goal:** Complete the rebrand (COM, service, installer) and make tools/installer CI-verifiable. Ship a fully installable Mailaholic distribution.

**Base commit:** `93c02a54` (Phase 3A complete)

---

## Part A — Extend CI (prerequisite for verifiable work)

### Task A1 — Add C# tools build job

Add a `tools` job to `.github/workflows/build.yml`:
- Install .NET Framework 4.8 SDK (or retarget tools to .NET 8)
- Build Administrator, DBSetup, DBSetupQuick
- Upload artifacts

**Files:**
- `.github/workflows/build.yml`
- `hmailserver/source/Tools/Administrator/Administrator.csproj` (retarget v4.5→v4.8 or net8.0-windows)
- `hmailserver/source/Tools/DBSetup/DBSetup.csproj`
- `hmailserver/source/Tools/DBSetupQuick/DBSetupQuick.csproj`

**Verification:** CI job builds all three tools green.

---

### Task A2 — Add InnoSetup installer build job

Add an `installer` job:
- Install InnoSetup 6
- Compile `hmailserver/installation/hMailServer64.iss`
- Upload installer artifact

**Dependency:** Needs server + tools artifacts from prior jobs.

**Verification:** CI produces `Mailaholic-Setup.exe` artifact.

---

## Part B — COM/Service Rebrand

### Task B1 — Regenerate COM GUIDs

Generate new GUIDs for all interfaces, coclasses, and type library in:
- `hmailserver/source/Server/hMailServer/hMailServer.idl`
- All `.rgs` files (ProgID + CLSID registry entries)

**Why:** Fresh GUIDs = clean break from hMailServer COM registration. Old scripts using `CreateObject("hMailServer.Application")` will fail clearly (object not found) rather than silently getting Mailaholic.

**ProgID changes:**
- `hMailServer.Application` → `Mailaholic.Application`
- `hMailServer.Utilities` → `Mailaholic.Utilities`
- (all others similarly)

**Verification:** CI builds green; COM registration test (new CI step) confirms objects creatable.

---

### Task B2 — Rename service

In `hmailserver/source/Server/hMailServer/hMailServer.cpp`:
- Service name `"hMailServer"` → `"Mailaholic"`
- Display name similarly
- Service control handler registration

In `hmailserver/source/Server/Common/Util/ServiceManager.cpp`:
- Service name references

**Verification:** CI builds green (can't verify service install without Windows, but build proves syntax).

---

### Task B3 — Rename registry paths

- `SOFTWARE\hMailServer` → `SOFTWARE\Mailaholic`
- Any other registry key references

**Files:** Search for `hMailServer` in registry-touching code.

---

## Part C — C# Tools Rebrand

### Task C1 — Rename Administrator tool

- Namespace `hMailServer.Administrator` → `Mailaholic.Administrator`
- Assembly name `hMailAdmin` → `MailaholicAdmin`
- COM interop reference (regenerate from new IDL)
- Window titles, about dialogs

**Verification:** Tools CI job builds green.

---

### Task C2 — Rename DBSetup tools

- Similar namespace/assembly renames
- Update any "hMailServer" strings in UI

---

## Part D — Installer Rebrand

### Task D1 — InnoSetup rename

In `hmailserver/installation/*.iss`:
- AppName, AppVerName → Mailaholic
- Default install dir → `{pf}\Mailaholic`
- Service name registration
- Registry paths
- Uninstaller entries
- File references (now Mailaholic.exe, MailaholicAdmin.exe, etc.)

---

### Task D2 — Installer admin password to PBKDF2

In `hmailserver/installation/hMailServerInnoExtension.iss`:
- Line 239: `GetMD5OfString` → use PBKDF2 (call into Mailaholic.exe or bundle a helper)
- INI writes to `Mailaholic.ini`

**Note:** InnoSetup Pascal doesn't have native PBKDF2. Options:
1. Shell out to `Mailaholic.exe /HashPassword <pw>` (new CLI mode)
2. Bundle a small helper exe
3. Write plaintext initially, require DBSetup to set real password

Recommend option 1: add `/HashPassword` CLI mode to server exe.

---

### Task D3 — VC++ runtime update

Current installer bundles VC142 CRT. v143 build needs ≥14.3x.
- Update `installation/section_files_*.iss` to include correct vcruntime DLLs
- Or switch to VC++ Redistributable installer merge module

---

## Part E — Database Schema (Optional)

### Task E1 — Table prefix `hm_` → `ma_`

- Update all SQL scripts in `hmailserver/source/DBScripts/`
- Update 154 references in `hmailserver/source/Server/Common/Persistence/*.cpp`

**Risk:** Large mechanical change. Consider deferring or making prefix configurable.

**Verification:** If done, add a CI step that runs schema against an ephemeral SQLite/MSSQL to verify syntax.

---

## Execution Order

1. A1, A2 — CI infrastructure (enables verification)
2. B1 — COM GUIDs (high impact, unlocks clean COM story)
3. B2, B3 — Service/registry
4. C1, C2 — Tools rebrand
5. D1-D3 — Installer rebrand
6. E1 — DB prefix (optional/deferred)

---

## Success Criteria

- CI builds server, tools, and installer
- All artifacts named Mailaholic
- COM objects register as `Mailaholic.*`
- Service installs as "Mailaholic"
- Installer produces working installation on fresh Windows
- Admin password stored as PBKDF2 from initial install

---

## Deferred

| Item | Why |
|------|-----|
| .NET 8 port | Major effort, separate phase |
| OAuth/modern auth | Separate feature work |
| SQL CE replacement | Separate DB modernization |
