// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"

#include "DistributionList.h"
#include "..\Persistence\PersistentDistributionList.h"

namespace MA
{
   class DistributionLists : public Collection<DistributionList, PersistentDistributionList>
   {
   public:
      DistributionLists(__int64 iDomainID);
      ~DistributionLists(void);

      std::shared_ptr<DistributionList> GetItemByAddress(const String & sAddress);
      void Refresh();

   protected:
      virtual bool PreSaveObject(std::shared_ptr<DistributionList> pDistributionList, XNode *node);
      virtual String GetCollectionName() const {return "DistributionLists"; }
   private:

      __int64 domain_id_;
   
   };
}
