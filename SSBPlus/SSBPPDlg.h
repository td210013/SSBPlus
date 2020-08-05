
// SSBPPDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <vector>
#include "PipeServer.h"
#include "MessageHandler.h"


// CSSBPPDlg dialog
class CSSBPPDlg : public CDialogEx
{
// Construction
public:
	CSSBPPDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SSBPP_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    void UpdateStatus(CString csStatusLine);

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

protected:
    std::vector<CString> m_vStatus;
    CEdit m_ctlEdit_AppStatus;
    CPipeServer oPipeServer;
    CMessageHandler m_oMessageHandler;
public:
    afx_msg void OnBnClickedExit();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
