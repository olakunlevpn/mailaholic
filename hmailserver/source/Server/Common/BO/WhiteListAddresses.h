// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"

#include "WhiteListAddress.h"
#include "../Persistence/PersistentWhiteListAddress.h"

namespace MA
{
   class WhiteListAddresses : public Collection<WhiteListAddress, PersistentWhiteListAddress>
   {
   public:
      WhiteListAddresses();
      ~WhiteListAddresses(void);

      // Refreshes this collection from the database.
      void Refresh();

   protected:
   
      virtual String GetCollectionName() const {return "WhiteListAddresses"; }

   private:
     
   };
}