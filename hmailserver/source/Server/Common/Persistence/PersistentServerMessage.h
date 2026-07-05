// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class ServerMessage;

   class PersistentServerMessage
   {
   public:
      PersistentServerMessage(void);
      ~PersistentServerMessage(void);
      
      static bool DeleteObject(std::shared_ptr<ServerMessage> pObject);
      static bool SaveObject(std::shared_ptr<ServerMessage> pObject);
      static bool ReadObject(std::shared_ptr<ServerMessage> pObject, std::shared_ptr<DALRecordset> pRS);

   };
}