// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"

#include "VirusScanningResult.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   

   VirusScanningResult::VirusScanningResult(ScanningResult result, const String &details) :
      result_(result),
      details_(details)
   {
      
   }

   VirusScanningResult::VirusScanningResult(const String &errorMessageSource, const String &errorMessage) :
      result_(ErrorOccurred),
      details_(errorMessage),
      message_source_(errorMessageSource)
   {

   }
}
