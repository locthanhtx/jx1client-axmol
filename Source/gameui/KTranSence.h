#ifndef __GAME_KTRANSENCE_H__
#define __GAME_KTRANSENCE_H__
/*
//��½���桡�����ʺ�����
*/
#include "cocos2d.h"

#include <gameui/Uiglobaldata.h>
#include<list>
#include "ImageStore/KImageStore2.h"
#include "engine/Kgetinidata.h"
#include "physics-nodes/PhysicsSprite.h"
#include "network/Thread.h"
#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"

using namespace ax;

using namespace std;

class KTranSence : public Layer,protected CThread//,public EditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in ax-x returns bool, instead of returning 'id' in ax-iphone
	KTranSence();
	~KTranSence();
	virtual bool init();
	/*virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
   */
	using CThread::Start;
	virtual void update(float delta);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static ax::Scene* scene();
	CREATE_FUNC(KTranSence);
	//��дdraw()����
//	virtual void draw();
	virtual void  Run();
	//KImageStore2 m_ImageStore;
private:
	void loadSuccess();
	//��ȡ�Ļص�����
	void loadingCallBack(Ref *obj);
	void updateProgress(float dt);
	// a selector callback
	void mianCancelCallback(Ref* pSender);
	// implement the "static node()" method manually
	//��ȡ��ʼʱ��Ľ�����
	Sprite *m_pLoadBarStart;
	//��ȡ���ʱ��Ľ�����
	ProgressTimer *m_pLoadBarEnd;
	//�߳���صĺ���
	//�ܵļ���ͼƬ��
	int m_nAllLoadResources;
	//��ǰ����ͼƬ��
	int m_nLoadedResources;
	//��ȡ����
	float m_fProgressIndex;
	Label *m_pLabelLoading;
	Label *m_pLabelPercent;
	//	��Ҫ������m_fWaitTime��Ч����ʱ�򣬽���һ����update()��init()�����ʼ����ע��ȡ��������
	//	float m_fWaitTime
	//====iKNetMsgTargetObject�ӿں���====
	void	AcceptNetMsg(void* pMsgData);	//����������Ϣ
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����
	Label* pMainLabel;
	//Label* pBugLabel;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	//virtual void ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent);

	// default implements are used to call script callback if exist
	virtual bool ccTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void ccTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void ccTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void ccTouchCancelled(Touch *pTouch, Event *pEvent);
	// default implements are used to call script callback if exist
	virtual void ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesCancelled(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void didAccelerate(Acceleration* pAccelerationValue);

	void    startload();//������ɺ� ִ��ɾ������
	void	ClearAccountPassword(bool bAccount, bool bPassword);
	void	ClearCachedata();
	void    mExit(Ref* pSender);
private:			//��ɫ�б���
	float m_ScaleX;
	float m_ScaleY;
	int   m_NpcIndex; //�ͻ������ǵ�NPC����
	//char nBugInfo[128];
};

#endif // __GAMEWORLD_SCENE_H__
