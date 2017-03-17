#pragma once
#include "afxcmn.h"
#include "ViewModelLogic.h"

// CLANspyDlg dialog
class CLANspyDlg : public CDialogEx
{
// Construction
public:
	CListCtrl listCtrlView;
	ViewModelLogic viewModelLogic;
	CProgressCtrl progressBar;

	CLANspyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LANSPY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnRadioBtnChange();	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
