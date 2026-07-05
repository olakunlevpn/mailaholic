// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "DiagnosticResult.h"

namespace MA
{

   class TestIPRanges
   {
   public:
	   TestIPRanges();
	   virtual ~TestIPRanges();

      DiagnosticResult PerformTest();

   private:
   };


}
