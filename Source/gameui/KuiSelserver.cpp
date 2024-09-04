#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "KuiSelserver.h"
#include "Klogin_f.h"
#include "Klogin.h"
#include "KSelPlayer.h"
#include "engine/CXmlStream.h"
#include "gamecore/CoreShell.h"
#include "gameui/PopupLayer.h"
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
//#include "network/Socket.h"
extern iCoreShell*	g_pCoreShell;
extern KImageStore2 m_ImageStore;
//extern KPakList     g_pPakList;
//extern KPakList * g_pPakList;
#include <list>
#include <iostream>
#include <fstream>
USING_NS_AX;

KuiSelserver::KuiSelserver()
{
	m_sprCount = 0;
	m_nWidth   = 0;
	m_nHeight  = 0;
	animate    = nullptr;
	nserCount  = 0;
	nCurSelIndex = -1;
	m_bIsPlayerInGame =false;
}

KuiSelserver::~KuiSelserver()
{
	nserlist.Clear();
}

Scene* KuiSelserver::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	KuiSelserver *layer = KuiSelserver::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool KuiSelserver::init()
{
	if (!Layer::init()) {
        return false;
    }

    m_Status = LL_S_WAIT_INPUT_ACCOUNT;
    char nSprName[128]={0};
    int nFrams=0;

    Sprite *bgSprite=Sprite::create("loginbg.jpg");
	bgSprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	float winw = visibleSize.width;
	float winh = visibleSize.height;
	float spx  = bgSprite->getTextureRect().getMaxX();
	float spy  = bgSprite->getTextureRect().getMaxY();
	bgSprite->setScaleX(winw/spx);
	bgSprite->setScaleY(winh/spy);
	m_sprCount++;
	this->addChild(bgSprite,0,m_sprCount);

    sprintf(nSprName, "%s", "\\spr\\DangNhap\\ChonMayChu\\XacNhan.spr");
    g_StrLower(nSprName);
    MenuItemImage *pCancelItem = nullptr;
    MenuItemImage *pLoginItem = nullptr;
    Texture2D *Login = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
    if  (Login)
    {
        char nSprFilePath[64]={0};
        DWORD nFielpahtdwid = g_FileName2Id(nSprName);
        sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
        Texture2D *pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
        Texture2D *pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pLoginItem = MenuItemImage::create();
        pLoginItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pLoginItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pLoginItem->setDisabledImage(Sprite::createWithTexture(pSelectedTexture));
        pLoginItem->setCallback(CC_CALLBACK_1(KuiSelserver::mianLoginCallback, this));
    }

    nFrams=0;
    sprintf(nSprName, "%s", "\\spr\\DangNhap\\ChonMayChu\\HuyBo.spr");
    g_StrLower(nSprName);
    Texture2D *Cancel = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
    if (Cancel)
    {
        char nSprFilePath[64]={0};
        DWORD nFielpahtdwid = g_FileName2Id(nSprName);
        sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
        Texture2D *pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
        Texture2D *pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pCancelItem = MenuItemImage::create();
        pCancelItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pCancelItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pCancelItem->setDisabledImage(Sprite::createWithTexture(pSelectedTexture));
        pCancelItem->setCallback(CC_CALLBACK_1(KuiSelserver::mianCancelCallback, this));
    }

    sprintf(nSprName, "%s", "\\spr\\DangNhap\\ChonMayChu\\ChonMayChu.spr");
    g_StrLower(nSprName);
    nFrams=0;

    Texture2D *Cur = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
    testSprite = nullptr;
    if (Cur)
    {
        testSprite = Sprite::createWithTexture(Cur);

        if (testSprite)
        {
            testSprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
            this->addChild(testSprite,2);

            Menu* pMenu = nullptr;
            if (Login && Cancel)
            {
                pMenu = Menu::create(pLoginItem,pCancelItem,nullptr);
                Size menuSize = testSprite->getContentSize();
                menuSize.height = pLoginItem->getContentSize().height;
                pMenu->setContentSize(menuSize);
                Size spriteSize = testSprite->getContentSize();

                pMenu->setPosition(ax::Vec2(spriteSize.width / 2 + 25, menuSize.height - 5));
                testSprite->addChild(pMenu, 2);
                pMenu->alignItemsHorizontallyWithPadding(15);
            }
        }
    }

    if (nserlist.Load("\\Settings\\Ver_vn\\FsjxClient.ini"))
    {
        nserlist.GetInteger("List","RegionCount",0,&nserCount);
        Menu* serlistMenu = Menu::create(nullptr);
        serlistMenu->setAnchorPoint(ax::Vec2(0,0));
        serlistMenu->setPosition(ax::Vec2(testSprite->getContentSize().width / 2, testSprite->getContentSize().height / 2));
        testSprite->addChild(serlistMenu,4);
        int offSetX = 0, offSetY = 0, nfontSize = 12;

        for (int i=0; i < nserCount; ++i)
        {
            Sprite *serBgSor = Sprite::create("ui/btn_sel.png");

            char strSerName[128],cckey[32];
            sprintf(cckey,"Region_%d",i);
            nserlist.GetString(cckey,"0_Title","",strSerName,sizeof(strSerName));

            auto mItemServer = MenuItemSprite::create(
                    serBgSor,
                    serBgSor,
                    CC_CALLBACK_1(KuiSelserver::selServerCallBack, this)
            );

            mItemServer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

            auto serLabel = Label::createWithTTF(UTEXT(strSerName,1).c_str(),UI_GAME_FONT_VN,nfontSize);
            serLabel->setColor(Color3B(0,255,0));
            serLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            serLabel->setPosition(mItemServer->getContentSize() / 2);

            mItemServer->setPosition(ax::Vec2(offSetX, offSetY));
            mItemServer->setTag(i + 1);
            mItemServer->addChild(serLabel);

            int nIsOpen=0;
            nserlist.GetInteger(cckey,"0_IsOpen",0,&nIsOpen);
            if (nIsOpen==0)
                mItemServer->setEnabled(false);
            serlistMenu->addChild(mItemServer, i + 1);
            offSetY -=30;
        }
    }

	return true;
}

void KuiSelserver::selServerCallBack(Ref* menuItem)
{
	if (menuItem)
	{
		MenuItemLabel *nCerBox  = (MenuItemLabel * )menuItem;
		KIniFile nClentMak;
		if (!nClentMak.Load(CLIENT_VER_FILE))
		{
#ifdef WIN32
			messageBox("?��????","???:");
#else
			messageBox(UTEXT("?��????",1).c_str(),UTEXT("???:",1).c_str());
#endif
			return;
		}

		char nstrmak[64]={0},bstrmak[64]={0};
		ZeroMemory(nstrmak,sizeof(nstrmak));
		ZeroMemory(bstrmak,sizeof(bstrmak));
		nClentMak.GetString("CilentVer","mobilemak","0yUDTM0xXm7ZrAjDj7Gf00Mw72fwnrBv",nstrmak,sizeof(nstrmak));
		nClentMak.Clear();

		dataChecksum njjj;
		char nstrps[64]={0};
		ZeroMemory(nstrps,sizeof(nstrps));
		njjj.SimplyDecrypt(bstrmak,nstrmak);
		njjj.SimplyDecrypt(nstrps,CLIENT_MASK);

		if (!strstr(nstrps,bstrmak))
		{
#ifdef WIN32
			messageBox("?��????","???:");
#else
			messageBox(UTEXT("?��????",1).c_str(),UTEXT("???:",1).c_str());
#endif
			return;
		}

		if (nAcclient)
		{
			   KIniFile dwmask;
			   std::string fielDir ="";
#ifdef WIN32
			   //fielDir = std::string("paklist.ini");
			   if (_clientlanguage!=1)
				   fielDir = std::string("paklist_vn.ini");
			   else
				   fielDir = std::string("paklist.ini");
#else
			   //fielDir = std::string("data\\paklist.ini");
			   if (_clientlanguage!=1)
				   fielDir = std::string("data\\paklist_vn.ini");
			   else
				   fielDir = std::string("data\\paklist.ini");
#endif
			   char nTempstr[128];
			   sprintf(nTempstr, "%s", fielDir.c_str());

			   if (!_clientNoNetWork && !dwmask.Load(nTempstr))
			   {
#ifdef WIN32
					messageBox("????????????...","????");
#else
					messageBox(UTEXT("????????????...",1).c_str(),UTEXT("????",1).c_str());
#endif
					return;
			    }
			    dwmask.Clear();

				char downUrlmask[64];
				ZeroMemory(downUrlmask,sizeof(downUrlmask));
				//dwmask.GetString("list","accinfo","",downUrlmask,sizeof(downUrlmask));

				char cckey[32];
				sprintf(cckey,"Region_%d",nCerBox->getTag()-1);
				nserlist.GetString(cckey,"0_Address","",downUrlmask,sizeof(downUrlmask));

				dataChecksum nurlMask;
				char donwPath[64]={0};
				ZeroMemory(donwPath,sizeof(donwPath));
				nurlMask.SimplyDecrypt(donwPath,downUrlmask);

				/*if  (_clientNoNetWork)
				{//????
					KIniFile nTempIni;
					if (nTempIni.Load("data\\package_temp.ini"))
					{
						nTempIni.GetString("Package","down","",donwPath,sizeof(donwPath));

						dataChecksum nurlMask;
						char nTempPaht[64];
						ZeroMemory(nTempPaht,sizeof(nTempPaht));
						nurlMask.SimplyDecrypt(nTempPaht,CLIENT_IP);
						if  (!strstr(donwPath,nTempPaht))
						{
#ifdef WIN32
							messageBox("????????????...","????");
#else
							messageBox(UTEXT("????????????...",1).c_str(),UTEXT("????",1).c_str());
#endif
							nTempIni.Clear();
							return;
						}

						nTempIni.Clear();
					}
					else
						sprintf(donwPath,"192.168.1.128");
				}*/

				if (nAcclient->FsGameServerConnectTo(donwPath,56722))
				{//????????????
					//???????????????
					m_ClientChoices.nServerRegionIndex = nCerBox->getTag();
					nAcclient->Startup();//???????????????
					m_bIsClientConnecting=true;
				}
				else
				{
					nAcclient->Cleanup();
					m_bIsClientConnecting=false;
#ifdef WIN32
					//messageBox("????????????...","????");
					messageBox(donwPath,"Connect IP:");
#else
					//messageBox(UTEXT("????????????...",1).c_str(),UTEXT("????",1).c_str());
					messageBox(donwPath,"Connect IP:");
#endif
					return;
				}
	   }
	  ax::Director::getInstance()->replaceScene(Klogin::scene());
	}
}

void KuiSelserver::mianCancelCallback(Ref* pSender)
{
	m_ClientChoices.nServerRegionIndex =-1;
	if (nAcclient)
	{
		m_bIsClientConnecting=false;
		m_bIsPlayerInGame =false;
		nAcclient->Cleanup();

	}
	ax::Director::getInstance()->replaceScene(Klogin_f::scene());
}

void KuiSelserver::mianLoginCallback(Ref* pSender)
{

}

//????????�D?? ??????????
void KuiSelserver::update(float delta)
{
}

//
bool KuiSelserver::isFileExist(const char* pFileName)
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

void KuiSelserver::copyData(const char* pFileName)
{
    std::string strPath = ax::FileUtils::getInstance()->fullPathForFilename(pFileName);
    unsigned int len = 0;
    unsigned char * data = nullptr;

    std::ifstream file(strPath, std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streampos size = file.tellg();
        data = new unsigned char[size];
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(data), size);
        file.close();
    }
    len = ax::FileUtils::getInstance()->getFileSize(strPath);

    std::string destPath = ax::FileUtils::getInstance()->getWritablePath();//getWriteablePath();
    destPath += pFileName;
    FILE *fp = fopen(destPath.c_str(),"w+");
    if (fp)
    {
        fwrite(data,sizeof(unsigned char),len,fp);
        fclose(fp);
        fp =nullptr;
    }

    if (data)
    {
        delete [] data;
        data = nullptr;
    }
}

void KuiSelserver::finish()
{

}

bool KuiSelserver::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}
void KuiSelserver::ccTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void KuiSelserver::ccTouchCancelled(Touch *pTouch, Event *pEvent)
{

}
// default implements are used to call script callback if exist
void KuiSelserver::ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{

}
void KuiSelserver::ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{

}

void KuiSelserver::ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{
	auto it = pTouches.begin();
	Touch* touch = (Touch*)(*it);

	Point location = touch->getLocation();//?????????????????
}

void KuiSelserver::ccTouchesCancelled(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{

}

void KuiSelserver::didAccelerate(Acceleration* pAccelerationValue)
{

}
void KuiSelserver::ccTouchEnded(Touch *pTouch, Event *pEvent)
{//convertTouchToNodeSpace
   /*Point touchLocation = convertTouchToNodeSpace(pTouch);
   if(Rect::CCRectContainsPoint(getRect(pSprite), touchLocation))
	{
	   printf("?????????!\n");
	}*/
}
/*
//????????????????????????
void KuiSelserver::editBoxEditingDidBegin(extension::EditBox *editBox)
{
	CCLOG("begin");
}

//?????????????????????????
void KuiSelserver::editBoxEditingDidEnd(extension::EditBox *editBox)
{
	CCLOG("end");
}

//???????????��?????
void KuiSelserver::editBoxTextChanged(extension::EditBox *editBox, const std::string &text)
{
	CCLOG("change");
}

//???��???????????????��?????????????????????????????
void KuiSelserver::editBoxReturn(extension::EditBox *editBox)
{
	CCLOG("return");
}*/
