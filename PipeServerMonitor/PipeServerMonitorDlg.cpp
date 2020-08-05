
// PipeServerMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PipeServerMonitor.h"
#include "PipeServerMonitorDlg.h"
#include "afxdialogex.h"
#include "Logging.h"
#include "Config.h"
#include "GTOControlObject.h"
#include <fstream>
#include <istream>
#include <streambuf>

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


// CPipeServerMonitorDlg dialog



CPipeServerMonitorDlg::CPipeServerMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PIPESERVERMONITOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPipeServerMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_APP_HEALTH, m_ctlStatic_AppHealth);
    DDX_Control(pDX, IDC_EDIT_APP_STATUS, m_ctlAppStatus);
    DDX_Control(pDX, IDC_STATIC_PICTURE, m_ctlStaticPicture);
}

BEGIN_MESSAGE_MAP(CPipeServerMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(WM_USER_UPDATE_STATUS, &CPipeServerMonitorDlg::OnUpdateStatus )
    ON_MESSAGE(WM_USER_UPDATE_IMAGE, &CPipeServerMonitorDlg::OnUpdateImage)
    //ON_STN_CLICKED(IDC_STATIC_APP_HEALTH, &CPipeServerMonitorDlg::OnStnClickedStaticAppHealth)
    ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGE, &CPipeServerMonitorDlg::OnBnClickedButtonLoadImage)
END_MESSAGE_MAP()


// CPipeServerMonitorDlg message handlers

BOOL CPipeServerMonitorDlg::OnInitDialog()
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

    CLogging::Initialize();

    Trace(TRACE_INFO, _T("Ssb++ImageCollectDlg started"));

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
    bool bInitializationFailed = false;
    UpdateStatus(_T("Initializing pipe server..."));

    if (!InitializePipeServer())
    {
        Trace(TRACE_ERROR, _T("Failed to initialize pipe server."));
        UpdateStatus(_T("Failed to initialize pipe server."));
        bInitializationFailed = true;
    }
    else
    {
        Trace(TRACE_ERROR, _T("Successfully initialized pipe server."));
        UpdateStatus(_T("Successfully initialized pipe server."));
    }

    if (bInitializationFailed)
    {
        m_ctlStatic_AppHealth.SetWindowText(_T("Initialization Failed"));
    }
    else
    {
        m_ctlStatic_AppHealth.SetWindowText(_T("Initialization Successful"));
    }

    // UpdatePicture(_T("Test"), 0, 0);

    // m_ctlStaticPicture.ModifyStyleEx(0, WS_BORDER | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE);
    m_ctlStaticPicture.ModifyStyleEx(0, WS_BORDER | WS_VISIBLE | SS_CENTERIMAGE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPipeServerMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPipeServerMonitorDlg::OnPaint()
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
HCURSOR CPipeServerMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CPipeServerMonitorDlg::InitializePipeServer()
{
    int nRetry = 0;
    BOOL bGTOObject = FALSE;
    BOOL bSuccess = FALSE;
    do
    {
        //if (nRetry)
        //    g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Failed to Initialize PipeServer Connection. Retry:%d"), nRetry);
        DWORD inCount = ::GetTickCount();
        bGTOObject = CGTOControlObject::InitializeGTOCO(m_hWnd);
        DWORD outCount = ::GetTickCount();
        if (!bGTOObject)
        {
            nRetry++;
            int iDiff = 2000 - (outCount - inCount);
            // g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Time elapsed in trying for PipeServer connection is:%d"), outCount - inCount);
            if (iDiff>100)
            {
                CEvent*  hWaitEvent = new CEvent();
                hWaitEvent->ResetEvent();
                WaitForSingleObject(hWaitEvent->m_hObject, iDiff);
                //Sleep(iDiff);
            }
        }
        else
            break;

    } while (nRetry < 5);

    if (!bGTOObject)
    {
        //g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Failed to Initialize PipeServer Connection"));
        // GetDlgItem(IDC_SSB_COMM_STATUS)->SetWindowText(GetStatusString(SSB_COMM_STATUS_PIPESER_FAIL_INIT));
        bSuccess = FALSE;
    }
    else
    {
        bSuccess = TRUE;
    }

    return bSuccess;
}

BOOL CPipeServerMonitorDlg::UninitializePipeServer()
{
    BOOL bGTOObject = CGTOControlObject::UnintializeGTOCO();
    return bGTOObject;
}
LRESULT CPipeServerMonitorDlg::OnUpdateStatus(UINT wParam, LONG lParam)
{
    if (lParam)
    {
        std::auto_ptr<CString> msg(reinterpret_cast<CString*>(lParam));
        UpdateStatus(*msg);
    }
    return 0;
}

LRESULT CPipeServerMonitorDlg::OnUpdateImage(UINT wParam, LONG lParam)
{
    if (lParam)
    {
        std::auto_ptr<std::vector<uint8_t>> vecImage(reinterpret_cast<std::vector<uint8_t>*>(lParam));

        CImage oImage;

        Load_Image(*vecImage, oImage);
        UpdatePicture(oImage);
    }
    return 0;
}

bool CPipeServerMonitorDlg::Load_Image(const std::vector<uint8_t> &buf, CImage &image)
{
    UINT len = buf.size();
    HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, len);
    BYTE *pmem = (BYTE*)GlobalLock(hMem);
    memcpy(pmem, &buf[0], len);
    IStream    *stream = NULL;
    CreateStreamOnHGlobal(hMem, FALSE, &stream);
    image.Load(stream);
    stream->Release();
    GlobalUnlock(hMem);
    GlobalFree(hMem);
    return true;
}

void CPipeServerMonitorDlg::UpdateStatus(CString csStatusLine)
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

    m_ctlAppStatus.SetWindowTextW(csStatus);

    m_ctlAppStatus.LineScroll(m_ctlAppStatus.GetLineCount());
}




void CPipeServerMonitorDlg::UpdatePicture(CImage & a_cImage )
{
    CClientDC dc(this); // device context for painting
    CRect rect;
    m_ctlStaticPicture.GetWindowRect(&rect);
    ScreenToClient(&rect);
    rect.top += 5; rect.left += 5; rect.right -= 5; rect.bottom -= 5;
    // a_cImage.TransparentBlt(dc, rect, RGB(255, 255, 255));
    a_cImage.StretchBlt(dc, rect);

}


void CPipeServerMonitorDlg::UpdatePicture(CString filePath, int nFrameType, int nImageType)
{
    {	//Load and Display Image
        CClientDC dc(this); // device context for painting
        CRect rect;
        m_ctlStaticPicture.GetWindowRect(&rect);
        ScreenToClient(&rect);
        rect.top += 5; rect.left += 5; rect.right -= 5; rect.bottom -= 5;
        CImage ObjImage;

        // ObjImage.Load(_T("c:\\temp\\63205A96-2D98-4C79-88A1-EB9AB34F4275.jpeg"));
        ObjImage.Load(_T("c:\\temp\\test.jpg"));

        // ObjImage.TransparentBlt(dc, rect, RGB(255, 255, 255));
        ObjImage.StretchBlt(dc, rect);
    }
}



void CPipeServerMonitorDlg::OnBnClickedButtonLoadImage()
{
    UpdatePicture(_T("Test"), 0, 0);
    // TODO: Add your control notification handler code here
}
