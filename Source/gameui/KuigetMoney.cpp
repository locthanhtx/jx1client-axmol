#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuigetMoney.h"
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
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

KuigetMoney::KuigetMoney()
{
	isOpen = false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	//pMoneyLabel       = NULL;
	//pXuLabel          = NULL;
	colorLayer        = NULL;
	//red               = NULL;
	pMoneyEditBox     = NULL;
	//m_nMoney = 0;
	//m_nXu = 0;
	//m_nNumObjects = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	nKind = 0;
	seveMoney = 0;
}

KuigetMoney::~KuigetMoney()
{
	isOpen = false;
	colorLayer = NULL;
	nKind = 0;
}

//����ȷ��������
KuigetMoney * KuigetMoney::create(int nMoney,int nMaxMoney,int nKind,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuigetMoney * popLayer = KuigetMoney::create();

	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	popLayer->nKind = nKind;
	return popLayer;
}

//��ʼ��
bool KuigetMoney::init()
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
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����Ի���\\ȡǮ��.spr");
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
	spriteBox->setPosition(ax::Vec2(winSize.width/2,winSize.height/2));
	this->addChild(spriteBox);

	//��ñ���ͼƬ�Ĵ�С
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();
	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С
	//����������Ӧ

	this->scheduleUpdate();                   //���ú���
	 //��ֹ��㴥��
	return true;
}

void KuigetMoney::addDialogData()
{
	if  (!g_pCoreShell) return;

	//Left=24
	//Top=72
	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;

    /*pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Arial
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	pMoneyLabel->setPosition(ax::Vec2(60,128));
	ParentNode_ItemEx->addChild(pMoneyLabel,1);

	pXuLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Arial
	pXuLabel->setAnchorPoint(ax::Vec2(0,0));
	pXuLabel->setPosition(ax::Vec2(60,105));
	ParentNode_ItemEx->addChild(pXuLabel,1);*/

	ax::ui::Scale9Sprite *pBg = ax::ui::Scale9Sprite::create();
	pMoneyEditBox = EditBox::create(Size(112,24),pBg);
	const ax::Color3B nfcolor ={255,255,255};
	if (pMoneyEditBox)
	{
		//pEditBox->setContentSize()
		//pMoneyEditBox->setText("your money");
		pMoneyEditBox->setPlaceHolder("input money");
		pMoneyEditBox->setFontColor(nfcolor);
		pMoneyEditBox->setFontSize(24);
		pMoneyEditBox->setMaxLength(9);
		pMoneyEditBox->setPosition(ax::Vec2(57,50));
		pMoneyEditBox->setInputMode(EditBox::InputMode::DECIMAL);
		pMoneyEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		ParentNode_ItemEx->addChild(pMoneyEditBox,1,500);
	}

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
void KuigetMoney::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����Ի���\\������������ȷ��.spr");
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
	MenuItemSprite * okConfirm = MenuItemSprite::create(sprite_ok_btn_confirm_normal, sprite_ok_btn_confirm_select, CC_CALLBACK_1(KuigetMoney::getmoneyCallBackFunc, this));
	okConfirm->setPosition(ax::Vec2(-28,11));
	//�ر�
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����Ի���\\������������ȡ��.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_close_normal =  Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_close_select = Sprite::createWithTexture(bgCur);
	MenuItemSprite * closeConfirm = MenuItemSprite::create(sprite_close_normal, sprite_close_select, CC_CALLBACK_1(KuigetMoney::buttonCallBackFunc, this));
	closeConfirm->setPosition(ax::Vec2(28,11));


	Point bgSpritePoint = ParentNode_ItemEx->getPosition();
	Menu * menu = Menu::create(okConfirm,closeConfirm,NULL);
	//menu->setPosition(ax::Vec2(bgSpritePoint.x-m_size.width/2,bgSpritePoint.y-m_size.height/2));//�˵�λ������Ϊ����������
	menu->setPosition(ax::Vec2(m_size.width/2,3));//�˵�λ������Ϊ����������
	ParentNode_ItemEx->addChild(menu);         //�ӵ�������
}

void KuigetMoney::getmoneyCallBackFunc(Ref * pSender)
{
	std::string nCurMoney="";
	if (pMoneyEditBox)
	{
		nCurMoney = pMoneyEditBox->getText();
		if (nCurMoney.length()>10 || nCurMoney.length()<= 0)
		{
#ifdef WIN32
			ccMessageBox("����Ϊ�ղ����Ȳ��ܴ���10","����");
#else
			ccMessageBox(G2U("����Ϊ�ղ����Ȳ��ܴ���10").c_str(),G2U("����").c_str());
#endif
			return;
		}
		//��ʼ����ַ��ĺϷ���
		std::string strcheck="1";
		strcheck+=nCurMoney;
		int count = 0,spaceNum=0,chineseNum=0,other=0,letter=0,xiaoxie=0;
		for(int i = 0; i <strcheck.length();i++)
		{
			//isalnum
			if(isdigit(strcheck[i]))     //�ж��Ƿ�������0-9
				count++;
			else if(isspace(strcheck[i]))//�ж��Ƿ��ǿո�
				++spaceNum;
			else if(isalpha(strcheck[i])==1)//�ж��Ƿ�����ĸ	 1 Ϊ��д 2 Ϊ Сд  0 Ϊ������ĸ
			{
				++letter;
			}
			else if(isalpha(strcheck[i])==2)//�ж��Ƿ�����ĸ	 1 Ϊ��д 2 Ϊ Сд  0 Ϊ������ĸ
			{
				++xiaoxie;
			}
			else if(strcheck[i]&0x80)//if(strcheck[0]>=0xB0)
				++chineseNum;        //�ж��Ƿ�������
			else
			{
				++other;
			}
		}

		if (spaceNum>0 || chineseNum>0 || xiaoxie >0 || other>0 || letter>0)
		{ 	//system("pause");
#ifdef WIN32
			ccMessageBox("ֻ����д����!","����");
#else
			ccMessageBox(G2U("ֻ����д����!").c_str(),G2U("����").c_str());
#endif
			return;
		}

	}
	else
	{
#ifdef WIN32
		ccMessageBox("�������󣮣�","����");
#else
		ccMessageBox(G2U("�������󣮣�").c_str(),G2U("����").c_str());
#endif
		return;
	}
    //��ʼ��ȡǮ����
	unsigned int nMoney=0;
	char nstrInfo[64];
	ZeroMemory(nstrInfo,sizeof(nstrInfo));
	t_sprintf(nstrInfo,nCurMoney.c_str());
	nMoney = g_Atoui(nstrInfo);
	if (nMoney > 0)
	{
		if (nKind)
		{//0 ΪȡǮ ��ȡ�������Ǯ
			g_pCoreShell->OperationRequest(GOI_MONEY_INOUT_STORE_BOX,true,nMoney);
		}
		else
		{// 1 ��Ǯ
			g_pCoreShell->OperationRequest(GOI_MONEY_INOUT_STORE_BOX,false,nMoney);
		}
		buttonCallBackFunc(NULL);
	}
}
//ִ���ϲ����Ļص��������رյ�����
void KuigetMoney::buttonCallBackFunc(Ref * pSender)
{
	Node * node =NULL;
	if (pSender)
	   node = dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);
	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuigetMoney::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}


//��д����ע�ắ�������¸�����������
void KuigetMoney::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
}

Rect KuigetMoney::getRect(Node* pNode,int i)
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
bool KuigetMoney::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void KuigetMoney::update(float delta)
{
	//ccMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	    /*m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
		m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
		char nPointCount[32];
	   if (pMoneyLabel)
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
	   }*/
	}
}
