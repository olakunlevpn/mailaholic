#include "stdafx.h"
#include "AuthApi.h"
#include "WebSessionManager.h"
#include "../BO/Account.h"
#include "../Persistence/PersistentAccount.h"
#include "../Util/Crypt.h"
#include <sstream>

namespace WebAdmin
{
   std::string AuthApi::JsonError(const std::string& message)
   {
      return "{\"error\":\"" + message + "\"}";
   }

   std::string AuthApi::JsonSuccess(const std::string& data)
   {
      return "{\"success\":true,\"data\":" + data + "}";
   }

   void AuthApi::RegisterRoutes(httplib::SSLServer& server)
   {
      server.Post("/api/auth/login", Login);
      server.Post("/api/auth/logout", Logout);
      server.Get("/api/auth/check", Check);
   }

   bool AuthApi::RequireAuth(const httplib::Request& req, httplib::Response& res)
   {
      auto it = req.headers.find("Authorization");
      if (it == req.headers.end())
      {
         res.status = 401;
         res.set_content(JsonError("No authorization header"), "application/json");
         return false;
      }

      std::string token = it->second;
      if (token.length() > 7 && token.substr(0, 7) == "Bearer ")
         token = token.substr(7);

      int userId = WebSessionManager::Instance().ValidateSession(token);
      if (userId < 0)
      {
         res.status = 401;
         res.set_content(JsonError("Invalid or expired session"), "application/json");
         return false;
      }

      return true;
   }

   void AuthApi::Login(const httplib::Request& req, httplib::Response& res)
   {
      std::string body = req.body;

      // Parse email
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

      // Look up account
      MA::String maEmail(email.c_str());
      auto account = std::make_shared<MA::Account>();

      if (!MA::PersistentAccount::ReadObject(account, maEmail))
      {
         res.status = 401;
         res.set_content(JsonError("Invalid credentials"), "application/json");
         return;
      }

      // Check admin level (NormalUser = 0 means no admin rights)
      if (account->GetAdminLevel() == MA::Account::NormalUser)
      {
         res.status = 401;
         res.set_content(JsonError("Not authorized"), "application/json");
         return;
      }

      // Validate password
      MA::String maPassword(password.c_str());
      MA::Crypt::EncryptionType encType = static_cast<MA::Crypt::EncryptionType>(account->GetPasswordEncryption());

      if (!MA::Crypt::Instance()->Validate(maPassword, account->GetPassword(), encType))
      {
         res.status = 401;
         res.set_content(JsonError("Invalid credentials"), "application/json");
         return;
      }

      // Create session using account ID
      std::string token = WebSessionManager::Instance().CreateSession(static_cast<int>(account->GetID()));

      std::ostringstream json;
      json << "{\"token\":\"" << token << "\"}";
      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void AuthApi::Logout(const httplib::Request& req, httplib::Response& res)
   {
      auto it = req.headers.find("Authorization");
      if (it != req.headers.end())
      {
         std::string token = it->second;
         if (token.length() > 7 && token.substr(0, 7) == "Bearer ")
            token = token.substr(7);
         WebSessionManager::Instance().InvalidateSession(token);
      }
      res.set_content(JsonSuccess("{}"), "application/json");
   }

   void AuthApi::Check(const httplib::Request& req, httplib::Response& res)
   {
      if (RequireAuth(req, res))
      {
         res.set_content(JsonSuccess("{\"authenticated\":true}"), "application/json");
      }
   }
}
