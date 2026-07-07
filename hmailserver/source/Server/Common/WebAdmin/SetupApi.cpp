#include "stdafx.h"
#include "SetupApi.h"
#include "SetupState.h"
#include "../Application/IniFileSettings.h"
#include "../BO/Domain.h"
#include "../BO/Account.h"
#include "../Persistence/PersistentDomain.h"
#include "../Persistence/PersistentAccount.h"
#include "../Persistence/PersistenceMode.h"
#include "../Util/Crypt.h"
#include "../Util/Unicode.h"
#include <sstream>

namespace WebAdmin
{
   std::string SetupApi::JsonError(const std::string& message)
   {
      return "{\"error\":\"" + message + "\"}";
   }

   std::string SetupApi::JsonSuccess(const std::string& data)
   {
      return "{\"success\":true,\"data\":" + data + "}";
   }

   void SetupApi::RegisterRoutes(httplib::SSLServer& server)
   {
      server.Get("/api/setup/state", GetState);
      server.Post("/api/setup/domain", SetDomain);
      server.Post("/api/setup/usage", SetUsage);
      server.Post("/api/setup/admin", SetAdmin);
      server.Get("/api/setup/autoconfig", GetAutoConfig);
      server.Get("/api/setup/dns", GetDnsRecords);
      server.Post("/api/setup/complete", Complete);
   }

   void SetupApi::GetState(const httplib::Request& req, httplib::Response& res)
   {
      auto& state = SetupState::Instance();
      std::ostringstream json;
      json << "{";
      json << "\"step\":" << state.GetStep() << ",";
      json << "\"domain\":\"" << state.GetDomain() << "\",";
      json << "\"usageType\":" << static_cast<int>(state.GetUsageType()) << ",";
      json << "\"adminEmail\":\"" << state.GetAdminEmail() << "\",";
      json << "\"isComplete\":" << (state.IsComplete() ? "true" : "false");
      json << "}";
      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void SetupApi::SetDomain(const httplib::Request& req, httplib::Response& res)
   {
      std::string body = req.body;
      size_t pos = body.find("\"domain\"");
      if (pos == std::string::npos)
      {
         res.status = 400;
         res.set_content(JsonError("Missing domain"), "application/json");
         return;
      }

      size_t start = body.find("\"", pos + 8) + 1;
      size_t end = body.find("\"", start);
      std::string domain = body.substr(start, end - start);

      if (domain.empty() || domain == "localhost")
      {
         res.status = 400;
         res.set_content(JsonError("Invalid domain"), "application/json");
         return;
      }

      SetupState::Instance().SetDomain(domain);
      SetupState::Instance().SetStep(2);
      res.set_content(JsonSuccess("{}"), "application/json");
   }

   void SetupApi::SetUsage(const httplib::Request& req, httplib::Response& res)
   {
      std::string body = req.body;
      size_t pos = body.find("\"usageType\"");
      if (pos == std::string::npos)
      {
         res.status = 400;
         res.set_content(JsonError("Missing usageType"), "application/json");
         return;
      }

      size_t start = body.find(":", pos) + 1;
      int type = std::stoi(body.substr(start, 1));

      SetupState::Instance().SetUsageType(static_cast<UsageType>(type));
      SetupState::Instance().SetStep(3);
      res.set_content(JsonSuccess("{}"), "application/json");
   }

   void SetupApi::SetAdmin(const httplib::Request& req, httplib::Response& res)
   {
      std::string body = req.body;

      size_t emailPos = body.find("\"email\"");
      if (emailPos == std::string::npos)
      {
         res.status = 400;
         res.set_content(JsonError("Missing email"), "application/json");
         return;
      }
      size_t start = body.find("\"", emailPos + 7) + 1;
      size_t end = body.find("\"", start);
      std::string email = body.substr(start, end - start);

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

      if (email.find("@") == std::string::npos || password.length() < 8)
      {
         res.status = 400;
         res.set_content(JsonError("Invalid email or password too short"), "application/json");
         return;
      }

      SetupState::Instance().SetAdminEmail(email);
      SetupState::Instance().SetAdminPassword(password);
      SetupState::Instance().SetStep(4);
      res.set_content(JsonSuccess("{}"), "application/json");
   }

   void SetupApi::GetAutoConfig(const httplib::Request& req, httplib::Response& res)
   {
      std::ostringstream json;
      json << "{";

      // Get local hostname for display
      char hostname[256];
      gethostname(hostname, sizeof(hostname));
      json << "\"localIp\":\"" << hostname << "\",";

      json << "\"port25\":true,";
      json << "\"port587\":true,";
      json << "\"port993\":true,";
      json << "\"port995\":true,";
      json << "\"certGenerated\":true,";
      json << "\"dkimReady\":false";
      json << "}";

      SetupState::Instance().SetStep(5);
      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void SetupApi::GetDnsRecords(const httplib::Request& req, httplib::Response& res)
   {
      auto& state = SetupState::Instance();
      std::string domain = state.GetDomain();

      std::string baseDomain = domain;
      if (domain.length() > 5 && domain.substr(0, 5) == "mail.")
         baseDomain = domain.substr(5);

      // Use localhost placeholder - real IP would need network detection
      std::string ip = "YOUR_SERVER_IP";

      std::ostringstream json;
      json << "{\"records\":[";
      json << "{\"type\":\"MX\",\"name\":\"" << baseDomain << "\",\"value\":\"" << domain << "\",\"priority\":10},";
      json << "{\"type\":\"A\",\"name\":\"" << domain << "\",\"value\":\"" << ip << "\"},";
      json << "{\"type\":\"TXT\",\"name\":\"" << baseDomain << "\",\"value\":\"v=spf1 ip4:" << ip << " -all\"},";
      json << "{\"type\":\"TXT\",\"name\":\"mail._domainkey." << baseDomain << "\",\"value\":\"v=DKIM1; k=rsa; p=...\"},";
      json << "{\"type\":\"TXT\",\"name\":\"_dmarc." << baseDomain << "\",\"value\":\"v=DMARC1; p=quarantine\"}";
      json << "]}";

      SetupState::Instance().SetStep(6);
      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void SetupApi::Complete(const httplib::Request& req, httplib::Response& res)
   {
      auto& state = SetupState::Instance();

      // Extract domain from admin email
      std::string adminEmail = state.GetAdminEmail();
      std::string adminPassword = state.GetAdminPassword();

      size_t atPos = adminEmail.find("@");
      if (atPos == std::string::npos)
      {
         res.status = 400;
         res.set_content(JsonError("Invalid admin email"), "application/json");
         return;
      }

      std::string domainName = adminEmail.substr(atPos + 1);

      // Create domain
      auto domain = std::make_shared<MA::Domain>();
      domain->SetName(MA::String(domainName.c_str()));
      domain->SetIsActive(true);

      if (!MA::PersistentDomain::SaveObject(domain))
      {
         res.status = 500;
         res.set_content(JsonError("Failed to create domain"), "application/json");
         return;
      }

      // Create admin account
      auto account = std::make_shared<MA::Account>();
      account->SetDomainID(domain->GetID());
      account->SetAddress(MA::String(adminEmail.c_str()));
      account->SetPassword(MA::Crypt::Instance()->EnCrypt(MA::String(adminPassword.c_str()), MA::Crypt::ETSHA256));
      account->SetActive(true);
      account->SetAdminLevel(MA::Account::ServerAdmin);

      MA::String errorMsg;
      if (!MA::PersistentAccount::SaveObject(account, errorMsg, MA::PersistenceModeNormal))
      {
         res.status = 500;
         res.set_content(JsonError("Failed to create admin account"), "application/json");
         return;
      }

      state.MarkComplete();
      res.set_content(JsonSuccess("{}"), "application/json");
   }
}
