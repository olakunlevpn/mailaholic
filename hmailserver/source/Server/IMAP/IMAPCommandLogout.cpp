// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "IMAPCommandLogout.h"
#include "IMAPConnection.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   IMAPResult
   IMAPCommandLOGOUT::ExecuteCommand(std::shared_ptr<IMAPConnection> pConnection, std::shared_ptr<IMAPCommandArgument> pArgument)
   {
      String sResponse = "* BYE Have a nice day\r\n"; 
      sResponse += pArgument->Tag() + " OK Logout completed\r\n";

      pConnection->Logout(sResponse);

      return IMAPResult(IMAPResult::ResultOKSupressRead, "");
   }
}