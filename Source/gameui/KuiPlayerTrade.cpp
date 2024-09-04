//�а�ť�ġ������롡�Ի���
//
#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiPlayerTrade.h"
#include "engine/Text.h"
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

#define	STR_WAIT_TRADING "�ȴ�����ִ��"
#define	STR_OTHER_OK	 "�Է�������"
#define	STR_OTHER_NOT_OK "�ȴ��Է�����"
#define	SEND_MONEY_CHANGE_MSG_DELAY		16

KuiPlayerTrade::KuiPlayerTrade()
{
	isOpen = false;
	colorLayer        = NULL;
	//red               = NULL;
	ParentNode_Item   = NULL;
	ParentNode_Give   = NULL;
	ParentNode_tarGive= NULL;
	ptradeInfo        = NULL;
	m_StartPos.x=0;
	m_StartPos.y=0;
	m_StartPos_g.x = 0;
	m_StartPos_g.y = 0;
	m_StartPos_t.x = 0;
	m_StartPos_t.y = 0;
	//btnBackPic = NULL;
	m_nItemNumObjects = 0;
	m_ItemData.clear();
	m_giveItemData.clear();
	m_targiveItemData.clear();
	m_nIstrue = false;
	nstrCurSelItemKey = "";
	inCurSelItemKey   = -1;
	m_descPadIsOpen = false;
	m_targiveNum = 0;
	m_giveNum = 0;
	m_itemNum = 0;
	m_nNeed   = 0;
	nBakChickTime = 0;
	m_nMoney =0;
	//m_nXu    =0;
	m_bAdjustingMoney= 0;
	m_nTakewithMoney= 0;
	m_nSelfTradeMoney = 0;
	m_OtherMoney = NULL;
	m_SelfMoneyBox = NULL;
	okConfirm = NULL;
	tradeConfirm= NULL;
	__IsLock = false;
}

KuiPlayerTrade::~KuiPlayerTrade()
{
	isOpen = false;
	colorLayer = NULL;
	//if (red)
	//	red->release();

	m_ItemData.clear();
	m_giveItemData.clear();
	m_targiveItemData.clear();
}

//����ȷ��������
KuiPlayerTrade * KuiPlayerTrade::create(KUiPlayerItem* pOther,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiPlayerTrade * popLayer = KuiPlayerTrade::create();

	if (pOther)
	   popLayer->m_OtherData = *pOther;

	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;

	//popLayer->m_nNeed = nNum;  //��ִ�нű����� kind  ���ǲ�ִ�нű�
	//t_sprintf(popLayer->m_Callback,"%s",pszCallback);
	//t_sprintf(popLayer->m_Error,"%s",pszError);
	return popLayer;
}


//��ʼ��
bool KuiPlayerTrade::init()
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
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\��ҽ���.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;
	Sprite * spriteBox = Sprite::createWithTexture(bgCur);
	ParentNode_ItemEx = spriteBox;
	//��ñ���ͼƬ�Ĵ�С
	Size contentSize = spriteBox->getContentSize();
	m_size   = contentSize;
	m_origin = spriteBox->getPosition();
	spriteBox->setPosition(ax::Vec2(winSize.width/2,winSize.height/2+20)); //-m_size.width/2+50
	this->addChild(spriteBox);

	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С
	//����������Ӧ

	this->scheduleUpdate();                   //���ú���
	auto listener = ax::EventListenerTouchOneByOne::create();
ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);   //��ֹ��㴥��
	return true;
}

void KuiPlayerTrade::addDialogData()
{
	if  (!g_pCoreShell) return;
	Size nSize;
	nSize.width =150;
	nSize.height=100;

	m_StartPos.x =m_size.width-28*6-10;    //+ITEM_CELL_SIZE/2;
	m_StartPos.y =40;//+ITEM_CELL_SIZE/2;

	m_StartPos_g.x =9;
	m_StartPos_g.y =210;

	m_StartPos_t.x =9;
	m_StartPos_t.y =55;

	char nTempStr[64];
	ParentNode_Give =Node::create();
	ParentNode_Item =Node::create();
	ParentNode_tarGive =Node::create();
	ParentNode_ItemEx->addChild(ParentNode_Item);
	ParentNode_ItemEx->addChild(ParentNode_Give);
	ParentNode_ItemEx->addChild(ParentNode_tarGive);

	t_sprintf(nTempStr,m_OtherData.Name);
	Label *ptextLabel = Label::createWithTTF(UTEXT(nTempStr,1).c_str(),UI_GAME_FONT_DEFAULT,12);//Arial
	ptextLabel->setColor(ax::Color3B::YELLOW);
	ptextLabel->setPosition(ax::Vec2(50,m_size.height-48));
	ParentNode_ItemEx->addChild(ptextLabel);
	/*t_sprintf(nTempStr,pContent);
	Label *pstrLabel = Label::createWithTTF(UTEXT(nTempStr,1).c_str(),UI_GAME_FONT_DEFAULT,12,nSize,ax::TextHAlignment::LEFT);//Arial
	pstrLabel->setColor(ax::Color3B::YELLOW);
	pstrLabel->setPosition(ax::Vec2(128/2+40,m_size.height-143));
	ParentNode_ItemEx->addChild(pstrLabel);

	char ntitleStr[64];
	t_sprintf(ntitleStr,strTitel);
	Label *ptitLabel = Label::createWithTTF(UTEXT(ntitleStr,1).c_str(),UI_GAME_FONT_DEFAULT,12);//Arial
	ptitLabel->setPosition(ax::Vec2(m_size.width/2,m_size.height-80));
	ParentNode_ItemEx->addChild(ptitLabel);
	*/
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Arial
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	pMoneyLabel->setPosition(ax::Vec2(m_size.width/2+70,43));
	ParentNode_ItemEx->addChild(pMoneyLabel,1);

	char nPointCount[32];
	m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
	m_nTakewithMoney = m_nMoney;
	//m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
	if (pMoneyLabel)
	{
		t_sprintf(nPointCount,"%d",m_nMoney);
		pMoneyLabel->setString(nPointCount);
	}

	KUiObjAtRegion* pObjs = NULL;
	int nCount   = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 0); //�õ��������Ʒ������

	m_nItemNumObjects = nCount;

	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, (uintptr_t)pObjs, nCount);//���߳�ִ�У�nCountֵ����
		for (int i = 0; i < nCount; ++i)
		{
			KUiDraggedObject no;
			no.uGenre = pObjs[i].Obj.uGenre;
			no.uId    = pObjs[i].Obj.uId;
			no.DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
			no.DataY  = pObjs[i].Region.v;     //�����е����Yλ��
			no.DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
			no.DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
			if (Item && pObjs[i].Obj.uId>0 && pObjs[i].Obj.uId<MAX_ITEM)
			{
				/*m_ItemData[i].uGenre = pObjs[i].Obj.uGenre;
				m_ItemData[i].uId    = pObjs[i].Obj.uId;
				m_ItemData[i].nindex = i;
				m_ItemData[i].DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
				m_ItemData[i].DataY  = pObjs[i].Region.v;     //�����е����Yλ��
				m_ItemData[i].DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
				m_ItemData[i].DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
				m_ItemData[i].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��
				*/
				char nItemSprPath[256];
				ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
				if  (Item[pObjs[i].Obj.uId].GetImagePath())
					t_sprintf(nItemSprPath,Item[pObjs[i].Obj.uId].GetImagePath());
				else
					t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");

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
						{//����װ�� �ɵ�����Ʒ
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
					//��ʼ������
					m_ItemData[nKey].uGenre = pObjs[i].Obj.uGenre;
					m_ItemData[nKey].uId    = pObjs[i].Obj.uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
					m_ItemData[nKey].DataY  = pObjs[i].Region.v;     //�����е����Yλ��
					m_ItemData[nKey].DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
					m_ItemData[nKey].DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��

					Color4B color(112, 128, 144, 150); //112, 128, 144
					//t_sprintf(nItemKey,"color_%d",i+1);
					strcat(nItemKey,"_color");
					nKey = nItemKey;
					LayerColor *bgcolorLayer = (LayerColor *)ParentNode_Item->getChildByName(nKey);
					if (!bgcolorLayer)
					{
						bgcolorLayer = LayerColor::create(color);//��ɫ��
					    //bgcolorLayer->setPosition(ax::Vec2::ZERO);
						bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
						Size bgsize;
						bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
						bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					    bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
						int nikey = std::hash<std::string>{}(nKey);
bgcolorLayer->setTag(nikey);
						ParentNode_Item->addChild(bgcolorLayer,0);
					}

					int nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
					int nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;

					nItemSpr->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));
					//��ɫ��
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
					//if (eProp == IIEP_NORMAL) //����
					//else
					if (eProp == IIEP_NOT_USEABLE)  //������
					{
						ax::Color3B color_(128, 0, 0);
						bgcolorLayer->setColor(color_);
					}
					//else if (eProp == IIEP_SPECIAL) //����������Ʒ
				}
			}
		}
		free(pObjs); //�ͷ��ڴ�
		pObjs = NULL; //���
	}

	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);//���úʹ��ڵĴ�С
	ParentNode_Item->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	//red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	//red->retain();
}


//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiPlayerTrade::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\��ҽ��ף���������.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	Point nStartPos;
	nStartPos.x = m_size.width/2+20;
	nStartPos.y = m_size.height/2;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_ok_btn_confirm_normal =  Sprite::createWithTexture(bgCur);//Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_normal);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_ok_btn_confirm_select =  Sprite::createWithTexture(bgCur);
	okConfirm = MenuItemSprite::create(sprite_ok_btn_confirm_normal, sprite_ok_btn_confirm_select, CC_CALLBACK_1(KuiPlayerTrade::okCallBackFunc, this));
	okConfirm->setPosition(ax::Vec2(-140,0));
    int nikey = std::hash<std::string>{}("box");
	okConfirm->setTag(nikey);
	//����ȷ��
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\��ҽ��ף�����ȷ��.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_trade_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_trade_select = Sprite::createWithTexture(bgCur);
	tradeConfirm = MenuItemSprite::create(sprite_trade_normal, sprite_trade_select, CC_CALLBACK_1(KuiPlayerTrade::tradeCallBackFunc, this));
	tradeConfirm->setPosition(ax::Vec2(-15,0));
	tradeConfirm->setTag(nikey);
	//�ر�
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\��ҽ��ף�ȡ������.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_close_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_close_select = Sprite::createWithTexture(bgCur);
	MenuItemSprite * closeConfirm = MenuItemSprite::create(sprite_close_normal, sprite_close_select, CC_CALLBACK_1(KuiPlayerTrade::buttonCallBackFunc, this));
	//closeConfirm->setTag("box");
	closeConfirm->setPosition(ax::Vec2(120,0));

	//��ʼ���Ӳ�����ť
	Sprite * sprite_fanzhi_normal =  Sprite::create("ui/btn/fz01.png");
	Sprite * sprite_fanzhi_select = Sprite::create("ui/btn/fz02.png");
	MenuItemSprite * fanzhiConfirm = MenuItemSprite::create(sprite_fanzhi_normal, sprite_fanzhi_select, CC_CALLBACK_1(KuiPlayerTrade::btnCallBackFunc_fangzhi, this));
	fanzhiConfirm->setPosition(ax::Vec2(38,-30));

	Sprite * sprite_quxia_normal =  Sprite::create("ui/btn/qx01.png");
	Sprite * sprite_quxia_select = Sprite::create("ui/btn/qx02.png");
	MenuItemSprite * quxiaConfirm = MenuItemSprite::create(sprite_quxia_normal, sprite_quxia_select, CC_CALLBACK_1(KuiPlayerTrade::btnCallBackFunc_quxia, this));
	quxiaConfirm->setPosition(ax::Vec2(38+sprite_quxia_normal->getContentSize().width,-30));
	//Point bgSpritePoint = ParentNode_ItemEx->getPosition();
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\������\\ͼ��\\zenglibtn.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_zhengli_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_zhengli_select =  Sprite::createWithTexture(bgCur);
	MenuItemSprite * zhengliConfirm = MenuItemSprite::create(sprite_zhengli_normal, sprite_zhengli_select, CC_CALLBACK_1(KuiPlayerTrade::zhengCallBackFunc, this));
	zhengliConfirm->setPosition(ax::Vec2(35+sprite_quxia_normal->getContentSize().width*2,-30));

	tradeConfirm->setEnabled(false);
	Menu * menu = Menu::create(okConfirm,tradeConfirm,closeConfirm,fanzhiConfirm,quxiaConfirm,zhengliConfirm,NULL);
    //[TODO][zer0kull]
//	menu->setisCheckBox(true);
	menu->setPosition(ax::Vec2(m_size.width/2,20)); //�˵�λ������Ϊ����������
	ParentNode_ItemEx->addChild(menu);         //�ӵ�������

	ax::ui::Scale9Sprite *pBg = ax::ui::Scale9Sprite::create();
	m_SelfMoneyBox = EditBox::create(Size(112,24),pBg);
	const ax::Color3B nfcolor ={255,255,255};
	if (m_SelfMoneyBox)
	{
		//pEditBox->setContentSize()
		//pMoneyEditBox->setText("your money");
		m_SelfMoneyBox->setPlaceHolder("input money");
		m_SelfMoneyBox->setFontColor(nfcolor);
		m_SelfMoneyBox->setFontSize(24);
		m_SelfMoneyBox->setMaxLength(9);
		m_SelfMoneyBox->setPosition(ax::Vec2(90,45));
		m_SelfMoneyBox->setInputMode(EditBox::InputMode::DECIMAL);
		m_SelfMoneyBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		ParentNode_ItemEx->addChild(m_SelfMoneyBox,1,500);
	}

	m_OtherMoney = Label::createWithTTF("money",UI_GAME_FONT_DEFAULT,12);//Arial
	m_OtherMoney->setColor(ax::Color3B::YELLOW);
	m_OtherMoney->setPosition(ax::Vec2(90,m_size.height-170));
	ParentNode_ItemEx->addChild(m_OtherMoney);
}


void KuiPlayerTrade::zhengCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		ccMessageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}
	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		ccMessageBox("����Ʒ��������","��ʾ:");
#else
		ccMessageBox(UTEXT("����Ʒ��������",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);

	unsigned int nCurChicktime = pInfo.npTime;  //��¼�򿪴��ڵ�����

	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//�������
		//nBakChickTime = nCurChicktime;
#ifdef WIN32
		ccMessageBox("�����̫Ƶ����","��ʾ:");
#else
		ccMessageBox(UTEXT("�����̫Ƶ����",1).c_str(),UTEXT("��ʾ:",1).c_str());
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
//����ȷ�� ��ʼ����
void KuiPlayerTrade::tradeCallBackFunc(Ref * pSender)
{
	g_pCoreShell->OperationRequest(GOI_TRADE,0,0);
	UpdateTradeOperData();
}
//��������
void KuiPlayerTrade::okCallBackFunc(Ref * pSender)
{
	if (pSender)
	{
		Node *  nCurSelNode = ((MenuItemImage *)pSender)->getSelectedImage();
		if (nCurSelNode)
		{
			OnAdjustedMoney();
			if  (nCurSelNode->isVisible())
			{//�Ѿ���ѡ��״̬ δѡ��---ѡ��
				g_pCoreShell->OperationRequest(GOI_TRADE_LOCK,0,true);
				__IsLock = true;
			}
			else
			{//����Ϊѡ��״̬ :ѡ��---δѡ��
				g_pCoreShell->OperationRequest(GOI_TRADE_LOCK,0,false);
				__IsLock = false;
			}
			UpdateTradeOperData();
		}
		//((MenuItemImage *)pSender)->setEnabled(false);
	}

	/*Node * node = NULL;
	if (pSender)
		node = dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);*/
}

void KuiPlayerTrade::btnCallBackFunc_fangzhi(Ref * pSender)
{//�����Լ��Ķ���
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (nstrCurSelItemKey.empty() || m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("��ѡ��һ����Ʒ!","��ʾ");
#else
		ccMessageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		return;
	}
	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		ccMessageBox("����Ʒ���ܷ���","��ʾ");
#else
		ccMessageBox(UTEXT("����Ʒ���ܷ���",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		return;
	}

	if (__IsLock)
	{
#ifdef WIN32
		ccMessageBox("�������ײ��ܷ���","��ʾ");
#else
		ccMessageBox(UTEXT("�������ײ��ܷ���",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		return;
	}
	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);

	unsigned int nCurChicktime = pInfo.npTime;  //��¼�򿪴��ڵ�����

	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//�������
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
	nstrCurSelItemKey="";
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_trade);
	//g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_immediacy);
}

void KuiPlayerTrade::btnCallBackFunc_quxia(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
	}

	if  (inCurSelItemKey<=-1 || m_giveItemData.count(inCurSelItemKey)<=0)
	{
#ifdef WIN32
		ccMessageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		ccMessageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}
	if  (m_giveItemData[inCurSelItemKey].uId<=0 || m_giveItemData[inCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		ccMessageBox("����Ʒ����ȡ��","��ʾ:");
#else
		ccMessageBox(UTEXT("����Ʒ����ȡ��",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

	if (__IsLock)
	{
#ifdef WIN32
		ccMessageBox("�������ײ���ȡ��","��ʾ");
#else
		ccMessageBox(UTEXT("�������ײ���ȡ��",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		return;
	}

	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);

	unsigned int nCurChicktime = pInfo.npTime;  //��¼�򿪴��ڵ�����

	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//�������
		return;
	}
	nBakChickTime = nCurChicktime;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_giveItemData[inCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_giveItemData[inCurSelItemKey].uId;
	Obj.Region.h      = m_giveItemData[inCurSelItemKey].DataX;
	Obj.Region.v      = m_giveItemData[inCurSelItemKey].DataY;
	Obj.Region.Width  = m_giveItemData[inCurSelItemKey].DataW;
	Obj.Region.Height = m_giveItemData[inCurSelItemKey].DataH;
	Obj.eContainer    = UOC_TO_BE_TRADE;
	inCurSelItemKey=-1;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_equipment);
}

void KuiPlayerTrade::__ClosePad()
{
	Node * node = NULL;
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}
//�ر����
void KuiPlayerTrade::buttonCallBackFunc(Ref * pSender)
{
	//����ȡ��
	g_pCoreShell->OperationRequest(GOI_TRADE_CANCEL,0,0);

	Node * node = NULL;
	if (pSender)
		node = dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiPlayerTrade::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiPlayerTrade::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
}

void KuiPlayerTrade::closeItemPadCallback(Node *pNode)
{//��������б�Ǻţ���ȷ�����ĸ���ť
	m_descPadIsOpen = false;
}

Rect KuiPlayerTrade::gettarGiveItem(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition();                      //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();                   //�����Ʒ�Ŀ�Ⱥ͸߶�
	rc.origin.x   = m_origin.x + m_size.width/2-28+rc.origin.x;      //�������� + m_size.width-30
	rc.origin.y   = m_origin.y+rc.origin.y+m_size.height/2-115;    //���ϱ�    //- rc.origin.y
	return rc;
}

Rect KuiPlayerTrade::getGiveItem(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition();                      //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();                   //�����Ʒ�Ŀ�Ⱥ͸߶�
	rc.origin.x   = m_origin.x + m_size.width/2-28+rc.origin.x;      //�������� + m_size.width-30
	rc.origin.y   = m_origin.y+ rc.origin.y + 70;              //���ϱ�
	return rc;
}


Rect KuiPlayerTrade::getRectItem(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();
	rc.origin.x   = m_origin.x+ rc.origin.x+20+28*6; //�������� m_size.width/2-30
	rc.origin.y   = m_origin.y+ rc.origin.y+70; //���ϱ�    //- rc.origin.y

	return rc;
}
//��������ccTouchBegan������true
bool KuiPlayerTrade::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
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
			if  (nCurColorlayer)
			{
				if (getRectItem(nCurColorlayer,0).containsPoint(location))
				{//������
					if (SkillNode)
					{
						inCurSelItemKey   = -1;
						//SkillNode->runAction(red);
						SkillNode->setColor(ax::Color3B::RED);
						m_ItemData[nItemKey].nRect  = getRectItem(nCurColorlayer,0);
						m_ItemData[nItemKey].nindex = -1;
						Layer * sDescLayer = KuiItemdesc::create(it->second,this,CC_CALLBACK_1(KuiPlayerTrade::closeItemPadCallback, this),1);
						m_descPadIsOpen = true;
						//m_CurSelItemIndex = -1;//m_ItemData[i].uId;
						nstrCurSelItemKey =nItemKey;

						isthis = true;
						this->addChild(sDescLayer,2,1001);
					}
				}
				else
				{
					if (!isthis) //������,�Ͳ�����
					{
						nstrCurSelItemKey ="";
						inCurSelItemKey   = -1;
						//m_CurSelItemIndex = -1;
					}
					if (SkillNode)
						SkillNode->setColor(ax::Color3B::WHITE);           //Ĭ����ɫ
				}
			}
		}
	}
	//give���
	isthis = false;
	_giveItemData::iterator its;
	for( its = m_giveItemData.begin(); its != m_giveItemData.end(); ++its)
	{
		if (its->second.uId>0 && its->second.uId<MAX_ITEM)
		{////����ĳ���ܵļӳ�
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[its->second.uId].GetID());
			std::string nItemKey = nKey;
			nCurColorlayer = (LayerColor *)ParentNode_Give->getChildByName(nItemKey);
			t_sprintf(nKey,"item_%u",Item[its->second.uId].GetID());
			nItemKey = nKey;
			SkillNode = (Sprite *)ParentNode_Give->getChildByName(nItemKey);
			if  (nCurColorlayer)
			{
				//ccMessageBox("��ʼ���","��ʼ���");
				if (getGiveItem(nCurColorlayer,0).containsPoint(location))
				{//������
					//ccMessageBox("������","������");
					if (SkillNode)
					{
						//SkillNode->runAction(red);
						SkillNode->setColor(ax::Color3B::RED);
						its->second.nRect  = getGiveItem(nCurColorlayer,0);
						inCurSelItemKey    = its->second.nindex;
						//its->second.nindex = -1;
						Layer * sDescLayer = KuiItemdesc::create(its->second,this,CC_CALLBACK_1(KuiPlayerTrade::closeItemPadCallback, this),1);
						m_descPadIsOpen = true;
						//m_CurSelItemIndex = -1;//m_ItemData[i].uId;
						//nstrCurSelItemKey =nItemKey;
						isthis = true;
						this->addChild(sDescLayer,2,1001);
					}
				}
				else
				{
					if (!isthis) //������,�Ͳ�����
					{
						//nstrCurSelItemKey ="";
						inCurSelItemKey   = -1;
						//m_CurSelItemIndex = -1;
					}
					if (SkillNode)
						SkillNode->setColor(ax::Color3B::WHITE);           //Ĭ����ɫ
				}
			}
		}
	}

	//targive���
	isthis = false;
	_targiveItemData::iterator itt;
	for( itt = m_targiveItemData.begin(); itt != m_targiveItemData.end(); ++itt)
	{
		if (itt->second.uId>0 && itt->second.uId<MAX_ITEM)
		{////����ĳ���ܵļӳ�
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[itt->second.uId].GetID());
			std::string nItemKey = nKey;
			nCurColorlayer = (LayerColor *)ParentNode_tarGive->getChildByName(nItemKey);
			t_sprintf(nKey,"item_%u",Item[itt->second.uId].GetID());
			nItemKey = nKey;
			SkillNode = (Sprite *)ParentNode_tarGive->getChildByName(nItemKey);
			if  (nCurColorlayer)
			{
				//ccMessageBox("��ʼ���","��ʼ���");
				if (gettarGiveItem(nCurColorlayer,0).containsPoint(location))
				{//������
					//ccMessageBox("������","������");
					if (SkillNode)
					{
						//SkillNode->runAction(red);
						SkillNode->setColor(ax::Color3B::RED);
						itt->second.nRect  = gettarGiveItem(nCurColorlayer,0);
						//inCurSelItemKey    = itt->second.nindex;
						//its->second.nindex = -1;
						Layer * sDescLayer = KuiItemdesc::create(itt->second,this,CC_CALLBACK_1(KuiPlayerTrade::closeItemPadCallback, this),1);
						m_descPadIsOpen = true;
						//m_CurSelItemIndex = -1;//m_ItemData[i].uId;
						//nstrCurSelItemKey =nItemKey;

						isthis = true;
						this->addChild(sDescLayer,2,1001);
					}
				}
				else
				{
					if (!isthis) //������,�Ͳ�����
					{
						//nstrCurSelItemKey ="";
						//inCurSelItemKey   = -1;
						//m_CurSelItemIndex = -1;
					}
					if (SkillNode)
						SkillNode->setColor(ax::Color3B::WHITE);           //Ĭ����ɫ
				}
			}
		}
	}
	return true;
}

void KuiPlayerTrade::UpdateTradeOperData()
{
	int bLock = g_pCoreShell->GetGameData(GDI_TRADE_OPER_DATA, UTOD_IS_LOCKED, 0);
	//m_Ok.CheckButton(bLock);
	if(okConfirm)
	{
		if (bLock)
	       okConfirm->selected();
		else
			okConfirm->unselected();
	}
	if (!ptradeInfo)
	{
		ptradeInfo = Label::createWithTTF("",UI_GAME_FONT_DEFAULT,12);//Arial
		ptradeInfo->setColor(ax::Color3B::YELLOW);
		ptradeInfo->setPosition(ax::Vec2(120,m_size.height-190));
		ParentNode_ItemEx->addChild(ptradeInfo);
	}

	char nTempStr[64];
	ZeroMemory(nTempStr,sizeof(nTempStr));
	if (g_pCoreShell->GetGameData(GDI_TRADE_OPER_DATA, UTOD_IS_TRADING, 0))
	{//�ȴ��Է�����
		if (tradeConfirm)
		{
			tradeConfirm->selected();
			tradeConfirm->setEnabled(true);
		}
		//m_Trade.Enable(true);
		//m_Trade.CheckButton(true);
		//m_Info.SetText(STR_WAIT_TRADING, strlen(STR_WAIT_TRADING));
		//m_Info.SetTextColor(m_uWaitTradeMsgColor);
		t_sprintf(nTempStr,STR_WAIT_TRADING);
		if (ptradeInfo)
			ptradeInfo->setString(UTEXT(nTempStr,1).c_str());
	}
	else
	{
		int bOtherLock = g_pCoreShell->GetGameData(GDI_TRADE_OPER_DATA, UTOD_IS_OTHER_LOCKED, 0);
		//m_Trade.CheckButton(false);
		//m_Trade.Enable(bLock && bOtherLock);
		if (tradeConfirm)
		{
			tradeConfirm->unselected();
			tradeConfirm->setEnabled(bLock && bOtherLock);
		}
		//�Է��Ƿ�����
		t_sprintf(nTempStr,bOtherLock ? STR_OTHER_OK:STR_OTHER_NOT_OK);
		if (ptradeInfo)
			ptradeInfo->setString(UTEXT(nTempStr,1).c_str());
		//m_Info.SetText(bOtherLock ? STR_OTHER_OK : STR_OTHER_NOT_OK, -1);
		//m_Info.SetTextColor(bOtherLock ? m_uOtherLockMsgColor : m_uOtherNotLockMsgColor);
	}
	if (m_SelfMoneyBox)
	   m_SelfMoneyBox->setEnabled(!bLock);
	/*m_SelfItemsBox.Enable(!bLock);
	m_SelfMoney.Enable(!bLock);
	m_TakewithItemsBox.Enable(!bLock);
	m_AddMoney.Enable(!bLock);
	m_ReduceMoney.Enable(!bLock);
	m_SelfMoney.Enable(!bLock);*/
}

//�Է��Ķ���
void KuiPlayerTrade::UpdateTarGiveItem(KUiObjAtRegion* pObj, int bAdd)
{
	if (!pObj) return;
	if (pObj->Obj.uGenre == CGOG_MONEY)
	{//�Է���Ǯ�ı���
		if (m_OtherMoney)
		{
		  char nTempStr[64];
		  ZeroMemory(nTempStr,sizeof(nTempStr));
		  t_sprintf(nTempStr,"%u",pObj->Obj.uId);
		  m_OtherMoney->setString(nTempStr);
		}
	}
	else
	{//��Ʒ
			KUiDraggedObject Obj;
			Obj.uGenre = pObj->Obj.uGenre;
			Obj.uId    = pObj->Obj.uId;
			Obj.DataX  = pObj->Region.h;
			Obj.DataY  = pObj->Region.v;
			Obj.DataW  = pObj->Region.Width;
			Obj.DataH  = pObj->Region.Height;

			if  (Obj.uId<=0 || Obj.uId>=MAX_ITEM)
			{
				//ccMessageBox("��������","��Ʒ");
				return;
			}

			if (bAdd) //����
			{
				//m_ItemBox.AddObject(&Obj,1);
				if (pObj->Obj.uId)
				{
					m_targiveNum++;
				}

				AddtarGiveObject(&Obj,m_targiveNum);

			}
			else      //����
			{
				if (pObj->Obj.uId)
				{
					m_targiveNum--;
				}
				char nKey[32];
				t_sprintf(nKey,"item_%u_color",Item[Obj.uId].GetID());
				std::string nItemKey = nKey;
				ParentNode_tarGive->removeChildByName(nItemKey,true);
				t_sprintf(nKey,"item_%u",Item[Obj.uId].GetID());
				nItemKey = nKey;
				ParentNode_tarGive->removeChildByName(nItemKey,true);
				_targiveItemData::iterator it;
				for( it = m_targiveItemData.begin(); it != m_targiveItemData.end(); it++)
				{
					if (it->second.uId>0 && it->second.uId<MAX_ITEM && it->second.uId ==Obj.uId)
					{
						m_targiveItemData.erase(it->second.nindex);//
						break;
					}
				}
			}
	}
}

//���˵Ķ���
void KuiPlayerTrade::UpdateGiveItem(KUiObjAtRegion* pItem, int bAdd)
{
	if (pItem)
	{

		if (pItem->Obj.uGenre == CGOG_MONEY)
		{//�Լ���Ǯ
			m_nSelfTradeMoney = pItem->Obj.uId;
			//m_SelfMoney.SetIntText(pObj->Obj.uId);
			if (m_SelfMoneyBox)
			{
				char nTempStr[64];
				ZeroMemory(nTempStr,sizeof(nTempStr));
				t_sprintf(nTempStr,"%u",pItem->Obj.uId);
				m_SelfMoneyBox->setText(nTempStr);
			}
			return;
		}

		KUiDraggedObject Obj;
		Obj.uGenre = pItem->Obj.uGenre;
		Obj.uId    = pItem->Obj.uId;
		Obj.DataX  = pItem->Region.h;
		Obj.DataY  = pItem->Region.v;
		Obj.DataW  = pItem->Region.Width;
		Obj.DataH  = pItem->Region.Height;

		if  (Obj.uId<=0 || Obj.uId>=MAX_ITEM)
		{
			//ccMessageBox("��������","��Ʒ");
			return;
		}

		if (bAdd) //����
		{
			//m_ItemBox.AddObject(&Obj,1);
			if (pItem->Obj.uId)
			{
				m_giveNum++;
			}

			AddGiveObject(&Obj,m_giveNum);

		}
		else      //����
		{
			if (pItem->Obj.uId)
			{
				m_giveNum--;
			}
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[Obj.uId].GetID());
			std::string nItemKey = nKey;
			ParentNode_Give->removeChildByName(nItemKey,true);
			t_sprintf(nKey,"item_%u",Item[Obj.uId].GetID());
			nItemKey = nKey;
			ParentNode_Give->removeChildByName(nItemKey,true);
			_giveItemData::iterator it;
			for( it = m_giveItemData.begin(); it != m_giveItemData.end(); it++)
			{
				if (it->second.uId>0 && it->second.uId<MAX_ITEM && it->second.uId ==Obj.uId)
				{
					m_giveItemData.erase(it->second.nindex);
					break;
				}
			}
			//m_giveItemData.erase(nItemKey);
		}
	}
}

void KuiPlayerTrade::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
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
			//ccMessageBox("��������","��Ʒ");
			return;
		}

		if (bAdd) //����
		{
			//m_ItemBox.AddObject(&Obj,1);
			if (pItem->Obj.uId)
			{
				m_itemNum ++;
			}

			AddObject(&Obj,1);

		}
		else      //����
		{
			if (pItem->Obj.uId)
			{
				m_itemNum--;
			}
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
	else //����ȫ������Ʒ
		UpdateItemData();
}

void KuiPlayerTrade::UpdateItemData()
{
	char nTempStr[64];
	ZeroMemory(nTempStr,sizeof(nTempStr));
	m_nTakewithMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
	t_sprintf(nTempStr,"&d",m_nTakewithMoney);
	pMoneyLabel->setString(nTempStr);
// m_CurSelItemIndex = -1;
   nstrCurSelItemKey ="";
  // memset(m_ItemData,0,sizeof(KuiSellItemdObject) * 60);
   _ItemData::iterator it;
   for( it = m_ItemData.begin(); it != m_ItemData.end(); it++)
   {
	   if (it->second.uId>0 && it->second.uId<MAX_ITEM)
	   {////����ĳ���ܵļӳ�
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

	KUiObjAtRegion* pObjs = NULL;
	int nCount    = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 0); //�õ��������Ʒ������

	m_nItemNumObjects = nCount;

	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, (uintptr_t)pObjs, nCount);//���߳�ִ�У�nCountֵ����
		for (int i = 0; i < nCount; ++i)
		{
			KUiDraggedObject no;
			no.uGenre = pObjs[i].Obj.uGenre;
			no.uId    = pObjs[i].Obj.uId;
			no.DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
			no.DataY  = pObjs[i].Region.v;     //�����е����Yλ��
			no.DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
			no.DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
			if (Item && pObjs[i].Obj.uId>0 && pObjs[i].Obj.uId<MAX_ITEM)
			{
				char nItemSprPath[256];
				ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
				if  (Item[pObjs[i].Obj.uId].GetImagePath())
					t_sprintf(nItemSprPath,Item[pObjs[i].Obj.uId].GetImagePath());
				else
					t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");

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
						{//����װ�� �ɵ�����Ʒ
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
					//��ʼ������
					m_ItemData[nKey].uGenre = pObjs[i].Obj.uGenre;
					m_ItemData[nKey].uId    = pObjs[i].Obj.uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
					m_ItemData[nKey].DataY  = pObjs[i].Region.v;     //�����е����Yλ��
					m_ItemData[nKey].DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
					m_ItemData[nKey].DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��

					Color4B color(112, 128, 144, 150);//112, 128, 144
					//t_sprintf(nItemKey,"color_%d",i+1);
					strcat(nItemKey,"_color");
					nKey =nItemKey;
					LayerColor *bgcolorLayer = (LayerColor *)ParentNode_Item->getChildByName(nKey);
					if (!bgcolorLayer)
					{
						bgcolorLayer = LayerColor::create(color);//��ɫ��
					    //bgcolorLayer->setPosition(ax::Vec2::ZERO);
						bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
						Size bgsize;
						bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
						bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					    bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
						int nikey = std::hash<std::string>{}(nKey);
bgcolorLayer->setTag(nikey);
						ParentNode_Item->addChild(bgcolorLayer,0);
					}

					int nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
					int nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;

					nItemSpr->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));
					//��ɫ��
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
					//if (eProp == IIEP_NORMAL) //����
					//else
					if (eProp == IIEP_NOT_USEABLE)  //������
					{
						ax::Color3B color_(128, 0, 0);
						bgcolorLayer->setColor(color_);
					}
					//else if (eProp == IIEP_SPECIAL) //����������Ʒ
				}
			}
		}
		free(pObjs); //�ͷ��ڴ�
		pObjs = NULL; //���
	}
}

void KuiPlayerTrade::OnModififyMoney()
{
	m_bAdjustingMoney = 1;
	char nTempStr[64];
	ZeroMemory(nTempStr,sizeof(nTempStr));
	t_sprintf(nTempStr,m_SelfMoneyBox->getText());

	int nNewSelfTradeMoney = g_Atoui(nTempStr);//m_SelfMoney.GetIntNumber();
	if (nNewSelfTradeMoney < 0)
		nNewSelfTradeMoney = 0;
	if (nNewSelfTradeMoney > m_nTakewithMoney + m_nSelfTradeMoney)
	{
		nNewSelfTradeMoney = m_nTakewithMoney + m_nSelfTradeMoney;
		//m_SelfMoney.SetIntText(nNewSelfTradeMoney);
		t_sprintf(nTempStr,"%u",nNewSelfTradeMoney);
		m_SelfMoneyBox->setText(nTempStr);
	}

	m_nTakewithMoney -= nNewSelfTradeMoney - m_nSelfTradeMoney;
	m_nSelfTradeMoney = nNewSelfTradeMoney;
	//m_TakewithMoney.SetMoneyText(m_nTakewithMoney);
	t_sprintf(nTempStr,"%u",m_nTakewithMoney);
	pMoneyLabel->setString(nTempStr);
}

void KuiPlayerTrade::OnAdjustedMoney()
{
	OnModififyMoney();
	m_bAdjustingMoney = 0;
	if (g_pCoreShell)
	{
		KUiObjAtRegion	Obj = {0};
		Obj.Obj.uGenre = CGOG_MONEY;
		Obj.Obj.uId = m_nSelfTradeMoney;
		g_pCoreShell->OperationRequest(GOI_TRADE_DESIRE_ITEM,(uintptr_t)&Obj, 0);
	}
}

void KuiPlayerTrade::update(float delta)
{
	//ccMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
		if (m_bAdjustingMoney)
		{
			if ((++m_bAdjustingMoney) == SEND_MONEY_CHANGE_MSG_DELAY)
				OnAdjustedMoney();
		}

	   if (g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, GET_BOXEX_OPEN, 0))
		   m_nIstrue = true;
		   //	m_ItemEX.SetOpenClose(TRUE);
		   else
			   m_nIstrue = false;

	  //���µ�������
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

//�Է������
int KuiPlayerTrade::AddtarGiveObject(KUiDraggedObject* pObject, int nindex)
{
	if (!pObject) return false;
	for (int i = 0; i < 1; ++i)
	{
		KUiDraggedObject no;
		no.uGenre = pObject->uGenre;
		no.uId    = pObject->uId;
		no.DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
		no.DataY  = pObject->DataY;     //�����е����Yλ��
		no.DataW  = pObject->DataW; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
		no.DataH  = pObject->DataH;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
		if (Item && pObject->uId>0 && pObject->uId<MAX_ITEM)
		{
			char nItemSprPath[256];
			ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
			if  (Item[pObject->uId].GetImagePath())
				t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
			else
				t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");
			if (nItemSprPath[0])
			{
				//t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
				g_StrLower(nItemSprPath);
				int m_nWidth,m_nHeight,nFrams;
				Texture2D *bgCur = NULL;
				SPRFRAMSINFO nSprInfo;
				ZeroMemory(&nSprInfo,sizeof(nSprInfo));
				bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
				if (bgCur==NULL)
					return false;
				char nItemKey[32];
				ZeroMemory(&nItemKey,sizeof(nItemKey));
				t_sprintf(nItemKey,"item_%u",Item[pObject->uId].GetID());
				std::string nKey =nItemKey;
				Sprite *nItemSpr = (Sprite *)ParentNode_tarGive->getChildByName(nKey);
				if (!nItemSpr)
				{
					nItemSpr = Sprite::createWithTexture(bgCur);
					//nItemSpr->setAnchorPoint(ax::Vec2(0,0));
					int nikey = std::hash<std::string>{}(nKey);
nItemSpr->setTag(nikey);
					ParentNode_tarGive->addChild(nItemSpr,1);

					if  (Item[pObject->uId].GetGenre()!=item_equip && Item[pObject->uId].IsStack())
					{//����װ�� �ɵ�����Ʒ
						char stack[32];
						t_sprintf(stack,"%d",Item[pObject->uId].GetStackNum());
						Label *stuckCountlabel = Label::createWithTTF(stack,UI_GAME_FONT_DEFAULT,14);
						stuckCountlabel->setColor(ax::Color3B::YELLOW);
						stuckCountlabel->setAnchorPoint(ax::Vec2(0,0));
						stuckCountlabel->setPosition(ax::Vec2(0,0));
						int nikey = std::hash<std::string>{}("stack");
stuckCountlabel->setTag(nikey);
						nItemSpr->addChild(stuckCountlabel,1);
					}
				}
				//��ʼ������
				m_targiveItemData[nindex].uGenre = pObject->uGenre;
				m_targiveItemData[nindex].uId    = pObject->uId;
				m_targiveItemData[nindex].nindex = nindex;
				m_targiveItemData[nindex].DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
				m_targiveItemData[nindex].DataY  = pObject->DataY;     //�����е����Yλ��
				m_targiveItemData[nindex].DataW  = pObject->DataW;     //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
				m_targiveItemData[nindex].DataH  = pObject->DataH;     //��Ʒ�Ŀ��(ռ���˶��ٸ�����)
				m_targiveItemData[nindex].nGenkind = UOC_OTHER_TO_BE_TRADE;  //�Լ����

				Color4B color(112, 128, 144, 150);//112, 128, 144
				//t_sprintf(nItemKey,"color_%d",i+1);
				strcat(nItemKey,"_color");
				nKey =nItemKey;
				LayerColor *bgcolorLayer = (LayerColor *)ParentNode_tarGive->getChildByName(nKey);
				if (!bgcolorLayer)
				{
					bgcolorLayer = LayerColor::create(color);//��ɫ��
					//bgcolorLayer->setPosition(ax::Vec2::ZERO);
					bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
					Size bgsize;
					bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
					bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
					int nikey = std::hash<std::string>{}(nKey);
bgcolorLayer->setTag(nikey);
					ParentNode_tarGive->addChild(bgcolorLayer,0);
				}

				int nCurX = m_StartPos_t.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
				int nCurY = m_StartPos_t.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;

				nItemSpr->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));
				//��ɫ��
				nCurX = m_StartPos_t.x+no.DataX*ITEM_CELL_SIZE+1;
				nCurY = m_StartPos_t.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE-1;
				bgcolorLayer->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));

				KUiObjAtContRegion	Obj;
				Obj.Obj.uGenre = no.uGenre;
				Obj.Obj.uId    = no.uId;
				Obj.Region.h   = Obj.Region.v = 0;
				Obj.Region.Width = Obj.Region.Height = 0;
				Obj.nContainer = UOC_ITEM_GIVE;

				ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (uintptr_t)&Obj, 0);
				//if (eProp == IIEP_NORMAL) //����
				//else
				if (eProp == IIEP_NOT_USEABLE)  //������
				{
					ax::Color3B color_(128, 0, 0);
					bgcolorLayer->setColor(color_);
				}
				//else if (eProp == IIEP_SPECIAL) //����������Ʒ
			}
		}
	}
	return true;
}

//�Լ����
int KuiPlayerTrade::AddGiveObject(KUiDraggedObject* pObject, int nindex)
{
	if (!pObject) return false;
	for (int i = 0; i < 1; ++i)
	{
		KUiDraggedObject no;
		no.uGenre = pObject->uGenre;
		no.uId    = pObject->uId;
		no.DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
		no.DataY  = pObject->DataY;     //�����е����Yλ��
		no.DataW  = pObject->DataW; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
		no.DataH  = pObject->DataH;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
		if (Item && pObject->uId>0 && pObject->uId<MAX_ITEM)
		{
			char nItemSprPath[256];
			ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
			if  (Item[pObject->uId].GetImagePath())
				t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
			else
				t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");
			if (nItemSprPath[0])
			{
				//t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
				g_StrLower(nItemSprPath);
				int m_nWidth,m_nHeight,nFrams;
				Texture2D *bgCur = NULL;
				SPRFRAMSINFO nSprInfo;
				ZeroMemory(&nSprInfo,sizeof(nSprInfo));
				bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
				if (bgCur==NULL)
					return false;
				char nItemKey[32];
				ZeroMemory(&nItemKey,sizeof(nItemKey));
				t_sprintf(nItemKey,"item_%u",Item[pObject->uId].GetID());
				std::string nKey =nItemKey;
				Sprite *nItemSpr = (Sprite *)ParentNode_Give->getChildByName(nKey);
				if (!nItemSpr)
				{
					nItemSpr = Sprite::createWithTexture(bgCur);
					//nItemSpr->setAnchorPoint(ax::Vec2(0,0));
					int nikey = std::hash<std::string>{}(nKey);
nItemSpr->setTag(nikey);
					ParentNode_Give->addChild(nItemSpr,1);

					if  (Item[pObject->uId].GetGenre()!=item_equip && Item[pObject->uId].IsStack())
					{//����װ�� �ɵ�����Ʒ
						char stack[32];
						t_sprintf(stack,"%d",Item[pObject->uId].GetStackNum());
						Label *stuckCountlabel = Label::createWithTTF(stack,UI_GAME_FONT_DEFAULT,14);
						stuckCountlabel->setColor(ax::Color3B::YELLOW);
						stuckCountlabel->setAnchorPoint(ax::Vec2(0,0));
						stuckCountlabel->setPosition(ax::Vec2(0,0));
						int nikey = std::hash<std::string>{}("stack");
stuckCountlabel->setTag(nikey);
						nItemSpr->addChild(stuckCountlabel,1);
					}
				}
				//��ʼ������
				m_giveItemData[nindex].uGenre = pObject->uGenre;
				m_giveItemData[nindex].uId    = pObject->uId;
				m_giveItemData[nindex].nindex = nindex;
				m_giveItemData[nindex].DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
				m_giveItemData[nindex].DataY  = pObject->DataY;     //�����е����Yλ��
				m_giveItemData[nindex].DataW  = pObject->DataW;     //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
				m_giveItemData[nindex].DataH  = pObject->DataH;     //��Ʒ�Ŀ��(ռ���˶��ٸ�����)
				m_giveItemData[nindex].nGenkind = UOC_TO_BE_TRADE;  //�Լ����

				Color4B color(112, 128, 144, 150);//112, 128, 144
				//t_sprintf(nItemKey,"color_%d",i+1);
				strcat(nItemKey,"_color");
				nKey =nItemKey;
				LayerColor *bgcolorLayer = (LayerColor *)ParentNode_Give->getChildByName(nKey);
				if (!bgcolorLayer)
				{
					bgcolorLayer = LayerColor::create(color);//��ɫ��
					//bgcolorLayer->setPosition(ax::Vec2::ZERO);
					bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
					Size bgsize;
					bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
					bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
					int nikey = std::hash<std::string>{}(nKey);
bgcolorLayer->setTag(nikey);
					ParentNode_Give->addChild(bgcolorLayer,0);
				}

				int nCurX = m_StartPos_g.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
				int nCurY = m_StartPos_g.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;

				nItemSpr->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));
				//��ɫ��
				nCurX = m_StartPos_g.x+no.DataX*ITEM_CELL_SIZE+1;
				nCurY = m_StartPos_g.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE-1;
				bgcolorLayer->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));

				KUiObjAtContRegion	Obj;
				Obj.Obj.uGenre = no.uGenre;
				Obj.Obj.uId    = no.uId;
				Obj.Region.h   = Obj.Region.v = 0;
				Obj.Region.Width = Obj.Region.Height = 0;
				Obj.nContainer = UOC_ITEM_GIVE;

				ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (uintptr_t)&Obj, 0);
				//if (eProp == IIEP_NORMAL) //����
				//else
				if (eProp == IIEP_NOT_USEABLE)  //������
				{
					ax::Color3B color_(128, 0, 0);
					bgcolorLayer->setColor(color_);
				}
				//else if (eProp == IIEP_SPECIAL) //����������Ʒ
			}
		}
	}
	return true;
}
//������Ʒ
int KuiPlayerTrade::AddObject(KUiDraggedObject* pObject, int nCount)
{
	if (!pObject) return false;

	for (int i = 0; i < nCount; ++i)
	{
			KUiDraggedObject no;
			no.uGenre = pObject->uGenre;
			no.uId    = pObject->uId;
			no.DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
			no.DataY  = pObject->DataY;     //�����е����Yλ��
			no.DataW  = pObject->DataW; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
			no.DataH  = pObject->DataH;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
			if (Item && pObject->uId>0 && pObject->uId<MAX_ITEM)
			{
				char nItemSprPath[256];
				ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
				if  (Item[pObject->uId].GetImagePath())
					t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
				else
					t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");
				if (nItemSprPath[0])
				{
					//t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
					g_StrLower(nItemSprPath);
					int m_nWidth,m_nHeight,nFrams;
					Texture2D *bgCur = NULL;
					SPRFRAMSINFO nSprInfo;
					ZeroMemory(&nSprInfo,sizeof(nSprInfo));
					bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
					if (bgCur==NULL)
						return false;
					char nItemKey[32];
					ZeroMemory(&nItemKey,sizeof(nItemKey));
					t_sprintf(nItemKey,"item_%u",Item[pObject->uId].GetID());
					std::string nKey =nItemKey;
					Sprite *nItemSpr = (Sprite *)ParentNode_Item->getChildByName(nKey);
					if (!nItemSpr)
					{
						nItemSpr = Sprite::createWithTexture(bgCur);
						//nItemSpr->setAnchorPoint(ax::Vec2(0,0));
						int nikey = std::hash<std::string>{}(nKey);
nItemSpr->setTag(nikey);
						ParentNode_Item->addChild(nItemSpr,1);

						if  (Item[pObject->uId].GetGenre()!=item_equip && Item[pObject->uId].IsStack())
						{//����װ�� �ɵ�����Ʒ
							char stack[32];
							t_sprintf(stack,"%d",Item[pObject->uId].GetStackNum());
							Label *stuckCountlabel = Label::createWithTTF(stack,UI_GAME_FONT_DEFAULT,14);
							stuckCountlabel->setColor(ax::Color3B::YELLOW);
							stuckCountlabel->setAnchorPoint(ax::Vec2(0,0));
							stuckCountlabel->setPosition(ax::Vec2(0,0));
							int nikey = std::hash<std::string>{}("stack");
stuckCountlabel->setTag(nikey);
							nItemSpr->addChild(stuckCountlabel,1);
						}
					}
					//��ʼ������
					m_ItemData[nKey].uGenre = pObject->uGenre;
					m_ItemData[nKey].uId    = pObject->uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
					m_ItemData[nKey].DataY  = pObject->DataY;     //�����е����Yλ��
					m_ItemData[nKey].DataW  = pObject->DataW; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
					m_ItemData[nKey].DataH  = pObject->DataH;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��

					Color4B color(112, 128, 144, 150);//112, 128, 144
					//t_sprintf(nItemKey,"color_%d",i+1);
					strcat(nItemKey,"_color");
					nKey =nItemKey;
					LayerColor *bgcolorLayer = (LayerColor *)ParentNode_Item->getChildByName(nKey);
					if (!bgcolorLayer)
					{
						bgcolorLayer = LayerColor::create(color);//��ɫ��
					    //bgcolorLayer->setPosition(ax::Vec2::ZERO);
						bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
						Size bgsize;
						bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
						bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					    bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
						int nikey = std::hash<std::string>{}(nKey);
bgcolorLayer->setTag(nikey);
						ParentNode_Item->addChild(bgcolorLayer,0);
					}

					int nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
					int nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;

					nItemSpr->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));
					//��ɫ��
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
					//if (eProp == IIEP_NORMAL) //����
					//else
					if (eProp == IIEP_NOT_USEABLE)  //������
					{
						ax::Color3B color_(128, 0, 0);
						bgcolorLayer->setColor(color_);
					}
					//else if (eProp == IIEP_SPECIAL) //����������Ʒ
				}
			}
	}
	return true;
}

int KuiPlayerTrade::FilterTextColor(char* pMsgBuff, unsigned short nMsgLength)
{
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_ENTER);          //��ֹ�س�����
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_COLOR);          //��ֹ��ɫ����
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_COLOR_RESTORE);  //��ֹ��ɫ��ԭ
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_BORDER_COLOR);   //��ֹ�������
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_BORDER_RESTORE); //��ֹ�������
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_INLINE_PIC);     //��ֹ��ͼ����
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_LINK);           //��ֹ��������
	//	return TEncodeText(pMsgBuff, nMsgLength);
	return  nMsgLength;
}
