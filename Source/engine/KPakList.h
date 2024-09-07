//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPakList.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
// Modify:	Wooy(2003-9-17)
//---------------------------------------------------------------------------
#ifndef KPakList_H
#define KPakList_H
#include "cocos2d.h"

USING_NS_AX;
#include "KbugInfo.h"
#include "XPackFile.h"
//---------------------------------------------------------------------------


class KPakList
{
public:
	KPakList();
	~KPakList();
	int		Open(char* pPakListFile,int nKind=0);
	void		Close();
	//���Ұ��ڵ����ļ�
	bool		pFindElemFileA(unsigned int uId, XPackElemFileRef& ElemRef);
	//���Ұ��ڵ����ļ�
	bool		pFindElemFile(const char* pszFileName, XPackElemFileRef& ElemRef);
	//��ȡ���ڵ����ļ�
	int			ElemFileRead(XPackElemFileRef& ElemRef, void* pBuffer, unsigned uSize);
	//��ȡspr�ļ�ͷ��������spr
	SPRHEAD*	GetSprHeader(XPackElemFileRef& ElemRef, SPROFFS*& pOffsetTable);
	//��ȡ��֡ѹ����spr��һ֡������
	SPRFRAME*	GetSprFrame(int nPackIndex, SPRHEAD* pSprHeader, int nFrame,unsigned int &nSingFrameSize);

	bool		pGetFilePath(const char* pszFileName, XPackElemFileRef& ElemRef);


private:
	unsigned int FileNameToId(const char* pszFileName);
private:
	#define MAX_PAK		50   //����PAK����������
	XPackFile*			m_PakFilePtrList[MAX_PAK];
	int				m_nPakNumber;
};

extern KPakList* g_pPakList;

#endif
