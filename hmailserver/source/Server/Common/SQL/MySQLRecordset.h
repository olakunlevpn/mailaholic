// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "DALRecordset.h"
#include "MySQLInterface.h"

namespace MA
{

   class ColumnPositions;

   class MySQLRecordset  : public DALRecordset
   {
   public:
	   MySQLRecordset();
	   virtual ~MySQLRecordset();

      virtual DALConnection::ExecutionResult TryOpen(std::shared_ptr<DALConnection> pDALConn, const SQLCommand &command, String &sErrorMessage);
      
      virtual bool MoveNext();
      virtual bool IsEOF() const;
      virtual long RecordCount() const; 

      virtual long GetLongValue(const AnsiString &FieldName) const;
      virtual String GetStringValue(const AnsiString &FieldName) const;
      virtual __int64 GetInt64Value(const AnsiString &FieldName) const;
      virtual double GetDoubleValue(const AnsiString &FieldName) const;

      std::vector<AnsiString> GetColumnNames() const;

      virtual bool GetIsNull(const AnsiString &FieldName) const;

   private:

      int GetColumnIndex_(const AnsiString &sColumnName) const;

      

      void Close_();

      hm_MYSQL_RES *result_;
      hm_MYSQL_ROW current_;

      std::vector<AnsiString> columns_;

      std::shared_ptr<ColumnPositions> column_positions_;
   };
}
