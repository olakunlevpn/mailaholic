// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic
// Created 2005-07-21

#pragma once

#include "IOOperation.h"

namespace MA
{

   class IOOperationQueue
   {
   public:
      IOOperationQueue();
      ~IOOperationQueue(void);

      void Push(std::shared_ptr<IOOperation> operation);
      std::shared_ptr<IOOperation> Front();
      void Pop(IOOperation::OperationType type);

      bool ContainsQueuedSendOperation();

   private:

      boost::recursive_mutex mutex_;

      std::deque<std::shared_ptr<IOOperation> > queue_operations_;
      
      std::vector<std::shared_ptr<IOOperation > > ongoing_operations_;
   };

}