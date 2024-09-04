#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiTeam.h"
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

KuiTeam::KuiTeam()
{
	isOpen = false;
	__isCanMove=true;
	m_bScrolling=false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	pMoneyLabel       = NULL;
	colorLayer        = NULL;
	red               = NULL;
	m_pUiLayer = NULL;
	//m_pWidget  = NULL;
	//m_array    = NULL;
	pNearbyPlayerslistView = NULL;
	pPlayersListView       = NULL;
	m_pNearbyPlayersList   = NULL;
	m_pPlayersList       = NULL;
	__nSelIndex          = -1;
	__playrSelIndex      = -1;
	__playerListCount    = 0;
	checkBox =NULL;
}

KuiTeam::~KuiTeam()
{
	isOpen = false;
	pMoneyLabel  = NULL;
	colorLayer   = NULL;
	__nSelIndex  = -1;
	__NearbyListCount = 0;
	if (m_pNearbyPlayersList)
	{
		free(m_pNearbyPlayersList);
		m_pNearbyPlayersList =NULL;
	}

	if (m_pPlayersList)
	{
		free(m_pPlayersList);
		m_pPlayersList =NULL;
	}

	//CC_SAFE_RELEASE(m_array);
	if (red)
	    red->release();

}

//����ȷ��������
KuiTeam * KuiTeam::create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiTeam * popLayer = KuiTeam::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiTeam::init()
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
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\���2.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;
	Sprite * spriteBox = Sprite::createWithTexture(bgCur);
	ParentNode_Team= spriteBox;
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
ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this); //��ֹ��㴥��
	return true;
}

void KuiTeam::addDialogData()
{
	if  (!g_pCoreShell) return;

	btnSize.width = 70;
	btnSize.height= 14;

	pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//ͳ˧
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	pMoneyLabel->setPosition(ax::Vec2(70,35));
	ParentNode_Team->addChild(pMoneyLabel,1);

	m_pUiLayer = Layer::create();
	ParentNode_Team->addChild(m_pUiLayer);

	// Create the checkbox
	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\��ӿ���.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Texture2D * check_box_normal = bgCur;

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Texture2D * check_box_active = bgCur;

	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Texture2D * check_box_active_disable = bgCur;
	checkBox = CheckBox::create();
	checkBox->setTouchEnabled(true);
    //[TODO][zer0kull]
//	checkBox->loadTexturesBydata(check_box_normal,
//		check_box_active_disable,
//		check_box_active_disable,check_box_normal,check_box_active);
	checkBox->setPosition(ax::Vec2(90,13));
	checkBox->addEventListener(CC_CALLBACK_2(KuiTeam::selectedBoxStateEvent, this));
	m_pUiLayer->addChild(checkBox);
	/*m_pWidget = Layout::create();
	m_pWidget->setBackGroundImage("loadingEnd.png");
	m_pWidget->setBackGroundImageScale9Enabled(true);
	m_pWidget->setBackGroundColorOpacity(105);
	m_pWidget->setContentSize(m_size);
	m_pUiLayer->addChild(m_pWidget);
	*/
	// create list view data
	/*m_array = CCArray::create();
	CC_SAFE_RETAIN(m_array);
	for (int i = 0; i < 16; ++i)
	{
		CCString* ccstr = CCString::createWithFormat("����б�_%d", i);
		m_array->addObject(ccstr);
	}*/
	//Size widgetSize = m_pWidget->getSize();
	// Create the list view


	pPlayersListView = ListView::create();
	// set list view direction
	pPlayersListView->setDirection(ListView::Direction::VERTICAL);
	pPlayersListView->setTouchEnabled(true);
	pPlayersListView->setBounceEnabled(true);
	pPlayersListView->setBackGroundImage("loadingStart.png");
	pPlayersListView->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pPlayersListView->setBackGroundImageOpacity(50);
	pPlayersListView->setContentSize(ax::Size(110, 110));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	pPlayersListView->setPosition(ax::Vec2(10,m_size.height/2-50));
	pPlayersListView->addEventListener(CC_CALLBACK_2(KuiTeam::selectedPlayersListItemEvent, this));
	m_pUiLayer->addChild(pPlayersListView);
	UpdateData(NULL);
	pPlayersListView->setGravity(ListView::Gravity::CENTER_VERTICAL);
	pPlayersListView->forceDoLayout();
	pPlayersListView->jumpToBottom();
	//��Χ���
	pNearbyPlayerslistView = ListView::create();
	// set list view direction
	pNearbyPlayerslistView->setDirection(ListView::Direction::VERTICAL);
	pNearbyPlayerslistView->setTouchEnabled(true);
	pNearbyPlayerslistView->setBounceEnabled(true);
	pNearbyPlayerslistView->setBackGroundImage("loadingStart.png");
	pNearbyPlayerslistView->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pNearbyPlayerslistView->setBackGroundImageOpacity(50);
	pNearbyPlayerslistView->setContentSize(ax::Size(110, 110));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	pNearbyPlayerslistView->setPosition(ax::Vec2(m_size.width/2+40,m_size.height/2-50));
	pNearbyPlayerslistView->addEventListener(CC_CALLBACK_2(KuiTeam::selectedItemEvent, this));
	m_pUiLayer->addChild(pNearbyPlayerslistView);
	OnRefresh();
	/*int count = m_array->count();
	// add custom item
	for (int i = 0; i < count/2; ++i)
	{
		Button* custom_button = Button::create();
		custom_button->setName("TextButton");
		custom_button->setTouchEnabled(true);
		custom_button->loadTextures("ui/state/life_normal.png", "ui/state/life_pk.png", "");
		custom_button->setScale9Enabled(true);
		custom_button->setContentSize(btnSize);

		Layout *custom_item = Layout::create();
		custom_item->setContentSize(custom_button->getSize());
		custom_button->setPosition(ax::Vec2(custom_item->getSize().width / 2, custom_item->getSize().height / 2));
		custom_item->addChild(custom_button);

		listView->pushBackCustomItem(custom_item);
	}
	// insert custom item
	CCArray *items = listView->getItems();
	int items_count = items->count();
	for (int i = 0; i < items_count; ++i)
	{
		Widget *item = listView->getItem(i);
		Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
		int index = listView->getIndex(item);
		button->setTitleFontSize(12);
		//static_cast<CCString*>(m_array->objectAtIndex(index))->getCString()
		char nTempstr[32];   //static_cast<CCString*>(
		t_sprintf(nTempstr,static_cast<CCString*>(m_array->objectAtIndex(index))->getCString());
		button->setTitleText(UTEXT(nTempstr,1));
	}
	*/
	pNearbyPlayerslistView->setGravity(ListView::Gravity::CENTER_VERTICAL);
	pNearbyPlayerslistView->forceDoLayout();
	pNearbyPlayerslistView->jumpToBottom();


	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);  //���úʹ��ڵĴ�С
	ParentNode_Team->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	//red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	//red->retain();
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiTeam::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\���\\�ر�.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_colse_normal = Sprite::createWithTexture(bgCur);
	sprite_colse_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_colse_select = Sprite::createWithTexture(bgCur);
	sprite_colse_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * colseConfirm = MenuItemSprite::create(sprite_colse_normal, sprite_colse_select, CC_CALLBACK_1(KuiTeam::buttonCallBackFunc, this));
	colseConfirm->setPosition(ax::Vec2(177+177/2,m_size.height-207-sprite_colse_select->getContentSize().height/2));
	//�������
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\�������.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Invite_normal = Sprite::createWithTexture(bgCur);
	sprite_Invite_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_Invite_select = Sprite::createWithTexture(bgCur);
	sprite_Invite_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * InviteConfirm = MenuItemSprite::create(sprite_Invite_normal, sprite_Invite_select, CC_CALLBACK_1(KuiTeam::btnCallBackFunc, this));
	InviteConfirm->setPosition(ax::Vec2(m_size.width/2,163));
	InviteConfirm->setTag(1);

	//�߳����� \Spr\Ui3\���\�������.spr
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\�߳�����.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Kick_normal = Sprite::createWithTexture(bgCur);
	sprite_Kick_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_Kick_select = Sprite::createWithTexture(bgCur);
	sprite_Kick_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * KickConfirm = MenuItemSprite::create(sprite_Kick_normal, sprite_Kick_select, CC_CALLBACK_1(KuiTeam::btnCallBackFunc, this));
	KickConfirm->setPosition(ax::Vec2(m_size.width/2,143));
	KickConfirm->setTag(2);
	//�ӳ��ƽ� \Spr\Ui3\���\�ӳ��ƽ�.spr
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\�ӳ��ƽ�.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Appoint_normal = Sprite::createWithTexture(bgCur);
	sprite_Appoint_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_Appoint_select = Sprite::createWithTexture(bgCur);
	sprite_Appoint_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * AppointConfirm = MenuItemSprite::create(sprite_Appoint_normal, sprite_Appoint_select, CC_CALLBACK_1(KuiTeam::btnCallBackFunc, this));
	AppointConfirm->setPosition(ax::Vec2(m_size.width/2,123));
	AppointConfirm->setTag(3);
	//ˢ��
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\ˢ���б�.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Refresh_normal = Sprite::createWithTexture(bgCur);
	sprite_Refresh_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Refresh_select = Sprite::createWithTexture(bgCur);
	sprite_Refresh_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * RefreshConfirm = MenuItemSprite::create(sprite_Refresh_normal, sprite_Refresh_select, CC_CALLBACK_1(KuiTeam::btnCallBackFunc, this));
	RefreshConfirm->setPosition(ax::Vec2(m_size.width/2,103));
	RefreshConfirm->setTag(4);

	//�뿪����
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���\\�뿪����.spr");
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

	MenuItemSprite * LeaveConfirm = MenuItemSprite::create(sprite_Leave_normal, sprite_Leave_select, CC_CALLBACK_1(KuiTeam::btnCallBackFunc, this));
	LeaveConfirm->setPosition(ax::Vec2(m_size.width/2,83));
	LeaveConfirm->setTag(5);
	//����˵�
	Menu * menu = Menu::create(colseConfirm,InviteConfirm,KickConfirm,AppointConfirm,RefreshConfirm,LeaveConfirm,NULL);
	//menu->setPosition(ax::Vec2(bgSpritePoint.x-m_size.width/2,bgSpritePoint.y-m_size.height/2));//�˵�λ������Ϊ����������
	menu->setAnchorPoint(ax::Vec2(0,0));
	menu->setPosition(ax::Vec2(0,0));             //�˵�λ������Ϊ����������
	ParentNode_Team->addChild(menu);         //�ӵ�������

	KUiPlayerTeam mTeam;
	ZeroMemory(&mTeam,sizeof(mTeam));
	if (g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (uintptr_t)&mTeam, 0)) //û�ж��� �ʹ���һ֧����
	{//�ж���
		if (mTeam.bTeamLeader)
		{//�ӳ�
			//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("��ʾ:�Ѿ��Ƕӳ�,����ѡ���Ա��");
			KickConfirm->setEnabled(true);
			AppointConfirm->setEnabled(true);
			LeaveConfirm->setEnabled(true);
		}
		else
		{//��Ա
			LeaveConfirm->setEnabled(true);
			KickConfirm->setEnabled(false);
			AppointConfirm->setEnabled(false);
		}
	}
	else
	{//û�ж���
		KickConfirm->setEnabled(false);
		AppointConfirm->setEnabled(false);
		LeaveConfirm->setEnabled(false);
	}
}

void KuiTeam::btnCallBackFunc(Ref * pSender)
{
	if (!g_pCoreShell|| !g_GameWorld || !pNearbyPlayerslistView) return;

	Node * node = NULL;
	if (pSender)
	{
		node= dynamic_cast<Node *>(pSender);
		switch(node->getTag())
		{
		case 1:
			{//�������
				if (__nSelIndex >= 0 && __nSelIndex<__NearbyListCount)
				{
					if (m_Info.cNumMember == 0)
						g_pCoreShell->TeamOperation(TEAM_OI_CREATE, 0, 0);

					g_pCoreShell->TeamOperation(TEAM_OI_INVITE,	(uintptr_t)(&m_pNearbyPlayersList[__nSelIndex]),0);
				}
			}
			break;
		case 2:
			{//�߳�����
				if (m_pPlayersList && __playrSelIndex >= 0 && __playrSelIndex<__playerListCount)
					g_pCoreShell->TeamOperation(TEAM_OI_KICK, (uintptr_t)&m_pPlayersList[__playrSelIndex],0);
			}
			break;
		case 3:
			{//�ӳ��ƽ�
				if (m_pPlayersList && __playrSelIndex >= 0 && __playrSelIndex<__playerListCount)
					g_pCoreShell->TeamOperation(TEAM_OI_APPOINT, (uintptr_t)&m_pPlayersList[__playrSelIndex], 0);
			}
			break;
		case 4:
			{//ˢ��
               pNearbyPlayerslistView->removeAllItems();
			   pNearbyPlayerslistView->setPosition(ax::Vec2(m_size.width/2+40,m_size.height/2-50));
			   OnRefresh();
			   pNearbyPlayerslistView->setGravity(ListView::Gravity::CENTER_VERTICAL);//�ж���
			   pNearbyPlayerslistView->forceDoLayout();
			}
			break;
		case 5:
			{//�˳�����
				g_pCoreShell->TeamOperation(TEAM_OI_LEAVE, 0, 0);
				break;

			   pNearbyPlayerslistView->removeItem(0);//ɾ����һ��
			   //listView->sortAllChildren();
			   pNearbyPlayerslistView->forceDoLayout();
			   //CCArray *items = listView->getItems();
			   //int items_count = items->count();
			   Size btnNewSize;
			   btnNewSize.width = 140;
			   btnNewSize.height= 28;

			   Button* custom_button = Button::create();
			   custom_button->setName("TextButton");
			   custom_button->setTouchEnabled(true);
			   custom_button->loadTextures("ui/state/life_normal.png", "ui/state/life_pk.png", "");
			   custom_button->setScale9Enabled(true);
			   custom_button->setContentSize(btnNewSize);

			   Layout *custom_item = Layout::create();
			   custom_item->setContentSize(btnNewSize);
			   custom_button->setPosition(ax::Vec2(btnNewSize.width / 2, btnNewSize.height / 2));
			   custom_item->addChild(custom_button);
			   pNearbyPlayerslistView->pushBackCustomItem(custom_item);
			   custom_button->setTitleText(UTEXT("���Բ���",1));

			   pNearbyPlayerslistView->forceDoLayout();
			   pNearbyPlayerslistView->jumpToBottom();
			}
			break;
		default:
			break;
		}
	}
	// remove last item
	// remove item by index
	//items_count = items->count();
	//listView->removeItem(items_count - 1);
}
//ִ���ϲ����Ļص��������رյ�����
void KuiTeam::buttonCallBackFunc(Ref * pSender)
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
void KuiTeam::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

void KuiTeam::selectedBoxStateEvent(Ref* pSender, CheckBox::EventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;
	switch (type)
	{
	case CheckBox::EventType::UNSELECTED:
		{
		if (!g_pCoreShell->TeamOperation(TEAM_OI_CLOSE, 0,0))
		    break;

		if  (checkBox)
			checkBox->setSelected(false);
		}
		break;
	case CheckBox::EventType::SELECTED:
		//m_pDisplayValueLabel->setText(CCString::createWithFormat("Selected")->getCString());
		//ccMessageBox("ѡ��","ѡ��");
		if (!g_pCoreShell->TeamOperation(TEAM_OI_CLOSE,0,1))
		{//��ʧ��
		   if  (checkBox)
			   checkBox->setSelected(false);
		}
		break;
	default:
		break;
	}
}
void KuiTeam::selectedPlayersListItemEvent(Ref *pSender, ListView::EventType type)
{
	char msg[64];
	switch (type)
	{
	case ListView::EventType::ON_SELECTED_ITEM_START:
		{
			ListView* listViewEx = static_cast<ListView*>(pSender);
			if (listViewEx)
			   __playrSelIndex = listViewEx->getCurSelectedIndex();
				//CCLOG("select child start index = %d", listViewEx->getCurSelectedIndex());
			break;
		}
	case ListView::EventType::ON_SELECTED_ITEM_END:
		break;
	default:
		break;
	}
}
void KuiTeam::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
	switch (type)
	{
	case ListView::EventType::ON_SELECTED_ITEM_START:
		{
			ListView* listViewEx = static_cast<ListView*>(pSender);
			if (listViewEx)
				__nSelIndex = listViewEx->getCurSelectedIndex();
				//CCLOG("select child start index = %d", listViewEx->getCurSelectedIndex());
			break;
		}
	case ListView::EventType::ON_SELECTED_ITEM_END:
		{
			break;
		}
	default:
		break;
	}

	//ccMessageBox(msg,"����¼�");
}


//��д����ע�ắ�������¸�����������
void KuiTeam::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,128, true);
}

Rect KuiTeam::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y

	return rc;
}

void KuiTeam::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//�ƶ���
void KuiTeam::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling || !__isCanMove) return;
	Point point   = ParentNode_Team->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff    = point - m_lastPoint;                  //����ͼƬ��λ��
	ParentNode_Team->setPosition((ParentNode_Team->getPosition() + diff));
	m_origin      = ParentNode_Team->getPosition();
	m_lastPoint = point;
}
//��������ccTouchBegan������true
bool KuiTeam::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	Point point = ParentNode_Team->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Team->getContentSize().width*/,winSize.height/* ParentNode_Team->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //����ڷ�Χ�� �Ϳ����϶�
	m_lastPoint = point;
	return true;
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	/*Point location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	Sprite * SkillNode =NULL;
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; ++i)
	{
		SkillNode = (Sprite *)ParentNode_Team->getChildByTag(i+1);
		if  (SkillNode)
		{
			if (getRect(SkillNode,i+1).containsPoint(location))
			{//������
				SkillNode->runAction(red);
			}
			else
			{
				SkillNode->setColor(ax::Color3B::WHITE);           //Ĭ����ɫ
			}
		}
	}
	return true;*/
}

void KuiTeam::OnRefresh()
{
	if (!g_pCoreShell || !g_GameWorld ||!pNearbyPlayerslistView) return;
	//��ʼ��Ӹ�������б�
	if (m_pNearbyPlayersList)
	{
		free(m_pNearbyPlayersList);
		m_pNearbyPlayersList = NULL;
	}

	int nCount = g_pCoreShell->GetGameData(GDI_NEARBY_IDLE_PLAYER_LIST, 0, 0);  //��ȡ��������
	__NearbyListCount = nCount;
	if (nCount)
	{
		m_pNearbyPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * nCount);
		if (m_pNearbyPlayersList)
		{
			g_pCoreShell->GetGameData(GDI_NEARBY_IDLE_PLAYER_LIST, (uintptr_t)m_pNearbyPlayersList,nCount);
			for (int i = 0; i < nCount; ++i)
			{
				Button* custom_button = Button::create();
				custom_button->setName("TextButton");
				custom_button->setTouchEnabled(true);
				//_button_2->loadTextures("ui/textbg.png", "ui/textbg.png", "");
				custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png","");
				custom_button->setScale9Enabled(true);
				custom_button->setContentSize(btnSize);
				custom_button->setTitleFontSize(12);
				char nTempstr[32];
				t_sprintf(nTempstr,m_pNearbyPlayersList[i].Name);
				custom_button->setTitleText(UTEXT(nTempstr,1));

				Layout *custom_item = Layout::create();
				custom_item->setContentSize(btnSize);
				custom_button->setPosition(ax::Vec2(btnSize.width/2, btnSize.height/2));
				custom_item->addChild(custom_button);
				custom_item->setTag(m_pNearbyPlayersList[i].nIndex);//NpcIndex
				pNearbyPlayerslistView->pushBackCustomItem(custom_item);
				//m_pNearbyPlayersList++;
			}
		}
	}
	else
	{
		Button* custom_button = Button::create();
		custom_button->setName("TextButton");
		custom_button->setTouchEnabled(true);
		custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png", "");
		custom_button->setScale9Enabled(true);
		custom_button->setContentSize(btnSize);
		custom_button->setTitleFontSize(12);
		char nTempstr[32];
		t_sprintf(nTempstr,"��������ڸ���");
		custom_button->setTitleText(UTEXT(nTempstr,1));

		Layout *custom_item = Layout::create();
		custom_item->setContentSize(btnSize);
		custom_button->setPosition(ax::Vec2(btnSize.width/2, btnSize.height/2));
		custom_item->addChild(custom_button);
		//custom_item->setTag(m_pNearbyPlayersList->nIndex);//NpcIndex
		pNearbyPlayerslistView->pushBackCustomItem(custom_item);
	}
}

//--------------------------------------------------------------------------
//	���ܣ����¶�����Ϣ
//--------------------------------------------------------------------------
void KuiTeam::UpdateData(KUiPlayerTeam* pInfo)
{
	if (!g_pCoreShell || !g_GameWorld) return;
	Clear();
	KUiPlayerLeaderShip	LeaderShip;
	memset(&LeaderShip, 0, sizeof(KUiPlayerLeaderShip));
	g_pCoreShell->GetGameData(GDI_PLAYER_LEADERSHIP,(uintptr_t)&LeaderShip, 0);  //����ͳ˧������ص�����
	char msg[64];
	t_sprintf(msg,"%d",LeaderShip.nLeaderShipLevel);
	pMoneyLabel->setString(msg);//m_LeaderAbility.SetIntText(LeaderShip.nLeaderShipLevel);  //ͳ˧

	if (pInfo)
	{
		m_Info = *pInfo;
	}
	else if (!g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (uintptr_t)&m_Info, 0))//�������ڵĶ�����Ϣ
		memset(&m_Info,0,sizeof(m_Info));

	if (m_Info.bTeamLeader)  //����Ƕӳ�
	{//��ť����
		if  (checkBox && m_Info.bOpened)
			checkBox->setSelected(true);

	}
	else
	{
		if  (checkBox)
			checkBox->setSelected(false);
	}

	if (m_Info.cNumMember > 0)
	{//����ж���
		m_pPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * (m_Info.cNumMember));
		if (m_pPlayersList)
		{
			int nCount = g_pCoreShell->TeamOperation(TEAM_OI_GD_MEMBER_LIST,(uintptr_t)m_pPlayersList, m_Info.cNumMember);
			if (nCount == m_Info.cNumMember)
			{//��ʼ�����б�����
                __playerListCount =nCount;
				for (int i = 0; i < nCount; ++i)
				{
					Button* custom_button = Button::create();
					custom_button->setName("TextButton");
					custom_button->setTouchEnabled(true);
					custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png","");
					custom_button->setScale9Enabled(true);
					custom_button->setContentSize(btnSize);
					custom_button->setTitleFontSize(12);
					char nTempstr[32];
					t_sprintf(nTempstr,m_pPlayersList[i].Name);
					custom_button->setTitleText(UTEXT(nTempstr,1));

					Layout *custom_item = Layout::create();
					custom_item->setContentSize(btnSize);
					custom_button->setPosition(ax::Vec2(btnSize.width/2, btnSize.height/2));
					custom_item->addChild(custom_button);
					custom_item->setTag(m_pPlayersList[i].nIndex);//NpcIndex
					pPlayersListView->pushBackCustomItem(custom_item);
				}
			}
		}
		else
		{
			m_Info.cNumMember = 0;
			__playerListCount = 0;
		}
	}
	OnRefresh();
}

void KuiTeam::Clear()
{
   //���ð�ť״̬
	memset(&m_Info, 0, sizeof(KUiPlayerTeam));
	if (m_pPlayersList)
	{
		free(m_pPlayersList);
		m_pPlayersList = NULL;
	}

	if (m_pNearbyPlayersList)
	{
		free(m_pNearbyPlayersList);
		m_pNearbyPlayersList = NULL;
	}

	if (pPlayersListView)
	{
		pPlayersListView->removeAllItems();
		pPlayersListView->forceDoLayout();
	}
	if (pNearbyPlayerslistView)
	{
		pNearbyPlayerslistView->removeAllItems();
		pNearbyPlayerslistView->forceDoLayout();
	}
}
void KuiTeam::update(float delta)
{
	//ccMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	}
}
