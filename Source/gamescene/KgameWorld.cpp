#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/Text.h"
#include "KgameWorld.h"
#include "engine/CXmlStream.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gamecore/KItemSet.h"
#include "gamecore/ImgRef.h"
#include "gameui/PopupLayer.h"
#include "gameui/uiSkill.h"
#include "gameui/KuiSkill.h"
#include "gameui/KuiItem.h"
#include "gameui/KuiItemEx.h"
#include "gameui/KuistoreBox.h"
#include "gameui/KuiTeam.h"
#include "gameui/Klogin.h"
#include "gameui/Klogin_f.h"
#include "gameui/KuiRoleState.h"
#include "gameui/KuiDialog.h"
#include "gameui/KuiTalkDialog.h"
#include "gameui/KuiTalkDialog1.h"
#include "gameui/KuiShop.h"
#include "gameui/KuigetStringInUI.h"
#include "gameui/KuiProgressbar.h"
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
#include "gamecore/KNpc.h"
#include "gamecore/KNpcSet.h"
#include "gamecore/KPlayer.h"
#include "gamecore/KPlayerSet.h"
#include "gamecore/KSubWorldSet.h"
#include "gameui/KDrawGameWorld.h"
#include "gamescene/KScenePlaceC.h"
#include "gameui/KDrawGameWorld.h"
#include "gameui/KuiCometoMap.h"
#include "gameui/KuiSortSkill_r.h"
#include "gameui/KuiSortSkill_l.h"
#include "gameui/Kuigive.h"
#include "gameui/Kuilaotou.h"
#include "gameui/KuiAutoPlay.h"
#include "gameui/KuiAutoPlay_vn.h"
#include "gameui/Kuiplayerfun.h"
#include "gameui/KuiViewRoleState.h"
#include "gameui/KuiPlayerShop.h"
#include "gameui/KuiPaimaishop.h"
#include "gameui/KuiSellItem.h"
#include "gameui/KuiPlayerTrade.h"
#include "gameui/KuiDynamicShop.h"
#include "gameui/KuiMaxMap.h"
#include "gameui/KuiMoveToMap.h"
#include "gameui/KuiItemdesc.h"
#include "gameui/KuiShowSprite.h"
#include "gameui/KuiTaskInfo.h"
#include "gameui/KuiTongInfo.h"
#include "gameui/KuiEscInfo.h"
#include "gameui/KuiTongCreateSheet.h"
// #include "gameui/KuiScrollView.h"
// #include "gameui/KuiTestLayer.h"
#include <math.h>
#include <iostream>
#include <fstream>
// #include <ctype.h>
#ifndef WIN32
// #include <xtype.h>
#endif

extern iCoreShell* g_pCoreShell;  // 全局调用
// extern KImageStore2 m_ImageStore; //全局调用
KgameWorld* g_GameWorld = NULL;  ////外部调用 更新游戏世界UI界面
// extern KPakList     g_pPakList;
// extern KPakList * g_pPakList;
#include <list>
USING_NS_AX;

#define MINIMAP_INI_SMALL "\\ui\\ui3\\小地图_小.ini"
#define MINIMAP_INI_BIG   "\\ui\\ui3\\小地图_浏览版.ini"
#define BTNSCAE           1.3
#define BTNOFFSETX        25
#define BTNOFFSETY        150
#define DRAW_COUNT        2
// 命令前缀字符
#define TEXT_CTRL_CMD_PREFIX '?'
// 聊天前缀字符
#define TEXT_CTRL_CHAT_PREFIX '/'
// 频道前缀字符
#define TEXT_CTRL_CHANNEL_PREFIX '&'
// 表情前缀字符
#define TEXT_CTRL_EMOTE_PREFIX '.'
#define RECONTNET_TIME         200
// 换场景可以调用这个函数
KgameWorld::KgameWorld()
{
    // ZeroMemory(nRegSprCount,sizeof(nRegSprCount));
    m_NpcIndex      = 0;
    __pingTime      = 0;
    m_useSkillModel = 0;  // 默认自动 0 自动 1 手动
    m_recontnetTime = RECONTNET_TIME;
    int i;
    /*RegData  = new mapRegData[MAX_NUM_REGIONS];
    for(int j=0;j<MAX_NUM_REGIONS;j++)//9大区域都删除一遍
    {
            nRegSprCount[j]=0;
            nOtherSprCount[j]=0;
            nPartSprCount[j]=0;

            // 优化查找表
      if (RegData)
      {
            RegData[j].nDrawKind = -1;
            RegData[j].nSprCount = 0;
            RegData[j].m_FreeIdxNode.Init(MAX_SPR_COUNT);
            RegData[j].m_UseIdxNode.Init(MAX_SPR_COUNT);

            RegData[j].m_FreeIdxPartNode.Init(MAX_SPR_COUNT);
            RegData[j].m_UseIdxPartNode.Init(MAX_SPR_COUNT);
            // 开始时所有的数组元素都为空 0  和 65 无效
            for (i = MAX_SPR_COUNT - 1; i > 0; i--)
            {
                    RegData[j].m_FreeIdxNode.Insert(i);
                    RegData[j].m_FreeIdxPartNode.Insert(i);
            }
      }
    }*/
    /*
    m_FreeIdxTreeNode.Init(MAX_TREES_COUNT);			//	可用表
    m_UseIdxTreeNode.Init(MAX_TREES_COUNT);			    //	已用表

    for (i = MAX_TREES_COUNT - 1; i > 0; i--)
            m_FreeIdxTreeNode.Insert(i);

    m_FreeIdxHouseNode.Init(MAX_HOUSE_COUNT);		    //	可用表
    m_UseIdxHouseNode.Init(MAX_HOUSE_COUNT);			//	已用表

    for (i = MAX_HOUSE_COUNT - 1; i > 0; i--)
            m_FreeIdxHouseNode.Insert(i);
    */
    /*m_FreeIdxAboveNode.Init(MAX_ABOVE_COUNT);
    m_UseIdxAboveNode.Init(MAX_ABOVE_COUNT);

    for (i = MAX_ABOVE_COUNT - 1; i > 0; i--)
            m_FreeIdxAboveNode.Insert(i);*/
    isHave = false;
    ZeroMemory(_strPing, sizeof(_strPing));
    KskillMenu = NULL;
    // skillstick      = NULL;
    pcontrolMenu            = NULL;
    sortskillMenu           = NULL;
    ImmediaMenu_1           = NULL;
    ImmediaMenu_2           = NULL;
    ImmediaMenu_3           = NULL;
    inputEditBox            = NULL;
    sfunLayer               = NULL;
    animation_attack        = NULL;
    Attackanimate           = NULL;
    actionManager           = NULL;
    _Maniattack_State       = NULL;
    m_skilPadIsOpen         = false;
    m_itemsPadIsOpen        = false;
    m_playerSellPadIsOpen   = false;
    m_teamPadIsOpen         = false;
    m_RoleStatePadIsOpen    = false;
    m_itemExPadIsOpen       = false;
    m_NpcDialogPadIsOpen    = false;
    m_talkDialogPadIsOpen   = false;
    m_shopDialogPadIsOpen   = false;
    m_storeboxPadIsOpen     = false;
    m_npcshopPadIsOpen      = false;
    m_getstringPadIsOpen    = false;
    isClose                 = false;
    m_talkDialogPadIsOpen_1 = false;
    m_itemLinkPadIsOpen     = false;
    m_ProgressPadIsOpen     = false;
    m_movemapPadIsOpen      = false;
    m_sortskilPadIsOpen_r   = false;
    m_sortskilPadIsOpen_l   = false;
    m_laotouPadIsOpen       = false;
    m_givePadIsOpen         = false;
    m_autoPadIsOpen         = false;
    m_cavePadIsOpen         = false;
    m_playerfunPadIsOpen    = false;
    m_viewPadIsOpen         = false;
    m_tanweoPadIsOpen       = false;
    m_tradePadIsOpen        = false;
    m_maxMapPadIsOpen       = false;
    m_taskPadIsOpen         = false;
    m_tongPadIsOpen         = false;
    m_escPadIsOpen          = false;
    m_tongCreatPadIsOpen    = false;
    pMainSkillItem          = NULL;  // 主技能按钮
    attackSpr_dir           = NULL;
    chatLayer               = NULL;
    pNetStateLabel          = NULL;
    psortSkillItem_l        = NULL;
    nCurChannelMsgCount     = 0;
    m_nCurChannel           = -1;
    memset(auxiliaryskill, 0, sizeof(auxiliarySkillData) * MAX_FUZHUSKILL_COUNT);
    memset(m_msgChannelData, 0, sizeof(msgListData) * MAX_FUZHUSKILL_COUNT);
    // memset(skillstick,0,sizeof(skillstickData) * MAX_FUZHUSKILL_COUNT);

    memset(m_szSelfName, 0, sizeof(m_szSelfName));
    for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; ++i)  // 快捷栏数量
    {
        m_ImmediaItem[i].m_uid    = 0;
        m_ImmediaItem[i].m_nIndex = i;
        // m_ImmediaItem[i].m_nIsSKill = 0;
        // m_ImmediaItem[i].m_mIsFull  = 0;
    }
    m_uLastSwitchTime = IR_GetCurrentTime();
}

KgameWorld::~KgameWorld()
{
    if (g_GameWorld)
    {
        // delete g_GameWorld;
        g_GameWorld = NULL;

        /*if (RegData)
        {
        delete [] RegData;
                RegData = NULL;
        }*/
    }
    m_skilPadIsOpen       = false;
    m_itemsPadIsOpen      = false;
    m_teamPadIsOpen       = false;
    m_RoleStatePadIsOpen  = false;
    m_itemExPadIsOpen     = false;
    m_NpcDialogPadIsOpen  = false;
    m_talkDialogPadIsOpen = false;
    m_shopDialogPadIsOpen = false;
    m_storeboxPadIsOpen   = false;
    m_npcshopPadIsOpen    = false;
    m_ProgressPadIsOpen   = false;
    mainattackSkill       = 0;
    chatoffsetY           = 0;
    m_ImmediaItemSelIndex = -1;
    memset(auxiliaryskill, 0, sizeof(auxiliarySkillData) * MAX_FUZHUSKILL_COUNT);
    // memset(skillstick,0,sizeof(skillstickData) * MAX_FUZHUSKILL_COUNT);
    memset(m_msgChannelData, 0, sizeof(msgListData) * MAX_FUZHUSKILL_COUNT);
    ChannelSpr = NULL;
    /*if (g_pClient)
    {
            g_pClient->setBufferEmpty();
            m_bIsPlayerInGame =false;
            m_bIsGameSevConnecting=false;
            g_pClient->Cleanup();
            g_pClient->Shutdown();

            delete g_pClient;
            g_pClient=NULL;
    }*/
    if (animation_attack)
        animation_attack->release();
}

Scene* KgameWorld::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    KgameWorld* layer = KgameWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool KgameWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }
    /*

    其中kCCTouchesAllAtOnce表示支持多点触摸，kCCTouchesOneByOne表示支持单点触摸，在调用了
    setTouchMode(kCCTouchesOneByOne)时，那么调用另外一套虚函数如下：

    // default implements are used to call script callback if exist

    当触摸开始时，虚函数virtual bool ccTouchBegan(Touch *pTouch, Event *pEvent);被调用

    当手指移动时，虚函数virtual void ccTouchMoved(Touch *pTouch, Event *pEvent);被调用

    当手指离开时，虚函数virtual void ccTouchEnded(Touch *pTouch, Event *pEvent);被调用

    当来电打断触摸过程时，虚函数virtual void ccTouchCancelled(Touch *pTouch, Event *pEvent);被调用
    */
    auto touchListener            = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = AX_CALLBACK_2(KgameWorld::ccTouchesBegan, this);
    touchListener->onTouchesMoved = AX_CALLBACK_2(KgameWorld::ccTouchesMoved, this);
    touchListener->onTouchesEnded = AX_CALLBACK_2(KgameWorld::ccTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    this->scheduleUpdate();  // 遥杆更新调用函数
                             // 禁止多点触摸
    // ClippingNode 裁剪精灵
    // DrawNode     绘画节点
    nLoopCount = 0;
    timerStart.Passed(1);

    ParentNode_chat         = NULL;
    ParentNode_map          = NULL;
    ParentNode_npc          = NULL;
    ParentNode_player       = NULL;
    ParentNode_miss         = NULL;
    ParentNode_miss_Special = NULL;
    ParentNode_item         = NULL;
    ParentNode_obj          = NULL;
    // ParentNode_LeftSkill=NULL;
    ParentNode_ShortSkill = NULL;
    ParentNode_minmap     = NULL;
    ParentNode_Bigmap     = NULL;
    ParentNode_rolestate  = NULL;
    ParentNode_npc_state  = NULL;
    ParentNode_npcother   = NULL;
    ParentNode_npcblood   = NULL;
    ParentNode_objabove   = NULL;

    // 快捷技能
    ParentNode_ShortSkill = Node::create();
    ParentNode_ShortSkill->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_ShortSkill->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_ShortSkill, ORDER_UI_DIALOD, OBJ_NODE_SHORTKILL);
    // 高空层
    ParentNode_objabove = Node::create();
    ParentNode_objabove->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_objabove->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_objabove, ORDER_OBJ_ABOVE, OBJ_NODE_COUNT);
    // 三个物品快捷栏
    ParentNode_item = Node::create();  // Sprite::create("ui/state/kbgitem.png");
    ParentNode_item->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_item->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_item, ORDER_UI_DIALOD, OBJ_NODE_ITEMBOX);
    // 123
    Sprite* imidspr_1   = Sprite::create("ui/state/kbgitem.png");
    Size nItEmBoxSize_1 = imidspr_1->getContentSize();
    imidspr_1->setPosition(ax::Vec2(visibleSize.width - nItEmBoxSize_1.width * 0.5 - 10,
                                    visibleSize.height / 2 + 20));  // 重左下角开始移动
    ParentNode_item->addChild(imidspr_1, 1, 1);
    // 456
    Sprite* imidspr_2 = Sprite::create("ui/state/kbgitem.png");
    imidspr_2->setPosition(ax::Vec2(visibleSize.width - nItEmBoxSize_1.width * 1.5 - 10,
                                    visibleSize.height / 2 + 20));  // 重左下角开始移动 +nItEmBoxSize_1.height/2
    ParentNode_item->addChild(imidspr_2, 1, 2);
    // 789
    Sprite* imidspr_3 = Sprite::create("ui/state/kbgitem.png");
    imidspr_3->setPosition(ax::Vec2(visibleSize.width - nItEmBoxSize_1.width * 2.5 - 10,
                                    visibleSize.height / 2 + 20));  // 重左下角开始移动+nItEmBoxSize_1.height/2
    ParentNode_item->addChild(imidspr_3, 1, 3);

    ImmediaMenu_1 = Menu::create(NULL);
    ImmediaMenu_1->setPosition(ax::Vec2::ZERO);
    ImmediaMenu_1->setAnchorPoint(ax::Vec2::ZERO);
    imidspr_1->addChild(ImmediaMenu_1, 0);

    ImmediaMenu_2 = Menu::create(NULL);
    ImmediaMenu_2->setPosition(ax::Vec2::ZERO);
    ImmediaMenu_2->setAnchorPoint(ax::Vec2::ZERO);
    imidspr_2->addChild(ImmediaMenu_2, 0);

    ImmediaMenu_3 = Menu::create(NULL);
    ImmediaMenu_3->setPosition(ax::Vec2::ZERO);
    ImmediaMenu_3->setAnchorPoint(ax::Vec2::ZERO);
    imidspr_3->addChild(ImmediaMenu_3, 0);
    // 聊天输入框
    ParentNode_chat = Node::create();
    ParentNode_chat->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_chat->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_chat, ORDER_UI_DIALOD, OBJ_NODE_CHAT);

    // 角色状态
    ParentNode_rolestate = Node::create();
    ParentNode_rolestate->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_rolestate->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_rolestate, ORDER_UI_DIALOD, OBJ_NODE_ROLESTATE);

    // 小地图
    ParentNode_minmap = Node::create();
    ParentNode_minmap->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_minmap->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_minmap, ORDER_UI_DIALOD, OBJ_NODE_MINMAP);

    ParentNode_Bigmap = Node::create();
    ParentNode_Bigmap->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_Bigmap->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_Bigmap, ORDER_TREE_HOUSE, OBJ_NODE_BIGMAP);

    ParentNode_map = Node::create();
    ParentNode_map->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_map->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_map, ORDER_MAP_UNDER, OBJ_NODE_MAP);
    /*for (int i=0;i<9;i++)
    {//预先增加 9个区域节点
            Node *Node_reg = Node::create();
            ParentNode_map->addChild(Node_reg,1,i+1);
            Node_reg->setPosition(ax::Vec2(0,0));
            Node_reg->setAnchorPoint(ax::Vec2(0,0));
    }*/
    // nRegCanvas = RenderTexture::create(512,512); //创建一个区域画板
    // ParentNode_map->addChild(nRegCanvas,-1);
    //
    ParentNode_miss_Special = Node::create();
    ParentNode_miss_Special->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_miss_Special->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_miss_Special, ORDER_NPC_PLAYER, OBJ_NODE_MISS_SPECIAL);

    ParentNode_miss = Node::create();
    ParentNode_miss->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_miss->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_miss, ORDER_NPC_PLAYER, OBJ_NODE_MISS);
    // NPC
    ParentNode_npc = Node::create();
    ParentNode_npc->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_npc->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_npc, ORDER_NPC_PLAYER, OBJ_NODE_NPC);

    ParentNode_npcother = Node::create();
    ParentNode_npcother->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_npcother->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_npcother, ORDER_NPC_PLAYER, OBJ_NODE_NPC_OTHER);

    ParentNode_npcblood = Node::create();
    ParentNode_npcblood->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_npcblood->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_npcblood, ORDER_NPC_PLAYER, OBJ_NODE_NPC_BLOOD);

    ParentNode_npc_state = Node::create();
    ParentNode_npc_state->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_npc_state->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_npc_state, ORDER_NPC_PLAYER, OBJ_NODE_SKILL_STATE);

    ParentNode_obj = Node::create();
    ParentNode_obj->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_obj->setPosition(ax::Vec2(0, 0));  // 重左下角开始移动
    this->addChild(ParentNode_obj, ORDER_OBJ_MIDDLE, OBJ_NODE_OBJSET);

    attackSpr_dir = Sprite::create("selpic/pkno1tong-012.png");
    // attackSpr_dir->setAnchorPoint(ax::Vec2(0,0));
    attackSpr_dir->setVisible(false);
    this->addChild(attackSpr_dir, ORDER_TREE_HOUSE, OBJ_NODE_COUNT + 1);
    // 顶部控制条按钮
    MenuItemImage* pRoleeqItem  = NULL;
    MenuItemImage* ppItemsItem  = NULL;
    MenuItemImage* pItemExItem  = NULL;
    MenuItemImage* pSkillsItem  = NULL;
    MenuItemImage* pTeamItem    = NULL;
    MenuItemImage* pFactionItem = NULL;
    MenuItemImage* pOptionsItem = NULL;
    MenuItemImage* ptaskItem    = NULL;
    MenuItemImage* pPaiItem     = NULL;
    MenuItemImage* pQizItem     = NULL;
    pRoleeqItem  = MenuItemImage::create("ui/tools/role.png", "ui/tools/role_d.png", "ui/tools/role_s.png",
                                         CC_CALLBACK_1(KgameWorld::mRoleStatusCallback, this));
    ppItemsItem  = MenuItemImage::create("ui/tools/itembox.png", "ui/tools/itembox_d.png", "ui/tools/itembox_s.png",
                                         CC_CALLBACK_1(KgameWorld::mItemsCallback, this));
    pItemExItem  = MenuItemImage::create("ui/tools/friend.png", "ui/tools/friend_d.png", "ui/tools/friend_s.png",
                                         CC_CALLBACK_1(KgameWorld::mItemExCallback, this));
    pSkillsItem  = MenuItemImage::create("ui/tools/skill.png", "ui/tools/skill_d.png", "ui/tools/skill_s.png",
                                         CC_CALLBACK_1(KgameWorld::mSkillsCallback, this));
    pTeamItem    = MenuItemImage::create("ui/tools/team.png", "ui/tools/team_d.png", "ui/tools/team_s.png",
                                         CC_CALLBACK_1(KgameWorld::mTeamCallback, this));
    pFactionItem = MenuItemImage::create("ui/tools/tong.png", "ui/tools/tong_d.png", "ui/tools/tong_d.png",
                                         CC_CALLBACK_1(KgameWorld::mFactionCallback, this));
    pOptionsItem = MenuItemImage::create("ui/tools/option.png", "ui/tools/option_d.png", "ui/tools/option_d.png",
                                         CC_CALLBACK_1(KgameWorld::mOptionsCallback, this));
    ptaskItem    = MenuItemImage::create("ui/tools/task.png", "ui/tools/task_d.png", "ui/tools/task_d.png",
                                         CC_CALLBACK_1(KgameWorld::mTaskCallback, this));
    pPaiItem     = MenuItemImage::create("ui/tools/team.png", "ui/tools/tong_d.png", "ui/tools/tong_d.png",
                                         CC_CALLBACK_1(KgameWorld::mPaimaiCallback, this));
    char nSprName[128] = {0};
    int m_nWidth, m_nHeight, nFrams;
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BaoVat.spr");
    g_StrLower(nSprName);
    Texture2D* pQiz = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (pQiz)
    {
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 2);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pQizItem = MenuItemImage::create();
        pQizItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pQizItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pQizItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pQizItem->setCallback(CC_CALLBACK_1(KgameWorld::mQizCallback, this));

        // pQizItem->setScale(0.8,0.8);
    }

    Menu* pControlMenu = NULL;
    if (pRoleeqItem)
    {
        pControlMenu = Menu::create(pRoleeqItem, ppItemsItem, pItemExItem, pSkillsItem, ptaskItem, pTeamItem,
                                    pFactionItem, pOptionsItem, pPaiItem, pQizItem, NULL);
        pControlMenu->alignItemsHorizontally();               // 横排排列 默认间隔是kDefaultPadding=5
        pControlMenu->alignItemsHorizontallyWithPadding(10);  // 指定横间隔

        // kích thước button thông tin nhân vật
        pRoleeqItem->setScale(1.1);
        Label* pRoleeqItemLabel = Label::createWithTTF("", UI_GAME_FONT_VN, 11);
        pRoleeqItemLabel->setString("Nhân vật");
        pRoleeqItemLabel->setColor(ax::Color3B::WHITE);
        pRoleeqItemLabel->setAnchorPoint(ax::Vec2(0, 0));
        pRoleeqItemLabel->enableOutline(Color4B(0, 0, 0, 255), 6);  // viền chữ đen
        pRoleeqItemLabel->setPosition(ax::Vec2(4, -12));
        // pRoleeqItemLabel->setScale(0.33);
        pRoleeqItem->addChild(pRoleeqItemLabel, 1);
        // kích thước button thông tin nhân vật

        // kích thước button Hành trang
        ppItemsItem->setScale(1.1);
        Label* ppItemsItemLabel = Label::createWithTTF("", UI_GAME_FONT_VN, 11);
        ppItemsItemLabel->setString("Hành trang");
        ppItemsItemLabel->setColor(ax::Color3B::WHITE);
        ppItemsItemLabel->setAnchorPoint(ax::Vec2(0, 0));
        ppItemsItemLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
        ppItemsItemLabel->setPosition(ax::Vec2(-2, -12));
        // ppItemsItemLabel->setScale(0.33);
        ppItemsItem->addChild(ppItemsItemLabel, 1);
        // kích thước button hành trang

        // kích thước button Kho
        pItemExItem->setScale(1.1);
        Label* pItemExItemLabel = Label::createWithTTF("", UI_GAME_FONT_VN, 11);
        pItemExItemLabel->setString("Bạn bè");
        pItemExItemLabel->setColor(ax::Color3B::WHITE);
        pItemExItemLabel->setAnchorPoint(ax::Vec2(0, 0));
        pItemExItemLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
        pItemExItemLabel->setPosition(ax::Vec2(8, -12));
        // pItemExItemLabel->setScale(0.33);
        pItemExItem->addChild(pItemExItemLabel, 1);
        // kích thước button hành Kho

        // kích thước button pSkillsItem
        pSkillsItem->setScale(1.1);
        Label* pSkillsItemLabel = Label::createWithTTF("", UI_GAME_FONT_VN, 11);
        pSkillsItemLabel->setString("Võ Công");
        pSkillsItemLabel->setColor(ax::Color3B::WHITE);
        pSkillsItemLabel->setAnchorPoint(ax::Vec2(0, 0));
        pSkillsItemLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
        pSkillsItemLabel->setPosition(ax::Vec2(5, -12));
        // pSkillsItemLabel->setScale(0.33);
        pSkillsItem->addChild(pSkillsItemLabel, 1);
        // kích thước button hành pSkillsItem

        // kích thước button ptaskItem
        ptaskItem->setScale(1.1);
        Label* ptaskItemLabel = Label::createWithTTF("", UI_GAME_FONT_VN, 11);
        ptaskItemLabel->setString("Nhiệm vụ");
        ptaskItemLabel->setColor(ax::Color3B::WHITE);
        ptaskItemLabel->setAnchorPoint(ax::Vec2(0, 0));
        ptaskItemLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
        ptaskItemLabel->setPosition(ax::Vec2(2, -12));
        // ptaskItemLabel->setScale(0.33);
        ptaskItem->addChild(ptaskItemLabel, 1);
        // kích thước button hành ptaskItem

        // kích thước button pTeamItem
        pTeamItem->setScale(1.1);
        Label* pTeamItemLabel = Label::createWithTTF("", UI_GAME_FONT_VN, 11);
        pTeamItemLabel->setString("Tổ đội");
        pTeamItemLabel->setColor(ax::Color3B::WHITE);
        pTeamItemLabel->setAnchorPoint(ax::Vec2(0, 0));
        pTeamItemLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
        pTeamItemLabel->setPosition(ax::Vec2(10, -12));
        // pTeamItemLabel->setScale(0.33);
        pTeamItem->addChild(pTeamItemLabel, 1);
        // kích thước button hành pTeamItem

        // kích thước button pFactionItem
        pFactionItem->setScale(1.1);
        Label* pFactionItemLabel = Label::createWithTTF("", UI_GAME_FONT_VN, 11);
        pFactionItemLabel->setString("Bang hội");
        pFactionItemLabel->setColor(ax::Color3B::WHITE);
        pFactionItemLabel->setAnchorPoint(ax::Vec2(0, 0));
        pFactionItemLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
        pFactionItemLabel->setPosition(ax::Vec2(3, -12));
        // pFactionItemLabel->setScale(0.33);
        pFactionItem->addChild(pFactionItemLabel, 1);
        // kích thước button hành pFactionItem

        // kích thước button pOptionsItem
        pOptionsItem->setScale(1.1);
        Label* pOptionsItemLabel = Label::createWithTTF("", UI_GAME_FONT_VN, 11);
        pOptionsItemLabel->setString("Cài đặt");
        pOptionsItemLabel->setColor(ax::Color3B::WHITE);
        pOptionsItemLabel->setAnchorPoint(ax::Vec2(0, 0));
        pOptionsItemLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
        pOptionsItemLabel->setPosition(ax::Vec2(8, -12));
        // pOptionsItemLabel->setScale(0.33);
        pOptionsItem->addChild(pOptionsItemLabel, 1);
        // kích thước button hành pOptionsItem

        /*
        //kích thước button pPaiItem
        pPaiItem->setScale(1.5);
        Label * pPaiItemLabel = Label::createWithTTF("",UI_GAME_FONT_SONG_VN, 32);
        pPaiItemLabel->setString("pPaiItem");
        pPaiItemLabel->setColor(ax::Color3B::WHITE);
        pPaiItemLabel->setAnchorPoint(ax::Vec2(0,0));
        pPaiItemLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
        pPaiItemLabel->setPosition(ax::Vec2(4,-12));
        pPaiItemLabel->setScale(0.4);
        pPaiItem->addChild(pPaiItemLabel,1);
        //kích thước button hành pPaiItem
        */
        // kích thước button pQizItem
        pQizItem->setPosition(203, 0);
        pQizItem->setScale(1.1);
        // Label * pQizItemLabel = Label::createWithTTF("",UI_GAME_FONT_SONG_VN, 32);
        // pQizItemLabel->setString("pQizItem");
        // pQizItemLabel->setColor(ax::Color3B::WHITE);
        // pQizItemLabel->setAnchorPoint(ax::Vec2(0,0));
        // pQizItemLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
        // pQizItemLabel->setPosition(ax::Vec2(4,-12));
        // pQizItemLabel->setScale(0.33);
        // pQizItem->addChild(pQizItemLabel,1);
        // kích thước button hành pQizItem

        pControlMenu->setPosition(
            ax::Vec2(visibleSize.width / 2 + origin.x + 80, visibleSize.height - 40));  // ax::Vec2::ZERO
        this->addChild(pControlMenu, ORDER_UI_DIALOD);
    }
    // 主技能按钮
    pMainSkillItem = MenuItemImage::create("ui/at/mr-1.png", "ui/at/mr-2.png",
                                           CC_CALLBACK_1(KgameWorld::mianSkillCallback, this));  // 回调函数

    pMainSkillItem->setScale(0.7);
    pMainSkillItem->setPosition(
        ax::Vec2(origin.x + visibleSize.width - pMainSkillItem->getContentSize().width / 2 - 40,  // 20
                 origin.y + pMainSkillItem->getContentSize().height / 2 + 30));                   // 10

    // 攻击模式却换
    // 主技能按钮
    MenuItemImage* pUseModelItem =
        MenuItemImage::create("attack/model/kind_z.png", "attack/model/kind_z.png",
                              CC_CALLBACK_1(KgameWorld::useModelCallback, this));  // 回调函数

    pUseModelItem->setScale(0.8);
    pUseModelItem->setPosition(pMainSkillItem->getPosition().x - 50, pMainSkillItem->getContentSize().height + 37);

    // tháo xuống
    MenuItemImage* pHorseItem;
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_GiaoDich.spr");  // Button Giao Dich
    g_StrLower(nSprName);
    Texture2D* pHorse = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (pHorse)
    {
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pHorseItem = MenuItemImage::create();
        pHorseItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pHorseItem->setSelectedImage(Sprite::createWithTexture(pDisabledTexture));
        pHorseItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pHorseItem->setCallback(CC_CALLBACK_1(KgameWorld::mianHorseCallback, this));

        pHorseItem->setScale(BTNSCAE, BTNSCAE);
        // origin.x + visibleSize.width - pHorseItem->getContentSize().width/2-150
        pHorseItem->setPosition(
            ax::Vec2(origin.x + BTNOFFSETX, origin.y + pHorseItem->getContentSize().height / 2 + 20 + BTNOFFSETY));
    }

    MenuItemImage* pSitItem;
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_CuoiNgua.spr");  // Button Đi Bộ
    g_StrLower(nSprName);
    Texture2D* pSit = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (pSit)
    {
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pSitItem = MenuItemImage::create();
        pSitItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pSitItem->setSelectedImage(Sprite::createWithTexture(pDisabledTexture));
        pSitItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pSitItem->setCallback(CC_CALLBACK_1(KgameWorld::mianSitCallback, this));

        pSitItem->setScale(BTNSCAE, BTNSCAE);
        // origin.x + visibleSize.width - pSitItem->getContentSize().width/2-145
        pSitItem->setPosition(
            ax::Vec2(origin.x + BTNOFFSETX, origin.y + pSitItem->getContentSize().height / 2 + 60 + BTNOFFSETY));
    }

    // 拾取物品 \spr\Ui3\鼠标\拣物品FOR2000.cur

    MenuItemImage* pPickItem;
    pPickItem = MenuItemImage::create("ui/btn/pick_01.png", "ui/btn/pick_02.png", "ui/btn/pick_02.png",
                                      CC_CALLBACK_1(KgameWorld::pickCallback, this));
    pPickItem->setScale(2, 2);
    pPickItem->setPosition(
        ax::Vec2(origin.x + BTNOFFSETX + 50, origin.y + pSitItem->getContentSize().height / 2 + 60 + BTNOFFSETY));

    MenuItemImage* pRunItem;
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_ChayBo.spr");  // Button Đi Bộ
    g_StrLower(nSprName);
    Texture2D* pRun = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (pRun)
    {  // 打坐按钮
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pRunItem = MenuItemImage::create();
        pRunItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pRunItem->setSelectedImage(Sprite::createWithTexture(pDisabledTexture));
        pRunItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pRunItem->setCallback(CC_CALLBACK_1(KgameWorld::mianRunCallback, this));

        pRunItem->setScale(BTNSCAE, BTNSCAE);
        // origin.x + visibleSize.width - pRunItem->getContentSize().width/2-130
        pRunItem->setPosition(
            ax::Vec2(origin.x + BTNOFFSETX, origin.y + pRunItem->getContentSize().height / 2 + 100 + BTNOFFSETY));
    }

    MenuItemImage* pPkItem;
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  // Button Bật Pk
    g_StrLower(nSprName);
    Texture2D* pPk = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (pPk)
    {
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pPkItem = MenuItemImage::create();
        pPkItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pPkItem->setSelectedImage(Sprite::createWithTexture(pDisabledTexture));
        pPkItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pPkItem->setCallback(CC_CALLBACK_1(KgameWorld::mianPkCallback, this));

        pPkItem->setScale(BTNSCAE, BTNSCAE);
        pPkItem->setPosition(
            ax::Vec2(origin.x + BTNOFFSETX, origin.y + pPkItem->getContentSize().height / 2 + 140 + BTNOFFSETY));
    }

    MenuItemImage* pExchangeItem;
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  //\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
    g_StrLower(nSprName);
    Texture2D* pExc = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (pExc)
    {  // 交易按钮
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);  // 这个市用来标记缓存的 不需要转换编码
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pExchangeItem = MenuItemImage::create();
        pExchangeItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pExchangeItem->setSelectedImage(Sprite::createWithTexture(pDisabledTexture));
        pExchangeItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pExchangeItem->setCallback(CC_CALLBACK_1(KgameWorld::mianExcCallback, this));

        pExchangeItem->setScale(BTNSCAE, BTNSCAE);
        // origin.x + visibleSize.width - pExchangeItem->getContentSize().width/2-110
        pExchangeItem->setPosition(
            ax::Vec2(origin.x + BTNOFFSETX, origin.y + pExchangeItem->getContentSize().height / 2 + 180 + BTNOFFSETY));
    }
    // 简化地图
    MenuItemImage* pDelmapItem;
    sprintf(nSprName, "%s", "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  //\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
    g_StrLower(nSprName);
    Texture2D* pdelm = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (pdelm)
    {  // 简化地图
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);  // 这个市用来标记缓存的 不需要转换编码
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pDelmapItem = MenuItemImage::create();
        pDelmapItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pDelmapItem->setSelectedImage(Sprite::createWithTexture(pDisabledTexture));
        pDelmapItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pDelmapItem->setCallback(CC_CALLBACK_1(KgameWorld::delmapCallback, this));

        // pDelmapItem->setScale(BTNSCAE,BTNSCAE);
        // origin.x + visibleSize.width - pExchangeItem->getContentSize().width/2-110
        pDelmapItem->setPosition(
            ax::Vec2(origin.x + BTNOFFSETX, origin.y + pDelmapItem->getContentSize().height / 2 + 230 + BTNOFFSETY));
    }

    // btn trò chuyện
    //		MenuItemImage *playerfunItem =
    // MenuItemImage::create("ui/btn_chat.png","ui/btn_chat.png",NULL,CC_CALLBACK_1(KgameWorld::playerfunCallback,
    // this));
    MenuItemImage* playerfunItem = MenuItemImage::create();
    auto image                   = ImageView::create("ui/btn_chat.png");
    playerfunItem->setNormalImage(image);
    playerfunItem->setSelectedImage(image);
    playerfunItem->setCallback(CC_CALLBACK_1(KgameWorld::playerfunCallback, this));
    playerfunItem->setScale(0.6);
    playerfunItem->setPosition(ax::Vec2(origin.x + visibleSize.width / 2.9 - 3, 68));
    // btn trò chuyện

    // 设置摆摊
    MenuItemImage* playersellItem;
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  //\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
    g_StrLower(nSprName);
    Texture2D* playersell = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (playersell)
    {
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);  // 这个市用来标记缓存的 不需要转换编码
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        playersellItem = MenuItemImage::create();
        playersellItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        playersellItem->setSelectedImage(Sprite::createWithTexture(pDisabledTexture));
        playersellItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        playersellItem->setCallback(CC_CALLBACK_1(KgameWorld::playerSellCallback, this));

        // pDelmapItem->setScale(BTNSCAE,BTNSCAE);
        // origin.x + visibleSize.width - pExchangeItem->getContentSize().width/2-110

        playersellItem->setPosition(ax::Vec2(origin.x + BTNOFFSETX + 80,
                                             origin.y + playerfunItem->getContentSize().height / 2 + 235 + BTNOFFSETY));
    }
    // 装备强化
    MenuItemImage* qianghItem = NULL;
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  //\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
    g_StrLower(nSprName);
    Texture2D* qiang = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (qiang)
    {
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);  // 这个市用来标记缓存的 不需要转换编码
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        qianghItem = MenuItemImage::create();
        qianghItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        qianghItem->setSelectedImage(Sprite::createWithTexture(pDisabledTexture));
        qianghItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        qianghItem->setCallback(CC_CALLBACK_1(KgameWorld::qiangItemCallback, this));

        // pDelmapItem->setScale(BTNSCAE,BTNSCAE);
        // origin.x + visibleSize.width - pExchangeItem->getContentSize().width/2-110
        qianghItem->setPosition(ax::Vec2(origin.x + BTNOFFSETX + 110,
                                         origin.y + playerfunItem->getContentSize().height / 2 + 235 + BTNOFFSETY));
    }

    // MenuItemSprite* item1=MenuItemSprite::create(btn_normal_sprite,
    // btn_select_sprite,nullptr,CC_CALLBACK_1(MenuItemSpritTest::select_learn,this));
    MenuItemImage* pquxiaItem;
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  //\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
    g_StrLower(nSprName);
    Texture2D* pqux = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (pqux)
    {  // 取下按钮
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);  // 这个市用来标记缓存的 不需要转换编码
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pquxiaItem = MenuItemImage::create();
        pquxiaItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pquxiaItem->setSelectedImage(Sprite::createWithTexture(pDisabledTexture));
        pquxiaItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pquxiaItem->setCallback(CC_CALLBACK_1(KgameWorld::mianQuCallback, this));

        pquxiaItem->setPosition(
            ax::Vec2(origin.x + visibleSize.width - pquxiaItem->getContentSize().width / 2 - nItEmBoxSize_1.width - 15,
                     origin.y + pquxiaItem->getContentSize().height / 2 + 180));
        char mKey[32];
        sprintf(mKey, "quxia");
        //		int nikey = std::hash<std::string>{}(mKey);
        pquxiaItem->setName(mKey);
    }

    MenuItemImage* pyincItem;
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  //\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
    g_StrLower(nSprName);
    Texture2D* pyin = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    if (pyin)
    {  // 隐藏按钮
        char nSprFilePath[64]       = {0};
        unsigned long nFielpahtdwid = g_FileName2Id(nSprName);  // 这个市用来标记缓存的 不需要转换编码
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 0);
        Texture2D* pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
        sprintf(nSprFilePath, "%u-%d", nFielpahtdwid, 1);
        Texture2D* pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pyincItem = MenuItemImage::create();
        pyincItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pyincItem->setSelectedImage(Sprite::createWithTexture(pDisabledTexture));
        pyincItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pyincItem->setCallback(CC_CALLBACK_1(KgameWorld::mianYinCallback, this));

        pyincItem->setPosition(ax::Vec2(origin.x + visibleSize.width - pyincItem->getContentSize().width / 2 - 10,
                                        origin.y + pyincItem->getContentSize().height / 2 + 180));
    }

    // 创建一个按钮菜单
    pcontrolMenu =
        Menu::create(pUseModelItem, pHorseItem, pSitItem, pRunItem, pPkItem, pExchangeItem, pquxiaItem, pyincItem,
                     pDelmapItem, playerfunItem, playersellItem, pPickItem, qianghItem, pMainSkillItem, NULL);
    pcontrolMenu->setPosition(ax::Vec2::ZERO);
    this->addChild(pcontrolMenu, ORDER_UI_DIALOD);
    // 创建一个攻击按钮的动画

    actionManager = ax::Director::getInstance()->getActionManager();

    animation_attack  = Animation::create();  // 创建一个动画
    _Maniattack_State = Sprite::create();
    _Maniattack_State->setPosition(pMainSkillItem->getPosition());
    //_Maniattack_State->setVisible(false);
    this->addChild(_Maniattack_State, ORDER_COUNT);  // 最上层

    int j;
    char nTempPath[128];

    for (j = 0; j < 9; j++)
    {
        sprintf(nTempPath, "attack/attack_%d.png", j);
        animation_attack->addSpriteFrameWithFile(nTempPath);  // 往这个动画增加纹理帧
    }
    // 设置动画帧的时间间隔
    animation_attack->setDelayPerUnit(0.01f);
    // 设置播放循环 一直播放 为-1
    animation_attack->setLoops(1);
    // 设置动画结束后恢复到第一帧 setRestoreOriginalFrame
    animation_attack->setRestoreOriginalFrame(TRUE);

    animation_attack->retain();
    // 创建一个永远循环的动画
    // Attackanimate = Animate::create(animation_attack);
    // 加入动画缓存
    // ax::AnimationCache::getInstance()->addAnimation(animation_attack,"main_attack");

    // 创建一个快捷键技能菜单
    // 创建一个背景
    Sprite* sortskillbg = Sprite::create("ui/sortskill_g.png");
    sortskillbg->setAnchorPoint(ax::Vec2(0, 0));
    sortskillbg->setPosition(ax::Vec2(visibleSize.width / 2 + 200, 1));
    ParentNode_ShortSkill->addChild(sortskillbg, 1, 1);

    Texture2D* pSortskill_rn_Texture = ax::Director::getInstance()->getTextureCache()->addImage("ui/sortskill_r_1.png");
    Texture2D* pSortskill_s_Texture  = ax::Director::getInstance()->getTextureCache()->addImage("ui/sortskill_r_2.png");

    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  //\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
    g_StrLower(nSprName);
    Texture2D* pSortskill_n_Texture = _getinidata.getinidata_new(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams);
    MenuItemImage* psortSkillItem_r = NULL;

    if (pSortskill_n_Texture)
    {
        psortSkillItem_r = MenuItemImage::create();
        psortSkillItem_r->setNormalImage(Sprite::createWithTexture(pSortskill_rn_Texture));
        psortSkillItem_r->setSelectedImage(Sprite::createWithTexture(pSortskill_s_Texture));
        psortSkillItem_r->setDisabledImage(Sprite::createWithTexture(pSortskill_s_Texture));
        psortSkillItem_r->setCallback(CC_CALLBACK_1(KgameWorld::mianSortSillCallback, this));

        psortSkillItem_r->setTag(2);
        psortSkillItem_r->setScale(0.8);
        psortSkillItem_r->setPosition(ax::Vec2(256, 48));

        psortSkillItem_l = MenuItemImage::create();
        psortSkillItem_l->setNormalImage(Sprite::createWithTexture(pSortskill_n_Texture));
        psortSkillItem_l->setSelectedImage(Sprite::createWithTexture(pSortskill_n_Texture));
        psortSkillItem_l->setDisabledImage(Sprite::createWithTexture(pSortskill_n_Texture));
        psortSkillItem_l->setCallback(CC_CALLBACK_1(KgameWorld::mianSortSillCallback, this));

        psortSkillItem_l->setTag(1);
        psortSkillItem_l->setPosition(ax::Vec2(221, 48));
    }
    sortskillMenu = Menu::create(psortSkillItem_l, psortSkillItem_r, NULL);
    sortskillMenu->setPosition(visibleSize.width / 2, 1);
    ParentNode_ShortSkill->addChild(sortskillMenu, ORDER_UI_DIALOD, 2);

    // const char * strTiele = CXmlStream::GetStringByKeyFromFile("string.xml","title"); //Resources
    // 资源放在目录下,如果没有这个文件程序会崩溃 KIniFile test; KPakList g_pPakList; char strtest[32]={0};
    // FileUtils::getPathForFilename()
    // std::string nPath = ax::FileUtils::getInstance()->getWritablePath();
    // messageBox(nPath.c_str(),"getWritablePath");//可读写路径 /data/data/包名/files/
    // nPath = ax::FileUtils::getInstance()->fullPathForFilename("package.ini");
    // nPath = ax::FileUtils::getInstance()->getWritablePath();//getWriteablePath();
    // nPath += "download";
    // int nCount = 0;
    // tolower() //大写转小写
    // toupper() //小写转大写
    /*char nCurStr[128]={0};
        sprintf(nCurStr,"%d \n time:%u",nCount,timerStart.GetElapse());
        pMainLabel = Label::createWithTTF(strTiele,"Arial", 24);
        pMainLabel->setString(nCurStr);//设置显示的问题
        pMainLabel->setPosition(ax::Vec2(origin.x + visibleSize.width/2,
                origin.y + visibleSize.height - pMainLabel->getContentSize().height));
        this->addChild(pMainLabel,1);*/
    // 渲染纹理 图片数据
    /*int nFrams=0;
    Texture2D *Cur =
    _getinidata.getinidata_new("\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr",10,&m_nWidth,&m_nHeight,&nFrams);
    testSprite = NULL;
    if (Cur)
    {
                    testSprite=Sprite::createWithTexture(Cur);
                    testSprite->setPosition(ax::Vec2(m_nWidth + origin.x+100, visibleSize.height/2 + origin.y));
                    m_sprCount++;
                    this->addChild(testSprite,2,m_sprCount);

                    char nSprFilePath[64]={0};
                    Animation*animation = Animation::create();//创建一个动画

        //std:string nFileName ="\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr";
                    //g_StrLower(nFileName);
                    unsigned long nFielpahtdwid = g_FileName2Id("\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr");

                    for (int i=1;i<nFrams;i++)
                    {
                            //Texture2D* nCurCount=NULL;
                            //sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,i);
                            //messageBox(nSprFilePath,"animation");
                            //nCurCount =
    ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);//如果存在的 就
                            //if (nCurCount)
                            //testSprite->setTexture(nCurCount);//如果没有 会自动创建一个
                            animation->addSpriteFrameWithFile(nSprFilePath);//往这个动画增加纹理帧
                    }
                    //设置动画帧的时间间隔
                    animation->setDelayPerUnit(0.02f);
                    //设置播放循环 一直播放 为-1
                animation->setLoops(-1);
                    //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
                    animation->setRestoreOriginalFrame(TRUE);
                    // 创建一个永远循环的动画
                    animate = Animate::create(animation);

                    //RepeatForever* forever = RepeatForever::create(animate);
                    if (testSprite)
                       //testSprite->runAction(animate);
                       testSprite->runAction(Sequence::create(animate,CallFunc::create(this,
    callfunc_selector(KgameWorld::finish)),NULL));

                    //if(testSprite->isActionDone(animate))
                     // removeChild(testSprite,true);
                    //CCMoveTo* move = CCMoveTo::create(30, ax::Vec2(-20, 200));
                    //testSprite->runAction(move);
                    //removeChild(testSprite,true);
    }
    */
    // SpriteFrameCache
    // TextureCache 纹理缓存
    /*
    void addSpriteFramesWithFile(const char *pszPlist)
    ：从一个.plist文件添加多个精灵帧。 一个纹理将被自动加载。纹理名称将把.plist后缀名替换为.png来组成。
    void addSpriteFramesWithFile(const char* plist, const char* textureFileName)
    ：通过一个.plist文件添加多个精灵帧。纹理将与被创建的精灵帧结合。
    void addSpriteFramesWithFile(const char *pszPlist, Texture2D *pobTexture)
    ：通过一个.plist文件添加多个精灵帧。纹理将与被创建的精灵帧结合。
    void addSpriteFrame(SpriteFrame *pobFrame, const char *pszFrameName)
    ：通过给定的名称添加一个精灵帧。 如果名称已经存在，那么原来名称的内容将被新的所替代
    */
    // 摇杆
    Sprite* controlSprite = Sprite::create("yaoganx.png");
    controlSprite->setScale(1.3);
    // 摇杆背景
    Sprite* ygSprite = Sprite::create("yaoganz.png");  //"yaoganz.png"
    ygSprite->setOpacity(50);

    joystick = HRocker::HRockerWithCenter(ax::Vec2(100.0f, 100.0f), 60.0f, controlSprite, ygSprite, false);
    this->addChild(joystick, ORDER_UI_DIALOD);
    if (joystick)
    {  // 设置摇杆的控制指针
        joystick->setCoreShell(g_pCoreShell);
    }

    /* for (int i=0;i<MAX_FUZHUSKILL_COUNT;i++)
         {
                 skillstick[i].controlSprite_s=Sprite::create("yaoganx.png");
                 skillstick[i].controlSprite_s->setScale(1.3);
                 skillstick[i].controlSprite_s->setOpacity(100);
                 //摇杆背景
                 skillstick[i].ygSprite_s=Sprite::create("yaoganz.png");//"yaoganz.png"
                 skillstick[i].ygSprite_s->setOpacity(100);

                 skillstick[i].skillstick=KSkillRocker::KSkillRockerWithCenter(ax::Vec2(visibleSize.width,-100.0f),60.0f
       ,skillstick[i].controlSprite_s ,skillstick[i].ygSprite_s,false);
                 this->addChild(skillstick[i].skillstick,ORDER_COUNT+i);
                 //pcontrolMenu->addChild(skillstick,50);
                 if  (skillstick[i].skillstick)
                 {//设置摇杆的控制指针
                         skillstick[i].skillstick->setCoreShell(g_pCoreShell);
                         //skillstick->setSpriteVisible(false);//隐藏了
                         //skillstick->setSpritePiont();
                 }
         }*/

    Sprite* controlSprite_s = Sprite::create("yaoganx.png");
    controlSprite_s->setScale(1.3);
    // 摇杆背景
    Sprite* ygSprite_s = Sprite::create("yaoganz.png");  //"yaoganz.png"
    ygSprite_s->setOpacity(50);

    /*skillstick=KSkillRocker::KSkillRockerWithCenter(ax::Vec2(visibleSize.width,-100.0f),60.0f ,controlSprite_s
    ,ygSprite_s,false); this->addChild(skillstick,ORDER_COUNT);
    //pcontrolMenu->addChild(skillstick,50);
    if  (skillstick)
    {//设置摇杆的控制指针
            skillstick->setCoreShell(g_pCoreShell);
            //skillstick->setSpriteVisible(false);//隐藏了
            //skillstick->setSpritePiont();
    }*/

    KskillMenu = KuiMyMenu::menuWithItem(NULL);
    this->addChild(KskillMenu, ORDER_UI_DIALOD);
    // 攻击技能显示
    // attackSpr =Sprite::create();
    // attackSpr->setAnchorPoint(ax::Vec2(0,0));
    // this->addChild(attackSpr,2);
    Rect CCRectZero;
    // 聊天
    // ParentNode_chat
    // 经验条
    Sprite* tiliSprite = Sprite::create("ui/tili.png");
    tiliSprite->setAnchorPoint(ax::Vec2(0, 0));
    Size nchatsize = tiliSprite->getContentSize();
    tiliSprite->setPosition(ax::Vec2(visibleSize.width / 2 - nchatsize.width / 2, 1));
    ParentNode_chat->addChild(tiliSprite, 1, CHAT_NODE_TILI);

    // 聊天显示背景
    ax::Color3B nChatbgColor = {0, 0, 0};  // 255,255,0 黄
    Color4B color_4(0, 0, 0, 0);
    LayerColor* MainChatSprite = LayerColor::create(color_4);  // 颜色层
    //	ui::Layout *MainChatSprite = ui::Layout::create();
    // Sprite *MainChatSprite=Sprite::create();
    MainChatSprite->setAnchorPoint(ax::Vec2(0, 0));
    CCRectZero = ax::Rect(0, 0, nchatsize.width, 20);
    MainChatSprite->setContentSize(CCRectZero.size);
    MainChatSprite->setPosition(ax::Vec2(visibleSize.width / 2 - nchatsize.width / 2, nchatsize.height + 4));
    ParentNode_chat->addChild(MainChatSprite, 0, CHAT_NODE_BGSPR);

    auto callback = [this](ax::Node* sender) { KgameWorld::closeChatlistPad(sender); };

    chatLayer = KuiChatList::create(this, callback);
    ParentNode_chat->addChild(chatLayer, ORDER_UI_DIALOD, CHAT_NODE_CHATLIST);
    // 聊天编辑框
    // nút ẩn khung chat
    Texture2D* bgCur = NULL;
    SPRFRAMSINFO nSprInfo;
    ZeroMemory(&nSprInfo, sizeof(nSprInfo));

    auto imageChannelSpr = ImageView::create("ui/btn_square1.png");
    //	ChannelSpr =
    // MenuItemImage::create(imageChannelSpr,imageChannelSpr,NULL,CC_CALLBACK_1(KgameWorld::hidechatCallback, this));
    ChannelSpr = MenuItemImage::create();
    ChannelSpr->setNormalImage(imageChannelSpr);
    ChannelSpr->setSelectedImage(imageChannelSpr);
    ChannelSpr->setCallback(CC_CALLBACK_1(KgameWorld::hidechatCallback, this));
    ChannelSpr->setPosition(ax::Vec2(-50, 0));
    // nút ẩn khung chat
    Sprite* inputSpr = Sprite::create("ui/chatbar.png");
    inputSpr->setPosition(ax::Vec2(140, 15));
    float nTempScaleX = MainChatSprite->getContentSize().width / inputSpr->getContentSize().width;
    inputSpr->setScaleX(nTempScaleX);
    MainChatSprite->addChild(inputSpr, 1);
    // hộp chỉnh sửa đầu vào
    ax::ui::Scale9Sprite* pBg = ax::ui::Scale9Sprite::create();
    inputEditBox              = EditBox::create(Size(545, 28), pBg);
    const ax::Color3B nfcolor = {255, 255, 255};
    if (inputEditBox)
    {
        inputEditBox->setPlaceHolder("Nhập nội dung chat");
        inputEditBox->setPlaceholderFontColor(ax::Color3B::WHITE);
        inputEditBox->setFontColor(nfcolor);
        inputEditBox->setFontSize(24);
        inputEditBox->setMaxLength(125);
        inputEditBox->setPosition(ax::Vec2(inputEditBox->getContentSize().width / 2, 20));
        inputEditBox->setInputMode(ax::ui::EditBox::InputMode::ANY);
        inputEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
        inputEditBox->setFontName(UI_GAME_FONT_DEFAULT_VN);
        MainChatSprite->addChild(inputEditBox, 2);
    }
    Sprite* picSpr = Sprite::create("ui/img_emoji.png");  // img_emoji.png
    picSpr->setPosition(ax::Vec2(MainChatSprite->getContentSize().width - picSpr->getContentSize().width + 50, 55));
    MainChatSprite->addChild(picSpr, 2);
    // nút biểu tượng cảm xúc
    auto imageChatsendSpr = ImageView::create("ui/btn_square1.png");
    //	auto chatsendSpr =
    // MenuItemImage::create(imageChatsendSpr,imageChatsendSpr,NULL,CC_CALLBACK_1(KgameWorld::onsendCallback, this));
    auto chatsendSpr = MenuItemImage::create();
    chatsendSpr->setNormalImage(imageChatsendSpr);
    chatsendSpr->setSelectedImage(imageChatsendSpr);
    chatsendSpr->setCallback(CC_CALLBACK_1(KgameWorld::onsendCallback, this));
    Label* chatSendLabel = Label::createWithTTF("Gửi", UI_GAME_FONT_DEFAULT_VN_BTN, 25);
    chatSendLabel->setColor(ax::Color3B::WHITE);
    chatSendLabel->setAnchorPoint(ax::Vec2(0, 0));
    chatSendLabel->enableOutline(Color4B(0, 0, 0, 255), 3);  // viền chữ đen
    chatSendLabel->setPosition(
        ax::Vec2(chatsendSpr->getContentSize().width / 2 - chatSendLabel->getContentSize().width / 4,
                 chatsendSpr->getContentSize().height / 4));
    chatsendSpr->addChild(chatSendLabel, 1);
    chatsendSpr->setScale(0.6);
    chatsendSpr->setPosition(ax::Vec2(picSpr->getPositionX() - picSpr->getContentSize().width / 4, 8));

    Menu* pchatMenu = Menu::create(ChannelSpr, chatsendSpr, NULL);
    pchatMenu->setPosition(ax::Vec2(10, 10));
    MainChatSprite->addChild(pchatMenu, 2);

    // Khung HP/MP
    Sprite* nRoleStateMainNode = Sprite::create("ui/rolestate.png");
    nRoleStateMainNode->setAnchorPoint(ax::Vec2(0, 0));
    Size nbgsize = nRoleStateMainNode->getContentSize();  // 背景
    // TEN NHÂN VẬT
    pRoleNameLabel         = Label::createWithTTF("", UI_GAME_FONT_VN, 12);
    ax::Color3B nNameColor = {255, 255, 0};  // 255,255,0 黄
    pRoleNameLabel->setColor(ax::Color3B::WHITE);
    pRoleNameLabel->setAnchorPoint(ax::Vec2(0, 0));
    pRoleNameLabel->enableOutline(Color4B(0, 0, 0, 255), 4);
    pRoleNameLabel->setPosition(ax::Vec2(105, 52));
    nRoleStateMainNode->addChild(pRoleNameLabel, 1, ROLESTATE_NODE_NAME);
    // HP
    Sprite* nRoleStateLifeNode = Sprite::create("ui/statelife.png");
    nRoleStateLifeNode->setAnchorPoint(ax::Vec2(0, 0));
    nRoleStateLifeNode->setPosition(ax::Vec2(70, 35));
    nRoleStateMainNode->addChild(nRoleStateLifeNode, 1, ROLESTATE_NODE_LIFE);
    // MP
    Sprite* nRoleStateLanNode = Sprite::create("ui/statemana.png");
    nRoleStateLanNode->setAnchorPoint(ax::Vec2(0, 0));
    nRoleStateLanNode->setPosition(ax::Vec2(70, 17));
    nRoleStateMainNode->addChild(nRoleStateLanNode, 1, ROLESTATE_NODE_LAN);
    // SAITAMA
    Sprite* nRoleStateSaitamaNode = Sprite::create("ui/statestamina.png");
    nRoleStateSaitamaNode->setAnchorPoint(ax::Vec2(0, 0));
    nRoleStateSaitamaNode->setPosition(ax::Vec2(70, 0));
    nRoleStateMainNode->addChild(nRoleStateSaitamaNode, 1, ROLESTATE_NODE_SAITAMA);
    // PING
    pNetStateLabel = Label::createWithTTF("ping:0", UI_GAME_FONT_VN, 12);
    pNetStateLabel->setColor(ax::Color3B::YELLOW);
    pNetStateLabel->setAnchorPoint(ax::Vec2(0, 0));
    pNetStateLabel->setPosition(ax::Vec2(185, 52));
    pNetStateLabel->enableOutline(Color4B(0, 0, 0, 255), 4);
    nRoleStateMainNode->addChild(pNetStateLabel, 1, 0);

    pmanaLabel    = NULL;
    plifeLabel    = NULL;
    pSaitamaLabel = NULL;

    // plifeLabel =
    // Label::createWithTTF("0/0",UI_GAME_FONT_DEFAULT_VN,24,nRoleStateLifeNode->getContentSize(),TextHAlignment::CENTER);//Arial
    // Thêm cột hiển thị thể lực: nRoleStateSaitamaNode
    plifeLabel = Label::createWithTTF("0/0", UI_GAME_FONT_VN, 13);  // Arial
    plifeLabel->setColor(ax::Color3B::WHITE);
    plifeLabel->setAnchorPoint(ax::Vec2(0, 0));
    plifeLabel->enableOutline(Color4B(0, 0, 0, 255), 4);  // viền chữ đen

    pmanaLabel = Label::createWithTTF("0/0", UI_GAME_FONT_VN, 13);  // Arial
    pmanaLabel->setColor(ax::Color3B::WHITE);
    pmanaLabel->setAnchorPoint(ax::Vec2(0, 0));
    pmanaLabel->enableOutline(Color4B(0, 0, 0, 255), 4);  // viền chữ đen

    pSaitamaLabel = Label::createWithTTF("0/0", UI_GAME_FONT_VN, 13);  // Arial
    pSaitamaLabel->setColor(ax::Color3B::WHITE);
    pSaitamaLabel->setAnchorPoint(ax::Vec2(0, 0));
    pSaitamaLabel->enableOutline(Color4B(0, 0, 0, 255), 4);  // viền chữ đen

    Point nTempPos;
    nTempPos.x = nRoleStateLifeNode->getPosition().x + nRoleStateLifeNode->getContentSize().width / 2 - 28;
    nTempPos.y = nRoleStateLifeNode->getPosition().y - 2;
    plifeLabel->setPosition(nTempPos);

    nTempPos.x = nRoleStateLanNode->getPosition().x + nRoleStateLanNode->getContentSize().width / 2 - 28;
    nTempPos.y = nRoleStateLanNode->getPosition().y - 2;
    pmanaLabel->setPosition(nTempPos);

    nTempPos.x = nRoleStateSaitamaNode->getPosition().x + nRoleStateSaitamaNode->getContentSize().width / 2 - 28;
    nTempPos.y = nRoleStateSaitamaNode->getPosition().y - 2;
    pSaitamaLabel->setPosition(nTempPos);

    nRoleStateMainNode->addChild(plifeLabel, 2);
    nRoleStateMainNode->addChild(pmanaLabel, 2);
    nRoleStateMainNode->addChild(pSaitamaLabel, 2);
    // nRoleStateMainNode->setScale(1.5);
    ParentNode_rolestate->addChild(nRoleStateMainNode, 0, ROLESTATE_NODE_BGSPR);
    ParentNode_rolestate->setPosition(ax::Vec2(20, visibleSize.height - nbgsize.height - 10));

    // 小地图
    KIniFile pIni;
    // 设置小地图的大小
    if (pIni.Load(MINIMAP_INI_SMALL))
    {
        pIni.GetInteger("MapRect", "Left", 0, (int*)&m_MapPos.x);
        pIni.GetInteger("MapRect", "Top", 0, (int*)&m_MapPos.y);
        pIni.GetInteger("MapRect", "Width", 0, (int*)&m_MapSize.cx);
        pIni.GetInteger("MapRect", "Height", 0, (int*)&m_MapSize.cy);
        int inOpen = false;
        pIni.GetInteger("shop", "shop_1", 0, &inOpen);

        if (inOpen <= 0 && pPaiItem)
            pPaiItem->setVisible(false);
        else
            pPaiItem->setVisible(true);

        pIni.GetInteger("shop", "shop_2", 0, &inOpen);

        if (inOpen <= 0 && pQizItem)
            pQizItem->setVisible(false);
        else
            pQizItem->setVisible(true);

        pIni.Clear();
        //  messageBox("存在","存在");
    }
    Sprite* nMineMapNode = Sprite::create();
    Size nMapSize;  //= ((float)m_MapSize.cx,(float)m_MapSize.cy);
    nMapSize.width  = (float)m_MapSize.cx;
    nMapSize.height = (float)m_MapSize.cy;
    // nMineMapNode->setContentSize(nMapSize);
    nMineMapNode->setAnchorPoint(ax::Vec2(0, 0));
    CCRectZero = ax::Rect(0, 0, nMapSize.width, nMapSize.height);
    nMineMapNode->setTextureRect(CCRectZero, false, CCRectZero.size);
    // nMineMapNode->setOpacity(100);//255;//设置透明度为完全不透明（范围0～255）
    nMineMapNode->setPosition(ax::Vec2(visibleSize.width - nMapSize.width, visibleSize.height - nMapSize.height));

    float x, y;
    nMineMapNode->getPosition(&x, &y);
    nMapStartLeft = (int)x;
    nMapStartTop  = 0;
    // nMineMapNode->setColor(ax::Color3B::BLACK);
    ParentNode_minmap->addChild(nMineMapNode, 0, MINIMAP_NODE_BGSPR);
    // SubWorld[0].GetMpsByLocalPosition(nMapStartLeft,nMapStartTop,0);
    // 地图名称
    pMapNameLabel = Label::createWithTTF("", UI_GAME_FONT_DEFAULT, 12);  // Arial
    g_ScenePlace.GetSceneNameAndFocus(nMapName, nMapidx, nPosX, nPosY, nMapKind);
    // g_ScenePlace.GetSceneFocus(nMapidx,nPosX,nPosY);
    ax::Color4B nlineColor = {0, 0, 0, 255};      // 0,0,255蓝色 //255,255,0 黄
    pMapNameLabel->enableOutline(nlineColor, 1);  // 开始描边
    ParentNode_minmap->addChild(pMapNameLabel, 1, MINIMAP_NODE_NAME);
    pMapNameLabel->setString(UTEXT(nMapName, 1).c_str());
    ax::Color3B nColor = {0, 255, 0};  // 255,255,0 黄
    pMapNameLabel->setColor(nColor);
    pMapNameLabel->setPosition(ax::Vec2(visibleSize.width - nMapSize.width / 2, visibleSize.height - 5));
    // 主角当前坐标
    ptestLabel = Label::createWithTTF("0", "fonts/number_16_w.fnt", 10);
    ptestLabel->setPosition(
        ax::Vec2(visibleSize.width - nMapSize.width / 2, visibleSize.height - nMapSize.height + 20));
    ParentNode_minmap->addChild(ptestLabel, 1, MINIMAP_NODE_COUNT);

    //\Spr\Ui3\小地图\小地图－切换按钮0.spr

    //	Texture2D *bgCur = NULL;
    //	SPRFRAMSINFO nSprInfo;
    ZeroMemory(&nSprInfo, sizeof(nSprInfo));
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");
    g_StrLower(nSprName);
    bgCur                     = _getinidata.getinidata_one(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
    Sprite* sprite_max_normal = NULL;
    Sprite* sprite_max_select = NULL;
    if (bgCur)
    {
        sprite_max_normal = Sprite::createWithTexture(bgCur);
        sprite_max_normal->setAnchorPoint(ax::Vec2(0, 0));
    }
    bgCur = _getinidata.getinidata_one(nSprName, 1, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
    if (bgCur)
    {
        sprite_max_select = Sprite::createWithTexture(bgCur);
        sprite_max_select->setAnchorPoint(ax::Vec2(0, 0));
    }
    MenuItemSprite* maxMapConfirm = MenuItemSprite::create(sprite_max_normal, sprite_max_select,
                                                           CC_CALLBACK_1(KgameWorld::maxmapCallBackFunc, this));
    maxMapConfirm->setPosition(ax::Vec2(250, 5));
    maxMapConfirm->setScale(2.0);

    // 活动引导按钮
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  //\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
    g_StrLower(nSprName);
    Texture2D* pHuo_1 = _getinidata.getinidata_one(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
    Sprite* btn_normal_huo_1;
    if (pHuo_1)
        btn_normal_huo_1 = Sprite::createWithTexture(pHuo_1);
    pHuo_1 = _getinidata.getinidata_one(nSprName, 1, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
    Sprite* btn_select_huo_1;
    if (pHuo_1)
        btn_select_huo_1 = Sprite::createWithTexture(pHuo_1);
    MenuItemSprite* pHuoDongSpr = MenuItemSprite::create(btn_normal_huo_1, btn_select_huo_1, NULL,
                                                         CC_CALLBACK_1(KgameWorld::huodongCallback, this));
    pHuoDongSpr->setScale(1.2);
    pHuoDongSpr->setPosition(ax::Vec2(250, -30));
    pHuoDongSpr->_setLocalZOrder(100);
    // 自动挂机
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  //\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
    g_StrLower(nSprName);
    Texture2D* pAuto_1 = _getinidata.getinidata_one(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
    Sprite* btn_normal_Auto_1;
    if (pAuto_1)
        btn_normal_Auto_1 = Sprite::createWithTexture(pAuto_1);
    pAuto_1 = _getinidata.getinidata_one(nSprName, 1, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
    Sprite* btn_select_Auto_1;
    if (pAuto_1)
        btn_select_Auto_1 = Sprite::createWithTexture(pAuto_1);

    MenuItemSprite* pAutoSpr = MenuItemSprite::create(btn_normal_Auto_1, btn_select_Auto_1, NULL,
                                                      CC_CALLBACK_1(KgameWorld::autoCallback, this));
    pAutoSpr->setScale(1.3);
    pAutoSpr->setPosition(ax::Vec2(250, -65));
    pAutoSpr->_setLocalZOrder(101);
    // 山洞地图
    sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");  //\\spr\\Ui3\\loginui\\startbtn\\log_2_start.spr"
    g_StrLower(nSprName);
    Texture2D* pCave_1 = _getinidata.getinidata_one(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
    Sprite* btn_normal_Cave_1;
    if (pCave_1)
        btn_normal_Cave_1 = Sprite::createWithTexture(pCave_1);
    pCave_1 = _getinidata.getinidata_one(nSprName, 1, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
    Sprite* btn_select_Cave_1;
    if (pCave_1)
        btn_select_Cave_1 = Sprite::createWithTexture(pCave_1);

    MenuItemSprite* pCaveSpr = MenuItemSprite::create(btn_normal_Cave_1, btn_select_Cave_1, NULL,
                                                      CC_CALLBACK_1(KgameWorld::caveCallback, this));
    pCaveSpr->setScale(2.0);
    pCaveSpr->setPosition(ax::Vec2(250, -100));
    pCaveSpr->_setLocalZOrder(102);

    // khai báo và set vị trí dãy btn dọc cạnh mini map
    pminMapMenu = Menu::create(maxMapConfirm, pHuoDongSpr, pAutoSpr, pCaveSpr, NULL);
    pminMapMenu->setPosition(ax::Vec2(visibleSize.width / 1.65, visibleSize.height - 20));  // 50
    pminMapMenu->setAnchorPoint(ax::Vec2::ZERO);
    ParentNode_minmap->addChild(pminMapMenu, 2, MINIMAP_NODE_MAXMAP);

    pPointLabel = Label::createWithTTF("0/0", "fonts/number_16_w.fnt", 10);
    // pPointLabel->setColor(nColor);
    // ax::Color3B nlineColor={0,0,255};//蓝色 //255,255,0 黄
    // pPointLabel->enableStroke(nlineColor,1);//开始描边
    char nPoinInfo[32];
    sprintf(nPoinInfo, "%d/%d", nPosX / 8, nPosY / 8);
    pPointLabel->setString(nPoinInfo);
    pPointLabel->setColor(ax::Color3B::YELLOW);
    pPointLabel->setPosition(
        ax::Vec2(visibleSize.width - nMapSize.width / 2, visibleSize.height - nMapSize.height + 5));
    ParentNode_minmap->addChild(pPointLabel, 1, MINIMAP_NODE_POINT);
    // ParentNode_minmap->setPosition(ax::Vec2(visibleSize.width-nMapSize.width-5,visibleSize.height-nMapSize.height-5));
    g_ScenePlace.SetMapParam(SCENE_PLACE_MAP_ELEM_PIC | SCENE_PLACE_MAP_ELEM_CHARACTER | SCENE_PLACE_MAP_ELEM_PARTNER,
                             (m_MapSize.cx | (m_MapSize.cy << 16)));

    // red = TintBy::create(0.2,0,-255,-255);//持续时间+颜色
    // red->retain();
    //	this->addChild(red,10,OBJ_NODE_COUNT);
    // 创建一个区域画板
    // nRegCanvas = RenderTexture::create(512,512,kTexture2DPixelFormat_RGBA8888,0);
    // RenderTexture::create(512,512);
    // nRegCanvas->setAnchorPoint(ax::Vec2(0,0));
    // nRegCanvas->retain();
    // ParentNode_map->addChild(nRegCanvas,-10);
    IR_UpdateTime();
    g_GameWorld = this;
    /*if  (joystick)
    {//设置摇杆的游戏中心指针
            joystick->setGameWorld(g_GameWorld);
    }*/
    TempUpdataImmediacy();

    auto callback1 = [this](ax::Node* sender) { KgameWorld::closeplayerfunPadCallback(sender); };

    sfunLayer            = Kuiplayerfun::create(this, callback1);
    m_playerfunPadIsOpen = true;
    ((Kuiplayerfun*)sfunLayer)->QueryAllChannel();
    this->addChild(sfunLayer, ORDER_UI_DIALOD, OBJ_NODE_PLAYFUN);
    ((Kuiplayerfun*)sfunLayer)->_removeDelegate(false);
    sfunLayer->setVisible(false);
    // 设置为附近频道
    setchatCurChannel("CH_NEARBY");
    if (chatLayer)
        chatLayer->_setPlayerFun((Kuiplayerfun*)sfunLayer);
    return true;
}

void KgameWorld::selchatlistItemEvent(Ref* pSender, ui::ListView::EventType type)
{
    /*if (type ==ui::ListView::EventType::ON_SELECTED_ITEM_START)
       messageBox("开始点击item","点击item");
    else if (type ==ui::ListView::EventType::ON_SELECTED_ITEM_END)
            messageBox("点击item结束","点击item");

    messageBox("点击item结束","点击item");*/
}
// Kỳ trận các
void KgameWorld::mQizCallback(Ref* pSender)
{
    if (m_laotouPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_LAOTOU, true);
        m_laotouPadIsOpen = false;
    }
    if (m_npcshopPadIsOpen)
    {
        m_npcshopPadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_NPCSHOP, true);
    }

    if (m_givePadIsOpen)
    {
        m_givePadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_GIVE, true);
    }

    if (m_dyshopPadIsOpen)
        this->removeChildByTag(OBJ_NODE_DYSHOP, true);

    BuySellInfo nShopInfo;
    nShopInfo.Clear();
    nShopInfo.m_nPriceType = 0;
    nShopInfo.m_SubWorldID = 0;

    auto callback = [this](ax::Node* sender) { this->closeDyshopPadCallback(sender); };

    Layer* dyshopLayer = KuiDynamicShop::create(&nShopInfo, this, callback);
    this->addChild(dyshopLayer, ORDER_UI_DIALOD, OBJ_NODE_DYSHOP);
    m_dyshopPadIsOpen = true;
}

void KgameWorld::closeDyshopPadCallback(Node* pNode)
{
    m_dyshopPadIsOpen = false;
}
// mua và bán
void KgameWorld::mPaimaiCallback(Ref* pSender) {}

void KgameWorld::mTaskCallback(Ref* pSender)
{
    if (m_taskPadIsOpen)
    {
        m_taskPadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_TASK, true);
    }

    auto callback = [this](ax::Node* sender) { this->closetaskPadCallback(sender); };

    Layer* sTaskExLayer = KuiTaskInfo::create("", this, callback);
    m_taskPadIsOpen     = true;
    this->addChild(sTaskExLayer, ORDER_UI_DIALOD, OBJ_NODE_TASK);
}

void KgameWorld::closetaskPadCallback(Node* pNode)
{
    m_taskPadIsOpen = false;
}

void KgameWorld::_quitGame()
{
    // mOptionsCallback(NULL);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    messageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    {
        if (nAcclient)
        {
            m_bIsClientConnecting = false;
            nAcclient->setBufferEmpty();
            nAcclient->Cleanup();
            nAcclient->Shutdown();
        }

        if (g_pClient)
        {
            g_pClient->setBufferEmpty();
            m_bIsPlayerInGame      = false;
            m_bIsGameSevConnecting = false;
            g_pClient->Cleanup();
            g_pClient->Shutdown();

            delete g_pClient;
            g_pClient = NULL;
        }
        setIsPaint(true);
        nPlaceId       = -1;
        nNativePlaceId = -1;
        g_ReleaseCore();  // 关闭场景中的所有数据
        // g_ScenePlace.Terminate();
        // 删除缓存
        ax::Director::getInstance()->getTextureCache()->removeUnusedTextures();
        ax::AnimationCache::getInstance()->destroyInstance();
        ax::SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
        if (__pSound)
            __pSound->end();

        ax::Director::getInstance()->replaceScene(Klogin_f::scene());
    }
    // ax::Director::getInstance()->end();

#    if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#    endif
#endif
}
// tuỳ chọn
void KgameWorld::mOptionsCallback(Ref* pSender)
{
    if (m_escPadIsOpen)
    {
        m_escPadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_ESC, true);
    }

    auto callback = [this](ax::Node* sender) { this->closeescPadCallback(sender); };

    Layer* sEscLayer = KuiEscInfo::create("", this, callback);
    m_escPadIsOpen   = true;
    this->addChild(sEscLayer, ORDER_UI_DIALOD, OBJ_NODE_ESC);
    return;
}

void KgameWorld::closeescPadCallback(Node* pNode)
{
    m_escPadIsOpen = false;
}
// Bang hội
void KgameWorld::mFactionCallback(Ref* pSender)
{

    if (m_laotouPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_LAOTOU, true);
        m_laotouPadIsOpen = false;
    }

    if (m_givePadIsOpen)
    {
        m_givePadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_GIVE, true);
    }

    if (m_tongPadIsOpen)
    {
        m_tongPadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_TONG, true);
    }

    auto callback = [this](ax::Node* sender) { this->closeTongPadCallback(sender); };

    Layer* sTongLayer = KuiTongInfo::create("", this, callback);
    m_tongPadIsOpen   = true;
    this->addChild(sTongLayer, ORDER_UI_DIALOD, OBJ_NODE_TONG);
}

void KgameWorld::closeTongPadCallback(Node* pNode)
{
    m_tongPadIsOpen = false;
}

void KgameWorld::closetradePadCallback(Node* pNode)
{
    m_tradePadIsOpen = false;
}

void KgameWorld::closeChatlistPad(Node* pNode) {}

void KgameWorld::closegivePadCallback(Node* pNode)
{
    m_givePadIsOpen = false;
}

void KgameWorld::closeLaoPadCallback(Node* pNode)
{
    m_laotouPadIsOpen = false;
}
// 队伍
void KgameWorld::mTeamCallback(Ref* pSender)
{
    if (m_teamPadIsOpen)
        return;

    auto callback = [this](ax::Node* sender) { this->closeTeamPadCallback(sender); };

    Layer* sTeamLayer = KuiTeam::create("", this, callback);
    m_teamPadIsOpen   = true;
    this->addChild(sTeamLayer, ORDER_UI_DIALOD, OBJ_NODE_TEAM);
}

void KgameWorld::closeTeamPadCallback(Node* pNode)
{
    m_teamPadIsOpen = false;
}
// mở giao diện skill
void KgameWorld::mSkillsCallback(Ref* pSender)
{  // OBJ_NODE_SKILL
    if (m_skilPadIsOpen)
        return;

    auto callback = [this](ax::Node* sender) { this->closeSkillPadCallback(sender); };

    Layer* skillLayer = Kuiskill::create(0, this, callback);
    m_skilPadIsOpen   = true;
    this->addChild(skillLayer, ORDER_UI_DIALOD, OBJ_NODE_SKILL);
}

void KgameWorld::closeItemPadCallback(Node* pNode)
{  // 在这个层中标记号，以确定是哪个按钮
    m_itemsPadIsOpen = false;
}

void KgameWorld::closeSkillPadCallback(Node* pNode)
{  // 在这个层中标记号，以确定是哪个按钮
    m_skilPadIsOpen = false;
}
// giao dịch
void KgameWorld::mItemExCallback(Ref* pSender)
{
    if (m_itemExPadIsOpen)
        return;

    auto callback = [this](ax::Node* sender) { this->closeItemExPadCallback(sender); };

    Layer* sItemExLayer = KuiItemEx::create("", this, callback);
    m_itemExPadIsOpen   = true;
    this->addChild(sItemExLayer, ORDER_UI_DIALOD, OBJ_NODE_ITEMBOX_EX);
}

void KgameWorld::closeItemExPadCallback(Node* pNode)
{
    m_itemExPadIsOpen = false;
}
// gánh nặng item
void KgameWorld::mItemsCallback(Ref* pSender)
{
    if (m_itemsPadIsOpen || m_playerSellPadIsOpen || m_givePadIsOpen)
        return;

    auto callback = [this](ax::Node* sender) { this->closeItemPadCallback(sender); };

    Layer* sItemsLayer = KuiItem::create("", this, callback);
    m_itemsPadIsOpen   = true;
    this->addChild(sItemsLayer, ORDER_UI_DIALOD, OBJ_NODE_ITEM);
}
// TÌNH TRẠNG NHÂN VẬT
void KgameWorld::mRoleStatusCallback(Ref* pSender)
{
    if (m_RoleStatePadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_STATE, true);
        // return;
    }

    if (chatLayer)
    {
        Node* nTempNode = (Node*)chatLayer;
        if (nTempNode->isVisible())
        {
            chatLayer->_removeDelegate(false);
            // nTempNode->setVisible(false);
        }
    }
    if (pcontrolMenu)
        pcontrolMenu->setEnabled(false);
    if (sortskillMenu)
        sortskillMenu->setEnabled(false);
    if (KskillMenu)
        KskillMenu->setEnabled(false);

    auto callback = [this](ax::Node* sender) { this->closeItemPadCallback(sender); };

    Layer* sRoleStateLayer = KuiRoleState::create("", this, callback);
    m_RoleStatePadIsOpen   = true;
    this->addChild(sRoleStateLayer, ORDER_UI_DIALOD, OBJ_NODE_STATE);
}

void KgameWorld::closeRolePadCallback(Node* pNode)
{
    m_RoleStatePadIsOpen = false;
    if (pcontrolMenu)
        pcontrolMenu->setEnabled(true);
    if (sortskillMenu)
        sortskillMenu->setEnabled(true);
    if (KskillMenu)
        KskillMenu->setEnabled(true);

    if (chatLayer)
    {
        Node* nTempNode = (Node*)chatLayer;
        if (nTempNode->isVisible())
        {
            chatLayer->_removeDelegate(true);
            // nTempNode->setVisible(true);
        }
    }
}

void KgameWorld::useModelCallback(Ref* pSender)
{
    if (!pSender)
        return;

    if (m_useSkillModel <= 0)
        m_useSkillModel = 1;
    else
        m_useSkillModel = 0;

    // MenuItem * nCurMenu= (MenuItem *)pSender;
    MenuItemImage* nCurMenu = (MenuItemImage*)pSender;

    if (nCurMenu)
    {
        switch (m_useSkillModel)
        {
        case 0:  // tự động
        {
            Sprite* tempSpr         = Sprite::create("attack/model/kind_z.png");
            SpriteFrame* nTempFrame = SpriteFrame::createWithTexture(tempSpr->getTexture(), tempSpr->getTextureRect());

            auto nCurMenuItem = MenuItemSprite::create(Sprite::createWithSpriteFrame(nTempFrame),
                                                       Sprite::createWithSpriteFrame(nTempFrame), nullptr);

            nCurMenu->setNormalImage(nCurMenuItem->getNormalImage());
            nCurMenu->setSelectedImage(nCurMenuItem->getSelectedImage());
        }
        break;
        case 1:  // thủ công
        {
            Sprite* tempSpr         = Sprite::create("attack/model/kind_s.png");
            SpriteFrame* nTempFrame = SpriteFrame::createWithTexture(tempSpr->getTexture(), tempSpr->getTextureRect());
            auto nCurMenuItem       = MenuItemSprite::create(Sprite::createWithSpriteFrame(nTempFrame),
                                                             Sprite::createWithSpriteFrame(nTempFrame), nullptr);

            nCurMenu->setNormalImage(nCurMenuItem->getNormalImage());
            nCurMenu->setSelectedImage(nCurMenuItem->getSelectedImage());
        }
        break;
        default:
            break;
        }
    }
}
// Sử dụng các kỹ năng tấn công
void KgameWorld::mianSkillCallback(Ref* pSender)
{  // 可以变更纹理
    // if (pMainSkillItem)
    //	pMainSkillItem->setVisible(false);//隐藏
    // messageBox("攻击 开始","攻击");

    if (!g_pCoreShell)
        return;
    if (!g_pCoreShell->GetAutoplayid())
        g_pCoreShell->MainAttack(0);

    // MenuItemImage *pMainSkillItem=(MenuItemImage *)pSender;
    // Node *_nSelNode=pMainSkillItem->getNormalSprite();
    // 开始执行一个动画
    // Animation* nCurAnimat = NULL;
    // if (actionManager && _Maniattack_State)
    //	actionManager->removeAllActionsFromTarget(_Maniattack_State);
    // animation_attack=ax::AnimationCache::getInstance()->getAnimation("main_attack");
    if (_Maniattack_State && animation_attack)
    {
        auto callFunc = ax::CallFunc::create([this]() { this->attackFinish(); });

        _Maniattack_State->runAction(Sequence::create(Animate::create(animation_attack), callFunc, NULL));
    }
}

void KgameWorld::attackFinish()
{
    // Node *_nSelNode=pMainSkillItem->getNormalSprite();
    // 开始执行一个动画
    // Animation* nCurAnimat = NULL;
    if (actionManager && _Maniattack_State)
        actionManager->removeAllActionsFromTarget(_Maniattack_State);
}
// gửi tin nhắn kênh
void KgameWorld::OnDirectSendChannelMessage(unsigned long nChannelID,
                                            BYTE cost,
                                            const char* Buffer,
                                            int nLen,
                                            int nItemDwIdx)  // 发送频道聊天到服务器
{
    if (g_pCoreShell && nChannelID != -1 && Buffer && nLen > 0)
    {
        unsigned int chatsize = sizeof(CHAT_CHANNELCHAT_CMD) + nLen;
        unsigned int pckgsize = sizeof(tagExtendProtoHeader) + chatsize;

        tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)alloca(pckgsize);  //_alloca
        pExHeader->ProtocolType         = c2s_extendchat;
        pExHeader->wLength              = pckgsize - 1;

        CHAT_CHANNELCHAT_CMD* pCccCmd = (CHAT_CHANNELCHAT_CMD*)(pExHeader + 1);
        pCccCmd->ProtocolType         = chat_channelchat;
        pCccCmd->wSize                = chatsize - 1;
        pCccCmd->packageID            = -1;
        pCccCmd->filter               = 1;

        pCccCmd->channelid = nChannelID;
        if (nChannelID == -1)
            pCccCmd->nItemDwid = 0;
        else
        {
            // unsigned long m_ItemLinkDwid=0;

            // m_ItemLinkDwid=g_pCoreShell->GetGameData(GDI_ITEM_LINKDATA,0, 0);//得到物品信息指针GDI_ITEM_LINKDATA
            pCccCmd->nItemDwid = nItemDwIdx;  // g_pCoreShell->GetGameData(GDI_ITEM_LINKDATA,0, 0);

            // char szWarning[64];
            // sprintf(szWarning, "物品dwid:[%d]!",m_ItemLinkDwid);
            // KUiMsgCentrePad::SystemMessageArrival(szWarning, strlen(szWarning) + 1);
        }

        pCccCmd->cost    = cost;
        pCccCmd->sentlen = nLen;
        memcpy(pCccCmd + 1, Buffer, nLen);
        // char szWarning[64];
        // sprintf(szWarning, "频道信息发送成功!");
        // KUiMsgCentrePad::SystemMessageArrival(szWarning, strlen(szWarning) + 1);
        if (g_pCoreShell)
            g_pCoreShell->SendNewDataToServer(pExHeader, pckgsize);
    }
}
// Thông tin trò chuyện bí mật
void KgameWorld::OnSendSomeoneMessage(const char* Name,
                                      const char* Buffer,
                                      int nLen,
                                      int nIsItemLink)  // 发送someone聊天到服务器
{
    if (g_pCoreShell && Name && Name[0] != 0 && Buffer && nLen > 0)
    {
        unsigned int chatsize = sizeof(CHAT_SOMEONECHAT_CMD) + nLen;
        unsigned int pckgsize = sizeof(tagExtendProtoHeader) + chatsize;

        tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)alloca(pckgsize);  //_alloca
        pExHeader->ProtocolType         = c2s_extendchat;
        pExHeader->wLength              = pckgsize - 1;

        CHAT_SOMEONECHAT_CMD* pCscCmd = (CHAT_SOMEONECHAT_CMD*)(pExHeader + 1);
        pCscCmd->ProtocolType         = chat_someonechat;
        pCscCmd->wSize                = chatsize - 1;
        pCscCmd->packageID            = -1;

        if (nIsItemLink)
        {
            if (g_pCoreShell)
                pCscCmd->nItemDwid = 0;  // g_pCoreShell->GetGameData(GDI_ITEM_LINKDATA,0, 0);
            else
                pCscCmd->nItemDwid = 0;
        }
        else
            pCscCmd->nItemDwid = 0;

        char nTempName[64];
        ZeroMemory(nTempName, sizeof(nTempName));
        sprintf(nTempName, "%s", Name);
        // messageBox(U2G(nTempName).c_str(),"私聊");
        sprintf(pCscCmd->someone, "%s", U2G(nTempName).c_str());
        // strcpy(pCscCmd->someone, Name);
        pCscCmd->sentlen = nLen;
        memcpy(pCscCmd + 1, Buffer, nLen);
        //----------------------------------------------------------------------
        // char szWarning[256],szWarninga[256],szWarningb[256],szWarningc[256];
        // char szName[32]={0};
        // int  nMsgLength;
        // sprintf(szName,Name);
        // TEncodeText(Buffer, nMsgLength);
        /*_snprintf(szWarning,sizeof(szWarning)+1,"向(%s)私聊(<color=yellow>%s<color>)发送成功!",Name,szWarningc);
nMsgLength = KUiFaceSelector::ConvertFaceText(szWarninga, szWarning,strlen(szWarning));  //转换表情符号
        nMsgLength = KUiItem::ConvertItemColor(szWarningb, szWarninga, nMsgLength);
    nMsgLength = TEncodeText(szWarningb, nMsgLength);//控制标记进行转换
    //int nMsgLen = TEncodeText(szWarning, strlen(szWarning));
        KUiMsgCentrePad::SystemMessageArrival(szWarningb, nMsgLength);*/
        // if (sfunLayer)
        //	((Kuiplayerfun *)sfunLayer)->ShowMSNMessage(pCscCmd->someone,Buffer, nLen,0,true);
        MSNMessageArrival(pCscCmd->someone, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, Buffer, nLen, true, 0,
                          true);
        //-----------------------------------------------------------------------
        // int nLen = strlen(Buffer);
        // char nBuffer[1536];
        // nLen = KUiFaceSelector::ConvertFaceText(nBuffer, Buffer, nLen);
        // nLen = TEncodeText(nBuffer, nLen);
        // KUiMsgCentrePad::ShowSomeoneMessage(szName, nBuffer, nLen);
        // KUiMsgCentrePad::NewMSNMessageArrival(m_pSelf->m_szSelfName,szName,Buffer,nLen,pCscCmd->nItemDwid);
        // //自己显示
        //-----------------------------------------------------------------------
        if (g_pCoreShell)
            g_pCoreShell->SendNewDataToServer(pExHeader, pckgsize);
    }
}

// thông tin kênh
void KgameWorld::OnSendChannelMessage(unsigned long nChannelID, const char* Buffer, int nLen)  // 发送频道聊天到服务器
{
    if (nChannelID != -1 && Buffer && nLen > 0)
    {
        int nLeft = -1;  //

        if (sfunLayer)
            nLeft = ((Kuiplayerfun*)sfunLayer)
                        ->PushChannelData(nChannelID, Buffer, nLen);  // 压入这个系统消息 判断发送间隔判断

        if (nLeft < 0)
        {
            char szWarning[] = "Thao tác quá nhanh";
            // KUiMsgCentrePad::SystemMessageArrival(szWarning, sizeof(szWarning));
            messageBox(szWarning, "Chú ý");
        }
        else if (nLeft > 0)
        {
            char szWarning[64] = {0};
            sprintf(szWarning, "Tin nhắn sẽ được gửi sau [%d] giây!", (nLeft + 999) / 1000);
            // KUiMsgCentrePad::SystemMessageArrival(szWarning, strlen(szWarning) + 1);
            messageBox(szWarning, "Chú ý");
        }
    }
    Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid = 0;
}

int KgameWorld::GetChannelIndex(const char* pTitle)
{
    if (pTitle)
    {
        int nChannelDataCount = 0;  // KUiMsgCentrePad::GetChannelCount();

        if (sfunLayer)
            nChannelDataCount = ((Kuiplayerfun*)sfunLayer)->GetChannelCount();

        int n = 0;  //
        if (sfunLayer)
            n = ((Kuiplayerfun*)sfunLayer)->GetChannelIndex((char*)pTitle);

        if (n >= 0 && n < nChannelDataCount)
            return n;

        n = nChannelDataCount;
        for (; n < nChannelDataCount + m_nRecentPlayerName; ++n)
        {
            if (m_RecentPlayerName[n - nChannelDataCount][0] == 0)
                break;
            if (strcmp(m_RecentPlayerName[n - nChannelDataCount], pTitle) == 0)
            {
                return n;
            }
        }
    }
    return -1;
}

void KgameWorld::InputCurrentChannel(int nIndex, bool bFocus)
{
    if (nIndex < 0)
        return;
    int nChannelDataCount = -1;  //
    if (sfunLayer)
        nChannelDataCount = ((Kuiplayerfun*)sfunLayer)->GetChannelCount();
    if (nChannelDataCount < 0)
        return;
    if (nIndex < nChannelDataCount)
    {
        // InputNameMsg(true, KUiMsgCentrePad::GetChannelTitle(nIndex), bFocus);
        // SetCurrentChannel(nIndex);
    }
    else if (nIndex < nChannelDataCount + m_nRecentPlayerName)
    {
        // InputNameMsg(false, m_RecentPlayerName[nIndex - nChannelDataCount], bFocus);
        // SetCurrentChannel(nIndex);
    }
}

int KgameWorld::AddRecentPlayer(const char* szName)
{
    if (!this)
        return -1;
    if (szName == NULL || szName[0] == 0)
        return -1;
    if (strcmp(m_szSelfName, szName) == 0)  // 自己的名字则返回
        return -1;
    int n = GetChannelIndex(szName);
    if (n >= 0)
        return n;
    n                     = 0;
    int nChannelDataCount = 0;  //
    if (sfunLayer)
        nChannelDataCount = ((Kuiplayerfun*)sfunLayer)->GetChannelCount();
    for (; n < MAX_RECENTPLAYER_COUNT; ++n)
    {
        if (m_RecentPlayerName[n][0] == 0)
        {
            strncpy(m_RecentPlayerName[n], szName, 32);
            m_nRecentPlayerName++;
            return n + nChannelDataCount;
        }
    }

    if (n >= MAX_RECENTPLAYER_COUNT)
    {
        memmove(m_RecentPlayerName[0], m_RecentPlayerName[1], sizeof(m_RecentPlayerName) - 32);
        n = MAX_RECENTPLAYER_COUNT - 1;
        strncpy(m_RecentPlayerName[n], szName, 32);
        return n + nChannelDataCount;
    }

    return -1;
}

void KgameWorld::ReplaceSpecialName(char* szDest, unsigned int nDestSize, char* szSrc)
{
    if (sfunLayer)
        ((Kuiplayerfun*)sfunLayer)->ReplaceChannelName(szDest, nDestSize, szSrc);  // 把频道的名字换成 玩家的名字
}
// gửi trò chuyện
void KgameWorld::onsendCallback(Ref* pSender)
{
    if (g_pCoreShell == NULL)
        return;
    // int  nChannelDataCount = 0;
    // if (sfunLayer)
    //	nChannelDataCount =((Kuiplayerfun*)sfunLayer)->GetChannelCount();
    // char pin[64];
    // sprintf(pin,"%d",nChannelDataCount);
    // messageBox(pin,"频道");

    char Buffer[512] = {0};
    ZeroMemory(Buffer, sizeof(Buffer));
    // char nTenmpstr[512];
    // ZeroMemory(nTenmpstr,sizeof(nTenmpstr));
    // int nMsgLength = inputEditBox.getText();  // h获取消息内容
    if (!inputEditBox || !inputEditBox->getText())
        return;
    // sprintf(nTenmpstr,inputEditBox->getText());
    std::string nTempBuffer = inputEditBox->getText();
    sprintf(Buffer, "%s", inputEditBox->getText());  // U2G(nTenmpstr).c_str()
    int nMsgLength = strlen(Buffer);
    // messageBox("发送聊天","发送聊天");
    int bChannel     = false;
    int nDestChannel = -1;
    char Name[32]    = {0};
    ZeroMemory(Name, sizeof(Name));
    int nName = 0;
    Name[0]   = 0;
    if (Buffer[nName] == TEXT_CTRL_CHAT_PREFIX || Buffer[nName] == TEXT_CTRL_CHANNEL_PREFIX)
    {  // 私聊
        bChannel = (Buffer[nName] == TEXT_CTRL_CHANNEL_PREFIX);
        while (nName < nMsgLength)
        {
            if (Buffer[nName] == ' ')
            {
                Buffer[nName] = 0;
                nName++;
                break;
            }
            nName++;
        }

        if (bChannel)
        {
            ReplaceSpecialName(Name, 31, Buffer + 1);  // 把频道的名字换成 玩家的名字?
            nDestChannel = GetChannelIndex(Name);
        }
        else
        {
            strncpy(Name, Buffer + 1, 31);
        }
        Name[31] = 0;
    }

    if (Name[0] == 0)  // 不指定名字
    {
        bChannel              = true;
        int nChannelDataCount = 0;  //
        if (sfunLayer)
            nChannelDataCount = ((Kuiplayerfun*)sfunLayer)->GetChannelCount();  // KUiMsgCentrePad::GetChannelCount();
        if (m_nCurChannel >= 0 && m_nCurChannel < nChannelDataCount)
        {
            nDestChannel = m_nCurChannel;
        }
        else
        {
            // 是附近玩家
            for (int n = 0; n < nChannelDataCount; ++n)
            {
                // if (KUiMsgCentrePad::IsChannelType(n, KUiMsgCentrePad::ch_Screen)) //全部频道
                if (sfunLayer && ((Kuiplayerfun*)sfunLayer)->IsChannelType(n, ch_Screen))
                {
                    // messageBox("查找成功","suue");
                    nDestChannel = n;
                    break;
                }
            }
        }

        if (nDestChannel >= 0)
        {
            if (sfunLayer)
                strncpy(Name, ((Kuiplayerfun*)sfunLayer)->GetChannelTitle(nDestChannel), 31);  // 取得频道的名称
        }

        Name[31] = 0;
    }
    else  // 指定名字的恢复Buffer数据
    {
        Buffer[nName - 1] = ' ';
    }

    nMsgLength -= nName;

    if (bChannel && sfunLayer &&
        ((Kuiplayerfun*)sfunLayer)->IsChannelType(nDestChannel, ch_GM))  // 在非GM频道中输入GM指令,不发送出去,以免泄密
    {
        return;  // GM频道 就返回
    }

    if (bChannel && sfunLayer &&
        ((Kuiplayerfun*)sfunLayer)->IsChannelType(nDestChannel, ch_GM))  // 在非GM频道中输入GM指令,不发送出去,以免泄密
    {
        if (nMsgLength > 3 && Buffer[nName] == '?' && (Buffer[nName + 1] == 'g' || Buffer[nName + 1] == 'G') &&
            (Buffer[nName + 2] == 'm' || Buffer[nName + 2] == 'M'))  // 前面含有GM的字母的 返回
            return;
    }
    nMsgLength = TEncodeText(Buffer, nMsgLength);  // 控制标记进行转换

    if (bChannel)
    {  // 频道聊天
        unsigned long nChannelID = -1;
        if (sfunLayer)
            nChannelID = ((Kuiplayerfun*)sfunLayer)->GetChannelID(nDestChannel);
        if (nChannelID != -1)
        {
            if (sfunLayer)
                ((Kuiplayerfun*)sfunLayer)->CheckChannel(nDestChannel, true);

            // char mgs[64];
            // sprintf(mgs,"%d:%u",nDestChannel,nChannelID);
            // messageBox(mgs,"SEND");
            OnSendChannelMessage(nChannelID, Buffer, nMsgLength);  // 计算发送频道信息 时间间隔

            // m_InputEdit.ClearText();
            inputEditBox->setText("");
            // if (nIsAutoSay)
            //	InputCurrentChannel(nDestChannel,false);
            // else
            InputCurrentChannel(nDestChannel, true);
        }
    }
    else if (!IsSelfName(Name))  // 密聊
    {
        std::string::size_type pos = nTempBuffer.find(" ");  // 空格后面为内容 前面围
        std::string _Buffer_       = nTempBuffer.substr(pos, nTempBuffer.length());
        ZeroMemory(Buffer, sizeof(Buffer));
        sprintf(Buffer, "%s", _Buffer_.c_str());
        sprintf(Buffer, "%s", U2G(Buffer).c_str());
        nMsgLength = strlen(Buffer);
        nMsgLength = TEncodeText(Buffer, nMsgLength);  // 控制标记进行转换
        // messageBox(Buffer,U2G(msg).c_str());
        // nMsgLength = TEncodeText(Buffer, nMsgLength);//控制标记进行转换
        OnSendSomeoneMessage(Name, Buffer, nMsgLength, TRUE);  // 发送密聊
        int nAdd = AddRecentPlayer(Name);
        if (nAdd >= 0)
        {
            char nTempName[64];
            ZeroMemory(nTempName, sizeof(nTempName));
            sprintf(nTempName, "%s%s%s", "/", Name, " ");
            inputEditBox->setText(nTempName);
            // m_InputEdit.ClearText();
            // if (nIsAutoSay)
            //	InputCurrentChannel(nAdd, false);
            // else
            InputCurrentChannel(nAdd, true);
        }
    }
}
// Phím tắt nhấp chuột phải gọi lại kỹ năng
void KgameWorld::auxiliaryskillCallback(Ref* pSender)
{
    if (!pSender)
        return;

    MenuItem* nCurMenu = (MenuItem*)pSender;
    int nSkillID       = nCurMenu->getTag();
    if (nSkillID > 0 && nSkillID < MAX_SKILL)
    {
        // nCurMenu->setVisible(false);

        // nCurMenu->setEnabled(false);

        /*int _Index=getIndexBySkillID(nSkillID);

        if (_Index>=0 && _Index<MAX_FUZHUSKILL_COUNT)
        {
                if (skillstick[_Index].skillstick)
                {
                        if (skillstick[_Index].controlSprite_s)
                           skillstick[_Index].controlSprite_s->setOpacity(255);
                        if (skillstick[_Index].ygSprite_s)
                                skillstick[_Index].ygSprite_s->setOpacity(255);
                }
               //skillstick->setSpritePiont(nCurMenu->getPosition());
               //skillstick->setSpriteVisible(true);
        }
        if  (skillstick)
        {
                skillstick->setSpritePiont(nCurMenu->getPosition());
                //skillstick->setSpriteVisible(true);
        }
        */
        ISkill* pOrdinSkill = g_SkillManager.GetSkill(nSkillID, 1);
        if (pOrdinSkill && !pOrdinSkill->IsAura())
        {  // Sử dụng nó nếu nó không phải là một kỹ năng hào quang
            int ms_MouseX = 0, ms_MouseY = 0;
            bool nIsRun = false;
            nIsRun      = joystick->getIsRun();
            if (nIsRun)
            {  // Nếu thanh thắt lưng đang di chuyển tọa độ sau khi j kết thúc
                Point nTempPos;
                nTempPos  = joystick->getMoveEndPos();
                ms_MouseX = nTempPos.x;
                ms_MouseY = nTempPos.y;
            }
            else
            {
                ms_MouseX = screenPoint.x;
                ms_MouseY = screenPoint.y;
            }
            if (g_pCoreShell)  // 自动找怪打
            {
                Player[CLIENT_PLAYER_INDEX].SetRightSkill(nSkillID);
                // g_pCoreShell->MainAttack(1); //只针对敌人
                g_pCoreShell->UseSkill(ms_MouseX, ms_MouseY, nSkillID, 1);
            }
        }
        else
        {
            if (pOrdinSkill && pOrdinSkill->IsAura())
            {
                Npc[m_NpcIndex].SetAuraSkill(0);
                Npc[m_NpcIndex].SetAuraSkill(nSkillID);
            }
        }

        // char msg[64];
        // sprintf(msg,"skillidx:%d",nSkillID);
        // messageBox(msg,"按钮执行成功");
    }
}

// tấn công thủ công
void KgameWorld::customSkillAttack(int nSkillid, float xPos, float yPos)
{
    if (nSkillid > 0 && nSkillid < MAX_SKILL)
    {
        ISkill* pOrdinSkill = g_SkillManager.GetSkill(nSkillid, 1);
        if (pOrdinSkill && !pOrdinSkill->IsAura())
        {                      // 不是光环技能就使用
            if (g_pCoreShell)  // 自动找怪打
            {
                Player[CLIENT_PLAYER_INDEX].SetRightSkill(nSkillid);
                // g_pCoreShell->MainAttack(1); //只针对敌人
                g_pCoreShell->UseSkill((int)xPos, (int)yPos, nSkillid, -1);
            }
        }
        else
        {
            if (pOrdinSkill && pOrdinSkill->IsAura())
            {
                Npc[m_NpcIndex].SetAuraSkill(0);
                Npc[m_NpcIndex].SetAuraSkill(nSkillid);
            }
        }
    }
}

void KgameWorld::playerSellCallback(Ref* pSender)
{
    if (m_itemsPadIsOpen || m_playerSellPadIsOpen)
        return;

    auto callback = [this](ax::Node* sender) { this->closesellItemPadCallback(sender); };

    Layer* sItemsLayer    = KuiSellItem::create(this, callback);
    m_playerSellPadIsOpen = true;
    this->addChild(sItemsLayer, ORDER_UI_DIALOD, OBJ_NODE_ITEM_SELL);
}

void KgameWorld::closesellItemPadCallback(Node* pNode)
{
    m_playerSellPadIsOpen = false;
}

// Chức năng người chơi
void KgameWorld::playerfunCallback(Ref* pSender)
{
    if (m_playerfunPadIsOpen && sfunLayer)
    {
        /*if  (((Kuiplayerfun*)sfunLayer)->isVisible())
        {
                ((Kuiplayerfun*)sfunLayer)->setVisible(false);
                ((Kuiplayerfun*)sfunLayer)->_removeDelegate(false);
        }
        else
        {
                ((Kuiplayerfun*)sfunLayer)->setVisible(true);
                ((Kuiplayerfun*)sfunLayer)->_removeDelegate(true);
        }
        */
        if (sfunLayer->isVisible())
        {
            ((Kuiplayerfun*)sfunLayer)->_removeDelegate(false);
            sfunLayer->setVisible(false);
        }
        else
        {
            ((Kuiplayerfun*)sfunLayer)->_removeDelegate(true);
            sfunLayer->setVisible(true);

            // if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFigure>=0)
            //    ((Kuiplayerfun *)sfunLayer)->QueryAllChannel();
        }
        return;
    }

    auto callback = [this](ax::Node* sender) { this->closeplayerfunPadCallback(sender); };

    sfunLayer            = Kuiplayerfun::create(this, callback);
    m_playerfunPadIsOpen = true;
    this->addChild(sfunLayer, ORDER_UI_DIALOD, OBJ_NODE_PLAYFUN);
    ((Kuiplayerfun*)sfunLayer)->QueryAllChannel();
}

void KgameWorld::closeplayerfunPadCallback(Node* pNode)
{  // 关闭的时候再次隐藏
    // m_playerfunPadIsOpen =false;
    if (m_playerfunPadIsOpen && sfunLayer)
    {
        ((Kuiplayerfun*)sfunLayer)->_removeDelegate(false);
        ((Kuiplayerfun*)sfunLayer)->setVisible(false);
    }
}

void KgameWorld::delmapCallback(Ref* pSender)
{
    if (g_ScenePlace.GetClearMap())
        g_ScenePlace.SetClearMap(false);
    else
        g_ScenePlace.SetClearMap(true);

    ax::Director::getInstance()->getTextureCache()->removeUnusedTextures();
    ax::AnimationCache::getInstance()->destroyInstance();
    ax::SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    /// ax::Director::getInstance()->getTextureCache()->dumpCachedTextureInfo();
    if (__pSound)
        __pSound->end();

    // ccHideTitleBar(true);
}

void KgameWorld::mianYinCallback(Ref* pSender)
{
    if (ParentNode_item)
    {
        isClose = false;
        TempUpdataImmediacy();

        m_ImmediaItemSelIndex = -1;

        Sprite* nTempSpr = (Sprite*)ParentNode_item->getChildByTag(2);
        if (nTempSpr)
        {
            if (nTempSpr->isVisible())
                nTempSpr->setVisible(false);
            else
                nTempSpr->setVisible(true);
        }
        nTempSpr = NULL;
        nTempSpr = (Sprite*)ParentNode_item->getChildByTag(3);
        if (nTempSpr)
        {
            if (nTempSpr->isVisible())
                nTempSpr->setVisible(false);
            else
                nTempSpr->setVisible(true);
        }
    }

    if (!pcontrolMenu)
        return;

    char mKey[32];
    sprintf(mKey, "quxia");
    std::string nikey         = mKey;
    MenuItemImage* pquxiaItem = (MenuItemImage*)pcontrolMenu->getChildByName(nikey);
    if (pquxiaItem)
    {
        if (pquxiaItem->isVisible())
            pquxiaItem->setVisible(false);
        else
            pquxiaItem->setVisible(true);
    }
}
// dịch chuyển bản đồ hang động
void KgameWorld::caveCallback(Ref* pSender)
{
    if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
        return;
    Layer* caveLayer = NULL;
    if (m_cavePadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_MOVECAVE, true);
        m_cavePadIsOpen = false;
        return;
        // caveLayer = (Layer *)this->getChildByTag(OBJ_NODE_MOVECAVE);
    }
    auto callback = [this](ax::Node* sender) { this->cavePadCallback(sender); };

    caveLayer = KuiMoveToMap::create("Move to Map", this, callback);
    this->addChild(caveLayer, ORDER_UI_DIALOD, OBJ_NODE_MOVECAVE);
    m_cavePadIsOpen = true;
}

void KgameWorld::cavePadCallback(Node* pNode)
{
    m_cavePadIsOpen = false;
}

// Auto
void KgameWorld::autoCallback(Ref* pSender)
{
    if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
        return;

    Layer* autoLayer = NULL;
    if (m_autoPadIsOpen)
    {
        // this->removeChildByTag(OBJ_NODE_AUTOPLAY,true);
        autoLayer = (Layer*)this->getChildByTag(OBJ_NODE_AUTOPLAY);
        if (autoLayer)
        {
            if (autoLayer->isVisible())
            {
                if (_clientlanguage == 1)
                    ((KuiAutoPlay*)autoLayer)->_removeDelegate(false);
                else
                    ((KuiAutoPlay_vn*)autoLayer)->_removeDelegate(false);
                autoLayer->setVisible(false);
            }
            else
            {
                if (_clientlanguage == 1)
                    ((KuiAutoPlay*)autoLayer)->_removeDelegate(true);
                else
                    ((KuiAutoPlay_vn*)autoLayer)->_removeDelegate(false);

                autoLayer->setVisible(true);
            }
            return;
        }
    }

    auto callback = [this](ax::Node* sender) { this->autoPadCallback(sender); };

    if (_clientlanguage == 1)
    {
        autoLayer = KuiAutoPlay::create("", this, callback);
    }
    else
    {
        autoLayer = KuiAutoPlay_vn::create("", this, callback);
    }

    this->addChild(autoLayer, ORDER_UI_DIALOD, OBJ_NODE_AUTOPLAY);
    m_autoPadIsOpen = true;
}

void KgameWorld::autoPadCallback(Node* pNode)
{
    m_autoPadIsOpen = false;
}

// Hướng dẫn hoạt động Bản đồ được đề xuất
void KgameWorld::huodongCallback(Ref* pSender)
{
    if (m_NpcDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_NPCDIALOG, true);
        m_NpcDialogPadIsOpen = false;
    }
    if (m_talkDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_TALKDIALOG, true);
        m_talkDialogPadIsOpen = false;
    }

    if (m_storeboxPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_STORE, true);
        m_storeboxPadIsOpen = false;
    }

    if (m_movemapPadIsOpen)
        this->removeChildByTag(OBJ_NODE_MOVEMAP, true);

    if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
        return;

    if (chatLayer)
    {
        Node* nTempNode = (Node*)chatLayer;
        if (nTempNode->isVisible())
        {
            chatLayer->_removeDelegate(false);
            // nTempNode->setVisible(false);
        }
    }
    if (pcontrolMenu)
        pcontrolMenu->setEnabled(false);
    if (sortskillMenu)
        sortskillMenu->setEnabled(false);
    if (KskillMenu)
        KskillMenu->setEnabled(false);

    auto callback = [this](ax::Node* sender) { this->closemapPadCallback(sender); };

    Layer* moveDialogLayer = KuiCometoMap::create(this, callback);
    this->addChild(moveDialogLayer, ORDER_UI_DIALOD, OBJ_NODE_MOVEMAP);
    m_movemapPadIsOpen = true;
}

void KgameWorld::closemapPadCallback(Node* pNode)
{
    m_movemapPadIsOpen = false;

    if (pcontrolMenu)
        pcontrolMenu->setEnabled(true);
    if (sortskillMenu)
        sortskillMenu->setEnabled(true);
    if (KskillMenu)
        KskillMenu->setEnabled(true);

    if (chatLayer)
    {
        Node* nTempNode = (Node*)chatLayer;
        if (nTempNode->isVisible())
        {
            chatLayer->_removeDelegate(true);
            // nTempNode->setVisible(true);
        }
    }
}

// 右键快捷技能
void KgameWorld::mianSortSillCallback(Ref* pSender)
{
    if (!pSender)
        return;

    if (m_sortskilPadIsOpen_l)
        this->removeChildByTag(OBJ_NODE_SHORTKILL_L, true);

    if (m_sortskilPadIsOpen_r)
        this->removeChildByTag(OBJ_NODE_SHORTKILL_R, true);

    int nKey = ((MenuItemImage*)pSender)->getTag();
    Layer* sortLayer;

    if (nKey == 1)
    {  // 左键
       // messageBox("左技能","test");
        auto callback = [this](ax::Node* sender) { this->closesortskillPad_l(sender); };

        sortLayer             = KuiSortSkill_l::create(1, this, callback);
        m_sortskilPadIsOpen_l = true;
        this->addChild(sortLayer, ORDER_UI_DIALOD, OBJ_NODE_SHORTKILL_L);
        return;
    }

    auto callback = [this](ax::Node* sender) { this->closesortskillPad_r(sender); };

    sortLayer             = KuiSortSkill_r::create(1, this, callback);
    m_sortskilPadIsOpen_r = true;
    this->addChild(sortLayer, ORDER_UI_DIALOD, OBJ_NODE_SHORTKILL_R);
}

void KgameWorld::closesortskillPad_l(Node* pNode)
{
    m_sortskilPadIsOpen_l = false;
}
void KgameWorld::closesortskillPad_r(Node* pNode)
{
    m_sortskilPadIsOpen_r = false;
}

// Nút xóa mục trên thanh nóng
void KgameWorld::mianQuCallback(Ref* pSender)
{
    isClose  = true;
    int nidx = getImmediacyBoxindex(m_ImmediaItemSelIndex);
    if (nidx < 0)
    {
#ifdef WIN32
        messageBox("请选择一个物品", "提示:");
#else
        messageBox(UTEXT("请选择一个物品", 1).c_str(), UTEXT("提示:", 1).c_str());
#endif
        return;
    }
    KUiObjAtContRegion Obj;
    Obj.Obj.uGenre    = CGOG_ITEM;
    Obj.Obj.uId       = m_ImmediaItemSelIndex;
    Obj.Region.h      = nidx;
    Obj.Region.v      = 0;
    Obj.Region.Width  = Item[m_ImmediaItemSelIndex].GetWidth();
    Obj.Region.Height = Item[m_ImmediaItemSelIndex].GetHeight();
    Obj.eContainer    = UOC_IMMEDIA_ITEM;
    if (g_pCoreShell)
        g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT, (intptr_t)(&Obj), room_equipment);

    m_ImmediaItemSelIndex = -1;
}

// 交易
void KgameWorld::mianExcCallback(Ref* pSender)
{
    if (g_pCoreShell)
    {
        g_pCoreShell->OperationRequest(GOI_TRADE_WILLING, 0, true);
    }
}
// PK
void KgameWorld::mianPkCallback(Ref* pSender)
{
    int nPk = g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0);
    if (!nPk)
        nPk = 1;  // 切磋状态
    else if (nPk == 1)
        nPk = 2;  // 屠杀状态
    else
        nPk = 0;  // 正常状态

    g_pCoreShell->OperationRequest(GOI_PK_SETTING, 0, nPk);
}

void KgameWorld::setInputMsg(char* strInfo)
{
    if (!strInfo || !inputEditBox)
        return;
    char nTempSerInfo[256];
    ZeroMemory(nTempSerInfo, sizeof(nTempSerInfo));
    sprintf(nTempSerInfo, "%s%s", inputEditBox->getText(), UTEXT(strInfo, 1).c_str());
    inputEditBox->setText(nTempSerInfo);
}

void KgameWorld::setchatInputText(char* text)
{
    if (!text || !inputEditBox)
        return;

    inputEditBox->setText(text);
}

void KgameWorld::setPadCanMove(int _PadIndex, bool ver)
{
    switch (_PadIndex)
    {
    case PAD_KIND_ITEM:
    {
        if (m_itemsPadIsOpen)
        {
            KuiItem* ItemPad = NULL;
            ItemPad          = (KuiItem*)this->getChildByTag(OBJ_NODE_ITEM);
            if (ItemPad)
                ItemPad->setCanMove(ver);
        }

        if (m_RoleStatePadIsOpen)
        {
            KuiRoleState* ItemPad = NULL;
            ItemPad               = (KuiRoleState*)this->getChildByTag(OBJ_NODE_STATE);
            if (ItemPad)
                ItemPad->setCanMove(ver);
        }

        if (m_itemExPadIsOpen)
        {
            KuiItemEx* ItemPad = NULL;
            ItemPad            = (KuiItemEx*)this->getChildByTag(OBJ_NODE_ITEMBOX_EX);
            if (ItemPad)
                ItemPad->setCanMove(ver);
        }

        if (m_teamPadIsOpen)
        {
            KuiTeam* ItemPad = NULL;
            ItemPad          = (KuiTeam*)this->getChildByTag(OBJ_NODE_TEAM);
            if (ItemPad)
                ItemPad->setCanMove(ver);
        }
        if (m_playerfunPadIsOpen)
        {
            Kuiplayerfun* ItemPad = NULL;
            ItemPad               = (Kuiplayerfun*)this->getChildByTag(OBJ_NODE_PLAYFUN);
            if (ItemPad)
                ItemPad->setCanMove(ver);
        }

        if (m_playerSellPadIsOpen)
        {
            KuiSellItem* ItemPad = NULL;
            ItemPad              = (KuiSellItem*)this->getChildByTag(OBJ_NODE_ITEM_SELL);
            if (ItemPad)
                ItemPad->setCanMove(ver);
        }
    }
    break;
    default:
        break;
    }
}

void KgameWorld::setchatCurChannel(const char* name)
{
    if (!name)
        return;
    /*
    Channel0=CH_NEARBY
    Channel1=CH_TEAM
    Channel2=CH_WORLD
    Channel3=CH_FACTION
    Channel4=CH_TONG
    Channel5=CH_SYSTEM
    Channel6=CH_CITY
    Channel7=CH_MSGR
    Channel8=CH_MAIMAI
    ;Channel8=CH_CHATROOM
    Channel9=CH_ATTACK
    Channel10=CH_DEFEND
    ;Channel11=CH_JABBER
    Channel11=CH_SONG
    Channel12=CH_JIN
    ;Channel14=CH_CUSTOM
    DefaultChannel=CH_SYSTEM
    */
    m_nCurChannel = GetChannelIndex(name);

    if (ChannelSpr)
    {  // 开始变更纹理
        Label* tempLabel = Label::createWithTTF("", UI_GAME_FONT_DEFAULT_VN_BTN, 15, ChannelSpr->getContentSize(),
                                                TextHAlignment::CENTER);
        if (strstr(name, "CH_NEARBY"))
        {  // 附近

            tempLabel->setString("Cận");
        }
        else if (strstr(name, "CH_TEAM"))
        {  // 队伍
            tempLabel->setString("Đội");
        }
        else if (strstr(name, "CH_WORLD"))
        {  // 世界
            tempLabel->setString("Tổng");
        }
        else if (strstr(name, "CH_CITY"))
        {  // 城市
            tempLabel->setString("Thành");
        }
        else if (strstr(name, "CH_TONG"))
        {  //
            tempLabel->setString("Bang");
        }
        else if (strstr(name, "CH_FACTION"))
        {  //
            tempLabel->setString("Phái");
        }
        else if (strstr(name, "CH_JABBER"))
        {
            tempLabel->setString("Chọn");
        }
        tempLabel->setAnchorPoint(ax::Vec2(0, 0));
        tempLabel->setScale(0.8);
        tempLabel->setAnchorPoint(ax::Vec2(0, 0));
        tempLabel->setPosition(ax::Vec2(4, 0));
        ChannelSpr->setScale(0.6);
        ChannelSpr->setPosition(ax::Vec2(-ChannelSpr->getContentSize().width / 2, 8));
        ChannelSpr->addChild(tempLabel);
    }
}

void KgameWorld::setchatDelegate(bool isYes)
{
    if (chatLayer)
    {
        Node* nTempNode = (Node*)chatLayer;
        chatLayer->_removeDelegate(isYes);
    }
}
// mở bản đồ
void KgameWorld::maxmapCallBackFunc(Ref* pSender)
{
    if (m_maxMapPadIsOpen)
    {
        m_maxMapPadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_MAXMAP, true);
        // if  (chatLayer)
        //	 chatLayer->_removeDelegate(true);
        // hidechatCallback(NULL);
        return;
    }

    // if  (chatLayer)
    //	chatLayer->_removeDelegate(false);

    // hidechatCallback(NULL);
    // 开始创建大地图

    auto callback = [this](ax::Node* sender) { this->closeMaxMapPadFunc(sender); };

    Layer* maxmapLayer = KuiMaxMap::create("", this, callback);
    this->addChild(maxmapLayer, ORDER_UI_DIALOD, OBJ_NODE_MAXMAP);
    m_maxMapPadIsOpen = true;
}

void KgameWorld::closeMaxMapPadFunc(Node* pNode)
{
    m_maxMapPadIsOpen = false;
    // if  (chatLayer)
    //	chatLayer->_removeDelegate(true);
    // hidechatCallback(NULL);
}

void KgameWorld::hidechatCallback(Ref* pSender)
{
    // if  (m_maxMapPadIsOpen) return;

    if (chatLayer)
    {
        // static_cast<Node*>chatLayer->
        Node* nTempNode = (Node*)chatLayer;
        if (nTempNode->isVisible())
        {
            chatLayer->_removeDelegate(false);
            nTempNode->setVisible(false);
        }
        else
        {
            chatLayer->_removeDelegate(true);
            nTempNode->setVisible(true);
        }
    }
    else
    {  // 创建
        auto callback = [this](ax::Node* sender) { this->closeChatlistPad(sender); };

        chatLayer = KuiChatList::create(this, callback);
        ParentNode_chat->addChild(chatLayer, ORDER_UI_DIALOD, CHAT_NODE_CHATLIST);
        if (chatLayer)
            chatLayer->_setPlayerFun((Kuiplayerfun*)sfunLayer);
    }
}

void KgameWorld::pickCallback(Ref* pSender)
{
    if (g_pCoreShell)
        g_pCoreShell->clientPickItem();
}

// 走/跑
void KgameWorld::mianRunCallback(Ref* pSender)
{
    if (g_pCoreShell)
        g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, PA_RUN, 0);
}
// 打坐/站立
void KgameWorld::mianSitCallback(Ref* pSender)
{
    if (g_pCoreShell)
        g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, PA_SIT, 0);
}
// 上下马
void KgameWorld::mianHorseCallback(Ref* pSender)
{
    if (g_pCoreShell)
        g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, PA_RIDE, 0);
}

// 循环更新虚函数 默认自动调用
void KgameWorld::update(float delta)
{
    if (!nisgetinfo)
        _quitGame();

    unsigned int nSize  = 0;
    const char* pBuffer = NULL;

    /*if (m_bIsClientConnecting)
    { //账号服务器
            while (true)
            {
                    if (!nAcclient)
                            break;
                    //无限循环从服务器获取数据小包
                    nSize = 0;
                    pBuffer = NULL;
                    pBuffer = (const char*)nAcclient->GetPackFromServer(nSize);
                    if (!(pBuffer && nSize))
                            break;
                    PROTOCOL_MSG_TYPE*	pMsg = (PROTOCOL_MSG_TYPE*)pBuffer;
                    PROTOCOL_MSG_TYPE	Msg  = *pMsg;  //协议头
                    //CCAssert(Msg > s2c_multiserverbegin || Msg < s2c_end,"");
                    if  (Msg == s2c_logiclogout)
                    {
                            //AcceptNetMsg(pMsg);
                            messageBox("eixtGame:out netWork","eixtGame");
                            break;
                    }
            }
    }
    */
    if (/*m_bIsGameSevConnecting && */ g_pClient)
    {
        nSize   = 0;
        pBuffer = NULL;
        while (true)
        {
            if (!g_pClient)  // GS的数据包
                break;
            // 无限循环从服务器获取数据小包
            nSize   = 0;
            pBuffer = NULL;

            pBuffer = (const char*)g_pClient->GetPackFromServer(nSize);

            if (!(pBuffer && nSize))
                break;

            std::string str(pBuffer);

            PROTOCOL_MSG_TYPE* pMsg = (PROTOCOL_MSG_TYPE*)pBuffer;  // 当前包

            while (pMsg < (PROTOCOL_MSG_TYPE*)(pBuffer + nSize))
            {  // 帅选数据包
                PROTOCOL_MSG_TYPE Msg = *pMsg;
                // 跨服务器的协议
                if (Msg == s2c_notifyplayerexchange)
                {  // 转换地图或转服
                    // ProcessSwitchGameSvrMsg(pMsg);
                    break;
                }
                else if (g_pCoreShell)
                {
                    if (Msg <= s2c_clientbegin || Msg >= s2c_end || g_pCoreShell->GetProtocolSize(Msg) == 0)
                        break;

                    g_pCoreShell->NetMsgCallbackFunc(pMsg);  // 接受从服务器发来的协议消息

                    if (g_pCoreShell->GetProtocolSize(Msg) > 0)
                        pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + g_pCoreShell->GetProtocolSize(Msg));
                    else  // 没有限制长度的 -1
                        pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + PROTOCOL_MSG_SIZE +
                                                    (*(unsigned short*)(((char*)pMsg) + PROTOCOL_MSG_SIZE)));
                }
            }
        }
    }  // end 网络循环

    if (g_GameWorld /*&& m_bIsPlayerInGame*/)
    {                      // 如果 有子节点
        if (g_pCoreShell)  // 游戏主循环
        {
            if (nLoopCount > 2000000)
            {  // 重起 计时器
                nLoopCount = 0;
                timerStart.Passed(1);
            }
            if (nLoopCount * 1000 <= timerStart.GetElapse() * 18)
            {
                g_pCoreShell->Breathe();
                ++nLoopCount;  // 每秒增加 18帧
            }
            if (nLoopCount * 1000 >= timerStart.GetElapse() * 18)
            {
#ifdef WIN32
                Sleep(1);  // 太快就暂停一秒
#else
                // sleep(1000);
#endif
            }
            else if ((nLoopCount >> 3))  // 每半帧就停
            {                            // 否则停止绘画游戏世界，就卡住
#ifdef WIN32
                Sleep(1);  // 太快就暂停一秒
#else
               // sleep(1000);
#endif
            }
        }
    }
}

void KgameWorld::draw(Renderer* renderer, const Mat4& transform, unsigned int flags)
{
    if (!g_GameWorld || !g_pClient || !m_bIsPlayerInGame)
    {
        // messageBox("draw error","test draw");
        return;
    }
    /*unsigned long dwTimer;
    dwTimer=g_SubWorldSet.getCilentRunTime();
    dwTimer -= __pingTime;
    dwTimer >>= 1;

    if (dwTimer-g_SubWorldSet.GetPing()>=12000) //断线了
            g_pClient->setSocketStates(false);
    else
            g_pClient->setSocketStates(true);
    */
    // 网络状态的显示
    if (!g_ScenePlace.getIsPaint())
    {  //
        pNetStateLabel->setVisible(true);
        if (!isHave)
        {  // 0--500 500-2000 2000>
            // isHave = true;
            sprintf(_strPing, "Ping:%u %d", g_SubWorldSet.GetPing(), g_pClient->getSocketStates());
            pNetStateLabel->setString(UTEXT(_strPing, 1).c_str());
        }
    }
    else
    {
        isHave = false;
        // if (pNetStateLabel->isVisible())
        //	pNetStateLabel->setVisible(false);
        sprintf(_strPing, "ping:%u %d", g_SubWorldSet.GetPing(), g_pClient->getSocketStates());
        pNetStateLabel->setString(_strPing);
    }

    if (g_GameWorld && m_bIsPlayerInGame)
    {
        m_NpcIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
        if (m_NpcIndex > 0 && m_NpcIndex < MAX_NPC)
        {  // 设置摇杆的主角索引
            if (!g_pClient->getSocketStates())
            {  // 如果是短线的了
                if (m_recontnetTime > 0)
                    m_recontnetTime--;

                if (m_recontnetTime <= 0)
                {
                    m_recontnetTime = RECONTNET_TIME;
                    // Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(UTEXT("掉线了,正在帮你重新连接...",1).c_str());
                }
            }
            else
                m_recontnetTime = RECONTNET_TIME;

            if (joystick)
                joystick->setRoleIndex(m_NpcIndex);

            if (KskillMenu)
                KskillMenu->setRoleIndex(m_NpcIndex);
            // if (!pRoleNameLabel->getString())
            char msg[64], tempName[64];
            sprintf(tempName, "%s", Npc[m_NpcIndex].Name);
            sprintf(m_szSelfName, "%s", Npc[m_NpcIndex].Name);

            sprintf(msg, "%s %d%s", "", Npc[m_NpcIndex].m_Level, "");
            pRoleNameLabel->setString(msg);
            float nTempScalX     = 0.0;
            Sprite* nTempSprite  = NULL;
            float nCurLife       = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife;
            float nCurMaxLife    = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax;
            float nCurmana       = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana;
            float nCurMaxmana    = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax;
            float nCursaitama    = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina;
            float nCurMaxsaitama = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStaminaMax;
            float nCurExp        = Player[CLIENT_PLAYER_INDEX].m_nExp;
            float nCurMaxExp     = Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp;
            int nRankPlayer      = Player[CLIENT_PLAYER_INDEX].m_nWorldStat;
            char nkey[32];
            if (plifeLabel)
            {
                sprintf(nkey, "%d/%d", Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife,
                        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax);
                plifeLabel->setString(nkey);
            }

            if (pmanaLabel)
            {
                sprintf(nkey, "%d/%d", Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana,
                        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax);
                pmanaLabel->setString(nkey);
            }
            if (pSaitamaLabel)
            {
                sprintf(nkey, "%d/%d", Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina,
                        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStaminaMax);
                pSaitamaLabel->setString(nkey);
            }
            if (pNetStateLabel)
            {
                sprintf(nkey, "%d", nRankPlayer);
                pNetStateLabel->setString(nkey);
            }

            nTempSprite = (Sprite*)ParentNode_rolestate->getChildByTag(ROLESTATE_NODE_BGSPR);
            if (nTempSprite)
            {
                Sprite* nTempSprite_1 = (Sprite*)nTempSprite->getChildByTag(ROLESTATE_NODE_LIFE);
                if (nTempSprite_1)
                {
                    nTempScalX = nCurLife / nCurMaxLife;
                    nTempSprite_1->setScaleX(nTempScalX);
                }
            }

            nTempSprite = (Sprite*)ParentNode_rolestate->getChildByTag(ROLESTATE_NODE_BGSPR);
            if (nTempSprite)
            {
                Sprite* nTempSprite_1 = (Sprite*)nTempSprite->getChildByTag(ROLESTATE_NODE_LAN);
                if (nTempSprite_1)
                {
                    nTempScalX = nCurmana / nCurMaxmana;
                    nTempSprite_1->setScaleX(nTempScalX);
                }
            }

            nTempSprite = (Sprite*)ParentNode_rolestate->getChildByTag(ROLESTATE_NODE_BGSPR);
            if (nTempSprite)
            {
                Sprite* nTempSprite_1 = (Sprite*)nTempSprite->getChildByTag(ROLESTATE_NODE_SAITAMA);
                if (nTempSprite_1)
                {
                    nTempScalX = nCursaitama / nCurMaxsaitama;
                    nTempSprite_1->setScaleX(nTempScalX);
                }
            }

            nTempSprite = (Sprite*)ParentNode_chat->getChildByTag(CHAT_NODE_TILI);
            if (nTempSprite)
            {
                bool isFu = false;
                if (nCurExp < 0)
                {
                    isFu    = true;
                    nCurExp = -nCurExp;
                }

                nTempScalX = nCurExp / nCurMaxExp;

                if (nTempScalX > 1)
                    nTempScalX = 1;
                nTempSprite->setScaleX(nTempScalX);

                if (isFu)
                    nTempSprite->setColor(ax::Color3B::MAGENTA);
                else
                    nTempSprite->setColor(ax::Color3B::WHITE);
                /*if (pExpLabel)
                {
                        char nTempStr[64];
                        sprintf(nTempStr,"%d/%d %d%s",(int)nCurExp,(int)nCurMaxExp,100*nTempScalX,"%%");
                        pExpLabel->setString(nTempStr);
                }*/
            }
            // if (Npc[m_NpcIndex].m_SubWorldIndex>=0 && Npc[m_NpcIndex].m_RegionIndex>=0)
            //   g_ScenePlace.Paint();
        }
        g_ScenePlace.GetSceneNameAndFocus(nMapName, nMapidx, nPosX, nPosY, nMapKind);
        ax::Color3B nlineColor = {0, 0, 0};  // 蓝色 //255,255,0 黄
        pMapNameLabel->setString(UTEXT(nMapName, 1).c_str());
        char nPoinInfo[32];
        sprintf(nPoinInfo, "%d/%d", nPosX / 8, nPosY / 8);
        pPointLabel->setColor(ax::Color3B::YELLOW);
        pPointLabel->setString(nPoinInfo);

        char ntesInfo[32];
        sprintf(ntesInfo, "%d", g_ScenePlace.getObjsAboveCount());
        ptestLabel->setColor(ax::Color3B::RED);
        ptestLabel->setString(ntesInfo);
    }

    // 场景的绘画
    g_ScenePlace.Paint();
    g_ScenePlace.PaintMap(nMapStartLeft, nMapStartTop);
    if (ParentNode_npc && ParentNode_npc->getChildrenCount())
    {
        Ref* temp          = NULL;
        auto nodeArray_Npc = ParentNode_npc->getChildren();  // 获取父节点下的 所有子节点的数组
        // CCARRAY_FOREACH(nodeArray_Npc,temp)
        for (auto it = nodeArray_Npc.rbegin(); it != nodeArray_Npc.rend(); ++it)  // 逆向
        {
            temp = *it;
            if (temp)
            {
                int nNpcIndex = ((Node*)temp)->getTag();
                if (nNpcIndex > 0 && nNpcIndex < MAX_NPC)
                {
                    Npc[nNpcIndex].PaintBlood(0);  // 绘制冒血
                    Npc[nNpcIndex].PaintOther();
                    Npc[nNpcIndex].Paint();
                }
            }
        }
    }  // end ParentNode_npc

    if (ParentNode_miss->getChildrenCount())
    {  // ParentNode_miss
        Ref* temp           = NULL;
        auto nodeArray_Miss = ParentNode_miss->getChildren();  // 获取父节点下的 所有子节点的数组
        // CCARRAY_FOREACH(nodeArray_Miss,temp)
        for (auto it = nodeArray_Miss.rbegin(); it != nodeArray_Miss.rend(); ++it)  // 逆向
        {
            temp = *it;
            if (temp)
            {
                int nMissindex = ((Node*)temp)->getTag();
                if (nMissindex > 0 && nMissindex < MAX_MISSLE)
                {
                    Missle[nMissindex].Paint();
                }
            }
        }
    }  // end ParentNode_miss

    if (ParentNode_obj->getChildrenCount())
    {  // end ParentNode_obj
        Ref* temp          = NULL;
        auto nodeArray_Obj = ParentNode_obj->getChildren();  // 获取父节点下的 所有子节点的数组
                                                             // CCARRAY_FOREACH(nodeArray_Obj,temp)//正向
        for (auto it = nodeArray_Obj.rbegin(); it != nodeArray_Obj.rend(); ++it)  // 逆向
        {
            temp = *it;
            if (temp)
            {
                int Objindex = ((Node*)temp)->getTag();
                //[TODO]
                //				if  (Objindex>0 && Objindex<MAX_OBJECT)
                //					Object[Objindex].Draw();
            }
        }
    }  // end ParentNode_obj
    // 聊天的循环
    if (!sfunLayer)
        return;
    int nUseMana   = 0;
    int nUseMoney  = 0;
    int nUseLevel  = 0;
    int nItemDwIdx = 0, nMoney = 10, nLevel = 10, nMana = 100, nFullMana = 100;
    int nForBitChat = 0;
    // UpdateXXXNumber(nMana, nFullMana);  //血 蓝 坐标等的刷新
    if (g_pCoreShell)
    {
        KUiPlayerRuntimeInfo Info;
        memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
        g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (intptr_t)&Info, 0);
        nMana     = max(Info.nMana, 0);
        nFullMana = max(Info.nManaFull, 0);

        // UpdateRuntimeAttribute(nMoney, nLevel,nForBitChat);//钱 等级的 刷新  人物属性
        KUiPlayerAttribute _Info;
        memset(&_Info, 0, sizeof(KUiPlayerAttribute));
        g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (uintptr_t)&_Info, 0);
        nMoney      = _Info.nMoney;  // 更新钱
        nLevel      = _Info.nLevel;  // 更新等级
        nForBitChat = _Info.nForBitChat;
    }
    unsigned long dwID;
    int nLen;
    BYTE cost;
    char* Buffer;
    while (((Kuiplayerfun*)sfunLayer)->GetChannelData(dwID, cost, Buffer, nLen, nItemDwIdx))  // 计算时间
    {                                                                                         // 获取频道的消息
        int nRet           = IsHasCost(cost, nMoney, nLevel, nMana, nFullMana, nUseMoney, nUseLevel, nUseMana);
        bool bSend         = !nRet;
        char szSystem[256] = {0};
        szSystem[0]        = 0;
        if (!bSend)
        {
            if (nRet == 2 && nUseLevel > 0)
                sprintf(szSystem, "您的等级不足%d级,无法使用千里传音发送消息。", nUseLevel);

            if (nRet == 1 && nUseMoney > 0)
                sprintf(szSystem, "您的银两不足%d两,无法使用千里传音发送消息。", nUseMoney);

            if (nRet == 3 && nUseMana > 0)
                sprintf(szSystem, "您的内力不足%d点,无法使用千里传音发送消息。", nUseMana);
        }
        else
        {
            if (nUseMoney > 0)
                sprintf(szSystem, "您使用千里传音,花费银两%d两。", nUseMoney);
            if (nUseMana > 0)
                sprintf(szSystem, "您使用千里传音,消耗内力%d点。", nUseMana);
        }
        // 发送频道消息
        if (bSend)
            OnDirectSendChannelMessage(dwID, cost, Buffer, nLen, nItemDwIdx);  /// 发送频道消息协议到服务器

        ((Kuiplayerfun*)sfunLayer)->PopChannelData(dwID);  // 出栈数据，不管成功不成功。。。

        if (szSystem[0])  // 发送系统公告
        {
            // int nMsgLength;
            // nMsgLength = TEncodeText(szSystem, strlen(szSystem));//控制标记进行转换
            // KUiMsgCentrePad::SystemMessageArrival(szSystem, nMsgLength);
            if (chatLayer)
                chatLayer->addmsg("[提示]:", szSystem);
        }
    }
}

// 0 成功, 返回消耗的银两和内力
// 1 因为nUseMoney,钱不足
// 2 因为nUseLevel,等级不足
// 3 因为nUseMana,内力不足
int KgameWorld::IsHasCost(BYTE cost,
                          int nMoney,
                          int nLevel,
                          int nMana,
                          int nFullMana,
                          int& nUseMoney,
                          int& nUseLevel,
                          int& nUseMana)
{
    if (cost == 0)  // 免费
    {
        nUseMoney = 0;
        nUseLevel = 0;
        nUseMana  = 0;
    }
    else if (cost == 1)  // 10元每句
    {
        if (nMoney < 10)
        {
            nUseMoney = 10;
            return 1;
        }
        nUseMoney = 10;
        nUseLevel = 0;
        nUseMana  = 0;
    }
    else if (cost == 2)  // 2: <10Lv ? 不能说 : MaxMana/2/句
    {
        if (nLevel < 10)
        {
            nUseLevel = 10;
            return 2;
        }

        if (nMana < nFullMana / 2)
        {
            nUseMana = nFullMana / 2;
            return 3;
        }
        nUseMoney = 0;
        nUseLevel = 0;
        nUseMana  = nFullMana / 2;
    }
    else if (cost == 3)  // 3: MaxMana/10/句
    {
        if (nMana < nFullMana / 10)
        {
            nUseMana = nFullMana / 10;
            return 3;
        }
        nUseMoney = 0;
        nUseLevel = 0;
        nUseMana  = nFullMana / 10;
    }
    else if (cost == 4)  // 4: <20Lv ? 不能说 : MaxMana*4/5/句
    {
        if (nLevel < 20)
        {
            nUseLevel = 20;
            return 2;
        }

        if (nMana < nFullMana * 4 / 5)
        {
            nUseMana = nFullMana * 4 / 5;
            return 3;
        }
        nUseMoney = 0;
        nUseLevel = 0;
        nUseMana  = nFullMana * 4 / 5;
    }
    else
    {
        nUseMoney = 0;
        nUseLevel = 0;
        nUseMana  = 0;
    }
    return 0;
}

void KgameWorld::DrawSpecial_miss(int nMissIndex,
                                  int eStatus,
                                  int nNpcIndex,
                                  unsigned int nNpcDwidx,
                                  int nPos,
                                  KRUImage* pPrimitives,
                                  bool isClear,
                                  unsigned int uGenre,
                                  int nOrDer,
                                  int bSinglePlaneCoord)
{
    if (!g_GameWorld || !ParentNode_miss_Special || nMissIndex <= 0 || nMissIndex >= MAX_MISSLE)
        return;
    Node* nPartCurNode = NULL;
    char nTempKey[32];
    sprintf(nTempKey, "special_%d_%d", nMissIndex, nNpcIndex);
    std::string nCurKey = nTempKey;
    if (isClear)
    {
        ParentNode_miss_Special->removeChildByName(nCurKey, true);
        return;
    }

    if (pPrimitives && pPrimitives->szImage[0])
    {
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        Texture2D* bgCur = NULL;
        float x = 0, y = 0;
        Rect CCRectZero = ax::Rect(0, 0, 0, 0);
        nPartCurNode    = ParentNode_miss_Special->getChildByName(nCurKey);
        bgCur           = ccgetTxtData_g(pPrimitives->szImage, pPrimitives->nFrame, pPrimitives->oPosition.nX,
                                         pPrimitives->oPosition.nY, pPrimitives->oPosition.nZ, &NewscrX, &NewscrY,
                                         pPrimitives->bRenderFlag, bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);

        if (!bgCur)
        {
            if (nPartCurNode)
                ParentNode_miss_Special->removeChild(nPartCurNode, true);
            return;
        }

        if (nPartCurNode == NULL)
        {  //
            nPartCurNode = Node::create();
            if (!nPartCurNode)
                return;

            //            int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
            nPartCurNode->setName(nCurKey);
            nPartCurNode->setAnchorPoint(ax::Vec2(0, 0));
            int _scrX = pPrimitives->oPosition.nX, _scrY = pPrimitives->oPosition.nY;
            SubWorld[0].GetLocalPositionByMps(_scrX, _scrY, pPrimitives->oPosition.nZ);  // 像素坐标－－－＞屏幕坐标
            nPartCurNode->setPosition(ax::Vec2(_scrY, visibleSize.height - _scrY));
            ParentNode_miss_Special->addChild(nPartCurNode, 1);
        }
        int _scrX = pPrimitives->oPosition.nX, _scrY = pPrimitives->oPosition.nY;
        SubWorld[0].GetLocalPositionByMps(_scrX, _scrY, pPrimitives->oPosition.nZ);  // 像素坐标－－－＞屏幕坐标
        nPartCurNode->setPosition(ax::Vec2(_scrY, visibleSize.height - _scrY));

        CCRectZero.size = bgCur->getContentSize();
        (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
        Sprite* nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(1));
        if (!nPartSpr)
        {
            nPartSpr = Sprite::createWithTexture(bgCur);
            nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
            nPartCurNode->addChild(nPartSpr, 2, 1);
            NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
            nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
            nPartSpr->setLocalZOrder(nOrDer);
            return;
        }
        nPartSpr->setTexture(bgCur);
        nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);
        NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
        nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
        nPartSpr->setLocalZOrder(nOrDer);
    }
}

void KgameWorld::DrawPrimitives_miss(int nMissIndex,
                                     int nPos,
                                     KRUImage* pPrimitives,
                                     unsigned int uGenre,
                                     int bpartNo,
                                     int nOrDer,
                                     int bSinglePlaneCoord)
{
    if (!g_GameWorld || !ParentNode_miss || pPrimitives == NULL || nMissIndex <= 0 || nMissIndex >= MAX_MISSLE)
        return;

    Node* nPartCurNode = NULL;
    nPartCurNode       = ParentNode_miss->getChildByTag(nMissIndex);
    if (nPartCurNode == NULL)
        return;

    if (pPrimitives->szImage[0])
    {
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        Texture2D* bgCur = NULL;
        Rect CCRectZero  = ax::Rect(0, 0, 0, 0);
        bgCur            = ccgetTxtData(pPrimitives->szImage, pPrimitives->nFrame, pPrimitives->oPosition.nX,
                                        pPrimitives->oPosition.nY, pPrimitives->oPosition.nZ, &NewscrX, &NewscrY,
                                        pPrimitives->bRenderFlag, bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
        if (bgCur)
        {  // 修改精灵的纹理
            char msg[128] = {0};
            float x, y;
            CCRectZero.size = bgCur->getContentSize();
            (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
            Sprite* nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(1));
            if (!nPartSpr)
            {  // f
                // char msg[64];
                // sprintf(msg,"%d",pPrimitives->nFrame);
                // messageBox(pPrimitives->szImage,msg);
                nPartSpr = Sprite::createWithTexture(bgCur);
                nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                nPartCurNode->addChild(nPartSpr, 2, 1);
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                nPartSpr->setLocalZOrder(nOrDer);
                return;
            }
            nPartSpr->setTexture(bgCur);
            nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);
            NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
            nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
            nPartSpr->setLocalZOrder(nOrDer);
        }
    }
}
// VẼ MINI MAP
void KgameWorld::DrawPrimitives_minemap(int nposX, int nposY, RECT pRc, std::string nJpaPath, Texture2D* nMapTxtture)
{
    if (!m_bIsPlayerInGame || !ParentNode_minmap || m_NpcIndex <= 0 || !g_GameWorld)
        return;

    Sprite* nPartNode = (Sprite*)ParentNode_minmap->getChildByTag(MINIMAP_NODE_BGSPR);
    if (!nPartNode)
    {
        nPartNode = Sprite::create();
        nPartNode->setAnchorPoint(ax::Vec2(0, 0));
        // nPartNode->setOpacity(100);//255;//设置透明度为完全不透明（范围0～255）
        nPartNode->setColor(ax::Color3B::BLACK);
        ParentNode_minmap->addChild(nPartNode, 0, MINIMAP_NODE_BGSPR);
    }
    float m_CurPicWidth = 0, m_CurPicHeight = 0;
    Texture2D* nPicTexture = NULL;
    char nSprFileKey[64]   = {0};
    maxMapPicPath          = nJpaPath;
    maxMapRc               = pRc;
    char mjpgPath[256];
    sprintf(mjpgPath, "%s", nJpaPath.c_str());
    unsigned long nFielpahtdwid = g_FileName2Id(mjpgPath);
    sprintf(nSprFileKey, "%u-%d", nFielpahtdwid, 0);
    nPicTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFileKey);

    if (nPicTexture == NULL)
    {
        XPackElemFileRef m_PackRef;
        ZeroMemory(&m_PackRef, sizeof(XPackElemFileRef));
        bool m_bHavePicMap = false;
        if (g_pPakList)
        {
            char nTemp[128];
            sprintf(nTemp, "%s", nJpaPath.c_str());
            m_bHavePicMap = g_pPakList->pGetFilePath(nTemp, m_PackRef);
        }
        if (m_bHavePicMap)
        {
            unsigned char* sBuf = NULL;
            sBuf                = (unsigned char*)malloc(m_PackRef.nSize + 1);
            if (g_pPakList)  // 读取图片的纹理 addImageWihtData
                g_pPakList->ElemFileRead(m_PackRef, sBuf, m_PackRef.nSize);
            if (sBuf)
            {
                //			  nPicTexture =
                // ax::TextureCache::getInstance()->addImage(nSprFileKey,sBuf,m_PackRef.nSize,0);
                auto image = new ax::Image();
                image->initWithImageData(sBuf, m_PackRef.nSize);
                nPicTexture = ax::Director::getInstance()->getTextureCache()->addImage(image, nSprFileKey);
                free(sBuf);
                sBuf = NULL;
            }
        }
    }
    // nPicTexture = ax::Director::getInstance()->getTextureCache()->addImage(nJpaPath.c_str());
    if (nPicTexture)
    {
        // messageBox(nSprFilePath,"成功");
        Size n         = nPicTexture->getContentSize();
        m_CurPicWidth  = n.width;   // 纹理的宽度
        m_CurPicHeight = n.height;  // 纹理的高度
    }

    if (m_CurPicWidth <= 0 || m_CurPicHeight <= 0)
        return;

    // float nWorldWdth   = ((float)pRc.right - pRc.left)*512;
    // float nWorldHeight = ((float)pRc.bottom - pRc.top)*1024;

    // 人物的当前像素坐标
    // nPosX*32,nPosY*64
    int nRoleMpsX, nRoleMpsY;
    g_ScenePlace.GetSceneFocus(nMapidx, nRoleMpsX, nRoleMpsY);
    // float nXsele    = nWorldWdth/m_CurPicWidth;
    // float nYsele    = nWorldHeight/m_CurPicHeight;
    nCuraMapIdx     = nMapidx;
    float nRoleDisX = nRoleMpsX - pRc.left * 512;  // 人物距离原点的距离 offx  宽度
    float nRoleDisY = nRoleMpsY - pRc.top * 1024;  // 人物距离原点的距离 offy  高度

    // char msg[128];
    // sprintf(msg,"cx:%f,cy:%f\nWx:%f,Wy:%f",nXsele,nYsele,nWorldWdth,nWorldHeight);
    // messageBox(msg,"nWorldHeight");
    // 开始截取纹理

    if (nPicTexture)
    {  // nRoleDisX/nXsele nRoleDisY/nYsele
        Rect CCRectZero = ax::Rect(nRoleDisX / 16 - 64, nRoleDisY / 32 - 64, 128, 128);  // 主角在小地图内的偏移
        nPartNode->setTexture(nPicTexture);
        nPartNode->setTextureRect(CCRectZero, false, CCRectZero.size);      //,false,CCRectZero.size
        nPartNode->setPosition(ax::Vec2(nposX, visibleSize.height - 128));  // 相对偏移
    }
}

// TOẠ ĐỘ NHÂN VẬT
void KgameWorld::DrawPrimitives_RolePoint(int nRegIndex,
                                          int nPos,
                                          KRepresentUnit* pPrimitives,
                                          unsigned int uGenre,
                                          int bpartNo,
                                          int nOrDer,
                                          int bSinglePlaneCoord)
{
    if (!ParentNode_minmap || pPrimitives == NULL || !g_GameWorld)
        return;

    Sprite* nPartNode = (Sprite*)ParentNode_minmap->getChildByTag(MINIMAP_NODE_ROLE);

    if (!nPartNode)
    {
        nPartNode = Sprite::create();
        nPartNode->setAnchorPoint(ax::Vec2(0, 0));
        // nPartNode->setOpacity(100);//255;//设置透明度为完全不透明（范围0～255）
        ParentNode_minmap->addChild(nPartNode, 2, MINIMAP_NODE_ROLE);
    }

    KRUShadow* pTemp = (KRUShadow*)pPrimitives;
    for (int i = 0; i < nPos; ++i, ++pTemp)
    {
        int nX1 = pTemp->oPosition.nX;
        int nY1 = pTemp->oPosition.nY;
        int nX2 = pTemp->oEndPos.nX;
        int nY2 = pTemp->oEndPos.nY;
        if (!bSinglePlaneCoord)
        {
            SubWorld[0].GetLocalPositionByMps(nX1, nY1, pTemp->oPosition.nZ);
            SubWorld[0].GetLocalPositionByMps(nX2, nY2, pTemp->oEndPos.nZ);
        }
        // 绘画一个点
        if (nPartNode)
        {
            const Rect CCRectZero = ax::Rect(0, 0, 5, 5);
            nPartNode->setTextureRect(CCRectZero, false, CCRectZero.size);
            nPartNode->setColor(ax::Color3B::YELLOW);
            nPartNode->setPosition(ax::Vec2(nX1, visibleSize.height - nY1));
        }
    }
}

// Vẽ nhà, tường, cây cối, v.v. Các nút cư dân Tải bản đồ để vẽ toàn cục
void KgameWorld::DrawPrimitives_BigMap(int nRegIndex,
                                       int nPos,
                                       KRUImage* pPrimitives,
                                       unsigned int uGenre,
                                       int bpartNo,
                                       int nOrDer,
                                       int bSinglePlaneCoord,
                                       POINT nRegPiont)
{  // 绘画地图
    if (!ParentNode_Bigmap || pPrimitives == NULL || !g_GameWorld)
        return;

    char nRegKey[32] = {0};
    sprintf(nRegKey, "%d-%d", nRegPiont.x, nRegPiont.y);  // 区域父亲结点
    std::string nCurRegKey = nRegKey;
    Node* nRegNode         = ParentNode_Bigmap->getChildByName(nCurRegKey);  // 如果区域节点存在
    if (!nRegNode)
    {  // 如果不存在就 创建
        nRegNode = Node::create();
        //        int nCurRegKey_tmp = std::hash<std::string>{}(nCurRegKey);
        nRegNode->setName(nCurRegKey);
        ParentNode_Bigmap->addChild(nRegNode, 1);
        nRegNode->setPosition(ax::Vec2(0, 0));
        nRegNode->setAnchorPoint(ax::Vec2(0, 0));
    }
    // messageBox("绘画中","绘画中");
    Texture2D* bgCur = NULL;
    int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
    Rect CCRectZero = ax::Rect(0, 0, 0, 0);
    char nKey[32];
    switch (bpartNo)
    {
    case MAP_NODE_TREES:
    {  // 这个区域有多少棵树
        // 需要再创建一个part节点
        Node* nPartNode = nRegNode->getChildByTag(MAP_NODE_TREES);
        if (!nPartNode)
        {
            nPartNode = Node::create();
            nRegNode->addChild(nPartNode, 2, MAP_NODE_TREES);
            nPartNode->setPosition(ax::Vec2(0, 0));
            nPartNode->setAnchorPoint(ax::Vec2(0, 0));
        }
        if (nPartNode)
        {  // 如果这个节点存在
            KRUImage4* pTemp = (KRUImage4*)pPrimitives;

            for (int i = 0; i < nPos; ++i, ++pTemp)
            {
                Sprite* sprNode = NULL;
                sprintf(nKey, "tree-%d", i + 1);
                std::string nCurKey = nKey;
                sprNode             = (Sprite*)nPartNode->getChildByName(nCurKey);
                if (sprNode)
                {  // 如果存在就更新节点spr
                    bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                         pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                         &nCavesWidth, &nCavesHeight);
                    if (bgCur)
                    {  // 修改精灵的纹理
                        CCRectZero.size = bgCur->getContentSize();
                        sprNode->setTexture(bgCur);
                        sprNode->setTextureRect(CCRectZero, false, CCRectZero.size);
                        float x, y;
                        nPartNode->getPosition(&x, &y);  // 屏幕坐标
                        NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                        sprNode->setPosition(
                            ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                                                                  // sprNode->setContentSize(CCRectZero.size);
                    }
                    continue;
                }

                if (!sprNode)
                {
                    /*sprNode = Sprite::create();
                    nPartNode->addChild(sprNode,1);
                    int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
sprNode->setTag(nCurKey_tmp);
                    sprNode->setPosition(ax::Vec2(0,0));
                    sprNode->setAnchorPoint(ax::Vec2(0,0));*/
                    //}
                    // if (sprNode)
                    //{//如果存在就更新节点spr
                    // sprNode->setVisible(true);
                    bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                         pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                         &nCavesWidth, &nCavesHeight);
                    if (bgCur)
                    {  // 修改精灵的纹理
                        CCRectZero.size = bgCur->getContentSize();
                        sprNode         = Sprite::createWithTexture(bgCur, CCRectZero);
                        nPartNode->addChild(sprNode, 1);
                        //                            int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
                        sprNode->setName(nCurKey);
                        // sprNode->setPosition(ax::Vec2(0,0));
                        sprNode->setAnchorPoint(ax::Vec2(0, 0));
                        // sprNode->setTextureRect(CCRectZero,false,CCRectZero.size);
                        float x, y;
                        nPartNode->getPosition(&x, &y);  // 屏幕坐标
                        NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                        sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                        // sprNode->setTexture(bgCur);
                        // sprNode->setLocalZOrder(nOrDer);
                    }
                }
            }
        }
    }
    break;
    case MAP_NODE_HOUSE:
    {  // 这个区域有多少房子
        // 需要再创建一个part节点
        Node* nPartNode = nRegNode->getChildByTag(MAP_NODE_HOUSE);

        if (!nPartNode)
        {
            nPartNode = Node::create();
            nRegNode->addChild(nPartNode, 2, MAP_NODE_HOUSE);
            nPartNode->setPosition(ax::Vec2(0, 0));
            nPartNode->setAnchorPoint(ax::Vec2(0, 0));
        }
        if (nPartNode)
        {  // 如果这个节点存在
            KRUImage4* pTemp = (KRUImage4*)pPrimitives;

            for (int i = 0; i < nPos; ++i, ++pTemp)
            {
                Sprite* sprNode = NULL;
                sprintf(nKey, "house-%d", i + 1);
                std::string nCurKey = nKey;
                sprNode             = (Sprite*)nPartNode->getChildByName(nCurKey);

                if (sprNode)
                {
                    bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                         pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                         &nCavesWidth, &nCavesHeight);
                    if (bgCur)
                    {  // 修改精灵的纹理
                        CCRectZero.size = bgCur->getContentSize();
                        RECT rc;
                        rc.left   = 0;                    // 原来的位置
                        rc.top    = 0;                    // 原来的位置
                        rc.right  = pTemp->oImgRBPos.nX;  // 需要剪切的宽度
                        rc.bottom = pTemp->oImgRBPos.nY;  // 需要剪切的高度

                        if (rc.left < CCRectZero.origin.x)
                            rc.left = CCRectZero.origin.x;
                        if (rc.top < CCRectZero.origin.y)
                            rc.top = CCRectZero.origin.y;

                        if (rc.right > CCRectZero.size.width)
                            rc.right = CCRectZero.size.width;
                        if (rc.bottom > CCRectZero.size.height)
                            rc.bottom = CCRectZero.size.height;

                        CCRectZero = ax::Rect(pTemp->oImgLTPos.nX, pTemp->oImgLTPos.nY, rc.right, rc.bottom);
                        sprNode->setTexture(bgCur);
                        sprNode->setTextureRect(CCRectZero, false, CCRectZero.size);  //,false,CCRectZero.size
                        float x, y;
                        nPartNode->getPosition(&x, &y);                                   // 屏幕坐标
                        NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;  // g_Int2Round(NewscrX)
                        // sprNode->setLocalZOrder(nOrDer);
                        sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                    }
                    continue;
                }

                if (!sprNode)
                {
                    /*sprNode = Sprite::create();
                    nPartNode->addChild(sprNode,1);
                    int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
sprNode->setTag(nCurKey_tmp);
                    sprNode->setPosition(ax::Vec2(0,0));
                    sprNode->setAnchorPoint(ax::Vec2(0,0));*/
                    //}
                    //
                    // if (sprNode)
                    //{//如果存在就更新节点spr
                    // sprNode->setVisible(true);
                    bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                         pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                         &nCavesWidth, &nCavesHeight);
                    if (bgCur)
                    {  // 修改精灵的纹理
                        CCRectZero.size = bgCur->getContentSize();
                        RECT rc;
                        rc.left   = 0;                    // 原来的位置
                        rc.top    = 0;                    // 原来的位置
                        rc.right  = pTemp->oImgRBPos.nX;  // 需要剪切的宽度
                        rc.bottom = pTemp->oImgRBPos.nY;  // 需要剪切的高度

                        if (rc.left < CCRectZero.origin.x)
                            rc.left = CCRectZero.origin.x;
                        if (rc.top < CCRectZero.origin.y)
                            rc.top = CCRectZero.origin.y;

                        if (rc.right > CCRectZero.size.width)
                            rc.right = CCRectZero.size.width;
                        if (rc.bottom > CCRectZero.size.height)
                            rc.bottom = CCRectZero.size.height;

                        CCRectZero = ax::Rect(pTemp->oImgLTPos.nX, pTemp->oImgLTPos.nY, rc.right, rc.bottom);
                        sprNode    = Sprite::createWithTexture(bgCur, CCRectZero);
                        nPartNode->addChild(sprNode, 1);
                        //                            int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
                        sprNode->setName(nCurKey);
                        // sprNode->setPosition(ax::Vec2(0,0));
                        sprNode->setAnchorPoint(ax::Vec2(0, 0));

                        // sprNode->setTextureRect(CCRectZero,false,CCRectZero.size);//,false,CCRectZero.size
                        float x, y;
                        nPartNode->getPosition(&x, &y);                                   // 屏幕坐标
                        NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;  // g_Int2Round(NewscrX)
                        sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));         // 相对偏移
                        // sprNode->setTexture(bgCur);
                        // sprNode->setLocalZOrder(nOrDer);
                    }
                }
            }
        }
    }
    break;
    case MAP_NODE_ABOVE:
        break;
    default:
        break;
    }
}

// Vẽ các vật thể có độ cao, mái nhà, v.v.
void KgameWorld::DrawAboveHead(int index,
                               POINT nRegionIndex,
                               int nCount,
                               KRUImage* pPrimitives,
                               int nKind,
                               int bSinglePlaneCoord)
{
    if (!g_GameWorld || !ParentNode_objabove || pPrimitives == NULL)
        return;
    char nRegKey[32] = {0};  // 整个9大区域的总结点
    sprintf(nRegKey, "above-0");
    std::string nCurRegKey = nRegKey;
    Node* nRegNode         = ParentNode_objabove->getChildByName(nCurRegKey);  // 如果区域节点存在

    if (!nRegNode)
    {  // 如果不存在就 创建
        nRegNode = Node::create();
        //        int nCurRegKey_tmp = std::hash<std::string>{}(nCurRegKey);
        nRegNode->setName(nCurRegKey);
        ParentNode_objabove->addChild(nRegNode, 1);
        nRegNode->setPosition(ax::Vec2(0, 0));
        nRegNode->setAnchorPoint(ax::Vec2(0, 0));
    }
    int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
    Texture2D* bgCur = NULL;
    Rect CCpicRect   = ax::Rect(0, 0, 0, 0);
    switch (nKind)
    {
    case BAOVE_NODE_PART:
    {
        Node* nPartNode = nRegNode->getChildByTag(BAOVE_NODE_PART);
        if (!nPartNode)
        {
            nPartNode = Node::create();
            nRegNode->addChild(nPartNode, 4, BAOVE_NODE_PART);
            nPartNode->setPosition(ax::Vec2(0, 0));
            nPartNode->setAnchorPoint(ax::Vec2(0, 0));
        }
        if (nPartNode)
        {  // 如果这个节点存在
            KRUImage4* pTemp = (KRUImage4*)pPrimitives;
            for (int i = 0; i < nCount; ++i, ++pTemp)
            {
                Sprite* sprNode = NULL;
                // sprintf(nKey,"part-%d-%d",index+1,i);
                // std::string nCurKey = nKey;
                char nSprFilePath[64]       = {0};  // 这个spr的第几帧的精灵如果存在
                unsigned long nFielpahtdwid = g_FileName2Id(pTemp->szImage);
                sprintf(nSprFilePath, "%u-%d_%d", nFielpahtdwid, pTemp->nFrame, BAOVE_NODE_PART);
                std::string nCurKey = nSprFilePath;
                sprNode             = (Sprite*)nPartNode->getChildByName(nCurKey);
                if (sprNode)
                {  // 如果存在
                    bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                         pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                         &nCavesWidth, &nCavesHeight);
                    // SubWorld[0].GetLocalPositionByMps(nStartPointX,nStartPointY,0);
                    if (!bgCur)
                        break;
                    CCpicRect.size = bgCur->getContentSize();
                    RECT rc;
                    rc.left   = 0;                    // 原来的位置 oImgRBPos
                    rc.top    = 0;                    // 原来的位置
                    rc.right  = pTemp->oImgRBPos.nX;  // 需要剪切的宽度
                    rc.bottom = pTemp->oImgRBPos.nY;  // 需要剪切的高度
                    if (rc.left < CCpicRect.origin.x)
                        rc.left = CCpicRect.origin.x;
                    if (rc.top < CCpicRect.origin.y)
                        rc.top = CCpicRect.origin.y;

                    if (rc.right > CCpicRect.size.width)
                        rc.right = CCpicRect.size.width;
                    if (rc.bottom > CCpicRect.size.height)
                        rc.bottom = CCpicRect.size.height;

                    CCpicRect = ax::Rect(pTemp->oImgLTPos.nX, pTemp->oImgLTPos.nY, rc.right, rc.bottom);
                    // sprNode->setTextureRect(CCpicRect,false,CCpicRect.size);//,false,CCRectZero.size
                    float x, y;
                    nPartNode->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCpicRect.size.height;
                    sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                    continue;
                }

                if (!sprNode)
                {
                    bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                         pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                         &nCavesWidth, &nCavesHeight);
                    // SubWorld[0].GetLocalPositionByMps(nStartPointX,nStartPointY,0);
                    if (!bgCur)
                        break;
                    CCpicRect.size = bgCur->getContentSize();
                    RECT rc;
                    rc.left   = 0;                    // 原来的位置 oImgRBPos
                    rc.top    = 0;                    // 原来的位置
                    rc.right  = pTemp->oImgRBPos.nX;  // 需要剪切的宽度
                    rc.bottom = pTemp->oImgRBPos.nY;  // 需要剪切的高度
                    if (rc.left < CCpicRect.origin.x)
                        rc.left = CCpicRect.origin.x;
                    if (rc.top < CCpicRect.origin.y)
                        rc.top = CCpicRect.origin.y;

                    if (rc.right > CCpicRect.size.width)
                        rc.right = CCpicRect.size.width;
                    if (rc.bottom > CCpicRect.size.height)
                        rc.bottom = CCpicRect.size.height;

                    CCpicRect = ax::Rect(pTemp->oImgLTPos.nX, pTemp->oImgLTPos.nY, rc.right, rc.bottom);
                    sprNode   = Sprite::createWithTexture(bgCur, CCpicRect);
                    // sprNode->setTextureRect(CCpicRect,false,CCpicRect.size);//,false,CCRectZero.size
                    nPartNode->addChild(sprNode, 1);  // 图集加入精灵
                    //                        int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
                    sprNode->setName(nCurKey);
                    // sprNode->setOpacity(0);
                    sprNode->setPosition(ax::Vec2(0, 0));
                    sprNode->setAnchorPoint(ax::Vec2(0, 0));
                    float x, y;
                    nPartNode->getPosition(&x, &y);
                    NewscrY = visibleSize.height - NewscrY - CCpicRect.size.height;
                    sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    // sprNode->setOpacity(255);
                    break;
                }

                /*if (sprNode)
                {//如果存在就更新节点spr
                        //sprNode->setVisible(true);
                        bgCur =
                ccgetTxtData(pTemp->szImage,pTemp->nFrame,pTemp->oPosition.nX,pTemp->oPosition.nY,pTemp->oPosition.nZ,&NewscrX,&NewscrY,pTemp->bRenderFlag,bSinglePlaneCoord,&nCavesWidth,&nCavesHeight);
                        if (bgCur)
                        {//修改精灵的纹理
                                CCpicRect.size = bgCur->getContentSize();

                                RenderTexture *npicCanvas =
                RenderTexture::create(CCpicRect.size.width,CCpicRect.size.height,kTexture2DPixelFormat_RGBA8888,0);
                                npicCanvas->setAnchorPoint(ax::Vec2(0,0));
                                ParentNode_objabove->addChild(npicCanvas,-1);
                                //画板类
                                if  (npicCanvas)
                                {//同一个区域的
                                        npicCanvas->clear(255,255,255,0);//半透明的方式清除画板
                                        //获取屏幕坐标
                                        //SubWorld[0].GetLocalPositionByMps(nStartPointX,nStartPointY,0);
                                        //设置画板的位置
                                        RECT	rc;
                                        rc.left  = 0;             //原来的位置 oImgRBPos
                                        rc.top   = 0;             //原来的位置
                                        rc.right = pTemp->oImgRBPos.nX; //需要剪切的宽度
                                        rc.bottom= pTemp->oImgRBPos.nY; //需要剪切的高度
                                        if (rc.left < CCpicRect.origin.x)
                                                rc.left = CCpicRect.origin.x;
                                        if (rc.top < CCpicRect.origin.y)
                                                rc.top = CCpicRect.origin.y;

                                        if (rc.right > CCpicRect.size.width)
                                                rc.right = CCpicRect.size.width;
                                        if (rc.bottom > CCpicRect.size.height)
                                                rc.bottom = CCpicRect.size.height;

                                        CCpicRect =
                ax::Rect(pTemp->oImgLTPos.nX,pTemp->oImgLTPos.nY,rc.right,rc.bottom);
                                        //nBgtestSpr->setTextureRect(CCpicRect,false,CCpicRect.size);//,false,CCRectZero.size
                                        float x,y;
                                        nPartNode->getPosition(&x,&y);             //屏幕坐标
                                        NewscrY = visibleSize.height-NewscrY-CCpicRect.size.height;
                                        npicCanvas->setPosition(ax::Vec2(NewscrX-x,NewscrY-y));//相对偏移
                                        //npicCanvas->setPosition(ax::Vec2(nStartPointX+256,visibleSize.height-nStartPointY-256));

                                        Sprite *nBgtestSpr =  (Sprite *)this->getChildByName("spr_house_1");
                                        //创建一个用于绘画的精灵
                                        if (!nBgtestSpr)
                                        {
                                                nBgtestSpr = Sprite::create();
                                                nBgtestSpr->setAnchorPoint(ax::Vec2(0,0));
                                                nBgtestSpr->setPosition(ax::Vec2(0,0));
                                                nBgtestSpr->setTag("spr_house_1");
                                        }
                                    //开始绘画
                                        Rect CCRectZero= ax::Rect(0,0,0,0);
                                        npicCanvas->begin();
                                        //SubWorld[0].GetMpsByLocalPosition(NewscrX,NewscrY,0);//计算偏移后新的像素坐标
                                        for (int j = 0;j<2;j++)
                                        {//每张绘画几次 修改精灵的纹理
                                                CCpicRect.size = bgCur->getContentSize();
                                                RECT	rc;
                                                rc.left  = 0;             //原来的位置 oImgRBPos
                                                rc.top   = 0;             //原来的位置
                                                rc.right = pTemp->oImgRBPos.nX; //需要剪切的宽度
                                                rc.bottom= pTemp->oImgRBPos.nY; //需要剪切的高度
                                                if (rc.left < CCpicRect.origin.x)
                                                        rc.left = CCpicRect.origin.x;
                                                if (rc.top < CCpicRect.origin.y)
                                                        rc.top = CCpicRect.origin.y;

                                                if (rc.right > CCpicRect.size.width)
                                                        rc.right = CCpicRect.size.width;
                                                if (rc.bottom > CCpicRect.size.height)
                                                        rc.bottom = CCpicRect.size.height;

                                                CCpicRect =
                ax::Rect(pTemp->oImgLTPos.nX,pTemp->oImgLTPos.nY,rc.right,rc.bottom);
                                                nBgtestSpr->setTextureRect(CCpicRect,false,CCpicRect.size);//,false,CCRectZero.size
                                                float x,y;
                                                nPartNode->getPosition(&x,&y);             //屏幕坐标
                                                NewscrY = visibleSize.height-NewscrY-CCpicRect.size.height;
                                                nBgtestSpr->setPosition(ax::Vec2(0,0));//相对偏移 NewscrX-x,NewscrY-y
                                                nBgtestSpr->setTexture(bgCur);
                                                nBgtestSpr->visit();
                                        }
                                        npicCanvas->end();

                                        Texture2D* ncurSpr =NULL;
                                        ncurSpr = npicCanvas->getSprite()->getTexture();
                                        if (ncurSpr)
                                        {//合成后的大纹理
                                                CCpicRect.size = ncurSpr->getContentSize();
                                                RECT	rc;
                                                rc.left  = 0;             //原来的位置 oImgRBPos
                                                rc.top   = 0;             //原来的位置
                                                rc.right = pTemp->oImgRBPos.nX; //需要剪切的宽度
                                                rc.bottom= pTemp->oImgRBPos.nY; //需要剪切的高度
                                                if (rc.left < CCpicRect.origin.x)
                                                        rc.left = CCpicRect.origin.x;
                                                if (rc.top < CCpicRect.origin.y)
                                                        rc.top = CCpicRect.origin.y;

                                                if (rc.right > CCpicRect.size.width)
                                                        rc.right = CCpicRect.size.width;
                                                if (rc.bottom > CCpicRect.size.height)
                                                        rc.bottom = CCpicRect.size.height;

                                                CCpicRect =
                ax::Rect(pTemp->oImgLTPos.nX,pTemp->oImgLTPos.nY,rc.right,rc.bottom);
                                                sprNode->setTextureRect(CCpicRect,false,CCpicRect.size);//,false,CCRectZero.size
                                                float x,y;
                                                nPartNode->getPosition(&x,&y);             //屏幕坐标
                                                NewscrY = visibleSize.height-NewscrY-CCpicRect.size.height;
                                                sprNode->setPosition(ax::Vec2(NewscrX-x,NewscrY-y));//相对偏移
                                                sprNode->setTexture(ncurSpr);
                                                sprNode->setScaleY(-1);
                                        }
                                        ParentNode_objabove->removeChild(npicCanvas,true);
                                }
                        }
                }*/
            }
        }
    }
    break;
    case BAOVE_NODE_FULL:
    {
        // messageBox("测试测试","测试测试");
        Node* nPartNode = nRegNode->getChildByTag(BAOVE_NODE_FULL);
        if (!nPartNode)
        {
            nPartNode = Node::create();
            nRegNode->addChild(nPartNode, 4, BAOVE_NODE_FULL);
            nPartNode->setPosition(ax::Vec2(0, 0));
            nPartNode->setAnchorPoint(ax::Vec2(0, 0));
        }
        if (nPartNode)
        {  // 如果这个节点存在
            KRUImage* pTemp = (KRUImage*)pPrimitives;
            //				char nKey[32];
            for (int i = 0; i < nCount; ++i, ++pTemp)
            {
                Sprite* sprNode             = NULL;
                char nSprFilePath[64]       = {0};  // 这个spr的第几帧的精灵如果存在
                unsigned long nFielpahtdwid = g_FileName2Id(pTemp->szImage);
                sprintf(nSprFilePath, "%u-%d_%d", nFielpahtdwid, pTemp->nFrame, BAOVE_NODE_FULL);
                std::string nCurKey = nSprFilePath;
                sprNode             = (Sprite*)nPartNode->getChildByName(nCurKey);
                if (sprNode)
                {  // 如果存在
                    bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                         pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                         &nCavesWidth, &nCavesHeight);
                    // SubWorld[0].GetLocalPositionByMps(nStartPointX,nStartPointY,0);
                    if (!bgCur)
                        break;
                    CCpicRect.size = bgCur->getContentSize();
                    // sprNode->setTextureRect(CCpicRect,false,CCpicRect.size);//,false,CCRectZero.size
                    float x, y;
                    nPartNode->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCpicRect.size.height;
                    sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                    continue;
                }

                if (!sprNode)
                {
                    bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                         pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                         &nCavesWidth, &nCavesHeight);
                    // SubWorld[0].GetLocalPositionByMps(nStartPointX,nStartPointY,0);
                    if (!bgCur)
                        break;
                    CCpicRect.size = bgCur->getContentSize();
                    sprNode        = Sprite::createWithTexture(bgCur, CCpicRect);
                    // sprNode->setTextureRect(CCpicRect,false,CCpicRect.size);//,false,CCRectZero.size
                    nPartNode->addChild(sprNode, 1);  // 图集加入精灵
                    //                        int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
                    sprNode->setName(nCurKey);
                    // sprNode->setOpacity(0);
                    sprNode->setPosition(ax::Vec2(0, 0));
                    sprNode->setAnchorPoint(ax::Vec2(0, 0));
                    float x, y;
                    nPartNode->getPosition(&x, &y);
                    NewscrY = visibleSize.height - NewscrY - CCpicRect.size.height;
                    sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    // sprNode->setOpacity(255);
                    break;
                }
            }  // end for
        }      // end part
    }
    break;
    default:
        break;
    }
}
// Vẽ nhà, tường, cây cối, v.v. Các nút cố định Tải bản đồ để vẽ toàn cầu
void KgameWorld::_DrawPrimitives_BigMap(int nRegIndex,
                                        int nPos,
                                        KRUImage* pPrimitives,
                                        unsigned int uGenre,
                                        int bpartNo,
                                        int nOrDer,
                                        int bSinglePlaneCoord,
                                        POINT nRegPiont)
{  // 绘画地图
    if (!ParentNode_Bigmap || pPrimitives == NULL || !g_GameWorld)
        return;

    char nRegKey[32] = {0};
    sprintf(nRegKey, "%d-%d", nRegPiont.x, nRegPiont.y);
    std::string nCurRegKey = nRegKey;
    // SpriteBatchNode::create("TextureAtlas/default.png"); //默认的纹理图集
    Node* nRegNode = ParentNode_Bigmap->getChildByName(nCurRegKey);  // 如果区域节点存在

    if (!nRegNode)
    {  // 如果不存在就 创建
        nRegNode = Node::create();
        //           int nCurRegKey_tmp = std::hash<std::string>{}(nCurRegKey);
        nRegNode->setName(nCurRegKey);
        ParentNode_Bigmap->addChild(nRegNode, 1);
        nRegNode->setPosition(ax::Vec2(0, 0));
        nRegNode->setAnchorPoint(ax::Vec2(0, 0));
    }
    // messageBox("绘画中","绘画中");
    Texture2D* bgCur = NULL;
    int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
    Rect CCRectZero = ax::Rect(0, 0, 0, 0);
    char nKey[32];
    switch (bpartNo)
    {
    case MAP_NODE_TREES:
    {
        // 需要再创建一个part节点
        // SpriteBatchNode::create("TextureAtlas/default.png",nPos);//树图集
        SpriteBatchNode* nPartNode = (SpriteBatchNode*)nRegNode->getChildByTag(MAP_NODE_TREES);

        if (!nPartNode)
        {
            // nPartNode = Node::create();
            nPartNode = SpriteBatchNode::create("TextureAtlas/default.png", nPos);
            nRegNode->addChild(nPartNode, 2, MAP_NODE_TREES);
            nPartNode->setPosition(ax::Vec2(0, 0));
            nPartNode->setAnchorPoint(ax::Vec2(0, 0));
        }
        if (nPartNode)
        {  // 如果这个节点存在
            KRUImage4* pTemp = (KRUImage4*)pPrimitives;

            for (int i = 0; i < nPos; ++i, ++pTemp)
            // for(int i = 0 ;i<nPos;i++)
            {
                // int nidx =  FindTreeFree();//最小从1开始
                Sprite* sprNode = NULL;
                // 已经存字
                // if (nidx>0)
                {
                    // AddTreeIndex(nidx);
                    sprintf(nKey, "tree-%d", i + 1);
                    std::string nCurKey = nKey;
                    sprNode             = (Sprite*)nPartNode->getChildByName(nCurKey);
                }

                if (!sprNode)
                {
                    sprintf(nKey, "tree-%d", i + 1);
                    std::string nCurKey = nKey;
                    Sprite* sprNode     = Sprite::create();
                    nPartNode->addChild(sprNode, 1);  // 图集加入精灵
                    //                           int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
                    sprNode->setName(nCurKey);
                    sprNode->setPosition(ax::Vec2(0, 0));
                    sprNode->setAnchorPoint(ax::Vec2(0, 0));
                }

                if (sprNode)
                {  // 如果存在就更新节点spr
                    // sprNode->setVisible(true);
                    bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                         pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                         &nCavesWidth, &nCavesHeight);
                    if (bgCur)
                    {  // 修改精灵的纹理
                        CCRectZero.size = bgCur->getContentSize();
                        sprNode->setTextureRect(CCRectZero, false, CCRectZero.size);
                        float x, y;
                        nPartNode->getPosition(&x, &y);  // 屏幕坐标
                        NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                        sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                        sprNode->setTexture(bgCur);
                        // sprNode->setLocalZOrder(nOrDer);
                    }
                }
            }
        }
    }
    break;
    case MAP_NODE_HOUSE:
    {
        // 需要再创建一个part节点
        // Node *nPartNode = nRegNode->getChildByTag(MAP_NODE_HOUSE);
        SpriteBatchNode* nPartNode = (SpriteBatchNode*)nRegNode->getChildByTag(MAP_NODE_HOUSE);
        if (!nPartNode)
        {
            // nPartNode = Node::create();
            nPartNode = SpriteBatchNode::create("TextureAtlas/default.png", nPos);
            nRegNode->addChild(nPartNode, 2, MAP_NODE_HOUSE);
            nPartNode->setPosition(ax::Vec2(0, 0));
            nPartNode->setAnchorPoint(ax::Vec2(0, 0));
        }
        if (nPartNode)
        {  // 如果这个节点存在
            KRUImage4* pTemp = (KRUImage4*)pPrimitives;

            for (int i = 0; i < nPos; ++i, ++pTemp)
            {
                // int nidx =  FindHouseFree();//最小从1开始
                Sprite* sprNode = NULL;
                // 已经存字
                // if (nidx>0)
                {
                    // AddHouseIndex(nidx);
                    sprintf(nKey, "house-%d", i + 1);
                    std::string nCurKey = nKey;
                    sprNode             = (Sprite*)nPartNode->getChildByName(nCurKey);
                }

                if (!sprNode)
                {
                    sprintf(nKey, "house-%d", i + 1);
                    std::string nCurKey = nKey;
                    Sprite* sprNode     = Sprite::create();
                    nPartNode->addChild(sprNode, 1);
                    //                           int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
                    sprNode->setName(nCurKey);
                    sprNode->setPosition(ax::Vec2(0, 0));
                    sprNode->setAnchorPoint(ax::Vec2(0, 0));
                }

                if (sprNode)
                {  // 如果存在就更新节点spr
                    // sprNode->setVisible(true);
                    bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                         pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                         &nCavesWidth, &nCavesHeight);
                    if (bgCur)
                    {  // 修改精灵的纹理
                        CCRectZero.size = bgCur->getContentSize();
                        RECT rc;
                        rc.left   = 0;                    // 原来的位置
                        rc.top    = 0;                    // 原来的位置
                        rc.right  = pTemp->oImgRBPos.nX;  // 需要剪切的宽度
                        rc.bottom = pTemp->oImgRBPos.nY;  // 需要剪切的高度

                        if (rc.left < CCRectZero.origin.x)
                            rc.left = CCRectZero.origin.x;
                        if (rc.top < CCRectZero.origin.y)
                            rc.top = CCRectZero.origin.y;

                        if (rc.right > CCRectZero.size.width)
                            rc.right = CCRectZero.size.width;
                        if (rc.bottom > CCRectZero.size.height)
                            rc.bottom = CCRectZero.size.height;

                        CCRectZero = ax::Rect(pTemp->oImgLTPos.nX, pTemp->oImgLTPos.nY, rc.right, rc.bottom);
                        sprNode->setTextureRect(CCRectZero, false, CCRectZero.size);  //,false,CCRectZero.size
                        float x, y;
                        nPartNode->getPosition(&x, &y);                                   // 屏幕坐标
                        NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;  // g_Int2Round(NewscrX)
                        sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));         // 相对偏移
                        sprNode->setTexture(bgCur);
                        // sprNode->setLocalZOrder(nOrDer);
                    }
                }
            }
        }
    }
    break;
    case MAP_NODE_ABOVE:
    {
        // 需要再创建一个part节点
        Node* nPartNode = ParentNode_Bigmap->getChildByTag(MAP_NODE_ABOVE);

        if (!nPartNode)
        {
            nPartNode = Node::create();
            ParentNode_Bigmap->addChild(nPartNode, 2, MAP_NODE_ABOVE);
            nPartNode->setPosition(ax::Vec2(0, 0));
            nPartNode->setAnchorPoint(ax::Vec2(0, 0));
        }
        if (nPartNode)
        {  // 如果这个节点存在
            for (int i = 0; i < nPos; i++)
            {
            }
        }
    }
    break;
    default:
        break;
    }
}

void KgameWorld::DrawPrimitives_map(int nRegIndex,
                                    int nPos,
                                    KRUImage* pPrimitives,
                                    unsigned int uGenre,
                                    int bpartNo,
                                    int nOrDer,
                                    int bSinglePlaneCoord,
                                    POINT nRegPiont,
                                    POINT nRegRbMpsPoint)
{  // 绘画地图
   // messageBox("rawPrimitives_map","DrawPrimitives_map");

    if (!g_GameWorld || !ParentNode_map || pPrimitives == NULL)
        return;

    // if  (bpartNo != MAP_NODE_PART && nPos<=0)
    //   return;

    // char msg[128];
    // sprintf(msg,"index:%d,uGenre:%d,bpartNo:%d,nPos:%d",nRegIndex,uGenre,bpartNo,nPos);
    // messageBox(msg,"DrawPrimitives_map");
    char nRegKey[32] = {0};
    sprintf(nRegKey, "%d-%d", nRegPiont.x, nRegPiont.y);
    std::string nCurRegKey = nRegKey;
    Node* nRegNode         = ParentNode_map->getChildByName(nCurRegKey);  // 如果区域节点存在

    if (!nRegNode)
    {  // 如果不存在就 创建
        nRegNode           = Node::create();
        int nCurRegKey_tmp = std::hash<std::string>{}(nCurRegKey);
        nRegNode->setName(nCurRegKey);
        ParentNode_map->addChild(nRegNode, 1);
        nRegNode->setPosition(ax::Vec2(0, 0));
        nRegNode->setAnchorPoint(ax::Vec2(0, 0));
    }

    if (nRegNode)
    {  // 区域 已经存在

        Texture2D* bgCur = NULL;
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        Rect CCRectZero = ax::Rect(0, 0, 0, 0);
        switch (bpartNo)
        {
        case MAP_NODE_LUMIAN_A:
        {  // 一些零散的路面 草堆
            Node* nPartNode = nRegNode->getChildByTag(MAP_NODE_LUMIAN_A);
            if (!nPartNode)
            {
                nPartNode = Node::create();
                nRegNode->addChild(nPartNode, 1, MAP_NODE_LUMIAN_A);
                nPartNode->setPosition(ax::Vec2(0, 0));
                nPartNode->setAnchorPoint(ax::Vec2(0, 0));
            }
            if (nPartNode)
            {  // 如果这个节点存在
                char nKey[32] = {0};

                for (int i = 0; i < nPos; i++)
                {
                    // int nidx =  FindFree(nRegIndex-1);//最小从1开始
                    Sprite* sprNode = NULL;
                    // 已经存字
                    // if (nidx>0)
                    {
                        // AddIndex(nRegIndex-1,nidx);
                        sprintf(nKey, "%s-%d", nCurRegKey.c_str(), i + 1);
                        std::string nCurKey = nKey;
                        sprNode             = (Sprite*)nPartNode->getChildByName(nCurKey);
                    }

                    if (!sprNode)
                    {
                        sprintf(nKey, "%s-%d", nCurRegKey.c_str(), i + 1);
                        std::string nCurKey = nKey;
                        Sprite* sprNode     = Sprite::create();
                        nPartNode->addChild(sprNode, 1);
                        //                           int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
                        sprNode->setName(nCurKey);
                        sprNode->setPosition(ax::Vec2(0, 0));
                        sprNode->setAnchorPoint(ax::Vec2(0, 0));
                    }
                    // char msg[128];
                    // sprintf(msg,"%d/%d,%s\n%s",i,nPos,nKey,pPrimitives[i].szImage);
                    // messageBox(msg,"MAP_NODE_LUMIAN_A");
                    if (sprNode)
                    {  // 如果存在就更新节点spr
                        // sprNode->setVisible(true);
                        bgCur =
                            ccgetTxtData(pPrimitives[i].szImage, pPrimitives[i].nFrame, pPrimitives[i].oPosition.nX,
                                         pPrimitives[i].oPosition.nY, pPrimitives[i].oPosition.nZ, &NewscrX, &NewscrY,
                                         pPrimitives[i].bRenderFlag, bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
                        if (bgCur)
                        {  // 修改精灵的纹理
                            CCRectZero.size = bgCur->getContentSize();
                            sprNode->setTexture(bgCur);
                            sprNode->setTextureRect(CCRectZero, false, CCRectZero.size);
                            float x, y;
                            nPartNode->getPosition(&x, &y);  // 屏幕坐标
                            NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                            sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                            // sprNode->setLocalZOrder(nOrDer);
                        }
                    }
                }
            }
        }
        break;
        case MAP_NODE_LUMIAN:
        {  // 是大于 64个的最低层节点数
        }
        break;
        case MAP_NODE_PART:
        {  // 一张一张的绘画 上层路面
            // 需要再创建一个part节点
            Node* nPartNode = nRegNode->getChildByTag(MAP_NODE_PART);

            if (!nPartNode)
            {
                nPartNode = Node::create();
                nRegNode->addChild(nPartNode, 2, MAP_NODE_PART);
                nPartNode->setPosition(ax::Vec2(0, 0));
                nPartNode->setAnchorPoint(ax::Vec2(0, 0));
            }

            if (nPartNode)
            {  // 如果这个节点存在
                char nKey[32]       = {0};
                KRUImagePart* pTemp = (KRUImagePart*)pPrimitives;

                for (int i = 0; i < nPos; ++i, ++pTemp)
                {
                    // int nidx =  FindPartFree(nRegIndex-1);//最小从1开始
                    Sprite* sprNode = NULL;
                    // 已经存字
                    // if (nidx>0)
                    {
                        // AddPartIndex(nRegIndex-1,nidx);
                        // sprintf(nKey,"%d-%d",nRegIndex,i+1);
                        sprintf(nKey, "%s-%d", nCurRegKey.c_str(), i + 1);
                        std::string nCurKey = nKey;
                        sprNode             = (Sprite*)nPartNode->getChildByName(nCurKey);
                    }
                    if (!sprNode)
                    {
                        // sprintf(nKey,"%d-%d",nRegIndex,i+1);
                        sprintf(nKey, "%s-%d", nCurRegKey.c_str(), i + 1);
                        std::string nCurKey = nKey;
                        Sprite* sprNode     = Sprite::create();
                        nPartNode->addChild(sprNode, 1);
                        //                           int nCurKey_tmp = std::hash<std::string>{}(nCurKey);
                        sprNode->setName(nCurKey);
                        sprNode->setPosition(ax::Vec2(0, 0));
                        sprNode->setAnchorPoint(ax::Vec2(0, 0));
                    }

                    if (sprNode)
                    {  // 如果存在就更新节点spr
                        sprNode->setVisible(true);
                        bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                             pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag,
                                             bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
                        if (bgCur)
                        {  // 修改精灵的纹理
                            CCRectZero.size = bgCur->getContentSize();
                            RECT rc;
                            rc.left   = 0;                    // 原来的位置
                            rc.top    = 0;                    // 原来的位置
                            rc.right  = pTemp->oImgRBPos.nX;  // 需要剪切的宽度
                            rc.bottom = pTemp->oImgRBPos.nY;  // 需要剪切的高度

                            if (rc.left < CCRectZero.origin.x)
                                rc.left = CCRectZero.origin.x;
                            if (rc.top < CCRectZero.origin.y)
                                rc.top = CCRectZero.origin.y;

                            if (rc.right > CCRectZero.size.width)
                                rc.right = CCRectZero.size.width;
                            if (rc.bottom > CCRectZero.size.height)
                                rc.bottom = CCRectZero.size.height;

                            CCRectZero = ax::Rect(pTemp->oImgLTPos.nX, pTemp->oImgLTPos.nY, rc.right, rc.bottom);
                            sprNode->setTextureRect(CCRectZero, false, CCRectZero.size);  //,false,CCRectZero.size
                            float x, y;
                            nPartNode->getPosition(&x, &y);  // 屏幕坐标
                            NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                            sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                            sprNode->setTexture(bgCur);
                            // sprNode->setLocalZOrder(nOrDer);
                        }
                    }
                }

                /*int  nSprCount =  GetPartSprCount(nRegIndex-1);
                if (nPos>0 && nSprCount>0 && nSprCount>nPos)
                {
                        for(int k = nPos+1 ;k<nSprCount+1;k++)
                        {//删除多余的spr
                                sprintf(nKey,"%d-%d",nRegIndex,k);
                                std::string nCurKey = nKey;
                                Sprite *sprNode = (Sprite *)nPartNode->getChildByName(nCurKey);
                                if (sprNode)
                                        sprNode->setVisible(false); //先停止渲染
                                nPartNode->removeChild(sprNode,true);
                        }
                }
                SetPartSprCount(nRegIndex-1,nPos);*/
            }
        }
        break;
        case MAP_NODE_OTHER:
        {
            // 需要再创建一个part节点
            Node* nPartNode = nRegNode->getChildByTag(MAP_NODE_OTHER);
            if (!nPartNode)
            {
                nPartNode = Node::create();
                nRegNode->addChild(nPartNode, 1, MAP_NODE_OTHER);
                nPartNode->setPosition(ax::Vec2(0, 0));
                nPartNode->setAnchorPoint(ax::Vec2(0, 0));
            }

            if (nPartNode)
            {  // 如果这个节点存在
                for (int i = 0; i < nPos; i++)
                {
                    char nKey[32] = {0};
                    // sprintf(nKey,"%d-%d-%d",nRegIndex,uGenre,i+1);
                    sprintf(nKey, "%d-%d", nRegIndex, i + 1);
                    std::string nCurKey = nKey;
                    Sprite* sprNode     = (Sprite*)nPartNode->getChildByName(nCurKey);

                    if (!sprNode)
                    {
                        Sprite* sprNode = Sprite::create();
                        nPartNode->addChild(sprNode, 1);
                        //						   int nCurKey_tmp =
                        // std::hash<std::string>{}(nCurKey);
                        sprNode->setName(nCurKey);
                        sprNode->setPosition(ax::Vec2(0, 0));
                        sprNode->setAnchorPoint(ax::Vec2(0, 0));
                    }

                    if (sprNode)
                    {  // 如果存在就更新节点spr
                        bgCur =
                            ccgetTxtData(pPrimitives[i].szImage, pPrimitives[i].nFrame, pPrimitives[i].oPosition.nX,
                                         pPrimitives[i].oPosition.nY, pPrimitives[i].oPosition.nZ, &NewscrX, &NewscrY,
                                         pPrimitives[i].bRenderFlag, bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
                        if (bgCur)
                        {  // 修改精灵的纹理
                            CCRectZero.size = bgCur->getContentSize();
                            sprNode->setTextureRect(CCRectZero, false, CCRectZero.size);
                            // char msg[128]={0};
                            float x, y;
                            nPartNode->getPosition(&x, &y);  // 屏幕坐标
                            NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                            sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                            sprNode->setTexture(bgCur);
                            sprNode->setLocalZOrder(nOrDer);
                        }
                    }
                }
            }
        }
        break;
        default:
            break;
        }
    }
    /* else
     {//还没有父节点 ---区域节点
              Node *nCurRegNode = Node::create();
                  ParentNode_map->addChild(nCurRegNode,1,nRegIndex+1);
                  nCurRegNode->setPosition(ax::Vec2(0,0));
                  nCurRegNode->setAnchorPoint(ax::Vec2(0,0));
     }*/
}

void KgameWorld::DrawPrimitives_BigMap_house(int nRegIndex,
                                             int nPos,
                                             KRUImage* pPrimitives,
                                             unsigned int uGenre,
                                             int bpartNo,
                                             int nOrDer,
                                             int bSinglePlaneCoord,
                                             POINT nRegPiont)
{
    if (!ParentNode_Bigmap || pPrimitives == NULL || !g_GameWorld)
        return;

    char nRegKey[32] = {0};
    sprintf(nRegKey, "%d-%d-house", nRegPiont.x, nRegPiont.y);
    std::string nCurRegKey = nRegKey;
    int nStartPointX, nStartPointY;
    nStartPointX      = nRegPiont.x * 512;                                       // 起点x
    nStartPointY      = nRegPiont.y * 1024;                                      // 起点y
    Sprite* nBgRegSpr = (Sprite*)ParentNode_Bigmap->getChildByName(nCurRegKey);  // 如果区域节点存在

    if (nBgRegSpr)
    {
        // 获取屏幕坐标
        SubWorld[0].GetLocalPositionByMps(nStartPointX, nStartPointY, 0);
        nBgRegSpr->setPosition(ax::Vec2(nStartPointX, visibleSize.height - nStartPointY - 512));  // 设置画板的位置;
        return;
    }

    //	RenderTexture *nRegCanvas = RenderTexture::create(512,512,backend::PixelFormat::RGBA4);
    RenderTexture* nRegCanvas = RenderTexture::create(512, 512, backend::PixelFormat::RGBA4);
    nRegCanvas->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_Bigmap->addChild(nRegCanvas, -1);

    if (nRegCanvas)
    {                                         // 同一个区域的
        nRegCanvas->clear(255, 255, 255, 0);  // 半透明的方式清除画板
        // nRegCanvas->clear(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1());
        // 获取屏幕坐标
        SubWorld[0].GetLocalPositionByMps(nStartPointX, nStartPointY, 0);
        // 设置画板的位置
        nRegCanvas->setPosition(ax::Vec2(nStartPointX + 256, visibleSize.height - nStartPointY - 256));
        Sprite* nBgtestSpr = (Sprite*)this->getChildByName("spr_tree");

        if (!nBgtestSpr)
        {
            nBgtestSpr = Sprite::create();
            nBgtestSpr->setAnchorPoint(ax::Vec2(0, 0));
            nBgtestSpr->setPosition(ax::Vec2(0, 0));
            //            int nikey = std::hash<std::string>{}("spr_part");
            nBgtestSpr->setName("spr_part");
        }
        Rect ccRectPos = ax::Rect(0, 0, 64, 64);
        // nBgtestSpr->setTextureRect(ccRectPos,false,ccRectPos.size);
        // nRegCanvas->beginWithClear(255,255,255,100);//调用begin()开始在画布上绘制纹理
        Texture2D* bgCur = NULL;
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        // bgCur = ax::Director::getInstance()->getTextureCache()->addImage("TextureAtlas/default.png"); //
        KRUImage4* pTemp = (KRUImage4*)pPrimitives;
        nRegCanvas->begin();
        for (int i = 0; i < nPos; ++i, ++pTemp)
        {
            bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                 pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                 &nCavesWidth, &nCavesHeight);
            if (bgCur)
            {
                SubWorld[0].GetMpsByLocalPosition(NewscrX, NewscrY, 0);  // 计算偏移后新的像素坐标

                Rect CCRectZero = ax::Rect(0, 0, 0, 0);
                CCRectZero.size = bgCur->getContentSize();

                for (int j = 0; j < 2; j++)  // nPos
                {                            // 修改精灵的纹理
                    RECT rc;
                    rc.left   = 0;                    // 原来的位置
                    rc.top    = 0;                    // 原来的位置
                    rc.right  = pTemp->oImgRBPos.nX;  // 需要剪切的宽度
                    rc.bottom = pTemp->oImgRBPos.nY;  // 需要剪切的高度

                    if (rc.left < CCRectZero.origin.x)
                        rc.left = CCRectZero.origin.x;
                    if (rc.top < CCRectZero.origin.y)
                        rc.top = CCRectZero.origin.y;

                    if (rc.right > CCRectZero.size.width)
                        rc.right = CCRectZero.size.width;
                    if (rc.bottom > CCRectZero.size.height)
                        rc.bottom = CCRectZero.size.height;
                    CCRectZero = ax::Rect(pTemp->oImgLTPos.nX, pTemp->oImgLTPos.nY, rc.right, rc.bottom);

                    nBgtestSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                    int OffsetX = (NewscrX - nRegPiont.x * 512);
                    int OffsetY = 512 - (NewscrY - nRegPiont.y * 1024) / 2 - CCRectZero.size.height;
                    nBgtestSpr->setPosition(ax::Vec2(OffsetX, OffsetY));  // 相对整个画板的偏移
                    nBgtestSpr->setTexture(bgCur);

                    nBgtestSpr->visit();
                }
            }
        }
        nRegCanvas->end();

        if (!nBgRegSpr)
        {
            nBgRegSpr = Sprite::create();
            // nBgRegSpr->setVisible(false);//隐藏
            nBgRegSpr->setPosition(ax::Vec2(0, 0));
            nBgRegSpr->setAnchorPoint(ax::Vec2(0, 0));
            //			int nCurRegKey_tmp = std::hash<std::string>{}(nCurRegKey);
            nBgRegSpr->setName(nCurRegKey);
            ParentNode_Bigmap->addChild(nBgRegSpr, 1);
        }
        Texture2D* ncurSpr = NULL;
        ncurSpr            = nRegCanvas->getSprite()->getTexture();
        if (ncurSpr)
        {  // 合成后的大纹理
            nBgRegSpr->setTexture(ncurSpr);
            nBgRegSpr->setScaleY(-1);
            ccRectPos = ax::Rect(0, 0, 512, 512);
            nBgRegSpr->setTextureRect(ccRectPos, false, ccRectPos.size);
            nBgRegSpr->setPosition(ax::Vec2(nStartPointX, visibleSize.height - nStartPointY - 512));  // 整个spr的位置
        }
        ParentNode_Bigmap->removeChild(nRegCanvas, true);
        return;
    }
}
void KgameWorld::DrawPrimitives_BigMap_Tree(int nRegIndex,
                                            int nPos,
                                            KRUImage* pPrimitives,
                                            unsigned int uGenre,
                                            int bpartNo,
                                            int nOrDer,
                                            int bSinglePlaneCoord,
                                            POINT nRegPiont)
{
    if (!ParentNode_Bigmap || pPrimitives == NULL || !g_GameWorld)
        return;

    char nRegKey[32] = {0};
    sprintf(nRegKey, "%d-%d-tree", nRegPiont.x, nRegPiont.y);
    std::string nCurRegKey = nRegKey;
    int nStartPointX, nStartPointY;
    nStartPointX      = nRegPiont.x * 512;                                       // 起点x
    nStartPointY      = nRegPiont.y * 1024;                                      // 起点y
    Sprite* nBgRegSpr = (Sprite*)ParentNode_Bigmap->getChildByName(nCurRegKey);  // 如果区域节点存在

    if (nBgRegSpr)
    {
        // 获取屏幕坐标
        SubWorld[0].GetLocalPositionByMps(nStartPointX, nStartPointY, 0);
        nBgRegSpr->setPosition(ax::Vec2(nStartPointX, visibleSize.height - nStartPointY - 512));  // 设置画板的位置;
        return;
    }

    RenderTexture* nRegCanvas = RenderTexture::create(512, 512, backend::PixelFormat::RGBA4);
    nRegCanvas->setAnchorPoint(ax::Vec2(0, 0));
    ParentNode_Bigmap->addChild(nRegCanvas, -1);

    if (nRegCanvas)
    {                                         // 同一个区域的
        nRegCanvas->clear(255, 255, 255, 0);  // 半透明的方式清除画板
        // nRegCanvas->clear(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1());
        // 获取屏幕坐标
        SubWorld[0].GetLocalPositionByMps(nStartPointX, nStartPointY, 0);
        // 设置画板的位置
        nRegCanvas->setPosition(ax::Vec2(nStartPointX + 256, visibleSize.height - nStartPointY - 256));
        Sprite* nBgtestSpr = (Sprite*)this->getChildByName("spr_tree");

        if (!nBgtestSpr)
        {
            nBgtestSpr = Sprite::create();
            nBgtestSpr->setAnchorPoint(ax::Vec2(0, 0));
            nBgtestSpr->setPosition(ax::Vec2(0, 0));
            //			int nCurRegKey_tmp = std::hash<std::string>{}("spr_tree");
            nBgtestSpr->setName("spr_tree");
        }
        Rect ccRectPos = ax::Rect(0, 0, 64, 64);
        // nBgtestSpr->setTextureRect(ccRectPos,false,ccRectPos.size);
        // nRegCanvas->beginWithClear(255,255,255,100);//调用begin()开始在画布上绘制纹理
        Texture2D* bgCur = NULL;
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        // bgCur = ax::Director::getInstance()->getTextureCache()->addImage("TextureAtlas/default.png"); //
        KRUImage4* pTemp = (KRUImage4*)pPrimitives;
        nRegCanvas->begin();
        for (int i = 0; i < nPos; ++i, ++pTemp)
        {
            bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                 pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag, bSinglePlaneCoord,
                                 &nCavesWidth, &nCavesHeight);
            if (bgCur)
            {
                SubWorld[0].GetMpsByLocalPosition(NewscrX, NewscrY, 0);  // 计算偏移后新的像素坐标

                Rect CCRectZero = ax::Rect(0, 0, 0, 0);
                CCRectZero.size = bgCur->getContentSize();

                for (int j = 0; j < 2; j++)  // nPos
                {                            // 修改精灵的纹理

                    nBgtestSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                    int OffsetX = (NewscrX - nRegPiont.x * 512);
                    int OffsetY = 512 - (NewscrY - nRegPiont.y * 1024) / 2 - CCRectZero.size.height;
                    nBgtestSpr->setPosition(ax::Vec2(OffsetX, OffsetY));  // 相对整个画板的偏移
                    nBgtestSpr->setTexture(bgCur);
                    nBgtestSpr->visit();
                }
            }
        }
        nRegCanvas->end();

        if (!nBgRegSpr)
        {
            nBgRegSpr = Sprite::create();
            // nBgRegSpr->setVisible(false);//隐藏
            nBgRegSpr->setPosition(ax::Vec2(0, 0));
            nBgRegSpr->setAnchorPoint(ax::Vec2(0, 0));
            //			int nCurRegKey_tmp = std::hash<std::string>{}(nCurRegKey);
            nBgRegSpr->setName(nCurRegKey);
            ParentNode_Bigmap->addChild(nBgRegSpr, 1);
        }
        Texture2D* ncurSpr = NULL;
        ncurSpr            = nRegCanvas->getSprite()->getTexture();
        if (ncurSpr)
        {  // 合成后的大纹理
            nBgRegSpr->setTexture(ncurSpr);
            nBgRegSpr->setScaleY(-1);
            ccRectPos = ax::Rect(0, 0, 512, 512);
            nBgRegSpr->setTextureRect(ccRectPos, false, ccRectPos.size);
            nBgRegSpr->setPosition(ax::Vec2(nStartPointX, visibleSize.height - nStartPointY - 512));  // 整个spr的位置
        }
        ParentNode_Bigmap->removeChild(nRegCanvas, true);
        return;
    }
}

void KgameWorld::Draw_Part_map(int nRegIndex,
                               int nPos,
                               KRUImage* pPrimitives,
                               unsigned int uGenre,
                               int bpartNo,
                               int nOrDer,
                               int bSinglePlaneCoord,
                               POINT nRegPiont,
                               POINT nRegRbMpsPoint)
{
    if (nPos <= 0 || !ParentNode_map || pPrimitives == NULL || !g_GameWorld)
        return;
    char nRegKey[32] = {0};
    sprintf(nRegKey, "%d-%d-part", nRegPiont.x, nRegPiont.y);
    std::string nCurRegKey = nRegKey;
    int nStartPointX, nStartPointY;
    nStartPointX      = nRegPiont.x * 512;                                    // 起点x
    nStartPointY      = nRegPiont.y * 1024;                                   // 起点y
    Sprite* nBgRegSpr = (Sprite*)ParentNode_map->getChildByName(nCurRegKey);  // 如果区域节点存在

    if (nBgRegSpr)
    {
        // 获取屏幕坐标
        SubWorld[0].GetLocalPositionByMps(nStartPointX, nStartPointY, 0);
        nBgRegSpr->setPosition(ax::Vec2(nStartPointX, visibleSize.height - nStartPointY - 512));  // 设置画板的位置
        // nRegCanvas->setPosition(ax::Vec2(nStartPointX+256,visibleSize.height-nStartPointY-256));//设置画板的位置
        // nRegCanvas->setPosition(ax::Vec2(0+256,256));
        return;
    }
    // RenderTexture *nRegCanvas = RenderTexture::create(512,512); //创建一个区域画板
    RenderTexture* nRegCanvas = RenderTexture::create(512, 512, backend::PixelFormat::RGBA4);  //
    nRegCanvas->setAnchorPoint(ax::Vec2(0, 0));
    nRegCanvas->setAutoDraw(true);
    //	int nCurRegKey_tmp = std::hash<std::string>{}("canvas_part");
    nRegCanvas->setName("canvas_part");
    ParentNode_map->addChild(nRegCanvas, -1);

    if (nRegCanvas)
    {                                   // 同一个区域的
        nRegCanvas->clear(0, 0, 0, 0);  // 半透明的方式清除画板
        // nRegCanvas->clear(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1());
        // 获取屏幕坐标
        SubWorld[0].GetLocalPositionByMps(nStartPointX, nStartPointY, 0);
        // 设置画板的位置
        nRegCanvas->setPosition(ax::Vec2(nStartPointX + 256, visibleSize.height - nStartPointY - 256));
        Sprite* nBgtestSpr = NULL;  //(Sprite *)this->getChildByName("spr_part");
        Rect ccRectPos     = ax::Rect(0, 0, 64, 64);
        // nBgtestSpr->setTextureRect(ccRectPos,false,ccRectPos.size);
        // nRegCanvas->beginWithClear(255,255,255,100);//调用begin()开始在画布上绘制纹理
        Texture2D* bgCur = NULL;
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        // bgCur = ax::Director::getInstance()->getTextureCache()->addImage("TextureAtlas/default.png"); //
        KRUImagePart* pTemp = (KRUImagePart*)pPrimitives;
        nRegCanvas->begin();
        for (int i = 0; i < nPos; ++i /*,++pTemp*/)  //
        {
            bgCur = ccgetTxtData(pTemp[i].szImage, pTemp[i].nFrame, pTemp[i].oPosition.nX, pTemp[i].oPosition.nY,
                                 pTemp[i].oPosition.nZ, &NewscrX, &NewscrY, pTemp[i].bRenderFlag, bSinglePlaneCoord,
                                 &nCavesWidth, &nCavesHeight);
            Rect CCRectZero = ax::Rect(0, 0, 0, 0);
            SubWorld[0].GetMpsByLocalPosition(NewscrX, NewscrY, 0);  // 计算偏移后新的像素坐标
            if (bgCur)
            {
                /*nBgtestSpr = Sprite::create();
                nBgtestSpr->setColor(ax::Color3B::YELLOW);
                nBgtestSpr->setOpacity(105);
                nBgtestSpr->setContentSize(ax::Size(64,64));
                nBgtestSpr->setAnchorPoint(ax::Vec2(0,0));
                nBgtestSpr->setTag("spr_part");
                CCRectZero.size = bgCur->getContentSize();
                if (pTemp->oImgRBPos.nX > CCRectZero.size.width)
                        pTemp->oImgRBPos.nX = CCRectZero.size.width;
                if (pTemp->oImgRBPos.nY > CCRectZero.size.height)
                        pTemp->oImgRBPos.nY = CCRectZero.size.height;
                CCRectZero = ax::Rect(pTemp->oImgLTPos.nX,pTemp->oImgLTPos.nY,pTemp->oImgRBPos.nX,pTemp->oImgRBPos.nY);

                for (int j = 0;j<DRAW_COUNT;j++)//nPos
                {//修改精灵的纹理
                        nBgtestSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
                        int  OffsetX = (NewscrX-nRegPiont.x*512);
                        int  OffsetY = 512-(NewscrY-nRegPiont.y*1024)/2 - CCRectZero.size.height;
                        nBgtestSpr->setPosition(ax::Vec2(OffsetX,OffsetY));     //相对整个画板的偏移
                        nBgtestSpr->visit();
                }*/
                CCRectZero.size = bgCur->getContentSize();
                if (pTemp[i].oImgRBPos.nX > CCRectZero.size.width)
                    pTemp[i].oImgRBPos.nX = CCRectZero.size.width;
                if (pTemp[i].oImgRBPos.nY > CCRectZero.size.height)
                    pTemp[i].oImgRBPos.nY = CCRectZero.size.height;
                CCRectZero = ax::Rect(pTemp[i].oImgLTPos.nX, pTemp[i].oImgLTPos.nY, pTemp[i].oImgRBPos.nX,
                                      pTemp[i].oImgRBPos.nY);
                nBgtestSpr = Sprite::createWithTexture(bgCur, CCRectZero);  //,
                // nBgtestSpr->setColor(ax::Color3B::RED);
                // nBgtestSpr->setOpacity(105);
                nBgtestSpr->setAnchorPoint(ax::Vec2(0, 0));
                //				int nikey = std::hash<std::string>{}("spr_part");
                nBgtestSpr->setName("spr_part");
                int OffsetX = (NewscrX - nRegPiont.x * 512);
                int OffsetY = 512 - (NewscrY - nRegPiont.y * 1024) / 2 - CCRectZero.size.height;
                // for (int j = 0;j<DRAW_COUNT;j++)//nPos
                //{//修改精灵的纹理
                // nBgtestSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
                // nBgtestSpr->setTexture(bgCur);
                nBgtestSpr->setPosition(ax::Vec2(OffsetX, OffsetY));  // Chênh lệch so với toàn bộ bảng vẽ
                nBgtestSpr->visit();
                //}
            }
            else
            {  // Nếu không có kết cấu, hãy tô nó bằng một khối màu vàng
                nBgtestSpr = Sprite::create();
                nBgtestSpr->setColor(ax::Color3B::YELLOW);
                nBgtestSpr->setOpacity(255);
                nBgtestSpr->setContentSize(ax::Size(64, 64));
                nBgtestSpr->setAnchorPoint(ax::Vec2(0, 0));
                //				int nikey = std::hash<std::string>{}("spr_part");
                nBgtestSpr->setName("spr_part");
                CCRectZero.size = nBgtestSpr->getContentSize();
                if (pTemp[i].oImgRBPos.nX > CCRectZero.size.width)
                    pTemp[i].oImgRBPos.nX = CCRectZero.size.width;
                if (pTemp[i].oImgRBPos.nY > CCRectZero.size.height)
                    pTemp[i].oImgRBPos.nY = CCRectZero.size.height;
                CCRectZero =
                    ax::Rect(pTemp[i].oImgLTPos.nX, pTemp[i].oImgLTPos.nY, pTemp->oImgRBPos.nX, pTemp->oImgRBPos.nY);

                for (int j = 0; j < DRAW_COUNT; j++)  // nPos
                {                                     // 修改精灵的纹理
                    nBgtestSpr->setTextureRect(CCRectZero, false, CCRectZero.size);
                    int OffsetX = (NewscrX - nRegPiont.x * 512);
                    int OffsetY = 512 - (NewscrY - nRegPiont.y * 1024) / 2 - CCRectZero.size.height;
                    nBgtestSpr->setPosition(ax::Vec2(OffsetX, OffsetY));  // 相对整个画板的偏移
                    nBgtestSpr->visit();
                }
            }
        }
        nRegCanvas->end();
        Texture2D* ncurSpr = nRegCanvas->getSprite()->getTexture();
        if (ncurSpr)
        {  // 合成后的大纹理
            ccRectPos = ax::Rect(0, 0, 512, 512);
            if (nBgRegSpr)
            {
                ParentNode_map->removeChild(nBgRegSpr, true);
                nBgRegSpr = NULL;
            }
            nBgRegSpr = Sprite::createWithTexture(ncurSpr, ccRectPos);
            nBgRegSpr->setAnchorPoint(ax::Vec2(0, 0));
            //			int nCurRegKey_tmp = std::hash<std::string>{}(nCurRegKey);
            nBgRegSpr->setName(nCurRegKey);
            ParentNode_map->addChild(nBgRegSpr, 1);
            /*else
            {
                    nBgRegSpr->setTexture(ncurSpr);
                    nBgRegSpr->setTextureRect(ccRectPos,false,ccRectPos.size);
            }*/
            nBgRegSpr->setScaleY(-1);
            // 默认的情况下，通过CCRenderTexture得到的纹理是倒立的，这里需要做一下翻转
            nBgRegSpr->setPosition(ax::Vec2(nStartPointX, visibleSize.height - nStartPointY - 512));  // 整个spr的位置
        }
        ParentNode_map->removeChildByName("canvas_part", true);
        // ParentNode_map->removeChild(nRegCanvas,true);
        // CC_SAFE_DELETE(nRegCanvas);
        return;
    }
}

void KgameWorld::_DrawPrimitives_map(int nRegIndex,
                                     int nPos,
                                     KRUImage* pPrimitives,
                                     unsigned int uGenre,
                                     int bpartNo,
                                     int nOrDer,
                                     int bSinglePlaneCoord,
                                     POINT nRegPiont,
                                     POINT nRegRbMpsPoint)
{  // 绘画地图

    if (nPos <= 0 || !ParentNode_map || pPrimitives == NULL || !g_GameWorld)
        return;
    char nRegKey[32] = {0};
    sprintf(nRegKey, "%d-%d-dimian", nRegPiont.x, nRegPiont.y);
    std::string nCurRegKey = nRegKey;
    int nStartPointX, nStartPointY;
    nStartPointX      = nRegPiont.x * 512;                                    // 起点x
    nStartPointY      = nRegPiont.y * 1024;                                   // 起点y
    Sprite* nBgRegSpr = (Sprite*)ParentNode_map->getChildByName(nCurRegKey);  // 如果区域节点存在

    if (nBgRegSpr)
    {
        // 获取屏幕坐标
        SubWorld[0].GetLocalPositionByMps(nStartPointX, nStartPointY, 0);
        nBgRegSpr->setPosition(ax::Vec2(nStartPointX, visibleSize.height - nStartPointY - 512));  // 设置画板的位置
        // nRegCanvas->setPosition(ax::Vec2(nStartPointX+256,visibleSize.height-nStartPointY-256));//设置画板的位置
        // nRegCanvas->setPosition(ax::Vec2(0+256,256));
        return;
    }
    RenderTexture* nRegCanvas = RenderTexture::create(512, 512, backend::PixelFormat::RGBA4);  // 0x88F0
    // nRegCanvas->setClearColor()
    nRegCanvas->setAnchorPoint(ax::Vec2(0, 0));
    //   int nCurRegKey_tmp = std::hash<std::string>{}("canvas_full");
    nRegCanvas->setName("canvas_full");
    ParentNode_map->addChild(nRegCanvas, -2);

    if (nRegCanvas)
    {                                   // 同一个区域的
        nRegCanvas->clear(0, 0, 0, 0);  // 半透明的方式清除画板
        // nRegCanvas->clear(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1());
        // 获取屏幕坐标
        SubWorld[0].GetLocalPositionByMps(nStartPointX, nStartPointY, 0);
        // 设置画板的位置
        // nRegCanvas->setPosition(ax::Vec2(nStartPointX,visibleSize.height-nStartPointY));
        nRegCanvas->setPosition(ax::Vec2(nStartPointX + 256, visibleSize.height - nStartPointY - 256));
        Sprite* nBgtestSpr = NULL;  // (Sprite *)this->getChildByName("spr_draw");
        Rect ccRectPos     = ax::Rect(0, 0, 64, 64);
        // nBgtestSpr->setTextureRect(ccRectPos,false,ccRectPos.size);
        // nRegCanvas->beginWithClear(255,255,255,100);//调用begin()开始在画布上绘制纹理
        Texture2D* bgCur = NULL;
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        ;
        // bgCur = ax::Director::getInstance()->getTextureCache()->addImage("TextureAtlas/default.png"); //
        nRegCanvas->begin();
        for (int i = 0; i < nPos; i++)  // nPos
        {
            bgCur = ccgetTxtData(pPrimitives[i].szImage, pPrimitives[i].nFrame, pPrimitives[i].oPosition.nX,
                                 pPrimitives[i].oPosition.nY, pPrimitives[i].oPosition.nZ, &NewscrX, &NewscrY,
                                 pPrimitives[i].bRenderFlag, bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
            if (bgCur)
            {
                SubWorld[0].GetMpsByLocalPosition(NewscrX, NewscrY, 0);  // 计算偏移后新的像素坐标
                Rect CCRectTemp = ax::Rect(0, 0, 0, 0);
                CCRectTemp.size = bgCur->getContentSize();
                nBgtestSpr      = Sprite::createWithTexture(bgCur, CCRectTemp);
                nBgtestSpr->setAnchorPoint(ax::Vec2(0, 0));
                // nBgtestSpr->setPosition(ax::Vec2(0,0));
                //			int nCurRegKey_tmp = std::hash<std::string>{}("spr_draw");
                nBgtestSpr->setName("spr_draw");
                // for (int j = 0;j<DRAW_COUNT;j++)//nPos
                //{//修改精灵的纹理
                // nBgtestSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
                // nBgtestSpr->setTexture(bgCur);
                int OffsetX = (NewscrX - nRegPiont.x * 512);
                int OffsetY = 512 - (NewscrY - nRegPiont.y * 1024) / 2 - CCRectTemp.size.height;
                nBgtestSpr->setPosition(ax::Vec2(OffsetX, OffsetY));  // 相对整个画板的偏移
                nBgtestSpr->visit();
                //}
            }
        }
        nRegCanvas->end();

        Texture2D* nregSpr = nRegCanvas->getSprite()->getTexture();

        if (nregSpr)
        {  // 合成后的大纹理
            ccRectPos = ax::Rect(0, 0, 512, 512);
            if (nBgRegSpr)
            {
                ParentNode_map->removeChild(nBgRegSpr, true);
                nBgRegSpr = NULL;
            }

            // nBgRegSpr=Sprite::create();
            // nBgRegSpr->setDisplayFrame(nRegCanvas.getSprite()->getSpriteFrame());//这里必须重新创建精灵，否则会报错

            nBgRegSpr = Sprite::createWithTexture(nregSpr, ccRectPos);
            nBgRegSpr->setAnchorPoint(ax::Vec2(0, 0));
            //			int nCurRegKey_tmp = std::hash<std::string>{}(nCurRegKey);
            nBgRegSpr->setName(nCurRegKey);
            ParentNode_map->addChild(nBgRegSpr, -2);
            /*  else
              {
                  nBgRegSpr->setTextureRect(ccRectPos,false,ccRectPos.size);
                  nBgRegSpr->setTexture(ncurSpr);
              }*/
            nBgRegSpr->setScaleY(-1);
            nBgRegSpr->setPosition(ax::Vec2(nStartPointX, visibleSize.height - nStartPointY - 512));  // 整个spr的位置
        }
        // ParentNode_map->removeChild(nRegCanvas,true);  nRegCanvas->setTag("canvas_full");
        ParentNode_map->removeChildByName("canvas_full", true);
        return;
    }

    bool iscolse = true;
    if (iscolse)
        return;
    //////////////////////////////////////////////////////////取消以下的
    Node* nRegNode = ParentNode_map->getChildByName(nCurRegKey);  // 如果区域节点存在

    if (!nRegNode)
    {  // 如果不存在就 创建
        nRegNode = Node::create();
        //	  int nCurRegKey_tmp = std::hash<std::string>{}(nCurRegKey);
        nRegNode->setName(nCurRegKey);
        ParentNode_map->addChild(nRegNode, 1);
        nRegNode->setPosition(ax::Vec2(0, 0));
        nRegNode->setAnchorPoint(ax::Vec2(0, 0));
    }

    if (nRegNode)
    {  // 区域 已经存在

        Texture2D* bgCur = NULL;
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        Rect CCRectZero = ax::Rect(0, 0, 0, 0);
        /*if (RegData)
        {//用于  MAP_NODE_LUMIAN 转向 MAP_NODE_LUMIAN_A 区域时候调用
           RegData[nRegIndex-1].nDrawKind = bpartNo;
        }*/
        switch (bpartNo)
        {
        case MAP_NODE_LUMIAN_A:
        {  // 一些零散的路面 草堆
            Node* nPartNode = nRegNode->getChildByTag(MAP_NODE_LUMIAN_A);
            if (!nPartNode)
            {
                nPartNode = Node::create();
                nRegNode->addChild(nPartNode, 1, MAP_NODE_LUMIAN_A);
                nPartNode->setPosition(ax::Vec2(0, 0));
                nPartNode->setAnchorPoint(ax::Vec2(0, 0));
            }
            if (nPartNode)
            {  // 如果这个节点存在
                char nKey[32] = {0};

                for (int i = 0; i < nPos; i++)
                {
                    // int nidx =  FindFree(nRegIndex-1);//最小从1开始
                    Sprite* sprNode = NULL;
                    // 已经存字
                    // if (nidx>0)
                    {
                        // AddIndex(nRegIndex-1,nidx);
                        sprintf(nKey, "%s-%d", nCurRegKey.c_str(), i + 1);
                        std::string nCurKey = nKey;
                        sprNode             = (Sprite*)nPartNode->getChildByName(nCurKey);
                    }

                    if (!sprNode)
                    {
                        sprintf(nKey, "%s-%d", nCurRegKey.c_str(), i + 1);
                        std::string nCurKey = nKey;
                        Sprite* sprNode     = Sprite::create();
                        nPartNode->addChild(sprNode, 1);
                        //						   int nCurKey_tmp =
                        // std::hash<std::string>{}(nCurKey);
                        sprNode->setName(nCurKey);
                        sprNode->setPosition(ax::Vec2(0, 0));
                        sprNode->setAnchorPoint(ax::Vec2(0, 0));
                    }
                    // char msg[128];
                    // sprintf(msg,"%d/%d,%s\n%s",i,nPos,nKey,pPrimitives[i].szImage);
                    // messageBox(msg,"MAP_NODE_LUMIAN_A");
                    if (sprNode)
                    {  // 如果存在就更新节点spr
                        // sprNode->setVisible(true);
                        bgCur =
                            ccgetTxtData(pPrimitives[i].szImage, pPrimitives[i].nFrame, pPrimitives[i].oPosition.nX,
                                         pPrimitives[i].oPosition.nY, pPrimitives[i].oPosition.nZ, &NewscrX, &NewscrY,
                                         pPrimitives[i].bRenderFlag, bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
                        if (bgCur)
                        {  // 修改精灵的纹理
                            CCRectZero.size = bgCur->getContentSize();
                            sprNode->setTextureRect(CCRectZero, false, CCRectZero.size);
                            float x, y;
                            nPartNode->getPosition(&x, &y);  // 屏幕坐标
                            NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                            sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                            sprNode->setTexture(bgCur);
                            // sprNode->setLocalZOrder(nOrDer);
                        }
                    }
                }
            }
        }
        break;
        case MAP_NODE_PART:
        {  // 一张一张的绘画 上层路面
            // 需要再创建一个part节点
            Node* nPartNode = nRegNode->getChildByTag(MAP_NODE_PART);

            if (!nPartNode)
            {
                nPartNode = Node::create();
                nRegNode->addChild(nPartNode, 2, MAP_NODE_PART);
                nPartNode->setPosition(ax::Vec2(0, 0));
                nPartNode->setAnchorPoint(ax::Vec2(0, 0));
            }

            if (nPartNode)
            {  // 如果这个节点存在
                char nKey[32]       = {0};
                KRUImagePart* pTemp = (KRUImagePart*)pPrimitives;

                for (int i = 0; i < nPos; ++i, ++pTemp)
                {
                    // int nidx =  FindPartFree(nRegIndex-1);//最小从1开始
                    Sprite* sprNode = NULL;
                    // 已经存字
                    // if (nidx>0)
                    {
                        // AddPartIndex(nRegIndex-1,nidx);
                        // sprintf(nKey,"%d-%d",nRegIndex,i+1);
                        sprintf(nKey, "%s-%d", nCurRegKey.c_str(), i + 1);
                        std::string nCurKey = nKey;
                        sprNode             = (Sprite*)nPartNode->getChildByName(nCurKey);
                    }
                    if (!sprNode)
                    {
                        // sprintf(nKey,"%d-%d",nRegIndex,i+1);
                        sprintf(nKey, "%s-%d", nCurRegKey.c_str(), i + 1);
                        std::string nCurKey = nKey;
                        Sprite* sprNode     = Sprite::create();
                        nPartNode->addChild(sprNode, 1);
                        //						   int nCurKey_tmp =
                        // std::hash<std::string>{}(nCurKey);
                        sprNode->setName(nCurKey);
                        sprNode->setPosition(ax::Vec2(0, 0));
                        sprNode->setAnchorPoint(ax::Vec2(0, 0));
                    }

                    if (sprNode)
                    {  // 如果存在就更新节点spr
                        sprNode->setVisible(true);
                        bgCur = ccgetTxtData(pTemp->szImage, pTemp->nFrame, pTemp->oPosition.nX, pTemp->oPosition.nY,
                                             pTemp->oPosition.nZ, &NewscrX, &NewscrY, pTemp->bRenderFlag,
                                             bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
                        if (bgCur)
                        {  // 修改精灵的纹理
                            CCRectZero.size = bgCur->getContentSize();
                            RECT rc;
                            rc.left   = 0;                    // 原来的位置
                            rc.top    = 0;                    // 原来的位置
                            rc.right  = pTemp->oImgRBPos.nX;  // 需要剪切的宽度
                            rc.bottom = pTemp->oImgRBPos.nY;  // 需要剪切的高度

                            if (rc.left < CCRectZero.origin.x)
                                rc.left = CCRectZero.origin.x;
                            if (rc.top < CCRectZero.origin.y)
                                rc.top = CCRectZero.origin.y;

                            if (rc.right > CCRectZero.size.width)
                                rc.right = CCRectZero.size.width;
                            if (rc.bottom > CCRectZero.size.height)
                                rc.bottom = CCRectZero.size.height;

                            CCRectZero = ax::Rect(pTemp->oImgLTPos.nX, pTemp->oImgLTPos.nY, rc.right, rc.bottom);
                            sprNode->setTextureRect(CCRectZero, false, CCRectZero.size);  //,false,CCRectZero.size
                            float x, y;
                            nPartNode->getPosition(&x, &y);  // 屏幕坐标
                            NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                            sprNode->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                            sprNode->setTexture(bgCur);
                            // sprNode->setLocalZOrder(nOrDer);
                        }
                    }
                }
            }
        }
        break;
        default:
            break;
        }
    }
}

void KgameWorld::DrawPrimitives_obj(int nObjIndex,
                                    int nPos,
                                    KRUImage* pPrimitives,
                                    unsigned int uGenre,
                                    int bpartNo,
                                    int nOrDer,
                                    int bSinglePlaneCoord,
                                    ax::Color3B nObjColor,
                                    char* nObjName,
                                    char* _clientObjName)
{  // 地上的物品

    if (!g_GameWorld || !ParentNode_obj || pPrimitives == NULL || nObjIndex <= 0 || nObjIndex >= MAX_OBJECT)
        return;

    Node* nPartCurNode = NULL;
    nPartCurNode       = ParentNode_obj->getChildByTag(nObjIndex);

    if (nPartCurNode == NULL)
        return;

    if (pPrimitives->szImage[0])
    {
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        Texture2D* bgCur = NULL;
        Rect CCRectZero  = ax::Rect(0, 0, 0, 0);
        bgCur            = ccgetTxtData(pPrimitives->szImage, pPrimitives->nFrame, pPrimitives->oPosition.nX,
                                        pPrimitives->oPosition.nY, pPrimitives->oPosition.nZ, &NewscrX, &NewscrY,
                                        pPrimitives->bRenderFlag, bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
        if (bgCur)
        {  // 修改精灵的纹理
            char msg[128] = {0};
            float x, y;
            (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
            CCRectZero.size  = bgCur->getContentSize();
            Sprite* nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(1));
            if (!nPartSpr)
            {
                nPartSpr = Sprite::createWithTexture(bgCur);
                nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                nPartCurNode->addChild(nPartSpr, 1, 1);
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
                nPartSpr->setLocalZOrder(nOrDer);
                return;
            }
            nPartSpr->setTexture(bgCur);
            nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);
            NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
            nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));  // 相对偏移
            nPartSpr->setLocalZOrder(nOrDer);
        }

        if (nObjName)
        {
            Label* nPartlabel = (Label*)(nPartCurNode->getChildByTag(2));
            if (!nPartlabel)
            {
                Size nTempSize;
                nTempSize.width = 128;
                if (_clientlanguage != 1)
                    nTempSize.height = 28;
                else
                    nTempSize.height = 14;
                nPartlabel =
                    Label::createWithTTF("", UI_GAME_FONT_DEFAULT_VN, 13, nTempSize, TextHAlignment::CENTER);  // Arial

                // stroke only label
                nPartlabel->enableOutline(ax::Color4B::BLACK, 1.5);
                nPartlabel->setPosition(ax::Vec2(0, 30));
                nPartCurNode->addChild(nPartlabel, 1, 2);
            }
            nPartlabel->setColor(nObjColor);
            if (nPartlabel->getStringLength())  // 只绘画一次
                return;
            char nObjTempNane[128];
            sprintf(nObjTempNane, "%s", nObjName);
            if (_clientlanguage != 1 && _clientObjName)
            {
                strcat(nObjTempNane, "\n");
                strcat(nObjTempNane, _clientObjName);
            }
#ifdef WIN32
            nPartlabel->setString(UTEXT(nObjTempNane, 1).c_str());
#else
            nPartlabel->setString(UTEXT(nObjTempNane, 1).c_str());
#endif
        }
    }
}

Texture2D* KgameWorld::ccgetTxtData_g(char* sprPath,
                                      int nFramIndex,
                                      int nMpsX,
                                      int nMpsY,
                                      int nMpsZ,
                                      int* mScreeX,
                                      int* mScreeY,
                                      int bRenderFlag,
                                      int bSinglePlaneCoord,
                                      int* mHeadw,
                                      int* mHeadh)
{
    if (sprPath == NULL)
        return NULL;

    char nSprName[128] = {0};
    ZeroMemory(nSprName, sizeof(nSprName));
    sprintf(nSprName, "%s", sprPath);
    g_StrLower(nSprName);
    int m_nWidth, m_nHeight, nFrams;
    Texture2D* bgCur = NULL;
    SPRFRAMSINFO nSprInfo;
    ZeroMemory(&nSprInfo, sizeof(nSprInfo));
    // if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
    bgCur = _getinidata.getinidata_one(nSprName, nFramIndex, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);

    if (bgCur == NULL)
        return NULL;

    *mHeadw = nSprInfo.pHwidth;
    *mHeadh = nSprInfo.pHheight;

#define CENTERX 160
#define CENTERY 192
    // 转化成象素坐标
    // SubWorld[0].GetMpsByLocalPosition(nMpsX,nMpsY,0);

    int nX = nMpsX;  // 位置
    int nY = nMpsY;  // 位置

    if (bSinglePlaneCoord == false)  // 转换成屏幕坐标
        SubWorld[0].GetLocalPositionByMps(nX, nY, nMpsZ);

    if (bRenderFlag & RUIMAGE_RENDER_FLAG_REF_SPOT)
    {  // 渲染模式
        //****to be modify****
        int nCenterX = nSprInfo.pHCenterX;  // 重心的水平位移
        int nCenterY = nSprInfo.pHCenterY;  // 重心的垂直位移

        if (nCenterX || nCenterY)
        {
            nX -= nCenterX;
            nY -= nCenterY;
        }
        else if (nSprInfo.pHwidth > CENTERX)
        {
            nX -= CENTERX;
            nY -= CENTERY;
        }
    }

    if ((bRenderFlag & RUIMAGE_RENDER_FLAG_FRAME_DRAW) == 0)
    {  // 帧内有偏移的
        nX += nSprInfo.pFoffsetX;
        nY += nSprInfo.pFoffsetY;
    }
    // 开始裁剪

    *mScreeX = nX;
    *mScreeY = nY;

    return bgCur;
}

Texture2D* KgameWorld::ccgetTxtData(char* sprPath,
                                    int nFramIndex,
                                    int nMpsX,
                                    int nMpsY,
                                    int nMpsZ,
                                    int* mScreeX,
                                    int* mScreeY,
                                    int bRenderFlag,
                                    int bSinglePlaneCoord,
                                    int* mHeadw,
                                    int* mHeadh)
{
    if (sprPath == NULL)
        return NULL;

    char nSprName[128] = {0};
    ZeroMemory(nSprName, sizeof(nSprName));
    sprintf(nSprName, "%s", sprPath);
    g_StrLower(nSprName);
    int m_nWidth, m_nHeight, nFrams;
    Texture2D* bgCur = NULL;
    SPRFRAMSINFO nSprInfo;
    ZeroMemory(&nSprInfo, sizeof(nSprInfo));
    // if ((bgCur = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))==NULL)
    bgCur = _getinidata.getinidata_one(nSprName, nFramIndex, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);

    if (bgCur == NULL)
        return NULL;

    *mHeadw = nSprInfo.pHwidth;
    *mHeadh = nSprInfo.pHheight;

#define CENTERX 160
#define CENTERY 192

    int nX = nMpsX;  // 位置
    int nY = nMpsY;  // 位置

    if (bSinglePlaneCoord == false)  // 转换成屏幕坐标
        SubWorld[0].GetLocalPositionByMps(nX, nY, nMpsZ);

    if (bRenderFlag & RUIMAGE_RENDER_FLAG_REF_SPOT)
    {  // 渲染模式
        //****to be modify****
        int nCenterX = nSprInfo.pHCenterX;  // 重心的水平位移
        int nCenterY = nSprInfo.pHCenterY;  // 重心的垂直位移

        if (nCenterX || nCenterY)
        {
            nX -= nCenterX;
            nY -= nCenterY;
        }
        else if (nSprInfo.pHwidth > CENTERX)
        {
            nX -= CENTERX;
            nY -= CENTERY;
        }
    }

    if ((bRenderFlag & RUIMAGE_RENDER_FLAG_FRAME_DRAW) == 0)
    {  // 帧内有偏移的
        nX += nSprInfo.pFoffsetX;
        nY += nSprInfo.pFoffsetY;
    }
    // 开始裁剪

    *mScreeX = nX;
    *mScreeY = nY;

    return bgCur;
}
// 偏色设置
void KgameWorld::setadjustColor(int nIndex, Sprite* nResSpr)
{
    if (nResSpr == NULL)
        return;
    // freeze={255,127,0};//{250,255,0};
    // stun={50,30,40};
    // poison={50,128,200};
    // njustColor ={250,255,0};
    switch (Npc[nIndex].m_AdjustColorKind)
    {
    case kind_color_freeze:  // 冰
    {
        ax::Color3B njustColor = {255, 127, 0};
        nResSpr->setColor(njustColor);
    }
    break;
    case kind_color_stun:  // 晕
    {
        ax::Color3B njustColor = {50, 30, 40};
        nResSpr->setColor(njustColor);
    }
    break;
    case kind_color_poison:  // 毒
    {
        ax::Color3B njustColor = {50, 128, 200};
        nResSpr->setColor(njustColor);
    }
    break;
    default:
    {
        // ax::Color3B njustColor ={255,255,255};//{250,255,0};
        nResSpr->setColor(ax::Color3B::WHITE);
    }
    break;
    }
}

// 头顶状态绘画
void KgameWorld::DrawPrimitives_MenuState(int nNpcIndex,
                                          int nstateIdx,
                                          int nPos,
                                          KRUImage* pPrimitives,
                                          unsigned int uGenre,
                                          int bpartNo,
                                          int nOrDer,
                                          int bSinglePlaneCoord)
{
    if (!g_GameWorld || !ParentNode_npc || pPrimitives == NULL || nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;

    Node* nPartNpcNode = NULL;
    char nTempKey[64];
    sprintf(nTempKey, "npc_%d", nNpcIndex);
    std::string _npcKey = nTempKey;
    nPartNpcNode        = ParentNode_npc->getChildByName(_npcKey);  // 这个NPC的父节点 子节点有很多部位
    if (nPartNpcNode == NULL)
        return;

    if (pPrimitives[nPos].szImage[0])
    {
        // KAutoCriticalSection AutoLock(m_GameProcessLock);
        Texture2D* bgCur = NULL;
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        Rect CCRectZero = ax::Rect(0, 0, 0, 0);
        char nKey[32];
        sprintf(nKey, "state_%d_menu", nstateIdx);
        std::string m_stateKey = nKey;
        bgCur = ccgetTxtData(pPrimitives[nPos].szImage, pPrimitives[nPos].nFrame, pPrimitives[nPos].oPosition.nX,
                             pPrimitives[nPos].oPosition.nY, pPrimitives[nPos].oPosition.nZ, &NewscrX, &NewscrY,
                             pPrimitives[nPos].bRenderFlag, bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
        if (bgCur)
        {  // 修改精灵的纹理
            Sprite* nPartSpr = (Sprite*)(nPartNpcNode->getChildByName(m_stateKey));
            if (!nPartSpr)
            {  // 不存在就创建
                nPartSpr = Sprite::createWithTexture(bgCur);
                nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                //				 int m_stateKey_tmp = std::hash<std::string>{}(m_stateKey);
                nPartSpr->setName(m_stateKey);
                // nPartSpr->setContentSize(nsprSize);
                nPartNpcNode->addChild(nPartSpr, 50);
            }
            CCRectZero.size = bgCur->getContentSize();
            nPartSpr->setTexture(bgCur);
            nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);
            float x, y;
            nPartNpcNode->getPosition(&x, &y);  // 屏幕坐标

            NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;  //;
            nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
        }
    }
}
// 状态的绘画
void KgameWorld::DrawPrimitives_State(int nNpcIndex,
                                      int nstateIdx,
                                      int nPos,
                                      KRUImage* pPrimitives,
                                      unsigned int uGenre,
                                      int bpartNo,
                                      int nOrDer,
                                      int bSinglePlaneCoord)
{
    if (!g_GameWorld || !ParentNode_npc || pPrimitives == NULL || nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;

    Node* nPartNpcNode = NULL;
    char nTempKey[64];
    sprintf(nTempKey, "npc_%d", nNpcIndex);
    std::string _npcKey = nTempKey;
    nPartNpcNode        = ParentNode_npc->getChildByName(_npcKey);  // 这个NPC的父节点 子节点有很多部位
    if (nPartNpcNode == NULL)
        return;
    /*Node *nPartNode  = nPartNpcNode->getChildByTag(UI_EQ_PART_NPC_STATE);//状态的父节点
    if (!nPartNode)
    {
            nPartNode = Node::create();
            nPartNode->setAnchorPoint(ax::Vec2(0,0));
            nPartNode->setPosition(ax::Vec2(0,0));
            nPartNpcNode->addChild(nPartNode,2,UI_EQ_PART_NPC_STATE);
    }*/

    if (pPrimitives[nPos].szImage[0])
    {
        // KAutoCriticalSection AutoLock(m_GameProcessLock);
        Texture2D* bgCur = NULL;
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        Rect CCRectZero = ax::Rect(0, 0, 0, 0);
        char nKey[32];
        sprintf(nKey, "state_%d", nstateIdx);
        std::string m_stateKey = nKey;
        bgCur = ccgetTxtData(pPrimitives[nPos].szImage, pPrimitives[nPos].nFrame, pPrimitives[nPos].oPosition.nX,
                             pPrimitives[nPos].oPosition.nY, pPrimitives[nPos].oPosition.nZ, &NewscrX, &NewscrY,
                             pPrimitives[nPos].bRenderFlag, bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
        if (bgCur)
        {  // 修改精灵的纹理
            Sprite* nPartSpr = (Sprite*)(nPartNpcNode->getChildByName(m_stateKey));
            if (!nPartSpr)
            {  // 不存在就创建
                nPartSpr = Sprite::createWithTexture(bgCur);
                nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                //				 int m_stateKey_tmp = std::hash<std::string>{}(m_stateKey);
                nPartSpr->setName(m_stateKey);
                // nPartSpr->setContentSize(nsprSize);
                nPartNpcNode->addChild(nPartSpr, 2);
            }
            CCRectZero.size = bgCur->getContentSize();
            nPartSpr->setTexture(bgCur);
            nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);
            float x, y;
            nPartNpcNode->getPosition(&x, &y);  // 屏幕坐标

            NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;  //;
            nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
            switch (bpartNo)
            {
            case STATE_PART_HEAD:
            {  // 头部状态
                // nPartSpr->setLocalZOrder(16);
                nPartSpr->setLocalZOrder(16);
            }
            break;
            case STATE_PART_BODY_Q:
            {  // 身体前状态
                nPartSpr->setLocalZOrder(20);
            }
            break;
            case STATE_PART_BODY_H:
            {  // 身体后状态
                nPartSpr->setLocalZOrder(-1);
            }
            break;
            case STATE_PART_FOOT:
            {  // 脚底状态
                nPartSpr->setLocalZOrder(-1);
                // nPartNode->setLocalZOrder(-1);
            }
            break;
            default:
                break;
            }
        }
    }
}

void KgameWorld::DrawPrimitives_NpcBlood(int nNpcIndex,
                                         int bloodindex,
                                         char* strBloodNo,
                                         int nNpcMpsX,
                                         int nNpcMpsY,
                                         int nOffsetY,
                                         bool isClear)
{
    if (!ParentNode_npcblood || !g_GameWorld || nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;

    // messageBox("冒血中","冒血中");

    Node* NpcBloopParentNode = NULL;
    char nKey[64];
    sprintf(nKey, "blood_%d", nNpcIndex);
    std::string nTempKey = nKey;

    if (bloodindex == -1)
    {
        ParentNode_npcblood->removeChildByName(nTempKey, true);
        return;
    }
    NpcBloopParentNode = ParentNode_npcblood->getChildByName(nTempKey);
    if (!NpcBloopParentNode)
    {  // 这个NPC的冒血父节点
        NpcBloopParentNode = Node::create();
        NpcBloopParentNode->setAnchorPoint(ax::Vec2(0, 0));
        //		int nTempKey_tmp = std::hash<std::string>{}(nTempKey);
        NpcBloopParentNode->setName(nTempKey);
        ParentNode_npcblood->addChild(NpcBloopParentNode, 1);
    }
    Label* pBloodLabel = NULL;
    if (isClear)
    {
        NpcBloopParentNode->removeChildByTag(bloodindex + 1, true);
        return;
    }

    pBloodLabel = (Label*)NpcBloopParentNode->getChildByTag(bloodindex + 1);
    if (!pBloodLabel)
    {
        pBloodLabel = Label::createWithTTF(strBloodNo, "fonts/number_16_r.fnt", 10);
        NpcBloopParentNode->addChild(pBloodLabel, bloodindex + 1, bloodindex + 1);
    }
    // char nBloodInfo[32];
    // sprintf(nBloodInfo,"%s",strBloodNo);
    // pBloodLabel->setColor(ax::Color3B::GREEN);
    pBloodLabel->setString(strBloodNo);
    int NewscrX = nNpcMpsX, NewscrY = nNpcMpsY;
    SubWorld[0].GetLocalPositionByMps(NewscrX, NewscrY, 0);  // 像素坐标－－－＞屏幕坐标
    pBloodLabel->setPosition(ax::Vec2(NewscrX, visibleSize.height - NewscrY + nOffsetY));  // GL坐标
}

void KgameWorld::DrawPrimitives_NpcOther(int nNpcIndex, int m_Kind, bool isClear)
{
    if (!ParentNode_npcother || !g_GameWorld || nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;

    char nKey[64];
    sprintf(nKey, "other_%d", nNpcIndex);
    std::string otherKey = nKey;
    Node* nNpcOtherNode  = NULL;
    nNpcOtherNode        = (Node*)ParentNode_npcother->getChildByName(otherKey);
    if (!nNpcOtherNode)
    {
        // 姓名 称号 血条的父亲节点
        nNpcOtherNode = Node::create();
        nNpcOtherNode->setAnchorPoint(ax::Vec2(0, 0));
        //		int otherKey_tmp = std::hash<std::string>{}(otherKey);
        nNpcOtherNode->setName(otherKey);
        ParentNode_npcother->addChild(nNpcOtherNode, 1);
    }
    if (relation_enemy == NpcSet.GetRelation(nNpcIndex, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
        (m_Kind == kind_player || m_Kind == kind_partner) &&
        Npc[nNpcIndex].m_Hide.nTime > 0)  // 和玩家有敌对关系并且隐藏时间大于的 不显示
    {
        nNpcOtherNode->setVisible(false);
        return;
    }
    int NewscrX, NewscrY, mapidx;
    Npc[nNpcIndex].GetMpsPos(&NewscrX, &NewscrY, &mapidx);
    SubWorld[0].GetLocalPositionByMps(NewscrX, NewscrY, 0);  // 像素坐标－－－＞屏幕坐标
    nNpcOtherNode->setPosition(ax::Vec2(NewscrX, visibleSize.height - NewscrY + 10));  // GL坐标

    if (nNpcOtherNode)
    {  // Bắt đầu vẽ thanh trạng thái tên NPC_NAME
        if (relation_enemy == NpcSet.GetRelation(nNpcIndex, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
            (m_Kind == kind_player || m_Kind == kind_partner) &&
            Npc[nNpcIndex].m_Hide.nTime > 0)  // 和玩家有敌对关系并且隐藏时间大于的 不显示
        {
            nNpcOtherNode->setVisible(false);
            return;
        }

        nNpcOtherNode->setVisible(true);

        float nCurLife    = Npc[nNpcIndex].m_CurrentLife;
        float nCurMaxLife = Npc[nNpcIndex].m_CurrentLifeMax;
        float nCurScaleX  = nCurLife / nCurMaxLife;
        int nOffHeight    = Npc[nNpcIndex].GetNpcPate();
        int xMps, yMps, nMap1;
        Npc[nNpcIndex].GetMpsPos(&xMps, &yMps, &nMap1);

        // 创建一个层
        // Size nTempSize;
        // nTempSize.width  = 100;
        // nTempSize.height = 14;
        Label* objnamelabel = NULL;
        // Node *objautoplaylabel = NULL;
        // Node *ReBornLabel = NULL;
        Label* itemTanlabel = NULL;
        objnamelabel        = (Label*)nNpcOtherNode->getChildByTag(NPC_NAME);
        Sprite* npcLifespr  = NULL;
        npcLifespr          = (Sprite*)nNpcOtherNode->getChildByTag(NPC_LIFE);
        if (!npcLifespr)
        {
            npcLifespr = Sprite::create("ui/state/life_normal.png");  // Arial
            nNpcOtherNode->addChild(npcLifespr, 1, NPC_LIFE);
        }

        char nTempName[128];
        ZeroMemory(nTempName, sizeof(nTempName));
        if (Npc[nNpcIndex].m_Kind == kind_player)
        {
            sprintf(nTempName, "%s", UTEXT(Npc[nNpcIndex].Name, 1).c_str());
        }
        // sprintf(nTempName,UTEXT(Npc[nNpcIndex].Name,1).c_str());
        // int offset=0;
        if (Npc[nNpcIndex].m_FreezeState.nTime || Npc[nNpcIndex].m_PoisonState.nTime ||
            Npc[nNpcIndex].m_ConfuseState.nTime || Npc[nNpcIndex].m_StunState.nTime || Npc[nNpcIndex].m_Hide.nTime ||
            Npc[nNpcIndex].m_randmove.nTime)
        {
            strcat(nTempName, "(");
            if (Npc[nNpcIndex].m_FreezeState.nTime)
                strcat(nTempName, "Băng");
            if (Npc[nNpcIndex].m_PoisonState.nTime)
                strcat(nTempName, "Độc");
            if (Npc[nNpcIndex].m_ConfuseState.nTime)
                strcat(nTempName, "Loạn");
            if (Npc[nNpcIndex].m_StunState.nTime)
                strcat(nTempName, "Choáng");
            if (Npc[nNpcIndex].m_randmove.nTime)
                strcat(nTempName, "Sợ");
            if (Npc[nNpcIndex].m_Hide.nTime)
                strcat(nTempName, "Ẩn");
            strcat(nTempName, ")");
            // offset =2;
        }
        // float nFontSize = 12;
        // nTempSize.width  = nFontSize*strlen(nTempName)/2-offset*3;//字节
        // nTempSize.height = 14;
        if (!objnamelabel)
        {
            // objnamelabel = Label::createWithTTF("","fonts/gb_2312.fnt");
            //  create the stroke only label
            //  FontDefinition strokeTextDef;
            //  strokeTextDef.m_fontSize = 13;
            //  strokeTextDef.m_fontName = std::string(UI_GAME_FONT_DEFAULT_VN);//UI_GAME_FONT_DEFAULT
            //  strokeTextDef.m_stroke.m_strokeEnabled = true;
            //  strokeTextDef.m_stroke.m_strokeColor   = ax::Color3B::BLACK;
            //  strokeTextDef.m_stroke.m_strokeSize    = 1.5;
            //  strokeTextDef.m_fontFillColor          = ax::Color3B::WHITE;
            //  strokeTextDef.m_alignment              = TextHAlignment::CENTER;
            //  stroke only label
            // objnamelabel = Label::createWithFontDefinition("",strokeTextDef);
            // tên nhân vật
            objnamelabel = Label::createWithTTF("", UI_GAME_FONT_VN, 13);
            // objnamelabel->setAnchorPoint(ax::Vec2(0,0));
            objnamelabel->enableOutline(Color4B(0, 0, 0, 255), 4);
            // objnamelabel =
            // Label::createWithTTF("",UI_GAME_FONT_DEFAULT,nFontSize,nTempSize,TextHAlignment::CENTER);//Arial
            nNpcOtherNode->addChild(objnamelabel, 1, NPC_NAME);
        }
        // float nscale = 12/30;
        // objnamelabel->setScale(0.4);
        // objnamelabel->enableStroke(ax::Color3B::BLACK,1,true);
        // objnamelabel->setContentSize(nTempSize);
        // sprintf(nTempName,nTempName);

        if (_clientlanguage != 1 && Npc[nNpcIndex]._clientName[0])
        {  // 名称的翻译
            // strcat(nTempName,"\n");
            strcat(nTempName, Npc[nNpcIndex]._clientName);
        }

        objnamelabel->setString(nTempName);
        ax::Color3B nNameColor = ax::Color3B::WHITE;
        Sprite* npcSexspr      = NULL;
        switch (m_Kind)
        {
        case kind_player:
        {  //
            int nTempHeight        = nOffHeight;
            char* nPath            = NULL;
            Node* objautoplaylabel = NULL;
            Node* ReBornLabel      = NULL;
            Label* ToneNameLabel   = NULL;
            Node* nTempNode        = NULL;
            Label* nTitleLabel     = NULL;
            itemTanlabel           = (Label*)nNpcOtherNode->getChildByTag(PLAYER_TAN);
            if (Npc[nNpcIndex].m_BaiTan)
            {  // 如果是摆摊的  PLAYER_TAN
                if (!itemTanlabel)
                {
                    Size nTempSize = ((KuiShowSprite*)itemTanlabel)->getShowSize();
                    // stroke only label
                    // objnamelabel =
                    // Label::createWithTTF("",UI_GAME_FONT_DEFAULT,nFontSize,nTempSize,TextHAlignment::CENTER);//Arial
                    itemTanlabel = Label::createWithTTF("", UI_GAME_FONT_DEFAULT_VN, 50, nTempSize,
                                                        TextHAlignment::CENTER);  // Arial
                    itemTanlabel->enableOutline(ax::Color4B::BLACK, 1.5);
                    char nstrName[64] = {0};
                    Npc[nNpcIndex].GetstrInfo(STR_SHOP_NAME, nstrName);
                    itemTanlabel->setString(UTEXT(nstrName, 1).c_str());
                    nNpcOtherNode->addChild(itemTanlabel, 1, PLAYER_TAN);
                }
            }
            else
            {
                nNpcOtherNode->removeChild(itemTanlabel, true);
                itemTanlabel = NULL;
            }

            if (npcSexspr)
                npcSexspr->setVisible(false);

            switch (Npc[nNpcIndex].m_CurrentCamp)
            {
            case camp_begin:  // 新手
                break;
            case camp_justice:  // 正派
                nNameColor = ax::Color3B::ORANGE;
                break;
            case camp_evil:  // 邪派
                nNameColor = ax::Color3B::MAGENTA;
                break;
            case camp_balance:  // 中立
                nNameColor = ax::Color3B::GREEN;
                break;
            case camp_free:  // 杀手
                nNameColor = ax::Color3B::RED;
                break;
            case camp_blue:  // 蓝帮
                break;
            case camp_green:  // 绿帮
                break;
            default:
                break;
            }
            objnamelabel->setColor(nNameColor);  // màu tên nv

            // hiển thị danh hiệu
            if (nIsShowTitle)
            {
                float nTempOffsetX = 0;
                char nTempTongName[32];
                ZeroMemory(nTempTongName, sizeof(nTempTongName));
                if (Npc[nNpcIndex].m_nFigure >= 0 && Npc[nNpcIndex].TongName[0])
                {  // 帮派 显示帮派称号
                    // messageBox("tongmane","test");
                    FontDefinition strokeTextDef;
                    strokeTextDef._fontSize = 13;
                    strokeTextDef._fontName = std::string(UI_GAME_FONT_DEFAULT_VN);  // UI_GAME_FONT_DEFAULT
                    strokeTextDef._stroke._strokeEnabled = true;
                    strokeTextDef._stroke._strokeColor   = ax::Color3B::BLACK;
                    strokeTextDef._stroke._strokeSize    = 1.5;
                    strokeTextDef._fontFillColor         = nNameColor;  // ax::Color3B::WHITE;
                    strokeTextDef._alignment             = TextHAlignment::CENTER;

                    ToneNameLabel = (Label*)nNpcOtherNode->getChildByTag(NPC_TONG);
                    switch (Npc[nNpcIndex].m_nFigure)  // 帮会职位
                    {
                        /*
                        enumTONG_FIGURE_MEMBER,				// 帮众
                        enumTONG_FIGURE_MANAGER,			// 队长
                        enumTONG_FIGURE_DIRECTOR,			// 长老
                        enumTONG_FIGURE_MASTER,				// 帮主
                        */
                    case enumTONG_FIGURE_MEMBER:  // 帮众
                        sprintf(nTempTongName, "%s(Bang chúng)", UTEXT(Npc[nNpcIndex].TongName, 1).c_str());
                        break;
                    case enumTONG_FIGURE_MANAGER:
                        sprintf(nTempTongName, "%s(Đội trưởng)", UTEXT(Npc[nNpcIndex].TongName, 1).c_str());
                        break;
                    case enumTONG_FIGURE_DIRECTOR:  // 长老
                        sprintf(nTempTongName, "%s(Trưởng lão)", UTEXT(Npc[nNpcIndex].TongName, 1).c_str());
                        break;
                    case enumTONG_FIGURE_MASTER:
                        sprintf(nTempTongName, "%s(Bang chủ)", UTEXT(Npc[nNpcIndex].TongName, 1).c_str());
                        break;
                    default:
                        sprintf(nTempTongName, "%s(Dã nhân)", UTEXT(Npc[nNpcIndex].TongName, 1).c_str());
                        break;
                    }
                    // sprintf(nTempTongName,Npc[nNpcIndex].TongName);

                    if (!ToneNameLabel)
                    {
                        Size nDsize;
                        nDsize.width  = nNpcOtherNode->getContentSize().width;
                        nDsize.height = 0;
                        // ToneNameLabel =
                        // Label::createWithFontDefinition(UTEXT(nTempTongName,1).c_str(),strokeTextDef);
                        //  nTitleLabel =
                        //  Label::createWithTTF(UTEXT(PlayerRank,1).c_str(),UI_GAME_FONT_DEFAULT,12);//Arial
                        ToneNameLabel = Label::createWithTTF(nTempTongName, UI_GAME_FONT_DEFAULT_VN, 13, nDsize,
                                                             TextHAlignment::CENTER);
                        ToneNameLabel->setAnchorPoint(ax::Vec2(0, 1));
                        ToneNameLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
                        ToneNameLabel->setColor(nNameColor);
                        ToneNameLabel->setPosition(ax::Vec2(0, nTempHeight + 13));
                        nNpcOtherNode->addChild(ToneNameLabel, 1, NPC_TONG);
                    }

                    ToneNameLabel->setString(UTEXT(nTempTongName, 1).c_str());
                    ToneNameLabel->setPosition(ax::Vec2(0, nTempHeight + 13));
                    nTempHeight += 14;
                }
                else
                {  // 隐藏
                    nTitleLabel = (Label*)nNpcOtherNode->getChildByTag(NPC_TONG);
                    if (nTitleLabel)
                        nNpcOtherNode->removeChild(nTitleLabel, true);
                }

                if (Npc[nNpcIndex].m_btRankFFId > 0 && Npc[nNpcIndex].m_btRankFFId != 88 &&
                    Npc[nNpcIndex].m_btRankFFId != 99)
                {  // spr称号
                    // PaintHonor(m_btRankFFId,nMpsX,nMpsY,6);	         //显示spr称号
                    nPath = (char*)"\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr";  // 红心右旋转
                    // SetClientSpr(nPath,nMpsX,nMpsY+50,0,8);
                }
                // 转生的光环
                if (Npc[nNpcIndex].nReBorn > 0)
                {
                    ReBornLabel = nNpcOtherNode->getChildByTag(PLAYER_REBORN);
                    if (ReBornLabel == NULL)
                    {
                        auto callback = [this](ax::Node* sender) { this->closeShowSpritePad(sender); };

                        nPath       = (char*)"\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr";  // 凤凰印
                        ReBornLabel = KuiShowSprite::create(nPath, 0, nTempHeight + 20, this, callback, true,
                                                            RUIMAGE_RENDER_FLAG_REF_SPOT);
                        ((KuiShowSprite*)ReBornLabel)->setRegisterWithTouchDispatcher(128);
                        nNpcOtherNode->addChild(ReBornLabel, 1, PLAYER_REBORN);
                    }
                    Size nTempSize = ((KuiShowSprite*)ReBornLabel)->getShowSize();
                    ((KuiShowSprite*)ReBornLabel)
                        ->setShowPosition(0 - nTempSize.width / 2, nTempHeight + 20);  // ax::Vec2(0,nOffHeight+20)
                    //((KuiShowSprite*)ReBornLabel)->setRegisterWithTouchDispatcher(128);
                }
                else
                {
                    if (nTempNode = nNpcOtherNode->getChildByTag(PLAYER_REBORN))
                    {
                        ((KuiShowSprite*)nTempNode)->removeShowSprite();
                        nNpcOtherNode->removeChild(nTempNode, true);
                    }
                }
                // Node *nTempNode=NULL;
                if (Npc[nNpcIndex].m_PifengType > 0)
                {  // 开始设置披风称号显示
                    // nTempOffsetX+=30;
                    nTempNode = PaintPifeng(Npc[nNpcIndex].m_PifengType, nNpcOtherNode, 0, nOffHeight - 20);
                    if (nTempNode)
                    {
                        Size nTempSize = ((KuiShowSprite*)nTempNode)->getShowSize();
                        ((KuiShowSprite*)nTempNode)
                            ->setShowPosition(0 - nTempSize.width - 15, nOffHeight - nTempSize.height / 2);
                    }
                    // objnamelabel->setPosition(ax::Vec2(0,nOffHeight));
                }
                else
                {
                    if (nTempNode = nNpcOtherNode->getChildByTag(PLAYER_PIFENG))
                    {
                        ((KuiShowSprite*)nTempNode)->removeShowSprite();
                        nNpcOtherNode->removeChild(nTempNode, true);
                    }
                }
                // vị trí thanh hp và tên nv, title
                objnamelabel->setPosition(ax::Vec2(0, nOffHeight + 20));

                char PlayerRank[32] = {0};
                FontDefinition strokeTextDef;
                strokeTextDef._fontSize              = 13;
                strokeTextDef._fontName              = std::string(UI_GAME_FONT_DEFAULT_VN);  // UI_GAME_FONT_DEFAULT
                strokeTextDef._stroke._strokeEnabled = true;
                strokeTextDef._stroke._strokeColor   = ax::Color3B::BLACK;
                strokeTextDef._stroke._strokeSize    = 1.5;
                strokeTextDef._fontFillColor         = ax::Color3B::MAGENTA;  // ax::Color3B::WHITE;
                strokeTextDef._alignment             = TextHAlignment::CENTER;
                Size nDsize;
                nDsize.width  = nNpcOtherNode->getContentSize().width;
                nDsize.height = 0;
                if (Npc[nNpcIndex].m_CurNpcTitle > 0)
                {  // 新文字称号
                    nTitleLabel = (Label*)nNpcOtherNode->getChildByTag(PLAYER_TITLE);
                    g_PlayerTitle.GetString(1 + 1, "TitleName", "", PlayerRank, sizeof(PlayerRank));
                    // nTitleLabel =
                    // Label::createWithTTF(UTEXT(PlayerRank,1).c_str(),UI_GAME_FONT_DEFAULT,12,nSize,TextHAlignment::CENTER);//Arial
                    if (!nTitleLabel)
                    {
                        // nTitleLabel = Label::createWithFontDefinition(UTEXT(PlayerRank,1).c_str(),strokeTextDef);
                        nTitleLabel = Label::createWithTTF(PlayerRank, UI_GAME_FONT_DEFAULT_VN, 13, nDsize,
                                                           TextHAlignment::CENTER);
                        nTitleLabel->setAnchorPoint(ax::Vec2(0.5, 0.5));
                        nTitleLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
                        nTitleLabel->setColor(ax::Color3B::MAGENTA);
                        nTitleLabel->setPosition(
                            ax::Vec2(-nNpcOtherNode->getContentSize().width / 2, nTempHeight + 35));
                        nNpcOtherNode->addChild(nTitleLabel, 1, PLAYER_TITLE);
                    }

                    nTitleLabel->setString(UTEXT(PlayerRank, 1).c_str());
                    nTitleLabel->setPosition(ax::Vec2(-nNpcOtherNode->getContentSize().width / 2, nTempHeight + 35));
                }
                else if (Npc[nNpcIndex].m_NpcTitle > 0)
                {
                    nTitleLabel = (Label*)nNpcOtherNode->getChildByTag(PLAYER_TITLE);
                    g_PlayerTitle.GetString(Npc[nNpcIndex].m_NpcTitle + 1, "TitleName", "", PlayerRank,
                                            sizeof(PlayerRank));
                    // nTitleLabel =
                    // Label::createWithTTF(UTEXT(PlayerRank,1).c_str(),UI_GAME_FONT_DEFAULT,12,nSize,TextHAlignment::CENTER);//Arial
                    if (!nTitleLabel)
                    {
                        // nTitleLabel = Label::createWithFontDefinition(UTEXT(PlayerRank,1).c_str(),strokeTextDef);
                        nTitleLabel = Label::createWithTTF(PlayerRank, UI_GAME_FONT_DEFAULT_VN, 13, nDsize,
                                                           TextHAlignment::CENTER);
                        nTitleLabel->setAnchorPoint(ax::Vec2(0.5, 0.5));
                        nTitleLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
                        nTitleLabel->setColor(ax::Color3B::MAGENTA);
                        nTitleLabel->setPosition(
                            ax::Vec2(-nNpcOtherNode->getContentSize().width / 2, nTempHeight + 35));
                        nNpcOtherNode->addChild(nTitleLabel, 1, PLAYER_TITLE);
                    }
                    nTitleLabel->setString(UTEXT(PlayerRank, 1).c_str());
                    nTitleLabel->setPosition(ax::Vec2(-nNpcOtherNode->getContentSize().width / 2, nTempHeight + 35));
                }
                else if (Npc[nNpcIndex].m_btRankId > 0)
                {  // 显示老的称号

                    nTitleLabel = (Label*)nNpcOtherNode->getChildByTag(PLAYER_TITLE);
                    g_RankTabSetting.GetString(Npc[nNpcIndex].m_btRankId, "RANKSTR", "", PlayerRank,
                                               sizeof(PlayerRank));
                    if (!nTitleLabel)
                    {
                        // nTitleLabel =
                        // Label::createWithTTF(UTEXT(PlayerRank,1).c_str(),UI_GAME_FONT_DEFAULT,12);//Arial
                        // nTitleLabel = Label::createWithFontDefinition(UTEXT(PlayerRank,1).c_str(),strokeTextDef);
                        nTitleLabel = Label::createWithTTF(PlayerRank, UI_GAME_FONT_DEFAULT_VN, 13, nDsize,
                                                           TextHAlignment::CENTER);
                        nTitleLabel->setAnchorPoint(ax::Vec2(0.5, 0.5));
                        nTitleLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
                        nTitleLabel->setColor(ax::Color3B::MAGENTA);
                        nTitleLabel->setPosition(
                            ax::Vec2(-nNpcOtherNode->getContentSize().width / 2, nTempHeight + 13));
                        nNpcOtherNode->addChild(nTitleLabel, 1, PLAYER_TITLE);
                    }
                    nTitleLabel->setString(UTEXT(PlayerRank, 1).c_str());
                    nTitleLabel->setPosition(ax::Vec2(-nNpcOtherNode->getContentSize().width / 2, nTempHeight + 13));
                }
                else
                {  // 隐藏
                    nTitleLabel = (Label*)nNpcOtherNode->getChildByTag(PLAYER_TITLE);
                    if (nTitleLabel)
                        nNpcOtherNode->removeChild(nTitleLabel, true);
                }
            }
            else
            {  // 否则就隐藏　或　删除　这些称号
                Node* nTempNode = NULL;
                if (nTempNode = nNpcOtherNode->getChildByTag(PLAYER_PIFENG))
                {
                    ((KuiShowSprite*)nTempNode)->removeShowSprite();
                    nNpcOtherNode->removeChild(nTempNode, true);
                }

                if (nTempNode = nNpcOtherNode->getChildByTag(PLAYER_REBORN))
                {
                    ((KuiShowSprite*)nTempNode)->removeShowSprite();
                    nNpcOtherNode->removeChild(nTempNode, true);
                }

                nTitleLabel = (Label*)nNpcOtherNode->getChildByTag(PLAYER_TITLE);
                if (nTitleLabel)
                    nNpcOtherNode->removeChild(nTitleLabel, true);

                nTitleLabel = (Label*)nNpcOtherNode->getChildByTag(NPC_TONG);
                if (nTitleLabel)
                    nNpcOtherNode->removeChild(nTitleLabel, true);

                objnamelabel->setPosition(ax::Vec2(0, nOffHeight + 32));
            }

            if (m_Kind == kind_player)
            {
                if (itemTanlabel)
                {
                    itemTanlabel->setColor(ax::Color3B::YELLOW);
                    itemTanlabel->setPosition(ax::Vec2(0, nTempHeight + 13));
                }
            }

            if (m_Kind == kind_player && nNpcIndex == Player[CLIENT_PLAYER_INDEX].m_nIndex)
            {
                objautoplaylabel = nNpcOtherNode->getChildByTag(PLAYER_AUTO);
                if (nNpcIndex == Player[CLIENT_PLAYER_INDEX].m_nIndex && g_pCoreShell &&
                    g_pCoreShell->GetAutoplayid())  // 获取挂机的状态
                {                                   // Trong chiến đấu tự động Khi treo
                    if (!objautoplaylabel)
                    {
                        auto callback = [this](ax::Node* sender) { this->closeShowSpritePad(sender); };

                        nPath = (char*)"\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr";  //"\\spr\\skill\\others\\亢龙有悔.spr";//
                        objautoplaylabel = KuiShowSprite::create(nPath, 0, nTempHeight + 2, this, callback, true,
                                                                 RUIMAGE_RENDER_FLAG_REF_SPOT);
                        ((KuiShowSprite*)objautoplaylabel)->setRegisterWithTouchDispatcher(128);
                        nNpcOtherNode->addChild(objautoplaylabel, 1, PLAYER_AUTO);
                    }
                }
                else
                {
                    if (objautoplaylabel)
                        ((KuiShowSprite*)objautoplaylabel)->removeShowSprite();
                    nNpcOtherNode->removeChild(objautoplaylabel, true);
                    objautoplaylabel = NULL;
                }

                if (g_pCoreShell && g_pCoreShell->GetAutoplayid())  // 获取挂机的状态
                {                                                   // 自动战斗中 挂机中
                    if (objautoplaylabel)
                    {
                        Size nTempSize = ((KuiShowSprite*)objautoplaylabel)->getShowSize();
                        ((KuiShowSprite*)objautoplaylabel)
                            ->setShowPosition(0 - nTempSize.width / 2, nTempHeight + 2);  // ax::Vec2(0,nOffHeight+20)
                    }
                }
            }

            Texture2D* plifeTexture = NULL;
            Rect CCRectLife         = ax::Rect(0, 0, 0, 0);
            // 0  为正常状态 1 为切磋 2 为屠杀
            int nPk =
                Npc[nNpcIndex]
                    .m_nPKFlag;  // Player[Npc[nNpcIndex].GetNpcPepoleID()].m_cPK.GetNormalPKState();//g_pCoreShell->GetGameData(GDI_PK_SETTING,0,0);
            switch (nPk)
            {
            case 0:
            {  // 正常
                plifeTexture = ax::Director::getInstance()->getTextureCache()->addImage("ui/state/life_normal.png");
            }
            break;
            case 1:
            {  // 切磋
                plifeTexture = ax::Director::getInstance()->getTextureCache()->addImage("ui/state/life_pk.png");
            }
            break;
            case 2:
            {  // 屠杀
                plifeTexture = ax::Director::getInstance()->getTextureCache()->addImage("ui/state/life_tusha.png");
            }
            break;
            default:
                break;
            }

            if (plifeTexture)
            {
                CCRectLife.size = plifeTexture->getContentSize();
                npcLifespr->setTexture(plifeTexture);
                npcLifespr->setTextureRect(CCRectLife, false, CCRectLife.size);
            }
            npcLifespr->setScaleX(nCurScaleX);
            npcLifespr->setAnchorPoint(ax::Vec2(0, 0));
            // vị trí thanh hp nhân vật
            npcLifespr->setPosition(
                ax::Vec2(-nNpcOtherNode->getContentSize().width - npcLifespr->getContentSize().width / 2, nOffHeight));
            if (relation_enemy == NpcSet.GetRelation(nNpcIndex, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
                Npc[nNpcIndex].m_Kind == kind_player && nPk != 2)
            {
                npcLifespr->setVisible(false);  // 有敌对关系的玩家不显示生命
                break;
            }
            if (!npcLifespr->isVisible())
                npcLifespr->setVisible(true);
        }
        break;
        case kind_dialoger:
        {
            // sprintf(nTempName,Npc[nNpcIndex].Name);
            // objnamelabel->setString(nTempName);
            objnamelabel->setPosition(ax::Vec2(0, nOffHeight / 2 + 32));
            if (npcSexspr)
                npcSexspr->setPosition(ax::Vec2(objnamelabel->getContentSize().width, nOffHeight / 2));
            if (npcLifespr)
                npcLifespr->setVisible(false);
            if (npcSexspr)
                npcSexspr->setVisible(false);
        }
        break;
        case kind_normal:
        {                                                      // ten quai vat
            int nType = Npc[nNpcIndex].m_cGold.GetGoldType();  // 获取是否黄金怪物！0为非黄金
            int nVal  = 0;
            g_NpcSetting.GetInteger(Npc[nNpcIndex].m_NpcSettingIdx + 2, "AuraSkillId", 0, &nVal);

            if (nType == 0 && nVal == 0)
                nNameColor = ax::Color3B::WHITE;  // 白色名字怪物
            else if (nType == 1)
                nNameColor = ax::Color3B::GREEN;  // 绿怪  爆钱
            else if (nType >= 2 && nType <= 4)
            {
                nNameColor.r = 100;
                nNameColor.g = 100;
                nNameColor.b = 255;
                // dwColor=TGetColor("100,100,255");//浅蓝100,100,255 深蓝 0,0,160
            }
            else if (nType >= 5 && nType <= 11)
            {
                nNameColor.r = 234;
                nNameColor.g = 189;
                nNameColor.b = 11;
                // dwColor=TGetColor("234,189,11"); //GetColor(" 255,255,0");
            }
            else if (nType >= 12 && nType <= 16)
            {
                nNameColor.r = 255;
                nNameColor.g = 62;
                nNameColor.b = 62;
                // dwColor=TGetColor("255,62,62");	//   深红"210,0,0"
            }
            else
            {
                if (nVal > 0)
                {
                    nNameColor.r = 234;
                    nNameColor.g = 189;
                    nNameColor.b = 11;
                }
            }
            char seriesPath[128];
            ZeroMemory(seriesPath, sizeof(seriesPath));
            switch (Npc[nNpcIndex].m_Series)
            {
            case 0:
                sprintf(seriesPath, "\\spr\\ui3\\npc\\series0.spr");
                break;
            case 1:
                sprintf(seriesPath, "\\spr\\ui3\\npc\\series1.spr");
                break;
            case 2:
                sprintf(seriesPath, "\\spr\\ui3\\npc\\series2.spr");
                break;
            case 3:
                sprintf(seriesPath, "\\spr\\ui3\\npc\\series3.spr");
                break;
            case 4:
                sprintf(seriesPath, "\\spr\\ui3\\npc\\series4.spr");
                break;
            default:
                break;
            }
            int m_nWidth, m_nHeight, nFrams;
            Texture2D* bgCur = NULL;
            SPRFRAMSINFO nSprInfo;
            ZeroMemory(&nSprInfo, sizeof(nSprInfo));
            bgCur = _getinidata.getinidata_one(seriesPath, 0, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
            if (bgCur)
            {
                npcSexspr = (Sprite*)nNpcOtherNode->getChildByTag(NPC_SEX);
                if (!npcSexspr)
                {
                    npcSexspr = Sprite::createWithTexture(bgCur);
                    nNpcOtherNode->addChild(npcSexspr, 1, NPC_SEX);
                }
            }
            int nNpcoffY;
            if (_clientlanguage != 1)
                nNpcoffY = nOffHeight - objnamelabel->getContentSize().height;
            else
                nNpcoffY = nOffHeight - objnamelabel->getContentSize().height;

            objnamelabel->setColor(nNameColor);
            if (npcSexspr)
                npcSexspr->setPosition(
                    ax::Vec2(0 - npcSexspr->getContentSize().width / 2 - objnamelabel->getContentSize().width / 2 + 30,
                             nNpcoffY + 8));

            objnamelabel->setPosition(ax::Vec2(0, nNpcoffY + 30));  // vị trí tên NPC

            /*if  (m_Kind==kind_player)
                            {
                                    if (g_pCoreShell && g_pCoreShell->GetAutoplayid())	//获取挂机的状态
                                    {//自动战斗中 挂机中
                                            if (objautoplaylabel)
                                                objautoplaylabel->setPosition(ax::Vec2(0,nNpcoffY+20));
                                    }
                            }*/
            //   if (_clientlanguage!=1)
            // 	 nNpcoffY-=15;
            //   else
            //      nNpcoffY-=10;
            // thanh hp cua npc
            npcLifespr->setPosition(ax::Vec2(0, nNpcoffY + 5));
            npcLifespr->setScaleX(nCurScaleX);
        }
        break;
        default:
        {
            if (npcLifespr)
                npcLifespr->setVisible(false);
            if (npcSexspr)
                npcSexspr->setVisible(false);
        }
        break;
        }
        /*if (Npc[nNpcIndex].m_Kind == kind_player) //玩家
        else if (Npc[nNpcIndex].m_Kind == kind_dialoger)//对话
        else if (Npc[nNpcIndex].m_Kind == kind_normal)//动物
        */
    }
}

// 披风=================
Node* KgameWorld::PaintPifeng(int m_PifengType, Node* nNpcOtherNode, int nMpsX, int nMpsY)
{
    if (!nNpcOtherNode)
        return NULL;
    Layer* pShowSprite = NULL;
    if (pShowSprite = (Layer*)nNpcOtherNode->getChildByTag(PLAYER_PIFENG))
    {
        ((KuiShowSprite*)pShowSprite)->setShowPosition(nMpsX, nMpsY);
        return (Node*)pShowSprite;
    }

    KIniFile nBank;
    nBank.Load("\\Ui\\npcbobo.ini");
    int nCount = 0, nTime = 20;
    char szImageName[128] = {0}, nDir[8] = {0};
    nBank.GetInteger("FortuneRank", "count", 11, &nCount);
    nBank.GetInteger("FortuneRank", "nTime", 20, &nTime);
    if (m_PifengType > nCount)
        m_PifengType = nCount;
    sprintf(nDir, "Spr_%d", m_PifengType);
    nBank.GetString("FortuneRank", nDir, "", szImageName, sizeof(szImageName));
    nBank.Clear();

    auto callback = [this](ax::Node* sender) { this->closeShowSpritePad(sender); };

    pShowSprite = KuiShowSprite::create(szImageName, nMpsX, nMpsX, this, callback, false);
    nNpcOtherNode->addChild(pShowSprite, 1, PLAYER_PIFENG);

    return (Node*)pShowSprite;
}
// 称号
Node* KgameWorld::PaintHonor(int nbtRankFFId, Node* nNpcOtherNode, int nMpsX, int nMpsY)
{
    if (!nNpcOtherNode)
        return NULL;  // ax::Size(0,0);

    Layer* pShowSprite = NULL;
    if (pShowSprite = (Layer*)nNpcOtherNode->getChildByTag(PLAYER_SPRCHENGHAO))
    {
        ((KuiShowSprite*)pShowSprite)->setShowPosition(nMpsX, nMpsY);
        return (Node*)pShowSprite;
    }

    KTabFile nBank;
    nBank.Load("\\Settings\\HonorSetting.txt");
    int nRows = nBank.GetHeight();  // 行数
    int nLies = nBank.GetWidth();   // 列数
    char szImageName[128], nLuaField[64];
    int n_X, n_Y, mInterval = 100;
    ZeroMemory(szImageName, strlen(szImageName));
    ZeroMemory(nLuaField, strlen(nLuaField));
    if (nbtRankFFId <= nRows)
    {
        nBank.GetString(nbtRankFFId, "HONORLINK", "", nLuaField, sizeof(nLuaField));
        sprintf(szImageName, "\\spr\\skill\\others\\%s", nLuaField);
        nBank.GetInteger(nbtRankFFId, "HONORX", 10, &n_X);
        nBank.GetInteger(nbtRankFFId, "HONORY", 10, &n_Y);
        nBank.GetInteger(nbtRankFFId, "nInterval", 100, &mInterval);
    }
    else
    {  // 宠物
        sprintf(szImageName, "\\spr\\skill\\others\\助攻王.spr");
    }

    auto callback = [this](ax::Node* sender) { this->closeShowSpritePad(sender); };

    pShowSprite = KuiShowSprite::create(szImageName, nMpsX + n_X, nMpsX + n_Y, this, callback, false);
    nNpcOtherNode->addChild(pShowSprite, 1, PLAYER_SPRCHENGHAO);
    nBank.Clear();
    return (Node*)pShowSprite;
}

void KgameWorld::closeShowSpritePad(Node* pNode) {}

void KgameWorld::taskCilentMsgArrival(KNewsMessage* pMsg, int pTime, int nIsBtn)
{
    if (!pMsg)
        return;

    if (m_taskPadIsOpen)
    {
        // this->removeChildByTag(OBJ_NODE_TASK,true);
        KuiTaskInfo* nCurPtr = (KuiTaskInfo*)this->getChildByTag(OBJ_NODE_TASK);
        if (nCurPtr)
        {
            TEncodeText_(pMsg->sMsg, strlen(pMsg->sMsg));
            nCurPtr->addmsgInfo(pMsg->sMsg, nIsBtn, pMsg->nType);
        }
    }
}
//
void KgameWorld::taskCilentFindPathArrival(KTaskPathInfo* pMsg, int pTime, int nIsBtn)
{
    if (!pMsg)
        return;
    if (m_taskPadIsOpen)
    {
        KuiTaskInfo* nCurPtr = (KuiTaskInfo*)this->getChildByTag(OBJ_NODE_TASK);
        if (nCurPtr)
        {
            nCurPtr->addmsgBtnInfo(pMsg);
        }
    }
}

void KgameWorld::_setBuWeiHide(int nNpcIndex, bool isHide, int isthis)
{
    Node* nPartCurNode = NULL;
    char nTempKey[64];
    sprintf(nTempKey, "npc_%d", nNpcIndex);
    std::string _npcKey = nTempKey;
    nPartCurNode        = ParentNode_npc->getChildByName(_npcKey);  // 这个NPC的父节点 子节点有很多部位
    if (nPartCurNode == NULL)
        return;
    Sprite* nPartSpr = NULL;
    nPartSpr         = (Sprite*)(nPartCurNode->getChildByTag(isthis));
    if (nPartSpr)
        nPartSpr->setVisible(isHide);
}

void KgameWorld::setBuWeiHide(int nNpcIndex, bool isHide, int isthis)
{
    if (!g_GameWorld || !ParentNode_npc || nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;

    Node* nPartCurNode = NULL;
    char nTempKey[64];
    sprintf(nTempKey, "npc_%d", nNpcIndex);
    std::string _npcKey = nTempKey;
    nPartCurNode        = ParentNode_npc->getChildByName(_npcKey);  // 这个NPC的父节点 子节点有很多部位
    if (nPartCurNode == NULL)
        return;
    Sprite* nPartSpr = NULL;
    nPartSpr         = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_HEAD));
    if (nPartSpr)
        nPartSpr->setVisible(isHide);
    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_BOBY));
    if (nPartSpr)
        nPartSpr->setVisible(isHide);
    /* UI_EQ_PART_LEFTHD npc通用
    UI_EQ_PART_LEFTHD,
    UI_EQ_PART_RIGTHHD,
    UI_EQ_PART_LEFTHD_W,
    UI_EQ_PART_RIGTHHD_W,
    UI_EQ_PART_HORSE_Q,
    UI_EQ_PART_HORSE_Z,
    UI_EQ_PART_HORSE_H,
    UI_EQ_PART_PIFENG,
    */

    // nPartSpr = (Sprite *)(nPartCurNode->getChildByTag(UI_EQ_PART_LEFTHD)); //npc通用
    // if (nPartSpr)
    //	nPartSpr->setVisible(isHide);

    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_RIGTHHD));
    if (nPartSpr)
        nPartSpr->setVisible(isHide);
    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_LEFTHD_W));
    if (nPartSpr)
        nPartSpr->setVisible(isHide);
    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_RIGTHHD_W));
    if (nPartSpr)
        nPartSpr->setVisible(isHide);
    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_HORSE_Q));
    if (nPartSpr)
        nPartSpr->setVisible(isHide);
    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_HORSE_Z));
    if (nPartSpr)
        nPartSpr->setVisible(isHide);
    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_HORSE_H));
    if (nPartSpr)
        nPartSpr->setVisible(isHide);
    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_PIFENG));
    if (nPartSpr)
        nPartSpr->setVisible(isHide);
}

void KgameWorld::DrawShadow(int nNpcIndex, int nPos, KRUImage* pPrimitives)
{
    if (!g_GameWorld || !ParentNode_npc || pPrimitives == NULL || nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;
}

void KgameWorld::DrawPrimitives(int nNpcIndex,
                                int nPos,
                                KRUImage* pPrimitives,
                                unsigned int uGenre,
                                int bpartNo,
                                int nOrDer,
                                int bSinglePlaneCoord)
{  // Tạo hình NPC

    if (!g_GameWorld || !ParentNode_npc || pPrimitives == NULL || nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;

    Node* nPartCurNode = NULL;
    char nTempKey[64];
    sprintf(nTempKey, "npc_%d", nNpcIndex);
    std::string _npcKey = nTempKey;
    nPartCurNode        = ParentNode_npc->getChildByName(_npcKey);  // 这个NPC的父节点 子节点有很多部位
    if (nPartCurNode == NULL)
        return;

    if (pPrimitives[nPos].szImage[0])
    {
        // KAutoCriticalSection AutoLock(m_GameProcessLock);
        Texture2D* bgCur = NULL;
        Sprite* nPartSpr = NULL;
        int NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        Rect CCRectZero = ax::Rect(0, 0, 0, 0);
        // 开始绘画阴影
        if (Npc[nNpcIndex]._NpcShadow.szImage[0])
        {
            bgCur =
                ccgetTxtData(Npc[nNpcIndex]._NpcShadow.szImage, Npc[nNpcIndex]._NpcShadow.nFrame,
                             Npc[nNpcIndex]._NpcShadow.nX, Npc[nNpcIndex]._NpcShadow.nY, Npc[nNpcIndex]._NpcShadow.nZ,
                             &NewscrX, &NewscrY, 1, Npc[nNpcIndex]._NpcShadow.bInMenu, &nCavesWidth, &nCavesHeight);
            if (bgCur)
            {
                CCRectZero.size = bgCur->getContentSize();
                nPartSpr        = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_SHADOW));
                if (!nPartSpr)
                {
                    nPartSpr = Sprite::createWithTexture(bgCur);
                    nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                    nPartCurNode->addChild(nPartSpr, 0, UI_EQ_PART_SHADOW);
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    // setadjustColor(nNpcIndex,nPartSpr);
                    nPartSpr->setLocalZOrder(0);
                }
                else
                {  // 已经存在
                    nPartSpr->setTexture(bgCur);
                    nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    nPartSpr->setLocalZOrder(0);
                }

                if (nPartSpr)
                    nPartSpr->setVisible(nIsShowShadow);
            }
        }
        //-----阴影绘画结束-------
        NewscrX = 0, NewscrY = 0, nCavesWidth = 0, nCavesHeight = 0;
        bgCur      = NULL;
        nPartSpr   = NULL;
        CCRectZero = ax::Rect(0, 0, 0, 0);
        bgCur      = ccgetTxtData(pPrimitives[nPos].szImage, pPrimitives[nPos].nFrame, pPrimitives[nPos].oPosition.nX,
                                  pPrimitives[nPos].oPosition.nY, pPrimitives[nPos].oPosition.nZ, &NewscrX, &NewscrY,
                                  pPrimitives[nPos].bRenderFlag, bSinglePlaneCoord, &nCavesWidth, &nCavesHeight);
        if (!bgCur)
        {  // 如果没有纹理 就隐藏部位
            switch (uGenre)
            {
            case EQ_PART_BODY:
            {  // 左右手
                switch (bpartNo)
                {
                case 6:
                {
                    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_LEFTHD));
                    if (nPartSpr && nPartSpr->isVisible())
                        nPartSpr->setVisible(false);
                }
                break;
                case 7:
                {
                    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_RIGTHHD));
                    if (nPartSpr && nPartSpr->isVisible())
                        nPartSpr->setVisible(false);
                }
                break;
                default:
                    break;
                }  // switch(bpartNo)
            }
            break;
            case EQ_PART_HAND_W:
            {  // 左右手武器
                switch (bpartNo)
                {
                case 8:
                {
                    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_LEFTHD_W));
                    if (nPartSpr && nPartSpr->isVisible())
                        nPartSpr->setVisible(false);
                }
                break;
                case 9:
                {
                    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_RIGTHHD_W));
                    if (nPartSpr && nPartSpr->isVisible())
                        nPartSpr->setVisible(false);
                }
                break;
                default:
                    break;
                }  // switch(bpartNo)
            }
            break;
            default:
                break;
            }
            return;
        }
        CCRectZero.size = bgCur->getContentSize();

        switch (uGenre)
        {
        case EQ_PART_HEAD:
        {  // 头部
            switch (bpartNo)
            {
            case 0:
            {  // 头盔
                if (bgCur)
                {  // 修改精灵的纹理
                    nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_HEAD));
                    if (!nPartSpr)
                    {
                        nPartSpr = Sprite::createWithTexture(bgCur);
                        nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                        nPartCurNode->addChild(nPartSpr, 2, UI_EQ_PART_HEAD);
                        float x, y;
                        (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                        NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                        nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                        setadjustColor(nNpcIndex, nPartSpr);
                        nPartSpr->setLocalZOrder(nOrDer);
                        break;
                    }
                    nPartSpr->setTexture(bgCur);
                    nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    setadjustColor(nNpcIndex, nPartSpr);
                    nPartSpr->setLocalZOrder(nOrDer);
                }
            }
            break;
            default:
                break;
            }
        }
        break;
        case EQ_PART_BODY:
        {  // 默认位置在节点的中间
            switch (bpartNo)
            {
            case 5:
            {  // 躯体
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_BOBY));
                if (!nPartSpr)
                {
                    nPartSpr = Sprite::createWithTexture(bgCur);
                    nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                    nPartCurNode->addChild(nPartSpr, 2, UI_EQ_PART_BOBY);
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    setadjustColor(nNpcIndex, nPartSpr);
                    nPartSpr->setLocalZOrder(nOrDer);
                    break;
                }
                nPartSpr->setTexture(bgCur);
                nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                float x, y;
                (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                setadjustColor(nNpcIndex, nPartSpr);
                nPartSpr->setLocalZOrder(nOrDer);
                /* bgCur =
                ccgetTxtData(pPrimitives[nPos].szImage,pPrimitives[nPos].nFrame,pPrimitives[nPos].oPosition.nX,pPrimitives[nPos].oPosition.nY,pPrimitives[nPos].oPosition.nZ,&NewscrX,&NewscrY,pPrimitives[nPos].bRenderFlag,bSinglePlaneCoord,&nCavesWidth,&nCavesHeight);
                if (bgCur)
                {//修改精灵的纹理
                        Sprite *nPartSpr = (Sprite *)(nPartCurNode->getChildByTag(UI_EQ_PART_BOBY));
                        if (!nPartSpr)
                                break;
                        setadjustColor(nNpcIndex,nPartSpr);
                        CCRectZero.size = bgCur->getContentSize();
                        nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
                        float x,y;
                        (nPartCurNode)->getPosition(&x,&y);//屏幕坐标
                        NewscrY = visibleSize.height-NewscrY-CCRectZero.size.height;
                        nPartSpr->setPosition(ax::Vec2(NewscrX-x,NewscrY-y));
                        nPartSpr->setTexture(bgCur);
                        nPartSpr->setLocalZOrder(nOrDer);
                }*/
            }
            break;
            case 6:
            {  // 左手 和 NPC公用
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_LEFTHD));
                if (!nPartSpr)
                {
                    nPartSpr = Sprite::createWithTexture(bgCur);
                    nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                    nPartCurNode->addChild(nPartSpr, 2, UI_EQ_PART_LEFTHD);
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    setadjustColor(nNpcIndex, nPartSpr);
                    if (Npc[nNpcIndex].m_Kind != kind_player)
                        nPartSpr->setLocalZOrder(1);
                    else
                        nPartSpr->setLocalZOrder(nOrDer);
                    break;
                }

                if (!nPartSpr->isVisible())
                    nPartSpr->setVisible(true);

                nPartSpr->setTexture(bgCur);
                nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                float x, y;
                (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                setadjustColor(nNpcIndex, nPartSpr);
                if (Npc[nNpcIndex].m_Kind != kind_player)
                    nPartSpr->setLocalZOrder(1);
                else
                    nPartSpr->setLocalZOrder(nOrDer);
                /*bgCur =
                ccgetTxtData(pPrimitives[nPos].szImage,pPrimitives[nPos].nFrame,pPrimitives[nPos].oPosition.nX,pPrimitives[nPos].oPosition.nY,pPrimitives[nPos].oPosition.nZ,&NewscrX,&NewscrY,pPrimitives[nPos].bRenderFlag,bSinglePlaneCoord,&nCavesWidth,&nCavesHeight);
                if (bgCur)
                {//修改精灵的纹理
                        Sprite * nPartSpr = (Sprite *)(nPartCurNode->getChildByTag(UI_EQ_PART_LEFTHD));
                        //bgCur->getContentSize();
                        if (!nPartSpr)
                                break;

                        CCRectZero.size = bgCur->getContentSize();
                        nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
                    float x,y;
                        nPartCurNode->getPosition(&x,&y);//屏幕坐标
                        NewscrY = visibleSize.height-NewscrY-CCRectZero.size.height;
                        nPartSpr->setPosition(ax::Vec2(NewscrX-x,NewscrY-y));
                        nPartSpr->setTexture(bgCur);
                        setadjustColor(nNpcIndex,nPartSpr);
                        if (Npc[nNpcIndex].m_Kind!=kind_player)
                                nPartSpr->setLocalZOrder(1);
                        else
                                nPartSpr->setLocalZOrder(nOrDer);
                }	*/
            }
            break;
            case 7:
            {  // 右手
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_RIGTHHD));
                if (!nPartSpr)
                {
                    nPartSpr = Sprite::createWithTexture(bgCur);
                    nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                    nPartCurNode->addChild(nPartSpr, 2, UI_EQ_PART_RIGTHHD);
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    setadjustColor(nNpcIndex, nPartSpr);
                    nPartSpr->setLocalZOrder(nOrDer);
                    break;
                }
                if (!nPartSpr->isVisible())
                    nPartSpr->setVisible(true);

                nPartSpr->setTexture(bgCur);
                nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                float x, y;
                (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                setadjustColor(nNpcIndex, nPartSpr);
                nPartSpr->setLocalZOrder(nOrDer);
                /*bgCur =
                   ccgetTxtData(pPrimitives[nPos].szImage,pPrimitives[nPos].nFrame,pPrimitives[nPos].oPosition.nX,pPrimitives[nPos].oPosition.nY,pPrimitives[nPos].oPosition.nZ,&NewscrX,&NewscrY,pPrimitives[nPos].bRenderFlag,bSinglePlaneCoord,&nCavesWidth,&nCavesHeight);
                                    if (bgCur)
                                    {//修改精灵的纹理
                                            Sprite * nPartSpr = (Sprite
                   *)(nPartCurNode->getChildByTag(UI_EQ_PART_RIGTHHD)); if (!nPartSpr) break;

                                            CCRectZero.size = bgCur->getContentSize();
                                            nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
                                            setadjustColor(nNpcIndex,nPartSpr);
                                            float x,y;
                                            (nPartCurNode)->getPosition(&x,&y);//屏幕坐标
                                            NewscrY = visibleSize.height-NewscrY-CCRectZero.size.height;
                                            nPartSpr->setPosition(ax::Vec2(NewscrX-x,NewscrY-y));
                                            nPartSpr->setTexture(bgCur);
                                            nPartSpr->setLocalZOrder(nOrDer);
                                    }*/
            }
            break;
            default:
                break;
            }
        }
        break;
        case EQ_PART_HAND_W:  // 左右武器
        {                     // 默认位置在节点的中间
            switch (bpartNo)
            {
            case 8:
            {
                /*if  (nNpcIndex==Player[CLIENT_PLAYER_INDEX].m_nIndex)
                {//如果是客户端本人
                        Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(pPrimitives[nPos].szImage);
                }*/
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_LEFTHD_W));
                if (!nPartSpr)
                {
                    nPartSpr = Sprite::createWithTexture(bgCur);
                    nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                    nPartCurNode->addChild(nPartSpr, 2, UI_EQ_PART_LEFTHD_W);
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    setadjustColor(nNpcIndex, nPartSpr);
                    nPartSpr->setLocalZOrder(nOrDer);
                    break;
                }
                if (!nPartSpr->isVisible())
                    nPartSpr->setVisible(true);

                nPartSpr->setTexture(bgCur);
                nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                float x, y;
                (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                setadjustColor(nNpcIndex, nPartSpr);
                nPartSpr->setLocalZOrder(nOrDer);
            }
            break;
            case 9:
            {
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_RIGTHHD_W));
                if (!nPartSpr)
                {
                    nPartSpr = Sprite::createWithTexture(bgCur);
                    nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                    nPartCurNode->addChild(nPartSpr, 2, UI_EQ_PART_RIGTHHD_W);
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    setadjustColor(nNpcIndex, nPartSpr);
                    nPartSpr->setLocalZOrder(nOrDer);
                    break;
                }

                if (!nPartSpr->isVisible())
                    nPartSpr->setVisible(true);

                nPartSpr->setTexture(bgCur);
                nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                float x, y;
                (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                setadjustColor(nNpcIndex, nPartSpr);
                nPartSpr->setLocalZOrder(nOrDer);
            }
            break;
            default:
                break;
            }
        }
        break;
        case EQ_PART_HORSE:
        {
            switch (bpartNo)
            {
            case 12:
            {
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_HORSE_Q));
                if (!nPartSpr)
                {
                    nPartSpr = Sprite::createWithTexture(bgCur);
                    nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                    nPartCurNode->addChild(nPartSpr, 2, UI_EQ_PART_HORSE_Q);
                    if (Npc[nNpcIndex].m_HorseType < 0)
                        nPartSpr->setVisible(false);  // 设置不可见
                    else
                        nPartSpr->setVisible(true);  // 设置可见
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    setadjustColor(nNpcIndex, nPartSpr);
                    nPartSpr->setLocalZOrder(nOrDer);
                    break;
                }
                if (Npc[nNpcIndex].m_HorseType < 0)
                    nPartSpr->setVisible(false);  // 设置不可见
                else
                    nPartSpr->setVisible(true);  // 设置可见
                nPartSpr->setTexture(bgCur);
                nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                float x, y;
                (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                setadjustColor(nNpcIndex, nPartSpr);
                nPartSpr->setLocalZOrder(nOrDer);
                /*bgCur =
                ccgetTxtData(pPrimitives[nPos].szImage,pPrimitives[nPos].nFrame,pPrimitives[nPos].oPosition.nX,pPrimitives[nPos].oPosition.nY,pPrimitives[nPos].oPosition.nZ,&NewscrX,&NewscrY,pPrimitives[nPos].bRenderFlag,bSinglePlaneCoord,&nCavesWidth,&nCavesHeight);
                if (bgCur)
                {//修改精灵的纹理
                        Sprite * nPartSpr = (Sprite *)(nPartCurNode->getChildByTag(UI_EQ_PART_HORSE_Q));
                        if (!nPartSpr)
                                break;
                        if (Npc[nNpcIndex].m_HorseType<0)
                                nPartSpr->setVisible(false);//设置不可见
                        else
                                nPartSpr->setVisible(true); //设置可见

                        CCRectZero.size = bgCur->getContentSize();
                        nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
                        float x,y;
                        (nPartCurNode)->getPosition(&x,&y);//屏幕坐标
                        NewscrY = visibleSize.height-NewscrY-CCRectZero.size.height;
                        nPartSpr->setPosition(ax::Vec2(NewscrX-x,NewscrY-y));
                        nPartSpr->setTexture(bgCur);
                        setadjustColor(nNpcIndex,nPartSpr);
                        nPartSpr->setLocalZOrder(nOrDer);
                }*/
            }
            break;
            case 13:
            {
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_HORSE_Z));
                if (!nPartSpr)
                {
                    nPartSpr = Sprite::createWithTexture(bgCur);
                    nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                    nPartCurNode->addChild(nPartSpr, 2, UI_EQ_PART_HORSE_Z);
                    if (Npc[nNpcIndex].m_HorseType < 0)
                        nPartSpr->setVisible(false);  // 设置不可见
                    else
                        nPartSpr->setVisible(true);  // 设置可见
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    setadjustColor(nNpcIndex, nPartSpr);
                    nPartSpr->setLocalZOrder(nOrDer);
                    break;
                }
                if (Npc[nNpcIndex].m_HorseType < 0)
                    nPartSpr->setVisible(false);  // 设置不可见
                else
                    nPartSpr->setVisible(true);  // 设置可见
                nPartSpr->setTexture(bgCur);
                nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                float x, y;
                (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                setadjustColor(nNpcIndex, nPartSpr);
                nPartSpr->setLocalZOrder(nOrDer);
                /*bgCur =
                ccgetTxtData(pPrimitives[nPos].szImage,pPrimitives[nPos].nFrame,pPrimitives[nPos].oPosition.nX,pPrimitives[nPos].oPosition.nY,pPrimitives[nPos].oPosition.nZ,&NewscrX,&NewscrY,pPrimitives[nPos].bRenderFlag,bSinglePlaneCoord,&nCavesWidth,&nCavesHeight);
                if (bgCur)
                {//修改精灵的纹理
                        Sprite * nPartSpr = (Sprite *)(nPartCurNode->getChildByTag(UI_EQ_PART_HORSE_Z));
                        if (!nPartSpr)
                                break;
                        if (Npc[nNpcIndex].m_HorseType<0)
                                nPartSpr->setVisible(false);//设置不可见
                        else
                                nPartSpr->setVisible(true); //设置可见

                        nPartSpr->setTexture(bgCur);
                        CCRectZero.size = bgCur->getContentSize();
                        nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
                        float x,y;
                        (nPartCurNode)->getPosition(&x,&y);//屏幕坐标
                        NewscrY = visibleSize.height-NewscrY-CCRectZero.size.height;
                        nPartSpr->setPosition(ax::Vec2(NewscrX-x,NewscrY-y));
                        nPartSpr->setTexture(bgCur);
                        setadjustColor(nNpcIndex,nPartSpr);
                        nPartSpr->setLocalZOrder(-1);
                }*/
            }
            break;
            case 14:
            {
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_HORSE_H));
                if (!nPartSpr)
                {
                    nPartSpr = Sprite::createWithTexture(bgCur);
                    nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                    nPartCurNode->addChild(nPartSpr, 2, UI_EQ_PART_HORSE_H);
                    if (Npc[nNpcIndex].m_HorseType < 0)
                        nPartSpr->setVisible(false);  // 设置不可见
                    else
                        nPartSpr->setVisible(true);  // 设置可见
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    setadjustColor(nNpcIndex, nPartSpr);
                    nPartSpr->setLocalZOrder(nOrDer);
                    break;
                }
                if (Npc[nNpcIndex].m_HorseType < 0)
                    nPartSpr->setVisible(false);  // 设置不可见
                else
                    nPartSpr->setVisible(true);  // 设置可见
                nPartSpr->setTexture(bgCur);
                nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                float x, y;
                (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                setadjustColor(nNpcIndex, nPartSpr);
                nPartSpr->setLocalZOrder(nOrDer);
                /*bgCur =
                ccgetTxtData(pPrimitives[nPos].szImage,pPrimitives[nPos].nFrame,pPrimitives[nPos].oPosition.nX,pPrimitives[nPos].oPosition.nY,pPrimitives[nPos].oPosition.nZ,&NewscrX,&NewscrY,pPrimitives[nPos].bRenderFlag,bSinglePlaneCoord,&nCavesWidth,&nCavesHeight);
                if (bgCur)
                {//修改精灵的纹理
                        Sprite * nPartSpr = (Sprite *)(nPartCurNode->getChildByTag(UI_EQ_PART_HORSE_H));
                        if (!nPartSpr)
                                break;
                        if (Npc[nNpcIndex].m_HorseType<0)
                                nPartSpr->setVisible(false);//设置不可见
                        else
                                nPartSpr->setVisible(true); //设置可见

                        CCRectZero.size = bgCur->getContentSize();
                        nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
                        float x,y;
                        (nPartCurNode)->getPosition(&x,&y);//屏幕坐标
                        NewscrY = visibleSize.height-NewscrY-CCRectZero.size.height;
                        nPartSpr->setPosition(ax::Vec2(NewscrX-x,NewscrY-y));
                        nPartSpr->setTexture(bgCur);
                        setadjustColor(nNpcIndex,nPartSpr);
                        nPartSpr->setLocalZOrder(nOrDer);
                }*/
            }
            break;
            default:
                break;
            }
        }
        break;
        case EQ_PART_PIFENG:
        {  // 披风
            switch (bpartNo)
            {
            case 16:
            {
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_PIFENG));
                if (!nPartSpr)
                {
                    nPartSpr = Sprite::createWithTexture(bgCur);
                    nPartSpr->setAnchorPoint(ax::Vec2(0, 0));
                    nPartCurNode->addChild(nPartSpr, 2, UI_EQ_PART_PIFENG);
                    if (Npc[nNpcIndex].m_PifengType <= 0)
                        nPartSpr->setVisible(false);  // 设置不可见
                    else
                    {
                        if (!nPartSpr->isVisible())
                            nPartSpr->setVisible(true);  // 设置可见
                    }
                    float x, y;
                    (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                    NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                    nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                    setadjustColor(nNpcIndex, nPartSpr);
                    nPartSpr->setLocalZOrder(nOrDer);
                    break;
                }
                if (Npc[nNpcIndex].m_PifengType <= 0)
                    nPartSpr->setVisible(false);  // 设置不可见
                else
                {
                    if (!nPartSpr->isVisible())
                        nPartSpr->setVisible(true);  // 设置可见
                }
                nPartSpr->setTexture(bgCur);
                nPartSpr->setTextureRect(CCRectZero, false, CCRectZero.size);

                float x, y;
                (nPartCurNode)->getPosition(&x, &y);  // 屏幕坐标
                NewscrY = visibleSize.height - NewscrY - CCRectZero.size.height;
                nPartSpr->setPosition(ax::Vec2(NewscrX - x, NewscrY - y));
                setadjustColor(nNpcIndex, nPartSpr);
                nPartSpr->setLocalZOrder(nOrDer);
                /*bgCur =
                   ccgetTxtData(pPrimitives[nPos].szImage,pPrimitives[nPos].nFrame,pPrimitives[nPos].oPosition.nX,pPrimitives[nPos].oPosition.nY,pPrimitives[nPos].oPosition.nZ,&NewscrX,&NewscrY,pPrimitives[nPos].bRenderFlag,bSinglePlaneCoord,&nCavesWidth,&nCavesHeight);
                                        if (bgCur)
                                        {//修改精灵的纹理
                                                Sprite * nPartSpr = (Sprite
                   *)(nPartCurNode->getChildByTag(UI_EQ_PART_PIFENG)); if (!nPartSpr) break; if
                   (Npc[nNpcIndex].m_PifengType<=0) nPartSpr->setVisible(false);//设置不可见 else
                                                {
                                                        if (!nPartSpr->isVisible())
                                                            nPartSpr->setVisible(true); //设置可见
                                                }

                                                CCRectZero.size = bgCur->getContentSize();
                                                nPartSpr->setTextureRect(CCRectZero,false,CCRectZero.size);
                                                float x,y;
                                                (nPartCurNode)->getPosition(&x,&y);//屏幕坐标
                                                NewscrY = visibleSize.height-NewscrY-CCRectZero.size.height;
                                                nPartSpr->setPosition(ax::Vec2(NewscrX-x,NewscrY-y));
                                                nPartSpr->setTexture(bgCur);
                                                setadjustColor(nNpcIndex,nPartSpr);
                                                nPartSpr->setLocalZOrder(nOrDer);
                                        }*/
            }
            break;
            default:
                break;
            }
        }
        break;
        default:
            break;
        }
    }
    else
    {  // 隐藏部位
        Sprite* nPartSpr = NULL;
        switch (uGenre)
        {
        case EQ_PART_BODY:
        {  // 左右手
            switch (bpartNo)
            {
            case 6:
            {
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_LEFTHD));
                if (nPartSpr && nPartSpr->isVisible())
                    nPartSpr->setVisible(false);
            }
            break;
            case 7:
            {
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_RIGTHHD));
                if (nPartSpr && nPartSpr->isVisible())
                    nPartSpr->setVisible(false);
            }
            break;
            default:
                break;
            }  // switch(bpartNo)
        }
        break;
        case EQ_PART_HAND_W:
        {  // 左右手武器
            switch (bpartNo)
            {
            case 8:
            {
                /*if  (nNpcIndex==Player[CLIENT_PLAYER_INDEX].m_nIndex)
                {//如果是客户端本人
                        Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("该部位不存在！");
                }*/
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_LEFTHD_W));
                if (nPartSpr && nPartSpr->isVisible())
                    nPartSpr->setVisible(false);
            }
            break;
            case 9:
            {
                nPartSpr = (Sprite*)(nPartCurNode->getChildByTag(UI_EQ_PART_RIGTHHD_W));
                if (nPartSpr && nPartSpr->isVisible())
                    nPartSpr->setVisible(false);
            }
            break;
            default:
                break;
            }  // switch(bpartNo)
        }
        break;
        default:
            break;
        }  // switch(uGenre)
    }
    // nPartCurNode->sortAllChildren();
}
// 设置精灵隐藏/显示
void KgameWorld::SetHorseState(int nNpcIndex)
{
    if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;

    if (ParentNode_npc && Npc[nNpcIndex].m_HorseType < 0)
    {
        char nTempKey[64];
        sprintf(nTempKey, "npc_%d", nNpcIndex);
        std::string _npcKey = nTempKey;

        Node* CurTempNode = ParentNode_npc->getChildByName(_npcKey);
        if (!CurTempNode)
            return;
        // messageBox("马匹","马匹");
        Sprite* nPartSpr = (Sprite*)(CurTempNode->getChildByTag(UI_EQ_PART_HORSE_Q));
        if (!nPartSpr)
            return;
        nPartSpr->setVisible(false);
        nPartSpr = (Sprite*)(CurTempNode->getChildByTag(UI_EQ_PART_HORSE_Z));
        nPartSpr->setVisible(false);
        nPartSpr = (Sprite*)(CurTempNode->getChildByTag(UI_EQ_PART_HORSE_H));
        nPartSpr->setVisible(false);
    }
}

// Đặt ẩn / hiện sprite
void KgameWorld::SetPiFengState(int nNpcIndex)
{
    if (nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;

    if (ParentNode_npc && Npc[nNpcIndex].m_PifengType <= 0)
    {
        char nTempKey[64];
        sprintf(nTempKey, "npc_%d", nNpcIndex);
        std::string _npcKey = nTempKey;

        Sprite* nPartSpr = (Sprite*)(ParentNode_npc->getChildByName(_npcKey)->getChildByTag(UI_EQ_PART_PIFENG));
        if (!nPartSpr)
            return;

        if (nPartSpr->isVisible())
            nPartSpr->setVisible(false);
    }
}

bool KgameWorld::isFileExist(const char* pFileName)
{
    if (!pFileName)
        return false;
    // strFilePathName is :/data/data/ + package name
    std::string filePath = ax::FileUtils::getInstance()->getWritablePath();  // getWriteablePath();

    filePath += pFileName;

    FILE* fp = fopen(filePath.c_str(), "r");
    if (fp)
    {
        fclose(fp);
        fp = NULL;
        return true;
    }
    return false;
}

void KgameWorld::copyData(const char* pFileName)
{
    std::string strPath = ax::FileUtils::getInstance()->fullPathForFilename(pFileName);
    unsigned long len   = 0;
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
    len = ax::FileUtils::getInstance()->getFileSize(strPath);

    std::string destPath = ax::FileUtils::getInstance()->getWritablePath();  // getWriteablePath();
    destPath += pFileName;
    FILE* fp = fopen(destPath.c_str(), "w+");
    if (fp)
    {
        fwrite(data, sizeof(unsigned char), len, fp);
        fclose(fp);
        fp = NULL;
    }

    if (data)
    {
        delete[] data;
        data = NULL;
    }
}

/*
void KgameWorld::onEnter()
{

}

void KgameWorld::onExit()
{

}
//进入后 有 过渡完成
void KgameWorld::onEnterTransitionDidFinish()
{

}*/

/*bool KgameWorld::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
        return true;
}
void KgameWorld::ccTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void KgameWorld::ccTouchCancelled(Touch *pTouch, Event *pEvent)
{

}
void KgameWorld::ccTouchEnded(Touch *pTouch, Event *pEvent)
{//convertTouchToNodeSpace
}
*/
// default implements are used to call script callback if exist
void KgameWorld::ccTouchesBegan(const std::vector<ax::Touch*>& pTouches, Event* pEvent)
{  // Di chuột xuống
    // ccHideTitleBar(true);
    if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
        return;

    if (joystick)
    {
        bool nIsRun;
        nIsRun           = joystick->getIsRun();
        KuiItem* ItemPad = NULL;
        if (nIsRun)
        {  // 如果是腰杆移动结束了
            joystick->setIsRun(false);
            /*if  (m_itemsPadIsOpen)
            {
                    ItemPad  = (KuiItem *)this->getChildByTag(OBJ_NODE_ITEM);
                    if (ItemPad)
                            ItemPad->setCanMove(true);
            }*/
            return;
        }
        /*else
        {//遥杆移动中
                if  (m_itemsPadIsOpen)
                {
                        ItemPad  = (KuiItem *)this->getChildByTag(OBJ_NODE_ITEM);
                        if (ItemPad)
                                ItemPad->setCanMove(false);
                }
        }*/
    }

    if (m_tradePadIsOpen || m_skilPadIsOpen || m_itemsPadIsOpen || m_teamPadIsOpen || m_RoleStatePadIsOpen ||
        m_itemExPadIsOpen || m_NpcDialogPadIsOpen || m_talkDialogPadIsOpen || m_getstringPadIsOpen ||
        m_shopDialogPadIsOpen || m_storeboxPadIsOpen || m_npcshopPadIsOpen || m_givePadIsOpen)
    {
        return;
    }

    auto it      = pTouches.begin();
    Touch* touch = (Touch*)(*it);

    if (touch)
    {
        Point location = touch->getLocation();        // 目的的OpenGL坐标系(+x向右　+y向上)屏幕点击处
        screenPoint    = touch->getLocationInView();  // 屏幕坐标系(+x向右　+y向下)
        if (m_NpcIndex > 0 && g_pCoreShell)
        {  // nhân vật chính
            // if (g_pCoreShell->ThrowAwayItem())	//先扔掉手里的物品
            // return ;
            // int yyy=g_pCoreShell->GetAutoplayid();
            // if(yyy)
            // g_pCoreShell->OperationRequest(GOI_AUTOPALYOPEN,0,-1,100);    //动鼠标取消挂机状态[暂时取消该功能]

            // g_pCoreShell->OperationRequest(GOI_SWITCH_SKILLS, 0, 0);      //动鼠标丢弃拾取技能
            // g_pCoreShell->SceneMapOperation(GSMOI_DEL_GREEN_LINE, 0, 0);  //动鼠标就取消画黄线
            // g_pCoreShell->OperationRequest(GOI_JINDUTIAO_CALLBACK, 0, 0); //关闭进度条
            KUiPlayerItem SelectPlayer;
            int nNPCKind = -1;
            char nDebugmsg[200];
            sprintf(nDebugmsg, "Toa do 6:%d/%d", screenPoint.x, screenPoint.y);
            Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nDebugmsg);
            if (g_pCoreShell->FindSelectNPC(screenPoint.x, screenPoint.y, relation_all, false, &SelectPlayer, nNPCKind))
            {  // Liệu có NPC ở vị trí chuột hay không
                // int nRelation = g_pCoreShell->GetNPCRelation(SelectPlayer.nIndex);
                g_pCoreShell->LockSomeoneAction(SelectPlayer.nIndex);
            }
            else
                g_pCoreShell->LockSomeoneAction(0);

            int nObjKind   = -1;
            int nObjectIdx = 0;
            if (g_pCoreShell->FindSelectObject(screenPoint.x, screenPoint.y, false, nObjectIdx, nObjKind))
            {  // Cho dù có một mục ở vị trí của con chuột
                g_pCoreShell->LockObjectAction(nObjectIdx);
            }
            else
                g_pCoreShell->LockObjectAction(0);

            if (g_pCoreShell)
                g_pCoreShell->GotoWhere(screenPoint.x, screenPoint.y, 0);  // Đi đến điểm tọa độ chuột

            return;
        }  // end if m_NpcIndex>0
    }      // end touch
}

void KgameWorld::ccTouchesMoved(const std::vector<ax::Touch*>& pTouches, Event* pEvent)
{  // 拖动触摸的时候
}

void KgameWorld::ccTouchesEnded(const std::vector<ax::Touch*>& pTouches, Event* pEvent)
{  // 点击屏幕结束 鼠标放起

    /*if (m_skilPadIsOpen || m_itemsPadIsOpen ||m_teamPadIsOpen||m_RoleStatePadIsOpen||
            m_itemExPadIsOpen|| m_NpcDialogPadIsOpen||m_talkDialogPadIsOpen||m_getstringPadIsOpen ||
            m_shopDialogPadIsOpen || m_storeboxPadIsOpen || m_npcshopPadIsOpen)
             return;*/
}

void KgameWorld::ccTouchesCancelled(const std::vector<ax::Touch*>& pTouches, Event* pEvent)
{  // 触摸被改变的时候
}

void KgameWorld::didAccelerate(Acceleration* pAccelerationValue) {}

// 弹出对话框层
/*void KgameWorld::beiyonga(Ref* pSender)
{//模态对话框，只能对本曾进行操作
        // 定义一个弹出层，传入一张背景图 \\spr\\Ui4\\主界面\\武功技能资源\\武功技能底.spr
        //PopupLayer* pl = PopupLayer::create("laohuji.png"); //创建一个层 父节点
        PopupLayer* pl = PopupLayer::create("ui/state/skill.png",0);
        // ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放
        //pl->setContentSize(ax::Size(400, 360));
        //pl->setTitle(UTEXT("吾名一叶",1).c_str());
        //pl->setContentText(UTEXT("娇兰傲梅世人赏，却少幽芬暗里藏。不看百花共争艳，独爱疏樱一枝香。",1).c_str(), 20,
50, 150);
        // 设置回调函数，回调传回一个 Node 以获取 tag 判断点击的按钮
        // 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数了
        pl->setCallbackFunc(this, CC_CALLBACK_1(KgameWorld::buttonCallback));
        // 添加按钮，设置图片，文字，tag 信息
        pl->addButton("ok.png","ok.png","",0);      //在父节点下创建一个子节点按钮 0
        pl->addButton("close.png","close.png","",1);//在父节点下创建一个子节点按钮 1
        // 添加到当前层
        pl->setPosition(ax::Vec2(0,0));
        this->addChild(pl,ORDER_UI_DIALOD,OBJ_NODE_STATE);        //主场景中增加父节点
}*/
// 弹出对话框的回调函数
void KgameWorld::buttonCallback(Node* pNode)
{  // 在这个层中标记号，以确定是哪个按钮
#ifdef WIN32
    //_CCMessageBox("状态提示:", "button call back. tag: %d", pNode->getTag());
#endif
}

void KgameWorld::buttonSkillCallback(Node* pNode)
{  // 在这个层中标记号，以确定是哪个按钮
#ifdef WIN32
    //_CCMessageBox("技能提示:", "button call back. tag: %d", pNode->getTag());
#endif
}

// 外部调用 不能自动加载图片资源 ，必须预先加载纹理 否则出现黑色。。。
// 只能获取缓存中的数据
void KgameWorld::MoveObject(int ObjKind, int nIndex, bool nIsClear, int nPosX, int nPosY, int nPosZ)
{
    // KAutoCriticalSection AutoLock(m_GameProcessLock);
    char msg[64] = {0};
    switch (ObjKind)
    {
    case OBJ_NODE_MISS:
    {  // 子弹的绘画

        if (!ParentNode_miss || nIndex <= 0 || nIndex >= MAX_MISSLE)
            break;

        if (Node* nCurNode = ParentNode_miss->getChildByTag(nIndex))
        {  // 如果存在的了
            if (nIsClear)
            {
                nCurNode->setVisible(false);  // 先停止渲染
                // nCurNode->retain();
                ParentNode_miss->removeChild(nCurNode, true);  // 删除父节点下的某个子节点
                // nCurNode->autorelease();
                break;
            }
            // 更新位置 要在这里直接绘画
            int NewscrX = nPosX, NewscrY = nPosY;
            SubWorld[0].GetLocalPositionByMps(NewscrX, NewscrY, nPosZ);  // 像素坐标－－－＞屏幕坐标
            nCurNode->setPosition(ax::Vec2(NewscrX, visibleSize.height - NewscrY));  // GL坐标
            break;
        }
        Node* nMissNode = Node::create();
        if (!nMissNode)
            break;
        ParentNode_miss->addChild(nMissNode, 1, nIndex);
        int NewscrX = nPosX, NewscrY = nPosY;
        SubWorld[0].GetLocalPositionByMps(NewscrX, NewscrY, nPosZ);  // 像素坐标－－－＞屏幕坐标
        NewscrY = visibleSize.height - NewscrY;
        nMissNode->setPosition(ax::Vec2(NewscrX, NewscrY));  // GL坐标

        /* Sprite *zMissSpr=Sprite::create();
         if (zMissSpr)
         {
                 zMissSpr->setAnchorPoint(ax::Vec2(0,0));
                 nMissNode->addChild(zMissSpr,2,1);
         }*/
    }
    break;
    case OBJ_NODE_OBJSET:
    {  // 地上的物品
        if (!ParentNode_obj || nIndex <= 0 || nIndex >= MAX_OBJECT)
            break;
        if (Node* nCurNode = ParentNode_obj->getChildByTag(nIndex))
        {
            if (nIsClear)
            {
                nCurNode->setVisible(false);  // 先停止渲染
                // nCurNode->retain();
                ParentNode_obj->removeChild(nCurNode, true);  // 删除父节点下的某个子节点
                // nCurNode->autorelease();
                break;
            }
            // 更新位置
            int NewscrX = nPosX, NewscrY = nPosY;
            SubWorld[0].GetLocalPositionByMps(NewscrX, NewscrY, nPosZ);  // 像素坐标－－－＞屏幕坐标
            nCurNode->setPosition(ax::Vec2(NewscrX, visibleSize.height - NewscrY));  // GL坐标
            break;
        }
        Node* nObjNode = Node::create();
        if (!nObjNode || nObjNode == NULL)
            break;
        ParentNode_obj->addChild(nObjNode, 0, nIndex);

        int NewscrX = nPosX, NewscrY = nPosY;
        SubWorld[0].GetLocalPositionByMps(NewscrX, NewscrY, nPosZ);  // 像素坐标－－－＞屏幕坐标
        NewscrY = visibleSize.height - NewscrY;
        nObjNode->setPosition(ax::Vec2(NewscrX, NewscrY));  // GL坐标

        /* Sprite *zObjSpr=Sprite::create();
         if (zObjSpr)
         {
                 zObjSpr->setAnchorPoint(ax::Vec2(0,0));
                 nObjNode->addChild(zObjSpr,1,1);
         }

         Size nTempSize;
         nTempSize.width  = 100;
         nTempSize.height = 14;
         Label *objnamelabel = Label::createWithTTF("",UI_GAME_FONT_DEFAULT,12,nTempSize,TextHAlignment::CENTER);//Arial
         //objnamelabel->setColor(ax::Color3B::YELLOW);
         objnamelabel->setPosition(ax::Vec2(0,20));
         nObjNode->addChild(objnamelabel,1,2);
         */
    }
    break;
    case OBJ_NODE_UI:
        break;
    case OBJ_NODE_MAP:
        break;
    case OBJ_NODE_NPC:
    {  // NPC 的各个部位的信息
        if (nIndex <= 0 || nIndex >= MAX_NPC)
            break;
        // 父节点                     --子节点 --------------孙节点
        // ParentNode_npc 大的类型节点 nCurNode一个NPC的节点（里面还有各个装备的节点）
        // if (Node * nCurNode = ParentNode_npc->getChildByTag(nIndex))
        char nKey[64];
        sprintf(nKey, "npc_%d", nIndex);
        std::string npcKey = nKey;

        if (Node* nCurNode = (Node*)ParentNode_npc->getChildByName(npcKey))
        {
            if (nIsClear && Player[CLIENT_PLAYER_INDEX].m_nIndex != nIndex)
            {  // 主角不能删除
               // 删除名字 称号等层
                char nKey[64];
                sprintf(nKey, "other_%d", nIndex);
                std::string otherKey = nKey;
                ParentNode_npcother->removeChildByName(otherKey, true);
                sprintf(nKey, "blood_%d", nIndex);
                otherKey = nKey;
                ParentNode_npcblood->removeChildByName(otherKey, true);

                nCurNode->setVisible(false);  // 先停止渲染
                // nCurNode->removeAllChildrenWithCleanup(true);//删除子节点下的所有子节点
                // if (!nCurNode->isVisible())
                // nCurNode->retain();
                ParentNode_npc->removeChild(nCurNode, true);  // 删除父节点下的某个子节点

                break;
            }
            // 更新位置
            int NewscrX = nPosX, NewscrY = nPosY;
            SubWorld[0].GetLocalPositionByMps(NewscrX, NewscrY, nPosZ);  // 像素坐标－－－＞屏幕坐标
            nCurNode->setPosition(ax::Vec2(NewscrX, visibleSize.height - NewscrY));  // GL坐标
            // 设置偏色
            // ax::Color3B nAdiustColor = {255,255,255};
            if (Player[CLIENT_PLAYER_INDEX].m_nIndex == nIndex)
            {
                if (joystick)  // 设置主角的坐标
                    joystick->setRolePoint(ax::Vec2(NewscrX, visibleSize.height - NewscrY));

                if (KskillMenu)  // 设置主角的坐标
                    KskillMenu->setRolePoint(ax::Vec2(NewscrX, visibleSize.height - NewscrY));
            }
            break;
        }
        // Node *nNpcNode = Node::create();
        Node* nNpcNode = Node::create();
        if (!nNpcNode || nNpcNode == NULL)
            break;
        nNpcNode->setName(npcKey);
        ParentNode_npc->addChild(nNpcNode, 1, nIndex);
        // 父-子(父--子) 先父后子，否则释放的时候　会出现问题
        int NewscrX = nPosX, NewscrY = nPosY;
        SubWorld[0].GetLocalPositionByMps(NewscrX, NewscrY, nPosZ);              // 像素坐标－－－＞屏幕坐标
        nNpcNode->setPosition(ax::Vec2(NewscrX, visibleSize.height - NewscrY));  // GL坐标
    }                                                                            // end case
    break;
    case OBJ_NODE_ITEM:
        break;
    default:
        break;
    }
}
/*
Node *KgameWorld::GetMainWorldNode(int nNodeKind)
{
        switch(nNodeKind)
        {
        case OBJ_NODE_UI:
                break;
        case OBJ_NODE_MAP:
                break;
        case OBJ_NODE_NPC:
                {//NPC 的各个部位的信息
                   return ParentNode_npc;
                }
                break;
        case OBJ_NODE_MISS:
                break;
        case OBJ_NODE_ITEM:
                break;
        default:
                break;
        }
        return NULL;
}
*/
void KgameWorld::RemoveObject(int ObjKind, int nIndex)
{
    // KAutoCriticalSection AutoLock(m_GameProcessLock);
    switch (ObjKind)
    {
    case OBJ_NODE_UI:
        break;
    case OBJ_NODE_MAP:
        break;
    case OBJ_NODE_NPC:
    {
        char nTempKey[64];
        sprintf(nTempKey, "npc_%d", nIndex);
        std::string _npcKey = nTempKey;
        if (Node* nCurNode = ParentNode_npc->getChildByName(_npcKey))
        {
            // nCurNode->removeFromParentAndCleanup(true);
            nCurNode->removeAllChildrenWithCleanup(true);  // 删除子节点下的所有子节点
            ParentNode_npc->removeChild(nCurNode, true);   // 删除父节点下的某个子节点
            // if (ParentNode_npc)//重新排列所有子节点
            //	ParentNode_npc->sortAllChildren();
            // ParentNode_npc->removeChildByTag(nIndex,true);
            // removeFromParent();
        }
    }
    break;
    default:
        break;
    }
    return;
}

void KgameWorld::ergodicAllNode(Node* rootNode)
{
    Ref* temp;
    // 获得rootnode根下的节点数组
    const auto& nodeArray = rootNode->getChildren();
    for (const auto& temp : nodeArray)
    {
        // 判断rootnode的节点下还是否存在节点 遍历调用
        if (((Node*)temp)->getChildrenCount())  // 如果子节点下 还有节点则再历遍
            ergodicAllNode((Node*)temp);
        // 这里 do something
    }
}
/*
void KgameWorld::AddTreeIndex(int i)
{
        m_FreeIdxTreeNode.Remove(i);
        m_UseIdxTreeNode.Insert(i);
}

void KgameWorld::AddHouseIndex(int i)
{
        m_FreeIdxHouseNode.Remove(i);
        m_UseIdxHouseNode.Insert(i);
}

int	  KgameWorld::FindTreeFree()
{
        return m_FreeIdxTreeNode.GetNext(0);
}

int	  KgameWorld::FindHouseFree()
{
        return m_FreeIdxHouseNode.GetNext(0);
}
*/
/*
int	KgameWorld::FindFree(int nRegIndex)
{
        if (RegData)
           return RegData[nRegIndex].m_FreeIdxNode.GetNext(0);

           return 0;
}

int	KgameWorld::FindPartFree(int nRegIndex)
{
        if (RegData)
                return RegData[nRegIndex].m_FreeIdxPartNode.GetNext(0);

        return 0;
}


void KgameWorld::AddPartIndex(int nRegIndex,int i)
{
        if (RegData)
        {
                RegData[nRegIndex].m_FreeIdxPartNode.Remove(i);
                RegData[nRegIndex].m_UseIdxPartNode.Insert(i);
        }
}

void KgameWorld::AddIndex(int nRegIndex,int i)
{
        if (RegData)
        {
                RegData[nRegIndex].m_FreeIdxNode.Remove(i);
                RegData[nRegIndex].m_UseIdxNode.Insert(i);
        }
}

int   KgameWorld::GetCount(int nRegIndex)
{
        if (RegData)
        {
                return RegData[nRegIndex].m_UseIdxNode.GetCount();
        }
        return 0;
}

void   KgameWorld::ReSetUseNode(int nRegIndex)
{
        if (RegData)
        {
                int nIdx = RegData[nRegIndex].m_UseIdxNode.GetNext(0);
                int nIdx1 = 0;
                while(nIdx)
                {
                        nIdx1 = RegData[nRegIndex].m_UseIdxNode.GetNext(nIdx);
                        RegData[nRegIndex].m_FreeIdxNode.Insert(nIdx);
                        RegData[nRegIndex].m_UseIdxNode.Remove(nIdx);
                        nIdx = nIdx1;
                }
        }
}


void   KgameWorld::ReSetUsePartNode(int nRegIndex)
{
        if (RegData)
        {
                int nIdx = RegData[nRegIndex].m_UseIdxPartNode.GetNext(0);
                int nIdx1 = 0;
                while(nIdx)
                {
                        nIdx1 = RegData[nRegIndex].m_UseIdxPartNode.GetNext(nIdx);
                        RegData[nRegIndex].m_FreeIdxPartNode.Insert(nIdx);
                        RegData[nRegIndex].m_UseIdxPartNode.Remove(nIdx);
                        nIdx = nIdx1;
                }
        }
}
*/
/*
void  KgameWorld::ReSetTreeNode()
{
        int nIdx = m_UseIdxTreeNode.GetNext(0);
        int nIdx1 = 0;
        while(nIdx)
        {
                nIdx1 = m_UseIdxTreeNode.GetNext(nIdx);
                m_FreeIdxTreeNode.Insert(nIdx);
                m_UseIdxTreeNode.Remove(nIdx);
                nIdx = nIdx1;
        }
}

void  KgameWorld::ReSetHouseNode()
{
        int nIdx = m_UseIdxHouseNode.GetNext(0);
        int nIdx1 = 0;
        while(nIdx)
        {
                nIdx1 = m_UseIdxHouseNode.GetNext(nIdx);
                m_FreeIdxHouseNode.Insert(nIdx);
                m_UseIdxHouseNode.Remove(nIdx);
                nIdx = nIdx1;
        }
}

int  KgameWorld::GetTreeNodeCount()
{
        return m_UseIdxTreeNode.GetCount();
}

int  KgameWorld::GetHouseNodeCount()
{
        return m_UseIdxHouseNode.GetCount();
}
*/
/*
void KgameWorld::RemoveIndex(int nRegIndex,int i)
{
        if (RegData)
        {
                RegData[nRegIndex].m_FreeIdxNode.Insert(i);        //删除链表
                RegData[nRegIndex].m_UseIdxNode.Remove(i);
        }
}

void KgameWorld::SetOtherSprCount(int nRegIndex,int i)
{
        nOtherSprCount[nRegIndex]=i;
}

int KgameWorld::GetOtherSprCount(int nRegIndex)
{
        return nOtherSprCount[nRegIndex];
}

void KgameWorld::SetPartSprCount(int nRegIndex,int i)
{
        nPartSprCount[nRegIndex]=i;
}

int KgameWorld::GetPartSprCount(int nRegIndex)
{
        return nPartSprCount[nRegIndex];
}

int KgameWorld::GetRegDrawKind(int nRegIndex)
{
        if (RegData)
        {
                return RegData[nRegIndex].nDrawKind;        //删除链表
        }
        return -1;
}
*/

/*int KgameWorld::GetRegSprCount(int nRegIndex)
{
        if (RegData)
        {
                return RegData[nRegIndex].nSprCount;        //删除链表
        }
        return 0;
}*/

// 右键技能

int KgameWorld::getIndexBySkillID(int nSkillIdx)
{
    for (int i = 0; i < MAX_FUZHUSKILL_COUNT; i++)
    {
        if (auxiliaryskill[i].m_skillidx == nSkillIdx)
        {  // 重复点就删除
            return i;
        }
    }
    return -1;
}

void KgameWorld::setaauxiliaryskillInfo(int nSkillIdx, int uGenre, char* icoPath)
{
    int nCount = 0;
    if (KskillMenu && nSkillIdx > 0 && nSkillIdx < MAX_SKILL && icoPath)
    {
        ISkill* pOrdinSkill = g_SkillManager.GetSkill(nSkillIdx, 1);

        int i;
        bool isReturn = false;
        for (i = 0; i < MAX_FUZHUSKILL_COUNT; i++)
        {
            if (auxiliaryskill[i].m_skillidx == nSkillIdx)
            {  // 重复点就删除
                if (pOrdinSkill && pOrdinSkill->IsAura())
                    Npc[m_NpcIndex].SetAuraSkill(0);
                if (KskillMenu)
                    KskillMenu->removeChild(auxiliaryskill[i].auxiliaryskillSpr, true);
                // pcontrolMenu->removeChild(auxiliaryskill[i].auxiliaryskillSpr);
                auxiliaryskill[i].auxiliaryskillSpr = NULL;
                auxiliaryskill[i].m_skillidx        = 0;
                isReturn                            = true;
            }
        }

        if (isReturn)
            return;

        for (i = 0; i < MAX_FUZHUSKILL_COUNT; i++)
        {
            if (auxiliaryskill[i].m_skillidx == nSkillIdx)
            {  // 重复点就删除
                if (pOrdinSkill && pOrdinSkill->IsAura())
                    Npc[m_NpcIndex].SetAuraSkill(0);
                if (KskillMenu)
                    KskillMenu->removeChild(auxiliaryskill[i].auxiliaryskillSpr, true);
                // pcontrolMenu->removeChild(auxiliaryskill[i].auxiliaryskillSpr);
                auxiliaryskill[i].auxiliaryskillSpr = NULL;
                auxiliaryskill[i].m_skillidx        = 0;

                break;
            }

            if (auxiliaryskill[i].m_skillidx <= 0)
            {  // 有空位置
                auxiliaryskill[i].m_skillidx = nSkillIdx;

                Texture2D* bgCur = NULL;
                int nTempX, nTempY, nCavesWidth = 0, nCavesHeight = 0;
                bgCur = ccgetTxtData(icoPath, 0, 0, 0, 0, &nTempX, &nTempY, 0, 1, &nCavesWidth, &nCavesHeight);
                if (bgCur)
                {  // 修改精灵的纹理
                    Sprite* btn_normal_sprite = Sprite::createWithTexture(bgCur);
                    Sprite* btn_select_sprite = Sprite::createWithTexture(bgCur);

                    // 通过sequence重复执行两个渐变动作
                    FadeTo* fadeto88           = FadeTo::create(0.5, 88);
                    FadeTo* fadeto255          = FadeTo::create(1, 255);
                    ActionInterval* fadeaction = Sequence::create(fadeto88, fadeto255, NULL);
                    btn_normal_sprite->runAction(RepeatForever::create(fadeaction));
                    // static MenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite,
                    // Ref* target, SEL_MenuHandler selector)
                    auxiliaryskill[i].auxiliaryskillSpr =
                        MenuItemSprite::create(btn_normal_sprite, btn_select_sprite, NULL,
                                               CC_CALLBACK_1(KgameWorld::auxiliaryskillCallback, this));

                    if (i == 0)
                    {
                        auxiliaryskill[i].OffsetX =
                            origin.x + visibleSize.width - bgCur->getContentSize().width / 2 - 150;      // 100
                        auxiliaryskill[i].OffsetY = origin.y + bgCur->getContentSize().height / 2 + 30;  // 10
                    }
                    else
                    {
                        switch (i)
                        {
                        case 1:
                        {
                            auxiliaryskill[i].OffsetX = auxiliaryskill[i - 1].OffsetX;
                            auxiliaryskill[i].OffsetY = auxiliaryskill[i - 1].OffsetY + 50;
                        }
                        break;
                        case 2:
                        {
                            auxiliaryskill[i].OffsetX = auxiliaryskill[i - 1].OffsetX + 15;
                            auxiliaryskill[i].OffsetY = auxiliaryskill[i - 1].OffsetY + 50;
                        }
                        break;
                        default:
                        {  // 3,4
                            auxiliaryskill[i].OffsetX = auxiliaryskill[i - 1].OffsetX + 50;
                            auxiliaryskill[i].OffsetY = auxiliaryskill[i - 1].OffsetY;
                        }
                        break;
                        }
                    }
                    auxiliaryskill[i].auxiliaryskillSpr->setScale(1.2);
                    auxiliaryskill[i].auxiliaryskillSpr->setPosition(
                        ax::Vec2(auxiliaryskill[i].OffsetX, auxiliaryskill[i].OffsetY));
                    // skillstick[i].skillstick->setSpritePiont(ax::Vec2(auxiliaryskill[i].OffsetX,auxiliaryskill[i].OffsetY));
                    // pcontrolMenu->addChild(auxiliaryskill[i].auxiliaryskillSpr,nSkillIdx);
                    if (KskillMenu)
                        KskillMenu->addChild(auxiliaryskill[i].auxiliaryskillSpr, nSkillIdx);
                    if (pOrdinSkill)
                    {
                        if (pOrdinSkill->IsAura())
                        {  // 如果是光环技能 就立即使用
                            Npc[m_NpcIndex].SetAuraSkill(nSkillIdx);
                        }
                        else
                        {  // 否则，设置为不是光环技能
                           // 设置攻击技能
                           // Npc[m_NpcIndex].SetAuraSkill(0);
                        }
                    }
                }
                break;
            }
        }
    }
}
// set skill tấn công
void KgameWorld::setattackSprInfo(int nSkillIdx, int uGenre, char* icoPath)
{

    if (nSkillIdx > 0 && nSkillIdx < MAX_SKILL && icoPath)
    {
        mainattackSkill = nSkillIdx;
        Player[CLIENT_PLAYER_INDEX].SetLeftSkill(nSkillIdx, true);  // 设置攻击技能

        char nCurPath[128];
        sprintf(nCurPath, "%s", icoPath);
        g_StrLower(nCurPath);
        int m_nWidth, m_nHeight, nFrams;
        Texture2D* bgCur = NULL;
        SPRFRAMSINFO nSprInfo;
        ZeroMemory(&nSprInfo, sizeof(nSprInfo));
        bgCur = _getinidata.getinidata_one(nCurPath, 0, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
        if (bgCur == NULL)
            return;

        if (psortSkillItem_l)
        {
            Sprite* btn_normal_sprite = Sprite::createWithTexture(bgCur);
            psortSkillItem_l->setNormalImage(btn_normal_sprite);
            psortSkillItem_l->setSelectedImage(btn_normal_sprite);
            // psortSkillItem_l->setDisabledImage(btn_normal_sprite);
        }

        // messageBox(icoPath,"设置成功");
        /* if (!attackSpr)
         {
                 attackSpr =Sprite::createWithTexture(bgCur);
                 this->addChild(attackSpr,4);
                 attackSpr->setAnchorPoint(ax::Vec2(0,0));
                 attackSpr->setPosition(ax::Vec2((visibleSize.width/2+150),10));
                 return;
         }
          attackSpr->setTexture(bgCur);*/
        // Size m_size = bgCur->getContentSize();
        // attackSpr->setContentSize(m_size);
        // attackSpr->setTexture(bgCur);
    }
}
/*
int KgameWorld::MakeClip(long nX, long nY, long nWidth, long nHeight, KClipper* pClipper)
{
        // 初始化裁减量
        pClipper->x = nX; //位置
        pClipper->y = nY; //位置
        pClipper->width = nWidth; //宽度
        pClipper->height = nHeight;//高度
        pClipper->top = 0;
        pClipper->left = 0;
        pClipper->right = 0;

        // 上边界裁减
        if (pClipper->y < m_ClipRect.top)
        {
                pClipper->y = m_ClipRect.top;
                pClipper->top = m_ClipRect.top - nY;
                pClipper->height -= pClipper->top;
        }
        if (pClipper->height <= 0)
                return FALSE;

        // 下边界裁减
        if (pClipper->height > m_ClipRect.bottom - pClipper->y)
        {
                pClipper->height = m_ClipRect.bottom - pClipper->y;
        }
        if (pClipper->height <= 0)
                return FALSE;

        // 左边界裁减
        if (pClipper->x < m_ClipRect.left)
        {
                pClipper->x = m_ClipRect.left;
                pClipper->left = m_ClipRect.left - nX;
                pClipper->width -= pClipper->left;
        }
        if (pClipper->width <= 0)
                return FALSE;

        // 右边界裁减
        if (pClipper->width > m_ClipRect.right - pClipper->x)
        {
                pClipper->right = pClipper->width + pClipper->x - m_ClipRect.right;
                pClipper->width -= pClipper->right;
        }
        if (pClipper->width <= 0)
                return FALSE;

        return TRUE;
}*/
// 聊天系统

int KgameWorld::FilterTextColor(char* pMsgBuff, unsigned short nMsgLength)
{
    nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_ENTER);  // 禁止回车代码
    nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_COLOR);  // 禁止颜色代码
    // nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_COLOR_RESTORE);  //禁止颜色复原
    nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_BORDER_COLOR);  // 禁止描边设置
    // nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_BORDER_RESTORE); //禁止描边设置
    nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_INLINE_PIC);  // 禁止内图设置
    // nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_LINK);           //禁止连接设置
    //	return TEncodeText(pMsgBuff, nMsgLength);
    return nMsgLength;
}

void KgameWorld::ChannelMessageArrival(UINT nChannelID,
                                       char* szSendName,
                                       char* pMsgBuff,
                                       unsigned short nMsgLength,
                                       bool bSucc,
                                       UINT nItemDwidx,
                                       int nPackage)
{  // 公告 通知等等
    if (!pMsgBuff || !sfunLayer)
        return;

    if (!bSucc)
    {
        // char szInfo[256];
        // int n = sprintf(szInfo, "抱歉, 频道还未开放,消息无法发到!");
        // KUiMsgCentrePad::SystemMessageArrival(szInfo, n);
        return;
    }

    int nIndex = -1;
    if (nChannelID == -1)  // gm alias ID
    {
        // 找到系统频道
        int nChannelDataCount = ((Kuiplayerfun*)sfunLayer)->GetChannelCount();
        for (int n = 0; n < nChannelDataCount; n++)
        {
            if (((Kuiplayerfun*)sfunLayer)->IsChannelType(n, ch_GM))
            {  // GM频
                nIndex = n;
                break;
            }
        }
        if (nIndex < 0)
            return;

        nChannelID = ((Kuiplayerfun*)sfunLayer)->GetChannelID(nIndex);
        if (nChannelID == -1)
            return;
    }
    else
    {
        nIndex = ((Kuiplayerfun*)sfunLayer)->GetChannelIndex(nChannelID);
        if (nIndex < 0)
            return;
        // if (IsInBlackName(szSendName))  //在黑名单中
        //	 return;
    }

    // int msglen = TEncodeText(pMsgBuff,nMsgLength);//去掉控制符号
    char nTempstr[512];
    ZeroMemory(nTempstr, sizeof(nTempstr));
    memcpy(&nTempstr[0], pMsgBuff, nMsgLength);
    nMsgLength           = FilterTextColor(nTempstr, nMsgLength);
    nTempstr[nMsgLength] = '\0';
    char nTempSendname[64];
    sprintf(nTempSendname, "%s", szSendName);  // nChannelID
    if (chatLayer)
    {  // 主屏幕
       //  chatLayer->addmsg(nTempSendname,nTempstr);
        chatLayer->NewChannelMessageArrival(nChannelID, nTempSendname, nTempstr, nMsgLength, nItemDwidx, szSendName,
                                            nPackage);
    }
    // 聊天面板信息推送
    ((Kuiplayerfun*)sfunLayer)
        ->NewChannelMessageArrival(nChannelID, nTempSendname, nTempstr, nMsgLength, nItemDwidx, szSendName, nPackage);

    // 头顶信息显示
    if (((Kuiplayerfun*)sfunLayer)->GetChannelSubscribe(nIndex) &&
        ((Kuiplayerfun*)sfunLayer)->IsChannelType(nIndex, ch_Screen))
    {
        KUiPlayerItem SelectPlayer;
        int nKind = -1;
        if (g_pCoreShell->FindSpecialNPC(szSendName, &SelectPlayer, nKind) && nKind == kind_player)
        {  ////自己头顶聊天信息显示
           // g_pCoreShell->ChatSpecialPlayer(&SelectPlayer, pMsgBuff, nMsgLength);  //发送自己聊天信息显示
        }
    }
    return;
}

// trò chuyện riêng tư
void KgameWorld::MSNMessageArrival(char* szSourceName,
                                   char* szSendName,
                                   const char* pMsgBuff,
                                   unsigned short nMsgLength,
                                   bool bSucc,
                                   UINT nItemDwidx,
                                   bool isClent)
{
    if (!szSourceName || !pMsgBuff)
        return;
    if (sfunLayer)
        ((Kuiplayerfun*)sfunLayer)->ShowMSNMessage(szSourceName, pMsgBuff, nMsgLength, nItemDwidx, isClent);
    // 私聊
    if (chatLayer)
    {
        char Buffer[512];
        char nTempSendName[64];
        ZeroMemory(nTempSendName, sizeof(nTempSendName));
        ZeroMemory(Buffer, sizeof(Buffer));
        int nOffset = 0;
        /*int nLen = 0;
        nLen = strlen(szName);
        memcpy(Buffer + nOffset,szName, nLen);
        nOffset += nLen;
        Buffer[nOffset++] = ':'*/
        memcpy(&Buffer[nOffset], pMsgBuff, nMsgLength);
        // n/Offset += FilterTextColor(Buffer + nOffset, nMsgLength);
        Buffer[nMsgLength] = '\0';
        if (isClent)
            sprintf(nTempSendName, "[%s]:", szSourceName);
        else
            sprintf(nTempSendName, "[%s]:", szSourceName);

#ifdef WIN32
        chatLayer->addmsg(nTempSendName, Buffer, 12, isClent);
#else
        if (!isClent)  // 如果是客户端就不需要转换了
            sprintf(nTempSendName, "%s", UTEXT(nTempSendName, 1).c_str());

        // sprintf(Buffer,UTEXT(Buffer,1).c_str());
        chatLayer->addmsg(nTempSendName, Buffer, 12, isClent);
#endif
    }
}

void KgameWorld::QueryAllChannel()
{
    // messageBox("请求增加频道","请求增加频道");
    if (sfunLayer)
    {
        ((Kuiplayerfun*)sfunLayer)->ReleaseActivateChannelAll();
        ((Kuiplayerfun*)sfunLayer)->QueryAllChannel();
    }
}

void KgameWorld::NotifyChannelID(char* ChannelName, UINT channelid, BYTE cost)
{  // Thông báo thay đổi kênh
    // messageBox("通知频道信息到达",ChannelName);
    if (sfunLayer)
        ((Kuiplayerfun*)sfunLayer)->OpenChannel(ChannelName, channelid, cost);  // 打开频道
}

// Thông tin hệ thống đến
bool KgameWorld::AMessageArrival(KSystemMessage* pMsg, void* pParam)  // 信息到达
{
    if (pMsg == NULL || this == NULL || (pMsg->byParamSize != 0 && pParam == NULL))
        return false;
    // sMsg.eType = SMT_TEAM;
    // sMsg.byConfirmType = SMCT_UI_TEAM_INVITE;
    switch (pMsg->eType)
    {
    case SMT_CLIQUE:  // 帮会
    {
        switch (pMsg->byConfirmType)
        {
        case SMCT_UI_TONG_JOIN_APPLY:
        {  // 通知有人申请入帮派
            if (m_talkDialogPadIsOpen_1)
                break;
            KUiPlayerItem* nTempTong = (KUiPlayerItem*)pParam;
            if (!nTempTong)
                break;
            m_talkDialogPadIsOpen_1 = true;
            auto callback           = [this](ax::Node* sender) { this->closeNewDialogPadCallback(sender); };
            Layer* sDialogLayer_1 =
                KuiTalkDialog1::create(pMsg->szMessage, "Đồng ý tham gia", "Từ chối tham gia", SMCT_UI_TONG_JOIN_APPLY,
                                       this, callback, nTempTong->nIndex, nTempTong->Name);
            this->addChild(sDialogLayer_1, ORDER_UI_DIALOD, OBJ_NODE_TALKDIALOG_NEW);
        }
        break;
        default:
            break;
        }
    }
    break;
    case SMT_TEAM:
    {  // 队伍
        switch (pMsg->byConfirmType)
        {
        case SMCT_UI_TEAM:
        {
            int nMsgLength = strlen(pMsg->szMessage);
            nMsgLength     = FilterTextColor(pMsg->szMessage, nMsgLength);
            if (chatLayer)
                chatLayer->addmsg("[Nhóm]:", pMsg->szMessage);
        }
        break;
        case SMCT_UI_TEAM_APPLY:
        {  // 答应或拒绝加入队伍的申请,
            // messageBox("同意入队","test");
            if (m_talkDialogPadIsOpen_1)
                break;
            //						this->removeChildByTag(OBJ_NODE_TALKDIALOG_NEW,true);
            unsigned int nTarNpcDwid = 0;
            KUiPlayerItem* nTempTeam = (KUiPlayerItem*)pParam;
            if (nTempTeam)
            {
                nTarNpcDwid = nTempTeam->uId;
            }

            m_talkDialogPadIsOpen_1 = true;
            auto callback           = [this](ax::Node* sender) { this->closeNewDialogPadCallback(sender); };
            Layer* sDialogLayer_1 = KuiTalkDialog1::create(pMsg->szMessage, "Đồng ý tham gia", "Từ chối tham gia",
                                                           SMCT_UI_TEAM_APPLY, this, callback, nTarNpcDwid);
            this->addChild(sDialogLayer_1, ORDER_UI_DIALOD, OBJ_NODE_TALKDIALOG_NEW);
        }
        break;
        case SMCT_UI_TEAM_INVITE:
        {  // 邀请入队的应答
            // messageBox("邀请入队","test");
            if (m_talkDialogPadIsOpen_1)
                break;
            //						this->removeChildByTag(OBJ_NODE_TALKDIALOG_NEW,true);
            int nTarPlaerindex       = 0;
            KUiPlayerItem* nTempTeam = (KUiPlayerItem*)pParam;
            if (nTempTeam)
            {
                nTarPlaerindex = nTempTeam->nIndex;
            }

            m_talkDialogPadIsOpen_1 = true;
            auto callback           = [this](ax::Node* sender) { this->closeNewDialogPadCallback(sender); };
            Layer* sDialogLayer_1 = KuiTalkDialog1::create(pMsg->szMessage, "Đồng ý tham gia", "Từ chối tham gia",
                                                           SMCT_UI_TEAM_INVITE, this, callback, nTarPlaerindex);
            this->addChild(sDialogLayer_1, ORDER_UI_DIALOD, OBJ_NODE_TALKDIALOG_NEW);
        }
        break;
        default:
            break;
        }
    }
    break;
    case SMT_NORMAL:
    case SMT_SYSTEM:
    {
        switch (pMsg->byConfirmType)
        {
        case SMCT_UI_TRADE:
        {  // 交易
            if (m_talkDialogPadIsOpen_1)
                break;
            // strcpy(sPlayer.Name, Npc[nNpcIdx].Name);
            // sPlayer.nIndex = pApply->m_nDestIdx;
            // sPlayer.uId = pApply->m_dwNpcId; KUiPlayerItem
            int nTarPlaerindex       = 0;
            KUiPlayerItem* nTempTeam = (KUiPlayerItem*)pParam;
            if (nTempTeam)
            {
                nTarPlaerindex = nTempTeam->nIndex;
            }

            m_talkDialogPadIsOpen_1 = true;
            auto callback           = [this](ax::Node* sender) { this->closeNewDialogPadCallback(sender); };
            Layer* sDialogLayer_1 = KuiTalkDialog1::create(pMsg->szMessage, "Đồng ý giao dịch", "Từ chối giao dịch",
                                                           SMCT_UI_TRADE, this, callback, nTarPlaerindex);
            this->addChild(sDialogLayer_1, ORDER_UI_DIALOD, OBJ_NODE_TALKDIALOG_NEW);
        }
        break;
        default:
        {
            int nMsgLength = strlen(pMsg->szMessage);
            nMsgLength     = FilterTextColor(pMsg->szMessage, nMsgLength);
            if (chatLayer)
            {
                chatLayer->addmsg("[Chú ý]:", pMsg->szMessage);
                break;
            }
            /*char nKey[32];
            std::string chatKey = "chat_key";
            ui::RichText  * chatRickText=NULL;
            LayerColor * chagbgspr =(LayerColor *)ParentNode_chat->getChildByTag(CHAT_NODE_BGSPR);
            chatRickText = (ui::RichText  *)chagbgspr->getChildByName(chatKey);
            Rect CCchatRect = ax::Rect(0,0,chagbgspr->getContentSize().width,80);
            Size nParentSize = chagbgspr->getContentSize();
            if (!chatRickText)
            {//就创建
                    chatRickText = ui::RichText::createWithParentSize(nParentSize);
                    chatRickText->setContentSize(CCchatRect.size);
                    chatRickText->ignoreContentAdaptWithSize(false);//是否忽略内容适应高宽度  就一行
                    chatRickText->setAnchorPoint(ax::Vec2(0,0));
                    chatRickText->setTag(chatKey);
                    chagbgspr->addChild(chatRickText,1);
            }
            chatRickText->clearAllElement();
            chatRickText->setContentSize(CCchatRect.size);
            float nCurFontSize = 12;
            //sprintf(nTempSendname,"[提示]:");
            ui::RichElementText* senname =
            ui::RichElementText::create(1,ax::Color3B::RED,255,UTEXT("[提示]:",1).c_str(),UI_GAME_FONT_DEFAULT,nCurFontSize);
            ui::RichElementText* Msg     =
            ui::RichElementText::create(2,ax::Color3B::YELLOW,255,UTEXT(pMsg->szMessage,1).c_str(),UI_GAME_FONT_DEFAULT,nCurFontSize);
            chatRickText->pushBackElement(senname);
            chatRickText->pushBackElement(Msg);
            Size nRenderSize = chatRickText->getVirtualRendererSize();
            chatRickText->setContentSize(nRenderSize);*/
        }
        break;
        }
    }
    break;
    case SMT_PLAYER:
    {
        switch (pMsg->byConfirmType)
        {
        case SMCT_UI_RENASCENCE:
        {
            if (m_NpcDialogPadIsOpen)
                this->removeChildByTag(OBJ_NODE_NPCDIALOG, true);

            if (m_talkDialogPadIsOpen)
            {
                this->removeChildByTag(OBJ_NODE_TALKDIALOG, true);
                m_talkDialogPadIsOpen = false;
            }
            if (m_givePadIsOpen)
            {
                this->removeChildByTag(OBJ_NODE_GIVE, true);
                m_givePadIsOpen = false;
            }
            if (m_npcshopPadIsOpen)
            {
                this->removeChildByTag(OBJ_NODE_NPCSHOP, true);
                m_npcshopPadIsOpen = false;
            }
            if (m_laotouPadIsOpen)
            {
                this->removeChildByTag(OBJ_NODE_LAOTOU, true);
                m_laotouPadIsOpen = false;
            }

            if (m_talkDialogPadIsOpen_1)
                this->removeChildByTag(OBJ_NODE_TALKDIALOG_NEW, true);

#define MSG_RENASCENCE "你已经身受重伤!"
            auto callback = [this](ax::Node* sender) { this->closeNewDialogPadCallback(sender); };
            Layer* sDialogLayer_1 = KuiTalkDialog1::create(MSG_RENASCENCE, "Về thành dưỡng sức", "Hồi sinh tại chỗ",
                                                           SMCT_UI_RENASCENCE, this, callback);
            m_talkDialogPadIsOpen_1 = true;
            this->addChild(sDialogLayer_1, ORDER_UI_DIALOD, OBJ_NODE_TALKDIALOG_NEW);
        }
        break;
        case SMCT_MSG_BOX:
            break;
        default:
            break;
        }
    }
    break;
    default:
        break;
    }

    return true;
}

void KgameWorld::closeNewDialogPadCallback(Node* pNode)
{
    m_talkDialogPadIsOpen_1 = false;
}

// Mở cửa hàng npc
void KgameWorld::CreatNpcShopDialog()
{
    m_shopDialogPadIsOpen = true;
}

// Mở hộp thoại nói chuyện
void KgameWorld::CreatTalkDialog(KUiInformationParam* pWordDataList, int nCount)
{
    if (m_NpcDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_NPCDIALOG, true);
        m_NpcDialogPadIsOpen = false;
    }
    if (m_talkDialogPadIsOpen)
        this->removeChildByTag(OBJ_NODE_TALKDIALOG, true);

    auto callback = [this](ax::Node* sender) { this->closeTaikDialogPadCallback(sender); };

    Layer* sTalkDialogLayer = KuiTalkDialog::create(pWordDataList, nCount, this, callback);
    this->addChild(sTalkDialogLayer, ORDER_UI_DIALOD, OBJ_NODE_TALKDIALOG);
    m_talkDialogPadIsOpen = true;
}

void KgameWorld::closeTaikDialogPadCallback(Node* pNode)
{
    // messageBox("回调成功","closeTaikDialogPadCallback");
    m_talkDialogPadIsOpen = false;
}

// Mở hộp thoại chung NPC
void KgameWorld::CreatNpcDialog(KUiQuestionAndAnswer* pContent, KUiNpcSpr* pImage, int nIsTimeClose)
{  //
    if (m_NpcDialogPadIsOpen)
        this->removeChildByTag(OBJ_NODE_NPCDIALOG, true);

    if (m_talkDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_TALKDIALOG, true);
        m_talkDialogPadIsOpen = false;
    }

    auto callback = [this](ax::Node* sender) { this->closeNpcDialogPadCallback(sender); };

    Layer* sDialogLayer  = KuiDialog::create(pContent, pImage, nIsTimeClose, this, callback);
    m_NpcDialogPadIsOpen = true;
    this->addChild(sDialogLayer, ORDER_UI_DIALOD, OBJ_NODE_NPCDIALOG);
}

void KgameWorld::closeNpcDialogPadCallback(Node* pNode)
{
    m_NpcDialogPadIsOpen = false;
}
// 更新物品的变化
void KgameWorld::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{
    KuiItem* ItemPad = NULL;
    if (m_itemsPadIsOpen)
        ItemPad = (KuiItem*)this->getChildByTag(OBJ_NODE_ITEM);
    if (ItemPad)
        ItemPad->UpdateItem(pItem, bAdd);

    KuiSellItem* sItemPad = NULL;
    if (m_playerSellPadIsOpen)
        sItemPad = (KuiSellItem*)this->getChildByTag(OBJ_NODE_ITEM_SELL);
    if (sItemPad)
        sItemPad->UpdateItem(pItem, bAdd);

    Kuigive* sGivePad = NULL;
    if (m_givePadIsOpen)
        sGivePad = (Kuigive*)this->getChildByTag(OBJ_NODE_GIVE);
    if (sGivePad)
        sGivePad->UpdateItem(pItem, bAdd);

    KuiPlayerTrade* stradePad = NULL;
    if (m_tradePadIsOpen)
        stradePad = (KuiPlayerTrade*)this->getChildByTag(OBJ_NODE_TRADE);
    if (stradePad)
    {
        stradePad->UpdateItem(pItem, bAdd);
        // 更新对方物品
        // stradePad->UpdateTarGiveItem(pItem,bAdd);
    }
}

void KgameWorld::UpdateItemEx(KUiObjAtRegion* pItem, int bAdd)
{
    KuiItemEx* ItemPad = NULL;
    if (m_itemExPadIsOpen)
        ItemPad = (KuiItemEx*)this->getChildByTag(OBJ_NODE_ITEMBOX_EX);
    if (ItemPad)
        ItemPad->UpdateItem(pItem, bAdd);
}

void KgameWorld::UpdateExBox(KUiObjAtRegion* pItem, int bAdd, int exBoxIndex)
{
    KuistoreBox* ItemPad = NULL;
    if (m_storeboxPadIsOpen)
        ItemPad = (KuistoreBox*)this->getChildByTag(OBJ_NODE_STORE);
    if (ItemPad)
        ItemPad->UpdateExItem(pItem, bAdd, exBoxIndex);
}

void KgameWorld::UpdateStoreBox(KUiObjAtRegion* pItem, int bAdd)
{
    KuistoreBox* ItemPad = NULL;
    if (m_storeboxPadIsOpen)
        ItemPad = (KuistoreBox*)this->getChildByTag(OBJ_NODE_STORE);
    if (ItemPad)
        ItemPad->UpdateItem(pItem, bAdd);
}

void KgameWorld::openstringBox(const char* pszTitle,
                               int nMinLen,
                               int nMaxLen,
                               const char* pszCallback,
                               const char* pszError,
                               int bNum)
{
    if (m_getstringPadIsOpen)
        this->removeChildByTag(OBJ_NODE_STRINGBOX, true);
    char strTitle[128];
    sprintf(strTitle, "%s", pszTitle);
    auto callback = [this](ax::Node* sender) { this->getStringPadCallback(sender); };
    Layer* getStringDialogLayer =
        KuigetStringInUI::create(strTitle, nMinLen, nMaxLen, pszCallback, pszError, bNum, this, callback);
    this->addChild(getStringDialogLayer, ORDER_UI_DIALOD, OBJ_NODE_STRINGBOX);
    m_getstringPadIsOpen = true;
}

void KgameWorld::getStringPadCallback(Node* pNode)
{
    m_getstringPadIsOpen = false;
}

void KgameWorld::openNpcshop(int nshopidx)
{
    if (m_NpcDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_NPCDIALOG, true);
        m_NpcDialogPadIsOpen = false;
    }

    if (m_talkDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_TALKDIALOG, true);
        m_talkDialogPadIsOpen = false;
    }

    if (m_npcshopPadIsOpen)
        this->removeChildByTag(OBJ_NODE_NPCSHOP, true);

    auto callback              = [this](ax::Node* sender) { this->NpcshopPadCallback(sender); };
    Layer* snpcshopDialogLayer = KuiShop::create(nshopidx, this, callback);
    this->addChild(snpcshopDialogLayer, ORDER_UI_DIALOD, OBJ_NODE_NPCSHOP);
    m_npcshopPadIsOpen = true;
}

void KgameWorld::ClosePlayerShop()
{
    if (m_tanweoPadIsOpen)
        this->removeChildByTag(OBJ_NODE_PLAYERSHOP, true);
    if (g_pCoreShell)
        g_pCoreShell->OperationRequest(GOI_VIEW_PLAYERSELLITEM_END, 0, 0);
}

// 顶部飞滚信息显示
void KgameWorld::OpenFlyNewsDialog_a(KNewsMessage* pMsg, SYSTEMTIME* pTime) {}

// Mở hộp thoại Tạo băng đảng
void KgameWorld::OpenCreatTongSheet()
{
    if (m_laotouPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_LAOTOU, true);
        m_laotouPadIsOpen = false;
    }

    if (m_givePadIsOpen)
    {
        m_givePadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_GIVE, true);
    }

    if (m_itemsPadIsOpen)
    {
        m_itemsPadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_ITEM, true);
    }

    if (m_tongCreatPadIsOpen)
    {
        m_tongCreatPadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_TONG_CREAT, true);
    }

    auto callback = [this](ax::Node* sender) { this->closeTongSheetPadCallback(sender); };

    Layer* sTongLayer    = KuiTongCreateSheet::create("", this, callback);
    m_tongCreatPadIsOpen = true;
    this->addChild(sTongLayer, ORDER_UI_DIALOD, OBJ_NODE_TONG_CREAT);
}

void KgameWorld::closeTongSheetPadCallback(Node* pNode)
{
    m_tongCreatPadIsOpen = false;
}
// 顶部飞滚信息显示

void KgameWorld::OpenFlyNewsDialog_b(KNewsMessage* pMsg, SYSTEMTIME* pTime) {}

// xem thong tin vat pham kenh chat
void KgameWorld::OpenItenLinkDialog(KUiLianItem* pDest)
{
    if (!pDest)
        return;

    if (m_itemLinkPadIsOpen)
        this->removeChildByTag(OBJ_NODE_ITEMLINK, true);
    // messageBox("itemLink open",UTEXT(pDest->Name,1).c_str());
    KUiItemdObject nItemData;
    // nItemData.
    nItemData.nGenkind = -1;
    nItemData.uId      = ItemSet.SearchID(pDest->uId);  // pDest->uId;
    nItemData.DataH    = pDest->h;
    nItemData.DataW    = pDest->w;
    // nItemData.nRect.size
    auto callback       = [this](ax::Node* sender) { this->closeLinkPadCallback(sender); };
    Layer* sDescLayer   = KuiItemdesc::create(nItemData, this, callback, 3);
    m_itemLinkPadIsOpen = true;
    this->addChild(sDescLayer, ORDER_UI_DIALOD, OBJ_NODE_ITEMLINK);
}

void KgameWorld::closeLinkPadCallback(Node* pNode)
{
    m_itemLinkPadIsOpen = false;
}
// Mở bảng giao dịch
void KgameWorld::OpenPlayerTradeDialog(KUiPlayerItem* pDest)
{
    if (m_tradePadIsOpen)
        this->removeChildByTag(OBJ_NODE_TRADE, true);

    if (m_playerfunPadIsOpen && sfunLayer)
    {
        ((Kuiplayerfun*)sfunLayer)->_removeDelegate(false);
        sfunLayer->setVisible(false);
        /*else
        {
                ((Kuiplayerfun *)sfunLayer)->_removeDelegate(true);
                sfunLayer->setVisible(true);
        }*/
    }

    auto callback = [this](ax::Node* sender) { this->closetradePadCallback(sender); };

    Layer* tradeLayer = KuiPlayerTrade::create(pDest, this, callback);
    this->addChild(tradeLayer, ORDER_UI_DIALOD, OBJ_NODE_TRADE);
    m_tradePadIsOpen = true;
}

void KgameWorld::UpdateTradeOperData()
{
    if (m_tradePadIsOpen)
    {
        KuiPlayerTrade* nTempPad = (KuiPlayerTrade*)this->getChildByTag(OBJ_NODE_TRADE);
        if (nTempPad)
            nTempPad->UpdateTradeOperData();
    }
}

void KgameWorld::UpdateTradeEnd()  // 交易结束
{
    KuiPlayerTrade* nCurTrade = (KuiPlayerTrade*)this->getChildByTag(OBJ_NODE_TRADE);

    if (nCurTrade)
    {
        m_tradePadIsOpen = false;
        nCurTrade->__ClosePad();
    }
}
// Cập nhật vật phẩm của đối thủ
void KgameWorld::OnTarChangedItem(KUiObjAtRegion* pObj, int bAdd)
{
    // if (m_tradePadIsOpen)
    //{
    KuiPlayerTrade* nTempPad = (KuiPlayerTrade*)this->getChildByTag(OBJ_NODE_TRADE);
    if (nTempPad)
        nTempPad->UpdateTarGiveItem(pObj, bAdd);
    //}
}
// Cập nhật các mặt hàng của riêng bạn
void KgameWorld::OnSelfChangedItem(KUiObjAtRegion* pObj, int bAdd)
{
    if (m_tradePadIsOpen)
    {
        KuiPlayerTrade* nTempPad = (KuiPlayerTrade*)this->getChildByTag(OBJ_NODE_TRADE);
        if (nTempPad)
            nTempPad->UpdateGiveItem(pObj, bAdd);
    }
}
// Mở bảng gian hàng
void KgameWorld::OpenPlayerShopDialog(KUiPlayerItem* pDest)
{
    if (m_NpcDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_NPCDIALOG, true);
        m_NpcDialogPadIsOpen = false;
    }

    if (m_talkDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_TALKDIALOG, true);
        m_talkDialogPadIsOpen = false;
    }
    if (m_npcshopPadIsOpen)
        this->removeChildByTag(OBJ_NODE_NPCSHOP, true);
    if (m_tanweoPadIsOpen)
        this->removeChildByTag(OBJ_NODE_PLAYERSHOP, true);
    if (!pDest)
        return;

    auto callback = [this](ax::Node* sender) { this->playershopPadCallback(sender); };

    Layer* playershopDialogLayer = KuiPlayerShop::create(pDest, this, callback);
    this->addChild(playershopDialogLayer, ORDER_UI_DIALOD, OBJ_NODE_PLAYERSHOP);
    m_tanweoPadIsOpen = true;
}

void KgameWorld::playershopPadCallback(Node* pNode)
{
    m_tanweoPadIsOpen = false;
}

// Cập nhật các mặt hàng trong gian hàng
void KgameWorld::UpPlayerShopData()
{
    KuiPlayerShop* ItemPad = NULL;
    if (m_tanweoPadIsOpen)
        ItemPad = (KuiPlayerShop*)this->getChildByTag(OBJ_NODE_PLAYERSHOP);
    if (ItemPad)
        ItemPad->UpdateItem();
}

void KgameWorld::OpenViewStateDialog(KUiPlayerItem* pDest)
{
    if (!pDest)
        return;
    if (m_NpcDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_NPCDIALOG, true);
        m_NpcDialogPadIsOpen = false;
    }

    if (m_talkDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_TALKDIALOG, true);
        m_talkDialogPadIsOpen = false;
    }

    if (m_npcshopPadIsOpen)
        this->removeChildByTag(OBJ_NODE_NPCSHOP, true);

    if (m_viewPadIsOpen)
        this->removeChildByTag(OBJ_NODE_VIEW, true);

    auto callback          = [this](ax::Node* sender) { this->closeViewPadCallback(sender); };
    Layer* viewDialogLayer = KuiViewRoleState::create(pDest, this, callback);
    this->addChild(viewDialogLayer, ORDER_UI_DIALOD, OBJ_NODE_VIEW);
    m_viewPadIsOpen = true;
}

void KgameWorld::closeViewPadCallback(Node* pNode)
{
    m_viewPadIsOpen = false;
}
// Mở bảng trao quyền
void KgameWorld::qiangItemCallback(Ref* pSender)
{
    if (!g_pCoreShell)
        return;

    if (g_pCoreShell->GetAutoplayid())
    {
#ifdef WIN32
        messageBox("挂机中,禁止强化!", "Chú ý");
#else
        messageBox(UTEXT("挂机中,禁止强化!", 1).c_str(), UTEXT("提示", 1).c_str());
#endif
        return;
    }

    if (g_pCoreShell->GetSkillLiveTime(1705) > 0)
    {
#ifdef WIN32
        messageBox("Không thể cường hoá khi đang bật tự động chiến đấu", "Chú ý");
#else
        messageBox(UTEXT("自动经验中,禁止强化!", 1).c_str(), UTEXT("提示", 1).c_str());
#endif
        return;
    }

    if (g_pCoreShell->GetSkillLiveTime(1706) > 0)
    {
#ifdef WIN32
        messageBox("烤火中,禁止强化!", "Chú ý");
#else
        messageBox(UTEXT("烤火中,禁止强化!", 1).c_str(), UTEXT("提示", 1).c_str());
#endif
        return;
    }
    if (m_givePadIsOpen)
        this->removeChildByTag(OBJ_NODE_GIVE, true);

    if (m_itemsPadIsOpen)
    {
        m_itemsPadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_ITEM, true);
    }
    // 开始发送打开给与界面协议
    g_pCoreShell->__opensysui(2, 1);
    /*Layer * giveLayer =
    Kuigive::create("装备强化面板","注意:装备强化只能放一件装备和一个强化石头!友情提示:强化可升可降,需谨慎!",1,"sysqianghua","sysError",this,CC_CALLBACK_1(KgameWorld::closegivePadCallback));
    m_givePadIsOpen = true;
    this->addChild(giveLayer,ORDER_UI_DIALOD,OBJ_NODE_GIVE);*/
}

void KgameWorld::openGive(char* pszTitle, char* pszInitString, int nNum, char* pszCallback, char* pszError)
{
    if (m_givePadIsOpen)
        this->removeChildByTag(OBJ_NODE_GIVE, true);
    if (m_itemsPadIsOpen)
    {
        m_itemsPadIsOpen = false;
        this->removeChildByTag(OBJ_NODE_ITEM, true);
    }

    auto callback = [this](ax::Node* sender) { this->closegivePadCallback(sender); };

    Layer* giveLayer = Kuigive::create(pszTitle, pszInitString, nNum, pszCallback, pszError, this, callback);
    m_givePadIsOpen  = true;
    this->addChild(giveLayer, ORDER_UI_DIALOD, OBJ_NODE_GIVE);
}

void KgameWorld::openlaotou()
{
    if (m_laotouPadIsOpen)
        this->removeChildByTag(OBJ_NODE_LAOTOU, true);

    auto callback = [this](ax::Node* sender) { this->closeLaoPadCallback(sender); };

    Layer* slaotouLayer = Kuilaotou::create(this, callback);
    m_laotouPadIsOpen   = true;
    this->addChild(slaotouLayer, ORDER_UI_DIALOD, OBJ_NODE_LAOTOU);
}
// 打开进度条
void KgameWorld::openProgressbar(KUiJinDuTiaoInfo* pJinDu)
{
    if (m_ProgressPadIsOpen)
        this->removeChildByTag(OBJ_NODE_PROGRESS, true);

    if (!pJinDu)
    {
        if (m_ProgressPadIsOpen)
            this->removeChildByTag(OBJ_NODE_PROGRESS, true);

        m_ProgressPadIsOpen = false;
        return;
    }

    auto callback = [this](ax::Node* sender) { this->progressPadCallback(sender); };

    Layer* progressDialogLayer = KuiProgressbar::create(pJinDu, this, callback);
    this->addChild(progressDialogLayer, ORDER_UI_DIALOD, OBJ_NODE_PROGRESS);
    m_ProgressPadIsOpen = true;
}

void KgameWorld::progressPadCallback(Node* pNode)
{
    m_ProgressPadIsOpen = false;
}
void KgameWorld::NpcshopPadCallback(Node* pNode)
{
    m_npcshopPadIsOpen = false;
    // 恢复聊天的触摸
    setchatDelegate(true);
}

void KgameWorld::openStorebox()
{
    if (m_NpcDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_NPCDIALOG, true);
        m_NpcDialogPadIsOpen = false;
    }
    if (m_talkDialogPadIsOpen)
    {
        this->removeChildByTag(OBJ_NODE_TALKDIALOG, true);
        m_talkDialogPadIsOpen = false;
    }
    if (m_storeboxPadIsOpen)
        this->removeChildByTag(OBJ_NODE_STORE, true);

    auto callback = [this](ax::Node* sender) { this->closestorePadCallback(sender); };

    Layer* sstoreDialogLayer = KuistoreBox::create("", this, callback);
    this->addChild(sstoreDialogLayer, ORDER_UI_DIALOD, OBJ_NODE_STORE);
    m_storeboxPadIsOpen = true;
}

void KgameWorld::closestorePadCallback(Node* pNode)
{
    m_storeboxPadIsOpen = false;
}

void KgameWorld::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
    if (m_skilPadIsOpen)
    {  // 如果技能面板是打开的
        Kuiskill* Upskill = (Kuiskill*)this->getChildByTag(OBJ_NODE_SKILL);
        if (Upskill)
            Upskill->UpdateSkill(pSkill, nIndex);
    }
}

void KgameWorld::FreeAllSprite()
{
    if (g_GameWorld)
    {  // 换地图的时候全部删除精灵
        Node* nRegNode = g_GameWorld->ParentNode_map;
        if (nRegNode)
            nRegNode->removeAllChildren();
        // nRegNode->setVisible(false);

        nRegNode = g_GameWorld->ParentNode_Bigmap;

        if (nRegNode)
            nRegNode->removeAllChildren();

        nRegNode = g_GameWorld->ParentNode_npc;
        if (nRegNode)
            nRegNode->removeAllChildren();

        nRegNode = g_GameWorld->ParentNode_obj;
        if (nRegNode)
            nRegNode->removeAllChildren();

        nRegNode = g_GameWorld->ParentNode_miss;
        if (nRegNode)
            nRegNode->removeAllChildren();

        nRegNode = g_GameWorld->ParentNode_npcother;
        if (nRegNode)
            nRegNode->removeAllChildren();

        nRegNode = g_GameWorld->ParentNode_npcblood;
        if (nRegNode)
            nRegNode->removeAllChildren();
    }
}

void KgameWorld::removeMenuByIdx(int nNpcIndex, int nStateIdx)
{
    if (!ParentNode_npc || nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;
    char nTempKey[64];
    sprintf(nTempKey, "npc_%d", nNpcIndex);
    std::string _npcKey = nTempKey;
    Node* PartNode      = (Node*)ParentNode_npc->getChildByName(_npcKey);
    if (PartNode)
    {
        char nKey[32];
        sprintf(nKey, "state_%d_menu", nStateIdx);
        std::string nstateKey = nKey;
        PartNode->removeChildByName(nstateKey, true);
    }
}

void KgameWorld::removespriteByIdx(int nNpcIndex, int nStateIdx, bool isAll)
{
    if (!ParentNode_npc || nNpcIndex <= 0 || nNpcIndex >= MAX_NPC)
        return;
    char nTempKey[64];
    sprintf(nTempKey, "npc_%d", nNpcIndex);
    std::string _npcKey = nTempKey;
    Node* PartNode      = (Node*)ParentNode_npc->getChildByName(_npcKey);
    if (PartNode)
    {
        // Node *nStateSpr = (Node *)PartNode->getChildByTag(UI_EQ_PART_NPC_STATE);
        // if (!isAll && nStateSpr)
        char nKey[32];
        sprintf(nKey, "state_%d", nStateIdx);
        std::string nstateKey = nKey;
        PartNode->removeChildByName(nstateKey, true);
    }
}
// 更新组队面板
void KgameWorld::UpdateDataTeam(KUiPlayerTeam* pInfo)
{
    if (m_teamPadIsOpen)
    {
        KuiTeam* nTempPtr = (KuiTeam*)this->getChildByTag(OBJ_NODE_TEAM);
        if (nTempPtr)
            nTempPtr->UpdateData(pInfo);
    }
}

void KgameWorld::UpdateGiveItem(KUiObjAtRegion* pItem, int bAdd)
{
    Kuigive* sGivePad = NULL;
    if (m_givePadIsOpen)
        sGivePad = (Kuigive*)this->getChildByTag(OBJ_NODE_GIVE);
    if (sGivePad)
        sGivePad->UpdateGiveItem(pItem, bAdd);
}

// 更新快捷栏的物品
void KgameWorld::UpdateImmediaItem(int nIndex, unsigned int uGenre, unsigned int uId)
{
    // messageBox("快捷兰","快捷兰");
    if (nIndex >= 0 && nIndex < UPB_IMMEDIA_ITEM_COUNT)
    {  // 绘画图标

        m_ImmediaItem[nIndex].m_uid = uId;

        if (uGenre == CGOG_NOTHING)  // 删除
            HoldObject_(CGOG_NOTHING, nIndex, 0, false);
        else  // 增加
            HoldObject_(CGOG_IME_ITEM, nIndex, uId, true);
        // m_ImmediaItem[nIndex].HoldObject(CGOG_IME_ITEM, uId, 0, 0);  //快捷栏物品
    }
}
/*
void  KgameWorld::HoldObject(int nType,int nBoxIndex,int nItemidx,bool isAdd)
{
        if (!ParentNode_item) return;

        Sprite *tempSpr_1 = (Sprite *)ParentNode_item->getChildByTag(1);
        Sprite *tempSpr_2 = (Sprite *)ParentNode_item->getChildByTag(2);
        Sprite *tempSpr_3 = (Sprite *)ParentNode_item->getChildByTag(3);
        if  (!tempSpr_1 || !tempSpr_2 || !tempSpr_3) return;
        char nKey[32];
        sprintf(nKey,"immed_%d",nBoxIndex+1);
        std::string ntempKey = nKey;
        sprintf(nKey,"color_%d",nBoxIndex+1);
        std::string ncolorKey = nKey;
        if  (!isAdd)
        {//删除
                if (nBoxIndex>=0 && nBoxIndex<=2) //012
                {
                        tempSpr_1->removeChildByName(ntempKey,true);
                        tempSpr_1->removeChildByName(ncolorKey,true);
                }
                else if (nBoxIndex>2 && nBoxIndex<=5)//345
                {
                    tempSpr_2->removeChildByName(ntempKey,true);
                        tempSpr_2->removeChildByName(ncolorKey,true);
                }
                else//678
                {
                        tempSpr_3->removeChildByName(ntempKey,true);
                        tempSpr_3->removeChildByName(ncolorKey,true);
                }

                return;
        }
        if  (nItemidx<=0 || nItemidx>=MAX_ITEM) return;

        char nSprName[128]={0};
        ZeroMemory(nSprName,sizeof(nSprName));
        if  (Item[nItemidx].GetImagePath())
                sprintf(nSprName,Item[nItemidx].GetImagePath());
        else
                sprintf(nSprName,"\\spr\\others\\问号.spr");
        //sprintf(nSprName,Item[nItemidx].GetImagePath());
        g_StrLower(nSprName);
        int m_nWidth,m_nHeight,nFrams;
        Texture2D *bgCur = NULL;
        SPRFRAMSINFO nSprInfo;
        ZeroMemory(&nSprInfo,sizeof(nSprInfo));
        bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
        if (bgCur==NULL)
                return;
        Sprite *nIiemTempSpr = NULL;
        LayerColor *bgcolorLayer =NULL;
        Color4B color(112, 128, 144, 125); //112, 128, 144
        Size colorsize;
        colorsize.width  = 35;
        colorsize.height = 35;
        Size bgsize    = tempSpr_1->getContentSize();
        Size nTextsize = bgCur->getContentSize();
        float offsetX    = 5;
        float offsetY    = 5;
        if (nBoxIndex>=0 && nBoxIndex<=2)
        {//012
                nIiemTempSpr = (Sprite *)tempSpr_1->getChildByName(ntempKey);
                bgcolorLayer = (LayerColor *)tempSpr_1->getChildByName(ncolorKey);
                if (!nIiemTempSpr)
                {
                        nIiemTempSpr = Sprite::createWithTexture(bgCur);
                        nIiemTempSpr->setAnchorPoint(ax::Vec2(0,0));
                        nIiemTempSpr->setPosition(ax::Vec2(0,0));
                        nIiemTempSpr->setTag(ntempKey);
                        tempSpr_1->addChild(nIiemTempSpr,1);
                        if (!bgcolorLayer)
                        {
                                bgcolorLayer = LayerColor::create(color);//颜色层
                                bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
                                bgcolorLayer->setContentSize(colorsize);//设置和窗口的大小
                                bgcolorLayer->setTag(ncolorKey);
                                tempSpr_1->addChild(bgcolorLayer,0);
                        }
                        if (nBoxIndex==0)
                        {
                                bgcolorLayer->setPosition(ax::Vec2(offsetX,bgsize.height-colorsize.height-offsetY));
                                Point nclorPos = bgcolorLayer->getPosition();
                                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x+colorsize.width/2-nTextsize.width/2,nclorPos.y+colorsize.height/2-nTextsize.height/2));

                        }
                        if (nBoxIndex==1)
                        {
                                bgcolorLayer->setPosition(ax::Vec2(offsetX,bgsize.height-(colorsize.height+offsetY)*2));
                                Point nclorPos = bgcolorLayer->getPosition();
                                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x+colorsize.width/2-nTextsize.width/2,nclorPos.y+colorsize.height/2-nTextsize.height/2));
                        }
                        if (nBoxIndex==2)
                        {
                                //nIiemTempSpr->setPosition(ax::Vec2(bgsize.width/2,bgsize.height-colorsize.height*2));
                                bgcolorLayer->setPosition(ax::Vec2(offsetX,bgsize.height-(colorsize.height+offsetY)*3));
                                Point nclorPos = bgcolorLayer->getPosition();
                                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x+colorsize.width/2-nTextsize.width/2,nclorPos.y+colorsize.height/2-nTextsize.height/2));
                        }
                }
        }
        else if (nBoxIndex>2 && nBoxIndex<=5)
        {//345
                nIiemTempSpr = (Sprite *)tempSpr_2->getChildByName(ntempKey);
                if (!nIiemTempSpr)
                {
                        nIiemTempSpr = Sprite::createWithTexture(bgCur);
                        nIiemTempSpr->setAnchorPoint(ax::Vec2(0,0));
                        nIiemTempSpr->setPosition(ax::Vec2(0,0));
                        nIiemTempSpr->setTag(ntempKey);
                        tempSpr_2->addChild(nIiemTempSpr,1);
                        if (!bgcolorLayer)
                        {
                                bgcolorLayer = LayerColor::create(color);//颜色层
                                bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
                                bgcolorLayer->setContentSize(colorsize);//设置和窗口的大小
                                bgcolorLayer->setTag(ncolorKey);
                                tempSpr_2->addChild(bgcolorLayer,0);
                        }
                        if (nBoxIndex==3)
                        {
                                bgcolorLayer->setPosition(ax::Vec2(offsetX,bgsize.height-colorsize.height-offsetY));
                                Point nclorPos = bgcolorLayer->getPosition();
                                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x+colorsize.width/2-nTextsize.width/2,nclorPos.y+colorsize.height/2-nTextsize.height/2));

                        }
                        if (nBoxIndex==4)
                        {
                                bgcolorLayer->setPosition(ax::Vec2(offsetX,bgsize.height-(colorsize.height+offsetY)*2));
                                Point nclorPos = bgcolorLayer->getPosition();
                                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x+colorsize.width/2-nTextsize.width/2,nclorPos.y+colorsize.height/2-nTextsize.height/2));
                        }
                        if (nBoxIndex==5)
                        {
                                //nIiemTempSpr->setPosition(ax::Vec2(bgsize.width/2,bgsize.height-colorsize.height*2));
                                bgcolorLayer->setPosition(ax::Vec2(offsetX,bgsize.height-(colorsize.height+offsetY)*3));
                                Point nclorPos = bgcolorLayer->getPosition();
                                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x+colorsize.width/2-nTextsize.width/2,nclorPos.y+colorsize.height/2-nTextsize.height/2));
                        }
                }

        }
        else
        {//678
                nIiemTempSpr = (Sprite *)tempSpr_3->getChildByName(ntempKey);
                if (!nIiemTempSpr)
                {
                        nIiemTempSpr = Sprite::createWithTexture(bgCur);
                        nIiemTempSpr->setAnchorPoint(ax::Vec2(0,0));
                        nIiemTempSpr->setPosition(ax::Vec2(0,0));
                        nIiemTempSpr->setTag(ntempKey);
                        tempSpr_3->addChild(nIiemTempSpr,1);

                        if (!bgcolorLayer)
                        {
                                bgcolorLayer = LayerColor::create(color);//颜色层
                                bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
                                bgcolorLayer->setContentSize(colorsize);//设置和窗口的大小
                                bgcolorLayer->setTag(ncolorKey);
                                tempSpr_3->addChild(bgcolorLayer,0);
                        }
                        if (nBoxIndex==6)
                        {
                                bgcolorLayer->setPosition(ax::Vec2(offsetX,bgsize.height-colorsize.height-offsetY));
                                Point nclorPos = bgcolorLayer->getPosition();
                                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x+colorsize.width/2-nTextsize.width/2,nclorPos.y+colorsize.height/2-nTextsize.height/2));

                        }
                        if (nBoxIndex==7)
                        {
                                bgcolorLayer->setPosition(ax::Vec2(offsetX,bgsize.height-(colorsize.height+offsetY)*2));
                                Point nclorPos = bgcolorLayer->getPosition();
                                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x+colorsize.width/2-nTextsize.width/2,nclorPos.y+colorsize.height/2-nTextsize.height/2));
                        }
                        if (nBoxIndex==8)
                        {
                                //nIiemTempSpr->setPosition(ax::Vec2(bgsize.width/2,bgsize.height-colorsize.height*2));
                                bgcolorLayer->setPosition(ax::Vec2(offsetX,bgsize.height-(colorsize.height+offsetY)*3));
                                Point nclorPos = bgcolorLayer->getPosition();
                                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x+colorsize.width/2-nTextsize.width/2,nclorPos.y+colorsize.height/2-nTextsize.height/2));
                        }
                }
        }

        Rect nRectZero = ax::Rect(0,0,0,0);
        nRectZero.size = bgCur->getContentSize();
        if (nIiemTempSpr)
        {
                nIiemTempSpr->setTextureRect(nRectZero,false,nRectZero.size);
                nIiemTempSpr->setTexture(bgCur);
        }
}*/
// auxiliaryskill[i].auxiliaryskillSpr=MenuItemSprite::create(btn_normal_sprite,btn_select_sprite,NULL,CC_CALLBACK_1(KgameWorld::auxiliaryskillCallback,
// this));
void KgameWorld::HoldObject_(int nType, int nBoxIndex, int nItemidx, bool isAdd)
{
    if (!ParentNode_item)
        return;

    Sprite* tempSpr_1 = (Sprite*)ParentNode_item->getChildByTag(1);
    Sprite* tempSpr_2 = (Sprite*)ParentNode_item->getChildByTag(2);
    Sprite* tempSpr_3 = (Sprite*)ParentNode_item->getChildByTag(3);

    if (!tempSpr_1 || !tempSpr_2 || !tempSpr_3)
        return;

    char nKey[32];
    sprintf(nKey, "immed_%d", nBoxIndex + 1);
    std::string ntempKey = nKey;
    sprintf(nKey, "color_%d", nBoxIndex + 1);
    std::string ncolorKey = nKey;
    sprintf(nKey, "stack_%d", nBoxIndex + 1);
    std::string nstackKey = nKey;
    if (!isAdd)
    {                                          // 删除
        if (nBoxIndex >= 0 && nBoxIndex <= 2)  // 012
        {
            ImmediaMenu_1->removeChildByTag(nBoxIndex + 1, true);
            ImmediaMenu_1->removeChildByName(ncolorKey, true);
        }
        else if (nBoxIndex > 2 && nBoxIndex <= 5)  // 345
        {
            ImmediaMenu_2->removeChildByTag(nBoxIndex + 1, true);
            ImmediaMenu_2->removeChildByName(ncolorKey, true);
        }
        else  // 678
        {
            ImmediaMenu_3->removeChildByTag(nBoxIndex + 1, true);
            ImmediaMenu_3->removeChildByName(ncolorKey, true);
        }
        m_ImmediaItemSelIndex = -1;
        return;
    }
    if (nItemidx <= 0 || nItemidx >= MAX_ITEM)
        return;
    char nSprName[128] = {0};
    ZeroMemory(nSprName, sizeof(nSprName));
    if (Item[nItemidx].GetImagePath())
        sprintf(nSprName, "%s", Item[nItemidx].GetImagePath());
    else
        sprintf(nSprName, "\\spr\\Ui4\\KgameWorld\\btn_BatPk.spr");

    g_StrLower(nSprName);
    int m_nWidth, m_nHeight, nFrams;
    Texture2D* bgCur = NULL;
    SPRFRAMSINFO nSprInfo;
    ZeroMemory(&nSprInfo, sizeof(nSprInfo));
    bgCur = _getinidata.getinidata_one(nSprName, 0, &m_nWidth, &m_nHeight, &nFrams, &nSprInfo);
    if (bgCur == NULL)
        return;
    MenuItemSprite* nIiemTempSpr = NULL;
    LayerColor* bgcolorLayer     = NULL;
    Color4B color(112, 128, 144, 125);  // 112, 128, 144
    Size colorsize;
    colorsize.width    = 35;
    colorsize.height   = 35;
    Size bgsize        = tempSpr_1->getContentSize();
    Size nTextsize     = bgCur->getContentSize();
    float offsetX      = 5;
    float offsetY      = 4;
    float offScaleX    = 1.2;  // colorsize.width/nTextsize.width;
    float offScaleY    = 1.2;  // colorsize.height/nTextsize.height;
    int nAllstackCount = 0;
    nTextsize.width *= offScaleX;
    nTextsize.height *= offScaleY;
    char strStack[32];
    if (nBoxIndex >= 0 && nBoxIndex <= 2)
    {  // 012
        nIiemTempSpr = (MenuItemSprite*)ImmediaMenu_1->getChildByTag(nBoxIndex + 1);
        bgcolorLayer = (LayerColor*)ImmediaMenu_1->getChildByName(ncolorKey);
        if (g_pCoreShell)
            nAllstackCount = g_pCoreShell->GetGameData(GDI_IMMEDIATEITEM_NUM, nBoxIndex, nItemidx);
        sprintf(strStack, "%d", nAllstackCount + Item[nItemidx].GetStackNum());
        if (!nIiemTempSpr)
        {
            // nIiemTempSpr = Sprite::createWithTexture(bgCur);
            Sprite* btn_normal_sprite = Sprite::createWithTexture(bgCur);
            Sprite* btn_select_sprite = Sprite::createWithTexture(bgCur);
            nIiemTempSpr              = MenuItemSprite::create(btn_normal_sprite, btn_select_sprite, NULL,
                                                               CC_CALLBACK_1(KgameWorld::ImmediaCallback, this));
            nIiemTempSpr->setAnchorPoint(ax::Vec2(0, 0));
            nIiemTempSpr->setPosition(ax::Vec2(0, 0));
            nIiemTempSpr->setPosition(ax::Vec2(0, 0));
            nIiemTempSpr->setScale(offScaleX, offScaleY);
            ImmediaMenu_1->addChild(nIiemTempSpr, nBoxIndex + 1);

            if (!bgcolorLayer)
            {
                bgcolorLayer = LayerColor::create(color);  // 颜色层
                bgcolorLayer->setAnchorPoint(ax::Vec2(0, 0));
                bgcolorLayer->setContentSize(colorsize);  // 设置和窗口的大小
                //				int ncolorKey_tmp = std::hash<std::string>{}(ncolorKey);
                bgcolorLayer->setName(ncolorKey);
                ImmediaMenu_1->addChild(bgcolorLayer, -1);
                Size stackSize;
                stackSize.width  = colorsize.width;
                stackSize.height = 12;

                Label* stackCountlabel = Label::createWithTTF(strStack, UI_GAME_FONT_DEFAULT, 12, stackSize,
                                                              TextHAlignment::RIGHT);  // Arial
                stackCountlabel->setAnchorPoint(ax::Vec2(0, 0));
                stackCountlabel->setPosition(ax::Vec2(0, 0));
                //				int nstackKey_tmp = std::hash<std::string>{}(nstackKey);
                stackCountlabel->setName(nstackKey);
                stackCountlabel->setColor(ax::Color3B::YELLOW);
                bgcolorLayer->addChild(stackCountlabel, 1);
            }

            Label* tempstackCount = (Label*)bgcolorLayer->getChildByName(nstackKey);
            if (tempstackCount)
                tempstackCount->setString(strStack);

            if (nBoxIndex == 0)
            {
                bgcolorLayer->setPosition(ax::Vec2(offsetX, bgsize.height - colorsize.height - offsetY));
                Point nclorPos = bgcolorLayer->getPosition();
                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x + colorsize.width / 2 - nTextsize.width / 2,
                                                   nclorPos.y + colorsize.height / 2 - nTextsize.height / 2));
            }
            if (nBoxIndex == 1)
            {
                bgcolorLayer->setPosition(ax::Vec2(offsetX, bgsize.height - (colorsize.height + offsetY) * 2));
                Point nclorPos = bgcolorLayer->getPosition();
                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x + colorsize.width / 2 - nTextsize.width / 2,
                                                   nclorPos.y + colorsize.height / 2 - nTextsize.height / 2));
            }
            if (nBoxIndex == 2)
            {
                // nIiemTempSpr->setPosition(ax::Vec2(bgsize.width/2,bgsize.height-colorsize.height*2));
                bgcolorLayer->setPosition(ax::Vec2(offsetX, bgsize.height - (colorsize.height + offsetY) * 3));
                Point nclorPos = bgcolorLayer->getPosition();
                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x + colorsize.width / 2 - nTextsize.width / 2,
                                                   nclorPos.y + colorsize.height / 2 - nTextsize.height / 2));
            }
        }
        else
        {
            Label* tempstackCount = (Label*)bgcolorLayer->getChildByName(nstackKey);
            if (tempstackCount)
                tempstackCount->setString(strStack);
        }
    }
    else if (nBoxIndex > 2 && nBoxIndex <= 5)
    {  // 345
        nIiemTempSpr = (MenuItemSprite*)ImmediaMenu_2->getChildByTag(nBoxIndex + 1);
        bgcolorLayer = (LayerColor*)ImmediaMenu_2->getChildByName(ncolorKey);

        if (g_pCoreShell)
            nAllstackCount = g_pCoreShell->GetGameData(GDI_IMMEDIATEITEM_NUM, nBoxIndex, nItemidx);
        sprintf(strStack, "%d", nAllstackCount + Item[nItemidx].GetStackNum());

        if (!nIiemTempSpr)
        {
            // ui::CheckBox::create();

            Sprite* btn_normal_sprite = Sprite::createWithTexture(bgCur);
            Sprite* btn_select_sprite = Sprite::createWithTexture(bgCur);
            nIiemTempSpr              = MenuItemSprite::create(btn_normal_sprite, btn_select_sprite, NULL,
                                                               CC_CALLBACK_1(KgameWorld::ImmediaCallback, this));
            nIiemTempSpr->setAnchorPoint(ax::Vec2(0, 0));
            nIiemTempSpr->setPosition(ax::Vec2(0, 0));
            nIiemTempSpr->setScale(offScaleX, offScaleY);
            ImmediaMenu_2->addChild(nIiemTempSpr, nBoxIndex + 1);

            if (!bgcolorLayer)
            {
                bgcolorLayer = LayerColor::create(color);  // 颜色层
                bgcolorLayer->setAnchorPoint(ax::Vec2(0, 0));
                bgcolorLayer->setContentSize(colorsize);  // 设置和窗口的大小
                //				int ncolorKey_tmp = std::hash<std::string>{}(ncolorKey);
                bgcolorLayer->setName(ncolorKey);
                ImmediaMenu_2->addChild(bgcolorLayer, -1);
                Size stackSize;
                stackSize.width        = colorsize.width;
                stackSize.height       = 12;
                Label* stackCountlabel = Label::createWithTTF(strStack, UI_GAME_FONT_DEFAULT, 12, stackSize,
                                                              TextHAlignment::RIGHT);  // Arial
                stackCountlabel->setAnchorPoint(ax::Vec2(0, 0));
                stackCountlabel->setPosition(ax::Vec2(0, 0));
                //				int nstackKey_tmp = std::hash<std::string>{}(nstackKey);
                stackCountlabel->setName(nstackKey);
                stackCountlabel->setColor(ax::Color3B::YELLOW);
                bgcolorLayer->addChild(stackCountlabel, 1);
            }

            Label* tempstackCount = (Label*)bgcolorLayer->getChildByName(nstackKey);
            if (tempstackCount)
                tempstackCount->setString(strStack);

            if (nBoxIndex == 3)
            {
                bgcolorLayer->setPosition(ax::Vec2(offsetX, bgsize.height - colorsize.height - offsetY));
                Point nclorPos = bgcolorLayer->getPosition();
                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x + colorsize.width / 2 - nTextsize.width / 2,
                                                   nclorPos.y + colorsize.height / 2 - nTextsize.height / 2));
            }
            if (nBoxIndex == 4)
            {
                bgcolorLayer->setPosition(ax::Vec2(offsetX, bgsize.height - (colorsize.height + offsetY) * 2));
                Point nclorPos = bgcolorLayer->getPosition();
                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x + colorsize.width / 2 - nTextsize.width / 2,
                                                   nclorPos.y + colorsize.height / 2 - nTextsize.height / 2));
            }
            if (nBoxIndex == 5)
            {
                // nIiemTempSpr->setPosition(ax::Vec2(bgsize.width/2,bgsize.height-colorsize.height*2));
                bgcolorLayer->setPosition(ax::Vec2(offsetX, bgsize.height - (colorsize.height + offsetY) * 3));
                Point nclorPos = bgcolorLayer->getPosition();
                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x + colorsize.width / 2 - nTextsize.width / 2,
                                                   nclorPos.y + colorsize.height / 2 - nTextsize.height / 2));
            }
        }
        else
        {
            Label* tempstackCount = (Label*)bgcolorLayer->getChildByName(nstackKey);
            if (tempstackCount)
                tempstackCount->setString(strStack);
        }
    }
    else
    {  // 678
        nIiemTempSpr = (MenuItemSprite*)ImmediaMenu_3->getChildByTag(nBoxIndex + 1);
        bgcolorLayer = (LayerColor*)ImmediaMenu_3->getChildByName(ncolorKey);
        if (g_pCoreShell)
            nAllstackCount = g_pCoreShell->GetGameData(GDI_IMMEDIATEITEM_NUM, nBoxIndex, nItemidx);
        sprintf(strStack, "%d", nAllstackCount + Item[nItemidx].GetStackNum());

        if (!nIiemTempSpr)
        {
            Sprite* btn_normal_sprite = Sprite::createWithTexture(bgCur);
            Sprite* btn_select_sprite = Sprite::createWithTexture(bgCur);
            nIiemTempSpr              = MenuItemSprite::create(btn_normal_sprite, btn_select_sprite, NULL,
                                                               CC_CALLBACK_1(KgameWorld::ImmediaCallback, this));
            nIiemTempSpr->setAnchorPoint(ax::Vec2(0, 0));
            nIiemTempSpr->setPosition(ax::Vec2(0, 0));
            nIiemTempSpr->setScale(offScaleX, offScaleY);
            ImmediaMenu_3->addChild(nIiemTempSpr, nBoxIndex + 1);
            if (!bgcolorLayer)
            {
                bgcolorLayer = LayerColor::create(color);  // 颜色层
                bgcolorLayer->setAnchorPoint(ax::Vec2(0, 0));
                bgcolorLayer->setContentSize(colorsize);  // 设置和窗口的大小
                //				int ncolorKey_tmp = std::hash<std::string>{}(ncolorKey);
                bgcolorLayer->setName(ncolorKey);
                ImmediaMenu_3->addChild(bgcolorLayer, -1);
                Size stackSize;
                stackSize.width  = colorsize.width;
                stackSize.height = 12;

                Label* stackCountlabel = Label::createWithTTF(strStack, UI_GAME_FONT_DEFAULT, 12, stackSize,
                                                              TextHAlignment::RIGHT);  // Arial
                stackCountlabel->setAnchorPoint(ax::Vec2(0, 0));
                stackCountlabel->setPosition(ax::Vec2(0, 0));
                //				int nstackKey_tmp = std::hash<std::string>{}(nstackKey);
                stackCountlabel->setName(nstackKey);
                stackCountlabel->setColor(ax::Color3B::YELLOW);
                bgcolorLayer->addChild(stackCountlabel, 1);
            }

            Label* tempstackCount = (Label*)bgcolorLayer->getChildByName(nstackKey);
            if (tempstackCount)
                tempstackCount->setString(strStack);

            if (nBoxIndex == 6)
            {
                bgcolorLayer->setPosition(ax::Vec2(offsetX, bgsize.height - colorsize.height - offsetY));
                Point nclorPos = bgcolorLayer->getPosition();
                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x + colorsize.width / 2 - nTextsize.width / 2,
                                                   nclorPos.y + colorsize.height / 2 - nTextsize.height / 2));
            }
            if (nBoxIndex == 7)
            {
                bgcolorLayer->setPosition(ax::Vec2(offsetX, bgsize.height - (colorsize.height + offsetY) * 2));
                Point nclorPos = bgcolorLayer->getPosition();
                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x + colorsize.width / 2 - nTextsize.width / 2,
                                                   nclorPos.y + colorsize.height / 2 - nTextsize.height / 2));
            }
            if (nBoxIndex == 8)
            {
                // nIiemTempSpr->setPosition(ax::Vec2(bgsize.width/2,bgsize.height-colorsize.height*2));
                bgcolorLayer->setPosition(ax::Vec2(offsetX, bgsize.height - (colorsize.height + offsetY) * 3));
                Point nclorPos = bgcolorLayer->getPosition();
                nIiemTempSpr->setPosition(ax::Vec2(nclorPos.x + colorsize.width / 2 - nTextsize.width / 2,
                                                   nclorPos.y + colorsize.height / 2 - nTextsize.height / 2));
            }
        }
        else
        {
            Label* tempstackCount = (Label*)bgcolorLayer->getChildByName(nstackKey);
            if (tempstackCount)
                tempstackCount->setString(strStack);
        }
    }

    // nIiemTempSpr->setContentSize(colorsize);
    /*Rect nRectZero = ax::Rect(0,0,0,0);
    nRectZero.size = bgCur->getContentSize();
    if (nIiemTempSpr)
    {
            nIiemTempSpr->setTextureRect(nRectZero,false,nRectZero.size);
            nIiemTempSpr->setTexture(bgCur);
    }*/
}
void KgameWorld::ImmediaCallback(Ref* pSender)
{
    if (pSender)
    {
        MenuItemSprite* nTempSpr = (MenuItemSprite*)pSender;
        int idx                  = nTempSpr->getTag();
        m_ImmediaItemSelIndex    = m_ImmediaItem[idx - 1].m_uid;  // 物品的索引
        char nKey[32];
        sprintf(nKey, "color_%d", idx);
        std::string colorKey = nKey;
        idx--;
        LayerColor* nTempColor = NULL;
        ax::Color3B noldColor  = {112, 128, 144};
        for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
        {  // 复原所有颜色
            char ntKey[32];
            sprintf(ntKey, "color_%d", i + 1);
            std::string TcolorKey = ntKey;
            if (i >= 0 && i <= 2)
                nTempColor = (LayerColor*)ImmediaMenu_1->getChildByName(TcolorKey);
            else if (i > 2 && i <= 5)
                nTempColor = (LayerColor*)ImmediaMenu_2->getChildByName(TcolorKey);
            else if (i > 5 && i <= 8)
                nTempColor = (LayerColor*)ImmediaMenu_3->getChildByName(TcolorKey);

            if (nTempColor)
            {
                nTempColor->setColor(noldColor);
                nTempColor->setOpacity(125);
            }
        }

        nTempColor = NULL;
        // ax::Color3B nColor = {};
        if (idx >= 0 && idx <= 2)
            nTempColor = (LayerColor*)ImmediaMenu_1->getChildByName(colorKey);
        else if (idx > 2 && idx <= 5)
            nTempColor = (LayerColor*)ImmediaMenu_2->getChildByName(colorKey);
        else if (idx > 5 && idx <= 8)
            nTempColor = (LayerColor*)ImmediaMenu_3->getChildByName(colorKey);

        if (nTempColor)
        {
            nTempColor->setColor(ax::Color3B::RED);
            nTempColor->setOpacity(125);
        }
        if (isClose)
        {
            /*#ifdef WIN32
                                    messageBox("快捷栏被锁住中,请点击隐藏按钮","提示");
            #else
                                    messageBox(UTEXT("快捷栏被锁住中,请点击隐藏按钮",1).c_str(),UTEXT("提示",1).c_str());
            #endif*/
            return;
        }
        if (m_ImmediaItemSelIndex <= 0 || m_ImmediaItemSelIndex >= MAX_ITEM)
            return;
        KUiObjAtRegion Info;
        Info.Obj.uGenre   = CGOG_ITEM;
        Info.Obj.uId      = m_ImmediaItemSelIndex;
        Info.Region.h     = idx;
        Info.Region.v     = 0;
        Info.Region.Width = Info.Region.Height = 0;

        if (g_pCoreShell)
            g_pCoreShell->OperationRequest(GOI_USE_ITEM, (intptr_t)&Info, UOC_IMMEDIA_ITEM);
        // char msg[64];
        // sprintf(msg,"%s",Item[m_ImmediaItemSelIndex].GetName());
        // messageBox(msg,"测试");
        TempUpdataImmediacy();
    }
}

void KgameWorld::TempUpdataImmediacy()
{
    for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; i++)
    {
        int nItemindex = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetImmediacyItemIndex(i);
        if (nItemindex <= 0)
            UpdateImmediaItem(i, CGOG_NOTHING, 0);
        else
            UpdateImmediaItem(i, CGOG_ITEM, nItemindex);
    }
}

int KgameWorld::getImmediacyBoxindex(int nidx)
{
    int nReg = -1;
    if (nidx <= 0 || nidx >= MAX_ITEM)
        return nReg;

    for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; ++i)
    {
        if (m_ImmediaItem[i].m_uid == nidx)
            return i;
    }
    return nReg;
}

bool KgameWorld::CheckImmediacyBox(int nGenre, int DetailType)
{
    bool nReg = false;
    for (int i = 0; i < UPB_IMMEDIA_ITEM_COUNT; ++i)
    {
        int index = m_ImmediaItem[i].m_uid;
        if (index > 0 && index < MAX_ITEM)
        {
            if (Item[index].GetGenre() == nGenre && Item[index].GetDetailType() == DetailType)
            {
                nReg = true;
                break;
            }
        }
    }
    return nReg;
}

void KgameWorld::setIsPaint(bool ver)
{
    g_ScenePlace.setIsPaint(ver);
}

void KgameWorld::KickOutGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    messageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    {
        if (nAcclient)
        {
            m_bIsClientConnecting = false;
            nAcclient->Cleanup();
            nAcclient->Shutdown();
        }

        if (g_pClient)
        {
            m_bIsPlayerInGame      = false;
            m_bIsGameSevConnecting = false;
            g_pClient->Cleanup();
            delete g_pClient;
            g_pClient = NULL;
        }

        nPlaceId       = -1;
        nNativePlaceId = -1;
        g_ReleaseCore();  // 关闭场景中的所有数据
        // 删除缓存
        // ax::Director::getInstance()->getTextureCache()->removeAnimation(name);
        ax::Director::getInstance()->getTextureCache()->removeUnusedTextures();
        ax::AnimationCache::getInstance()->destroyInstance();
        ax::SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
        ax::Director::getInstance()->replaceScene(Klogin_f::scene());
    }
    // ax::Director::getInstance()->end();

#    if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#    endif
#endif
}

void KgameWorld::setAttackSprVisible(bool ver)
{
    if (attackSpr_dir)
        attackSpr_dir->setVisible(ver);
}

void KgameWorld::setAttackSprPosition(Point nPos)
{
    if (attackSpr_dir)
        attackSpr_dir->setPosition(nPos);
}
// 重写触摸注册函数，重新给定触摸级别
/*void KgameWorld::registerWithTouchDispatcher()
{
        //这里的触摸优先级设置为－128，与CCMenu同级，保证了屏蔽下方的触摸
        ////ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
}*/
