// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"


// InterfaceScripting

class ATL_NO_VTABLE InterfaceScripting : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceScripting, &CLSID_Scripting>,
	public IDispatchImpl<IInterfaceScripting, &IID_IInterfaceScripting, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
   InterfaceScripting();

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACESCRIPTING)


BEGIN_COM_MAP(InterfaceScripting)
	COM_INTERFACE_ENTRY(IInterfaceScripting)
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

   bool LoadSettings();

   STDMETHOD(get_Enabled)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(put_Enabled)(/*[in]*/ VARIANT_BOOL newVal);	

   STDMETHOD(get_Language)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(put_Language)(/*[in]*/ BSTR newVal);

   STDMETHOD(Reload)(void);
   STDMETHOD(CheckSyntax)(BSTR *pVal);

   STDMETHOD(get_Directory)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(get_CurrentScriptFile)(/*[out, retval]*/ BSTR *pVal);
      

private:

   MA::Configuration *config_;
   MA::IniFileSettings *ini_file_settings_;
};

OBJECT_ENTRY_AUTO(__uuidof(Scripting), InterfaceScripting)
