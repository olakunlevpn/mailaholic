// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"

// InterfaceEventLog

class ATL_NO_VTABLE InterfaceEventLog : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceEventLog, &CLSID_EventLog>,
	public IDispatchImpl<IInterfaceEventLog, &IID_IInterfaceEventLog, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	InterfaceEventLog()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEEVENTLOG)


BEGIN_COM_MAP(InterfaceEventLog)
	COM_INTERFACE_ENTRY(IInterfaceEventLog)
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
   
   STDMETHOD(Write)(BSTR sMessage);

};

OBJECT_ENTRY_AUTO(__uuidof(EventLog), InterfaceEventLog)
