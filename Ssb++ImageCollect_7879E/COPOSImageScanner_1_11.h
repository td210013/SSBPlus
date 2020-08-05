// COPOSImageScanner_1_11.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// COPOSImageScanner_1_11

class COPOSImageScanner_1_11 : public CWnd
{
protected:
  // DECLARE_DYNCREATE(COPOSImageScanner_1_11)
public:
  CLSID const& GetClsid()
  {
    static CLSID const clsid
      = { 0xCCB90312, 0xB81E, 0x11D2, { 0xAB, 0x74, 0x0, 0x40, 0x5, 0x4C, 0x37, 0x19 } };
    return clsid;
  }
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
                      const RECT& rect, CWnd* pParentWnd, UINT nID,
                      CCreateContext* pContext = NULL)
  {
    return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
  }

  BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
              UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
              BSTR bstrLicKey = NULL)
  {
    return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
                         pPersist, bStorage, bstrLicKey);
  }

// Attributes
public:

// Operations
public:

  void SOData(long Status)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Status);
  }
  void SODirectIO(long EventNumber, long * pData, BSTR * pString)
  {
    static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PBSTR ;
    InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, EventNumber, pData, pString);
  }
  void SOError(long ResultCode, long ResultCodeExtended, long ErrorLocus, long * pErrorResponse)
  {
    static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PI4 ;
    InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ResultCode, ResultCodeExtended, ErrorLocus, pErrorResponse);
  }
  void SOOutputCompleteDummy(long OutputID)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, OutputID);
  }
  void SOStatusUpdate(long Data)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Data);
  }
  long SOProcessID()
  {
    long result;
    InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  long get_OpenResult()
  {
    long result;
    InvokeHelper(0x31, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  BOOL get_AutoDisable()
  {
    BOOL result;
    InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void put_AutoDisable(BOOL newValue)
  {
    static BYTE parms[] = VTS_BOOL ;
    InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  long get_BinaryConversion()
  {
    long result;
    InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  void put_BinaryConversion(long newValue)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  long get_CapPowerReporting()
  {
    long result;
    InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  CString get_CheckHealthText()
  {
    CString result;
    InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
    return result;
  }
  BOOL get_Claimed()
  {
    BOOL result;
    InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  long get_DataCount()
  {
    long result;
    InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  BOOL get_DataEventEnabled()
  {
    BOOL result;
    InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void put_DataEventEnabled(BOOL newValue)
  {
    static BYTE parms[] = VTS_BOOL ;
    InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  BOOL get_DeviceEnabled()
  {
    BOOL result;
    InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void put_DeviceEnabled(BOOL newValue)
  {
    static BYTE parms[] = VTS_BOOL ;
    InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  BOOL get_FreezeEvents()
  {
    BOOL result;
    InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void put_FreezeEvents(BOOL newValue)
  {
    static BYTE parms[] = VTS_BOOL ;
    InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  long get_PowerNotify()
  {
    long result;
    InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  void put_PowerNotify(long newValue)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  long get_PowerState()
  {
    long result;
    InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  long get_ResultCode()
  {
    long result;
    InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  long get_ResultCodeExtended()
  {
    long result;
    InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  long get_State()
  {
    long result;
    InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  CString get_ControlObjectDescription()
  {
    CString result;
    InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
    return result;
  }
  long get_ControlObjectVersion()
  {
    long result;
    InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  CString get_ServiceObjectDescription()
  {
    CString result;
    InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
    return result;
  }
  long get_ServiceObjectVersion()
  {
    long result;
    InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  CString get_DeviceDescription()
  {
    CString result;
    InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
    return result;
  }
  CString get_DeviceName()
  {
    CString result;
    InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapStatisticsReporting()
  {
    BOOL result;
    InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapUpdateStatistics()
  {
    BOOL result;
    InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapCompareFirmwareVersion()
  {
    BOOL result;
    InvokeHelper(0x2c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapUpdateFirmware()
  {
    BOOL result;
    InvokeHelper(0x2d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  long CheckHealth(long Level)
  {
    long result;
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Level);
    return result;
  }
  long ClaimDevice(long Timeout)
  {
    long result;
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Timeout);
    return result;
  }
  long ClearInput()
  {
    long result;
    InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  long Close()
  {
    long result;
    InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  long DirectIO(long Command, long * pData, BSTR * pString)
  {
    long result;
    static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PBSTR ;
    InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Command, pData, pString);
    return result;
  }
  long Open(LPCTSTR DeviceName)
  {
    long result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DeviceName);
    return result;
  }
  long ReleaseDevice()
  {
    long result;
    InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  long ResetStatistics(LPCTSTR StatisticsBuffer)
  {
    long result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, parms, StatisticsBuffer);
    return result;
  }
  long RetrieveStatistics(BSTR * pStatisticsBuffer)
  {
    long result;
    static BYTE parms[] = VTS_PBSTR ;
    InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pStatisticsBuffer);
    return result;
  }
  long UpdateStatistics(LPCTSTR StatisticsBuffer)
  {
    long result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x2b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, StatisticsBuffer);
    return result;
  }
  long CompareFirmwareVersion(LPCTSTR FirmwareFileName, long * pResult)
  {
    long result;
    static BYTE parms[] = VTS_BSTR VTS_PI4 ;
    InvokeHelper(0x2e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FirmwareFileName, pResult);
    return result;
  }
  long UpdateFirmware(LPCTSTR FirmwareFileName)
  {
    long result;
    static BYTE parms[] = VTS_BSTR ;
    InvokeHelper(0x2f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, FirmwareFileName);
    return result;
  }
  long ClearInputProperties()
  {
    long result;
    InvokeHelper(0x30, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapAim()
  {
    BOOL result;
    InvokeHelper(0x3c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapDecodeData()
  {
    BOOL result;
    InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapHostTriggered()
  {
    BOOL result;
    InvokeHelper(0x3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapIlluminate()
  {
    BOOL result;
    InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapImageData()
  {
    BOOL result;
    InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapImageQuality()
  {
    BOOL result;
    InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL get_CapVideoData()
  {
    BOOL result;
    InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  BOOL get_AimMode()
  {
    BOOL result;
    InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void put_AimMode(BOOL newValue)
  {
    static BYTE parms[] = VTS_BOOL ;
    InvokeHelper(0x43, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  long get_BitsPerPixel()
  {
    long result;
    InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  CString get_FrameData()
  {
    CString result;
    InvokeHelper(0x45, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
    return result;
  }
  long get_FrameType()
  {
    long result;
    InvokeHelper(0x46, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  BOOL get_IlluminateMode()
  {
    BOOL result;
    InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
    return result;
  }
  void put_IlluminateMode(BOOL newValue)
  {
    static BYTE parms[] = VTS_BOOL ;
    InvokeHelper(0x47, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  long get_ImageHeight()
  {
    long result;
    InvokeHelper(0x48, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  long get_ImageLength()
  {
    long result;
    InvokeHelper(0x49, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  long get_ImageMode()
  {
    long result;
    InvokeHelper(0x4a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  void put_ImageMode(long newValue)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x4a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  long get_ImageQuality()
  {
    long result;
    InvokeHelper(0x4b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  void put_ImageQuality(long newValue)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x4b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  long get_ImageType()
  {
    long result;
    InvokeHelper(0x4c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  long get_ImageWidth()
  {
    long result;
    InvokeHelper(0x4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  long get_VideoCount()
  {
    long result;
    InvokeHelper(0x4e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  void put_VideoCount(long newValue)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x4e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  long get_VideoRate()
  {
    long result;
    InvokeHelper(0x4f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
    return result;
  }
  void put_VideoRate(long newValue)
  {
    static BYTE parms[] = VTS_I4 ;
    InvokeHelper(0x4f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
  }
  long StartSession()
  {
    long result;
    InvokeHelper(0x5a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }
  long StopSession()
  {
    long result;
    InvokeHelper(0x5b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
    return result;
  }


};
