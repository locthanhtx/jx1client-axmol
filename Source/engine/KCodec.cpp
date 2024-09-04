//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCodec.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Data Compress / Decompress Base Class
//---------------------------------------------------------------------------
#include "KMemBase.h"
#include "KCodec.h"
//---------------------------------------------------------------------------
//#include "KCodecLha.h"
#include "KCodecLzo.h"
//---------------------------------------------------------------------------
// ����:	GetPackLen
// ����:	ȡ��ѹ������������С����
// ����:	dwDataLen	��ѹ�����ݳ���
// ����:	ѹ������������С����(in Bytes)
// ע��:	�п���ѹ��������ݱ�Դ���ݶ�, ������Ҫ�õ��������������С����
//---------------------------------------------------------------------------
uint32_t KCodec::GetPackLen(uint32_t dwDataLen)
{
	return (dwDataLen + dwDataLen / 10 + 1024);
}
//---------------------------------------------------------------------------
// ����:	Encode
// ����:	ѹ��
// ����:	pCodeInfo	ѹ�����ݽṹ
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
bool KCodec::Encode(TCodeInfo* pCodeInfo)
{
	g_MemCopyMmx(pCodeInfo->lpPack, pCodeInfo->lpData, pCodeInfo->dwDataLen);
	pCodeInfo->dwPackLen = pCodeInfo->dwDataLen;
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Decode
// ����:	��ѹ��
// ����:	pCodeInfo	ѹ�����ݽṹ
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
bool KCodec::Decode(TCodeInfo* pCodeInfo)
{
	g_MemCopyMmx(pCodeInfo->lpData, pCodeInfo->lpPack, pCodeInfo->dwPackLen);
	pCodeInfo->dwDataLen = pCodeInfo->dwPackLen;
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	g_InitCodec
// ����:	��ʼ�����롢������
// ����:	ppCodec				���롢������ָ��
//			nCompressMethod		ѹ������
// ����:	void
//---------------------------------------------------------------------------
void g_InitCodec(KCodec** ppCodec, int32_t nCompressMethod)
{
	*ppCodec = NULL;

	switch (nCompressMethod)
	{
	case CODEC_NONE:
		*ppCodec = new KCodec;
		break;
//
/*	case CODEC_LHA:
		*ppCodec = new KCodecLha;
		break;
*/
	case CODEC_LZO:
		*ppCodec = new KCodecLzo;
		break;
		
	}
}
//---------------------------------------------------------------------------
// ����:	g_FreeCodec
// ����:	�ͷű��롢������
// ����:	ppCodec				���롢������ָ��
//			nCompressMethod		ѹ������
// ����:	void
//---------------------------------------------------------------------------
void g_FreeCodec(KCodec** ppCodec, int32_t nCompressMethod)
{
	if (*ppCodec == NULL)
		return;

	switch (nCompressMethod)
	{
	case CODEC_NONE:
		{
		  delete (KCodec *)*ppCodec;
		  *ppCodec=NULL;
		}
		break;
//
//	case CODEC_LHA:
//		delete (KCodecLha *)*ppCodec;
//		break;

	case CODEC_LZO:
		{
        	delete (KCodecLzo *)*ppCodec;
			*ppCodec=NULL;
		}
		break;

	}
	*ppCodec = NULL;
}
//---------------------------------------------------------------------------

