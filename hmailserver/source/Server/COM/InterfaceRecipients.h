// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"

namespace MA {class Message; }

class ATL_NO_VTABLE InterfaceRecipients : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceRecipients, &CLSID_Recipients>,
	public IDispatchImpl<IInterfaceRecipients, &IID_IInterfaceRecipients, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceRecipients()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACERECIPIENTS)


BEGIN_COM_MAP(InterfaceRecipients)
	COM_INTERFACE_ENTRY(IInterfaceRecipients)
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
   
   void Attach(std::shared_ptr<MA::Message> pMessage);

   STDMETHOD(get_Item)(/*[in]*/ long Index, /*[out, retval]*/ IInterfaceRecipient **pVal);
   STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);


private:

   std::shared_ptr<MA::Message> message_;
};

OBJECT_ENTRY_AUTO(__uuidof(Recipients), InterfaceRecipients)
