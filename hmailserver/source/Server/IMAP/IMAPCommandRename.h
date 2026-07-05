// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once


#include "IMAPCommand.h"

namespace MA
{
   class IMAPFolder;

   class IMAPCommandRENAME : public IMAPCommand
   {
      virtual IMAPResult ExecuteCommand(std::shared_ptr<MA::IMAPConnection> pConnection, std::shared_ptr<IMAPCommandArgument> pArgument);

   private:
      
      IMAPResult ConfirmPossibleToRename(std::shared_ptr<MA::IMAPConnection> pConnection, std::shared_ptr<IMAPFolder> pFolderToRename, const std::vector<String> &vecOldPath, const std::vector<String> &vecNewPath);

      std::shared_ptr<IMAPFolder> GetParentFolder(std::shared_ptr<MA::IMAPConnection> pConnection, const std::vector<String> &vecFolderPath);
   };

}

