#ifndef	KItemListH
#define	KItemListH
#include "engine/KLinkArray.h"
#include "KItem.h"

#define	MAX_ITEM_ACTIVE	3

#define	REQUEST_EQUIP_ITEM		1
#define	REQUEST_EAT_MEDICINE	2

typedef struct
{
	int		nPlace;
	int		nX;     // 0 1 2 3 4
	int		nY;     // 0123456789
	int     nSIsRongqi;
	int     nDIsRongqi;
} ItemPos;

typedef struct
{
	int		nPlace;
	int		nX;     // 0 1 2 3 4
	int		nY;     // 0123456789
	int     nIsRongqi;
	int     nIdx;
} ItemXinXi;

class  KItemList
{
private:
	int			m_PlayerIdx;
	int			m_Hand;
	int			m_nBackHand;
	int			m_EquipItem[itempart_num];						// ���װ���ĵ��ߣ���Ӧ��Ϸ�����е��������������
	int			m_ItemBox[MAX_ITEM_BOX];                        // װ����������
	PlayerItem	m_Items[MAX_PLAYER_ITEM];						// ���ӵ�е����е��ߣ�����װ���ŵĺ�������ŵģ���Ӧ��Ϸ�����е��������������
	PlayerItem	m_sBackItems[MAX_PLAYER_ITEM];					// ���׹����� m_Items �ı���
	static int	ms_ActiveEquip[itempart_num][MAX_ITEM_ACTIVE];	// ĳ�����ֵ�װ���������װ��
	static int	ms_ActivedEquip[itempart_num][MAX_ITEM_ACTIVE];	// ĳ�����ֵ�װ����ʲôװ������
	KLinkArray	m_FreeIdxItemList;
	KLinkArray	m_UseIdxItemList;
	int			m_nListCurIdx;									// ���� GetFirstItem �� GetNextItem
	int         m_ItemCount;                                    // ��ҵ�������Ʒ������
	int		m_bActiveSet;                   //�Ƿ񼤻����ÿ���
	int		m_nMaskLock;	                //mat na
	int		m_bLockOperation;               //�Ƿ����ſͻ���
public:
	KInventory	m_Room[room_num];
private:
	int			FindFree();
	int			FindSame(int nGameIdx);							// nGameIdxָ��Ϸ�����е�������ı��
	int			GetEquipPlace(int nType);						// ȡ��ĳ����װ��Ӧ�÷ŵ�λ��
	int		Fit(int nIdx, int nPlace);						// ����Ƿ����װ����
	int		Fit(KItem* pItem, int nPlace);
	int			GetEquipEnhance(int nPlace);					// ȡ��װ���ļ�����������
	int			GetActiveEquipPlace(int nPlace, int nCount);	// ȡ��nPlace��װ������ĵ�nCount��װ��λ��
	void		InfectionNextEquip(int nPlace, int bEquip = FALSE);
	// ��room_equipment�в���ָ��Genre��DetailType����Ʒ���õ�ItemIdx��λ��
	int		FindSameDetailTypeInEquipment(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY);
	friend	class KPlayer;

public:
	KItemList();
	~KItemList();
	int			Init(int nIdx);
	int			GetEquipment(int nIdx) { return m_EquipItem[nIdx]; }
	int         GetEquPlace(int nIdx) { return GetEquipPlace(nIdx); }
	int         _getEquipPlaceByItemIdx(int nidx);
	int			GetItemBox(int nIdx) { return m_ItemBox[nIdx]; }
	void        GetItemsPos(int nIdx,ItemPos* nPos){nPos->nPlace=m_Items[nIdx].nPlace;nPos->nX=m_Items[nIdx].nX;nPos->nY=m_Items[nIdx].nY;};
	int			GetActiveAttribNum(int nIdx);					// nIdxָ��Ϸ�����е�������ı��
	int			GetWeaponType();								// ȡ�����������ͣ���������(equip_meleeweapon)����Զ������(equip_rangeweapon)
	int			GetWeaponParticular();							// ȡ�������ľ������ͣ����ڼ��㲻ͬ���˺���ǿ
	void		GetWeaponDamage(int* nMin, int* nMax);			// ȡ���������˺�
	int			Add(int nIdx, int nPlace, int nX, int nY, int bInit = FALSE,int nIsAutoDie=0,int StackNum=1,int nIsLoign=0);		// nIdxָ��Ϸ�����е�������ı��
	int		Remove(int nIdx,int ndX=0,int ndY=0,int nModel=0,int nIsSave=TRUE);								// nIdxָ��Ϸ�����е�������ı��
	void		RemoveAll();
	int		CanEquip(int nIdx, int nPlace = -1);			// nIdxָ��Ϸ�����е�������ı��
	int        CanTonEquip(int nIdx, int nPlace=-1);
	int		CanEquip(KItem* pItem, int nPlace = -1);
	int		EnoughAttrib(void* pData,int nPlace=-1, unsigned long nDwIdx=0);
	int		Equip(int nIdx, int nPlace = -1,int nIsLoign=0);				// nIdxָ��Ϸ�����е�������ı��
	int		TonEquip(int nIdx, int nPlace = -1,int nIsLoign=0);
	int		UnEquip(int nIdx, int nPos = -1);				// nIdxָ��Ϸ�����е�������ı��
	int        UnTonEquip(int nIdx, int nPos=-1);              //�Ƴ�ͬ���װ��
	int        RemoveItemBox(int nIdx , int nPos=-1);
	int        TimeUnEquip(int nIdx,int nPos = -1);            // �Ƴ�ʱ�䵽��ĳװ������
	int        tempUnEquip(int nIdx,int nPos=-1);
	void        ReSkillEnhance(int m_PlayerIdx,int nNpcIdx);
	int		EatMecidineS(int nIdx,int nx=0,int nY=0);							// nIdxָ��Ϸ�����е�������ı��
	PlayerItem*	GetFirstItem();
	PlayerItem*	GetNextItem();
	int			SearchID(int nID);
	int	        SearchItemsIdx(int nDwid);
	void		ExchangeMoney(int nSrcRoom, int DesRoom, int nMoney);
	void		ExchangeItem(ItemPos* SrcPos,ItemPos* DesPos,int nIsMianBam=0);
	int         QiangHua(int nLindexID=0,int nBaoshiID=0,int nRank=0);//ǿ��װ��
	int         RongLian(int nLindexID=0,int nBaoshiID=0);//װ������
	int         WenGang(int nLindexID=0,int nBaoshiID=0); //�ƽ�װ���Ƹ�
	int         BlueWenGang(int nLindexID,int nBaoshiID); //��װ�Ƹ�
	int         PurpleWenGang(int nLindexID,int nBaoshiID); //��װ�Ƹ�

	int         BlueBianShi(int nLindexID,int nBaoshiID); //��װ��ʶ
	int         BlueRonHe(int nIdxa,int nIdxb,int nEqType,int nRank,int nCurLuck,int nsLevel);   //��װ�ں�
	int         LingHe(int nLindexID,int nBaoshiID);      //��ʯ���
	int         WenGangGenerator(int nRItemLevelRows);    //�Ƹֲ�����
	int         TiLian(int nLindexID=0,int nBaoshiID=0,int nModel=0,int nWenGangPos=0);
	int         BlueTiLian(int nLindexID=0,int nBaoshiID=0,int nModel=0,int nWenGangPos=0);
	int         CilentAddItem(int nItemIdx=0);      // ��������
    int        CheckItemType(int ItemClass,int Detail,int nPart,int IsMianBan);
	int			GetMoneyAmount();					// �õ���Ʒ���ʹ��������Ǯ��
	int			GetEquipmentMoney();				// �õ���Ʒ���ʹ������Ǯ��
	int		AddmServer(int nRoom, int nMi);
	int		CostMoney(int nMoney);
	int		DecMoney(int nMoney);
	void		SetClient(int nMon1, int nMon2, int nMon3);
	void		SetRoomMoney(int nRoom, int nMoney);
	int			GetsMon(int nRoom) { return m_Room[nRoom].GetmServer(); }	// ȡ�ÿռ��Ǯ

	int			GetIndexItem(int nRoom,int nIndex) { return m_Room[nRoom].GetCurIndexItem(nIndex); }
	//Xu
	int			GetxLient(int nRoom) { return m_Room[nRoom].GetxCLient(); }
	void		SetServer(int nXu);
	void		SetRoomXu(int nRoom, int nXu);
	int		AddxCliet(int nRoom, int ni);
	int		CostXu(int nXu);
	//int		DecXu(int nXu);
	int			GetXuAmount();
	int			GetEquipmentXu();
	/////////////////////////////////               // ------------
	void		SetPlayerIdx(int nIdx);				// �趨�������
	int			Hand() { return m_Hand; };
	void		ClearRoom(int nRoom);
	void		BackupTrade();
	void		RecoverTrade();
	void		StartTrade();
	int			GetItemNum(int nGenre, int nDetailType, int nParticular, int nLevel = 1,ITEM_POSITION nRongType=pos_equiproom); // dem so item cung loai
	int         GetItemCount();
	int GetPlayerItemCount()
	{
		/*int nIdx = 0;
		int nCount=0;
		while((nIdx = m_UseIdxItemList.GetNext(nIdx)))
		{
			nCount++;
		}
	    return nCount;*/
		return m_ItemCount;
	};
	void		RemoveAllInOneRoom(int nRoom);//ɾ���������ж�����Ʒ
	void		BackupGive();
	void		RecoverGive();
	void		StartGive();

	int		GetIfActive();
	int		GetMaskLock() {return m_nMaskLock;};	// mat na
	void		SetMaskLock(int bFlag);// mat na

    int		CheckCanPlaceInEquipment(int nWidth, int nHeight, int *pnX, int *pnY,int nRoomType=0);// �ж�һ���������Ʒ�ܷ�Ž���Ʒ��
	int		SearchPosition(int nWidth, int nHeight, ItemPos* pPos);
	int         GetFreePositionCount(int nWidth, int nHeight,int RoomKind);
	int        SearchItemPosition(int nWidth, int nHeight, ItemPos* pPos,int roomType);
	int        SearchItemPositionKienTM(int nWidth, int nHeight, int posX,int posY,int roomType);

    void		AutoEquip(ItemPos SrcPos,ItemPos DesPos,unsigned long nSrcItemDwIdx,int nEuqPlace,int nKind=0);
    int         AutoChangeItem(int nIdx); //�Զ���������
	int			UseItem(int nIdx,int cnx=0,int cny=0);					//nIdxָ��Ϸ�����е�������ı��
	//int		SearchPosition(int nWidth, int nHeight, ItemPos* pPos);
	//int        SearchItemPosition(int nWidth, int nHeight, ItemPos* pPos,int roomType);
	int		AutoMoveItem(ItemPos SrcPos,ItemPos DesPos,unsigned int nItemdwID=0);
	void		MenuSetMouseItem();
//	void		RemoveAllInOneRoom(int nRoom);
	void		LockOperation();											// �����ͻ��˶�װ���Ĳ���
	void		UnlockOperation();
	int		IsLockOperation() { return m_bLockOperation; };
	int			GetSameDetailItemNum(int nImmediatePos);
	int	        GetImmediacyItemIndex(int nImmediatePos);
    int	        GetSameEquipmentItemNum(int nIdx);
	int			GetGoldColor(int nSet,int nId);
	void	    ClientShowMsg(const char *Msg);
	void        CheckBianShiItemTime();
};
#endif
