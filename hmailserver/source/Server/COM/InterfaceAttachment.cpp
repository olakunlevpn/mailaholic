// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "COMError.h"
#include "InterfaceAttachment.h"

void 
InterfaceAttachment::Attach(std::shared_ptr<MA::Attachment> pAttachment) 
{ 

   attachment_ = pAttachment; 
}

STDMETHODIMP InterfaceAttachment::get_Filename(BSTR *pVal)
{
   try
   {
      if (!attachment_)
         return GetAccessDenied();

      *pVal = attachment_->GetFileName().AllocSysString();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceAttachment::get_Size(long *pVal)
{
   try
   {
      if (!attachment_)
         return GetAccessDenied();

      *pVal = attachment_->GetSize();
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceAttachment::SaveAs(BSTR sName)
{
   try
   {
      if (!attachment_)
         return GetAccessDenied();

      attachment_->SaveAs(sName);
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP 
InterfaceAttachment::Delete()
{
   try
   {
      if (!attachment_)
         return GetAccessDenied();

      attachment_->Delete();
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}


