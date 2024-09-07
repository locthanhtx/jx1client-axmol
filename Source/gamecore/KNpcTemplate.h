#ifndef	_KNPCTEMPLATE_H
#define	_KNPCTEMPLATE_H

#include "KCore.h"
#include "KNpc.h"

//#include "KBinTreeNode.h"
//#include "KBinsTree.h"
//-------------------------------------------------------------

class KNpcTemplate
{

public:
	char	Name[32];
	unsigned long	m_Kind;
	int		m_Camp;
	int		m_Series;
	int		m_HeadImage;
	int		m_bClientOnly;
	int		m_CorpseSettingIdx;
	int		m_DeathFrame;
	int		m_WalkFrame;
	int		m_StandFrame;
	int		m_StandFrame1;
	int		m_RunFrame;
	int		m_HurtFrame;
	int		m_WalkSpeed;
	int		m_AttackFrame;  //�⹦�����ٶ�
	int		m_CastFrame;    //�ڹ������ٶ�
	int		m_RunSpeed;
	int 	m_nLifeMax;
	int    m_bHaveLoadedFromTemplate;

	int		m_ArmorType;
	int		m_HelmType;
	int		m_WeaponType;
	int		m_HorseType;
	int		m_bRideHorse;
	int		m_PifengType;           //����
	char	ActionScript[128];
	char	m_szLevelSettingScript[128];

	int		m_NpcSettingIdx;
	int		m_nStature;
	int		m_nLevel;

public:
	int	        InitNpcBaseData(int nNpcTemplateId);
	int	        InitNpcLevelData(int nNpcTemplateId, KLuaScript * pLevelScript, int nLevel);
	static int		GetNpcLevelDataFromScript(KLuaScript * pScript, char * szDataName, int nLevel, char * szParam,int nSeries);
        static int GetNpcKeyDataFromScript(KLuaScript* pScript,
                                           const char* szDataName,
                                           int nLevel,
                                           double nParam1,
                                           double nParam2,
                                           double nParam3,
                                           int nSeries);
	static int		SkillString2Id(char * szSkillString);
     int 	        InitDropRate(int nNpcIdex,char * nDropRateFiled);
     int 	        gmUpdataDropRate(int nNpcIdex,char * nDropRateFiled);
	KNpcTemplate(){	m_bHaveLoadedFromTemplate = FALSE;};
};

/*#ifdef _SERVER
extern KItemDropRate *g_GetItemDropRate(const char* pszDropTabFile);
#endif*/

//extern  KNpcTemplate	* g_pNpcTemplate[MAX_NPCSTYLE][MAX_NPC_LEVEL]; //0,0Ϊ���




#endif

