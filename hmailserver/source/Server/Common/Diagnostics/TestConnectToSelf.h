// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "DiagnosticResult.h"

namespace MA
{

   class TestConnectToSelf
   {
   public:
	   TestConnectToSelf(const String &localDomainName);
	   virtual ~TestConnectToSelf();

      DiagnosticResult PerformTest();

   private:

      String local_domain_name_;
   };


}
