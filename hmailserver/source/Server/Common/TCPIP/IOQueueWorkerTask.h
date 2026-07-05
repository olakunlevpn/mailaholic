// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once


#include "..\Threading\Task.h"

namespace MA
{
   class Socket;
   class SocketCompletionPort;

   class IOCPQueueWorkerTask : public Task
   {
   public:

      IOCPQueueWorkerTask(boost::asio::io_context &io_context);

      virtual void DoWork();
      void DoWorkInner();

   private:

      boost::asio::io_context &io_context_;
   };
}