//
//  KuiTaskInfo.h
//  KuiTaskInfo
//
//  Created by �ᵶ����Ц QQ:25557432.
//
//
#ifndef __KuiTaskInfo__
#define __KuiTaskInfo__

#include <iostream>
#include "cocos2d.h"

#include "Engine/LinkStruct.h"
#include <map>
USING_NS_AX;

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

struct	nNewTaskFindPath
{
	int nTsakType;	    //����
	int	nMsgidx;		//������Ǯ����ֵΪ���۸񣬸�ֵ��ʾ����ļ۸�Ϊ(-nPrice)
	int	nMapIdx;
	int nXpos;
	int nYpos;
};

class KuiTaskInfo:public Layer{
public:
	KuiTaskInfo();
	~KuiTaskInfo();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiTaskInfo);

	static KuiTaskInfo* create(const char* title,
                                   Ref* callbackListener,
                                   const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
	void    setCanMove(bool ver){__isCanMove =ver;};
	void    addmsgInfo(char *nMsgInfo,int nIsBtn,int nMsgIndex);
	void    addmsgBtnInfo(KTaskPathInfo* pMsg);
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	void selectedMainListItemEvent(Ref* pSender, ListView::EventType type);
	void selectedMsgListItemEvent(Ref* pSender, ListView::EventType type);
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
	//Sprite * m_bgSprite;
	Sprite *ParentNode_Task;
	//nNewTaskFindPath nFindPath[MAX_TASK_MESSAGE_MUM];
	typedef std::map<int,nNewTaskFindPath> _FindPath;
	_FindPath nFindPath;
	Rect getRect(Node* pNode,int i);
	void removeitems();
	void AddTaskIndex();
	//Label* pMoneyLabel;
	//Label* pXLabel;
	LayerColor * colorLayer;
	Action* red;
	Layer *m_pMainUiLayer;
	//Layout  *m_pWidget;
	//CCArray* m_array;
	ListView* ptaskMsglistView;
	ListView* pmainListView;
	//CheckBox* checkBox;
	Size btnSize;
	bool m_bScrolling;
	Point m_lastPoint;
	bool __isCanMove;
	Size btnNewSize;
	Size btnCurSize;
	Size btnMsgSize;
	int  __nSelIndex;
	int  __nCount;
	KIniFile           m_IndexIDIni;
	KLinkStruct<KTaskIndexNode> m_IndexData; //�����������ݵ�����
};

#endif /* defined(__KuiTaskInfo__) */
