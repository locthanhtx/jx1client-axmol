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
// #include <hash_map>
// #include <algorithm>
// #include "KSubworld.h"
#include "KNpcRes.h"
//---------------------------------------------------------------------------
#define MAX_NPC      200  // NPcÏÞÖÆ  500
#define VER_INFO     "50JX-×¨ÓÃ¶Ë"
#define MAX_NPCSTYLE 3000  // npcÄ£°åÊýÁ¿ 2000
// #define VER_INFO "½£ÏÀ<color=red>¡ï<color>½­ºþ-×¨ÓÃ¶Ë"
#define defMAX_SHOW_BLOOD_NUM 5   // 50
#define MAX_AI_PARAM          10  // AIÄ£Ê½ Àà±ð
#define MAX_NPC_USE_SKILL     4   // NPC¹ÖÎï×î´ó¼¼ÄÜÊýÁ¿

#define MAX_FUMO_COUNT        5  // Éñ½«µÄÊýÁ¿
#define defLOGIC_CELL_WIDTH   32

// #define	TABFILE_MAGICALEVEL_PATH	"\\Settings\\item\\004\\magicattriblevel_index.txt"

// #define	KItemNormalAttrib KMagicAttrib
//	ÅÜ¶¯ËÙ¶ÈÓëÐÐ×ßËÙ¶ÈµÄ±¶Êý
// #define	WALK_RUN_TIMES	3

#define STATE_FREEZE  0x01  // 2 µÄ 0 ´Î·½
#define STATE_POISON  0x02  // 2 µÄ 1 ´Î·½
#define STATE_CONFUSE 0x04  // 4 2µÄ 2´Î·½
#define STATE_STUN    0x08  // 8 2µÄ 3 ´Î·½
#define STATE_HIDE    0x10  // 16 2µÄ 4 ´Î·½
// #define		STATE_MOVE		0x32   //32 2µÄ 5 ´Î·½

// #define		STATE_ZHUA		0x09
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
    do_none,          // Ê²Ã´Ò²²»¸É
    do_stand,         // Õ¾Á¢
    do_walk,          // ÐÐ×ß
    do_run,           // ÅÜ¶¯
    do_jump,          // ÌøÔ¾
    do_skill,         // ·¢¼¼ÄÜµÄÃüÁî
    do_magic,         // Ê©·¨
    do_attack,        // ¹¥»÷
    do_sit,           // ´ò×ø
    do_hurt,          // ÊÜÉË
    do_death,         // ËÀÍö
    do_defense,       // ¸ñµ²
    do_idle,          // ´­Æø
    do_specialskill,  // ¼¼ÄÜ¿ØÖÆ¶¯×÷
    do_special1,      // ÌØÊâ1
    do_special2,      // ÍµÇÔ¼¼ÄÜ
    do_special3,      // ÌØÊâ3
    do_special4,      // ÒÆ¶¯
    do_runattack,
    do_manyattack,
    do_jumpattack,
    do_revive,  // ÖØÉú
    do_stall,
    do_movepos,        // Ë²¼äÒÆ¶¯
    do_knockback,      // ÕðÍË
    do_drag,           // À­³¶¹ýÀ´
    do_rushattack,     // ³å¿³
    do_runattackmany,  // ³å´Ì¶àÈË
    do_num,
    /*	do_sunyi,       //Ë²¼äÒÆ¶¯
            do_yidong,      //¿çµØÍ¼ÒÆ¶¯

            do_movepos,		// Ë²¼äÒÆ¶¯
            do_knockback,	// ÕðÍË
            do_drag,		// À­³¶¹ýÀ´
            do_rushattack,	// ³å¿³
            do_runattackmany, //³å´Ì¶àÈË */
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
// ÉËº¦ÀàÐÍ
enum DAMAGE_TYPE
{
    damage_physics = 0,  // ÎïÀíÉËº¦
    damage_fire,         // »ðÑæÉËº¦
    damage_cold,         // ±ù¶³ÉËº¦
    damage_light,        // ÉÁµçÉËº¦
    damage_poison,       // ¶¾ËØÉËº¦
    damage_magic,        // ÎÞÊôÐÔÉËº¦
    damage_num,          // ÉËº¦ÀàÐÍÊýÄ¿
};

// DoDeath Ê±µÄ²ÎÊý£¬¶ÔÓ¦²»Í¬µÄËÀÍö³Í·£
enum DEATH_MODE
{
    DEATH_MODE_NPC_KILL = 0,      // ±»npcÉ±ËÀ0
    DEATH_MODE_PLAYER_NO_PUNISH,  // ÇÐ´èÄ£Ê½±»Íæ¼ÒÉ±ËÀ  1  Ã»ÓÐ³Í·£
    DEATH_MODE_PLAYER_PUNISH,     // ±»Íæ¼ÒPKÖÂËÀ£¬¸ù¾ÝPKÖµ½øÐÐ³Í·£2
    DEATH_MODE_PKBATTLE_PUNISH,   // ÀàÊ½ÓÚ¹úÕ½Ê±µÄ³Í·£´¦Àí 3
    DEATH_MODE_EXP_PUNISH,        // ¾­Ñé³Í·£
    DEATH_MODE_MONEY_PUNISH,      // ÒøÁ½³Í·£
    DEATH_MODE_EQUIP_PUNISH,      // ×°±¸³Í·£
    DEATH_MODE_JINBI_PUNISH,      // ½ð±Ò³Í·£
    DEATH_MODE_NUM,
};

typedef struct NPC_COMMAND
{
    NPCCMD CmdKind;  // ÃüÁîC
    int Param_X;     // ²ÎÊýX
    int Param_Y;     // ²ÎÊýY
    int Param_Z;     // ²ÎÊýY
};

typedef struct DOING_FRAME
{
    int nTotalFrame;    // È«²¿Ö¡
    int nCurrentFrame;  // µ±Ç°Ö¡
    void SetFrame(int nTotal)
    {
        if (nTotal <= 0)  // Ò»°ã²»ÐèÒªÊ±¼äÎª0µÄ¶¯×÷
            nTotalFrame = 1;
        else
            nTotalFrame = nTotal;

        nCurrentFrame = 0;
    };
};

struct KState
{
    int nMagicAttrib;
    int nValue[3];
    int nTime;
};
// ¸Ä±äNPCµÄ»ù±¾Êý¾Ý½á¹¹
struct KChanelBaseInfo
{
    int PhysicsDamage;    // »ù±¾ÉËº¦
    int m_PhysicsDamage;  // NpcµÄµ±Ç°ÉËº¦
    int m_FireDamage;     // NpcµÄµ±Ç°»ðÉËº¦
    int m_ColdDamage;     // NpcµÄµ±Ç°±ùÉËº¦
    int m_LightDamage;    // NpcµÄµ±Ç°µçÉËº¦
    int m_PoisonDamage;   // NpcµÄµ±Ç°¶¾ÉËº¦

    int m_FireResist;     // NpcµÄµ±Ç°»ð¿¹ÐÔ
    int m_ColdResist;     // NpcµÄµ±Ç°±ù¿¹ÐÔ
    int m_PoisonResist;   // NpcµÄµ±Ç°¶¾¿¹ÐÔ
    int m_LightResist;    // NpcµÄµ±Ç°µç¿¹ÐÔ
    int m_PhysicsResist;  // NpcµÄµ±Ç°ÎïÀí¿¹ÐÔ
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

// Éñ½«Àà
struct CFuMoInfo
{
    int nNpcIdx;
    int nNpcSetings;      // NPC settings	´æµµ
    int nNpcSkill;        // Í¬°éµÄ¼¼ÄÜid ´æµµ
    int nSkillLevel;      // ¼¼ÄÜµÈ¼¶     ´æµµ
    int nNowLevel;        // Í¬°éÏÖÔÚµÄµÈ¼¶
    unsigned long nCastTime;      // ÊÍ·ÅÊ±¼ä¼ä¸ô
    unsigned long nNextCastTime;  // ÏÂ´ÎÊÍ·ÅÊ±¼ä
    int nNowExp;          // ÏÖÔÚµÄ¾­Ñé
};

// ¼¼ÄÜ¼Ó³É
struct CEnhanceInfo
{
    int nSkillIdx;
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
struct KSyncPos
{
    unsigned long m_dwRegionID;
    int m_nMapX;
    int m_nMapY;
    int m_nOffX;
    int m_nOffY;
    int m_nDoing;
};

struct KNpcAutoChat
{
    int m_Model;  //
    // char                m_NpcTalkMsg[250];
    int m_NpcTalkTime;
    // char                m_NpcTalkName[64];
};

class KStateNode : public KNode
{
public:
    int m_SkillID;                          // ¼¼ÄÜID
    int m_Level;                            // ¼¼ÄÜµÈ¼¶
    int m_LeftTime;                         // Ê£ÓàÊ±¼ä
    KMagicAttrib m_State[MAX_SKILL_STATE];  // ÊôÐÔÊýÁ¿×´Ì¬ÁÐ±í
    // KMagicAttrib	m_JinMaiState;	            // ¾­Âö×´Ì¬ÁÐ±í
    int m_StateGraphics;  // ×´Ì¬¶¯»­Ë÷Òý
    int m_IsClientState;  // ÊÇ·ñ¶Ô·½¿´±¾ÈËµÄ×´Ì¬
};

class KNpc
{
    friend class KNpcSet;

public:
    sNpcShadowInfo _NpcShadow;
    unsigned long m_dwID;  // NpcµÄID  µÚ¸öNPC ±àºÅ ÎÞÏÞÔö´ó
    // BOOL              m_IsSyn;
    unsigned long m_bLockNpcDwID;  // Ëø¶¨NPCµÄ	DWid
    BYTE m_IsHaveAttack;   // ÊÇ·ñÉèÖÃÎª¹¥»÷ÎÞÐ§ÁË
    unsigned long m_AttackerDwid;  // ÉÏ´Î¹¥»÷×ÅµÄDWID
    unsigned long m_IsRevive;      // ÊÇ·ñÖØÉú
    BYTE m_IsRe;           // Í¬²½ÖØÉúÖ¡Êý
    // bool                m_isClearSpr;           //ÊÇ·ñÍ¬²½µ½¿Í»§¶ËÉ¾³ý¾«Áé
    int m_Index;        // NpcµÄË÷Òý
    KIndexNode m_Node;  // Npc's Node
    int m_Level;        // NpcµÄµÈ¼¶
    BYTE m_Kind;        // NpcµÄÀàÐÍ
    BYTE m_Series;      // NpcµÄÏµ
    int m_Height;       // NpcµÄ¸ß¶È(ÌøÔ¾µÄÊ±ºò·ÇÁã)
    int m_btRankId;     // ³ÆºÅµÄid
    int m_NpcTitle;     // NPCÐÂ³ÆºÅ£¨´æµµ£©
    int m_CurNpcTitle;  // µ±Ç°³ÆºÅ	 £¨²»´æµµÁÙÊ±³ÆºÅ£©
    BYTE m_btRankFFId;  // SPR³ÆºÅµÄ
    BYTE m_AutoplayId;  // ¹Ò»úSPR³ÆºÅµÄ
    int nRankInWorld;   // ÊÀ½çÅÅÃû
    int nLevelInWorld;  // ÊÀ½çµÈ¼¶ÅÅÐÐ
    int nRepute;        // ÉùÍû
    int nPKValue;
    int nReBorn;       // ×ªÉú´ÎÊý
    int m_nStature;    // Tall
    int m_CJtaskID;    // 172³é½±×¨ÓÃ
    int m_KillNumber;  // É±ÈËÊýÁ¿ ÅÅÃûµ÷ÓÃ
    //	CJinMaiInfo         m_JinMai[10];           //0 0 Îª½îÂö´óÀà
    // CFuMoInfo         m_nFuMoNum[MAX_FUMO_COUNT];          //¸½Ä§ÁÐ±í
    int m_nCurJiHuo;      // µ±Ç°¼¤»îµÄ¸½Ä§µÄNPC
    int m_ZhenYuan;       // ÕæÔªÖµ
    int m_JinMaiBingJia;  // ¾­Âö±ø¼×Öµ
    int m_GameliveTime;   // NPC ´æ»îÊ±¼ä
    int m_TempliveTime;   // ÁÙÊ±±£´æ´æ»îÊ±¼ä
    int m_liveType;       // Ê±¼äÉè¶¨µÄÀàÐÍ
    // BYTE                m_WhereSer;		        //ÄÇ¸ö·þÎñÆ÷µÄ

    /// ÁÙÊ±²ÎÊý/////²»¼ÓÈëÊý¾Ý¿â
    int m_njb;   // Ð¯´øµÄ½ð±Ò
    int m_njxb;  // Ð¯´øµÄ½£ÏÀB
    // char				m_WarTongNamea[32];     //Õ¼Áì°ï»á
    // char				m_WarMaster[32];        //³ÇÖ÷
    //	int				    m_WarShuishou;          //Ë°ÊÕ
    //	int                 m_Warzhi;               //Îï¼ÛÖ¸Êý
    int m_WarCityGX;    // ¸öÈË¹±Ï×
    int m_IsWarCity;    // ÄÄ¸ö³ÇµÄ³ÉÔ±
    int m_IsInCity;     // ÊÇ·ñ¼ÓÈë³ÇÊÐ
    int m_nsPlayerIdx;  // Íæ¼ÒµÄ·þÎñÆ÷Í¬²½¹ýÀ´µÄ
    int m_nMissionGroup;
    /*	BYTE                 m_WarFucheng;           //¸±³É
        BYTE                 m_WarZuoHu;             //×ó»¤·¨
            BYTE                 m_WarYouHu;             //ÓÒ»¤·¨
            BYTE                 m_WarTaishi;            //Ì«Ê·
            BYTE                 m_WarZhongShu;          //ÖÐÊé
            BYTE                 m_WarShangShu;          //ÉÐÊé
        BYTE                 m_WarMiShu;             //ÃØÊé
            BYTE                 m_WarTaiLe;             //Ì«ÀÖ
            */
    BYTE m_IsDoing;   // ÊÇ·ñÕýÔÚÖ´ÐÐ½Å±¾
    int m_ReviceNum;  // ÖØÉú´ÎÊý
    BYTE m_IsSynDir;  // ÊÇ·ñ¿ÉÒÔÍ¬²½·½Ïò
    //	int                 m_WarIsGong;            //ÊôÓÚ¹¥·½
    //    int               m_WarIsShou;            //ÊôÓÚÊØ·½
    // char                m_GuishuName[32];       //NPC¹éÊôË­µÄÍ¬°é
    unsigned long m_GuiShuDwid;  // Ö÷ÈËµÄDwid
    // char              m_BiaoCheName[32];      //NPC¹éÊôË­µÄïÚ³µ
    BYTE m_RestNameCount;  // ¸ÄÃûµÄ´ÎÊý
    // char                m_ChenHaoName[32];      //³ÆºÅµÄÃû³Æ

    //   BYTE                m_IsWaiGua;              //ÊÇ·ñÊ¹ÓÃÍâ¹Ò
    /*
           int                 m_GuziYajing;            //Ñº½ð
           int                 m_GuziDianshu;           //µãÊý
           BYTE                m_GuziZhuang;            //ÊÇ·ñ×¯¼Ò
           BYTE                m_IsShuai;               //ÊÇ·ñÒÑ¾­Ë¦ÁË
           BYTE                m_GuziMenber;            //1ÎªÖ÷²ÎÓëÕß 2ÎªÅÔ¹ÛÏÂ×¢Õß
           BYTE                m_ZuoWeihao;             //×ùÎ»ºÅ
   */
    //	BYTE                IsDeath;
    int IsJinYan;     // »î¶¯¾­Ñé
    int IsLuKey;      // »î¶¯ÐÒÔË
    int m_GoldLucky;  // È«¾ÖÐÒÔË
    int IsJinQian;    // »î¶¯½ðÇ®
    //	char                m_ItmeInfo[128];          //ÎïÆ·ËµÃ÷
    //	BYTE                m_IsTuiGuang;             //ÊÇ·ñÍÆ¹ãÔ±
    int m_IsVip;  // »áÔ±
    int m_IsXingYunXing;

    int m_mMapX;
    int m_mMapY;
    int m_IsgetSkill;      // ÊÇ·ñÊ°È¡¼¼ÄÜ×´Ì¬
    BYTE IsExeGoldScript;  // ÊÇ·ñÖ´ÐÐÈ«¾ÖËÀÍö½Å±¾
    BYTE IsCreatBoss;
    BYTE IsCreatTongBan;  // ÊÇ·ñÒÑ¾­¿ÉÒÔÕÙ»½Í¬°é
    int m_ZhuaVal;        // µ±Ç°µÄ±»×¥²¶Öµ
    unsigned long m_TongBanNum;   // Í¬°éÊýÁ¿
    BYTE m_IsSerLock;     // ÊÇ·ñËø×¡Ä³ÈË
    BOOL m_nIsOver;

    BOOL m_bIsHideNpc;
    BOOL m_bIsHidePlayer;
    BOOL m_bIsHideMissle;
    BOOL m_bIsHideLife;
    BOOL m_bIsHideTong;
    BOOL m_bIsHideNuqi;

    // char                m_PicPath[256];  //Í·ÏñÂ·¾¶
    // char                m_ScriptPicPath[256];//½Å±¾Â·¾¶

    BYTE m_BtnFlag;                             // ÊÇ·ñ²åÆì×´Ì¬
    BOOL m_BtnFindPath;                         // ÊÇ·ñ×Ô¶¯Ñ°Â·×´Ì¬
    int m_btStateInfo[MAX_NPC_RECORDER_STATE];  // Npcµ±Ç°×îÐÂµÄ¼¸¸ö×´Ì¬  BYTE
    //	unsigned long				m_NpcEnchant;			                // NpcµÄ¼ÓÇ¿ÀàÐÍ£¨»Æ½ð¹ÖÎï£©
    int m_nNextStatePos;  // ÏÂÒ»´Î×´Ì¬ÔÚm_btStateµÄÎ»ÖÃ
    KNpcGold m_cGold;

    // ÓÃÓÚNpcÄ£°å¿âÖÐ£¬µ±FALSE±íÊ¾¸ÃNpcÊý¾Ýµ±Ç°ÊÇÎÞÐ§µÄ £¬ÊýÖµÎ´¾­¹ý½Å±¾¼ÆËã£¬ÐèÒªÉú³É.
    // TRUE±íÊ¾ÓÐÐ§Êý¾Ý
    // BOOL				m_bHaveLoadedFromTemplate;//

    // KState			m_PowerState;			// Å­»ð×´Ì¬

    // typedef std::map<INT, KState> _KStateNote;                 //SkillId,Echance
    //_KStateNote nKStateNote;
    KState m_PoisonState;    // ÖÐ¶¾×´Ì¬
    KState m_FreezeState;    // ±ù¶³×´Ì¬
    KState m_BurnState;      // È¼ÉÕ×´Ì¬
    KState m_ConfuseState;   // »ìÂÒ×´Ì¬
    KState m_StunState;      // Ñ£ÔÎ×´Ì¬
    KState m_LifeState;      // ²¹Ñª×´Ì¬
    KState m_ManaState;      // ²¹MANA×´Ì¬
    KState m_MenuState;      // ²Ëµ¥
    KState m_DrunkState;     // ×í¾Æ×´Ì¬
    KState m_Hide;           // Òþ²Ø£¿
    KState m_ZhuaState;      // ±»×¥²¶×´Ì¬
    KState m_LoseMana;       // µõÀ¼×´Ì¬
    KState m_ExpState;       // ¾­Ñé×´Ì¬
    KState m_DoScriptState;  // Ö´ÐÐ½Å±¾×´Ì¬
    KState m_randmove;       // ÈºÌå×ß¶¯×´Ì¬
    KState m_MapUseModel;    // µØÍ¼ÏûºÄÀàÐÍ
    KState m_PhysicsArmor;
    KState m_ColdArmor;
    KState m_LightArmor;
    KState m_PoisonArmor;
    KState m_FireArmor;
    KState m_ManaShield;
    KState m_Returnskill;  // ±»µÐÈË¹¥»÷ÊÇ ×Ô¶¯ÊÍ·ÅÄ³¼¼ÄÜ¼¸ÂÊ
    KState m_Deathkill;    // ±»µÐÈË¹¥»÷ÊÇ ×Ô¶¯ÊÍ·ÅÄ³¼¼ÄÜ¼¸ÂÊ
    KState m_Rescueskill;  // ¾ÈÃü×Ô¶¯ÊÍ·Å¼¼ÄÜ
    KState m_Replyskill;   // ×Ô¶¯·´»÷¼¼ÄÜ
    // KState            m_Staticmagicshield;    //Ä§·¨¶Ü×´Ì¬

    int m_AdjustColorKind;  // NPCµÄµ±Ç°Æ«É«
    int m_RideState;        // NpcÆïÂíÉÏÏÂÂíÊ±¼äÏÞÖÆ

    KList m_StateSkillList;      // ¸½¼Ó×´Ì¬¼¼ÄÜÁÐ±í£¨Í¬¼¼ÄÜ²»µþ¼Ó£©
    BYTE m_Camp;                 // NpcµÄÕóÓª
    BYTE m_CurrentCamp;          // NpcµÄµ±Ç°ÕóÓª
    NPCCMD m_Doing;              // NpcµÄÐÐÎª
    CLIENTACTION m_ClientDoing;  // NpcµÄ¿Í»§¶ËÐÐÎª
    char szNpcTypeName[32];
    DOING_FRAME m_Frames;    // NpcµÄÐÐÎªÖ¡Êý
    KSkillList m_SkillList;  // NpcµÄ¼¼ÄÜÁÐ±í
    int m_SubWorldIndex;     // NpcËùÔÚµÄSubWorld ID mapsÀïÃæ µÚ¼¸¸öµØÍ¼
    int m_RegionIndex;       // NpcËùÔÚµÄRegion ID
    int m_ActiveSkillID;     // Npc¼¤»îµÄ¼¼ÄÜID
    int m_ActiveSkListIndex;

    int m_ActiveAuraID;  // Npc¼¤»îµÄ¹â»·¼¼ÄÜID
    int m_ActiveAuraIndex;
    int m_ActiveXinYunXingID;

    bool m_IsMoreAura;
    skillAuraInfo m_TmpAuraID[5];  // ¶ëÃ¼¼¼ÄÜ¶à¹âÁÙÊ±µÄ¹â»·¼¼ÄÜID
    int m_ExtSkill[5];             // ³ÆºÅÀ©Õ¹×´Ì¬¼¼ÄÜ

    // NpcµÄÊµ¼ÊÊý¾Ý£¨ÒÑ¾­¾­¹ý×°±¸¼°¼¼ÄÜµÄÔËËãÁË£©===================================
    int m_CurrentExperience;       // Npc±»É±ºóËÍ³öµÄ¾­Ñé
    int m_CurrentLife;             // NpcµÄµ±Ç°ÉúÃü
    int m_CurrentLifeDamage;       // Ö÷ÒªÓÃÓÚÃ°Ñª´¦Àí
    int m_CurPoisonDamage;         // Ö÷ÒªÓÃÓÚÃ°Ñª´¦Àí
    int m_CurFireDamage;           // Ö÷ÒªÓÃÓÚÃ°Ñª´¦Àí
    int m_CurrentLifeMax;          // NpcµÄµ±Ç°ÉúÃü×î´óÖµ
    int m_CurrentLifeReplenish;    // NpcµÄµ±Ç°ÉúÃü»Ø¸´ËÙ¶È
    int m_CurrentLifeReplenish_p;  // NpcµÄµ±Ç°ÉúÃü»Ø¸´ËÙ¶È °Ù·Ö±È
    int m_CurrentMana;             // NpcµÄµ±Ç°ÄÚÁ¦
    int m_CurrentManaMax;          // NpcµÄµ±Ç°×î´óÄÚÁ¦
    int m_CurrentManaReplenish;    // NpcµÄµ±Ç°ÄÚÁ¦»Ø¸´ËÙ¶È
    int m_CurrentManaReplenish_p;  //
    int m_CurrentStamina;          // NpcµÄµ±Ç°ÌåÁ¦
    int m_CurrentStaminaMax;       // NpcµÄµ±Ç°×î´óÌåÁ¦
    int m_CurrentStaminaGain;      // NpcµÄµ±Ç°ÌåÁ¦»Ø¸´ËÙ¶È
    int m_CurrentStaminaLoss;      // NpcµÄµ±Ç°ÌåÁ¦ÏÂ½µËÙ¶È

    int m_CurrentNuQi;           // NpcµÄµ±Ç°Å­ÆøÖµ
    int m_CurrentNuQiMax;        // NpcµÄµ±Ç°Å­Æø×î´óÖµ
    int m_CurrentNuQiReplenish;  // NpcµÄµ±Ç°Å­Æø»Ø¸´ËÙ¶È
    //////===================¼¼ÄÜ¹¥»÷Êý¾Ý£¨´Ó½Å±¾»ñµÃ£©=============================
    KMagicAttrib m_PhysicsDamage;  // NpcµÄ»ù±¾ÉËº¦µã(ÓÉÁ¦Á¿Ãô½ÝÓëÎäÆ÷ÉËº¦¾ö¶¨£¬²»¿¼ÂÇÖ±½Ó¼ÓÉËº¦µÄÄ§·¨ÊôÐÔ)(¹¥»÷µã)
    KMagicAttrib m_CurrentPhysicsMagicDamageP;  // NpcµÄµ±Ç°ÆÕµã°Ù·Ö±È
    KMagicAttrib m_CurrentPhysicsMagicDamageV;  // NpcµÄµ±Ç°ÆÕµãÉËº¦
    KMagicAttrib m_CurrentMagicFireDamage;      // NpcµÄµ±Ç°»ðÉËº¦
    KMagicAttrib m_CurrentMagicColdDamage;      // NpcµÄµ±Ç°±ùÉËº¦
    KMagicAttrib m_CurrentMagicLightDamage;     // NpcµÄµ±Ç°µçÉËº¦
    KMagicAttrib m_CurrentMagicPoisonDamage;    // NpcµÄµ±Ç°¶¾ÉËº¦
    int m_CurrentAttackRating;    // NpcµÄµ±Ç°ÃüÖÐÂÊ£¨Éí·¨ÃüÖÐÂÊ+¼¼ÄÜÃüÖÐÂÊ£©
    int m_CurrentDefend;          // NpcµÄµ±Ç°ÉÁ±ÜÂÊµã
                                  ///=============================================================================
    int m_CurrentSkillMingZhong;  // NpcµÄµ±Ç°ÉúÃü×î´óÖµ
    int m_TempFireResist;         // NpcµÄµ±Ç°»ð¿¹ÐÔ
    int m_TempColdResist;         // NpcµÄµ±Ç°±ù¿¹ÐÔ
    int m_TempPoisonResist;       // NpcµÄµ±Ç°¶¾¿¹ÐÔ
    int m_TempLightResist;        // NpcµÄµ±Ç°µç¿¹ÐÔ
    int m_TempPhysicsResist;      // NpcµÄµ±Ç°ÎïÀí¿¹ÐÔ

    int m_CurrentFireResist;     // NpcµÄµ±Ç°»ð¿¹ÐÔ
    int m_CurrentColdResist;     // NpcµÄµ±Ç°±ù¿¹ÐÔ
    int m_CurrentPoisonResist;   // NpcµÄµ±Ç°¶¾¿¹ÐÔ
    int m_CurrentLightResist;    // NpcµÄµ±Ç°µç¿¹ÐÔ
    int m_CurrentPhysicsResist;  // NpcµÄµ±Ç°ÎïÀí¿¹ÐÔ

    int m_CurrentFireResistMax;     // NpcµÄµ±Ç°×î´ó»ð¿¹ÐÔ
    int m_CurrentColdResistMax;     // NpcµÄµ±Ç°×î´ó±ù¿¹ÐÔ
    int m_CurrentPoisonResistMax;   // NpcµÄµ±Ç°×î´ó¶¾¿¹ÐÔ
    int m_CurrentLightResistMax;    // NpcµÄµ±Ç°×î´óµç¿¹ÐÔ
    int m_CurrentPhysicsResistMax;  // NpcµÄµ±Ç°×î´óÎïÀí¿¹ÐÔ

    // BOOL                m_AttackState;
    int m_CurrentTempSpeed;
    int m_CurrentWalkSpeed;      // NpcµÄµ±Ç°×ß¶¯ËÙ¶È
    int m_CurrentRunSpeed;       // NpcµÄµ±Ç°ÅÜ¶¯ËÙ¶È
    int m_CurrentJumpSpeed;      // NpcµÄµ±Ç°ÌøÔ¾ËÙ¶È
    int m_CurrentJumpFrame;      // NpcµÄµ±Ç°ÌøÔ¾Ö¡Êý
    int m_CurrentAttackSpeed;    // NpcµÄµ±Ç°¹¥»÷ËÙ¶È  --Íâ¹¦
    int m_CurrentCastSpeed;      // NpcµÄµ±Ç°Ê©·¨ËÙ¶È  --ÄÚ¹¦
    int m_CurrentVisionRadius;   // NpcµÄµ±Ç°ÊÓÒ°·¶Î§
    int m_CurrentAttackRadius;   // NpcµÄµ±Ç°¹¥»÷·¶Î§
    int m_CurrentActiveRadius;   // NpcµÄµ±Ç°»î¶¯·¶Î§
    int m_CurrentHitRecover;     // NpcµÄÊÜÉË¶¯×÷Ê±¼ä
    int m_CurrentHitNpcRecover;  // Ôì³É¶Ô·½µÄÊÜÉË¶¯×÷Ôö¼Ó¶àÉÙ
    int m_CurrentTreasure;       // Npc¶ªÂä×°±¸µÄÊýÁ¿
    int m_CurrentHitRank;        // ÊÜÉËµÄ¸ÅÂÊ
    // ÒÔÏÂÊý¾ÝÖ»ÐèÒªµ±Ç°Öµ£¬»ù´¡Öµ¾ùÎª0 = ¼¼ÄÜ×´Ì¬Êý¾Ý+×°±¸Êý¾Ý£¨Ã»×´Ì¬-Ö±½Ó¸³Öµ£©==================================
    int m_CurrentMeleeDmgRetPercent;  // Npc½ü³ÌÉËº¦·µ»ØµÄ°Ù·Ö±È
    int m_CurrentMeleeDmgRet;         // Npc½ü³Ì±»»÷Ê±·µ»ØµÄÉËº¦µãÊý
    int m_CurrentRangeDmgRetPercent;  // NpcÔ¶³ÌÉËº¦·µ»ØµÄ°Ù·Ö±È
    int m_CurrentRangeDmgRet;         // NpcÔ¶³Ì±»»÷Ê±·µ»ØµÄÉËº¦µãÊý
    BOOL m_CurrentSlowMissle;         // NpcÊÇ·ñÔÚÂýËÙ×Óµ¯×´Ì¬ÏÂ
    int m_CurrentHulueMeleeDmgRet;    // ºöÂÔ½ü³Ì·´µ¯¶àÉÙµã¡¢
    int m_CurrentHulueRangeDmgRet;    // ºöÂÔÔ¶³Ì³Ì·´µ¯¶àÉÙµã¡¢

    int m_CurrentDamageReduce;  // ÎïÀíÉËº¦¼õÉÙ
                                //	int					m_CurrentElementDamageReduce;	// ÔªËØÉËº¦¼õÉÙ

    int m_CurrentDamage2Mana;    // ÉËº¦×ªÄÚÁ¦°Ù·Ö±È
                                 //	int					m_CurrentManaPerEnemy;			// É±Ò»¸öµÐÈË¼Ó¶àÉÙµãÄÚÁ¦ //
    int m_CurrentLifeStolen;     // ÍµÉúÃü°Ù·Ö±È ÎüÑª
    int m_CurrentManaStolen;     // ÍµÄÚÁ¦°Ù·Ö±È ÎüÄÚ
    int m_CurrentStaminaStolen;  // ÍµÌåÁ¦°Ù·Ö±È
    int m_CurrentKnockBack;      // ÕðÍË°Ù·Ö±È
    int m_CurrentDeadlyStrike;   // ÖÂÃüÒ»»÷°Ù·Ö±È
                                 //	int					m_CurrentBlindEnemy;			// ÖÂÃ¤µÐÈË°Ù·Ö±È
                                 //	int					m_CurrentPiercePercent;			// ´©Í¸¹¥»÷°Ù·Ö±È
    int m_CurrentFreezeTimeReducePercent;  // ±ù¶³Ê±¼ä¼õÉÙ°Ù·Ö±È
    int m_CurrentPoisonTimeReducePercent;  // ÖÐ¶¾Ê±¼ä¼õÉÙ°Ù·Ö±È
    int m_CurrentStunTimeReducePercent;    // Ñ£ÔÎÊ±¼ä¼õÉÙ°Ù·Ö±È
    int m_CurrentBurnTimeReducePercent;    // È¼ÉÕÊ±¼ä¼õÉÙ°Ù·Ö±È
    int m_CurrentautoReviverate;           // µ±Ç°µÄ¸´»î¸ÅÂÊ
    int m_CurrentStunRank_p;               // ¼õÉÙÑ£ÔÎ¸ÅÂÊÔö¼Ó°Ù·Ö±È

    int m_EnemyPoisonTimeReducePercent;  // Ôì¶Ô·½ÖÐ¶¾Ê±¼äÔö¼Ó°Ù·Ö±È
    int m_EnemyStunTimeReducePercent;    // Ôì¶Ô·½Ñ£ÔÎÊ±¼äÔö¼Ó°Ù·Ö±È

    int m_CurrentBuZhuoRate;          // µ±Ç°µÄ²¶»ñ¸ÅÂÊ
    int m_CurrentUpExp;               // ¾­Ñé±¶ÂÊ
    int m_CurrentdanggeRate;          // µµ¸ñ¸ÅÂÊ
    int m_CurrentzhongjiRate;         // ÖØ»÷¸ÅÂÊ
    int m_CurrentcjdanggeRate;        // ²ð½âµµ¸ñ
    int m_CurrentcjzhongjiRate;       // ²ð½âÖØ»÷
    int m_Currentsorbdamage;          // µÖÏûÉËº¦ µãÊý
    int m_Currentsorbdamage_v;        // µÖÏûÉËº¦°Ù·Ö±È
    int m_Currenadddamagev;           // ¼ÓÉîÉËº¦µã
    int m_Currenadddamagep;           // ¼ÓÉîÉËº¦°Ù·Ö±È
    int m_Currentpoisonres;           //=ºöÂÔ¶Ô·½¶¾·À:#d1-%
    int m_Currentfireres;             //=ºöÂÔ¶Ô·½»ð·À:#d1-%
    int m_Currentlightingres;         //=ºöÂÔ¶Ô·½À×·À:#d1-%
    int m_Currentphysicsres;          //=ºöÂÔ¶Ô·½ÆÕ·À:#d1-%
    int m_Currentcoldres;             //=ºöÂÔ¶Ô·½±ù·À:#d1-%
    int m_Currentallres;              //=ºöÂÔ¶Ô·½È«¿¹:#d1-%
    int m_CurrentIgnoredefensep;      // µ±Ç°ºöÂÔ¶Ô·½ÉÁ±ÜÂÊ   //¼¼ÄÜµÄÊý¾Ý
    int m_CurrentIgnorenAttacRating;  // µ±Ç°ºöÂÔ¶Ô·½ÃüÖÐÂÊ   //×´Ì¬Êý¾Ý
    int m_Currentnopkvalue;           //=²»Ôö¼ÓPKÖµ¸ÅÂÊ:#d1+%
    int m_Currentbossdamage;          //=¶Ô»Æ½ðboss¹¥»÷ÉËº¦<color=orange>#d1+%<color>
    int m_Currentelementsenhance;  //=ÎåÐÐÇ¿»¯Öµ£º#d1-µã¡£Ç¿»¯¶ÔÏà¿ËÎåÐÐµÄ¿ËÖÆÐ§¹û ÎÒ¿Ë
    int m_Currentelementsresist;  //=ÎåÐÐÈõ»¯Öµ£º#d1-µã¡£Èõ»¯ÊÜÏà¿ËÎåÐÐµÄ¿ËÖÆÐ§¹û ¿ËÎÒ

    int m_Currentskillenhance;          // ¹¥»÷¼¼ÄÜ×´Ì¬¼ÓÇ¿
    int m_CurrentFullManaskillenhance;  // ÂúÄÚÁ¦Ê±¹¥»÷¼¼ÄÜ¼Ó³É;
    // CEnhanceInfo        m_CurrentSkillEnhance[MAX_NPCSKILL]; // ¶ÔÄ³¼¼ÄÜ×´Ì¬¼ÓÇ¿
    typedef std::map<int, CEnhanceInfo> _EnhanceInfo;  // SkillId,Echance
    _EnhanceInfo nEnhanceInfo;
    int m_CurrentFireEnhance;    // »ð¼ÓÇ¿
    int m_CurrentColdEnhance;    // ³Ù»ºÊ±¼äÔö¼Ó
    int m_CurrentPoisonEnhance;  // ¶¾·¢Ê±¼ä¼ä¸ô
    int m_CurrentLightEnhance;   // Ñ£ÔÎÊ±¼ä³ÖÐø
    int m_CurrentPoisonTime;     // ¼¼ÄÜµÄ¶¾·¢Ê±¼ä

    int m_CurrentAttackRatingEnhancep;  // ×°±¸ÊôÐÔÃüÖÐÂÊ¼ÓÇ¿°Ù·Ö±È
    int m_CurrentAttackRatingEnhancev;  // ×°±¸ÊôÐÔÃüÖÐÂÊ¼ÓÇ¿µã

    int m_CurrentAddPhysicsDamage;   // µ±Ç°±»¶¯Íâ¹¦ÆÕµã Ö±½ÓµÄÎïÀíÉËº¦¼ÓÇ¿µãÊý
    int m_CurrentAddPhysicsDamageP;  // µ±Ç°±»¶¯ÍâÆÕ°Ù·Ö±È
    int m_CurrentAddFireDamagev;     // µ±Ç°±»¶¯Íâ»ðµã
    int m_CurrentAddColdDamagev;     // µ±Ç°±»¶¯Íâ±ùµã
    int m_CurrentAddLighDamagev;     // µ±Ç°±»¶¯ÍâÀ×µã
    int m_CurrentAddPoisonDamagev;   // µ±Ç°±»¶¯Íâ¶¾µã
    BOOL m_IsDel;                    // ÊÇ·ñÉ¾³ýNPC
    int m_CurrentAddmagicphysicsDamage;   // µ±Ç°±»¶¯ÄÚÆÕµã
    int m_CurrentAddmagicphysicsDamageP;  // µ±Ç°±»¶¯ÄÚÆÕ°Ù·Ö±È
    int m_CurrentAddmagicColdDamagicv;    // µ±Ç°±»¶¯ÄÚ±ùµã
    int m_CurrentAddmagicFireDamagicv;    // µ±Ç°±»¶¯ÄÚ»ðµã
    int m_CurrentAddmagicLightDamagicv;   // µ±Ç°±»¶¯ÄÚÀ×µã
    int m_CurrentAddmagicPoisonDamagicv;  // µ±Ç°±»¶¯ÄÚ¶¾µã

    int m_CurrentMeleeEnhance[MAX_MELEE_WEAPON];  // ½ü³ÌÎïÀí¼ÓÇ¿
    int m_CurrentRangeEnhance;                    // Ô¶³ÌÎïÀí¼ÓÇ¿
    int m_CurrentHandEnhance;                     // ¿ÕÊÖÎïÀí¼ÓÇ¿
    int m_CurrentSerisesEnhance;                  // ÎåÐÐµÄÉËº¦¼ÓÇ¿

    int m_CurrentPoisondamagereturnV;  // µ±Ç°¶¾·´µ¯µã
    int m_CurrentPoisondamagereturnP;  // µ±Ç°¶¾·´µ¯°Ù·Ö±È
    int m_CurrentReturnskillp;         // µ±Ç°×çÖä·´µ¯¸ÅÂÊ
    int m_CurrentIgnoreskillp;         // ÎÞÊÓ×çÖä·´µ¯¸ÅÂÊ
    int m_CurrentReturnresp;           // ¶Ô·´µ¯ÉËº¦µÄ¿¹ÐÔ
    int m_CurrentCreatnpcv;            // µ±Ç°×¥²¶BBµÄ¸ÅÂÊ
    int m_CurrentAllJiHuo;             // ÊÇ·ñÈ«Éí¼¤»î
    int m_CurrentCreatStatus;          // µ±Ç°²úÉúµÄ×´Ì¬
    int m_Currentbaopoisondmax_p;
    int m_nCurNpcLucky;

    int m_Me2metaldamage_p;  //=¶Ô½ðÏµÉËº¦Ôö¼Ó£º#d1+%
    int m_Metal2medamage_p;  //=¼õÉÙÀ´×Ô½ðÏµµÄÉËº¦£º#d1-%
    int m_Me2wooddamage_p;   //=¶ÔÄ¾ÏµÉËº¦Ôö¼Ó£º#d1+%
    int m_Wood2medamage_p;   //=¼õÉÙÀ´×ÔÄ¾ÏµµÄÉËº¦£º#d1-%
    int m_Me2waterdamage_p;  //=¶ÔË®ÏµÉËº¦Ôö¼Ó£º#d1+%
    int m_Water2medamage_p;  //=¼õÉÙÀ´×ÔË®ÏµµÄÉËº¦£º#d1-%
    int m_Me2firedamage_p;   //=¶Ô»ðÏµÉËº¦Ôö¼Ó£º#d1+%
    int m_Fire2medamage_p;   //=¼õÉÙÀ´×Ô»ðÏµµÄÉËº¦£º#d1-%
    int m_Me2earthdamage_p;  //=¶ÔÍÁÏµÉËº¦Ôö¼Ó£º#d1+%
    int m_Earth2medamage_p;  //=¼õÉÙÀ´×ÔÍÁÏµµÄÉËº¦£º#d1-%

    int m_Staticmagicshield_p;  // µÍÓÚÄÚÁ¦×î´óÖµ¶àÉÙ±¶µÄÉËº¦

    // Ö»ÐèÒªµ±Ç°ÖµµÄÊý¾Ý½áÊø
    int m_Dir;     // NpcµÄ·½Ïò
    int m_RedLum;  // NpcµÄÁÁ¶È
    int m_GreenLum;
    int m_BlueLum;
    int m_MapX, m_MapY, m_MapZ;        // NpcµÄµØÍ¼×ø±ê(¸ñ×Ó×ø±ê) x £¨0-15£© y£¨0-31£©
    int m_OffX, m_OffY;                // NpcÔÚ¸ñ×ÓÖÐµÄÆ«ÒÆ×ø±ê£¨·Å´óÁË1024±¶£©
    int m_DesX, m_DesY;                // NpcµÄÐèÒªÒÆ¶¯µ½µÄÄ¿±ê×ø±ê
    int m_SkillParam1, m_SkillParam2;  //
    int m_OriginX, m_OriginY;          // NpcµÄÔ­Ê¼×ø±ê £¨ÏñËØ×ø±ê£©
    unsigned long m_NextAITime;
    BYTE m_AIMAXTime;  // NpcAI
    // NpcµÄ×°±¸£¨¾ö¶¨¿Í»§¶ËµÄ»»×°±¸£©
    int m_HelmType;     // NpcµÄÍ·¿øÀàÐÍ
    int m_ArmorType;    // NpcµÄ¿ø¼×ÀàÐÍ
    int m_WeaponType;   // NpcµÄÎäÆ÷ÀàÐÍ
    int m_HorseType;    // NpcµÄÆïÂíÀàÐÍ
    BOOL m_bRideHorse;  // NpcÊÇ·ñÆïÂí
    int m_MaskType;     // Npc Ãæ¾ß¹¦ÄÜ
    // int				m_FuMoType;					// Npc ¸½Ä§¹¦ÄÜ
    int m_PifengType;    // Npc Åû·ç¹¦ÄÜ
    BYTE m_ChiBangType;  // Npc Åû·ç¹¦ÄÜ
    int m_MaskMark;      // Ãæ¾ßÊÐ³¡
    // BYTE				m_IsFuMo;					//
    BYTE m_BaiTan;  // NpcÊÇ·ñÔÚ°ÚÌ¯
    // char				ShopName[32];
    // ¿Í»§¶Ë
    char TongName[32];
    int m_nFigure;
    char Name[32];  // NpcµÄÃû³Æ-Íæ¼Ò
    char _clientName[32];
    BYTE m_nSex;             // NpcµÄÐÔ±ð0ÎªÄÐ£¬1ÎªÅ®
    int m_NpcSettingIdx;     // NpcµÄÉè¶¨ÎÄ¼þË÷Òý  £¨ÐÐºÅ£¿£©
    int m_CorpseSettingIdx;  // NpcµÄÊ¬Ìå¶¨ÒåË÷Òý
    // char				ActionScript[256];		// NpcµÄÐÐÎª½Å±¾
    typedef std::map<int, std::string> _StrNoteInfo;
    _StrNoteInfo nstrNoteInfo;

    void ClearstrInfo(int i)
    {
        if (nstrNoteInfo.count(i) > 0)
        {
            nstrNoteInfo.erase(i);
        }
    }

    char* GetstrInfo(int i, char* str)
    {
        // char nMsg[256]={0};
        /*_StrNoteInfo::iterator it;
        for( it = nstrNoteInfo.begin(); it != nstrNoteInfo.end(); ++it)
        {
                 if (it->first == i)
                 {
#ifdef _SERVER
                         printf("--»ñÈ¡(%s)½Å±¾:%s --\n",Name,it->second.c_str());
#endif
                         sprintf(str,it->second.c_str());
                     return str;
                 }
        } */
        if (nstrNoteInfo.count(i) > 0)
        {  // Èç¹û´æÔÚ
            sprintf(str, "%s", nstrNoteInfo[i].c_str());

            /*#ifdef _SERVER
                                    printf("--»ñÈ¡(%s)(%d)½Å±¾:%s --\n",Name,i,nstrNoteInfo[i].c_str());
            #endif */

            return str;
        }
        return NULL;
    }
    void SetstrInfo(int i, const char* str)
    {
        if (str)
            nstrNoteInfo[i] = str;
        else
            nstrNoteInfo[i] = "";
        /*#ifdef _SERVER
                                     printf("--ÉèÖÃ(%s)(%d)½Å±¾:%s --\n",Name,i,nstrNoteInfo[i].c_str());
        #endif */
    }

    unsigned long m_ActionScriptID;  // NpcµÄÐÐÎª½Å±¾ID£¨Ê¹ÓÃÊ±ÓÃÕâ¸öÀ´¼ìË÷£©

    unsigned long m_TrapScriptID;  // NpcµÄµ±Ç°Trap½Å±¾ID;

    int m_nPeopleIdx;            // Ëø×¡µÄ¶ÔÏóÈËÎïË÷Òý
    int m_nLastDamageIdx;        // ×îºóÒ»´ÎÉËº¦µÄÈËÎïË÷Òý
    int m_nLastPoisonDamageIdx;  // ×îºóÒ»´Î¶¾ÉËº¦µÄÈËÎïË÷Òý
    int m_nLastBurnDamageIdx;    // ×îºóÒ»´Î»ðÉËº¦µÄÈËÎïË÷Òý
    int m_nObjectIdx;            // ¶ÔÏóÎïÆ·

    // NpcµÄ»ù±¾Êý¾Ý£¨Î´¿¼ÂÇ×°±¸¡¢¼¼ÄÜµÄÓ°Ïì£©
    int m_Experience;     // Npc±»É±»ñµÃµÄ¾­Ñé
    int m_LifeMax;        // NpcµÄ×î´óÉúÃü
    int m_LifeReplenish;  // NpcµÄÉúÃü»Ø¸´ËÙ¶È

    int m_NuqiMax;        // NpcÅ­Æø×î´óÖµ
    int m_NuqiReplenish;  // NpcÅ­Æø»Ø¸´ËÙ¶È

    int m_ManaMax;        // NpcµÄ×î´óÄÚÁ¦
    int m_ManaReplenish;  // NpcµÄÄÚÁ¦»Ø¸´ËÙ¶È

    int m_StaminaMax;   // NpcµÄ×î´óÌåÁ¦
    int m_StaminaGain;  // NpcµÄÌåÁ¦»Ø¸´ËÙ¶È
    int m_StaminaLoss;  // NpcµÄÌåÁ¦ÏÂ½µËÙ¶È

    int m_AttackRating;   // NpcµÄÃüÖÐµã£¨Éí·¨¾ö¶¨Ô­Ê¼ÃüÖÐÂÊ£©
    int m_Defend;         // NpcµÄÉÁ±ÜÂÊ
    int m_FireResist;     // NpcµÄ»ð¿¹ÐÔ
    int m_ColdResist;     // NpcµÄÀä¿¹ÐÔ
    int m_PoisonResist;   // NpcµÄ¶¾¿¹ÐÔ
    int m_LightResist;    // NpcµÄµç¿¹ÐÔ
    int m_PhysicsResist;  // NpcµÄÎïÀí¿¹ÐÔ

    int m_FireResistMax;     // NpcµÄ×î´ó»ð¿¹ÐÔ
    int m_ColdResistMax;     // NpcµÄ×î´ó±ù¿¹ÐÔ
    int m_PoisonResistMax;   // NpcµÄ×î´ó¶¾¿¹ÐÔ
    int m_LightResistMax;    // NpcµÄ×î´óµç¿¹ÐÔ
    int m_PhysicsResistMax;  // NpcµÄ×î´óÎïÀí¿¹ÐÔ

    int m_WalkSpeed;     // NpcµÄÐÐ×ßËÙ¶È
    int m_RunSpeed;      // NpcµÄÅÜ¶¯ËÙ¶È
    int m_JumpSpeed;     // NpcµÄÌøÔ¾ËÙ¶È
    int m_AttackSpeed;   // NpcµÄÍâ¹¦¹¥»÷ËÙ¶È
    int m_CastSpeed;     // NpcµÄÄÚ¹¦Ê©·¨ËÙ¶È
    int m_VisionRadius;  // NpcµÄÊÓÒ°·¶Î§
    int m_DialogRadius;  // NpcµÄ¶Ô»°·¶Î§
    int m_ActiveRadius;  // NpcµÄ»î¶¯·¶Î§
    int m_HitRecover;    // NpcµÄÊÜ»÷»Ø¸´ËÙ¶È
    int m_Treasure;      // Npc¶ªÂä×°±¸µÄÊýÁ¿(NPC.txtÊý¾Ý)
    BOOL m_bClientOnly;  // ÊÇ·ñ¼ÓÔØ¿Í»§¶ËNPCÊý¾Ý
                         //	int					m_AttackFrame;          // Íâ¹¦¹¥»÷Ö¡Êý
                         //	int					m_CastFrame;            // ÄÚ¹¦¹¥»÷Ö¡Êý
                         //	int					m_nCurrentMeleeSkill;	// Npcµ±Ç°ÕýÖ´ÐÐµÄ¸ñ¶·¼¼ÄÜ
    int m_nCurrentMeleeTime;

    // AI²ÎÊý
    int m_AiMode;                 // AIÄ£Ê½  ¹Ò»úÄ£Ê½
    int m_AiParam[MAX_AI_PARAM];  // ÓÃÓÚAIÄ£¿é¼ÆËãAI
    int m_AiAddLifeTime;

    int m_HeadImage;

    BYTE m_FightMode;  // ¿Í»§¶Ë´¦Àí¶¯×÷ÓÃ¡£ Õ½¶·Ä£Ê½
    BYTE m_BayBan;     // bayban
    BYTE m_OldFightMode;
    BOOL m_bExchangeServer;
    BYTE m_ExItemId;              // ×ÓÄ¸´ü
    BYTE m_ExBoxId;               // À©Õ¹Ïä
    unsigned long m_SyncSignal;           // Í¬²½ÐÅºÅ
    KClientNpcID m_sClientNpcID;  // ÓÃÓÚ±êÃ÷¿Í»§¶ËnpcÊÇÄÄ¸öregionµÄµÚ¼¸¸önpc
    unsigned long m_dwRegionID;           // ±¾npcËùÔÚregionµÄid
    KSyncPos m_sSyncPos;
    BYTE m_nPKFlag;  // PKÄ£Ê½
    char m_szChatBuffer[MAX_SENTENCE_LENGTH];
    int m_nChatContentLen;
    int m_nChatNumLine;
    int m_nChatFontWidth;
    unsigned int m_nCurChatTime;
    BYTE m_nSleepFlag;
    int m_nHurtHeight;
    int m_nHurtDesX;
    int m_nHurtDesY;
    int m_PiFenLoop;
    BYTE m_nTongFlag;  // ÊÇ·ñÓÐÕÐÈËÍ¼±ê
    int m_IsbeSel;     // ÊÇ·ñÑ¡ÖÐÁË

    int m_DeathFrame;  // ËÀÍöÖ¡Êý
    int m_StandFrame;
    int m_HurtFrame;
    int m_AttackFrame;
    int m_CastFrame;
    int m_WalkFrame;
    int m_RunFrame;
    int m_StandFrame1;
    int m_ReviveFrame;  // ÖØÉúÖ¡Êý Ê±¼ä
    int m_SitFrame;
    int m_JumpFrame;

private:
    bool isRemoveMenu;
    unsigned long m_LoopFrames;  // Ñ­»·Ö¡Êý
    int m_nPlayerIdx;    // Íæ¼ÒË÷ÒýºÅ

    int m_JumpFirstSpeed;
    NPC_COMMAND m_Command;  // ÃüÁî½á¹¹
    BOOL m_ProcessAI;       // ´¦ÀíAI±êÖ¾
    BOOL m_ProcessState;    // ´¦Àí×´Ì¬±êÖ¾
    int m_XFactor;
    int m_YFactor;
    int m_JumpStep;
    int m_JumpDir;
    int m_SpecialSkillStep;  // ÌØÊâ¼¼ÄÜ²½Öè
    // NPC_COMMAND			m_SpecialSkillCommand;	// ÌØÊâ¼¼ÄÜÐÐÎªÃüÁî
    KNpcFindPath m_PathFinder;
    BOOL m_bActivateFlag;  // ÊÇ·ñÖ´ÐÐÑ­»·
    int m_ResDir;
    KNpcRes m_DataRes;  // NpcµÄ¿Í»§¶Ë×ÊÔ´£¨Í¼Ïó¡¢ÉùÒô£©
    int m_DurFrame[20];
    unsigned int uFlipTime[20];

    // ÓÃÓÚÆÕÍ¨Õ½¶·npcÃ°Ñª´¦Àí
    int m_nBloodNo[defMAX_SHOW_BLOOD_NUM][2];
    // int				m_nBloodAlpha;
    char m_szBloodNo[64];

private:
    BOOL WaitForFrame();  // µÈ´ýÖØÉúÖ¡Êý
    BOOL IsReachFrame(int nPercent);
    void DoStand();
    void OnStand();
    void DoRevive();  // ÖØÉú
    void OnRevive();  // ÖØÉú
    void DoWait();
    void OnWait();
    void DoWalk();
    void OnWalk();
    void DoRun();
    void OnRun();
    void DoSkill(int nX, int nY);
    int DoOrdinSkill(KSkill* pSkill, int nX, int nY);
    void OnSkill();
    void DoAttack();
    void DoMagic();
    void DoJump();
    BOOL OnJump();
    void DoSit();
    void OnSit();
    void DoHurt(int nHurtFrames = 0, int nX = 0, int nY = 0, int nRank = 0);
    void OnHurt();
    void DoDeath(int nMode = 0, int nLastDamageIdx = 0);
    void OnDeath();
    void DoDefense();  // µµ¸ñ
    void OnDefense();  // µµ¸ñ
    void DoIdle();
    void OnIdle();
    //	ÓÐ¹Ø¸ñ¶·¼¼ÄÜµÄ------------------------------------------

    BOOL DoSecMove(int nDistance);  // ÊÇ¶Ï»ê´Ì »¹ÊÇ Ë²ÒÆ£¿
    BOOL DoManyAttack();
    void OnManyAttack();
    BOOL DoBlurAttack();
    BOOL DoJumpAttack();
    BOOL OnJumpAttack();
    BOOL DoRunAttack();
    void OnRunAttack();
    BOOL CastMeleeSkill(KSkill* pSkill);
    //-----------------------------------------------------------
    void OnPlayerTalk();
    void DoSpecial1();
    void OnSpecial1();
    void DoSpecial2();
    void OnSpecial2();
    void DoSpecial3();
    void OnSpecial3();
    void DoSpecial4(int nX, int nY);
    void OnSpecial4();
    void Goto(int nMpsX, int nMpsY);
    void RunTo(int nMpsX, int nMpsY);
    void JumpTo(int nMpsX, int nMpsY);
    void ServeMove(int nSpeed);
    void ServeJump(int nSpeed);
    //	void				Enchant(void);
    //	void				ModifyEnchant(void* pData, int nAttrib);
    BOOL NewPath(int nMpsX, int nMpsY);
    BOOL NewJump(int nMpsX, int nMpsY);
    void MovePos(int nMoveX, int nMoveY);
    BOOL CheckHitTarget(int nAR, int nDf, int nIngore = 0, int nInHuLieAr = 0);
    void DoDrag(int nDragFrame, int nDesX, int nDesY);

    void HurtAutoMove();

public:
    friend class KNpcAttribModify;
    friend class KThiefSkill;
    KNpc();
    ~KNpc();

    // void                ClearEnhanceNote();
    unsigned long GetLoopFrames() { return m_LoopFrames; };
    void SetActiveFlag(BOOL bFlag) { m_bActivateFlag = bFlag; };
    void DoPlayerTalk(char*);
    void CheckTrap();
    //	int                 CheckWaiGua(char *exe_name);
    void ReSkillEnhance(int nLeftSkill, int nIsAdd = -1);  // ÖØÖÃ¶Ô¹¥»÷¼¼ÄÜµÄ¼Ó³É
    void ReFullManaSkillEnhance(int nLeftSkill, int nLeftListidx);
    void ReWhereSkillEnhance(int nActiveSkillID, int nCurSkillEnchance, int nIsAdd = -1);
    // int               NpcInitJinMaiVal();                            //¾­Âö³õÊ¼»¯
    int NpcNewInitJinMaiVal();
    void UseSkills(int sKillID,
                   int sLevel,
                   int nNpcIdx,
                   KMagicAttrib* pData = NULL,
                   BOOL nIfMagic       = TRUE,
                   int nTime           = -1);  // ÉèÖÃ¼¼ÄÜ×´Ì¬Ð§¹û
    void Init();
    void Remove();
    // unsigned long             TakeTrader(unsigned long a,unsigned long b); //È¡ÉÌ
    // unsigned long             TakeRemainder(unsigned long a,unsigned long b);	//È¡Óà
    void Activate();
    // void				ActivateDrop();	//±¬ÂÊµÄÑ­»·
    void NewSetNpcEnChance();
    BOOL IsPlayer();
    void GetTongBanDamage(int* nMin, int* nMax);

    /*int               GetCurFuMoNpcNo(int idx);
    int                 GetCurFuMoIdx();
    int                 GetCurFoMoSkllLevel(int idx);
    */
    void SetFightMode(BOOL bFightMode);
    void SetBayBan(BOOL bBayBan);  // bayban
    void TurnTo(int nIdx);
    void setNpcDir(int nX2, int nY2);
    int GetAttackFrame() { return m_AttackFrame; };
    int GetCastFrame() { return m_CastFrame; };
    void SetAttackFrame(int nCurFrame) { m_AttackFrame = nCurFrame; };
    void SetCastFrame(int nCurFrame) { m_CastFrame = nCurFrame; };
    void SendSerCommand(NPCCMD cmd, int x = 0, int y = 0, int z = 0);
    void ProcCommand(int nAI);
    ISkill* GetActiveSkill();
    int GetSkillLeftTime(int nSkillId);             // »ñÈ¡Õâ¸ö¼¼ÄÜµÄ×´Ì¬µÄÊ£ÓàÊ±¼ä
    int SetSkillLeftTime(int nSkillId, int nTime);  // ÉèÖÃÕâ¸ö¼¼ÄÜµÄ´æ»îÊ±¼ä
    void ClearOneSkillState(int nSkillId);

    BOOL ProcessState();
    void ProcStatus();
    void ModifyAttrib(int nAttacker, void* pData);
    int GetSkillLevel(int nSkillId);
    void SetId(unsigned long dwID) { m_dwID = dwID; };
    BOOL IsMatch(unsigned long dwID) { return dwID == m_dwID; };  // ÊÇ·ñIDÓë¸ÃIndexÆ¥Åä
    BOOL Cost(NPCATTRIB nType,
              int nCost,
              BOOL bOnlyCheckCanCast =
                  FALSE);  // ÏûºÄÄÚÁ¦ÌåÁ¦µÈ,Èç¹ûOnlyCheckCanCostÎªTRUE,±íÊ¾Ö»ÊÇ¼ì²éµ±Ç°µÄÄÚÁ¦µÈÊÇ·ñ¹»ÏûºÄ£¬²¢²»Êµ¼ÊµÄ¿Û
    // void				SelfDamage(int nDamage);						//
    // ×ÔÉíµÄÉËº¦£¬ÈçÎþÉü¹¥»÷
    void Load(int nNpcSettingIdx, int nLevel, int nSubWorld, int nBoss = 0);  // ´ÓTabFileÖÐ¼ÓÔØ
    void GetMpsPos(int* pPosX, int* pPosY, int* nMapid);
    BOOL SetActiveSkill(int nSkillIdx);
    BOOL ChangeSkillAttackRadius(int nSkillIdx, int nDis);
    int GetSkillID(int nSkillIdx);
    void UpdataNpcCurData(int nWonerIndex = 0, int nPlace = -1);
    void ReCalcNpcEquip(int nWonerIndex, int nEquipPlace = -1);
    BOOL UpMagicAttrib(int i, int nWonerIndex);
    void SetAuraSkill(int nSkillID);
    void SetCamp(int nCamp);
    void SetRankFF(int nRankFF);
    void SetPlayFF(int nPlayFF);
    void SetCurrentCamp(int nCamp);  // ÉèÖÃµ±Ç°ÕóÓª
    void RestoreCurrentCamp();       // »Ö¸´ÕóÓª
    void SetStateSkillEffect(int nLauncher,
                             int nSkillID,
                             int nLevel,
                             void* pData,
                             int nDataNum,
                             int nTime  = -1,
                             int nIsEuq = 0);  // Ö÷¶¯¸¨Öú¼¼ÄÜÓë±»¶¯¼¼ÄÜ
    void SetToolStateSkillEffect(int nLauncher, int nSkillID, int nLevel, void* pData, int nDataNum, int nTime = -1);
    void SetToolNoStateEffect(int nLauncher, int nSkillID, int nLevel, int nTime);
    void SysnCastSkillEffect(int nLauncher,
                             int nSkillID,
                             int nParam1,
                             int nParam2,
                             int nLevel,
                             int nTime   = -1,
                             int mMaxBei = 1);
    void ClearStateSkillEffect();  // Çå³ýNPCÉíÉÏµÄ·Ç±»¶¯ÀàµÄ¼¼ÄÜ×´Ì¬nMaxBeiLv
    void ReCalcStateEffect();
    void ClearNormalState();
    void SetImmediatelySkillEffect(int nLauncher, void* pData, int nDataNum);
    void AppendSkillEffect(int nIsMaigc,
                           BOOL bIsPhysical,
                           BOOL bIsMelee,
                           void* pSrcData,
                           void* pDesData,
                           int nEnChance);
    int ModifyMissleLifeTime(int nLifeTime);
    int ModifyMissleSpeed(int nSpeed);
    BOOL ModifyMissleCollsion(BOOL bCollsion);
    void RestoreNpcBaseInfo();  // Set Current_Data ;
    int GetNpcPepoleID() { return m_nPlayerIdx; };
    //	int                 GetNpcIsReviveID(){return m_IsRevive;};
    void SetNpcIsRe(int nIsRe, int nTime)
    {
        m_IsRevive    = nIsRe;
        m_ReviveFrame = nTime;
    };
    void SetNpcIsReFrame(int nIsRe, int nTime)
    {
        m_IsRe        = nIsRe;
        m_ReviveFrame = nTime;
    };
    void RestoreState();
    void ClearNpcState();
    BOOL SetPlayerIdx(int nIdx);
    void DialogNpc(int nIndex);
    void Revive();                      // cÖØÉúhon
    void AddBaseLifeMax(int nLife);     // Ôö¼Ó»ù±¾×î´óÉúÃüµã
    void SetBaseLifeMax(int nLifeMax);  // ÉèÖÃ»ù±¾×î´óÉúÃüµã
    void AddCurLifeMax(int nLife);
    void AddBaseStaminaMax(int nStamina);  // Ôö¼Ó»ù±¾×î´óÌåÁ¦µã
    void SetBaseStaminaMax(int nStamina);
    void AddCurStaminaMax(int nStamina);
    void AddBaseManaMax(int nMana);  // Ôö¼Ó»ù±¾×î´óÄÚÁ¦µã
    void SetBaseManaMax(int nMana);
    void AddCurManaMax(int nMana);
    void NpcFuMoCastSkll(int nLauncher, int nParam1, int nParam2, int nWaitTime = 0);
    //	void				ResetLifeReplenish();		// ÖØÐÂ¼ÆËãÉúÃü»Ø¸´ËÙ¶È
    //	void				CalcCurLifeMax();			// ¼ÆËãµ±Ç°×î´óÉúÃüµã
    //	void				CalcCurStaminaMax();		// ¼ÆËãµ±Ç°×î´óÌåÁ¦µã
    //	void				CalcCurManaMax();			// ¼ÆËãµ±Ç°×î´óÄÚÁ¦µã
    void CalcCurLifeReplenish();  // ¼ÆËãµ±Ç°ÉúÃü»Ø¸´ËÙ¶È
    void SetSeries(int nSeries);  // Éè¶¨´Ë npc µÄÎåÐÐÊôÐÔ£¨ÄÚÈÝ»¹Ã»Íê³É£©
    void SetFangAndHai(KChanelBaseInfo* nNpcInfo);  // ¸Ä±äNPCµÄËÄ·À£¬ÓÃÓÚ´¥·¢BB
    void SetBaseDamage(int nMix, int nMax, int nType = 0);
    void GetFangAndHai(KChanelBaseInfo* nNpcInfo);
    //	int					GetNpcLevelDataFromScript(KLuaScript * pScript, char * szDataName, int
    //nLevel, char * szParam); 	int					SkillString2Id(char * szSkillString);
    void GetNpcCopyFromTemplate(int nNpcTemplateId, int nLevel);
    //	void				InitNpcLevelData(KTabFile * pTabFile, int nNpcTemplateId, KLuaScript *
    //pLevelScript, int nLevel); 	void				InitNpcBaseData(int nNpcTemplateId);
    void SetPhysicsDamage(int nMinDamage, int nMaxDamage);  // Éè¶¨ÎïÀí¹¥»÷µÄ×î´ó×îÐ¡Öµ
    void SetBaseAttackRating(int nAttackRating);            // Éè¶¨¹¥»÷ÃüÖÐÂÊ
    void SetBaseDefence(int nDefence);                      // Éè¶¨·ÀÓùÁ¦
    //	void				SetBaseWalkSpeed(int nSpeed); // Éè¶¨ÐÐ×ßËÙ¶È 	void
    //SetBaseRunSpeed(int nSpeed);							// Éè¶¨ÅÜ²½ËÙ¶È
    int GetCurActiveWeaponSkill();
    void LoadDataFromTemplate(void* nNpcTemp = NULL);  //(int nNpcTemplateId, int nLevel);
    // unsigned int	    GetColor(const char* pString); //½«×Ö·û´®ÑÕÉ«×ª»»³É16½øÖÆ
    // const               char* GetColorString(unsigned int nColor); //½«16½øÖÆÑÕÉ«×ª»»³É ×Ö·û´®ÐÎÊ½
    void ReSetRes(int nMark);
    void GetFrameCopyFromTemplate(int nNpcTemplateId, int nLevel);
    void LoadFrameFromTemplate(void* nNpcTemp = NULL);  //(int nNpcTemplateId, int nLevel);
    //	int                 NpcInitJinMaiVal(); //¾­Âö³õÊ¼»¯
    //	void                UseSkills(int sKillID,int sLevel,int nNpcIdx); //ÉèÖÃ¼¼ÄÜ×´Ì¬Ð§¹û
    //    int				GetPlayerIdx();
    int CheckMaps(char* nKey, int nMapIdx);
    int CheckAllItem(int nKeyCol, int nGen, int nDetail, int nPart);
    BOOL TestMovePos(int& nMpsX, int& nMpsY, int& nLength, int nSpeed, BOOL bCanJumpOver);

    void OnDrag();  // À­³¶ Ëæ»úÒÆ¶¯

    void SetClientSpr(const char* nSprPath,
                      int nxLeft,
                      int nyTop,
                      int nzPos    = 0,
                      int i        = 0,
                      char* ncPart = NULL);  // ÉèÖÃ Í¬Ê±ÔËÐÐµÄSPR¶¯»­
    void SetSleepMode(BOOL bSleep) { m_nSleepFlag = bSleep; /*m_DataRes.SetSleepState(bSleep);*/ };
    void SetNpcState(int* pNpcState);
    void ActivateWaiGua();
    void RemoveRes();
    void ClientShowMsg(const char* Msg);
    void ProcNetCommand(NPCCMD cmd, int x = 0, int y = 0, int z = 0);
    void Paint();
    int PaintInfo(int nHeightOffset, bool bSelect, int nFontSize = 12, unsigned long dwBorderColor = 0XFF000000);
    void PaintHonor(int nbtRankFFId, int nMpsX, int nMpsY, int i);    // spr³ÆºÅ
    void PaintPifeng(int m_PifengType, int nMpsX, int nMpsY, int i);  // sprÅû·ç³ÆºÅ
    //	void				PaintHonor(char szString[16], int nHeightOffset, int nFontSize = 12, unsigned long
    //dwBorderColor = 0);
    int PaintChat(int nHeightOffset);
    int SetChatInfo(const char* Name, const char* pMsgBuff, unsigned short nMsgLength);  // ×Ô¼ºNPCËµ»°
    int PaintLife(int nHeightOffset, bool bSelect);
    int PaintMana(int nHeightOffset);
    void DrawBorder();
    int DrawMenuState(int n);
    void DrawBlood();                           // »æÖÆÑªÌõºÍÃû×ÖÔÚ¹Ì¶¨Î»ÖÃ
    BOOL IsCanInput() { return m_ProcessAI; };  // ÈËÎïAI ±êÖ¾
    void SetMenuState(int nState, char* lpszSentence = NULL, int nLength = 0);  // Éè¶¨Í·¶¥×´Ì¬
    int GetMenuState();                                                         // »ñµÃÍ·¶¥×´Ì¬
    unsigned long SearchAroundID(unsigned long dwID);       // ²éÕÒÖÜÎ§9¸öRegionÖÐÊÇ·ñÓÐÖ¸¶¨ ID µÄ npc
    void SetSpecialSpr(char* lpszSprName);  // Éè¶¨ÌØÊâµÄÖ»²¥·ÅÒ»±éµÄËæÉísprÎÄ¼þ
    void SetFrameSpr(char* lpszSprName, int nX, int nY, int nHeight, int mInterval = 0);
    void SetInstantSpr(int nNo);
    int GetNormalNpcStandDir(int nFrame);
    KNpcRes* GetNpcRes() { return NULL; /*&m_DataRes;*/ };  // µÃµ½NPCµÄÍâ¹Û
    // KImageParam	        imgParam;
    int GetNpcPate();
    int GetNpcPatePeopleInfo();

    // Ã°Ñª´¦Àí
    void ClearBlood(int i);
    void SetBlood(int nNo);
    int PaintBlood(int nHeightOffset);  // »æÖÆÃ°Ñª
    int PaintOther();
    int PaintNewBlood(int nHeightOffset, int Val);
    void ApplySynNpcInfo(unsigned long nNpcDwid);
};

// µ¼ÖÂÄÚ´æì­Éý MAX_NPC
extern KNpc* Npc;  // Npc[MAX_NPC];
#endif
