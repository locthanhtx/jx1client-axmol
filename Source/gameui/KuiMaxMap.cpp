#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "Uiglobaldata.h"
#include "engine/CXmlStream.h"
#include "KuiMaxMap.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
//#include "engine/KPalette.h"
#include "gamecore/KPlayer.h"
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
#define MAX_LOOP_TIME	180
KuiMaxMap::KuiMaxMap()
{
	isOpen = false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	//pMoneyLabel       = NULL;
	//pXuLabel          = NULL;
	//colorLayer        = NULL;
	red               = NULL;
	clipper           = NULL;
	nPartNode         = NULL;
	content_Map           = NULL;
	//m_nMoney = 0;
	//m_nXu = 0;
	//m_nNumObjects = 0;
	//m_StartPos.x=0;
	//m_StartPos.y=0;
	//m_pUiLayer = NULL;
	//m_pWidget  = NULL;
	//m_array    = NULL;
	//listView   = NULL;
	//nPlayerFun = NULL;
	nMapPicSpr = NULL;
	pPointLabel = NULL;
	BtnFlag     = NULL;
	__loopTime = 0;
}

KuiMaxMap::~KuiMaxMap()
{
	isOpen = false;
	//pMoneyLabel  = NULL;
	//colorLayer   = NULL;
	//CC_SAFE_RELEASE(m_array);
	if (red)
	    red->release();
}

//创建确定弹出框
KuiMaxMap* KuiMaxMap::create(const char* title, Ref* callbackListener, const std::function<void(ax::Node*)>& callfun)
{
	KuiMaxMap * popLayer = KuiMaxMap::create();
	popLayer->addDialogData();
	//设置按钮
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//初始化
bool KuiMaxMap::init()
{
	if ( !Layer::init() ){
		return false;
	}
	//精灵帧缓存
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//添加模态背景
	winSize = ax::Director::getInstance()->getWinSize();
	Color4B color(255,255,255,0);
	LayerColor * colorLayer = LayerColor::create(color);//颜色层
	bgSize  = winSize;//ax::Size(376,326);
	// bgSize.width = 500;
	// bgSize.height = 500;
	colorLayer->setContentSize(winSize);
	ParentNode_Team= colorLayer;//-1+45  +43
	colorLayer->setPosition(ax::Vec2(winSize.width/2-bgSize.width/2,winSize.height/2-bgSize.height/2));
	this->addChild(colorLayer);
	//获得背景图片的大小
	Size contentSize = colorLayer->getContentSize();
	m_size = contentSize;
	m_origin = colorLayer->getPosition();

	//colorLayer->setContentSize(contentSize);//设置和窗口的大小
	//开启触摸响应

	this->scheduleUpdate();                   //调用函数
	//禁止多点触摸
    auto listener = ax::EventListenerTouchAllAtOnce::create();
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void KuiMaxMap::addDialogData()
{
	if  (!g_pCoreShell) return;

	int nRoleMpsX,nRoleMpsY,nMapidx;
	nMapidx = g_GameWorld->getMapIdx();
	g_ScenePlace.GetSceneFocus(nMapidx,nRoleMpsX,nRoleMpsY);
	nRoleDisX = nRoleMpsX-maxMapRc.left*512;     //人物距离原点的距离 offx  宽度
	nRoleDisY = nRoleMpsY-maxMapRc.top*1024;     //人物距离原点的距离 offy  高度
	/*char msg[64];
	int  NewscrX= nRoleMpsX,NewscrY= nRoleMpsY;
	SubWorld[0].GetLocalPositionByMps(NewscrX,NewscrY,0);      //像素坐标－－－＞屏幕坐标
	sprintf(msg,"x:%d,y:%d",NewscrX,NewscrY);
	messageBox(msg,"DSTEN");*/


	Texture2D* nPicTexture=NULL;
	char nSprFileKey[64]={0};
	char mjpgPath[256];
	sprintf(mjpgPath, "%s", maxMapPicPath.c_str());
	unsigned long nFielpahtdwid = g_FileName2Id(mjpgPath);
	sprintf(nSprFileKey,"%u-%d",nFielpahtdwid,0);
	//sprintf(nDebugmsg,"Toa do 6: nXpos:%d-nYpos:%d",xPos,yPos);
	//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nSprFileKey);
	nPicTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFileKey);
	if  (!nPicTexture) return;

	nTextureSize = nPicTexture->getContentSize();
	Color4B color(0,0,0,150);
	LayerColor * bgLayer = LayerColor::create(color);//颜色层
	bgLayer->setAnchorPoint(ax::Vec2(0.5,0.5));
	bgLayer->setContentSize(ax::Size(506,506));
	bgLayer->setPosition(ax::Vec2(m_size.width/2-bgLayer->getContentSize().width/2+45,m_size.height/2-bgLayer->getContentSize().height/2+43));
	ParentNode_Team->addChild(bgLayer);
	clipper = ClippingNode::create();//创建一个可显示区域
	clipper->setTag(1);
	clipper->setContentSize(ax::Size(500,500));
	clipper->setAnchorPoint(ax::Vec2(0.5,0.5));

	clipper->setPosition(ax::Vec2(m_size.width/2+45,m_size.height/2+43));
	//clipper->runAction(RepeatForever::create(CCRotateBy::create(1, 45)));
	ParentNode_Team->addChild(clipper);

	m_pUiLayer = Layer::create();
	ParentNode_Team->addChild(m_pUiLayer);

	ax::ui::Scale9Sprite *pBg;
	pBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	pstrEditBox_d = EditBox::create(Size(140,24),pBg);
	if (pstrEditBox_d)
	{
		//pEditBox->setContentSize()
		//pstrEditBox_d->setText("your money");
		pstrEditBox_d->setPlaceHolder(UTEXT("0/0",1).c_str());
		pstrEditBox_d->setFontColor(ax::Color3B::YELLOW);
		pstrEditBox_d->setFontSize(24);
		pstrEditBox_d->setMaxLength(12);
		pstrEditBox_d->setPosition(ax::Vec2(m_size.width/2+45,m_size.height/2-clipper->getContentSize().height/2+55));
		pstrEditBox_d->setInputMode(ax::ui::EditBox::InputMode::ANY);
		pstrEditBox_d->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		m_pUiLayer->addChild(pstrEditBox_d);

		//增加启动　保存按钮
		Button* button = Button::create();
		button->setTouchEnabled(true);
		button->loadTextures("ui/textbg.png", "ui/textbg.png","");
		button->setTitleFontSize(18);
		button->setTitleText("Tìm");
		button->setPosition(ax::Vec2(m_size.width/2+145,m_size.height/2-clipper->getContentSize().height/2+55));
		button->setTag(1);
		button->addTouchEventListener(CC_CALLBACK_2(KuiMaxMap::oktouchEvent, this));
		m_pUiLayer->addChild(button);
	}

	DrawNode *stencil = DrawNode::create();
	Point rectangle[4];
	rectangle[0] = ax::Vec2(0,0);
	rectangle[1] = ax::Vec2(clipper->getContentSize().width, 0);
	rectangle[2] = ax::Vec2(clipper->getContentSize().width, clipper->getContentSize().height);
	rectangle[3] = ax::Vec2(0,clipper->getContentSize().height);

	Color4F white = {1, 1, 1, 1};
	stencil->drawPolygon(rectangle,4,white,1,white);
	clipper->setStencil(stencil);

	content_Map = Sprite::createWithTexture(nPicTexture);
	content_Map->setTag(2);
	content_Map->setAnchorPoint(ax::Vec2(0,0));
	//content->setPosition(ax::Vec2(clipper->getContentSize().width/2,clipper->getContentSize().height/2));
	content_Map->setPosition(ax::Vec2(-nRoleDisX/16+clipper->getContentSize().width/2,-(nPicTexture->getContentSize().height-nRoleDisY/32)+clipper->getContentSize().height/2));
	//nRoleDisX/16，nRoleDisY/32，Phần bù này trong hình là vị trí của nhân vật chính (so với kích thước của hình)
	clipper->addChild(content_Map);
	//绘制小黄点
	nPartNode = Sprite::create();
	nPartNode->setAnchorPoint(ax::Vec2(0,0));
	//nPartNode->setOpacity(100);//255;//设置透明度为完全不透明（范围0～255）
	Rect CCRectZero = ax::Rect(0,0,10,10);
	nPartNode->setTextureRect(CCRectZero,false,CCRectZero.size);
	nPartNode->setColor(ax::Color3B::YELLOW);
	nPartNode->setPosition(ax::Vec2(clipper->getContentSize().width/2,clipper->getContentSize().height/2));
	clipper->addChild(nPartNode,5);

	/*nPartNode = Sprite::create();
	nPartNode->setAnchorPoint(ax::Vec2(0,0));
	//nPartNode->setOpacity(100);//255;//设置透明度为完全不透明（范围0～255）
	Rect CCRectZero = ax::Rect(0,0,10,10);
	nPartNode->setTextureRect(CCRectZero,false,CCRectZero.size);
	nPartNode->setColor(ax::Color3B::YELLOW);
	nPartNode->setPosition(ax::Vec2(clipper->getContentSize().width/2,clipper->getContentSize().height/2));
	clipper->addChild(nPartNode,5);*/

	pPointLabel   = Label::createWithTTF("0/0","fonts/number_16_w.fnt", 10);
	//pPointLabel->setColor(nColor);
	//ax::Color3B nlineColor={0,0,255};//蓝色 //255,255,0 黄
	//pPointLabel->enableStroke(nlineColor,1);//开始描边
	//char nPoinInfo[32];
	//sprintf(nPoinInfo,"%d/%d",nPosX/8,nPosY/8);
	//pPointLabel->setString(nPoinInfo);
	//pPointLabel->setColor(ax::Color3B::YELLOW);
	pPointLabel->setPosition(ax::Vec2(0,0));
	pPointLabel->setVisible(false);
	clipper->addChild(pPointLabel,5);
	m_bScrolling = false;
	//小旗子 \Spr\Ui3\小地图\地图小旗帜.spr
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\Spr\\Ui3\\小地图\\地图小旗帜.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	//if (bgCur==NULL)
	//	return ;
	BtnFlag = Sprite::createWithTexture(bgCur);
	BtnFlag->setVisible(false);
	BtnFlag->setScale(1.2);
	BtnFlag->setAnchorPoint(ax::Vec2(0,0));
	//nPartNode->setOpacity(100);//255;//设置透明度为完全不透明（范围0～255）
	BtnFlag->setPosition(ax::Vec2(0,0));
	clipper->addChild(BtnFlag,5);
}

//设置确定取消按钮,参数：调用层对象，调用层回调函数
void KuiMaxMap::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	return;
}

//执行上层对象的回调函数，关闭弹出框
void KuiMaxMap::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node= dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//设置tag，在调用层可以获取到
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);
	this->removeFromParentAndCleanup(true);
}

//关闭弹出框
void KuiMaxMap::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}
//点击按钮回调函数
void KuiMaxMap::oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;

	switch (type)
	{
	case Widget::TouchEventType::BEGAN://Touch Down
		{
			if (pSender)
			{
				Button* nTbtn = (Button*)pSender;
				nTbtn->setTitleColor(ax::Color3B::YELLOW);
				if  (nTbtn->getTag()==1)
				{//激活取消
					if  (pstrEditBox_d)
					{
						//char nPoint[64];
						//ZeroMemory(nPoint,sizeof(nPoint));
						std::string nPoint = pstrEditBox_d->getText();

						if  (nPoint.empty())
						{
							Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Toạ độ không được để trống");
							return;
						}
						//开始检测字符的合法性
						std::string ntempstr = nPoint;
						std::string::size_type pos = ntempstr.find("/");
						//ntempstr.replace(pos, ntempstr.length(),"1"); //某个位置后面的全部替换掉
						ntempstr.replace(ntempstr.begin(), ntempstr.end(),'/','1');//整个字符串的 某个字符给替换掉
						std::string strcheck="1";
						strcheck+=ntempstr;
						int count = 0,spaceNum=0,chineseNum=0,other=0,letter=0,xiaoxie=0;
						for(int i = 0; i <strcheck.length();i++)
						{
							//isalnum
							if(isdigit(strcheck[i]))     //判断是否是数字0-9
								count++;
							else if(isspace(strcheck[i]))//判断是否是空格
								++spaceNum;
							else if(isalpha(strcheck[i])==1)//判断是否是字母	 1 为大写 2 为 小写  0 为不是字母
							{
								++letter;
							}
							else if(isalpha(strcheck[i])==2)//判断是否是字母	 1 为大写 2 为 小写  0 为不是字母
							{
								++xiaoxie;
							}
							else if(strcheck[i]&0x80)//if(strcheck[0]>=0xB0)
								++chineseNum;        //判断是否是中文
							else
							{
								++other;
							}
						}

						if (spaceNum>0 || chineseNum>0 || xiaoxie >0 || other>0 || letter>0)
						{ 	//system("pause");
							Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Chỉ nhập số");
							return;
						}
						std::string _xPos = nPoint.substr(0, pos);
						std::string _yPos = nPoint.substr(pos+1, nPoint.length());
						//messageBox(_xPos.c_str(),_yPos.c_str());
						char nTempPoint[64];
						ZeroMemory(nTempPoint,sizeof(nTempPoint));
						sprintf(nTempPoint, "%s", _xPos.c_str());
						int nXpos = g_Atoui(nTempPoint);
						sprintf(nTempPoint, "%s", _yPos.c_str());
						int nYpos = g_Atoui(nTempPoint);
						//Debug
						char nDebugmsg[200];
						sprintf(nDebugmsg,"Toa do 1: nXpos:%d-nYpos:%d",nXpos,nYpos);
						Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nDebugmsg);
						//Debug
						//开始转换成象素坐标
						//伪装成地图主角的坐标
						int _nRoleDisX = nXpos*8*32-maxMapRc.left*512;     //人物距离原点的距离 offx  宽度
						int _nRoleDisY = nYpos*8*64-maxMapRc.top*1024;     //人物距离原点的距离 offy  高度
						//Debug
						char nDebugmsg1[200];
						sprintf(nDebugmsg1,"Toa do 2: nXpos:%d-nYpos:%d",_nRoleDisX,_nRoleDisY);
						Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nDebugmsg1);
						//Debug
						//if (clipper && content)
						//	content->setPosition(ax::Vec2(-_nRoleDisX/16+clipper->getContentSize().width/2,-(nTextureSize.height-_nRoleDisY/32)+clipper->getContentSize().height/2));
						int nX = nXpos*8*32;
						int nY = nYpos*8*64;
						int nZ = 0;
						SubWorld[0].GetLocalPositionByMps(nX, nY, nZ);
						if (g_pCoreShell){
							g_pCoreShell->GotoWhere(nX, nY, 0);
						}

						if (pPointLabel)
						{
							__loopTime = MAX_LOOP_TIME;
							pPointLabel->setVisible(true);

							//Point point = nPartNode->getPosition();//clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
							Point destpoint;
							destpoint.x = -_nRoleDisX/16+clipper->getContentSize().width/2;
							destpoint.y = -(nTextureSize.height-_nRoleDisY/32)+clipper->getContentSize().height/2;
							char nPoinInfo[32];
							sprintf(nPoinInfo,"%d/%d",destpoint.x,destpoint.y);
							// if (g_pCoreShell){
							// 	g_pCoreShell->GotoWhere(destpoint.x, destpoint.y, 0);
							// }
							sprintf(nDebugmsg1,"Toa do 3: nXpos:%d-nYpos:%d",destpoint.x,destpoint.y);
							Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nDebugmsg1);
							pPointLabel->setString(nPoinInfo);
							Point diff = destpoint - content_Map->getPosition();
							nPartNode->setPosition(nPartNode->getPosition() + diff);
							content_Map->setPosition(ax::Vec2(destpoint.x,destpoint.y)); //chuy?n ??ng n?n
							if (clipper && nPartNode)
								pPointLabel->setPosition(ax::Vec2(clipper->getContentSize().width/2,clipper->getContentSize().height/2));
							if (BtnFlag)
							{
								BtnFlag->setVisible(true);
								BtnFlag->setPosition(ax::Vec2(clipper->getContentSize().width/2,clipper->getContentSize().height/2+5));
							}
						}

					}//end edit
				}
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
//重写触摸注册函数，重新给定触摸级别
void KuiMaxMap::registerWithTouchDispatcher()
{
	//这里的触摸优先级设
    // auto touchListener = ax::EventListenerTouchOneByOne::create();
    //    touchListener->setSwallowTouches(true);
    //    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);置为－128，与CCMenu同级，保证了屏蔽下方的触摸
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2,true);
}

Rect KuiMaxMap::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //这个因该是图内偏移
	rc.size       = pNode->getContentSize();
	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //起点最左边
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //最上边    //- rc.origin.y
	return rc;
}
//触摸函数ccTouchBegan，返回true
bool KuiMaxMap::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//Touch *touch = (Touch*)pTouches->anyObject();
	Node *clipper = ParentNode_Team->getChildByTag(1);
	Point point = clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
	m_bScrolling = rect.containsPoint(point); //如果在范围内 就可以拖动
	m_lastPoint = point;

	return true;
}
//移动中
void KuiMaxMap::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("ccTouchMoved");
	//Touch *touch = (Touch*)pTouches->anyObject();
	//if  (nPartNode)
	//	nPartNode->setVisible(false);
	__loopTime = MAX_LOOP_TIME;
	Node *clipper = ParentNode_Team->getChildByTag(1);
	Point point = clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff = (point - m_lastPoint);
	Node *content = clipper->getChildByTag(2);//Đặt vị trí của bức tranh
	content->setPosition((content->getPosition() + diff));
	nPartNode->setPosition((nPartNode->getPosition() + diff)); //di chuyển điểm này cùng nhau
	if  (pPointLabel)
		pPointLabel->setPosition((pPointLabel->getPosition() + diff)); //这个点一起移动
	if  (BtnFlag)
		BtnFlag->setPosition((BtnFlag->getPosition() + diff));         //这个点一起移动

	m_lastPoint = point;
}
//移动结束
void KuiMaxMap::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	KIniFile	IniFile;
	winSize = ax::Director::getInstance()->getWinSize();
	int nRoleMpsX,nRoleMpsY,nMapidx;
	nMapidx = g_GameWorld->getMapIdx();
	g_ScenePlace.GetSceneFocus(nMapidx,nRoleMpsX,nRoleMpsY);
	nRoleDisX = nRoleMpsX-maxMapRc.left*512;
	nRoleDisY = nRoleMpsY-maxMapRc.top*1024;
	int x1,x2,y1,y2;
	if (IniFile.Load("settings\\ToadoML.ini"))
	{
		char szKeyName[10]={0}, szPos[32]={0},debug[50],szWorY[10];
		memset(szKeyName, 0, sizeof(szKeyName));
		memset(szWorY, 0, sizeof(szWorY));
		memset(szPos, 0, sizeof(szPos));

		sprintf(szKeyName,"%d_WorX", nMapidx);
		sprintf(szWorY,"%d_WorY", nMapidx);

		IniFile.GetInteger2("List",szKeyName, &x1, &x2);
		IniFile.GetInteger2("List",szWorY, &y1, &y2);


		sprintf(debug,"X1:%d, Y1:%d, MapID:%d",x1,y1,nMapidx);
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(debug);

	}

	Node *clipper = ParentNode_Team->getChildByTag(1);

	Point point = clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));

	Rect rect = ax::Rect(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
	if (rect.containsPoint(point)){
		Node *content = clipper->getChildByTag(2);
		if (content){
			Point pointInMap = content->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
			BtnFlag->setVisible(true);
			BtnFlag->setPosition(ax::Vec2(point.x,point.y));
			int picposx = pointInMap.x;
			int picposy = -pointInMap.y;
			int cellx = 32;
			int actualcell = 0;
			int mapwith = winSize.width;
			int mapheight = winSize.height;
			int beginnposx = x1*2;
			int beginnposy = y1*2;
			int picwith = 1024;
			int picheight = picwith * mapheight/mapwith;
			if  (picheight>768){
				picheight = 768;
				picwith = picheight * mapwith/mapheight;
			}
			if (mapwith<picwith){
				actualcell = cellx;
				picwith = mapwith;
			}else{
				actualcell = picwith * cellx / (mapwith*2);
			}

			int nx = beginnposx + round(picposx/actualcell);
			int ny = beginnposy + round(picposy/actualcell);
			int nox = picposx - (nx - beginnposx) * actualcell + actualcell /10;
			int noy = picposy - (ny - beginnposy) * actualcell + actualcell /10;
			int curx = (nx + nox/10)*8;
			int cury = (ny + noy/10)*16;

			char nTempPoit[128],nPoinInfo[32];
			// sprintf(nTempPoit,"%s:nx:%d/%d,nox: %d/%d, nChieuRong: %d/%d,mapwith: %d/%d","MapInfo:",nx,ny,curx,cury,x1,y1,picwith,picheight);
			// Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nTempPoit);
			sprintf(nPoinInfo,"%d/%d",curx/8,(cury/16));
			pPointLabel->setVisible(true);
			pPointLabel->setPosition(ax::Vec2(point.x,point.y));
			pPointLabel->setString(nPoinInfo);
			g_ScenePlace.DrawGreenLine(curx/8,(cury/16),true);
		}
	}




	if (!m_bScrolling) return;
	   m_bScrolling = false;


}


void KuiMaxMap::ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{
	auto it = pTouches.begin();
	Touch* touch = (Touch*)(*it);

	if (touch)
	{
		Point location   = touch->getLocation();      //目的的OpenGL坐标系(+x向右　+y向上)屏幕点击处
		Point screenPoint= touch->getLocationInView();
		Node *clipper = ParentNode_Team->getChildByTag(1);
		Point point = clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(touch->getLocationInView()));
		Rect rect = ax::Rect(0, 0, clipper->getContentSize().width, clipper->getContentSize().height);
		if (clipper){
			if (rect.containsPoint(point)){
				char nDebugmsg[200],nDebugmsg1[200],nDebugmsg2[200],nDebugmsg3[200];
				int posX = screenPoint.x;
				int posY = screenPoint.y;
				if  (BtnFlag){
					char nPoinInfo[32];
					BtnFlag->setVisible(true);
					BtnFlag->setPosition(ax::Vec2(posX,posY));
					Point diff = (BtnFlag->getPosition() - content_Map->getPosition());
					nPartNode->setColor(Color3B::GREEN);
					int nX = BtnFlag->getPosition().x*8*32;//nPartNode->getPosition().x/16+clipper->getContentSize().width/2;
					int nY = BtnFlag->getPosition().y*8*64;//(nTextureSize.height-nPartNode->getPosition().y/32)+clipper->getContentSize().height/2;
					int nZ = 0;
					//SubWorld[0].GetMpsByLocalPosition(nX, nY, nZ);

					//SubWorld[0].GetLocalPositionByMps(nX1, nY1, nZ);
					if (g_pCoreShell){

						int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
						sprintf(nDebugmsg,"Toa do 5: nXpos:%d-nYpos:%d-nIndex:%d",nX,nY,nIndex);
						Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nDebugmsg);
						Npc[nIndex].SendSerCommand(do_run, nX, nY);
						// Send to Server
						SendClientCmdRun(nX, nY);
						//g_pCoreShell->GotoWhere(nX1, nY1, 0);
					}

					sprintf(nPoinInfo,"%d/%d",nX,nY);
					//sprintf(nDebugmsg,"nXpos:%d-nYpos:%d",nXpos,nYpos);
					//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nDebugmsg);
				}
			}
		}

		m_bScrolling = rect.containsPoint(point);
		m_lastPoint = point;
	}
	//Touch *touch = (Touch*)pTouches->anyObject();

}

void KuiMaxMap::ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{
	if (!m_bScrolling) return;
	Touch *touch = (Touch*)pTouches.at(0);
	Node *clipper = ParentNode_Team->getChildByTag(1);
	Point point = clipper->convertToNodeSpace(ax::Director::getInstance()->convertToGL(touch->getLocationInView()));
	Point diff = (point - m_lastPoint);
	Node *content = clipper->getChildByTag(2);
	content->setPosition((content->getPosition() + diff));
	m_lastPoint = point;
}

void KuiMaxMap::ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;
}
void KuiMaxMap::update(float delta)
{
	//messageBox("无限循环","update");
	if (g_GameWorld && isOpen && g_pCoreShell)
	{
		if  (!m_bScrolling)
		{//不是拖动状态
			if (__loopTime>0)
				__loopTime--;

			if (__loopTime==0)
			{//恢复原样
				int nRoleMpsX,nRoleMpsY,nMapidx;
				nMapidx = g_GameWorld->getMapIdx();
				g_ScenePlace.GetSceneFocus(nMapidx,nRoleMpsX,nRoleMpsY);
				nRoleDisX = nRoleMpsX-maxMapRc.left*512;     //人物距离原点的距离 offx  宽度
				nRoleDisY = nRoleMpsY-maxMapRc.top*1024;     //人物距离原点的距离 offy  高度
				Point diff;// = ccpSub(destpoint,content->getPosition());
				diff.x = 0;
				diff.y = 0;
				if (clipper && content_Map)
				{
					Point destpoint;
					destpoint.x = -nRoleDisX/16+clipper->getContentSize().width/2;
					destpoint.y = -(nTextureSize.height-nRoleDisY/32)+clipper->getContentSize().height/2;
					diff = (destpoint - content_Map->getPosition());
					content_Map->setPosition(ax::Vec2(destpoint.x,destpoint.y));
				}

				if (clipper && nPartNode)
					nPartNode->setPosition(ax::Vec2(clipper->getContentSize().width/2,clipper->getContentSize().height/2));

				//nPartNode->setPosition(ccpAdd(nPartNode->getPosition(),diff));
				//content->setPosition(ax::Vec2(destpoint.x,destpoint.y)); //背景的移动
				if (clipper && nPartNode)
					pPointLabel->setPosition((pPointLabel->getPosition() + diff));

				if (BtnFlag)
				{
					//BtnFlag->setVisible(true);
					BtnFlag->setPosition((BtnFlag->getPosition() + diff));
					//BtnFlag->setPosition(ax::Vec2(clipper->getContentSize().width/2,clipper->getContentSize().height/2));
				}

			}
		}
	}
}
