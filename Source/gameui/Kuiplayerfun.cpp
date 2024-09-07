#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "Kuiplayerfun.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
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
#include "gamecore/KSkillManager.h"
#include "gamecore/KItem.h"
#include "engine/Text.h"
#include "gamecore/ImgRef.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;
#define  MSG_MAX_COUNT 100
////�ؼ���
static char s_keys [][__X_NAME_LEN_] =
{
	"Team#",
	"Faction#",
	"Tong#",
	"\\S",
	"Msgr#",
	"GM",
	"TongUnion#",
	"",
	"",
	"",
	"",

};
Kuiplayerfun::Kuiplayerfun()
{
	isOpen = false;
	__isCanMove=true;
	m_bScrolling = false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	//pMoneyLabel       = NULL;
	colorLayer        = NULL;
	red               = NULL;
	m_pUiLayer = NULL;
	//m_pWidget  = NULL;
	//m_array    = NULL;
	pNearbyPlayerslistView = NULL;
	pListView_all= NULL;
	pListView_shijie= NULL;
	pListView_chengshi= NULL;
	pListView_duiwu= NULL;
	pListView_siliao= NULL;
	pListView_bangpai= NULL;
	m_pNearbyPlayersList   = NULL;
	m_pPlayersList       = NULL;
	__nSelIndex          = -1;
	__playrSelIndex      = -1;
	__playerListCount    = 0;
	checkBox =NULL;
	pfindEditBox =NULL;
	btnPageBox_q =NULL;//ȫ��
	btnPageBox_s =NULL;//����
	btnPageBox_c =NULL;//����
	btnPageBox_d =NULL;//����
	btnPageBox_sl=NULL;//˽��
	btnPageBox_m =NULL;//����
	btnPageBox_b =NULL;//����
	m_pActivateChannel=NULL;
	m_nActivateChannels = 0;
	m_nChannelsResource = 0;
    btnSize = Vec2(0, 0);
}

Kuiplayerfun::~Kuiplayerfun()
{
	isOpen = false;
	//pMoneyLabel  = NULL;
	colorLayer   = NULL;
	__nSelIndex  = -1;
	__NearbyListCount = 0;
	if (m_pNearbyPlayersList)
	{
		free(m_pNearbyPlayersList);
		m_pNearbyPlayersList =NULL;
	}

	if (m_pPlayersList)
	{
		free(m_pPlayersList);
		m_pPlayersList =NULL;
	}

	//CC_SAFE_RELEASE(m_array);
	if (red)
	    red->release();

	ReleaseActivateChannelAll();

}

//����ȷ��������
Kuiplayerfun * Kuiplayerfun::create(Ref * callbackListener, const std::function<void(ax::Node*)>& callfun)
{
	auto popLayer = Kuiplayerfun::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool Kuiplayerfun::init()
{
	if ( !Layer::init() ){
		return false;
	}
	//����֡����
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");
	//���ģ̬����
	winSize = ax::Director::getInstance()->getWinSize();
	/*Color4B color(112, 128, 144, 150);
	LayerColor * colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	//colorLayer->setContentSize(winSize);//���úʹ��ڵĴ�С
	this->addChild(colorLayer);
	*/
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
//	sprintf(nSprName,"\\Spr\\Ui3\\锟斤拷医锟斤拷锟絓\\锟斤拷医锟斤拷锟?.spr");
	sprintf(nSprName,"\\spr\\Ui4\\KgameWorld\\btn_BaoVat.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur == NULL)
		return false;
	Sprite * spriteBox = Sprite::createWithTexture(bgCur);
	ParentNode_Team= spriteBox;
	spriteBox->setPosition(ax::Vec2(winSize.width/2,winSize.height/2));
	this->addChild(spriteBox);
	//��ñ���ͼƬ�Ĵ�С
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();
	bgSize  = ax::Size(265,158);
	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С
	//����������Ӧ

	this->scheduleUpdate();                   //���ú���
//	auto listener = ax::EventListenerTouchOneByOne::create();
//    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this); //��ֹ��㴥��
	return true;
}

void Kuiplayerfun::addDialogData()
{
	if  (!g_pCoreShell) return;

	btnSize.width = 70;
	btnSize.height= 14;

	/*pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//ͳ˧
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	pMoneyLabel->setPosition(ax::Vec2(70,35));
	ParentNode_Team->addChild(pMoneyLabel,1);*/
	//����ϵͳ����
	//SetAutoDelMsgInterval(SECOND_AUTODELMSG); //�����Զ�ɾ����Ϣ��ʱ������Ĭ��ֵΪ0����ʾ������ʱ�����Զ�ɾ��
	//����Ƶ����Դ
	int nCh;
	KIniFile pIni;
	if  (!pIni.Load("\\Ui\\Ui3\\消息集合面板_左.ini"))
	{
#ifdef WIN32
		messageBox("װ����Ϣʧ��","��ʾ");
#else
		messageBox(UTEXT("装载信息失败",1).c_str(),UTEXT("提示",1).c_str());
#endif
	}
	char ChName[64];
	ZeroMemory(ChName,sizeof(ChName));
	for (nCh = 0; nCh < MAX_CHANNELRESOURCE; nCh++)
	{
		sprintf(ChName, "%s%d", "Channel", nCh);
		if (pIni.GetString("Channels", ChName, "", m_ChannelsResource[nCh].cTitle, 32))
		{
			//messageBox(m_ChannelsResource[nCh].cTitle,"TITLE");
			pIni.GetString(m_ChannelsResource[nCh].cTitle, "TextColor", "0,0,0", ChName, sizeof(ChName));
			m_ChannelsResource[nCh].uTextColor.Color_dw = TGetColor(ChName);
			m_ChannelsResource[nCh].uTextColor.Color_b.a = KTC_COLOR;

			pIni.GetString(m_ChannelsResource[nCh].cTitle, "TextBorderColor", "0,0,0", ChName, sizeof(ChName));
			m_ChannelsResource[nCh].uTextBorderColor.Color_dw = TGetColor(ChName);
			m_ChannelsResource[nCh].uTextBorderColor.Color_b.a = KTC_BORDER_COLOR;  //���ֱ�Ե��ɫ

			for (int m = 0; m < MAX_CHANNELRESOURCE; m++)
			{
				m_ChannelsResource[nCh].cShortTitle[m][0] = 0;
				sprintf(ChName, "%s%d", "ShortName", m); //������,�޸�
				pIni.GetString(m_ChannelsResource[nCh].cTitle, ChName, "", m_ChannelsResource[nCh].cShortTitle[m], sizeof(m_ChannelsResource[nCh].cShortTitle[m]));  //Ƶ�����
			}

			pIni.GetString(m_ChannelsResource[nCh].cTitle, "FormatName", "", m_ChannelsResource[nCh].cFormatName, sizeof(m_ChannelsResource[nCh].cFormatName)); //Ƶ����ʽ����

			pIni.GetString(m_ChannelsResource[nCh].cTitle, "MenuText", "", m_ChannelsResource[nCh].cMenuText, sizeof(m_ChannelsResource[nCh].cMenuText));
			pIni.GetString(m_ChannelsResource[nCh].cTitle, "DeactivateMenuText", m_ChannelsResource[nCh].cMenuText, m_ChannelsResource[nCh].cMenuDeactivateText, sizeof(m_ChannelsResource[nCh].cMenuDeactivateText));
			m_ChannelsResource[nCh].nMenuPicIndex = -1;
			//if (pIni.GetString(m_ChannelsResource[nCh].cTitle, "MenuImage", "", szImage, MAX_PATH))
			//{
			//}
			m_ChannelsResource[nCh].nMenuDeactivatePicIndex = m_ChannelsResource[nCh].nMenuPicIndex;
			m_ChannelsResource[nCh].nMenuDeactivatePicHeight = m_ChannelsResource[nCh].nMenuPicHeight;
			//if (pIni.GetString(m_ChannelsResource[nCh].cTitle, "DeactivateMenuImage", "", szImage, MAX_PATH))
			//{
			//}
			m_ChannelsResource[nCh].nTextPicIndex = -1;
			//if (pIni.GetString(m_ChannelsResource[nCh].cTitle, "TextImage", "", szImage, MAX_PATH))
			//{

			//}
			pIni.GetString(m_ChannelsResource[nCh].cTitle, "MenuBkColor", "0,0,0", ChName, sizeof(ChName));
			m_ChannelsResource[nCh].uMenuBkColor.Color_dw = TGetColor(ChName);
			m_ChannelsResource[nCh].uMenuBkColor.Color_b.a = 0;

			pIni.GetInteger(m_ChannelsResource[nCh].cTitle, "NeverClose", 0, &m_ChannelsResource[nCh].nNeverClose); //�Ƿ����û�йرգ�----����
			pIni.GetString(m_ChannelsResource[nCh].cTitle, "Sound", "", m_ChannelsResource[nCh].szSoundFileName, sizeof(m_ChannelsResource[nCh].szSoundFileName));

			pIni.GetInteger(m_ChannelsResource[nCh].cTitle, "SendMsgInterval", 0, (int*)(&m_ChannelsResource[nCh].uSendMsgInterval));
			pIni.GetInteger(m_ChannelsResource[nCh].cTitle, "SendMsgNum", 0, (int*)(&m_ChannelsResource[nCh].uSendMsgNum));

			m_ChannelsResource[nCh].uSendMsgNum++;

			if (m_ChannelsResource[nCh].uSendMsgNum > 10)
				m_ChannelsResource[nCh].uSendMsgNum = 10;
			if (m_ChannelsResource[nCh].uSendMsgNum <= 0)
				m_ChannelsResource[nCh].uSendMsgNum = 1;
		}
		else
			break;
	}
	if (nCh < MAX_CHANNELRESOURCE)
	{
		m_ChannelsResource[nCh].cTitle[0] = 0;
		for (int m = 0; m < MAX_CHANNELRESOURCE; m++)
		{
			m_ChannelsResource[nCh].cShortTitle[m][0] = 0;
		}
	}
	m_nChannelsResource = nCh;

	m_nDefaultChannelResource = -1;
	if (pIni.GetString("Channels", "DefaultChannel", "", ChName, 32)) //ϵͳ  ��ɫ��Ϣ
	{
		m_nDefaultChannelResource = FindChannelResourceIndex(ChName);
	}

	pIni.GetString("Channels", "DefaultChannelSendName", "ϵͳ", m_DefaultChannelSendName, 32);  //��ɫ��ʾ��Ϣ

	pIni.Clear();
	//����
	float nTempFontSize =14;
	m_pUiLayer = Layer::create();
	ParentNode_Team->addChild(m_pUiLayer);

	// Create the checkbox
	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	ZeroMemory(nSprName,sizeof(nSprName));
	//
	sprintf(nSprName,"\\spr\\Ui3\\����\\����������\\ͨ�õĶ��ֱ�ǩ.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Texture2D * check_btn_normal = bgCur;
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Texture2D * check_btn_active = bgCur;

	btnPageBox_q = Button::create();
	btnPageBox_q->setTouchEnabled(true);
//	btnPageBox_q->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_q->setTitleFontSize(nTempFontSize);
	btnPageBox_q->setTitleText(UTEXT("ȫ��",1).c_str());
	btnPageBox_q->setPosition(ax::Vec2(40,m_size.height-31));
	btnPageBox_q->setTag(1);
	//btnPageBox[i]->setTag("1");
	btnPageBox_q->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_q);

	btnPageBox_s = Button::create();
	btnPageBox_s->setTouchEnabled(true);
//	btnPageBox_s->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_s->setTitleFontSize(nTempFontSize);
	btnPageBox_s->setTitleText(UTEXT("����",1).c_str());
	btnPageBox_s->setPosition(ax::Vec2(40+check_btn_normal->getContentSize().width+2,m_size.height-31));
	btnPageBox_s->setTag(2);
	//btnPageBox[i]->setTag("1");
	btnPageBox_s->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_s);

	btnPageBox_c = Button::create();
	btnPageBox_c->setTouchEnabled(true);
//	btnPageBox_c->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_c->setTitleFontSize(nTempFontSize);
	btnPageBox_c->setTitleText(UTEXT("����",1).c_str());
	btnPageBox_c->setPosition(ax::Vec2(40+(check_btn_normal->getContentSize().width+2)*2,m_size.height-31));
	btnPageBox_c->setTag(3);
	//btnPageBox[i]->setTag("1");
	btnPageBox_c->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_c);

	btnPageBox_d = Button::create();
	btnPageBox_d->setTouchEnabled(true);
//	btnPageBox_d->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_d->setTitleFontSize(nTempFontSize);
	btnPageBox_d->setTitleText(UTEXT("����",1).c_str());
	btnPageBox_d->setPosition(ax::Vec2(40+(check_btn_normal->getContentSize().width+2)*3,m_size.height-31));
	btnPageBox_d->setTag(4);
	//btnPageBox[i]->setTag("1");
	btnPageBox_d->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_d);

	btnPageBox_sl = Button::create();
	btnPageBox_sl->setTouchEnabled(true);
//	btnPageBox_sl->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_sl->setTitleFontSize(nTempFontSize);
	btnPageBox_sl->setTitleText(UTEXT("˽��",1).c_str());
	btnPageBox_sl->setPosition(ax::Vec2(40+(check_btn_normal->getContentSize().width+2)*4,m_size.height-31));
	btnPageBox_sl->setTag(5);
	//btnPageBox[i]->setTag("1");
	btnPageBox_sl->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_sl);

	btnPageBox_b = Button::create();
	btnPageBox_b->setTouchEnabled(true);
//	btnPageBox_b->loadTexturesByData(check_btn_normal,check_btn_active,check_btn_normal);
	btnPageBox_b->setTitleFontSize(nTempFontSize);
	btnPageBox_b->setTitleText(UTEXT("����",1).c_str());
	btnPageBox_b->setPosition(ax::Vec2(40+(check_btn_normal->getContentSize().width+2)*5,m_size.height-31));
	btnPageBox_b->setTag(6);
	//btnPageBox[i]->setTag("1");
	btnPageBox_b->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::touchPageBtnEvent, this));
	m_pUiLayer->addChild(btnPageBox_b);

	pListView_all = ListView::create();
	// set list view direction
	pListView_all->setDirection(ListView::Direction::VERTICAL);
	pListView_all->setBackGroundImage("ui/btn_sel.png");
	pListView_all->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pListView_all->setBackGroundImageOpacity(200);
	pListView_all->setContentSize(ax::Size(265,158));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	pListView_all->setPosition(ax::Vec2(20,m_size.height-200));
	pListView_all->setTouchEnabled(true);
	pListView_all->setBounceEnabled(true);
	pListView_all->addEventListener(CC_CALLBACK_2(Kuiplayerfun::selectedPlayersListItemEvent, this));
	pListView_all->setTag(CH_ALL_LIST);
	m_pUiLayer->addChild(pListView_all);
	//UpdateData(NULL);
	pListView_all->setGravity(ListView::Gravity::CENTER_VERTICAL);
	pListView_all->forceDoLayout();
	pListView_all->jumpToBottom();


	pListView_shijie = ListView::create();
	// set list view direction
	pListView_shijie->setDirection(ListView::Direction::VERTICAL);
	pListView_shijie->setTouchEnabled(false);
	pListView_shijie->setBounceEnabled(false);
	pListView_shijie->setBackGroundImage("ui/btn_sel.png");
	pListView_shijie->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pListView_shijie->setBackGroundImageOpacity(200);
	pListView_shijie->setContentSize(ax::Size(265,158));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	pListView_shijie->setPosition(ax::Vec2(20,m_size.height-200));
	pListView_shijie->addEventListener(CC_CALLBACK_2(Kuiplayerfun::selectedPlayersListItemEvent, this));
	pListView_shijie->setTag(CH_SHIJIE_LIST);
	m_pUiLayer->addChild(pListView_shijie);
	//UpdateData(NULL);
	pListView_shijie->setGravity(ListView::Gravity::CENTER_VERTICAL);
	pListView_shijie->forceDoLayout();
	pListView_shijie->jumpToBottom();
	pListView_shijie->setVisible(false);

	pListView_chengshi = ListView::create();
	// set list view direction
	pListView_chengshi->setDirection(ListView::Direction::VERTICAL);
	pListView_chengshi->setTouchEnabled(false);
	pListView_chengshi->setBounceEnabled(false);
	pListView_chengshi->setBackGroundImage("ui/btn_sel.png");
	pListView_chengshi->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pListView_chengshi->setBackGroundImageOpacity(200);
	pListView_chengshi->setContentSize(ax::Size(265,158));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	pListView_chengshi->setPosition(ax::Vec2(20,m_size.height-200));
	pListView_chengshi->addEventListener(CC_CALLBACK_2(Kuiplayerfun::selectedPlayersListItemEvent, this));
	pListView_chengshi->setTag(CH_CHENGSHI_LIST);
	m_pUiLayer->addChild(pListView_chengshi);
	//UpdateData(NULL);
	pListView_chengshi->setGravity(ListView::Gravity::CENTER_VERTICAL);
	pListView_chengshi->forceDoLayout();
	pListView_chengshi->jumpToBottom();
	pListView_chengshi->setVisible(false);

	pListView_duiwu = ListView::create();
	// set list view direction
	pListView_duiwu->setDirection(ListView::Direction::VERTICAL);
	pListView_duiwu->setTouchEnabled(false);
	pListView_duiwu->setBounceEnabled(false);
	pListView_duiwu->setBackGroundImage("ui/btn_sel.png");
	pListView_duiwu->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pListView_duiwu->setBackGroundImageOpacity(200);
	pListView_duiwu->setContentSize(ax::Size(265,158));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	pListView_duiwu->setPosition(ax::Vec2(20,m_size.height-200));
	pListView_duiwu->addEventListener(CC_CALLBACK_2(Kuiplayerfun::selectedPlayersListItemEvent, this));
	pListView_duiwu->setTag(CH_DUIWU_LIST);
	m_pUiLayer->addChild(pListView_duiwu);
	//UpdateData(NULL);
	pListView_duiwu->setGravity(ListView::Gravity::CENTER_VERTICAL);
	pListView_duiwu->forceDoLayout();
	pListView_duiwu->jumpToBottom();
	pListView_duiwu->setVisible(false);

	pListView_siliao = ListView::create();
	// set list view direction
	pListView_siliao->setDirection(ListView::Direction::VERTICAL);
	pListView_siliao->setTouchEnabled(false);
	pListView_siliao->setBounceEnabled(false);
	pListView_siliao->setBackGroundImage("ui/btn_sel.png");
	pListView_siliao->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pListView_siliao->setBackGroundImageOpacity(200);
	pListView_siliao->setContentSize(ax::Size(265,158));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	pListView_siliao->setPosition(ax::Vec2(20,m_size.height-200));
	pListView_siliao->addEventListener(CC_CALLBACK_2(Kuiplayerfun::selectedPlayersListItemEvent, this));
	pListView_siliao->setTag(CH_SILIAO_LIST);
	m_pUiLayer->addChild(pListView_siliao);
	//UpdateData(NULL);
	pListView_siliao->setGravity(ListView::Gravity::CENTER_VERTICAL);
	pListView_siliao->forceDoLayout();
	pListView_siliao->jumpToBottom();
	pListView_siliao->setVisible(false);

	pListView_bangpai = ListView::create();
	// set list view direction
	pListView_bangpai->setDirection(ListView::Direction::VERTICAL);
	pListView_bangpai->setTouchEnabled(false);
	pListView_bangpai->setBounceEnabled(false);
	pListView_bangpai->setBackGroundImage("ui/btn_sel.png");
	pListView_bangpai->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pListView_bangpai->setBackGroundImageOpacity(200);
	pListView_bangpai->setContentSize(ax::Size(265,158));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	pListView_bangpai->setPosition(ax::Vec2(20,m_size.height-200));
	pListView_bangpai->addEventListener(CC_CALLBACK_2(Kuiplayerfun::selectedPlayersListItemEvent, this));
	pListView_bangpai->setTag(CH_BANGPAI_LIST);
	m_pUiLayer->addChild(pListView_bangpai);
	//UpdateData(NULL);
	pListView_bangpai->setGravity(ListView::Gravity::CENTER_VERTICAL);
	pListView_bangpai->forceDoLayout();
	pListView_bangpai->jumpToBottom();
	pListView_bangpai->setVisible(false);

	//��Χ���
	pNearbyPlayerslistView = ListView::create();
	// set list view direction
	pNearbyPlayerslistView->setDirection(ListView::Direction::VERTICAL);
	pNearbyPlayerslistView->setTouchEnabled(true);
	pNearbyPlayerslistView->setBounceEnabled(true);
	pNearbyPlayerslistView->setBackGroundImage("loadingStart.png");
	pNearbyPlayerslistView->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pNearbyPlayerslistView->setBackGroundImageOpacity(50);
	pNearbyPlayerslistView->setContentSize(ax::Size(110, 178));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	pNearbyPlayerslistView->setPosition(ax::Vec2(m_size.width/2+80,m_size.height-200));
	pNearbyPlayerslistView->addEventListener(CC_CALLBACK_2(Kuiplayerfun::selectedItemEvent, this));
	m_pUiLayer->addChild(pNearbyPlayerslistView);
	OnRefresh();
	pNearbyPlayerslistView->setGravity(ListView::Gravity::CENTER_VERTICAL);
	pNearbyPlayerslistView->forceDoLayout();
	pNearbyPlayerslistView->jumpToBottom();


	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);  //���úʹ��ڵĴ�С
	ParentNode_Team->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	//red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	//red->retain();
}


void Kuiplayerfun::touchPageBtnEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type)
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

				switch(nTbtn->getTag())
				{
				case 1:
					{//ȫ��Ƶ��
						g_GameWorld->setchatCurChannel("CH_NEARBY");
						pListView_all->setTouchEnabled(true);
						pListView_all->setBounceEnabled(true);
						pListView_all->setVisible(true);
						setBtnTouchEnabled(pListView_all,true);

						pListView_siliao->setTouchEnabled(false);
						pListView_siliao->setBounceEnabled(false);
						pListView_siliao->setVisible(false);
						setBtnTouchEnabled(pListView_siliao,false);

						pListView_duiwu->setTouchEnabled(false);
						pListView_duiwu->setBounceEnabled(false);
						pListView_duiwu->setVisible(false);
						setBtnTouchEnabled(pListView_duiwu,false);

						pListView_chengshi->setTouchEnabled(false);
						pListView_chengshi->setBounceEnabled(false);
						pListView_chengshi->setVisible(false);
						setBtnTouchEnabled(pListView_chengshi,false);

						pListView_shijie->setTouchEnabled(false);
						pListView_shijie->setBounceEnabled(false);
						pListView_shijie->setVisible(false);
						setBtnTouchEnabled(pListView_shijie,false);

						pListView_bangpai->setTouchEnabled(false);
						pListView_bangpai->setBounceEnabled(false);
						pListView_bangpai->setVisible(false);
						setBtnTouchEnabled(pListView_bangpai,false);
					}
					break;
				case 2:
					{//����Ƶ��
						g_GameWorld->setchatCurChannel("CH_WORLD");
						pListView_all->setTouchEnabled(false);
						pListView_all->setBounceEnabled(false);
						pListView_all->setVisible(false);
						setBtnTouchEnabled(pListView_all,false);

						pListView_siliao->setTouchEnabled(false);
						pListView_siliao->setBounceEnabled(false);
						pListView_siliao->setVisible(false);
						setBtnTouchEnabled(pListView_siliao,false);

						pListView_duiwu->setTouchEnabled(false);
						pListView_duiwu->setBounceEnabled(false);
						pListView_duiwu->setVisible(false);
						setBtnTouchEnabled(pListView_duiwu,false);

						pListView_chengshi->setTouchEnabled(false);
						pListView_chengshi->setBounceEnabled(false);
						pListView_chengshi->setVisible(false);
						setBtnTouchEnabled(pListView_chengshi,false);

						pListView_shijie->setTouchEnabled(true);
						pListView_shijie->setBounceEnabled(true);
						pListView_shijie->setVisible(true);
						setBtnTouchEnabled(pListView_shijie,true);

						pListView_bangpai->setTouchEnabled(false);
						pListView_bangpai->setBounceEnabled(false);
						pListView_bangpai->setVisible(false);
						setBtnTouchEnabled(pListView_bangpai,false);
					}
					break;
				case 3:
					{//����Ƶ��
						g_GameWorld->setchatCurChannel("CH_CITY");

						pListView_all->setTouchEnabled(false);
						pListView_all->setBounceEnabled(false);
						pListView_all->setVisible(false);
						setBtnTouchEnabled(pListView_all,false);

						pListView_siliao->setTouchEnabled(false);
						pListView_siliao->setBounceEnabled(false);
						pListView_siliao->setVisible(false);
						setBtnTouchEnabled(pListView_siliao,false);

						pListView_duiwu->setTouchEnabled(false);
						pListView_duiwu->setBounceEnabled(false);
						pListView_duiwu->setVisible(false);
						setBtnTouchEnabled(pListView_duiwu,false);

						pListView_chengshi->setTouchEnabled(true);
						pListView_chengshi->setBounceEnabled(true);
						pListView_chengshi->setVisible(true);
						setBtnTouchEnabled(pListView_chengshi,true);

						pListView_shijie->setTouchEnabled(false);
						pListView_shijie->setBounceEnabled(false);
						pListView_shijie->setVisible(false);
						setBtnTouchEnabled(pListView_shijie,false);

						pListView_bangpai->setTouchEnabled(false);
						pListView_bangpai->setBounceEnabled(false);
						pListView_bangpai->setVisible(false);
						setBtnTouchEnabled(pListView_bangpai,false);
					}
					break;
				case 4:
					{//����Ƶ��
						g_GameWorld->setchatCurChannel("CH_TEAM");

						pListView_all->setTouchEnabled(false);
						pListView_all->setBounceEnabled(false);
						pListView_all->setVisible(false);
						setBtnTouchEnabled(pListView_all,false);

						pListView_siliao->setTouchEnabled(false);
						pListView_siliao->setBounceEnabled(false);
						pListView_siliao->setVisible(false);
						setBtnTouchEnabled(pListView_siliao,false);

						pListView_duiwu->setTouchEnabled(true);
						pListView_duiwu->setBounceEnabled(true);
						pListView_duiwu->setVisible(true);
						setBtnTouchEnabled(pListView_duiwu,true);

						pListView_chengshi->setTouchEnabled(false);
						pListView_chengshi->setBounceEnabled(false);
						pListView_chengshi->setVisible(false);
						setBtnTouchEnabled(pListView_chengshi,false);

						pListView_shijie->setTouchEnabled(false);
						pListView_shijie->setBounceEnabled(false);
						pListView_shijie->setVisible(false);
						setBtnTouchEnabled(pListView_shijie,false);

						pListView_bangpai->setTouchEnabled(false);
						pListView_bangpai->setBounceEnabled(false);
						pListView_bangpai->setVisible(false);
						setBtnTouchEnabled(pListView_bangpai,false);
					}
					break;
				case 5:
					{//˽��Ƶ��
						g_GameWorld->setchatCurChannel("CH_JABBER");

						pListView_all->setTouchEnabled(false);
						pListView_all->setBounceEnabled(false);
						pListView_all->setVisible(false);
						setBtnTouchEnabled(pListView_all,false);

						pListView_siliao->setTouchEnabled(true);
						pListView_siliao->setBounceEnabled(true);
						pListView_siliao->setVisible(true);
						setBtnTouchEnabled(pListView_siliao,true);

						pListView_duiwu->setTouchEnabled(false);
						pListView_duiwu->setBounceEnabled(false);
						pListView_duiwu->setVisible(false);
						setBtnTouchEnabled(pListView_duiwu,false);

						pListView_chengshi->setTouchEnabled(false);
						pListView_chengshi->setBounceEnabled(false);
						pListView_chengshi->setVisible(false);
						setBtnTouchEnabled(pListView_chengshi,false);

						pListView_shijie->setTouchEnabled(false);
						pListView_shijie->setBounceEnabled(false);
						pListView_shijie->setVisible(false);
						setBtnTouchEnabled(pListView_shijie,false);

						pListView_bangpai->setTouchEnabled(false);
						pListView_bangpai->setBounceEnabled(false);
						pListView_bangpai->setVisible(false);
						setBtnTouchEnabled(pListView_bangpai,false);
					}
					break;
				case 6:
					{//����Ƶ�� \Spr\Ui3\������\�����水ť-����Ƶ��ѡ��.spr
						g_GameWorld->setchatCurChannel("CH_TONG");
						pListView_all->setTouchEnabled(false);
						pListView_all->setBounceEnabled(false);
						pListView_all->setVisible(false);
						setBtnTouchEnabled(pListView_all,false);

						pListView_siliao->setTouchEnabled(false);
						pListView_siliao->setBounceEnabled(false);
						pListView_siliao->setVisible(false);
						setBtnTouchEnabled(pListView_siliao,false);

						pListView_duiwu->setTouchEnabled(false);
						pListView_duiwu->setBounceEnabled(false);
						pListView_duiwu->setVisible(false);
						setBtnTouchEnabled(pListView_duiwu,false);

						pListView_chengshi->setTouchEnabled(false);
						pListView_chengshi->setBounceEnabled(false);
						pListView_chengshi->setVisible(false);
						setBtnTouchEnabled(pListView_chengshi,false);

						pListView_shijie->setTouchEnabled(false);
						pListView_shijie->setBounceEnabled(false);
						pListView_shijie->setVisible(false);
						setBtnTouchEnabled(pListView_shijie,false);

						pListView_bangpai->setTouchEnabled(true);
						pListView_bangpai->setBounceEnabled(true);
						pListView_bangpai->setVisible(true);
						setBtnTouchEnabled(pListView_bangpai,true);
					}
					break;
				case 7:
					{//����
						g_GameWorld->setchatCurChannel("CH_FACTION");

						pListView_all->setTouchEnabled(false);
						pListView_all->setBounceEnabled(false);
						pListView_all->setVisible(false);
						setBtnTouchEnabled(pListView_all,false);

						pListView_siliao->setTouchEnabled(false);
						pListView_siliao->setBounceEnabled(false);
						pListView_siliao->setVisible(false);
						setBtnTouchEnabled(pListView_siliao,false);

						pListView_duiwu->setTouchEnabled(false);
						pListView_duiwu->setBounceEnabled(false);
						pListView_duiwu->setVisible(false);
						setBtnTouchEnabled(pListView_duiwu,false);

						pListView_chengshi->setTouchEnabled(false);
						pListView_chengshi->setBounceEnabled(false);
						pListView_chengshi->setVisible(false);
						setBtnTouchEnabled(pListView_chengshi,false);

						pListView_shijie->setTouchEnabled(false);
						pListView_shijie->setBounceEnabled(false);
						pListView_shijie->setVisible(false);
						setBtnTouchEnabled(pListView_shijie,false);

						pListView_bangpai->setTouchEnabled(false);
						pListView_bangpai->setBounceEnabled(false);
						pListView_bangpai->setVisible(false);
						setBtnTouchEnabled(pListView_bangpai,false);
					}
					break;
				default:
					break;
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
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void Kuiplayerfun::setcoloseButton(Ref * callbackListener, const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui3\\��ҽ���\\��ҽ������ر�.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_colse_normal = Sprite::createWithTexture(bgCur);
	sprite_colse_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_colse_select = Sprite::createWithTexture(bgCur);
	sprite_colse_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * colseConfirm = MenuItemSprite::create(sprite_colse_normal, sprite_colse_select, CC_CALLBACK_1(Kuiplayerfun::buttonCallBackFunc, this));
	colseConfirm->setPosition(ax::Vec2(m_size.width/2+sprite_colse_normal->getContentSize().width/2-8,sprite_colse_select->getContentSize().height/2+4));

	//ˢ��  \Spr\Ui3\��ҽ���\��ҽ�����ˢ��.spr  \\Spr\\Ui3\\���\\ˢ���б�.spr
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\Spr\\Ui3\\��ҽ���\\��ҽ�����ˢ��.spr");
	g_StrLower(nSprName);

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Refresh_normal = Sprite::createWithTexture(bgCur);
	sprite_Refresh_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Refresh_select = Sprite::createWithTexture(bgCur);
	sprite_Refresh_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * RefreshConfirm = MenuItemSprite::create(sprite_Refresh_normal, sprite_Refresh_select, CC_CALLBACK_1(Kuiplayerfun::btnCallBackFunc, this));
	RefreshConfirm->setPosition(ax::Vec2(sprite_Refresh_select->getContentSize().width/2+28,sprite_Refresh_select->getContentSize().height/2+4));
	RefreshConfirm->setTag(4);

	//����˵�
	Menu * menu = Menu::create(colseConfirm,RefreshConfirm,NULL);
	//menu->setPosition(ax::Vec2(bgSpritePoint.x-m_size.width/2,bgSpritePoint.y-m_size.height/2));//�˵�λ������Ϊ����������
	menu->setAnchorPoint(ax::Vec2(0,0));
	menu->setPosition(ax::Vec2(0,0));             //�˵�λ������Ϊ����������
	ParentNode_Team->addChild(menu);         //�ӵ�������


	ax::ui::Scale9Sprite *pBg = ax::ui::Scale9Sprite::create(); //"ui/editbg.png"
	pfindEditBox = EditBox::create(Size(110,24),pBg);
	if (pfindEditBox)
	{
		pfindEditBox->setPlaceHolder(UTEXT("�������",1).c_str());
		pfindEditBox->setFontColor(ax::Color3B::YELLOW);
		pfindEditBox->setFontSize(24);
		pfindEditBox->setMaxLength(32);
		pfindEditBox->setPosition(ax::Vec2(218,102));
		pfindEditBox->setInputMode(ax::ui::EditBox::InputMode::ANY);
		pfindEditBox->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		ParentNode_Team->addChild(pfindEditBox);
		//pfindEditBox->setText(nTempStr);
	}
	//��ʼ�������ܲ˵�
	//�������������水ť
	Button* button_jiaoyi = Button::create();
	button_jiaoyi->setTouchEnabled(true);
	button_jiaoyi->loadTextures("ui/textbg.png", "ui/textbg.png", "");
	button_jiaoyi->setTitleFontSize(14);
	button_jiaoyi->setTitleText(UTEXT("���뽻��",1).c_str());
	button_jiaoyi->setPosition(ax::Vec2(85,78));
	button_jiaoyi->setTag(1);
	button_jiaoyi->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::oktouchEvent, this));
	m_pUiLayer->addChild(button_jiaoyi);

	Button* button_chakan = Button::create();
	button_chakan->setTouchEnabled(true);
	button_chakan->loadTextures("ui/textbg.png", "ui/textbg.png", "");
	button_chakan->setTitleFontSize(14);
	button_chakan->setTitleText(UTEXT("�쿴װ��",1).c_str());
	button_chakan->setPosition(ax::Vec2(215,78));
	button_chakan->setTag(2);
	button_chakan->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::oktouchEvent, this));
	m_pUiLayer->addChild(button_chakan);

	Button* button_chousha = Button::create();
	button_chousha->setTouchEnabled(true);
	button_chousha->loadTextures("ui/textbg.png", "ui/textbg.png", "");
	button_chousha->setTitleFontSize(14);
	button_chousha->setTitleText(UTEXT("��ɱĳ��",1).c_str());
	button_chousha->setPosition(ax::Vec2(345,78));
	button_chousha->setTag(3);
	button_chousha->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::oktouchEvent, this));
	m_pUiLayer->addChild(button_chousha);
	//�ڶ���
	Button* button_rudui = Button::create();
	button_rudui->setTouchEnabled(true);
	button_rudui->loadTextures("ui/textbg.png", "ui/textbg.png", "");
	button_rudui->setTitleFontSize(14);
	button_rudui->setTitleText(UTEXT("�������",1).c_str());
	button_rudui->setPosition(ax::Vec2(85,48));
	button_rudui->setTag(4);
	button_rudui->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::oktouchEvent, this));
	m_pUiLayer->addChild(button_rudui);

	Button* button_tanwei = Button::create();
	button_tanwei->setTouchEnabled(true);
	button_tanwei->loadTextures("ui/textbg.png", "ui/textbg.png", "");
	button_tanwei->setTitleFontSize(14);
	button_tanwei->setTitleText(UTEXT("�쿴̯λ",1).c_str());
	button_tanwei->setPosition(ax::Vec2(215,48));
	button_tanwei->setTag(5);
	button_tanwei->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::oktouchEvent, this));
	m_pUiLayer->addChild(button_tanwei);

	Button* button_rubang = Button::create();
	button_rubang->setTouchEnabled(true);
	button_rubang->loadTextures("ui/textbg.png", "ui/textbg.png", "");
	button_rubang->setTitleFontSize(14);
	button_rubang->setTitleText(UTEXT("�������",1).c_str());
	button_rubang->setPosition(ax::Vec2(345,48));
	button_rubang->setTag(6);
	button_rubang->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::oktouchEvent, this));
	m_pUiLayer->addChild(button_rubang);

	Button* button_liaotian = Button::create();
	button_liaotian->setTouchEnabled(true);
	button_liaotian->loadTextures("ui/textbg_1.png", "ui/textbg_1.png", "");
	button_liaotian->setTitleFontSize(14);
	button_liaotian->setTitleText(UTEXT("��������",1).c_str());
	button_liaotian->setPosition(ax::Vec2(345,103));
	button_liaotian->setTag(7);
	button_liaotian->addTouchEventListener(CC_CALLBACK_2(Kuiplayerfun::oktouchEvent, this));
	m_pUiLayer->addChild(button_liaotian);

	KUiPlayerTeam mTeam;
	ZeroMemory(&mTeam,sizeof(mTeam));
	if (g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (uintptr_t)&mTeam, 0)) //û�ж��� �ʹ���һ֧����
	{//�ж���
		if (mTeam.bTeamLeader)
		{//�ӳ�
			//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("��ʾ:�Ѿ��Ƕӳ�,����ѡ���Ա��");
		}
		else
		{//��Ա
		}
	}
	else
	{//û�ж���
	}

	//QueryAllChannel();
}

void Kuiplayerfun::_removeDelegate(bool isThis)
{
	/*pNearbyPlayerslistView->setTouchEnabled(isThis);
	pNearbyPlayerslistView->setBounceEnabled(isThis);

	pPlayersListView->setTouchEnabled(isThis);
	pPlayersListView->setBounceEnabled(isThis);*/
//	this->setTouchEnabled(isThis);

	if (isThis)
		ParentNode_Team->setPosition(ax::Vec2(winSize.width/2,winSize.height/2));
	else
		ParentNode_Team->setPosition(ax::Vec2(winSize.width*2,winSize.height/2));
	//ax::Director::getInstance()->getTouchDispatcher()->removeDelegate(this);//ɾ��ί��
}
void Kuiplayerfun::btnCallBackFunc(Ref * pSender)
{
	if (!g_pCoreShell|| !g_GameWorld || !pNearbyPlayerslistView) return;

	Node * node = NULL;
	if (pSender)
	{
		node= dynamic_cast<Node *>(pSender);
		switch(node->getTag())
		{
		case 1:
			{//�������
				if (__nSelIndex >0)
				{

				}
			}
			break;
		case 2:
			{//�߳�����

			}
			break;
		case 3:
			{//�ӳ��ƽ�
			}
			break;
		case 4:
			{//ˢ��  m_size.height-200
               pNearbyPlayerslistView->removeAllItems();
			   pNearbyPlayerslistView->setPosition(ax::Vec2(m_size.width/2+80,m_size.height-200));
			   OnRefresh();
			   pNearbyPlayerslistView->setGravity(ListView::Gravity::CENTER_VERTICAL);//�ж���
			   pNearbyPlayerslistView->forceDoLayout();
			}
			break;
		case 5:
			break;
		default:
			break;
		}
	}
	// remove last item
	// remove item by index
	//items_count = items->count();
	//listView->removeItem(items_count - 1);
}
//ִ���ϲ����Ļص��������رյ�����
void Kuiplayerfun::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node= dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);

	//this->removeFromParentAndCleanup(true);
}

//�رյ�����
void Kuiplayerfun::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//�����ť�ص�����
void Kuiplayerfun::oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type)
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

				if (nTbtn->getTag()==7)
				{
			      char nRoleName[64];
			      ZeroMemory(nRoleName,sizeof(nRoleName));
			      if (pfindEditBox)
				     sprintf(nRoleName, "%s", pfindEditBox->getText());

			      if  (nRoleName[0]==0)
			      {
#ifdef WIN32
				   messageBox("������ƴ���","��ʾ");
#else
				   messageBox(UTEXT("������ƴ���",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
				   break;
			      }
				  //��ʼ����ĳ���Ƿ����ߵ�
			      g_GameWorld->setchatCurChannel("CH_JABBER");

			      pListView_all->setTouchEnabled(false);
			      pListView_all->setBounceEnabled(false);
			      pListView_all->setVisible(false);
				  setBtnTouchEnabled(pListView_all,false);

			      pListView_siliao->setTouchEnabled(true);
			      pListView_siliao->setBounceEnabled(true);
			      pListView_siliao->setVisible(true);
				  setBtnTouchEnabled(pListView_siliao,true);

			      pListView_duiwu->setTouchEnabled(false);
			      pListView_duiwu->setBounceEnabled(false);
			      pListView_duiwu->setVisible(false);
				  setBtnTouchEnabled(pListView_duiwu,false);

			      pListView_chengshi->setTouchEnabled(false);
			      pListView_chengshi->setBounceEnabled(false);
			      pListView_chengshi->setVisible(false);
				  setBtnTouchEnabled(pListView_chengshi,false);

			      pListView_shijie->setTouchEnabled(false);
			      pListView_shijie->setBounceEnabled(false);
			      pListView_shijie->setVisible(false);
				  setBtnTouchEnabled(pListView_shijie,false);

			      pListView_bangpai->setTouchEnabled(false);
			      pListView_bangpai->setBounceEnabled(false);
				  setBtnTouchEnabled(pListView_bangpai,false);
			      pListView_bangpai->setVisible(false);

				  char nTempName[64];
				  ZeroMemory(nTempName,sizeof(nTempName));
				  sprintf(nTempName,"%s%s%s","/",nRoleName," ");
				  if (g_GameWorld)
				     g_GameWorld->setchatInputText(nTempName);
		          break;
				}
				if (__nSelIndex >0)
				{
					if (g_pCoreShell)
					{
						switch(nTbtn->getTag())
						{
						case 1:
							{//���뽻�ס���m_cMenuState.m_nState
								if (g_pCoreShell)
									g_pCoreShell->TradeApplyStart(__nSelIndex);
							}
							break;
						case 2:
							{//�쿴װ��
								g_pCoreShell->OperationRequest(GOI_VIEW_PLAYERITEM,0,__nSelIndex);
							}
							break;
						case 3:
							{//��ɱ
								g_pCoreShell->OperationRequest(GOI_REVENGE_SOMEONE,0,__nSelIndex);
							}
							break;
						case 4:
							{//���������
								g_pCoreShell->__ApplyAddTeam(__nSelIndex);
							}
							break;
						case 5:
							{//�򿪿�̯���
								g_pCoreShell->OperationRequest(GOI_VIEW_PLAYERSELLITEM,0,__nSelIndex);//�򿪿�̯���
							}
							break;
						case 6:
							{////�������

								KTongOperationParam Param;
								KTongMemberItem Player;
								memset(&Player, 0, sizeof(KTongMemberItem));
								memset(&Param, 0, sizeof(KTongOperationParam));
								Param.eOper = TONG_ACTION_APPLY; //��������ö��
								/*if(nType == TONG_ACTION_LEAVE)
								{//�������
									Param.nData[0] = m_nRelation;//�ͻ��˱��˺Ͱ��Ĺ�ϵ
									Param.nData[1] = -1;
								}
								else*/
								{
									Param.nData[0] = Npc[__nSelIndex].m_nFigure; // ��������Ա����Ĺ�ϵ
									Param.nData[1] = __nSelIndex;             // ������
								}
								//strcpy(Param.szPassword, m_szPassword);
								strcpy(Player.Name, Npc[__nSelIndex].Name);  //�������������
								g_pCoreShell->TongOperation(GTOI_TONG_ACTION, (uintptr_t)&Param, (intptr_t)&Player);
								//g_pCoreShell->TongOperation(GTOI_TONG_ACTION, 0,__nSelIndex);
							}
							break;
						default:
							break;
						}
					}
					break;
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
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void Kuiplayerfun::selectedBoxStateEvent(Ref* pSender, CheckBox::EventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;
	switch (type)
	{
	case CheckBox::EventType::UNSELECTED:
		{
		if (!g_pCoreShell->TeamOperation(TEAM_OI_CLOSE, 0,0))
		    break;

		if  (checkBox)
			checkBox->setSelected(false);
		}
		break;
	case CheckBox::EventType::SELECTED:
		//m_pDisplayValueLabel->setText(CCString::createWithFormat("Selected")->getCString());
		//messageBox("ѡ��","ѡ��");
		if (!g_pCoreShell->TeamOperation(TEAM_OI_CLOSE,0,1))
		{//��ʧ��
		   if  (checkBox)
			   checkBox->setSelected(false);
		}
		break;
	default:
		break;
	}
}
void Kuiplayerfun::selectedPlayersListItemEvent(Ref *pSender, ListView::EventType type)
{
//	char msg[64];
	switch (type)
	{
	case ListView::EventType::ON_SELECTED_ITEM_START:
		{
			ListView* listViewEx = static_cast<ListView*>(pSender);
			if (listViewEx)
			{
				__playrSelIndex = listViewEx->getCurSelectedIndex();
				Widget *item = listViewEx->getItem(__playrSelIndex);
				if  (item)
				{
					Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
					if  (button)
					{
						/*char nTempStr[512];
						ZeroMemory(nTempStr,sizeof(nTempStr));
						sprintf(nTempStr,button->getTitleText());
						messageBox(UTEXT(nTempStr,1).c_str(),"test");*/
						unsigned int nItemDwidx = button->getItemDwidx();
						if (nItemDwidx>0)
						{
							Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid = nItemDwidx;
							KUiItemLian nLian;
							memset(&nLian, 0, sizeof(KUiItemLian));
							sprintf(nLian.nPlayerName, "%s", std::string(button->getName()).c_str());
							nLian.nItemDWID=nItemDwidx;
							g_pCoreShell->OperationRequest(GOI_ITEM_LIANJIE,(intptr_t)&nLian,false);
						}
						Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid =0;
						//��ʼ������������ƣ�
						if  (button->getTag())
						{//������������Ƶ�
							char nTempName[64],nTempStr[64];
							ZeroMemory(nTempName,sizeof(nTempName));
							ZeroMemory(nTempStr,sizeof(nTempStr));
							std::string nSendNameStr = std::to_string(button->getTag());
							if (nSendNameStr.empty())
								break;
                            sprintf(nTempStr,"%s",nSendNameStr.c_str());
							int nLen =strlen(nTempStr);
							if (nLen>0)
							   nTempStr[nLen-1]='\0';

							sprintf(nTempName,"%s%s%s","/",UTEXT(nTempStr,1).c_str()," ");
							if (g_GameWorld)
								g_GameWorld->setchatInputText(nTempName);

						}

					}
				}

			}
			/*ListView* listViewEx = static_cast<ListView*>(pSender);
			if (listViewEx)
			{
			   __playrSelIndex = listViewEx->getCurSelectedIndex();

			   Widget *item = listViewEx->getItem(__playrSelIndex);
			   RichText * chatRickText = static_cast<RichText*>(item->getChildByTag(2));

			   if  (chatRickText)
			   {
				  RichElementText* nCurText = (RichElementText*)chatRickText->getElement(1);
				  if  (nCurText)
				  {
					  char nTempStr[512];
					  ZeroMemory(nTempStr,sizeof(nTempStr));
					  sprintf(nTempStr,nCurText->_getText().c_str());
					  messageBox(nTempStr,"test");
				  }
			   }
			}
			*/
			break;
		}
	case ListView::EventType::ON_SELECTED_ITEM_END:
		break;
	default:
		break;
	}
}

void Kuiplayerfun::setBtnTouchEnabled(ListView*listView,bool mEnabled)
{
	if  (!listView) return;
	auto items  = listView->getItems();
	int items_count = items.size();

	if  (items_count>0)
	{
		int i;
		for (i=0;i<items_count;i++)
		{
			Widget *item = listView->getItem(i);
			if  (item)
			{
			  Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
			  if  (button)
				  button->setTouchEnabled(mEnabled);
			}
		}
	}
}

void Kuiplayerfun::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
	switch (type)
	{
	case ListView::EventType::ON_SELECTED_ITEM_START:
		{
			ListView* listViewEx = static_cast<ListView*>(pSender);
			if (listViewEx)
			{
				int nCurSelindex = listViewEx->getCurSelectedIndex();
				if (pfindEditBox)
				{
					Widget *item = listViewEx->getItem(nCurSelindex);
					Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
					__nSelIndex = button->getTag();//npcindex;
					pfindEditBox->setText(std::string(button->getTitleText()).c_str());
				}
			}
				//CCLOG("select child start index = %d", listViewEx->getCurSelectedIndex());
			break;
		}
	case ListView::EventType::ON_SELECTED_ITEM_END:
		{
			break;
		}
	default:
		break;
	}

	//messageBox(msg,"����¼�");
}


//��д����ע�ắ�������¸�����������
void Kuiplayerfun::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,128, true);
}

Rect Kuiplayerfun::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y

	return rc;
}

void Kuiplayerfun::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//�ƶ���
void Kuiplayerfun::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling || !__isCanMove) return;
	Point point   = ParentNode_Team->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff    = (point - m_lastPoint);                  //����ͼƬ��λ��
	ParentNode_Team->setPosition((ParentNode_Team->getPosition() + diff));
	m_origin      = ParentNode_Team->getPosition();
	m_lastPoint = point;
}
//��������ccTouchBegan������true
bool Kuiplayerfun::ccTouchBegan(Touch *pTouch, Event *pEvent)
{

	Point point = ParentNode_Team->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Team->getContentSize().width*/,winSize.height/* ParentNode_Team->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //����ڷ�Χ�� �Ϳ����϶�
	m_lastPoint = point;

	return true;
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	/*Point location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	Sprite * SkillNode =NULL;
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; ++i)
	{
		SkillNode = (Sprite *)ParentNode_Team->getChildByTag(i+1);
		if  (SkillNode)
		{
			if (getRect(SkillNode,i+1).containsPoint(location))
			{//������
				SkillNode->runAction(red);
			}
			else
			{
				SkillNode->setColor(ax::Color3B::WHITE);           //Ĭ����ɫ
			}
		}
	}
	return true;*/
}

void Kuiplayerfun::OnRefresh()
{
	if (!g_pCoreShell || !g_GameWorld ||!pNearbyPlayerslistView) return;
	//��ʼ��Ӹ�������б�
	if (m_pNearbyPlayersList)
	{
		free(m_pNearbyPlayersList);
		m_pNearbyPlayersList = NULL;
	}
	//GDI_NEARBY_PLAYER_LIST
	int nCount = g_pCoreShell->GetGameData(GDI_NEARBY_PLAYER_LIST, 0, 0);
	__NearbyListCount = nCount;
	if (nCount)
	{
		m_pNearbyPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * nCount);
		if (m_pNearbyPlayersList)
		{
			g_pCoreShell->GetGameData(GDI_NEARBY_PLAYER_LIST, (uintptr_t)m_pNearbyPlayersList,nCount);
			for (int i = 0; i < nCount; ++i)
			{
				Button* custom_button = Button::create();
				custom_button->setName("TextButton");
				custom_button->setTouchEnabled(true);
				//_button_2->loadTextures("ui/textbg.png", "ui/textbg.png", "");
				custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png","");
				custom_button->setScale9Enabled(true);
				custom_button->setContentSize(btnSize);
				custom_button->setTitleFontSize(12);
				char nTempstr[32];
				sprintf(nTempstr, "%s", m_pNearbyPlayersList[i].Name);
				custom_button->setTitleText(UTEXT(nTempstr,1));

				Layout *custom_item = Layout::create();
				custom_item->setContentSize(btnSize);
				custom_button->setPosition(ax::Vec2(btnSize.width/2, btnSize.height/2));
				custom_item->addChild(custom_button);
                int nikey = std::hash<std::string>{}(m_pNearbyPlayersList[i].Name);
				custom_button->setTag(nikey);
				custom_button->setTag(m_pNearbyPlayersList[i].nIndex);//NpcIndex
				pNearbyPlayerslistView->pushBackCustomItem(custom_item);
				//m_pNearbyPlayersList++;
			}
		}
	}
	else
	{
		Button* custom_button = Button::create();
		custom_button->setName("TextButton");
		custom_button->setTouchEnabled(true);
		custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png", "");
		custom_button->setScale9Enabled(true);
		custom_button->setContentSize(btnSize);
		custom_button->setTitleFontSize(12);
		char nTempstr[32];
		sprintf(nTempstr, "%s", "��������ڸ���");
		custom_button->setTitleText(UTEXT(nTempstr,1));
		custom_button->setTag(-1);//NpcIndex
        int nikey = std::hash<std::string>{}("");
		custom_button->setTag(nikey);
		Layout *custom_item = Layout::create();
		custom_item->setContentSize(btnSize);
		custom_button->setPosition(ax::Vec2(btnSize.width/2, btnSize.height/2));
		custom_item->addChild(custom_button);
		//custom_item->setTag(m_pNearbyPlayersList->nIndex);//NpcIndex
		pNearbyPlayerslistView->pushBackCustomItem(custom_item);
	}
}


void Kuiplayerfun::addchatmsg(char * typeName,char * sendName,char * contentt,ListView*listView,int nKind,int nColor,unsigned int nItemDwidx,char*scrSendName,int nPackage)
{
	if  (!listView) return;

	auto items = listView->getItems();
	int items_count = items.size();
	if (items_count>=MSG_MAX_COUNT/2)
	{
		listView->removeItem(0);//ɾ����һ��
		//listView->sortAllChildren();
		listView->forceDoLayout();
		//CCArray *items = listView->getItems();
		//int items_count = items->count();
	}
	ax::Color3B ccNameColor={255,255,255};
	ax::Color3B ccContColor={255,255,255};
	ax::Color3B ccGREEN_ ={98,255,255};
	ax::Color3B ccBlue_ ={100,100,255};
	ax::Color3B ccZIse_ ={188,64,255};
	switch(listView->getTag())
	{
	case CH_ALL_LIST://all
		{//ȫ��
			switch(nKind)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				{
					ccNameColor=ax::Color3B::RED;
					ccContColor=ax::Color3B::RED;
				}
				break;
			case 4:
				{//����
					ccNameColor=ax::Color3B::GREEN;
					ccContColor=ax::Color3B::GREEN;
				}
				break;
			case 5:
				{//����
					ccNameColor=ccGREEN_;
					ccContColor=ccGREEN_;
				}
				break;
			case 6:
				{//���
					ccNameColor=ax::Color3B::YELLOW;
					ccContColor=ax::Color3B::YELLOW;
				}
				break;
			case 7:
				{//����
					ccNameColor=ccBlue_;
					ccContColor=ccBlue_;
				}
				break;
			case 8:
				{//����
					ccNameColor=ax::Color3B::YELLOW;
					ccContColor=ax::Color3B::YELLOW;
				}
				break;
			case 9:
				{//ϵͳ
					ccNameColor=ax::Color3B::RED;
					ccContColor=ax::Color3B::YELLOW;
				}
				break;
			case 10:
				{//��������
					ccNameColor=ax::Color3B::WHITE;
					ccContColor=ax::Color3B::WHITE;
				}
				break;
			case 11:
				{//��������Ϣ
					ccNameColor=ax::Color3B::RED;
					ccContColor=ax::Color3B::RED;
				}
				break;
			default:
				{
					ccNameColor=ax::Color3B::RED;
					ccContColor=ax::Color3B::RED;
				}
				break;
			}
		}
		break;
	case CH_SHIJIE_LIST:
		{//����
			ccNameColor=ax::Color3B::GREEN;
			ccContColor=ax::Color3B::GREEN;
		}
		break;
	case CH_CHENGSHI_LIST:
		{//����
			ccNameColor=ccGREEN_;
			ccContColor=ccGREEN_;
		}
		break;
	case CH_DUIWU_LIST:
		{//����
			ccNameColor=ccBlue_;
			ccContColor=ccBlue_;
		}
		break;
	case CH_SILIAO_LIST:
		{//˽��
		  if  (nKind)
		  {//�ͻ��˱���
			  ccNameColor=ax::Color3B::ORANGE;
			  ccContColor=ax::Color3B::ORANGE;
		  }
		  else
		  {//�Է�
			ccNameColor=ax::Color3B::MAGENTA;
			ccContColor=ax::Color3B::MAGENTA;
		  }
		}
		break;
	case CH_BANGPAI_LIST:
		{//����
			ccNameColor=ax::Color3B::YELLOW;
			ccContColor=ax::Color3B::YELLOW;
		}
		break;
	default:
		break;
	}
	Size nRenderSize;
	nRenderSize.width  = bgSize.width-5;
	nRenderSize.height = 12;
	RichText  * chatRickText = RichText::create();//WithParentSize(nParentSize);
	chatRickText->setContentSize(nRenderSize);
	chatRickText->ignoreContentAdaptWithSize(false);//�Ƿ����������Ӧ�߿��  ��һ��
	chatRickText->setAnchorPoint(ax::Vec2(0,0));
	//chatRickText->setTag(chatKey);
	RichElementText* kindname = NULL;
	if (typeName)
		kindname = RichElementText::create(1,ccNameColor,255,UTEXT(typeName,1).c_str(),UI_GAME_FONT_DEFAULT,12);

	RichElementText* senname = RichElementText::create(1,ccNameColor,255,UTEXT(sendName,1).c_str(),UI_GAME_FONT_DEFAULT,12);
	RichElementText* Msg;//     = RichElementText::create(2,ccContColor,255,UTEXT(contentt,1).c_str(),UI_GAME_FONT_DEFAULT,12);
	//if  (nPackage==100000000)
	//	Msg = RichElementText::create(2,ccContColor,255,contentt,UI_GAME_FONT_DEFAULT,12);
	//else
	Msg = RichElementText::create(2,ccContColor,255,UTEXT(contentt,1).c_str(),UI_GAME_FONT_DEFAULT,12);

	if (kindname)
	  chatRickText->pushBackElement(kindname);

	chatRickText->pushBackElement(senname);
	chatRickText->pushBackElement(Msg);
	//chatRickText->RendererElement();
	nRenderSize = chatRickText->getVirtualRendererSize();
	chatRickText->setContentSize(nRenderSize);

	Button* custom_button = Button::create();

	if (typeName)
	{
		custom_button->setTag(1);
        int nikey = std::hash<std::string>{}(sendName);
	    custom_button->setTag(nikey);
	}
	else
	{
		custom_button->setTag(false);
        int nikey = std::hash<std::string>{}("");
		custom_button->setTag(nikey);
	}

	custom_button->setName("TextButton");
	custom_button->setItemDwidx(nItemDwidx);
//	custom_button->setItemName(scrSendName);
	custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png","");
	custom_button->setOpacity(0); //255Ϊ��ȫ��͸�� 0Ϊ��ȫ͸��
	custom_button->setAnchorPoint(ax::Vec2(0,0));
	custom_button->setScale9Enabled(true);
	Size nBtnSize;
	nBtnSize.width = nRenderSize.width - 50;
	nBtnSize.height= nRenderSize.height;
	custom_button->setContentSize(nBtnSize);
	custom_button->setTitleFontSize(12);

	//custom_button->setTitleText(UTEXT(contentt,1));

	Layout *custom_item = Layout::create();
	custom_item->setContentSize(nRenderSize);
	chatRickText->setPosition(ax::Vec2(0,nRenderSize.height-16));
	custom_item->addChild(chatRickText,2,2);
	//һ�еľͶ� ���г�������
	int nRowS = ceil(chatRickText->getContentSize().width / chatRickText->getVirtualRendererSize().width);
	if  (nRowS>1)
	{
		float nh = nRenderSize.height/nRowS; //ÿ�еĸ߶�
		custom_button->setPosition(ax::Vec2(0,chatRickText->getPosition().y-3-(nRowS-1)*nh));
	}
	else
	    custom_button->setPosition(ax::Vec2(0,chatRickText->getPosition().y-3));

	custom_item->addChild(custom_button,1,1);
	listView->pushBackCustomItem(custom_item);

	listView->forceDoLayout();
	listView->jumpToBottom();
}

//--------------------------------------------------------------------------
//	���ܣ����¶�����Ϣ
//--------------------------------------------------------------------------
void Kuiplayerfun::UpdateData(KUiPlayerTeam* pInfo)
{
	if (!g_pCoreShell || !g_GameWorld) return;
	Clear();
	KUiPlayerLeaderShip	LeaderShip;
	memset(&LeaderShip, 0, sizeof(KUiPlayerLeaderShip));
	g_pCoreShell->GetGameData(GDI_PLAYER_LEADERSHIP,(uintptr_t)&LeaderShip, 0);  //����ͳ˧������ص�����
	//char msg[64];
	//sprintf(msg,"%d",LeaderShip.nLeaderShipLevel);
	//pMoneyLabel->setString(msg);//m_LeaderAbility.SetIntText(LeaderShip.nLeaderShipLevel);  //ͳ˧

	if (pInfo)
	{
		m_Info = *pInfo;
	}
	else if (!g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (uintptr_t)&m_Info, 0))//�������ڵĶ�����Ϣ
		memset(&m_Info,0,sizeof(m_Info));

	if (m_Info.bTeamLeader)  //����Ƕӳ�
	{//��ť����
		if  (checkBox && m_Info.bOpened)
			checkBox->setSelected(true);

	}
	else
	{
		if  (checkBox)
			checkBox->setSelected(false);
	}

	if (m_Info.cNumMember > 0)
	{//����ж���
		m_pPlayersList = (KUiPlayerItem*)malloc(sizeof(KUiPlayerItem) * (m_Info.cNumMember));
		if (m_pPlayersList)
		{
			int nCount = g_pCoreShell->TeamOperation(TEAM_OI_GD_MEMBER_LIST,(uintptr_t)m_pPlayersList, m_Info.cNumMember);
			if (nCount == m_Info.cNumMember)
			{//��ʼ�����б�����
                __playerListCount =nCount;
				for (int i = 0; i < nCount; ++i)
				{
					Button* custom_button = Button::create();
					custom_button->setName("TextButton");
					custom_button->setTouchEnabled(true);
					custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png","");
					custom_button->setScale9Enabled(true);
					custom_button->setContentSize(btnSize);
					custom_button->setTitleFontSize(12);
					char nTempstr[32];
					sprintf(nTempstr, "%s", m_pPlayersList[i].Name);
					custom_button->setTitleText(UTEXT(nTempstr,1));

					Layout *custom_item = Layout::create();
					custom_item->setContentSize(btnSize);
					custom_button->setPosition(ax::Vec2(btnSize.width/2, btnSize.height/2));
					custom_item->addChild(custom_button);
					custom_item->setTag(m_pPlayersList[i].nIndex);//NpcIndex
					pListView_all->pushBackCustomItem(custom_item);
				}
			}
		}
		else
		{
			m_Info.cNumMember = 0;
			__playerListCount = 0;
		}
	}
	OnRefresh();
}

void Kuiplayerfun::Clear()
{
   //���ð�ť״̬
	memset(&m_Info, 0, sizeof(KUiPlayerTeam));
	if (m_pPlayersList)
	{
		free(m_pPlayersList);
		m_pPlayersList = NULL;
	}

	if (m_pNearbyPlayersList)
	{
		free(m_pNearbyPlayersList);
		m_pNearbyPlayersList = NULL;
	}

	if (pListView_all)
	{
		pListView_all->removeAllItems();
		pListView_all->forceDoLayout();
	}
	if (pNearbyPlayerslistView)
	{
		pNearbyPlayerslistView->removeAllItems();
		pNearbyPlayerslistView->forceDoLayout();
	}
}
//����ϵͳ����
int	Kuiplayerfun::PushChannelData(unsigned long dwID, const char* Buffer, int nLen)
{
	if (!m_pActivateChannel)
	{
		Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid = 0;
		return -1;
	}
	int nIndex  = GetChannelIndex(dwID);
	if (nIndex >= 0 && this && Buffer && nLen > 0)
	{
		int nRes = m_pActivateChannel[nIndex].ResourceIndex;
		if (nRes >= 0 &&
			m_pActivateChannel[nIndex].uLeftSendMsgNum < m_ChannelsResource[nRes].uSendMsgNum)
		{
			int nCur = IR_GetCurrentTime();
			int nLeft = (nCur - m_pActivateChannel[nIndex].uLastSendMsgTime) > m_ChannelsResource[nRes].uSendMsgInterval ? 0 : (nCur - m_pActivateChannel[nIndex].uLastSendMsgTime);
			nLeft += m_pActivateChannel[nIndex].uLeftSendMsgNum * m_ChannelsResource[nRes].uSendMsgInterval;

			int nOffset = m_pActivateChannel[nIndex].uBufferOffset; //ԭ����ƫ��

			unsigned int nSendTime = nCur + nLeft;
			memcpy(m_pActivateChannel[nIndex].Buffer + nOffset, &nSendTime, sizeof(nSendTime));
			nOffset += sizeof(nSendTime);

			unsigned int nItemDwidx = Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid;//g_pCoreShell->GetGameData(GDI_ITEM_LINKDATA,0, 0);
			memcpy(m_pActivateChannel[nIndex].Buffer + nOffset, &nItemDwidx, sizeof(nItemDwidx));
			nOffset += sizeof(nItemDwidx);

			memcpy(m_pActivateChannel[nIndex].Buffer + nOffset, &nLen, sizeof(nLen));
			nOffset += sizeof(nLen);

			memcpy(m_pActivateChannel[nIndex].Buffer + nOffset, Buffer, nLen);
			nOffset += nLen;

			m_pActivateChannel[nIndex].uLeftSendMsgNum++;
			m_pActivateChannel[nIndex].uBufferOffset = nOffset;
			Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid = 0;
			return nLeft;
		}
	}
	Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid = 0;
	return -1;
}

int	Kuiplayerfun::GetChannelData(unsigned long& dwID, BYTE& cost, char*& Buffer, int& nLen,int& nIiemDwIdx)
{
	if (this)
	{
		if (!m_pActivateChannel) return 0;
		unsigned int nSendTime = IR_GetCurrentTime();
		for (int i = 0; i < m_nActivateChannels; ++i)
		{
			if (m_pActivateChannel[i].uLeftSendMsgNum > 0)
			{
				unsigned int* pST = (unsigned int*)(m_pActivateChannel[i].Buffer);
				if (*pST <= nSendTime)
				{
					dwID = m_pActivateChannel[i].nChannelID;
					cost = m_pActivateChannel[i].cost;
					nIiemDwIdx = *((unsigned int*)(m_pActivateChannel[i].Buffer + sizeof(unsigned int)));
					nLen = *((int *)(m_pActivateChannel[i].Buffer + sizeof(unsigned int)+ sizeof(unsigned int)));  //ԭ���ݵĳ���
					Buffer = m_pActivateChannel[i].Buffer + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(int); //ԭ����
					return 1;
				}
			}
		}
	}

	return 0;
}
//��ջ
int	Kuiplayerfun::PopChannelData(unsigned long dwID)
{
	if (this)
	{
		if (!m_pActivateChannel) return 0;
		unsigned int nSendTime = IR_GetCurrentTime();
		for (int i = 0; i < m_nActivateChannels; ++i)
		{
			if (m_pActivateChannel[i].nChannelID == dwID &&
				m_pActivateChannel[i].uLeftSendMsgNum > 0)
			{
				int nLen = *((int *)(m_pActivateChannel[i].Buffer + sizeof(unsigned int) + sizeof(unsigned int)));
				int nMem = nLen + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(int);
				memmove(m_pActivateChannel[i].Buffer,
					m_pActivateChannel[i].Buffer + nMem,
					m_pActivateChannel[i].uBufferOffset - nMem);
				m_pActivateChannel[i].uBufferOffset -= nMem;
				m_pActivateChannel[i].uLeftSendMsgNum--;
				m_pActivateChannel[i].uLastSendMsgTime = nSendTime; //��ֵ���ķ���ʱ��

				return 1;
			}
		}
	}

	return 0;
}

void Kuiplayerfun::ReplaceChannelName(char* szDest, size_t nDestSize, char* szSrc) //�滻Ƶ�������֣�
{
	if (this && szSrc[0] != 0)
	{
		for (int n = 0; n < m_nChannelsResource; n++)
		{
			for (int m = 0; m < MAX_CHANNELRESOURCE; m++)
			{
				if (m_ChannelsResource[n].cShortTitle[m][0] != 0)
				{
					if (strcmp(szSrc, m_ChannelsResource[n].cShortTitle[m]) == 0)
					{
						strncpy(szDest, m_ChannelsResource[n].cTitle, nDestSize);  //Ƶ�������� ��ֵ�� Ŀ�� Dest ����
						return;
					}
				}
			}
		}
	}
	strncpy(szDest, szSrc, nDestSize);
}

int	Kuiplayerfun::ReleaseActivateChannelAll()
{
	if (this)
	{
		int nOld = m_nActivateChannels;
		if (m_pActivateChannel)
			delete [] m_pActivateChannel;  //�ͷ��ڴ�ռ�
		m_pActivateChannel = NULL;
		m_nActivateChannels = 0;
		return nOld;
	}
	return 0;
}

//�����µ�Ƶ��
void Kuiplayerfun::SendChannelSubscribe(int nChannelIndex, bool b)
{
	if (nChannelIndex < 0 || nChannelIndex >= m_nActivateChannels)
		return;

	char ProtocolBuffer[sizeof(tagExtendProtoHeader) + sizeof(PLAYERCOMM_SUBSCRIBE)];
	int nProtocolOffset = 0;
	tagExtendProtoHeader* pHead = (tagExtendProtoHeader*)ProtocolBuffer;
	pHead->ProtocolType = c2s_extend;
	nProtocolOffset += sizeof(tagExtendProtoHeader);

	PLAYERCOMM_SUBSCRIBE* pdata = (PLAYERCOMM_SUBSCRIBE*)(ProtocolBuffer + nProtocolOffset);
	pdata->ProtocolFamily = pf_playercommunity;
	pdata->ProtocolID =	playercomm_c2s_subscribe;
	pdata->channelid = m_pActivateChannel[nChannelIndex].nChannelID;
	pdata->subscribe = b;

	nProtocolOffset += sizeof(PLAYERCOMM_SUBSCRIBE);

	pHead->wLength = nProtocolOffset - 1;

	if (g_pCoreShell)
	   g_pCoreShell->SendNewDataToServer(ProtocolBuffer, nProtocolOffset);
}
//����Ƶ��
void Kuiplayerfun::SendQueryChannelID(int nChannelResourceIndex) //����Ƶ�� �ı� Ƶ������
{
	if (nChannelResourceIndex < 0 || nChannelResourceIndex >= m_nChannelsResource)
		return;

	if (FindActivateChannelResourceIndex(m_ChannelsResource[nChannelResourceIndex].cTitle) >= 0)	//���������
		return;

	char ChannelName[__X_NAME_LEN_]={0};
	//ChannelName[0];
	if (m_ChannelsResource[nChannelResourceIndex].cFormatName[0] == 0)
		return;
	if (!ReplaceSpecialField(ChannelName, m_ChannelsResource[nChannelResourceIndex].cFormatName)) //�ܲ�������Ƶ�� Ҫ��������
	{
		//messageBox(ChannelName,"EORRPR TEST");
		return;
	}
	if (ChannelName[0] == 0)   //��˫б�� �ַ�
		return;
	//messageBox(ChannelName,"TEST");
	/////�����Ƿ�Э��� S3Relay ����Ƶ��
	char ProtocolBuffer[sizeof(tagExtendProtoHeader) + sizeof(PLAYERCOMM_QUERYCHANNELID)];
	int nProtocolOffset = 0;
	tagExtendProtoHeader* pHead = (tagExtendProtoHeader*)ProtocolBuffer;
	pHead->ProtocolType = c2s_extend;
	nProtocolOffset += sizeof(tagExtendProtoHeader);

	PLAYERCOMM_QUERYCHANNELID* pdata = (PLAYERCOMM_QUERYCHANNELID*)(ProtocolBuffer + nProtocolOffset);
	pdata->ProtocolFamily = pf_playercommunity;
	pdata->ProtocolID =	playercomm_c2s_querychannelid;
	strncpy(pdata->channel,U2G(ChannelName).c_str(), __X_NAME_LEN_);  //��Ƶ�����ַ��͹�ȥ   channel �Ǻ��� ˫б�ܵ� �ַ�

	nProtocolOffset += sizeof(PLAYERCOMM_QUERYCHANNELID);

	pHead->wLength = nProtocolOffset - 1;
	if (g_pCoreShell)
	   g_pCoreShell->SendNewDataToServer(ProtocolBuffer, nProtocolOffset); //֪ͨ������
}


void Kuiplayerfun::QueryAllChannel()
{
	if (this)
	{
		for (int n = 0; n < m_nChannelsResource; n++)
		{
			SendQueryChannelID(n);
		}
	}
}

BYTE Kuiplayerfun::FindActivateChannelCost(unsigned long nChannelID)
{
	if  (!m_pActivateChannel) return -1;
	for (int n = 0; n < m_nActivateChannels; n++)
	{
		if (m_pActivateChannel[n].nChannelID == nChannelID)
			return m_pActivateChannel[n].cost;
	}
	return -1;
}

int	Kuiplayerfun::FindActivateChannelIndexByKey(char* cKey)
{
	if (cKey == NULL || cKey[0] == 0)
		return -1;
	if  (!m_pActivateChannel) return -1;
	for (int n = 0; n < m_nActivateChannels; n++)
	{
		if (m_pActivateChannel[n].ResourceIndex >= 0)
		{
			if (m_ChannelsResource[m_pActivateChannel[n].ResourceIndex].cFormatName[0] != 0)
			{
				char *pdest = strstr(m_ChannelsResource[m_pActivateChannel[n].ResourceIndex].cFormatName, cKey);
				if (pdest)
					return n;
			}
		}
	}
	return -1;
}

unsigned long Kuiplayerfun::GetChannelID(int nChannelIndex)
{
	if (!m_pActivateChannel) return -1;
	if (this && nChannelIndex >= 0 && nChannelIndex < m_nActivateChannels)
	{
		return m_pActivateChannel[nChannelIndex].nChannelID;
	}
	return -1;
}

int Kuiplayerfun::GetChannelResourceIndex(int nChannelIndex)
{
	if (!m_pActivateChannel) return -1;
	if (this && nChannelIndex >= 0 && nChannelIndex < m_nActivateChannels)
	{
		return m_pActivateChannel[nChannelIndex].ResourceIndex;
	}
	return -1;
}

char* Kuiplayerfun::GetChannelTitle(int nChannelIndex)//ȡ��ĳIDƵ���� ����
{
	if (!m_pActivateChannel) return NULL;
	if (this && nChannelIndex >= 0 && nChannelIndex < m_nActivateChannels)
	{
		if (m_pActivateChannel[nChannelIndex].ResourceIndex >= 0)
			return m_ChannelsResource[m_pActivateChannel[nChannelIndex].ResourceIndex].cTitle;
	}
	return NULL;
}

unsigned long Kuiplayerfun::GetChannelCount() //ȡ��Ƶ�� ����
{
	if (this)
		return m_nActivateChannels;
	return 0;
}

int Kuiplayerfun::GetChannelIndex(char* channelName)
{
	if (this)
		return FindActivateChannelIndex(channelName);
	return -1;
}

int Kuiplayerfun::GetChannelIndex(unsigned long dwID)
{
	if (this)
		return  FindActivateChannelIndexById(dwID);
	return -1;
}

//˽����ʾ(��ʾ�Է�����Ϣ)���ܣ�
void Kuiplayerfun::ShowMSNMessage(char* szName, const char* pMsgBuff, unsigned short nMsgLength,unsigned long nItemDwidx,bool isClient)
{//
	//CCTMXTiledMap
	char Buffer[560];
	char nTempSendName[64];
	char nTempSendLinkName[64];
	ZeroMemory(nTempSendName,sizeof(nTempSendName));
	ZeroMemory(nTempSendLinkName,sizeof(nTempSendLinkName));
	ZeroMemory(Buffer,sizeof(Buffer));
	int nOffset = 0;
	/*int nLen = 0;
	nLen = strlen(szName);
	memcpy(Buffer + nOffset,szName, nLen);
	nOffset += nLen;
	Buffer[nOffset++] = ':'*/
	memcpy(&Buffer[nOffset],pMsgBuff,nMsgLength);
	//nOffset += FilterTextColor(Buffer + nOffset, nMsgLength);
	Buffer[nMsgLength] = '\0';
	char nTempTypeName[32];
	ZeroMemory(nTempTypeName,sizeof(nTempTypeName));
	if (isClient)
	{
		sprintf(nTempSendLinkName, "%s", Player[CLIENT_PLAYER_INDEX].m_PlayerName);
		sprintf(nTempTypeName,"[��]");
		sprintf(nTempSendName,"%s:",szName);
	}
	else
	{
		sprintf(nTempSendLinkName, "%s", szName);
		sprintf(nTempTypeName,"[��]");
	    sprintf(nTempSendName,"%s:",szName);
	}
#ifdef WIN32
	//messageBox(Buffer,"����");//
	addchatmsg(nTempTypeName,nTempSendName,Buffer,pListView_siliao,isClient,0,nItemDwidx,nTempSendLinkName);
#else
	//messageBox(UTEXT(Buffer,1).c_str(),"˽��");//
	if (!isClient)
	    sprintf(nTempSendName, "%s", UTEXT(nTempSendName,1).c_str());
	//sprintf(Buffer,UTEXT(Buffer,1).c_str());
	addchatmsg(nTempTypeName,nTempSendName,Buffer,pListView_siliao,isClient,0,nItemDwidx,nTempSendLinkName);
#endif
	m_uLastDelMsgTime = IR_GetCurrentTime();


}

//��Ϣ���� ��ʼ������Ϣ
int Kuiplayerfun::NewChannelMessageArrival(unsigned long nChannelID, char* szSendName,char* pMsgBuff, unsigned short nMsgLength,unsigned long nItemDwidx,char *scrSendName,int nPackage)
{
	if  (!m_pActivateChannel) return false;
	int nChannelIndex =FindActivateChannelIndex(nChannelID);

	int nID = m_pActivateChannel[nChannelIndex].ResourceIndex;
	char nTempSendName[64];
	char nTempTypeName[64];
	ZeroMemory(nTempTypeName,sizeof(nTempTypeName));
	ZeroMemory(nTempSendName,sizeof(nTempSendName));
	if (nChannelIndex >= CH_NEARBY) //ȫ��  �� ����Ƶ��
	{
		/*if  (strstr(szSendName,"����"))
		    addmsg(szSendName,pMsgBuff,pListView_all,0);
		else */
		if (strstr(szSendName,"֪ͨ:"))
		{
			//nMsgLength = FilterTextColor(pMsgBuff,nMsgLength);
			sprintf(nTempTypeName,"֪ͨ:");
			addchatmsg(nTempTypeName,szSendName,pMsgBuff,pListView_all,1,0,0,NULL,nPackage);
		}
	    else if (strstr(szSendName,"ս��:"))
		{
			sprintf(nTempTypeName,"ս��:");
		    addchatmsg(nTempTypeName,szSendName,pMsgBuff,pListView_all,2,0,0,NULL,nPackage);
		}
		else if (strstr(szSendName,"��ս:"))
		{
			sprintf(nTempTypeName,"��ս:");
			addchatmsg(nTempTypeName,szSendName,pMsgBuff,pListView_all,3,0,0,NULL,nPackage);
		}
	    else
		{

			if (nID == CH_WORLD) //����
			{
				sprintf(nTempTypeName,"[����]");
				sprintf(nTempSendName,"%s:",szSendName);
			    addchatmsg(nTempTypeName,nTempSendName,pMsgBuff,pListView_all,4,0,nItemDwidx,scrSendName,nPackage);
				addchatmsg(nTempTypeName,nTempSendName,pMsgBuff,pListView_shijie,-1,0,nItemDwidx,scrSendName,nPackage);
			}
			else if (nID == CH_CITY)//����
			{
				sprintf(nTempTypeName,"[����]");
				sprintf(nTempSendName,"%s:",szSendName);
				addchatmsg(nTempTypeName,nTempSendName,pMsgBuff,pListView_all,5,0,nItemDwidx,scrSendName,nPackage);
				addchatmsg(nTempTypeName,nTempSendName,pMsgBuff,pListView_chengshi,-1,0,nItemDwidx,scrSendName,nPackage);
			}
			else if (nID == CH_TONG) //����
			{
				sprintf(nTempTypeName,"[���]");
				sprintf(nTempSendName,"%s:",szSendName);
				addchatmsg(nTempTypeName,nTempSendName,pMsgBuff,pListView_all,6,0,nItemDwidx,scrSendName,nPackage);
				addchatmsg(nTempTypeName,nTempSendName,pMsgBuff,pListView_bangpai,-1,0,nItemDwidx,scrSendName,nPackage);
			}
			else if (nID == CH_TEAM)
			{
				sprintf(nTempTypeName,"[����]");
				sprintf(nTempSendName,"%s:",szSendName);
				addchatmsg(nTempTypeName,nTempSendName,pMsgBuff,pListView_all,7,0,nItemDwidx,scrSendName,nPackage);
				addchatmsg(nTempTypeName,nTempSendName,pMsgBuff,pListView_duiwu,-1,0,nItemDwidx,scrSendName,nPackage);
			}
			else if (nID == CH_FACTION)
			{
				sprintf(nTempTypeName,"[����]");
				sprintf(nTempSendName,"%s:",szSendName);
				addchatmsg(nTempTypeName,szSendName,pMsgBuff,pListView_all,8,0,nItemDwidx,scrSendName,nPackage);
			}
			else if (nID == CH_SYSTEM)
			{
				sprintf(nTempSendName,"%s:",szSendName);
				addchatmsg(NULL,nTempSendName,pMsgBuff,pListView_all,9,0,nItemDwidx,scrSendName,nPackage);
			}
			else if (nID == CH_NEARBY)
			{//��������
				sprintf(nTempTypeName,"[����]");
				sprintf(nTempSendName,"%s:",szSendName);
				addchatmsg(nTempTypeName,nTempSendName,pMsgBuff,pListView_all,10,0,nItemDwidx,scrSendName,nPackage);
			}
			else
			{
				//nMsgLength = FilterTextColor(pMsgBuff,nMsgLength);
				sprintf(nTempTypeName,"[����]:");
				addchatmsg(nTempTypeName,szSendName,pMsgBuff,pListView_all,11,0,nItemDwidx,scrSendName,nPackage);
			}

	    }
	}

	return true;
}

bool Kuiplayerfun::GetChannelSubscribe(int nChannelIndex) //ȡ�� ����Ƶ��
{
	if (!m_pActivateChannel) return false;
	if (this && nChannelIndex >= 0 && nChannelIndex < m_nActivateChannels)
	{
		return m_pActivateChannel[nChannelIndex].bSubscribe;
	}
	return false;
}

bool Kuiplayerfun::IsChannelType(int nChannelIndex, SelfChannel type) //Ƶ������
{
	if (this && nChannelIndex >= 0 && nChannelIndex < m_nActivateChannels)
	{
		switch (type)
		{
		case ch_Team: //����
		case ch_Faction://����
		case ch_Tong://����
		case ch_Screen://����
		case ch_Msgr:
		case ch_GM://GM
			{
				return FindActivateChannelIndexByKey(s_keys[type]) == nChannelIndex;
			}
			break;
		default:
			return false;
		}
	}
	return false;
}


int	Kuiplayerfun::IsNeverCloseActivateChannel(int nChannelIndex)
{
	if (nChannelIndex < 0 || nChannelIndex >= m_nActivateChannels)
		return 0;
	if (!m_pActivateChannel) return 0;
	int n = m_pActivateChannel[nChannelIndex].ResourceIndex;
	if (n >= 0 && m_ChannelsResource[n].nNeverClose)	//���ܹرյ�Ƶ��
		return 1;
	return 0;
}
int Kuiplayerfun::CheckChannel(int nChannelIndex, bool b)
{
	if (this == NULL)
		return 0;

	if (nChannelIndex < 0 || nChannelIndex >= m_nActivateChannels)
		return 0;
	if (IsNeverCloseActivateChannel(nChannelIndex))	//���ܹرյ�Ƶ�����ܸı�״̬
		return 0;
	if  (!m_pActivateChannel) return -1;
	if (m_pActivateChannel[nChannelIndex].bSubscribe == b)
		return 1;
	m_pActivateChannel[nChannelIndex].bSubscribe = b;
	SendChannelSubscribe(nChannelIndex, b);
	int n = m_pActivateChannel[nChannelIndex].ResourceIndex;
	/*if (n >= 0)
	{
		char Buffer[32];
		strncpy(Buffer, b ? m_ChannelsResource[n].cMenuText : m_ChannelsResource[n].cMenuDeactivateText, 32);
		strcat(Buffer, b ? "��" : "�ر�");
		SystemMessageArrival(Buffer, strlen(Buffer));
	}*/
	return 1;
}

bool Kuiplayerfun::ReplaceSpecialField(char* szDest, char* szSrc)  //�滻�ļ���szSrc ΪUI�µ� formatname  �ܲ��ܵ���Ƶ�� �Ϳ�������
{
	char s_keyDests [][__X_NAME_LEN_] =
	{
		"NULL", //����0
		"NULL", //����1
		"NULL",//����2
		"NULL",//3����
		"NULL",//4MS
		"NULL",//5GM
		"NULL",//����
		"NULL",//��
		"NULL",//��
		"NULL",//��
		"NULL",//��
	};

	KUiPlayerTeam	Team;
	Team.nTeamServerID = -1; //û�����
	g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (uintptr_t)&Team, 0);
	if (Team.nTeamServerID >= 0) //����ж�����
	{
		sprintf(s_keyDests[0], "%d", Team.nTeamServerID);
	}
	KUiPlayerBaseInfo self;
	self.nCurFaction = -1;//���õ�ǰû����������
	g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (uintptr_t)&self, 0);
	if (self.nCurFaction >= 0) //�����������   ���ɺ� ���ɲ��ܲ��棡
	{
		sprintf(s_keyDests[1], "%d", self.nCurFaction);
	}
	else if (self.nCurTong!=0) //��������
	{
		sprintf(s_keyDests[2], "%u", self.nCurTong);// �����
	}

	if (self.nMissionGroup >= 0)
	{
		sprintf(s_keyDests[4], "%d", self.nMissionGroup);
	}

	if (self.nCurPinDao >= 0) //�������ʱƵ��
	{
		sprintf(s_keyDests[6], "%d", self.nCurPinDao);
	}


	int nKeySize = sizeof(s_keys) / __X_NAME_LEN_;
	char key[__X_NAME_LEN_];

	if (this)
	{
		int n = 0;
		int m = 0;
		int nKey = -1;
		while(szSrc[n] != 0)
		{
			if (szSrc[n] == '<')
			{
				nKey = 0;
				memset(key, 0, sizeof(key));  //��ʼ��key��ǰN���ֽ� �滻��Ϊ0
			}
			else if (szSrc[n] == '>')
			{
				for (int i = 0; i < nKeySize; ++i) //��5���ؼ���
				{
					if (strcmp(s_keys[i], key) == 0)	//strcmpi ֧�ֵĹؼ���  �ֽ�����ȣ� ���ִ�дд
					{
						if (s_keyDests[i][0] == 0)  //û�����ݷ��ط�
							return false;
						strcpy(szDest + m, s_keyDests[i]);
						m += strlen(s_keyDests[i]);
						nKey = -1;
						break;
					}
				}

				if (nKey >= 0)	//��֧�ֵĹؼ���
				{
					strcpy(szDest + m, key);
					m += nKey;
					nKey = -1;
					return false;
				}
			}
			else
			{
				if (nKey >= 0)
				{
					key[nKey++] = szSrc[n];
				}
				else
				{
					szDest[m++] = szSrc[n];
				}
			}

			n++;
		}

		szDest[m] = 0;
	}

	return true;
}

int Kuiplayerfun::FindActivateChannelResourceIndex(char* cTitle)
{
	if (cTitle == NULL || cTitle[0] == 0)
		return -1;
	if  (!m_pActivateChannel) return -1;
	for (int n = 0; n < m_nActivateChannels; n++)
	{
		if (m_pActivateChannel[n].ResourceIndex < 0)
			continue;
		if (strcmp(cTitle, m_ChannelsResource[m_pActivateChannel[n].ResourceIndex].cTitle) == 0)
			return m_pActivateChannel[n].ResourceIndex;
	}
	return -1;
}


int Kuiplayerfun::FindActivateChannelIndex(unsigned long nChannelID) //�����Ѿ������Ƶ�� ��ID����
{
	for (int n = 0; n < m_nActivateChannels; n++)
	{
		if (m_pActivateChannel[n].nChannelID == nChannelID)
			return n;
	}
	return -1;
}

int Kuiplayerfun::FindActivateChannelIndex(char* cTitle) //�����Ѿ������Ƶ�� �����ֲ���
{
	if (cTitle == NULL || cTitle[0] == 0)
		return -1;
	if  (!m_pActivateChannel) return -1;
	for (int n = 0; n < m_nActivateChannels; n++)
	{
		if (m_pActivateChannel[n].ResourceIndex < 0)
			continue;
		if (strcmp(cTitle, m_ChannelsResource[m_pActivateChannel[n].ResourceIndex].cTitle) == 0)
			return n;
	}
	return -1;
}

int Kuiplayerfun::FindActivateChannelIndexById(unsigned long nChannelID) //�����Ѿ������Ƶ�� ��ID����
{
	if  (!m_pActivateChannel) return -1;
	for (int n = 0; n < m_nActivateChannels; n++)
	{
		if (m_pActivateChannel[n].nChannelID == nChannelID)
			return n;
	}
	return -1;
}

int Kuiplayerfun::FindChannelResourceIndex(char* cTitle)
{
	if (cTitle == NULL || cTitle[0] == 0)
		return -1;

	for (int n = 0; n < m_nChannelsResource; n++)
	{
		if (strcmp(cTitle, m_ChannelsResource[n].cTitle) == 0)
			return n;
	}
	return -1;
}

int	Kuiplayerfun::AddActivateChannel(const KChannelActivateInfo& Item)  //����Ƶ��
{
	int n = FindActivateChannelIndexById(Item.nChannelID);
	if (n >= 0)
	   return n;
	//����Ƶ��
	KChannelActivateInfo* pChannels = new KChannelActivateInfo[m_nActivateChannels + 1];
	if (m_nActivateChannels > 0 && m_pActivateChannel)
	{
		memcpy(pChannels, m_pActivateChannel, m_nActivateChannels * sizeof(KChannelActivateInfo));
		m_nActivateChannels = ReleaseActivateChannelAll();
	}
	m_pActivateChannel = pChannels;
	pChannels = m_pActivateChannel + m_nActivateChannels;
	*pChannels = Item;
	pChannels->uLastSendMsgTime = IR_GetCurrentTime() - m_ChannelsResource[Item.ResourceIndex].uSendMsgInterval;	//ȷ����һ���϶��ܷ�
	pChannels->uLeftSendMsgNum = 0;
	pChannels->uBufferOffset   = 0;
	memset(pChannels->Buffer, 0, sizeof(pChannels->Buffer));
	m_nActivateChannels++;

	return m_nActivateChannels - 1;
}
// ��Ƶ��
void Kuiplayerfun::OpenChannel(char* channelName, unsigned long nChannelID, BYTE cost)  //���һ����Ƶ��
{
	if (nChannelID == -1)
		return;

	if (this)
	{
		char ChannelName[__X_NAME_LEN_];
		int nChannelIndex = -1;
		for (int n = 0; n < m_nChannelsResource; n++)
		{
			//ChannelName[0];
			if (m_ChannelsResource[n].cFormatName[0] == 0)
				return;
			if (!ReplaceSpecialField(ChannelName, m_ChannelsResource[n].cFormatName))
				continue;
			if (ChannelName[0] == 0)
				continue;

			//messageBox(ChannelName,ChannelName);

			if (strcmp(channelName, U2G(ChannelName).c_str()) == 0)
			{
				//messageBox(channelName,ChannelName);
				nChannelIndex = n;
				break;
			}
		}

		if (nChannelIndex >= 0)
		{
			KChannelActivateInfo Info;
			Info.nChannelID = nChannelID;
			Info.cost = cost;
			Info.ResourceIndex = nChannelIndex;
			Info.bSubscribe = true;
			SendChannelSubscribe(AddActivateChannel(Info), true);  //�����µ�Ƶ��
		}
	}
}

void Kuiplayerfun::update(float delta)
{
	//messageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	}
}

//void Kuiplayerfun::draw()
//{
//	//messageBox("����ѭ��","draw");
//}
