// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "COMError.h"
#include "InterfaceDomainAliases.h"

#include "InterfaceDomainAlias.h"

#include "../Common/BO/DomainAliases.h"
#include "../Common/BO/DomainAlias.h"

void 
InterfaceDomainAliases::Attach(std::shared_ptr<MA::DomainAliases> pDomainAliases)
{
   domain_aliases_ = pDomainAliases;
}

STDMETHODIMP InterfaceDomainAliases::get_ItemByDBID(long lDBID, IInterfaceDomainAlias** pVal)
{
   try
   {
      if (!domain_aliases_)
         return GetAccessDenied();

      CComObject<InterfaceDomainAlias>* pInterfaceDA = new CComObject<InterfaceDomainAlias>();
      pInterfaceDA->SetAuthentication(authentication_);
   
      std::shared_ptr<MA::DomainAlias> pDomainAlias = domain_aliases_->GetItemByDBID(lDBID);
      if (!pDomainAlias)
         return DISP_E_BADINDEX;
   
      pInterfaceDA->AttachItem(pDomainAlias);
      pInterfaceDA->AttachParent(domain_aliases_, true);
      pInterfaceDA->AddRef();
      *pVal = pInterfaceDA;   
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDomainAliases::get_Item(long lIndex, IInterfaceDomainAlias** pVal)
{
   try
   {
      if (!domain_aliases_)
         return GetAccessDenied();

      CComObject<InterfaceDomainAlias>* pInterfaceAccount = new CComObject<InterfaceDomainAlias>();
      pInterfaceAccount->SetAuthentication(authentication_);
   
      std::shared_ptr<MA::DomainAlias> pDomainAlias = domain_aliases_->GetItem(lIndex);
      if (!pDomainAlias)
         return DISP_E_BADINDEX;
   
      pInterfaceAccount->AttachItem(pDomainAlias);
      pInterfaceAccount->AttachParent(domain_aliases_, true);
      pInterfaceAccount->AddRef();
      *pVal = pInterfaceAccount;   
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDomainAliases::Add(IInterfaceDomainAlias **pVal)
{
   try
   {
      if (!domain_aliases_)
         return GetAccessDenied();

      if (!domain_aliases_)
         return authentication_->GetAccessDenied();
   
      CComObject<InterfaceDomainAlias>* pIntDA = new CComObject<InterfaceDomainAlias>();
      pIntDA->SetAuthentication(authentication_);
   
      std::shared_ptr<MA::DomainAlias> pDA = std::shared_ptr<MA::DomainAlias>(new MA::DomainAlias);
   
      pDA->SetDomainID(domain_aliases_->GetDomainID());
   
      pIntDA->AttachItem(pDA);
      pIntDA->AttachParent(domain_aliases_, false);
   
      pIntDA->AddRef();
   
      *pVal = pIntDA;
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDomainAliases::Refresh(void)
{
   try
   {
      if (!domain_aliases_)
         return GetAccessDenied();

      domain_aliases_->Refresh();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDomainAliases::Delete(LONG Index)
{
   try
   {
      if (!domain_aliases_)
         return GetAccessDenied();

      domain_aliases_->DeleteItem(Index);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDomainAliases::DeleteByDBID(LONG DBID)
{
   try
   {
      if (!domain_aliases_)
         return GetAccessDenied();

      domain_aliases_->DeleteItemByDBID(DBID);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceDomainAliases::get_Count(long *pVal)
{
   try
   {
      if (!domain_aliases_)
         return GetAccessDenied();

      *pVal = domain_aliases_->GetCount();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}


