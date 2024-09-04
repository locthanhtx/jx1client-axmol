//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiDynamicShop__
#define __KuiDynamicShop__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;

using namespace ui;
#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define PAGE_MAX_COUNT_ITEM	60
#define	MAX_DYNANMIC_PAGETAB 12
class KuiDynamicShop:public Layer{
public:
	KuiDynamicShop();
	~KuiDynamicShop();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiDynamicShop);

	static KuiDynamicShop * create(BuySellInfo* pBSinfo,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
private:
	void UpdateData(int nSellIdx=0);
	//���Ӽ���ͼ��
	void addDialoaData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);
	void touchPageBtnEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buyCallBackFunc(Ref * pSender);
	//��һҳ
	void backCallBackFunc(Ref * pSender);
	//��һҳ
	void nextCallBackFunc(Ref * pSender);
	void closeCallBackFunc(Ref * pSender);
	void closedescPadCallback(Node *pNode);
	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	//�Ի��򱳾�����
	Sprite * m_bgSprite;
	Sprite *ParentNode_ItemEx;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	void   SetPage(int nIndex);
    int    AddObject(KUiDraggedObject* pObject, int nCount);
	void	Clear();
	void	SellSortChange(int i);
	Layer *m_pUiLayer;
	Button* btnPageBox[MAX_DYNANMIC_PAGETAB];
	Label* pMoneyLabel;
	Label* pXuLabel;
	LayerColor * colorLayer;
	Action* red;
	Node * ItemNode;//һ����
	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	int m_nObjCount;
	int m_CurShopIdx;
	int m_nPageCount;
	int	m_nCurrentPage;  //��ǰҳ
	int	m_nCurrentCount;
	int m_ItemIndex[PAGE_MAX_COUNT_ITEM];
	KUiObjAtContRegion* m_pObjsList;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	bool m_descPadIsOpen;
	int m_selItemIndex;
	EditBox *pstrEditBox_count;
	BuySellInfo m_pBSinfo;
	int m_nSellTypeCount;
	int m_nCurrentShopId[MAX_DYNANMIC_PAGETAB];
};

#endif /* defined(__MyGame__PopLayer__) */
