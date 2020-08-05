#pragma once
#include "FOInterface.h"

#define PATH_ERROR			-1
#define PATH_NOT_FOUND		0
#define PATH_IS_FILE		1
#define PATH_IS_FOLDER		2
/// <summary>
/// File Exception Handler
/// </summary>
class CFExeption
{
public:
  CFExeption(DWORD dwErrCode,CString sErrText = _T(""));
  CFExeption(CString sErrText);
  CString GetErrorText()
  {
    return m_sError;
  }
  DWORD GetErrorCode()
  {
    return m_dwError;
  }

private:
  CString m_sError;
  DWORD m_dwError;
};
/// <summary>
/// File Operations
/// </summary>
class CFileOperations:public CFOInterface
{
public:
  CFileOperations(void);
  ~CFileOperations(void);
  bool RenameFile(std::string sOldName, std::string sNewName);
  bool Delete(std::string sPathName);
  bool CopyImageFile(std::string sSource, std::string sDest, bool bDeleteSource);
  bool CleanDirectory(std::string sSource, int ndays);

private:
  void DoRename(CString sSource, CString sDest);
  void DoDelete(CString sPathName);
  void DoCopy(CString sSource, CString sDest);
  BOOL CanDelete(CString sPathName);
  int CheckPath(CString sPath);
  int GetFileAgeinDays(CString csFile);
};
