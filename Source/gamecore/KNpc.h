#ifndef KNpcH
#define KNpcH
//---------------------------------------------------------------------------
class ISkill;
class KSkill;
#include "KCore.h"
#include "KSkillList.h"
#include "KMagicAttrib.h"
#include "GameDataDef.h"
#include "KNpcFindPath.h"
#include "KNpcDeathCalcExp.h"
#include "KIndexNode.h"
#include "KNpcGold.h"

#include <map>
//#include <hash_map>
//#include <algorithm>
//#include "KSubworld.h"
#include "KNpcRes.h"
//---------------------------------------------------------------------------
#define	MAX_NPC			200        // NPc����  500
#define VER_INFO "50JX-ר�ö�"
#define MAX_NPCSTYLE	3000       // npcģ������ 2000
//#define VER_INFO "����<color=red>��<color>����-ר�ö�"
#define	defMAX_SHOW_BLOOD_NUM		5    //50
#define	MAX_AI_PARAM				10   //AIģʽ ���
#define	MAX_NPC_USE_SKILL			4    //NPC�������������

#define	MAX_FUMO_COUNT              5    //�񽫵�����
#define defLOGIC_CELL_WIDTH         32


//#define	TABFILE_MAGICALEVEL_PATH	"\\Settings\\item\\004\\magicattriblevel_index.txt"

//#define	KItemNormalAttrib KMagicAttrib
//	�ܶ��ٶ��������ٶȵı���
//#define	WALK_RUN_TIMES	3

#define		STATE_FREEZE	0x01   //2 �� 0 �η�
#define		STATE_POISON	0x02   //2 �� 1 �η�
#define		STATE_CONFUSE	0x04   //4 2�� 2�η�
#define		STATE_STUN		0x08   //8 2�� 3 �η�
#define		STATE_HIDE		0x10   //16 2�� 4 �η�
//#define		STATE_MOVE		0x32   //32 2�� 5 �η�

//#define		STATE_ZHUA		0x09
enum NPCATTRIB
{
	attrib_mana,
	attrib_stamina,
	attrib_life,
	attrib_maxmana,
	attrib_maxstamina,
	attrib_maxlife,
};

enum NPCCMD
{
	do_none,		// ʲôҲ����
	do_stand,		// վ��
	do_walk,		// ����
	do_run,			// �ܶ�
	do_jump,		// ��Ծ
	do_skill,		// �����ܵ�����
	do_magic,		// ʩ��
	do_attack,		// ����
	do_sit,			// ����
	do_hurt,		// ����
	do_death,		// ����
	do_defense,		// ��
	do_idle,		// ����
	do_specialskill,// ���ܿ��ƶ���
	do_special1,	// ����1
	do_special2,	// ͵�Լ���
	do_special3,	// ����3
	do_special4,	// �ƶ�
	do_runattack,
	do_manyattack,
	do_jumpattack,
	do_revive,      //����
	do_stall,
	do_movepos,		// ˲���ƶ�
	do_knockback,	// ����
	do_drag,		// ��������
	do_rushattack,	// �忳
	do_runattackmany, //��̶���
	do_num,
/*	do_sunyi,       //˲���ƶ�
	do_yidong,      //���ͼ�ƶ�

	do_movepos,		// ˲���ƶ�
	do_knockback,	// ����
	do_drag,		// ��������
	do_rushattack,	// �忳
	do_runattackmany, //��̶��� */
};

enum CLIENTACTION
{
	cdo_fightstand,
	cdo_stand,
	cdo_stand1,
	cdo_fightwalk,
	cdo_walk,
	cdo_fightrun,
	cdo_run,
	cdo_hurt,
	cdo_death,
	cdo_attack,
	cdo_attack1,
	cdo_magic,
	cdo_sit,
	cdo_jump,
	cdo_none,
	cdo_count,
};
 //�˺�����
enum DAMAGE_TYPE
{
	damage_physics = 0,		// �����˺�
	damage_fire,			// �����˺�
	damage_cold,			// �����˺�
	damage_light,			// �����˺�
	damage_poison,			// �����˺�
	damage_magic,			// �������˺�
	damage_num,				// �˺�������Ŀ
};

// DoDeath ʱ�Ĳ�������Ӧ��ͬ�������ͷ�
enum DEATH_MODE
{
	DEATH_MODE_NPC_KILL = 0,		// ��npcɱ��0
	DEATH_MODE_PLAYER_NO_PUNISH,	// �д�ģʽ�����ɱ��  1  û�гͷ�
	DEATH_MODE_PLAYER_PUNISH,		// �����PK����������PKֵ���гͷ�2
	DEATH_MODE_PKBATTLE_PUNISH,		// ��ʽ�ڹ�սʱ�ĳͷ����� 3
	DEATH_MODE_EXP_PUNISH,		    // ����ͷ�
	DEATH_MODE_MONEY_PUNISH,		// �����ͷ�
	DEATH_MODE_EQUIP_PUNISH,		// װ���ͷ�
	DEATH_MODE_JINBI_PUNISH,        // ��ҳͷ�
	DEATH_MODE_NUM,
};

typedef struct
{
	NPCCMD		CmdKind;		// ����C
	int			Param_X;		// ����X
	int			Param_Y;		// ����Y
	int			Param_Z;		// ����Y
} NPC_COMMAND;

typedef struct
{
	int		nTotalFrame;  //ȫ��֡
	int		nCurrentFrame;//��ǰ֡
	void SetFrame(int nTotal)
	{
		if (nTotal <= 0)	// һ�㲻��Ҫʱ��Ϊ0�Ķ���
			nTotalFrame = 1;
		else
			nTotalFrame = nTotal;

		nCurrentFrame = 0;
	};
} DOING_FRAME;

struct KState
{
	int	nMagicAttrib;
	int	nValue[3];
	int	nTime;
};
//�ı�NPC�Ļ������ݽṹ
struct KChanelBaseInfo
{
	int     PhysicsDamage;             // �����˺�
	int		m_PhysicsDamage;		   // Npc�ĵ�ǰ�˺�
	int		m_FireDamage;	           // Npc�ĵ�ǰ���˺�
	int		m_ColdDamage;	           // Npc�ĵ�ǰ���˺�
	int		m_LightDamage;	           // Npc�ĵ�ǰ���˺�
	int		m_PoisonDamage;	           // Npc�ĵ�ǰ���˺�

	int		m_FireResist;	            // Npc�ĵ�ǰ����
	int		m_ColdResist;	            // Npc�ĵ�ǰ������
	int		m_PoisonResist;	            // Npc�ĵ�ǰ������
	int		m_LightResist;	            // Npc�ĵ�ǰ�翹��
	int		m_PhysicsResist;	        // Npc�ĵ�ǰ������
};

struct skillAuraInfo
{
	int skillid;
	int skilllistIndex;
	int level;
};

struct sNpcShadowInfo
{
	bool bInMenu;
	unsigned int uImage;
	short nFrame;
	int nX;
	int nY;
	int nZ;
	char szImage[256];
};

//����
struct CFuMoInfo
{
	int nNpcIdx;
	int nNpcSetings;     //NPC settings	�浵
	int	nNpcSkill;       //ͬ��ļ���id �浵
	int nSkillLevel;	 //���ܵȼ�     �浵
	int nNowLevel;       //ͬ�����ڵĵȼ�
	DWORD nCastTime;     //�ͷ�ʱ����
	DWORD nNextCastTime; //�´��ͷ�ʱ��
	int nNowExp;         //���ڵľ���
};

//���ܼӳ�
struct CEnhanceInfo
{
	int	nSkillIdx;
	int nEnhance;
};
/*
struct CTongBanInfo
{
	int nNum1;
	int nNum2;
	int nNum3;
	int nNum4;
	int nNum5;
};
*/
struct	KSyncPos
{
	DWORD	m_dwRegionID;
	int		m_nMapX;
	int		m_nMapY;
	int		m_nOffX;
	int		m_nOffY;
	int		m_nDoing;
};

struct	KNpcAutoChat
{
	int                 m_Model;      //
	//char                m_NpcTalkMsg[250];
	int                 m_NpcTalkTime;
	//char                m_NpcTalkName[64];
};

class KStateNode : public KNode
{
public:

	int				m_SkillID;					// ����ID
	int				m_Level;					// ���ܵȼ�
	int				m_LeftTime;					// ʣ��ʱ��
	KMagicAttrib	m_State[MAX_SKILL_STATE];	// ��������״̬�б�
	//KMagicAttrib	m_JinMaiState;	            // ����״̬�б�
	int				m_StateGraphics;			// ״̬��������
	int             m_IsClientState;            // �Ƿ�Է������˵�״̬
};

class KNpc
{
	friend class KNpcSet;
public:
	sNpcShadowInfo      _NpcShadow;
	DWORD				m_dwID;					// Npc��ID  �ڸ�NPC ��� ��������
	//BOOL              m_IsSyn;
	DWORD               m_bLockNpcDwID;			// ����NPC��	DWid
    BYTE                m_IsHaveAttack;         //�Ƿ�����Ϊ������Ч��
	DWORD               m_AttackerDwid;         //�ϴι����ŵ�DWID
	DWORD               m_IsRevive;             //�Ƿ�����
	BYTE                m_IsRe;                 //ͬ������֡��
	//bool                m_isClearSpr;           //�Ƿ�ͬ�����ͻ���ɾ������
	int					m_Index;				// Npc������
	KIndexNode			m_Node;					// Npc's Node
	int					m_Level;				// Npc�ĵȼ�
	BYTE				m_Kind;					// Npc������
	BYTE				m_Series;				// Npc��ϵ
	int					m_Height;				// Npc�ĸ߶�(��Ծ��ʱ�����)
	int 				m_btRankId;             // �ƺŵ�id
	int 				m_NpcTitle;             // NPC�³ƺţ��浵��
	int                 m_CurNpcTitle;          // ��ǰ�ƺ�	 �����浵��ʱ�ƺţ�
	BYTE				m_btRankFFId;           //SPR�ƺŵ�
	BYTE                m_AutoplayId;           //�һ�SPR�ƺŵ�
	int 				nRankInWorld;           //��������
	int                 nLevelInWorld;          //����ȼ�����
	int 				nRepute;                //����
	int 				nPKValue;
	int 				nReBorn;                //ת������
	int					m_nStature;				//Tall
	int                 m_CJtaskID;             //172�齱ר��
	int                 m_KillNumber;           //ɱ������ ��������
//	CJinMaiInfo         m_JinMai[10];           //0 0 Ϊ��������
	//CFuMoInfo         m_nFuMoNum[MAX_FUMO_COUNT];          //��ħ�б�
	int                 m_nCurJiHuo;            //��ǰ����ĸ�ħ��NPC
	int                 m_ZhenYuan;             //��Ԫֵ
	int                 m_JinMaiBingJia;        //��������ֵ
	int                 m_GameliveTime;         //NPC ���ʱ��
	int                 m_TempliveTime;         //��ʱ������ʱ��
	int                 m_liveType;             //ʱ���趨������
	//BYTE                m_WhereSer;		        //�Ǹ���������

	///��ʱ����/////���������ݿ�
	int					m_njb;				    //Я���Ľ��
    int					m_njxb;				    //Я���Ľ���B
	//char				m_WarTongNamea[32];     //ռ����
    //char				m_WarMaster[32];        //����
//	int				    m_WarShuishou;          //˰��
//	int                 m_Warzhi;               //���ָ��
	int                 m_WarCityGX;            //���˹���
	int                 m_IsWarCity;            //�ĸ��ǵĳ�Ա
	int                 m_IsInCity;             //�Ƿ�������
	int					m_nsPlayerIdx;          // ��ҵķ�����ͬ��������
	int	                m_nMissionGroup;
/*	BYTE                 m_WarFucheng;           //����
    BYTE                 m_WarZuoHu;             //�󻤷�
	BYTE                 m_WarYouHu;             //�һ���
	BYTE                 m_WarTaishi;            //̫ʷ
	BYTE                 m_WarZhongShu;          //����
	BYTE                 m_WarShangShu;          //����
    BYTE                 m_WarMiShu;             //����
	BYTE                 m_WarTaiLe;             //̫��
	*/
    BYTE                m_IsDoing;              //�Ƿ�����ִ�нű�
	int                 m_ReviceNum;            //��������
    BYTE                m_IsSynDir;             //�Ƿ����ͬ������
//	int                 m_WarIsGong;            //���ڹ���
//    int               m_WarIsShou;            //�����ط�
	//char                m_GuishuName[32];       //NPC����˭��ͬ��
	DWORD               m_GuiShuDwid;           //���˵�Dwid
	//char              m_BiaoCheName[32];      //NPC����˭���ڳ�
	BYTE                m_RestNameCount;          //�����Ĵ���
	//char                m_ChenHaoName[32];      //�ƺŵ�����

 //   BYTE                m_IsWaiGua;              //�Ƿ�ʹ�����
 /*
	int                 m_GuziYajing;            //Ѻ��
	int                 m_GuziDianshu;           //����
	BYTE                m_GuziZhuang;            //�Ƿ�ׯ��
	BYTE                m_IsShuai;               //�Ƿ��Ѿ�˦��
	BYTE                m_GuziMenber;            //1Ϊ�������� 2Ϊ�Թ���ע��
	BYTE                m_ZuoWeihao;             //��λ��
*/
//	BYTE                IsDeath;
	int                 IsJinYan;                 //�����
	int                 IsLuKey;                  //�����
	int                 m_GoldLucky;              //ȫ������
	int                 IsJinQian;                //���Ǯ
//	char                m_ItmeInfo[128];          //��Ʒ˵��
//	BYTE                m_IsTuiGuang;             //�Ƿ��ƹ�Ա
	int                 m_IsVip;                  //��Ա
	int                 m_IsXingYunXing;

	int                 m_mMapX;
	int                 m_mMapY;
	int                 m_IsgetSkill;             //�Ƿ�ʰȡ����״̬
	BYTE                IsExeGoldScript;          //�Ƿ�ִ��ȫ�������ű�
	BYTE                IsCreatBoss;
	BYTE                IsCreatTongBan;           //�Ƿ��Ѿ������ٻ�ͬ��
	int                 m_ZhuaVal;                //��ǰ�ı�ץ��ֵ
    DWORD               m_TongBanNum;             //ͬ������
	BYTE                m_IsSerLock;              //�Ƿ���סĳ��
	BOOL                m_nIsOver;

    BOOL                m_bIsHideNpc;
    BOOL                m_bIsHidePlayer;
    BOOL                m_bIsHideMissle;
	BOOL                m_bIsHideLife;
	BOOL                m_bIsHideTong;
	BOOL                m_bIsHideNuqi;

	//char                m_PicPath[256];  //ͷ��·��
	//char                m_ScriptPicPath[256];//�ű�·��

	BYTE                m_BtnFlag;                //�Ƿ����״̬
	BOOL                m_BtnFindPath;            //�Ƿ��Զ�Ѱ·״̬
	int 				m_btStateInfo[MAX_NPC_RECORDER_STATE];	// Npc��ǰ���µļ���״̬  BYTE
//	DWORD				m_NpcEnchant;			                // Npc�ļ�ǿ���ͣ��ƽ���
	int					m_nNextStatePos;		                // ��һ��״̬��m_btState��λ��
	KNpcGold			m_cGold;

	//����Npcģ����У���FALSE��ʾ��Npc���ݵ�ǰ����Ч�� ����ֵδ�����ű����㣬��Ҫ����.
	//TRUE��ʾ��Ч����
	//BOOL				m_bHaveLoadedFromTemplate;//

	//KState			m_PowerState;			// ŭ��״̬

	//typedef std::map<INT, KState> _KStateNote;                 //SkillId,Echance
	//_KStateNote nKStateNote;
	KState				m_PoisonState;			// �ж�״̬
	KState				m_FreezeState;			// ����״̬
	KState				m_BurnState;			// ȼ��״̬
	KState				m_ConfuseState;			// ����״̬
	KState				m_StunState;			// ѣ��״̬
	KState				m_LifeState;			// ��Ѫ״̬
	KState				m_ManaState;			// ��MANA״̬
	KState				m_MenuState;			// �˵�
	KState				m_DrunkState;			// ���״̬
	KState				m_Hide;                 // ���أ�
	KState				m_ZhuaState;            // ��ץ��״̬
	KState				m_LoseMana;             // ����״̬
    KState				m_ExpState;             // ����״̬
	KState				m_DoScriptState;        // ִ�нű�״̬
    KState              m_randmove;             // Ⱥ���߶�״̬
	KState              m_MapUseModel;          // ��ͼ��������
	KState				m_PhysicsArmor;
	KState				m_ColdArmor;
	KState				m_LightArmor;
	KState				m_PoisonArmor;
	KState				m_FireArmor;
	KState				m_ManaShield;
	KState              m_Returnskill;          //�����˹����� �Զ��ͷ�ĳ���ܼ���
	KState              m_Deathkill;            //�����˹����� �Զ��ͷ�ĳ���ܼ���
	KState              m_Rescueskill; 	        //�����Զ��ͷż���
	KState              m_Replyskill; 	        //�Զ���������
	//KState            m_Staticmagicshield;    //ħ����״̬

	int                 m_AdjustColorKind;      //NPC�ĵ�ǰƫɫ
	int					m_RideState;            //Npc����������ʱ������

	KList				m_StateSkillList;		// ����״̬�����б�ͬ���ܲ����ӣ�
	BYTE				m_Camp;					// Npc����Ӫ
	BYTE				m_CurrentCamp;			// Npc�ĵ�ǰ��Ӫ
	NPCCMD				m_Doing;				// Npc����Ϊ
	CLIENTACTION		m_ClientDoing;			// Npc�Ŀͻ�����Ϊ
	char	            szNpcTypeName[32];
	DOING_FRAME			m_Frames;				// Npc����Ϊ֡��
	KSkillList			m_SkillList;			// Npc�ļ����б�
	int					m_SubWorldIndex;		// Npc���ڵ�SubWorld ID maps���� �ڼ�����ͼ
	int					m_RegionIndex;			// Npc���ڵ�Region ID
	int					m_ActiveSkillID;		// Npc����ļ���ID
	int                 m_ActiveSkListIndex;

	int					m_ActiveAuraID;			// Npc����Ĺ⻷����ID
	int                 m_ActiveAuraIndex;
	int                 m_ActiveXinYunXingID;

	bool                m_IsMoreAura;
	skillAuraInfo		m_TmpAuraID[5];         // ��ü���ܶ����ʱ�Ĺ⻷����ID
	int					m_ExtSkill[5];          // �ƺ���չ״̬����

//Npc��ʵ�����ݣ��Ѿ�����װ�������ܵ������ˣ�===================================
	int					m_CurrentExperience;	// Npc��ɱ���ͳ��ľ���
	int 				m_CurrentLife;			// Npc�ĵ�ǰ����
	int 				m_CurrentLifeDamage;    // ��Ҫ����ðѪ����
	int 				m_CurPoisonDamage;      // ��Ҫ����ðѪ����
	int 				m_CurFireDamage;        // ��Ҫ����ðѪ����
	int 				m_CurrentLifeMax;		// Npc�ĵ�ǰ�������ֵ
	int					m_CurrentLifeReplenish;	// Npc�ĵ�ǰ�����ظ��ٶ�
	int					m_CurrentLifeReplenish_p;// Npc�ĵ�ǰ�����ظ��ٶ� �ٷֱ�
    int   				m_CurrentMana;			// Npc�ĵ�ǰ����
	int				    m_CurrentManaMax;		// Npc�ĵ�ǰ�������
	int					m_CurrentManaReplenish;	// Npc�ĵ�ǰ�����ظ��ٶ�
	int                 m_CurrentManaReplenish_p;//
	int				    m_CurrentStamina;		// Npc�ĵ�ǰ����
	int				    m_CurrentStaminaMax;	// Npc�ĵ�ǰ�������
	int					m_CurrentStaminaGain;	// Npc�ĵ�ǰ�����ظ��ٶ�
	int					m_CurrentStaminaLoss;	// Npc�ĵ�ǰ�����½��ٶ�

	int					m_CurrentNuQi;			// Npc�ĵ�ǰŭ��ֵ
	int					m_CurrentNuQiMax;		// Npc�ĵ�ǰŭ�����ֵ
	int					m_CurrentNuQiReplenish;	// Npc�ĵ�ǰŭ���ظ��ٶ�
//////===================���ܹ������ݣ��ӽű���ã�=============================
	KMagicAttrib		m_PhysicsDamage;		        // Npc�Ļ����˺���(�����������������˺�������������ֱ�Ӽ��˺���ħ������)(������)
	KMagicAttrib		m_CurrentPhysicsMagicDamageP;	// Npc�ĵ�ǰ�յ�ٷֱ�
	KMagicAttrib		m_CurrentPhysicsMagicDamageV;	// Npc�ĵ�ǰ�յ��˺�
	KMagicAttrib		m_CurrentMagicFireDamage;	    // Npc�ĵ�ǰ���˺�
	KMagicAttrib		m_CurrentMagicColdDamage;	    // Npc�ĵ�ǰ���˺�
	KMagicAttrib		m_CurrentMagicLightDamage;	    // Npc�ĵ�ǰ���˺�
	KMagicAttrib		m_CurrentMagicPoisonDamage;	    // Npc�ĵ�ǰ���˺�
	int					m_CurrentAttackRating;	        // Npc�ĵ�ǰ�����ʣ���������+���������ʣ�
	int					m_CurrentDefend;		        // Npc�ĵ�ǰ�����ʵ�
///=============================================================================
	int 				m_CurrentSkillMingZhong;	// Npc�ĵ�ǰ�������ֵ
	int					m_TempFireResist;	        // Npc�ĵ�ǰ����
	int					m_TempColdResist;	        // Npc�ĵ�ǰ������
	int					m_TempPoisonResist;	        // Npc�ĵ�ǰ������
	int					m_TempLightResist;	        // Npc�ĵ�ǰ�翹��
	int					m_TempPhysicsResist;	    // Npc�ĵ�ǰ������

	int					m_CurrentFireResist;	    // Npc�ĵ�ǰ����
	int					m_CurrentColdResist;	    // Npc�ĵ�ǰ������
	int					m_CurrentPoisonResist;	    // Npc�ĵ�ǰ������
	int					m_CurrentLightResist;	    // Npc�ĵ�ǰ�翹��
	int					m_CurrentPhysicsResist;	    // Npc�ĵ�ǰ������

	int					m_CurrentFireResistMax;		// Npc�ĵ�ǰ������
	int					m_CurrentColdResistMax;		// Npc�ĵ�ǰ��������
	int					m_CurrentPoisonResistMax;	// Npc�ĵ�ǰ��󶾿���
	int					m_CurrentLightResistMax;	// Npc�ĵ�ǰ���翹��
	int					m_CurrentPhysicsResistMax;	// Npc�ĵ�ǰ���������

	//BOOL                m_AttackState;
	int                 m_CurrentTempSpeed;
	int					m_CurrentWalkSpeed;		// Npc�ĵ�ǰ�߶��ٶ�
	int					m_CurrentRunSpeed;		// Npc�ĵ�ǰ�ܶ��ٶ�
	int					m_CurrentJumpSpeed;		// Npc�ĵ�ǰ��Ծ�ٶ�
	int					m_CurrentJumpFrame;		// Npc�ĵ�ǰ��Ծ֡��
	int					m_CurrentAttackSpeed;	// Npc�ĵ�ǰ�����ٶ�  --�⹦
	int					m_CurrentCastSpeed;		// Npc�ĵ�ǰʩ���ٶ�  --�ڹ�
	int					m_CurrentVisionRadius;	// Npc�ĵ�ǰ��Ұ��Χ
	int					m_CurrentAttackRadius;	// Npc�ĵ�ǰ������Χ
	int					m_CurrentActiveRadius;	// Npc�ĵ�ǰ���Χ
	int					m_CurrentHitRecover;	// Npc�����˶���ʱ��
	int					m_CurrentHitNpcRecover;	// ��ɶԷ������˶������Ӷ���
	int					m_CurrentTreasure;		// Npc����װ��������
	int					m_CurrentHitRank;       // ���˵ĸ���
//��������ֻ��Ҫ��ǰֵ������ֵ��Ϊ0 = ����״̬����+װ�����ݣ�û״̬-ֱ�Ӹ�ֵ��==================================
	int					m_CurrentMeleeDmgRetPercent;	// Npc�����˺����صİٷֱ�
	int					m_CurrentMeleeDmgRet;			// Npc���̱���ʱ���ص��˺�����
	int					m_CurrentRangeDmgRetPercent;	// NpcԶ���˺����صİٷֱ�
	int					m_CurrentRangeDmgRet;			// NpcԶ�̱���ʱ���ص��˺�����
	BOOL				m_CurrentSlowMissle;			// Npc�Ƿ��������ӵ�״̬��
	int	                m_CurrentHulueMeleeDmgRet;      // ���Խ��̷������ٵ㡢
    int	                m_CurrentHulueRangeDmgRet;      // ����Զ�̷̳������ٵ㡢

	int					m_CurrentDamageReduce;			// �����˺�����
//	int					m_CurrentElementDamageReduce;	// Ԫ���˺�����

	int					m_CurrentDamage2Mana;			// �˺�ת�����ٷֱ�
//	int					m_CurrentManaPerEnemy;			// ɱһ�����˼Ӷ��ٵ����� //
	int					m_CurrentLifeStolen;			// ͵�����ٷֱ� ��Ѫ
	int					m_CurrentManaStolen;			// ͵�����ٷֱ� ����
	int					m_CurrentStaminaStolen;			// ͵�����ٷֱ�
	int					m_CurrentKnockBack;				// ���˰ٷֱ�
	int					m_CurrentDeadlyStrike;			// ����һ���ٷֱ�
//	int					m_CurrentBlindEnemy;			// ��ä���˰ٷֱ�
//	int					m_CurrentPiercePercent;			// ��͸�����ٷֱ�
	int					m_CurrentFreezeTimeReducePercent;	// ����ʱ����ٰٷֱ�
	int					m_CurrentPoisonTimeReducePercent;	// �ж�ʱ����ٰٷֱ�
	int					m_CurrentStunTimeReducePercent;		// ѣ��ʱ����ٰٷֱ�
	int					m_CurrentBurnTimeReducePercent;		// ȼ��ʱ����ٰٷֱ�
    int                 m_CurrentautoReviverate;            //��ǰ�ĸ������
	int					m_CurrentStunRank_p;		        // ����ѣ�θ������Ӱٷֱ�

	int					m_EnemyPoisonTimeReducePercent;	   // ��Է��ж�ʱ�����Ӱٷֱ�
	int					m_EnemyStunTimeReducePercent;       // ��Է�ѣ��ʱ�����Ӱٷֱ�

    int                 m_CurrentBuZhuoRate;                   //��ǰ�Ĳ������
	int				    m_CurrentUpExp;                        //���鱶��
	int					m_CurrentdanggeRate;		           //�������
    int					m_CurrentzhongjiRate;		           //�ػ�����
    int                 m_CurrentcjdanggeRate;                 //��⵵��
    int                 m_CurrentcjzhongjiRate;                //����ػ�
    int                 m_Currentsorbdamage;                   // �����˺� ����
    int                 m_Currentsorbdamage_v;                 // �����˺��ٷֱ�
	int                 m_Currenadddamagev;                    //�����˺���
	int                 m_Currenadddamagep;                    //�����˺��ٷֱ�
    int                 m_Currentpoisonres;                    //=���ԶԷ�����:#d1-%
    int                 m_Currentfireres;                      //=���ԶԷ����:#d1-%
    int                 m_Currentlightingres;                  //=���ԶԷ��׷�:#d1-%
    int                 m_Currentphysicsres;                   //=���ԶԷ��շ�:#d1-%
    int                 m_Currentcoldres;                      //=���ԶԷ�����:#d1-%
    int 	            m_Currentallres;                       //=���ԶԷ�ȫ��:#d1-%
	int                 m_CurrentIgnoredefensep;               //��ǰ���ԶԷ�������   //���ܵ�����
	int                 m_CurrentIgnorenAttacRating;           //��ǰ���ԶԷ�������   //״̬����
    int                 m_Currentnopkvalue;                    //=������PKֵ����:#d1+%
    int                 m_Currentbossdamage;                   //=�Իƽ�boss�����˺�<color=orange>#d1+%<color>
    int                 m_Currentelementsenhance;              //=����ǿ��ֵ��#d1-�㡣ǿ����������еĿ���Ч�� �ҿ�
    int                 m_Currentelementsresist;               //=��������ֵ��#d1-�㡣������������еĿ���Ч�� ����

    int                 m_Currentskillenhance;                 // ��������״̬��ǿ
    int                 m_CurrentFullManaskillenhance;         // ������ʱ�������ܼӳ�;
	//CEnhanceInfo        m_CurrentSkillEnhance[MAX_NPCSKILL]; // ��ĳ����״̬��ǿ
	typedef std::map<int, CEnhanceInfo> _EnhanceInfo;                 //SkillId,Echance
	_EnhanceInfo nEnhanceInfo;
	int					m_CurrentFireEnhance;			// ���ǿ
	int					m_CurrentColdEnhance;			// �ٻ�ʱ������
	int					m_CurrentPoisonEnhance;			// ����ʱ����
	int					m_CurrentLightEnhance;			// ѣ��ʱ�����
	int                 m_CurrentPoisonTime;            // ���ܵĶ���ʱ��

	int                 m_CurrentAttackRatingEnhancep;  // װ�����������ʼ�ǿ�ٷֱ�
    int                 m_CurrentAttackRatingEnhancev;  // װ�����������ʼ�ǿ��

	int					m_CurrentAddPhysicsDamage;		// ��ǰ�����⹦�յ� ֱ�ӵ������˺���ǿ����
	int                 m_CurrentAddPhysicsDamageP;     // ��ǰ�������հٷֱ�
   	int                 m_CurrentAddFireDamagev;        // ��ǰ��������
 	int                 m_CurrentAddColdDamagev;        // ��ǰ���������
	int                 m_CurrentAddLighDamagev;        // ��ǰ�������׵�
	int                 m_CurrentAddPoisonDamagev;      // ��ǰ�����ⶾ��
    BOOL                m_IsDel;                        // �Ƿ�ɾ��NPC
    int                 m_CurrentAddmagicphysicsDamage; // ��ǰ�������յ�
	int                 m_CurrentAddmagicphysicsDamageP;// ��ǰ�������հٷֱ�
	int                 m_CurrentAddmagicColdDamagicv;  // ��ǰ�����ڱ���
	int                 m_CurrentAddmagicFireDamagicv;  // ��ǰ�����ڻ��
	int                 m_CurrentAddmagicLightDamagicv; // ��ǰ�������׵�
	int                 m_CurrentAddmagicPoisonDamagicv;// ��ǰ�����ڶ���

	int					m_CurrentMeleeEnhance[MAX_MELEE_WEAPON];	// ���������ǿ
	int					m_CurrentRangeEnhance;			// Զ�������ǿ
	int					m_CurrentHandEnhance;			// ���������ǿ
	int					m_CurrentSerisesEnhance;        // ���е��˺���ǿ

    int                  m_CurrentPoisondamagereturnV;   //��ǰ��������
    int                  m_CurrentPoisondamagereturnP;   //��ǰ�������ٷֱ�
	int                  m_CurrentReturnskillp;          //��ǰ���䷴������
	int                  m_CurrentIgnoreskillp;          //�������䷴������
	int                  m_CurrentReturnresp;            //�Է����˺��Ŀ���
	int                  m_CurrentCreatnpcv;             //��ǰץ��BB�ĸ���
    int                  m_CurrentAllJiHuo;              //�Ƿ�ȫ����
	int                  m_CurrentCreatStatus;           //��ǰ������״̬
	int                  m_Currentbaopoisondmax_p;
	int                  m_nCurNpcLucky;

    int                  m_Me2metaldamage_p;              //=�Խ�ϵ�˺����ӣ�#d1+%
    int                  m_Metal2medamage_p;              //=�������Խ�ϵ���˺���#d1-%
    int                  m_Me2wooddamage_p;              //=��ľϵ�˺����ӣ�#d1+%
    int                  m_Wood2medamage_p;              //=��������ľϵ���˺���#d1-%
    int                  m_Me2waterdamage_p;              //=��ˮϵ�˺����ӣ�#d1+%
    int                  m_Water2medamage_p;              //=��������ˮϵ���˺���#d1-%
    int                  m_Me2firedamage_p;              //=�Ի�ϵ�˺����ӣ�#d1+%
    int                  m_Fire2medamage_p;              //=�������Ի�ϵ���˺���#d1-%
    int                  m_Me2earthdamage_p;              //=����ϵ�˺����ӣ�#d1+%
    int                  m_Earth2medamage_p;              //=����������ϵ���˺���#d1-%

	int                  m_Staticmagicshield_p;		      //�����������ֵ���ٱ����˺�

// ֻ��Ҫ��ǰֵ�����ݽ���
	int					m_Dir;							// Npc�ķ���
	int					m_RedLum;						// Npc������
	int					m_GreenLum;
	int					m_BlueLum;
	int					m_MapX, m_MapY, m_MapZ;			// Npc�ĵ�ͼ����(��������) x ��0-15�� y��0-31��
	int					m_OffX, m_OffY;					// Npc�ڸ����е�ƫ�����꣨�Ŵ���1024����
	int					m_DesX, m_DesY;					// Npc����Ҫ�ƶ�����Ŀ������
	int					m_SkillParam1, m_SkillParam2;   //
	int					m_OriginX, m_OriginY;			// Npc��ԭʼ���� ���������꣩
	DWORD				m_NextAITime;
	BYTE				m_AIMAXTime;                //NpcAI
// Npc��װ���������ͻ��˵Ļ�װ����
	int					m_HelmType;					// Npc��ͷ������
	int					m_ArmorType;				// Npc�Ŀ�������
	int					m_WeaponType;				// Npc����������
	int					m_HorseType;				// Npc����������
	BOOL				m_bRideHorse;				// Npc�Ƿ�����
	int					m_MaskType;					// Npc ��߹���
	//int				m_FuMoType;					// Npc ��ħ����
	int 				m_PifengType;				// Npc ���繦��
	BYTE				m_ChiBangType;				// Npc ���繦��
	int					m_MaskMark;					// ����г�
	//BYTE				m_IsFuMo;					//
	BYTE				m_BaiTan;					// Npc�Ƿ��ڰ�̯
	//char				ShopName[32];
//�ͻ���
	char				TongName[32];
	int					m_nFigure;
	char				Name[32];				// Npc������-���
	char                _clientName[32];
	BYTE				m_nSex;					// Npc���Ա�0Ϊ�У�1ΪŮ
	int					m_NpcSettingIdx;		// Npc���趨�ļ�����  ���кţ���
	int					m_CorpseSettingIdx;		// Npc��ʬ�嶨������
	//char				ActionScript[256];		// Npc����Ϊ�ű�
	typedef std::map<int, std::string> _StrNoteInfo;
	_StrNoteInfo nstrNoteInfo;

	void ClearstrInfo(int i)
	{
		if (nstrNoteInfo.count(i)>0)
		{
		   nstrNoteInfo.erase(i);
		}
	}

	char * GetstrInfo(int i,char * str)
	{
		//char nMsg[256]={0};
		/*_StrNoteInfo::iterator it;
		for( it = nstrNoteInfo.begin(); it != nstrNoteInfo.end(); ++it)
		{
			 if (it->first == i)
			 {
#ifdef _SERVER
				 printf("--��ȡ(%s)�ű�:%s --\n",Name,it->second.c_str());
#endif
				 sprintf(str,it->second.c_str());
			     return str;
			 }
		} */
		if (nstrNoteInfo.count(i)>0)
		{//�������
			sprintf(str,"%s", nstrNoteInfo[i].c_str());

/*#ifdef _SERVER
			printf("--��ȡ(%s)(%d)�ű�:%s --\n",Name,i,nstrNoteInfo[i].c_str());
#endif */

			return str;
		}
		return NULL;
	}
	void   SetstrInfo(int i,char * str)
	{
		if  (str)
			nstrNoteInfo[i] = str;
		else
			nstrNoteInfo[i] = "";
/*#ifdef _SERVER
			     printf("--����(%s)(%d)�ű�:%s --\n",Name,i,nstrNoteInfo[i].c_str());
#endif */
	}

	DWORD				m_ActionScriptID;		// Npc����Ϊ�ű�ID��ʹ��ʱ�������������

	DWORD				m_TrapScriptID;			// Npc�ĵ�ǰTrap�ű�ID;

	int					m_nPeopleIdx;			// ��ס�Ķ�����������
	int					m_nLastDamageIdx;		// ���һ���˺�����������
	int					m_nLastPoisonDamageIdx;	// ���һ�ζ��˺�����������
	int                 m_nLastBurnDamageIdx;	// ���һ�λ��˺�����������
	int					m_nObjectIdx;			// ������Ʒ

	// Npc�Ļ������ݣ�δ����װ�������ܵ�Ӱ�죩
	int					m_Experience;			// Npc��ɱ��õľ���
	int 				m_LifeMax;				// Npc���������
	int					m_LifeReplenish;		// Npc�������ظ��ٶ�

	int					m_NuqiMax;		        // Npcŭ�����ֵ
	int					m_NuqiReplenish;	    // Npcŭ���ظ��ٶ�

	int				    m_ManaMax;				// Npc���������
	int					m_ManaReplenish;		// Npc�������ظ��ٶ�

	int				    m_StaminaMax;			// Npc���������
	int					m_StaminaGain;			// Npc�������ظ��ٶ�
	int					m_StaminaLoss;			// Npc�������½��ٶ�

	int					m_AttackRating;			// Npc�����е㣨������ԭʼ�����ʣ�
	int					m_Defend;				// Npc��������
	int					m_FireResist;			// Npc�Ļ���
	int					m_ColdResist;			// Npc���俹��
	int					m_PoisonResist;			// Npc�Ķ�����
	int					m_LightResist;			// Npc�ĵ翹��
	int					m_PhysicsResist;		// Npc��������

	int					m_FireResistMax;		// Npc��������
	int					m_ColdResistMax;		// Npc����������
	int					m_PoisonResistMax;		// Npc����󶾿���
	int					m_LightResistMax;		// Npc�����翹��
	int					m_PhysicsResistMax;		// Npc�����������

	int					m_WalkSpeed;			// Npc�������ٶ�
	int					m_RunSpeed;				// Npc���ܶ��ٶ�
	int					m_JumpSpeed;			// Npc����Ծ�ٶ�
	int					m_AttackSpeed;			// Npc���⹦�����ٶ�
	int					m_CastSpeed;			// Npc���ڹ�ʩ���ٶ�
	int					m_VisionRadius;			// Npc����Ұ��Χ
	int					m_DialogRadius;			// Npc�ĶԻ���Χ
	int					m_ActiveRadius;			// Npc�Ļ��Χ
	int					m_HitRecover;			// Npc���ܻ��ظ��ٶ�
	int					m_Treasure;				// Npc����װ��������(NPC.txt����)
	BOOL				m_bClientOnly;			// �Ƿ���ؿͻ���NPC����
//	int					m_AttackFrame;          // �⹦����֡��
//	int					m_CastFrame;            // �ڹ�����֡��
//	int					m_nCurrentMeleeSkill;	// Npc��ǰ��ִ�еĸ񶷼���
	int					m_nCurrentMeleeTime;

	// AI����
	int					m_AiMode;				// AIģʽ  �һ�ģʽ
	int					m_AiParam[MAX_AI_PARAM];// ����AIģ�����AI
	int					m_AiAddLifeTime;

	int					m_HeadImage;

	BYTE				m_FightMode;			// �ͻ��˴������á� ս��ģʽ
	BYTE				m_BayBan;			    // bayban
	BYTE				m_OldFightMode;
	BOOL				m_bExchangeServer;
	BYTE				m_ExItemId;             // ��ĸ��
	BYTE				m_ExBoxId;              // ��չ��
	DWORD				m_SyncSignal;			// ͬ���ź�
	KClientNpcID		m_sClientNpcID;			// ���ڱ����ͻ���npc���ĸ�region�ĵڼ���npc
	DWORD				m_dwRegionID;			// ��npc����region��id
	KSyncPos			m_sSyncPos;
	BYTE				m_nPKFlag;	               //PKģʽ
	char				m_szChatBuffer[MAX_SENTENCE_LENGTH];
	int					m_nChatContentLen;
	int					m_nChatNumLine;
	int					m_nChatFontWidth;
	unsigned int		m_nCurChatTime;
	BYTE				m_nSleepFlag;
	int					m_nHurtHeight;
	int					m_nHurtDesX;
	int					m_nHurtDesY;
	int                 m_PiFenLoop;
	BYTE				m_nTongFlag;			// �Ƿ�������ͼ��
	int                 m_IsbeSel;              // �Ƿ�ѡ����

	int					m_DeathFrame;			// ����֡��
	int					m_StandFrame;
	int					m_HurtFrame;
	int					m_AttackFrame;
	int					m_CastFrame;
	int					m_WalkFrame;
	int					m_RunFrame;
	int					m_StandFrame1;
	int					m_ReviveFrame;			// ����֡�� ʱ��
	int					m_SitFrame;
	int					m_JumpFrame;
private:
	bool                isRemoveMenu;
	DWORD				m_LoopFrames;			// ѭ��֡��
	int					m_nPlayerIdx;           // ���������

	int					m_JumpFirstSpeed;
	NPC_COMMAND			m_Command;				// ����ṹ
	BOOL				m_ProcessAI;			// ����AI��־
	BOOL				m_ProcessState;			// ����״̬��־
	int					m_XFactor;
	int					m_YFactor;
	int					m_JumpStep;
	int					m_JumpDir;
	int					m_SpecialSkillStep;		// ���⼼�ܲ���
	//NPC_COMMAND			m_SpecialSkillCommand;	// ���⼼����Ϊ����
	KNpcFindPath		m_PathFinder;
	BOOL				m_bActivateFlag;         //�Ƿ�ִ��ѭ��
	int					m_ResDir;
	KNpcRes				m_DataRes;				// Npc�Ŀͻ�����Դ��ͼ��������
   	int                 m_DurFrame[20];
    unsigned int        uFlipTime[20];

	// ������ͨս��npcðѪ����
	int			    	m_nBloodNo[defMAX_SHOW_BLOOD_NUM][2];
	//int				m_nBloodAlpha;
	char				m_szBloodNo[64];
private:
	BOOL				WaitForFrame(); //�ȴ�����֡��
	BOOL				IsReachFrame(int nPercent);
	void				DoStand();
	void				OnStand();
	void				DoRevive(); //����
	void				OnRevive(); //����
	void				DoWait();
	void				OnWait();
	void				DoWalk();
	void				OnWalk();
	void				DoRun();
	void				OnRun();
	void				DoSkill(int nX, int nY);
	int					DoOrdinSkill(KSkill * pSkill, int nX, int nY);
	void				OnSkill();
	void				DoAttack();
	void				DoMagic();
	void				DoJump();
	BOOL				OnJump();
	void				DoSit();
	void				OnSit();
	void				DoHurt(int nHurtFrames = 0, int nX = 0, int nY =0,int nRank=0);
	void				OnHurt();
	void				DoDeath(int nMode = 0,int nLastDamageIdx=0);
	void				OnDeath();
	void				DoDefense(); // ����
	void				OnDefense();  //����
	void				DoIdle();
	void				OnIdle();
//	�йظ񶷼��ܵ�------------------------------------------

	BOOL				DoSecMove(int nDistance); //�Ƕϻ�� ���� ˲�ƣ�
	BOOL				DoManyAttack();
	void				OnManyAttack();
	BOOL				DoBlurAttack();
	BOOL				DoJumpAttack();
	BOOL				OnJumpAttack();
	BOOL				DoRunAttack();
	void				OnRunAttack();
	BOOL				CastMeleeSkill(KSkill * pSkill);
//-----------------------------------------------------------
	void				OnPlayerTalk();
	void				DoSpecial1();
	void				OnSpecial1();
	void				DoSpecial2();
	void				OnSpecial2();
	void				DoSpecial3();
	void				OnSpecial3();
	void				DoSpecial4(int nX, int nY);
	void				OnSpecial4();
	void				Goto(int nMpsX, int nMpsY);
	void				RunTo(int nMpsX, int nMpsY);
	void				JumpTo(int nMpsX, int nMpsY);
	void				ServeMove(int nSpeed);
	void				ServeJump(int nSpeed);
//	void				Enchant(void);
//	void				ModifyEnchant(void* pData, int nAttrib);
	BOOL				NewPath(int nMpsX, int nMpsY);
	BOOL				NewJump(int nMpsX, int nMpsY);
	void                MovePos(int nMoveX, int nMoveY);
	BOOL				CheckHitTarget(int nAR, int nDf, int nIngore = 0,int nInHuLieAr=0);
	void                DoDrag(int nDragFrame, int nDesX, int nDesY);

	void				HurtAutoMove();

public:
	friend class KNpcAttribModify;
	friend class KThiefSkill;
	KNpc();
	~KNpc();

	//void                ClearEnhanceNote();
	DWORD               GetLoopFrames(){return m_LoopFrames;};
	void				SetActiveFlag(BOOL bFlag) { m_bActivateFlag = bFlag; };
	void				DoPlayerTalk(char *);
	void				CheckTrap();
//	int                 CheckWaiGua(char *exe_name);
	void                ReSkillEnhance(int nLeftSkill,int nIsAdd=-1);//���öԹ������ܵļӳ�
	void                ReFullManaSkillEnhance(int nLeftSkill,int nLeftListidx);
	void                ReWhereSkillEnhance(int nActiveSkillID,int nCurSkillEnchance,int nIsAdd=-1);
	//int               NpcInitJinMaiVal();                            //������ʼ��
	int                 NpcNewInitJinMaiVal();
	void                UseSkills(int sKillID,int sLevel,int nNpcIdx,KMagicAttrib *pData=NULL,BOOL nIfMagic=TRUE,int nTime=-1); //���ü���״̬Ч��
	void				Init();
	void				Remove();
	//DWORD             TakeTrader(DWORD a,DWORD b); //ȡ��
	//DWORD             TakeRemainder(DWORD a,DWORD b);	//ȡ��
	void				Activate();
	//void				ActivateDrop();	//���ʵ�ѭ��
	void                NewSetNpcEnChance();
	BOOL				IsPlayer();
	void                GetTongBanDamage(int* nMin, int* nMax);

	/*int               GetCurFuMoNpcNo(int idx);
	int                 GetCurFuMoIdx();
	int                 GetCurFoMoSkllLevel(int idx);
	*/
	void				SetFightMode(BOOL bFightMode);
	void				SetBayBan(BOOL bBayBan);//bayban
	void				TurnTo(int nIdx);
	void                setNpcDir(int nX2,int nY2);
	int                 GetAttackFrame(){return m_AttackFrame;};
	int                 GetCastFrame(){return m_CastFrame;};
	void                SetAttackFrame(int nCurFrame){ m_AttackFrame=nCurFrame;};
	void                SetCastFrame(int nCurFrame){m_CastFrame=nCurFrame;};
	void				SendSerCommand(NPCCMD cmd, int x = 0, int y = 0, int z = 0);
	void				ProcCommand(int nAI);
	ISkill* 			GetActiveSkill();
	int                 GetSkillLeftTime(int nSkillId);//��ȡ������ܵ�״̬��ʣ��ʱ��
	int                 SetSkillLeftTime(int nSkillId,int nTime);   //����������ܵĴ��ʱ��
	void                ClearOneSkillState(int nSkillId);

	BOOL				ProcessState();
	void				ProcStatus();
	void				ModifyAttrib(int nAttacker, void* pData);
	int					GetSkillLevel(int nSkillId);
	void				SetId(DWORD	dwID)	{ m_dwID = dwID;};
	BOOL				IsMatch(DWORD dwID)	{ return dwID == m_dwID; };	// �Ƿ�ID���Indexƥ��
	BOOL				Cost(NPCATTRIB nType, int nCost, BOOL bOnlyCheckCanCast = FALSE);				// ��������������,���OnlyCheckCanCostΪTRUE,��ʾֻ�Ǽ�鵱ǰ���������Ƿ����ģ�����ʵ�ʵĿ�
	//void				SelfDamage(int nDamage);						// ������˺�������������
	void				Load(int nNpcSettingIdx, int nLevel,int nSubWorld,int nBoss=0);						// ��TabFile�м���
	void				GetMpsPos(int * pPosX, int *pPosY,int *nMapid);
	BOOL				SetActiveSkill(int nSkillIdx);
	BOOL	            ChangeSkillAttackRadius(int nSkillIdx,int nDis);
	int				    GetSkillID(int nSkillIdx);
	void                UpdataNpcCurData(int nWonerIndex=0,int nPlace=-1);
	void                ReCalcNpcEquip(int nWonerIndex,int nEquipPlace=-1);
	BOOL                UpMagicAttrib(int i,int nWonerIndex);
	void				SetAuraSkill(int nSkillID);
	void				SetCamp(int nCamp);
	void				SetRankFF(int nRankFF);
	void                SetPlayFF(int nPlayFF);
	void				SetCurrentCamp(int nCamp); //���õ�ǰ��Ӫ
	void				RestoreCurrentCamp();      //�ָ���Ӫ
	void				SetStateSkillEffect(int nLauncher, int nSkillID, int nLevel, void *pData, int nDataNum, int nTime = -1,int nIsEuq=0);	// �������������뱻������
	void                SetToolStateSkillEffect(int nLauncher, int nSkillID, int nLevel, void *pData, int nDataNum, int nTime=-1);
	void                SetToolNoStateEffect(int nLauncher, int nSkillID, int nLevel,int nTime);
	void				SysnCastSkillEffect(int nLauncher,int nSkillID,int nParam1,int nParam2,int nLevel, int nTime = -1,int mMaxBei=1);
	void				ClearStateSkillEffect();//���NPC���ϵķǱ�����ļ���״̬nMaxBeiLv
	void				ReCalcStateEffect();
	void				ClearNormalState();
	void				SetImmediatelySkillEffect(int nLauncher, void *pData, int nDataNum);
	void				AppendSkillEffect(int nIsMaigc,BOOL bIsPhysical, BOOL bIsMelee, void *pSrcData, void *pDesData, int nEnChance);
	int					ModifyMissleLifeTime(int nLifeTime);
	int					ModifyMissleSpeed(int nSpeed);
	BOOL				ModifyMissleCollsion(BOOL bCollsion);
	void				RestoreNpcBaseInfo(); //Set Current_Data ;
	int                 GetNpcPepoleID(){return m_nPlayerIdx;};
//	int                 GetNpcIsReviveID(){return m_IsRevive;};
	void				SetNpcIsRe(int nIsRe,int nTime) {m_IsRevive = nIsRe; m_ReviveFrame= nTime;};
	void				SetNpcIsReFrame(int nIsRe,int nTime) {m_IsRe = nIsRe; m_ReviveFrame= nTime;};
	void				RestoreState();
	void				ClearNpcState();
	BOOL				SetPlayerIdx(int nIdx);
	void				DialogNpc(int nIndex);
	void				Revive(); //c����hon
	void				AddBaseLifeMax(int nLife);	// ���ӻ������������
	void				SetBaseLifeMax(int nLifeMax); // ���û������������
	void				AddCurLifeMax(int nLife);
	void				AddBaseStaminaMax(int nStamina);// ���ӻ������������
	void				SetBaseStaminaMax(int nStamina);
	void				AddCurStaminaMax(int nStamina);
	void				AddBaseManaMax(int nMana);	// ���ӻ������������
	void				SetBaseManaMax(int nMana);
	void				AddCurManaMax(int nMana);
    void                NpcFuMoCastSkll(int nLauncher, int nParam1, int nParam2, int nWaitTime=0);
//	void				ResetLifeReplenish();		// ���¼��������ظ��ٶ�
//	void				CalcCurLifeMax();			// ���㵱ǰ���������
//	void				CalcCurStaminaMax();		// ���㵱ǰ���������
//	void				CalcCurManaMax();			// ���㵱ǰ���������
	void				CalcCurLifeReplenish();		// ���㵱ǰ�����ظ��ٶ�
	void				SetSeries(int nSeries);// �趨�� npc ���������ԣ����ݻ�û��ɣ�
    void	            SetFangAndHai(KChanelBaseInfo *nNpcInfo);//�ı�NPC���ķ������ڴ���BB
	void	            SetBaseDamage(int nMix,int nMax,int nType=0);
	void	            GetFangAndHai(KChanelBaseInfo *nNpcInfo);
//	int					GetNpcLevelDataFromScript(KLuaScript * pScript, char * szDataName, int nLevel, char * szParam);
//	int					SkillString2Id(char * szSkillString);
	void				GetNpcCopyFromTemplate(int nNpcTemplateId, int nLevel);
//	void				InitNpcLevelData(KTabFile * pTabFile, int nNpcTemplateId, KLuaScript * pLevelScript, int nLevel);
//	void				InitNpcBaseData(int nNpcTemplateId);
	void				SetPhysicsDamage(int nMinDamage, int nMaxDamage);	// �趨�������������Сֵ
	void				SetBaseAttackRating(int nAttackRating);					// �趨����������
	void				SetBaseDefence(int nDefence);							// �趨������
//	void				SetBaseWalkSpeed(int nSpeed);							// �趨�����ٶ�
//	void				SetBaseRunSpeed(int nSpeed);							// �趨�ܲ��ٶ�
	int					GetCurActiveWeaponSkill();
	void				LoadDataFromTemplate(void * nNpcTemp=NULL);//(int nNpcTemplateId, int nLevel);
    //unsigned int	    GetColor(const char* pString); //���ַ�����ɫת����16����
	//const               char* GetColorString(unsigned int nColor); //��16������ɫת���� �ַ�����ʽ
	void				ReSetRes(int nMark);
	void				GetFrameCopyFromTemplate(int nNpcTemplateId, int nLevel);
	void				LoadFrameFromTemplate(void * nNpcTemp=NULL);//(int nNpcTemplateId, int nLevel);
//	int                 NpcInitJinMaiVal(); //������ʼ��
//	void                UseSkills(int sKillID,int sLevel,int nNpcIdx); //���ü���״̬Ч��
//    int				GetPlayerIdx();
	int	                CheckMaps(char *nKey,int nMapIdx);
	int	                CheckAllItem(int nKeyCol,int nGen,int nDetail,int nPart);
	BOOL				TestMovePos(int& nMpsX,int& nMpsY,int& nLength, int nSpeed, BOOL bCanJumpOver);

	void                OnDrag();	                       // ���� ����ƶ�

	void                SetClientSpr(char *nSprPath,int nxLeft,int nyTop,int nzPos=0,int i=0,char *ncPart=NULL);//���� ͬʱ���е�SPR����
	void				SetSleepMode(BOOL bSleep) { m_nSleepFlag = bSleep; /*m_DataRes.SetSleepState(bSleep);*/};
	void				SetNpcState(int* pNpcState);
	void				ActivateWaiGua();
	void				RemoveRes();
	void			    ClientShowMsg(const char *Msg);
	void				ProcNetCommand(NPCCMD cmd, int x = 0, int y = 0, int z = 0);
	void				Paint();
	int					PaintInfo(int nHeightOffset, bool bSelect, int nFontSize = 12, DWORD	dwBorderColor = 0XFF000000);
	void				PaintHonor(int nbtRankFFId,int nMpsX,int nMpsY,int i); //spr�ƺ�
	void				PaintPifeng(int m_PifengType,int nMpsX,int nMpsY,int i); //spr����ƺ�
//	void				PaintHonor(char szString[16], int nHeightOffset, int nFontSize = 12, DWORD	dwBorderColor = 0);
	int					PaintChat(int nHeightOffset);
	int					SetChatInfo(const char* Name, const char* pMsgBuff, unsigned short nMsgLength);//�Լ�NPC˵��
	int					PaintLife(int nHeightOffset, bool bSelect);
	int					PaintMana(int nHeightOffset);
	void				DrawBorder();
	int					DrawMenuState(int n);
	void				DrawBlood();	//����Ѫ���������ڹ̶�λ��
	BOOL				IsCanInput() { return m_ProcessAI; };//����AI ��־
	void				SetMenuState(int nState, char *lpszSentence = NULL, int nLength = 0);	// �趨ͷ��״̬
	int					GetMenuState();				// ���ͷ��״̬
	DWORD				SearchAroundID(DWORD dwID);	// ������Χ9��Region���Ƿ���ָ�� ID �� npc
	void				SetSpecialSpr(char *lpszSprName);// �趨�����ֻ����һ�������spr�ļ�
	void				SetFrameSpr(char *lpszSprName, int nX, int nY, int nHeight,int mInterval=0);
	void				SetInstantSpr(int nNo);
	int					GetNormalNpcStandDir(int nFrame);
	KNpcRes*			GetNpcRes(){return NULL;/*&m_DataRes;*/};  //�õ�NPC�����
  //KImageParam	        imgParam;
	int                 GetNpcPate();
	int                 GetNpcPatePeopleInfo();

	// ðѪ����
	void				ClearBlood(int i);
	void				SetBlood(int nNo);
	int				    PaintBlood(int nHeightOffset);	//����ðѪ
	int	                PaintOther();
	int	                PaintNewBlood(int nHeightOffset,int Val);
	void                ApplySynNpcInfo(DWORD nNpcDwid);
};

//�����ڴ���� MAX_NPC
extern  KNpc *Npc;//Npc[MAX_NPC];
#endif


