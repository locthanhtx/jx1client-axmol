
#include "KCore.h"
#include "KSkillManager.h"
//#include "MyAssert.H"
#include "KThiefSkill.h"

unsigned int g_ulSkillCount = 0;
KSkillManager g_SkillManager;

KSkillManager::KSkillManager()
{
	 nSkillNum=0;
}

// Add By Freeway Chen in 2003.6.6
KSkillManager::~KSkillManager()
{
    unsigned int i = 0;
    unsigned int j = 0;

    for (i = 0; i < MAX_SKILL; ++i)
    {
        for (j = 0; j < MAX_SKILLLEVEL; ++j)
        {
            if (m_pOrdinSkill[i][j])
            {
                delete m_pOrdinSkill[i][j];
                m_pOrdinSkill[i][j] = NULL;
            }
        }
    }
}

int KSkillManager::Init()
{
    int i = 0;
    int j = 0;
    for (i = 0; i < MAX_SKILL; ++i)
    {
        for (j = 0; j < MAX_SKILLLEVEL; ++j)
        {
            m_pOrdinSkill[i][j] = NULL;
        }
    }

	memset(m_SkillInfo, 0,  sizeof(m_SkillInfo));

	//	Load OrdinSkill Info
	   nSkillNum = g_OrdinSkillsSetting.GetHeight();

	if (nSkillNum <= 0 )
		return FALSE;

	for (i = 0; i < nSkillNum-1; ++i)
	{
		int nSkillId = -1;
		int nSkillStyle = -1;
		int nSkillMaxLevel = 0;
		g_OrdinSkillsSetting.GetInteger(i + 2, "SkillId", -1, &nSkillId);
		g_OrdinSkillsSetting.GetInteger(i + 2, "SkillStyle", -1, &nSkillStyle);
		g_OrdinSkillsSetting.GetInteger(i + 2, "MaxLevel", 20, &nSkillMaxLevel);

		//_ASSERT(nSkillMaxLevel >= 0);
		if  (nSkillMaxLevel<0)
			nSkillMaxLevel = 0;

		if (nSkillId > 0 && nSkillStyle >= 0)
		{
			m_SkillInfo[nSkillId - 1].m_nSkillStyle = nSkillStyle;
			m_SkillInfo[nSkillId - 1].m_nTabFileRowId = i + 2;
			m_SkillInfo[nSkillId - 1].m_ulMaxSkillLevel = nSkillMaxLevel;
		}
		else
		{    //�ṹ��
			m_SkillInfo[nSkillId - 1].m_nSkillStyle = -1;
			m_SkillInfo[nSkillId - 1].m_nTabFileRowId = 0;
			m_SkillInfo[nSkillId - 1].m_ulMaxSkillLevel = 0;
		}
	}
	KTabFile ThiefSkillTab;
	if (ThiefSkillTab.Load(THIEFSKILL_SETTINGFILE))
	{
		int nSkillId = 0;
		ThiefSkillTab.GetInteger(2, "SkillId", 400, &nSkillId);   //������ƶ
		//_ASSERT(nSkillId > 0);
		if (nSkillId>0)
		{
			m_SkillInfo[nSkillId - 1].m_nSkillStyle = SKILL_SS_Thief;
			m_SkillInfo[nSkillId - 1].m_nTabFileRowId = 2;
			m_SkillInfo[nSkillId - 1].m_ulMaxSkillLevel = 1;
		}
	}
	ThiefSkillTab.Clear();
	//printf(" KSkillManager::Init(%d)MUN(%d) OK..!!!!\n",MAX_SKILL,nSkillNum);
	return TRUE;
}
//�������ܽű�����
ISkill*	KSkillManager::InstanceSkill( unsigned int ulSkillID, unsigned int ulSkillLevel)
{
	ISkill *pRetSkill = NULL;

    int nStyle = GetSkillStyle(ulSkillID);

	switch (nStyle) // eSkillStyle
	{
	case SKILL_SS_Missles:			        //	�ӵ��� 0	���������ڷ����ӵ���
	case SKILL_SS_Melee:                    //         1    ��������
	case SKILL_SS_InitiativeNpcState:	    //	������ 2	���������ڸı䵱ǰNpc������״̬
	case SKILL_SS_PassivityNpcState:		//	������ 3	���������ڸı�Npc�ı���״̬
		{
            KSkill * pNewOrdinSkill = NULL;
			unsigned int ulFirstLoadLevel = 0;

            if (m_pOrdinSkill[ulSkillID - 1][ulSkillLevel - 1])
            {
                pRetSkill = m_pOrdinSkill[ulSkillID - 1][ulSkillLevel - 1];
				break;
            }

			pNewOrdinSkill   = new KSkill;  //Ϊ�¼����ļ� ���ڴ�

			if  (pNewOrdinSkill==NULL)
				break;

			ulFirstLoadLevel = m_SkillInfo[ulSkillID - 1].m_ulFirstLoadLevel;

			if (!ulFirstLoadLevel)  //�����û�м��ع���
			{   //��ԭʼTXT��ȡ��������
				pNewOrdinSkill->GetInfoFromTabFile(m_SkillInfo[ulSkillID - 1].m_nTabFileRowId);
				//�ӽű���ȡ�������ݸ���ԭʼTXT����
				pNewOrdinSkill->LoadSkillLevelData(ulSkillLevel, m_SkillInfo[ulSkillID - 1].m_nTabFileRowId);
				m_SkillInfo[ulSkillID - 1].m_ulFirstLoadLevel = ulSkillLevel;
			}
			else
			{
				//_ASSERT(m_pOrdinSkill[ulSkillID - 1][ulFirstLoadLevel - 1]);  //�������ݵ�Ϊǰ������
                if (!m_pOrdinSkill[ulSkillID - 1][ulFirstLoadLevel - 1])
					break;

				*pNewOrdinSkill = *(KSkill*)m_pOrdinSkill[ulSkillID - 1][ulFirstLoadLevel - 1];
				//�ӽű���ȡ��������
				pNewOrdinSkill->LoadSkillLevelData(ulSkillLevel, m_SkillInfo[ulSkillID - 1].m_nTabFileRowId);
			 }

			pNewOrdinSkill->SetSkillId(ulSkillID);
			pNewOrdinSkill->SetSkillLevel(ulSkillLevel);

			m_pOrdinSkill[ulSkillID - 1][ulSkillLevel - 1] = pNewOrdinSkill;

			pRetSkill = pNewOrdinSkill;
            pNewOrdinSkill = NULL;
        }
		break;
	case SKILL_SS_Thief:
		{
			if (!m_pOrdinSkill[ulSkillID - 1][0])
			{
				m_pOrdinSkill[ulSkillID - 1][0] = (ISkill*)new KThiefSkill;

				if  (m_pOrdinSkill[ulSkillID - 1][0]==NULL)
					break;

			}

			pRetSkill = m_pOrdinSkill[ulSkillID - 1][0];

			((KThiefSkill*)pRetSkill)->LoadSetting(THIEFSKILL_SETTINGFILE);

		}
		break;
	default:
		break;
	}

	return pRetSkill;
}


