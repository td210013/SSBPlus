#include "stdAfx.h"
#include "FileOperations.h"
#include "Logging.h"

//extern CmDataCapture g_cmDc;

/// <summary>
/// Initializes a new instance of the <see cref="CFExeption"/> class.
/// </summary>
/// <param name="dwErrCode">error code.</param>
/// <param name="sErrText">error text.</param>
CFExeption::CFExeption(DWORD dwErrCode,CString sErrText)
{
  LPVOID lpMsgBuf;

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, dwErrCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
  m_sError = (LPTSTR)lpMsgBuf;
  m_sError.Remove('\n');
  if(!sErrText.IsEmpty())
  {
    m_sError = sErrText + ": " + m_sError;
  }
  LocalFree(lpMsgBuf);
  m_dwError = dwErrCode;
}
/// <summary>
/// Initializes a new instance of the <see cref="CFExeption"/> class.
/// </summary>
/// <param name="sErrText">error text.</param>
CFExeption::CFExeption(CString sErrText)
{
  m_sError = sErrText;
  m_dwError = 0;
}
/// <summary>
/// Initializes a new instance of the <see cref="CFileOperations"/> class.
/// </summary>
CFileOperations::CFileOperations(void):CFOInterface()
{
}
/// <summary>
/// Finalizes an instance of the <see cref="CFileOperations"/> class.
/// </summary>
CFileOperations::~CFileOperations(void)
{
}
/// <summary>
/// Renames the file.
/// </summary>
/// <param name="sOldName">Old name of file.</param>
/// <param name="sNewName">New name of file.</param>
/// <returns></returns>
bool CFileOperations::RenameFile(std::string sOldName, std::string sNewName)
{
  try
  {
    DoRename(CString(sOldName.c_str()), CString(sNewName.c_str()));
  }
  catch(CFExeption* e)
  {
    m_sError = (char*)e->GetErrorText().GetString();
    m_dwError = e->GetErrorCode();
    Trace(TRACE_INFO, _T("Error occured while renaming the %s file to %s, Error Code = %d, Error String = %s"),sOldName,sNewName,m_dwError,m_sError );
    delete e;
    return FALSE;
  }
  return TRUE;
}
/// <summary>
/// Does the rename.
/// </summary>
/// <param name="sSource">source.</param>
/// <param name="sDest">dest.</param>
void CFileOperations::DoRename(CString sSource, CString sDest)
{
  if (!MoveFile(sSource, sDest)) throw new CFExeption(GetLastError());
}
/// <summary>
/// Deletes the specified file.
/// </summary>
/// <param name="sPathName">Path of the file</param>
/// <returns></returns>
bool CFileOperations::Delete(std::string sPathName)
{
  try
  {
    DoDelete(CString(sPathName.c_str()));
  }
  catch(CFExeption* e)
  {
    m_sError = (char*)e->GetErrorText().GetString();
    m_dwError = e->GetErrorCode();
    Trace(TRACE_INFO, _T("Error occured while deleting the %s file, Error Code = %d, Error String = %s"),sPathName,m_dwError,m_sError );
    delete e;
    if (m_dwError == 0) return TRUE;
    return FALSE;
  }
  return TRUE;
}
/// <summary>
/// Does the delete.
/// </summary>
/// <param name="sPathName">Path of the file</param>
void CFileOperations::DoDelete(CString sPathName)
{
  CString sPath = sPathName;

  if (CheckPath(sPath) == PATH_IS_FILE)
  {
    if (!CanDelete(sPath))
    {
      return;
    }
    if (!DeleteFile(sPath)) throw new CFExeption(GetLastError());
  }
}
/// <summary>
/// Determines whether this instance can delete the specified file.
/// </summary>
/// <param name="sPathName">Path of the file</param>
/// <returns></returns>
BOOL CFileOperations::CanDelete(CString sPathName)
{
  DWORD dwAttr = GetFileAttributes(sPathName);
  if (dwAttr == -1) return FALSE;
  if (dwAttr & FILE_ATTRIBUTE_READONLY)
  {
    if (!SetFileAttributes(sPathName, FILE_ATTRIBUTE_NORMAL)) return FALSE;
    return TRUE;
  }
  return TRUE;
}
/// <summary>
/// Checks the path.
/// </summary>
/// <param name="sPath">File Path</param>
/// <returns></returns>
int CFileOperations::CheckPath(CString sPath)
{
  DWORD dwAttr = GetFileAttributes(sPath);
  if (dwAttr == 0xffffffff)
  {
    if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND)
      return PATH_NOT_FOUND;
    return PATH_ERROR;
  }
  if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) return PATH_IS_FOLDER;
  return PATH_IS_FILE;
}
/// <summary>
/// Gets the file age in days.
/// </summary>
/// <param name="csFile">File Name</param>
/// <returns>Number of Days</returns>
int CFileOperations::GetFileAgeinDays(CString csFile)
{
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;
  hFind = FindFirstFile(csFile, &FindFileData);

  const FILETIME  ftFile = FindFileData.ftCreationTime;

  CTime ctFile = ftFile;
  CTime ctNow = ctNow.GetCurrentTime();

  CTimeSpan tsAge;
  tsAge = ctNow - ctFile;

  return (int)tsAge.GetDays();
}
/// <summary>
/// Does the copy.
/// </summary>
/// <param name="sSource">Source</param>
/// <param name="sDest">Destination</param>
void CFileOperations::DoCopy(CString sSource, CString sDest)
{
  if (!::CopyFile(sSource, sDest, false)) throw new CFExeption(GetLastError());
}
/// <summary>
/// Copies the image file.
/// </summary>
/// <param name="sSource">Source</param>
/// <param name="sDest">Destination</param>
/// <param name="bDeleteSource">if set to <c>true</c> [b delete source].</param>
/// <returns>TRUE/FALSE</returns>
bool CFileOperations::CopyImageFile(std::string sSource, std::string sDest, bool bDeleteSource)
{
  try
  {
    DoCopy(CString(sSource.c_str()), CString(sDest.c_str()));
    if(bDeleteSource)
      DoDelete(CString(sSource.c_str()));
  }
  catch(CFExeption* e)
  {
    m_sError = (char*) e->GetErrorText().GetString();
    m_dwError = e->GetErrorCode();
    Trace(TRACE_INFO, _T("Error occured while Copying the %s file to %s, Error Code = %d, Error String = %s"),sSource,sDest,m_dwError,m_sError );
    delete e;
    return FALSE;
  }
  return TRUE;
}
/// <summary>
/// Cleans the directory.
/// </summary>
/// <param name="sSource">Source Directory</param>
/// <param name="ndays">Number of days to delete the old files</param>
/// <returns>TRUE/FALSE</returns>
bool CFileOperations::CleanDirectory(std::string sSource, int ndays)
{
  CFileFind cFinder;
  sSource.append("\\*.*");
  BOOL bworking = cFinder.FindFile(CString(sSource.c_str()));
  while(bworking)
  {
    bworking = cFinder.FindNextFile();
    if (cFinder.IsDots() || cFinder.IsDirectory())
      continue;
    CString cFileName = cFinder.GetFilePath();
	if((cFileName.Find(_T(".bmp")) != -1) || (cFileName.Find(_T(".yuy2")) != -1) || (cFileName.Find(_T(".jpg")) != -1))
	{
		int numofDays = GetFileAgeinDays(cFileName);
		if(numofDays >= ndays)
		{
		  // SSCOB-14484 Modified the Code to handle the exception
		  Delete((char*)cFileName.GetString());
		}
	}
  }
  return TRUE;
}