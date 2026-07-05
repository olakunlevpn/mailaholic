// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "../Mailaholic/resource.h"       // main symbols
#include "../Mailaholic/Mailaholic.h"

namespace MA
{
   class Configuration;
   class IniFileSettings;
}

class ATL_NO_VTABLE InterfaceBackupSettings : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceBackupSettings, &CLSID_BackupSettings>,
	public IDispatchImpl<IInterfaceBackupSettings, &IID_IInterfaceBackupSettings, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
   InterfaceBackupSettings();


   bool LoadSettings();

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEBACKUPSETTINGS)


BEGIN_COM_MAP(InterfaceBackupSettings)
	COM_INTERFACE_ENTRY(IInterfaceBackupSettings)
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
   STDMETHOD(get_Destination)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(put_Destination)(/*[in]*/ BSTR newVal);

   STDMETHOD(get_LogFile)(/*[out, retval]*/ BSTR *pVal);

   STDMETHOD(get_BackupSettings)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(put_BackupSettings)(/*[in]*/ VARIANT_BOOL newVal);
   STDMETHOD(get_BackupDomains)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(put_BackupDomains)(/*[in]*/ VARIANT_BOOL newVal);
   STDMETHOD(get_BackupMessages)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(put_BackupMessages)(/*[in]*/ VARIANT_BOOL newVal);
   STDMETHOD(get_CompressDestinationFiles)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(put_CompressDestinationFiles)(/*[in]*/ VARIANT_BOOL newVal);

private:

   MA::Configuration* config_;
   MA::IniFileSettings* ini_file_settings_;

};

OBJECT_ENTRY_AUTO(__uuidof(BackupSettings), InterfaceBackupSettings)
