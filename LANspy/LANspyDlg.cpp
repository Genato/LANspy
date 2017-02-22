// LANspyDlg.cpp : implementation file

#include "stdafx.h"

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLANspyDlg dialog



CLANspyDlg::CLANspyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LANSPY_DIALOG, pParent) 
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLANspyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, listCtrlView);
}

BEGIN_MESSAGE_MAP(CLANspyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLANspyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLANspyDlg message handlers

BOOL CLANspyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	RECT lpRect;

	listCtrlView.GetWindowRect(&lpRect);

	// TODO: Add extra initialization here
	listCtrlView.InsertColumn(0, _T("IPaddress"), LVCFMT_LEFT, (lpRect.right - lpRect.left) / 3);
	listCtrlView.InsertColumn(1, _T("Hostname"), LVCFMT_LEFT, (lpRect.right - lpRect.left) / 3);
	listCtrlView.InsertColumn(2, _T("MACaddress"), LVCFMT_LEFT, ((lpRect.right - lpRect.left) / 3) - 5);

	CheckRadioButton(IDC_RADIO1, IDC_RADIO5, IDC_RADIO1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLANspyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLANspyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLANspyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//
//Custom Events
//
void CLANspyDlg::OnBnClickedOk()
{
	ViewModelLogic viewModelLogic;

	viewModelLogic.Load(listCtrlView);
}

//CMenu* cMenu = this->GetMenu();

//save = !save;
//cMenu->CheckMenuItem(ID_SAVE, save ? MF_CHECKED : MF_UNCHECKED);
//if (cMenu->GetMenuState(ID_LOAD, 1) == MF_CHECKED)
//{
//	cMenu->CheckMenuItem(ID_LOAD, MF_UNCHECKED);
//	load = !load;
//}s
