// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"

#include "SURBLServers.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   SURBLServers::SURBLServers()
   {
   }

   SURBLServers::~SURBLServers(void)
   {
   }


   void 
   SURBLServers::Refresh()
   //---------------------------------------------------------------------------()
   // DESCRIPTION:
   // Reads all SURBL servers from the database.
   //---------------------------------------------------------------------------()
   {
      String sSQL = String("select * from hm_surblservers order by surblid asc");
      DBLoad_(sSQL);
   }
}