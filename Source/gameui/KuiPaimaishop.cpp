#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiPaimaishop.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
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
#include "gamecore/KSkillManager.h"
#include "gamecore/KItem.h"
#include "gamecore/KBuySell.h"
#include "gameui/KuiItemdesc.h"
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

KuiPaimaishop::KuiPaimaishop()
{
	isOpen = false;
	//memset(Skills,0,sizeof(KUiSkillData) * 50);
	pMoneyLabel       = NULL;
	pXuLabel          = NULL;
	colorLayer        = NULL;
	red               = NULL;
	m_selItemIndex    = -1;
	//m_nMoney = 0;
	//m_nXu = 0;
	//m_nNumObjects = 0;
	m_CurShopIdx  = 0;
	m_nPageCount  = 0;
	m_nObjCount = 0;
	m_pObjsList = NULL;
	m_StartPos.x=0;
	m_StartPos.y=0;
	m_nCurrentCount = 0;
	m_nCurrentPage  = 0;
	ItemNode = NULL;
	pstrEditBox_count = NULL;
	m_descPadIsOpen = false;
}

KuiPaimaishop::~KuiPaimaishop()
{
	isOpen = false;
	pMoneyLabel  = NULL;
	colorLayer   = NULL;
	Clear();
	if (red)
	   red->release();
	//red               = NULL;
}

//����ȷ��������
KuiPaimaishop * KuiPaimaishop::create(KUiPlayerItem* pDest,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	KuiPaimaishop * popLayer = KuiPaimaishop::create();
	//popLayer->setTitle(title);
	popLayer->m_CurShopIdx = 0;//nshopIdx;
	popLayer->addDialoaData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiPaimaishop::init()
{
	if ( !Layer::init() ){
		return false;
	}

	//����֡����
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");

	//���ģ̬����
	Size winSize = ax::Director::getInstance()->getWinSize();
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\����������\\�����б���.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;
	Sprite * spriteBox = Sprite::createWithTexture(bgCur);
	ParentNode_ItemEx = spriteBox;
	spriteBox->setPosition(ax::Vec2(winSize.width/2,winSize.height/2));
	this->addChild(spriteBox);

	//��ñ���ͼƬ�Ĵ�С
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();
	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С
	//����������Ӧ

	this->scheduleUpdate();                   //���ú���
	auto listener = ax::EventListenerTouchOneByOne::create();
ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);   //��ֹ��㴥��
	return true;
}

void KuiPaimaishop::addDialoaData()
{
	if  (!g_pCoreShell) return;

	//Left=24
	//Top=72
	m_StartPos.x =24;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =72;//+ITEM_CELL_SIZE/2;

    pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Arial
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	pMoneyLabel->setPosition(ax::Vec2(60,128));
	ParentNode_ItemEx->addChild(pMoneyLabel,1);

	pXuLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//Arial
	pXuLabel->setAnchorPoint(ax::Vec2(0,0));
	pXuLabel->setPosition(ax::Vec2(60,105));
	ParentNode_ItemEx->addChild(pXuLabel,1);

	ItemNode = Node::create();
	ItemNode->setAnchorPoint(ax::Vec2(0,0));
	ItemNode->setPosition(ax::Vec2(0,0));
	ParentNode_ItemEx->addChild(ItemNode,1);
	UpdateData();
	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);//���úʹ��ڵĴ�С
	ParentNode_ItemEx->addChild(colorLayer,1000);
	//����һ������ �ı侫����ɫ
	red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	red->retain();
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiPaimaishop::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\���������.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_buy_normal = Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_buy_select = Sprite::createWithTexture(bgCur);
	MenuItemSprite * buyConfirm   = MenuItemSprite::create(sprite_buy_normal, sprite_buy_select, CC_CALLBACK_1(KuiPaimaishop::buyCallBackFunc, this));
	buyConfirm->setPosition(ax::Vec2(-70,1));

	ax::ui::Scale9Sprite *gBg = ax::ui::Scale9Sprite::create("ui/editbg.png");
	pstrEditBox_count = EditBox::create(Size(50,24),gBg);
	if (pstrEditBox_count)
	{
		//pEditBox->setContentSize()
		pstrEditBox_count->setText("1");
		pstrEditBox_count->setPlaceHolder(UTEXT("��������",1).c_str());
		pstrEditBox_count->setFontColor(ax::Color3B::YELLOW);
		pstrEditBox_count->setFontSize(24);
		pstrEditBox_count->setMaxLength(2);
		pstrEditBox_count->setPosition(ax::Vec2(m_size.width/2-10,47));
		pstrEditBox_count->setInputMode(EditBox::InputMode::DECIMAL);
		pstrEditBox_count->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//pMoneyEditBox->setDelegate(this);
		//pMoneyEditBox->setDefaultTouchPriority(-128);
		ParentNode_ItemEx->addChild(pstrEditBox_count);
	}

	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\spr\\Ui3\\����\\���ߣ��ر�.spr");
	g_StrLower(nSprName);
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_close_normal = Sprite::createWithTexture(bgCur);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_close_select = Sprite::createWithTexture(bgCur);
	MenuItemSprite * closeConfirm   = MenuItemSprite::create(sprite_close_normal, sprite_close_select, CC_CALLBACK_1(KuiPaimaishop::closeCallBackFunc, this));
	closeConfirm->setPosition(ax::Vec2(69,1));

	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\������壭��.spr");
	g_StrLower(nSprName);
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_back_normal = Sprite::createWithTexture(bgCur);
	//sprite_back_normal->setScaleX(2.0);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_back_select = Sprite::createWithTexture(bgCur);
	//sprite_back_select->setScaleX(2.0);
	MenuItemSprite * backConfirm   = MenuItemSprite::create(sprite_back_normal, sprite_back_select, CC_CALLBACK_1(KuiPaimaishop::backCallBackFunc, this));
	backConfirm->setPosition(ax::Vec2(-50,25));
	backConfirm->setScaleX(3.0);


	ZeroMemory(nSprName,sizeof(nSprName));
	t_sprintf(nSprName,"\\Spr\\Ui3\\����\\������壭��.spr");
	g_StrLower(nSprName);
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_next_normal = Sprite::createWithTexture(bgCur);
	//sprite_next_normal->setScaleX(2.0);
	bgCur = _getinidata.getinidata_one(nSprName,1,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return;
	Sprite * sprite_next_select = Sprite::createWithTexture(bgCur);
	//sprite_next_select->setScaleX(2.0);
	MenuItemSprite * nextConfirm   = MenuItemSprite::create(sprite_next_normal, sprite_next_select, CC_CALLBACK_1(KuiPaimaishop::nextCallBackFunc, this));
	nextConfirm->setPosition(ax::Vec2(50,25));
	nextConfirm->setScaleX(3.0);

	Point bgSpritePoint = ParentNode_ItemEx->getPosition();
	Menu * menu = Menu::create(buyConfirm,closeConfirm,backConfirm,nextConfirm,NULL);
	menu->setPosition(ax::Vec2(m_size.width/2,45));//�˵�λ������Ϊ����������
	ParentNode_ItemEx->addChild(menu);         //�ӵ�������
	if (g_GameWorld)//�����������Ĵ���
		g_GameWorld->setchatDelegate(false);
}
//��һҳ
void KuiPaimaishop::backCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen =false;
		ParentNode_ItemEx->removeChildByTag(1001,true);
	}

	int index = m_nCurrentPage - 1;
	if (index<0)
		return;
	SetPage(index);
}
//��һҳ
void KuiPaimaishop::nextCallBackFunc(Ref * pSender)
{
	if (m_descPadIsOpen)
	{
		m_descPadIsOpen =false;
		ParentNode_ItemEx->removeChildByTag(1001,true);
	}

	int index = m_nCurrentPage + 1;
	if (index<0 || index >=m_nPageCount)
		return;

	SetPage(index);

	/*if  (nIndex == (m_nPageCount-1))
			//m_NextBtn.Enable(FALSE);
		else if  (nIndex <= 0 )
		{
			//m_PreBtn.Enable(FALSE);
		}
		else if  (nIndex > 0 && nIndex < (m_nPageCount-1))
		{
			//m_PreBtn.Enable(TRUE);
			//m_NextBtn.Enable(TRUE);
		}*/

}
//ִ���ϲ����Ļص��������رյ�����
void KuiPaimaishop::buyCallBackFunc(Ref * pSender)
{
	if  (m_selItemIndex<0)
	{
#ifdef WIN32
		ccMessageBox("��ѡ��һ����Ʒ","��ʾ");
#else
		ccMessageBox(UTEXT("��ѡ��һ����Ʒ",1).c_str(),UTEXT("��ʾ",1).c_str());
#endif
		return;
	}
	//��⹺�������
	int  nCount =1;
	char nTempStr[32];
	if (pstrEditBox_count)
	{
		std::string nTempString = pstrEditBox_count->getText();
		if (nTempString.empty())
			nTempString = "1";
		//����ַ�����
		//��ʼ����ַ��ĺϷ���
		std::string strcheck="1";
		strcheck+=nTempString;
		int count = 0,spaceNum=0,chineseNum=0,other=0,letter=0,xiaoxie=0;
		for(int i = 0; i <strcheck.length();i++)
		{
			//isalnum
			if(isdigit(strcheck[i]))     //�ж��Ƿ�������0-9
				count++;
			else if(isspace(strcheck[i]))//�ж��Ƿ��ǿո�
				++spaceNum;
			else if(isalpha(strcheck[i])==1)//�ж��Ƿ�����ĸ	 1 Ϊ��д 2 Ϊ Сд  0 Ϊ������ĸ
			{
				++letter;
			}
			else if(isalpha(strcheck[i])==2)//�ж��Ƿ�����ĸ	 1 Ϊ��д 2 Ϊ Сд  0 Ϊ������ĸ
			{
				++xiaoxie;
			}
			else if(strcheck[i]&0x80)//if(strcheck[0]>=0xB0)
				++chineseNum;        //�ж��Ƿ�������
			else
			{
				++other;
			}
		}

		if (spaceNum>0 || chineseNum>0 || xiaoxie >0 || other>0 || letter>0)
		{ 	//system("pause");
#ifdef WIN32
			ccMessageBox("ֻ����д����!","����");
#else
			ccMessageBox(G2U("ֻ����д����!").c_str(),G2U("����").c_str());
#endif
			return;
		}
		t_sprintf(nTempStr,pstrEditBox_count->getText());
		nCount = g_Atoui(nTempStr);
	}
	if  (nCount<=0)
		nCount = 1;

	if  (nCount>30)
	{
		nCount = 30;
		pstrEditBox_count->setText("30");
	}

	KUiDraggedObject* pItem  = (KUiDraggedObject*)&m_pObjsList[m_selItemIndex];
	if (pItem)
	{
		KUiObjAtContRegion	Obj;
		Obj.Obj.uGenre    = pItem->uGenre;
		Obj.Obj.uId       = pItem->uId;
		Obj.Region.h      = pItem->DataX;
		Obj.Region.v      = pItem->DataY;
		Obj.Region.Width  = pItem->DataW;
		Obj.Region.Height = pItem->DataH;
		Obj.eContainer    = UOC_NPC_SHOP;
		if  (g_pCoreShell)
			g_pCoreShell->OperationRequest(GOI_TRADE_NPC_BUY,(uintptr_t)(&Obj),nCount);
	}
}

void KuiPaimaishop::closeCallBackFunc(Ref * pSender)
{
	Node * node = dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
	{
		//ִ�е��ò�ص����������ݲ���Node
		(m_callfun)(node);
	}

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiPaimaishop::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}


//��д����ע�ắ�������¸�����������
void KuiPaimaishop::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,128,true);
}

Rect KuiPaimaishop::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y

	return rc;
}
//��������ccTouchBegan������true
bool KuiPaimaishop::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	bool isThis=false;
	for (int i = 0; i < m_nCurrentCount; ++i)
	{
		if (m_ItemIndex[i]>=0)
		{
			 int nidx     = m_pObjsList[m_ItemIndex[i]].Obj.uId;
			 int nIndex   = BuySell.GetItemIndex(m_CurShopIdx,nidx);
			 char nItemKey[32];
			 ZeroMemory(&nItemKey,sizeof(nItemKey));
			 t_sprintf(nItemKey,"item_%u",nIndex+1);
			 std::string nKey =nItemKey;
			 Sprite *nItemSpr = (Sprite *)ItemNode->getChildByName(nKey);
			 t_sprintf(nItemKey,"color_%d",nIndex+1);
			 nKey =nItemKey;
			 LayerColor *ncolorlable = (LayerColor *)ItemNode->getChildByName(nKey);
			 if  (nItemSpr && ncolorlable)
			 {
				 if (getRect(ncolorlable,i+1).containsPoint(location))
				 {//������
					 //restartAction();
					 nItemSpr->runAction(red);
					 m_selItemIndex = m_ItemIndex[i];
					 KUiItemdObject second;
					 second.uId    = m_pObjsList[m_ItemIndex[i]].Obj.uId;
					 second.uGenre = UOC_NPC_SHOP;
					 second.nRect  = getRect(ncolorlable,0);
					// Layer * sDescLayer = KuiItemdesc::create("",second,this,CC_CALLBACK_1(KuiPaimaishop::closedescPadCallback));
					 Layer * sDescLayer = KuiItemdesc::create(m_CurShopIdx,second,this,CC_CALLBACK_1(KuiPaimaishop::closedescPadCallback, this));
					 m_descPadIsOpen = true;
					 isThis = true;
					 this->addChild(sDescLayer,2,1001);
				 }
				 else
				 {
					 if (!isThis)
						 m_selItemIndex = -1;
					 nItemSpr->setColor(ax::Color3B::WHITE);           //Ĭ����ɫ
				 }
			 }
		}
	}
	return true;
}

void KuiPaimaishop::closedescPadCallback(Node *pNode)
{
	m_descPadIsOpen = false;
}

void KuiPaimaishop::update(float delta)
{
	//ccMessageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	   /* m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
		m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
		char nPointCount[32];
	   if (pMoneyLabel)
	   {
		   t_sprintf(nPointCount,"%d",m_nMoney);
		  // pSkillPointLabel->setColor(ccWit)
		   pMoneyLabel->setString(nPointCount);
	   }
	   if  (pXuLabel)
	   {
		   t_sprintf(nPointCount,"%d",m_nXu);
		   // pSkillPointLabel->setColor(ccWit)
		   pXuLabel->setString(nPointCount);
	   }*/
	}
}

void KuiPaimaishop::Clear()
{
	m_nObjCount  = 0;
	m_nPageCount = 0;

	if (m_pObjsList)
	{
		free(m_pObjsList);
		m_pObjsList = NULL;
	}
}


//������Ʒ
int KuiPaimaishop::AddObject(KUiDraggedObject* pObject, int nCount)
{
	if (!pObject) return false;


	for (int i = 0; i < nCount; ++i)
	{
		    int nIndex   = BuySell.GetItemIndex(m_CurShopIdx,pObject->uId);
		    KItem* pItem = BuySell.GetItemidx(nIndex);

			if (pItem==NULL) return false;

			KUiDraggedObject no;
			no.uGenre = pObject->uGenre;
			no.uId    = pObject->uId;
			no.DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
			no.DataY  = pObject->DataY;     //�����е����Yλ��
			no.DataW  = pObject->DataW;     //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
			no.DataH  = pObject->DataH;     //��Ʒ�Ŀ��(ռ���˶��ٸ�����)
			if (pItem)
			{
				/*m_ItemData[i].uGenre = pObjs[i].Obj.uGenre;
				m_ItemData[i].uId    = pObjs[i].Obj.uId;
				m_ItemData[i].nindex = i;
				m_ItemData[i].DataX  = pObjs[i].Region.h;     //�����е����Xλ�� �ڼ������� 25x25
				m_ItemData[i].DataY  = pObjs[i].Region.v;     //�����е����Yλ��
				m_ItemData[i].DataW  = pObjs[i].Region.Width; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
				m_ItemData[i].DataH  = pObjs[i].Region.Height;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
				m_ItemData[i].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��
				*/
				char nItemSprPath[256];
				ZeroMemory(nItemSprPath,sizeof(nItemSprPath));
				if  (pItem->GetImagePath())
					t_sprintf(nItemSprPath,pItem->GetImagePath());
				else
					t_sprintf(nItemSprPath,"\\spr\\others\\�ʺ�.spr");

				if  (nItemSprPath[0])
				{
					//t_sprintf(nItemSprPath,pItem->GetImagePath());
					g_StrLower(nItemSprPath);
					int m_nWidth,m_nHeight,nFrams;
					Texture2D *bgCur = NULL;
					SPRFRAMSINFO nSprInfo;
					ZeroMemory(&nSprInfo,sizeof(nSprInfo));
					bgCur = _getinidata.getinidata_one(nItemSprPath,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
					if (bgCur==NULL)
						return false;
					char nItemKey[32];
					ZeroMemory(&nItemKey,sizeof(nItemKey));
					t_sprintf(nItemKey,"item_%u",nIndex+1);
					std::string nKey =nItemKey;
					Sprite *nItemSpr = (Sprite *)ItemNode->getChildByName(nKey);
					if (!nItemSpr)
					{
						nItemSpr = Sprite::createWithTexture(bgCur);
						//nItemSpr->setAnchorPoint(ax::Vec2(0,0));
						int nikey = std::hash<std::string>{}(nKey);
nItemSpr->setTag(nikey);
						ItemNode->addChild(nItemSpr,1);

						if  (pItem->GetGenre()!=item_equip && pItem->IsStack())
						{//����װ�� �ɵ�����Ʒ
							char stack[32];
							t_sprintf(stack,"%d",pItem->GetStackNum());
							Label *stuckCountlabel = Label::createWithTTF(stack,UI_GAME_FONT_DEFAULT,14);
							stuckCountlabel->setColor(ax::Color3B::YELLOW);
							stuckCountlabel->setAnchorPoint(ax::Vec2(0,0));
							stuckCountlabel->setPosition(ax::Vec2(0,0));
							int nikey = std::hash<std::string>{}("stack");
stuckCountlabel->setTag(nikey);
							nItemSpr->addChild(stuckCountlabel,1);
						}
					}
					//��ʼ������
					/*m_ItemData[nKey].uGenre = pObject->uGenre;
					m_ItemData[nKey].uId    = pObject->uId;
					m_ItemData[nKey].nindex = i;
					m_ItemData[nKey].DataX  = pObject->DataX;     //�����е����Xλ�� �ڼ������� 25x25
					m_ItemData[nKey].DataY  = pObject->DataY;     //�����е����Yλ��
					m_ItemData[nKey].DataW  = pObject->DataW; //��Ʒ�ĸ߶�(ռ���˶��ٸ�����)
					m_ItemData[nKey].DataH  = pObject->DataH;//��Ʒ�Ŀ��(ռ���˶��ٸ�����)
					m_ItemData[nKey].nGenkind = UOC_ITEM_TAKE_WITH;  //����Я��
					*/
					Color4B color(112, 128, 144, 150);//112, 128, 144
					t_sprintf(nItemKey,"color_%d",nIndex+1);
					//strcat(nItemKey,"_color");
					nKey =nItemKey;
					LayerColor *bgcolorLayer = (LayerColor *)ItemNode->getChildByName(nKey);
					if (!bgcolorLayer)
					{
						bgcolorLayer = LayerColor::create(color);//��ɫ��
					    //bgcolorLayer->setPosition(ax::Vec2::ZERO);
						bgcolorLayer->setAnchorPoint(ax::Vec2(0,0));
						Size bgsize;
						bgsize.width  = no.DataW*ITEM_CELL_SIZE-2;
						bgsize.height = no.DataH*ITEM_CELL_SIZE-2;
					    bgcolorLayer->setContentSize(bgsize);//���úʹ��ڵĴ�С
						int nikey = std::hash<std::string>{}(nKey);
bgcolorLayer->setTag(nikey);
						ItemNode->addChild(bgcolorLayer,0);
					}

					int nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+no.DataW*ITEM_CELL_SIZE/2;
					int nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE/2;

					nItemSpr->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));
					//��ɫ��
					nCurX = m_StartPos.x+no.DataX*ITEM_CELL_SIZE+1;
					nCurY = m_StartPos.y+no.DataY*ITEM_CELL_SIZE+no.DataH*ITEM_CELL_SIZE-1;
					bgcolorLayer->setPosition(ax::Vec2(nCurX,m_size.height-nCurY));

					KUiObjAtContRegion	Obj;
					Obj.Obj.uGenre = no.uGenre;
					Obj.Obj.uId    = no.uId;
					Obj.Region.h   = Obj.Region.v = 0;
					Obj.Region.Width = Obj.Region.Height = 0;
					Obj.nContainer = UOC_NPC_SHOP;

					ITEM_IN_ENVIRO_PROP eProp = (ITEM_IN_ENVIRO_PROP)g_pCoreShell->
					    GetGameData(GDI_ITEM_IN_ENVIRO_PROP, (uintptr_t)&Obj, 0);
					//if (eProp == IIEP_NORMAL) //����
					//else
					if (eProp == IIEP_NOT_USEABLE)  //������
					{
						ax::Color3B color_(128, 0, 0);
						bgcolorLayer->setColor(color_);
					}
					//else if (eProp == IIEP_SPECIAL) //����������Ʒ
				}
			}
	}
	return true;
}
void KuiPaimaishop::SetPage(int nIndex)
{
	if  (ItemNode)
		ParentNode_ItemEx->removeChild(ItemNode,true);

	ItemNode = Node::create();
	ItemNode->setAnchorPoint(ax::Vec2(0,0));
	ItemNode->setPosition(ax::Vec2(0,0));
	ParentNode_ItemEx->addChild(ItemNode,1);

	if (nIndex >= 0 && nIndex < m_nPageCount && m_pObjsList)
	{
		//m_ItemsBox.Clear();
		int nTempCount=0;
		int i;
		for (i = 0; i < PAGE_MAX_COUNT_ITEM; i++)
		{//
			m_ItemIndex[i]=-1;
		}

		for (i = 0; i < m_nObjCount; i++)
		{//��ʼ������Ʒ
			if (m_pObjsList[i].nContainer == nIndex)  //�ڼ�ҳ����Ʒ
			{
				AddObject((KUiDraggedObject*)&m_pObjsList[i], 1);
				m_ItemIndex[nTempCount] = i; //ÿҳ���������
				nTempCount++;
			}
			//	m_ItemsBox.AddObject((KUiDraggedObject*)&m_pObjsList[i], 1);
		}
		m_nCurrentCount = nTempCount;
		m_nCurrentPage  = nIndex;                      //��ǰҳ
		//char nPages[16]={0};
		//t_sprintf(nPages,"%d/%d",m_nCurrentPage+1,m_nPageCount);
		//m_CurPageText.SetText(nPages);

		/*if  (nIndex == (m_nPageCount-1))
			//m_NextBtn.Enable(FALSE);
		else if  (nIndex <= 0 )
		{
			//m_PreBtn.Enable(FALSE);
		}
		else if  (nIndex > 0 && nIndex < (m_nPageCount-1))
		{
			//m_PreBtn.Enable(TRUE);
			//m_NextBtn.Enable(TRUE);
		}*/
	}
}
void KuiPaimaishop::UpdateData()
{
	Clear();
	m_nObjCount = g_pCoreShell->GetDataNpcShop(m_CurShopIdx, 0, 0);
	if (m_nObjCount == 0)
		return;

	if (m_pObjsList = (KUiObjAtContRegion*)malloc(sizeof(KUiObjAtContRegion) * m_nObjCount))
	{
		g_pCoreShell->GetDataNpcShop(m_CurShopIdx, (uintptr_t)m_pObjsList, m_nObjCount);//���߳�ִ�У�nCountֵ����
		m_nPageCount = m_pObjsList[m_nObjCount - 1].nContainer + 1;
		SetPage(0);
		if (m_nPageCount > 1)
		{
			//m_PreBtn.Enable(TRUE);
			//m_NextBtn.Enable(TRUE);
		}
		//char msg[64];
		//t_sprintf(msg,"����:%d,ҳ:%d,%d",m_nObjCount,m_nPageCount,m_CurShopIdx);
		//ccMessageBox(msg,"�̵�");
	}
	else
		m_nObjCount = 0;
}
