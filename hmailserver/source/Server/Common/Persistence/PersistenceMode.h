// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA   
{
   enum PersistenceMode
   {
      PersistenceModeNormal = 0,
      PersistenceModeRestore = 1,
      PersistenceModeRename = 2,
   };
   
}