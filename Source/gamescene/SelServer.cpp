//
// Created by Kull on 3/10/24.
//

#include "SelServer.h"
#include "MainMenu.h"
#include "MainLogin.h"
#include "gameui/Uiglobaldata.h"
#include "engine/dataChecksum.h"
#include "gamecore/KCore.h"
#include "gameui/Klogin.h"

bool SelServer::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _groot = GRoot::create(this);
    _groot->retain();

    UIPackage::addPackage("FGUI/SelServer");
//    UIConfig::registerFont(UIConfig::defaultFont, "fonts/UVNButLong2.TTF");
    _view = UIPackage::createObject("SelServer", "Main")->as<GComponent>();
    _groot->addChild(_view);

    _list = _view->getChild("list_server")->as<GList>();
    _list->addEventListener(UIEventType::ClickItem, AX_CALLBACK_1(SelServer::onClickItem, this));

    _view->getChild("bt_huy")->addClickListener([this](EventContext*) {
        this->mianCancelCallback(this);
    });

    if (nserlist.Load("\\Settings\\Ver_vn\\FsjxClient.ini"))//补丁包里面的版本号 GetDouble
    {
        int nserCount;
        nserlist.GetInteger("List","RegionCount",0,&nserCount);

        _list->removeChildrenToPool();
        for (int i = 0; i < nserCount; ++i)
        {
            char strSerName[128],cckey[32];
            sprintf(cckey,"Region_%d",i);
            nserlist.GetString(cckey,"0_Title","",strSerName,sizeof(strSerName));
            int nIsOpen=0;
            nserlist.GetInteger(cckey,"0_IsOpen",0,&nIsOpen);

            if (nIsOpen)
            {
                GButton* item = _list->addItemFromPool()->as<GButton>();
                item->setTitle(strSerName);
                item->setData(reinterpret_cast<void*>(static_cast<intptr_t>(i + 1)));
            }
        }
    }

    return true;
}

SelServer::SelServer() : _groot(nullptr)
{
}

SelServer::~SelServer()
{
    AX_SAFE_RELEASE(_groot);
}

void SelServer::onClickItem(EventContext* context)
{
    GObject* item = (GObject*)context->getData();
    if (item)
    {
        int index = static_cast<int>(reinterpret_cast<intptr_t>(item->getData())) - 1;
        KIniFile nClentMak;
        if (!nClentMak.Load(CLIENT_VER_FILE))
        {
            messageBox("Phiên bản không hợp lệ","Chú ý:");
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
            messageBox("Phiên bản không hợp lệ","Chú ý:");
            return;
        }

        if (nAcclient)
        {
            KIniFile dwmask;
            std::string fielDir ="";
            fielDir = std::string("data\\paklist_vn.ini");

            char nTempstr[128];
            sprintf(nTempstr, "%s", fielDir.c_str());

            if (!_clientNoNetWork && !dwmask.Load(nTempstr))
            {
                messageBox("Không thể kết nối tới máy chủ...","Chú ý:");
                return;
            }
            dwmask.Clear();

            char downUrlmask[64];
            ZeroMemory(downUrlmask,sizeof(downUrlmask));
            //dwmask.GetString("list","accinfo","",downUrlmask,sizeof(downUrlmask));

            char cckey[32];
            t_sprintf(cckey,"Region_%d", index);
            nserlist.GetString(cckey,"0_Address","",downUrlmask,sizeof(downUrlmask));

            dataChecksum nurlMask;
            char donwPath[64]={0};
            ZeroMemory(donwPath,sizeof(donwPath));
            nurlMask.SimplyDecrypt(donwPath,downUrlmask);

            if (nAcclient->FsGameServerConnectTo(donwPath,56722))
            {//连接帐号服务器
                //创建接收数据线程
                m_ClientChoices.nServerRegionIndex = index + 1;
                nAcclient->Startup();//启动接收数据线程
                m_bIsClientConnecting=true;
            }
            else
            {
                nAcclient->Cleanup();
                m_bIsClientConnecting=false;
                messageBox("Không thể kết nối tới máy chủ...","Chú ý:");
                return;
            }
        }
        else
        {
            m_bIsClientConnecting=false;
            messageBox("Không thể kết nối tới máy chủ...","Chú ý:");
            return;
        }
        Director::getInstance()->replaceScene(MainLogin::create());
    }
}

void SelServer::mianCancelCallback(Object* pSender)
{
    m_ClientChoices.nServerRegionIndex =-1;
    if (nAcclient)
    {
        m_bIsClientConnecting=false;
        m_bIsPlayerInGame =false;
        nAcclient->Cleanup();

    }
    Director::getInstance()->replaceScene(MainMenu::create());
}
