#ifndef _BASE64_ENCODE_DECODE_H
#define _BASE64_ENCODE_DECODE_H

#include "string"

class CBase64EncodeDecode
{

public:
	CBase64EncodeDecode();
	virtual ~CBase64EncodeDecode();


    //CString EncodeString(CString csSource);
    //CString DecodeString(CString csEncodedData);

    void Base64Encode(const void* pSrc, const size_t nLen, std::string & outStr);
    void Base64Decode(const void* pSrc, const size_t nLen, std::string & outStr);

    //CString GetBase64EncodeWithPrefix(const CString csPrefix, const CString csSource);
    //CString GetBase64DecodeWithPrefix(const CString csPrefix, const CString csEncodedString);

protected:
    
    //char* ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn, int sizein, int* sizeout);

};

#endif 
