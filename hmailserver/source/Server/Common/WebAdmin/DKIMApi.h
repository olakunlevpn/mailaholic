#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <string>

namespace WebAdmin
{
   class DKIMApi
   {
   public:
      static void RegisterRoutes(httplib::SSLServer& server);

   private:
      static void GetStatus(const httplib::Request& req, httplib::Response& res);
      static void GenerateKeys(const httplib::Request& req, httplib::Response& res);

      static std::string JsonError(const std::string& message);
      static std::string JsonSuccess(const std::string& data = "{}");

      static bool GenerateRSAKeyPair(std::string& privateKey, std::string& publicKey);
   };
}
