#include "stdafx.h"
#include "NcrZebraCamera.h"

#include <iostream>
#include <windows.h>
#include <strsafe.h>
#include "Base64EncodeDecode.h"
#include "config.h"
#include "logging.h"

#pragma comment(lib, "device_manager.lib")
#pragma comment(lib, "zebracam.lib")
#pragma comment(lib, "image_converter.lib")

#define IMAGE_EXT_BMP ".bmp"
#define IMAGE_EXT_JPEG ".jpeg"
#define IMAGE_NAME_PREFIX "ZebraImage_"

//SnapshotImageEventHandler::SnapshotImageEventHandler(IEventHandler* pEventHandler): m_pEventHandler(pEventHandler)
//{}
//
//void SnapshotImageEventHandler::ImageReceived(ImageEventData ev, ImageEventMetaData evm)
//{
//    m_pEventHandler->HandleImage(ev.image, ev.format, "SnapShot");
//}

NcrZebraCamera::NcrZebraCamera()
{};

NcrZebraCamera::~NcrZebraCamera()
{
    camera_ = nullptr;

    //if (snapshot_image_event_handler_)
    //{
    //    delete snapshot_image_event_handler_;
    //}
};

bool NcrZebraCamera::Initialize()
{
    bool bSuccess = false;

    device_info_list_ = device_manager_.EnumerateDevices();

    if (device_info_list_.size() > 0)
    {
        camera_ = camera_manager_.CreateZebraCamera(device_info_list_[0]);
    }
    else
    {
        Trace(TRACE_ERROR, _T("Enumerate devices returned 0 devices."));

        return false;
    }

    if (camera_ != nullptr)
    {
        LogDeviceDetails();

        // snapshot_image_event_handler_ = new SnapshotImageEventHandler(this);

        std::vector<FrameType> vFrameTypes = camera_->GetSupportedFrameTypes();

        LogFrameTypes(vFrameTypes);

        if (static_cast<size_t>(GetConfig()->FrameType) >= vFrameTypes.size())
        {
            Trace(TRACE_WARNING, _T("Warning! Invalid frame type index: %d, using default frame type."), GetConfig()->FrameType);
        }
        else
        {
            camera_->SetCurrentFrameType(vFrameTypes[GetConfig()->FrameType]);

            Trace(TRACE_INFO, _T("Using FrameType [%d]: %s"), GetConfig()->FrameType, GetFrameTypeString(vFrameTypes[GetConfig()->FrameType]));
        }

        camera_->IlluminationModeSetting.Value((IlluminationMode)GetConfig()->IlluminationMode);

        CString fullPath = _T("c:\\temp\\");
        // fullPath.AppendFormat(_T("\\%s"), _T(IMAGE_SAVE_FOLDER_DEFAULT));
        StringCbCopyW(image_save_path_, MAX_PATH, fullPath);
        CreateDirectory(image_save_path_, NULL);

        target_file_format_ = TargetFileFormat::JPEG;
        bSuccess = true;
    }
    else
    {
        Trace(TRACE_ERROR, _T("ERROR! Failed to create camera device."), GetConfig()->FrameType);
    }

    return bSuccess;
}

bool NcrZebraCamera::TakeSnapShot(NcrImageData &a_sNcrImageData)
{
    bool bSuccess = false;

    if (camera_ == nullptr)
    {
        Trace(TRACE_ERROR, _T("ERROR! camera instance is null."));

        return false;
    }
    else
    {
        zebra::camera_sdk::Image snapshot_image = camera_->CaptureSnapshot();

        ImageData image_data = { snapshot_image.Width(), snapshot_image.Height(), snapshot_image.Stride(), snapshot_image.Length(), snapshot_image.Data() };

        auto image_format = snapshot_image.Format();

        converter_type_ = GetFileConverter(image_format);

        // convert YUY2 data to target file format
        a_sNcrImageData.data = zebra::image::Encode(converter_type_, image_data);

        if (a_sNcrImageData.data.size() > 0)
        {
            a_sNcrImageData.format = ImageFileFormatToString(target_file_format_);
            a_sNcrImageData.height = image_data.height;
            a_sNcrImageData.width = image_data.width;
            a_sNcrImageData.size = a_sNcrImageData.data.size();
            a_sNcrImageData.stride = image_data.stride;

            // B64 conversions 
            CBase64EncodeDecode oB64;

            oB64.Base64Encode((const void*)&a_sNcrImageData.data[0], a_sNcrImageData.data.size(), a_sNcrImageData.B64data);


            bSuccess = true;
        }
        else
        {
            Trace(TRACE_ERROR, _T("ERROR! Camera image has a zero byte length."));
        }
    }
    return bSuccess;
}


CString NcrZebraCamera::GetFileName(std::string decode_data_str)
{
    CString currentTime = CTime::GetCurrentTime().Format("%Y%m%d_%H%M%S");
    SYSTEMTIME systemTime;
    GetSystemTime(&systemTime);
    auto seconds = std::to_string(systemTime.wMilliseconds);
    CString secondsStr(seconds.c_str());
    currentTime = currentTime + L"_" + secondsStr + L"_";

    std::string illegal_chars("\\/:?\"<>|");  // Remove illegal characters for file names , from the string
    std::string::iterator it;

    for (it = decode_data_str.begin(); it != decode_data_str.end(); ++it)
    {
        BOOL found = illegal_chars.find(*it) != std::string::npos;
        if (found) {
            *it = ' ';
        }
    }

    decode_data_str = decode_data_str + "_";

    CString image_source("c");
    // decide file extension to save image
    CString fileExtention;
    if (target_file_format_ == TargetFileFormat::BMP)
    {
        fileExtention = IMAGE_EXT_BMP;
    }
    else if (target_file_format_ == TargetFileFormat::JPEG)
    {
        fileExtention = IMAGE_EXT_JPEG;
    }
    else
    {
        fileExtention = "";
    }

    CString csFoldereName(image_save_path_);
    CString csFileName = csFoldereName + L"\\" + IMAGE_NAME_PREFIX + currentTime + decode_data_str.c_str() + image_source + fileExtention;

    return csFileName;
}

void NcrZebraCamera::SaveImage(std::vector<uint8_t>* imageBuffer, CString file_name)
{
    try
    {
        CFile saveImagefile;
        saveImagefile.Open(file_name, CFile::modeCreate | CFile::modeWrite);
        saveImagefile.SeekToBegin();
        saveImagefile.Write(imageBuffer->data(), (UINT)imageBuffer->size());
        saveImagefile.Close();
    }
    catch (...)
    {
        // check if error occurs due to save location issue
        //if (!CheckSaveImageLocation())
        //{
        //    m_check_autosaveimage_value_ = FALSE;
        //    m_button_save_.EnableWindow(TRUE);
        //    m_button_browse_.EnableWindow(TRUE);
        //}
    }
}

zebra::image::FileConverter NcrZebraCamera::GetFileConverter(ImageFormat& format)
{
    switch (format)
    {
    case ImageFormat::YUY2:
        return target_file_format_ == TargetFileFormat::BMP ? zebra::image::FileConverter::YUY2_TO_BMP : zebra::image::FileConverter::YUY2_TO_JPEG;
    case ImageFormat::UYVY:
        return target_file_format_ == TargetFileFormat::BMP ? zebra::image::FileConverter::UYVY_TO_BMP : zebra::image::FileConverter::UYVY_TO_JPEG;
    default:
        return target_file_format_ == TargetFileFormat::BMP ? zebra::image::FileConverter::YUY2_TO_BMP : zebra::image::FileConverter::YUY2_TO_JPEG;
        break;
    }
}

//void NcrZebraCamera::HandleImage(ImageData& evdat, ImageFormat& format, std::string decode_data)
//{
//    converter_type_ = GetFileConverter(format);
//    std::vector<uint8_t> image = zebra::image::Encode(converter_type_, evdat);
//    std::string temp_decode_data = "";
//    auto it = decode_data.begin();
//    for (; it < decode_data.end(); it++)
//    {
//        if (*it < 0x20)
//        {
//            temp_decode_data.append(" ");
//        }
//        else
//        {
//            auto s = *it;
//            temp_decode_data.append(1, *it);
//        }
//    }
//    CString csFileName = GetFileName(temp_decode_data);
//    std::future<void> result(std::async(std::launch::async | std::launch::deferred, &NcrZebraCamera::SaveImage, this, &image, csFileName));
//    result.get();
//}

void NcrZebraCamera::LogFrameTypes(const vector<FrameType> & vframe_types)
{
    Trace(TRACE_INFO, _T("Zebra Camera Frame Types: "));

    for (int i = 0; i < (int)vframe_types.size(); i++)
    {
        Trace(TRACE_INFO, _T("  [%d]: %s"), i, GetFrameTypeString(vframe_types[i]).GetString());
    }
}

CString NcrZebraCamera::GetFrameTypeString(const FrameType& frame_type)
{
    auto format = frame_type.format;
    auto format_string = format == zebra::camera_sdk::FrameDataFormat::YUY2 ? "YUY2" : format == zebra::camera_sdk::FrameDataFormat::UYVY ? "UYVY" : "UNKNOWN";
    std::string frame_type_name; //= (int)frame_type.format_ + "_" /*+ (int)frame_type.width_ + "_" + (int)frame_type.height_ + "_" */ + (int)frame_type.framerate_;
    frame_type_name.append(format_string);
    frame_type_name.append("_");
    frame_type_name.append(std::to_string(frame_type.width));
    frame_type_name.append("_");
    frame_type_name.append(std::to_string(frame_type.height));
    frame_type_name.append("_");
    frame_type_name.append(std::to_string(frame_type.framerate));
    return CString(frame_type_name.c_str());
}


const char * NcrZebraCamera::ImageFileFormatToString(TargetFileFormat a_eFormat)
{
    char * szFormat = "IMAGE_FORMAT_UNKNOWN";

    switch (a_eFormat)
    {
    case BMP:
        szFormat = "BMP";
        break;
    case JPEG:
        szFormat = "JPEG";
        break;
    }
    return szFormat;
}


void NcrZebraCamera::LogDeviceDetails()
{
    try
    {
        if ( camera_ == nullptr)
        {
            return;
        }

        Trace(TRACE_INFO, _T("Camera Details:"));
        Trace(TRACE_INFO, _T("  Serial Number:      %s"), CString( camera_->GetSerialNumber().c_str()).GetString() );
        Trace(TRACE_INFO, _T("  Model Number:       %s"), CString(camera_->GetModelNumber().c_str()).GetString());
        Trace(TRACE_INFO, _T("  Manufacture Date:   %s"), CString( camera_->GetDateOfManufacture().c_str()).GetString());
        Trace(TRACE_INFO, _T("  First Service Date: %s"), CString( camera_->GetFirstServiceDate().c_str()).GetString());
        Trace(TRACE_INFO, _T("  First Program Date: %s"), CString( camera_->GetDateOfFirstProgram().c_str()).GetString()); 
        Trace(TRACE_INFO, _T("  Hardware Version:   %s"), CString( camera_->GetHardwareVersion().c_str()).GetString());

        try
        {
            std::string ssFirmwareVersion = camera_->GetFirmwareVersion().c_str();

            Trace(TRACE_INFO, _T("  Firmware Version:   %s"), CString(ssFirmwareVersion.c_str()).GetString());
        }
        catch (const std::exception& e)
        {
            Trace(TRACE_INFO, _T("  Could not get firmware version: %s"), CString( e.what()).GetString() );
        }
    }
    catch (const std::exception& e)
    {
        Trace(TRACE_INFO, _T("LogDeviceDetails() exception occured: %s"), CString(e.what()).GetString());
    }

}