//
// Created by Kull on 10/3/2024.
//

#include "MainMenu.h"
#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "gameui/KuiSelserver.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gameui/KuiAccreg.h"
#include "SelServer.h"
#include "engine/dataChecksum.h"

extern iCoreShell * g_pCoreShell;
extern KImageStore2 m_ImageStore;
KClientCallback g_ClientCallbackMenu;

using namespace ax;

bool MainMenu::init()
{
    std::string m_WritablePath = ax::FileUtils::getInstance()->getWritablePath();
    if  (GetRandomNumber(1,10)>=5)
        m_WritablePath += "music/start.mp3";
    else
        m_WritablePath += "music/start.wav";

    if  (__pSound)
        __pSound->play2d(m_WritablePath.c_str(),false);
    if (!Scene::init())
    {
        return false;
    }

    if (!nIsGaldInit)
    {
        if ((g_pCoreShell = CoreGetShell()))
        {
            g_pCoreShell->SetRepresentAreaSize(visibleSize.width, visibleSize.height);
            g_pCoreShell->SetCallDataChangedNofify(&g_ClientCallbackMenu);
        }
        nIsGaldInit = true;
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

    _groot = GRoot::create(this);
    _groot->retain();

    UIPackage::addPackage("FGUI/MainMenu");
//    UIConfig::registerFont(UIConfig::defaultFont, "fonts/UVNButLong2.TTF");
    _view = UIPackage::createObject("MainMenu", "Main")->as<GComponent>();
    _groot->addChild(_view);

    _view->getChild("b1")->addClickListener([this](EventContext*) {
        this->mianLoginCallback(this);
    });
    _view->getChild("b2")->addClickListener([this](EventContext*) {
//        Director::getInstance()->replaceScene(TransitionDemoScene::create());
    });
    _view->getChild("b3")->addClickListener([this](EventContext*) {
//        Director::getInstance()->replaceScene(VirtualListScene::create());
    });
    _view->getChild("b4")->addClickListener([this](EventContext*) {
        this->mianCancelCallback(this);
    });

    return true;
}

void MainMenu::onClose(EventContext* context)
{

}

MainMenu::MainMenu() : _groot(nullptr)
{
}

MainMenu::~MainMenu()
{
    AX_SAFE_RELEASE(_groot);
}

void MainMenu::mianCancelCallback(Object* pSender)
{
    Director::getInstance()->end();
}

void MainMenu::mianLoginCallback(Object* pSender)
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
            Director::getInstance()->replaceScene(SelServer::create());
        }
        else
        {
            char nTempChar[128];
            ZeroMemory(nTempChar,sizeof(nTempChar));
            _mver.GetString("CilentVer","mobileinfo","Vui lòng cấp nhật bản mới nhất.",nTempChar,sizeof(nTempChar));
            messageBox(UTEXT(nTempChar,1).c_str(),"Chú ý:");
            _mver.Clear();
        }
    }
    else
        messageBox("Vui lòng cấp nhật bản mới nhất.","Chú ý:");
    //Director::getInstance()->replaceScene(Klogin::scene());
}
