// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "PersistentMessageRecipient.h"


#include "../BO/MessageRecipient.h"


#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   PersistentMessageRecipient::PersistentMessageRecipient(void)
   {
   }

   PersistentMessageRecipient::~PersistentMessageRecipient(void)
   {
   }


   bool 
   PersistentMessageRecipient::DeleteObject(std::shared_ptr<MessageRecipient> pRecipient)
   {
      SQLCommand command("delete from hm_messagerecipients where recipientaddress = @ADDRESS AND recipientmessageid = @MESSAGEID");
      command.AddParameter("@ADDRESS", pRecipient->GetAddress());
      command.AddParameter("@MESSAGEID", pRecipient->GetMessageID());

      return Application::Instance()->GetDBManager()->Execute(command);

   }
}