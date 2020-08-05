
// Ssb++ImageCollectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ssb++ImageCollect.h"
#include "Ssb++ImageCollectDlg.h"
#include "afxdialogex.h"
#include "Logging.h"
#include "Config.h"
#include "DeviceConnector.h"
#include "GTOControlObject.h"

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


// CSsbImageCollectDlg dialog



CSsbImageCollectDlg::CSsbImageCollectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SSBIMAGECOLLECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSsbImageCollectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_APP_HEALTH, m_ctlStatic_AppHealth);
}

BEGIN_MESSAGE_MAP(CSsbImageCollectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CSsbImageCollectDlg::OnBnClickedOk)
    ON_MESSAGE(WM_USER_UPDATE_STATUS, &CSsbImageCollectDlg::OnUpdateStatus )
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSsbImageCollectDlg, CDialogEx)
    ON_EVENT(CSsbImageCollectDlg, IDC_OPOSIMAGERSCANNER, 1 /* DataEvent */, OnDataEvent, VTS_I4)
    ON_EVENT(CSsbImageCollectDlg, IDC_OPOSIMAGERSCANNER, 2 /* DirectIOEvent */, OnDirectIOEvent, VTS_I4 VTS_PI4 VTS_PBSTR)
    ON_EVENT(CSsbImageCollectDlg, IDC_OPOSIMAGERSCANNER, 3 /* ErrorEvent */, OnErrorEvent, VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
    ON_EVENT(CSsbImageCollectDlg, IDC_OPOSIMAGERSCANNER, 4 /* OutputComplete */, OnOutputCompleteEvent, VTS_I4)
    ON_EVENT(CSsbImageCollectDlg, IDC_OPOSIMAGERSCANNER, 5 /* StatusUpdateEvent */, OnStatusUpdateEvent, VTS_I4)
END_EVENTSINK_MAP()

// CSsbImageCollectDlg message handlers

BOOL CSsbImageCollectDlg::OnInitDialog()
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

    CDeviceConnector* pDeviceConn = NULL;

    CString csStatus;

    pDeviceConn = CDeviceConnector::GetInstance();

    CString csProfile;

    UpdateStatus(_T("Initializing camera hardware..."));

    bool bInitializationFailed = false;

    bool bSucess = pDeviceConn->InitializeDM(*this, csStatus, GetConfig()->csScannerProfile);

    if (bSucess)
    {
        Trace(TRACE_INFO, _T("Successfully initialized camera hardware."));
        UpdateStatus(_T("Successfully initialized camera hardware."));
    }
    else
    {
        Trace(TRACE_ERROR, _T("Failed to initialize camera hardware."));
        UpdateStatus(_T("Failed to initialize camera hardware."));
        bInitializationFailed = true;
    }

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


    //GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Test1\nTest2\nTest3\nTest4Test1\nTest2\nTest3\nTest4Test1\nTest2\nTest3\nTest4Test1\nTest2\nTest3\nTest4Test1\nTest2\nTest3\nTest4"));
    //GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Test2"));
    //GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Test3"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSsbImageCollectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSsbImageCollectDlg::OnPaint()
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
HCURSOR CSsbImageCollectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSsbImageCollectDlg::OnBnClickedOk()
{
    Trace(TRACE_INFO, _T("Ssb++ImageCollectDlg closed"));

    CDeviceConnector::GetInstance()->ReleaseDevice();

    UninitializePipeServer();

    CLogging::UnInitialize();

    if (GetConfig())
    {
        DestroyConfig();
    }

    // TODO: Add your control notification handler code here
    CDialogEx::OnOK();
}

BOOL CSsbImageCollectDlg::InitializePipeServer()
{
    int nRetry = 0;
    BOOL bGTOObject = FALSE;
    BOOL bSuccess = FALSE;
    do
    {
        //if (nRetry)
        //    g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Failed to Initialize PipeServer Connection. Retry:%d"), nRetry);
        DWORD inCount = ::GetTickCount();
        bGTOObject = CGTOControlObject::InitializeGTOCO(*this);
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

BOOL CSsbImageCollectDlg::UninitializePipeServer()
{
    BOOL bGTOObject = CGTOControlObject::UnintializeGTOCO();
    return bGTOObject;
}

void CSsbImageCollectDlg::LogImageInfo(IMAGEDATA imaData)
{
    Trace(TRACE_INFO, _T("   IMAGEDATA.cImgePath     = %s"), imaData.cImgePath.c_str());
    Trace(TRACE_INFO, _T("   IMAGEDATA.nBitsPerPixel = %d"), imaData.nBitsPerPixel);
    Trace(TRACE_INFO, _T("   IMAGEDATA.nFrameType    = %d"), imaData.nFrameType);
    Trace(TRACE_INFO, _T("   IMAGEDATA.nImageHeight  = %d"), imaData.nImageHeight);
    Trace(TRACE_INFO, _T("   IMAGEDATA.nImageLength  = %d"), imaData.nImageLength);
    Trace(TRACE_INFO, _T("   IMAGEDATA.nImageType    = %d"), imaData.nImageType);
    Trace(TRACE_INFO, _T("   IMAGEDATA.nImageWidth   = %d"), imaData.nImageWidth);
}

LRESULT CSsbImageCollectDlg::OnUpdateStatus(UINT wParam, LONG lParam)
{
    if (lParam)
    {
        std::auto_ptr<CString> msg(reinterpret_cast<CString*>(lParam));
        UpdateStatus(*msg);
    }
    return 0;
}

void CSsbImageCollectDlg::UpdateStatus(CString csStatusLine)
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

    GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(csStatus);
    //GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Test2"));
    //GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("Test3"));
}


void CSsbImageCollectDlg::OnDataEvent(long lEvent)
{
    IMAGEDATA imaData;

    CDeviceConnector::GetInstance()->setDataEvent();

    bool bSuccess = CDeviceConnector::GetInstance()->CheckforDataEvent(imaData);

    if (bSuccess)
    {
        //LogImageInfo(imaData);

        CGTOControlObject::LogImageInfo(imaData);
        // UpdatePicture(imaData.cImgePath.c_str(), imaData.nFrameType, imaData.nImageType);
    }
    else
    {
        TRACE("FAILED to get image data");
    }
    // UpdateUiStatus(eIdle);
}

void CSsbImageCollectDlg::OnDirectIOEvent(long lEventNumber, long FAR* pData, BSTR FAR* pString)
{
}

void CSsbImageCollectDlg::OnStatusUpdateEvent(long lStatus)
{
    //CScanWatchSecurityBrokerApp* pApp = (CScanWatchSecurityBrokerApp*)AfxGetApp();
    //switch (lStatus)
    //{
    //case OPOS_SUE_POWER_ONLINE:
    //    if (pApp)
    //    {
    //        pApp->SetDeviceStatus(true);
    //    }
    //    break;
    //case OPOS_SUE_POWER_OFF:
    //case OPOS_SUE_POWER_OFF_OFFLINE:
    //case OPOS_SUE_POWER_OFFLINE:
    //    if (pApp)
    //    {
    //        pApp->SetDeviceStatus(false);
    //    }
    //    break;
    //default:
    //    break;
    //}
}


void CSsbImageCollectDlg::OnErrorEvent(long l1, long l2, long l3, long l4)
{

}

void CSsbImageCollectDlg::OnOutputCompleteEvent(long lEvent)
{

}

