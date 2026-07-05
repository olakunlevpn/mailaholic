// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once


namespace MA
{
   class Account;

   class PersistentLogonFailure
   {
   public:
	   PersistentLogonFailure();
	   virtual ~PersistentLogonFailure();

      int GetCurrrentFailureCount(const IPAddress & ipaddress);
      bool AddFailure(const IPAddress &ipaddress);
      bool ClearOldFailures(int olderThanMinutes);
      bool ClearFailuresByIP(const IPAddress &ipaddress);
   };


}