//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemGenerator.CPP
// Date:	2002.08.26
// Code:	DongBo
// Desc:    CPP file. ���ļ�ʵ�ֵ����������ɵ���
//---------------------------------------------------------------------------
#include "engine/KEngine.h"
#include "KCore.h"
//#include "MyAssert.H"
#include "KItem.h"
#include "KSubWorldSet.h"
#include "KItemGenerator.h"

//#define	 TABFILE_MAGICATTRIB_PATH	"\\settings\\item\\004\\magicattrib.txt" //��װħ������
//#define	 TABFILE_MAGICAGOLD_PATH	"\\settings\\item\\004\\GoldMagic.txt"   //�ƽ�ħ������

KItemGenerator	ItemGen;			//	װ��������

KItemGenerator::KItemGenerator()
{
}

KItemGenerator::~KItemGenerator()
{
}

/******************************************************************************
	���ܣ�	װ�����ݳ�ʼ��. ��tab file�ж�ȡ����
******************************************************************************/
BOOL KItemGenerator::Init()   
{
	if (!m_BPTLib.Init())		// �˵��ô����ɵ�tab file���������еĳ�ʼ����
	{
		//printf(" KItemGenerator::m_BPTLib() error!!!..\n");
		ccMessageBox("m_BPTLib() error!","KItemGenerator::Init");
		return FALSE;
	}
	for (int i = equip_meleeweapon; i < equip_detailnum - equip_meleeweapon; ++i)
	{//��װ���� ����
		 switch(i)
		 {//��ȡ�����ݵ����� ��TXT������
		case equip_meleeweapon:
			m_EquipNumOfEntries[i] = m_BPTLib.GetMeleeWeaponRecordNumber();
			break;
		case equip_rangeweapon:
			m_EquipNumOfEntries[i] = m_BPTLib.GetRangeWeaponRecordNumber();
			break;
		case equip_armor:
			m_EquipNumOfEntries[i] = m_BPTLib.GetArmorRecordNumber();
			break;
		case equip_helm:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHelmRecordNumber();
			break;
		case equip_boots:
			m_EquipNumOfEntries[i] = m_BPTLib.GetBootRecordNumber();
			break;
		case equip_belt:
			m_EquipNumOfEntries[i] = m_BPTLib.GetBeltRecordNumber();
			break;
		case equip_cuff:
			m_EquipNumOfEntries[i] = m_BPTLib.GetCuffRecordNumber();
			break;
		case equip_amulet:
			m_EquipNumOfEntries[i] = m_BPTLib.GetAmuletRecordNumber();
			break;
		case equip_ring:
			m_EquipNumOfEntries[i] = m_BPTLib.GetRingRecordNumber();
			break;
		case equip_pendant:
			m_EquipNumOfEntries[i] = m_BPTLib.GetPendantRecordNumber();
			break;
		case equip_horse:
			m_EquipNumOfEntries[i] = m_BPTLib.GetHorseRecordNumber();
			break;
		case equip_mask:	// mat na
			m_EquipNumOfEntries[i] = m_BPTLib.GetMaskRecordNumber();
			break;		
		case equip_pifeng:	// ����
			m_EquipNumOfEntries[i] = m_BPTLib.GetPifengRecordNumber();
			break;
		case equip_yinjian:	// 
			m_EquipNumOfEntries[i] = m_BPTLib.GetYinjianRecordNumber();
			break;
		case equip_shiping:	// 
			m_EquipNumOfEntries[i] = m_BPTLib.GetShipinRecordNumber();
			break;			
	
		 } 
	} 
	  m_MedNumOfEntries = m_BPTLib.GetMedicineRecordNumber();
	  //printf(" KItemGenerator::Init(%d) OK!!!..\n",equip_detailnum-equip_meleeweapon);
	
	return TRUE;
}

/******************************************************************************
	���ܣ�	����ҩƷ
	���:	nDetailType: ҩƷ����
			nLevel: �ȼ�
			nVersion: �汾��
	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и���
			ʧ��ʱ������
******************************************************************************/
BOOL KItemGenerator::Gen_Medicine(IN int nDetailType,
								  IN int nLevel,
								  IN int nVersion,
								  IN OUT KItem* pItem,
								  IN int nStackNum)
{
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if  (this==NULL || pItem==NULL)
		return false;

	BOOL bEC = FALSE;
	
#ifdef _SERVER	// �������汾
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = 0;
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	int nGameVersion = pItem->m_GeneratorParam.nVersion;
#endif

	const int i = nDetailType * 5 + nLevel - 1;	// ����ʽ�ɲ߻�����
												// �ȼ����ݴ�1��ʼ������Ҫ������
	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);
	if (NULL == pMed)
	{
		//_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pMed;
	pItem->SetStackNum(nStackNum);              //���ÿɵ�������
	return TRUE;
}
/******************************************************************************
	���ܣ�	����ҩƷ=--�̵��õ�
	���:	nDetailType: ҩƷ����
			nLevel: �ȼ�
			nVersion: �汾��
	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и���
			ʧ��ʱ������
******************************************************************************/
BOOL KItemGenerator::Gen_MedicineShop(IN int nDetailType,
								  IN int nLevel,
								  IN int nVersion,
								  IN OUT KItem* pItem,
								  IN int nStackNum,
								  int PerXu,int nIsBang,int nPriceType)
{
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if  (this==NULL || pItem==NULL)
			return false;

	BOOL bEC = FALSE;
	
#ifdef _SERVER	// �������汾
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = 0;
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	int nGameVersion = pItem->m_GeneratorParam.nVersion;
#endif

	const int i = nDetailType * 5 + nLevel - 1;	// ����ʽ�ɲ߻�����
												// �ȼ����ݴ�1��ʼ������Ҫ������
	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);  //������Ϣ
	if (NULL == pMed)
	{
		//_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pMed;
	pItem->SetStackNum(nStackNum); //���ÿɵ�������
    pItem->SetBang(nIsBang);

	if (PerXu>0)
	{//���������� �͸ı�ԭʼ����
       pItem->SetPerXu(PerXu);
	   pItem->SetPerPrice(PerXu);
	}

	pItem->SetModel(nPriceType);
	return TRUE;
}

/******************************************************************************
	���ܣ�	�������װ��
	���:	nSeriesReq: ��������
			nLevel: �ȼ�
			nLucky: ����ֵ
			nVersion: �汾��
	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и���
			ʧ��ʱ������
******************************************************************************/
/*BOOL KItemGenerator::Gen_Item(IN int nItemGenre,
							  IN int nSeriesReq,
							  IN int nLevel,
							  IN int nLucky,
							  IN OUT KItem* pItem,
							  IN int nStackNum,
							  IN int nEnChance)
{
	_ASSERT(this != NULL);
	_ASSERT(pItem != NULL);

	switch (nItemGenre)
	{
	case item_equip:
		{
			int nDetailType, nParticularType, i;
			nDetailType = g_Random(equip_detailnum);
			nParticularType = g_Random(m_EquipNumOfEntries[nDetailType] + 1);
			int naryMALevel[6];
			ZeroMemory(naryMALevel, 6 * sizeof(int));
			for (i = 0; i < 6; ++i)
			{
				naryMALevel[i] = g_Random(nLevel / 10 + 1);
				if (!naryMALevel[i])
					break;
			}
			Gen_Equipment(nDetailType, nParticularType, nSeriesReq, nLevel, naryMALevel, nLucky, g_SubWorldSet.GetGameVersion(), pItem, nEnChance,0,0);
		}
		break;
	case item_medicine:
		{
			int nDetailType;
			nDetailType = g_Random(medicine_detailnum);
			Gen_Medicine(nDetailType, nLevel, g_SubWorldSet.GetGameVersion(), pItem,nStackNum);
		}
		break;
	case item_task:
		{
			Gen_Quest(nSeriesReq, pItem,nStackNum,nLevel);			
		}
		break;
	case item_townportal:
		{
			Gen_TownPortal(pItem);
		}
		break;
	case item_mine:
		{
			int nParticularType = g_Random(500);
			int nJaryLevel[6];
			Gen_Mine(nParticularType, pItem,nStackNum,nLevel,nSeriesReq,nLucky,nJaryLevel);			
		}
		break;
	default:
		break;
	}
	
	return TRUE;
} */

/******************************************************************************
	���ܣ�	����װ��  ---����������

	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и��� �̵� --�������װ��
			ʧ��ʱ������
******************************************************************************/
BOOL KItemGenerator::Gen_paiEquipment(IN int nDetailType,\
								   IN int nParticularType,\
								   IN int nSeriesReq,\
								   IN int nLevel,\
								   IN const int* pnaryMALevel,\
								   IN const int* pnaryRMALevel,\
								   IN UINT nSeed,\
								   IN OUT KItem* pItem, \
								   IN int nEnChance,\
								   IN int nPoint,\
								   IN int nRongpoint,\
								   IN int IsWhere,IN BOOL nIsShop,\
								   IN int nPrice,IN int nFsxu,int nIsBang,\
								   IN int nPriceType,IN int nLucky)
{

	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if  (this==NULL || !pItem==NULL)
		return false;

	BOOL bEC = FALSE;

#ifdef _SERVER	// �������˰汾
	return bEC;
#endif

#ifndef _SERVER	// �ͻ��˰汾

	pItem->m_GeneratorParam.uRandomSeed = nSeed;

	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);       //���ÿͻ��˵��������

	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
	if (pnaryRMALevel)
		memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, pnaryRMALevel, sizeof(int) * 6);


	/*for (int k = 0;k < 6;k++)
	{ 
		char nMsgInfo[64]={0};
		if (pItem->m_GeneratorParam.nRGeneratorLevel[k] > 0)
		{ 
			sprintf(nMsgInfo,"nRGenerator:%d",pItem->m_GeneratorParam.nRGeneratorLevel[k]);
		}
		if (pItem->m_GeneratorParam.nGeneratorLevel[k] > 0)
		{ 
			sprintf(nMsgInfo,"nGenerat--:%d",pItem->m_GeneratorParam.nGeneratorLevel[k]);
		}
	}
	return bEC;*/

	pItem->m_GeneratorParam.nLuck = nLucky;
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();//�������װ��������ֵ �浵
	int nGameVersion = pItem->m_GeneratorParam.nVersion;

	// ������ڲ���, ȷ��װ���Ļ�������
    int i = nParticularType * 10 + nLevel - 1;// ����ʽ�ɲ߻�����
												// ���SPE 2002��8��31��7:40
												// email: ����������ֵ��
												// �ȼ����ݴ�1��ʼ������Ҫ������
	if (nDetailType==equip_mask)
	{
		i = nParticularType;
	}

	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{//��ȡԭʼװ������
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:	// ���
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_pifeng:	// ����
		pEqu = m_BPTLib.GetPifengRecord(i);
		break;
	case equip_yinjian:	// ӡ��
		pEqu = m_BPTLib.GetYinjianRecord(i);
		break;
	case equip_shiping:	// ��Ʒ
		pEqu = m_BPTLib.GetShipinRecord(i);
		break;
	default:
		break;
	}

	if (NULL == pEqu)
	{//��ȡԭʼ����ʧ��
	  //_ASSERT(FALSE);
	  return NULL; 
	}  
		// �������˵�ֱ��ԭ��: ֻ��n��װ��, ������ i ��ֵ��[0,n-1]֮��
		// ���3��: nParticularType ����?
		//			nLevel ����?
		//			ԭʼ��tab file������ m_BPTLib.m_BPTEquipment ��
		//			���������������?
	pItem->SetAttrib_CBR(pEqu);  //���û������� ����������
	pItem->SetSeries(nSeriesReq);
	pItem->SetModel(nPriceType);

	// ȷ��װ����ħ������ һ��װ��
	if (nPoint == 0)  //��װ �� ��װ  ���
	{
		if (NULL == pnaryMALevel)
		    return NULL;

		KItemNormalAttrib	sMA[6];	// ���ߵ�ħ������
		bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
		if (bEC)
		   pItem->SetAttrib_MA(sMA);  //����ħ������
		//pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
		   pItem->EnChance(nEnChance);

		if (nIsShop)
		{
			if (nFsxu>0)
			   pItem->SetPerXu(nFsxu);
			if (nPrice>0)
	           pItem->SetPerPrice(nPrice);

			pItem->SetBang(nIsBang);
		}

		///����Ϊ�����Ƹ�����
		KItemNormalAttrib	sMAR[6];// ħ��
		ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
		KTabFile MagicTabR;
		MagicTabR.Load(TABFILE_MAGICAGOLD_PATH);  //������װ��ħ�������ļ�

		if (IsWhere>0)   //��װ�ı�ʶ���� ������� ��װ����������
			nRongpoint = IsWhere;

		int nTypeR,nMaxR,nMinR;
		for (int r = 0;r < 6;r++)
		{ 
			if (pItem->m_GeneratorParam.nRGeneratorLevel[r] > 0 && r < nRongpoint)
			{ 
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],5,0,&nTypeR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],6,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],7,0,&nMinR);
				sMAR[r].nAttribType = nTypeR;  //��������
				//sMAR[r].nValue[0] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[0] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],8,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],9,0,&nMinR);
				//sMAR[r].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[1] = GetRandomNumber(nMaxR,nMinR);

				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],10,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],11,0,&nMinR);
				//sMAR[r].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[2] = GetRandomNumber(nMaxR,nMinR);
				continue;
			}
			sMAR[r].nAttribType = 0;
			sMAR[r].nValue[0] = 0;
			sMAR[r].nValue[1] = 0;
			sMAR[r].nValue[2] = 0;
		} 

		MagicTabR.Clear();

		pItem->SetAttrib_RON(sMAR);   //��������ħ������
		pItem->SetIsWhere(IsWhere); 
		pItem->SetRPoint(nRongpoint); 		
	    return bEC;
	}
	else if (nPoint ==7) 
	{
		KItemNormalAttrib sMA[6];
		ZeroMemory(sMA,sizeof(sMA)); //�����ڴ�
		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);
		int nType,nMax,nMin;
		for (int j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 0)
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;

				sMA[j].nValue[0] = GetRandomNumber(nMax,nMin);

				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);

				sMA[j].nValue[1] = GetRandomNumber(nMax,nMin);

				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);

				sMA[j].nValue[2] = GetRandomNumber(nMax,nMin);

                if (sMA[j].nValue[2]>1)
				{
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;

						nRows = nMax+IsWhere;
						if (nRows>nMin)
                            nRows=nMin;

						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMA[j].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
						
						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMA[j].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
                    nJnLevel.Clear();
				}
                
				continue;
			}
			//ʣ�µ��趨Ϊ�տ�
			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}
		pItem->SetAttrib_MA(sMA);  //����ħ������
		pItem->SetPoint(nPoint);   //��ֵ�� Point 	
		pItem->SetIsWhere(IsWhere);
		//pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
		pItem->EnChance(nEnChance);	
		if (nIsShop)
		{
			if (nFsxu>0)
			   pItem->SetPerXu(nFsxu);
			if (nPrice>0)
				pItem->SetPerPrice(nPrice);
			pItem->SetBang(nIsBang);
		}
		tabMagicAttribe.Clear();
		return bEC;
	}
	else if (nPoint ==8) 
	{
		int bECR=0;

		///����Ϊ�����Ƹ�����
		KItemNormalAttrib	sMAR[6];// ħ��
		ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
		KTabFile MagicTabR;
		MagicTabR.Load(TABFILE_MAGICATTRIB_PATH);  //������װ��ħ�������ļ�

		int nTypeR,nMaxR,nMinR;
		for (int r = 0;r < 6;r++)
		{ 
			if (pItem->m_GeneratorParam.nRGeneratorLevel[r] > 0 && r < nRongpoint)
			{ 
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],5,0,&nTypeR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],6,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],7,0,&nMinR);
				sMAR[r].nAttribType = nTypeR;  //��������
				//sMAR[r].nValue[0] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[0] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],8,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],9,0,&nMinR);
				//sMAR[r].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[1] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],10,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],11,0,&nMinR);
				//sMAR[r].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[2] = GetRandomNumber(nMaxR,nMinR);

				if (sMAR[r].nValue[2]>1)
				{
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;
						nRows = sMAR[r].nValue[2]+IsWhere;
						if (nRows>nMinR)
							nRows=nMinR;

						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMAR[r].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 

						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMAR[r].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
					nJnLevel.Clear();
				} 
				continue;
			}
			sMAR[r].nAttribType = 0;
			sMAR[r].nValue[0]   = 0;
			sMAR[r].nValue[1]   = 0;
			sMAR[r].nValue[2]   = 0;
			//  bEC=TRUE;
		} 

		MagicTabR.Clear();

		bECR= pItem->SetAttrib_RON(sMAR);   //��������ħ������
		pItem->SetRPoint(nRongpoint); 		

		if (nIsShop)
		{
			if (nFsxu>0)
				pItem->SetPerXu(nFsxu);
			if (nPrice>0)
				pItem->SetPerPrice(nPrice);

			pItem->SetBang(nIsBang);
		}
		//		pItem->EnChanceRong(nEnChance);
		return bECR;
	}
	else //��װ ����
	{
		KItemNormalAttrib sMA[6];
		ZeroMemory(sMA,sizeof(sMA)); //�����ڴ�
		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);
		int nType,nMax,nMin,j;
		for (j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 0 && j < nPoint)
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;
				sMA[j].nValue[0] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);
				sMA[j].nValue[1] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);
				sMA[j].nValue[2] = GetRandomNumber(nMax,nMin);

				if (sMA[j].nValue[2]>1)
				{
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;

						nRows = nMax+IsWhere;
						if (nRows>nMin)
							nRows=nMin;

						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMA[j].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 

						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMA[j].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
					nJnLevel.Clear();
				}

				continue;
			}
			//ʣ�µ��趨Ϊ�տ�
			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}
		pItem->SetAttrib_MA(sMA);  //����ħ������
		pItem->SetPoint(nPoint);   //��ֵ�� Point �ж��Ƿ�����װ		
		tabMagicAttribe.Clear();
		pItem->EnChance(nEnChance);	

		if (nIsShop)
		{
			if (nFsxu>0)
			   pItem->SetPerXu(nFsxu);
			if (nPrice>0)
				pItem->SetPerPrice(nPrice);

			pItem->SetBang(nIsBang);
		}

		//----------------------�Ƹֵ�����
		///����Ϊ�����Ƹ�����
		KItemNormalAttrib	sMAR[6];  // ħ��
		ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
		KTabFile MagicTabR;
		MagicTabR.Load(TABFILE_MAGICAGOLD_PATH);  //������װ��ħ�������ļ�		
		int nTypeR,nMaxR,nMinR;

		for (j = 0;j < 6;j++)
		{ 
			if (pItem->m_GeneratorParam.nRGeneratorLevel[j] > 0 && j < nRongpoint)
			{ 
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],5,0,&nTypeR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],6,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],7,0,&nMinR);
				sMAR[j].nAttribType = nTypeR;  //��������
				sMAR[j].nValue[0] = GetRandomNumber(nMaxR,nMinR);//nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],8,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],9,0,&nMinR);
				sMAR[j].nValue[1] = GetRandomNumber(nMaxR,nMinR);
				//sMAR[j].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],10,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],11,0,&nMinR);
				//sMAR[j].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[j].nValue[2] = GetRandomNumber(nMaxR,nMinR);
				continue;
			}
			sMAR[j].nAttribType = 0;
			sMAR[j].nValue[0] = 0;
			sMAR[j].nValue[1] = 0;
			sMAR[j].nValue[2] = 0;
		} 

		MagicTabR.Clear();
		bEC=pItem->SetAttrib_RON(sMAR);   //��������ħ������
		pItem->SetRPoint(nRongpoint); 

	    return bEC;
	}
#endif

}

/******************************************************************************
	���ܣ�	�������װ��  ---�����̳ǻ��̵�

	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и��� �̵� --�������װ��
			ʧ��ʱ������
******************************************************************************/
BOOL KItemGenerator::Gen_Equipment(IN int nDetailType,\
								   IN int nParticularType,\
								   IN int nSeriesReq,\
								   IN int nLevel,\
								   IN const int* pnaryMALevel,\
								   IN int nLucky,\
								   IN int nVersion,\
								   IN OUT KItem* pItem, \
								   IN int nEnChance,\
								   IN int nPoint,\
								   IN int nRongpoint,\
								   IN int IsWhere,IN BOOL nIsShop,\
								   IN int nPrice,IN int nFsxu,int nIsBang,int nPriceType)
{
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if  (this==NULL || pItem==NULL)
			return false;

	BOOL bEC = FALSE;
#ifdef _SERVER	// �������汾
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed(); //ȡ���������
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6); //ħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = nLucky;	                 //�������װ��������ֵ �浵
#else												
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);       //���ÿͻ��˵��������
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
#endif
	int nGameVersion = pItem->m_GeneratorParam.nVersion;

	// ������ڲ���, ȷ��װ���Ļ�������
    int i = nParticularType * 10 + nLevel - 1;// ����ʽ�ɲ߻�����
												// ���SPE 2002��8��31��7:40
												// email: ����������ֵ��
												// �ȼ����ݴ�1��ʼ������Ҫ������
	if (nDetailType==equip_mask)
	{
		i = nParticularType;
	}

	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{//��ȡԭʼװ������
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:	// ���
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_pifeng:	// ����
		pEqu = m_BPTLib.GetPifengRecord(i);
		break;
	case equip_yinjian:	// ӡ��
		pEqu = m_BPTLib.GetYinjianRecord(i);
		break;
	case equip_shiping:	// ��Ʒ
		pEqu = m_BPTLib.GetShipinRecord(i);
		break;
	default:
		break;
	}

	if (NULL == pEqu)
	{//��ȡԭʼ����ʧ��
	  //_ASSERT(FALSE);
	  return NULL; 
	}  
		// �������˵�ֱ��ԭ��: ֻ��n��װ��, ������ i ��ֵ��[0,n-1]֮��
		// ���3��: nParticularType ����?
		//			nLevel ����?
		//			ԭʼ��tab file������ m_BPTLib.m_BPTEquipment ��
		//			���������������?
	pItem->SetAttrib_CBR(pEqu);  //���û������� ����������
	pItem->SetSeries(nSeriesReq);
	pItem->SetModel(nPriceType);

	// ȷ��װ����ħ������ һ��װ��
	if (nPoint == 0)  //��װ �� ��װ  ���
	{
		if (NULL == pnaryMALevel)
		return NULL;
		KItemNormalAttrib	sMA[6];	// ���ߵ�ħ������
		bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
		if (bEC)
		   pItem->SetAttrib_MA(sMA);  //����ħ������
		//pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
		   pItem->EnChance(nEnChance);

		if (nIsShop)
		{
			if (nFsxu>0)
			   pItem->SetPerXu(nFsxu);
			if (nPrice>0)
	           pItem->SetPerPrice(nPrice);

			pItem->SetBang(nIsBang);
		}
	    return bEC;
	}
	else if (nPoint ==7) 
	{
		KItemNormalAttrib sMA[6];
		ZeroMemory(sMA,sizeof(sMA)); //�����ڴ�
		
		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);
		
		if (nLucky > 10)//0--10֮��
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}
		
		int nType,nMax,nMin;
		for (int j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 0)
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;

				sMA[j].nValue[0] = nMax +((nMin - nMax) * nLucky / 10);

				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);

				sMA[j].nValue[1] = nMax +((nMin - nMax) * nLucky / 10);

				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);

				sMA[j].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);

                if (sMA[j].nValue[2]>1)
				{
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;

						nRows = nMax+IsWhere;
						if (nRows>nMin)
                            nRows=nMin;

						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMA[j].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
						
						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMA[j].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
                    nJnLevel.Clear();
				}
                
				continue;
			}
			//ʣ�µ��趨Ϊ�տ�
			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}
		pItem->SetAttrib_MA(sMA);  //����ħ������
		pItem->SetPoint(nPoint);   //��ֵ�� Point 	
		pItem->SetIsWhere(IsWhere);
		//pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
		pItem->EnChance(nEnChance);	
		if (nIsShop)
		{
			if (nFsxu>0)
			   pItem->SetPerXu(nFsxu);
			if (nPrice>0)
				pItem->SetPerPrice(nPrice);
			pItem->SetBang(nIsBang);
		}
		tabMagicAttribe.Clear();
		return bEC;
	}
	else //��װ ����
	{
		KItemNormalAttrib sMA[6];
		ZeroMemory(sMA,sizeof(sMA)); //�����ڴ�

		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);

		if (nLucky > 10)//0--10֮��
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}

		int nType,nMax,nMin;
		for (int j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 0 && j < nPoint)
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;
				sMA[j].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);
				sMA[j].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);
				sMA[j].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				continue;
			}
			//ʣ�µ��趨Ϊ�տ�
			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}
		pItem->SetAttrib_MA(sMA);  //����ħ������
		pItem->SetPoint(nPoint);   //��ֵ�� Point �ж��Ƿ�����װ		
		tabMagicAttribe.Clear();
		pItem->EnChance(nEnChance);	

		if (nIsShop)
		{
			if (nFsxu>0)
			   pItem->SetPerXu(nFsxu);
			if (nPrice>0)
				pItem->SetPerPrice(nPrice);

			pItem->SetBang(nIsBang);
		}
	    return bEC;
	}
}
/******************************************************************************
���ܣ�	����ħ������
���:	nType: װ������
pnaryMALevel: ����ָ��, ����ħ���ĵȼ�Ҫ��, ��6��
nSeriesReq: ��������
nLucky: ����ֵ
����:	�� pnaryMA ��ָ�����з���ħ�������Ժ�������, ��6��
******************************************************************************/
// Add by Freeway Chen in 2003.5.30
BOOL KItemGenerator::Gen_MagicAttrib(int nType, const int* pnaryMALevel,\
									         int nSeriesReq, int nLucky,\
						   KItemNormalAttrib* pnaryMA, int nGameVersion)
{
    int nResult = false;

    KBPT_ClassMAIT  SelectedMagicTable;	
	// ��¼ÿһ����¼ѡ�е�ħ�����ԣ���Ҫ��Ϊ�����ʹ�ñ�־���
    KMAGICATTRIB_TABFILE *pMagicAttrTable[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

    int i = 0;

    //_ASSERT(pnaryMALevel);
    //_ASSERT(pnaryMA);
    if  (pnaryMALevel==NULL || pnaryMA==NULL)
    		return false;

    for (i = 0; i < 6; ++i) 
    {
        // �������ħ������Ϊ�㣬��ʾ����Ҫ���������ˣ��˳� 
        if (pnaryMALevel[i] == 0)	
        	break;

		/*if (pnaryMALevel[i]-10 >0)	
        {//��������
		int nnMgaRows = pnaryMALevel[i]-10;
			//pnaryMA[i].nAttribType = pnaryMALevel[i];
		 	
			KTabFile tabMagicAttribe;
			  tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);

			  
		    int nType,nMax,nMin;
	
			tabMagicAttribe.GetInteger(nnMgaRows,5,0,&nType);
			tabMagicAttribe.GetInteger(nnMgaRows,6,0,&nMax);
		    tabMagicAttribe.GetInteger(nnMgaRows,7,0,&nMin);
		    pnaryMA[i].nAttribType = nType;
		    pnaryMA[i].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				
		    tabMagicAttribe.GetInteger(nnMgaRows,8,0,&nMax);
			tabMagicAttribe.GetInteger(nnMgaRows,9,0,&nMin);
		    pnaryMA[i].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				  
			tabMagicAttribe.GetInteger(nnMgaRows,10,0,&nMax);
			tabMagicAttribe.GetInteger(nnMgaRows,11,0,&nMin);
			pnaryMA[i].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
			
			tabMagicAttribe.Clear();
					  
			if (pnaryMA[i].nValue[2]>1)
			{ 
				nnMgaRows =	pnaryMA[i].nValue[2];

				KTabFile nmpLevel;
				if (nmpLevel.Load(TABFILE_MAGICAGOLD_PATH))
				{  
					int nMinVal=0,nMaxVal=0;
	
					nmpLevel.GetInteger(nnMgaRows,"����1��Сֵ",0,&nMinVal);
					nmpLevel.GetInteger(nnMgaRows,"����1���ֵ",0,&nMaxVal);
					pnaryMA[i].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
						  
					nmpLevel.GetInteger(nnMgaRows,"����3��Сֵ",0,&nMinVal);
					nmpLevel.GetInteger(nnMgaRows,"����3���ֵ",0,&nMaxVal);
					pnaryMA[i].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
				}
				nmpLevel.Clear();
			}
		  
		  continue;
		}
	    */
        SelectedMagicTable.Clear();

        // ����ǰ׺��׺����Ʒ���͡������������󼶱����з���������ħ��������ȡ����
        const KBPT_ClassMAIT *pCMITItem = m_BPTLib.GetCMIT(
            1 - (i & 1),        // ǰ׺��׺	��������
            nType,              // ��Ʒ���� ����  ñ�� ���ӵ�
            nSeriesReq,         // �������� 
            pnaryMALevel[i]     // ħ�����Ե���  1-10  ���Եȼ�
        );
			
        //_ASSERT(pCMITItem);
        if (!pCMITItem)
               break;
        // ��������������ħ�����ԣ�����
        // 1.�Ƿ��Ѿ�ʹ�ã�
        // 2.����ֵ
        // 3.�Ƿ�����ͬ�ġ����Ե������
        // ȡ����Ӧ�������SelectedMagicTable��
        int nCMITItemCount = pCMITItem->GetCount();  //��ȡ����������ħ����������
        int j = 0; 
		int nMinDropRate,nMaxDropRate;
        g_GameSetTing.GetInteger2("SYSTEM","ItemDropRate",&nMinDropRate,&nMaxDropRate);

        int nDecide = GetRandomNumber(nMinDropRate,nMaxDropRate-1)/(1 + nLucky * 20/100);  //����ֵ 10000 500000=25/1250||3000/160000
		for (j = 0; j < nCMITItemCount; ++j)
        {//������������
            // ��������ֵ��ȡ����Ӧ����
			//int nIsThis=GetRandomNumber(0,nCMITItemCount-1); //���ȡһ����������
            int nMAIndex = pCMITItem->Get(j);  //��ȡħ����������� ����
            KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(nMAIndex);
            if (!pMAItem)
            {
               // _ASSERT(pMAItem);
                continue;
            }

            // ����Ѿ�ʹ�ã�����ȡ��һ��
            if (pMAItem->m_nUseFlag)
                continue;

            // ����������㣬ȡ��һ��
            if ((pMAItem->m_DropRate[nType]) <=1 || (pMAItem->m_DropRate[nType]) <= nDecide) //25/1250||3000/160000  10--2000
                continue; //ͣ����ִ�У�ִ����һ��ѭ������ͷ��ʼfor�� 
            
            // ����Ƿ��Ѿ�����ͬ�ġ�ħ�����Ե����ࡱ
            int k = 0;
            for (k = 0; k < i; ++k)
            {
                //_ASSERT(pMagicAttrTable[k]);
                if (!pMagicAttrTable[k])
                	continue;

                if ((pMagicAttrTable[k]->m_MagicAttrib.nPropKind) == (pMAItem->m_MagicAttrib.nPropKind))
                    break;  //��ͬ������
            }

            // ������� �����Ե��������ͬ��˵���Ѿ���һ����ͬ��ֵ��ѡ�У���ôȡ��һ��
            if (k < i)
                continue;

            SelectedMagicTable.Insert(nMAIndex);  //ѡ����� ��� ħ����������
        }

        int nSelectedCount = SelectedMagicTable.GetCount();   //��ȡ�Ѿ��������Ե�����

        if (nSelectedCount == 0)
        {   
            // ���û������������ħ�����ԣ����˳�
            break;
        }
        
        // �����������������ħ�����ԣ�������
        int nLuckyItemIndex = GetRandomNumber(0,nSelectedCount-1);//ȡ���ϵ����һ��
        KMAGICATTRIB_TABFILE *pMAItem = (KMAGICATTRIB_TABFILE *)m_BPTLib.GetMARecord(SelectedMagicTable.Get(nLuckyItemIndex));
        // ��������������ħ�����ԣ��Ա�������ʱ��ʹ�ñ�־���
        pMagicAttrTable[i] = pMAItem;

        //_ASSERT(pMAItem);
        //_ASSERT(!(pMAItem->m_nUseFlag));     // ȷ����û��ʹ��
        if (pMAItem==NULL)
        	break;
		if (pMAItem->m_nUseFlag)
			break;
        // ����ʹ�ñ�־
        pMAItem->m_nUseFlag = true;
        pnaryMA[i].nAttribType = pMAItem->m_MagicAttrib.nPropKind;  //ħ�����Ե�����
        pnaryMA[i].nValue[0] = GetRandomNumber(pMAItem->m_MagicAttrib.aryRange[0].nMin,pMAItem->m_MagicAttrib.aryRange[0].nMax);
        pnaryMA[i].nValue[1] = GetRandomNumber(pMAItem->m_MagicAttrib.aryRange[1].nMin,pMAItem->m_MagicAttrib.aryRange[1].nMax);
        pnaryMA[i].nValue[2] = GetRandomNumber(pMAItem->m_MagicAttrib.aryRange[2].nMin,pMAItem->m_MagicAttrib.aryRange[2].nMax);

		if (pnaryMA[i].nValue[2]>1)
		{
			KTabFile nJnLevel;
			if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
			{  
				int nMinVal=0,nMaxVal=0;

				nJnLevel.GetInteger(pnaryMA[i].nValue[2],"����1��Сֵ",0,&nMinVal);
                nJnLevel.GetInteger(pnaryMA[i].nValue[2],"����1���ֵ",0,&nMaxVal);
                pnaryMA[i].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 

                nJnLevel.GetInteger(pnaryMA[i].nValue[2],"����3��Сֵ",0,&nMinVal);
                nJnLevel.GetInteger(pnaryMA[i].nValue[2],"����3���ֵ",0,&nMaxVal);
                pnaryMA[i].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
			}
            nJnLevel.Clear();
		}

    } // for ������Ҫȡ�ص�ħ�����Ա�

    for (NULL; i < 6; ++i)
    {
        // ���ʣ�µ���
        pnaryMA[i].nAttribType = 0;
        pnaryMA[i].nValue[0] = 0;
        pnaryMA[i].nValue[1] = 0;
        pnaryMA[i].nValue[2] = 0;
    }

    for (i = 0; i < 6; ++i)
    {
        if (!pMagicAttrTable[i])
            break;

       //_ASSERT(pMagicAttrTable[i]->m_nUseFlag);   // ȷ���Ѿ�ʹ��
        //if (!pMagicAttrTable[i]->m_nUseFlag)
    	//  ccontinue;

        pMagicAttrTable[i]->m_nUseFlag = false;    // ����Ϊû��ʹ��
    }

    nResult = true;
	return nResult;
}

// Following Code is implement by DongBo
//BOOL KItemGenerator::Gen_MagicAttrib(int nType, const int* pnaryMALevel, int nSeriesReq, int nLucky,
//									 KItemNormalAttrib* pnaryMA)
//{
//	
//    // ȷ�������ڸ�װ����ȫ��ħ�����Լ�������
//	if (FALSE == GMA_GetAvaliableMA(nType))
//		{ _ASSERT(FALSE); return FALSE; }
//
//	// ȷ��������ħ�����Եĵȼ�,���ָ���
//	if (FALSE == GMA_GetLevelAndDropRate(nType))
//		{ _ASSERT(FALSE); return FALSE; }
//
//	// ������ħ��������ѡ������������. �߻�Ҫ��ѡ��6��
//	for (int i = 0; i < 6; ++i)				// ����: ǰ׺, ż��: ��׺
//	{
//		GMA_ChooseMA((i&1), pnaryMALevel[i], nLucky, &(pnaryMA[i]));
//		if (pnaryMA[i].nAttribType == 0)
//			break;
//	}
//	return TRUE;
//}

/******************************************************************************
	����:	ȷ��������ָ��װ����ȫ��ħ�����Լ�������
	���:	nType: ָ��װ��
	����:	�ɹ�ʱ���ط���. m_GMA_naryMA[0]���������ڸ�װ����ȫ��ħ������ǰ׺
							m_GMA_nCount[0]��������ħ�����Ե�����
							m_GMA_naryMA[1]���������ڸ�װ����ȫ��ħ�����Ժ�׺
							m_GMA_nCount[1]��������ħ�����Ե�����
			ʧ��ʱ������
******************************************************************************/
/*BOOL KItemGenerator::GMA_GetAvaliableMA(int nType)
{
	//TODO: ������Ҫ��nType����ת��, ��װ������ֵ��� m_BPTLib.m_CMAT ���������
	for (int i = 0; i < 2; ++i)		// 0: ǰ׺, 1: ��׺
	{
		const KBPT_ClassifiedMAT* pCMAT;
		pCMAT = m_BPTLib.GetCMAT(i, nType);
		if (pCMAT == NULL)
			{ _ASSERT(FALSE); return FALSE; }
		m_GMA_nCount[i] = NUMOFCMA;
		pCMAT->GetAll((int*)m_GMA_naryMA[i], &(m_GMA_nCount[i]));
	}
	return TRUE;
}
*/
/******************************************************************************
	����:	ȷ��������ħ�����Եĵȼ�Ҫ��ͳ��ָ���
	���:	nType: ָ��װ��
			m_GMA_naryMA����: ����ȫ��ħ�����Ե�����ֵ
	����:	�ɹ�ʱ���ط���. m_GMA_naryLevel�����������ħ�����Եĵȼ�Ҫ��
							m_GMA_naryDropRate��������ħ�����Եĳ��ָ���
			ʧ��ʱ������
******************************************************************************/
/*BOOL KItemGenerator::GMA_GetLevelAndDropRate(int nType)
{
	//TODO: ������Ҫ��nType����ת��, ��װ������ֵ��� m_BPTLib.m_CMAT ���������

	for (int i = 0; i < 2; ++i)		// 0: ǰ׺, 1: ��׺
	{
		for (int n = 0; n < m_GMA_nCount[i]; ++n)
		{
			const KMAGICATTRIB_TABFILE* pMATF;
			pMATF = GetMARecord(m_GMA_naryMA[i][n]);
			if (pMATF == NULL)
				{ _ASSERT(FALSE); return FALSE; }
			m_GMA_naryLevel[i][n] = pMATF->m_nLevel;
			m_GMA_naryDropRate[i][n] = pMATF->m_DropRate[nType];		// ���Ż�
		}
	}
	return TRUE;
}
/*
/******************************************************************************
	����:	��ȫ�����õ�ħ��������ѡ������������ħ��
	���:	nPos: 0: ѡǰ׺, 1: ѡ��׺
			nLevel: ��ħ�����Եĵȼ�Ҫ��
			nLucky: ����ֵ
			m_GMA_naryMA����: ����ȫ�����õ�ħ�����Ե�����ֵ
	����:	*pINA ������ѡħ���ĺ��Ĳ���
******************************************************************************/
/*void KItemGenerator::GMA_ChooseMA(int nPos, int nLevel, int nLucky, KItemNormalAttrib* pINA)
{
	_ASSERT(pINA != NULL);

	// ����ֵ
	pINA->nAttribType = 0;
	pINA->nValue[0] = 0;
	pINA->nValue[1] = 0;
	pINA->nValue[2] = 0;

	// ɸѡ��ȫ�����õ�ħ������
	int nDropRate = ::GetRandomNumber(0, 100) - nLucky;
	int nCount = GMA_GetCandidateMA(nPos, nLevel, nDropRate);
	if (nCount > 0)
	{
		int nFinal = ::GetRandomNumber(0, nCount);
		int nMAi = m_GMA_naryCandidateMA[nFinal];
		int nMA = m_GMA_naryMA[nPos][nMAi];
		if (nMA == -1)	// ������ظ���������
		{
			pINA->nAttribType = 0;
			return;
		}
		m_GMA_naryMA[nPos][nMAi] = -1;		// ��Ϊ-1,��ʾ��MA����

		const KMAGICATTRIB_TABFILE* pMATF = GetMARecord(nMA);
		if (NULL != pMATF)
		{
			const KMACP* pMACP = &(pMATF->m_MagicAttrib);
			pINA->nAttribType = pMACP->nPropKind;
			pINA->nValue[0] = ::GetRandomNumber(pMACP->aryRange[0].nMin, pMACP->aryRange[0].nMax);
			pINA->nValue[1] = ::GetRandomNumber(pMACP->aryRange[1].nMin, pMACP->aryRange[1].nMax);
			pINA->nValue[2] = ::GetRandomNumber(pMACP->aryRange[2].nMin, pMACP->aryRange[2].nMax);
		}
	}
}
*/
/******************************************************************************
	����:	��ȡָ����ħ�����Լ�¼
	���:	i: ָ����¼
	����:	�ɹ�ʱ����ָ��ü�¼��ָ��
			ʧ��ʱ����NULL
******************************************************************************/
const KMAGICATTRIB_TABFILE* KItemGenerator::GetMARecord(int i) const
{
	return m_BPTLib.GetMARecord(i);
}

/******************************************************************************
	����:	��ȫ�����õ�ħ��������ѡ������������ħ��
	���:	nPos: 0: ѡǰ׺, 1: ѡ��׺
			nLevel: ��ħ�����Եĵȼ�Ҫ��
			nDropRate: ѡ�����ָ��ʴ��ڴ�ֵ��ħ������
			m_GMA_naryMA����: ����ȫ�����õ�ħ�����Ե�����ֵ
			m_GMA_naryLevel����: ��������ħ�����Եĵȼ�Ҫ��
			m_GMA_naryDropRate����: ��������ħ�����Եĳ��ָ���
	����:	m_GMA_naryCandidateMA�����������������ħ��
	˵��:	�� n = m_GMA_naryMA[nPos][m_GMA_naryCandidateMA[i]]
			�� GetMARecord(n) ����ָ��ħ�����Խṹ��ָ��
******************************************************************************/
/*int KItemGenerator::GMA_GetCandidateMA(int nPos, int nLevel, int nDropRate)
{
	int nCount = 0;
	for (int i = 0; i < m_GMA_nCount[nPos]; ++i)
	{
		if (m_GMA_naryMA[nPos][i] != -1 &&		// -1��ʾ��MA���ù�
			m_GMA_naryLevel[nPos][i] == nLevel &&
			m_GMA_naryDropRate[nPos][i] >= nDropRate)
		{
			m_GMA_naryCandidateMA[nCount++] = i;
		}
	}
	return nCount;
}
 */
BOOL KItemGenerator::GetMedicineCommonAttrib(IN int nDetailType, IN int nLevel, IN OUT KItem* pItem)
{
	if (NULL == pItem)
	{
//		KASSERT(FALSE);
		return FALSE;
	}

	const int i = nDetailType * 5 + nLevel - 1;

	const KBASICPROP_MEDICINE* pMed = NULL;
	pMed = m_BPTLib.GetMedicineRecord(i);
	if (NULL == pMed)
	{
		//_ASSERT(FALSE);
		return FALSE;
	}
	*pItem = *pMed;
	return TRUE;
}

/******************************************************************************
	���ܣ�	������ɻƽ�װ��װ��
	���:	nCondition: ǰ��������(Emf+Pmf)*Level��������
			pItem: װ����Ŀָ�롣
	����:	�ɹ�ʱ������, ��������� pItem ��ָ�����и���
			ʧ��ʱ���ؼ�
******************************************************************************/
// Add by flying on May.30.2003
// Try to get a "Gold Item" if possible.
/*BOOL KItemGenerator::GetGoldItemByRandom(IN int nCondition, OUT KItem * pItem)
{
	BOOL bResult = FALSE;
	const KBASICPROP_EQUIPMENT_GOLD* pArray[MAX_ITEM];
	int i = 0;
	int j = 0;
	int nIndex = 0;

	::memset(pArray, 0, sizeof(KBASICPROP_EQUIPMENT_GOLD *) * MAX_ITEM);

	for (i = 0; i < MAX_ITEM; ++i)
	{
		const KBASICPROP_EQUIPMENT_GOLD* pTemp = m_BPTLib.GetGoldItemRecord(i);  //��ȡ�ƽ�װ��������
		if (pTemp == 0)
			break;
//		if (pTemp->m_nRarity < nCondition)
			pArray[j] = pTemp;
	}

	if (j == 0)
		goto Exit0;			 
	nIndex = GetRandomNumber(0, j);	
	*pItem = *pArray[nIndex];
Exit0:
	return FALSE;
} */
//ˢ�׽�װ��
BOOL KItemGenerator::GetPlatinaByIndex(IN int nIndex, OUT KItem * pItem, IN const int* pnay, IN int nSeries, IN int nEnChance,IN int nRongpiont,IN int nLevel,IN BOOL nIsLogin)
{
	int i = 0;
	int j = 0;
	int nCount = 0;
	
#ifdef _SERVER	// �������汾
	if (nIsLogin)
		g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	else
	    pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();

	if (pnay)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnay, sizeof(int) * 6);
	else
	{
		for (i = 0; i < 6;++i)
		{
			pItem->m_GeneratorParam.nGeneratorLevel[i] = ::GetRandomNumber(0,10); //ȡ���ֵ
		}
	}
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = 10;
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnay)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnay, sizeof(int) * 6);
	else
	{
		for (i = 0; i < 6;++i)
		{
			pItem->m_GeneratorParam.nGeneratorLevel[i] = ::GetRandomNumber(0,10); //ȡ���ֵ
		}
	}
#endif
	
	nCount = m_BPTLib.GetPlatinaNumber();// ���Ա��ж���������
	const KBASICPROP_EQUIPMENT_PLATINA* pTemp = m_BPTLib.GetPlatinaRecord(nIndex); //��ȡ�ƽ�ԭʼ����
	if (nIndex >= nCount)
		return FALSE;
	if (pTemp == NULL)
		return FALSE;

	pItem->SetAttrib_BCBR((KBASICPROP_EQUIPMENT_PLATINA*)pTemp); //���û������Ժ� ��������
	pItem->SetGoldId(nIndex);
	pItem->SetLevel(nLevel);
	
	if (nSeries<0 || nSeries > 4)
	{
		pItem->SetSeries(::GetRandomNumber(0,4)); //��������
	} 
	else
	{
		pItem->SetSeries(nSeries);
	}
	
	KItemNormalAttrib	sMA[6];
	KTabFile MagicTab;
	MagicTab.Load(TABFILE_MAGICAGOLD_PATH);   //�ƽ�װ��ħ�������ļ�
	for (i = 0; i < 6; ++i)
	{
		const int* pSrc;
		KItemNormalAttrib* pDst;
		pSrc = &(pTemp->m_aryMagicAttribs[i]); //���ܱ��
		pDst = &(sMA[i]);
		
		int nType,nLow,nHigh;
		MagicTab.GetInteger(*pSrc + 1,"���Ե������",0,&nType);
		pDst->nAttribType = nType;
		MagicTab.GetInteger(*pSrc + 1,"����1��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����1���ֵ",0,&nHigh);
		//pDst->nValue[0] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[0] =  GetRandomNumber(nLow,nHigh);
		MagicTab.GetInteger(*pSrc + 1,"����2��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����2���ֵ",0,&nHigh);
		//pDst->nValue[1] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[1] =  GetRandomNumber(nLow,nHigh);
		MagicTab.GetInteger(*pSrc + 1,"����3��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����3���ֵ",0,&nHigh);
		//pDst->nValue[2] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[2] =  GetRandomNumber(nLow,nHigh);
	}
	
	pItem->SetAttrib_MA(sMA);  //��ֵ�� �������� ---m_aryMagicAttrib[i]  ,����ħ������
    pItem->SetRPoint(nRongpiont);
	pItem->EnChance(nEnChance);

	KItemNormalAttrib	syMA[2];
	for (i = 0; i < 2; ++i)
	{//��������
		const int* pSrc;
		KItemNormalAttrib* pDst;
		pSrc = &(pTemp->m_yinMagicAttribs[i]); //���ܱ��
		pDst = &(syMA[i]);
		
		int nType,nLow,nHigh;
		MagicTab.GetInteger(*pSrc + 1,"���Ե������",0,&nType);
		pDst->nAttribType = nType;	//����
		MagicTab.GetInteger(*pSrc + 1,"����1��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����1���ֵ",0,&nHigh);
		//pDst->nValue[0] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[0] =  GetRandomNumber(nLow,nHigh);
		MagicTab.GetInteger(*pSrc + 1,"����2��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����2���ֵ",0,&nHigh);
		//pDst->nValue[1] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[1] =  GetRandomNumber(nLow,nHigh);
		MagicTab.GetInteger(*pSrc + 1,"����3��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����3���ֵ",0,&nHigh);
		//pDst->nValue[2] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[2] =  GetRandomNumber(nLow,nHigh);
	}
	pItem->SetAttrib_Yin(syMA); 
	MagicTab.Clear();
	return TRUE;
}


/******************************************************************************
	���ܣ�	����ָ���ƽ�װ��װ��   ---ˢ�ƽ�װ��  
	���:	nCondition: ǰ��������(Emf+Pmf)*Level��������
			pItem: װ����Ŀָ�롣
	����:	�ɹ�ʱ������, ��������� pItem ��ָ�����и���
			ʧ��ʱ���ؼ�
******************************************************************************/
BOOL KItemGenerator::GetGoldItemByIndex(IN int nIndex, OUT KItem * pItem, IN const int* pnay, IN int nSeries, IN int nEnChance,IN int nRongpiont,IN int nLevel,IN BOOL nIsLogin)
{
	int i = 0;
	int j = 0;
	int nCount = 0;

#ifdef _SERVER	// �������汾
	if (nIsLogin)
		g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	else
	   pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed();

	if (pnay)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnay, sizeof(int) * 6);
	else
	{
		for (i = 0; i < 6;++i)
		{
			pItem->m_GeneratorParam.nGeneratorLevel[i] = ::GetRandomNumber(0,10); //ȡ���ֵ
		}
	}
	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = 10;
#else
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnay)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnay, sizeof(int) * 6);
	else
	{
		for (i = 0; i < 6;++i)
		{
			pItem->m_GeneratorParam.nGeneratorLevel[i] = ::GetRandomNumber(0,10); //ȡ���ֵ
		}
	}
#endif

	nCount = m_BPTLib.GetGoldItemNumber();// ���Ա��ж���������
	const KBASICPROP_EQUIPMENT_GOLD* pTemp = m_BPTLib.GetGoldItemRecord(nIndex); //��ȡ�ƽ�ԭʼ����
	if (nIndex >= nCount)
		return FALSE;
	if (pTemp == NULL)
		return FALSE;
	pItem->SetAttrib_CBR((KBASICPROP_EQUIPMENT_GOLD*)pTemp); //���û������Ժ� ��������
	if (nSeries > 4 || nSeries < 0)
	{
		pItem->SetSeries(::GetRandomNumber(0,4)); //��������
	} 
	else
	{
		pItem->SetSeries(nSeries);
	}

	pItem->SetLevel(nLevel);

	KItemNormalAttrib	sMA[6];
	KTabFile MagicTab;
	MagicTab.Load(TABFILE_MAGICAGOLD_PATH);   //�ƽ�װ��ħ�������ļ�
	for (i = 0; i < 6; ++i)
	{
		const int* pSrc;
		KItemNormalAttrib* pDst;
		pSrc = &(pTemp->m_aryMagicAttribs[i]); //���ܱ��
		pDst = &(sMA[i]);
		
		int nType,nLow,nHigh;
		MagicTab.GetInteger(*pSrc + 1,"���Ե������",0,&nType);
		pDst->nAttribType = nType;
		MagicTab.GetInteger(*pSrc + 1,"����1��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����1���ֵ",0,&nHigh);
		//pDst->nValue[0] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[0] =  GetRandomNumber(nLow,nHigh);
		MagicTab.GetInteger(*pSrc + 1,"����2��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����2���ֵ",0,&nHigh);
		//pDst->nValue[1] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[1] =  GetRandomNumber(nLow,nHigh);
		MagicTab.GetInteger(*pSrc + 1,"����3��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����3���ֵ",0,&nHigh);
		//pDst->nValue[2] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[2] =  GetRandomNumber(nLow,nHigh);
	}

	pItem->SetAttrib_MA(sMA);  //��ֵ�� �������� ---m_aryMagicAttrib[i]  ,����ħ������
    pItem->SetRPoint(nRongpiont);
	pItem->EnChance(nEnChance);


	KItemNormalAttrib	syMA[2];
	for (i = 0; i < 2; ++i)
	{//��������
		const int* pSrc;
		KItemNormalAttrib* pDst;
		pSrc = &(pTemp->m_yinMagicAttribs[i]); //���ܱ��
		pDst = &(syMA[i]);
		
		int nType,nLow,nHigh;
		MagicTab.GetInteger(*pSrc + 1,"���Ե������",0,&nType);
		pDst->nAttribType = nType;	//����
		MagicTab.GetInteger(*pSrc + 1,"����1��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����1���ֵ",0,&nHigh);
		//pDst->nValue[0] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[0] =  GetRandomNumber(nLow,nHigh);
		MagicTab.GetInteger(*pSrc + 1,"����2��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����2���ֵ",0,&nHigh);
		//pDst->nValue[1] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[1] =  GetRandomNumber(nLow,nHigh);
		MagicTab.GetInteger(*pSrc + 1,"����3��Сֵ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"����3���ֵ",0,&nHigh);
		//pDst->nValue[2] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		pDst->nValue[2] =  GetRandomNumber(nLow,nHigh);
	}
	pItem->SetAttrib_Yin(syMA); 
	MagicTab.Clear(); 
	return TRUE;
}

/****************************************************************************
	���ܣ�	����ָ�������İ׽�װ��װ�� ����
	���:	nDetailType: �������, �����������������......
			nParticularType: ��ϸ���
			nSeriesReq: ��������
			nLevel: �ȼ�
			pnaryMALevel: ħ�����Եȼ�����[6]
			nLucky: ����ֵ
	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и���
			ʧ��ʱ������
******************************************************************************/
BOOL KItemGenerator::GetPlatinaItemByRong(IN int nDetailType,\
								   IN int nParticularType,\
								   IN int nSeriesReq,\
								   IN int nLevel,\
								   IN const int* pnaryMALevel,\
								   IN const int* rongALevel,\
								   IN int nLucky,\
								   IN int nVersion,\
								   IN OUT KItem* pItem, \
								   IN int nEnChance,\
								   IN int nRongID,\
								   IN int Goidid,\
								   IN const int* BaoMagLevel,IN BOOL nIsLogin)
{
	
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	  if  (this==NULL || pItem==NULL)
	    		return false;

    int nCount = 0;
	BOOL bEC = FALSE;
	
#ifdef _SERVER	// �������汾
	if (nIsLogin)
		g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	else
	   pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed(); //ȡ���������

	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);

	if (rongALevel)
        memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
    else
		ZeroMemory(pItem->m_GeneratorParam.nRGeneratorLevel, sizeof(int) * 6);

/*	if (BaoMagLevel)
        memcpy(pItem->m_GeneratorParam.nBGeneratorLevel, BaoMagLevel, sizeof(int) * 6); //��ʯħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nBGeneratorLevel, sizeof(int) * 6); */

	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = nLucky;
    
#else //�ͻ���
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel,pnaryMALevel, sizeof(int) * 6);  //��Ʒ���Ը��²���
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);

	if (rongALevel)
         memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nRGeneratorLevel, sizeof(int) * 6);

	/*if (BaoMagLevel)
        memcpy(pItem->m_GeneratorParam.nBGeneratorLevel, BaoMagLevel, sizeof(int) * 6); //��ʯħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nBGeneratorLevel, sizeof(int) * 6);*/
#endif
		
	int nGameVersion = pItem->m_GeneratorParam.nVersion;

	nCount = m_BPTLib.GetPlatinaNumber();// װ��TXT���ж���������

	const KBASICPROP_EQUIPMENT_PLATINA* pTemp = m_BPTLib.GetPlatinaRecord(Goidid); //��ȡ�׽�ԭʼ����

	if (Goidid >= nCount-1)
	{
		//_ASSERT(FALSE);
	     return bEC; 
	}
	if (pTemp == NULL)
	{
		//_ASSERT(FALSE);
	     return bEC; 
	}
		// �������˵�ֱ��ԭ��: ֻ��n��װ��, ������ i ��ֵ��[0,n-1]֮��
		// ���3��: nParticularType ����?
		//			nLevel ����?
		//			ԭʼ��tab file������ m_BPTLib.m_BPTEquipment ��
		//			���������������?
	pItem->SetAttrib_BCBR((KBASICPROP_EQUIPMENT_PLATINA*)pTemp);  //���û������� ����������
	pItem->SetSeries(nSeriesReq); 

	pItem->SetGoldId(Goidid);
	pItem->SetLevel(nLevel);

		if (nLucky > 10)
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}
		KItemNormalAttrib	sMA[6];  //ħ��
		ZeroMemory(sMA,sizeof(sMA)); //�����ڴ�
		KTabFile MagicTab;
		MagicTab.Load(TABFILE_MAGICAGOLD_PATH);   //װ��ħ�������ļ�
		int i;
	     	for (i = 0;i< 6;++i)
			{ 
			const int* pSrc;
			KItemNormalAttrib* pDst;
			pSrc = &(pTemp->m_aryMagicAttribs[i]); //ԭʼ�ļ��ܱ��
			pDst = &(sMA[i]);		
			int nType,nLow,nHigh;
			MagicTab.GetInteger(*pSrc + 1,5,0,&nType);
			pDst->nAttribType = nType;
			MagicTab.GetInteger(*pSrc + 1,6,0,&nLow);
			MagicTab.GetInteger(*pSrc + 1,7,0,&nHigh);
			//pDst->nValue[0] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			pDst->nValue[0] =  GetRandomNumber(nLow,nHigh);
			MagicTab.GetInteger(*pSrc + 1,8,0,&nLow);
			MagicTab.GetInteger(*pSrc + 1,9,0,&nHigh);
			//pDst->nValue[1] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			pDst->nValue[1] =  GetRandomNumber(nLow,nHigh);
			MagicTab.GetInteger(*pSrc + 1,10,0,&nLow);
			MagicTab.GetInteger(*pSrc + 1,11,0,&nHigh);
			//pDst->nValue[2] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			pDst->nValue[2] =  GetRandomNumber(nLow,nHigh);
			} 

            pItem->SetAttrib_MA(sMA);
			//pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
            pItem->EnChance(nEnChance); //���¸�ֵ��ǿ��

//==========================����Ϊ�����Ƹ�����===================================
            KItemNormalAttrib	sMAR[6];// ħ��
			ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�

			int nType,nMax,nMin;
	     	for (i = 0;i < 6;++i)
			{ 
			   if (pItem->m_GeneratorParam.nRGeneratorLevel[i] > 0 && i < nRongID)
			   { 
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],5,0,&nType);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],6,0,&nMax);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],7,0,&nMin);
				sMAR[i].nAttribType = nType;
				//sMAR[i].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				sMAR[i].nValue[0] =  GetRandomNumber(nMax,nMin);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],8,0,&nMax);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],9,0,&nMin);
				//sMAR[i].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				sMAR[i].nValue[1] =  GetRandomNumber(nMax,nMin);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],10,0,&nMax);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],11,0,&nMin);
				//sMAR[i].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				sMAR[i].nValue[2] =  GetRandomNumber(nMax,nMin);
				continue;
			   }
			    sMAR[i].nAttribType = 0;
				sMAR[i].nValue[0] = 0;
				sMAR[i].nValue[1] = 0;
		    	sMAR[i].nValue[2] = 0;
              //  bEC=TRUE;
			}  	
		 pItem->SetAttrib_RON(sMAR);   //����ħ������
		 //pItem->SetBackUpRmagicAttrib();
	     pItem->SetRPoint(nRongID); 
		 pItem->EnChanceRong(nEnChance);
		 
//==========================����Ϊ���ױ�ʯ����===================================
/*         KItemNormalAttrib	sbMAR[6];// ħ��
		 ZeroMemory(sbMAR,sizeof(sbMAR)); //�����ڴ�

		 int nbType,nbMax,nbMin;
		 for (int b = 0;b < 6;b++)
		 {
			 if (pItem->m_GeneratorParam.nBGeneratorLevel[b] > 0)
			 { 
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],5,0,&nbType);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],6,0,&nbMax);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],7,0,&nbMin);
				 sbMAR[b].nAttribType = nbType;
				 sbMAR[b].nValue[0] = nbMax + ((nbMin - nbMax) * nLucky / 10);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],8,0,&nbMax);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],9,0,&nbMin);
				 sbMAR[b].nValue[1] = nbMax + ((nbMin - nbMax) * nLucky / 10);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],10,0,&nbMax);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],11,0,&nbMin);
				 sbMAR[b].nValue[2] = nbMax + ((nbMin - nbMax) * nLucky / 10);
				 continue;
			 }
			 sbMAR[b].nAttribType = 0;
			 sbMAR[b].nValue[0] = 0;
			 sbMAR[b].nValue[1] = 0;
			 sbMAR[b].nValue[2] = 0;
		 }  	
		 pItem->SetAttrib_Bao(sbMAR);   //����ħ������
		 pItem->EnChanceBao(nEnChance);	*/
//-----------------��������------------------------------------------------------
		 KItemNormalAttrib	syMA[2];
		 for (i = 0; i < 2; ++i)
		 {//��������
			 const int* pSrc;
			 KItemNormalAttrib* pDst;
			 pSrc = &(pTemp->m_yinMagicAttribs[i]); //���ܱ��
			 pDst = &(syMA[i]);
			 
			 int nType,nLow,nHigh;
			 MagicTab.GetInteger(*pSrc + 1,"���Ե������",0,&nType);
			 pDst->nAttribType = nType;	//����
			 MagicTab.GetInteger(*pSrc + 1,"����1��Сֵ",0,&nLow);
			 MagicTab.GetInteger(*pSrc + 1,"����1���ֵ",0,&nHigh);
			 pDst->nValue[0] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			 MagicTab.GetInteger(*pSrc + 1,"����2��Сֵ",0,&nLow);
			 MagicTab.GetInteger(*pSrc + 1,"����2���ֵ",0,&nHigh);
			 pDst->nValue[1] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			 MagicTab.GetInteger(*pSrc + 1,"����3��Сֵ",0,&nLow);
			 MagicTab.GetInteger(*pSrc + 1,"����3���ֵ",0,&nHigh);
			 pDst->nValue[2] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
		 }
		 pItem->SetAttrib_Yin(syMA); 
//===============================================================================
		 MagicTab.Clear();
     	 return TRUE;
}


/****************************************************************************
	���ܣ�	����������ԵĻƽ�
	���:	nDetailType: �������, �����������������......
			nParticularType: ��ϸ���
			nSeriesReq: ��������
			nLevel: �ȼ�
			pnaryMALevel: ħ�����Եȼ�����[6]
			nLucky: ����ֵ
	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и���
			ʧ��ʱ������
******************************************************************************/
BOOL KItemGenerator::GetGoldItemByRandom(IN int Goidid,\
								   IN int nDetailType,\
								   IN int nParticularType,\
								   IN int nSeriesReq,\
								   IN int nLevel,\
								   IN const int* pnaryMALevel,\
								   IN const int* rongALevel,\
								   IN int nLucky,\
								   IN int nVersion,\
								   IN OUT KItem* pItem, \
								   IN int nEnChance,\
								   IN int nRongID,\
								   IN BOOL nIsLogin,int nPoint)
{
	
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	  if  (this==NULL || pItem==NULL)
		    		return false;
    int nCount = 0;
	BOOL bEC = FALSE;
	
#ifdef _SERVER	// �������汾

	if (nIsLogin)
	   g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);       //�����������,װ�����ԾͲ����
	else
	   pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed(); //ȡ���������

	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);

	if (rongALevel)
        memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
    else
		ZeroMemory(pItem->m_GeneratorParam.nRGeneratorLevel, sizeof(int) * 6);

	/*if (BaoMagLevel)
        memcpy(pItem->m_GeneratorParam.nBGeneratorLevel, BaoMagLevel, sizeof(int) * 6); //��ʯħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nBGeneratorLevel, sizeof(int) * 6);*/

	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = nLucky;
    
#else //�ͻ���
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel,pnaryMALevel, sizeof(int) * 6);  //��Ʒ���Ը��²���
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);

	if (rongALevel)
         memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nRGeneratorLevel, sizeof(int) * 6);

	/*if (BaoMagLevel)
        memcpy(pItem->m_GeneratorParam.nBGeneratorLevel, BaoMagLevel, sizeof(int) * 6); //��ʯħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nBGeneratorLevel, sizeof(int) * 6);*/
#endif
		
	int nGameVersion = pItem->m_GeneratorParam.nVersion;

	nCount = m_BPTLib.GetGoldItemNumber();// װ��TXT���ж���������
	const KBASICPROP_EQUIPMENT_GOLD* pTemp = m_BPTLib.GetGoldItemRecord(Goidid); //��ȡ�ƽ�ԭʼ����

	if (Goidid >= nCount)
	{
		//_ASSERT(FALSE);
	     return bEC; 
	}
	if (pTemp == NULL)
	{
		//_ASSERT(FALSE);
	     return bEC; 
	}
		// �������˵�ֱ��ԭ��: ֻ��n��װ��, ������ i ��ֵ��[0,n-1]֮��
		// ���3��: nParticularType ����?
		//			nLevel ����?
		//			ԭʼ��tab file������ m_BPTLib.m_BPTEquipment ��
		//			���������������?

	if (NULL == pnaryMALevel)
		return TRUE;

	pItem->SetAttrib_RCBR((KBASICPROP_EQUIPMENT_GOLD*)pTemp,TRUE);  //���û������� ����������

	KItemNormalAttrib	sMA[6];	// ���ߵ�ħ������
	bEC = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);

	if (bEC)
		pItem->SetAttrib_MA(sMA);  //����ħ������

    pItem->EnChance(nEnChance);
	
	if (nSeriesReq > 4 || nSeriesReq < 0)
	{
		pItem->SetSeries(::GetRandomNumber(0,4)); //��������
	} 
	else
	{
		pItem->SetSeries(nSeriesReq);
	}

	pItem->SetLevel(nLevel);
	pItem->SetPoint(nPoint);


		if (nLucky > 10)
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}
//==========================����Ϊ�����Ƹ�����===================================
            KItemNormalAttrib	sMAR[6];   //ħ��
			ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
			KTabFile MagicTab;
		    MagicTab.Load(TABFILE_MAGICAGOLD_PATH);   //�ƽ�װ��ħ�������ļ�
			int nType,nMax,nMin,i;
	     	for (i = 0;i < 6;++i)
			{ 
			   if (pItem->m_GeneratorParam.nRGeneratorLevel[i] > 0 && i < nRongID)
			   { 
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],5,0,&nType);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],6,0,&nMax);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],7,0,&nMin);
				sMAR[i].nAttribType = nType;
				//sMAR[i].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				sMAR[i].nValue[0]   =GetRandomNumber(nMax,nMin);

				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],8,0,&nMax);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],9,0,&nMin);
				//sMAR[i].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				sMAR[i].nValue[1]   =GetRandomNumber(nMax,nMin);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],10,0,&nMax);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],11,0,&nMin);
				//sMAR[i].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				sMAR[i].nValue[2]   =GetRandomNumber(nMax,nMin);
				continue;
			   }
			    sMAR[i].nAttribType = 0;
				sMAR[i].nValue[0] = 0;
				sMAR[i].nValue[1] = 0;
		    	sMAR[i].nValue[2] = 0;
              //  bEC=TRUE;
			}  	
		 pItem->SetAttrib_RON(sMAR);   //����ħ������
		 //pItem->SetBackUpRmagicAttrib();
	     pItem->SetRPoint(nRongID); 
		 pItem->EnChanceRong(nEnChance);
//-----------------��������------------------------------------------------------
		 KItemNormalAttrib	syMA[2];
		 for (i = 0; i < 2; ++i)
		 {//��������
			 const int* pSrc;
			 KItemNormalAttrib* pDst;
			 pSrc = &(pTemp->m_yinMagicAttribs[i]); //���ܱ�� �к�
			 pDst = &(syMA[i]);
			 
			 int nType,nLow,nHigh;
			 MagicTab.GetInteger(*pSrc + 1,"���Ե������",0,&nType);
			 pDst->nAttribType = nType;	//����
			 MagicTab.GetInteger(*pSrc + 1,"����1��Сֵ",0,&nLow);
			 MagicTab.GetInteger(*pSrc + 1,"����1���ֵ",0,&nHigh);
			// pDst->nValue[0] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			 pDst->nValue[0]   = GetRandomNumber(nLow,nHigh);
			 MagicTab.GetInteger(*pSrc + 1,"����2��Сֵ",0,&nLow);
			 MagicTab.GetInteger(*pSrc + 1,"����2���ֵ",0,&nHigh);
			// pDst->nValue[1] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			 pDst->nValue[1]   = GetRandomNumber(nLow,nHigh);
			 MagicTab.GetInteger(*pSrc + 1,"����3��Сֵ",0,&nLow);
			 MagicTab.GetInteger(*pSrc + 1,"����3���ֵ",0,&nHigh);
			// pDst->nValue[2] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			 pDst->nValue[2]   = GetRandomNumber(nLow,nHigh);
		 }
		 pItem->SetAttrib_Yin(syMA); 
//===============================================================================
		 MagicTab.Clear();
     	 return TRUE;
}

/******************************************************************************
	���ܣ�	����ָ�������Ļƽ�װ��װ�� ����
	���:	nCondition: ǰ��������(Emf+Pmf)*Level��������
			pItem: װ����Ŀָ�롣
	����:	�ɹ�ʱ������, ��������� pItem ��ָ�����и���
			ʧ��ʱ���ؼ�
****************************************************************************
// Add by flying on May.30.2003
// Try to get a "Gold Item" if possible.
BOOL KItemGenerator::GetGoldItemByRong(IN int nIndex,
									   OUT KItem * pItem, 
									   IN const int* pnay,
									   IN int nSeries, 
									   IN int nEnChance,
									   IN int nRongID)
****************************************************************************
	���ܣ�	����ָ�������Ļƽ�װ��װ�� ����
	���:	nDetailType: �������, �����������������......
			nParticularType: ��ϸ���
			nSeriesReq: ��������
			nLevel: �ȼ�
			pnaryMALevel: ħ�����Եȼ�����[6]
			nLucky: ����ֵ
	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и���
			ʧ��ʱ������
******************************************************************************/
BOOL KItemGenerator::GetGoldItemByRong(IN int nDetailType,\
								   IN int nParticularType,\
								   IN int nSeriesReq,\
								   IN int nLevel,\
								   IN const int* pnaryMALevel,\
								   IN const int* rongALevel,\
								   IN int nLucky,\
								   IN int nVersion,\
								   IN OUT KItem* pItem, \
								   IN int nEnChance,\
								   IN int nRongID,\
								   IN int Goidid,\
								   IN const int* BaoMagLevel,IN BOOL nIsLogin)
{
	
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	  if (this==NULL || pItem==NULL)
		return false;
    int nCount = 0;
	BOOL bEC = FALSE;
	
#ifdef _SERVER	// �������汾

	if (nIsLogin)
	   g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);       //�����������,װ�����ԾͲ����
	else
	   pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed(); //ȡ���������

	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);

	if (rongALevel)
        memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
    else
		ZeroMemory(pItem->m_GeneratorParam.nRGeneratorLevel, sizeof(int) * 6);

	/*if (BaoMagLevel)
        memcpy(pItem->m_GeneratorParam.nBGeneratorLevel, BaoMagLevel, sizeof(int) * 6); //��ʯħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nBGeneratorLevel, sizeof(int) * 6);*/

	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = nLucky;
    
#else //�ͻ���
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel,pnaryMALevel, sizeof(int) * 6);  //��Ʒ���Ը��²���
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);

	if (rongALevel)
         memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nRGeneratorLevel, sizeof(int) * 6);

	/*if (BaoMagLevel)
        memcpy(pItem->m_GeneratorParam.nBGeneratorLevel, BaoMagLevel, sizeof(int) * 6); //��ʯħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nBGeneratorLevel, sizeof(int) * 6);*/
#endif
		
	int nGameVersion = pItem->m_GeneratorParam.nVersion;

	nCount = m_BPTLib.GetGoldItemNumber();// װ��TXT���ж���������
	const KBASICPROP_EQUIPMENT_GOLD* pTemp = m_BPTLib.GetGoldItemRecord(Goidid); //��ȡ�ƽ�ԭʼ����
	if (Goidid >= nCount)
	{
		//_ASSERT(FALSE);
	     return bEC; 
	}
	if (pTemp == NULL)
	{
		//_ASSERT(FALSE);
	     return bEC; 
	}
		// �������˵�ֱ��ԭ��: ֻ��n��װ��, ������ i ��ֵ��[0,n-1]֮��
		// ���3��: nParticularType ����?
		//			nLevel ����?
		//			ԭʼ��tab file������ m_BPTLib.m_BPTEquipment ��
		//			���������������?
	pItem->SetAttrib_RCBR((KBASICPROP_EQUIPMENT_GOLD*)pTemp);  //���û������� ����������
	pItem->SetSeries(nSeriesReq); 
	pItem->SetLevel(nLevel);

		if (nLucky > 10)
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}

		KItemNormalAttrib	sMA[6];// ħ��
		ZeroMemory(sMA,sizeof(sMA)); //�����ڴ�
		KTabFile MagicTab;
		MagicTab.Load(TABFILE_MAGICAGOLD_PATH);   //�ƽ�װ��ħ�������ļ�
		int i;
	     	for (i = 0;i< 6;++i)
			{ 
			const int* pSrc;
			KItemNormalAttrib* pDst;
			pSrc = &(pTemp->m_aryMagicAttribs[i]); //ԭʼ�ļ��ܱ��
			pDst = &(sMA[i]);		
			int nType,nLow,nHigh;
			MagicTab.GetInteger(*pSrc + 1,5,0,&nType);
			pDst->nAttribType = nType;
			MagicTab.GetInteger(*pSrc + 1,6,0,&nLow);
			MagicTab.GetInteger(*pSrc + 1,7,0,&nHigh);
			//pDst->nValue[0] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			pDst->nValue[0]   =GetRandomNumber(nLow,nHigh);
			MagicTab.GetInteger(*pSrc + 1,8,0,&nLow);
			MagicTab.GetInteger(*pSrc + 1,9,0,&nHigh);
			//pDst->nValue[1] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			pDst->nValue[1]   =GetRandomNumber(nLow,nHigh);
			MagicTab.GetInteger(*pSrc + 1,10,0,&nLow);
			MagicTab.GetInteger(*pSrc + 1,11,0,&nHigh);
			//pDst->nValue[2] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			pDst->nValue[2]   =GetRandomNumber(nLow,nHigh);
			} 

            pItem->SetAttrib_MA(sMA);
			//pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
            pItem->EnChance(nEnChance); //���¸�ֵ��ǿ��

//==========================����Ϊ�����Ƹ�����===================================
            KItemNormalAttrib	sMAR[6];   //ħ��
			ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�

			int nType,nMax,nMin;
	     	for (i = 0;i < 6;++i)
			{ 
			   if (pItem->m_GeneratorParam.nRGeneratorLevel[i] > 0 && i < nRongID)
			   { 
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],5,0,&nType);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],6,0,&nMax);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],7,0,&nMin);
				sMAR[i].nAttribType = nType;
				//sMAR[i].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				sMAR[i].nValue[0]   =GetRandomNumber(nMax,nMin);

				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],8,0,&nMax);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],9,0,&nMin);
				//sMAR[i].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				sMAR[i].nValue[1]   =GetRandomNumber(nMax,nMin);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],10,0,&nMax);
				MagicTab.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[i],11,0,&nMin);
				//sMAR[i].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				sMAR[i].nValue[2]   =GetRandomNumber(nMax,nMin);
				continue;
			   }
			    sMAR[i].nAttribType = 0;
				sMAR[i].nValue[0] = 0;
				sMAR[i].nValue[1] = 0;
		    	sMAR[i].nValue[2] = 0;
              //  bEC=TRUE;
			}  	
		 pItem->SetAttrib_RON(sMAR);   //����ħ������
		 //pItem->SetBackUpRmagicAttrib();
	     pItem->SetRPoint(nRongID); 
		 pItem->EnChanceRong(nEnChance);
		 
//==========================����Ϊ���ױ�ʯ����===================================
/*         KItemNormalAttrib	sbMAR[6];// ħ��
		 ZeroMemory(sbMAR,sizeof(sbMAR)); //�����ڴ�

		 int nbType,nbMax,nbMin;
		 for (int b = 0;b < 6;b++)
		 {
			 if (pItem->m_GeneratorParam.nBGeneratorLevel[b] > 0)
			 { 
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],5,0,&nbType);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],6,0,&nbMax);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],7,0,&nbMin);
				 sbMAR[b].nAttribType = nbType;
				 sbMAR[b].nValue[0] = nbMax + ((nbMin - nbMax) * nLucky / 10);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],8,0,&nbMax);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],9,0,&nbMin);
				 sbMAR[b].nValue[1] = nbMax + ((nbMin - nbMax) * nLucky / 10);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],10,0,&nbMax);
				 MagicTab.GetInteger(pItem->m_GeneratorParam.nBGeneratorLevel[b],11,0,&nbMin);
				 sbMAR[b].nValue[2] = nbMax + ((nbMin - nbMax) * nLucky / 10);
				 continue;
			 }
			 sbMAR[b].nAttribType = 0;
			 sbMAR[b].nValue[0] = 0;
			 sbMAR[b].nValue[1] = 0;
			 sbMAR[b].nValue[2] = 0;
		 }  	
		 pItem->SetAttrib_Bao(sbMAR);   //����ħ������
		 pItem->EnChanceBao(nEnChance);	 */
//-----------------��������------------------------------------------------------
		 KItemNormalAttrib	syMA[2];
		 for (i = 0; i < 2; ++i)
		 {//��������
			 const int* pSrc;
			 KItemNormalAttrib* pDst;
			 pSrc = &(pTemp->m_yinMagicAttribs[i]); //���ܱ��
			 pDst = &(syMA[i]);
			 
			 int nType,nLow,nHigh;
			 MagicTab.GetInteger(*pSrc + 1,"���Ե������",0,&nType);
			 pDst->nAttribType = nType;	//����
			 MagicTab.GetInteger(*pSrc + 1,"����1��Сֵ",0,&nLow);
			 MagicTab.GetInteger(*pSrc + 1,"����1���ֵ",0,&nHigh);
			 //pDst->nValue[0] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
             pDst->nValue[0]   = GetRandomNumber(nLow,nHigh);

			 MagicTab.GetInteger(*pSrc + 1,"����2��Сֵ",0,&nLow);
			 MagicTab.GetInteger(*pSrc + 1,"����2���ֵ",0,&nHigh);
			 //pDst->nValue[1] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			 pDst->nValue[1]   = GetRandomNumber(nLow,nHigh);
			 MagicTab.GetInteger(*pSrc + 1,"����3��Сֵ",0,&nLow);
			 MagicTab.GetInteger(*pSrc + 1,"����3���ֵ",0,&nHigh);
			 //pDst->nValue[2] =  nLow + ((nHigh - nLow) *  pItem->m_GeneratorParam.nGeneratorLevel[i] / 10);
			 pDst->nValue[2]   = GetRandomNumber(nLow,nHigh);
		 }
		 pItem->SetAttrib_Yin(syMA); 
//===============================================================================
		 MagicTab.Clear();
     	 return TRUE;
}

/*
	pItem->SetAttrib_MA(sMA);   //����ԭ��ħ������
	pItem->SetAttrib_RON(sMAR);  //��������ħ������
    pItem->SetRPoint(nRongID);  //����Ϊ�Ѿ�����
	MagicTab.Clear();
	pItem->EnChanceRong(nEnChance);
*/

/******************************************************************************
	���ܣ�	�������װ��  ---����������װ
	���:	nDetailType: �������, �����������������......
			nParticularType: ��ϸ���
			nSeriesReq: ��������
			nLevel: �ȼ�
			pnaryMALevel: ħ�����Եȼ�����[6]
			nLucky: ����ֵ
	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и���
			ʧ��ʱ������
******************************************************************************/
BOOL KItemGenerator::Gen_EquipmentRong(IN int nDetailType,\
								   IN int nParticularType,\
								   IN int nSeriesReq,\
								   IN int nLevel,\
								   IN const int* pnaryMALevel,\
								   IN const int* rongALevel,\
								   IN int nLucky,\
								   IN int nVersion,\
								   IN OUT KItem* pItem, \
								   IN int nEnChance,\
								   IN int nPoint,\
								   IN int nRongID,\
								   IN int Goidid,\
								   IN int nIsDaZhao,\
								   KMagicAttrib *nRogMag,IN int IsWhere,IN BOOL nIsLogin)
{
	
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if (this==NULL || pItem==NULL)
			return false;
    int nCount = 0;
	BOOL bEC = FALSE;
/*
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
	if (rongALevel)
        memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
	int nGameVersion = pItem->m_GeneratorParam.nVersion;
*/
	
#ifdef _SERVER	// �������汾
	if  (nIsLogin)
		g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	else
	    pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed(); //ȡ���������

	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6); //ħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);

	if (rongALevel)
        memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩	
    else
		ZeroMemory(pItem->m_GeneratorParam.nRGeneratorLevel, sizeof(int) * 6);

	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck    = nLucky;
    
#else //�ͻ���
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);  //�����������
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel,pnaryMALevel, sizeof(int) * 6);  //��Ʒ���Ը��²���	
	if (rongALevel)
		memcpy(pItem->m_GeneratorParam.nRGeneratorLevel,rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
	//pItem->m_GeneratorParam.nLuck = nLucky;
#endif
    
	int nGameVersion =pItem->m_GeneratorParam.nVersion;

	// ������ڲ���, ȷ��װ���Ļ�������
	int i = nParticularType * 10 + nLevel - 1;      // ����ʽ�ɲ߻�����
												    // ���SPE 2002��8��31��7:40
												    // email: ����������ֵ��
												    // �ȼ����ݴ�1��ʼ������Ҫ������
	if (nDetailType==equip_mask)
	{
		i = nParticularType;
	}

	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)  //���Դ����װ������
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);//��ȡԭʼװ������    iΪ ����
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:	// ���
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_pifeng:	// ����
		pEqu = m_BPTLib.GetPifengRecord(i);
		break;
	case equip_yinjian:	// ӡ��
		pEqu = m_BPTLib.GetYinjianRecord(i);
		break;
	case equip_shiping:	// ��Ʒ
		pEqu = m_BPTLib.GetShipinRecord(i);
		break;
	default:
		break;
	}
   //����ʧ��
	if (NULL == pEqu)
	{ 
	 // _ASSERT(FALSE);
	  return bEC; 
	}  
		// �������˵�ֱ��ԭ��: ֻ��n��װ��, ������ i ��ֵ��[0,n-1]֮��
		// ���3��: nParticularType ����?
		//			nLevel ����?
		//			ԭʼ��tab file������ m_BPTLib.m_BPTEquipment ��
		//			���������������?
	pItem->SetAttrib_CBR(pEqu);  //���û������� ����������

	pItem->SetSeries(nSeriesReq); 

	// ȷ��װ����ħ������ һ��װ��
	if (nPoint == 0)  //��װ  ��װ
	{
       int bECA=0;  
		if (NULL == pnaryMALevel)  //��װ ����Ҫ����ħ������
		return TRUE;
		KItemNormalAttrib sMA[6];  //���ߵ�ħ������
		
		bECA = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
		if (bECA)
			pItem->SetAttrib_MA(sMA);  //  ���� ħ������
	        //pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
		    pItem->EnChance(nEnChance);

		if (nLucky > 10)  //����ֵ
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}

       	///����Ϊ��ʶ����
		KItemNormalAttrib	sMAR[6];// ħ��
		ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
		KTabFile MagicTabR;
		MagicTabR.Load(TABFILE_MAGICAGOLD_PATH);  //������װ��ħ�������ļ�

		if  (IsWhere>0)
			  nRongID = IsWhere;

		int nTypeR,nMaxR,nMinR;
		for (int r = 0;r < 6;r++)
		{ 
			if (pItem->m_GeneratorParam.nRGeneratorLevel[r] > 0 && r < nRongID)
			{ 
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],5,0,&nTypeR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],6,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],7,0,&nMinR);
				sMAR[r].nAttribType = nTypeR;  //��������
				//sMAR[r].nValue[0] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[0] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],8,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],9,0,&nMinR);
				//sMAR[r].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[1] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],10,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],11,0,&nMinR);
				//sMAR[r].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[2] = GetRandomNumber(nMaxR,nMinR);
				continue;
			}
			sMAR[r].nAttribType = 0;
			sMAR[r].nValue[0] = 0;
			sMAR[r].nValue[1] = 0;
			sMAR[r].nValue[2] = 0;
			//  bEC=TRUE;
		} 
		
		MagicTabR.Clear();

		pItem->SetAttrib_RON(sMAR);   //��������ħ������
		pItem->SetRPoint(nRongID);
		pItem->SetIsWhere(IsWhere);   //�Ƿ��ʶ��
//		pItem->EnChanceRong(nEnChance);
     	return bECA;

	}
	else if (nPoint ==7) //ָ����װ
	{
		KItemNormalAttrib sMA[6];
		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);
		if (nLucky > 10)  //����ֵ
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}
		pItem->m_GeneratorParam.nLuck = nLucky;  //���� ������ֵ

		int nType,nMax,nMin;
		for (int j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 0 )
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;
				//sMA[j].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[0] = GetRandomNumber(nMax,nMin);

				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);
				//sMA[j].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[1] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);
				//sMA[j].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[2] = GetRandomNumber(nMax,nMin);

				if (sMA[j].nValue[2]>1)
				{ 
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;
						    nRows = sMA[j].nValue[2]+IsWhere;//nMax
						if (nRows>nMin)
                            nRows=nMin;

						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMA[j].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
						
						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMA[j].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
                    nJnLevel.Clear();
				}

				continue;
			}
			
			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}
		bEC=pItem->SetAttrib_MA(sMA);
		pItem->SetPoint(nPoint);
		pItem->SetIsWhere(IsWhere);
		tabMagicAttribe.Clear();
		pItem->EnChance(nEnChance);
		return bEC;
	}

	else if (nPoint ==8) //ָ����װ
	{
		/*int bECA=0;  
		if (NULL == pnaryMALevel)  //��װ ����Ҫ����ħ������
			return FALSE;
		KItemNormalAttrib sMA[6];  //���ߵ�ħ������

		bECA = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
		if (bECA)
			pItem->SetAttrib_MA(sMA);  //  ���� ħ������
		*/


		KItemNormalAttrib sMA[6];
		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);
	
		pItem->m_GeneratorParam.nLuck = nLucky;  //���� ������ֵ

		int nType,nMax,nMin;
		for (int j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 10 )
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;
				//sMA[j].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[0] = GetRandomNumber(nMax,nMin);


				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);
				//sMA[j].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[1] = GetRandomNumber(nMax,nMin);

				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);
				//sMA[j].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[2] = GetRandomNumber(nMax,nMin);



				if (sMA[j].nValue[2]>1)
				{ 
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;
						    nRows = sMA[j].nValue[2]+IsWhere;
						if (nRows>nMin)
                            nRows=nMin;

						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMA[j].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
						
						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMA[j].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
                    nJnLevel.Clear();
				}

				continue;
			}
			
			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}
		bEC=pItem->SetAttrib_MA(sMA);
		pItem->SetPoint(nPoint);
		pItem->SetIsWhere(IsWhere);
		tabMagicAttribe.Clear();
		pItem->EnChance(nEnChance);
		return bEC;
	}
	else  //��װ  �� ��װ
	{
		KItemNormalAttrib sMA[6];
		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);
		if (nLucky > 10)  //����ֵ
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}

		pItem->m_GeneratorParam.nLuck = nLucky;  //���� ������ֵ
		int nType,nMax,nMin;
		for (int j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 0 && j < nPoint)
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;
				//sMA[j].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[0] = GetRandomNumber(nMax,nMin);

				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);
				//sMA[j].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[1] = GetRandomNumber(nMax,nMin);

				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);
				//sMA[j].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[2] = GetRandomNumber(nMax,nMin);

				if (sMA[j].nValue[2]>1)
				{ 
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;
						nRows = sMA[j].nValue[2]+IsWhere;
						if (nRows>nMin)
                            nRows=nMin;
						
						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMA[j].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
						
						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMA[j].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
                    nJnLevel.Clear();
				}

				continue;
			}

			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}
		bEC=pItem->SetAttrib_MA(sMA);
		pItem->SetPoint(nPoint);		
		tabMagicAttribe.Clear();
		//pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
		pItem->EnChance(nEnChance);
     	return bEC;
	}
}
/******************************************************************************
	���ܣ�	�������װ��  ---���ڿͻ��˲���װ��
	���:	nDetailType: �������, �����������������......
			nParticularType: ��ϸ���
			nSeriesReq: ��������
			nLevel: �ȼ�
			pnaryMALevel: ħ�����Եȼ�����[6]
			nLucky: ����ֵ
	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и���
			ʧ��ʱ������
******************************************************************************/
BOOL KItemGenerator::Gen_EquipmentCilent(IN int nDetailType,\
								   IN int nParticularType,\
								   IN int nSeriesReq,\
								   IN int nLevel,\
								   IN const int* pnaryMALevel,\
								   IN const int* rongALevel,\
								   IN int nLucky,\
								   IN int nVersion,\
								   IN OUT KItem* pItem, \
								   IN int nEnChance,\
								   IN int nPoint,\
								   IN int nRongID,
								   IN int Goidid,\
								   IN const int* pnaryMAType,\
								   IN const int* pnaryMAVala,\
								   IN const int* pnaryMAValb,\
								   IN const int* pnaryMAValc,IN int IsWhere)
{
	
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if (this==NULL || pItem==NULL)
			return false;
    int nCount = 0;
	BOOL bEC = FALSE;
#ifdef _SERVER	// �������汾
	pItem->m_GeneratorParam.uRandomSeed = g_GetRandomSeed(); //ȡ���������
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6); //ħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);
	
	if (rongALevel)
        memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩	
	else
		ZeroMemory(pItem->m_GeneratorParam.nRGeneratorLevel, sizeof(int) * 6);

	pItem->m_GeneratorParam.nVersion = g_SubWorldSet.GetGameVersion();
	pItem->m_GeneratorParam.nLuck = nLucky;
    
#else //�ͻ���

	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
	if (rongALevel)
        memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, rongALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
#endif
    
	int nGameVersion =pItem->m_GeneratorParam.nVersion;	
	// ������ڲ���, ȷ��װ���Ļ�������
	int i = nParticularType * 10 + nLevel - 1;// ����ʽ�ɲ߻�����
												// ���SPE 2002��8��31��7:40
												// email: ����������ֵ��
												// �ȼ����ݴ�1��ʼ������Ҫ������
	if (nDetailType==equip_mask)
	{
		i = nParticularType;
	}

	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)  //���Դ����װ������
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);//��ȡԭʼװ������    iΪ ����
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:	// ���
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_pifeng:	// ����
		pEqu = m_BPTLib.GetPifengRecord(i);
		break;
	case equip_yinjian:	// ����
		pEqu = m_BPTLib.GetYinjianRecord(i);
		break;
	case equip_shiping:	// ����
		pEqu = m_BPTLib.GetShipinRecord(i);
		break;
	default:
		break;
	}
   //����ʧ��
	if (NULL == pEqu)
	{ 
	//_ASSERT(FALSE);
	return bEC; 
	}  
		// �������˵�ֱ��ԭ��: ֻ��n��װ��, ������ i ��ֵ��[0,n-1]֮��
		// ���3��: nParticularType ����?
		//			nLevel ����?
		//			ԭʼ��tab file������ m_BPTLib.m_BPTEquipment ��
		//			���������������?
	pItem->SetAttrib_CBR(pEqu);  //���û������� ����������
	pItem->SetSeries(nSeriesReq); 
//	pItem->SetRPoint(nRongID); 
	// ȷ��װ����ħ������ һ��װ��
	if (nPoint == 0)  //��װ  ��װ
	{
		if (NULL == pnaryMALevel)   //��װ ����Ҫ����ħ������
		return TRUE;
		KItemNormalAttrib sMA[6];	// ���ߵ�ħ������
		int bECA=0;
		bECA = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
		if (bECA)
			pItem->SetAttrib_MA(sMA);  //���� ħ������
            //pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
		    pItem->EnChance(nEnChance);//���üӳ�

		if (nLucky > 10)  //����ֵ
		{
			nLucky = 10;
		}

		else if (nLucky < 0)
		{
			nLucky = 0;
		}

       	///����Ϊ�����Ƹ�����
		KItemNormalAttrib	sMAR[6];// ħ��
		ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
		KTabFile MagicTabR;
		MagicTabR.Load(TABFILE_MAGICAGOLD_PATH);  //������װ��ħ�������ļ�

		if (IsWhere>0)   //��װ�ı�ʶ���� ������� ��װ����������
            nRongID = IsWhere;

		int nTypeR,nMaxR,nMinR;
		for (int r = 0;r < 6;r++)
		{ 
			if (pItem->m_GeneratorParam.nRGeneratorLevel[r] > 0 && r < nRongID)
			{ 
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],5,0,&nTypeR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],6,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],7,0,&nMinR);
				sMAR[r].nAttribType = nTypeR;  //��������
				//sMAR[r].nValue[0] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[0] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],8,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],9,0,&nMinR);
				//sMAR[r].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[1] = GetRandomNumber(nMaxR,nMinR);

				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],10,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],11,0,&nMinR);
				//sMAR[r].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[2] = GetRandomNumber(nMaxR,nMinR);
				continue;
			}
			sMAR[r].nAttribType = 0;
			sMAR[r].nValue[0] = 0;
			sMAR[r].nValue[1] = 0;
			sMAR[r].nValue[2] = 0;
			//  bEC=TRUE;
		} 
		
		MagicTabR.Clear();

		pItem->SetAttrib_RON(sMAR);   //��������ħ������
        pItem->SetIsWhere(IsWhere); 
		pItem->SetRPoint(nRongID); 		
//		pItem->EnChanceRong(nEnChance);
     	return bECA;

	}
	else if (nPoint ==7) //ָ����װ
	{
		KItemNormalAttrib sMA[6];
		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);

		if (nLucky > 10)  //����ֵ
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}

		pItem->m_GeneratorParam.nLuck = nLucky;  //���� ������ֵ

		int nType,nMax,nMin;
		for (int j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 0 )
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;
				//sMA[j].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[0] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);
				//sMA[j].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[1] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);
				//sMA[j].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[2] = GetRandomNumber(nMax,nMin);

				if (sMA[j].nValue[2]>1)
				{
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;
						   nRows = sMA[j].nValue[2]+IsWhere;
						if (nRows>nMin)
                           nRows=nMin;

						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMA[j].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
						
						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMA[j].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
                    nJnLevel.Clear();
				}

				continue;
			}
			
			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}
		bEC=pItem->SetAttrib_MA(sMA);
		pItem->SetPoint(nPoint);
		pItem->SetIsWhere(IsWhere);
		tabMagicAttribe.Clear();
		pItem->EnChance(nEnChance);
		return bEC;
	}
	else if (nPoint ==8) //ָ����װ

	{
		int bECR=0;

	  ///����Ϊ�����Ƹ�����
	  KItemNormalAttrib	sMAR[6];// ħ��
	  ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
	  KTabFile MagicTabR;
	  MagicTabR.Load(TABFILE_MAGICATTRIB_PATH);  //������װ��ħ�������ļ�
	  
	  int nTypeR,nMaxR,nMinR;
	  for (int r = 0;r < 6;r++)
	  { 
		  if (pItem->m_GeneratorParam.nRGeneratorLevel[r] > 0 && r < nRongID)
		  { 
			  MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],5,0,&nTypeR);
			  MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],6,0,&nMaxR);
			  MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],7,0,&nMinR);
			  sMAR[r].nAttribType = nTypeR;  //��������
			  //sMAR[r].nValue[0] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
			  sMAR[r].nValue[0] = GetRandomNumber(nMaxR,nMinR);
			  MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],8,0,&nMaxR);
			  MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],9,0,&nMinR);
			  //sMAR[r].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
			  sMAR[r].nValue[1] = GetRandomNumber(nMaxR,nMinR);
			  MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],10,0,&nMaxR);
			  MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],11,0,&nMinR);
			  //sMAR[r].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
			  sMAR[r].nValue[2] = GetRandomNumber(nMaxR,nMinR);

			  if (sMAR[r].nValue[2]>1)
			  {
				  KTabFile nJnLevel;
				  if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
				  {  
					  int nMinVal=0,nMaxVal=0,nRows;
					  nRows = sMAR[r].nValue[2]+IsWhere;
					  if (nRows>nMinR)
						  nRows=nMinR;
					  
					  nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
					  nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
					  sMAR[r].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
					  
					  nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
					  nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
					  sMAR[r].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
				  }
				  nJnLevel.Clear();
			  } 
			  continue;
		  }
		  sMAR[r].nAttribType = 0;
		  sMAR[r].nValue[0]   = 0;
		  sMAR[r].nValue[1]   = 0;
		  sMAR[r].nValue[2]   = 0;
		  //  bEC=TRUE;
	  } 
	  
	  MagicTabR.Clear();
	  
	  bECR= pItem->SetAttrib_RON(sMAR);   //��������ħ������
	  pItem->SetRPoint(nRongID); 		
	  //		pItem->EnChanceRong(nEnChance);
     return bECR;
	}
	else  //��װ  �� ��װ
	{
		KItemNormalAttrib sMA[6];
		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);
		if (nLucky > 10)  //����ֵ
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}
		pItem->m_GeneratorParam.nLuck = nLucky;  //���� ������ֵ
		int nType,nMax,nMin,j;
		for (j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 0 && j < nPoint)
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;
				//sMA[j].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[0] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);
				//sMA[j].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[1] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);
				//sMA[j].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[2] = GetRandomNumber(nMax,nMin);


				if (sMA[j].nValue[2]>1)
				{
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;
						nRows = sMA[j].nValue[2]+IsWhere;
						if (nRows>nMin)
							nRows=nMin;
						
						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMA[j].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
						
						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMA[j].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
					nJnLevel.Clear();
				} 
				continue;
			}

			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}
		bEC=pItem->SetAttrib_MA(sMA);
		pItem->SetPoint(nPoint);		
		tabMagicAttribe.Clear();
		pItem->EnChance(nEnChance);
     	//return bEC;
		//----------------------�Ƹֵ�����
       	///����Ϊ�����Ƹ�����
		KItemNormalAttrib	sMAR[6];  // ħ��
		ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
		KTabFile MagicTabR;
		MagicTabR.Load(TABFILE_MAGICAGOLD_PATH);  //������װ��ħ�������ļ�		
		int nTypeR,nMaxR,nMinR;

		for (j = 0;j < 6;j++)
		{ 
			if (pItem->m_GeneratorParam.nRGeneratorLevel[j] > 0 && j < nRongID)
			{ 
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],5,0,&nTypeR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],6,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],7,0,&nMinR);
				sMAR[j].nAttribType = nTypeR;  //��������
				sMAR[j].nValue[0] = GetRandomNumber(nMaxR,nMinR);//nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],8,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],9,0,&nMinR);
				sMAR[j].nValue[1] = GetRandomNumber(nMaxR,nMinR);
				//sMAR[j].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],10,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],11,0,&nMinR);
				//sMAR[j].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[j].nValue[2] = GetRandomNumber(nMaxR,nMinR);
				continue;
			}
			sMAR[j].nAttribType = 0;
			sMAR[j].nValue[0] = 0;
			sMAR[j].nValue[1] = 0;
			sMAR[j].nValue[2] = 0;
		} 
		
		MagicTabR.Clear();
		bEC=pItem->SetAttrib_RON(sMAR);   //��������ħ������
		pItem->SetRPoint(nRongID); 

	   return bEC;
	}
}


BOOL KItemGenerator::GetEquipmentCommonAttrib(IN int nDetailType, IN int nParticularType, IN int nLevel, IN int nSeries, IN OUT KItem* pItem)
{
	if (NULL == pItem)
	{
//		KASSERT(FALSE);
		return FALSE;
	}

	const int i = nParticularType * 10 + nLevel - 1;// ����ʽ�ɲ߻�����
												// ���SPE 2002��8��31��7:40
												// email: ����������ֵ��
	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:	// mat na
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_pifeng:	//����
		pEqu = m_BPTLib.GetPifengRecord(i);
		break;	
	case equip_yinjian:	// mat na
		pEqu = m_BPTLib.GetYinjianRecord(i);
		break;
	case equip_shiping:	// mat na
		pEqu = m_BPTLib.GetShipinRecord(i);
		break;
	default:
		break;
	}
	if (NULL == pEqu)
		{// _ASSERT(FALSE);
		return FALSE; }
		// �������˵�ֱ��ԭ��: ֻ��n��װ��, ������ i ��ֵ��[0,n-1]֮��
		// ���3��: nParticularType ����?
		//			nLevel ����?
		//			ԭʼ��tab file������ m_BPTLib.m_BPTEquipment ��
		//			���������������?
	pItem->SetAttrib_CBR(pEqu);
	pItem->SetSeries(nSeries);
	return TRUE;
}

/******************************************************************************
	���ܣ�	�������װ��    ��¼ʱ���� ͬ��װ��  ��װ ��װ 
	����:	�ɹ�ʱ���ط���, ��������� pItem ��ָ�����и���
			ʧ��ʱ������
******************************************************************************/

BOOL KItemGenerator::Gen_ExistEquipment(IN int nDetailType,\
								   IN int nParticularType,\
								   IN int nSeriesReq,\
								   IN int nLevel,\
								   IN const int* pnaryMALevel,\
								   IN int nLucky,\
								   IN int nVersion,\
								   IN OUT KItem* pItem,\
								   IN int nEnChance,\
								   IN int nPoint,\
								   IN const int* pnaryRMALevel,\
								   IN int nRongpiont,IN int nIsWhere)
{
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if (this==NULL || pItem==NULL)
			return false;

	BOOL bEC = FALSE;

	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);//�����������,װ�����ԾͲ����

	if (pnaryMALevel)
		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnaryMALevel, sizeof(int) * 6);
    else
		ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(int) * 6);

	if (pnaryRMALevel)
        memcpy(pItem->m_GeneratorParam.nRGeneratorLevel, pnaryRMALevel, sizeof(int) * 6); //����ħ���ȼ���itemlevel���飩
	else
		ZeroMemory(pItem->m_GeneratorParam.nRGeneratorLevel, sizeof(int) * 6);

	int nGameVersion = pItem->m_GeneratorParam.nVersion;

	
	// ������ڲ���, ȷ��װ���Ļ�������
	 int i = nParticularType * 10 + nLevel - 1;// ����ʽ�ɲ߻�����
												// ���SPE 2002��8��31��7:40
												// email: ����������ֵ��
												// �ȼ����ݴ�1��ʼ������Ҫ������
	 if (nDetailType==equip_mask)
	 {
		 i = nParticularType;
	 }

	const KBASICPROP_EQUIPMENT* pEqu = NULL;
	switch(nDetailType)
	{
	case equip_meleeweapon:
		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
		break;
	case equip_rangeweapon:
		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
		break;
	case equip_armor:
		pEqu = m_BPTLib.GetArmorRecord(i);
		break;
	case equip_helm:
		pEqu = m_BPTLib.GetHelmRecord(i);
		break;
	case equip_boots:
		pEqu = m_BPTLib.GetBootRecord(i);
		break;
	case equip_belt:
		pEqu = m_BPTLib.GetBeltRecord(i);
		break;
	case equip_amulet:
		pEqu = m_BPTLib.GetAmuletRecord(i);
		break;
	case equip_ring:
		pEqu = m_BPTLib.GetRingRecord(i);
		break;
	case equip_cuff:
		pEqu = m_BPTLib.GetCuffRecord(i);
		break;
	case equip_pendant:
		pEqu = m_BPTLib.GetPendantRecord(i);
		break;
	case equip_horse:
		pEqu = m_BPTLib.GetHorseRecord(i);
		break;
	case equip_mask:	// mat na
		pEqu = m_BPTLib.GetMaskRecord(i);
		break;
	case equip_pifeng:	// ����
		pEqu = m_BPTLib.GetPifengRecord(i);
		break;
	case equip_yinjian:	// mat na
		pEqu = m_BPTLib.GetYinjianRecord(i);
		break;
	case equip_shiping:	// mat na
		pEqu = m_BPTLib.GetShipinRecord(i);
		break;
	default:
		break;
	}
   //����ʧ��
	if (NULL == pEqu)
	{ 
	//_ASSERT(FALSE);
	return bEC; 
	}  
		// �������˵�ֱ��ԭ��: ֻ��n��װ��, ������ i ��ֵ��[0,n-1]֮��
		// ���3��: nParticularType ����?
		//			nLevel ����?
		//			ԭʼ��tab file������ m_BPTLib.m_BPTEquipment ��
		//			���������������?
	pItem->SetAttrib_CBR(pEqu);  //���û������� ����������
	pItem->SetSeries(nSeriesReq); 

	// ȷ��װ����ħ������ һ��װ��

	if (nPoint == 0)  //��װ  ��װ
	{
		if (NULL == pnaryMALevel)  //��װ ����Ҫ����ħ������
		return TRUE;
		KItemNormalAttrib	sMA[6];	// ���ߵ�ħ������
		int bECA=0;
		    bECA = Gen_MagicAttrib(nDetailType, pnaryMALevel, nSeriesReq, nLucky, sMA, nGameVersion);
		if (bECA)
			pItem->SetAttrib_MA(sMA);    //���� ħ������
           // pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
		    pItem->EnChance(nEnChance);  //ǿ������

		if (nLucky > 10)                 //����ֵ
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}

       	///����Ϊ��������
		KItemNormalAttrib	sMAR[6];// ħ��
		ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
		KTabFile MagicTabR;
		MagicTabR.Load(TABFILE_MAGICAGOLD_PATH);  //������װ��ħ�������ļ�	

		if (nIsWhere>0)
            nRongpiont = nIsWhere;

		int nTypeR,nMaxR,nMinR;
		for (int r = 0;r < 6;r++)
		{ 
			if (pItem->m_GeneratorParam.nRGeneratorLevel[r] > 0 && r < nRongpiont)
			{ 
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],5,0,&nTypeR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],6,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],7,0,&nMinR);
				sMAR[r].nAttribType = nTypeR;  //��������
				//sMAR[r].nValue[0] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[0] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],8,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],9,0,&nMinR);
				//sMAR[r].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[1] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],10,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],11,0,&nMinR);
				//sMAR[r].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[2] = GetRandomNumber(nMaxR,nMinR);
				continue;
			}
			sMAR[r].nAttribType = 0;
			sMAR[r].nValue[0] = 0;
			sMAR[r].nValue[1] = 0;
			sMAR[r].nValue[2] = 0;
			//  bEC=TRUE;
		} 
		
		MagicTabR.Clear();
		pItem->SetAttrib_RON(sMAR);   //��������ħ������
		pItem->SetIsWhere(nIsWhere);
		pItem->SetRPoint(nRongpiont); 		
//		pItem->EnChanceRong(nEnChance);
     	return bECA;

	}
	else  if  (nPoint ==7) //ָ����װ
	{
		KItemNormalAttrib sMA[6];
		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);
		if (nLucky > 10)  //����ֵ
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}
		pItem->m_GeneratorParam.nLuck = nLucky;  //���� ������ֵ
		int nType,nMax,nMin;
		for (int j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 0)
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;
				//sMA[j].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[0] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);
				//sMA[j].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[1] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);
				//sMA[j].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[2] = GetRandomNumber(nMax,nMin);

				if (sMA[j].nValue[2]>1)
				{
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;
						nRows = sMA[j].nValue[2]+nIsWhere;
						if (nRows>nMin)
                            nRows=nMin;

						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMA[j].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
						
						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMA[j].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
                    nJnLevel.Clear();
				}

				continue;
			}
			
			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}
		bEC=pItem->SetAttrib_MA(sMA);
		pItem->SetPoint(nPoint);
		pItem->SetIsWhere(nIsWhere);
		tabMagicAttribe.Clear();
		//pItem->SetBackUpMagicAttrib();//����ԭʼħ������(����ǿ��)
		pItem->EnChance(nEnChance);
		return bEC;
	}
	else  if  (nPoint ==8) //ָ����װ
	{
		int bECR=0;
       	///����Ϊ�����Ƹ�����
		KItemNormalAttrib	sMAR[6];// ħ��
		ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
		KTabFile MagicTabR;
		MagicTabR.Load(TABFILE_MAGICATTRIB_PATH);  //������װ��ħ�������ļ�		
		int nTypeR,nMaxR,nMinR;
		for (int r = 0;r < 6;r++)
		{ 
			if (pItem->m_GeneratorParam.nRGeneratorLevel[r] > 0 && r < nRongpiont)
			{ 
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],5,0,&nTypeR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],6,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],7,0,&nMinR);
				sMAR[r].nAttribType = nTypeR;  //��������
				//sMAR[r].nValue[0] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[0] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],8,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],9,0,&nMinR);
				//sMAR[r].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[1] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],10,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[r],11,0,&nMinR);
				//sMAR[r].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[r].nValue[2] = GetRandomNumber(nMaxR,nMinR);
				continue;
			}
			sMAR[r].nAttribType = 0;
			sMAR[r].nValue[0] = 0;
			sMAR[r].nValue[1] = 0;
			sMAR[r].nValue[2] = 0;
			//  bEC=TRUE;
		} 
		
		MagicTabR.Clear();
		bECR=pItem->SetAttrib_RON(sMAR);   //��������ħ������
		pItem->SetRPoint(nRongpiont); 		
		//		pItem->EnChanceRong(nEnChance);
		return bECR;
	}
	else  //��װ
	{
		int j;
		KItemNormalAttrib sMA[6];
		KTabFile tabMagicAttribe;
		tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);
		if (nLucky > 10)  //����ֵ
		{
			nLucky = 10;
		}
		else if (nLucky < 0)
		{
			nLucky = 0;
		}
		pItem->m_GeneratorParam.nLuck = nLucky;  //���� ������ֵ
		int nType,nMax,nMin;
		for (j = 0;j < 6;++j)
		{
			if (pItem->m_GeneratorParam.nGeneratorLevel[j] > 0 && j < nPoint)
			{
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],5,0,&nType);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],6,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],7,0,&nMin);
				sMA[j].nAttribType = nType;
				//sMA[j].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[0] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],8,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],9,0,&nMin);
				//sMA[j].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[1] = GetRandomNumber(nMax,nMin);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],10,0,&nMax);
				tabMagicAttribe.GetInteger(pItem->m_GeneratorParam.nGeneratorLevel[j],11,0,&nMin);
				//sMA[j].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
				sMA[j].nValue[2] = GetRandomNumber(nMax,nMin);

				if (sMA[j].nValue[2]>1)
				{
					KTabFile nJnLevel;
					if (nJnLevel.Load(TABFILE_MAGICAGOLD_PATH))
					{  
						int nMinVal=0,nMaxVal=0,nRows;
						nRows = sMA[j].nValue[2]+nIsWhere;
						if (nRows>nMin)
                            nRows=nMin;
						
						nJnLevel.GetInteger(nRows,"����1��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����1���ֵ",0,&nMaxVal);
						sMA[j].nValue[0] = GetRandomNumber(nMinVal,nMaxVal); 
						
						nJnLevel.GetInteger(nRows,"����3��Сֵ",0,&nMinVal);
						nJnLevel.GetInteger(nRows,"����3���ֵ",0,&nMaxVal);
						sMA[j].nValue[2] = GetRandomNumber(nMinVal,nMaxVal); 
					}
                    nJnLevel.Clear();
				}

				continue;
			}

			sMA[j].nAttribType = 0;
			sMA[j].nValue[0] = 0;
			sMA[j].nValue[1] = 0;
			sMA[j].nValue[2] = 0;
		}

		bEC=pItem->SetAttrib_MA(sMA);
		pItem->SetPoint(nPoint);		
		tabMagicAttribe.Clear();
		pItem->EnChance(nEnChance);
	   //----------------------�Ƹֵ�����
       	///����Ϊ�����Ƹ�����
		KItemNormalAttrib	sMAR[6];  // ħ��
		ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
		KTabFile MagicTabR;
		MagicTabR.Load(TABFILE_MAGICAGOLD_PATH);  //������װ��ħ�������ļ�		
		int nTypeR,nMaxR,nMinR;

		for (j = 0;j < 6;j++)
		{ 
			if (pItem->m_GeneratorParam.nRGeneratorLevel[j] > 0 && j < nRongpiont)
			{ 
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],5,0,&nTypeR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],6,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],7,0,&nMinR);
				sMAR[j].nAttribType = nTypeR;  //��������
				//sMAR[j].nValue[0] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[j].nValue[0] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],8,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],9,0,&nMinR);
				//sMAR[j].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[j].nValue[1] = GetRandomNumber(nMaxR,nMinR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],10,0,&nMaxR);
				MagicTabR.GetInteger(pItem->m_GeneratorParam.nRGeneratorLevel[j],11,0,&nMinR);
				//sMAR[j].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLucky / 10);
				sMAR[j].nValue[2] = GetRandomNumber(nMaxR,nMinR);
				continue;
				/*
				tabMagic.GetInteger(Item[nLindexID].GetItemParam()->nRGeneratorLevel[i],5,0,&nTypeR);
				tabMagic.GetInteger(Item[nLindexID].GetItemParam()->nRGeneratorLevel[i],6,0,&nMaxR);
				tabMagic.GetInteger(Item[nLindexID].GetItemParam()->nRGeneratorLevel[i],7,0,&nMinR);
				sMAR[i].nAttribType = nTypeR;  //��������
				sMAR[i].nValue[0] = nMaxR + ((nMinR - nMaxR) * nLuck / 10);
				tabMagic.GetInteger(Item[nLindexID].GetItemParam()->nRGeneratorLevel[i],8,0,&nMaxR);
				tabMagic.GetInteger(Item[nLindexID].GetItemParam()->nRGeneratorLevel[i],9,0,&nMinR);
				sMAR[i].nValue[1] = nMaxR + ((nMinR - nMaxR) * nLuck / 10);
				tabMagic.GetInteger(Item[nLindexID].GetItemParam()->nRGeneratorLevel[i],10,0,&nMaxR);
				tabMagic.GetInteger(Item[nLindexID].GetItemParam()->nRGeneratorLevel[i],11,0,&nMinR);
				sMAR[i].nValue[2] = nMaxR + ((nMinR - nMaxR) * nLuck / 10);	
				continue;
				*/
			}
			sMAR[j].nAttribType = 0;
			sMAR[j].nValue[0] = 0;
			sMAR[j].nValue[1] = 0;
			sMAR[j].nValue[2] = 0;
		} 
		
		MagicTabR.Clear();
		bEC=pItem->SetAttrib_RON(sMAR);   //��������ħ������
		pItem->SetRPoint(nRongpiont); 		
		//pItem->EnChanceRong(nEnChance);
		return bEC;
	   //----------------------
	}
} 
//������������Ʒ
BOOL KItemGenerator::Gen_Quest(IN int nDetailType, IN OUT KItem* pItem,IN int nStackNum,IN int nLevel)
{
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if (this==NULL || pItem==NULL)
			return false;
	BOOL bEC = FALSE;

	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));
	

	//if (nDetailType>m_BPTLib.GetQuestRecordNumber()-1);	  //�ݴ� ������Χ ���е� Mine��Ʒ�� ����
	//   return bEC;

	const KBASICPROP_QUEST* pQuest = NULL;
	pQuest = m_BPTLib.GetQuestRecord(nDetailType);  //����ģ������  �������ques��
	if (NULL == pQuest)
	{
		//_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pQuest;
	pItem->SetStackNum(nStackNum);
	pItem->SetLevel(nLevel);
	return TRUE;
}
//�����������̵���Ʒ
BOOL KItemGenerator::Gen_QuestShop(IN int nDetailType,\
								   IN OUT KItem* pItem,\
								   IN int nStackNum,\
								   int nPerXu,\
								   int nPrice,\
								   IN int nLevel,int nIsBang,int nPriceType)
{
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if (this==NULL || pItem==NULL)
			return false;
	
	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));
	
	const KBASICPROP_QUEST* pQuest = NULL;
	pQuest = m_BPTLib.GetQuestRecord(nDetailType);  //����ģ������ ��ques��
	if (NULL == pQuest)
	{
		//_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pQuest;
	pItem->SetStackNum(nStackNum);
	//pItem->SetPerXu(nPerXu);
	//pItem->SetPerPrice(nPrice);
	if (nPerXu>0) //���������� �͸ı�ԭʼ����
		pItem->SetPerXu(nPerXu);
	if (nPrice>0)
		pItem->SetPerPrice(nPrice);

	pItem->SetLevel(nLevel);
	pItem->SetBang(nIsBang);
	pItem->SetModel(nPriceType);
	
	return TRUE;
}

//------------------------------------------�����Ƹ�
BOOL KItemGenerator::Gen_Fusion(IN int nParticularType,\
								IN OUT KItem* pItem,\
								IN int nStackNum,\
								IN int nLevel,\
								IN int nSeries,\
								IN int nLuck,\
								IN const int* npnaryJBLevel)
{//(nParticularType,pItem,nStackNum,nLevel,nSeries,nLuck,pnJbLevel);
   //	_ASSERT(this != NULL);
//	_ASSERT(pItem != NULL);
	if (this==NULL || pItem==NULL)
			return false;
	
	BOOL bEC = FALSE;
/*
	if (npnaryJBLevel)
        memcpy(pItem->m_GeneratorParam.nJGeneratorLevel, npnaryJBLevel, sizeof(int) * 7); //�������ԣ�itemlevel���飩
	else
        ZeroMemory(pItem->m_GeneratorParam.nJGeneratorLevel, sizeof(int) * 7);
*/
    ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));
	
	const KBASICPROP_FUSION* pQuest = NULL;

    /*#ifdef _SERVER	// �������汾
	      pQuest = m_BPTLib.GetFusionRecord(nParticularType+nLevel-1);  //��ȡ����ƸٵĻ�����Ϣ
    #else*/
	//if (nParticularType>m_BPTLib.GetFusionRecordNumber()-1);	  //�ݴ� ������Χ ���е� Mine��Ʒ�� ����
	//   return bEC;


	pQuest = m_BPTLib.GetFusionRecord(nParticularType);
	//#endif

	if (NULL == pQuest)
	{
		//_ASSERT(FALSE);
		return bEC;
	}
     
	*pItem = *pQuest;

	pItem->SetSeries(nSeries);
	pItem->SetStackNum(nStackNum);
	pItem->SetLevel(nLevel);
		
//if (nMagicA && pQuest->m_nParticularType >= 200 && pQuest->m_nParticularType <= 205)  //����ʯͷ�Ĳ���
/*	KEQCP_BASIC sBAS[7];
	ZeroMemory(sBAS,sizeof(sBAS));
	
		for (int i=0;i<7;++i)
		{//�ű�ˢ��
			if 	(pItem->m_GeneratorParam.nJGeneratorLevel[i] > 0 )  //0 1
			{  
				sBAS[i].nType = 0;  //����
				sBAS[i].sRange.nMin = pItem->m_GeneratorParam.nJGeneratorLevel[i];
				sBAS[i].sRange.nMax = pItem->m_GeneratorParam.nJGeneratorLevel[i];
				continue;
			}		
			sBAS[i].nType =0;  //����
			sBAS[i].sRange.nMin = 0;
			sBAS[i].sRange.nMax = 0;
		}

	pItem->SetAttrib_Base(sBAS);   //��������
*/
	return TRUE;
}
//------------------------------------------
BOOL KItemGenerator::Gen_Mine(IN int nDetailType, \
							  IN OUT KItem* pItem,\
							  IN int nStackNum,\
							  IN int nLevel,\
							  IN int nSerises,\
							  IN int nMagicA,\
							  IN const int* npnaryJBLevel)
{
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if (this==NULL || pItem==NULL)
			return false;
	
	BOOL bEC = FALSE;
	if (npnaryJBLevel)
        memcpy(pItem->m_GeneratorParam.nJGeneratorLevel, npnaryJBLevel, sizeof(int) * 7); //�������ԣ�itemlevel���飩
	else
        ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));

	//if (nDetailType>m_BPTLib.GetMineRecordNumber()-1);	  //�ݴ� ������Χ ���е� Mine��Ʒ�� ����
	//   return bEC;

	const KBASICPROP_MINE* pQuest = NULL;
	pQuest = m_BPTLib.GetMine(nDetailType);

	

	if (NULL == pQuest)
	{
		//_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pQuest;
	pItem->SetSeries(nSerises);
	pItem->SetStackNum(nStackNum);
	pItem->SetLevel(nLevel);

    KTabFile Mine;
    Mine.Load("\\Settings\\ItemMine.txt");  //�Ƿ����ħ�����Ե�mine��Ʒ�б�
	int nParticular=0,nMagciType=0;
	int nReg=0,mReg=0, i;
	for (i=0;i<Mine.GetHeight()-1;++i)
	{
       Mine.GetInteger(i+2,"nMineItemID",0,&nParticular);
	   Mine.GetInteger(i+2,"nMagciType",0,&nMagciType);
	   if (nParticular==pQuest->m_nParticularType)
	   {  
		  nReg=1;
		  mReg=nMagciType;
		  break;
	   } 
	}
    Mine.Clear();

//	if (nMagicA && pQuest->m_nParticularType >= 200 && pQuest->m_nParticularType <= 205)  //����ʯͷ�Ĳ���
	KEQCP_BASIC sBAS[7];
	      ZeroMemory(sBAS,sizeof(sBAS));
	if (nReg==1 && mReg==1)  //ͼ���� ����ʯ��
	{//�ű�ˢ��
		
		for (i=0;i<7;++i)
		{
		    if 	(pItem->m_GeneratorParam.nJGeneratorLevel[i] > 0 )  //0 1
			{  
		     sBAS[i].nType = 0;  //����
		     sBAS[i].sRange.nMin = pItem->m_GeneratorParam.nJGeneratorLevel[i];
		     sBAS[i].sRange.nMax = pItem->m_GeneratorParam.nJGeneratorLevel[i];
			 continue;
			}

		   sBAS[i].nType =0;  //����
		   sBAS[i].sRange.nMin = 0;
		   sBAS[i].sRange.nMax = 0;
		}
	}
	else if (nReg==1 && mReg==0)   //����ʯͷ ������װ
	{
		pItem->m_GeneratorParam.nLuck = nMagicA; //����ֵ
		sBAS[0].nType = 0;
		sBAS[0].sRange.nMin = nMagicA;
		sBAS[0].sRange.nMax = nMagicA;	
	}
	else 
	{//ʲô�����ǵ�����
		for (i=0;i<7;++i)
		{
			sBAS[i].nType =0;  //����
			sBAS[i].sRange.nMin = 0;
			sBAS[i].sRange.nMax = 0;
		}
	}
	pItem->SetAttrib_Base(sBAS);   //��������

	return TRUE;
}
//�����̵���mine��Ʒ
BOOL KItemGenerator::Gen_MineShop(IN int nDetailType, IN OUT KItem* pItem,IN int nStackNum,IN int nLevel,IN int nSerises,IN int nMagicA,int nPerXu,int nPrice,int nIsBang,int nPriceType)
{
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if (this==NULL || pItem==NULL)
			return false;
	
	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));
	
	//if (nDetailType>m_BPTLib.GetMineRecordNumber()-1);	  //�ݴ� ������Χ ���е� Mine��Ʒ�� ����
	//   return bEC;


	const KBASICPROP_MINE* pQuest = NULL;
	pQuest = m_BPTLib.GetMine(nDetailType);
	if (NULL == pQuest)
	{
		//_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pQuest;
	pItem->SetSeries(nSerises);
	pItem->SetStackNum(nStackNum);
	pItem->SetLevel(nLevel);
	//pItem->SetPerXu(nPerXu);
	//pItem->SetPerPrice(nPrice);
	if (nPerXu>0) //���������� �͸ı�ԭʼ����
		pItem->SetPerXu(nPerXu);
	if (nPrice>0)
		pItem->SetPerPrice(nPrice);

	pItem->SetBang(nIsBang);
	pItem->SetModel(nPriceType);

	KTabFile MineShop;
    MineShop.Load("\\Settings\\ItemMine.txt");
	int nParticular=0,nMagciType=0;
	int nReg=0,mReg=0,i;
	for ( i=0;i<MineShop.GetHeight()-1;++i)
	{
		MineShop.GetInteger(i+2,"nMineItemID",0,&nParticular);
		MineShop.GetInteger(i+2,"nMagciType",0,&nMagciType);
		if (nParticular==pQuest->m_nParticularType)
		{  
			nReg=1;
			mReg=nMagciType;
			break;
		} 
	}
    MineShop.Clear();
	KEQCP_BASIC sBAS[7];
	ZeroMemory(sBAS,sizeof(sBAS));
	//	if (nMagicA && pQuest->m_nParticularType >= 200 && pQuest->m_nParticularType <= 205)  //����ʯͷ�Ĳ���
    if (nReg==1 && mReg==0)  //����ʯͷ
	{
		pItem->m_GeneratorParam.nLuck = nMagicA;
		sBAS[0].nType = 0;
		sBAS[0].sRange.nMin = nMagicA;
		sBAS[0].sRange.nMax = nMagicA;

	}
	else
	{
		for (i=0;i<7;++i)
		{
			sBAS[i].nType =0;  //����
			sBAS[i].sRange.nMin = 0;
			sBAS[i].sRange.nMax = 0;
		}	
	}
	pItem->SetAttrib_Base(sBAS);
	return TRUE;
}
BOOL KItemGenerator::Gen_TownPortal(IN OUT KItem* pItem) //����һ���سǾ�
{
	//_ASSERT(this != NULL);
	//_ASSERT(pItem != NULL);
	if (this==NULL || pItem==NULL)
			return false;

	BOOL bEC = FALSE;
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));
	
	const KBASICPROP_TOWNPORTAL* pPortal = NULL;
	pPortal = m_BPTLib.GetTownPortalRecord(0);
	if (NULL == pPortal)
	{
		//_ASSERT(FALSE);
		return bEC;
	}
	*pItem = *pPortal;
	return TRUE;
}

// BOOL KItemGenerator::GetDIYItem(IN int nLuck,IN int nPoint, IN int nLevel, OUT KItem* pItem,IN int nSeries,IN int nDetail, IN int nParticular,IN int* pnMagicLevel, IN int nEnChance)
// {
// 	_ASSERT(this != NULL);
// 	_ASSERT(pItem != NULL);
// 	
// 	BOOL bEC = FALSE;
// 	
// 	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
// 	if (pnMagicLevel)
// 		memcpy(pItem->m_GeneratorParam.nGeneratorLevel, pnMagicLevel, sizeof(int) * 6);
// 	int nGameVersion = pItem->m_GeneratorParam.nVersion;
// 	
// 	// ������ڲ���, ȷ��װ���Ļ�������
// 	const int i = nParticular * 10 + nLevel - 1;// ����ʽ�ɲ߻�����
// 	// ���SPE 2002��8��31��7:40
// 	// email: ����������ֵ��
// 	// �ȼ����ݴ�1��ʼ������Ҫ������
// 	const KBASICPROP_EQUIPMENT* pEqu = NULL;
// 	switch(nDetail)
// 	{
// 	case equip_meleeweapon:
// 		pEqu = m_BPTLib.GetMeleeWeaponRecord(i);
// 		break;
// 	case equip_rangeweapon:
// 		pEqu = m_BPTLib.GetRangeWeaponRecord(i);
// 		break;
// 	case equip_armor:
// 		pEqu = m_BPTLib.GetArmorRecord(i);
// 		break;
// 	case equip_helm:
// 		pEqu = m_BPTLib.GetHelmRecord(i);
// 		break;
// 	case equip_boots:
// 		pEqu = m_BPTLib.GetBootRecord(i);
// 		break;
// 	case equip_belt:
// 		pEqu = m_BPTLib.GetBeltRecord(i);
// 		break;
// 	case equip_amulet:
// 		pEqu = m_BPTLib.GetAmuletRecord(i);
// 		break;
// 	case equip_ring:
// 		pEqu = m_BPTLib.GetRingRecord(i);
// 		break;
// 	case equip_cuff:
// 		pEqu = m_BPTLib.GetCuffRecord(i);
// 		break;
// 	case equip_pendant:
// 		pEqu = m_BPTLib.GetPendantRecord(i);
// 		break;
// 	case equip_horse:
// 		pEqu = m_BPTLib.GetHorseRecord(i);
// 		break;
// 	case equip_mask:
// 		pEqu = m_BPTLib.GetMaskRecord(i);
// 		break;
// 	default:
// 		break;
// 	}
// 	if (NULL == pEqu)
// 	{ _ASSERT(FALSE); return bEC; }
// 	// �������˵�ֱ��ԭ��: ֻ��n��װ��, ������ i ��ֵ��[0,n-1]֮��
// 	// ���3��: nParticularType ����?
// 	//			nLevel ����?
// 	//			ԭʼ��tab file������ m_BPTLib.m_BPTEquipment ��
// 	//			���������������?
// 	pItem->SetAttrib_CBR(pEqu);
// 	pItem->SetSeries(nSeries);
// 
// 	pItem->m_GeneratorParam.nLuck = nPoint;
// 	pItem->m_GeneratorParam.nVersion = nPoint;
// 	pItem->SetSet(nPoint);
// 	pItem->SetSetId(nLuck);
// 	KItemNormalAttrib sMA[6];
// 	ZeroMemory(sMA,sizeof(sMA));
// 	KTabFile tabMagicAttribe;
// 	tabMagicAttribe.Load("\\Settings\\item\\magicattrib.txt");
// 
// 	int nType,nMax,nMin;
// 	for (int j = 0;j < nPoint;++j)
// 	{
// 		if (pnMagicLevel[j])
// 		{
// 			tabMagicAttribe.GetInteger(pnMagicLevel[j],"���Ե������",0,&nType);
// 			tabMagicAttribe.GetInteger(pnMagicLevel[j],"����1���ֵ",0,&nMax);
// 			tabMagicAttribe.GetInteger(pnMagicLevel[j],"����1��Сֵ",0,&nMin);
// 			sMA[j].nAttribType = nType;
// 			sMA[j].nValue[0] = nMin + ((nMax - nMin) * nLuck / 10);
// 			tabMagicAttribe.GetInteger(pnMagicLevel[j],"����2���ֵ",0,&nMax);
// 			tabMagicAttribe.GetInteger(pnMagicLevel[j],"����2��Сֵ",0,&nMin);
// 			sMA[j].nValue[1] = nMin + ((nMax - nMin) * nLuck / 10);
// 			tabMagicAttribe.GetInteger(pnMagicLevel[j],"����3���ֵ",0,&nMax);
// 			tabMagicAttribe.GetInteger(pnMagicLevel[j],"����3��Сֵ",0,&nMin);
// 			sMA[j].nValue[2] = nMin + ((nMax - nMin) * nLuck / 10);
// 		}
// 		else
// 		{
// 			sMA[j].nAttribType = magic_no;
// 			sMA[j].nValue[0] = 0;
// 			sMA[j].nValue[1] = 0;
// 			sMA[j].nValue[2] = 0;
// 		}
// 	}
// 	pItem->SetAttrib_MA(sMA);
// 	pItem->EnChance(nEnChance);
// 
// 	tabMagicAttribe.Clear();
// 	return TRUE;
// }
