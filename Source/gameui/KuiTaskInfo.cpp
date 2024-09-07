#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "engine/CXmlStream.h"
#include "KuiTaskInfo.h"
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
extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;
KuiTaskInfo::KuiTaskInfo()
{
	isOpen = false;
	__isCanMove=true;
	m_bScrolling=false;
	pmainListView = NULL;
	ptaskMsglistView = NULL;
	colorLayer    = NULL;
	red           = NULL;
	m_pMainUiLayer = NULL;
	__nSelIndex    = -1;
	__nCount     = 0;
	//ZeroMemory(nFindPath,sizeof(nFindPath));
	nFindPath.clear();
}

KuiTaskInfo::~KuiTaskInfo()
{
	m_IndexIDIni.Clear();
	m_IndexData.Clear();
	nFindPath.clear();
	isOpen = false;
	colorLayer   = NULL;
	//CC_SAFE_RELEASE(m_array);
	if (red)
	    red->release();

}

//����ȷ��������
KuiTaskInfo* KuiTaskInfo::create(const char* title,
                                 Ref* callbackListener,
                                 const std::function<void(ax::Node*)>& callfun)
{
	KuiTaskInfo * popLayer = KuiTaskInfo::create();
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener,callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiTaskInfo::init()
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
	sprintf(nSprName,"\\Spr\\Ui3\\����\\���Ӱ���2.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return false;
	Sprite * spriteBox = Sprite::createWithTexture(bgCur);
	ParentNode_Task= spriteBox;
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
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void KuiTaskInfo::addDialogData()
{
	if  (!g_pCoreShell) return;

	/*pMoneyLabel = Label::createWithTTF("","fonts/gb_song.ttf", 12);//ͳ˧
	pMoneyLabel->setAnchorPoint(ax::Vec2(0,0));
	pMoneyLabel->setPosition(ax::Vec2(70,35));
	ParentNode_Task->addChild(pMoneyLabel,1);
*/
	m_pMainUiLayer = Layer::create();
	ParentNode_Task->addChild(m_pMainUiLayer);

	m_IndexIDIni.Load("\\Ui\\uitasklist.ini");
	AddTaskIndex();
	// Create the checkbox
	char nSprName[128]={0};
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\Spr\\Ui3\\���\\��ӿ���.spr");
	g_StrLower(nSprName);


	pmainListView = ListView::create();
	// set list view direction
	pmainListView->setDirection(ListView::Direction::VERTICAL);
	pmainListView->setTouchEnabled(true);
	pmainListView->setBounceEnabled(true);
	pmainListView->setBackGroundImage("loadingStart.png");
	pmainListView->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	pmainListView->setBackGroundImageOpacity(50);
	pmainListView->setContentSize(ax::Size(120, 250));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	pmainListView->setPosition(ax::Vec2(20,26));
	pmainListView->addEventListener(CC_CALLBACK_2(KuiTaskInfo::selectedMainListItemEvent, this));
	m_pMainUiLayer->addChild(pmainListView);
	pmainListView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);

	btnMsgSize.width  =245;
    btnMsgSize.height =16;

	ptaskMsglistView = ListView::create();
	// set list view direction
	ptaskMsglistView->setDirection(ListView::Direction::VERTICAL);
	ptaskMsglistView->setTouchEnabled(true);
	ptaskMsglistView->setBounceEnabled(true);
	ptaskMsglistView->setBackGroundImage("loadingStart.png");
	ptaskMsglistView->setBackGroundImageScale9Enabled(true);
	//listView->setBackGroundColorOpacity(0);
	ptaskMsglistView->setBackGroundImageOpacity(50);
	ptaskMsglistView->setContentSize(ax::Size(265, 250));
	//listView->setInnerContainerSize(ax::Size(110, 110));
	ptaskMsglistView->setPosition(ax::Vec2(150,26));
	ptaskMsglistView->addEventListener(CC_CALLBACK_2(KuiTaskInfo::selectedMsgListItemEvent, this));
	m_pMainUiLayer->addChild(ptaskMsglistView);
	ptaskMsglistView->setGravity(ListView::Gravity::CENTER_HORIZONTAL);

	btnNewSize.width = 100; //120
	btnNewSize.height= 28;
	btnCurSize.width = 100;
	btnCurSize.height= 16;
	/*Button* custom_button = Button::create();
	custom_button->setName("TextButton");
	custom_button->setTouchEnabled(true);
	custom_button->loadTextures("ui/state/life_normal.png", "ui/state/life_pk.png", "");
	custom_button->setScale9Enabled(true);
	custom_button->setContentSize(btnNewSize);

	Layout *custom_item = Layout::create();
	custom_item->setContentSize(btnNewSize);
	custom_button->setPosition(ax::Vec2(btnNewSize.width/2,btnNewSize.height/2));
	custom_item->addChild(custom_button);
	pmainListView->pushBackCustomItem(custom_item);
	custom_button->setTitleText(UTEXT("���Բ��Բ���",1));
	*/
	for (int i=0;i<__nCount;i++)
	{
		KTaskIndexNode *SelNode = m_IndexData.GetItem(i);
		if (!SelNode) continue;

		if (SelNode->nTaskId==-1)
		{//�Ǹ��ڵ�
			Button* custom_button = Button::create();
			custom_button->setName("TextButton");
			custom_button->setTouchEnabled(true);
			custom_button->loadTextures("ui/textbg_1.png", "ui/state/life_pk.png", "");
			custom_button->setScale9Enabled(true);
			custom_button->setContentSize(btnNewSize);

			Layout *custom_item = Layout::create();
			custom_item->setContentSize(btnNewSize);
			custom_button->setPosition(ax::Vec2(btnNewSize.width/2,btnNewSize.height/2));
			custom_item->addChild(custom_button);
			pmainListView->pushBackCustomItem(custom_item);
			custom_button->setTag(i+1);
			custom_button->setTag(-1);//�Ǹ��ڵ�
			custom_button->setUserData(SelNode);
			//pmainListView->insertCustomItem(custom_item,SelNode->nIndex+1);
			char nTempInfo[128];
			sprintf(nTempInfo,"%s",SelNode->szMsg);
			custom_button->setTitleText(UTEXT(nTempInfo,1));
			custom_button->setTitleColor(ax::Color3B::YELLOW);
		}
	}
	pmainListView->forceDoLayout();
	pmainListView->jumpToBottom();

	/*Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(ax::Size::ZERO);  //���úʹ��ڵĴ�С
	ParentNode_Task->addChild(colorLayer,1000);
	*/
	//����һ������ �ı侫����ɫ
	//red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	//red->retain();
}

void KuiTaskInfo::addmsgInfo(char *nMsgInfo,int nIsBtn,int nMsgIndex)
{
	if  (!ptaskMsglistView || !nMsgInfo) return;
	char nTempInfo[512];
	sprintf(nTempInfo,"%s",nMsgInfo);


	/*FontDefinition strokeTextDef;
	strokeTextDef.m_fontSize = 12;
	strokeTextDef.m_fontName = std::string(UI_GAME_FONT_SONG);//UI_GAME_FONT_DEFAULT
	strokeTextDef.m_stroke.m_strokeEnabled = true;
	strokeTextDef.m_stroke.m_strokeColor   = ax::Color3B::BLACK;
	strokeTextDef.m_stroke.m_strokeSize    = 1.5;
	strokeTextDef.m_fontFillColor          = ax::Color3B::WHITE;
	strokeTextDef.m_alignment              = TextHAlignment::CENTER;
	// stroke only label
	Label* custom_button = Label::createWithFontDefinition(UTEXT(nTempInfo,1).c_str(),strokeTextDef);
	*/
	//Label* custom_button = Label::createWithTTF(UTEXT(nTempInfo,1).c_str(),UI_GAME_FONT_DEFAULT,12);
	//custom_button->setTag("TextLabel");
	//custom_button->setTouchEnabled(true);
	//custom_button->loadTextures("ui/textbg_1.png", "ui/state/life_pk.png", "");
	//custom_button->setScale9Enabled(true);
	//custom_button->setContentSize(btnMsgSize);


	Size nRenderSize;
	nRenderSize.width  = btnMsgSize.width-5;
	nRenderSize.height = 12;
	RichText  * chatRickText = RichText::create();//WithParentSize(nParentSize);
	chatRickText->setContentSize(nRenderSize);
	chatRickText->ignoreContentAdaptWithSize(false);//�Ƿ����������Ӧ�߿��  ��һ��
	chatRickText->setAnchorPoint(ax::Vec2(0,0));

	RichElementText* nCurMsg = RichElementText::create(1,ax::Color3B::YELLOW,255,UTEXT(nTempInfo,1).c_str(),UI_GAME_FONT_DEFAULT,12);
	chatRickText->pushBackElement(nCurMsg);

	nRenderSize = chatRickText->getVirtualRendererSize();
	chatRickText->setContentSize(nRenderSize);

	Layout *custom_item = Layout::create();
	custom_item->setContentSize(nRenderSize);
	//custom_button->setPosition(ax::Vec2(btnMsgSize.width/2,btnMsgSize.height/2));
	chatRickText->setPosition(ax::Vec2(0,nRenderSize.height-20));
	custom_item->addChild(chatRickText,2,2);

	if (nIsBtn)
	{//�ǰ�Ť
		Button* custom_button = Button::create();
		custom_button->setName("TextButton");
		//custom_button->setItemDwidx(nItemDwidx);
		//custom_button->setItemName("");
		custom_button->loadTextures("ui/textbg.png", "ui/state/life_pk.png","");
		custom_button->setOpacity(0); //255Ϊ��ȫ��͸�� 0Ϊ��ȫ͸��
		custom_button->setAnchorPoint(ax::Vec2(0,0));
		custom_button->setScale9Enabled(true);
		Size nBtnSize;
		nBtnSize.width = nRenderSize.width - 50;
		nBtnSize.height= nRenderSize.height;
		custom_button->setContentSize(nBtnSize);
		custom_button->setTitleFontSize(12);

		//һ�еľͶ� ���г�������
//		int nRowS = chatRickText->getRenderRows();
		int nRowS = chatRickText->getVirtualRendererSize().height / chatRickText->getContentSize().height;
		if  (nRowS>1)
		{
			float nh = nRenderSize.height / nRowS; //ÿ�еĸ߶�
			custom_button->setPosition(ax::Vec2(0,chatRickText->getPosition().y - 3 - (nRowS - 1) * nh));
		}
		else
			custom_button->setPosition(ax::Vec2(0,chatRickText->getPosition().y-3));

		//��ʼ������
		//nFindPath[nMsgIndex]
		if (nMsgIndex>=0 && nMsgIndex < MAX_TASK_MESSAGE_MUM)
		     custom_button->setUserData(&nFindPath[nMsgIndex]);

		custom_item->addChild(custom_button,1,1);
	}

	ptaskMsglistView->pushBackCustomItem(custom_item);
	ptaskMsglistView->forceDoLayout();
	ptaskMsglistView->jumpToBottom();

}

void KuiTaskInfo::addmsgBtnInfo(KTaskPathInfo* pMsg)
{
	if  (!ptaskMsglistView || !pMsg) return;

	int nCurMsgidex=pMsg->nMsgIndex;

	if (nCurMsgidex>-1 && nCurMsgidex < MAX_TASK_MESSAGE_MUM)
	{
		nFindPath[nCurMsgidex].nMsgidx=pMsg->nMsgIndex;
		nFindPath[nCurMsgidex].nTsakType=pMsg->nTaskType;
		nFindPath[nCurMsgidex].nMapIdx=pMsg->nMapIdx;
		nFindPath[nCurMsgidex].nXpos=pMsg->nMapXpos;
		nFindPath[nCurMsgidex].nYpos=pMsg->nMapYpos;
	}
	else
		messageBox("�������������,����ϵGM����!","����");

	/*Button* custom_button = Button::create();
	custom_button->setName("TextButton");
	custom_button->setTouchEnabled(true);
	custom_button->loadTextures("ui/textbg_1.png", "ui/state/life_pk.png", "");
	custom_button->setScale9Enabled(true);
	custom_button->setContentSize(btnMsgSize);

	Layout *custom_item = Layout::create();
	custom_item->setContentSize(btnMsgSize);
	custom_button->setPosition(ax::Vec2(btnMsgSize.width/2,btnMsgSize.height/2));
	custom_item->addChild(custom_button);
	pmainListView->pushBackCustomItem(custom_item);
	//custom_button->setTag(i+1);
	//custom_button->setTag("-1");//�Ǹ��ڵ�
	//custom_button->setUserData(SelNode);
	//pmainListView->insertCustomItem(custom_item,SelNode->nIndex+1);
	char nTempInfo[128];
	sprintf(nTempInfo,"%s",nMsgInfo);
	custom_button->setTitleText(UTEXT(nTempInfo,1));
	custom_button->setTitleColor(ax::Color3B::YELLOW);
	*/

}

void KuiTaskInfo::AddTaskIndex()
{
	//KRankIndex  *pIncome;
	KTaskIndexNode Node;
	char szSec[32]={0};
	int i, nValue;
	//if(!m_IndexIDIni.IsSectionExist("Main"))
	__nCount =m_IndexIDIni.GetSectionCount();

	for(i = 0;i < __nCount;i++)
	{
		//memcpy(&Node, pIncome, sizeof(KRankIndex));
		Node.nIndex=i;
		sprintf(szSec, "%d", i);
		m_IndexIDIni.GetString(szSec, "Name","", Node.szMsg,MAX_TASK_MESSAGE_LENGTH);

		Node.nMsgLen = strlen(Node.szMsg);

		m_IndexIDIni.GetString(szSec, "Unit","", Node.szUnit, sizeof(Node.szUnit));
		m_IndexIDIni.GetInteger(szSec,"TaskId", -1, &nValue);
		Node.nTaskId = nValue;
		m_IndexIDIni.GetInteger(szSec,"Type", 0, &nValue);
		Node.nType = nValue;
		m_IndexIDIni.GetInteger(szSec,"Class", 0, &nValue);
		Node.nClass = nValue;
		m_IndexIDIni.GetInteger(szSec,"Trace", 0, &nValue);
		Node.nTrace = nValue;
		m_IndexIDIni.GetInteger(szSec,"Count", 0, &nValue);
		Node.nCount = nValue;
		m_IndexData.AddItem(Node);
	}
	//m_bIsIndexDataChange = TRUE;

}

void KuiTaskInfo::removeitems()
{
	if  (!pmainListView) return;
	auto items = pmainListView->getItems();
	if  (items.size() <= 0)  return;

	int items_count = items.size();
	for (int i=0;i<items_count;i++)
	{
		Widget *item = pmainListView->getItem(i);
		if  (item)
		{
			Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
			if  (button)
			{
				KTaskIndexNode *nCurData = (KTaskIndexNode *)button->getUserData();
				if (nCurData && nCurData->nTaskId!=-1)
				    pmainListView->removeItem(i);

				if (nCurData && nCurData->nTaskId==-1)
				{
                   if  (button->getCustomState())
				       button->setCustomState(false);
				}

				if (!nCurData)
					pmainListView->removeItem(i);
			}
			else
				pmainListView->removeItem(i);
		}
		pmainListView->forceDoLayout();
	}
}

void KuiTaskInfo::selectedMsgListItemEvent(Ref *pSender, ListView::EventType type)
{
	switch (type)
	{
        case ListView::EventType::ON_SELECTED_ITEM_START:
		{
			ListView* listViewEx = static_cast<ListView*>(pSender);
			if (listViewEx)
			{
				int nSelIndex_ = listViewEx->getCurSelectedIndex();

				Widget *item = listViewEx->getItem(nSelIndex_);
				if  (item)
				{
					Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
					if  (button)
					{
						nNewTaskFindPath * nCurPtr =(nNewTaskFindPath *)button->getUserData();
						if (nCurPtr)
						{//��ʼ����
							if (nCurPtr->nMapIdx>0)
							{
								if (g_pCoreShell)
								{
									KUiTongYong nPos;
									ZeroMemory(&nPos,sizeof(nPos));
									nPos.inKind=1;
									nPos.inParma=nCurPtr->nMapIdx;
									nPos.inParmb=nCurPtr->nXpos;
									nPos.inParmc=nCurPtr->nYpos;
									nPos.inParmd=0;
									if (g_pCoreShell)
									   g_pCoreShell->OperationRequest(GOI_PLAYER_SYSSHOP,(uintptr_t)&nPos,0);
								}
							}
						}
					}
				}
			}
		}
	    break;
	case ListView::EventType::ON_SELECTED_ITEM_END:
		break;
	default:
		break;
	}
}

void KuiTaskInfo::selectedMainListItemEvent(Ref *pSender, ListView::EventType type)
{
	char msg[64];
	switch (type)
	{
	case ListView::EventType::ON_SELECTED_ITEM_START:
		{
			if (ptaskMsglistView)
				ptaskMsglistView->removeAllItems();

			ListView* listViewEx = static_cast<ListView*>(pSender);
			if (listViewEx)
			{
				__nSelIndex = listViewEx->getCurSelectedIndex();

				Widget *item = listViewEx->getItem(__nSelIndex);
				if  (item)
				{
					Button *button = static_cast<Button*>(item->getChildByName("TextButton"));
					if  (button)
					{
						KTaskIndexNode *nCurData = (KTaskIndexNode *)button->getUserData();
						if (nCurData)
						{
							if (nCurData->nTaskId==-1)
							{
								/*char mTemp[64];
								sprintf(mTemp,"%d",nCurData->nIndex);
								messageBox(mTemp,"test");*/
								if (nCurData->nCount>0 && !button->getCustomState())
								{
									for (int i=0;i<nCurData->nCount;i++)
									{
										KTaskIndexNode *SelNode = m_IndexData.GetItem(nCurData->nIndex+1+i);
										if (!SelNode) continue;
										Button* custom_button = Button::create();
										custom_button->setName("TextButton");
										custom_button->setTouchEnabled(true);
										custom_button->loadTextures("ui/textbg_1.png", "ui/state/life_pk.png", "");
										custom_button->setScale9Enabled(true);
										custom_button->setContentSize(btnCurSize);

										Layout *custom_item = Layout::create();
										custom_item->setContentSize(btnCurSize);
										custom_button->setPosition(ax::Vec2(btnCurSize.width/2,btnCurSize.height/2));
										custom_item->addChild(custom_button);
										//custom_item->setPositionX(btnNewSize.width/2);
										//pmainListView->pushBackCustomItem(custom_item);
										//custom_button->setTag(i+1);
										//custom_button->setTag("-1");//�Ǹ��ڵ�
										custom_button->setUserData(SelNode);
										pmainListView->insertCustomItem(custom_item,__nSelIndex+1+i);
										char nTempInfo[128];
										sprintf(nTempInfo,"%s",SelNode->szMsg);
										custom_button->setTitleText(UTEXT(nTempInfo,1));
										custom_button->setTitleColor(ax::Color3B::WHITE);
									}
									pmainListView->forceDoLayout();
									button->setCustomState(true);
								}
								else
								{	//����Ǵ򿪵� �ڶ��ΰ��� ��ɾ��
									removeitems();
									removeitems();
									removeitems();
									removeitems();
									button->setCustomState(false);
								}
							}
							else
							{//��������
								/*char mTemp[64];
								sprintf(mTemp,"%d/%d",nCurData->nIndex,__nSelIndex);
								messageBox(mTemp,"test");*/
								if (nCurData->nTaskId>0)
								{
									KUiScrptinfo nVal;
									memset(&nVal, 0, sizeof(KUiScrptinfo));
									if (g_pCoreShell)
									   g_pCoreShell->GetGameData(GDI_SCRIPT_VAL_WITH, (intptr_t)&nVal,1,nCurData->nTaskId);  //ִ�нű����ص�����
									//UIMessageBox2(SelNode->szMsg);
								}
							}
						}
						/*if (button->getTag()=="-1")
						{//������Ǹ���Ŀ
							char mTemp[64];
							sprintf(mTemp,"%d",button->getTag());
							messageBox(mTemp,"test");
						}
						else
						{//�����������Ŀ
						}*/
					}
				}

				/*Button* custom_button = Button::create();
				custom_button->setName("TextButton");
				custom_button->setTouchEnabled(true);
				custom_button->loadTextures("ui/state/life_normal.png", "ui/state/life_pk.png", "");
				custom_button->setScale9Enabled(true);
				custom_button->setContentSize(btnNewSize);

				Layout *custom_item = Layout::create();
				custom_item->setContentSize(btnNewSize);
				custom_button->setPosition(ax::Vec2(btnNewSize.width/2,btnNewSize.height/2));
				custom_item->addChild(custom_button);
				pmainListView->insertCustomItem(custom_item,__nSelIndex+1);
				char nTempInfo[64];
				sprintf(nTempInfo,"���Բ���_%d",__nSelIndex+1);
				custom_button->setTitleText(UTEXT(nTempInfo,1));
				custom_button->setTitleColor(ax::Color3B::YELLOW);*/
				//pmainListView->forceDoLayout();
				//pmainListView->jumpToBottom();
			}
			break;
		}
	case ListView::EventType::ON_SELECTED_ITEM_END:
		//removeitems();
		break;
	default:
		break;
	}
}
//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiTaskInfo::setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;

	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui3\\������\\����\\����ָ��.spr");
	g_StrLower(nSprName);
	int m_nWidth,m_nHeight,nFrams;
	Texture2D *bgCur = NULL;
	SPRFRAMSINFO nSprInfo;
	ZeroMemory(&nSprInfo,sizeof(nSprInfo));

	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
//    ImageView *titleImg = ImageView::create();
    Sprite *titleImg = Scale9Sprite::createWithTexture(bgCur);
//	titleImg->loadTextureFromData(bgCur);
	titleImg->setPosition(ax::Vec2(m_size.width/2-7,m_size.height-12));
	m_pMainUiLayer->addChild(titleImg);

	//�ر�
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\Spr\\Ui3\\����\\���Ӱ���2-�ر�.spr");
	g_StrLower(nSprName);
	bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Leave_normal = Sprite::createWithTexture(bgCur);
	sprite_Leave_normal->setAnchorPoint(ax::Vec2(0,0));
	bgCur = _getinidata.getinidata_one(nSprName,2,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
	if (bgCur==NULL)
		return ;
	Sprite * sprite_Leave_select = Sprite::createWithTexture(bgCur);
	sprite_Leave_select->setAnchorPoint(ax::Vec2(0,0));

	MenuItemSprite * LeaveConfirm = MenuItemSprite::create(sprite_Leave_normal, sprite_Leave_select, CC_CALLBACK_1(KuiTaskInfo::btnCallBackFunc, this));
	LeaveConfirm->setPosition(ax::Vec2(m_size.width/2-9,13));
	LeaveConfirm->setTag(1);
	//����˵�
	Menu * menu = Menu::create(LeaveConfirm,NULL);
	menu->setAnchorPoint(ax::Vec2(0,0));
	menu->setPosition(ax::Vec2(0,0));             //�˵�λ������Ϊ����������
	ParentNode_Task->addChild(menu);         //�ӵ�������


	KUiPlayerTeam mTeam;
	ZeroMemory(&mTeam,sizeof(mTeam));
	if (g_pCoreShell->TeamOperation(TEAM_OI_GD_INFO, (uintptr_t)&mTeam, 0)) //û�ж��� �ʹ���һ֧����
	{//�ж���
	}
	else
	{//û�ж���
	}
}


//�����ť�ص�����
void KuiTaskInfo::oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type)
{
	if  (!g_pCoreShell || !g_GameWorld) return;

	switch (type)
	{
	case Widget::TouchEventType::BEGAN://Touch Down
		{
			buttonCallBackFunc(NULL);
		}
		break;
	case Widget::TouchEventType::MOVED://Touch Move
		break;
	case Widget::TouchEventType::ENDED://Touch Up
		{
			if (pSender)
			{
				Button* nTbtn = (Button*)pSender;
				//nTbtn->setTitleColor(ax::Color3B::WHITE);
				//if  (nTbtn->getTag()==1)
				//{

				//}
			}
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void KuiTaskInfo::btnCallBackFunc(Ref * pSender)
{
	if (!g_pCoreShell|| !g_GameWorld) return;

	Node * node = NULL;
	if (pSender)
	{
		node= dynamic_cast<Node *>(pSender);
		switch(node->getTag())
		{
		case 1:
			 buttonCallBackFunc(NULL);
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}
	}
}
//ִ���ϲ����Ļص��������رյ�����
void KuiTaskInfo::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node= dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiTaskInfo::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiTaskInfo::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
//	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,128, true);
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

Rect KuiTaskInfo::getRect(Node* pNode,int i)
{
	Rect  rc;
	rc.origin     = pNode->getPosition(); //��������ͼ��ƫ��
	//visibleSize
	rc.size       = pNode->getContentSize();

	rc.origin.x   = m_origin.x - m_size.width/2 + rc.origin.x; //��������
	rc.origin.y   = m_origin.y - m_size.height/2+ rc.origin.y; //���ϱ�    //- rc.origin.y

	return rc;
}

void KuiTaskInfo::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling) return;
	m_bScrolling = false;

	//__loopTime = MAX_LOOP_TIME;
}

//�ƶ���
void KuiTaskInfo::ccTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrolling || !__isCanMove) return;
	Point point   = ParentNode_Task->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Point diff    = point - m_lastPoint;                  //����ͼƬ��λ��
	ParentNode_Task->setPosition(ParentNode_Task->getPosition() + diff);
	m_origin      = ParentNode_Task->getPosition();
	m_lastPoint = point;
}
//��������ccTouchBegan������true
bool KuiTaskInfo::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	Point point = ParentNode_Task->convertToNodeSpace(ax::Director::getInstance()->convertToGL(pTouch->getLocationInView()));
	Rect rect = ax::Rect(0, 0, winSize.width/*ParentNode_Task->getContentSize().width*/,winSize.height/* ParentNode_Task->getContentSize().height*/);
	m_bScrolling = rect.containsPoint(point); //����ڷ�Χ�� �Ϳ����϶�
	m_lastPoint = point;
	return true;
}

//--------------------------------------------------------------------------
//	���ܣ����¶�����Ϣ
//--------------------------------------------------------------------------
void KuiTaskInfo::update(float delta)
{
	//messageBox("����ѭ��","update");
	if (isOpen && g_pCoreShell)
	{
	}
}
