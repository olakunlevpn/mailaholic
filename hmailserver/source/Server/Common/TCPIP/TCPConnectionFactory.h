// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "TCPConnection.h"


namespace MA
{
   class TCPConnectionFactory
   {
   public:
      virtual std::shared_ptr<TCPConnection> Create(ConnectionSecurity connection_security,
         boost::asio::io_context& io_context, 
         boost::asio::ssl::context& context) = 0;
   };

   class SMTPConnectionFactory : public TCPConnectionFactory
   {
   public:
      virtual std::shared_ptr<TCPConnection> Create(ConnectionSecurity connection_security,
         boost::asio::io_context& io_context, 
         boost::asio::ssl::context& context);
   };

   class POP3ConnectionFactory : public TCPConnectionFactory
   {
   public:
      virtual std::shared_ptr<TCPConnection> Create(ConnectionSecurity connection_security,
         boost::asio::io_context& io_context, 
         boost::asio::ssl::context& context);
   };

   class IMAPConnectionFactory : public TCPConnectionFactory
   {
   public:
      virtual std::shared_ptr<TCPConnection> Create(ConnectionSecurity connection_security,
         boost::asio::io_context& io_context, 
         boost::asio::ssl::context& context);
   };

}