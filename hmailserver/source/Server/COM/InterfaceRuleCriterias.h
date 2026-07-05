// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Mailaholic/resource.h"    // main symbols
#include "../Mailaholic/Mailaholic.h"


namespace MA
{
   class RuleCriterias;
}

class ATL_NO_VTABLE InterfaceRuleCriterias : 
   
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceRuleCriterias, &CLSID_RuleCriterias>,
	public IDispatchImpl<IInterfaceRuleCriterias, &IID_IInterfaceRuleCriterias, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceRuleCriterias()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACERULECRITERIAS)


BEGIN_COM_MAP(InterfaceRuleCriterias)
	COM_INTERFACE_ENTRY(IInterfaceRuleCriterias)
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

   void Attach(std::shared_ptr<MA::RuleCriterias> pCriterias);

   STDMETHOD(get_ItemByDBID)(/*[in]*/ long DBID, /*[out, retval]*/ IInterfaceRuleCriteria** pVal);
   STDMETHOD(get_Item)(/*[in]*/ long Index, /*[out, retval]*/ IInterfaceRuleCriteria **pVal);
   STDMETHOD(get_Count)(LONG* pVal);
   STDMETHOD(Add)(IInterfaceRuleCriteria** pVal);
   STDMETHOD(DeleteByDBID)(LONG DBID);
   STDMETHOD(Refresh)(void);
   STDMETHOD(Delete)(LONG DBID);

private:

   std::shared_ptr<MA::RuleCriterias> rule_criterias;



};

OBJECT_ENTRY_AUTO(__uuidof(RuleCriterias), InterfaceRuleCriterias)
