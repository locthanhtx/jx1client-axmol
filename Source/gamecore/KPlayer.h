#ifndef KPlayerH
#define KPlayerH

// #include "KWorldMsg.h"
// #include "KMessage.h"

#include "KInventory.h"
/*
#include "KPlayerChat.h"
*/
#include "KPlayerTong.h"

#include "KPlayerMenuState.h"
#include "KSkillManager.h"
#include "KItemList.h"
#include "KSkills.h"
#include "KNpc.h"
#include "KPlayerDef.h"
#include "KPlayerFaction.h"
#include "KPlayerTeam.h"
#include "KPlayerTask.h"
#include "KPlayerPK.h"

#include "KPlayerTrade.h"
#include "KPlayerRepute.h"
#include "KPlayerFuYuan.h"
#include "KPlayerReBorn.h"
#include "KPlayerVip.h"
#include "engine/KTabFileCtrl.h"
#define MAX_ANSWERNUM              16  //
#define PLAYER_LIFE_REPLENISH      0
#define PLAYER_MANA_REPLENISH      0
#define PLAYER_STAMINA_GAIN        1
#define PLAYER_STAMINA_LOSS        4

#define STRENGTH_SET_DAMAGE_VALUE  5
#define DEXTERITY_SET_DAMAGE_VALUE 5

// #define		MAX_AVENGE_NUM					4

#define BASE_FIRE_RESIST_MAX    150          // 275  //������ֵ
#define BASE_COLD_RESIST_MAX    150          //     //�������ֵ
#define BASE_POISON_RESIST_MAX  150          //     //�������ֵ
#define BASE_LIGHT_RESIST_MAX   150          //     //�׷����ֵ
#define BASE_PHYSICS_RESIST_MAX 150          //     //�շ����ֵ
#define BASE_FANGYU_ALL_MAX     75           // ԭʼ�����
#define defPLAYER_SAVE_TIMEOUT  18 * 60 * 5  // �浵ʱ��

enum UIInfo  // �ű�֪ͨ��ʾ�Ľ�������
{
    UI_SELECTDIALOG,
    UI_TRADEDIALOG,
    UI_TALKDIALOG,
    UI_NOTEINFO,
    UI_MSGINFO,   // ��������ð��������Ϣ
    UI_NEWSINFO,  // ��������
    UI_PLAYMUSIC,
    UI_OPENTONGUI,
};

// ������λ����Ϣ
typedef struct PLAYER_REVIVAL_POS_DATA
{
    int m_nSubWorldID;  // �������ͼ
    int m_ReviveID;     // ����������
    int m_nMpsX;        // �������ͼλ�� x
    int m_nMpsY;        // �������ͼλ�� y
} PLAYER_REVIVAL_POS;

typedef struct
{
    int m_nSubWorldId;  // ����������ID
    int m_nTime;        // �����ű���ʱ��
    int m_nMpsX;
    int m_nMpsY;
} PLAYER_TOWNPORTAL_POS;

typedef struct tPLAYER_EXCHANGE_POS
{
    UINT m_dwMapID;
    int m_nX;
    int m_nY;
} PLAYER_EXCHANGE_POS;

struct KplayerTimeInfo
{
    int nMapLetTime;
    int nExpLeftTime;
    int nLuckLeftTime;
    int nKangLeftTime;
    int nYaoLeftTime;
};

// ����ϻ����ṹ
struct KplayerGuzi
{
    int nIsOpen;
    int nTpye;
    int nYingL;
    int nJinBi;
    int nDianShu;
    int nBeiLv;
};

struct KplayerAuto
{
    struct Kteam
    {
        BYTE nIsHuiDa;
        BYTE nIsDuiZhang;
        BYTE nIsDuiYuan;
    };  // ��ӵ�

    struct Kskill
    {
        BYTE nIsOpen;
        int nsKillId;
        int nCostTime;
        int nsKillRows;
        int nsFactionTaskVal;
        // int     nSkilloop;
    };

    struct KLRskill
    {
        BYTE nIsOpen;
        int nsLeftKillId;
        int nsRightKillId;
        int nCostTime;
        int nsKillRows;
        int nsFactionTaskVal;
        int nSkilloop;
    };
    int nSkilloop;
    unsigned long nLocKnpcDw;
    int nCurXpos;
    int nCurYpos;
    int nOldIndex;  // ���ֵ�NPC������
    int nHideNpc;
    int nHidePlayer;
    int nHideMiss;
    int nAutoMoney;  // Ǯ
    int nAutoEques;  // װ��
    int nAutoMoneyAndEques;
    BYTE nShuXingNum;  // ��������
    int nAutoTuPu;     // ͼ��
    BYTE nAutoTool;    // ����
    BYTE nAutoOther;   // ���װ������
    BYTE nIsAuToFenJie;
    int nIsYaBiao;
    BYTE nCurNum;
    BYTE nIsHaveDo;
    BYTE nIsJiaoBen;  // �Ƿ�¼�ƽű�
    BYTE nAutoModel;
    BYTE nCurStaus;    // ��ǰ��״̬
    BYTE nIsOpenZiMu;  // �Ƿ��Զ��Ž���ĸ��
    BYTE nIsAutoSay;   // �Ƿ��Զ�����
    BYTE nIsAutoNuQi;
    BYTE nIsAutoUseItem;  // �Զ�ʹ����Ʒ
    BYTE nIsShowPath;     // �Ƿ���ʾ·��
    BYTE nIsNoBlue;       // �Ƿ񲻹������̹�
    BYTE nIsNoYellow;     // �Ƿ񲻹����ƹ�
    BYTE nIsNoRed;        // �Ƿ񲻹������
    int nIsFollow;        // �Ƿ����ӳ�
    int nIsFell;
    int nAttackNum;
    int _checkLoopTime;
    Kteam nCteam;
    int nNpcCurlife;  // ���ݵ�NPC����
    int nShiQuNum;
    int nVal;
    int nAutoTime;  // ȡ����ȡ��ʱ�� ��ʱ������� ȡ����ȡ �򹥻�
    int ngMoneyLoop;
    int ngItemLoop;
    int ngTuzhiLoop;
    int ngToolLoop;
    int ngAttackLoop;
    Kskill nAutoSkill[4];
    KLRskill nAutoLRskill;
    int nIsReTurn;  // �Ƿ����ڻس�
    int nIsUseSkill;
    int nNpcDis;     // Ѱ�ַ�Χ
    int nBackIndex;  // ���ݵ� ѡ��NPC�� ����
    int nRunLoop;
    int nIsOpenPiick;
};

class KIniFile;

class KPlayer
{
private:
    int m_nLeftSkillID;
    int m_nLeftListidx;
    int m_nLeftSkillLevel;
    int m_nRightSkillID;
    int m_nRightListidx;
    int m_nRightSkillLevel;

    int m_MouseDown[2];

    UINT m_nExtPoint;  // �����

    int m_nPhysicsSkillID;  // ��ǰ��ҵ�������������
    int m_nPeapleIdx;
    int m_nObjectIdx;
    int m_nPickObjectIdx;
    int m_nPlayerIndex;  // ��ʵ���� Player �����е�λ��
                         // KCacheNode *	m_pLastScriptCacheNode;

public:
    UINT m_GameLeftTime;
    int m_Isinvalid;
    int m_nRoomFull;  // �����Ƿ�����

    int m_IsTranse;                      // �Ƿ���
    int m_CurGsSerIdx;                   // ��ǰ������
    int m_nImageNpcID;                   // id npc SPR
    int m_RunStatus;                     // ���ܻ�����
    unsigned long m_dwNextLevelLeadExp;  // ͳ������һ������ֵ
    int m_nSendMoveFrames;  // ���ڿ��ƿͻ���������������ƶ�(�߻���)Э���Ƶ�ʣ�ʹ֮���ܷ��ʹ������ƶ�Э�飬��С����ѹ��
    // char            szClienResult[256];       // �ű�����ֵȫ�ֱ���
    KTabFile nPos, nYaBiao, nEquipItem, nCheckName;
    KIniFile m_FileDesc;  //,nEquipItem,nToolItem,nCheckName;
    KTabFile nGuaSkill;
    KTabFileCtrl nMapPos;

    unsigned long m_ItemLinkDwid;  // ��ǰװ�����ӵ���ƷID
    TMissionLadderSelfInfo m_MissionData;
    TMissionLadderInfo m_MissionRank[MISSION_STATNUM];

    int m_ExpTime;           //  ��ǰ��ȡ����ʱ��
    int m_YinLiangTime;      //  ��ǰ��ȡ����ʱ��
    int m_JinBiTime;         //  ��ǰ��ȡ���ʱ��
    int m_ExpPiontTime;      //  ��ǰ��ȡ�㿨ʱ��
    KplayerAuto m_Autoplay;  // �������ϵͳ(����)
    int m_DebugMode;         // ����ģʽ

    int m_nCurPinDao;  // ��ʱƵ��
    enum
    {
        FF_CHAT = 0x01,
    };
    BYTE m_nForbiddenFlag;  // ��ֹ���Ա�־
    KIndexNode m_Node;
    GUID m_Guid;            // �������
    int m_bExchangeServer;  // �Ƿ���״̬

    UINT m_dwID;            // ��ҵ�32λID
    int m_nIndex;           // ��ҵ�Npc���
    int m_nNetConnectIdx;   // �ڼ�����������
    KItemList m_ItemList;   // ��ҵ�װ���б�
    BuySellInfo m_BuyInfo;  // ���еĽ����б�
    // NewBuySellInfo	m_NewBuyInfo;				// ���еĽ����б�
    KPlayerMenuState m_cMenuState;  // �Ƿ��ڽ��׻���鿪��״̬
    KTrade m_cTrade;                // ����ģ��
    int m_nAttributePoint;          // δ�������Ե�
    int m_nSkillPoint;              // δ���似�ܵ�
    int m_nChestPW;                 // ���� �Ѿ��� ���е�ͼ ʹ��
    int m_nStrength;                // ��ҵĻ������������������˺���
    int m_nDexterity;               // ��ҵĻ������ݣ��������С�������
    int m_nVitality;  // ��ҵĻ�������������������������
    int m_nEngergy;   // ��ҵĻ�������������������
    int m_nLucky;     // ��ҵĻ���������Ӱ��õ�װ���ĺû���
    BYTE m_btChatSpecialChannel;
    int m_nUpExp;  // ɱ�����˻�þ������Ӱٷֱ�

    int m_nCurStrength;   // ��ҵĵ�ǰ���������������˺���   ����
    int m_nCurDexterity;  // ��ҵĵ�ǰ���ݣ��������С������� ����
    int m_nCurVitality;   // ��ҵĵ�ǰ���������������������� �⹦
    int m_nCurEngergy;    // ��ҵĵ�ǰ����������������       �ڹ�
    int m_nCurLucky;      // ��ҵĵ�ǰ������Ӱ��õ�װ���ĺû���
    int m_nTempLucky_p;   // ��¼��ҵ���ʱװ���������˰ٷֱ�
    BYTE m_nVipType;      // ��Ա������
                          //	int             m_nZhonji;                  // ��ǰ�ػ�
                          //	int             m_nDangge;                  // ��ǰ����

    int m_nExp;           // ��ǰ����ֵ(��ǰ�ȼ���npc����)
    int m_nNextLevelExp;  // ��һ������ֵ
    BYTE m_CUnlocked;     // �ͻ����Ƿ���� ��Ļ

    UINT m_dwLeadExp;       // ͳ��������ֵ
    UINT m_dwLeadLevel;     // ͳ�����ȼ�
    char m_PlayerName[32];  // ��ҵĽ�ɫ����
    // char			m_PeiouName[32];            // ��ż����

    KPlayerTeam m_cTeam;        // ��ҵ������Ϣ
    KPlayerFaction m_cFaction;  // ��ҵ�������Ϣ

    //	KPlayerChat		m_cChat;                    // ���ͷ��˵����ʾ

    KPlayerTask m_cTask;  // �������ϵͳ(����)

    //	KplayerGuzi		m_GuziGame;	                // ��ҵ��ϻ���ϵͳ

    KplayerTimeInfo m_MapTimeInfo;  // ���˲� ��Чҩ�浵

    KPlayerPK m_cPK;  // PK��ϵ����

    KPlayerRepute m_cRepute;  // ����

    KPlayerFuYuan m_cFuYuan;  // ��Դ

    KPlayerReBorn m_cReBorn;  // ת��

    KPlayerVip m_cVip;  // ��Ա

    KPlayerTong m_cTong;  // �Լ��İ����Ϣ

    unsigned long m_dwDeathScriptId;  // �����ű�

    char m_szTaskAnswerCanshu[MAX_ANSWERNUM][32];  // ��� ������ �ű�����
    char m_szTaskAnswerFun[MAX_ANSWERNUM][32];
    int m_nAvailableAnswerNum;  // ��ǰѡ������£����ش�����
    int m_nWorldStat;           // ����
    int m_nSectStat;            // �������
    bool m_isopenjuli;

    unsigned long m_nGiveCallBackScript;
    BYTE m_nGiveCallBackNum;

public:
    KPlayer();
    ~KPlayer();

    void SetPlayerIndex(int nNo);        // �趨 m_nPlayerIndex
    void GetAboutPos(KMapPos* pMapPos);  // �����Ҹ���һ����λ��
    void GetNewAboutPos(KMapPos* pMapPos);
    int GetPlayerIndex();  // ��ñ�ʵ���� Player �����е�λ��
    UINT GetPlayerID() { return m_dwID; };
    void GetFactionName(char* lpszName, int nSize);             // ��õ�ǰ�������� not end
    void ChatFriendOnLine(unsigned long dwID, int nFriendIdx);  // ���֪ͨĳ����������
    int ExecuteScriptA(char* ScriptFileName,
                       char* szFunName,
                       int nParam     = 0,
                       char* szCanshu = NULL,
                       int nIsGive    = TRUE);  // NPC�Ի������
    int ExeNoBackScript(char* ScriptFileName,
                        char* szFunName,
                        int nKind   = 0,
                        int nParama = 0,
                        int nParamb = 0,
                        int nParamc = 0);
    int ExecuteScriptC(char* ScriptFileName,
                       const char* szFunName,
                       const char* szParams = NULL,
                       const char* snParams = NULL,
                       int inParam          = 0,
                       int njb              = 0,
                       int njxb             = 0,
                       int inval            = 0,
                       int invala           = 0,
                       int nIsGive          = TRUE);
    int ExecuteScriptD(unsigned long dwScriptId,
                       const char* szFunName,
                       const char* szParams = NULL,
                       const char* snParams = NULL,
                       int inParam          = 0,
                       int njb              = 0,
                       int njxb             = 0,
                       int inval            = 0,
                       int invala           = 0,
                       int nIsGive          = FALSE);
    int ExecuteScriptB(unsigned long dwScriptId,
                       char* szFunName,
                       int nParam     = 0,
                       char* szCanshu = NULL,
                       int nIsGive    = TRUE);
    int DoScript(char* ScriptCommand);  // ִ��ĳ���ű�ָ��
    int ExeItemScriptA(const char* ScriptFileName,
                       const char* szFunName,
                       char* szParams   = NULL,
                       char* snParams   = NULL,
                       int inParam      = 0,
                       int njb          = 0,
                       int njxb         = 0,
                       int inGenre      = 0,
                       int inDetail     = 0,
                       int inParticular = 0,
                       int inStackNum   = 0,
                       int inItemX      = 0,
                       int inItemY      = 0);
    int ExeItemScript(unsigned long dwScriptId,
                      const char* szFunName,
                      char* szParams   = NULL,
                      char* snParams   = NULL,
                      int inParam      = 0,
                      int njb          = 0,
                      int njxb         = 0,
                      int inGenre      = 0,
                      int inDetail     = 0,
                      int inParticular = 0,
                      int inStackNum   = 0,
                      int inItemX      = 0,
                      int inItemY      = 0);
    void myPrintLuaStackA(lua_State* L);  // ����
    void ChangeCurStrength(int nData);    // �ı䵱ǰ����(�� nData С�� 0 ʱ������)
    void ChangeCurDexterity(int nData);   // �ı䵱ǰ����(�� nData С�� 0 ʱ������)
    void ChangeCurVitality(int nData);    // �ı䵱ǰ����(�� nData С�� 0 ʱ������)
    void ChangeCurEngergy(int nData);     // �ı䵱ǰ����(�� nData С�� 0 ʱ������)
                                          //	int			ExecuteScriptC(char * ScriptFileName);
    void Release();
    void Active();  // ���ÿ����Ϸѭ������Ҫ�����Ķ���
    //	void			ProcessMsg(KWorldMsgNode *lpMsg);		// ����������Ϣ��תΪNPC����
    void ReSkillEnhance();
    char* GetPlayerName() { return m_PlayerName; };

    int NewPlayerGetBaseAttribute(int Series);  // ����ҵ�½ʱ�����������Բ��� ���� ���� ���� ���� ������ֵ
    void AddBaseLucky(int nData);               // ���ӻ�������

    void DirectAddExp(int nExp);  // ֱ�����Ӿ���ֵ����������������
    void LevelUp();               // ��һ��
    void AddLeadExp(int nExp);    // ����ͳ��������
    void UpdataCurData(int nIsLoign = 0, int nUnEquip = 0);
    void ReCalcEquip();                  // ���¼������ϵ�װ��
    void ReCalcState(int nIsLoign = 0);  // ���¼������ϵ�״̬
    int CheckThisItem(int nSetings, char* nKey);
    void ChangePlayerCamp(int nCamp);                     // �ı������Ӫ
    void Revive(int nType);                               // ����
    void GetEchoDamage(int* nMin, int* nMax, int nType);  // ��ȡ������Ҫ��ʾ���˺�ֵ
    int CheckTrading();
    void SetFirstDamage();  // ���û����˺�
    void SetBaseAttackRating();
    void SetBaseDefence();
    void SetBaseResistData();
    void SetBaseSpeedAndRadius();
    int GetPhysicsSkillId() { return m_nPhysicsSkillID; };
    void SetPhysicsSkillId(int nPhysicsSkillID)
    {
        if (nPhysicsSkillID <= 0)
            return;
        ISkill* pISkill = g_SkillManager.GetSkill(nPhysicsSkillID, 1);
        if (!pISkill)
            return;

        if (nPhysicsSkillID > 0 && pISkill->IsPhysical())
            m_nPhysicsSkillID = nPhysicsSkillID;
    };
    int GetLeftSkill() { return m_nLeftSkillID; };
    int GetLeftSkillListidx() { return m_nLeftListidx; };
    int GetRightSkill() { return m_nRightSkillID; };
    int GetRightSkillListidx() { return m_nRightListidx; };

    void SetExtGameTime(unsigned long nPoint);

    int GetTargetNpc() { return m_nPeapleIdx; };
    int GetTargetObj() { return m_nObjectIdx; };
    void SetTargetNpc(int n) { m_nPeapleIdx = n; };
    void SetTargetObj(int n) { m_nObjectIdx = n; };
    void FindSelectNpc(int x, int y, int nRelation);
    void FindSelectObject(int x, int y);
    void Walk(int nDir, int nSpeed);
    void TurnLeft();
    void TurnRight();
    void TurnBack();
    void DrawSelectInfo();
    int ConformIdx(int nIdx);
    //	void			GetEchoDamage(int* nMin, int* nMax, int nType);// ��ȡ������Ҫ��ʾ���˺�ֵ
    void ProcessInputMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);  // �������������Ϣ
    void RecvSyncData();                                            // ����ͬ������
    int ClientExeItemScript(const char* nScriptPath,
                            const char* nFunName,
                            int nPaparm,
                            int ninVal,
                            const char* nstrVala);
    void ApplyTeamInfo(unsigned long dwNpcID);  // ������������ѯĳ��npc���ڶ������Ϣ
    void ApplyRoomInfo(int nIsFull);
    void ApplySelfTeamInfo();  // ������������ѯ��������Ķ������
    int ApplyCreateTeam();     // char *lpszTeamName);		// �������������봴������
    int ApplyTeamOpenClose(int bFlag);  // �ӳ�����������뿪�š��رն����Ƿ����������Ա״̬
    void ApplyAddTeam(int nNpcIndex);  // �����������������ĳ������
    void AcceptTeamMember(unsigned long dwNpcID);  // ���֪ͨ����������ĳ��npcΪ�����Ա
    void TeamDropApplyOne(unsigned long dwNpcID);  // �ӳ�ɾ��������������б��е�ĳ��npc
    void LeaveTeam();                              // ֪ͨ������������뿪����
    void TeamKickMember(unsigned long dwNpcID);  // �ӳ�֪ͨ�������߳�ĳ����Ա
    void ApplyTeamChangeCaptain(unsigned long dwNpcID);  // �ӳ��������������Լ��Ķӳ����ݽ�����Ķ�Ա
    void ApplyTeamDismiss();   // �ӳ�������������ɢ����
    void ApplySetPK(int bPK);  // ��������������򿪡��ر�pk����
    void ApplyFactionData();   // ����������������������
    void SendChat(KUiMsgParam* pMsg, char* lpszSentence);  // �ͻ��˷�����������������
    void ApplyAddBaseAttribute(
        int nAttribute,
        int nNo);  // �ӳ��������������������������ĳһ��ĵ���(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
    int ApplyAddSkillLevel(int nSkillID, int nAddPoint);  // �����������ĳ����������
    int ApplyUseItem(int nItemID, ItemPos SrcPos);  // �����������ʹ��ĳ����Ʒ������Ҽ��������Ʒ��
    int AutoChangeItem(int nItemID, ItemPos SrcPos, ItemPos DestPos);
    int ApplyAutoMoveItem(int nItemID, ItemPos SrcPos, int destPlace);
    void PickUpItem(int nObjIndex, int rid);
    void PickUpMoney(int nObjIndex, int rid);
    void PickUpObj(int nObjIndex);  // �ͻ��������obj����ĳ����Ʒ�������������Ϣ
    void ObjMouseClick(int nObjIndex);  // �ͻ��������obj�������������Ϣ
    int MoveItem(ItemPos DownPos, ItemPos UpPos, int nIsComp = 0);  // DownPos �������֣�UpPos ��������
    void MoveSkill(ItemPos DownPos, ItemPos UpPos);
    int ThrowAwayItem();
    int AutoAwayItem(unsigned long nItmeDwIdx, bool isAttack = true);  //---------------��Ҷ�����-------------------
    void ChatAddFriend(int nPlayerIdx);  // �ͻ���ͨ�����˵�����������ѵ�����
    void ChatRefuseFriend(int nPlayerIdx);  // �ͻ��˾ܾ����˵�����������ѵ�����
    void TradeApplyOpen(char* lpszSentence, int nLength);
    void TradeApplyClose();
    void TradeApplyStart(int nNpcIdx);
    int TradeMoveMoney(int nMoney);  // ����ʱ�����Լ���Ǯ
    void TradeDecision(
        int nDecision);  // if nDecision == 0 �Ƴ�����  if nDecision == 1 ȷ������  if nDecision == 2 ȡ������ȷ��
    void TradeApplyLock(int nLockOrNot);  // ����������ȡ������

    void SetChatCurChannel(int nChannelNo);     // �趨��ǰ����Ƶ��
    void TeamInviteAdd(unsigned long dwNpcID);  // ����������

    void SetLeftSkill(int nSkillID, int nIsNoLogin = FALSE);
    void SetRightSkill(int nSkillID);
    void SetShiquSkill(int nSkillID);
    void UpdateWeaponSkill();
    //	int				GetLeftSkill(){return m_nLeftSkillID;};
    //	int				GetRightSkill(){return m_nRightSkillID;};
    void SetDefaultImmedSkill();
    void s2cApplyAddTeam(BYTE* pProtocol);  // �յ�������֪ͨ��������������
    void s2cTradeChangeState(BYTE* pMsg);
    void s2cTradeMoneySync(BYTE* pMsg);
    void s2cTradeDecision(BYTE* pMsg);  // �յ�������֪ͨ������ɻ�ȡ��
    void SyncCurPlayer(BYTE* pMsg);
    void s2cLevelUp(BYTE* pMsg);
    void s2cGetCurAttribute(BYTE* pMsg);
    void s2cSetExp(int nExp);
    void s2cSyncMoney(BYTE* pMsg);
    void s2cSyncXu(BYTE* pMsg);
    void s2cTradeApplyStart(BYTE* pMsg);
    void CheckObject(int nIdx);

    UINT GetExtPoint();

private:
    void CalcCurLucky();            // ���㵱ǰ����
    void LevelAddBaseLifeMax();     // �ȼ���һ�����������������
    void LevelAddBaseManaMax();     // �ȼ���һ�����������������
    void LevelAddBaseStaminaMax();  // �ȼ���һ�����������������

    void SendFactionData();  // ��ͻ��˷�����������

    void SyncCurrentBaseAttriibute(int type, int attribute, int curAttribute);  // ham moi
    void AddBaseStrength(int nData);   // ���ӻ�������  ---����
    void AddBaseDexterity(int nData);  // ���ӻ�������  ---����
    void AddBaseVitality(int nData);   // ���ӻ�������---�⹦
    void AddBaseEngergy(int nData);    // ���ӻ�������---�ڹ�

    void ResetBaseStrength(int nData);   // ���ӻ�������
    void ResetBaseDexterity(int nData);  // ���ӻ�������
    void ResetBaseVitality(int nData);   // ���ӻ�������
    void ResetBaseEngergy(int nData);    // ���ӻ�������

    void SetNpcPhysicsDamage();  // �ɵ�ǰ���������Ӧnpc�������˺�(PhysicsDamage)
    void SetNpcAttackRating();   // �ɵ�ǰ���ݼ����Ӧnpc�Ĺ���������(AttackRating)
    void SetNpcDefence();        // �ɵ�ǰ���ݼ����Ӧnpc�ķ�����
                                 //	void			SetNpcWalkSpeed();						//
    //�ɵ�ǰ���ݼ����Ӧnpc�������ٶ� 	void			SetNpcRunSpeed(); // �ɵ�ǰ���ݼ����Ӧnpc���ܲ��ٶ�
    void ProcessMouse(int x, int y, int Key, MOUSE_BUTTON nButton);
    void OnButtonUp(int x, int y, MOUSE_BUTTON nButton);             // ��������̧��
    void OnButtonDown(int x, int y, int Key, MOUSE_BUTTON nButton);  // ������������
    void OnButtonMove(int x, int y, int Key, MOUSE_BUTTON nButton);  // �����������º��ƶ�
    void OnMouseMove(int x, int y);                                  // ��������ƶ�
    int NetCommandPlayerTalk(BYTE* pProtocol);

    // Question:Ϊ�������԰�ʹ��
    // friend		    int LuaInitStandAloneGame(Lua_State * L);

    // �����Ķ���
private:
    void S2CExecuteScript(char* ScriptName, char* szParam);

public:
    void SetNpcDamageAttrib();
    void DoScriptAction(PLAYER_SCRIPTACTION_SYNC* pUIInfo);  // ֪ͨ�ÿͻ�����ʾĳ��UI����
    void ProcessPlayerSelectFromUI(BYTE* pProtocol);  // ��������Ҵ�ѡ��˵�ѡ��ĳ��ʱ�Ĳ���

    void DialogNpc(int nIndex, int nIsServer = TRUE);
    void OpenBaiTan(unsigned long nIndwId);
    void OnSelectFromUI(
        PLAYER_SELECTUI_COMMAND* pSelectUI,
        UIInfo eUIInfo);  // ����Ҵ�ѡ�����ѡ��ĳ��󣬽������������
    void OnScriptAction(PLAYER_SCRIPTACTION_SYNC*);
};

extern KPlayer* Player;  // Player[MAX_PLAYER];

#endif  // KPlayerH
