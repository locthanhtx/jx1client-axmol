//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiChatList__
#define __KuiChatList__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_AX;
USING_NS_CC_EXT;
using namespace ui;
#include "Kuiplayerfun.h"
//#define FIGHT_SKILL_COUNT_PER_PAGE	25

class KuiChatList:public Layer{
public:
	KuiChatList();
	~KuiChatList();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiChatList);

	static KuiChatList * create(Ref * callbackListener,std::function<void(ax::Node*)> callfun);
	//���Ӽ���ͼ��
	void addDialogData();
	void addmsg(char * sendName,char * contentt,int nKind=-1,int nColor=0,int nPackage=-1);
	int	 NewChannelMessageArrival(DWORD nChannelID, char* szSendName,char* pMsgBuff, unsigned short nMsgLength,DWORD nItemDwidx=0,char *scrSendName=NULL,int nPackage=-1);
	void setcoloseButton(Ref * callbackListener,std::function<void(ax::Node*)> callfun);

	virtual void update(float delta);
//	virtual void draw();
	void _removeDelegate(bool isThis);
	bool    isOpen;
	void _setPlayerFun(Kuiplayerfun * ver){nPlayerFun=ver;};
private:
	Kuiplayerfun *nPlayerFun;
	void selectedItemEvent(Ref* pSender, ListView::EventType type);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();
	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);

	void buttonCallBackFunc(Ref * pSender);
	//�رյ�����
	void closePopLayer(Ref * pSender);

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
	LayerColor  *ParentNode_Team;
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
	//Layout  *m_pWidget;
	//CCArray* m_array;
	ListView* listView;
	Size btnSize;
	Size bgSize;
	//int    msgCount;

};

#endif /* defined(__MyGame__PopLayer__) */
