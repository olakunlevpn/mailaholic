// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"

#include ".\COMAuthentication.h"
#include "..\Common\BO\Account.h"
#include "..\Common\Util\PasswordValidator.h"
#include "..\Common\Util\Crypt.h"

#include "COMError.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   COMAuthentication::COMAuthentication(void)
   {
      
   }

   COMAuthentication::~COMAuthentication(void)
   {

   }

   std::shared_ptr<const Account>
   COMAuthentication::Authenticate(const String &sUsername, const String &sPassword)
   {
      // Try to fetch this account
      account_.reset();

      if (sUsername.CompareNoCase(_T("administrator")) == 0)
      {
         String sPasswordCorrect = MA::IniFileSettings::Instance()->GetAdministratorPassword();

         if (sPasswordCorrect.IsEmpty())
         {
            // The administrator password has not been set yet (fresh install
            // before DBSetup, or the upgrade tool probing with an empty
            // password). Only an empty submitted password authenticates.
            if (sPassword.IsEmpty())
            {
               account_ = std::shared_ptr<Account>
                  (
                     new Account("Administrator", Account::ServerAdmin)
                  );
            }
         }
         else if (MA::Crypt::Instance()->GetHashType(sPasswordCorrect) == MA::Crypt::ETPBKDF2 &&
                  MA::Crypt::Instance()->Validate(sPassword, sPasswordCorrect, MA::Crypt::ETPBKDF2))
         {
            // Mailaholic stores the administrator password only as PBKDF2. A
            // stored value in any other format (legacy MD5/SHA-256) is rejected;
            // reset it via DBSetup.

            // Create a dummy account since the administrator
            // does not have a real email account.
            account_ = std::shared_ptr<Account>
               (
                  new Account("Administrator", Account::ServerAdmin)
               );
         }
      }
      else
      {
         account_ = MA::PasswordValidator::ValidatePassword(sUsername, sPassword);
      }

      return account_;
   }

   void 
   COMAuthentication::AttempAnonymousAuthentication()
   {
      // No authentication is required if the administration password is empty.
      String sAdminPassword = MA::IniFileSettings::Instance()->GetAdministratorPassword();
      if (sAdminPassword.IsEmpty())
      {
         // Create a dummy account since the administrator
         // does not have a real email account.

         account_ = std::shared_ptr<Account> (new Account("Administrator", Account::ServerAdmin));
      }
   }

   bool 
   COMAuthentication::GetIsAuthenticated() const
   {
      return account_ != 0;
   }

   __int64 
   COMAuthentication::GetAccountID() const
   {
      return account_->GetID();
   }

   __int64 
   COMAuthentication::GetDomainID() const
   {
      return account_->GetDomainID();
   }

   bool 
   COMAuthentication::GetIsDomainAdmin() const
   {
      if (GetIsServerAdmin())
         return true;

      return account_ && 
             account_->GetAdminLevel() == Account::DomainAdmin;
   }

   bool 
   COMAuthentication::GetIsServerAdmin() const
   {
      return (account_ && account_->GetAdminLevel() == Account::ServerAdmin);
   }

   int 
   COMAuthentication::GetAccessDenied() const
   {
      return COMError::GenerateError("You do not have access to this property / method. Ensure that hMailServer.Application.Authenticate() is called with proper login credentials.");
   }

}