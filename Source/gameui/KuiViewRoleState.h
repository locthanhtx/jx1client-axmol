//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiViewRoleState__
#define __KuiViewRoleState__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_AX;
USING_NS_CC_EXT;

#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	27   //ÿ�����ӵĸ߶ȺͿ��
#define _ITEM_COUNT 15
class KuiViewRoleState:public Layer{
public:
	KuiViewRoleState();
	~KuiViewRoleState();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiViewRoleState);

	static KuiViewRoleState * create(KUiPlayerItem* pDest,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	bool    isOpen;
private:
	virtual void update(float delta);
//	virtual void draw();

	void  UpdateData(KUiPlayerItem* pDest);
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);

	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);

	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	//�Ի��򱳾�����
	Sprite * m_bgSprite;
	Sprite *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	void closedescPadCallback(Node *pNode);

	Label* pRoleName;
	Label* pRoleTitle;
	//Label* pRoleTitle_1;

	Label* pPk;

	Label* pZhuanSheng;
	Label* pZhuanShengLevel;

	LayerColor * colorLayer;
	Action* red;
	Node * ItemBoxNode;
	int  m_nMoney;
	int  m_nXu;
	int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	void UpdateAllEquips();
	void UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);
	void addpicBox(int index,int nItemIndx,bool isAdd);
	bool m_descPadIsOpen;
	int  m_itemIndexBox[_ITEM_COUNT];
	int  m_CurSelItemIndex;
	void UpdateBaseData(KUiPlayerItem* pDest);
	void BreatheUpdateInfo();
	void BreathAttributeUpdate();
	int  __RemainPoint;
	KUiPlayerItem* ncurDest;

};

#endif /* defined(__MyGame__PopLayer__) */
