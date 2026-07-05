// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{

   class TestConnect
   {
   public:

      bool PerformTest(ConnectionSecurity connection_security, const String &localAddressStr, const String &server, int port, String &result);

   };


}
