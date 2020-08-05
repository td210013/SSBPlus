
// PipeServerMonitorDlg.h : header file
//

#pragma once
#include "vector"
#include "afxwin.h"


// CPipeServerMonitorDlg dialog
class CPipeServerMonitorDlg : public CDialogEx
{
// Construction
public:
	CPipeServerMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PIPESERVERMONITOR_DIALOG };
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
    afx_msg LRESULT OnUpdateImage(UINT wParam, LONG lParam);
    DECLARE_MESSAGE_MAP()

    //DECLARE_EVENTSINK_MAP()
    //void OnStatusUpdateEvent(long lStatus);
    BOOL InitializePipeServer();
    BOOL UninitializePipeServer();

    void UpdatePicture(CString filePath, int nFrameType, int nImageType);
    void UpdatePicture(CImage & a_cImage);
    bool Load_Image(const std::vector<uint8_t> &buf, CImage &image);

    void UpdateStatus(CString csStatusLine);
    std::vector<CString> m_vStatus;

public:
    //afx_msg void OnStnClickedStaticAppHealth();
    CStatic m_ctlStatic_AppHealth;
    CEdit m_ctlAppStatus;
    CStatic m_ctlStaticPicture;
    afx_msg void OnBnClickedButtonLoadImage();
};
