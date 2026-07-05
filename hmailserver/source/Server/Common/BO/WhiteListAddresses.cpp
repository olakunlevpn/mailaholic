// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"

#include "WhiteListAddresses.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   WhiteListAddresses::WhiteListAddresses()
   {
   }

   WhiteListAddresses::~WhiteListAddresses(void)
   {
   }


   void 
   WhiteListAddresses::Refresh()
   //---------------------------------------------------------------------------()
   // DESCRIPTION:
   // Reads all SURBL servers from the database.
   //---------------------------------------------------------------------------()
   {
      String sSQL = "select * from hm_whitelist order by whiteloweripaddress1 asc, whiteloweripaddress2 asc";
      DBLoad_(sSQL);
   }



}