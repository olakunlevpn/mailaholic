// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "COMError.h"
#include "InterfaceEventLog.h"


STDMETHODIMP 
InterfaceEventLog::Write(BSTR sMessage)
{
   try
   {
      MA::Logger::Instance()->LogEvent(sMessage);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}


