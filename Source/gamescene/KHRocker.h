// ***************************************************************************************
// Chỉnh sửa lại bởi duccom0123 07/09/2024 6:42:22 CH
// ***************************************************************************************
#ifndef __HROCKER_SCENE_H__
#define __HROCKER_SCENE_H__
#include "cocos2d.h"
#include "gamecore/CoreShell.h"
using namespace ax;

class HRocker : public Layer
{
public:
    static HRocker* HRockerWithCenter(Point aPoint,
                                      float aRadius,
                                      Sprite* aJsSprite,
                                      Sprite* aJsBg,
                                      bool _isFollowRole);

    void Active();

    void Inactive();
    Point getDirection();
    float getAngleSigned();
    float getRad(Point p1, Point p2);
    bool getIsRun() { return isRun; };
    void setIsRun(bool nisRun) { isRun = nisRun; };
    void setRoleIndex(int nIdx) { nRoleIndex = nIdx; };
    void setRolePoint(Point nPoint) { nRolePoint = nPoint; };
    void setCoreShell(iCoreShell* nCoreShell) { pCoreShell = nCoreShell; };

    Point getMoveEndPos() { return moveEndPoint; };

private:
    HRocker* initWithCenter(Point aPoint, float aRadius, Sprite* aJsSprite, Sprite* aJsBg, bool _isFollowRole);
    Point centerPoint;
    Point currentPoint;
    bool isRun;
    bool active;
    float radius;
    Sprite* jsSprite;
    bool isFollowRole;
    float getVelocity();
    virtual bool ccTouchBegan(const std::vector<ax::Touch*>& pTouches, Event* pEvent);
    virtual void ccTouchMoved(const std::vector<ax::Touch*>& pTouches, Event* pEvent);
    virtual void ccTouchEnded(const std::vector<ax::Touch*>& pTouches, Event* pEvent);
    Point nRolePoint;
    int nRoleIndex;
    iCoreShell* pCoreShell;

    Size visibleSize;
    Point moveEndPoint;
    CREATE_FUNC(HRocker);
    void updatePos(float dt);
};
#endif
