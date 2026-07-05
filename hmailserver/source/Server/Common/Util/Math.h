// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Math  
   {
   public:
	   Math();
	   virtual ~Math();

      static float Round(const float &number, const int num_digits);

   };
}
