// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"

#include "../Persistence/PersistentDomain.h"
#include "Domain.h"

namespace MA
{
   class Domains : public Collection<Domain, PersistentDomain>
   {
   public:
	   Domains();
	   virtual ~Domains();

      void Refresh();
      void Refresh(__int64 iDomainID);
      String GetNames();

   protected:
      virtual String GetCollectionName() const {return "Domains"; }
   private:

   };
}
