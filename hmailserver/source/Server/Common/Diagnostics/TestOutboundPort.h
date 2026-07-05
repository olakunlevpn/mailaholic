// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "DiagnosticResult.h"

namespace MA
{

   class TestOutboundPort
   {
   public:
	   TestOutboundPort(const String &TestDomainName);
	   virtual ~TestOutboundPort();

      DiagnosticResult PerformTest();

   private:

      String local_test_domain_name_;

   };


}
