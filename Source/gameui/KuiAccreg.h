//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiAccreg__
#define __KuiAccreg__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_AX;
USING_NS_CC_EXT;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define TIME_OUT     5           //���峬ʱʱ��
class KuiAccreg: public Layer{
public:
	KuiAccreg();
	~KuiAccreg();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiAccreg);

	static KuiAccreg * create(Ref * callbackListener, const std::function<void(ax::Node*)>& callfun);
	virtual void update(float delta);
//	virtual void draw();

	virtual void editBoxEditingDidBegin(EditBox *editBox); //��ʼʱ�ĺ���
	virtual void editBoxEditingDidEnd(EditBox *editBox);   //����ʱ�ĺ���
	virtual void editBoxReturn(EditBox *editBox);          //����������س�ʱ�Ĵ���
	virtual void editBoxTextChanged(EditBox *editBox, const std::string &text); //���ָ��ĺ���

	bool    isOpen;
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(Ref * callbackListener, const std::function<void(ax::Node*)>& callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch, Event * pevent);

	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);

	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
    std::function<void(ax::Node*)> m_callfun;

	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);
	void mianLoginCallback(Ref* pSender);
	void mianCancelCallback(Ref* pSender);
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

	void colseSockets();
	int  m_AnswerCount;
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	EditBox * AccNode;
	EditBox * PasNode;
	EditBox * ePasNode;
	SOCKET s;
	unsigned int nport;
	std::string  naccIp;
	std::string m_WritablePath;

};

#endif /* defined(__MyGame__PopLayer__) */
