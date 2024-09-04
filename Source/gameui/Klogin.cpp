#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "gameui/Uiglobaldata.h"
#include "Klogin.h"
#include "Klogin_f.h"
#include "KSelPlayer.h"
#include "engine/CXmlStream.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gameui/PopupLayer.h"
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
#include "gamecore/KSubWorldSet.h"
#include "gamescene/KUpdateLayer.h"
#include <iostream>
#include <fstream>
#ifndef WIN32
#include <ctype.h>
#endif
extern iCoreShell*	g_pCoreShell; //ȫ�ֵ���
extern KImageStore2 m_ImageStore; //ȫ�ֵ���
//extern KPakList     g_pPakList;
//extern KPakList * g_pPakList;
#include <list>
USING_NS_AX;

void CcharToDouble(const char* ch, double& res)
{
	sscanf(ch,"%lf",&res);
}

Scene* Klogin::scene()
{
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// 'layer' is an autorelease object
	Klogin *layer = Klogin::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Klogin::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	m_sprCount         =0;
	nTimeCount = 0;
	m_nWidth   = 0;
	m_nHeight  = 0;
	//pWW        =NULL;
	animate    =NULL;
	m_bIsPlayerInGame =false;
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Klogin::ccTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(Klogin::ccTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Klogin::ccTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(Klogin::ccTouchCancelled, this);

    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	//MenuItemImage *pCloseItem = MenuItemImage::create();
	m_Status = LL_S_WAIT_INPUT_ACCOUNT;
	char nSprName[128]={0};
	int nFrams=0;
	t_sprintf(nSprName,"\\spr\\Ui3\\loginui\\okbtn\\loginui_6_ok.spr");
	g_StrLower(nSprName);
	MenuItemImage *pCancelItem=NULL;
	MenuItemImage *pLoginItem=NULL;
	Texture2D *Login = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if  (Login)
	{
		char nSprFilePath[64]={0};
		DWORD nFielpahtdwid = g_FileName2Id(nSprName);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
		Texture2D *pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		Texture2D *pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		Texture2D *pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pLoginItem = MenuItemImage::create();
        pLoginItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pLoginItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pLoginItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pLoginItem->setCallback(CC_CALLBACK_1(Klogin::mianLoginCallback, this));

		pLoginItem->setPosition(ax::Vec2(origin.x + visibleSize.width - pLoginItem->getContentSize().width/2-200 ,
			origin.y + pLoginItem->getContentSize().height/2+20));//һ����ť
	}

	nFrams=0;
	t_sprintf(nSprName,"\\spr\\Ui3\\loginui\\okbtn\\loginui_6_cancel.spr");
	g_StrLower(nSprName);
	Texture2D *Cancel = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
	if (Cancel)
	{
		char nSprFilePath[64]={0};
		DWORD nFielpahtdwid = g_FileName2Id(nSprName);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
		Texture2D *pNormalTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,1);
		Texture2D *pSelectedTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
		t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,2);
		Texture2D *pDisabledTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);

        pCancelItem = MenuItemImage::create();
        pCancelItem->setNormalImage(Sprite::createWithTexture(pNormalTexture));
        pCancelItem->setSelectedImage(Sprite::createWithTexture(pSelectedTexture));
        pCancelItem->setDisabledImage(Sprite::createWithTexture(pDisabledTexture));
        pCancelItem->setCallback(CC_CALLBACK_1(Klogin::mianCancelCallback, this));

		pCancelItem->setPosition(ax::Vec2(origin.x + visibleSize.width - pCancelItem->getContentSize().width/2 ,
			origin.y + pCancelItem->getContentSize().height/2+20));//һ����ť
	}
	/*MenuItemImage *pCloseItem = MenuItemImage::create(
		"mainskillmix.png",
		"mianskillbig.png",
		this,
		menu_selector(Klogin::mianSkillCallback)); //�ص�����
	pCloseItem->setPosition(ax::Vec2(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2-20 ,
		origin.y + pCloseItem->getContentSize().height/2+20));//һ����ť
	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create(pCloseItem, NULL);
	pMenu->setPosition(ax::Vec2::ZERO);
	m_sprCount ++;
	this->addChild(pMenu,1,m_sprCount);*/
	/////////////////////////////
	// 3. add your codes below...
	// add a label shows "Hello World"
	// create and initialize a label
	const char * strTiele = CXmlStream::GetStringByKeyFromFile("string.xml","title"); //Resources ��Դ����Ŀ¼��,���û������ļ���������
	//KPakList g_pPakList;
	//FileUtils::getPathForFilename()
	//std::string nPath = ax::FileUtils::getInstance()->getWritablePath();

	//ccMessageBox(nPath.c_str(),"getWritablePath");//�ɶ�д·�� /data/data/����/files/
	//nPath = ax::FileUtils::getInstance()->fullPathForFilename("package.ini");
	//nPath = ax::FileUtils::getInstance()->getWritablePath();//getWriteablePath();
	//nPath += "download";
	int nCount = 0;
	//tolower() //��дתСд
	//toupper() //Сдת��д
	//getExtSDCardPath();
	//Environment.getExternalStorageDirectory();
	//Context::getCacheDir();
	//Environment.getExternalStorageDirectory().toString() + "/" + "Tianchaoxiong/useso";
	//AssetManager::InputStream.open();
	//pContext.getAssets();
	//getAssetManager();
	//nCount = g_pPakList.Open(fielDir.c_str());  //����PAK
	//FileUtils::getFullPathForDirectoryAndFilename("Resources","test.ini");
	//fullPathForFilename(filename.c_str());
	pMainLabel = Label::createWithTTF(strTiele,UI_GAME_FONT_DEFAULT, 20);
	pMainLabel->setString(UTEXT("",1).c_str());//������ʾ������
	//ccMessageBox(nPath.c_str(),"title");
	// position the label on the center of the screen
	pMainLabel->setPosition(ax::Vec2(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - pMainLabel->getContentSize().height-10));
	// add the label as a child to this layer
	//m_sprCount++;
	this->addChild(pMainLabel, 1);

	Sprite *bgSprite=Sprite::create("loginbg.jpg");//��½����
	bgSprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	float winw = visibleSize.width;   //��ȡ��Ļ���
	float winh = visibleSize.height;  //��ȡ��Ļ�߶�
	float spx  = bgSprite->getTextureRect().getMaxX();
	float spy  = bgSprite->getTextureRect().getMaxY();
	bgSprite->setScaleX(winw/spx); //���þ��������ű���
	bgSprite->setScaleY(winh/spy);
	m_sprCount++;
	this->addChild(bgSprite,0,m_sprCount);
	//Node * d = newmenu->getChildByTag(1);//��ȡ���ڵ��µ��ӽڵ�
	//newmenu->removeChild(d,true);          //ɾ�����ڵ��µ��ӽڵ�
	//newmenu->removeChildByTag(1);
	//itemaddmaop->removeFromParentAndCleanup(true);  //
	//d->removeFromParentAndCleanup(true);
	//d->release();//�ͷ����ڴ档������
	//if (itemaddmaop)
	//   newmenu->addChild(itemaddmaop,true);  //���ڵ�������һ���ӽڵ�
	//����һ�������
	//CCComponent::create();
	//newmenu->setUserData();//Ϊĳ���ڵ���Զ�������
	//�����￪ʼ��ȡͼ�����ݵ�ָ��data
		//memset()
		//\\Spr\\Ui3\\�������\\��½����_6_�߿�.spr  \Spr\Ui3\�������\����С����\Ҷ��Ʈ��.spr  \\spr\\Ui3\\loginui\\uidonghua\\yezi.spr

		 t_sprintf(nSprName,"\\spr\\Ui3\\loginui\\uidonghua\\yezi.spr");
		 g_StrLower(nSprName);
		 Texture2D *bgCur = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
		 if  (bgCur)
		 {
			Sprite *ySprite=Sprite::createWithTexture(bgCur);
			ySprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x-100, visibleSize.height/2 + origin.y));
			/*float winw = visibleSize.width;   //��ȡ��Ļ���
			float winh = visibleSize.height;  //��ȡ��Ļ�߶�
			float spx  = bgSprite->getTextureRect().getMaxX();
			float spy  = bgSprite->getTextureRect().getMaxY();
			ySprite->setScaleX(winw/spx); //���þ��������ű���
			ySprite->setScaleY(winh/spy);*/
			m_sprCount++;
			this->addChild(ySprite,2,m_sprCount);
			char nySprFilePath[64]={0};
			Animation*animation = Animation::create();//����һ������
			DWORD nFielpahtdwid = g_FileName2Id(nSprName);
			for (int i=0;i<nFrams;i++)
			{
				t_sprintf(nySprFilePath,"%u-%d",nFielpahtdwid,i);
				//ccMessageBox(nSprFilePath,"animation");
                auto texture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nySprFilePath);
                Rect rect = Rect::ZERO;
                rect.size = texture->getContentSize();
                animation->addSpriteFrameWithTexture(texture, rect);
//                texture->release();
//				animation->addSpriteFrameWithFile(nySprFilePath);//�����������������֡
			}
			//���ö���֡��ʱ����
			animation->setDelayPerUnit(0.10f);
			//���ò���ѭ�� һֱ���� Ϊ-1
		    animation->setLoops(-1);
			//���ö���������ָ�����һ֡ setRestoreOriginalFrame
			animation->setRestoreOriginalFrame(TRUE);
			// ����һ����Զѭ���Ķ���
			animate = Animate::create(animation);
			//RepeatForever* forever = RepeatForever::create(animate);
			if (ySprite) {
                auto callFunc = ax::CallFunc::create([this]() {
                    this->finish();
                });

                ySprite->runAction(Sequence::create(animate, callFunc, NULL));
            }
		 }
		 //�����ʺš����� "\\Spr\\Ui3\\�������\\��½����_6_�߿�.spr"
		 t_sprintf(nSprName,"\\spr\\Ui3\\loginui\\longi_6_b.spr");//"\\spr\\Ui3\\loginui\\longi_6_b.spr"
		 g_StrLower(nSprName);
		 nFrams=0;
		 Texture2D *Cur = _getinidata.getinidata_new(nSprName,0,&m_nWidth,&m_nHeight,&nFrams);
		testSprite = NULL;
		if (Cur)
		{
			//��½����ͼ
			testSprite=Sprite::createWithTexture(Cur);

		  if (testSprite)
		  {
			char nTempStr[64];
			ZeroMemory(nTempStr,sizeof(nTempStr));
			KIniFile accinfo;
			accinfo.Load("accinfo.ini",true);

			testSprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
			m_sprCount++;
			this->addChild(testSprite,2,m_sprCount);
			//�ʺ������
			ax::ui::Scale9Sprite *pAccBg = ax::ui::Scale9Sprite::create();  //"boxgb.png"
			EditBox *pAccEditBox = EditBox::create(Size(200,24),pAccBg);
			const ax::Color3B nfcolor ={255,0,0};
			if (pAccEditBox)
			{
				//pEditBox->setContentSize()
				//pAccEditBox->setText("input your account");
				pAccEditBox->setFontColor(nfcolor);
				pAccEditBox->setFontSize(24);
				pAccEditBox->setMaxLength(8);
				//pEditBox->setPosition(ax::Vec2(testSprite->getPositionX(), testSprite->getPositionX()));
				pAccEditBox->setPositionX(testSprite->getPositionX());
				pAccEditBox->setPositionY(testSprite->getPositionY());
				///m_sprCount++;
				accinfo.GetString("info","acc","",nTempStr,sizeof(nTempStr));
				pAccEditBox->setText("xtnet001");
				this->addChild(pAccEditBox,4,1000);
			}

			//���������
			ax::ui::Scale9Sprite *pPassBg = ax::ui::Scale9Sprite::create(); //"boxgb.png"
			EditBox *pPassEditBox = EditBox::create(Size(200,24),pPassBg);
			if (pPassEditBox)
			{
				//pEditBox->setContentSize()
				//pPassEditBox->setText("input your password");
				pPassEditBox->setFontColor(nfcolor);
				pPassEditBox->setFontSize(24);
				pPassEditBox->setInputFlag(EditBox::InputFlag::PASSWORD);
				pPassEditBox->setMaxLength(16);
				//pEditBox->setPosition(ax::Vec2(testSprite->getPositionX(), testSprite->getPositionX()));
				pPassEditBox->setPositionX(testSprite->getPositionX()+20);
				pPassEditBox->setPositionY(testSprite->getPositionY());
				//m_sprCount++;
				accinfo.GetString("info","pas","",nTempStr,sizeof(nTempStr));
				pPassEditBox->setText("1");
				this->addChild(pPassEditBox,4,999);
			}
			accinfo.Clear();

			/*char nSprFilePath[64]={0};
			Animation*animation = Animation::create();//����һ������
            //std:string nFileName ="\\spr\\skill\\1502\\kl\\kl_150_jiankun_fu.spr";
			DWORD nFielpahtdwid = g_FileName2Id(nSprName);
			for (int i=0;i<nFrams;i++)
			{
				t_sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,i);
				//ccMessageBox(nSprFilePath,"animation");
				animation->addSpriteFrameWithFile(nSprFilePath);//�����������������֡
			}
			//���ö���֡��ʱ����
			animation->setDelayPerUnit(0.02f);
			//���ò���ѭ�� һֱ���� Ϊ-1
		    animation->setLoops(-1);
			//���ö���������ָ�����һ֡ setRestoreOriginalFrame
			animation->setRestoreOriginalFrame(TRUE);
			// ����һ����Զѭ���Ķ���
			animate = Animate::create(animation);
			//RepeatForever* forever = RepeatForever::create(animate);
			if (testSprite)
			   //testSprite->runAction(animate);
			   testSprite->runAction(Sequence::create(animate,CallFunc::create(this, callfunc_selector(Klogin::finish)),NULL));
			*/
			//if(testSprite->isActionDone(animate))
			 // removeChild(testSprite,true);
			//CCMoveTo* move = CCMoveTo::create(30, ax::Vec2(-20, 200));
			//testSprite->runAction(move);
			//removeChild(testSprite,true);
			// create menu, it's an autorelease object
			Menu* pMenu=NULL;
			if (Login && Cancel)
			{
				pLoginItem->setPositionX(testSprite->getPositionX()-75);
				pLoginItem->setPositionY(testSprite->getPositionY()-80); //ȷ�ϰ�ť
				pCancelItem->setPositionX(testSprite->getPositionX()+75);//�ı䰴ť
				pCancelItem->setPositionY(testSprite->getPositionY()-80);

				pMenu = Menu::create(pLoginItem,pCancelItem,NULL);
				pMenu->setPosition(ax::Vec2::ZERO);//
				m_sprCount ++;
				this->addChild(pMenu,2,m_sprCount);
			}
		  }
		}

		//SpriteFrameCache
	    //TextureCache ������
		/*

		void addSpriteFramesWithFile(const char *pszPlist)
		����һ��.plist�ļ���Ӷ������֡�� һ���������Զ����ء��������ƽ���.plist��׺���滻Ϊ.png����ɡ�
		void addSpriteFramesWithFile(const char* plist, const char* textureFileName)
		��ͨ��һ��.plist�ļ���Ӷ������֡�������뱻�����ľ���֡��ϡ�
		void addSpriteFramesWithFile(const char *pszPlist, Texture2D *pobTexture)
		��ͨ��һ��.plist�ļ���Ӷ������֡�������뱻�����ľ���֡��ϡ�
		void addSpriteFrame(SpriteFrame *pobFrame, const char *pszFrameName)
		��ͨ���������������һ������֡�� ��������Ѿ����ڣ���ôԭ�����Ƶ����ݽ����µ������
		*/
		/*Sprite *lpSprite=Sprite::create("jin.png");
		lpSprite->setPosition(ax::Vec2(visibleSize.width/2 +origin.x, visibleSize.height/2 + origin.y));
		m_sprCount++;
		this->addChild(lpSprite,1,m_sprCount);*/
		//
		//ax::FileUtils::getInstance()->CloseFilePtr("laohuji.spr",FALSE);

		//fclose(fp);
		/*
		float winw = visibleSize.width;   //��ȡ��Ļ���
		float winh = visibleSize.height;  //��ȡ��Ļ�߶�
		float spx = testSprite->getTextureRect().getMaxX();
		float spy = testSprite->getTextureRect().getMaxY();
		testSprite->setScaleX(winw/spx);  //���þ��������ű���
		testSprite->setScaleY(winh/spy);

		*/
	//this->setRotation(CC_RADIANS_TO_DEGREES(angle));
	// add "HelloWorld" splash screen"
	//Sprite* pSprite = Sprite::create("HelloWorld.png");
	// position the sprite on the center of the screen
	//pSprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	// add the sprite as a child to this layer
	//this->addChild(pSprite, 0);
	//���Զ����ǵ�ǰһ������
	//Sprite* sprite = Sprite::create("bg.jpg");
	//ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y)
	//sprite->setPosition(ax::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));//(ax::Vec2(visibleSize.width/2,visibleSize.height/2));
	//sprite->setScaleX(1.5f);
	//sprite->setScaleY(2.0f);
	//Size mywinsize=Director::getInstance()->getWinSize();
	/*float winw = visibleSize.width; //��ȡ��Ļ���
	float winh = visibleSize.height;//��ȡ��Ļ�߶�
	float spx = sprite->getTextureRect().getMaxX();
	float spy = sprite->getTextureRect().getMaxY();
	sprite->setScaleX(winw/spx); //���þ��������ű���
	sprite->setScaleY(winh/spy);*/
	//this->addChild(sprite,0);
	nTestCount = 0;
	//----------������Դ�ļ����ɶ�д�ļ���
	/*if (!isFileExist("laohuji.spr"))
	{//�����ھͿ�ʼ����
		copyData("laohuji.spr");
	}
	//��ʼ���� �Ƿ��Ƴɹ�....
	if (isFileExist("laohuji.spr"))
	{//�����ھͿ�ʼ����
		ccMessageBox("suee","isFileExist");
	}*/
	//this->draw();
	this->scheduleUpdate();  //ң�˸��µ��ú���
	 //��ֹ��㴥��
	return true;
}

void Klogin::mianCancelCallback(Ref* pSender)
{
	if (nAcclient)
	{
		m_bIsClientConnecting=false;
		m_bIsPlayerInGame =false;
		nAcclient->Cleanup();

	}
	ax::Director::getInstance()->replaceScene(Klogin_f::scene());
}

void Klogin::mianLoginCallback(Ref* pSender)
{
	EditBox * AccNode =  (EditBox *)this->getChildByTag(1000);
	EditBox * PasNode =  (EditBox *)this->getChildByTag(999);
	std::string strAcc;
	std::string strPas;
	//EditBox *pPassEditBox
	if (AccNode && PasNode)
	{
		strAcc = AccNode->getText();
		strPas = PasNode->getText();

		if (strAcc.length()>16 || strAcc.length()<=0 || strPas.length()>16 || strPas.length()<=0)
		{
#ifdef WIN32
			ccMessageBox("Sai t��i kho?n ho?c m?t kh?u","Ch�� y");
#else
			ccMessageBox(G2U("Sai t��i kho?n ho?c m?t kh?u��").c_str(),G2U("Ch�� y").c_str());
#endif
			return;
		}
		//��ʼ����ַ��ĺϷ���
		std::string strcheck="check";
		strcheck+=strAcc;
		strcheck+=strPas;
		int count = 0,spaceNum=0,chineseNum=0,other=0,letter=0,xiaoxie=0;
		for(int i = 0; i <strcheck.length();i++)
		{
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
{//���İ汾
		if (spaceNum>0 || chineseNum >0 || other>0 || letter>0)
		{ 	//system("pause");
#ifdef WIN32
			ccMessageBox("���зǷ��ַ�,�����ʺ�,����\n���ܺ��пո�,��д��ĸ���������!","����");
#else
			ccMessageBox(G2U("���зǷ��ַ�,�����ʺ�,����\n���ܺ��пո�,��д��ĸ���������!").c_str(),G2U("����").c_str());
#endif
			return;
		}
}
else
{//�������İ汾
	if (spaceNum>0 || chineseNum >0 || other>0)
	{ 	//system("pause");
#ifdef WIN32
		ccMessageBox("���зǷ��ַ�,�����ʺ�,����\n���ܺ��пո�,��д��ĸ���������!","����");
#else
		ccMessageBox("Invalid character,please check your name!\n Cannot contain special symbols such as chinese, spaces, etc.!","Warning");
#endif
		return;
	}
}

	}
	else
	{
#ifdef WIN32
		ccMessageBox("�������󣮣�","����");
#else
		ccMessageBox(G2U("�������󣮣�").c_str(),G2U("����").c_str());
#endif
		return;
	}
	KIniFile accinfo;
	if (accinfo.Load("accinfo.ini",true))
	{
		char nTempStr[64];
		t_sprintf(nTempStr,strAcc.c_str());
		accinfo.WriteString("info","acc",nTempStr);

		t_sprintf(nTempStr,strPas.c_str());
		accinfo.WriteString("info","pas",nTempStr);
		accinfo.Save("accinfo.ini");
		accinfo.Clear();
	}

	if (m_bIsClientConnecting && nAcclient)
	{
		if  (m_ClientChoices.nServerRegionIndex<=0)
		{
#ifdef WIN32
			ccMessageBox("��ѡ���½��������","����");
#else
			ccMessageBox(G2U("��ѡ���½��������").c_str(),G2U("����").c_str());
#endif
			return;
		}
		//ģ���ʺŵ�½
		m_nNumRole = 0;                          //��ɫ��������Ϊ0
		memset(&m_RoleList,0,sizeof(m_RoleList));//��ս�ɫ�嵥����
		ClearAccountPassword(true, true);        //��ձ��ݵ��ʺ�����

		BYTE Buff[sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE];
		RandMemSet(sizeof(Buff),(BYTE*)Buff);		// random memory for make a cipher
		char pszAccount[32]={0};
		t_sprintf(pszAccount,strAcc.c_str());

		(*(PROTOCOL_MSG_TYPE*)Buff) = c2s_login_fhgyrinhkmvnsheypo;
		KLoginAccountInfo* pInfo = (KLoginAccountInfo*)&Buff[PROTOCOL_MSG_SIZE];

		pInfo->Size  = sizeof(KLoginAccountInfo);
		pInfo->Param = LOGIN_A_LOGIN | LOGIN_R_REQUEST;
		strncpy(pInfo->Account,pszAccount,sizeof(pInfo->Account));

		pInfo->Account[sizeof(pInfo->Account) - 1] = '\0';
		//KSG_PASSWORD pcPassword;
		KSG_StringToMD5String(pInfo->Password.szPassword,strPas.c_str());
		//pInfo->Password      = pcPassword;
		pInfo->nSelSererIdx    = m_ClientChoices.nServerRegionIndex;//KUiSelServer::GetCurServer();
		pInfo->ProtocolVersion = KPROTOCOL_VERSION;       //  ����Э��汾���Ա�У���Ƿ����
		m_Status               = LL_S_ACCOUNT_CONFIRMING; //  �ȴ���֤�ʺ�����
		SetAccountPassword(pszAccount, &pInfo->Password);
	    if (!nAcclient->SendMsg(Buff, sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE))
		//if (!nAcclient->SendPackToServer(Buff,sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE))  //sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE
		{
			if (nAcclient)
			{
				nAcclient->Cleanup();
				m_bIsClientConnecting=false;
			}
			ax::Director::getInstance()->replaceScene(Klogin_f::scene());
		}
		//ax::Director::getInstance()->replaceScene(KSelPlayer::scene());
		//ccMessageBox("send suss","send suss");
	}
}
//�رհ�ť�ص�����
void Klogin::mianSkillCallback(Ref* pSender)
{//�ر�
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	ccMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	ax::Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif*/
}

//ѭ�������麯�� Ĭ���Զ�����
void Klogin::update(float delta)
{
	if  (m_Status == LL_S_ROLE_LIST_READY)
	{//����Ѿ���ȡ����ɫ������ �ͽ�����һ������
		m_Status = LL_S_IDLE;
		ax::Director::getInstance()->replaceScene(KSelPlayer::scene());
		return;
	}
	uint32_t nSize;
	const char* pBuffer = NULL;
	if (m_bIsClientConnecting)
	{ //�˺ŷ�����
		while (true)
		{
			if (!nAcclient)
				break;
			//����ѭ���ӷ�������ȡ����С��
			pBuffer = (const char*)nAcclient->GetPackFromServer(nSize);

			if (!(pBuffer && nSize))
				break;

			PROTOCOL_MSG_TYPE*	pMsg = (PROTOCOL_MSG_TYPE*)pBuffer;
			PROTOCOL_MSG_TYPE	Msg  = *pMsg;  //Э��ͷ

			//CCAssert(Msg > s2c_multiserverbegin || Msg < s2c_end,"");
			if  (Msg == s2c_login || Msg == s2c_roleserver_getrolelist_result ||Msg == s2c_notifypl_ayerlogin_hfkqotivbhd || Msg == s2c_rolenewdelresponse)
			     AcceptNetMsg(pMsg);
//			if (m_MsgTargetObjs[Msg])
	//		   (m_MsgTargetObjs[Msg])->AcceptNetMsg(pMsg);  //�������緢��������Ϣ
		}
	}
	int newX = 0;
	int newY = 0;
	//�ж��Ƿ���ҡ�˼�������
    //Point nCurPoint= joystick->getDirection();
	//float n = joystick->getAngleSigned();
	/*char nPiontInfo[64]={0};
	t_sprintf(nPiontInfo,"%f",n);
	pMainLabel->setString(nPiontInfo);
    */
	switch(newY)
	{
	case 1:
		break;
	case  2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

//[TODO][zer0kull]
//void Klogin::draw()
//{
	/*nTestCount ++;
	char nPiontInfo[64]={0};
	t_sprintf(nPiontInfo,"%d",nTestCount);
	pMainLabel->setString(nPiontInfo);*/
	//��Ļ�ߴ��С
	//Size mysize      = ax::Director::getInstance()->getVisibleSize();
	//Point origin     = ax::Director::getInstance()->getVisibleOrigin();
	//��
	//һ����
	/*ccPointSize(1);
	ccDrawColor4B(255, 25, 200, 250);
	ccDrawSolidRect(ax::Vec2(origin.x, origin.y), ax::Vec2(origin.x+mysize.width, origin.y+mysize.height), ccc4f(0, 0, 0, 0.5f));
	*/
	/*for (int i=0;i<=mysize.width;i++)
		for(int j=0;j<=mysize.height;j++)
		{
			Point nPos;
			nPos.x = i;
			nPos.y = j;
            ccDrawPoint(nPos);
		}*/
/*
	//�����
	ccPointSize(30);
	ccDrawColor4B(255, 0, 0, 100);
	Point Pointarray[] = {ax::Vec2(200, 150), ax::Vec2(200, 200), ax::Vec2(280, 150), ax::Vec2(280, 200)};
	ccDrawPoints(Pointarray, 4);


	//ֱ��
	glLineWidth(3);
	ccDrawColor4B(255, 255, 255, 130);
	ccDrawLine(ax::Vec2(10, 300), ax::Vec2(200, 300) );


	//Բ
	glLineWidth(3);
	ccDrawColor4B(255, 255, 100, 190);
	ccDrawCircle(ax::Vec2(50, 250), 40, 3.14/2, 360, true, 1, 0.5);
	*/

	//����
	//����
/*	glLineWidth(5);
	ccDrawColor4B(24, 25, 200, 140);
	//ccDrawRect(ax::Vec2(10, 150), ax::Vec2(110, 200));
	//ʵ��
	ccDrawSolidRect(ax::Vec2(origin.x, origin.y), ax::Vec2(origin.x+mysize.width, origin.y+mysize.height), ccc4f(0, 0, 0, 0.5f));
*/
/*
	//�����
	//����
	glLineWidth(10);
	ccDrawColor4B(240, 225, 100, 130);
	Point Polyarray[] = {ax::Vec2(20, 20), ax::Vec2(50, 0), ax::Vec2(250, 100), ax::Vec2(300, 100), ax::Vec2(250, 50)};
	ccDrawPoly(Polyarray, 5, 1);

	//ʵ��
	Point Polyarray2[] = {ax::Vec2(250,30), ax::Vec2(280,300), ax::Vec2(450,0), ax::Vec2(410,130), ax::Vec2(420,50)};
	ccDrawSolidPoly(Polyarray2, 5, ccc4f(142, 245, 70, 0.3f));


	//����������
	//ƽ��
	glLineWidth(5);
	ccDrawColor4B(100, 100, 100, 255);
	ccDrawQuadBezier(ax::Vec2(0,320), ax::Vec2(160,100), ax::Vec2(480,320), 100);

	//����
	glLineWidth(5);
	ccDrawColor4B(200, 200, 200, 255);
	ccDrawCubicBezier(ax::Vec2(0,0), ax::Vec2(160,300), ax::Vec2(320,20), ax::Vec2(480,320), 100);*/
//}
//
bool Klogin::isFileExist(const char* pFileName)
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

void Klogin::copyData(const char* pFileName)
{
    std::string strPath = ax::FileUtils::getInstance()->fullPathForFilename(pFileName);
    unsigned long len = 0;
    unsigned char * data = NULL;

    std::ifstream file(strPath, std::ios::binary | std::ios::ate);
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

void Klogin::finish()
{
   if (!nisgetinfo)
	  mExit(NULL);
}
/*
void Klogin::onEnter()
{

}

void Klogin::onExit()
{

}
//����� �� �������
void Klogin::onEnterTransitionDidFinish()
{

}*/

bool Klogin::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}
void Klogin::ccTouchMoved(Touch *pTouch, Event *pEvent)
{

}

void Klogin::ccTouchCancelled(Touch *pTouch, Event *pEvent)
{

}
// default implements are used to call script callback if exist
void Klogin::ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{

}
void Klogin::ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{

}

void Klogin::ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{
	auto it = pTouches.begin();
	Touch* touch = (Touch*)(*it);

	Point location = touch->getLocation();//Ŀ�ĵ�����?��Ļ�����


	/*public void GetMpsByLocalPosition(Vector3 v3LocalPosition, out int nMpsX, out int nMpsY)
	{
		nMpsX = ((int) (location.x * 100f)) + ((this.m_nRegionBeginX * this.m_nRegionWidth) * this.m_nCellWidth);
		nMpsY = ((this.m_nRegionBeginY * this.m_nRegionHeight) * this.m_nCellHeight) - ((int) (location.y * 200f));
	}*/

	/*Node* s = getChildByTag(1);           //ͨ����ǺŻ�ȡ����Ľڵ�
	//s->stopAllActions();                    //ֹͣ���ж���
	s->runAction(CCMoveTo::create(10, ax::Vec2(location.x, location.y)));//�ƶ���

	float o = location.x - s->getPosition().x;
	float a = location.y - s->getPosition().y;

	float at = (float) CC_RADIANS_TO_DEGREES( atanf( o/a) );

	if( a < 0 )
	{
		if(  o < 0 )
			at = 180 + fabs(at);
		else
			at = 180 - fabs(at);
	}

	s->runAction( CCRotateTo::create(10, at) ); //��ת��......
	*/
}
void Klogin::ccTouchesCancelled(const std::vector<ax::Touch*>&pTouches, Event *pEvent)
{

}
void Klogin::didAccelerate(Acceleration* pAccelerationValue)
{

}
void Klogin::ccTouchEnded(Touch *pTouch, Event *pEvent)
{//convertTouchToNodeSpace
   /*Point touchLocation = convertTouchToNodeSpace(pTouch);
   if(Rect::CCRectContainsPoint(getRect(pSprite), touchLocation))
	{
	   printf("�ұ�������!\n");
	}*/
}

void Klogin::AcceptNetMsg(void* pMsgData) //ѭ�����ܴ�����Ϣ
{
	if (pMsgData == NULL)  //���ݰ�Ϊ�� �򷵻�
		return;
	//ccMessageBox("�ص��ɹ�","GetPackFromServer");
	switch(m_Status)       //�������ݰ���ʶ����
	{
	case LL_S_ACCOUNT_CONFIRMING:  //�˺���֤
		ProcessAccountLoginResponse((KLoginStructHead*) (((char*)pMsgData) + PROTOCOL_MSG_SIZE));
		break;
	case LL_S_WAIT_ROLE_LIST:      //�ȴ���ɫ�嵥
		ProcessRoleListResponse((TProcessData*)pMsgData);   //����Э���ɫ��Ϣ
		break;
	case LL_S_CREATING_ROLE:       //������ɫ
		//ProcessCreateRoleResponse((tagNewDelRoleResponse*)pMsgData); //������ɫ
		break;
	case LL_S_DELETING_ROLE:       //ɾ����ɫ
		//ProcessDeleteRoleResponse((tagNewDelRoleResponse*)pMsgData);
		break;
	case LL_S_WAIT_TO_DEL:         //�Ѿ�ȡ��
		//ProcessGetRoleResponse((tagNotifyPlayerLogin*)pMsgData);
		break;
	case LL_S_WAIT_TO_LOGIN_GAMESERVER:  //�ȴ����ӵ�������
		//ProcessToLoginGameServResponse((tagNotifyPlayerLogin*)pMsgData); //�������ӵ��������ĺ���
		break;
	}
}

void Klogin::SetAccountPassword(const char* pszAccount, const KSG_PASSWORD* pcPassword)
{
	int i;
	if (pszAccount)
	{
		strncpy(m_ClientChoices.Account, pszAccount, sizeof(m_ClientChoices.Account));
		for (i = 0; i < 32; ++i)
			m_ClientChoices.Account[i] = ~m_ClientChoices.Account[i];
	}

	if (pcPassword)
	{
		m_ClientChoices.Password = *pcPassword;
		for (i = 0; i < KSG_PASSWORD_MAX_SIZE; ++i)
			m_ClientChoices.Password.szPassword[i] = ~m_ClientChoices.Password.szPassword[i];
	}
}

void Klogin::GetAccountPassword(char* pszAccount, KSG_PASSWORD* pPassword)
{
	int i;
	if (pszAccount)
	{
		memcpy(pszAccount, m_ClientChoices.Account, sizeof(m_ClientChoices.Account));
		for (i = 0; i < 32; ++i)
			pszAccount[i] = ~pszAccount[i];
	}
	if (pPassword)
	{
		*pPassword = m_ClientChoices.Password;
		for (i = 0; i < KSG_PASSWORD_MAX_SIZE; ++i)
			pPassword->szPassword[i] = ~pPassword->szPassword[i];
	}
}

void Klogin::ClearAccountPassword(bool bAccount, bool bPassword)
{
	if (bAccount)
		memset(m_ClientChoices.Account, 0xff, sizeof(m_ClientChoices.Account));
	if (bPassword)
		memset(&m_ClientChoices.Password, 0xff, sizeof(m_ClientChoices.Password));
}


void Klogin::ProcessAccountLoginResponse(KLoginStructHead* pResponse)
{
	//_ASSERT(m_Status == LL_S_ACCOUNT_CONFIRMING && pResponse != NULL);

	if (((pResponse->Param & LOGIN_ACTION_FILTER) == LOGIN_A_LOGIN) &&	//������ΪҪƥ��
		pResponse->Size >= sizeof(KLoginAccountInfo))				//�������ݵĴ�СҲҪƥ��
	{
		KLoginAccountInfo* pInfo = (KLoginAccountInfo*)pResponse;
		char	szAccount[32]={0};
		KSG_PASSWORD Password;
		GetAccountPassword(szAccount, &Password);  //��ȡ���ݿ���˺ź�����
		if (strcmp(pInfo->Account,  szAccount)  == 0 &&
			strcmp(pInfo->Password.szPassword, Password.szPassword) == 0)
		{//���صĽ��
			KIniFile nBVer;
			double mBver,mBishopver;
			if (!nBVer.Load("\\Settings\\Ver\\Xfsverjx.ini"))//����������İ汾�� GetDouble
			{
				isthisVer = false;
				m_Result = LL_R_INVALID_PROTOCOLVERSION;  //�汾����
				//ReturnToIdle();//�Ͽ�����
				nBVer.Clear();
#ifdef WIN32
				ccMessageBox("�汾����","��ʾ:");
#else
                ccMessageBox(UTEXT("�汾����",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
				return;
			}
			nBVer.GetDouble("CilentVer","ver",0.1,&mBver);  //�������汾�� GetDouble
			CcharToDouble(pInfo->nBishopver,mBishopver)	;  //bishiop�������İ汾

			if  (pInfo->nSelSererIdx<=0)
			{//���з������ͻ��˶����
			  if (mBishopver!=mBver)
				isthisVer = false;
			  else
				isthisVer = true;
			}
			else
			{
			  if (m_ClientChoices.nServerRegionIndex==pInfo->nSelSererIdx)
			  {//��������������ż��
				  if (mBishopver!=mBver)
					  isthisVer = false;
				  else
					  isthisVer = true;
			  }
			  else //����Ͳ���� ֱ��ͨ��
				  isthisVer = true;
			}
			//------------�࿪���--------------
			if (pInfo->nForbitopennum)
			{
			}
			nBVer.Clear();
			//Ԥ������ɫ���
			if (pInfo->ProtocolVersion!=KPROTOCOL_VERSION)
			{
#ifdef WIN32
				ccMessageBox("�汾������!","��ʾ:");
#else
				ccMessageBox(UTEXT("�汾������!",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
				m_Result = LL_R_INVALID_PROTOCOLVERSION;  //�汾����
				return;
			}
			//----------------------------------
			int nResult = ((pResponse->Param) & ~LOGIN_ACTION_FILTER);
			if (nResult == LOGIN_R_SUCCESS)
			{
				//g_NetConnectAgent.UpdateClientRequestTime(false);
				m_Status = LL_S_WAIT_ROLE_LIST;			  //���ڻ�ȡ��ɫ��Ϣ
				//m_Result = LL_R_ACCOUNT_CONFIRM_SUCCESS;  //���
				//m_LeftTime = pInfo->nLeftTime;
				//m_VipType  = pInfo->nVipType;
				//m_ExtPiont = pInfo->nExtPiont;
				//ccMessageBox("��½�ɹ�","ProcessAccountLoginResponse");
			}
			else
			{
				LOGIN_LOGIC_RESULT_INFO eResult = LL_R_NOTHING;
				switch(nResult)
				{
				case LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR:  //�������
					//eResult = LL_R_ACCOUNT_PWD_ERROR;
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
#ifdef WIN32
					ccMessageBox("�������","��ʾ:");
#else
					ccMessageBox(UTEXT("�������",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					break;
				case LOGIN_R_ACCOUNT_EXIST:   //�˺Ÿ�����  --�˺�����ʹ��
					//eResult = LL_R_ACCOUNT_LOCKED;
#ifdef WIN32
					ccMessageBox("�ʺ�����ʹ��","��ʾ:");
#else
                   ccMessageBox(UTEXT("�ʺ�����ʹ��",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				case LOGIN_R_FREEZE:  // �˺ű�����
					//eResult = LL_R_ACCOUNT_FREEZE;
#ifdef WIN32
					ccMessageBox("�ʺű�����","��ʾ:");
#else
                    ccMessageBox(UTEXT("�ʺű�����",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				case LOGIN_R_INVALID_PROTOCOLVERSION: // �汾������
					//eResult = LL_R_INVALID_PROTOCOLVERSION
#ifdef WIN32
					ccMessageBox("�汾������","��ʾ:");
#else
                   ccMessageBox(UTEXT("�汾������",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					break;
				case LOGIN_R_FAILED:  //����ʧ��
					//eResult = LL_R_CONNECT_SERV_BUSY; // ��ʾ������æ
#ifdef WIN32
					ccMessageBox("����ʧ��","��ʾ:");
#else
                    ccMessageBox(UTEXT("����ʧ��",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					break;
				case LOGIN_R_TIMEOUT: // �˺�ʹ��ʱ�䵽
					//eResult = LL_R_ACCOUNT_NOT_ENOUGH_POINT;
#ifdef WIN32
					ccMessageBox("�ʺ��Ѿ�����","��ʾ:");
#else
                    ccMessageBox(UTEXT("�ʺ��Ѿ�����",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					m_Status = LL_S_WAIT_INPUT_ACCOUNT;
					break;
				default:
#ifdef WIN32
					ccMessageBox("����ʧ��","��ʾ:");
#else
					ccMessageBox(UTEXT("����ʧ��",1).c_str(),UTEXT("��ʾ:",1).c_str());
#endif
					break;
				}

				if (m_Status != LL_S_WAIT_INPUT_ACCOUNT)
				{//�Ͽ�����
					//ReturnToIdle();
				}
				m_Result = eResult;
			}
		}
		memset(szAccount, 0, sizeof(szAccount));
		memset(&Password, 0, sizeof(Password));
	}
}


//--------------------------------------------------------------------------
//	���ܣ���ɫ�б���
//	״̬�л����ɹ� LL_S_WAIT_ROLE_LIST -> LL_S_ROLE_LIST_READY
//			  ʧ�� ״̬����
//--------------------------------------------------------------------------
void Klogin::ProcessRoleListResponse(TProcessData* pResponse)
{
	//_ASSERT(m_Status == LL_S_WAIT_ROLE_LIST && pResponse != NULL);
	if (pResponse->nProtoId == s2c_roleserver_getrolelist_result)
	{
		m_nNumRole = pResponse->pDataBuffer[0];
		if (m_nNumRole > MAX_PLAYER_PER_ACCOUNT)
			m_nNumRole = MAX_PLAYER_PER_ACCOUNT;
		else if (m_nNumRole < 0)
			m_nNumRole = 0;

		RoleBaseInfo* pList = (RoleBaseInfo*)&pResponse->pDataBuffer[1];
		for (int i = 0; i < m_nNumRole; ++i)
		{
			if (pList->szName[0])
			{
				strcpy(m_RoleList[i].Name, pList->szName);
				m_RoleList[i].Attribute = pList->Series;
				m_RoleList[i].Gender = pList->Sex;
				m_RoleList[i].nLevel = pList->Level;
				pList ++;
			}
			else
			{
				m_nNumRole = i;
				break;
			}
		}
		//g_NetConnectAgent.UpdateClientRequestTime(true);
		m_Status = LL_S_ROLE_LIST_READY;

		///ccMessageBox("��ɫ����׼���õ�½","ProcessRoleListResponse");
		//m_Result = LL_R_NOTHING;
		/*if (m_bInAutoProgress)
		{//������Զ���½
			int nAdviceChoice;
			GetRoleCount(nAdviceChoice);
			SelectRole(nAdviceChoice);
		}*/
	}
}

void Klogin::mExit(Ref* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	ccMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	ax::Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif

}
/*
//���̵�����������ý���ʱ����
void Klogin::editBoxEditingDidBegin(extension::EditBox *editBox)
{
	CCLOG("begin");
}

//�������غ������ʧȥ����ʱ����
void Klogin::editBoxEditingDidEnd(extension::EditBox *editBox)
{
	CCLOG("end");
}

//��������ı��仯ʱ����
void Klogin::editBoxTextChanged(extension::EditBox *editBox, const std::string &text)
{
	CCLOG("change");
}

//���·��ؼ������������λ��ʱ�����������ǰ�����ʱ��������
void Klogin::editBoxReturn(extension::EditBox *editBox)
{
	CCLOG("return");
}*/
