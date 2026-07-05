// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class InboxIDCache
   {
   public:
      InboxIDCache(void);
      virtual ~InboxIDCache(void);

      void Clear();

      __int64 GetUserInboxFolder(__int64 accountID);

   private:

      boost::recursive_mutex mutex_;
      std::map<__int64, __int64 > inbox_id_;

      bool enabled_;
   };
}