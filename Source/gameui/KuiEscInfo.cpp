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

//����ȷ��������
KuiEscInfo * KuiEscInfo::create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiEscInfo * popLayer = KuiEscInfo::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiEscInfo::init()
{
	if ( !Layer::init() ){
		return false;
	}
	//����֡����
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//���ģ̬����
	winSize = ax::Director::getInstance()->getWinSize();
	/*Color4B color(112, 128, 144, 150);
	LayerColor * colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	//colorLayer->setContentSize(winSize);//���úʹ��ڵĴ�С
	this->addChild(colorLayer);
	*/
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ2.spr");
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
	//��ñ���ͼƬ�Ĵ�С
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();

	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С
	//����������Ӧ

	this->scheduleUpdate();                   //���ú���
	auto listener = ax::EventListenerTouchOneByOne::create();
ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);   //��ֹ��㴥��
	return true;
}

void KuiEscInfo::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//ͳ˧
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
	t_sprintf(nSprName,"\\Spr\\Ui3\\������\\�����Ϣҳ\\����ҳ-�����Ϣ.spr");
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
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);  //���úʹ��ڵĴ�С
	ParentNode_Task->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	//red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	//red->retain();
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
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
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ���˳�.spr");
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
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ������.spr");
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
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ��ѡ��.spr");
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

	//\spr\Ui3\ϵͳ\ϵͳ�������й�.spr
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ�������й�.spr");
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


	//������Ϸ
	t_sprintf(nSprName,"\\spr\\Ui3\\ϵͳ\\ϵͳ������.spr");
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
	//����˵�
	Menu * menu = Menu::create(ExitGameConfirm,GameHelpConfirm,OptionsConfirm,OfflineConfirm,LeaveConfirm,NULL);
	menu->setAnchorPoint(ax::Vec2(0,0));
	menu->setPosition(ax::Vec2(0,0));             //�˵�λ������Ϊ����������
	ParentNode_Task->addChild(menu);         //�ӵ�������
}


//�����ť�ص�����
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
		case 4://����
			{
			  if(g_pCoreShell)
			     g_pCoreShell->OperationRequest(GOI_LIXIAN, 0, 0);  //������������
			   buttonCallBackFunc(NULL);
		    }
			break;
		case 5://������Ϸ
			 buttonCallBackFunc(NULL);
			break;
		default:
			break;
		}
	}
}
//ִ���ϲ����Ļص��������رյ�����
void KuiEscInfo::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node= dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiEscInfo::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiEscInfo::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
//	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,128, true);
}

Rect KuiEscInfo::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y

	return rc;
}

void KuiEscInfo::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//�ƶ���
void KuiEscInfo::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling || !__isCanMove) return;
	Point point   = ParentNode_Task->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff    = point - m_lastPoint;                  //����ͼƬ��λ��
	ParentNode_Task->setPosition(ParentNode_Task->getPosition() + diff);
	m_origin      = ParentNode_Task->getPosition();
	m_lastPoint = point;
}
//��������ccTouchBegan������true
bool KuiEscInfo::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	Point point = ParentNode_Task->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Task->getContentSize().width*/,winSize.height/* ParentNode_Task->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //����ڷ�Χ�� �Ϳ����϶�
	m_lastPoint = point;
	return true;
}

//--------------------------------------------------------------------------
//	���ܣ����¶�����Ϣ
//--------------------------------------------------------------------------
void KuiEscInfo::update(float delta)
{
	//ccMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	}
}

//void KuiEscInfo::draw()
//{
//	//ccMessageBox("����ѭ��","draw");
//}
