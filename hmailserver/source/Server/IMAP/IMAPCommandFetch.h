// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "IMAPCommand.h"

namespace MA
{
   class IMAPConnection;

   class IMAPCommandFETCH : public MA::IMAPCommand
   {
   public:
	   IMAPCommandFETCH();
	   virtual ~IMAPCommandFETCH();

      virtual IMAPResult ExecuteCommand(std::shared_ptr<IMAPConnection> pConnection, std::shared_ptr<IMAPCommandArgument> pArgument);

   };

}