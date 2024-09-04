#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiSellItem.h"
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
#include "gamecore/KPlayer.h"
#include "gameui/KuiItemdesc.h"
#include "gameui/KuigetMoney.h"
#include "gameui/KuigetStringInUI.h"
#include "gameui/KuiLockscree.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

KuiSellItem::KuiSellItem()
{
	isOpen = false;
	__isCanMove =true;
	m_bScrolling = false;
//	memset(m_ItemData,0,sizeof(KuiSellItemdObject) * 60);
	//pMoneyLabel       = NULL;
	//pXuLabel          = NULL;
	colorLayer        = NULL;
	//red               = NULL;
	menu              = NULL;
	m_IsSetPriceCount = 0;
	m_ItemData.clear();
	m_nMoney = 0;
	m_nXu = 0;
	m_nNumObjects = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	nBakChickTime=0;
	m_descPadIsOpen = false;
	m_EqdescPadIsOpen = false;
	m_gaoPadIsOpen  =false;
	savemenu = NULL;
	nstrCurSelItemKey ="";
	m_MovePosX  = 0;
	m_nIstrue   =false;
	isDoIt      =false;
	ZeroMemory(m_ShopName,sizeof(m_ShopName));
}

KuiSellItem::~KuiSellItem()
{
	isOpen = false;
	//pMoneyLabel  = NULL;
	colorLayer        = NULL;
	m_ItemData.clear();
	//if (red)
	//   red->release();
	m_descPadIsOpen = false;
	m_EqdescPadIsOpen = false;
	m_savemoneyPadIsOpen = false;
}

//����ȷ��������
KuiSellItem * KuiSellItem::create(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiSellItem * popLayer = KuiSellItem::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiSellItem::init()
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
	//���������ı���ͼƬ������������λ��Ϊ��Ļ�е�
	//SpriteFrame * f_tips_box = sfCache->spriteFrameByName("ui/state/skill.png");
	//Sprite * spriteBox = Sprite::create("ui/state/skill.png");//Sprite::createWithSpriteFrame(f_tips_box);
	//\\spr\\Ui3\\������\\���������.spr
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\�������3.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;
	Sprite * spriteBox = Sprite::createWithTexture(bgCur);
	ParentNode_Item = spriteBox;
	spriteBox->setPosition(ax::Vec2(winSize.width/2+60,winSize.height/2));
	this->addChild(spriteBox);

	//��ñ���ͼƬ�Ĵ�С
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();      //���������������Ļ��λ��
	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С
	//����������Ӧ

	this->scheduleUpdate();                   //���ú���
	auto listener = ax::EventListenerTouchOneByOne::create();
ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this); //��ֹ��㴥��
	return true;
}

void KuiSellItem::addDialogData()
{
	if  (!g_pCoreShell) return;

	//Left=24
	//Top=72
	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;
	t_sprintf(m_ShopName,"Ĭ��ϵͳ��̯");
   /* pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Arial
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	pMoneyLabel->setPosition(ax::Vec2(60,128));
	ParentNode_Item->addChild(pMoneyLabel,1);

	pXuLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Arial
	pXuLabel->setAnchorPoint(ax::Vec2(0,0));
	pXuLabel->setPosition(ax::Vec2(60,105));
	ParentNode_Item->addChild(pXuLabel,1);*/


	KUiObjAtRegion* pObjs = NULL;
	int nCount   = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 0); //�õ��������Ʒ������

	m_nNumObjects = nCount;

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

					//nItemSpr->setUserData((BYTE*)&no); //�ڵ��������
					//no.uId = 0;
					/*KUiDraggedObject * nItemData = (KUiDraggedObject *)nItemSpr->getUserData();
					if  (nItemData)
					{
						char msg[64];
						t_sprintf(msg,"����:h:%d,w:%d",nItemData->uGenre,nItemData->uId);
						ccMessageBox(msg,"����");
					}*/
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
void KuiSellItem::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	//Sprite * sprite_tips_btn_confirm_normal =  Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_normal);
	//SpriteFrame * f_tips_btn_confirm_select = sfCache->spriteFrameByName("btn_confirm_select.png");
	//Sprite * sprite_tips_btn_confirm_select =  Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_select);
	//MenuItemSprite * itemConfirm = MenuItemSprite::create(sprite_tips_btn_confirm_normal, sprite_tips_btn_confirm_select, CC_CALLBACK_1(KuiSellItem::buttonCallBackFunc, this));
	//itemConfirm->setPosition(ax::Vec2(0,0));

	Point bgSpritePoint = ParentNode_Item->getPosition();

	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	Point nStartPos;
	nStartPos.x = m_size.width/2+20;
	nStartPos.y = m_size.height/2;
	//�ر�
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\���ߣ��ر�.spr");
	g_StrLower(nSprName);
	bgCur =_getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_close_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_close_select =  Sprite::createWithTexture(bgCur);
	MenuItemSprite * closeConfirm = MenuItemSprite::create(sprite_close_normal, sprite_close_select, CC_CALLBACK_1(KuiSellItem::buttonCallBackFunc, this));
	closeConfirm->setPosition(ax::Vec2(0,0));
	//���� \spr\Ui3\������\ͼ��\zenglibtn.spr
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
	MenuItemSprite * zhengliConfirm = MenuItemSprite::create(sprite_zhengli_normal, sprite_zhengli_select, CC_CALLBACK_1(KuiSellItem::zhengCallBackFunc, this));
	zhengliConfirm->setPosition(ax::Vec2(0,28));
	//�й�
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\��̯\\�й�.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_tuo_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_tuo_select =  Sprite::createWithTexture(bgCur);
	MenuItemSprite * tuoConfirm = MenuItemSprite::create(sprite_tuo_normal, sprite_tuo_select, CC_CALLBACK_1(KuiSellItem::tuoguanCallBackFunc, this));
	tuoConfirm->setPosition(ax::Vec2(65,28));
	//�ٸ�
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\��̯\\���ù��.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_gao_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_gao_select =  Sprite::createWithTexture(bgCur);
	MenuItemSprite * gaoConfirm = MenuItemSprite::create(sprite_gao_normal, sprite_gao_select, CC_CALLBACK_1(KuiSellItem::gaoCallBackFunc, this));
	gaoConfirm->setPosition(ax::Vec2(-63,68));

	//���
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\��̯\\���.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_biao_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_biao_select =  Sprite::createWithTexture(bgCur);
	MenuItemSprite * biaoConfirm = MenuItemSprite::create(sprite_biao_normal, sprite_biao_select, CC_CALLBACK_1(KuiSellItem::biaoCallBackFunc, this));
	biaoConfirm->setPosition(ax::Vec2(-3,68));

	//��ʼ��̯
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\��̯\\��̯.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_kuai_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_kuai_select =  Sprite::createWithTexture(bgCur);
	MenuItemSprite * kuaiConfirm = MenuItemSprite::create(sprite_kuai_normal, sprite_kuai_select, CC_CALLBACK_1(KuiSellItem::tanCallBackFunc, this));
	kuaiConfirm->setPosition(ax::Vec2(63,68));

	//��Ǯ
	/*ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\���ߣ���Ǯ.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_save_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_save_select =  Sprite::createWithTexture(bgCur);
	MenuItemSprite * saveConfirm = MenuItemSprite::create(sprite_save_normal, sprite_save_select, CC_CALLBACK_1(KuiSellItem::saveCallBackFunc, this));
	//saveConfirm->setPosition(ax::Vec2(nStartPos.x+bgCur->getContentSize().width+2,nStartPos.y-(bgCur->getContentSize().height+2)*4));
	saveConfirm->setPosition(ax::Vec2(-sprite_save_select->getContentSize().width-5,0));
	*/
	menu = Menu::create(closeConfirm,gaoConfirm,biaoConfirm,kuaiConfirm,zhengliConfirm,tuoConfirm,NULL);
	//menu->setPosition(ax::Vec2(bgSpritePoint.x-m_size.width/2,bgSpritePoint.y-m_size.height/2));//�˵�λ������Ϊ����������
	menu->setPosition(ax::Vec2(m_size.width/2,45));//�˵�λ������Ϊ����������
	ParentNode_Item->addChild(menu);         //�ӵ�������

	/*savemenu = Menu::create(itemchuConfirm,itemziConfirm,itemkuo1Confirm,itemkuo2Confirm,itemkuo3Confirm,NULL);
	//menu->setPosition(ax::Vec2(bgSpritePoint.x-m_size.width/2,bgSpritePoint.y-m_size.height/2));//�˵�λ������Ϊ����������
	savemenu->setPosition(ax::Vec2(m_size.width/2,45));//�˵�λ������Ϊ����������
	ParentNode_Item->addChild(savemenu);         //�ӵ�������
	savemenu->setVisible(false);//����
	*/
}
//��Ǯ
void KuiSellItem::saveCallBackFunc(Ref * pSender)
{
	/*if (m_savemoneyPadIsOpen)
		this->removeChildByTag(1003,true);
	//1Ϊ��Ǯ������ΪȡǮ
	Layer * savemonyLayer = KuigetMoney::create(0,m_nMoney,1,this,CC_CALLBACK_1(KuiSellItem::closesavemonyPadCallback));
	m_savemoneyPadIsOpen = true;
	this->addChild(savemonyLayer,2,1003); */
}

void KuiSellItem::closegaoPadCallback(Node *pNode)
{
	if (pNode)
	   t_sprintf(m_ShopName, std::to_string(pNode->getTag()).c_str());

	m_gaoPadIsOpen = false;
	/*//���ò��ܴ���
	menu->setEnabled(true);
	menu->setTouchEnabled(false);
	menu->setTouchEnabled(true);
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-128,true);
	menu->setEnabled(true);*/
}

void KuiSellItem::closesavemonyPadCallback(Node *pNode)
{
	//int nMaxMoney = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetsMon(room_repository);
	if (pNode)
	{
		KUiObjAtContRegion	Obj;
		Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
		Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
		Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
		Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
		Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
		Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
		Obj.eContainer    = UOC_ITEM_TAKE_WITH;

		int nModel=0;

		/*if(m_YinModel.IsButtonChecked())  //  //��ť������
			nModel=1;
		else if (m_JinModel.IsButtonChecked())
			nModel=2;*/
		char nTempStr[64];
		t_sprintf(nTempStr,std::to_string(pNode->getTag()).c_str());
		int nPrice = g_Atoui(nTempStr);
		if  (nPrice>0)
		   m_IsSetPriceCount++;
		else
		{
			m_IsSetPriceCount --;
			if (m_IsSetPriceCount<0)
				m_IsSetPriceCount=0;
		}
		g_pCoreShell->OperationRequest(GDI_SET_TRADE_ITEM,(uintptr_t)(&Obj),nPrice,nModel);
	   // ccMessageBox(pNode->getTag().c_str(),"test");
	}
	m_savemoneyPadIsOpen = false;
	/*//���ò��ܴ���
	menu->setEnabled(true);
	menu->setTouchEnabled(false);
	menu->setTouchEnabled(true);
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-128,true);
	*/
}
//ʹ��
void KuiSellItem::tuoguanCallBackFunc(Ref * pSender)
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
	if (g_pCoreShell)
        g_pCoreShell->OperationRequest(GOI_LIXIAN, 0, 0);  //������������
}

void KuiSellItem::gaoCallBackFunc(Ref * pSender)
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
	if (m_savemoneyPadIsOpen)
		this->removeChildByTag(1003,true);

	if (m_gaoPadIsOpen)
		this->removeChildByTag(1004,true);

	Layer * savemonyLayer = KuigetStringInUI::create("��������",1,36,NULL,NULL,2,this,CC_CALLBACK_1(KuiSellItem::closegaoPadCallback, this),1);
	m_gaoPadIsOpen  = true;
	this->addChild(savemonyLayer,2,1004);
	//this->setTouchEnabled(false);
}

void KuiSellItem::tanCallBackFunc(Ref * pSender)
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

	int index = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if (index>0 && index<MAX_NPC && Npc[index].m_BaiTan)
	{
		g_pCoreShell->OperationRequest(GDI_PLAYER_TRADE,(uintptr_t)(&m_ShopName), 0);
		return;
	}

	int m_IsSetPrice=0;
	m_IsSetPrice = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 1); //��̯��Ʒ������
	if (m_IsSetPrice<=0)
	{//�Ƿ������˼۸�
#ifdef WIN32
		ccMessageBox("�����ð�̯�۸�!","��ʾ");
#else
		ccMessageBox(UTEXT("�����ð�̯�۸�!",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		return;
	}

    if (g_pCoreShell->OperationRequest(GDI_PLAYER_TRADE,(uintptr_t)(&m_ShopName),true))
	{//��̯�ɹ� ����������ס��Ļ
		Layer * sItemsLayer = KuiLockscree::create(this,CC_CALLBACK_1(KuiSellItem::closelockPadCallback, this));
		isDoIt = true;
		this->addChild(sItemsLayer,ORDER_UI_DIALOD,100);
	}
	else
	{

	}
	/*if (isDoIt)
	{
		isDoIt = false;
		//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2,true);
	}
	else
	{
	  isDoIt = true;
	  //ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-128,true);
	}*/
}

void KuiSellItem::closelockPadCallback(Node *pNode)
{
	isDoIt = false;
	if (g_pCoreShell) //�رհ�̯
	   g_pCoreShell->OperationRequest(GDI_PLAYER_TRADE,(uintptr_t)(&m_ShopName), 0);
}

void KuiSellItem::biaoCallBackFunc(Ref * pSender)
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
		ccMessageBox("��ѡ��һ����Ʒ","��ʾ:");
#else
		ccMessageBox(UTEXT("��ѡ��һ����Ʒ",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

	/*if  (m_ItemData[nstrCurSelItemKey].uId>0 || m_ItemData[nstrCurSelItemKey].uId<MAX_ITEM)
	{
		if (Item[m_ItemData[nstrCurSelItemKey].uId].GetGenre()==0)
		{
#ifdef WIN32
		ccMessageBox("����Ʒ���ܱ�۸�","��ʾ:");
#else
		ccMessageBox(UTEXT("����Ʒ���ܱ�۸�",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		    return;
		}
	}*/

	if (m_savemoneyPadIsOpen)
		this->removeChildByTag(1003,true);
	//1Ϊ��Ǯ������ΪȡǮ
	Layer * savemonyLayer = KuigetStringInUI::create("����۸�",1,10,NULL,NULL,1,this,CC_CALLBACK_1(KuiSellItem::closesavemonyPadCallback, this),1);
	m_savemoneyPadIsOpen = true;
	this->addChild(savemonyLayer,2,1003);
	//this->setTouchEnabled(false);//���ò��ܴ���
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
	{
		if (g_pCoreShell->OperationRequest(GOI_NPC_ITEM_BREAK,(uintptr_t)(&Obj), 1))
		{//��ֳɹ���,������ʾ
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


void KuiSellItem::zhengCallBackFunc(Ref * pSender)
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

//ִ���ϲ����Ļص��������رյ�����
void KuiSellItem::buttonCallBackFunc(Ref * pSender)
{
	if  (!pSender)
	{
		if (m_callfun &&  m_callbackListener)
			(m_callfun)(NULL);
	}
	else
	{
		Node * node = dynamic_cast<Node *>(pSender);
		//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
		if (m_callfun &&  m_callbackListener)
			(m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiSellItem::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}


//��д����ע�ắ�������¸�����������
void KuiSellItem::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2,true);
}

Rect KuiSellItem::getRect(Node* pNode,int i)
{
	//m_origin ���������������Ļ�Ŀ�ʼλ��
	Rect  rc;
	rc.origin     = pNode->getPosition();                      //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();                   //�����Ʒ�Ŀ�Ⱥ͸߶�
	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y

	return rc;
}

void KuiSellItem::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//�ƶ���
void KuiSellItem::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling ||!__isCanMove) return;
	//Touch *touch = (Touch*)pTouches->anyObject();
	//if  (nPartNode)
	//	nPartNode->setVisible(false);
	//__loopTime = MAX_LOOP_TIME;
	Point point   = ParentNode_Item->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff    = (point - m_lastPoint);
	//Node *content = clipper->getChildByTag(2);                   //����ͼƬ��λ��
	ParentNode_Item->setPosition((ParentNode_Item->getPosition() + diff));
	m_origin      = ParentNode_Item->getPosition();
	m_lastPoint = point;
}
//��������ccTouchBegan������true
bool KuiSellItem::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point point = ParentNode_Item->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Team->getContentSize().width*/,winSize.height/* ParentNode_Team->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //����ڷ�Χ�� �Ϳ����϶�
	m_lastPoint = point;

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
				if (getRect(nCurColorlayer,0).containsPoint(location))
				{//������
					if (SkillNode)
					{
						//SkillNode->runAction(red);
						SkillNode->setColor(ax::Color3B::RED);
						m_ItemData[nItemKey].nRect  = getRect(nCurColorlayer,0);
						m_ItemData[nItemKey].nindex = -1;
						Layer * sDescLayer = KuiItemdesc::create(it->second,this,CC_CALLBACK_1(KuiSellItem::closeItemPadCallback, this),1);
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

void KuiSellItem::closeItemPadCallback(Node *pNode)
{//��������б�Ǻţ���ȷ�����ĸ���ť
	m_descPadIsOpen = false;
}
void KuiSellItem::update(float delta)
{
	//ccMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	    m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
		m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
		//char nPointCount[32];
	   /*if (pMoneyLabel)
	   {
		   t_sprintf(nPointCount,"%d",m_nMoney);
		  // pSkillPointLabel->setColor(ccWit)
		   pMoneyLabel->setString(nPointCount);
	   }
	   if  (pXuLabel)
	   {
		   t_sprintf(nPointCount,"%d",m_nXu);
		   // pSkillPointLabel->setColor(ccWit)
		   pXuLabel->setString(nPointCount);
	   }
	   */
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

void KuiSellItem::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
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
			AddObject(&Obj,1);

		}
		else      //����
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
	else //����ȫ������Ʒ
		UpdateData();
}

void KuiSellItem::UpdateData()
{
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
	int nCount    = g_pCoreShell->GetGameData(GDI_ITEM_TAKEN_WITH, 0, 0); //�õ��������Ʒ������

	m_nNumObjects = nCount;

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

//������Ʒ
int KuiSellItem::AddObject(KUiDraggedObject* pObject, int nCount)
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
				if  (Item[pObject->uId].GetImagePath())
					t_sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
				else
					t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");
				if  (nItemSprPath[0])
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
