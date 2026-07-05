// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class MessageData;
   class SURBLServer;

   class SURBL
   {
   public:
      SURBL(void);
      ~SURBL(void);

      bool ExtractUrls(std::shared_ptr<MessageData> pMessageData, std::vector<String> &vecUrls);
      bool Run(std::shared_ptr<SURBLServer> pSURBLServer, std::vector<String> &vecUrls);

   private:

      void CleanURL_(String &sURL) const;
      bool CleanHost_(String &sDomain) const;
   };
}