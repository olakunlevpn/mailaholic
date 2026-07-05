// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "IMAPCommand.h"

namespace MA
{
   class IMAPFolder;

   class IMAPCommandDELETE : public IMAPCommand
   {
      virtual IMAPResult ExecuteCommand(std::shared_ptr<MA::IMAPConnection> pConnection, std::shared_ptr<IMAPCommandArgument> pArgument);

   private:

      void RemoveFolder_( std::shared_ptr<IMAPFolder> pFolder, std::shared_ptr<MA::IMAPConnection>  pConnection );
   };
}