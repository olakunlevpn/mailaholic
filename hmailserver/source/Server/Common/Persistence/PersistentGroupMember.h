// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class GroupMember;
   enum PersistenceMode;

   class PersistentGroupMember
   {
   public:
      PersistentGroupMember(void);
      ~PersistentGroupMember(void);
      
      static bool DeleteByAccount(__int64 iAccountID);
      static bool DeleteObject(std::shared_ptr<GroupMember> pObject);
      static bool SaveObject(std::shared_ptr<GroupMember> pObject);
      static bool SaveObject(std::shared_ptr<GroupMember> pObject, String &errorMessage, PersistenceMode mode);
      static bool ReadObject(std::shared_ptr<GroupMember> pObject, std::shared_ptr<DALRecordset> pRS);

   };
}