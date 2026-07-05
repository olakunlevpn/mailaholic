// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Mailaholic/resource.h"
#include "../Mailaholic/Mailaholic.h"

#include "../Common/BO/FetchAccountS.h"

// InterfaceFetchAccounts

class ATL_NO_VTABLE InterfaceFetchAccounts : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceFetchAccounts, &CLSID_FetchAccounts>,
	public IDispatchImpl<IInterfaceFetchAccounts, &IID_IInterfaceFetchAccounts, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator

{
public:
	InterfaceFetchAccounts()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEFETCHACCOUNTS)


BEGIN_COM_MAP(InterfaceFetchAccounts)
	COM_INTERFACE_ENTRY(IInterfaceFetchAccounts)
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

   void Attach(std::shared_ptr<MA::FetchAccounts> pFetchAccounts);

public:

   STDMETHOD(get_Count)(LONG* pVal);

   STDMETHOD(get_ItemByDBID)(/*[in]*/ long DBID, /*[out, retval]*/ IInterfaceFetchAccount** pVal);
   STDMETHOD(get_Item)(/*[in]*/ long Index, /*[out, retval]*/ IInterfaceFetchAccount **pVal);
   STDMETHOD(Refresh)();
   STDMETHOD(Delete)(/*[in]*/ long Index);
   STDMETHOD(DeleteByDBID)(/*[in]*/ long Index);

   STDMETHOD(Add)(/*[out, retval]*/ IInterfaceFetchAccount **pVal);

private:

   std::shared_ptr<MA::FetchAccounts> fetch_accounts_;
};

OBJECT_ENTRY_AUTO(__uuidof(FetchAccounts), InterfaceFetchAccounts)
