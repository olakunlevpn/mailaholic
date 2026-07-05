// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "ClientInfo.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   ClientInfo::ClientInfo() :
      port_(0),
      session_(0),
      is_authenticated_(false),
      is_encryptedconnection_(false),
      cipher_bits_(0)
   {

   }

   ClientInfo::~ClientInfo()
   {

   }


}
