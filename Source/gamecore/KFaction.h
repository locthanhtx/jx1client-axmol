//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KFaction.h
// Date:	2002.09.26
// Code:	�߳�����
// Desc:	Faction Class
//---------------------------------------------------------------------------

#ifndef KFACTION_H
#define KFACTION_H

#include "GameDataDef.h"

#define		FACTIONS_PRR_SERIES				2			// ����ÿ��ϵ��������
#define		MAX_FACTION						(FACTIONS_PRR_SERIES * series_num+1)	// �ܵ�������

class KFaction
{
public:
	struct	SFactionAttirbute							// ÿ�����ɵ�����
	{
		int		m_nIndex;								// ���
		int		m_nSeries;								// ��������
		int		m_nCamp;								// ��Ӫ
		char	m_szName[64];							// ������
	}m_sAttribute[MAX_FACTION];			                // ��������

public:
	KFaction();
	~KFaction();

	int			Init();								// ��ʼ������������˵���ļ�
	int				GetID(int nSeries, int nNo);		// �����������Ժͱ����Եڼ������ɵõ����ɱ��
	int				GetID(int nSeries, char *lpszName);	// �����������Ժ��������õ����ɱ��
	int				GetCamp(int nFactionID);			// ���ĳ�����ɵ���Ӫ
};

extern	KFaction	g_Faction;
#endif
