#pragma once
#ifdef _WIN32
#include "stdafx.h"
#endif
//#include "DeviceMetaData.h"
#include<string>
#include "SSBConstants.h"
using namespace std;
// Image Structure to store the Image details

typedef struct
{
	int nImageHeight;
	int nBitsPerPixel;
	int nFrameType;
	int nImageType;
	int nImageLength;
	int nImageWidth;
	std::string cImgePath;
    int nCameraNumber;
} IMAGEDATA,*PIMAGEDATA;

class CDCInterface
{
public:
	CDCInterface(void);
	virtual ~CDCInterface(void);
public :
	virtual bool InitScanWatchProcessing() = 0;
	virtual bool ResetScanWatchProcessing() = 0;
#ifdef _WIN32
	virtual bool InitializeDM(CWnd& hWnd, CString& csStatus, CString csProfile) = 0;
#else
	virtual bool InitializeDM(std::string& csStatus) = 0;
#endif
	virtual bool ReleaseDevice() = 0;
	virtual bool StartSession() = 0;
	virtual bool StopSession() = 0;
	virtual bool setDataEvent() = 0;
	virtual bool SetImageType(bool isRefImage) = 0;
	virtual bool SetCameraNum(int nCameraNum) = 0;
	virtual bool CheckforDataEvent(IMAGEDATA &imaData) = 0;
	//virtual bool GetDeviceMetaData(CDeviceMetaData& objSWMeta) = 0;

	//SSBERROR GetErrorInfo()
	//{
	//	return m_stErrorDetails;
	//}

protected:
	std::string m_sError;
	// SSBERROR m_stErrorDetails;
	unsigned long m_dwError;
	IMAGEDATA m_stImageData;
};
