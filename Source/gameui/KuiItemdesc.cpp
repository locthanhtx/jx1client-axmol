#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiItemdesc.h"
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
#include "gamecore/KBuySell.h"

extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;
using namespace ui;
KuiItemdesc::KuiItemdesc()
{
	isOpen = false;
	memset(&m_ItemData,0,sizeof(KUiItemdObject));
	ptitleLabel       = NULL;
	potherLabel       = NULL;
	sprItem       = NULL;
	pdescLabel        = NULL;
    pnameLabel        = NULL;
	pbaseLabel        = NULL; //Thuộc tính cơ bản
	prqLabel          = NULL;   //thuộc tính yêu cầu
	pmagicLabel       = NULL;//Thuộc tính phép
	colorLayer        = NULL;
	red               = NULL;
	m_ShowKind        = 0;
    m_pUiLayer = NULL;
    listView   = NULL;
	//m_nMoney = 0;
	//m_nXu = 0;
	//m_nNumObjects = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	//m_ItemIndex = 0;
}

KuiItemdesc::~KuiItemdesc()
{
	isOpen = false;
	ptitleLabel = NULL;
	//m_ItemIndex = 0;
	colorLayer        = NULL;
	if  (red)
	    red->release();
	//red               = NULL;
}

KuiItemdesc * KuiItemdesc::create(int nshopIndex,KUiItemdObject nItemData,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiItemdesc * popLayer = KuiItemdesc::create();
	popLayer->m_ItemData   = nItemData;
	popLayer->addshopdesc(nshopIndex);
	popLayer->isOpen = true;
	return popLayer;
}
KuiItemdesc * KuiItemdesc::create(KUiItemdObject nItemData,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun,int nShowKind)
{
	KuiItemdesc * popLayer = KuiItemdesc::create();
	popLayer->m_ItemData   = nItemData;
	popLayer->m_ShowKind   = nShowKind;
	popLayer->addDialogData();
	popLayer->setcoloseButton(callbackListener,callfun);

	popLayer->isOpen = true;
	return popLayer;
}

bool KuiItemdesc::init()
{
	if ( !Layer::init() ){
		return false;
	}
	Size winSize = ax::Director::getInstance()->getWinSize();
	Color4B color(33,56,41,180);
	colorLayer = LayerColor::create(color);
	//colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setAnchorPoint(ax::Vec2(0,0));
	Size nColorSize;
	nColorSize.width = 315;
	nColorSize.height= 315;
	colorLayer->setContentSize(nColorSize);
	this->addChild(colorLayer);


	ParentNode_Team= colorLayer;
	colorLayer->setPosition(ax::Vec2(winSize.width/2,winSize.height/2));
	Size contentSize = colorLayer->getContentSize();
	m_size   = contentSize;
	m_origin = colorLayer->getPosition();

	this->scheduleUpdate();
	auto listener = ax::EventListenerTouchOneByOne::create();
ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void KuiItemdesc::addshopdesc(int nshopIndex)
{
	if  (!g_pCoreShell) return;

	//if (m_ItemData.uId<=0 || m_ItemData.uId>=MAX_ITEM) return;
	int nidx     = m_ItemData.uId;
	int nIndex   = BuySell.GetItemIndex(nshopIndex,nidx);
	KItem* pItem = BuySell.GetItemidx(nIndex);

	if  (pItem==NULL)
	{
	  Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng chọn vật phẩm");
	  return;
    }

	ax::Color3B nItemColor;
	ax::Color3B __ccBlue = {100,100,255};
	int nItemKind=0;
	nItemKind = pItem->getItemKind();

	if  (nItemKind==1)
		nItemColor = ax::Color3B::YELLOW; //黄色
	else if (nItemKind==2)
		nItemColor = __ccBlue;//ax::Color3B::BLUE;   //蓝色  R=100 G=100 B=255
	else if (nItemKind==3)
		nItemColor = ax::Color3B::MAGENTA;//品红
	else
		nItemColor = ax::Color3B::WHITE;

	int  nEnChance  = pItem->GetEnChance();
	int  nItemGenre = pItem->GetGenre();
	int  nDetailType= pItem->GetDetailType();
	int  nParticularType =pItem->GetParticular();
	int  nItemLevel = pItem->GetLevel();
	int  nIsPlatina = pItem->GetIsPlatima();
	int  nGenKind   = m_ItemData.nGenkind;
	int  i;
	char nItemName[80],pszMsg[128];
	ZeroMemory(&pszMsg,sizeof(pszMsg));
	sprintf(nItemName, "%s", pItem->GetName());
	//Left=24
	//Top=72
	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;

	Size nDsize;
	float offSet  = 0;
	nDsize.width  = 250;
	nDsize.height = 0;
	int nActive = 0;
	if (m_ItemData.nGenkind == UOC_EQUIPTMENT)
	{//trang bị
		nActive = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetActiveAttribNum(m_ItemData.uId);

		if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetIfActive())
		{//nếu đang mở
			nActive = 3;
		}
	}
	//6 dòng thuộc tính
	//pmagicLabel = Label::createWithTTF(UTEXT(magicMsg,1).c_str(),UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
	pmagicLabel = Node::create();
	pmagicLabel->setAnchorPoint(ax::Vec2(0,0));
	//pmagicLabel->setPosition(ax::Vec2(0,5));
	ParentNode_Team->addChild(pmagicLabel,1);

	char magicMsg[256];
	ZeroMemory(&magicMsg,sizeof(magicMsg));
	int nTempOff =0;
	for (i = 0; i < 6; ++i)  //Đồ xanh, hoàng kim, tím
	{
		if (nItemGenre != item_equip)
			break;
		int  nAttribType = pItem->GetAttribType(i);
		if (!nAttribType) //Thuộc tính chưa có số liệu nAttribType = int
		{
			if (pItem->IsPurple() && i< pItem->GetPoint())  //打孔了
			{//紫装
				sprintf(magicMsg, "Chưa khảm nạm");
				Label *tempLabel = Label::createWithTTF(UTEXT(magicMsg,1).c_str(),UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
				tempLabel->setAnchorPoint(ax::Vec2(0,0));
				nTempOff+=tempLabel->getContentSize().height-10;//DANG EDIT TAI DAY
				tempLabel->setPosition(ax::Vec2(0,nTempOff));
				pmagicLabel->addChild(tempLabel);
			}
			continue;
		}
		//char* pszInfo = (char *)g_MagicDesc.GetDesc(&(pItem->getaryMagicAttrib(i)));
		//char pszInfo[512];
		//ZeroMemory(pszInfo,sizeof(pszInfo));
		KMagicAttrib nCurAttrib;
		ZeroMemory(&nCurAttrib,sizeof(KMagicAttrib));
		nCurAttrib = pItem->getaryMagicAttrib(i);
		//g_MagicDesc.GetDesc_New(pszInfo,&(nCurAttrib));
		char pszInfo[512];
		ZeroMemory(pszInfo,sizeof(pszInfo));
		g_MagicDesc.GetDesc_New(pszInfo,&nCurAttrib);
		if (!pszInfo||!pszInfo[0])
			continue;
		if ((i & 1) == 0) //i=1 mặc định i là số lẻ
		{//1 3 5 số lẻ
			sprintf(magicMsg,  "%s", pszInfo);
			Label *tempLabel = Label::createWithTTF(magicMsg,UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
			tempLabel->setAnchorPoint(ax::Vec2(0,0));
			nTempOff -=tempLabel->getContentSize().height-10;
			tempLabel->setPosition(ax::Vec2(0,nTempOff));
			tempLabel->setColor(nItemColor);
			pmagicLabel->addChild(tempLabel);
		}
		else
		{//2 4 6 số chẵn
			if ((i>>1) < nActive) // nếu được kích hoạt
			{
				sprintf(magicMsg,  "%s", pszInfo);
				Label *tempLabel = Label::createWithTTF(UTEXT(magicMsg,1).c_str(),UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
				tempLabel->setAnchorPoint(ax::Vec2(0,0));
				nTempOff -=tempLabel->getContentSize().height-10;
				tempLabel->setPosition(ax::Vec2(0,nTempOff));
				tempLabel->setColor(nItemColor);
				//tempLabel->setColor(ax::Color3B::GRAY);
				pmagicLabel->addChild(tempLabel);
			}
			else
			{//chưa kích hoạt
				sprintf(magicMsg,  "%s", pszInfo);
				Label *tempLabel = Label::createWithTTF(UTEXT(magicMsg,1).c_str(),UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
				tempLabel->setAnchorPoint(ax::Vec2(0,0));
				nTempOff -=tempLabel->getContentSize().height-10;
				tempLabel->setPosition(ax::Vec2(0,nTempOff));
				//tempLabel->setColor(nItemColor);
				tempLabel->setColor(ax::Color3B::GRAY);
				pmagicLabel->addChild(tempLabel);
			}
		}
	}
	Size nTemp;
	nTemp.width  = m_size.width;
	nTemp.height = -nTempOff;
	pmagicLabel->setContentSize(nTemp);
	pmagicLabel->setPosition(ax::Vec2(0,nTemp.height));
	//---------------------------------------------
	offSet +=pmagicLabel->getContentSize().height-10;
	//6 thuộc tính yêu cầu
	char reqMsg[256];
	ZeroMemory(&reqMsg,sizeof(reqMsg));
	nTempOff = 0;
	int  nreqCount = pItem->getReqCount();
	//prqLabel = Label::createWithTTF(UTEXT(reqMsg,1).c_str(),"fonts/gb_song.ttf",12,nDsize,TextHAlignment::CENTER);//Arial
	prqLabel = Node::create();
	prqLabel->setAnchorPoint(ax::Vec2(0,0));
	//prqLabel->setPosition(ax::Vec2(0,offSet));
	ParentNode_Team->addChild(prqLabel,1);
	for (i = 0; i < nreqCount; ++i)
	{
		int nreqnAttribType = pItem->GetItemReqType(i);
		if (!nreqnAttribType)
			continue;
		//char* pszInfo = (char *)g_MagicDesc.GetDesc(&pItem->getaryRequireAttrib(i));
		//char pszInfo[512];
		//ZeroMemory(pszInfo,sizeof(pszInfo));
		KMagicAttrib nCurAttrib;
		ZeroMemory(&nCurAttrib,sizeof(KMagicAttrib));
		nCurAttrib = pItem->getaryRequireAttrib(i);
		//g_MagicDesc.GetDesc_New(pszInfo,&(nCurAttrib));
		char pszInfo[512];
		ZeroMemory(pszInfo,sizeof(pszInfo));
		g_MagicDesc.GetDesc_New(pszInfo,&nCurAttrib);
		if (!pszInfo || !pszInfo[0])
			continue;
		//strcat(reqMsg, pszInfo);
		sprintf(reqMsg,  "%s", pszInfo);
		//		Label *tempLabel = Label::createWithTTF(UTEXT(reqMsg,1).c_str(),UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
		Label *tempLabel = Label::createWithTTF(reqMsg,UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
		tempLabel->setAnchorPoint(ax::Vec2(0,0));
		nTempOff -=tempLabel->getContentSize().height-10;
		tempLabel->setPosition(ax::Vec2(0,nTempOff));
		prqLabel->addChild(tempLabel);

		if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.EnoughAttrib(&nCurAttrib))
			tempLabel->setColor(ax::Color3B::RED);
	}

	if (nreqCount)
	{//
		nTemp.width  = m_size.width;
		nTemp.height = -nTempOff;
		prqLabel->setContentSize(nTemp);
		prqLabel->setPosition(ax::Vec2(0,nTemp.height+offSet));
		offSet +=prqLabel->getContentSize().height-10;
	}
	///
	//7 thuộc tính cơ bản
	char baseMsg[256];
	ZeroMemory(&baseMsg,sizeof(baseMsg));
	int  nbaseCount = pItem->getBaseCount();
	for ( i = 0; i < nbaseCount; i++)  //基本属性
	{
		int nAttribType = pItem->GetItemBaseType(i);
		if (!nAttribType)
			continue;
		if (nAttribType == magic_durability_v)
		{
			char szDurInfo[64]={0};
			int  m_nCurrentDur = pItem->GetDurability();
			if (m_nCurrentDur == -1)
			sprintf(szDurInfo,  "%s", UTEXT("Vĩnh cửu",1).c_str());
			else
			{
				if  (pItem->GetDurability()<=3 && pItem->GetDurability()>0)
				{
					if (nItemGenre==item_equip && nDetailType==equip_mask)
						sprintf(szDurInfo, UTEXT("Độ bền: %d/%d",1).c_str(), pItem->GetDurability(), pItem->GetMaxDurability());
					else
						sprintf(szDurInfo, UTEXT("Độ bền (Yêu cầu sửa chữa)：%d/%d",1).c_str(), pItem->GetDurability(), pItem->GetMaxDurability());
				}
				else
				{
					if (nItemGenre==item_equip && nDetailType==equip_mask)
						sprintf(szDurInfo, UTEXT("Độ bền: %d/%d",1).c_str(), pItem->GetDurability(), pItem->GetMaxDurability());
					else
						sprintf(szDurInfo, UTEXT("Độ bền: %d/%d",1).c_str(), pItem->GetDurability(), pItem->GetMaxDurability());
				}
			}
			strcat(baseMsg, szDurInfo);
			if (i==nbaseCount-1)
				break;
			strcat(baseMsg,"\n");
		}
		else
		{//其他属性
			//char pszInfo[512];
			//char* pszInfo = (char *)g_MagicDesc.GetDesc(&(pItem->getaryBaseAttrib(i)));
			//char pszInfo[512];
			//ZeroMemory(pszInfo,sizeof(pszInfo));
			KMagicAttrib nCurAttrib;
			ZeroMemory(&nCurAttrib,sizeof(KMagicAttrib));
			nCurAttrib = pItem->getaryBaseAttrib(i);
			//g_MagicDesc.GetDesc_New(pszInfo,&(nCurAttrib));
#ifdef WIN32
			char* pszInfo = (char *)g_MagicDesc.GetDesc(&nCurAttrib);
#else
			//char* pszInfo = (char *)g_MagicDesc.GetDesc_(&nCurAttrib);
			char pszInfo[512];
			ZeroMemory(pszInfo,sizeof(pszInfo));
			g_MagicDesc.GetDesc_New(pszInfo,&nCurAttrib);
#endif
			if (!pszInfo || !pszInfo[0])
				continue;
			strcat(baseMsg,pszInfo);
			if (i==nbaseCount-1)
				break;
			strcat(baseMsg,"\n");
		}
	}

	if (nbaseCount)
	{
		//pbaseLabel = Label::createWithTTF(UTEXT(baseMsg,1).c_str(),UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
		pbaseLabel = Label::createWithTTF(baseMsg,UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
		pbaseLabel->setAnchorPoint(ax::Vec2(0,0));
		pbaseLabel->setPosition(ax::Vec2(0,offSet));
		ParentNode_Team->addChild(pbaseLabel,1);
		offSet +=pbaseLabel->getContentSize().height-10;
	}
	//说明　五行
	char szIntro[512],strSeries[64];
	ZeroMemory(&szIntro,sizeof(szIntro));
	ZeroMemory(&strSeries,sizeof(strSeries));
	nTempOff = 0;
	pdescLabel =Node::create();
	pdescLabel->setAnchorPoint(ax::Vec2(0,0));
	//prqLabel->setPosition(ax::Vec2(0,offSet));
	ParentNode_Team->addChild(pdescLabel,1);
	if  (nItemGenre==item_equip||(nItemGenre==item_mine &&
		(nParticularType==150 ||nParticularType==152 ||nParticularType==154 ||
		(nParticularType>=200&&nParticularType<=205)
		)))
	{//是装备就显示五行
		int  nSeries = pItem->GetSeries();
		switch(nSeries)
		{
		case series_metal:
			sprintf(strSeries, "Thuộc tính ngũ hành: Kim");
			break;
		case series_wood:
			sprintf(strSeries, "Thuộc tính ngũ hành: Mộc");
			break;
		case series_water:
			sprintf(strSeries, "Thuộc tính ngũ hành: Thủy");
			break;
		case series_fire:
			sprintf(strSeries, "Thuộc tính ngũ hành: Hỏa");
			break;
		case series_earth:
			sprintf(strSeries, "Thuộc tính ngũ hành: Thổ");
			break;
		}
		Label *tempLabel = Label::createWithTTF(UTEXT(strSeries,1).c_str(),UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
		tempLabel->setAnchorPoint(ax::Vec2(0,0));
		nTempOff -=tempLabel->getContentSize().height-10;
		tempLabel->setPosition(ax::Vec2(0,nTempOff));
		tempLabel->setColor(ax::Color3B::GREEN);
		pdescLabel->addChild(tempLabel);
	}

	//char szIntro[512];pdescLabel
	sprintf(szIntro, "%s", pItem->GetItmeInfo());
	if (szIntro[0])
	{//mô tả vật phẩm
		sprintf(szIntro, "%s", UTEXT(szIntro,1).c_str());
		int newLen = TEncodeText(szIntro,strlen(szIntro));
		Label *tempLabel = Label::createWithTTF(szIntro,UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
		tempLabel->setAnchorPoint(ax::Vec2(0,0));
		nTempOff -=tempLabel->getContentSize().height-10;
		tempLabel->setPosition(ax::Vec2(0,nTempOff));
		tempLabel->setColor(ax::Color3B::YELLOW);
		pdescLabel->addChild(tempLabel);
	}
	nTemp.width  = m_size.width;
	nTemp.height = -nTempOff;
	pdescLabel->setContentSize(nTemp);
	pdescLabel->setPosition(ax::Vec2(0,nTemp.height+offSet));
	offSet +=pdescLabel->getContentSize().height-10;
	//các vật phẩm khác--------------------------------------------
	char otherzMsg[256];
	ZeroMemory(&otherzMsg,sizeof(otherzMsg));
	int nIsBang = pItem->GetIsBang();
	if (nIsBang<=0)
		strcat(otherzMsg,"Chưa khóa");
	else
		strcat(otherzMsg,"Đã Khóa");

	int  nUseMap = pItem->GetUseMapIdx();
	if (nUseMap>0)
	{
		strcat(otherzMsg, "\n");
		char nMapKey[32]={0},nMapName[64]={0};
		ZeroMemory(&nMapName,sizeof(nMapName));
		sprintf(nMapKey,"%d_name",nUseMap);
		g_NpcMapDropRate.GetString("List",nMapKey,"Tạm thời chưa có số liệu",nMapName,sizeof(nMapName));

		if (nItemGenre == item_equip)//锁魂
			strcat(otherzMsg, "Trang bị đã Đính: ");
		else
			strcat(otherzMsg, "Trang bị đã Đính: ");

		int  nUseKind = pItem->GetUseKind();
		if (nUseKind==0)  //锁魂地图
			strcat(otherzMsg, nMapName);
		else
		{
			sprintf(nMapName,"Cấp <%d",nUseMap);
			strcat(otherzMsg, nMapName);
		}
	}

	if (m_ItemData.uGenre==UOC_NPC_SHOP)
	{//商店
		strcat(otherzMsg,"\n");
		char szPrice[32]={0},nInfo[4]={0};
		int  ncSellModel =pItem->GetcModel();
		if (ncSellModel==moneyunit_money || ncSellModel==moneyunit_moneya)
			sprintf(nInfo, "%s", "Đồng");
		else if (ncSellModel==moneyunit_xu)
			sprintf(nInfo, "%s", "Bạc");
		else
			sprintf(nInfo, "%s", "Điểm");
		int  nPrice = pItem->GetPrice();
		if (nPrice/BUY_SELL_SCALE_DE < 10000)//万
			sprintf(szPrice, "Giá trị tài phú binh giáp: %d%s",nPrice/BUY_SELL_SCALE_DE,nInfo);
		else
		{
			int vPrice = (nPrice/BUY_SELL_SCALE_DE)/10000;//TakeTrader(TakeTrader(m_CommonAttrib.nPrice,nPriceScale), 10000);
			int lPrice = (nPrice/BUY_SELL_SCALE_DE)%10000;//TakeRemainder(TakeTrader(m_CommonAttrib.nPrice, nPriceScale),10000);
			if (lPrice == 0)
				sprintf(szPrice, "Giá trị tài phú binh giáp: %d vạn %s lượng", vPrice,nInfo);
			else
				sprintf(szPrice, "Giá trị tài phú binh giáp: %d vạn %d%s lượng", vPrice,lPrice,nInfo);
		}
		strcat(otherzMsg, szPrice);
		//strcat(otherzMsg,"\n");
	}

	potherLabel = Label::createWithTTF(otherzMsg,UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
	potherLabel->setAnchorPoint(ax::Vec2(0,0));
	potherLabel->setPosition(ax::Vec2(0,offSet));
	ParentNode_Team->addChild(potherLabel,1);
	offSet +=potherLabel->getContentSize().height-10;
	//物品名称　等级
	if (nEnChance)
	{//有加成的
		if (nItemGenre == 0)
		{
			char sItemName[80]={0};
			sprintf(sItemName,"%s+%d",nItemName,nEnChance);//强化
			char    TextLevel[128]={0};
			int     LevelItem = nItemLevel;
			if(LevelItem > 10)
			{
				if (LevelItem < 100)
				{
					if (LevelItem%10 == 0)
						sprintf(TextLevel, "%s[Cấp %d]", sItemName, LevelItem/(LevelItem/10));
					else
						sprintf(TextLevel, "%s[Cấp %d]", sItemName, LevelItem%10);
				}
				else if (LevelItem < 1000)
				{
					if (LevelItem%100 == 0)
						sprintf(TextLevel, "%s[Cấp %d]", sItemName, LevelItem/(LevelItem/100));
					else
						sprintf(TextLevel, "%s[Cấp %d]", sItemName, LevelItem%100);
				}
			}
			else
				sprintf(TextLevel, "%s[Cấp %d]", sItemName, LevelItem);
			strcat(pszMsg,TextLevel);
		}
		else
		{//非装备
			strcat(pszMsg,nItemName);
		}
	}
	else
	{//无加成的
		if (nItemGenre == 0)
		{//是装备
			char sItemName[80]={0};
			sprintf(sItemName,"%s",nItemName);
			char    TextLevel[128]={0};
			int     LevelItem = nItemLevel;

			if(LevelItem > 10)
			{
				if (LevelItem < 100)
				{
					if (LevelItem%10 == 0)
						sprintf(TextLevel, "%s[Cấp %d]", sItemName,LevelItem/(LevelItem/10));
					else
						sprintf(TextLevel, "%s[Cấp %d]", sItemName,LevelItem%10);
				}
				else if (LevelItem < 1000)
				{
					if (LevelItem%100 == 0)
						sprintf(TextLevel, "%s[Cấp %d]", sItemName, LevelItem/(LevelItem/100));
					else
						sprintf(TextLevel, "%s[Cấp %d]", sItemName, LevelItem%100);
				}
			}
			else
				sprintf(TextLevel, "%s[Cấp %d]", sItemName, LevelItem);
			strcat(pszMsg,TextLevel);
		}
		else
		{//非装备
			strcat(pszMsg,nItemName);  //物品名字
			if (nItemLevel>10)
			{
				char nMsg[32]={0};
				sprintf(nMsg,"[Cấp %d]",nItemLevel);
				strcat(pszMsg, nMsg);
			}
		}
	}
	//
	ptitleLabel = Label::createWithTTF(UTEXT(pszMsg,1).c_str(),UI_GAME_FONT_DEFAULT_VN,12,nDsize,TextHAlignment::CENTER);//Arial
	ptitleLabel ->setColor(nItemColor);
	ptitleLabel->setAnchorPoint(ax::Vec2(0,0));
	ptitleLabel->setPosition(ax::Vec2(0,offSet));
	ParentNode_Team->addChild(ptitleLabel,1);

	m_size.height = offSet+ptitleLabel->getContentSize().height-10;
	ParentNode_Team->setContentSize(m_size);
	//Di chuyển vị trí của toàn bộ Node
	if (m_ItemData.nRect.origin.y+m_size.height>visibleSize.height) //đứng đầu
	{//Nếu nó vượt quá phạm vi hiển thị
		m_ItemData.nRect.origin.y = visibleSize.height-m_size.height-10;
	}
	ParentNode_Team->setPosition(ax::Vec2(m_ItemData.nRect.origin.x-m_size.width,m_ItemData.nRect.origin.y));
	//Tạo một hành động để thay đổi màu sprite
	red = TintBy::create(0.2,0,-255,-255); //持续时间+颜色
	red->retain();

	//messageBox("成功","成功");
}

void KuiItemdesc::addDialogData()
{
	if  (!g_pCoreShell) return;
	if (m_ItemData.uId<=0 || m_ItemData.uId>=MAX_ITEM) return;

    Menu* itemInfoMenu = Menu::create(NULL);
    itemInfoMenu->setAnchorPoint(ax::Vec2(0,0));
    //itemInfoMenu->setPosition(ax::Vec2(0,0));

	ax::Color3B nItemColor;
	int nItemKind=0;
	    nItemKind = Item[m_ItemData.uId].getItemKind();
	ax::Color3B __ccBlue = {100,100,255};
    if  (nItemKind==1){
		nItemColor = ax::Color3B::YELLOW; //TB HOANG KIM
	}else if (nItemKind==2){
		nItemColor = __ccBlue;//ax::Color3B::BLUE;   //TB XANH
	}else if (nItemKind==3){
		nItemColor = ax::Color3B::MAGENTA;//TB TIM
	}else{
		nItemColor = ax::Color3B::WHITE;
	}


	int  nEnChance  = Item[m_ItemData.uId].GetEnChance();
	int  nItemGenre = Item[m_ItemData.uId].GetGenre();
	int  nDetailType= Item[m_ItemData.uId].GetDetailType();
	int  nParticularType =Item[m_ItemData.uId].GetParticular();
	int  nItemLevel = Item[m_ItemData.uId].GetLevel();
	int  nIsPlatina = Item[m_ItemData.uId].GetIsPlatima();



	int  nGenKind   = m_ItemData.nGenkind;
	int  i;
	char nItemName[80],pszMsg[128];
	ZeroMemory(&pszMsg,sizeof(pszMsg));
	sprintf(nItemName, "%s", Item[m_ItemData.uId].GetName());

	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;
	Size nDsize;
	float offSet  = 0;
	nDsize.width  =m_size.width*2;
	nDsize.height = 0;
	int nActive = 0;
	int nTempOff =0;
	Size nTemp;

	if (m_ItemData.nGenkind == UOC_EQUIPTMENT)
	{//đã trang bị
		nActive = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetActiveAttribNum(m_ItemData.uId);

		if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetIfActive())
		{//如果是打开的
			nActive = 3;
		}
	}

	//cấp item
	if (nEnChance)
	{//có tiền thưởng
		if (nItemGenre == 0)
		{
			char sItemName[80]={0};
			sprintf(sItemName,"%s+%d",nItemName,nEnChance);//强化
			char    TextLevel[128]={0};
			int     LevelItem = nItemLevel;
			if(LevelItem > 10)
			{
				if (LevelItem < 100)
				{
					if (LevelItem%10 == 0)
						sprintf(TextLevel, "%s [Cấp %d]", sItemName, LevelItem/(LevelItem/10));
					else
						sprintf(TextLevel, "%s [Cấp %d]", sItemName, LevelItem%10);
				}
				else if (LevelItem < 1000)
				{
					if (LevelItem%100 == 0)
						sprintf(TextLevel, "%s [Cấp %d]", sItemName, LevelItem/(LevelItem/100));
					else
						sprintf(TextLevel, "%s [Cấp %d]", sItemName, LevelItem%100);
				}
			}
			else
			   sprintf(TextLevel, "%s [Cấp %d]", sItemName, LevelItem);
			strcat(pszMsg,TextLevel);
		}
		else
		{//非装备
			strcat(pszMsg,nItemName);
		}
	}
	else
	{//无加成的
		if (nItemGenre == 0)
		{//是装备
			char sItemName[80]={0};
			sprintf(sItemName,"%s",nItemName);
			char    TextLevel[128]={0};
			int     LevelItem = nItemLevel;

			if(LevelItem > 10)
			{
				if (LevelItem < 100)
				{
					if (LevelItem%10 == 0)
						sprintf(TextLevel, "%s [Cấp %d]", sItemName,LevelItem/(LevelItem/10));
					else
						sprintf(TextLevel, "%s [Cấp %d]", sItemName,LevelItem%10);
				}
				else if (LevelItem < 1000)
				{
					if (LevelItem%100 == 0)
						sprintf(TextLevel, "%s [Cấp %d]", sItemName, LevelItem/(LevelItem/100));
					else
						sprintf(TextLevel, "%s [Cấp %d]", sItemName, LevelItem%100);
				}
			}
			else
				sprintf(TextLevel, "%s [Cấp %d]", sItemName, LevelItem);
			strcat(pszMsg,TextLevel);
		}
		else
		{//非装备
			strcat(pszMsg,nItemName);  //物品名字
			if (nItemLevel>10)
			{
				char nMsg[32]={0};
				sprintf(nMsg,"[Cấp %d]",nItemLevel);
				strcat(pszMsg, nMsg);
			}
		}
	}
    char debug[100];
	char szIntro[512],strSeries[64],strStatus[64];
	ZeroMemory(&szIntro,sizeof(szIntro));
	ZeroMemory(&strStatus,sizeof(strStatus));
	nTempOff = 0;
	pdescLabel =Node::create();
	pdescLabel->setAnchorPoint(ax::Vec2(0,1));
    Size nRenderSize;
    nRenderSize.width  = m_size.width;
    nRenderSize.height = 0;

	//Nếu là vật phẩm đã trang bị
	if (m_ShowKind==10)
	{

        //Dòng 1 -> tên item
		char FullName[136] = "[Đã trang bị]";
		strcat(FullName,pszMsg);

		ptitleLabel = Label::createWithTTF(FullName,UI_GAME_FONT_DEFAULT_VN,14,nDsize,TextHAlignment::CENTER);//Arial UI_GAME_FONT_DEFAULT_VN
		ptitleLabel->setColor(nItemColor);
		ptitleLabel->setAnchorPoint(ax::Vec2(0,0));
		ptitleLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
        nTempOff += ptitleLabel->getContentSize().height;
        nRenderSize.height = ptitleLabel->getContentSize().height/2;
        nRenderSize.width = ptitleLabel->getContentSize().width/2;
        //m_size.height += ptitleLabel->getContentSize().height/2;
        ptitleLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
        MenuItemLabel* tileLabel1 = MenuItemLabel::create(ptitleLabel);
        tileLabel1->setContentSize(nRenderSize);
        itemInfoMenu->addChild(tileLabel1);
        //Dòng 1 -> tên item
        //Dòng 2 mô tả vật phẩm
        sprintf(szIntro, "%s", Item[m_ItemData.uId].GetItmeInfo());
        if (szIntro[0])
        {
            Label *tempLabel = Label::createWithTTF(szIntro,UI_GAME_FONT_DEFAULT_VN, 12,nDsize,TextHAlignment::CENTER);//Arial
            nTempOff += tempLabel->getContentSize().height;
            tempLabel->setAnchorPoint(ax::Vec2(0,0));
            tempLabel->setColor(ax::Color3B::WHITE);
            tempLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
            tempLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
            nRenderSize.height = tempLabel->getContentSize().height/2;
            nRenderSize.width = tempLabel->getContentSize().width/2;
           //m_size.height += tempLabel->getContentSize().height/2;
            MenuItemLabel* tileLabel2 = MenuItemLabel::create(tempLabel);
            tileLabel2->setContentSize(nRenderSize);
            itemInfoMenu->addChild(tileLabel2);
        }

	}else{

        //Dòng 1 -> tên item
		//ptitleLabel = Label::createWithTTF(pszMsg,"",15,nDsize,TextHAlignment::CENTER);//Arial
		ptitleLabel = Label::createWithTTF(pszMsg,UI_GAME_FONT_DEFAULT_VN,14,nDsize,TextHAlignment::CENTER);//Arial
        nTempOff += ptitleLabel->getContentSize().height;
		ptitleLabel ->setColor(nItemColor);
		ptitleLabel->setAnchorPoint(ax::Vec2(0,0));
		ptitleLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
        nRenderSize.height = ptitleLabel->getContentSize().height/2;
        nRenderSize.width = ptitleLabel->getContentSize().width/2;
        //m_size.height += ptitleLabel->getContentSize().height/2;
        ptitleLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
        MenuItemLabel* tileLabel1 = MenuItemLabel::create(ptitleLabel);
        tileLabel1->setContentSize(nRenderSize);
        itemInfoMenu->addChild(tileLabel1);
        //Dòng 1 -> tên item
        //Dòng 2 mô tả vật phẩm
        sprintf(szIntro, "%s", Item[m_ItemData.uId].GetItmeInfo());
        if (szIntro[0])
        {
            Label *tempLabel = Label::createWithTTF(szIntro,UI_GAME_FONT_DEFAULT_VN, 13,nDsize,TextHAlignment::CENTER);//Arial
            nTempOff += tempLabel->getContentSize().height;
            tempLabel->setAnchorPoint(ax::Vec2(0,0));
            tempLabel->setColor(ax::Color3B::WHITE);
            tempLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
            nRenderSize.height = tempLabel->getContentSize().height/2;
            nRenderSize.width = tempLabel->getContentSize().width/2;
            //m_size.height += tempLabel->getContentSize().height/2;
            MenuItemLabel* tileLabel2 = MenuItemLabel::create(tempLabel);
            tileLabel2->setContentSize(nRenderSize);
            itemInfoMenu->addChild(tileLabel2);
        }

	}

    if  (nItemGenre==item_equip||(nItemGenre==item_mine &&
        (nParticularType==150 ||nParticularType==152 ||nParticularType==154 ||
        (nParticularType>=200&&nParticularType<=205)
        )))
    {
            int  nSeries = Item[m_ItemData.uId].GetSeries();
            Label *tempLabel = Label::createWithTTF("",UI_GAME_FONT_DEFAULT_VN, 13,nDsize,TextHAlignment::CENTER);//Arial
            switch(nSeries)
            {
            case series_metal:
                sprintf(strSeries, "Thuộc tính ngũ hành: Kim");
                tempLabel->setColor(ax::Color3B::YELLOW);
                break;
            case series_wood:
                sprintf(strSeries, "Thuộc tính ngũ hành: Mộc");
                tempLabel->setColor(ax::Color3B::GREEN);
                break;
            case series_water:
                sprintf(strSeries, "Thuộc tính ngũ hành: Thuỷ");
                tempLabel->setColor(ax::Color3B::BLUE);
                break;
            case series_fire:
                sprintf(strSeries, "Thuộc tính ngũ hành: Hoả");
                tempLabel->setColor(ax::Color3B::ORANGE);
                break;
            case series_earth:
                sprintf(strSeries, "Thuộc tính ngũ hành: Thổ");
                tempLabel->setColor(ax::Color3B::GRAY);
                break;
            }

        tempLabel->setString(strSeries);
        nTempOff += tempLabel->getContentSize().height;
        tempLabel->setAnchorPoint(ax::Vec2(0,1));
        tempLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
        tempLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
        nRenderSize.height = tempLabel->getContentSize().height/2;
        nRenderSize.width = tempLabel->getContentSize().width/2;
        //m_size.height += tempLabel->getContentSize().height/2;
        MenuItemLabel* tileLabel3 = MenuItemLabel::create(tempLabel);
        tileLabel3->setContentSize(nRenderSize);
        itemInfoMenu->addChild(tileLabel3);

    }

    //Dong 3 thuoc tinh ngu hanh

    //Thuoc tinh khac
    char baseMsg[256];
	ZeroMemory(&baseMsg,sizeof(baseMsg));
	int  nbaseCount = Item[m_ItemData.uId].getBaseCount();
	for ( i = 0; i < nbaseCount; i++)  //基本属性
	{
		int nAttribType = Item[m_ItemData.uId].GetItemBaseType(i);
		if (!nAttribType)
			continue;
		if (nAttribType == magic_durability_v)
		{
			char szDurInfo[64]={0};
			int  m_nCurrentDur = Item[m_ItemData.uId].GetDurability();
			if (m_nCurrentDur == -1)
            sprintf(szDurInfo, "Vĩnh cửu");
			else
			{
                if  (Item[m_ItemData.uId].GetDurability()<=3 && Item[m_ItemData.uId].GetDurability()>0)
				{
					if (nItemGenre==item_equip && nDetailType==equip_mask)
						sprintf(szDurInfo, "Độ bền: %d/%d", Item[m_ItemData.uId].GetDurability(), Item[m_ItemData.uId].GetMaxDurability());
					else
						sprintf(szDurInfo, "Độ bền(cần sửa chữa): %d/%d", Item[m_ItemData.uId].GetDurability(), Item[m_ItemData.uId].GetMaxDurability());
				}
				else
				{
					if (nItemGenre==item_equip && nDetailType==equip_mask)
						sprintf(szDurInfo, "Số lần SD còn: %d/%d", Item[m_ItemData.uId].GetDurability(), Item[m_ItemData.uId].GetMaxDurability());
					else
						sprintf(szDurInfo, "Độ bền: %d/%d", Item[m_ItemData.uId].GetDurability(), Item[m_ItemData.uId].GetMaxDurability());
				}
			}
			strcat(baseMsg, szDurInfo);
			if (i==nbaseCount-1)
				break;
			strcat(baseMsg,"\n");
		}
		else
		{
			KMagicAttrib nCurAttrib;
			ZeroMemory(&nCurAttrib,sizeof(KMagicAttrib));
			nCurAttrib = Item[m_ItemData.uId].getaryBaseAttrib(i);
            char pszInfo[512];
			ZeroMemory(pszInfo,sizeof(pszInfo));
			g_MagicDesc.GetDesc_New(pszInfo,&nCurAttrib);
			if (!pszInfo || !pszInfo[0])
				continue;
			strcat(baseMsg,pszInfo);
			if (i==nbaseCount-1)
				break;
			strcat(baseMsg,"\n");
		}
	}
    if (nbaseCount)
	{
		pbaseLabel = Label::createWithTTF(baseMsg,UI_GAME_FONT_DEFAULT, 13,nDsize,TextHAlignment::CENTER);//Arial
        nTempOff += pbaseLabel->getContentSize().height;
        pbaseLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
		nRenderSize.height = pbaseLabel->getContentSize().height/2;
        nRenderSize.width = pbaseLabel->getContentSize().width/2;
        pbaseLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
        //m_size.height += nRenderSize.height;
        MenuItemLabel* tileLabel9 = MenuItemLabel::create(pbaseLabel);
        tileLabel9->setContentSize(nRenderSize);
        itemInfoMenu->addChild(tileLabel9);
	}
    //Thuoc tinh khac

    //Yeu cau
    char reqMsg[256];
    ZeroMemory(&reqMsg,sizeof(reqMsg));
    //nTempOff = 0;
    int  nreqCount = Item[m_ItemData.uId].getReqCount();
    for (i = 0; i < nreqCount; ++i)
    {
        int nreqnAttribType = Item[m_ItemData.uId].GetItemReqType(i);
        if (!nreqnAttribType)
            continue;

        KMagicAttrib nCurAttrib;
        ZeroMemory(&nCurAttrib,sizeof(KMagicAttrib));
        nCurAttrib = Item[m_ItemData.uId].getaryRequireAttrib(i);
        char pszInfo[512];
        ZeroMemory(pszInfo,sizeof(pszInfo));
        g_MagicDesc.GetDesc_New(pszInfo,&nCurAttrib);
        if (!pszInfo || !pszInfo[0])
            continue;
        //strcat(reqMsg, pszInfo);
        sprintf(reqMsg,  "%s", pszInfo);
        Label *tempLabel = Label::createWithTTF(reqMsg,UI_GAME_FONT_DEFAULT_VN, 13,nDsize,TextHAlignment::CENTER);//Arial
        nTempOff += tempLabel->getContentSize().height;
        tempLabel->setAnchorPoint(ax::Vec2(0,1));
        tempLabel->enableOutline(Color4B(0, 0, 0, 255), 5);


        if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.EnoughAttrib(&nCurAttrib))
            tempLabel->setColor(ax::Color3B::RED);

        nRenderSize.height = tempLabel->getContentSize().height/2;
        nRenderSize.width = tempLabel->getContentSize().width/2;
        tempLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
        //m_size.height += nRenderSize.height;
        MenuItemLabel* tileLabel4 = MenuItemLabel::create(tempLabel);
        tileLabel4->setContentSize(nRenderSize);
        itemInfoMenu->addChild(tileLabel4);
    }
    // if (nreqCount)
    // {//
    //     nTemp.width  = m_size.width;
    //     nTemp.height = nTempOff;
    //     prqLabel->setContentSize(nTemp);
    //     prqLabel->setPosition(ax::Vec2(0,0));
    // }
    //Yeu cau

    //Thuoc tinh magic
    char magicMsg[256];
	ZeroMemory(&magicMsg,sizeof(magicMsg));

	for (i = 0; i < 6; ++i)  //蓝装，黄装，紫装
	{
		if (nItemGenre != item_equip)
			break;
		int  nAttribType = Item[m_ItemData.uId].GetAttribType(i);
		if (!nAttribType) //这个属性位置的数组中没有数据的 // nAttribType属性类型编码（int数字）
		{
			if (Item[m_ItemData.uId].IsPurple() && i< Item[m_ItemData.uId].GetPoint())  //打孔了
			{//紫装
				sprintf(magicMsg, " Chưa khảm");
				Label *tempLabel = Label::createWithTTF(magicMsg,UI_GAME_FONT_DEFAULT_VN, 13,nDsize,TextHAlignment::CENTER);//Arial
                nTempOff += tempLabel->getContentSize().height;
				tempLabel->setAnchorPoint(ax::Vec2(0,1));
                tempLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
                nRenderSize.height = tempLabel->getContentSize().height/2;
                nRenderSize.width = tempLabel->getContentSize().width/2;
                tempLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
                //m_size.height += nRenderSize.height;
                MenuItemLabel* tileLabel5 = MenuItemLabel::create(tempLabel);
                tileLabel5->setContentSize(nRenderSize);
                itemInfoMenu->addChild(tileLabel5);

			}
			continue;
		}
		KMagicAttrib nCurAttrib;
		ZeroMemory(&nCurAttrib,sizeof(KMagicAttrib));
		nCurAttrib = Item[m_ItemData.uId].getaryMagicAttrib(i);
		char pszInfo[512];
		ZeroMemory(pszInfo,sizeof(pszInfo));
		g_MagicDesc.GetDesc_New(pszInfo,&nCurAttrib);
		if (!pszInfo||!pszInfo[0])
			continue;
		if ((i & 1) == 0) //i=1 默认是激活 奇数判断
		{//1 3 5 奇数
				sprintf(magicMsg,  "%s", pszInfo);
                Label *tempLabel = Label::createWithTTF(magicMsg,UI_GAME_FONT_DEFAULT_VN, 13,nDsize,TextHAlignment::CENTER);
                nTempOff += tempLabel->getContentSize().height;
				tempLabel->setAnchorPoint(ax::Vec2(0,1));
                tempLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
                tempLabel->setColor(nItemColor);
                nRenderSize.height = tempLabel->getContentSize().height/2;
                nRenderSize.width = tempLabel->getContentSize().width/2;
                tempLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
                //m_size.height += nRenderSize.height;
                MenuItemLabel* tileLabel6 = MenuItemLabel::create(tempLabel);
                tileLabel6->setContentSize(nRenderSize);
                itemInfoMenu->addChild(tileLabel6);
		}
		else
		{//2 4 6 偶数
			if ((i>>1) < nActive) // 如果是被激活的
			{
					sprintf(magicMsg,  "%s", pszInfo);
					Label *tempLabel = Label::createWithTTF(magicMsg,UI_GAME_FONT_DEFAULT_VN, 13,nDsize,TextHAlignment::CENTER);//Arial
                    nTempOff += tempLabel->getContentSize().height;
					tempLabel->setAnchorPoint(ax::Vec2(0,1));
                    tempLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
                    tempLabel->setColor(nItemColor);
                    nRenderSize.height = tempLabel->getContentSize().height/2;
                    nRenderSize.width = tempLabel->getContentSize().width/2;
                    tempLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
                    //m_size.height += nRenderSize.height;
                    MenuItemLabel* tileLabel7 = MenuItemLabel::create(tempLabel);
                    tileLabel7->setContentSize(nRenderSize);
                    itemInfoMenu->addChild(tileLabel7);
			}
			else
			{//没有被激活
					sprintf(magicMsg,  "%s", pszInfo);
					Label *tempLabel = Label::createWithTTF(magicMsg,UI_GAME_FONT_DEFAULT_VN, 13,nDsize,TextHAlignment::CENTER);//Arial
                    nTempOff += tempLabel->getContentSize().height;
					tempLabel->setAnchorPoint(ax::Vec2(0,1));
                    tempLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
                    tempLabel->setColor(nItemColor);
                    nRenderSize.height = tempLabel->getContentSize().height/2;
                    nRenderSize.width = tempLabel->getContentSize().width/2;
                    tempLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
                    //m_size.height += nRenderSize.height;
                    MenuItemLabel* tileLabel8 = MenuItemLabel::create(tempLabel);
                    tileLabel8->setContentSize(nRenderSize);
                    itemInfoMenu->addChild(tileLabel8);
			}
		}

	}
    //Thuoc tinh magic
    //Thuoc tinh khac 2

    char otherzMsg[256];
	ZeroMemory(&otherzMsg,sizeof(otherzMsg));
	int nIsBang = Item[m_ItemData.uId].GetIsBang();
	if (nIsBang<=0)
		strcat(otherzMsg,"Chưa khoá");
	else
		strcat(otherzMsg,"Đã khoá");

	int  nUseMap = Item[m_ItemData.uId].GetUseMapIdx();
	if (nUseMap>0)
	{
		strcat(otherzMsg, "\n");
		char nMapKey[32]={0},nMapName[64]={0};
		ZeroMemory(&nMapName,sizeof(nMapName));
		sprintf(nMapKey,"%d_name",nUseMap);
		g_NpcMapDropRate.GetString("List",nMapKey,"Tạm thời chưa có dữ liệu",nMapName,sizeof(nMapName));

		if (nItemGenre == item_equip)//锁魂
			strcat(otherzMsg, "Trang bị đã khoá");
		else
			strcat(otherzMsg, "Vật phẩm đã khoá");

		int  nUseKind = Item[m_ItemData.uId].GetUseKind();
		if (nUseKind==0)  //锁魂地图
			strcat(otherzMsg, nMapName);
		else
		{
			sprintf(nMapName,"Cấp: %d",nUseMap);
			strcat(otherzMsg, nMapName);
		}
	}
    if  (m_ShowKind==1 && Item[m_ItemData.uId].GetSetPrice()>0)
	{//摆摊
		strcat(otherzMsg,"\n");
		char szPrice[32]={0},nInfo[4]={0};
		int  ncSellModel =Item[m_ItemData.uId].GetcModel();
		if (ncSellModel==moneyunit_money || ncSellModel==moneyunit_moneya)
			sprintf(nInfo, "%s", "đồng");
		else if (ncSellModel==moneyunit_xu)
			sprintf(nInfo, "%s", "bạc");
		else
			sprintf(nInfo, "%s", "xu");
		int  nPrice = Item[m_ItemData.uId].GetSetPrice();
		if (nPrice/BUY_SELL_SCALE < 10000)//万
			sprintf(szPrice, "Giá: %d%s",nPrice/BUY_SELL_SCALE_DE,nInfo);
		else
		{
			int vPrice = (nPrice/BUY_SELL_SCALE_DE)/10000;//TakeTrader(TakeTrader(m_CommonAttrib.nPrice,nPriceScale), 10000);
			int lPrice = (nPrice/BUY_SELL_SCALE_DE)%10000;//TakeRemainder(TakeTrader(m_CommonAttrib.nPrice, nPriceScale),10000);
			if (lPrice == 0)
				sprintf(szPrice, "Định giá: %d vạn %s", vPrice,nInfo);
			else
				sprintf(szPrice, "Định giá: %d vạn %d%s", vPrice,lPrice,nInfo);
		}
		strcat(otherzMsg, szPrice);

	}
    if (nGenKind==UOC_NPC_SHOP)
	{//商店
		    strcat(otherzMsg,"\n");
			char szPrice[32]={0},nInfo[4]={0};
			int  ncSellModel =Item[m_ItemData.uId].GetcModel();
			if (ncSellModel==moneyunit_money || ncSellModel==moneyunit_moneya)
				sprintf(nInfo, "%s", "đồng");
			else if (ncSellModel==moneyunit_xu)
				sprintf(nInfo, "%s", "bạc");
			else
				sprintf(nInfo, "%s", "xu");
			int  nPrice = Item[m_ItemData.uId].GetPrice();
			if (nPrice/BUY_SELL_SCALE < 10000)//万
				sprintf(szPrice, "Giá: %d%s",nPrice/BUY_SELL_SCALE_DE,nInfo);
			else
			{
					int vPrice = (nPrice/BUY_SELL_SCALE_DE)/10000;//TakeTrader(TakeTrader(m_CommonAttrib.nPrice,nPriceScale), 10000);
					int lPrice = (nPrice/BUY_SELL_SCALE_DE)%10000;//TakeRemainder(TakeTrader(m_CommonAttrib.nPrice, nPriceScale),10000);
					if (lPrice == 0)
						sprintf(szPrice, "Định giá: %d vạn %s", vPrice,nInfo);
					else
						sprintf(szPrice, "Định giá: %d vạn %d%s", vPrice,lPrice,nInfo);
			}
		strcat(otherzMsg, szPrice);
	}
    potherLabel = Label::createWithTTF(otherzMsg,UI_GAME_FONT_DEFAULT, 12,nDsize,TextHAlignment::CENTER);
    nTempOff += potherLabel->getContentSize().height;
    potherLabel->setAnchorPoint(ax::Vec2(0,1));
    potherLabel->enableOutline(Color4B(0, 0, 0, 255), 5);
    //potherLabel->setColor(ax::Color3B::GRAY);
    nRenderSize.height = potherLabel->getContentSize().height/2;
    nRenderSize.width = potherLabel->getContentSize().width/2;
    potherLabel->setPosition(ax::Vec2(nRenderSize.width/2,0));
    //m_size.height += nRenderSize.height;
    MenuItemLabel* tileLabel10 = MenuItemLabel::create(potherLabel);
    tileLabel10->setContentSize(nRenderSize);
    itemInfoMenu->addChild(tileLabel10);
    //Thuoc tinh khac 2

    //Hien thi hinh anh vat pham
    sprItem =Node::create();
	sprItem->setAnchorPoint(ax::Vec2(0,0));
    char nItemSprPath[256];
    int widthSpr = 0;
	ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
    if (Item[m_ItemData.uId].GetImagePath()){
        sprintf(nItemSprPath, "%s", Item[m_ItemData.uId].GetImagePath());
    }
    if (nItemSprPath[0]){
        g_StrLower(nItemSprPath);
        int m_nWidth,m_nHeight,nFrams;
        Texture2D *bgCur = NULL;
        SPRFRAMSINFO nSprInfo;
        ZeroMemory(&nSprInfo,sizeof(nSprInfo));
        bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
        if (bgCur==NULL)
            return;
        Sprite * sprItemImage =  Sprite::createWithTexture(bgCur);
        sprItemImage->setAnchorPoint(ax::Vec2(0,0));
        sprItemImage->setPosition(ax::Vec2(0,0));
        sprItemImage->setScale(1.2);
        widthSpr = (sprItemImage->getContentSize().width/2)+5;
        // LayerColor * colorLayer = LayerColor::create(ccc4(112, 128, 144, 150));
        // colorLayer->setAnchorPoint(ax::Vec2(0,0));
        // Size bgsize;
        // bgsize.width  = sprItem->getContentSize().width;
        // bgsize.height = sprItem->getContentSize().height;
        // colorLayer->setContentSize(bgsize);
        // sprItem->addChild(colorLayer,0);
        sprItem->addChild(sprItemImage,1);
        sprItem->setPosition(ax::Vec2(0,0));
        ParentNode_Team->addChild(sprItem);
    }
    //Hien thi hinh anh vat pham

    m_size.height = (nTempOff/2)+30;
    itemInfoMenu->alignItemsVerticallyWithPadding(1);
    itemInfoMenu->setPosition(0,m_size.height/2);
    ParentNode_Team->setContentSize(m_size);
    ParentNode_Team->addChild(itemInfoMenu);




	//Di chuyển vị trí của toàn bộ lớp
	if (m_ItemData.nRect.origin.y+m_size.height>visibleSize.height) //顶部
	{//Nếu nó vượt quá phạm vi hiển thị
		m_ItemData.nRect.origin.y = visibleSize.height-m_size.height-10;
	}


	if (m_ShowKind==3)
		ParentNode_Team->setPosition(ax::Vec2(visibleSize.width/2,(visibleSize.height-m_size.height)/2));
	else

	    ParentNode_Team->setPosition(ax::Vec2(m_ItemData.nRect.origin.x-m_size.width-64,m_ItemData.nRect.origin.y));
	//创建一个动作 改变精灵颜色
	red = TintBy::create(0.2,0,-255,-255); //持续时间+颜色
	red->retain();

}

//设置确定取消按钮,参数：调用层对象，调用层回调函数
void KuiItemdesc::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
	if  (m_ItemData.uId>0 && m_ItemData.uId<MAX_ITEM)
	{
		if (Item[m_ItemData.uId].GetGenre()==item_equip)
		{
			if(m_ItemData.nGenkind==UOC_ITEM_TAKE_WITH){
				int totalHeightMenu = 0;
				MenuItemImage * UseConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonSuDungFunc, this));
				//UseConfirm->setPosition(ax::Vec2(0,0));
				totalHeightMenu += UseConfirm->getContentSize().height;
				Label * UseConfirmLabel = Label::createWithTTF("Trang Bị",UI_GAME_FONT_DEFAULT_VN, 11);
				UseConfirmLabel->setColor(ax::Color3B::WHITE);
				UseConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
				UseConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
				//UseConfirm->setContentSize(btnSize);
				UseConfirmLabel->setPosition(ax::Vec2(UseConfirm->getContentSize().width/2-20,UseConfirm->getContentSize().height/2-7));

				UseConfirm->addChild(UseConfirmLabel,1);

				MenuItemImage * SendChatConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonSendChatFunc, this));
				totalHeightMenu += SendChatConfirm->getContentSize().height;
				Label * SendChatConfirmLabel = Label::createWithTTF("Rao",UI_GAME_FONT_DEFAULT_VN, 11);
				SendChatConfirmLabel->setColor(ax::Color3B::WHITE);
				SendChatConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
				SendChatConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
				SendChatConfirmLabel->setPosition(ax::Vec2(SendChatConfirm->getContentSize().width/2-20,SendChatConfirm->getContentSize().height/2-7));
				SendChatConfirm->addChild(SendChatConfirmLabel,1);

				MenuItemImage * DinhConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonDinhFunc, this));
				totalHeightMenu += DinhConfirm->getContentSize().height;
				Label * DinhConfirmLabel = Label::createWithTTF("Đính",UI_GAME_FONT_DEFAULT_VN, 11);
				DinhConfirmLabel->setColor(ax::Color3B::WHITE);
				DinhConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
				DinhConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
				DinhConfirmLabel->setPosition(ax::Vec2(DinhConfirm->getContentSize().width/2-20,DinhConfirm->getContentSize().height/2-7));
				DinhConfirm->addChild(DinhConfirmLabel,1);

				MenuItemImage * DinhGiaConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonDinhGiaFunc, this));
				totalHeightMenu += DinhGiaConfirm->getContentSize().height;
				Label * DinhGiaConfirmLabel = Label::createWithTTF("Định giá",UI_GAME_FONT_DEFAULT_VN, 11);
				DinhGiaConfirmLabel->setColor(ax::Color3B::WHITE);
				DinhGiaConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
				DinhGiaConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
				DinhGiaConfirmLabel->setPosition(ax::Vec2(DinhGiaConfirm->getContentSize().width/2-20,DinhGiaConfirm->getContentSize().height/2-7));
				DinhGiaConfirm->addChild(DinhGiaConfirmLabel,1);

				MenuItemImage * SellConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonSellItemFunc, this));
				totalHeightMenu += SellConfirm->getContentSize().height;
				Label * SellConfirmLabel = Label::createWithTTF("Bán",UI_GAME_FONT_DEFAULT_VN, 11);
				SellConfirmLabel->setColor(ax::Color3B::WHITE);
				SellConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
				SellConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
				SellConfirmLabel->setPosition(ax::Vec2(SellConfirm->getContentSize().width/2-20,SellConfirm->getContentSize().height/2-7));
				SellConfirm->addChild(SellConfirmLabel,1);

				MenuItemImage * RemoveConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonVutBoFunc, this));
				//RemoveConfirm->setPosition(ax::Vec2(0,100));
				Label * RemoveConfirmLabel = Label::createWithTTF("Vứt bỏ",UI_GAME_FONT_DEFAULT_VN, 11);
				RemoveConfirmLabel->setColor(ax::Color3B::WHITE);
				RemoveConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
				RemoveConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
				RemoveConfirmLabel->setPosition(ax::Vec2(RemoveConfirm->getContentSize().width/2-20,RemoveConfirm->getContentSize().height/2-7));
				RemoveConfirm->addChild(RemoveConfirmLabel,1);

				Point bgSpritePoint = ParentNode_Team->getPosition();
				Menu * menu = Menu::create(UseConfirm,SellConfirm,SendChatConfirm,DinhGiaConfirm,DinhConfirm,RemoveConfirm,NULL);
				menu->alignItemsVerticallyWithPadding(1);
				menu->setAnchorPoint(ax::Vec2(0, 0));
				menu->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width+SellConfirm->getContentSize().width/2,(ParentNode_Team->getContentSize().height-totalHeightMenu/2)-2));
				ParentNode_Team->addChild(menu,1000);
			}else if (m_ItemData.nGenkind==UOC_EQUIPTMENT){
				int totalHeightMenu = 0;
				MenuItemImage * UseConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonThaoFunc, this));
				totalHeightMenu += UseConfirm->getContentSize().height;
				Label * UseConfirmLabel = Label::createWithTTF("Tháo",UI_GAME_FONT_DEFAULT_VN, 11);
				UseConfirmLabel->setColor(ax::Color3B::WHITE);
				UseConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
				UseConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
				UseConfirmLabel->setPosition(ax::Vec2(UseConfirm->getContentSize().width/2-20,UseConfirm->getContentSize().height/2-7));
				UseConfirm->addChild(UseConfirmLabel,1);

				MenuItemImage * SendChatConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonSendChatFunc, this));
				totalHeightMenu += SendChatConfirm->getContentSize().height;
				Label * SendChatConfirmLabel = Label::createWithTTF("Rao",UI_GAME_FONT_DEFAULT_VN, 11);
				SendChatConfirmLabel->setColor(ax::Color3B::WHITE);
				SendChatConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
				SendChatConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
				SendChatConfirmLabel->setPosition(ax::Vec2(SendChatConfirm->getContentSize().width/2-20,SendChatConfirm->getContentSize().height/2-7));
				SendChatConfirm->addChild(SendChatConfirmLabel,1);

				MenuItemImage * DinhConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonDinhFunc, this));
				totalHeightMenu += DinhConfirm->getContentSize().height;
				Label * DinhConfirmLabel = Label::createWithTTF("Đính",UI_GAME_FONT_DEFAULT_VN, 11);
				DinhConfirmLabel->setColor(ax::Color3B::WHITE);
				DinhConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
				DinhConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
				DinhConfirmLabel->setPosition(ax::Vec2(DinhConfirm->getContentSize().width/2-20,DinhConfirm->getContentSize().height/2-7));
				DinhConfirm->addChild(DinhConfirmLabel,1);

				Point bgSpritePoint = ParentNode_Team->getPosition();
				Menu * menu = Menu::create(UseConfirm,SendChatConfirm,DinhConfirm,NULL);
				menu->alignItemsVerticallyWithPadding(1);
				menu->setAnchorPoint(ax::Vec2(0, 0));
				menu->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width+DinhConfirm->getContentSize().width/2,(ParentNode_Team->getContentSize().height-totalHeightMenu/2)-2));
				ParentNode_Team->addChild(menu,1000);
			}
			// if (m_ShowKind == 0){


			// }else if(m_ShowKind==2){//m_ShowKind = 10 -> được gọi khi touch vào item trong hành trang, = 2 -> touch từ bảng thông tin nhân vật


			// }



		}else{
			int totalHeightMenu = 0;
			MenuItemImage * UseConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonSuDungFunc, this));
			totalHeightMenu += UseConfirm->getContentSize().height;
			Label * UseConfirmLabel = Label::createWithTTF("Sử dụng",UI_GAME_FONT_DEFAULT_VN, 11);
			UseConfirmLabel->setColor(ax::Color3B::WHITE);
			UseConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
			UseConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
			UseConfirmLabel->setPosition(ax::Vec2(UseConfirm->getContentSize().width/2-20,UseConfirm->getContentSize().height/2-7));
			UseConfirm->addChild(UseConfirmLabel,1);

			// MenuItemImage * SendChatConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonSendChatFunc));
			// totalHeightMenu += SendChatConfirm->getContentSize().height;
			// Label * SendChatConfirmLabel = Label::createWithTTF("Rao",UI_GAME_FONT_DEFAULT_VN, 11);
			// SendChatConfirmLabel->setColor(ax::Color3B::WHITE);
			// SendChatConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
			// SendChatConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
			// SendChatConfirmLabel->setPosition(ax::Vec2(SendChatConfirm->getContentSize().width/2-20,SendChatConfirm->getContentSize().height/2-7));
			// SendChatConfirm->addChild(SendChatConfirmLabel,1);

			// MenuItemImage * DinhGiaConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonDinhGiaFunc));
			// totalHeightMenu += DinhGiaConfirm->getContentSize().height;
			// Label * DinhGiaConfirmLabel = Label::createWithTTF("Định giá",UI_GAME_FONT_DEFAULT_VN, 11);
			// DinhGiaConfirmLabel->setColor(ax::Color3B::WHITE);
			// DinhGiaConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
			// DinhGiaConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
			// DinhGiaConfirmLabel->setPosition(ax::Vec2(DinhGiaConfirm->getContentSize().width/2-20,DinhGiaConfirm->getContentSize().height/2-7));
			// DinhGiaConfirm->addChild(DinhGiaConfirmLabel,1);


			MenuItemImage * itemchaiConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::chaiCallBackFunc, this));
			itemchaiConfirm->setPosition(ax::Vec2(0,100));
			Label * itemchaiConfirmLabel = Label::createWithTTF("Tách",UI_GAME_FONT_VN, 11);
			itemchaiConfirmLabel->setColor(ax::Color3B::WHITE);
			itemchaiConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
			itemchaiConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
			itemchaiConfirmLabel->setPosition(ax::Vec2(itemchaiConfirm->getContentSize().width/2-20,itemchaiConfirm->getContentSize().height/2-7));
			itemchaiConfirm->addChild(itemchaiConfirmLabel,1);

			MenuItemImage * RemoveConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonVutBoFunc, this));
			//RemoveConfirm->setPosition(ax::Vec2(0,100));
			Label * RemoveConfirmLabel = Label::createWithTTF("Vứt bỏ",UI_GAME_FONT_DEFAULT_VN, 11);
			RemoveConfirmLabel->setColor(ax::Color3B::WHITE);
			RemoveConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
			RemoveConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
			RemoveConfirmLabel->setPosition(ax::Vec2(RemoveConfirm->getContentSize().width/2-20,RemoveConfirm->getContentSize().height/2-7));
			RemoveConfirm->addChild(RemoveConfirmLabel,1);

			MenuItemImage * kuaiConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::kuaiCallBackFunc, this));
			kuaiConfirm->setPosition(ax::Vec2(0,50));
			Label * kuaiConfirmLabel = Label::createWithTTF("Phím tắt",UI_GAME_FONT_VN, 11);
			kuaiConfirmLabel->setColor(ax::Color3B::WHITE);
			kuaiConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
			kuaiConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
			kuaiConfirmLabel->setPosition(ax::Vec2(kuaiConfirm->getContentSize().width/2-20,kuaiConfirm->getContentSize().height/2-7));
			kuaiConfirm->addChild(kuaiConfirmLabel,1);
			Point bgSpritePoint = ParentNode_Team->getPosition();
			Menu * menu = Menu::create(UseConfirm,itemchaiConfirm,kuaiConfirm,RemoveConfirm,NULL);
			menu->alignItemsVerticallyWithPadding(1);
			menu->setAnchorPoint(ax::Vec2(0, 0));
			menu->setPosition(ax::Vec2(ParentNode_Team->getContentSize().width+RemoveConfirm->getContentSize().width/2,(ParentNode_Team->getContentSize().height-totalHeightMenu/2)-2));
			ParentNode_Team->addChild(menu,1000);



		}
	}
	return;
	// MenuItemImage * SellConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonSellItemFunc));
	// //SellConfirm->setPosition(ax::Vec2(0,75));
	// Label * SellConfirmLabel = Label::createWithTTF("Bán",UI_GAME_FONT_DEFAULT_VN, 11);
	// SellConfirmLabel->setColor(ax::Color3B::WHITE);
	// SellConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// SellConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// SellConfirmLabel->setPosition(ax::Vec2(SellConfirm->getContentSize().width/2-20,SellConfirm->getContentSize().height/2-7));
	// SellConfirm->addChild(SellConfirmLabel,1);

	// MenuItemImage * RemoveConfirm = MenuItemImage::create("ui/btn_sel_3.png", "ui/btn_sel_3.png", CC_CALLBACK_1(KuiItemdesc::buttonVutBoFunc));
	// //RemoveConfirm->setPosition(ax::Vec2(0,100));
	// Label * RemoveConfirmLabel = Label::createWithTTF("Vứt bỏ",UI_GAME_FONT_DEFAULT_VN, 11);
	// RemoveConfirmLabel->setColor(ax::Color3B::WHITE);
	// RemoveConfirmLabel->setAnchorPoint(ax::Vec2(0,0));
	// RemoveConfirmLabel->enableOutline(Color4B(0, 0, 0, 255), 6);
	// RemoveConfirmLabel->setPosition(ax::Vec2(RemoveConfirm->getContentSize().width/2-20,RemoveConfirm->getContentSize().height/2-7));
	// RemoveConfirm->addChild(RemoveConfirmLabel,1);
}


void KuiItemdesc::kuaiCallBackFunc(Ref * pSender)
{

	if  (m_ItemData.uId<=0 || m_ItemData.uId>=MAX_ITEM)
	{
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vật phẩm không tồn tại");
		return;
	}
	// KUiPlayerGameTime	pInfo;
	// memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	// ZeroMemory(&pInfo,sizeof(pInfo));
	// if (g_pCoreShell)
	// 	g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);

	// unsigned int nCurChicktime = pInfo.npTime;

	// if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	// {
	// 	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Bạn thao tác quá nhanh");
	// 	return;
	// }
	// nBakChickTime = nCurChicktime;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData.uGenre;
	Obj.Obj.uId       = m_ItemData.uId;
	Obj.Region.h      = m_ItemData.DataX;
	Obj.Region.v      = m_ItemData.DataY;
	Obj.Region.Width  = m_ItemData.DataW;
	Obj.Region.Height = m_ItemData.DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_ADDITEM_CLIENT,(intptr_t)(&Obj),room_immediacy);
}

void KuiItemdesc::buttonSendChatFunc(Ref * pSender)
{


	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);
	unsigned int nCurChicktime = pInfo.npTime;  //记录打开窗口的秒数
	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//间隔２秒
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Bạn thao tác quá nhanh");
		return;
	}
	nBakChickTime = nCurChicktime;


	if  (m_ItemData.uId>0 && m_ItemData.uId<MAX_ITEM)
	{
		if (Item[m_ItemData.uId].GetGenre()==item_equip && g_pCoreShell->GetAutoplayid())
		{
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng tắt chức năng tự động đánh trước");
			return;
		}
	}
	else
		return;

	if  (g_GameWorld)
	{//开始输入聊天信息框
		char nTempStrVer[80];
		ZeroMemory(nTempStrVer,sizeof(nTempStrVer));
		sprintf(nTempStrVer,"<%s>",Item[m_ItemData.uId].GetName());
		g_GameWorld->setInputMsg(nTempStrVer);
		Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid = Item[m_ItemData.uId].GetID();
	}

	Node * node = dynamic_cast<Node *>(pSender);
	node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��

	if (m_callfun &&  m_callbackListener)
	{
		(m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}



void KuiItemdesc::chaiCallBackFunc(Ref * pSender)
{

	if  (m_ItemData.uId<=0)
	{
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng chọn 1 vật phẩm");
		return;
	}

	if  (m_ItemData.uId>0 && m_ItemData.uId<MAX_ITEM)
	{
		if (Item[m_ItemData.uId].GetGenre()==0)
		{
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vật phẩm này không thể tách");
		    return;
		}
	}
	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData.uGenre;
	Obj.Obj.uId       = m_ItemData.uId;
	Obj.Region.h      = m_ItemData.DataX;
	Obj.Region.v      = m_ItemData.DataY;
	Obj.Region.Width  = m_ItemData.DataW;
	Obj.Region.Height = m_ItemData.DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;

	if (g_pCoreShell)
	{
		if (g_pCoreShell->OperationRequest(GOI_NPC_ITEM_BREAK,(uintptr_t)(&Obj), 1))
		{//
			char nKey[32];
			sprintf(nKey,"item_%u",Item[Obj.Obj.uId].GetID());
			std::string nItemKey = nKey;
			Sprite *nItemSpr =(Sprite *)ParentNode_Team->getChildByName(nItemKey);
			if (nItemSpr)
			{
				Label *stacklabel =(Label *)nItemSpr->getChildByName("stack");
				if (stacklabel)
				{
					int nstackCount = Item[Obj.Obj.uId].GetStackNum();
					sprintf(nKey,"%d",nstackCount);
					stacklabel->setString(nKey);
				}
			}
		}
	}
}

void KuiItemdesc::buttonVutBoFunc(Ref * pSender)
{

	if (m_ItemData.nGenkind==UOC_ITEM_TAKE_WITH){
		//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vật phẩm chưa trang bị");
		KUiPlayerGameTime	pInfo;
		memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
		ZeroMemory(&pInfo,sizeof(pInfo));
		// if (g_pCoreShell)
		// 	g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);
		// unsigned int nCurChicktime = pInfo.npTime;  //记录打开窗口的秒数
		// if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
		// {//间隔２秒
		// 	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Bạn thao tác quá nhanh");
		// 	return;
		// }
		// nBakChickTime = nCurChicktime;


		if  (m_ItemData.uId>0 && m_ItemData.uId<MAX_ITEM)
		{
			if (Item[m_ItemData.uId].GetGenre()==item_equip && g_pCoreShell->GetAutoplayid())
			{
				Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng tắt chức năng tự động đánh trước");
				return;
			}
		}
		else
			return;

		bool isYes = false;
		if (g_pCoreShell && g_pCoreShell->GetAutoplayid())
			isYes = true;
		Player[CLIENT_PLAYER_INDEX].AutoAwayItem(Item[m_ItemData.uId].GetID(),isYes);

		Node * node = dynamic_cast<Node *>(pSender);
		node->setTag(3000);

		if (m_callfun &&  m_callbackListener)
		{
			(m_callfun)(node);
		}

		this->removeFromParentAndCleanup(true);
	}
	// else{
	// 	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vật phẩm đã trang bị");
	// }

}

void KuiItemdesc::buttonSellItemFunc(Ref * pSender)
{


	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);
	// unsigned int nCurChicktime = pInfo.npTime;  //记录打开窗口的秒数
	// if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	// {//间隔２秒
	// 	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Bạn thao tác quá nhanh");
	// 	return;
	// }
	// nBakChickTime = nCurChicktime;

	if (m_ItemData.nGenkind==UOC_ITEM_TAKE_WITH){
		//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vật phẩm trong hành trang");
		if  (m_ItemData.uId>0 && m_ItemData.uId<MAX_ITEM)
		{
			if (Item[m_ItemData.uId].GetGenre()==item_equip && g_pCoreShell->GetAutoplayid())
			{
				Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng tắt chức năng tự động đánh trước");
				return;
			}
		}
		else
			return;

		KUiObjAtContRegion	Obj;
		Obj.Obj.uGenre    = m_ItemData.uGenre;
		Obj.Obj.uId       = m_ItemData.uId;
		Obj.Region.h      = m_ItemData.DataX;
		Obj.Region.v      = m_ItemData.DataY;
		Obj.Region.Width  = m_ItemData.DataW;
		Obj.Region.Height = m_ItemData.DataH;
		Obj.eContainer    = UOC_ITEM_TAKE_WITH;
		if (g_pCoreShell)
			g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,(uintptr_t)(&Obj),0);

		Node * node = dynamic_cast<Node *>(pSender);
		node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��

		if (m_callfun &&  m_callbackListener)
		{
			(m_callfun)(node);
		}

		this->removeFromParentAndCleanup(true);
	}
	// else{
	// 	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vật phẩm đã trang bị");
	// }


}


void KuiItemdesc::buttonDinhGiaFunc(Ref * pSender)
{


	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);
	unsigned int nCurChicktime = pInfo.npTime;  //记录打开窗口的秒数
	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//间隔２秒
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Bạn thao tác quá nhanh");
		return;
	}
	nBakChickTime = nCurChicktime;


	if  (m_ItemData.uId>0 && m_ItemData.uId<MAX_ITEM)
	{
		if (Item[m_ItemData.uId].GetGenre()==item_equip && g_pCoreShell->GetAutoplayid())
		{
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng tắt chức năng tự động đánh trước");
			return;
		}
	}
	else
		return;

	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Chức năng đang được phát triển!");

	Node * node = dynamic_cast<Node *>(pSender);
	node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��

	if (m_callfun &&  m_callbackListener)
	{
		//ִ�е��ò�ص����������ݲ���Node
		(m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}

void KuiItemdesc::buttonDinhFunc(Ref * pSender)
{


	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);
	unsigned int nCurChicktime = pInfo.npTime;  //记录打开窗口的秒数
	if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	{//间隔２秒
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Bạn thao tác quá nhanh");
		return;
	}
	nBakChickTime = nCurChicktime;


	if  (m_ItemData.uId>0 && m_ItemData.uId<MAX_ITEM)
	{
		if (Item[m_ItemData.uId].GetGenre()==item_equip && g_pCoreShell->GetAutoplayid())
		{
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng tắt chức năng tự động đánh trước");
			return;
		}
	}
	else
		return;

	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Chức năng đang được phát triển!");

	Node * node = dynamic_cast<Node *>(pSender);
	node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��

	if (m_callfun &&  m_callbackListener)
	{
		//ִ�е��ò�ص����������ݲ���Node
		(m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}

void KuiItemdesc::buttonThaoFunc(Ref * pSender)
{


	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	// if (g_pCoreShell)
	// 	g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);
	// unsigned int nCurChicktime = pInfo.npTime;  //记录打开窗口的秒数
	// if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	// {//间隔２秒
	// 	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Bạn thao tác quá nhanh");
	// 	return;
	// }
	// nBakChickTime = nCurChicktime;


	if  (m_ItemData.uId>0 && m_ItemData.uId<MAX_ITEM)
	{
		if (Item[m_ItemData.uId].GetGenre()==item_equip && g_pCoreShell->GetAutoplayid())
		{
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng tắt chức năng tự động đánh trước");
			return;
		}
	}
	else
		return;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData.uGenre;
	Obj.Obj.uId       = m_ItemData.uId;
	Obj.Region.h      = m_ItemData.DataX;
	Obj.Region.v      = m_ItemData.DataY;
	Obj.Region.Width  = m_ItemData.DataW;
	Obj.Region.Height = m_ItemData.DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if  (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_USE_ITEM,(uintptr_t)(&Obj),UOC_ITEM_TAKE_WITH);

	Node * node = dynamic_cast<Node *>(pSender);
	node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��

	if (m_callfun &&  m_callbackListener)
	{
		//ִ�е��ò�ص����������ݲ���Node
		(m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}

void KuiItemdesc::buttonSuDungFunc(Ref * pSender)
{


	KUiPlayerGameTime	pInfo;
	memset(&pInfo, 0, sizeof(KUiPlayerGameTime));
	ZeroMemory(&pInfo,sizeof(pInfo));
	// if (g_pCoreShell)
	// 	g_pCoreShell->GetGameData(GDI_PLAYER_TIMEINFO, (intptr_t)&pInfo, 0);
	// unsigned int nCurChicktime = pInfo.npTime;  //记录打开窗口的秒数
	// if (nBakChickTime>0 && nCurChicktime-nBakChickTime<=2)
	// {//间隔２秒
	// 	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Bạn thao tác quá nhanh");
	// 	return;
	// }
	// nBakChickTime = nCurChicktime;


	if  (m_ItemData.uId>0 && m_ItemData.uId<MAX_ITEM)
	{
		if (Item[m_ItemData.uId].GetGenre()==item_equip && g_pCoreShell->GetAutoplayid())
		{
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("Vui lòng tắt chức năng tự động đánh trước");
			return;
		}
	}
	else
		return;

	KUiObjAtContRegion	Obj;
	Obj.Obj.uGenre    = m_ItemData.uGenre;
	Obj.Obj.uId       = m_ItemData.uId;
	Obj.Region.h      = m_ItemData.DataX;
	Obj.Region.v      = m_ItemData.DataY;
	Obj.Region.Width  = m_ItemData.DataW;
	Obj.Region.Height = m_ItemData.DataH;
	Obj.eContainer    = UOC_ITEM_TAKE_WITH;
	if  (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_USE_ITEM,(uintptr_t)(&Obj),UOC_ITEM_TAKE_WITH);

	Node * node = dynamic_cast<Node *>(pSender);
	node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��

	if (m_callfun &&  m_callbackListener)
	{
		//ִ�е��ò�ص����������ݲ���Node
		(m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}

//执行上层对象的回调函数，关闭弹出框
void KuiItemdesc::buttonCallBackFunc(Ref * pSender)
{
	Node * node = dynamic_cast<Node *>(pSender);
	node->setTag(3000);//设置tag，在调用层可以获取到

	if (m_callfun &&  m_callbackListener)
	{
		//执行调用层回调函数，传递参数CCNode
		(m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}

//关闭弹出框
void KuiItemdesc::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}


//重写触摸注册函数，重新给定触摸级别
void KuiItemdesc::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//这里的触摸优先级设置为－128，与CCMenu同级，保证了屏蔽下方的触摸
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-128, true);
}

Rect KuiItemdesc::getRect(Node* pNode,int i)
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
bool KuiItemdesc::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(NULL);
	this->removeFromParentAndCleanup(true);

	return true;
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	return true;
}

void KuiItemdesc::update(float delta)
{
	//messageBox("无限循环","update");
	if (isOpen && g_pCoreShell)
	{
		if (ptitleLabel)
		{

		}

	    //m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
		//m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
	}
}
