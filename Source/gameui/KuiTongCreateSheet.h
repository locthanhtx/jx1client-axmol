//
//  KuiTongCreateSheet.h
//  KuiTongCreateSheet
//
//  Created by �ᵶ����Ц QQ:25557432.
//
//
#ifndef __KuiTongCreateSheet__
#define __KuiTongCreateSheet__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;

using namespace ui;
class KuiTongCreateSheet:public Layer{
public:
	KuiTongCreateSheet();
	~KuiTongCreateSheet();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiTongCreateSheet);

	static KuiTongCreateSheet * create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

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
	void btnCallBackFunc(Ref * pSender);


	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	Size winSize;
	//�Ի��򱳾�����
	//Sprite * m_bgSprite;
	Sprite *ParentNode_Task;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	Label* pTongNameLabel;
	Label* pCampTitleLabel;
	Label* pCampInfoLabel;
	LayerColor * colorLayer;
	Action* red;
	Layer *m_pMainUiLayer;
	EditBox *pNameEditBox;
	Button* btnBox_zp;
	Button* btnBox_zl;
	Button* btnBox_xp;
	Size btnSize;
	bool m_bScrolling;
	Point m_lastPoint;
	bool __isCanMove;
    int   __nSelCamp;
};

#endif /* defined(__KuiTongCreateSheet__) */
