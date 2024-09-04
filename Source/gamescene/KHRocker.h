#ifndef __HROCKER_SCENE_H__
#define __HROCKER_SCENE_H__
#include "cocos2d.h"
#include "gamecore/CoreShell.h"
//#include "KgameWorld.h"
using namespace ax;

class HRocker :public Layer {
public :
	//��ʼ�� aPoint��ҡ������ aRadius��ҡ�˰뾶 aJsSprite��ҡ�˿��Ƶ� aJsBg��ҡ�˱���
	static HRocker*  HRockerWithCenter(Point aPoint ,float aRadius ,Sprite* aJsSprite,Sprite* aJsBg,bool _isFollowRole);
	//����ҡ��
	void Active();
	//���ҡ��
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
	Point centerPoint; //ҡ������
	Point currentPoint;//ҡ�˵�ǰλ��
	bool isRun;
	bool active;//�Ƿ񼤻�ҡ��
	float radius;//ҡ�˰뾶
	Sprite *jsSprite;
	bool isFollowRole;//�Ƿ�����û����
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