#ifndef KPlayerSetH
#define	KPlayerSetH

#include "engine/KLinkArray.h"

#define		MAX_LEVEL					500		// ������ȼ�
#define		MAX_SERIES					5
#define		MAX_LEAD_LEVEL				100		// ���ͳ�����ȼ�

typedef struct
{
	int		m_nExp;
	int		m_nMoney;
	int		m_nItem;
	int		m_nEquip;
} KPK_DEATH_PUNISH_PARAM;

typedef struct
{
	int		m_nLevel;
	int		m_nLeadLevel;
	int		m_nMoney;
} STONG_CREATE_PARAM;


class KLevelAdd
{
private:
	int		m_nLevelExp[MAX_LEVEL];				    // ��������
	int		m_nLifePerLevel[MAX_SERIES];			// ÿ��ϵ������������
	int		m_nStaminaPerLevel[MAX_SERIES];			// ÿ��ϵ������������
	int		m_nManaPerLevel[MAX_SERIES];			// ÿ��ϵ������������
	int		m_nLifePerVitality[MAX_SERIES];			// ÿ��ϵ����������һ�������������
	int		m_nStaminaPerVitality[MAX_SERIES];		// ÿ��ϵ����������һ�������������
	int		m_nManaPerEnergy[MAX_SERIES];			// ÿ��ϵ����������һ�������������
	int		m_nLeadExpShare[MAX_SERIES];			// ÿ��ϵ���ս������ʱ����ͳ��������Ĳ���
	int		m_nFireResistPerLevel[MAX_SERIES];		// ÿ��ϵ�����ӻ���
	int		m_nColdResistPerLevel[MAX_SERIES];		// ÿ��ϵ�����ӱ�����
	int		m_nPoisonResistPerLevel[MAX_SERIES];	// ÿ��ϵ�����Ӷ�����
	int		m_nLightResistPerLevel[MAX_SERIES];		// ÿ��ϵ�����ӵ翹��
	int		m_nPhysicsResistPerLevel[MAX_SERIES];	// ÿ��ϵ������������
public:
	KLevelAdd();
	int		Init();
	int			GetLevelExp(int nLevel);
	int			GetLifePerLevel(int nSeries);
	int			GetStaminaPerLevel(int nSeries);
	int			GetManaPerLevel(int nSeries);
	int			GetLifePerVitality(int nSeries);
	int			GetStaminaPerVitality(int nSeries);
	int			GetManaPerEnergy(int nSeries);
	int			GetLeadExpShare(int nSeries);
	int			GetFireResist(int nSeries, int nLevel);
	int			GetColdResist(int nSeries, int nLevel);
	int			GetPoisonResist(int nSeries, int nLevel);
	int			GetLightResist(int nSeries, int nLevel);
	int			GetPhysicsResist(int nSeries, int nLevel);
};


class KTeamLeadExp
{
public:
	struct LEADLEVEL
	{
		unsigned int	m_dwExp;								// ͳ������������
		unsigned int	m_dwMemNum;								// ͳ�ʵȼ�������ͳ�ʵĶ�Ա����
	}m_sPerLevel[MAX_LEAD_LEVEL];
public:
	KTeamLeadExp();
	int		Init();									// ��ʼ��
	int			GetLevel(unsigned int dwExp, int nCurLeadLevel = 0);// ���뾭���õȼ��� (������뵱ǰ�ȼ������Լӿ������ٶ�)
	int			GetMemNumFromExp(unsigned int dwExp);			// ���뾭���ÿɴ���Ա��
	int			GetMemNumFromLevel(int nLevel);			// ����ȼ���ÿɴ���Ա��
	int			GetLevelExp(int nLevel);				// ����ȼ�����������辭��ֵ
};

class KNewPlayerAttribute
{
public:
	int			m_nStrength[series_num];
	int			m_nDexterity[series_num];
	int			m_nVitality[series_num];
	int			m_nEngergy[series_num];
	int			m_nLucky[series_num];
public:
	KNewPlayerAttribute();
	int		Init();
};

class KPlayerSet
{
private:	// �����Ż������ٶ�
	KLinkArray		m_FreeIdxPlayerSet;				//	���ñ�
	KLinkArray		m_UseIdxPlayerSet;				//	���ñ�
	int				m_nListCurIdx;			// ���� GetFirstPlayer �� GetNextPlayer

public:
	KLevelAdd				m_cLevelAdd;
	KTeamLeadExp			m_cLeadExp;
	KNewPlayerAttribute		m_cNewPlayerAttribute;
	STONG_CREATE_PARAM		m_sTongParam;

public:
	KPlayerSet();
	int	Init();
	int		FindSame(unsigned int dwID);
	int     FindNameID(char *szName);  //����������
	int     FindByTongName(char * szTongName);  //����������
	int     CheckLiXian(char * szName);
	int     FindClient(int nClient);   //�������Ӻ� �������Playerid
	int		FindPlayerID(unsigned int dwID);
	int		GetFirstPlayer();				// ����������ҵ�һ��
	int		GetNextPlayer();				// �������������һ��(��֧������������һ֧����֮����ܵ���)
	int		GetOnlinePlayerCount() { return m_UseIdxPlayerSet.GetCount(); }
//	int		FindClient(int nClient);
private:
	int		FindFree();
};

extern KPlayerSet PlayerSet;
#endif //KPlayerSetH
