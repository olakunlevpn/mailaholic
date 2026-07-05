// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once


namespace MA
{
   class IPrerequisite;

   class PrerequisiteList
   {
   public:
      PrerequisiteList(void);
      ~PrerequisiteList(void);

      bool Ensure(std::shared_ptr<DALConnection> connection, int scriptDatabaseVersion, String &sErrorMessage);

   private:

      void Initialize_();

      std::vector<std::pair<int, std::shared_ptr<IPrerequisite > > > prerequisites_;
      
   };
}