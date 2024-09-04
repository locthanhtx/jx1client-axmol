//#include "gamecore/KCore.h"
#include "KHRocker.h"
#include "gamecore/GameDataDef.h"
#include "gameui/Uiglobaldata.h"
USING_NS_AX;

void HRocker::updatePos(float dt){
    ax::Vec2 currentVec = ax::Vec2(currentPoint.x, currentPoint.y);
    ax::Vec2 newPosition = jsSprite->getPosition() + (currentPoint - jsSprite->getPosition()) * 0.5;
    jsSprite->setPosition(newPosition);
	//jsSprite->setPosition(ccpAdd(jsSprite->getPosition(),ccpMult(ccpSub(currentPoint, jsSprite->getPosition()),0.5)));
}
//启动摇杆
void HRocker::Active()
{
	if (!active)
	{
		active=true;
		this->schedule(CC_SCHEDULE_SELECTOR(HRocker::updatePos), 0.0f);                       //添加刷新函数
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = AX_CALLBACK_2(HRocker::ccTouchBegan, this);
        touchListener->onTouchMoved = AX_CALLBACK_2(HRocker::ccTouchMoved, this);
        touchListener->onTouchEnded = AX_CALLBACK_2(HRocker::ccTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
//        auto listener = ax::EventListenerTouchOneByOne::create();
//		ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}else {
	}
}
//锟斤拷锟揭★拷锟?
void   HRocker::Inactive()
{
	if (active) {
		active=false;
		this->unschedule(CC_SCHEDULE_SELECTOR(HRocker::updatePos));                 //删除刷新
//		ax::Director::getInstance()->getTouchDispatcher()->removeDelegate(this);//删除委托

        auto dispatcher = ax::Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListenersForTarget(this);
	}else {
	}
}

//第二部：计算出两个向量之间的夹角
//因为求出的角度是以π为单位，所以这里要转换成以度为单位
float HRocker::getAngleSigned()
{//ccpSub(centerPoint, currentPoint),CCPointMake(0, 1))
	//return CC_RADIANS_TO_DEGREES(ccpAngleSigned(ccpSub(centerPoint, currentPoint),CCPointMake(0, 1)));
	/*  //#define PI 3.141592654    float angle = 180.f / PI * rad; ==等于角度
	float dx = currentPoint.x - centerPoint.x;
	float dy = currentPoint.y - centerPoint.y;
	float t = dx/dy;

	float  at = atan(t);
	return at/3.1415926*180;

	// 获取角度
	Point p1 = this->getPosition(); //摇杆的中心的位置
	Point p2 = touch->getLocation();//触摸点的位置

	*/
    float rad = getRad(centerPoint,currentPoint);

	return 180.f/3.141592654*rad;

}

float HRocker::getRad(Point p1,Point p2)
{
	float xx = p2.x - p1.x;
	float yy = p2.y - p1.y;
	// 斜边
	float xie = sqrt(pow(xx, 2) + pow(yy, 2));
	// yy >= 0 弧度在于 0 到 π 之间。(0~180°)
	// yy < 0 弧度在于 π 到 2π 之间。(180°~360°)
	float rad = yy >= 0 ? (acos(xx / xie)) : (3.141592654 * 2 - acos(xx / xie));
	return rad;
}
//摇杆方位
Point HRocker::getDirection()
{
    ax::Vec2 vecCenterPoint = ax::Vec2(centerPoint);
    ax::Vec2 vecCurrentPoint = ax::Vec2(currentPoint);
    ax::Vec2 vec2 = vecCenterPoint - vecCurrentPoint;
    vec2.normalize();
	return ax::Point(vec2);
}
//摇杆和中心点的距离
float HRocker::getVelocity()
{
    ax::Vec2 vecCenterPoint = ax::Vec2(centerPoint);
    ax::Vec2 vecCurrentPoint = ax::Vec2(currentPoint);
    float distance = vecCenterPoint.distance(vecCurrentPoint);

    return distance;
}
HRocker* HRocker:: HRockerWithCenter(Point aPoint ,float aRadius ,Sprite* aJsSprite,Sprite* aJsBg,bool _isFollowRole){
	HRocker *jstick=HRocker::create();
	jstick->initWithCenter(aPoint,aRadius,aJsSprite,aJsBg,_isFollowRole);
	return jstick;
}

bool HRocker::ccTouchBegan(Touch* touch, Event* event)
{//开始移动
	if (!active)
		return false;
	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,false);

	if (pCoreShell && pCoreShell->getTradeState()) return false;

	this->setVisible(true);
    ax::Vec2 touchVec2 = touch->getLocation();
    touchVec2 = ax::Director::getInstance()->convertToGL(touchVec2);
	Point touchPoint = ax::Point(touchVec2);
	if(!isFollowRole){
        ax::Vec2 vecTouchPoint = ax::Vec2(touchPoint);
        ax::Vec2 vecCenterPoint = ax::Vec2(centerPoint);
        float distance = vecTouchPoint.distance(vecCenterPoint);
		if (distance > radius){
			return false;
		}
	}
	currentPoint = touchPoint;
	if(isFollowRole){
		centerPoint=currentPoint;
		jsSprite->setPosition(currentPoint);
		this->getChildByTag(1000)->setPosition(currentPoint);
	}
	//CCMessageBox("开始移动中","开始移动中");
	return true;
}
void  HRocker::ccTouchMoved(Touch* touch, Event* event)
{//移动中
	if (pCoreShell && pCoreShell->getTradeState()) return;
	if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,false);
	int nBeilv = 3;
    ax::Vec2 touchVec2 = touch->getLocation();
    touchVec2 = ax::Director::getInstance()->convertToGL(touchVec2);
    Point touchPoint = ax::Point(touchVec2);

    ax::Vec2 vecTouchPoint = ax::Vec2(touchPoint);
    ax::Vec2 vecCenterPoint = ax::Vec2(centerPoint);
    float nDistance = vecTouchPoint.distance(vecCenterPoint);
	if (nDistance > radius)
	{//超出范围
        ax::Vec2 Vec2Tmp = vecCenterPoint + ((vecTouchPoint - vecCenterPoint).getNormalized() * radius);
		currentPoint = ax::Point(Vec2Tmp);
	}else {
		currentPoint = touchPoint;
	}
	//char nPiontinfo[128]={0};
    /*sprintf(nPiontinfo,"CurrentPointX:%f,CurrentPointY:%f\nCenterPointX:%f,CenterPointY:%f",
		    currentPoint.x,currentPoint.y,centerPoint.x,centerPoint.y);
	messageBox(nPiontinfo,"摇锟斤拷锟狡讹拷锟斤拷");*/
	float nVer = getAngleSigned();  //中心点与触摸点的距离
    float nDestPontX,nDestPontY;
	if (nVer==0)
	{//正右
        nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//目标gl坐标x y不变
		nDestPontY = visibleSize.height-nRolePoint.y;
	}
	else if(nVer>0 && nVer<90)
	{//右上
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//目标gl坐标x y不变
		nDestPontY = visibleSize.height-(nRolePoint.y+(currentPoint.y-centerPoint.y)*nBeilv);
	}
	else if (nVer==90)
	{//正上
		nDestPontX = nRolePoint.x;//目标gl坐标x y不变
		nDestPontY = visibleSize.height-(nRolePoint.y+(currentPoint.y-centerPoint.y)*nBeilv);
	}
	else if (nVer>90 && nVer<180)
	{//左上
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*nBeilv;//目标gl坐标x y不变
		nDestPontY = visibleSize.height-(nRolePoint.y+(currentPoint.y-centerPoint.y)*nBeilv);
	}
	else if (nVer==180)
	{//正左
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*nBeilv;//目标gl坐标x y不变
		nDestPontY = visibleSize.height-nRolePoint.y;
	}
	else if (nVer>180 && nVer<270)
	{//左下
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*nBeilv;//目标gl坐标x y不变
		nDestPontY = visibleSize.height-(nRolePoint.y-(centerPoint.y-currentPoint.y)*nBeilv);
	}
	else if (nVer==270)
	{//正下
		nDestPontX = nRolePoint.x;//目标gl坐标x y不变
		nDestPontY = visibleSize.height-(nRolePoint.y-(centerPoint.y-currentPoint.y)*nBeilv);
	}
	else if (nVer>270 && nVer<360)
	{//右下
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//
		nDestPontY = visibleSize.height-(nRolePoint.y-(centerPoint.y-currentPoint.y)*nBeilv);
	}
	else if (nVer==360)
	{//正右
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//
		nDestPontY = visibleSize.height-nRolePoint.y;
	}

	if (pCoreShell == NULL)
		return ;
	//if (g_pCoreShell->ThrowAwayItem())	//先扔掉手里的物品
	//return ;
	//int yyy=g_pCoreShell->GetAutoplayid();
	//if(yyy)
	//g_pCoreShell->OperationRequest(GOI_AUTOPALYOPEN,0,-1,100);    //动鼠标取消挂机状态[暂时取消该功能]

	//g_pCoreShell->OperationRequest(GOI_SWITCH_SKILLS, 0, 0);      //动鼠标丢弃拾取技能
	//g_pCoreShell->SceneMapOperation(GSMOI_DEL_GREEN_LINE, 0, 0);  //动鼠标就取消画黄线
	//g_pCoreShell->OperationRequest(GOI_JINDUTIAO_CALLBACK, 0, 0); //关闭进度条
	/*KUiPlayerItem SelectPlayer;
	int nNPCKind = -1;
	if (pCoreShell->FindSelectNPC(nDestPontX, nDestPontY, relation_all, false, &SelectPlayer, nNPCKind))
	{//鼠标的位置是否有NPC
		//int nRelation = g_pCoreShell->GetNPCRelation(SelectPlayer.nIndex);
		pCoreShell->LockSomeoneAction(SelectPlayer.nIndex);
	}
	else*/
		pCoreShell->LockSomeoneAction(0);

	/*int nObjKind = -1;
	int nObjectIdx = 0;
	if (pCoreShell->FindSelectObject(nDestPontX,nDestPontY, false, nObjectIdx, nObjKind))
	{//鼠标的位置是否 有物品
		pCoreShell->LockObjectAction(nObjectIdx);
	}
	else*/
		pCoreShell->LockObjectAction(0);

	if (pCoreShell)
	{
		isRun  = false;
		moveEndPoint.x = nDestPontX;
		moveEndPoint.y = nDestPontY;
		pCoreShell->GotoWhere(nDestPontX,nDestPontY,0); //走向鼠标坐标点
	}

}

void  HRocker::ccTouchEnded(Touch* touch, Event* event)
{//移动结束
	if (pCoreShell && pCoreShell->getTradeState()) return;

	if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,true);

	currentPoint = centerPoint;
	if(isFollowRole){
		this->setVisible(false);
	}
	if (pCoreShell)
		pCoreShell->SetNpcCurPos();
	  // pCoreShell->setSand();
	isRun = true;
	//messageBox("锟狡讹拷锟斤拷锟斤拷","锟狡讹拷锟斤拷锟斤拷");
}

HRocker* HRocker::initWithCenter(Point aPoint ,float aRadius ,Sprite* aJsSprite,Sprite* aJsBg,bool _isFollowRole){
	isFollowRole =_isFollowRole;
	active = false;
	isRun  = false;
	radius = aRadius;
	visibleSize = ax::Director::getInstance()->getVisibleSize();//锟缴硷拷锟斤拷
	pCoreShell = NULL;
	//__GameWorld = NULL;
	moveEndPoint = ax::Vec2(0,0);
	if(!_isFollowRole){
		centerPoint =aPoint;
	}else{
		centerPoint =ax::Vec2(0,0);  //锟斤拷锟斤拷锟斤拷锟斤拷
	}
	currentPoint = centerPoint; //移动后当前的坐标
	jsSprite = aJsSprite;
	jsSprite->setPosition(centerPoint);
	aJsBg->setPosition(centerPoint);
	aJsBg->setTag(1000);
	this->addChild(aJsBg);
	this->addChild(jsSprite);
	if(isFollowRole){
		this->setVisible(false);
	}
	this->Active();//激活摇杆
	return this;
}
