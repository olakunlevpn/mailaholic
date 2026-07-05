// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include <Winsvc.h>

namespace MA
{
   class ServiceManager  
   {
   public:
	   ServiceManager();
	   virtual ~ServiceManager();

      bool RegisterService(const String &ServiceName, const String &ServiceCaption);
      bool UnregisterService(const String &ServiceName);
      void MakeDependentOn(const String &ServiceName);

      bool StartServiceOnLocalComputer(const String &ServiceName);
      bool StopServiceOnLocalComputer(const String &ServiceName);

      SERVICE_STATUS GetServiceStatus(const String &ServiceName);

      bool UserControlService(const String &ServiceName, DWORD OpCode);
      bool DoesServiceExist(const String &ServiceName);

   private:

      bool ReconfigureService_(SC_HANDLE hSCMManager, const String &ServiceName);
   };
}
