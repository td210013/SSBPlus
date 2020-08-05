#pragma once

typedef enum
{
	PIPE_SERVER_EXCEPTION,
	DATABASE_EXCEPTION,
	DEVICE_EXCEPTION
}EXCEPTIONTYPE;

class CSSBCommException 
{
public:
	CSSBCommException(DWORD dwErrCode,CString sErrText = _T(""));
	CSSBCommException(CString sErrText);
	CString GetErrorText() {return m_sError;}
	DWORD GetErrorCode() {return m_dwError;}
	
private:
	CString m_sError;
	DWORD m_dwError;
	CString GetErrorString(long dwErrCode);

private:
	void ReconnectPipeServer();
	void ReconnectDatabase();
	void RefreshDeviceConnection();

public:
	//void ReportException(EXCEPTIONTYPE, char*);
	void LogErrorMsg(const char*);
};
