#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <string>

namespace WebAdmin
{
   class AuthApi
   {
   public:
      static void RegisterRoutes(httplib::SSLServer& server);
      static bool RequireAuth(const httplib::Request& req, httplib::Response& res);

   private:
      static void Login(const httplib::Request& req, httplib::Response& res);
      static void Logout(const httplib::Request& req, httplib::Response& res);
      static void Check(const httplib::Request& req, httplib::Response& res);

      static std::string JsonError(const std::string& message);
      static std::string JsonSuccess(const std::string& data = "{}");
   };
}
