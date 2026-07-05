// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "IPrerequisite.h"

namespace MA
{
   class PreReqNoDuplicateFolders : public IPrerequisite
   {
   public:
      PreReqNoDuplicateFolders(void);
      ~PreReqNoDuplicateFolders(void);

      int GetDatabaseVersion() {return 5200; }
      bool Ensure(std::shared_ptr<DALConnection> connection, String &sErrorMessage);


   private:
      

   };
}