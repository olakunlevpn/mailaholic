// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class IPAddress;

   class  IPAddressSQLHelper
   {
   public:
	   IPAddressSQLHelper();

      AnsiString GetAddress1String(const IPAddress &address) const;
      AnsiString GetAddress2String(const IPAddress &address) const;

      AnsiString GetAddress1Equals(const IPAddress &address) const;
      AnsiString GetAddress2Equals(const IPAddress &address) const;

      IPAddress Construct(std::shared_ptr<DALRecordset> recordset, const AnsiString &address1Column, const AnsiString &address2Column) const;
      void AppendStatement(SQLStatement &statement, const IPAddress &address, const AnsiString &address1Column, const AnsiString &address2Column) const;
   };
}
