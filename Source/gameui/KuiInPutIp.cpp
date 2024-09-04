//�а�ť�ġ������롡�Ի���
//
#include "gamecore/KCore.h"
#include "gameui/Uiglobaldata.h"
#ifndef WIN32
#include <sys/select.h>
#include <ctype.h>
//typedef struct fd_set FD_SET;
#endif
#include "network/Socket.h"

#include "engine/KbugInfo.h"
#include "engine/CXmlStream.h"
#include "KuiInPutIp.h"
#include "Klogin_f.h"
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
#include "engine/dataChecksum.h"

extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

KuiInPutIp::KuiInPutIp()
{
	isOpen = false;
	colorLayer        = NULL;
	red               = NULL;
	AccNode = NULL;
	PasNode = NULL;
	m_AnswerCount = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	s = INVALID_SOCKET;
	nport=INVALID_SOCKET;
	naccIp="";
}

KuiInPutIp::~KuiInPutIp()
{
	isOpen = false;
	colorLayer        = NULL;
}

//����ȷ��������
KuiInPutIp * KuiInPutIp::create(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiInPutIp * popLayer = KuiInPutIp::create();
	//popLayer->setTitle(title);
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiInPutIp::init()
{
	if ( !Layer::init() ){
		return false;
	}
	//����������Ӧ

	this->scheduleUpdate();                   //���ú���
	auto listener = ax::EventListenerTouchOneByOne::create();
ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);   //��ֹ��㴥��
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
	Sprite * spriteBox = Sprite::create("temp/inputip.png");
	ParentNode_ItemEx = spriteBox;
	spriteBox->setPosition(ax::Vec2(winSize.width/2,winSize.height/2-spriteBox->getContentSize().height/2));

	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(visibleSize);//���úʹ��ڵĴ�С
	colorLayer->addChild(spriteBox);
	this->addChild(colorLayer);

	//��ñ���ͼƬ�Ĵ�С
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();
	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С

	return true;
}

void KuiInPutIp::addDialogData()
{
	Size nSize;
	nSize.width =128;
	nSize.height=24;
	m_StartPos.x =10;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =140-24;//+ITEM_CELL_SIZE/2;
	//char nTempStr[128];
	//sprintf(nTempStr,pContent);
	/*Label *pstrLabel = Label::createWithTTF(UTEXT(nTempStr,1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,TextHAlignment::CENTER);//Arial
	pstrLabel->setColor(ax::Color3B::YELLOW);
	pstrLabel->setPosition(ax::Vec2(128/2+30,m_size.height-50));
	ParentNode_ItemEx->addChild(pstrLabel);*/

	/*Label *ptextLabel = Label::createWithTTF(UTEXT("�����Ļ�س�",1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,TextHAlignment::CENTER);//Arial
	ptextLabel->setColor(ax::Color3B::YELLOW);
	ptextLabel->setPosition(ax::Vec2(m_size.width/2,20));
	ParentNode_ItemEx->addChild(ptextLabel);*/

	//����һ������ �ı侫����ɫ
	//red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	//red->retain();
	ax::ui::Scale9Sprite *pAccBg = ax::ui::Scale9Sprite::create("loadingEnd.png");  //"boxgb.png"
	pAccBg->setOpacity(0);

	AccNode = EditBox::create(Size(200,24),pAccBg);
	const ax::Color3B nfcolor ={255,255,255};
	if (AccNode)
	{
		//pEditBox->setContentSize()
		//AccNode->setText("input your account");
		AccNode->setPlaceHolder("Input Server Ip");
		AccNode->setFontColor(nfcolor);
		AccNode->setFontSize(24);
		AccNode->setMaxLength(64);
		AccNode->setInputFlag(ax::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
		AccNode->setInputMode(ax::ui::EditBox::InputMode::ANY);
		AccNode->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//AccNode->setDelegate((EditBoxDelegate*)this);
		AccNode->setPosition(ax::Vec2(m_size.width/2+16,135));
		ParentNode_ItemEx->addChild(AccNode,5);
	}

	//���������
	ax::ui::Scale9Sprite *pPassBg = ax::ui::Scale9Sprite::create("loadingEnd.png"); //"boxgb.png"
	pPassBg->setOpacity(0);
	PasNode = EditBox::create(Size(200,24),pPassBg);
	if (PasNode)
	{
		//pEditBox->setContentSize()
		PasNode->setPlaceHolder("Input Server Ip");
		//PasNode->setText("input your password");
		PasNode->setFontColor(nfcolor);
		PasNode->setFontSize(24);
		PasNode->setMaxLength(64);
		AccNode->setInputFlag(ax::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
		AccNode->setInputMode(ax::ui::EditBox::InputMode::ANY);
		AccNode->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		PasNode->setPosition(ax::Vec2(m_size.width/2+16,100));
		ParentNode_ItemEx->addChild(PasNode,5);
	}
	//
	MenuItemImage *pCancelItem=NULL;
	MenuItemImage *pLoginItem=NULL;
		pLoginItem = MenuItemImage::create(
			"temp/ok.png",
			"temp/ok.png",
			"temp/ok.png",
            CC_CALLBACK_1(KuiInPutIp::mianLoginCallback, this));
		pLoginItem->setPosition(ax::Vec2(origin.x + visibleSize.width/2 - pLoginItem->getContentSize().width/2-249,
			origin.y + pLoginItem->getContentSize().height/2+7));//һ����ť
		pLoginItem->setTag(1);
		pCancelItem = MenuItemImage::create(
			"temp/c.png",
			"temp/c.png",
			"temp/c.png",CC_CALLBACK_1(KuiInPutIp::mianCancelCallback, this));
		pCancelItem->setPosition(ax::Vec2(origin.x + visibleSize.width/2 - pCancelItem->getContentSize().width/2-99,
			origin.y + pCancelItem->getContentSize().height/2+7));//һ����ť
		pCancelItem->setTag(1);
	    Menu* pMenu=NULL;
		pMenu = Menu::create(pLoginItem,pCancelItem,NULL);
		pMenu->setPosition(ax::Vec2::ZERO);//
		ParentNode_ItemEx->addChild(pMenu,2);
	//��ȡ��½���ķ���������������
	m_WritablePath = ax::FileUtils::getInstance()->getWritablePath();

}

void KuiInPutIp::mianCancelCallback(Ref* pSender)
{
	buttonCallBackFunc(pSender,2);
}

void KuiInPutIp::mianLoginCallback(Ref* pSender)
{
	KIniFile pPakList;
	if (pPakList.Load("data/package_temp.ini",true))
	{
		std::string strAcc;
		std::string strPas;
		if (AccNode && PasNode)
		{
			strAcc  = AccNode->getText();
			strPas  = PasNode->getText();

			if  (strPas.empty() || strPas.empty())
			{
				messageBox("Ip Is Empty","Error");
				pPakList.Clear();
				return;
			}
			if  (strAcc!=strPas)
			{
				messageBox("Ip Is Error","Error");
				pPakList.Clear();
				return;
			}

			dataChecksum nurlMask;
			char donwPath[64]={0},nTempPaht[128];
			ZeroMemory(donwPath,sizeof(donwPath));
			ZeroMemory(nTempPaht,sizeof(nTempPaht));
			nurlMask.SimplyDecrypt(donwPath,CLIENT_IP);
			sprintf(nTempPaht, "%s", strAcc.c_str());
			if  (!strstr(nTempPaht,donwPath))
			{
				messageBox("Ip Is Error","Error");
				pPakList.Clear();
				return;
			}

			pPakList.WriteString("Package","down",strAcc.c_str());
			pPakList.Save("data/package_temp.ini");
		}
		pPakList.Clear();
		buttonCallBackFunc(pSender,1);
	}
	else
     messageBox("The Client Is Error","Error");

}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiInPutIp::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

}
//ִ���ϲ����Ļص��������رյ�����
void KuiInPutIp::buttonCallBackFunc(Ref * pSender,int ver)
{
	Node * node = NULL;
	if (pSender)
	{
		node = dynamic_cast<Node *>(pSender);
		node->setTag(ver);
	}
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiInPutIp::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiInPutIp::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
//	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

Rect KuiInPutIp::getRect(Node* pNode,int i)
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
bool KuiInPutIp::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	//case SMCT_UI_RENASCENCE:	             //ѡ������
    //buttonCallBackFunc(NULL);
	return true;
}

void KuiInPutIp::update(float delta)
{
	//messageBox("����ѭ��","update");
	if (isOpen)
	{
	   // m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
	  //  m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
	}
}

//void KuiInPutIp::draw()
//{
//	//messageBox("����ѭ��","draw");
//}


int KuiInPutIp::FilterTextColor(char* pMsgBuff, unsigned short nMsgLength)
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

void KuiInPutIp::editBoxEditingDidBegin(EditBox *editBox)//��ʼʱ�ĺ���
{
	messageBox("�����ʼ","�����ʼ");
}
void KuiInPutIp::editBoxEditingDidEnd(EditBox *editBox) //����ʱ�ĺ���
{

}
void KuiInPutIp::editBoxReturn(EditBox *editBox)         //����������س�ʱ�Ĵ���
{

}
void KuiInPutIp::editBoxTextChanged(EditBox *editBox, const std::string &text)//���ָ��ĺ���
{

}
