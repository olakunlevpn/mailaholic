// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"

#include "BlockedAttachments.h"

#include "../Persistence/PersistentBlockedAttachment.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   BlockedAttachments::BlockedAttachments()
   {
   }

   BlockedAttachments::~BlockedAttachments(void)
   {
   }


   void 
   BlockedAttachments::Refresh()
   //---------------------------------------------------------------------------()
   // DESCRIPTION:
   // Reads all SURBL servers from the database.
   //---------------------------------------------------------------------------()
   {
      String sSQL = _T("select * from hm_blocked_attachments order by bawildcard asc");
      DBLoad_(sSQL);
   }

}