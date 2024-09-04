#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "gamecore/KPlayer.h"
#include "engine/CXmlStream.h"
#include "KuiRoleState.h"
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
#include "gameui/KuiItemdesc.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

enum WAIT_OTHER_WND_OPER_PARAM
{
	UISTOREBOX_IS_AVATAR,
};
// ---> 建立控件与UIEP_*以及可接纳物品的类型的对应关系
static struct UE_CTRL_MAP
{
	int				nPosition;
	const char*		pIniSection;
	int             nCellx;    // số hàng ngang
    int             nCelly;    // số hàng dọc
    float           nOffsetX;  // vị trí x
    float           nOffsetY;  // vị trí y
}CtrlItemMap[_ITEM_COUNT] =
{
	{ UIEP_HEAD,        "Cap"       ,1,1,-201.6,74},   //0 Trang bị - Mũ
    { UIEP_HAND,        "Weapon"    ,1,1,-89.7,175.7},  //1 Trang bị - Vũ khí
    { UIEP_NECK,        "Necklace"  ,1,1,-288,136.5},   //2 Trang bị - Dây chuyền
    { UIEP_FINESSE,     "Bangle"    ,1,1,-89.7,74},    //3 Trang bị - Vòng tay
    { UIEP_BODY,        "Cloth"     ,1,1,-201.6,175.7},  //4 Trang bị - Quần áo
    { UIEP_WAIST,       "Sash"      ,1,1,-201.7,275},  //5 Trang bị - Đai lưng
    { UIEP_FINGER1,     "Ring1"     ,1,1,-288,200},   //6 Trang bị - Chiếc nhẫn
    { UIEP_FINGER2,     "Ring2"     ,1,1,-288,266},   //7 Trang bị - Chiếc nhẫn
    { UIEP_WAIST_DECOR, "Pendant"   ,1,1,-288,332},   //8 Trang bị - Eo xuyết
    { UIEP_FOOT,        "Shoes"     ,1,1,-89.7,275},  //9 Trang bị - Giày
    { UIEP_HORSE,       "Horse"     ,1,1,-204,396.5},  //10 Trang bị - Ngựa
    { UIEP_MASK,        "Mask"      ,1,1,-288,74},    //11 Trang bị - Mặt nạ   // mat na
    { UIEP_PIFENG,      "Mantle"    ,1,1,-288,396.6},   //12 Áo choàng
    { UIEP_YINJIAN,     "Signet"    ,1,1,-130,396.6},  //13 Ấn giám
    { UIEP_SHIPING,     "Shipin"    ,1,1,247,297},  //14 Trang sức
};

KuiRoleState::KuiRoleState()
{
	isOpen = false;
	__isCanMove =true;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	nBakChickTime = 0;
	colorLayer        = NULL;
	red               = NULL;
	ItemBoxNode       = NULL;
	m_nMoney = 0;
	m_nXu = 0;
	m_nNumObjects = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	m_descPadIsOpen =false;
	nIsOpenTxt=false;
	m_CurSelItemIndex = 0;
	for (int i=0;i<_ITEM_COUNT;i++)
	    m_itemIndexBox[i]=0;

	m_ExeProcessSprite =NULL;
	_VipLevel=0;
	pRoleName=NULL;
	pRoleTitle=NULL;
	pRoleTitle_1=NULL;
	pFuYuan=NULL;
	pShenWang=NULL;
	pPaiMing=NULL;
	proelLevel = NULL;

	pLife=NULL;
	pMana=NULL;
	pTili=NULL;

	pSex=NULL;
	pExp=NULL;
	pLeftAct=NULL;
	pRigthAct=NULL;

	pMingZhong=NULL;
	pShanBi=NULL;
	pRunSpeed=NULL;
	pActpeed=NULL;
	pMicCount=NULL;

	pLiLiang=NULL;
	pWauGong=NULL;
	pNeiGong=NULL;
	pShenFa=NULL;

	pPuFang=NULL;
	pBingFang=NULL;
	pLeiFang=NULL;
	pHuoFang=NULL;
	pDuFang=NULL;

	pLucky=NULL;
	pPk=NULL;

	pZhuanSheng=NULL;
	pZhuanShengLevel=NULL;

	pTemDescItem=NULL;

	pRoleVip =NULL;
	liEditBox = NULL;
	waiEditBox= NULL;
	shenEditBox= NULL;
	neiEditBox= NULL;
	__RemainPoint = 0;
	m_bScrolling = false;
	//diff=ax::Vec2::ZERO;
}

KuiRoleState::~KuiRoleState()
{
	isOpen = false;
	nVipInfo.Clear();
	colorLayer        = NULL;
	if (red)
	   red->release();
	//red               = NULL;
}

//创建确定弹出框
KuiRoleState * KuiRoleState::create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiRoleState * popLayer = KuiRoleState::create();
	//popLayer->setTitle(title);
	popLayer->addDialogData();
	//设置按钮
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//初始化
bool KuiRoleState::init()
{
	if ( !Layer::init() ){
		return false;
	}
	winSize = ax::Director::getInstance()->getWinSize();

	Sprite * spriteBox = Sprite::create("ui/dialogBig_role.png");
	spriteBox->setScaleX(0.9);
	ParentNode_Team= spriteBox;
	spriteBox->setPosition(ax::Vec2(winSize.width/2,winSize.height/2));
	this->addChild(spriteBox);

	//获得背景图片的大小
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();

	this->scheduleUpdate();                   //调用函数
	auto listener = ax::EventListenerTouchOneByOne::create();
ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this); //禁止多点触摸
	return true;
}

void KuiRoleState::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*ItemBoxNode = Node::create();
	ItemBoxNode->setPosition(ax::Vec2(0,0));
	ItemBoxNode->setAnchorPoint(ax::Vec2(0,0));
	ParentNode_Team->addChild(ItemBoxNode,2);*/
	//Left=24
	//Top=72

	if  (nVipInfo.Load("\\settings\\rolevip\\rolevipsetting.txt"))
	    nIsOpenTxt=true;

	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;

	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//颜色层
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);  //设置和窗口的大小
	ParentNode_Team->addChild(colorLayer,1000);
	//创建一个动作 改变精灵颜色
	red = TintBy::create(0.2,0,-255,-255);//持续时间+颜色
	red->retain();
	float nfontsize =14;
	float nScale = nfontsize/12;
	//开始创建文本信息
	pRoleName = Label::createWithTTF("",UI_GAME_FONT_SONG, nfontsize);//Arial
	pRoleName->enableOutline(Color4B(0, 0, 0, 255), 2);
	pRoleName->setAnchorPoint(ax::Vec2(0,0));
	pRoleName->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2+175));
	ParentNode_Team->addChild(pRoleName);

	pRoleTitle = Label::createWithTTF("",UI_GAME_FONT_SONG, nfontsize);   //Arial
	pRoleTitle->enableOutline(Color4B(0, 0, 0, 255), 2);
	pRoleTitle->setAnchorPoint(ax::Vec2(0,0));
	pRoleTitle->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2+152));
	pRoleTitle->setColor(ax::Color3B::GREEN);
	ParentNode_Team->addChild(pRoleTitle);

	pRoleTitle_1 = Label::createWithTTF("",UI_GAME_FONT_SONG,nfontsize);   //Arial
	pRoleTitle_1->setAnchorPoint(ax::Vec2(0,0));
	pRoleTitle_1->setPosition(ax::Vec2(360,m_size.height/2+140));
	ParentNode_Team->addChild(pRoleTitle_1);

	pFuYuan   =  Label::createWithTTF("0",UI_GAME_FONT_SONG,nfontsize);
	pFuYuan->setAnchorPoint(ax::Vec2(0,0));
	pFuYuan->enableOutline(Color4B(0, 0, 0, 255), 2);
	pFuYuan->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2+100));
	//pFuYuan->setScale(nScale);
	ParentNode_Team->addChild(pFuYuan);

	pShenWang   = Label::createWithTTF("0",UI_GAME_FONT_SONG,nfontsize);
	pShenWang->setAnchorPoint(ax::Vec2(0,0));
	pShenWang->enableOutline(Color4B(0, 0, 0, 255), 2);
	pShenWang->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+263,m_size.height/2+100));
	//pShenWang->setScale(nScale);
	ParentNode_Team->addChild(pShenWang);

	proelLevel   = Label::createWithTTF("",UI_GAME_FONT_SONG,nfontsize);
	proelLevel->setAnchorPoint(ax::Vec2(0,0));
	proelLevel->enableOutline(Color4B(0, 0, 0, 255), 2);
	proelLevel->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2+75));
	//proelLevel->setScale(nScale);
	ParentNode_Team->addChild(proelLevel);

	pPaiMing   = Label::createWithTTF("",UI_GAME_FONT_SONG,nfontsize);
	pPaiMing->setAnchorPoint(ax::Vec2(0,0));
	pPaiMing->enableOutline(Color4B(0, 0, 0, 255), 2);
	pPaiMing->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+263,m_size.height/2+75));
	//pPaiMing->setScale(nScale);
	ParentNode_Team->addChild(pPaiMing);

	pLife   = Label::createWithTTF("",UI_GAME_FONT_SONG,nfontsize);
	pLife->setAnchorPoint(ax::Vec2(0,0));
    pLife->enableOutline(Color4B(255,0,0,255), 3);
	//pLife->setColor(ax::Color3B::RED);
	//pLife->setScale(nScale);
	pLife->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2+39));
	ParentNode_Team->addChild(pLife);

	pMana   = Label::createWithTTF("",UI_GAME_FONT_SONG,nfontsize);
	pMana->setAnchorPoint(ax::Vec2(0,0));
    pLife->enableOutline(Color4B(0,0,255,255), 3);
	//pMana->setColor(ax::Color3B::BLUE);
	//pMana->setScale(nScale);
	pMana->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2+12));
	ParentNode_Team->addChild(pMana);

	pTili   = Label::createWithTTF("",UI_GAME_FONT_SONG,nfontsize);
	pTili->setAnchorPoint(ax::Vec2(0,0));
    pLife->enableOutline(Color4B(0,240,0,255), 3);
	//pTili->setScale(nScale);
	pTili->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2-13));
	ParentNode_Team->addChild(pTili);

	pSex = Label::createWithTTF("",UI_GAME_FONT_SONG,nfontsize);
	pSex->setAnchorPoint(ax::Vec2(0,0));
	pSex->enableOutline(Color4B(0, 0, 0, 255), 3);
	pSex->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2-40));
	ParentNode_Team->addChild(pSex);

	pExp   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pExp->setAnchorPoint(ax::Vec2(0,0));
	pExp->enableOutline(Color4B(0, 0, 0, 255), 2);
	pExp->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2+127));
	//pExp->setScale(nScale);
	ParentNode_Team->addChild(pExp);

	pLeftAct   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pLeftAct->setAnchorPoint(ax::Vec2(0,0));
	pLeftAct->enableOutline(Color4B(0, 0, 0, 255), 2);
	pLeftAct->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2-74));
	//pLeftAct->setScale(nScale);
	ParentNode_Team->addChild(pLeftAct);

	pRigthAct   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pRigthAct->setAnchorPoint(ax::Vec2(0,0));
	pRigthAct->enableOutline(Color4B(0, 0, 0, 255), 2);
	pRigthAct->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+328,m_size.height/2-74));
	//pRigthAct->setScale(nScale);
	ParentNode_Team->addChild(pRigthAct);

	pMingZhong   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pMingZhong->setAnchorPoint(ax::Vec2(0,0));
	pMingZhong->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2-112));
	pMingZhong->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pMingZhong);

	pShanBi   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pShanBi->setAnchorPoint(ax::Vec2(0,0));
	pShanBi->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2-138));
	pShanBi->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pShanBi);

	pRunSpeed   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pRunSpeed->setAnchorPoint(ax::Vec2(0,0));
	pRunSpeed->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2-190));
	pRunSpeed->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pRunSpeed);

	pActpeed   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pActpeed->setAnchorPoint(ax::Vec2(0,0));
	pActpeed->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2-164));
	pActpeed->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pActpeed);

	pMicCount   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pMicCount->setAnchorPoint(ax::Vec2(0,0));
	pMicCount->enableOutline(Color4B(0, 0, 0, 255), 2);
	pMicCount->setColor(ax::Color3B::YELLOW);
	pMicCount->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+120,m_size.height/2-216));
	ParentNode_Team->addChild(pMicCount);

	pLiLiang   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pLiLiang->setAnchorPoint(ax::Vec2(0,0));
	pLiLiang->enableOutline(Color4B(0, 0, 0, 255), 2);
	pLiLiang->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+328,m_size.height/2+39));
	//pLiLiang->setScale(nScale);
	ParentNode_Team->addChild(pLiLiang);

	pWauGong   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pWauGong->setAnchorPoint(ax::Vec2(0,0));
	pWauGong->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+328,m_size.height/2-13));
	pWauGong->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pWauGong);

	pShenFa   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pShenFa->setAnchorPoint(ax::Vec2(0,0));
	pShenFa->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+328,m_size.height/2+12));
	pShenFa->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pShenFa);

	pNeiGong   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pNeiGong->setAnchorPoint(ax::Vec2(0,0));
	pNeiGong->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+328,m_size.height/2-40));
	pNeiGong->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pNeiGong);

	pPuFang   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pPuFang->setAnchorPoint(ax::Vec2(0,0));
	pPuFang->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+328,m_size.height/2-112));
	pPuFang->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pPuFang);

	pBingFang   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pBingFang->setAnchorPoint(ax::Vec2(0,0));
	pBingFang->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+328,m_size.height/2-138));
	pBingFang->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pBingFang);

	pLeiFang   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pLeiFang->setAnchorPoint(ax::Vec2(0,0));
	pLeiFang->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+328,m_size.height/2-216));
	pLeiFang->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pLeiFang);

	pHuoFang   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pHuoFang->setAnchorPoint(ax::Vec2(0,0));
	pHuoFang->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+328,m_size.height/2-190));
	pHuoFang->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pHuoFang);

	pDuFang   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pDuFang->setAnchorPoint(ax::Vec2(0,0));
	pDuFang->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+328,m_size.height/2-164));
	pDuFang->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pDuFang);

	pLucky   = Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,nfontsize);
	pLucky->setAnchorPoint(ax::Vec2(0,0));
	pLucky->setPosition(ax::Vec2(0,0));
	pLucky->enableOutline(Color4B(0, 0, 0, 255), 2);
	ParentNode_Team->addChild(pLucky);

	pPk   = Label::createWithTTF("",UI_GAME_FONT_SONG,nfontsize);
	pPk->setAnchorPoint(ax::Vec2(0,0));
	pPk->enableOutline(Color4B(0, 0, 0, 255), 2);
	pPk->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+398,m_size.height/2+100));
	ParentNode_Team->addChild(pPk);

	// pPaiMing   = Label::createWithTTF("",UI_GAME_FONT_SONG,nfontsize);
	// pPaiMing->setAnchorPoint(ax::Vec2(0,0));
	// pPaiMing->enableOutline(Color4B(0, 0, 0, 255), 2);
	// pPaiMing->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+263,m_size.height/2+75));
	// //pPaiMing->setScale(nScale);
	// ParentNode_Team->addChild(pPaiMing);



	pZhuanSheng = Label::createWithTTF("",UI_GAME_FONT_SONG,nfontsize);
	pZhuanSheng->setAnchorPoint(ax::Vec2(0,0));
	pZhuanSheng->enableOutline(Color4B(0, 0, 0, 255), 2);
	pZhuanSheng->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width/2+398,m_size.height/2+75));
	ParentNode_Team->addChild(pZhuanSheng);

	// pZhuanShengLevel = Label::createWithTTF(UTEXT("转生等级",1).c_str(),UI_GAME_FONT_SONG, 12);   //Arial
	// pZhuanShengLevel->setAnchorPoint(ax::Vec2(0,0));
	// pZhuanShengLevel->setPosition(ax::Vec2(m_size.width-100,m_size.height/2+108));
	// ParentNode_Team->addChild(pZhuanShengLevel);

	/*Label *addliliang = Label::createWithTTF(UTEXT("分配力量",1).c_str(),UI_GAME_FONT_SONG, 12);   //Arial
	addliliang->setAnchorPoint(ax::Vec2(0,0));
	addliliang->setPosition(ax::Vec2(m_size.width-100,m_size.height/2+108));
	ParentNode_Team->addChild(addliliang);

	Label *addwaigong = Label::createWithTTF(UTEXT("分配外功",1).c_str(),UI_GAME_FONT_SONG, 12);   //Arial
	addwaigong->setAnchorPoint(ax::Vec2(0,0));
	addwaigong->setPosition(ax::Vec2(m_size.width-100,m_size.height/2+108));
	ParentNode_Team->addChild(addwaigong);

	Label *addshenfa = Label::createWithTTF(UTEXT("分配身法",1).c_str(),UI_GAME_FONT_SONG, 12);   //Arial
	addshenfa->setAnchorPoint(ax::Vec2(0,0));
	addshenfa->setPosition(ax::Vec2(m_size.width-100,m_size.height/2+108));
	ParentNode_Team->addChild(addshenfa);

	Label *addneigong = Label::createWithTTF(UTEXT("分配内功",1).c_str(),UI_GAME_FONT_SONG, 12);   //Arial
	addneigong->setAnchorPoint(ax::Vec2(0,0));
	addneigong->setPosition(ax::Vec2(m_size.width-100,m_size.height/2+108));
	ParentNode_Team->addChild(addneigong);　ax::ui::Scale9Sprite *gBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	*/
	//VIP
	pRoleVip = Label::createWithTTF(UTEXT("VIP:0",1).c_str(),UI_GAME_FONT_SONG, nfontsize);   //Arial
	pRoleVip->setAnchorPoint(ax::Vec2(0,0));
	pRoleVip->setPosition(ax::Vec2(250,m_size.height/2+170));
	pRoleVip->setColor(ax::Color3B::YELLOW);
	pRoleVip->enableOutline(Color4B(0, 0, 0, 255), 2);
	//pRoleVip->setVisible(false); //隐藏起来
	ParentNode_Team->addChild(pRoleVip);

	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	m_ExeProcessSprite= Sprite::create("ui/img_carve_progress_green.png");
	m_ExeProcessSprite->setColor(ax::Color3B::GREEN);
	m_ExeProcessSprite->setAnchorPoint(ax::Vec2(0,0));
	m_ExeProcessSprite->setPosition(ax::Vec2(15,m_size.height/2+30));
	m_ExeProcessSprite->setScaleX(0);
	ParentNode_Team->addChild(m_ExeProcessSprite,2);

	Sprite *bgprocess= Sprite::create("ui/state/kbgitem.png");
	ImageView* ProcessBg=ImageView::create();
	ProcessBg->loadTexture("ui/img_carve_progressbg.png");
	ProcessBg->setAnchorPoint(ax::Vec2(0,0));
	ProcessBg->setPosition(ax::Vec2(15,m_size.height/2+30));
	ProcessBg->setScaleX(0.35);
	ParentNode_Team->addChild(ProcessBg,1);
	m_ExeProcessSprite->setVisible(false);
	ProcessBg->setVisible(false);
	// ZeroMemory(nSprName,sizeof(nSprName));
	// t_sprintf(nSprName,"\\spr\\Ui3\\activityguide\\progressbarback.spr");
	// g_StrLower(nSprName);
	// bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);

	// if (bgCur)
	// {
	// 	ImageView* ProcessBg=ImageView::create();
	// 	ProcessBg->loadTextureByData(bgCur);
	// 	ProcessBg->setAnchorPoint(ax::Vec2(0,0));
	// 	ProcessBg->setPosition(ax::Vec2(15,m_size.height/2+30));
	// 	ProcessBg->setScaleX(0.35);
	// 	ParentNode_Team->addChild(ProcessBg,1);
	// }

	ax::ui::Scale9Sprite *lipBg = ax::ui::Scale9Sprite::create("ui/editbg.png");  //"boxgb.png"
	lipBg->setOpacity(125);
	const ax::Color3B nfcolor ={255,255,255};
	liEditBox = EditBox::create(Size(55,24),lipBg);
	if (liEditBox)
	{
		//liEditBox->setText("0");
		liEditBox->setPlaceHolder("Sức mạnh");
		liEditBox->setFontColor(nfcolor);
		liEditBox->setFontSize(24);
		liEditBox->setMaxLength(4);
		liEditBox->setPosition(ax::Vec2(155,m_size.height/2+70));
		liEditBox->setInputMode(EditBox::InputMode::DECIMAL);
		liEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		ParentNode_Team->addChild(liEditBox,1);
	}
	ax::ui::Scale9Sprite *waipBg = ax::ui::Scale9Sprite::create("ui/editbg.png");  //"boxgb.png"
	waipBg->setOpacity(125);
	waiEditBox = EditBox::create(Size(55,24),waipBg);
	if (waiEditBox)
	{
		//waiEditBox->setText("0");
		waiEditBox->setPlaceHolder("Ngoại công");
		waiEditBox->setFontColor(nfcolor);
		waiEditBox->setFontSize(24);
		waiEditBox->setMaxLength(4);
		waiEditBox->setPosition(ax::Vec2(155,m_size.height/2+40));
		waiEditBox->setInputMode(EditBox::InputMode::DECIMAL);
		waiEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		ParentNode_Team->addChild(waiEditBox,1);
	}
	ax::ui::Scale9Sprite *shenBg = ax::ui::Scale9Sprite::create("ui/editbg.png");  //"boxgb.png"
	shenBg->setOpacity(125);
	shenEditBox = EditBox::create(Size(55,24),shenBg);
	if (shenEditBox)
	{
		//shenEditBox->setText("0");
		shenEditBox->setPlaceHolder("Thân pháp");
		shenEditBox->setFontColor(nfcolor);
		shenEditBox->setFontSize(24);
		shenEditBox->setMaxLength(4);
		shenEditBox->setPosition(ax::Vec2(155,m_size.height/2+10));
		shenEditBox->setInputMode(EditBox::InputMode::DECIMAL);
		shenEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		ParentNode_Team->addChild(shenEditBox,1);
	}
	ax::ui::Scale9Sprite *neiBg = ax::ui::Scale9Sprite::create("ui/editbg.png");  //"boxgb.png"
	neiBg->setOpacity(125);
	neiEditBox = EditBox::create(Size(55,24),neiBg);
	if (neiEditBox)
	{
		//neiEditBox->setText("0");
		neiEditBox->setPlaceHolder("Nội công");
		neiEditBox->setFontColor(nfcolor);
		neiEditBox->setFontSize(24);
		neiEditBox->setMaxLength(4);
		neiEditBox->setPosition(ax::Vec2(155,m_size.height/2-20));
		neiEditBox->setInputMode(EditBox::InputMode::DECIMAL);
		neiEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		ParentNode_Team->addChild(neiEditBox,1);
	}
	UpdateBaseData();
}

//设置确定取消按钮,参数：调用层对象，调用层回调函数
void KuiRoleState::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	//Btn close
	MenuItemImage * itemConfirm = MenuItemImage::create("ui/btn_close.png", "ui/btn_close.png", CC_CALLBACK_1(KuiRoleState::buttonCallBackFunc, this));
	itemConfirm->setPosition(ax::Vec2(m_size.width/2-10,m_size.height-60));

	//thao trang bi
	// t_sprintf(nSprName,"\\spr\\Ui3\\主界面\\卸下装备.spr");
	// g_StrLower(nSprName);
	// bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	// if (bgCur==NULL)
	// 	return;
	//  Sprite * sprite_uneq_confirm_normal = Sprite::createWithTexture(bgCur);

	//  bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	//  if (bgCur==NULL)
	// 	 return;
	//  Sprite * sprite_uneq_confirm_select = Sprite::createWithTexture(bgCur);
	//  MenuItemSprite * itemEqConfirm = MenuItemSprite::create(sprite_uneq_confirm_normal, sprite_uneq_confirm_select, CC_CALLBACK_1(KuiRoleState::buttonEqCallBackFunc, this));
	//  itemEqConfirm->setPosition(ax::Vec2(100,24));

	 //sua trang bi
	//  t_sprintf(nSprName,"\\spr\\Ui3\\主界面\\修理装备.spr");
	//  g_StrLower(nSprName);
	//  bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	//  if (bgCur==NULL)
	// 	 return;
	//  Sprite * sprite_xiu_confirm_normal = Sprite::createWithTexture(bgCur);

	//  bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	//  if (bgCur==NULL)
	// 	 return;
	//  Sprite * sprite_xiu_confirm_select = Sprite::createWithTexture(bgCur);
	//  MenuItemSprite * itemXiuConfirm = MenuItemSprite::create(sprite_xiu_confirm_normal, sprite_xiu_confirm_select, CC_CALLBACK_1(KuiRoleState::buttonxiuCallBackFunc, this));
	//  itemXiuConfirm->setPosition(ax::Vec2(186,24));
	 //Cong diem
	MenuItemImage * addConfirm = MenuItemImage::create("ui/btn_common1_1.png", "ui/btn_common1_2.png", CC_CALLBACK_1(KuiRoleState::addCallBackFunc, this));
	Label * addConfirmLabel = Label::createWithTTF("Cộng Điểm",UI_GAME_FONT_DEFAULT_VN_BTN, 17);
	addConfirmLabel->setColor(ax::Color3B::WHITE);
	addConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	addConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
	addConfirmLabel->setPosition(ax::Vec2(addConfirm->getContentSize().width/2-40,addConfirm->getContentSize().height/2-10));
	addConfirm->addChild(addConfirmLabel,1);
	addConfirm->setPosition(ax::Vec2(-340,m_size.height/2-75));

	 Menu * menu = Menu::create(itemConfirm,addConfirm,NULL);
	 menu->setPosition(ax::Vec2(m_size.width/2,13));
	 ParentNode_Team->addChild(menu);          //加到父层里
	//Point bgSpritePoint = ParentNode_Team->getPosition();
}

void KuiRoleState::addCallBackFunc(Ref * pSender)
{
	if (!liEditBox || !waiEditBox|| !shenEditBox || !neiEditBox) return;
	//开始检测字符的合法性
	std::string listr = liEditBox->getText();
	std::string waistr = waiEditBox->getText();
	std::string shenstr = shenEditBox->getText();
	std::string neistr = neiEditBox->getText();
	std::string strcheck="1";
	strcheck+=listr;
	strcheck+=waistr;
	strcheck+=neistr;
	strcheck+=shenstr;
	int count = 0,spaceNum=0,chineseNum=0,other=0,letter=0,xiaoxie=0;
	for(int i = 0; i <strcheck.length();i++)
	{
		if(isdigit(strcheck[i]))     //判断是否是数字0-9
			count++;
		else if(isspace(strcheck[i]))//判断是否是空格
			++spaceNum;
		else if(isalpha(strcheck[i])==1)//判断是否是字母	 1 为大写 2 为 小写  0 为不是字母
		{
			++letter;
		}
		else if(isalpha(strcheck[i])==2)//判断是否是字母	 1 为大写 2 为 小写  0 为不是字母
		{
			++xiaoxie;
		}
		else if(strcheck[i]&0x80)//if(strcheck[0]>=0xB0)
			++chineseNum;        //判断是否是中文
		else
		{
			++other;
		}
	}

	if (spaceNum>0 || chineseNum>0 || xiaoxie >0 || other>0 || letter>0)
	{ 	//system("pause");
#ifdef WIN32
		ccMessageBox("只能填写数字!","警告");
#else
		ccMessageBox(G2U("只能填写数字!").c_str(),G2U("警告").c_str());
#endif
		return;
	}
	if (listr.empty())
		listr ="0";
	if (waistr.empty())
		waistr ="0";
	if (shenstr.empty())
		shenstr ="0";
	if (neistr.empty())
		neistr ="0";
	char nTemStr[16];
	ZeroMemory(nTemStr,sizeof(nTemStr));
	t_sprintf(nTemStr,listr.c_str());
	int liCount = g_Atoui(nTemStr);

	ZeroMemory(nTemStr,sizeof(nTemStr));
	t_sprintf(nTemStr,waistr.c_str());
	int waiCount = g_Atoui(nTemStr);

	ZeroMemory(nTemStr,sizeof(nTemStr));
	t_sprintf(nTemStr,shenstr.c_str());
	int shenCount = g_Atoui(nTemStr);

	ZeroMemory(nTemStr,sizeof(nTemStr));
	t_sprintf(nTemStr,neistr.c_str());
	int neiCount = g_Atoui(nTemStr);

	if (liCount<0 || liCount>10 || waiCount<0 || waiCount>10 ||shenCount<0 ||shenCount>10|| neiCount<0||neiCount>10)
	{
#ifdef WIN32
		ccMessageBox("潜能点分配范围(1-10)","提示");
#else
		ccMessageBox(G2U("潜能点分配范围(1-10)").c_str(),G2U("提示").c_str());
#endif
		return;
	}

	if  (liCount+waiCount+shenCount+neiCount<=__RemainPoint)
	{
		if (g_pCoreShell)
		{
			if  (liCount>0)
			    g_pCoreShell->OperationRequest(GOI_TONE_UP_ATTRIBUTE,UIPA_STRENGTH,liCount);
			if  (waiCount>0)
				g_pCoreShell->OperationRequest(GOI_TONE_UP_ATTRIBUTE,UIPA_VITALITY,waiCount);
			if  (shenCount>0)
				g_pCoreShell->OperationRequest(GOI_TONE_UP_ATTRIBUTE,UIPA_DEXTERITY,shenCount);
			if  (neiCount>0)
				g_pCoreShell->OperationRequest(GOI_TONE_UP_ATTRIBUTE,UIPA_ENERGY,neiCount);
		}
		return;
	}

#ifdef WIN32
	ccMessageBox("潜能点不足!","警告");
#else
	ccMessageBox(G2U("潜能点不足!").c_str(),G2U("警告").c_str());
#endif

}

void KuiRoleState::buttonxiuCallBackFunc(Ref * pSender)
{
	if (m_CurSelItemIndex<=0 || m_CurSelItemIndex>=MAX_ITEM)
	{
#ifdef WIN32
		ccMessageBox("请选择一个装备","提示");
#else
		ccMessageBox(UTEXT("请选择一个装备",1).c_str(),UTEXT("提示",1).c_str());
#endif
		return;
	}
	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);
	unsigned int nCurChicktime = pInfo.npTime;  //记录打开窗口的秒数
	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//间隔２秒
		return;
	}
	nBakChickTime = nCurChicktime;

	if (g_pCoreShell)
	{
		if (g_pCoreShell->GetAutoplayid())
		{
#ifdef WIN32
			ccMessageBox("挂机中,禁止修理!","提示");
#else
			ccMessageBox(UTEXT("挂机中,禁止修理!",1).c_str(),UTEXT("提示",1).c_str());
#endif
			return;
		}
		//g_pCoreShell->AutoChangeItem(m_CurSelItemIndex);
		KUiObjAtContRegion	Obj;
		Obj.Obj.uGenre    = CGOG_ITEM;
		Obj.Obj.uId       = m_CurSelItemIndex;
		Obj.Region.h      = 0;
		Obj.Region.v      = 0;
		Obj.Region.Width  = 0;
		Obj.Region.Height = 0;
		Obj.eContainer    = UOC_EQUIPTMENT;

		if (g_pCoreShell)
			g_pCoreShell->OperationRequest(GOI_TRADE_NPC_REPAIR,(uintptr_t)(&Obj),0);
	}
	m_CurSelItemIndex =0;
}

void KuiRoleState::buttonEqCallBackFunc(Ref * pSender)
{
	if (m_CurSelItemIndex<=0 || m_CurSelItemIndex>=MAX_ITEM)
	{
#ifdef WIN32
	ccMessageBox("请选择一个装备","提示");
#else
	ccMessageBox(UTEXT("请选择一个装备",1).c_str(),UTEXT("提示",1).c_str());
#endif
	  return;
	}
	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);
	unsigned int nCurChicktime = pInfo.npTime;  //记录打开窗口的秒数
	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//间隔２秒
		return;
	}
	nBakChickTime = nCurChicktime;

	if (g_pCoreShell)
	{
		if (g_pCoreShell->GetAutoplayid())
		{
#ifdef WIN32
			ccMessageBox("挂机中,禁止卸载!","提示");
#else
			ccMessageBox(UTEXT("挂机中,禁止卸载!",1).c_str(),UTEXT("提示",1).c_str());
#endif
			return;
		}
	   g_pCoreShell->AutoChangeItem(m_CurSelItemIndex);
	}

	m_CurSelItemIndex =0;
}

//执行上层对象的回调函数，关闭弹出框
void KuiRoleState::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node = dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//设置tag，在调用层可以获取到
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//关闭弹出框
void KuiRoleState::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//重写触摸注册函数，重新给定触摸级别
void KuiRoleState::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//这里的触摸优先级设置为－128，与CCMenu同级，保证了屏蔽下方的触摸
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
}


void KuiRoleState::UpdateAllEquips()
{
	if  (!g_GameWorld || !g_pCoreShell) return;

	KUiObjAtRegion	Equips[_ITEM_COUNT];
	int nCount = g_pCoreShell->GetGameData(GDI_EQUIPMENT, (uintptr_t)&Equips, 0);

	int	i;

	//for (i = 0; i < _ITEM_COUNT; ++i)
		//m_EquipBox[i].Celar();

	for (i = 0; i < nCount; ++i)
	{
		if (Equips[i].Obj.uGenre != CGOG_NOTHING)
			UpdateEquip(&Equips[i], true);  //增加或更换
		else
		    UpdateEquip(&Equips[i], false); //删除这个精灵
	}
}

void KuiRoleState::addpicBox(int index,int nItemIndx,bool isAdd)
{
	char nKey[32];
	t_sprintf(nKey,"%s",CtrlItemMap[index].pIniSection);
	std::string m_ItemBoxKey = nKey;
	t_sprintf(nKey,"%s_color",CtrlItemMap[index].pIniSection);
	std::string m_colorbgKey = nKey;//颜色层的key
	if  (isAdd)
	{//增加
		if (nItemIndx<=0 || nItemIndx>=MAX_ITEM) return;
		if (ParentNode_Team)
		{
			int nItemBoxIdx = Item[nItemIndx].GetDetailType();
			int itemSize = 0;
			int nScale;
			switch(nItemBoxIdx)
			{
			case equip_meleeweapon:
			case equip_rangeweapon:
				{//武器
					itemSize = ITEM_CELL_SIZE;
					nScale = 1;
				}
				break;
			case equip_armor://áo
				{
					itemSize = ITEM_CELL_SIZE;
					nScale = 1;
				}
				break;
			case equip_ring://nhẫn
				{
					itemSize = ITEM_CELL_SIZE_SMALL;
					nScale = 0.7;
				}
				break;
			case equip_amulet://Dây chuyền
				{
					itemSize = ITEM_CELL_SIZE_SMALL;
					nScale = 0.7;
				}
				break;
			case equip_boots:
				{
					itemSize = ITEM_CELL_SIZE;
					nScale = 1;
				}
				break;
			case equip_belt:
				{
					itemSize = ITEM_CELL_SIZE;
					nScale = 1;
				}
				break;
			case equip_helm:
					itemSize = ITEM_CELL_SIZE;
					nScale = 1;
				break;
			case equip_cuff:
				{
					itemSize = ITEM_CELL_SIZE;
					nScale = 1;
				}
				break;
			case equip_pendant:
				{
					itemSize = ITEM_CELL_SIZE_SMALL;
					nScale = 0.7;
				}
				break;
			case equip_horse:
				{
					itemSize = ITEM_CELL_SIZE_SMALL;
					nScale = 0.7;
				}
				break;
			case equip_mask:
				{
					itemSize = ITEM_CELL_SIZE_SMALL;
					nScale = 0.7;
				}
				break;
			case equip_pifeng:
				{
					itemSize = ITEM_CELL_SIZE_SMALL;
					nScale = 0.7;
				}
				break;
			case equip_yinjian:
				{
					itemSize = ITEM_CELL_SIZE_SMALL;
					nScale = 0.7;
				}
				break;
			case equip_shiping:
				{
					itemSize = ITEM_CELL_SIZE_SMALL;
					nScale = 0.7;
				}
				break;
			default:
				break;
			}

			char nItemSprPath[256];
			Texture2D *bgCur = NULL;
			int nItemW = Item[nItemIndx].GetWidth()*itemSize;
			int nItemH = Item[nItemIndx].GetHeight()*itemSize;
			m_itemIndexBox[index]=nItemIndx;
			ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
			if  (Item[nItemIndx].GetImagePath())
				t_sprintf(nItemSprPath,Item[nItemIndx].GetImagePath());
			else
				t_sprintf(nItemSprPath,"\\spr\\others\\问号.spr");

			if  (nItemSprPath[0])
			{

				//t_sprintf(nItemSprPath,Item[nItemIndx].GetImagePath());
				g_StrLower(nItemSprPath);
				int m_nWidth,m_nHeight,nFrams;
				SPRFRAMSINFO nSprInfo;
				ZeroMemory(&nSprInfo,sizeof(nSprInfo));
				bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
				if (bgCur==NULL)
					return;
			}

		   Sprite * tempSpr = (Sprite *)ParentNode_Team->getChildByName(m_ItemBoxKey);
		   //LayerColor *bgcolorLayer = (LayerColor *)ParentNode_Team->getChildByName(m_colorbgKey);
		   Sprite *bgImage = (Sprite *)ParentNode_Team->getChildByName(m_colorbgKey);
		   char bgImageName[50];
		   Color4B nItemColor;
			int nItemKind=0;
				nItemKind = Item[nItemIndx].getItemKind();

			if  (nItemKind==1){
			  	nItemColor = Color4B(249, 229, 12, 150); //TB HOANG KIM
				t_sprintf(bgImageName,"ui/itembaseGold.png");
			}else if (nItemKind==2){
				nItemColor = Color4B(61, 103, 242, 150);   //TB XANH
				t_sprintf(bgImageName,"ui/itembaseBlue.png");
			}else if (nItemKind==3){
				nItemColor = Color4B(214, 34, 241, 150);//TB TIM
				t_sprintf(bgImageName,"ui/itembasePurple.png");
			}else{
				nItemColor = Color4B(112, 128, 144, 150);
				t_sprintf(bgImageName,"ui/itembaseWhite.png");
			}
		   Size bgsize;
		   bgsize.width  = CtrlItemMap[index].nCellx*itemSize;
		   bgsize.height = CtrlItemMap[index].nCelly*itemSize;
		   Size nTempSize = bgCur->getContentSize();
		   float tempOffsetY = 55;
		   if (!tempSpr)
		   {
				tempSpr =Sprite::createWithTexture(bgCur);
				tempSpr->setPosition(ax::Vec2(0,0));
				tempSpr->setAnchorPoint(ax::Vec2(0,0));
                int nikey = std::hash<std::string>{}(m_ItemBoxKey);
				tempSpr->setTag(nikey);
				if (nItemBoxIdx==equip_horse)
				{
					tempSpr->setScale(0.8);
				}
			    ParentNode_Team->addChild(tempSpr,2);

				if (nItemKind==1){
					//bg dong

					bgImage = Sprite::create(bgImageName);
					bgImage->setAnchorPoint(ax::Vec2(0,0));
					SpriteFrameCache * sfCache = SpriteFrameCache::getInstance();
					sfCache->addSpriteFramesWithFile("res/item/item_2.plist");
					SpriteBatchNode* spriteSheet = SpriteBatchNode::create("res/item/item_2.png");
                    Vector<SpriteFrame*> bgItemFrames;
					for (int i=1;i<=16;i++){

                        char filename[10];
                        t_sprintf(filename,"%d.png",i);
						SpriteFrame* spriteframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
						bgItemFrames.pushBack(spriteframe);
					}
					Animation* runAim = Animation::createWithSpriteFrames(bgItemFrames,0.1);
					Sprite* bgitem = Sprite::createWithSpriteFrameName("1.png");
					bgitem->setAnchorPoint(ax::Vec2(0,0));
					Action* action = RepeatForever::create(Animate::create(runAim));
					bgitem->runAction(action);
					spriteSheet->addChild(bgitem);
					//bg dong
					spriteSheet->setAnchorPoint(ax::Vec2(0,0));
					switch(nItemBoxIdx)
					{
					case equip_armor:
					case equip_boots:
					case equip_belt:
					case equip_helm:
					case equip_cuff:
					case equip_meleeweapon:
					case equip_rangeweapon:
						{//武器
							spriteSheet->setPosition(ax::Vec2(m_size.width/2+CtrlItemMap[index].nOffsetX-3,m_size.height-CtrlItemMap[index].nOffsetY-bgsize.height-tempOffsetY-3));
							spriteSheet->setScale(0.8);
							bgImage->setPosition(ax::Vec2(m_size.width/2+CtrlItemMap[index].nOffsetX-3,m_size.height-CtrlItemMap[index].nOffsetY-bgsize.height-tempOffsetY-3));
						}
						break;
					case equip_amulet:
					case equip_pendant:
					case equip_horse:
					case equip_mask:
					case equip_pifeng:
					case equip_yinjian:
					case equip_shiping:
					case equip_ring://nhẫn
						{
							spriteSheet->setScale(0.55);
							spriteSheet->setPosition(ax::Vec2(m_size.width/2+CtrlItemMap[index].nOffsetX,m_size.height-CtrlItemMap[index].nOffsetY-bgsize.height-tempOffsetY));
							bgImage->setScale(0.7);
							bgImage->setPosition(ax::Vec2(m_size.width/2+CtrlItemMap[index].nOffsetX,m_size.height-CtrlItemMap[index].nOffsetY-bgsize.height-tempOffsetY));
						}
						break;
					default:
						break;
					}
                    int nikey = std::hash<std::string>{}(m_colorbgKey);
					bgImage->setTag(nikey);
					spriteSheet->setTag(nikey);
					ParentNode_Team->addChild(spriteSheet,1);
					ParentNode_Team->addChild(bgImage,0);
				}else{

					bgImage = Sprite::create(bgImageName);
					bgImage->setAnchorPoint(ax::Vec2(0,0));
					switch(nItemBoxIdx)
					{
					case equip_armor:
					case equip_boots:
					case equip_belt:
					case equip_helm:
					case equip_cuff:
					case equip_meleeweapon:
					case equip_rangeweapon:
						{//武器
							bgImage->setPosition(ax::Vec2(m_size.width/2+CtrlItemMap[index].nOffsetX-3,m_size.height-CtrlItemMap[index].nOffsetY-bgsize.height-tempOffsetY-3));
						}
						break;
					case equip_amulet:
					case equip_pendant:
					case equip_horse:
					case equip_mask:
					case equip_pifeng:
					case equip_yinjian:
					case equip_shiping:
					case equip_ring://nhẫn
						{
							bgImage->setScale(0.7);
							bgImage->setPosition(ax::Vec2(m_size.width/2+CtrlItemMap[index].nOffsetX,m_size.height-CtrlItemMap[index].nOffsetY-bgsize.height-tempOffsetY));
						}
						break;
					default:
						break;
					}
                    int nikey = std::hash<std::string>{}(m_colorbgKey);
					bgImage->setTag(nikey);
					ParentNode_Team->addChild(bgImage,1);
				}
			   return;
		   }
		   //tempSpr->setContentSize(nTempSize);
		   Rect nRectZero = ax::Rect(0,0,0,0);
		   nRectZero.size = bgCur->getContentSize();
		   tempSpr->setTexture(bgCur);
		   tempSpr->setTextureRect(nRectZero,false,nRectZero.size);
		   tempSpr->setPosition(ax::Vec2(m_size.width/2+CtrlItemMap[index].nOffsetX+bgsize.width/2-nTempSize.width/2,m_size.height-CtrlItemMap[index].nOffsetY-(bgsize.height+nTempSize.height)/2-tempOffsetY));

		   //开始检测设置背景颜色
		   KUiObjAtContRegion	Obj;
		   Obj.Obj.uGenre = CGOG_ITEM;
		   Obj.Obj.uId    = nItemIndx;
		   Obj.Region.h     = Obj.Region.v = 0;
		   Obj.Region.Width = Obj.Region.Height = 0;
		   Obj.nContainer = UOC_EQUIPTMENT;

		   ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
			   GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (uintptr_t)&Obj, 0);
		   //if (eProp == IIEP_NORMAL) //可用
		   //else
		   if (eProp == IIEP_NOT_USEABLE)  //不可用
		   {
			   ax::Color3B color_(128, 0, 0);
			   if (bgImage){
				//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Item khong su dung");
			   }
		   }

		}
	}
	else
	{//删除
		if (ParentNode_Team)
		{
			// pTemDescItem = Label::createWithTTF("Bán",UI_GAME_FONT_VN, 11);
			// pTemDescItem->setColor(ax::Color3B::WHITE);
			// pTemDescItem->setAnchorPoint(ax::Vec2(0,0));
			// pTemDescItem->enableOutline(Color4B(0, 0, 0, 255), 6);//vi?n ch? ?en
			// pTemDescItem->setPosition(ax::Vec2(0,0));

			m_itemIndexBox[index]=0;
			ParentNode_Team->removeChildByName(m_ItemBoxKey);
			ParentNode_Team->removeChildByName(m_colorbgKey);
		}
	}
}
//--------------------------------------------------------------------------
//	功能：装备变化更新
//--------------------------------------------------------------------------
void KuiRoleState::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (Item && pEquip)
	{
		for (int i = 0; i < _ITEM_COUNT; ++i)
		{
			if (CtrlItemMap[i].nPosition == pEquip->Region.v)
			{//就是这个精灵
				if (bAdd)//增加
					//m_EquipBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
					//pEquip->Region.Width, pEquip->Region.Height);
					//ccMessageBox(Item[pEquip->Obj.uId].GetImagePath(),"增加");
					addpicBox(i,pEquip->Obj.uId,true);
				else//移除
					addpicBox(i,pEquip->Obj.uId,false);
					//ccMessageBox(Item[pEquip->Obj.uId].GetImagePath(),"移除");
					//m_EquipBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);

				break;
			}
		}
	}
}

Rect KuiRoleState::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //这个因该是图内偏移
	//visibleSize
	rc.size       = pNode->getContentSize();
	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //起点最左边
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //最上边    //- rc.origin.y
	//rc.origin     = ccpAdd(rc.origin,diff);
	return rc;
}

//移动结束
void KuiRoleState::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	//if  (nPartNode)
	//	nPartNode->setVisible(true);

	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//移动中
void KuiRoleState::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling||!__isCanMove) return;

	//Touch *touch = (Touch*)pTouches->anyObject();
	//if  (nPartNode)
	//	nPartNode->setVisible(false);
	//__loopTime = MAX_LOOP_TIME;
	//Node *clipper = ParentNode_Team->getChildByTag(1);
	Point point   = ParentNode_Team->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff    = point - m_lastPoint;
	//Node *content = clipper->getChildByTag(2);                   //设置图片的位置
	ParentNode_Team->setPosition((ParentNode_Team->getPosition() + diff));
	m_origin      = ParentNode_Team->getPosition();
	/*nPartNode->setPosition(ccpAdd(nPartNode->getPosition(),diff)); //这个点一起移动
	if  (pPointLabel)
		pPointLabel->setPosition(ccpAdd(pPointLabel->getPosition(),diff)); //这个点一起移动
	if  (BtnFlag)
		BtnFlag->setPosition(ccpAdd(BtnFlag->getPosition(),diff));         //这个点一起移动
	*/
	m_lastPoint = point;
}
//触摸函数ccTouchBegan，返回true
bool KuiRoleState::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point point = ParentNode_Team->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Team->getContentSize().width*/,winSize.height/* ParentNode_Team->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //如果在范围内 就可以拖动
	m_lastPoint = point;

	Point location = pTouch->getLocation();//目的的坐标?屏幕点击处
	Sprite * itemBoxSpr =NULL;
	bool isthis=false;
	for (int i = 0; i < _ITEM_COUNT; ++i)
	{
		char nKey[32];
		t_sprintf(nKey,"%s",CtrlItemMap[i].pIniSection);
		std::string m_ItemBoxKey = nKey;
		t_sprintf(nKey,"%s_color",CtrlItemMap[i].pIniSection);
		std::string m_colorbgKey = nKey;//颜色层的key
		itemBoxSpr = (Sprite *)ParentNode_Team->getChildByName(m_ItemBoxKey);
		LayerColor *colorBoxbg = (LayerColor *)ParentNode_Team->getChildByName(m_colorbgKey);
		//ccMessageBox("点击开始","点击开始");
		if  (itemBoxSpr && colorBoxbg)
		{
			if (getRect(colorBoxbg,i+1).containsPoint(location))
			{//点中了
				//restartAction();
				itemBoxSpr->runAction(red);
				//ax::Color3B color_(128, 0, 0);
				//if (colorBoxbg)
					//colorBoxbg->setColor(color_);
				KUiItemdObject second;
				second.uId    = m_itemIndexBox[i];
				second.uGenre = CGOG_ITEM;
				second.nRect  = getRect(colorBoxbg,0);
				second.nGenkind  =  UOC_EQUIPTMENT;
				Layer * sDescLayer = KuiItemdesc::create(second,this,CC_CALLBACK_1(KuiRoleState::closedescPadCallback, this),2);//2 ->show button with touch in role stage
				m_descPadIsOpen   = true;
				m_CurSelItemIndex = m_itemIndexBox[i];//m_ItemData[i].uId;
				//nstrCurSelItemKey =nItemKey;
				isthis = true;
				this->addChild(sDescLayer,2,1001);
			}
			else
			{
				if (!isthis)
				  m_CurSelItemIndex =0;

				itemBoxSpr->setColor(ax::Color3B::WHITE);           //默认颜色
			}
		}
	}
	return true;
}

void KuiRoleState::closedescPadCallback(Node *pNode)
{
	m_descPadIsOpen = false;
}


void KuiRoleState::UpdateBaseData()
{
	KUiPlayerBaseInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
	if (g_pCoreShell)
	    g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (intptr_t)&Info, 0);

	pRoleName->setString(Info.Name);
	pRoleTitle->setString(Info.Title);//Info.Title
	pRoleTitle_1->setString(UTEXT(Info.Agname,1).c_str());
	//m_Agname.SetText(Info.Agname);  //绰号
	//m_Name  .SetText(Info.Name);    //名字
	//m_Title .SetText(Info.Title);   //称号
}

void KuiRoleState::BreatheUpdateInfo()
{
	KUiPlayerRuntimeInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
	if (g_pCoreShell)
	    g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (intptr_t)&Info, 0);

	char nTempS4tr[64];
	ZeroMemory(nTempS4tr,sizeof(nTempS4tr));
	t_sprintf(nTempS4tr,"%d/%d",Info.nLife,Info.nLifeFull);
	pLife->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d/%d",Info.nMana,Info.nManaFull);
	pMana->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d/%d",Info.nStamina,Info.nStaminaFull);
	pTili->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d/%d",Info.nExperience,Info.nExperienceFull);
	pExp->setString(nTempS4tr);

	//UiNewStatus* pStatus = UiNewStatus::GetIfVisible();
	//if (pStatus)
	//	UpdateRuntimeInfo(&Info);
	//pStatus=NULL;
}


void  KuiRoleState::BreathAttributeUpdate()
{
	KUiPlayerAttribute	Info;
	memset(&Info, 0, sizeof(KUiPlayerAttribute));
	if (g_pCoreShell)
	   g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (uintptr_t)&Info, 0);

	char nTempS4tr[64];
	ZeroMemory(nTempS4tr,sizeof(nTempS4tr));
	t_sprintf(nTempS4tr,"%d",Info.nAttack);//tấn công/chính xác
	pMingZhong->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d/%d",Info.nAttackSpeed,Info.nCastSpeed);//外/内 攻击速度
	pActpeed->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nDefence);   //né tránh
	pShanBi->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nDexterity); // Thân pháp
	pShenFa->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nEnergy);    // Nội công
	pNeiGong->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nStrength);  //sức mạnh
	pLiLiang->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nVitality);  //ngoại công
	pWauGong->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nFuYuan);  //Phục hồi
	pFuYuan->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nRepute);  // Danh vọng
	pShenWang->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nLevel);  //cấp
	proelLevel->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%s",Info.StatusDesc);  //hệ
	pSex->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nBARemainPoint);  //tiềm năng còn lại
	__RemainPoint = Info.nBARemainPoint;
	pMicCount->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nMoveSpeed);    //tốc độ chạy
	pRunSpeed->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d",Info.nRankInWorld);  //xếp hạng
	pPaiMing->setString(nTempS4tr);

	int nTempVal =Player[CLIENT_PLAYER_INDEX].m_cVip.GetRoleVipNum()*Player[CLIENT_PLAYER_INDEX].m_cVip.GetRoleVipDamageMaxVal();


	KNpc*	pNpc = &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex];
	switch(pNpc->m_Series)
	{
	case series_water:
		pSex->setColor(ax::Color3B::BLUE);
		break;
	case series_wood:
		pSex->setColor(Color3B::GREEN);
		break;
	case series_metal:
		pSex->setColor(Color3B::YELLOW);
		break;
	case series_fire:
		pSex->setColor(Color3B::ORANGE);
		break;
	case series_earth:
		pSex->setColor(Color3B::GRAY);
		break;
	}


	t_sprintf(nTempS4tr,"%d/%d(%d)",Info.nKillMIN,Info.nKillMAX,nTempVal);            //左攻击
	pLeftAct->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%d/%d(%d)",Info.nRightKillMin,Info.nRightKillMax,nTempVal);  //左攻击
	pRigthAct->setString(nTempS4tr);
	if (pRoleVip)
	{
	   _VipLevel = Player[CLIENT_PLAYER_INDEX].m_cVip.GetRoleVipNum();
	   char nTempInfo[64];
	   ZeroMemory(nTempInfo,sizeof(nTempInfo));
	   t_sprintf(nTempInfo,"VIP:%d",_VipLevel);
	   pRoleVip->setString(UTEXT(nTempInfo,1).c_str());
	   if (_VipLevel>0)
          pRoleVip->setVisible(true);
	}

	t_sprintf(nTempS4tr,"%s",Info.nPhyDef);
	pPuFang->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%s",Info.nCoolDef);
	pBingFang->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%s",Info.nFireDef);
	pHuoFang->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%s",Info.nLightDef);
	pLeiFang->setString(nTempS4tr);
	t_sprintf(nTempS4tr,"%s",Info.nPoisonDef);
	pDuFang->setString(nTempS4tr);

	t_sprintf(nTempS4tr,"%d",Info.nPKValue);
	pPk->setString(nTempS4tr);

	t_sprintf(nTempS4tr,"%d",Info.nReBorn);
	pZhuanSheng->setString(UTEXT(nTempS4tr,1).c_str());

	//t_sprintf(nTempS4tr,"转生次数:%d",Info.);
	//pZhuanSheng->setString(UTEXT(nTempS4tr,1).c_str());

}
void KuiRoleState::update(float delta)
{
	if (isOpen && g_pCoreShell)
	{
	   UpdateAllEquips();
	   BreatheUpdateInfo();
	   BreathAttributeUpdate();

	   if (nIsOpenTxt && m_ExeProcessSprite)
	   {
		   int nCurExP_z;
		   g_pCoreShell->GetGameData(GDI_LAOHUJI_INFO,19,0);
		   nVipInfo.GetInteger(_VipLevel+2,"LevelExp",0,&nCurExP_z); //19
		   if  (nCurExP_z<=0)
			   nCurExP_z=1000000;
		   int nCurExp=Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(19);
		   float ver=(1.00*nCurExp)/nCurExP_z;
		   if (ver>=0 && ver<=1)
			   m_ExeProcessSprite->setScaleX(ver);
		   else
			   m_ExeProcessSprite->setScaleX(1);
	   }
	}
}
