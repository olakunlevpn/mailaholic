// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"
#include "ServerMessage.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   ServerMessage::ServerMessage(void)
   {

   }

   ServerMessage::~ServerMessage(void)
   {
   }

   bool 
   ServerMessage::XMLStore(XNode *pParentNode)
   {

      return true;
   }

   bool 
   ServerMessage::XMLLoad(XNode *pNode)
   {

      return true;
   }
}