#include "stdafx.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "WebServer.h"
#include "AssetHandler.h"
#include "SetupApi.h"
#include "SetupState.h"
#include "AuthApi.h"
#include "AdminApi.h"
#include "LetsEncryptApi.h"
#include "DKIMApi.h"
#include "../Util/FileUtilities.h"
#include "../Util/Unicode.h"
#include "../Application/IniFileSettings.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <random>

namespace WebAdmin
{
   WebServer::WebServer()
   {
   }

   WebServer::~WebServer()
   {
      Stop();
   }

   int WebServer::FindAvailablePort(int preferred)
   {
      if (preferred >= 49152 && preferred <= 65535)
      {
         httplib::Server testServer;
         if (testServer.bind_to_port("127.0.0.1", preferred))
         {
            return preferred;
         }
      }

      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dis(49152, 65535);

      for (int attempts = 0; attempts < 100; ++attempts)
      {
         int port = dis(gen);
         httplib::Server testServer;
         if (testServer.bind_to_port("127.0.0.1", port))
         {
            return port;
         }
      }
      return 0;
   }

   bool WebServer::GenerateSelfSignedCert()
   {
      MA::String dataDir = MA::IniFileSettings::Instance()->GetDataDirectory();
      MA::String certPathW = dataDir + _T("\\webadmin.crt");
      MA::String keyPathW = dataDir + _T("\\webadmin.key");
      certPath_ = MA::Unicode::ToANSI(certPathW).c_str();
      keyPath_ = MA::Unicode::ToANSI(keyPathW).c_str();

      if (MA::FileUtilities::Exists(certPathW) &&
          MA::FileUtilities::Exists(keyPathW))
      {
         return true;
      }

      EVP_PKEY* pkey = EVP_RSA_gen(2048);
      if (!pkey)
         return false;

      X509* x509 = X509_new();
      if (!x509)
      {
         EVP_PKEY_free(pkey);
         return false;
      }

      ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
      X509_gmtime_adj(X509_get_notBefore(x509), 0);
      X509_gmtime_adj(X509_get_notAfter(x509), 365 * 24 * 60 * 60);
      X509_set_pubkey(x509, pkey);

      X509_NAME* name = X509_get_subject_name(x509);
      X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
         (unsigned char*)"Mailaholic WebAdmin", -1, -1, 0);
      X509_set_issuer_name(x509, name);
      X509_sign(x509, pkey, EVP_sha256());

      FILE* certFile = nullptr;
      if (fopen_s(&certFile, certPath_.c_str(), "wb") == 0 && certFile)
      {
         PEM_write_X509(certFile, x509);
         fclose(certFile);
      }

      FILE* keyFile = nullptr;
      if (fopen_s(&keyFile, keyPath_.c_str(), "wb") == 0 && keyFile)
      {
         PEM_write_PrivateKey(keyFile, pkey, nullptr, nullptr, 0, nullptr, nullptr);
         fclose(keyFile);
      }

      X509_free(x509);
      EVP_PKEY_free(pkey);
      return true;
   }

   bool WebServer::Start(int preferredPort)
   {
      if (running_)
         return true;

      if (!GenerateSelfSignedCert())
         return false;

      port_ = FindAvailablePort(preferredPort);
      if (port_ == 0)
         return false;

      server_ = std::make_unique<httplib::SSLServer>(certPath_.c_str(), keyPath_.c_str());

      if (!server_->is_valid())
         return false;

      // Register setup API routes if setup not complete
      if (!SetupState::Instance().IsComplete())
      {
         SetupApi::RegisterRoutes(*server_);
      }

      // Register auth API routes (always available)
      AuthApi::RegisterRoutes(*server_);

      // Register admin API routes (always available, auth checked per endpoint)
      AdminApi::RegisterRoutes(*server_);

      // Register Let's Encrypt API routes
      LetsEncryptApi::RegisterRoutes(*server_);

      // Register DKIM API routes
      DKIMApi::RegisterRoutes(*server_);

      // Serve static assets
      server_->Get(".*", [](const httplib::Request& req, httplib::Response& res) {
         std::string content, mimeType;
         bool isCompressed;

         if (AssetHandler::ServeAsset(req.path, content, mimeType, isCompressed))
         {
            res.set_content(content, mimeType.c_str());
         }
         else
         {
            res.status = 404;
            res.set_content("Not Found", "text/plain");
         }
      });

      running_ = true;
      serverThread_ = std::thread([this]() {
         server_->listen("127.0.0.1", port_);
      });

      return true;
   }

   void WebServer::Stop()
   {
      if (!running_)
         return;

      running_ = false;
      if (server_)
         server_->stop();

      if (serverThread_.joinable())
         serverThread_.join();

      server_.reset();
   }

   int WebServer::GetPort() const
   {
      return port_;
   }

   bool WebServer::IsRunning() const
   {
      return running_;
   }

   void WebServer::Get(const std::string& pattern, RouteHandler handler)
   {
      server_->Get(pattern, [handler](const httplib::Request& req, httplib::Response& res) {
         std::string response;
         int statusCode = 200;
         handler(req.body, response, statusCode);
         res.status = statusCode;
         res.set_content(response, "application/json");
      });
   }

   void WebServer::Post(const std::string& pattern, RouteHandler handler)
   {
      server_->Post(pattern, [handler](const httplib::Request& req, httplib::Response& res) {
         std::string response;
         int statusCode = 200;
         handler(req.body, response, statusCode);
         res.status = statusCode;
         res.set_content(response, "application/json");
      });
   }

   void WebServer::Put(const std::string& pattern, RouteHandler handler)
   {
      server_->Put(pattern, [handler](const httplib::Request& req, httplib::Response& res) {
         std::string response;
         int statusCode = 200;
         handler(req.body, response, statusCode);
         res.status = statusCode;
         res.set_content(response, "application/json");
      });
   }

   void WebServer::Delete(const std::string& pattern, RouteHandler handler)
   {
      server_->Delete(pattern, [handler](const httplib::Request& req, httplib::Response& res) {
         std::string response;
         int statusCode = 200;
         handler(req.body, response, statusCode);
         res.status = statusCode;
         res.set_content(response, "application/json");
      });
   }
}
