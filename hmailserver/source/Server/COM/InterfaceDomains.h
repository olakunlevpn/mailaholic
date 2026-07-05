// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"

namespace MA
{
   class Domains;
}

class ATL_NO_VTABLE InterfaceDomains : 
   public ISupportErrorInfo,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceDomains, &CLSID_Domains>,
	public IDispatchImpl<IInterfaceDomains, &IID_IInterfaceDomains, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
   InterfaceDomains()
   {
#ifdef _DEBUG
      InterlockedIncrement(&counter);
#endif
   }

   ~InterfaceDomains()
   {
#ifdef _DEBUG
      InterlockedDecrement(&counter);
#endif
   }

   void SetAuthentication(std::shared_ptr<MA::COMAuthentication> pAuthentication);

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEDOMAINS)


BEGIN_COM_MAP(InterfaceDomains)
	COM_INTERFACE_ENTRY(IInterfaceDomains)
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
   STDMETHOD(get_ItemByDBID)(/*[in]*/ long DBID, /*[out, retval]*/ IInterfaceDomain* *pVal);
   STDMETHOD(get_Item)(/*[in]*/ long Index, /*[out, retval]*/ IInterfaceDomain* *pVal);
   STDMETHOD(get_ItemByName)(/*[in]*/ BSTR ItemName, /*[out, retval]*/ IInterfaceDomain** pVal);
   STDMETHOD(get_Names)(/*[out, retval]*/ BSTR* pVal);
   STDMETHOD(get_Count)(/*[out, retval]*/ long* pVal);
   STDMETHOD(Refresh)();
   STDMETHOD(Add)(/*[out, retval] */IInterfaceDomain** pVal);
   STDMETHOD(DeleteByDBID)(/*[in]*/ long DBID);

private:

   std::shared_ptr<MA::Domains> objects_;

#ifdef _DEBUG
   static long counter;
#endif

};

OBJECT_ENTRY_AUTO(__uuidof(Domains), InterfaceDomains)
