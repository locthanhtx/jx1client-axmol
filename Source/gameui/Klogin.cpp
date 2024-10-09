// ***************************************************************************************
// Chỉnh sửa lại bởi duccom0123 07/09/2024 6:42:22 CH
// ***************************************************************************************
#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "gameui/Uiglobaldata.h"
#include "Klogin.h"
#include "Klogin_f.h"
#include "KSelPlayer.h"
#include "engine/CXmlStream.h"
#include "engine/KIniFile.h"
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
#include "gamecore/KSubWorldSet.h"
#include "gamescene/KUpdateLayer.h"
#include <iostream>
#include <fstream>
#ifndef WIN32
#    include <ctype.h>
#endif
extern iCoreShell* g_pCoreShell;
extern KImageStore2 m_ImageStore;
#include <list>
USING_NS_AX;

void CcharToDouble(const char* ch, double& res)
{
    sscanf(ch, "%lf", &res);
}

Scene* Klogin::scene()
{

    Scene* scene = Scene::create();

    Klogin* layer = Klogin::create();

    scene->addChild(layer);

    return scene;
}

bool Klogin::init()
{
    if (!Layer::init())
    {
        return false;
    }

    m_sprCount = 0;
    nTimeCount = 0;
    m_nWidth   = 0;
    m_nHeight  = 0;

    animate                         = NULL;
    m_bIsPlayerInGame               = false;
    auto touchListener              = ax::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan     = CC_CALLBACK_2(Klogin::ccTouchBegan, this);
    touchListener->onTouchMoved     = CC_CALLBACK_2(Klogin::ccTouchMoved, this);
    touchListener->onTouchEnded     = CC_CALLBACK_2(Klogin::ccTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(Klogin::ccTouchCancelled, this);

    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    // duccom0123 thêm để load giao diện login từ file ini
    KIniFile config;
    if (config.Load("\\ui\\loginui\\config.ini"))
    {
        m_Status           = LL_S_WAIT_INPUT_ACCOUNT;
        char nSprName[128] = {0};
        int nFrams         = 0;
        int Left, Top, Width, Height = 0;

        config.GetInteger("loginbtn", "Left", 0, &Left);
        config.GetInteger("loginbtn", "Top", 0, &Top);
        config.GetInteger("loginbtn", "Width", 0, &Width);
        config.GetInteger("loginbtn", "Height", 0, &Height);
        config.GetString("loginbtn", "Image", "", nSprName, sizeof(nSprName));

        g_StrLower(nSprName);
        MenuItemImage* pCancelItem = NULL;
        MenuItemImage* pLoginItem  = NULL;
        Texture2D* Login           = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
        if (Login)
        {
            char nSprFilePath[64]       = {0};
            unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
            sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
            Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
            sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
            Texture2D* pSelectedTexture =
                ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
            sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 2);
            Texture2D* pDisabledTexture =
                ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

            pLoginItem = MenuItemImage::create();
            if (pNormalTexture)
                pLoginItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
            if (pSelectedTexture)
                pLoginItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
            if (pDisabledTexture)
                pLoginItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
            pLoginItem->setCallback(CC_CALLBACK_1(Klogin::mianLoginCallback, this));

            pLoginItem->setPosition(ax::Vec2(origin.x + visibleSize.width - Left, origin.y + Top));
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
            sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 2);
            Texture2D* pDisabledTexture =
                ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

            pCancelItem = MenuItemImage::create();
            if (pNormalTexture)
                pCancelItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
            if (pSelectedTexture)
                pCancelItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
            if (pDisabledTexture)
                pCancelItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
            pCancelItem->setCallback(CC_CALLBACK_1(Klogin::mianCancelCallback, this));

            pCancelItem->setPosition(ax::Vec2(origin.x + visibleSize.width - Left, origin.y + Top));
        }

        const char* strTiele = CXmlStream::GetStringByKeyFromFile("string.xml", "title");

        int nCount = 0;

        pMainLabel = Label::createWithTTF(strTiele, UI_GAME_FONT_DEFAULT, 20);
        pMainLabel->setString(UTEXT("", 1).c_str());

        pMainLabel->setPosition(ax::Vec2(origin.x + visibleSize.width / 2,
                                         origin.y + visibleSize.height - pMainLabel->getContentSize().height - 10));

        this->addChild(pMainLabel, 1);

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

        // hiệu ứng lá bay
        sprintf(nSprName, "\\spr\\Ui3\\loginui\\uidonghua\\yezi.spr");
        g_StrLower(nSprName);
        Texture2D* leafeffect = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
        if (leafeffect)
        {
            Sprite* ySprite = Sprite::createWithTexture(leafeffect);
            ySprite->setPosition(ax::Vec2(visibleSize.width / 2 + origin.x - 100, visibleSize.height / 2 + origin.y));

            m_sprCount++;
            this->addChild(ySprite, 2, m_sprCount);
            char nySprFilePath[64]      = {0};
            Animation* animation        = Animation::create();
            unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
            for (int i = 0; i < nFrams; i++)
            {
                sprintf(nySprFilePath, "%u-%d", nFielpahtdwid, i);

                auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                if (texture)
                {
                    Rect rect = Rect::ZERO;
                    rect.size = texture->getContentSize();
                    animation->addSpriteFrameWithTexture(texture, rect);
                }
            }

            animation->setDelayPerUnit(0.10f);

            animation->setLoops(-1);

            animation->setRestoreOriginalFrame(TRUE);

            animate = Animate::create(animation);

            if (ySprite)
            {
                auto callFunc = ax::CallFunc::create([this]() { this->finish(); });

                ySprite->runAction(Sequence::create(animate, callFunc, NULL));
            }
        }

        // khung đăng nhập
        nFrams = 0;
        Left, Top, Width, Height = 0;
        ZeroMemory(nSprName, sizeof(nSprName));

        config.GetInteger("loginframe", "Left", 0, &Left);
        config.GetInteger("loginframe", "Top", 0, &Top);
        config.GetInteger("loginframe", "Width", 0, &Width);
        config.GetInteger("loginframe", "Height", 0, &Height);
        config.GetString("loginframe", "Image", "", nSprName, sizeof(nSprName));
        g_StrLower(nSprName);
        Texture2D* loginframe = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
        testSprite            = NULL;
        if (loginframe)
        {
            testSprite = Sprite::createWithTexture(loginframe);
            if (testSprite)
            {
                char nTempStr[64];
                ZeroMemory(nTempStr, sizeof(nTempStr));
                KIniFile accinfo;
                accinfo.Load("accinfo.ini", true);

                testSprite->setPosition(ax::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
                m_sprCount++;
                this->addChild(testSprite, 2, m_sprCount);

                Left, Top, Width, Height = 0;

                config.GetInteger("acctxtbox", "Left", 0, &Left);
                config.GetInteger("acctxtbox", "Top", 0, &Top);
                config.GetInteger("acctxtbox", "Width", 0, &Width);
                config.GetInteger("acctxtbox", "Height", 0, &Height);

                ax::ui::Scale9Sprite* pAccBg = ax::ui::Scale9Sprite::create();
                EditBox* pAccEditBox         = EditBox::create(Size(Width, Height), pAccBg);
                const ax::Color3B nfcolor    = {255, 0, 0};
                if (pAccEditBox)
                {
                    pAccEditBox->setFontColor(nfcolor);
                    pAccEditBox->setFontSize(24);
                    pAccEditBox->setMaxLength(8);

                    pAccEditBox->setPositionX(Left);
                    pAccEditBox->setPositionY(Top);

                    accinfo.GetString("info", "acc", "", nTempStr, sizeof(nTempStr));
                    pAccEditBox->setText("kull3");
                    this->addChild(pAccEditBox, 4, 1000);
                }

                Left, Top, Width, Height = 0;

                config.GetInteger("passtxtbox", "Left", 0, &Left);
                config.GetInteger("passtxtbox", "Top", 0, &Top);
                config.GetInteger("passtxtbox", "Width", 0, &Width);
                config.GetInteger("passtxtbox", "Height", 0, &Height);

                ax::ui::Scale9Sprite* pPassBg = ax::ui::Scale9Sprite::create();
                EditBox* pPassEditBox         = EditBox::create(Size(Width, Height), pPassBg);
                if (pPassEditBox)
                {
                    pPassEditBox->setFontColor(nfcolor);
                    pPassEditBox->setFontSize(24);
                    pPassEditBox->setInputFlag(EditBox::InputFlag::PASSWORD);
                    pPassEditBox->setMaxLength(16);

                    pPassEditBox->setPositionX(Left);
                    pPassEditBox->setPositionY(Top);

                    accinfo.GetString("info", "pas", "", nTempStr, sizeof(nTempStr));
                    pPassEditBox->setText("1");
                    this->addChild(pPassEditBox, 4, 999);
                }
                accinfo.Clear();

                Menu* pMenu = NULL;
                if (Login && Cancel)
                {
                    pLoginItem->setPositionX(testSprite->getPositionX() - 75);
                    pLoginItem->setPositionY(testSprite->getPositionY() - 80);
                    pCancelItem->setPositionX(testSprite->getPositionX() + 75);
                    pCancelItem->setPositionY(testSprite->getPositionY() - 80);

                    pMenu = Menu::create(pLoginItem, pCancelItem, NULL);
                    pMenu->setPosition(ax::Vec2::ZERO);
                    m_sprCount++;
                    this->addChild(pMenu, 2, m_sprCount);
                }
            }
        }
    }

    nTestCount = 0;

    this->scheduleUpdate();

    return true;
}

void Klogin::mianCancelCallback(Ref* pSender)
{
    if (nAcclient)
    {
        m_bIsClientConnecting = false;
        m_bIsPlayerInGame     = false;
        nAcclient->Cleanup();
    }
    ax::Director::getInstance()->replaceScene(Klogin_f::scene());
}

void Klogin::mianLoginCallback(Ref* pSender)
{
    EditBox* AccNode = (EditBox*)this->getChildByTag(1000);
    EditBox* PasNode = (EditBox*)this->getChildByTag(999);
    std::string strAcc;
    std::string strPas;

    if (AccNode && PasNode)
    {
        strAcc = AccNode->getText();
        strPas = PasNode->getText();

        if (strAcc.length() > 16 || strAcc.length() <= 0 || strPas.length() > 16 || strPas.length() <= 0)
        {
#ifdef WIN32
            messageBox("Sai t��i kho?n ho?c m?t kh?u", "Ch�� y");
#else
            messageBox(G2U("Sai t��i kho?n ho?c m?t kh?u��").c_str(), G2U("Ch�� y").c_str());
#endif
            return;
        }

        std::string strcheck = "check";
        strcheck += strAcc;
        strcheck += strPas;
        int count = 0, spaceNum = 0, chineseNum = 0, other = 0, letter = 0, xiaoxie = 0;
        for (int i = 0; i < strcheck.length(); i++)
        {
            if (isdigit(strcheck[i]))
                count++;
            else if (isspace(strcheck[i]))
                ++spaceNum;
            else if (isalpha(strcheck[i]) == 1)
            {
                ++letter;
            }
            else if (isalpha(strcheck[i]) == 2)
            {
                ++xiaoxie;
            }
            else if (strcheck[i] & 0x80)
                ++chineseNum;
            else
            {
                ++other;
            }
        }
        if (_clientlanguage == 1)
        {
            if (spaceNum > 0 || chineseNum > 0 || other > 0 || letter > 0)
            {
#ifdef WIN32
                messageBox("���зǷ��ַ�,�����ʺ�,����\n���ܺ��пո�,��д��ĸ���������!", "����");
#else
                messageBox(G2U("���зǷ��ַ�,�����ʺ�,����\n���ܺ��пո�,��д��ĸ���������!").c_str(), G2U("����").c_str());
#endif
                return;
            }
        }
        else
        {
            if (spaceNum > 0 || chineseNum > 0 || other > 0)
            {
#ifdef WIN32
                messageBox("���зǷ��ַ�,�����ʺ�,����\n���ܺ��пո�,��д��ĸ���������!", "����");
#else
                messageBox(
                    "Invalid character,please check your name!\n Cannot contain special symbols such as chinese, "
                    "spaces, etc.!",
                    "Warning");
#endif
                return;
            }
        }
    }
    else
    {
#ifdef WIN32
        messageBox("�������󣮣�", "����");
#else
        messageBox(G2U("�������󣮣�").c_str(), G2U("����").c_str());
#endif
        return;
    }
    KIniFile accinfo;
    if (accinfo.Load("accinfo.ini", true))
    {
        char nTempStr[64];
        sprintf(nTempStr, "%s", strAcc.c_str());
        accinfo.WriteString("info", "acc", nTempStr);

        sprintf(nTempStr, "%s", strPas.c_str());
        accinfo.WriteString("info", "pas", nTempStr);
        accinfo.Save("accinfo.ini");
        accinfo.Clear();
    }

    if (m_bIsClientConnecting && nAcclient)
    {
        if (m_ClientChoices.nServerRegionIndex <= 0)
        {
#ifdef WIN32
            messageBox("��ѡ���½��������", "����");
#else
            messageBox(G2U("��ѡ���½��������").c_str(), G2U("����").c_str());
#endif
            return;
        }

        m_nNumRole = 0;
        memset(&m_RoleList, 0, sizeof(m_RoleList));
        ClearAccountPassword(true, true);

        BYTE Buff[sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE];
        RandMemSet(sizeof(Buff), (BYTE*)Buff);
        char pszAccount[32] = {0};
        sprintf(pszAccount, "%s", strAcc.c_str());

        (*(PROTOCOL_MSG_TYPE*)Buff) = c2s_login_fhgyrinhkmvnsheypo;
        KLoginAccountInfo* pInfo    = (KLoginAccountInfo*)&Buff[PROTOCOL_MSG_SIZE];

        pInfo->Size  = sizeof(KLoginAccountInfo);
        pInfo->Param = LOGIN_A_LOGIN | LOGIN_R_REQUEST;
        strncpy(pInfo->Account, pszAccount, sizeof(pInfo->Account));

        pInfo->Account[sizeof(pInfo->Account) - 1] = '\0';

        KSG_StringToMD5String(pInfo->Password.szPassword, strPas.c_str());

        pInfo->nSelSererIdx    = m_ClientChoices.nServerRegionIndex;
        pInfo->ProtocolVersion = KPROTOCOL_VERSION;
        m_Status               = LL_S_ACCOUNT_CONFIRMING;
        SetAccountPassword(pszAccount, &pInfo->Password);
        if (!nAcclient->SendMsg(Buff, sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE))

        {
            if (nAcclient)
            {
                nAcclient->Cleanup();
                m_bIsClientConnecting = false;
            }
            ax::Director::getInstance()->replaceScene(Klogin_f::scene());
        }
    }
}
void Klogin::mianSkillCallback(Ref* pSender)
{
    /*#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
            messageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    #else
            ax::Director::getInstance()->end();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
    #endif
    #endif*/
}

void Klogin::update(float delta)
{
    if (m_Status == LL_S_ROLE_LIST_READY)
    {
        m_Status = LL_S_IDLE;
        ax::Director::getInstance()->replaceScene(KSelPlayer::create());
        return;
    }
    unsigned int nSize;
    const char* pBuffer = NULL;
    if (m_bIsClientConnecting)
    {
        while (true)
        {
            if (!nAcclient)
                break;

            pBuffer = (const char*)nAcclient->GetPackFromServer(nSize);

            if (!(pBuffer && nSize))
                break;

            PROTOCOL_MSG_TYPE* pMsg = (PROTOCOL_MSG_TYPE*)pBuffer;
            PROTOCOL_MSG_TYPE Msg   = *pMsg;

            if (Msg == s2c_login || Msg == s2c_roleserver_getrolelist_result ||
                Msg == s2c_notifypl_ayerlogin_hfkqotivbhd || Msg == s2c_rolenewdelresponse)
                AcceptNetMsg(pMsg);
        }
    }
    int newX = 0;
    int newY = 0;

    /*char nPiontInfo[64]={0};
    sprintf(nPiontInfo,"%f",n);
    pMainLabel->setString(nPiontInfo);
*/
    switch (newY)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }
}

/*nTestCount ++;
char nPiontInfo[64]={0};
sprintf(nPiontInfo,"%d",nTestCount);
pMainLabel->setString(nPiontInfo);*/

/*ccPointSize(1);
ccDrawColor4B(255, 25, 200, 250);
ccDrawSolidRect(ax::Vec2(origin.x, origin.y), ax::Vec2(origin.x+mysize.width, origin.y+mysize.height), ccc4f(0, 0, 0,
0.5f));
*/
/*for (int i=0;i<=mysize.width;i++)
        for(int j=0;j<=mysize.height;j++)
        {
                Point nPos;
                nPos.x = i;
                nPos.y = j;
    ccDrawPoint(nPos);
        }*/
/*

        ccPointSize(30);
        ccDrawColor4B(255, 0, 0, 100);
        Point Pointarray[] = {ax::Vec2(200, 150), ax::Vec2(200, 200), ax::Vec2(280, 150), ax::Vec2(280, 200)};
        ccDrawPoints(Pointarray, 4);



        glLineWidth(3);
        ccDrawColor4B(255, 255, 255, 130);
        ccDrawLine(ax::Vec2(10, 300), ax::Vec2(200, 300) );



        glLineWidth(3);
        ccDrawColor4B(255, 255, 100, 190);
        ccDrawCircle(ax::Vec2(50, 250), 40, 3.14/2, 360, true, 1, 0.5);
        */

/*	glLineWidth(5);
        ccDrawColor4B(24, 25, 200, 140);


        ccDrawSolidRect(ax::Vec2(origin.x, origin.y), ax::Vec2(origin.x+mysize.width, origin.y+mysize.height), ccc4f(0,
   0, 0, 0.5f));
*/
/*


        glLineWidth(10);
        ccDrawColor4B(240, 225, 100, 130);
        Point Polyarray[] = {ax::Vec2(20, 20), ax::Vec2(50, 0), ax::Vec2(250, 100), ax::Vec2(300, 100), ax::Vec2(250,
   50)}; ccDrawPoly(Polyarray, 5, 1);


        Point Polyarray2[] = {ax::Vec2(250,30), ax::Vec2(280,300), ax::Vec2(450,0), ax::Vec2(410,130),
   ax::Vec2(420,50)}; ccDrawSolidPoly(Polyarray2, 5, ccc4f(142, 245, 70, 0.3f));




        glLineWidth(5);
        ccDrawColor4B(100, 100, 100, 255);
        ccDrawQuadBezier(ax::Vec2(0,320), ax::Vec2(160,100), ax::Vec2(480,320), 100);


        glLineWidth(5);
        ccDrawColor4B(200, 200, 200, 255);
        ccDrawCubicBezier(ax::Vec2(0,0), ax::Vec2(160,300), ax::Vec2(320,20), ax::Vec2(480,320), 100);*/
bool Klogin::isFileExist(const char* pFileName)
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

void Klogin::copyData(const char* pFileName)
{
    std::string strPath = ax::FileUtils::getInstance()->fullPathForFilename(pFileName);
    unsigned int len    = 0;
    unsigned char* data = NULL;

    std::ifstream file(strPath, std::ios::binary | std::ios::ate);
    if (file.is_open())
    {

        std::streampos size = file.tellg();
        data                = new unsigned char[size];
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(data), size);
        file.close();
    }
    if (data)
    {
        len                  = ax::FileUtils::getInstance()->getFileSize(strPath);
        std::string destPath = ax::FileUtils::getInstance()->getWritablePath();
        destPath += pFileName;
        FILE* fp = fopen(destPath.c_str(), "w+");

        if (fp)
        {
            fwrite(data, sizeof(unsigned char), len, fp);
            fclose(fp);
        }

        delete[] data;
        data = NULL;
    }
}

void Klogin::finish()
{
    if (!nisgetinfo)
        mExit(NULL);
}
/*
void Klogin::onEnter()
{

}

void Klogin::onExit()
{

}
void Klogin::onEnterTransitionDidFinish()
{

}*/

bool Klogin::ccTouchBegan(Touch* pTouch, Event* pEvent)
{
    return true;
}
void Klogin::ccTouchMoved(Touch* pTouch, Event* pEvent) {}

void Klogin::ccTouchCancelled(Touch* pTouch, Event* pEvent) {}
void Klogin::ccTouchesBegan(const std::vector<ax::Touch*>& pTouches, Event* pEvent) {}
void Klogin::ccTouchesMoved(const std::vector<ax::Touch*>& pTouches, Event* pEvent) {}

void Klogin::ccTouchesEnded(const std::vector<ax::Touch*>& pTouches, Event* pEvent)
{
    auto it      = pTouches.begin();
    Touch* touch = (Touch*)(*it);

    Point location = touch->getLocation();

    /*public void GetMpsByLocalPosition(Vector3 v3LocalPosition, out int nMpsX, out int nMpsY)
    {
            nMpsX = ((int) (location.x * 100f)) + ((this.m_nRegionBeginX * this.m_nRegionWidth) * this.m_nCellWidth);
            nMpsY = ((this.m_nRegionBeginY * this.m_nRegionHeight) * this.m_nCellHeight) - ((int) (location.y * 200f));
    }*/

    /*Node* s = getChildByTag(1);           //ͨ����ǺŻ�ȡ����Ľڵ�

    s->runAction(CCMoveTo::create(10, ax::Vec2(location.x, location.y)));

    float o = location.x - s->getPosition().x;
    float a = location.y - s->getPosition().y;

    float at = (float) CC_RADIANS_TO_DEGREES( atanf( o/a) );

    if( a < 0 )
    {
            if(  o < 0 )
                    at = 180 + fabs(at);
            else
                    at = 180 - fabs(at);
    }

    s->runAction( CCRotateTo::create(10, at) );
    */
}
void Klogin::ccTouchesCancelled(const std::vector<ax::Touch*>& pTouches, Event* pEvent) {}
void Klogin::didAccelerate(Acceleration* pAccelerationValue) {}
void Klogin::ccTouchEnded(Touch* pTouch, Event* pEvent)
{
    /*Point touchLocation = convertTouchToNodeSpace(pTouch);
    if(Rect::CCRectContainsPoint(getRect(pSprite), touchLocation))
         {
            printf("�ұ�������!\n");
         }*/
}

void Klogin::AcceptNetMsg(void* pMsgData)
{
    if (pMsgData == NULL)
        return;

    switch (m_Status)
    {
    case LL_S_ACCOUNT_CONFIRMING:
        ProcessAccountLoginResponse((KLoginStructHead*)(((char*)pMsgData) + PROTOCOL_MSG_SIZE));
        break;
    case LL_S_WAIT_ROLE_LIST:
        ProcessRoleListResponse((TProcessData*)pMsgData);
        break;
    case LL_S_CREATING_ROLE:

        break;
    case LL_S_DELETING_ROLE:

        break;
    case LL_S_WAIT_TO_DEL:

        break;
    case LL_S_WAIT_TO_LOGIN_GAMESERVER:

        break;
    }
}

void Klogin::SetAccountPassword(const char* pszAccount, const KSG_PASSWORD* pcPassword)
{
    int i;
    if (pszAccount)
    {
        strncpy(m_ClientChoices.Account, pszAccount, sizeof(m_ClientChoices.Account));
        for (i = 0; i < 32; ++i)
            m_ClientChoices.Account[i] = ~m_ClientChoices.Account[i];
    }

    if (pcPassword)
    {
        m_ClientChoices.Password = *pcPassword;
        for (i = 0; i < KSG_PASSWORD_MAX_SIZE; ++i)
            m_ClientChoices.Password.szPassword[i] = ~m_ClientChoices.Password.szPassword[i];
    }
}

void Klogin::GetAccountPassword(char* pszAccount, KSG_PASSWORD* pPassword)
{
    int i;
    if (pszAccount)
    {
        memcpy(pszAccount, m_ClientChoices.Account, sizeof(m_ClientChoices.Account));
        for (i = 0; i < 32; ++i)
            pszAccount[i] = ~pszAccount[i];
    }
    if (pPassword)
    {
        *pPassword = m_ClientChoices.Password;
        for (i = 0; i < KSG_PASSWORD_MAX_SIZE; ++i)
            pPassword->szPassword[i] = ~pPassword->szPassword[i];
    }
}

void Klogin::ClearAccountPassword(bool bAccount, bool bPassword)
{
    if (bAccount)
        memset(m_ClientChoices.Account, 0xff, sizeof(m_ClientChoices.Account));
    if (bPassword)
        memset(&m_ClientChoices.Password, 0xff, sizeof(m_ClientChoices.Password));
}

void Klogin::ProcessAccountLoginResponse(KLoginStructHead* pResponse)
{

    if (((pResponse->Param & LOGIN_ACTION_FILTER) == LOGIN_A_LOGIN) && pResponse->Size >= sizeof(KLoginAccountInfo))
    {
        KLoginAccountInfo* pInfo = (KLoginAccountInfo*)pResponse;
        char szAccount[32]       = {0};
        KSG_PASSWORD Password;
        GetAccountPassword(szAccount, &Password);
        if (strcmp(pInfo->Account, szAccount) == 0 && strcmp(pInfo->Password.szPassword, Password.szPassword) == 0)
        {
            KIniFile nBVer;
            double mBver, mBishopver;
            if (!nBVer.Load("\\Settings\\Ver\\Xfsverjx.ini"))
            {
                isthisVer = false;
                m_Result  = LL_R_INVALID_PROTOCOLVERSION;

                nBVer.Clear();
#ifdef WIN32
                messageBox("�汾����", "��ʾ:");
#else
                messageBox(UTEXT("�汾����", 1).c_str(), UTEXT("��ʾ:", 1).c_str());
#endif
                return;
            }
            nBVer.GetDouble("CilentVer", "ver", 0.1, &mBver);
            CcharToDouble(pInfo->nBishopver, mBishopver);

            if (pInfo->nSelSererIdx <= 0)
            {
                if (mBishopver != mBver)
                    isthisVer = false;
                else
                    isthisVer = true;
            }
            else
            {
                if (m_ClientChoices.nServerRegionIndex == pInfo->nSelSererIdx)
                {
                    if (mBishopver != mBver)
                        isthisVer = false;
                    else
                        isthisVer = true;
                }
                else
                    isthisVer = true;
            }

            if (pInfo->nForbitopennum)
            {
            }
            nBVer.Clear();

            if (pInfo->ProtocolVersion != KPROTOCOL_VERSION)
            {
#ifdef WIN32
                messageBox("�汾������!", "��ʾ:");
#else
                messageBox(UTEXT("�汾������!", 1).c_str(), UTEXT("��ʾ:", 1).c_str());
#endif
                m_Result = LL_R_INVALID_PROTOCOLVERSION;
                return;
            }

            int nResult = ((pResponse->Param) & ~LOGIN_ACTION_FILTER);
            if (nResult == LOGIN_R_SUCCESS)
            {

                m_Status = LL_S_WAIT_ROLE_LIST;
            }
            else
            {
                LOGIN_LOGIC_RESULT_INFO eResult = LL_R_NOTHING;
                switch (nResult)
                {
                case LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR:

                    m_Status = LL_S_WAIT_INPUT_ACCOUNT;
#ifdef WIN32
                    messageBox("�������", "��ʾ:");
#else
                    messageBox(UTEXT("�������", 1).c_str(), UTEXT("��ʾ:", 1).c_str());
#endif
                    break;
                case LOGIN_R_ACCOUNT_EXIST:

#ifdef WIN32
                    messageBox("�ʺ�����ʹ��", "��ʾ:");
#else
                    messageBox(UTEXT("�ʺ�����ʹ��", 1).c_str(), UTEXT("��ʾ:", 1).c_str());
#endif
                    m_Status = LL_S_WAIT_INPUT_ACCOUNT;
                    break;
                case LOGIN_R_FREEZE:

#ifdef WIN32
                    messageBox("�ʺű�����", "��ʾ:");
#else
                    messageBox(UTEXT("�ʺű�����", 1).c_str(), UTEXT("��ʾ:", 1).c_str());
#endif
                    m_Status = LL_S_WAIT_INPUT_ACCOUNT;
                    break;
                case LOGIN_R_INVALID_PROTOCOLVERSION:

#ifdef WIN32
                    messageBox("�汾������", "��ʾ:");
#else
                    messageBox(UTEXT("�汾������", 1).c_str(), UTEXT("��ʾ:", 1).c_str());
#endif
                    break;
                case LOGIN_R_FAILED:

#ifdef WIN32
                    messageBox("����ʧ��", "��ʾ:");
#else
                    messageBox(UTEXT("����ʧ��", 1).c_str(), UTEXT("��ʾ:", 1).c_str());
#endif
                    break;
                case LOGIN_R_TIMEOUT:

#ifdef WIN32
                    messageBox("�ʺ��Ѿ�����", "��ʾ:");
#else
                    messageBox(UTEXT("�ʺ��Ѿ�����", 1).c_str(), UTEXT("��ʾ:", 1).c_str());
#endif
                    m_Status = LL_S_WAIT_INPUT_ACCOUNT;
                    break;
                default:
#ifdef WIN32
                    messageBox("����ʧ��", "��ʾ:");
#else
                    messageBox(UTEXT("����ʧ��", 1).c_str(), UTEXT("��ʾ:", 1).c_str());
#endif
                    break;
                }

                if (m_Status != LL_S_WAIT_INPUT_ACCOUNT)
                {
                }
                m_Result = eResult;
            }
        }
        memset(szAccount, 0, sizeof(szAccount));
        memset(&Password, 0, sizeof(Password));
    }
}

void Klogin::ProcessRoleListResponse(TProcessData* pResponse)
{

    if (pResponse->nProtoId == s2c_roleserver_getrolelist_result)
    {
        m_nNumRole = pResponse->pDataBuffer[0];
        if (m_nNumRole > MAX_PLAYER_PER_ACCOUNT)
            m_nNumRole = MAX_PLAYER_PER_ACCOUNT;
        else if (m_nNumRole < 0)
            m_nNumRole = 0;

        RoleBaseInfo* pList = (RoleBaseInfo*)&pResponse->pDataBuffer[1];
        for (int i = 0; i < m_nNumRole; ++i)
        {
            if (pList->szName[0])
            {
                strcpy(m_RoleList[i].Name, pList->szName);
                m_RoleList[i].Attribute = pList->Series;
                m_RoleList[i].Gender    = pList->Sex;
                m_RoleList[i].nLevel    = pList->Level;
                pList++;
            }
            else
            {
                m_nNumRole = i;
                break;
            }
        }

        m_Status = LL_S_ROLE_LIST_READY;

        /*if (m_bInAutoProgress)
        {
                int nAdviceChoice;
                GetRoleCount(nAdviceChoice);
                SelectRole(nAdviceChoice);
        }*/
    }
}

void Klogin::mExit(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    messageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    ax::Director::getInstance()->end();
#    if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#    endif
#endif
}
/*
void Klogin::editBoxEditingDidBegin(extension::EditBox *editBox)
{
        CCLOG("begin");
}

void Klogin::editBoxEditingDidEnd(extension::EditBox *editBox)
{
        CCLOG("end");
}

void Klogin::editBoxTextChanged(extension::EditBox *editBox, const std::string &text)
{
        CCLOG("change");
}

void Klogin::editBoxReturn(extension::EditBox *editBox)
{
        CCLOG("return");
}*/
