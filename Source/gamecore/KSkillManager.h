#ifndef KSKILLMANAGER_H
#define KSKILLMANAGER_H

#include "KCore.h"
/*#ifndef _STANDALONE
#include "crtdbg.h"
#endif*/
#include "KSkills.h"
typedef eSKillStyle eSkillStyle ;

class KSkillManager
{
    friend class KSkill;

	typedef struct
	{
		int m_nTabFileRowId;
		int m_nSkillStyle;
		unsigned int	m_ulFirstLoadLevel;
		unsigned int	m_ulMaxSkillLevel;
	}	TSkillInfo;

private:

	TSkillInfo m_SkillInfo[MAX_SKILL];
	//TOrdinSkillCommonData * m_pOrdinSkillCommonData[MAX_SKILL];
	ISkill*			m_pOrdinSkill[MAX_SKILL][MAX_SKILLLEVEL];//

private:
	ISkill*	InstanceSkill( unsigned int ulSkillID, unsigned int ulSkillLevel);
	int		GetSkillStyle(unsigned int ulSkill);

	int   nSkillNum;

public:
	KSkillManager();
    // Add By Freeway Chen in 2003.6.6
	~KSkillManager();

	int Init();
	ISkill *GetSkill(int nSkill, int nSkillLevel);
	ISkill *GetSkillB(int nSkillID, int nSkillLevel, eSkillStyle eStyle);
	int    GetSkillStyleID(int nSkillID){return GetSkillStyle(nSkillID);};
	int    GetSkillCount(){return nSkillNum;};

	unsigned int	GetSkillMaxLevel(int nSkill)
	{
		if (nSkill<= 0 ) return 0;

		if (nSkill <= MAX_SKILL)
			return m_SkillInfo[nSkill - 1].m_ulMaxSkillLevel;
		else
			return 0;
	};

};


inline int KSkillManager::GetSkillStyle(unsigned int ulSkillID)
{
	if (m_SkillInfo[ulSkillID - 1].m_nTabFileRowId > 0)
		return m_SkillInfo[ulSkillID - 1].m_nSkillStyle;
	else
		return -1;
}

inline ISkill * KSkillManager::GetSkillB(int nSkillID, int nSkillLevel, eSkillStyle eStyle)
{
	ISkill * pSkill = GetSkill(nSkillID, nSkillLevel);
	if (pSkill)
	{
		if (pSkill->GetSkillStyle() == eStyle)
			return pSkill;
	}

	return NULL;
}


inline ISkill *KSkillManager::GetSkill(int nSkillID, int nSkillLevel)
{
//	_ASSERT((nSkillID <= MAX_SKILL)    && (nSkillID > 0));
//	_ASSERT((nSkillLevel <= MAX_SKILLLEVEL) && (nSkillLevel > 0));

	if (nSkillID >= MAX_SKILL || nSkillID <= 0 || nSkillLevel <= 0 || nSkillLevel >=MAX_SKILLLEVEL)
		return NULL;

	if (nSkillID>GetSkillCount())
		return NULL;

    unsigned int ulSkillID    = nSkillID;
    unsigned int ulSkillLevel = nSkillLevel;

   //�Ѿ��� InstanceSkill �������˴���
    if (m_pOrdinSkill[ulSkillID - 1][ulSkillLevel - 1])
    {//�������ԭʼ�ļ�������
        return m_pOrdinSkill[ulSkillID - 1][ulSkillLevel - 1];
    }

	return InstanceSkill(ulSkillID , ulSkillLevel); //���·����ڴ棬�������ܽű�����
}

extern KSkillManager g_SkillManager;
#endif
