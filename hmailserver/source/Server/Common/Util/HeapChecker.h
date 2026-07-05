// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include <crtdbg.h>

namespace MA
{
   class HeapChecker
   {
   public:
      HeapChecker(void);
      ~HeapChecker(void);

      static void CheckHeapOnAllocation();

      void Reset();
      void Report();

   private:
#ifdef _DEBUG
       _CrtMemState start_;
#endif
   };
}