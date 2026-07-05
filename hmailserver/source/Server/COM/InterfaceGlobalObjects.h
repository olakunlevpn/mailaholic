// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"


// InterfaceGlobalObjects

namespace MA
{
   class COMAuthentication;
}

class ATL_NO_VTABLE InterfaceGlobalObjects : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceGlobalObjects, &CLSID_GlobalObjects>,
   public ISupportErrorInfo,
	public IDispatchImpl<IInterfaceGlobalObjects, &IID_IInterfaceGlobalObjects, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceGlobalObjects()
	{
	}

   void SetAuthentication(std::shared_ptr<MA::COMAuthentication> pAuthentication);

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEGLOBALOBJECTS)


BEGIN_COM_MAP(InterfaceGlobalObjects)
	COM_INTERFACE_ENTRY(IInterfaceGlobalObjects)
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

public:
   
   STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
   
   STDMETHOD(get_DeliveryQueue)(/*[out, retval]*/ IInterfaceDeliveryQueue** pVal);
   STDMETHOD(get_Languages)(/*[out, retval]*/ IInterfaceLanguages **pVal);   

private:

   std::shared_ptr<MA::COMAuthentication> authentication_;
};

OBJECT_ENTRY_AUTO(__uuidof(GlobalObjects), InterfaceGlobalObjects)
