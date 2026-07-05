// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include <boost/thread.hpp>

namespace MA
{

   class MessageIndexer : public Singleton<MessageIndexer>
   {
   public:
      MessageIndexer();
      ~MessageIndexer(void);
      
      void Start();
      void Stop();

      void IndexNow();

   private:
   

      void WorkerFunc();
      void WorkerFuncInternal();

      void IndexMessages_();

      boost::thread workerThread_;
	   int iIndexRunCount;

      boost::recursive_mutex starterMutex_;

      Event index_now_;
   };
}