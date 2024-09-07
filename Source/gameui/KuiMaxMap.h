//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiMaxMap__
#define __KuiMaxMap__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;

using namespace ui;
#include "Kuiplayerfun.h"
//#define FIGHT_SKILL_COUNT_PER_PAGE	25

class KuiMaxMap:public Layer{
public:
	KuiMaxMap();
	~KuiMaxMap();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiMaxMap);

	static KuiMaxMap* create(const char* title,
                                 Ref* callbackListener,
                                 const std::function<void(ax::Node*)>& callfun);
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	virtual void update(float delta);
//	virtual void draw();
	bool    isOpen;
	//void _setPlayerFun(Kuiplayerfun * ver){nPlayerFun=ver;};
private:
	void oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();
	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch *pTouch, Event * pevent);
	void ccTouchMoved(Touch *pTouch, Event *pEvent);
	void ccTouchEnded(Touch *pTouch, Event *pEvent);

	virtual void ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent);

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
	Size winSize;
	//�Ի��򱳾�����
	//Sprite * m_bgSprite;
	LayerColor *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	//Label* pMoneyLabel;
	//Label* pXuLabel;
	//LayerColor * colorLayer;
	Action* red;

	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	//POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	Layer *m_pUiLayer;
	EditBox *pstrEditBox_d;
	//Layout  *m_pWidget;
	//CCArray* m_array;
	//ListView* listView;
	//Size btnSize;
	Size bgSize;
	Size nTextureSize;
	Sprite *nMapPicSpr;
	bool m_bScrolling;
    float nRoleDisX;
    float nRoleDisY;
	Point m_lastPoint;
	ClippingNode *clipper;
	Sprite *nPartNode;
	Sprite *BtnFlag;
	Sprite *content_Map;
	unsigned int __loopTime;
	Label *pPointLabel;
	//int    msgCount;

};

#endif /* defined(__MyGame__PopLayer__) */
