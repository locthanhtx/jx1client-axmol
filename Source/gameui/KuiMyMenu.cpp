#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiMyMenu.h"
#include "KuiItem.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gamecore/KPlayer.h"
#include "engine/KCodec.h"
#include "engine/KCodecLzo.h"
#include "engine/KFile.h"
#include "engine/KFilePath.h"
#include "engine/KList.h"
#include "engine/KMemBase.h"
#include "engine/KMemClass.h"
#include "engine/KMemStack.h"
#include "engine/KNode.h"
#include "engine/KPakFile.h"
#include "engine/KStrBase.h"
#include "engine/KPakList.h"
#include "engine/XPackFile.h"
#include "gamecore/KSkillManager.h"
#include "gameui/KuiSkilldesc.h"
#include "gameui/KCheckbox.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

KuiMyMenu::KuiMyMenu()
{
	isFollowRole =false;
	active = false;
	isRun  = false;
	radius = 60;
	nRolePoint=ax::Vec2(0,0);
	nRoleIndex = 0;

	jsSprite=NULL;
	bgSprite=NULL;
	_BackAttackRadius=0;
	_CurSkillId=0;
	_Beilv=0;
	_useSkillModel=0;
}

KuiMyMenu::~KuiMyMenu()
{
	/*if  (bgSprite)
	    bgSprite->release();
	if (jsSprite)
	    jsSprite->release();*/
}

void KuiMyMenu::updatePos(float dt){
	if  (jsSprite)
	    jsSprite->setPosition(jsSprite->getPosition() + currentPoint - jsSprite->getPosition() * 0.5);
}
//����ҡ��
void KuiMyMenu::Active()
{
	if (!active)
	{
		active=true;
        //[TODO][zer0kull]
		this->schedule(CC_SCHEDULE_SELECTOR(KuiMyMenu::updatePos));                       //���ˢ�º���
		////ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,0,false);
	}else {
	}
}
//���ҡ��
void   KuiMyMenu::Inactive()
{
	if (active) {
		active=false;
		this->unschedule(CC_SCHEDULE_SELECTOR(KuiMyMenu::updatePos));                 //ɾ��ˢ��
//		ax::Director::getInstance()->getTouchDispatcher()->removeDelegate(this);//ɾ��ί��
        auto dispatcher = ax::Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListenersForTarget(this);
	}else {
	}
}


//�ڶ������������������֮��ļн�
//��Ϊ����ĽǶ����Ԧ�Ϊ��λ����������Ҫת�����Զ�Ϊ��λ
float KuiMyMenu::getAngleSigned()
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

float KuiMyMenu::getRad(Point p1,Point p2)
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
Point KuiMyMenu::getDirection()
{
	return (centerPoint - currentPoint).getNormalized();
}
//ҡ�˺����ĵ�ľ���
float KuiMyMenu::getVelocity()
{
	return centerPoint.distance(currentPoint);
}

KuiMyMenu * KuiMyMenu::create(MenuItem* item, ...)
{
	va_list args;
	va_start(args,item);
	KuiMyMenu *pRet = new KuiMyMenu();
	if (pRet && pRet->createWithItems(item, args))
	{
		pRet->autorelease();
		va_end(args);
		//��ʼ����һ������ң��
        //[TODO][zer0kull]
//		pRet->setTouchPriority(kCCMenuHandlerPriority);
//		pRet->setTouchMode(kCCTouchesOneByOne);
		pRet->setEnabled(true);
		pRet->setIgnoreAnchorPointForPosition(true);
		pRet->setAnchorPoint(ax::Vec2(0.5f, 0.5f));
		pRet->_selectedItem = NULL;
//		pRet->_state = Menu::State::WAITING;

		pRet->initWithCenter();
		return pRet;
	}
	va_end(args);
	CC_SAFE_DELETE(pRet);
	return NULL;
}


KuiMyMenu* KuiMyMenu::initWithCenter(){

	visibleSize = ax::Director::getInstance()->getVisibleSize();//�ɼ���
	//pCoreShell = NULL;
	//__GameWorld = NULL;
	moveEndPoint = ax::Vec2(0,0);

	if(!isFollowRole){
		centerPoint =ax::Vec2(visibleSize.width,-100.0f);
	}else{
		centerPoint =ax::Vec2(visibleSize.width,-100.0f);  //��������
	}
	currentPoint = centerPoint; //�ƶ���ǰ������

	/*Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setContentSize(ax::Size(200,200));//���úʹ��ڵĴ�С
	colorLayer->setPosition(CCPointMake(visibleSize.width-200,10));
	colorLayer->setAnchorPoint(ax::Vec2(0,0));
	//colorLayer->addChild(spriteBox);
	this->addChild(colorLayer);*/

	bgSprite = Sprite::create("useskill_b.png");
	bgSprite->setPosition(centerPoint);   //����spr

	jsSprite = Sprite::create("useskill_z.png");
	jsSprite->setPosition(centerPoint);//����spr
	//bgSprite->setTag(1000);
	//bgSprite->retain();
	//jsSprite->retain();

    auto mItemBgSprite = MenuItemSprite::create(bgSprite, bgSprite);
    auto mItemJsSprite = MenuItemSprite::create(jsSprite, jsSprite);

	this->addChild(mItemBgSprite);
	this->addChild(mItemJsSprite);
	if(isFollowRole){
		//this->setVisible(false);
		setSpriteVisible(false);
	}
	this->Active();//����ҡ��
	 //��ֹ��㴥��
	return this;
}


KuiMyMenu* KuiMyMenu::menuWithItem(MenuItem* item)
{
	return create(item, NULL);
}


// ��дregisterWithTouchDispatcher
void KuiMyMenu::registerWithTouchDispatcher()
{
	//�������ȼ���Ϊ1��ֻҪ��CCScrollView��tableView��CCControlButton�ȣ���Щ���ȼ���Ϊ0���;Ϳ���
	////ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-129,true);
//	Director* pDirector = ax::Director::getInstance();
//	pDirector->getTouchDispatcher()->addTargetedDelegate(this, this->getTouchPriority(),true);//false
}


bool KuiMyMenu::ccTouchBegan(Touch* touch, Event* event)
{
	//��ִ��ң��
	//messageBox("KuiMyMenu::ccTouchBegan","Touch");

	if (!active)
		return false;
	if (g_GameWorld && g_pCoreShell && g_pCoreShell->getTradeState()) return false;

	_useSkillModel=g_GameWorld->getUseSkillModel();//0 Ϊ�Զ� 1 Ϊ�ֶ�

	//-------------------------------------------------
	AX_UNUSED_PARAM(event);
	if (_state != Menu::State::WAITING || !isEnabled())
	{
		return false;
	}

	for (Node *c = this->_parent; c != NULL; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}

	_selectedItem = this->getItemForTouch(touch, Camera::getDefaultCamera());
	if (_selectedItem)
	{
		//messageBox("_selectedItem Began","_selectedItem");
		if (_useSkillModel==0) //������Զ� ��ִ��
		   _selectedItem->activate();//ִ�лص�����

		_state = Menu::State::TRACKING_TOUCH;
		if (dynamic_cast<MenuItemToggle*>(_selectedItem) && _selectedItem->getTag() == std::hash<std::string>{}("box"))//
		{
			if (_selectedItem->isSelected())
                _selectedItem->unselected();
			else
                _selectedItem->selected();
		}
		else
            _selectedItem->selected();

		setSpriteOrder(0);
		setSpritePiont(_selectedItem->getPosition());
		setSpriteVisible(true);
		setSpriteOpacity(105);
		if (_useSkillModel==1) //��������Զ� ��ִ��
		{
			g_GameWorld->setAttackSprPosition(ax::Vec2(visibleSize.width/2,visibleSize.height/2));//nRolePoint
			g_GameWorld->setAttackSprVisible(true);
		}

		Point touchPoint = touch->getLocationInView();     //��ȡ����������
		touchPoint = Director::getInstance()->convertToUI(touchPoint); //ת����Ļ����
		if(!isFollowRole){
			if (touchPoint.distance(centerPoint) > radius){
				return false;
			}
		}
		currentPoint = touchPoint;
		if(isFollowRole){
			centerPoint=currentPoint;
			jsSprite->setPosition(currentPoint);
			bgSprite->setPosition(currentPoint);  //this->getChildByTag(1000)
		}
		//��ȡ���ܵķ�Χ
		if (_useSkillModel==1) //��������Զ� ��ִ��
		{
		  _CurSkillId = _selectedItem->getTag();
		  if (_CurSkillId<=0 || _CurSkillId>=MAX_SKILL) return false;
		  ISkill * pOrdinSkill = g_SkillManager.GetSkill(_CurSkillId, 1);
		  if (pOrdinSkill && !pOrdinSkill->IsAura())
		  {
			_BackAttackRadius = pOrdinSkill->getBackAttackRadius();
		  }
		  _Beilv=_BackAttackRadius/radius;
		}
		return true;
	}
	return false;
}

void KuiMyMenu::ccTouchMoved(Touch* touch, Event* event)
{

	if (g_GameWorld && g_pCoreShell && g_pCoreShell->getTradeState()) return;
	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,false);
	//int nBeilv = 2;
	Point touchPoint = touch->getLocationInView();                     //��Ļ����ϵ +x ���� +������
	touchPoint = Director::getInstance()->convertToUI(touchPoint); //ת�������½�gl�������(0,0)
	float nDistance = touchPoint.distance(centerPoint);
	if (nDistance > radius)
	{//������Χ
		currentPoint = centerPoint + (touchPoint - centerPoint).getNormalized() * radius;
	}else {
		currentPoint = touchPoint;
	}

	AX_UNUSED_PARAM(event);
	CCAssert(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchMoved] -- invalid state");
	MenuItem *currentItem = this->getItemForTouch(touch, Camera::getDefaultCamera());
	if (currentItem != _selectedItem)
	{
		if (_selectedItem)
		{
			_selectedItem->unselected();
		}
		_selectedItem = currentItem;
		if (_selectedItem)
		{
			_selectedItem->selected();
		}
		//---�������ͬ�İ�ť�ͷ���
		return;
	}

	//��ʼ��⼼���ƶ���λ�� ������������
	//currentPoint ��ǰ������λ�� centerPoint����Բ���ԭʼλ��
	float nVer = getAngleSigned();  //���ĵ��봥����ĽǶ�
	float nDestPontX,nDestPontY;

	if (nVer==0)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*_Beilv;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/nRolePoint.y;
	}
	else if(nVer>0 && nVer<90)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*_Beilv;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y+(currentPoint.y-centerPoint.y)*_Beilv);
	}
	else if (nVer==90)
	{//����
		nDestPontX = nRolePoint.x;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y+(currentPoint.y-centerPoint.y)*_Beilv);
	}
	else if (nVer>90 && nVer<180)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*_Beilv;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y+(currentPoint.y-centerPoint.y)*_Beilv);
	}
	else if (nVer==180)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*_Beilv;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/nRolePoint.y;
	}
	else if (nVer>180 && nVer<270)
	{//����
		nDestPontX = nRolePoint.x-(centerPoint.x-currentPoint.x)*_Beilv;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y-(centerPoint.y-currentPoint.y)*_Beilv);
	}
	else if (nVer==270)
	{//����
		nDestPontX = nRolePoint.x;//Ŀ��gl����x y����
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y-(centerPoint.y-currentPoint.y)*_Beilv);
	}
	else if (nVer>270 && nVer<360)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*_Beilv;//
		nDestPontY = /*visibleSize.height-*/(nRolePoint.y-(centerPoint.y-currentPoint.y)*_Beilv);
	}
	else if (nVer==360)
	{//����
		nDestPontX = nRolePoint.x+(currentPoint.x-centerPoint.x)*_Beilv;//
		nDestPontY = /*visibleSize.height-*/nRolePoint.y;
	}
	isRun  = false;
	moveEndPoint.x = nDestPontX;
	moveEndPoint.y = nDestPontY;
	if (_useSkillModel==1) //��������Զ� ��ִ��
	{
		g_GameWorld->setAttackSprPosition(moveEndPoint);
	}

	//int nX = nDestPontX;
	//int nY = nDestPontY;
	//int nZ = 0;

	//SubWorld[0].GetMpsByLocalPosition(nX, nY, nZ);  //����Ļ���� ���� ת����������
	//g_pCoreShell->setNpcDir(nX,nY);
	//g_pCoreShell->GotoWhere(nDestPontX,nDestPontY,0); //������������
}

void KuiMyMenu::ccTouchEnded(Touch* touch, Event* event)
{

	if (g_GameWorld && g_pCoreShell && g_pCoreShell->getTradeState()) return;

	//if (pCoreShell) pCoreShell->setPadCanMove(PAD_KIND_ITEM,true);
	currentPoint = centerPoint;
	if(isFollowRole){
	//this->setVisible(false);
	   setSpriteVisible(false);
	}
	isRun = true;
	//this->setVisible(false);
	//setSpriteOpacity(100);
	setSpritePiont(ax::Vec2(visibleSize.width,-100.0f));

	g_GameWorld->setAttackSprVisible(false);
	//this->Inactive();
	//messageBox("�ƶ�����","�ƶ�����");

	AX_UNUSED_PARAM(touch);
	AX_UNUSED_PARAM(event);
	CCAssert(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchEnded] -- invalid state");
	if (_selectedItem)
	{
        if (dynamic_cast<MenuItemToggle*>(_selectedItem))
		{//�����ʹbox
			if  (_selectedItem->getTag() != std::hash<std::string>{}("box"))
				_selectedItem->unselected();
		}
		else
			_selectedItem->unselected();

		//_selectedItem->activate();//ִ�лص�����
	}
	_state = Menu::State::WAITING;

	//�ֶ����ƿ�ʼ����
	if (_useSkillModel==1) //������ֶ� ��ִ��
	{
		g_GameWorld->customSkillAttack(_CurSkillId,moveEndPoint.x,visibleSize.height-moveEndPoint.y);
		_Beilv=0;
		_BackAttackRadius=0;
		_useSkillModel=0;
		_CurSkillId=0;
	}
}

void KuiMyMenu::ccTouchCancelled(Touch *touch, Event* event)
{
	AX_UNUSED_PARAM(touch);
	AX_UNUSED_PARAM(event);
	CCAssert(_state == Menu::State::TRACKING_TOUCH, "[Menu ccTouchCancelled] -- invalid state");
	if (_selectedItem)
	{
		_selectedItem->unselected();
	}
	_state = Menu::State::WAITING;
}


void  KuiMyMenu::setSpriteOpacity(int ver)
{
	if (jsSprite)
		jsSprite->setOpacity(ver);
	if (bgSprite)
		bgSprite->setOpacity(ver);
}

void  KuiMyMenu::setSpriteVisible(bool ver)
{
	if (jsSprite)
		jsSprite->setVisible(ver);
	if (bgSprite)
		bgSprite->setVisible(ver);

}

void  KuiMyMenu::setSpriteOrder(int ver)
{
	if  (ver<=0)
	{
		if (bgSprite)
			bgSprite->setLocalZOrder(0);
		if (jsSprite)
			jsSprite->setLocalZOrder(0);

	}
	else
	{
		if (bgSprite)
			bgSprite->setLocalZOrder(ver);
		if (jsSprite)
			jsSprite->setLocalZOrder(ver);
	}
}

void  KuiMyMenu::setSpritePiont(Point aPoint)
{
	this->Active();
	centerPoint  = aPoint;
	currentPoint = centerPoint;              //�ƶ���ǰ������
	if (jsSprite)
		jsSprite->setPosition(centerPoint);   //����spr
	if (bgSprite)
		bgSprite->setPosition(centerPoint);   //����spr
}
