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
	int Init();
	//int Gen_Item(IN int, IN int, IN int, IN int, IN OUT KItem*, IN int = 1, IN int = 0);
	int Gen_Quest(IN int, IN OUT KItem*,IN int,IN int nLevel=0);
	int Gen_QuestShop(IN int, IN OUT KItem*,IN int,int PerXu,int nPrice,IN int nLevel=0,int nIsBang=0,int nPriceType=0);
	int Gen_Mine(IN int, IN OUT KItem*,IN int,IN int,IN int,IN int,IN const int*);
	int Gen_Fusion(IN int,IN OUT KItem*,IN int,IN int,IN int,IN int,IN const int*);
    int Gen_MineShop(IN int, IN OUT KItem*,IN int,IN int,IN int,IN int,int PerXu,int nPrice,int nIsBang=0,int nPriceType=0);
	int Gen_TownPortal(IN OUT KItem*);
	int Gen_Medicine(IN int, IN int, IN int, IN OUT KItem*, IN int);
    int Gen_MedicineShop(IN int, IN int, IN int, IN OUT KItem*, IN int,int PerXu,int nIsBang=0,int nPriceType=0);
	int Gen_Equipment(IN int, IN int, IN int, IN int, IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int,IN int IsWhere=0,IN int nIsShop=FALSE,IN int nPrice=0,IN int nFsxu=0,int nIsBang=0,int nPriceType=0);  //��Ҫ�����̵�
	int Gen_paiEquipment(IN int, IN int, IN int, IN int, IN const int*, IN const int*,
		IN UINT, IN OUT KItem*,IN int,IN int,IN int,IN int IsWhere=0,IN int nIsShop=FALSE,
		IN int nPrice=0,IN int nFsxu=0,IN int nIsBang=0,IN int nPriceType=0,IN int nLucky=0);  //��Ҫ����������
	int Gen_EquipmentRong(IN int, IN int, IN int, IN int, IN const int*,IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int,IN int,IN int=0,IN OUT KMagicAttrib* =NULL,IN int=0,IN int nIsLogin=FALSE); //������װ
	int Gen_EquipmentCilent(IN int, IN int, IN int, IN int, IN const int*,IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int,IN int,IN const int*,IN const int*,IN const int*,IN const int*,IN int=0); //�ͻ��˲���װ��
	int Gen_ExistEquipment(IN int, IN int, IN int, IN int, IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int, IN const int*,IN int,IN int=0);//�����½ʱ����װ��
	int GetEquipmentCommonAttrib(IN int, IN int, IN int, IN int, IN OUT KItem*);
	int GetMedicineCommonAttrib(IN int, IN int, IN OUT KItem*);
	// Add by flying on May.30.2003
	// Try to get a "Gold Item" by random or by index.
	//int GetGoldItemByRandom(IN int, OUT KItem*);
	int GetGoldItemByRandom(IN int,IN int, IN int, IN int, IN int, IN const int*,IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int nIsLogin=FALSE,IN int nPoint=8);
	int GetGoldItemByIndex(IN int, OUT KItem*, IN const int* , IN int ,IN int,IN int,IN int,IN int nIsLogin=FALSE);
	int GetPlatinaByIndex(IN int, OUT KItem*, IN const int* , IN int ,IN int,IN int,IN int,IN int nIsLogin=FALSE);
    //int GetGoldItemByRong(IN int, OUT KItem*, IN const int* , IN int ,IN int,IN int); //��������װ��
	int GetGoldItemByRong(IN int, IN int, IN int, IN int, IN const int*,IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int,IN const int* =NULL,IN int nIsLogin=FALSE);
	int GetPlatinaItemByRong(IN int, IN int, IN int, IN int, IN const int*,IN const int*, IN int,
		IN int, IN OUT KItem*,IN int,IN int,IN int,IN const int* =NULL,IN int nIsLogin=FALSE);



	//	int GetDIYItem(IN int,IN int, IN int, OUT KItem*, IN int, IN int, IN int,IN int*, IN int);
	// �����Ǹ�������
private:
	int Gen_MagicAttrib(int, const int*, int, int, KItemNormalAttrib*, int nVersion);
	const KMAGICATTRIB_TABFILE* GetMARecord(int) const;
	/*int GMA_GetAvaliableMA(int);
	int GMA_GetLevelAndDropRate(int);
	int  GMA_GetCandidateMA(int, int, int);
	void GMA_ChooseMA(int nPos, int nLevel, int nLucky, KItemNormalAttrib* pINA); */
};

extern KItemGenerator	ItemGen;			//	װ��������
#endif	// #ifndef	KItemGeneratorH
