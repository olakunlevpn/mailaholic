// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class COMAuthenticator
   {
   public:

      COMAuthenticator()
      {
         // Create a dummy object so that it always exists.
         authentication_ = std::shared_ptr<MA::COMAuthentication>(new COMAuthentication);

         authentication_->AttempAnonymousAuthentication();
      }
      
      void SetAuthentication(std::shared_ptr<MA::COMAuthentication> pAuthentication)
      {
         authentication_ = pAuthentication; 
      }

   protected:

      bool GetIsServerAdmin()
      {
         if (!authentication_)
            return false;

         if (!authentication_->GetIsServerAdmin())
            return false;

         return true;
      }

      int GetAccessDenied()
      {
         if (!authentication_)
            return -1;

         return authentication_->GetAccessDenied();
      }

      std::shared_ptr<COMAuthentication> authentication_;

   private:

   };

}