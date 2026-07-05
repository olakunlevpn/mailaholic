// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "IMAPCommand.h"

namespace MA
{
   class IMAPCommandSUBSCRIBE : public IMAPCommand
   {
      virtual IMAPResult ExecuteCommand(std::shared_ptr<MA::IMAPConnection> pConnection, std::shared_ptr<IMAPCommandArgument> pArgument);
   };


}

