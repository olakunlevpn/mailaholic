#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <string>
#include <map>

namespace WebAdmin
{
   class LetsEncryptApi
   {
   public:
      static void RegisterRoutes(httplib::SSLServer& server);

      // ACME challenge token storage
      static void SetChallengeToken(const std::string& token, const std::string& authorization);
      static std::string GetChallengeToken(const std::string& token);
      static void ClearChallengeToken(const std::string& token);

   private:
      static void GetStatus(const httplib::Request& req, httplib::Response& res);
      static void RequestCertificate(const httplib::Request& req, httplib::Response& res);
      static void ServeChallenge(const httplib::Request& req, httplib::Response& res);

      static std::string JsonError(const std::string& message);
      static std::string JsonSuccess(const std::string& data = "{}");

      static std::map<std::string, std::string> challengeTokens_;
      static std::mutex tokenMutex_;
   };
}
