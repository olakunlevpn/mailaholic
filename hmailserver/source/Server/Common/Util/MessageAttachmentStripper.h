// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Message;
   class MimeBody;
   class CMimeMessage;
   
   
   class MessageAttachmentStripper  
   {
   public:
	   MessageAttachmentStripper();
	   virtual ~MessageAttachmentStripper();

      static void Strip(std::shared_ptr<Message> pMessage);
   private:
      
      static void WriteToDisk_(std::shared_ptr<Message> pMessage, MimeBody &oMainMessage, std::shared_ptr<MimeBody> pBody);
      static bool IsGoodTextPart_(std::shared_ptr<MimeBody> pBody);
   };

}