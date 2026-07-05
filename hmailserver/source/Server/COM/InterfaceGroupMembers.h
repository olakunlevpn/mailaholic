// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Mailaholic/resource.h"       // main symbols
#include "../Mailaholic/Mailaholic.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

namespace MA
{
   class GroupMembers;
}

class ATL_NO_VTABLE InterfaceGroupMembers :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceGroupMembers, &CLSID_GroupMembers>,
	public IDispatchImpl<IInterfaceGroupMembers, &IID_IInterfaceGroupMembers, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceGroupMembers()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEGROUPMEMBERS)


BEGIN_COM_MAP(InterfaceGroupMembers)
	COM_INTERFACE_ENTRY(IInterfaceGroupMembers)
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

   STDMETHOD(get_Item)(/*[in]*/ long Index, /*[out, retval]*/ IInterfaceGroupMember **pVal);
   STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
   STDMETHOD(get_ItemByDBID)(/*[in]*/ long lDBID, /*[out, retval]*/ IInterfaceGroupMember** pVal);
   STDMETHOD(DeleteByDBID)(/*[in]*/ long DBID);
   STDMETHOD(Add)(/*[out, retval]*/ IInterfaceGroupMember **pVal);

   void Attach(std::shared_ptr<MA::GroupMembers> pGroupMembers);

public:

   std::shared_ptr<MA::GroupMembers> group_members_;
};

OBJECT_ENTRY_AUTO(__uuidof(GroupMembers), InterfaceGroupMembers)
