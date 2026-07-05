// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Domain;

   class PlusAddressing
   {
   public:
      PlusAddressing(void);
      ~PlusAddressing(void);
   
      static String ExtractAccountAddress(const String &sAddress, std::shared_ptr<const Domain> pDomain);
      
   private:
   };
}