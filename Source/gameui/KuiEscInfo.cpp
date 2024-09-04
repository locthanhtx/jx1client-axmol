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

//创建确定弹出框
KuiEscInfo * KuiEscInfo::create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiEscInfo * popLayer = KuiEscInfo::create();
	popLayer->addDialogData();
	//设置按钮
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//初始化
bool KuiEscInfo::init()
{
	if ( !Layer::init() ){
		return false;
	}
	//精灵帧缓存
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//添加模态背景
	/*Color4B color(112, 128, 144, 150);
	LayerColor * colorLayer = LayerColor::create(color);//颜色层
	colorLayer->setPosition(ax::Vec2::ZERO);
	//colorLayer->setContentSize(winSize);//设置和窗口的大小
	this->addChild(colorLayer);
	*/
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\系统\\系统2.spr");
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
	//获得背景图片的大小
    Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();

	//colorLayer->setContentSize(contentSize);//设置和窗口的大小
	//开启触摸响应

	this->scheduleUpdate();
//    auto touchListener = EventListenerTouchOneByOne::create();
//    touchListener->onTouchBegan = AX_CALLBACK_2(KuiEscInfo::ccTouchBegan, this);
//    touchListener->onTouchMoved = AX_CALLBACK_2(KuiEscInfo::ccTouchMoved, this);
//    touchListener->onTouchEnded = AX_CALLBACK_2(KuiEscInfo::ccTouchEnded, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);   //?1¤7?1¤7?0?9?1¤7?1¤7?1?6?1¤7?1¤7?	return true;
}

void KuiEscInfo::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//统帅
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
	sprintf(nSprName,"\\Spr\\Ui3\\帮会界面\\帮会信息页\\帮会分页-帮会信息.spr");
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
	colorLayer = LayerColor::create(color);//颜色层
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);  //设置和窗口的大小
	ParentNode_Task->addChild(colorLayer,1000);
	//创建一个动作 改变精灵颜色
	//red = CCTintBy::create(0.2,0,-255,-255);//持续时间+颜色
	//red->retain();
}

//设置确定取消按钮,参数：调用层对象，调用层回调函数
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
	t_sprintf(nSprName,"\\spr\\Ui3\\系统\\系统－退出.spr");
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
	t_sprintf(nSprName,"\\spr\\Ui3\\系统\\系统－帮助.spr");
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
	t_sprintf(nSprName,"\\spr\\Ui3\\系统\\系统－选项.spr");
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

	//\spr\Ui3\系统\系统－离线托管.spr
	t_sprintf(nSprName,"\\spr\\Ui3\\系统\\系统－离线托管.spr");
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


	//返回游戏
	t_sprintf(nSprName,"\\spr\\Ui3\\系统\\系统－返回.spr");
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
	//加入菜单
    Menu * menu = Menu::create(ExitGameConfirm,GameHelpConfirm,OptionsConfirm,OfflineConfirm,LeaveConfirm,NULL);
	menu->setAnchorPoint(ax::Vec2(0,0));
	menu->setPosition(ax::Vec2(0,0));             //?1¤7?0?5?1¤7???1¤7?1¤7?1¤7?1¤7?1¤7?1¤7?0?2?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7
	ParentNode_Task->addChild(menu);         //?1¤7?0?3?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7
}


//点击按钮回调函数
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
		case 4://离线
			{
			  if(g_pCoreShell)
			     g_pCoreShell->OperationRequest(GOI_LIXIAN, 0, 0);  //发送离线请求
			   buttonCallBackFunc(NULL);
		    }
			break;
		case 5://返回游戏
			 buttonCallBackFunc(NULL);
			break;
		default:
			break;
		}
	}
}
//执行上层对象的回调函数，关闭弹出框
void KuiEscInfo::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node= dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//设置tag，在调用层可以获取到
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//关闭弹出框
void KuiEscInfo::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//重写触摸注册函数，重新给定触摸级别
void KuiEscInfo::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//?1¤7?1¤7?1¤7?1¤7?0?6?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7?0?7?1¤7?1¤7?1¤7?1¤7?1¤7?0?2?1¤7?1¤7?28?1¤7?1¤7?1¤7?1¤7CCMenu?0?4?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7?0?8?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7?1¤7?¤?1¤7?1¤7?0?6?1¤7?1¤7?1¤7
//	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,128, true);
}

Rect KuiEscInfo::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //这个因该是图内偏移
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //起点最左边
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //最上边    //- rc.origin.y

	return rc;
}

void KuiEscInfo::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//移动中
void KuiEscInfo::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling || !__isCanMove) return;
	Point point   = ParentNode_Task->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff    = point - m_lastPoint;                   //设置图片的位置
	ParentNode_Task->setPosition(ParentNode_Task->getPosition() + diff);
	m_origin      = ParentNode_Task->getPosition();
	m_lastPoint = point;
}
//触摸函数ccTouchBegan，返回true
bool KuiEscInfo::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	Point point = ParentNode_Task->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Task->getContentSize().width*/,winSize.height/* ParentNode_Task->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //如果在范围内 就可以拖动
	m_lastPoint = point;
	return true;
}

//--------------------------------------------------------------------------
//	功能：更新队伍信息
//--------------------------------------------------------------------------
void KuiEscInfo::update(float delta)
{
	//messageBox("无限循环","update");
	if (isOpen && g_pCoreShell)
	{
	}
}

//void KuiEscInfo::draw()
//{
//	//messageBox("无限循环","draw");
//}
