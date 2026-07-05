// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"


// InterfaceDeliveryQueue

class ATL_NO_VTABLE InterfaceDeliveryQueue : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceDeliveryQueue, &CLSID_DeliveryQueue>,
	public IDispatchImpl<IInterfaceDeliveryQueue, &IID_IInterfaceDeliveryQueue, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceDeliveryQueue()
	{
	}

   void SetAuthentication(std::shared_ptr<MA::COMAuthentication> pAuthentication);

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEDELIVERYQUEUE)


BEGIN_COM_MAP(InterfaceDeliveryQueue)
	COM_INTERFACE_ENTRY(IInterfaceDeliveryQueue)
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
   STDMETHOD(Clear());
   STDMETHOD(ResetDeliveryTime)(hyper iMessageID);
   STDMETHOD(Remove)(hyper iMessageID);
   STDMETHOD(StartDelivery());

private:

   std::shared_ptr<MA::COMAuthentication> authentication_;
};

OBJECT_ENTRY_AUTO(__uuidof(DeliveryQueue), InterfaceDeliveryQueue)
