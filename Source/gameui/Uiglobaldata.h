//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCodec.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef Uiglobaldata_H
#define Uiglobaldata_H
//---------------------------------------------------------------------------
#pragma warning (disable: 4512)
#pragma warning (disable: 4786)
#pragma warning (disable: 4244)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#define INT int
#endif

#define WEATHERID_NOTHING	255
#define	DIR_DOWN		0
#define	DIR_LEFTDOWN	1
#define	DIR_LEFT		2
#define	DIR_LEFTUP		3
#define	DIR_UP			4
#define	DIR_RIGHTUP		5
#define	DIR_RIGHT		6
#define	DIR_RIGHTDOWN	7
//---------------------------------------------------------------------------
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif*/
#define	CLIENT_IP	"FGSQE8WmCkIEMNXIBSlSt0IbCYIbwIHy"
#ifndef ZeroStruct
#define ZeroStruct(buf)					ZeroMemory(&(buf), sizeof(buf))
#endif

#define MAX_MELEEWEAPON_PARTICULARTYPE_NUM 100   //�����������
#define MAX_RANGEWEAPON_PARTICULARTYPE_NUM 100   //�����������

#define  SCHEME_INI_AUTO_SETING "jx50ai/autoset/%u_set.ini"
#define  SCHEME_INI_AUTO_ITEM   "jx50ai/equipName.txt"
#define  SCHEME_INI_AUTO_DAOJU  "jx50ai/itemName.txt"
#define  SCHEME_INI_AUTO_USE    "jx50ai/itemUseName.txt"
#define  SCHEME_INI_AUTO_SKILL  "\\settings\\Ai\\skill.txt"
#define  CONTROL_SCALE 0.5
#define	 LIFE_TIME_DISTANCE_DEFAULT		50
#define	 AUTO_TIME_SAY_DEFAULT          18
#define	 LIFE_TIME_USERITEM_DEFAULT		180


#include "cocos2d.h"
//stdio.h ���û�����ͷ�ļ� �����һЩ���� �������͵�xxx ��˼����û��������͵� ����
#include "engine/KbugInfo.h"
#include "engine/KEngine.h"
#include "engine/KGbktoUtf8.h"
//#include "KProtocol.h"
//#include "KMagicDesc.h"
#include <map>
#include "network/ClientStage.h"
#include "gamescene/LoginDef.h"
/*struct iKNetMsgTargetObject
{
	virtual void	AcceptNetMsg(void* pMsgData) = 0;
};

struct iKNetMsgTargetObject;
typedef void (*fnNetMsgCallbackFunc)(void* pMsgData);
*/
//#define	MAX_MSG_COUNT	1 << (PROTOCOL_MSG_SIZE * 8)
//iKNetMsgTargetObject*	m_MsgTargetObjs[MAX_MSG_COUNT];

//uiѡ������״̬ �ĸ�����
#define	MAX_PLAYER_PER_ACCOUNT	3
#define	SERVER_IP_MASK	"wNx0j0IA3_13KLLx2z_wbhAsAjD0e76C" //������ip
#define	CLIENT_MASK	    "0h706ENyd00gTA_vbiASkuQ02sDKnEHi"  //�ͻ��˱�ʾ
#define CLIENT_VER_FILE "\\Settings\\Ver\\Xfsverjx.ini"

#ifndef WIN32
typedef struct  _SYSTEMTIME
{
	unsigned short wYear;
	unsigned short wMonth;
	unsigned short wDayOfWeek;
	unsigned short wDay;
	unsigned short wHour;
	unsigned short wMinute;
	unsigned short wSecond;
	unsigned short wMilliseconds;
}	SYSTEMTIME;
typedef struct  _FILETIME
{
	unsigned long dwLowDateTime;
	unsigned long dwHighDateTime;
}	FILETIME;
#endif

struct KRoleChiefInfo
{
	char				Name[32];		//����
	unsigned	char	Gender;			//�Ա�	be one of the SSC_CHARACTER_GENDER value
	unsigned	char	Attribute;		//��������
	//	unsigned   short	NativeRevID;
	union
	{ //���ϱ��� һ��ֻ�ܸ�ֵһ��
		unsigned short	NativePlaceId;	//������ID
		short			nLevel;			//�ȼ�
	};
};
enum UI_STEP_STATE
{
	UI_STEP_NONE,
	UI_STEP_STOP,
	UI_STEP_START,
};

enum UI_EQ_KIND
{//��Ϸui�еĸ��ڵ�
	UI_EQ_PART_HEAD = 1, //ͷ10
	UI_EQ_PART_BOBY=6,
	UI_EQ_PART_LEFTHD,
	UI_EQ_PART_RIGTHHD,
	UI_EQ_PART_LEFTHD_W,
	UI_EQ_PART_RIGTHHD_W,
	UI_EQ_PART_HORSE_Q=13,
	UI_EQ_PART_HORSE_Z,
	UI_EQ_PART_HORSE_H,
	UI_EQ_PART_PIFENG=17,
	UI_EQ_PART_NPC,
	UI_EQ_PART_NPC_STATE,
	UI_EQ_PART_SHADOW, //Shadow
	UI_NPC_PART_OTHER,
	UI_EQ_PART_COUNT,
};


enum PAD_KIND
{
	PAD_KIND_ITEM, //����
	PAD_KIND_ROLE,
	PAD_KIND_ITEMEX,
	PAD_KIND_CHUWUXIANG,
	PAD_KIND_TEAM,
};

enum NPC_PART_OTHER_KEY
{
   NPC_NAME, //����
   NPC_LIFE, //������
   NPC_TITLE,//�ƺ�
   NPC_TONG, //�����
   NPC_TIME, //���ʱ��
   NPC_SEX,  //����
   PLAYER_AUTO,  //�һ�
   PLAYER_TAN,   //��̯
   PLAYER_PIFENG,   //PIFENG
   PLAYER_SPRCHENGHAO,   //PIFENG
   PLAYER_TITLE,    //TITLE
   PLAYER_REBORN,
   KEY_COUNT,
};

enum LOGIN_LOGIC_STATUS//���͵���Ϣ
{
	LL_S_IDLE = 0,					//����
	LL_S_WAIT_INPUT_ACCOUNT,		//�ȴ����˺�����
	LL_S_ACCOUNT_CONFIRMING,		//�ȴ��˺�������֤
	LL_S_WAIT_ROLE_LIST,			//�ȴ����ս�ɫ�б�����
	LL_S_ROLE_LIST_READY,			//��ɫ�б�����
	LL_S_CREATING_ROLE,				//�����½���ɫ
	LL_S_WAIT_TO_DEL,				//�ȴ���֤ɾ��
	LL_S_DELETING_ROLE,				//����ɾ����ɫ
	LL_S_WAIT_TO_LOGIN_GAMESERVER,	//�ȴ���½��Ϸ������
	LL_S_ENTERING_GAME,				//���ڽ�����Ϸ
	LL_S_IN_GAME,					//��Ϸ����ʱ
};

enum LOGIN_LOGIC_RESULT_INFO //���ص���Ϣ
{
	LL_R_NOTHING,					//�޽����Ϣ
	LL_R_CONNECT_FAILED,			//����ʧ��
	LL_R_CONNECT_SERV_BUSY,			//������æ
	LL_R_CONNECT_TIMEOUT,			//���ӳ�ʱδ�����Ӧ
	LL_R_ACCOUNT_PWD_ERROR,			//�˺�/�������
	LL_R_ACCOUNT_FREEZE,			//�˺Ŷ���
	LL_R_ACCOUNT_LOCKED,			//�˺ű�����
	LL_R_INVALID_ROLENAME,			//(�½�)��ɫ�����ֲ��Ϸ�
	LL_R_SERVER_SHUTDOWN,			//��Ϸ����������������ά����
	LL_R_INVALID_PROTOCOLVERSION,	//�汾�ŽϾɣ���Ҫ�������µĿͻ���
	LL_R_INVALID_PASSWORD,			//��ɾ����ɫʱ���ṩ���������
	LL_R_ACCOUNT_CONFIRM_SUCCESS,	//�˺���֤�ɹ�
	LL_R_CREATE_ROLE_SUCCESS,		//������ɫ�ɹ�
	LL_R_LOGIN_TO_GAMESERVER,		//��ʼ������Ϸ���������������
	LL_R_ACCOUNT_NOT_ENOUGH_POINT,	//�˺ŵ�������
	LL_R_DENGLUQI_XFSJX,	        //ר�õ�¼��
	LL_R_WAIGUA_XFSJX,	            //��Ҽ��
	LL_R_CHANNEL_ROLE,              //ȴ����ɫ
	LL_R_FORBITOPEN_GAMECLIENT,	    //���ƶ࿪
	LL_R_DIANNAO_GUANLIYUAN,        //����Ա����
	LL_R_OPNE_ACCOUNT,              //���˺�

};

enum BTN_PAGE_AUTO
{
	AUTOBTN_PAGE_BASE =1,
	AUTOBTN_PAGE_ART,
	AUTOBTN_PAGE_ITEM_1,
	AUTOBTN_PAGE_ITEM_2,
	AUTOBTN_PAGE_OTHER,
};

enum LIST_VIEW_TAG
{
	LIST_VIEW_ITEM =1,
	LIST_VIEW_TOOL,
	LIST_VIEW_USE,
	LIST_VIEW_OTHER,
};

enum CHECK_BOX_TAG
{
	BOX_TAG_BASE =1,
	BOX_TAG_ART,
	BOX_TAG_ITEM_1,
	BOX_TAG_ITEM_2,
	BOX_TAG_SHIQU,
	BOX_TAG_SAVE_M,
	BOX_TAG_TEAM_1,
	BOX_TAG_TEAM_2,
	BOX_TAG_TEAM_3,
	BOX_TAG_FENJIE,
	BOX_TAG_YANSHI,
	BOX_TAG_YABIAO,
	BOX_TAG_BAOLIU,
	BOX_TAG_USEITEM,
	BOX_TAG_DING,
	BOX_TAG_LUXIAN,
	BOX_TAG_LUZHI,
	BOX_TAG_GENSHUI,
	BOX_TAG_MIAOSHA,
	BOX_TAG_SOUND,
	BOX_TAG_JULI,
	BOX_TAG_SHADOM,
	BOX_TAG_TITLE,
	BOX_TAG_OTHER,
};
struct KLoginServer
{
	char			Title[32];		//����������˵��
	unsigned char	Address[4];		//������ip��ַ
};

struct	m_Choices
{
	int						nServerRegionIndex;			//��������������
	KLoginServer			AccountServer;				//��ǰʹ�õķ�����
	char					Account[32];				//��ǰ�˺�
	KSG_PASSWORD    		Password;					//��ǰ�˺ŵ�����
	char					szProcessingRoleName[32];	//��ǰ�����Ľ�ɫ������
	bool					bRememberAccount;			//�Ƿ��¼��½�˺�
	bool					bRememberAll;				//�Ƿ��¼ȫ���ĵ�½ѡ��
	bool					bAutoLoginEnable;			//�Ƿ������Զ���½
	bool					bIsRoleNewCreated;			//��ǰ��ɫ�Ƿ�Ϊ�½��Ľ�ɫ
	bool					bLoaded;					//�Ƿ��Ѽ���ѡ���¼
};

extern m_Choices m_ClientChoices;
extern short m_nNumRole;	        //��ɫ����
extern int   nGlobaluistep;
extern KRoleChiefInfo	m_RoleList[MAX_PLAYER_PER_ACCOUNT];

extern int  nPlaceId;               //ѡ��ĳ�����
extern int  nNativePlaceId;         //��ͼ���
extern RECT maxMapRc;
extern bool isthisVer;
extern int  nCuraMapIdx;            //ѡ��ĳ�����

#endif
