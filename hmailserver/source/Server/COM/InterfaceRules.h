// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Mailaholic/resource.h"    // main symbols
#include "../Mailaholic/Mailaholic.h"



// InterfaceRules
namespace MA
{
   class Rules;
}

class ATL_NO_VTABLE InterfaceRules : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceRules, &CLSID_Rules>,
	public IDispatchImpl<IInterfaceRules, &IID_IInterfaceRules, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceRules()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACERULES)


BEGIN_COM_MAP(InterfaceRules)
	COM_INTERFACE_ENTRY(IInterfaceRules)
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

   void Attach(std::shared_ptr<MA::Rules> pRules);

   STDMETHOD(get_ItemByDBID)(/*[in]*/ long DBID, /*[out, retval]*/ IInterfaceRule** pVal);
   STDMETHOD(get_Item)(/*[in]*/ long Index, /*[out, retval]*/ IInterfaceRule **pVal);
   STDMETHOD(get_Count)(LONG* pVal);
   STDMETHOD(Add)(IInterfaceRule** pVal);
   STDMETHOD(DeleteByDBID)(LONG DBID);
   STDMETHOD(Refresh)(void);

private:

   std::shared_ptr<MA::Rules> rules_;
};

OBJECT_ENTRY_AUTO(__uuidof(Rules), InterfaceRules)
