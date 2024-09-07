#ifndef KItemSetH
#define	KItemSetH

#include "engine/KLinkArray.h"

#define		IN
#define		OUT
class	KItem;
//ĥ������
enum enumAbrade
{
	    enumAbradeAttack = 0,  //����ĥ��
		enumAbradeDefend,      //������ʱ��ĥ��
		enumAbradeMove,        //�ƶ�ĥ��
		enumAbradeEquip,       //��װĥ��
		enumAbradeNum,
};
// ���֮�佻�׽���ʱ�������ж������Ʒ���ܷ�����������Ʒ
typedef struct
{
	int		m_nIdx;
	int		m_nX;
	int		m_nY;
	int		m_nWidth;
	int		m_nHeight;
} TRADE_ITEM_INFO;

typedef struct
{
	int		nPriceScale;
	int		nMagicScale;
	int		nLevelScale;
} REPAIR_ITEM_PARAM;

class KItemSet
{
private:
	unsigned long			m_dwIDCreator;		        //  ID�����������ڿͻ�����������˵Ľ���
	KLinkArray		m_FreeIdxItemSet;			//	���ñ�
	KLinkArray		m_UseIdxItemSet;			//	���ñ�

public:
	int				m_nItemAbradeRate[enumAbradeNum][itempart_num];   //ĥ�����
	REPAIR_ITEM_PARAM	m_sRepairParam;
public:
	KItemSet();
	~KItemSet();
	void			Init();
	int				GetItemCount(IN int nItemGenre = -1);
	int				SearchID(IN unsigned long dwID);
	int				Add(KItem* pItem);
	int				AddOther(IN int nItemGenre, IN int nSeries, IN int nLevel=1, IN int nLuck=0, IN int nDetail = -1, \
		                               IN int nParticular = -1, IN int* pnMagicLevel = NULL, IN int nVersion = 0,\
							IN UINT nRandomSeed = 0,IN int nStackNum = 1, IN int nEnChance = 0,IN int nPoint = 0, \
						  IN int nYear = 0, IN int nMonth = 0, IN int nDay = 0, IN int nHour = 0,IN int nMin = 0,\
						                                        IN int nRongpiont=0,IN int* pnRMagicLevel = NULL,\
						                        IN int* pnJBLevel = NULL,IN int* pnBsLevel=NULL,IN int IsWhere=0,IN int nIsLogin=FALSE); //��װ �Լ� ��װ����
	int				AddCilentItem(IN int nItemGenre, IN int nSeries, IN int nLevel=1, IN int nLuck=0, IN int nDetail = -1,\
		                                    IN int nParticular = -1, IN int* pnMagicLevel = NULL, IN int nVersion = 0, \
											       IN UINT nRandomSeed = 0,IN int nStackNum = 1, IN int nEnChance = 0,\
				            IN int nPoint = 0, IN int nYear = 0, IN int nMonth = 0, IN int nDay = 0, IN int nHour = 0,\
									                 IN int nMin = 0,IN int nRongpiont=0,IN int* pnRMagicLevel = NULL, \
									       IN int* pnJBLevel = NULL,IN int* pnMagType = NULL,IN int* pnMagVala = NULL,\
									                      IN int* pnMagValb = NULL,IN int* pnMagValc = NULL,IN int=0);

	int				AddGold(IN int nId , IN int* pnMagicLevel = NULL , IN int nSeries = 0, IN int nEnChance = 0,\
		                    IN int nRongpiont=0,IN int nYear = 0, IN int nMonth = 0, IN int nDay = 0, IN int nHour = 0,\
							IN int nMin = 0,IN int* pnRMagicLevel = NULL,IN int* pnBMagicLevel=NULL, IN int nLevel=1,\
							IN int nLuck=1, IN int nDetailType = -1, IN int nParticular = -1,IN int nStackNum = 1,IN int nVersion = 0,IN int nIsPlatina=0,IN int nIsLogin=FALSE,IN UINT nRandomSeed = 0);//�ƽ��
	int				AddRondomGold(IN int nId , IN int* pnMagicLevel = NULL , IN int nSeries = 0, IN int nEnChance = 0,\
		                    IN int nRongpiont=0,IN int nYear = 0, IN int nMonth = 0, IN int nDay = 0, IN int nHour = 0,\
		                    IN int nMin = 0,IN int* pnRMagicLevel = NULL, IN int nLevel=1,\
		                    IN int nLuck=1, IN int nDetailType = -1, IN int nParticular = -1,IN int nStackNum = 1,IN int nVersion = 0,IN int nIsPlatina=0,IN int nIsLogin=FALSE,IN UINT nRandomSeed = 0);//�ƽ��

    int             AddRong(IN int Goidid, IN int nSeries, IN int nLevel=1, IN int nLuck=0, IN int nDetailType = -1,\
		                      IN int nParticular = -1, IN int* pnMagicLevel = NULL, IN int* rnMagicLevel = NULL,\
					 int* BnMagicLevel = NULL,IN int nVersion = 0, IN UINT nRandomSeed = 0,IN int nStackNum = 1, \
							    IN int nEnChance = 0,IN int nRongID = 0, IN int nPoint = 0,IN int mIsDaZhao = 0, \
							IN KMagicAttrib *nItemMag=NULL,IN int nYear = 0, IN int nMonth = 0, IN int nDay = 0,\
							IN int nHour = 0, IN int nMin = 0,IN int IsWhere=0,int nIsLoggin=FALSE);
	void			Remove(IN int nIdx);
	int				GetAbradeRange(IN int nType, IN int nPart);
private:
	void			SetID(IN int nIdx);
	int				FindFreeItemSet();
};

extern KItemSet	ItemSet;
#endif

