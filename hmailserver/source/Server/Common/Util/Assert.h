// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Assert
   {
   public:
      Assert(void);
      ~Assert(void);

      static void IsTrue(bool argument);
      static void IsFalse(bool argument);
      static void AreEqual(const String &str1, const String &str2);

   private:

    
   };
}