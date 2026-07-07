#include "stdafx.h"
#include "DKIMApi.h"
#include "AuthApi.h"
#include "../BO/Domain.h"
#include "../BO/Domains.h"
#include "../Persistence/PersistentDomain.h"
#include "../Util/Unicode.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <sstream>

namespace WebAdmin
{
   std::string DKIMApi::JsonError(const std::string& message)
   {
      return "{\"error\":\"" + message + "\"}";
   }

   std::string DKIMApi::JsonSuccess(const std::string& data)
   {
      return "{\"success\":true,\"data\":" + data + "}";
   }

   void DKIMApi::RegisterRoutes(httplib::SSLServer& server)
   {
      server.Get("/api/dkim/status", GetStatus);
      server.Post("/api/dkim/generate", GenerateKeys);
   }

   bool DKIMApi::GenerateRSAKeyPair(std::string& privateKey, std::string& publicKey)
   {
      EVP_PKEY* pkey = EVP_RSA_gen(2048);
      if (!pkey)
         return false;

      // Extract private key
      BIO* privBio = BIO_new(BIO_s_mem());
      PEM_write_bio_PrivateKey(privBio, pkey, nullptr, nullptr, 0, nullptr, nullptr);

      char* privData;
      long privLen = BIO_get_mem_data(privBio, &privData);
      privateKey.assign(privData, privLen);
      BIO_free(privBio);

      // Extract public key in DER format for DNS
      BIO* pubBio = BIO_new(BIO_s_mem());
      PEM_write_bio_PUBKEY(pubBio, pkey);

      char* pubData;
      long pubLen = BIO_get_mem_data(pubBio, &pubData);
      std::string pubPEM(pubData, pubLen);
      BIO_free(pubBio);

      // Strip PEM headers for DNS record
      size_t start = pubPEM.find('\n') + 1;
      size_t end = pubPEM.rfind("\n-----");
      if (start != std::string::npos && end != std::string::npos && end > start)
      {
         publicKey = pubPEM.substr(start, end - start);
         // Remove newlines
         publicKey.erase(std::remove(publicKey.begin(), publicKey.end(), '\n'), publicKey.end());
      }
      else
      {
         publicKey = pubPEM;
      }

      EVP_PKEY_free(pkey);
      return true;
   }

   void DKIMApi::GetStatus(const httplib::Request& req, httplib::Response& res)
   {
      if (!AuthApi::RequireAuth(req, res)) return;

      // Check domain_id query param
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

      MA::String selector = domain->GetDKIMSelector();
      MA::String privateKeyFile = domain->GetDKIMPrivateKeyFile();

      std::ostringstream json;
      json << "{";
      json << "\"hasKeys\":" << (!selector.IsEmpty() ? "true" : "false") << ",";
      json << "\"selector\":\"" << MA::Unicode::ToANSI(selector).c_str() << "\"";
      json << "}";

      res.set_content(JsonSuccess(json.str()), "application/json");
   }

   void DKIMApi::GenerateKeys(const httplib::Request& req, httplib::Response& res)
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

      // Parse selector (optional, default "mail")
      std::string selector = "mail";
      size_t selPos = body.find("\"selector\"");
      if (selPos != std::string::npos)
      {
         start = body.find("\"", selPos + 10) + 1;
         size_t end = body.find("\"", start);
         selector = body.substr(start, end - start);
      }

      auto domain = std::make_shared<MA::Domain>();
      if (!MA::PersistentDomain::ReadObject(domain, domainId))
      {
         res.status = 404;
         res.set_content(JsonError("Domain not found"), "application/json");
         return;
      }

      // Generate key pair
      std::string privateKey, publicKey;
      if (!GenerateRSAKeyPair(privateKey, publicKey))
      {
         res.status = 500;
         res.set_content(JsonError("Failed to generate keys"), "application/json");
         return;
      }

      // Store private key and selector
      domain->SetDKIMSelector(MA::String(selector.c_str()));
      domain->SetDKIMPrivateKeyFile(MA::String(privateKey.c_str()));

      if (!MA::PersistentDomain::SaveObject(domain))
      {
         res.status = 500;
         res.set_content(JsonError("Failed to save domain"), "application/json");
         return;
      }

      // Return DNS record
      std::string domainName = MA::Unicode::ToANSI(domain->GetName());
      std::ostringstream json;
      json << "{";
      json << "\"selector\":\"" << selector << "\",";
      json << "\"dnsRecord\":\"" << selector << "._domainkey." << domainName << "\",";
      json << "\"dnsType\":\"TXT\",";
      json << "\"dnsValue\":\"v=DKIM1; k=rsa; p=" << publicKey << "\"";
      json << "}";

      res.set_content(JsonSuccess(json.str()), "application/json");
   }
}
