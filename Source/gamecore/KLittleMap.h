//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KLittleMap.h
// Date:	2003.05.08
// Code:	�߳�����
//---------------------------------------------------------------------------

#ifndef KLITTLEMAP_H
#define KLITTLEMAP_H

//#include	"../Represent/iRepresent/iRepresentShell.h"
//#include "../../Represent/iRepresent/KRepresentUnit.h"

#define	LITTLE_MAP_POINT_NUM			500
#define	LITTLE_MAP_SHOW_REGION_WIDTH	4
#define	LITTLE_MAP_SHOW_REGION_HEIGHT	4

class	KLittleMap
{
private:
	RECT		m_sMapRect;				// ����ͼ��region��Χ(���ϡ�����)
	int			m_nMapWidth;			// ��ͼ��(region, m_sMapRect.right - m_sMapRect.left + 1)
	int			m_nMapHeight;			// ��ͼ��(region, m_sMapRect.bottom - m_sMapRect.top + 1)
	BYTE		*m_pbyLoadFlag;			// ĳ��region���ϰ������������
	BYTE		**m_ppbtBarrier;		// ����region���ϰ�����

	int			m_nScreenX;				// ��Ļ��ʾ���Ͻ�����
	int			m_nScreenY;				// ��Ļ��ʾ���Ͻ�����
	int			m_nShowWidth;			// ��ʾ�����ٸ�region���ϰ�
	int			m_nShowHeight;			// ��ʾ�߶��ٸ�region���ϰ�

	KRColor		m_sBarrierColor;		// �ϰ���ɫ
	KRColor		m_sSelfColor;			// ������ɫ
	KRColor		m_sTeammateColor;		// ������ɫ
	KRColor		m_sPlayerColor;			// ���������ɫ
	KRColor		m_sFightNpcColor;		// ս��npc��ɫ
	KRColor		m_sNormalNpcColor;		// ��ͨnpc��ɫ
	int		m_bColorLoadFlag;		// ��ɫ�趨�ļ��Ƿ��Ѿ�����
	int		m_bTeammateShowFlag;	// �Ƿ���ʾ����
	int		m_bPlayerShowFlag;		// �Ƿ���ʾ�������
	int		m_bFightNpcShowFlag;	// �Ƿ���ʾս��npc
	int		m_bNormalNpcShowFlag;	// �Ƿ���ʾ��ͨnpc

	KRUPoint	m_sPoint[LITTLE_MAP_POINT_NUM];	// ���ڻ���
	int			m_nPointPos;		// ���ڻ���
	KRUShadow	m_sBigPoint;
public:
	int			m_nShowFlag;		// �Ƿ���ʾС��ͼ
public:
	KLittleMap();
	~KLittleMap();
	void	Release();
	void	Init(int nLeft, int nTop, int nRight, int nBottom);	// ��ʼ���������ڴ�ռ�
	BYTE	*GetBarrierBuf(int nX, int nY);	// ���ĳ��region���ϰ����ݵĴ洢��ַ
	void	SetHaveLoad(int nX, int nY);	// �趨������ĳ��region���ϰ�����
	void	Draw(int nX, int nY);			// ����
	void	SetTeammateShow(int bFlag);	// ������ʾ����
	void	SetPlayerShow(int bFlag);		// ���������ʾ����
	void	SetFightNpcShow(int bFlag);	// ս��npc��ʾ����
	void	SetNormalNpcShow(int bFlag);	// ��ͨnpc��ʾ����
	void	SetScreenPos(int nX, int nY);	// �趨С��ͼ����Ļ�ϵ���ʾλ��
	void	Show(int nFlag);				// С��ͼ��ʾ����
	void	SetShowSize(int nWidth, int nHeight);// �趨��ʾ���ݵĴ�С
};




#endif
