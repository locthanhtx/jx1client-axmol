//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiRoleState__
#define __KuiRoleState__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;


#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	74   //ÿ�����ӵĸ߶ȺͿ��
#define ITEM_CELL_SIZE_SMALL	55   //ÿ�����ӵĸ߶ȺͿ��
#define _ITEM_COUNT 15
class KuiRoleState:public Layer{
public:
	KuiRoleState();
	~KuiRoleState();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiRoleState);

	static KuiRoleState * create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
	void    setCanMove(bool ver){__isCanMove =ver;};
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);
	void ccTouchMoved(Touch *pTouch, Event *pEvent);
	void ccTouchEnded(Touch *pTouch, Event *pEvent);

	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);

	void buttonEqCallBackFunc(Ref * pSender);
	void buttonxiuCallBackFunc(Ref * pSender);
	void addCallBackFunc(Ref * pSender);

	//�ϲ����
	Ref * m_callbackListener;
	KTabFile nVipInfo;
	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
	//�Ի��򱳾���С
	Point m_origin;
	//Point diff;
	Size m_size;
	Size winSize;
	//�Ի��򱳾�����
	Sprite * m_bgSprite;
	Sprite *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	void closedescPadCallback(Node *pNode);

	Label* pRoleName;
	Label* pRoleTitle;
	Label* pRoleTitle_1;
	Label* pRoleVip;
	Label* pFuYuan;
	Label* pShenWang;
	Label* proelLevel;
	Label* pPaiMing;

	Label* pLife;
	Label* pMana;
	Label* pTili;

	Label* pSex;
	Label* pExp;
	Label* pLeftAct;
	Label* pRigthAct;

	Label* pMingZhong;
	Label* pShanBi;
	Label* pRunSpeed;
	Label* pActpeed;
	Label* pMicCount;

	Label* pLiLiang;
	Label* pWauGong;
	Label* pNeiGong;
	Label* pShenFa;

	Label* pPuFang;
	Label* pBingFang;
	Label* pLeiFang;
	Label* pHuoFang;
	Label* pDuFang;

	Label* pLucky;
	Label* pPk;

	Label* pZhuanSheng;
	Label* pZhuanShengLevel;

	Label* pTemDescItem;

	EditBox * liEditBox;
	EditBox * waiEditBox;
	EditBox * shenEditBox;
	EditBox * neiEditBox;
	LayerColor * colorLayer;
	Action* red;
	Node * ItemBoxNode;
	int  m_nMoney;
	int  m_nXu;
	int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	void UpdateAllEquips();
	void UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);
	void addpicBox(int index,int nItemIndx,bool isAdd);
	bool m_descPadIsOpen;
	int  m_itemIndexBox[_ITEM_COUNT];
	int  m_CurSelItemIndex;
	void UpdateBaseData();
	void BreatheUpdateInfo();
	void BreathAttributeUpdate();
	int  __RemainPoint;
	bool m_bScrolling;
	Point m_lastPoint;
	unsigned int nBakChickTime;
	bool __isCanMove;
	Sprite * m_ExeProcessSprite;
	int _VipLevel;
	bool nIsOpenTxt;
};

#endif /* defined(__MyGame__PopLayer__) */
