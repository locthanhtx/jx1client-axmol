#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiAutoPlay_vn.h"

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
#include "gamecore/KNpc.h"
#include "gamecore/KNpcSet.h"
#include "gamecore/KPlayer.h"
#include "gamecore/KPlayerSet.h"
#include "gamecore/KSubWorldSet.h"
#include "gamecore/KSkillManager.h"
#include "gamecore/KItem.h"
#include "gamescene/KScenePlaceC.h"

#ifdef WIN32
#include <direct.h>
#else
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

/*#ifndef WIN32
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#endif*/
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

/*#define  SCHEME_INI_AUTO_SETING "jx50ai/autoset/%u_set.ini"
#define  SCHEME_INI_AUTO_ITEM   "jx50ai/equipName.txt"
#define  SCHEME_INI_AUTO_DAOJU  "jx50ai/itemName.txt"
#define  SCHEME_INI_AUTO_USE    "jx50ai/itemUseName.txt"
#define  SCHEME_INI_AUTO_SKILL  "\\settings\\Ai\\skill.txt"
#define  CONTROL_SCALE 0.5
#define	 LIFE_TIME_DISTANCE_DEFAULT		50
#define	 AUTO_TIME_SAY_DEFAULT          18
#define	 LIFE_TIME_USERITEM_DEFAULT		180
*/
KuiAutoPlay_vn::KuiAutoPlay_vn()
{
	isOpen = false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	//pMoneyLabel       = NULL;
	//pXuLabel          = NULL;
//	colorLayer        = NULL;
	red               = NULL;

	m_uLifeTime = 0;
	m_uManaTime = 0;
	m_PortalTime = 0;
	m_uMoneyTime = 0;
	nUseItemLoopTime = 0;
	g_nServiceLoop   = 0;
	m_pUiLayer = NULL;
	m_pWidget  = NULL;
//	m_array    = NULL;
	listView_Item   = NULL;
	listView_tool   = NULL;
	listView_use    = NULL;
	btnPageBox_1= NULL;
	btnPageBox_2= NULL;
	btnPageBox_3= NULL;
	btnPageBox_4= NULL;
	btnPageBox_5= NULL;
	clientSaveDir = "";
	checkBox_baohu = NULL;
	checkBox_zhandou = NULL;
	checkBox_shiqu   = NULL;

	pshuxEditBox =NULL;
	pstrEditBox_s=NULL;
	pstrEditBox_sz=NULL;
	pstrEditBox_d=NULL;
	pstrEditBox_u=NULL;
	pEditBox_yanshi = NULL;

	switchControl_baohu = NULL;
	switchControl_team  = NULL;
	switchControl_team_g = NULL;
	switchControl_zhandou = NULL;
	switchControl_shiqu = NULL;
	switchControl_savem = NULL;
	switchControl_fenjie = NULL;
	switchControl_yanshi = NULL;
	switchControl_team_y =NULL;
	switchControl_team_d =NULL;
	switchControl_yabiao =NULL;
	switchControl_baoliu =NULL;
	switchControl_useitem=NULL;
	switchControl_luxian =NULL;
	switchControl_dingdian=NULL;
	switchControl_luzhi =NULL;
	switchControl_miaosha = NULL;
	switchControl_other_s = NULL;
	nFactionNo = 0;
	int i;
	for (i = 0; i < MAX_ITEMLIST_COUNT; ++i)
	{
		custom_button_item[i] = NULL;
		custom_button_tool[i] = NULL;
		custom_button_use[i]  = NULL;
	}
	for (i = 0; i < MAX_SKILL_COUNT; ++i)
	{
		switchControl_skill[i] = NULL;
		skillnameLabel[i] = NULL;
	}
	nTempSelItemIndex = -1;
	nTempSeltoolIndex = -1;
	nTempSeluseIndex  = -1;

	_button_1 =NULL;
	_button_2 =NULL;
	_button   =NULL;

	ZeroMemory(nFilePath,sizeof(nFilePath));
}

KuiAutoPlay_vn::~KuiAutoPlay_vn()
{
	isOpen = false;
	//pMoneyLabel  = NULL;
//	colorLayer   = NULL;
//	CC_SAFE_RELEASE(m_array);
	if (red)
	    red->release();

	nAutoNeiGua.Clear();
	nAutoSkill.Clear();
	nAutoZhuang.Clear();
	nAutoDaiju.Clear();
	nAutoUseItem.Clear();
	//Terminate(1);
}

//创建确定弹出框
KuiAutoPlay_vn* KuiAutoPlay_vn::create(const char* title,
                                       Ref* callbackListener,
                                       const std::function<void(ax::Node*)>& callfun)
{
	if (!g_GameWorld) return NULL;
	KuiAutoPlay_vn * popLayer = KuiAutoPlay_vn::create();
	popLayer->addDialogData();
	//设置按钮
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//初始化
bool KuiAutoPlay_vn::init()
{
	if ( !Layer::init() ){
		return false;
	}
	//开启触摸响应
	this->scheduleUpdate();                   //调用函数
    auto listener = ax::EventListenerTouchOneByOne::create();
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	//精灵帧缓存
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//添加模态背景
	winSize = ax::Director::getInstance()->getWinSize();
	/*Color4B color(112, 128, 144, 150);
	LayerColor * colorLayer = LayerColor::create(color);//颜色层
	colorLayer->setPosition(ax::Vec2::ZERO);
	//colorLayer->setContentSize(winSize);//设置和窗口的大小
	this->addChild(colorLayer);
	*/
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	// sprintf(nSprName,"\\Spr\\Ui3\\任务记事\\任务记事.spr");
	// g_StrLower(nSprName);
	// int m_nWidth,m_nHeight,nFrams;
	// Texture2D *bgCur = NULL;
	// SPRFRAMSINFO nSprInfo;
	// ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	// bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	// if (bgCur==NULL)
	// 	return false;
	// Sprite * spriteBox = Sprite::createWithTexture(bgCur);
	//ui/dialogBig_bg.png
	Sprite * spriteBox = Sprite::create("transcene.png");
	// Sprite * spriteBox = Sprite::create("ui/dialogBig_bg_small.png");
	// Label * ptitlebgLabel = Label::createWithTTF("Cài đặt Auto",UI_GAME_FONT_DEFAULT_VN_BTN, 22,spriteBox->getContentSize(),ax::TextHAlignment::LEFT);
	// ptitlebgLabel->setColor(ax::Color3B::WHITE);
	// ptitlebgLabel->setAnchorPoint(ax::Vec2(0,0));
	// ptitlebgLabel->enableOutline(Color4B(0, 0, 0, 255), 4);
	// ptitlebgLabel->setPosition(ax::Vec2(spriteBox->getContentSize().width/4,spriteBox->getContentSize().height/2));
	// spriteBox->addChild(ptitlebgLabel);
	ParentNode_Team= spriteBox;
	spriteBox->setPosition(ax::Vec2(winSize.width/2,winSize.height/2));
	spriteBox->setOpacity(100);
	this->addChild(spriteBox);
	//this->addChild(ptitlebgLabel,1);
	//获得背景图片的大小
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();
	clientSaveDir = creatAutoPlayDir("jx50ai");
	char nTempDir[128];
	sprintf(nTempDir,SCHEME_INI_AUTO_SETING);
	CreateFolder(nTempDir);
	//creatAutoPlayDir("jx50ai");
	//colorLayer->setContentSize(contentSize);//设置和窗口的大小

	return true;
}

void KuiAutoPlay_vn::addDialogData()
{
	if  (!g_pCoreShell) return;
	//CheckBox::
	//Left=24
	//Top=72
	//m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	//m_StartPos.y =72;//+ITEM_CELL_SIZE/2;

	btnSize.width = 110;
	btnSize.height= 16;
	g_pCoreShell->GetGameData(GDI_LAOHUJI_INFO,130,0);
	g_ScenePlace.GetSceneNameAndFocus(nMapName,nMapidx,nPosX,nPosY,nMapKind);
	sprintf(nFilePath,SCHEME_INI_AUTO_SETING,g_FileName2Id(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name));
	if (!nAutoNeiGua.Load(nFilePath,TRUE))
	{
		messageBox("Missing the config file","Warning");
	}
	if (!nAutoSkill.Load(SCHEME_INI_AUTO_SKILL))
	{
		messageBox("Missing the config file of skill","Warning");
	}

	if (!nAutoZhuang.Load(SCHEME_INI_AUTO_ITEM))
	{
		if (nAutoZhuang.CreatFile(SCHEME_INI_AUTO_ITEM))
		{//创建文件
			if (!nAutoZhuang.Load(SCHEME_INI_AUTO_ITEM))
			{
				messageBox("Missing the config file of Attributes","Warning");
			}
		}
	}
	if (!nAutoDaiju.Load(SCHEME_INI_AUTO_DAOJU))
	{
		if (nAutoDaiju.CreatFile(SCHEME_INI_AUTO_DAOJU))
		{//创建文件
			if (!nAutoDaiju.Load(SCHEME_INI_AUTO_DAOJU))
			{
				messageBox("Missing the config file of itemtool","Warning");
			}
		}
	}

	if (!nAutoUseItem.Load(SCHEME_INI_AUTO_USE))
	{
		if (nAutoUseItem.CreatFile(SCHEME_INI_AUTO_USE))
		{//创建文件
			if (!nAutoUseItem.Load(SCHEME_INI_AUTO_USE))
			{
				messageBox("Missing the config file of tiems","Warning");
			}
		}
	}
	m_pUiLayer = Layer::create();
	ParentNode_Team->addChild(m_pUiLayer);

	// ImageView* titleImg =ImageView::create();
	// titleImg->loadTexture("ui/dialogBig_bg.png");
	// //titleImg->setContentSize(ax::Vec2(400,200));
	// titleImg->setScale(0.5);
	// //titleImg->setAnchorPoint(ax::Vec2(0,0));
	// titleImg->setPosition(ax::Vec2(m_size.width/2,m_size.height-13));
	// m_pUiLayer->addChild(titleImg);

	// Create the checkbox
	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui4\\common\\勾选.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Texture2D * check_box_normal = bgCur;

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Texture2D * check_box_active = bgCur;
	Texture2D * check_box_active_disable = bgCur;

	/*bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;

	Texture2D * check_box_active_disable = bgCur;
	*/
	Color4B color(112, 128, 144,0);
	Size nPageSize    = ax::Size(m_size.width-35,m_size.height-63);
	Point CCPointPage(50,-10);
	int nisOpen=0,nsaveMonye=0;
	_PageLayer_1 = Node::create();//Layer::create();//基本设置
	//_PageLayer_1->setContentSize(nPageSize);
	_PageLayer_1->setPosition(CCPointPage);
	//_PageLayer_1->setVisible(false);
	ParentNode_Team->addChild(_PageLayer_1);

    Size tempSize;
    tempSize.width  = 200;
    tempSize.height = 100;
//------------基本设置面板-------------------------
	Label* pBaohuSetLabel = Label::createWithTTF("Thiết lập hồi phục:",/*"fonts/VnTahoma.ttf"*/UI_GAME_FONT_DEFAULT, 16);
    pBaohuSetLabel->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    pBaohuSetLabel->setVerticalAlignment(TextVAlignment::TOP);//���¾���
	// pBaohuSetLabel->setAnchorPoint(ax::Vec2(0,0));
	pBaohuSetLabel->setColor(ax::Color3B::YELLOW);
	pBaohuSetLabel->setPosition(ax::Vec2(125,nPageSize.height-50-50));
	// _PageLayer_1->addChild(pBaohuSetLabel);
    // listView_Item->pushBackCustomItem(pBaohuSetLabel);

	checkBox_Auto_HP = CheckBox::create();
	checkBox_Auto_HP->setEnabled(true);
    //[TODO][zer0kull]
//	checkBox_Auto_HP->loadTexturesBydata(check_box_normal,
//		check_box_active_disable,
//		check_box_active_disable,check_box_normal,check_box_active);
	checkBox_Auto_HP->setScale(3);
    // checkBox_Auto_HP->setAnchorPoint(ax::Vec2(0,0));
	// checkBox_Auto_HP->setPosition(ax::Vec2(pBaohuSetLabel->getPosition().x+15,pBaohuSetLabel->getPosition().y-50));
	checkBox_Auto_HP->setPosition(ax::Vec2(0,-20));
	checkBox_Auto_HP->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::checkBoxStateEvent, this));
	checkBox_Auto_HP->setTag(1001);
	// m_pUiLayer->addChild(checkBox_Auto_HP);

	// _PageLayer_1->addChild(checkBox_Auto_HP);
    // listView_Item->pushBackCustomItem(checkBox_Auto_HP);

	checkBox_Auto_HP->setSelected(true);


	char nTempStr[32];
	nAutoNeiGua.GetString("AAAA","IsOpenBaohu","0",nTempStr,sizeof(nTempStr));
	int nTempInt = g_Atoui(nTempStr);

	ax::ui::Scale9Sprite *pBg;

	Label* plifeLabel = Label::createWithTTF("Sinh lực(%)",UI_GAME_FONT_DEFAULT, 16);
    plifeLabel->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    plifeLabel->setVerticalAlignment(TextVAlignment::TOP);//���¾���
	// plifeLabel->setAnchorPoint(ax::Vec2(0,0));
	// plifeLabel->setPosition(ax::Vec2(checkBox_Auto_HP->getPosition().x,checkBox_Auto_HP->getPosition().y+15)); //switchControl_baohu->getPosition().x+30
	// _PageLayer_1->addChild(plifeLabel);
    // listView_Item->pushBackCustomItem(plifeLabel);

	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	//pBg->setColor(ax::Color3B::GREEN);

	pLifeEditBox = EditBox::create(Size(50,30),pBg);
	// const ax::Color3B nfcolor ={255,255,255};
	if (pLifeEditBox)
	{
		//pEditBox->setContentSize()
		//pMoneyEditBox->setText("your money");
		pLifeEditBox->setPlaceHolder("life");
		pLifeEditBox->setFontColor(ax::Color3B::YELLOW);
		pLifeEditBox->setFontSize(16);
		pLifeEditBox->setMaxLength(3);
		// pLifeEditBox->setPosition(ax::Vec2(plifeLabel->getPosition().x+125,plifeLabel->getPosition().y+15));
		pLifeEditBox->setInputMode(EditBox::InputMode::DECIMAL);
		pLifeEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		// _PageLayer_1->addChild(pLifeEditBox);

		nAutoNeiGua.GetString("AAAA","nBaohuInfo1","50",nTempStr,sizeof(nTempStr));
		//nTempInt = g_Atoui(nTempStr);
		pLifeEditBox->setText(nTempStr);
	}

	Label* plifeLabel_2 = Label::createWithTTF("Mức 2 (%)",UI_GAME_FONT_DEFAULT, 16);
    plifeLabel_2->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    plifeLabel_2->setVerticalAlignment(TextVAlignment::TOP);//���¾���
	// plifeLabel_2->setAnchorPoint(ax::Vec2(0,0));
	// plifeLabel_2->setPosition(ax::Vec2(pLifeEditBox->getPosition().x+50,pLifeEditBox->getPosition().y-15)); //switchControl_baohu->getPosition().x+30
	// _PageLayer_1->addChild(plifeLabel_2);
    // listView_Item->pushBackCustomItem(plifeLabel_2);

	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	//pBg->setColor(ax::Color3B::GREEN);
	pLifeEditBox_2 = EditBox::create(Size(50,30),pBg);
	// const ax::Color3B nfcolor ={255,255,255};
	if (pLifeEditBox_2)
	{
		//pEditBox->setContentSize()
		//pMoneyEditBox->setText("your money");
		pLifeEditBox_2->setPlaceHolder("life_2");
		pLifeEditBox_2->setFontColor(ax::Color3B::YELLOW);
		pLifeEditBox_2->setFontSize(16);
		pLifeEditBox_2->setMaxLength(3);
		// pLifeEditBox_2->setPosition(ax::Vec2(pLifeEditBox->getPosition().x+100+50+10,pLifeEditBox->getPosition().y));
		pLifeEditBox_2->setInputMode(EditBox::InputMode::DECIMAL);
		pLifeEditBox_2->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		// _PageLayer_1->addChild(pLifeEditBox_2);
        // listView_Item->pushBackCustomItem(pLifeEditBox_2);

		nAutoNeiGua.GetString("AAAA","nBaohuInfo1","50",nTempStr,sizeof(nTempStr));
		//nTempInt = g_Atoui(nTempStr);
		pLifeEditBox_2->setText(nTempStr);
	}

	Label* plifeLabel_3 = Label::createWithTTF("Số lượng dùng",UI_GAME_FONT_DEFAULT, 16);
    plifeLabel_3->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    plifeLabel_3->setVerticalAlignment(TextVAlignment::TOP);//���¾���
	// plifeLabel_3->setAnchorPoint(ax::Vec2(0,0));
	// plifeLabel_3->setPosition(ax::Vec2(pLifeEditBox->getPosition().x+100+100+25+25,pLifeEditBox->getPosition().y-15)); //switchControl_baohu->getPosition().x+30
	// _PageLayer_1->addChild(plifeLabel_3);
    // listView_Item->pushBackCustomItem(plifeLabel_3);

	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	//pBg->setColor(ax::Color3B::GREEN);
	EditBox* pLifeEditBox_3 = EditBox::create(Size(50,30),pBg);
	// const ax::Color3B nfcolor ={255,255,255};
	if (pLifeEditBox_3)
	{
		//pEditBox->setContentSize()
		//pMoneyEditBox->setText("your money");
		pLifeEditBox_3->setPlaceHolder("life_2");
		pLifeEditBox_3->setFontColor(ax::Color3B::YELLOW);
		pLifeEditBox_3->setFontSize(16);
		pLifeEditBox_3->setMaxLength(3);
		// pLifeEditBox_3->setPosition(ax::Vec2(pLifeEditBox->getPosition().x+100+100+100+50+50,pLifeEditBox->getPosition().y));
		pLifeEditBox_3->setInputMode(EditBox::InputMode::DECIMAL);
		pLifeEditBox_3->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		// _PageLayer_1->addChild(pLifeEditBox_3);
        // listView_Item->pushBackCustomItem(pLifeEditBox_3);

		nAutoNeiGua.GetString("AAAA","nBaohuInfo1","50",nTempStr,sizeof(nTempStr));
		//nTempInt = g_Atoui(nTempStr);
		pLifeEditBox_3->setText(nTempStr);
	}


	Label* plifeLabel_4 = Label::createWithTTF("Thời gian(ms)",UI_GAME_FONT_DEFAULT, 16);
    plifeLabel_4->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    plifeLabel_4->setVerticalAlignment(TextVAlignment::TOP);//���¾���
	// plifeLabel_3->setAnchorPoint(ax::Vec2(0,0));
	// plifeLabel_4->setAnchorPoint(ax::Vec2(0,0));
	// plifeLabel_4->setPosition(ax::Vec2(pLifeEditBox->getPosition().x+100+100+100+100+25+25+25,pLifeEditBox->getPosition().y-15)); //switchControl_baohu->getPosition().x+30
	// _PageLayer_1->addChild(plifeLabel_4);

	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	//pBg->setColor(ax::Color3B::GREEN);
	EditBox* pLifeEditBox_4 = EditBox::create(Size(50,30),pBg);
	// const ax::Color3B nfcolor ={255,255,255};
	if (pLifeEditBox_4)
	{
		//pEditBox->setContentSize()
		//pMoneyEditBox->setText("your money");
		pLifeEditBox_4->setPlaceHolder("life_2");
		pLifeEditBox_4->setFontColor(ax::Color3B::YELLOW);
		pLifeEditBox_4->setFontSize(16);
		pLifeEditBox_4->setMaxLength(3);
		// pLifeEditBox_4->setPosition(ax::Vec2(pLifeEditBox->getPosition().x+100+100+100+100+25+50+125+25,pLifeEditBox->getPosition().y));
		pLifeEditBox_4->setInputMode(EditBox::InputMode::DECIMAL);
		pLifeEditBox_4->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		// _PageLayer_1->addChild(pLifeEditBox_4);

		nAutoNeiGua.GetString("AAAA","nBaohuInfo1","50",nTempStr,sizeof(nTempStr));
		//nTempInt = g_Atoui(nTempStr);
		pLifeEditBox_4->setText(nTempStr);
	}

// void 	setHorizontalAlignment (CCTextAlignment alignment)
// CCVerticalTextAlignment 	getVerticalAlignment ()
// void 	setVerticalAlignment (CCVerticalTextAlignment verticalAlignment)

    // Menu* pMenu=NULL;
    // pMenu = Menu::create((MenuItem *)pBaohuSetLabel,(MenuItem *)checkBox_Auto_HP,(MenuItem *)plifeLabel,(MenuItem *)pLifeEditBox,(MenuItem *)plifeLabel_2,(MenuItem *)pLifeEditBox_2,(MenuItem *)plifeLabel_3,(MenuItem *)pLifeEditBox_3,(MenuItem *)plifeLabel_4,(MenuItem *)pLifeEditBox_4,NULL);
    // // pMenu = Menu::create((MenuItem *)checkBox_Auto_HP,(MenuItem *)plifeLabel,(MenuItem *)pLifeEditBox,(MenuItem *)plifeLabel_2,(MenuItem *)pLifeEditBox_2,(MenuItem *)plifeLabel_3,(MenuItem *)pLifeEditBox_3,(MenuItem *)plifeLabel_4,(MenuItem *)pLifeEditBox_4,NULL);
    // // pMenu->alignItemsVertically();//指定间隔
    // // pMenu->alignItemsHorizontallyWithPadding(0);
    // // pMenu->alignItemsVertically();//指定间隔
    // // pMenu->alignItemsInRows(1,1,1,1,1,1,1,1,1,NULL);//指定间隔
    // pMenu->setAnchorPoint(ax::Vec2(0,0));
    // pMenu->alignItemsInColumns(1,9,9,9,9,9,9,9,9,9,NULL);
    // pMenu->setPosition(ax::Vec2(525,500+125));//ax::Vec2::ZERO
    // pMenu->setEnabled(true);
    // _PageLayer_1->addChild(pBaohuSetLabel);
    // _PageLayer_1->addChild(pMenu);
    // m_pUiLayer->addChild(checkBox_Auto_HP);



// NOI LUC
	CheckBox* checkBox_Auto_MANA = CheckBox::create();
	checkBox_Auto_MANA->setEnabled(true);
    //[TODO][zer0kull]
//	checkBox_Auto_MANA->loadTexturesBydata(check_box_normal,
//		check_box_active_disable,
//		check_box_active_disable,check_box_normal,check_box_active);
	checkBox_Auto_MANA->setScale(3);
    checkBox_Auto_MANA->setPosition(ax::Vec2(0,-20));
	checkBox_Auto_MANA->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::checkBoxStateEvent, this));
	checkBox_Auto_MANA->setTag(1002);
	checkBox_Auto_MANA->setSelected(true);
	Label* pManaLabel = Label::createWithTTF("Nội lực(%)",UI_GAME_FONT_DEFAULT, 16);
    pManaLabel->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    pManaLabel->setVerticalAlignment(TextVAlignment::TOP);//���¾���
    pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	EditBox* pManaEditBox = EditBox::create(Size(50,30),pBg);

	if (pManaEditBox)
	{
		pManaEditBox->setPlaceHolder("life");
		pManaEditBox->setFontColor(ax::Color3B::YELLOW);
		pManaEditBox->setFontSize(16);
		pManaEditBox->setMaxLength(3);
		pManaEditBox->setInputMode(EditBox::InputMode::DECIMAL);
		pManaEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		nAutoNeiGua.GetString("AAAA","nBaohuInfo1","50",nTempStr,sizeof(nTempStr));
		pManaEditBox->setText(nTempStr);
	}

	Label* pManaLabel_2 = Label::createWithTTF("Mức 2 (%)",UI_GAME_FONT_DEFAULT, 16);
    pManaLabel_2->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    pManaLabel_2->setVerticalAlignment(TextVAlignment::TOP);//���¾���
    pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	EditBox* pManaEditBox_2 = EditBox::create(Size(50,30),pBg);
	if (pManaEditBox_2)
	{
		pManaEditBox_2->setPlaceHolder("life_2");
		pManaEditBox_2->setFontColor(ax::Color3B::YELLOW);
		pManaEditBox_2->setFontSize(16);
		pManaEditBox_2->setMaxLength(3);
		pManaEditBox_2->setInputMode(EditBox::InputMode::DECIMAL);
		pManaEditBox_2->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		nAutoNeiGua.GetString("AAAA","nBaohuInfo1","50",nTempStr,sizeof(nTempStr));
		pManaEditBox_2->setText(nTempStr);
	}

	Label* pManaLabel_3 = Label::createWithTTF("Số lượng dùng",UI_GAME_FONT_DEFAULT, 16);
    pManaLabel_3->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    pManaLabel_3->setVerticalAlignment(TextVAlignment::TOP);//���¾���
    pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	EditBox* pManaEditBox_3 = EditBox::create(Size(50,30),pBg);
	if (pManaEditBox_3)
	{
		pManaEditBox_3->setPlaceHolder("life_2");
		pManaEditBox_3->setFontColor(ax::Color3B::YELLOW);
		pManaEditBox_3->setFontSize(16);
		pManaEditBox_3->setMaxLength(3);
		pManaEditBox_3->setInputMode(EditBox::InputMode::DECIMAL);
		pManaEditBox_3->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		nAutoNeiGua.GetString("AAAA","nBaohuInfo1","50",nTempStr,sizeof(nTempStr));
		pManaEditBox_3->setText(nTempStr);
	}


	Label* pManaLabel_4 = Label::createWithTTF("Thời gian(ms)",UI_GAME_FONT_DEFAULT, 16);
    pManaLabel_4->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    pManaLabel_4->setVerticalAlignment(TextVAlignment::TOP);//���¾���
	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	EditBox* pManaEditBox_4 = EditBox::create(Size(50,30),pBg);
	if (pManaEditBox_4)
	{
		pManaEditBox_4->setPlaceHolder("life_2");
		pManaEditBox_4->setFontColor(ax::Color3B::YELLOW);
		pManaEditBox_4->setFontSize(16);
		pManaEditBox_4->setMaxLength(3);
		pManaEditBox_4->setInputMode(EditBox::InputMode::DECIMAL);
		pManaEditBox_4->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		nAutoNeiGua.GetString("AAAA","nBaohuInfo1","50",nTempStr,sizeof(nTempStr));
		pManaEditBox_4->setText(nTempStr);
	}

    // Menu* pMenu_Mana=NULL;
    // pMenu_Mana = Menu::create((MenuItem *)checkBox_Auto_MANA,(MenuItem *)pManaLabel,(MenuItem *)pManaEditBox,(MenuItem *)pManaLabel_2,(MenuItem *)pManaEditBox_2,(MenuItem *)pManaLabel_3,(MenuItem *)pManaEditBox_3,(MenuItem *)pManaLabel_4,(MenuItem *)pManaEditBox_4,NULL);
    // pMenu_Mana->setAnchorPoint(ax::Vec2(0,0));
    // pMenu_Mana->alignItemsInColumns(9,9,9,9,9,9,9,9,9,9,NULL);
    // pMenu_Mana->setPosition(ax::Vec2(525,500+125-0-75));//ax::Vec2::ZERO
    // pMenu_Mana->setEnabled(true);
    // _PageLayer_1->addChild(pMenu_Mana);
    // m_pUiLayer->addChild(checkBox_Auto_MANA);

//Tho Dia Phu
	Label* pAuto_TDP_Lable_Group = Label::createWithTTF("Thiết lập thổ địa phù:",/*"fonts/VnTahoma.ttf"*/UI_GAME_FONT_DEFAULT, 16);
    pAuto_TDP_Lable_Group->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    pAuto_TDP_Lable_Group->setVerticalAlignment(TextVAlignment::TOP);//���¾���
	// pBaohuSetLabel->setAnchorPoint(ax::Vec2(0,0));
	pAuto_TDP_Lable_Group->setColor(ax::Color3B::YELLOW);
	pAuto_TDP_Lable_Group->setPosition(ax::Vec2(125,nPageSize.height-50-50));

    CheckBox* pAuto_TDP_CheckBox_TDPSinhLuc = CheckBox::create();
	pAuto_TDP_CheckBox_TDPSinhLuc->setEnabled(true);
    //[TODO][zer0kull]
//	pAuto_TDP_CheckBox_TDPSinhLuc->loadTexturesBydata(check_box_normal,
//		check_box_active_disable,
//		check_box_active_disable,check_box_normal,check_box_active);
	pAuto_TDP_CheckBox_TDPSinhLuc->setScale(3);
    pAuto_TDP_CheckBox_TDPSinhLuc->setPosition(ax::Vec2(0,-20));
	pAuto_TDP_CheckBox_TDPSinhLuc->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::checkBoxStateEvent, this));
	pAuto_TDP_CheckBox_TDPSinhLuc->setTag(1003);
	pAuto_TDP_CheckBox_TDPSinhLuc->setSelected(true);
	Label* pAuto_TDP_Lable_HP = Label::createWithTTF("Sinh lực(%)",UI_GAME_FONT_DEFAULT, 16);//Arial
    pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	EditBox* pAuto_TDP_EditBox_HP = EditBox::create(Size(50,30),pBg);

	if (pAuto_TDP_EditBox_HP)
	{
		pAuto_TDP_EditBox_HP->setPlaceHolder("life");
		pAuto_TDP_EditBox_HP->setFontColor(ax::Color3B::YELLOW);
		pAuto_TDP_EditBox_HP->setFontSize(16);
		pAuto_TDP_EditBox_HP->setMaxLength(3);
		pAuto_TDP_EditBox_HP->setInputMode(EditBox::InputMode::DECIMAL);
		pAuto_TDP_EditBox_HP->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		nAutoNeiGua.GetString("AAAA","nBaohuInfo1","50",nTempStr,sizeof(nTempStr));
		pAuto_TDP_EditBox_HP->setText(nTempStr);
	}

	Label* pAuto_TDP_Lable_HetBinhSinhLuc = Label::createWithTTF("Hết bình sinh lực",UI_GAME_FONT_DEFAULT, 16);
    pAuto_TDP_Lable_HetBinhSinhLuc->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    pAuto_TDP_Lable_HetBinhSinhLuc->setVerticalAlignment(TextVAlignment::TOP);//���¾���
    CheckBox* pAuto_TDP_CheckBox_HetBinhSinhLuc = CheckBox::create();
	pAuto_TDP_CheckBox_HetBinhSinhLuc->setEnabled(true);
    //[TODO][zer0kull]
//	pAuto_TDP_CheckBox_HetBinhSinhLuc->loadTexturesBydata(check_box_normal,
//		check_box_active_disable,
//		check_box_active_disable,check_box_normal,check_box_active);
	pAuto_TDP_CheckBox_HetBinhSinhLuc->setScale(3);
    pAuto_TDP_CheckBox_HetBinhSinhLuc->setPosition(ax::Vec2(0,-20));
	pAuto_TDP_CheckBox_HetBinhSinhLuc->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::checkBoxStateEvent, this));
	pAuto_TDP_CheckBox_HetBinhSinhLuc->setTag(1004);
	pAuto_TDP_CheckBox_HetBinhSinhLuc->setSelected(true);

	Label* pAuto_TDP_Lable_HetBinhNoiLuc = Label::createWithTTF("Hết bình nội lực",UI_GAME_FONT_DEFAULT, 16);
    pAuto_TDP_Lable_HetBinhNoiLuc->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    pAuto_TDP_Lable_HetBinhNoiLuc->setVerticalAlignment(TextVAlignment::TOP);//���¾���
    CheckBox* pAuto_TDP_CheckBox_HetBinhNoiLuc = CheckBox::create();
	pAuto_TDP_CheckBox_HetBinhNoiLuc->setEnabled(true);
    //[TODO][zer0kull]
//	pAuto_TDP_CheckBox_HetBinhNoiLuc->loadTexturesBydata(check_box_normal,
//		check_box_active_disable,
//		check_box_active_disable,check_box_normal,check_box_active);
	pAuto_TDP_CheckBox_HetBinhNoiLuc->setScale(3);
    pAuto_TDP_CheckBox_HetBinhNoiLuc->setPosition(ax::Vec2(0,-20));
	pAuto_TDP_CheckBox_HetBinhNoiLuc->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::checkBoxStateEvent, this));
	pAuto_TDP_CheckBox_HetBinhNoiLuc->setTag(1005);
	pAuto_TDP_CheckBox_HetBinhNoiLuc->setSelected(true);


	Label* pAuto_TDP_Lable_DoBenTrangBi = Label::createWithTTF("Độ bền trang bị <",UI_GAME_FONT_DEFAULT, 16);
    pAuto_TDP_Lable_DoBenTrangBi->setHorizontalAlignment(ax::TextHAlignment::LEFT);        //����λ��
    pAuto_TDP_Lable_DoBenTrangBi->setVerticalAlignment(TextVAlignment::TOP);//���¾���
    CheckBox* pAuto_TDP_CheckBox_DoBenTrangBi = CheckBox::create();
	pAuto_TDP_CheckBox_DoBenTrangBi->setEnabled(true);
    //[TODO][zer0kull]
//	pAuto_TDP_CheckBox_DoBenTrangBi->loadTexturesBydata(check_box_normal,
//		check_box_active_disable,
//		check_box_active_disable,check_box_normal,check_box_active);
	pAuto_TDP_CheckBox_DoBenTrangBi->setScale(3);
    pAuto_TDP_CheckBox_DoBenTrangBi->setPosition(ax::Vec2(0,-20));
	pAuto_TDP_CheckBox_DoBenTrangBi->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::checkBoxStateEvent, this));
	pAuto_TDP_CheckBox_DoBenTrangBi->setTag(1006);
	pAuto_TDP_CheckBox_DoBenTrangBi->setSelected(true);

	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	EditBox* pAuto_TDP_EditBox_DoBenTrangBi = EditBox::create(Size(50,30),pBg);
	if (pAuto_TDP_EditBox_DoBenTrangBi)
	{
		pAuto_TDP_EditBox_DoBenTrangBi->setPlaceHolder("life_2");
		pAuto_TDP_EditBox_DoBenTrangBi->setFontColor(ax::Color3B::YELLOW);
		pAuto_TDP_EditBox_DoBenTrangBi->setFontSize(16);
		pAuto_TDP_EditBox_DoBenTrangBi->setMaxLength(3);
		pAuto_TDP_EditBox_DoBenTrangBi->setInputMode(EditBox::InputMode::DECIMAL);
		pAuto_TDP_EditBox_DoBenTrangBi->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		nAutoNeiGua.GetString("AAAA","nBaohuInfo1","50",nTempStr,sizeof(nTempStr));
		pAuto_TDP_EditBox_DoBenTrangBi->setText(nTempStr);
	}

// Group Menu
    Menu* pMenu_All=NULL;
    pMenu_All = Menu::create(
        (MenuItem *)pBaohuSetLabel,
        (MenuItem *)checkBox_Auto_HP,(MenuItem *)plifeLabel,(MenuItem *)pLifeEditBox,(MenuItem *)plifeLabel_2,(MenuItem *)pLifeEditBox_2,(MenuItem *)plifeLabel_3,(MenuItem *)pLifeEditBox_3,(MenuItem *)plifeLabel_4,(MenuItem *)pLifeEditBox_4,
        (MenuItem *)checkBox_Auto_MANA,(MenuItem *)pManaLabel,(MenuItem *)pManaEditBox,(MenuItem *)pManaLabel_2,(MenuItem *)pManaEditBox_2,(MenuItem *)pManaLabel_3,(MenuItem *)pManaEditBox_3,(MenuItem *)pManaLabel_4,(MenuItem *)pManaEditBox_4,
        (MenuItem *)pAuto_TDP_Lable_Group,
        (MenuItem *)pAuto_TDP_CheckBox_TDPSinhLuc,(MenuItem *)pAuto_TDP_Lable_HP,(MenuItem *)pAuto_TDP_CheckBox_HetBinhSinhLuc,(MenuItem *)pAuto_TDP_Lable_HetBinhSinhLuc,(MenuItem *)pAuto_TDP_CheckBox_HetBinhNoiLuc,(MenuItem *)pAuto_TDP_Lable_HetBinhNoiLuc,(MenuItem *)pAuto_TDP_CheckBox_DoBenTrangBi,(MenuItem *)pAuto_TDP_Lable_DoBenTrangBi,(MenuItem *)pAuto_TDP_EditBox_DoBenTrangBi,
        NULL);
    // Menu::create();
    pMenu_All->setAnchorPoint(ax::Vec2(0,0));
    pMenu_All->alignItemsInColumns(1,9,9,1,9,9,9,9,9,9,NULL);
    pMenu_All->setPosition(ax::Vec2(525,500+125-100));//ax::Vec2::ZERO
    pMenu_All->setEnabled(true);
    _PageLayer_1->addChild(pMenu_All);
    m_pUiLayer->addChild(checkBox_Auto_HP);
    m_pUiLayer->addChild(checkBox_Auto_MANA);
    m_pUiLayer->addChild(pAuto_TDP_CheckBox_DoBenTrangBi);
    m_pUiLayer->addChild(pAuto_TDP_CheckBox_HetBinhNoiLuc);
    m_pUiLayer->addChild(pAuto_TDP_CheckBox_HetBinhSinhLuc);
    m_pUiLayer->addChild(pAuto_TDP_CheckBox_TDPSinhLuc);


    return; //1001

// 	Label* pheyaoLabel = Label::createWithTTF("Use medicine",UI_GAME_FONT_DEFAULT,12);//Arial
// 	pheyaoLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pheyaoLabel->setPosition(ax::Vec2(pLifeEditBox->getPosition().x+pLifeEditBox->getContentSize().width-20,nPageSize.height-45));
// 	_PageLayer_1->addChild(pheyaoLabel);


// 	Label* pneiliLabel = Label::createWithTTF("energy",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pneiliLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pneiliLabel->setPosition(ax::Vec2(pheyaoLabel->getPosition().x+pheyaoLabel->getContentSize().width+10,nPageSize.height-45));
// 	_PageLayer_1->addChild(pneiliLabel);

// 	          pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	pNeiliEditBox = EditBox::create(Size(50,24),pBg);
// 	if (pNeiliEditBox)
// 	{
// 		pNeiliEditBox->setPlaceHolder("energy");
// 		pNeiliEditBox->setFontColor(ax::Color3B::YELLOW);
// 		pNeiliEditBox->setFontSize(24);
// 		pNeiliEditBox->setMaxLength(6);
// 		pNeiliEditBox->setPosition(ax::Vec2(pneiliLabel->getPosition().x+pneiliLabel->getContentSize().width+30,nPageSize.height-40));
// 		pNeiliEditBox->setInputMode(EditBox::InputMode::DECIMAL);
// 		pNeiliEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		//pMoneyEditBox->setDelegate(this);
// 		//pMoneyEditBox->setDefaultTouchPriority(-128);
// 		_PageLayer_1->addChild(pNeiliEditBox);
// 		nAutoNeiGua.GetString("AAAA","nBaohuInfo2","50",nTempStr,sizeof(nTempStr));
// 		//nTempInt = g_Atoui(nTempStr);
// 		pNeiliEditBox->setText(nTempStr);
// 	}
// 	Label* phelanLabel = Label::createWithTTF("Use medicine",UI_GAME_FONT_DEFAULT,12);//Arial
// 	phelanLabel->setAnchorPoint(ax::Vec2(0,0));
// 	phelanLabel->setPosition(ax::Vec2(pNeiliEditBox->getPosition().x+pNeiliEditBox->getContentSize().width-20,nPageSize.height-35));
// 	_PageLayer_1->addChild(phelanLabel);


// 	Label* plifeLabel_h = Label::createWithTTF("life <=",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	plifeLabel_h->setAnchorPoint(ax::Vec2(0,0));
// 	plifeLabel_h->setPosition(ax::Vec2(10,nPageSize.height-75));
// 	_PageLayer_1->addChild(plifeLabel_h);

// 	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	//pBg->setColor(ax::Color3B::GREEN);
// 	pLifeEditBox_h = EditBox::create(Size(50,24),pBg);
// 	if (pLifeEditBox_h)
// 	{
// 		pLifeEditBox_h->setPlaceHolder("life");
// 		pLifeEditBox_h->setFontColor(ax::Color3B::YELLOW);
// 		pLifeEditBox_h->setFontSize(24);
// 		pLifeEditBox_h->setMaxLength(6);
// 		pLifeEditBox_h->setPosition(ax::Vec2(plifeLabel_h->getPosition().x+plifeLabel_h->getContentSize().width+30,nPageSize.height-65));
// 		pLifeEditBox_h->setInputMode(EditBox::InputMode::DECIMAL);
// 		pLifeEditBox_h->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		_PageLayer_1->addChild(pLifeEditBox_h);
// 		nAutoNeiGua.GetString("AAAA","nBaohuInfo3","50",nTempStr,sizeof(nTempStr));
// 		//nTempInt = g_Atoui(nTempStr);
// 		pLifeEditBox_h->setText(nTempStr);
// 	}
// 	Label* phuichengLabel = Label::createWithTTF("Return to City",UI_GAME_FONT_DEFAULT,12);//Arial
// 	phuichengLabel->setAnchorPoint(ax::Vec2(0,0));
// 	phuichengLabel->setPosition(ax::Vec2(pLifeEditBox_h->getPosition().x+pLifeEditBox_h->getContentSize().width-20,nPageSize.height-75));
// 	_PageLayer_1->addChild(phuichengLabel);

// 	Label* pTeamSetLabel = Label::createWithTTF("Team setting",UI_GAME_FONT_DEFAULT,14);//Arial
// 	pTeamSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pTeamSetLabel->setColor(ax::Color3B::YELLOW);
// 	pTeamSetLabel->setPosition(ax::Vec2(10,nPageSize.height-100));
// 	_PageLayer_1->addChild(pTeamSetLabel);
// 	//跟随队长
// 	Label* pgenshuiOpenLabel = Label::createWithTTF("Follow captain",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pgenshuiOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pgenshuiOpenLabel->setPosition(ax::Vec2(100,nPageSize.height-100));
// 	_PageLayer_1->addChild(pgenshuiOpenLabel);
// 	switchControl_team_g = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_team->setScale(CONTROL_SCALE);
// 	switchControl_team_g->setTag(BOX_TAG_GENSHUI);
// 	switchControl_team_g->setPosition(ax::Vec2(pgenshuiOpenLabel->getPosition().x+pgenshuiOpenLabel->getContentSize().width+20,nPageSize.height-94));
// 	_PageLayer_1->addChild(switchControl_team_g);
// 	switchControl_team_g->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_team_g->setOn(false);
// 	valueChanged(switchControl_team_g, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	//--------
// 	Label* pteamOpenLabel = Label::createWithTTF("Auto promise",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pteamOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pteamOpenLabel->setPosition(ax::Vec2(15,nPageSize.height-120));
// 	_PageLayer_1->addChild(pteamOpenLabel);
// 	switchControl_team = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_team->setScale(CONTROL_SCALE);
// 	switchControl_team->setTag(BOX_TAG_TEAM_1);
// 	switchControl_team->setPosition(ax::Vec2(pteamOpenLabel->getPosition().x+pteamOpenLabel->getContentSize().width+20,nPageSize.height-114));
// 	_PageLayer_1->addChild(switchControl_team);
// 	switchControl_team->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_team->setOn(false);
// 	valueChanged(switchControl_team, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);

// 	Label* pduizhangOpenLabel = Label::createWithTTF(UTEXT("Is Captain",1).c_str(),UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pduizhangOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pduizhangOpenLabel->setPosition(ax::Vec2(130,nPageSize.height-120));
// 	_PageLayer_1->addChild(pduizhangOpenLabel);
// 	switchControl_team_d = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Yse",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("No",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_team->setScale(CONTROL_SCALE);
// 	switchControl_team_d->setTag(BOX_TAG_TEAM_2);
// 	switchControl_team_d->setPosition(ax::Vec2(pduizhangOpenLabel->getPosition().x+pduizhangOpenLabel->getContentSize().width+20,nPageSize.height-114));
// 	_PageLayer_1->addChild(switchControl_team_d);
// 	switchControl_team_d->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_team_d->setOn(false);
// 	//valueChanged(switchControl_team_d, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);


// 	Label* pduiyOpenLabel = Label::createWithTTF("Is Members",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pduiyOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pduiyOpenLabel->setPosition(ax::Vec2(230,nPageSize.height-120));
// 	_PageLayer_1->addChild(pduiyOpenLabel);
// 	switchControl_team_y = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Yes",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("No",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_team->setScale(CONTROL_SCALE);
// 	switchControl_team_y->setTag(BOX_TAG_TEAM_3);
// 	switchControl_team_y->setPosition(ax::Vec2(pduiyOpenLabel->getPosition().x+pduiyOpenLabel->getContentSize().width+20,nPageSize.height-114));
// 	_PageLayer_1->addChild(switchControl_team_y);
// 	switchControl_team_y->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_team_y->setOn(false);
// 	//valueChanged(switchControl_team_y, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);


// 	Label* pbiaoSetLabel = Label::createWithTTF("Shipping setting",UI_GAME_FONT_DEFAULT,14);//Arial
// 	pbiaoSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pbiaoSetLabel->setColor(ax::Color3B::YELLOW);
// 	pbiaoSetLabel->setPosition(ax::Vec2(10,nPageSize.height-140));
// 	_PageLayer_1->addChild(pbiaoSetLabel);

// 	Label* pybiaoOpenLabel = Label::createWithTTF("Auto Shipping",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pybiaoOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pybiaoOpenLabel->setPosition(ax::Vec2(pbiaoSetLabel->getContentSize().width+20,nPageSize.height-140));
// 	_PageLayer_1->addChild(pybiaoOpenLabel);
// 	switchControl_yabiao = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_team->setScale(CONTROL_SCALE);
// 	switchControl_yabiao->setTag(BOX_TAG_YABIAO);
// 	switchControl_yabiao->setPosition(ax::Vec2(pybiaoOpenLabel->getPosition().x+pybiaoOpenLabel->getContentSize().width+20,nPageSize.height-134));
// 	_PageLayer_1->addChild(switchControl_yabiao);
// 	switchControl_yabiao->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_yabiao->setOn(false);
// 	valueChanged(switchControl_yabiao, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	//UTEXT("其他设置(拾取装备,物品)",1).c_str()
// 	Label* pzbSetLabel = Label::createWithTTF("Other setting(equip,items)",UI_GAME_FONT_DEFAULT,14);//Arial
// 	pzbSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pzbSetLabel->setColor(ax::Color3B::YELLOW);
// 	pzbSetLabel->setPosition(ax::Vec2(10,nPageSize.height-160));
// 	_PageLayer_1->addChild(pzbSetLabel);

// 	Label* pshuOpenLabel = Label::createWithTTF(UTEXT("Equip's Attributes >=",1).c_str(),UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pshuOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pshuOpenLabel->setPosition(ax::Vec2(15,nPageSize.height-180));
// 	_PageLayer_1->addChild(pshuOpenLabel);

// 	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	//pBg->setColor(ax::Color3B::GREEN);
// 	pshuxEditBox = EditBox::create(Size(50,24),pBg);
// 	if (pshuxEditBox)
// 	{
// 		pshuxEditBox->setPlaceHolder("Count");
// 		pshuxEditBox->setFontColor(ax::Color3B::YELLOW);
// 		pshuxEditBox->setFontSize(24);
// 		pshuxEditBox->setMaxLength(1);
// 		pshuxEditBox->setPosition(ax::Vec2(pshuOpenLabel->getPosition().x+pshuOpenLabel->getContentSize().width+30,nPageSize.height-174));
// 		pshuxEditBox->setInputMode(EditBox::InputMode::DECIMAL);
// 		pshuxEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		_PageLayer_1->addChild(pshuxEditBox);
// 		//ShuXingNum=1
// 		//AllKeep=0 nisOpen
// 		nAutoNeiGua.GetString("CCCC","ShuXingNum","0",nTempStr,sizeof(nTempStr));
// 		//nTempInt = g_Atoui(nTempStr);
// 		pshuxEditBox->setText(nTempStr);
// 	}


// 	Label* pbaoliuOpenLabel = Label::createWithTTF("Count,Keep all equip",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pbaoliuOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pbaoliuOpenLabel->setPosition(ax::Vec2(200,nPageSize.height-180));
// 	_PageLayer_1->addChild(pbaoliuOpenLabel);
// 	switchControl_baoliu = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_team->setScale(CONTROL_SCALE);
// 	switchControl_baoliu->setTag(BOX_TAG_BAOLIU);
// 	switchControl_baoliu->setPosition(ax::Vec2(pbaoliuOpenLabel->getPosition().x+pbaoliuOpenLabel->getContentSize().width+20,nPageSize.height-174));
// 	_PageLayer_1->addChild(switchControl_baoliu);
// 	switchControl_baoliu->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("CCCC","AllKeep",0,&nisOpen);
// 	if (nisOpen>0)
// 		switchControl_baoliu->setOn(true);
// 	else
// 		switchControl_baoliu->setOn(false);
// 	valueChanged(switchControl_baoliu, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);

// 	Label* puseitemOpenLabel = Label::createWithTTF("Use Items",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	puseitemOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	puseitemOpenLabel->setPosition(ax::Vec2(15,nPageSize.height-200));
// 	_PageLayer_1->addChild(puseitemOpenLabel);
// 	switchControl_useitem = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_team->setScale(CONTROL_SCALE);
// 	switchControl_useitem->setTag(BOX_TAG_USEITEM);
// 	switchControl_useitem->setPosition(ax::Vec2(puseitemOpenLabel->getPosition().x+puseitemOpenLabel->getContentSize().width+20,nPageSize.height-194));
// 	_PageLayer_1->addChild(switchControl_useitem);
// 	switchControl_useitem->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("DDDD","IsOpenUseIetm",0,&nisOpen);
// 	if (nisOpen>0)
// 		switchControl_useitem->setOn(true);
// 	else
// 		switchControl_useitem->setOn(false);
// 	valueChanged(switchControl_useitem, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);

// 	Label* pmiaoshaOpenLabel = Label::createWithTTF("Filter Blue Npc",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pmiaoshaOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pmiaoshaOpenLabel->setPosition(ax::Vec2(110,nPageSize.height-200));
// 	_PageLayer_1->addChild(pmiaoshaOpenLabel);
// 	switchControl_miaosha = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_team->setScale(CONTROL_SCALE);
// 	switchControl_miaosha->setTag(BOX_TAG_MIAOSHA);
// 	switchControl_miaosha->setPosition(ax::Vec2(pmiaoshaOpenLabel->getPosition().x+pmiaoshaOpenLabel->getContentSize().width+20,nPageSize.height-194));
// 	_PageLayer_1->addChild(switchControl_miaosha);
// 	switchControl_miaosha->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("DDDD","IsOpenNoBlue",0,&nisOpen);
// 	if (nisOpen>0)
// 		switchControl_miaosha->setOn(true);
// 	else
// 		switchControl_miaosha->setOn(false);

// 	valueChanged(switchControl_miaosha, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// //--------攻击设置面板--------------------------
// 	_PageLayer_2 = Layer::create();//攻击
// 	_PageLayer_2->setContentSize(nPageSize);
// 	_PageLayer_2->setPosition(CCPointPage);
// 	//_PageLayer_2->setVisible(false); //设置隐藏
// 	ParentNode_Team->addChild(_PageLayer_2);


// 	Label* pZhandouSetLabel = Label::createWithTTF("Fighting setting",UI_GAME_FONT_DEFAULT, 14);//Arial
// 	pZhandouSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pZhandouSetLabel->setColor(ax::Color3B::YELLOW);
// 	pZhandouSetLabel->setPosition(ax::Vec2(10,nPageSize.height-18));
// 	_PageLayer_2->addChild(pZhandouSetLabel);

// 	Label* pZhandouOpenLabel = Label::createWithTTF("Auto Fight",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pZhandouOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pZhandouOpenLabel->setPosition(ax::Vec2(15,nPageSize.height-35));
// 	_PageLayer_2->addChild(pZhandouOpenLabel);
// 	//char nTempStr[32];
// 	nAutoNeiGua.GetString("BBBB","IsOpenZhanDou","0",nTempStr,sizeof(nTempStr));
// 	nTempInt = g_Atoui(nTempStr);
// 	switchControl_zhandou = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_zhandou->setScale(CONTROL_SCALE);
// 	switchControl_zhandou->setPosition(ax::Vec2(pZhandouOpenLabel->getContentSize().width+35,nPageSize.height-35+pZhandouOpenLabel->getContentSize().height/2));
// 	switchControl_zhandou->setTag(BOX_TAG_ART);
// 	_PageLayer_2->addChild(switchControl_zhandou);
// 	switchControl_zhandou->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	if  (nTempInt>0)
// 		switchControl_zhandou->setOn(true);
// 	else
// 		switchControl_zhandou->setOn(false);
// 	valueChanged(switchControl_zhandou, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_zhandou->setEnabled(false);
// 	/*checkBox_zhandou = CheckBox::create();
// 	checkBox_zhandou->setEnabled(true);
// 	Texture2D * check_box_normal_z          = ax::Director::getInstance()->getTextureCache()->addImage("ui/checkbox/001.png");
// 	Texture2D * check_box_active_disable_z  = ax::Director::getInstance()->getTextureCache()->addImage("ui/checkbox/002.png");
// 	Texture2D * check_box_active_z          = check_box_active_disable_z;
// 	checkBox_zhandou->loadTexturesBydata(check_box_normal_z,
// 		check_box_active_disable_z,
// 		check_box_active_disable_z,check_box_normal_z,check_box_active_z);
// 	checkBox_zhandou->setScale(1.5);
// 	checkBox_zhandou->setPosition(ax::Vec2(pZhandouOpenLabel->getContentSize().width+27,nPageSize.height-35+pZhandouOpenLabel->getContentSize().height/2));
// 	checkBox_zhandou->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::checkBoxStateEvent, this));
// 	checkBox_zhandou->setTag(BOX_TAG_ART);
// 	_PageLayer_2->addChild(checkBox_zhandou);
// 	if  (nTempInt>0)
// 		checkBox_zhandou->setSelected(true);
// 	*/

// 	Label* pxunguaiLabel = Label::createWithTTF("Find Npc Range",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pxunguaiLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pxunguaiLabel->setPosition(ax::Vec2(switchControl_zhandou->getPosition().x+30,nPageSize.height-35));
// 	_PageLayer_2->addChild(pxunguaiLabel);

// 	ax::ui::Scale9Sprite *gBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	//pBg->setColor(ax::Color3B::GREEN);
// 	pXunguaiEditBox = EditBox::create(Size(50,24),gBg);
// 	if (pXunguaiEditBox)
// 	{
// 		//pEditBox->setContentSize()
// 		//pMoneyEditBox->setText("your money");
// 		pXunguaiEditBox->setPlaceHolder("Range");
// 		pXunguaiEditBox->setFontColor(ax::Color3B::YELLOW);
// 		pXunguaiEditBox->setFontSize(24);
// 		pXunguaiEditBox->setMaxLength(6);
// 		pXunguaiEditBox->setPosition(ax::Vec2(pxunguaiLabel->getPosition().x+pxunguaiLabel->getContentSize().width+30,switchControl_zhandou->getPosition().y));
// 		pXunguaiEditBox->setInputMode(EditBox::InputMode::DECIMAL);
// 		pXunguaiEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		//pMoneyEditBox->setDelegate(this);
// 		//pMoneyEditBox->setDefaultTouchPriority(-128);
// 		_PageLayer_2->addChild(pXunguaiEditBox);

// 		nAutoNeiGua.GetString("BBBB","FindNpcDis","180",nTempStr,sizeof(nTempStr));
// 		//nTempInt = g_Atoui(nTempStr);
// 		pXunguaiEditBox->setText(nTempStr);
// 	}
// 	Label* phuodongLabel = Label::createWithTTF("Walk range",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	phuodongLabel->setAnchorPoint(ax::Vec2(0,0));
// 	phuodongLabel->setPosition(ax::Vec2(switchControl_zhandou->getPosition().x+180,nPageSize.height-35));
// 	_PageLayer_2->addChild(phuodongLabel);
// 	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	pGongjiEditBox = EditBox::create(Size(50,24),pBg);
// 	if (pGongjiEditBox)
// 	{
// 		pGongjiEditBox->setPlaceHolder("Walk range");
// 		pGongjiEditBox->setFontColor(ax::Color3B::YELLOW);
// 		pGongjiEditBox->setFontSize(24);
// 		pGongjiEditBox->setMaxLength(6);
// 		pGongjiEditBox->setPosition(ax::Vec2(phuodongLabel->getPosition().x+phuodongLabel->getContentSize().width+30,switchControl_zhandou->getPosition().y));
// 		pGongjiEditBox->setInputMode(EditBox::InputMode::DECIMAL);
// 		pGongjiEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		_PageLayer_2->addChild(pGongjiEditBox);
// 		nAutoNeiGua.GetString("BBBB","GongJiFanWei","180",nTempStr,sizeof(nTempStr));
// 		pGongjiEditBox->setText(nTempStr);
// 	}

// 	Label* pgmiaoLabel_h = Label::createWithTTF("Attack time",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pgmiaoLabel_h->setAnchorPoint(ax::Vec2(0,0));
// 	pgmiaoLabel_h->setPosition(ax::Vec2(switchControl_zhandou->getPosition().x+30,nPageSize.height-62));
// 	_PageLayer_2->addChild(pgmiaoLabel_h);

// 	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	//pBg->setColor(ax::Color3B::GREEN);
// 	pGongjiEditBox_m = EditBox::create(Size(50,24),pBg);
// 	if (pGongjiEditBox_m)
// 	{
// 		pGongjiEditBox_m->setPlaceHolder("sec");
// 		pGongjiEditBox_m->setFontColor(ax::Color3B::YELLOW);
// 		pGongjiEditBox_m->setFontSize(24);
// 		pGongjiEditBox_m->setMaxLength(2);
// 		pGongjiEditBox_m->setPosition(ax::Vec2(pgmiaoLabel_h->getPosition().x+pgmiaoLabel_h->getContentSize().width+30,nPageSize.height-55));
// 		pGongjiEditBox_m->setInputMode(EditBox::InputMode::DECIMAL);
// 		pGongjiEditBox_m->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		_PageLayer_2->addChild(pGongjiEditBox_m);
// 		nAutoNeiGua.GetString("BBBB","GongJiNun","10",nTempStr,sizeof(nTempStr));
// 		//nTempInt = g_Atoui(nTempStr);
// 		pGongjiEditBox_m->setText(nTempStr);
// 	}
// 	Label* ptempLabel = Label::createWithTTF("sec,Give up the attack",UI_GAME_FONT_DEFAULT,12);//Arial
// 	ptempLabel->setAnchorPoint(ax::Vec2(0,0));
// 	ptempLabel->setPosition(ax::Vec2(pGongjiEditBox_m->getPosition().x+pGongjiEditBox_m->getContentSize().width-20,nPageSize.height-62));
// 	_PageLayer_2->addChild(ptempLabel);
// 	//------录制脚本
// 	Label* pautoSetLabel = Label::createWithTTF("Auto Kind",UI_GAME_FONT_DEFAULT,14);//Arial
// 	pautoSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pautoSetLabel->setColor(ax::Color3B::YELLOW);
// 	pautoSetLabel->setPosition(ax::Vec2(10,nPageSize.height-80));
// 	_PageLayer_2->addChild(pautoSetLabel);

// 	Label* pdingLabel = Label::createWithTTF("Fixed",UI_GAME_FONT_DEFAULT,12);//Arial
// 	pdingLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pdingLabel->setPosition(ax::Vec2(80,nPageSize.height-80));
// 	_PageLayer_2->addChild(pdingLabel);
// 	switchControl_dingdian = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Yes",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("No",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_shiqu->setScale(CONTROL_SCALE);
// 	switchControl_dingdian->setPosition(ax::Vec2(pdingLabel->getPosition().x+pdingLabel->getContentSize().width+20,nPageSize.height-74));
// 	switchControl_dingdian->setTag(BOX_TAG_DING);
// 	_PageLayer_2->addChild(switchControl_dingdian);
// 	switchControl_dingdian->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("BBBB","IsXiTong",0,&nisOpen);
// 	if  (nisOpen>0)
// 		switchControl_dingdian->setOn(true);
// 	else
// 		switchControl_dingdian->setOn(false);
// 	valueChanged(switchControl_dingdian, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);

// 	switchControl_dingdian->setEnabled(false);

// 	Label* pluxianLabel = Label::createWithTTF("Route",UI_GAME_FONT_DEFAULT,12);//Arial
// 	pluxianLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pluxianLabel->setPosition(ax::Vec2(160,nPageSize.height-80));
// 	_PageLayer_2->addChild(pluxianLabel);
// 	switchControl_luxian = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Yes",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("No",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_shiqu->setScale(CONTROL_SCALE);
// 	switchControl_luxian->setPosition(ax::Vec2(pluxianLabel->getPosition().x+pluxianLabel->getContentSize().width+20,nPageSize.height-74));
// 	switchControl_luxian->setTag(BOX_TAG_LUXIAN);
// 	_PageLayer_2->addChild(switchControl_luxian);
// 	switchControl_luxian->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("BBBB","IsJiaoBen",0,&nisOpen);
// 	if  (nisOpen>0)
// 		switchControl_luxian->setOn(true);
// 	else
// 		switchControl_luxian->setOn(false);

// 	if  (switchControl_dingdian && switchControl_dingdian->isOn())
// 		switchControl_luxian->setOn(false);
// 	else
// 		switchControl_luxian->setOn(true);

// 	valueChanged(switchControl_luxian, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_luxian->setEnabled(false);


// 	switchControl_luzhi = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask-6.png"),
// 		Sprite::create("ui/switch/switch-on-6.png"),
// 		Sprite::create("ui/switch/switch-off-6.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Record Route:Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Record Route:Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//setPosition(ax::Vec2(160,nPageSize.height-80));
// 	//switchControl_luzhi->setScaleX(4);
// 	switchControl_luzhi->setPosition(ax::Vec2(300,nPageSize.height-74));
// 	switchControl_luzhi->setTag(BOX_TAG_LUZHI);
// 	_PageLayer_2->addChild(switchControl_luzhi);
// 	switchControl_luzhi->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	//valueChanged(switchControl_luzhi, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	if  (Player[CLIENT_PLAYER_INDEX].m_Autoplay.nIsJiaoBen)
// 		switchControl_luzhi->setOn(true);
// 	else
// 	    switchControl_luzhi->setOn(false);

// 	switchControl_luzhi->setEnabled(false);

// 	//------
// 	Label* pPickSetLabel = Label::createWithTTF("PickUp Setting",UI_GAME_FONT_DEFAULT,14);//Arial
// 	pPickSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pPickSetLabel->setColor(ax::Color3B::YELLOW);
// 	pPickSetLabel->setPosition(ax::Vec2(10,nPageSize.height-100));
// 	_PageLayer_2->addChild(pPickSetLabel);

// 	Label* pPickLabel = Label::createWithTTF("Auto PickUp",UI_GAME_FONT_DEFAULT,12);//Arial
// 	pPickLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pPickLabel->setPosition(ax::Vec2(110,nPageSize.height-100));
// 	_PageLayer_2->addChild(pPickLabel);

// 	nAutoNeiGua.GetString("BBBB","IsOpenShiQu","0",nTempStr,sizeof(nTempStr));
// 	nTempInt = g_Atoui(nTempStr);
// 	switchControl_shiqu = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_shiqu->setScale(CONTROL_SCALE);
// 	switchControl_shiqu->setPosition(ax::Vec2(pPickLabel->getPosition().x+pPickLabel->getContentSize().width+20,nPageSize.height-94));
// 	switchControl_shiqu->setTag(BOX_TAG_SHIQU);
// 	_PageLayer_2->addChild(switchControl_shiqu);
// 	switchControl_shiqu->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	if  (nTempInt>0)
// 		switchControl_shiqu->setOn(true);
// 	else
// 		switchControl_shiqu->setOn(false);
// 	valueChanged(switchControl_shiqu, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_shiqu->setEnabled(false);
// 	Label* pshiquOpenLabel = Label::createWithTTF(UTEXT("Save Money",1).c_str(),UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pshiquOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pshiquOpenLabel->setPosition(ax::Vec2(15,nPageSize.height-115));
// 	_PageLayer_2->addChild(pshiquOpenLabel);
// 	//char nTempStr[32];
// 	switchControl_savem = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_savem->setScale(CONTROL_SCALE);
// 	switchControl_savem->setPosition(ax::Vec2(pshiquOpenLabel->getContentSize().width+35,nPageSize.height-115+pshiquOpenLabel->getContentSize().height/2));
// 	//switchControl_savem->setPosition(ax::Vec2(popensaveLabel->getPosition().x+popensaveLabel->getContentSize().width+35,nPageSize.height-80+popensaveLabel->getContentSize().height/2));
// 	switchControl_savem->setTag(BOX_TAG_SAVE_M);
// 	_PageLayer_2->addChild(switchControl_savem);
// 	switchControl_savem->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger2("BBBB","IsOpenAutoSave",&nisOpen,&nsaveMonye);
// 	if  (nisOpen>0)
// 		switchControl_savem->setOn(true);
// 	else
// 		switchControl_savem->setOn(false);
// 	valueChanged(switchControl_savem, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_savem->setEnabled(false);
// 	/*checkBox_shiqu = CheckBox::create();
// 	checkBox_shiqu->setEnabled(true);
// 	check_box_normal_z          = ax::Director::getInstance()->getTextureCache()->addImage("ui/checkbox1/001.png");
// 	check_box_active_disable_z  = ax::Director::getInstance()->getTextureCache()->addImage("ui/checkbox1/002.png");
// 	check_box_active_z          = check_box_active_disable_z;
// 	checkBox_shiqu->loadTexturesBydata(check_box_normal_z,
// 		check_box_active_disable_z,
// 		check_box_active_disable_z,check_box_normal_z,check_box_active_z);
// 	checkBox_shiqu->setScale(1.5);
// 	checkBox_shiqu->setPosition(ax::Vec2(pshiquOpenLabel->getContentSize().width+27,nPageSize.height-115+pshiquOpenLabel->getContentSize().height/2));
// 	checkBox_shiqu->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::checkBoxStateEvent, this));
// 	checkBox_shiqu->setTag(BOX_TAG_SHIQU);
// 	_PageLayer_2->addChild(checkBox_shiqu);
// 	if  (nTempInt>0)
// 		checkBox_shiqu->setSelected(true);
// 	*/
// 	Label* psaveLabel = Label::createWithTTF(UTEXT("Money>=",1).c_str(),UI_GAME_FONT_DEFAULT, 12);//Arial
// 	psaveLabel->setAnchorPoint(ax::Vec2(0,0));
// 	psaveLabel->setPosition(ax::Vec2(switchControl_savem->getPosition().x+switchControl_savem->getContentSize().width+10,nPageSize.height-115));
// 	_PageLayer_2->addChild(psaveLabel);

// 	gBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	//pBg->setColor(ax::Color3B::GREEN);
// 	pmoneyEditBox = EditBox::create(Size(50,24),gBg);

// 	if (pmoneyEditBox)
// 	{
// 		pmoneyEditBox->setPlaceHolder("Money");
// 		pmoneyEditBox->setFontColor(ax::Color3B::YELLOW);
// 		pmoneyEditBox->setFontSize(24);
// 		pmoneyEditBox->setMaxLength(6);
// 		pmoneyEditBox->setPosition(ax::Vec2(psaveLabel->getPosition().x+psaveLabel->getContentSize().width+30,switchControl_savem->getPosition().y));
// 		pmoneyEditBox->setInputMode(EditBox::InputMode::DECIMAL);
// 		pmoneyEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		_PageLayer_2->addChild(pmoneyEditBox);
// 		sprintf(nTempStr,"%d",nsaveMonye);
// 		//nTempInt = g_Atoui(nTempStr);
// 		pmoneyEditBox->setText(nTempStr);
// 	}
// 	Label* popensaveLabel = Label::createWithTTF(UTEXT("W,AutoSave Money",1).c_str(),UI_GAME_FONT_DEFAULT,12);//Arial
// 	popensaveLabel->setAnchorPoint(ax::Vec2(0,0));
// 	popensaveLabel->setPosition(ax::Vec2(pmoneyEditBox->getPosition().x+30,nPageSize.height-115));
// 	_PageLayer_2->addChild(popensaveLabel);
// 	/*checkBox_savem = CheckBox::create();
// 	checkBox_savem->setEnabled(true);
// 	check_box_normal_z          = ax::Director::getInstance()->getTextureCache()->addImage("ui/checkbox1/001.png");
// 	check_box_active_disable_z  = ax::Director::getInstance()->getTextureCache()->addImage("ui/checkbox1/002.png");
// 	check_box_active_z          = check_box_active_disable_z;
// 	checkBox_savem->loadTexturesBydata(check_box_normal_z,
// 		check_box_active_disable_z,
// 		check_box_active_disable_z,check_box_normal_z,check_box_active_z);
// 	checkBox_savem->setScale(1.5);
// 	//checkBox_savem->setPosition(ax::Vec2(pshiquOpenLabel->getContentSize().width+47,nPageSize.height-115+pshiquOpenLabel->getContentSize().height/2));
// 	checkBox_savem->setPosition(ax::Vec2(popensaveLabel->getPosition().x+popensaveLabel->getContentSize().width+15,nPageSize.height-115+popensaveLabel->getContentSize().height/2));
// 	checkBox_savem->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::checkBoxStateEvent, this));
// 	checkBox_savem->setTag(BOX_TAG_SAVE_M);
// 	_PageLayer_2->addChild(checkBox_savem);
// 	if  (nTempInt>0)
// 		checkBox_savem->setSelected(true);
// 	*/
// 	Label* pfenjieOpenLabel = Label::createWithTTF("Auto Broken",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pfenjieOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pfenjieOpenLabel->setPosition(ax::Vec2(15,nPageSize.height-135));
// 	_PageLayer_2->addChild(pfenjieOpenLabel);
// 	//char nTempStr[32];
// 	switchControl_fenjie = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_savem->setScale(CONTROL_SCALE);
// 	switchControl_fenjie->setPosition(ax::Vec2(pfenjieOpenLabel->getContentSize().width+35,nPageSize.height-135+pfenjieOpenLabel->getContentSize().height/2));
// 	//switchControl_savem->setPosition(ax::Vec2(popensaveLabel->getPosition().x+popensaveLabel->getContentSize().width+35,nPageSize.height-80+popensaveLabel->getContentSize().height/2));
// 	switchControl_fenjie->setTag(BOX_TAG_FENJIE);
// 	_PageLayer_2->addChild(switchControl_fenjie);
// 	switchControl_fenjie->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("BBBB","IsOpenFenJie",0,&nisOpen);
// 	if  (nisOpen>0)
// 		switchControl_fenjie->setOn(true);
// 	else
// 		switchControl_fenjie->setOn(false);
// 	valueChanged(switchControl_fenjie,ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_fenjie->setEnabled(false);

// 	Label* pshiquLabel_c = Label::createWithTTF(UTEXT("PickUp>=",1).c_str(),UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pshiquLabel_c->setAnchorPoint(ax::Vec2(0,0));
// 	pshiquLabel_c->setPosition(ax::Vec2(switchControl_fenjie->getPosition().x+switchControl_fenjie->getContentSize().width+10,nPageSize.height-140));
// 	_PageLayer_2->addChild(pshiquLabel_c);

// 	gBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	//pBg->setColor(ax::Color3B::GREEN);
// 	pshiquEditBox_c = EditBox::create(Size(50,24),gBg);
// 	if (pshiquEditBox_c)
// 	{
// 		pshiquEditBox_c->setPlaceHolder("Count");
// 		pshiquEditBox_c->setFontColor(ax::Color3B::YELLOW);
// 		pshiquEditBox_c->setFontSize(24);
// 		pshiquEditBox_c->setMaxLength(6);
// 		pshiquEditBox_c->setPosition(ax::Vec2(pshiquLabel_c->getPosition().x+pshiquLabel_c->getContentSize().width+30,pshiquLabel_c->getPosition().y+6));
// 		pshiquEditBox_c->setInputMode(EditBox::InputMode::DECIMAL);
// 		pshiquEditBox_c->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		_PageLayer_2->addChild(pshiquEditBox_c);
// 		nAutoNeiGua.GetString("BBBB","ShiQuNum","10",nTempStr,sizeof(nTempStr));
// 		//sprintf(nTempStr,"%d",nsaveMonye);
// 		//nTempInt = g_Atoui(nTempStr);
// 		pshiquEditBox_c->setText(nTempStr);
// 	}
// 	Label* plostLabel = Label::createWithTTF("Count,Give up PickUp",UI_GAME_FONT_DEFAULT,12);//Arial
// 	plostLabel->setAnchorPoint(ax::Vec2(0,0));
// 	plostLabel->setPosition(ax::Vec2(pshiquEditBox_c->getPosition().x+30,nPageSize.height-140));
// 	_PageLayer_2->addChild(plostLabel);

// 	Label* pyanshiOpenLabel = Label::createWithTTF("Attack or PickUp timeout",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pyanshiOpenLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pyanshiOpenLabel->setPosition(ax::Vec2(15,nPageSize.height-155));
// 	_PageLayer_2->addChild(pyanshiOpenLabel);
// 	//char nTempStr[32];
// 	switchControl_yanshi = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	//switchControl_savem->setScale(CONTROL_SCALE);
// 	switchControl_yanshi->setPosition(ax::Vec2(pyanshiOpenLabel->getContentSize().width+35,nPageSize.height-155+pyanshiOpenLabel->getContentSize().height/2));
// 	//switchControl_savem->setPosition(ax::Vec2(popensaveLabel->getPosition().x+popensaveLabel->getContentSize().width+35,nPageSize.height-80+popensaveLabel->getContentSize().height/2));
// 	switchControl_yanshi->setTag(BOX_TAG_YANSHI);
// 	_PageLayer_2->addChild(switchControl_yanshi);
// 	switchControl_yanshi->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("BBBB","IsOpenTime",0,&nisOpen);
// 	//if  (nisOpen>0)
// 		switchControl_yanshi->setOn(true);
// 	//else
// 		//switchControl_yanshi->setOn(false);
// 	valueChanged(switchControl_yanshi,ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_yanshi->setEnabled(false);

// 		gBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	//pBg->setColor(ax::Color3B::GREEN);
// 	pEditBox_yanshi = EditBox::create(Size(25,24),gBg);
// 	if (pEditBox_yanshi)
// 	{
// 		pEditBox_yanshi->setPlaceHolder("sec");
// 		pEditBox_yanshi->setFontColor(ax::Color3B::YELLOW);
// 		pEditBox_yanshi->setFontSize(24);
// 		pEditBox_yanshi->setMaxLength(2);
// 		pEditBox_yanshi->setPosition(ax::Vec2(switchControl_yanshi->getPosition().x+switchControl_yanshi->getContentSize().width,pyanshiOpenLabel->getPosition().y+6));
// 		pEditBox_yanshi->setInputMode(EditBox::InputMode::DECIMAL);
// 		pEditBox_yanshi->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		_PageLayer_2->addChild(pEditBox_yanshi);
// 		nAutoNeiGua.GetString("BBBB","QuXiaoTime","8",nTempStr,sizeof(nTempStr));
// 		//sprintf(nTempStr,"%d",nsaveMonye);
// 		//nTempInt = g_Atoui(nTempStr);
// 		pEditBox_yanshi->setText(nTempStr);
// 	}
// 	Label* pfangqiLabel = Label::createWithTTF(UTEXT("sec,Give Up to attack",1).c_str(),UI_GAME_FONT_DEFAULT,12);//Arial
// 	pfangqiLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pfangqiLabel->setPosition(ax::Vec2(pEditBox_yanshi->getPosition().x+30,nPageSize.height-155));
// 	_PageLayer_2->addChild(pfangqiLabel);

//     //--------------------
// 	Label* pskillSetLabel = Label::createWithTTF("Skills Seting",UI_GAME_FONT_DEFAULT,14);//Arial
// 	pskillSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pskillSetLabel->setColor(ax::Color3B::YELLOW);
// 	pskillSetLabel->setPosition(ax::Vec2(10,nPageSize.height-175));
// 	_PageLayer_2->addChild(pskillSetLabel);
// 	/*ax::extension::ControlSwitch *switchControl = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF(UTEXT("开",1).c_str(),UI_GAME_FONT_DEFAULT,20),
// 		Label::createWithTTF(UTEXT("关",1).c_str(),UI_GAME_FONT_DEFAULT,20)
// 		);
// 	//switchControl->setScale(CONTROL_SCALE);
// 	switchControl->setPosition(ax::Vec2(100,nPageSize.height-175));
// 	_PageLayer_2->addChild(switchControl);
// 	switchControl->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	valueChanged(switchControl, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	*/
// 	nFactionNo =  Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(130);

// 	float offsetX=50,offsetY=190;
// 	if (nFactionNo>0)
// 	{
// 		for (int i=0;i<MAX_SKILL_COUNT;++i)
// 		{
// 			if (i==2)
// 			{
//               offsetX=50;
// 			  offsetY=205;
// 			}
// 			switchControl_skill[i] = ax::extension::ControlSwitch::create
// 				(
// 				Sprite::create("ui/switch/switch-mask.png"),
// 				Sprite::create("ui/switch/switch-on.png"),
// 				Sprite::create("ui/switch/switch-off.png"),
// 				Sprite::create("ui/switch/switch-thumb.png"),
// 				Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 				Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 				);
// 			//switchControl->setScale(CONTROL_SCALE);
// 			switchControl_skill[i]->setPosition(ax::Vec2(offsetX-17,nPageSize.height-offsetY+5));
// 			_PageLayer_2->addChild(switchControl_skill[i]);
// 			switchControl_skill[i]->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 			char ntempKey[32];
// 			sprintf(ntempKey,"IsOpenSkill_%d",i);
// 			nAutoNeiGua.GetInteger("BBBB",ntempKey,0,&nisOpen);
// 			if  (nisOpen>0)
// 				switchControl_skill[i]->setOn(true);
// 			else
// 				switchControl_skill[i]->setOn(false);
// 			valueChanged(switchControl_skill[i], ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 			switchControl_skill[i]->setEnabled(false);
// 			/*IsOpenSkill_0=0
// 				IsOpenSkill_1=1
// 				IsOpenSkill_2=1
// 				IsOpenSkill_3=0
// 				*/
// 			skillnameLabel[i] = Label::createWithTTF("NUll",UI_GAME_FONT_DEFAULT,12);//Arial
// 			skillnameLabel[i]->setAnchorPoint(ax::Vec2(0,0));
// 			skillnameLabel[i]->setColor(ax::Color3B::YELLOW);
// 			skillnameLabel[i]->setPosition(ax::Vec2(offsetX,nPageSize.height-offsetY));
// 			_PageLayer_2->addChild(skillnameLabel[i]);
// 			offsetX +=100;
// 			char nSkillKey[32],nNameKey[64],nSkillName[64];
// 			int  nSkillId=0;

// 			sprintf(nSkillKey,"skill_%d",i);
// 			sprintf(nNameKey,"%s_name",nSkillKey);
// 			nAutoSkill.GetInteger(nFactionNo+1,nSkillKey,0,&nSkillId);

// 			if (nSkillId>0)
// 			{
// 				nAutoSkill.GetString(nFactionNo+1,nNameKey,"",nSkillName,sizeof(nSkillName));
// 				//设置技能名称
// 				skillnameLabel[i]->setString(UTEXT(nSkillName,1).c_str());
// 			}
// 		}
// 	}
// 	_PageLayer_2->setVisible(false);
// //--------物品１设置面板------------------------
// 	_PageLayer_3 = Layer::create();//物品１
// 	_PageLayer_3->setContentSize(nPageSize);
// 	_PageLayer_3->setPosition(CCPointPage);
// 	_PageLayer_3->setVisible(false);
// 	ParentNode_Team->addChild(_PageLayer_3);

// 	Label* pfenleiSetLabel = Label::createWithTTF("Item type setting",UI_GAME_FONT_DEFAULT, 14);//Arial
// 	pfenleiSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pfenleiSetLabel->setColor(ax::Color3B::YELLOW);
// 	pfenleiSetLabel->setPosition(ax::Vec2(10,nPageSize.height-18));
// 	_PageLayer_3->addChild(pfenleiSetLabel);
// 	//--------------
// 	Label* pfenleiLabel1 = Label::createWithTTF("Money",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pfenleiLabel1->setAnchorPoint(ax::Vec2(0,0));
// 	pfenleiLabel1->setPosition(ax::Vec2(15,nPageSize.height-35));
// 	_PageLayer_3->addChild(pfenleiLabel1);

// 	switchControl_item_m = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	switchControl_item_m->setPosition(ax::Vec2(pfenleiLabel1->getPosition().x+pfenleiLabel1->getContentSize().width*2,pfenleiLabel1->getPosition().y+6));
// 	_PageLayer_3->addChild(switchControl_item_m);
// 	switchControl_item_m->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("CCCC","IsQian",0,&nisOpen);
// 	if  (nisOpen>0)
// 		switchControl_item_m->setOn(true);
// 	else
// 		switchControl_item_m->setOn(false);
// 	valueChanged(switchControl_item_m, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_item_m->setEnabled(false);
// 	//--------------
// 	Label* pfenleiLabel2 = Label::createWithTTF("ToolItem",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pfenleiLabel2->setAnchorPoint(ax::Vec2(0,0));
// 	pfenleiLabel2->setPosition(ax::Vec2(100,nPageSize.height-35));
// 	_PageLayer_3->addChild(pfenleiLabel2);

// 	switchControl_item_d = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	switchControl_item_d->setPosition(ax::Vec2(pfenleiLabel2->getPosition().x+pfenleiLabel2->getContentSize().width+5,pfenleiLabel2->getPosition().y+6));
// 	_PageLayer_3->addChild(switchControl_item_d);
// 	switchControl_item_d->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);

// 	nAutoNeiGua.GetInteger("CCCC","IsDaoJu",0,&nisOpen);
// 	if  (nisOpen>0)
// 		switchControl_item_d->setOn(true);
// 	else
// 		switchControl_item_d->setOn(false);
// 	valueChanged(switchControl_item_d, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_item_d->setEnabled(false);
// 	//--------------
// 	Label* pfenleiLabel3 = Label::createWithTTF("Equip",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pfenleiLabel3->setAnchorPoint(ax::Vec2(0,0));
// 	pfenleiLabel3->setPosition(ax::Vec2(200,nPageSize.height-35));
// 	_PageLayer_3->addChild(pfenleiLabel3);

// 	switchControl_item_z = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	switchControl_item_z->setPosition(ax::Vec2(pfenleiLabel3->getPosition().x+pfenleiLabel3->getContentSize().width*2,pfenleiLabel3->getPosition().y+6));
// 	_PageLayer_3->addChild(switchControl_item_z);
// 	switchControl_item_z->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("CCCC","IsZhuang",0,&nisOpen);
// 	if  (nisOpen>0)
// 		switchControl_item_z->setOn(true);
// 	else
// 		switchControl_item_z->setOn(false);
// 	valueChanged(switchControl_item_z, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_item_z->setEnabled(false);
// 	//--------------
// 	Label* pfenleiLabel5 = Label::createWithTTF("PIC",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pfenleiLabel5->setAnchorPoint(ax::Vec2(0,0));
// 	pfenleiLabel5->setPosition(ax::Vec2(15,nPageSize.height-55));
// 	_PageLayer_3->addChild(pfenleiLabel5);

// 	switchControl_item_t = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	switchControl_item_t->setPosition(ax::Vec2(pfenleiLabel5->getPosition().x+pfenleiLabel5->getContentSize().width*2,pfenleiLabel5->getPosition().y+6));
// 	_PageLayer_3->addChild(switchControl_item_t);
// 	switchControl_item_t->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("CCCC","IsTuZhi",0,&nisOpen);
// 	if  (nisOpen>0)
// 		switchControl_item_t->setOn(true);
// 	else
// 		switchControl_item_t->setOn(false);
// 	valueChanged(switchControl_item_t, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_item_t->setEnabled(false);
// 	//--------------
// 	Label* pfenleiLabel4 = Label::createWithTTF("Money and Equip",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pfenleiLabel4->setAnchorPoint(ax::Vec2(0,0));
// 	pfenleiLabel4->setPosition(ax::Vec2(100,nPageSize.height-55));
// 	_PageLayer_3->addChild(pfenleiLabel4);

// 	switchControl_item_zm = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	switchControl_item_zm->setPosition(ax::Vec2(pfenleiLabel4->getPosition().x+pfenleiLabel4->getContentSize().width*1.5,pfenleiLabel4->getPosition().y+6));
// 	_PageLayer_3->addChild(switchControl_item_zm);
// 	switchControl_item_zm->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("CCCC","IsQianZhuang",0,&nisOpen);
// 	if  (nisOpen>0)
// 		switchControl_item_zm->setOn(true);
// 	else
// 		switchControl_item_zm->setOn(false);
// 	valueChanged(switchControl_item_zm, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_item_zm->setEnabled(false);
// 	Label* pzhuangSetLabel = Label::createWithTTF("Equip setting",UI_GAME_FONT_DEFAULT, 14);//Arial
// 	pzhuangSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pzhuangSetLabel->setColor(ax::Color3B::YELLOW);
// 	pzhuangSetLabel->setPosition(ax::Vec2(10,nPageSize.height-75));
// 	_PageLayer_3->addChild(pzhuangSetLabel);

// 	Label* ptoolSetLabel = Label::createWithTTF("Tool setting",UI_GAME_FONT_DEFAULT, 14);//Arial
// 	ptoolSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	ptoolSetLabel->setColor(ax::Color3B::YELLOW);
// 	ptoolSetLabel->setPosition(ax::Vec2(150,nPageSize.height-75));
// 	_PageLayer_3->addChild(ptoolSetLabel);

// 	Label* puseSetLabel = Label::createWithTTF("Use Item setting",UI_GAME_FONT_DEFAULT, 14);//Arial
// 	puseSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	puseSetLabel->setColor(ax::Color3B::YELLOW);
// 	puseSetLabel->setPosition(ax::Vec2(290,nPageSize.height-75));
// 	_PageLayer_3->addChild(puseSetLabel);

// 	Label* pguanjianLabel_z = Label::createWithTTF("Key",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pguanjianLabel_z->setAnchorPoint(ax::Vec2(0,0));
// 	pguanjianLabel_z->setPosition(ax::Vec2(10,nPageSize.height-180));
// 	_PageLayer_3->addChild(pguanjianLabel_z);

// 	gBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	pstrEditBox_s = EditBox::create(Size(70,24),gBg);
// 	if (pstrEditBox_s)
// 	{
// 		//pEditBox->setContentSize()
// 		//pMoneyEditBox->setText("your money");
// 		pstrEditBox_s->setPlaceHolder("The Keyword");
// 		pstrEditBox_s->setFontColor(ax::Color3B::YELLOW);
// 		pstrEditBox_s->setFontSize(24);
// 		pstrEditBox_s->setMaxLength(12);
// 		pstrEditBox_s->setPosition(ax::Vec2(pguanjianLabel_z->getPosition().x+pguanjianLabel_z->getContentSize().width+40,nPageSize.height-174));
// 		pstrEditBox_s->setInputMode(ax::ui::EditBox::InputMode::ANY);
// 		pstrEditBox_s->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		//pMoneyEditBox->setDelegate(this);
// 		//pMoneyEditBox->setDefaultTouchPriority(-128);
// 		_PageLayer_3->addChild(pstrEditBox_s);

// 		//nAutoNeiGua.GetString("BBBB","FindNpcDis","0",nTempStr,sizeof(nTempStr));
// 		//nTempInt = g_Atoui(nTempStr);
// 		//pXunguaiEditBox->setText(nTempStr);
// 	}

// 	_button = Button::create();
// 	_button->setEnabled(false);
// 	_button->loadTextures("ui/textbg.png", "ui/textbg.png", "");
// 	_button->setScale9Enabled(true);
// 	_button->setContentSize(btnSize);
// 	_button->setTitleFontSize(14);
// 	_button->setTitleText("Save Change");
// 	_button->setPosition(ax::Vec2(10+btnSize.width/2,nPageSize.height-195));
// 	_button->setTag(3);
// 	_button->setTag("3");
// 	_button->addTouchEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::oktouchEvent));
// 	_PageLayer_3->addChild(_button);

// 	Label* pguanjianLabel_d = Label::createWithTTF("Key",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pguanjianLabel_d->setAnchorPoint(ax::Vec2(0,0));
// 	pguanjianLabel_d->setPosition(ax::Vec2(150,nPageSize.height-180));
// 	_PageLayer_3->addChild(pguanjianLabel_d);

// 	gBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	pstrEditBox_d = EditBox::create(Size(70,24),gBg);
// 	if (pstrEditBox_d)
// 	{
// 		//pEditBox->setContentSize()
// 		//pMoneyEditBox->setText("your money");
// 		pstrEditBox_d->setPlaceHolder("The Keyword");
// 		pstrEditBox_d->setFontColor(ax::Color3B::YELLOW);
// 		pstrEditBox_d->setFontSize(24);
// 		pstrEditBox_d->setMaxLength(12);
// 		pstrEditBox_d->setPosition(ax::Vec2(pguanjianLabel_d->getPosition().x+pguanjianLabel_d->getContentSize().width+40,nPageSize.height-174));
// 		pstrEditBox_d->setInputMode(ax::ui::EditBox::InputMode::ANY);
// 		pstrEditBox_d->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		//pMoneyEditBox->setDelegate(this);
// 		//pMoneyEditBox->setDefaultTouchPriority(-128);
// 		_PageLayer_3->addChild(pstrEditBox_d);

// 		//nAutoNeiGua.GetString("BBBB","FindNpcDis","0",nTempStr,sizeof(nTempStr));
// 		//nTempInt = g_Atoui(nTempStr);
// 		//pXunguaiEditBox->setText(nTempStr);
// 	}

// 	_button_1 = Button::create();
// 	_button_1->setEnabled(false);
// 	_button_1->loadTextures("ui/textbg.png", "ui/textbg.png", "");
// 	_button_1->setScale9Enabled(true);
// 	_button_1->setContentSize(btnSize);
// 	_button_1->setTitleFontSize(14);
// 	_button_1->setTitleText("Save Change");
// 	_button_1->setPosition(ax::Vec2(150+btnSize.width/2,nPageSize.height-195));
// 	_button_1->setTag(4);
// 	_button_1->setTag("4");
// 	_button_1->addTouchEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::oktouchEvent));
// 	_PageLayer_3->addChild(_button_1);

// 	Label* pguanjianLabel_u = Label::createWithTTF("Key",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pguanjianLabel_u->setAnchorPoint(ax::Vec2(0,0));
// 	pguanjianLabel_u->setPosition(ax::Vec2(290,nPageSize.height-180));
// 	_PageLayer_3->addChild(pguanjianLabel_u);

// 	gBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
// 	pstrEditBox_u = EditBox::create(Size(70,24),gBg);
// 	if (pstrEditBox_u)
// 	{
// 		//pEditBox->setContentSize()
// 		//pMoneyEditBox->setText("your money");
// 		pstrEditBox_u->setPlaceHolder("The Keyword");
// 		pstrEditBox_u->setFontColor(ax::Color3B::YELLOW);
// 		pstrEditBox_u->setFontSize(24);
// 		pstrEditBox_u->setMaxLength(12);
// 		pstrEditBox_u->setPosition(ax::Vec2(pguanjianLabel_u->getPosition().x+pguanjianLabel_u->getContentSize().width+40,nPageSize.height-174));
// 		pstrEditBox_u->setInputMode(ax::ui::EditBox::InputMode::ANY);
// 		pstrEditBox_u->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
// 		//pMoneyEditBox->setDelegate(this);
// 		//pMoneyEditBox->setDefaultTouchPriority(-128);
// 		_PageLayer_3->addChild(pstrEditBox_u);

// 		//nAutoNeiGua.GetString("BBBB","FindNpcDis","0",nTempStr,sizeof(nTempStr));
// 		//nTempInt = g_Atoui(nTempStr);
// 		//pXunguaiEditBox->setText(nTempStr);
// 	}

// 	_button_2 = Button::create();
// 	_button_2->setEnabled(false);
// 	_button_2->loadTextures("ui/textbg.png", "ui/textbg.png", "");
// 	_button_2->setScale9Enabled(true);
// 	_button_2->setContentSize(btnSize);
// 	_button_2->setTitleFontSize(14);
// 	_button_2->setTitleText("Save Change");
// 	_button_2->setPosition(ax::Vec2(290+btnSize.width/2,nPageSize.height-195));
// 	_button_2->setTag(5);
// 	_button_2->setTag("5");
// 	_button_2->addTouchEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::oktouchEvent));
// 	_PageLayer_3->addChild(_button_2);

// //--------物品2设置面板-------------------------
// 	_PageLayer_4 = Layer::create();//物品２
// 	_PageLayer_4->setContentSize(nPageSize);
// 	_PageLayer_4->setPosition(CCPointPage);
// 	_PageLayer_4->setVisible(false);
// 	ParentNode_Team->addChild(_PageLayer_4);
// 	Label* pitemSetLabel = Label::createWithTTF("PickUp setting",UI_GAME_FONT_DEFAULT, 14);//Arial
// 	pitemSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pitemSetLabel->setColor(ax::Color3B::YELLOW);
// 	pitemSetLabel->setPosition(ax::Vec2(10,nPageSize.height-18));
// 	_PageLayer_4->addChild(pitemSetLabel);
// //--------其他设置面板-------------------------
// 	_PageLayer_5 = Layer::create();//其他
// 	_PageLayer_5->setContentSize(nPageSize);
// 	_PageLayer_5->setPosition(CCPointPage);
// 	_PageLayer_5->setVisible(false);
// 	ParentNode_Team->addChild(_PageLayer_5);
// 	Label* potherSetLabel = Label::createWithTTF("Other setting",UI_GAME_FONT_DEFAULT, 14);//Arial
// 	potherSetLabel->setAnchorPoint(ax::Vec2(0,0));
// 	potherSetLabel->setColor(ax::Color3B::YELLOW);
// 	potherSetLabel->setPosition(ax::Vec2(10,nPageSize.height-18));
// 	_PageLayer_5->addChild(potherSetLabel);

// 	Label* psoundLabel1 = Label::createWithTTF("Game Sound",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	psoundLabel1->setAnchorPoint(ax::Vec2(0,0));
// 	psoundLabel1->setPosition(ax::Vec2(15,nPageSize.height-35));
// 	_PageLayer_5->addChild(psoundLabel1);

// 	switchControl_other_s = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	switchControl_other_s->setTag(BOX_TAG_SOUND);
// 	switchControl_other_s->setPosition(ax::Vec2(psoundLabel1->getPosition().x+psoundLabel1->getContentSize().width*2,psoundLabel1->getPosition().y+6));
// 	_PageLayer_5->addChild(switchControl_other_s);
// 	switchControl_other_s->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("EEEE","IsOenSound",0,&nisOpen);
// 	if  (nisOpen>0)
// 		switchControl_other_s->setOn(true);
// 	else
// 		switchControl_other_s->setOn(false);
// 	valueChanged(switchControl_other_s, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_other_s->setEnabled(false);

// 	//开关自动调整距离
// 	Label* pjuliLabel1 = Label::createWithTTF("Auto adjustment Attack Range",UI_GAME_FONT_DEFAULT, 12);//Arial
// 	pjuliLabel1->setAnchorPoint(ax::Vec2(0,0));
// 	pjuliLabel1->setPosition(ax::Vec2(15,nPageSize.height-60));
// 	_PageLayer_5->addChild(pjuliLabel1);

// 	switchControl_other_j = ax::extension::ControlSwitch::create
// 		(
// 		Sprite::create("ui/switch/switch-mask.png"),
// 		Sprite::create("ui/switch/switch-on.png"),
// 		Sprite::create("ui/switch/switch-off.png"),
// 		Sprite::create("ui/switch/switch-thumb.png"),
// 		Label::createWithTTF("Open",UI_GAME_FONT_DEFAULT,12),
// 		Label::createWithTTF("Close",UI_GAME_FONT_DEFAULT,12)
// 		);
// 	switchControl_other_j->setTag(BOX_TAG_JULI);
// 	switchControl_other_j->setPosition(ax::Vec2(pjuliLabel1->getPosition().x+pjuliLabel1->getContentSize().width+20,pjuliLabel1->getPosition().y+6));
// 	_PageLayer_5->addChild(switchControl_other_j);
// 	switchControl_other_j->addTargetWithActionForControlEvents(this, cccontrol_selector(KuiAutoPlay_vn::valueChanged), ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	nAutoNeiGua.GetInteger("EEEE","IsOenjuli",0,&nisOpen);
// 	if  (nisOpen>0)
// 		switchControl_other_j->setOn(true);
// 	else
// 		switchControl_other_j->setOn(false);
// 	valueChanged(switchControl_other_j, ax::extension::ControlSwitch::EventType::VALUE_CHANGED);
// 	switchControl_other_j->setEnabled(false);
// 	/*m_pWidget = Layout::create();
// 	m_pWidget->setBackGroundImage("loadingEnd.png");
// 	m_pWidget->setBackGroundImageScale9Enabled(true);
// 	m_pWidget->setBackGroundColorOpacity(105);
// 	m_pWidget->setContentSize(m_size);
// 	m_pUiLayer->addChild(m_pWidget);
// 	*/
// 	// create list view data
// 	/*m_array = CCArray::create();
// 	CC_SAFE_RETAIN(m_array);
// 	for (int i = 0; i < 32; ++i)
// 	{
// 		CCString* ccstr = CCString::createWithFormat("属性关键字_%d", i);
// 		m_array->addObject(ccstr);
// 	}*/
// 	//Size widgetSize = m_pWidget->getSize();
// 	// Create the list view
// 	char nsetionKey[32],nDestStr[64];
// 	float list_w = 110;
// 	float list_h = 80;
// 	listView_Item = ListView::create();
// 	// set list view direction
// 	listView_Item->setDirection(ListView::Direction::VERTICAL);
// 	listView_Item->setEnabled(false);
// 	listView_Item->setBounceEnabled(true);
// 	listView_Item->setBackGroundImage("loadingStart.png");
// 	listView_Item->setBackGroundImageScale9Enabled(true);
// 	//listView_Item->setBackGroundColorOpacity(0);
// 	listView_Item->setBackGroundImageOpacity(0);
// 	listView_Item->setContentSize(ax::Size(list_w, list_h));
// 	//listView_Item->setInnerContainerSize(ax::Size(110, 110));
// 	//listView_Item->setPosition(ax::Vec2(m_size.width/2+40,m_size.height/2-50));
// 	listView_Item->setPosition(ax::Vec2(10,nPageSize.height-80-list_h));
// 	listView_Item->setTag(LIST_VIEW_ITEM);
// 	listView_Item->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::selectedItemEvent));
// 	_PageLayer_3->addChild(listView_Item);

// //	int count = m_array->count();
// 	// add custom item
// 	for (int i = 0; i < MAX_ITEMLIST_COUNT; ++i)
// 	{
// 		custom_button_item[i] = Button::create();
// 		custom_button_item[i]->setName("TextButton");
// 		custom_button_item[i]->setEnabled(false);
// 		custom_button_item[i]->loadTextures("ui/textbg.png", "ui/state/life_pk.png", "");
// 		custom_button_item[i]->setScale9Enabled(true);
// 		custom_button_item[i]->setContentSize(btnSize);
// 		custom_button_item[i]->setTitleText("The Keyword");
// 		sprintf(nsetionKey,"item_%d",i);
// 		char nTempStr[32],nTempStr_1[128];
// 		ZeroMemory(nTempStr,sizeof(nTempStr));
// 		ZeroMemory(nTempStr_1,sizeof(nTempStr_1));
// 		nAutoZhuang.GetString(i+1,1,"The Keyword",nDestStr,sizeof(nDestStr));
// 		nAutoZhuang.GetInteger(i+1,2,0,&nTempInt);
// 		sprintf(nTempStr_1,"%s,%d",nDestStr,nTempInt);
// 		if (nTempStr_1[0])
// 			custom_button_item[i]->setTitleText(UTEXT(nTempStr_1,1));

// 		Layout *custom_item = Layout::create();
// 		custom_item->setContentSize(custom_button_item[i]->getSize());
// 		custom_button_item[i]->setPosition(ax::Vec2(custom_item->getSize().width/2, custom_item->getSize().height/2));
// 		custom_item->addChild(custom_button_item[i]);

// 		listView_Item->pushBackCustomItem(custom_item);
// 	}
// 	listView_Item->forceDoLayout();
// 	listView_Item->jumpToBottom();


// 	listView_tool = ListView::create();
// 	// set list view direction
// 	listView_tool->setDirection(ListView::Direction::VERTICAL);
// 	listView_tool->setEnabled(false);
// 	listView_tool->setBounceEnabled(true);
// 	listView_tool->setBackGroundImage("loadingStart.png");
// 	listView_tool->setBackGroundImageScale9Enabled(true);
// 	//listView_Item->setBackGroundColorOpacity(0);
// 	listView_tool->setBackGroundImageOpacity(0);
// 	listView_tool->setContentSize(ax::Size(list_w, list_h));
// 	//listView_Item->setInnerContainerSize(ax::Size(110, 110));
// 	//listView_Item->setPosition(ax::Vec2(m_size.width/2+40,m_size.height/2-50));
// 	listView_tool->setPosition(ax::Vec2(150,nPageSize.height-80-list_h));
// 	listView_tool->setTag(LIST_VIEW_TOOL);
// 	listView_tool->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::selectedItemEvent));
// 	_PageLayer_3->addChild(listView_tool);

// 	//	int count = m_array->count();
// 	// add custom item
// 	for (int i = 0; i < MAX_ITEMLIST_COUNT; ++i)
// 	{
// 		custom_button_tool[i] = Button::create();
// 		custom_button_tool[i]->setName("TextButton");
// 		custom_button_tool[i]->setEnabled(false);
// 		custom_button_tool[i]->loadTextures("ui/textbg.png", "ui/state/life_pk.png", "");
// 		custom_button_tool[i]->setScale9Enabled(true);
// 		custom_button_tool[i]->setContentSize(btnSize);
// 		custom_button_tool[i]->setTitleText("The Keyword");
// 		//sprintf(nsetionKey,"tool_%d",i);
// 		nAutoDaiju.GetString(i+1,1,"The Keyword",nDestStr,sizeof(nDestStr));
// 		if (nDestStr[0])
// 			custom_button_tool[i]->setTitleText(UTEXT(nDestStr,1));

// 		Layout *custom_item = Layout::create();
// 		custom_item->setContentSize(custom_button_tool[i]->getSize());
// 		custom_button_tool[i]->setPosition(ax::Vec2(custom_item->getSize().width/2, custom_item->getSize().height/2));
// 		custom_item->addChild(custom_button_tool[i]);

// 		listView_tool->pushBackCustomItem(custom_item);
// 	}
// 	listView_tool->forceDoLayout();
// 	listView_tool->jumpToBottom();


// 	listView_use = ListView::create();
// 	// set list view direction
// 	listView_use->setDirection(ListView::Direction::VERTICAL);
// 	listView_use->setEnabled(false);
// 	listView_use->setBounceEnabled(true);
// 	listView_use->setBackGroundImage("loadingStart.png");
// 	listView_use->setBackGroundImageScale9Enabled(true);
// 	//listView_Item->setBackGroundColorOpacity(0);
// 	listView_use->setBackGroundImageOpacity(0);
// 	listView_use->setContentSize(ax::Size(list_w, list_h));
// 	//listView_Item->setInnerContainerSize(ax::Size(110, 110));
// 	//listView_Item->setPosition(ax::Vec2(m_size.width/2+40,m_size.height/2-50));
// 	listView_use->setPosition(ax::Vec2(290,nPageSize.height-80-list_h));
// 	listView_use->setTag(LIST_VIEW_USE);
// 	listView_use->addEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::selectedItemEvent));
// 	_PageLayer_3->addChild(listView_use);

// 	//	int count = m_array->count();
// 	// add custom item
// 	for (int i = 0; i < MAX_ITEMLIST_COUNT; ++i)
// 	{
// 		custom_button_use[i] = Button::create();
// 		custom_button_use[i]->setName("TextButton");
// 		custom_button_use[i]->setEnabled(false);
// 		custom_button_use[i]->loadTextures("ui/textbg.png", "ui/state/life_pk.png", "");
// 		custom_button_use[i]->setScale9Enabled(true);
// 		custom_button_use[i]->setContentSize(btnSize);
// 		custom_button_use[i]->setTitleText("Item Keyword");
// 		//sprintf(nsetionKey,"use_%d",i);
// 		nAutoUseItem.GetString(i+1,1,"Item Keyword",nDestStr,sizeof(nDestStr));
// 		if (nDestStr[0])
// 			custom_button_use[i]->setTitleText(UTEXT(nDestStr,1));
// 		Layout *custom_item = Layout::create();
// 		custom_item->setContentSize(custom_button_use[i]->getSize());
// 		custom_button_use[i]->setPosition(ax::Vec2(custom_item->getSize().width/2, custom_item->getSize().height/2));
// 		custom_item->addChild(custom_button_use[i]);

// 		listView_use->pushBackCustomItem(custom_item);
// 	}
// 	listView_use->forceDoLayout();
// 	listView_use->jumpToBottom();

//     /*pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf",12);//Arial
// 	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pMoneyLabel->setPosition(ax::Vec2(60,128));
// 	ParentNode_Team->addChild(pMoneyLabel,1);

// 	pXuLabel = Label::createWithTTF("","fonts/gb_song.ttf",12);//Arial
// 	pXuLabel->setAnchorPoint(ax::Vec2(0,0));
// 	pXuLabel->setPosition(ax::Vec2(60,105));
// 	ParentNode_Team->addChild(pXuLabel,1);*/

// 	//Color4B color(112, 128, 144, 150);
// 	/*colorLayer = LayerColor::create(color);//颜色层
// 	colorLayer->setPosition(ax::Vec2::ZERO);
// 	colorLayer->setContentSize(ax::Size::ZERO);  //设置和窗口的大小
// 	ParentNode_Team->addChild(colorLayer,1000);*/
// 	//创建一个动作 改变精灵颜色
// 	//red = TintBy::create(0.2,0,-255,-255);//持续时间+颜色
// 	//red->retain();
}

void KuiAutoPlay_vn::valueChanged(Ref* sender/*, CCControlEvent controlEvent*/)
{
	if (!sender || !g_pCoreShell) return;

	ax::extension::ControlSwitch* pSwitch = (ax::extension::ControlSwitch*)sender;
	int tag = pSwitch->getTag();
	switch(tag)
	{
	case BOX_TAG_JULI:
		{
			if (pSwitch->isOn())
			  Player[CLIENT_PLAYER_INDEX].m_isopenjuli = true;
			else
              Player[CLIENT_PLAYER_INDEX].m_isopenjuli = false;
		}
		break;
	case BOX_TAG_SHIQU:
		{
			if (!pSwitch->isOn())
			{
				Player[CLIENT_PLAYER_INDEX].m_Autoplay.nAutoMoney = 0;           //钱
				Player[CLIENT_PLAYER_INDEX].m_Autoplay.nAutoEques= 0;           //装备
				Player[CLIENT_PLAYER_INDEX].m_Autoplay.nAutoMoneyAndEques= 0;    //钱和装备
				Player[CLIENT_PLAYER_INDEX].m_Autoplay.nAutoTuPu= 0;             //神秘图志
				Player[CLIENT_PLAYER_INDEX].m_Autoplay.nAutoTool= 0;
			}
			            //道具
		}
		break;
	case BOX_TAG_TEAM_3:
		{//我是队员
			if (!switchControl_team || !switchControl_team->isOn())
			{
				Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("warning:Turn on auto agree,please！");
				if (pSwitch->isOn())
					pSwitch->setOn(false);

				break;
			}

			if (pSwitch->isOn())
			{
				KUiPlayerTeam mTeam;
				ZeroMemory(&mTeam,sizeof(mTeam));
				if (g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (uintptr_t)&mTeam, 0)) //没有队伍 就创建一支队伍
				{
					if (mTeam.bTeamLeader)
					{
						Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("warning:Is Captain,Can't choose the members！");
						pSwitch->setOn(false);
						break;
					}
					else
					{//退出队伍
						g_pCoreShell->TeamOperation(TEAM_OI_LEAVE, 0, 0);
					}
				}
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,12,TRUE);
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,12,FALSE);
			}
			//if  (switchControl_team_d)
			//     switchControl_team_d->setOn(false);
		}
		break;
	case BOX_TAG_TEAM_2:
		{//我是队长
			if (!switchControl_team || !switchControl_team->isOn())
			{
				Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("warning:Turn on auto agree,please！");
				if (pSwitch->isOn())
					pSwitch->setOn(false);

				break;
			}

			if (pSwitch->isOn())
			{
				KUiPlayerTeam mTeam;
				ZeroMemory(&mTeam,sizeof(mTeam));
				if (!g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (uintptr_t)&mTeam, 0)) //没有队伍 就创建一支队伍
				{
					g_pCoreShell->TeamOperation(TEAM_OI_CREATE, 0, 0);
				}

				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,11,TRUE);
			}
			else
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,11,FALSE);

			//if (switchControl_team_y)
			 //   switchControl_team_y->setOn(false);
		}
		break;
	case BOX_TAG_TEAM_1:
		{//开启自动应答
			if (pSwitch->isOn())
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,10,TRUE);
			else
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,10,FALSE);
		}
		break;
	case BOX_TAG_GENSHUI:
		{//跟随队长
			if (pSwitch->isOn())
			{
				KUiPlayerTeam mTeam;
				ZeroMemory(&mTeam,sizeof(mTeam));
				if (!g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (uintptr_t)&mTeam, 0))
				{//没有队伍,不能开启
					//g_pCoreShell->TeamOperation(TEAM_OI_CREATE, 0, 0);
					//UIMessageBox2("提示:没有加入队伍,不能设置！");  "warning:Is Captain,Can't choose the members！"
					Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("warning:Did not join the team！");
					pSwitch->setOn(false);
					break;
				}
				else
				{
					if (mTeam.bTeamLeader)
					{//是队长 不能跟随自己
						//UIMessageBox2("提示:已经是队长,不能设置！");
						Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("warning:Is already a captain！");
						pSwitch->setOn(false);
						break;
					}
				}
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,29,TRUE);
			}
			else
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,29,FALSE);
		}
		break;
	case BOX_TAG_USEITEM:
		{
			if (pSwitch->isOn())                        //开启使用物品
			{
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,26,TRUE);
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,26,FALSE);
			}
		}
		break;
	case BOX_TAG_MIAOSHA:
		{
			if (pSwitch->isOn())
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,28,TRUE);
			else
			    g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,28,FALSE);
		}
		break;
	case BOX_TAG_SOUND:
		{//声音开关
			if (pSwitch->isOn())
			{//pshuxEditBox->setText(nTempStr);
				_openSound = true;
			    std::string m_WritablePath = m_SDcardDirPath;
				if  (GetRandomNumber(1,10)>=5)
					m_WritablePath += "music/start.mp3"; //download wav
				else
					m_WritablePath += "music/start.wav";
				//CocosDenshion::AudioEngine::sharedEngine()->playEffect(m_WritablePath.c_str());//专门用来播放简短的音效
//				if  (__pSound)
//					__pSound->playBackgroundMusic(m_WritablePath.c_str(),false);//播放音乐

			}
			else
			{
				_openSound = false;
//				if  (__pSound)
//					__pSound->stopBackgroundMusic(true);//播放音乐
			}
		}
		break;
	case BOX_TAG_BAOLIU:
		{
			if (pSwitch->isOn())
			{//pshuxEditBox->setText(nTempStr);
				if (pshuxEditBox)
					pshuxEditBox->setText("0");
			}
			else
			{
				if (pshuxEditBox)
					pshuxEditBox->setText("1");
			}
		}
		break;
	case BOX_TAG_LUXIAN:
		{
			if (pSwitch->isOn())
			{
			  if (switchControl_dingdian)
			     switchControl_dingdian->setOn(false);
			  g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,9,true);
			}
			else
			{

			}
		}
		break;
	case BOX_TAG_DING:
		{
			if (pSwitch->isOn())
			{
				if (switchControl_luxian)
				    switchControl_luxian->setOn(false);
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,9,false);
			}
			else
			{

			}
		}
		break;
	case BOX_TAG_YABIAO:
		{	if (pSwitch->isOn())
			    g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,7,false);
		}
		break;
	case BOX_TAG_LUZHI:
		{//录制脚本
			if (g_pCoreShell->GetAutoplayid())	//获取挂机的状态
			{
				//pSwitch->setOn(false);
				Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Warning:Please stop Auto");
				break;
			}
			if (pSwitch->isOn())
			{//录制路线
				//g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,8,FALSE,FALSE);
				if  (Player[CLIENT_PLAYER_INDEX].m_Autoplay.nIsJiaoBen)
					break;
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,8,TRUE,TRUE);
				//messageBox("开启","提示");
				//UIMessageBox2("提示:\n  1.请走动人物,录制你需要的挂机路线;完毕后,取消勾选<录制路线>=关闭录制!\n  2.挂机时需要选择<路线>模式\n  3.每次勾选<录制路线>都会清除原路线!\n  4.两点之间的距离需小于活动范围");
			}
			else
			{//停止录制
			   //g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,8,TRUE,TRUE);
				if (g_pCoreShell->GetAutoplayid())	//获取挂机的状态
				{
					Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Warning:Please stop Auto");
					break;
				}
				if  (!Player[CLIENT_PLAYER_INDEX].m_Autoplay.nIsJiaoBen)
					break;
			   g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,8,FALSE,FALSE);
			   //messageBox("关闭","提示");
			}
		}
		break;
	case BOX_TAG_BASE:
		{
			if (pSwitch->isOn())
			{
			}
			else
			{
			}
		}
		break;
	default:
		break;
	}

}

void KuiAutoPlay_vn::checkBoxStateEvent(Ref* pSender, CheckBox::EventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;
	switch (type)
	{
        case CheckBox::EventType::UNSELECTED:
		break;
	case CheckBox::EventType::SELECTED:
		break;
	default:
		break;
	}

	if (pSender){
		CheckBox* nTbtn = (CheckBox*)pSender;
		switch(nTbtn->getTag())
		{
			case 1001:
				{
                    // checkBox_Auto_HP->setSelected(false);
					// messageBox("Debug1","Debug2");
				}
				break;
			default:
				break;
		}
	}

}

void KuiAutoPlay_vn::selectedBoxStateEvent(Ref* pSender, CheckBox::EventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;
	switch (type)
	{
	case CheckBox::EventType::UNSELECTED:
		{
		}
		break;
	case CheckBox::EventType::SELECTED:
		{
			if (pSender)
			{
				CheckBox* nTbtn = (CheckBox*)pSender;
				switch(nTbtn->getTag())
				{
				case BOX_TAG_BASE:
					{
						messageBox("选中","选中");
					}
					break;
				default:
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}
//设置确定取消按钮,参数：调用层对象，调用层回调函数
void KuiAutoPlay_vn::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	//"\\spr\\UI3\\主界面\\关闭.spr"
	sprintf(nSprName,"\\spr\\Ui3\\activityguide\\closebutton.spr"); //"\\spr\\Ui3\\组队\\关闭.spr" "\\Spr\\Ui3\\特殊对话条\\输入文字条－取消.spr"
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

	// MenuItemSprite * colseConfirm = MenuItemSprite::create(sprite_colse_normal, sprite_colse_select, CC_CALLBACK_1(KuiAutoPlay_vn::buttonCallBackFunc, this));
	// colseConfirm->setPosition(ax::Vec2(m_size.width/2,14));
	// //加入菜单
	// Menu * menu = Menu::create(colseConfirm,NULL);
	// //menu->setPosition(ax::Vec2(bgSpritePoint.x-m_size.width/2,bgSpritePoint.y-m_size.height/2));//菜单位置设置为弹出框左下
	// menu->setAnchorPoint(ax::Vec2(0,0));
	// menu->setPosition(ax::Vec2(0,0));             //菜单位置设置为弹出框左下
	// ParentNode_Team->addChild(menu);         //加到父层里

	// Create the checkbox
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\Spr\\UI3\\状态与装备\\分页.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Texture2D * check_btn_normal = bgCur;
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Texture2D * check_btn_active = bgCur;

	float nTempFontSize =24;
	float nOffsetY = 100;

	float nOffsetX = 150;

	btnPageBox_1 = Button::create();
	btnPageBox_1->setEnabled(true);
	// btnPageBox_1->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_1->loadTextures("ui/textbg.png", "ui/textbg_1.png", "");
    btnPageBox_1->setScaleY(3.0);
	btnPageBox_1->setTitleFontSize(nTempFontSize);
	btnPageBox_1->setTitleText("Cơ bản");
	//btnPageBox_1->setAnchorPoint(ax::Vec2(0,0));
	btnPageBox_1->setPosition(ax::Vec2(nOffsetX+50,nOffsetY));
	btnPageBox_1->setTag(AUTOBTN_PAGE_BASE);
	btnPageBox_1->setTag(1);
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	btnPageBox_1->addTouchEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_1);

	btnPageBox_2 = Button::create();
	btnPageBox_2->setEnabled(true);
	// btnPageBox_2->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
    btnPageBox_2->loadTextures("ui/textbg.png", "ui/textbg_1.png", "");
    btnPageBox_2->setScaleY(3.0);
	btnPageBox_2->setTitleFontSize(nTempFontSize);
	btnPageBox_2->setTitleText("Chiến đấu");
	//btnPageBox_1->setAnchorPoint(ax::Vec2(0,0));
	btnPageBox_2->setPosition(ax::Vec2(nOffsetX+150,nOffsetY));
	btnPageBox_2->setTag(AUTOBTN_PAGE_ART);
	btnPageBox_2->setTag(2);
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	btnPageBox_2->addTouchEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_2);

	btnPageBox_3 = Button::create();
	btnPageBox_3->setEnabled(true);
	// btnPageBox_3->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
    btnPageBox_3->loadTextures("ui/textbg.png", "ui/textbg_1.png", "");
    btnPageBox_3->setScaleY(3.0);
	btnPageBox_3->setTitleFontSize(nTempFontSize);
	btnPageBox_3->setTitleText("Nhặt đồ");
	//btnPageBox_1->setAnchorPoint(ax::Vec2(0,0));
	btnPageBox_3->setPosition(ax::Vec2(nOffsetX+250,nOffsetY));
	btnPageBox_3->setTag(AUTOBTN_PAGE_ITEM_1);
	btnPageBox_3->setTag(3);
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	btnPageBox_3->addTouchEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_3);

	btnPageBox_4 = Button::create();
	btnPageBox_4->setEnabled(true);
	// btnPageBox_4->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
    btnPageBox_4->loadTextures("ui/textbg.png", "ui/textbg_1.png", "");
    btnPageBox_4->setScaleY(3.0);
	btnPageBox_4->setTitleFontSize(nTempFontSize);
	btnPageBox_4->setTitleText("Khác");
	//btnPageBox_1->setAnchorPoint(ax::Vec2(0,0));
	btnPageBox_4->setPosition(ax::Vec2(nOffsetX+350,nOffsetY));
	btnPageBox_4->setTag(AUTOBTN_PAGE_ITEM_2);
	btnPageBox_4->setTag(4);
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	btnPageBox_4->addTouchEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_4);

	btnPageBox_5 = Button::create();
	btnPageBox_5->setEnabled(true);
	// btnPageBox_5->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
    btnPageBox_5->loadTextures("ui/textbg.png", "ui/textbg_1.png", "");
    btnPageBox_5->setScaleY(3.0);
	btnPageBox_5->setTitleFontSize(nTempFontSize);
	btnPageBox_5->setTitleText("Kịch bản");
	//btnPageBox_1->setAnchorPoint(ax::Vec2(0,0));
	btnPageBox_5->setPosition(ax::Vec2(nOffsetX+450,nOffsetY));
	btnPageBox_5->setTag(AUTOBTN_PAGE_OTHER);
	btnPageBox_5->setTag(5);
	//btnPageBox_1->ignoreContentAdaptWithSize(true);
	btnPageBox_5->addTouchEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_5);

	// //增加启动　保存按钮
	// Button* button = Button::create();
	// button->setEnabled(true);
	// button->loadTextures("ui/textbg.png", "ui/textbg_1.png", "");
    // button->setScaleY(3.0);
	// button->setTitleFontSize(24);
	// button->setTitleText("Mặc định");
	// button->setPosition(ax::Vec2(m_size.width-200,m_size.height-100));
	// button->setTag(1);
	// button->setTag("1");
	// button->addTouchEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::oktouchEvent));
	// m_pUiLayer->addChild(button);

	Button* _button = Button::create();
	_button->setEnabled(true);
	_button->loadTextures("ui/textbg.png", "ui/textbg_1.png", "");
    _button->setScaleY(3.0);
	_button->setTitleFontSize(24);
	_button->setTitleText("Đóng lại");
	_button->setPosition(ax::Vec2(m_size.width-200,100));
	_button->setTag(2);
	// _button->addTouchEventListener(CC_CALLBACK_2(KuiAutoPlay_vn::oktouchEvent));
	_button->addTouchEventListener(CC_CALLBACK_1(KuiAutoPlay_vn::buttonCallBackFunc, this));
	m_pUiLayer->addChild(_button);

	//开始创建线程
	//Start();
}

//执行上层对象的回调函数，关闭弹出框
void KuiAutoPlay_vn::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node= dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//设置tag，在调用层可以获取到
	if(g_pCoreShell->GetAutoplayid())	//获取挂机的状态
	{//如果是激活的 就隐藏
		_removeDelegate(false);
		this->setVisible(false);
		return;
	}

	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);
	this->removeFromParentAndCleanup(true);
}

//关闭弹出框
void KuiAutoPlay_vn::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

void KuiAutoPlay_vn::clearTempSelindex()
{
	nTempSelItemIndex = -1;
	nTempSeltoolIndex = -1;
	nTempSeluseIndex  = -1;
}
//列表点击
void KuiAutoPlay_vn::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
	if (!pSender) return;
	char msg[64];
	switch (type)
	{
	case ListView::EventType::ON_SELECTED_ITEM_START:
		{
			ListView* listView_ItemEx = static_cast<ListView*>(pSender);
			if (listView_ItemEx)
			{
				int tag          = listView_ItemEx->getTag();
				int nCurSelIndex = listView_ItemEx->getCurSelectedIndex();
				/*
				int   nTempSelItemIndex;
				int   nTempSeltoolIndex;
				int   nTempSeluseIndex;

				*/
				switch(tag)
				{
				case LIST_VIEW_ITEM:
					{//装备
						nTempSelItemIndex = nCurSelIndex;
						nTempSeltoolIndex = -1;
						nTempSeluseIndex  = -1;
						if (pstrEditBox_s)
						{
							Widget *item = listView_ItemEx->getItem(nCurSelIndex);
							Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
							pstrEditBox_s->setText(std::string(button->getTitleText()).c_str());
						}

					}
					break;
				case LIST_VIEW_TOOL:
					{//道具
						nTempSelItemIndex = -1;
						nTempSeltoolIndex = nCurSelIndex;
						nTempSeluseIndex  = -1;
						if (pstrEditBox_d)
						{
							Widget *item = listView_ItemEx->getItem(nCurSelIndex);
							Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
							pstrEditBox_d->setText(std::string(button->getTitleText()).c_str());
						}
					}
					break;
				case LIST_VIEW_USE:
					{//使用物品
						nTempSelItemIndex = -1;
						nTempSeltoolIndex = -1;
						nTempSeluseIndex  = nCurSelIndex;
						if (pstrEditBox_u)
						{
							Widget *item = listView_ItemEx->getItem(nCurSelIndex);
							Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
							pstrEditBox_u->setText(std::string(button->getTitleText()).c_str());
						}

					}
					break;
				default:
					break;
				}
				sprintf(msg,"开始:%d",listView_ItemEx->getCurSelectedIndex());
			}
				//CCLOG("select child start index = %d", listView_ItemEx->getCurSelectedIndex());
			break;
		}
	case ListView::EventType::ON_SELECTED_ITEM_END:
		{
			ListView* listView_ItemEx = static_cast<ListView*>(pSender);
			if (listView_ItemEx)
				sprintf(msg,"结束:%d",listView_ItemEx->getCurSelectedIndex());
				//CCLOG("select child end index = %d", listView_ItemEx->getCurSelectedIndex());
			break;
		}
	default:
		break;
	}
}


//重写触摸注册函数，重新给定触摸级别
void KuiAutoPlay_vn::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//这里的触摸优先级设置为－128，与CCMenu同级，保证了屏蔽下方的触摸
//	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,128, true);
}

Rect KuiAutoPlay_vn::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //这个因该是图内偏移
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //起点最左边
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //最上边    //- rc.origin.y

	return rc;
}

//点击按钮回调函数
void KuiAutoPlay_vn::oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;

	switch (type)
	{
	case Widget::TouchEventType::BEGAN://Touch Down
		{
			if (pSender)
			{
				Button* nTbtn = (Button*)pSender;
				nTbtn->setTitleColor(ax::Color3B::YELLOW);
				if  (nTbtn->getTag()==1)
				{//激活取消
					//检测是不是新手村
					int n_mapindex = g_GameWorld->getMapIdx();
					bool isbreak=false;
					switch(n_mapindex)
					{
					case 1:
					case 11:
					case 20:
					case 37:
					case 53:
					case 54:
					case 78:
					case 99:
					case 100:
					case 101:
					case 121:
					case 153:
					case 174:
					case 175:
					case 176:
						isbreak = true;
						break;
					default:
						break;
					}

					if(g_pCoreShell->GetAutoplayid())	//获取挂机的状态
						g_pCoreShell->OperationRequest(GOI_AUTOPALYOPEN,0,-1,100);  //关闭挂机
					else
					{//m_pSelf->nXpos=pInfo->nScenePos0/8;
						//m_pSelf->nYpos=pInfo->nScenePos1/8;
						if  (isbreak)
						{
							//messageBox("This Map don't run AutoPlay","Warning");
							//break;
						}

						g_pCoreShell->Setpos(nPosX/8,nPosY/8);                                        //设定挂机起点
						g_pCoreShell->OperationRequest(GOI_AUTOPALYOPEN,1,-1,101);
					}
				}
				else if  (nTbtn->getTag()==3)
				{//保存修改装备属性
					if (listView_Item && nTempSelItemIndex!=-1 && pstrEditBox_s)
					{
						Widget *item = listView_Item->getItem(nTempSelItemIndex);
						Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
						button->setTitleText(pstrEditBox_s->getText());
						messageBox("Modify Successful","Warning");
					}
				}
				else if  (nTbtn->getTag()==4)
				{//保存修改道具
					if (listView_tool && nTempSeltoolIndex!=-1 && pstrEditBox_d)
					{
						Widget *item = listView_tool->getItem(nTempSeltoolIndex);
						Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
						button->setTitleText(pstrEditBox_d->getText());
						messageBox("Modify Successful","Warning");
					}
				}
				else if  (nTbtn->getTag()==5)
				{//保存修改使用物品
					if (listView_use && nTempSeluseIndex!=-1 && pstrEditBox_u)
					{
						Widget *item = listView_use->getItem(nTempSeluseIndex);
						Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
						button->setTitleText(pstrEditBox_u->getText());
						messageBox("Modify Successful","Warning");
					}
				}
				else if  (nTbtn->getTag()==2)
				{//保存配置
					if(switchControl_baohu->isOn())
						nAutoNeiGua.WriteInteger("AAAA","IsOpenBaohu",1);
					else
						nAutoNeiGua.WriteInteger("AAAA","IsOpenBaohu",0);

					nAutoNeiGua.WriteString("AAAA","nBaohuInfo1",pLifeEditBox->getText());  //血
					nAutoNeiGua.WriteString("AAAA","nBaohuInfo2",pNeiliEditBox->getText()); //蓝
					nAutoNeiGua.WriteString("AAAA","nBaohuInfo3",pLifeEditBox_h->getText());//回城血量


					if(switchControl_zhandou->isOn())
						nAutoNeiGua.WriteInteger("BBBB","IsOpenZhanDou",1);
					else
						nAutoNeiGua.WriteInteger("BBBB","IsOpenZhanDou",0);

					nAutoNeiGua.WriteString("BBBB","FindNpcDis",pXunguaiEditBox->getText());

					nAutoNeiGua.WriteString("BBBB","GongJiFanWei",pGongjiEditBox->getText());

					nAutoNeiGua.WriteString("BBBB","GongJiNun",pGongjiEditBox_m->getText());

					nAutoNeiGua.WriteString("BBBB","QuXiaoTime",pEditBox_yanshi->getText());

					if(switchControl_shiqu->isOn())
						nAutoNeiGua.WriteInteger("BBBB","IsOpenShiQu",1);
					else
						nAutoNeiGua.WriteInteger("BBBB","IsOpenShiQu",0);

					nAutoNeiGua.WriteString("BBBB","ShiQuNum",pshiquEditBox_c->getText());
					int nisOpen=0,nsaveMonye=0;
					if  (switchControl_savem->isOn())
						nisOpen =1;
					char nTempMoney[64];
					sprintf(nTempMoney, "%s", pmoneyEditBox->getText());
					nsaveMonye = g_Atoui(nTempMoney);
					nAutoNeiGua.WriteInteger2("BBBB","IsOpenAutoSave",nisOpen,nsaveMonye);

					if  (switchControl_fenjie->isOn())
						 nisOpen =1;
					else
						 nisOpen =0;
					nAutoNeiGua.WriteInteger("BBBB","IsOpenFenJie",nisOpen);

					if  (switchControl_yanshi->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("BBBB","IsOpenTime",nisOpen);
					char ntempKey[32];
					for (int i=0;i<MAX_SKILL_COUNT;i++)
					{
						if (switchControl_skill[i])
						{
							if  (switchControl_skill[i]->isOn())
								nisOpen =1;
							else
								nisOpen =0;
							sprintf(ntempKey,"IsOpenSkill_%d",i);
							nAutoNeiGua.WriteInteger("BBBB",ntempKey,nisOpen);
						}
					}
					nAutoNeiGua.WriteString("CCCC","ShuXingNum",pshuxEditBox->getText());
					if  (switchControl_baoliu->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("CCCC","AllKeep",nisOpen);

					if  (switchControl_item_m->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("CCCC","IsQian",nisOpen);
					if  (switchControl_item_d->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("CCCC","IsDaoJu",nisOpen);
					if  (switchControl_item_t->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("CCCC","IsTuZhi",nisOpen);
					if  (switchControl_item_z->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("CCCC","IsZhuang",nisOpen);
					if  (switchControl_item_zm->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("CCCC","IsQianZhuang",nisOpen);

					if  (switchControl_useitem->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("DDDD","IsOpenUseIetm",nisOpen);

					if  (switchControl_miaosha->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("DDDD","IsOpenNoBlue",nisOpen);


					if  (switchControl_dingdian->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("BBBB","IsXiTong",nisOpen);

					if  (switchControl_luxian->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("BBBB","IsJiaoBen",nisOpen);

					if  (switchControl_other_s->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("EEEE","IsOenSound",nisOpen);

					if  (switchControl_other_j->isOn())
						nisOpen =1;
					else
						nisOpen =0;
					nAutoNeiGua.WriteInteger("EEEE","IsOenjuli",nisOpen);

					nAutoNeiGua.Save(nFilePath);
					int i;
					char nsetionKey[32],nTempStr[64];
					KTabFileCtrl nTempTxt;
					nTempTxt.CreatFile(SCHEME_INI_AUTO_ITEM);
					if (listView_Item && nTempTxt.Load(SCHEME_INI_AUTO_ITEM))
					{
					  for (i=0;i<MAX_ITEMLIST_COUNT;i++)
					  {
						//sprintf(nsetionKey,"item_%d",i);
						Widget *item = listView_Item->getItem(i);
						Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
						std::string _nTempStr = std::string(button->getTitleText());
						std::string _nstr_a = _nTempStr.substr(0, _nTempStr.rfind(","));
						std::string _nstr_b = _nTempStr.substr(_nTempStr.rfind(",")+1,_nTempStr.length());
						sprintf(nsetionKey, "%s", _nstr_b.c_str());
						sprintf(nTempStr, "%s", _nstr_a.c_str());
						int nRows = 0;
						nRows=nTempTxt.GetHeight();
						nTempTxt.InsertAfter(nRows);
						nTempTxt.WriteString(nRows+1,1,nTempStr);//Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex);
						nTempTxt.WriteInteger(nRows+1,2,g_Atoui(nsetionKey));
						//nTempTxt.WriteInteger(nRows+1,3,nY);
						//nAutoZhuang.WriteString(nsetionKey,"listStr",_nstr_a.c_str());
						//nAutoZhuang.WriteString(nsetionKey,"listStrVer",_nstr_b.c_str());
					  }
						nTempTxt.Save(SCHEME_INI_AUTO_ITEM);
						nTempTxt.Clear();
					}

					nTempTxt.CreatFile(SCHEME_INI_AUTO_DAOJU);
					if (listView_tool && nTempTxt.Load(SCHEME_INI_AUTO_DAOJU))
					{
						for (i=0;i<MAX_ITEMLIST_COUNT;i++)
						{
							//sprintf(nsetionKey,"tool_%d",i);
							Widget *item = listView_tool->getItem(i);
							Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
							int nRows = 0;
							nRows=nTempTxt.GetHeight();
							nTempTxt.InsertAfter(nRows);
							sprintf(nTempStr, "%s", std::string(button->getTitleText()).c_str());
							nTempTxt.WriteString(nRows+1,1,nTempStr);//Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex);
							//nTempTxt.WriteInteger(nRows+1,2,g_Atoui(nsetionKey));
							//nAutoDaiju.WriteString(nsetionKey,"listStr",button->getTitleText());
						}
						//nAutoDaiju.Save(SCHEME_INI_AUTO_DAOJU);
						nTempTxt.Save(SCHEME_INI_AUTO_DAOJU);
						nTempTxt.Clear();
					}

					nTempTxt.CreatFile(SCHEME_INI_AUTO_USE);
					if (listView_use && nTempTxt.Load(SCHEME_INI_AUTO_USE))
					{
						for (i=0;i<MAX_ITEMLIST_COUNT;i++)
						{
							//sprintf(nsetionKey,"use_%d",i);
							Widget *item = listView_use->getItem(i);
							Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
							int nRows = 0;
							nRows=nTempTxt.GetHeight();
							nTempTxt.InsertAfter(nRows);
							sprintf(nTempStr, "%s", std::string(button->getTitleText()).c_str());
							nTempTxt.WriteString(nRows+1,1,nTempStr);//Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex);
							//nAutoUseItem.WriteString(nsetionKey,"listStr",button->getTitleText());
						}
						//nAutoUseItem.Save(SCHEME_INI_AUTO_USE);
						nTempTxt.Save(SCHEME_INI_AUTO_USE);
						nTempTxt.Clear();
					}

					// messageBox("Save the auto Config successful..","Warning");

				}
			}
		}
		//m_pDisplayValueLabel->setText(CCString::createWithFormat("Touch Down")->getCString());
		break;
	case Widget::TouchEventType::MOVED://Touch Move
		break;

	case Widget::TouchEventType::ENDED://Touch Up
		{
			if (pSender)
			{
				Button* nTbtn = (Button*)pSender;
				nTbtn->setTitleColor(ax::Color3B::WHITE);
				if  (nTbtn->getTag()==1)
				{

				}
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}


void KuiAutoPlay_vn::touchPageBtnEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;

	switch (type)
	{
	case Widget::TouchEventType::BEGAN://Touch Down
		{
			if (pSender)
			{
				Button* nTbtn = (Button*)pSender;
				nTbtn->setTitleColor(ax::Color3B::YELLOW);
				switch(nTbtn->getTag())
				{
				case AUTOBTN_PAGE_BASE:
					{//基本设置
						clearTempSelindex();
						if (listView_Item)
						{
							for (int i = 0; i < MAX_ITEMLIST_COUNT; ++i)
							{
								if (custom_button_item[i])
									custom_button_item[i]->setEnabled(false);
								if (custom_button_tool)
									custom_button_tool[i]->setEnabled(false);
								if (custom_button_use)
									custom_button_use[i]->setEnabled(false);
							}
							listView_Item->setEnabled(false);
							listView_Item->setBounceEnabled(false);
							listView_tool->setEnabled(false);
							listView_tool->setBounceEnabled(false);
							listView_use->setEnabled(false);
							listView_use->setBounceEnabled(false);
						}
						_PageLayer_1->setVisible(true);
						_PageLayer_2->setVisible(false);
						_PageLayer_3->setVisible(false);
						_PageLayer_4->setVisible(false);
						_PageLayer_5->setVisible(false);
						setBtnTouchEnble(1,true);
						setBtnTouchEnble(2,false);
						setBtnTouchEnble(3,false);
						setBtnTouchEnble(4,false);
						setBtnTouchEnble(5,false);

					}
					break;
				case AUTOBTN_PAGE_ART:
					{//攻击设置
						clearTempSelindex();
						if (listView_Item)
						{
							for (int i = 0; i < MAX_ITEMLIST_COUNT; ++i)
							{
								if (custom_button_item[i])
									custom_button_item[i]->setEnabled(false);
								if (custom_button_tool)
									custom_button_tool[i]->setEnabled(false);
								if (custom_button_use)
									custom_button_use[i]->setEnabled(false);
							}
							listView_Item->setEnabled(false);
							listView_Item->setBounceEnabled(false);
							listView_tool->setEnabled(false);
							listView_tool->setBounceEnabled(false);
							listView_use->setEnabled(false);
							listView_use->setBounceEnabled(false);
						}
						_PageLayer_1->setVisible(false);
						_PageLayer_2->setVisible(true);
						_PageLayer_3->setVisible(false);
						_PageLayer_4->setVisible(false);
						_PageLayer_5->setVisible(false);
						setBtnTouchEnble(1,false);
						setBtnTouchEnble(2,true);
						setBtnTouchEnble(3,false);
						setBtnTouchEnble(4,false);
						setBtnTouchEnble(5,false);

					}
					break;
				case AUTOBTN_PAGE_ITEM_1:
					{//物品1设置
						clearTempSelindex();
						_PageLayer_1->setVisible(false);
						_PageLayer_2->setVisible(false);
						_PageLayer_3->setVisible(true);
						_PageLayer_4->setVisible(false);
						_PageLayer_5->setVisible(false);
						if (listView_Item)
						{
							for (int i = 0; i < MAX_ITEMLIST_COUNT; ++i)
							{
								if (custom_button_item[i])
									custom_button_item[i]->setEnabled(true);
								if (custom_button_tool)
									custom_button_tool[i]->setEnabled(true);
								if (custom_button_use)
									custom_button_use[i]->setEnabled(true);
							}
							listView_Item->setEnabled(true);
							listView_Item->setBounceEnabled(true);
							listView_tool->setEnabled(true);
							listView_tool->setBounceEnabled(true);
							listView_use->setEnabled(true);
							listView_use->setBounceEnabled(true);
						}
						setBtnTouchEnble(1,false);
						setBtnTouchEnble(2,false);
						setBtnTouchEnble(3,true);
						setBtnTouchEnble(4,false);
						setBtnTouchEnble(5,false);
					}
					break;
				case AUTOBTN_PAGE_ITEM_2:
					{//物品2设置
						clearTempSelindex();
						if (listView_Item)
						{
							for (int i = 0; i < MAX_ITEMLIST_COUNT; ++i)
							{
								if (custom_button_item[i])
									custom_button_item[i]->setEnabled(false);
								if (custom_button_tool)
									custom_button_tool[i]->setEnabled(false);
								if (custom_button_use)
									custom_button_use[i]->setEnabled(false);
							}
							listView_Item->setEnabled(false);
							listView_Item->setBounceEnabled(false);
							listView_tool->setEnabled(false);
							listView_tool->setBounceEnabled(false);
							listView_use->setEnabled(false);
							listView_use->setBounceEnabled(false);
						}
						_PageLayer_1->setVisible(false);
						_PageLayer_2->setVisible(false);
						_PageLayer_3->setVisible(false);
						_PageLayer_4->setVisible(true);
						_PageLayer_5->setVisible(false);
						setBtnTouchEnble(1,false);
						setBtnTouchEnble(2,false);
						setBtnTouchEnble(3,false);
					    setBtnTouchEnble(4,true);
						setBtnTouchEnble(5,false);
					}
					break;
				case AUTOBTN_PAGE_OTHER:
					{//其他设置
						clearTempSelindex();
						if (listView_Item)
						{
							for (int i = 0; i < MAX_ITEMLIST_COUNT; ++i)
							{
								if (custom_button_item[i])
									custom_button_item[i]->setEnabled(false);
								if (custom_button_tool)
									custom_button_tool[i]->setEnabled(false);
								if (custom_button_use)
									custom_button_use[i]->setEnabled(false);
							}
							listView_Item->setEnabled(false);
							listView_Item->setBounceEnabled(false);
							listView_tool->setEnabled(false);
							listView_tool->setBounceEnabled(false);

							listView_tool->setEnabled(false);
							listView_tool->setBounceEnabled(false);

							listView_use->setEnabled(false);
							listView_use->setBounceEnabled(false);
						}
						_PageLayer_1->setVisible(false);
						_PageLayer_2->setVisible(false);
						_PageLayer_3->setVisible(false);
						_PageLayer_4->setVisible(false);
						_PageLayer_5->setVisible(true);

						setBtnTouchEnble(1,false);
						setBtnTouchEnble(2,false);
						setBtnTouchEnble(3,false);
						setBtnTouchEnble(4,false);
						setBtnTouchEnble(5,true);

					}
					break;
				default:
					break;
				}
			}
		}
		//m_pDisplayValueLabel->setText(CCString::createWithFormat("Touch Down")->getCString());
		break;
	case Widget::TouchEventType::MOVED://Touch Move
		break;

	case Widget::TouchEventType::ENDED://Touch Up
		{
			if (pSender)
			{
				Button* nTbtn = (Button*)pSender;
				nTbtn->setTitleColor(ax::Color3B::WHITE);
				if  (nTbtn->getTag()==AUTOBTN_PAGE_BASE)
				{
					//buttonCallBackFunc(NULL);
					//messageBox(nTbtn->getTag().c_str(),"按下按钮");
				}
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

//触摸函数ccTouchBegan，返回true
bool KuiAutoPlay_vn::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	/*Point location = pTouch->getLocation();//目的的坐标?屏幕点击处
	Sprite * SkillNode =NULL;
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; ++i)
	{
		SkillNode = (Sprite *)ParentNode_Team->getChildByTag(i+1);
		if  (SkillNode)
		{
			if (getRect(SkillNode,i+1).containsPoint(location))
			{//点中了
				SkillNode->runAction(red);
			}
			else
			{
				SkillNode->setColor(ax::Color3B::WHITE);           //默认颜色
			}
		}
	}
	return true;*/
}

std::string  KuiAutoPlay_vn::creatAutoPlayDir(std::string nTempPath)
{
	std::string m_TempDir = ax::FileUtils::getInstance()->getWritablePath();
	m_TempDir += nTempPath;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	DIR *pDir = NULL;
	pDir = opendir(m_TempDir.c_str());
	if (!pDir)
	{
		mkdir(m_TempDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else
	if ((GetFileAttributesA(m_TempDir.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(m_TempDir.c_str(),0);
	}
#endif
	return m_TempDir;
}

void KuiAutoPlay_vn::setBtnTouchEnble(int index,bool enble)
{
	switch(index)
	{
	case 1:
		{
			if (switchControl_baohu)
				switchControl_baohu->setEnabled(enble);
			if (switchControl_team_d)
				switchControl_team_d->setEnabled(enble);
			if (switchControl_team)
				switchControl_team->setEnabled(enble);
			if (switchControl_team_y)
				switchControl_team_y->setEnabled(enble);
			if (switchControl_team_g)
				switchControl_team_g->setEnabled(enble);
			if (switchControl_yabiao)
				switchControl_yabiao->setEnabled(enble);
			if (switchControl_baoliu)
				switchControl_baoliu->setEnabled(enble);
			if (switchControl_useitem)
				switchControl_useitem->setEnabled(enble);
			if (switchControl_miaosha)
				switchControl_miaosha->setEnabled(enble);
		}
		break;
	case 3:
		{
			if (switchControl_item_m)
			    switchControl_item_m->setEnabled(enble);
			if (switchControl_item_d)
				switchControl_item_d->setEnabled(enble);
			if (switchControl_item_z)
				switchControl_item_z->setEnabled(enble);
			if (switchControl_item_t)
				switchControl_item_t->setEnabled(enble);
			if (switchControl_item_zm)
				switchControl_item_zm->setEnabled(enble);

			if (_button)
				_button->setEnabled(enble);
			if (_button_1)
				_button_1->setEnabled(enble);
			if (_button_2)
				_button_2->setEnabled(enble);
		}
        break;
	case 2:
		{
			if (switchControl_zhandou)
			    switchControl_zhandou->setEnabled(enble);
			if (switchControl_shiqu)
			    switchControl_shiqu->setEnabled(enble);
			if (switchControl_savem)
			    switchControl_savem->setEnabled(enble);
			if (switchControl_fenjie)
				switchControl_fenjie->setEnabled(enble);
			if (switchControl_yanshi)
				switchControl_yanshi->setEnabled(enble);
			if (switchControl_dingdian)
				switchControl_dingdian->setEnabled(enble);
			if (switchControl_luxian)
				switchControl_luxian->setEnabled(enble);
			if (switchControl_luzhi)
				switchControl_luzhi->setEnabled(enble);


			for (int i=0;i<MAX_SKILL_COUNT;++i)
			{
				if (switchControl_skill[i])
					switchControl_skill[i]->setEnabled(enble);
			}
		}
		break;
	case 5:
		{
			if (switchControl_other_s)
				switchControl_other_s->setEnabled(enble);
			if (switchControl_other_j)
				switchControl_other_j->setEnabled(enble);
		}
		break;
	default:
		break;
	}
}
//创建文件夹
int KuiAutoPlay_vn::CreateFolder(char * nPath)
{
	/* 格式化路径并创建目录 */
	/*for (int i = 0; i < (int)strlen(nPath); i++)
	{
		if (nPath[i] == '/')
		{
			nPath[i] = '\\';
		}
	}
	if(nPath[0]=='\\'||nPath[0]=='/')
	{
		memcpy(oPath,nPath+1,strlen(nPath)-1);
	}
	else
	{
		memcpy(oPath, nPath, strlen(nPath));
	}*/
	char tPath[255];
	if (nPath[0]=='/'|| nPath[0]=='\\')
	{
		nPath[0]='/';
		nPath++;
	}

	for (unsigned int i = 1; i < strlen(nPath); i++)
	{
		if (nPath[i] == '\\') nPath[i] = '/';
		if (nPath[i] == '/')
		{
			memcpy(tPath, nPath, i );
			tPath[i] = 0;
			char nTempDir[256];
			ZeroMemory(nTempDir,sizeof(nTempDir));
			sprintf(nTempDir,"%s%s",m_SDcardDirPath.c_str(),tPath);
#ifdef WIN32 //完整路径
			_mkdir(nTempDir);//tPath
#else
			//DIR *pDir = NULL;
			//pDir = opendir(nTempDir);
			//if (!pDir)
			mkdir(nTempDir,S_IRWXU|S_IRWXG|S_IRWXO);
			//else
			//closedir(pDir);
#endif
		}
	}

	return true;

}

void KuiAutoPlay_vn::_removeDelegate(bool isThis)
{
	this->setVisible(isThis);
	//this->setBounceEnabled(isThis);
	//this->setEnabled(isThis);
	//ParentNode_Team= spriteBox;
	if (isThis)
	   ParentNode_Team->setPosition(ax::Vec2(winSize.width/2,winSize.height/2));
	else
	   ParentNode_Team->setPosition(ax::Vec2(winSize.width*2,winSize.height/2));
	//ax::Director::getInstance()->getTouchDispatcher()->removeDelegate(this);//删除委托
}
//线程
/*void KuiAutoPlay_vn::Run()
{

}
*/
//基本设置循环
void KuiAutoPlay_vn::base_Breathe()
{
    if (switchControl_baohu && switchControl_baohu->isOn())
	{//保护开启的
		if(m_uLifeTime)
		   m_uLifeTime--;

		if (m_uLifeTime<0)
			m_uLifeTime=0;

		int nLife_1 = 0;
		char nTemPstr[64];
		if (pLifeEditBox)
		{
		  sprintf(nTemPstr, "%s", pLifeEditBox->getText());
		  nLife_1 = g_Atoui(nTemPstr);
		}

		if(nLife_1 > 0 && !m_uLifeTime)
		{
			int nRet;
			KUiPlayerRuntimeInfo	Info;
			memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
			g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (intptr_t)&Info, 0);

			if(Info.nLife <= nLife_1)
				nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,0,false); //吃红
			else
				nRet = 0;

			if(nRet)
				m_uLifeTime = LIFE_TIME_DISTANCE_DEFAULT;  //时间 50？
		}
		//吃蓝
		int nMana = 0;
        if (pNeiliEditBox)
		{
			 sprintf(nTemPstr, "%s", pNeiliEditBox->getText());
			 nMana = g_Atoui(nTemPstr);
		}
		if(m_uManaTime)
			m_uManaTime--;

		if (m_uManaTime<0)
			m_uManaTime=0;

		if(nMana > 0 && !m_uManaTime)
		{
			int nRet;
			KUiPlayerRuntimeInfo	Info;
			memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
			g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (intptr_t)&Info, 0);

			if(Info.nMana <= nMana)
				nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,1,false); //吃内
			else
				nRet = 0;

			if(nRet)
				m_uManaTime = LIFE_TIME_DISTANCE_DEFAULT;
		}
		//回城
		int nPortal = 0;
		if (pLifeEditBox_h)
		{
			sprintf(nTemPstr, "%s", pLifeEditBox_h->getText());
			nPortal = g_Atoui(nTemPstr);
		}

		if(m_PortalTime)
			m_PortalTime--;

		if(m_PortalTime<0)
			m_PortalTime=0;

		if(nPortal > 0 && !m_PortalTime)
		{
			int nRet;
			KUiPlayerRuntimeInfo	Info;
			memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
			g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (intptr_t)&Info, 0);
			if(Info.nLife <= nPortal)
				nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,2,TRUE); //回城
			else
			{
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,2,FALSE);       //设置内挂活动状态
				nRet = 0;
			}
			if(nRet)
				m_PortalTime = LIFE_TIME_DISTANCE_DEFAULT;
		}
		//自动使用物品
		if(switchControl_useitem && switchControl_useitem->isOn())
		{
			int nRet;
			if (nUseItemLoopTime)
				nUseItemLoopTime--;
			if (nUseItemLoopTime<=0)
			{
				nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,26,true);
				nUseItemLoopTime=LIFE_TIME_USERITEM_DEFAULT;
			}
		}
		else
		{
			int nReta;
			nReta = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,26,false);
			nUseItemLoopTime=0;
		}
		//跟随队长
		if (switchControl_team_g && switchControl_team_g->isOn())
		{//如果开启了 自动跟随队长 就循环
			//if (m_FollowCheck.IsVisible())
			//	m_FollowCheck.Hide();
			if (switchControl_zhandou && switchControl_zhandou->isOn())
				switchControl_zhandou->setOn(false);//关闭战斗
			if (switchControl_shiqu && switchControl_shiqu->isOn())
				switchControl_shiqu->setOn(false);  //关闭拾取

			if (g_pCoreShell)
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,29,TRUE);
		}
		else
		{
			//if (m_FollowCheck.IsVisible())
			//	m_FollowCheck.Hide();

			if (g_pCoreShell)
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,29,FALSE);
		}

	}
}
//无限循环
void KuiAutoPlay_vn::update(float delta)
{
	if (isOpen && g_pCoreShell)
	{
/*#ifdef WIN32
		if (++g_nServiceLoop & 0x80000000)
		{
			g_nServiceLoop = 0;
		}
		if (g_nServiceLoop & 0x1)
			Sleep(10);       //挂起1毫秒
#else
		usleep(10000); //微秒 毫秒=1000微妙  sleep 为秒
		//sleep(1);
#endif*/
		if (g_pCoreShell->GetAutoplayid())
		{//挂机状态
			base_Breathe();
			if(switchControl_luzhi && switchControl_luzhi->isOn())
			{//如果是勾选状态 取消 录制脚本
				switchControl_luzhi->setOn(false);	//关闭录制
				g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,8,FALSE,FALSE);
			}

			if (switchControl_yabiao && switchControl_yabiao->isOn())
			{//自动押镖
				if (switchControl_zhandou && switchControl_zhandou->isOn())
				   switchControl_zhandou->setOn(false);//关闭战斗
				if (switchControl_shiqu && switchControl_shiqu->isOn())
				   switchControl_shiqu->setOn(false);  //关闭拾取
				if (switchControl_team_g && switchControl_team_g->isOn())
					switchControl_team_g->setOn(false);//关闭跟随队长

				g_pCoreShell->YaBiao();                //开始押镖
				return;
			}
			//如果跟随开启的
			/*if (switchControl_team_g &&switchControl_team_g->isOn())
			{
				if (switchControl_zhandou)
					switchControl_zhandou->setOn(false);//关闭战斗
				if (switchControl_shiqu)
					switchControl_shiqu->setOn(false);  //关闭拾取
			}*/

			if(switchControl_zhandou && switchControl_zhandou->isOn())
			{//自动战斗
				if(switchControl_luxian && switchControl_luxian->isOn())
				{//脚本模式
					if  (switchControl_dingdian && switchControl_dingdian->isOn())
					     switchControl_dingdian->setOn(false);
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,9,TRUE);
				}
				else//定点模式
			        g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,9,FALSE);
				char nTempStr[32];
				int fanwei=180,juli=75,AtrakTime=0;
				if (pGongjiEditBox)
				{//活动范围
					sprintf(nTempStr, "%s", pGongjiEditBox->getText());
					fanwei = g_Atoui(nTempStr);
				}
				if (pXunguaiEditBox)
				{//寻怪范围
					int nNpcDis=0;
					sprintf(nTempStr, "%s", pXunguaiEditBox->getText());
					nNpcDis = g_Atoui(nTempStr);

					if (nNpcDis<90)
					{
						nNpcDis = 90;
						pXunguaiEditBox->setText("90");
					}
					if (nNpcDis>800)
					{
						nNpcDis=800;
						pXunguaiEditBox->setText("800");
					}
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,31,nNpcDis);
				}
				if  (pGongjiEditBox_m)
				{//攻击秒数
					sprintf(nTempStr, "%s", pGongjiEditBox_m->getText());
					AtrakTime = g_Atoui(nTempStr);
				}

				if (AtrakTime<=0)
					AtrakTime=10;

				g_pCoreShell->Guaji(fanwei,juli,AtrakTime); //开始跑动

				if (pshiquEditBox_c)
				{//拾取的次数
                    sprintf(nTempStr, "%s", pshiquEditBox_c->getText());
					AtrakTime = g_Atoui(nTempStr);
					if (AtrakTime<=0)
						AtrakTime=10;
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,17,AtrakTime);
				}
				//拾取攻击延时
				AtrakTime = 10;
				if (pEditBox_yanshi)
				{
					sprintf(nTempStr, "%s", pEditBox_yanshi->getText());
					AtrakTime = g_Atoui(nTempStr);
				}

				if(switchControl_yanshi)
				{//开启延时
					if (!switchControl_yanshi->isOn())
					    switchControl_yanshi->setOn(true);

					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,20,AtrakTime);
				}
				else
				{
					if (switchControl_yanshi && !switchControl_yanshi->isOn())
						switchControl_yanshi->setOn(true);
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,20,AtrakTime);
				}

				//自动存钱
				int  nDesMoney = 0;
				if  (pmoneyEditBox)
				{
					sprintf(nTempStr, "%s", pmoneyEditBox->getText());
					nDesMoney = g_Atoui(nTempStr);
				}
				 nDesMoney=nDesMoney*10000;

				if (m_uMoneyTime)
					m_uMoneyTime--;
				if (m_uMoneyTime<=0)
					m_uMoneyTime=0;

				if(switchControl_savem && switchControl_savem->isOn() && nDesMoney>0 && m_uMoneyTime==0)
				{
					int nMoney=0;
					nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);                     //获取包袱的钱

					if (nMoney>nDesMoney)
					{
						g_pCoreShell->OperationRequest(GOI_MONEY_INOUT_STORE_BOX,true,nMoney-nDesMoney);  //存取钱
					}
					m_uMoneyTime=100;
				}

				item_Breathe();

				if(switchControl_skill[0] &&switchControl_skill[0]->isOn())
				{//开启技能1
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,21,1,TRUE);
				}
				else
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,21,1,FALSE);


				if(switchControl_skill[1] &&switchControl_skill[1]->isOn())
				{//开启技能2
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,21,2,TRUE);
				}
				else
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,21,2,FALSE);

				if(switchControl_skill[2] &&switchControl_skill[2]->isOn())
				{//开启技能3
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,21,3,TRUE);
				}
				else
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,21,3,FALSE);

				if(switchControl_skill[3] && switchControl_skill[3]->isOn())
				{//开启技能4
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,21,4,TRUE);
				}
				else
					g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,21,4,FALSE);

				g_pCoreShell->AutoUseSkill();	 //自动使用技能
			}//end zhandou
			//item_Breathe();
		}
	}
}
//物品循环
void KuiAutoPlay_vn::item_Breathe()
{
	if (switchControl_shiqu && switchControl_shiqu->isOn())
	{//如果拾取 是开启的
		//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("自动捡取开启中");
		if(switchControl_item_m && switchControl_item_m->isOn())  //自动捡钱
		{
			int nRet;
			nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,3,true); //
			//if (switchControl_item_z)
			//	switchControl_item_z->setOn(false);
			if (switchControl_item_zm && switchControl_item_zm->isOn())
				switchControl_item_zm->setOn(false);
		}
		else
		{
			int nReta;
			nReta = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,3,false); //关闭捡钱
		}

		int shuxingCount = 0;
		char nTempStrVer[32];
		if  (pshuxEditBox)
		{//属性数量
			sprintf(nTempStrVer, "%s", pshuxEditBox->getText());
			shuxingCount = g_Atoui(nTempStrVer);
		}

		if(switchControl_item_z && switchControl_item_z->isOn())  //自动捡装备
		{
			int nRet;
			nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,4,true,shuxingCount); //
			//if (switchControl_item_m)
			//	switchControl_item_m->setOn(false);
			if (switchControl_item_zm &&switchControl_item_zm->isOn())
				switchControl_item_zm->setOn(false);
		}
		else
		{
			int nReta;
			nReta = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,4,false,shuxingCount); //关闭捡装备
		}

		if(switchControl_item_zm && switchControl_item_zm->isOn())  //自动钱装备
		{
			int nRet;
			nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,5,true,shuxingCount); //
			if (switchControl_item_z && switchControl_item_z->isOn())
				switchControl_item_z->setOn(false);
			if (switchControl_item_m && switchControl_item_m->isOn())
				switchControl_item_m->setOn(false);
		}
		else
		{
			int nReta;
			nReta = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,5,false,shuxingCount); //关闭捡装备和钱
		}

		if(switchControl_item_t && switchControl_item_t->isOn())  //自动捡图
		{
			int nRet;
			nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,6,true); //
			//m_AutoTu.CheckButton(false);
			//m_AutoTu.CheckButton(false);
		}
		else
		{
			int nReta;
			nReta = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,6,false);
		}

		if(switchControl_item_d && switchControl_item_d->isOn())  //道具拾取
		{
			g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,14,true,shuxingCount);
		}
		else
		{
			g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND,14,false,shuxingCount);
		}


	}
}

void KuiAutoPlay_vn::onDraw(const ax::Mat4 &transform, unsigned int flags)
{
//    ax::Layer::onDraw(transform, flags);
	if (isOpen && g_pCoreShell)
	{
		if (switchControl_team_d && switchControl_team_d->isOn())
			switchControl_team_y->setOn(false);

		if (switchControl_team_y && switchControl_team_y->isOn())
			switchControl_team_d->setOn(false);
	}
	//if (switchControl_team_y)
	//   switchControl_team_y->setOn(false);
	//messageBox("无限循环","draw");
}
