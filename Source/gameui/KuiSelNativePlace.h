#ifndef __KuiSelNativePlace__
#define __KuiSelNativePlace__
/*
//��½���� ѡ������ ���� ɾ����
*/
#include "cocos2d.h"

#include <gameui/Uiglobaldata.h>
#include<list>
#include "ImageStore/KImageStore2.h"
#include "engine/Kgetinidata.h"
#include "physics-nodes/PhysicsSprite.h"

#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"

using namespace ax;
//
using namespace std;

struct KNativePlace
{
	char			szName[32];
	char			szImage[128];
	unsigned short	nId;
	unsigned short	nDescLen;
	char			sDesc[256];
	unsigned short  inRevID;  //������
};
#define	PLACE_LIST_FILE	 "\\Settings\\NativePlaceList.ini"
#define	PLACE_LIST_FILE_VN	 "\\Settings\\NativePlaceList_vn.ini"
class KuiSelNativePlace : public ax::Layer,public Rect//,public EditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in ax-x returns bool, instead of returning 'id' in ax-iphone
	KuiSelNativePlace();
	~KuiSelNativePlace();
	virtual bool init();
	/*virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
   */
	virtual void update(float delta);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static ax::Scene* scene();

	//Label* GetTestLabel(){return pMainLabel;};
	// implement the "static node()" method manually
	CREATE_FUNC(KuiSelNativePlace);
	//��дdraw()����
//	virtual void draw();
	KImageStore2 m_ImageStore;
	//Size visibleSize;
	//Point origin;
private:
	//Label*pMainLabel;
	//Label* pBugLabel;
	Label* pDescLabel; //˵��
	//====iKNetMsgTargetObject�ӿں���====
	void AcceptNetMsg(void* pMsgData);	//����������Ϣ
	bool clicked;  //�Ƿ�˫��
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����
	//KRoleInfo  nCurRoleList[MAX_PLAYER_PER_ACCOUNT];
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	Rect getRect(Node* pNode);
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	void  buttonCallback(Node *pNode);
	// a selector callback
	void mainEnterCallback(Ref* pSender);
	void CheckClick(float tt);

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

	void LoadList(int nPlaceId);

	unsigned char	m_Gender[MAX_PLAYER_PER_ACCOUNT];
	unsigned char	m_Attribute[MAX_PLAYER_PER_ACCOUNT];
	char		    m_szLoginBg[32];
	char		    m_szPlayerImgPrefix[128];
	int             m_CurSelIndex;
	int   m_selIndex; //ѡ�������
	ActionManager* actionManager;
	Node* pListNode; //ѡ��˵�
	Sprite *SelbgSprite;
	Sprite *showSprite;
	KNativePlace*	m_pPlaceList;
	int    m_itemsCount;
	int    m_nLastSelPlace;
};

#endif // __KuiSelNativePlace__
