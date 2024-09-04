//#include "gamecore/KCore.h"
#include "KSkillRocker.h"
#include "gamecore/GameDataDef.h"
#include "gameui/Uiglobaldata.h"
using namespace ax;

void KSkillRocker::updatePos(float dt){
    ax::Vec2 currentVec = ax::Vec2(currentPoint.x, currentPoint.y);
    jsSprite->setPosition(jsSprite->getPosition() + (currentVec - jsSprite->getPosition()) * 0.5);
}
//����ҡ��
void KSkillRocker::Active()
{
	if (!active)
	{
		active=true;
		this->schedule(CC_SCHEDULE_SELECTOR(KSkillRocker::updatePos), 0.0f);                       //���ˢ�º���
//		//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-129,false);
        auto listener = ax::EventListenerTouchOneByOne::create();
        ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        auto touchListener = EventListenerTouchOneByOne::create();
	    touchListener->onTouchesBegan = AX_CALLBACK_2(KSkillRocker::ccTouchBegan, this);
	    touchListener->onTouchesMoved = AX_CALLBACK_2(KSkillRocker::ccTouchMoved, this);
	    touchListener->onTouchesEnded = AX_CALLBACK_2(KSkillRocker::ccTouchEnded, this);
	    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	}else {
	}
}
//���ҡ��
void   KSkillRocker::Inactive()
{
	if (active) {
		active=false;
		this->unschedule(CC_SCHEDULE_SELECTOR(KSkillRocker::updatePos));                 //ɾ��ˢ��
//		ax::Director::getInstance()->getTouchDispatcher()->removeDelegate(this);//ɾ��ί��
        auto dispatcher = ax::Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListenersForTarget(this);
	}else {
	}
}

//�ڶ������������������֮��ļн�
//��Ϊ����ĽǶ����Ԧ�Ϊ��λ����������Ҫת�����Զ�Ϊ��λ
float KSkillRocker::getAngleSigned()
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

float KSkillRocker::getRad(Point p1,Point p2)
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
Point KSkillRocker::getDirection()
{
    ax::Vec2 vecCenterPoint = ax::Vec2(centerPoint);
    ax::Vec2 vecCurrentPoint = ax::Vec2(currentPoint);
    ax::Vec2 vec2 = vecCenterPoint - vecCurrentPoint;
    vec2.normalize();
    return ax::Point(vec2);
}
//ҡ�˺����ĵ�ľ���
float KSkillRocker::getVelocity()
{
    ax::Vec2 vecCenterPoint = ax::Vec2(centerPoint);
    ax::Vec2 vecCurrentPoint = ax::Vec2(currentPoint);
    float distance = vecCenterPoint.distance(vecCurrentPoint);

    return distance;
}
KSkillRocker* KSkillRocker:: KSkillRockerWithCenter(Point aPoint ,float aRadius ,Sprite* aJsSprite,Sprite* aJsBg,bool _isFollowRole){
	KSkillRocker *jstick=KSkillRocker::create();
	jstick->initWithCenter(aPoint,aRadius,aJsSprite,aJsBg,_isFollowRole);
	return jstick;
}

bool KSkillRocker::ccTouchBegan(Touch* touch, Event* event)
{//��ʼ�ƶ�
	if (!active)
		return false;
	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,false);

	if (pCoreShell && pCoreShell->getTradeState()) return false;

	this->setVisible(true);

	setSpriteOpacity(255);
	Point touchPoint = touch->getLocationInView();     //��ȡ����������
//	touchPoint = Director:: sharedDirector()->convertToGL(touchPoint); //ת����Ļ����
	touchPoint = Director::getInstance()->convertToUI(touchPoint); //ת����Ļ����
	if(!isFollowRole){
        ax::Vec2 vecTouchPoint = ax::Vec2(touchPoint);
        ax::Vec2 vecCenterPoint = ax::Vec2(centerPoint);
        float distance_tmp = vecTouchPoint.distance(vecCenterPoint);
		if (distance_tmp > radius){
			return false;
		}
	}
	currentPoint = touchPoint;
	if(isFollowRole){
		centerPoint=currentPoint;
		jsSprite->setPosition(currentPoint);
		this->getChildByTag(1000)->setPosition(currentPoint);
	}
	//messageBox("��ʼ�ƶ���","��ʼ�ƶ���");
	return true;
}
void  KSkillRocker::ccTouchMoved(Touch* touch, Event* event)
{//�ƶ���
	if (pCoreShell && pCoreShell->getTradeState()) return;
	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,false);
	int nBeilv = 3;
	Point touchPoint = touch->getLocationInView();                     //��Ļ����ϵ +x ���� +������
	touchPoint = Director::getInstance()->convertToUI(touchPoint); //ת�������½�gl�������(0,0)

    ax::Vec2 vecTouchPoint = ax::Vec2(touchPoint);
    ax::Vec2 vecCurrentPoint = ax::Vec2(currentPoint);

	float nDistance = vecTouchPoint.distance(vecCurrentPoint);
	if (nDistance > radius)
	{//������Χ
        ax::Vec2 Vec2Tmp = vecCurrentPoint + ((vecTouchPoint - vecCurrentPoint).getNormalized() * radius);
        currentPoint = ax::Point(Vec2Tmp);
	}else {
		currentPoint = touchPoint;
	}
	//char nPiontinfo[128]={0};
    /*sprintf(nPiontinfo,"CurrentPointX:%f,CurrentPointY:%f\nCenterPointX:%f,CenterPointY:%f",
		    currentPoint.x,currentPoint.y,centerPoint.x,centerPoint.y);
	messageBox(nPiontinfo,"ҡ���ƶ���");*/
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
/*
		pCoreShell->LockSomeoneAction(0);

		pCoreShell->LockObjectAction(0);

	if (pCoreShell)
	{
		isRun  = false;
		moveEndPoint.x = nDestPontX;
		moveEndPoint.y = nDestPontY;
		pCoreShell->GotoWhere(nDestPontX,nDestPontY,0); //������������
	}*/

}

void  KSkillRocker::ccTouchEnded(Touch* touch, Event* event)
{//�ƶ�����
	if (pCoreShell && pCoreShell->getTradeState()) return;

	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,true);

	currentPoint = centerPoint;
	//if(isFollowRole){
	//this->setVisible(false);
	//}
	//if (pCoreShell)
	//   pCoreShell->setSand();
	isRun = true;
	//this->setVisible(false);
	//setSpriteOpacity(100);
	setSpritePiont(ax::Vec2(visibleSize.width,-100.0f));
	//this->Inactive();
	//messageBox("�ƶ�����","�ƶ�����");
}

KSkillRocker* KSkillRocker::initWithCenter(Point aPoint ,float aRadius ,Sprite* aJsSprite,Sprite* aJsBg,bool _isFollowRole){
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

	/*Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setContentSize(ax::Size(200,200));//���úʹ��ڵĴ�С
	colorLayer->setPosition(CCPointMake(visibleSize.width-200,10));
	colorLayer->setAnchorPoint(ax::Vec2(0,0));
	//colorLayer->addChild(spriteBox);
	this->addChild(colorLayer);
	*/
	jsSprite = aJsSprite;
	jsSprite->setPosition(centerPoint);//����spr
	bgSprite = aJsBg;
	aJsBg->setPosition(centerPoint);   //����spr
	aJsBg->setTag(1000);
	this->addChild(aJsBg);
	this->addChild(jsSprite);
	if(isFollowRole){
		this->setVisible(false);
	}
	//this->Active();//����ҡ��
	 //��ֹ��㴥��
	return this;
}


void  KSkillRocker::setSpriteOpacity(int ver)
{
	if (jsSprite)
	  jsSprite->setOpacity(ver);
	if (bgSprite)
	  bgSprite->setOpacity(ver);
}

void  KSkillRocker::setSpriteVisible(bool ver)
{
	this->setVisible(ver);

}

void  KSkillRocker::setSpritePiont(Point aPoint)
{
	this->Active();
	centerPoint  = aPoint;
	currentPoint = centerPoint;              //�ƶ���ǰ������
    if (jsSprite)
	   jsSprite->setPosition(centerPoint);   //����spr
	if (bgSprite)
	   bgSprite->setPosition(centerPoint);   //����spr
}
