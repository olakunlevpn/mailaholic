// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class DefaultDomain
   {
   public:
      DefaultDomain(void);
      ~DefaultDomain(void);

      static String ApplyDefaultDomain(const String &sAddress);
   };
}