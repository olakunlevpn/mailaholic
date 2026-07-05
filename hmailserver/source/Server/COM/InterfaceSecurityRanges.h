// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"


#include "../Common/bo/SecurityRanges.h"

class ATL_NO_VTABLE InterfaceSecurityRanges : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceSecurityRanges, &CLSID_SecurityRanges>,
	public IDispatchImpl<IInterfaceSecurityRanges, &IID_IInterfaceSecurityRanges, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceSecurityRanges()
	{
	}

   bool LoadSettings();

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACESECURITYRANGES)


BEGIN_COM_MAP(InterfaceSecurityRanges)
	COM_INTERFACE_ENTRY(IInterfaceSecurityRanges)
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

   STDMETHOD(get_Item)(/*[in]*/ long Index, /*[out, retval]*/ IInterfaceSecurityRange **pVal);
   STDMETHOD(get_ItemByDBID)(/*[in]*/ long DBID, /*[out, retval]*/ IInterfaceSecurityRange **pVal);
   STDMETHOD(get_ItemByName)(/*[in]*/ BSTR sName, /*[out, retval]*/ IInterfaceSecurityRange **pVal);
   STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
   STDMETHOD(Delete)(/*[in]*/ long Index);
   STDMETHOD(DeleteByDBID)(/*[in]*/ long DBID);
   STDMETHOD(Refresh)();
   STDMETHOD(SetDefault)();
   STDMETHOD(Add)(/*[out, retval]*/ IInterfaceSecurityRange** pVal);

   void Attach(std::shared_ptr<MA::SecurityRanges> pRanges);
private:


   std::shared_ptr<MA::SecurityRanges> security_ranges_;

};

OBJECT_ENTRY_AUTO(__uuidof(SecurityRanges), InterfaceSecurityRanges)
