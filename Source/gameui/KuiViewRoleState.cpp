#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiViewRoleState.h"
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
// ---> �����ؼ���UIEP_*�Լ��ɽ�����Ʒ�����͵Ķ�Ӧ��ϵ
static struct UE_CTRL_MAP_VIEW
{
	int				nPosition;
	const char*		pIniSection;
	int             nCellx;    //��ռ�ü�������
	int             nCelly;    //��ռ�ü�������
	float           nOffsetX;  //������е�λ��ƫ�ƹ̶���
	float           nOffsetY;  //������е�λ��ƫ�ƹ̶���
}ViewCtrlItemMap[_ITEM_COUNT] =
{
	{ UIEP_HEAD,		"Cap"		,2,2,119,51},	//0װ��-ñ��
	{ UIEP_HAND,		"Weapon"	,2,4,217,122},	//1װ��-����
	{ UIEP_NECK,		"Necklace"	,2,1,218,83},	//2װ��-����
	{ UIEP_FINESSE,		"Bangle"	,1,2,46,95},	//3װ��-����
	{ UIEP_BODY,		"Cloth"		,2,3,119,111},	//4װ��-�·�
	{ UIEP_WAIST,		"Sash"		,2,1,119,199},	//5װ��-����
	{ UIEP_FINGER1,		"Ring1"		,1,1,46,153},	//6װ��-��ָ
	{ UIEP_FINGER2,		"Ring2"		,1,1,46,183},	//7װ��-��ָ
	{ UIEP_WAIST_DECOR,	"Pendant"	,1,2,46,216},	//8װ��-��׺
	{ UIEP_FOOT,		"Shoes"		,2,2,217,239},	//9װ��-Ь��
	{ UIEP_HORSE,		"Horse"		,2,3,119,240},	//10װ��-��
	{ UIEP_MASK,		"Mask"		,1,1,46,51},	//11װ��-���	// mat na
	{ UIEP_PIFENG,	    "Mantle"	,1,1,46,305},	//12����
	{ UIEP_YINJIAN,		"Signet"	,1,1,213,297},	//13ӡ��
	{ UIEP_SHIPING,	    "Shipin"	,1,1,247,297},	//14��Ʒ
};

KuiViewRoleState::KuiViewRoleState()
{
	isOpen = false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	colorLayer        = NULL;
	red               = NULL;
	ItemBoxNode       = NULL;
	m_nMoney = 0;
	m_nXu = 0;
	m_nNumObjects = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	m_descPadIsOpen =false;
	m_CurSelItemIndex = 0;
	for (int i=0;i<_ITEM_COUNT;i++)
	    m_itemIndexBox[i]=0;

	pRoleName=NULL;
	pRoleTitle=NULL;
	//pRoleTitle_1=NULL;
	pPk=NULL;

	pZhuanSheng=NULL;
	pZhuanShengLevel=NULL;

	__RemainPoint = 0;
	ncurDest = NULL;
}

KuiViewRoleState::~KuiViewRoleState()
{
	isOpen = false;
	colorLayer        = NULL;
	if (red)
	   red->release();
	//red               = NULL;
}

//����ȷ��������
KuiViewRoleState * KuiViewRoleState::create(KUiPlayerItem* pDest,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiViewRoleState * popLayer = KuiViewRoleState::create();
	popLayer->ncurDest = pDest;

	//memcpy(&m_pSelf->m_ZhuangBeiPad.m_UseData, m_UseData, sizeof(KUiPlayerItem));     //װ����Ϣ
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;

	popLayer->UpdateData(pDest);
	return popLayer;
}

//��ʼ��
bool KuiViewRoleState::init()
{
	if ( !Layer::init() ){
		return false;
	}

	//����֡����
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");

	//���ģ̬����
	Size winSize = ax::Director::getInstance()->getWinSize();
	/*Color4B color(112, 128, 144, 150);
	LayerColor * colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	//colorLayer->setContentSize(winSize);//���úʹ��ڵĴ�С
	this->addChild(colorLayer);
	*/
	/*char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\Spr\\Ui3\\���\\���2.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;*/
	Sprite * spriteBox = Sprite::create("ui/state/view_other.png");
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

void KuiViewRoleState::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*ItemBoxNode = Node::create();
	ItemBoxNode->setPosition(ax::Vec2(0,0));
	ItemBoxNode->setAnchorPoint(ax::Vec2(0,0));
	ParentNode_Team->addChild(ItemBoxNode,2);*/
	//Left=24
	//Top=72
	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;

	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);  //���úʹ��ڵĴ�С
	ParentNode_Team->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	red->retain();
	float nfontsize =12;
	float nScale = nfontsize/16;
	//��ʼ�����ı���Ϣ
	pRoleName = Label::createWithTTF(UTEXT("���Բ���1",1).c_str(),UI_GAME_FONT_DEFAULT, nfontsize);//Arial
	pRoleName->setAnchorPoint(ax::Vec2(0,0));
	pRoleName->setPosition(ax::Vec2(50,m_size.height/2+142));
	ParentNode_Team->addChild(pRoleName);

	pRoleTitle = Label::createWithTTF(UTEXT("���Բ���2",1).c_str(),UI_GAME_FONT_SONG, nfontsize);   //Arial
	pRoleTitle->setAnchorPoint(ax::Vec2(0,0));
	pRoleTitle->setPosition(ax::Vec2(50,m_size.height/2+124));
	pRoleTitle->setColor(ax::Color3B::GREEN);
	ParentNode_Team->addChild(pRoleTitle);

	/*pRoleTitle_1 = Label::createWithTTF(UTEXT("���Բ���3",1).c_str(),UI_GAME_FONT_SONG,nfontsize);   //Arial
	pRoleTitle_1->setAnchorPoint(ax::Vec2(0,0));
	pRoleTitle_1->setPosition(ax::Vec2(360,m_size.height/2+140));
	ParentNode_Team->addChild(pRoleTitle_1);*/

	pPk   = Label::createWithTTF("0",UI_GAME_FONT_NUMBER, 10);
	pPk->setAnchorPoint(ax::Vec2(0,0));
	pPk->setPosition(ax::Vec2(m_size.width-55,m_size.height/2+138));
	ParentNode_Team->addChild(pPk);

	pZhuanSheng = Label::createWithTTF(UTEXT("ת������",1).c_str(),UI_GAME_FONT_SONG, 12);   //Arial
	pZhuanSheng->setAnchorPoint(ax::Vec2(0,0));
	pZhuanSheng->setPosition(ax::Vec2(m_size.width-100,m_size.height/2+124));
	ParentNode_Team->addChild(pZhuanSheng);

	pZhuanShengLevel = Label::createWithTTF(UTEXT("ת���ȼ�",1).c_str(),UI_GAME_FONT_SONG, 12);   //Arial
	pZhuanShengLevel->setAnchorPoint(ax::Vec2(0,0));
	pZhuanShengLevel->setPosition(ax::Vec2(m_size.width-100,m_size.height/2+108));
	ParentNode_Team->addChild(pZhuanShengLevel);

}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiViewRoleState::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	//Sprite * sprite_tips_btn_confirm_normal =  Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_normal);
	//SpriteFrame * f_tips_btn_confirm_select = sfCache->spriteFrameByName("btn_confirm_select.png");
	//Sprite * sprite_tips_btn_confirm_select =  Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_select);
	//"\\spr\\UI3\\������\\�ر�.spr"
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui3\\activityguide\\closebutton.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
    Sprite * sprite_tips_btn_confirm_normal = Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_tips_btn_confirm_select = Sprite::createWithTexture(bgCur);
	MenuItemSprite * itemConfirm   = MenuItemSprite::create(sprite_tips_btn_confirm_normal, sprite_tips_btn_confirm_select, CC_CALLBACK_1(KuiViewRoleState::buttonCallBackFunc, this));
	itemConfirm->setPosition(ax::Vec2(0,0));

	 Menu * menu = Menu::create(itemConfirm,NULL);
	 menu->setPosition(ax::Vec2(m_size.width/2,13));
	 ParentNode_Team->addChild(menu);          //�ӵ�������
	//Point bgSpritePoint = ParentNode_Team->getPosition();
}


//ִ���ϲ����Ļص��������رյ�����
void KuiViewRoleState::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node = dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiViewRoleState::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiViewRoleState::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-128,true);
}


void KuiViewRoleState::UpdateAllEquips()
{
	if  (!g_GameWorld || !g_pCoreShell) return;

	KUiObjAtRegion	Equips[_ITEM_COUNT];
	int nCount = g_pCoreShell->GetGameData(GDI_PARADE_EQUIPMENT, (uintptr_t)&Equips, 0);

	int	i;

	//for (i = 0; i < _ITEM_COUNT; ++i)
		//m_EquipBox[i].Celar();

	for (i = 0; i < nCount; ++i)
	{
		if (Equips[i].Obj.uGenre != CGOG_NOTHING)
			UpdateEquip(&Equips[i], true);  //���ӻ����
		else
		    UpdateEquip(&Equips[i], false); //ɾ���������
	}
}

void KuiViewRoleState::addpicBox(int index,int nItemIndx,bool isAdd)
{
	char nKey[32];
	sprintf(nKey,"%s",ViewCtrlItemMap[index].pIniSection);
	std::string m_ItemBoxKey = nKey;
	sprintf(nKey,"%s_color",ViewCtrlItemMap[index].pIniSection);
	std::string m_colorbgKey = nKey;//��ɫ���key
	if  (isAdd)
	{//����
		if (nItemIndx<=0 || nItemIndx>=MAX_ITEM) return;
		if (ParentNode_Team)
		{
			char nItemSprPath[256];
			ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
			Texture2D *bgCur = NULL;
			int nItemW = Item[nItemIndx].GetWidth()*ITEM_CELL_SIZE;
			int nItemH = Item[nItemIndx].GetHeight()*ITEM_CELL_SIZE;
			m_itemIndexBox[index]=nItemIndx;
			if  (Item[nItemIndx].GetImagePath())
			{
				sprintf(nItemSprPath, "%s", Item[nItemIndx].GetImagePath());
			}
			else
				sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");

			if  (nItemSprPath[0])
			{
				//sprintf(nItemSprPath,Item[nItemIndx].GetImagePath());
				g_StrLower(nItemSprPath);
				int m_nWidth,m_nHeight,nFrams;
				SPRFRAMSINFO nSprInfo;
				ZeroMemory(&nSprInfo,sizeof(nSprInfo));
				bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
			}

			if (bgCur==NULL)
				return;

		   Sprite * tempSpr = (Sprite *)ParentNode_Team->getChildByName(m_ItemBoxKey);
		   LayerColor *bgcolorLayer = (LayerColor *)ParentNode_Team->getChildByName(m_colorbgKey);

		   Size bgsize;
		   bgsize.width  = ViewCtrlItemMap[index].nCellx*ITEM_CELL_SIZE;
		   bgsize.height = ViewCtrlItemMap[index].nCelly*ITEM_CELL_SIZE;
		   Size nTempSize = bgCur->getContentSize();
		   float tempOffsetY = 55;
		   if (!tempSpr)
		   {
			   tempSpr =Sprite::createWithTexture(bgCur);
			   tempSpr->setPosition(ax::Vec2(0,0));
			   tempSpr->setAnchorPoint(ax::Vec2(0,0));
               int nikey = std::hash<std::string>{}(m_ItemBoxKey);
			   tempSpr->setTag(nikey);
			   //tempSpr->setPosition(ax::Vec2(m_size.width/2+CtrlItemMap[index].nOffsetX+bgsize.width/2-nItemW/2,m_size.height-CtrlItemMap[index].nOffsetY));
			   ParentNode_Team->addChild(tempSpr,2);

			   if (!bgcolorLayer)
			   {
				   Color4B color(112, 128, 144, 150);
				   bgcolorLayer = LayerColor::create(color);//��ɫ��
				   //bgcolorLayer->setPosition(ax::Vec2::ZERO);
				   bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));

				   bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
                   int nikey = std::hash<std::string>{}(m_colorbgKey);
				   bgcolorLayer->setTag(nikey);
				   bgcolorLayer->setPosition(ax::Vec2(0+ViewCtrlItemMap[index].nOffsetX,m_size.height-ViewCtrlItemMap[index].nOffsetY-bgsize.height-tempOffsetY));
				   ParentNode_Team->addChild(bgcolorLayer,1);
			   }
			 //  return;
		   }
		   //tempSpr->setContentSize(nTempSize);
		   Rect nRectZero = ax::Rect(0,0,0,0);
		   nRectZero.size = bgCur->getContentSize();
		   tempSpr->setTexture(bgCur);
		   tempSpr->setTextureRect(nRectZero,false,nRectZero.size);
		   tempSpr->setPosition(ax::Vec2(0+ViewCtrlItemMap[index].nOffsetX+bgsize.width/2-nTempSize.width/2,m_size.height-ViewCtrlItemMap[index].nOffsetY-(bgsize.height+nTempSize.height)/2-tempOffsetY));

		   //��ʼ������ñ�����ɫ
		   KUiObjAtContRegion	Obj;
		   Obj.Obj.uGenre = CGOG_ITEM;
		   Obj.Obj.uId    = nItemIndx;
		   Obj.Region.h     = Obj.Region.v = 0;
		   Obj.Region.Width = Obj.Region.Height = 0;
		   Obj.nContainer   = UOC_EQUIPTMENT;

		   ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
			   GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (uintptr_t)&Obj, 0);
		   //if (eProp == IIEP_NORMAL) //����
		   //else
		   if (eProp == IIEP_NOT_USEABLE)  //������
		   {
			   ax::Color3B color_(128, 0, 0);
			   if (bgcolorLayer)
			      bgcolorLayer->setColor(color_);
		   }
		}
	}
	else
	{//ɾ��
		if (ParentNode_Team)
		{
			m_itemIndexBox[index]=0;
			ParentNode_Team->removeChildByName(m_ItemBoxKey);
			ParentNode_Team->removeChildByName(m_colorbgKey);
		}
	}
}
//--------------------------------------------------------------------------
//	���ܣ�װ���仯����
//--------------------------------------------------------------------------
void KuiViewRoleState::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (Item && pEquip)
	{
		for (int i = 0; i < _ITEM_COUNT; ++i)
		{
			if (ViewCtrlItemMap[i].nPosition == pEquip->Region.v)
			{//�����������
				if (bAdd)//����
					//m_EquipBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
					//pEquip->Region.Width, pEquip->Region.Height);
					//messageBox(Item[pEquip->Obj.uId].GetImagePath(),"����");
					addpicBox(i,pEquip->Obj.uId,true);
				else//�Ƴ�
					addpicBox(i,pEquip->Obj.uId,false);
					//messageBox(Item[pEquip->Obj.uId].GetImagePath(),"�Ƴ�");
					//m_EquipBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);

				break;
			}
		}
	}
}

Rect KuiViewRoleState::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();
	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y

	return rc;
}
//��������ccTouchBegan������true
bool KuiViewRoleState::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	Sprite * itemBoxSpr =NULL;
	bool isthis=false;
	for (int i = 0; i < _ITEM_COUNT; ++i)
	{
		char nKey[32];
		sprintf(nKey,"%s",ViewCtrlItemMap[i].pIniSection);
		std::string m_ItemBoxKey = nKey;
		sprintf(nKey,"%s_color",ViewCtrlItemMap[i].pIniSection);
		std::string m_colorbgKey = nKey;//��ɫ���key
		itemBoxSpr = (Sprite *)ParentNode_Team->getChildByName(m_ItemBoxKey);
		LayerColor *colorBoxbg = (LayerColor *)ParentNode_Team->getChildByName(m_colorbgKey);
		//messageBox("�����ʼ","�����ʼ");
		if  (itemBoxSpr && colorBoxbg)
		{
			if (getRect(colorBoxbg,i+1).containsPoint(location))
			{//������
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
				Layer * sDescLayer = KuiItemdesc::create(second,this,CC_CALLBACK_1(KuiViewRoleState::closedescPadCallback, this),0);
				m_descPadIsOpen = true;
				m_CurSelItemIndex = m_itemIndexBox[i];//m_ItemData[i].uId;
				//nstrCurSelItemKey =nItemKey;
				isthis = true;
				this->addChild(sDescLayer,2,1001);
			}
			else
			{
				if (!isthis)
				  m_CurSelItemIndex =0;

				itemBoxSpr->setColor(ax::Color3B::WHITE);           //Ĭ����ɫ
			}
		}
	}
	return true;
}

void KuiViewRoleState::closedescPadCallback(Node *pNode)
{
	m_descPadIsOpen = false;
}

void KuiViewRoleState::UpdateData(KUiPlayerItem* pDest)
{
	if (pDest == NULL)
		return;

	UpdateBaseData(pDest);

	UpdateAllEquips();
}

void KuiViewRoleState::UpdateBaseData(KUiPlayerItem* pDest)
{
	KUiPlayerBaseInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
	if (g_pCoreShell)
	    g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (intptr_t)&Info,pDest->uId);

	pRoleName->setString(UTEXT(Info.Name,1).c_str());
	pRoleTitle->setString(UTEXT(Info.Title,1).c_str());
	//pRoleTitle_1->setString(UTEXT(Info.Agname,1).c_str());
	//m_Agname.SetText(Info.Agname);  //�º�
	//m_Name  .SetText(Info.Name);    //����
	//m_Title .SetText(Info.Title);   //�ƺ�
}

void KuiViewRoleState::BreatheUpdateInfo()
{
	/*
	KUiPlayerRuntimeInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
	if (g_pCoreShell)
	    g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (intptr_t)&Info, 0);

	char nTempS4tr[64];
	ZeroMemory(nTempS4tr,sizeof(nTempS4tr));
	sprintf(nTempS4tr,"%d/%d",Info.nLife,Info.nLifeFull);
	pLife->setString(nTempS4tr);
	sprintf(nTempS4tr,"%d/%d",Info.nMana,Info.nManaFull);
	pMana->setString(nTempS4tr);
	sprintf(nTempS4tr,"%d/%d",Info.nStamina,Info.nStaminaFull);
	pTili->setString(nTempS4tr);
	sprintf(nTempS4tr,"%d/%d",Info.nExperience,Info.nExperienceFull);
	pExp->setString(nTempS4tr);
	*/
	//UiNewStatus* pStatus = UiNewStatus::GetIfVisible();
	//if (pStatus)
	//	UpdateRuntimeInfo(&Info);
	//pStatus=NULL;
}


void  KuiViewRoleState::BreathAttributeUpdate()
{
	KUiPlayerAttribute	Info;
	memset(&Info, 0, sizeof(KUiPlayerAttribute));
	if (g_pCoreShell)
	   g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (uintptr_t)&Info, 0);

	char nTempS4tr[64];
	ZeroMemory(nTempS4tr,sizeof(nTempS4tr));
	sprintf(nTempS4tr,"%d",Info.nPKValue);
	pPk->setString(nTempS4tr);

	sprintf(nTempS4tr,"ת������:%d",Info.nReBorn);
	pZhuanSheng->setString(UTEXT(nTempS4tr,1).c_str());

	//sprintf(nTempS4tr,"ת������:%d",Info.);
	//pZhuanSheng->setString(UTEXT(nTempS4tr,1).c_str());

}
void KuiViewRoleState::update(float delta)
{
	if (isOpen && g_pCoreShell)
	{
	   //UpdateAllEquips();
	   //BreatheUpdateInfo();
	   //BreathAttributeUpdate();
	}
}
