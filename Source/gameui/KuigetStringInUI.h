//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuigetStringInUI__
#define __KuigetStringInUI__

#include <iostream>
#include "cocos2d.h"


USING_NS_AX;

using namespace ui;
#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��

class KuigetStringInUI:public Layer, public EditBoxDelegate
{
public:
	KuigetStringInUI();
	~KuigetStringInUI();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuigetStringInUI);
        static KuigetStringInUI* create(const char* pszTitle,
                                        int nMinLen,
                                        int nMaxLen,
                                        const char* pszCallback,
                                        const char* pszError,
                                        int bNum,
                                        Ref* callbackListener,
                                        const std::function<void(ax::Node*)>& callfun,
                                        int outKind = 0);

	virtual void editBoxEditingDidBegin(EditBox* editBox);
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(EditBox* editBox);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
private:
	//���Ӽ���ͼ��
    void addDialogData(const char* pszTitle);
	//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	/*void editBoxEditingDidBegin(EditBox *editBox); //��ʼʱ�ĺ���
	void editBoxEditingDidEnd(EditBox *editBox); //����ʱ�ĺ���
	void editBoxReturn(EditBox *editBox); //����������س�ʱ�Ĵ���
	void editBoxTextChanged(EditBox *editBox, const std::string &text); //���ָ��ĺ���
	*/
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);

	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);
	void getstringgCallBackFunc(Ref * pSender);
	void _getstringgCallBackFunc(Ref * pSender);

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
	Label* ptitleLabel;
	//Label* pXuLabel;
	LayerColor * colorLayer;
	Action* red;

	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	EditBox *pMoneyEditBox;
	//char strTitle[64];
	char strCallBackfun[64];
	char strErrorfun[16];
	int  minMsgLen;
	int  maxMsgLen;
	int  inPutKind;
	int  outPutKind;
};

#endif /* defined(__MyGame__PopLayer__) */
