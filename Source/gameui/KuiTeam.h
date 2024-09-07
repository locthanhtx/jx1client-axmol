//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiTeam__
#define __KuiTeam__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;

using namespace ui;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiTeam:public Layer{
public:
	KuiTeam();
	~KuiTeam();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiTeam);

	static KuiTeam* create(const char* title, Ref* callbackListener, const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
	void	UpdateData(KUiPlayerTeam* pInfo);
	void    setCanMove(bool ver){__isCanMove =ver;};
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	void selectedItemEvent(Ref* pSender, ListView::EventType type);
	void selectedPlayersListItemEvent(Ref* pSender, ListView::EventType type);
	void selectedBoxStateEvent(Ref* pSender, CheckBox::EventType type);
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
	void btnCallBackFunc(Ref * pSender);

	void Clear();
	void OnRefresh();					//ˢ��
	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	Size winSize;
	//�Ի��򱳾�����
	//Sprite * m_bgSprite;
	Sprite *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	Label* pMoneyLabel;
	//Label* pXLabel;
	LayerColor * colorLayer;
	Action* red;

	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	//POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	Layer *m_pUiLayer;
	//Layout  *m_pWidget;
	//CCArray* m_array;
	ListView* pNearbyPlayerslistView;
	ListView* pPlayersListView;
	CheckBox* checkBox;
	Size btnSize;
	KUiPlayerTeam	m_Info;
	KUiPlayerItem*	m_pPlayersList;
	KUiPlayerItem*	m_pNearbyPlayersList;
	int   __nSelIndex;
	int   __playrSelIndex;
	int   __playerListCount;
	int   __NearbyListCount;
	bool m_bScrolling;
	Point m_lastPoint;
	bool __isCanMove;
};

#endif /* defined(__MyGame__PopLayer__) */
