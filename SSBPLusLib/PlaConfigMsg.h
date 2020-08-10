#pragma once
#include "PipeServer.h"
#include <string>

//<config _name="PLA" msgid="cb">
//    <hdr>
//        <transID></transID>
//        <computerName>192.168.163.133</computerName>
//        <sequenceID>19</sequenceID>
//        <lane>1</lane>
//    </hdr>
//    <timeout>3000</timeout>
//    <centerDetection>Off</centerDetection>
//    <numberofItems>24</numberofItems>
//    <enable>On</enable>
//    <scalemetric>Off</scalemetric>
//    <validation>On</validation>
//</config>

class CPlaConfigMsg
{
public:

    typedef enum {
        eStatusOk = 0,
        eStatusInvalidParameter = -1,
        eGeneralFailure
    } eStatus;

public:

    CPlaConfigMsg();
    ~CPlaConfigMsg();

public:

    _bstr_t hdr;
    int nNoOfitems;
    bool bEnable;
    bool bCenterDetection;
    bool bScaleMetric;
    long ltimeout;

    eStatus m_eStatus;
    bool bMaskTransId;

public:

    void Set(IGTGenObjPtr a_ptrGenObj);

    IGTGenObjPtr Get();

protected:

    _bstr_t GetObjectValue(IGTGenObjPtr a_ptrGenObj, LPWSTR a_szName );

    BOOL SetObjectValue(IGTGenObjPtr a_ptrGenObj, LPWSTR a_szName, _bstr_t a_szValue);

    const std::string GetCurrentDateTime();

private:
};

