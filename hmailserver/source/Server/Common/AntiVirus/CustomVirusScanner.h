// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "VirusScanningResult.h"

namespace MA
{
   class CustomVirusScanner
   {
   public:
      CustomVirusScanner(void);
      ~CustomVirusScanner(void);

      static VirusScanningResult Scan(const String &executablePath, int virusReturnCode, const String &sFilename);
      static VirusScanningResult Scan(const String &sFilename);
   };

}