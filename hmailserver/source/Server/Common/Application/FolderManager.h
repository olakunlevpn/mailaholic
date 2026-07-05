// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Message;
   class Account;
   class IMAPFolder;

   class FolderManager
   {
   public:
      FolderManager(void);
      ~FolderManager(void);

      bool GetInboxMessages(int accountID, std::vector<std::shared_ptr<Message> > &result);
      bool DeleteInboxMessages(int accountID, std::set<int> uids, const std::function<void()> &callbackEvery1000Message);

      bool UpdateMessageFlags(int accountID, int folderID, __int64 messageID, int flags);

	private:


   };
}