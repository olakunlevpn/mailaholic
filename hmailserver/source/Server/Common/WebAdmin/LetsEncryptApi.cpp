#include "stdafx.h"
#include "LetsEncryptApi.h"
#include "AuthApi.h"
#include <sstream>

namespace WebAdmin
{
   std::map<std::string, std::string> LetsEncryptApi::challengeTokens_;
   std::mutex LetsEncryptApi::tokenMutex_;

   std::string LetsEncryptApi::JsonError(const std::string& message)
   {
      return "{\"error\":\"" + message + "\"}";
   }

   std::string LetsEncryptApi::JsonSuccess(const std::string& data)
   {
      return "{\"success\":true,\"data\":" + data + "}";
   }

   void LetsEncryptApi::RegisterRoutes(httplib::SSLServer& server)
   {
      server.Get("/api/letsencrypt/status", GetStatus);
      server.Post("/api/letsencrypt/request", RequestCertificate);
      server.Get(R"(/\.well-known/acme-challenge/(.+))", ServeChallenge);
   }

   void LetsEncryptApi::SetChallengeToken(const std::string& token, const std::string& authorization)
   {
      std::lock_guard<std::mutex> lock(tokenMutex_);
      challengeTokens_[token] = authorization;
   }

   std::string LetsEncryptApi::GetChallengeToken(const std::string& token)
   {
      std::lock_guard<std::mutex> lock(tokenMutex_);
      auto it = challengeTokens_.find(token);
      if (it != challengeTokens_.end())
         return it->second;
      return "";
   }

   void LetsEncryptApi::ClearChallengeToken(const std::string& token)
   {
      std::lock_guard<std::mutex> lock(tokenMutex_);
      challengeTokens_.erase(token);
   }

   void LetsEncryptApi::GetStatus(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      std::ostringstream json;
      json << "{";
      json << "\"hasCertificate\":true,";
      json << "\"isLetsEncrypt\":false,";
      json << "\"instructions\":\"To use Let's Encrypt certificates, run certbot externally and configure the certificate paths in mailaholic.ini\"";
      json << "}";

      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void LetsEncryptApi::RequestCertificate(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      // Full ACME implementation requires:
      // 1. Register account with ACME directory
      // 2. Request certificate order
      // 3. Complete HTTP-01 challenge
      // 4. Fetch and install certificate
      //
      // For now, provide instructions for external certbot usage

      std::ostringstream json;
      json << "{";
      json << "\"manual\":true,";
      json << "\"command\":\"certbot certonly --webroot -w /path/to/mailaholic/data -d YOUR_DOMAIN\",";
      json << "\"certPath\":\"/etc/letsencrypt/live/YOUR_DOMAIN/fullchain.pem\",";
      json << "\"keyPath\":\"/etc/letsencrypt/live/YOUR_DOMAIN/privkey.pem\"";
      json << "}";

      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void LetsEncryptApi::ServeChallenge(const httplib::Request& req, httplib::Response& res)
   {
      // ACME HTTP-01 challenge endpoint
      // When certbot places a challenge file, we serve it here
      std::string token = req.matches[1];
      std::string authorization = GetChallengeToken(token);

      if (authorization.empty())
      {
         res.status = 404;
         res.set_content("Not found", "text/plain");
         return;
      }

      res.set_content(authorization, "text/plain");
   }
}
