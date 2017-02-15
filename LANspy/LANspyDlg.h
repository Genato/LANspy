// LANspyDlg.h : header file

#pragma once
#include "afxcmn.h"

typedef bool* BoolArray[4];

// CLANspyDlg dialog
class CLANspyDlg : public CDialogEx
{
// Construction
public:
	CLANspyDlg(CWnd* pParent = NULL);	// standard constructor

	enum { SAVE, LOAD, THISPC = 0, SUBNET, RANGE };

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
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void ThisPcInfo();
	afx_msg void RangeOfIpAddr();
	afx_msg void ThisPcSubnet();
	void CheckUncheckItem(int checkItemId, bool& check, int unCheckItemId[], BoolArray& unChecked, int itemMenuNum[]);

	CListCtrl listCtrlView;
	bool save, load, thisPcInfo, thisPcSubnet, rangeOfIpAddr;
};
