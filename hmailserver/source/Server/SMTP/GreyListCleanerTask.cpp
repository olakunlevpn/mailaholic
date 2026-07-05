// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"

#include "GreyListCleanerTask.h"

#include "../Common/AntiSpam/AntiSpamConfiguration.h"
#include "../Common/Persistence/PersistentGreyList.h"


#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   GreyListCleanerTask::GreyListCleanerTask(void)
   {
   }

   GreyListCleanerTask::~GreyListCleanerTask(void)
   {
   }
   
   void
   GreyListCleanerTask::DoWork()
   //---------------------------------------------------------------------------()
   // DESCRIPTION:
   // Remove old records from the greylist
   //---------------------------------------------------------------------------()
   {
      // Are we using grey-listing?
      AntiSpamConfiguration &antiSpamConfig = Configuration::Instance()->GetAntiSpamConfiguration();

      if (!antiSpamConfig.GetUseGreyListing())
      {
         // Grey listing is not used. Don't delete old records.
         return;
      }

      LOG_DEBUG("GreyListCleanerTask::DoWork()");
      
      // Check if we should temporarily disable grey listing while we
      // run this task. We might need to do this if the task takes a
      // long time and causes the entire greylisting table to become locked.
      bool bGreylistingEnabledDuringExpiration = IniFileSettings::Instance()->GetGreylistingEnabledDuringRecordExpiration();

      if (!bGreylistingEnabledDuringExpiration)
         antiSpamConfig.SetGreylistingTemporarilyDisabled(true);

      PersistentGreyList::ClearExpiredRecords();      
      
      if (!bGreylistingEnabledDuringExpiration)
         antiSpamConfig.SetGreylistingTemporarilyDisabled(false);
   }

}