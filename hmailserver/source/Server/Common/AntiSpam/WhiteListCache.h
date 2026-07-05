// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class WhiteListAddress;

   class WhiteListCache
   {
   public:
      WhiteListCache(void);

      
      static void SetNeedRefresh();

      bool IsWhitelisted(const String &fromAddress, const IPAddress &address);
   

   private:

      static void Refresh();
      static std::vector<std::shared_ptr<WhiteListAddress>> whitelistItems_;
      static bool needRefresh_;
   };
}