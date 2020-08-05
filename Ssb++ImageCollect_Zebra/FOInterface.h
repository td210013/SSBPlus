#pragma once
#include <string>
using namespace std;
class CFOInterface
{
public:
  CFOInterface(void);
  virtual ~CFOInterface(void);
public:
  virtual bool RenameFile(std::string sOldName, std::string sNewName) = 0;
  virtual bool Delete(std::string sPathName) = 0;
  virtual bool CopyImageFile(std::string sSource, std::string sDest, bool bDeleteSource) = 0;
  virtual bool CleanDirectory(std::string sSource, int ndays) = 0;

  std::string GetErrorText()
  {
    return m_sError;
  }

  unsigned long GetErrorCode()
  {
    return m_dwError;
  }
protected:
  std::string m_sError;
  unsigned long m_dwError;
};

