//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuigetMoney__
#define __KuigetMoney__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;


//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuigetMoney:public Layer{
public:
	KuigetMoney();
	~KuigetMoney();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuigetMoney); //��ʼ��

	static KuigetMoney * create(int nMoney,int nMaxMoney,int nKind,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
	void setConfirmCancelButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	void editBoxEditingDidBegin(EditBox *editBox); //��ʼʱ�ĺ���
	void editBoxEditingDidEnd(EditBox *editBox); //����ʱ�ĺ���
	void editBoxReturn(EditBox *editBox); //����������س�ʱ�Ĵ���
	void editBoxTextChanged(EditBox *editBox, const std::string &text); //���ָ��ĺ���
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);

	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);
	void getmoneyCallBackFunc(Ref * pSender);

	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
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
	//Action* red;

	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	EditBox *pMoneyEditBox;
    int  nKind;
	unsigned int seveMoney;
};

#endif /* defined(__MyGame__PopLayer__) */
