// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "DiagnosticResult.h"

namespace MA
{

   class TestIPv6
   {
   public:
	   TestIPv6();
	   virtual ~TestIPv6();

      DiagnosticResult PerformTest();

   private:
   };


}
