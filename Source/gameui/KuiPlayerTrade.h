//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiPlayerTrade__
#define __KuiPlayerTrade__
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define MAX_ITEM_COUNT  60
#define MAX_TRADE_COUNT  32
#include <iostream>
#include "cocos2d.h"

#include <map>
USING_NS_AX;


//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiPlayerTrade:public Layer{
public:
	KuiPlayerTrade();
	~KuiPlayerTrade();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiPlayerTrade);

	static KuiPlayerTrade * create(KUiPlayerItem* pOther,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	//char* pszTitle, char* pszInitString,const int nNum,const char* pszCallback,const char* pszError

	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd);//��Ʒ�仯
	void	UpdateGiveItem(KUiObjAtRegion* pItem, int bAdd);
	void	UpdateTarGiveItem(KUiObjAtRegion* pObj, int bAdd);
	void	UpdateTradeOperData();
	virtual void update(float delta);
//	virtual void draw();
	void	__ClosePad();
	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);

	void  closeItemPadCallback(Node *pNode);
	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);
	void btnCallBackFunc_fangzhi(Ref * pSender);
	void btnCallBackFunc_quxia(Ref * pSender);
	void okCallBackFunc(Ref * pSender);
	void tradeCallBackFunc(Ref * pSender);
	void zhengCallBackFunc(Ref * pSender);

	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;

	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);
	typedef std::map<std::string,KUiItemdObject> _ItemData;                 //SkillId,Echance
	_ItemData m_ItemData;

	typedef std::map<unsigned int,KUiItemdObject> _giveItemData;                 //SkillId,Echance
	_giveItemData m_giveItemData;

	typedef std::map<unsigned int,KUiItemdObject> _targiveItemData;
	_targiveItemData m_targiveItemData;

	bool  m_nIstrue;
	int AddObject(KUiDraggedObject* pObject, int nCount);     //�ɲ���
	int AddGiveObject(KUiDraggedObject* pObject, int nindex); //�Լ���� �ɲ���
	int AddtarGiveObject(KUiDraggedObject* pObject, int nindex); //�Լ���� �ɲ���
	void  UpdateItemData();
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	Size winSize;
	//Size itemBoxSize;
	//Size giveBoxSize;
	//�Ի��򱳾�����
	//Sprite * m_bgSprite;
	Sprite *ParentNode_ItemEx;

	Node *ParentNode_Item;
	Node *ParentNode_Give;
	Node *ParentNode_tarGive;

	MenuItemSprite * okConfirm;
	MenuItemSprite * tradeConfirm;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRectItem(Node* pNode,int i);
	Rect getGiveItem(Node* pNode,int i);
	Rect gettarGiveItem(Node* pNode,int i);
	void   OnAdjustedMoney();
	void   OnModififyMoney();
	Label* pMoneyLabel;
	Label* ptradeInfo;
	Label* m_OtherMoney;

	EditBox *m_SelfMoneyBox;
	//Label* pXuLabel;
	LayerColor * colorLayer;
	Action* red;
	//Sprite *btnBackPic;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	POINT m_StartPos_g; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	POINT m_StartPos_t; //�Է���Ʒ��
	int  m_nItemNumObjects;
	std::string nstrCurSelItemKey;
	int inCurSelItemKey;
	bool m_descPadIsOpen;
	int m_itemNum,m_giveNum,m_nNeed,m_targiveNum;
	//char   m_Callback[64];
	//char   m_Error[16];
	unsigned int nBakChickTime;
	int  m_nMoney;
	//int  m_nXu;
	KUiPlayerItem m_OtherData;
	int	 m_bAdjustingMoney;
	int	 m_nTakewithMoney;
	int	 m_nSelfTradeMoney;
	bool __IsLock;
};

#endif /* defined(__MyGame__PopLayer__) */
