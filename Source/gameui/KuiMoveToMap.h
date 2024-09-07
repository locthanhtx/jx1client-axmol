//  ɽ����ͼ����
//  KuiMoveToMap.h
//  KuiMoveToMap
//
#ifndef __KuiMoveToMap__
#define __KuiMoveToMap__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;

using namespace ui;
#include "Kuiplayerfun.h"
//#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define MAX_COUNT_MAPS	120
struct mapListData
{
	Button* m_MoveMapBtn;
	int       m_SubwordId;
	int       m_PosX;
	int       m_PosY;
};

class KuiMoveToMap:public Layer{
public:
	KuiMoveToMap(const std::function<void(ax::Node*)>& callfun = NULL);
	~KuiMoveToMap();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiMoveToMap);
	static KuiMoveToMap * create(const char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	virtual void update(float delta);
//	virtual void draw();
	bool    isOpen;
	//void _setPlayerFun(Kuiplayerfun * ver){nPlayerFun=ver;};
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

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

	LayerColor *ParentNode_Team;
	Rect getRect(Node* pNode,int i);

	Action* red;
	Layer *m_pUiLayer;
	Size bgSize;
	Size nTextureSize;
	Sprite *nMapPicSpr;
	bool m_bScrolling;
	Point m_lastPoint;
	ClippingNode *clipper;
	Sprite *content_Map;
	//int    msgCount;
	int m_PosCount;
	mapListData m_MoveMapBtn[MAX_COUNT_MAPS];
};

#endif /* defined(__MyGame__PopLayer__) */
