// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Message;

   class MirrorMessage
   {
   public:
      MirrorMessage(std::shared_ptr<Message> message);
      ~MirrorMessage(void);

      void Send();

   private:

      std::shared_ptr<Message> message_;

   };
}