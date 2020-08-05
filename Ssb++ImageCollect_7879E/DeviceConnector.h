#pragma once
#include "DCInterface.h"
#include "NCRImg.h"
#include "OposImg.h"
#include "COPOSImageScanner_1_11.h"
#include "DeviceCapabilities.h"

// Class to handle NCR_7879 Scanner Device with opos standards
class CDeviceConnector: public CDCInterface
{
public:
	/**
	* @brief Destructor
	*/
	~CDeviceConnector(void);
	/**
	* @brief Get single instance of DeviceConnector object.
	*
	* @return
	*   CDeviceConnector object.
	*/
	static CDeviceConnector* GetInstance()
	{
		if(m_pDevice == NULL)
			m_pDevice = new CDeviceConnector();
		return m_pDevice;
	}

    static void ReleaseInstance()
    {
        if (m_pDevice != NULL)
        {
            m_pDevice->ReleaseDevice();
            delete m_pDevice;
            m_pDevice = NULL;
        }
    }

private:

	/**
	* @brief: Constructor
	*/
	CDeviceConnector(void);
	static CDeviceConnector*    m_pDevice;          // static instance of the DeviceConnector
	COPOSImageScanner_1_11      m_devImagerScanner; // instance of Imagescanner Device
	BOOL	                    m_bDeviceOpen;      // checks the device is open or not
	CDeviceCapabilities         m_oDevCaps;         // Device Capabilites
	CEvent*                     m_pDataRecived;     // Event to recieve the Data
	HANDLE                      m_hEventLog;
public :
	/**
	* @brief: Initializes the  Scanwatch processing.
	*
	* @param:void
	*
	* @return: true/false
	*
	*/
	bool InitScanWatchProcessing();

	/**
	* @brief: Resets the Scanwatch processing.
	*
	* @param:void
	*
	* @return: true/false
	*
	*/
	bool ResetScanWatchProcessing();
	/**
	* @brief: It will create and open the device.
	*
	* @param: hWnd - Handle to window  [OUT]csStatus - Status of the method
	*
	* @return: true/false
	*
	*/
	bool InitializeDM(CWnd& hWnd, CString& csStatus, CString csProfile);
	/*
	* @brief: Closes and releases the device.
	*		  
	* @param: void
	*
	* @return: bool - true/false
	*
	*/
	bool ReleaseDevice();
	/**
	* @brief: Retrives the Image from the scanner
	*
	* @param: void
	*
	* @return: true/false
	*
	*/
	bool StartSession();
	/**
	* @brief: Ends the Session.
	*
	* @param: void
	*
	* @return: true/false
	*
	*/
	bool StopSession();
	/*
	* @brief: When window/dialog recieves the data event it will call this function.
	*		  This function will get all information about the image.
	*
	* @param: void
	*
	* @return: bool - true/false
	*
	*/
	bool setDataEvent();
	/*
	* @brief: Before StartSession Application should set the image type to
	*				 retrive the image of specifed type.
	*
	* @param: isRefImage: True if its a reference image
	*
	* @return: bool - true/false
	*
	*/
	bool SetImageType(bool isRefImage);

	/**
	* @brief Before StartSession Application should set the camera number to
	*		 retrive the image from specifed camera number.
	*
	* @param: nCameraNum: Camera Number
	*
	* @return: bool - true/false
	*
	*/
	bool SetCameraNum(int nCameraNum);
	/*
	* @brief:  Function will wait for the data event from the device and fills the image
	*			information structure.
	*
	* @param: imaData - Image Information
	*
	* @return: bool - true/false
	*
	*/
	bool CheckforDataEvent(IMAGEDATA &imaData);
	/*
	* @brief:  Gets the metadata from the device and retries for metadata if device reports refreshing metadata.
	*
	* @param: [OUT]objSWMeta- Metadata Structure
	*
	* @return: bool - true/false
	*
	*/
	// bool GetDeviceMetaData(CDeviceMetaData& objSWMeta);
	/*
	* @brief:  Gets the camera info data bytes.
	*
	* @param: [OUT]camInfoOut - stores the camera info data bytes retrieved during
	*                           device initialization. Buffer must be large
	*                           enough to store the expected 512 bytes of data.
	*			[in] nModel - '0'-7879, '1'- 7879e (Clear Sheild), '2'- 7879e(Saphire Plate)
	*
	* @return: bool - true/false
	*
	*/
	void GetCameraInfoBytes(unsigned char *camInfoOut, int nModel = 0);
	/*
	* @brief:  Gets whether the Device supports the PLA Functionality
	*
	* @param: void
	*
	* @return: bool - true/false
	*
	*/
	bool IsPLASupported(){return m_oDevCaps.IsPLASupported();}
	/*
	* @brief:  Gets whether the Device supports the PA Functionality
	*
	* @param: void
	*
	* @return: bool - true/false
	*
	*/
	bool IsPASupported(){return m_oDevCaps.IsPASupported();}

	/*
	* @brief:  Gets whether the UVC is enabled on Device
	*
	* @param: void
	*
	* @return: bool - true/false
	*
	*/
	bool IsUVCEnabled(){return m_oDevCaps.IsUVCEnabled();}

	/*
	* @brief:  Gets whether the Scanwatch is enabled on Device
	*
	* @param: void
	*
	* @return: bool - true/false
	*
	*/
	bool IsScanwatchEnabled(){return m_oDevCaps.IsScanwatchEnabled();}

	/*
	* @brief:  Gets the Device Model
	*
	* @param: void
	*
	* @return: std::string - Model or N/A incase of UVC Disabled or FW not supported.
	*
	*/
	std::string GetModelNum(){return m_oDevCaps.GetModelNumber();}


	/*
	* @brief:  Gets the Device FW Version
	*
	* @param: void
	*
	* @return: std::string - FW version or N/A incase of UVC Disabled or FW not supported.
	*
	*/
	std::string GetFWVersion(){return m_oDevCaps.GetFWVersion();}
	

	/*
	* @brief:  Gets the Device Serial Number
	*
	* @param: void
	*
	* @return: std::string - Serial Number or N/A incase of UVC Disabled or FW not supported.
	*
	*/
	std::string GetSerialNum(){return m_oDevCaps.GetSerialNumber();}
	
	/**
	* @brief Gets camera setup information from the scanner.
	*
	* @param: camInfo - Camera Information
	*
	* @return: bool - true/false
	*
	*/
	bool GetCameraInfo(unsigned char *camInfo);

	/**
	* @brief check whether the PA Library Status
	*
	* @param void
	*
	* @return SSBSTATUS: ONLINE, OFFLINE, UNINITIALIZED
	*/
	//SSBSTATUS GetStatus()
	//{
	//	return m_stErrorDetails.enErrorSatus;
	//}

	/**
	* @brief refreshes the device capabilities
	*
	* @param void
	*
	* @return bool: true/false
	*/
	bool RefreshDeviceCaps();

	/**
	* @brief Set Device Online Status
	*
	* @param IsOnline - false/true
	*
	* @return bool: true/false
	*/
	bool SetDeviceOnlineStatus(bool IsOnline);

    //void WaitForDataEvent();


private:
	/*
	* @brief:  Checks whether the device is calimed or not.
	*
	* @param: void
	*
	* @return: bool - true/false
	*
	*/
	bool IsClaimed();

    // CWnd& m_hWnd;
    HWND m_ParentHwnd;

	BYTE m_ucCameraInfoData[SCANWATCH_CAMERAINFO_SIZE];
};

