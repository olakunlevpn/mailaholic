// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"


#include "../Common/bo/Aliases.h"

namespace MA
{
   class Alias;
}


class ATL_NO_VTABLE InterfaceAliases : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceAliases, &CLSID_Aliases>,
	public IDispatchImpl<IInterfaceAliases, &IID_IInterfaceAliases, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceAliases()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEALIASES)


BEGIN_COM_MAP(InterfaceAliases)
	COM_INTERFACE_ENTRY(IInterfaceAliases)
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
   STDMETHOD(get_ItemByDBID)(/*[in]*/ long DBID, /*[out, retval]*/ IInterfaceAlias* *pVal);
   STDMETHOD(Add)(/*[out, retval]*/ IInterfaceAlias **pVal);
   STDMETHOD(get_Item)(/*[in]*/ long Index, /*[out, retval]*/ IInterfaceAlias **pVal);
   STDMETHOD(Refresh)();
   STDMETHOD(Delete)(/*[in]*/ long Index);
   STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
   STDMETHOD(DeleteByDBID)(/*[in]*/ long Index);
   STDMETHOD(get_ItemByName)(BSTR Name, IInterfaceAlias **pVal);

   void Attach(std::shared_ptr<MA::Aliases> pAliases);
   void SetDomain(__int64 iDomain) { domain_id_ = iDomain; }

private:

   // void InternalAdd(std::shared_ptr<MA::Alias> pObject);

   std::shared_ptr<MA::Aliases> aliases_;
   __int64 domain_id_;

};

OBJECT_ENTRY_AUTO(__uuidof(Aliases), InterfaceAliases)
