#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "Kuiskill.h"
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

Kuiskill::Kuiskill()
{
	isOpen = false;
	memset(Skills,0,sizeof(KUiSkillData) * 50);
	nRemainSkillPoint = 0;
	pSkillPointLabel  = NULL;
	pSkillInfoLabel   = NULL;
	colorLayer        = NULL;
	red               = NULL;
	nSkillCount       = 0;
	nCurSelindex      = 0;
}

Kuiskill::~Kuiskill()
{
	isOpen = false;
	nRemainSkillPoint = 0;
	pSkillPointLabel  = NULL;
	colorLayer        = NULL;
	pSkillInfoLabel   = NULL;
	if (red)
	   red->release();
	//red               = NULL;
}

//创建确定弹出框
Kuiskill * Kuiskill::create(int m_Kind,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	Kuiskill * popLayer = Kuiskill::create();
	//popLayer->setTitle(title);
	popLayer->addDialogData();
	//设置按钮
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//初始化
bool Kuiskill::init()
{
	if ( !Layer::init() ){
		return false;
	}
	Size winSize = ax::Director::getInstance()->getWinSize();
	Sprite * spriteBox = Sprite::create("ui/uiSkill.png");//Sprite::createWithSpriteFrame(f_tips_box);
	ParentNode_Fight = spriteBox;
	spriteBox->setPosition(ax::Vec2(winSize.width/2,winSize.height/2));
	this->addChild(spriteBox);
	//获得背景图片的大小
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();
	m_pUiLayer = Layer::create();
	m_pUiLayer->setPosition(ax::Vec2(0,0));
	addChild(m_pUiLayer);
	//开启触摸响应

	this->scheduleUpdate();                   //调用函数
	auto listener = ax::EventListenerTouchOneByOne::create();
ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this); //禁止多点触摸
	return true;
}

void Kuiskill::addDialogData()
{
    pSkillPointLabel = Label::createWithTTF("",UI_GAME_FONT_SONG, 12);//Arial
	pSkillPointLabel->setPosition(ax::Vec2(180,0));
	ParentNode_Fight->addChild(pSkillPointLabel,1);
	Size pageSize;
	pageSize.width = 711;
	pageSize.height = 410;
	int NewscrX =0,NewscrY =0;
	MenuItemImage *paddPointItem = MenuItemImage::create("ui/btn_common1_1.png","ui/btn_common1_2.png",CC_CALLBACK_1(Kuiskill::addskillpoint, this));
	paddPointItem->setPosition(ax::Vec2(pageSize.width/2-100,-240));
	paddPointItem->setAnchorPoint(ax::Vec2(0,0));
	Label * paddPointItemLabel = Label::createWithTTF("Tăng điểm",UI_GAME_FONT_DEFAULT_VN_BTN, 18);
	paddPointItemLabel->setColor(ax::Color3B::WHITE);
	paddPointItemLabel->setPosition(ax::Vec2(paddPointItemLabel->getContentSize().width/4-paddPointItemLabel->getContentSize().width/4+10,paddPointItemLabel->getContentSize().height/4));
	paddPointItemLabel->setAnchorPoint(ax::Vec2(0,0));
	paddPointItemLabel->enableOutline(Color4B(0, 0, 0, 255), 3);
	paddPointItem->addChild(paddPointItemLabel);




	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_FIGHT_SKILLS, (uintptr_t)Skills, 0); //获取分页技能数据


	//offsetX_b=215;
	//offsetY_b=98;

	// Create the page view
	ax::ui::PageView* pageView = ax::ui::PageView::create();
	pageView->setTouchEnabled(true);
	pageView->setContentSize(pageSize);
	//pageView->setAnchorPoint(ax::Vec2(0,0));
	pageView->setPosition(ax::Vec2(165,170));
	pageView->setTag(2000);
	pageView->setName("pageView");
	Layout* layout = Layout::create();
	layout->setContentSize(pageSize);
	ImageView* imageView = ImageView::create();
	imageView->setTouchEnabled(false);
	imageView->setScale9Enabled(true);
	//imageView->loadTexture("ui/bg_page.png");
	imageView->setContentSize(pageSize);
	imageView->setPosition(ax::Vec2(pageSize.width/2,pageSize.height/2));
	layout->addChild(imageView);

	Label* label = Label::createWithTTF("Trang 1", UI_GAME_FONT_DEFAULT_VN, 36);
	label->setScale(0.5);
	label->setColor(Color3B(255, 255, 255));
	label->setPosition(ax::Vec2(layout->getLayoutSize().width / 2, layout->getLayoutSize().height / 2-180));
	layout->addChild(label);
	layout->setPosition(ax::Vec2(0,0));
	pageView->addPage(layout);

	Layout* layout1 = Layout::create();
	layout1->setContentSize(pageSize);

	ImageView* imageView2 = ImageView::create();
	imageView2->setTouchEnabled(false);
	imageView2->setScale9Enabled(true);
	//imageView2->loadTexture("ui/bg_page.png");
	imageView2->setContentSize(pageSize);
	imageView2->setPosition(ax::Vec2(pageSize.width/2,pageSize.height/2));
	layout1->addChild(imageView2);

	Label* label2 = Label::createWithTTF("Trang 2", UI_GAME_FONT_DEFAULT_VN, 36);
	label2->setScale(0.5);
	label2->setColor(Color3B(255, 255, 255));
	label2->setPosition(ax::Vec2(layout1->getLayoutSize().width / 2, layout1->getLayoutSize().height / 2-180));
	layout1->addChild(label2);
	layout1->setPosition(ax::Vec2(0,0));
	pageView->addPage(layout1);

	Layout* layout2 = Layout::create();
	layout2->setContentSize(pageSize);

	ImageView* imageView3 = ImageView::create();
	imageView3->setTouchEnabled(false);
	imageView3->setScale9Enabled(true);
	//imageView3->loadTexture("ui/bg_page.png");
	imageView3->setContentSize(pageSize);
	imageView3->setPosition(ax::Vec2(pageSize.width/2,pageSize.height/2));
	layout2->addChild(imageView3);

	Label* label3 = Label::createWithTTF("Trang 3", UI_GAME_FONT_DEFAULT_VN, 36);
	label3->setScale(0.5);
	label3->setColor(Color3B(255, 255, 255));
	label3->setPosition(ax::Vec2(layout2->getLayoutSize().width / 2, layout2->getLayoutSize().height / 2-180));
	layout2->addChild(label3);
	layout2->setPosition(ax::Vec2(0,0));
	pageView->addPage(layout2);

	layout->setVisible(false);
	layout1->setVisible(false);
	layout2->setVisible(false);

	//trang truoc
	MenuItemImage *prePage = MenuItemImage::create("ui/btn_turn_left.png","ui/btn_turn_left.png",CC_CALLBACK_1(Kuiskill::toprePage, this));
	prePage->setPosition(ax::Vec2(-pageSize.width/2-25,-20));
	prePage->setAnchorPoint(ax::Vec2(0,0));
	prePage->setScaleX(0.9);

	//trang sau
	MenuItemImage *nextPage = MenuItemImage::create("ui/btn_turn_right.png","ui/btn_turn_right.png",CC_CALLBACK_1(Kuiskill::tonextPage, this));
	nextPage->setPosition(ax::Vec2(pageSize.width/2-15,-20));
	nextPage->setAnchorPoint(ax::Vec2(0,0));
	nextPage->setScaleX(0.9);


	Menu *newmenu=Menu::create(paddPointItem,prePage,nextPage,NULL);  // 此步很关键，否则只是菜单项，但不能点击
	newmenu->setAnchorPoint(ax::Vec2(0,0));
	newmenu->setPosition(ax::Vec2(m_size.width/2,m_size.height/2)); //m_size.width-pSelectedTexture->getContentSize().width*3-30,42
	ParentNode_Fight->addChild(newmenu,1);

	int offsetX=120,offsetY=180,i;
	int offsetX_b=207;
	int offsetY_b=98;
	for (i = 0; i < FIGHT_SKILL_COUNT; ++i)  //FIGHT_SKILL_COUNT_PER_PAGE
	{//tao node icon skill

		if (i<15){//page 1
			Button* button = Button::create();
			button->setTouchEnabled(true);
			button->loadTextures("ui/itembg1.png", "ui/itembg1.png", "");
			button->setContentSize(ax::Size(216,61));
			button->setVisible(false);
			Rect CCRectZero = ax::Rect(0,0,5,5);
			CCRectZero.size = button->getContentSize();
			int NewscrX =0,NewscrY =0;
			Texture2D * bgCur= GetiniData("\\spr\\Ui\\技能图标\\枪法.spr",0,0,0,0,&NewscrX,&NewscrY,0,true);
			if (bgCur){
				ImageView* nSkillSpr = ImageView::create();
//				nSkillSpr->loadTextureByData(bgCur);
				nSkillSpr->setAnchorPoint(ax::Vec2(0,0));
				nSkillSpr->setScale(1.2);

				nSkillSpr->setTag(i+1);
				nSkillSpr->setPosition(ax::Vec2(-button->getLayoutSize().width/2+8,-button->getLayoutSize().height/2+8));
				if(i==0){
					offsetX=120;
					offsetY=180;
				}
				else if  (i>=0 && i<3)//item 1-2
				{
					offsetX = offsetX+CCRectZero.size.width+20;//đặt độ lệch tiếp theo
				}
				else if (i==3)
				{//2行
					offsetX = 120;
					offsetY = 190+CCRectZero.size.height;
				}
				else if  (i>3 && i<6)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}
				else if (i==6)
				{//3行
					offsetX = 120;
					offsetY = 200+CCRectZero.size.height*2;
				}
				else if  (i>6 && i<9)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}
				else if (i==9)
				{//4行
					offsetX = 120;
					offsetY = 210+CCRectZero.size.height*3;
				}
				else if  (i>9 && i<12)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}

				else if (i==12)
				{//5行
					offsetX = 120;
					offsetY = 220+CCRectZero.size.height*4;
				}
				else if  (i>12 &&i<15)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}

				char nKey[32],nKeyPoint[32];
				sprintf(nKey,"skillname_%d",i+1);
				std::string m_Key = nKey;

				//Ten skill
                char skillName[255];
                sprintf(skillName,"Tên kỹ năng %d",i);
				Label* nSkillName = Label::createWithTTF(skillName, UI_GAME_FONT_DEFAULT_VN, 36);
				nSkillName->setScale(0.5);
				nSkillName->setColor(Color3B(0, 255, 0));
				nSkillName->setAnchorPoint(ax::Vec2(0,0));
                int nikey = std::hash<std::string>{}(m_Key);
				nSkillName->setTag(nikey);
				nSkillName->setPosition(ax::Vec2(nSkillSpr->getPosition().x+nSkillSpr->getLayoutSize().width+15, -button->getLayoutSize().height / 2+33));
				//Ten skill

				//cap hien tai
				sprintf(nKeyPoint,"skillpoint_%d",i+1);
				std::string m_KeyPoint = nKeyPoint;
                char skillPoint[255];
                sprintf(skillPoint,"Cấp hiện tại %d",i);
                Label* nSkillPoint = Label::createWithTTF(skillPoint, UI_GAME_FONT_DEFAULT_VN, 32);
				nSkillPoint->setScale(0.5);
				nSkillPoint->setColor(Color3B(255, 255, 255));
				nSkillPoint->setAnchorPoint(ax::Vec2(0,0));
				nSkillPoint->setTag(std::hash<std::string>{}(m_KeyPoint));
				nSkillPoint->setPosition(ax::Vec2(nSkillSpr->getPosition().x+nSkillSpr->getLayoutSize().width+15, -button->getLayoutSize().height / 2+8));
				//cap hien tai

				//cap hien tai
				ZeroMemory(nKeyPoint,sizeof(nKeyPoint));
				sprintf(nKeyPoint,"skillpointadd_%d",i+1);
				std::string m_KeyPointadd = nKeyPoint;

                char skillPointadd[255];
                sprintf(skillPointadd,"Tăng %d",i);
                Label* nSkillPointadd = Label::createWithTTF(skillPoint, UI_GAME_FONT_DEFAULT_VN, 32);
				nSkillPointadd->setScale(0.5);
				nSkillPointadd->setColor(Color3B(255, 255, 255));
				nSkillPointadd->setAnchorPoint(ax::Vec2(0,0));
                nikey = std::hash<std::string>{}(m_KeyPointadd);
				nSkillPointadd->setTag(nikey);
				nSkillPointadd->setPosition(ax::Vec2(nSkillSpr->getPosition().x+nSkillSpr->getLayoutSize().width+120, -button->getLayoutSize().height / 2+8));
				//cap hien tai

				button->addChild(nSkillName);
				button->addChild(nSkillPoint);
				button->addChild(nSkillPointadd);
				button->addChild(nSkillSpr);
			}
			button->setPosition(ax::Vec2(offsetX,m_size.height-offsetY));
			button->setTag(i+1);
			button->addTouchEventListener(CC_CALLBACK_2(Kuiskill::oktouchEvent, this));
			//button->setTag(m_Key);

			layout->addChild(button);
		}else if (i>14 && i<30){ //Page 2
			Button* button = Button::create();
			button->setTouchEnabled(true);
			button->loadTextures("ui/itembg1.png", "ui/itembg1.png", "");
			button->setContentSize(ax::Size(216,61));
			button->setVisible(false);
			Rect CCRectZero = ax::Rect(0,0,5,5);
			CCRectZero.size = button->getContentSize();
			int NewscrX =0,NewscrY =0;
			Texture2D * bgCur= GetiniData("\\spr\\Ui\\技能图标\\枪法.spr",0,0,0,0,&NewscrX,&NewscrY,0,true);
			if (bgCur){
				ImageView* nSkillSpr = ImageView::create();
//				nSkillSpr->loadTextureByData(bgCur);
				nSkillSpr->setAnchorPoint(ax::Vec2(0,0));
				nSkillSpr->setScale(1.2);

				nSkillSpr->setTag(i+1);
				nSkillSpr->setPosition(ax::Vec2(-button->getLayoutSize().width/2+8,-button->getLayoutSize().height/2+8));
				if(i==15){
					offsetX=120;
					offsetY=180;
				}
				else if  (i>=15 && i<18)//item 1-2
				{
					offsetX = offsetX+CCRectZero.size.width+20;//đặt độ lệch tiếp theo
				}
				else if (i==18)
				{//2行
					offsetX = 120;
					offsetY = 190+CCRectZero.size.height;
				}
				else if  (i>18 && i<21)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}
				else if (i==21)
				{//3行
					offsetX = 120;
					offsetY = 200+CCRectZero.size.height*2;
				}
				else if  (i>21 && i<24)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}
				else if (i==24)
				{//4行
					offsetX = 120;
					offsetY = 210+CCRectZero.size.height*3;
				}
				else if  (i>24 && i<27)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}

				else if (i==27)
				{//5行
					offsetX = 120;
					offsetY = 220+CCRectZero.size.height*4;
				}
				else if  (i>27 &&i<30)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}

				char nKey[32],nKeyPoint[32];
				sprintf(nKey,"skillname_%d",i+1);
				std::string m_Key = nKey;

				//Ten skill
                char skillName[255];
                sprintf(skillName,"Tên kỹ năng %d",i);
                Label* nSkillName = Label::createWithTTF(skillName, UI_GAME_FONT_DEFAULT_VN, 36);
				nSkillName->setScale(0.5);
				nSkillName->setColor(Color3B(0, 255, 0));
				nSkillName->setAnchorPoint(ax::Vec2(0,0));
                int nikey = std::hash<std::string>{}(m_Key);
				nSkillName->setTag(nikey);
				nSkillName->setPosition(ax::Vec2(nSkillSpr->getPosition().x+nSkillSpr->getLayoutSize().width+15, -button->getLayoutSize().height / 2+33));
				//Ten skill

				//cap hien tai
				sprintf(nKeyPoint,"skillpoint_%d",i+1);
				std::string m_KeyPoint = nKeyPoint;
                char skillPoint[255];
                sprintf(skillPoint,"Cấp hiện tại %d",i);
                Label* nSkillPoint = Label::createWithTTF(skillName, UI_GAME_FONT_DEFAULT_VN, 32);

				nSkillPoint->setScale(0.5);
				nSkillPoint->setColor(Color3B(255, 255, 255));
				nSkillPoint->setAnchorPoint(ax::Vec2(0,0));
				nSkillPoint->setTag(std::hash<std::string>{}(m_KeyPoint));
				nSkillPoint->setPosition(ax::Vec2(nSkillSpr->getPosition().x+nSkillSpr->getLayoutSize().width+15, -button->getLayoutSize().height / 2+8));
				//cap hien tai

				//cap hien tai
				ZeroMemory(nKeyPoint,sizeof(nKeyPoint));
				sprintf(nKeyPoint,"skillpointadd_%d",i+1);
				std::string m_KeyPointadd = nKeyPoint;

                char skillPointadd[255];
                sprintf(skillPointadd,"Tăng %d",i);
                Label* nSkillPointadd = Label::createWithTTF(skillName, UI_GAME_FONT_DEFAULT_VN, 32);
				nSkillPointadd->setScale(0.5);
				nSkillPointadd->setColor(Color3B(255, 255, 255));
				nSkillPointadd->setAnchorPoint(ax::Vec2(0,0));
				nSkillPointadd->setTag(std::hash<std::string>{}(m_KeyPointadd));
				nSkillPointadd->setPosition(ax::Vec2(nSkillSpr->getPosition().x+nSkillSpr->getLayoutSize().width+120, -button->getLayoutSize().height / 2+8));
				//cap hien tai

				button->addChild(nSkillName);
				button->addChild(nSkillPoint);
				button->addChild(nSkillPointadd);
				button->addChild(nSkillSpr);
			}
			button->setPosition(ax::Vec2(offsetX,m_size.height-offsetY));
			button->setTag(i+1);
			button->addTouchEventListener(CC_CALLBACK_2(Kuiskill::oktouchEvent, this));
			//button->setTag(m_Key);

			layout1->addChild(button);
		}else if (i>29 && i<45){ //Page 2
			Button* button = Button::create();
			button->setTouchEnabled(true);
			button->loadTextures("ui/itembg1.png", "ui/itembg1.png", "");
			button->setContentSize(ax::Size(216,61));
			button->setVisible(false);
			Rect CCRectZero = ax::Rect(0,0,5,5);
			CCRectZero.size = button->getContentSize();
			int NewscrX =0,NewscrY =0;
			Texture2D * bgCur= GetiniData("\\spr\\Ui\\技能图标\\枪法.spr",0,0,0,0,&NewscrX,&NewscrY,0,true);
			if (bgCur){
				ImageView* nSkillSpr = ImageView::create();
//				nSkillSpr->loadTextureByData(bgCur);
				nSkillSpr->setAnchorPoint(ax::Vec2(0,0));
				nSkillSpr->setScale(1.2);

				nSkillSpr->setTag(i+1);
				nSkillSpr->setPosition(ax::Vec2(-button->getLayoutSize().width/2+8,-button->getLayoutSize().height/2+8));
				if(i==30){
					offsetX=120;
					offsetY=180;
				}
				else if  (i>=30 && i<33)//item 1-2
				{
					offsetX = offsetX+CCRectZero.size.width+20;//đặt độ lệch tiếp theo
				}
				else if (i==33)
				{//2行
					offsetX = 120;
					offsetY = 190+CCRectZero.size.height;
				}
				else if  (i>33 && i<36)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}
				else if (i==36)
				{//3行
					offsetX = 120;
					offsetY = 200+CCRectZero.size.height*2;
				}
				else if  (i>36 && i<39)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}
				else if (i==39)
				{//4行
					offsetX = 120;
					offsetY = 210+CCRectZero.size.height*3;
				}
				else if  (i>39 && i<42)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}

				else if (i==42)
				{//5行
					offsetX = 120;
					offsetY = 220+CCRectZero.size.height*4;
				}
				else if  (i>42 &&i<45)
				{
					offsetX = offsetX+CCRectZero.size.width +20;
				}

				char nKey[32],nKeyPoint[32];
				sprintf(nKey,"skillname_%d",i+1);
				std::string m_Key = nKey;

				//Ten skill
                char skillName[255];
                sprintf(skillName,"Tên kỹ năng %d",i);
                Label* nSkillName = Label::createWithTTF(skillName, UI_GAME_FONT_DEFAULT_VN, 36);
				nSkillName->setScale(0.5);
				nSkillName->setColor(Color3B(0, 255, 0));
				nSkillName->setAnchorPoint(ax::Vec2(0,0));
				nSkillName->setTag(std::hash<std::string>{}(m_Key));
				nSkillName->setPosition(ax::Vec2(nSkillSpr->getPosition().x+nSkillSpr->getLayoutSize().width+15, -button->getLayoutSize().height / 2+33));
				//Ten skill

				//cap hien tai
				sprintf(nKeyPoint,"skillpoint_%d",i+1);
				std::string m_KeyPoint = nKeyPoint;

                char skillPoint[255];
                sprintf(skillPoint,"Cấp hiện tại %d",i);
                Label* nSkillPoint = Label::createWithTTF(skillName, UI_GAME_FONT_DEFAULT_VN, 32);
				nSkillPoint->setScale(0.5);
				nSkillPoint->setColor(Color3B(255, 255, 255));
				nSkillPoint->setAnchorPoint(ax::Vec2(0,0));
				nSkillPoint->setTag(std::hash<std::string>{}(m_KeyPoint));
				nSkillPoint->setPosition(ax::Vec2(nSkillSpr->getPosition().x+nSkillSpr->getLayoutSize().width+15, -button->getLayoutSize().height / 2+8));
				//cap hien tai

				//cap hien tai
				ZeroMemory(nKeyPoint,sizeof(nKeyPoint));
				sprintf(nKeyPoint,"skillpointadd_%d",i+1);
				std::string m_KeyPointadd = nKeyPoint;

                char skillPointadd[255];
                sprintf(skillPointadd,"Tăng %d",i);
                Label* nSkillPointadd = Label::createWithTTF(skillName, UI_GAME_FONT_DEFAULT_VN, 32);
				nSkillPointadd->setScale(0.5);
				nSkillPointadd->setColor(Color3B(255, 255, 255));
				nSkillPointadd->setAnchorPoint(ax::Vec2(0,0));
				nSkillPointadd->setTag(std::hash<std::string>{}(m_KeyPointadd));
				nSkillPointadd->setPosition(ax::Vec2(nSkillSpr->getPosition().x+nSkillSpr->getLayoutSize().width+120, -button->getLayoutSize().height / 2+8));
				//cap hien tai

				button->addChild(nSkillName);
				button->addChild(nSkillPoint);
				button->addChild(nSkillPointadd);
				button->addChild(nSkillSpr);
			}
			button->setPosition(ax::Vec2(offsetX,m_size.height-offsetY));
			button->setTag(i+1);

			button->addTouchEventListener(CC_CALLBACK_2(Kuiskill::oktouchEvent, this));
			//button->setTag(m_Key);

			layout2->addChild(button);
		}
	}
	//pageView->addEventListenerPageView(this, pagevieweventselector(Kuiskill::pageViewEvent));
	m_pUiLayer->addChild(pageView);
	for (i = 0; i < FIGHT_SKILL_COUNT; ++i)
	{//战斗技能
		//m_FightSkills[i].HoldObject(Skills[i].uGenre, Skills[i].uId, Skills[i].nLevel, 0);//显示鼠标进过的物品描述，设置容纳对象的类型
		ISkill *pSkill = g_SkillManager.GetSkill(Skills[i].uId,1);
		if (pSkill)
		{//绘画技能图标
			if (i<15){
				layout->setVisible(true);
				//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(pSkill->GetSkillName());
				nSkillCount++;
				int NewscrX =0,NewscrY =0;
				char nIconFile[128];
				char nSkillNameData[128];
				t_printf(nSkillNameData,pSkill->GetSkillName());
				sprintf(nIconFile, "%s", pSkill->getIconPath().c_str());
				Texture2D * bgCur= GetiniData(nIconFile,0,0,0,0,&NewscrX,&NewscrY,0,true);
				Button* btn = (Button *)layout->getChildByTag(i+1);
				if (btn){
					btn->setVisible(true);
					ImageView* nSkillSpr = (ImageView *)btn->getChildByTag(i+1);
					if (nSkillSpr){
						char nKey[32],nKeyPoint[32];
						sprintf(nKey,"skillname_%d",i+1);
						std::string m_Key = nKey;
						Label  *nSkillNameLabel =(Label *)btn->getChildByName(m_Key);
						if (nSkillNameLabel){
							//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nSkillNameData);
							nSkillNameLabel->setString(pSkill->GetSkillName());
						}
						sprintf(nKeyPoint,"skillpoint_%d",i+1);
						std::string m_KeyPoint = nKeyPoint;
						Label  *nSkillLvLabel =(Label *)btn->getChildByName(m_KeyPoint);
						if (nSkillLvLabel){
							char nskillinfo[32];
							ZeroMemory(nskillinfo,sizeof(nskillinfo));
							sprintf(nskillinfo,"Cấp %d",Skills[i].nLevel);
							nSkillLvLabel->setString(nskillinfo);
						}
//						nSkillSpr->loadTextureByData(bgCur);
					}
				}
			}else if (i>14 && i<30){
				layout1->setVisible(true);
				//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(pSkill->GetSkillName());
				nSkillCount++;
				int NewscrX =0,NewscrY =0;
				char nIconFile[128];
				char nSkillNameData[128];
				t_printf(nSkillNameData,pSkill->GetSkillName());
				sprintf(nIconFile, "%s", pSkill->getIconPath().c_str());
				Texture2D * bgCur= GetiniData(nIconFile,0,0,0,0,&NewscrX,&NewscrY,0,true);
				Button* btn = (Button *)layout1->getChildByTag(i+1);
				if (btn){
					btn->setVisible(true);
					ImageView* nSkillSpr = (ImageView *)btn->getChildByTag(i+1);
					if (nSkillSpr){
						char nKey[32],nKeyPoint[32];
						sprintf(nKey,"skillname_%d",i+1);
						std::string m_Key = nKey;
						Label  *nSkillNameLabel =(Label *)btn->getChildByName(m_Key);
						if (nSkillNameLabel){
							//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nSkillNameData);
							nSkillNameLabel->setString(pSkill->GetSkillName());
						}
						sprintf(nKeyPoint,"skillpoint_%d",i+1);
						std::string m_KeyPoint = nKeyPoint;
						Label  *nSkillLvLabel =(Label *)btn->getChildByName(m_KeyPoint);
						if (nSkillLvLabel){
							char nskillinfo[32];
							ZeroMemory(nskillinfo,sizeof(nskillinfo));
							sprintf(nskillinfo,"Cấp %d",Skills[i].nLevel);
							nSkillLvLabel->setString(nskillinfo);
						}
//						nSkillSpr->loadTextureByData(bgCur);
					}
				}
			}else if (i>29 && i<45){
				layout2->setVisible(true);
				//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(pSkill->GetSkillName());
				nSkillCount++;
				int NewscrX =0,NewscrY =0;
				char nIconFile[128];
				char nSkillNameData[128];
				t_printf(nSkillNameData,pSkill->GetSkillName());
				sprintf(nIconFile, "%s", pSkill->getIconPath().c_str());
				Texture2D * bgCur= GetiniData(nIconFile,0,0,0,0,&NewscrX,&NewscrY,0,true);
				Button* btn = (Button *)layout2->getChildByTag(i+1);
				if (btn){
					btn->setVisible(true);
					ImageView* nSkillSpr = (ImageView *)btn->getChildByTag(i+1);
					if (nSkillSpr){
						char nKey[32],nKeyPoint[32];
						sprintf(nKey,"skillname_%d",i+1);
						std::string m_Key = nKey;
						Label  *nSkillNameLabel =(Label *)btn->getChildByName(m_Key);
						if (nSkillNameLabel){
							//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nSkillNameData);
							nSkillNameLabel->setString(pSkill->GetSkillName());
						}
						sprintf(nKeyPoint,"skillpoint_%d",i+1);
						std::string m_KeyPoint = nKeyPoint;
						Label  *nSkillLvLabel =(Label *)btn->getChildByName(m_KeyPoint);
						if (nSkillLvLabel){
							char nskillinfo[32];
							ZeroMemory(nskillinfo,sizeof(nskillinfo));
							sprintf(nskillinfo,"Cấp %d",Skills[i].nLevel);
							nSkillLvLabel->setString(nskillinfo);
						}
//						nSkillSpr->loadTextureByData(bgCur);
					}
				}
			}

			// Sprite  *nSkillSpr =(Sprite *)ParentNode_Fight->getChildByTag(i+1);
			// if (nSkillSpr) //改变纹理
			// {
			// 	char nKey[32];
			// 	sprintf(nKey,"point_%d",i+1);
			// 	std::string m_Key = nKey;
			// 	Label  *npontLabel =(Label *)ParentNode_Fight->getChildByName(m_Key);
			// 	//nSkillPoint->setTag(m_Key);
			// 	if (npontLabel)
			// 	{
			// 		npontLabel->setVisible(true);
			// 		char nskillinfo[32];
			// 		ZeroMemory(nskillinfo,sizeof(nskillinfo));
			// 		if (pSkill)
			// 		{
			// 			int nAddPoint=g_pCoreShell->GetGameData(GDI_PLAYER_SKILL_ADDPOINT,Skills[i].uId,0);	//额外的技能点
			// 			sprintf(nskillinfo,"%d+%d",Skills[i].nLevel,nAddPoint);
			// 		}

			// 		npontLabel->setString(nskillinfo);
			// 	}

			// 	nSkillSpr->setVisible(true);
			// 	nSkillSpr->setTexture(bgCur);
			// }
		}
	}
	if  (nSkillCount<=0)
	{//还没有学会技能 就直接设置空手技能为左键
		//Player[CLIENT_PLAYER_INDEX].SetDefaultImmedSkill();
		//messageBox("技能为空","技能为空");
		/*
		pSkillList->uGenre = CGOG_SKILL_SHORTCUT;
		pSkillList->uId = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetCurActiveWeaponSkill();//武器空手技能ID!
		pSkillList->nData = 0;
		*/
	}

	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//颜色层
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);//设置和窗口的大小
	ParentNode_Fight->addChild(colorLayer,1000);
	//创建一个动作 改变精灵颜色
	red = TintBy::create(0.2,0,-255,-255);//持续时间+颜色
	//ActionInterval* tintred_back = tintred->reverse();
	//red = RepeatForever::create(Sequence::create(tintred,tintred_back,NULL));
	red->retain();
	/*
	ActionInterval*  action1 = CCTintTo::create(2, 255, 0, 255);
	ActionInterval*  action2 = TintBy::create(2, -127, -255, -127);
	ActionInterval*  action2Back = action2->reverse();
	m_tamara->runAction( action1);
	m_kathia->runAction( Sequence::create( action2, action2Back, NULL));

	ActionInterval* action = CCFadeIn::create(2);
	ActionInterval* action_back = action->reverse();
	Action* fade = RepeatForever::create( Sequence::create( action, action_back, NULL) );

	ActionInterval* tintred = TintBy::create(2, 0, -255, -255);
	ActionInterval* tintred_back = tintred->reverse();
	Action* red = RepeatForever::create( Sequence::create(tintred, tintred_back, NULL) );

	ActionInterval* tintgreen = TintBy::create(2, -255, 0, -255);
	ActionInterval* tintgreen_back = tintgreen->reverse();
	Action* green = RepeatForever::create( Sequence::create( tintgreen, tintgreen_back, NULL) );

	ActionInterval* tintblue = TintBy::create(2, -255, -255, 0);
	ActionInterval* tintblue_back = tintblue->reverse();
	Action* blue = RepeatForever::create( Sequence::create( tintblue, tintblue_back, NULL) );

	sprite5->runAction(red);
	sprite6->runAction(green);
	sprite7->runAction(blue);
	sprite8->runAction(fade);
	*/
}
//返回数组的上标
int Kuiskill::getfreeSkillNode(int skillidx)
{//nSkillCount已经是最大的数
	for (int i = 0; i < nSkillCount; ++i)
	{//战斗技能
		if  (Skills[i].uId>0 && Skills[i].uId==skillidx) //已经有这个技能了相同的技能
			return i;
	}
	nSkillCount++;
	return nSkillCount;
}

void Kuiskill::oktouchEvent(Ref *pSender, ax::ui::Widget::TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;

	switch (type)
	{
	case Widget::TouchEventType::BEGAN://Touch Down
	{
		Layer * sDesc = (Layer*)this->getChildByTag(1001);
		if (sDesc){
			this->removeChildByTag(1001);
		}
		if (pSender)
		{
			Button* nTbtn = (Button*)pSender;
			if (nTbtn && nTbtn->isVisible()){
				//nTbtn->setScale(1.1);

				bool isThis = false;
				if  (g_GameWorld)
				{
					int i = nTbtn->getTag()-1;
					ISkill *pSkill = g_SkillManager.GetSkill(Skills[i].uId,1);
					char nIconName[128] ,nskillinfo[128];
					ZeroMemory(nIconName,sizeof(nIconName));
					ZeroMemory(nskillinfo,sizeof(nskillinfo));
					if (pSkill)
						sprintf(nskillinfo,"%s cấp %d ",pSkill->GetSkillName(),Skills[i].nLevel);

					if (!pSkillInfoLabel)
					{
						pSkillInfoLabel = Label::createWithTTF("",UI_GAME_FONT_SONG, 12);//Arial
						pSkillInfoLabel->setPosition(ax::Vec2(245,43));
						pSkillInfoLabel->setAnchorPoint(ax::Vec2(0,0));
						pSkillInfoLabel->setColor(ax::Color3B::YELLOW);
						ParentNode_Fight->addChild(pSkillInfoLabel,1);
					}
					pSkillInfoLabel->setString(nskillinfo);
					KSkill * pOrdinSkill = NULL;
					pOrdinSkill = (KSkill *)pSkill;
					if (pSkill && pOrdinSkill && (pOrdinSkill->GetSkillLRInfo() == BothSkill || pOrdinSkill->GetSkillLRInfo() == leftOnlySkill))
					{//攻击技能（除断魂和轻功）//Skills[i].uId
						sprintf(nIconName, "%s", pSkill->getIconPath().c_str());
						g_GameWorld->setattackSprInfo(pSkill->GetSkillId(),Skills[i].uGenre,nIconName);
					}
					KUiItemdObject skillData;
					ZeroMemory(&skillData,sizeof(KUiItemdObject));
					skillData.nRect  = getRect(nTbtn,0);
					skillData.uId    = Skills[i].uId;
					skillData.uGenre = Skills[i].uGenre;
					skillData.nindex = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(Skills[i].uId);
					//int nList = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(Skills[i].uId);
					skillData.DataH  = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetAddPoint(skillData.nindex);
					skillData.DataW  = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetEnChance(skillData.nindex);

					Layer * sDescLayer = KuiSkilldesc::create(Skills[i].uId,Skills[i].nLevel,skillData,this,CC_CALLBACK_1(Kuiskill::closeSkilldescPadCallback, this));
					this->addChild(sDescLayer,2,1001);
					nCurSelindex = Skills[i].uId;
					isThis = true;

				}
			}else{
				this->removeChildByTag(1001);
			}
		}
	}
		break;
	case Widget::TouchEventType::MOVED://Touch Move
		break;

	case Widget::TouchEventType::ENDED://Touch Up
		{

		}
		break;
	case Widget::TouchEventType::CANCELED:
		this->removeChildByTag(1001);
		break;

	default:
		break;
	}
}
void Kuiskill::pageViewEvent(Ref *pSender, ax::ui::PageView::EventType type,Touch *pTouch)
{
	// ax::ui::PageView* pageView = dynamic_cast<ax::ui::PageView*>(pSender);
	// if (pageView){
	// 	Point location = pTouch->getLocation();//目的的坐标?屏幕点击处
	// 	Sprite * SkillNode =NULL;
	// 	bool isThis = false;
	// 	for (int i = 0; i < FIGHT_SKILL_COUNT; ++i)
	// 	{
	// 		if (i<15){//page 1

	// 			Layout *layout = (Layout *)pageView->getPage(0);
	// 			if (layout){
	// 				Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("111");
	// 				Button *btn = (Button *)layout->getChildByTag(i+1);
	// 				if (btn && btn->isVisible()){
	// 					Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("222");
	// 					if (getRect(btn,i+1).containsPoint(location)){
	// 						Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("333");
	// 						btn->runAction(red);
	// 						btn->setScale(2);
	// 					}
	// 				}
	// 			}
	// 		}else if (i>14 && i<30){//page 2

	// 		}else if (i>29 && i<45){//page 3

	// 		}
	// 	}
	// }
	//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("OKKKKKK");
    // switch (type)
    // {
    //     case PAGEVIEW_EVENT_TURNING:
    //     {

    //        // m_pDisplayValueLabel->setText(CCString::createWithFormat("page = %d", pageView->getCurPageIndex() + 1)->getCString());
    //     }
    //         break;

    //     default:
    //         break;
    // }
}
void Kuiskill::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	if (pSkill)
	{
			//if (pSkill->uGenre == CGOG_SKILL_LIVE)
				//m_pSelf->m_LiveSkillPad.UpdateSkill(pSkill, nIndex);
			//else
		if (pSkill->uGenre == CGOG_SKILL_FIGHT)
		{
			//messageBox("技能更新","技能更新");
			/*char skillinfo[64];
			sprintf(skillinfo,"%d",nIndex);
			messageBox(skillinfo,"技能更新");
			*/
			//int nPage = nIndex/FIGHT_SKILL_COUNT_PER_PAGE; //第几页
			//    nIndex = nIndex%FIGHT_SKILL_COUNT_PER_PAGE;//索引


			if (pSkill==NULL || nIndex<0 || nIndex >=FIGHT_SKILL_COUNT)
				return;
			//messageBox("技能更新","技能更新");
			//Skills[i].uId
			 Sprite  *nSkillSpr =(Sprite *)ParentNode_Fight->getChildByTag(nIndex+1);
			 if (nSkillSpr)
			 {//如果存在就增加
				 nSkillSpr->setVisible(true);
				 Skills[nIndex].uId     = pSkill->uId;
				 Skills[nIndex].nLevel  = pSkill->nLevel;
				 Skills[nIndex].uGenre  = pSkill->uGenre;

				 char nKey[32];
				 sprintf(nKey,"point_%d",nIndex+1);
				 std::string m_Key = nKey;
				 Label  *npontLabel =(Label *)ParentNode_Fight->getChildByName(m_Key);
				 if (npontLabel)
				 {
					 npontLabel->setVisible(true);
					 char nskillinfo[32];
					 ZeroMemory(nskillinfo,sizeof(nskillinfo));
					 if (pSkill)
					 {
						 int nAddPoint=g_pCoreShell->GetGameData(GDI_PLAYER_SKILL_ADDPOINT,Skills[nIndex].uId,0);	//额外的技能点
						 sprintf(nskillinfo,"%d+%d",Skills[nIndex].nLevel,nAddPoint);
					 }
					 npontLabel->setString(nskillinfo);
				 }

				 int NewscrX =0,NewscrY =0;
				 char nIconFile[128];
				 ISkill *mSkill = g_SkillManager.GetSkill(Skills[nIndex].uId,1);
				 if  (!mSkill)
					  return;
				 sprintf(nIconFile, "%s", mSkill->getIconPath().c_str());
				 Texture2D * bgCur= GetiniData(nIconFile,0,0,0,0,&NewscrX,&NewscrY,0,true);
				 if (bgCur)
				 {
					 Rect CCRectZero = ax::Rect(0,0,5,5);
				     CCRectZero.size = bgCur->getContentSize();
					 nSkillSpr->setTextureRect(CCRectZero);
					 nSkillSpr->setTexture(bgCur);
				 }
			 }
			//Skills[nIndex].
		}
				//m_pSelf->m_FightSkillPad.UpdateSkill(pSkill, nIndex); //更新某个位置的技能
	}
}
//设置确定取消按钮,参数：调用层对象，调用层回调函数
void Kuiskill::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	Sprite * sprite_tips_btn_confirm_normal =  Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_normal);
	//SpriteFrame * f_tips_btn_confirm_select = sfCache->spriteFrameByName("btn_confirm_select.png");
	Sprite * sprite_tips_btn_confirm_select =  Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_select);
	MenuItemSprite * itemConfirm = MenuItemSprite::create(sprite_tips_btn_confirm_normal, sprite_tips_btn_confirm_select, CC_CALLBACK_1(Kuiskill::buttonCallBackFunc, this));
	itemConfirm->setPosition(ax::Vec2(0,0));
	Point bgSpritePoint = ParentNode_Fight->getPosition();
	Menu * menu = Menu::create(itemConfirm,NULL);
	//menu->setPosition(ax::Vec2(bgSpritePoint.x-m_size.width/2,bgSpritePoint.y-m_size.height/2));//菜单位置设置为弹出框左下
	menu->setPosition(ax::Vec2(m_size.width/2,20));//菜单位置设置为弹出框左下
	ParentNode_Fight->addChild(menu);         //加到父层里
}

//执行上层对象的回调函数，关闭弹出框
void Kuiskill::buttonCallBackFunc(Ref * pSender)
{
	Node * node = dynamic_cast<Node *>(pSender);
	node->setTag(3000);//设置tag，在调用层可以获取到

	if (m_callfun &&  m_callbackListener)
	{
		//执行调用层回调函数，传递参数CCNode
		(m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}

//关闭弹出框
void Kuiskill::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}


//重写触摸注册函数，重新给定触摸级别
void Kuiskill::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//这里的触摸优先级设置为－128，与CCMenu同级，保证了屏蔽下方的触摸
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
}

Rect Kuiskill::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //这个因该是图内偏移
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //起点最左边
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //最上边    //- rc.origin.y

	return rc;
}
//触摸函数ccTouchBegan，返回true
bool Kuiskill::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	ax::ui::PageView* pageView = (ax::ui::PageView *)m_pUiLayer->getChildByName("pageView");
	if(pageView)
	{
		Point location = pTouch->getLocation();//目的的坐标?屏幕点击处
		Sprite * SkillNode =NULL;
		bool isThis = false;
		for (int i = 0; i < FIGHT_SKILL_COUNT; ++i)
		{
			if (i<15){//page 1

				Layout *layout = (Layout *)pageView->getItem(1);
				if (layout){
					Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("1111");
					Button *btn = (Button *)layout->getChildByTag(i+1);
					if (btn && btn->isVisible()){
						Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("2222");
						if (getRect(layout,i+1).containsPoint(location)){
							Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("3333");
							btn->runAction(red);
							btn->setScale(2);
						}
					}
				}
			}else if (i>14 && i<30){//page 2

			}else if (i>29 && i<45){//page 3

			}
		}
	}


	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	// Point location = pTouch->getLocation();//目的的坐标?屏幕点击处
	// Sprite * SkillNode =NULL;
	// bool isThis = false;
	// for (int i = 0; i < FIGHT_SKILL_COUNT; ++i)
	// {
	// 	SkillNode = (Sprite *)ParentNode_Fight->getChildByTag(i+1);
	// 	if  (SkillNode && SkillNode->isVisible())
	// 	{
	// 		if (getRect(SkillNode,i+1).containsPoint(location))
	// 		{//点中了
	// 			SkillNode->runAction(red);
	// 			if  (g_GameWorld)
	// 			{
	// 				ISkill *pSkill = g_SkillManager.GetSkill(Skills[i].uId,1);
	// 				char nIconName[128] ,nskillinfo[128];
	// 				ZeroMemory(nIconName,sizeof(nIconName));
	// 				ZeroMemory(nskillinfo,sizeof(nskillinfo));
	// 				if (pSkill)
	// 					sprintf(nskillinfo,"%s %d级",pSkill->GetSkillName(),Skills[i].nLevel);

	// 				if (!pSkillInfoLabel)
	// 				{
	// 					pSkillInfoLabel = Label::createWithTTF("",UI_GAME_FONT_SONG, 12);//Arial
	// 					pSkillInfoLabel->setPosition(ax::Vec2(245,43));
	// 					pSkillInfoLabel->setAnchorPoint(ax::Vec2(0,0));
	// 					pSkillInfoLabel->setColor(ax::Color3B::YELLOW);
	// 					ParentNode_Fight->addChild(pSkillInfoLabel,1);
	// 				}
	// 				pSkillInfoLabel->setString(UTEXT(nskillinfo,1).c_str());
	// 				KSkill * pOrdinSkill = NULL;
	// 				         pOrdinSkill = (KSkill *)pSkill;
	// 				//if (pSkill && Skills[i].nLevel>0 && Skills[i].uId !=15 && Skills[i].uId !=210 && Skills[i].uId !=40 && (pSkill->GetSkillStyle() == SKILL_SS_Missles || pSkill->GetSkillStyle() == SKILL_SS_Melee || Skills[i].uId ==53))
	// 				if (pSkill && pOrdinSkill && (pOrdinSkill->GetSkillLRInfo() == BothSkill || pOrdinSkill->GetSkillLRInfo() == leftOnlySkill))
	// 				{//攻击技能（除断魂和轻功）//Skills[i].uId
	// 					sprintf(nIconName,pSkill->getIconPath().c_str());
	// 					g_GameWorld->setattackSprInfo(pSkill->GetSkillId(),Skills[i].uGenre,nIconName);
	// 				}
	// 				/*else if (pSkill && Skills[i].nLevel>0 && (Skills[i].uId ==15 || pSkill->GetSkillStyle() == SKILL_SS_InitiativeNpcState || pSkill->GetSkillStyle() == SKILL_SS_Melee || Skills[i].uId ==210))
	// 				{//右边技能
	// 					sprintf(nIconName,pSkill->getIconPath().c_str());
	// 					g_GameWorld->setaauxiliaryskillInfo(Skills[i].uId,Skills[i].uGenre,nIconName);
	// 				}*/
	// 				KUiItemdObject skillData;
	// 				ZeroMemory(&skillData,sizeof(KUiItemdObject));
	// 				skillData.nRect  = getRect(SkillNode,0);
	// 				skillData.uId    = Skills[i].uId;
	// 				skillData.uGenre = Skills[i].uGenre;
	// 				skillData.nindex = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(Skills[i].uId);
	// 				//int nList = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(Skills[i].uId);
	// 				skillData.DataH  = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetAddPoint(skillData.nindex);
	// 				skillData.DataW  = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetEnChance(skillData.nindex);

	// 				Layer * sDescLayer = KuiSkilldesc::create(Skills[i].uId,Skills[i].nLevel,skillData,this,CC_CALLBACK_1(Kuiskill::closeSkilldescPadCallback));
	// 				this->addChild(sDescLayer,2,1001);
	// 				nCurSelindex = Skills[i].uId;
	// 				isThis = true;
	// 			}
	// 			//colorLayer->setContentSize(SkillNode->getContentSize());
	// 			//colorLayer->setPosition(SkillNode->getPosition());
	// 		}
	// 		else
	// 		{
	// 			if (!isThis)
	// 				nCurSelindex = 0;
	// 			SkillNode->setColor(ax::Color3B::WHITE);           //默认颜色
	// 			//colorLayer->setContentSize(ax::Size::ZERO);
	// 		}
	// 	}
	// }
	return true;
}

void Kuiskill::closeSkilldescPadCallback(Node *pNode)
{

}

void Kuiskill::closeDescCallback(Node *pNode)
{
	//messageBox("成功","成功");
}

void Kuiskill::toprePage(Ref* pSender)
{

	ax::ui::PageView* pageView = (ax::ui::PageView *)m_pUiLayer->getChildByName("pageView");
	if(pageView){
		Layout* layout = (Layout *)pageView->getItem(pageView->getCurrentPageIndex()-1);
		if (layout){
			if (!layout->isVisible()){
				pageView->scrollToPage(0);
			}else{
				pageView->scrollToPage(pageView->getCurrentPageIndex()-1);
			}
		}

		//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("toprePage");
	}
	return;
}
void Kuiskill::tonextPage(Ref* pSender)
{
	ax::ui::PageView* pageView = (ax::ui::PageView *)m_pUiLayer->getChildByName("pageView");
	if(pageView){
		//pageView->scrollToPage(pageView->getCurPageIndex()+1);
		Layout* layout = (Layout *)pageView->getItem(pageView->getCurrentPageIndex()+1);
		if (layout){
			if (!layout->isVisible()){
				pageView->scrollToPage(0);
			}else{
				pageView->scrollToPage(pageView->getCurrentPageIndex()+1);
			}
		}
	}

	return;
}
void Kuiskill::addskillpoint(Ref* pSender)
{//增加技能
	if (nCurSelindex<=0 || nCurSelindex>=MAX_SKILL)
	{
#ifdef WIN32
		messageBox("请选择一个技能","提示");
#else
		messageBox(UTEXT("请选择一个技能",1).c_str(),UTEXT("提示",1).c_str());
#endif
	   return;
	}
	   nRemainSkillPoint--;	 //使用技能点数
	if (pSkillPointLabel)
	{
		char nPointCount[32];
		sprintf(nPointCount,"%d",nRemainSkillPoint);
		pSkillPointLabel->setString(nPointCount);
	}
	if (g_pCoreShell)
	  g_pCoreShell->OperationRequest(GOI_TONE_UP_SKILL,CGOG_SKILL_FIGHT,nCurSelindex);
	//Layer * sItemsLayer = KuiItem::create("",this,CC_CALLBACK_1(Kuiskill::closeDescCallback));
	//ParentNode_Fight->addChild(sItemsLayer,2,OBJ_NODE_ITEM);
	//messageBox("成功","成功");
}

void Kuiskill::update(float delta)
{
	//messageBox("无限循环","update");
	if (isOpen && g_pCoreShell)
	{
	   nRemainSkillPoint = g_pCoreShell->GetGameData(GDI_FIGHT_SKILL_POINT, 0, 0);
	   if (pSkillPointLabel)
	   {
		   char nPointCount[32];
		   sprintf(nPointCount,"%d",nRemainSkillPoint);
		  // pSkillPointLabel->setColor(ccWit)
		   pSkillPointLabel->setString(nPointCount);
	   }
	}
}

Texture2D* Kuiskill::GetiniData(const char* sprPath,
                                int nFramIndex,
                                int nMpsX,
                                int nMpsY,
                                int nMpsZ,
                                int* mScreeX,
                                int* mScreeY,
                                int bRenderFlag,
                                int bSinglePlaneCoord)
{
	    if  (sprPath==NULL)
			return NULL;

	    char nSprName[128]={0};
		ZeroMemory(nSprName,sizeof(nSprName));
		sprintf(nSprName, "%s", sprPath);
		g_StrLower(nSprName);
		int m_nWidth,m_nHeight,nFrams;
		Texture2D *bgCur = NULL;
		SPRFRAMSINFO nSprInfo;
		ZeroMemory(&nSprInfo,sizeof(nSprInfo));
		//if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
		bgCur = _getinidata.getinidata_one(nSprName,nFramIndex,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);

		if (bgCur==NULL)
			return NULL;

#define CENTERX		160
#define	CENTERY		192

		int nX = nMpsX;  //位置
		int nY = nMpsY;  //位置

		if (bSinglePlaneCoord == false)//转换成屏幕坐标
			SubWorld[0].GetLocalPositionByMps(nX,nY,nMpsZ);

		if (bRenderFlag & RUIMAGE_RENDER_FLAG_REF_SPOT)
		{//渲染模式
		//****to be modify****
		  int nCenterX = nSprInfo.pHCenterX; //重心的水平位移
		  int nCenterY = nSprInfo.pHCenterY; //重心的垂直位移

		   if (nCenterX || nCenterY)
		   {
			 nX -= nCenterX;
			 nY -= nCenterY;
		   }
		   else if (nSprInfo.pHwidth > CENTERX)
		   {
			  nX -= CENTERX;
			  nY -= CENTERY;
		   }
		}

		if ((bRenderFlag & RUIMAGE_RENDER_FLAG_FRAME_DRAW) == 0)
		{//帧内有偏移的
			nX += nSprInfo.pFoffsetX;
			nY += nSprInfo.pFoffsetY;
		}
		//开始裁剪

		*mScreeX = nX;
		*mScreeY = nY;
	return bgCur;
}
