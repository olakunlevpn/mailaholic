// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"
#include "../common/bo/attachment.h"

class ATL_NO_VTABLE InterfaceAttachment : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceAttachment, &CLSID_Attachment>,
	public IDispatchImpl<IInterfaceAttachment, &IID_IInterfaceAttachment, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceAttachment()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEATTACHMENT)


BEGIN_COM_MAP(InterfaceAttachment)
	COM_INTERFACE_ENTRY(IInterfaceAttachment)
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
   STDMETHOD(get_Size)(/*[out, retval]*/ long *pVal);
   STDMETHOD(get_Filename)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(SaveAs)(/*[out, retval]*/ BSTR pVal);
   STDMETHOD(Delete)();

   void Attach(std::shared_ptr<MA::Attachment> pAttachment);

private:

   std::shared_ptr<MA::Attachment> attachment_;

};

OBJECT_ENTRY_AUTO(__uuidof(Attachment), InterfaceAttachment)
