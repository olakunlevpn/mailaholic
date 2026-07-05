// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "DNSRecord.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   DNSRecord::DNSRecord(AnsiString value, int recordType, int preference)
   {
      value_ = value;
      record_type_ = recordType;
      preference_ = preference;
   }
}
