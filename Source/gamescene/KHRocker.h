#ifndef __HROCKER_SCENE_H__
#define __HROCKER_SCENE_H__
#include "cocos2d.h"
#include "gamecore/CoreShell.h"
//#include "KgameWorld.h"
using namespace ax;

class HRocker :public Layer {
public :
	//初始化 aPoint是摇杆中心 aRadius是摇杆半径 aJsSprite是摇杆控制点 aJsBg是摇杆背景
	static HRocker*  HRockerWithCenter(CCPoint aPoint ,float aRadius ,CCSprite* aJsSprite,CCSprite* aJsBg,bool _isFollowRole);
	//启动摇杆
	void Active();
	//解除摇杆
	void Inactive();
	Point getDirection();
	float   getAngleSigned();
	float   getRad(Point p1,Point p2);
	bool    getIsRun(){return isRun;};
	void    setIsRun(bool nisRun){isRun=nisRun;};
	void    setRoleIndex(int nIdx){nRoleIndex=nIdx;};
	void    setRolePoint(Point nPoint){nRolePoint=nPoint;};
	void    setCoreShell(iCoreShell * nCoreShell){pCoreShell=nCoreShell;};
	//void    setGameWorld(KgameWorld * nCoreShell){__GameWorld=nCoreShell;};
	Point getMoveEndPos(){return moveEndPoint;};
private:
	HRocker * initWithCenter(Point aPoint ,float aRadius ,Sprite* aJsSprite,Sprite* aJsBg,bool _isFollowRole);
	Point centerPoint; //摇杆中心
	Point currentPoint;//摇杆当前位置
	bool isRun;
	bool active;//是否激活摇杆
	float radius;//摇杆半径
	Sprite *jsSprite;
	bool isFollowRole;//是否跟随用户点击
	float getVelocity();
	virtual bool ccTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void ccTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void ccTouchEnded(Touch *pTouch, Event *pEvent);
	Point nRolePoint;
	int     nRoleIndex;
	iCoreShell * pCoreShell;
	//KgameWorld *__GameWorld; 
	Size visibleSize;
	Point moveEndPoint;
	CREATE_FUNC(HRocker);
    void updatePos(float dt);  //ax::Time
};
#endif // __HROCKER_SCENE_H__