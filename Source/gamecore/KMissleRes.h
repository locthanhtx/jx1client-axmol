/*******************************************************************************
// FileName			:	KMissleRes.h
// FileAuthor		:	RomanDou
// FileCreateDate	:	2002-7-8 16:21:51
// FileDescription	:
������Ҫ�Ĺ�����
һ��ά���ӵ�����Դ������ͼ����������Դ�ļ��ء�ɾ��
����ά���ӵ��Ļ�֡����֡��ͬ������
���������ӵ���ͼ����ʾ���������ŵ�����
// Revision Count	:
*******************************************************************************/

#ifndef __KMISSLERES_H__
#define __KMISSLERES_H__
#include "engine/KEngine.h"
#include "KCore.h"
#include "SkillDef.h"
#include "KRUImage.h"
//#include "../../Represent/iRepresent/KRepresentUnit.h"

class KShadowNode :public KNode
{
public:
	int X;
	int Y;
	int Frame;
	int Alpha;
};

typedef struct
{
	char	AnimFileName[128];							// ͼ��spr �ļ���
	int		nTotalFrame;
	int		nInterval;
	int		nDir;
	char	SndFileName[128];							// ����wav �ļ���
} TMissleRes;


class KMissleRes
{
public:
	KMissleRes(	);
	~KMissleRes();
	KList		m_SkillSpecialList;		//	�ӵ�����Ч���б�

	int		m_bHaveEnd;
	int		m_bLoopAnim;											//�Ƿ�ѭ�����Ŷ���
	TMissleRes	m_MissleRes[MAX_MISSLE_STATUS * 2];							//����״̬�µ���Դ���
	KCacheNode  * m_pSndNode ;
	int			m_nLastSndIndex;
	int			m_nMissleId;
	KList		m_ShadowList;
	int			m_nMaxShadowNum;		//	������Ĳ�Ӱ��
	int		m_bNeedShadow;
	int			m_nLastShadowLifeTime;
	//int			m_nLifePerFrame;		//	ÿ֡����ʱ��

	int		m_bSubLoop;
	int			m_nSubStart;			//	��ѭ������ʼ֡
	int			m_nSubStop;			//	��ѭ���Ľ���֡
	KRUImage	m_RUImage[MAX_MISSLE_STATUS];			//	ͼ
	//int			m_SceneID;
#ifdef TOOLVERSION
	KCacheNode * m_pSprNode;
#endif
public:
	int		Init();
	void		Remove();
	void		Clear();
	void		LoadResource(int nStatus, char * MissleImage, char * MissleSound);
	int			Draw(int nStatus, int nX, int nY , int nZ, int nDir,  int nAllTime, int nCurLifeTime);
	void		PlaySound(int nStatus, int nX, int nY, int nLoop);
	int			GetSndVolume(int nVol);
	void		StopSound();
	void		NextFrame();
	int		SpecialMovieIsAllEnd()
	{
		if (m_SkillSpecialList.GetHead())
		{
			return FALSE;
		}

		return TRUE;
	};
};

#endif //__KMISSLERES_H__
