#ifndef	KNpcSetH
#define	KNpcSetH
#include "engine/KbugInfo.h"
#include "engine/KLinkArray.h"
#include "KNpc.h"
#include "GameDataDef.h"
#include "KNpcGold.h"
#include <map>

#define		MAX_NPC_REQUEST	200   //����NPC ��������

class KNpcTemplate;

#define		MAX_INSTANT_STATE	20
#define		MAX_INSTANT_SOUND	30
class	KInstantSpecial
{
private:
	int		m_nLoadFlag;
	char	m_szSprName[MAX_INSTANT_STATE][FILE_NAME_LENGTH];
	char	m_szSoundName[MAX_INSTANT_SOUND][FILE_NAME_LENGTH];

	KCacheNode	*m_pSoundNode;	// ��Чָ��
//	KWavSound	*m_pWave;		// ��Чwavָ��

private:
	void	LoadSprName();
	void	LoadSoundName();

public:
	KInstantSpecial();
	void	GetSprName(int nNo, char *lpszName, int nLength);
	void	PlaySound(int nNo);
};

typedef struct
{
	DWORD	dwRequestId;
	DWORD	dwRequestTime;
} RequestNpc;

typedef struct
{
	int		nStandFrame[2];
	int		nWalkFrame[2];
	int		nRunFrame[2];
	int		nWalkSpeed;
	int		nRunSpeed;
	int		nAttackFrame;
	int		nHurtFrame;
} PlayerBaseValue;

class KNpcSet
{
public:
	KNpcGoldTemplate	m_cGoldTemplate;					// �ƽ����ģ��
//	KNpcPartnerTemplate	m_cPartnerTemplate;					// ͬ��ģ��
	PlayerBaseValue		m_cPlayerBaseValue;					// ��ұ�׼����

	KInstantSpecial		m_cInstantSpecial;

private:
	DWORD				m_dwIDCreator;						//	��Ϸ�����е�DWID��
	KLinkArray			m_FreeIdxNpcSet;					//	���ñ�
	KLinkArray			m_UseIdxNpcSet;						//	���ñ�
	//int                 m_nListCurIdx;
	typedef std::map<DWORD, KNpcTemplate*> _KMapTemplate;
	//INT				m_anTemplateRowId[MAX_NPCSTYLE + 1];
	_KMapTemplate		m_mapTemplate;
	//KTabFile			m_cTabFile;
	enum
	{
		PATE_CHAT = 0x01,
		PATE_NAME = 0x02,
		PATE_LIFE = 0x04,
		PATE_MANA = 0x08,
	};
	int					m_nShowPateFlag;					// �Ƿ�ȫ����ʾ��ҵ�������ͷ���� zroc add
	RequestNpc			m_RequestNpc[MAX_NPC_REQUEST];		//	������������ID��
	KLinkArray			m_RequestFreeIdx;					//  �������������б�
	KLinkArray			m_RequestUseIdx;					//	�������������ñ�
public:
	KNpcSet();
	void			Init();
	virtual         BOOL Release(void);
	int				GetNpcCount(int nKind = -1, int nCamp = -1);
	int				SearchName(LPSTR szName);
	int             SearchNameInMap(LPSTR szName,int inMapidx,int nType=0);
	int             SearchGSNameInMap(LPSTR szName,int inMapidx);
	int				SearchNpcSettingID(LPSTR szName);
	int				SearchByTongName(LPSTR szName);  //�������õ�
	int				SearchID(DWORD dwID);            //�������õ�
	int             DelNpcInNpcSet(int nNpcSettings);
	int             SearchGSName(LPSTR szName);
	int				SearchNameID(DWORD dwID);        //������ҵ�
	BOOL			IsNpcExist(int nIdx, DWORD dwId);
	int				AddServerNpcB(int nNpcSetingIdxInfo, int nSubWorld, int nRegion, int nMapX, int nMapY, int nOffX = 0, int nOffY = 0,int nBoss=0);
	int				AddServerNpcA(int nNpcSetingIdxInfo, int nSubWorld, int nMpsX, int nMpsY,int nBoss=0,int nRodom=0,int nOffwminx=1,int nOffwmaxx=1,int nOffhminy=1,int nOffhmaxy=1,int nNpcKind=-1,unsigned int nNpcDwidx=0);
	int				AddMapNpc(int nSubWorld, void* pNpcInfo);
    int				Addyabiao(int nNpcSetingIdxInfo, int nSubWorld, int nMpsX, int nMpsY, int nSeries, IN int nComp,IN int nRindexid,int nBoss=0);
    int	            AddTongBan(int nNpcSettingIdxInfo, int nSubWorld, int nMpsX, int nMpsY, IN int nSeries, IN int nComp,IN int nRindexid,int nBoss=0);
	void			Remove(int nIdx,BOOL isNeedSyn = TRUE);
	void			RemoveAll();
	NPC_RELATION	GetRelation(int nIdx1, int nIdx2);
	int             GetNPCBaiTan(int nIdex);
	int				GetNearestNpc(int nMapX, int nMapY, int nId, int nRelation);
	static int		GetDistance(int nIdx1, int nIdx2);
	static int		GetDistanceSquare(int nIdx1, int nIdx2);
	static INT	    GetMapDisX(INT nIdx1, INT nIdx2);
	static INT	    GetMapDisY(INT nIdx1, INT nIdx2);

	int				GetNextIdx(int nIdx);
	// ������npc�� bActivateFlag ��Ϊ FALSE (ÿ����Ϸѭ����������npc��activate֮ǰ���������)
	void			ClearActivateFlagOfAllNpc();
	void			LoadPlayerBaseValue(LPSTR szFile);
	int				GetPlayerWalkSpeed() { return m_cPlayerBaseValue.nWalkSpeed; };
	int				GetPlayerRunSpeed() { return m_cPlayerBaseValue.nRunSpeed; };
	int				GetPlayerAttackFrame() { return m_cPlayerBaseValue.nAttackFrame; };
	int				GetPlayerHurtFrame() { return m_cPlayerBaseValue.nHurtFrame; };
	int             CheckForBit(int *nVal,char* mVal,int Model=1);
	BOOL            CheckThisNpc(int nSetings,char *nKey);

	KNpcTemplate*	GetTemplate(INT nNpcTemplateId,INT nLevel);

	int				GetPlayerStandFrame(BOOL bMale)
	{
		if (bMale)
			return m_cPlayerBaseValue.nStandFrame[0];
		else
			return m_cPlayerBaseValue.nStandFrame[1];
	};
	int				GetPlayerWalkFrame(BOOL bMale)
	{
		if (bMale)
			return m_cPlayerBaseValue.nWalkFrame[0];
		else
			return m_cPlayerBaseValue.nWalkFrame[1];
	};
	int				GetPlayerRunFrame(BOOL bMale)
	{
		if (bMale)
			return m_cPlayerBaseValue.nRunFrame[0];
		else
			return m_cPlayerBaseValue.nRunFrame[1];
	};
	BOOL			IsNpcRequestExist(DWORD	dwID);
	void			InsertNpcRequest(DWORD dwID);
	void			RemoveNpcRequest(DWORD dwID);
	int				GetRequestIndex(DWORD dwID);
	// ���һ���ͻ���npc����Ҫ�趨ClientNpcID��
	int				AddClientNpc(int nTemplateID, int nRegionX, int nRegionY, int nMpsX, int nMpsY, int nNo);
	// ��npc������Ѱ������ĳ��region�� client npc ����ӽ�ȥ
	void			InsertNpcToRegion(int nRegionIdx);
	// ����ĳ��ClientID��npc�Ƿ����
	int				SearchClientID(KClientNpcID sClientID);
	// ĳ�����Ͼ�ȷ����Npc���ͻ���ר��
	int				SearchNpcAt(int nX, int nY, int nRelation, int nRange);
	void			CheckBalance();
	int				GetAroundPlayerForTeamInvite(KUiPlayerItem *pList, int nCount);	// �����Χ����б�(���ڶ��������б�)
	void			GetAroundOpenCaptain(int nCamp);		// �����Χͬ��Ӫ���ѿ��Ŷ���ӳ��б�
	int				GetAroundPlayer(KUiPlayerItem *pList, int nCount);	// �����Χ����б�(�����б�)

	// �趨�Ƿ�ȫ����ʾ��ҵ�����  bFlag ==	TRUE ��ʾ��bFlag == FALSE ����ʾ zroc add
	void			SetShowNameFlag(BOOL bFlag);
	// �ж��Ƿ�ȫ����ʾ��ҵ�����  ����ֵ TRUE ��ʾ��FALSE ����ʾ
	BOOL			CheckShowName();
	// �趨�Ƿ�ȫ����ʾ��ҵ�����  bFlag ==	TRUE ��ʾ��bFlag == FALSE ����ʾ zroc add
	void			SetShowChatFlag(BOOL bFlag);
	// �ж��Ƿ�ȫ����ʾ��ҵ�����  ����ֵ TRUE ��ʾ��FALSE ����ʾ
	BOOL			CheckShowChat();
	// �趨�Ƿ�ȫ����ʾ��ҵ�Ѫ  bFlag ==	TRUE ��ʾ��bFlag == FALSE ����ʾ zroc add
	void			SetShowLifeFlag(BOOL bFlag);
	// �ж��Ƿ�ȫ����ʾ��ҵ�Ѫ  ����ֵ TRUE ��ʾ��FALSE ����ʾ
	BOOL			CheckShowLife();
	// �趨�Ƿ�ȫ����ʾ��ҵ�����  bFlag ==	TRUE ��ʾ��bFlag == FALSE ����ʾ zroc add
	void			SetShowManaFlag(BOOL bFlag);
	// �ж��Ƿ�ȫ����ʾ��ҵ�����  ����ֵ TRUE ��ʾ��FALSE ����ʾ
	BOOL			CheckShowMana();
private:
	void			SetID(int m_nIndex);
	int				FindFreeNpcSet();


    // Add By Freeway Chen in 2003.7.14
private:
    // [��һ��NPC.m_Kind][�ڶ���NPC.m_Kind][��һ��NPC.m_CurrentCamp][�ڶ�����NPC.m_CurrentCamp]
    //unsigned char m_RelationTable[kind_num][kind_num][camp_num][camp_num];
    // ���ɹ�ϵ��
    //int GenRelationTable();
    NPC_RELATION GenOneRelation(NPCKIND Kind1, NPCKIND Kind2, NPCCAMP Camp1, NPCCAMP Camp2);

};

// modify by Freeway Chen in 2003.7.14
// ȷ������NPC֮���ս����ϵ
extern KNpcSet NpcSet;
#endif
