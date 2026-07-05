// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once
#include "../Mailaholic/resource.h"       // main symbols

#include "../Mailaholic/Mailaholic.h"
#include "COMCollection.h"

#include "../common/BO/Alias.h"

namespace MA 
{ 
   class Aliases; 
}

class ATL_NO_VTABLE InterfaceAlias : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<InterfaceAlias, &CLSID_Alias>,
	public IDispatchImpl<IInterfaceAlias, &IID_IInterfaceAlias, &LIBID_hMailServer, /*wMajor =*/ 1, /*wMinor =*/ 0>,
   public COMCollectionItem<MA::Alias, MA::Aliases>,
   public MA::COMAuthenticator,
   public ISupportErrorInfo
{
public:
   InterfaceAlias()
   {
#ifdef _DEBUG
      InterlockedIncrement(&counter);
#endif
   }

   ~InterfaceAlias()
   {
#ifdef _DEBUG
      InterlockedDecrement(&counter);
#endif
   }

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACEALIAS)


BEGIN_COM_MAP(InterfaceAlias)
	COM_INTERFACE_ENTRY(IInterfaceAlias)
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

   HRESULT STDMETHODCALLTYPE get_ID(LONG* pVal)
   {
      *pVal = (long) object_->GetID();

      return S_OK;
   }

   STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

   STDMETHOD(Save)();
   STDMETHOD(Delete)();
   STDMETHOD(get_Value)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(put_Value)(/*[in]*/ BSTR newVal);
   STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
   STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
   STDMETHOD(get_DomainID)(/*[out, retval]*/ long *pVal);
   STDMETHOD(put_DomainID)(LONG newVal);
   STDMETHOD(get_Active)(/*[out, retval]*/ VARIANT_BOOL *pVal);
   STDMETHOD(put_Active)(/*[in]*/ VARIANT_BOOL newVal);

private:

#ifdef _DEBUG
   static long counter;
#endif

};

OBJECT_ENTRY_AUTO(__uuidof(Alias), InterfaceAlias)
