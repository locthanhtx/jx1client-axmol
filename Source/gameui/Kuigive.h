//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __Kuigive__
#define __Kuigive__
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define MAX_ITEM_COUNT  60
#define MAX_GIVE_COUNT  24
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
USING_NS_AX;
USING_NS_CC_EXT;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class Kuigive:public Layer{
public:
	Kuigive();
	~Kuigive();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(Kuigive);

	static Kuigive * create(char * strTitel,char *pContent,int nNum,char* pszCallback,char* pszError,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	//char* pszTitle, char* pszInitString,const int nNum,const char* pszCallback,const char* pszError

	void	UpdateItem(KUiObjAtRegion* pItem, int bAdd);//��Ʒ�仯
	void	UpdateGiveItem(KUiObjAtRegion* pItem, int bAdd);
	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData(char * strTitel,char *pContent);
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);

	void  closeItemPadCallback(Node *pNode);
	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);
	void btnCallBackFunc_fangzhi(Ref * pSender);
	void btnCallBackFunc_quxia(Ref * pSender);
	void okCallBackFunc(Ref * pSender);
	void zhengCallBackFunc(Ref * pSender);

	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
    std::function<void(ax::Node*)> m_callfun;

	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);
	typedef std::map<std::string,KUiItemdObject> _ItemData;                 //SkillId,Echance
	_ItemData m_ItemData;

	typedef std::map<std::string,KUiItemdObject> _giveItemData;                 //SkillId,Echance
	_giveItemData m_giveItemData;

	bool  m_nIstrue;
	int AddObject(KUiDraggedObject* pObject, int nCount);
	int AddGiveObject(KUiDraggedObject* pObject, int nindex);
	void  UpdateItemData();
	void  UpdateGiveItemData();
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	Size winSize;
	Size itemBoxSize;
	Size giveBoxSize;
	//�Ի��򱳾�����
	Sprite * m_bgSprite;
	Sprite *ParentNode_ItemEx;

	Node *ParentNode_Item;
	Node *ParentNode_Give;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRectItem(Node* pNode,int i);
	Rect getGiveItem(Node* pNode,int i);
	Label* pMoneyLabel;
	Label* pXuLabel;
	LayerColor * colorLayer;
	//Action* red;
	Sprite *btnBackPic;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	POINT m_StartPos_g; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	int  m_nItemNumObjects;
	int  m_ngiveItemNumObjects;
	std::string nstrCurSelItemKey;
	std::string inCurGiveItemKey;
	bool m_descPadIsOpen;
	int m_itemNum,m_giveNum,m_nNeed;
	char   m_Callback[64];
	char   m_Error[16];
	unsigned int nBakChickTime;
	int  m_nMoney;
	int  m_nXu;
};

#endif /* defined(__MyGame__PopLayer__) */
