
// SSBPPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SSBPP.h"
#include "SSBPPDlg.h"
#include "afxdialogex.h"
#include "Logging.h"
#include "Config.h"
#include "PipeServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CSSBPPDlg dialog



CSSBPPDlg::CSSBPPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SSBPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSSBPPDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_APP_STATUS, m_ctlEdit_AppStatus);
}

BEGIN_MESSAGE_MAP(CSSBPPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WM_USER_UPDATE_STATUS, &CSSBPPDlg::OnUpdateStatus)
    ON_BN_CLICKED(ID_EXIT, &CSSBPPDlg::OnBnClickedExit)
    ON_BN_CLICKED(IDCANCEL, &CSSBPPDlg::OnBnClickedCancel)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CSSBPPDlg message handlers

BOOL CSSBPPDlg::OnInitDialog()
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
    bool bInitializationFailed = false;

    CLogging::Initialize();

    Trace(TRACE_INFO, _T("SsbPP started"));

    UpdateStatus(_T("Reading configuration..."));

    if (!InitializeConfig())
    {
        Trace(TRACE_WARNING, _T("Failed to open configuration file. Falling back to defaults"));
        UpdateStatus(_T("Error...Failed to read configuration. Falling back to defaults."));
    }
    else
    {
        UpdateStatus(_T("Configuration loaded successfully."));
    }

    m_oMessageHandler.Open(this->m_hWnd);

    UpdateStatus(_T("Initializing pipe server..."));

    if (!oPipeServer.Open(m_hWnd, &m_oMessageHandler) )
    {
        Trace(TRACE_ERROR, _T("Failed to initialize pipe server."));
        UpdateStatus(_T("Failed to initialize pipe server."));
        bInitializationFailed = true;
    }
    else
    {
        if (!oPipeServer.Connect())
        {
            Trace(TRACE_ERROR, _T("Failed to connect to pipe server."));
            UpdateStatus(_T("Failed to connect to pipe server."));
            bInitializationFailed = true;
        }
        else
        {
            Trace(TRACE_INFO, _T("Pipe server connection successful."));
            UpdateStatus(_T("Pipe server connection successful."));
        }
    }
    SetTimer(1,3000, NULL);

    /*
    */
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSSBPPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSSBPPDlg::OnPaint()
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
HCURSOR CSSBPPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSSBPPDlg::UpdateStatus(CString csStatusLine)
{
    m_vStatus.push_back(csStatusLine);

    if (m_vStatus.size() > 10)
    {
        m_vStatus.erase(m_vStatus.begin(), m_vStatus.begin() + 1);
    }

    size_t nSize = m_vStatus.size();
    CString csStatus;

    for (size_t i = 0; i < nSize; i++)
    {
        csStatus += m_vStatus[i] + _T("\r\n");
    }

    m_ctlEdit_AppStatus.SetWindowTextW(csStatus);

    m_ctlEdit_AppStatus.LineScroll(m_ctlEdit_AppStatus.GetLineCount());
}

LRESULT CSSBPPDlg::OnUpdateStatus(UINT wParam, LONG lParam)
{
    if (lParam)
    {
        std::auto_ptr<CString> msg(reinterpret_cast<CString*>(lParam));
        UpdateStatus(*msg);
    }
    return 0;
}


void CSSBPPDlg::OnBnClickedExit()
{
    // oPipeServer.Close();
    OnOK();
    // TODO: Add your control notification handler code here
}


void CSSBPPDlg::OnBnClickedCancel()
{
    CLogging::UnInitialize();
    DestroyConfig();
    oPipeServer.Close();
    // TODO: Add your control notification handler code here
    CDialogEx::OnCancel();
}


void CSSBPPDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    if (!oPipeServer.IsConnected())
    {
        if (oPipeServer.Connect())
        {
            Trace(TRACE_INFO, _T("Pipe server connection successful."));
            UpdateStatus(_T("Pipe server connection successful."));
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}
