// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"

#include "../Common/BO/Attachments.h"
// InterfaceAttachments

class ATL_NO_VTABLE InterfaceAttachments : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceAttachments, &CLSID_Attachments>,
	public IDispatchImpl<IInterfaceAttachments, &IID_IInterfaceAttachments, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator,
   public ISupportErrorInfo
{
public:
	InterfaceAttachments()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEATTACHMENTS)


BEGIN_COM_MAP(InterfaceAttachments)
	COM_INTERFACE_ENTRY(IInterfaceAttachments)
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

   STDMETHOD(get_Item)(/*[in]*/ long Index, /*[out, retval]*/ IInterfaceAttachment* *pVal);
   STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
   STDMETHOD(Clear)();
   STDMETHOD(Add)(BSTR sFilename);

   void Attach(std::shared_ptr<MA::Attachments> pAttachments);

private:

   std::shared_ptr<MA::Attachments> attachments_;

};

OBJECT_ENTRY_AUTO(__uuidof(Attachments), InterfaceAttachments)
