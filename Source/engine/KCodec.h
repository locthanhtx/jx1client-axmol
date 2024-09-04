//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCodec.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KCodec_H
#define KCodec_H
#include "cocos2d.h"

USING_NS_AX;
//---------------------------------------------------------------------------
#define CODEC_NONE		0
#define CODEC_LHA		1
#define CODEC_LZO		2
//---------------------------------------------------------------------------
typedef struct {
	unsigned char *		lpData;			// ��ѹ��&ѹ��ǰ ����ָ��
	uint32_t		dwDataLen;		// ��ѹ��&ѹ��ǰ ���ݳ���
	unsigned char *	    lpPack;			// ѹ����&ѹ���� ����ָ��
	uint32_t		dwPackLen;		// ѹ����&ѹ���� ���ݳ���
} TCodeInfo;
//---------------------------------------------------------------------------
class  KCodec
{
public:
	 uint32_t	GetPackLen(uint32_t dwDataLen);
	 bool	Encode(TCodeInfo* pCodeInfo);
	 bool	Decode(TCodeInfo* pCodeInfo);
	//lpData;dwDataLen;lpPack;������ȷ���ã�dwPackLen��Ϊ0����
};
//---------------------------------------------------------------------------
 void	g_InitCodec(KCodec** ppCodec, int32_t nCompressMethod);
 void	g_FreeCodec(KCodec** ppCodec, int32_t nCompressMethod);
//---------------------------------------------------------------------------
#endif
