#include "stdafx.h"
#include "AdminApi.h"
#include "AuthApi.h"
#include "../BO/Domain.h"
#include "../BO/Domains.h"
#include "../BO/Account.h"
#include "../BO/Accounts.h"
#include "../Persistence/PersistentDomain.h"
#include "../Persistence/PersistentAccount.h"
#include "../Util/Crypt.h"
#include "../Util/Unicode.h"
#include "../Application/Configuration.h"
#include "../../SMTP/SMTPConfiguration.h"
#include <sstream>

namespace WebAdmin
{
   std::string AdminApi::JsonError(const std::string& message)
   {
      return "{\"error\":\"" + message + "\"}";
   }

   std::string AdminApi::JsonSuccess(const std::string& data)
   {
      return "{\"success\":true,\"data\":" + data + "}";
   }

   void AdminApi::RegisterRoutes(httplib::SSLServer& server)
   {
      // Domains
      server.Get("/api/domains", ListDomains);
      server.Post("/api/domains", CreateDomain);
      server.Put(R"(/api/domains/(\d+))", UpdateDomain);
      server.Delete(R"(/api/domains/(\d+))", DeleteDomain);

      // Accounts
      server.Get("/api/accounts", ListAccounts);
      server.Post("/api/accounts", CreateAccount);
      server.Put(R"(/api/accounts/(\d+))", UpdateAccount);
      server.Delete(R"(/api/accounts/(\d+))", DeleteAccount);

      // Dashboard
      server.Get("/api/dashboard", GetDashboard);

      // Settings
      server.Get("/api/settings", GetSettings);
      server.Put("/api/settings/smtp", UpdateSmtpSettings);
      server.Put("/api/settings/logging", UpdateLoggingSettings);
   }

   void AdminApi::ListDomains(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      auto domains = std::make_shared<MA::Domains>();
      domains->Refresh();

      std::ostringstream json;
      json << "[";

      bool first = true;
      for (int i = 0; i < domains->GetCount(); i++)
      {
         auto domain = domains->GetItem(i);
         if (!first) json << ",";
         first = false;

         json << "{";
         json << "\"id\":" << domain->GetID() << ",";
         json << "\"name\":\"" << MA::Unicode::ToANSI(domain->GetName()).c_str() << "\",";
         json << "\"isActive\":" << (domain->GetIsActive() ? "true" : "false");
         json << "}";
      }

      json << "]";
      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void AdminApi::CreateDomain(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      std::string body = req.body;

      // Parse name
      size_t namePos = body.find("\"name\"");
      if (namePos == std::string::npos)
      {
         res.status = 400;
         res.set_content(JsonError("Missing name"), "application/json");
         return;
      }
      size_t start = body.find("\"", namePos + 6) + 1;
      size_t end = body.find("\"", start);
      std::string name = body.substr(start, end - start);

      auto domain = std::make_shared<MA::Domain>();
      domain->SetName(MA::String(name.c_str()));
      domain->SetIsActive(true);

      if (!MA::PersistentDomain::SaveObject(domain))
      {
         res.status = 500;
         res.set_content(JsonError("Failed to create domain"), "application/json");
         return;
      }

      std::ostringstream json;
      json << "{\"id\":" << domain->GetID() << "}";
      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void AdminApi::UpdateDomain(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      long long id = std::stoll(req.matches[1]);

      auto domain = std::make_shared<MA::Domain>();
      if (!MA::PersistentDomain::ReadObject(domain, id))
      {
         res.status = 404;
         res.set_content(JsonError("Domain not found"), "application/json");
         return;
      }

      std::string body = req.body;

      // Parse active state
      size_t activePos = body.find("\"isActive\"");
      if (activePos != std::string::npos)
      {
         bool isActive = body.find("true", activePos) != std::string::npos;
         domain->SetIsActive(isActive);
      }

      if (!MA::PersistentDomain::SaveObject(domain))
      {
         res.status = 500;
         res.set_content(JsonError("Failed to update domain"), "application/json");
         return;
      }

      res.set_content(JsonSuccess("{}"), "application/json");
   }

   void AdminApi::DeleteDomain(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      long long id = std::stoll(req.matches[1]);

      auto domain = std::make_shared<MA::Domain>();
      if (!MA::PersistentDomain::ReadObject(domain, id))
      {
         res.status = 404;
         res.set_content(JsonError("Domain not found"), "application/json");
         return;
      }

      if (!MA::PersistentDomain::DeleteObject(domain))
      {
         res.status = 500;
         res.set_content(JsonError("Failed to delete domain"), "application/json");
         return;
      }

      res.set_content(JsonSuccess("{}"), "application/json");
   }

   void AdminApi::ListAccounts(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      // Get domain_id from query
      auto it = req.params.find("domain_id");
      if (it == req.params.end())
      {
         res.status = 400;
         res.set_content(JsonError("Missing domain_id"), "application/json");
         return;
      }

      long long domainId = std::stoll(it->second);

      auto domain = std::make_shared<MA::Domain>();
      if (!MA::PersistentDomain::ReadObject(domain, domainId))
      {
         res.status = 404;
         res.set_content(JsonError("Domain not found"), "application/json");
         return;
      }

      auto accounts = domain->GetAccounts();
      accounts->Refresh();

      std::ostringstream json;
      json << "[";

      bool first = true;
      for (int i = 0; i < accounts->GetCount(); i++)
      {
         auto account = accounts->GetItem(i);
         if (!first) json << ",";
         first = false;

         json << "{";
         json << "\"id\":" << account->GetID() << ",";
         json << "\"address\":\"" << MA::Unicode::ToANSI(account->GetAddress()).c_str() << "\",";
         json << "\"isActive\":" << (account->GetActive() ? "true" : "false") << ",";
         json << "\"adminLevel\":" << account->GetAdminLevel();
         json << "}";
      }

      json << "]";
      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void AdminApi::CreateAccount(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      std::string body = req.body;

      // Parse domain_id
      size_t domainPos = body.find("\"domain_id\"");
      if (domainPos == std::string::npos)
      {
         res.status = 400;
         res.set_content(JsonError("Missing domain_id"), "application/json");
         return;
      }
      size_t start = body.find(":", domainPos) + 1;
      long long domainId = std::stoll(body.substr(start));

      // Parse address
      size_t addrPos = body.find("\"address\"");
      if (addrPos == std::string::npos)
      {
         res.status = 400;
         res.set_content(JsonError("Missing address"), "application/json");
         return;
      }
      start = body.find("\"", addrPos + 9) + 1;
      size_t end = body.find("\"", start);
      std::string address = body.substr(start, end - start);

      // Parse password
      size_t passPos = body.find("\"password\"");
      if (passPos == std::string::npos)
      {
         res.status = 400;
         res.set_content(JsonError("Missing password"), "application/json");
         return;
      }
      start = body.find("\"", passPos + 10) + 1;
      end = body.find("\"", start);
      std::string password = body.substr(start, end - start);

      auto account = std::make_shared<MA::Account>();
      account->SetDomainID(domainId);
      account->SetAddress(MA::String(address.c_str()));
      account->SetPassword(MA::Crypt::Instance()->EnCrypt(MA::String(password.c_str()), MA::Crypt::ETSHA256));
      account->SetActive(true);

      MA::String errorMsg;
      if (!MA::PersistentAccount::SaveObject(account, errorMsg, MA::PersistenceModeNormal))
      {
         res.status = 500;
         res.set_content(JsonError("Failed to create account"), "application/json");
         return;
      }

      std::ostringstream json;
      json << "{\"id\":" << account->GetID() << "}";
      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void AdminApi::UpdateAccount(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      long long id = std::stoll(req.matches[1]);

      auto account = std::make_shared<MA::Account>();
      if (!MA::PersistentAccount::ReadObject(account, id))
      {
         res.status = 404;
         res.set_content(JsonError("Account not found"), "application/json");
         return;
      }

      std::string body = req.body;

      // Parse active state
      size_t activePos = body.find("\"isActive\"");
      if (activePos != std::string::npos)
      {
         bool isActive = body.find("true", activePos) != std::string::npos;
         account->SetActive(isActive);
      }

      // Parse password (optional)
      size_t passPos = body.find("\"password\"");
      if (passPos != std::string::npos)
      {
         size_t start = body.find("\"", passPos + 10) + 1;
         size_t end = body.find("\"", start);
         std::string password = body.substr(start, end - start);
         if (!password.empty())
         {
            account->SetPassword(MA::Crypt::Instance()->EnCrypt(MA::String(password.c_str()), MA::Crypt::ETSHA256));
         }
      }

      MA::String errorMsg;
      if (!MA::PersistentAccount::SaveObject(account, errorMsg, MA::PersistenceModeNormal))
      {
         res.status = 500;
         res.set_content(JsonError("Failed to update account"), "application/json");
         return;
      }

      res.set_content(JsonSuccess("{}"), "application/json");
   }

   void AdminApi::DeleteAccount(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      long long id = std::stoll(req.matches[1]);

      auto account = std::make_shared<MA::Account>();
      if (!MA::PersistentAccount::ReadObject(account, id))
      {
         res.status = 404;
         res.set_content(JsonError("Account not found"), "application/json");
         return;
      }

      if (!MA::PersistentAccount::DeleteObject(account))
      {
         res.status = 500;
         res.set_content(JsonError("Failed to delete account"), "application/json");
         return;
      }

      res.set_content(JsonSuccess("{}"), "application/json");
   }

   void AdminApi::GetDashboard(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      auto domains = std::make_shared<MA::Domains>();
      domains->Refresh();

      int domainCount = domains->GetCount();
      int accountCount = 0;

      for (int i = 0; i < domainCount; i++)
      {
         auto domain = domains->GetItem(i);
         auto accounts = domain->GetAccounts();
         accounts->Refresh();
         accountCount += accounts->GetCount();
      }

      std::ostringstream json;
      json << "{";
      json << "\"domainCount\":" << domainCount << ",";
      json << "\"accountCount\":" << accountCount << ",";
      json << "\"serverStatus\":\"running\"";
      json << "}";

      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void AdminApi::GetSettings(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      auto config = MA::Configuration::Instance();
      auto smtpConfig = config->GetSMTPConfiguration();

      std::ostringstream json;
      json << "{";

      // SMTP settings
      json << "\"smtp\":{";
      json << "\"enabled\":" << (config->GetUseSMTP() ? "true" : "false") << ",";
      json << "\"maxMessageSize\":" << smtpConfig->GetMaxMessageSize() << ",";
      json << "\"maxRecipients\":" << smtpConfig->GetMaxSMTPRecipientsInBatch();
      json << "},";

      // Logging settings
      json << "\"logging\":{";
      json << "\"enabled\":" << (config->GetUseLogging() ? "true" : "false") << ",";
      json << "\"logSMTP\":" << (config->GetLogSMTPConversations() ? "true" : "false") << ",";
      json << "\"logPOP3\":" << (config->GetLogPOP3Conversations() ? "true" : "false") << ",";
      json << "\"logIMAP\":" << (config->GetLogIMAPConversations() ? "true" : "false") << ",";
      json << "\"logDebug\":" << (config->GetLogDebug() ? "true" : "false");
      json << "}";

      json << "}";
      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void AdminApi::UpdateSmtpSettings(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      auto config = MA::Configuration::Instance();
      auto smtpConfig = config->GetSMTPConfiguration();
      std::string body = req.body;

      // Parse enabled
      size_t pos = body.find("\"enabled\"");
      if (pos != std::string::npos)
      {
         bool enabled = body.find("true", pos) != std::string::npos;
         config->SetUseSMTP(enabled);
      }

      // Parse maxMessageSize
      pos = body.find("\"maxMessageSize\"");
      if (pos != std::string::npos)
      {
         size_t start = body.find(":", pos) + 1;
         int size = std::stoi(body.substr(start));
         smtpConfig->SetMaxMessageSize(size);
      }

      // Parse maxRecipients
      pos = body.find("\"maxRecipients\"");
      if (pos != std::string::npos)
      {
         size_t start = body.find(":", pos) + 1;
         int count = std::stoi(body.substr(start));
         smtpConfig->SetMaxSMTPRecipientsInBatch(count);
      }

      res.set_content(JsonSuccess("{}"), "application/json");
   }

   void AdminApi::UpdateLoggingSettings(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      auto config = MA::Configuration::Instance();
      std::string body = req.body;

      // Parse enabled
      size_t pos = body.find("\"enabled\"");
      if (pos != std::string::npos)
      {
         bool enabled = body.find("true", pos) != std::string::npos;
         config->SetUseLogging(enabled);
      }

      // Parse logSMTP
      pos = body.find("\"logSMTP\"");
      if (pos != std::string::npos)
      {
         bool val = body.find("true", pos) != std::string::npos;
         config->SetLogSMTPConversations(val);
      }

      // Parse logPOP3
      pos = body.find("\"logPOP3\"");
      if (pos != std::string::npos)
      {
         bool val = body.find("true", pos) != std::string::npos;
         config->SetLogPOP3Conversations(val);
      }

      // Parse logIMAP
      pos = body.find("\"logIMAP\"");
      if (pos != std::string::npos)
      {
         bool val = body.find("true", pos) != std::string::npos;
         config->SetLogIMAPConversations(val);
      }

      // Parse logDebug
      pos = body.find("\"logDebug\"");
      if (pos != std::string::npos)
      {
         bool val = body.find("true", pos) != std::string::npos;
         config->SetLogDebug(val);
      }

      res.set_content(JsonSuccess("{}"), "application/json");
   }
}
