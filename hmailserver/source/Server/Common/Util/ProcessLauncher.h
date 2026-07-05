// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class ProcessLauncher
   {
   public:
      ProcessLauncher(const String &commandLine, const String &workingDirectory);
      ProcessLauncher(const String &commandLine);
      ~ProcessLauncher(void);

      bool Launch(unsigned int &exitCode);

      void SetErrorLogTimeout(unsigned int milliseconds);

   private:

      unsigned int error_log_timeout_;

      String command_line_;
      String working_directory_;
   };
}