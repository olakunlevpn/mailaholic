// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class FetchAccountUID;
   enum PersistenceMode;

   class PersistentFetchAccountUID
   {
   public:
      PersistentFetchAccountUID(void);
      ~PersistentFetchAccountUID(void);

      static __int64 AddUID(__int64 iFAID, const String &sValue);
      static bool DeleteUID(__int64 iUIDID);
      
      static bool SaveObject(std::shared_ptr<FetchAccountUID> pUID, String &result, PersistenceMode mode);
      static bool DeleteObject(std::shared_ptr<FetchAccountUID> fetchAccountUID);
   };
}