// IPaddresses.h : Implementation of the CIPaddresses class



// CIPaddresses implementation

// code generated on 11. veljače 2017., 14:07

#include "stdafx.h"
#include "IPaddresses.h"
IMPLEMENT_DYNAMIC(CIPaddresses, CRecordset)

CIPaddresses::CIPaddresses(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_IPaddress = "";
	m_Hostname = "";
	m_MACaddress = "";
	m_nFields = 3;
	m_nDefaultType = dynaset;
}

CString CIPaddresses::GetDefaultConnect()
{
	return _T("DSN=NWP;Trusted_Connection=Yes;APP=Microsoft\x00ae Visual Studio\x00ae 2015;WSID=RENATO;DATABASE=LANspy");
}

CString CIPaddresses::GetDefaultSQL()
{
	return _T("[dbo].[IPaddresses]");
}

void CIPaddresses::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// Macros such as RFX_Text() and RFX_Int() are dependent on the
// type of the member variable, not the type of the field in the database.
// ODBC will try to automatically convert the column value to the requested type
	RFX_Text(pFX, _T("[IPaddress]"), m_IPaddress);
	RFX_Text(pFX, _T("[Hostname]"), m_Hostname);
	RFX_Text(pFX, _T("[MACaddress]"), m_MACaddress);

}
/////////////////////////////////////////////////////////////////////////////
// CIPaddresses diagnostics

#ifdef _DEBUG
void CIPaddresses::AssertValid() const
{
	CRecordset::AssertValid();
}

void CIPaddresses::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


