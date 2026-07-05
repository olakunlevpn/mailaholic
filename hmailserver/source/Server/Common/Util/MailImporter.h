// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Time.h"

namespace MA
{
   class MessageData;

   class MailImporter
   {
   public:
      MailImporter(void);
      ~MailImporter(void);

      static bool Import (const String &originalFullPath, int iAccountID, const String &sIMAPFolder);
      // Imports a message from file to the database.

   private:

      static bool ReplaceMessagePath_(__int64 messageID, const String &messagePath);
      static String GetInternalDate_(std::shared_ptr<MessageData> pMessageData);
      static String GetInternalDate_DatePlusTimeZone(DateTime dtTime);
      static bool MoveToNamedSubdirectory_(const String &sourceFile, String &resultFile, String &newFullPath);
      static bool GetRootLevelDirectory_(const String &fullPath, String &rootLevel);
   };

#ifdef _DEBUG
   class MailImporterTester
   {
   public :
      MailImporterTester () {};
      ~MailImporterTester () {};      

      void Test();
   };

#endif 
}