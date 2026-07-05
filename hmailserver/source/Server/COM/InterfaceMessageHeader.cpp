// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "COMError.h"
#include "InterfaceMessageHeader.h"

#include "../Common/Mime/Mime.h"

InterfaceMessageHeader::InterfaceMessageHeader() :
   object_(nullptr)
{

}

void 
InterfaceMessageHeader::AttachItem (std::shared_ptr<MA::MimeHeader> pHeader, MA::MimeField *pField)
{
   header_ = pHeader;
   object_ = pField;
}


STDMETHODIMP 
InterfaceMessageHeader::Delete()
{
   try
   {
      header_->DeleteField(object_);
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceMessageHeader::put_Name(BSTR newVal)
{
   try
   {
      MA::AnsiString sName = newVal;
      object_->SetName(sName);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceMessageHeader::get_Name(BSTR *pVal)
{
   try
   {
      MA::String sName = object_->GetName();
      *pVal = sName.AllocSysString();
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceMessageHeader::put_Value(BSTR newVal)
{
   try
   {
      MA::AnsiString sValue = newVal;
      object_->SetValue(sValue);
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceMessageHeader::get_Value(BSTR *pVal)
{
   try
   {
      MA::String sValue = object_->GetValue();
      *pVal = sValue.AllocSysString();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}


