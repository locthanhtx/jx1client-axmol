//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __Kuiplayerfun__
#define __Kuiplayerfun__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIListView.h"
#include "ui/UICheckBox.h"

USING_NS_AX;
using namespace ui;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
//#define ITEM_CELL_SIZE	28   //ÿ�����ӵĸ߶ȺͿ��
enum  SelfChannel   //����Ƶ��
{
	ch_Team = 0,
	ch_Faction,
	ch_Tong,
	ch_Screen,//ȫ��
	ch_Msgr,
	ch_GM
};

enum CHANNELRESOURCE
{
	CH_NEARBY,
	CH_TEAM,
	CH_WORLD,
	CH_FACTION,
	CH_TONG,
	CH_SYSTEM,
	CH_CITY,
	CH_MSGR,
	CH_CHATROOM,
};

enum LISTAG_CHAT
{
	CH_ALL_LIST=1,
	CH_SHIJIE_LIST,
	CH_CHENGSHI_LIST,
	CH_DUIWU_LIST,
	CH_SILIAO_LIST,
	CH_BANGPAI_LIST,
	CH_COUNT_LIST,
};

#define MAX_CHANNELRESOURCE 8  //Ƶ��������

struct KChannelResourceInfo
{
	char cTitle[32];
	char cShortTitle[MAX_CHANNELRESOURCE][32];
	KRColor uTextColor;
	KRColor uTextBorderColor;
	unsigned short nMenuPicIndex;
	unsigned short nMenuPicHeight;
	char cMenuText[32];
	unsigned short nMenuDeactivatePicIndex;
	unsigned short nMenuDeactivatePicHeight;
	char cMenuDeactivateText[32];
	KRColor uMenuBkColor;
	unsigned short nTextPicIndex;
	char cFormatName[32];	        //����Ƶ������
	int nNeverClose;
	char szSoundFileName[80];
	unsigned int uSendMsgInterval;	//���ڷ�����Ϣ��ʱ����(����)
	unsigned int uSendMsgNum;	    //���ڻ����������Ϣ�����Ŀ,���10��

};

struct KChannelActivateInfo
{
	unsigned long nChannelID;               //Ƶ��ID
	BYTE cost;
	bool bSubscribe;
	int ResourceIndex;	            //-1��ʾ����Դ
	unsigned int uLastSendMsgTime;	//�ϴη���Ϣ��ʱ��
	unsigned int uLeftSendMsgNum;	//�Ѿ�����Ĵ�����Ϣ
	unsigned int uBufferOffset;	    //���ÿռ����ʼλ��
	char Buffer[1024 * 10];         //�����Ϣ��
};

struct KMSN_ChannelInfo
{
	KRColor uTextColorSelf;
	KRColor uTextBorderColorSelf;
	unsigned short nSelfTextPicIndex;
	KRColor uTextBKColorSelf;

	KRColor uTextFriendColor;
	KRColor uTextBorderFriendColor;
	unsigned short nFriendMenuPicIndex;
	unsigned short nFriendMenuPicHeight;
	KRColor uFriendMenuBkColor;
	unsigned short nFriendTextPicIndex;
	char szFriendSoundFileName[80];

	KRColor uTextColorUnknown;
	KRColor uTextBorderColorUnknown;
	unsigned short nStrangerMenuPicIndex;
	unsigned short nStrangerMenuPicHeight;
	KRColor uStrangerMenuBkColor;
	unsigned short nStrangerTextPicIndex;
};
struct KPlayer_Chat_Tab
{
	char	szChatTabName[32];	 //Ƶ�����֣�
	int		nId;
};

#define MAX_CHAT_TAB 9   //����Ƶ������

class Kuiplayerfun:public Layer{
public:
	Kuiplayerfun();
	~Kuiplayerfun();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(Kuiplayerfun);

	static Kuiplayerfun * create(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);

	virtual void update(float delta);
//	virtual void draw();

	bool    isOpen;
	void	UpdateData(KUiPlayerTeam* pInfo);

	void	QueryAllChannel();
	void	ReplaceChannelName(char* szDest, unsigned int nDestSize, char* szSrc);
	int	    ReleaseActivateChannelAll();
	void	OpenChannel(char* channelName, unsigned long nChannelID, BYTE cost);
	int	    PushChannelData(unsigned long dwID, const char* Buffer, int nLen);
	int	    GetChannelData(unsigned long& dwID, BYTE& cost, char*& Buffer, int& nLen,int& nIiemDwIdx);
	int   	PopChannelData(unsigned long dwID);
	void    _removeDelegate(bool isThis);
	unsigned long   GetChannelCount(); // ȡ��Ƶ��������
	int	    GetChannelIndex(unsigned long dwID);
	int	    GetChannelIndex(char* channelName);
	bool    IsChannelType(int nChannelIndex, SelfChannel type);
	char*   GetChannelTitle(int nChannelIndex);
	unsigned long   GetChannelID(int nChannelIndex);
	int     GetChannelResourceIndex(int nChannelIndex);
	int     CheckChannel(int nChannelIndex, bool b);
	bool    GetChannelSubscribe(int nChannelIndex);
	int		NewChannelMessageArrival(unsigned long nChannelID, char* szSendName,char* pMsgBuff, unsigned short nMsgLength,unsigned long nItemDwidx=0,char *scrSendName=NULL,int nPackage=-1);
	void    ShowMSNMessage(char* szName, const char* pMsgBuff, unsigned short nMsgLength,unsigned long nItemDwidx,bool isClient=false);
	int	    FindActivateChannelIndex(unsigned long nChannelID);

	void    setCanMove(bool ver){__isCanMove =ver;};
private:
	//���Ӽ���ͼ��
	void setBtnTouchEnabled(ListView*listView,bool mEnabled);
	void addDialogData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	void selectedItemEvent(Ref* pSender, ListView::EventType type);
	void selectedPlayersListItemEvent(Ref* pSender, ListView::EventType type);
	void selectedBoxStateEvent(Ref* pSender, CheckBox::EventType type);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();

	void oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);
	void ccTouchMoved(Touch *pTouch, Event *pEvent);
	void ccTouchEnded(Touch *pTouch, Event *pEvent);
	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);
	void btnCallBackFunc(Ref * pSender);

	void Clear();
	void OnRefresh();					//ˢ��
	void touchPageBtnEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
	//�ϲ����
	Ref * m_callbackListener;
	Button* btnPageBox_q;//ȫ��
	Button* btnPageBox_s;//����
	Button* btnPageBox_c;//����
	Button* btnPageBox_d;//����
	Button* btnPageBox_sl; //˽��
	Button* btnPageBox_m;//����
	Button* btnPageBox_b;//����
	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
	//�Ի��򱳾���С
	Point m_origin;
    Size m_size;
	Size winSize;
    Size bgSize;
	//�Ի��򱳾�����
	//Sprite * m_bgSprite;
	Sprite *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	//Label* pMoneyLabel;
	//Label* pXLabel;
	LayerColor * colorLayer;
	Action* red;

	//int  m_nMoney;
	//int  m_nXu;
	//int  m_nNumObjects;
	//POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	Layer *m_pUiLayer;
	//Layout  *m_pWidget;
	//CCArray* m_array;
	ListView* pNearbyPlayerslistView;
	ListView* pListView_all;
	ListView* pListView_shijie;
	ListView* pListView_chengshi;
	ListView* pListView_duiwu;
	ListView* pListView_siliao;
    ListView* pListView_bangpai;

	CheckBox* checkBox;
    Vec2 btnSize;
	KUiPlayerTeam	m_Info;
	KUiPlayerItem*	m_pPlayersList;
	KUiPlayerItem*	m_pNearbyPlayersList;
	int   __nSelIndex;
	int   __playrSelIndex;
	int   __playerListCount;
	int   __NearbyListCount;

	EditBox *pfindEditBox;
	//����ϵͳ
	KChannelResourceInfo m_ChannelsResource[MAX_CHANNELRESOURCE];
	int m_nChannelsResource;
	char m_DefaultChannelSendName[32];
	int m_nDefaultChannelResource;
	KChannelActivateInfo* m_pActivateChannel;
	int m_nActivateChannels;  //�Ѿ������Ƶ���˵�
	unsigned int m_uLastDelMsgTime;
	int	 IsNeverCloseActivateChannel(int nChannelIndex);


	bool ReplaceSpecialField(char* szDest, char* szSrc);

	int	FindActivateChannelResourceIndex(char* cTitle);
	int	FindChannelResourceIndex(char* cTitle);
	int	FindActivateChannelIndexByKey(char* cKey);
	BYTE FindActivateChannelCost(unsigned long nChannelID);
	void SendQueryChannelID(int nChannelResourceIndex);
	void SendChannelSubscribe(int nChannelIndex, bool b);
	int	 AddActivateChannel(const KChannelActivateInfo& Item);	//��������Item��Index
	int	 FindActivateChannelIndexById(unsigned long nChannelID);
	int	 FindActivateChannelIndex(char* cTitle);

	void addchatmsg(char * typeName,char * sendName,char * contentt,ListView*listView,int nKind,int nColor=0,unsigned int nItemDwidx=0,char*scrSendName=NULL,int nPackage=0);

	bool m_bScrolling;
	Point m_lastPoint;
	bool __isCanMove;
};

#endif /* defined(__MyGame__PopLayer__) */
