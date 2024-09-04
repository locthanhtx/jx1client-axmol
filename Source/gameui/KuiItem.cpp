#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiItem.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
//#include "engine/KPalette.h"
#include "engine/KCodec.h"
#include "engine/KCodecLzo.h"
#include "../engine/KFile.h"
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
#include "gamecore/KPlayer.h"
#include "gameui/KuiItemdesc.h"
#include "gameui/KuigetMoney.h"
#include "engine/Text.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

KuiItem::KuiItem()
{
	__isCanMove  = true;
	m_bScrolling = false;
	isOpen = false;
//	memset(m_ItemData,0,sizeof(KUiItemdObject) * 60);
	pMoneyLabel       = NULL;
	pXuLabel          = NULL;
	colorLayer        = NULL;
	//red               = NULL;
	m_ItemData.clear();
	m_nMoney = 0;
	m_nXu = 0;
	m_nNumObjects = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	nBakChickTime=0;
	m_descPadIsOpen = false;
	m_EqdescPadIsOpen = false;
	m_TempPadIsOpen=false;
	savemenu = NULL;
	nstrCurSelItemKey ="";
	m_MovePosX  = 0;
	m_nIstrue   =false;
}

KuiItem::~KuiItem()
{
	isOpen = false;
	pMoneyLabel  = NULL;
	colorLayer        = NULL;
	m_ItemData.clear();
	//if (red)
	//   red->release();
	m_descPadIsOpen = false;
	m_EqdescPadIsOpen = false;
	m_savemoneyPadIsOpen = false;
	m_TempPadIsOpen=false;
}

//创建确定弹出框
KuiItem * KuiItem::create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiItem * popLayer = KuiItem::create();
	//popLayer->setTitle(title);
	popLayer->addDialogData();
	//设置按钮
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//初始化
bool KuiItem::init()
{
	if ( !Layer::init() ){
		return false;
	}

	winSize = ax::Director::getInstance()->getWinSize();

	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	Sprite * spriteBox = Sprite::create("ui/item.png");
	Sprite * spriteBoxMove = Sprite::create("ui/itembg.png");
	spriteBox->setContentSize(Size(335,500));
	spriteBoxMove->setContentSize(Size(ITEM_CELL_SIZE,ITEM_CELL_SIZE));
	ParentNode_Item = spriteBox;
	ParentNode_ItemMove = spriteBoxMove;
	spriteBox->setPosition(ax::Vec2(winSize.width/2+330,winSize.height/2));
	this->addChild(spriteBoxMove);
	this->addChild(spriteBox);

	//获得背景图片的大小
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();      //背景的相对整个屏幕的位置

	this->scheduleUpdate();                   //调用函数
	auto listener = ax::EventListenerTouchOneByOne::create();
ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this); //禁止多点触摸
	return true;
}

void KuiItem::addDialogData()
{
	if  (!g_pCoreShell) return;

	//Left=24
	//Top=72
	m_StartPos.x =31;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =53;//+ITEM_CELL_SIZE/2;

    pMoneyLabel = Label::createWithTTF("",UI_GAME_FONT_VN, 12);//Arial
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	pMoneyLabel->setPosition(ax::Vec2(65,70));
	pMoneyLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
	ParentNode_Item->addChild(pMoneyLabel,1);

	pXuLabel = Label::createWithTTF("",UI_GAME_FONT_VN, 12);//Arial
	pXuLabel->setAnchorPoint(ax::Vec2(0,0));
	pXuLabel->setPosition(ax::Vec2(65,50));
	ParentNode_Item->addChild(pXuLabel,1);


	KUiObjAtRegion* pObjs = NULL;
	int nCount   = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 0); //得到随身带物品的数量

	m_nNumObjects = nCount;

	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, (uintptr_t)pObjs, nCount);//单线程执行，nCount值不变
		for (int i = 0; i < nCount; ++i)
		{
			KUiDraggedObject no;
			no.uGenre = pObjs[i].Obj.uGenre;
			no.uId    = pObjs[i].Obj.uId;
			no.DataX  = pObjs[i].Region.h;     //包袱中的起点X位置 第几个格子 25x25
			no.DataY  = pObjs[i].Region.v;     //包袱中的起点Y位置
			no.DataW  = pObjs[i].Region.Width; //物品的高度(占用了多少个格子)
			no.DataH  = pObjs[i].Region.Height;//物品的宽度(占用了多少个格子)
			if (Item && pObjs[i].Obj.uId>0 && pObjs[i].Obj.uId<MAX_ITEM)
			{

				char nItemSprPath[256];
				ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
				if  (Item[pObjs[i].Obj.uId].GetImagePath())
					t_sprintf(nItemSprPath,Item[pObjs[i].Obj.uId].GetImagePath());
				else
				   t_sprintf(nItemSprPath,"\\spr\\others\\问号.spr");

				if  (nItemSprPath[0])
				{
					g_StrLower(nItemSprPath);
					int m_nWidth,m_nHeight,nFrams;
					Texture2D *bgCur = NULL;
					SPRFRAMSINFO nSprInfo;
					ZeroMemory(&nSprInfo,sizeof(nSprInfo));
					bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
					if (bgCur==NULL)
						return;
					char nItemKey[32];
					t_sprintf(nItemKey,"item_%u",Item[pObjs[i].Obj.uId].GetID());
					std::string nKey =nItemKey;
					Sprite *nItemSpr = (Sprite *)ParentNode_Item->getChildByName(nKey);
					if (!nItemSpr)
					{
						nItemSpr = Sprite::createWithTexture(bgCur);
                        int nikey = std::hash<std::string>{}(nKey);
						nItemSpr->setTag(nikey);
						ParentNode_Item->addChild(nItemSpr,1);
						ParentNode_ItemMove->addChild(nItemSpr,1);

						// if (Item[pObjs[i].Obj.uId].GetGenre()==item_equip){
						// 	nItemSpr->setScale(0.8);
						// }
						//Kientm item 1 o vuong nen can giam scale cua spr item
						if (Item[pObjs[i].Obj.uId].GetDetailType()==equip_armor || Item[pObjs[i].Obj.uId].GetDetailType()==equip_amulet){
							nItemSpr->setScale(0.7);
						}
						if (Item[pObjs[i].Obj.uId].GetDetailType()==equip_horse){
							nItemSpr->setScale(0.6);
						}
						if (Item[pObjs[i].Obj.uId].GetGenre()==item_medicine){
							nItemSpr->setScale(1);
						}
						if (Item[pObjs[i].Obj.uId].GetDetailType()==equip_meleeweapon || Item[pObjs[i].Obj.uId].GetDetailType()==equip_rangeweapon){
							nItemSpr->setScale(0.65);
						}

					}
					//开始填充书库
					m_ItemData[nKey].uGenre = pObjs[i].Obj.uGenre;
					m_ItemData[nKey].uId    = pObjs[i].Obj.uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObjs[i].Region.h;     //包袱中的起点X位置 第几个格子 25x25
					m_ItemData[nKey].DataY  = pObjs[i].Region.v;     //包袱中的起点Y位置
					m_ItemData[nKey].DataW  = pObjs[i].Region.Width; //物品的高度(占用了多少个格子)
					m_ItemData[nKey].DataH  = pObjs[i].Region.Height;//物品的宽度(占用了多少个格子)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //随身携带

					Color4B color(0, 56, 33, 150); //112, 128, 144
					//t_sprintf(nItemKey,"color_%d",i+1);
					strcat(nItemKey,"_color");
					nKey = nItemKey;
					LayerColor *bgcolorLayer = (LayerColor *)ParentNode_Item->getChildByName(nKey);
					if (!bgcolorLayer)
					{
						bgcolorLayer = LayerColor::create(color);//颜色层
					    //bgcolorLayer->setPosition(ax::Vec2::ZERO);
						bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
						Size bgsize;
						bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
						bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					    bgcolorLayer->setContentSize(bgsize);//设置和窗口的大小
                        int nikey = std::hash<std::string>{}(nKey);
						bgcolorLayer->setTag(nikey);
						if  (Item[pObjs[i].Obj.uId].GetGenre()!=item_equip && Item[pObjs[i].Obj.uId].IsStack())
						{//item xep chong
							char stack[32];
							t_sprintf(stack,"%d",Item[pObjs[i].Obj.uId].GetStackNum());
							Label *stuckCountlabel = Label::createWithTTF(stack,UI_GAME_FONT_VN,14);
							stuckCountlabel->setColor(ax::Color3B::YELLOW);
							stuckCountlabel->setAnchorPoint(ax::Vec2(0,0));
							stuckCountlabel->enableOutline(Color4B(0, 0, 0, 255), 5);
							stuckCountlabel->setPosition(ax::Vec2(3,0));
                            int nikey = std::hash<std::string>{}("stack");
							stuckCountlabel->setTag(nikey);
							bgcolorLayer->addChild(stuckCountlabel,1);
						}
						ParentNode_Item->addChild(bgcolorLayer,0);
					}

					int nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
					int nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;

					switch (no.DataX)
					{
					case 0:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2)+2;

						break;
					case 1:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2)+8;
						break;
					case 2:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2)+14;
						break;
					case 3:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2)+20;
						break;
					case 4:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2)+26;
						break;
					default:
						break;
					}

					switch (no.DataY)
					{
					case 0:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+2;
						break;
					case 1:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+8;
						break;
					case 2:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+14;
						break;
					case 3:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+20;
						break;
					case 4:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+26;
						break;
					case 5:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+32;
						break;
					default:
						break;
					}
					nItemSpr->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));
					//颜色层
					switch (no.DataX)
					{
					case 0:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE)+2;
						break;
					case 1:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE)+8;
						break;
					case 2:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE)+14;
						break;
					case 3:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE)+20;
						break;
					case 4:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE)+26;
						break;
					default:
						break;
					}

					switch (no.DataY)
					{
					case 0:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+2;
						break;
					case 1:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+8;
						break;
					case 2:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+14;
						break;
					case 3:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+20;
						break;
					case 4:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+26;
						break;
					case 5:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+32;
						break;
					default:
						break;
					}
					bgcolorLayer->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));

					KUiObjAtContRegion	Obj;
					Obj.Obj.uGenre = no.uGenre;
					Obj.Obj.uId    = no.uId;
					Obj.Region.h   = Obj.Region.v = 0;
					Obj.Region.Width = Obj.Region.Height = 0;
					Obj.nContainer = UOC_ITEM_TAKE_WITH;

					ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					    GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (uintptr_t)&Obj, 0);
					//if (eProp == IIEP_NORMAL) //可用
					//else
					if (eProp == IIEP_NOT_USEABLE)  //不可用
					{
						ax::Color3B color_(128, 0, 0);
						bgcolorLayer->setColor(color_);
					}
					//else if (eProp == IIEP_SPECIAL) //其他特殊物品
				}
			}
		}
		free(pObjs); //释放内存
		pObjs = NULL; //清空
	}


	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//颜色层
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);//设置和窗口的大小
	ParentNode_Item->addChild(colorLayer,1000);
	//创建一个动作 改变精灵颜色
	//red = TintBy::create(0.2,0,-255,-255);//持续时间+颜色
	//red->retain();
}

//设置确定取消按钮,参数：调用层对象，调用层回调函数
void KuiItem::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;


	Point bgSpritePoint = ParentNode_Item->getPosition();

	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	Point nStartPos;
	nStartPos.x = m_size.width/2+20;
	nStartPos.y = m_size.height/2;
	//dong
	MenuItemImage * closeConfirm = MenuItemImage::create("", "", CC_CALLBACK_1(KuiItem::buttonCallBackFunc, this));
	closeConfirm->setContentSize(Size(125,30));
	closeConfirm->setPosition(ax::Vec2(235,-128));
	//sap xep
	// MenuItemImage * zhengliConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::zhengCallBackFunc, this));
	// zhengliConfirm->setPosition(ax::Vec2(0,25));
	// Label * zhengliConfirmLabel = Label::createWithTTF("Sắp xếp",UI_GAME_FONT_VN, 11);
	// zhengliConfirmLabel->setColor(ax::Color3B::WHITE);
	// zhengliConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// zhengliConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// zhengliConfirmLabel->setPosition(ax::Vec2(zhengliConfirm->getContentSize().width/2-20,zhengliConfirm->getContentSize().height/2-7));
	// zhengliConfirm->addChild(zhengliConfirmLabel,1);
	//them vao thanh phim tat
	// MenuItemImage * kuaiConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::kuaiCallBackFunc, this));
	// kuaiConfirm->setPosition(ax::Vec2(0,50));
	// Label * kuaiConfirmLabel = Label::createWithTTF("Phím tắt",UI_GAME_FONT_VN, 11);
	// kuaiConfirmLabel->setColor(ax::Color3B::WHITE);
	// kuaiConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// kuaiConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// kuaiConfirmLabel->setPosition(ax::Vec2(kuaiConfirm->getContentSize().width/2-20,kuaiConfirm->getContentSize().height/2-7));
	// kuaiConfirm->addChild(kuaiConfirmLabel,1);

	//用
	// MenuItemImage * itemchuanConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::chuanCallBackFunc, this));
	// itemchuanConfirm->setPosition(ax::Vec2(0,75));
	// Label * itemchuanConfirmLabel = Label::createWithTTF("Dùng",UI_GAME_FONT_VN, 11);
	// itemchuanConfirmLabel->setColor(ax::Color3B::WHITE);
	// itemchuanConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// itemchuanConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// itemchuanConfirmLabel->setPosition(ax::Vec2(itemchuanConfirm->getContentSize().width/2-20,itemchuanConfirm->getContentSize().height/2-7));
	// itemchuanConfirm->addChild(itemchuanConfirmLabel,1);

	//拆
	// MenuItemImage * itemchaiConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::chaiCallBackFunc, this));
	// itemchaiConfirm->setPosition(ax::Vec2(0,100));
	// Label * itemchaiConfirmLabel = Label::createWithTTF("Tách",UI_GAME_FONT_VN, 11);
	// itemchaiConfirmLabel->setColor(ax::Color3B::WHITE);
	// itemchaiConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// itemchaiConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// itemchaiConfirmLabel->setPosition(ax::Vec2(itemchaiConfirm->getContentSize().width/2-20,itemchaiConfirm->getContentSize().height/2-7));
	// itemchaiConfirm->addChild(itemchaiConfirmLabel,1);
	//丢
	// MenuItemImage * itemdiuConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::diuCallBackFunc, this));
	// itemdiuConfirm->setPosition(ax::Vec2(0,125));
	// Label * itemdiuConfirmLabel = Label::createWithTTF("Vứt",UI_GAME_FONT_VN, 11);
	// itemdiuConfirmLabel->setColor(ax::Color3B::WHITE);
	// itemdiuConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// itemdiuConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// itemdiuConfirmLabel->setPosition(ax::Vec2(itemdiuConfirm->getContentSize().width/2-20,itemdiuConfirm->getContentSize().height/2-7));
	// itemdiuConfirm->addChild(itemdiuConfirmLabel,1);
	//修理
	// MenuItemImage * itemxiuConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::xiuCallBackFunc, this));
	// itemxiuConfirm->setPosition(ax::Vec2(0,150));
	// Label * itemxiuConfirmLabel = Label::createWithTTF("Sửa",UI_GAME_FONT_VN, 11);
	// itemxiuConfirmLabel->setColor(ax::Color3B::WHITE);
	// itemxiuConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// itemxiuConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// itemxiuConfirmLabel->setPosition(ax::Vec2(itemxiuConfirm->getContentSize().width/2-20,itemxiuConfirm->getContentSize().height/2-7));
	// itemxiuConfirm->addChild(itemxiuConfirmLabel,1);
	//卖
	// MenuItemImage * itemmaiConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::maiCallBackFunc, this));
	// itemmaiConfirm->setPosition(ax::Vec2(0,175));
	// Label * itemmaiConfirmLabel = Label::createWithTTF("Bán",UI_GAME_FONT_VN, 11);
	// itemmaiConfirmLabel->setColor(ax::Color3B::WHITE);
	// itemmaiConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// itemmaiConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// itemmaiConfirmLabel->setPosition(ax::Vec2(itemmaiConfirm->getContentSize().width/2-20,itemmaiConfirm->getContentSize().height/2-7));
	// itemmaiConfirm->addChild(itemmaiConfirmLabel,1);
	//存
	// MenuItemSprite * itemcunConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::cunCallBackFunc, this));
	// itemcunConfirm->setPosition(ax::Vec2(0,200));
	// Label * itemcunConfirmLabel = Label::createWithTTF("Rương",UI_GAME_FONT_VN, 11);
	// itemcunConfirmLabel->setColor(ax::Color3B::WHITE);
	// itemcunConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// itemcunConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// itemcunConfirmLabel->setPosition(ax::Vec2(itemcunConfirm->getContentSize().width/2-20,itemcunConfirm->getContentSize().height/2-7));
	// itemcunConfirm->addChild(itemcunConfirmLabel,1);

	//链接
	// MenuItemImage * itemLianConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::lianCallBackFunc, this));
	// itemLianConfirm->setPosition(ax::Vec2(0,225));
	// Label * itemLianConfirmLabel = Label::createWithTTF("Rao",UI_GAME_FONT_VN, 11);
	// itemLianConfirmLabel->setColor(ax::Color3B::WHITE);
	// itemLianConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// itemLianConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// itemLianConfirmLabel->setPosition(ax::Vec2(itemLianConfirm->getContentSize().width/2-20,itemLianConfirm->getContentSize().height/2-7));
	// itemLianConfirm->addChild(itemLianConfirmLabel,1);
	//存钱
	// MenuItemImage * saveConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::saveCallBackFunc, this));
	// saveConfirm->setPosition(ax::Vec2(0,250));
	// Label * saveConfirmLabel = Label::createWithTTF("Gửi",UI_GAME_FONT_VN, 11);
	// saveConfirmLabel->setColor(ax::Color3B::WHITE);
	// saveConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// saveConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// saveConfirmLabel->setPosition(ax::Vec2(saveConfirm->getContentSize().width/2-20,saveConfirm->getContentSize().height/2-7));
	// saveConfirm->addChild(saveConfirmLabel,1);
	//Menu * menu = Menu::create(closeConfirm,itemchuanConfirm,itemdiuConfirm,itemxiuConfirm,itemmaiConfirm,itemchaiConfirm,itemcunConfirm,saveConfirm,kuaiConfirm,zhengliConfirm,itemLianConfirm,NULL);
	Menu * menu = Menu::create(closeConfirm,NULL);
	menu->setPosition(ax::Vec2(-65,150));//V? trí menu
	ParentNode_Item->addChild(menu);         //thêm vào node ParentNode_Item
	menu->setVisible(true);//


	//储
	MenuItemImage * itemchuConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::chuCallBackFunc, this));
	itemchuConfirm->setPosition(ax::Vec2(100,0));
	Label * itemchuConfirmLabel = Label::createWithTTF("Cửa hàng",UI_GAME_FONT_VN, 11);
	itemchuConfirmLabel->setColor(ax::Color3B::WHITE);
	itemchuConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	itemchuConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	itemchuConfirmLabel->setPosition(ax::Vec2(closeConfirm->getContentSize().width/2-20,closeConfirm->getContentSize().height/2-7));
	itemchuConfirm->addChild(itemchuConfirmLabel,1);
	//子
	MenuItemImage * itemziConfirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::ziCallBackFunc, this));
	itemziConfirm->setPosition(ax::Vec2(100,25));
	Label * itemziConfirmLabel = Label::createWithTTF("Button 2",UI_GAME_FONT_VN, 11);
	itemziConfirmLabel->setColor(ax::Color3B::WHITE);
	itemziConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	itemziConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	itemziConfirmLabel->setPosition(ax::Vec2(closeConfirm->getContentSize().width/2-20,closeConfirm->getContentSize().height/2-7));
	itemziConfirm->addChild(itemziConfirmLabel,1);

	//扩1
	MenuItemImage * itemkuo1Confirm =  MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::kuo1CallBackFunc, this));
	itemkuo1Confirm->setPosition(ax::Vec2(100,50));
	Label * itemkuo1ConfirmLabel = Label::createWithTTF("Rương 1",UI_GAME_FONT_VN, 11);
	itemkuo1ConfirmLabel->setColor(ax::Color3B::WHITE);
	itemkuo1ConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	itemkuo1ConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	itemkuo1ConfirmLabel->setPosition(ax::Vec2(closeConfirm->getContentSize().width/2-20,closeConfirm->getContentSize().height/2-7));
	itemkuo1Confirm->addChild(itemkuo1ConfirmLabel,1);

	//扩2
	MenuItemImage * itemkuo2Confirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::kuo2CallBackFunc, this));
	itemkuo2Confirm->setPosition(ax::Vec2(100,75));
	Label * itemkuo2ConfirmLabel = Label::createWithTTF("Rương2",UI_GAME_FONT_VN, 11);
	itemkuo2ConfirmLabel->setColor(ax::Color3B::WHITE);
	itemkuo2ConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	itemkuo2ConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	itemkuo2ConfirmLabel->setPosition(ax::Vec2(closeConfirm->getContentSize().width/2-20,closeConfirm->getContentSize().height/2-7));
	itemkuo2Confirm->addChild(itemkuo2ConfirmLabel,1);

	//扩3
	MenuItemImage * itemkuo3Confirm = MenuItemImage::create("ui/textbg_1.png", "ui/textbg_2.png", CC_CALLBACK_1(KuiItem::kuo3CallBackFunc, this));
	itemkuo3Confirm->setPosition(ax::Vec2(100,75));
	Label * itemkuo3ConfirmLabel = Label::createWithTTF("Rương 3",UI_GAME_FONT_VN, 11);
	itemkuo3ConfirmLabel->setColor(ax::Color3B::WHITE);
	itemkuo3ConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	itemkuo3ConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	itemkuo3ConfirmLabel->setPosition(ax::Vec2(closeConfirm->getContentSize().width/2-20,closeConfirm->getContentSize().height/2-7));
	itemkuo3Confirm->addChild(itemkuo3ConfirmLabel,1);

	savemenu = Menu::create(itemchuConfirm,itemziConfirm,itemkuo1Confirm,itemkuo2Confirm,itemkuo3Confirm,NULL);
	savemenu->setPosition(ax::Vec2(300,150));
	ParentNode_Item->addChild(savemenu);
	savemenu->setVisible(false);//
}
//存钱
void KuiItem::saveCallBackFunc(Ref * pSender)
{
	if (m_savemoneyPadIsOpen)
		this->removeChildByTag(1003,true);
	//1为存钱　０　为取钱
	Layer * savemonyLayer = KuigetMoney::create(0,m_nMoney,1,this,CC_CALLBACK_1(KuiItem::closesavemonyPadCallback, this));
	m_savemoneyPadIsOpen = true;
	this->addChild(savemonyLayer,2,1003);
}
//取钱回调
void KuiItem::closesavemonyPadCallback(Node *pNode)
{
	//int nMaxMoney = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetsMon(room_repository);
	m_savemoneyPadIsOpen = false;
}
//使用
void KuiItem::chuanCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng chọn vật phẩm");
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
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Bạn thao tác quá nhanh");
		return;
	}
	nBakChickTime = nCurChicktime;


	if  (m_ItemData[nstrCurSelItemKey].uId>0 && m_ItemData[nstrCurSelItemKey].uId<MAX_ITEM)
	{
		if (Item[m_ItemData[nstrCurSelItemKey].uId].GetGenre()==item_equip && g_pCoreShell->GetAutoplayid())
		{
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng tắt chức năng tự động đánh trước");
			return;
		}
	}
	else
		return;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if  (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_USE_ITEM,(uintptr_t)(&Obj),UOC_ITEM_TAKE_WITH);
}

void KuiItem::diuCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}
	/*if (m_CurSelItemIndex<0)
	{
		ccMessageBox("请选择一个物品","提示:");
		return;
	}*/
	if  (nstrCurSelItemKey=="" || m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}

	if  (Item[m_ItemData[nstrCurSelItemKey].uId].GetTime()->bYear>0 || Item[m_ItemData[nstrCurSelItemKey].uId].GetIsBang())
	{
		nstrCurSelItemKey="";
#ifdef WIN32
		ccMessageBox("限时/邦定物品禁止丢弃","提示:");
#else
		ccMessageBox(UTEXT("限时/邦定物品禁止丢弃",1).c_str(),UTEXT("提示:",1).c_str());
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

	bool isYes = false;
	if (g_pCoreShell && g_pCoreShell->GetAutoplayid())
		isYes = true;
	Player[CLIENT_PLAYER_INDEX].AutoAwayItem(Item[m_ItemData[nstrCurSelItemKey].uId].GetID(),isYes);
	nstrCurSelItemKey="";
	//char msg[64];
	//t_sprintf(msg,"丢弃:%d",m_CurSelItemIndex);
	//ccMessageBox(msg,"提示");
}


void KuiItem::lianCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}
	/*if (m_CurSelItemIndex<0)
	{
		ccMessageBox("请选择一个物品","提示:");
		return;
	}*/
	if  (nstrCurSelItemKey=="" || m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}

	if  (Item[m_ItemData[nstrCurSelItemKey].uId].GetTime()->bYear>0 || Item[m_ItemData[nstrCurSelItemKey].uId].GetIsBang())
	{
		nstrCurSelItemKey="";
#ifdef WIN32
		ccMessageBox("限时/邦定物品禁止出售","提示:");
#else
		ccMessageBox(UTEXT("限时/邦定物品禁止出售",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}

	if  (g_GameWorld)
	{//开始输入聊天信息框
		char nTempStrVer[80];
		ZeroMemory(nTempStrVer,sizeof(nTempStrVer));
		t_sprintf(nTempStrVer,"<%s>",Item[m_ItemData[nstrCurSelItemKey].uId].GetName());
		g_GameWorld->setInputMsg(nTempStrVer);
		Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid = Item[m_ItemData[nstrCurSelItemKey].uId].GetID();
	}

}

void KuiItem::cunCallBackFunc(Ref * pSender)
{
	if (savemenu)
	{
		if (savemenu->isVisible())
			savemenu->setVisible(false);//隐藏
		else
			savemenu->setVisible(true);//显示
	}

}

void KuiItem::xiuCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}

	if  (m_ItemData[nstrCurSelItemKey].uId>0 && m_ItemData[nstrCurSelItemKey].uId<MAX_ITEM)
	{
		if (Item[m_ItemData[nstrCurSelItemKey].uId].GetGenre()!=item_equip)
		{
#ifdef WIN32
			ccMessageBox("该物品不能修理!","提示:");
#else
			ccMessageBox(UTEXT("该物品不能修理!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
			return;
		}
	}
	else
		return;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;

	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_REPAIR,(uintptr_t)(&Obj),0);
}
void KuiItem::maiCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}


	if  (nstrCurSelItemKey=="" || m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}
	/*KUiPlayerGameTime	pInfo;
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
	*/
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	nstrCurSelItemKey="";
	if (g_pCoreShell)
		//g_pCoreShell->OperationRequest(GOI_TRADE_PLAYER_BUY,(uintptr_t)(&Obj), m_uId);
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,(uintptr_t)(&Obj),0);
}
void KuiItem::chaiCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}


	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}

	if  (m_ItemData[nstrCurSelItemKey].uId>0 && m_ItemData[nstrCurSelItemKey].uId<MAX_ITEM)
	{
		if (Item[m_ItemData[nstrCurSelItemKey].uId].GetGenre()==0)
		{
#ifdef WIN32
		ccMessageBox("该物品不能拆分","提示:");
#else
		ccMessageBox(UTEXT("该物品不能拆分",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		    return;
		}
	}
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;

	if (g_pCoreShell)
	{
		if (g_pCoreShell->OperationRequest(GOI_NPC_ITEM_BREAK,(uintptr_t)(&Obj), 1))
		{//拆分成功了,设置显示
			char nKey[32];
			t_sprintf(nKey,"item_%u",Item[Obj.Obj.uId].GetID());
			std::string nItemKey = nKey;
			Sprite *nItemSpr =(Sprite *)ParentNode_Item->getChildByName(nItemKey);
			if (nItemSpr)
			{
				Label *stacklabel =(Label *)nItemSpr->getChildByName("stack");
				if (stacklabel)
				{
					int nstackCount = Item[Obj.Obj.uId].GetStackNum();
					t_sprintf(nKey,"%d",nstackCount);
					stacklabel->setString(nKey);
				}
			}
		}
	}
}
//储物箱
void KuiItem::chuCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}

	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
			ccMessageBox("该物品不能储存","提示:");
#else
			ccMessageBox(UTEXT("该物品不能储存",1).c_str(),UTEXT("提示:",1).c_str());
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
		//nBakChickTime = nCurChicktime;
/*#ifdef WIN32
		ccMessageBox("移动的太频繁了","提示:");
#else
		ccMessageBox(UTEXT("移动的太频繁了",1).c_str(),UTEXT("提示:",1).c_str());
#endif*/
		return;
	}
	nBakChickTime = nCurChicktime;
		KUiObjAtContRegion	Obj;
		Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
		Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
		Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
		Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
		Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
		Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
		Obj.eContainer    = UOC_ITEM_TAKE_WITH;
		if (g_pCoreShell)
		   g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_repository);
}

void KuiItem::zhengCallBackFunc(Ref * pSender)
{
	if (!g_pCoreShell) return;

	if (g_pCoreShell->GetAutoplayid())
	{
#ifdef WIN32
		ccMessageBox("挂机中,禁止整理!","提示");
#else
		ccMessageBox(UTEXT("挂机中,禁止整理!",1).c_str(),UTEXT("提示",1).c_str());
#endif
		return;
	}

	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}
	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		ccMessageBox("该物品不能整理","提示:");
#else
		ccMessageBox(UTEXT("该物品不能整理",1).c_str(),UTEXT("提示:",1).c_str());
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
		//nBakChickTime = nCurChicktime;
#ifdef WIN32
		ccMessageBox("整理的太频繁了","提示:");
#else
		ccMessageBox(UTEXT("整理的太频繁了",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}

	nBakChickTime = nCurChicktime;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_equipment);
}

//放快捷栏
void KuiItem::kuaiCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}
	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		ccMessageBox("该物品不能移动","提示:");
#else
		ccMessageBox(UTEXT("该物品不能移动",1).c_str(),UTEXT("提示:",1).c_str());
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
		//nBakChickTime = nCurChicktime;
/*#ifdef WIN32
		ccMessageBox("移动的太频繁了","提示:");
#else
		ccMessageBox(UTEXT("移动的太频繁了",1).c_str(),UTEXT("提示:",1).c_str());
#endif*/
		return;
	}
	nBakChickTime = nCurChicktime;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_immediacy);
}
//字母袋
void KuiItem::ziCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}
	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		ccMessageBox("该物品不能储存","提示:");
#else
		ccMessageBox(UTEXT("该物品不能储存",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}
	if  (!m_nIstrue)
	{
#ifdef WIN32
		ccMessageBox("子母袋还没有开启","提示:");
#else
		ccMessageBox(UTEXT("子母袋还没有开启",1).c_str(),UTEXT("提示:",1).c_str());
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
		//nBakChickTime = nCurChicktime;
/*#ifdef WIN32
		ccMessageBox("移动的太频繁了","提示:");
#else
		ccMessageBox(UTEXT("移动的太频繁了",1).c_str(),UTEXT("提示:",1).c_str());
#endif*/
		return;
	}
	nBakChickTime = nCurChicktime;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_equipmentex);
		//g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_immediacy);
}

void KuiItem::kuo1CallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}

	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		ccMessageBox("该物品不能储存","提示:");
#else
		ccMessageBox(UTEXT("该物品不能储存",1).c_str(),UTEXT("提示:",1).c_str());
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
		//nBakChickTime = nCurChicktime;
/*#ifdef WIN32
		ccMessageBox("移动的太频繁了","提示:");
#else
		ccMessageBox(UTEXT("移动的太频繁了",1).c_str(),UTEXT("提示:",1).c_str());
#endif*/
		return;
	}
	nBakChickTime = nCurChicktime;
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_exbox1);
	/*
#ifdef WIN32
	ccMessageBox("扩展箱暂时关闭","提示:");
#else
	ccMessageBox(UTEXT("扩展箱暂时关闭",1).c_str(),UTEXT("提示:",1).c_str());
#endif*/

}
void KuiItem::kuo2CallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}
	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		ccMessageBox("该物品不能储存","提示:");
#else
		ccMessageBox(UTEXT("该物品不能储存",1).c_str(),UTEXT("提示:",1).c_str());
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
		//nBakChickTime = nCurChicktime;
/*#ifdef WIN32
		ccMessageBox("移动的太频繁了","提示:");
#else
		ccMessageBox(UTEXT("移动的太频繁了",1).c_str(),UTEXT("提示:",1).c_str());
#endif*/
		return;
	}
	nBakChickTime = nCurChicktime;
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_exbox2);
	/*
#ifdef WIN32
	ccMessageBox("扩展箱暂时关闭","提示:");
#else
	ccMessageBox(UTEXT("扩展箱暂时关闭",1).c_str(),UTEXT("提示:",1).c_str());
#endif*/
}
void KuiItem::kuo3CallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("请选择一个物品!","提示:");
#else
		ccMessageBox(UTEXT("请选择一个物品!",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		return;
	}
	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		ccMessageBox("该物品不能储存","提示:");
#else
		ccMessageBox(UTEXT("该物品不能储存",1).c_str(),UTEXT("提示:",1).c_str());
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
		//nBakChickTime = nCurChicktime;
/*#ifdef WIN32
		ccMessageBox("移动的太频繁了","提示:");
#else
		ccMessageBox(UTEXT("移动的太频繁了",1).c_str(),UTEXT("提示:",1).c_str());
#endif*/
		return;
	}
	nBakChickTime = nCurChicktime;
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_exbox3);
/*#ifdef WIN32
	ccMessageBox("扩展箱暂时关闭","提示:");
#else
	ccMessageBox(UTEXT("扩展箱暂时关闭",1).c_str(),UTEXT("提示:",1).c_str());
#endif*/

}
//执行上层对象的回调函数，关闭弹出框
void KuiItem::buttonCallBackFunc(Ref * pSender)
{
	if  (!pSender)
	{
		if (m_callfun &&  m_callbackListener)
			(m_callfun)(NULL);
	}
	else
	{
		Node * node = dynamic_cast<Node *>(pSender);
		//node->setTag(3000);//设置tag，在调用层可以获取到
		if (m_callfun &&  m_callbackListener)
			(m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}

//关闭弹出框
void KuiItem::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}


//重写触摸注册函数，重新给定触摸级别
void KuiItem::registerWithTouchDispatcher()
{
	//这里的触摸优先级设置为－128，与CCMenu同级，保证了屏蔽下方的触摸
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
//	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
}

Rect KuiItem::getRect(Node* pNode,int i)
{
	//m_origin 背景的相对整个屏幕的开始位置
	Rect  rc;
	rc.origin     = pNode->getPosition();                      //这个因该是图内偏移
	rc.size       = pNode->getContentSize();                   //这个物品的宽度和高度
	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //起点最左边
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //最上边    //- rc.origin.y

	return rc;
}
//sap xep item dang drag & drop KienTM
void KuiItem::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;



	Point point = ParentNode_Item->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Team->getContentSize().width*/,winSize.height/* ParentNode_Team->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //如果在范围内 就可以拖动
	m_lastPoint = point;
	Point diff    = point - m_lastPoint;
	Point location = pTouch->getLocation();//目的的坐标?屏幕点击处
	Sprite * SkillNode =NULL;
	LayerColor *nCurColorlayer = NULL;
	_ItemData::iterator it;
	for( it = m_ItemData.begin(); it != m_ItemData.end(); ++it)
	{
		if (it->second.uId>0 && it->second.uId<MAX_ITEM)
		{////����ĳ���ܵļӳ�
			char nKey[32];
			//char msg[50];
			t_sprintf(nKey,"item_%u_color",Item[it->second.uId].GetID());
			std::string nItemKey = nKey;
			nCurColorlayer = (LayerColor *)ParentNode_Item->getChildByName(nItemKey);
			t_sprintf(nKey,"item_%u",Item[it->second.uId].GetID());
			nItemKey = nKey;
			SkillNode = (Sprite *)ParentNode_Item->getChildByName(nItemKey);
			// t_sprintf(msg,"ccTouchEnded:%d,nKey:%d",nKey,nKey);
			// ClientShowMsg(msg);
			if(ParentNode_ItemMove->getTag() == std::hash<std::string>{}(nItemKey)){
				if (SkillNode)
				{


					int nWidth = 0;
					int nHeight = 0;
					m_StartPos.x =31;
					m_StartPos.y =53;
					//lay vi tri cua o item
					int nCurX = (SkillNode->getPosition().x-m_StartPos.x)/ITEM_CELL_SIZE;

					int nCurY =(m_size.height-SkillNode->getPosition().y-m_StartPos.y)/(ITEM_CELL_SIZE);
					if (nCurX>=5){
						nCurX = 4;
					}
					if (nCurY>5){
						nCurY = 5;
					}
					int nIdx = Item[it->second.uId].GetID();
					ItemPos	sSrc, sDest;
					sSrc.nPlace     = pos_equiproom;    //Vi tri goc
					sSrc.nX         = m_ItemData[nstrCurSelItemKey].DataX;
					sSrc.nY         = m_ItemData[nstrCurSelItemKey].DataY;
					//sSrc.nSIsRongqi = m_ItemData[nstrCurSelItemKey].m_SisRongqi;

					sDest.nPlace     = pos_equiproom;  //Vi tri moi
					sDest.nX         = nCurX;
					sDest.nY         = nCurY;
					//sDest.nDIsRongqi = pSync->m_DisRongqi;
					if (!Player[CLIENT_PLAYER_INDEX].MoveItem(sDest, sSrc,nIdx)){
						//Neu vi tri chuyen toi da co item thi khai bao object no la data cua item, va call func AddObject de tra ve vi tri cu
						KUiDraggedObject no;
						no.uGenre = m_ItemData[nstrCurSelItemKey].uGenre;
						no.uId    = m_ItemData[nstrCurSelItemKey].uId;
						no.DataX  = m_ItemData[nstrCurSelItemKey].DataX;
						no.DataY  = m_ItemData[nstrCurSelItemKey].DataY;
						no.DataW  = m_ItemData[nstrCurSelItemKey].DataW;
						no.DataH  = m_ItemData[nstrCurSelItemKey].DataH;
						AddObject(&no,1);
					}

					//Hoan thanh chuc nang sap xep item drag & drop mobile 6:25 AM 21/09/2022 - Dev KienTM
				}
			}

		}else{
			if (m_descPadIsOpen)
			{
				m_descPadIsOpen = false;
				this->removeChildByTag(1001,true);
			}
			if (m_EqdescPadIsOpen)
			{
				m_EqdescPadIsOpen = false;
				this->removeChildByTag(1002,true);
			}
		}
	}



}
void KuiItem::ClientShowMsg(const char *Msg)
{
	// ccMessageBox(Msg,"ClientShowMsg");
	KSystemMessage	sMsg;
	ZeroMemory(&sMsg,sizeof(sMsg));
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.eType         = SMT_NORMAL;
	sMsg.byParamSize   = 0;
	sMsg.byPriority    = 0;

	if (sizeof(Msg)<125)
	  t_sprintf(sMsg.szMessage,Msg);
	else
	  t_sprintf(sMsg.szMessage,"Tin nhắn quá dài không thể gửi!");

	sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg,0);
}

//移动中
void KuiItem::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling || !__isCanMove) return;
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}
	if (m_EqdescPadIsOpen)
	{
		m_EqdescPadIsOpen = false;
		this->removeChildByTag(1002,true);
	}

	Point point = ParentNode_Item->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width,winSize.height);
	m_bScrolling = rect.containsPoint(point); //如果在范围内 就可以拖动
	m_lastPoint = point;
	Point diff = point - m_lastPoint;
	Point location = pTouch->getLocation();//目的的坐标?屏幕点击处
	Sprite * SkillNode =NULL;
	LayerColor *nCurColorlayer = NULL;
	bool isthis = false;


	_ItemData::iterator it;
	for( it = m_ItemData.begin(); it != m_ItemData.end(); ++it)
	{
		if (it->second.uId>0 && it->second.uId<MAX_ITEM)
		{////����ĳ���ܵļӳ�
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[it->second.uId].GetID());
			std::string nItemKey = nKey;
			nCurColorlayer = (LayerColor *)ParentNode_Item->getChildByName(nItemKey);
			t_sprintf(nKey,"item_%u",Item[it->second.uId].GetID());
			nItemKey = nKey;
			SkillNode = (Sprite *)ParentNode_Item->getChildByName(nItemKey);
			if(ParentNode_ItemMove->getTag() == std::hash<std::string>{}(nItemKey)){
				if (SkillNode)
				{
					Point touchLocaltion = pTouch->getLocation();
					SkillNode->setPosition(SkillNode->getPosition() + pTouch->getDelta());
					if (nCurColorlayer)
					{
						nCurColorlayer->setPosition(ax::Vec2(SkillNode->getPosition().x-ITEM_CELL_SIZE/2,SkillNode->getPosition().y-ITEM_CELL_SIZE/2) + pTouch->getDelta());
					}
				}
			}


		}
	}
}
//触摸函数ccTouchBegan，返回true
bool KuiItem::ccTouchBegan(Touch *pTouch, Event *pEvent)
{

	Point point = ParentNode_Item->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Team->getContentSize().width*/,winSize.height/* ParentNode_Team->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //如果在范围内 就可以拖动
	m_lastPoint = point;

	Point location = pTouch->getLocation();//目的的坐标?屏幕点击处
	Sprite * SkillNode =NULL;
	LayerColor *nCurColorlayer = NULL;
	bool isthis = false;

	_ItemData::iterator it;
	for( it = m_ItemData.begin(); it != m_ItemData.end(); ++it)
	{
		if (it->second.uId>0 && it->second.uId<MAX_ITEM)
		{////重置某技能的加成
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[it->second.uId].GetID());
			std::string nItemKey = nKey;
			nCurColorlayer = (LayerColor *)ParentNode_Item->getChildByName(nItemKey);
			t_sprintf(nKey,"item_%u",Item[it->second.uId].GetID());
			nItemKey = nKey;
			SkillNode = (Sprite *)ParentNode_Item->getChildByName(nItemKey);
			if  (nCurColorlayer)
			{
				if (getRect(nCurColorlayer,0).containsPoint(location))
				{//点中了
					if (SkillNode)
					{
						char msg[32];
						//SkillNode->runAction(red);
						SkillNode->setColor(ax::Color3B::RED);
						KUiItemdObject nTempItemData;
						ZeroMemory(&nTempItemData,sizeof(nTempItemData));
						m_ItemData[nItemKey].nRect  = getRect(nCurColorlayer,0);
						nTempItemData.nRect = m_ItemData[nItemKey].nRect;
						//m_ItemData[nItemKey].nindex = -1;

						//开始创建一个已装备说明
						if (Item[it->second.uId].GetGenre()==item_equip)
						{//如果是装备

							int nItemBoxIdx= Item[it->second.uId].GetDetailType();

							switch(nItemBoxIdx)
							{
							case 0:
							case 1:
								{//武器
									nTempItemData.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_weapon);
								}
								break;
							case 2://衣服
								{
									nTempItemData.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_body);
								}
								break;
							case 3://戒指
								{
									nTempItemData.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_ring1);
									if (!nTempItemData.uId)
										nTempItemData.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_ring2);
								}
								break;
							case 4:
								{
									nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_amulet);
								}
								break;
							case 5:
								{
									nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_foot);
								}
								break;
							case 6:
								{
									nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_belt);
								}
								break;
							case 7://itempart_head
								    nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_head);
								break;
							case 8:
								{
                                    nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_cuff);
								}
								break;
							case 9:
								{
									nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_pendant);
								}
								break;
							case 10:
								{
									nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_horse);
								}
								break;
							case 11:
								{
									nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_mask);
								}
								break;
							case 12:
								{
									nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_pifeng);
								}
								break;
							case 13:
								{
									nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_yinjian);
								}
								break;
							case 14:
								{
									nTempItemData.uId=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_shiping);
								}
								break;
							default:
								break;
							}

							if (nTempItemData.uId>0)
							{
							  float _bak_x =m_ItemData[nItemKey].nRect.origin.x;
							  nTempItemData.nRect.origin.x =m_ItemData[nItemKey].nRect.origin.x-370+Item[it->second.uId].GetWidth()*ITEM_CELL_SIZE;
							  Layer * sTempLayer = KuiItemdesc::create(nTempItemData,this,CC_CALLBACK_1(KuiItem::closeItemPadCallback1, this),10);
							//恢复原来的数据
							  nTempItemData.nRect.origin.x =_bak_x;
							  m_EqdescPadIsOpen = true;

							  this->addChild(sTempLayer,2,1002);
							}

						}
						Layer * sDescLayer = KuiItemdesc::create(it->second,this,CC_CALLBACK_1(KuiItem::closeItemPadCallback, this),0);
                        int nikey = std::hash<std::string>{}(nItemKey);
                        ParentNode_ItemMove->setTag(nikey);
						ParentNode_ItemMove->addChild(SkillNode,1);
						m_lastPoint = SkillNode->getPosition();
						m_descPadIsOpen = true;
						nstrCurSelItemKey =nItemKey;
						isthis = true;
						this->addChild(sDescLayer,2,1001);
					}
				}
				else
				{

					if (!isthis) //点中了,就不归零
					{
						nstrCurSelItemKey ="";
						//m_CurSelItemIndex = -1;
					}

					if (SkillNode)
						SkillNode->setColor(ax::Color3B::WHITE);           //默认颜色
				}
			}

		}
	}

	return true;
}

void KuiItem::closeItemPadCallback1(Node *pNode)
{
 	//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("KuiItem::closeItemPadCallback1");
	m_descPadIsOpen = false;
	m_EqdescPadIsOpen =false;
	this->removeChildByTag(1001,true);
	this->removeChildByTag(1002,true);
}

void KuiItem::closeItemPadCallback(Node *pNode)
{
	//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("KuiItem::closeItemPadCallback");
	m_descPadIsOpen = false;
	m_EqdescPadIsOpen =false;
	this->removeChildByTag(1001,true);
	this->removeChildByTag(1002,true);
}
void KuiItem::update(float delta)
{
	//ccMessageBox("无限循环","update");
	if (isOpen && g_pCoreShell)
	{
	    m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
		m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
		char nPointCount[32];
	   if (m_nMoney)
	   {
			char szMoneyText[32];
			if (m_nMoney < 10000)
			{
				sprintf(nPointCount, "%d lượng", m_nMoney);
			}
			else
			{

				if (m_nMoney%10000 == 0)
				{
					sprintf(nPointCount, "%d vạn lượng", m_nMoney/10000);
				}
				else
				{
					sprintf(nPointCount, "%d vạn %d lượng", m_nMoney/10000, m_nMoney%10000);
				}
			}
		  // t_sprintf(nPointCount,"%d",m_nMoney);
		  // pSkillPointLabel->setColor(ccWit)
		   pMoneyLabel->setString(nPointCount);
		   pMoneyLabel->setColor(ax::Color3B::RED);
	   }
	   if  (pXuLabel)
	   {
		   t_sprintf(nPointCount,"%d",m_nXu);
		   // pSkillPointLabel->setColor(ccWit)
		   pXuLabel->setString(nPointCount);
		   pXuLabel->setColor(ax::Color3B::YELLOW);
	   }
		   if (g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, GET_BOXEX_OPEN, 0))
			   m_nIstrue = true;
		   //	m_ItemEX.SetOpenClose(TRUE);
		   else
			   m_nIstrue = false;

	  //更新叠加数量
		   _ItemData::iterator it;
		   for( it = m_ItemData.begin(); it != m_ItemData.end(); ++it)
		   {
			   if (it->second.uId>0 && it->second.uId<MAX_ITEM)
			   {
				   if  (Item[it->second.uId].GetGenre()==0)
					   continue;

				   char nItemKey[32];
				   t_sprintf(nItemKey,"item_%u",Item[it->second.uId].GetID());
				   std::string nKey =nItemKey;
				   Sprite *nTempSpr = (Sprite *)ParentNode_Item->getChildByName(nKey);
				   if  (nTempSpr)
				   {
					   Label *stuckCountlabel = (Label *)nTempSpr->getChildByName("stack");
					   if  (stuckCountlabel)
					   {
						   char nCount[32];
						   t_sprintf(nCount,"%d",Item[it->second.uId].GetStackNum());
						   stuckCountlabel->setString(nCount);
					   }
				   }
			   }
		   }
	}
}

//void KuiItem::draw()
//{
//	//ccMessageBox("无限循环","draw");
//}

void KuiItem::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{
	if (pItem)
	{
		KUiDraggedObject Obj;
		Obj.uGenre = pItem->Obj.uGenre;
		Obj.uId    = pItem->Obj.uId;
		Obj.DataX  = pItem->Region.h;
		Obj.DataY  = pItem->Region.v;
		Obj.DataW  = pItem->Region.Width;
		Obj.DataH  = pItem->Region.Height;

		if  (Obj.uId<=0 || Obj.uId>=MAX_ITEM)
		{
			//ccMessageBox("数据有误","物品");
			return;
		}

		if (bAdd) //增加
		{
			//m_ItemBox.AddObject(&Obj,1);
			AddObject(&Obj,1);

		}
		else      //减少
		{
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[Obj.uId].GetID());
			std::string nItemKey = nKey;
			ParentNode_Item->removeChildByName(nItemKey,true);
			t_sprintf(nKey,"item_%u",Item[Obj.uId].GetID());
			nItemKey = nKey;
			ParentNode_Item->removeChildByName(nItemKey,true);
			m_ItemData.erase(nItemKey);
		}
			//m_ItemBox.RemoveObject(&Obj);
		//UiSoundPlay(UI_SI_PICKPUT_ITEM);
		//UpdateData();
	}
	else //更新全部的物品
		UpdateData();
}

void KuiItem::UpdateData()
{
// m_CurSelItemIndex = -1;
   nstrCurSelItemKey ="";
  // memset(m_ItemData,0,sizeof(KUiItemdObject) * 60);
   _ItemData::iterator it;
   for( it = m_ItemData.begin(); it != m_ItemData.end(); it++)
   {
	   if (it->second.uId>0 && it->second.uId<MAX_ITEM)
	   {////重置某技能的加成
		   char nKey[32];
		   t_sprintf(nKey,"item_%u_color",Item[it->second.uId].GetID());
		   std::string nItemKey = nKey;
		   ParentNode_Item->removeChildByName(nItemKey,true);
		   //nCurColorlayer = (LayerColor *)ParentNode_Item->getChildByName(nItemKey);
		   t_sprintf(nKey,"item_%u",Item[it->second.uId].GetID());
		   nItemKey = nKey;
		   //SkillNode = (Sprite *)ParentNode_Item->getChildByName(nItemKey);
		   ParentNode_Item->removeChildByName(nItemKey,true);
	   }
   }
   m_ItemData.clear();
  /* for (int i=0;i<m_nNumObjects;++i)
   {
	t_sprintf(nItemKey,"item_%d",i+1);
	std::string nKey =nItemKey;
	ParentNode_Item->removeChildByName(nKey,true);
	t_sprintf(nItemKey,"color_%d",i+1);
	nKey =nItemKey;
	ParentNode_Item->removeChildByName(nKey,true);
	//LayerColor *bgcolorLayer = (LayerColor *)ParentNode_Item->getChildByName(nKey);
   }*/

	KUiObjAtRegion* pObjs = NULL;
	int nCount    = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 0); //得到随身带物品的数量

	m_nNumObjects = nCount;

	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, (uintptr_t)pObjs, nCount);//单线程执行，nCount值不变
		for (int i = 0; i < nCount; ++i)
		{
			KUiDraggedObject no;
			no.uGenre = pObjs[i].Obj.uGenre;
			no.uId    = pObjs[i].Obj.uId;
			no.DataX  = pObjs[i].Region.h;     //包袱中的起点X位置 第几个格子 25x25
			no.DataY  = pObjs[i].Region.v;     //包袱中的起点Y位置
			no.DataW  = pObjs[i].Region.Width; //物品的高度(占用了多少个格子)
			no.DataH  = pObjs[i].Region.Height;//物品的宽度(占用了多少个格子)
			if (Item && pObjs[i].Obj.uId>0 && pObjs[i].Obj.uId<MAX_ITEM)
			{
				/*m_ItemData[i].uGenre = pObjs[i].Obj.uGenre;
				m_ItemData[i].uId    = pObjs[i].Obj.uId;
				m_ItemData[i].nindex = i;
				m_ItemData[i].DataX  = pObjs[i].Region.h;     //包袱中的起点X位置 第几个格子 25x25
				m_ItemData[i].DataY  = pObjs[i].Region.v;     //包袱中的起点Y位置
				m_ItemData[i].DataW  = pObjs[i].Region.Width; //物品的高度(占用了多少个格子)
				m_ItemData[i].DataH  = pObjs[i].Region.Height;//物品的宽度(占用了多少个格子)
				m_ItemData[i].nGenkind = UOC_ITEM_TAKE_WITH;  //随身携带
				*/
				char nItemSprPath[256];
				ZeroMemory(nItemSprPath,sizeof(nItemSprPath));

				if (!Item[pObjs[i].Obj.uId].GetImagePath())
				{
					t_sprintf(nItemSprPath,"\\spr\\others\\问号.spr");
				}
				else
					t_sprintf(nItemSprPath,Item[pObjs[i].Obj.uId].GetImagePath());

				if  (nItemSprPath[0])
				{
					//t_sprintf(nItemSprPath,Item[pObjs[i].Obj.uId].GetImagePath());
					g_StrLower(nItemSprPath);
					int m_nWidth,m_nHeight,nFrams;
					Texture2D *bgCur = NULL;
					SPRFRAMSINFO nSprInfo;
					ZeroMemory(&nSprInfo,sizeof(nSprInfo));
					bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
					if (bgCur==NULL)
						return;
					char nItemKey[32];
					ZeroMemory(&nItemKey,sizeof(nItemKey));
					t_sprintf(nItemKey,"item_%u",Item[pObjs[i].Obj.uId].GetID());
					std::string nKey =nItemKey;
					Sprite *nItemSpr = (Sprite *)ParentNode_Item->getChildByName(nKey);
					if (!nItemSpr)
					{
						nItemSpr = Sprite::createWithTexture(bgCur);
						//nItemSpr->setAnchorPoint(ax::Vec2(0,0));
                        int nikey = std::hash<std::string>{}(nKey);
                        nItemSpr->setTag(nikey);
						ParentNode_Item->addChild(nItemSpr,1);

						if  (Item[pObjs[i].Obj.uId].GetGenre()!=item_equip && Item[pObjs[i].Obj.uId].IsStack())
						{//不是装备 可叠加物品
							char stack[32];
							t_sprintf(stack,"%d",Item[pObjs[i].Obj.uId].GetStackNum());
							Label *stuckCountlabel = Label::createWithTTF(stack,UI_GAME_FONT_DEFAULT,14);
							stuckCountlabel->setColor(ax::Color3B::YELLOW);
							stuckCountlabel->setAnchorPoint(ax::Vec2(0,0));
							stuckCountlabel->setPosition(ax::Vec2(0,0));
                            int nikey = std::hash<std::string>{}("stack");
							stuckCountlabel->setTag(nikey);
							nItemSpr->addChild(stuckCountlabel,1);
						}
					}
					//开始填充书库
					m_ItemData[nKey].uGenre = pObjs[i].Obj.uGenre;
					m_ItemData[nKey].uId    = pObjs[i].Obj.uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObjs[i].Region.h;     //包袱中的起点X位置 第几个格子 25x25
					m_ItemData[nKey].DataY  = pObjs[i].Region.v;     //包袱中的起点Y位置
					m_ItemData[nKey].DataW  = pObjs[i].Region.Width; //物品的高度(占用了多少个格子)
					m_ItemData[nKey].DataH  = pObjs[i].Region.Height;//物品的宽度(占用了多少个格子)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //随身携带

					Color4B color(112, 128, 144, 150);//112, 128, 144
					//t_sprintf(nItemKey,"color_%d",i+1);
					strcat(nItemKey,"_color");
					nKey =nItemKey;
					LayerColor *bgcolorLayer = (LayerColor *)ParentNode_Item->getChildByName(nKey);
					if (!bgcolorLayer)
					{
						bgcolorLayer = LayerColor::create(color);//颜色层
					    //bgcolorLayer->setPosition(ax::Vec2::ZERO);
						bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
						Size bgsize;
						bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
						bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					    bgcolorLayer->setContentSize(bgsize);//设置和窗口的大小
                        int nikey = std::hash<std::string>{}(nKey);
						bgcolorLayer->setTag(nikey);
						ParentNode_Item->addChild(bgcolorLayer,0);
					}

					int nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
					int nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;

					nItemSpr->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));
					//颜色层
					nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+1;
					nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE-1;
					bgcolorLayer->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));

					KUiObjAtContRegion	Obj;
					Obj.Obj.uGenre = no.uGenre;
					Obj.Obj.uId    = no.uId;
					Obj.Region.h   = Obj.Region.v = 0;
					Obj.Region.Width = Obj.Region.Height = 0;
					Obj.nContainer = UOC_ITEM_TAKE_WITH;

					ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					    GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (uintptr_t)&Obj, 0);
					//if (eProp == IIEP_NORMAL) //可用
					//else
					if (eProp == IIEP_NOT_USEABLE)  //不可用
					{
						ax::Color3B color_(128, 0, 0);
						bgcolorLayer->setColor(color_);
					}
					//else if (eProp == IIEP_SPECIAL) //其他特殊物品
				}
			}
		}
		free(pObjs); //释放内存
		pObjs = NULL; //清空
	}
}

//增加物品
int KuiItem::AddObject(KUiDraggedObject* pObject, int nCount)
{
	if (!pObject) return false;
	m_StartPos.x =31;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =53;//+ITEM_CELL_SIZE/2;
	for (int i = 0; i < nCount; ++i)
	{
			KUiDraggedObject no;
			no.uGenre = pObject->uGenre;
			no.uId    = pObject->uId;
			no.DataX  = pObject->DataX;     //包袱中的起点X位置 第几个格子 25x25
			no.DataY  = pObject->DataY;     //包袱中的起点Y位置
			no.DataW  = pObject->DataW; //物品的高度(占用了多少个格子)
			no.DataH  = pObject->DataH;//物品的宽度(占用了多少个格子)
			if (Item && pObject->uId>0 && pObject->uId<MAX_ITEM)
			{
				char nItemSprPath[256];
				ZeroMemory(nItemSprPath,sizeof(nItemSprPath));

				if (!Item[pObject->uId].GetImagePath())
				{
					t_sprintf(nItemSprPath,"\\spr\\others\\问号.spr");
				}
				else
					t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());

				if  (nItemSprPath[0])
				{
					//char msg[50];
					g_StrLower(nItemSprPath);
					int m_nWidth,m_nHeight,nFrams;
					Texture2D *bgCur = NULL;
					SPRFRAMSINFO nSprInfo;
					ZeroMemory(&nSprInfo,sizeof(nSprInfo));
					bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
					if (bgCur==NULL)
					{
						t_sprintf(nItemSprPath,"\\spr\\others\\问号.spr");
						g_StrLower(nItemSprPath);
						bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
						//return false;
					}
					char nItemKey[32];
					ZeroMemory(&nItemKey,sizeof(nItemKey));
					t_sprintf(nItemKey,"item_%u",Item[pObject->uId].GetID());
					std::string nKey =nItemKey;
					Sprite *nItemSpr = (Sprite *)ParentNode_Item->getChildByName(nKey);
					//t_sprintf(msg,"AddObject:%d,nKey:%d",nItemKey,nItemKey);
					//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nItemKey);
					if (!nItemSpr)
					{

						nItemSpr = Sprite::createWithTexture(bgCur);
                        int nikey = std::hash<std::string>{}(nKey);
						nItemSpr->setTag(nikey);
						ParentNode_Item->addChild(nItemSpr,1);
						if (Item[pObject->uId].GetGenre()==item_equip){
							nItemSpr->setScale(0.8);
						}
						//find here
						if (Item[pObject->uId].GetDetailType()==equip_armor){
							nItemSpr->setScale(0.7);
						}
						if  (Item[pObject->uId].GetGenre()!=item_equip && Item[pObject->uId].IsStack())
						{//不是装备 可叠加物品
							char stack[32];
							t_sprintf(stack,"%d",Item[pObject->uId].GetStackNum());
							Label *stuckCountlabel = Label::createWithTTF(stack,UI_GAME_FONT_DEFAULT,14);
							stuckCountlabel->setColor(ax::Color3B::WHITE);
							stuckCountlabel->setAnchorPoint(ax::Vec2(0,0));
							stuckCountlabel->setPosition(ax::Vec2(0,0));
                            int nikey = std::hash<std::string>{}("stack");
							stuckCountlabel->setTag(nikey);
							nItemSpr->addChild(stuckCountlabel,1);
						}
					}
					//开始填充书库
					m_ItemData[nKey].uGenre = pObject->uGenre;
					m_ItemData[nKey].uId    = pObject->uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObject->DataX;     //包袱中的起点X位置 第几个格子 25x25
					m_ItemData[nKey].DataY  = pObject->DataY;     //包袱中的起点Y位置
					m_ItemData[nKey].DataW  = pObject->DataW; //物品的高度(占用了多少个格子)
					m_ItemData[nKey].DataH  = pObject->DataH;//物品的宽度(占用了多少个格子)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //随身携带

					Color4B color(0, 56, 33, 150);//112, 128, 144
					strcat(nItemKey,"_color");
					nKey =nItemKey;
					LayerColor *bgcolorLayer = (LayerColor *)ParentNode_Item->getChildByName(nKey);
					if (!bgcolorLayer)
					{
						bgcolorLayer = LayerColor::create(color);//颜色层
					    //bgcolorLayer->setPosition(ax::Vec2::ZERO);
						bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
						Size bgsize;
						bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
						bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					    bgcolorLayer->setContentSize(bgsize);//设置和窗口的大小
						int nikey = std::hash<std::string>{}(nKey);
bgcolorLayer->setTag(nikey);
						ParentNode_Item->addChild(bgcolorLayer,0);
					}

					int nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
					int nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;
					switch (no.DataX)
					{
					case 0:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2)+2;
						break;
					case 1:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2)+8;
						break;
					case 2:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2)+14;
						break;
					case 3:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2)+20;
						break;
					case 4:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2)+26;
						break;
					default:
						break;
					}

					switch (no.DataY)
					{
					case 0:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+2;
						break;
					case 1:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+8;
						break;
					case 2:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+14;
						break;
					case 3:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+20;
						break;
					case 4:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+26;
						break;
					case 5:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2)+32;
						break;
					default:
						break;
					}
					nItemSpr->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));

					switch (no.DataX)
					{
					case 0:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE)+2;
						break;
					case 1:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE)+8;
						break;
					case 2:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE)+14;
						break;
					case 3:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE)+20;
						break;
					case 4:
						nCurX = (m_StartPos.x+no.DataX*ITEM_CELL_SIZE)+26;
						break;
					default:
						break;
					}

					switch (no.DataY)
					{
					case 0:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+2;
						break;
					case 1:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+8;
						break;
					case 2:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+14;
						break;
					case 3:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+20;
						break;
					case 4:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+26;
						break;
					case 5:
						nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+32;
						break;
					default:
						break;
					}
					//nCurY = (m_StartPos.y+no.DataY*ITEM_CELL_SIZE+1*ITEM_CELL_SIZE)+32;
					bgcolorLayer->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));

					KUiObjAtContRegion	Obj;
					Obj.Obj.uGenre = no.uGenre;
					Obj.Obj.uId    = no.uId;
					Obj.Region.h   = Obj.Region.v = 0;
					Obj.Region.Width = Obj.Region.Height = 0;
					Obj.nContainer = UOC_ITEM_TAKE_WITH;

					ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					    GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (uintptr_t)&Obj, 0);
					//if (eProp == IIEP_NORMAL) //可用
					//else
					if (eProp == IIEP_NOT_USEABLE)  //不可用
					{
						ax::Color3B color_(128, 0, 0);
						bgcolorLayer->setColor(color_);
					}
					//else if (eProp == IIEP_SPECIAL) //其他特殊物品
				}
			}
	}
	return true;
}
