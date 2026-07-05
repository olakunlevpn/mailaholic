// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "DALRecordsetFactory.h"
#include "ADORecordset.h"
#include "MySQLRecordset.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   DALRecordsetFactory::DALRecordsetFactory()
   {

   }

   DALRecordsetFactory::~DALRecordsetFactory()
   {

   }


   /*std::shared_ptr<DALRecordset>
   DALRecordsetFactory::CreateRecordset()
   {

   
      return pRS;
   }*/
}