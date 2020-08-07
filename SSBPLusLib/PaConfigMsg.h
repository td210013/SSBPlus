#pragma once
#include "PipeServer.h"
#include <string>

//  <config _name="PA" msgid="ca">
//      <hdr>
//          <transID></transID>
//          <computerName>192.168.163.133</computerName>
//          <sequenceID>19</sequenceID>
//          <lane>1</lane>
//      </hdr>
//      <timeout>3000</timeout>
//      <centerDetection>Off</centerDetection>
//      <enable>On</enable>
//      <mode>0</mode>
//      <scalemetric>Off</scalemetric>
//  </config>

class CPaConfigMsg
{
public:

    typedef enum
    {
        ePA_MODE_0_CONSERVATIVE = 0,
        ePA_MODE_1_UNKNOWN,
        ePA_MODE_2_UNKNOWN,
        ePA_MODE_3_AGGRESSIVE,
        ePA_MODE_4_UNKNOWN
    } ePAMODE;


    typedef enum {
        eStatusOk = 0,
        eStatusInvalidParameter = -1,
        eGeneralFailure
    } eStatus;

public:

    CPaConfigMsg();
    ~CPaConfigMsg();

public:

    _bstr_t hdr;
    bool bEnable;
    bool bCenterDetection;
    bool bScaleMetric;
    ePAMODE eMode;
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

