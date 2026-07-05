// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Mailaholic/resource.h"       // main symbols
#include "../Mailaholic/Mailaholic.h"

namespace MA
{
   class WhiteListAddresses;
}


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// InterfaceWhiteListAddresses

class ATL_NO_VTABLE InterfaceWhiteListAddresses :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceWhiteListAddresses, &CLSID_WhiteListAddresses>,
	public IDispatchImpl<IInterfaceWhiteListAddresses, &IID_IInterfaceWhiteListAddresses, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceWhiteListAddresses()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEWHITELISTADDRESSES)


BEGIN_COM_MAP(InterfaceWhiteListAddresses)
	COM_INTERFACE_ENTRY(IInterfaceWhiteListAddresses)
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
   STDMETHOD(Refresh)();
   STDMETHOD(Clear)();

   STDMETHOD(get_Item)(/*[in]*/ long Index, /*[out, retval]*/ IInterfaceWhiteListAddress **pVal);
   STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
   STDMETHOD(get_ItemByDBID)(/*[in]*/ long lDBID, /*[out, retval]*/ IInterfaceWhiteListAddress** pVal);
   STDMETHOD(get_ItemByName)(/*[in]*/ BSTR sName, /*[out, retval]*/ IInterfaceWhiteListAddress** pVal);
   STDMETHOD(DeleteByDBID)(/*[in]*/ long DBID);
   STDMETHOD(Add)(/*[out, retval]*/ IInterfaceWhiteListAddress **pVal);

   void Attach(std::shared_ptr<MA::WhiteListAddresses> pWhiteListAddresses);

public:

   std::shared_ptr<MA::WhiteListAddresses> object_;

};

OBJECT_ENTRY_AUTO(__uuidof(WhiteListAddresses), InterfaceWhiteListAddresses)
