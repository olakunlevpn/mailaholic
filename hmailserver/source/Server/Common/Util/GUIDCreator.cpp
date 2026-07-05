// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "GUIDCreator.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   GUIDCreator::GUIDCreator()
   {

   }

   GUIDCreator::~GUIDCreator()
   {

   }

   String
   GUIDCreator::GetGUID()
   {
      GUID uuid;
      CoCreateGuid(&uuid);

      wchar_t szGUID[39];
      StringFromGUID2 (uuid, szGUID, 39);

      return szGUID;
   }
}
