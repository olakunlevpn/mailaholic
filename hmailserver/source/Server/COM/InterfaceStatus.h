// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"

namespace MA
{
   class ServerStatus;
}

class ATL_NO_VTABLE InterfaceStatus : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceStatus, &CLSID_Status>,
	public IDispatchImpl<IInterfaceStatus, &IID_IInterfaceStatus, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
   InterfaceStatus();
	
   bool LoadSettings();

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACESTATUS)


BEGIN_COM_MAP(InterfaceStatus)
	COM_INTERFACE_ENTRY(IInterfaceStatus)
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
   STDMETHOD(get_UndeliveredMessages)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(get_StartTime)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(get_ProcessedMessages)(/*[out, retval]*/ long *pVal);
   STDMETHOD(get_RemovedViruses)(/*[out, retval]*/ long *pVal);
   STDMETHOD(get_RemovedSpamMessages)(/*[out, retval]*/ long *pVal);
   STDMETHOD(get_SessionCount)(eSessionType iType, long *pVal);
   STDMETHOD(get_ThreadID)(/*[out, retval]*/ long* pVal);

private:

   MA::ServerStatus *status_;
   MA::Application *application_;
};

OBJECT_ENTRY_AUTO(__uuidof(Status), InterfaceStatus)
