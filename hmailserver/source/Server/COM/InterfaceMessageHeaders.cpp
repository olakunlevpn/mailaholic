// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "COMError.h"
#include "InterfaceMessageHeaders.h"
#include "InterfaceMessageHeader.h"

#include "../Common/Mime/Mime.h"

void 
InterfaceMessageHeaders::AttachItem(std::shared_ptr<MA::MimeHeader> pHeader)
{
   header_ = pHeader;
}

STDMETHODIMP InterfaceMessageHeaders::get_Count(long *pVal)
{
   try
   {
      if (!header_)
         return DISP_E_BADINDEX;
   
      *pVal = header_->GetFieldCount();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP 
InterfaceMessageHeaders::get_Item(long Index, IInterfaceMessageHeader **pVal)
{
   try
   {
      if (!header_)
         return DISP_E_BADINDEX;
   
      MA::MimeField *pField = header_->GetField(Index);
   
      if (!pField)
         return DISP_E_BADINDEX;
   
      CComObject<InterfaceMessageHeader>* pInterfaceMessageHeader = new CComObject<InterfaceMessageHeader>();
      pInterfaceMessageHeader->AttachItem(header_, pField);
      
      pInterfaceMessageHeader->AddRef();
   
      *pVal = pInterfaceMessageHeader;
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP 
InterfaceMessageHeaders::get_ItemByName(BSTR sName, IInterfaceMessageHeader **pVal)
{
   try
   {
      if (!header_)
         return DISP_E_BADINDEX;
   
      MA::AnsiString sFieldName = sName;
      MA::MimeField *pField = header_->GetField(sFieldName);
   
      if (!pField)
         return DISP_E_BADINDEX;
   
      CComObject<InterfaceMessageHeader>* pInterfaceMessageHeader = new CComObject<InterfaceMessageHeader>();
      pInterfaceMessageHeader->AttachItem(header_, pField);
      pInterfaceMessageHeader->AddRef();
   
      *pVal = pInterfaceMessageHeader;
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}


