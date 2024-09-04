//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __uiskill__
#define __uiskill__

#include "cocos2d.h"


using namespace ax;

using namespace std;

#define LIVE_SKILL_COUNT			10
#define	FIGHT_SKILL_COUNT			50
#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define	FIGHT_SKILL_SUB_PAGE_COUNT	FIGHT_SKILL_COUNT / FIGHT_SKILL_COUNT_PER_PAGE

class uiskill:public Layer
{

public:
    uiskill();
    ~uiskill();

    virtual bool init();
    CREATE_FUNC(uiskill);

     // ��Ҫ��д����ע�ắ�������¸�����������
    virtual void registerWithTouchDispatcher(void);
     // ��д������������Զ���� true �����������㣬�ﵽ ��ģ̬�� Ч��
    bool ccTouchBegan(ax::Touch *pTouch,ax::Event *pEvent);
    // ���ܣ������öԻ��򱳾�ͼƬ
    static uiskill* create(const char* backgroundImage,int nKind=0);
	//static uiskill* createWith();

     // ��������ʾ���⣬�����趨��ʾ���ִ�С
    void setTitle(const char*title,int fontsize = 20);
    // �ı����ݣ�padding Ϊ���ֵ��Ի�������Ԥ���ľ��룬���ǿɿصģ����Ϸ��ľ����������
    void setContentText(const char *text, int fontsize = 20, int padding = 50, int paddintTop = 100);
    // �ص��������������ť�����ǹرյ������ͬ�£���Ҫһ���ص���������֪ͨ���ǵ�����ĸ���ť������ж����
    void setCallbackFunc(Ref* target, const std::function<void(ax::Node*)>& callfun);
     // Ϊ����Ӱ�ť���棬��װ��һ������������Щ��Ҫ�Ĳ���
    bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0,int isSpr=0);

    // Ϊ������ʾ��ʱ֮ǰ��������Ч��ѡ���� onEnter �ﶯ̬չʾ
    virtual void onEnter();
    virtual void onExit();
	virtual void update(float delta);
//	virtual void draw();
private:

    void buttonCallback(Ref* pSender);

    // �����������ߵĿհ���
    int m_contentPadding;
    int m_contentPaddingTop;

    Ref* m_callbackListener;
    std::function<void(ax::Node*)> m_callback;

	AX_SYNTHESIZE_RETAIN(Sprite*, m_ParentNode_Fight, ParentNode_Fight);
    AX_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
    AX_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
    AX_SYNTHESIZE_RETAIN(Scale9Sprite*, m__s9BackGround, Sprite9BackGround);
    AX_SYNTHESIZE_RETAIN(Label*, m__ltTitle, LabelTitle);
    AX_SYNTHESIZE_RETAIN(Label*, m__ltContentText, LabelContentText);

	//KUiSkillData	Skills[FIGHT_SKILL_COUNT];
	//Node *ParentNode_Fight;

};
#endif /* defined(__PopupDemo__PopupLayer__) */
