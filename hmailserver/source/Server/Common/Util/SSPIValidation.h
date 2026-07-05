// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class SSPIValidation
   {
   public:
	   SSPIValidation();
	   virtual ~SSPIValidation();

      static bool ValidateUser(const String &sDomain, const String &sUsername, const String &sPassword);

   };

}
