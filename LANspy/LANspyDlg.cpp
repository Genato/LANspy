#include "stdafx.h"
#include "LANspyDlg.h"

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
	DDX_Control(pDX, IDC_PROGRESS1, progressBar);
}

BEGIN_MESSAGE_MAP(CLANspyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLANspyDlg::OnBnClickedOk)
	ON_COMMAND(IDC_THISPCINFO, &CLANspyDlg::OnRadioBtnChange)
	ON_COMMAND(IDC_THISPCSUBNET, &CLANspyDlg::OnRadioBtnChange)
	ON_COMMAND(IDC_RANGEOFIPADDR, &CLANspyDlg::OnRadioBtnChange)
	ON_COMMAND(IDC_SAVE, &CLANspyDlg::OnRadioBtnChange)
	ON_COMMAND(IDC_LOAD, &CLANspyDlg::OnRadioBtnChange)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////
// CLANspyDlg message handlers
//////////////////////////////

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
	CString ipAddr, hostName, macAddr;

	listCtrlView.GetWindowRect(&lpRect);

	if (!ipAddr.LoadString(LISTCNTRLIPADDR))
		ipAddr = "";
	listCtrlView.InsertColumn(0, ipAddr, LVCFMT_LEFT, (lpRect.right - lpRect.left) / 3);
	if (!hostName.LoadString(LISTCNTRLIHOSTNAME))
		hostName = "";
	listCtrlView.InsertColumn(1, hostName, LVCFMT_LEFT, (lpRect.right - lpRect.left) / 3);
	if (!macAddr.LoadString(LISTCNTRLIPADDRMACADDR))
		macAddr = "";
	listCtrlView.InsertColumn(2, macAddr, LVCFMT_LEFT, ((lpRect.right - lpRect.left) / 3) - 5);

	CheckRadioButton(IDC_THISPCINFO, IDC_LOAD, IDC_THISPCINFO);

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

///////////////////////////////////////////////////////////////////////////////////////
//Custom message handlers
///////////////////////////////////////////////////////////////////////////////////////

//Function that calls ViewModelLogic to performe some logic based on options that user choose
void CLANspyDlg::OnBnClickedOk()
{
	this->SetTimer(ID_TIMER, 500, 0);
	progressBar.SetPos(0);
	this->progressBar.SetStep(10);

	if (IsDlgButtonChecked(IDC_THISPCINFO) == BST_CHECKED)
	{
		viewModelLogic.Search(IDC_THISPCINFO);
	}
	else if (IsDlgButtonChecked(IDC_THISPCSUBNET) == BST_CHECKED)
	{
		viewModelLogic.Search(IDC_THISPCSUBNET);
	}
	else if (IsDlgButtonChecked(IDC_RANGEOFIPADDR) == BST_CHECKED)
	{
		CIPAddressCtrl* cIpCntrl;
		DWORD startDwAddress;
		DWORD endDwAddress;

		cIpCntrl = (CIPAddressCtrl*)(GetDlgItem(IDC_IPADDRESS1));
		cIpCntrl->GetAddress(startDwAddress);
		cIpCntrl = (CIPAddressCtrl*)(GetDlgItem(IDC_IPADDRESS2));
		cIpCntrl->GetAddress(endDwAddress);

		viewModelLogic.Search(startDwAddress, endDwAddress, IDC_RANGEOFIPADDR);
	}
	else if (IsDlgButtonChecked(IDC_SAVE) == BST_CHECKED)
	{
		CString tmpStr1, tmpStr2;

		if(listCtrlView.GetItemCount())
			viewModelLogic.Save(listCtrlView);
		else
		{
			if (!tmpStr1.LoadString(IDC_SAVE))
				tmpStr1 = "";
			if (!tmpStr2.LoadString(MESSAGEBOXSAVE))
				tmpStr2 = "";

			MessageBox(tmpStr2, tmpStr1, MB_ICONASTERISK);
		}
	}
	else if (IsDlgButtonChecked(IDC_LOAD) == BST_CHECKED)
	{
		viewModelLogic.Load(listCtrlView);
	}
}

//Function that disable/enable controls
afx_msg void CLANspyDlg::OnRadioBtnChange()
{
	CWnd* cWnd;

	cWnd = GetDlgItem(IDC_IPADDRESS1);
	cWnd->EnableWindow(0);
	cWnd = GetDlgItem(IDC_IPADDRESS2);
	cWnd->EnableWindow(0);

	if (IsDlgButtonChecked(IDC_THISPCINFO) == BST_CHECKED)
	{
		CWnd* cWnd;
		CString tmpStr;

		cWnd = GetDlgItem(IDOK);
		if(!tmpStr.LoadString(IDC_THISPCINFO))
			tmpStr = "";

		cWnd->SetWindowText(tmpStr);
	}
	else if (IsDlgButtonChecked(IDC_THISPCSUBNET) == BST_CHECKED)
	{
		CWnd* cWnd;
		CString tmpStr;

		cWnd = GetDlgItem(IDOK);
		if (!tmpStr.LoadString(IDC_THISPCSUBNET))
			tmpStr = "";

		cWnd->SetWindowText(tmpStr);
	}
	else if (IsDlgButtonChecked(IDC_RANGEOFIPADDR) == BST_CHECKED)
	{
		CWnd* cWnd;
		CString tmpStr;

		cWnd = GetDlgItem(IDOK);
		if (!tmpStr.LoadString(IDC_RANGEOFIPADDR))
			tmpStr = "";

		cWnd->SetWindowText(tmpStr);
		cWnd = GetDlgItem(IDC_IPADDRESS1);
		cWnd->EnableWindow(1);
		cWnd = GetDlgItem(IDC_IPADDRESS2);
		cWnd->EnableWindow(1);
	}
	else if (IsDlgButtonChecked(IDC_SAVE) == BST_CHECKED)
	{
		CWnd* cWnd;
		CString tmpStr;

		cWnd = GetDlgItem(IDOK);
		if (!tmpStr.LoadString(IDC_SAVE))
			tmpStr = "";

		cWnd->SetWindowText(tmpStr);
	}
	else if (IsDlgButtonChecked(IDC_LOAD) == BST_CHECKED)
	{
		CWnd* cWnd;
		CString tmpStr;

		cWnd = GetDlgItem(IDOK);
		if (!tmpStr.LoadString(IDC_LOAD))
			tmpStr = "";

		cWnd->SetWindowText(tmpStr);
	}
}


void CLANspyDlg::OnTimer(UINT_PTR nIDEvent)
{
	srand(time(NULL));

	if (!viewModelLogic.GetTraverseResult(listCtrlView))
	{
		progressBar.SetPos((rand() % 100) + progressBar.GetPos());
	}
	else
	{
		progressBar.SetPos(100);
		this->KillTimer(ID_TIMER);
	}

	CDialogEx::OnTimer(nIDEvent);
}
