//
//  PopupLayer.h
//  PopupDemo
//  ��Ʒ˵����
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiItemdesc__
#define __KuiItemdesc__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_AX;
USING_NS_CC_EXT;
using namespace ui;
#define	BUY_SELL_SCALE_DE	1
class KuiItemdesc:public Layer{
public:
	KuiItemdesc();
	~KuiItemdesc();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiItemdesc);

	static KuiItemdesc * create(KUiItemdObject nItemData,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun,int nShowKind);
	static KuiItemdesc * create(int nshopIndex,KUiItemdObject nItemData,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void addshopdesc(int nshopIndex);
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	void setcoloseButtonInfo(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);

	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);
	void chaiCallBackFunc(Ref * pSender);
	void kuaiCallBackFunc(Ref * pSender);
	void buttonSuDungFunc(Ref * pSender);
	void buttonThaoFunc(Ref * pSender);
	void buttonSellItemFunc(Ref * pSender);
	void buttonVutBoFunc(Ref * pSender);
	void buttonSendChatFunc(Ref * pSender);
	void buttonDinhFunc(Ref * pSender);
	void buttonDinhGiaFunc(Ref * pSender);
	//�ϲ����
	Ref * m_callbackListener;
	typedef enum
	{
		LAYOUT_ABSOLUTE,
		LAYOUT_LINEAR_VERTICAL,
		LAYOUT_LINEAR_HORIZONTAL,
		LAYOUT_RELATIVE
	}LayoutType;
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
	Label* potherLabel;//khong co mau
	Node* pdescLabel;     //can set mau
	Node* pnameLabel;     //can set mau
	Label* pbaseLabel; //thuoc tinh co ban
	Node* prqLabel;   //thuoc tinh yeu cau
	Node* sprItem;   //thuoc tinh yeu cau
	Node* pmagicLabel;//thuoc tinh phep thuat
	LayerColor * colorLayer;
	// ListView* listView;
	// Layer *m_pUiLayer;
	Layer *m_pUiLayer;
	ListView* listView;
	Action* red;
	unsigned int nBakChickTime;
	//int  m_ItemIndex;
	KUiItemdObject m_ItemData;
	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	int   m_ShowKind;

};

#endif /* defined(__MyGame__PopLayer__) */
