// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class IncomingRelay;
   enum PersistenceMode;

   class PersistentIncomingRelay
   {
   public:
	   PersistentIncomingRelay();
	   virtual ~PersistentIncomingRelay();

      static bool DeleteObject(std::shared_ptr<IncomingRelay> pSR);
      static bool SaveObject(std::shared_ptr<IncomingRelay> pSR);
      static bool SaveObject(std::shared_ptr<IncomingRelay> pSR, String &errorMessage, PersistenceMode mode);

      static bool ReadObject(std::shared_ptr<IncomingRelay> pSR, std::shared_ptr<DALRecordset> pRS);
      static bool ReadObject(std::shared_ptr<IncomingRelay> pSR, const SQLCommand &command);
      static bool ReadObject(std::shared_ptr<IncomingRelay> pSR, int ObjectID);
   };

}
