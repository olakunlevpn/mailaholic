// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "DiagnosticResult.h"

namespace MA
{
   
   class Diagnostic
   {
   public:
	   Diagnostic();
	   virtual ~Diagnostic();
  
      void SetLocalDomain(String &sDomainName);
      void SetTestDomain(String &sTestDomainName);
      String GetLocalDomain() const;
      String GetTestDomain() const;

      std::vector<DiagnosticResult> PerformTests();

   private:

      String local_domain_name_;
      String local_test_domain_name_;
   };


}
