//�а�ť�ġ������롡�Ի���
//
#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiProgressbar.h"
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
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

KuiProgressbar::KuiProgressbar()
{
	isOpen = false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	//pMoneyLabel       = NULL;
	//pXuLabel          = NULL;
	colorLayer        = NULL;
	red               = NULL;

	//m_nMoney = 0;
	//m_nXu = 0;
	//m_AnswerCount = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	ZeroMemory(&proData,sizeof(nJinDuClient));
	proSpr = NULL;
	starttime = 0;
}

KuiProgressbar::~KuiProgressbar()
{
	isOpen = false;
	//pMoneyLabel  = NULL;
	colorLayer        = NULL;
	//if (red)
	  //  red->release();
	//red               = NULL;
}

//����ȷ��������
KuiProgressbar * KuiProgressbar::create(KUiJinDuTiaoInfo* pJinDu,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	if (!pJinDu)
		return NULL;

	KuiProgressbar * popLayer = KuiProgressbar::create();
	popLayer->proData.nMsgidx = pJinDu->nMsgIndex;

	popLayer->proData.nTimeVal= pJinDu->nTimes;//��ʱ��
	if (pJinDu->nTimes<=0)
       popLayer->proData.nTimeVal =10;

	t_sprintf(popLayer->ntitle,pJinDu->nTitle);
	t_sprintf(popLayer->proData.szCallFun,pJinDu->nCallFuc);
	t_sprintf(popLayer->proData.szParma,pJinDu->szParma);
	t_sprintf(popLayer->proData.szParmb,pJinDu->szParmb);
	t_sprintf(popLayer->proData.szParmc,pJinDu->szParmc);
	popLayer->addDialogData();
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}


//��ʼ��
bool KuiProgressbar::init()
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
	t_sprintf(nSprName,"\\Spr\\Ui3\\progressbar\\progressbar_back.spr");
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


void KuiProgressbar::addDialogData()
{
	if  (!g_pCoreShell) return;
	Size nSize;
	nSize.width =128;
	nSize.height=24;
	m_StartPos.x =10;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =140-24;//+ITEM_CELL_SIZE/2;
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\progressbar\\progressbar_bit.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);

	if (bgCur==NULL)
		return;

	proSpr = Sprite::createWithTexture(bgCur);
	proSpr->setAnchorPoint(ax::Vec2(0,0));
	proSpr->setPosition(ax::Vec2(10,7));
	proSpr->setScaleX(0); //��������Ϊ��;
	ParentNode_ItemEx->addChild(proSpr);

	Label *ptextLabel = Label::createWithTTF(UTEXT(ntitle,1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,TextHAlignment::CENTER);//Arial
	ptextLabel->setColor(ax::Color3B::YELLOW);
	ptextLabel->setPosition(ax::Vec2(m_size.width/2,m_size.height-15));
	ParentNode_ItemEx->addChild(ptextLabel);

	/*timeval m_pStartUpdate;
	gettimeofday(&m_pStartUpdate,NULL);
	starttime = m_pStartUpdate.tv_sec*1000+m_pStartUpdate.tv_usec/1000;//����timeGetTime();���ϵͳ���е�ʱ��
	*/
	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);

	starttime = pInfo.npTime;  //��¼�򿪴��ڵ�����
	/*Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);//���úʹ��ڵĴ�С
	ParentNode_ItemEx->addChild(colorLayer,1000);
	*/
	//����һ������ �ı侫����ɫ
	//red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	//red->retain();
}


//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiProgressbar::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
}
//ִ���ϲ����Ļص��������رյ�����
void KuiProgressbar::buttonCallBackFunc(Ref * pSender)
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
void KuiProgressbar::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiProgressbar::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-128, true);
}

Rect KuiProgressbar::getRect(Node* pNode,int i)
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
bool KuiProgressbar::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	//case SMCT_UI_RENASCENCE:	//ѡ������
	//	if (g_pCoreShell)
	//	   g_pCoreShell->OperationRequest(GOI_PLAYER_RENASCENCE,0,true);//nSelAction=0��ԭ������, nSelAction=1���س�

    buttonCallBackFunc(NULL);
	return true;
}

void KuiProgressbar::update(float delta)
{
	//ccMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell && proSpr)
	{
		KUiPlayerGameTime	pInfo;
		memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
		ZeroMemory(&pInfo,sizeof(pInfo));
		if (g_pCoreShell)
			g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);
	   float ncTime=0;
	      ncTime=pInfo.npTime-starttime;	//ʱ���

       if (ncTime>0 && ncTime<=proData.nTimeVal)
	   {
		 float sX = ncTime/proData.nTimeVal;
		 proSpr->setScaleX(sX);
	     //m_Progress.Set2IntValue(ncTime,proData.nTimeVal);//���ý���
		 if (ncTime==proData.nTimeVal)
		 {//��ʼ���ͻص�����ִ�нű�
			 if (atoi(proData.szParmc)==1)
			 {//�Ƿ�ѭ��ִ��--����
				 starttime=pInfo.npTime;        //��¼ִ�е�ʱ��
				 g_pCoreShell->OperationRequest(GOI_JINDUTIAO_CALLBACK,(uintptr_t)&proData,0);
				 proSpr->setScaleX(0);
				 //m_pSelf->m_Progress.Set2IntValue(0,proData.nTimeVal); //���ý���
			 }
             else
			 {//һ���� �ɼ�
			     starttime=pInfo.npTime;
				 g_pCoreShell->OperationRequest(GOI_JINDUTIAO_CALLBACK,(uintptr_t)&proData,0);
			     buttonCallBackFunc(NULL);
			 }
		 }
	   }
	}
}

int KuiProgressbar::FilterTextColor(char* pMsgBuff, unsigned short nMsgLength)
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
