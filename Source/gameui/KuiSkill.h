//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __Kuiskill__
#define __Kuiskill__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/UIPageView.h"

USING_NS_AX;
USING_NS_CC_EXT;

#define LIVE_SKILL_COUNT			10
#define	FIGHT_SKILL_COUNT			50
#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define FIGHT_SKILL_PAGE_1	0
#define FIGHT_SKILL_PAGE_2	1
#define FIGHT_SKILL_PAGE_3	2
#define FIGHT_SKILL_PAGE_4	3
#define FIGHT_SKILL_PAGE_5	4
#define	FIGHT_SKILL_SUB_PAGE_COUNT	FIGHT_SKILL_COUNT / FIGHT_SKILL_COUNT_PER_PAGE

class Kuiskill:public Layer{
public:
	Kuiskill();
	~Kuiskill();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(Kuiskill);

	static Kuiskill * create(int m_Kind,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	void touchEvent(Ref* pSender, ax::ui::AbstractCheckButton::TouchEventType type);
    void scrollEvent(Ref* pSender, ScriptEventType type);
	void pageViewEvent(Ref* pSender, ax::ui::PageView::EventType type, Touch *pTouch);
	void UpdateSkill(KUiSkillData* pSkill, int nIndex);
	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);
	void oktouchEvent(Ref *pSender, ax::ui::Widget::TouchEventType type);
	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);

	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
	void   addskillpoint(Ref* pSender);
	void   toprePage(Ref* pSender);
	void   tonextPage(Ref* pSender);

	void  closeDescCallback(Node *pNode);
	void  closeSkilldescPadCallback(Node *pNode);

	int   getfreeSkillNode(int skillidx);
	Texture2D * GetiniData(char *sprPath,int nFramIndex,int nMpsX,int nMpsY,int nMpsZ,int *mScreeX,int *mScreeY,int bRenderFlag=0,int bSinglePlaneCoord=0);
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	//�Ի��򱳾�����
	Sprite * m_bgSprite;
	Layer *m_pUiLayer;
    Layout *m_pWidget;
	ax::ui::PageView* pageView;
	Node * ItemNode;
	ax::ui::PageView* pageViewTemp;
	Sprite *ParentNode_Fight;
	Node *_pageLayout1;
	Node *_pageLayout2;
	Node *_pageLayout3;
	Node *_pageLayout4;
	Node *_pageLayout5;
	KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	int nRemainSkillPoint;
	Label* pSkillPointLabel;
	Label* pSkillInfoLabel;
	LayerColor * colorLayer;
	Action* red;
	int  nSkillCount;
	int  nCurSelindex;
};

#endif /* defined(__MyGame__PopLayer__) */
