
// Ssb++ImageCollectDlg.h : header file
//

#pragma once

#include "DeviceConnector.h"
#include "afxcmn.h"
#include "vector"
#include "afxwin.h"

// CSsbImageCollectDlg dialog
class CSsbImageCollectDlg : public CDialogEx
{
// Construction
public:
	CSsbImageCollectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SSBIMAGECOLLECT_DIALOG };
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
    afx_msg LRESULT OnUpdateStatus(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();

    DECLARE_EVENTSINK_MAP()
    void OnDirectIOEvent(long lEventNumber, long FAR* pData, BSTR FAR* pString);
    void OnStatusUpdateEvent(long lStatus);
    void OnErrorEvent(long l1, long l2, long l3, long l4);
    void OnOutputCompleteEvent(long lEvent);
    void OnDataEvent(long lStatus);

    void LogImageInfo(IMAGEDATA imaData);

    BOOL InitializePipeServer();
    BOOL UninitializePipeServer();
    BOOL GetDeviceImage(int a_nCameraNum, BOOL a_bIsRefImage);

    void UpdateStatus(CString csStatusLine);

    std::vector<CString> m_vStatus;


    CStatic m_ctlStatic_AppHealth;
};
