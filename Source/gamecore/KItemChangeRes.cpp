//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemChangeRes.h
// Date:	2002.12
// Code:	Spe
// Desc:	Header File
//---------------------------------------------------------------------------
#include "KCore.h"
#include "engine/KEngine.h"
#include "KItemChangeRes.h"
#include "KItem.h"
#include "CoreUseNameDef.h"
KItemChangeRes	g_ItemChangeRes;

int KItemChangeRes::Init()  //�ı���۳�ʼ��
{
	if (!m_MeleeWeapon.Load(CHANGERES_MELEE_FILE))//����
		return FALSE;
	if (!m_RangeWeapon.Load(CHANGERES_RANGE_FILE)) //����
		return FALSE;
	if (!m_Armor.Load(CHANGERES_ARMOR_FILE)) //�·�
		return FALSE;
	if (!m_Helm.Load(CHANGERES_HELM_FILE)) //ͷ��
		return FALSE;
	if (!m_Horse.Load(CHANGERES_HORSE_FILE)) //��ƨ
		return FALSE;
	if (!m_Pifeng.Load(CHANGERES_PIFENG_FILE)) //�������
		return FALSE;
	if (!m_Gold.Load(CHANCERES_GOLD_FILE)) //�ƽ�
		return FALSE;
	if (!m_ChiBang.Load(CHANGERES_CHIBANG_FILE)) //������
		return FALSE;
	if (!m_Plat.Load(CHANCERES_PLAT_FILE)) //�׽����
	{
		printf("load config(%s)error!\n",CHANCERES_PLAT_FILE);
		return FALSE;
	}

	return TRUE;
}

int	KItemChangeRes::GetWeaponRes(int nDetail, int nParti, int nLevel)
{
	int nRet;
	if (nLevel == 0)
	{
		m_MeleeWeapon.GetInteger(2, 2, 2, &nRet);
		return nRet - 2;
	}

	int nRow = nParti * 10 + nLevel + 2;
	switch(nDetail)
	{
	case equip_meleeweapon:
		m_MeleeWeapon.GetInteger(nRow, 2, 2, &nRet);
		break;
	case equip_rangeweapon:
		m_RangeWeapon.GetInteger(nRow - 1, 2, 2, &nRet);	// û��Ĭ��״̬
		break;
	}
	return nRet - 2;
}

int	KItemChangeRes::GetArmorRes(int nParti, int nLevel)
{
	int nRet;
	if (nLevel == 0)
	{
		m_Armor.GetInteger(2, 2, 19, &nRet);
		return nRet - 2;
	}
	int nRow = nParti * 10 + nLevel + 2;
	m_Armor.GetInteger(nRow, 2, 19, &nRet);
	return nRet - 2;
}

int	KItemChangeRes::GetHelmRes(int nParti, int nLevel)
{
	int nRet;
	if (nLevel == 0)
	{
		m_Helm.GetInteger(2, 2, 19, &nRet);
		return nRet - 2;
	}
	int nRow = nParti * 10 + nLevel + 2;
	m_Helm.GetInteger(nRow, 2, 19, &nRet);
	return nRet - 2;
}

int	KItemChangeRes::GetPifengRes(int nParti, int nLevel)
{
	int nRet;
	if (nLevel == 0)
	{
		m_Pifeng.GetInteger(2, 2, 2, &nRet);
		return nRet;
	}
	int nRow = nParti * 10 + nLevel + 2;
	m_Pifeng.GetInteger(nRow, 2, 2, &nRet);
	return nRet;
}

int	KItemChangeRes::GetChiBangRes(int nParti, int nLevel)
{
	int nRet;
	if (nLevel == 0)
	{
		m_ChiBang.GetInteger(2, 2, 2, &nRet);
		return nRet;
	}
	int nRow = nParti * 10 + nLevel + 2;
	m_ChiBang.GetInteger(nRow, 2, 2, &nRet);
	return nRet;
}

int KItemChangeRes::GetHorseRes(int nParti, int nLevel)
{
	int nRet;
	if (nLevel == 0)
	{
		return -1;
	}
	int nRow = nParti * 10 + nLevel + 2;
	m_Horse.GetInteger(nRow, 2, 2, &nRet);
	return nRet - 2;
}

int	KItemChangeRes::GetGoldItemRes(int nGoldId) //��ȡ�ƽ����ID
{
	int nRet=2,mRet=0;
    int nRows=m_Gold.GetHeight();
	int i;
	for  (i=0;i<nRows;++i)
	{
		   m_Gold.GetInteger(i+1,1, 2, &mRet);
           if (mRet==nGoldId)
		   {
                 m_Gold.GetInteger(i+1, 2, 2, &nRet);
				 break;
		   }
		   //printf("--�ƽ��(%d)��,ֵ:%d--\n",i,mRet);

	}
	//m_Gold.GetInteger(nGoldId+1,2,2,&nRet);
	return nRet - 2;
}

int	KItemChangeRes::GetPlatinaRes(int nGoldId) //��ȡ�ƽ����ID
{
	int nRet=2,mRet=0;

    int nRows=m_Plat.GetHeight();
	//printf("--�׽���(%d)��--\n",nRows);

	int i;
	for (i=0;i<nRows;++i)
	{
    	m_Plat.GetInteger(i+1,1,2,&mRet);

		if (mRet==nGoldId)
		{
			m_Plat.GetInteger(i+1,2,2,&nRet);
			break;
		}
	    //printf("--�׽��(%d)��,ֵ:%d--\n",i,mRet);
	}
	//m_Plat.GetInteger(nGoldId+1,2,2,&nRet);
	return nRet - 2;
}
