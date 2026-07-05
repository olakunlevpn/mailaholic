// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic
// Created 2005-07-21

#pragma once

#include "../Util/Event.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace MA
{
   class Task;
   class Thread;

   class WorkQueue
   {
   public:

      WorkQueue(unsigned int iMaxSimultaneous, const String &sName);
      ~WorkQueue(void);

      void SetMaxSimultaneous(int iMaxSimultaneous);

      void AddTask(std::shared_ptr<Task> pTask);
      void Start();

      void Stop();

      const String &GetName() const;

   private:

      void WorkQueue::RemoveRunningTask_(std::shared_ptr<Task> task);

      void IoServiceRunWorker();
      void ExecuteTask(std::shared_ptr<Task> pTask);

      boost::asio::io_context io_context_;
      boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;

      std::set<std::shared_ptr<Task>> runningTasks_;
      boost::recursive_mutex runningTasksMutex_;

      unsigned int max_simultaneous_;

      String queue_name_;

      std::set<std::shared_ptr<boost::thread>> workerThreads_;
   };

}