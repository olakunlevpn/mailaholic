// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "DiagnosticResult.h"

namespace MA
{

   class TestMXRecords
   {
   public:
	   TestMXRecords(const String &localDomainName);
	   virtual ~TestMXRecords();

      DiagnosticResult PerformTest();

   private:

      String local_domain_name_;
   };


}
