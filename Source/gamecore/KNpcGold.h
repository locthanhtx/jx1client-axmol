//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcGold.h
// Date:	2003.07.23
// Code:	�߳�����
// Desc:	KNpcGold Class
//---------------------------------------------------------------------------

#ifndef _KNPCGOLD_H
#define	_KNPCGOLD_H

#include "engine/KLuaScript.h"

#define	defMAX_NPC_GOLD_TYEP	30  //����NPC����������

class KNpcGold
{
	friend class KNpc;
private:
	int		m_nNpcIdx;
	int		m_nIsGold;			// �Ƿ��ǻƽ������
	int		m_nIsGolding;		// ��ǰ�Ƿ��ǻƽ����
	int		m_nGoldType;		// �����ǰ�ǻƽ����仯������

// ���ݵ� npc ����
	unsigned long	m_dwSkill5ID;				// ������ת�ɵ�id  SkillString2Id()
	int		m_nSkill5Level;				// ���ܵȼ�
	int		m_nFireResist;				// ����
	int		m_nFireResistMax;			// �������ֵ
	int		m_nColdResist;				// ������
	int		m_nColdResistMax;			// ���������ֵ
	int		m_nLightingResist;			// �翹��
	int		m_nLightingResistMax;		// �翹�����ֵ
	int		m_nPoisonResist;			// ������
	int		m_nPoisonResistMax;			// ���������ֵ
	int		m_nPhycicsResist;			// ��������
	int		m_nPhycicsResistMax;		// �����������ֵ
//////////////////////////////////////////////////////
	/*int		m_nAiMode;
	int		m_nAiParam1;
	int		m_nAiParam2;
	int		m_nAiParam3;
	int		m_nAiParam4;
	int		m_nAiParam5;
	int		m_nAiParam6;
	int		m_nAiParam7;
	int		m_nAiParam8;
	int		m_nAiParam9;
	int		m_nAiParam10;
	int		m_nAiMaxTime;
	int		m_nPhysicalDamageBase;
	int		m_nPhysicalMagicBase;
	int		m_nPoisonDamageBase;
	int		m_nPoisonMagicBase;
	int		m_nColdDamageBase;
	int		m_nColdMagicBase;
	int		m_nFireDamageBase;
	int		m_nFireMagicBase;
	int		m_nLightingDamageBase;
	int		m_nLightingMagicBase;
	//char	m_nDeathScript[128]; //�����ű�
	//char	m_nDropRate[128]; //����
	//int   m_nRate;
	*/
	int     GetGoldNpcLevelDataFromScript(KLuaScript * pScript, char * szDataName, int nLevel, char * szParam,int nSeries);
public:
	void	Init(int nIdx);
	void	SetGoldType(int bFlag);	// �趨�Ƿ�Ϊ�ƽ����
	void	SetGoldTypeAndBackData();	// �趨����Ϊ�ƽ���ͬʱ������Ӧ����
	void	SetBossTypeAndBackData(int nBoss);
	void	RecoverBackData();			// �ָ������ݣ������ͨnpc
	void	RandChangeGold(int bGold = 0,int BossType=0,int nSubWorld=0);			// ��һ�����ʱ�ɻƽ����
	int		GetGoldType();				// ��õ�ǰ�ƽ����ͣ�0 �ǻƽ��
	void	SetGoldCurrentType(int nType,int nSubWorld=0);
	void	ClientClearState();

};

struct	KNpcGoldTemplateInfo
{
// ���������仯���ɷ���ָ�
	int		m_nExp;						// ����
	int		m_nLife;					// ����
	int		m_nLifeReplenish;			// ��Ѫ
	int		m_nAttackRating;			// ����
	int		m_nDefense;					// ����
	int		m_nMinDamage;
	int		m_nMaxDamage;

// �ӼӼ����Ķ������ɷ���ָ�
	int		m_nTreasure;				// װ��
	int		m_nWalkSpeed;
	int		m_nRunSpeed;
	int		m_nAttackSpeed;
	int		m_nCastSpeed;

// ��Ҫ���ݵĶ��������ɷ���ָ�
	int	    m_dwSkill5ID;				// ������ת�ɵ�id  SkillString2Id()
	//char	m_szSkill5Level[32];		// ���ܵȼ�
	int 	m_szSkill5Level;		    // ���ܵȼ�
	int		m_nFireResist;				// ����
	int		m_nFireResistMax;			// �������ֵ
	int		m_nColdResist;				// ������
	int		m_nColdResistMax;			// ���������ֵ
	int		m_nLightingResist;			// �翹��
	int		m_nLightingResistMax;		// �翹�����ֵ
	int		m_nPoisonResist;			// ������
	int		m_nPoisonResistMax;			// ���������ֵ
	int		m_nPhycicsResist;			// ��������
	int		m_nPhycicsResistMax;		// �����������ֵ
//////////////////////////////////////////////////////
	/*int		m_nAiMode;
	int		m_nAiParam1;
	int		m_nAiParam2;
	int		m_nAiParam3;
	int		m_nAiParam4;
	int		m_nAiParam5;
	int		m_nAiParam6;
	int		m_nAiParam7;
	int		m_nAiParam8;
	int		m_nAiParam9;
	int		m_nAiParam10;
	int		m_nAiMaxTime;
	int		m_nPhysicalDamageBase;
	int		m_nPhysicalMagicBase;
	int		m_nPoisonDamageBase;
	int		m_nPoisonMagicBase;
	int		m_nColdDamageBase;
	int		m_nColdMagicBase;
	int		m_nFireDamageBase;
	int		m_nFireMagicBase;
	int		m_nLightingDamageBase;
	int		m_nLightingMagicBase;
	//char	m_nDeathScript[128]; //�����ű�
	//char	m_nDropRate[128]; //����
	int     m_nRate;
	*/
};

class KNpcGoldTemplate
{
	friend class KNpcSet;
public:
	KNpcGoldTemplateInfo m_sInfo[defMAX_NPC_GOLD_TYEP];
	int	 m_nEffectTypeNum; //���ֵ��������
public:
	KNpcGoldTemplate();
	int	Init();
};
//extern 	KNpcGoldTemplateInfo *m_sInfo;
#endif
