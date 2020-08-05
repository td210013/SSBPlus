
#ifdef WIN32
#include "stdafx.h"
#endif

#include "DCInterface.h"

CDCInterface::CDCInterface(void): m_sError("") ,m_dwError(0)
{
	// m_stErrorDetails.nErrorCode = 0;
	//m_stErrorDetails.nSubSystemError = 0;
	//m_stErrorDetails.enErrorModule = HEALTHY;
	//m_stErrorDetails.enErroLevel = SSB_INFO;
	//m_stErrorDetails.enErrorSatus = UNINITIALIZED;
}

CDCInterface::~CDCInterface(void)
{
}
