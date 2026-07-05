// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"
#include "FetchAccountUID.h"
#include "../Persistence/PersistentFetchAccountUID.h"

namespace MA
{
   class FetchAccountUID;

   class FetchAccountUIDs : public Collection<FetchAccountUID, PersistentFetchAccountUID>
   {
   public:
      FetchAccountUIDs(void);
      ~FetchAccountUIDs(void);

      void Refresh(__int64 iFAID);

      bool PreSaveObject(std::shared_ptr<FetchAccountUID> pUID, XNode *node);

   protected:
      virtual String GetCollectionName() const {return "FetchAccountUIDs"; }

   private:

      std::map<String, std::shared_ptr<FetchAccountUID> > fetched_uids_;

      __int64 faid_;
   };
}
