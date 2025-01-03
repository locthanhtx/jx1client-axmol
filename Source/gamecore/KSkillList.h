#ifndef	KSkillListH
#define	KSkillListH
//#include "Windows.h"
#include "GameDataDef.h"

#define	MAX_NPCSKILL	80
#define	MAX_TRAPNUM		5

/*typedef struct
{
	int		nPlace;
	int		nX;     // 0 1 2 3 4
	int		nY;     // 0123456789
} SkillPos;*/

typedef struct tagNpcSkill
{
	int		SkillId;
	int		SkillLevel;         //��λ�õ�ʵ�ʵȼ�����װ�� ���ܵĸ���ֵ��
	int		MaxTimes;
	int		RemainTimes;
	unsigned long	NextCastTime;		//�´οɷ���ʱ����Сʱ��
	unsigned long	NextHorseCastTime;  //�´�����ɷ���ʱ����Сʱ��
	int		CurrentSkillLevel;  //��λ�õ�ǰ���ܵĵȼ������� װ�� ���ܵĵȼ�����ֵ��
	int		mAddPoint;          //��λ�õĶ��⼼�ܵ� ��װ�� �Լ��ܵ� ����ֵ��
	int		EnChance;           //���ܼӳɰٷֱ�
	int     nSkillStyle;        //���λ�õļ�������(���� ���� ���� �ӵ�)
	int     nEquiptLimited;     //���������λ�������������Ƶ����� -2 Ϊ������
	int     nHorseLimited;      //���ø�λ�õļ����Ƿ����������Ƶ�
	BYTE    nIsExpSkill;        //�Ƿ��������ȼ���
	int     nExpSkill;          //��λ�õ���������
	int     nCurSkillExp;       //��ǰ������ �浵
	int     nTempEnChance;      //���ݵļ���ԭʼ�ļӳ�����
} NPCSKILL, *LPNPCSKILL;

//struct TDBPlayerData;

class  KSkillList
{
public:
	int			m_nTrapNpcID[5];
	NPCSKILL	m_Skills[MAX_NPCSKILL];//�����б� �±��1��ʼ��0Ϊ��Чindex
    int			m_nNpcIndex;
public:
	KSkillList();
	~KSkillList();
	void		ReEnChance();
        int CheckNoSkill(const char* nKey, int nSkillidx);
	int			Add(int nSkillID, int nLevel = 1, int nMaxTimes = 0, int RemainTimes = 0, int nAdd = 0,int nCurExpSkill=0,int nExpSkillPonit=0);//������Ӽ��� �� ʹ�ü��ܵ��õĺ���
	int		    Remove(int nSkillID);
	void        RemoveAllSkill();  //ɾ��ȫ������

	int GetSkillId(unsigned int ulSkillIdx) const
	{
		if (ulSkillIdx >= MAX_NPCSKILL || ulSkillIdx == 0)
			return 0;
		else
			return m_Skills[ulSkillIdx].SkillId;
	};

	int CheckHaveSkill(unsigned int ulSkillId) const
	{
		for (int i=1;i<MAX_NPCSKILL;i++)
		{
			if 	(m_Skills[i].SkillId==ulSkillId)
				return TRUE;
		}
		return FALSE;
	};


	int	SetSkillLevelDirectlyUsingIndex(unsigned int ulSkillIdx, unsigned int ulLevel);/*�����øú�������ΪĿǰδ���κα������ܽ�������ֵ��Ӱ��*/

	int	SetSkillLevelDirectlyUsingId(unsigned int ulSkillId, unsigned int ulLevel);/*�����øú�������ΪĿǰδ���κα������ܽ�������ֵ��Ӱ��*/
	//void SetSkillLeftTime(int nSkillId,int nTime);
	void		Clear(){memset(m_Skills, 0, sizeof(m_Skills));};

	void		SetAddPoint(int nId, int nPoint);
	void		SetSkillEnChance(int nId, int nEnChance);
	void		AddCEnChance(int nId, int nEnChance);
	void		SetSkillLevel(int nId, int nLevel);
	int		SetLevel(int nIndex, int nLevel);		// ��ĳ����ż�����Ϊĳһ��
	int			GetSkillSortList(KUiSkillData *);		// ��ý�ɫ��ǰ���м��������б�
	int			GetSkillPosition(int nSkillId);         //��ü����ڼ��ܽ����λ��
	int			GetLeftSkillSortList(KUiSkillData*);	// ��ý�ɫ��ǰ������������б�
	int			GetRightSkillSortList(KUiSkillData*);	// ��ý�ɫ��ǰ�Ҽ����������б�
//  int         Add(int nIdx, int nPlace, int nX, int nY, int bInit);

	int		IncreaseLevel(int nIdx, int nLvl, int Qeuip = FALSE,int nIsClearExp=FALSE);//�����趨�������ݣ����������ű�����
	int			GetLevel(int nSkillID);
	int         AddSkillExp(int nSkillID,int nCurExp=0,int nType=1);
	int         GetCurSkillExp(int nSkillID);
	int         GetSkillExp(int nSkillID);
    void        SetSkillExp(int nSkillID,int nExp);
	int         IsSkillExp(int nSkillID);

	int         GetLevelByListidx(int nListidx);
	int         AddSkillExpByListidx(int nListidx,int nCurExp=0,int nType=1);
	int         GetCurSkillExpByListidx(int nListidx);
	int         GetSkillExpByListidx(int nListidx);
	void        SetSkillExpByListidx(int nListidx,int nExp);
	int         IsSkillExpByListidx(int nListidx);
	int        SetSLevelByListidx(int nListidx, int nLevel,int nKind=FALSE);

	int		SetSLevel(int nSkillID, int nLevel,int nKind=FALSE);
	int			GetSkillIdxLevel(int nIdx)
	{
		if (nIdx <= 0) return 0;
		return m_Skills[nIdx].SkillLevel;
	};
	int			GetCurrentLevel(int nSkillID);
	int         GetCurrentLevelByIdx(int nListIdx);
	int			GetCount();
	int			FindSame(int nSkillID);                //���Ҹü����ڼ����б��е�λ��
	int			FindFree();

	/*
	int		CanCast(int nSkillID, unsigned long dwTime);   //���ʱ���� ���Ƿ��ܷ�����
	void		SetNextCastTime(int nSkillID, unsigned long dwTime);
	void		SetHorseNextCastTime(int nSkillID, unsigned long dwTime);
	int 		GetHorseNextCastTime(int nSkillID);
	int		    GetNextCastTime(int nSkillID);
	*/
	int		CanCast(int nSkillID, unsigned long dwTime);   //���ʱ���� ���Ƿ��ܷ�����
	int        CanCastByIndex(int nSkListIndex, unsigned long dwTime);
	void		SetNextCastTime(int nSkillID, unsigned long dwTime);
	void		SetNextCastTimeByIndex(int nSkListIndex, unsigned long dwTime);
	void		SetHorseNextCastTime(int nSkillID, unsigned long dwTime);
	void		SetHorseNextCastTimeByIndex(int nSkListIndex, unsigned long dwTime);
	int 		GetHorseNextCastTime(int nSkillID);
	int         GetHorseNextCastTimeByIndex(int nSkListIndex);
	int		    GetNextCastTime(int nSkillID);
	int		    GetNextCastTimeByIndex(int nSkListIndex);

	int			GetSkillIdByListidx(int nListIndex)
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return m_Skills[nListIndex].SkillId;
		else
			return -1;
	};
	int			GetSkillStyle(int nListIndex)
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return m_Skills[nListIndex].nSkillStyle;
		else
			return -1;
	};


	int			GetAddPoint(int nListIndex) //��ȡĳ��λ�ü��ܵĶ��⼼�ܵ㣨װ���ȣ�
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return m_Skills[nListIndex].mAddPoint;
		else
			return 0;
	};

	int			GetEnChance(int nListIndex)                 //�������˵�ǰ�����ܵļ��ܼӳ�
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return m_Skills[nListIndex].EnChance;
		else
			return 0;
	};

	int			GetTempEnChance(int nListIndex)                 //�������˵�ǰ����ԭʼ���ܵļ��ܼӳ�
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			return 	m_Skills[nListIndex].nTempEnChance;
		else
			return 0;
	};

	void		QeuipAddPoint(int nListIndex,int sPoint)    //���÷���������Ҫ�������ӻ���ٵļ��ܵ�
	{
			m_Skills[nListIndex].mAddPoint += sPoint;
	};

	void		SetEnChance(int nListIndex,int sEnChance)   //ֱ�����ü��ܼӳɵ�����
	{
		if (nListIndex < MAX_NPCSKILL && nListIndex > 0)
			m_Skills[nListIndex].EnChance = sEnChance;
		else
			m_Skills[nListIndex].EnChance = 0;

		if (m_Skills[nListIndex].EnChance<0)
		{
			m_Skills[nListIndex].EnChance=0;
			m_Skills[nListIndex].nTempEnChance = 0;
		}
	};
	void		AddEnChance(int nListIndex,int sEnChance)   //�������˵�ǰ�����ܼ��ܼӳɵ�����
	{
		    m_Skills[nListIndex].EnChance += sEnChance;

			if (m_Skills[nListIndex].EnChance<0)
			{
				m_Skills[nListIndex].EnChance=0;
				m_Skills[nListIndex].nTempEnChance =0;
			}

	};
};
#endif
