#pragma once
#include <string>
using namespace std;
// #include "SSBException.h"
class CDeviceException // : public CSSBException
{
public:
	//CDeviceException();
	CDeviceException(DWORD dwErrCode, DWORD dwLibErrCode, std::string sErrText);
    DWORD GetErrorCode() { return m_dwErrorCode; }
    DWORD GetLibErrorCode() { return m_dwLibErrorCode; }

	// void SetExtErrorCode(long lexRetcode);
	// virtual void LoadErrorInfo();
	virtual ~CDeviceException(void);
public:
    std::string  GetErrorDesc() { return m_sErrorText; }
	std::string  GetSubSysErrorDesc(DWORD dwErrCode);
	// std::string GetDeviceExtErrorText(long lRetCode);

    DWORD m_dwErrorCode;
    DWORD m_dwLibErrorCode;
    std::string m_sErrorText;
};