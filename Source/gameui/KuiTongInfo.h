//
//  KuiTongInfo.h
//  KuiTongInfo
//
//  Created by �ᵶ����Ц QQ:25557432.
//
//
#ifndef __KuiTongInfo__
#define __KuiTongInfo__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;

using namespace ui;
class KuiTongInfo:public Layer{
public:
	KuiTongInfo();
	~KuiTongInfo();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiTongInfo);

	static KuiTongInfo * create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
	void    setCanMove(bool ver){__isCanMove =ver;};
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
	void oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);
	void btnCallBackFunc(Ref * pSender);


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
	Sprite *ParentNode_Task;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	//Label* pMoneyLabel;
	//Label* pXLabel;
	LayerColor * colorLayer;
	Action* red;
	Layer *m_pMainUiLayer;
	//Layout  *m_pWidget;
	//CCArray* m_array;
	//ListView* ptaskMsglistView;
	//ListView* pmainListView;
	//CheckBox* checkBox;
	Size btnSize;
	//KUiPlayerTeam	m_Info;
	bool m_bScrolling;
	Point m_lastPoint;
	bool __isCanMove;

    int   __nSelIndex;
};

#endif /* defined(__KuiTongInfo__) */
