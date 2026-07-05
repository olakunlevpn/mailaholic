// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class SURBLServer;
   enum PersistenceMode;

   class PersistentSURBLServer
   {
   public:
      PersistentSURBLServer(void);
      ~PersistentSURBLServer(void);
      
      static bool DeleteObject(std::shared_ptr<SURBLServer> pObject);
      static bool SaveObject(std::shared_ptr<SURBLServer> pObject);
      static bool SaveObject(std::shared_ptr<SURBLServer> pObject, String &errorMessage, PersistenceMode mode);
      static bool ReadObject(std::shared_ptr<SURBLServer> pObject, std::shared_ptr<DALRecordset> pRS);

   };
}