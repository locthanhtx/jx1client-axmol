//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiCometoMap__
#define __KuiCometoMap__

#include <iostream>
#include "cocos2d.h"

#include "gameui/KuiShowSprite.h"
USING_NS_AX;

using namespace ui;
//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
#define _MAPMAX_COUNT 20
class KuiCometoMap:public Layer{
public:
	KuiCometoMap();
	~KuiCometoMap();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiCometoMap);

	static KuiCometoMap * create(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	//���Ӽ���ͼ��
	void addDialogData();
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
	void  closeShowPad(Node *pNode);
	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);
	void movemapCallBack(Ref* menuItem);
	//�ϲ����
	Ref * m_callbackListener;

	void UpdateData();
	//�ص�����
	std::function<void(ax::Node*)> m_callfun;

	void touchPageBtnEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
	void touchAwardBtnEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);
	void setBtnTouchEnble(int index,bool enble);
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
	POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	int  m_MaxCount;
	int  m_MapCount;
	Label *m_recommend[_MAPMAX_COUNT];
	Label *m_TimeInfo;;
	Layer *m_pMainUiLayer;
	Layout  *m_pWidget;

	Button* btnPageBox_1;//��ҳ��ť
	Button* btnPageBox_2;
	Button* btnPageBox_3;
	Button* btnPageBox_4;
	Button* btnPageBox_5;

	Layer * _PageLayer_1;//��ҳ���
	Layer * _PageLayer_2;
	Layer * _PageLayer_3;
	Layer * _PageLayer_4;
	Layer * _PageLayer_5;

	Button* m_AwardButton_yl;//����/����
	Button* m_AwardButton_fs;//����/��Ե
	Button* m_AwardButton_jb;//���
	Button* m_AwardButton_jf;//����

	Sprite * m_timeProcessSprite;
	KuiShowSprite * nShowLayer;

	int _nCurTime; //(��λ������)
	int _nMoneyExpTime; //��������� 0
	int _nFuYuanTime;   //��Ե������ 1
	int _nJiFenTime;    //����       3
	int _nJinBiTime;    //���       2

	int _curPage;
};

#endif /* defined(__MyGame__PopLayer__) */
