// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class TLD : public Singleton<TLD>
   {
   public:

      TLD(void);
      ~TLD(void);

      void Initialize();
      bool IsTLD(const String &sName);

      bool GetDomainNameFromHost(String &sHost, bool &bIsIPAddress);

   private:

      std::set<String> tld_;
   };
}