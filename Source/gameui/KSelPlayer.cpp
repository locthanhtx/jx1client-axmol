#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "KSelPlayer.h"
//#include "Klogin.h"
#include "gamescene/MainMenu.h"
#include "KTranSence.h"
#include "engine/CXmlStream.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gameui/PopupLayer.h"
//#include "engine/KPalette.h"
#include "gamescene/SelPlayer.h"
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
#include "gamescene/KUpdateLayer.h"
#include "gameui/KuiSelNativePlace.h"
#include <iostream>
#include <fstream>

extern iCoreShell*	g_pCoreShell; //全局调用
extern KImageStore2 m_ImageStore; //全局调用
//extern KPakList     g_pPakList;
//extern KPakList * g_pPakList;
#include <list>
#include "FairyGUI.h"

USING_NS_FGUI;
USING_NS_AX;

KSelPlayer::KSelPlayer() : _groot(nullptr), _view(nullptr)
{
}

KSelPlayer::~KSelPlayer()
{
    _eventDispatcher->removeEventListener(_touchListener);
    AX_SAFE_RELEASE(_groot);
}

// on "init" you need to initialize your instance
bool KSelPlayer::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Scene::init() )
	{
		return false;
	}

    _groot = GRoot::create(this, 0);
    _groot->retain();

    UIPackage::addPackage("FGUI/SelPlayer");
    _view = UIPackage::createObject("SelPlayer", "Main")->as<GComponent>();
    _groot->addChild(_view);
    _view->getChild("bt_tao")->addClickListener([this](EventContext*) {
        this->mainNewCallback(this);
    });
    _view->getChild("bt_vaogame")->addClickListener([this](EventContext*) {
        this->mainEnterCallback(this);
    });
    _view->getChild("bt_thoat")->addClickListener([this](EventContext*) {
        this->mExit(this);
    });

	m_sprCount         =0;
	//nTimeCount = 0;
	m_nWidth   = 0;
	m_nHeight  = 0;
	clicked    = false;
	//pWW        =NULL;
	Animate* animate         =NULL;
	Animate* Roleanimatea    =NULL;
	Animate* Roleanimateb    =NULL;
	Animate* Roleanimatec    =NULL;
	SelbgSprite = NULL;

    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = AX_CALLBACK_2(KSelPlayer::onTouchBegan, this);
    _touchListener->onTouchMoved = AX_CALLBACK_2(KSelPlayer::onTouchMoved, this);
    _touchListener->onTouchEnded = AX_CALLBACK_2(KSelPlayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_touchListener, -100);

	m_Status = LL_S_ROLE_LIST_READY; //等待角色进入游戏服务器
	m_Result = LL_R_NOTHING;
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	//MenuItemImage *pCloseItem = MenuItemImage::createWithTexture();
	char nSprName[128]={0};
	int nFrams=0;
	m_CurSelIndex = 0;
	//Layer *mainlayer = Layer::create();
	actionManager = Director::getInstance()->getActionManager();
	pRoelb = Label::createWithTTF("", UI_GAME_FONT_DEFAULT, 12);
	pRoelc = Label::createWithTTF("", UI_GAME_FONT_DEFAULT, 12);

	//float m_Scalex,m_Scaley; //与背景 屏幕大小的缩放比例
	//登陆选择框 \Spr\Ui3\登入界面\选存档人物\角色
	t_sprintf(m_szPlayerImgPrefix,"\\Spr\\Ui3\\登入界面\\选存档人物\\角色"); //"\\Spr\\Ui3\\登入界面\\选存档人物\\角色"  "\\spr\\Ui3\\loginui\\saverole\\role"

	//界面动画效果
     t_sprintf(nSprName,"\\spr\\Ui3\\loginui\\uidonghua\\yezi.spr");
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
            t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,i);
            //messageBox(nSprFilePath,"animation");
            auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
            if (texture) {
                Rect rect = Rect::ZERO;
                rect.size = texture->getContentSize();
                animation->addSpriteFrameWithTexture(texture, rect);
            }
//				animation->addSpriteFrameWithFile(nySprFilePath);//往这个动画增加纹理帧
        }
        //设置动画帧的时间间隔
        animation->setDelayPerUnit(0.10f);
        //设置播放循环 一直播放 为-1
        animation->setLoops(-1);
        //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
        animation->setRestoreOriginalFrame(true);
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
		if (m_nNumRole>0)
		{//如果角色大于零的 就开始创建精灵
			static const char* pszAttribute[series_num] = {"金","木","水","火","土"};
			static const char* pszGender[2] = { "男", "女" };
			for (int i = 0; i < m_nNumRole; ++i)
			{
				char nInfo[64]={0};
				char szFileName[128]={0};
				t_sprintf(nCurRoleList[i].Name,m_RoleList[i].Name);       //姓名
				nCurRoleList[i].nLevel    = m_RoleList[i].nLevel;
				nCurRoleList[i].Attribute = m_RoleList[i].Attribute;
				nCurRoleList[i].Gender    = m_RoleList[i].Gender;
				t_sprintf(nInfo,"%d 级",m_RoleList[i].nLevel);             //等级
				m_Gender[i]               = m_RoleList[i].Gender;        //性别
				m_Attribute[i]            = m_RoleList[i].Attribute;     //五行属性
				GetRoleImageName(szFileName, m_szPlayerImgPrefix, m_RoleList[i].Gender, m_RoleList[i].Attribute,2);//2 灰色动画 0选中 1 为一性过渡动画
				nFrams=0;
				g_StrLower(szFileName);
				//角色1
				if (i == 0)
				{
					Texture2D *RoleCura = _getinidata.getinidata_new(szFileName,0,&m_nWidth,&m_nHeight,&nFrams);
					Sprite *RoleSpritea = NULL;
					if (RoleCura)
					{
						RoleSpritea=Sprite::createWithTexture(RoleCura);
						RoleSpritea->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y + 60));
                        RoleSpritea->setScale(1.4f);
						m_sprCount++;
						this->addChild(RoleSpritea,2,1000);
                        _view->getChild("role_name_a")->setText(nCurRoleList[i].Name);
                        _view->getChild("level_a")->setText(std::to_string(nCurRoleList[i].nLevel));
                    }
                char nySprFilePath[64]={0};
                Animation*animation = Animation::create();//创建一个动画
                unsigned long nFielpahtdwid = g_FileName2Id(szFileName);
                int j;
                for (j=0;j<nFrams;j++)
                {
                    t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,j);
                    //messageBox(nSprFilePath,"animation");
                    auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                    if (texture) {
                        Rect rect = Rect::ZERO;
                        rect.size = texture->getContentSize();
                        animation->addSpriteFrameWithTexture(texture, rect);
                    }
//						animation->addSpriteFrameWithFile(nySprFilePath);//往这个动画增加纹理帧
                }
                //设置动画帧的时间间隔
                animation->setDelayPerUnit(0.10f);
                //设置播放循环 一直播放 为-1
                animation->setLoops(-1);
                //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
                animation->setRestoreOriginalFrame(true);
                // 创建一个永远循环的动画
                Roleanimatea = Animate::create(animation);
                //加入动画缓存
                //sprintf(nySprFilePath,"%u",nFielpahtdwid);
                ax::AnimationCache::getInstance()->addAnimation(animation, "0_2");
                //RepeatForever* forever = RepeatForever::create(animate);

                GetRoleImageName(szFileName, m_szPlayerImgPrefix, m_RoleList[i].Gender, m_RoleList[i].Attribute, 0);
                g_StrLower(szFileName);
                _getinidata.getinidata_new(szFileName,0,&m_nWidth,&m_nHeight,&nFrams);
                Animation*animationb = Animation::create();//创建一个动画
                nFielpahtdwid = g_FileName2Id(szFileName);
                for (j=0;j<nFrams;j++)
                {
                    t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,j);
                    //messageBox(nSprFilePath,"animation");
                    auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                    if (texture) {
                        Rect rect = Rect::ZERO;
                        rect.size = texture->getContentSize();
                        animationb->addSpriteFrameWithTexture(texture, rect);
                    }

//						animationb->addSpriteFrameWithFile(nySprFilePath);//往这个动画增加纹理帧
                }
                //设置动画帧的时间间隔
                animationb->setDelayPerUnit(0.10f);
                //设置播放循环 一直播放 为-1
                animationb->setLoops(-1);
                //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
                animationb->setRestoreOriginalFrame(true);
                // 创建一个永远循环的动画
                //Roleanimatea = Animate::create(animationb);
                //加入动画缓存
                //sprintf(nySprFilePath,"%u",nFielpahtdwid);
                ax::AnimationCache::getInstance()->addAnimation(animationb,"0_0");

                GetRoleImageName(szFileName, m_szPlayerImgPrefix, m_RoleList[i].Gender, m_RoleList[i].Attribute, 1);
                g_StrLower(szFileName);
                _getinidata.getinidata_new(szFileName,0,&m_nWidth,&m_nHeight,&nFrams);
                Animation*animationc = Animation::create();//创建一个动画
                nFielpahtdwid = g_FileName2Id(szFileName);
                for (j=0;j<nFrams;j++)
                {
                    t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,j);
                    //messageBox(nSprFilePath,"animation");
                    auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                    if (texture) {
                        Rect rect = Rect::ZERO;
                        rect.size = texture->getContentSize();
                        animationc->addSpriteFrameWithTexture(texture, rect);
                    }

//						animationc->addSpriteFrameWithFile(nySprFilePath);//往这个动画增加纹理帧
                }
                //设置动画帧的时间间隔
                animationc->setDelayPerUnit(0.10f);
                //设置播放循环 一直播放 为-1
                animationc->setLoops(1);
                //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
                animationc->setRestoreOriginalFrame(true);
                // 创建一个永远循环的动画
                //Roleanimatea = Animate::create(animationb);
                //加入动画缓存
                ax::AnimationCache::getInstance()->addAnimation(animationc,"0_1");

                if (RoleSpritea) {
                    auto callFunc = ax::CallFunc::create([this]() {
                        this->finish();
                    });
                    RoleSpritea->runAction(Sequence::create(Roleanimatea, callFunc, NULL));
                }

                //RoleSpritea->getBoundingBox();
                //MenuItemSprite* sprite2 = MenuItemSprite::create(RoleSpritea, RoleSpritea, AX_CALLBACK_1(KuiSelNativePlace::finish));
                //Menu* dddmenu = Menu::create(sprite2, NULL);
                //dddmenu->alignItemsVerticallyWithPadding(100);
                //dddmenu->setPosition(ax::Vec2(size.width / 2, size.height / 2));
            }
            //角色2
            if (i == 1)
            {
                Texture2D *RoleCurb = _getinidata.getinidata_new(szFileName,0,&m_nWidth,&m_nHeight,&nFrams);
                Sprite *RoleSpriteb = NULL;
                if (RoleCurb)
                {
                    RoleSpriteb=Sprite::createWithTexture(RoleCurb);
                    RoleSpriteb->setPosition(ax::Vec2(visibleSize.width/2 - RoleSpriteb->getContentSize().width / 2, visibleSize.height/2 + origin.y + 60));
                    //RoleSpriteb->setScaleX(m_Scalex); //设置精灵宽度缩放比例
                    //RoleSpriteb->setScaleY(m_Scaley);
                    RoleSpriteb->setScale(1.4f);
                    m_sprCount++;
                    this->addChild(RoleSpriteb,2,999);
                    _view->getChild("role_name_b")->setText(nCurRoleList[i].Name);
                    _view->getChild("level_b")->setText(std::to_string(nCurRoleList[i].nLevel));
                }
                char nySprFilePath[64]={0};
                Animation*animation = Animation::create();//创建一个动画
                unsigned long nFielpahtdwid = g_FileName2Id(szFileName);
                int j;
                for (j=0;j<nFrams;j++)
                {
                    t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,j);
                    //messageBox(nSprFilePath,"animation");
                    auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                    if (texture) {
                        Rect rect = Rect::ZERO;
                        rect.size = texture->getContentSize();
                        animation->addSpriteFrameWithTexture(texture, rect);
                    }

//						animation->addSpriteFrameWithFile(nySprFilePath);//往这个动画增加纹理帧
                }
                //设置动画帧的时间间隔
                animation->setDelayPerUnit(0.10f);
                //设置播放循环 一直播放 为-1
                animation->setLoops(-1);
                //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
                animation->setRestoreOriginalFrame(true);
                // 创建一个永远循环的动画
                Roleanimateb = Animate::create(animation);
                //加入动画缓存
                ax::AnimationCache::getInstance()->addAnimation(animation,"1_2");
                //RepeatForever* forever = RepeatForever::create(animate);

                GetRoleImageName(szFileName,m_szPlayerImgPrefix, m_RoleList[i].Gender,m_RoleList[i].Attribute,0);
                g_StrLower(szFileName);
                _getinidata.getinidata_new(szFileName,0,&m_nWidth,&m_nHeight,&nFrams);
                Animation*animationb = Animation::create();//创建一个动画
                nFielpahtdwid = g_FileName2Id(szFileName);
                for (j=0;j<nFrams;j++)
                {
                    t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,j);
                    //messageBox(nSprFilePath,"animation");
                    auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                    if (texture) {
                        Rect rect = Rect::ZERO;
                        rect.size = texture->getContentSize();
                        animationb->addSpriteFrameWithTexture(texture, rect);
                    }

//						animationb->addSpriteFrameWithFile(nySprFilePath);//往这个动画增加纹理帧
                }
                //设置动画帧的时间间隔
                animationb->setDelayPerUnit(0.10f);
                //设置播放循环 一直播放 为-1
                animationb->setLoops(-1);
                //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
                animationb->setRestoreOriginalFrame(true);
                // 创建一个永远循环的动画
                //Roleanimatea = Animate::create(animationb);
                //加入动画缓存
                //sprintf(nySprFilePath,"%u",nFielpahtdwid);
                ax::AnimationCache::getInstance()->addAnimation(animationb,"1_0");

                GetRoleImageName(szFileName, m_szPlayerImgPrefix, m_RoleList[i].Gender, m_RoleList[i].Attribute,1);
                g_StrLower(szFileName);
                _getinidata.getinidata_new(szFileName,0,&m_nWidth,&m_nHeight,&nFrams);
                Animation*animationc = Animation::create();//创建一个动画
                nFielpahtdwid = g_FileName2Id(szFileName);
                for (j=0;j<nFrams;j++)
                {
                    t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,j);
                    //messageBox(nSprFilePath,"animation");
                    auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                    if (texture) {
                        Rect rect = Rect::ZERO;
                        rect.size = texture->getContentSize();
                        animationc->addSpriteFrameWithTexture(texture, rect);
                    }

//						animationc->addSpriteFrameWithFile(nySprFilePath);//往这个动画增加纹理帧
                }
                //设置动画帧的时间间隔
                animationc->setDelayPerUnit(0.10f);
                //设置播放循环 一直播放 为-1
                animationc->setLoops(1);
                //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
                animationc->setRestoreOriginalFrame(true);
                // 创建一个永远循环的动画
                //Roleanimatea = Animate::create(animationb);
                //加入动画缓存
                ax::AnimationCache::getInstance()->addAnimation(animationc,"1_1");

                if (RoleSpriteb) {
                    auto callFunc = ax::CallFunc::create([this]() {
                        this->finish();
                    });
                    RoleSpriteb->runAction(Sequence::create(Roleanimateb, callFunc, NULL));
                }
            }
            //角色3
            if (i == 2)
            {
                Texture2D *RoleCurc = _getinidata.getinidata_new(szFileName,0,&m_nWidth,&m_nHeight,&nFrams);
                Sprite *RoleSpritec = NULL;
                if (RoleCurc)
                {
                    RoleSpritec=Sprite::createWithTexture(RoleCurc);
                    RoleSpritec->setPosition(ax::Vec2(visibleSize.width - 300, visibleSize.height/2 + origin.y + 60));
                    //RoleSpritec->setScaleX(m_Scalex); //设置精灵宽度缩放比例
                    //RoleSpritec->setScaleY(m_Scaley);
                    RoleSpritec->setScale(1.4f);
                    m_sprCount++;
                    this->addChild(RoleSpritec,2,998);
                    _view->getChild("role_name_c")->setText(nCurRoleList[i].Name);
                    _view->getChild("level_c")->setText(std::to_string(nCurRoleList[i].nLevel));
                }
                //Rect::
                char nySprFilePath[64]={0};
                Animation*animation = Animation::create();//创建一个动画
                unsigned long nFielpahtdwid = g_FileName2Id(szFileName);
                int j;
                for (j=0;j<nFrams;j++)
                {
                    t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,j);
                    //messageBox(nSprFilePath,"animation");
                    auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                    if (texture) {
                        Rect rect = Rect::ZERO;
                        rect.size = texture->getContentSize();
                        animation->addSpriteFrameWithTexture(texture, rect);
                    }
//						animation->addSpriteFrameWithFile(nySprFilePath);//往这个动画增加纹理帧
                }
                //设置动画帧的时间间隔
                animation->setDelayPerUnit(0.10f);
                //设置播放循环 一直播放 为-1
                animation->setLoops(-1);
                //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
                animation->setRestoreOriginalFrame(true);
                // 创建一个永远循环的动画
                Roleanimatec = Animate::create(animation);
                //加入动画缓存
                ax::AnimationCache::getInstance()->addAnimation(animation,"2_2");
                //RepeatForever* forever = RepeatForever::create(animate);

                GetRoleImageName(szFileName, m_szPlayerImgPrefix, m_RoleList[i].Gender, m_RoleList[i].Attribute, 0);
                g_StrLower(szFileName);
                _getinidata.getinidata_new(szFileName,0,&m_nWidth,&m_nHeight,&nFrams);
                Animation*animationb = Animation::create();//创建一个动画
                nFielpahtdwid = g_FileName2Id(szFileName);
                for (j=0;j<nFrams;j++)
                {
                    t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,j);
                    //messageBox(nSprFilePath,"animation");
                    auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                    if (texture) {
                        Rect rect = Rect::ZERO;
                        rect.size = texture->getContentSize();
                        animationb->addSpriteFrameWithTexture(texture, rect);
                    }

//						animationb->addSpriteFrameWithFile(nySprFilePath);//往这个动画增加纹理帧
                }
                //设置动画帧的时间间隔
                animationb->setDelayPerUnit(0.10f);
                //设置播放循环 一直播放 为-1
                animationb->setLoops(-1);
                //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
                animationb->setRestoreOriginalFrame(true);
                // 创建一个永远循环的动画
                //Roleanimatea = Animate::create(animationb);
                //加入动画缓存
                //sprintf(nySprFilePath,"%u",nFielpahtdwid);
                ax::AnimationCache::getInstance()->addAnimation(animationb,"2_0");

                GetRoleImageName(szFileName, m_szPlayerImgPrefix, m_RoleList[i].Gender, m_RoleList[i].Attribute, 1);
                g_StrLower(szFileName);
                _getinidata.getinidata_new(szFileName,0,&m_nWidth,&m_nHeight,&nFrams);
                Animation*animationc = Animation::create();//创建一个动画
                nFielpahtdwid = g_FileName2Id(szFileName);
                for (j=0;j<nFrams;j++)
                {
                    t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,j);
                    //messageBox(nSprFilePath,"animation");
                    auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                    if (texture) {
                        Rect rect = Rect::ZERO;
                        rect.size = texture->getContentSize();
                        animationc->addSpriteFrameWithTexture(texture, rect);
                    }

//						animationc->addSpriteFrameWithFile(nySprFilePath);//往这个动画增加纹理帧
                }
                //设置动画帧的时间间隔
                animationc->setDelayPerUnit(0.10f);
                //设置播放循环 一直播放 为-1
                animationc->setLoops(1);
                //设置动画结束后恢复到第一帧 setRestoreOriginalFrame
                animationc->setRestoreOriginalFrame(true);
                // 创建一个永远循环的动画
                //Roleanimatea = Animate::create(animationb);
                //加入动画缓存
                ax::AnimationCache::getInstance()->addAnimation(animationc,"2_1");

                if (RoleSpritec) {
                    auto callFunc = ax::CallFunc::create([this]() {
                        this->finish();
                    });
                    RoleSpritec->runAction(Sequence::create(Roleanimatec, callFunc, NULL));
                }
            }
        }
    }

	//DrawNode* drawNode = ccDrawNode::create();
	this->scheduleUpdate();  //遥杆更新调用函数
	//this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE); //禁止多点触摸
	return true;
}
//新建立角色
void KSelPlayer::mainNewCallback(Object* pSender)
{
	int nSel;
	if (GetRoleCount(nSel) < MAX_PLAYER_PER_ACCOUNT)
	{//打开新建立角色窗口
		Director::getInstance()->replaceScene(KuiSelNativePlace::scene());
	}
}

int KSelPlayer::GetRoleCount(int& nAdviceChoice)
{
	nAdviceChoice = 0;
	if (m_ClientChoices.szProcessingRoleName[0])
	{
		for (int i = 0; i < m_nNumRole; ++i)
		{
			if (strcmp(m_ClientChoices.szProcessingRoleName, m_RoleList[i].Name) == 0)
			{
				nAdviceChoice = i;
				break;
			}
		}
	}
	return m_nNumRole;
}
//删除角色
void KSelPlayer::mainDelCallback(Object* pSender)
{
	/*if (nAcclient)
	{
		nAcclient->Cleanup();
		m_bIsClientConnecting=false;
	}

	Director::getInstance()->replaceScene(KUpdateLayer::scene());*/
}
//进入游戏
void KSelPlayer::mainEnterCallback(Object* pSender)
{//
	if (nAcclient/* && g_pClient*/)
	{
		EnterGame();
	}
}

//循环更新虚函数 默认自动调用
void KSelPlayer::update(float delta)
{
	unsigned int nSize = 0;
	const char* pBuffer = NULL;
	if (m_bIsClientConnecting)
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
			if  (Msg == s2c_login || Msg == s2c_roleserver_getrolelist_result || Msg == s2c_notifypl_ayerlogin_hfkqotivbhd || Msg == s2c_rolenewdelresponse)
			     AcceptNetMsg(pMsg);
		}
	}
	//----处理来自游戏服务器的数据----
	if (/*m_bIsGameSevConnecting && */g_pClient)
	{
		while (true)
		{
			if (!g_pClient)	//GS的数据包
				break;
			//无限循环从服务器获取数据小包
			nSize  = 0;
			pBuffer = NULL;
			pBuffer = (const char*)g_pClient->GetPackFromServer(nSize);

			if (!(pBuffer && nSize))
				break;
			PROTOCOL_MSG_TYPE *pMsg = (PROTOCOL_MSG_TYPE*)pBuffer; //当前包

			while(pMsg < (PROTOCOL_MSG_TYPE*)(pBuffer + nSize))
			{//帅选数据包
				PROTOCOL_MSG_TYPE Msg = *pMsg;
				// 跨服务器的协议
				if (Msg == s2c_notifyplayerexchange)
				{//转换地图或转服
					//ProcessSwitchGameSvrMsg(pMsg);
					break;
				}
				else if (g_pCoreShell)
				{
					if  (Msg <= s2c_clientbegin ||  Msg >= s2c_end || g_pCoreShell->GetProtocolSize(Msg)==0)
						break;

					g_pCoreShell->NetMsgCallbackFunc(pMsg);          //接受从服务器发来的协议消息

					if (g_pCoreShell->GetProtocolSize(Msg) > 0)
						pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + g_pCoreShell->GetProtocolSize(Msg));
					else //没有限制长度的 -1
						pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + PROTOCOL_MSG_SIZE + (*(unsigned short*)(((char*)pMsg) + PROTOCOL_MSG_SIZE)));
				}
			}
		}
	}
}

Rect KSelPlayer::getRect(Node* pNode)
{
	Rect  rc;
	rc.origin     = pNode->getPosition();
	rc.size       = pNode->getContentSize();
	rc.origin.x -= 50;//rc.size.width;
	rc.origin.y -= 80;//rc.size.height;
	rc.size.width /=6;
	rc.size.height /=4;
	//Size s = pNode->getContentSize();
	//return ax::Rect(-s.width / 2, -s.height / 2, s.width, s.height);
	return rc;
}
void KSelPlayer::draw(Renderer* renderer, const Mat4& transform, unsigned int flags)
{

	if (m_Result != LL_R_NOTHING)
	{
		if (nAcclient)
		{
			m_bIsPlayerInGame =false;
			m_bIsClientConnecting=false;
			nAcclient->Cleanup();
		}

		if (g_pClient)
		{;
			m_bIsPlayerInGame =false;
			m_bIsGameSevConnecting=false;
			g_pClient->Cleanup();
			delete g_pClient;
			g_pClient=NULL;
		}
		if  (m_Result==LL_R_INVALID_PROTOCOLVERSION)
		{//版本不队
#ifdef WIN32
			messageBox("版本不对","提示:");
#else
			messageBox(UTEXT("版本不对",1).c_str(),UTEXT("提示:",1).c_str());
#endif
		}
		Director::getInstance()->replaceScene(MainMenu::create());
		return;
	}

	if (m_Status == LL_S_ENTERING_GAME)
	{//马上跳转画面
		//Director::getInstance()->replaceScene(KTranSence::scene());
		Director::getInstance()->replaceScene(KgameWorld::scene());
		return;
	}
}
//
bool KSelPlayer::isFileExist(const char* pFileName)
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

void KSelPlayer::copyData(const char* pFileName)
{
    std::string strPath = ax::FileUtils::getInstance()->fullPathForFilename(pFileName);
    unsigned int len = 0;
    unsigned char * data = NULL;

    std::ifstream file(strPath, std::ios::in);
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

/*
void KSelPlayer::onEnter()
{

}

void KSelPlayer::onExit()
{

}
//进入后 有 过渡完成
void KSelPlayer::onEnterTransitionDidFinish()
{

}*/

bool KSelPlayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (clicked)
    {
        clicked = false;
        messageBox("双击","警告");
    }
    else
    {
        //延时0.25s执行（注意在这0.25s的延时过程中clicked已经为true），  
        //如果在这个过程中再次click，那么就执行上面的双击事件处理了  
        //否则，那么就执行下面的回调函数了，处理单击事件 
//		scheduleOnce(CC_SCHEDULE_SELECTOR(KSelPlayer::CheckClick),0.25f);
        scheduleOnce([=](float dt){
            this->CheckClick(dt);
        }, 0.25f, "unique_key");
        clicked =false;
        //定时器
        //this->schedule(schedule_selector(LoginScenes::movebd));
    }

	return true;
}
void KSelPlayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void KSelPlayer::onTouchCancelled(Touch *pTouch, Event *pEvent)
{

}


void KSelPlayer::CheckClick(float tt)
{
	if (clicked)
	{//单击
	   clicked = false;
	   messageBox("单击","警告");
	}
}

void KSelPlayer::finish()
{
	Animation* nCurAnimat = NULL;
	if  (m_CurSelIndex==0)
	{
		//actionManager->removeAllActionsFromTarget(RoleNodea);
		nCurAnimat= ax::AnimationCache::getInstance()->getAnimation("0_0");
		if (nCurAnimat)
		    RoleNodea->runAction(Sequence::create(Animate::create(nCurAnimat),NULL,NULL));
	}

	if  (m_CurSelIndex==1)
	{
		//actionManager->removeAllActionsFromTarget(RoleNodeb);
		nCurAnimat= ax::AnimationCache::getInstance()->getAnimation("1_0");
		if (nCurAnimat)
		   RoleNodeb->runAction(Sequence::create(Animate::create(nCurAnimat),NULL,NULL));
	}

	if  (m_CurSelIndex==2)
	{
		//actionManager->removeAllActionsFromTarget(RoleNodec);
		nCurAnimat= ax::AnimationCache::getInstance()->getAnimation("2_0");
		if (nCurAnimat)
		   RoleNodec->runAction(Sequence::create(Animate::create(nCurAnimat),NULL,NULL));
	}

}

void KSelPlayer::didAccelerate(Acceleration* pAccelerationValue)
{

}

void KSelPlayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    Point location = pTouch->getLocation();//目的的坐标?屏幕点击处
    RoleNodea = (Sprite *)this->getChildByTag(1000);
    RoleNodeb = (Sprite *)this->getChildByTag(999);
    RoleNodec = (Sprite *)this->getChildByTag(998);
    char nRoleInfo[128]={0};
    if (RoleNodea)
    {
        if (getRect(RoleNodea).containsPoint(location))
        {
            m_CurSelIndex = 0;
            if (m_CurSelIndex == 0)
            {//原来就等于0 灰色的
                actionManager->removeAllActionsFromTarget(RoleNodea);
                //RoleNodea->runAction(RepeatForever::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("0_1"))));
                auto callFunc = ax::CallFunc::create([this]() {
                    this->finish();
                });
                RoleNodea->runAction(Sequence::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("0_1")), callFunc, NULL));
                if (RoleNodeb)
                {
                    actionManager->removeAllActionsFromTarget(RoleNodeb);
                    RoleNodeb->runAction(Sequence::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("1_2")),NULL,NULL));
                }
                if (RoleNodec)
                {
                    actionManager->removeAllActionsFromTarget(RoleNodec);
                    RoleNodec->runAction(Sequence::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("2_2")),NULL,NULL));
                }
            }
            //EnterGame();
        }
    }


    if (RoleNodeb)
    {
        if (getRect(RoleNodeb).containsPoint(location))
        {
            m_CurSelIndex = 1;
            if (m_CurSelIndex == 1)
            {//原来就等于0 灰色的
                actionManager->removeAllActionsFromTarget(RoleNodeb);
                //RoleNodea->runAction(RepeatForever::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("0_1"))));
                auto callFunc = ax::CallFunc::create([this]() {
                    this->finish();
                });
                RoleNodeb->runAction(Sequence::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("1_1")), callFunc, NULL));
                if (RoleNodea)
                {
                    actionManager->removeAllActionsFromTarget(RoleNodea);
                    RoleNodea->runAction(Sequence::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("0_2")),NULL,NULL));
                }
                if (RoleNodec)
                {
                    actionManager->removeAllActionsFromTarget(RoleNodec);
                    RoleNodec->runAction(Sequence::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("2_2")),NULL,NULL));
                }
            }

            //EnterGame();
        }
    }


    if (RoleNodec)
    {
        if (getRect(RoleNodec).containsPoint(location))
        {
            m_CurSelIndex = 2;
            if (m_CurSelIndex == 2)
            {//原来就等于0 灰色的
                actionManager->removeAllActionsFromTarget(RoleNodec);
                //RoleNodea->runAction(RepeatForever::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("0_1"))));
                auto callFunc = ax::CallFunc::create([this]() {
                    this->finish();
                });
                RoleNodec->runAction(Sequence::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("2_1")), callFunc, NULL));
                if (RoleNodea)
                {
                    actionManager->removeAllActionsFromTarget(RoleNodea);
                    RoleNodea->runAction(Sequence::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("0_2")),NULL,NULL));
                }
                if (RoleNodeb)
                {
                    actionManager->removeAllActionsFromTarget(RoleNodeb);
                    RoleNodeb->runAction(Sequence::create(Animate::create(ax::AnimationCache::getInstance()->getAnimation("1_2")),NULL,NULL));
                }
            }
            //EnterGame();
        }
    }
}

void KSelPlayer::mExit(Object* pSender){

	if (nAcclient)
	{//断开连接
		m_bIsClientConnecting=false;
		m_bIsPlayerInGame  = false;
		nAcclient->Cleanup();
	}

	if (g_pClient)
	{
		m_bIsPlayerInGame  = false;
		m_bIsGameSevConnecting=false;
		g_pClient->Cleanup();
		delete g_pClient;
		g_pClient=NULL;
	}
	//退到 开始登陆界面
	Director::getInstance()->replaceScene(MainMenu::create());
}

void KSelPlayer::GetRoleImageName(char* pszName, const char* pszPrefix, unsigned char bGender, unsigned char bAttribute, int nIndex)
{
	//CCAssert(pszName && bAttribute >= series_metal && bAttribute < series_num,"");
	if  (!pszName || bAttribute < series_metal || bAttribute >= series_num)
		return;

	static const char* pszAttribute[series_num] = {"金","木","水","火","土"};
	static const char* pszGender[2] = { "男", "女" };
	sprintf(pszName,"%s_%s_%s_%d.spr", pszPrefix, pszAttribute[bAttribute],
		(bGender ? pszGender[1] : pszGender[0]), nIndex);  //spr格式 门派 人物spr  角色_系_性别_性别编码
}

void KSelPlayer::AcceptNetMsg(void* pMsgData) //循环接受处理消息
{
	if (pMsgData == NULL)  //数据包为空 则返回
		return;
	//messageBox("回调成功","GetPackFromServer");
	switch(m_Status)       //发送数据包标识变量
	{
	case LL_S_ACCOUNT_CONFIRMING:  //账号验证
		//ProcessAccountLoginResponse((KLoginStructHead*) (((char*)pMsgData) + PROTOCOL_MSG_SIZE));
		break;
	case LL_S_WAIT_ROLE_LIST:      //等待角色清单
		//ProcessRoleListResponse((TProcessData*)pMsgData);   //接收协议角色信息
		break;
	case LL_S_CREATING_ROLE:       //创建角色
		//ProcessCreateRoleResponse((tagNewDelRoleResponse*)pMsgData); //创建角色
		break;
	case LL_S_DELETING_ROLE:       //删除角色
		//ProcessDeleteRoleResponse((tagNewDelRoleResponse*)pMsgData);
		break;
	case LL_S_WAIT_TO_DEL:         //已经取消等待 删除角色
		//ProcessGetRoleResponse((tagNotifyPlayerLogin*)pMsgData);
		break;
	case LL_S_WAIT_TO_LOGIN_GAMESERVER:  //等待链接到服务器
		ProcessToLoginGameServResponse((tagNotifyPlayerLogin*)pMsgData); //调用链接到服务器的函数
		break;
	}
}

//--------------------------------------------------------------------------
//	功能：准备登陆游戏服务器的响应---无限线程接包-有问题 空包
//	状态切换：LL_S_WAIT_TO_LOGIN_GAMESERVER -> LL_S_ENTERING_GAME
//--------------------------------------------------------------------------
void KSelPlayer::ProcessToLoginGameServResponse(tagNotifyPlayerLogin* pResponse)
{
	//messageBox("收包成功","提示");
	if (pResponse->cProtocol == s2c_notifypl_ayerlogin_hfkqotivbhd)  ///bishop 传过来的协议头
	{
		char pzAc[32]={0};
		for (int k = 0; k < 32;k++)
		{
			pzAc[k] = ~m_ClientChoices.Account[k];
		}

		if (!pResponse->bPermit &&(pResponse->nIPAddr==0 || pResponse->nPort==0))
		{
			m_Result = LL_R_OPNE_ACCOUNT; //卡号 正在尝试打开账号
			messageBox("Không thể đăng nhập vào máy chủ","Cảnh báo");
			return;
		}

		if (!pResponse->bPermit)
		{
			//ReturnToIdle();
			m_Result = LL_R_OPNE_ACCOUNT; //卡号 正在尝试打开账号
			messageBox("Đang mở tài khoản","Nhắc:");
			return;
		}
		//开始检查预备建立角色
		/*if (pInfo->ProtocolVersion==0)
			{
#ifdef WIN32
				messageBox("当前服务器处理预建立角色状态,不能登陆","提示:");
#else
				messageBox(UTEXT("当前服务器处理预建立角色状态,不能登陆",1).c_str(),UTEXT("提示:",1).c_str());
#endif
				return;
			}*/
		if (strcmp((const char*)pResponse->szRoleName, m_ClientChoices.szProcessingRoleName) == 0
			&& strcmp((const char*)pResponse->szAccountName, pzAc) == 0)
		{
			    // g_NetConnectAgent.UpdateClientRequestTime(true); //时间归零
			if (NULL==g_pClient)
			{
				const unsigned int bufferSize           = 1024;   //Scoket决定发包的大小 分配的内存(m_bufferSize > 0) ? m_bufferSize : (1024*64);
				const unsigned int bufferSize_Cache     = 1024*512; //分配的内存 读包 接包的缓存大小
				const unsigned int maxFreeBuffers	      = 2;        //Scoket保留的数量
				const unsigned int maxFreeBuffers_Cache = 2;        //读包 接包的缓存 保留的数量
				g_pClient     = new CGameClient(maxFreeBuffers,maxFreeBuffers_Cache,bufferSize_Cache,bufferSize,1); //2,2   8
			}
			if (g_pClient)
			{   // (const unsigned char*)&pResponse->nIPAddr
				const unsigned char* pIpAddress = (const unsigned char*)&pResponse->nIPAddr;
				char Address[128]={0}; //IP地址
				t_sprintf(Address, "%d.%d.%d.%d", pIpAddress[0], pIpAddress[1],pIpAddress[2], pIpAddress[3]);
			    //  messageBox(Address,"开始连接服务器..!");
				if (g_pClient->FsGameServerConnectTo(Address,pResponse->nPort))
				{
				//	messageBox(Address,"连接服务器成功!");
					g_pClient->Startup();
					m_bIsGameSevConnecting = true;	             //服务器已经链接
					g_pClient->setSocketStates(true);            //设置已经连接
					tagLogicLogin ll;
					ll.cProtocol = c2s__loginfs_kfjghtueodnchsf; //通知GS服务器开始登陆	  赋值我网络号号给角色

					GUID* pGuid = &pResponse->guid;//服务器端唯一的标示
					memcpy(&ll.guid,pGuid,sizeof(GUID));

					if (!g_pClient->SendPackToServer(&ll,sizeof(tagLogicLogin)))
					{
						m_Result = LL_R_CONNECT_SERV_BUSY; //卡号 正在尝试打开账号
						g_pClient->Cleanup();
						messageBox("连接服务器失败","提示:");
						return;
					}
					//开始断开网关的连接
					if (nAcclient)
					{
						m_bIsClientConnecting =false;
						nAcclient->Cleanup(true); //只杀掉线程，不断开连接
						nAcclient->Shutdown();
					}
					m_Status = LL_S_ENTERING_GAME;//正在进入游戏中
					m_Result = LL_R_NOTHING;      //连接成功了
				}
				else
				{//连接失败
					m_Result = LL_R_CONNECT_FAILED;
					messageBox("连接服务器失败","提示:");
				}
			}
			// 断开与网关的连接
			//g_NetConnectAgent.DisconnectClient();
		}
		else
		{
			//ReturnToIdle();
			m_Result = LL_R_SERVER_SHUTDOWN; //提示服务器满员或离线中
			messageBox("连接服务器失败","提示:");
		}
	}
}

void KSelPlayer::EnterGame()
{
	if (!isthisVer)
	{
		m_Result = LL_R_INVALID_PROTOCOLVERSION;
		return;
	}
	if (m_bIsClientConnecting && nAcclient)
	{//帐号服务器如果是可以用的
		if (m_Status == LL_S_ROLE_LIST_READY && m_CurSelIndex >= 0 && m_CurSelIndex < m_nNumRole)
		{
			//messageBox("EnterGame","2222");
			tagDBSelPlayer	NetCommand;
			NetCommand.cProtocol = c2s_dbplayerselect;
			strcpy(NetCommand.szRoleName, nCurRoleList[m_CurSelIndex].Name);
			nAcclient->SendPackToServer(&NetCommand, sizeof(tagDBSelPlayer));
			//g_NetConnectAgent.UpdateClientRequestTime(false);
			strcpy(m_ClientChoices.szProcessingRoleName,nCurRoleList[m_CurSelIndex].Name);
			m_Status = LL_S_WAIT_TO_LOGIN_GAMESERVER;	 //链接服务器
			m_Result = LL_R_NOTHING;
		}
		else
		{
			m_Result = LL_R_CONNECT_TIMEOUT;
			//m_Result = LL_R_CONNECT_FAILED; //链接失败！
		}
	}
}
