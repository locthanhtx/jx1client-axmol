//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcDeathCalcExp.cpp
// Date:	2003.07.21
// Code:	�߳�����
// Desc:	KNpcDeathCalcExp Class   �������
//---------------------------------------------------------------------------

#include	"KCore.h"
#include	"KNpc.h"
#include	"KPlayerDef.h"
#include	"KPlayer.h"
#include	"KNpcDeathCalcExp.h"

void	KNpcDeathCalcExp::Init(int nNpcIdx)
{
	m_nNpcIdx = (nNpcIdx > 0 ? nNpcIdx : 0);
	memset(m_sCalcInfo, 0, sizeof(m_sCalcInfo));
	ZeroMemory(&m_sCalcInfo,sizeof(m_sCalcInfo));
}

void	KNpcDeathCalcExp::Active()
{
	if (Npc[m_nNpcIdx].m_Kind != kind_normal)
		return;

	for (int i = 0; i < defMAX_CALC_EXP_NUM; ++i)
	{
		if (m_sCalcInfo[i].m_nAttackIdx <= 0 || m_sCalcInfo[i].m_nAttackIdx>=MAX_PLAYER)
			continue;

		int nDistance = KNpcSet::GetDistanceSquare(m_nNpcIdx, Player[m_sCalcInfo[i].m_nAttackIdx].m_nIndex);
		int nSerDistance = 400;
		g_GameSetTing.GetInteger("SkillsIcon","ExpDisDistance",400,&nSerDistance);
		if  (nDistance>nSerDistance)
		{//������ߵľ��뷶Χ ���� 800 ��������
			m_sCalcInfo[i].m_nTime = 0;
			m_sCalcInfo[i].m_nAttackIdx = 0;     //����������
			m_sCalcInfo[i].m_nTotalDamage = 1;   //������ʧ
			continue;
		}

		m_sCalcInfo[i].m_nTime--;               //������ʱ��

		if (m_sCalcInfo[i].m_nTime <= 0)
		{
			m_sCalcInfo[i].m_nTime = 0;
			m_sCalcInfo[i].m_nAttackIdx = 0;     //����������
			m_sCalcInfo[i].m_nTotalDamage = 1;   //������ʧ
		}
	}
}
void KNpcDeathCalcExp::Clear()
{
	memset(m_sCalcInfo, 0, sizeof(m_sCalcInfo));
	ZeroMemory(&m_sCalcInfo,sizeof(m_sCalcInfo));
}



