#ifndef __GAME_KLOGIN_H__
#define __GAME_KLOGIN_H__
/*
//��½���桡�����ʺ�����
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
using namespace ax::extension;
using namespace std;

class Klogin : public Layer//,public EditBoxDelegate
{
public:
	// Here's a difference. Method 'init' in ax-x returns bool, instead of returning 'id' in ax-iphone
	
	virtual bool init();  
	/*virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
   */
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static ax::Scene* scene();
	
	//Label* GetTestLabel(Ref* pSender){return pMainLabel;};
	// implement the "static node()" method manually
	CREATE_FUNC(Klogin);
	//��дdraw()����
//	virtual void draw();
	virtual void update(float delta); 
	//KImageStore2 m_ImageStore;
private: 
	//====iKNetMsgTargetObject�ӿں���====
	void	AcceptNetMsg(void* pMsgData);	//����������Ϣ
	LOGIN_LOGIC_STATUS			m_Status;  //���ͱ�ʶ����
	LOGIN_LOGIC_RESULT_INFO		m_Result; //���ܱ�ʶ����

	Animate* animate;
	Label* pMainLabel;
	
	Sprite *testSprite;
	int       nTestCount;
	UINT      nTimeCount;
	int       m_nWidth;
	int       m_nHeight;
	int       m_sprCount;
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	// a selector callback
	void mianSkillCallback(Ref* pSender);
	void mianLoginCallback(Ref* pSender);
	void mianCancelCallback(Ref* pSender);
	// default implements are used to call script callback if exist
	virtual bool ccTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void ccTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void ccTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void ccTouchCancelled(Touch *pTouch, Event *pEvent);
	// default implements are used to call script callback if exist
	virtual void ccTouchesBegan(const std::vector<ax::Touch*> &Touches, Event *pEvent);
	virtual void ccTouchesMoved(const std::vector<ax::Touch*> &pTouches, Event *pEvent);
	virtual void ccTouchesEnded(const std::vector<ax::Touch*> &pTouches, Event *pEvent);
	virtual void ccTouchesCancelled(const std::vector<ax::Touch*> &pTouches, Event *pEvent);
	virtual void didAccelerate(Acceleration* pAccelerationValue);

	void   finish();//������ɺ� ִ��ɾ������
	void   mExit(Ref* pSender);
	void	GetAccountPassword(char* pszAccount, KSG_PASSWORD* pPassword);
	void	SetAccountPassword(const char* pszAccount, const KSG_PASSWORD* pcPassword);
	void	ClearAccountPassword(bool bAccount, bool bPassword);
private:
	//====��������������Ӧ�ķ��ش���====
	void	ProcessAccountLoginResponse(KLoginStructHead* pResponse);		//�����˺ŵ�½����Ӧ
	void	ProcessRoleListResponse(TProcessData* pResponse);				//��ɫ�б���
};

#endif // __GAMEWORLD_SCENE_H__
