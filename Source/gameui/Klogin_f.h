#ifndef __GAME_KLOGIN_F_H__
#define __GAME_KLOGIN_F_H__
/*
//��½���桡��ʼ����
*/
#include "cocos2d.h"

//#include <gameui/Uiglobaldata.h>
#include<list>
#include "ImageStore/KImageStore2.h"
#include "engine/Kgetinidata.h"
#include "physics-nodes/PhysicsSprite.h"

#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"
//#include "gamescene/KSkillRocker.h"

using namespace ax;
//
using namespace std;

class Klogin_f : public ax::Layer//,public EditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in ax-x returns bool, instead of returning 'id' in ax-iphone

	virtual bool init();
	/*virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
   */
	virtual void update(float delta);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static ax::Scene* scene();
	// a selector callback
	//Label* GetTestLabel(Ref* pSender){return pMainLabel;};
	// implement the "static node()" method manually
	CREATE_FUNC(Klogin_f);
	//��дdraw()����
//	virtual void draw();
	KImageStore2 m_ImageStore;
	//Size visibleSize;
	//Point origin;
private:
	//Texture2D* m_pTexture;
	//Label* pMainLabel;

	Sprite *testSprite;
	int       nTestCount;
	UINT      nTimeCount;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	void  buttonCallback(Node *pNode);
	//unsigned int g_FileName2Id_(char * lpFileName);
	//virtual void ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	void mianLoginCallback(Ref* pSender);
	void mianCancelCallback(Ref* pSender);
	void accRegCallback(Ref* pSender);
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

	void closeAccCallback(Node *pNode);

	void finish();
	void mExit(Ref* pSender);
	bool m_AccPadIsOpen;
	Layer *mainlayer;
	//KSkillRocker *skillstick_f;

};

#endif // __GAMEWORLD_SCENE_H__
