// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once


namespace MA
{
   class PasswordRemover
   {
   public:

      enum PRType
      {
         PRIMAP = 1,
         PRPOP3 = 2,
         PRSMTP = 3
      };

	   PasswordRemover();
	   virtual ~PasswordRemover();

		static void Remove(PRType prt, String &sClientCommand);
   private:
      

   };
}

