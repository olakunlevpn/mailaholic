// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"

#include "DeliveryTask.h"
#include "SMTPDeliverer.h"



#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   DeliveryTask::DeliveryTask(std::shared_ptr<Message> pMessage) :
      Task("DeliveryTask"),
      message_(pMessage)
   {
      
   }

   DeliveryTask::~DeliveryTask(void)
   {
   }

   void 
   DeliveryTask::DoWork()
   {
      // Do our delivery work.
      SMTPDeliverer::DeliverMessage(message_);
   }

}