#ifndef __GAME_KSELPLAYER_H__
#define __GAME_KSELPLAYER_H__
/*
//��½���� ѡ������ ���� ɾ����
*/
#include "cocos2d.h"

#include <gameui/Uiglobaldata.h>
#include <list>
#include "ImageStore/KImageStore2.h"
#include "engine/Kgetinidata.h"
#include "physics-nodes/PhysicsSprite.h"

#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"

using namespace ax;
//
using namespace std;
struct KRoleInfo
{
	char				Name[32];		//����
	unsigned	char	Gender;			//�Ա�	be one of the SSC_CHARACTER_GENDER value
	unsigned	char	Attribute;		//��������
	unsigned short	NativePlaceId;	//������ID
	short			nLevel;			//�ȼ�
};
class KSelPlayer : public ax::Layer//,public EditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in ax-x returns bool, instead of returning 'id' in ax-iphone

	virtual bool init();
	/*virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
   */
	virtual void update(float delta);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static ax::Scene* scene();
	// implement the "static node()" method manually
	CREATE_FUNC(KSelPlayer);
	//��дdraw()����
    virtual void draw(Renderer* renderer, const Mat4& transform, unsigned int flags) override;
	KImageStore2 m_ImageStore;
	//Size visibleSize;
	//Point origin;
private:
	//Label*pMainLabel;
	//Label* pBugLabel;
    Label* pRoela;
	Label* pRoelb;
	Label* pRoelc;
	//====iKNetMsgTargetObject�ӿں���====
	void	AcceptNetMsg(void* pMsgData);	//����������Ϣ
	bool clicked;  //�Ƿ�˫��
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����
	KRoleInfo  nCurRoleList[MAX_PLAYER_PER_ACCOUNT];
	//Sprite *testSprite;
	//int       nTestCount;
	//UINT      nTimeCount;
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
	void mainEnterCallback(Ref* pSender);
	void mainNewCallback(Ref* pSender);
	void mainDelCallback(Ref* pSender);
	void CheckClick(float tt);
	void GetRoleImageName(char* pszName, const char* pszPrefix, unsigned char bGender, unsigned char bAttribute, int nIndex);

	// default implements are used to call script callback if exist
	virtual bool ccTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void ccTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void ccTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void ccTouchCancelled(Touch *pTouch, Event *pEvent);
	// default implements are used to call script callback if exist
	virtual void ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesCancelled(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void didAccelerate(Acceleration* pAccelerationValue);

	void ProcessToLoginGameServResponse(tagNotifyPlayerLogin* pResponse);//׼����½��Ϸ����������Ӧ

	void finish();
	void mExit(Ref* pSender);

	unsigned char	m_Gender[MAX_PLAYER_PER_ACCOUNT];
	unsigned char	m_Attribute[MAX_PLAYER_PER_ACCOUNT];
	char		    m_szLoginBg[32];
	char		    m_szPlayerImgPrefix[128];
	int             m_CurSelIndex;
	int   m_NpcIndex; //�ͻ������ǵ�NPC����
	char nBugInfo[128];
	Node *RoleNodea;
	Node *RoleNodeb;
	Node *RoleNodec;
	ActionManager* actionManager;
	Sprite *SelbgSprite;
};

#endif // __GAMEWORLD_SCENE_H__
