// IPaddresses.h : Declaration of the CIPaddresses

#pragma once

class CIPaddresses : public CRecordset
{
public:
	CIPaddresses(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CIPaddresses)

// Field/Param Data

	CString	m_IPaddress;
	CString	m_Hostname;
	CString	m_MACaddress;
	CString	m_SSID;

// Overrides
	// Wizard generated virtual function overrides
	public:
	virtual CString GetDefaultConnect();	// Default connection string

	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


