// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "DALConnectionFactory.h"
#include "ADOConnection.h"
#include "MySQLConnection.h"
#include "PGConnection.h"
#include "SQLCEConnection.h"
#include "DatabaseSettings.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{

   DALConnectionFactory::DALConnectionFactory()
   {

   }

   DALConnectionFactory::~DALConnectionFactory()
   {

   }

   std::shared_ptr<DALConnection>
   DALConnectionFactory::CreateConnection(std::shared_ptr<DatabaseSettings> pSettings)
   {
      std::shared_ptr<DALConnection> pConn;
      
      MA::DatabaseSettings::SQLDBType t = pSettings->GetType();

     switch (t)
      {
      case MA::DatabaseSettings::TypeMSSQLServer:
         pConn = std::shared_ptr<ADOConnection>(new ADOConnection(pSettings));
         break;
      case MA::DatabaseSettings::TypeMYSQLServer:
         pConn = std::shared_ptr<MySQLConnection>(new MySQLConnection(pSettings));
         break;
      case MA::DatabaseSettings::TypePGServer:
         pConn = std::shared_ptr<PGConnection>(new PGConnection(pSettings));
         break;
      case MA::DatabaseSettings::TypeMSSQLCompactEdition:
         pConn = std::shared_ptr<SQLCEConnection>(new SQLCEConnection(pSettings));
         break;
      }
   
      return pConn;
   }
}