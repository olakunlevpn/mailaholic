// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class GroupMembers;
   class Groups;

   class Group : public BusinessObject<Group>
   {
   public:
      Group(void);
      ~Group(void);

      String GetName() const;
      void SetName(const String &sName);

      std::shared_ptr<GroupMembers> GetMembers();
      
      bool UserIsMember(__int64 iAccountID);

      bool XMLStore(XNode *pParentNode, int iOptions);
      bool XMLLoad(XNode *pParentNode, int iRestoreOptions);
      bool XMLLoadSubItems(XNode *pParentNode, int iRestoreOptions);

   private:

      String name_;
   };
}