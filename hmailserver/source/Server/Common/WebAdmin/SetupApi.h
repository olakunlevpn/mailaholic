#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

namespace WebAdmin
{
   class SetupApi
   {
   public:
      static void RegisterRoutes(httplib::SSLServer& server);

   private:
      static void GetState(const httplib::Request& req, httplib::Response& res);
      static void SetDomain(const httplib::Request& req, httplib::Response& res);
      static void SetUsage(const httplib::Request& req, httplib::Response& res);
      static void SetAdmin(const httplib::Request& req, httplib::Response& res);
      static void GetAutoConfig(const httplib::Request& req, httplib::Response& res);
      static void GetDnsRecords(const httplib::Request& req, httplib::Response& res);
      static void Complete(const httplib::Request& req, httplib::Response& res);

      static std::string JsonError(const std::string& message);
      static std::string JsonSuccess(const std::string& data = "{}");
   };
}
