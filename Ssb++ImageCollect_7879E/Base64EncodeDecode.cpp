#include "stdafx.h"
#include "Base64EncodeDecode.h"
#include <string>

const char* pBase64CharSet=("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                         "abcdefghijklmnopqrstuvwxyz"
                                         "0123456789+/");
const int nBase64Index[256] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                    0,0,0,0,0,0,0,0,0,0,0,62,63,62,62,63,52,53,54,55,56,57,58,59,60,61,
                                    0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
                                    21,22,23,24,25,0,0,0,0,63,0,26,27,28,29,30,31,32,33,34,35,36,37,38,
                                    39,40,41,42,43,44,45,46,47,48,49,50,51 };


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBase64EncodeDecode::CBase64EncodeDecode()
{
}

CBase64EncodeDecode::~CBase64EncodeDecode()
{
}

/*
CString CBase64EncodeDecode::EncodeString(CString csSource)
{
    int nBufferLength = csSource.GetLength();
    LPTSTR sBuffer = csSource.GetBuffer(nBufferLength);

    //need to convert to ANSI for Base64 Encoding
    char *myANSIdata;
    int myANSIDataSize = 0;
#ifdef _UNICODE
    myANSIdata = ConvertLPWSTRToLPSTR((LPTSTR)sBuffer, nBufferLength, &myANSIDataSize);
#else 
    myANSIdata = new char[nBufferLength];
    myANSIDataSize=nBufferLength;
    strcpy(myANSIdata, sBuffer);
#endif
    int nEncodedSize=4*nBufferLength*sizeof(TCHAR);
    char *buffer = new char[nEncodedSize];
    memset(buffer,0,nEncodedSize);
    Base64Encode((unsigned char *)myANSIdata, myANSIDataSize, buffer);

    CString csEncodedString = buffer;
    csSource.ReleaseBuffer();
    
    delete [] buffer;
    delete [] myANSIdata;

    return csEncodedString;
   
}
*/

void CBase64EncodeDecode::Base64Encode(const void* pSrc, const size_t nLen, std::string & outStr)
{
    unsigned char* pSource = (unsigned char*)pSrc;
    size_t d = nLen % 3;
    std::string str64(4 * (int(d > 0) + nLen / 3), '=');
    int size = 4 * (int(d > 0) + nLen / 3);

    for (size_t i = 0, j = 0; i < nLen - d; i += 3)
    {//8 bit chars o 24-bit
        int n = int(pSource[i]) << 16 
                | int(pSource[i + 1]) << 8 
                | pSource[i + 2];
        //sparated into 4bit numbers
        str64[j++] = pBase64CharSet[n >> 18];
        str64[j++] = pBase64CharSet[n >> 12 & 0x3F];
        str64[j++] = pBase64CharSet[n >> 6 & 0x3F];
        str64[j++] = pBase64CharSet[n & 0x3F];
    }
    if (d--)
    {
        //base 64 char list
        int n = d ? int(pSource[nLen - 2]) << 8 
            | pSource[nLen - 1] : pSource[nLen - 1];
        str64[size - 2] = d ? pBase64CharSet[(n & 0xF) << 2] : '=';
        str64[size - 3] = d ? pBase64CharSet[n >> 4 & 0x03F] : pBase64CharSet[(n & 3) << 4];
        str64[size - 4] = d ? pBase64CharSet[n >> 10] : pBase64CharSet[n >> 2];
    }

    outStr = str64;
    //strcpy(szDest, str64.c_str());  
}

/*
char* CBase64EncodeDecode::ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn, int sizein, int* sizeout)
{
  LPSTR pszOut = NULL;
  *sizeout=0;
  if (lpwszStrIn != NULL)
  {
	int nInputStrLen = sizein;

	// Double NULL Termination
	int nOutputStrLen = WideCharToMultiByte (CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
	pszOut = new char [nOutputStrLen];

	if (pszOut)
	{
	  memset (pszOut, 0x00, nOutputStrLen);
	  WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
	}
    *sizeout=nOutputStrLen-2;
  }
  return pszOut;
}
*/

/*
CString CBase64EncodeDecode::DecodeString(CString csEncodedData)
{
    int nBufferLength = csEncodedData.GetLength();
    LPTSTR sBuffer = csEncodedData.GetBuffer(nBufferLength);
    
    //need to convert to ANSI for Base64 Encoding
    char *myANSIdata;
    int myANSIDataSize = 0;
#ifndef _CPPUNIT
    int nBufferSize=_tcslen (sBuffer);
#else
    int nBufferSize = nBufferLength;
#endif
#ifdef _UNICODE

    myANSIdata = ConvertLPWSTRToLPSTR(sBuffer,nBufferSize,&myANSIDataSize);
#else 
    myANSIdata = new char[nBufferLength+1];
    myANSIDataSize = nBufferLength;
    strcpy(myANSIdata, sBuffer);
#endif

    int nDecodedSize=4*nBufferLength*sizeof(TCHAR);
    char *buffer = new char[nDecodedSize];
    memset(buffer,0,nDecodedSize);
    Base64Decode(myANSIdata, myANSIDataSize, buffer);

    CString csDecodedString = buffer;
    csEncodedData.ReleaseBuffer();

    delete [] buffer;
    delete [] myANSIdata;

    return csDecodedString;
}
*/

void CBase64EncodeDecode::Base64Decode(const void* pSrc, const size_t nLen, std::string & outStr)
{
    unsigned char* pEncodedStr = (unsigned char*)pSrc;
    int pad = nLen > 0 && (nLen % 4 || pEncodedStr[nLen - 1] == '=');
    const size_t L = ((nLen + 3) / 4 - pad) * 4;
    //std::string str(L / 4 * 3 + pad, '\0');
    std::string str; 
    const size_t length = 3 *((nLen + 3) / 4);
    str.resize(length);
    char* out = &str[0];

    size_t i;
    size_t j;
    for (i = 0, j = 0; i < L; i += 4)
    {
        int n = nBase64Index[pEncodedStr[i]] << 18 
                | nBase64Index[pEncodedStr[i + 1]] << 12 
                | nBase64Index[pEncodedStr[i + 2]] << 6 
                | nBase64Index[pEncodedStr[i + 3]];
        out[j++] = n >> 16;
        out[j++] = n >> 8 & 0xFF;
        out[j++] = n & 0xFF;
    }
    
    if (pad)
    {
        int n = nBase64Index[pEncodedStr[L]] << 18 | nBase64Index[pEncodedStr[L + 1]] << 12;
        out[j++] = n >> 16; 

        if (nLen > L + 2 && pEncodedStr[L + 2] != '=')
        {
            n |= nBase64Index[pEncodedStr[L + 2]] << 6;
            out[j++] = n >> 8 & 0xFF;
        }
    }
    str.resize(j);
    outStr = str;
    //strcpy(szDest, str.c_str());
}

/*
CString CBase64EncodeDecode::GetBase64EncodeWithPrefix(const CString csPrefix, const CString csSource)
{
    CString csEncodeString(EncodeString(csSource));

    return csPrefix + csEncodeString;
}
*/

/*
CString CBase64EncodeDecode::GetBase64DecodeWithPrefix(const CString csPrefix, const CString csEncodedString)
{
    CString csDecoded(csEncodedString);
    csDecoded.Replace(csPrefix,_T(""));

    return DecodeString(csDecoded);
}
*/