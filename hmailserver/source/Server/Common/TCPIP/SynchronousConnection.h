// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

using boost::asio::ip::tcp;

namespace MA
{
   class SynchronousConnection
   {
   public:
	   SynchronousConnection(int timeoutSeconds);
	   virtual ~SynchronousConnection();

      bool Connect(const AnsiString &hostName, int port);
      bool Write(const AnsiString &data);
      bool Write(const ByteBuffer &buffer);
      bool ReadUntil(const AnsiString &delimiter, AnsiString &readData);
      void Close();

   private:
      
      bool Write_(const unsigned char *buf, size_t bufSize);

      boost::asio::io_context io_context_;
      tcp::socket socket_;
      int seconds_;
   };


}
