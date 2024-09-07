#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "Uiglobaldata.h"
#include "engine/CXmlStream.h"
#include "KuiMoveToMap.h"
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
#include "gamescene/KScenePlaceC.h"
#include "gamecore/KSubWorld.h"
//#include "gamecore/KSkillManager.h"
//#include "gamecore/KItem.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;
#define	WORLD_MAP_POS_FILE		"\\ui\\С��ͼ_ɽ����ͼ��_͸����ť.ini"
KuiMoveToMap::KuiMoveToMap(const std::function<void(ax::Node*)>& callfun) : m_callfun(callfun)
{
	isOpen = false;
	red               = NULL;
	clipper           = NULL;
	content_Map       = NULL;
	m_PosCount        = 0;
	nMapPicSpr = NULL;
	ZeroMemory(m_MoveMapBtn,sizeof(mapListData)*MAX_COUNT_MAPS);
}

KuiMoveToMap::~KuiMoveToMap()
{
	isOpen = false;
	if (red)
	    red->release();
}

//����ȷ��������
KuiMoveToMap * KuiMoveToMap::create(const char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiMoveToMap * popLayer = KuiMoveToMap::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiMoveToMap::init()
{
	if ( !Layer::init() ){
		return false;
	}
	//����֡����
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//���ģ̬����
	winSize = ax::Director::getInstance()->getWinSize();
	Color4B color(255,255,255,0);
	LayerColor * colorLayer = LayerColor::create(color);//��ɫ��
	bgSize  = winSize;//ax::Size(376,326);
	colorLayer->setContentSize(winSize); //������Ļ
	ParentNode_Team= colorLayer;         //-1+45  +43
	colorLayer->setPosition(ax::Vec2(winSize.width/2-bgSize.width/2,winSize.height/2-bgSize.height/2));
	this->addChild(colorLayer);
	//��ñ���ͼƬ�Ĵ�С
	Size contentSize = colorLayer->getContentSize();
	m_size = contentSize;
	m_origin = colorLayer->getPosition();

	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С
	//����������Ӧ
	this->scheduleUpdate();                   //���ú���

    auto listener = ax::EventListenerTouchOneByOne::create();
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void KuiMoveToMap::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*int nRoleMpsX,nRoleMpsY,nMapidx;
	nMapidx = g_GameWorld->getMapIdx();
	g_ScenePlace.GetSceneFocus(nMapidx,nRoleMpsX,nRoleMpsY);
	nRoleDisX = nRoleMpsX-maxMapRc.left*512;     //�������ԭ��ľ��� offx  ���
	nRoleDisY = nRoleMpsY-maxMapRc.top*1024;     //�������ԭ��ľ��� offy  �߶�
	/*char msg[64];
	int  NewscrX= nRoleMpsX,NewscrY= nRoleMpsY;
	SubWorld[0].GetLocalPositionByMps(NewscrX,NewscrY,0);      //�������꣭��������Ļ����
	sprintf(msg,"x:%d,y:%d",NewscrX,NewscrY);
	messageBox(msg,"DSTEN");*/
	//\Spr\Ui3\С��ͼ\�����ɽ��ͼ.spr


	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	Texture2D* nPicTexture=NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\Spr\\Ui3\\С��ͼ\\�����ɽ��ͼ.spr");
	g_StrLower(nSprName);

	nPicTexture = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (nPicTexture==NULL)
		return ;

	nTextureSize = nPicTexture->getContentSize();
	Color4B color(0,0,0,150);
	LayerColor * bgLayer = LayerColor::create(color);//��ɫ��
	bgLayer->setAnchorPoint(ax::Vec2(0.5,0.5));
	bgLayer->setContentSize(ax::Size(376,326));  //ax::Size(376,326)
	bgLayer->setPosition(ax::Vec2(m_size.width/2-bgLayer->getContentSize().width/2+45,m_size.height/2-bgLayer->getContentSize().height/2+43));
	ParentNode_Team->addChild(bgLayer);

	clipper = ClippingNode::create();//����һ������ʾ����
	clipper->setTag(1);
	clipper->setContentSize(ax::Size(370,320)); //ax::Size(370,320)
	clipper->setAnchorPoint(ax::Vec2(0.5,0.5));
	//clipper->setPosition(ax::Vec2(m_size.width/2-clipper->getContentSize().width/2+45,m_size.height/2-clipper->getContentSize().height/2+43));
	clipper->setPosition(ax::Vec2(m_size.width/2+45,m_size.height/2+43));//GL POIT
	//clipper->runAction(RepeatForever::create(CCRotateBy::create(1, 45)));
	ParentNode_Team->addChild(clipper);

	Size nSize;
	nSize.width =376;
	nSize.height=16;
	Label *pdescInfo =  Label::createWithTTF(UTEXT("����հ�(������)���϶�,�����Ӧ��ͼ�ƶ�",1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,TextHAlignment::CENTER);//Arial
	//pdescInfo->setColor(ax::Color3B::YELLOW);
	pdescInfo->setPosition(ax::Vec2(m_size.width/2+40,m_size.height/2-clipper->getContentSize().height/2+35));
	ParentNode_Team->addChild(pdescInfo,10);

	content_Map = Sprite::createWithTexture(nPicTexture);
	content_Map->setTag(2);
	content_Map->setAnchorPoint(ax::Vec2(0,0));
	//content->setPosition(ax::Vec2(clipper->getContentSize().width/2,clipper->getContentSize().height/2));
	//content_Map->setPosition(ax::Vec2(-nRoleDisX/16+clipper->getContentSize().width/2,-(nPicTexture->getContentSize().height-nRoleDisY/32)+clipper->getContentSize().height/2));
	//nRoleDisX/16��nRoleDisY/32��ͼ�ڵ����ƫ�ƾ������ǵ�λ��(�����ͼƬ�Ĵ�С��˵)
	content_Map->setPosition(ax::Vec2(0,0));//������ڵ������
	clipper->addChild(content_Map);//���ñ��ü��ľ���

	m_pUiLayer = Layer::create();
	//ParentNode_Team->addChild(m_pUiLayer);
	content_Map->addChild(m_pUiLayer);

	KIniFile	Ini;
	if (Ini.Load(WORLD_MAP_POS_FILE))
	{
		Ini.GetInteger("BUTTON_COUNT","Count",0,&m_PosCount);
		//Width=754
		//Height=578
		if (m_PosCount>0 && m_PosCount<MAX_COUNT_MAPS)
		{
			char nBtn[32]={0};
			int nLeft,nTop,nWidth,nHeight,nSubwordId,nPosX,nPosY;
			for (int i=0;i<m_PosCount;++i)
			{
				sprintf(nBtn,"BUTTON_%d",i);
				Ini.GetInteger(nBtn,"Left",0,&nLeft);
				Ini.GetInteger(nBtn,"Top",0,&nTop);
				Ini.GetInteger(nBtn,"Width",0,&nWidth);
				Ini.GetInteger(nBtn,"Height",0,&nHeight);
				Ini.GetInteger(nBtn,"SubwordId",0,&nSubwordId);
				Ini.GetInteger(nBtn,"TargetPosX",0,&nPosX);
				Ini.GetInteger(nBtn,"TargetPosY",0,&nPosY);
				m_MoveMapBtn[i].m_SubwordId = nSubwordId;
				m_MoveMapBtn[i].m_PosX      = nPosX;
				m_MoveMapBtn[i].m_PosY      = nPosY;
				m_MoveMapBtn[i].m_MoveMapBtn = Button::create();
				m_MoveMapBtn[i].m_MoveMapBtn->setTouchEnabled(true);
				m_MoveMapBtn[i].m_MoveMapBtn->loadTextures("btn_n.png", "loadingStart.png","");
				m_MoveMapBtn[i].m_MoveMapBtn->setScale9Enabled(true);
				m_MoveMapBtn[i].m_MoveMapBtn->setContentSize(ax::Size(nWidth,nHeight));
				//m_MoveMapBtn[i].m_MoveMapBtn->setContentSize(ax::Size(nWidth,nHeight));
				//m_MoveMapBtn[i].m_MoveMapBtn->ignoreContentAdaptWithSize(true);
				m_MoveMapBtn[i].m_MoveMapBtn->setPosition(ax::Vec2(nLeft+20,nTextureSize.height-nTop-8));
				m_MoveMapBtn[i].m_MoveMapBtn->setTag(i+1);
                int nBtn_tmp = std::hash<std::string>{}(nBtn);
				m_MoveMapBtn[i].m_MoveMapBtn->setTag(nBtn_tmp);
				m_MoveMapBtn[i].m_MoveMapBtn->addTouchEventListener(CC_CALLBACK_2(KuiMoveToMap::oktouchEvent, this));
				/*char nTemp[64],mapName[128];
				ZeroMemory(nTemp,sizeof(nTemp));
				ZeroMemory(mapName,sizeof(mapName));
				sprintf(nTemp,"%d_name",nSubwordId);
				g_NpcMapDropRate.GetString("List",nTemp,0,mapName,sizeof(mapName));
				m_MoveMapBtn[i].m_MoveMapBtn->setTitleText(UTEXT(mapName,1).c_str());
				*/
				m_pUiLayer->addChild(m_MoveMapBtn[i].m_MoveMapBtn);
			}
		}

	}
	Ini.Clear();

	DrawNode *stencil = DrawNode::create();
	Point rectangle[4];
	rectangle[0] = ax::Vec2(0,0);
	rectangle[1] = ax::Vec2(clipper->getContentSize().width, 0);
	rectangle[2] = ax::Vec2(clipper->getContentSize().width, clipper->getContentSize().height);
	rectangle[3] = ax::Vec2(0,clipper->getContentSize().height);

	Color4F white = {1, 1, 1, 1};
	stencil->drawPolygon(rectangle,4,white,1,white);
	clipper->setStencil(stencil); //����ģ��

	m_bScrolling = false;
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiMoveToMap::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	return;
}

//ִ���ϲ����Ļص��������رյ�����
void KuiMoveToMap::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node= dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);
	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiMoveToMap::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}
//�����ť�ص�����
void KuiMoveToMap::oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;

	switch (type)
	{
	case Widget::TouchEventType::BEGAN://Touch Down
		{
			if (pSender)
			{
				Button* nTbtn = (Button*)pSender;
				if (nTbtn)
				{
					nTbtn->setTitleColor(ax::Color3B::YELLOW);
					int nSelIndex = nTbtn->getTag()-1;
					g_pCoreShell->OperationRequest(GOI_MAP_SUNYI,m_MoveMapBtn[nSelIndex].m_SubwordId,1);
				}
				buttonCallBackFunc(NULL);
				break;
/*
				Node *clipper = ParentNode_Team->getChildByTag(1);
				Node *content = clipper->getChildByTag(2);
				Point diff = ccpSub(content->getPosition(),ax::Vec2(0,0));//ƫ����ԭ���ĵط��ľ��������ֵ
				Point curPos = ccpAdd(nTbtn->getPosition(),diff);
				curPos = ccpAdd(curPos,ax::Vec2(clipper->getPosition().x-clipper->getContentSize().width/2,clipper->getPosition().y-clipper->getContentSize().height/2));
				//Point point = clipper->convertToNodeSpace(curPos);
				//Rect rect = ax::Rect(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
				//m_bScrolling = rect.containsPoint(point);
				char nTempPoit[128];
				sprintf(nTempPoit,"%s:%f/%f",nTbtn->getTitleText(),curPos.x,curPos.y);
				messageBox(nTempPoit,"test");
*/
			}
		}
		break;
	case Widget::TouchEventType::MOVED://Touch Move
		break;
	case Widget::TouchEventType::ENDED://Touch Up
		{
			if (pSender)
			{
				Button* nTbtn = (Button*)pSender;
				nTbtn->setTitleColor(ax::Color3B::WHITE);
				if  (nTbtn->getTag()==1)
				{

				}
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
//��д����ע�ắ�������¸�����������
void KuiMoveToMap::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2,true);
}

Rect KuiMoveToMap::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	rc.size       = pNode->getContentSize();
	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y
	return rc;
}
//��������ccTouchBegan������true
bool KuiMoveToMap::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//Touch *touch = (Touch*)pTouches->anyObject();
	Node *clipper = ParentNode_Team->getChildByTag(1);
	Point point = clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
	m_bScrolling = rect.containsPoint(point); //����ڷ�Χ�� �Ϳ����϶�
	m_lastPoint = point;
	/*if (!m_bScrolling)
	    button->setTouchEnabled(false);
	else
		button->setTouchEnabled(true);*/
	return true;
}
//�ƶ���
void KuiMoveToMap::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	//Touch *touch = (Touch*)pTouches->anyObject();
	//if  (nPartNode)
	//	nPartNode->setVisible(false);
	Node *clipper = ParentNode_Team->getChildByTag(1);
	Point point = clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff = point - m_lastPoint;
	Node *content = clipper->getChildByTag(2);//����ͼƬ��λ��
	content->setPosition(content->getPosition() + diff);
	m_lastPoint = point;
}
//�ƶ�����
void KuiMoveToMap::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	//if  (nPartNode)
	//	nPartNode->setVisible(true);

	if (!m_bScrolling) return;
	   m_bScrolling = false;

}


void KuiMoveToMap::ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{
	Touch *touch = (Touch*)pTouches.at(0);
	Node *clipper = ParentNode_Team->getChildByTag(1);
	Point point = clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(touch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
	m_bScrolling = rect.containsPoint(point);
	m_lastPoint = point;
}

void KuiMoveToMap::ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{
	if (!m_bScrolling) return;
	Touch *touch = (Touch*)pTouches.at(0);
	Node *clipper = ParentNode_Team->getChildByTag(1);
	Point point = clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(touch->getLocationInView()));
	Point diff = (point - m_lastPoint);
	Node *content = clipper->getChildByTag(2);
	content->setPosition(content->getPosition() + diff);
	m_lastPoint = point;
}

void KuiMoveToMap::ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;
}
void KuiMoveToMap::update(float delta)
{
	//messageBox("����ѭ��","update");
	if (g_GameWorld && isOpen && g_pCoreShell)
	{
		if  (clipper)
		{
			Node *content = clipper->getChildByTag(2);//����ͼƬ��λ��)
			if (!content) return;
			//Point point = clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
			for (int i=0;i<m_PosCount;++i)
			{
				if (m_MoveMapBtn[i].m_SubwordId>0 && m_MoveMapBtn[i].m_MoveMapBtn)
				{

					Point diff = (content->getPosition() - ax::Vec2(0,0));//ƫ����ԭ���ĵط��ľ��������ֵ
					Point curPos = (m_MoveMapBtn[i].m_MoveMapBtn->getPosition() + diff);
					curPos = (curPos + ax::Vec2(clipper->getPosition().x-clipper->getContentSize().width/2,clipper->getPosition().y-clipper->getContentSize().height/2));

					Point newPoint = clipper->convertToNodeSpace(curPos);
					Rect rect = ax::Rect(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
					bool m_IsIn = rect.containsPoint(newPoint);

					//Rect rect = ax::Rect(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
					//bool m_IsIn = rect.containsPoint(clipper->convertToNodeSpace(content->getPosition() + diff));
					if  (m_IsIn)//�ڷ�Χ��
						m_MoveMapBtn[i].m_MoveMapBtn->setTouchEnabled(true);
					else
						m_MoveMapBtn[i].m_MoveMapBtn->setTouchEnabled(false);
				}
			}

		}
		return;
	}
}
