// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"

namespace MA
{
   class ServerMessage;
   class PersistentServerMessage;

   class ServerMessages : public Collection<ServerMessage, PersistentServerMessage>
   {
   public:
      ServerMessages();
      ~ServerMessages(void);

      // Refreshes this collection from the database.
      void Refresh();

      String GetMessage(const String &sName) const;
   
   protected:
      virtual String GetCollectionName() const {return "ServerMessages"; }
   private:
     
   };
}