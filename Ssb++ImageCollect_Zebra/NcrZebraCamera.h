#pragma once

#include <vector>
#include <windows.h>
#include "device_manager.h"
#include "image_converter.h"
#include "zebra_camera.h"
#include "zebra_camera_manager.h"
#include "camera_bounding_box_factory.h"

using namespace zebra;
using namespace zebra::camera_sdk;
using namespace zebra::image;
using namespace std;

struct NcrImageData
{
    std::string format;
    uint32_t width;
    uint32_t height;
    size_t stride;
    size_t size;
    std::string B64data;
    std::vector<uint8_t> data;
};

//Target file format
enum TargetFileFormat
{
    BMP,
    JPEG
};

#define IMAGE_EXT_BMP ".bmp"
#define IMAGE_EXT_JPEG ".jpeg"
#define IMAGE_NAME_PREFIX "ZebraImage_"

//class IEventHandler
//{
//public:
//    virtual void HandleImage(ImageData& evdat, ImageFormat& format, std::string decode_data) = 0;
//};

//// class to Handle Snapshot Image Events 
//class SnapshotImageEventHandler :public SnapshotImageEventListener
//{
//public:
//    SnapshotImageEventHandler(IEventHandler* pEventHandler);
//
//    void ImageReceived(ImageEventData ev, ImageEventMetaData evm) override;
//
//private:
//    IEventHandler* m_pEventHandler;
//};


class NcrZebraCamera // : public IEventHandler
{
public:

    NcrZebraCamera();

    virtual ~NcrZebraCamera();

    bool Initialize();

    // void TakeSnapShot();

    bool TakeSnapShot(NcrImageData &image_data);

    // Save Image to a file
    void SaveImage(std::vector<uint8_t>* imageBuffer, CString file_name);

protected:

    //Generate File Name
    CString GetFileName(std::string decode_data_str);

    zebra::image::FileConverter GetFileConverter(ImageFormat& format);

    //void HandleImage(ImageData& evdat, ImageFormat& format, std::string decode_data = "");

    CString GetFrameTypeString(const FrameType& frame_type);

    void LogFrameTypes(const vector<FrameType> & vframe_types);

    const char * ImageFileFormatToString(TargetFileFormat a_eFormat);

    void LogDeviceDetails();


private:

    std::shared_ptr<ZebraCamera> camera_ = nullptr;
    std::vector<DeviceInfo> device_info_list_;
    DeviceInfo* selected_device_info_;

    //SnapshotImageEventHandler* snapshot_image_event_handler_ = nullptr;

    ZebraCameraManager camera_manager_;
    DeviceManager device_manager_;

    // TargetFileFormat target_file_format_ = TargetFileFormat::BMP;
    TargetFileFormat target_file_format_ = TargetFileFormat::JPEG;
    FileConverter converter_type_;

    TCHAR image_save_path_[MAX_PATH];

    std::vector<uint8_t> imageBuffer_;
};

