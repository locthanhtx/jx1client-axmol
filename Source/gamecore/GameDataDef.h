/*****************************************************************************************
//	外界访问Core用到数据结构等的定义
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-12
------------------------------------------------------------------------------------------
	一些定义可能处于在游戏世界各模块的头文件中，请在此处包含那个头文件，并请那样的头文件
不要包含一些与游戏世界对外无关的内容。
    开发过程中游戏世界的外部客户在未获得游戏世界接口完整定义的情况下，会现先直接在此文件
定义它需要的数据定义，游戏世界各模块可根据自身需要与设计把定义作修改或移动到模块内的对外
头文件，并在此建立充要的包含。
*****************************************************************************************/
#ifndef GAMEDATADEF_H
#define GAMEDATADEF_H

#include "CoreObjGenreDef.h"
#include "CoreUseNameDef.h"
#include "KRUImage.h"
#define		_CHAT_SCRIPT_OPEN                           // 聊天内容过滤开启

#define		MAX_TEAM_MEMBER						7		// 最大队员数量(不包括队长)
#define		MAX_SENTENCE_LENGTH					256		// 聊天每个语句最大长度

#define		FILE_NAME_LENGTH		            256


#define		defMAX_PLAYER_SEND_MOVE_FRAME		5

//#define		PLAYER_PICKUP_CLIENT_DISTANCE		20000
//#define		MAX_INT								0x7fffffff

#define		ROLE_NO								2
#define		PLAYER_MALE_NPCTEMPLATEID			-1
#define		PLAYER_FEMALE_NPCTEMPLATEID			-2

#define		MAX_DEATH_PUNISH_PK_VALUE			10		// PK处罚，PK值从 0 到 10
#define		MAX_REPUTE_VALUE					100000	//声望
#define		MAX_FUYUAN_VALUE					100000	//福源

/*--- 用于标明客户端npc是哪个region的第几个npc ，
如果这是一个服务器控制的npc ，ID 值为 0 ，No 值为 -1  ---*/
struct	KClientNpcID
{
	unsigned long	m_dwRegionID;
	int		m_nNo;
};

typedef struct
{
	int		nIdx;
	int		nPlace;
	int		nX;
	int		nY;
	int		nPrice;      //�̵깺��۸�
	int     nIsRongQi;   //�ĸ�������Ķ���
	BYTE    nLianJieFlg; //�Ƿ���������
	BYTE    nModel;
} PlayerItem;


#define		MAX_HAND_ITEM				1	  //����

#define		EQUIPMENT_ROOM_WIDTH		5
#define		EQUIPMENT_ROOM_HEIGHT		6
#define		MAX_EQUIPMENT_ITEM			(EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT)	   //����

#define		REPOSITORY_ROOM_WIDTH		5
#define		REPOSITORY_ROOM_HEIGHT		6
#define		MAX_REPOSITORY_ITEM			(REPOSITORY_ROOM_WIDTH * REPOSITORY_ROOM_HEIGHT*5)   //�����䣨����3��չ�� 1��ĸ����

#define		TRADE_ROOM_WIDTH			8
#define		TRADE_ROOM_HEIGHT			4
#define		MAX_TRADE_ITEM				(TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT)	           //������
#define		MAX_TRADE1_ITEM				MAX_TRADE_ITEM

#define		IMMEDIACY_ROOM_WIDTH		9     //��ݼ��������� 3
#define		IMMEDIACY_ROOM_HEIGHT		1
#define		MAX_IMMEDIACY_ITEM			(IMMEDIACY_ROOM_WIDTH * IMMEDIACY_ROOM_HEIGHT)	   //�����

#define		DAZAO_ROOM_WIDTH		    14    //���쵥��������
#define		DAZAO_ROOM_HEIGHT		    4
#define	    MAX_DAZAO_ITEM              (DAZAO_ROOM_WIDTH*DAZAO_ROOM_HEIGHT) 	           //������

#define		CAILIAO_ROOM_WIDTH		    2     //���϶��������
#define		CAILIAO_ROOM_HEIGHT		    4
#define		MAX_CAILIAO_ITEM            (CAILIAO_ROOM_WIDTH*CAILIAO_ROOM_HEIGHT*5)         //������DAZAO_ROOM_WIDTH*CAILIAO_ROOM_HEIGHT+

#define		MAX_PLAYER_ITEM_RESERVED	32
#define		MAX_ITEM_BOX                100	 //���������������� ͬ����  ��  ����װ������
#define		MAX_PLAYER_ITEM				(MAX_EQUIPMENT_ITEM + MAX_REPOSITORY_ITEM + MAX_TRADE_ITEM + MAX_TRADE1_ITEM + MAX_IMMEDIACY_ITEM + itempart_num + MAX_HAND_ITEM + MAX_PLAYER_ITEM_RESERVED+MAX_ITEM_BOX+MAX_CAILIAO_ITEM)

// ����ģʽ
//#define		REMOTE_REVIVE_TYPE			0
//#define		LOCAL_REVIVE_TYPE			1



/*
#define		MAX_ARMOR					14
#define		MAX_HELM					14
#define		MAX_RING					1
#define		MAX_BELT					2
#define		MAX_PENDANT					2
#define		MAX_AMULET					2
#define		MAX_CUFF					2
#define		MAX_BOOT					4
#define		MAX_HORSE					6
*/
#define		MAX_NPC_TYPE	1000      //NPC��������
#define		MAX_NPC_LEVEL	300       //NPc�ȼ�����

#define		MAX_NPC_DIR		64        //����
//#define		MAX_WEAPON		MAX_MELEE_WEAPON + MAX_RANGE_WEAPON
#define		MAX_SKILL_STATE 20       //ÿ�����ܵ�����������״̬(���Ϊ20�� ������skill.txt)
//#define		MAX_NPC_HEIGHT	128
#define		MAX_RESIST		150        //2000  //������ֵ
#define		MAX_BASE_RESIST		75     //2000  //������ֵ
#define		BASE_NPC_RESIST_MAX 50     //NPC��������
//#define		MAX_HIT_PERCENT	2000   //������ֵ--����
#define		MIN_HIT_PERCENT	10         //��С����ֵ--����
#define		MAX_NPC_RECORDER_STATE 36  //���ͬ����NPC���µ�״̬����; 20

//#define	PLAYER_MOVE_DO_NOT_MANAGE_DISTANCE	5

#ifndef _SERVER
#define		C_REGION_X(x)	(LOWORD(SubWorld[0].m_Region[ (x) ].m_RegionID))
#define		C_REGION_Y(y)	(HIWORD(SubWorld[0].m_Region[ (y) ].m_RegionID))
#endif
//----------���ÿ�ʼ---------------------------------
#define MAX_MESSAGE_LENGTH          512
#define	ITEM_VERSION				1    //װ���汾
#define	MAX_RANGE_WEAPON			3
#define	MAX_MELEE_WEAPON			6

enum GAME_STR_NOTE
{
	STR_NONE = 0,
	STR_ACTION_SCRIPT,    //�ű�
	STR_NPCTALK_MSG,
	STR_NPCTALK_NAME,
	STR_WARTONG_NAME,	  //ռ����
	STR_WARMASTER_NAME,	  //ռ�����
	STR_GUISHU_NAME,
	STR_CHEGNHAO_NAME,
	STR_SHOP_NAME,       //��̨������
	STR_PICTATH_CLIENT,	 //ͷ��·��
	STR_SCRIPTTATH_CLIENT, //�ͻ��˽ű�·��
	STR_MAP_CURTIME,   //���������
	STR_MAP_NEXTTIME,
	STR_MAP_NAME,
	STR_MAP_TGONGNAME,
	STR_MAP_TSHOUNAME,
	STR_MAP_MASTER,
	STR_MAP_ZTONGNAME,
	STR_ROLE_CHANGENAME, //��¼���������� ���浵
	STR_CHECK_NAME_RET,  //��¼��ѯ���
	STR_ITEM_DESCINFO,   //��Ʒ��˵������
	STR_ITEM_WONNAME,    //��������
	STR_ITEM_NAME,
	//m_WarTongNamea  m_WarMaster	 m_GuishuName PicPath	  m_ScriptPicPath  m_SubWorldName
};
enum GAME_STATE_NOTE
{
	    STATE_NOTE_NONE = 0,
	    STATE_NOTE_POISON,      //��
		STATE_NOTE_FREEZE,      //��
		STATE_NOTE_BURN,	    //ȼ��
		STATE_NOTE_CONFUSE,     //����
		STATE_NOTE_STUN,        //ѣ��
		STATE_NOTE_LIFE,        //BU XUE
		STATE_NOTE_MANA,        //BU LAN
		STATE_NOTE_MEUN,	    //�˵�
		STATE_NOTE_DRUNK,
		STATE_NOTE_HIDE,	    //����״̬
		STATE_NOTE_ZHUA,	    //ץ��״̬
		STATE_NOTE_LOSE,
		STATE_NOTE_EXP,	        //����״̬
		STATE_NOTE_DOSCRIPT,    //ִ�нű�״̬
		STATE_NOTE_RANDMOVE,	//Ⱥ������ƶ�״̬
		STATE_NOTE_MAPUSEMODEL, //��ͼ ʹ������״̬
		STATE_NOTE_PHYSICSARMOR,
		STATE_NOTE_COLDARMOR,
		STATE_NOTE_LIGHTARMOR,
		STATE_NOTE_FIREARMOR,
		STATE_NOTE_MANASHIELD,	//��͵��״̬
		STATE_NOTE_RETURNSKILL,	//�����˹����� �Զ��ͷ�ĳ���ܼ���
		STATE_NOTE_DEATHSKILL,	//�����˹����� �Զ��ͷ�ĳ���ܼ���
		STATE_NOTE_RESCUESKILL,	//�����Զ��ͷż���
		STATE_NOTE_REPLYSKILL,	////�Զ���������
};

enum ITEM_PART
{
	itempart_head = 0,	// ͷ
		itempart_body,		// ����
		itempart_belt,		// ����
		itempart_weapon,	// ����
		itempart_foot,	    //ѥ��
		itempart_cuff,		//�������
		itempart_amulet,    //����
		itempart_ring1,	    //����
		itempart_ring2,		//����
		itempart_pendant,	//���һ�����
		itempart_horse,	    //��
		itempart_mask,	    //���
		itempart_pifeng,
		itempart_yinjian,
		itempart_shiping,
		itempart_num,
};

enum MONEYUNIT
{
	    moneyunit_money = 0,//����
		moneyunit_moneya, //����
		moneyunit_xu,	  //���
		moneyunit_fuyuan, //��Դ
		moneyunit_repute, //����
		moneyunit_expoint,//�㿨
		moneyunit_jifen,  //����
		moneyunit_num,
};

enum INVENTORY_ROOM
{
	room_equipment = 0,	// װ����-����
		room_repository,	// ������
		room_exbox1,		// ��չ��1
		room_exbox2,		// ��չ��2
		room_exbox3,		// ��չ��3
		room_equipmentex,	// ͬ�����
		room_trade,			// ������
		room_tradeback,		// ���׹�����װ�����ı���
		room_trade1,		// ���׹����жԷ��Ľ�����
		room_immediacy,		// �����Ʒ
		room_give,          // ����ռ�
		room_giveback,
		room_dazao,         // �����
		room_cailiao,       // ���Ͽ�
		room_num,			// �ռ�����
};

enum ITEM_POSITION
{
	pos_hand = 1,		// ����
		pos_equip,			// װ���ŵ�
		pos_equiproom,		// ������  ����
		pos_repositoryroom,	// ������
		pos_exbox1room,		// mo rong ruong 1  ��չ��
		pos_exbox2room,		// mo rong ruong 2
		pos_exbox3room,		// mo rong ruong 3
		pos_equiproomex,	// ͬ�����
		pos_traderoom,		// ������
		pos_trade1,			// ���׹����жԷ��Ľ�����
		pos_immediacy,		// �������Ʒ
		pos_give,           // �������
		pos_dazao,          // ��������
		pos_cailiao,        // ������
		pos_xiangqi,        // ������
};
//������ݵ�λ��һЩ�ؼ�������
enum PLAYER_BRIEF_PROP
{
	PBP_LEVEL = 1,	//�Ǽ��仯	nParam��ʾ��ǰ�ȼ�
		PBP_FACTION,	//����		nParam��ʾ�������ԣ����nParamΪ-1��ʾû������
		PBP_CLIQUE,		//����		nParamΪ��0ֵ��ʾ���˰��ɣ�0ֵ��ʾ�����˰���
};

//������Ϣ�����Ͷ���
enum NEWS_MESSAGE_TYPE
{
	NEWSMESSAGE_NORMAL,			//һ����Ϣ����ʾ��һ�Σ�����Ϣ������
								//��ʱ�����
	NEWSMESSAGE_COUNTING,		//���ƣ��룩����Ϣ��������0ʱ������Ϣ�������ˡ�
								//ʱ������е����ݽṹ�н���������Ч������������Ϊ��λ��
	NEWSMESSAGE_TIMEEND,		//��ʱ��Ϣ����ʱ��ʱ����Ϣ�������ˣ�����ÿ�������ʾһ�Ρ�

	NEWSMESSAGE_SHUIJI,         //���������Ϣ  ��ʾһ�ξ�����
								//ʱ�������ʾ������ָ��ʱ�䡣
};

enum	// �������
{
	Obj_Kind_MapObj = 0,		    // ��ͼ�������Ҫ���ڵ�ͼ����
		Obj_Kind_Body,				// npc ��ʬ��
		Obj_Kind_Box,				// ����
		Obj_Kind_Item,				// ���ڵ��ϵ���Ʒ
		Obj_Kind_Money,				// ���ڵ��ϵ�Ǯ
		Obj_Kind_LoopSound,			// ѭ����Ч
		Obj_Kind_RandSound,			// �����Ч
		Obj_Kind_Light,				// ��Դ��3Dģʽ�з���Ķ�����
		Obj_Kind_Door,				// ����
		Obj_Kind_Trap,				// ����
		Obj_Kind_Prop,				// С���ߣ�������
		Obj_Kind_Num,				// �����������
};

struct KMapPos
{
	int		nSubWorld;
	int		nRegion;
	int		nMapX;
	int		nMapY;
	int		nOffX;
	int		nOffY;
};

// ע�⣺��ö�ٲ��������(by zroc)
enum OBJ_ATTRIBYTE_TYPE
{
	    series_metal = 0,		//	��ϵ
		series_wood,			//	ľϵ
		series_water,			//	ˮϵ
		series_fire,			//	��ϵ
		series_earth,			//	��ϵ
		series_num,
};

enum NPCCAMP
{
	    camp_begin,				// ������Ӫ����������ǰ����ң���
		camp_justice,			// ������Ӫ  1	 ��ɫ
		camp_evil,				// а����Ӫ  2   ǳ��ɫ
		camp_balance,			// ������Ӫ  3	 ǳ��ɫ
		camp_free,				// ɱ����Ӫ����ʦ�����ң�4  ��ɫ
		camp_animal,			// Ұ����Ӫ   5	 ����ɫ
		camp_event,				// ·����Ӫ   6	 ����ɫ
		camp_blue,				// ��
		camp_green,				// ��
		camp_num,				// ��Ӫ��
};
enum NPC_RELATION
{
	    relation_none	= 1,
		relation_self	= 2,   // �Լ�
		relation_ally	= 4,   // ͬ��
		relation_enemy	= 8,   // ����
		relation_dialog	= 16,  // �����Ի�
		relation_all	= relation_none | relation_ally | relation_enemy | relation_self | relation_dialog,
		relation_num,
};
enum NPCKIND
{
	    kind_normal = 0, //����
		kind_player,     //���
		kind_partner,    //ͬ��
		kind_dialoger,	 //�Ի���
		kind_bird,       //���Ϸɵ�
		kind_mouse,      //�����ߵ�
		kind_num
};

enum COLORKIND
{
	kind_color_physics = 0, //��ɫ
	kind_color_freeze,     //��
	kind_color_stun,       //��
	kind_color_poison,	   //��
	kind_color_burn,       //ȼ��
	kind_color_confuse,    //����
	kind_color_num
};

//==================================
//	ħ������
//==================================
#ifndef MAGICATTRIB
#define MAGICATTRIB
struct KMagicAttrib
{
	int				nAttribType;					//��������
	int				nValue[3];						//���Բ���
	KMagicAttrib(){nValue[0] = nValue[1] = nValue[2] = nAttribType = 0;};
};
#else
struct KMagicAttrib;
#endif

#define MAX_MISSION_PARAM			10
#define MAX_GLBMISSION_PARAM		3
#define MISSION_STATNUM				10
#define MISSION_PARAM_AVAILABLE		0
#define MISSION_AVAILABLE_VALUE		1

#define	defTONG_NAME_MAX_LENGTH		12//8
#define	defTONG_STR_LENGTH		    32



//-----------------------------
struct TMissionLadderSelfInfo
{
	char		  szMissionName[32];	 //64
	unsigned char ucGroup;				 // 2
	int			  nGlbParam[MAX_GLBMISSION_PARAM]; //6
	int			  nParam[MAX_MISSION_PARAM];   // 32
	void Clear() {
		memset(szMissionName, 0, sizeof(szMissionName));
		memset(nGlbParam, 0, sizeof(nGlbParam));
		memset(nParam, 0, sizeof(nParam));
	};
};

struct TMissionLadderInfo
{
	unsigned char ucGroup;	 //2
	char		  Name[32];		//32
	int			  nParam[MAX_MISSION_PARAM]; //32
	/*void Clear() {
		ucGroup = 0;
		memset(Name, 0, sizeof(Name));
		memset(nParam, 0, sizeof(nParam));
	};*/
};
typedef struct BuySellInfo
{

	int		m_nBuyIdx;
	unsigned long	m_SubWorldID;
	int		m_nMpsX;
	int		m_nMpsY;
	//int		m_nShopIdx[MAX_SUPERSHOP_SHOPTAB];
	//int		m_nShopNum;
	int		m_nPriceType;

	void	Clear() {
		//memset(m_nShopIdx, -1, sizeof(m_nShopIdx));
		m_nBuyIdx = -1 ; m_nPriceType = moneyunit_money; m_SubWorldID = -1; m_nMpsX = 0; m_nMpsY = 0;
	}
} ;

struct KUiMsgParam
{
	unsigned char	eGenre;	//ȡֵ��ΧΪö������MSG_GENRE_LIST,��MsgGenreDef.h�ļ�
	unsigned char	cChatPrefixLen;
	unsigned short	nMsgLength;
	char			szName[32];  //�����ߵ�����
#define	CHAT_MSG_PREFIX_MAX_LEN	16
	unsigned char	cChatPrefix[CHAT_MSG_PREFIX_MAX_LEN];
};
struct KTime
{
	int bYear;
	int bMonth;
	int bDay;
	int bHour;
	int bMin;
};
struct KviewTime
{
	int bYear;
	BYTE bMonth;
	BYTE bDay;
	BYTE bHour;
	BYTE bMin;
};


//---------------------------- ������ ------------------------

#define		defTONG_MAX_DIRECTOR				7   //������
#define		defTONG_MAX_MANAGER					7   //�ӳ���
#define		defTONG_MAX_LENMENG					10
#define		defTONG_ONE_PAGE_MAX_NUM			7   //ÿ��Ҫ���ȡͬ�������
enum TONG_MEMBER_FIGURE
{
	enumTONG_FIGURE_MEMBER,				// ����
		enumTONG_FIGURE_MANAGER,			// �ӳ�
		enumTONG_FIGURE_DIRECTOR,			// ����
		enumTONG_FIGURE_MASTER,				// ����
		enumTONG_FIGURE_LENMENG,            // ����
		enumTONG_FIGURE_LIST,               // �б�
		enumTONG_FIGURE_NUM,
};

enum
{
	enumTONG_APPLY_INFO_ID_SELF,		// �����ѯ������Ϣ
		enumTONG_APPLY_INFO_ID_MASTER,		// �����ѯ������Ϣ
		enumTONG_APPLY_INFO_ID_DIRECTOR,	// �����ѯ������Ϣ
		enumTONG_APPLY_INFO_ID_MANAGER,		// �����ѯ�ӳ���Ϣ
		enumTONG_APPLY_INFO_ID_MEMBER,		// �����ѯ������Ϣ(һ������)
		enumTONG_APPLY_INFO_ID_ONE,			// �����ѯĳ����Ա��Ϣ(һ������)
		enumTONG_APPLY_INFO_ID_TONG_HEAD,	// �����ѯĳ�����Ϣ���������������
		enumTONG_APPLY_INFO_ID_LIST,        // �����ȡ����б�
		enumTONG_APPLY_INFO_ID_NUM,
};

enum
{
	enumTONG_CREATE_ERROR_ID1,	// Player[m_nPlayerIndex].m_nIndex <= 0
		enumTONG_CREATE_ERROR_ID2,	// ���׹�����
		enumTONG_CREATE_ERROR_ID3,	// ���������
		enumTONG_CREATE_ERROR_ID4,	// �����Ӫ����
		enumTONG_CREATE_ERROR_ID5,	// �Ѿ��ǰ���Ա
		enumTONG_CREATE_ERROR_ID6,	// �Լ�����Ӫ��
		enumTONG_CREATE_ERROR_ID8,	// Ǯ������
		enumTONG_CREATE_ERROR_ID7,	// �ȼ�����
		enumTONG_CREATE_ERROR_ID9,	// ��Ӳ��ܽ����
		enumTONG_CREATE_ERROR_ID10,	// ���ģ�����
		enumTONG_CREATE_ERROR_ID11,	// �����ַ�������
		enumTONG_CREATE_ERROR_ID12,	// �����ַ�������
		enumTONG_CREATE_ERROR_ID13,	// ���ͬ������
		enumTONG_CREATE_ERROR_ID14,	// ������ʧ��
};

//-------- ���ý���------------------------------------------
//---------���Է������˿�ʼ----------------------------------
#ifdef _SERVER
#define	    MAX_SUPERSHOP_SHOPTAB 10
#define		PLAYER_PICKUP_SERVER_DISTANCE		40000
#define		PLAYER_SHARE_EXP_DISTANCE			768
#define		defMAX_EXEC_OBJ_SCRIPT_DISTANCE		200     //��Ʒִ�нű�����
#define		MAX_ITEMLIST_CONUT		            250	    //��󴢴����Ʒ����


enum ITEM_PARM_DROP
{
	    ITEM_PARM_COUNT,
		ITEM_PARM_RATESUM,
		ITEM_PARM_LUCKRATESUM,
		ITEM_PARM_MAGRATE,
		ITEM_PARM_MAXRATE,
		ITEM_PARM_MONEYRATE,
		ITEM_PARM_MONEYNUM,
		ITEM_PARM_MONEYSCALE,
	    ITEM_PARM_MINLEVELSCALE,
		ITEM_PARM_MAXLEVELSCALE,
		ITEM_PARM_MINLEVEL,
		ITEM_PARM_MAXLEVEL,
		ITEM_PARM_TYPENUM,
		ITEM_PARM_BIANSHI,

		/*
		  INT	        m_nRandSum;		// ������֮�ͣ�100w��֮һ	���������������ֵ���򲻻���䣩
		  int         nLuckRateSum;
		  int			nMagicRate;
		  int			nMaxRandRate;
		  int			nMoneyRate;
		  int         nMoneyNum;
		  int			nMoneyScale;
		  int			nMinItemLevelScale;   //��С�ȼ�����
		  int			nMaxItemLevelScale;	  //
		  int		    nMinItemLevel;
		  int		    nMaxItemLevel;
		  int         nTypeNum;             //���µ�����
		  int         nIsBianShi;           //�Ƿ���Ա� �ɱ�ʶװ��
		*/
};


#endif
//���Է������˽���-------------------------------------------

//-------���Կͻ��˿�ʼ-------------------------------------
#ifndef _SERVER

#define	NORMAL_NPC_PART_NO		6		           // ��ͨnpcͼ��Ĭ��Ϊֻ��һ�����������ǵڼ���
#define	PLAYER_PICKUP_CLIENT_DISTANCE		63     //�����ľ���

struct ItemCounmd
{
	int Uid[16];
	int nMun;
};
enum
{
	CHAT_S_STOP = 0,						// ������״̬
	CHAT_S_SCREEN,							// ��ͬ��Ļ�������
	CHAT_S_SINGLE,							// ��ͬ������ĳ���˽��
	CHAT_S_TEAM,							// �����ȫ���Ա��̸
	CHAT_S_NUM,								// ����״̬������
};

enum PLAYER_INSTANT_STATE
{
	enumINSTANT_STATE_LEVELUP = 0,
	enumINSTANT_STATE_REVIVE,
	enumINSTANT_STATE_CREATE_TEAM,
	enumINSTANT_STATE_LOGIN,
    enumINSTANT_STATE_DANGGE,
	enumINSTANT_STATE_NUM,
};

enum CHAT_STATUS
{
	CHAT_S_ONLINE = 0,		//����
	CHAT_S_BUSY,			//æµ
	CHAT_S_HIDE,			//����
	CHAT_S_LEAVE,			//�뿪
	CHAT_S_DISCONNECT,		//����
};



enum OBJ_GENDER
{
	OBJ_G_MALE	= 0,	//���ԣ��е�
	OBJ_G_FEMALE,		//�Ƶģ�Ů��
};


enum ITEM_IN_ENVIRO_PROP
{
	IIEP_NORMAL = 0,	//һ��/����/����
	IIEP_NOT_USEABLE,	//������/����װ��
	IIEP_SPECIAL,		//�ض��Ĳ�ͬ���
};

#define	GOD_MAX_OBJ_TITLE_LEN 1024	//128��ʱ��Ϊ1024Ϊ�˼��ݾɴ��� to be modified
#define	GOD_MAX_OBJ_PROP_LEN 512
#define	GOD_MAX_OBJ_DESC_LEN 512

//==================================
//	��Ϸ���������
//==================================
struct KGameObjDesc
{
	char szTitle[GOD_MAX_OBJ_TITLE_LEN];	//���⣬����
	char	 szProp[GOD_MAX_OBJ_PROP_LEN];	//���ԣ�ÿ�п���tab����Ϊ�����뿿�Ҷ���������
	char	 szDesc[GOD_MAX_OBJ_DESC_LEN];	//����
};


//==================================
//	�������ѡ��--ѡ��
//==================================

struct KUiAnswer
{
	char  AnswerText[64];	//��ѡ�����֣����԰������Ʒ���
	int		AnswerLen;		//��ѡ�𰸴洢���ȣ��������Ʒ�����������������
};
struct KUiQuestionAndAnswer
{
	char		Question[512];	//�������֣����԰������Ʒ��� //����Ϣ
	int			QuestionLen;	//�������ִ洢���ȣ��������Ʒ�����������������
	int			AnswerCount;	//��ѡ�𰸵���Ŀ
	KUiAnswer	Answer[1];		//��ѡ��
};

struct KUiNpcSpr
{
    char                ImageFile[128];
    unsigned short         MaxFrame;
};

//==================================
//	��Ʒ������Ϣ
//	Added by Fanghao_Wu 2004.8.30
//==================================
struct KUiItemNameInfo
{
	char	szItemName[64];			//��Ʒ����
	unsigned long	dwItemNameColor;		//��Ʒ������ɫ
};
//==================================
//	������Ϸ�������ɵĵط�
//==================================
enum UIOBJECT_CONTAINER
{
	UOC_IN_HAND	= 1,		//��������
	UOC_GAMESPACE,			//��Ϸ����
	UOC_IMMEDIA_ITEM,		//�����Ʒ
	UOC_IMMEDIA_SKILL,		//����书0->�Ҽ��书��1,2...-> F1,F2...����书
	UOC_ITEM_TAKE_WITH,		//����Я��
	UOC_TO_BE_TRADE,		//Ҫ�����������������
	UOC_OTHER_TO_BE_TRADE,	//��������ϣ�����Ҫ�����Լ��ģ�
	UOC_EQUIPTMENT,			//����װ��
	UOC_NPC_SHOP,			//npc��������
	UOC_MARKET,             //ϵͳ�̳�
	UOC_STORE_BOX,			//������
	UOC_EX_BOX1,            //��չ��1
	UOC_EX_BOX2,
	UOC_EX_BOX3,
	UOC_ITEM_EX,            //ͬ�����
	UOC_SKILL_LIST,			//�г�ȫ��ӵ�м��ܵĴ��ڣ����ܴ���
	UOC_SKILL_TREE,			//���ҿ��ü�����
	UOC_ITEM_GIVE,          //����ռ�
    UOC_ITEM_DAZAO,         //�趨�������Ϊ������
	UOC_ITEM_CAILIAO,       //�趨�������Ϊ������
	UOC_CHESSGAME,			//����
	UOC_NPC_SHOP_PAI,       //������

};

//==================================
// iCoreShell::GetGameData��������,uDataIdȡֵΪGDI_TRADE_DATAʱ��
// uParam�����ȡֵ�б�
// ע���е�Return:�б�ʾ��ص�GetGameData���õķ���ֵ�ĺ���
//==================================
enum UI_TRADE_OPER_DATA
{
	UTOD_IS_WILLING,		//�Ƿ�������(������)
	//Return: �����Լ��Ƿ��ڽ����еĲ���ֵ
	UTOD_IS_LOCKED,			//�Լ��Ƿ���������״̬
	//Return: �����Լ��Ƿ���������״̬�Ĳ���ֵ
	UTOD_IS_TRADING,		//�Ƿ�������ڵȴ����ײ����������Ƿ���ȷ����
	//Return: �����Ƿ����ڵȴ����ײ����������Ƿ���ȷ����
	UTOD_IS_OTHER_LOCKED,	//�Է��Ƿ��Ѿ���������״̬
	//Return: ���ضԷ��Ƿ��Ѿ���������״̬�Ĳ���ֵ
};

//==================================
//	������Ʒ
//==================================
struct KUiItemBuySelInfo
{
	char			szItemName[64];	//��Ʒ����
	int				nPrice;			//����������Ǯ����ֵΪ���۸񣬸�ֵ��ʾ����ļ۸�Ϊ(-nPrice)
	int				nPriceXu;       //���
	BYTE            nPriceType;     //���ҵ�����

	int             nOldPrice;
	int             nOldPriceXu;
	int             nCurPriceXu;
	BOOL            bNewArrival;
//	int             nMoneyUnit;

};

//==================================
//	һЩͨ�ýṹ
//==================================
struct KUiTongYong
{
	int inKind;
	int inParma;
	int inParmb;
	int inParmc;
	int inParmd;
};
/*
struct	nJinDuClient
{
	int  nTimeVal;
	int	 nMsgidx;
	char szCallFun[32];   //�ص�����
	char szParma[32];     //����
	char szParmb[32];     //����
	char szParmc[32];     //����
};
*/
struct KUiScrptinfo
{
	struct	KTaskClient
	{
		char szContent[128];	//����
		int	 nTaskidx;			//������Ǯ����ֵΪ���۸񣬸�ֵ��ʾ����ļ۸�Ϊ(-nPrice)
		int	 nTaskType;
		char  nTaskName[32];
	};
	KTaskClient nTaskInfo[20];
};
//==================================
//	���Ա�ʾ��Ϸ����Ľṹ
//==================================
struct KUiGameObject
{
	unsigned int uGenre;	//��������
	unsigned int uId;		//����id
	//	int			 nData;		//�����ʵ����ص�ĳ����
};

//==================================
//	�������ʾ��һ������Χ
//==================================
struct KUiRegion
{
	int		h;		//���Ͻ���������
	int		v;		//���Ͻ����������
	int		Width;	//������
	int		Height;	//�����ݿ�
	int     IsRongqi; //���ĸ����������� �������� �� �����������
};

struct KUiItemdObject
{
	unsigned int	uGenre;
	unsigned int	uId;
	unsigned int    nindex;
	int				DataX;
	int				DataY;
	int				DataW;   //���
	int				DataH;   //�߶�
	int             nGenkind;//���ڵ���������
	Rect          nRect;
};

struct KUiDraggedObject
{
	unsigned int	uGenre;
	unsigned int	uId;
	int				DataX;
	int				DataY;
	int				DataW;  //���
	int				DataH;  //�߶�
	//    char            uName[32];
	int m_posvs;
	unsigned char m_dirvs,m_vs_lastframe;
	int bCheckpVS;
	KRUPoint* pVS;
	int* ndir;
	unsigned int nDelay;
	KUiDraggedObject()
	{
		bCheckpVS = 0; //FALSE
		nDelay = 0;
		m_posvs=0;
		m_dirvs=0;
		m_vs_lastframe=0;
	};
};
//==================================
//	��ʾĳ����Ϸ��������������Χ����Ϣ�ṹ
//==================================
struct KUiObjAtRegion
{
	KUiGameObject	Obj;
	KUiRegion		Region;
};

struct KUiObjAtContRegion : public KUiObjAtRegion
{
	union
	{
		UIOBJECT_CONTAINER	eContainer;
		int					nContainer;
	};
};
//========��������===============
struct KUiJinMaiDescz

{
	struct KUiDesc
	{
		int ncZhenYuan;
		int ncHuMaiDan;
		int ncRaTe;
		int ncMgcid;
		int ncBinjia;
		char ncMiaoShu[215];
		char nXueNane[32];

	};
	KUiDesc  nJinMaiDesc[17];
};

//========�ϻ���===============
struct KUiLaoHuJi
{
	int nIsOpen;
	int nType;
	int nYinL;
	int nJinBi;
	int nBeiLv;
	int nDianShu;
};

//========����===============
struct KUiJinMaiLevel
{
	struct KUiLevel
	{
		int  nLeVel[18];
	};
  KUiLevel  nJinMai[10];
  int nZhenYuan;
  int nBinJia;
};
//========����===============
struct KUiJinMaiType
{
	int  nType;
	int  nLevel;
	//	KUiJinMaiLevel  nJinMai[10];
};



//========����===============
struct KUiJinMaiInfo
{
	int  nZhenYuan;
	int  nBingJia;
//	KUiJinMaiLevel  nJinMai[10];
};

//========��ħ===============
struct KUiFuMoInfo
{
	int  nNpcSkill[5];
	BYTE nSkillLevel[5];
	int  nNpcGongji[5];
	int  nNpcFangyu[5];
	int  nNpclife[5];
	int  nNpcGongsu[5];
	int  nNpcXingYun[5];

};

//========��ս��Ϣ===============
struct KUiAttackInfo
{
	int         m_AttackState;
	int         m_AttackTime;
	int         m_nDeathCount;	   //�����Ĵ���
	int         m_nDDeathCount;	   //�����Ĵ���\
	int         m_nAttackNum;	                            // ��ս����
	int         m_nWinNum;								// ʤ������
	int         m_nLoseNum;
	char        m_AttackName[32];  //��ս�İ��
};

/*typedef struct
{
	int		m_nBuyIdx;     //����ID
	unsigned long	m_SubWorldID;
	int		m_nMpsX;
	int		m_nMpsY;
	void	Clear() {m_nBuyIdx = -1; m_SubWorldID = -1; m_nMpsX = 0; m_nMpsY = 0;}
} NewBuySellInfo;*/


//========ͬ��===============
struct KUiTongBanbaseInfo
{
int  nmLife;
int  nmLifeFull;
int  nmMana;
int  nmManaFull;
};
//========ͬ��===============
struct KUiTongBanInfo
{
	int   nDwID;
	char  nTname[32];
	char  nGuiShuName[32];
	int   nCount;
};
//========ת�̳齱===============
struct KUiZhuanpanInfo
{
int  nIsVip;
int  nCJcishu;
char nPlayerName[32];
};
//========����===============
struct KUiGuZiInfo
{
	int  nIsVip;
	int  nCJcishuA;   //����
	int  nCJcishuB;   //����
	int  nCJcishuC;   //����
	int  nCJcishuD;   //����
	int  nCJcishuE;   //����
	char nPlayerNameA[32];
	char nPlayerNameB[32];
	char nPlayerNameC[32];
	char nPlayerNameD[32];
	char nPlayerNameE[32];
	int  nIsShuai;
	int  nGyajinA;  //Ѻ��
	int  nGyajinB;  //Ѻ��
	int  nGyajinC;  //Ѻ��
	int  nGyajinD;  //Ѻ��
	int  nGyajinE;  //Ѻ��
	int  nIsStartA;  //��ʼ�ı�ʾ
	int  nIsStartB;  //��ʼ�ı�ʾ
	int  nIsStartC;  //��ʼ�ı�ʾ
	int  nIsStartD;  //��ʼ�ı�ʾ
	int  nIsStartE;  //��ʼ�ı�ʾ
	int  nZuoweiHao;  //��λ��
	int  nIsMenber;
	int  nmodel;   //ģʽ
	int  nSubZuoWeiHao; //��ͼ����λ ��˭˦����
	int  nIsXiaZhuA;//�Ƿ�������
	int  nIsXiaZhuB;
	int  nIsXiaZhuC;
	int  nIsXiaZhuD;
	int  nIsXiaZhuE;
	int  nCurFarmA;  //��ǰ֡
	int  nCurFarmB;  //��ǰ֡

};

struct KUiItemLian
{
	int nItemDWID;
	char nPlayerName[32];
};

struct KUiGuZiXinxi
{
int nMapID;
int nDianshu;
int nIsStartID;

};
struct KUiGuZiJieSuan
{
	int nMaxDian;
	int nKFmoney;
	int nGRdian[5];
	int nGRjinbi[5];
	char nPlayerNameA[32];
	char nPlayerNameB[32];
	char nPlayerNameC[32];
	char nPlayerNameD[32];
	char nPlayerNameE[32];
};
struct KUiGuZiCurFarms
{
	int nCurFarmsA;
	int nCurFarmsB;
	int nMapID;
};
//================================

struct PLAYER_GIVEUI
{
	BYTE	ProtocolType;
	char	m_szName[256];
	BYTE	m_nNum;
    char    m_Callback[64];
	char    m_Error[16];
	//char    m_Content[125];
	unsigned int uId[24];
    char 	m_szTitle[64];
};

struct PLAYER_JISHOU
{
	BYTE	ProtocolType;
	char    m_szName[256];
	BYTE	m_nNum;
    char    m_Callback[16];
	char    m_Error[16];
	char    m_Content[125];
	unsigned int uId[32];
	char 	m_szTitle[125];
};

struct PLAYER_STRINGUI
{
	BYTE	ProtocolType;
	char    m_szTitle[64];
	BYTE	m_nNum;
    char    m_SCallback[64];
	char    m_SError[16];
	char    m_Content[125];  //����
};

struct KUiInformationParam
{
	char	sInformation[256];	//��Ϣ��������
	char	sConfirmText[64];	//ȷ����Ϣ(��ť)�ı�������
	short	nInforLen;			//��Ϣ�������ݵĴ洢����
	bool	bNeedConfirmNotify;	//�Ƿ�Ҫ����ȷ����Ϣ(��core)
	bool	bReserved;			//������ֵ�̶�Ϊ0
};
//��Ϊ�嵥


enum PLAYER_ACTION_LIST
{
	PA_NONE = 0,	//�޶���
	PA_RUN  = 0x01,	//��
	PA_SIT  = 0x02,	//����
	PA_RIDE = 0x04,	//���
	EX_BOX,			// mo rong ruong 1
	EX_BOX2,		// mo rong ruong 2
	EX_BOX3,		// mo rong ruong 3
	ITEMEX,			// hanh trang
	HT_CN,			// chuc nang dang hoan thien
	OPEN_MARKET,	// ky tran cac
	PW_NOT_SAME,	// mat khau ko giong nhau
	PW_ACCEPTED,	// mat khau dc chap nhan
	PW_NOT_LONG,	// mat khau ko du do dai
	CN_GH,			// chuc nang gioi han
	KD_VP,
	MO_AUTO,
	TAT_AUTO,
	ERORR_AUTO,// ����
	CESHI_AUTO,//����
    OPEN_JISHOU,//����
    HAVE_SKILL,
	GET_BOX_OPEN,
	GET_BOXEX_OPEN,
};

//==================================
//	ϵͳ��Ϣ����
//==================================
enum SYS_MESSAGE_TYPE
{
	SMT_NORMAL = 0,	//���μӷ������Ϣ
	SMT_SYSTEM,		//ϵͳ���������
	SMT_PLAYER,		//������
	SMT_TEAM,		//������
	SMT_FRIEND,		//����������
	SMT_MISSION,	//�������
	SMT_CLIQUE,		//�������
};

//==================================
//	ϵͳ��Ϣ��Ӧ��ʽ
//==================================
enum SYS_MESSAGE_CONFIRM_TYPE
{
	SMCT_NONE,				//�ڶԻ���Ϣ����ֱ���ӹ�������Ҫ��Ӧ��
	SMCT_CLICK,				//���ͼ�������ɾ����
	SMCT_MSG_BOX,			//���ͼ��󵯳���Ϣ��
	SMCT_UI_RENASCENCE,		//ѡ������
	SMCT_UI_ATTRIBUTE,		//������ҳ��
	SMCT_UI_SKILLS,			//�򿪼���ҳ��
	SMCT_UI_ATTRIBUTE_SKILLS,//������ҳ�漼��ҳ��
	SMCT_UI_TEAM_INVITE,	//��Ӧ��ܾ�������������,
	SMCT_UI_TEAM_APPLY,		//��Ӧ��ܾ�������������,
	SMCT_UI_TEAM,			//�򿪶���������
	SMCT_UI_INTERVIEW,		//������Ի�����,
	SMCT_UI_FRIEND_INVITE,	//��׼��ܾ����˼��Լ�Ϊ����
	SMCT_UI_TRADE,			//��Ӧ��ܾ����׵�����,
	SMCT_DISCONNECT,		//����
	SMCT_UI_TONG_JOIN_APPLY,//��Ӧ��ܾ������������
	SMCT_UI_WAIGUA_FORBIT,  //�����ʾ
	SMCT_UI_LOCK_FORBIT,    //�˺Ŷ���
};

//==================================
//	ϵͳ��Ϣ
//==================================
struct KSystemMessage
{
	char			szMessage[256];	//��Ϣ�ı�
	unsigned int	uReservedForUi;	//����ʹ�õ�������,core����0����
	unsigned char	eType;			//��Ϣ����ȡֵ����ö������ SYS_MESSAGE_TYPE
	unsigned char	byConfirmType;	//��Ӧ����
	unsigned char	byPriority;		//���ȼ�,��ֵԽ�󣬱�ʾ���ȼ�Խ��
	unsigned char	byParamSize;	//����GDCNI_SYSTEM_MESSAGE��Ϣ��pParamBuf��ָ�����������ռ�Ĵ�С��
    int		        nMsgLen;
};

//==================================
//	����Ƶ��������
//==================================
struct KUiChatChannel
{
	int			 nChannelNo;
	unsigned int uChannelId;
	union
	{
		int		 nChannelIndex;
		int		 nIsSubscibed;	//�Ƿ񱻶���
	};
	char		 cTitle[32];
};

//==================================
//	������ѵ�һ���������Ϣ
//==================================
struct KUiChatGroupInfo
{
	char	    szTitle[32];	//���������
	int		nNumFriend;		//���ں��ѵ���Ŀ
};

//==================================
//	���ѷ��������컰��
//==================================
struct KUiChatMessage
{
	unsigned int uColor;
	short	nContentLen;
	char    szContent[256];
};

//==================================
//	��ȡ�����ֵ
//==================================

struct KUiBaseTask
{
	int nVala;
	int nValb;
};

//==================================
//	Զ�̿���
//==================================

struct KUiKongZhi
{
	char nRoleName[32];
	char nMiYu[32];
};

//==================================
//	���ǵ�һЩ���ױ������
//==================================
struct KUiPlayerBaseInfo
{
	char	    Agname[32];	//�º�
	char	    Name[32];	//����
	char    Title[32];	//�ƺ�
	int		nCurFaction;// Hi?n t?i ?ang gia nh?p m?n ph��i id N?u l�� -1 th�� hi?n t?i kh?ng c�� trong m?n ph��i.
	int		nRankInWorld;//Gi�� tr? x?p h?ng Jianghu, gi�� tr? 0 c�� ngh?a l�� kh?ng c�� trong b?ng x?p h?ng
	unsigned int nCurTong;// Id t��n nh��m hi?n ?? tham gia, n?u n�� l�� 0, hi?n kh?ng c�� trong nh��m
	int		nMissionGroup;// Hi?n t?i ?ang gia nh?p m?n ph��i id N?u l�� -1 th�� hi?n t?i kh?ng c�� trong m?n ph��i.
	int		nRepute; // danh vong
	int		nFuYuan; // phuc duyen
	int		nReBorn; // trung sinh
	int     nReBornLevel;
	int		nPKValue; // pk
	int     nLucky;
	int     nTonLuky;//B?n ??ng h��nh may m?n
	int     nCurPinDao;//k��nh t?m th?i -1 cho kh?ng
};

struct KUiPlayerPicInfo
{
	char  nPicPath[256];	//ͷ��·��
	int     nSprRankid;
};

struct KUiPlayerActivityInfo
{
	char	nTimeInfo[64];	//ʱ����Ϣ
	int     nTaskInfo;
	int     nHour;
	int     nMin;
	int     nNpcLevel;
	int     nExpTime;            //�ϴ���ȡ�����ʱ��
	int     nYinLiangTime;       //�ϴ���ȡ�����ʱ��
	int     nJinBiTime;
	int     nExpPiontTime;
};

struct KUiPlayerGameTime
{

	int   npTime;

};



//----------------------
struct KUiPlayerSetTimeInfo
{
	char	  nTimeInfo[64];	//ʱ����Ϣ
	int     nTaskInfo;
	int     nHour;
	int     nMin;
	int     nNpcLevel;
	int     nExpTime;            //�ϴ���ȡ�����ʱ��
	int     nYinLiangTime;       //�ϴ���ȡ�����ʱ��
	int     nJinBiTime;
	int     nExpPiontTime;

};
//==================================
//	���ǵ�һЩ�ױ������
//==================================
struct KUiPlayerRuntimeInfo
{
	int		nLifeFull;			//������ֵ
	int		nLife;				//����
	int		nManaFull;			//������ֵ
	int		nMana;				//����
	int		nStaminaFull;		//������ֵ
	int		nStamina;			//����
	int		nAngryFull;			//ŭ��ֵ
//	int		nAngry;				//ŭ
	int		nExperienceFull;	//������ֵ
	int		nExperience;		//��ǰ����ֵ
	int		nCurLevelExperience;//��ǰ����������Ҫ�ľ���ֵ
    int     nIsGetSkill;
	int     nIsGetChaqi;
	unsigned char	byActionDisable;//�Ƿ񲻿ɽ��и��ֶ�����Ϊö��PLAYER_ACTION_LISTȡֵ�����
	unsigned char	byAction;	//���ڽ��е���Ϊ������Ϊö��PLAYER_ACTION_LISTȡֵ�����
	unsigned short wReserved;	//����
	int     nVipType;
	unsigned long  nGameLeftTime;
	unsigned int     nIsTranse;
};

struct KUiGetCurAngry
{
	int		nAngry;				//ŭ
};

struct KItemlian
{

	unsigned int     uGenre;
	int                cClorid;
	unsigned int    cItemID;
	char           cItemName[32];  //����
	char         cPlayerName[32];  //�������
    int         nDwid;
};

struct KUiPlayerindexbaseInfo
{
	int		nmLifeFull;			//������ֵ
	int		nmLife;				//��ǰ����
	int		nmManaFull;			//������ֵ
	int		nmMana;				//��ǰ����
	int		nmStaminaFull;		//������ֵ
	int		nmStamina;			//����
	int		nmAngryFull;			//ŭ��ֵ
	int		nmAngry;				//ŭ
	int		nmExperienceFull;	//������ֵ
	int		nmExperience;		//��ǰ����ֵ
	int		nmCurLevelExperience;//��ǰ����������Ҫ�ľ���ֵ

	unsigned char	mbyActionDisable;//�Ƿ񲻿ɽ��и��ֶ�����Ϊö��PLAYER_ACTION_LISTȡֵ�����
	unsigned char	mbyAction;	//���ڽ��е���Ϊ������Ϊö��PLAYER_ACTION_LISTȡֵ�����
	unsigned short mwReserved;	//����
	int     nmLuky;//��ǰ����ֵ
};

//==================================
//	���ǵ�һЩ������������
//==================================
enum UI_PLAYER_ATTRIBUTE
{
	UIPA_STRENGTH = 0,			 //����
	UIPA_DEXTERITY,				//��
	UIPA_VITALITY,				//�⹦
	UIPA_ENERGY,				//�ڹ�
};

//==================================
//	���ǵ�һЩ�ױ����������
//==================================
struct KUiPlayerAttribute
{
	int		nMoney;				//����
	int		nXu;
	int		nLevel;				//�ȼ�
	char    StatusDesc[16];		//״̬����

	int		nBARemainPoint;		//��������ʣ�����
	int		nStrength;			//����
	int		nDexterity;			//����
	int		nVitality;			//����
	int		nEnergy;			//����

	int		nKillMAX;			//sat thuong lon nhat
	int		nKillMIN;			//sat thuong nho nhat
	int		nRightKillMax;		//sat thuong lon nhat tay phai
	int		nRightKillMin;		//sat thuong nho nhat tay phai

	int		nAttack;			//������
	int		nDefence;			//������
	int		nMoveSpeed;			//�ƶ��ٶ�
	int		nAttackSpeed;		//�⹥���ٶ�
	int		nCastSpeed;		    //�ڹ����ٶ�

	char		nPhyDef[32];			// Ph��ng ng? v?t l?
	char		nCoolDef[32];			//��������
	char		nLightDef[32];			//�������
	char		nFireDef[32];			//�������
	char		nPoisonDef[32];			//���ط���

	int		nRankInWorld;	// xep hang
	int		nRepute; // danh vong
	int		nFuYuan; // phuc duyen
	int		nReBorn; // trung sinh
	int		nPKValue; // phuc duyen
	int     nLuky;//gi�� tr? may m?n hi?n t?i
	int     nZhonji; // Tr?ng k��ch
	int     nDangge; // Ng?n c��ch
	int     nSprRankId;
	int     nForBitChat;
};

//==================================
//	���ǵ�����ʹ�ÿ�ݼ���Ʒ���书
//==================================
struct KUiPlayerImmedItemSkill
{
	KUiGameObject	ImmediaItem[9];  //��ݼ���Ʒ��������
	KUiGameObject	IMmediaSkill[2];
	int nStackNum;
};

//==================================
//	����װ��������λ��
//==================================
enum UI_EQUIPMENT_POSITION
{
	UIEP_HEAD = 0,		//ͷ��
	UIEP_HAND = 1,		//�ֳ�
	UIEP_NECK = 2,		//����
	UIEP_FINESSE = 3,	//����
	UIEP_BODY = 4,		//��
	UIEP_WAIST = 5,		//����
	UIEP_FINGER1 = 6,	//��ָ��
	UIEP_FINGER2 = 7,	//��ָ��
	UIEP_WAIST_DECOR= 8,//��׹
	UIEP_FOOT = 9,		//�Ų�
	UIEP_HORSE = 10,	//��ƥ
	UIEP_MASK = 11,		// mat na
	UIEP_PIFENG=12,	    //����
	UIEP_YINJIAN=13,    //ӡ��
	UIEP_SHIPING=14,	//��Ʒ
};

//==================================
//	���ǵ����������
//==================================
struct KUiPlayerLiveSkillBase
{
	int		nRemainPoint;			//ʣ�༼�ܵ���
	int		nLiveExperience;		//��ǰ���ܾ���ֵ
	int		nLiveExperienceFull;	//�����¼���Ҫ�ľ���ֵ
};


//==================================
//	����������
//==================================

struct KUiJinDuTiaoInfo
{
	char 	nTitle[32];
	int		nMsgIndex;
	int		nTimes;
	char    nCallFuc[16];
	char    szParma[32];
	char    szParmb[32];
	char    szParmc[32];
};

//==================================
//	���������
//==================================
struct KUiSkillData : public KUiGameObject
{
	union
	{
		int		nLevel;
		int		nData;
	};
//int	nJiaChengSkill;
};


//==================================
//	������������
//==================================
struct KUiChessData : public KUiGameObject
{
	union
	{
		int		nLevel;
		int		nData;
	};
	//int	nJiaChengSkill;
};


//==================================
//	һ����������������Ա����Ŀ
//==================================
#define	PLAYER_TEAM_MAX_MEMBER	8

//==================================
//	ͳ˧������ص�����
//==================================
struct KUiPlayerLeaderShip
{
	int		nLeaderShipLevel;			//ͳ˧���ȼ�
	int		nLeaderShipExperience;		//ͳ˧������ֵ
	int		nLeaderShipExperienceFull;	//�����¼���Ҫ�ľ���ֵ
};

/*struct KUiItemObject
{
	unsigned int	uGenre;
	unsigned int	uId;
	int				DataX;
	int				DataY;
	int				DataW;  //���
	int				DataH;  //�߶�
};*/
//==================================
//	һ����ҽ�ɫ��
//==================================
struct KUiPlayerItem
{
	char			Name[32];	    //��ҽ�ɫ����
	unsigned int	uId;		    //��ҽ�ɫDWid
	int				nIndex;		    //��ҽ�ɫ����
	int				nData;		    //�������ص�һ����ֵ������������Ӧ��λ���й�
	char            BZname[32];     //����������
	int             nShopType;      //�̵������
	int             nApplyStartPos; //�����б�ʼλ��
};
//==================================
//	һ����ҽ�ɫ��
//==================================
struct KUiLianItem
{
	char			Name[32];	//�������
	unsigned int	uId;		//��ƷDWid
	int				nIndex;		//��Ʒ����
	int				w;		    //���
	int             h;          //����
};

//==================================
//	�����Ϣ������
//==================================
struct KUiTeamItem
{
	KUiPlayerItem	Leader;
};

//==================================
//	������Ϣ
//==================================
struct KUiPlayerTeam
{
	bool			bTeamLeader;			//����Լ��Ƿ�ӳ�
	char			cNumMember;				//��Ա��Ŀ
	char			cNumTojoin;				//���������Ա����Ŀ
	bool			bOpened;				//�����Ƿ����������˼���
	int				nTeamServerID;			//�����ڷ������ϵ�id�����ڱ�ʶ�ö��飬-1 Ϊ��
	int				nCaptainPower;
};

/* ���ǾɵĴ��룬�µ��Ѿ�����KNpcGold���洦����
//==================================
//	NPC��ǿ
//==================================
struct KNpcEnchant
{
	int		nExp;					// ����
	int		nLife;					// ����
	int		nLifeReplenish;			// ��Ѫ
	int		nAttackRating;			// ����
	int		nDefense;				// ����
	int		nMinDamage;
	int		nMaxDamage;

	int		TreasureNumber;				// װ��
	int		AuraSkill;					// �⻷
	int		DamageEnhance;				// �˺�
	int		SpeedEnhance;				// �ٶ�
	int		SelfResist;					// ������
	int		ConquerResist;				// ��������
#ifndef _SERVER
	char	NameModify[32];				// ����
#endif
};

//==================================
//	NPC�����ǿ
//==================================
struct KNpcSpeicalEnchant
{
	int		ValueModify;
	char	NameModify[16];
};
*/

//==================================
//	ѡ��������
//==================================
enum OPTIONS_LIST
{
	OPTION_PERSPECTIVE,		//͸��ģʽ  nParam = (int)(bool)bEnable �Ƿ���
	OPTION_DYNALIGHT,		//��̬��Ӱ	nParam = (int)(bool)bEnable �Ƿ���
	OPTION_MUSIC_VALUE,		//��������	nParam = ������С��ȡֵΪ0��-10000��
	OPTION_SOUND_VALUE,		//��Ч����	nParam = ������С��ȡֵΪ0��-10000��
	OPTION_BRIGHTNESS,		//���ȵ���	nParam = ���ȴ�С��ȡֵΪ0��-100��
	OPTION_WEATHER,			//����Ч������ nParam = (int)(bool)bEnable �Ƿ���
};

//==================================
//	�����ĵ���ʱ�价����Ϣ
//==================================
struct KUiSceneTimeInfo
{
	char    szSceneName[64];		//������
	int		nSceneId;				//����id
	int		nScenePos0;				//������ǰ���꣨����
	int		nScenePos1;				//������ǰ���꣨�ϣ�
	int		nGameSpaceTime;			//�Է���Ϊ��λ
	char    nMapType[32];           //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int     nWarShui;
	int     nWarZhi;
	int     nIsWarCity;

	char    nWarGTong[32];
	char    nWarSTong[32];
	int     nIsWhos;

    char    nPlayerName[32];  //�Լ�����
	char    nTongMaster[32];  //�Լ�������
 /*

	char    nWarFuchengName[32]; //��������
	char    nWarZuohuName[32];
	char    nWarYuohuName[32];
	char    nWarTaiShiName[32];
	char    nWarZhonSName[32];
	char    nWarShangSName[32];
	char    nWarMishuName[32];
	char	nWarTaiLeName[32];    // ̫��


	int     nIsFuCheng;
	int     nIsZuoHu;
    int     nIsYuoHu;
	int     nIsTaiS;
	int     nIsZhonS;
	int     nIsShangS;
	int     nIsMiShu;
	int     nIsTaiLe;

	int     nCZIsNanOrNv;//���л�Ů
	int     nFZIsNanOrNv;
	int     nZHIsNanOrNv;
	int     nYHIsNanOrNv;
	int     nTSIsNanOrNv;
	int     nZSIsNanOrNv;
	int     nSSIsNanOrNv;
	int     nMSIsNanOrNv;
	int     nTLIsNanOrNv;
	*/
    int     nTaskVal;
	BOOL    nIsHideNuQi;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoA
{
	char	   szSceneName[32];		   //������
	int		nSceneId;				   //����id
	int		nScenePos0;				   //������ǰ���꣨����
	int		nScenePos1;				   //������ǰ���꣨�ϣ�
	int		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int     nWarShui;
	int     nWarZhi;
	int     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoB
{
	char	   szSceneName[32];		   //������
	int		nSceneId;				   //����id
	int		nScenePos0;				   //������ǰ���꣨����
	int		nScenePos1;				   //������ǰ���꣨�ϣ�
	int		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int     nWarShui;
	int     nWarZhi;
	int     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoC
{
	char    szSceneName[32];		   //������
	int		nSceneId;				   //����id
	int		nScenePos0;				   //������ǰ���꣨����
	int		nScenePos1;				   //������ǰ���꣨�ϣ�
	int		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int     nWarShui;
	int     nWarZhi;
	int     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoD
{
	char	   szSceneName[32];		   //������
	int		nSceneId;				   //����id
	int		nScenePos0;				   //������ǰ���꣨����
	int		nScenePos1;				   //������ǰ���꣨�ϣ�
	int		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int     nWarShui;
	int     nWarZhi;
	int     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoE
{
	char	    szSceneName[32];		   //������
	int		nSceneId;				   //����id
	int		nScenePos0;				   //������ǰ���꣨����
	int		nScenePos1;				   //������ǰ���꣨�ϣ�
	int		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int     nWarShui;
	int     nWarZhi;
	int     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoF
{
	char    szSceneName[32];		   //������
	int		nSceneId;				   //����id
	int		nScenePos0;				   //������ǰ���꣨����
	int		nScenePos1;				   //������ǰ���꣨�ϣ�
	int		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int     nWarShui;
	int     nWarZhi;
	int     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoG
{
	char   szSceneName[32];		   //������
	int		nSceneId;				   //����id
	int		nScenePos0;				   //������ǰ���꣨����
	int		nScenePos1;				   //������ǰ���꣨�ϣ�
	int		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int     nWarShui;
	int     nWarZhi;
	int     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoH
{
	char    szSceneName[32];		   //������
	int		nSceneId;				   //����id
	int		nScenePos0;				   //������ǰ���꣨����
	int		nScenePos1;				   //������ǰ���꣨�ϣ�
	int		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int     nWarShui;
	int     nWarZhi;
	int     nIsWarCity;
};

//==================================
//	��Դ��Ϣ
//==================================
//������ʾ����ά������
struct KPosition3
{
	int nX;
	int nY;
	int nZ;
};

struct KLightInfo
{
	KPosition3 oPosition;			// ��Դλ��
	unsigned long dwColor;					// ��Դ��ɫ������
	long  nRadius;					// ���ð뾶
};


//С��ͼ����ʾ������
enum SCENE_PLACE_MAP_ELEM
{
	SCENE_PLACE_MAP_ELEM_NONE		= 0x00,		//�޶���
	SCENE_PLACE_MAP_ELEM_PIC		= 0x01,		//��ʾ����ͼ
	SCENE_PLACE_MAP_ELEM_CHARACTER	= 0x02,		//��ʾ����
	SCENE_PLACE_MAP_ELEM_PARTNER	= 0x04,		//��ʾͬ������
};

//�����ĵ�ͼ��Ϣ
struct KSceneMapInfo
{
	int	nScallH;		//��ʵ��������ڵ�ͼ�ĺ���Ŵ����
	int nScallV;		//��ʵ��������ڵ�ͼ������Ŵ����
	int	nFocusMinH;
	int nFocusMinV;
	int nFocusMaxH;
	int nFocusMaxV;
	int nOrigFocusH;
	int nOrigFocusV;
	int nFocusOffsetH;
	int nFocusOffsetV;
};

struct KSceneMapSizePos
{
int Arealeft;
int Areatop;
};

struct KChaQIMapPos
{
	int	nCursorX;		//��ʵ��������ڵ�ͼ�ĺ���Ŵ����
	int nCursorY;		//��ʵ��������ڵ�ͼ������Ŵ����

};

struct KSkillMapPos
{
	int	nCursorX;		//��ʵ��������ڵ�ͼ�ĺ���Ŵ����
	int nCursorY;		//��ʵ��������ڵ�ͼ������Ŵ����

};


struct KNewsMessage
{
	int		nType;						//��Ϣ����
	char	sMsg[MAX_MESSAGE_LENGTH];	//��Ϣ����
	int		nMsgLen;					//��Ϣ���ݴ洢����
};

struct KTaskPathInfo
{
	int		nTaskType;				    //��Ϣ����
	int 	nMsgIndex;	                //��Ϣ����
	int		nMapIdx;					//�����ͼ
    int		nMapXpos;
	int		nMapYpos;
};


struct	KTaskClient
{
	char szContent[128];	//����
	int	 nTaskidx;			//������Ǯ����ֵΪ���۸񣬸�ֵ��ʾ����ļ۸�Ϊ(-nPrice)
	int	 nTaskType;
	char szTaskName[32];
};

struct KRankIndex
{
	bool			bValueAppened;	//ÿһ���Ƿ���û�ж�������
	bool			bSortFlag;		//ÿһ���Ƿ���û���������
	unsigned short	usIndexId;		//������ID��ֵ
};

struct KListIndex
{
    int nTaskId;
	int nType;
	int nClass;
	int nTrace;
};


#define MAX_RANK_MESSAGE_STRING_LENGTH 128

struct KRankMessage
{
	char szMsg[MAX_RANK_MESSAGE_STRING_LENGTH];	// ��������
	unsigned short		usMsgLen;				// �������ݵĳ���
	short				cSortFlag;				// ���ֵ��QOO_RANK_DATA��ʱ���ʾ����������ֵ��ʾ������ֵ��ʾ����0ֵ��ʾλ��δ��
	int					nValueAppend;			// �������ֵ

};

struct KMissionRecord
{
	char			sContent[256];	//�洢������ʾ��Ϣ�Ļ����������ַ���Ϊ���Ʒ��Ѿ�������ַ�����
	int				nContentLen;	//sContent����Ч���ݵĳ���(��λ���ֽ�)���������һ��������256�ֽ�
	unsigned int	uValue;			//������ֵ
};

//������Ϣ
struct KTongInfo
{
	char  szName[32];			//�������
	char  szMasterName[32];		//��������
	int   nFaction;				//�����Ӫ
	int   nMoney;				//����ʽ�
	int   nReserve[2];          //��������
	int	  nMemberCount;         //��������
	int   nManagerCount;        //�ӳ�����
	int   nDirectorCount;       //��������
	int   nFigure;              //��ɾ��
	int   nLevel;               //���ȼ�
	int   nGongxian;            //��ṱ�׶�
	int   nHouYue;              //����Ծ��
};

//���ɻ�����Ϣ
struct KClientTongInfo
{
	char  szName[32];			//�������
	char  szMasterName[32];		//��������
	int   nFaction;				//�����Ӫ
	int   nMoney;				//����ʽ�
	int   nReserve[2];          //��������
	int	  nMemberCount;         //��������
	int   nManagerCount;        //�ӳ�����
	int   nDirectorCount;       //��������
	int   nApplyCount;          //�����б������
	int   nZhaoMuCount;         //������ļ��Ϣ������
	int   nFigure;              //��ɾ��
	int   nLevel;               //���ȼ�
	int   nGongxian;            //��ṱ�׶�
	int   nHouYue;              //����Ծ��
};

//������ļ�͹�����Ϣ
struct KClientZhaoMuInfo
{
    int 	inType;
    char szQingXiang[32];
    char szHuoDong1[32];
    char	 szHuoDong2[32];
    char szHuoDong3[32];
    char szHuoDong4[32];
    char szJiYu[64];
    char    szGongGao[128];
    int     inAcceptLevel;
	int     inRefuseLevel;
};
//���������б�
struct KClientApplyList
{
    struct ApplyList
	{
      char	  szName[32];  //����
      int     inLevel;     //�ȼ�
	};
      int     nCurNum;
	  int 	  inType;      //����
	  int     nStartPos;
    ApplyList nInfo[defTONG_ONE_PAGE_MAX_NUM];
};
//�԰��ڳ�Ա���Ķ�������ı��Լ�����Ĺ�ϵ�Ķ���
enum TONG_ACTION_TYPE
{
	TONG_ACTION_DISMISS,       //����
	TONG_ACTION_ASSIGN,        //����
	TONG_ACTION_DEMISE,        //��λ
	TONG_ACTION_LEAVE,         //���
	TONG_ACTION_RECRUIT,       //����״̬
	TONG_ACTION_APPLY,         //�������

};

enum TONG_MONEY_ACTION_TYPE
{
	TONG_ACTION_SAVE,       //����
	TONG_ACTION_GET,        //����
	TONG_ACTION_SND,        //��λ
};

//�԰��ڳ�Ա���Ķ�������ı��Լ�����Ĺ�ϵ�Ķ���ʱ�Ĳ���
struct KTongOperationParam
{
	TONG_ACTION_TYPE	eOper;				//������
	int                 nData[4];
	char				szPassword[32];		//��������Ҫ�����룬�����Ӧ�Ĳ������Ҫ���룬��ֱ��������Ϊ�ա�
};

//����Ա��
struct KTongMemberItem : public KUiPlayerItem
{
	char szAgname[64];	//�ڰ��ڵ�ְ��/�ƺ�
};

//ĳ�������XX�Ĺ�ϵ��(XX�����ǰ�ᣬ����ȵ�)
struct KUiPlayerRelationWithOther : KUiPlayerItem
{
	int		nRelation;
	int		nParam;
};

//ͨ�õĴ�����������Ϸ����Ľṹ
struct KUiGameObjectWithName
{
	char			szName[32];  //�����
	char			szString[32];
	int				nData;       //ְλ
	int				nParam;      //��ǰ��ʼ���
	unsigned int 	uParam;      //��ǰ����
};

//ͨ�ð����Ϣ�ṹ
struct KUiTongInfoWithName
{
    char	 szTongName[32];
	char	 szQingXiang[32];
	char	 szHuoDong1[32];
	char	 szHuoDong2[32];
	char szHuoDong3[32];
	char szHuoDong4[32];
	char szJiYu[64];
	char    szGongGao[128];
	int     inAcceptLevel;
	int     inRefuseLevel;
};


//ͨ�ð����Ϣ�ṹ
struct gameAutoplaerInfo
{
	int 	 isopenmoney;
	int      isopendaoju;
	int      insopenzhuang;
	int      isopentuzhi;
	int      ismonyeandzhuang;
};

//ͨ��ִ�нű�
struct KUiPlyerDoscript
{
	int     inKind;
	int     inCurTime;
	int     inCurTimeb;
	int     inCurTimec;
	int     inCurTimed;
	char    szItemInfo[32];
};

//ͨ��ִ�нű�
struct KUiPlyerVipDoscript
{
	int     inKind;
	int     inCurExp;
	int     inCurLucky;
	int     inCurJiHuo;
	int     inCurWuHang;
	char    snCardAcc[32];
	char    snCardPass[32];
};


//ͨ��ִ�нű�
struct KUiPlyerDuanZao
{
	int     inKind;
	int     inCurTimea;
	int     inCurTimeb;
	int     inCurTimec;
	int     inCurTimed;
	//char    szItemInfo[32];
};


//ͨ��ִ�нű�
struct KUiPlyerVipDaZao
{
	int     inKind;
	int     inCurExp;
	int     inCurLucky;
	int     inCurJiHuo;
	int     inCurWuHang;
	char    snCardAcc[32];
	char    snCardPass[32];

};

struct	nJinDuClient
{
	int  nTimeVal;
	int	 nMsgidx;
	char szCallFun[16];   //�ص�����
	char szParma[32];     //����
	char szParmb[32];     //����
	char szParmc[32];     //����
};
//----------���Կͻ��˽���
#endif


#endif
