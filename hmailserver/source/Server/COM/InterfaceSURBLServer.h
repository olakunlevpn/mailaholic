// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Mailaholic/resource.h"       // main symbols
#include "../Mailaholic/Mailaholic.h"

#include "COMCollection.h"

#include "..\Common\BO\SURBLServer.h"
#include "..\Common\BO\SURBLServers.h"

class ATL_NO_VTABLE InterfaceSURBLServer :
   public COMCollectionItem<MA::SURBLServer, MA::SURBLServers>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceSURBLServer, &CLSID_SURBLServer>,
	public IDispatchImpl<IInterfaceSURBLServer, &IID_IInterfaceSURBLServer, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceSURBLServer()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACESURBLSERVER)


BEGIN_COM_MAP(InterfaceSURBLServer)
	COM_INTERFACE_ENTRY(IInterfaceSURBLServer)
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

   STDMETHOD(get_Active)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(put_Active)(/*[in]*/ VARIANT_BOOL newVal);
   STDMETHOD(get_ID)(/*[out, retval]*/ long *pVal);

   STDMETHOD(get_DNSHost)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(put_DNSHost)(/*[in]*/ BSTR newVal);
   STDMETHOD(get_RejectMessage)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(put_RejectMessage)(/*[in]*/ BSTR newVal);

   STDMETHOD(get_Score)(/*[out, retval]*/ long *pVal);
   STDMETHOD(put_Score)(/*[in]*/ long newVal);

};

OBJECT_ENTRY_AUTO(__uuidof(SURBLServer), InterfaceSURBLServer)
