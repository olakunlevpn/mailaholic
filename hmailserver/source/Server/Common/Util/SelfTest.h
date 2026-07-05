// Copyright (c) 2026 Mailaholic.
#pragma once

namespace MA
{
   // Runs only environment-independent unit testers (no DB, DNS, registry, or
   // service startup). Safe to run on a bare CI runner. Returns 0 if every
   // tester passed, 1 if any tester threw.
   class SelfTest
   {
   public:
      static int RunAll();
   };
}
