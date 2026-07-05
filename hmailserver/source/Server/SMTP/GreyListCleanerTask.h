// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Common/BO/ScheduledTask.h"

namespace MA
{
   class GreyListCleanerTask : public ScheduledTask
   {
   public:
      GreyListCleanerTask(void);
      ~GreyListCleanerTask(void);

      virtual void DoWork();
   private:
   };
}