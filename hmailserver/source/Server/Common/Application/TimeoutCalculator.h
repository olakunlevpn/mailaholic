// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class TimeoutCalculator  
   {
   public:
      TimeoutCalculator();

      enum Constants
      {
         MaxConnectionCountOptimized = 20000
      };

      int Calculate(int minSecs, int maxSecs);
      int Calculate(int connectionCount, int minSecs, int maxSecs);

   private:
   };

   class TimeoutCalculatorTester
   {
   public:
      void Test();


   };
}
