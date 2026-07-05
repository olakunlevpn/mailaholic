// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{

   class DatabaseSettings;

   class SQLScriptParser  
   {
   public:
      SQLScriptParser(std::shared_ptr<DatabaseSettings> pSettings, const String &sFile);
      virtual ~SQLScriptParser();

      bool Parse(String &sErrorMessage);

      int GetNoOfCommands() {return (int)commands_.size(); }
      String GetCommand(int index) {return commands_[index]; }

   private:

      bool PreprocessLine_(String &sLine);

      std::shared_ptr<DatabaseSettings> settings_;
      String file_;
      std::vector<String> commands_;

   };

}
