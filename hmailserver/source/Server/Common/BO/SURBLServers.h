// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"

#include "SURBLServer.h"
#include "../Persistence/PersistentSURBLServer.h"

namespace MA
{

   class SURBLServers : public Collection<SURBLServer, PersistentSURBLServer>
   {
   public:
      SURBLServers();
      ~SURBLServers(void);

      // Refreshes this collection from the database.
      void Refresh();

   protected:
      virtual String GetCollectionName() const {return String("SURBLServers"); }
   private:
     
   };
}