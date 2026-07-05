// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "IMAPConnection.h"
#include "IMAPCommand.h"


namespace MA
{
   class IMAPCommand;

   class StaticIMAPCommandHandlers : public Singleton<StaticIMAPCommandHandlers>
   {

   public:
	   StaticIMAPCommandHandlers();
      static std::map<IMAPConnection::eIMAPCommandType, std::shared_ptr<IMAPCommand> > &GetStaticHandlers() {return mapCommandHandlers; }

   private:
      

      static std::map<IMAPConnection::eIMAPCommandType, std::shared_ptr<IMAPCommand> > mapCommandHandlers;
   };

   class IMAPCommandUNKNOWN : public IMAPCommand
   {
      virtual IMAPResult ExecuteCommand(std::shared_ptr<MA::IMAPConnection> pConnection, std::shared_ptr<IMAPCommandArgument> pArgument);

   };

   class IMAPCommandNOOP : public IMAPCommand
   {
      virtual IMAPResult ExecuteCommand(std::shared_ptr<MA::IMAPConnection> pConnection, std::shared_ptr<IMAPCommandArgument> pArgument);
   };



}