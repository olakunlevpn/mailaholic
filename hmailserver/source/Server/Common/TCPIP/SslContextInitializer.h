// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class SSLCertificate;

   class SslContextInitializer
   {
   public:

      static bool InitServer(boost::asio::ssl::context& context, std::shared_ptr<SSLCertificate> certificate, String ip_address, int port);
      static bool InitClient(boost::asio::ssl::context& context);


   private:

      static void SetContextOptions_(boost::asio::ssl::context& context, bool serverContext);
 
      static std::string  GetPassword_();

      static void SetCipherList_(boost::asio::ssl::context& context);
      static void EnableEllipticCurveCrypto_(boost::asio::ssl::context& context);
      
   };
}