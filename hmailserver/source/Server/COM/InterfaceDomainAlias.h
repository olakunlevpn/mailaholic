// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"

#include "COMCollection.h"

namespace MA 
{ 
   class DomainAlias; 
   class DomainAliases;
}



class ATL_NO_VTABLE InterfaceDomainAlias :
   public COMCollectionItem<MA::DomainAlias, MA::DomainAliases>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceDomainAlias, &CLSID_DomainAlias>,
	public IDispatchImpl<IInterfaceDomainAlias, &IID_IInterfaceDomainAlias, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator,
   public ISupportErrorInfo
{
public:
	InterfaceDomainAlias()
	{
	}
   ~InterfaceDomainAlias()
   {
   }

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEDOMAINALIAS)


BEGIN_COM_MAP(InterfaceDomainAlias)
	COM_INTERFACE_ENTRY(IInterfaceDomainAlias)
	COM_INTERFACE_ENTRY(IDispatch)
   COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

   STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

   STDMETHOD(Save)();

   STDMETHOD(get_ID)(LONG* pVal);

   STDMETHOD(get_DomainID)(LONG* pVal);

   STDMETHOD(get_AliasName)(BSTR* pVal);
   STDMETHOD(put_AliasName)(BSTR newVal);

   STDMETHOD(put_DomainID)(LONG newVal);

   STDMETHOD(Delete)();
private:

};

OBJECT_ENTRY_AUTO(__uuidof(DomainAlias), InterfaceDomainAlias)
