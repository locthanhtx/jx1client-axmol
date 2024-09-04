#ifndef KPROTOCOL_H
#define KPROTOCOL_H
#include "cocos2d.h"
USING_NS_AX;
#include "GameDataDef.h"
#include <string.h>
#include "KProtocolDef.h"
#include "KRelayProtocol.h"

#pragma pack(push, enter_protocol)
#pragma	pack(1)  //4�ֽڶ���

#define	PROTOCOL_MSG_TYPE	BYTE
#define PROTOCOL_MSG_SIZE	(sizeof(PROTOCOL_MSG_TYPE))
#define	MAX_PROTOCOL_NUM	249  //Э����������
#define _NAME_LEN	32

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
typedef struct _GUID {
	uint32_t  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;
typedef struct _GUID GUID;
#endif

////
typedef struct
{
	BYTE	ProtocolType;
	int 	WalkSpeed;
	int  	RunSpeed;
	int 	HelmType;
	int 	ArmorType;
	int 	WeaponType;
	int 	HorseType;
	int 	RankID;
	UINT	ID;
	BYTE	m_btSomeFlag;	// 0x01 PKFlag 0x02 FightModeFlag 0x04 SleepModeFlag
	int		MaskType;	// ���
	int 	PifengType; // ����
	BYTE	m_bBaiTan;	// ban hang
	char	TongName[32]; //���
	int 	m_bFigure;	// �Ƿ�Ӱ���
	BYTE	RankFFID;
	BYTE    AutoplayId;  //�һ�
	BYTE	ExItemID; // hanh trang
	BYTE	ExBoxID; // ruong mo rong
	int 	RankInWorld; // xep hang tin tuc
	int 	Repute; // ����
	int 	PKValue; // pk tin tuc
	int 	ReBorn; // trung sinh tin tuc
	int     mFaction;
	int     mCJtaskID;      //ת��
} PLAYER_SYNC;

struct SFuMoInfo
{
	int nNpxidx;
	int nNpxSetNo;
	int nNpxSkill;
	int nSlevel;
};

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;//
	int 	WalkSpeed;			// �Ƿ�ֻҪ��һ�ݾ͹��ˣ�ֻ�ı�һ������ͬʱ�ı䣩
	int 	RunSpeed;
	int 	HelmType;
	int 	ArmorType;
	int 	WeaponType;
	int 	HorseType;
	int 	RankID;
	BYTE	m_btSomeFlag;	// 0x01 PKFlag 0x02 FightModeFlag 0x04 SleepModeFlag 0x08 TongOpenFlag
	int 	PifengType;//����
	BYTE    nChiBangType;
	BYTE	m_bBaiTan;	// ban hang
	BYTE	RankFFID;
	BYTE    AutoplayId;
	BYTE	ExItemID; // hanh trang
	BYTE	ExBoxID; // ruong mo rong
	int 	RankInWorld; // xep hang tin tuc
	int 	Repute; // danh vong tin tuc
	int 	PKValue; // pk tin tuc
	int 	ReBorn; // trung sinh tin tuc
	int     mFaction;
	BYTE    mIsCreatTongBan;//�Ƿ��Ѿ���ͬ��
	int     mJinMaiBingJia;
	int     mCJtaskID;      // ת��
	int     mZhenYuan;
	int     msPlayerIdx;
	BYTE    nIsTranse;
	BYTE    nIsSerLock;
	BYTE    nVipType;
	BYTE    nIsOpenMapType;
	int     mAttackState;
	int     m_IsInCity;
	int     IsVip;
	int     mIsWarCity;
	int     m_bFigure; //���ְλ
	char    TongName[32];
	UINT	m_LockNpcDwID;//DWORD
	char    ShopName[32];
	BYTE    nCurGsSerIdx;
} PLAYER_NORMAL_SYNC;


typedef struct
{
	BYTE	ProtocolType;
	UINT   ID;//DWORD
	int     m_IsInCity;

} PLAYER_NORMALMAP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT    ID;//
	int     m_Skillid;
	int     m_Time;
} PLAYER_ONESTATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int				int_PW;
} PLAYER_REQUEST_CP_UNLOCK;

typedef struct
{
	BYTE			ProtocolType;
	int             _isMobile; //�Ƿ��ֻ���
	int             _clientVer;//�ͻ��˰汾 1 �й� 2Խ�� ���� Ӣ��
} PLAYER_REQUEST_CP_LOCK;

typedef struct
{
	BYTE			ProtocolType;
} RESET_PASS;

typedef struct
{
	BYTE			ProtocolType;
	char m_szTitle[64];
    char m_SCallback[64];
    char m_SError[16];
	int  m_nNum;
	int  m_MaxLen;	 //��󳤶�
}PLAYER_STRING;
 /*
typedef struct
{
	BYTE			ProtocolType; //stringbox
} STRING_BOX;
 */
typedef struct
{
	BYTE			ProtocolType;
} DATAU_BOX;

typedef struct
{
	BYTE			ProtocolType;
} PKVALUE;

typedef struct
{
	BYTE			ProtocolType;
	int				int_OldPW;
	int				int_NewPW;
} PLAYER_REQUEST_CP_CHANGE;

typedef struct
{
	BYTE			ProtocolType;
	int				int_ResetPW;
} PLAYER_REQUEST_CP_RESET;

typedef struct
{
	BYTE			ProtocolType;
	int				m_nReputeValue;
} REPUTE_VALUE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int				m_nFuYuanValue;
} FUYUAN_VALUE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int				m_nReBornValue;
	int             m_nReBornLevel;		       //��ǰת���ĵȼ�
	int             m_nReBornKeepQpiont;       //������Ǳ�ܵ�
	int	            m_nReBornKeepJpiont;       //�����ļ���
	int             m_nReBornSetSkillMaxLevel; //���ܵȼ���������
	int             m_nReBornSetFanYuMaxVal;   //������������
	int             m_nCurPlyaerLvel;
	int             m_nCurReBornNum;
	int             m_nReBornLifeMaxVal;
} REBORN_VALUE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int				m_nRoleVipValue;
	int             m_nRoleVipLevel;		       //��ǰת���ĵȼ�
	int             m_nRoleVipKeepQpiont;       //������Ǳ�ܵ�
	int	            m_nRoleVipKeepJpiont;       //�����ļ���
	int             m_nRoleVipSetSkillMaxLevel; //���ܵȼ���������
	int             m_nRoleVipSetFanYuMaxVal;   //������������
	int             m_nCurPlyaerLvel;
	int             m_nCurRoleVipNum;
	int             m_nRoleVipLifeMaxVal;
	int             m_nRoleVipDamageMaxVal;
} ROLEVIP_VALUE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;//DWORD
	int		RankFF;
}	RANKFF_SYNC; // danh hieu
/*
typedef struct
{
	BYTE	ProtocolType; //stringbox
	int		nbutton;
} CP_STRING;

typedef struct
{
	BYTE	ProtocolType; //stringbox
	int		nbutton;
} CP_AUTOPLAYOP;

typedef struct
{
	BYTE	ProtocolType; //stringbox
	int		nbutton;
} CP_AUTOPLAYCL;
 */
typedef struct
{
	BYTE	ProtocolType;
	int		nbutton;
	int     nXuHao;
} CP_DATAU;

typedef struct
{
	BYTE	ProtocolType; //pkvalue
	int		nbutton;
} CP_PKVALUE;

typedef struct
{
	BYTE	ProtocolType; //pkvalue
	int		nbutton;
    int     ncishu;
	int     ByteVal;
	int     ByteVala;
	int     ByteValb;
} CP_AUTOPLAY;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	Camp;				// ��Ӫ
	BYTE	CurrentCamp;		// ��ǰ��Ӫ
	BYTE	m_bySeries;			// ����ϵ
	//BYTE 	LifePerCent;		// �����ٷֱ�
	int 	m_btMenuState;		// ��ӡ����׵�״̬
	BYTE	m_Doing;			// ��Ϊ
	BYTE	m_btKind;			// npc����
	int 	MapX;				// λ����Ϣ
	int 	MapY;				// λ����Ϣ
	UINT	ID;					// Npc��ΨһID  DWORD
	int		NpcSettingIdx;		// �ͻ������ڼ��������Դ��������ֵ�趨
	WORD	NpcEnchant;			// ��ǿ��NPC�������bit��ʾ��ǿ���ͣ������Ƿ��ɫ֮����ɼ�ǿ����Ŀ�ڿͻ���ȷ����
	int     Dangge;
	int     Zhongji;
	int     nLifeYu;
	char	m_szName[32];		// ����
} NPC_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;//DWORD
	int 	MapX;
	int 	MapY;
	BYTE	Camp;
	BYTE	nKind;
	BYTE	Doing;
	BYTE	nState;
	int		ASpeed;
	int		CSpeed;
	int     Dangge;
	int     Zhongji;
	BYTE    RankFFID;
	int     AIsLukey;
	int     AIsJinYan;
	int     AIsJinQian;
	int     IsXingYunXing;
	char    nName[32];
	char    _clientName[32]; //�ͻ�����ʾ��˵������
	int     nLifeDamagePerCent;
	int 	mliveTime;
	UINT	nGuiShuDwid;//DWORD
	int     nZhuaVal;
	UINT    nTongBanNum;//DWORD
	int     nAttackFrame;
	int     nCastFrame;
	int     nNpcMaxLife;
	int     nNpcDefen;
	int     nNpcMaxDefen;
	BYTE    nIsSerLock;
	int     nExpPer;
	int     gsLukey;
	int     nMaskType;
	BYTE    nIsSynDir;
	int     nDir;
	BYTE    nIsMove;
	int     nReForm;
	int     nCurrentLifeMax;
	int     nLifeYu;
	int     nNpcTitle;
	int     nCurNpcTitle;
} NPC_NORMAL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	m_dwNpcID;//DWORD
	int 	m_dwMapX;
	int 	m_dwMapY;
	int 	m_wOffX;
	int 	m_wOffY;
	UINT    m_GameLeftTiem;//DWORD
} NPC_PLAYER_TYPE_NORMAL_SYNC;

typedef struct
{
	BYTE	ProtocolType;			// Э������
	int		m_nMissionGroup;		// ����Ӫ
} PLAYER_MISSION_DATA;

typedef struct
{
	BYTE	ProtocolType;
	TMissionLadderSelfInfo SelfData;
	TMissionLadderInfo MissionRank[MISSION_STATNUM]; //680+209=
} PLAYER_MISSION_RANKDATA;

typedef struct
{
	BYTE	    ProtocolType;
	int			m_nSaleType;
	BuySellInfo m_BuySellInfo;
} S2C_SUPERSHOP;


typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;//DWORD
    char	nDeathName[32];

}NPC_SIT_SYNC, NPC_DEATH_SYNC, NPC_REQUEST_COMMAND, NPC_REQUEST_FAIL;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;//DWORD
	BOOL	Rv;
} NPC_REMOVE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;
	char	szMsg[MAX_SENTENCE_LENGTH];
	int		nMsgLen;
}NPC_CHAT_SYNC;


typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;
	int		nMpsX;
	int		nMpsY;
} NPC_WALK_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;
	int 	mType;

} NPC_REVIVE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;
	int		nMpsX;
	int		nMpsY;
} NPC_JUMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;
	int		nMpsX;
	int		nMpsY;
} NPC_RUN_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;
	int		nFrames;
	int		nX;
	int		nY;
} NPC_HURT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;
	BYTE	Camp;
} NPC_CHGCURCAMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;
	BYTE	Camp;
} NPC_CHGCAMP_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	ID;
	int		nSkillID;
	int		nSkillLevel;
	int		nMpsX;
	int		nMpsY;
	int		nSkillEnChance;
	int     nIsEnChance;
	int     nMaxBei;
	int     nWaitTime;
} NPC_SKILL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	int		nSkillID;
	int		nMpsX;
	int		nMpsY;
} NPC_SKILL_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		nMpsX;
	int		nMpsY;
} NPC_WALK_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	ReviveType;
} NPC_REVIVE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		nMpsX;
	int		nMpsY;
	int     nMapID;
} NPC_RUN_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		nShopIndex;
} SALE_BOX_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	int		nNpcId;
} PLAYER_DIALOG_NPC_COMMAND; //������nNpcId�Ի�������

typedef struct
{
	BYTE	ProtocolType;		// Э������
	int		m_nExp;				// ��ǰ����
} PLAYER_EXP_SYNC;				// ���ͬ������

typedef struct
{
	BYTE	ProtocolType;
	uint32_t  nNpcDwid;
} PLAYER_REQ_COMMAND;

typedef struct
{
	BYTE	ProtocolType;		// Э������
} PLAYER_APPLY_CREATE_TEAM;		// �ͻ�����Ҵ������飬�������������

struct PLAYER_SEND_CREATE_TEAM_SUCCESS
{
	BYTE	ProtocolType;		// Э������
	UINT	nTeamServerID;		// �����ڷ������ϵ�Ψһ��ʶ
	PLAYER_SEND_CREATE_TEAM_SUCCESS() {nTeamServerID = -1;}
};	// ������֪ͨ��Ҷ��鴴���ɹ�

typedef struct
{
	BYTE	ProtocolType;		// Э������
	BYTE	m_btErrorID;		// ���鴴�����ɹ�ԭ��0 ͬ�� 1 ��ұ����Ѿ�����ĳһ֧���� 3 ��ǰ���ڲ������״̬
} PLAYER_SEND_CREATE_TEAM_FALSE;// ������֪ͨ�ͻ��˶��鴴�����ɹ�

typedef struct
{
	BYTE	ProtocolType;		// Э������
	UINT	m_dwTarNpcID;		// ��ѯĿ�� npc id
} PLAYER_APPLY_TEAM_INFO;		// �ͻ���������������ѯĳ��npc��������

typedef struct
{
	BYTE	ProtocolType;		// Э������
	int  	m_IsNoRoom;		    // ��ѯĿ�� npc id
} PLAYER_NO_ROOM_INFO;		    // �ͻ���������������������

typedef struct
{
	BYTE	ProtocolType;		// Э������
	UINT   nNpcDwid;
} NPC_SYN_INFO;		            // �ͻ����������Ҫȥ����NPC��Ϣ


typedef struct
{
	BYTE	ProtocolType;		// Э������
} PLAYER_APPLY_TEAM_INFO_FALSE;	// ��������֪�ͻ��������ѯĳ��npc��������ʧ��

typedef struct PLAYER_SEND_TEAM_INFO_DATA
{
	BYTE	ProtocolType;		// Э������
	int		m_nCaptain;			// �ӳ� npc id
	int		m_nMember[MAX_TEAM_MEMBER];	// ���ж�Ա npc id
	UINT	nTeamServerID;		// �����ڷ������ϵ�Ψһ��ʶ
	PLAYER_SEND_TEAM_INFO_DATA() {nTeamServerID = -1;};
} PLAYER_SEND_TEAM_INFO;		// ��������ͻ��˷���ĳ���������Ϣ����

typedef struct PLAYER_SEND_SELF_TEAM_INFO_DATA
{
	BYTE	ProtocolType;							// Э������
	BYTE	m_btState;								// ����״̬
	UINT	m_dwNpcID[MAX_TEAM_MEMBER + 1];			// ÿ����Ա��npc id ���ӳ����ڵ�һλ��
	char	m_szNpcName[MAX_TEAM_MEMBER + 1][32];	// ÿ����Ա�����֣��ӳ����ڵ�һλ��
	UINT	nTeamServerID;							// �����ڷ������ϵ�Ψһ��ʶ
	UINT	m_dwLeadExp;							// ��ҵ�ͳ��������
	WORD	m_btLevel[MAX_TEAM_MEMBER + 1];			// ÿ����Ա�ĵȼ����ӳ����ڵ�һλ��
	PLAYER_SEND_SELF_TEAM_INFO_DATA() {memset(m_szNpcName, 0, 32 * (MAX_TEAM_MEMBER + 1)); nTeamServerID = -1;};
} PLAYER_SEND_SELF_TEAM_INFO;						// ��������ͻ��˷��Ϳͻ�������������Ϣ����

typedef struct
{
	BYTE	ProtocolType;		// Э������
	BYTE	m_btOpenClose;		// �򿪻�ر�
} PLAYER_TEAM_OPEN_CLOSE;		// ����ӳ�����������뿪�š��رն����Ƿ�������ճ�Ա״̬

typedef struct
{
	BYTE	ProtocolType;		// Э������
	UINT	m_dwTarNpcID;		// Ŀ�����ӳ�npc id ���� ������ npc id
} PLAYER_APPLY_ADD_TEAM;		// �����������������ĳ��������߷�������ĳ���ӳ�ת��ĳ����ҵļ�������

typedef struct
{
	BYTE	ProtocolType;		// Э������
	UINT	m_dwNpcID;			// ������������npc id
} PLAYER_ACCEPT_TEAM_MEMBER;	// ���֪ͨ����������ĳ����������

typedef struct PLAYER_TEAM_ADD_MEMBER_DATA
{
	BYTE	ProtocolType;		// Э������
	WORD	m_btLevel;			// �����ߵȼ�
	UINT	m_dwNpcID;			// ������npc id
	char	m_szName[32];		// ����������
	PLAYER_TEAM_ADD_MEMBER_DATA() {memset(m_szName, 0, 32);};
} PLAYER_TEAM_ADD_MEMBER;		// ������֪ͨ�����еĸ���������³�Ա����

typedef struct
{
	BYTE	ProtocolType;		// Э������
} PLAYER_APPLY_LEAVE_TEAM;		// �ͻ�������������

typedef struct
{
	BYTE	ProtocolType;		// Э������
	UINT	m_dwNpcID;			// ���npc id
} PLAYER_LEAVE_TEAM;			// ������֪ͨ����Աĳ�����

typedef struct
{
	BYTE	ProtocolType;		// Э������
	UINT	m_dwNpcID;			// ���npc id
} PLAYER_TEAM_KICK_MEMBER;		// �ӳ��߳�ĳ����Ա

typedef struct
{
	BYTE	ProtocolType;		// Э������
	UINT	m_dwNpcID;			// Ŀ��npc id
} PLAYER_APPLY_TEAM_CHANGE_CAPTAIN;// �ӳ��������������Լ��Ķӳ���ݽ�����Ķ�Ա

typedef struct
{
	BYTE	ProtocolType;		// Э������
	UINT	m_dwCaptainID;		// �¶ӳ�npc id
	UINT	m_dwMemberID;		// �¶�Աnpc id
} PLAYER_TEAM_CHANGE_CAPTAIN;	// ������֪ͨ����Ա�����ӳ�

typedef struct
{
	BYTE	ProtocolType;		// Э������
} PLAYER_APPLY_TEAM_DISMISS;	// ������������ɢ����

typedef struct
{
	BYTE	ProtocolType;		// Э������
	BYTE	m_btPKFlag;			// pk ����
} PLAYER_SET_PK;				// �����������򿪡��ر�PK

typedef struct
{
	BYTE	ProtocolType;			// Э������
	BYTE	m_btCamp;				// ����Ӫ
	BYTE	m_btCurFaction;			// ��ǰ����
	BYTE	m_btFirstFaction;		// �״μ�������
	int		m_nAddTimes;			// �������ɴ���
} PLAYER_FACTION_DATA;				// �����������ͻ���������Ϣ

typedef struct
{
	BYTE	ProtocolType;			// Э������
} PLAYER_LEAVE_FACTION;				// ������֪ͨ����뿪����

typedef struct
{
	BYTE	ProtocolType;			// Э������
	BYTE	m_btCurFactionID;		// ��ǰ����id
	WORD	m_btLevel;				// ���ܿ��ŵȼ�
} PLAYER_FACTION_SKILL_LEVEL;		// ������֪ͨ��ҿ��ŵ�ǰ���ɼ��ܵ�ĳ���ȼ�

typedef struct
{
	BYTE	ProtocolType;			// Э������
} PLAYER_APPLY_FACTION_DATA;		// �ͻ������������������

typedef struct PLAYER_SEND_CHAT_DATA_COMMAND
{
	BYTE	ProtocolType;		// Э������
	WORD	m_wLength;
	BYTE	m_btCurChannel;		// ��ǰ����Ƶ��
	BYTE	m_btType;			// MSG_G_CHAT �� MSG_G_CMD �򡭡�
	BYTE	m_btChatPrefixLen;	// ��ʽ�����ַ�����
	WORD	m_wSentenceLen;		// ������䳤��
	UINT	m_dwTargetID;		// ������� id
	int		m_nTargetIdx;		// ��������ڷ������˵� idx
	char	m_szSentence[MAX_SENTENCE_LENGTH + CHAT_MSG_PREFIX_MAX_LEN];	// �����������
	PLAYER_SEND_CHAT_DATA_COMMAND() {memset(m_szSentence, 0, sizeof(m_szSentence));};
} PLAYER_SEND_CHAT_COMMAND;		// �ͻ����������ݷ��͸�������

typedef struct PLAYER_SEND_CHAT_DATA_SYNC
{
	BYTE	ProtocolType;		// Э������
	WORD	m_wLength;
	BYTE	m_btCurChannel;		// ��ǰ����״̬
	BYTE	m_btNameLen;		// ���ֳ���
	BYTE	m_btChatPrefixLen;	// �����ַ�����
	WORD	m_wSentenceLen;		// ������䳤��
	UINT	m_dwSourceID;		//
	char	m_szSentence[32 + CHAT_MSG_PREFIX_MAX_LEN + MAX_SENTENCE_LENGTH];	// �����������
	PLAYER_SEND_CHAT_DATA_SYNC() { memset(m_szSentence, 0, sizeof(m_szSentence)); };
} PLAYER_SEND_CHAT_SYNC;		// �ͻ����������ݷ��͸�������

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btState;
	int		m_nID;
	int		m_nDataID;
	int		m_nXpos;
	int		m_nYpos;
	int		m_nMoneyNum;
	int		m_nItemID;
	BYTE	m_btDir;
	BYTE	m_btItemWidth;
	WORD	m_wCurFrame;
	BYTE	m_btItemHeight;
	int 	m_btColorID;
	BYTE	m_btFlag;
	int     m_Genre;
	int     m_DetailType;
	int     m_ParticularType;
	int     m_GoldId;
	int     m_ItemLevel;
	int     m_StackNum;
	char	m_szName[FILE_NAME_LENGTH];
} OBJ_ADD_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	//WORD    m_wLength;
	int		m_nID;
	//int		m_nDataID;
	//int		m_nItemID;
	char	_clientOgjName[FILE_NAME_LENGTH];                   //���ڷ��������
} OBJ_ADD_SYNC_VN;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btState;
	int		m_nID;
} OBJ_SYNC_STATE;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btDir;
	int		m_nID;
} OBJ_SYNC_DIR;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nID;
	BYTE	m_btSoundFlag;
} OBJ_SYNC_REMOVE;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nID;
	int		m_nTarX;
	int		m_nTarY;
} OBJ_SYNC_TRAP_ACT;

typedef struct
{
	BYTE	ProtocolType;
	int		m_nID;
} OBJ_CLIENT_SYNC_ADD;

typedef struct
{
	BYTE	ProtocolType;		// Э������
	UINT	m_dwLeadExp;		// ͳ��������ֵ
} PLAYER_LEAD_EXP_SYNC;			// ͬ��ͳ��������ֵ

typedef struct
{
	BYTE	ProtocolType;		// Э������
	WORD  	m_btLevel;			// ��ǰ�ȼ�
	int		m_nExp;				// ��ǰ����
	int		m_nAttributePoint;	// ʣ�����Ե�
	int		m_nSkillPoint;		// ʣ�༼�ܵ�
	UINT	m_nBaseLifeMax;		// ��ǰ�������ֵ
	UINT	m_nBaseStaminaMax;	// ��ǰ�������ֵ
	UINT	m_nBaseManaMax;		// ��ǰ�������ֵ
} PLAYER_LEVEL_UP_SYNC;			// �������

typedef struct
{
	BYTE	ProtocolType;		// Э������
	WORD	m_btLevel;			// ��ǰ�ȼ�
	UINT	m_dwTeammateID;		// ���� npc id
} PLAYER_TEAMMATE_LEVEL_SYNC;	// ���������ʱ��֪ͨ����

typedef struct
{
	BYTE	ProtocolType;		// Э������
	BYTE	m_btAttribute;		// ����(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	int		m_nAddNo;			// �ӵĵ���
} PLAYER_ADD_BASE_ATTRIBUTE_COMMAND;	// �����ӻ������Ե�

typedef struct
{
	BYTE	ProtocolType;		// Э������
	int		m_nSkillID;			// ����id
	int		m_nAddPoint;		// Ҫ�ӵĵ���
} PLAYER_ADD_SKILL_POINT_COMMAND;// �����������ĳ�����ܵĵ���

typedef struct
{
	BYTE	ProtocolType;		// Э������
	BYTE	m_btAttribute;		// ����(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
	int		m_nBasePoint;		// ��������
	int		m_nCurPoint;		// ��ǰ����
	int		m_nLeavePoint;		// ʣ��δ�������Ե�
} PLAYER_ATTRIBUTE_SYNC;		// ���ͬ�����Ե�

typedef struct
{
	BYTE	ProtocolType;		// Э������
	int		m_nSkillID;			// ����id
	int		m_nSkillLevel;		// ���ܵȼ�
	int		m_nLeavePoint;		// ʣ��δ���似�ܵ�
	int		m_nAddPoint;		// װ����ӵļ��ܵ�
	int     m_Type;
} PLAYER_SKILL_LEVEL_SYNC;		// ���ͬ�����ܵ�


typedef struct
{
	BYTE	ProtocolType;		// Э������
	BOOL	m_nIsOpen;			// �Ƿ���
	int		m_nDianShu;		    // ����
	int		m_nYinL;		    // ��������
	int		m_nJinBi;		    // ������
	int     m_Type;             // ˦������
	int     m_Beilv;
} PLAYER_SYNCALLBACK_GUZI;

typedef struct
{
	BYTE	ProtocolType;		// Э������
	int		m_nItemID;			// ��Ʒid
	int		m_nSourcePos;		// ��Դλ��
	int		m_nTargetPos;		// Ŀ��λ��
} PLAYER_EQUIP_ITEM_COMMAND;	// �������Ҽ����ʹ����Ʒ(װ��)

typedef struct
{
	BYTE	ProtocolType;		// Э������
	BYTE	m_btPlace;			// ҩƷλ��
	BYTE	m_btX;				// ҩƷ��������λ��
	BYTE	m_btY;				// ҩƷ��������λ��
	int		m_nItemID;			// ��Ʒid
} PLAYER_EAT_ITEM_COMMAND;		// �������Ҽ����ʹ����Ʒ(��ҩ)

typedef struct
{
	BYTE	ProtocolType;		// Э������
	BYTE	m_btPlace;			// ҩƷλ��
	BYTE	m_btX;				// ҩƷλ��
	BYTE	m_btY;				// ҩƷλ��
	uint32_t m_nItemID;		// ��Ʒid
	int     m_destRoom;         // Ŀ������
	int     m_ItemWidth;
	int     m_ItemHigth;
} PLAYER_AUTOMUVE_ITEM_COMMAND;		// �������Ҽ����ʹ����Ʒ(��ҩ)

typedef struct
{
	BYTE	ProtocolType;		// Э������
	int		m_nObjID;			// �������obj��id
	int 	m_btPosType;		// λ������
	int 	m_btPosX;			// ���� x
	int 	m_btPosY;			// ���� y
} PLAYER_PICKUP_ITEM_COMMAND;	// ��һ����Ʒ���������ͼ�ϵ�obj��     int		m_nColorID;         //������ɫ 0��ɫ  1��ɫ  3��ɫ 4��ɫ
/*
struct sMagInfo
{
	int nMagType;
	int nMagVala;  //6��v0 ֵ
	int nMagValb;  //6��v1 ֵ
	int nMagValc;  //6��v2 ֵ
};

typedef struct
{
	BYTE			ProtocolType;		// Э������
	BYTE            nRoomType;
    int             nItemIdx;
	int             nOldx;
	int             nOldy;
	int             nCurX;
	int             nCurY;

}ITEM_SYNC_POSITIONS;
*/
typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_ID;				// ��Ʒ��ID
	int 			m_Genre;			// ��Ʒ������
	int			    m_Detail;			// ��Ʒ�����
	int				m_Particur;			// ��Ʒ����ϸ���
	int 			m_Series; 			// ��Ʒ������
	int 			m_Level;			// ��Ʒ�ĵȼ�
	int 			m_btPlace;			// ����������
	int			    m_btX;				// ����
	int 			m_btY;				// ����
	int				m_Luck;				// MF
	int				m_MagicLevel[6];	// ħ�����ɲ���
	int				m_RongMagicLev[6];	// �������ɲ���
//	int		        m_BaoMagicLev[6];
	int             m_JBLevel[7];
	WORD			m_Version;			// װ���汾
	int 			m_Durability;		// �;ö�
	UINT			m_RandomSeed;		// �������
	int				m_GoldId;			// �ƽ�Id
	int				m_StackNum;
	int 			m_EnChance;
	int 			m_Point;
	KTime			m_Time;
    int 	        m_Rongpiont;//  ����
	int             m_IsBang;
	int             m_IsKuaiJie;
	int             m_IsMagic;
	int             m_SkillType;
	int             m_MagicID;
	char            m_ItmeInfo[516];
	//char            *m_ItmeInfo;
	char            m_WonName[32];
//	sMagInfo        m_MagInfo[6];
	int             m_IsWhere;
	int             m_SyncType;
	int             m_IsCanUse;
	BYTE            m_IsLoign;
	int             m_IsPlatima;
    int             m_UseMap;
	int             m_Res;
	int             m_UseKind;
} ITEM_SYNC;

typedef struct
{
   BYTE			ProtocolType;		// Э������
   int          nShopIdx; //ROWS
   int          nBuyIdx;  //COLS
   char         m_ItmeInfo[516];

}ITEM_SYNC_SHOP;


typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_ID;				// ��Ʒ��ID
	int             m_model;
	int             m_dx;
	int             m_dy;
} ITEM_REMOVE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_ID;				// ��Ʒ��ID
	int             m_ClinetKind;       // �ͻ��˵�����
} PLAYER_SELL_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	BYTE			m_BuyIdx;			// ��ڼ�������
	BYTE			m_Place;			// ���������ĸ��ط�
	BYTE			m_X;				// ����X
	BYTE			m_Y;				// ����Y
	int             m_shuishou;
	int             m_CityMenber;
    int             m_MapID;
	int             m_ItemNum;
} PLAYER_BUY_ITEM_COMMAND;


typedef struct
{
	BYTE			ProtocolType;		// Э������
	int 			m_TaskId;			// ����ID��
	UINT           m_pDwid;
	int             m_TaskVal;
}PLAYER_SET_TASKVAL_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		nKind;
	int		nVala;
	int		nValb;
	int		nValc;
} CLIENT_RUN_SCRIPT;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int 			m_TaskId;			// ����ID��
	UINT           m_pDwid;

}PLAYER_GET_TASKVAL_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int 			m_TaskId;			// ����ID��
	UINT           m_pDwid;
	int             m_TaskVal;

}PLAYER_GETBACK_TASKVAL;


typedef struct
{
	BYTE			ProtocolType;		// Э������
	int             m_TaskVala;
	int             m_TaskValb;
	int             m_TaskValc;
	int             m_TaskVald;
	int             m_TaskVale;
	int             m_TaskValf;
	int             m_TaskValg;
	int             m_TaskValh;

}PLAYER_JINMAI_GETBACK;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT           m_NpcDwid;
	int             m_TaskVala;
	int             m_TaskValb;
	int             m_TaskValc;
	int             m_TaskVald;
	int             m_TaskVale;
	int             m_TaskValf;
	int             m_TaskValg;
	int             m_TaskValh;
	int             m_TaskVali;
	int             m_TaskValj;

}NPC_SYN_INFO_GETBACK;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_nMoney1;			// װ����
	int				m_nMoney2;			// ������
	int				m_nMoney3;			// ������
} PLAYER_FS_MONEY_SYNC;					// ������֪ͨ�ͻ���Ǯ������

/*typedef struct
{
	BYTE			ProtocolType;		// Э������
	BYTE            m_CurDir;

}NPC_DIR_SET; */

typedef struct
{
	BYTE			ProtocolType;
	int				m_nXu;
} PLAYER_FS_XU_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int			    m_btDownPos;
	int			    m_btDownX;
	int			    m_btDownY;
	int			    m_btUpPos;
	int			    m_btUpX;
	int			    m_btUpY;
	int             m_DownRongqi;
	int             m_UpRongqi;
	int             m_IsComp;
} PLAYER_MOVE_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int 			m_btDownPos;
	int			    m_btDownX;
	int			    m_btDownY;
	int			    m_btUpPos;
	int			    m_btUpX;
	int			    m_btUpY;
	int             m_DownRongqi;
	int             m_UpRongqi;
	int             m_IsMianBan;
} PLAYER_MOVE_ITEM_SYNC;

// s2c_ItemAutoMove
typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT            m_ItenDwid;
	int 			m_btSrcPos;
	int			    m_btSrcX;
	int			    m_btSrcY;
	int			    m_btDestPos;
	int			    m_btDestX;
	int		        m_btDestY;
	int             m_DisRongqi;
    int             m_SisRongqi;
} ITEM_AUTO_MOVE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT            m_ItemDwId;
	int			    m_btSrcX;
	int			    m_btSrcY;
	int			    m_btDestX;
	int		        m_btDestY;
	int             m_Dplace;
    int             m_Splace;
	int             m_Kind;
}PLAYER_AUTOCALLBACK_EQUIP;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int             m_Kind;
	UINT           m_ItemDwidx;
	BYTE            m_IsAuToFenJie;
	int             m_isAttack;
} PLAYER_THROW_AWAY_ITEM_COMMAND;

typedef struct
{
	BYTE		ProtocolType;		// Э������
	int 			m_shLife;
	int 			m_shStamina;
	int 			m_shMana;
	//short	    m_shAngry;          //��ǰŭֵ
	BYTE		   m_btTeamData;
	int 			m_shLifeMax;
	int 			m_shStaminaMax;
	int 			m_shManaMax;
    int 	     m_shAngryMax;
	BYTE         m_nForbiddenFlag;
} CURPLAYER_NORMAL_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	// npc����
	UINT			m_dwID;				// Npc��ID
	WORD  			m_btLevel;			// Npc�ĵȼ�
	BYTE			m_btSex;			// �Ա�
	BYTE			m_btKind;			// Npc������
	BYTE			m_btSeries;			// Npc������ϵ
	UINT			m_wLifeMax;			// Npc���������
	UINT			m_wStaminaMax;		// Npc���������
	UINT			m_wManaMax;			// Npc���������
	int				m_HeadImage;
	// player ����
	WORD			m_wAttributePoint;	// δ�������Ե�
	WORD			m_wSkillPoint;		// δ���似�ܵ�
	WORD			m_wStrength;		// ��ҵĻ������������������˺���
	WORD			m_wDexterity;		// ��ҵĻ������ݣ��������С�������
	WORD			m_wVitality;		// ��ҵĻ�������������������������
	WORD			m_wEngergy;			// ��ҵĻ�������������������
	WORD			m_wLucky;			// ��ҵĻ�������ֵ
	int 			m_nExp;				// ��ǰ����ֵ(��ǰ�ȼ���npc����)
	UINT			m_dwLeadExp;		// ͳ��������ֵ
	BYTE 			m_btCUnlocked;      //�Ƿ�����

	// ����
	BYTE			m_btCurFaction;		// ��ǰ����
	BYTE			m_btFirstFaction;	// ��һ�μ�������ĸ�����
	int				m_nFactionAddTimes;	// ����������ɵ��ܴ���

	// ����
	WORD			m_wWorldStat;		// ��������
	WORD			m_wSectStat;		// ��������

	// Ǯ
	int				m_nMoney1;
	int				m_nMoney2;
	int				m_nXu;

	int				m_nRequte;
	int				m_nFuYuan;
	int             m_wNuqiMax;
    //int             m_nForbiddenFlag;      //�Ƿ������
} CURPLAYER_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	BOOL            m_IsLogin;
	uint32_t    m_clientKey;
}CLIENT_SYNC_END;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int iid;        //���idx 1��ʼ
	int iequipgen;  //
	int idetailtype;
	int iparticulartype;
	char itemName[64];//��Ʒ������
	int ilevel;     //�ȼ�
	int iseries;    //����
	UINT irandseed; //�������
	int iparam2;    //��װ
	int iparam3;
	int iparam5;
	int iparam4;
	int iparam6;
	int iparam1;
	int iparamr2;   //����
	int iparamr3;
	int iparamr5;
	int iparamr4;
	int iparamr6;
	int iparamr1;
	int ilucky;   //װ��������ֵ
	int igoldid;  //�Ƿ�ƽ�
	int	istacknum;//��������
	int ienchance;//�ӳ�
	BYTE ipoint;  //�Ƿ���װ 7Ϊ����װ 8 Ϊ����ƽ�   1-6 Ϊ��װ
	UINT addtime; //��ʼʱ��
	UINT endtime; //����ʱ��
	BYTE iPriceKind;   //��������
	int  iSellPrice;   //�����۸�
	BYTE iRongpiont;   //�Ƿ�����
	char iWonName[32]; //������
	char iaccName[32]; //�ʺ�
	int iswhere;    //�Ƿ�ϴ��װ
	int isplatina;  //�Ƿ�׽�
	UINT isSellOut; //�Ƿ��Ѿ�����
	int iparamj1;   //(����ʹ��)
	int iparamj2;
	UINT iparamj3;
	int iparamj5;
	int iparamj4;
	int iparamj6;
	int iparamj7;
}ITEMDATA_SYNC_END;


typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;				// Npc��ID
	int				m_nRequte;
	int				m_nFuYuan;
}CURPLAYER_TASKINFO_SYNC;

#define MAX_SCIRPTACTION_BUFFERNUM 1024

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wProtocolLong;
	BYTE	m_nOperateType;				//��������
	BYTE	m_bUIId, m_bOptionNum, m_bParam1, m_bParam2, m_Select;// m_bParam1,����Ϣ�����ֱ�ʶ�����ַ�����ʶ, m_bParam2,�Ƿ����������������ѡ�����
	int		m_nParam;
	int		m_nBufferLen;
	char	m_szSprPath[128];
	char	m_pContent[MAX_SCIRPTACTION_BUFFERNUM];				//�����Ʒ�
} PLAYER_SCRIPTACTION_SYNC;

typedef struct
{
	WORD	SkillId;
	BYTE	SkillLevel;
	BYTE	SkillAdd;
	int     CurSkillExp;
} SKILL_SEND_ALL_SYNC_DATA;

typedef struct
{
	BYTE						ProtocolType;
	WORD						m_wProtocolLong;
	SKILL_SEND_ALL_SYNC_DATA	m_sAllSkill[80];
} SKILL_SEND_ALL_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	WeatherID;
} SYNC_WEATHER;


typedef struct defWORLD_SYNC
{
	BYTE	ProtocolType;
	int		SubWorld;
	int		Region;
	BYTE	Weather;
	UINT	Frame;
	int     SRegion;
	char WarMaster[32];       //����
	char WarTong[32];         //ռ����
	char WarGongTong[32]; //��
	char WarShouTong[32]; //��
	BYTE WarIsWho;
	int  mShuiType;    //˰�յ�����
	BYTE IsWarCity;    // �Ƿ�ռ��
	int WarCityManey;  //���н����ʽ�
	int WarCityJB;     //���н����ʽ�
	int WarCityGX;     //���н��蹱��
	int WPKFlag;       //��ͼ��pkģʽ����
	BOOL m_IsShowLoop;
	int m_GameStat;
} WORLD_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	int		nSelectIndex;
}PLAYER_SELECTUI_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwTakeChannel;	// ����Ƶ��
} CHAT_SET_CHANNEL_COMMAND;				// �趨����Ƶ��

typedef struct
{
	BYTE			ProtocolType;		// Э������
	WORD			m_wLength;
	UINT			m_dwTargetNpcID;	// Ŀ�� npc id
	char			m_szInfo[MAX_SENTENCE_LENGTH];// ���Է��Ļ�
} CHAT_APPLY_ADD_FRIEND_COMMAND;		// ������Ӻ���

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_nSrcPlayerIdx;	// ��Դ player idx
	char			m_szSourceName[32];	// ��Դ�������
	char			m_szInfo[MAX_SENTENCE_LENGTH];// �Է����Ļ�
} CHAT_APPLY_ADD_FRIEND_SYNC;			// ������Ӻ���

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_nTargetPlayerIdx;	// ������player idx
} CHAT_ADD_FRIEND_COMMAND;				// ���ĳ���Ϊ�������

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_nTargetPlayerIdx;	// ���ܾ�player idx
} CHAT_REFUSE_FRIEND_COMMAND;			// �ܾ����ĳ���Ϊ�������

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;				// ����Ӻ��ѵ� id
	int				m_nIdx;				// ����Ӻ����� player �����е�λ��
	char			m_szName[32];		// ����Ӻ��ѵ�����
} CHAT_ADD_FRIEND_SYNC;					// ֪ͨ�ͻ��˳ɹ����һ���������

typedef struct
{
	BYTE			ProtocolType;		// Э������
	WORD			m_wLength;
	char			m_szName[32];		// �ܾ�������
} CHAT_REFUSE_FRIEND_SYNC;				// ֪ͨ�ͻ������������ѵ����뱻�ܾ�

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_nTargetPlayerIdx;	// ���� player idx (һ������Ǵ�player���߻��߻���������)
} CHAT_ADD_FRIEND_FAIL_SYNC;			// ֪ͨ�ͻ�������������ʧ��

typedef struct
{
	BYTE			ProtocolType;	    // c2s_viewequip  �鿴װ����Ϣ
	UINT			m_dwNpcID;
	BYTE			m_bPrcess;
} VIEW_EQUIP_COMMAND;


//�˽ṹ�Ѿ���tagDBSelPlayer�ṹ�滻
/*typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_nSelect;			//
} DB_PLAYERSELECT_COMMAND;*/

/*
 * { Add by liupeng 2003.05.10
 *
 * #pragma pack( push, 1 )
*/

/*
 * Nonstandard extension used : zero-sized array in struct/union
 */
#pragma warning(disable: 4200)

#define KSG_PASSWORD_MAX_SIZE   64

typedef struct tagKSG_PASSWORD
{
    char szPassword[KSG_PASSWORD_MAX_SIZE];    // ���ڲ���MD5���ַ�����������32���ַ�������ĩβ'\0'����Ҫ����33���ռ䣬���ʹ��64
} KSG_PASSWORD;

#define _NAME_LEN	32

struct tagProtoHeader
{
	BYTE	cProtocol;
};

/*struct tagResult : public tagProtoHeader
{
	BYTE	cResult;
}; */

struct tagDBSelPlayer : public tagProtoHeader
{
	char	szRoleName[_NAME_LEN];
};

struct tagDBDelPlayer : public tagProtoHeader
{
	char	        szAccountName[_NAME_LEN];
    KSG_PASSWORD    Password;
	char	        szRoleName[_NAME_LEN];
};

//ɾ�����½���ɫ�ķ�����Ϣ��������
struct tagNewDelRoleResponse : public tagDBSelPlayer
{
	bool	bSucceeded;		//�Ƿ�ɹ�
};

// 2003.05.11
/*struct tagDBSyncPlayerInfo : public tagProtoHeader
{
	size_t	dataLength;
	char	szData[0];
}; */

/*
 * 2003.06.27
 * s2c_gateway_broadcast
*/

#define	AP_WARNING_ALL_PLAYER_QUIT			1
#define	AP_NOTIFY_GAMESERVER_SAFECLOSE		2
#define	AP_NOTIFY_ALL_PLAYER				3
//#define	AP_NOTIFY_WELCOME_MSG				4   //��ӭ��
#define	MAX_GATEWAYBROADCAST_LEN	260
struct tagGatewayBroadCast : public tagProtoHeader
{
	UINT	uCmdType;
	BOOL    iSavePai;
	char	szData[MAX_GATEWAYBROADCAST_LEN];
};

/*
 * 2003.05.22
 * s2c_syncgamesvr_roleinfo_cipher
*/
struct tagGuidableInfo : public tagProtoHeader
{
	GUID guid;
	UINT nExtPoint;			//���õĸ��͵�
	int  nChangePoint;		    //�仯�ĸ��͵�
	UINT nLeftTime;            //
	int  nGameTime;            //�仯��ʱ��
    int   nVipType;
	UINT	datalength;
	char	szData[0];
};

/*
 * c2s_permitplayerlogin
 */
struct tagPermitPlayerLogin : public tagProtoHeader
{
	GUID guid;

	BYTE szRoleName[_NAME_LEN];
	BYTE szAccountName[_NAME_LEN];
	char strRoleNmae[_NAME_LEN];
	/*
	 * Succeeded : true
	 * Failed	 : false
	 */
	bool bPermit;
};

struct tagPermitPlayerExchange
{
	BYTE cProtocol;
	GUID guid;
	UINT dwIp;
	WORD wPort;
	bool bPermit;
};
/*
 * c2s_notifyplayerlogin
 */
struct tagNotifyPlayerLogin : public tagPermitPlayerLogin
{
	UINT			nIPAddr;
	unsigned short	nPort;
};

/*
 * s2c_querymapinfo
 */
struct tagQueryMapInfo : public tagProtoHeader
{
};

/*
 * s2c_querygameserverinfo
 */
struct tagQueryGameSvrInfo : public tagProtoHeader
{
};

/*
 * s2c_notifysvrip
 */
struct tagNotifySvrIp : public tagProtoHeader
{
	WORD	pckgID;

	BYTE	cIPType;
	UINT	dwMapID;

	UINT	dwSvrIP;
};

/*
 * s2c_notifyplayerexchange
 */
struct tagNotifyPlayerExchange : public tagProtoHeader
{
	GUID			guid;
	UINT			nIPAddr;
	unsigned short	nPort;
};

/*
 * c2s_requestsvrip
 */

/*
 * BYTE	cIPType
 */
#define INTRANER_IP	0
#define INTERNET_IP 1

struct tagRequestSvrIp : public tagProtoHeader
{
	WORD	pckgID;

	BYTE	cIPType;
	UINT	dwMapID;
};

/*
 * c2c_notifyexchange
 */
struct tagSearchWay : public tagProtoHeader
{
	int		lnID;
	int		nIndex;
	UINT	dwPlayerID;
};

/*
 * c2s_updatemapinfo
 */
struct tagUpdateMapID : public tagProtoHeader
{
	/*
	 * For example : Are your clear older information when it
	 *		update local informatin
	 */
	/*BYTE cReserve;
	BYTE cMapCount;
	BYTE szMapID[0];*/

	UINT cMapCount;
};

/*
 * c2s_updategameserverinfo
 */
struct tagGameSvrInfo : public tagProtoHeader
{
	UINT			nIPAddr_Intraner;
	UINT			nIPAddr_Internet;
	unsigned short	nPort;
	WORD			wCapability;
};

/*
 * s2c_identitymapping
 */
/*struct tagIdentityMapping : public tagGameSvrInfo
{
	GUID guid;
}; */

/*
 * c2s_logiclogin
 * s2c_gmgateway2relaysvr
 * s2c_gmnotify
 */
struct tagLogicLogin : public tagProtoHeader
{
	GUID guid;
};

/*
 * s2c_logiclogout
 */
struct tagLogicLogout : public tagProtoHeader
{
	BYTE szRoleName[_NAME_LEN];
};

/*
 * c2s_registeraccount
 */
struct tagRegisterAccount : public tagProtoHeader
{
	BYTE szAccountName[_NAME_LEN];
};

/*
 * c2s_entergame
 */
struct tagEnterGame : public tagProtoHeader
{
	/*
	 * Succeeded : content is account name
	 * Failed	 : content is null
	 */

	BYTE szAccountName[_NAME_LEN];  //�˺���
};

struct tagEnterGame2 : public EXTEND_HEADER
{
	char szAccountName[_NAME_LEN];
	char szCharacterName[_NAME_LEN];
	UINT	dwNameID;
	uint32_t	lnID;
	uint32_t    nSelSerVer;
};

/*
 * c2s_leavegame
 */

/*
 * BYTE cCmdType
 */
#define NORMAL_LEAVEGAME	0x0		// lock account
#define HOLDACC_LEAVEGAME	0x1A	// clear resource but don't to unlock account

struct tagLeaveGame : public tagProtoHeader
{
	BYTE cCmdType;
	int nExtPoint;        //��Ҫ�ı����չ�� ��Ϊ�� ����Ϊ��
	int nGameExtTime;     //��Ҫ�ı��ʱ�� ��Ϊ�� ����Ϊ��
	/*
	 * Succeeded : content is account name
	 * Failed	 : content is null
	 */
	char nClientIp[KSG_PASSWORD_MAX_SIZE];
	char szAccountName[_NAME_LEN];
};

struct tagMsgInGame : public tagProtoHeader
{
	BYTE cCmdType;
	int nSerIdx;        //���������
	int nMgsIdx;
	int nValIdx;
};

struct tagLeaveGame2 : public EXTEND_HEADER
{
	BYTE cCmdType;
	char szAccountName[_NAME_LEN];
	UINT nSelServer;   //�ĸ�������������˳���Ϸ��
};
/*
*  c2s_registerfamily
*/
/*struct tagRegisterFamily : public tagProtoHeader
{
	BYTE bRegister;		//1 is Register, 0 is unRegister
	BYTE nFamily;
	BYTE RelayMethod;
};*/

/*
 * c2s_gmsvr2gateway_saverole
 */
/*struct tagGS2GWSaveRole : public tagProtoHeader
{
	size_t	datalength;
	BYTE	szData[0];
}; */

/*
 * #pragma pack( pop )
 *
 * } End of the struct define
 */

typedef struct
{
	char	szName[32];
	BYTE	Sex;
	BYTE	Series;
//	BYTE	HelmType;
//	BYTE	ArmorType;
//	BYTE	WeaponType;
	BYTE	Level;
} RoleBaseInfo/* client */, S3DBI_RoleBaseInfo /* server */;

typedef struct
{
	BYTE				ProtocolType;
	RoleBaseInfo		m_RoleList[MAX_PLAYER_IN_ACCOUNT];
} ROLE_LIST_SYNC;

//����RoleDBManager/kroledbheader.h
//�����滻�����ROLE_LIST_SYNC,ROLE_LIST_SYNC�ṹ������Ҫ��
struct TProcessData
{
	unsigned char	nProtoId;
	uint32_t	nDataLen;//TRoleNetMsgʱ��ʾ��Block��ʵ�����ݳ���,TProcessDataʱ��ʾStream��ʵ�����ݳ��� size_t
	uint32_t	ulIdentity;//long
	bool			bLeave;
	UINT            uSelServer;     //��ǰ����½�ķ�����
	char			pDataBuffer[1]; //ʵ�ʵ�����
};


struct TCheckNameData
{
	unsigned char	nProtoId;
	UINT			nDataLen;//TRoleNetMsgʱ��ʾ��Block��ʵ�����ݳ���,TProcessDataʱ��ʾStream��ʵ�����ݳ���
	uint32_t	ulIdentity;
	char            nAccName[32];   //�˺���
	char            nRoleName[32];
};


struct tagRoleEnterGame
{
	BYTE			ProtocolType;
	bool			bLock;
	char			Name[_NAME_LEN];
};

//�½���ɫ����Ϣ�ṹ
//ע�ͣ��½�����Ϣc2s_newplayer�����͵Ĳ���ΪTProcessData�ṹ���������ݣ�����TProcessData::pDataBufferҪ��չΪNEW_PLAYER_COMMAND
struct NEW_PLAYER_COMMAND
{
	BYTE			m_btRoleNo;			// ��ɫ���
	BYTE			m_btSeries;			// ����ϵ
	unsigned short	m_NativePlaceId;	// ������ID
	char			m_szName[32];		// ����
};


typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;				// ���� id
	int				m_nPlayerIdx;		// ���� player index
} CHAT_LOGIN_FRIEND_NONAME_SYNC;		// ��ҵ�¼ʱ�����������������ݣ��������֣�

typedef struct
{
	BYTE			ProtocolType;		// Э������
	WORD			m_wLength;
	UINT			m_dwID;				// ���� id
	int				m_nPlayerIdx;		// ���� player index
	char			m_szName[32];		// ��������
} CHAT_LOGIN_FRIEND_NAME_SYNC;			// ��ҵ�¼ʱ�����������������ݣ������֣�

typedef struct
{
	BYTE			ProtocolType;		// Э������
} CHAT_APPLY_RESEND_ALL_FRIEND_NAME_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;				// ���� id
} CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND;	// ����õ����ĳ��������ѵ���������

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;				// ���� id
	int				m_nPlayerIdx;		// ���� player index
	char			m_szName[32];		// ��������
} CHAT_ONE_FRIEND_DATA_SYNC;			// �������ĳһ������������ݣ������֣�

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;				// ���� id
	int				m_nPlayerIdx;		// ���� player index
} CHAT_FRIEND_ONLINE_SYNC;				// ֪ͨ�ͻ����к�������

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;				// ��ɾ��id
} CHAT_DELETE_FRIEND_COMMAND;			// ɾ��ĳ���������

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;				// ɾ��id
} CHAT_DELETE_FRIEND_SYNC;				// ��ĳ���������ɾ��

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;				// ��ɾ��id
} CHAT_REDELETE_FRIEND_COMMAND;			// ɾ��ĳ���������
/*
typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_LogoutType;
} LOGOUT_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			szAccName[32];
} LOGIN_COMMAND;  */

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;				// ���� id
} CHAT_FRIEND_OFFLINE_SYNC;				// ֪ͨ�ͻ����к�������

typedef struct
{
	BYTE			ProtocolType;		// Э������
	WORD			m_wLength;
	char			m_szSentence[MAX_SENTENCE_LENGTH];
} TRADE_APPLY_OPEN_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
} TRADE_APPLY_CLOSE_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	BYTE			m_btState;			// if == 0 close if == 1 open if == 2 trading
	UINT			m_dwNpcID;			// ����ǿ�ʼ���ף��Է��� npc id
} TRADE_CHANGE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	WORD			m_wLength;
	UINT			m_dwID;
	BYTE			m_btState;
	char			m_szSentence[MAX_SENTENCE_LENGTH];
} NPC_SET_MENU_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwID;
} TRADE_APPLY_START_COMMAND;

// ������ת����������
typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_nDestIdx;			// �������ڷ������˵�player idx
	UINT			m_dwNpcId;			// �����ߵ� npc id
} TRADE_APPLY_START_SYNC;

// ���ܻ�ܾ����˵Ľ�������
typedef struct
{
	BYTE			ProtocolType;		// Э������
	BYTE			m_bDecision;		// ͬ�� 1 ��ͬ�� 0
	int				m_nDestIdx;			// ���׶Է��ڷ������˵�player idx
} TRADE_REPLY_START_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_nMoney;
} TRADE_MOVE_MONEY_COMMAND;		// c2s_trademovemoney

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_nMoney;
} TRADE_MONEY_SYNC;				// s2c_trademoneysync

typedef struct
{
	BYTE			ProtocolType;		// Э������
	BYTE			m_btDecision;		// ȷ������ 1  �˳����� 0  ȡ��ȷ�� 4  �������� 2  ȡ������ 3
} TRADE_DECISION_COMMAND;				// ����ִ�л�ȡ�� c2s_tradedecision


typedef struct
{
	BYTE			ProtocolType;		// Э������
	bool			m_isout;		    // �Ƿ��˳�
} KICK_OUT_GAME_COMMAND;				// ����ִ�л�ȡ�� c2s_tradedecision


typedef struct
{
	BYTE			ProtocolType;		// Э������
	BYTE			m_btDecision;		// ����ok 1  ����ȡ�� 0  ���� 2  ȡ������ 3
} TRADE_DECISION_SYNC;					// s2c_tradedecision

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_byDir;			// ȡǮ�ķ���0�棬1ȡ��
	UINT			m_dwMoney;			// Ǯ��
} STORE_MONEY_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	WORD			m_wLength;			// ����
	BYTE			m_btError;			// ��������	0 �Է��ر��˴�Ƶ����1 �Ҳ����Է�
	char			m_szName[32];		// �Է�����
} CHAT_SCREENSINGLE_ERROR_SYNC;

typedef struct
{
	BYTE		ProtocolType;		// Э������
	int			m_btStateInfo[MAX_NPC_RECORDER_STATE];  //BYTE
	UINT			m_ID;				// Npc��GID
}	NPC_SYNC_STATEINFO;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_dwNpcID;
} TEAM_INVITE_ADD_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	WORD			m_wLength;			// ����
	int				m_nIdx;             //���NPC����
	char			m_szName[32];
} TEAM_INVITE_ADD_SYNC;

typedef struct
{
	BYTE			ProtocolType;		//
	int				m_nAuraSkill;
} SKILL_CHANGEAURASKILL_COMMAND;		//�����⻷����

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btResult;
	int				m_nIndex;
} TEAM_REPLY_INVITE_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btSelfLock;
	BYTE			m_btDestLock;
	BYTE			m_btSelfOk;
	BYTE			m_btDestOk;
} TRADE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	UINT			m_dwSkillID;		// ����
	int				m_nLevel;           //�ȼ�
	int				m_nTime;			// ʱ��
	int             m_IsEuq;
	KMagicAttrib	m_MagicAttrib[MAX_SKILL_STATE];
} STATE_EFFECT_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	UINT			m_dwSkillID;		// ����
	int				m_nLevel;           //�ȼ�
	int				m_nTime;			// ʱ��
	int             m_IsEuq;
}STATE_NODATA_EFFECT_SYNC;


typedef struct
{
	BYTE			ProtocolType;
	UINT			m_dwTime;
} PING_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	UINT			m_dwReplyServerTime;
	UINT			m_dwClientTime;
} PING_CLIENTREPLY_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btSitFlag;
} NPC_SIT_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
} NPC_RIDE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	int		nMapID;
	int		nMpsX;
	int		nMpsY;
	int     nMap;
	int     nDwid;
}NPC_SUNYI_COMMAND; //˲��Э��


typedef struct
{
	BYTE	ProtocolType;
	int     nmapid ;
    int     ndianshu;
	int     nNetConnectIdx;
    int     nZuoWeiHao;
}CP_GUZI;


typedef struct
{
	BYTE	ProtocolType;
	int     nmapid ;
    int     nFarmsA ;
    int     nFarmsB;
}CP_GUZI_FARMS;


typedef struct
{
	BYTE	ProtocolType;
	int     nJinBi ;
}CP_ADD_JINBI;

typedef struct
{
	BYTE	ProtocolType;
	char m_PLname[32];
    int  m_ItemDwid;
	int  m_bLPrcess;
}VIEW_LIANJIE_COMMAND;


typedef struct
{
	BYTE	ProtocolType;
	int     ntype;
    int     nlevel;
}CP_JINGMAI;


typedef struct
{
	BYTE	ProtocolType;
    int  GuaType;
}NPC_GUAFORBIT_COMMAND;


typedef struct
{
	BYTE	ProtocolType;
	int     m_nLeftskill;
    INT     m_Type;
}SKILL_LEFT_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int				nMpsX;
	int				nMpsY;
} NPC_JUMP_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	int				m_dwRegionID;
	int				m_nObjID;
} OBJ_MOUSE_CLICK_SYNC;

typedef struct tagSHOW_MSG_SYNC
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	WORD			m_wMsgID;
	//LPVOID			m_lpBuf;
	DWORD           m_lpBuf;
	char            m_Mmsgs[125];
	//tagSHOW_MSG_SYNC() {m_lpBuf = NULL;};
	//~tagSHOW_MSG_SYNC() {Release();}
   //void	Release() {if (m_lpBuf) delete []m_lpBuf; m_lpBuf = NULL;}
} SHOW_MSG_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btFlag;
} PK_APPLY_NORMAL_FLAG_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	BYTE			m_btFlag;
} PK_NORMAL_FLAG_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	UINT			m_dwNpcID;
} PK_APPLY_ENMITY_COMMAND;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	BYTE			m_btState;
	UINT			m_dwNpcID;
	char			m_szName[32];
} PK_ENMITY_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	WORD			m_wLength;
	BYTE			m_btState;
	UINT			m_dwNpcID;
	char			m_szName[32];
} PK_EXERCISE_STATE_SYNC;

typedef struct
{
	BYTE			ProtocolType;
	int				m_nPKValue;
} PK_VALUE_SYNC;
//�鿴����װ��
struct SViewItemInfo
{
	int				m_ID;				// ��Ʒ��ID
	BYTE			m_Genre;			// ��Ʒ������
	BYTE 			m_Detail;			// ��Ʒ�����
	BYTE 			m_Particur;			// ��Ʒ����ϸ���
	BYTE			m_Series; 			// ��Ʒ������
	BYTE 			m_Level;			// ��Ʒ�ĵȼ�
	int  			m_Luck;				// MF
	int  			m_MagicLevel[6];	// ħ�����ɲ���
    //int  			m_RongMagicLev[6];	// �������ɲ���
	//int		    m_BaoMagicLev[6];
	//int           m_JBLevel[7];
	WORD			m_Version;			// װ���汾
	//BYTE 			m_Durability;		// �;ö�
	UINT			m_RandomSeed;		// �������
	UINT  			m_GoldId;			// �ƽ�Id
	UINT  			m_IsBaiJin;
	BYTE    		m_EnChance;
	BYTE  			m_Point;
	KviewTime	    m_Time;
	//int             m_Year;
    BYTE 	        m_Rongpiont;//  ����
	//BYTE            m_IsBang;
	//BYTE             m_IsKuaiJie;
	//BYTE             m_IsMagic;
	//BYTE             m_SkillType;
	//BYTE             m_MagicID;
	//int              m_IsWhere;
	BYTE             m_IsCanUse;
	//char            m_WonName[32];
};
/*
typedef struct
{
	int				m_ID;				// ��Ʒ��ID
	BYTE			m_Genre;			// ��Ʒ������
	BYTE			m_Detail;			// ��Ʒ�����
	BYTE			m_Particur;			// ��Ʒ����ϸ���
	BYTE			m_Series; 			// ��Ʒ������
	BYTE			m_Level;			// ��Ʒ�ĵȼ�
//	BYTE			m_btPlace;			// ����
//	BYTE			m_btX;				// ����
//	BYTE			m_btY;				// ����
	BYTE			m_Luck;				// MF
	int  			m_MagicLevel[6];	// ħ�����ɲ���
    int  			m_RongMagicLev[6];	// �������ɲ���
//	int		        m_BaoMagicLev[6];
//	int             m_JBLevel[7];
	WORD			m_Version;			// װ���汾
	BYTE			m_Durability;		// �;ö�
	UINT			m_RandomSeed;		// �������
	BYTE			m_GoldId;			// �ƽ�Id
	BYTE			m_StackNum;
	BYTE			m_EnChance;
	BYTE			m_Point;
	KTime			m_Time;
    BYTE	        m_Rongpiont;//  ����
	BYTE             m_IsBang;
	BYTE             m_IsKuaiJie;
	BYTE             m_IsMagic;
	BYTE             m_SkillType;
	BYTE             m_MagicID;
	//	char            m_ItmeInfo[516];
//	char            m_WonName[32];
	//char          m_DescInfo[516];
	BYTE            m_IsWhere;
//	int             m_SyncType;
	BYTE            m_IsCanUse;
//	int             m_Count;
//	char            nShopName[32];
//	int		        m_nIdx;
//	UINT	        m_uPrice;
}SViewItemInfo;
*/
typedef struct
{
	BYTE			ProtocolType;
	UINT			m_dwNpcID;
	SViewItemInfo	m_sInfo[itempart_num]; //itempart_num
}VIEW_EQUIP_SYNC;  // s2c_viewequip

typedef struct//�ýṹ����ͳ�Ƶ���ҵĻ�������
{
	char	Name[20];
	int		nValue;
	BYTE	bySort;
}TRoleList;

// ��Ϸͳ�ƽṹ
typedef struct
{
	TRoleList MoneyStat[10];			//��Ǯ��������б�ʮ����ң����ɴﵽ100����
	TRoleList XuStat[10];
	TRoleList LevelStat[10];			//������������б�ʮ����ң����ɴﵽ100����
	TRoleList KillerStat[10];			//ɱ����������б�

	//[���ɺ�][�����]������[0]��û�м������ɵ����
	TRoleList MoneyStatBySect[11][10];	//�����ɽ�Ǯ��������б�
	TRoleList XuStatBySect[11][10];
	TRoleList LevelStatBySect[11][10];	//�����ɼ�����������б�

	//[���ɺ�]������[0]��û�м������ɵ����
	int SectPlayerNum[11];				//�������ɵ������
	int SectMoneyMost[11];				//�Ƹ�����ǰһ������и�������ռ������
	int SectXuMost[11];
	int SectLevelMost[11];				//��������ǰһ������и�������ռ������
}  TGAME_STAT_DATA;


typedef struct
{
	BYTE	ProtocolType;
	BYTE	bSleep;
	UINT	NpcID;
} NPC_SLEEP_SYNC;

//////////////
//�������
typedef struct
{
	BYTE		ProtocolType;
	UINT		dwLadderID;
	TRoleList	StatData[10];
} LADDER_DATA;

typedef struct
{
	BYTE		ProtocolType;
	WORD		wSize;
	int			nCount;
	UINT		dwLadderID[0];
} LADDER_LIST;

typedef struct
{
	BYTE		ProtocolType;
	UINT		dwLadderID;
} LADDER_QUERY;


///////////////////
//chat ���

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	UINT	packageID;
	char	someone[_NAME_LEN];
	UINT    nItemDwid;
	BYTE	sentlen;
} CHAT_SOMEONECHAT_CMD, CHAT_SOMEONECHAT_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	UINT	packageID;
	BYTE	filter;
	UINT	channelid;
	UINT    nItemDwid;
	BYTE	cost;	//0: �����ƣ�1: 10Ԫ/�䣬2: <10Lv ? ����˵ : MaxMana/2/��, 3: MaxMana/10/��,4: <20Lv ? ����˵ : MaxMana*4/5/��
	BYTE	sentlen;
} CHAT_CHANNELCHAT_CMD;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	UINT	packageID;
	char	someone[_NAME_LEN];
	UINT	channelid;
	UINT	nItemDwid;
	BYTE	sentlen;
} CHAT_CHANNELCHAT_SYNC;

enum {codeSucc, codeFail, codeStore};
typedef struct
{
	BYTE	ProtocolType;
	UINT	packageID;
	BYTE	code;
} CHAT_FEEDBACK;

typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	WORD	wChatLength;
} CHAT_EVERYONE;


typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	WORD	wChatLength;
	BYTE	byHasIdentify;
	WORD	wPlayerCount;
//	UINT   nItemDwid;
} CHAT_GROUPMAN;   ////���� �� ���� ������ ����


typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	UINT	nameid;
	uint32_t lnID;
	WORD	wChatLength;
} CHAT_SPECMAN;


enum { tgtcls_team, tgtcls_fac, tgtcls_tong, tgtcls_scrn, tgtcls_bc,tgtcls_msgr};
typedef struct
{
	BYTE	ProtocolType;
	WORD	wSize;
	UINT	nFromIP;
	UINT	nFromRelayID;
	UINT	channelid;
	//UINT	nItemDwid;
	BYTE	TargetCls;
	UINT	TargetID;  //��·��
	WORD	routeDateLength;
} CHAT_RELEGATE;


///////////////////////////////////////
// tong ���

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} S2C_TONG_HEAD;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} STONG_PROTOCOL_HEAD;

typedef struct
{
	int		m_nPlayerIdx;
	int		m_nCamp;
	char	m_szTongName[defTONG_NAME_MAX_LENGTH + 1];
} STONG_SERVER_TO_CORE_APPLY_CREATE;

typedef struct
{
	int		m_nPlayerIdx;
	UINT	m_dwNpcID;
} STONG_SERVER_TO_CORE_APPLY_ADD;

typedef struct
{
	int		m_nSelfIdx;
	int		m_nTargetIdx;
	UINT	m_dwNameID;
} STONG_SERVER_TO_CORE_CHECK_ADD_CONDITION;

typedef struct
{
	int		m_nCamp;
	int		m_nPlayerIdx;
	UINT	m_dwPlayerNameID;
	char	m_szTongName[64];
} STONG_SERVER_TO_CORE_CREATE_SUCCESS;

typedef struct
{
	int		m_nSelfIdx;
	int		m_nTargetIdx;
	UINT	m_dwNameID;
} STONG_SERVER_TO_CORE_REFUSE_ADD;

typedef struct
{
	int		m_nSelfIdx;
	int		m_nInfoID;
	int		m_nParam1;
	int		m_nParam2;
	int		m_nParam3;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_GET_INFO;

typedef struct
{
	int		m_nCamp;
	int		m_nPlayerIdx;
	UINT	m_dwPlayerNameID;
	char	m_szTongName[32];
	char	m_szMasterName[32];
	char	m_szTitleName[32];
} STONG_SERVER_TO_CORE_ADD_SUCCESS;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[32];
	char	m_szName[32];
} STONG_SERVER_TO_CORE_BE_INSTATED;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_BE_KICKED;

typedef struct
{
	int		m_nPlayerIdx;
	BOOL	m_bSuccessFlag;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_LEAVE;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	UINT	m_dwTongNameID;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_CHECK_GET_MASTER_POWER;

typedef struct
{
	int		m_nPlayerIdx;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	UINT	m_dwTongNameID;
	char	m_szTitle[32];
	char	m_szName[32];
} STONG_SERVER_TO_CORE_CHANGE_AS;

typedef struct
{
	UINT	m_dwTongNameID;
	char	m_szName[32];
} STONG_SERVER_TO_CORE_CHANGE_MASTER;

typedef struct
{
	UINT	m_dwTongNameID;
	UINT	m_dwMoney;
	UINT	m_nMoney;
	BYTE		nType;
	int		m_nPlayerIdx;
} STONG_SERVER_TO_CORE_MONEY;

typedef struct
{
	UINT	m_dwParam;
	int		m_nFlag;
	int		m_nCamp;
	int		m_nFigure;
	int		m_nPos;
	char	m_szTongName[32];
	char	m_szTitle[32];
	char	m_szMaster[32];
	char	m_szName[32];
	UINT	m_nMoney;
	int     m_nTempCamp;                            // ������ʱ��Ӫ ��������ս��(��� �̰�)
	char	m_szAttackName[defTONG_STR_LENGTH];		// ����ս�İ����
	int     m_AttackState;                          // ��ս״̬ 0����ս 1������ս�� 2����ս��
	int     m_AttackTime;                           // ��ս��ʱ��
	int     m_nDeathCount;
	int     m_nLevel;
} STONG_SERVER_TO_CORE_LOGIN;

// ������뽨����� ����չЭ��
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btCamp;
	char	m_szName[defTONG_NAME_MAX_LENGTH + 1];  //�������
} TONG_APPLY_CREATE_COMMAND;

// ������������ ����չЭ��
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwNpcID;
} TONG_APPLY_ADD_COMMAND;

// ������������ ����չЭ��
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwTongNameID;
	BYTE	m_btCurFigure;
	BYTE	m_btCurPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szName[32];
} TONG_APPLY_INSTATE_COMMAND;

// ��Ὠ��ʧ�� ��չЭ��
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btFailId;
} TONG_CREATE_FAIL_SYNC;


// ��Ὠ��ʧ�� ��չЭ��

struct CTONG_WAR_CITY_INFO
{
	char	m_szMapName[32];	//������ͼ
	int 	m_idx;			 //����
	int 	m_mapidx;		 //���
	char	m_Tongmaster[32];//ռ������
	char	m_Mastername[32];//����
	int     m_levle;		  //���еȼ�
	int     m_shushou;		  //����˰��
};

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwParam;	 //��������ID
	UINT	m_dwNpcID;
	int     m_nTempCamp;                            // ������ʱ��Ӫ ��������ս��(��� �̰�)
	char	m_szAttackName[defTONG_STR_LENGTH];		// ��ս�����
	int     m_AttackState;                          // ��ս״̬ 0����ս 1������ս�� 2����ս��
	int     m_AttackTime;                           // ��ս��ʱ��
	int     m_WarCityCount;
	int     m_nLevel;
	int     m_YDeathCount;
	int     m_DDeathCount;
	int     m_nAttackNum;	                        // ��ս����
	int     m_nWinNum;								// ʤ������
	int     m_nLoseNum;								// �������
	int     m_nMapidx;                              // ����ͼ
//	CTONG_WAR_CITY_INFO snWarInfo[7];               // ��ս��������Ϣ
} CTONG_ATTTACK_INFO_SYNC;

typedef struct
{
 BYTE	ProtocolType;
 WORD	m_wLength;
 BYTE	m_btMsgId;
 int    m_WarCityCount;
 CTONG_WAR_CITY_INFO snWarInfo[7];               // ��ս��������Ϣ
}CTONG_CITY_INFO_SYNC;
// ת������������ ��չЭ��
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int		m_nPlayerIdx;
	char	m_szName[32];
	int     m_IsAuToAdd;    //�Ƿ��Զ�����
} TONG_APPLY_ADD_SYNC;

// ֪ͨ��ҽ������ɹ� ����ͨЭ��
typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_btCamp;  //��Ӫ
	char	m_szName[defTONG_NAME_MAX_LENGTH + 1]; //�����
//	char	m_szMaster[32]; //������
    BYTE	m_btlevle;
} TONG_CREATE_SYNC;  //�д�

// ֪ͨ��Ҽ����� ����չЭ��
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btCamp;
	char	m_szTongName[32];
	char	m_szTitle[32];
	char	m_szMaster[32];
} TONG_Add_SYNC;

// ��������ɢ��� ����չЭ��
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
} TONG_APPLY_DISMISS_COMMAND;

// ��Ұ�ᱻ��ɢ ����ͨЭ��
typedef struct
{
	BYTE	ProtocolType;
	int     m_nKind;
	int     m_inParma;
	int     m_inParmb;
	int     m_inParmc;
	int     m_inParmd;
} TONG_DISMISS_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	int		m_nPlayerIdx;       //�����˵�Playerid
	UINT	m_dwNameID;         //�����˵�����ID
	BYTE	m_btFlag;			// �Ƿ���� TRUE ���� FALSE ������
} TONG_ACCEPT_MEMBER_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btInfoID;
	int		m_nParam1;   // ����İ����id
	int		m_nParam2;   // ����ʼ��λ��
	int		m_nParam3;   // ������������
	char	m_szBuf[64];
} TONG_APPLY_INFO_COMMAND;

typedef struct
{
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[32];
	char	m_szName[32];
} TONG_ONE_LEADER_INFO;

typedef struct
{
	char	m_szName[32];
} TONG_ONE_MEMBER_INFO;

typedef struct
{
	char	m_szName[32];  //��������
	int     m_inLevel;     //����ȼ�
} TONG_CALL_BACK_INFO;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwNpcID;
	UINT	m_dwMoney;
	int		m_nCredit;
	BYTE	m_btCamp;
	WORD	m_btLevel;	  //���ĵȼ�
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	UINT	m_dwMemberNum;
	int     m_btApplyNum;
	int     m_btZhaoMuNum;
	char	m_szTongName[32];
	TONG_ONE_LEADER_INFO	m_sMember[1 + defTONG_MAX_DIRECTOR];
} TONG_HEAD_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwMoney;
	int		m_nCredit;
	BYTE	m_btCamp;
	WORD	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	UINT	m_dwMemberNum;
	BYTE	m_btStateNo;
	BYTE	m_btCurNum;
	char	m_szTongName[32];
	TONG_ONE_LEADER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
} TONG_MANAGER_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwMoney;
	int		m_nCredit;
	BYTE	m_btCamp;
	WORD	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	UINT	m_dwMemberNum;
	BYTE	m_btStateNo;
	BYTE	m_btCurNum;
	char	m_szTitle[32];
	char	m_szTongName[32];
	TONG_ONE_MEMBER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
} TONG_MEMBER_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btStateNo;
	BYTE	m_btCurNum;
	int     m_Count;
//	char	m_szTitle[32];
//	char	m_szTongName[32];
	TONG_ONE_MEMBER_INFO	m_sTongList[defTONG_ONE_PAGE_MAX_NUM];
} TONG_LIST_INFO_SYNC;

// ��������ڰ���е���Ϣ ����չЭ��
typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	BYTE	m_btJoinFlag;
	BYTE	m_btFigure;
	BYTE	m_btCamp;
	char	m_szTongName[32];
	char	m_szTitle[32];
	char	m_szMaster[32];
	UINT	m_dwMoney;
	int     m_nAttackState;//��ս״̬
    int	    m_AttackCamp;
    int     m_AttackTime;
    char    m_AttackName[32];
	int     m_nDeathCount;
	int     m_nLevel;
} TONG_SELF_INFO_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btOldFigure;
	BYTE	m_btOldPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szTitle[32];
	char	m_szName[32];
} TONG_INSTATE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
} TONG_APPLY_KICK_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
} TONG_KICK_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
} TONG_APPLY_LEAVE_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
} TONG_APPLY_CHANGE_MASTER_COMMAND;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwTongNameID;
	BYTE	m_btFailID;
	char	m_szName[32];
} TONG_CHANGE_MASTER_FAIL_SYNC;

// tong ��� end
typedef struct
{
	BYTE	ProtocolType;
	UINT	m_dwNpcID;
	WORD	m_wGoldFlag;
} NPC_GOLD_CHANGE_SYNC;

typedef struct
{
	BYTE	ProtocolType;
	UINT	dwItemID;
	int		nChange;
	BYTE    nCanUse;
} ITEM_DURABILITY_CHANGE;

typedef struct
{
	BYTE	ProtocolType;
	UINT	dwItemID;
} ITEM_REPAIR;

typedef struct
{
	BYTE	ProtocolType;
	int 	nType;
} PLAYER_SYN_TYPE;


typedef struct
{
	BYTE	ProtocolType;
	int 	nXpos;
	int 	nYpos;

}PLAYER_SYN_POS;

typedef struct
{
	BYTE	ProtocolType;
	UINT	dwItemID;
	int		nNum;
} ITEM_BREAK;

typedef struct
{
	BYTE			ProtocolType;		// Э������
    UINT			uId[16];
	int				bKind;
	int             mFuMoVal;
}PLAYER_COMPITEM;

typedef struct
{
	BYTE	ProtocolType;		// Э������
    int 	inType;
	char	szQingXiang[32];
	char	szHuoDong1[32];
	char	szHuoDong2[32];
	char	szHuoDong3[32];
	char	szHuoDong4[32];
	char	szJiYu[64];
	char    szGongGao[128];
	int     inAcceptLevel;
	int     inRefuseLevel;
}PLAYER_TONGINFO;


typedef struct
{
	BYTE	ProtocolType;		// Э������
    int 	inKind;
	int     inCurTime;
	int     inCurTimeb;
	int     inCurTimec;
	int     inCurTimed;
    char    szItemInfo[32];
}PLAYER_DOSCRIPT;


typedef struct
{
	BYTE	ProtocolType;		// Э������
  	int     cnKind;
	int     cnCurExp;
	int     cnCurLucky;
	int     cnCurJiHuo;
	int     cnCurWuHang;
	char    snCardAcc[32];
	char    snCardPass[32];
}PLAYER_VIP_DOSCRIPT;

typedef struct
{
	BYTE	ProtocolType;		// Э������
	UINT   m_wLength;
    int 	inType;
	char	szQingXiang[32];
	char	szHuoDong1[32];
	char	szHuoDong2[32];
	char	szHuoDong3[32];
	char	szHuoDong4[32];
	char	szJiYu[64];
	char    szGongGao[128];
	int     inAcceptLevel;
	int     inRefuseLevel;
    TONG_CALL_BACK_INFO  m_sMember[defTONG_ONE_PAGE_MAX_NUM];//
}PLAYER_TONGINFO_CALLBACK;

typedef struct
{
	BYTE			ProtocolType;		// Э������
    int 			uIiemId;
	int             nKind;
	int             nX;
	int             nY;
	//int				bKind;
}PLAYER_ADD_ITEM;

typedef struct
{
	BYTE			ProtocolType;		// Э������
    UINT 			uIiemDwId;
	int				bKind;              // 0 Ϊ��װ 1 Ϊжװ
}PLAYER_AUTO_EQUIP;


typedef struct
{
	BYTE			ProtocolType;		// Э������
    int 			uIiemIdx;
	//int				bKind;
}PLAYER_DELL_ITEM;

typedef struct
{
	BYTE			ProtocolType;		// Э������
    UINT			uId[11];
	int				bKind;
}PLAYER_RONGITEM;


typedef struct
{
	int				m_ID;				// ��Ʒ��ID
	BYTE			m_Genre;			// ��Ʒ������
	int 			m_Detail;			// ��Ʒ�ľ������
	int 			m_Particur;			// ��Ʒ����ϸ���
	BYTE			m_Series; 			// ��Ʒ������
	BYTE			m_Level;			// ��Ʒ�ĵȼ�
	BYTE			m_btPlace;			// ����
	int			    m_btX;				// ����
	int 			m_btY;				// ����
	int				m_Luck;				// MF
	int				m_MagicLevel[6];	// ħ�����ɲ���
	int				m_RongMagicLev[6];	// �������ɲ���
//	int		        m_BaoMagicLev[6];
//	int             m_JBLevel[7];
	WORD			m_Version;			// װ���汾
//	WORD			m_Durability;		// �;ö�
	UINT			m_RandomSeed;		// �������
	int				m_GoldId;			// �ƽ�Id
	int             m_IsBaiJin;
	BYTE			m_StackNum;
	BYTE			m_EnChance;
	BYTE			m_Point;
	KTime			m_Time;
    BYTE	        m_Rongpiont;//  ����
//    BYTE             m_IsBang;
//	BYTE             m_IsKuaiJie;
//	BYTE             m_IsMagic;
//    BYTE             m_SkillType;
//	BYTE             m_MagicID;
//	char            m_ItmeInfo[516];
//	char            m_WonName[32];
	//char          m_DescInfo[516];
	int              m_IsWhere;
//	BYTE             m_SyncType;
	BYTE             m_IsCanUse;
//	BYTE             m_Count;
	char            nShopName[32];
	int		        m_nIdx;
	UINT	        m_uPrice;
	BYTE            m_Model;
} SViewSellItemInfo;

typedef struct
{
	BYTE			ProtocolType;
	UINT			m_dwNpcID;
	SViewSellItemInfo	m_sInfo[6];
} VIEW_ITEM_SYNC;				// s2c_viewequip

/*
typedef struct
{
	int		m_nID;				// ��Ʒ��ID
	BYTE	m_btGenre;			// ��Ʒ������
	int		m_btDetail;			// ��Ʒ�����
	int		m_btParticur;		// ��Ʒ����ϸ���
	BYTE	m_btSeries;			// ��Ʒ������
	BYTE	m_btLevel;			// ��Ʒ�ĵȼ�
	BYTE	m_btLuck;			// MF
	int		m_btMagicLevel[6];	// ���ɲ���
    int	    m_btRMagicLevel[6];
	int 	m_btBMagicLevel[6]; // ��ʯ
	int 	m_btJMagicLevel[7]; // ����
	WORD	m_wVersion;			// װ���汾
	UINT	m_dwRandomSeed;		// �������
	int		m_nIdx;
	UINT	m_uPrice;
	int		m_bX;
	int		m_bY;
	BYTE	m_bPoint;
	BYTE	m_bRPoint;
	BYTE	m_bStack;
	BYTE	m_bEnChance;
	int		m_nGoldId;
	int     m_Isbule;
	int     m_LianJieFlg;
//	char    m_DescInfo[516];
	int     m_IsWhere;
} SLianSellItemInfo;

typedef struct
{
	BYTE	ProtocolType;
	UINT	m_dwNpcID;
	SLianSellItemInfo m_sLInfo;
	int     m_Count;
	char    nShopName[32];
} VIEW_LIAN_ITEM_SYNC;
*/

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_ID;				// ��Ʒ��ID
	BYTE			m_Genre;			// ��Ʒ������
	int			    m_Detail;			// ��Ʒ�����
	int				m_Particur;			// ��Ʒ����ϸ���
	BYTE			m_Series; 			// ��Ʒ������
	BYTE			m_Level;			// ��Ʒ�ĵȼ�
//	BYTE			m_btPlace;			// ����
	BYTE			m_btX;				// ����
	BYTE			m_btY;				// ����
	int				m_Luck;				// MF
	int				m_MagicLevel[6];	// ħ�����ɲ���
	int				m_RongMagicLev[6];	// �������ɲ���
//	int		        m_BaoMagicLev[6];
	int             m_JBLevel[7];
	WORD			m_Version;			// װ���汾
	WORD			m_Durability;		// �;ö�
	UINT			m_RandomSeed;		// �������
	int				m_GoldId;			// �ƽ�Id
	int				m_StackNum;
	BYTE			m_EnChance;
	BYTE			m_Point;
	KTime			m_Time;
    BYTE	        m_Rongpiont;//  ����
	int             m_IsBang;
	int             m_IsKuaiJie;
	int             m_IsMagic;
	int             m_SkillType;
	int             m_MagicID;
	char            m_ItmeInfo[516];
	char            m_WonName[32];
	//	sMagInfo        m_MagInfo[6];
	int             m_IsWhere;
	int             m_SyncType;
	int             m_IsCanUse;
//	UINT	        m_dwNpcID;
//	int             m_Count;
//	char            nShopName[32];
	int		        m_nIdx;
//	UINT	        m_uPrice;
	BYTE            m_LianJieFlg;
	int             m_IsBaijin;
	int             m_UseMap;
	int             m_UseKind;
} VIEW_LIAN_ITEM_SYNC;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			dwId;
	int				nCount;
}PLAYER_GET_COUNT;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			dwId;
}PLAYER_NEED_COUNT;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	BYTE            nMsgIndex;
	char            szCallFuc[16];
	char            szParma[32];
	char            szParmb[32];
	char            szParmc[32];

}PLAYER_JINDU_CALLBACK;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int 			m_Idx;		    	// ��ڼ�������
	UINT			m_PlayerId;         // ��ҵ�playeridx
	BYTE			m_Place;			// ���������ĸ��ط�
	BYTE			m_X;				// ����X
	BYTE			m_Y;				// ����Y
	int             m_IsCityMenber;		// �Ƿ�ռ���Ա
    int             m_Shuihou;
	int             m_MapID;
	BYTE            m_Model;
} PLAYER_TRADE_BUY_ITEM_COMMAND;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	UINT			m_ID;				// ��Ʒ��ID
	int				m_Price;
	BYTE            m_Model;
}PLAYER_SET_PRICE;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	int				m_ID;				// ��Ʒ��ID
	BYTE			m_LianFlg;
}PLAYER_SET_LIANJIEFLG;


typedef struct
{
	BYTE			ProtocolType;		// Э������
	char			m_Name[32];         //��̯���֣�
	BYTE			m_bSet;
}PLAYER_START_TRADE;

typedef struct
{
	BYTE			ProtocolType;		// Э������
	char			m_Name[32];
	UINT			m_dwNpcID;
}PLAYER_PLAYER_SHOPNAME;

typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwTongNameID;
	UINT	m_dwMoney;
	char	m_szName[32];
} TONG_APPLY_SAVE_COMMAND;


typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwTongNameID;
	UINT   m_NpcDwid;
} TONG_APPLY_GET_ATTACK;


typedef struct
{
	BYTE	ProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwYTongNameID;
	UINT	m_dwDTongNameID;
	UINT   m_NpcDwid;
} TONG_APPLY_SEND_ATTACK;



typedef struct
{
	BYTE	ProtocolType;
	BYTE	NextProtocolType;
	WORD	m_wLength;
	BYTE	m_btMsgId;
	UINT	m_dwTongNameID;
	int  	m_inParam;
	UINT   m_NpcDwid;
	char    m_Title[32];
} TONG_APPLY_SEND_SETINFO;


typedef struct
{
	BYTE	ProtocolType;
	char	m_szName[256]; //˵��
	BYTE	m_nNum;
	char    m_szTitle[64];
	char    m_Callback[64];
	char    m_Error[16];
}PLAYER_GIVE;


typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_nType;

}PLAYER_DUANZHAO;

typedef struct
{
	BYTE	ProtocolType;
	BYTE	m_nType;
	int     m_nCurExtPiont;
}PLAYER_VIPDAZAO;

typedef struct
{
	BYTE	ProtocolType;
	UINT	m_uId[24];
	BYTE	nKind;
	int     nPrice;

    char	m_Callback[64]; //�ص�����
    char    m_Error[16];
}PLAYER_GIVE_CALLBACK;


typedef struct
{
	BYTE	ProtocolType;
	char    m_szTitle[32];  //����
	int     m_MsgIndex;     //ģ��ı��
	char    m_Callback[16]; //�ص��ĺ���
	int     m_inTime;       //��ͣ��ʱ��
    char    m_szParma[32];  //�ص��Ĳ���һ
    char    m_szParmb[32];
	char    m_szParmc[32];
}PLAYER_OPEN_JINDUTIAO;



typedef struct
{
	BYTE			ProtocolType;
	int             nShopType;
	int             nShopID;
}PLAYER_REQUEST_OPEN_MARKET;


typedef struct
{
	BYTE    ProtocolType;
    char    m_Content[125];
    char    m_SCallback[64];
    char    m_SError[16];
    int     m_nNum;
}PLAYER_STRING_CALLBACK;


typedef struct
{
	BYTE	ProtocolType;
	int		nShopID;
} SALE_MARKET_SYNC;

#ifndef _SERVER

// �ڵ�����֧����֮ǰ�����ж��Ƿ��ڽ���״̬��������ڽ��ף����ܵ�����֧����
void SendClientCmdSell(int nID);
// �ڵ�����֧����֮ǰ�����ж��Ƿ��ڽ���״̬��������ڽ��ף����ܵ�����֧����
void SendClientCmdBuy(int nBuyIdx, int nPlace, int nX, int nY,int nShuiShou,int nIsCityMenber,int nMapID,int nItemNum=0);
// �ڵ�����֧����֮ǰ�����ж��Ƿ��ڽ���״̬��������ڽ��ף����ܵ�����֧����
void SendClientCmdRun(int nX, int nY,int nMapID=0);
// �ڵ�����֧����֮ǰ�����ж��Ƿ��ڽ���״̬��������ڽ��ף����ܵ�����֧����
void SendClientCmdWalk(int nX, int nY);
// �ڵ�����֧����֮ǰ�����ж��Ƿ��ڽ���״̬��������ڽ��ף����ܵ�����֧����
void SendClientCmdSkill(int nSkillID, int nX, int nY);
void SendClientCmdSunyi(int nMaps,int nX, int nY,int nNpcID,int nModel=0); //˲��
//void SendClientCmdPing();
void SendClientCmdSit(int nSitFlag);
void SendClientCPUnlockCmd(int CP_IntPW);
void SendClientCPLockCmd();
void SendClientCPChangeCmd(int oldPW, int newPW);
void SendClientCPResetCmd(int resetPW);
void SendClientOpenMarket(int nShopid,int iShopType=0);
void SendClientString(char* sSContent,char* sSCallback,char* sSError,int biaozhi);//stringbox
void SendClientAutoplayopen(int nstt); //�һ������ű�
void SendClientAutoplayclose(int nstt);
void SendClientDaTau(int nstt,int nXuHao);
void SendClientCmdMoveItem(void* pDownPos, void* pUpPos,int nIsComp=0);
void SendClientCmdQueryLadder(UINT	dwLadderID);
void SendClientCmdRequestNpc(int nID);
void SendClientCmdJump(int nX, int nY);
void SendClientCmdStoreMoney(int nDir, int nMoney);
void SendClientCmdRevive(int nReviveType);
void SendObjMouseClick(int nObjID, UINT dwRegionID);
void SendClientCmdRepair(UINT dwID);
void SendClientCmdRide(); //����
void SendClientCmdBreak(UINT dwID, int nNum);
void SendClientCmdSetPrice(UINT nId, int nPrice,BYTE nModel=0); //��̯��۸�
void SendClientCmdStartTrade(int nStart, char* sName); //��̯
void SendClientCmdPlayerBuy(int nIdx, UINT nPlayerId, int nPlace, int nX, int nY,int IsCityMenber,int nShuishou,int nMapID,BYTE nModel=0);
void SendClientCmdGetCount(UINT dwId);
void SendClientCmdLianCount(UINT dwId);
void SendClientCmdShopName(UINT dwID);
void SendClientCmdSysShop(int nKind,int inParma=0,int inParmb=0,int inParmc=0,int inParmd=0);
void SendClientCmdLiXian();
void SendClientCmdGive(uint32_t uId, int nItemNum, int nKind, char* sCallback,char* sError,int nPrice=0);
void SendClientPKValue(int nstt);

void SendClientGuZi(int nMapID,int dianshu,int nNetIndexid,int ZuoWeiHao);
void SendClientGuZiFarms(int nMapID,int nFarmA,int nFarmB);
void SendClientAddJinBi(int nJinBi);
void SendClientAutoPlay(int nstt,int cishu,int nIsOpen);
void SendClientLianJie(char*nPLname,int nItemDWid);
void SendClientCmdJinMai(int nType,int nLevel);
void SendClientCmdGua(int nGuaType);
void SendClientCmdRong(uint32_t uId, int nItemNum, int nKind);
void SendClientCmdComp(ItemCounmd *pInfo, int nItemNum, int nKind,int nFuMoVal=0);

void SendClientAddItem(void* pInfo); //KUiObjAtContRegion *pInfo   //û���õ�
void SendClientDelItem(int nIndex);
void SendClientCmdSetLianFlg(int nId, BYTE nLianFlg); //���÷���������״̬
void SendClientSetTongInfo(KUiTongInfoWithName *nTong); //����������
void SendClientSetTonggg(KUiTongInfoWithName *nTong);
void SendClientGetTongInfo(int nIndex,int nType=0,int nStartPos=2);
void SendClientGetTonggg(int nIndex);
void SendClientDoScipt(KUiPlyerDoscript *nDoscript=NULL);
void SendClientJdCallBack(nJinDuClient *nJdscript=NULL);
void SendClientUpData();            //������µ�ǰ����
void SendClientVipDoScipt(KUiPlyerVipDoscript *nVipDoscript=NULL);
void SendClientSetTaskVal(int nTaskNo);
void SendClientCmdTpye(int nType);  //������� ĳ������
void SendClientSetPos(int mXpos,int mYpos);
void SendClientRunScript(int nKind,int nVala, int nValb, int nValc);

#endif

extern	int	g_nProtocolSize[MAX_PROTOCOL_NUM];
#pragma pack(pop, enter_protocol)
#endif


