//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiItem__
#define __KuiItem__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
USING_NS_AX;
USING_NS_CC_EXT;

#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	50   //ÿ�����ӵĸ߶ȺͿ��
#define MAX_ITEM_COUNT  60

class KuiItem:public Layer{
public:
	KuiItem();
	~KuiItem();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiItem);

	static KuiItem * create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	virtual void update(float delta);
//	virtual void draw();
	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd);//��Ʒ�仯
	void    setCanMove(bool ver){__isCanMove =ver;};
	bool    isOpen;
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
	void ClientShowMsg(const char *Msg);
	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);

	void chuanCallBackFunc(Ref * pSender);
	void diuCallBackFunc(Ref * pSender);
	void cunCallBackFunc(Ref * pSender);
	void lianCallBackFunc(Ref * pSender);
	void kuaiCallBackFunc(Ref * pSender);
	void zhengCallBackFunc(Ref * pSender);
	void xiuCallBackFunc(Ref * pSender);
	void maiCallBackFunc(Ref * pSender);
	void chaiCallBackFunc(Ref * pSender);
	void chuCallBackFunc(Ref * pSender);
	void ziCallBackFunc(Ref * pSender);
	void kuo1CallBackFunc(Ref * pSender);
	void kuo2CallBackFunc(Ref * pSender);
	void kuo3CallBackFunc(Ref * pSender);
	void saveCallBackFunc(Ref * pSender);

	void closesavemonyPadCallback(Node *pNode);
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
	Sprite *ParentNode_ItemMove;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	Label* pMoneyLabel;
	Label* pXuLabel;
	LayerColor * colorLayer;
	//Action* red;
	Menu * savemenu;

	bool m_descPadIsOpen;
	bool m_TempPadIsOpen;
	bool m_EqdescPadIsOpen;
	int  m_nMoney;
	int  m_nXu;
	int  m_nNumObjects;
	POINT m_StartPos;//���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	//KUiItemdObject m_ItemData[MAX_ITEM_COUNT];//KUiItemObject
	int AddObject(KUiDraggedObject* pObject, int nCount);
	typedef std::map<std::string,KUiItemdObject> _ItemData;                 //SkillId,Echance
	_ItemData m_ItemData;

	//int   m_CurSelItemIndex;
	void  closeItemPadCallback(Node *pNode);
	void  closeItemPadCallback1(Node *pNode);
	void  UpdateData();
	std::string  nstrCurSelItemKey;
	float m_MovePosX;
	bool  m_nIstrue;
	bool  m_savemoneyPadIsOpen;
	unsigned int nBakChickTime;
	bool m_bScrolling;
	Point m_lastPoint;
	bool __isCanMove;
};

#endif /* defined(__MyGame__PopLayer__) */
