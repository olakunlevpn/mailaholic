// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "IMAPCommand.h"

namespace MA
{
   class IMAPFolder;

   class IMAPCommandCREATE : public IMAPCommand
   {
      virtual IMAPResult ExecuteCommand(std::shared_ptr<MA::IMAPConnection> pConnection, std::shared_ptr<IMAPCommandArgument> pArgument);

   private:

      IMAPResult ConfirmPossibleToCreate(std::shared_ptr<MA::IMAPConnection> pConnection, const std::vector<String> &vecNewPath, bool bIsPublicFolder);
   };
}