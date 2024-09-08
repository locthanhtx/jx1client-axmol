// ***************************************************************************************
// Chỉnh sửa lại bởi duccom0123 07/09/2024 6:42:22 CH
// ***************************************************************************************
#include "KHRocker.h"
#include "gamecore/GameDataDef.h"
#include "gameui/Uiglobaldata.h"
USING_NS_AX;

void HRocker::updatePos(float dt)
{
    ax::Vec2 currentVec  = ax::Vec2(currentPoint.x, currentPoint.y);
    ax::Vec2 newPosition = jsSprite->getPosition() + (currentPoint - jsSprite->getPosition()) * 0.5;
    jsSprite->setPosition(newPosition);
}
void HRocker::Active()
{
    if (!active)
    {
        active = true;
        this->schedule(CC_SCHEDULE_SELECTOR(HRocker::updatePos), 0.0f);
        //duccom0123 chỉnh sửa để sử dụng được joystick
        auto touchListener            = EventListenerTouchAllAtOnce::create();
        touchListener->onTouchesBegan = AX_CALLBACK_2(HRocker::ccTouchBegan, this);
        touchListener->onTouchesMoved = AX_CALLBACK_2(HRocker::ccTouchMoved, this);
        touchListener->onTouchesEnded = AX_CALLBACK_2(HRocker::ccTouchEnded, this);
        auto dispatcher               = ax::Director::getInstance()->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    }
    else
    {
    }
}
void HRocker::Inactive()
{
    if (active)
    {
        active = false;
        this->unschedule(CC_SCHEDULE_SELECTOR(HRocker::updatePos));

        auto dispatcher = ax::Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListenersForTarget(this);
    }
    else
    {
    }
}

float HRocker::getAngleSigned()
{

    float rad = getRad(centerPoint, currentPoint);

    return 180.f / 3.141592654 * rad;
}

float HRocker::getRad(Point p1, Point p2)
{
    float xx = p2.x - p1.x;
    float yy = p2.y - p1.y;

    float xie = sqrt(pow(xx, 2) + pow(yy, 2));

    float rad = yy >= 0 ? (acos(xx / xie)) : (3.141592654 * 2 - acos(xx / xie));
    return rad;
}
Point HRocker::getDirection()
{
    ax::Vec2 vecCenterPoint  = ax::Vec2(centerPoint);
    ax::Vec2 vecCurrentPoint = ax::Vec2(currentPoint);
    ax::Vec2 vec2            = vecCenterPoint - vecCurrentPoint;
    vec2.normalize();
    return ax::Point(vec2);
}
float HRocker::getVelocity()
{
    ax::Vec2 vecCenterPoint  = ax::Vec2(centerPoint);
    ax::Vec2 vecCurrentPoint = ax::Vec2(currentPoint);
    float distance           = vecCenterPoint.distance(vecCurrentPoint);

    return distance;
}
HRocker* HRocker::HRockerWithCenter(Point aPoint, float aRadius, Sprite* aJsSprite, Sprite* aJsBg, bool _isFollowRole)
{
    HRocker* jstick = HRocker::create();
    jstick->initWithCenter(aPoint, aRadius, aJsSprite, aJsBg, _isFollowRole);
    return jstick;
}
// duccom0123 chỉnh sửa để sử dụng được joystick
bool HRocker::ccTouchBegan(const std::vector<ax::Touch*>& pTouches, Event* pEvent)
{
    if (!active)
        return false;

    if (pCoreShell && pCoreShell->getTradeState())
        return false;

    this->setVisible(true);
    auto it            = pTouches.begin();
    Touch* touch       = (Touch*)(*it);
    ax::Vec2 touchVec2 = touch->getLocationInView();
    touchVec2          = ax::Director::getInstance()->convertToGL(touchVec2);
    Point touchPoint   = ax::Point(touchVec2);
    if (!isFollowRole)
    {
        ax::Vec2 vecTouchPoint  = ax::Vec2(touchPoint);
        ax::Vec2 vecCenterPoint = ax::Vec2(centerPoint);
        float distance          = vecTouchPoint.distance(vecCenterPoint);
        if (distance > radius)
        {
            return false;
        }
    }
    currentPoint = touchPoint;
    if (isFollowRole)
    {
        centerPoint = currentPoint;
        jsSprite->setPosition(currentPoint);
        this->getChildByTag(1000)->setPosition(currentPoint);
    }

    return true;
}
// duccom0123 chỉnh sửa để sử dụng được joystick
void HRocker::ccTouchMoved(const std::vector<ax::Touch*>& pTouches, Event* pEvent)
{
    if (pCoreShell && pCoreShell->getTradeState())
        return;
    if (pCoreShell)
        pCoreShell->setPadCanMove(PAD_KIND_ITEM, false);
    int nBeilv         = 3;
    auto it            = pTouches.begin();
    Touch* touch       = (Touch*)(*it);
    ax::Vec2 touchVec2 = touch->getLocationInView();
    touchVec2          = ax::Director::getInstance()->convertToGL(touchVec2);
    Point touchPoint   = ax::Point(touchVec2);

    ax::Vec2 vecTouchPoint  = ax::Vec2(touchPoint);
    ax::Vec2 vecCenterPoint = ax::Vec2(centerPoint);
    float nDistance         = vecTouchPoint.distance(vecCenterPoint);
    if (nDistance > radius)
    {
        ax::Vec2 Vec2Tmp = vecCenterPoint + ((vecTouchPoint - vecCenterPoint).getNormalized() * radius);
        currentPoint     = ax::Point(Vec2Tmp);
    }
    else
    {
        currentPoint = touchPoint;
    }

    float nVer = getAngleSigned();
    float nDestPontX, nDestPontY;
    if (nVer == 0)
    {
        nDestPontX = nRolePoint.x + (currentPoint.x - centerPoint.x) * nBeilv;
        nDestPontY = visibleSize.height - nRolePoint.y;
    }
    else if (nVer > 0 && nVer < 90)
    {
        nDestPontX = nRolePoint.x + (currentPoint.x - centerPoint.x) * nBeilv;
        nDestPontY = visibleSize.height - (nRolePoint.y + (currentPoint.y - centerPoint.y) * nBeilv);
    }
    else if (nVer == 90)
    {
        nDestPontX = nRolePoint.x;
        nDestPontY = visibleSize.height - (nRolePoint.y + (currentPoint.y - centerPoint.y) * nBeilv);
    }
    else if (nVer > 90 && nVer < 180)
    {
        nDestPontX = nRolePoint.x - (centerPoint.x - currentPoint.x) * nBeilv;
        nDestPontY = visibleSize.height - (nRolePoint.y + (currentPoint.y - centerPoint.y) * nBeilv);
    }
    else if (nVer == 180)
    {
        nDestPontX = nRolePoint.x - (centerPoint.x - currentPoint.x) * nBeilv;
        nDestPontY = visibleSize.height - nRolePoint.y;
    }
    else if (nVer > 180 && nVer < 270)
    {
        nDestPontX = nRolePoint.x - (centerPoint.x - currentPoint.x) * nBeilv;
        nDestPontY = visibleSize.height - (nRolePoint.y - (centerPoint.y - currentPoint.y) * nBeilv);
    }
    else if (nVer == 270)
    {
        nDestPontX = nRolePoint.x;
        nDestPontY = visibleSize.height - (nRolePoint.y - (centerPoint.y - currentPoint.y) * nBeilv);
    }
    else if (nVer > 270 && nVer < 360)
    {
        nDestPontX = nRolePoint.x + (currentPoint.x - centerPoint.x) * nBeilv;
        nDestPontY = visibleSize.height - (nRolePoint.y - (centerPoint.y - currentPoint.y) * nBeilv);
    }
    else if (nVer == 360)
    {
        nDestPontX = nRolePoint.x + (currentPoint.x - centerPoint.x) * nBeilv;
        nDestPontY = visibleSize.height - nRolePoint.y;
    }

    if (pCoreShell == NULL)
        return;

    pCoreShell->LockSomeoneAction(0);

    pCoreShell->LockObjectAction(0);

    if (pCoreShell)
    {
        isRun          = false;
        moveEndPoint.x = nDestPontX;
        moveEndPoint.y = nDestPontY;
        pCoreShell->GotoWhere(nDestPontX, nDestPontY, 0);
    }
}
// duccom0123 chỉnh sửa để sử dụng được joystick
void HRocker::ccTouchEnded(const std::vector<ax::Touch*>& pTouches, Event* pEvent)
{
    if (pCoreShell && pCoreShell->getTradeState())
        return;

    if (pCoreShell)
        pCoreShell->setPadCanMove(PAD_KIND_ITEM, true);

    currentPoint = centerPoint;
    if (isFollowRole)
    {
        this->setVisible(false);
    }
    if (pCoreShell)
        pCoreShell->SetNpcCurPos();

    isRun = true;
}

HRocker* HRocker::initWithCenter(Point aPoint, float aRadius, Sprite* aJsSprite, Sprite* aJsBg, bool _isFollowRole)
{
    isFollowRole = _isFollowRole;
    active       = false;
    isRun        = false;
    radius       = aRadius;
    visibleSize  = ax::Director::getInstance()->getVisibleSize();
    pCoreShell   = NULL;

    moveEndPoint = ax::Vec2(0, 0);
    if (!_isFollowRole)
    {
        centerPoint = aPoint;
    }
    else
    {
        centerPoint = ax::Vec2(0, 0);
    }
    currentPoint = centerPoint;
    jsSprite     = aJsSprite;
    jsSprite->setPosition(centerPoint);
    aJsBg->setPosition(centerPoint);
    aJsBg->setTag(1000);
    this->addChild(aJsBg);
    this->addChild(jsSprite);
    if (isFollowRole)
    {
        this->setVisible(false);
    }
    this->Active();
    return this;
}
