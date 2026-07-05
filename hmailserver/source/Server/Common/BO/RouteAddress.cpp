// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "RouteAddress.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   RouteAddress::RouteAddress()
   {
      route_id_ = 0;
   }

   RouteAddress::~RouteAddress()
   {

   }

   bool 
   RouteAddress::XMLStore(XNode *pParentNode, int iOptions)
   {
      XNode *pNode = pParentNode->AppendChild(_T("RouteAddress"));

      pNode->AppendAttr(_T("Name"), address_);

      return true;
   }

   bool 
   RouteAddress::XMLLoad(XNode *pNode, int iOptions)
   {
      address_ = pNode->GetAttrValue(_T("Name"));

      return true;
   }

}
