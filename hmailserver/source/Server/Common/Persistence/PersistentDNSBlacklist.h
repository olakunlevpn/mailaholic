// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class DNSBlackList;
   enum PersistenceMode;

   class PersistentDNSBlackList
   {
   public:
      PersistentDNSBlackList(void);
      ~PersistentDNSBlackList(void);
      
      static bool DeleteObject(std::shared_ptr<DNSBlackList> pObject);
      static bool SaveObject(std::shared_ptr<DNSBlackList> pObject, String &errorMessage, PersistenceMode mode);
      static bool SaveObject(std::shared_ptr<DNSBlackList> pObject);
      static bool ReadObject(std::shared_ptr<DNSBlackList> pObject, std::shared_ptr<DALRecordset> pRS);

   };
}