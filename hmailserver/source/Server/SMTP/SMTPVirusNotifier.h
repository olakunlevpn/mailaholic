// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once


namespace MA
{
   class Message;
   
   class SMTPVirusNotifier  
   {
   public:
	   SMTPVirusNotifier();
	   virtual ~SMTPVirusNotifier();

      static void CreateMessageDeletedNotification(const std::shared_ptr<Message> pOriginalMessage, const String &sRecipient);
      
   };

}
