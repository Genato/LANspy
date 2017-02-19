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
	: CDialogEx(IDD_LANSPY_DIALOG, pParent), 
	save(false),
	load(false),
	thisPcInfo(false), 
	thisPcSubnet(false), 
	rangeOfIpAddr(false)
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
	ON_COMMAND(ID_SAVE, &CLANspyDlg::OnSave)
	ON_COMMAND(ID_LOAD, &CLANspyDlg::OnLoad)
	ON_COMMAND(ID_OPTIONS_THISPCINFO, &CLANspyDlg::ThisPcInfo)
	ON_COMMAND(ID_OPTIONS_RANGEOFIPADDRESSES, &CLANspyDlg::RangeOfIpAddr)
	ON_COMMAND(ID_OPTIONS_THISPCSUBNET, &CLANspyDlg::ThisPcSubnet)
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

	// TODO: Add extra initialization here
	listCtrlView.InsertColumn(0, _T("IPaddress"), LVCFMT_LEFT, 100);
	listCtrlView.InsertColumn(1, _T("Hostname"), LVCFMT_LEFT, 100);
	listCtrlView.InsertColumn(2, _T("MACaddress"), LVCFMT_LEFT, 100);

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

//Custom Events
void CLANspyDlg::OnBnClickedOk()
{
	ViewModelLogic viewModelLogic;

	viewModelLogic.Search(listCtrlView);
}

void CLANspyDlg::OnSave()
{
	//CMenu* cMenu = this->GetMenu();
	
	//save = !save;
	//cMenu->CheckMenuItem(ID_SAVE, save ? MF_CHECKED : MF_UNCHECKED);
	//if (cMenu->GetMenuState(ID_LOAD, 1) == MF_CHECKED)
	//{
	//	cMenu->CheckMenuItem(ID_LOAD, MF_UNCHECKED);
	//	load = !load;
	//}s

	int unCheckItemId[] = { ID_LOAD, ID_OPTIONS_THISPCINFO, ID_OPTIONS_THISPCSUBNET, ID_OPTIONS_RANGEOFIPADDRESSES };
	BoolArray unChecked = { &load, &thisPcInfo, &thisPcSubnet, &rangeOfIpAddr };
	int itemMenuNum[] = {LOAD, THISPC, SUBNET, RANGE};

	CheckUncheckItem(ID_SAVE, save, unCheckItemId, unChecked, itemMenuNum);
}

void CLANspyDlg::OnLoad()
{
	//CMenu* cMenu = this->GetMenu();

	//load = !load;
	//cMenu->CheckMenuItem(ID_LOAD, load ? MF_CHECKED : MF_UNCHECKED);
	//if (cMenu->GetMenuState(ID_SAVE, 0) == MF_CHECKED)
	//{
	//	cMenu->CheckMenuItem(ID_SAVE, MF_UNCHECKED);
	//	save = !save;
	//}		

	int unCheckItemId[] = { ID_SAVE, ID_OPTIONS_THISPCINFO, ID_OPTIONS_THISPCSUBNET, ID_OPTIONS_RANGEOFIPADDRESSES };
	BoolArray unChecked = { &save, &thisPcInfo, &thisPcSubnet, &rangeOfIpAddr };
	int itemMenuNum[] = { SAVE, THISPC, SUBNET, RANGE };

	CheckUncheckItem(ID_LOAD, load, unCheckItemId, unChecked, itemMenuNum);
}

void CLANspyDlg::ThisPcInfo()
{
	int unCheckItemId[] = { ID_SAVE, ID_LOAD, ID_OPTIONS_THISPCSUBNET, ID_OPTIONS_RANGEOFIPADDRESSES };
	BoolArray unChecked = { &save, &load, &thisPcSubnet, &rangeOfIpAddr };
	int itemMenuNum[] = { SAVE, LOAD, SUBNET, RANGE };

	CheckUncheckItem(ID_OPTIONS_THISPCINFO, thisPcInfo, unCheckItemId, unChecked, itemMenuNum);
}


void CLANspyDlg::ThisPcSubnet()
{
	int unCheckItemId[] = { ID_SAVE, ID_LOAD, ID_OPTIONS_THISPCINFO, ID_OPTIONS_RANGEOFIPADDRESSES };
	BoolArray unChecked = { &save, &load, &thisPcInfo, &rangeOfIpAddr };
	int itemMenuNum[] = { SAVE, LOAD, THISPC, RANGE };

	CheckUncheckItem(ID_OPTIONS_THISPCSUBNET, thisPcSubnet, unCheckItemId, unChecked, itemMenuNum);
}

void CLANspyDlg::RangeOfIpAddr()
{
	int unCheckItemId[] = { ID_SAVE, ID_LOAD, ID_OPTIONS_THISPCINFO, ID_OPTIONS_THISPCSUBNET };
	BoolArray unChecked = { &save, &load, &thisPcInfo, &thisPcSubnet };
	int itemMenuNum[] = { SAVE, LOAD, THISPC, SUBNET };
	
	CheckUncheckItem(ID_OPTIONS_RANGEOFIPADDRESSES, rangeOfIpAddr, unCheckItemId, unChecked, itemMenuNum);
}


//Helper methods
void CLANspyDlg::CheckUncheckItem(int checkItemId, bool& check, int unCheckItemId[], BoolArray& unChecked, int itemMenuNum[])
{
	CMenu* cMenu = this->GetMenu();

	check = !check;
	cMenu->CheckMenuItem(checkItemId, check ? MF_CHECKED : MF_UNCHECKED);

	for (int i = 0; i < sizeof(unChecked)/sizeof(unChecked[0]); ++i)
	{
		if (cMenu->GetMenuState(unCheckItemId[i], itemMenuNum[i]) == MF_CHECKED)
		{
			cMenu->CheckMenuItem(unCheckItemId[i], MF_UNCHECKED);
			*unChecked[i] = (!*unChecked[i]);
		}
	}
}
