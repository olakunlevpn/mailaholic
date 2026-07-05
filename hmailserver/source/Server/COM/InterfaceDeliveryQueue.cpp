// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "InterfaceDeliveryQueue.h"
#include "../SMTP/DeliveryQueue.h"
#include "COMError.h"
void
InterfaceDeliveryQueue::SetAuthentication(std::shared_ptr<MA::COMAuthentication> pAuthentication)
{
   authentication_ = pAuthentication;
}

STDMETHODIMP 
InterfaceDeliveryQueue::Clear()
{
   try
   {
      if (!authentication_)
         return GetAccessDenied();

      if (!authentication_->GetIsServerAdmin())
      {
         return COMError::GenerateError("Server admin privileges are required to clear queue.");
      }
   
      MA::DeliveryQueue::Clear();
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP 
InterfaceDeliveryQueue::ResetDeliveryTime(hyper iMessageID)
{
   try
   {
      if (!authentication_)
         return GetAccessDenied();

      if (!authentication_->GetIsServerAdmin())
         return S_FALSE;
   
      MA::DeliveryQueue::ResetDeliveryTime(iMessageID);
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP 
InterfaceDeliveryQueue::Remove(hyper iMessageID)
{
   try
   {
      if (!authentication_)
         return GetAccessDenied();

      if (!authentication_->GetIsServerAdmin())
         return S_FALSE;
   
      MA::DeliveryQueue::Remove(iMessageID);
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP 
InterfaceDeliveryQueue::StartDelivery()
{
   try
   {
      if (!authentication_)
         return GetAccessDenied();

      if (!authentication_->GetIsServerAdmin())
         return S_FALSE;
   
      MA::DeliveryQueue::StartDelivery();
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

