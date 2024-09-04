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
	DWORD	m_dwRegionID;
	int32_t		m_nNo;
};

typedef struct
{
	int32_t		nIdx;
	int32_t		nPlace;
	int32_t		nX;
	int32_t		nY;
	int32_t		nPrice;      //�̵깺��۸�
	int32_t     nIsRongQi;   //�ĸ�������Ķ���
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
	int32_t		nSubWorld;
	int32_t		nRegion;
	int32_t		nMapX;
	int32_t		nMapY;
	int32_t		nOffX;
	int32_t		nOffY;
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
	int32_t				nAttribType;					//��������
	int32_t				nValue[3];						//���Բ���
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
	int32_t			  nGlbParam[MAX_GLBMISSION_PARAM]; //6
	int32_t			  nParam[MAX_MISSION_PARAM];   // 32
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
	int32_t			  nParam[MAX_MISSION_PARAM]; //32
	/*void Clear() {
		ucGroup = 0;
		memset(Name, 0, sizeof(Name));
		memset(nParam, 0, sizeof(nParam));
	};*/
};
typedef struct
{

	int32_t		m_nBuyIdx;
	DWORD	m_SubWorldID;
	int32_t		m_nMpsX;
	int32_t		m_nMpsY;
	//int32_t		m_nShopIdx[MAX_SUPERSHOP_SHOPTAB];
	//int32_t		m_nShopNum;
	int32_t		m_nPriceType;

	void	Clear() {
		//memset(m_nShopIdx, -1, sizeof(m_nShopIdx));
		m_nBuyIdx = -1 ; m_nPriceType = moneyunit_money; m_SubWorldID = -1; m_nMpsX = 0; m_nMpsY = 0;
	}
} BuySellInfo;

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
	int32_t bYear;
	int32_t bMonth;
	int32_t bDay;
	int32_t bHour;
	int32_t bMin;
};
struct KviewTime
{
	int32_t bYear;
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
		  int32_t         nLuckRateSum;
		  int32_t			nMagicRate;
		  int32_t			nMaxRandRate;
		  int32_t			nMoneyRate;
		  int32_t         nMoneyNum;
		  int32_t			nMoneyScale;
		  int32_t			nMinItemLevelScale;   //��С�ȼ�����
		  int32_t			nMaxItemLevelScale;	  //
		  int32_t		    nMinItemLevel;
		  int32_t		    nMaxItemLevel;
		  int32_t         nTypeNum;             //���µ�����
		  int32_t         nIsBianShi;           //�Ƿ���Ա� �ɱ�ʶװ��
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
	int32_t Uid[16];
	int32_t nMun;
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
	int32_t		AnswerLen;		//��ѡ�𰸴洢���ȣ��������Ʒ�����������������
};
struct KUiQuestionAndAnswer
{
	char		Question[512];	//�������֣����԰������Ʒ��� //����Ϣ
	int32_t			QuestionLen;	//�������ִ洢���ȣ��������Ʒ�����������������
	int32_t			AnswerCount;	//��ѡ�𰸵���Ŀ
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
	DWORD	dwItemNameColor;		//��Ʒ������ɫ
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
	int32_t				nPrice;			//����������Ǯ����ֵΪ���۸񣬸�ֵ��ʾ����ļ۸�Ϊ(-nPrice)
	int32_t				nPriceXu;       //���
	BYTE            nPriceType;     //���ҵ�����

	int32_t             nOldPrice;
	int32_t             nOldPriceXu;
	int32_t             nCurPriceXu;
	BOOL            bNewArrival;
//	int32_t             nMoneyUnit;

};

//==================================
//	һЩͨ�ýṹ
//==================================
struct KUiTongYong
{
	int32_t inKind;
	int32_t inParma;
	int32_t inParmb;
	int32_t inParmc;
	int32_t inParmd;
};
/*
struct	nJinDuClient
{
	int32_t  nTimeVal;
	int32_t	 nMsgidx;
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
		int32_t	 nTaskidx;			//������Ǯ����ֵΪ���۸񣬸�ֵ��ʾ����ļ۸�Ϊ(-nPrice)
		int32_t	 nTaskType;
		char  nTaskName[32];
	};
	KTaskClient nTaskInfo[20];
};
//==================================
//	���Ա�ʾ��Ϸ����Ľṹ
//==================================
struct KUiGameObject
{
	uint32_t uGenre;	//��������
	uint32_t uId;		//����id
	//	int32_t			 nData;		//�����ʵ����ص�ĳ����
};

//==================================
//	�������ʾ��һ������Χ
//==================================
struct KUiRegion
{
	int32_t		h;		//���Ͻ���������
	int32_t		v;		//���Ͻ����������
	int32_t		Width;	//������
	int32_t		Height;	//�����ݿ�
	int32_t     IsRongqi; //���ĸ����������� �������� �� �����������
};

struct KUiItemdObject
{
	uint32_t	uGenre;
	uint32_t	uId;
	uint32_t    nindex;
	int32_t				DataX;
	int32_t				DataY;
	int32_t				DataW;   //���
	int32_t				DataH;   //�߶�
	int32_t             nGenkind;//���ڵ���������
	Rect          nRect;
};

struct KUiDraggedObject
{
	uint32_t	uGenre;
	uint32_t	uId;
	int32_t				DataX;
	int32_t				DataY;
	int32_t				DataW;  //���
	int32_t				DataH;  //�߶�
	//    char            uName[32];
	int32_t m_posvs;
	unsigned char m_dirvs,m_vs_lastframe;
	int32_t bCheckpVS;
	KRUPoint* pVS;
	int32_t* ndir;
	uint32_t nDelay;
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
		int32_t					nContainer;
	};
};
//========��������===============
struct KUiJinMaiDescz

{
	struct KUiDesc
	{
		int32_t ncZhenYuan;
		int32_t ncHuMaiDan;
		int32_t ncRaTe;
		int32_t ncMgcid;
		int32_t ncBinjia;
		char ncMiaoShu[215];
		char nXueNane[32];

	};
	KUiDesc  nJinMaiDesc[17];
};

//========�ϻ���===============
struct KUiLaoHuJi
{
	int32_t nIsOpen;
	int32_t nType;
	int32_t nYinL;
	int32_t nJinBi;
	int32_t nBeiLv;
	int32_t nDianShu;
};

//========����===============
struct KUiJinMaiLevel
{
	struct KUiLevel
	{
		int32_t  nLeVel[18];
	};
  KUiLevel  nJinMai[10];
  int32_t nZhenYuan;
  int32_t nBinJia;
};
//========����===============
struct KUiJinMaiType
{
	int32_t  nType;
	int32_t  nLevel;
	//	KUiJinMaiLevel  nJinMai[10];
};



//========����===============
struct KUiJinMaiInfo
{
	int32_t  nZhenYuan;
	int32_t  nBingJia;
//	KUiJinMaiLevel  nJinMai[10];
};

//========��ħ===============
struct KUiFuMoInfo
{
	int32_t  nNpcSkill[5];
	BYTE nSkillLevel[5];
	int32_t  nNpcGongji[5];
	int32_t  nNpcFangyu[5];
	int32_t  nNpclife[5];
	int32_t  nNpcGongsu[5];
	int32_t  nNpcXingYun[5];

};

//========��ս��Ϣ===============
struct KUiAttackInfo
{
	int32_t         m_AttackState;
	int32_t         m_AttackTime;
	int32_t         m_nDeathCount;	   //�����Ĵ���
	int32_t         m_nDDeathCount;	   //�����Ĵ���\
	int32_t         m_nAttackNum;	                            // ��ս����
	int32_t         m_nWinNum;								// ʤ������
	int32_t         m_nLoseNum;
	char        m_AttackName[32];  //��ս�İ��
};

/*typedef struct
{
	int32_t		m_nBuyIdx;     //����ID
	DWORD	m_SubWorldID;
	int32_t		m_nMpsX;
	int32_t		m_nMpsY;
	void	Clear() {m_nBuyIdx = -1; m_SubWorldID = -1; m_nMpsX = 0; m_nMpsY = 0;}
} NewBuySellInfo;*/


//========ͬ��===============
struct KUiTongBanbaseInfo
{
int32_t  nmLife;
int32_t  nmLifeFull;
int32_t  nmMana;
int32_t  nmManaFull;
};
//========ͬ��===============
struct KUiTongBanInfo
{
	int32_t   nDwID;
	char  nTname[32];
	char  nGuiShuName[32];
	int32_t   nCount;
};
//========ת�̳齱===============
struct KUiZhuanpanInfo
{
int32_t  nIsVip;
int32_t  nCJcishu;
char nPlayerName[32];
};
//========����===============
struct KUiGuZiInfo
{
	int32_t  nIsVip;
	int32_t  nCJcishuA;   //����
	int32_t  nCJcishuB;   //����
	int32_t  nCJcishuC;   //����
	int32_t  nCJcishuD;   //����
	int32_t  nCJcishuE;   //����
	char nPlayerNameA[32];
	char nPlayerNameB[32];
	char nPlayerNameC[32];
	char nPlayerNameD[32];
	char nPlayerNameE[32];
	int32_t  nIsShuai;
	int32_t  nGyajinA;  //Ѻ��
	int32_t  nGyajinB;  //Ѻ��
	int32_t  nGyajinC;  //Ѻ��
	int32_t  nGyajinD;  //Ѻ��
	int32_t  nGyajinE;  //Ѻ��
	int32_t  nIsStartA;  //��ʼ�ı�ʾ
	int32_t  nIsStartB;  //��ʼ�ı�ʾ
	int32_t  nIsStartC;  //��ʼ�ı�ʾ
	int32_t  nIsStartD;  //��ʼ�ı�ʾ
	int32_t  nIsStartE;  //��ʼ�ı�ʾ
	int32_t  nZuoweiHao;  //��λ��
	int32_t  nIsMenber;
	int32_t  nmodel;   //ģʽ
	int32_t  nSubZuoWeiHao; //��ͼ����λ ��˭˦����
	int32_t  nIsXiaZhuA;//�Ƿ�������
	int32_t  nIsXiaZhuB;
	int32_t  nIsXiaZhuC;
	int32_t  nIsXiaZhuD;
	int32_t  nIsXiaZhuE;
	int32_t  nCurFarmA;  //��ǰ֡
	int32_t  nCurFarmB;  //��ǰ֡

};

struct KUiItemLian
{
	int32_t nItemDWID;
	char nPlayerName[32];
};

struct KUiGuZiXinxi
{
int32_t nMapID;
int32_t nDianshu;
int32_t nIsStartID;

};
struct KUiGuZiJieSuan
{
	int32_t nMaxDian;
	int32_t nKFmoney;
	int32_t nGRdian[5];
	int32_t nGRjinbi[5];
	char nPlayerNameA[32];
	char nPlayerNameB[32];
	char nPlayerNameC[32];
	char nPlayerNameD[32];
	char nPlayerNameE[32];
};
struct KUiGuZiCurFarms
{
	int32_t nCurFarmsA;
	int32_t nCurFarmsB;
	int32_t nMapID;
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
	uint32_t uId[24];
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
	uint32_t uId[32];
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
	uint32_t	uReservedForUi;	//����ʹ�õ�������,core����0����
	unsigned char	eType;			//��Ϣ����ȡֵ����ö������ SYS_MESSAGE_TYPE
	unsigned char	byConfirmType;	//��Ӧ����
	unsigned char	byPriority;		//���ȼ�,��ֵԽ�󣬱�ʾ���ȼ�Խ��
	unsigned char	byParamSize;	//����GDCNI_SYSTEM_MESSAGE��Ϣ��pParamBuf��ָ�����������ռ�Ĵ�С��
    int32_t		        nMsgLen;
};

//==================================
//	����Ƶ��������
//==================================
struct KUiChatChannel
{
	int32_t			 nChannelNo;
	uint32_t uChannelId;
	union
	{
		int32_t		 nChannelIndex;
		int32_t		 nIsSubscibed;	//�Ƿ񱻶���
	};
	char		 cTitle[32];
};

//==================================
//	������ѵ�һ���������Ϣ
//==================================
struct KUiChatGroupInfo
{
	char	    szTitle[32];	//���������
	int32_t		nNumFriend;		//���ں��ѵ���Ŀ
};

//==================================
//	���ѷ��������컰��
//==================================
struct KUiChatMessage
{
	uint32_t uColor;
	short	nContentLen;
	char    szContent[256];
};

//==================================
//	��ȡ�����ֵ
//==================================

struct KUiBaseTask
{
	int32_t nVala;
	int32_t nValb;
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
	int32_t		nCurFaction;// Hi?n t?i ?ang gia nh?p m?n ph��i id N?u l�� -1 th�� hi?n t?i kh?ng c�� trong m?n ph��i.
	int32_t		nRankInWorld;//Gi�� tr? x?p h?ng Jianghu, gi�� tr? 0 c�� ngh?a l�� kh?ng c�� trong b?ng x?p h?ng
	uint32_t nCurTong;// Id t��n nh��m hi?n ?? tham gia, n?u n�� l�� 0, hi?n kh?ng c�� trong nh��m
	int32_t		nMissionGroup;// Hi?n t?i ?ang gia nh?p m?n ph��i id N?u l�� -1 th�� hi?n t?i kh?ng c�� trong m?n ph��i.
	int32_t		nRepute; // danh vong
	int32_t		nFuYuan; // phuc duyen
	int32_t		nReBorn; // trung sinh
	int32_t     nReBornLevel;
	int32_t		nPKValue; // pk
	int32_t     nLucky;
	int32_t     nTonLuky;//B?n ??ng h��nh may m?n
	int32_t     nCurPinDao;//k��nh t?m th?i -1 cho kh?ng
};

struct KUiPlayerPicInfo
{
	char  nPicPath[256];	//ͷ��·��
	int32_t     nSprRankid;
};

struct KUiPlayerActivityInfo
{
	char	nTimeInfo[64];	//ʱ����Ϣ
	int32_t     nTaskInfo;
	int32_t     nHour;
	int32_t     nMin;
	int32_t     nNpcLevel;
	int32_t     nExpTime;            //�ϴ���ȡ�����ʱ��
	int32_t     nYinLiangTime;       //�ϴ���ȡ�����ʱ��
	int32_t     nJinBiTime;
	int32_t     nExpPiontTime;
};

struct KUiPlayerGameTime
{

	LONG   npTime;

};



//----------------------
struct KUiPlayerSetTimeInfo
{
	char	  nTimeInfo[64];	//ʱ����Ϣ
	int32_t     nTaskInfo;
	int32_t     nHour;
	int32_t     nMin;
	int32_t     nNpcLevel;
	int32_t     nExpTime;            //�ϴ���ȡ�����ʱ��
	int32_t     nYinLiangTime;       //�ϴ���ȡ�����ʱ��
	int32_t     nJinBiTime;
	int32_t     nExpPiontTime;

};
//==================================
//	���ǵ�һЩ�ױ������
//==================================
struct KUiPlayerRuntimeInfo
{
	int32_t		nLifeFull;			//������ֵ
	int32_t		nLife;				//����
	int32_t		nManaFull;			//������ֵ
	int32_t		nMana;				//����
	int32_t		nStaminaFull;		//������ֵ
	int32_t		nStamina;			//����
	int32_t		nAngryFull;			//ŭ��ֵ
//	int32_t		nAngry;				//ŭ
	int32_t		nExperienceFull;	//������ֵ
	int32_t		nExperience;		//��ǰ����ֵ
	int32_t		nCurLevelExperience;//��ǰ����������Ҫ�ľ���ֵ
    int32_t     nIsGetSkill;
	int32_t     nIsGetChaqi;
	unsigned char	byActionDisable;//�Ƿ񲻿ɽ��и��ֶ�����Ϊö��PLAYER_ACTION_LISTȡֵ�����
	unsigned char	byAction;	//���ڽ��е���Ϊ������Ϊö��PLAYER_ACTION_LISTȡֵ�����
	unsigned short wReserved;	//����
	int32_t     nVipType;
	DWORD  nGameLeftTime;
	uint32_t     nIsTranse;
};

struct KUiGetCurAngry
{
	int32_t		nAngry;				//ŭ
};

struct KItemlian
{

	uint32_t     uGenre;
	int32_t                cClorid;
	uint32_t    cItemID;
	char           cItemName[32];  //����
	char         cPlayerName[32];  //�������
    int32_t         nDwid;
};

struct KUiPlayerindexbaseInfo
{
	int32_t		nmLifeFull;			//������ֵ
	int32_t		nmLife;				//��ǰ����
	int32_t		nmManaFull;			//������ֵ
	int32_t		nmMana;				//��ǰ����
	int32_t		nmStaminaFull;		//������ֵ
	int32_t		nmStamina;			//����
	int32_t		nmAngryFull;			//ŭ��ֵ
	int32_t		nmAngry;				//ŭ
	int32_t		nmExperienceFull;	//������ֵ
	int32_t		nmExperience;		//��ǰ����ֵ
	int32_t		nmCurLevelExperience;//��ǰ����������Ҫ�ľ���ֵ

	unsigned char	mbyActionDisable;//�Ƿ񲻿ɽ��и��ֶ�����Ϊö��PLAYER_ACTION_LISTȡֵ�����
	unsigned char	mbyAction;	//���ڽ��е���Ϊ������Ϊö��PLAYER_ACTION_LISTȡֵ�����
	unsigned short mwReserved;	//����
	int32_t     nmLuky;//��ǰ����ֵ
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
	int32_t		nMoney;				//����
	int32_t		nXu;
	int32_t		nLevel;				//�ȼ�
	char    StatusDesc[16];		//״̬����

	int32_t		nBARemainPoint;		//��������ʣ�����
	int32_t		nStrength;			//����
	int32_t		nDexterity;			//����
	int32_t		nVitality;			//����
	int32_t		nEnergy;			//����

	int32_t		nKillMAX;			//sat thuong lon nhat
	int32_t		nKillMIN;			//sat thuong nho nhat
	int32_t		nRightKillMax;		//sat thuong lon nhat tay phai
	int32_t		nRightKillMin;		//sat thuong nho nhat tay phai

	int32_t		nAttack;			//������
	int32_t		nDefence;			//������
	int32_t		nMoveSpeed;			//�ƶ��ٶ�
	int32_t		nAttackSpeed;		//�⹥���ٶ�
	int32_t		nCastSpeed;		    //�ڹ����ٶ�

	char		nPhyDef[32];			// Ph��ng ng? v?t l?
	char		nCoolDef[32];			//��������
	char		nLightDef[32];			//�������
	char		nFireDef[32];			//�������
	char		nPoisonDef[32];			//���ط���

	int32_t		nRankInWorld;	// xep hang
	int32_t		nRepute; // danh vong
	int32_t		nFuYuan; // phuc duyen
	int32_t		nReBorn; // trung sinh
	int32_t		nPKValue; // phuc duyen
	int32_t     nLuky;//gi�� tr? may m?n hi?n t?i
	int32_t     nZhonji; // Tr?ng k��ch
	int32_t     nDangge; // Ng?n c��ch
	int32_t     nSprRankId;
	int32_t     nForBitChat;
};

//==================================
//	���ǵ�����ʹ�ÿ�ݼ���Ʒ���书
//==================================
struct KUiPlayerImmedItemSkill
{
	KUiGameObject	ImmediaItem[9];  //��ݼ���Ʒ��������
	KUiGameObject	IMmediaSkill[2];
	int32_t nStackNum;
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
	int32_t		nRemainPoint;			//ʣ�༼�ܵ���
	int32_t		nLiveExperience;		//��ǰ���ܾ���ֵ
	int32_t		nLiveExperienceFull;	//�����¼���Ҫ�ľ���ֵ
};


//==================================
//	����������
//==================================

struct KUiJinDuTiaoInfo
{
	char 	nTitle[32];
	int32_t		nMsgIndex;
	int32_t		nTimes;
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
		int32_t		nLevel;
		int32_t		nData;
	};
//int32_t	nJiaChengSkill;
};


//==================================
//	������������
//==================================
struct KUiChessData : public KUiGameObject
{
	union
	{
		int32_t		nLevel;
		int32_t		nData;
	};
	//int32_t	nJiaChengSkill;
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
	int32_t		nLeaderShipLevel;			//ͳ˧���ȼ�
	int32_t		nLeaderShipExperience;		//ͳ˧������ֵ
	int32_t		nLeaderShipExperienceFull;	//�����¼���Ҫ�ľ���ֵ
};

/*struct KUiItemObject
{
	uint32_t	uGenre;
	uint32_t	uId;
	int32_t				DataX;
	int32_t				DataY;
	int32_t				DataW;  //���
	int32_t				DataH;  //�߶�
};*/
//==================================
//	һ����ҽ�ɫ��
//==================================
struct KUiPlayerItem
{
	char			Name[32];	    //��ҽ�ɫ����
	uint32_t	uId;		    //��ҽ�ɫDWid
	int32_t				nIndex;		    //��ҽ�ɫ����
	int32_t				nData;		    //�������ص�һ����ֵ������������Ӧ��λ���й�
	char            BZname[32];     //����������
	int32_t             nShopType;      //�̵������
	int32_t             nApplyStartPos; //�����б�ʼλ��
};
//==================================
//	һ����ҽ�ɫ��
//==================================
struct KUiLianItem
{
	char			Name[32];	//�������
	uint32_t	uId;		//��ƷDWid
	int32_t				nIndex;		//��Ʒ����
	int32_t				w;		    //���
	int32_t             h;          //����
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
	int32_t				nTeamServerID;			//�����ڷ������ϵ�id�����ڱ�ʶ�ö��飬-1 Ϊ��
	int32_t				nCaptainPower;
};

/* ���ǾɵĴ��룬�µ��Ѿ�����KNpcGold���洦����
//==================================
//	NPC��ǿ
//==================================
struct KNpcEnchant
{
	int32_t		nExp;					// ����
	int32_t		nLife;					// ����
	int32_t		nLifeReplenish;			// ��Ѫ
	int32_t		nAttackRating;			// ����
	int32_t		nDefense;				// ����
	int32_t		nMinDamage;
	int32_t		nMaxDamage;

	int32_t		TreasureNumber;				// װ��
	int32_t		AuraSkill;					// �⻷
	int32_t		DamageEnhance;				// �˺�
	int32_t		SpeedEnhance;				// �ٶ�
	int32_t		SelfResist;					// ������
	int32_t		ConquerResist;				// ��������
#ifndef _SERVER
	char	NameModify[32];				// ����
#endif
};

//==================================
//	NPC�����ǿ
//==================================
struct KNpcSpeicalEnchant
{
	int32_t		ValueModify;
	char	NameModify[16];
};
*/

//==================================
//	ѡ��������
//==================================
enum OPTIONS_LIST
{
	OPTION_PERSPECTIVE,		//͸��ģʽ  nParam = (int32_t)(bool)bEnable �Ƿ���
	OPTION_DYNALIGHT,		//��̬��Ӱ	nParam = (int32_t)(bool)bEnable �Ƿ���
	OPTION_MUSIC_VALUE,		//��������	nParam = ������С��ȡֵΪ0��-10000��
	OPTION_SOUND_VALUE,		//��Ч����	nParam = ������С��ȡֵΪ0��-10000��
	OPTION_BRIGHTNESS,		//���ȵ���	nParam = ���ȴ�С��ȡֵΪ0��-100��
	OPTION_WEATHER,			//����Ч������ nParam = (int32_t)(bool)bEnable �Ƿ���
};

//==================================
//	�����ĵ���ʱ�价����Ϣ
//==================================
struct KUiSceneTimeInfo
{
	char    szSceneName[64];		//������
	int32_t		nSceneId;				//����id
	int32_t		nScenePos0;				//������ǰ���꣨����
	int32_t		nScenePos1;				//������ǰ���꣨�ϣ�
	int32_t		nGameSpaceTime;			//�Է���Ϊ��λ
	char    nMapType[32];           //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int32_t     nWarShui;
	int32_t     nWarZhi;
	int32_t     nIsWarCity;

	char    nWarGTong[32];
	char    nWarSTong[32];
	int32_t     nIsWhos;

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


	int32_t     nIsFuCheng;
	int32_t     nIsZuoHu;
    int32_t     nIsYuoHu;
	int32_t     nIsTaiS;
	int32_t     nIsZhonS;
	int32_t     nIsShangS;
	int32_t     nIsMiShu;
	int32_t     nIsTaiLe;

	int32_t     nCZIsNanOrNv;//���л�Ů
	int32_t     nFZIsNanOrNv;
	int32_t     nZHIsNanOrNv;
	int32_t     nYHIsNanOrNv;
	int32_t     nTSIsNanOrNv;
	int32_t     nZSIsNanOrNv;
	int32_t     nSSIsNanOrNv;
	int32_t     nMSIsNanOrNv;
	int32_t     nTLIsNanOrNv;
	*/
    int32_t     nTaskVal;
	BOOL    nIsHideNuQi;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoA
{
	char	   szSceneName[32];		   //������
	int32_t		nSceneId;				   //����id
	int32_t		nScenePos0;				   //������ǰ���꣨����
	int32_t		nScenePos1;				   //������ǰ���꣨�ϣ�
	int32_t		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int32_t     nWarShui;
	int32_t     nWarZhi;
	int32_t     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoB
{
	char	   szSceneName[32];		   //������
	int32_t		nSceneId;				   //����id
	int32_t		nScenePos0;				   //������ǰ���꣨����
	int32_t		nScenePos1;				   //������ǰ���꣨�ϣ�
	int32_t		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int32_t     nWarShui;
	int32_t     nWarZhi;
	int32_t     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoC
{
	char    szSceneName[32];		   //������
	int32_t		nSceneId;				   //����id
	int32_t		nScenePos0;				   //������ǰ���꣨����
	int32_t		nScenePos1;				   //������ǰ���꣨�ϣ�
	int32_t		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int32_t     nWarShui;
	int32_t     nWarZhi;
	int32_t     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoD
{
	char	   szSceneName[32];		   //������
	int32_t		nSceneId;				   //����id
	int32_t		nScenePos0;				   //������ǰ���꣨����
	int32_t		nScenePos1;				   //������ǰ���꣨�ϣ�
	int32_t		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int32_t     nWarShui;
	int32_t     nWarZhi;
	int32_t     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoE
{
	char	    szSceneName[32];		   //������
	int32_t		nSceneId;				   //����id
	int32_t		nScenePos0;				   //������ǰ���꣨����
	int32_t		nScenePos1;				   //������ǰ���꣨�ϣ�
	int32_t		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int32_t     nWarShui;
	int32_t     nWarZhi;
	int32_t     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoF
{
	char    szSceneName[32];		   //������
	int32_t		nSceneId;				   //����id
	int32_t		nScenePos0;				   //������ǰ���꣨����
	int32_t		nScenePos1;				   //������ǰ���꣨�ϣ�
	int32_t		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int32_t     nWarShui;
	int32_t     nWarZhi;
	int32_t     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoG
{
	char   szSceneName[32];		   //������
	int32_t		nSceneId;				   //����id
	int32_t		nScenePos0;				   //������ǰ���꣨����
	int32_t		nScenePos1;				   //������ǰ���꣨�ϣ�
	int32_t		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int32_t     nWarShui;
	int32_t     nWarZhi;
	int32_t     nIsWarCity;
};
//==================================
//	�����İ�ս��ͼ��Ϣ
//==================================
struct KUiCityMapInfoH
{
	char    szSceneName[32];		   //������
	int32_t		nSceneId;				   //����id
	int32_t		nScenePos0;				   //������ǰ���꣨����
	int32_t		nScenePos1;				   //������ǰ���꣨�ϣ�
	int32_t		nGameSpaceTime;			   //�Է���Ϊ��λ
	char    nMapType[16];               //��ͼ����
	char    nWarMaster[32];
	char    nWarTong[32];
	int32_t     nWarShui;
	int32_t     nWarZhi;
	int32_t     nIsWarCity;
};

//==================================
//	��Դ��Ϣ
//==================================
//������ʾ����ά������
struct KPosition3
{
	int32_t nX;
	int32_t nY;
	int32_t nZ;
};

struct KLightInfo
{
	KPosition3 oPosition;			// ��Դλ��
	DWORD dwColor;					// ��Դ��ɫ������
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
	int32_t	nScallH;		//��ʵ��������ڵ�ͼ�ĺ���Ŵ����
	int32_t nScallV;		//��ʵ��������ڵ�ͼ������Ŵ����
	int32_t	nFocusMinH;
	int32_t nFocusMinV;
	int32_t nFocusMaxH;
	int32_t nFocusMaxV;
	int32_t nOrigFocusH;
	int32_t nOrigFocusV;
	int32_t nFocusOffsetH;
	int32_t nFocusOffsetV;
};

struct KSceneMapSizePos
{
int32_t Arealeft;
int32_t Areatop;
};

struct KChaQIMapPos
{
	int32_t	nCursorX;		//��ʵ��������ڵ�ͼ�ĺ���Ŵ����
	int32_t nCursorY;		//��ʵ��������ڵ�ͼ������Ŵ����

};

struct KSkillMapPos
{
	int32_t	nCursorX;		//��ʵ��������ڵ�ͼ�ĺ���Ŵ����
	int32_t nCursorY;		//��ʵ��������ڵ�ͼ������Ŵ����

};


struct KNewsMessage
{
	int32_t		nType;						//��Ϣ����
	char	sMsg[MAX_MESSAGE_LENGTH];	//��Ϣ����
	int32_t		nMsgLen;					//��Ϣ���ݴ洢����
};

struct KTaskPathInfo
{
	int32_t		nTaskType;				    //��Ϣ����
	int32_t 	nMsgIndex;	                //��Ϣ����
	int32_t		nMapIdx;					//�����ͼ
    int32_t		nMapXpos;
	int32_t		nMapYpos;
};


struct	KTaskClient
{
	char szContent[128];	//����
	int32_t	 nTaskidx;			//������Ǯ����ֵΪ���۸񣬸�ֵ��ʾ����ļ۸�Ϊ(-nPrice)
	int32_t	 nTaskType;
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
    int32_t nTaskId;
	int32_t nType;
	int32_t nClass;
	int32_t nTrace;
};


#define MAX_RANK_MESSAGE_STRING_LENGTH 128

struct KRankMessage
{
	char szMsg[MAX_RANK_MESSAGE_STRING_LENGTH];	// ��������
	unsigned short		usMsgLen;				// �������ݵĳ���
	short				cSortFlag;				// ���ֵ��QOO_RANK_DATA��ʱ���ʾ����������ֵ��ʾ������ֵ��ʾ����0ֵ��ʾλ��δ��
	int32_t					nValueAppend;			// �������ֵ

};

struct KMissionRecord
{
	char			sContent[256];	//�洢������ʾ��Ϣ�Ļ����������ַ���Ϊ���Ʒ��Ѿ�������ַ�����
	int32_t				nContentLen;	//sContent����Ч���ݵĳ���(��λ���ֽ�)���������һ��������256�ֽ�
	uint32_t	uValue;			//������ֵ
};

//������Ϣ
struct KTongInfo
{
	char  szName[32];			//�������
	char  szMasterName[32];		//��������
	int32_t   nFaction;				//�����Ӫ
	int32_t   nMoney;				//����ʽ�
	int32_t   nReserve[2];          //��������
	int32_t	  nMemberCount;         //��������
	int32_t   nManagerCount;        //�ӳ�����
	int32_t   nDirectorCount;       //��������
	int32_t   nFigure;              //��ɾ��
	int32_t   nLevel;               //���ȼ�
	int32_t   nGongxian;            //��ṱ�׶�
	int32_t   nHouYue;              //����Ծ��
};

//���ɻ�����Ϣ
struct KClientTongInfo
{
	char  szName[32];			//�������
	char  szMasterName[32];		//��������
	int32_t   nFaction;				//�����Ӫ
	int32_t   nMoney;				//����ʽ�
	int32_t   nReserve[2];          //��������
	int32_t	  nMemberCount;         //��������
	int32_t   nManagerCount;        //�ӳ�����
	int32_t   nDirectorCount;       //��������
	int32_t   nApplyCount;          //�����б������
	int32_t   nZhaoMuCount;         //������ļ��Ϣ������
	int32_t   nFigure;              //��ɾ��
	int32_t   nLevel;               //���ȼ�
	int32_t   nGongxian;            //��ṱ�׶�
	int32_t   nHouYue;              //����Ծ��
};

//������ļ�͹�����Ϣ
struct KClientZhaoMuInfo
{
    int32_t 	inType;
    char szQingXiang[32];
    char szHuoDong1[32];
    char	 szHuoDong2[32];
    char szHuoDong3[32];
    char szHuoDong4[32];
    char szJiYu[64];
    char    szGongGao[128];
    int32_t     inAcceptLevel;
	int32_t     inRefuseLevel;
};
//���������б�
struct KClientApplyList
{
    struct ApplyList
	{
      char	  szName[32];  //����
      int32_t     inLevel;     //�ȼ�
	};
      int32_t     nCurNum;
	  int32_t 	  inType;      //����
	  int32_t     nStartPos;
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
	int32_t                 nData[4];
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
	int32_t		nRelation;
	int32_t		nParam;
};

//ͨ�õĴ�����������Ϸ����Ľṹ
struct KUiGameObjectWithName
{
	char			szName[32];  //�����
	char			szString[32];
	int32_t				nData;       //ְλ
	int32_t				nParam;      //��ǰ��ʼ���
	uint32_t 	uParam;      //��ǰ����
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
	int32_t     inAcceptLevel;
	int32_t     inRefuseLevel;
};


//ͨ�ð����Ϣ�ṹ
struct gameAutoplaerInfo
{
	int32_t 	 isopenmoney;
	int32_t      isopendaoju;
	int32_t      insopenzhuang;
	int32_t      isopentuzhi;
	int32_t      ismonyeandzhuang;
};

//ͨ��ִ�нű�
struct KUiPlyerDoscript
{
	int32_t     inKind;
	int32_t     inCurTime;
	int32_t     inCurTimeb;
	int32_t     inCurTimec;
	int32_t     inCurTimed;
	char    szItemInfo[32];
};

//ͨ��ִ�нű�
struct KUiPlyerVipDoscript
{
	int32_t     inKind;
	int32_t     inCurExp;
	int32_t     inCurLucky;
	int32_t     inCurJiHuo;
	int32_t     inCurWuHang;
	char    snCardAcc[32];
	char    snCardPass[32];
};


//ͨ��ִ�нű�
struct KUiPlyerDuanZao
{
	int32_t     inKind;
	int32_t     inCurTimea;
	int32_t     inCurTimeb;
	int32_t     inCurTimec;
	int32_t     inCurTimed;
	//char    szItemInfo[32];
};


//ͨ��ִ�нű�
struct KUiPlyerVipDaZao
{
	int32_t     inKind;
	int32_t     inCurExp;
	int32_t     inCurLucky;
	int32_t     inCurJiHuo;
	int32_t     inCurWuHang;
	char    snCardAcc[32];
	char    snCardPass[32];

};

struct	nJinDuClient
{
	int32_t  nTimeVal;
	int32_t	 nMsgidx;
	char szCallFun[16];   //�ص�����
	char szParma[32];     //����
	char szParmb[32];     //����
	char szParmc[32];     //����
};
//----------���Կͻ��˽���
#endif


#endif
