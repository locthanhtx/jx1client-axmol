//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KSG_EncodeDecode.h
//  Version     :   1.0
//  Creater     :
//  Date        :   2003-6-3 10:29:43
//  Comment     :
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KSG_ENCODEDECODE_H
#define _KSG_ENCODEDECODE_H 1
#include "cocos2d.h"
USING_NS_AX;
int32_t KSG_DecodeEncode(uint32_t uSize, unsigned char *pbyBuf, uint32_t *puKey);
int32_t KSG_DecodeEncode_ASM(uint32_t uSize, unsigned char *pbyBuf, unsigned *puKey);
int32_t KSG_DecodeEncode_New(uint32_t uSize, unsigned char *pbyBuf, uint32_t *puKey);

inline int32_t KSG_DecodeBuf(uint32_t uSize, unsigned char *pbyBuf, unsigned *puKey)
{
	if (!uSize) return 1;
	if (!pbyBuf) return 1;
	if (!puKey) return 1;

	unsigned uKey = *puKey;
    return KSG_DecodeEncode(uSize,pbyBuf,puKey);

    //return KSG_DecodeEncode_ASM(uSize, pbyBuf, &uKey);
	//return KSG_DecodeEncode_New(uSize, pbyBuf, puKey);
	//return 1;
}

inline int32_t KSG_EncodeBuf(uint32_t uSize, unsigned char *pbyBuf, unsigned *puKey)
{
	if (!uSize) return 1;
	if (!pbyBuf) return 1;
	if (!puKey) return 1;

	unsigned uKey = *puKey;

    return KSG_DecodeEncode(uSize, pbyBuf, puKey);
    //return KSG_DecodeEncode_ASM(uSize, pbyBuf, &uKey);
	//return KSG_DecodeEncode_New(uSize, pbyBuf, puKey);
}



#endif  // _KSG_ENCODEDECODE_H
