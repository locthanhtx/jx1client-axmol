//#include "gamecore/KCore.h"
#include "KHRocker.h"
#include "gamecore/GameDataDef.h"
#include "gameui/Uiglobaldata.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerTouch.h"
USING_NS_AX;

void HRocker::updatePos(float dt){
    ax::Vec2 currentVec = ax::Vec2(currentPoint.x, currentPoint.y);
    ax::Vec2 newPosition = jsSprite->getPosition() + (currentPoint - jsSprite->getPosition()) * 0.5;
    jsSprite->setPosition(newPosition);
	//jsSprite->setPosition(ccpAdd(jsSprite->getPosition(),ccpMult(ccpSub(currentPoint, jsSprite->getPosition()),0.5)));
}
//����ҡ��
void HRocker::Active()
{
	if (!active)
	{
		active=true;
		this->schedule(CC_SCHEDULE_SELECTOR(HRocker::updatePos), 0.0f);                       //���ˢ�º���
        auto listener = ax::EventListenerTouchOneByOne::create();
		ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}else {
	}
}
//���ҡ��
void   HRocker::Inactive()
{
	if (active) {
		active=false;
		this->unschedule(CC_SCHEDULE_SELECTOR(HRocker::updatePos));                 //ɾ��ˢ��
//		ax::Director::getInstance()->getTouchDispatcher()->removeDelegate(this);//ɾ��ί��

        auto touchListener = ax::EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(false);

        touchListener->onTouchBegan = [](ax::Touch* touch, ax::Event* event) {
            return true;
        };

        touchListener->onTouchMoved = [](ax::Touch* touch, ax::Event* event) {

        };

        ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	}else {
	}
}

//�ڶ������������������֮��ļн�
//��Ϊ����ĽǶ����Ԧ�Ϊ��λ����������Ҫת�����Զ�Ϊ��λ
float HRocker::getAngleSigned()
{//ccpSub(centerPoint, currentPoint),CCPointMake(0, 1))
	//return CC_RADIANS_TO_DEGREES(ccpAngleSigned(ccpSub(centerPoint, currentPoint),CCPointMake(0, 1)));
	/*  //#define PI 3.141592654    float angle = 180.f / PI * rad; ==���ڽǶ�
	float dx = currentPoint.x - centerPoint.x;
	float dy = currentPoint.y - centerPoint.y;
	float t = dx/dy;

	float  at = atan(t);
	return at/3.1415926*180;

	// ��ȡ�Ƕ�
	Point p1 = this->getPosition(); //ҡ�˵����ĵ�λ��
	Point p2 = touch->getLocation();//�������λ��

	*/
    float rad = getRad(centerPoint,currentPoint);

	return 180.f/3.141592654*rad;

}

float HRocker::getRad(Point p1,Point p2)
{
	float xx = p2.x - p1.x;
	float yy = p2.y - p1.y;
	// б��
	float xie = sqrt(pow(xx, 2) + pow(yy, 2));
	// yy >= 0 �������� 0 �� �� ֮�䡣(0~180��)
	// yy < 0 �������� �� �� 2�� ֮�䡣(180��~360��)
	float rad = yy >= 0 ? (acos(xx / xie)) : (3.141592654 * 2 - acos(xx / xie));
	return rad;
}
//ҡ�˷�λ
Point HRocker::getDirection()
{
    ax::Vec2 vecCenterPoint = ax::Vec2(centerPoint);
    ax::Vec2 vecCurrentPoint = ax::Vec2(currentPoint);
    ax::Vec2 vec2 = vecCenterPoint - vecCurrentPoint;
    vec2.normalize();
	return ax::Point(vec2);
}
//ҡ�˺����ĵ�ľ���
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
{//��ʼ�ƶ�
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
	//ccMessageBox("��ʼ�ƶ���","��ʼ�ƶ���");
	return true;
}
void  HRocker::ccTouchMoved(Touch* touch, Event* event)
{//�ƶ���
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
	{
        ax::Vec2 Vec2Tmp = vecCenterPoint + ((vecTouchPoint - vecCenterPoint).getNormalized() * radius);
		currentPoint = ax::Point(Vec2Tmp);
	}else {
		currentPoint = touchPoint;
	}
	//char nPiontinfo[128]={0};
    /*sprintf(nPiontinfo,"CurrentPointX:%f,CurrentPointY:%f\nCenterPointX:%f,CenterPointY:%f",
		    currentPoint.x,currentPoint.y,centerPoint.x,centerPoint.y);
	ccMessageBox(nPiontinfo,"ҡ���ƶ���");*/
	float nVer = getAngleSigned();  //���ĵ��봥����ľ���
    float nDestPontX,nDestPontY;
	if (nVer==0)
	{//����
        nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-nRolePoint.y;
	}
	else if(nVer>0 && nVer<90)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-(nRolePoint.y+(currentPoint.y-centerPoint.y)*nBeilv);
	}
	else if (nVer==90)
	{//����
		nDestPontX = nRolePoint.x;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-(nRolePoint.y+(currentPoint.y-centerPoint.y)*nBeilv);
	}
	else if (nVer>90 && nVer<180)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*nBeilv;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-(nRolePoint.y+(currentPoint.y-centerPoint.y)*nBeilv);
	}
	else if (nVer==180)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*nBeilv;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-nRolePoint.y;
	}
	else if (nVer>180 && nVer<270)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*nBeilv;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-(nRolePoint.y-(centerPoint.y-currentPoint.y)*nBeilv);
	}
	else if (nVer==270)
	{//����
		nDestPontX = nRolePoint.x;//Ŀ��gl����x y����
		nDestPontY = visibleSize.height-(nRolePoint.y-(centerPoint.y-currentPoint.y)*nBeilv);
	}
	else if (nVer>270 && nVer<360)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//
		nDestPontY = visibleSize.height-(nRolePoint.y-(centerPoint.y-currentPoint.y)*nBeilv);
	}
	else if (nVer==360)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*nBeilv;//
		nDestPontY = visibleSize.height-nRolePoint.y;
	}

	if (pCoreShell == NULL)
		return ;
	//if (g_pCoreShell->ThrowAwayItem())	//���ӵ��������Ʒ
	//return ;
	//int yyy=g_pCoreShell->GetAutoplayid();
	//if(yyy)
	//g_pCoreShell->OperationRequest(GOI_AUTOPALYOPEN,0,-1,100);    //�����ȡ���һ�״̬[��ʱȡ���ù���]

	//g_pCoreShell->OperationRequest(GOI_SWITCH_SKILLS, 0, 0);      //����궪��ʰȡ����
	//g_pCoreShell->SceneMapOperation(GSMOI_DEL_GREEN_LINE, 0, 0);  //������ȡ��������
	//g_pCoreShell->OperationRequest(GOI_JINDUTIAO_CALLBACK, 0, 0); //�رս�����
	/*KUiPlayerItem SelectPlayer;
	int nNPCKind = -1;
	if (pCoreShell->FindSelectNPC(nDestPontX, nDestPontY, relation_all, false, &SelectPlayer, nNPCKind))
	{//����λ���Ƿ���NPC
		//int nRelation = g_pCoreShell->GetNPCRelation(SelectPlayer.nIndex);
		pCoreShell->LockSomeoneAction(SelectPlayer.nIndex);
	}
	else*/
		pCoreShell->LockSomeoneAction(0);

	/*int nObjKind = -1;
	int nObjectIdx = 0;
	if (pCoreShell->FindSelectObject(nDestPontX,nDestPontY, false, nObjectIdx, nObjKind))
	{//����λ���Ƿ� ����Ʒ
		pCoreShell->LockObjectAction(nObjectIdx);
	}
	else*/
		pCoreShell->LockObjectAction(0);

	if (pCoreShell)
	{
		isRun  = false;
		moveEndPoint.x = nDestPontX;
		moveEndPoint.y = nDestPontY;
		pCoreShell->GotoWhere(nDestPontX,nDestPontY,0); //������������
	}

}

void  HRocker::ccTouchEnded(Touch* touch, Event* event)
{//�ƶ�����
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
	//ccMessageBox("�ƶ�����","�ƶ�����");
}

HRocker* HRocker::initWithCenter(Point aPoint ,float aRadius ,Sprite* aJsSprite,Sprite* aJsBg,bool _isFollowRole){
	isFollowRole =_isFollowRole;
	active = false;
	isRun  = false;
	radius = aRadius;
	visibleSize = ax::Director::getInstance()->getVisibleSize();//�ɼ���
	pCoreShell = NULL;
	//__GameWorld = NULL;
	moveEndPoint = ax::Vec2(0,0);
	if(!_isFollowRole){
		centerPoint =aPoint;
	}else{
		centerPoint =ax::Vec2(0,0);  //��������
	}
	currentPoint = centerPoint; //�ƶ���ǰ������
	jsSprite = aJsSprite;
	jsSprite->setPosition(centerPoint);
	aJsBg->setPosition(centerPoint);
	aJsBg->setTag(1000);
	this->addChild(aJsBg);
	this->addChild(jsSprite);
	if(isFollowRole){
		this->setVisible(false);
	}
	this->Active();//����ҡ��
	return this;
}
