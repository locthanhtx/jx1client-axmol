//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiSellItem__
#define __KuiSellItem__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
USING_NS_AX;
USING_NS_CC_EXT;

#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define MAX_ITEM_COUNT  60

class KuiSellItem:public Layer{
public:
	KuiSellItem();
	~KuiSellItem();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiSellItem);

	static KuiSellItem * create(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	virtual void update(float delta);
//	virtual void draw();
	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd);//��Ʒ�仯
	bool    isOpen;
	void    setCanMove(bool ver){__isCanMove =ver;};
private:
	//���Ӽ���ͼ��
	void addDialogData();
	//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);
	void ccTouchMoved(Touch *pTouch, Event *pEvent);
	void ccTouchEnded(Touch *pTouch, Event *pEvent);
	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);

	void tuoguanCallBackFunc(Ref * pSender);
	void kuaiCallBackFunc(Ref * pSender);
	void zhengCallBackFunc(Ref * pSender);
	void gaoCallBackFunc(Ref * pSender);
	void tanCallBackFunc(Ref * pSender);
	void biaoCallBackFunc(Ref * pSender);
	void saveCallBackFunc(Ref * pSender);

	void closesavemonyPadCallback(Node *pNode);
	void closegaoPadCallback(Node *pNode);
	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	Size winSize;
	//�Ի��򱳾�����
	Sprite * m_bgSprite;
	Sprite *ParentNode_Item;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	//Label* pMoneyLabel;
	//Label* pXuLabel;
	LayerColor * colorLayer;
	//Action* red;
	Menu * savemenu;

	bool m_descPadIsOpen;
	bool m_EqdescPadIsOpen;
	bool m_gaoPadIsOpen;
	int  m_nMoney;
	int  m_nXu;
	int  m_nNumObjects;
	POINT m_StartPos;//���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	//KuiSellItemdObject m_ItemData[MAX_ITEM_COUNT];//KuiSellItemObject
	int AddObject(KUiDraggedObject* pObject, int nCount);
	typedef std::map<std::string,KUiItemdObject> _ItemData;                 //SkillId,Echance
	_ItemData m_ItemData;
	void closelockPadCallback(Node *pNode);
	//int   m_CurSelItemIndex;
	void  closeItemPadCallback(Node *pNode);
	void  UpdateData();
	std::string  nstrCurSelItemKey;
	float m_MovePosX;
	bool  m_nIstrue;
	bool  m_savemoneyPadIsOpen;
	unsigned int nBakChickTime;
	char m_ShopName[64];
	int   m_IsSetPriceCount;
	Menu * menu;
	bool isDoIt;
	bool m_bScrolling;
	Point m_lastPoint;
	bool __isCanMove;
};

#endif /* defined(__MyGame__PopLayer__) */
