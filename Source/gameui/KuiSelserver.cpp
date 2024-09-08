// ***************************************************************************************
// Chỉnh sửa lại bởi duccom0123 07/09/2024 6:42:22 CH
// ***************************************************************************************
#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "KuiSelserver.h"
#include "Klogin_f.h"
#include "Klogin.h"
#include "KSelPlayer.h"
#include "engine/CXmlStream.h"
#include "gamecore/CoreShell.h"
#include "gameui/PopupLayer.h"
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
extern iCoreShell* g_pCoreShell;
extern KImageStore2 m_ImageStore;
#include <list>
#include <iostream>
#include <fstream>
USING_NS_AX;

KuiSelserver::KuiSelserver()
{
    m_sprCount        = 0;
    m_nWidth          = 0;
    m_nHeight         = 0;
    animate           = nullptr;
    nserCount         = 0;
    nCurSelIndex      = -1;
    m_bIsPlayerInGame = false;
}

KuiSelserver::~KuiSelserver()
{
    nserlist.Clear();
}

Scene* KuiSelserver::scene()
{

    Scene* scene = Scene::create();

    KuiSelserver* layer = KuiSelserver::create();

    scene->addChild(layer);

    return scene;
}

bool KuiSelserver::init()
{
    if (!Layer::init())
    {
        return false;
    }
    // duccom0123 thêm để load giao diện login từ file ini
    KIniFile config;
    if (config.Load("\\ui\\selserver\\config.ini"))
    {
        m_Status           = LL_S_WAIT_INPUT_ACCOUNT;
        char nSprName[128] = {0};
        int nFrams         = 0;
        int Left, Top, Width, Height = 0;

        Sprite* bgSprite = Sprite::create("loginbg.jpg");
        bgSprite->setPosition(ax::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        float winw = visibleSize.width;
        float winh = visibleSize.height;
        float spx  = bgSprite->getTextureRect().getMaxX();
        float spy  = bgSprite->getTextureRect().getMaxY();
        bgSprite->setScaleX(winw / spx);
        bgSprite->setScaleY(winh / spy);
        m_sprCount++;
        this->addChild(bgSprite, 0, m_sprCount);

        config.GetInteger("okbtn", "Left", 0, &Left);
        config.GetInteger("okbtn", "Top", 0, &Top);
        config.GetInteger("okbtn", "Width", 0, &Width);
        config.GetInteger("okbtn", "Height", 0, &Height);
        config.GetString("okbtn", "Image", "", nSprName, sizeof(nSprName));

        g_StrLower(nSprName);
        MenuItemImage* pCancelItem = nullptr;
        MenuItemImage* pOkItem     = nullptr;
        Texture2D* okbtn           = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
        if (okbtn)
        {
            char nSprFilePath[64]       = {0};
            unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
            sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
            Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
            sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
            Texture2D* pSelectedTexture =
                ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

            pOkItem = MenuItemImage::create();
            pOkItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
            pOkItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
            pOkItem->setDisabledImage(Sprite::createWithTexture(pSelectedTexture));
            pOkItem->setCallback(CC_CALLBACK_1(KuiSelserver::mianLoginCallback, this));
        }

        nFrams = 0;
        Left, Top, Width, Height = 0;
        ZeroMemory(nSprName, sizeof(nSprName));
        config.GetInteger("cancelbtn", "Left", 0, &Left);
        config.GetInteger("cancelbtn", "Top", 0, &Top);
        config.GetInteger("cancelbtn", "Width", 0, &Width);
        config.GetInteger("cancelbtn", "Height", 0, &Height);
        config.GetString("cancelbtn", "Image", "", nSprName, sizeof(nSprName));

        g_StrLower(nSprName);
        Texture2D* Cancel = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
        if (Cancel)
        {
            char nSprFilePath[64]       = {0};
            unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
            sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
            Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
            sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
            Texture2D* pSelectedTexture =
                ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
   
            pCancelItem = MenuItemImage::create();
            pCancelItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
            pCancelItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
            pCancelItem->setDisabledImage(Sprite::createWithTexture(pSelectedTexture));
            pCancelItem->setCallback(CC_CALLBACK_1(KuiSelserver::mianCancelCallback, this));

        }
        nFrams = 0;
        Left, Top, Width, Height = 0;
        ZeroMemory(nSprName, sizeof(nSprName));
        config.GetInteger("selserver", "Left", 0, &Left);
        config.GetInteger("selserver", "Top", 0, &Top);
        config.GetInteger("selserver", "Width", 0, &Width);
        config.GetInteger("selserver", "Height", 0, &Height);
        config.GetString("selserver", "Image", "", nSprName, sizeof(nSprName));

        g_StrLower(nSprName);

        Texture2D* selserver = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
        testSprite           = nullptr;
        if (selserver)
        {
            testSprite = Sprite::createWithTexture(selserver);

            if (testSprite)
            {
                testSprite->setPosition(ax::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
                this->addChild(testSprite, 2);

                Menu* pMenu = nullptr;
                if (okbtn && Cancel)
                {
                    Left, Top, Width, Height = 0;
                    config.GetInteger("menubtn", "Left", 0, &Left);
                    config.GetInteger("menubtn", "Top", 0, &Top);
                    config.GetInteger("menubtn", "Align", 0, &Width);

                    pMenu           = Menu::create(pOkItem, pCancelItem, nullptr);
                    Size menuSize   = testSprite->getContentSize();
                    pMenu->setContentSize(menuSize);

                    pMenu->setPosition(ax::Vec2(Left, Top));
                    testSprite->addChild(pMenu, 2);
                    pMenu->alignItemsHorizontallyWithPadding(Width);
                }
            }
        }

        if (nserlist.Load("\\Settings\\Ver_vn\\FsjxClient.ini"))
        {
            nFrams = 0;
            Left, Top, Width, Height = 0;
            ZeroMemory(nSprName, sizeof(nSprName));
            config.GetInteger("listsel", "Left", 0, &Left);
            config.GetInteger("listsel", "Top", 0, &Top);
            config.GetInteger("listsel", "Width", 0, &Width);
            config.GetInteger("listsel", "Height", 0, &Height);
            config.GetString("listsel", "Image", "", nSprName, sizeof(nSprName));

            nserlist.GetInteger("List", "RegionCount", 0, &nserCount);
            Menu* serlistMenu = Menu::create(nullptr);
            serlistMenu->setAnchorPoint(ax::Vec2(0, 0));
            serlistMenu->setPosition(ax::Vec2(Left, Top));
            testSprite->addChild(serlistMenu, 4);
            int offSetX = 0, offSetY = 0, nfontSize = 12;

            for (int i = 0; i < nserCount; ++i)
            {
                Sprite* serBgSor = Sprite::create(nSprName);

                char strSerName[128], cckey[32];
                sprintf(cckey, "Region_%d", i);
                nserlist.GetString(cckey, "0_Title", "", strSerName, sizeof(strSerName));

                auto mItemServer =
                    MenuItemSprite::create(serBgSor, serBgSor, CC_CALLBACK_1(KuiSelserver::selServerCallBack, this));

                mItemServer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

                auto serLabel = Label::createWithTTF(UTEXT(strSerName, 1).c_str(), UI_GAME_FONT_VN, nfontSize);
                serLabel->setColor(Color3B(0, 255, 0));
                serLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                serLabel->setPosition(mItemServer->getContentSize() / 2);

                mItemServer->setPosition(ax::Vec2(offSetX, offSetY));
                mItemServer->setTag(i + 1);
                mItemServer->addChild(serLabel);

                int nIsOpen = 0;
                nserlist.GetInteger(cckey, "0_IsOpen", 0, &nIsOpen);
                if (nIsOpen == 0)
                    mItemServer->setEnabled(false);
                serlistMenu->addChild(mItemServer, i + 1);
                offSetY -= 30;
            }
        }
        return true;
    }

    return false;
}

void KuiSelserver::selServerCallBack(Ref* menuItem)
{
    if (menuItem)
    {
        MenuItemLabel* nCerBox = (MenuItemLabel*)menuItem;
        KIniFile nClentMak;
        if (!nClentMak.Load(CLIENT_VER_FILE))
        {
#ifdef WIN32
            messageBox("?��????", "???:");
#else
            messageBox(UTEXT("?��????", 1).c_str(), UTEXT("???:", 1).c_str());
#endif
            return;
        }

        if (nAcclient)
        {
            KIniFile dwmask;
            std::string fielDir = "";
#ifdef WIN32

            if (_clientlanguage != 1)
                fielDir = std::string("paklist_vn.ini");
            else
                fielDir = std::string("paklist.ini");
#else

            if (_clientlanguage != 1)
                fielDir = std::string("data\\paklist_vn.ini");
            else
                fielDir = std::string("data\\paklist.ini");
#endif
            char nTempstr[128];
            sprintf(nTempstr, "%s", fielDir.c_str());

            if (!_clientNoNetWork && !dwmask.Load(nTempstr))
            {
#ifdef WIN32
                messageBox("????????????...", "????");
#else
                messageBox(UTEXT("????????????...", 1).c_str(), UTEXT("????", 1).c_str());
#endif
                return;
            }
            dwmask.Clear();

            char downUrlmask[64];
            ZeroMemory(downUrlmask, sizeof(downUrlmask));

            char cckey[32];
            sprintf(cckey, "Region_%d", nCerBox->getTag() - 1);
            nserlist.GetString(cckey, "0_Address", "", downUrlmask, sizeof(downUrlmask));

            if (nAcclient->FsGameServerConnectTo(downUrlmask, 56722))
            {

                m_ClientChoices.nServerRegionIndex = nCerBox->getTag();
                nAcclient->Startup();
                m_bIsClientConnecting = true;
            }
            else
            {
                nAcclient->Cleanup();
                m_bIsClientConnecting = false;
#ifdef WIN32

                messageBox(downUrlmask, "Connect IP:");
#else

                messageBox(downUrlmask, "Connect IP:");
#endif
                return;
            }
        }
        ax::Director::getInstance()->replaceScene(Klogin::scene());
    }
}

void KuiSelserver::mianCancelCallback(Ref* pSender)
{
    m_ClientChoices.nServerRegionIndex = -1;
    if (nAcclient)
    {
        m_bIsClientConnecting = false;
        m_bIsPlayerInGame     = false;
        nAcclient->Cleanup();
    }
    ax::Director::getInstance()->replaceScene(Klogin_f::scene());
}

void KuiSelserver::mianLoginCallback(Ref* pSender) {}

void KuiSelserver::update(float delta) {}

bool KuiSelserver::isFileExist(const char* pFileName)
{
    if (!pFileName)
        return false;

    std::string filePath = ax::FileUtils::getInstance()->getWritablePath();

    filePath += pFileName;

    FILE* fp = fopen(filePath.c_str(), "r");
    if (fp)
    {
        fclose(fp);
        return true;
    }
    return false;
}

void KuiSelserver::copyData(const char* pFileName)
{
    std::string strPath = ax::FileUtils::getInstance()->fullPathForFilename(pFileName);
    unsigned int len    = 0;
    unsigned char* data = nullptr;

    std::ifstream file(strPath, std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        std::streampos size = file.tellg();
        data                = new unsigned char[size];
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(data), size);
        file.close();
    }
    len = ax::FileUtils::getInstance()->getFileSize(strPath);

    std::string destPath = ax::FileUtils::getInstance()->getWritablePath();
    destPath += pFileName;
    FILE* fp = fopen(destPath.c_str(), "w+");
    if (fp)
    {
        fwrite(data, sizeof(unsigned char), len, fp);
        fclose(fp);
        fp = nullptr;
    }

    if (data)
    {
        delete[] data;
        data = nullptr;
    }
}

void KuiSelserver::finish() {}

bool KuiSelserver::ccTouchBegan(Touch* pTouch, Event* pEvent)
{
    return true;
}
void KuiSelserver::ccTouchMoved(Touch* pTouch, Event* pEvent) {}

void KuiSelserver::ccTouchCancelled(Touch* pTouch, Event* pEvent) {}
void KuiSelserver::ccTouchesBegan(const std::vector<ax::Touch*>& pTouches, Event* pEvent) {}
void KuiSelserver::ccTouchesMoved(const std::vector<ax::Touch*>& pTouches, Event* pEvent) {}

void KuiSelserver::ccTouchesEnded(const std::vector<ax::Touch*>& pTouches, Event* pEvent)
{
    auto it      = pTouches.begin();
    Touch* touch = (Touch*)(*it);

    Point location = touch->getLocation();
}

void KuiSelserver::ccTouchesCancelled(const std::vector<ax::Touch*>& pTouches, Event* pEvent) {}

void KuiSelserver::didAccelerate(Acceleration* pAccelerationValue) {}
void KuiSelserver::ccTouchEnded(Touch* pTouch, Event* pEvent)
{
    /*Point touchLocation = convertTouchToNodeSpace(pTouch);
    if(Rect::CCRectContainsPoint(getRect(pSprite), touchLocation))
         {
            printf("?????????!\n");
         }*/
}
/*
void KuiSelserver::editBoxEditingDidBegin(extension::EditBox *editBox)
{
        CCLOG("begin");
}

void KuiSelserver::editBoxEditingDidEnd(extension::EditBox *editBox)
{
        CCLOG("end");
}

void KuiSelserver::editBoxTextChanged(extension::EditBox *editBox, const std::string &text)
{
        CCLOG("change");
}

void KuiSelserver::editBoxReturn(extension::EditBox *editBox)
{
        CCLOG("return");
}*/
