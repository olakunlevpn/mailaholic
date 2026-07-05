// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class RegularExpression
   {
   public:
      RegularExpression(void);
      ~RegularExpression(void);

      static bool TestExactMatch(const String &sExpression, const String &sValue);

      
   };

   class RegularExpressionTester
   {
   public:
      RegularExpressionTester() {}; 

      static void Test();
   };
}