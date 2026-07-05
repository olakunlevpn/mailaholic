// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Message;
   
   class MailerDaemonAddressDeterminer  
   {
   public:
	   MailerDaemonAddressDeterminer();
	   virtual ~MailerDaemonAddressDeterminer();

      static String GetMailerDaemonAddress(const std::shared_ptr<Message> pOrigMessage);
      static String GetMailerDaemonAddress(const String &sOrigSender, const String &sOrigReceiver);

      static bool IsMailerDaemonAddress(const String &sAddress);
   };

}
