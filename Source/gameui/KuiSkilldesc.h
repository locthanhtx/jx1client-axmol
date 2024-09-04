//
//  PopupLayer.h
//  PopupDemo
//  ��Ʒ˵����
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiSkilldesc__
#define __KuiSkilldesc__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;


class KuiSkilldesc:public Layer{
public:
	KuiSkilldesc();
	~KuiSkilldesc();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiSkilldesc);

	static KuiSkilldesc * create(int nskillId,int nLevel,KUiItemdObject nKillData,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun,int nKind=0);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

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
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	//�Ի��򱳾�����
	Sprite * m_bgSprite;
	LayerColor  *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	Label* ptitleLabel;//����
	Label* potherLabel;//������Ҫ����ɫ���õ�
	Node* pdescLabel;     //��Ҫ������ɫ��
	Label* pbaseLabel; //��������
	Label* pScriptLabel; //�ű�˵��
	Label* pScriptLabel_n;
	Node* prqLabel;       //��������
	Node* pmagicLabel;//ħ������
	LayerColor * colorLayer;
	Action* red;

	//int  m_ItemIndex;
	KUiItemdObject skillData;
	int _nKind;
	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)

};

#endif /* defined(__MyGame__PopLayer__) */
