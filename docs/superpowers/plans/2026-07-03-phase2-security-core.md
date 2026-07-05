# Mailaholic Phase 2: Security Core — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace every weak cryptographic path in the server core with modern primitives (PBKDF2 password hashing, AES-256-GCM reversible secrets, RFC 8301 DKIM, constant-time compares, strict TLS), verified by a hardened CI self-test gate — with no legacy fallback (fresh-install product).

**Architecture:** All crypto flows through two existing singletons: `Crypt` (`Common/Util/Crypt.{h,cpp}` — reversible + hash dispatch) and `HashCreator` (`Common/Util/Hashing/HashCreator.{h,cpp}` — OpenSSL digest wrapper). Account passwords are stored via `PersistentAccount`/`COM InterfaceAccount`, validated in `PasswordValidator`. Reversible secrets (FetchAccount/Route relay passwords) round-trip through `Crypt::EnCrypt/DeCrypt(...,ETBlowFish)`. The INI admin password lives in `IniFileSettings`. DKIM signing/verification is self-contained in `AntiSpam/DKIM/DKIM.cpp`. This phase rewrites the internals of those units; it does not restructure them. A new self-contained self-test entry point (`/SelfTest`) becomes the CI gate.

**Tech Stack:** C++ (MSVC v143), OpenSSL 3.5.5 EVP APIs (`PKCS5_PBKDF2_HMAC`, `EVP_EncryptInit_ex`/AES-256-GCM, `EVP_DigestInit`, `EVP_MD_CTX`, `CRYPTO_memcmp`, `RAND_bytes`), existing `AnsiString`/`String` types, existing `MimeCodeBase64` for base64, GitHub Actions Windows CI.

## Global Constraints

- **Fresh installs only.** No migrate-on-login, no legacy-hash acceptance. Deleting the MD5/SHA256/plaintext/Blowfish validation branches is intended, not a regression.
- **Fail closed.** Any malformed stored value, or any OpenSSL primitive returning failure (`RAND_bytes`, `PKCS5_PBKDF2_HMAC`, `EVP_*`), makes the operation fail — never fall back to a weaker interpretation, never store a weak salt/IV.
- **Storage formats (exact, self-describing):**
  - Password: `$PBKDF2$<iterations>$<salt-base64>$<hash-base64>`
  - Reversible: `$AESGCM$<iv-base64>$<tag-base64>$<ciphertext-base64>`
- **PBKDF2 parameters:** HMAC-SHA256, 600000 iterations, 16-byte salt (`RAND_bytes`), 32-byte derived key.
- **AES-GCM parameters:** AES-256-GCM, 12-byte IV (`RAND_bytes`) per encryption, 16-byte tag. Key = 32 random bytes generated on first server start, stored as `Security\SecretKey` (hex, 64 chars) in the INI.
- **Column fit:** all formats must fit `accountpassword`/`fapassword`/`routeauthenticationpassword` `varchar(255)` in MSSQL/MySQL/PGSQL — no schema change (`DBScripts/CreateTables*.sql` untouched).
- **base64 helper:** reuse `MimeCodeBase64` (see `HashCreator.cpp:156-166` for the existing encode idiom, which trims a trailing `\r\n` — replicate that exactly) so output has no embedded newlines.
- **COM/IDL frozen this phase.** Do not change `.idl`, method signatures, vtable order, GUIDs, or `EncryptionType` enum *numeric values*. `Crypt::EncryptionType` may gain `ETPBKDF2 = 4` and `ETAESGCM = 5` (additive only). Removed COM utility methods return a scripting error, they are NOT deleted from the interface (Phase 3 deletes them when GUIDs regenerate).
- **Git rules (every commit):** conventional prefix (`feat:`/`fix:`/`refactor:`/`test:`/`ci:`), subject ≤ 50 chars, imperative, NO AI attribution/Co-Authored-By, stage only the files the task lists — never `git add .`/`-A`. Before any `git add`, run olakunlevpn-clean-export-skills checks (`.gitignore` keeps its `.exclude` entry; never stage `.exclude`, `docs/`, `.claude/`, `.superpowers/`, plan/spec files).
- **macOS host — no local build.** Per-task "verification" is a self-review of the diff; real verification is the CI build + `/SelfTest` gate, which a later task turns green. Tasks that add C++ testers MUST register them in the new `/SelfTest` path so CI actually runs them.
- **Base commit for this phase:** `35228eeb` (Phase 1 tip). Spec: `docs/superpowers/specs/2026-07-03-mailaholic-modernization-design.md`.

---

### Task 1: Self-contained `/SelfTest` mode (CI gate foundation)

Do this first: it is the harness that proves every later crypto task on CI. Without it, nothing in Phase 2 is verifiable.

**Files:**
- Create: `hmailserver/source/Server/Common/Util/SelfTest.h`
- Create: `hmailserver/source/Server/Common/Util/SelfTest.cpp`
- Modify: `hmailserver/source/Server/hMailServer/hMailServer.cpp` (add `/SelfTest` branch near the `/Test` branch at lines 254-276)
- Modify: `hmailserver/source/Server/hMailServer/hMailServer.vcxproj` (add the two new files to `<ClCompile>`/`<ClInclude>` item groups)
- Modify: `.github/workflows/build.yml` (replace the self-test step)

**Interfaces:**
- Consumes: existing environment-independent testers whose `Test()` throws on failure — `MimeTester`, `StringParserTester`, `FormatterTester`, `DateTimeTests`, `Base64Tester`, `ModifiedUTF7Tester`, `HashCreatorTester`, `AddresslistParserTester`, `IMAPSimpleCommandParserTester` (all invoked in `Common/Util/ClassTester.cpp:57-160`; these do not touch DB/DNS/registry).
- Produces: `HM::SelfTest::RunAll()` returning `int` (0 = all passed, 1 = a tester threw). Later tasks append their testers to its list.

- [ ] **Step 1: Write SelfTest.h**

```cpp
// Copyright (c) 2026 Mailaholic.
#pragma once

namespace HM
{
   // Runs only environment-independent unit testers (no DB, DNS, registry, or
   // service startup). Safe to run on a bare CI runner. Returns 0 if every
   // tester passed, 1 if any tester threw.
   class SelfTest
   {
   public:
      static int RunAll();
   };
}
```

- [ ] **Step 2: Write SelfTest.cpp**

Mirror the include + invocation pattern already in `ClassTester.cpp`. Each tester's `Test()` throws on failure; catch and report. Use `printf`/`fflush(stdout)` (this runs as a console process on CI, not under a debugger, so `OutputDebugString` is invisible).

```cpp
// Copyright (c) 2026 Mailaholic.
#include "stdafx.h"
#include "SelfTest.h"

#include <cstdio>

#include "../Mime/MimeTester.h"
#include "Parsing/StringParser.h"
#include "Formatter.h"
#include "Time.h"
#include "Encoding/Base64.h"
#include "Encoding/ModifiedUTF7.h"
#include "Hashing/HashCreator.h"
#include "Parsing/AddresslistParser.h"
#include "../../IMAP/IMAPSimpleCommandParser.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace HM
{
   int
   SelfTest::RunAll()
   {
      int failures = 0;

      struct NamedTest { const char* name; std::function<void()> run; };

      std::vector<NamedTest> tests =
      {
         { "MimeTester",                []{ MimeTester t; t.Test(); } },
         { "StringParserTester",        []{ StringParserTester t; t.Test(); } },
         { "FormatterTester",           []{ FormatterTester t; t.Test(); } },
         { "DateTimeTests",             []{ DateTimeTests t; t.Test(); } },
         { "Base64Tester",              []{ Base64Tester t; t.Test(); } },
         { "ModifiedUTF7Tester",        []{ ModifiedUTF7Tester t; t.Test(); } },
         { "HashCreatorTester",         []{ HashCreatorTester t; t.Test(); } },
         { "AddresslistParserTester",   []{ AddresslistParserTester t; t.Test(); } },
         { "IMAPSimpleCommandParserTester", []{ IMAPSimpleCommandParserTester t; t.Test(); } },
      };

      for (auto& test : tests)
      {
         try
         {
            test.run();
            printf("[ PASS ] %s\n", test.name);
         }
         catch (...)
         {
            printf("[ FAIL ] %s\n", test.name);
            failures++;
         }
         fflush(stdout);
      }

      printf("SelfTest: %d/%d passed\n", (int)tests.size() - failures, (int)tests.size());
      fflush(stdout);
      return failures == 0 ? 0 : 1;
   }
}
```

Note on includes: verify each tester class's real header path before committing (some tester classes are declared in the header of the class they test — e.g. `HashCreatorTester` is in `Hashing/HashCreator.h`, confirmed at `HashCreator.h:47`). If a header path differs, fix the `#include` — do not add new dependencies. If a tester needs `<functional>`/`<vector>`, they arrive via `stdafx.h`; add explicit includes only if the compiler complains on CI.

- [ ] **Step 3: Add the `/SelfTest` branch to hMailServer.cpp**

Immediately BEFORE the existing `if (sLastParam.CompareNoCase(_T("/Test")) == 0)` block (hMailServer.cpp:254), insert:

```cpp
   if (sLastParam.CompareNoCase(_T("/SelfTest")) == 0)
   {
      // Environment-independent unit tests only (no DB/DNS/registry/service).
      // Used as the CI build gate. See Common/Util/SelfTest.cpp.
      return HM::SelfTest::RunAll();
   }
```

Add `#include "../Common/Util/SelfTest.h"` with the other includes at the top of hMailServer.cpp (match the existing include style/paths in that file).

- [ ] **Step 4: Register the new files in the vcxproj**

In `hmailserver/source/Server/hMailServer/hMailServer.vcxproj`, add to the existing `<ItemGroup>` that holds `<ClCompile>` entries:
```xml
    <ClCompile Include="..\Common\Util\SelfTest.cpp" />
```
and to the `<ItemGroup>` holding `<ClInclude>` entries:
```xml
    <ClInclude Include="..\Common\Util\SelfTest.h" />
```
Match the relative-path style of neighboring entries (grep the file for an existing `..\Common\Util\` include to copy the exact prefix). Preserve file ordering conventions if any; otherwise append within the group.

- [ ] **Step 5: Swap the CI self-test step to the gate**

In `.github/workflows/build.yml`, replace the step:
```yaml
      - name: Run internal self-tests (experimental)
        continue-on-error: true
        shell: cmd
        run: hmailserver\source\Server\hMailServer\x64\Release\hMailServer.exe /Test
```
with:
```yaml
      - name: Run self-tests
        shell: cmd
        run: hmailserver\source\Server\hMailServer\x64\Release\hMailServer.exe /SelfTest
```
(No `continue-on-error`: a non-zero exit now fails the build. This is the point of the task.)

- [ ] **Step 6: Commit**

```bash
git add hmailserver/source/Server/Common/Util/SelfTest.h hmailserver/source/Server/Common/Util/SelfTest.cpp hmailserver/source/Server/hMailServer/hMailServer.cpp hmailserver/source/Server/hMailServer/hMailServer.vcxproj .github/workflows/build.yml
git commit -m "test: add self-contained SelfTest CI gate"
```

- [ ] **Step 7: Push and confirm the gate goes green**

```bash
git push origin master
gh run watch --repo olakunlevpn/mailaholic --exit-status
```
Expected: `completed success`, and the "Run self-tests" step prints `SelfTest: 9/9 passed` with exit 0. If `/SelfTest` still crashes (0xC0000005), read `gh run view --log-failed`: the crash is now inside a specific tester's construction, not service startup — report which tester and STOP (that tester has a hidden environment dependency; controller decides whether to drop it from the list). If a compile error, fix the include path minimally and re-push.

---

### Task 2: PBKDF2 in HashCreator + Crypt (password hashing core)

**Files:**
- Modify: `hmailserver/source/Server/Common/Util/Crypt.h` (add enum values, method decls)
- Modify: `hmailserver/source/Server/Common/Util/Crypt.cpp` (PBKDF2 encrypt/validate/detect)
- Modify: `hmailserver/source/Server/Common/Util/Hashing/HashCreator.cpp` (constant-time compare — Task 4 also touches this; do the PBKDF2-adjacent part here)

**Interfaces:**
- Consumes: OpenSSL `PKCS5_PBKDF2_HMAC`, `RAND_bytes`, `EVP_sha256()`, `CRYPTO_memcmp`; `MimeCodeBase64` for base64.
- Produces:
  - `Crypt::EncryptionType` gains `ETPBKDF2 = 4` (additive; existing 0-3 unchanged).
  - `String Crypt::GeneratePBKDF2_(const String &password) const` — returns `$PBKDF2$600000$<salt-b64>$<hash-b64>`.
  - `bool Crypt::ValidatePBKDF2_(const String &password, const String &stored) const` — constant-time, fail-closed on malformed input.
  - `Crypt::GetHashType` returns `ETPBKDF2` when the value starts with `$PBKDF2$`.
  - `Crypt::EnCrypt(pw, ETPBKDF2)` produces the PBKDF2 string; `Crypt::Validate(pw, stored, ETPBKDF2)` verifies it.

- [ ] **Step 1: Add the failing self-test (TDD red)**

In `hmailserver/source/Server/Common/Util/Hashing/HashCreator.cpp`, extend `HashCreatorTester::Test()` (currently ends at line 199) with PBKDF2 assertions. Add before the final closing brace of `Test()`:

```cpp
      // PBKDF2 round-trip: a generated hash must validate against its password
      // and reject a wrong password. Fail-closed on malformed input.
      {
         Crypt* crypt = Crypt::Instance();

         String stored = crypt->EnCrypt("correct horse battery staple", Crypt::ETPBKDF2);

         if (crypt->GetHashType(stored) != Crypt::ETPBKDF2)
            throw 0;
         if (!stored.StartsWith(_T("$PBKDF2$600000$")))
            throw 0;
         if (!crypt->Validate("correct horse battery staple", stored, Crypt::ETPBKDF2))
            throw 0;
         if (crypt->Validate("wrong password", stored, Crypt::ETPBKDF2))
            throw 0;

         // Two hashes of the same password differ (random salt).
         String stored2 = crypt->EnCrypt("correct horse battery staple", Crypt::ETPBKDF2);
         if (stored == stored2)
            throw 0;

         // Malformed stored values must fail closed, never throw uncaught, never validate.
         if (crypt->Validate("x", _T("$PBKDF2$notanumber$aa$bb"), Crypt::ETPBKDF2))
            throw 0;
         if (crypt->Validate("x", _T("garbage"), Crypt::ETPBKDF2))
            throw 0;
      }
```

Add `#include "../Crypt.h"` to HashCreator.cpp's includes if not already present (check the top of the file first).

- [ ] **Step 2: Confirm it fails**

This is verified on CI, not locally. After implementing nothing yet, the code won't compile (`ETPBKDF2` undefined) — that is the red state. Do not push a known-non-compiling commit; proceed to implementation and let the FIRST push of this task exercise red→green in one CI run. Note in your report that red was confirmed by the undefined symbol.

- [ ] **Step 3: Add enum + method declarations to Crypt.h**

Change the enum (Crypt.h:16-22) to:
```cpp
      enum EncryptionType
      {
         ETNone = 0,
         ETBlowFish = 1,
         ETMD5 = 2,
         ETSHA256 = 3,
         ETPBKDF2 = 4
      };
```
Add to the `private:` section (after `BlowFishEncryptor *blow_fish_;`):
```cpp
      String GeneratePBKDF2_(const String &password) const;
      bool ValidatePBKDF2_(const String &password, const String &stored) const;
```

- [ ] **Step 4: Implement PBKDF2 in Crypt.cpp**

Add includes at the top (after the existing `#include "Hashing/HashCreator.h"`):
```cpp
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include "../Mime/MimeCode.h"
```

Add these private helpers (implementation) and wire the public methods. Constants at file scope inside `namespace HM`:
```cpp
   namespace
   {
      const int PBKDF2_ITERATIONS = 600000;
      const int PBKDF2_SALT_LEN = 16;
      const int PBKDF2_KEY_LEN = 32;

      // Base64-encode raw bytes with no trailing newline, matching the idiom
      // in HashCreator::GetHash_Raw (HashCreator.cpp:156-166).
      AnsiString Base64EncodeRaw(const unsigned char *data, int len)
      {
         MimeCodeBase64 encoder;
         encoder.SetInput((const char*)data, len, true);
         AnsiString encoded;
         encoder.GetOutput(encoded);
         // Trim the trailing CRLF the encoder appends.
         encoded = encoded.Mid(0, encoded.GetLength() - 2);
         return encoded;
      }

      // Returns decoded byte vector; empty vector signals decode failure/empty.
      std::vector<unsigned char> Base64DecodeRaw(const AnsiString &input)
      {
         MimeCodeBase64 decoder;
         decoder.SetInput(input.c_str(), input.GetLength(), false);
         AnsiString decoded;
         decoder.GetOutput(decoded);
         return std::vector<unsigned char>(decoded.begin(), decoded.end());
      }
   }
```

Implementation of the helpers:
```cpp
   String
   Crypt::GeneratePBKDF2_(const String &password) const
   {
      unsigned char salt[PBKDF2_SALT_LEN];
      if (RAND_bytes(salt, PBKDF2_SALT_LEN) != 1)
         throw std::runtime_error("RAND_bytes failed generating PBKDF2 salt");

      AnsiString passwordAnsi = password;

      unsigned char key[PBKDF2_KEY_LEN];
      if (PKCS5_PBKDF2_HMAC(passwordAnsi.c_str(), passwordAnsi.GetLength(),
                            salt, PBKDF2_SALT_LEN,
                            PBKDF2_ITERATIONS, EVP_sha256(),
                            PBKDF2_KEY_LEN, key) != 1)
         throw std::runtime_error("PKCS5_PBKDF2_HMAC failed");

      String result;
      result.Format(_T("$PBKDF2$%d$%s$%s"),
                    PBKDF2_ITERATIONS,
                    String(Base64EncodeRaw(salt, PBKDF2_SALT_LEN)).c_str(),
                    String(Base64EncodeRaw(key, PBKDF2_KEY_LEN)).c_str());
      return result;
   }

   bool
   Crypt::ValidatePBKDF2_(const String &password, const String &stored) const
   {
      // Format: $PBKDF2$<iterations>$<salt-b64>$<hash-b64>
      std::vector<String> parts;
      StringParser::SplitString(stored, _T("$"), parts);
      // Leading '$' yields an empty first element: ["", "PBKDF2", iters, salt, hash]
      if (parts.size() != 5)
         return false;
      if (parts[1] != _T("PBKDF2"))
         return false;

      int iterations = _ttoi(parts[2].c_str());
      if (iterations <= 0)
         return false;

      std::vector<unsigned char> salt = Base64DecodeRaw(AnsiString(parts[3]));
      std::vector<unsigned char> expected = Base64DecodeRaw(AnsiString(parts[4]));
      if (salt.empty() || expected.empty())
         return false;

      AnsiString passwordAnsi = password;
      std::vector<unsigned char> actual(expected.size());
      if (PKCS5_PBKDF2_HMAC(passwordAnsi.c_str(), passwordAnsi.GetLength(),
                            &salt[0], (int)salt.size(),
                            iterations, EVP_sha256(),
                            (int)actual.size(), &actual[0]) != 1)
         return false;

      return CRYPTO_memcmp(&actual[0], &expected[0], actual.size()) == 0;
   }
```

Wire into the public switch statements:
- In `Crypt::EnCrypt` (add a `case ETPBKDF2:` returning `GeneratePBKDF2_(sInput);`).
- In `Crypt::Validate` (add a `case ETPBKDF2:` returning `ValidatePBKDF2_(password, originalHash);`).
- In `Crypt::GetHashType` (before the length checks): `if (hash.StartsWith(_T("$PBKDF2$"))) return ETPBKDF2;`

Add `#include <vector>`, `#include <stdexcept>`, and the `StringParser` include (`#include "Parsing/StringParser.h"`) if not already reachable via stdafx — check first, add only if the compiler needs them.

Verify before relying on them: `String::StartsWith`, `String::Format`, and `StringParser::SplitString` signatures — grep `Common/Util/String.h` and `Common/Util/Parsing/StringParser.h`. If `StartsWith` doesn't exist, use `hash.Mid(0, 8) == _T("$PBKDF2$")`. If `SplitString`'s signature differs, adapt the call — the logic (split on `$`, expect 5 parts) is what matters.

- [ ] **Step 5: Commit and verify on CI**

```bash
git add hmailserver/source/Server/Common/Util/Crypt.h hmailserver/source/Server/Common/Util/Crypt.cpp hmailserver/source/Server/Common/Util/Hashing/HashCreator.cpp
git commit -m "feat: add PBKDF2-HMAC-SHA256 password hashing"
git push origin master
gh run watch --repo olakunlevpn/mailaholic --exit-status
```
Expected: build green, `SelfTest: 9/9 passed` (HashCreatorTester now includes PBKDF2 cases). If a PBKDF2 case throws, the self-test step fails the build — read the `[ FAIL ] HashCreatorTester` line, fix, re-push. Max 4 iterations; if base64 trimming or split logic fights the `MimeCodeBase64`/`StringParser` API, report BLOCKED with the failing assertion.

---

### Task 3: Switch all password write/validate paths to PBKDF2 (clean break)

**Files:**
- Modify: `hmailserver/source/Server/Common/Application/IniFileSettings.cpp:157` (default PreferredHashAlgorithm 3 → 4)
- Modify: `hmailserver/source/Server/Common/Util/PasswordValidator.cpp:149-188` (accept only PBKDF2 for account passwords)
- Modify: `hmailserver/source/Server/COM/COMAuthentication.cpp:38-90` (admin password validated as PBKDF2)
- Modify: `hmailserver/source/Server/Common/Util/Hashing/HashCreator.cpp` (add migration self-test asserting legacy rejection)

**Interfaces:**
- Consumes: `Crypt::ETPBKDF2`, `Crypt::Validate(...,ETPBKDF2)`, `Crypt::GetHashType` from Task 2.
- Produces: account + admin password validation that accepts ONLY `$PBKDF2$…`; default new-password encryption = PBKDF2 (via `PreferredHashAlgorithm = 4`, already consumed at `PersistentAccount.cpp:189` and `InterfaceAccount.cpp:259` — no change needed there since they read the INI value).

- [ ] **Step 1: Default new passwords to PBKDF2**

`IniFileSettings.cpp:157`, change:
```cpp
      preferred_hash_algorithm_ = ReadIniSettingInteger_("Settings", "PreferredHashAlgorithm", 3);
```
to:
```cpp
      preferred_hash_algorithm_ = ReadIniSettingInteger_("Settings", "PreferredHashAlgorithm", 4);
```
(4 = `ETPBKDF2`. `PersistentAccount.cpp:189-191` and `InterfaceAccount.cpp:259-263` already call `Crypt::EnCrypt(pw, (EncryptionType)preferredHashAlgorithm)` — with the default now 4, new/changed account passwords are stored as PBKDF2 with no further change.)

- [ ] **Step 2: Account password validation — PBKDF2 only**

In `PasswordValidator.cpp`, replace the encryption-type branch (lines 149-188, from `Crypt::EncryptionType iPasswordEncryption = ...` through the final `else return false;`) with:

```cpp
      String sComparePassword = pAccount->GetPassword();

      // Mailaholic stores account passwords only as PBKDF2. Legacy plaintext,
      // MD5, SHA-256, and Blowfish schemes are not accepted (fresh-install
      // product; no migration path). A stored value in any other format fails
      // closed.
      if (Crypt::Instance()->GetHashType(sComparePassword) != Crypt::ETPBKDF2)
      {
         ErrorManager::Instance()->ReportError(ErrorManager::Medium, 4340,
            "PasswordValidator::ValidatePassword",
            "Account password is not stored in PBKDF2 format; authentication denied. Reset the password.");
         return false;
      }

      if (!Crypt::Instance()->Validate(sPassword, sComparePassword, Crypt::ETPBKDF2))
         return false;

      return true;
```
Keep everything above line 149 (the AD/SSPI branch at 136-147) unchanged. Add `#include "../Application/ErrorManager.h"` if `ErrorManager` isn't already included in this file (grep the top of PasswordValidator.cpp; it likely is via other includes — add only if needed). Pick an unused error code near existing ones if 4340 collides (grep `4340` under Server/; if present, choose another free number in that band).

- [ ] **Step 3: Admin password validation — PBKDF2 only**

Read `COM/COMAuthentication.cpp:30-95` first. It fetches `GetAdministratorPassword()` and compares. Replace its hash-comparison logic so the stored admin password is validated via `Crypt::Instance()->Validate(sSubmitted, sStored, Crypt::ETPBKDF2)` and rejects any non-PBKDF2 stored value (same fail-closed pattern as Step 2). Preserve the surrounding COM method contract (return values / HRESULT). Show the exact replacement in your report; the shape mirrors Step 2 but adapt to whatever comparison call is there now (it currently relies on `Crypt::GetHashType` length detection).

- [ ] **Step 4: Add legacy-rejection self-test**

In `HashCreatorTester::Test()` (HashCreator.cpp), add:
```cpp
      // Legacy formats must not be recognized as PBKDF2.
      {
         Crypt* crypt = Crypt::Instance();
         // A 32-char MD5-looking string and a 70-char SHA256-looking string
         // must NOT be treated as PBKDF2.
         if (crypt->GetHashType(_T("5f4dcc3b5aa765d61d8327deb882cf99")) == Crypt::ETPBKDF2)
            throw 0;
         String sha256like = _T("abcdef") ; // padded below
         while (sha256like.GetLength() < 70) sha256like += _T("a");
         if (crypt->GetHashType(sha256like) == Crypt::ETPBKDF2)
            throw 0;
      }
```

- [ ] **Step 5: Commit and verify on CI**

```bash
git add hmailserver/source/Server/Common/Application/IniFileSettings.cpp hmailserver/source/Server/Common/Util/PasswordValidator.cpp hmailserver/source/Server/COM/COMAuthentication.cpp hmailserver/source/Server/Common/Util/Hashing/HashCreator.cpp
git commit -m "feat: require PBKDF2 for account and admin auth"
git push origin master
gh run watch --repo olakunlevpn/mailaholic --exit-status
```
Expected: green, self-test passes. Report exactly what you changed in COMAuthentication.cpp (the one non-mechanical edit).

---

### Task 4: AES-256-GCM for reversible secrets + constant-time hash compare

**Files:**
- Modify: `hmailserver/source/Server/Common/Util/Crypt.h` (add `ETAESGCM = 5`, decls)
- Modify: `hmailserver/source/Server/Common/Util/Crypt.cpp` (AES-GCM encrypt/decrypt; route ETBlowFish call sites use new type — see Step 4)
- Modify: `hmailserver/source/Server/Common/Application/IniFileSettings.{h,cpp}` (SecretKey getter, generate-on-first-start)
- Modify: `hmailserver/source/Server/Common/Util/Hashing/HashCreator.cpp:62-79` (`ValidateHash` uses `CRYPTO_memcmp`)
- Modify: 4 call sites currently passing `Crypt::ETBlowFish`: `Common/BO/FetchAccount.cpp:64,90`, `Common/BO/Route.cpp:64,85`, `Common/Persistence/PersistentRoute.cpp:72,138`, `Common/Persistence/PersistentFetchAccount.cpp:100,165`

**Interfaces:**
- Consumes: OpenSSL `EVP_EncryptInit_ex`/`EVP_DecryptInit_ex` with `EVP_aes_256_gcm()`, `RAND_bytes`, `EVP_CIPHER_CTX`; `IniFileSettings::GetSecretKey()`.
- Produces:
  - `Crypt::ETAESGCM = 5`.
  - `Crypt::EnCrypt(s, ETAESGCM)` → `$AESGCM$<iv-b64>$<tag-b64>$<ct-b64>`; `Crypt::DeCrypt(s, ETAESGCM)` → plaintext (fail-closed: returns empty + logs on tag-verify failure or malformed input).
  - `String IniFileSettings::GetSecretKey()` — returns the 64-hex-char key, generating and persisting it on first call if absent.
  - `HashCreator::ValidateHash` compares with `CRYPTO_memcmp`.

- [ ] **Step 1: SecretKey in IniFileSettings**

In `IniFileSettings.h`, declare (public): `String GetSecretKey();` and (private): `String secret_key_;`.
In `IniFileSettings.cpp`, implement `GetSecretKey()`: read `Security\SecretKey`; if empty, generate 32 random bytes via `RAND_bytes`, hex-encode to 64 chars, write it back via the existing `WriteIniSetting_` used at line 366 for the admin password, and return it. Fail-closed: if `RAND_bytes` fails, throw (never return a constant/empty key). Show the implementation in your report. Add `#include <openssl/rand.h>` to IniFileSettings.cpp.

- [ ] **Step 2: AES-GCM in Crypt**

Add `ETAESGCM = 5` to the enum. Add private helpers `String EncryptAESGCM_(const String &plaintext) const` and `String DecryptAESGCM_(const String &stored) const`. Implementation uses `EVP_aes_256_gcm()`, a 12-byte IV from `RAND_bytes`, key = the 32 bytes decoded from `IniFileSettings::Instance()->GetSecretKey()` (hex→bytes). Format `$AESGCM$<iv-b64>$<tag-b64>$<ct-b64>` using the same `Base64EncodeRaw`/`Base64DecodeRaw`/`StringParser::SplitString` helpers from Task 2 (they now exist in Crypt.cpp). Decrypt sets the expected tag with `EVP_CTRL_GCM_SET_TAG` before `EVP_DecryptFinal_ex`; if final returns ≤0 (tag mismatch) return empty + log. Wire `case ETAESGCM:` into `EnCrypt` and `DeCrypt`. Provide the full implementation in your report; it mirrors the PBKDF2 helper structure.

- [ ] **Step 3: Constant-time ValidateHash**

In `HashCreator.cpp` `ValidateHash` (lines 62-79), replace both `if (result == originalHash)` string equalities with a length check plus `CRYPTO_memcmp` over the bytes:
```cpp
      // Constant-time comparison to avoid timing side channels.
      AnsiString a = result, b = originalHash;
      if (a.GetLength() != b.GetLength())
         return false;
      return CRYPTO_memcmp(a.c_str(), b.c_str(), a.GetLength()) == 0;
```
Apply to both the salted and unsalted branches (compute `result` as each branch does today, then compare via the snippet). Add `#include <openssl/crypto.h>` to HashCreator.cpp if not present.

- [ ] **Step 4: Repoint the 8 Blowfish call sites**

In each of the 4 files, change `Crypt::ETBlowFish` → `Crypt::ETAESGCM` at the listed lines. These are the reversible relay/fetch passwords. After this, `ETBlowFish` has no remaining callers except the COM utility methods (handled in Task 5) and the Blowfish self-tester.

- [ ] **Step 5: Add AES-GCM round-trip + tamper self-test**

Append to `HashCreatorTester::Test()`:
```cpp
      // AES-GCM round-trip and tamper rejection.
      {
         Crypt* crypt = Crypt::Instance();
         String secret = "hunter2 relay password";
         String blob = crypt->EnCrypt(secret, Crypt::ETAESGCM);
         if (!blob.StartsWith(_T("$AESGCM$")))
            throw 0;
         if (crypt->DeCrypt(blob, Crypt::ETAESGCM) != secret)
            throw 0;
         // Tampering with the ciphertext must fail the GCM tag (empty result).
         String tampered = blob.Left(blob.GetLength() - 2) + _T("AA");
         String out = crypt->DeCrypt(tampered, Crypt::ETAESGCM);
         if (out == secret)
            throw 0;
         // Two encryptions of the same secret differ (random IV).
         if (crypt->EnCrypt(secret, Crypt::ETAESGCM) == blob)
            throw 0;
      }
```
NOTE: this self-test calls `IniFileSettings::Instance()->GetSecretKey()` transitively, which reads/writes the INI. On the CI runner there is no INI. Two options — pick the one that keeps `/SelfTest` environment-independent: (a) have `GetSecretKey()` fall back to an in-memory generated key when no INI path is configured (preferred — also covers first-boot ordering); or (b) if `GetSecretKey` cannot run without an INI, move ONLY the AES-GCM self-test into the DB-dependent `ClassTester::DoTests` path instead of `/SelfTest`, and note that AES-GCM is not covered by the CI gate. Prefer (a); document your choice.

- [ ] **Step 6: Commit and verify on CI**

```bash
git add hmailserver/source/Server/Common/Util/Crypt.h hmailserver/source/Server/Common/Util/Crypt.cpp hmailserver/source/Server/Common/Application/IniFileSettings.h hmailserver/source/Server/Common/Application/IniFileSettings.cpp hmailserver/source/Server/Common/Util/Hashing/HashCreator.cpp hmailserver/source/Server/Common/BO/FetchAccount.cpp hmailserver/source/Server/Common/BO/Route.cpp hmailserver/source/Server/Common/Persistence/PersistentRoute.cpp hmailserver/source/Server/Common/Persistence/PersistentFetchAccount.cpp
git commit -m "feat: AES-256-GCM secrets, constant-time compare"
git push origin master
gh run watch --repo olakunlevpn/mailaholic --exit-status
```
Expected: green, self-test passes (now 9 testers + PBKDF2 + AES-GCM assertions inside HashCreatorTester).

---

### Task 5: DKIM RFC 8301 + retire weak COM utilities

**Files:**
- Modify: `hmailserver/source/Server/Common/AntiSpam/DKIM/DKIM.cpp` (lines 137, 368, 452 and the verification SHA1 branches)
- Modify: `hmailserver/source/Server/COM/InterfaceUtilities.cpp:86,122,140` (MD5/Blowfish methods return scripting error)

**Interfaces:**
- Consumes: existing DKIM signing/verification structure.
- Produces: signer always emits `a=rsa-sha256`; verifier PermFails `a=rsa-sha1`; COM `MD5`/`BlowfishEncrypt`/`BlowfishDecrypt` return a scripting error without performing weak crypto (interface shape unchanged).

- [ ] **Step 1: Read the DKIM SHA1 sites**

Read `DKIM.cpp` around lines 130-145 (signing algorithm string, `ASDKIMSHA1` flag), 360-400 (header hash), and 445-505 (body hash + verification algorithm selection). Identify every `HashCreator::SHA1` / `EVP_sha1()` / `"rsa-sha1"` usage. List them in your report before editing.

- [ ] **Step 2: Force rsa-sha256 on signing**

At DKIM.cpp:137 (`String tagA = algorithm == HashCreator::SHA1 ? "rsa-sha1" : "rsa-sha256";`) and the corresponding signer hash selection (line ~368, `HashCreator shaer(tagA == "rsa-sha256" ? SHA256 : SHA1)`), make the signer unconditionally use SHA-256 and emit `a=rsa-sha256`, ignoring the `ASDKIMSHA1` domain flag. Keep the flag readable (COM/Domain BO unchanged this phase) but the signer coerces to SHA-256.

- [ ] **Step 3: PermFail rsa-sha1 on verification**

At the verification algorithm selection (line ~452, `HashCreator shaer(tagA == "rsa-sha1" ? SHA1 : SHA256)`) and any `EVP_sha1()` fallback in header/body hash verification: when the incoming signature declares `a=rsa-sha1`, return the verification's permanent-failure result (match how the surrounding code signals PermFail — read it and mirror it exactly). Remove the SHA1 branch so an rsa-sha1 signature can never be considered valid (RFC 8301 §3.1).

- [ ] **Step 4: Retire weak COM utilities**

In `InterfaceUtilities.cpp`, for `MD5` (line 86), `BlowfishEncrypt` (line 122), `BlowfishDecrypt` (line 140): replace the body so it returns a COM scripting error instead of performing the operation. Match the existing COM error-return idiom in this file (grep for how other methods return errors — likely `COMError::GenerateError` or an `HRESULT`/`Error()` pattern). Do NOT remove the methods from the interface or change signatures. `SHA256` (line 104) stays functional. Provide the exact replacement in your report.

- [ ] **Step 5: DKIM self-test**

If a `DKIMTester` exists (grep `AntiSpam/DKIM/` for `Tester`), add a case asserting the signer output contains `a=rsa-sha256` and an rsa-sha1 signature verifies as PermFail, and register it in `/SelfTest` (Task 1's list) ONLY if it is environment-independent (no DNS lookups — DKIM verification normally fetches DNS TXT keys, so a full verify test is NOT self-test-safe). If no environment-independent DKIM test is feasible, state that in your report and rely on code review for this task; do not force a DNS-dependent test into the CI gate.

- [ ] **Step 6: Commit and verify on CI**

```bash
git add hmailserver/source/Server/Common/AntiSpam/DKIM/DKIM.cpp hmailserver/source/Server/COM/InterfaceUtilities.cpp
git commit -m "fix: DKIM rsa-sha256 only, retire weak COM utils"
git push origin master
gh run watch --repo olakunlevpn/mailaholic --exit-status
```
Expected: green. Report the PermFail mechanism you mirrored and the COM error idiom you used.

---

### Task 6: Strict TLS defaults

**Files:**
- Modify: `hmailserver/source/Server/Common/TCPIP/SslContextInitializer.cpp` (SetContextOptions_, ~line 177-205)
- Read-only reference: `Common/BO/TLSCertificate` / `Configuration` TLS getters to understand the admin-tunable settings.

**Interfaces:**
- Consumes: existing `Configuration::GetSslVersionEnabled(...)` admin settings.
- Produces: an OpenSSL context that refuses < TLS 1.2 and sets security level ≥ 2 regardless of admin toggles, while still honoring admin choices at or above that floor.

- [ ] **Step 1: Enforce a TLS 1.2 floor**

In `SetContextOptions_` (SslContextInitializer.cpp:177-205), after the existing per-version option assembly, unconditionally add `SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1` to `options` (some are already present — ensure TLS 1.0 and 1.1 are always disabled even if `tlsv10`/`tlsv11` config is true). Add a comment that Mailaholic enforces a TLS 1.2 minimum. Leave the 1.2/1.3 toggles honoring admin config.

- [ ] **Step 2: Set OpenSSL security level ≥ 2**

After `SSL_CTX_set_options(ssl, options);` (line 204), add:
```cpp
      // Mailaholic enforces OpenSSL security level 2 (rejects <112-bit
      // security, e.g. SHA1 signatures, RSA/DH < 2048, weak ciphers).
      SSL_CTX_set_security_level(ssl, 2);
```

- [ ] **Step 3: Commit and verify on CI**

```bash
git add hmailserver/source/Server/Common/TCPIP/SslContextInitializer.cpp
git commit -m "feat: enforce TLS 1.2 minimum, security level 2"
git push origin master
gh run watch --repo olakunlevpn/mailaholic --exit-status
```
Expected: green (this changes runtime TLS, not compiled tests). Verification of actual TLS behavior is deferred to a live server (not available on CI) — note this in your report.

---

### Task 7: Drop deprecated OpenSSL API pinning

Do this LAST — only after Tasks 2-6 have removed all direct `SHA1_Init`/`MD5_Init`/`SHA256_Init` legacy digest calls and replaced them with EVP. If HashCreator still uses the legacy `SHA_CTX`/`MD5_CTX`/`SHA256_CTX` APIs (HashCreator.cpp:114-137), this task also migrates those to `EVP_MD_CTX` — otherwise dropping `no-deprecated` breaks the build.

**Files:**
- Modify: `hmailserver/source/Server/Common/Util/Hashing/HashCreator.cpp:93-139` (migrate raw digest to EVP if still legacy)
- Modify: `.github/workflows/build.yml` (drop `--api=1.1.1 no-deprecated`, bump cache key to `-v2`)
- Modify: `README.md` (same OpenSSL Configure line)

**Interfaces:**
- Consumes: `EVP_MD_CTX_new`, `EVP_DigestInit_ex`, `EVP_DigestUpdate`, `EVP_DigestFinal_ex`, `EVP_MD_CTX_free`, `EVP_sha1/sha256/md5`.
- Produces: a build that compiles against OpenSSL configured without `no-deprecated`, no deprecated symbol usage remaining.

- [ ] **Step 1: Migrate HashCreator raw digest to EVP**

Replace the `switch (hash_type_)` block that uses `SHA_CTX`/`MD5_CTX`/`SHA256_CTX` (HashCreator.cpp:112-139) with a single EVP flow: pick `const EVP_MD* md` (`EVP_sha1()`/`EVP_md5()`/`EVP_sha256()`) by `hash_type_`, then `EVP_MD_CTX_new` → `EVP_DigestInit_ex(ctx, md, NULL)` → `EVP_DigestUpdate(ctx, input, inputLength)` → `EVP_DigestFinal_ex(ctx, results, &outLen)` → `EVP_MD_CTX_free(ctx)`. Preserve the existing `digestLength` sizing and the hex/base64 encoding that follows (lines 142-170 unchanged). Keep SHA1/MD5 *available* as digest types — they are still used for DKIM body-hash canonicalization inputs and legacy interop reads elsewhere; only the deprecated *C API* is being dropped, not the algorithms. Show the replacement in your report.

- [ ] **Step 2: Grep for any remaining deprecated symbols**

```bash
grep -rn "SHA1_Init\|SHA1_Update\|SHA1_Final\|MD5_Init\|MD5_Update\|MD5_Final\|SHA256_Init\|SHA256_Update\|SHA256_Final\|SHA_CTX\|MD5_CTX\|SHA256_CTX" hmailserver/source/Server
```
Expected: no output. If any remain, migrate them to EVP the same way before proceeding.

- [ ] **Step 3: Drop the flag from workflow and README, bump cache key**

In `.github/workflows/build.yml`, change the OpenSSL Configure line (currently `perl Configure no-asm VC-WIN64A ... --api=1.1.1 no-deprecated`) to remove `--api=1.1.1 no-deprecated`, and change the OpenSSL cache key `openssl-3.5.5-vs2022-x64-api111-nodep-v1` → `openssl-3.5.5-vs2022-x64-v2` (new key forces a fresh OpenSSL build with the new config). Apply the identical Configure-line change to README.md's Building OpenSSL section.

- [ ] **Step 4: Commit and verify on CI**

```bash
git add hmailserver/source/Server/Common/Util/Hashing/HashCreator.cpp .github/workflows/build.yml README.md
git commit -m "refactor: use OpenSSL EVP, drop deprecated API pin"
git push origin master
gh run watch --repo olakunlevpn/mailaholic --exit-status
```
Expected: OpenSSL rebuilds (cache miss on new key, ~15-20 min added this run), build green, `SelfTest` passes with EVP-based digests. If EVP migration changed any hash output, HashCreatorTester's existing SHA256 round-trip will catch it — fix before considering done.

---

## Self-Review Notes

- Spec coverage (Phase 2 section 2a-2h): 2a PBKDF2 → Tasks 2,3; 2b DKIM → Task 5; 2c constant-time → Task 4 Step 3; 2d AES-GCM → Task 4; 2e strict TLS → Task 6; 2f INI admin password → Task 3 Step 3 (server side; installer side is Phase 4); 2g self-test hardening → Task 1; 2h drop API pin → Task 7.
- Ordering rationale: Task 1 builds the verification gate before any crypto lands; Task 7 is last because it depends on all legacy digest C-API usage being gone.
- TDD shape: C++ has no external test runner here — the in-process tester classes ARE the tests, and `/SelfTest` on CI is the gate. Each crypto task adds assertions to `HashCreatorTester` and relies on the CI self-test step (made blocking in Task 1) as red→green.
- Deferred to Phase 4 (installer): PBKDF2 admin password written by installer, INI ACL hardening, VC++ redist refresh, .NET 4.8 retarget, InnoSetup 6, SQL CE / libmysql decisions. Tracked in spec Phase 4.
- Known risk: several implementations (COMAuthentication rewrite, DKIM PermFail mechanism, COM error idiom, IniFileSettings SecretKey ordering) require reading surrounding code to match existing contracts — those steps say "show it in your report" rather than prescribing exact lines, because the exact call shape isn't visible from this plan's vantage. Implementers must read before editing and escalate if the contract is ambiguous.
