// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Registry
   {
   public:
      Registry(void);
      ~Registry(void);

      bool GetStringValue(HKEY hive, String key, String valueName, String &value);

   private:

   };
}