//�а�ť�ġ������롡�Ի���
//
#include "gamecore/KCore.h"

#ifndef WIN32
#include <sys/select.h>
#include <ctype.h>
//typedef struct fd_set FD_SET;
#endif
#include "network/Socket.h"

#include "engine/KbugInfo.h"
#include "engine/CXmlStream.h"
#include "KuiAccreg.h"
#include "Klogin_f.h"
#include "engine/Text.h"
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
#include "engine/dataChecksum.h"

extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

KuiAccreg::KuiAccreg()
{
	isOpen = false;
	colorLayer        = NULL;
	red               = NULL;
	AccNode = NULL;
	PasNode = NULL;
	ePasNode = NULL;
	m_AnswerCount = 0;
	m_StartPos.x=0;
	m_StartPos.y=0;
	s = INVALID_SOCKET;
	nport=INVALID_SOCKET;
	naccIp="";
}

KuiAccreg::~KuiAccreg()
{
	colseSockets();
	isOpen = false;
	colorLayer        = NULL;
}

//����ȷ��������
KuiAccreg * KuiAccreg::create(Ref * callbackListener, const std::function<void(ax::Node*)>& callfun)
{
	KuiAccreg * popLayer = KuiAccreg::create();
	//popLayer->setTitle(title);
	popLayer->addDialogData();
	//���ð�ť
	popLayer->setcoloseButton(callbackListener, callfun);
	popLayer->isOpen = true;
	return popLayer;
}

//��ʼ��
bool KuiAccreg::init()
{
	if ( !Layer::init() ){
		return false;
	}
	//����������Ӧ
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(KuiAccreg::ccTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	//����֡����
	//SpriteFrameCache * sfCache = SpriteFrameCache::sharedSpriteFrameCache();
	//sfCache->addSpriteFramesWithFile("p_tips.plist");

	//���ģ̬����
	Size winSize = ax::Director::getInstance()->getWinSize();
	/*Color4B color(112, 128, 144, 150);
	LayerColor * colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	//colorLayer->setContentSize(winSize);//���úʹ��ڵĴ�С
	this->addChild(colorLayer);
	*/
	char nSprName[128]={0};
	ZeroMemory(nSprName,sizeof(nSprName));
	sprintf(nSprName,"\\spr\\Ui3\\������\\ͼ��\\�ʺ�ע��.spr");
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
	spriteBox->setPosition(ax::Vec2(winSize.width/2,winSize.height/2-bgCur->getContentSize().height/2));

	Color4B color(112, 128, 144, 150);
	colorLayer = LayerColor::create(color);//��ɫ��
	colorLayer->setPosition(ax::Vec2::ZERO);
	colorLayer->setContentSize(visibleSize);//���úʹ��ڵĴ�С
	colorLayer->addChild(spriteBox);
	this->addChild(colorLayer);

	//��ñ���ͼƬ�Ĵ�С
	Size contentSize = spriteBox->getContentSize();
	m_size = contentSize;
	m_origin = spriteBox->getPosition();
	//colorLayer->setContentSize(contentSize);//���úʹ��ڵĴ�С

	return true;
}

void KuiAccreg::addDialogData()
{
	Size nSize;
	nSize.width =128;
	nSize.height=24;
	m_StartPos.x =10;//+ITEM_CELL_SIZE/2;
	m_StartPos.y =140-24;//+ITEM_CELL_SIZE/2;
	//char nTempStr[128];
	//sprintf(nTempStr,pContent);
	/*Label *pstrLabel = Label::createWithTTF(UTEXT(nTempStr,1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,TextHAlignment::CENTER);//Arial
	pstrLabel->setColor(ax::Color3B::YELLOW);
	pstrLabel->setPosition(ax::Vec2(128/2+30,m_size.height-50));
	ParentNode_ItemEx->addChild(pstrLabel);*/

	/*Label *ptextLabel = Label::createWithTTF(UTEXT("�����Ļ�س�",1).c_str(),UI_GAME_FONT_DEFAULT,14,nSize,TextHAlignment::CENTER);//Arial
	ptextLabel->setColor(ax::Color3B::YELLOW);
	ptextLabel->setPosition(ax::Vec2(m_size.width/2,20));
	ParentNode_ItemEx->addChild(ptextLabel);*/

	//����һ������ �ı侫����ɫ
	//red = TintBy::create(0.2,0,-255,-255);//����ʱ��+��ɫ
	//red->retain();
	ax::ui::Scale9Sprite *pAccBg = ax::ui::Scale9Sprite::create("loadingEnd.png");  //"boxgb.png"
	pAccBg->setOpacity(0);

	AccNode = EditBox::create(Size(200,24),pAccBg);
	const ax::Color3B nfcolor ={255,255,255};
	if (AccNode)
	{
		//pEditBox->setContentSize()
		//AccNode->setText("input your account");
		AccNode->setPlaceHolder("input your account");
		AccNode->setFontColor(nfcolor);
		AccNode->setFontSize(24);
		AccNode->setMaxLength(8);
		AccNode->setInputFlag(ax::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
		AccNode->setInputMode(ax::ui::EditBox::InputMode::ANY);
		AccNode->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//AccNode->setDelegate((EditBoxDelegate*)this);
		AccNode->setPosition(ax::Vec2(m_size.width/2+16,135));
		ParentNode_ItemEx->addChild(AccNode,5);
	}

	//���������
	ax::ui::Scale9Sprite *pPassBg = ax::ui::Scale9Sprite::create("loadingEnd.png"); //"boxgb.png"
	pPassBg->setOpacity(0);
	PasNode = EditBox::create(Size(200,24),pPassBg);
	if (PasNode)
	{
		//pEditBox->setContentSize()
		PasNode->setPlaceHolder("input your password");
		//PasNode->setText("input your password");
		PasNode->setFontColor(nfcolor);
		PasNode->setFontSize(24);
		PasNode->setInputFlag(EditBox::InputFlag::PASSWORD);
		PasNode->setMaxLength(16);
		PasNode->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		//CCTouchDelegate *pDelegate
		//PasNode->setDelegate(this);
		PasNode->setPosition(ax::Vec2(m_size.width/2+16,100));
		ParentNode_ItemEx->addChild(PasNode,5);
	}
	//��������
	ax::ui::Scale9Sprite *pePassBg = ax::ui::Scale9Sprite::create("loadingEnd.png"); //"boxgb.png"
	pePassBg->setOpacity(0);
	ePasNode = EditBox::create(Size(200,24),pePassBg);
	if (ePasNode)
	{
		//ePasNode->setText("input your epassword");
		ePasNode->setPlaceHolder("input your epassword");
		ePasNode->setFontColor(nfcolor);
		ePasNode->setFontSize(24);
		ePasNode->setInputFlag(EditBox::InputFlag::PASSWORD);
		ePasNode->setMaxLength(16);
		//ePasNode->setDelegate(this);
		ePasNode->setReturnType(ax::ui::EditBox::KeyboardReturnType::DONE);
		ePasNode->setPosition(ax::Vec2(m_size.width/2+16, 65));
		ParentNode_ItemEx->addChild(ePasNode,5);
	}

	//
	char nSprName[128]={0};
	int nFrams=0,m_nWidth,m_nHeight;
	sprintf(nSprName,"\\spr\\Ui3\\loginui\\okbtn\\loginui_6_ok.spr");
	g_StrLower(nSprName);
	MenuItemImage *pCancelItem=NULL;
	MenuItemImage *pLoginItem=NULL;
	Texture2D *Login = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if  (Login)
	{
		char nSprFilePath[64]={0};
		unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
		Texture2D *pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		Texture2D *pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		Texture2D *pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pLoginItem = MenuItemImage::create();
        pLoginItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pLoginItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pLoginItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pLoginItem->setCallback(CC_CALLBACK_1(KuiAccreg::mianLoginCallback, this));

		pLoginItem->setPosition(ax::Vec2(origin.x + visibleSize.width/2 - pLoginItem->getContentSize().width/2-249,
			origin.y + pLoginItem->getContentSize().height/2+7));//һ����ť
	}

	nFrams=0;
	sprintf(nSprName,"\\spr\\Ui3\\loginui\\okbtn\\loginui_6_cancel.spr");
	g_StrLower(nSprName);
	Texture2D *Cancel = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if (Cancel)
	{
		char nSprFilePath[64]={0};
		unsigned long nFielpahtdwid = g_FileName2Id(nSprName);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
		Texture2D *pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		Texture2D *pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		Texture2D *pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pCancelItem = MenuItemImage::create();
        pCancelItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pCancelItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pCancelItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pCancelItem->setCallback(CC_CALLBACK_1(KuiAccreg::mianCancelCallback, this));

		pCancelItem->setPosition(ax::Vec2(origin.x + visibleSize.width/2 - pCancelItem->getContentSize().width/2-99,
			origin.y + pCancelItem->getContentSize().height/2+7));//һ����ť
	}

	Menu* pMenu=NULL;
	if (Login && Cancel)
	{
		pMenu = Menu::create(pLoginItem,pCancelItem,NULL);
		pMenu->setPosition(ax::Vec2::ZERO);//
		ParentNode_ItemEx->addChild(pMenu,2);
	}
	//��ȡ��½���ķ���������������
	m_WritablePath = ax::FileUtils::getInstance()->getWritablePath();
	KIniFile pPakList;
	if (pPakList.Load("data/paklist.ini"))
	{
		char nstrName[64];
		char nstrmak[64]={0},bstrmak[64]={0};
		ZeroMemory(nstrmak,sizeof(nstrmak));
		ZeroMemory(bstrmak,sizeof(bstrmak));
		dataChecksum njjj;
		pPakList.GetString("list","accinfo","",nstrName,sizeof(nstrName));
		njjj.SimplyDecrypt(bstrmak,nstrName);
		naccIp = bstrmak;
		pPakList.GetString("list","accpint","",nstrName,sizeof(nstrName));
		njjj.SimplyDecrypt(nstrmak,nstrName);
		nport = g_Atoui(nstrmak);

		pPakList.Clear();
	}
}

void KuiAccreg::mianCancelCallback(Ref* pSender)
{
	buttonCallBackFunc(NULL);
	/*if (nAcclient)
	{
		m_bIsClientConnecting=false;
		m_bIsPlayerInGame =false;
		nAcclient->Cleanup();

	}
	ax::Director::getInstance()->replaceScene(Klogin_f::scene());
	*/
}

void KuiAccreg::mianLoginCallback(Ref* pSender)
{
	std::string strAcc;
	std::string strPas;
	std::string strePas;

	//EditBox *pPassEditBox
	if (AccNode && PasNode && ePasNode)
	{
		strAcc  = AccNode->getText();
		strPas  = PasNode->getText();
		strePas = ePasNode->getText();
		if (strAcc.length()>16 || strAcc.length()<= 0 || strPas.length()>16 || strPas.length()<=0 || strePas.length()>16 || strePas.length()<=0)
		{
#ifdef WIN32
			messageBox("�ʺŻ�����,����Ϊ�ղ����Ȳ��ܴ���16","����");
#else
			messageBox(G2U("�ʺŻ�����,����Ϊ�ղ����Ȳ��ܴ���16").c_str(),G2U("����").c_str());
#endif
			return;
		}
	}
	else
	{
#ifdef WIN32
		messageBox("�������󣮣�","����");
#else
		messageBox(G2U("�������󣮣�").c_str(),G2U("����").c_str());
#endif
		return;
	}
	//��ʼ����ַ��ĺϷ���
	std::string strcheck="check";
	strcheck+=strAcc;
	strcheck+=strPas;
	strcheck+=strePas;
	int count = 0,spaceNum=0,chineseNum=0,other=0,letter=0,xiaoxie=0;
	for(int i = 0; i <strcheck.length();i++)
	{
		//  if(str[i] < 0 && str[i] > 9);
		if(isdigit(strcheck[i])) //�ж��Ƿ�������
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
			++chineseNum;    //�ж��Ƿ�������
		else
		{//���ź���ע�� ������������
		    ++other;
		}
	}
if (_clientlanguage==1)
{//����
	if (spaceNum>0 || chineseNum >0 || other>0 || letter>0)
	{ 	//system("pause");
		//AfxMessageBox("���зǷ��ַ����������������룬���ܺ������ģ��ո񣬴�д��ĸ��������ţ�");
#ifdef WIN32
		messageBox("���зǷ��ַ�,��������,����\n���ܺ�������,�ո�,��д��ĸ���������!","����");
#else
		messageBox(G2U("���зǷ��ַ�,��������,����\n���ܺ�������,�ո�,��д��ĸ���������!").c_str(),G2U("����").c_str());
#endif
		return;
	}
}
else
{//�����汾
	if (spaceNum>0 || chineseNum >0 || other>0)
	{ 	//system("pause");
		//AfxMessageBox("���зǷ��ַ����������������룬���ܺ������ģ��ո񣬴�д��ĸ��������ţ�");
#ifdef WIN32
		messageBox("���зǷ��ַ�,��������,����\n���ܺ�������,�ո�,��д��ĸ���������!","����");
#else
		messageBox("Invalid character,please check your name or password��\n Cannot contain special symbols such as Chinese, spaces, etc.!","Warning");
#endif
		return;
	}
}
	//��ʼ���ӷ�����
	//��ʼ����һ�����ӽ�
	//char msg[64];
	//sprintf(msg,"%d",nport);
	//messageBox(naccIp.c_str(),msg);
	s = socket(AF_INET, SOCK_STREAM,IPPROTO_IP);

	if  (s==INVALID_SOCKET)
	{
#ifdef WIN32
		messageBox("���ӷ�����ʧ��","����");
#else
		messageBox(G2U("���ӷ�����ʧ��").c_str(),G2U("����").c_str());
#endif
		ax::Director::getInstance()->replaceScene(Klogin_f::scene());
		return;
	}

/*#ifndef WIN32
	int flags = fcntl(s,F_GETFL,0);                        //��ȡ�ļ���flagsֵ��
	fcntl(s,F_SETFL,flags|O_NONBLOCK);                     //���óɷ�����ģʽ��
#endif*/

	KSG_PASSWORD pcPassword;
	ZeroMemory(&pcPassword,sizeof(pcPassword));
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(naccIp.c_str());
	svraddr.sin_port = htons(nport);
	int ret =::connect(s,(struct sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR)
	{
#ifdef WIN32
		messageBox("���ӷ�����ʧ��A","����");
#else
		messageBox(G2U("���ӷ�����ʧ��A").c_str(),G2U("����").c_str());
#endif
		return;
	}
	std::string strMsg = "SEND MSG";
	strMsg +=strAcc;
	strMsg +=" ";
	KSG_StringToMD5String(pcPassword.szPassword, strPas.c_str());
	strMsg +=pcPassword.szPassword;   //����
	strMsg +=" ";
	ZeroMemory(&pcPassword,sizeof(pcPassword));
	KSG_StringToMD5String(pcPassword.szPassword, strePas.c_str());
	strMsg +=pcPassword.szPassword;   //��������
	strMsg +=" ";
	strMsg +="ENDMSG";
	strMsg +=" ";

	int nLen = strMsg.length();
	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(s,&fd);
	//FD_SET fd = {1,s};
	//memset( &writefds, 0, sizeof( writefds ) );
	//fd.fd_count = 1;
	//fd.fd_array[0] = s;
	TIMEVAL tv = {TIME_OUT,0};
	if(select(s+1,NULL,&fd,NULL,&tv) == 0)
	{
		colseSockets();
#ifdef WIN32
		messageBox("�ύע�����볬ʱ","����");
#else
		messageBox(G2U("�ύע�����볬ʱ").c_str(),G2U("����").c_str());
#endif
		return;
	}

	int nBytesSent;
	if((nBytesSent = ::send(s,strMsg.c_str(),nLen,0)) == SOCKET_ERROR)
	{
#ifdef WIN32
		messageBox("�ύע������ʧ��","����");
#else
		messageBox(G2U("�ύע������ʧ��").c_str(),G2U("����").c_str());
#endif
		colseSockets();
		return;
	}
	if (nBytesSent == nLen)          // ���ͳɹ�
	{
#ifdef WIN32
		messageBox("�ύע������ɹ�","����");
#else
		messageBox(G2U("�ύע������ɹ�").c_str(),G2U("����").c_str());
#endif
		char m_pReadBuf[256]={0};
		//ѭ���ȴ�����������Ӧ��Ϣ
		while(1)
		{//���������ݻ���������
			memset(m_pReadBuf,0,sizeof(m_pReadBuf));
			if(select(s+1,&fd,NULL,NULL,&tv) == 0)
			{
#ifdef WIN32
				messageBox("ע��ʧ��A","����");
#else
				messageBox(G2U("ע��ʧ��A").c_str(),G2U("����").c_str());
#endif
				colseSockets();
				return;
			}
			//��������
			int nBytesReceived;
			if((nBytesReceived =recv(s,m_pReadBuf,sizeof(m_pReadBuf),0)) == SOCKET_ERROR)
			{
#ifdef WIN32
				messageBox("ע��ʧ��B","����");
#else
				messageBox(G2U("ע��ʧ��B").c_str(),G2U("����").c_str());
#endif
				colseSockets();
				return;
			}
			//������ܵ������ݳ��ȴ���0�����˳�ѭ��������ѭ���ȴ�
			if (nBytesReceived > 0)
				break;
		};
		char strCommand[9];
		memset(strCommand,0,9);          //����
		strncpy(strCommand,m_pReadBuf,7);//����m_pReadBufǰ�߸��ַ��ܵ�strCommand
		if (strcmp(strCommand,"ACK MSG") == 0)   //ֻ����������˶�ԭ��Ϣ��Ӧ������  �Ƚϴ�С����
		{
#ifdef WIN32
			messageBox("ע��ɹ�","����");
#else
			messageBox(G2U("ע��ɹ�").c_str(),G2U("����").c_str());
#endif
			ax::Director::getInstance()->replaceScene(Klogin_f::scene());
		}
		else if (strcmp(strCommand,"YCK MSG")== 0)
		{
#ifdef WIN32
			messageBox("���û����Ѿ�ע�ᣡ","����");
#else
			messageBox(G2U("���û����Ѿ�ע�ᣡ").c_str(),G2U("����").c_str());
#endif
		}
		else if (strcmp(strCommand,"IGNORED")== 0)
		{
#ifdef WIN32
			messageBox("ע��ʧ�ܣ�����ϵGM����","����");
#else
			messageBox(G2U("ע��ʧ�ܣ�����ϵGM����").c_str(),G2U("����").c_str());
#endif
		}
	}
	colseSockets();
	//messageBox(naccIp.c_str(),"���ӷ������ɹ�");
}


// �ر�Socket
void KuiAccreg::colseSockets()
{
	LINGER lingerStruct;
	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0;
	if (SOCKET_ERROR == ::setsockopt(s,
		SOL_SOCKET,
		SO_LINGER,
		(char *)&lingerStruct,
		sizeof(lingerStruct)))
	{
	}
#ifdef WIN32
	if(closesocket(s) == SOCKET_ERROR)
		s = INVALID_SOCKET;
#else
	close(s);
#endif
	s = INVALID_SOCKET;
}

//����ȷ��ȡ����ť,���������ò���󣬵��ò�ص�����
void KuiAccreg::setcoloseButton(Ref * callbackListener, const std::function<void(ax::Node*)>& callfun)
{
	m_callbackListener = callbackListener;
	m_callfun = callfun;
}
//ִ���ϲ����Ļص��������رյ�����
void KuiAccreg::buttonCallBackFunc(Ref * pSender)
{
	Node * node = NULL;
	if (pSender)
		node = dynamic_cast<Node *>(pSender);
	//node->setTag(3000);//����tag���ڵ��ò���Ի�ȡ��
	if (m_callfun &&  m_callbackListener)
		(m_callfun)(node);

	this->removeFromParentAndCleanup(true);
}

//�رյ�����
void KuiAccreg::closePopLayer(Ref * pSender)
{
	this->removeFromParentAndCleanup(true);
}

//��д����ע�ắ�������¸�����������
void KuiAccreg::registerWithTouchDispatcher()
{
	//����Ĵ������ȼ�����Ϊ��128����CCMenuͬ������֤�������·��Ĵ���
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
//	//ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,2, true);
}

Rect KuiAccreg::getRect(Node* pNode,int i)
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
bool KuiAccreg::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	//CCSetIterator it = pTouch->begin();
	//Touch* touch = (Touch*)(*it);
	//containsPoint(convertTouchToNodeSpaceAR(touch))
	Point location = pTouch->getLocation();//Ŀ�ĵ�����?��Ļ�����
	//case SMCT_UI_RENASCENCE:	             //ѡ������
    //buttonCallBackFunc(NULL);
	return true;
}

void KuiAccreg::update(float delta)
{
	//messageBox("����ѭ��","update");
	if (isOpen)
	{
	   // m_nMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
	  //	m_nXu    = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
	}
}

int KuiAccreg::FilterTextColor(char* pMsgBuff, unsigned short nMsgLength)
{
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_ENTER);          //��ֹ�س�����
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_COLOR);          //��ֹ��ɫ����
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_COLOR_RESTORE);  //��ֹ��ɫ��ԭ
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_BORDER_COLOR);   //��ֹ�������
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_BORDER_RESTORE); //��ֹ�������
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_INLINE_PIC);     //��ֹ��ͼ����
	nMsgLength = TClearSpecialCtrlInEncodedText(pMsgBuff, nMsgLength, KTC_LINK);           //��ֹ��������
	//	return TEncodeText(pMsgBuff, nMsgLength);
	return  nMsgLength;
}

void KuiAccreg::editBoxEditingDidBegin(EditBox *editBox)//��ʼʱ�ĺ���
{
	messageBox("�����ʼ","�����ʼ");
}
void KuiAccreg::editBoxEditingDidEnd(EditBox *editBox) //����ʱ�ĺ���
{

}
void KuiAccreg::editBoxReturn(EditBox *editBox)         //����������س�ʱ�Ĵ���
{

}
void KuiAccreg::editBoxTextChanged(EditBox *editBox, const std::string &text)//���ָ��ĺ���
{

}
