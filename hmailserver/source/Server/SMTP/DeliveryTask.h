// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "..\Common\Threading\Task.h"

namespace MA
{
   class Message;

   class DeliveryTask : public Task
   {
   public:
      DeliveryTask(std::shared_ptr<Message> pMessage);
      ~DeliveryTask(void);

      virtual void DoWork();

   private:
      
      std::shared_ptr<Message> message_;
   };
}