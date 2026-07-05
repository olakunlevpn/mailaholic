// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"

namespace MA
{
   class DistributionListRecipient;
   class PersistentDistributionListRecipient;

   class DistributionListRecipients : public Collection<DistributionListRecipient, PersistentDistributionListRecipient>
   {
   public:
      DistributionListRecipients(__int64 iListID);
      ~DistributionListRecipients(void);

      void Refresh();

   protected:

      virtual String GetCollectionName() const {return "DistributionList"; }
      virtual bool PreSaveObject(std::shared_ptr<DistributionListRecipient> pListRecipient, XNode *node);

   private:

      __int64 list_id_;
   };
}
