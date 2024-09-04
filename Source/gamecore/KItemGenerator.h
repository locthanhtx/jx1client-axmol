//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItemGenerator.h
// Date:	2002.08.26
// Code:	DongBo
// Desc:    header file. ���ļ���������������ɵ���
//---------------------------------------------------------------------------

#ifndef	KItemGeneratorH
#define	KItemGeneratorH

#include "KBasPropTbl.h"
#include "KItem.h"

#define		IN
#define		OUT

#define	NUMOFCMA	15		// ����ֵ. ��ÿ��װ��ʹ�õ�ħ����������������

//class KItem;

class KItemGenerator
{
public:
	KItemGenerator();
	~KItemGenerator();
	
	// �����Ǻ��ĳ�Ա����
protected:
	KLibOfBPT	m_BPTLib;
	
	// �����Ǹ�����Ա����
	/*int			m_GMA_naryMA[2][NUMOFCMA];	// [0][x]: ǰ׺; [1][x]: ��׺
	int			m_GMA_nCount[2];
	int			m_GMA_naryLevel[2][NUMOFCMA];
	int			m_GMA_naryDropRate[2][NUMOFCMA];
	int			m_GMA_naryCandidateMA[NUMOFCMA]; */
	int			m_EquipNumOfEntries[equip_detailnum];
	int			m_MedNumOfEntries;
	// �����Ƕ���ӿ�
public:
	BOOL Init();
	//BOOL Gen_Item(IN int, IN int, IN int, IN int, IN OUT KItem*, IN int = 1, IN int = 0);
	BOOL Gen_Quest(IN int, IN OUT KItem*,IN int,IN int nLevel=0);
	BOOL Gen_QuestShop(IN int, IN OUT KItem*,IN int,int PerXu,int nPrice,IN int nLevel=0,int nIsBang=0,int nPriceType=0);
	BOOL Gen_Mine(IN int, IN OUT KItem*,IN int,IN int,IN int,IN int,IN const int*);
	BOOL Gen_Fusion(IN int,IN OUT KItem*,IN int,IN int,IN int,IN int,IN const int*);
    BOOL Gen_MineShop(IN int, IN OUT KItem*,IN int,IN int,IN int,IN int,int PerXu,int nPrice,int nIsBang=0,int nPriceType=0);
	BOOL Gen_TownPortal(IN OUT KItem*);
	BOOL Gen_Medicine(IN int, IN int, IN int, IN OUT KItem*, IN int);
    BOOL Gen_MedicineShop(IN int, IN int, IN int, IN OUT KItem*, IN int,int PerXu,int nIsBang=0,int nPriceType=0);
	BOOL Gen_Equipment(IN int, IN int, IN int, IN int, IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int,IN int IsWhere=0,IN BOOL nIsShop=FALSE,IN int nPrice=0,IN int nFsxu=0,int nIsBang=0,int nPriceType=0);  //��Ҫ�����̵�
	BOOL Gen_paiEquipment(IN int, IN int, IN int, IN int, IN const int*, IN const int*,
		IN UINT, IN OUT KItem*,IN int,IN int,IN int,IN int IsWhere=0,IN BOOL nIsShop=FALSE,
		IN int nPrice=0,IN int nFsxu=0,IN int nIsBang=0,IN int nPriceType=0,IN int nLucky=0);  //��Ҫ����������
	BOOL Gen_EquipmentRong(IN int, IN int, IN int, IN int, IN const int*,IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int,IN int,IN int=0,IN OUT KMagicAttrib* =NULL,IN int=0,IN BOOL nIsLogin=FALSE); //������װ	
	BOOL Gen_EquipmentCilent(IN int, IN int, IN int, IN int, IN const int*,IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int,IN int,IN const int*,IN const int*,IN const int*,IN const int*,IN int=0); //�ͻ��˲���װ��
	BOOL Gen_ExistEquipment(IN int, IN int, IN int, IN int, IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int, IN const int*,IN int,IN int=0);//�����½ʱ����װ��	
	BOOL GetEquipmentCommonAttrib(IN int, IN int, IN int, IN int, IN OUT KItem*);
	BOOL GetMedicineCommonAttrib(IN int, IN int, IN OUT KItem*);
	// Add by flying on May.30.2003
	// Try to get a "Gold Item" by random or by index.
	//BOOL GetGoldItemByRandom(IN int, OUT KItem*);
	BOOL GetGoldItemByRandom(IN int,IN int, IN int, IN int, IN int, IN const int*,IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN BOOL nIsLogin=FALSE,IN int nPoint=8);
	BOOL GetGoldItemByIndex(IN int, OUT KItem*, IN const int* , IN int ,IN int,IN int,IN int,IN BOOL nIsLogin=FALSE);
	BOOL GetPlatinaByIndex(IN int, OUT KItem*, IN const int* , IN int ,IN int,IN int,IN int,IN BOOL nIsLogin=FALSE);
    //BOOL GetGoldItemByRong(IN int, OUT KItem*, IN const int* , IN int ,IN int,IN int); //��������װ��
	BOOL GetGoldItemByRong(IN int, IN int, IN int, IN int, IN const int*,IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int,IN const int* =NULL,IN BOOL nIsLogin=FALSE);
	BOOL GetPlatinaItemByRong(IN int, IN int, IN int, IN int, IN const int*,IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int,IN const int* =NULL,IN BOOL nIsLogin=FALSE);


	
	//	BOOL GetDIYItem(IN int,IN int, IN int, OUT KItem*, IN int, IN int, IN int,IN int*, IN int);
	// �����Ǹ�������
private:
	BOOL Gen_MagicAttrib(int, const int*, int, int, KItemNormalAttrib*, int nVersion);
	const KMAGICATTRIB_TABFILE* GetMARecord(int) const;
	/*BOOL GMA_GetAvaliableMA(int);
	BOOL GMA_GetLevelAndDropRate(int);
	int  GMA_GetCandidateMA(int, int, int);
	void GMA_ChooseMA(int nPos, int nLevel, int nLucky, KItemNormalAttrib* pINA); */
};

extern KItemGenerator	ItemGen;			//	װ��������
#endif	// #ifndef	KItemGeneratorH
