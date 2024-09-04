#ifndef __GAMEWORLD_SCENE_H__
#define __GAMEWORLD_SCENE_H__

#include "cocos2d.h"
#include <list>
#include "gameui/Uiglobaldata.h"
#include "ImageStore/KImageStore2.h"
#include "engine/Kgetinidata.h"
#include "engine/KGbktoUtf8.h"
#include "engine/KTimer.h"
#include "engine/KLinkArray.h"
#include "gamecore/GameDataDef.h"
#include "gameui/KuiChatList.h"
#include "gameui/KuiMyMenu.h"
USING_NS_AX;
using namespace ui;
#define MAX_NUM_REGIONS	9
#define MAX_SPR_COUNT  165
#define MAX_TREES_COUNT  100
#define MAX_HOUSE_COUNT  100
#define MAX_ABOVE_COUNT  100
#define MAX_FUZHUSKILL_COUNT 5
#define MAX_MSGREV_COUNT 5

#include "gamescene/KHRocker.h"
#include "gamescene/KSkillRocker.h"

struct mapRegData
{
	KLinkArray		m_FreeIdxNode;				//	可用表
	KLinkArray		m_UseIdxNode;				//	已用表

	KLinkArray		m_FreeIdxPartNode;			//	可用表
	KLinkArray		m_UseIdxPartNode;			//	已用表

	int             nDrawKind;
	int             nSprCount;
};

struct msgListData
{
	int             nindex;      //当前的索引
	int             nkind;       //信息的类型
	float           offsetY;     //当前高度的偏移
	char            m_msgContent[256];//内容
};

struct skillstickData
{
	KSkillRocker  *skillstick;
	Sprite      *controlSprite_s;
	Sprite      *ygSprite_s;
};

struct auxiliarySkillData
{
	//Sprite *auxiliaryskillSpr;
	MenuItemSprite* auxiliaryskillSpr;
	float OffsetX,OffsetY;
	int m_skillidx;
};
//快捷物品描绘
class KImmediaItem
{
public:
	int m_nIndex;
	//int m_nIsSKill;
	//int m_mIsFull;
	int m_uid;
	//DWORD m_TextColor;
	KImmediaItem() :m_nIndex(-1){} //, m_TextColor(0xFFFFFF00)
//	int	Init(KIniFile* pIniFile, const char* pSection);
};

class KgameWorld : public ax::Layer//,public ui::Layout
{
public:
	// Here's a difference. Method 'init' in ax-x returns bool, instead of returning 'id' in ax-iphone
	KgameWorld();
	~KgameWorld();
	virtual bool init();
	/*virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
   */
	virtual void update(float delta);
    virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static ax::Scene* scene();
	CREATE_FUNC(KgameWorld);


	//void registerWithTouchDispatcher();
	KImageStore2 m_ImageStore;
	KTimer start;
	void setIsPaint(bool ver);
	void KickOutGame();
	void MoveObject(int ObjKind,int nIndex,bool nIsClear,int nPosX=0,int nPosY=0,int nPosZ=0);
	void RemoveObject(int ObjKind,int nIndex);
	void ergodicAllNode(Node *rootNode);
	void SetHorseState(int nNpcIndex);
	void SetPiFengState(int  nNpcIndex);
	//Texture2D * ccgetTxtData(char *sprPath,int nFramIndex,int nMpsX,int nMpsY,int nMpsZ,int *mScreeX,int *mScreeY,int bRenderFlag,int bSinglePlaneCoord,int *mHeadw,int *mHeadh);
	//Texture2D * ccgetTxtData_g(char *sprPath,int nFramIndex,int nMpsX,int nMpsY,int nMpsZ,int *mScreeX,int *mScreeY,int bRenderFlag,int bSinglePlaneCoord,int *mHeadw,int *mHeadh);
	void DrawPrimitives_State(int nNpcIndex,int nstateIdx,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord);
	void DrawPrimitives_MenuState(int nNpcIndex,int nstateIdx,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord);
	void DrawShadow(int nNpcIndex,int nPos, KRUImage* pPrimitives);
	void DrawPrimitives(int nNpcIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord);
	void DrawPrimitives_obj(int nObjIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord,ax::Color3B nObjColor,char *nObjName,char *_clientObjName);
	void DrawSpecial_miss(int nMissIndex,int eStatus,int nNpcIndex,unsigned int nNpcDwidx,int nPos,KRUImage* pPrimitives,bool isClear ,unsigned int uGenre,int nOrDer,int bSinglePlaneCoord);
	void DrawPrimitives_miss(int nMissIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord);
	void DrawPrimitives_map(int nRegIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord,POINT nRegPiont,POINT nRegRbMpsPoint);
	void _DrawPrimitives_map(int nRegIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord,POINT nRegPiont,POINT nRegRbMpsPoint);
	void Draw_Part_map(int nRegIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord,POINT nRegPiont,POINT nRegRbMpsPoint);
	void DrawPrimitives_BigMap(int nRegIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord,POINT nRegPiont);
	void DrawPrimitives_BigMap_Tree(int nRegIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord,POINT nRegPiont);
	void DrawPrimitives_BigMap_house(int nRegIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord,POINT nRegPiont);
	void _DrawPrimitives_BigMap(int nRegIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord,POINT nRegPiont);
	void DrawPrimitives_RolePoint(int nRegIndex,int nPos, KRepresentUnit* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer,int bSinglePlaneCoord);
	void DrawPrimitives_minemap(int nposX,int nposY, RECT pRc,std::string nJpaPath,Texture2D *nMapTxtture=NULL);
	//Node *GetMainWorldNode(int nNodeKind);
	void DrawPrimitives_NpcBlood(int nNpcIndex,int bloodindex,char *strBloodNo,int nNpcMpsX,int nNpcMpsY,int nOffsetY,bool isClear);
	void DrawPrimitives_NpcOther(int nNpcIndex,int m_Kind,bool isClear = false);
	void DrawAboveHead(int index,POINT nRegionIndex,int nCount,KRUImage* pPrimitives,int nKind,int bSinglePlaneCoord=false);
	void setBuWeiHide(int nNpcIndex,bool isHide,int isthis);
	void _setBuWeiHide(int nNpcIndex,bool isHide,int isthis);
	void taskCilentMsgArrival(KNewsMessage* pMsg, int pTime,int nIsBtn);
	void taskCilentFindPathArrival(KTaskPathInfo* pMsg,int pTime,int nIsBtn);
	//void  AddIndex(int nRegIndex,int i);
	//void  RemoveIndex(int nRegIndex,int i);
	//int   GetCount(int nRegIndex);
	//void  ReSetUseNode(int nRegIndex);
	//void  ReSetUsePartNode(int nRegIndex);
	//void  ReSetTreeNode();
	//void  ReSetHouseNode();
	//int   GetTreeNodeCount();
	//int   GetHouseNodeCount();
	/*void  SetOtherSprCount(int nRegIndex,int i);
	int   GetOtherSprCount(int nRegIndex);
	void  SetPartSprCount(int nRegIndex,int i);
	int   GetPartSprCount(int nRegIndex);
	int   GetRegDrawKind(int nRegIndex);
	int   GetRegSprCount(int nRegIndex);
	void  AddPartIndex(int nRegIndex,int i);
	*/
	void  setattackSprInfo(int nSkillIdx,int uGenre,char * icoPath);
	void  setaauxiliaryskillInfo(int nSkillIdx,int uGenre,char * icoPath);
	//聊天系统
	 void ChannelMessageArrival(UINT nChannelID, char* szSendName,char* pMsgBuff, unsigned short nMsgLength, bool bSucc,UINT nItemDwidx=0,int nPackage=-1);
	 //私聊显示(显示对方的信息)（密）
	 void MSNMessageArrival(char* szSourceName, char* szSendName, const char* pMsgBuff, unsigned short nMsgLength, bool bSucc,UINT nItemDwidx=0,bool isClent=false);
	 void NotifyChannelID(char* ChannelName, UINT channelid, BYTE cost);
	 bool AMessageArrival(KSystemMessage* pMsg, void* pParam);

	 //聊天
	 void QueryAllChannel();
	 //---------------------------------
	 void CreatNpcDialog(KUiQuestionAndAnswer* pContent, KUiNpcSpr* pImage=NULL,int nIsTimeClose=0);
	 void CreatTalkDialog(KUiInformationParam* pWordDataList, int nCount);
	 void CreatNpcShopDialog();
	 bool GetuiItemIfVisible(){return m_itemsPadIsOpen;};
	 bool GetuiItemExIfVisible(){return m_itemExPadIsOpen;};
	 bool GetuistoreIfVisible(){return m_storeboxPadIsOpen;};

	 void UpdateItem(KUiObjAtRegion* pItem, int bAdd);//物品变化更新
	 void UpdateItemEx(KUiObjAtRegion* pItem, int bAdd);//物品变化更新
	 void UpdateStoreBox(KUiObjAtRegion* pItem, int bAdd);
	 void UpdateExBox(KUiObjAtRegion* pItem, int bAdd,int exBoxIndex);
	 void openstringBox(const char* pszTitle,int nMinLen, int nMaxLen,const char* pszCallback,const char* pszError,int bNum);
	 void openStorebox();
	 void openNpcshop(int nshopidx);
	 void openProgressbar(KUiJinDuTiaoInfo* pJinDu=NULL);
	 void openlaotou();
	 void openGive(char* pszTitle, char* pszInitString,int nNum,char* pszCallback,char* pszError);
	 void OpenViewStateDialog(KUiPlayerItem* pDest);
	 void OpenPlayerShopDialog(KUiPlayerItem* pDest);
	 void OpenPlayerTradeDialog(KUiPlayerItem* pDest);
	 void OpenItenLinkDialog(KUiLianItem*  pDest);
	 void OpenFlyNewsDialog_a(KNewsMessage* pMsg, SYSTEMTIME* pTime);
	 void OpenFlyNewsDialog_b(KNewsMessage* pMsg, SYSTEMTIME* pTime);
	 void OpenCreatTongSheet();
	 void OnSelfChangedItem(KUiObjAtRegion* pObj, int bAdd);
	 void OnTarChangedItem(KUiObjAtRegion* pObj, int bAdd);
	 void UpdateTradeOperData();
	 void UpdateTradeEnd();	//交易结束

	 void UpPlayerShopData();
	 void ClosePlayerShop();

	 void FreeAllSprite();
	 void UpdateSkill(KUiSkillData* pSkill, int nIndex);
	 void removespriteByIdx(int nNpcIndex,int nStateIdx,bool isAll=false);
	 void removeMenuByIdx(int nNpcIndex,int nStateIdx);
	 bool CheckImmediacyBox(int nGenre, int DetailType);
	 void UpdateImmediaItem(int nIndex, unsigned int uGenre, unsigned int uId);	//变更物品摆换
	 void UpdateGiveItem(KUiObjAtRegion* pItem, int bAdd);

	 void UpdateDataTeam(KUiPlayerTeam* pInfo);
	 void setchatDelegate(bool isYes);
	 void setchatCurChannel(char  *name);
	 void setchatInputText(char  *text);
	 int  getMapIdx(){return nMapidx;};
	 void setPingTime(unsigned int ver){__pingTime=ver;};
	 void setPadCanMove(int _PadIndex,bool ver);
	 Node   *ParentNode_map;//
	 Node   *ParentNode_Bigmap;
	 Node   *ParentNode_objabove;
	 void setAttackSprVisible(bool ver);
	 void setAttackSprPosition(Point nPos);
	 int  getUseSkillModel(){return m_useSkillModel;};
	 void customSkillAttack(int nSkillid,float xPos,float yPos);
	 void setInputMsg(char * strInfo);
	 void _quitGame();
private:
	char     _strPing[64];
	Node   *ParentNode_rolestate;
	Node   *ParentNode_chat;
	Node   *ParentNode_npc;
	Node   *ParentNode_npc_state;//状态
	Node   *ParentNode_npcother;
	Node   *ParentNode_npcblood;
	Node   *ParentNode_player;
	Node   *ParentNode_miss;
	Node   *ParentNode_miss_Special;
	Node   *ParentNode_item;    //9个快捷栏
	Node   *ParentNode_obj;
	Node   *ParentNode_ShortSkill;
	//Node   *ParentNode_RightSkill;
	Node   *ParentNode_minmap;
	//mapRegData *RegData;
	HRocker   *joystick;
	//KSkillRocker  *skillstick;
	//skillstickData skillstick[MAX_FUZHUSKILL_COUNT];
	//Animate* animate;
	Label* pmanaLabel;
	Label* plifeLabel;
	Label* pSaitamaLabel;
	Label* pExpLabel;
	Label* pPointLabel;
	Label* pMapNameLabel;
	Label* pRoleNameLabel;
	Label* pNetStateLabel;
	Label* ptestLabel;
	MenuItemSprite *ChannelSpr;

	Texture2D * ccgetTxtData(char *sprPath,int nFramIndex,int nMpsX,int nMpsY,int nMpsZ,int *mScreeX,int *mScreeY,int bRenderFlag,int bSinglePlaneCoord,int *mHeadw,int *mHeadh);
	Texture2D * ccgetTxtData_g(char *sprPath,int nFramIndex,int nMpsX,int nMpsY,int nMpsZ,int *mScreeX,int *mScreeY,int bRenderFlag,int bSinglePlaneCoord,int *mHeadw,int *mHeadh);

	Node *PaintPifeng(int m_PifengType,Node *nNpcOtherNode,int nMpsX,int nMpsY);
	Node *PaintHonor(int nbtRankFFId,Node *nNpcOtherNode,int nMpsX,int nMpsY);
	int  FilterTextColor(char* pMsgBuff, unsigned short nMsgLength);

	void qiangItemCallback(Ref* pSender);
	void playerSellCallback(Ref* pSender);
	void playerfunCallback(Ref* pSender);
	void delmapCallback(Ref* pSender);
	void mianYinCallback(Ref* pSender);
	void mianQuCallback(Ref* pSender);
	void mianSortSillCallback(Ref* pSender);
	void huodongCallback(Ref* pSender);
	void autoCallback(Ref* pSender);
	void caveCallback(Ref* pSender);

	int  getIndexBySkillID(int nSkillIdx);
	//交易
	void mianExcCallback(Ref* pSender);
	void auxiliaryskillCallback(Ref* pSender);
	void onsendCallback(Ref* pSender);
	void ImmediaCallback(Ref* pSender);
	//PK
	void mianPkCallback(Ref* pSender);
	void mSkillsCallback(Ref* pSender);
	void mOptionsCallback(Ref* pSender);
	void mTaskCallback(Ref* pSender);
	void mPaimaiCallback(Ref* pSender);
	void mQizCallback(Ref* pSender);
	void mFactionCallback(Ref* pSender);
	void mTeamCallback(Ref* pSender);
	void mItemExCallback(Ref* pSender);
	void mItemsCallback(Ref* pSender);
	void mRoleStatusCallback(Ref* pSender);
	void mianSkillCallback(Ref* pSender);
	void useModelCallback(Ref* pSender);
	void mianHorseCallback(Ref* pSender);
	void mianSitCallback(Ref* pSender);
	void mianRunCallback(Ref* pSender);
	void pickCallback(Ref* pSender);
	void hidechatCallback(Ref* pSender);
	void maxmapCallBackFunc(Ref* pSender);
	void  copyData(const char* pFileName);
	bool  isFileExist(const char* pFileName);
	void  buttonCallback(Node *pNode);
	void  buttonSkillCallback(Node *pNode);
	void  closeSkillPadCallback(Node *pNode);
	void  closeItemPadCallback(Node *pNode);
	void  closeTeamPadCallback(Node *pNode);
	void  closeRolePadCallback(Node *pNode);
	void  closeItemExPadCallback(Node *pNode);
	void  closeNpcDialogPadCallback(Node *pNode);
	void  closeTaikDialogPadCallback(Node *pNode);
	void  closestorePadCallback(Node *pNode);
	void  closeNewDialogPadCallback(Node *pNode);
	void  NpcshopPadCallback(Node *pNode);
	void  getStringPadCallback(Node *pNode);
	void  progressPadCallback(Node *pNode);
	void  closemapPadCallback(Node *pNode);
	void  closesortskillPad_r(Node *pNode);
	void  closesortskillPad_l(Node *pNode);
	void  closeLaoPadCallback(Node *pNode);
	void  closegivePadCallback(Node *pNode);
	void  closeChatlistPad(Node *pNode);
	void  closeplayerfunPadCallback(Node *pNode);
	void  autoPadCallback(Node *pNode);
	void  cavePadCallback(Node *pNode);
	void  closeViewPadCallback(Node *pNode);
	void  closesellItemPadCallback(Node *pNode);
	void  playershopPadCallback(Node *pNode);
	void  closetradePadCallback(Node *pNode);
	void  closeDyshopPadCallback(Node *pNode);
	void  closeMaxMapPadFunc(Node* pNode);
	void  closeLinkPadCallback(Node* pNode);
	void  closeShowSpritePad(Node* pNode);
	void  closetaskPadCallback(Node* pNode);
	void  closeTongPadCallback(Node *pNode);
	void  closeescPadCallback(Node *pNode);
	void  closeTongSheetPadCallback(Node *pNode);
	void  attackFinish();
	//virtual void ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent);

	// default implements are used to call script callback if exist
	/*virtual bool ccTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void ccTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void ccTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void ccTouchCancelled(Touch *pTouch, Event *pEvent);
	*/
	// default implements are used to call script callback if exist
	virtual void ccTouchesBegan(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesMoved(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesEnded(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void ccTouchesCancelled(const std::vector<ax::Touch*>&pTouches, Event *pEvent);
	virtual void didAccelerate(Acceleration* pAccelerationValue);
	void   setadjustColor(int nIndex,Sprite *nResSpr);
	//void   HoldObject(int nType,int nBoxIndex,int nItemidx,bool isAdd);
	void   HoldObject_(int nType,int nBoxIndex,int nItemidx,bool isAdd);
	void   TempUpdataImmediacy();
	int    getImmediacyBoxindex(int nidx);
	void   selchatlistItemEvent(Ref* pSender,ui::ListView::EventType type);

	int    m_NpcIndex;                    //客户端主角的NPC索引
	//char   nBugInfo[256];
	//KCriticalSection    m_GameProcessLock;//互斥锁
	unsigned int nLoopCount;
	//int   nRegSprCount[MAX_NUM_REGIONS];
	//int   nOtherSprCount[MAX_NUM_REGIONS];
	//int   nPartSprCount[MAX_NUM_REGIONS];
	//int	  FindFree(int nRegIndex);
	//int	  FindPartFree(int nRegIndex);
	//int	  FindTreeFree();
	//int	  FindHouseFree();
	//void  AddTreeIndex(int i);
	//void  AddHouseIndex(int i);
	/*KLinkArray		m_FreeIdxTreeNode;			//	可用表
	KLinkArray		m_UseIdxTreeNode;			//	已用表

	KLinkArray		m_FreeIdxHouseNode;			//	可用表
	KLinkArray		m_UseIdxHouseNode;			//	已用表
	*/
	//KLinkArray		m_FreeIdxAboveNode;			//	可用表
	//KLinkArray		m_UseIdxAboveNode;			//	已用表

	POINT	m_MapPos;
    SIZE	m_MapSize;
	char nMapName[64];
	char nMapKind[32];
	int  nMapStartLeft;
	int  nMapStartTop;
	int  nMapidx,nPosX,nPosY;
	bool m_skilPadIsOpen;
	bool m_itemsPadIsOpen;
	bool m_playerSellPadIsOpen;
	bool m_teamPadIsOpen;
	bool m_RoleStatePadIsOpen;
	bool m_itemExPadIsOpen;
	bool m_NpcDialogPadIsOpen;
	bool m_talkDialogPadIsOpen;
	bool m_shopDialogPadIsOpen;
	bool m_storeboxPadIsOpen;
	bool m_npcshopPadIsOpen;
	bool m_getstringPadIsOpen;
	bool m_talkDialogPadIsOpen_1;
	bool m_ProgressPadIsOpen;
	bool m_movemapPadIsOpen;
	bool m_sortskilPadIsOpen_r;
	bool m_sortskilPadIsOpen_l;
	bool m_laotouPadIsOpen;
	bool m_givePadIsOpen;
	bool m_autoPadIsOpen;
	bool m_cavePadIsOpen;
	bool m_playerfunPadIsOpen;
	bool m_viewPadIsOpen;
	bool m_tanweoPadIsOpen;
	bool m_tradePadIsOpen;
	bool m_dyshopPadIsOpen;
	bool m_maxMapPadIsOpen;
	bool m_itemLinkPadIsOpen;
	bool m_taskPadIsOpen;
	bool m_tongPadIsOpen;
	bool m_escPadIsOpen;
	bool m_tongCreatPadIsOpen;
	//RenderTexture *nRegCanvas; //创建一个区域画板
	//主技能按钮
	MenuItemImage *pMainSkillItem;
	MenuItemImage *psortSkillItem_l;
	Menu* sortskillMenu;
	Menu* pcontrolMenu;
	Menu* ImmediaMenu_1;
	Menu* ImmediaMenu_2;
	Menu* ImmediaMenu_3;
	Menu* pminMapMenu;
	KuiMyMenu* KskillMenu;
	Sprite *attackSpr_dir;
	auxiliarySkillData auxiliaryskill[MAX_FUZHUSKILL_COUNT];
	//接受信息的最大数量的数据缓存，超过将删除
	msgListData       m_msgChannelData[MAX_MSGREV_COUNT];
	Point           screenPoint;
	int               mainattackSkill;
#define	UPB_IMMEDIA_ITEM_COUNT	9    //������ ���ܿ�
	KImmediaItem m_ImmediaItem[UPB_IMMEDIA_ITEM_COUNT];
	int m_ImmediaItemSelIndex;
	bool isClose;
	EditBox *inputEditBox;
	int  nCurChannelMsgCount;
	//ui::ListView* chatlistView;
	KuiChatList * chatLayer;
	//ui::ListView *chatListView;
	/*Action* freeze;
	Action* stun;
	Action* poison;
	Action* red;*/
	int chatoffsetY;
	bool isHave;
	Layer * sfunLayer;

	char	m_cPreMsgCounter;
	char	m_cLatestMsgIndex;
#define	MAX_RECENT_MSG_COUNT	8
	char	m_RecentMsg[MAX_RECENT_MSG_COUNT][512];
	int m_nCurChannel;
#define	MAX_RECENTPLAYER_COUNT	10
	char m_RecentPlayerName[MAX_RECENTPLAYER_COUNT][32];
	int m_nRecentPlayerName;

	char m_szSelfName[64];	//自己的名字
	int  AddRecentPlayer(const char* szName);
	void ReplaceSpecialName(char* szDest, uint32_t nDestSize, char* szSrc);
	int  GetChannelIndex(const char* pTitle);
	void OnSendChannelMessage(DWORD nChannelID, const char* Buffer, int nLen);			//����Ƶ�����쵽������
	void OnSendSomeoneMessage(const char* Name, const char* Buffer, int nLen,BOOL nIsItemLink=FALSE);
	void InputCurrentChannel(int nIndex, bool bFocus);
	int  IsHasCost(BYTE cost, int nMoney, int nLevel, int nMana, int nFullMana, int&nUseMoney, int&nUseLevel, int&nUseMana);
	void OnDirectSendChannelMessage(DWORD nChannelID, BYTE cost, const char* Buffer, int nLen,int nItemDwIdx=0);
	bool IsSelfName(char* szName)
	{
		if (szName)
		{
			return (strcmp(m_szSelfName,szName) == 0);   //�Ա������ַ��� ��ͬ ==0
		}
		return false;
	}
#define SECOND_AUTODELMSG 20000
	unsigned int	m_uAutoDelMsgInterval;	//自动删除消息的间隔时间，单位毫秒
	unsigned int	m_uLastDelMsgTime;		//上次删除消息时的时间
	unsigned int    __pingTime;
	unsigned int    m_uLastSwitchTime;
	unsigned int    m_recontnetTime;
	Animation     *animation_attack;
	Sprite        *_Maniattack_State;
	Animate       *Attackanimate;
	ActionManager* actionManager;
	int             m_useSkillModel;
};

#endif // __GAMEWORLD_SCENE_H__
