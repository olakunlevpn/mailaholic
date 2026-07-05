// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "InterfaceDistributionList.h"
#include "InterfaceDistributionListRecipients.h"

#include "../common/persistence/PersistentDistributionList.h"
#include "../common/persistence/PersistentDistributionListRecipient.h"

#include "../common/bo/DistributionLists.h"
#include "../common/bo/DistributionListRecipients.h"

#include "COMError.h"


#ifdef _DEBUG
   long InterfaceDistributionList::counter = 0;
#endif

STDMETHODIMP InterfaceDistributionList::InterfaceSupportsErrorInfo(REFIID riid)
{
   try
   {
      static const IID* arr[] = 
      {
         &IID_IInterfaceDistributionList,
      };
   
      for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
      {
         if (InlineIsEqualGUID(*arr[i],riid))
            return S_OK;
      }
      return S_FALSE;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::get_ID(long *pVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

      *pVal = (long) object_->GetID();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::get_Address(BSTR *pVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

   
   
      MA::String sName = object_->GetAddress();
   
      *pVal = sName.AllocSysString();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::put_Address(BSTR newVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

   
   
      MA::String sName = newVal;
      object_->SetAddress(sName);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::get_RequireSenderAddress(BSTR *pVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

   
   
      MA::String sAddress = object_->GetRequireAddress();
   
      *pVal = sAddress.AllocSysString();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::put_RequireSenderAddress(BSTR newVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

   
   
      MA::String sName = newVal;
      object_->SetRequireAddress(sName);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::get_RequireSMTPAuth(VARIANT_BOOL *pVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

   
   
      if (object_->GetRequireAuth())
         *pVal = VARIANT_TRUE;
      else
         *pVal = VARIANT_FALSE;
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::put_RequireSMTPAuth(VARIANT_BOOL newVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

   
   
      if (newVal == VARIANT_TRUE)
         object_->SetRequireAuth(true);
      else
         object_->SetRequireAuth(false);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::get_Active(VARIANT_BOOL *pVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

   
   
      if (object_->GetActive())
         *pVal = VARIANT_TRUE;
      else
         *pVal = VARIANT_FALSE;
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::put_Active(VARIANT_BOOL newVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

   
   
      if (newVal == VARIANT_TRUE)
         object_->SetActive(true);
      else
         object_->SetActive(false);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::Delete()
{
   try
   {
      if (!object_)
         return GetAccessDenied();

   
   
      if (!object_)
         return S_FALSE;
   
      MA::PersistentDistributionList::DeleteObject(object_);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::Save()
{
   try
   {
      if (!object_)
         return GetAccessDenied();

      if (!object_)
         return S_FALSE;
   
      MA::String sErrorMessage;
      if (MA::PersistentDistributionList::SaveObject(object_, sErrorMessage, MA::PersistenceModeNormal))
      {
         // Add to parent collection
         AddToParentCollection();
   
         return S_OK;
      }
   
      return COMError::GenerateError("Failed to save object. " +  sErrorMessage);
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::get_Recipients(IInterfaceDistributionListRecipients **pVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

      CComObject<InterfaceDistributionListRecipients>* pItem = new CComObject<InterfaceDistributionListRecipients>();
      pItem->SetAuthentication(authentication_);
   
      pItem->SetListID(object_->GetID());
      std::shared_ptr<MA::DistributionListRecipients> pRecipients = object_->GetMembers();
   
      if (pRecipients)
      {
         pItem->Attach(pRecipients);
         pItem->AddRef();
         *pVal = pItem;
      }
   
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::get_Mode(eDistributionListMode *pVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

      switch (object_->GetListMode())
      {
      case MA::DistributionList::LMPublic:
         *pVal = eLMPublic;
         break;
      case MA::DistributionList::LMMembership:
         *pVal = eLMMembership;
         break;
      case MA::DistributionList::LMAnnouncement:
         *pVal = eLMAnnouncement;
         break;
      case MA::DistributionList::LMDomainMembers:
         *pVal = eLMDomainMembers;
         break;
      default:
         *pVal = eLMPublic;
         break;
      }
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDistributionList::put_Mode(eDistributionListMode newVal)
{
   try
   {
      if (!object_)
         return GetAccessDenied();

      MA::DistributionList::ListMode iMode = MA::DistributionList::LMPublic;
   
      switch (newVal)
      {
      case eLMPublic:
         iMode = MA::DistributionList::LMPublic;
         break;
      case eLMMembership:
         iMode = MA::DistributionList::LMMembership;
         break;
      case eLMAnnouncement:
         iMode = MA::DistributionList::LMAnnouncement;
         break;
      case eLMDomainMembers:
         iMode = MA::DistributionList::LMDomainMembers;
         break;
      }
   
      object_->SetListMode(iMode);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

