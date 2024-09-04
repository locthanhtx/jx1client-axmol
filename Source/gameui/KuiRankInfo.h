//
//  KuiRankInfo.h
//  KuiRankInfo
//
//  Created by �ᵶ����Ц QQ:25557432.
//
//
#ifndef __KuiRankInfo__
#define __KuiRankInfo__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Engine/LinkStruct.h"
USING_NS_AX;
USING_NS_CC_EXT;
using namespace ui;

#define MAX_TASK_MESSAGE_LENGTH 128
#define MAX_TASK_MESSAGE_MUM 512

struct KTaskIndexNode// : public KListIndex
{
	char szMsg[MAX_TASK_MESSAGE_LENGTH]; //����
	int  nMsgLen;		   //����
	char szUnit[16];	   //��λ
	int nTaskId;
	int nType;
	int nClass;
	int nTrace;
	int nIndex;
	int nCount;
	KTaskIndexNode *next;
	KTaskIndexNode *previous;
};


class KuiRankInfo:public Layer{
public:
	KuiRankInfo();
	~KuiRankInfo();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiRankInfo);

	static KuiRankInfo * create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

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

	void selectedMainListItemEvent(Ref* pSender, ListView::EventType type);
	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);
	void ccTouchMoved(Touch *pTouch, Event *pEvent);
	void ccTouchEnded(Touch *pTouch, Event *pEvent);
	//�رյ�����
	void closePopLayer(Ref * pSender);
	void oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
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
	Sprite *ParentNode_Task;
	Rect getRect(Node* pNode,int i);
	void removeitems();
	void AddTaskIndex();
	LayerColor * colorLayer;
	Action* red;
	Layer *m_pMainUiLayer;
	ListView* pmainListView;
	Size btnSize;
	bool m_bScrolling;
	Point m_lastPoint;
	bool __isCanMove;
	Size btnNewSize;
	Size btnCurSize;
	int  __nSelIndex;
	int  __nCount;
	KIniFile  m_IndexIDIni;
	KLinkStruct<KTaskIndexNode> m_IndexData; //�����������ݵ�����
};

#endif /* defined(__KuiRankInfo__) */
