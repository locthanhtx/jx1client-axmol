//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiTalkDialog1__
#define __KuiTalkDialog1__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_AX;
USING_NS_CC_EXT;
using namespace ui;
//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiTalkDialog1:public Layer{
public:
	KuiTalkDialog1();
	~KuiTalkDialog1();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiTalkDialog1);

	static KuiTalkDialog1 * create(char * pContent, char *OkbtnStr,char *NobtnStr,int nKind,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun,unsigned int nParam=0,char *strParam=NULL);

	//���Ӽ���ͼ��
	void addDialogData(char * pContent,int nKind);
	void setcoloseButton(char *OkbtnStr,char *NobtnStr,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	int  getDialogKind(){return __Kind ;};
	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
private:
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);
	void touchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);

	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
    std::function<void(ax::Node*)> m_callfun;

	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	//�Ի��򱳾�����
	Sprite *ParentNode_ItemEx;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	//Label* pMoneyLabel;
	//Label* pXuLabel;
	LayerColor * colorLayer;
	Action* red;
	Layer *m_pUiLayer;
	int  __Kind;
	unsigned int  __nParam;
	char __strParam[32];

};

#endif /* defined(__MyGame__PopLayer__) */
