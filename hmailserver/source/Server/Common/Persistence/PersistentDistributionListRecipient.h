// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../BO/DistributionListRecipient.h"

namespace MA
{  
   enum PersistenceMode;

   class PersistentDistributionListRecipient
   {
   public:
      PersistentDistributionListRecipient(void);
      ~PersistentDistributionListRecipient(void);

      static bool DeleteByListID(__int64 iListID);
      static bool DeleteObject(std::shared_ptr<DistributionListRecipient> pRecipient);
      
      static bool SaveObject(std::shared_ptr<DistributionListRecipient> pRecipient);
      static bool SaveObject(std::shared_ptr<DistributionListRecipient> pRecipient, String &sErrorMessage, PersistenceMode mode);

      static bool ReadObject(std::shared_ptr<DistributionListRecipient> pRecipient, long ObjectID);
      static bool ReadObject(std::shared_ptr<DistributionListRecipient> pRecipient, const SQLCommand &command);
      static bool ReadObject(std::shared_ptr<DistributionListRecipient> pRecipient, std::shared_ptr<DALRecordset> pRS);

   };
}
