#include "KCore.h"
#include "KItem.h"
#include "KItemGenerator.h"
#include "KItemSet.h"
//#include "MyAssert.h"

KItemSet	ItemSet;
/*!*****************************************************************************
// Function		: KItemSet::KItemSet
// Purpose		:
// Return		:
// Comments		:
// Author		: Spe
*****************************************************************************/
KItemSet::KItemSet()
{
	m_dwIDCreator = 100;  //dwid ��ʼ����
	ZeroMemory(&m_sRepairParam, sizeof(REPAIR_ITEM_PARAM));
#ifdef _SERVER
	m_psItemInfo = NULL;
	m_psBackItemInfo = NULL;
#endif
}

KItemSet::~KItemSet()
{
#ifdef _SERVER
	if (m_psItemInfo)
	{
		delete [] m_psItemInfo;
	    m_psItemInfo = NULL;
	}
	if (m_psBackItemInfo)
	{
		delete [] m_psBackItemInfo;
	    m_psBackItemInfo = NULL;
	}
#endif
}

/*!*****************************************************************************
// Function		: KItemSet::Init
// Purpose		:
// Return		: void
// Comments		:
// Author		: Spe  ά��ĥ������
*****************************************************************************/
void KItemSet::Init()
{
	m_FreeIdxItemSet.Init(MAX_ITEM);
	m_UseIdxItemSet.Init(MAX_ITEM);

	for (int i = MAX_ITEM - 1; i > 0; i--)
	{
		m_FreeIdxItemSet.Insert(i);
	}
	KIniFile	IniFile;
	IniFile.Load(ITEM_ABRADE_FILE);   //ά�޼۸� ĥ������
//	ά�޼۸�
	IniFile.GetInteger("Repair", "ItemPriceScale", 100, &m_sRepairParam.nPriceScale);  //ά�޼۸�ı���
	IniFile.GetInteger("Repair", "MagicPriceScale", 10, &m_sRepairParam.nMagicScale);
    IniFile.GetInteger("Repair", "LevelPriceScale", 1000, &m_sRepairParam.nLevelScale);
//	����ĥ��
	IniFile.GetInteger("Attack", "Weapon", 256, &m_nItemAbradeRate[enumAbradeAttack][itempart_weapon]);
	IniFile.GetInteger("Attack", "Head", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_head]);
	IniFile.GetInteger("Attack", "Body", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_body]);
	IniFile.GetInteger("Attack", "Belt", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_belt]);
	IniFile.GetInteger("Attack", "Foot", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_foot]);
	IniFile.GetInteger("Attack", "Cuff", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_cuff]);
	IniFile.GetInteger("Attack", "Amulet", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_amulet]);
	IniFile.GetInteger("Attack", "Ring1", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_ring1]);
	IniFile.GetInteger("Attack", "Ring2", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_ring2]);
	IniFile.GetInteger("Attack", "Pendant", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_pendant]);
	IniFile.GetInteger("Attack", "Horse", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_horse]);
    IniFile.GetInteger("Attack", "Pifeng", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_pifeng]);
	IniFile.GetInteger("Attack", "Yinjian", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_yinjian]);
	IniFile.GetInteger("Attack", "Shipin", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_shiping]);
	IniFile.GetInteger("Attack", "Mask", 0, &m_nItemAbradeRate[enumAbradeAttack][itempart_mask]);
	// ����ĥ��
	IniFile.GetInteger("Defend", "Weapon", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_weapon]);
	IniFile.GetInteger("Defend", "Head", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_head]);
	IniFile.GetInteger("Defend", "Body", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_body]);
	IniFile.GetInteger("Defend", "Belt", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_belt]);
	IniFile.GetInteger("Defend", "Foot", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_foot]);
	IniFile.GetInteger("Defend", "Cuff", 64, &m_nItemAbradeRate[enumAbradeDefend][itempart_cuff]);
	IniFile.GetInteger("Defend", "Amulet", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_amulet]);
	IniFile.GetInteger("Defend", "Ring1", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_ring1]);
	IniFile.GetInteger("Defend", "Ring2", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_ring2]);
	IniFile.GetInteger("Defend", "Pendant", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_pendant]);
	IniFile.GetInteger("Defend", "Horse", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_horse]);
	IniFile.GetInteger("Defend", "Pifeng", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_pifeng]);
	IniFile.GetInteger("Defend", "Yinjian", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_yinjian]);
	IniFile.GetInteger("Defend", "Shipin", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_shiping]);
	IniFile.GetInteger("Defend", "Mask", 0, &m_nItemAbradeRate[enumAbradeDefend][itempart_mask]);
	// �ƶ�ĥ��
	IniFile.GetInteger("Move", "Weapon", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_weapon]);
	IniFile.GetInteger("Move", "Head", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_head]);
	IniFile.GetInteger("Move", "Body", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_body]);
	IniFile.GetInteger("Move", "Belt", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_belt]);
	IniFile.GetInteger("Move", "Foot", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_foot]);
	IniFile.GetInteger("Move", "Cuff", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_cuff]);
	IniFile.GetInteger("Move", "Amulet", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_amulet]);
	IniFile.GetInteger("Move", "Ring1", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_ring1]);
	IniFile.GetInteger("Move", "Ring2", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_ring2]);
	IniFile.GetInteger("Move", "Pendant", 0, &m_nItemAbradeRate[enumAbradeMove][itempart_pendant]);
	IniFile.GetInteger("Move", "Horse", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_horse]);
    IniFile.GetInteger("Move", "Pifeng", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_pifeng]);
	IniFile.GetInteger("Move", "Yinjian", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_yinjian]);
	IniFile.GetInteger("Move", "Shipin", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_shiping]);
	IniFile.GetInteger("Move", "Mask", 64, &m_nItemAbradeRate[enumAbradeMove][itempart_mask]);
	//��װĥ�� enumAbradeEquip
	IniFile.GetInteger("Equip", "Weapon", 0, &m_nItemAbradeRate[enumAbradeEquip][itempart_weapon]);
	IniFile.GetInteger("Equip", "Head", 0, &m_nItemAbradeRate[enumAbradeEquip][itempart_head]);
	IniFile.GetInteger("Equip", "Body", 0, &m_nItemAbradeRate[enumAbradeEquip][itempart_body]);
	IniFile.GetInteger("Equip", "Belt", 0, &m_nItemAbradeRate[enumAbradeEquip][itempart_belt]);
	IniFile.GetInteger("Equip", "Foot", 64, &m_nItemAbradeRate[enumAbradeEquip][itempart_foot]);
	IniFile.GetInteger("Equip", "Cuff", 0, &m_nItemAbradeRate[enumAbradeEquip][itempart_cuff]);
	IniFile.GetInteger("Equip", "Amulet", 0, &m_nItemAbradeRate[enumAbradeEquip][itempart_amulet]);
	IniFile.GetInteger("Equip", "Ring1", 0, &m_nItemAbradeRate[enumAbradeEquip][itempart_ring1]);
	IniFile.GetInteger("Equip", "Ring2", 0, &m_nItemAbradeRate[enumAbradeEquip][itempart_ring2]);
	IniFile.GetInteger("Equip", "Pendant", 0, &m_nItemAbradeRate[enumAbradeEquip][itempart_pendant]);
	IniFile.GetInteger("Equip", "Horse", 64, &m_nItemAbradeRate[enumAbradeEquip][itempart_horse]);
    IniFile.GetInteger("Equip", "Pifeng", 64, &m_nItemAbradeRate[enumAbradeEquip][itempart_pifeng]);
	IniFile.GetInteger("Equip", "Yinjian", 64, &m_nItemAbradeRate[enumAbradeEquip][itempart_yinjian]);
	IniFile.GetInteger("Equip", "Shipin", 64, &m_nItemAbradeRate[enumAbradeEquip][itempart_shiping]);
	IniFile.GetInteger("Equip", "Mask", 64, &m_nItemAbradeRate[enumAbradeEquip][itempart_mask]);
	IniFile.Clear();
	//printf(" KItemSet::Init(%d) OK!!!..\n",MAX_ITEM);
}

/*!*****************************************************************************
// Function		: KItemSet::SearchID
// Purpose		:
// Return		: int
// Argumant		: unsigned long dwID
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemSet::SearchID(unsigned long dwID)
{
	int nIdx = 0;

	while(1)
	{
		nIdx = m_UseIdxItemSet.GetNext(nIdx);
		if (!nIdx)
			break;
		if (Item[nIdx].GetID() == dwID)
			break;
	}
	return nIdx;

}
//������Ʒ
int KItemSet::Add(KItem* pItem)
{
	//KASSERT(NULL!= pItem);
	if (!pItem)
		return 0;

	int i = FindFreeItemSet();

	if (i<=0)
		return 0;

	Item[i] = *pItem;
	m_FreeIdxItemSet.Remove(i);
	m_UseIdxItemSet.Insert(i);
	return i;

}


//************************************
// Method:    Add
// FullName:  KItemSet::Add   ˢ������ԵĻƽ�װ��  ��IN int nId  �ƽ�ID ��
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: IN int nId
// Parameter: KItem * pItem
// Parameter: IN int * pnMagicLevel   ��������� ָ���ƽ�װ�� ���ڻƽ���죿
//************************************
int KItemSet::AddRondomGold(IN int nGoldIdx , IN int* pnMagicLevel , \
				  IN int nSeries,IN int nEnChance,\
				  IN int nRongpiont, \
				  int nYear, int nMonth, int nDay, int nHour,int nMin,\
				  IN int* pnRMagicLevel,\
				  IN int nLevel,IN int nLuck, IN int nDetailType, \
				  IN int nParticular,IN int nStackNum,IN int nVersion,int nIsPlatina,IN int nIsLogin,IN UINT nRandomSeed)
{
//	KASSERT(NULL != pItem);

	int i = FindFreeItemSet();

	if (i<=0)
		return 0;

	KItem*	pItem = &Item[i]; //�²��� û�д���

	pItem->m_GeneratorParam.nVersion    = nVersion;      // �汾
	pItem->m_GeneratorParam.uRandomSeed = nRandomSeed;   // �������

	if (nLevel<=0)
		nLevel = 1;

	ItemGen.GetGoldItemByRandom(nGoldIdx,nDetailType, nParticular, nSeries, nLevel, pnMagicLevel,pnRMagicLevel, nLuck, nVersion, pItem, nEnChance, nRongpiont,nIsLogin);

    /*if (pnRMagicLevel>0 || pnBMagicLevel>0)              // ���� �� ��ʯ
	{
	   if  (nIsPlatina==0) //�ƽ�
           ItemGen.GetGoldItemByRong(nDetailType, nParticular, nSeries, nLevel, pnMagicLevel,pnRMagicLevel, nLuck, nVersion, pItem, nEnChance, nRongpiont,nId,pnBMagicLevel,nIsLogin);
	   else	//�׽�
		   ItemGen.GetPlatinaItemByRong(nDetailType, nParticular, nSeries, nLevel, pnMagicLevel,pnRMagicLevel, nLuck, nVersion, pItem, nEnChance, nRongpiont,nId,pnBMagicLevel,nIsLogin);
	}
	else
	{
		if (nIsPlatina==0) //�ƽ�
	       ItemGen.GetGoldItemByIndex(nId,pItem,pnMagicLevel,nSeries,nEnChance,nRongpiont,nLevel,nIsLogin);  //����ָ���ƽ�
		else
           ItemGen.GetPlatinaByIndex(nId,pItem,pnMagicLevel,nSeries,nEnChance,nRongpiont,nLevel,nIsLogin);
	}*/

#ifdef _SERVER
	SetID(i);
#endif
	pItem->SetTime(nYear,nMonth,nDay,nHour,nMin);
	m_FreeIdxItemSet.Remove(i);
	m_UseIdxItemSet.Insert(i);
	return i;
}


//************************************
// Method:    Add
// FullName:  KItemSet::Add   ˢ�ƽ�װ��  ��IN int nId  �ƽ�ID ��
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: IN int nId
// Parameter: KItem * pItem
// Parameter: IN int * pnMagicLevel   ��������� ָ���ƽ�װ�� ���ڻƽ���죿
//************************************
int KItemSet::AddGold(IN int nId , IN int* pnMagicLevel , \
				  IN int nSeries,IN int nEnChance,\
				  IN int nRongpiont, \
				  int nYear, int nMonth, int nDay, int nHour,int nMin,\
				  IN int* pnRMagicLevel,IN int* pnBMagicLevel,\
				  IN int nLevel,IN int nLuck, IN int nDetailType, \
				  IN int nParticular,IN int nStackNum,IN int nVersion,int nIsPlatina,IN int nIsLogin,IN UINT nRandomSeed)
{
//	KASSERT(NULL != pItem);

	int i = FindFreeItemSet();

	if (i<=0)
		return 0;

	KItem*	pItem = &Item[i]; //�²��� û�д���

	pItem->m_GeneratorParam.nVersion    = nVersion;      // �汾
	pItem->m_GeneratorParam.uRandomSeed = nRandomSeed;   // �������

	if (nLevel<=0)
		nLevel = 1;

    if (*pnRMagicLevel>0 || *pnBMagicLevel>0)              // ���� �� ��ʯ
	{
	   if  (nIsPlatina==0) //�ƽ�
           ItemGen.GetGoldItemByRong(nDetailType, nParticular, nSeries, nLevel, pnMagicLevel,pnRMagicLevel, nLuck, nVersion, pItem, nEnChance, nRongpiont,nId,pnBMagicLevel,nIsLogin);
	   else	//�׽�
		   ItemGen.GetPlatinaItemByRong(nDetailType, nParticular, nSeries, nLevel, pnMagicLevel,pnRMagicLevel, nLuck, nVersion, pItem, nEnChance, nRongpiont,nId,pnBMagicLevel,nIsLogin);
	}
	else
	{
		if (nIsPlatina==0) //�ƽ�
	       ItemGen.GetGoldItemByIndex(nId,pItem,pnMagicLevel,nSeries,nEnChance,nRongpiont,nLevel,nIsLogin);  //����ָ���ƽ�
		else
           ItemGen.GetPlatinaByIndex(nId,pItem,pnMagicLevel,nSeries,nEnChance,nRongpiont,nLevel,nIsLogin);
	}

#ifdef _SERVER
	SetID(i);
#endif
	pItem->SetTime(nYear,nMonth,nDay,nHour,nMin);
	m_FreeIdxItemSet.Remove(i);
	m_UseIdxItemSet.Insert(i);
	return i;
}
//************************************
// Method:    Add
// FullName:  KItemSet::Add
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: IN int nId
// Parameter: KItem * pItem
// Parameter: IN int * pnMagicLevel   ��������� ָ�������Ļƽ�װ�� -������װ
//************************************
//int KItemSet::AddRONG(IN int Goidid , IN int* pnMagicLevel , IN int nSeries,IN int nEnChance,IN int nRongID, int nYear, int nMonth, int nDay, int nHour)
int KItemSet::AddRong(int Goidid, int nSeries, \
				  int nLevel, int nLuck, int nDetailType, \
				  int nParticularType, int* pnMagicLevel,int* rnMagicLevel,int* BnMagicLevel, \
				  int nVersion, UINT nRandomSeed,int nStackNum, int nEnChance,\
				  int nRongID,int nPoint,int mIsDaZhao,KMagicAttrib *nItemMag,\
				  int nYear, int nMonth, int nDay, int nHour,int nMin,int IsWhere,int nIsLoggin)

{
	int i = FindFreeItemSet();	//���ҿ����õ�װ���ڵ�
    if (i<=0)
	return 0;
//	int nPoint=0; //���ܴ�����װ
	KItem*	pItem = &Item[i];

	pItem->m_GeneratorParam.nVersion    = nVersion;    //�汾
	pItem->m_GeneratorParam.uRandomSeed = nRandomSeed; //�������

	if (nLevel<=0)
		nLevel = 1;

     if (Goidid>0) //��
        ItemGen.GetGoldItemByRong(nDetailType, nParticularType, nSeries, nLevel, pnMagicLevel,rnMagicLevel, nLuck, nVersion, pItem, nEnChance, nRongID,Goidid,BnMagicLevel,nIsLoggin);
     else //��װ�� ��װ
	    //ItemGen.Gen_ExistEquipment(nDetailType,nParticularType,nSeries,nLevel,pnMagicLevel,nLuck,nVersion,pItem,nEnChance,nPoint,rnMagicLevel,nRongID);
        ItemGen.Gen_EquipmentRong(nDetailType, nParticularType, nSeries, nLevel, pnMagicLevel,rnMagicLevel, nLuck, nVersion, pItem, nEnChance,nPoint, nRongID,Goidid,mIsDaZhao,nItemMag,IsWhere,nIsLoggin);

#ifdef _SERVER
	 SetID(i);
#endif
	pItem->SetTime(nYear,nMonth,nDay,nHour,nMin);
	m_FreeIdxItemSet.Remove(i);
	m_UseIdxItemSet.Insert(i);
	return i;  //����װ���� ������
}
/*!*****************************************************************************
// Function		: KItemSet::Add   ������װ�õ�   ��� ����   ---�������װ�� �����õĺ���
// Purpose		:
// Return		: int ������
// Argumant		: int �������ͣ�װ����ҩƷ����ʯ��������
// Argumant		: int ħ���ȼ������Ӧ��װ��������һ��װ������ɫװ��������ȡ�����
// Argumant		: int ��������
// Argumant		: int �ȼ�
// Argumant		: int ����ֵ
// Comments		:
// Author		: Spe  ����װ��---ˢװ�����ú���
*****************************************************************************/
int KItemSet::AddOther(int nItemGenre, int nSeries, \
				  int nLevel, int nLuck, int nDetailType/*=-1*/, \
				  int nParticularType/*=-1*/, int* pnMagicLevel,\
				  int nVersion/*=0*/, UINT nRandomSeed,int nStackNum,\
				  int nEnChance,int nPoint, int nYear, int nMonth,\
				  int nDay, int nHour,int nMin,IN int nRongpiont,\
				  IN int* pnRMagicLevel,IN int* pnJbLevel,\
				  IN int* pnBsLevel,IN int IsWhere,IN int nIsLogin)
{
	int i = FindFreeItemSet();

	if (i <= 0)
		return 0;

	KItem*	pItem = &Item[i];
    //int Goidid=0;
	pItem->m_GeneratorParam.nVersion    = nVersion;
	pItem->m_GeneratorParam.uRandomSeed = nRandomSeed;

    if (nLevel<=0)
		nLevel = 1;

	switch(nItemGenre)          //���Դ����װ������
	{
	case item_equip:            // װ����0
         ItemGen.Gen_EquipmentRong(nDetailType,nParticularType, nSeries, nLevel, pnMagicLevel,pnRMagicLevel, nLuck, nVersion, pItem, nEnChance,nPoint, nRongpiont,0,0,NULL,IsWhere,nIsLogin);
		break;
	case item_medicine:			// ҩƷ��1
		 ItemGen.Gen_Medicine(nDetailType, nLevel, nVersion, pItem, nStackNum);
		break;
	case item_mine:				// ��ʯ��2
		 ItemGen.Gen_Mine(nParticularType, pItem, nStackNum, nLevel, nSeries,nLuck,pnJbLevel);
		break;
	case item_materials:		// ҩ����3
		break;
	case item_task:				// Ĭ����������4
		ItemGen.Gen_Quest(nDetailType, pItem, nStackNum,nLevel);
		break;
	case item_fusion:			// �Ƹ�
		ItemGen.Gen_Fusion(nParticularType,pItem,nStackNum,nLevel,nSeries,nLuck,pnJbLevel);
		break;
	case item_townportal:       // �س�5
		ItemGen.Gen_TownPortal(pItem);
		break;
	default:
		break;
	}
#ifdef _SERVER
	SetID(i);
#endif
	pItem->SetTime(nYear,nMonth,nDay,nHour,nMin);  // ����ʱ������
	m_FreeIdxItemSet.Remove(i);                    //���ñ�ɾ��һ��
	m_UseIdxItemSet.Insert(i);                     //���ñ� ����һ��
	return i;
}
//�ͻ��˲���װ��
int KItemSet::AddCilentItem(int nItemGenre, int nSeries, \
			  int nLevel, int nLuck, int nDetailType/*=-1*/, \
			  int nParticularType/*=-1*/, int* pnMagicLevel, \
			  int nVersion/*=0*/, UINT nRandomSeed,int nStackNum, int nEnChance,\
			  int nPoint, int nYear, int nMonth, int nDay, int nHour,int nMin,\
			  IN int nRongpiont,IN int* pnRMagicLevel,IN int* pnJbLevel,\
			  IN int* pnMagType,IN int* pnMagVala,IN int* pnMagValb,IN int* pnMagValc,IN int IsWhere)
{
	int i = FindFreeItemSet();

	if (i<=0)
		return 0;

	KItem*	pItem = &Item[i];
    //int Goidid=0;
	pItem->m_GeneratorParam.nVersion    = nVersion;
	pItem->m_GeneratorParam.uRandomSeed = nRandomSeed;

	if (nLevel<=0)
		nLevel = 1;

	switch(nItemGenre)  //���Դ����װ������
	{
	case item_equip:// װ����
		ItemGen.Gen_EquipmentCilent(nDetailType, nParticularType,\
			                       nSeries, nLevel, pnMagicLevel,\
								   pnRMagicLevel, nLuck, nVersion,\
								   pItem, nEnChance,nPoint, \
								   nRongpiont,0,\
								   pnMagType,pnMagVala,\
								   pnMagValb,pnMagValc,IsWhere);
		break;
	case item_medicine:			// ҩƷ��
		ItemGen.Gen_Medicine(nDetailType, nLevel, nVersion, pItem, nStackNum);
		break;
	case item_mine:				// ��ʯ��
		ItemGen.Gen_Mine(nParticularType,pItem,nStackNum,nLevel,nSeries,nLuck,pnJbLevel);
		break;
	case item_materials:		// ҩ����
		break;
	case item_task:				// Ĭ����������
		ItemGen.Gen_Quest(nDetailType, pItem, nStackNum,nLevel);
		break;
	case item_fusion:		   // �Ƹ�
		//ItemGen.Gen_Fusion(nDetailType, pItem, nStackNum,nLevel);
		ItemGen.Gen_Fusion(nParticularType,pItem,nStackNum,nLevel,nSeries,nLuck,pnJbLevel);
		break;
	case item_townportal:
		ItemGen.Gen_TownPortal(pItem);
		break;
	default:
		break;
	}
#ifdef _SERVER
	SetID(i);
#endif

	pItem->SetTime(nYear,nMonth,nDay,nHour,nMin);  // ����ʱ������
	m_FreeIdxItemSet.Remove(i); //���ñ�ɾ��һ��
	m_UseIdxItemSet.Insert(i);  //���ñ� ����һ��
	return i;
}

/*!*****************************************************************************
// Function		: KItemSet::FindFree
// Purpose		:
// Return		: int
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemSet::FindFreeItemSet()
{
	return m_FreeIdxItemSet.GetNext(0);
}

void KItemSet::Remove(IN int nIdx)
{
	Item[nIdx].Remove();	         //��������Ϊ0
	m_UseIdxItemSet.Remove(nIdx);    //���ñ��Ƴ�
	m_FreeIdxItemSet.Insert(nIdx);   //δ�ñ�����
}

void KItemSet::SetID(IN int nIdx)
{
	Item[nIdx].SetID(m_dwIDCreator);
	m_dwIDCreator++;
}

#ifdef _SERVER
//---------------------------------------------------------------------------
//	���ܣ��������� �Է���Ʒ������
//---------------------------------------------------------------------------
void	KItemSet::BackItemInfo()
{
	_ASSERT(this->m_psItemInfo);
	_ASSERT(this->m_psBackItemInfo);
	if (!m_psItemInfo)
		return;
	if (!m_psBackItemInfo)
		m_psBackItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];

	memcpy(m_psBackItemInfo,this->m_psItemInfo,sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);
}
#endif
//��ȡĥ�����
int KItemSet::GetAbradeRange(IN int nType, IN int nPart)
{
	if (nType < 0 || nType >= enumAbradeNum)
		return 0;
	if (nPart < 0 || nPart >= itempart_num)
		return 0;

	return m_nItemAbradeRate[nType][nPart];
}
