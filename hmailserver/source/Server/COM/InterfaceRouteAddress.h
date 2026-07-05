// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"

namespace MA
{
   class RouteAddress;
   class RouteAddresses;
}

#include "COMCollection.h"

class ATL_NO_VTABLE InterfaceRouteAddress : 
   public COMCollectionItem<MA::RouteAddress, MA::RouteAddresses>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceRouteAddress, &CLSID_RouteAddress>,
	public IDispatchImpl<IInterfaceRouteAddress, &IID_IInterfaceRouteAddress, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceRouteAddress()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEROUTEADDRESS)


BEGIN_COM_MAP(InterfaceRouteAddress)
	COM_INTERFACE_ENTRY(IInterfaceRouteAddress)
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

   STDMETHOD(get_ID)(/*[out, retval]*/ long *pVal);
   STDMETHOD(get_Address)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(put_Address)(/*[in]*/ BSTR newVal);
   STDMETHOD(get_RouteID)(/*[out, retval]*/ long *pVal);
   STDMETHOD(put_RouteID)(/*[in]*/ long newVal);
   STDMETHOD(Save)();
   STDMETHOD(Delete)();

private:

};

OBJECT_ENTRY_AUTO(__uuidof(RouteAddress), InterfaceRouteAddress)
