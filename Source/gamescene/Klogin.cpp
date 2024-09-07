//#include "Klogin.h"
//#include "engine/KGbktoUtf8.h"
//#include "engine/KbugInfo.h"
//#include "Klogin.h"
//#include "engine/CXmlStream.h"
//#include "engine/KIniFile.h"
//#include "gamecore/CoreShell.h"
//#include "gameui/PopupLayer.h"
////#include "engine/KPalette.h"
//#include "engine/KCodec.h"
//#include "engine/KCodecLzo.h"
//#include "engine/KFile.h"
//#include "engine/KFilePath.h"
//#include "engine/KList.h"
//#include "engine/KMemBase.h"
//#include "engine/KMemClass.h"
//#include "engine/KMemStack.h"
//#include "engine/KNode.h"
//#include "engine/KPakFile.h"
//#include "engine/KStrBase.h"
//#include "engine/KPakList.h"
//#include "engine/XPackFile.h"
//#include "gamecore/KSubWorldSet.h"
//#include "../gamecore/KCore.h"
//#include <iostream>
//#include <fstream>
//
//#ifdef WIN32
//#include <windows.h>
//#else
//#include <unistd.h>
//#endif
//
//extern iCoreShell*	g_pCoreShell; //ȫ�ֵ���
//extern KImageStore2 m_ImageStore; //ȫ�ֵ���
////extern KPakList     g_pPakList;
////extern KPakList * g_pPakList;
//#include <list>
//USING_NS_AX;
//
//Scene* Klogin::scene()
//{
//	// 'scene' is an autorelease object
//	Scene *scene = Scene::create();
//
//	// 'layer' is an autorelease object
//	Klogin *layer = Klogin::create();
//
//	// add layer as a child to scene
//	scene->addChild(layer);
//
//	// return the scene
//	return scene;
//}
//
//// on "init" you need to initialize your instance
//bool Klogin::init()
//{
//	//////////////////////////////
//	// 1. super init first
//	if ( !Layer::init() )
//	{
//		return false;
//	}
//
//    visibleSize        = ax::Director::getInstance()->getVisibleSize();//�ɼ���
//	origin             = ax::Director::getInstance()->getVisibleOrigin();
//	Size size        = ax::Director::getInstance()->getWinSize();
//	m_sprCount         =0;
//	nTimeCount = 0;
//	m_nWidth   = 0;
//	m_nHeight  = 0;
//	pWW        =NULL;
//	animate    =NULL;
//
//    auto touchListener = ax::EventListenerTouchOneByOne::create();
//    touchListener->onTouchBegan = CC_CALLBACK_2(Klogin::ccTouchBegan, this);
//    touchListener->onTouchMoved = CC_CALLBACK_2(Klogin::ccTouchMoved, this);
//    touchListener->onTouchEnded = CC_CALLBACK_2(Klogin::ccTouchEnded, this);
//    touchListener->onTouchCancelled = CC_CALLBACK_2(Klogin::ccTouchCancelled, this);
//
//    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
//
//	if (!m_ImageStore.Init())  //��ʼ��ͼƬ������
//		messageBox(UTEXT("�ռ䲻��", 1).c_str(),"gameclient");
//	if (g_pCoreShell)
//	{
////		if (g_pCoreShell->test())
////		  messageBox("test is suss","g_pCoreShell");
//	}
//	/////////////////////////////
//	// 2. add a menu item with "X" image, which is clicked to quit the program
//	//    you may modify it.
//
//	// add a "close" icon to exit the progress. it's an autorelease object
//	auto pCloseItem = ax::MenuItemImage::create(
//		"mainskillmix.png",
//		"mianskillbig.png",
//        CC_CALLBACK_1(Klogin::mianSkillCallback, this)); //�ص�����
//
//	    pCloseItem->setPosition(ax::Vec2(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2-20 ,
//		origin.y + pCloseItem->getContentSize().height/2+20));
//	// create menu, it's an autorelease object
//	Menu* pMenu = Menu::create(pCloseItem, NULL);
//	pMenu->setPosition(ax::Vec2::ZERO);
//	m_sprCount ++;
//	this->addChild(pMenu, 1, m_sprCount);
//	/////////////////////////////
//	// 3. add your codes below...
//	// add a label shows "Hello World"
//	// create and initialize a label
//	const char * strTiele = CXmlStream::GetStringByKeyFromFile("string.xml","title"); //Resources ��Դ����Ŀ¼��,���û������ļ���������
//	KIniFile test;
//	//KPakList g_pPakList;
//	char strtest[32]={0};
//	//FileUtils::getPathForFilename()
//	std::string nPath = ax::FileUtils::getInstance()->getWritablePath();
//
//	//messageBox(nPath.c_str(),"getWritablePath");//�ɶ�д·�� /data/data/����/files/
//
//	//nPath = ax::FileUtils::getInstance()->fullPathForFilename("package.ini");
//	//nPath = ax::FileUtils::getInstance()->getWritablePath();//getWriteablePath();
//	//nPath += "download";
//	int nCount = 0;
//	//tolower() //��дתСд
//	//toupper() //Сдת��д
//	//getExtSDCardPath();
//	//Environment.getExternalStorageDirectory();
//
//	//Context::getCacheDir();
//	//Environment.getExternalStorageDirectory().toString() + "/" + "Tianchaoxiong/useso";
//	//AssetManager::InputStream.open();
//	//pContext.getAssets();
//	//getAssetManager();
//	std::string fielDir;
//	fielDir = "package.ini";
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//	fielDir = "download\\package.ini";
//#endif
//	//nCount = g_pPakList.Open(fielDir.c_str());  //����PAK
//	//FileUtils::getFullPathForDirectoryAndFilename("Resources","test.ini");
//	//fullPathForFilename(filename.c_str());
//	if (test.Load(fielDir.c_str()))  //"D:\\cocos2dx226\\projects\\jxclient\\Resources\\test.ini"
//	{
//		test.GetString("Package","Path","world",strtest,sizeof(strtest));
//	}
//	test.Clear();
//	start.Start();
//	char nCurStr[128]={0};
//	sprintf(nCurStr,"(%s)%d \ntime:%u",strtest,nCount,start.GetElapse());
//	//Label*
//	pMainLabel = Label::createWithTTF(strTiele,UI_GAME_FONT_DEFAULT, 24);
//	pMainLabel->setString(nCurStr);//������ʾ������
//	//messageBox(nPath.c_str(),"title");
//	//Label* pLabel = Label::createWithTTF("Hello World", "Arial", 24);
//	// position the label on the center of the screen
//	pMainLabel->setPosition(ax::Vec2(origin.x + visibleSize.width/2,
//		origin.y + visibleSize.height - pMainLabel->getContentSize().height));
//
//	// add the label as a child to this layer
//	//m_sprCount++;
//	this->addChild(pMainLabel, 1);
//
//	Sprite *bgSprite=Sprite::create("bg.jpg");
//	bgSprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//	float winw = visibleSize.width;   //��ȡ��Ļ���
//	float winh = visibleSize.height;  //��ȡ��Ļ�߶�
//	float spx  = bgSprite->getTextureRect().getMaxX();
//	float spy  = bgSprite->getTextureRect().getMaxY();
//	bgSprite->setScaleX(winw/spx); //���þ��������ű���
//	bgSprite->setScaleY(winh/spy);
//	//m_sprCount++;
//	//this->addChild(bgSprite,0,m_sprCount);
//
//	MenuItemFont* itemReset = MenuItemFont::create(UTEXT("�˳�", 1).c_str(), CC_CALLBACK_1(Klogin::beiyongb, this));
//	//itemReset->setFontName("fonts/����������.TTF");
//	itemReset->setPosition(ax::Vec2(visibleSize.width/2, 50));
//	itemReset->setFontSize(16);
//
//	MenuItemFont* itemGetClientVersion = MenuItemFont::create(UTEXT("����", 1).c_str(), CC_CALLBACK_1(Klogin::beiyonga, this));
//	//itemGetClientVersion->setFontName("fonts/����������.TTF");
//	itemGetClientVersion->setPosition(ax::Vec2(visibleSize.width/2, 100));
//	itemGetClientVersion->setFontSize(16);
//	MenuItemFont* itemGetServerVersion = MenuItemFont::create(UTEXT("����NPC", 1).c_str(), CC_CALLBACK_1(Klogin::nextsenc, this));
//	itemGetServerVersion->setPosition(ax::Vec2(visibleSize.width/2, 150));
//	itemGetServerVersion->setFontSize(16);
//	MenuItemFont* itemUpdateVersion = MenuItemFont::create(UTEXT("ɾ������", 1).c_str(), CC_CALLBACK_1(Klogin::delspr, this));
//	itemUpdateVersion->setPosition(ax::Vec2(visibleSize.width/2, 200));
//	itemUpdateVersion->setFontSize(16);
//	MenuItemFont* itemEnterScene = MenuItemFont::create(UTEXT("���Ӿ���", 1).c_str(), CC_CALLBACK_1(Klogin::addspr, this));
//	itemEnterScene->setPosition(ax::Vec2(visibleSize.width/2, 250));
//	itemEnterScene->setFontSize(16);
//
//	MenuItemFont* itemaddmaop = MenuItemFont::create(UTEXT("���ص�ͼ", 1).c_str(), CC_CALLBACK_1(Klogin::addmap, this));
//	itemaddmaop->setPosition(ax::Vec2(visibleSize.width/2, 300));
//	itemaddmaop->setFontSize(16);
//	itemaddmaop->setTag(1);   //���ñ�־
//	//CCAssert(,"");//����
//	Menu *newmenu=Menu::create(itemaddmaop,itemEnterScene,itemUpdateVersion,itemGetServerVersion,itemGetClientVersion,itemReset,NULL);  // �˲��ܹؼ�������ֻ�ǲ˵�������ܵ��
//	newmenu->setPosition(ax::Vec2(origin.x+70, visibleSize.height/2 + origin.y+150));//�����˵���λ��
//	newmenu->alignItemsVertically();       //����Ϊ��������
//	//m_sprCount++;
//	this->addChild(newmenu,2);             //
//	Node * d = newmenu->getChildByTag(1);//��ȡ���ڵ��µ��ӽڵ�
//	//newmenu->removeChild(d,true);          //ɾ�����ڵ��µ��ӽڵ�
//	//newmenu->removeChildByTag(1);
//	//itemaddmaop->removeFromParentAndCleanup(true);  //
//	d->removeFromParentAndCleanup(true);
//	//d->release();//�ͷ����ڴ档������
//
//	if (itemaddmaop)
//	   newmenu->addChild(itemaddmaop,true);  //���ڵ�������һ���ӽڵ�
//	//����һ�������
//	//CCComponent::create();
//	//newmenu->setUserData();//Ϊĳ���ڵ���Զ�������
//	//��Ⱦ���� ͼƬ����
//	/*
//	List<Texture2D> loadTexture = new list_t<Texture2D>();
//	List<Sprite> loadsprite     = new list_t<Sprite>();
//	*/
//	m_pTexture = NULL;
//    int nFrams = 0;
//	m_pTexture = new Texture2D();
//
//	if (m_pTexture)
//	{//�����￪ʼ��ȡͼ�����ݵ�ָ��data
//		//unsigned char *data = NULL;
//		//unsigned long m_size = 0;
//		//memset()
//		/*CCImage* image = new CCImage();
//
//		if (!image)
//		{
//			messageBox("image a","title");
//			return false;
//		}
//		//bool bRet = image->initWithImageFile("bg.jpg",CCImage::kFmtJpg);
//		*/
//		//std::string bFilePath = ax::FileUtils::getInstance()->fullPathForFilename("laohuji.spr");//("MA_BD_075_ST01.spr");//
//		/*FILE *fp=NULL;
//		    fp = ax::FileUtils::getInstance()->getFilePtr("laohuji.spr", "rb", &m_size);
//		if (!fp)
//		{
//			messageBox("fp is NULL","title");
//			return true;
//		}
//		*/
//
//		//char nSprInfo[128]={0};
//		//sprintf(nSprInfo,"m_nWidth:%d,m_nHeight:%d,m_nFrames:%d %d/%d\n��ɫ:%d ƫ��:%d/%d",m_nWidth,m_nHeight,m_nFrames,pFrame->Width,pFrame->Height,m_nColors,pFrame->OffsetX,pFrame->OffsetY);
//		//messageBox(nSprInfo,"title");
//	     //RenderToA4R4G4B4Alpha2(pW,(BYTE *)pNode->m_pBitmap,pNode->m_nWidth,pNode->m_nHeight,(BYTE *)pNode->m_pPalette,m_ptDes,m_rtSrc,nPitch,pCanvas->m_nMask32);
//		//SprDecode.RenderToSprDecode(pW,(BYTE *)pFrame->Sprite,m_nWidth,m_nHeight,m_Palette);
//
//		 Texture2D *Cur = _getinidata.getinidata_new("\\spr\\skill\\1502\\kl_150_jiankun_fu.spr", 10, &m_nWidth, &m_nHeight, &nFrams);
//		//SprDecode.RenderToSprDecode("laohuji.spr",0,pW);\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr
//		/*stImageInfo m_info;
//		m_info.bytes_per_pixel=2;
//		m_info.width          =m_nWidth;
//		m_info.height         =m_nHeight;
//		m_info.buffer         = (BYTE *)pW;
//		SprDecode.SaveBMPFile("D://test.bmp",m_info);*/
//		//---------------
//		/*��Ҫ���� png ���ݸ�ʽ
//		bool bRet = image->initWithImageData(pFrame->Sprite,m_size, CCImage::kFmtJpg);
//		if (!bRet)
//		{
//			messageBox("load bg.jpg er","title");
//			return true;
//		}
//		bool ret = false;
//		     ret = m_pTexture->initWithImage(image);
//	    */
//		//Size imageSize =ax::Size((float)(m_nWidth), (float)(m_nHeight));
//
//		/*RenderTexture* renderTexture=RenderTexture::create(m_nWidth, m_nHeight);//ָ�������Ĵ�С
//		renderTexture->beginWithClear(0, 23, 5, 200);                    //����begin()��ʼ�ڻ����ϻ�������
//	    renderTexture->end();                                            //��Ⱦ���
//		renderTexture->getSprite()->getTexture()->initWithData(pWW,(CCTexture2DPixelFormat)kTexture2DPixelFormat_RGBA8888,m_nWidth,m_nHeight,imageSize);
//		Sprite * dynamicTexture=Sprite::createWithTexture(renderTexture->getSprite()->getTexture());//ʹ�ö�̬����������
//		dynamicTexture->setPosition(ax::Vec2(m_nWidth + origin.x+400, visibleSize.height/2 + origin.y));
//	    this->addChild(dynamicTexture);*/
//
//		//std::string bFilePath = ax::FileUtils::getInstance()->fullPathForFilename("bg.jpg");
//		//data = ax::FileUtils::getInstance()->getFileData(bFilePath.c_str(), "rb", &m_size);
//
//		//imageSize.width = visibleSize.width;
//		//imageSize.height= visibleSize.height;
//		//m_pTexture->initWithData(pWW,(CCTexture2DPixelFormat)kTexture2DPixelFormat_RGBA8888,m_nWidth,m_nHeight,imageSize); //ax::Size((float)w, (float)h)
//		/*Rect rect;
//			 rect.size = ax::Size((float)(100), (float)(200));
//			 rect.origin.x =0;
//			 rect.origin.y =0; �ϻ������
//		*/
//		//messageBox(m_pTexture->description(),"title");
//		testSprite = NULL;
//		if (Cur)
//		{
//			/*char nSprFilePath[256]={0};
//			sprintf(nSprFilePath,"%s_%d","kl_150_jiankun_fu.spr",10);
//			Texture2D* OldText=NULL;
//			if (OldText = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))
//			   messageBox(Cur->description(),"title");*/
//			testSprite=Sprite::createWithTexture(Cur);
//			testSprite->setPosition(ax::Vec2(m_nWidth + origin.x+100, visibleSize.height/2 + origin.y));
//			m_sprCount++;
//			this->addChild(testSprite,2,m_sprCount);
//
//			char nSprFilePath[64]={0};
//			Animation*animation = Animation::create();//����һ������
//
//            //std:string nFileName ="\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr";
//			//g_StrLower(nFileName);
//			unsigned long nFielpahtdwid = g_FileName2Id("\\spr\\skill\\1502\\kl_150_jiankun_fu.spr");
//
//			for (int i=1;i<21;i++)
//			{
//				//Texture2D* nCurCount=NULL;
//				sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,i);
//				messageBox(nSprFilePath,"animation");
//				//nCurCount = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);//������ڵ� ��
//				//if (nCurCount)
//				//testSprite->setTexture(nCurCount);//���û�� ���Զ�����һ��
//				animation->addSpriteFrameWithFile(nSprFilePath);//�����������������֡
//			}
//			//���ö���֡��ʱ����
//			animation->setDelayPerUnit(0.02f);
//			//���ò���ѭ�� һֱ���� Ϊ-1
//		    animation->setLoops(-1);
//			//���ö���������ָ�����һ֡ setRestoreOriginalFrame
//			animation->setRestoreOriginalFrame(TRUE);
//			// ����һ����Զѭ���Ķ���
//			animate = Animate::create(animation);
//
//			//RepeatForever* forever = RepeatForever::create(animate);
//			if (testSprite){
//                //testSprite->runAction(animate);
//                auto callFunc = ax::CallFunc::create([this]() {
//                    this->finish();
//                });
//
//                testSprite->runAction(Sequence::create(animate, callFunc, NULL));
//            }
//			//if(testSprite->isActionDone(animate))
//			 // removeChild(testSprite,true);
//			//CCMoveTo* move = CCMoveTo::create(30, ax::Vec2(-20, 200));
//			//testSprite->runAction(move);
//			//removeChild(testSprite,true);
//		}
//
//		//SpriteFrameCache
//	    //TextureCache ������
//		/*
//
//		void addSpriteFramesWithFile(const char *pszPlist)
//		����һ��.plist�ļ���Ӷ������֡�� һ���������Զ����ء��������ƽ���.plist��׺���滻Ϊ.png����ɡ�
//		void addSpriteFramesWithFile(const char* plist, const char* textureFileName)
//		��ͨ��һ��.plist�ļ���Ӷ������֡�������뱻�����ľ���֡��ϡ�
//		void addSpriteFramesWithFile(const char *pszPlist, Texture2D *pobTexture)
//		��ͨ��һ��.plist�ļ���Ӷ������֡�������뱻�����ľ���֡��ϡ�
//		void addSpriteFrame(SpriteFrame *pobFrame, const char *pszFrameName)
//		��ͨ���������������һ������֡�� ��������Ѿ����ڣ���ôԭ�����Ƶ����ݽ����µ������
//		*/
//
//		/*if (pWW)
//		{
//		   free (pWW);
//		}*/
//
//		//delete pW;pW=NULL;
//		//delete data;data=NULL;
//		Sprite *lpSprite=Sprite::create("jin.png");
//		lpSprite->setPosition(ax::Vec2(visibleSize.width/2 +origin.x, visibleSize.height/2 + origin.y));
//		m_sprCount++;
//		this->addChild(lpSprite,1,m_sprCount);
//		//
//		//ax::FileUtils::getInstance()->CloseFilePtr("laohuji.spr",FALSE);
//
//		//fclose(fp);
//		/*
//		float winw = visibleSize.width;   //��ȡ��Ļ���
//		float winh = visibleSize.height;  //��ȡ��Ļ�߶�
//		float spx = testSprite->getTextureRect().getMaxX();
//		float spy = testSprite->getTextureRect().getMaxY();
//		testSprite->setScaleX(winw/spx);  //���þ��������ű���
//		testSprite->setScaleY(winh/spy);
//
//		*/
//	}
//	else
//	{
//
//	}
//
//	//this->setRotation(CC_RADIANS_TO_DEGREES(angle));
//	// add "HelloWorld" splash screen"
//	//Sprite* pSprite = Sprite::create("HelloWorld.png");
//	// position the sprite on the center of the screen
//	//pSprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//	// add the sprite as a child to this layer
//	//this->addChild(pSprite, 0);
//	//���Զ����ǵ�ǰһ������
//	//Sprite* sprite = Sprite::create("bg.jpg");
//	//ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y)
//	//sprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));//(ax::Vec2(visibleSize.width/2,visibleSize.height/2));
//	//sprite->setScaleX(1.5f);
//	//sprite->setScaleY(2.0f);
//	//Size mywinsize=Director::getInstance()->getWinSize();
//	/*float winw = visibleSize.width; //��ȡ��Ļ���
//	float winh = visibleSize.height;//��ȡ��Ļ�߶�
//	float spx = sprite->getTextureRect().getMaxX();
//	float spy = sprite->getTextureRect().getMaxY();
//	sprite->setScaleX(winw/spx); //���þ��������ű���
//	sprite->setScaleY(winh/spy);*/
//	//this->addChild(sprite,0);
//	//ҡ��
//	Sprite *controlSprite=Sprite::create("yaoganx.png");
//	//ҡ�˱���
//	Sprite *ygSprite=Sprite::create("yaoganz.png");
//	joystick=HRocker::HRockerWithCenter(ax::Vec2(100.0f,100.0f),60.0f ,controlSprite ,ygSprite,false);
//	m_sprCount++;
//	this->addChild(joystick,2,m_sprCount);
//	nTestCount = 0;
//	//----------������Դ�ļ����ɶ�д�ļ���
//	/*if (!isFileExist("laohuji.spr"))
//	{//�����ھͿ�ʼ����
//		copyData("laohuji.spr");
//	}
//	//��ʼ���� �Ƿ��Ƴɹ�....
//	if (isFileExist("laohuji.spr"))
//	{//�����ھͿ�ʼ����
//		messageBox("suee","isFileExist");
//	}*/
//	//this->draw();
//
//
//	this->scheduleUpdate();  //ң�˸��µ��ú���
//
//	return true;
//}
//
////�رհ�ť�ص�����
//void Klogin::mianSkillCallback(Ref* pSender)
//{//�ر�
///*#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//	messageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//#else
//	ax::Director::getInstance()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	exit(0);
//#endif
//#endif*/
//}
//
////ѭ�������麯�� Ĭ���Զ�����
//void Klogin::update(float delta)
//{
//	if (nTimeCount*1000 <= start.GetElapse()*18) //
//	{//�ÿͻ��������� ÿ������һ��
//		if (g_pCoreShell)
//		    g_pCoreShell->Breathe();
//		nTimeCount++;
//	//g_SubWorldSet.m_nLoopRate=10000;
//	char nPiontInfo[64*3]={0}; //start.GetElapse()
//	sprintf(nPiontInfo,"%u,%u,%u",nTimeCount,g_SubWorldSet.m_nLoopRate,start.GetElapse()); //g_pCoreShell->GetSubGameTime()
//	pMainLabel->setString(nPiontInfo);
//	}
//	if (nTimeCount*1000 >= start.GetElapse()*18)
//	{
//		//UiPaint(nGameFps);    //�滭��Ϸ����
//        #ifdef WIN32
//                Sleep(10);
//        #else
//                sleep(1);
//        #endif               //̫�����ͣһ��
//	}
//	else if ((nTimeCount >> 3))   //ÿ��֡��ͣ
//	{//����ֹͣ�滭��Ϸ���磬�Ϳ�ס
//        #ifdef WIN32
//                Sleep(10);
//        #else
//                sleep(1);
//        #endif
//    }
//
//	unsigned long nFielpahtdwid = g_FileName2Id("\\spr\\skill\\1502\\kl_150_jiankun_fu.spr");
//	for (int i=1;i<21;i++)
//	{
//		char nSprFilePath[64]={0};
//		sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,i);
//		Texture2D* nCurTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);//������ڵ� ��
//		if (!nCurTexture)
//			continue;
//		Animation*animation = Animation::create();//����һ������
//		animation->addSpriteFrameWithFile(nSprFilePath);//�����������������֡
//		animation->setDelayPerUnit(0.02f);
//		//���ò���ѭ�� һֱ���� Ϊ-1
//		animation->setLoops(1);
//		//���ö���������ָ�����һ֡ setRestoreOriginalFrame
//		animation->setRestoreOriginalFrame(TRUE);
//		// ����һ����Զѭ���Ķ���
//		animate = Animate::create(animation);
//		Sprite * tSprite=Sprite::createWithTexture(nCurTexture);
//		tSprite->setPosition(ax::Vec2(m_nWidth + origin.x+300, visibleSize.height/2 + origin.y));
//		this->addChild(tSprite,2,100);
//        auto callFunc = ax::CallFunc::create([this]() {
//            this->endSprite();
//        });
//		tSprite->runAction(Sequence::create(animate, callFunc, NULL));
//
//	}
//
//	if  (m_pTexture && testSprite)
//	{//������������ڵĻ�,ˢ�������������� �ܴﵽ��������ʾ��
//			//pWW = SprDecode.RenderToSprDecode_New("kl_150_jiankun_fu.spr",18,&m_nWidth,&m_nHeight);//�ڴ�й¶
//				/*m_pTexture->DeleteTexture();
//
//				Size imageSize =ax::Size((float)(m_nWidth), (float)(m_nHeight));
//			    //�ڴ�й¶
//				m_pTexture->initWithData(pWW,(CCTexture2DPixelFormat)kTexture2DPixelFormat_RGBA8888,m_nWidth,m_nHeight,imageSize); //ax::Size((float)w, (float)h)
//				*/
//		/*if (animate)
//		{
//			if (testSprite->isActionDone(animate))
//			{
//				testSprite->stopAction(animate);
//				removeChild(testSprite,true);
//				testSprite->release();
//				testSprite=NULL;
//			}
//		}*/
//	}
//
//	int newX = 0;
//	int newY = 0;
//	//�ж��Ƿ���ҡ�˼�������
//    //Point nCurPoint= joystick->getDirection();
//	//float n = joystick->getAngleSigned();
//	/*char nPiontInfo[64]={0};
//	sprintf(nPiontInfo,"%f",n);
//	pMainLabel->setString(nPiontInfo);
//    */
//	switch(newY)
//	{
//	case 1:
//		break;
//	case  2:
//		break;
//	case 3:
//		break;
//	case 4:
//		break;
//	default:
//		break;
//	}
//}
//
////[TODO][zer0kull]
////void Klogin::draw()
////{
//	/*nTestCount ++;
//	char nPiontInfo[64]={0};
//	sprintf(nPiontInfo,"%d",nTestCount);
//	pMainLabel->setString(nPiontInfo);*/
//	//��Ļ�ߴ��С
//	//Size mysize      = ax::Director::getInstance()->getVisibleSize();
//	//Point origin     = ax::Director::getInstance()->getVisibleOrigin();
//	//��
//	//һ����
//	/*ccPointSize(1);
//	ccDrawColor4B(255, 25, 200, 250);
//	ccDrawSolidRect(ax::Vec2(origin.x, origin.y), ax::Vec2(origin.x+mysize.width, origin.y+mysize.height), ccc4f(0, 0, 0, 0.5f));
//	*/
//	/*for (int i=0;i<=mysize.width;i++)
//		for(int j=0;j<=mysize.height;j++)
//		{
//			Point nPos;
//			nPos.x = i;
//			nPos.y = j;
//            ccDrawPoint(nPos);
//		}*/
///*
//	//�����
//	ccPointSize(30);
//	ccDrawColor4B(255, 0, 0, 100);
//	Point Pointarray[] = {ax::Vec2(200, 150), ax::Vec2(200, 200), ax::Vec2(280, 150), ax::Vec2(280, 200)};
//	ccDrawPoints(Pointarray, 4);
//
//
//	//ֱ��
//	glLineWidth(3);
//	ccDrawColor4B(255, 255, 255, 130);
//	ccDrawLine(ax::Vec2(10, 300), ax::Vec2(200, 300) );
//
//
//	//Բ
//	glLineWidth(3);
//	ccDrawColor4B(255, 255, 100, 190);
//	ccDrawCircle(ax::Vec2(50, 250), 40, 3.14/2, 360, true, 1, 0.5);
//	*/
//
//	//����
//	//����
///*	glLineWidth(5);
//	ccDrawColor4B(24, 25, 200, 140);
//	//ccDrawRect(ax::Vec2(10, 150), ax::Vec2(110, 200));
//	//ʵ��
//	ccDrawSolidRect(ax::Vec2(origin.x, origin.y), ax::Vec2(origin.x+mysize.width, origin.y+mysize.height), ccc4f(0, 0, 0, 0.5f));
//*/
///*
//	//�����
//	//����
//	glLineWidth(10);
//	ccDrawColor4B(240, 225, 100, 130);
//	Point Polyarray[] = {ax::Vec2(20, 20), ax::Vec2(50, 0), ax::Vec2(250, 100), ax::Vec2(300, 100), ax::Vec2(250, 50)};
//	ccDrawPoly(Polyarray, 5, 1);
//
//	//ʵ��
//	Point Polyarray2[] = {ax::Vec2(250,30), ax::Vec2(280,300), ax::Vec2(450,0), ax::Vec2(410,130), ax::Vec2(420,50)};
//	ccDrawSolidPoly(Polyarray2, 5, ccc4f(142, 245, 70, 0.3f));
//
//
//	//����������
//	//ƽ��
//	glLineWidth(5);
//	ccDrawColor4B(100, 100, 100, 255);
//	ccDrawQuadBezier(ax::Vec2(0,320), ax::Vec2(160,100), ax::Vec2(480,320), 100);
//
//	//����
//	glLineWidth(5);
//	ccDrawColor4B(200, 200, 200, 255);
//	ccDrawCubicBezier(ax::Vec2(0,0), ax::Vec2(160,300), ax::Vec2(320,20), ax::Vec2(480,320), 100);*/
////}
////
//bool Klogin::isFileExist(const char* pFileName)
//{
//	if( !pFileName ) return false;
//	//strFilePathName is :/data/data/ + package name
//	std::string filePath = ax::FileUtils::getInstance()->getWritablePath();//getWriteablePath();
//
//	filePath += pFileName;
//
//	FILE *fp = fopen(filePath.c_str(),"r");
//	if(fp)
//	{
//		fclose(fp);
//		return true;
//	}
//	return false;
//}
//
//void Klogin::copyData(const char* pFileName)
//{
//	std::string strPath = ax::FileUtils::getInstance()->fullPathForFilename(pFileName);
//	unsigned long len = 0;
//	unsigned char * data = NULL;
//
//    std::ifstream file(strPath, std::ios::binary | std::ios::ate);
//    if (file.is_open()) {
//        // Lấy kích thước của tệp
//        std::streampos size = file.tellg();
//        data = new unsigned char[size];
//        file.seekg(0, std::ios::beg);
//        file.read(reinterpret_cast<char*>(data), size);
//        file.close();
//    }
//
//    if (data) {
//        len = ax::FileUtils::getInstance()->getFileSize(strPath);
//        std::string destPath = ax::FileUtils::getInstance()->getWritablePath();
//        destPath += pFileName;
//        FILE *fp = fopen(destPath.c_str(),"w+");
//
//        if (fp) {
//            fwrite(data,sizeof(unsigned char),len,fp);
//            fclose(fp);
//        }
//
//        delete [] data;
//        data = NULL;
//    }
//}
//
//
//void Klogin::endSprite()
//{
//	//removeChild(tSprite,true);
//	Node* s = getChildByTag(100);
//	if (s)
//		removeChild(s,true);
//}
//
//void Klogin::finish()
//{
//		if (testSprite)
//		{
//		   testSprite->stopAction(animate);
//		   removeChild(testSprite,true);
//		   //testSprite->removeFromParent();
//		   testSprite->release();
//		   //testSprite=NULL;
//		}
//
//		/*if (testSprite)
//		{
//		 addChild(testSprite,2,1);
//		}*/
//}
///*
//void Klogin::onEnter()
//{
//
//}
//
//void Klogin::onExit()
//{
//
//}
////����� �� �������
//void Klogin::onEnterTransitionDidFinish()
//{
//
//}*/
//
//bool Klogin::ccTouchBegan(Touch *pTouch, Event *pEvent)
//{
//	return true;
//}
//void Klogin::ccTouchMoved(Touch *pTouch, Event *pEvent)
//{
//
//}
//
//void Klogin::ccTouchCancelled(Touch *pTouch, Event *pEvent)
//{
//
//}
//// default implements are used to call script callback if exist
//void Klogin::ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
//{
//
//}
//void Klogin::ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
//{
//
//}
//void Klogin::ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
//{
//	auto it = pTouches.begin();
//	Touch* touch = (Touch*)(*it);
//
//	Point location = touch->getLocation();//Ŀ�ĵ�����?��Ļ�����
//
//
//	/*public void GetMpsByLocalPosition(Vector3 v3LocalPosition, out int nMpsX, out int nMpsY)
//	{
//		nMpsX = ((int) (location.x * 100f)) + ((this.m_nRegionBeginX * this.m_nRegionWidth) * this.m_nCellWidth);
//		nMpsY = ((this.m_nRegionBeginY * this.m_nRegionHeight) * this.m_nCellHeight) - ((int) (location.y * 200f));
//	}*/
//
//	/*Node* s = getChildByTag(1);           //ͨ����ǺŻ�ȡ����Ľڵ�
//	//s->stopAllActions();                    //ֹͣ���ж���
//	s->runAction(CCMoveTo::create(10, ax::Vec2(location.x, location.y)));//�ƶ���
//
//	float o = location.x - s->getPosition().x;
//	float a = location.y - s->getPosition().y;
//
//	float at = (float) CC_RADIANS_TO_DEGREES( atanf( o/a) );
//
//	if( a < 0 )
//	{
//		if(  o < 0 )
//			at = 180 + fabs(at);
//		else
//			at = 180 - fabs(at);
//	}
//
//	s->runAction( CCRotateTo::create(10, at) ); //��ת��......
//	*/
//}
//void Klogin::ccTouchesCancelled(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
//{
//
//}
//void Klogin::didAccelerate(Acceleration* pAccelerationValue)
//{
//
//}
//void Klogin::ccTouchEnded(Touch *pTouch, Event *pEvent)
//{//convertTouchToNodeSpace
//   /*Point touchLocation = convertTouchToNodeSpace(pTouch);
//   if(Rect::CCRectContainsPoint(getRect(pSprite), touchLocation))
//	{
//	   printf("�ұ�������!\n");
//	}*/
//}
//
//void Klogin::beiyongb(Ref* pSender){
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//	messageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//#else
//	ax::Director::getInstance()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	exit(0);
//#endif
//#endif
//
//}
////�����Ի����
//void Klogin::beiyonga(Ref* pSender)
//{//ģ̬�Ի���ֻ�ܶԱ������в���
//	// ����һ�������㣬����һ�ű���ͼ
//	PopupLayer* pl = PopupLayer::create("laohuji.png"); //����һ���� ���ڵ�
//	// ContentSize �ǿ�ѡ�����ã����Բ����ã�������ð������� 9 ͼ����
//	pl->setContentSize(ax::Size(400, 360));
//	pl->setTitle(UTEXT("����һҶ", 1).c_str());
//	pl->setContentText(UTEXT("������÷�����ͣ�ȴ���ķҰ���ء������ٻ������ޣ�������ӣһ֦�㡣", 1).c_str(), 20, 50, 150);
//	// ���ûص��������ص�����һ�� Node �Ի�ȡ tag �жϵ���İ�ť
//	// ��ֻ����Ϊһ�ַ�װʵ�֣����ʹ�� delegate �Ǿ��ܹ������Ŀ��Ʋ�����
//    //[TODO][zer0kull]
////	pl->setCallbackFunc(CC_CALLBACK_1(Klogin::buttonCallback, this));
//	// ��Ӱ�ť������ͼƬ�����֣�tag ��Ϣ
//	pl->addButton("ok.png","ok.png","", 0);      //�ڸ��ڵ��´���һ���ӽڵ㰴ť 0
//	pl->addButton("close.png","close.png","", 1);//�ڸ��ڵ��´���һ���ӽڵ㰴ť 1
//	// ��ӵ���ǰ��
//	pl->setPosition(ax::Vec2(0,0));
//	this->addChild(pl,2,1000); //�����������Ӹ��ڵ�
//}
////�����Ի���Ļص�����
//void Klogin::buttonCallback(Node *pNode)
//{//��������б�Ǻţ���ȷ�����ĸ���ť
//	CCLOG("button call back. tag: %d", pNode->getTag());
//}
//
//
//void  Klogin::nextsenc(Ref* pSender)
//{//����һ��NPC
//
//}
//
////���ص�ͼ
//void Klogin::addmap(Ref* pSender)
//{
//	//MTNotificationQueue::sharedNotificationQueue()->postNotification("connectok", NULL
//}
//
//void Klogin::addspr(Ref* pSender){
//	if (m_sprCount>0)
//	{
//		for (int i=1;i<=m_sprCount;i++)
//		{
//			Node* s = getChildByTag(i);
//			if (s)
//			   addChild(s,2,i);
//		}
//	}
//
//}
//
//void Klogin::delspr(Ref* pSender){
//
//	if (m_sprCount>0)
//	{
//		for (int i=1;i<=m_sprCount;i++)
//		{
//			Node* s = getChildByTag(i);
//			if (s)
//				removeChild(s,true);
//		}
//
//	}
//}
