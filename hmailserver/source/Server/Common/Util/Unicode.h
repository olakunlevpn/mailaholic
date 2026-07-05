// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Unicode  
   {
   public:
	   Unicode();
	   virtual ~Unicode();

      static AnsiString ToANSI(const String &sString);
      static bool WideToMultiByte(const String &sInput, AnsiString &sOutput);
      static bool MultiByteToWide(const AnsiString &sInput, String &sOutput);

      static unsigned char* CharMoveNext(unsigned char*, bool utf8);
   private:

   };


}