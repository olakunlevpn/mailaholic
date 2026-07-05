// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"

#include "SSLCertificates.h"

#include "../Persistence/PersistentSSLCertificate.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   SSLCertificates::SSLCertificates()
   {
   }

   SSLCertificates::~SSLCertificates(void)
   {
   }


   void
   SSLCertificates::Refresh()
   //---------------------------------------------------------------------------()
   // DESCRIPTION:
   // Reads all SURBL servers from the database.
   //---------------------------------------------------------------------------()
   {
      String sSQL = "select * from hm_sslcertificates order by sslcertificatename asc";
      DBLoad_(sSQL);
   }

}