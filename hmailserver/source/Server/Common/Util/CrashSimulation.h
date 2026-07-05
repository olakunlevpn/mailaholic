// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class CrashSimulation
   {
   public:

      static void Execute(int simulation_mode);

   private:
      CrashSimulation(void);

    
   };
}