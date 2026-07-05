# Phase 3A — CI-Verifiable Rename (Mailaholic)

**Goal:** Rebrand C++ server code (namespace, copyright, product strings) while keeping every commit verifiable via the existing Windows CI. COM/IDL GUIDs, C# tools, installer, DB schema, and service name are deferred to Phase 4 (requires extended CI).

**Base commit:** `55711a0f` (Phase 2 complete)

---

## Constraints

- CI builds only the server `.exe`; C# tools and installer are NOT built.
- /WX (warnings-as-errors) is on — any unused symbol breaks the build.
- COM IDL/RGS contain GUIDs and ProgIDs (`hMailServer.*`). Changing GUIDs breaks existing COM clients; changing ProgIDs without new GUIDs breaks typelib resolution. Both are Phase 4 (when we add COM registration tests).
- Service name (`hMailServer`) is embedded in hMailServer.cpp and registry paths; changing it breaks existing installs (acceptable per spec) but can't be CI-verified. Phase 4.
- DB table prefix `hm_` in SQL scripts and 154 Persistence-layer references — schema migration is Phase 4.

---

## Tasks

### Task 1 — Copyright headers (mechanical, high-volume)

Update every C++ source file's copyright header:
```cpp
// Copyright (c) 2010 Martin Knafve / hMailServer.com.
// http://www.hmailserver.com
```
→
```cpp
// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic
```

**Scope:** ~1225 files (every `.cpp`/`.h` in `hmailserver/source/Server`).

**Verification:** CI builds green (no file excluded, no syntax error from bad replacement).

**Commit:** `chore: update copyright headers to Mailaholic`

---

### Task 2 — C++ namespace `HM` → `MA`

Global search-replace across the server:
1. `namespace HM` → `namespace MA`
2. `HM::` → `MA::`
3. `using namespace HM;` → `using namespace MA;`

**Scope:** 833 files with `namespace HM`; 1215 `HM::` occurrences.

**Order:** Do the long forms first (`namespace HM`, `using namespace HM`) then the prefix form (`HM::`). This avoids partial replacements.

**Verification:** CI builds green (linker finds everything, no unresolved symbols).

**Commit:** `refactor: rename namespace HM to MA`

---

### Task 3 — Product strings in code

Replace internal string literals that name the product but are NOT externally exposed (COM ProgIDs, service name):

| Pattern | Replacement | Files |
|---------|-------------|-------|
| `"hMailServer"` in log/error strings | `"Mailaholic"` | ~20 |
| `hMailServer.ini` literal | `Mailaholic.ini` | IniFileSettings.cpp |
| `hMailServer.com` URL | `github.com/olakunlevpn/mailaholic` | error messages |

**Exclusions (Phase 4):**
- Service name registration (`"hMailServer"` in hMailServer.cpp, ServiceManager.cpp)
- COM ProgIDs (`"hMailServer.Application"` etc.)
- Installer/DB script strings

**Verification:** CI builds green; no change to enum values, GUIDs, or COM interfaces.

**Commit:** `chore: rename product strings to Mailaholic`

---

### Task 4 — Build artifact + workflow rename

1. Rename the output artifact from `hMailServer.exe` to `Mailaholic.exe` in `hMailServer.vcxproj` (`<TargetName>`).
2. Update `.github/workflows/build.yml` artifact name and paths.
3. Update `README.md` build instructions.

**Verification:** CI produces `Mailaholic.exe`; `/SelfTest` still passes (binary is byte-different but functionally identical).

**Commit:** `build: rename output to Mailaholic.exe`

---

### Task 5 — Rename vcxproj file

1. Rename `hMailServer.vcxproj` → `Mailaholic.vcxproj` (and `.vcxproj.filters`).
2. Update `build/build.ps1` reference.
3. Update any solution file if present.

**Verification:** CI builds using the new project file name.

**Commit:** `build: rename vcxproj to Mailaholic`

---

### Task 6 — Rename source directory (optional, high churn)

Rename `hmailserver/source/Server/hMailServer/` → `hmailserver/source/Server/Mailaholic/`.

This touches:
- All include paths in the vcxproj
- CI workflow paths
- stdafx.h references

**Risk:** Large diff, many merge conflicts if other work lands. Consider deferring to Phase 4.

**Verification:** CI builds green.

**Commit:** `refactor: rename hMailServer source directory to Mailaholic`

---

## Execution Order

1. T1 (copyright) — cosmetic, low risk
2. T2 (namespace) — functional but mechanical
3. T3 (product strings) — small, careful
4. T4 (artifact) — changes binary name
5. T5 (vcxproj) — changes project file name
6. T6 (source dir) — optional, defer if risky

Each task = one commit, push, CI-green before next.

---

## Deferred to Phase 4

| Item | Why |
|------|-----|
| COM GUIDs / ProgIDs | Needs COM registration test in CI |
| C# tools namespace (`hMailServer.Application`) | Needs C# build in CI |
| Service name (`hMailServer`) | Needs service-install test |
| Registry paths (`SOFTWARE\hMailServer`) | Tied to service |
| DB table prefix `hm_` | Needs DB schema test |
| Installer strings | InnoSetup not in CI yet |
| .NET 8 port | Separate major effort |

---

## Success Criteria

- Every Phase 3A commit builds green on Windows CI.
- `/SelfTest` passes after each commit.
- No COM IDL/RGS touched; no GUID changed.
- README, build scripts, artifact all reflect "Mailaholic".
