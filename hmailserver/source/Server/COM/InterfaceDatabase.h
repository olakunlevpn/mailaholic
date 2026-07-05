// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"

namespace MA
{
   class IniFileSettings;
}

// InterfaceDatabase

class ATL_NO_VTABLE InterfaceDatabase : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceDatabase, &CLSID_Database>,
   public ISupportErrorInfo,
	public IDispatchImpl<IInterfaceDatabase, &IID_IInterfaceDatabase, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public MA::COMAuthenticator
{
public:
	InterfaceDatabase();
   ~InterfaceDatabase();

   bool LoadSettings();

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEDATABASE)


BEGIN_COM_MAP(InterfaceDatabase)
	COM_INTERFACE_ENTRY(IInterfaceDatabase)
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

   STDMETHOD(ExecuteSQL)(BSTR sSQLStatement);
   STDMETHOD(ExecuteSQLWithReturn)(BSTR sSQLStatement, long *uniqueID);
   STDMETHOD(ExecuteSQLScript)(BSTR sFilename);

   STDMETHOD(BeginTransaction)();
   STDMETHOD(CommitTransaction)();
   STDMETHOD(RollbackTransaction)();

   STDMETHOD(get_DatabaseType)(/*[out, retval]*/ eDBtype *pVal);
   STDMETHOD(get_CurrentVersion)(/*[out, retval]*/ long *pVal);
   STDMETHOD(get_RequiredVersion)(/*[out, retval]*/ long *pVal);
   STDMETHOD(get_RequiresUpgrade)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(get_IsConnected)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(get_DatabaseExists)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(get_ServerName)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(get_DatabaseName)(/*[out, retval]*/ BSTR *pVal);

   STDMETHOD(UtilGetFileNameByMessageID)(hyper lMessageID, BSTR *Output);
   
   STDMETHOD(CreateExternalDatabase)(eDBtype ServerType, BSTR ServerName, long lPort, BSTR DatabaseName, BSTR Username, BSTR Password);
   STDMETHOD(CreateInternalDatabase)();
   STDMETHOD(SetDefaultDatabase)(eDBtype ServerType, BSTR ServerName, long lPort, BSTR DatabaseName, BSTR Username, BSTR Password);

   STDMETHOD(EnsurePrerequisites)(long DBVersion);
   
private:
   HRESULT EnsureDatabaseConnectivity_();

   MA::Configuration *config_;
   MA::IniFileSettings *ini_file_settings_;
   std::shared_ptr<MA::DatabaseConnectionManager> db_manager_;

   std::shared_ptr<MA::DALConnection> conn_;
};

OBJECT_ENTRY_AUTO(__uuidof(Database), InterfaceDatabase)
