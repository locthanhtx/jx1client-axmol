//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiSortSkill_l__
#define __KuiSortSkill_l__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_AX;
USING_NS_CC_EXT;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define	SKILLTREE_MAX_SKILL_COUNT		65
#define	SKILLTREE_SHORTCUT_SKILL_COUNT	9

class KuiSortSkill_l:public Layer{
public:
	KuiSortSkill_l();
	~KuiSortSkill_l();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiSortSkill_l);

	static KuiSortSkill_l * create(int nKind,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData(int nKind);
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

	void UpdateData();
	//�ص�����
	std::function<void(ax::Node*)> m_callfun;

	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);

	void skillCallback(Ref* pSender);
	void closeSkilldescPadCallback(Node *pNode);
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	//�Ի��򱳾�����
	Sprite * m_bgSprite;
	LayerColor * ParentNode_ItemEx;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	//Label* pMoneyLabel;
	//Label* pXuLabel;
	LayerColor * colorLayer;
	Action* red;

	int				m_nNumSkills;
	KUiSkillData	m_Skills[SKILLTREE_MAX_SKILL_COUNT];
	//KUiSkillData::nLevel�ڴ˵ĺ����ʾλ�˼�����ʾ�ڵڼ���
	KUiSkillData	ms_ShortcutSkills[SKILLTREE_SHORTCUT_SKILL_COUNT];
	//KUiSkillData::nLevel�ڴ˵ĺ���0��ʾ���ּ��ܣ�1��ʾ���ּ���
	int   _sortCount_r;

};

#endif /* defined(__MyGame__PopLayer__) */
