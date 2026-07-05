// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class WhiteListAddress;
   enum PersistenceMode;

   class PersistentWhiteListAddress
   {
   public:
      PersistentWhiteListAddress(void);
      ~PersistentWhiteListAddress(void);
      
      static bool DeleteObject(std::shared_ptr<WhiteListAddress> pObject);
      static bool SaveObject(std::shared_ptr<WhiteListAddress> pObject, String &errorMessage, PersistenceMode mode);
      static bool SaveObject(std::shared_ptr<WhiteListAddress> pObject);
      static bool ReadObject(std::shared_ptr<WhiteListAddress> pObject, std::shared_ptr<DALRecordset> pRS);
   };
}