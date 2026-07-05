// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class ExceptionLogger
   {
   private:
      ExceptionLogger();

   public:
      static void Log(int exception_code, EXCEPTION_POINTERS* pExp);

   private:

      static void CreateMiniDump_(EXCEPTION_POINTERS* pExp, const String &file_name);

      static bool TryToMakeRoom();

      static boost::mutex logging_mutex_;
   };
}