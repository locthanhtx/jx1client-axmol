#include "engine/KbugInfo.h"
#include "gamecore/KCore.h"
#include "PopupLayer.h"
#include "engine/CXmlStream.h"
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

extern iCoreShell*	g_pCoreShell;
#include <list>
USING_NS_AX;

PopupLayer::PopupLayer():
m__pMenu(NULL)
,m_contentPadding(0)
,m_contentPaddingTop(0)
,m_callbackListener(NULL)
,m_callback(NULL)
,m__sfBackGround(NULL)
,m__s9BackGround(NULL)
,m__ltContentText(NULL)
,m__ltTitle(NULL)
{

}

PopupLayer::~PopupLayer()
{
    CC_SAFE_RELEASE(m__pMenu);
    CC_SAFE_RELEASE(m__sfBackGround);
    CC_SAFE_RELEASE(m__ltContentText);
    CC_SAFE_RELEASE(m__ltTitle);
    CC_SAFE_RELEASE(m__s9BackGround);
}

bool PopupLayer::init()
{
    bool bRef = false;
    do
    {
        AX_BREAK_IF(!Layer::init());
        this->setContentSize(ax::Size::ZERO);
        // ��ʼ����Ҫ�� Menu
        Menu* menu = Menu::create();
        menu->setPosition(ax::Vec2::ZERO);
        setMenuButton(menu);
//        setTouchEnabled(true);
        bRef = true;
    } while (0);
    return bRef;
}


void PopupLayer::registerWithTouchDispatcher()
{
    auto touchListener = ax::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    // ����Ĵ������ȼ�����Ϊ -128 �Ᵽ֤�ˣ������·��Ĵ���
    //ax::Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,-128, true);
}

bool PopupLayer::ccTouchBegan(ax::Touch *pTouch, ax::Event *pEvent)
{//����������
   // CCLOG("PopupLayer touch");
    return true;
}


PopupLayer* PopupLayer::create(const char *backgroundImage,int nKind)
{
    PopupLayer* ml = PopupLayer::create();
	if  (!nKind)
	{
      ml->setSpriteBackGround(Sprite::create(backgroundImage));//�����ľ���
//      ml->setSprite9BackGround(ax::ui::Scale9Sprite::create(backgroundImage));
	}
	else
	{//������أ���������
		char nSprName[128]={0};
		ZeroMemory(nSprName,sizeof(nSprName));
		sprintf(nSprName, "%s", backgroundImage);
		g_StrLower(nSprName);
		int m_nWidth,m_nHeight,nFrams;
		Texture2D *bgCur = NULL;
		SPRFRAMSINFO nSprInfo;
		ZeroMemory(&nSprInfo,sizeof(nSprInfo));
		bgCur = _getinidata.getinidata_one(nSprName,0,&m_nWidth,&m_nHeight,&nFrams,&nSprInfo);
		if (bgCur==NULL)
			return NULL;
		ml->setSpriteBackGround(Sprite::createWithTexture(bgCur));//�����ľ���
	}
    return ml;
}


void PopupLayer::setTitle(const char*title,int fontsize)
{
    Label* ltfTitle = Label::createWithTTF(title, "", fontsize);
    setLabelTitle(ltfTitle);
}

void PopupLayer::setContentText(const char *text, int fontsize, int padding, int paddingTop){
    Label* ltf = Label::createWithTTF(text, "", fontsize);
    setLabelContentText(ltf);
    m_contentPadding    = padding;
    m_contentPaddingTop = paddingTop;
}

void PopupLayer::setCallbackFunc(ax::Ref *target, const std::function<void(ax::Node*)>& callfun)
{
    m_callbackListener = target;
    m_callback         = callfun;
}


bool PopupLayer::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag,int isSpr){

	Size winSize = ax::Director::getInstance()->getWinSize();//��ǰ���ڵĸ߶ȺͿ��
    Point pCenter = ax::Vec2(winSize.width/2, winSize.height/2);

    // ����ͼƬ�˵���ť
    MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage, CC_CALLBACK_1(PopupLayer::buttonCallback, this));
    menuImage->setTag(tag);
    menuImage->setPosition(pCenter);
    // �������˵��������λ�� ���ְ�ť
    Size imenu = menuImage->getContentSize();
    Label* ttf = Label::createWithTTF(title,"",20);
    ttf->setColor(Color3B(0,0,0));
    ttf->setPosition(ax::Vec2(imenu.width/2,imenu.height/2));
    menuImage->addChild(ttf);
    getMenuButton()->addChild(menuImage);
    return true;
}


void PopupLayer::buttonCallback(ax::Ref *pSender){
    Node* node = dynamic_cast<Node*>(pSender);
    //CCLOG("touch tag: %d", node->getTag());
    if (m_callback && m_callbackListener){
        (m_callback)(node);
    }
	//�ͷ���
    this->removeFromParentAndCleanup(true);
}

void PopupLayer::onEnter()
{
    Layer::onEnter();

    Size winSize = ax::Director::getInstance()->getWinSize();
    Point pCenter = ax::Vec2(winSize.width / 2, winSize.height / 2);

    Size contentSize;
     // �趨�ò�����������ʱ����
    if (getContentSize().equals(ax::Size::ZERO))
    {//���û�����ô�С��
        getSpriteBackGround()->setPosition(ax::Vec2(winSize.width / 2, winSize.height / 2));
        this->addChild(getSpriteBackGround(),0,0);
        contentSize = getSpriteBackGround()->getTexture()->getContentSize();
    }else
    {//��������� �Ͱ��� 9������
        ax::ui::Scale9Sprite *background = getSprite9BackGround();
        background->setContentSize(getContentSize());
        background->setPosition(ax::Vec2(winSize.width / 2, winSize.height / 2));
        this->addChild(background,0);
        contentSize = getContentSize();
    }

     // ��Ӱ�ť����������λ��
    this->addChild(getMenuButton());
    //float btnWidth = contentSize.width/(getMenuButton()->getChildrenCount()+1);

    auto array = getMenuButton()->getChildren();
    Ref* pObj = NULL;

	MenuItemImage* itemImage = dynamic_cast<MenuItemImage*>(array.at(0));
    int count = getMenuButton()->getChildrenCount();
    int btnWidth = itemImage->getContentSize().width;
    int padingWidth = (contentSize.width - btnWidth * count) / (count + 1);
	int i = 0;
    for (const auto &pObj: array)
    {//���ð�ť��λ��
        Node* node = dynamic_cast<Node*>(pObj);
        node->setAnchorPoint(ax::Vec2(0, 0.5f)); //������㣿
        node->setPosition(ax::Vec2(winSize.width * 0.5f - contentSize.width * 0.5f + padingWidth * (i + 1) + btnWidth * i, winSize.height * 0.5f - contentSize.height / 3));
        i++;
    }

    /*int i = 0; ԭ����
    CCARRAY_FOREACH(array, pObj)
    {
        Node* node = dynamic_cast<Node*>(pObj);
        node->setPosition(ax::Vec2(winSize.width / 2 - contentSize.width / 2 + btnWidth * (i + 1), winSize.height / 2 - contentSize.height / 3));
        i++;
    }*/

    // ��ʾ�Ի������
    if (getLabelTitle())
    {//����б���Ļ�
        getLabelTitle()->setPosition((pCenter + ax::Vec2(0, contentSize.height / 2 - 35.0f)));
        this->addChild(getLabelTitle());
    }

    // ��ʾ�ı�����
    if (getLabelContentText())
    {
        Label* ltf = getLabelContentText();
        ltf->setPosition(ax::Vec2(winSize.width / 2, winSize.height / 2));
        ltf->setDimensions(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop);
        ltf->setHorizontalAlignment(ax::TextHAlignment::LEFT);
        this->addChild(ltf);
    }
    //����Ч��
   // Action* popupLayer = Sequence::create(CCScaleTo::create(0.0, 0.0),
   //                                           CCScaleTo::create(0.06, 1.05),
     //                                         CCScaleTo::create(0.08, 0.95),
       //                                       CCScaleTo::create(0.08, 1.0), NULL);
    //this->runAction(popupLayer); //ִ�������Ϊ

	this->setPosition(ax::Vec2(0,0));   //�����������λ��

}


void PopupLayer::onExit()
{
    //CCLOG("popup on exit.");
    Layer::onExit();
}

void PopupLayer::update(float delta)
{
	//messageBox("����ѭ��","update");
}

//void PopupLayer::draw()
//{
//	//messageBox("����ѭ��","draw");
//}
