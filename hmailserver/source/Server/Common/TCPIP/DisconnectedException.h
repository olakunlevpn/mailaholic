// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class DisconnectedException : public std::exception
   {
   public:

      virtual const char* what() const
      {
         return "The client has been disconnected.";
      }


   private:

   };
}