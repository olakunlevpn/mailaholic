// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"

#include "COMCollection.h"

namespace MA
{
   class DistributionListRecipient;
   class DistributionListRecipients;
}

class ATL_NO_VTABLE InterfaceDistributionListRecipient : 
   public COMCollectionItem<MA::DistributionListRecipient, MA::DistributionListRecipients>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceDistributionListRecipient, &CLSID_DistributionListRecipient>,
	public IDispatchImpl<IInterfaceDistributionListRecipient, &IID_IInterfaceDistributionListRecipient, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator,
   public ISupportErrorInfo
{
public:
	InterfaceDistributionListRecipient()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEDISTRIBUTIONLISTRECIPIENT)


BEGIN_COM_MAP(InterfaceDistributionListRecipient)
	COM_INTERFACE_ENTRY(IInterfaceDistributionListRecipient)
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

   STDMETHOD(get_ID)(/*[out, retval]*/ long *pVal);
   STDMETHOD(put_ID)(/*[in]*/ long newVal);
   STDMETHOD(get_RecipientAddress)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(put_RecipientAddress)(/*[in]*/ BSTR newVal);
   STDMETHOD(Delete)();
   STDMETHOD(Save)();

private:

};

OBJECT_ENTRY_AUTO(__uuidof(DistributionListRecipient), InterfaceDistributionListRecipient)
