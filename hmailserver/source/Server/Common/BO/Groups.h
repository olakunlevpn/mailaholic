// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"

#include "Group.h"
#include "../Persistence/PersistentGroup.h"

namespace MA
{
   class Groups : public Collection<Group, PersistentGroup>
   {
   public:
      Groups();

      ~Groups(void);

      // Refreshes this collection from the database.
      void Refresh();

   protected:
      
      virtual String GetCollectionName() const {return "Groups"; }

   private:

   };
}