// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"

#include "GreyListingWhiteAddress.h"
#include "../Persistence/PersistentGreyListingWhiteAddress.h"

namespace MA
{
   class GreyListingWhiteAddresses : public Collection<GreyListingWhiteAddress, PersistentGreyListingWhiteAddress>
   {
   public:
      GreyListingWhiteAddresses();
      ~GreyListingWhiteAddresses(void);

      // Refreshes this collection from the database.
      void Refresh();

   protected:
      virtual String GetCollectionName() const {return "GreyListingWhiteAddresses"; }

   private:
     
   };
}