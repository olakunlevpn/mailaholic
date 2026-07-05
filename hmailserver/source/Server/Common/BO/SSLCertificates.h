// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"
#include "SSLCertificate.h"

#include "../Persistence/PersistentSSLCertificate.h"

namespace MA
{
   class SSLCertificates : public Collection<SSLCertificate, PersistentSSLCertificate>
   {
   public:
      SSLCertificates();
      ~SSLCertificates(void);

      // Refreshes this collection from the database.
      void Refresh();

   protected:
      virtual String GetCollectionName() const {return "SSLCertificates"; }

   private:
     
   };
}