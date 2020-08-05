#include "StdAfx.h"
#include "SSBCommException.h"
#include "SSBCommErrors.h"
#include "Resource.h"
/// <summary>
/// Initializes a new instance of the <see cref="CSSBCommException"/> class.
/// </summary>
/// <param name="dwErrCode">error code.</param>
/// <param name="sErrText">additional information</param>
CSSBCommException::CSSBCommException(DWORD dwErrCode,CString sErrText)
{
	m_sError = GetErrorString(dwErrCode);
	m_sError.Remove('\n');
	if(!sErrText.IsEmpty())
	{
		m_sError = sErrText + ": " + m_sError;
	}
	m_dwError = dwErrCode;
}
/// <summary>
/// Initializes a new instance of the <see cref="CSSBCommException"/> class.
/// </summary>
/// <param name="sErrText">error text.</param>
CSSBCommException::CSSBCommException(CString sErrText)
{
	m_sError = sErrText;
	m_dwError = 0;
}
/// <summary>
/// Gets the error string.
/// </summary>
/// <param name="dwErrCode">error code.</param>
/// <returns>error text</returns>
CString CSSBCommException::GetErrorString(long dwErrCode)
{
	CString csErrorText = _T("");
	switch(dwErrCode)
	{
    //case SSB_COMM_REG_ERROR_OPEN_FAILED:csErrorText.LoadString(IDS_SSB_COMM_REG_ERROR_OPEN_FAILED); break;
    //case SSB_COMM_REG_ERROR_QUERY_FAILED:csErrorText.LoadString(IDS_SSB_COMM_REG_ERROR_QUERY_FAILED); break;
    case SSB_COMM_REG_ERROR_OPEN_FAILED:        csErrorText = _T("Failed to open registry"); break;
	case SSB_COMM_REG_ERROR_QUERY_FAILED		:csErrorText = _T("Failed to query registry");break;
	default:csErrorText = _T("Success");
	}
	return csErrorText;
}