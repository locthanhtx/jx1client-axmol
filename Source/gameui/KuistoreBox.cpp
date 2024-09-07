#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuistoreBox.h"
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
#include "gamecore/KItem.h"
#include "gamecore/KPlayer.h"
#include "gameui/KuiItemdesc.h"
#include "gameui/KuigetMoney.h"
#include "KuiItemKuo.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

KuistoreBox::KuistoreBox()
{
	isOpen = false;
	m_bScrolling=false;
//	memset(m_ItemData,0,sizeof(KUiItemdObject) * 60);
	pMoneyLabel       = NULL;
	//pXuLabel          = NULL;
	colorLayer        = NULL;
	//red               = NULL;
	m_ItemData.clear();
	m_nMoney = 0;
	//m_nXu = 0;
	m_nNumObjects = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	m_descPadIsOpen = false;
	m_getmoneyPadIsOpen =false;
	//savemenu = NULL;
	btnBackPic = NULL;
	nstrCurSelItemKey ="";
	m_MovePosX  = 0;
	nBakChickTime = 0;
	_m_itemExPadIsOpen =false;
	sItemExLayer=NULL;
}

KuistoreBox::~KuistoreBox()
{
	isOpen = false;
	pMoneyLabel  = NULL;
	colorLayer        = NULL;
	m_ItemData.clear();
	//if (red)
	//	red->release();
	m_descPadIsOpen = false;
	m_getmoneyPadIsOpen =false;
}

//����ȷ��������
KuistoreBox* KuistoreBox::create(const char* title, Ref* callbackListener, const std::function<void(ax::Node*)>& callfun)
{
	KuistoreBox * popLayer = KuistoreBox::create();
	//popLayer->setTitle(title);
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuistoreBox::init()
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
	sprintf(nSprName,"\\spr\\Ui3\\������\\���������.spr");
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
	spriteBox->setPosition(ax::Vec2(winSize.width/2-170,winSize.height/2));
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

void KuistoreBox::addDialogData()
{
	if  (!g_pCoreShell) return;

	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;

    pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Arial
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	//pMoneyLabel->setPosition(ax::Vec2(60,128));
	pMoneyLabel->setPosition(ax::Vec2(60,88));
	ParentNode_Item->addChild(pMoneyLabel,1);

	btnBackPic = Sprite::create("ui/btn_3.png");
	btnBackPic->setAnchorPoint(ax::Vec2(0,0));
	btnBackPic->setPosition(ax::Vec2(m_size.width,m_size.height/2-btnBackPic->getContentSize().height/2));
	ParentNode_Item->addChild(btnBackPic);

	KUiObjAtRegion* pObjs = NULL;

	int nCount = g_pCoreShell->GetGameData(GDI_ITEM_IN_STORE_BOX, 0, 0);
	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_IN_STORE_BOX, (uintptr_t)pObjs, nCount);//���߳�ִ�У�nCountֵ����
		for (int i = 0; i < nCount; ++i)
			UpdateItem(&pObjs[i], true);
		free(pObjs);
		pObjs = NULL;
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
void KuistoreBox::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui3\\������\\�����䣭ȡǮ.spr");
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
	MenuItemSprite * okConfirm = MenuItemSprite::create(sprite_ok_btn_confirm_normal, sprite_ok_btn_confirm_select, CC_CALLBACK_1(KuistoreBox::getmoneyCallBackFunc, this));
	okConfirm->setPosition(ax::Vec2(-50,3));
	//�ر�
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui3\\������\\�����䣭�ر�.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_close_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_close_select = Sprite::createWithTexture(bgCur);
	MenuItemSprite * closeConfirm = MenuItemSprite::create(sprite_close_normal, sprite_close_select, CC_CALLBACK_1(KuistoreBox::buttonCallBackFunc, this));
	closeConfirm->setPosition(ax::Vec2(50,3));
	//Point bgSpritePoint = ParentNode_Item->getPosition();
	//����չ
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui3\\������\\��չ������.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_openbox_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_openbox_select = Sprite::createWithTexture(bgCur);
	MenuItemSprite * openboxConfirm = MenuItemSprite::create(sprite_openbox_normal, sprite_openbox_select, CC_CALLBACK_1(KuistoreBox::openboxCallBackFunc, this));
	openboxConfirm->setPosition(ax::Vec2(65,30));

	//ȡ
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui3\\������\\ͼ��\\ȡ_3.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_qu_normal =  Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_qu_select=  Sprite::createWithTexture(bgCur);
	MenuItemSprite * quConfirm = MenuItemSprite::create(sprite_qu_normal, sprite_qu_select, CC_CALLBACK_1(KuistoreBox::chuCallBackFunc, this));
	quConfirm->setPosition(ax::Vec2(m_size.width/2+20,m_size.height/2-5));

	//��
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui3\\������\\ͼ��\\��_3.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_qi_normal =  Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_qi_select=  Sprite::createWithTexture(bgCur);
	MenuItemSprite * qiConfirm = MenuItemSprite::create(sprite_qi_normal, sprite_qi_select, CC_CALLBACK_1(KuistoreBox::diuCallBackFunc, this));
	qiConfirm->setPosition(ax::Vec2(m_size.width/2+20,m_size.height/2-sprite_qi_select->getContentSize().height-10));

	//����
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui3\\������\\ͼ��\\��.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_zheng_normal =  Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_zheng_select=  Sprite::createWithTexture(bgCur);
	MenuItemSprite * zenConfirm = MenuItemSprite::create(sprite_zheng_normal, sprite_zheng_select, CC_CALLBACK_1(KuistoreBox::zenliCallBackFunc, this));
	zenConfirm->setPosition(ax::Vec2(m_size.width/2+20,m_size.height/2-sprite_zheng_select->getContentSize().height*2-10*2));


	Menu * menu = Menu::create(okConfirm,quConfirm,qiConfirm,zenConfirm,closeConfirm,openboxConfirm,NULL);
	//menu->setPosition(ax::Vec2(bgSpritePoint.x-m_size.width/2,bgSpritePoint.y-m_size.height/2));//�˵�λ������Ϊ����������
	menu->setPosition(ax::Vec2(m_size.width/2,45));//�˵�λ������Ϊ����������
	ParentNode_Item->addChild(menu);         //�ӵ�������
}

void KuistoreBox::zenliCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		messageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		messageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
		messageBox("����Ʒ��������","��ʾ:");
#else
		messageBox(UTEXT("����Ʒ��������",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}
	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);

	unsigned int nCurChicktime = pInfo.npTime;  //��¼�򿪴��ڵ�����
#define MOVE_TIME_1 2
	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=MOVE_TIME_1)
	{//�������
#ifdef WIN32
		messageBox("�����̫Ƶ����","��ʾ:");
#else
		messageBox(UTEXT("�����̫Ƶ����",1).c_str(),UTEXT("��ʾ:",1).c_str());
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
	Obj.eContainer    = UOC_STORE_BOX;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_repository);
}

void KuistoreBox::diuCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	/*if (m_CurSelItemIndex<0)
	{
		messageBox("��ѡ��һ����Ʒ","��ʾ:");
		return;
	}*/
	if  (nstrCurSelItemKey.empty() || m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		messageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		messageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

	if  (Item[m_ItemData[nstrCurSelItemKey].uId].GetTime()->bYear>0 || Item[m_ItemData[nstrCurSelItemKey].uId].GetIsBang())
	{
		nstrCurSelItemKey="";
#ifdef WIN32
		messageBox("��ʱ/���Ʒ��ֹ����","��ʾ:");
#else
		messageBox(UTEXT("��ʱ/���Ʒ��ֹ����",1).c_str(),UTEXT("��ʾ:",1).c_str());
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
	bool isYes = false;
	if (g_pCoreShell && g_pCoreShell->GetAutoplayid())
		isYes = true;
	Player[CLIENT_PLAYER_INDEX].AutoAwayItem(Item[m_ItemData[nstrCurSelItemKey].uId].GetID(),isYes);
	nstrCurSelItemKey="";
	//char msg[64];
	//sprintf(msg,"����:%d",m_CurSelItemIndex);
	//messageBox(msg,"��ʾ");
}

//������
void KuistoreBox::chuCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (nstrCurSelItemKey.empty() || m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		messageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		messageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

	if  (m_ItemData[nstrCurSelItemKey].uId<=0 || m_ItemData[nstrCurSelItemKey].uId>=MAX_ITEM)
	{
#ifdef WIN32
			messageBox("����Ʒ����ȡ��","��ʾ:");
#else
			messageBox(UTEXT("����Ʒ����ȡ��",1).c_str(),UTEXT("��ʾ:",1).c_str());
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
		Obj.eContainer    = UOC_STORE_BOX;
		nstrCurSelItemKey="";
		if (g_pCoreShell)
		   g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_equipment);
}
//��ĸ��
void KuistoreBox::ziCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		messageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		messageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}
	char msg[64];
	sprintf(msg,"��ĸ��:%s",nstrCurSelItemKey.c_str());
	messageBox(msg,"��ʾ");
}
void KuistoreBox::kuo1CallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		messageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		messageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}
	char msg[64];
	sprintf(msg,"��һ:%s",nstrCurSelItemKey.c_str());
	messageBox(msg,"��ʾ");
}
void KuistoreBox::kuo2CallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		messageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		messageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}
	char msg[64];
	sprintf(msg,"����:%s",nstrCurSelItemKey.c_str());
	messageBox(msg,"��ʾ");
}
void KuistoreBox::kuo3CallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (m_ItemData.count(nstrCurSelItemKey)<=0)
	{
#ifdef WIN32
		messageBox("��ѡ��һ����Ʒ!","��ʾ:");
#else
		messageBox(UTEXT("��ѡ��һ����Ʒ!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

}

void KuistoreBox::getmoneyCallBackFunc(Ref * pSender)
{//ok �ص�
	if (m_getmoneyPadIsOpen)
		this->removeChildByTag(1002,true);

	Layer * sgetmonyLayer = KuigetMoney::create(0,m_nMoney,0,this,CC_CALLBACK_1(KuistoreBox::closegetmonyPadCallback, this));
	m_getmoneyPadIsOpen = true;
	this->addChild(sgetmonyLayer,2,1002);
}
//ȡǮ�ص�
void KuistoreBox::closegetmonyPadCallback(Node *pNode)
{
	//int nMaxMoney = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetsMon(room_repository);
	m_getmoneyPadIsOpen = false;
}


void KuistoreBox::openboxCallBackFunc(Ref * pSender)
{
	if  (_m_itemExPadIsOpen) return;

	sItemExLayer = KuiItemKuo::create(this,CC_CALLBACK_1(KuistoreBox::closeItemKuoPadCallback, this));
	_m_itemExPadIsOpen = true;
	ParentNode_Item->addChild(sItemExLayer);
	//buttonCallBackFunc(NULL);
}
void KuistoreBox::closeItemKuoPadCallback(Node *pNode)
{
	sItemExLayer=NULL;
	_m_itemExPadIsOpen = false;
}
//ִ���ϲ����Ļص��������رյ�����
void KuistoreBox::buttonCallBackFunc(Ref * pSender)
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
void KuistoreBox::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}


//��д����ע�ắ�������¸�����������
void KuistoreBox::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
}

Rect KuistoreBox::getRect(Node* pNode,int i)
{
	//m_origin ���������������Ļ�Ŀ�ʼλ��
	Rect  rc;
	rc.origin     = pNode->getPosition();                      //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();                   //�����Ʒ�Ŀ�Ⱥ͸߶�
	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //�������� - m_size.width/2
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�     - m_size.height/2

	return rc;
}

void KuistoreBox::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//�ƶ���
void KuistoreBox::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	Point point   = ParentNode_Item->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff    = point - m_lastPoint;                  //����ͼƬ��λ��
	ParentNode_Item->setPosition((ParentNode_Item->getPosition() + diff));
	m_origin      = ParentNode_Item->getPosition();
	m_lastPoint = point;
}
//��������ccTouchBegan������true
bool KuistoreBox::ccTouchBegan(Touch *pTouch, Event *pEvent)
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
			sprintf(nKey,"item_%u_color",Item[it->second.uId].GetID());
			std::string nItemKey = nKey;
			nCurColorlayer = (LayerColor *)ParentNode_Item->getChildByName(nItemKey);
			sprintf(nKey,"item_%u",Item[it->second.uId].GetID());
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
						Layer * sDescLayer = KuiItemdesc::create(it->second,this,CC_CALLBACK_1(KuistoreBox::closeItemPadCallback, this),0);
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

void KuistoreBox::closeItemPadCallback(Node *pNode)
{//��������б�Ǻţ���ȷ�����ĸ���ť
	m_descPadIsOpen = false;
}
void KuistoreBox::update(float delta)
{
	//messageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
		int nCurMoney = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetsMon(room_repository);
		char nPointCount[32];
		if (pMoneyLabel)
		{
			sprintf(nPointCount,"%d",nCurMoney);
			pMoneyLabel->setString(nPointCount);
		}
	}
}

void KuistoreBox::UpdateExItem(KUiObjAtRegion* pItem, int bAdd,int exBoxIndex)
{
	if  (sItemExLayer)
	{
		((KuiItemKuo *)sItemExLayer)->UpdateItem(pItem,bAdd,exBoxIndex);
	}
}

void KuistoreBox::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{
	if (pItem)
	{
		if (pItem->Obj.uGenre == CGOG_MONEY)
		{
			m_nMoney = pItem->Obj.uId;
			char nPointCount[32];
			if (pMoneyLabel)
			{
				sprintf(nPointCount,"%d",m_nMoney);
				pMoneyLabel->setString(nPointCount);
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
			//messageBox("��������","��Ʒ");
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
			sprintf(nKey,"item_%u_color",Item[Obj.uId].GetID());
			std::string nItemKey = nKey;
			ParentNode_Item->removeChildByName(nItemKey,true);
			sprintf(nKey,"item_%u",Item[Obj.uId].GetID());
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

void KuistoreBox::UpdateData()
{
// m_CurSelItemIndex = -1;
   nstrCurSelItemKey ="";
  // memset(m_ItemData,0,sizeof(KuistoreBoxdObject) * 60);
   _ItemData::iterator it;
   for( it = m_ItemData.begin(); it != m_ItemData.end(); it++)
   {
	   if (it->second.uId>0 && it->second.uId<MAX_ITEM)
	   {////����ĳ���ܵļӳ�
		   char nKey[32];
		   sprintf(nKey,"item_%u_color",Item[it->second.uId].GetID());
		   std::string nItemKey = nKey;
		   ParentNode_Item->removeChildByName(nItemKey,true);
		   //nCurColorlayer = (LayerColor *)ParentNode_Item->getChildByName(nItemKey);
		   sprintf(nKey,"item_%u",Item[it->second.uId].GetID());
		   nItemKey = nKey;
		   //SkillNode = (Sprite *)ParentNode_Item->getChildByName(nItemKey);
		   ParentNode_Item->removeChildByName(nItemKey,true);
	   }
   }
   m_ItemData.clear();
  /* for (int i=0;i<m_nNumObjects;++i)
   {
	sprintf(nItemKey,"item_%d",i+1);
	std::string nKey =nItemKey;
	ParentNode_Item->removeChildByName(nKey,true);
	sprintf(nItemKey,"color_%d",i+1);
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
					sprintf(nItemSprPath, "%s", Item[pObjs[i].Obj.uId].GetImagePath());
				else
					sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");

				if  (nItemSprPath[0])
				{
					//sprintf(nItemSprPath,Item[pObjs[i].Obj.uId].GetImagePath());
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
					sprintf(nItemKey,"item_%u",Item[pObjs[i].Obj.uId].GetID());
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
							sprintf(stack,"%d",Item[pObjs[i].Obj.uId].GetStackNum());
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
					m_ItemData[nKey].nGenkind = UOC_ITEM_EX;  //����Я��

					Color4B color(112, 128, 144, 150);
					//sprintf(nItemKey,"color_%d",i+1);
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
int KuistoreBox::AddObject(KUiDraggedObject* pObject, int nCount)
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
					sprintf(nItemSprPath, "%s", Item[pObject->uId].GetImagePath());
				else
					sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");
				if  (nItemSprPath[0])
				{
					//sprintf(nItemSprPath,Item[pObject->uId].GetImagePath());
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
					sprintf(nItemKey,"item_%u",Item[pObject->uId].GetID());
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
							sprintf(stack,"%d",Item[pObject->uId].GetStackNum());
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
					m_ItemData[nKey].nGenkind = UOC_STORE_BOX;  //����Я��

					Color4B color(112, 128, 144, 150);
					//sprintf(nItemKey,"color_%d",i+1);
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
					Obj.nContainer = UOC_STORE_BOX;

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
