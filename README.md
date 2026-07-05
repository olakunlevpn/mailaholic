Mailaholic
==========

Mailaholic is an open source email server for Microsoft Windows.

**Features:**
- PBKDF2-HMAC-SHA256 password hashing (600k iterations)
- AES-256-GCM for reversible secrets
- TLS 1.2 minimum, security level 2
- OpenSSL 3.5.x with modern EVP API
- VS2022/v143 toolchain
- GitHub Actions CI

Building Mailaholic
===================

Environment set up
------------------

**Required software**

   * Visual Studio 2022 Community edition
   * InnoSetup 6 (https://jrsoftware.org/isinfo.php)
   * Perl 5 (https://strawberryperl.com/)
   
Installing Visual Studio 2022 Community edition
------------------------------------------------

1. Download [Visual Studio 2022](https://visualstudio.microsoft.com/vs/) and launch the installation.
2. Select the following _Workloads_
  * .NET desktop development
  * Desktop development with C++
3. Select the following _Individual components_
  * C++ ATL for latest v143 build tools (x86 & x64)
  * Windows 11 SDK (latest)

3rd party libraries
-------------------

Create an environment variable named `MailaholicLibs` pointing at a folder where you will store libraries, such as `C:\Dev\MailaholicLibs`.

Building OpenSSL
----------------
1. Download OpenSSL 3.5.x from http://www.openssl.org/source/ and put it into `%MailaholicLibs%\<OpenSSL-Version>`.
   You should now have a folder named `%MailaholicLibs%\<OpenSSL-version>`, for example `C:\Dev\MailaholicLibs\openssl-3.5.5`
2. Start a x64 Native Tools Command Prompt for VS2022.
3. Change dir to `%MailaholicLibs%\<OpenSSL-version>`.
4. Run the following commands:

   ```
   Perl Configure no-asm VC-WIN64A --prefix=%cd%\out64 --openssldir=%cd%\out64 -D_WIN32_WINNT=0x600
   nmake clean
   nmake install_sw
   ```

Building PostgreSQL
-------------------
1. Download PostgreSQL 15.18 source from https://www.postgresql.org/ftp/source/v15.18/ and put it into `%MailaholicLibs%\postgresql-15.18`.
2. Start a x64 Native Tools Command Prompt for VS2022.
3. Change dir to `%MailaholicLibs%`
4. Run the following commands:

   ```
   cd src\tools\msvc
   perl build.pl Release libpq
   ```

Building Boost
--------------
1. Download Boost 1.90.0 from http://www.boost.org/ and put it into `%MailaholicLibs%\<Boost-Version>`.
2. Start a x64 Native Tools Command Prompt for VS2022.
3. Change dir to `%MailaholicLibs%\<Boost-Version>`.
4. Run the following commands:

   ```
   bootstrap
   b2 debug release threading=multi link=static --with-thread --with-filesystem --with-regex --with-chrono --with-system --with-atomic --toolset=msvc-14.3 address-model=64 stage --build-dir=out64 -j 4
   ```

Building Mailaholic
-------------------

Visual Studio 2022 must be started with _Run as Administrator_.

1. Download the source code from this Git repository.
2. Compile the solution `source\Server\Mailaholic\Mailaholic.sln`.
   This builds the server (Mailaholic.exe)
3. Compile the solution `source\Tools\Mailaholic Tools.sln`.
   This builds tools (Mailaholic Administrator, DBSetup, DBUpdater, etc.)
4. Compile `installation\Mailaholic64.iss` (using InnoSetup 6)
   This builds the installer.

Running in Debug
----------------

To run in debug mode in Visual Studio, add the command argument /debug in Project properties under Configuration Properties -> Debugging.

Running tests
-------------

```
Mailaholic.exe /SelfTest
```

This runs built-in crypto and environment validation tests.

License
=======

Mailaholic is released under the GNU General Public License. See LICENSE file.
