#ifndef __KuiNewPlayer__
#define __KuiNewPlayer__
/*
//��½���� ѡ������ ���� ɾ����
*/
#include "cocos2d.h"
#include "cocos-ext.h"
#include <gameui/Uiglobaldata.h>
#include<list>
#include "ImageStore/KImageStore2.h"
#include "engine/Kgetinidata.h"
#include "physics-nodes/CCPhysicsSprite.h"

#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"

using namespace ax;
//using namespace ax::extension;
using namespace std;
struct KRoleInfonew
{
	char				Name[32];		//����
	unsigned	char	Gender;			//�Ա�	be one of the SSC_CHARACTER_GENDER value
	unsigned	char	Attribute;		//��������
	unsigned short	NativePlaceId;	//������ID
	short			nLevel;			//�ȼ�		
};
class KuiNewPlayer : public ax::Layer,public Rect//,public EditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in ax-x returns bool, instead of returning 'id' in ax-iphone
	KuiNewPlayer();
	~KuiNewPlayer();
	virtual bool init();  
	virtual void update(float delta); 
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static ax::Scene* scene();
	CREATE_FUNC(KuiNewPlayer);
	//��дdraw()����
    virtual void onDraw(const ax::Mat4 &transform, uint32_t flags);
	KImageStore2 m_ImageStore;
	//Size visibleSize;
	//Point origin;
private: 
    /*Label* pRoela;
	Label* pRoelb;
	Label* pRoelc;*/
	//====iKNetMsgTargetObject�ӿں���====
	void	AcceptNetMsg(void* pMsgData);	//����������Ϣ
	bool clicked;  //�Ƿ�˫��
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����
	KRoleInfonew  m_Info;
	//Sprite *testSprite;
	//int       nTestCount;
	//UINT      nTimeCount;
	MenuItemImage *pLoginItem;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	Rect getRect(Node* pNode);
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	void  buttonCallback(Node *pNode);
	int   GetRoleCount(int& nAdviceChoice);
	// a selector callback
	void mainEnterCallback(Ref* pSender);
	void CheckClick(float tt);
	void GetRoleImageName(char* pszName, const char* pszPrefix, unsigned char bGender, unsigned char bAttribute, int nIndex);
	long _GetBig5Count(char *str);
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
	void ProcessCreateRoleResponse(tagNewDelRoleResponse* pResponse);	//�½���ɫ�б����Ӧ
	void ProcessToLoginGameServResponse(tagNotifyPlayerLogin* pResponse);//׼����½��Ϸ����������Ӧ
	Animation* creatSpranimation(int nindex,int isLoop=-1,bool isforever=true);
	void finish();
	void mExit(Ref* pSender);
	void mSellistCallBack(Ref* pSender);
	int  CreateRole();

	//unsigned char	m_Gender[MAX_PLAYER_PER_ACCOUNT];
	//unsigned char	m_Attribute[MAX_PLAYER_PER_ACCOUNT];
	char		    m_szLoginBg[32];
	char		    m_szPlayerImgPrefix[128];
	int             m_CurSelIndex;
	Node *RoleNodeman;   //�нڵ�
	Node *RoleNodewoman; //Ů�ڵ�
	ActionManager* actionManager;
	Sprite *SelbgSprite;
	EditBox *proleNameEditBox;
	struct PROPTYPEINFO
	{
		int					nShowTextLen;
		char				propertyShow[256];		//����˵��
		char				szMaleImg[128];
		char				szFemaleImg[128];
		char				szMaleSound[128];
		char				szSeriesImg[128];
		char				szFemaleSound[128];
	}m_propTypeInfoTable[series_num];
};

#endif // __GAMEWORLD_SCENE_H__
