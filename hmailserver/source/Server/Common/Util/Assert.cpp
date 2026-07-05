// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"

#include "Assert.h"
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   void
   Assert::IsTrue(bool argument)
   {
      if (!argument)
         throw;
   }

   void
   Assert::IsFalse(bool argument)
   {
      if (argument)
         throw;
   }


   void
   Assert::AreEqual(const String &str1, const String &str2)
   {
      if (str1 != str2)
         throw;

   }


}