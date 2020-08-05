#pragma once
#include <string>
struct CameraSetup
{
    unsigned short illumination;
    unsigned short mono_dark_level;
    unsigned char mono_sigma_noise[5];
    unsigned char mono_noise_slope[5];
    unsigned short color_dark_level;
    unsigned char color_sigma_noise[5];
    unsigned char color_noise_slope[5];
    unsigned char num_cameras;
    /**
    * MAX_NUM_CAMERAS is defined a 6 in the FW.
    */
    struct
    {
    /**
        * Identifies the camera.
        * Possible values:
        * - "VERTICAL"
        * - "LEADTRAIL"
        * - "HORIZONTAL"
        * - "TOPDOWN"
        * - "ICS"
        * - "RCS1"
        * - "RCS2"
        */
    unsigned char camera_type[15];
    /**
        * Identifies the sensor and mono/color combination.
        * Possible values:
        * - "M021"
        * - "AR0134Mono"
        * - "AR0134Color"
        * - "AR0135Mono"
        * - "AR0135Color"
        * - "Invalid"
        */
    unsigned char sensor_type[15];
    unsigned char red_gain[5];
    unsigned char green_gain[5];
    unsigned char blue_gain[5];
    } cam_params[7];
	unsigned char scanwatch_status;  // 1- sw enabled 0-sw disabled
	unsigned char scale_status;      //   0 -scale enable , 1-scale disable
	unsigned char firmware_version[12];  // firmware version
	unsigned char serial_number[12];     // serial number
	unsigned char class_model[10];        // class model .
};
enum CAMERA_TYPES
    {
      VERTICAL_CAMERA = 0,
      LEADTRAIL_CAMERA,
      HORIZONTAL_CAMERA,
      TOPDOWN_CAMERA,
      ICS_CAMERA,
      RCS_CAMERA_1,
      RCS_CAMERA_2,
      MAX_NUM_CAMERAS
    };
class CDeviceCapabilities
{
public:
	CDeviceCapabilities(void);
	~CDeviceCapabilities(void);
private:
	static const DWORD m_nVid = 1028;
	static const DWORD m_nPid = 533;
	bool m_bUVCEnabled;
	bool m_bScanwatchEnable;
	bool m_bScaleOnline;
	bool m_bScannerModelSupported;
	bool m_bPLASupport;
	bool m_bPASupport;
	bool m_bFirmwareSupported;
	std::string m_csFirmwareVersion;
	std::string m_csModel;
	std::string m_csSerialNumber;
public:
	bool SetDevCapsfromCamInfo(unsigned char *camInfoOut);
	BOOL CheckUVCDeviceEnabled();
	bool IsUVCEnabled() {return m_bUVCEnabled;}
	bool IsScanwatchEnabled(){return m_bScanwatchEnable;}
	bool IsScaleOnline(){return m_bScaleOnline;}
	bool IsPLASupported(){return m_bPLASupport;}
	bool IsPASupported(){return m_bPASupport;}
	bool IsFirmwareSupported(){return m_bFirmwareSupported;}
	bool IsModelSupported(){return m_bScannerModelSupported;}
	std::string GetModelNumber(){return m_csModel;}
	std::string GetFWVersion(){return m_csFirmwareVersion;}
	std::string GetSerialNumber(){return m_csSerialNumber;}
private:
	//BOOL IsMatchingDeviceFound(CString& strDevicePath);
};

