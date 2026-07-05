// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Mailaholic/resource.h"    // main symbols

#include "../Mailaholic/Mailaholic.h"

#include "COMCollection.h"

namespace MA
{
   class RuleCriteria;
   class RuleCriterias;
}

class ATL_NO_VTABLE InterfaceRuleCriteria : 
   public COMCollectionItem<MA::RuleCriteria, MA::RuleCriterias>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceRuleCriteria, &CLSID_RuleCriteria>,
	public IDispatchImpl<IInterfaceRuleCriteria, &IID_IInterfaceRuleCriteria, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceRuleCriteria()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACERULECRITERIA)


BEGIN_COM_MAP(InterfaceRuleCriteria)
	COM_INTERFACE_ENTRY(IInterfaceRuleCriteria)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
   STDMETHOD(Save)();
   STDMETHOD(Delete)();

   STDMETHOD(get_ID)(LONG* pVal);

   STDMETHOD(get_RuleID)(LONG* pVal);
   STDMETHOD(put_RuleID)(LONG pVal);

   STDMETHOD(get_UsePredefined)(VARIANT_BOOL* pVal);
   STDMETHOD(put_UsePredefined)(VARIANT_BOOL pVal);

   STDMETHOD(get_PredefinedField)(eRulePredefinedField* pVal);
   STDMETHOD(put_PredefinedField)(eRulePredefinedField pVal);

   STDMETHOD(get_HeaderField)(BSTR* pVal);
   STDMETHOD(put_HeaderField)(BSTR pVal);

   STDMETHOD(get_MatchType)(eRuleMatchType* pVal);
   STDMETHOD(put_MatchType)(eRuleMatchType pVal);

   STDMETHOD(get_MatchValue)(BSTR* pVal);
   STDMETHOD(put_MatchValue)(BSTR pVal);


};

OBJECT_ENTRY_AUTO(__uuidof(RuleCriteria), InterfaceRuleCriteria)
