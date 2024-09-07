//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerFaction.cpp
// Date:	2002.09.26
// Code:	�߳�����
// Desc:	PlayerFaction Class
//---------------------------------------------------------------------------

#include	"KCore.h"
//#include	"MyAssert.H"
#include	"GameDataDef.h"
#include	"KFaction.h"
#include	"KPlayerFaction.h"

#define		FACTION_NEW			"���뽭��"
//#define		FACTION_OLD			"��������"

//---------------------------------------------------------------------------
//	���ܣ����캯��
//---------------------------------------------------------------------------
KPlayerFaction::KPlayerFaction()
{
	Release();
}

//---------------------------------------------------------------------------
//	���ܣ����������Ϣ
//---------------------------------------------------------------------------
void	KPlayerFaction::Release()
{
	m_nCurFaction = -1;
	m_nFirstAddFaction = -1;
	m_nAddTimes = 0;
}

//---------------------------------------------------------------------------
//	���ܣ�������ҵ���������ȷ�������������
//---------------------------------------------------------------------------
void	KPlayerFaction::SetSeries(int nSeries)
{
	return;
/*
	if (nSeries < series_metal || nSeries >= series_num)
		return;
	int		nID;
	for (int i = 0; i < FACTIONS_PRR_SERIES; i++)
	{
		nID = g_Faction.GetID(nSeries, i);
		_ASSERT(nID >= 0);
		if (m_sSkillOpen[i].m_nID != nID)
		{
			m_sSkillOpen[i].m_nID = nID;
			m_sSkillOpen[i].m_nOpenLevel = 0;
		}
	}
*/
}

//---------------------------------------------------------------------------
//	���ܣ���������
//---------------------------------------------------------------------------
int	KPlayerFaction::AddFaction(int nSeries, char *lpszFactionName)
{
	int		nID;

	nID = g_Faction.GetID(nSeries, lpszFactionName);
	if (nID < 0)
		return FALSE;
	return AddFaction(nSeries, nID);
}

//---------------------------------------------------------------------------
//	���ܣ���������
//---------------------------------------------------------------------------
int	KPlayerFaction::AddFaction(int nSeries, int nFactionID)
{
	if (nSeries < series_metal || nSeries >= series_num)
		return FALSE;
	//if (nFactionID < nSeries * FACTIONS_PRR_SERIES || nFactionID >= (nSeries + 1) * FACTIONS_PRR_SERIES)
	if (nFactionID < -1 || nFactionID >= MAX_FACTION)
		return FALSE;

	SetSeries(nSeries);

	m_nCurFaction = nFactionID;
	m_nAddTimes++;
	if (m_nAddTimes == 1)
		m_nFirstAddFaction = nFactionID;

	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ��뿪��ǰ����
//---------------------------------------------------------------------------
void	KPlayerFaction::LeaveFaction()
{
	m_nCurFaction = -1;
}

//---------------------------------------------------------------------------
//	���ܣ����ŵ�ǰ����ĳ���ȼ��ļ���
//---------------------------------------------------------------------------
int	KPlayerFaction::OpenCurSkillLevel(int nLevel, KSkillList *pSkillList)
{
	return TRUE;
/*
	if (nLevel < 0 || nLevel >= FACTION_SKILL_LEVEL || !pSkillList)
		return FALSE;
	if (m_nCurFaction < 0)
		return FALSE;

	int		i, j;

	// �趨��ǰ���ŵȼ�
	for (i = 0; i < FACTIONS_PRR_SERIES; i++)
	{
		if (m_nCurFaction == m_sSkillOpen[i].m_nID)
		{
			m_sSkillOpen[i].m_nOpenLevel = nLevel;
			break;
		}
	}

	// �趨��Ҽ���
	for (i = 0; i <= nLevel; i++)
	{
		for (j = 0; j < FACTION_SKILLS_PER_LEVEL; j++)
		{
			if (g_Faction.m_sAttribute[m_nCurFaction].m_nSkill[i][j] <= 0)
				continue;
			pSkillList->Add(g_Faction.m_sAttribute[m_nCurFaction].m_nSkill[i][j], 0);
		}
	}

	return TRUE;
*/
}

//---------------------------------------------------------------------------
//	���ܣ���õ�ǰ������Ӫ
//---------------------------------------------------------------------------
int		KPlayerFaction::GetGurFactionCamp()
{
	if (m_nCurFaction < 0)
	{
		if (m_nAddTimes == 0)
			return camp_begin;
		else
			return camp_free;
	}
	else
	{
		if (g_Faction.GetCamp(m_nCurFaction) >= 0)
			return g_Faction.GetCamp(m_nCurFaction);
		else
			return camp_begin;
	}
}

//---------------------------------------------------------------------------
//	���ܣ���õ�ǰ����
//---------------------------------------------------------------------------
int		KPlayerFaction::GetCurFactionNo()
{
	return m_nCurFaction;
}

//---------------------------------------------------------------------------
//	���ܣ���õ�ǰ����
//---------------------------------------------------------------------------
void	KPlayerFaction::GetCurFactionName(char *lpszGetName)
{
	if (!lpszGetName)
		return;

	if (this->m_nCurFaction == -1)
	{
		if (this->m_nAddTimes == 0)
		{
			lpszGetName[0] = 0;
		}
		else
		{
			strcpy(lpszGetName,FACTION_NEW);
		}
	}
	else
	{
		strcpy(lpszGetName, g_Faction.m_sAttribute[m_nCurFaction].m_szName);
	}

	return;
}



