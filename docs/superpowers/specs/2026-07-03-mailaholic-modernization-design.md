# Mailaholic — Full Modernization & Rebrand Design Spec

Date: 2026-07-03
Status: Draft — pending user review
Supersedes: `2026-07-03-security-modernization-design.md` (scope widened per user decisions)
Repo: https://github.com/olakunlevpn/mailaholic (hard fork of hmailserver/hmailserver master, June 2026)

## User decisions driving this spec

1. Goal: **full modernization + rebrand** to Mailaholic (not security-only).
2. Compatibility: **fresh installs only** — no upgrade path from hMailServer; legacy password/crypto paths are removed, not migrated.
3. Rebrand depth: **full deep rename** — service name, registry, COM API, namespaces, installer, database.
4. Build/test: **GitHub Actions Windows CI** (user develops on macOS; no local Windows box).
5. Extra scope (user AFK; defaults chosen, revisit at review): VS2022 + InnoSetup 6 toolchain **in**, strict modern TLS defaults **in**, .NET 8 port of admin tools **deferred**, OAUTHBEARER/XOAUTH2 **deferred**.

## Background

Upstream README declares the project unmaintained and warns about SHA1 and outdated OpenSSL. Audit of master (June 2026) shows dependencies are already current (OpenSSL 3.5.5, Boost 1.90, libpq 15.18). The real weaknesses are in code and defaults:

1. Account passwords: plaintext supported (compared case-insensitively), unsalted MD5 accepted, best scheme is single-round salted SHA-256 — no key stretching. (`Common/Util/PasswordValidator.cpp:149-188`, `Common/Util/Hashing/HashCreator.cpp:28-40`)
2. DKIM signs and verifies rsa-sha1 (per-domain flag `ASDKIMSHA1`), violating RFC 8301. (`Common/AntiSpam/DKIM/DKIM.cpp:137,368,452`, `Common/BO/Domain.h`)
3. Hash comparisons are ordinary string equality, not constant-time.
4. Reversible secrets (FetchAccount/Route relay passwords) use Blowfish with MD5-derived keys. (`Common/Util/Crypt.cpp:33,42,70,110`)
5. README build instructions set `OPENSSL_TLS_SECURITY_LEVEL=0`, deliberately re-enabling weak ciphers.
6. Toolchain targets VS2019; GitHub retired windows-2019 runners, so CI requires VS2022 retarget anyway.

## Goals

- Mailaholic 1.0.0: standalone product, coexists on a machine with an existing hMailServer install (separate service, registry, COM CLSIDs, database).
- Account passwords stored only with a modern KDF; no weak scheme accepted.
- DKIM complies with RFC 8301.
- Strict modern TLS defaults out of the box.
- Every change compiles and passes self-tests on Windows CI before it is considered done.

## Non-Goals (deferred)

- .NET 8 port of Administrator GUI / DBSetup (stays .NET Framework this cycle).
- OAUTHBEARER / XOAUTH2 SASL mechanisms.
- Running the 500+ scenario regression suite in CI (needs registered COM + database on the runner; stretch goal after Phase 2).
- Upstreaming fixes to hmailserver/hmailserver.
- Any migration/import tool from hMailServer databases.

## Approach

Phased, CI-gated. Each phase lands as a reviewed PR branch onto `master` of mailaholic; CI must be green before the next phase starts. Rename comes AFTER security work, because a 1,364-file mechanical rename with no working baseline would make regressions undetectable.

### Phase 1 — Repo + toolchain + CI baseline

- Local git: `origin` → `github.com/olakunlevpn/mailaholic`; keep `upstream` → hmailserver/hmailserver for reference.
- Retarget solution/projects to VS2022 (v143 toolset, latest Windows SDK). C++ language standard stays as-is unless the compiler forces changes.
- GitHub Actions workflow `build.yml`: `windows-2022` runner; install Perl + NASM; build OpenSSL 3.5.5 (without `OPENSSL_TLS_SECURITY_LEVEL=0`) and Boost 1.90; cache both with `actions/cache` keyed on version; then `build/build.ps1` + self-tests. First run ~40-60 min, cached runs minutes.
- Exit criterion: unmodified (retargeted) code builds green.

### Phase 2 — Security core (clean break)

**2a. Password hashing: PBKDF2-HMAC-SHA256**
- Parameters: 600,000 iterations (OWASP), 16-byte salt from `RAND_bytes`.
- Via OpenSSL `PKCS5_PBKDF2_HMAC` — already a dependency. (Argon2/bcrypt rejected: new vendored library in a high-friction Windows build for marginal gain over tuned PBKDF2.)
- Storage format: `$PBKDF2$<iterations>$<salt-base64>$<hash-base64>` — self-describing, iteration count tunable later without code archaeology. Fits `accountpassword varchar(255)` on all three DB backends; no schema change.
- `PasswordValidator::ValidatePassword` accepts ONLY this format for account passwords (AD/SSPI path unchanged). Plaintext, MD5, SHA-256, and Blowfish account-password branches are deleted. A stored value not in PBKDF2 format fails closed with a specific error log.
- All server-side write paths (COM, Administrator, DBSetup default accounts) produce PBKDF2. COM `Account.PasswordEncryption` becomes inert: always reports PBKDF2; setter accepted but ignored (kept so the property itself doesn't break scripts syntactically).

**2b. DKIM: RFC 8301**
- Signing always emits `a=rsa-sha256`; `ASDKIMSHA1` flag ignored (COM property remains settable, coerced).
- Verification: `a=rsa-sha1` → PermFail. SHA1 branches removed from header/body hash validation.

**2c. Constant-time comparisons**
- `HashCreator::ValidateHash` and `Crypt::Validate` use `CRYPTO_memcmp`.

**2d. Reversible secrets: Blowfish → AES-256-GCM**
- FetchAccount/Route relay passwords must be replayed to external servers, so they stay reversible — but via EVP AES-256-GCM, random 12-byte IV per encryption. Storage format `$AESGCM$<iv-b64>$<tag-b64>$<ct-b64>`. Fresh installs → no legacy Blowfish blobs to read; Blowfish code deleted.
- Key material: hMailServer's Blowfish used a key hardcoded in the binary (obfuscation, not encryption). Mailaholic generates a random 32-byte secret on first server start (`RAND_bytes`), stored as `Security\SecretKey` (hex) in the INI; AES-GCM uses it directly. INI file must be ACL-protected to Administrators/SYSTEM (installer duty, Phase 4).
- COM `Utilities.MD5()` / `BlowfishEncrypt` / `BlowfishDecrypt`: in Phase 2 the methods stay on the COM interface (vtable/IDL untouched) but return a scripting error ("removed for security reasons"); Phase 3's deep rename deletes them from the IDL when all GUIDs regenerate anyway. `Utilities.SHA256()` stays.

**2f. INI administrator password: MD5 → PBKDF2** *(added 2026-07-03 after installation audit)*
- Today: installer writes `Security\AdministratorPassword` as **unsalted MD5** (`hMailServerInnoExtension.iss:239-241`); server validates it in `COM/COMAuthentication.cpp:38,78` via length-based hash detection (`Crypt::GetHashType`).
- Phase 2 server side: `IniFileSettings::SetAdministratorPassword` writes PBKDF2 format; validation accepts only `$PBKDF2$…`. Consequence: installers older than Phase 4 produce a non-working admin password — acceptable pre-release; Phase 4 installer writes PBKDF2 (or defers hashing to first DBSetup run).

**2g. Self-test hardening** *(carried from Phase 1)*
- `hMailServer.exe /Test` crashes 0xC0000005 in ~0.07s on the CI runner (no INI/registry/DB present). Phase 2 adds a `/SelfTest` mode: runs only environment-independent testers (Mime, StringParser, Formatter, DateTime, Base64, ModifiedUTF7, HashCreator/PBKDF2, AES-GCM, AddresslistParser, IMAPSimpleCommandParser), no `InitInstance`, no `StartServers`, no DNS testers, prints results to stdout, exit code 0/1. CI switches to `/SelfTest` and drops `continue-on-error` — the step becomes a required gate.

**2h. OpenSSL build flags**
- After HashCreator/DKIM migrate to EVP APIs, drop `--api=1.1.1 no-deprecated` from the OpenSSL Configure line (README + workflow) and bump the CI cache key suffix to `-v2`.

**2e. Strict TLS defaults**
- Minimum TLS 1.2, TLS 1.3 enabled, OpenSSL security level ≥ 2, modern cipher list in `SslContextInitializer`. No `--api=1.1.1`/security-level-0 build flags. Admin can still tune versions via existing TLSOptions settings, but cannot go below TLS 1.2.

### Phase 3 — Deep rename to Mailaholic

Mechanical but staged into CI-green commits:

1. **COM API**: new ProgIDs `Mailaholic.*` and **freshly generated GUIDs** for all CLSIDs/IIDs/TypeLib (required for side-by-side coexistence with installed hMailServer). IDL, registration code, C# interop references.
2. **Service + OS integration**: service name `Mailaholic`, event log source, registry `HKLM\SOFTWARE\Mailaholic`, INI `mailaholic.ini`, default paths (`C:\Program Files\Mailaholic`), data directories.
3. **Namespaces**: C++ `HM` → `MA`; C# `hMailServer` → `Mailaholic`. Pure mechanical rename, single commit each, verified by CI.
4. **Database**: default database name `mailaholic`; table prefix `hm_` → `ma_` across DBScripts + DAL. (Open question below.)
5. **Strings/branding**: product name, banners (SMTP/IMAP/POP3 greetings), Administrator GUI text, docs, README (deprecation notice removed, own project statement written).

### Phase 4 — Packaging + release

- InnoSetup 6 installer producing `Mailaholic-1.0.0.exe`; new AppId GUID (`section_setup_64.iss` has none today); installs alongside hMailServer without conflict.
- Version scheme: SemVer starting 1.0.0; assembly/file versions wired to one source of truth (currently hardcoded `1.0.0-B0` in `section_setup_64.iss`).
- CI release job: tag → build server + Tools + installer → artifact.

*Installation-audit additions (2026-07-03):*
- **Tools sln**: retarget C# projects from .NET Framework 4.5 (`Administrator.csproj:34` — VS2022 has no 4.5 targeting pack) to 4.8; Tools build requires registered COM server (`Administrator.csproj:970` `COMReference hMailServer`) — CI must run `hMailServer.exe /Register` before building Tools, or convert to tlbimp against the .tlb.
- **VC++ runtime**: installer ships stale 14.2-era CRT copies (`Microsoft.VC142.CRT/`, UCRT from SDK 10240) — v143 binaries need ≥14.3x; replace with current redist copies or chain official `vc_redist.x64.exe`.
- **Installer writes PBKDF2 admin password** (see 2f); ACL-protect the INI.
- **Drop XP-era cruft**: isxdl dependency downloads (IE6/MDAC/JET/.NET2.0), `System Files\dnsapi.dll`, `atl70.dll`, legacy `Shared.iss` 32-bit script, hSMTPServer/hPOP3Server/bundled-MySQL uninstall entries. MinVersion → Windows 10 (10.0.17763).
- **Default database**: bundled SQL Server Compact 3.5 is EOL (2013). Decision needed: keep (works) vs migrate internal DB to SQLite (larger change — separate spec if chosen).
- **MySQL client**: `libmysql.dll` loaded at runtime from Bin (`MySQLInterface.cpp:64`), never shipped; vendored `libraries/libmysql-5.7.38` is EOL. Document requirement or ship a maintained client.

## Error handling

- PBKDF2/AESGCM format parse failure → fail closed, specific error log, never fallback to weaker interpretation.
- `RAND_bytes` failure → hard error; password change/encryption rejected (never silent weak salt/IV).
- AES-GCM tag verification failure → decryption error, fetch/route attempt aborted with logged error.
- Legacy-format value encountered at runtime → auth/decrypt fails with log message naming the incompatibility (fresh-install product; points user at password reset).

## Testing

- Extend self-test pattern (`HashCreatorTester` et al.): PBKDF2 known-answer vectors (RFC 6070-style), generate/validate round-trip, malformed-format rejection, AES-GCM round-trip + tamper rejection, DKIM signer emits rsa-sha256, rsa-sha1 verification PermFails.
- CI gate per phase: compile + self-tests.
- Rename phases verified by CI build + self-tests; COM registration smoke-tested via installer stretch goal.
- Regression suite (`test/RegressionTests/`) on CI: stretch goal after Phase 2 (needs COM registration + SQL on runner).

## Resolved decisions (user approved 2026-07-03)

1. Table prefix `hm_` → `ma_`: **in scope** (full deep rename confirmed).
2. C++ namespace target: **`MA`** confirmed.
3. Protocol banners: say "Mailaholic", **no version number disclosed** (hardening default).
4. Deferrals confirmed: .NET 8 admin tools port + OAuth (XOAUTH2) out of this cycle.
