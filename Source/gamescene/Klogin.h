#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
//#include <collections.h>
#include<list>
//#include "ODSocket.h"
//#include "Box2D.h"
#include "cocos-ext.h"                      //��CCPhysicsSprite����Ҫ�����ͷ�ļ�
#include "ImageStore/KImageStore2.h"
//#include "engine/KSprCodec.h"
#include "physics-nodes/CCPhysicsSprite.h"
#include "cocos-ext.h"
#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"

USING_NS_CC_EXT;
USING_NS_AX;

#include "gamescene/KHRocker.h"
class Klogin : public ax::Layer
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
	void mianSkillCallback(Ref* pSender);
	Label* GetTestLabel(Ref* pSender){return pMainLabel;};
	// implement the "static node()" method manually
	CREATE_FUNC(Klogin);
	//��дdraw()����
//	virtual void draw();
	KImageStore2 m_ImageStore;
	KTimer start;
    ax::Size visibleSize;
	Point origin;
private:
	HRocker   *joystick;
	PBYTE     pWW;
	Animate* animate;
	Texture2D* m_pTexture;
	Label* pMainLabel;
//	KSprCodec SprDecode;
	Sprite *testSprite;
	int       nTestCount;
	UINT      nTimeCount;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	void  buttonCallback(Node *pNode);
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

	void   beiyongb(Ref* pSender);
	void   beiyonga(Ref* pSender);
	void   nextsenc(Ref* pSender);
	void   delspr(Ref* pSender);
	void   addspr(Ref* pSender);
	void   addmap(Ref* pSender);


	void   finish();//������ɺ� ִ��ɾ������
	void   endSprite();
};

#endif // __LOGIN_SCENE_H__
