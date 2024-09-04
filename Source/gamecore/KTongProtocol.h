// -------------------------------------------------------------------------
//	�ļ���		��	KTongProtocol.h
//	������		��	лï�� (Hsie)
//	����ʱ��	��	2003-08-13 15:12:19
//	��������	��	
//
// -------------------------------------------------------------------------
#ifndef __KTONGPROTOCOL_H__
#define __KTONGPROTOCOL_H__
#include "KProtocol.h"
#pragma pack(push, 1)

#define		defTONG_PROTOCOL_SERVER_NUM		255
#define		defTONG_PROTOCOL_CLIENT_NUM		255

//---------------------------- tong protocol ----------------------------
// relay server �յ��� game server ��Э��
enum 
{
	enumC2S_TONG_CREATE = 0,			// �������
	enumC2S_TONG_ADD_MEMBER,			// ��ӳ�Ա
	enumC2S_TONG_GET_HEAD_INFO,			// ��ð����Ϣ
	enumC2S_TONG_GET_MANAGER_INFO,		// ��ð��ӳ���Ϣ
	enumC2S_TONG_GET_MEMBER_INFO,		// ��ð�������Ϣ
	enumC2S_TONG_INSTATE,				// ����
	enumC2S_TONG_KICK,					// ����
	enumC2S_TONG_LEAVE,					// ���
	enumC2S_TONG_CHANGE_MASTER,			// ��λ
	enumC2S_TONG_ACCEPT_MASTER,			// �Ƿ���ܴ�λ
	enumC2S_TONG_GET_LOGIN_DATA,		// ��ҵ�½ʱ����������
	enumC2S_TONG_MONEY_SAVE,
	enumC2S_TONG_MONEY_GET,
	enumC2S_TONG_MONEY_SND,
	enumC2S_TONG_ATTACK,
	enumC2S_TONG_ATTACK_GET_INFO,
	enumC2S_TONG_ATTACK_SEND_INFO,
	enumC2S_TONG_GET_LIST_INFO,		    // ��ð���б���Ϣ
	enumC2S_TONG_INFO_SET,              // ���ð����Ϣ
	enumC2S_TONG_NUM,					// ����
};
//���ð����Ϣ����Э��
enum 
{
	    enumC2S_TONG_SET_LEVEL = 0,			//���õȼ�
		enumC2S_TONG_SET_MAP,
		enumC2S_TONG_SET_CREDIT,
		enumC2S_TONG_SET_BANGTITLE,
		enumC2S_TONG_SET_ZHANGTITLE,
		enumC2S_TONG_SET_DUITITLE,
		enumC2S_TONG_SET_ZONGTITLE,
		enumC2S_TONG_SET_DEATHCOUNT,
		enumC2S_TONG_SET_ATTACKWIN,
		enumC2S_TONG_SET_ATTACKLOSE,
		enumC2S_TONG_RANDOM_MAP,  //������ð�����ͼ
		enumC2S_TONG_SET_NUM,
};

// relay server ���� game server ��Э��
enum
{
	enumS2C_TONG_CREATE_SUCCESS = 0,		// ��ᴴ���ɹ�
	enumS2C_TONG_CREATE_FAIL,				// ��ᴴ��ʧ��
	enumS2C_TONG_ADD_MEMBER_SUCCESS,		// �����ӳ�Ա�ɹ�
	enumS2C_TONG_ADD_MEMBER_FAIL,			// �����ӳ�Աʧ��
	enumS2C_TONG_HEAD_INFO,					// �����Ϣ
	enumS2C_TONG_MANAGER_INFO,				// ���ӳ���Ϣ
	enumS2C_TONG_MEMBER_INFO,				// ���ӳ���Ϣ
	enumS2C_TONG_BE_INSTATED,				// ������
	enumS2C_TONG_INSTATE,					// �����ɹ���ʧ��
	enumS2C_TONG_KICK,						// ���˳ɹ���ʧ��
	enumS2C_TONG_BE_KICKED,					// ���߳����
	enumS2C_TONG_LEAVE,						// �뿪���ɹ���ʧ��
	enumS2C_TONG_CHECK_CHANGE_MASTER_POWER,	// �ж��Ƿ��е�����������
	enumS2C_TONG_CHANGE_MASTER_FAIL,		// ��λʧ��
	enumS2C_TONG_CHANGE_AS,					// ��λ�ɹ�����ݸı�
	enumS2C_TONG_CHANGE_MASTER,				// �㲥����������
	enumS2C_TONG_LOGIN_DATA,				// ��ҵ�½ʱ���ð������
	enumS2C_TONG_MONEY_SAVE,	            // ��Ǯ
	enumS2C_TONG_MONEY_GET,	                // ȡǮ
	enumS2C_TONG_MONEY_SND,                 // ��Ǯ
	enumS2C_TONG_ATTACK_GETBACK,
	enumS2C_TONG_ATTACK_SENDBACK,
	enumS2C_TONG_ATTACK_OVER,
	enumS2C_TONG_ALLLIST_INFO,
	enumS3C_TONG_SYNCITY_INFO,
	enumS3C_TONG_SERVER_TIEM,
	enumS2C_TONG_NUM,					    // ����
};
//-------------------------- tong protocol end --------------------------

//friend protocol
enum 
{
	friend_c2c_askaddfriend,	//�����Ϊ����
	friend_c2c_repaddfriend,	//ͬ��/�ܾ���Ϊ����
	friend_c2s_groupfriend,		//�����ѷ���
	friend_c2s_erasefriend,		//ɾ������

	friend_c2s_asksyncfriendlist,	//����ͬ�������б�
	friend_s2c_repsyncfriendlist,	//ͬ�������б�

	friend_s2c_friendstate,		//����״̬֪ͨ

	friend_c2s_associate,		//GS��Relay���Զ����2���ˣ��з���
	friend_c2s_associatebevy,	//GS��Relay���Զ����n����
	friend_s2c_syncassociate,	//Relay��Client��֪ͨ���
};

//extend protocol
enum
{
	extend_s2c_passtosomeone,
	extend_s2c_passtobevy,
};



/////////////////////////////////////////////////////////////////
//friend struct

const int _GROUP_NAME_LEN = _NAME_LEN * 2;


struct ASK_ADDFRIEND_CMD : EXTEND_HEADER
{
	BYTE pckgid;
	char dstrole[_NAME_LEN];
};
struct ASK_ADDFRIEND_SYNC : EXTEND_HEADER
{
	BYTE pckgid;
	char srcrole[_NAME_LEN];
};

enum {answerAgree, answerDisagree, answerUnable};
struct REP_ADDFRIEND_CMD : EXTEND_HEADER
{
	BYTE pckgid;
	char dstrole[_NAME_LEN];
	BYTE answer;	//agree/disagree/unable
};
struct REP_ADDFRIEND_SYNC : EXTEND_HEADER
{
	BYTE pckgid;
	char srcrole[_NAME_LEN];
	BYTE answer;	//agree/disagree/unable
};



//used by GROUP_FRIEND & REP_SYNCFRIENDLIST
enum {specOver = 0x00, specGroup = 0x01, specRole = 0x02};

struct GROUP_FRIEND : EXTEND_HEADER
{
	//format: char seq
	//specGroup����飬���Ӹ�������б���\0�����specRole��ǽ�ɫ��
	//�����˫\0����
};


struct ERASE_FRIEND : EXTEND_HEADER
{
	char friendrole[_NAME_LEN];
};


struct ASK_SYNCFRIENDLIST : EXTEND_HEADER
{
	BYTE pckgid;
	BYTE full;
};


struct REP_SYNCFRIENDLIST : EXTEND_HEADER
{
	BYTE pckgid;
	//format: char seq (same as GROUP_FRIEND)
	//specGroup����飬���Ӹ�������б���\0�����specRole��ǽ�ɫ��
	//�����˫\0����
};


enum {stateOffline, stateOnline};

struct FRIEND_STATE : EXTEND_HEADER
{
	BYTE state;
	//format: char seq, \0�����˫\0����
};

struct FRIEND_ASSOCIATE : EXTEND_HEADER
{
	BYTE bidir;
	//format: string * 3
	//����
	//��ɫ�� * 2
};

struct FRIEND_ASSOCIATEBEVY : EXTEND_HEADER
{
	//format: char seq, \0�����˫\0����
	//����
	//��ɫ���б�
};

struct FRIEND_SYNCASSOCIATE : EXTEND_HEADER
{
	//format: char seq, \0�����˫\0����
	//����
	//��ɫ���б�
};


/////////////////////////////////////////////////////////////////
//extend struct

struct EXTEND_PASSTOSOMEONE : EXTEND_HEADER
{
	UINT			nameid;
	uint32_t	lnID;
	WORD			datasize;
};

struct EXTEND_PASSTOBEVY : EXTEND_HEADER
{
	WORD	datasize;
	WORD	playercount;
	//data
	//tagPlusSrcInfo vector
};

//----------------------------- tong struct -----------------------------
//��ᴴ��ʱ�ĽṹЭ�� ���Կͻ���
struct STONG_CREATE_COMMAND : EXTEND_HEADER
{
	WORD	m_wLength;
	UINT	m_dwPlayerNameID;
	UINT	m_dwParam;
	BYTE	m_btCamp;
	int	    m_btTongNameLength;
	int	    m_btPlayerNameLength;
	char	m_szBuffer[64];
	char    m_TongName[32];
	char    m_PlayerName[32];
	int     m_blevel; // ���ȼ�
};
//��ᴴ���ɹ�ʱ ���ظ��ͻ��˵�ͬ����Ϣ�ṹ
struct STONG_CREATE_SUCCESS_SYNC : EXTEND_HEADER
{
	WORD	m_wLength;
	UINT	m_dwPlayerNameID;
	UINT	m_dwParam;
	BYTE	m_btCamp;
	int 	m_btTongNameLength;
	char	m_szTongName[32];
	int     m_blevel; // ���ȼ�
//	char    m_BangZhu[32];
};

struct STONG_CREATE_FAIL_SYNC : EXTEND_HEADER
{
	UINT	m_dwPlayerNameID;
	UINT	m_dwParam;
	BYTE	m_btFailID;
};

struct STONG_ADD_MEMBER_COMMAND : EXTEND_HEADER
{
	WORD	m_wLength;
	UINT	m_dwPlayerNameID;
	UINT	m_dwParam;
	BYTE	m_btTongNameLength;
	BYTE	m_btPlayerNameLength;
	char	m_szBuffer[64];
};

struct STONG_ADD_MEMBER_SUCCESS_SYNC : EXTEND_HEADER
{
	UINT	m_dwPlayerNameID;
	UINT	m_dwParam;
	BYTE	m_btCamp;
	char	m_szTongName[32];
	char	m_szMasterName[32];
	char	m_szTitleName[32];
};

struct STONG_ADD_MEMBER_FAIL_SYNC : EXTEND_HEADER
{
	UINT	m_dwPlayerNameID;
	UINT	m_dwParam;
	BYTE	m_btFailID;
};

struct STONG_GET_TONG_HEAD_INFO_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwNpcID;
	UINT	m_dwTongNameID;
};

struct STONG_GET_TONG_ATTACK_INFO_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwNpcID;
	UINT	m_dwTongNameID;
};

struct STONG_SET_TONG_INFO_COMMAND : EXTEND_HEADER
{
	BYTE    NextProtocolID;
	UINT	m_dwParam;
	UINT	m_dwNpcID;
	UINT	m_dwTongNameID;
	char    m_title[32];
};


struct STONG_GET_MANAGER_INFO_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	int		m_nParam1;
	int		m_nParam2;
	int		m_nParam3;
};

struct STONG_GET_MEMBER_INFO_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	int		m_nParam1;  //����İ������id
	int		m_nParam2;  //����ʼ��λ��
	int		m_nParam3;  //��������� ���Ϊ 7��
};

struct STONG_GET_LIST_INFO_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	int		m_nParam1;  //����İ������id
	int		m_nParam2;  //����ʼ��λ��
	int		m_nParam3;  //��������� ���Ϊ 7��
};

struct STONG_ONE_LEADER_INFO
{
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[32];  //�ƺ�
	char	m_szName[32];   //����
};

struct STONG_WAR_CITY_INFO
{
	char	m_szMapName[32];	//������ͼ
	int 	m_idx;			 //����
	int 	m_mapidx;		 //���
	char	m_Tongmaster[32];//ռ������
	char	m_Mastername[32];//����
	int     m_levle;		  //���еȼ�
	int     m_shushou;		  //����˰��
};

struct STONG_ONE_MEMBER_INFO
{
	char	m_szName[32];
};

//�����ս��Ϣͬ���ṹ
struct STONG_ATTACKSTIME_SENDBACK : EXTEND_HEADER
{
	WORD	    m_wLength;
	char 		m_szTime[64];
};

//�����ս��Ϣͬ���ṹ
struct STONG_ATTACK_SENDBACK : EXTEND_HEADER
{
	WORD	    m_wLength;
	UINT	    m_dwParam;
	UINT	    m_dwNpcID;
	char		m_szAttackName[defTONG_STR_LENGTH];		    // ��ս�����
	char		m_szDAttackName[defTONG_STR_LENGTH];		// ��ս�����
};
//�����ս��Ϣͬ���ṹ
struct STONG_CITY_INFO_SYNC : EXTEND_HEADER
{
	WORD m_wLength;
    int	 m_WarCityCount;
	STONG_WAR_CITY_INFO snWarInfo[7];                   //��ս��������Ϣ
};

//�����ս��Ϣͬ���ṹ
struct STONG_ATTACK_INFO_SYNC : EXTEND_HEADER
{
	WORD	    m_wLength;
	UINT	    m_dwParam;
	UINT	    m_dwNpcID;
	int         m_nTempCamp;                            // ������ʱ��Ӫ ��������ս��(��� �̰�)
	char		m_szAttackName[defTONG_STR_LENGTH];		// ��ս�����
	int         m_AttackState;                          // ��ս״̬ 0����ս 1������ս�� 2����ս��
	int         m_AttackTime;                           // ��ս��ʱ��
//	int         m_WarCityCount;
	int         m_CurMapIdx;                            //��ǰ����ͼ
	int         m_nLevel;
	int         m_YDeathCount;
	int         m_DDeathCount;                          //��ս����������
	int         m_nAttackNum;	                            // ��ս����
	int         m_nWinNum;								// ʤ������
	int         m_nLoseNum;								// �������
};
//���ͷ��Ϣͬ���ṹ
struct STONG_HEAD_INFO_SYNC : EXTEND_HEADER
{
	WORD	m_wLength;
	UINT	m_dwParam;
	UINT	m_dwNpcID;
	UINT	m_dwMoney;
	int		m_nCredit;
	BYTE	m_btCamp;
	int	    m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	UINT	m_dwMemberNum;
	char	m_szTongName[32];
	STONG_ONE_LEADER_INFO	m_sMember[1 + defTONG_MAX_DIRECTOR]; //0 Ϊ���� 1-7Ϊ����
};
//�ӳ�ͬ����Ϣ�ṹ
struct STONG_MANAGER_INFO_SYNC : EXTEND_HEADER
{
	WORD	m_wLength;
	UINT	m_dwParam;
	UINT	m_dwMoney;
	int		m_nCredit;
	BYTE	m_btCamp;
	int  	m_btLevel;
	BYTE	m_btDirectorNum;
	BYTE	m_btManagerNum;
	UINT	m_dwMemberNum;
	BYTE	m_btStartNo;
	BYTE	m_btCurNum;
	char	m_szTongName[32];
	STONG_ONE_LEADER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
};
//����ͬ����Ϣ
struct STONG_MEMBER_INFO_SYNC : EXTEND_HEADER
{
	WORD	m_wLength;
	UINT	m_dwParam;
	UINT	m_dwMoney;
	int		m_nCredit;
	BYTE	m_btCamp;
	int 	m_btLevel;
	BYTE	m_btDirectorNum;  //��������
	BYTE	m_btManagerNum;   //�ӳ�����
	UINT	m_dwMemberNum;    //��������
	BYTE	m_btStartNo;      //��¼�ϴ�����Ŀ�ʼλ��
	BYTE	m_btCurNum;       //��ǰ��¼������
	char	m_szTitle[32];
	char	m_szTongName[32];
	STONG_ONE_MEMBER_INFO	m_sMember[defTONG_ONE_PAGE_MAX_NUM];
};


//����б���Ϣ
struct STONG_LIST_INFO_SYNC : EXTEND_HEADER
{
	WORD	m_wLength;
	UINT	m_dwParam;
	BYTE	m_btStartNo;      //��¼�ϴ�����Ŀ�ʼλ��
	BYTE	m_btCurNum;       //��ǰ��¼������
	int     m_TongCount;
	STONG_ONE_MEMBER_INFO	m_sTongList[defTONG_ONE_PAGE_MAX_NUM];
};

struct STONG_INSTATE_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	BYTE	m_btCurFigure;
	BYTE	m_btCurPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szName[32];
};

struct STONG_BE_INSTATED_SYNC : EXTEND_HEADER
{
	UINT	m_dwParam;			// �������Ӻ�
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[32];
	char	m_szName[32];
};

struct STONG_BE_KICKED_SYNC : EXTEND_HEADER
{
	UINT	m_dwParam;			// �������Ӻ�
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
};

struct STONG_INSTATE_SYNC : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btOldFigure;
	BYTE	m_btOldPos;
	BYTE	m_btNewFigure;
	BYTE	m_btNewPos;
	char	m_szTitle[32];
	char	m_szName[32];
};

struct STONG_KICK_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
};

struct STONG_ATTACK_COMMAND : EXTEND_HEADER
{

};

struct STONG_KICK_SYNC : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	BYTE	m_btSuccessFlag;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
};

struct STONG_LEAVE_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
};

struct STONG_LEAVE_SYNC : EXTEND_HEADER
{
	UINT	m_dwParam;
	BYTE	m_btSuccessFlag;
	char	m_szName[32];
};

struct STONG_CHANGE_MASTER_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szName[32];
};

struct STONG_CHECK_GET_MASTER_POWER_SYNC : EXTEND_HEADER
{
	UINT	m_dwParam;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	UINT	m_dwTongNameID;
	char	m_szName[32];
};

struct STONG_CHANGE_MASTER_FAIL_SYNC : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	BYTE	m_btFailID;
	char	m_szName[32];
};

struct STONG_CHANGE_AS_SYNC : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	BYTE	m_btFigure;
	BYTE	m_btPos;
	char	m_szTitle[32];		// �Լ�����ͷ��
	char	m_szName[32];		// �°���������
};

struct STONG_ACCEPT_MASTER_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID; //�������ID
	BYTE	m_btFigure;
	BYTE	m_btPos;
	BYTE	m_btAcceptFalg;
	char	m_szName[32];
};

struct STONG_CHANGE_MASTER_SYNC : EXTEND_HEADER
{
	UINT	m_dwTongNameID;
	char	m_szName[32];
};

struct STONG_GET_LOGIN_DATA_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	char	m_szName[32];
};
//��¼ʱ ��ȡ�����Ϣ�ṹ
struct STONG_LOGIN_DATA_SYNC : EXTEND_HEADER
{
	UINT	m_dwParam;
	BYTE	m_btFlag;
	BYTE	m_btCamp;
	BYTE	m_btFigure;
	BYTE	m_btPos;
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
};

struct STONG_MONEY_COMMAND : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	UINT	m_dwMoney;
	char	m_szName[32];
};

struct STONG_MONEY_SYNC : EXTEND_HEADER
{
	UINT	m_dwParam;
	UINT	m_dwTongNameID;
	char	m_szName[32];
	UINT	m_nMoney;
	UINT	m_dwMoney;
};

//--------------------------- tong struct end ---------------------------


#pragma pack(pop)

#endif // __KTONGPROTOCOL_H__
