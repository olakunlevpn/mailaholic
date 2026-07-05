// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "StdAfx.h"
#include "COMError.h"

#include "InterfaceFetchAccounts.h"

#include "../COM/InterfaceFetchAccount.h"

void 
InterfaceFetchAccounts::Attach(std::shared_ptr<MA::FetchAccounts> pFetchAccounts)
{
   fetch_accounts_ = pFetchAccounts;
}

STDMETHODIMP InterfaceFetchAccounts::get_ItemByDBID(long lDBID, IInterfaceFetchAccount** pVal)
{
   try
   {
      if (!fetch_accounts_)
         return GetAccessDenied();

      CComObject<InterfaceFetchAccount>* pInterfaceAccount = new CComObject<InterfaceFetchAccount>();
      pInterfaceAccount->SetAuthentication(authentication_);
   
      std::shared_ptr<MA::FetchAccount> pFetchAccount = fetch_accounts_->GetItemByDBID(lDBID);
      if (!pFetchAccount)
         return DISP_E_BADINDEX;
   
      pInterfaceAccount->AttachItem(pFetchAccount);
      pInterfaceAccount->AttachParent(fetch_accounts_, true);
      pInterfaceAccount->AddRef();
      *pVal = pInterfaceAccount;   
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceFetchAccounts::get_Item(long lIndex, IInterfaceFetchAccount** pVal)
{
   try
   {
      if (!fetch_accounts_)
         return GetAccessDenied();

      CComObject<InterfaceFetchAccount>* pInterfaceAccount = new CComObject<InterfaceFetchAccount>();
      pInterfaceAccount->SetAuthentication(authentication_);
   
      std::shared_ptr<MA::FetchAccount> pFetchAccount = fetch_accounts_->GetItem(lIndex);
      if (!pFetchAccount)
         return DISP_E_BADINDEX;
   
      pInterfaceAccount->AttachItem(pFetchAccount);
      pInterfaceAccount->AttachParent(fetch_accounts_, true);
      pInterfaceAccount->AddRef();
      *pVal = pInterfaceAccount;   
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceFetchAccounts::Refresh(void)
{
   try
   {
      if (!fetch_accounts_)
         return GetAccessDenied();

      fetch_accounts_->Refresh();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceFetchAccounts::Delete(LONG Index)
{
   try
   {
      if (!fetch_accounts_)
         return GetAccessDenied();

      fetch_accounts_->DeleteItem(Index);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceFetchAccounts::DeleteByDBID(LONG DBID)
{
   try
   {
      if (!fetch_accounts_)
         return GetAccessDenied();

      fetch_accounts_->DeleteItemByDBID(DBID);
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceFetchAccounts::get_Count(long *pVal)
{
   try
   {
      if (!fetch_accounts_)
         return GetAccessDenied();

      *pVal = fetch_accounts_->GetCount();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceFetchAccounts::Add(IInterfaceFetchAccount **pVal)
{
   try
   {
      if (!fetch_accounts_)
         return GetAccessDenied();

      if (!fetch_accounts_)
         return authentication_->GetAccessDenied();
   
      CComObject<InterfaceFetchAccount>* pIntFA = new CComObject<InterfaceFetchAccount>();
      pIntFA->SetAuthentication(authentication_);
   
      std::shared_ptr<MA::FetchAccount> pFA = std::shared_ptr<MA::FetchAccount>(new MA::FetchAccount);
   
      pFA->SetAccountID(fetch_accounts_->GetAccountID());
   
      pIntFA->AttachItem(pFA);
      pIntFA->AttachParent(fetch_accounts_, false);
   
      pIntFA->AddRef();
      *pVal = pIntFA;
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}


