//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCodec.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef Kgetinidata_H
#define Kgetinidata_H
#include "cocos2d.h"
#include "KbugInfo.h"
#include "ImageStore/KImageStore2.h"
//#include "KPalette.h"
USING_NS_AX;
//---------------------------------------------------------------------------
//#define CODEC_NONE	0
//#define CODEC_LHA		1
//#define CODEC_LZO		2
//---------------------------------------------------------------------------
typedef struct {
	unsigned char *		lpData;			// ��ѹ��&ѹ��ǰ ����ָ��
	uint32_t		dwDataLen;		// ��ѹ��&ѹ��ǰ ���ݳ���
	unsigned char *	    lpPack;			// ѹ����&ѹ���� ����ָ��
	uint32_t		dwPackLen;		// ѹ����&ѹ���� ���ݳ���
}TSprCodeInfo;

struct Palette_Colour
{
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Alpha;
};

typedef struct {
	int32_t		x;			// �ü����X����
	int32_t		y;			// �ü����Y����
	int32_t		width;		// �ü���Ŀ��
	int32_t		height;		// �ü���ĸ߶�
	int32_t		left;		// ��߽�ü�����
	int32_t		top;		// �ϱ߽�ü���
	int32_t		right;		// �ұ߽�ü���
} KClipper;
//---------------------------------------------------------------------------
class  Kgetinidata
{
public:
	Kgetinidata(void);
	virtual ~Kgetinidata(){};
	 uint32_t	GetLen(uint32_t dwDataLen);
	// bool	SprEncode(TSprCodeInfo* pCodeInfo);
	// bool	SprDecode(TSprCodeInfo* pCodeInfo);
	 Texture2D * getinidata_new(char *nFileName, int32_t nFrameIndex, int32_t *mCurwidth, int32_t *mCurheight, int32_t *mFrams);
	 Texture2D * getinidata_one(char *nFileName, int32_t nFrameIndex, int32_t *mCurwidth, int32_t *mCurheight, int32_t *mFrams, SPRFRAMSINFO *nSprInfo=NULL);
	 Texture2D * getpicPath(char *picPath);
	 //lpData;dwDataLen;lpPack;������ȷ���ã�dwPackLen��Ϊ0����
	 //bool   SaveBMPFile(const char *filename,  const stImageInfo& m_info);
private:

	SPRHEAD     * pHeader;
	BYTE*	 	m_Palette;
	KPAL16*		m_pPal16;
	SPROFFS* 	m_pOffset;
	SPRFRAME*   nFrameData;
	PBYTE		m_pSprite;
	int32_t			m_nWidth;
	int32_t			m_nHeight;
	int32_t	        m_nCenterX;
	int32_t	        m_nCenterY;
    int32_t	m_nOffsetX;
    int32_t	m_nOffsetY;
	uint32_t	m_nFrames;
	int32_t			m_nColors;
	uint32_t	m_nDirections;
	int32_t			m_nInterval;
	unsigned char *data;
	uint32_t m_size;
	int32_t          m_ReadModel;
	int32_t  GetGreaterNear2Fold(int32_t d);
	PBYTE RevertRowRGBA(PBYTE data, int32_t &width, int32_t &height);
	bool ReadFileData(char *nFileName,int32_t nFrameIndex=0);
};
//---------------------------------------------------------------------------
// void	g_InitCodec(KCodec** ppCodec, int32_t nCompressMethod);
// void	g_FreeCodec(KCodec** ppCodec, int32_t nCompressMethod);
//---------------------------------------------------------------------------
#endif
