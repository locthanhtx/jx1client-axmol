//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiDialog__
#define __KuiDialog__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;


//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiDialog:public Layer{
public:
	KuiDialog();
	~KuiDialog();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiDialog);

	static KuiDialog * create(KUiQuestionAndAnswer* pContent, KUiNpcSpr* pImage,int nIsTimeClose,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	//���Ӽ���ͼ��
	void addDialogData(KUiQuestionAndAnswer* pContent, KUiNpcSpr* pImage,int nIsTimeClose);

	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
private:
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

	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	//�Ի��򱳾�����
	Sprite * m_bgSprite;
	Sprite *ParentNode_ItemEx;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	//Label* pMoneyLabel;
	//Label* pXuLabel;
	LayerColor * colorLayer;
	Action* red;

	//int  m_nMoney;
	//int  m_nXu;
	int  m_AnswerCount;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)

};

#endif /* defined(__MyGame__PopLayer__) */
