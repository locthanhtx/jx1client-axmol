#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "Klogin_f.h"
#include "Klogin.h"
#include "KuiSelserver.h"
#include "engine/CXmlStream.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gameui/PopupLayer.h"
#include "gameui/KDrawGameWorld.h"
#include "gameui/KuiAccreg.h"
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
#include "gamescene/KUpdateLayer.h"
#include "KuiAccreg.h"
#include <iostream>
#include <fstream>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#endif
extern iCoreShell * g_pCoreShell;
extern KImageStore2 m_ImageStore; //全局调用
//extern KPakList     g_pPakList;
//extern KPakList * g_pPakList;
KClientCallback g_ClientCallback;

#include <list>
USING_NS_AX;

Scene* Klogin_f::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	Klogin_f *layer = Klogin_f::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Klogin_f::init()
{
	//////////////////////////////
	// 1. super init first
	std::string m_WritablePath = ax::FileUtils::getInstance()->getWritablePath();
	if  (GetRandomNumber(1,10)>=5)
	   m_WritablePath += "music/start.mp3"; //download wav
	else
	   m_WritablePath += "music/start.wav";
	//CocosDenshion::AudioEngine::sharedEngine()->playEffect(m_WritablePath.c_str());//专门用来播放简短的音效
	if  (__pSound)
	     __pSound->play2d(m_WritablePath.c_str(),false);//播放音乐

	if (!Layer::init())
	{
		return false;
	}

    //test
    char nSprName11[128]={0};
    ZeroMemory(nSprName11,sizeof(nSprName11));
    t_sprintf(nSprName11,"\\spr\\Ui3\\ÏµÍ³\\ÏµÍ³2.spr");
    g_StrLower(nSprName11);
    int m_nWidth1, m_nHeight1, nFrams1;
    SPRFRAMSINFO nSprInfo;
    ZeroMemory(&nSprInfo,sizeof(nSprInfo));
    Texture2D *bgCur111 = _getinidata.getinidata_one(nSprName11,0,&m_nWidth1,&m_nHeight1,&nFrams1,&nSprInfo);
    //end test


	m_sprCount =0;
	nTimeCount = 0;
	m_nWidth   = 0;
	m_nHeight  = 0;
	m_AccPadIsOpen = false;
	//pWW        =NULL;
	m_bIsPlayerInGame =false;
	mainlayer = NULL;
	Animate* animate    =NULL;

    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Klogin_f::ccTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(Klogin_f::ccTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Klogin_f::ccTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(Klogin_f::ccTouchCancelled, this);

    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	this->scheduleUpdate();  //遥杆更新调用函数

	char nSprName[128]={0};
	int nFrams=0;
	MenuItemImage *pCancelItem=NULL;
	MenuItemImage *pLoginItem=NULL;
	MenuItemImage *pGameConfigItem=NULL;
	MenuItemImage *pOpenRepItem=NULL;
    //[TODO][zer0kull]
	if (!nIsGaldInit)
	{
		if ((g_pCoreShell = CoreGetShell()))
		{
			g_pCoreShell->SetRepresentAreaSize(visibleSize.width, visibleSize.height);
			g_pCoreShell->SetCallDataChangedNofify(&g_ClientCallback);
			//开始检验
			//g_pCoreShell->StartUp();//开启游戏主线程
			//g_DrawGameWorld.StartUp();
		}
		nIsGaldInit = true; //是否已经初始化过了．．．．
	}

#ifndef WIN32
	dataChecksum nappInfo;
	char nstrps[64]={0},destStr[128];
	ZeroMemory(destStr,sizeof(destStr));
	ZeroMemory(nstrps,sizeof(nstrps));
	//njjj.SimplyDecrypt(bstrmak,nstrmak);
	nappInfo.SimplyDecrypt(nstrps,APP_STRINFO_1);
	strcat(destStr,nstrps);
	ZeroMemory(nstrps,sizeof(nstrps));
	nappInfo.SimplyDecrypt(nstrps,APP_STRINFO_2);
	strcat(destStr,nstrps);
//	m_MobileKey = getappstrInfo();
//	if (strcmp(m_MobileKey.c_str(),destStr)== 0)
		nisgetinfo = true;
#endif


	sprintf(nSprName,"\\spr\\DangNhap\\Login\\btn_StarTroChoi.spr"); //"\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
	g_StrLower(nSprName);
	mainlayer = Layer::create();
	Texture2D *Login = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if (Login)
	{
		char nSprFilePath[64]={0};
		unsigned long nFielpahtdwid = g_FileName2Id(nSprName);//这个市用来标记缓存的 不需要转换编码
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
		Texture2D *pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		Texture2D *pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		Texture2D *pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pLoginItem = MenuItemImage::create();
        pLoginItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pLoginItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pLoginItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pLoginItem->setCallback(CC_CALLBACK_1(Klogin_f::mianLoginCallback, this));

		pLoginItem->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y+280));//一个按钮
		//pLoginItem->setOrderOfArrival(1);
	}
	//系统选项
	nFrams=0;
	sprintf(nSprName,"\\spr\\DangNhap\\Login\\btn_TrangChu.spr");
	g_StrLower(nSprName);
	Texture2D *GameConfig = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if (GameConfig)
	{
		char nSprFilePath[64]={0};
		unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
		Texture2D *pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		Texture2D *pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		Texture2D *pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pGameConfigItem = MenuItemImage::create();
        pGameConfigItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pGameConfigItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pGameConfigItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pGameConfigItem->setCallback(CC_CALLBACK_1(Klogin_f::mianCancelCallback, this));

		pGameConfigItem->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y+230));//一个按钮
		//pGameConfigItem->setOrderOfArrival(1);
	}
	//注册帐号
	nFrams=0;
	sprintf(nSprName,"\\spr\\DangNhap\\Login\\btn_TrangChu.spr");
	g_StrLower(nSprName);
	Texture2D *Rep = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if (Rep)
	{
		char nSprFilePath[64]={0};
		unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
		Texture2D *pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		Texture2D *pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		Texture2D *pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pOpenRepItem = MenuItemImage::create();
        pOpenRepItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pOpenRepItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pOpenRepItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pOpenRepItem->setCallback(CC_CALLBACK_1(Klogin_f::accRegCallback, this));

		pOpenRepItem->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y+120));//一个按钮
	    //pOpenRepItem->setOrderOfArrival(1);
	}
	//退出游戏
	nFrams=0;
	sprintf(nSprName,"\\spr\\DangNhap\\Login\\btn_Exit.spr");
	g_StrLower(nSprName);
	Texture2D *Cancel = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if (Cancel)
	{
		char nSprFilePath[64]={0};
		unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
		Texture2D *pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		Texture2D *pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		Texture2D *pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pCancelItem = MenuItemImage::create();
        pCancelItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pCancelItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pCancelItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pCancelItem->setCallback(CC_CALLBACK_1(Klogin_f::mExit, this));

		pCancelItem->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y+400));//一个按钮
	    //pCancelItem->setOrderOfArrival(1);
	}


	Menu* pMenu=NULL;
	if (Login && Cancel)
	{
		pMenu = Menu::create(pLoginItem,pGameConfigItem,pOpenRepItem,pCancelItem,NULL);

		// sprintf(nSprName,"\\spr\\Ui3\\loginui\\startbtn\\log_2_b.spr");
		// g_StrLower(nSprName);
		// Texture2D *bgSpr = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
		// Sprite *lonbgspr;
		// Sprite *setbgspr;
		// Sprite *repbgspr;
		// Sprite *Canbgspr;
		// if  (bgSpr)
		// {
		// 	lonbgspr = Sprite::createWithTexture(bgSpr);
		// 	lonbgspr->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-164));
		// 	lonbgspr->setOrderOfArrival(0);
		// 	mainlayer->addChild(lonbgspr,0);
		// 	setbgspr = Sprite::createWithTexture(bgSpr);
		// 	setbgspr->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-122));
		// 	setbgspr->setOrderOfArrival(0);
		// 	mainlayer->addChild(setbgspr,0);
		// 	repbgspr = Sprite::createWithTexture(bgSpr);
		// 	repbgspr->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-80));
		// 	repbgspr->setOrderOfArrival(0);
		// 	mainlayer->addChild(repbgspr,0);
		// 	Canbgspr = Sprite::createWithTexture(bgSpr);
		// 	Canbgspr->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-38));
		// 	Canbgspr->setOrderOfArrival(0);
		// 	mainlayer->addChild(Canbgspr,0);
		// }
		// pMenu->alignItemsVertically();     //设置为竖排排列
		//pMenu->alignItemsHorizontally();//默认间隔是kDefaultPadding=5
		//pMenu->alignItemsHorizontallyWithPadding(20);//指定间隔
		//pMenu->alignItemsVertically();//默认间隔也是kDefaultPadding=5
		//pMenu->alignItemsInRows(3,2,1,NULL);
		pMenu->alignItemsVerticallyWithPadding(5);//指定间隔
		pMenu->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));//ax::Vec2::ZERO
		m_sprCount ++;
		//this->addChild(pMenu,2,m_sprCount);
		mainlayer->addChild(pMenu,0);
		this->addChild(mainlayer,2,m_sprCount);
	}

	Sprite *bgSprite=Sprite::create("loginbg.jpg");//登陆背景
	bgSprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	float winw = visibleSize.width;   //获取屏幕宽度
	float winh = visibleSize.height;  //获取屏幕高度
	float spx  = bgSprite->getTextureRect().getMaxX();
	float spy  = bgSprite->getTextureRect().getMaxY();
	bgSprite->setScaleX(winw/spx); //设置精灵宽度缩放比例
	bgSprite->setScaleY(winh/spy);
	m_sprCount++;
	this->addChild(bgSprite,0,m_sprCount);

	 sprintf(nSprName,"\\spr\\Ui3\\loginui\\uidonghua\\yezi.spr");
		 g_StrLower(nSprName);
		 Texture2D *bgCur = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
		 if  (bgCur)
		 {
			Sprite *ySprite=Sprite::createWithTexture(bgCur);
			ySprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x-100, visibleSize.height/2 + origin.y));
			m_sprCount++;
			this->addChild(ySprite,2,m_sprCount);
			char nySprFilePath[64]={0};
			Animation*animation = Animation::create();//创建一个动画
			unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
			for (int i=0;i<nFrams;i++)
			{
				sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,i);
				//messageBox(nSprFilePath,"animation");
                auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                Rect rect = Rect::ZERO;
                rect.size = texture->getContentSize();
                animation->addSpriteFrameWithTexture(texture, rect);
//                texture->release();
            }
			//设置动画帧的时间间隔
			animation->setDelayPerUnit(0.10f);
			//设置播放循环 一直播放 为-1
		    animation->setLoops(-1);
			//设置动画结束后恢复到第一帧 setRestoreOriginalFrame
			animation->setRestoreOriginalFrame(TRUE);
			// 创建一个永远循环的动画
			animate = Animate::create(animation);
			//RepeatForever* forever = RepeatForever::create(animate);
			if (ySprite) {
                auto callFunc = ax::CallFunc::create([this]() {
                    this->finish();
                });
                ySprite->runAction(Sequence::create(animate, callFunc, NULL));
            }
		 }
	return true;
}


void Klogin_f::accRegCallback(Ref* pSender)
{//开始创建注册窗口
	if  (m_AccPadIsOpen)
		return;

    auto callback = [this](ax::Node* sender) {
        Klogin_f::closeAccCallback(sender);
    };

	Layer * sAccLayer = KuiAccreg::create(this, callback);
	m_AccPadIsOpen = true;
	this->addChild(sAccLayer,ORDER_UI_DIALOD);
	if (m_AccPadIsOpen && mainlayer)
	{
		mainlayer->setVisible(false);
	}
}

void Klogin_f::closeAccCallback(Node *pNode)
{
	m_AccPadIsOpen = false;

	if  (mainlayer && !mainlayer->isVisible())
		 mainlayer->setVisible(true);
}
void Klogin_f::mianCancelCallback(Ref* pSender)
{
	/*if (nAcclient)
	{
		nAcclient->Cleanup();
		m_bIsClientConnecting=false;
	}

	ax::Director::getInstance()->replaceScene(KUpdateLayer::scene());*/

	/*MenuItem * nCurMenu= (MenuItem *)pSender;
		//nCurMenu->setVisible(false);

	if (skillstick_f)
	{

		skillstick_f->setSpritePiont(ax::Vec2(nCurMenu->getPosition().x+visibleSize.width/2 + origin.x,nCurMenu->getPosition().y+visibleSize.height/2 + origin.y-100));
		skillstick_f->setSpriteVisible(true);
	}*/
}

void Klogin_f::mianLoginCallback(Ref* pSender)
{
	//开始检测是否配套客户端
	KIniFile _mver;
	if (_mver.Load("\\settings\\ver\\xfsverjx.ini"))
	{
		int nCurVer = 0;
		_mver.GetInteger("CilentVer","mobilever",0,&nCurVer);
		if (curMobileVer==nCurVer)
		{
		   _mver.Clear();
		   ax::Director::getInstance()->replaceScene(KuiSelserver::scene());
		}
		else
		{
			char nTempChar[128];
			ZeroMemory(nTempChar,sizeof(nTempChar));
			_mver.GetString("CilentVer","mobileinfo",UTEXT("请下载最新app版本",1).c_str(),nTempChar,sizeof(nTempChar));
			messageBox(UTEXT(nTempChar,1).c_str(),UTEXT("提示:",1).c_str());
		   _mver.Clear();
		}
	}
	else
		messageBox(UTEXT("请下载最新app版本",1).c_str(),UTEXT("提示:",1).c_str());
	//ax::Director::getInstance()->replaceScene(Klogin::scene());
}

//循环更新虚函数 默认自动调用
void Klogin_f::update(float delta)
{

}


//void Klogin_f::draw()
//{
//}
//
bool Klogin_f::isFileExist(const char* pFileName)
{
	if( !pFileName ) return false;
	//strFilePathName is :/data/data/ + package name
	std::string filePath = ax::FileUtils::getInstance()->getWritablePath();//getWriteablePath();

	filePath += pFileName;

	FILE *fp = fopen(filePath.c_str(),"r");
	if(fp)
	{
		fclose(fp);
		return true;
	}
	return false;
}

void Klogin_f::copyData(const char* pFileName)
{
    std::string strPath = ax::FileUtils::getInstance()->fullPathForFilename(pFileName);
    unsigned int len = 0;
    unsigned char * data = NULL;

    std::ifstream file(strPath, std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        // Lấy kích thước của tệp
        std::streampos size = file.tellg();
        data = new unsigned char[size];
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(data), size);
        file.close();
    }
    if (data) {
        len = ax::FileUtils::getInstance()->getFileSize(strPath);
        std::string destPath = ax::FileUtils::getInstance()->getWritablePath();
        destPath += pFileName;
        FILE *fp = fopen(destPath.c_str(),"w+");

        if (fp) {
            fwrite(data,sizeof(unsigned char),len,fp);
            fclose(fp);
        }

        delete [] data;
        data = NULL;
    }
}


void Klogin_f::finish()
{
	if (!nisgetinfo)
       mExit(NULL);
}
/*
void Klogin_f::onEnter()
{

}

void Klogin_f::onExit()
{

}
//进入后 有 过渡完成
void Klogin_f::onEnterTransitionDidFinish()
{

}*/

bool Klogin_f::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}
void Klogin_f::ccTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void Klogin_f::ccTouchCancelled(Touch *pTouch, Event *pEvent)
{

}
// default implements are used to call script callback if exist
void Klogin_f::ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{

}
void Klogin_f::ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{

}

void Klogin_f::ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{
	auto it = pTouches.begin();
	Touch* touch = (Touch*)(*it);
	Point location = touch->getLocation();//目的的坐标?屏幕点击处
}
void Klogin_f::ccTouchesCancelled(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{

}
void Klogin_f::didAccelerate(Acceleration* pAccelerationValue)
{

}
void Klogin_f::ccTouchEnded(Touch *pTouch, Event *pEvent)
{//convertTouchToNodeSpace
   /*Point touchLocation = convertTouchToNodeSpace(pTouch);
   if(Rect::CCRectContainsPoint(getRect(pSprite), touchLocation))
	{
	   printf("我被点中了!\n");
	}*/
}

void Klogin_f::mExit(Ref* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	messageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	ax::Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif

}
			/*if (lpFileName[i] >= 0xE0) //中文 3个字节
			{
				len = 3;
				messageBox("china","china");
			}
			else if (lpFileName[i]>= 0xC0)
				len = 2;
			else //英文数字
			{
				len = 1;
				messageBox("engish","engish");
			}
unsigned int Klogin_f::g_FileName2Id_(char * lpFileName)
{
	////////////////////////////////////////////////////////////////
	if (lpFileName)
	{
		int len = strlen(lpFileName);
		unsigned int dwID = 0;
		for (int i = 0;i<len; ++i) //lpFileName[i]
		{

#ifdef WIN32
			dwID =(dwID + (i + 1)*lpFileName[i])%0x8000000B*0xFFFFFFEF;//0x8000000B*0xFFFFFFEF;
#else       //每个字符就必须增加 在android 下
			if(lpFileName[i]&0x80)
			{//是中文
				dwID =(dwID + (i + 1)*(lpFileName[i]+0xFFFFFF00))%0x8000000B*0xFFFFFFEF;//0x8000000B*0xFFFFFFEF;
				dwID =(dwID + (i + 2)*(lpFileName[i+1]+0xFFFFFF00))%0x8000000B*0xFFFFFFEF;
				i++;
			}
			else
				dwID =(dwID + (i + 1)*lpFileName[i])%0x8000000B*0xFFFFFFEF;//0x8000000B*0xFFFFFFEF;
#endif
		}
		return (dwID^305419896);	    //0x12345678  305419896
	}
	return (unsigned int)(-1);
}*/
