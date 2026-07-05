// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once


namespace MA
{
   class PasswordGenerator
   {
   public:
	   PasswordGenerator();
	   virtual ~PasswordGenerator();

      static String Generate();
      static bool IsStrongPassword(const String &sUsername, const String &sPassword);

   private:
      
      static int Seed;

   };

}

