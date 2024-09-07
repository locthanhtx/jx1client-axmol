#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiEscInfo.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
//#include "engine/KPalette.h"
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
#include "gamecore/KItem.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;
KuiEscInfo::KuiEscInfo()
{
	isOpen = false;
	__isCanMove=true;
	m_bScrolling=false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	colorLayer        = NULL;
	red               = NULL;
	m_pMainUiLayer    = NULL;
	__nSelIndex       = -1;
}

KuiEscInfo::~KuiEscInfo()
{
	isOpen = false;
	colorLayer   = NULL;
	//CC_SAFE_RELEASE(m_array);
	if (red)
	    red->release();

}

//´´½¨È·¶¨µ¯³ö¿ò
KuiEscInfo* KuiEscInfo::create(const char* title, Ref* callbackListener, const std::function<void(ax::Node*)>& callfun)
{
	KuiEscInfo * popLayer = KuiEscInfo::create();
	popLayer->addDialogData();
	//ÉèÖÃ°´Å¥
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//³õÊ¼»¯
bool KuiEscInfo::init()
{
	if ( !Layer::init() ){
		return false;
	}
	//¾«ÁéÖ¡»º´æ
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//Ìí¼ÓÄ£Ì¬±³¾°
	/*Color4B color(112, 128, 144, 150);
	LayerColor * colorLayer = LayerColor::create(color);//ÑÕÉ«²ã
	colorLayer->setPosition(ax::Vec2::ZERO);
	//colorLayer->setContentSize(winSize);//ÉèÖÃºÍ´°¿ÚµÄ´óÐ¡
	this->addChild(colorLayer);
	*/
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\ÏµÍ³\\ÏµÍ³2.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;
	Sprite * spriteBox = Sprite::createWithTexture(bgCur);
	ParentNode_Task= spriteBox;
	spriteBox->setPosition(ax::Vec2(winSize.width/2,winSize.height/2));
	this->addChild(spriteBox);
	//»ñµÃ±³¾°Í¼Æ¬µÄ´óÐ¡
    Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();

	//colorLayer->setContentSize(contentSize);//ÉèÖÃºÍ´°¿ÚµÄ´óÐ¡
	//¿ªÆô´¥ÃþÏìÓ¦

	this->scheduleUpdate();
//    auto touchListener = EventListenerTouchOneByOne::create();
//    touchListener->onTouchBegan = AX_CALLBACK_2(KuiEscInfo::ccTouchBegan, this);
//    touchListener->onTouchMoved = AX_CALLBACK_2(KuiEscInfo::ccTouchMoved, this);
//    touchListener->onTouchEnded = AX_CALLBACK_2(KuiEscInfo::ccTouchEnded, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);   //?1¡è7?1¡è7?0?9?1¡è7?1¡è7?1?6?1¡è7?1¡è7?	return true;
}

void KuiEscInfo::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Í³Ë§
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	pMoneyLabel->setPosition(ax::Vec2(70,35));
	ParentNode_Task->addChild(pMoneyLabel,1);
*/
	m_pMainUiLayer = Layer::create();
	ParentNode_Task->addChild(m_pMainUiLayer);

	// Create the checkbox
	/*char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\Spr\\Ui3\\°ï»á½çÃæ\\°ï»áÐÅÏ¢Ò³\\°ï»á·ÖÒ³-°ï»áÐÅÏ¢.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	ImageView *titleImg = ImageView::create();
	titleImg->loadTextureByData(bgCur);
	titleImg->setPosition(ax::Vec2(m_size.width/2,m_size.height/2-15));
	m_pMainUiLayer->addChild(titleImg);

	*/
	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//ÑÕÉ«²ã
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);  //ÉèÖÃºÍ´°¿ÚµÄ´óÐ¡
	ParentNode_Task->addChild(colorLayer,1000);
	//´´½¨Ò»¸ö¶¯×÷ ¸Ä±ä¾«ÁéÑÕÉ«
	//red = CCTintBy::create(0.2,0,-255,-255);//³ÖÐøÊ±¼ä+ÑÕÉ«
	//red->retain();
}

//ÉèÖÃÈ·¶¨È¡Ïû°´Å¥,²ÎÊý£ºµ÷ÓÃ²ã¶ÔÏó£¬µ÷ÓÃ²ã»Øµ÷º¯Êý
void KuiEscInfo::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\ÏµÍ³\\ÏµÍ³£­ÍË³ö.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_ExitGame_normal = Sprite::createWithTexture(bgCur);
	sprite_ExitGame_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_ExitGame_select = Sprite::createWithTexture(bgCur);
	sprite_ExitGame_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * ExitGameConfirm = MenuItemSprite::create(sprite_ExitGame_normal, sprite_ExitGame_select, CC_CALLBACK_1(KuiEscInfo::btnCallBackFunc, this));
	ExitGameConfirm->setPosition(ax::Vec2(m_size.width/2,m_size.height-15));
	ExitGameConfirm->setTag(1);
	//
	t_sprintf(nSprName,"\\spr\\Ui3\\ÏµÍ³\\ÏµÍ³£­°ïÖú.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_GameHelp_normal = Sprite::createWithTexture(bgCur);
	sprite_GameHelp_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_GameHelp_select = Sprite::createWithTexture(bgCur);
	sprite_GameHelp_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * GameHelpConfirm = MenuItemSprite::create(sprite_GameHelp_normal, sprite_GameHelp_select, CC_CALLBACK_1(KuiEscInfo::btnCallBackFunc, this));
	GameHelpConfirm->setPosition(ax::Vec2(m_size.width/2,m_size.height-55));
	GameHelpConfirm->setTag(2);

	//
	t_sprintf(nSprName,"\\spr\\Ui3\\ÏµÍ³\\ÏµÍ³£­Ñ¡Ïî.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Options_normal = Sprite::createWithTexture(bgCur);
	sprite_Options_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Options_select = Sprite::createWithTexture(bgCur);
	sprite_Options_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * OptionsConfirm = MenuItemSprite::create(sprite_Options_normal, sprite_Options_select, CC_CALLBACK_1(KuiEscInfo::btnCallBackFunc, this));
	OptionsConfirm->setPosition(ax::Vec2(m_size.width/2,m_size.height-92));
	OptionsConfirm->setTag(3);

	//\spr\Ui3\ÏµÍ³\ÏµÍ³£­ÀëÏßÍÐ¹Ü.spr
	t_sprintf(nSprName,"\\spr\\Ui3\\ÏµÍ³\\ÏµÍ³£­ÀëÏßÍÐ¹Ü.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_offline_normal = Sprite::createWithTexture(bgCur);
	sprite_offline_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_offline_select = Sprite::createWithTexture(bgCur);
	sprite_offline_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * OfflineConfirm = MenuItemSprite::create(sprite_offline_normal, sprite_offline_select, CC_CALLBACK_1(KuiEscInfo::btnCallBackFunc, this));
	OfflineConfirm->setPosition(ax::Vec2(m_size.width/2,m_size.height-130));
	OfflineConfirm->setTag(4);


	//·µ»ØÓÎÏ·
	t_sprintf(nSprName,"\\spr\\Ui3\\ÏµÍ³\\ÏµÍ³£­·µ»Ø.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Leave_normal = Sprite::createWithTexture(bgCur);
	sprite_Leave_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Leave_select = Sprite::createWithTexture(bgCur);
	sprite_Leave_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * LeaveConfirm = MenuItemSprite::create(sprite_Leave_normal, sprite_Leave_select, CC_CALLBACK_1(KuiEscInfo::btnCallBackFunc, this));
	LeaveConfirm->setPosition(ax::Vec2(m_size.width/2,m_size.height-165));
	LeaveConfirm->setTag(5);
	//¼ÓÈë²Ëµ¥
    Menu * menu = Menu::create(ExitGameConfirm,GameHelpConfirm,OptionsConfirm,OfflineConfirm,LeaveConfirm,NULL);
	menu->setAnchorPoint(ax::Vec2(0,0));
	menu->setPosition(ax::Vec2(0,0));             //?1¡è7?0?5?1¡è7???1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?0?2?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7
	ParentNode_Task->addChild(menu);         //?1¡è7?0?3?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7
}


//µã»÷°´Å¥»Øµ÷º¯Êý
void KuiEscInfo::oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;

	switch (type)
	{
	case Widget::TouchEventType::BEGAN://Touch Down
		{
			buttonCallBackFunc(NULL);
		}
		break;
	case Widget::TouchEventType::MOVED://Touch Move
		break;
	case Widget::TouchEventType::ENDED://Touch Up
		{
			if (pSender)
			{
				Button* nTbtn = (Button*)pSender;
				//nTbtn->setTitleColor(ax::Color3B::WHITE);
				//if  (nTbtn->getTag()==1)
				//{

				//}
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void KuiEscInfo::btnCallBackFunc(Ref * pSender)
{
	if (!g_pCoreShell|| !g_GameWorld) return;

	Node * node = NULL;
	if (pSender)
	{
		node= dynamic_cast<Node *>(pSender);
		switch(node->getTag())
		{
		case 1:
			{
			   if (g_GameWorld)
				  g_GameWorld->_quitGame();
				buttonCallBackFunc(NULL);
			}
			break;
		case 2:
			break;
		case 3:
			break;
		case 4://ÀëÏß
			{
			  if(g_pCoreShell)
			     g_pCoreShell->OperationRequest(GOI_LIXIAN, 0, 0);  //·¢ËÍÀëÏßÇëÇó
			   buttonCallBackFunc(NULL);
		    }
			break;
		case 5://·µ»ØÓÎÏ·
			 buttonCallBackFunc(NULL);
			break;
		default:
			break;
		}
	}
}
//Ö´ÐÐÉÏ²ã¶ÔÏóµÄ»Øµ÷º¯Êý£¬¹Ø±Õµ¯³ö¿ò
void KuiEscInfo::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node= dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//ÉèÖÃtag£¬ÔÚµ÷ÓÃ²ã¿ÉÒÔ»ñÈ¡µ½
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//¹Ø±Õµ¯³ö¿ò
void KuiEscInfo::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//ÖØÐ´´¥Ãþ×¢²áº¯Êý£¬ÖØÐÂ¸ø¶¨´¥Ãþ¼¶±ð
void KuiEscInfo::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//?1¡è7?1¡è7?1¡è7?1¡è7?0?6?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?0?7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?0?2?1¡è7?1¡è7?28?1¡è7?1¡è7?1¡è7?1¡è7CCMenu?0?4?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?0?8?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?1¡è7?¡è?1¡è7?1¡è7?0?6?1¡è7?1¡è7?1¡è7
//	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,128, true);
}

Rect KuiEscInfo::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //Õâ¸öÒò¸ÃÊÇÍ¼ÄÚÆ«ÒÆ
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //Æðµã×î×ó±ß
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //×îÉÏ±ß    //- rc.origin.y

	return rc;
}

void KuiEscInfo::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//ÒÆ¶¯ÖÐ
void KuiEscInfo::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling || !__isCanMove) return;
	Point point   = ParentNode_Task->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff    = point - m_lastPoint;                   //ÉèÖÃÍ¼Æ¬µÄÎ»ÖÃ
	ParentNode_Task->setPosition(ParentNode_Task->getPosition() + diff);
	m_origin      = ParentNode_Task->getPosition();
	m_lastPoint = point;
}
//´¥Ãþº¯ÊýccTouchBegan£¬·µ»Øtrue
bool KuiEscInfo::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	Point point = ParentNode_Task->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Task->getContentSize().width*/,winSize.height/* ParentNode_Task->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //Èç¹ûÔÚ·¶Î§ÄÚ ¾Í¿ÉÒÔÍÏ¶¯
	m_lastPoint = point;
	return true;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¸üÐÂ¶ÓÎéÐÅÏ¢
//--------------------------------------------------------------------------
void KuiEscInfo::update(float delta)
{
	//messageBox("ÎÞÏÞÑ­»·","update");
	if (isOpen && g_pCoreShell)
	{
	}
}

//void KuiEscInfo::draw()
//{
//	//messageBox("ÎÞÏÞÑ­»·","draw");
//}
