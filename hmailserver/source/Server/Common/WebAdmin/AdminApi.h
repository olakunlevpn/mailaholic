#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <string>

namespace WebAdmin
{
   class AdminApi
   {
   public:
      static void RegisterRoutes(httplib::SSLServer& server);

   private:
      // Domains
      static void ListDomains(const httplib::Request& req, httplib::Response& res);
      static void CreateDomain(const httplib::Request& req, httplib::Response& res);
      static void UpdateDomain(const httplib::Request& req, httplib::Response& res);
      static void DeleteDomain(const httplib::Request& req, httplib::Response& res);

      // Accounts
      static void ListAccounts(const httplib::Request& req, httplib::Response& res);
      static void CreateAccount(const httplib::Request& req, httplib::Response& res);
      static void UpdateAccount(const httplib::Request& req, httplib::Response& res);
      static void DeleteAccount(const httplib::Request& req, httplib::Response& res);

      // Dashboard
      static void GetDashboard(const httplib::Request& req, httplib::Response& res);

      static std::string JsonError(const std::string& message);
      static std::string JsonSuccess(const std::string& data = "{}");
   };
}
