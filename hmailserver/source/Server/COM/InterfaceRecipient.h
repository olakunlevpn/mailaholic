// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"
#include "../Common/BO/MessageRecipient.h"

class ATL_NO_VTABLE InterfaceRecipient : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceRecipient, &CLSID_Recipient>,
	public IDispatchImpl<IInterfaceRecipient, &IID_IInterfaceRecipient, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceRecipient()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACERECIPIENT)


BEGIN_COM_MAP(InterfaceRecipient)
	COM_INTERFACE_ENTRY(IInterfaceRecipient)
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

   void AttachItem(std::shared_ptr<MA::MessageRecipient> pRecipient);

public:

   STDMETHOD(get_Address)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(get_IsLocalUser)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(get_OriginalAddress)(/*[out, retval]*/ BSTR *pVal);

private:

   std::shared_ptr<MA::MessageRecipient> object_;
};

OBJECT_ENTRY_AUTO(__uuidof(Recipient), InterfaceRecipient)
