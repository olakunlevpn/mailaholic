// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "COMError.h"

#include "InterfaceLanguages.h"
#include "InterfaceLanguage.h"

#include "../Common/Util/Languages.h"

STDMETHODIMP InterfaceLanguages::get_Count(long *pVal)
{
   try
   {
      *pVal = (int) MA::Languages::Instance()->GetCount();
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceLanguages::get_ItemByName(BSTR ItemName, IInterfaceLanguage **pVal)
{
   try
   {
      CComObject<InterfaceLanguage>* pInterfaceLanguage = new CComObject<InterfaceLanguage>();
   
      std::shared_ptr<MA::Language> pLanguage = MA::Languages::Instance()->GetLanguage(ItemName);
   
      if (!pLanguage)
         return DISP_E_BADINDEX;
   
      pInterfaceLanguage->Attach(pLanguage);
      pInterfaceLanguage->AddRef();
   
      *pVal = pInterfaceLanguage;
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}

STDMETHODIMP InterfaceLanguages::get_Item(long Index, IInterfaceLanguage **pVal)
{
   try
   {
      CComObject<InterfaceLanguage>* pInterfaceLanguage = new CComObject<InterfaceLanguage>();
   
      std::shared_ptr<MA::Language> pLanguage = MA::Languages::Instance()->GetLanguage(Index);
   
      if (!pLanguage)
         return DISP_E_BADINDEX;
   
      pInterfaceLanguage->Attach(pLanguage);
      pInterfaceLanguage->AddRef();
   
      *pVal = pInterfaceLanguage;
   
   
      return S_OK;
   }
   catch (...)
   {
      return COMError::GenerateGenericMessage();
   }
}


