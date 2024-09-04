//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiLockscree__
#define __KuiLockscree__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_AX;
USING_NS_CC_EXT;
using namespace ui;
class KuiLockscree:public Layer{
public:
	KuiLockscree();
	~KuiLockscree();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiLockscree);

	static KuiLockscree * create(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
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
	void tuoguanCallBackFunc(Ref * pSender);
	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);

	void oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);

	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;

	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	//�Ի��򱳾�����
	LayerColor *ParentNode_ItemEx;
	Layer *m_pUiLayer;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	//Label* pMoneyLabel;
	//Label* pXuLabel;
	LayerColor * colorLayer;
	Action* red;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	std::string m_WritablePath;

};

#endif /* defined(__MyGame__PopLayer__) */
