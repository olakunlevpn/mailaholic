// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"
#include "BlockedAttachment.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   BlockedAttachment::BlockedAttachment(void)
   {
      
   }

   BlockedAttachment::~BlockedAttachment(void)
   {
   }

   bool 
   BlockedAttachment::XMLStore(XNode *pParentNode, int iOptions)
   {
      XNode *pNode = pParentNode->AppendChild(_T("BlockedAttachment"));

      pNode->AppendAttr(_T("Name"), wildcard_);
      pNode->AppendAttr(_T("Description"), description_);

      return true;
   }

   bool 
   BlockedAttachment::XMLLoad(XNode *pNode, int iOptions)
   {
      wildcard_ = pNode->GetAttrValue(_T("Name"));
      description_ = pNode->GetAttrValue(_T("Description"));

      return true;
   }
}