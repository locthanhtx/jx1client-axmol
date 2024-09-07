#include "engine/KEngine.h"
#include "KCore.h"
#include "SkillDef.h"
#include "KSkillList.h"
//#include "crtdbg.h"
//#include "S3DBInterface.h"
#include "KSkills.h"
#include "KPlayer.h"

#include "CoreShell.h"

KSkillList::KSkillList()
{
	ZeroMemory(m_Skills, sizeof(NPCSKILL) * MAX_NPCSKILL);
	m_nNpcIndex = 0;
}

KSkillList::~KSkillList()
{
}


//static int g_CallCount = 0;

int	KSkillList::FindSame(int nSkillID)
{
//    g_CallCount++;

	if (!nSkillID)
		return 0;

	if (nSkillID>g_SkillManager.GetSkillCount())
		return 0;


	for (int i=1; i<MAX_NPCSKILL; ++i)
	{
		if (m_Skills[i].SkillId == nSkillID)
		{
			return i;
		}
	}
	return 0;
}

int KSkillList::FindFree()
{
	for (int i = 1; i < MAX_NPCSKILL; ++i)
	{
		if (!m_Skills[i].SkillId)
		{//���ؿ�λ��
			return i;
		}
	}
	return 0;
}
//�����NPC�������� ������ʹ�ü���
int KSkillList::GetCount()
{
	int nCount = 0;

	for (int i = 1; i < MAX_NPCSKILL; ++i)
	{
		if (m_Skills[i].SkillId)
		{
			nCount++;
		}
	}
	return nCount;
}

void KSkillList::SetSkillLevel(int nId/*���ܵ�ID��*/, int nLevel)
{
	int i = FindSame(nId);

	if (!i)
	{
		Add(nId, nLevel);
		return;
	}
	else
		//m_Skills[i].SkillLevel = nLevel;
		SetLevel(i, nLevel);
}

int KSkillList::SetLevel(int nIndex/*�����б��������*/, int nLevel)
{
	if (nIndex <= 0 || nIndex >= MAX_NPCSKILL)
		return FALSE;
	if (nLevel < 0 || nLevel >= MAX_SKILLLEVEL)
		return FALSE;

	int nInc = nLevel - m_Skills[nIndex].SkillLevel;

	//if 	(nInc==0)
	IncreaseLevel(nIndex, nInc);
	//else
	//	IncreaseLevel(nIndex, nInc,FALSE,TRUE);

	return TRUE;
}
//�ӽű��н������µȼ����ܵ�����
int KSkillList::IncreaseLevel(int nIdx, int nLvl, int Qeuip,int nIsClearExp)
{
	if (nIdx <= 0 || nIdx >= MAX_NPCSKILL)
		return FALSE;

	if (m_Skills[nIdx].SkillId>g_SkillManager.GetSkillCount())
		return FALSE;

	m_Skills[nIdx].SkillLevel +=nLvl; //ʵ�ʵȼ�

	if (m_Skills[nIdx].SkillLevel>=MAX_SKILLLEVEL)
		m_Skills[nIdx].SkillLevel=MAX_SKILLLEVEL-1;

	if (m_Skills[nIdx].SkillLevel<0)
		m_Skills[nIdx].SkillLevel=0;

	m_Skills[nIdx].CurrentSkillLevel +=nLvl;//��ǰ�ȼ�

	if (m_Skills[nIdx].CurrentSkillLevel>=MAX_SKILLLEVEL)
		m_Skills[nIdx].CurrentSkillLevel=MAX_SKILLLEVEL-1;

	if (m_Skills[nIdx].CurrentSkillLevel<0)
		m_Skills[nIdx].CurrentSkillLevel=0;

	/*if (nLvl<0 && !Qeuip)  //������ӵĵȼ���С��0�Ļ� �����״̬  �ǻ�װ�Ļ�
	{//ж��װ�����������ݴ���
		KSkill * pCurinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[nIdx].SkillId, m_Skills[nIdx].CurrentSkillLevel);
		//�ӽű��н������µȼ����ܵ�����
		if (pCurinSkill) //�����������ܵ�����
		{
			if (pCurinSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState) //�Ƿ�����ʹ�ü���
			{//����Ǳ������� ���״̬

				//pCurinSkill->Cast(m_nNpcIndex, -1, m_nNpcIndex,-1);           //���ñ���״̬����NPC�ϵ�Ч��
				Npc[m_nNpcIndex].SetSkillLeftTime(m_Skills[nIdx].SkillId,0);
			}
			pCurinSkill=NULL;
		}
	}
   */
	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(m_Skills[nIdx].SkillId, m_Skills[nIdx].CurrentSkillLevel);
	//�ӽű��н������µȼ����ܵ�����
	if (!pOrdinSkill) //���û��������ܵ�����
        return FALSE;

    m_Skills[nIdx].NextCastTime      = pOrdinSkill->GetDelayPerCast();
	m_Skills[nIdx].NextHorseCastTime = pOrdinSkill->GetHorsePerCast();
	m_Skills[nIdx].nSkillStyle       = pOrdinSkill->GetSkillStyle();
    m_Skills[nIdx].nEquiptLimited    = pOrdinSkill->GetEquiptLimited();
	m_Skills[nIdx].nIsExpSkill       = pOrdinSkill->GetIsExpSkill();        //�Ƿ��������ȼ���
	m_Skills[nIdx].nExpSkill         = pOrdinSkill->GetSkillExp();          //��λ�õ���������
    if (nIsClearExp)
	   m_Skills[nIdx].nCurSkillExp      = 0;       //��ǰ������ �浵

    int nReg=0;
        nReg = pOrdinSkill->NewCanCastSkill(m_nNpcIndex,-1,m_nNpcIndex);       //�������Ƽ��

	if (pOrdinSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState && nReg==1 && !Qeuip) //�Ƿ�����ʹ�ü��� ���ǻ�װ�ľ� ִ�м���
	{//����Ǳ������ܲ����뵱ǰ��������ϵ� ����ʹ�ü���
		//messageBox(pOrdinSkill->GetSkillName(),"IncreaseLevelʹ��");
		pOrdinSkill->Cast(m_nNpcIndex,-1,m_nNpcIndex,-1); //���ñ���״̬����NPC�ϵ�Ч��
	}
	//ԭ��û�е�
	else if (pOrdinSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState && nReg <= 0 && !Qeuip)
	{//�뵱ǰ���������ϵı���״̬ �����
		//messageBox(pOrdinSkill->GetSkillName(),"IncreaseLevel���");
         pOrdinSkill->Cast(m_nNpcIndex, -1, m_nNpcIndex,1);
	}

	pOrdinSkill=NULL;
	ReEnChance(); //�������üӳ�Ч��

	return TRUE;
}

int	KSkillList::SetSkillLevelDirectlyUsingId(unsigned int ulSkillId, unsigned int ulLevel)/*�����øú�������ΪĿǰδ���κα������ܽ�������ֵ��Ӱ��*/
{
	if (ulSkillId >= MAX_SKILL || ulSkillId == 0 ||  ulLevel >= MAX_SKILLLEVEL)
		return 0;

	if (ulSkillId>g_SkillManager.GetSkillCount())
		return FALSE;

	unsigned int  ulSkillIdx = FindSame(ulSkillId);
	if (ulSkillIdx)
	{
		if (m_Skills[ulSkillIdx].SkillId)
		{
			m_Skills[ulSkillIdx].SkillLevel = ulLevel;
			m_Skills[ulSkillIdx].CurrentSkillLevel = ulLevel;

			ISkill * pSkill =  g_SkillManager.GetSkill(m_Skills[ulSkillIdx].SkillId, m_Skills[ulSkillIdx].CurrentSkillLevel);
			if (!pSkill)
				return 0 ;

			m_Skills[ulSkillIdx].NextCastTime      = pSkill->GetDelayPerCast();
			m_Skills[ulSkillIdx].NextHorseCastTime = pSkill->GetHorsePerCast();
			m_Skills[ulSkillIdx].nSkillStyle       = pSkill->GetSkillStyle();
			m_Skills[ulSkillIdx].nEquiptLimited    = pSkill->GetEquiptLimited();

			if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
			{
				((KSkill*)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex,-1);
			}
			ReEnChance();
			return 1;
		}
	}
	return 0;
	}

int	KSkillList::SetSkillLevelDirectlyUsingIndex(unsigned int ulSkillIdx, unsigned int ulLevel)/*�����øú�������ΪĿǰδ���κα������ܽ�������ֵ��Ӱ��*/
{
	if (ulSkillIdx >= MAX_NPCSKILL ||  ulSkillIdx == 0 || ulLevel >= MAX_SKILLLEVEL)
		return 0;

	if (m_Skills[ulSkillIdx].SkillId>g_SkillManager.GetSkillCount())
		return 0;

	if (m_Skills[ulSkillIdx].SkillId)
	{
		m_Skills[ulSkillIdx].SkillLevel = ulLevel;
		m_Skills[ulSkillIdx].CurrentSkillLevel = ulLevel;

		ISkill * pSkill =  g_SkillManager.GetSkill(m_Skills[ulSkillIdx].SkillId, m_Skills[ulSkillIdx].CurrentSkillLevel);
		if (!pSkill)
			return 0 ;

		m_Skills[ulSkillIdx].NextCastTime      = pSkill->GetDelayPerCast();
		m_Skills[ulSkillIdx].NextHorseCastTime = pSkill->GetHorsePerCast();
		m_Skills[ulSkillIdx].nSkillStyle       = pSkill->GetSkillStyle();
		m_Skills[ulSkillIdx].nEquiptLimited    = pSkill->GetEquiptLimited();

		if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState)
		{
			((KSkill*)pSkill)->Cast(m_nNpcIndex, -1, m_nNpcIndex,-1);
		}

		ReEnChance();

		return 1;
	}
	return 0;
}

int KSkillList::Remove(int nSkillID)
{
	if (nSkillID <= 0 || nSkillID >= MAX_SKILL)  //������������
		return FALSE;
	int i = FindSame(nSkillID);
	if (i)
	{
		m_Skills[i].CurrentSkillLevel = 0;
		m_Skills[i].SkillId = 0;
		m_Skills[i].SkillLevel = 0;
		m_Skills[i].mAddPoint = 0;
		m_Skills[i].EnChance = 0;   //������ӵ�
		m_Skills[i].nTempEnChance = 0;
		m_Skills[i].nSkillStyle = -1;
		m_Skills[i].NextCastTime = 0;
		m_Skills[i].NextHorseCastTime = 0;
		m_Skills[i].nEquiptLimited = 0;
		m_Skills[i].nExpSkill=0;     //��ǰ�����Ⱦ���
		m_Skills[i].nCurSkillExp=0;
		return i;

	}
	return FALSE;

}
//�Ƴ�ȫ������
void KSkillList::RemoveAllSkill()
{
//	int i = FindSame(nSkillID);
	for (int i=0;i<MAX_NPCSKILL;++i)
	{
      if (m_Skills[i].SkillId>0 && !CheckNoSkill("NoCanXiSui",m_Skills[i].SkillId))
	  {
		   m_Skills[i].CurrentSkillLevel = 0;
		   m_Skills[i].SkillId = 0;
		   m_Skills[i].SkillLevel = 0;
		   m_Skills[i].mAddPoint = 0;
		   m_Skills[i].EnChance = 0;   //������ӵ�
		   m_Skills[i].nTempEnChance = 0;
		   m_Skills[i].nSkillStyle = -1;
		   m_Skills[i].NextCastTime = 0;
		   m_Skills[i].NextHorseCastTime = 0;
		   m_Skills[i].nEquiptLimited = 0;
		   m_Skills[i].nExpSkill=0;
		   m_Skills[i].nCurSkillExp=0;
	  }
	}
}

//������Ӽ��� �� ʹ�ü��ܵ��õĺ���   //�ͻ���  �������� ������
int KSkillList::Add(int nSkillID, int nLevel, int nMaxTimes, int RemainTimes, int nAdd,int nCurExpSkill,int nExpSkillPonit)
{
	int i;
	if (nSkillID <= 0 || nLevel < 0)
		return 0;

	if (nSkillID>g_SkillManager.GetSkillCount())
		return 0;

	if (nLevel>=MAX_SKILLLEVEL)
		nLevel=	MAX_SKILLLEVEL-1;
	int nTempLevel=0;
//--------------------�Ѿ����������-------------------------
	i = FindSame(nSkillID);

	if (i)
	{//������ҵ�������ܵ��б�λ��--�Ѿ������������
		m_Skills[i].SkillId = nSkillID;
		if (nLevel > m_Skills[i].SkillLevel)
		{//�ȼ�����ԭ���ĵȼ�
			int nInc = nLevel - m_Skills[i].SkillLevel;	 //����ļ��ܵ�
			IncreaseLevel(i,nInc,FALSE,TRUE);            //���¼��ؼ���λ����Ϣ
		}
		else
		{
			if (nSkillID==1 || nSkillID==2 || nSkillID==53)
			{
		        m_Skills[i].SkillLevel = 1;
		        m_Skills[i].CurrentSkillLevel = 1;
			}
			else
			{
				m_Skills[i].SkillLevel = nLevel;
		        m_Skills[i].CurrentSkillLevel = nLevel;
			}
		}

		m_Skills[i].MaxTimes = 0;
		m_Skills[i].RemainTimes = 0;
		m_Skills[i].mAddPoint = nAdd;
		m_Skills[i].EnChance = 0;
		m_Skills[i].nTempEnChance = 0;
        m_Skills[i].nCurSkillExp=0;

		nTempLevel = m_Skills[i].CurrentSkillLevel;

		if  (nTempLevel<=0)
		{
			nTempLevel = 1;
		}

		if (nTempLevel  > 0)
		{
			KSkill * pSkill =(KSkill *) g_SkillManager.GetSkill(nSkillID, nTempLevel); //m_Skills[i].CurrentSkillLevel
			if (!pSkill)
				return 0;

			m_Skills[i].NextCastTime      = pSkill->GetDelayPerCast();
			m_Skills[i].NextHorseCastTime = pSkill->GetHorsePerCast();
			m_Skills[i].nSkillStyle       = pSkill->GetSkillStyle();
            m_Skills[i].nEquiptLimited    = pSkill->GetEquiptLimited();

			if (m_Skills[i].CurrentSkillLevel<=0)
				m_Skills[i].nExpSkill         = 0;
			else
				m_Skills[i].nExpSkill         = pSkill->GetSkillExp();

			m_Skills[i].nIsExpSkill       = pSkill->GetIsExpSkill();
			m_Skills[i].nCurSkillExp      = nCurExpSkill;
			//����ü������ڱ�����������ʱ��������Npc״̬
			if (m_Skills[i].CurrentSkillLevel>0)
			{
			  int nReg=0;
                  nReg = pSkill->NewCanCastSkill(m_nNpcIndex, -1, m_nNpcIndex);
			  if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState && nReg==1)
			  {//������ �� ���������Ƶ� ִ��״̬
				//messageBox(pSkill->GetSkillName(),"����ʹ��");
				pSkill->Cast(m_nNpcIndex,-1,m_nNpcIndex,-1);
			  }
			  else if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState && nReg<=0)
			  {//��� �뵱ǰ����������ı�������״̬Ч��
				//messageBox(pSkill->GetSkillName(),"�������");
                pSkill->Cast(m_nNpcIndex, -1, m_nNpcIndex,1);
			  }
			  ReEnChance();
			}
			pSkill=NULL;
		}
		return i;
	}
//----------------------����Ӽ���---------------------------------
	i = FindFree();
	if (i)
	{//���������λ�ò����ø�λ�õļ�����Ϣ
		m_Skills[i].SkillId = nSkillID;
		if (nSkillID==1 || nSkillID==2 || nSkillID==53)
		{
			m_Skills[i].SkillLevel = 1;
			m_Skills[i].CurrentSkillLevel = 1;
		}
		else
		{
			m_Skills[i].SkillLevel = nLevel;
			m_Skills[i].CurrentSkillLevel = nLevel;
		}
		m_Skills[i].MaxTimes = 0;
		m_Skills[i].RemainTimes = 0;
		m_Skills[i].mAddPoint = nAdd;
		m_Skills[i].EnChance = 0;
		m_Skills[i].nTempEnChance = 0;
		m_Skills[i].nSkillStyle = -1;
		m_Skills[i].NextCastTime = 180;
        m_Skills[i].NextHorseCastTime = 180;
		m_Skills[i].nEquiptLimited = -2;
		m_Skills[i].nCurSkillExp=0;

		nTempLevel = m_Skills[i].CurrentSkillLevel;

		if  (nTempLevel<=0)
		{
			nTempLevel = 1;
		}

		if (nTempLevel > 0)
		{
			KSkill * pSkill = (KSkill*) g_SkillManager.GetSkill(nSkillID, nTempLevel); //nLevel
			if (!pSkill)
				return 0;

		    m_Skills[i].NextCastTime      = pSkill->GetDelayPerCast();
			m_Skills[i].NextHorseCastTime = pSkill->GetHorsePerCast();
			m_Skills[i].nSkillStyle       = pSkill->GetSkillStyle();
            m_Skills[i].nEquiptLimited    = pSkill->GetEquiptLimited();

			if (m_Skills[i].CurrentSkillLevel<=0)
				m_Skills[i].nExpSkill         = 0;
			else
				m_Skills[i].nExpSkill     = pSkill->GetSkillExp();

			m_Skills[i].nIsExpSkill       = pSkill->GetIsExpSkill();
			m_Skills[i].nCurSkillExp      = nCurExpSkill;
			if (m_Skills[i].CurrentSkillLevel>0)
			{
              int nReg=0;
                  nReg = pSkill->NewCanCastSkill(m_nNpcIndex, -1, m_nNpcIndex);
			  if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState && nReg==1)
			  {//�������� �� �����ƵĶ�ʹ��
				//char msg[64];
				//sprintf(msg,"ʹ������:%d",pSkill->GetEquiptLimited());
				//messageBox(pSkill->GetSkillName(),msg);
				pSkill->Cast(m_nNpcIndex,-1,m_nNpcIndex,-1);
			  }
			  else if (pSkill->GetSkillStyle() == SKILL_SS_PassivityNpcState && nReg<=0)
			  {//����뵱ǰ����������ϵı���״̬
				//messageBox(pSkill->GetSkillName(),"�������1111");
				pSkill->Cast(m_nNpcIndex,-1,m_nNpcIndex,1);
			  }
			  ReEnChance();
			}
			pSkill=NULL;
		}

		return i;
	}

	return 0;
}

//-----------------------------------------
int  KSkillList::AddSkillExpByListidx(int nListidx,int nCurExp,int nType)
{
	if  (nListidx<=0 && nListidx>=MAX_NPCSKILL) return 0;

	if (m_Skills[nListidx].SkillLevel < MAX_SKILLLEVEL)
	{
		if (nType==1)
		{
			m_Skills[nListidx].nCurSkillExp +=nCurExp;
		}
		else
		{
			m_Skills[nListidx].nCurSkillExp=nCurExp;
		}

		if (m_Skills[nListidx].nCurSkillExp>=m_Skills[nListidx].nExpSkill)
			m_Skills[nListidx].nCurSkillExp=m_Skills[nListidx].nExpSkill;

		return 1;
	}

	return 0;

}
int  KSkillList::GetCurSkillExpByListidx(int nListidx)
{
	if  (nListidx<=0 && nListidx>=MAX_NPCSKILL) return 0;

	if (m_Skills[nListidx].SkillLevel < MAX_SKILLLEVEL)
		return m_Skills[nListidx].nCurSkillExp;

	return 0;
}
int  KSkillList::GetSkillExpByListidx(int nListidx)
{
	if  (nListidx<=0 && nListidx>=MAX_NPCSKILL) return 0;

	if (m_Skills[nListidx].SkillLevel < MAX_SKILLLEVEL)
		return m_Skills[nListidx].nExpSkill;

	return 0;
}
void  KSkillList::SetSkillExpByListidx(int nListidx,int nExp)
{
	if  (nListidx<=0 && nListidx>=MAX_NPCSKILL) return;

	if (m_Skills[nListidx].SkillLevel < MAX_SKILLLEVEL)
		m_Skills[nListidx].nExpSkill=nExp;
	else
		m_Skills[nListidx].nExpSkill=0;

}

int   KSkillList::IsSkillExpByListidx(int nListidx)
{
	if  (nListidx<=0 && nListidx>=MAX_NPCSKILL) return 0;
	if (m_Skills[nListidx].SkillLevel < MAX_SKILLLEVEL)
		return m_Skills[nListidx].nIsExpSkill;

	return 0;
}
//-----------------------------------------
//��������
int KSkillList::GetSkillExp(int nSkillID)
{
	int i;

	if (!nSkillID)
		return 0;

	i = FindSame(nSkillID);
	if (i)
	{
		//_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
		if (m_Skills[i].SkillLevel < MAX_SKILLLEVEL)
		    return m_Skills[i].nExpSkill;
	}

	return 0;
}
//������������
void KSkillList::SetSkillExp(int nSkillID,int nExp)
{
	int i;

	if (!nSkillID)
		return;

	i = FindSame(nSkillID);
	if (i)
	{
		//_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
		if (m_Skills[i].SkillLevel < MAX_SKILLLEVEL)
		   m_Skills[i].nExpSkill=nExp;
	}
}

//��ǰ������
int KSkillList::GetCurSkillExp(int nSkillID)
{
	int i;

	if (!nSkillID)
		return 0;

	i = FindSame(nSkillID);
	if (i)
	{
		//_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
		if (m_Skills[i].SkillLevel < MAX_SKILLLEVEL)
		   return m_Skills[i].nCurSkillExp;
	}

	return 0;
}
//�Ƿ������ȼ���
int KSkillList::IsSkillExp(int nSkillID)
{
	int i;

	if (!nSkillID)
		return 0;

	i = FindSame(nSkillID);
	if (i)
	{
		//_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
		if (m_Skills[i].SkillLevel < MAX_SKILLLEVEL)
		    return m_Skills[i].nIsExpSkill;
	}

	return 0;
}

int KSkillList::GetLevelByListidx(int nListidx)
{

	if (nListidx<=0 && nListidx>=MAX_NPCSKILL) return 0;

	if (m_Skills[nListidx].SkillLevel < MAX_SKILLLEVEL)
		return m_Skills[nListidx].SkillLevel;

	return 0;
}


int KSkillList::AddSkillExp(int nSkillID,int nCurExp,int nType)
{
	int i;

	if (!nSkillID)
		return 0;

	i = FindSame(nSkillID);
	if (i)
	{
	//	_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
	  if (m_Skills[i].SkillLevel < MAX_SKILLLEVEL)
	  {
        if (nType==1)
		{
            m_Skills[i].nCurSkillExp +=nCurExp;
		}
		else
		{
            m_Skills[i].nCurSkillExp=nCurExp;
		}

		if (m_Skills[i].nCurSkillExp>=m_Skills[i].nExpSkill)
				m_Skills[i].nCurSkillExp=m_Skills[i].nExpSkill;

		return 1;
	  }
	}

	return 0;
}


int KSkillList::GetLevel(int nSkillID)
{
	int i;

	if (!nSkillID)
		return 0;

	i = FindSame(nSkillID);
	if (i)
	{
		//_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
		if (m_Skills[i].SkillLevel < MAX_SKILLLEVEL)
		   return m_Skills[i].SkillLevel;
	}

	return 0;
}

int KSkillList::SetSLevelByListidx(int nListidx, int nLevel,int nKind)
{
	if (nListidx<=0 && nListidx>=MAX_NPCSKILL) return FALSE;

	if (m_Skills[nListidx].SkillLevel < MAX_SKILLLEVEL)
	{

		if (nKind)
		{
			m_Skills[nListidx].SkillLevel +=nLevel;
			m_Skills[nListidx].CurrentSkillLevel +=nLevel;
		}
		else
		{
			m_Skills[nListidx].SkillLevel =nLevel;
			m_Skills[nListidx].CurrentSkillLevel =nLevel;
		}

		if (m_Skills[nListidx].SkillLevel<0)
			m_Skills[nListidx].SkillLevel = 0;
		if (m_Skills[nListidx].CurrentSkillLevel<0)
			m_Skills[nListidx].CurrentSkillLevel = 0;

		return TRUE;
	}

	return FALSE;
}

int KSkillList::SetSLevel(int nSkillID, int nLevel,int nKind)
{
	int i;

	if (!nSkillID)
		return FALSE;

	i = FindSame(nSkillID);
	if (i)
	{
		//_ASSERT(m_Skills[i].SkillLevel < MAX_SKILLLEVEL);
		if (m_Skills[i].SkillLevel < MAX_SKILLLEVEL)
		{

		  if (nKind)
		  {
		      m_Skills[i].SkillLevel +=nLevel;
		      m_Skills[i].CurrentSkillLevel +=nLevel;
		  }
		  else
		  {
			  m_Skills[i].SkillLevel =nLevel;
		      m_Skills[i].CurrentSkillLevel =nLevel;
		  }

		   if (m_Skills[i].SkillLevel<0)
			   m_Skills[i].SkillLevel = 0;
		   if (m_Skills[i].CurrentSkillLevel<0)
			   m_Skills[i].CurrentSkillLevel = 0;

		   return TRUE;
		}
	}

	return FALSE;
}

int KSkillList::GetCurrentLevel(int nSkillID)
{
	int i;

	if (!nSkillID)
		return 0;

	i = FindSame(nSkillID);

	if (i)
	{
		return m_Skills[i].CurrentSkillLevel;
	}

	return 0;
}

int KSkillList::GetCurrentLevelByIdx(int nListIdx)
{
	if (nListIdx > 0 && nListIdx < MAX_NPCSKILL)
		return m_Skills[nListIdx].CurrentSkillLevel;

	return 0;
}

//�Ƿ���Է����� �����ܵ� ʱ����
int KSkillList::CanCast(int nSkillID, unsigned long dwTime)
{

	if (!nSkillID)
		return FALSE;

	int i = FindSame(nSkillID);
	if (!i)
	{
		return FALSE;
	}

	if (m_Skills[i].CurrentSkillLevel <= 0 || m_Skills[i].NextCastTime > dwTime)  //ʱ�����������ڵ�ʱ��
		return FALSE;

	return TRUE;
}
//�Ƿ���Է����� �����ܵ� ʱ����
int KSkillList::CanCastByIndex(int nSkListIndex, unsigned long dwTime)
{

	if (nSkListIndex<=0 && nSkListIndex>=MAX_NPCSKILL) return false;

	if (m_Skills[nSkListIndex].CurrentSkillLevel <= 0 || m_Skills[nSkListIndex].NextCastTime > dwTime)  //ʱ�����������ڵ�ʱ��
		return FALSE;

	return TRUE;
}
//���������ܽⶳʱ��
void	KSkillList::SetHorseNextCastTimeByIndex(int nSkListIndex, unsigned long dwTime)
{
	if (nSkListIndex<=0 && nSkListIndex>=MAX_NPCSKILL) return;
	m_Skills[nSkListIndex].NextHorseCastTime = dwTime;
}
//�´η�����ʱ��
void KSkillList::SetNextCastTimeByIndex(int nSkListIndex, unsigned long dwTime)
{
	if (nSkListIndex<=0 && nSkListIndex>=MAX_NPCSKILL) return;
	m_Skills[nSkListIndex].NextCastTime = dwTime;
}

//���������ܽⶳʱ��
void	KSkillList::SetHorseNextCastTime(int nSkillID, unsigned long dwTime)
{
	if (!nSkillID)
		return;
	int i = FindSame(nSkillID);
	if (!i)
		return;
	m_Skills[i].NextHorseCastTime = dwTime;
}
//�´η�����ʱ��
void KSkillList::SetNextCastTime(int nSkillID, unsigned long dwTime)
{
//	m_Skills[nSkillID].NextCastTime = dwTime;
//	return;
	if (!nSkillID)
		return;

	int i = FindSame(nSkillID);
	if (!i)
		return;

	m_Skills[i].NextCastTime = dwTime;
}

int KSkillList::GetHorseNextCastTimeByIndex(int nSkListIndex)
{
	if (nSkListIndex<=0 || nSkListIndex>=MAX_NPCSKILL)
		return -1;

	return	m_Skills[nSkListIndex].NextHorseCastTime;
}

int KSkillList::GetHorseNextCastTime(int nSkillID)
{
	if (!nSkillID)
		return -1;

	int i = FindSame(nSkillID);
	if (!i)
		return -1;

    return	m_Skills[i].NextHorseCastTime;
}

int KSkillList::GetNextCastTimeByIndex(int nSkListIndex)
{
	if (nSkListIndex<=0 || nSkListIndex>=MAX_NPCSKILL)
		return -1;

	return	m_Skills[nSkListIndex].NextCastTime;
}

int KSkillList::GetNextCastTime(int nSkillID)
{
	//	m_Skills[nSkillID].NextCastTime = dwTime;
	//	return;
	if (!nSkillID)
		return -1;

	int i = FindSame(nSkillID);
	if (!i)
		return -1;
    return	m_Skills[i].NextCastTime;
}
//���¼��㼼�ܼӳ�
void KSkillList::ReEnChance()
{
	int i;
	for(i = 0;i < MAX_NPCSKILL;++i)
	{
		if (m_Skills[i].SkillId <= 0)
			continue;
		m_Skills[i].EnChance = 0;
		m_Skills[i].nTempEnChance = 0;
	}

	for(i = 0;i < MAX_NPCSKILL;++i)
	{
		if (m_Skills[i].SkillId <= 0 || m_Skills[i].SkillId==1 || m_Skills[i].SkillId==2 || m_Skills[i].SkillId==53)
			continue;
		int nSkillId    = m_Skills[i].SkillId;
		int nSkillLevel = m_Skills[i].CurrentSkillLevel;

		if (nSkillLevel <= 0)
			continue;
		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillId, nSkillLevel);
        //���üӳ�Ч��
		if (!pOrdinSkill)
			continue;

		pOrdinSkill->EnChanceSkill(m_nNpcIndex);
		//ִ�����ʼ����
		m_Skills[i].nTempEnChance = m_Skills[i].EnChance;
	}
}

//--------------------------�ͻ��˼�������-----------------------
int	KSkillList::GetSkillSortList(KUiSkillData * pSkillList)
{
	//Ques
	if (!pSkillList)
		return 0;

	memset(pSkillList, 0, sizeof(KUiSkillData) * 50);
	int nCount = 0;
	ISkill* pSkill = NULL;
	KSkill * pOrdinSkill = NULL;
	for (int i = 1; i < MAX_NPCSKILL; ++i)
	{
		if (m_Skills[i].SkillId)
		{
			//_ASSERT(m_Skills[i].SkillLevel >= 0);

			if (m_Skills[i].SkillLevel<=0)
			{
				pSkill =  g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
			}
			else
			{
				pSkill =  g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].SkillLevel);
			}

			if (!pSkill)
				continue;
			eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();

			switch(eStyle)
			{
			case SKILL_SS_Missles:			        //	�ӵ���		���������ڷ����ӵ���
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:	    //	������		���������ڸı䵱ǰNpc������״̬
			case SKILL_SS_PassivityNpcState:		//	������		���������ڸı�Npc�ı���״̬
				{
					/*
					pOrdinSkill = (KSkill * ) pSkill;
					if (pOrdinSkill->IsPhysical())
						continue;//��һ������
					*/
					if (m_Skills[i].SkillId==1 || m_Skills[i].SkillId==2 || m_Skills[i].SkillId==53)
						continue;//��һ������
				}
				break;
			case SKILL_SS_Thief:					//	͵����
				{

				}
				break;
			default:
				if (m_Skills[i].SkillId==1 || m_Skills[i].SkillId==2 || m_Skills[i].SkillId==53)
					continue;//��һ������
			}

            KUiSkillData * pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_FIGHT;
			pSkill->uId = m_Skills[i].SkillId;
			pSkill->nLevel = m_Skills[i].SkillLevel;
			if ((++nCount) == 50)  //��������������
			{
				printf("Npc�ļ�����������������50������������!!!\n");
				break;
			}
		}
	}
	return nCount;
}

int KSkillList::GetLeftSkillSortList(KUiSkillData* pSkillList)
{
	if (!pSkillList) return 0;
	memset(pSkillList, 0, sizeof(KUiSkillData) * 65);
	int nCount = 1;

	pSkillList->uGenre = CGOG_SKILL_SHORTCUT;
	pSkillList->uId = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetCurActiveWeaponSkill();//�������ּ���ID!
	pSkillList->nData = 0;

	ISkill * pISkill = NULL;
	KSkill * pOrdinSkill = NULL;
	for (int i = 1; i < MAX_NPCSKILL; ++i)
	{
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel > 0)
		{
			//_ASSERT(m_Skills[i].SkillId > 0);
			pISkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].SkillLevel);
			if (!pISkill)
				continue;

			eSkillStyle eStyle = (eSkillStyle)pISkill->GetSkillStyle();
			switch(eStyle)
			{
			case SKILL_SS_Missles:			//	�ӵ���		���������ڷ����ӵ���
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:	//	������		���������ڸı䵱ǰNpc������״̬
			case SKILL_SS_PassivityNpcState:		//	������		���������ڸı�Npc�ı���״̬
				{
					if (m_Skills[i].SkillId==1 || m_Skills[i].SkillId==2 || m_Skills[i].SkillId==53)
                        continue;

					pOrdinSkill = (KSkill * ) pISkill;
					if (pOrdinSkill->GetSkillLRInfo() == BothSkill || pOrdinSkill->GetSkillLRInfo() == leftOnlySkill)
					{
					}
					else
					   continue;

				}
				break;
			case SKILL_SS_Thief:
				{
					continue;
				}
				break;
			}
			KUiSkillData * pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_SHORTCUT;
			pSkill->uId = m_Skills[i].SkillId;
			//pSkill->nData = nCount / 8;
			pSkill->nLevel=m_Skills[i].SkillLevel;
			nCount ++;
			if (nCount >= 65)
				break;

		}
	}
	return nCount;
}

int KSkillList::GetRightSkillSortList(KUiSkillData* pSkillList)
{
	if (!pSkillList) return 0;
	memset(pSkillList, 0, sizeof(KUiSkillData) * 65);

	int nCount = 1;
	pSkillList->uGenre = CGOG_SKILL_SHORTCUT;
	//pSkillList->uId = (unsigned int)-1;//��������ǰ!
	pSkillList->uId = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetCurActiveWeaponSkill();//��������ǰ!;
	pSkillList->nData = 0;
	ISkill * pISkill = NULL;
	KSkill * pOrdinSkill = NULL;

	for (int i = 1; i < MAX_NPCSKILL; ++i)
	{
		if (m_Skills[i].SkillId && m_Skills[i].SkillLevel > 0)
		{
			//_ASSERT(m_Skills[i].SkillId > 0);
			pISkill  =  g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].SkillLevel);
			if(!pISkill)
				continue;

			eSkillStyle eStyle = ( eSkillStyle ) pISkill->GetSkillStyle();

			switch(eStyle)
			{
			case SKILL_SS_Missles:			    //	�ӵ���		���������ڷ����ӵ���
			case SKILL_SS_Melee:
			case SKILL_SS_InitiativeNpcState:	//	������		���������ڸı䵱ǰNpc������״̬
			case SKILL_SS_PassivityNpcState:    //	������		���������ڸı�Npc�ı���״̬
				{
					if (m_Skills[i].SkillId==1 || m_Skills[i].SkillId==2 || m_Skills[i].SkillId==53)
                        continue;
					pOrdinSkill = (KSkill * ) pISkill;
					if (pOrdinSkill->GetSkillLRInfo() == BothSkill || pOrdinSkill->GetSkillLRInfo() == RightOnlySkill)
					{
					}
					else
					    continue;
				}
				break;
			case SKILL_SS_Thief:
				{
				}
				break;
			}
			KUiSkillData * pSkill = pSkillList + nCount;
			pSkill->uGenre = CGOG_SKILL_SHORTCUT;
			pSkill->uId = m_Skills[i].SkillId;
			//pSkill->nData = nCount / 8;
			pSkill->nLevel=m_Skills[i].SkillLevel;
			nCount ++;
			if (nCount >= 65)
				break;

		}
	}
	return nCount;
}

//��ü����ڼ��ܽ����λ��
int KSkillList::GetSkillPosition(int nSkillId)
{
	if (nSkillId <= 0)
		return -1;
	KSkill * pOrdinSkill = NULL;

	ISkill * pISkill = NULL;
	int nCount = 0;
	for (int i = 1; i < MAX_NPCSKILL; ++i)
	{
		//_ASSERT(m_Skills[i].SkillLevel >= 0);

		if (m_Skills[i].SkillId<= 0)
			continue;

		if (m_Skills[i].SkillLevel == 0)
		{//�ȼ�Ϊ0ʱ�� ������Ĭ�ϵȼ�1������
			pISkill = g_SkillManager.GetSkill(m_Skills[i].SkillId, 1);
		}
		else
		{
			pISkill =  g_SkillManager.GetSkill(m_Skills[i].SkillId, m_Skills[i].SkillLevel);
		}

		if (!pISkill)
        	continue ;

		eSkillStyle eStyle = ( eSkillStyle ) pISkill->GetSkillStyle();
		switch(eStyle)
		{
		case SKILL_SS_Missles:			//	�ӵ���		���������ڷ����ӵ���
		case SKILL_SS_Melee:
		case SKILL_SS_InitiativeNpcState:	//	������		���������ڸı䵱ǰNpc������״̬
		case SKILL_SS_PassivityNpcState:	//	������		���������ڸı�Npc�ı���״̬
		case SKILL_SS_Thief:
			break;
		}

		if (m_Skills[i].SkillId == nSkillId)
		{
			nCount=i;
			return nCount;
		}
		//nCount ++;

	}
	return nCount;//nCount = i-1;
}

//���ö���ļ��ܵ�
void KSkillList::SetAddPoint( int nId, int nPoint )
{
	int i = FindSame(nId);

	if (i > 0 && i < MAX_NPCSKILL)
	{
		m_Skills[i].mAddPoint = nPoint;
	}
}

void KSkillList::SetSkillEnChance( int nId, int nEnChance )
{
	int i = FindSame(nId);

	if (i > 0 && i < MAX_NPCSKILL)
	{
		m_Skills[i].EnChance = nEnChance;
	}
}

void KSkillList::AddCEnChance( int nId, int nEnChance )
{
	int i = FindSame(nId);

	if (i > 0 && i < MAX_NPCSKILL)
	{
		m_Skills[i].EnChance+= nEnChance;
	}
}

int KSkillList::CheckNoSkill(const char* nKey, int nSkillidx)
{
	int nRow = g_ForbitMap.GetHeight()+1,nReg=0;

	for (int i=2;i<nRow;++i)
	{
		int nSkilid =0;
		g_ForbitMap.GetInteger(i,nKey,0,&nSkilid);
		if (nSkilid==nSkillidx)
		{
			nReg =1;
			break;
		}
	}
	return nReg;
}
