#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiSkilldesc.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gamecore/KPlayer.h"
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
#include "engine/Text.h"
#include "gamecore/KSkillManager.h"
#include "gamecore/KItem.h"
#include "gameui/KuiItem.h"
#include "gameui/KuiItemEx.h"
#include "gameui/KuiShop.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

KuiSkilldesc::KuiSkilldesc()
{
	isOpen = false;
	memset(&skillData,0,sizeof(KUiItemdObject));
	ptitleLabel       = NULL;
	potherLabel       = NULL;
	pdescLabel        = NULL;
	pScriptLabel      = NULL;
	pScriptLabel_n    = NULL;
	pbaseLabel        = NULL;   //基本属性
	prqLabel          = NULL;   //需求属性
	pmagicLabel       = NULL;   //魔法属性
	colorLayer        = NULL;
	red               = NULL;

	//m_nMoney = 0;
	//m_nXu = 0;
	//m_nNumObjects = 0;
	m_StartPos.x=0;
    _nKind = 0;
	m_StartPos.y=0;
	//m_ItemIndex = 0;
}

KuiSkilldesc::~KuiSkilldesc()
{
	isOpen = false;
	ptitleLabel = NULL;
	//m_ItemIndex = 0;
	colorLayer        = NULL;
	if (red)
	    red->release();
	//red               = NULL;
}

//创建确定弹出框
KuiSkilldesc * KuiSkilldesc::create(int nskillId,int nLevel,KUiItemdObject nKillData,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun,int nKind)
{
	KuiSkilldesc * popLayer = KuiSkilldesc::create();
	popLayer->skillData       = nKillData;
	popLayer->skillData.DataX = nLevel;
	popLayer->_nKind          = nKind;
	//popLayer->setTitle(title);
	//char msg[64];
	//sprintf(msg,"%d",popLayer->m_ItemData.uId);
	//messageBox(msg,"KuiSkilldesc");
	popLayer->addDialogData();
	//设置按钮
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}



//初始化
bool KuiSkilldesc::init()
{
	if ( !Layer::init() ){
		return false;
	}

	//精灵帧缓存
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//添加模态背景
	Size winSize = ax::Director::getInstance()->getWinSize();

	Color4B color(0,0,0,150);
	colorLayer = LayerColor::create(color);//颜色层
	//colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setAnchorPoint(ax::Vec2(0,0));
	Size nColorSize;
	nColorSize.width = 200;
	nColorSize.height= 200;
	colorLayer->setContentSize(nColorSize);//设置和窗口的大小
	this->addChild(colorLayer);

	ParentNode_Team= colorLayer;

	colorLayer->setPosition(ax::Vec2(winSize.width/2,winSize.height/2)); //
	//this->addChild(spriteBox);

	//获得背景图片的大小
	Size contentSize = colorLayer->getContentSize();
	m_size   = contentSize;
	m_origin = colorLayer->getPosition();
	//开启触摸响应

	this->scheduleUpdate();                   //调用函数
	 //禁止多点触摸
	return true;
}

void KuiSkilldesc::addDialogData()
{
	if  (!g_pCoreShell) return;

	if (skillData.uId<=0 || skillData.uId>=MAX_SKILL) return;

	KSkill * pTempSkill = NULL;
	KSkill * pCurSkill = NULL;
	KSkill * pNextSkill = NULL;
	ax::Color3B __ccBlue = {100,100,255};
	int  ulCurLevel = skillData.DataX;
	if(ulCurLevel == 0)
	{
		pNextSkill = (KSkill *)g_SkillManager.GetSkill(skillData.uId, 1);
		pTempSkill = pNextSkill;
	}
	else
	{
		pCurSkill = (KSkill *) g_SkillManager.GetSkill(skillData.uId, ulCurLevel);
		pNextSkill = (KSkill *) g_SkillManager.GetSkill(skillData.uId, ulCurLevel + 1);//下一等级
		pTempSkill = pCurSkill;
	}

	if (pTempSkill == NULL)
	{
		return;
	}

	//int  i;
	char pszMsg[128];
	ZeroMemory(&pszMsg,sizeof(pszMsg));
	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;
	Size nDsize;
	float offSet  = 0;
	nDsize.width  = m_size.width*2;
	nDsize.height = 0;
	int nTempOff =0;
//下个技能等级数据
    char nNextLevelData[640];
    ZeroMemory(nNextLevelData,sizeof(nNextLevelData));
    char *strnextlevel = nNextLevelData;
	bool bGetNextLevelDesc = true;
	if (skillData.uGenre != CGOG_SKILL_SHORTCUT && bGetNextLevelDesc)
	{
		if (pNextSkill)
		{
			strcat(strnextlevel, UTEXT("Cấp độ tiếp theo\n",1).c_str());
			if (pNextSkill->m_szDesc[0])
			{
				int newLen = TEncodeText_(pNextSkill->m_szDesc,strlen(pNextSkill->m_szDesc));
				pScriptLabel_n = Label::createWithTTF(UTEXT(pNextSkill->m_szDesc,1).c_str(),UI_GAME_FONT_DEFAULT, 12,nDsize,TextHAlignment::CENTER);//Arial
				pScriptLabel_n->setColor(ax::Color3B::RED);
				pScriptLabel_n->setAnchorPoint(ax::Vec2(0,0));
				pScriptLabel_n->setPosition(ax::Vec2(0,offSet));
				ParentNode_Team->addChild(pScriptLabel_n,1);
				offSet +=pScriptLabel_n->getContentSize().height+1;
			}
			pNextSkill->GetDescAboutLevel(strnextlevel);
			if  (strnextlevel[0])
			{
				Label *pNextLabel =NULL;
#ifdef WIN32
				pNextLabel = Label::createWithTTF(UTEXT(strnextlevel,1).c_str(),UI_GAME_FONT_DEFAULT, 12,nDsize,TextHAlignment::CENTER);//Arial
#else
				pNextLabel = Label::createWithTTF(strnextlevel,UI_GAME_FONT_DEFAULT, 12,nDsize,TextHAlignment::CENTER);//Arial
#endif
				pNextLabel->setAnchorPoint(ax::Vec2(0,0));
				pNextLabel->setPosition(ax::Vec2(0,offSet));
				pNextLabel->setColor(ax::Color3B::RED);
				ParentNode_Team->addChild(pNextLabel,1);
				offSet +=pNextLabel->getContentSize().height+1;
			}
		}
	}
    //技能限制
     char strLimited[128];
     ZeroMemory(strLimited,sizeof(strLimited));
	 strcat(strLimited,"Hạn chế: ");
	 char nSkillDesc[64]={0};
	 char nKey[8]={0};
	 int m_nEquiptLimited = pTempSkill->GetEquiptLimited();
	 if (m_nEquiptLimited==-2)
		 sprintf(nKey,"%s","F1");  //无限制
	 else if (m_nEquiptLimited==-1)
		 sprintf(nKey,"%s","F2");  //空手
	 else
		 sprintf(nKey,"%d",m_nEquiptLimited);

	 g_GameSetTing.GetString("WeaponLimit_m",nKey,"Không có",nSkillDesc,sizeof(nSkillDesc));
	 strcat(strLimited, nSkillDesc);
	 strcat(strLimited, "\n");
	 int  m_nHorseLimited = pTempSkill->GetHorseLimited();
	 if (m_nHorseLimited)  //骑马限制
	 {
		 switch(m_nHorseLimited)
		 {
		 case 1:
			 strcat(strLimited, "Không thể sử dụng kỹ năng này khi đang cưỡi ngựa\n");
			 break;
		 case 2:
			 strcat(strLimited, "Kỹ năng này chỉ có thể được sử dụng khi cưỡi ngựa\n");
			 break;
		 default:
			 break ;
		 }
	 }
	Label * pskilllitLabel = Label::createWithTTF(strLimited,UI_GAME_FONT_DEFAULT, 14,nDsize,TextHAlignment::CENTER);

	 pskilllitLabel->setAnchorPoint(ax::Vec2(0,0));
	 pskilllitLabel->setPosition(ax::Vec2(0,offSet));
	 pskilllitLabel->setColor(__ccBlue);
	 ParentNode_Team->addChild(pskilllitLabel,1);
	 offSet +=pskilllitLabel->getContentSize().height+1;
	//当前等级数据
    char ncurLevelData[640];
	ZeroMemory(ncurLevelData,sizeof(ncurLevelData));
	char *strlevel = ncurLevelData;
    if (pCurSkill)
    {//但前技能
		//脚本的说明
	   if (pCurSkill->m_szDesc[0])
	   {
		   int newLen = TEncodeText_(pCurSkill->m_szDesc,strlen(pCurSkill->m_szDesc));
			pScriptLabel = Label::createWithTTF(pCurSkill->m_szDesc,UI_GAME_FONT_DEFAULT, 14,nDsize,TextHAlignment::CENTER);
		    pScriptLabel->setAnchorPoint(ax::Vec2(0,0));
		    pScriptLabel->setPosition(ax::Vec2(0,offSet));
		    ParentNode_Team->addChild(pScriptLabel,1);
		    offSet +=pScriptLabel->getContentSize().height+1;

	   }
//------但前等级数据开始
	   pCurSkill->GetDescAboutLevel(strlevel);
	   if  (strlevel[0])
	   {
		   pbaseLabel = Label::createWithTTF(strlevel,UI_GAME_FONT_DEFAULT, 12,nDsize,TextHAlignment::CENTER);
		   pbaseLabel->setAnchorPoint(ax::Vec2(0,0));
		   pbaseLabel->setPosition(ax::Vec2(0,offSet));
		   ParentNode_Team->addChild(pbaseLabel,1);
		   offSet +=pbaseLabel->getContentSize().height+1;
	   }
    }
	//武功流派　 等级
	char szIntro[256],strSeries[128];
	ZeroMemory(&szIntro,sizeof(szIntro));
	ZeroMemory(&strSeries,sizeof(strSeries));
	nTempOff = 0;
	pdescLabel =Node::create();
	pdescLabel->setAnchorPoint(ax::Vec2(0,0));
	//prqLabel->setPosition(ax::Vec2(0,offSet));
	ParentNode_Team->addChild(pdescLabel,1);
	char szTemp[64];
	//if (skillData.uGenre != CGOG_SKILL_SHORTCUT)
	{
	  if (skillData.DataH)//如果有额外的技能点
		sprintf(szTemp, "cấp độ hiện tại:%d(%d+%d)",ulCurLevel , ulCurLevel - skillData.DataH,skillData.DataH);
	  else
		sprintf(szTemp, "cấp độ hiện tại:%d", ulCurLevel);
	  strcat(strSeries, szTemp);
	  strcat(strSeries, "\n");
	}

	if (pTempSkill->getUseAttackRate())
	{
		strcat(strSeries, "Đặc điểm kỹ năng：tỉ lệ chính xác");
		strcat(strSeries, "\n");
	}

	if (!_nKind && pTempSkill->getIsExpSkill())
	{//如果是 熟练度技能
		int  nCurExpSKill=0;
		//if (Npc[nOwnerIndex].m_SkillList.FindSame(ulSkillId))
		nCurExpSKill=Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurSkillExp(skillData.uId);

		int nper =0;
		int m_nSKillExp = pTempSkill->GetSkillExp();
		if (m_nSKillExp)
			nper= nCurExpSKill/(m_nSKillExp/100);

		sprintf(szTemp, "Trình độ hiện tại:%d%s", nper,"%");
		strcat(strSeries, szTemp);
		strcat(strSeries, "\n");
	}
	int  nEnChance = skillData.DataW;
	if (nEnChance)   //技能加成
	{
		sprintf(szTemp, "Tăng cường bởi các kỹ năng:+%d%s", nEnChance,"%");
		strcat(strSeries, szTemp);
		strcat(strSeries, "\n");
	}
	Label *tempLabel = Label::createWithTTF(UTEXT(strSeries,1).c_str(),UI_GAME_FONT_DEFAULT, 12,nDsize,TextHAlignment::CENTER);//Arial
	tempLabel->setAnchorPoint(ax::Vec2(0,0));
	nTempOff -=tempLabel->getContentSize().height+1;
	tempLabel->setPosition(ax::Vec2(0,nTempOff));
	tempLabel->setColor(ax::Color3B::GREEN);
	pdescLabel->addChild(tempLabel);
	//武功流派
	int  m_eSkillStyle = pTempSkill->GetSkillStyle();
	//sprintf(szIntro,Item[m_ItemData.uId].GetItmeInfo());
	if (m_eSkillStyle == SKILL_SS_Melee||m_eSkillStyle ==SKILL_SS_PassivityNpcState||m_eSkillStyle ==SKILL_SS_InitiativeNpcState|| m_eSkillStyle == SKILL_SS_Missles)
	{
		int nAttribId = pTempSkill->IsAttrib();  //武功流派
		if (nAttribId <=1)
		{
			//pTempSkill->GetSkillName();
			if (pTempSkill->IsMagic())  //1 为内功系  0为外攻系
				strcat(szIntro, "Đòn đánh thường không trang bị vũ khí");
			else
				strcat(szIntro, "Đòn đánh thường không trang bị vũ khí");
		}
		else
		{//技能描述
			char nSkey[6]={0},sDecsInfo[125]={0};
			sprintf(nSkey,"%d",nAttribId);
			g_GameSetTing.GetString("SkillAttrib",nSkey,"Không có dữ liệu",sDecsInfo,sizeof(sDecsInfo));
			strcat(szIntro, sDecsInfo);
		}
		strcat(szIntro,"\n");
	}

	if (szIntro[0])
	{

		sprintf(szIntro, "%s", UTEXT(szIntro,1).c_str());
		int newLen = TEncodeText(szIntro,strlen(szIntro));
		Label *tempLabel = Label::createWithTTF(szIntro,UI_GAME_FONT_DEFAULT, 12,nDsize,TextHAlignment::CENTER);
		tempLabel->setAnchorPoint(ax::Vec2(0,0));
		nTempOff -=tempLabel->getContentSize().height+1;
		tempLabel->setPosition(ax::Vec2(0,nTempOff));
		tempLabel->setColor(ax::Color3B::YELLOW);
		pdescLabel->addChild(tempLabel);
	}

	Size nTemp;
	nTemp.width  = m_size.width;
	nTemp.height = -nTempOff;
	pdescLabel->setContentSize(nTemp);
	pdescLabel->setPosition(ax::Vec2(0,nTemp.height+offSet));
	offSet +=pdescLabel->getContentSize().height+1;
	////技能的描述--------------------------------------------
	char otherzMsg[256];
	ZeroMemory(&otherzMsg,sizeof(otherzMsg));
	strcat(otherzMsg, pTempSkill->m_szSkillDesc);
	potherLabel = Label::createWithTTF(UTEXT(otherzMsg,1).c_str(),UI_GAME_FONT_DEFAULT, 12,nDsize,TextHAlignment::CENTER);//Arial
	potherLabel->setAnchorPoint(ax::Vec2(0,0));
	potherLabel->setPosition(ax::Vec2(0,offSet));
	ParentNode_Team->addChild(potherLabel,1);
	offSet +=potherLabel->getContentSize().height+1;
	//物品名称　等级
	ZeroMemory(&pszMsg,sizeof(pszMsg));
	sprintf(pszMsg, "%s", pTempSkill->GetSkillName());
    ptitleLabel = Label::createWithTTF(UTEXT(pszMsg,1).c_str(),UI_GAME_FONT_DEFAULT,12,nDsize,TextHAlignment::CENTER);//Arial
	ptitleLabel ->setColor(ax::Color3B::YELLOW);
	ptitleLabel->setAnchorPoint(ax::Vec2(0,0));
	ptitleLabel->setPosition(ax::Vec2(0,offSet));
	ParentNode_Team->addChild(ptitleLabel,1);

    m_size.height = offSet+ptitleLabel->getContentSize().height+1;
	ParentNode_Team->setContentSize(m_size);
	//移动整层的位置
	if (skillData.nRect.origin.y+m_size.height>visibleSize.height) //顶部
	{//如果超过了可见范围
		skillData.nRect.origin.y = visibleSize.height-m_size.height-10;
	}
	ParentNode_Team->setPosition(ax::Vec2(skillData.nRect.origin.x-m_size.width,skillData.nRect.origin.y));
	//创建一个动作 改变精灵颜色
	red = TintBy::create(0.2,0,-255,-255); //持续时间+颜色
	red->retain();
}
//设置确定取消按钮,参数：调用层对象，调用层回调函数
void KuiSkilldesc::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	return;

	Sprite * sprite_tips_btn_confirm_normal =  Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_normal);
	//SpriteFrame * f_tips_btn_confirm_select = sfCache->spriteFrameByName("btn_confirm_select.png");
	Sprite * sprite_tips_btn_confirm_select =  Sprite::create("close.png");//Sprite::createWithSpriteFrame(f_tips_btn_confirm_select);
	MenuItemSprite * itemConfirm = MenuItemSprite::create(sprite_tips_btn_confirm_normal, sprite_tips_btn_confirm_select, CC_CALLBACK_1(KuiSkilldesc::buttonCallBackFunc, this));
	itemConfirm->setPosition(ax::Vec2(0,0));
	Point bgSpritePoint = ParentNode_Team->getPosition();
	Menu * menu = Menu::create(itemConfirm,NULL);
	//menu->setPosition(ax::Vec2(bgSpritePoint.x-m_size.width/2,bgSpritePoint.y-m_size.height/2));//菜单位置设置为弹出框左下
	menu->setPosition(ax::Vec2(m_size.width/2,45));//菜单位置设置为弹出框左下
	ParentNode_Team->addChild(menu);          //加到父层里
}

//执行上层对象的回调函数，关闭弹出框
void KuiSkilldesc::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node = dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//设置tag，在调用层可以获取到
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);
	this->removeFromParentAndCleanup(true);
}

//关闭弹出框
void KuiSkilldesc::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}


//重写触摸注册函数，重新给定触摸级别
void KuiSkilldesc::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//这里的触摸优先级设置为－128，与CCMenu同级，保证了屏蔽下方的触摸
//	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
}

Rect KuiSkilldesc::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //这个因该是图内偏移
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //起点最左边
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //最上边    //- rc.origin.y

	return rc;
}
//触摸函数ccTouchBegan，返回true
bool KuiSkilldesc::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(NULL);
	this->removeFromParentAndCleanup(true);

	return true;
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point location = pTouch->getLocation();//目的的坐标?屏幕点击处
	return true;
}

void KuiSkilldesc::update(float delta)
{
	//messageBox("无限循环","update");
	/*if (isOpen && g_pCoreShell)
	{
		if (ptitleLabel)
		{

		}

	    //m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
		//m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
	}*/
}
