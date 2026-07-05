// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"


// InterfaceClient

namespace MA
{
   class ClientInfo;
}

class ATL_NO_VTABLE InterfaceClient : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceClient, &CLSID_Client>,
	public IDispatchImpl<IInterfaceClient, &IID_IInterfaceClient, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	InterfaceClient()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACECLIENT)


BEGIN_COM_MAP(InterfaceClient)
	COM_INTERFACE_ENTRY(IInterfaceClient)
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
   void AttachItem(std::shared_ptr<MA::ClientInfo> pClientInfo);

   STDMETHOD(get_Port)(LONG* pVal);
   STDMETHOD(get_IPAddress)(BSTR* pVal);
   STDMETHOD(get_Username)(BSTR* pVal);
   STDMETHOD(get_HELO)(BSTR* pVal);
   STDMETHOD(get_Authenticated)(VARIANT_BOOL* pVal);
   STDMETHOD(get_EncryptedConnection)(VARIANT_BOOL* pVal);
   STDMETHOD(get_CipherVersion)(BSTR* pVal);
   STDMETHOD(get_CipherName)(BSTR* pVal);
   STDMETHOD(get_CipherBits)(LONG* pVal);
   STDMETHOD(get_SessionID)(LONG* pVal);

private:

   std::shared_ptr<MA::ClientInfo> client_info_;
};

OBJECT_ENTRY_AUTO(__uuidof(Client), InterfaceClient)
