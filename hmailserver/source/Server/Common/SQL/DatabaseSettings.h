// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{

   class DatabaseSettings  
   {
   public:
   
      enum SQLDBType
      {
         TypeUnknown      = 0,
         TypeMYSQLServer  = 1,
         TypeMSSQLServer  = 2,
         TypePGServer = 3,
         TypeMSSQLCompactEdition = 4
      };

	   DatabaseSettings(const String &sDatabaseProvider, const String &sDatabaseServer, const String &sDatabaseName, const String &sUsername, const String &sPassword,
			const String &sDatabaseDirectory, const String &sDatabaseServerFailoverPartner, MA::DatabaseSettings::SQLDBType dbType, long lDBPort);
	   virtual ~DatabaseSettings();

      String GetProvider() { return database_provider_; }
      String GetServer() {return database_server_; }
      String GetDatabaseName() {return database_name_;}
      String GetUsername() {return username_;}
      String GetPassword() {return password_;}
      String GetDatabaseDirectory() {return database_directory_;}
      MA::DatabaseSettings::SQLDBType GetType() {return sqldbtype_; }
      int GetPort() {return dbport_; }
      String GetDatabaseServerFailoverPartner() const { return database_server_failover_partner_; }
   
      String GetDefaultScript();
	  
      static String GetDatabaseTypeName(MA::DatabaseSettings::SQLDBType type);

   private:

      String database_provider_;
      String database_server_;
      String database_name_;
      String username_;
      String password_;
      String database_directory_;
      String database_server_failover_partner_;
      MA::DatabaseSettings::SQLDBType sqldbtype_;
      long dbport_;
   };

}
