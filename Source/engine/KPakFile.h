//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPakFile.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KPakFile_H
#define KPakFile_H
#include "cocos2d.h"

USING_NS_AX;
#include "KbugInfo.h"
//---------------------------------------------------------------------------
#include "KFile.h"

//#ifndef _SERVER
#include "XPackFile.h"
//#endif

//---------------------------------------------------------------------------
void g_SetPakFileMode(int nFileMode);
//---------------------------------------------------------------------------
class KPakFile
{
public:
	KPakFile();
	~KPakFile();
	int		Open(const char* pszFileName);
	void		Close();
	bool		IsFileInPak();
	unsigned long		Read(void* pBuffer, unsigned int uSize);
	unsigned long		Seek(int nOffset, unsigned int uMethod);
	unsigned long		Tell();
	unsigned long		Size();
	int		Save(const char* pszFileName);
	KFile       getFilePtr(){return m_File;};
private:
	KFile		m_File;			// ��ʵ�ļ�(���ڰ���)
	//#ifndef _SERVER
	XPackElemFileRef	m_PackRef;
	//#endif
};

//---------------------------------------------------------------------------

	struct KSGImageContent
	{
		int				nWidth;
		int				nHeight;
		unsigned short	Data[1];
	};
	#define	KSG_IMAGE_CONTENT_SIZE(w, h)    ((unsigned)((&((KSGImageContent *)0)->Data[0])) + w * h * 2)

	SPRHEAD*		SprGetHeader(char* pszFileName, SPROFFS*& pOffsetTable);
	void			SprReleaseHeader(SPRHEAD* pSprHeader);
	SPRFRAME*	SprGetFrame(SPRHEAD* pSprHeader, int nFrame,unsigned int &nSingFrameSize);
	void			SprReleaseFrame(SPRFRAME* pFrame);

	KSGImageContent *get_jpg_image(const char cszName[], unsigned uRGBMask16 = ((unsigned)-1));
	void release_image(KSGImageContent *pImage);

#endif
