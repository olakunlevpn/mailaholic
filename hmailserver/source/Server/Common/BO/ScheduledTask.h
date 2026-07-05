// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Util/VariantDateTime.h"
#include "../Threading/Task.h"

namespace MA
{
   class ScheduledTask : public Task
   {
   public:
      ScheduledTask();
      ~ScheduledTask(void);

      enum Reoccurance
      {
         RunOnce = 1,
         RunInfinitely =2
      };

      int GetMinutesBetweenRun() const;
      void SetMinutesBetweenRun(int iNewVal);

      Reoccurance GetReoccurance() const;
      void SetReoccurance(Reoccurance ro);

      DateTime GetNextRunTime() const;
      void SetNextRunTime();

   private:

      Reoccurance reoccurance_;
      DateTime next_run_time_;
      int minutes_between_run_;

   };
}