#pragma once

#include "../Util/Event.h"

#include <boost/thread/thread.hpp>

namespace MA
{
   class Reinitializator : public Singleton<Reinitializator>
   {
   public:
      Reinitializator(void);
      ~Reinitializator(void);

     
      void ReInitialize();

   private:

      void WorkerFunc();

      boost::thread worker_thread_;

      bool is_running_;
   };
}