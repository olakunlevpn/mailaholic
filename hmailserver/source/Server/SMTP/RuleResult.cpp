// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"

#include "./RuleResult.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{ 
   RuleResult::RuleResult(void) :
      send_using_route_(0),
      delete_email_(false)
   {
   }

   RuleResult::~RuleResult(void)
   {
   }

}