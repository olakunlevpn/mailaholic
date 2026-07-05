// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"
#include "../Common/Scripting/Result.h"

// InterfaceResult

class ATL_NO_VTABLE InterfaceResult : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceResult, &CLSID_Result>,
	public IDispatchImpl<IInterfaceResult, &IID_IInterfaceResult, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	InterfaceResult()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACERESULT)


BEGIN_COM_MAP(InterfaceResult)
	COM_INTERFACE_ENTRY(IInterfaceResult)
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

   void AttachItem(std::shared_ptr<MA::Result> pResult);

   STDMETHOD(get_Value)(/*[out, retval]*/ long *pVal);
   STDMETHOD(put_Value)(/*[in]*/ long newVal);
   STDMETHOD(get_Message)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(put_Message)(/*[in]*/ BSTR newVal);
   STDMETHOD(get_Parameter)(/*[out, retval]*/ long *pVal);
   STDMETHOD(put_Parameter)(/*[in]*/ long newVal);

private:
   std::shared_ptr<MA::Result> result_;

};

OBJECT_ENTRY_AUTO(__uuidof(Result), InterfaceResult)
