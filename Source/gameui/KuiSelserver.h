#ifndef __GAME_KuiSelserver_H__
#define __GAME_KuiSelserver_H__
/*
//��½���桡�����ʺ�����
*/
#include "cocos2d.h"

#include <gameui/Uiglobaldata.h>
#include<list>
#include "ImageStore/KImageStore2.h"
#include "engine/KIniFile.h"
#include "engine/Kgetinidata.h"
#include "physics-nodes/PhysicsSprite.h"

#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"

using namespace ax;

using namespace std;
using namespace ui;

class KuiSelserver : public Layer//,public EditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in ax-x returns bool, instead of returning 'id' in ax-iphone
	KuiSelserver();
	~KuiSelserver();
	virtual bool init();
	static ax::Scene* scene();

	CREATE_FUNC(KuiSelserver);
	//��дdraw()����
//	virtual void draw();
	virtual void update(float delta);
private:
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����

	Animate* animate;

	Sprite *testSprite;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	void  selServerCallBack(Ref* menuItem);
	// a selector callback
	void mianLoginCallback(Ref* pSender);
	void mianCancelCallback(Ref* pSender);

	virtual bool ccTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void ccTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void ccTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void ccTouchCancelled(Touch *pTouch, Event *pEvent);

	virtual void ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesCancelled(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void didAccelerate(Acceleration* pAccelerationValue);
	void    finish();//������ɺ� ִ��ɾ������
private:
	int     nserCount;
	int     nCurSelIndex;
	KIniFile nserlist;
};

#endif // __GAMEWORLD_SCENE_H__
