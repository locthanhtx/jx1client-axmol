//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuistoreBox__
#define __KuistoreBox__

#include <iostream>
#include "cocos2d.h"

#include <map>
USING_NS_AX;


//#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
//#define MAX_ITEM_COUNT  60

class KuistoreBox:public Layer{
public:
	KuistoreBox();
	~KuistoreBox();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuistoreBox);

	static KuistoreBox * create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();
	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd);//��Ʒ�仯
	void	UpdateExItem(KUiObjAtRegion* pItem, int bAdd,int exBoxIndex);
	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
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
	void getmoneyCallBackFunc(Ref * pSender);
	void buttonCallBackFunc(Ref * pSender);
	void openboxCallBackFunc(Ref * pSender);

	void closeItemKuoPadCallback(Node *pNode);

	void zenliCallBackFunc(Ref * pSender);
	void diuCallBackFunc(Ref * pSender);
	void chuCallBackFunc(Ref * pSender);
	void ziCallBackFunc(Ref * pSender);
	void kuo1CallBackFunc(Ref * pSender);
	void kuo2CallBackFunc(Ref * pSender);
	void kuo3CallBackFunc(Ref * pSender);

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
	Label* pMoneyLabel;
	//Label* pXuLabel;
	LayerColor * colorLayer;
	//Action* red;
	//Menu * savemenu;

	bool m_descPadIsOpen;
	bool m_getmoneyPadIsOpen;
	int  m_nMoney;
	//int  m_nXu;
	int  m_nNumObjects;
	POINT m_StartPos;//���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	//KuistoreBoxdObject m_ItemData[MAX_ITEM_COUNT];//KuistoreBoxObject
	int AddObject(KUiDraggedObject* pObject, int nCount);
	typedef std::map<std::string,KUiItemdObject> _ItemData;                 //SkillId,Echance
	_ItemData m_ItemData;
	Sprite *btnBackPic;
	//int   m_CurSelItemIndex;
	void  closeItemPadCallback(Node *pNode);
	void  closegetmonyPadCallback(Node *pNode);
	void  UpdateData();
	std::string  nstrCurSelItemKey;
	float m_MovePosX;
	unsigned int nBakChickTime;
	bool m_bScrolling;
	Point m_lastPoint;
	bool _m_itemExPadIsOpen;
	Layer * sItemExLayer;

};

#endif /* defined(__MyGame__PopLayer__) */
