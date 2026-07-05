// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"

#include "../Persistence/PersistentAlias.h"
#include "../BO/Alias.h"

namespace MA
{
  
   class Aliases : public Collection<Alias, PersistentAlias>
   {
   public:
	   Aliases(__int64 iDomainID);
	   virtual ~Aliases();

      // Deletes all aliases in this collection
      void Refresh();
            
   protected:
      virtual bool PreSaveObject(std::shared_ptr<Alias> pAlias, XNode *node);
      virtual String GetCollectionName() const {return "Aliases"; }
   private:

      __int64 domain_id_;

   };
}
