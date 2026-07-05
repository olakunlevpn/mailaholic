// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic
// Created 2008-08-12

#include "StdAfx.h"
#include "IOOperation.h"
#include "../Util/ByteBuffer.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   IOOperation::IOOperation(OperationType type, std::shared_ptr<ByteBuffer> buffer) :
      type_(type),
      buffer_(buffer)
   {
      
   }

   IOOperation::IOOperation(OperationType type, const AnsiString &string) :
      type_(type),
      string_(string)
   {
      
   }

   IOOperation::~IOOperation(void)
   {

   }

   
}