#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiItemKuo.h"
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
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;
#define MOVE_TIME 2
KuiItemKuo::KuiItemKuo()
{
	isOpen = false;
	m_bScrolling=false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	m_ItemData.clear();
	colorLayer        = NULL;
	//red               = NULL;
	nstrCurSelItemKey ="";
	m_nIstrue = false;
	m_nNumObjects = 0;
	m_descPadIsOpen =false;
	m_StartPos.x=0;
	m_StartPos.y=0;
	btnBackPic = NULL;
	nBakChickTime = 0;
	nCurExPadIndex =-1;
}

KuiItemKuo::~KuiItemKuo()
{
	isOpen = false;
	m_ItemData.clear();
	nstrCurSelItemKey ="";
	colorLayer        = NULL;
	//if (red)
	//	red->release();
	//red               = NULL;
}

//����ȷ��������
KuiItemKuo * KuiItemKuo::create(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiItemKuo * popLayer = KuiItemKuo::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiItemKuo::init()
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
	t_sprintf(nSprName,"\\Spr\\Ui3\\���Ӵ�����\\����������.spr");
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
	spriteBox->setPosition(ax::Vec2(winSize.width/2-30,winSize.height/2));
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

void KuiItemKuo::addDialogData()
{
	if  (!g_pCoreShell) return;
	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =60;//+ITEM_CELL_SIZE/2;

    /*pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Arial
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	pMoneyLabel->setPosition(ax::Vec2(60,128));
	ParentNode_ItemEx->addChild(pMoneyLabel,1);

	pXuLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Arial
	pXuLabel->setAnchorPoint(ax::Vec2(0,0));
	pXuLabel->setPosition(ax::Vec2(60,105));
	ParentNode_ItemEx->addChild(pXuLabel,1);
	*/

	UpdateData(GDI_ITEM_IN_EX_BOX1);
	//ax::Director::getInstance()->getTextureCache()->addImage("");
	btnBackPic = Sprite::create("ui/btn_3.png");
	btnBackPic->setAnchorPoint(ax::Vec2(0,0));
	btnBackPic->setPosition(ax::Vec2(m_size.width,m_size.height/2-btnBackPic->getContentSize().height/2));
	ParentNode_ItemEx->addChild(btnBackPic);

	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);//���úʹ��ڵĴ�С
	ParentNode_ItemEx->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	//red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	//red->retain();
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiItemKuo::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
	//\Spr\Ui3\���Ӵ�����\�ر�.spr
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���Ӵ�����\\�ر�.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	Sprite * sprite_colse_normal;
	Sprite * sprite_colse_select;
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	sprite_colse_normal = Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	sprite_colse_select = Sprite::createWithTexture(bgCur);//Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_select);
	MenuItemSprite * closeConfirm = MenuItemSprite::create(sprite_colse_normal, sprite_colse_select, CC_CALLBACK_1(KuiItemKuo::buttonCallBackFunc, this));
	closeConfirm->setPosition(ax::Vec2(-5,-23));

	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���Ӵ�����\\һ.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	sprite_colse_normal = Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	sprite_colse_select = Sprite::createWithTexture(bgCur);//Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_select);
	MenuItemSprite * kuoConfirm_1 = MenuItemSprite::create(sprite_colse_normal, sprite_colse_select, CC_CALLBACK_1(KuiItemKuo::buttonCallBackFunc_1, this));
	//kuoConfirm_1->setScaleX(2);
	kuoConfirm_1->setScale(2);
	kuoConfirm_1->setPosition(ax::Vec2(-m_size.width/2+45,m_size.height-92));

	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���Ӵ�����\\��.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	sprite_colse_normal = Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	sprite_colse_select = Sprite::createWithTexture(bgCur);//Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_select);
	MenuItemSprite * kuoConfirm_2 = MenuItemSprite::create(sprite_colse_normal, sprite_colse_select, CC_CALLBACK_1(KuiItemKuo::buttonCallBackFunc_2, this));
	kuoConfirm_2->setScale(2);
	kuoConfirm_2->setPosition(ax::Vec2(-m_size.width/2+97,m_size.height-92));

	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\���Ӵ�����\\��.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	sprite_colse_normal = Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	sprite_colse_select = Sprite::createWithTexture(bgCur);//Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_select);
	MenuItemSprite * kuoConfirm_3 = MenuItemSprite::create(sprite_colse_normal, sprite_colse_select, CC_CALLBACK_1(KuiItemKuo::buttonCallBackFunc_3, this));
	kuoConfirm_3->setScale(2);
	kuoConfirm_3->setPosition(ax::Vec2(-m_size.width/2+150,m_size.height-92));

	//ȡ
	t_sprintf(nSprName,"\\spr\\Ui3\\������\\ͼ��\\ȡ_3.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_qu_normal =  Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_qu_select=  Sprite::createWithTexture(bgCur);
	MenuItemSprite * quConfirm = MenuItemSprite::create(sprite_qu_normal, sprite_qu_select, CC_CALLBACK_1(KuiItemKuo::quCallBackFunc, this));
	quConfirm->setPosition(ax::Vec2(m_size.width/2+20,m_size.height/2-5));

	//��
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\������\\ͼ��\\��_3.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_qi_normal =  Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_qi_select=  Sprite::createWithTexture(bgCur);
	MenuItemSprite * qiConfirm = MenuItemSprite::create(sprite_qi_normal, sprite_qi_select, CC_CALLBACK_1(KuiItemKuo::qiCallBackFunc, this));
	qiConfirm->setPosition(ax::Vec2(m_size.width/2+20,m_size.height/2-sprite_qi_select->getContentSize().height-10));

	//����
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\������\\ͼ��\\��.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_zheng_normal =  Sprite::createWithTexture(bgCur);

	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_zheng_select=  Sprite::createWithTexture(bgCur);
	MenuItemSprite * zenConfirm = MenuItemSprite::create(sprite_zheng_normal, sprite_zheng_select, CC_CALLBACK_1(KuiItemKuo::zenliCallBackFunc, this));
	zenConfirm->setPosition(ax::Vec2(m_size.width/2+20,m_size.height/2-sprite_zheng_select->getContentSize().height*2-10*2));

	Menu * menu = Menu::create(closeConfirm,kuoConfirm_1,kuoConfirm_2,kuoConfirm_3,quConfirm,qiConfirm,zenConfirm,NULL);
	menu->setPosition(ax::Vec2(m_size.width/2,45));//�˵�λ������Ϊ����������
	ParentNode_ItemEx->addChild(menu);         //�ӵ�������
}

void KuiItemKuo::zenliCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (nstrCurSelItemKey.empty() || m_ItemData.count(nstrCurSelItemKey)<=0)
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
	if  (!m_nIstrue)
	{
#ifdef WIN32
		ccMessageBox("��չ�仹û�п���","��ʾ:");
#else
		ccMessageBox(UTEXT("��չ�仹û�п���",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);

	unsigned int nCurChicktime = pInfo.npTime;  //��¼�򿪴��ڵ�����

	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=MOVE_TIME)
	{//�������
		//nBakChickTime = nCurChicktime;
#ifdef WIN32
		ccMessageBox("�����̫Ƶ����","��ʾ:");
#else
		ccMessageBox(UTEXT("�����̫Ƶ����",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

	//if (nBakChickTime<=0)
	nBakChickTime = nCurChicktime;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData[nstrCurSelItemKey].uGenre;
	Obj.Obj.uId       = m_ItemData[nstrCurSelItemKey].uId;
	Obj.Region.h      = m_ItemData[nstrCurSelItemKey].DataX;
	Obj.Region.v      = m_ItemData[nstrCurSelItemKey].DataY;
	Obj.Region.Width  = m_ItemData[nstrCurSelItemKey].DataW;
	Obj.Region.Height = m_ItemData[nstrCurSelItemKey].DataH;
	//Obj.eContainer    = UOC_ITEM_EX;
	if (g_pCoreShell&&nCurExPadIndex>0)
	{
		if  (nCurExPadIndex==GDI_ITEM_IN_EX_BOX1)
		{
			Obj.eContainer    = UOC_EX_BOX1;
		    g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_exbox1);
		}
		else if (nCurExPadIndex==GDI_ITEM_IN_EX_BOX2)
		{
			Obj.eContainer    = UOC_EX_BOX2;
			g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_exbox2);
		}
		else if (nCurExPadIndex==GDI_ITEM_IN_EX_BOX3)
		{
			Obj.eContainer    = UOC_EX_BOX3;
			g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_exbox3);
		}
	}
}

void KuiItemKuo::qiCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (nstrCurSelItemKey.empty()|| m_ItemData.count(nstrCurSelItemKey)<=0)
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
		ccMessageBox("����Ʒ���ܶ���","��ʾ:");
#else
		ccMessageBox(UTEXT("����Ʒ���ܶ���",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}

	if  (!m_nIstrue)
	{
#ifdef WIN32
		ccMessageBox("��չ�仹û�п���","��ʾ:");
#else
		ccMessageBox(UTEXT("��ĸ����û�п���",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}
	if  (Item[m_ItemData[nstrCurSelItemKey].uId].GetTime()->bYear>0 || Item[m_ItemData[nstrCurSelItemKey].uId].GetIsBang())
	{
#ifdef WIN32
		ccMessageBox("��ʱ/���Ʒ��ֹ����","��ʾ:");
#else
		ccMessageBox(UTEXT("��ʱ/���Ʒ��ֹ����",1).c_str(),UTEXT("��ʾ:",1).c_str());
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
}

void KuiItemKuo::quCallBackFunc(Ref * pSender)
{

	if (m_descPadIsOpen)
	{
		m_descPadIsOpen = false;
		this->removeChildByTag(1001,true);
	}

	if  (nstrCurSelItemKey.empty() || m_ItemData.count(nstrCurSelItemKey)<=0)
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
		ccMessageBox("����Ʒ���ܴ���","��ʾ:");
#else
		ccMessageBox(UTEXT("����Ʒ���ܴ���",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
		return;
	}
	if  (!m_nIstrue)
	{
#ifdef WIN32
		ccMessageBox("��չ�仹û�п���","��ʾ:");
#else
		ccMessageBox(UTEXT("��չ�仹û�п���",1).c_str(),UTEXT("��ʾ:",1).c_str());
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

	if (nCurExPadIndex>0)
	{
		if  (nCurExPadIndex==GDI_ITEM_IN_EX_BOX1)
			Obj.eContainer    = UOC_EX_BOX1;
		else if (nCurExPadIndex==GDI_ITEM_IN_EX_BOX2)
			Obj.eContainer    = UOC_EX_BOX2;
		else if (nCurExPadIndex==GDI_ITEM_IN_EX_BOX3)
			Obj.eContainer    = UOC_EX_BOX3;
		nstrCurSelItemKey="";
	    if (g_pCoreShell)
		    g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_equipment);
	}
}
void KuiItemKuo::buttonCallBackFunc_1(Ref * pSender)
{
	UpdateData(GDI_ITEM_IN_EX_BOX1);
}
void KuiItemKuo::buttonCallBackFunc_2(Ref * pSender)
{
	UpdateData(GDI_ITEM_IN_EX_BOX2);
}
void KuiItemKuo::buttonCallBackFunc_3(Ref * pSender)
{
	UpdateData(GDI_ITEM_IN_EX_BOX3);
}
//ִ���ϲ����Ļص��������رյ�����
void KuiItemKuo::buttonCallBackFunc(Ref * pSender)
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
void KuiItemKuo::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}


//��д����ע�ắ�������¸�����������
void KuiItemKuo::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
}

Rect KuiItemKuo::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition();          //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();
	rc.origin.x   = m_origin.x + rc.origin.x + 15; //�������� - m_size.width/2
	rc.origin.y   = m_origin.y - m_size.height/2 + rc.origin.y+10; //���ϱ�    //- m_size.height/2

	return rc;
}

void KuiItemKuo::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//�ƶ���
void KuiItemKuo::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	Point point   = ParentNode_ItemEx->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff    = point - m_lastPoint;                  //����ͼƬ��λ��
	ParentNode_ItemEx->setPosition((ParentNode_ItemEx->getPosition() + diff));
	m_origin      = ParentNode_ItemEx->getPosition();
	m_lastPoint = point;
}
//��������ccTouchBegan������true
bool KuiItemKuo::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point point = ParentNode_ItemEx->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Team->getContentSize().width*/,winSize.height/* ParentNode_Team->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //����ڷ�Χ�� �Ϳ����϶�
	m_lastPoint = point;
	if  (!m_nIstrue)
	{
		nstrCurSelItemKey ="";
#ifdef WIN32
		ccMessageBox("��չ��û�п���","��ʾ");
#else
		ccMessageBox(UTEXT("��չ��û�п���",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		return true;
	}
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
			nCurColorlayer = (LayerColor *)ParentNode_ItemEx->getChildByName(nItemKey);
			t_sprintf(nKey,"item_%u",Item[it->second.uId].GetID());
			nItemKey = nKey;
			SkillNode = (Sprite *)ParentNode_ItemEx->getChildByName(nItemKey);
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
						Layer * sDescLayer = KuiItemdesc::create(it->second,this,CC_CALLBACK_1(KuiItemKuo::closeItemPadCallback, this),0);
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

void KuiItemKuo::closeItemPadCallback(Node *pNode)
{//��������б�Ǻţ���ȷ�����ĸ���ť
	m_descPadIsOpen = false;
}

void KuiItemKuo::update(float delta)
{
	//ccMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
		if (g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, GET_BOX_OPEN, 0))
			m_nIstrue = true;
		//	m_ItemEX.SetOpenClose(TRUE);
		else
			m_nIstrue = false;
	}
}

//void KuiItemKuo::draw()
//{
//	//ccMessageBox("����ѭ��","draw");
//}

void KuiItemKuo::UpdateItem(KUiObjAtRegion* pItem, int bAdd,int nExBoxIndex)
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
			return;

		if (bAdd) //����
		{
			//m_ItemBox.AddObject(&Obj,1);
			AddObject(&Obj,1,nExBoxIndex);

		}
		else      //����
		{
			char nKey[32];
			t_sprintf(nKey,"item_%u_color",Item[Obj.uId].GetID());
			std::string nItemKey = nKey;
			ParentNode_ItemEx->removeChildByName(nItemKey,true);
			t_sprintf(nKey,"item_%u",Item[Obj.uId].GetID());
			nItemKey = nKey;
			ParentNode_ItemEx->removeChildByName(nItemKey,true);
			m_ItemData.erase(nItemKey);
		}
		//m_ItemBox.RemoveObject(&Obj);
		//UiSoundPlay(UI_SI_PICKPUT_ITEM);
		//UpdateData();
	}
	else //����ȫ������Ʒ
	{
		if (nExBoxIndex==UOC_EX_BOX1)
		    UpdateData(GDI_ITEM_IN_EX_BOX1);
		else if (nExBoxIndex==UOC_EX_BOX2)
			UpdateData(GDI_ITEM_IN_EX_BOX2);
		else if (nExBoxIndex==UOC_EX_BOX3)
			UpdateData(GDI_ITEM_IN_EX_BOX3);
	}
}
//������Ʒ
int KuiItemKuo::AddObject(KUiDraggedObject* pObject, int nCount,int ExBoxIndex)
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
					Sprite *nItemSpr = (Sprite *)ParentNode_ItemEx->getChildByName(nKey);
					if (!nItemSpr)
					{
						nItemSpr = Sprite::createWithTexture(bgCur);
						//nItemSpr->setAnchorPoint(ax::Vec2(0,0));
						int nikey = std::hash<std::string>{}(nKey);
nItemSpr->setTag(nikey);
						ParentNode_ItemEx->addChild(nItemSpr,1);

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
					m_ItemData[nKey].nGenkind = ExBoxIndex;  //����Я��

					Color4B color(112, 128, 144, 150);
					//t_sprintf(nItemKey,"color_%d",i+1);
					strcat(nItemKey,"_color");
					nKey =nItemKey;
					LayerColor *bgcolorLayer = (LayerColor *)ParentNode_ItemEx->getChildByName(nKey);
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
						ParentNode_ItemEx->addChild(bgcolorLayer,0);
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
					Obj.nContainer = UOC_ITEM_EX;

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
//GDI_ITEM_IN_EX_BOX1
void KuiItemKuo::UpdateData(int nExboxIndex)  //123
{
// m_CurSelItemIndex = -1;
   nstrCurSelItemKey ="";
   nCurExPadIndex =nExboxIndex;
  // memset(m_ItemData,0,sizeof(KUiItemdObject) * 60);
   _ItemData::iterator it;
   for( it = m_ItemData.begin(); it != m_ItemData.end(); it++)
   {
	   if (it->second.uId>0 && it->second.uId<MAX_ITEM)
	   {////����ĳ���ܵļӳ�
		   char nKey[32];
		   t_sprintf(nKey,"item_%u_color",Item[it->second.uId].GetID());
		   std::string nItemKey = nKey;
		   ParentNode_ItemEx->removeChildByName(nItemKey,true);
		   //nCurColorlayer = (LayerColor *)ParentNode_Item->getChildByName(nItemKey);
		   t_sprintf(nKey,"item_%u",Item[it->second.uId].GetID());
		   nItemKey = nKey;
		   //SkillNode = (Sprite *)ParentNode_Item->getChildByName(nItemKey);
		   ParentNode_ItemEx->removeChildByName(nItemKey,true);
		   //m_ItemData.erase(it);
	   }
   }
   m_ItemData.clear();

	KUiObjAtRegion* pObjs = NULL;
	int nCount= g_pCoreShell->GetGameData(nExboxIndex, 0, 0); //�õ��������Ʒ������

	m_nNumObjects = nCount;

	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(nExboxIndex, (uintptr_t)pObjs, nCount);//���߳�ִ�У�nCountֵ����
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
					Sprite *nItemSpr = (Sprite *)ParentNode_ItemEx->getChildByName(nKey);
					if (!nItemSpr)
					{
						nItemSpr = Sprite::createWithTexture(bgCur);
						//nItemSpr->setAnchorPoint(ax::Vec2(0,0));
						int nikey = std::hash<std::string>{}(nKey);
nItemSpr->setTag(nikey);
						ParentNode_ItemEx->addChild(nItemSpr,1);

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
					if  (nExboxIndex==GDI_ITEM_IN_EX_BOX1)
					   m_ItemData[nKey].nGenkind = UOC_EX_BOX1;         //����Я��
					if  (nExboxIndex==GDI_ITEM_IN_EX_BOX2)
						m_ItemData[nKey].nGenkind = UOC_EX_BOX2;         //����Я��
					if  (nExboxIndex==GDI_ITEM_IN_EX_BOX3)
						m_ItemData[nKey].nGenkind = UOC_EX_BOX3;         //����Я��
					Color4B color(112, 128, 144, 150);
					//t_sprintf(nItemKey,"color_%d",i+1);
					strcat(nItemKey,"_color");
					nKey =nItemKey;
					LayerColor *bgcolorLayer = (LayerColor *)ParentNode_ItemEx->getChildByName(nKey);
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
						ParentNode_ItemEx->addChild(bgcolorLayer,0);
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
					Obj.nContainer = UOC_ITEM_EX;

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
				//}
			}
		}
		free(pObjs); //�ͷ��ڴ�
		pObjs = NULL; //���
	}
}
