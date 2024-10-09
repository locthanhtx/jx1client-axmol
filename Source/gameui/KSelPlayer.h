#ifndef __GAME_KSELPLAYER_H__
#define __GAME_KSELPLAYER_H__
/*
//登陆界面 选择人物 创建 删除等
*/
#include "cocos2d.h"
//#include "cocos-ext.h"
#include <gameui/Uiglobaldata.h>
#include<list>
#include "ImageStore/KImageStore2.h"
#include "engine/Kgetinidata.h"
//#include "physics_nodes/CCPhysicsSprite.h"

#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"
#include "FairyGUI.h"

USING_NS_FGUI;
using namespace ax;
////using namespace ax::extension;
using namespace std;
struct KRoleInfo
{
	char				Name[32];		//姓名
	unsigned	char	Gender;			//性别	be one of the SSC_CHARACTER_GENDER value
	unsigned	char	Attribute;		//五行属性
	unsigned short	NativePlaceId;	//出生地ID
	short			nLevel;			//等级
};

//class SelPlayer;
class KSelPlayer : public ax::Scene, public Rect//,public CCEditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    KSelPlayer();
    virtual ~KSelPlayer();
	virtual bool init();
	virtual void update(float delta);
	// implement the "static node()" method manually
	CREATE_FUNC(KSelPlayer);
	//重写draw()函数
	virtual void draw(Renderer* renderer, const Mat4& transform, unsigned int flags) override;
	KImageStore2 m_ImageStore;
	//Size visibleSize;
	//Point origin;
protected:
    GRoot* _groot;

private:
    GComponent* _view;
    EventListenerTouchOneByOne *_touchListener;
	//Label*pMainLabel;
	//Label* pBugLabel;
    Label* pRoela;
	Label* pRoelb;
	Label* pRoelc;
	//====iKNetMsgTargetObject接口函数====
	void	AcceptNetMsg(void* pMsgData);	//接受网络消息
	bool clicked;  //是否双击
	LOGIN_LOGIC_STATUS			m_Status;  //发送标识变量
	LOGIN_LOGIC_RESULT_INFO		m_Result; //接受标识变量
	KRoleInfo  nCurRoleList[MAX_PLAYER_PER_ACCOUNT];
	//Sprite *testSprite;
	//int       nTestCount;
	//unsigned int      nTimeCount;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	Rect getRect(Node* pNode);
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	void  buttonCallback(Node *pNode);
	void  EnterGame();
	int   GetRoleCount(int& nAdviceChoice);
	// a selector callback
	void mainEnterCallback(Object* pSender);
	void mainNewCallback(Object* pSender);
	void mainDelCallback(Object* pSender);
	void CheckClick(float tt);
	void GetRoleImageName(char* pszName, const char* pszPrefix, unsigned char bGender, unsigned char bAttribute, int nIndex);

	// default implements are used to call script callback if exist
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
	virtual void didAccelerate(Acceleration* pAccelerationValue);

	void ProcessToLoginGameServResponse(tagNotifyPlayerLogin* pResponse);//准备登陆游戏服务器的响应

	void finish();
	void mExit(Object* pSender);

	unsigned char	m_Gender[MAX_PLAYER_PER_ACCOUNT];
	unsigned char	m_Attribute[MAX_PLAYER_PER_ACCOUNT];
	char		    m_szLoginBg[32];
	char		    m_szPlayerImgPrefix[128];
	int             m_CurSelIndex;
	int   m_NpcIndex; //客户端主角的NPC索引
	char nBugInfo[128];
	Node *RoleNodea;
	Node *RoleNodeb;
	Node *RoleNodec;
	ActionManager* actionManager;
	Sprite *SelbgSprite;
};

#endif // __GAMEWORLD_SCENE_H__
