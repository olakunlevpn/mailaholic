// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"
#include "SSPIValidation.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   bool 
   SSPIValidation::ValidateUser(const String &sDomain, const String &sUsername, const String &sPassword)
   {
	  HANDLE token;
      BOOL result = LogonUser(sUsername, sDomain, sPassword, LOGON32_LOGON_NETWORK, LOGON32_PROVIDER_DEFAULT, &token);
	  CloseHandle(token);

	  return result != 0;
   }
}