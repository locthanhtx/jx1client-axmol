//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiItemEx__
#define __KuiItemEx__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;


#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuiItemEx:public Layer{
public:
	KuiItemEx();
	~KuiItemEx();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiItemEx);

	static KuiItemEx * create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
	void    UpdateData();
	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd);//��Ʒ�仯
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
	void qiCallBackFunc(Ref * pSender);
	void quCallBackFunc(Ref * pSender);
	void zenliCallBackFunc(Ref * pSender);
	void closeItemPadCallback(Node *pNode);
	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	Size winSize;
	//�Ի��򱳾�����
	Sprite * m_bgSprite;
	Sprite *ParentNode_ItemEx;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	//Label* pMoneyLabel;
	//Label* pXuLabel;
	LayerColor * colorLayer;
	//Action* red;

	typedef std::map<std::string,KUiItemdObject> _ItemData;                 //SkillId,Echance
	_ItemData m_ItemData;

	int  m_nIstrue;
	int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	std::string nstrCurSelItemKey;
	bool m_descPadIsOpen;
	int AddObject(KUiDraggedObject* pObject, int nCount);
	Sprite *btnBackPic;
	unsigned int nBakChickTime;
	bool m_bScrolling;
	Point m_lastPoint;
	bool __isCanMove;
};

#endif /* defined(__MyGame__PopLayer__) */
