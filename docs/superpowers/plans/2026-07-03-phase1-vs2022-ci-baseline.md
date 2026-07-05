# Mailaholic Phase 1: Repo + VS2022 Retarget + CI Baseline — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Get the unmodified (toolchain-retargeted) codebase building green on GitHub Actions Windows CI against the mailaholic repo, so later security and rename phases have a regression-detecting baseline.

**Architecture:** Point local git at `olakunlevpn/mailaholic` (keep upstream for reference). Retarget the two C++ projects from v142 (VS2019) to v143 (VS2022) because GitHub retired windows-2019 runners. Add one workflow that builds OpenSSL 3.5.5, Boost 1.90.0, and PostgreSQL libpq 15.18 on a `windows-2022` runner (cached by version key), then compiles the server solution via the existing `build/build.ps1`.

**Tech Stack:** MSVC v143 (VS2022), MSBuild, PowerShell build scripts, GitHub Actions (`windows-2022`), OpenSSL 3.5.5, Boost 1.90.0, PostgreSQL 15.18 (libpq only), Strawberry Perl (preinstalled on runner).

## Global Constraints

- Dependency versions and folder names are hardcoded in `hmailserver/source/Server/hMailServer/hMailServer.vcxproj`: `openssl-3.5.5`, `boost_1_90_0`, `postgresql-15.18` under `%hMailServerLibs%`. CI must use these exact directory names.
- Platform is x64 only. Solution: `hmailserver/source/Server/hMailServer/hMailServer.sln` (2 projects: hMailServer, hMailServer.Minidump).
- `build/build.ps1` passes `/p:PreBuildEventUseInBuild=false /p:PostBuildEventUseInBuild=false` — post-build COM/service registration is already disabled for CI-style builds. Do not re-enable.
- OpenSSL is built WITHOUT `-DOPENSSL_TLS_SECURITY_LEVEL=0` (per spec Phase 1). Keep `--api=1.1.1 no-deprecated` for now — code still calls `SHA1_Init` etc.; Phase 2 migrates to EVP.
- Git rules (user-mandated, apply to every commit in this plan): conventional commit type prefix, subject ≤ 50 chars, imperative mood, NO AI attribution/Co-Authored-By, never `git add .`/`-A` — stage listed files only. Before any `git add`, run the olakunlevpn-clean-export-skills checks (`.gitignore` has `.exclude` entry; no `.md` staged except `README.md`; never stage `.exclude`, `docs/`, `.claude/`, plan/spec files).
- This machine is macOS: nothing compiles locally. Verification for build changes = pushing and reading GitHub Actions results (`gh run watch`).
- Spec: `docs/superpowers/specs/2026-07-03-mailaholic-modernization-design.md`.

---

### Task 1: Point local repo at mailaholic

**Files:**
- No file changes — git remote configuration only.

**Interfaces:**
- Produces: `origin` = `https://github.com/olakunlevpn/mailaholic.git`, `upstream` = `https://github.com/hmailserver/hmailserver.git`. All later tasks push to `origin master`.

- [ ] **Step 1: Rename existing remote and add mailaholic as origin**

```bash
cd /Users/kunle/code/hmailserver
git remote rename origin upstream
git remote add origin https://github.com/olakunlevpn/mailaholic.git
git fetch origin
```

Expected: fetch succeeds, `origin/master` ref appears.

- [ ] **Step 2: Verify local master matches pushed mailaholic master**

```bash
git rev-parse master origin/master
```

Expected: both lines show the same SHA (`5ce8536...`). If they differ, STOP and show the user both SHAs and `git log --oneline master..origin/master` / `git log --oneline origin/master..master` — do not force-push; the user decides which side wins.

- [ ] **Step 3: Set upstream tracking**

```bash
git branch --set-upstream-to=origin/master master
git remote -v
```

Expected: `origin` → mailaholic (fetch+push), `upstream` → hmailserver.

---

### Task 2: Retarget C++ projects to VS2022 (v143)

**Files:**
- Modify: `hmailserver/source/Server/hMailServer/hMailServer.vcxproj:22,29`
- Modify: `hmailserver/source/Server/hMailServer.Minidump/hMailServer.Minidump.vcxproj:22,28`
- Modify: `build/Find-MsBuild.ps1:3`
- Modify: `build/build.ps1:22`

**Interfaces:**
- Consumes: nothing.
- Produces: projects declaring `<PlatformToolset>v143</PlatformToolset>`; `Find-MsBuild` returning the newest VS ≥ 2019 MSBuild (VS2022 on the runner). Task 4's workflow relies on `build/build.ps1` finding MSBuild without arguments.

- [ ] **Step 1: Update PlatformToolset in both vcxproj files**

In `hmailserver/source/Server/hMailServer/hMailServer.vcxproj`, replace both occurrences (Debug and Release property groups):

```xml
    <PlatformToolset>v142</PlatformToolset>
```

with:

```xml
    <PlatformToolset>v143</PlatformToolset>
```

Repeat identically for both occurrences in `hmailserver/source/Server/hMailServer.Minidump/hMailServer.Minidump.vcxproj`.

Leave `<WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>` unchanged in both files (it already floats to the newest installed SDK).

- [ ] **Step 2: Widen Find-MsBuild to accept VS2022**

In `build/Find-MsBuild.ps1`, change:

```powershell
        [string]$VsWhereMinVersion = '[16.0,17.0)'
```

to:

```powershell
        [string]$VsWhereMinVersion = '[16.0,)'
```

and change the vswhere invocation to prefer the newest install:

```powershell
            $msbuild = & $vswhere -latest -version $VsWhereMinVersion -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
```

(The only change on that line is adding `-latest`.)

- [ ] **Step 3: Update build.ps1 error message**

In `build/build.ps1`, change:

```powershell
	Write-Error "MSBuild not found. Install Visual Studio 2019 (Build Tools) or ensure msbuild is on PATH."
```

to:

```powershell
	Write-Error "MSBuild not found. Install Visual Studio 2022 (Build Tools) or ensure msbuild is on PATH."
```

- [ ] **Step 4: Verify no other v142 references remain in the two projects**

```bash
grep -rn "v142" hmailserver/source/Server/hMailServer/hMailServer.vcxproj hmailserver/source/Server/hMailServer.Minidump/hMailServer.Minidump.vcxproj
```

Expected: no output.

- [ ] **Step 5: Commit**

```bash
git add hmailserver/source/Server/hMailServer/hMailServer.vcxproj hmailserver/source/Server/hMailServer.Minidump/hMailServer.Minidump.vcxproj build/Find-MsBuild.ps1 build/build.ps1
git commit -m "build: retarget C++ projects to VS2022 v143"
```

Note: cannot compile on macOS — build verification happens in Task 5 via CI.

---

### Task 3: Update README build instructions

**Files:**
- Modify: `README.md:31,40-49,62,67-68,77,90,98,104`

**Interfaces:**
- Consumes: nothing.
- Produces: README whose manual build steps match what CI does in Task 4 (VS2022, no `OPENSSL_TLS_SECURITY_LEVEL=0`, boost toolset `msvc-14.3`). Do NOT touch the "No active development" section or any hMailServer naming — rebranding is Phase 3.

- [ ] **Step 1: Replace VS2019 references with VS2022**

- Line 31: `* Visual Studio 2019 Community edition` → `* Visual Studio 2022 Community edition`
- Section heading `Installing Visual Studio 2019 Community edition` → `Installing Visual Studio 2022 Community edition`
- In that section: `Download [Visual Studio 2019](...)` → `Download [Visual Studio 2022](https://visualstudio.microsoft.com/vs/)`; individual component `C++ ATL for latest v142 build tools (x86 & x64)` → `C++ ATL for latest v143 build tools (x86 & x64)`; `Windows 10 SDK (10.0.18362.0)` → `Windows 11 SDK (latest)`
- All three occurrences of `Start a x64 Native Tools Command Prompt for VS2019.` → `Start a x64 Native Tools Command Prompt for VS2022.`
- `Visual Studio 2019 must be started with _Run as Administrator_.` → `Visual Studio 2022 must be started with _Run as Administrator_.`

- [ ] **Step 2: Remove the weak-cipher OpenSSL flag from build commands**

In the "Building OpenSSL" section, change:

```
   SET CFLAGS=-DOPENSSL_TLS_SECURITY_LEVEL=0
   Perl Configure no-asm VC-WIN64A --prefix=%cd%\out64 --openssldir=%cd%\out64 -D_WIN32_WINNT=0x600 --api=1.1.1 no-deprecated
```

to:

```
   Perl Configure no-asm VC-WIN64A --prefix=%cd%\out64 --openssldir=%cd%\out64 -D_WIN32_WINNT=0x600 --api=1.1.1 no-deprecated
```

(Delete the `SET CFLAGS` line entirely.)

- [ ] **Step 3: Update boost toolset for VS2022**

In the "Building Boost" b2 command, change `--toolset=msvc-14.2` to `--toolset=msvc-14.3`.

- [ ] **Step 4: Commit**

```bash
git add README.md
git commit -m "docs: update build instructions for VS2022"
```

---

### Task 4: GitHub Actions build workflow

**Files:**
- Create: `.github/workflows/build.yml`

**Interfaces:**
- Consumes: `build/build.ps1` (Task 2's Find-MsBuild change), vcxproj dependency paths under `%hMailServerLibs%`.
- Produces: workflow named `build`, job `server`, producing artifact `hMailServer-exe`. Task 5 watches this workflow; Phase 2+ reuses it as the regression gate.

- [ ] **Step 1: Create the workflow file**

Create `.github/workflows/build.yml` with exactly:

```yaml
name: build

on:
  push:
    branches: [master]
  pull_request:

env:
  hMailServerLibs: C:\hMailLibs
  VCVARS: C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat

jobs:
  server:
    runs-on: windows-2022
    timeout-minutes: 120
    steps:
      - uses: actions/checkout@v4

      - name: Create libs directory
        shell: cmd
        run: if not exist C:\hMailLibs mkdir C:\hMailLibs

      - name: Cache OpenSSL
        id: cache-openssl
        uses: actions/cache@v4
        with:
          path: C:\hMailLibs\openssl-3.5.5\out64
          key: openssl-3.5.5-vs2022-x64-api111-nodep-v1

      - name: Build OpenSSL
        if: steps.cache-openssl.outputs.cache-hit != 'true'
        shell: cmd
        run: |
          curl -sSL -o %TEMP%\openssl.tar.gz https://github.com/openssl/openssl/releases/download/openssl-3.5.5/openssl-3.5.5.tar.gz
          tar -xzf %TEMP%\openssl.tar.gz -C C:\hMailLibs
          call "%VCVARS%"
          cd /d C:\hMailLibs\openssl-3.5.5
          perl Configure no-asm VC-WIN64A --prefix=%CD%\out64 --openssldir=%CD%\out64 -D_WIN32_WINNT=0x600 --api=1.1.1 no-deprecated
          if errorlevel 1 exit /b 1
          nmake
          if errorlevel 1 exit /b 1
          nmake install_sw

      - name: Cache Boost
        id: cache-boost
        uses: actions/cache@v4
        with:
          path: |
            C:\hMailLibs\boost_1_90_0\boost
            C:\hMailLibs\boost_1_90_0\stage\lib
          key: boost-1.90.0-vs2022-x64-static-v1

      - name: Build Boost
        if: steps.cache-boost.outputs.cache-hit != 'true'
        shell: cmd
        run: |
          curl -sSL -o %TEMP%\boost.tar.gz https://archives.boost.io/release/1.90.0/source/boost_1_90_0.tar.gz
          tar -xzf %TEMP%\boost.tar.gz -C C:\hMailLibs
          call "%VCVARS%"
          cd /d C:\hMailLibs\boost_1_90_0
          call bootstrap.bat
          if errorlevel 1 exit /b 1
          b2 debug release threading=multi link=static --with-thread --with-filesystem --with-regex --with-chrono --with-system --with-atomic --toolset=msvc-14.3 address-model=64 stage --build-dir=out64 -j %NUMBER_OF_PROCESSORS%
          if errorlevel 1 exit /b 1

      - name: Cache libpq
        id: cache-libpq
        uses: actions/cache@v4
        with:
          path: |
            C:\hMailLibs\postgresql-15.18\src\include
            C:\hMailLibs\postgresql-15.18\src\interfaces\libpq
            C:\hMailLibs\postgresql-15.18\Release\libpq
          key: libpq-15.18-vs2022-x64-v1

      - name: Build libpq
        if: steps.cache-libpq.outputs.cache-hit != 'true'
        shell: cmd
        run: |
          curl -sSL -o %TEMP%\postgresql.tar.gz https://ftp.postgresql.org/pub/source/v15.18/postgresql-15.18.tar.gz
          tar -xzf %TEMP%\postgresql.tar.gz -C C:\hMailLibs
          call "%VCVARS%"
          cd /d C:\hMailLibs\postgresql-15.18\src\tools\msvc
          perl build.pl Release libpq
          if errorlevel 1 exit /b 1

      - name: Build hMailServer
        shell: pwsh
        run: ./build/build.ps1 -Configuration Release

      - name: Run internal self-tests (experimental)
        continue-on-error: true
        shell: cmd
        run: hmailserver\source\Server\hMailServer\x64\Release\hMailServer.exe /Test

      - name: Upload server binary
        uses: actions/upload-artifact@v4
        with:
          name: hMailServer-exe
          path: hmailserver/source/Server/hMailServer/x64/Release/hMailServer.exe
```

Notes locked into this file (do not "improve" them away):
- `vcvars64.bat` path is the VS2022 Enterprise install present on the `windows-2022` image; Strawberry Perl is preinstalled and on PATH.
- Self-test step is `continue-on-error: true` deliberately: `/Test` (`hMailServer.cpp:254-276`) runs `ClassTester::DoTests()` which includes DNS-dependent testers (`BLCheckTester`, `SPFTester`) and tries a database connection first. Phase 2 hardens this into a required gate; in Phase 1 it is observability only.
- Cache keys end in `-v1`: bump the suffix to force a rebuild when build flags change (e.g., Phase 2 drops `--api=1.1.1`).

- [ ] **Step 2: Validate YAML syntax locally**

```bash
python3 -c "import yaml,sys; yaml.safe_load(open('.github/workflows/build.yml')); print('OK')"
```

Expected: `OK`. (If PyYAML is missing: `pip3 install --user pyyaml` first.)

- [ ] **Step 3: Commit**

```bash
git add .github/workflows/build.yml
git commit -m "ci: add Windows build workflow"
```

---

### Task 5: Push and iterate CI to green

**Files:**
- Possibly modify: any C++ source that fails under v143's stricter conformance (unknown until first run); `.github/workflows/build.yml` if a dependency step fails.

**Interfaces:**
- Consumes: Tasks 1-4 (remotes, retarget, workflow).
- Produces: green `build` workflow run on `origin/master` — the Phase 1 exit criterion and the regression gate every Phase 2+ change builds on.

- [ ] **Step 1: Push master to mailaholic**

```bash
git push origin master
```

- [ ] **Step 2: Watch the workflow run**

```bash
gh run list --repo olakunlevpn/mailaholic --workflow build --limit 1
gh run watch --repo olakunlevpn/mailaholic --exit-status
```

Expected first run: ~40-60 minutes (dependency compilation). Subsequent runs: minutes (cache hits).

- [ ] **Step 3: If a dependency build step fails**

Read the failing step's log:

```bash
gh run view --repo olakunlevpn/mailaholic --log-failed
```

Fix `.github/workflows/build.yml` only (download URL, path, or vcvars issue), commit as `ci: fix <specific problem>` (one commit per fix, workflow file only), push, re-watch. Do not touch C++ source for dependency-step failures.

- [ ] **Step 4: If the MSBuild step fails with compiler errors**

v143 is stricter than v142 (two-phase name lookup, conformance defaults). For each error:
1. Read the error + file:line from `gh run view --log-failed`.
2. Make the minimal source fix that preserves existing behavior (e.g., add a missing `typename`, fix a narrowing conversion) — no refactoring, no modernization beyond what the compiler demands.
3. One commit per logical fix: `git add <specific files>` then `git commit -m "build: fix v143 <short description>"`.
4. Push and re-watch.

If an error is not obviously mechanical (behavior could change), stop and show it to the user before changing code.

- [ ] **Step 5: Confirm exit criterion**

```bash
gh run list --repo olakunlevpn/mailaholic --workflow build --limit 1
```

Expected: `completed success` on `master`. Record in the final report: run URL, duration, whether the experimental self-test step passed or failed (informational), and confirm the `hMailServer-exe` artifact exists.

---

## Self-Review Notes

- Spec coverage (Phase 1 section): remote setup → Task 1; VS2022 retarget → Task 2; workflow with runner/Perl/cache/build.ps1/self-tests → Task 4; "no OPENSSL_TLS_SECURITY_LEVEL=0" → Task 3 Step 2 + Task 4 OpenSSL step (flag absent); exit criterion "retargeted code builds green" → Task 5.
- TDD note: this phase is build configuration; the "test" is the CI run itself. First failing run (nothing exists) → workflow added → green run is the red/green cycle.
- Out of scope, deliberately: `hmailserver Tools.sln` (C#), installer, regression suite in CI, any renaming — later phases per spec.
