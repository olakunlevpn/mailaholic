// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"
#include "BlockedAttachment.h"

#include "../Persistence/PersistentBlockedAttachment.h"

namespace MA
{
   class BlockedAttachments : public Collection<BlockedAttachment, PersistentBlockedAttachment>
   {
   public:
      BlockedAttachments();
      ~BlockedAttachments(void);

      // Refreshes this collection from the database.
      void Refresh();

   protected:
      virtual String GetCollectionName() const {return "BlockedAttachments"; }

   private:
     
   };
}