// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class ExceptionHandler
   {
   public:
      ExceptionHandler();

      static bool Run(const String &descriptive_name, boost::function<void()>& functionToRun);

   private:

      static void RunWithStandardExceptions(const String &descriptive_name, boost::function<void()>& functionToRun);

      static String GetExceptionText(const String &descriptive_name);

   };
}