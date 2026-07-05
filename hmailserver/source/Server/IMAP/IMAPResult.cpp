// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "IMAPResult.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{

   IMAPResult::IMAPResult(Result result, const String &message) :
      result_(result),
      message_(message)
   {
      
   }

   IMAPResult::IMAPResult() :
      result_(ResultOK),
      message_("")
   {

   }

   IMAPResult::IMAPResult(const IMAPResult &otherCopy)
   {
      result_ = otherCopy.GetResult();
      message_ = otherCopy.GetMessage();
   }

   IMAPResult& 
   IMAPResult::operator= (const IMAPResult&otherCopy)
   {
      result_ = otherCopy.GetResult();
      message_ = otherCopy.GetMessage();

      return *this;
   }


   IMAPResult::~IMAPResult()
   {
      
   }
}
