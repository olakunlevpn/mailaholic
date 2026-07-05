// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Dictionary
   {
   public:
	   Dictionary();
      ~Dictionary();

      static String GetWindowsErrorDescription(int iErrorCode);
   
   };
}
