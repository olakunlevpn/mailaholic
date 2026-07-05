// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "VirusScanningResult.h"

namespace MA
{
   class ClamWinVirusScanner  
   {
   public:
	   ClamWinVirusScanner();
	   virtual ~ClamWinVirusScanner();

      static VirusScanningResult Scan(const String &sFilename);
      static VirusScanningResult Scan(const String &scannerExecutable, const String &databasePath, const String &sFilename);

   protected:
      
   private:


   };
}
