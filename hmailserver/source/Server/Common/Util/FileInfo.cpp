// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "FileInfo.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   FileInfo::FileInfo(const String &name, const DateTime &created) :
      name_(name),
      created_(created)

   {
      
   }

   FileInfo::FileInfo()
   {

   }

   

}