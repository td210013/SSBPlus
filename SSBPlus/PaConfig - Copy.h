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

class CPaConfig
{

    typedef enum {
        eStatusOk = 0,
        eStatusInvalidParameter = -1,
        eGeneralFailure
    } eStatus;

public:
    CPaConfig();
    ~CPaConfig();


    CString name; // {"PA"|"PLA"}

    CString hdr;

    //<hdr>
        CString transId;
        CString computerName;
        CString sequenceID;
        CString lane;
    //</hdr>
    CString timeout;
    CString centerDetection;
    CString enable;
    CString mode;
    CString scalemetric;

    eStatus m_eStatus;
        

    void Set(IGTGenObjPtr a_ptrGenObjPtr);
    IGTGenObjPtr Get();

protected:
    _bstr_t GetObjectValue(IGTGenObjPtr a_ptrGenObjPtr, LPWSTR a_szName );
    BOOL SetObjectValue(IGTGenObjPtr a_ptrGenObjPtr, LPWSTR a_szName, _bstr_t a_szValue);

    BOOL Validate();

    const std::string currentDateTime();

private:
};

