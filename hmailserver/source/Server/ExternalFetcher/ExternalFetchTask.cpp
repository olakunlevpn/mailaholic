// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"
#include ".\externalfetchtask.h"

#include "ExternalFetch.h"
#include "..\Common\BO\FetchAccount.h"
#include "../Common/Persistence/PersistentFetchAccount.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   ExternalFetchTask::ExternalFetchTask(std::shared_ptr<FetchAccount> pFA) : 
      Task("ExternalFetchTask"),
      fetch_account_(pFA)
   {
   }

   ExternalFetchTask::~ExternalFetchTask(void)
   {
   }

   void 
   ExternalFetchTask::DoWork()
   {
      // Do the actual delivery of the message.
      ExternalFetch oFetcher;
      oFetcher.Start(fetch_account_);

      // Set next fetch time 
      PersistentFetchAccount::SetNextTryTime(fetch_account_);

      // Unlock the account
      PersistentFetchAccount::Unlock(fetch_account_->GetID());
   }

}