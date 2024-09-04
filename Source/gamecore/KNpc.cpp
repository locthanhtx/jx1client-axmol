//-----------------------------------------------------------------------
//	Sword3 KNpc.cpp
//-----------------------------------------------------------------------
#include "KCore.h"
//#include <crtdbg.h>
#include "KNpcAI.h"
#include "KSkills.h"
#include "KObj.h"
#include "KObjSet.h"
#include "KMath.h"
#include "KPlayer.h"
#include "KNpc.h"
#include "GameDataDef.h"
#include "KRegion.h"
#include "KNpcTemplate.h"
#include "KItemSet.h"
//#include "KForBitGua.h"
///////ԭ��û�е�/////////////////////////////////////
#include  "KPlayerFaction.h"
#include  <time.h>
//#include "KSubWorld.h"
#include "KSubWorldSet.h"

////////////////////////
#ifdef WIN32
#include <Tlhelp32.h>
#include "tchar.h"
#include "psapi.h"
#endif
////////////////////////////////////////////////////////
//�ͻ���
//#include "../../../Headers/IClient.h"
#include "CoreShell.h"
//#include "Scene/KScenePlaceC.h"
//#include "KIme.h"
//#include "../../Represent/iRepresent/iRepresentshell.h"
#include "ImgRef.h"
#include "engine/Text.h"

//extern iRepresentShell*	g_pRepresentShell;
extern KImageStore2 m_ImageStore;

#include "KNpcAttribModify.h"
#include "CoreUseNameDef.h"
#include "gamescene/ObstacleDef.h"
#include "KThiefSkill.h"
/*
#ifdef _STANDALONE
#include "KThiefSkill.cpp"
#endif
*/
#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif

extern KLuaScript		*g_pNpcLevelScript;

#define	ATTACKACTION_EFFECT_PERCENT		60	// �����ܶ�����ɰٷ�֮���ٲ�����������
#define	MIN_JUMP_RANGE					0
#define	ACCELERATION_OF_GRAVITY			10
#define		SHOW_CHAT_COLOR				0xffffffff
#define		SHOW_BLOOD_COLOR			0x00ff0000
#define		defMAX_SHOW_BLOOD_TIME		50
#define		defSHOW_BLOOD_MOVE_SPEED	1

#define		SHOW_LIFE_WIDTH				38
#define		SHOW_LIFE_HEIGHT			3

#define		SHOW_SPACE_HEIGHT			1

#define		FIND_PATH_DISTANCE		1
//-----------------------------------------------------------------------
#define FRAME2TIME				18
//#define AEXP_INTERVAL			5400
#define	GAME_UPDATE_TIME		10
//#define	GAME_SYNC_LOSS			100
//#define	STAMINA_RECOVER_SCALE	4
// ����Ŀ�ߣ����ӵ�λ��
#define	REGIONWIDTH			SubWorld[m_SubWorldIndex].m_nRegionWidth
#define	REGIONHEIGHT		SubWorld[m_SubWorldIndex].m_nRegionHeight
// ���ӵĿ�ߣ����ص�λ���Ŵ���1024����
#define	CELLWIDTH			(SubWorld[m_SubWorldIndex].m_nCellWidth << 10)
#define	CELLHEIGHT			(SubWorld[m_SubWorldIndex].m_nCellHeight << 10)
// ��ǰ����
#define	CURREGION			SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex]
// �������������
#define	LEFTREGIONIDX		CURREGION.m_nConnectRegion[2]
#define	RIGHTREGIONIDX		CURREGION.m_nConnectRegion[6]
#define	UPREGIONIDX			CURREGION.m_nConnectRegion[4]
#define	DOWNREGIONIDX		CURREGION.m_nConnectRegion[0]
#define	LEFTUPREGIONIDX		CURREGION.m_nConnectRegion[3]
#define	LEFTDOWNREGIONIDX	CURREGION.m_nConnectRegion[1]
#define	RIGHTUPREGIONIDX	CURREGION.m_nConnectRegion[5]
#define	RIGHTDOWNREGIONIDX	CURREGION.m_nConnectRegion[7]

#define	LEFTREGION			SubWorld[m_SubWorldIndex].m_Region[LEFTREGIONIDX]
#define	RIGHTREGION			SubWorld[m_SubWorldIndex].m_Region[RIGHTREGIONIDX]
#define	UPREGION			SubWorld[m_SubWorldIndex].m_Region[UPREGIONIDX]
#define	DOWNREGION			SubWorld[m_SubWorldIndex].m_Region[DOWNREGIONIDX]
#define	LEFTUPREGION		SubWorld[m_SubWorldIndex].m_Region[LEFTUPREGIONIDX]
#define	LEFTDOWNREGION		SubWorld[m_SubWorldIndex].m_Region[LEFTDOWNREGIONIDX]
#define	RIGHTUPREGION		SubWorld[m_SubWorldIndex].m_Region[RIGHTUPREGIONIDX]
#define	RIGHTDOWNREGION		SubWorld[m_SubWorldIndex].m_Region[RIGHTDOWNREGIONIDX]

#define	CONREGION(x)		SubWorld[m_SubWorldIndex].m_Region[CURREGION.m_nConnectRegion[x]]
#define	CONREGIONIDX(x)		CURREGION.m_nConnectRegion[x]

//-----------------------------------------------------------------------
// Npc[0]������Ϸ������ʹ�ã���Ϊһ��NpcSet��������µ�NPC��
//ԭ���е� MAX_NPC
KNpc	*Npc= NULL;//Npc[MAX_NPC];

KSubWorldSet    *M_SubWorldSet;
//KNpcTemplate	* g_pNpcTemplate[MAX_NPCSTYLE][MAX_NPC_LEVEL]; //0,0Ϊ���

//-----------------------------------------------------------------------

KNpc::KNpc()
{

	Init();
}


KNpc::~KNpc()
{//�������� �ͷ���Դ
//#ifdef _SERVER
   	nEnhanceInfo.clear();
	nstrNoteInfo.clear();
//#endif
}

void KNpc::Init() //npc���ݳ�ʼ��
{
	m_dwID = 0;
	m_AdjustColorKind = kind_color_physics;
	m_IsRevive = 0;
	m_IsRe = 0;  //�Ƿ��������
	m_Index = 0;
	m_nPlayerIdx = 0;
	m_ProcessAI = 1;
	m_Kind = kind_normal;
	m_Series = series_metal;
	m_Camp = camp_free;
	m_CurrentCamp = camp_free;
	m_Doing = do_stand;
	m_Height = 0;
	m_Frames.nCurrentFrame = 0;
	m_Frames.nTotalFrame = 0;
	m_SubWorldIndex =0;
	m_RegionIndex = -1;
	m_Experience = 0;
	m_ActiveSkillID = 0;
	m_ActiveSkListIndex=0;
	m_IsMoreAura=false;
	m_SkillParam1 = 0;
	m_SkillParam2 = 0;
	m_KillNumber  =0;  //ɱ������ ��������
	//ZeroMemory(m_nFuMoNum,sizeof(m_nFuMoNum));

	//m_isClearSpr = false;
	m_nCurJiHuo=0;
	m_ZhenYuan=0;             //��Ԫֵ
	//m_WhereSer=1;             //�Ǹ�������
    m_JinMaiBingJia=0;        //��������ֵ
	IsCreatTongBan=0;         //�Ƿ�����ٻ�ͬ��
	//////��ʱ���������������ݿ�///////
	m_ZhuaVal = 0;
	m_TongBanNum = 0;
	m_IsSerLock  = 0;
    m_njxb=0;    //Я���Ľ��
	m_njb=0;
	m_IsDoing=0;
	m_IsSynDir=0;
//	ZeroMemory(m_WarTongNamea,sizeof(m_WarTongNamea)); 
//	ZeroMemory(m_WarMaster,sizeof(m_WarMaster)); 
//	ZeroMemory(m_ChenHaoName,sizeof(m_ChenHaoName)); 	
//	ZeroMemory(m_GuishuName,sizeof(m_WarMaster)); //ͬ�����
	ZeroMemory(&m_ExpState, sizeof(m_ExpState));
	ZeroMemory(&m_DoScriptState, sizeof(m_DoScriptState));
	ZeroMemory(&m_randmove, sizeof(m_randmove));
	ZeroMemory(&m_MapUseModel, sizeof(m_MapUseModel));
	ZeroMemory(&_NpcShadow, sizeof(_NpcShadow));
	/*_EnhanceInfo::iterator it;
	for( it = nEnhanceInfo.begin(); it != nEnhanceInfo.end(); ++it )
	{
		if (it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}
    nEnhanceInfo.clear();*/

    //t_sprintf(m_GuishuName,"ϵͳ");
	SetstrInfo(STR_GUISHU_NAME,"ϵͳ");
	m_GuiShuDwid = 0;
	m_IsDel=FALSE;
//	m_WarShuishou=0;   //˰��
//	m_Warzhi=0;
	m_IsWarCity=-1;
	m_IsInCity=0;
	m_nMissionGroup =-1;
/*	m_WarFucheng=0;   //����
    m_WarZuoHu=0;     //�󻤷�
	m_WarYouHu=0;     //�һ���
	m_WarTaishi=0;     //̫ʷ
	m_WarZhongShu=0;   //����
	m_WarShangShu=0;   //����
	m_WarMiShu=0;      //����
	m_WarTaiLe=0;      //̫��
	*/
	m_WarCityGX=0;     //���˹���
	m_ReviceNum=0;
//    m_WarIsGong=-1;      //���ڹ���
//    m_WarIsShou=-1;      //�����ط�
    m_RestNameCount=0;     //�����Ĵ���
    m_CJtaskID=0;        //172�齱ר��
	//m_IsWaiGua=0;        //�Ƿ�ʹ�����
	m_GameliveTime = -1; //NPC����ʱ��
	m_liveType = 0;       //ʱ������
    m_TempliveTime = 0;
   /*
    m_GuziYajing=0;      //Ѻ��
    m_GuziDianshu=0;     //����
    m_GuziZhuang=0;      //�Ƿ�ׯ��
	m_GuziMenber=0;
	m_IsShuai=0;         //�Ƿ��Ѿ�˦��
	m_ZuoWeihao=0;       //��λ��
	*/
	//IsDeath=0;           //�Ƿ�����
	IsLuKey=1;
	IsJinYan=0;
	m_GoldLucky=1;
	IsJinQian=1;
	//m_IsTuiGuang=0;
    m_IsVip=0;                  //��Ա
    m_IsXingYunXing=0;

	m_IsgetSkill=0;             //�Ƿ�ʰȡ����״̬
    m_mMapX=0;
    m_mMapY=0;
	IsExeGoldScript=1;          //Ĭ��ִ��ȫ�ֽű�
	IsCreatBoss=1;              //�Ƿ񴥷�BOSS
    //t_sprintf(m_ItmeInfo,"��������");
	m_nIsOver  = FALSE;
	m_bLockNpcDwID      = 0;
////////////����////////////////
	m_BtnFlag=0;                //�Ƿ����״̬
	m_BtnFindPath=FALSE;
	/*KIniFile nClient;
	if (nClient.Load("//config.ini"))
	{
		nClient.GetInteger("Client","Represent",3,&m_ISrepresent2);
	}
	nClient.Clear();*/
	//m_ISrepresent2=2;

    m_bIsHideNpc        = FALSE;
    m_bIsHidePlayer     = FALSE;
    m_bIsHideMissle     = FALSE;
	m_bIsHideLife       = FALSE;
	m_bIsHideTong		= TRUE;
	m_bIsHideNuqi	    = FALSE;



//	ZeroMemory(m_PicPath,sizeof(m_PicPath));
//	ZeroMemory(m_ScriptPicPath,sizeof(m_ScriptPicPath));	
	m_nChatContentLen = 0;
	m_nCurChatTime    = 0;
	m_nChatNumLine    = 0;
	m_nChatFontWidth  = 0;
	m_nStature        = 0;
	ZeroMemory(TongName,sizeof(TongName)); //��հ������
	m_nFigure = -1;
	//m_IsHaveAttack=0;  //�Ƿ�����Ϊ������Ч��
	//m_AttackerDwid=0;  //�ϴι����ŵ�DWID	

    m_IsHaveAttack=0;  //�Ƿ�����Ϊ������Ч��
	m_AttackerDwid=0;  //�ϴι����ŵ�DWID
	m_CurrentLife    = 100;		// Npc�ĵ�ǰ����
	m_CurrentLifeMax = 100;		// Npc�ĵ�ǰ�������ֵ
	m_CurrentLifeReplenish = 0;	// Npc�ĵ�ǰ�����ظ��ٶ�
	m_CurrentLifeReplenish_p = 0;
    m_CurrentLifeDamage = 0;
	m_CurPoisonDamage   = 0;
	m_CurFireDamage     = 0;
	m_CurrentNuQi = 0;		    // Npc�ĵ�ǰŭ��
	m_CurrentNuQiMax = 100;		// Npc�ĵ�ǰŭ�����ֵ
	m_CurrentNuQiReplenish = 1;	// Npc�ĵ�ǰŭ���ظ��ٶ�
    
//	m_CurrentFuMoVal    =0;		        // Npc�ĵ�ǰ��ħֵ
//	m_CurrentFuMoValMax =500;
//	m_CurrentFuMoValReplenish =1;	    // Npc�ĵ�ǰ��ħֵ�ظ��ٶ�
    //m_FuMoTimesVal      =5;		    // Npc�ĵ�ǰ��ħ����ʱ��ֵ�����ֵ��
	//m_AttackState=FALSE;
	m_CurrentMana    = 100;		// Npc�ĵ�ǰ����
	m_CurrentManaMax = 100;		// Npc�ĵ�ǰ�������
	m_CurrentManaReplenish = 0;	// Npc�ĵ�ǰ�����ظ��ٶ�
	m_CurrentManaReplenish_p = 0;
	m_CurrentStamina = 100;		// Npc�ĵ�ǰ����
	m_CurrentStaminaMax = 100;	// Npc�ĵ�ǰ�������
	m_CurrentStaminaGain = 0;	// Npc�ĵ�ǰ�����ظ��ٶ�
	m_CurrentStaminaLoss = 0;	// Npc�ĵ�ǰ�����½��ٶ�
	m_CurrentAttackRating = 100;// Npc�ĵ�ǰ�����ʵ�
    m_CurrentSkillMingZhong  = 0;
	m_CurrentDefend = 10;		// Npc�ĵ�ǰ����
	m_CurrentWalkSpeed = 5;		// Npc�ĵ�ǰ�߶��ٶ�
	m_CurrentRunSpeed = 10;		// Npc�ĵ�ǰ�ܶ��ٶ�
	m_CurrentJumpSpeed = 12;	// Npc�ĵ�ǰ��Ծ�ٶ�
	m_CurrentJumpFrame = 40;	// Npc�ĵ�ǰ��Ծʱ��
	m_CurrentAttackSpeed = 0;	// Npc�ĵ�ǰ�����ٶ�
	m_CurrentCastSpeed = 0;		// Npc�ĵ�ǰʩ���ٶ�
	m_CurrentVisionRadius = 40;	// Npc�ĵ�ǰ��Ұ��Χ
	m_CurrentAttackRadius = 30;	// Npc�ĵ�ǰ������Χ
	m_CurrentHitRecover = 0;	// Npc�����˶���ʱ��
	m_CurrentHitNpcRecover=0;
	m_CurrentHitRank=0;
    m_CurrentStunRank_p=0;          // ѣ�μ��ʼ���
	m_CurrentFireResistMax=75;		// Npc�ĵ�ǰ������
	m_CurrentColdResistMax=75;		// Npc�ĵ�ǰ��������
	m_CurrentPoisonResistMax=75;	// Npc�ĵ�ǰ��󶾿���
	m_CurrentLightResistMax=75;	    // Npc�ĵ�ǰ���翹��
	m_CurrentPhysicsResistMax=75;	// Npc�ĵ�ǰ���������

	m_CurrentAddPhysicsDamage = 0;	// Npc�ĵ�ǰ�����˺�ֱ�Ӽӵĵ���

    m_CurrentAddPhysicsDamageP= 0;     // ��ǰ�������հٷֱ�
    m_CurrentAddFireDamagev= 0;        // ��ǰ��������
    m_CurrentAddColdDamagev= 0;        // ��ǰ��������� 
    m_CurrentAddLighDamagev= 0;        // ��ǰ�������׵�
    m_CurrentAddPoisonDamagev= 0;      // ��ǰ�����ⶾ��
	
    m_CurrentAddmagicphysicsDamage= 0; // ��ǰ�������յ�
    m_CurrentAddmagicphysicsDamageP= 0;// ��ǰ�������հٷֱ�
    m_CurrentAddmagicColdDamagicv= 0;  // ��ǰ�����ڱ���
    m_CurrentAddmagicFireDamagicv= 0;  // ��ǰ�����ڻ��  
	m_CurrentAddmagicLightDamagicv= 0; // ��ǰ�������׵�
    m_CurrentAddmagicPoisonDamagicv= 0;// ��ǰ�����ڶ���
	m_Currentbaopoisondmax_p=0;
	m_CurrentPoisondamagereturnV=0;
	m_CurrentPoisondamagereturnP=0;
	m_CurrentReturnskillp=0;
	m_CurrentIgnoreskillp=0;
	m_CurrentReturnresp=0;
    m_CurrentCreatnpcv=0;
	m_CurrentAllJiHuo=0; //�Ƿ�ȫ����
	m_CurrentdanggeRate = 0;  //����
    m_CurrentzhongjiRate = 0; // �ػ�

    m_Me2metaldamage_p= 0;             //=�Խ�ϵ�˺����ӣ�#d1+%
    m_Metal2medamage_p= 0;             //=�������Խ�ϵ���˺���#d1-%
    m_Me2wooddamage_p= 0;             //=��ľϵ�˺����ӣ�#d1+%
    m_Wood2medamage_p= 0;              //=��������ľϵ���˺���#d1-%
    m_Me2waterdamage_p= 0;              //=��ˮϵ�˺����ӣ�#d1+%
    m_Water2medamage_p= 0;              //=��������ˮϵ���˺���#d1-%
    m_Me2firedamage_p= 0;              //=�Ի�ϵ�˺����ӣ�#d1+%
    m_Fire2medamage_p= 0;             //=�������Ի�ϵ���˺���#d1-%
    m_Me2earthdamage_p= 0;             //=����ϵ�˺����ӣ�#d1+%
    m_Earth2medamage_p= 0;             //=����������ϵ���˺���#d1-%

	m_Staticmagicshield_p=0;

	m_CurrentPoisonTime=0;
	m_nCurNpcLucky =100;
    m_CurrentUpExp=0;
	m_Dir = 0;					// Npc�ķ���
	m_JumpStep = 0;
	m_JumpDir = 0;			
	m_MapZ = 0;					// Npc�ĸ߶�
	m_HelmType = 1;				// Npc��ͷ������
	m_ArmorType = 1;			// Npc�Ŀ�������
	m_WeaponType = 1;			// Npc����������
	m_HorseType = -1;			// Npc����������  (Ĭ����û����)
	m_bRideHorse = FALSE;		// Npc�Ƿ�����
	m_MaskType = 0;				// Npc ��߹���
	m_PifengType =0;			// Npc ������
	m_ChiBangType=0;            // �������
	m_MaskMark = 0;				// mat na
//	m_IsFuMo   = 0;
	m_BaiTan = 0;
//	ZeroMemory(ShopName,sizeof(ShopName));
	m_RideState = 0;			// Npc����ʱ������
	ZeroMemory(Name, 32);		// Npc������
	ZeroMemory(_clientName,32);
	m_NpcSettingIdx = 0;		// Npc���趨�ļ�����
	m_CorpseSettingIdx = 0;		// Body���趨�ļ�����
	//ZeroMemory(ActionScript,sizeof(ActionScript)); //�ű�

	nstrNoteInfo.clear();

	m_ActionScriptID = 0;				    
	m_TrapScriptID = 0;
	ZeroMemory(m_TmpAuraID,sizeof(m_TmpAuraID));
	ZeroMemory(m_ExtSkill,sizeof(m_ExtSkill));
	m_btRankId					= 0; //���ֳƺ�
	m_NpcTitle                  = 0;
	m_CurNpcTitle               = 0;
	m_btRankFFId				= 0; //spr�ƺ�
	m_AutoplayId                = 0; //guaji
	m_ExItemId					= 0; // hanh trang
	m_ExBoxId					= 0; // ruong mo rong
	nRankInWorld				= 0; //��������
	nLevelInWorld               = 0; //�ȼ�����
	nRepute						= 0;
	nPKValue					= 0;
	nReBorn						= 0;

	m_LifeMax					= 100;		// Npc���������
	m_LifeReplenish				= 0;		// Npc�������ظ��ٶ�

	m_NuqiMax                   = 100;		// Npcŭ�����ֵ
	m_NuqiReplenish             = 1;	    // Npcŭ���ظ��ٶ�

	m_ManaMax					= 100;		// Npc���������
	m_ManaReplenish				= 0;		// Npc�������ظ��ٶ�
	m_StaminaMax				= 100;		// Npc���������
	m_StaminaGain				= 0;		// Npc�������ظ��ٶ�
	m_StaminaLoss				= 0;		// Npc�������½��ٶ�
	m_AttackRating				= 100;		// Npc�������� ��
	m_Defend					= 10;		// Npc��������
	m_WalkSpeed					= 6;		// Npc�������ٶ�
	m_RunSpeed					= 10;		// Npc���ܶ��ٶ�
	m_JumpSpeed					= 12;		// Npc����Ծ�ٶ�
	m_AttackSpeed				= 0;		// Npc�Ĺ����ٶ�
	m_CastSpeed					= 0;		// Npc��ʩ���ٶ�
	m_VisionRadius				= 40;		// Npc����Ұ��Χ
	m_DialogRadius				= 124;		// Npc�ĶԻ���Χ
	m_HitRecover				= 12;		// Npc�����˶����ָ���ʱ��֡��
	m_nPeopleIdx				= 0;
	m_LoopFrames				= 0;
	m_WalkFrame					= 12;
	m_RunFrame					= 15;
	m_StandFrame				= 15;
	m_DeathFrame				= 15;
	m_HurtFrame					= 10; //���˶���
	m_AttackFrame				= 20; //�⹦�����ٶ�֡��
	m_CastFrame					= 20; //�ڹ������ٶ�֡��
	m_SitFrame					= 15;
	m_JumpFrame					= 40;
	m_AIMAXTime					= 25;
	m_NextAITime				= 0;
	m_ProcessState				= 1;
	m_ReviveFrame				= 1080;  //����ʱ��
	m_bExchangeServer			= FALSE;
	m_bActivateFlag				= FALSE;
	m_FightMode					= 0;
	m_OldFightMode				= 0;
	m_BayBan					= 0;
	m_SyncSignal				= 0;
	m_PiFenLoop                 = 0;
	m_sClientNpcID.m_dwRegionID	= 0;
	m_sClientNpcID.m_nNo		= -1;
	m_ResDir					= 0;
	//uFlipTime                   = 0;
	m_nPKFlag					= 0;
	m_nSleepFlag				= 0;   //˯��״̬
	memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
/*
	for(int i=0;i<50;++i)
	{
		m_nBloodNo[i][0]		= 0;
		m_nBloodNo[i][1]		= 0;
	}

	m_nBloodAlpha			    = 0;
	m_nBloodTime[0]				= 0;
	m_szBloodNo[0]				= 0;
	*/
	//m_nBloodAlpha			    = 0;
	memset(m_nBloodNo, 0, sizeof(m_nBloodNo));
	//memset(m_nBloodAlpha, 0, sizeof(m_nBloodAlpha));
	//memset(m_nBloodTime, 0, sizeof(m_nBloodTime));
	memset(m_szBloodNo, 0, sizeof(m_szBloodNo));
	m_nTongFlag					= 0;
	m_IsbeSel                   = 0;
	m_nLastPoisonDamageIdx = 0;
	m_nLastBurnDamageIdx = 0;
	m_nLastDamageIdx = 0;
	//m_bHaveLoadedFromTemplate = FALSE;
	m_bClientOnly = FALSE;
	isRemoveMenu  = false;
}

ISkill* KNpc::GetActiveSkill()
{
	//CCAssert(m_ActiveSkillID < MAX_SKILL,"");
	if  (m_ActiveSkillID <=0||m_ActiveSkillID >= MAX_SKILL)
		return NULL;

	int nCurLevel = m_SkillList.GetCurrentLevel(m_ActiveSkillID);
	if (nCurLevel > 0)
		return g_SkillManager.GetSkill(m_ActiveSkillID, nCurLevel);
	else 
		return NULL;
};

void KNpc::SetCurrentCamp(int nCamp)
{
	m_CurrentCamp = nCamp;

}

void KNpc::SetCamp(int nCamp)
{
	m_Camp = nCamp;//ʵ����Ӫ
}
//���ûָ���ǰ��ӪΪ ʵ����Ӫ
void KNpc::RestoreCurrentCamp()
{
	m_CurrentCamp = m_Camp;
}

#define		NPC_SHOW_CHAT_TIME		15000
int		IR_IsTimePassed(uint32_t uInterval, uint32_t& uLastTimer);

/*void KNpc::ActivateDrop()
{
#ifdef _SERVER

	   if (strstr(nDropFlie,".ini"))
	   {
		   if (g_pNpcTemplate[m_NpcSettingIdx][0])
	          g_pNpcTemplate[m_NpcSettingIdx][0]->InitDropRate(m_Index,nDropFlie);  //���������޸�	
		   else if (g_pNpcTemplate[m_NpcSettingIdx][m_Level])
		   	     g_pNpcTemplate[m_NpcSettingIdx][m_Level]->InitDropRate(m_Index,nDropFlie);  //���������޸�	   
	   }

#endif
}*/
//ȡ��
/*DWORD KNpc::TakeTrader(DWORD a,DWORD b)
{
	DWORD nRet = 0;
	//DWORD nYuShu=0;
	__asm
	{
		    mov eax,a
			mov ecx,b
			xor edx,edx
			idiv ecx
			mov nRet,eax
			//mov nYuShu,edx
	}
	return nRet;
}
//ȡ����
DWORD KNpc::TakeRemainder(DWORD a,DWORD b)
{
	DWORD nRet = 0;
//	DWORD nYuShu=0;
	__asm
	{
		    mov eax,a
			mov ecx,b
			xor edx,edx
			idiv ecx
			mov nRet,edx
			//mov nYuShu,edx
	}
	return nRet;
}
 */
  


void KNpc::Activate()
{
	// ���������NPC
	if (m_Index<=0)
	{
		return;
	}

	// �л���ͼ�У�������,���
	if (m_bExchangeServer)  
	{
		return;
	}
	
	// Check here
	if (m_bActivateFlag)
	{
		m_bActivateFlag = FALSE;	// restore flag  �Ῠһ��
		return;
	}

/*#ifndef _SERVER
	// ����ͬ������ɾ��Npc��9����32������
	if (!IsPlayer() && 
		(NpcSet.GetMapDisX(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) >= 32 || NpcSet.GetMapDisY(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) >= 32))
	{
		SubWorld[0].m_Region[m_RegionIndex].RemoveNpc(m_Index);
		SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY,obj_npc);
		m_RegionIndex = -1;
		return;
	} 
	
	// lbh_06_06_20��ǿ��ͬ�������������ƶ���δ�յ�����˵ĸ�npcͬ��Э�飬ɾ��Щnpc
	if (!IsPlayer() && SubWorld[0].m_dwCurrentTime - m_SyncSignal > 120)
	{
		SubWorld[0].m_Region[m_RegionIndex].RemoveNpc(m_Index);
		SubWorld[0].m_Region[m_RegionIndex].DecRef(m_MapX, m_MapY,obj_npc);
		m_RegionIndex = -1;
		return;
	}
#endif */

	++m_LoopFrames;

	if (m_ProcessState && m_CurrentLife>0)
	{
		if (ProcessState())      //����״̬�ĳ���ѭ��--�ᵼ��mainloop���󣬲���ԭ��
			return; 
	}

	if (m_ProcessAI)
	{
		NpcAI.Activate(m_Index); //npcAI		
	}
	ProcCommand(m_ProcessAI);    //ʹ�ü��ܵ� 
//------------------------------------�д�
	ProcStatus();                     //����Э��״̬     ----�ᵼ��mainloop���󣬲���ԭ��
//-------------------------------------------------------------------------
	//��߳���״̬
 if (m_Kind == kind_player)
 {
	if (m_MaskType > 0 && m_MaskMark != 0 && m_MaskMark != m_MaskType)// ���
	{//��ֹ����  �ָ�ԭ��
		ReSetRes(1);
		m_MaskMark = 0;
	}
	else if (m_MaskType > 0 && !m_MaskMark)
	{//���ñ��� �������
		ReSetRes(0);
		m_MaskMark = m_MaskType;
	}
	else if (m_MaskType == 0 && m_MaskMark)
	{//�ָ�ԭ�� �������
		ReSetRes(1);
		m_MaskMark = 0;
	} 
	
	if (m_MaskMark >0)//��ȡNPC��ģ��
	{
		//m_MaskMark = 2;
		GetFrameCopyFromTemplate(m_MaskType,m_Level);
	}
 } 
  if (m_Kind == kind_player)
  {
//ʵʱ���»�������
	  Player[CLIENT_PLAYER_INDEX].SetNpcDamageAttrib();	
    //�������ü��ܼӳ�
	//NewSetNpcEnChance();
	ReFullManaSkillEnhance(Player[CLIENT_PLAYER_INDEX].GetLeftSkill(),Player[CLIENT_PLAYER_INDEX].GetLeftSkillListidx());
  }
///------------------------------------
	if (m_RegionIndex == -1)
		return;
//	HurtAutoMove();	  //�˺��ƶ���
	int	nMpsX, nMpsY;
//------------------------------------
//------------------------------------
	m_DataRes.SetAction(m_ClientDoing);  //������Ϊ
	
	m_DataRes.SetRideHorse(m_bRideHorse);//�Ƿ�����
	m_DataRes.SetArmor(m_ArmorType);     //�·�
	m_DataRes.SetHelm(m_HelmType);       //ͷ��
	m_DataRes.SetHorse(m_HorseType);     //��ƥ����
	m_DataRes.SetWeapon(m_WeaponType);   //����
	
   if (m_Kind==kind_player)
   { 
	   m_DataRes.SetPifeng(m_PifengType);   //���� m_PifengType
	   //m_DataRes.SetChiBang(m_ChiBangType);
   } 
	//�ͻ��˴����ܲ�����״̬����Ч
	m_DataRes.SetState(&m_StateSkillList, &g_NpcResList);      //�⻷����״̬

	if (m_CurNpcTitle>0)
	   m_DataRes.SetSprState(m_CurNpcTitle,&g_NpcResList);	   //�Զ����SPR״̬
	else
	   m_DataRes.SetSprState(m_NpcTitle,&g_NpcResList);
	
	if (g_GameWorld)
	{//����������������
	  if (Player[CLIENT_PLAYER_INDEX].m_nIndex == m_Index/* && !Player[CLIENT_PLAYER_INDEX].m_bExchangeServer*/)   //����ǿͻ��˱���     if (m_Kind==kind_player)
	  {//�ͻ��˱���
		SubWorld[0].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
		m_DataRes.SetPos(m_Index, nMpsX, nMpsY, m_Height, TRUE);  //�趨������۵�λ��
	  }
	  else
	  {//����NPC
		SubWorld[0].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
		m_DataRes.SetPos(m_Index, nMpsX, nMpsY, m_Height, FALSE);
	  }
	}
	// client npc ʱ�����������������  ()
	if ( m_Kind == kind_bird || m_Kind == kind_mouse || m_Kind == kind_normal)
		 m_SyncSignal = SubWorld[0].m_dwCurrentTime;   //ͬ����ǰ��ͼʱ��֡��

	if (m_nChatContentLen > 0)
	{
		if (IR_GetCurrentTime() - m_nCurChatTime > NPC_SHOW_CHAT_TIME)
		{
			m_nChatContentLen = 0;
			m_nChatNumLine = 0;
			m_nChatFontWidth = 0;
			m_nCurChatTime = 0;
		}
	}
}

//Э��״̬ m_Doing
void KNpc::ProcStatus()
{
	if (m_bExchangeServer)  //��ת��ͼʱ ������
		return;

	switch(m_Doing)
	{
	case do_stand:
		OnStand(); 
		break;
	case do_run:
		OnRun();    //�е���core mainloop����
		break;
	case do_walk:
		OnWalk();   //�е���core mainloop����
		break;
	case do_attack: //����������ʱʹ�ü���
	case do_magic:  //����������ʱʹ�ü���
		OnSkill();
		break;
	case do_sit:
		OnSit();
		break;
	case do_jump:
		OnJump();
		break;
	case do_hurt:
		OnHurt();
		break;
	case do_revive:
		OnRevive();
		break;
	case do_death:
		OnDeath();
		break;
	case do_defense:  //����
		OnDefense();
		break;
	case do_special1: //�ż���
		OnSpecial1();
		break;
	case do_special2:  //͵ȡ����
		OnSpecial2();
		break;
	case do_special3:
		OnSpecial3();
		break;
	case do_special4:
		OnSpecial4();
		break;
	case do_manyattack:
		OnManyAttack();
		break;
	case do_runattack: //�ϻ��
		OnRunAttack();
		break;
	case do_jumpattack:
		OnJumpAttack();
		break;
	case do_idle:
		OnIdle();
		break;
	case do_stall:
          break;
	case do_movepos:
		break;
	case do_knockback:
		break;
	case do_drag:
		OnDrag();
		break;
	case do_rushattack:
		break;
	case do_runattackmany:
		break;
		  /*
		  
			do_stall,
			do_movepos,		// ˲���ƶ�
			do_knockback,	// ����
			do_drag,		// ��������
			do_rushattack,	// �忳
	do_runattackmany, //��̶��� 
		  */
	default:
		break;
	}
}

void KNpc::ProcCommand(int nAI)  //Э�����
{
	// CmdKind < 0 ��ʾû��ָ��	������ͼҲ������
	if (m_Command.CmdKind == do_none || m_bExchangeServer)
		return;

	if (nAI)
	{
		if (m_RegionIndex < 0)
			return;
		switch (m_Command.CmdKind)
		{
		case do_stand:
			DoStand();
			break;
		case do_walk:
			Goto(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_run:
			RunTo(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_jump:
			JumpTo(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_skill:  //�ͻ��˷������˹���ִ�з�����
		{
			if (int nSkillIdx = m_SkillList.FindSame(m_Command.Param_X))  //�ڼ����б����ҵ�����ID
			{
				if  (SetActiveSkill(nSkillIdx))//����Ϊ��������
				{
				}
				DoSkill(m_Command.Param_Y, m_Command.Param_Z); //�����NPC������ʹ�ü���
			}
			else
			{
			  DoStand();  //վ��
			}
		}
			break;
		case do_sit:
			DoSit();
			break;
		case do_defense:  //����
			DoDefense();
			break;
		case do_idle:     // ����
			DoIdle();
			break;
		case do_hurt: // �˺�
			DoHurt(m_Command.Param_X, m_Command.Param_Y, m_Command.Param_Z);
			break;	
		case do_revive://վ�ţ�
			{
			DoStand();
			m_ProcessAI = 1;
			m_ProcessState = 1;
			this->SetInstantSpr(enumINSTANT_STATE_REVIVE); //�ͷ�һ��������˲����Ч
			}
			break;
		case  do_special4://ת����ͼ
				DoSpecial4(m_Command.Param_X, m_Command.Param_Y);
			break;
		case do_stall:
			break;
		case do_movepos:
			break;
		case do_knockback:
			break;
		case do_drag:
			break;
		case do_rushattack:
			break;
		case do_runattackmany:
			break;

		}
	}
	else
	{
		switch(m_Command.CmdKind)
		{
		case do_hurt:
			if (m_RegionIndex >= 0)
				DoHurt(m_Command.Param_X, m_Command.Param_Y, m_Command.Param_Z);
			break;
		case do_revive:  //����
			{
			DoStand();
			m_ProcessAI = 1;
			m_ProcessState = 1;
			this->SetInstantSpr(enumINSTANT_STATE_REVIVE); //�ͷ�һ��������˲����Ч
			}
			break;
		case do_stall:
			break;
		case do_movepos:
			break;
		case do_knockback:
			break;
		case do_drag:
			break;
		case do_rushattack:
			break;
		case do_runattackmany:
			break;

		default:
			break;
		}
	}
	m_Command.CmdKind = do_none;
}
//����ͬ��״̬����
BOOL KNpc::ProcessState()
{
	int nRet = FALSE;
	if (m_RegionIndex < 0)
		return FALSE;
    if (!(m_LoopFrames % GAME_UPDATE_TIME)) //ÿ����ִ��һ��
	//if (m_LoopFrames - (m_LoopFrames>>4<<4)==0)
	{
////////////////////////////////////////////////////////////
        if (m_Kind==kind_player)
		{
			Player[CLIENT_PLAYER_INDEX].m_ItemList.CheckBianShiItemTime();
		}
//---------------------�⻷����ͬ��----------------------------------------------------
		/*if (m_ActiveAuraID >0 && m_ActiveAuraID < MAX_SKILL)  //��Ҫ���ڼ��ܵ� ��������
		{//ֻ�ܿ�һ���⻷״̬
			//int nListidx = m_SkillList.FindSame(m_ActiveAuraID);
			int nLevel    = m_SkillList.GetCurrentLevelByIdx(m_ActiveAuraIndex);
			int nEnChance = m_SkillList.GetEnChance(m_ActiveAuraIndex);
			if (nLevel > 0 && nLevel < MAX_SKILLLEVEL)
			{
				int nMpsX, nMpsY;
				SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
				//_ASSERT(m_ActiveAuraID < MAX_SKILL && nLevel < MAX_SKILLLEVEL)			
				KSkill * pOrdinSkill1 = (KSkill *) g_SkillManager.GetSkill(m_ActiveAuraID, nLevel);
				int nChildSkillId = 0;
				if (pOrdinSkill1)
				{
					nChildSkillId = pOrdinSkill1->GetChildSkillId(); //�Ӽ���
                    //ִ���Ӽ���
					KSkill * pOrdinSkill2 = (KSkill *) g_SkillManager.GetSkill(nChildSkillId, nLevel);
					//int nEnChance = m_SkillList.GetEnChance(m_SkillList.FindSame(nChildSkillId));
					if (pOrdinSkill2)
                    {
						pOrdinSkill2->m_nEnChance = nEnChance;
						pOrdinSkill2->Cast(m_Index, nMpsX, nMpsY); //ʹ�ù⻷
                    }
				}
			}
		}
//-----------------�����Ǽ���ͬ��--------------------------------------
		if (IsPlayer() && m_ActiveXinYunXingID>0 && m_ActiveXinYunXingID < MAX_SKILL)  //��Ҫ���ڼ��ܵ� ��������
		{//ֻ�ܿ�һ���⻷״̬

			int nLevel =1;// m_SkillList.GetCurrentLevel(m_ActiveXinYunXingID);
			int nEnChance = m_SkillList.GetEnChance(m_SkillList.FindSame(m_ActiveXinYunXingID));
			if (nLevel > 0 && nLevel < MAX_SKILLLEVEL)
			{
				int nMpsX, nMpsY;
				SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
			//	_ASSERT(m_ActiveXinYunXingID < MAX_SKILL && nLevel < MAX_SKILLLEVEL);				
				KSkill * pOrdinSkill1 = (KSkill *) g_SkillManager.GetSkill(m_ActiveXinYunXingID, nLevel);
				int nChildSkillId = 0;
				if (pOrdinSkill1)
				{
					nChildSkillId = pOrdinSkill1->GetChildSkillId(); //�Ӽ���
                    //ִ���Ӽ���
					KSkill * pOrdinSkill2 = (KSkill *) g_SkillManager.GetSkill(nChildSkillId, nLevel);
					//int nEnChance = m_SkillList.GetEnChance(m_SkillList.FindSame(nChildSkillId));
					if (pOrdinSkill2)
                    {
						pOrdinSkill2->m_nEnChance = nEnChance;
						pOrdinSkill2->Cast(m_Index, nMpsX, nMpsY); //ʹ�ù⻷
                    }
				}
			}
		}
//-----------------------------���ܶ�⻷��������ͬ��----------------------------------------------------
		int nMpsX, nMpsY,k;
		SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nMpsX, &nMpsY);
		//-----------------------------------------------------------------------------------------------
		if (m_IsMoreAura && IsPlayer())
		{//����Ƕ�⻷
			if (m_ActiveAuraID >0 && m_ActiveAuraID < MAX_SKILL)  //��Ҫ���ڼ��ܵ� ��������
			{
				//int nListIdx  = m_SkillList.FindSame(m_ActiveAuraID); 
				int nMainLevel = m_SkillList.GetCurrentLevelByIdx(m_ActiveAuraIndex);//ĸ�����⻷����ID

				for (k = 0;k < 5;++k)
				{
					if (m_TmpAuraID[k].skillid>0 && m_TmpAuraID[k].skillid < MAX_SKILL) //�����⻷�ļ���ID
					{//��ü���ܶ�⻷��������
						//int nListIdx     = m_TmpAuraID[k].skilllistIndex;//m_SkillList.FindSame(m_TmpAuraID[k]);    //m_ActiveAuraID
						int nLevel       = m_SkillList.GetCurrentLevelByIdx(m_TmpAuraID[k].skilllistIndex);
						int nEnChance    = m_SkillList.GetEnChance(m_TmpAuraID[k].skilllistIndex);
						nLevel =  (((nLevel)<(nMainLevel))?(nLevel):(nMainLevel));//Min(nLevel,nMainLevel); //ȡ��Сֵ
						if (nLevel > 0 &&  nLevel < MAX_SKILLLEVEL)
						{//s2c_castskilldirectly									
							KSkill * pOrdinSkill1 = (KSkill *) g_SkillManager.GetSkill(m_TmpAuraID[k].skillid, nLevel);
							int nChildSkillId = 0;
							if (pOrdinSkill1)
							{
								nChildSkillId = pOrdinSkill1->GetChildSkillId();

								KSkill * pOrdinSkill2 = (KSkill *) g_SkillManager.GetSkill(nChildSkillId, nLevel);
								//int nEnChance = m_SkillList.GetEnChance(m_SkillList.FindSame(nChildSkillId));
								if (pOrdinSkill2)
								{
									pOrdinSkill2->m_nEnChance = nEnChance;
									pOrdinSkill2->Cast(m_Index, nMpsX, nMpsY);
								}
							}
						}
					}//end if
				} //end for
			}
		}*/
///----------------------------------���⼼��ͬ�����-------------------------------------------------------
	}
	if (!(m_LoopFrames%GAME_UPDATE_TIME))
	//if (m_LoopFrames-(m_LoopFrames>>3<<3)==0)
	{
	    m_CurrentNuQi += m_CurrentNuQiReplenish;
	  if (m_CurrentNuQi > m_CurrentNuQiMax)
		 m_CurrentNuQi = m_CurrentNuQiMax;		        // ŭ��������Ȼ�ظ�
	}

	if (!(m_LoopFrames%540))
	//if  (m_LoopFrames-18*30*(m_LoopFrames/(18*30))==0)
	{
	    m_IsHaveAttack=0;  //�Ƿ�����Ϊ������Ч��
	    m_AttackerDwid=0;  //�ϴι����ŵ�DWID
	}

/*
	int					m_CurrentFuMoVal;		// Npc�ĵ�ǰ��ħֵ
	int					m_CurrentFuMoValReplenish;	// Npc�ĵ�ǰ��ħֵ�ظ��ٶ�
    int	                m_FuMoTimesVal;		    // Npc�ĵ�ǰ��ħ����ʱ��ֵ�����ֵ��
	int					m_CurrentFuMoValMax;    // Npc�ĵ�ǰ��ħ���ֵ
*/

	bool bAdjustColorId = false;

	if (m_FreezeState.nTime > 0)
	{//����
		//m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_freeze); //
		m_AdjustColorKind = kind_color_freeze;
		bAdjustColorId = true;

		if (SubWorld[0].m_dwCurrentTime & 1)
			nRet = TRUE;
	}
	
	if (m_StunState.nTime > 0)
	{//ѣ��
		//m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_stun);
		m_AdjustColorKind = kind_color_stun;
		bAdjustColorId = true;
		nRet = TRUE;
	}

	//if (m_PoisonState.nTime > 0)
	if (m_PoisonState.nTime>0)
	{//��
	//	m_PoisonState.nTime--;
		//m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_poison);
		m_AdjustColorKind = kind_color_poison;
		bAdjustColorId = true;
	}
	// ŭ��״̬(ս��ģʽ����ɫ)
/*	if (m_CurrentNuQi >= m_CurrentNuQiMax && m_FightMode==1)
	{//ŭȥ
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_red);
		bAdjustColorId = true;
	}
	*/
	//��ץ��״̬��ɫ
    /*if (m_ZhuaState.nTime > 0)
	{
		m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_confuse);
		//nRet = TRUE;
		bAdjustColorId = true;
	}*/ 
	// ȼ��״̬
	if (m_BurnState.nTime > 0)
	{
	//	m_BurnState.nTime--;
		//m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_burn);
		m_AdjustColorKind = kind_color_burn;
		bAdjustColorId = true;
	}
	// ����״̬
	if (m_ConfuseState.nTime > 0)
	{
	//	m_ConfuseState.nTime--;
		//m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_confuse);
		m_AdjustColorKind = kind_color_confuse;
		bAdjustColorId = true;
	}

	if (!bAdjustColorId)
		m_AdjustColorKind = kind_color_physics;
		//m_DataRes.SetAdjustColorId(KNpcRes::adjustcolor_physics);
//״̬���Ƴ� �� ͬ��=========================================
	KStateNode* pNode=NULL;
	            pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
		KStateNode* pTempNode = pNode;
        pNode = (KStateNode *)pNode->GetNext();
		if (pTempNode->m_LeftTime <= -1)	
		{//�������� ��ʱ�����ƵĲ��ᱻ�����
			continue;
		}
	
		if (pTempNode->m_LeftTime == 0 && pTempNode->m_IsClientState==0)    //�ڼ���״̬ʱ�� ��������
		{//�Ƴ�����״̬ ��ʱ�����Ƶ� �Լ���״̬
			int i;
			for (i = 0; i < MAX_SKILL_STATE; ++i)
			{//���¼��㼼��״̬���Ը���ֵ
				if (pTempNode->m_State[i].nAttribType) //����״̬�еĵڼ�������
				{//�Ƴ�����ֵ
					KMagicAttrib nMagicAttrib;   
					nMagicAttrib.nAttribType = pTempNode->m_State[i].nAttribType;
					nMagicAttrib.nValue[0]   = pTempNode->m_State[i].nValue[0];
					nMagicAttrib.nValue[1]   = pTempNode->m_State[i].nValue[1];
					nMagicAttrib.nValue[2]   = pTempNode->m_State[i].nValue[2];
                    ModifyAttrib(m_Index, &nMagicAttrib);
					/*
					char msg[64];		
					#ifdef _SERVER	 
					   t_sprintf(msg,"S(%d)�Ƴ�����:%d,ֵ��Min:%d,max:%d",i,pTempNode->m_State[i].nAttribType,pTempNode->m_State[i].nValue[0],pTempNode->m_State[i].nValue[2]);
					   Player[m_nPlayerIdx].m_ItemList.msgshow(msg);
                    #else
					   //ModifyAttrib(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Index, &nMagicAttrib);
					   t_sprintf(msg,"<color=yellow>C(%d)�Ƴ�����:%d,ֵ��Min:%d,max:%d",i,pTempNode->m_State[i].nAttribType,pTempNode->m_State[i].nValue[0],pTempNode->m_State[i].nValue[2]);
                       Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(msg);
                    #endif 
					*/
				}
			}
			//ReCalcStateEffect();
			//_ASSERT(pTempNode != NULL);
			if (pTempNode != NULL)
			{//״̬���Ƴ� ���￪ʼ�Ƴ� ����
			   int nIdx = pTempNode->m_StateGraphics;
			   pTempNode->Remove();
			   delete pTempNode;
			   pTempNode = NULL;
			   if (g_GameWorld)//ɾ��״̬����
				   g_GameWorld->removespriteByIdx(m_Index,nIdx);
			}
			continue;
		}
		else
		{//ʱ�����
			pTempNode->m_LeftTime --;
			//��ͼ��
			//======
		}
	}
///===========================================================
	return nRet;

}

int KNpc::GetSkillLeftTime(int nSkillId)
{
    KStateNode* pNode;
	    pNode = (KStateNode *)m_StateSkillList.GetHead(); //��һ���ڵ�� ����
	//KStateNode *mNode =(KStateNode *)m_StateSkillList.GetStatusNode(5);  //��5���ڵ�ļ���
	int nMunTime=-1;

	while(pNode)
	{
		if (pNode->m_SkillID == nSkillId)
		{//ID��ͬ ������������
			nMunTime= pNode->m_LeftTime;    
			break;
		}
		pNode = (KStateNode *)pNode->GetNext();
	}

    return nMunTime;
}


int KNpc::SetSkillLeftTime(int nSkillId,int nTime)
{
    KStateNode* pNode;
	pNode = (KStateNode *)m_StateSkillList.GetHead(); //��һ���ڵ�� ����
	//KStateNode *mNode =(KStateNode *)m_StateSkillList.GetStatusNode(5);  //��5���ڵ�ļ���
	int nReut=0;
	while(pNode)
	{
		if (pNode->m_SkillID == nSkillId)
		{//ID��ͬ
			nReut =pNode->m_LeftTime;
			pNode->m_LeftTime=nTime;
			//----------------------
			break;
		}
		pNode = (KStateNode *)pNode->GetNext();
	}
	return nReut;

}


void KNpc::ClearOneSkillState(int nSkillId)
{
  return;
}

//������� Npc������
void KNpc::DoDeath(int nMode,int nLastDamageIdx)
{
//	_ASSERT(m_Doing != do_death);
	// do_death == 10
	//g_DebugLog("[DEATH] m_Doing: %d", m_Doing);
	//CCAssert(m_RegionIndex >= 0,"");
	//_ASSERT
	if (m_RegionIndex < 0 || m_Doing == do_death)
		return;
//��������
	if (IsPlayer() && !m_FightMode)	// �����ڲ�����������Ч��
	{
		m_CurrentLife = 1;  
		return;
	}

	if (IsPlayer())//����Ч��
	{
		Player[m_nPlayerIdx].m_ItemList.SetMaskLock(FALSE);// ������
		int nIdx = Player[m_nPlayerIdx].m_ItemList.GetEquipment(itempart_mask); //
		m_MaskType = Item[nIdx].GetBaseMagic();
	}
	m_IsbeSel  = 0;
	//m_DataRes.SetBlur(FALSE);	 //�����Ӱ

	if (this->m_Kind == kind_normal || this->m_Kind == kind_partner)  //���ж�ðѪ����
		this->SetBlood(this->m_CurrentLife);  //ðѪ������ʾΪ��ǰ����this->m_CurrentLife

//Ҫ�ȷ��侭�����
	m_bLockNpcDwID       = 0;
	m_Doing              = do_death;      //ָ����� ondeath ����
	m_ProcessAI	         = 0;
	m_ProcessState       = 0;
	m_Frames.nTotalFrame = m_DeathFrame;  //����������֡��	
	m_Height = 0;                         //�߶�Ϊ��

	m_ClientDoing = cdo_death;

	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx == m_Index)
	{
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = 0;
	}

	this->m_cGold.ClientClearState();  //ɾ���ƽ��״̬

	if (IsPlayer())
	{// �Զ����
		if (Player[m_nPlayerIdx].m_cTeam.m_nFlag)
		{
			Player[m_nPlayerIdx].LeaveTeam();
		}
	}

	//CoreDataChanged(GDCNI_OPEN_JINDUTIAO,0,0);  //�����رս�����
}

//��� NPC�����ű�  ����ѭ������״̬
//int                 IsExeGoldScript;          //�Ƿ�ִ��ȫ�������ű�
//int                 IsCreatBoss;
void KNpc::OnDeath()
{
	if (WaitForFrame())
	{
//		printf("[DEATH] ����(%s) ����BOSS(%d),ִ��ȫ�ֽű�(%d) TRUE\n",Name,IsCreatBoss,IsExeGoldScript);
		m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;		// ��֤�������ػص�һ֡�����m_Frames.nTotalFrame - 1
		int		nTempX, nTempY,nObjIndex;
		KCObjItemInfo	sInfo;
		SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &nTempX, &nTempY);
		sInfo.m_nItemID = 0;
		sInfo.m_nItemWidth = 0;
		sInfo.m_nItemHeight = 0;
		sInfo.m_nMoneyNum = 0;
		sInfo.m_nColorID = 0;
		sInfo.m_nMovieFlag = 0;
		sInfo.m_nSoundFlag = 0;
		sInfo.m_szName[0] = 0;
		sInfo.m_cHaveAttack=0;
		sInfo.m_Genre=-1;
		sInfo.m_DetailType=-1;
		sInfo.m_ParticularType=-1;
		sInfo.m_GoldId=0;
		sInfo.m_ItemLevel=0;
		sInfo.m_StackNum=0;

		//sInfo.m_cAttackerDwid=0;
		nObjIndex=ObjSet.ClientAdd(0, m_CorpseSettingIdx, 0, m_Dir, 0, nTempX, nTempY, sInfo);  //����ʬ��
        m_ProcessAI	= 0;
		// ������
		if (m_Kind != kind_partner)//ս��Npcʱ
		{//��������
			if (this->m_cGold.GetGoldType() <= 4) //���ǻƽ����͸��� 1-4 5-11 12-16
			{
				DoRevive();  //��ʼ����	  �Ѿ�ɾ���˽ڵ� ���������б�
			}
			// �ͻ��˰�NPCɾ��
			if (m_Kind!= kind_player)
			{
			   if (nObjIndex >0 && nObjIndex<MAX_OBJECT)
			   {//ɾ��ʬ��
				  //SubWorld[0].m_WorldMessage.NewSend(GWM_OBJ_DEL,nObjIndex);
				   if (Object[nObjIndex].m_nSubWorldID>=0 && Object[nObjIndex].m_nRegionIdx >= 0)
					   SubWorld[Object[nObjIndex].m_nSubWorldID].m_Region[Object[nObjIndex].m_nRegionIdx].RemoveObj(nObjIndex);
				   
		           ObjSet.Remove(nObjIndex);
			   }

			   if (m_Index>0  && m_Index<MAX_NPC)
			   {
				   if (Npc[m_Index].m_SubWorldIndex>=0 && Npc[m_Index].m_RegionIndex >= 0)
				   {				
					   int nSubWorld = Npc[m_Index].m_SubWorldIndex;
					   int nRegion = Npc[m_Index].m_RegionIndex;

					   SubWorld[nSubWorld].m_Region[nRegion].RemoveNpc(m_Index);
					   SubWorld[nSubWorld].m_Region[nRegion].DecNpcRef(Npc[m_Index].m_MapX, Npc[m_Index].m_MapY);
				   }
		            NpcSet.Remove(m_Index);
			   } 
			  return;
			}			
		}
		else
		{//ͬ����ֱ�Ӳ�������
			//Remove();
		}
	}
	else
	{
//		printf("[DEATH] ����֡������WaitForFrame FALSE\n");
	}
}

void KNpc::DoDefense() //����
{
	m_ProcessAI = 0;
}

void KNpc::OnDefense()  //����
{

}

void KNpc::DoIdle()
{
	if (m_Doing == do_idle)
		return;
	m_Doing = do_idle;
}

void KNpc::OnIdle()
{

}

VOID KNpc::DoDrag(INT nDragFrame, INT nDesX, INT nDesY)
{
	if (m_RegionIndex < 0 || m_Doing == do_death)
		return;
	if (m_Doing == do_drag)
		return;
	
	INT nMyX, nMyY,nMap;
	GetMpsPos(&nMyX, &nMyY,&nMap);
	if (nDesX != nMyX || nDesY != nMyY)
		m_Dir	= g_GetDirIndex(nDesX, nDesY, nMyX, nMyY);
	
	m_Doing = do_drag;
	
	//ClearProcessAI();
	m_ProcessAI = 0;
	//m_ProcessState = 0;
	
	m_ClientDoing = cdo_hurt;
	
	m_Frames.SetFrame(nDragFrame);
	
	m_DesX	= nDesX;
	m_DesY	= nDesY;
}

//��������
VOID KNpc::OnDrag()
{
	if (m_RegionIndex < 0)
		return;
	
	INT nX, nY,nMap;
	GetMpsPos(&nX, &nY,&nMap);
	
	INT nRestSteps	= m_Frames.nTotalFrame - m_Frames.nCurrentFrame;
	if (nRestSteps <= 0)
		nRestSteps	= 1;
	
	INT nMoveX	= ((m_DesX - nX) << 10) / nRestSteps;
	INT nMoveY  = ((m_DesY - nY) << 10) / nRestSteps;
	
	MovePos(nMoveX, nMoveY);
	
	if (WaitForFrame())
	{
		DoStand();
		//SetProcessAI();
		m_ProcessAI = 1;
	}
}

// -------------------------------------------------------------------------
// ����		: KNpc::MovePos
// ����		: �ƶ�һ��ƫ����
// ����ֵ	: VOID 
// ����		: INT nMoveX, nMoveY	Npc�ڸ����е�ƫ�����꣨�Ŵ���1024����
// ����		: FanZai
// ��ע		: ����ServeMove��ServeJump���ϲ��ظ�����
// -------------------------------------------------------------------------
#define CORRECT_SYNC_RANGE 30 

VOID KNpc::MovePos(INT nMoveX, INT nMoveY)
{
	INT nOldRegion = m_RegionIndex;
	INT nOldMapX = m_MapX;
	INT nOldMapY = m_MapY;
	INT nOldOffX = m_OffX;
	INT nOldOffY = m_OffY;

	m_OffX += nMoveX;
	m_OffY += nMoveY;

	if (!m_bClientOnly)
		CURREGION.DecNpcRef(m_MapX, m_MapY);

	INT nCellWidth	= (SubWorld[m_SubWorldIndex].m_nCellWidth << 10);
	INT nCellHeight	= (SubWorld[m_SubWorldIndex].m_nCellHeight << 10);

	if (nCellWidth <= 0 || nCellHeight <= 0)
	{
		//_ASSERT(FALSE);
		return;
	}

	while (m_OffX < 0)
	{
		m_MapX--;
		m_OffX += nCellWidth;
	}
	while (m_OffX >= nCellWidth)
	{
		m_MapX++;
		m_OffX -= nCellWidth;
	}

	while (m_OffY < 0)
	{
		m_MapY--;
		m_OffY += nCellHeight;
	}
	while (m_OffY >= nCellHeight)
	{
		m_MapY ++;
		m_OffY -= nCellHeight;
	}
	
	if (m_MapX < 0)
	{
		m_RegionIndex = LEFTREGIONIDX;
		m_MapX += REGIONWIDTH;
	}
	else if (m_MapX >= REGIONWIDTH)
	{
		m_RegionIndex = RIGHTREGIONIDX;
		m_MapX -= REGIONWIDTH;
	}
	
	if (m_RegionIndex >= 0)
	{
		if (m_MapY < 0)
		{
			m_RegionIndex = UPREGIONIDX;
			m_MapY += REGIONHEIGHT;
		}
		else if (m_MapY >= REGIONHEIGHT)
		{
			m_RegionIndex = DOWNREGIONIDX;
			m_MapY -= REGIONHEIGHT;
		}
		if (!m_bClientOnly && m_RegionIndex >= 0)
			CURREGION.AddNpcRef(m_MapX,m_MapY);
			//CURREGION.AddRef(m_MapX, m_MapY,obj_npc);
	}

	if (m_RegionIndex < 0)	// �������ƶ���-1 Region�������������������ָ�ԭ����
	{
		SubWorld[0].m_Region[nOldRegion].RemoveNpc(m_Index);
		m_RegionIndex = -1;
		return;
	}
	if  (m_OffX>=nCellWidth || m_OffY>=nCellHeight)
		return;
	//CCAssert(m_OffX < nCellWidth && m_OffY < nCellHeight,"");
	//�쵽ͬ����Եʱͬ��һ������
	if (!IsPlayer() && (NpcSet.GetMapDisX(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) >= CORRECT_SYNC_RANGE ||
		NpcSet.GetMapDisY(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) >= CORRECT_SYNC_RANGE ))
	{
//		ForceSyncPos(TRUE);
	}

	if (nOldRegion != m_RegionIndex)
	{
		SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
		m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
	}
}


//�����˶���
void KNpc::DoHurt(int nHurtFrames, int nX, int nY,int nRank)
{
	//_ASSERT(m_RegionIndex >= 0);
	//m_DataRes.SetBlur(FALSE);
	if (m_RegionIndex < 0)
		return;
	if (m_Doing == do_hurt || m_Doing == do_death)
		return;

	// Npc�����˶���ʱ���Ѿ��ﵽ100%�ˣ��������˶���
	m_Doing = do_hurt;
	//m_ProcessAI	= 0;  //ȡ�������������߶�
	m_ClientDoing = cdo_hurt;
	m_Frames.nTotalFrame = nHurtFrames; //����֡��
	m_nHurtDesX = nX;
	m_nHurtDesY = nY;
	if (m_Height > 0)
	{
		// ��ʱ��¼������Ϊ�߶ȱ仯����OnHurt��ʹ��
		m_nHurtHeight = m_Height;
	}
	else
	{
		m_nHurtHeight = 0;
	}

	CoreDataChanged(GDCNI_OPEN_JINDUTIAO,0,0); //ȡ��������
	if (m_Frames.nTotalFrame == 0)
		m_Frames.nTotalFrame = 1;

	    m_Frames.nCurrentFrame = 0;
}

void KNpc::OnHurt()
{
	if (m_RegionIndex < 0 || m_Frames.nTotalFrame<=0)
	{
		return;
	}
	int nX, nY,nMap;
	GetMpsPos(&nX, &nY,&nMap);

	//CCAssert(m_Frames.nTotalFrame>0,"");
	if (m_Frames.nTotalFrame<=0)
		return;

	m_Height = m_nHurtHeight * (m_Frames.nTotalFrame - m_Frames.nCurrentFrame - 1) / m_Frames.nTotalFrame;
	nX = nX + (m_nHurtDesX - nX) * m_Frames.nCurrentFrame / m_Frames.nTotalFrame;
	nY = nY + (m_nHurtDesY - nY) * m_Frames.nCurrentFrame / m_Frames.nTotalFrame;

	int nOldRegion = m_RegionIndex;
	//SetPos(nX, nY);
	CURREGION.DecNpcRef(m_MapX, m_MapY);

	int nRegion, nMapX, nMapY, nOffX, nOffY;
	nRegion = -1;
	nMapX = nMapY = nOffX = nOffY = 0;
	SubWorld[m_SubWorldIndex].Mps2Map(nX, nY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	
	if (nRegion == -1)
	{
		SubWorld[0].m_Region[nOldRegion].RemoveNpc(m_Index);
		m_dwRegionID = 0;
	}
	else if (nOldRegion != nRegion || nRegion < 0)
	{
		m_RegionIndex = nRegion;
		m_MapX = nMapX;
		m_MapY = nMapY;
		m_OffX = nOffX;
		m_OffY = nOffY;
		SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
		m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
	}
	if (nRegion >= 0)
		CURREGION.AddNpcRef(m_MapX,m_MapY);
		//CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
	//SubWorld[m_SubWorldIndex].m_Region[nRegion].AddNpcRef(m_MapX,m_MapY);
	if (WaitForFrame())   //�ȴ���֡��
	{
	//	g_DebugLog("[DEATH]On Hurt Finished");                 //�ܵ��˺��Ķ���
	   if (m_Kind==kind_player)
	   {
           Player[CLIENT_PLAYER_INDEX].UpdataCurData();
	   }
		DoStand();
		m_ProcessAI = 1; //������˶���
	}
}

void KNpc::DoSpecial1()
{
	 DoBlurAttack();
}

void KNpc::OnSpecial1()
{
	if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
	{
		//m_DataRes.SetBlur(FALSE);
		DoStand();
		m_ProcessAI = 1;	
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
		KSkill * pSkill = (KSkill*)GetActiveSkill();
		if (pSkill)
		{
			int nEnChance = m_SkillList.GetEnChance(m_ActiveSkListIndex);//m_SkillList.FindSame(pSkill->GetSkillId())
			pSkill->m_nEnChance = nEnChance;
			int nChildSkill = pSkill->GetChildSkillId();

			int nChildSkillLevel = pSkill->m_ulLevel;
			
			if (nChildSkill > 0)
			{
				KSkill * pChildSkill = (KSkill*)g_SkillManager.GetSkill(nChildSkill, nChildSkillLevel);
				if (pChildSkill)
				{
					pChildSkill->m_nEnChance = nEnChance;
					pChildSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
				}
			}
		}

        if (m_Kind==kind_player)
            NpcFuMoCastSkll(m_Index, m_SkillParam1, m_SkillParam2);

		if (m_Frames.nTotalFrame == 0)
		{
			m_ProcessAI = 1;
		}
	}
}

void KNpc::DoSpecial2()
{
}

void KNpc::OnSpecial2()
{

	if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
	{
		//m_DataRes.SetBlur(FALSE);
		DoStand();
		m_ProcessAI = 1;	
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
		ISkill * pSkill = GetActiveSkill();
		eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
		switch(eStyle)
		{
		case SKILL_SS_Thief:
			{
				((KThiefSkill*)pSkill)->OnSkill(this);  //͵ȡ����
			}
			break;
		} 
		
		if (m_Frames.nTotalFrame == 0)
		{
			m_ProcessAI = 1;
		}
	}
	
}

void KNpc::DoSpecial3()
{
}

void KNpc::OnSpecial3()
{
}
//ת������ ת����ͼ
void KNpc::DoSpecial4(int nX, int nY)
{
	WaitForFrame();
	if (m_RegionIndex < 0 || m_RegionIndex >= 9)
	{
		DoStand();
		return;
	}
	int nOldRegion = m_RegionIndex;
	int nOldMapX = m_MapX;
	int nOldMapY = m_MapY;
	int nOldOffX = m_OffX;
	int nOldOffY = m_OffY;
	
	//	����NPC��������
	//	CELLWIDTH��CELLHEIGHT��OffX��OffY���ǷŴ���1024��
	
	if (!m_bClientOnly)
		CURREGION.DecNpcRef(m_MapX, m_MapY);
	
	SubWorld[m_SubWorldIndex].Mps2Map(nX,nY,&m_RegionIndex,&m_MapX,&m_MapY,&m_OffX,&m_OffY);
	
	if (!m_bClientOnly && m_RegionIndex >= 0)
		CURREGION.AddNpcRef(m_MapX,m_MapY);
		//CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
	
	
	if (m_RegionIndex == -1)	// �������ƶ���-1 Region�������������������ָ�ԭ����
	{
		m_RegionIndex = nOldRegion;
		m_MapX = nOldMapX;
		m_MapY = nOldMapY;
		m_OffX = nOldOffX;
		m_OffY = nOldOffY;
		//CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
		CURREGION.AddNpcRef(m_MapX,m_MapY);
		return;
	}
	
	if (nOldRegion != m_RegionIndex)
	{
		SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
		m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
	}
	DoStand();
}



void KNpc::OnSpecial4()
{

}

void KNpc::DoStand()
{
	
	m_Frames.nTotalFrame = m_StandFrame;
	if (m_Doing == do_stand)
	{
		m_bLockNpcDwID=0;
		return;
	}
	else
	{
		m_Doing = do_stand;
		m_bLockNpcDwID=0;
		m_Frames.nCurrentFrame = 0;
		int  m_Dmap=0;
		GetMpsPos(&m_DesX, &m_DesY,&m_Dmap);
		if (m_FightMode)
			m_ClientDoing = cdo_fightstand;
		else if (g_Random(6) != 1)
		{
			m_ClientDoing = cdo_stand;
		}
		else
		{
			m_ClientDoing = cdo_stand1;
		}
		//m_DataRes.StopSound();
	}
}


void KNpc::OnStand()
{
	if (WaitForFrame())
	{
		if (m_FightMode)
		{
			m_ClientDoing = cdo_fightstand;
		}
		else if (g_Random(6) != 1)
		{
			m_ClientDoing = cdo_stand;
		}
		else
		{
			m_ClientDoing = cdo_stand1;
		}
	}
}
//��� ���� ��������������	return m_Kind == kind_player;//������  ���return m_Index == Player[CLIENT_PLAYER_INDEX].m_nIndex;
//�ͻ��˽�����ʾ
	
void KNpc::DoRevive() //NPC����
{
	if (m_RegionIndex < 0)
		return;

	if (m_Doing == do_revive)
	{
		m_bLockNpcDwID=0;
		return;
	}
	else
	{
		m_bLockNpcDwID=0;
		m_Doing = do_revive;
		m_ProcessAI = 0;
		m_ProcessState = 0;
		ClearStateSkillEffect();  //ɾ��״̬Ч��
		ClearNormalState();       //�������״̬
		if (IsPlayer())  //��������
		{
			KSystemMessage MsgA;
			MsgA.byConfirmType = SMCT_UI_RENASCENCE; //������Ϣ ѡ������ ��ʽ	
			MsgA.byParamSize = 0;
			MsgA.byPriority =255;
			MsgA.eType = SMT_PLAYER;
			//t_sprintf(MsgA.szMessage, MSG_NPC_DEATH, Name); //��������
			t_sprintf(MsgA.szMessage, strCoreInfo[MSG_NPC_DEATH].c_str(), Name); //��������
			Player[CLIENT_PLAYER_INDEX].m_cPK.ApplySetNormalPKState(0);
			MsgA.nMsgLen = TEncodeText_(MsgA.szMessage, strlen(MsgA.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&MsgA, NULL);
		}	
		m_Frames.nTotalFrame = m_DeathFrame;  //����������֡��
		m_ClientDoing = cdo_death;            //�ͻ�����Ϊ	
	}
}

void KNpc::OnRevive()  //����
{
	m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;
}

void KNpc::DoRun()
{
	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex<0)
		return ;
	if (m_CurrentRunSpeed) //��ǰ������	  m_RunSpeed ԭʼ���� m_CurrentRunSpeed +=  m_RunSpeed*�ٷֱ�
	{
		if (m_CurrentRunSpeed<10)
			m_CurrentRunSpeed =10;

		if (m_CurrentRunSpeed>m_RunFrame * m_RunSpeed)
		   m_Frames.nTotalFrame=m_RunFrame;
		else
		   m_Frames.nTotalFrame = (m_RunFrame * m_RunSpeed)/m_CurrentRunSpeed; //15*���ٰٷֱ�
	}
	else
		m_Frames.nTotalFrame = m_RunFrame;

	if (m_FightMode)
	{
		m_ClientDoing = cdo_fightrun;
	}
	else
	{
		m_ClientDoing = cdo_run;
	}
/*
	if (IsPlayer())
	{
		if (!Cost(attrib_stamina, m_CurrentStaminaLoss))
		{
			DoWalk();
			return;
		}
	}
*/
	if (m_Doing == do_run)
	{
		m_bLockNpcDwID=0;
		return;
	}
	m_bLockNpcDwID=0;
	m_Doing = do_run;

	m_Frames.nCurrentFrame = 0;
}

void KNpc::OnRun()
{		
	WaitForFrame();
	//if (!(m_LoopFrames % GAME_UPDATE_TIME) && Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 2)
	if (m_LoopFrames - (m_LoopFrames>>3<<3)==0 && Player[m_nPlayerIdx].m_cPK.GetNormalPKState() == 2)
	{
		m_CurrentStamina -= 18;  //���ٵ�����ֵ

		if (m_CurrentStamina <= 0)
		{
			m_CurrentStamina = 0;
		}
	}

	if (m_CurrentStamina == 0)  //û������ֵ
		ServeMove(m_CurrentWalkSpeed);

	else if(m_Doing == do_runattack)
	{//���Ź���	
		m_CurrentRunSpeed += 50;
		ServeMove(m_CurrentRunSpeed);
		m_CurrentRunSpeed -= 50;
	}
	else
	{//��ͨ���ܲ�
		ServeMove(m_CurrentRunSpeed);
	}

}

void KNpc::OnWalk()
{
	//#ifndef	_SERVER
	// ����ͻ��˵Ķ�����֡�ȡ���
	//#endif
	WaitForFrame();
	ServeMove(m_CurrentWalkSpeed);
	
}

void KNpc::DoSit()
{
	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex<0)
		return ;

	if (m_Doing == do_sit)
	{
//		DoStand();
		m_bLockNpcDwID=0;

		return;
	}
	m_bLockNpcDwID=0;
	m_Doing = do_sit;
    m_ClientDoing = cdo_sit;
	m_Frames.nTotalFrame = m_SitFrame;
	m_Frames.nCurrentFrame = 0;
}

void KNpc::OnSit()
{
	// ������������û���趨��
	if (WaitForFrame())
	{
		m_Frames.nCurrentFrame = m_Frames.nTotalFrame - 1;
	}
}
//ʹ�ü���(ʹ�ü������)
void KNpc::DoSkill(int nX, int nY)
{
	//_ASSERT(m_RegionIndex >= 0);

	if (m_RegionIndex<0)
		return ;

	if (m_Doing == do_skill || m_Doing == do_hurt)
		return;

	m_Hide.nTime = 0;      //ʹ�ü��ܺ� ����ȡ��
	// ��ս��״̬���ܷ�����
	if (IsPlayer())
	{//����ǿͻ��˱���
		if (!m_FightMode)
		{
			m_bLockNpcDwID=0;
			return;
		}
		//CoreDataChanged(GDCNI_OPEN_JINDUTIAO,0,0);       //�����رս�����
	}
	if (m_randmove.nTime>0)                          //����״̬���ܷ���
		return;

	ISkill * pSkill = GetActiveSkill(); // ��ȡ���������
	
	if(pSkill)
	{
		eSkillStyle eStyle = (eSkillStyle)pSkill->GetSkillStyle();
		if (m_SkillList.GetNextCastTimeByIndex(m_ActiveSkListIndex)>0 && (m_SkillList.GetNextCastTimeByIndex(m_ActiveSkListIndex)>SubWorld[m_SubWorldIndex].m_dwCurrentTime))
		{//��û�ⶳ
			m_bLockNpcDwID=0;
			return;
		}
		
		if (m_HorseType)
		{//����ʱ������
			if (m_SkillList.GetHorseNextCastTimeByIndex(m_ActiveSkListIndex)>0 && (m_SkillList.GetHorseNextCastTimeByIndex(m_ActiveSkListIndex)>SubWorld[m_SubWorldIndex].m_dwCurrentTime))
			{ 
				m_bLockNpcDwID=0;
				return;
			}
		}
		if (m_SkillList.CanCastByIndex(m_ActiveSkListIndex, SubWorld[m_SubWorldIndex].m_dwCurrentTime)   //�Ƿ���Է�����
			&& pSkill->CanCastSkill(m_Index, nX, nY) && (/*m_Kind != kind_player || */Cost(pSkill->GetSkillCostType(), pSkill->GetSkillCost(this))))
		{	
		/*------------------------------------------------------------------------------------
		������ʱ������ָ��Ŀ�����ʱ������Skill.Cast������������һ������Ϊ-1,�ڶ���ΪNpc index
		��S2Cʱ���ڶ�������������Server��NpcIndexתΪNpcdwID�γ�ȥ��
		��C�յ���ָ��ʱ����NpcdwIDתΪ������NpcIndex
			-------------------------------------------------------------------------------------*/
		    if (m_HorseType)
				m_SkillList.SetHorseNextCastTimeByIndex(m_ActiveSkListIndex, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetHorsePerCast());  //��������ʹ������ʱ��
			else
			{
			    m_SkillList.SetNextCastTimeByIndex(m_ActiveSkListIndex, SubWorld[m_SubWorldIndex].m_dwCurrentTime + pSkill->GetDelayPerCast());       //���ü��ܵ���С���ʱ��
			}
			if (eStyle == SKILL_SS_Missles || eStyle == SKILL_SS_Melee|| eStyle == SKILL_SS_InitiativeNpcState || eStyle == SKILL_SS_PassivityNpcState)
			{
				DoOrdinSkill((KSkill *) pSkill, nX, nY);
			}
			else
			{
				switch(eStyle)
				{
				case SKILL_SS_Thief:
					{
						((KThiefSkill*)pSkill)->DoSkill(this, nX, nY);

					}
					break;
				default:
					return;
				}
			}
		}	
		else
		{
			m_nPeopleIdx = 0;
			m_nObjectIdx = 0;
			m_bLockNpcDwID=0;
			DoStand();
		}
	}
} 

int KNpc::DoOrdinSkill(KSkill * pSkill, int nX, int nY)
{
	//_ASSERT(pSkill);
	if (!pSkill)
		return 0;

	int nEnChance = m_SkillList.GetEnChance(m_SkillList.FindSame(pSkill->GetSkillId()));
	//m_DataRes.StopSound();
	int x, y, tx, ty,tmap;
	SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
	
	if (nY < 0)
		return 0;
	
	if (nX < 0)
	{
		if (nX != -1) 
			return 0;
		
		if (nY >= MAX_NPC || Npc[nY].m_dwID == 0 || Npc[nY].m_SubWorldIndex != m_SubWorldIndex)
			return 0;
		Npc[nY].GetMpsPos(&tx, &ty,&tmap);
	}
	else
	{
		tx = nX;
		ty = nY;
	}
	
	m_SkillParam1 = nX;
	m_SkillParam2 = nY;
	m_DesX = nX;
	m_DesY = nY;
	
	m_Dir = g_GetDirIndex(x, y, tx, ty);  //��ȡ������ ����ķ���

	//if (pSkill->GetPreCastEffectFile()[0])
		//m_DataRes.SetSpecialSpr((char *)pSkill->GetPreCastEffectFile()); //����SPR����״̬
	
	if (IsPlayer())
		pSkill->PlayPreCastSound(m_nSex,x, y);	//ʹ�ü��ܵ�����
	
	//if (pSkill->IsNeedShadow())		
		//m_DataRes.SetBlur(TRUE);
	//else
		//m_DataRes.SetBlur(FALSE);
	
	CLIENTACTION ClientDoing = pSkill->GetActionType();  //���ж�������
	if (ClientDoing >= cdo_count) 
		m_ClientDoing = cdo_magic;
	else if (ClientDoing != cdo_none)
		m_ClientDoing = ClientDoing;
	pSkill->m_nEnChance = nEnChance;

	int naAttackSpeed=100,nbAttackSpeed=100;
	    g_GameSetTing.GetInteger2("SYSTEM","NpcAttackSpeed",&naAttackSpeed,&nbAttackSpeed);

	if (pSkill->GetSkillStyle() == SKILL_SS_Melee)
	{//
		if (CastMeleeSkill(pSkill) == FALSE)
		{
			m_nPeopleIdx = 0;
			m_nObjectIdx = 0;
			m_ProcessAI = 1;
			DoStand();
			return 1 ;
		}
	}
	//�����ܵļ����ͷ�ʱ������ͨ���ܲ�ͬ��һ����AttackFrame,һ����CastFrame
	else if (pSkill->IsPhysical() || !pSkill->IsMagic())
	{//�⹦����
		if (ClientDoing == cdo_none) 
			m_Frames.nTotalFrame = 0;
		else
		{
			if  (nbAttackSpeed + m_CurrentAttackSpeed !=0)
				m_Frames.nTotalFrame = m_AttackFrame * naAttackSpeed / (nbAttackSpeed + m_CurrentAttackSpeed); //  �⹦����
			else
				m_Frames.nTotalFrame = m_AttackFrame;

		}
		if (g_Random(3))
			m_ClientDoing = cdo_attack;
		else 
			m_ClientDoing = cdo_attack1;
		m_Doing = do_attack; //�⹦��������
	}
	else
	{//�ڹ�
		if (ClientDoing == cdo_none) 
			m_Frames.nTotalFrame = 0;
		else
		{
			if (m_CurrentCastSpeed + nbAttackSpeed !=0)
				m_Frames.nTotalFrame = m_CastFrame * naAttackSpeed / (m_CurrentCastSpeed + nbAttackSpeed);  // �ڹ�����
			else
				m_Frames.nTotalFrame = m_CastFrame ;
		}

		m_Doing  = do_magic;//�ڹ���������
	}
	
	m_ProcessAI = 0;
	m_Frames.nCurrentFrame = 0;	
	return 1;
}


//�⹦��������
void KNpc::DoAttack()
{
	if (m_Doing == do_attack)
		return;

	if (g_Random(2) == 1)
	{
		m_ClientDoing = cdo_attack;
	}
	else
	{
		m_ClientDoing = cdo_attack1;
	}
    int naAttackSpeed=100,nbAttackSpeed=100;
	    g_GameSetTing.GetInteger2("SYSTEM","NpcAttackSpeed",&naAttackSpeed,&nbAttackSpeed);
	
	m_ProcessAI = 0;

	if  (nbAttackSpeed + m_CurrentAttackSpeed !=0)
		m_Frames.nTotalFrame = m_AttackFrame * naAttackSpeed/(nbAttackSpeed + m_CurrentAttackSpeed);
	else
		m_Frames.nTotalFrame = m_AttackFrame;

	m_Frames.nCurrentFrame = 0;
	m_Doing = do_attack;
}


BOOL	KNpc::CastMeleeSkill(KSkill * pSkill)
{
	BOOL bSuceess = FALSE;
	//_ASSERT(pSkill);
	if (!pSkill)
		return bSuceess;

	switch(pSkill->GetMeleeType())
	{
	case Melee_AttackWithBlur:
		{
			bSuceess = DoBlurAttack();
		}break;
	case Melee_Jump:
		{
			if (NewJump(m_DesX, m_DesY))
			{
				DoJump();
				bSuceess = TRUE;
			}
			
		}break;
	case Melee_JumpAndAttack:
		{
			if (m_DesX < 0 && m_DesY > 0) 
			{
				int x, y;
				SubWorld[m_SubWorldIndex].NewMap2Mps
					(
					Npc[m_DesY].m_RegionIndex,
					Npc[m_DesY].m_MapX, 
					Npc[m_DesY].m_MapY, 
					Npc[m_DesY].m_OffX, 
					Npc[m_DesY].m_OffY, 
					&x,
					&y
					);
				
				m_DesX = x + 1;
				m_DesY = y;
			}

			if (NewJump(m_DesX, m_DesY))
			{
				DoJumpAttack();
				bSuceess = TRUE;
			}

		}break;
	case Melee_RunAndAttack:
		{
			bSuceess = DoRunAttack();

		}break;
	case Melee_ManyAttack:
		{
			bSuceess = DoManyAttack();
		}break;
	case Melee_Move:  //�Ṧ ��Ծ
		{
			if (NewJump(m_DesX, m_DesY))
				bSuceess = DoSecMove(pSkill->GetParam1());
		}break;
	default:
		m_ProcessAI = 1;
		break;
	}
	return bSuceess;

}

BOOL KNpc::DoBlurAttack()// DoSpecail1
{
	if (m_Doing == do_special1)
		return FALSE;
	
	KSkill * pSkill = (KSkill*) GetActiveSkill();
	if (!pSkill) 
        return FALSE;
	
	//_ASSERT(pSkill->GetSkillStyle() == SKILL_SS_Melee);
	if  (pSkill->GetSkillStyle()!=SKILL_SS_Melee)
		return false;

		m_ClientDoing = pSkill->GetActionType();
		//m_DataRes.SetBlur(TRUE);
	int naAttackSpeed=100,nbAttackSpeed=100;
	    g_GameSetTing.GetInteger2("SYSTEM","NpcAttackSpeed",&naAttackSpeed,&nbAttackSpeed);

		if  (nbAttackSpeed + m_CurrentAttackSpeed !=0)
			m_Frames.nTotalFrame = m_AttackFrame * naAttackSpeed / (nbAttackSpeed + m_CurrentAttackSpeed);
		else
			m_Frames.nTotalFrame = m_AttackFrame ;

	m_Frames.nCurrentFrame = 0;
	m_Doing = do_special1;
	return TRUE;
}
//�ڹ���������
void KNpc::DoMagic()
{
	if (m_Doing == do_magic)
		return;
	m_ProcessAI = 0;
	m_ClientDoing = cdo_magic;
	int naAttackSpeed=100,nbAttackSpeed=100;
	    g_GameSetTing.GetInteger2("SYSTEM","NpcAttackSpeed",&naAttackSpeed,&nbAttackSpeed);

		if (m_CurrentCastSpeed + nbAttackSpeed!=0) 
			m_Frames.nTotalFrame = m_CastFrame * naAttackSpeed/(m_CurrentCastSpeed + nbAttackSpeed);
		else
			m_Frames.nTotalFrame = m_CastFrame;

	m_Frames.nCurrentFrame = 0;
	m_Doing = do_magic;
}
//ʹ�ü���
void KNpc::OnSkill()
{
	KSkill * pSkill = NULL;
	KSkill * pFmSkill = NULL;
	int nSkill=0,nSkllLevel=0;
	if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
	{
		DoStand();
		m_ProcessAI = 1;	
	}
	else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
	{
		//m_DataRes.SetBlur(FALSE);
		if (m_DesX == -1) 
		{
			if (m_DesY <= 0) 
				goto Label_ProcessAI;
			
			//��ʱ�ý�ɫ�Ѿ���Чʱ
			if (Npc[m_DesY].m_RegionIndex < 0) 
				goto Label_ProcessAI;
		}
		
	    //������ȴʱ��	
		pSkill =(KSkill*) GetActiveSkill();  //��ȡ����ļ���

		if (pSkill)
		{
			int nEnChance = m_SkillList.GetEnChance(m_ActiveSkListIndex); //���㼼�ܼӳ�  m_SkillList.FindSame(pSkill->GetSkillId())
			pSkill->m_nEnChance = nEnChance;       //���ܼӳ�
			pSkill->Cast(m_Index, m_DesX, m_DesY); //��ĳ�� ���� ʹ�ü���
			//�����´�ʹ��ʱ����
		}
		  //�񽫼���
		  NpcFuMoCastSkll(m_Index, m_DesX, m_DesY);

Label_ProcessAI:
		if (m_Frames.nTotalFrame == 0)
		{
			m_ProcessAI = 1;
		}
	}	
}
//�񽫸��Ӽ���
void KNpc::NpcFuMoCastSkll(int nLauncher, int nParam1, int nParam2, int nWaitTime)
{
/*	KSkill * pFmSkill = NULL;
	int nSkill=0,nCaseTime=0,nSKillBiaoShi;

	if (m_nFuMoNum[0].nNpcSetings>1)
	{//��ϵ����	
		g_GameSetTing.GetInteger2("FuMoNpcSet","Skill_0",&nSkill,&nCaseTime);
		if (m_nFuMoNum[0].nSkillLevel<=0)
			m_nFuMoNum[0].nSkillLevel=1;
		 m_nFuMoNum[0].nCastTime = nCaseTime;
		 nSKillBiaoShi = 0;

#ifdef _SERVER
		 m_nFuMoNum[1].nNextCastTime = 0;//	���ü��ܵ���С���ʱ��
		 m_nFuMoNum[2].nNextCastTime = 0;
		 m_nFuMoNum[3].nNextCastTime = 0;
		 m_nFuMoNum[4].nNextCastTime = 0;
#else
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[1].nNextCastTime = 0;//	���ü��ܵ���С���ʱ��
         Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[2].nNextCastTime = 0;
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[3].nNextCastTime = 0;
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[4].nNextCastTime = 0;
#endif

		pFmSkill = (KSkill *)g_SkillManager.GetSkill(nSkill,m_nFuMoNum[0].nSkillLevel);
	}
	else if (m_nFuMoNum[1].nNpcSetings>1)
	{//ľϵ����
		g_GameSetTing.GetInteger2("FuMoNpcSet","Skill_1",&nSkill,&nCaseTime);
		if (m_nFuMoNum[1].nSkillLevel<=0)
			m_nFuMoNum[1].nSkillLevel=1;

		 m_nFuMoNum[1].nCastTime = nCaseTime;
		 nSKillBiaoShi = 1;

#ifdef _SERVER
		 m_nFuMoNum[0].nNextCastTime = 0;//	���ü��ܵ���С���ʱ��
		 m_nFuMoNum[2].nNextCastTime = 0;
		 m_nFuMoNum[3].nNextCastTime = 0;
		 m_nFuMoNum[4].nNextCastTime = 0;
#else
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[0].nNextCastTime = 0;//	���ü��ܵ���С���ʱ��
         Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[2].nNextCastTime = 0;
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[3].nNextCastTime = 0;
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[4].nNextCastTime = 0;
#endif
		pFmSkill = (KSkill *)g_SkillManager.GetSkill(nSkill,m_nFuMoNum[1].nSkillLevel);
	}
	else if (m_nFuMoNum[2].nNpcSetings>1)
	{//ˮϵ����
		g_GameSetTing.GetInteger2("FuMoNpcSet","Skill_2",&nSkill,&nCaseTime);
		if (m_nFuMoNum[2].nSkillLevel<=0)
			m_nFuMoNum[2].nSkillLevel=1;
		 m_nFuMoNum[2].nCastTime = nCaseTime;
		 nSKillBiaoShi = 2;

#ifdef _SERVER
		 m_nFuMoNum[0].nNextCastTime = 0;//	���ü��ܵ���С���ʱ��
		 m_nFuMoNum[1].nNextCastTime = 0;
		 m_nFuMoNum[3].nNextCastTime = 0;
		 m_nFuMoNum[4].nNextCastTime = 0;
#else
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[0].nNextCastTime = 0;//	���ü��ܵ���С���ʱ��
         Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[1].nNextCastTime = 0;
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[3].nNextCastTime = 0;
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[4].nNextCastTime = 0;
#endif
		pFmSkill = (KSkill *)g_SkillManager.GetSkill(nSkill,m_nFuMoNum[2].nSkillLevel);
	}
	else if (m_nFuMoNum[3].nNpcSetings>1)
	{//��ϵ����
		g_GameSetTing.GetInteger2("FuMoNpcSet","Skill_3",&nSkill,&nCaseTime);
		if (m_nFuMoNum[3].nSkillLevel<=0)
			m_nFuMoNum[3].nSkillLevel=1;
		 m_nFuMoNum[3].nCastTime = nCaseTime;
		 nSKillBiaoShi = 3;
#ifdef _SERVER
		 m_nFuMoNum[0].nNextCastTime = 0;//	���ü��ܵ���С���ʱ��
		 m_nFuMoNum[1].nNextCastTime = 0;
		 m_nFuMoNum[2].nNextCastTime = 0;
		 m_nFuMoNum[4].nNextCastTime = 0;
#else
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[0].nNextCastTime = 0;//	���ü��ܵ���С���ʱ��
         Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[1].nNextCastTime = 0;
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[2].nNextCastTime = 0;
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[4].nNextCastTime = 0;
#endif
		pFmSkill = (KSkill *)g_SkillManager.GetSkill(nSkill,m_nFuMoNum[3].nSkillLevel);
	}
	else if (m_nFuMoNum[4].nNpcSetings>1)
	{//��ϵ����
		g_GameSetTing.GetInteger2("FuMoNpcSet","Skill_4",&nSkill,&nCaseTime);
		if (m_nFuMoNum[4].nSkillLevel<=0)
			m_nFuMoNum[4].nSkillLevel=1;
        m_nFuMoNum[4].nCastTime = nCaseTime;
		nSKillBiaoShi = 4;
#ifdef _SERVER
		m_nFuMoNum[0].nNextCastTime = 0;//	���ü��ܵ���С���ʱ��
		m_nFuMoNum[1].nNextCastTime = 0;
		m_nFuMoNum[2].nNextCastTime = 0;
		m_nFuMoNum[3].nNextCastTime = 0;
#else
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[0].nNextCastTime = 0;//	���ü��ܵ���С���ʱ��
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[1].nNextCastTime = 0;
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[2].nNextCastTime = 0;
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[3].nNextCastTime = 0;
#endif
		pFmSkill = (KSkill *)g_SkillManager.GetSkill(nSkill,m_nFuMoNum[4].nSkillLevel);	
	}

#ifdef _SERVER
	//if  (pFmSkill)
	
		if (m_nFuMoNum[nSKillBiaoShi].nNextCastTime > 0 && (m_nFuMoNum[nSKillBiaoShi].nNextCastTime > SubWorld[m_SubWorldIndex].m_dwCurrentTime))
		{//��û�ⶳ
			return;
		}
		
		//if (m_HorseType)
		//{//����ʱ������
		//   if (m_nFuMoNum[nSKillBiaoShi].nNextCastTime > 0 && m_nFuMoNum[nSKillBiaoShi].nNextCastTime > SubWorld[m_SubWorldIndex].m_dwCurrentTime)
	    //      return;
		//} 
	
        //char msg[64];
        //t_sprintf(msg,"S����ʱ����:%d,S�´��ͷ�ʱ��:%d",m_nFuMoNum[nSKillBiaoShi].nCastTime,m_nFuMoNum[nSKillBiaoShi].nNextCastTime);
		//Player[GetnPlayerIdx()].m_ItemList.msgshow(msg);
#else
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[nSKillBiaoShi].nNextCastTime > 0 && (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[nSKillBiaoShi].nNextCastTime > SubWorld[0].m_dwCurrentTime))
		{//��û�ⶳ
			return;
		}	
       // char msg[64];
       // t_sprintf(msg,"C����ʱ����:%d,C�´��ͷ�ʱ��:%d",Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[nSKillBiaoShi].nCastTime,Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[nSKillBiaoShi].nNextCastTime);
	   //Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(msg);
#endif
	   //if (Npc[nParam2].m_Index <= 0)  //���ܿշ�
	   //return;

	if  (pFmSkill)
			pFmSkill->Cast(nLauncher, nParam1, nParam2,nWaitTime);
	//�����´�ʹ��ʱ����
	//if (m_HorseType)
	//	m_nFuMoNum[nSKillBiaoShi].nNextCastTime = SubWorld[m_SubWorldIndex].m_dwCurrentTime + nCaseTime;  //��������ʹ������ʱ��
	//else
	#ifdef _SERVER
	    m_nFuMoNum[nSKillBiaoShi].nNextCastTime = SubWorld[m_SubWorldIndex].m_dwCurrentTime + nCaseTime;//	���ü��ܵ���С���ʱ��
    #else
	    Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nFuMoNum[nSKillBiaoShi].nNextCastTime = SubWorld[0].m_dwCurrentTime + nCaseTime;//	���ü��ܵ���С���ʱ��
    #endif

    pFmSkill=NULL;
	*/
   return;
}

void KNpc::JumpTo(int nMpsX, int nMpsY)
{
	if (NewJump(nMpsX, nMpsY))
		DoJump();
	else
	{
		RunTo(nMpsX, nMpsY);
	}
}

void KNpc::RunTo(int nMpsX, int nMpsY)
{
	if (NewPath(nMpsX, nMpsY))
		DoRun();
}

void KNpc::Goto(int nMpsX, int nMpsY)
{
	if (NewPath(nMpsX, nMpsY))
		DoWalk();
}

void KNpc::DoWalk()
{
	//_ASSERT(m_RegionIndex >= 0);
  if (m_RegionIndex < 0)
	     return;

    if (m_CurrentWalkSpeed<5)
		m_CurrentWalkSpeed = 5;

	if ((m_CurrentWalkSpeed + 1)!=0)
		m_Frames.nTotalFrame = (m_WalkFrame * m_WalkSpeed) / m_CurrentWalkSpeed + 1;
	else
		m_Frames.nTotalFrame = m_WalkFrame;
	if (m_Doing == do_walk)
	{
		return;
	}
	m_Doing = do_walk;
	m_Frames.nCurrentFrame = 0;

	if (m_FightMode)
	{
		m_ClientDoing = cdo_fightwalk;
	}
	else
	{
		m_ClientDoing = cdo_walk;
	}
}

/*void KNpc::ClearEnhanceNote()
{
	  for (int i = 0;i<MAX_NPCSKILL)
	  {
		  //SAFE_DELETE(it->second);
		  if  (nEnhanceInfo.count(i)>0)
			   nEnhanceInfo.erase(i);
	  }

	  nEnhanceInfo.clear();

}


void KNpc::SetEnhanceNote(int i,int v)
{
   if  (nEnhanceInfo.count(i)>0)
	    nEnhanceInfo[i] = v;	

	    nEnhanceInfo[i] = v;
}


int  KNpc::GetEnhanceNote(int i)
{

    if  (nEnhanceInfo.count(i)>0)
		return 	nEnhanceInfo[i];
	else
		return 0;

} */

void KNpc::DoPlayerTalk(char * szTalk)
{
/*	
#ifdef _SERVER
	_ASSERT(m_RegionIndex >= 0);
	int nTalkLen = strlen(szTalk);
	if (!nTalkLen) return;
	BYTE * pNetCommand = new  BYTE[nTalkLen + 6 + 1];
	pNetCommand[0] = (BYTE)s2c_playertalk;
	*(DWORD *)(pNetCommand + 1) = m_dwID;
	pNetCommand[5] = nTalkLen;
	strcpy((char*)(pNetCommand + 6), szTalk);
	pNetCommand[nTalkLen + 6 ] = '\0';

	POINT	POff[8] = 
	{
		{0, 32},
		{-16, 32},
		{-16, 0},
		{-16, -32},
		{0, -32},
		{16, -32},
		{16, 0},
		{16, 32},
	};
	int nMaxCount = MAX_BROADCAST_COUNT;
	CURREGION.BroadCast(pNetCommand, nTalkLen + 6 + 1, nMaxCount, m_MapX, m_MapY);
	int i;
	for (i = 0; i < 8; ++i)
	{
		if (CONREGIONIDX(i) == -1)
			continue;
		CONREGION(i).BroadCast(pNetCommand, nTalkLen + 6 + 1, nMaxCount, m_MapX - POff[i].x, m_MapY - POff[i].y);
	}
	if (pNetCommand)
	{
		delete [] pNetCommand;
	}
#endif
*/
}

void KNpc::OnPlayerTalk()
{
	//û�����
}

int	KNpc::GetSkillLevel(int nSkillId)
{
	int nIndex = m_SkillList.FindSame(nSkillId);
	if (nIndex)
	{
		return m_SkillList.m_Skills[nIndex].SkillLevel;
	}
	else
	{
		return 0;
	}
}
//��������Ӧ�õ�NPC����  �Ѽ�������Ӧ�õ�NPC����  ��������
void KNpc::ModifyAttrib(int nAttacker, void* pData)
{
	if (pData != NULL)
		g_NpcAttribModify.ModifyAttrib(this,pData,nAttacker);  // �޸�����

}

//������������״̬Ч��
void KNpc::SetImmediatelySkillEffect(int nLauncher, void *pData, int nDataNum)
{
	if (!pData || !nDataNum)
		return;

	KMagicAttrib*	pTemp = (KMagicAttrib *)pData;
	//_ASSERT(nDataNum <= MAX_SKILL_STATE);
	if (nDataNum >= MAX_SKILL_STATE)
		nDataNum = MAX_SKILL_STATE;

	for (int i = 0; i < nDataNum; ++i)
	{
		ModifyAttrib(nLauncher, pTemp);//�ı�����-���ܼӳ�
		++pTemp;
	}
}

//�����ӵ���������  --�����ӵ��˺�����
void KNpc::AppendSkillEffect(int nIsMaigc ,BOOL bIsPhysical, BOOL bIsMelee, void *pSrcData, void *pDesData, int nEnChance)
{
	int nMinDamage,nMaxDamage;

		nMinDamage = m_PhysicsDamage.nValue[0] + m_CurrentPhysicsMagicDamageV.nValue[0]; //�����˺�+�̵��˺�
		nMaxDamage = m_PhysicsDamage.nValue[2] + m_CurrentPhysicsMagicDamageV.nValue[2];

		if (IsPlayer())
		{//������
			if (Player[m_nPlayerIdx].m_ItemList.GetWeaponType() == equip_meleeweapon)
			{//����н�������
				nMinDamage += nMinDamage * m_CurrentMeleeEnhance[Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular()] / 100;
				nMaxDamage += nMaxDamage * m_CurrentMeleeEnhance[Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular()] / 100;
			}
			else if (Player[m_nPlayerIdx].m_ItemList.GetWeaponType() == equip_rangeweapon)
			{//�����Զ������
				nMinDamage += nMinDamage * m_CurrentRangeEnhance / 100;
				nMaxDamage += nMaxDamage * m_CurrentRangeEnhance / 100;
			}
			else	// ����
			{
				nMinDamage += nMinDamage * m_CurrentHandEnhance / 100;
				nMaxDamage += nMaxDamage * m_CurrentHandEnhance / 100;
			}
		}
//===========================================================================
	KMagicAttrib* pTemp = (KMagicAttrib *)pSrcData;   //�ű����������ļ�������
	KMagicAttrib* pDes  = (KMagicAttrib *)pDesData;   //ͨ������������
	//�����ʵ�
	/*if (pTemp->nAttribType == magic_attackrating_p)
	{
		pDes->nAttribType = magic_attackrating_v;
		pDes->nValue[0] = (m_CurrentAttackRating+ m_CurrentAttackRatingEnhancev) * (m_CurrentAttackRatingEnhancep + pTemp->nValue[0]+100) / 100;
	}
	else
	{
		pDes->nAttribType = magic_attackrating_v;
		pDes->nValue[0] = (m_CurrentAttackRating+m_CurrentAttackRatingEnhancev)* (m_CurrentAttackRatingEnhancep +100) / 100;
	}*/
	if (pTemp->nAttribType == magic_attackrating_p)
	{
	   pDes->nAttribType = magic_attackrating_v;
	   pDes->nValue[0] =  m_AttackRating*(100+pTemp->nValue[0])/100;
	}
	else if (pTemp->nAttribType == magic_attackrating_v)
	{
	   pDes->nAttribType = magic_attackrating_v;
	   pDes->nValue[0] = m_AttackRating+pTemp->nValue[0];
	}
	pTemp++;
	pDes++;                                          //1 
	if (pTemp->nAttribType == magic_ignoredefense_p) //���Ե���������
	{
		pDes->nAttribType = magic_ignoredefense_p;   //���Ե���������
		pDes->nValue[0] = pTemp->nValue[0];
	}
	pTemp++;
	pDes++; //2  �չ��˺� �ٷֱ�
	if (pTemp->nAttribType == magic_physicsenhance_p)
	{ 
		pDes->nAttribType = magic_physicsenhance_p;//magic_physicsdamage_v;   //��ֵ��Ϊ�յ�
		pDes->nValue[0] = nMinDamage * (100 + pTemp->nValue[0]) / 100;
		pDes->nValue[2] = nMaxDamage * (100 + pTemp->nValue[0]) / 100;
			   
	if (nIsMaigc)
	{//�ű���������+����״̬(װ��+���ܵ�״̬)
		pDes->nValue[0] = (pDes->nValue[0]+m_CurrentAddmagicphysicsDamage)*(100+m_CurrentAddmagicphysicsDamageP)/100;
		pDes->nValue[2] = (pDes->nValue[2]+m_CurrentAddmagicphysicsDamage)*(100+m_CurrentAddmagicphysicsDamageP)/100;
	}
	else
	{//�ű���������+����״̬(װ��+���ܵ�״̬)
		pDes->nValue[0] = (pDes->nValue[0]+m_CurrentAddPhysicsDamage)*(100+m_CurrentAddPhysicsDamageP)/100;
		pDes->nValue[2] = (pDes->nValue[2]+m_CurrentAddPhysicsDamage)*(100+m_CurrentAddPhysicsDamageP)/100;
	} 

	}
	pTemp++;
	pDes++;//3  ���˺�
	if (pTemp->nAttribType == magic_colddamage_v)
	{
		pDes->nAttribType = magic_colddamage_v;
		pDes->nValue[0] = pTemp->nValue[0]+nMinDamage;
		pDes->nValue[1] = pTemp->nValue[1]; 
		pDes->nValue[2] = pTemp->nValue[2]+nMaxDamage;//*(100+ m_CurrentColdEnhance)/100;  
	if (nIsMaigc) 
	{//�ڱ�״̬(װ��+���ܵ�״̬)
		pDes->nValue[0] += m_CurrentMagicColdDamage.nValue[0]+m_CurrentAddmagicColdDamagicv;
		if  ((2+m_CurrentColdEnhance+m_CurrentMagicColdDamage.nValue[1])!= 0)
			pDes->nValue[1] = (pTemp->nValue[0]+pTemp->nValue[2]+m_CurrentAddmagicColdDamagicv)/2+m_CurrentColdEnhance+m_CurrentMagicColdDamage.nValue[1];//max(pDes->nValue[1]+m_CurrentAddmagicColdDamagicv,m_CurrentAddmagicColdDamagicv+m_CurrentMagicColdDamage.nValue[1] + m_CurrentColdEnhance); //����ʱ��
		else
			pDes->nValue[1] = 0;
		pDes->nValue[2] += m_CurrentMagicColdDamage.nValue[2]+m_CurrentAddmagicColdDamagicv;
	} 
	else
	{//���״̬(װ��+���ܵ�״̬)
		pDes->nValue[0] += m_CurrentMagicColdDamage.nValue[0]+m_CurrentAddColdDamagev;
		if ((2+m_CurrentColdEnhance+m_CurrentMagicColdDamage.nValue[1])!=0) 
			pDes->nValue[1] = (pTemp->nValue[0]+pTemp->nValue[2]+m_CurrentAddColdDamagev)/2+m_CurrentColdEnhance+m_CurrentMagicColdDamage.nValue[1];//max(pDes->nValue[1]+m_CurrentAddColdDamagev, m_CurrentAddColdDamagev+m_CurrentMagicColdDamage.nValue[1] + m_CurrentColdEnhance);   //����ʱ��
		else
			pDes->nValue[1] = 0;
		pDes->nValue[2] += m_CurrentMagicColdDamage.nValue[2]+m_CurrentAddColdDamagev;
	}
	}
	pTemp++;
	pDes++;//4 ���˺�
	if (pTemp->nAttribType == magic_firedamage_v)
	{
		pDes->nAttribType = magic_firedamage_v;
		pDes->nValue[0] = pTemp->nValue[0]+nMinDamage; //������������
		pDes->nValue[2] = (pTemp->nValue[2]+nMaxDamage)*(100 + m_CurrentFireEnhance)/ 100; //������������

	if (nIsMaigc) 
	{//�ű����ܻ�������+�ڻ�״̬(װ��+���ܵ�״̬)
		pDes->nValue[0] += m_CurrentMagicFireDamage.nValue[0]+m_CurrentAddmagicFireDamagicv;
	 	pDes->nValue[2] += m_CurrentMagicFireDamage.nValue[2]+m_CurrentAddmagicFireDamagicv;
	}
	else
	{ //�ű����ܻ�������+���״̬(װ��+���ܵ�״̬)
		pDes->nValue[0] += m_CurrentMagicFireDamage.nValue[0]+m_CurrentAddFireDamagev;
		pDes->nValue[2] += m_CurrentMagicFireDamage.nValue[2]+m_CurrentAddFireDamagev;
	} 
	}
	pTemp++;
	pDes++;//5 ���˺�
	if (pTemp->nAttribType == magic_lightingdamage_v)
	{
		pDes->nAttribType = magic_lightingdamage_v;
		pDes->nValue[0]   = pTemp->nValue[0]+nMinDamage;
		pDes->nValue[2]   = pTemp->nValue[2]+nMaxDamage;//(pTemp->nValue[2]+nMaxDamage) * (100 + m_CurrentLightEnhance)/ 100;
	  if (nIsMaigc)
	  { //�ű����ܻ�������+����״̬(װ��+���ܵ�״̬)
		pDes->nValue[0] += m_CurrentMagicLightDamage.nValue[0]+m_CurrentAddmagicLightDamagicv;
		pDes->nValue[2] += m_CurrentMagicLightDamage.nValue[2]+m_CurrentAddmagicLightDamagicv;
	  } 
	  else//if (bIsPhysical)
	  { //�ű����ܻ�������+����״̬(װ��+���ܵ�״̬)
		pDes->nValue[0] += m_CurrentMagicLightDamage.nValue[0]+m_CurrentAddLighDamagev;
		pDes->nValue[2] += m_CurrentMagicLightDamage.nValue[2]+m_CurrentAddLighDamagev;
	  }  
	}
	pTemp++;
	pDes++;//6 ���˺�
	if (pTemp->nAttribType == magic_poisondamage_v)
	{
		pDes->nAttribType = magic_poisondamage_v;
		pDes->nValue[0] = pTemp->nValue[0];//* (100 + m_CurrentPoisonEnhance) / 100;//���ܻ�������
		pDes->nValue[1] = pTemp->nValue[1];//���ܳ���ʱ��
		pDes->nValue[2] = pTemp->nValue[2];//���ܶ���ʱ��

		if (pDes->nValue[2] <= 0)
			pDes->nValue[2] = 18;

	if (nIsMaigc) 
	{//�ڶ�(װ��+���ܵ�״̬)
		pDes->nValue[0] += m_CurrentMagicPoisonDamage.nValue[0]+m_CurrentAddmagicPoisonDamagicv;  //���˺�
		pDes->nValue[1] += m_CurrentMagicPoisonDamage.nValue[1];  //����ʱ��
		pDes->nValue[2] += m_CurrentMagicPoisonDamage.nValue[2];  //����ʱ��
		
	} 
	else
	{//�ⶾ (װ��+���ܵ�״̬)
		pDes->nValue[0] += m_CurrentMagicPoisonDamage.nValue[0]+m_CurrentAddPoisonDamagev;        //���˺�
		pDes->nValue[1] += m_CurrentMagicPoisonDamage.nValue[1];  //����ʱ��
		pDes->nValue[2] += m_CurrentMagicPoisonDamage.nValue[2];  //����ʱ��
	} 
	//�ϳ��µĶ��˺�
        //g_NpcAttribModify.MixPoisonDamage(&m_CurrentPoisonDamage,pDes);
	}
	pTemp++;
	pDes++;//7 ħ���˺��������˺� ���ӷ�����
	if (pTemp->nAttribType == magic_magicdamage_v)  // ħ���˺�����
	{
		pDes->nAttribType = magic_magicdamage_v;
		pDes->nValue[0] = pTemp->nValue[0]+nMinDamage;
		pDes->nValue[2] = pTemp->nValue[2]+nMaxDamage;
	}
	pTemp++;
	pDes++;// 8 ѣ���˺�
	if (pTemp->nAttribType == magic_stun_p)
	{
		pDes->nAttribType = magic_stun_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		//pDes->nValue[1] += pTemp->nValue[1] * m_CurrentLightEnhance / 100;
	}
	pTemp++;
	pDes++; // 9 ���Ĺ���
	if (pTemp->nAttribType == magic_deadlystrike_p)
	{
		pDes->nAttribType = magic_deadlystrike_p;
		pDes->nValue[0] = pTemp->nValue[0];

	  if (bIsPhysical)
	  { //���⹦ϵ
		pDes->nAttribType = magic_deadlystrike_p;
		pDes->nValue[0] += m_CurrentDeadlyStrike;
	  }  
	}
	pTemp++;
	pDes++;// 10 ����һ��
	if (pTemp->nAttribType == magic_fatallystrike_p)
	{
		pDes->nAttribType = magic_fatallystrike_p;
		pDes->nValue[0] = pTemp->nValue[0];
	}
	pTemp++;
	pDes++;// 11 ��Ѫ ����
	if (pTemp->nAttribType == magic_steallife_p || pTemp->nAttribType == magic_steallifeenhance_p)
	{
		pDes->nAttribType = magic_steallife_p;   
		pDes->nValue[0] = pTemp->nValue[0];
	}
	pTemp++;
	pDes++; // 12 ���� ����
	if (pTemp->nAttribType == magic_stealmana_p || pTemp->nAttribType == magic_stealmanaenhance_p)
	{
		pDes->nAttribType = magic_stealmana_p; 
    //	if (nIsMaigc)
		pDes->nValue[0] = pTemp->nValue[0];
	}
	pTemp++;
	pDes++; // 13  �����˺�
	if (pTemp->nAttribType == magic_seriesdamage_p)  //13
	{
		pDes->nAttribType = magic_seriesdamage_p;
		pDes->nValue[0] = pTemp->nValue[0];	         //����

	    if (bIsPhysical)
		{  
		   pDes->nAttribType = magic_seriesdamage_p;
		   pDes->nValue[0] += m_CurrentSerisesEnhance;	//״̬
		}  
	}
	pTemp++;
	pDes++;  // 14 �Զ��ͷż���
	if (pTemp->nAttribType == magic_autoattackskill )    //�Զ����� 14
	{
		pDes->nAttribType = magic_autoattackskill;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
      #ifdef _SERVER
	    pDes->nAttribType = magic_autoattackskill;
	    pDes->nValue[0] = m_nAutoSkillId;
	    pDes->nValue[1] = m_nAutoLevel;
	    pDes->nValue[2] = m_nAutoRate;
      #endif // _SERVER
    }
	pTemp++;
	pDes++;  // 15  �յ�
	if (pTemp->nAttribType == magic_physicsdamage_v)
	{ 
		pDes->nAttribType = magic_physicsdamage_v;
		pDes->nValue[0] = pTemp->nValue[0]+nMinDamage;
		pDes->nValue[2] = pTemp->nValue[2]+nMaxDamage;

	if (nIsMaigc)
	{//�ű���������+����״̬(װ��+���ܵ�״̬)
		pDes->nValue[0] = (pDes->nValue[0]+m_CurrentAddmagicphysicsDamage)*(100+m_CurrentAddmagicphysicsDamageP)/100;
		pDes->nValue[2] = (pDes->nValue[2]+m_CurrentAddmagicphysicsDamage)*(100+m_CurrentAddmagicphysicsDamageP)/100;
	}
	else
	{//�ű���������+����״̬(װ��+���ܵ�״̬)
		pDes->nValue[0] = (pDes->nValue[0]+m_CurrentAddPhysicsDamage)*(100+m_CurrentAddPhysicsDamageP)/100;
		pDes->nValue[2] = (pDes->nValue[2]+m_CurrentAddPhysicsDamage)*(100+m_CurrentAddPhysicsDamageP)/100;
	}
	}
	pTemp++;
	pDes++;  // 16  ����ʱ��
	if (pTemp->nAttribType == magic_poisonenhance_p)
	{
		pDes->nAttribType = magic_poisonenhance_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}
	pTemp++;
	pDes++;  // 17  �ٻ�ʱ��
	if (pTemp->nAttribType == magic_coldenhance_p)
	{
		pDes->nAttribType = magic_coldenhance_p;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	}
	pTemp++;
	pDes++;  // 18  ץ������  
	if (pTemp->nAttribType == magic_addzhuabu_v)    //ץ������ 18
	{
		pDes->nAttribType = magic_addzhuabu_v;
		pDes->nValue[0] = pTemp->nValue[0];
		pDes->nValue[1] = pTemp->nValue[1];
		pDes->nValue[2] = pTemp->nValue[2];
	} 

 }

void KNpc::ServeMove(int MoveSpeed)  // �������ƶ�
{

	if (m_Doing != do_walk && m_Doing != do_run && m_Doing != do_hurt && m_Doing != do_runattack)
		return;

	if (MoveSpeed <= 0)
		return;

	if (MoveSpeed >= SubWorld[m_SubWorldIndex].m_nCellWidth)
	{//�ƶ����ٶ�̫���
		MoveSpeed = SubWorld[m_SubWorldIndex].m_nCellWidth - 1;	//31   15*
	}
	if (m_RegionIndex < 0 || m_RegionIndex >= 9) //���������ж�
	{
		//g_DebugLog("[zroc]Npc(%d)ServerMove RegionIdx = %d", m_Index, m_RegionIndex);
		//_ASSERT(0);
		DoStand(); //վ��
		return;
	}
	int x, y;

	SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex,m_MapX,m_MapY,0,0,&x,&y); //�������ת������������

	x = (x << 10) + m_OffX;
	y = (y << 10) + m_OffY;

	int nRet = m_PathFinder.GetDir(x,y,m_Dir,m_DesX,m_DesY,MoveSpeed,&m_Dir);  //����Ѱ·����

	if(nRet == 1)  //��·������
	{
		if (m_Dir >= MaxMissleDir)
			m_Dir=m_Dir%MaxMissleDir;

		x = g_DirCos(m_Dir,MaxMissleDir)*MoveSpeed*FIND_PATH_DISTANCE;  //�¸�Ҫ�ߵĵ�
		y = g_DirSin(m_Dir,MaxMissleDir)*MoveSpeed*FIND_PATH_DISTANCE;

		if (m_Kind==kind_player)
		{ 
			/*char nPos[128];
			t_sprintf(nPos,"<color=gyellow>����:%d,��һ����:X:%d,Y:%d<color>",m_Dir,x,y);
			//Player[nPlayerIndex].m_ItemList.ClientShowMsg(nPos);
			ccMessageBox(nPos,"ServeMove");*/
		} 

	}
	else if (nRet == 0) //·��ͨ
	{
		DoStand(); //վ��
		//ccMessageBox("·��ͨ","ServeMove");
		return;
	}
	else if (nRet == -1) //����ͼ����
	{//ɾ��NPC
		SubWorld[0].m_Region[m_RegionIndex].RemoveNpc(m_Index);
		SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY);
		m_RegionIndex = -1;
		return;
	}
	else
	{
		//ccMessageBox("��������","ServeMove");
		return;
	}

	int nOldRegion = m_RegionIndex;
	int nOldMapX = m_MapX;
	int nOldMapY = m_MapY;
	int nOldOffX = m_OffX;
	int nOldOffY = m_OffY;

	m_OffX +=x;
	m_OffY +=y;
//	����NPC��������
//	CELLWIDTH��CELLHEIGHT��OffX��OffY���ǷŴ���1024��

	if (!m_bClientOnly)
		CURREGION.DecNpcRef(m_MapX, m_MapY);
	
	if (m_OffX < 0)
	{
		m_MapX--;
		m_OffX += CELLWIDTH;
	}
	else if (m_OffX > CELLWIDTH)
	{
		m_MapX++;
		m_OffX -= CELLWIDTH;
	}

	if (m_OffY < 0)
	{
		m_MapY--;
		m_OffY += CELLHEIGHT;
	}
	else if (m_OffY > CELLHEIGHT)
	{
		m_MapY++;
		m_OffY -= CELLHEIGHT;
	}
	
	if (m_MapX < 0)
	{
		m_RegionIndex = LEFTREGIONIDX;
		m_MapX += REGIONWIDTH;
	}
	else if (m_MapX >= REGIONWIDTH)
	{
		m_RegionIndex = RIGHTREGIONIDX;
		m_MapX -= REGIONWIDTH;
	}

	if (m_RegionIndex >= 0)
	{
		if (m_MapY < 0)
		{
			m_RegionIndex = UPREGIONIDX;
			m_MapY += REGIONHEIGHT;
		}
		else if (m_MapY >= REGIONHEIGHT)
		{
			m_RegionIndex = DOWNREGIONIDX;
			m_MapY -= REGIONHEIGHT;
		}

		if (!m_bClientOnly && m_RegionIndex >= 0)
			CURREGION.AddNpcRef(m_MapX,m_MapY);
	}

	if (m_RegionIndex == -1)	// �������ƶ���-1 Region�������������������ָ�ԭ����
	{//�ָ�ԭ��
		//Ҫ��Ҫ������ ɾ������?
		if (g_GameWorld)
		{//ɾ�����о���
           g_GameWorld->FreeAllSprite();
		}
		m_RegionIndex = nOldRegion;
		m_MapX = nOldMapX;
		m_MapY = nOldMapY;
		m_OffX = nOldOffX;
		m_OffY = nOldOffY;
		CURREGION.AddNpcRef(m_MapX,m_MapY);
		return;
	}

	if (nOldRegion != m_RegionIndex)
	{//������������ת������
		SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID,SubWorld[0].m_Region[m_RegionIndex].m_RegionID,m_Index);
		m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
	}
}

void KNpc::ServeJump(int nSpeed)
{
	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	if (!(m_Doing == do_jump || m_Doing == do_jumpattack))
		return;
	
	if (nSpeed <= 0)
		return;

	if (nSpeed >= SubWorld[m_SubWorldIndex].m_nCellWidth)
	{
		nSpeed = SubWorld[m_SubWorldIndex].m_nCellWidth - 1;
	}

	m_OffX += g_DirCos(m_JumpDir, 64) * nSpeed;
	m_OffY += g_DirSin(m_JumpDir, 64) * nSpeed;

	// s = vt - a * t * t / 2
	m_Height = (m_JumpFirstSpeed * m_Frames.nCurrentFrame - ACCELERATION_OF_GRAVITY * m_Frames.nCurrentFrame * m_Frames.nCurrentFrame / 2) / 8;
	if (m_Height < 0)
		m_Height = 0;

	int nOldRegion = m_RegionIndex;
	int nOldMapX = m_MapX;
	int nOldMapY = m_MapY;
	int nOldOffX = m_OffX;
	int nOldOffY = m_OffY;
	CURREGION.DecNpcRef(m_MapX, m_MapY);
	
	if (m_OffX < 0)
	{
		m_MapX--;
		m_OffX += CELLWIDTH;
	}
	else if (m_OffX > CELLWIDTH)
	{
		m_MapX++;
		m_OffX -= CELLWIDTH;
	}
	
	if (m_OffY < 0)
	{
		m_MapY--;
		m_OffY += CELLHEIGHT;
	}
	else if (m_OffY > CELLHEIGHT)
	{
		m_MapY++;
		m_OffY -= CELLHEIGHT;
	}
	
	if (m_MapX < 0)
	{
		m_RegionIndex = LEFTREGIONIDX;
		m_MapX += REGIONWIDTH;
	}
	else if (m_MapX >= REGIONWIDTH)
	{
		m_RegionIndex = RIGHTREGIONIDX;
		m_MapX -= REGIONWIDTH;
	}
	
	if (m_RegionIndex >= 0)
	{
		if (m_MapY < 0)
		{
			m_RegionIndex = UPREGIONIDX;
			m_MapY += REGIONHEIGHT;
		}
		else if (m_MapY >= REGIONHEIGHT)
		{
			m_RegionIndex = DOWNREGIONIDX;
			m_MapY -= REGIONHEIGHT;
		}
		if (m_RegionIndex >= 0)
			CURREGION.AddNpcRef(m_MapX, m_MapY);
	}

	if (m_RegionIndex == -1)	// �������ƶ���-1 Region�������������������ָ�ԭ����
	{
		m_RegionIndex = nOldRegion;
		m_MapX = nOldMapX;
		m_MapY = nOldMapY;
		m_OffX = nOldOffX;
		m_OffY = nOldOffY;
		CURREGION.AddNpcRef(m_MapX, m_MapY);
		return;
	}

	if (nOldRegion != m_RegionIndex)
	{
		if (m_RegionIndex >= 0)
		{
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
			m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
		}
	}		
}

void KNpc::SendSerCommand(NPCCMD cmd,int x,int y, int z)
{
	m_Command.CmdKind = cmd;
	m_Command.Param_X = x;
	m_Command.Param_Y = y;
	m_Command.Param_Z = z;
}

BOOL KNpc::NewPath(int nMpsX, int nMpsY)
{
	m_DesX = nMpsX;
	m_DesY = nMpsY;
	return TRUE;
}

BOOL KNpc::NewJump(int nMpsX, int nMpsY)
{
	//_ASSERT(m_CurrentJumpSpeed > 0);
	if (m_CurrentJumpSpeed <= 0)
		return FALSE;
	
	int nX, nY,nMap;
	    GetMpsPos(&nX, &nY,&nMap);
														
		if (nX == nMpsX && nY == nMpsY)	//���NPC������ Ŀ����ͬ�� ��������
		return FALSE;

	int nDir = g_GetDirIndex(nX, nY, nMpsX, nMpsY);	 //��ȡ������֮��ķ���
	int	nMaxLength = m_CurrentJumpSpeed * m_CurrentJumpFrame;
	int	nWantLength = g_GetDistance(nX, nY, nMpsX, nMpsY); //������ľ���

	if (m_Dir >= MaxMissleDir)
		m_Dir -= MaxMissleDir;

	int	nSin = g_DirSin(nDir, MaxMissleDir);
	int	nCos = g_DirCos(nDir, MaxMissleDir);

	if (nWantLength > nMaxLength)
	{
		m_DesX = nX + ((nMaxLength * nCos) >> 10);
		m_DesY = nY + ((nMaxLength * nSin) >> 10);
		nWantLength = nMaxLength;
	}
	else if (nWantLength <= MIN_JUMP_RANGE)
	{
		m_DesX = nMpsX;
		m_DesY = nMpsY;
		return FALSE;
	}

	m_JumpStep = nWantLength / m_CurrentJumpSpeed;	  //Ҫ�����ٲ�

	int nTestX = 0;
	int nTestY = 0;
	int nSuccessStep = 0;

	for (int i = 1; i < m_JumpStep + 1; ++i)
	{
		nTestX = nX + ((m_CurrentJumpSpeed * nCos * i) >> 10);
		nTestY = nY + ((m_CurrentJumpSpeed * nSin * i) >> 10);
		int nBarrier = SubWorld[m_SubWorldIndex].GetBarrier(nTestX, nTestY); //������Ƿ����ϰ�
		if (Obstacle_NULL == nBarrier)
		{
			nSuccessStep = i;
		}
		if (Obstacle_Normal == nBarrier || Obstacle_Fly == nBarrier)
		{
			if (nSuccessStep <= MIN_JUMP_RANGE / m_CurrentJumpSpeed)
			{
				return FALSE;
			}
			m_DesX = nX + ((m_CurrentJumpSpeed * nCos * nSuccessStep) >> 10);
			m_DesY = nY + ((m_CurrentJumpSpeed * nSin * nSuccessStep) >> 10);
			m_JumpStep = nSuccessStep;
			break;
		}

		int	nTrap = SubWorld[m_SubWorldIndex].GetTrap(nTestX, nTestY);

		if (nTrap)
		{//��Trap
			if (i <= MIN_JUMP_RANGE / m_CurrentJumpSpeed)
			{
				return FALSE;
			}
			m_DesX = nX + ((m_CurrentJumpSpeed * nCos * i) >> 10);
			m_DesY = nY + ((m_CurrentJumpSpeed * nSin * i) >> 10);
			m_JumpStep = i;
			break;
		}
	}
	m_JumpDir = nDir;
	return TRUE;
}

/*void KNpc::SelfDamage(int nDamage)
{
	m_CurrentLife -= nDamage;

	if (m_CurrentLife <= 0)
	{
		m_CurrentLife = 1;
	}
} */
//��ʾ�������� �������� ������������
BOOL KNpc::Cost(NPCATTRIB nType, int nCost, BOOL bOnlyCheckCanCast)
{
	if (!IsPlayer())
		return TRUE;

	int *pSource = NULL;

	switch(nType)
	{
	case attrib_mana:
		pSource = &m_CurrentMana;
		break;
	case attrib_life:
		pSource = &m_CurrentLife;
		break;
	case attrib_stamina:
		pSource = &m_CurrentStamina;
		break;
	default:
		break;
	}

	if (pSource)
	{
		if (*pSource - nCost<=0)
		{
			KSystemMessage Msg;

			Msg.byConfirmType = SMCT_NONE;
			Msg.byParamSize = 0;
			Msg.byPriority = 1;
			Msg.eType = SMT_NORMAL;

			switch(nType)
			{
			case attrib_mana:
				g_StrCpyLen(Msg.szMessage,  strCoreInfo[MSG_NPC_NO_MANA].c_str(), sizeof(Msg.szMessage));
				break;
			case attrib_life:
				g_StrCpyLen(Msg.szMessage, strCoreInfo[MSG_NPC_NO_LIFE].c_str(), sizeof(Msg.szMessage));
				break;
			case attrib_stamina:
				g_StrCpyLen(Msg.szMessage, strCoreInfo[MSG_NPC_NO_STAMINA].c_str(), sizeof(Msg.szMessage));
				break;
			default:
				break;
			}
			//Msg.nMsgLen = TEncodeText(Msg.szMessage, strlen(Msg.szMessage));
			//CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&Msg, NULL);
			return FALSE;
		}
		else
		{
			if (!bOnlyCheckCanCast)
				*pSource -= nCost;
			return TRUE;
		}
	}
	return FALSE;
}

void KNpc::DoJump()
{
	//_ASSERT(m_RegionIndex >= 0);
	if (m_RegionIndex < 0)
		return;

	if (m_Doing == do_jump)
		return;
	
	m_Doing = do_jump;
	m_Dir = m_JumpDir;
	m_ProcessAI	= 0;
	m_JumpFirstSpeed = ACCELERATION_OF_GRAVITY * (m_JumpStep - 1) / 2 ;
	m_ClientDoing = cdo_jump;
	m_Frames.nTotalFrame = m_JumpStep;
	m_Frames.nCurrentFrame = 0;	
}

BOOL KNpc::OnJump()
{
	ServeJump(m_CurrentJumpSpeed);
	if (WaitForFrame())
	{
		DoStand();
		m_ProcessAI	= 1;
		return FALSE;
	}
	return TRUE;
}

BOOL KNpc::WaitForFrame()
{
	m_Frames.nCurrentFrame ++;
	if (m_Frames.nCurrentFrame < m_Frames.nTotalFrame)
	{
		return FALSE;
	}
	m_Frames.nCurrentFrame = 0;
	return TRUE;
}

BOOL KNpc::IsReachFrame(int nPercent)
{
	if (m_Frames.nCurrentFrame == m_Frames.nTotalFrame * nPercent / 100)
	{
		return TRUE;
	}
	return FALSE;
}
//ˢ��
//�ͻ��˴�����õ���NpcSettingIdx�ǰ�����16λNpc��ģ������16λΪ�ȼ�
void KNpc::Load(int nNpcSettingIdx, int nLevel,int nSubWorld,int nBoss)
{
	m_PathFinder.Init(m_Index);

	if (nLevel <= 0) 
	{
		nLevel = 1;
	}

	//����NPC ���
	if (nNpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID || nNpcSettingIdx == PLAYER_FEMALE_NPCTEMPLATEID)
	{
		m_NpcSettingIdx = nNpcSettingIdx;
		m_Level = nLevel;
		
		if (nNpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID)
		{
			//ccMessageBox("start add nan role","start add nan");
			strcpy(szNpcTypeName, "������");
			m_StandFrame = NpcSet.GetPlayerStandFrame(TRUE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(TRUE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(TRUE);
		}
		else
		{
			//ccMessageBox("start add nv role","start add nv");
			strcpy(szNpcTypeName,"Ů����");
			m_StandFrame = NpcSet.GetPlayerStandFrame(FALSE);
			m_WalkFrame = NpcSet.GetPlayerWalkFrame(FALSE);
			m_RunFrame = NpcSet.GetPlayerRunFrame(FALSE);
		}
		m_WalkSpeed   = NpcSet.GetPlayerWalkSpeed();
		m_RunSpeed    = NpcSet.GetPlayerRunSpeed();
		m_AttackFrame = NpcSet.GetPlayerAttackFrame();
		m_HurtFrame	  = NpcSet.GetPlayerHurtFrame();
	}
	else
	{ //��ͨnpc ����
		GetNpcCopyFromTemplate(nNpcSettingIdx, nLevel);  //���� NPCs.TXT ����Ļ�����Ϣ����
		g_NpcSetting.GetString(nNpcSettingIdx + 2, "NpcResType", "enemy003", szNpcTypeName, sizeof(szNpcTypeName));
		if (!szNpcTypeName[0])
		{ //�������ͱ� npcres
			g_NpcKindFile.GetString(2, "��������", "enemy003", szNpcTypeName, sizeof(szNpcTypeName));//���û�ҵ����õ�һ��npc����
		}
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIMode", 12, &m_AiMode);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam1", 12, &m_AiParam[0]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam2", 12, &m_AiParam[1]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam3", 12, &m_AiParam[2]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam4", 12, &m_AiParam[3]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam5", 12, &m_AiParam[4]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam6", 12, &m_AiParam[5]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam7", 12, &m_AiParam[6]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam8", 12, &m_AiParam[7]);
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "AIParam9", 12, &m_AiParam[8]);

		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "ActiveRadius", 12, &m_ActiveRadius); //���Χ
		int m_nIsRevive;
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "ClientOnly", 0, &m_nIsRevive);
		if  (m_nIsRevive>0)
			m_bClientOnly = true;
		else
			m_bClientOnly =false;
		m_nIsRevive = 0;
		g_NpcSetting.GetInteger(nNpcSettingIdx + 2, "IsRevive", 0, &m_nIsRevive);
	    m_IsRevive = (DWORD)m_nIsRevive;
        char nMapNpc[32]={0};
		int  nIsCreat=0;
		ZeroMemory(nMapNpc,sizeof(nMapNpc));
		t_sprintf(nMapNpc,"%d_IsScript",SubWorld[nSubWorld].m_SubWorldID);
		g_NpcMapDropRate.GetInteger("List",nMapNpc,0,&nIsCreat);
		IsExeGoldScript = nIsCreat; 
		t_sprintf(nMapNpc,"%d_IsCreat",SubWorld[nSubWorld].m_SubWorldID);
		g_NpcMapDropRate.GetInteger("List",nMapNpc,0,&nIsCreat);//Ĭ��ִ��ȫ�ֽű�
	    IsCreatBoss     = nIsCreat; 
		// �����࣬11��12��17����AiParam[6]����߻��趨�߶�
		// add by flying
		if (m_AiMode == 11 || m_AiMode == 12 || m_AiMode == 17)
			m_AiParam[6] = m_AiMode;
	}
    //��ͨNPC ������NPC ͨ��
	/*m_DataRes.Init(szNpcTypeName, &g_NpcResList);//��۳�ʼ��
	m_DataRes.SetAction(m_ClientDoing);  //�趨�����Ϊ�Խ�
	m_DataRes.SetRideHorse(m_bRideHorse);//�趨�Ƿ������֡��
	m_DataRes.SetArmor(m_ArmorType);     //�·�
	m_DataRes.SetHelm(m_HelmType);       //ͷ��
	m_DataRes.SetHorse(m_HorseType);     //��λ��
	m_DataRes.SetWeapon(m_WeaponType);   //����
	*/
  if (m_Kind==kind_player)
  { 
        // m_DataRes.SetPifeng(m_PifengType); //����
        // m_DataRes.SetChiBang(m_ChiBangType);//���
	 // ccMessageBox("add player finish..","add player finish.");
  }
	m_CurrentCamp = m_Camp;
}

void KNpc::GetMpsPos(int *pPosX, int *pPosY,int *nMapid)
{
	SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex,m_MapX,m_MapY,m_OffX,m_OffY,pPosX,pPosY);
    *nMapid=m_SubWorldIndex;
}


//����Ϊ�����
BOOL	KNpc::ChangeSkillAttackRadius(int nSkillIdx,int nDis)
{
	if (nSkillIdx <= 0 || nSkillIdx >= MAX_NPCSKILL)
		return FALSE;

    if (!IsPlayer())
      return FALSE;

	if (!(m_SkillList.m_Skills[nSkillIdx].SkillId && m_SkillList.m_Skills[nSkillIdx].CurrentSkillLevel))
		return FALSE;

	int nLevel = m_SkillList.m_Skills[nSkillIdx].SkillLevel;

	int nChanSkillID= m_SkillList.m_Skills[nSkillIdx].SkillId;

	if  (nChanSkillID >=MAX_SKILL && nLevel >=MAX_SKILLLEVEL && nLevel <=0)
		return FALSE;

	ISkill * pISkill =  g_SkillManager.GetSkill(nChanSkillID, nLevel);

	if (pISkill)
	{
		//int nTempDis = 0;
		if (!Player[CLIENT_PLAYER_INDEX].m_isopenjuli)
			nDis  = pISkill->getBackAttackRadius();
		//�ָ���������
		m_CurrentAttackRadius = pISkill->ChangeAttackRadius(nDis); //�õ����ܵĹ�����Χ
     //�ͻ��˷���Э��ͬ����������
		//char Info[64];
		//t_sprintf(Info,"���ֵ�����������:%d",m_CurrentAttackRadius);
		//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(Info);
		SendClientRunScript(1,nSkillIdx,nDis,nChanSkillID);
	}
	return TRUE;
}

//����Ϊ�����
BOOL	KNpc::SetActiveSkill(int nSkillIdx)
{
	if (nSkillIdx <= 0 || nSkillIdx >= MAX_NPCSKILL)
		return FALSE;

	if (!(m_SkillList.m_Skills[nSkillIdx].SkillId && m_SkillList.m_Skills[nSkillIdx].CurrentSkillLevel))
		return FALSE;

	m_ActiveSkillID = m_SkillList.m_Skills[nSkillIdx].SkillId;
	m_ActiveSkListIndex =nSkillIdx;
	int nLevel = m_SkillList.m_Skills[nSkillIdx].SkillLevel;
	//_ASSERT(m_ActiveSkillID < MAX_SKILL && nLevel < MAX_SKILLLEVEL && nLevel > 0);
	if  (m_ActiveSkillID >=MAX_SKILL || nLevel >=MAX_SKILLLEVEL || nLevel <=0)
		return FALSE;

	ISkill * pISkill =  g_SkillManager.GetSkill(m_ActiveSkillID, nLevel);

	if (pISkill)
    {
		m_CurrentAttackRadius = pISkill->GetAttackRadius(); //�õ����ܵĹ�����Χ
    }
	return TRUE;
}
//��ȡ�ڼ������ܵ�ID
int	KNpc::GetSkillID(int nSkillIdx)
{
	if (nSkillIdx <= 0 || nSkillIdx >= MAX_NPCSKILL)
		return FALSE;
	
	if (!(m_SkillList.m_Skills[nSkillIdx].SkillId && m_SkillList.m_Skills[nSkillIdx].CurrentSkillLevel))
		return FALSE;
	return m_SkillList.m_Skills[nSkillIdx].SkillId;
}

// ͬ���ͻ�������״̬���ܵ�����
void KNpc::UpdataNpcCurData(int nWonerIndex,int nEquipPlace) //������������Բ���
{

/*	for (int j = 0 ; j< MAX_NPCSKILL;j++)
	{
		int nAdd = 	Npc[m_nIndex].m_SkillList.GetAddPoint(j);
		if (nAdd > 0)
		{
			nAdd = -nAdd;
#ifdef _SERVER	
			Npc[m_nIndex].m_SkillList.IncreaseLevel(j,nAdd); //����������ܱ���״̬
#endif
			Npc[m_nIndex].m_SkillList.QeuipAddPoint(j,nAdd);
		}

		Npc[m_nIndex].m_SkillList.m_Skills[j].mAddPoint = 0;
		Npc[m_nIndex].m_SkillList.m_Skills[j].EnChance = 0;  // ���ܵļӳ�
	}*/

	ZeroMemory(&m_CurrentPhysicsMagicDamageP, sizeof(KMagicAttrib));
	ZeroMemory(&m_CurrentPhysicsMagicDamageV, sizeof(KMagicAttrib));

/*	int nFomoidx=Npc[m_nIndex].GetCurFuMoIdx(),nCurbei=0;
	
	if 	(nFomoidx>-1)
	{
		if (Npc[m_nIndex].GetCurFoMoSkllLevel(nFomoidx)>=5) //��ȡ�񽫵�ǰ�ĵȼ�
		{
			nCurbei=Npc[m_nIndex].GetCurFoMoSkllLevel(nFomoidx)/5;
		}
	}*/

	m_CurrentLifeMax		= m_LifeMax;
	m_CurrentManaMax		= m_ManaMax;
	m_CurrentStaminaMax	= m_StaminaMax;
	m_CurrentAttackRating	= m_AttackRating;
	m_CurrentAttackSpeed	= m_AttackSpeed;
	m_CurrentCastSpeed	= m_CastSpeed;
    m_CurrentRunSpeed	    = m_RunSpeed; 
	m_CurrentJumpSpeed	= m_JumpSpeed;
	m_CurrentVisionRadius	= m_VisionRadius;
	m_CurrentWalkSpeed	= m_WalkSpeed;
	m_CurrentNuQiMax		= 0;//ŭ��
	m_CurrentDefend		= m_Defend;
	m_CurrentLifeReplenish= m_LifeReplenish;
	m_CurrentHitRecover	= m_HitRecover;  //Npc�����˶���ʱ��
	m_CurrentManaReplenish= m_ManaReplenish;

	m_TempFireResist = 0;	        // Npc�ĵ�ǰ����
	m_TempColdResist = 0;	            // Npc�ĵ�ǰ������
	m_TempPoisonResist = 0;	        // Npc�ĵ�ǰ������
	m_TempLightResist = 0;	        // Npc�ĵ�ǰ�翹��
	m_TempPhysicsResist = 0;	    // Npc�ĵ�ǰ������

	m_CurrentLightResist	= BASE_NPC_RESIST_MAX; //m_LightResist;
	m_CurrentPhysicsResist  = BASE_NPC_RESIST_MAX; //m_PhysicsResist;
	m_CurrentPoisonResist	= BASE_NPC_RESIST_MAX; //m_PoisonResist;
	m_CurrentColdResist	    = BASE_NPC_RESIST_MAX; //m_ColdResist;
	m_CurrentFireResist	    = BASE_NPC_RESIST_MAX; //m_FireResist;
	m_nCurNpcLucky          = BASE_NPC_RESIST_MAX;

	m_CurrentStaminaGain	= m_StaminaGain;
	m_CurrentStaminaLoss	= m_StaminaLoss;
	//SetBaseSpeedAndRadius();
	ZeroMemory(&m_CurrentMagicColdDamage, sizeof(KMagicAttrib));
//	ZeroMemory(&m_CurrentColdDamage, sizeof(KMagicAttrib));
	m_CurrentColdEnhance	= 0;

	m_CurrentColdResistMax	= MAX_RESIST;//m_ColdResistMax;
	m_CurrentFireResistMax  = MAX_RESIST;
	m_CurrentLightResistMax = MAX_RESIST;//m_LightResistMax;
	m_CurrentPhysicsResistMax = MAX_RESIST;//m_PhysicsResistMax;
	m_CurrentPoisonResistMax  = MAX_RESIST;//m_PoisonResistMax;

	m_CurrentDamage2Mana	= 0;
	m_CurrentDamageReduce	= 0;
	m_CurrentDeadlyStrike	= 0;
//	m_CurrentElementDamageReduce = 0;
	ZeroMemory(&m_CurrentMagicFireDamage, sizeof(KMagicAttrib));
//	ZeroMemory(&m_CurrentFireDamage, sizeof(KMagicAttrib));
	m_CurrentFireEnhance	= 0;
	
	m_CurrentHandEnhance	= 0;
	m_CurrentKnockBack	= 0;
	m_CurrentLifeStolen	= 0;
	ZeroMemory(&m_CurrentMagicLightDamage, sizeof(KMagicAttrib));
//	ZeroMemory(&m_CurrentLightDamage, sizeof(KMagicAttrib));
//	ZeroMemory(&m_WaiPhysicsDamage, sizeof(KMagicAttrib));
	m_CurrentLightEnhance	= 0;
	m_CurrentPoisonTime=0;

//	m_CurrentManaPerEnemy	= 0;
	m_CurrentManaStolen	= 0;
	m_CurrentMeleeDmgRet	= 0;        // ���̷���
	m_CurrentHulueMeleeDmgRet=0;      // ���Խ��̷������ٵ㡢
   	m_CurrentHulueRangeDmgRet=0;      // ����Զ�̷̳������ٵ㡢
	m_CurrentMeleeDmgRetPercent = 0;  // ���̷����ٷֱ�
	m_CurrentTempSpeed=0;
	ZeroMemory(&m_CurrentMeleeEnhance, sizeof(m_CurrentMeleeEnhance));

//	m_CurrentPiercePercent	= 0;
	ZeroMemory(&m_CurrentMagicPoisonDamage, sizeof(KMagicAttrib));
//	ZeroMemory(&m_CurrentPoisonDamage, sizeof(KMagicAttrib));
	m_CurrentPoisonEnhance	= 0;

	m_CurrentRangeDmgRet	= 0;         //Զ�̷���
	m_CurrentRangeDmgRetPercent	= 0; //Զ�̷����ٷֱ�
	m_CurrentRangeEnhance	= 0;
	m_CurrentSlowMissle	= 0;         //�ӵ�����
	m_CurrentStaminaStolen	= 0;
	m_CurrentAddPhysicsDamage = 0;      //�⹦�յ�
	m_CurrentUpExp        = 0;
	m_CurrentFreezeTimeReducePercent = 0;
	m_CurrentPoisonTimeReducePercent = 0;
	m_EnemyPoisonTimeReducePercent   = 0;
	m_CurrentStunTimeReducePercent = 0;
	m_EnemyStunTimeReducePercent   = 0;
	m_CurrentSerisesEnhance = 0;
	m_CurrentDamageReduce = 0;
	m_CurrentdanggeRate = 0;  //����
	m_CurrentzhongjiRate = 0; //�ػ�
	 m_CurrentcjdanggeRate	= 0;               //��⵵��
	 m_CurrentcjzhongjiRate	= 0;               //����ػ�
	 m_Currentsorbdamage	= 0;                   //�����˺�
	 m_Currentsorbdamage_v=0;
	 m_Currenadddamagev   = 0;
	 m_Currenadddamagep   = 0;
	 m_Currentpoisonres	= 0;                   //=���ԶԷ�����:#d1-%
	 m_Currentfireres	= 0;                       //=���ԶԷ����:#d1-%
	 m_Currentlightingres	= 0;                   //=���ԶԷ��׷�:#d1-%
	 m_Currentphysicsres	= 0;                   //=���ԶԷ��շ�:#d1-%
	 m_Currentcoldres	= 0;                       //=���ԶԷ�����:#d1-%
	 m_Currentallres	= 0; 
	 m_Currentnopkvalue	= 0;                   //=������PKֵ����:#d1+%
	 m_Currentbossdamage	= 0;                   //=�Իƽ�boss�����˺�<color=orange>#d1+%<color>
	 m_Currentelementsenhance	= 0;               //=����ǿ��ֵ��#d1-�㡣ǿ����������еĿ���Ч��
     m_Currentelementsresist	= 0;               //=��������ֵ��#d1-�㡣������������еĿ���Ч��
	 m_Currentskillenhance=0;                    //���ܵļӳ�
//	  ZeroMemory(m_CurrentSkillEnhance,sizeof(m_CurrentSkillEnhance));
	/* _EnhanceInfo::iterator it;
	 for( it = nEnhanceInfo.begin(); it != nEnhanceInfo.end(); ++it)
	 {
		 it->second.nSkillIdx = 0;
		 it->second.nEnhance = 0;
	 }*/
	 nEnhanceInfo.clear();

	 m_CurrentFullManaskillenhance=0;
	 m_CurrentautoReviverate=0;                  //�������
	 m_CurrentAddPhysicsDamageP= 0;     // ��ǰ�������հٷֱ�
	 m_CurrentAddFireDamagev= 0;        // ��ǰ��������
	 m_CurrentAddColdDamagev= 0;        // ��ǰ��������� 
	 m_CurrentAddLighDamagev= 0;        // ��ǰ�������׵�
	 m_CurrentAddPoisonDamagev= 0;      // ��ǰ�����ⶾ��
	 m_CurrentAddmagicphysicsDamage= 0; // ��ǰ�������յ�
	 m_CurrentAddmagicphysicsDamageP= 0;// ��ǰ�������հٷֱ�
	 m_CurrentAddmagicColdDamagicv= 0;  // ��ǰ�����ڱ���
	 m_CurrentAddmagicFireDamagicv= 0;  // ��ǰ�����ڻ��  
	 m_CurrentAddmagicLightDamagicv= 0; // ��ǰ�������׵�
     m_CurrentAddmagicPoisonDamagicv= 0;// ��ǰ�����ڶ���
	 m_CurrentPoisondamagereturnV=0;
	 m_CurrentPoisondamagereturnP=0;
	 m_Currentbaopoisondmax_p=0;
	 m_CurrentReturnskillp=0;
	 m_CurrentIgnoreskillp=0;
	 m_CurrentReturnresp=0;
	 m_CurrentCreatnpcv=0;
	 m_CurrentAllJiHuo=0; //�Ƿ�ȫ����
	 m_CurrentCreatStatus=0;
	 m_CurrentAttackRatingEnhancep=0;
	 m_CurrentAttackRatingEnhancev=0;
	 m_CurrentIgnorenAttacRating  =0;
	 m_Me2metaldamage_p=0;              //=�Խ�ϵ�˺����ӣ�#d1+%
	 m_Metal2medamage_p=0;              //=�������Խ�ϵ���˺���#d1-%
	 m_Me2wooddamage_p=0;              //=��ľϵ�˺����ӣ�#d1+%
	 m_Wood2medamage_p=0;              //=��������ľϵ���˺���#d1-%
	 m_Me2waterdamage_p=0;              //=��ˮϵ�˺����ӣ�#d1+%
	 m_Water2medamage_p=0;              //=��������ˮϵ���˺���#d1-%
	 m_Me2firedamage_p=0;              //=�Ի�ϵ�˺����ӣ�#d1+%
	 m_Fire2medamage_p=0;              //=�������Ի�ϵ���˺���#d1-%
	 m_Me2earthdamage_p=0;              //=����ϵ�˺����ӣ�#d1+%
	 m_Earth2medamage_p=0;              //=����������ϵ���˺���#d1-%
     m_CurrentStunRank_p=0;
	 m_Staticmagicshield_p=0;
	 ReCalcNpcEquip(nWonerIndex,nEquipPlace); //���¼������ϵ�װ��
	 //ReCalcState(nIsLoign); //���¼��㼼�ܵ�״̬
}

void KNpc::ReCalcNpcEquip(int nWonerIndex,int nEquipPlace)
{
	
  if (nWonerIndex<=0)	//������˲����ڵĻ�
		return;

  if (nEquipPlace==-1)
  {
	  int nNsta,nNstb,nNstc,nNstd,nNste;
	      g_GameSetTing.GetInteger("FuMoNpcSet","Mo_0",0,&nNsta);
	      g_GameSetTing.GetInteger("FuMoNpcSet","Mo_1",0,&nNstb);
	      g_GameSetTing.GetInteger("FuMoNpcSet","Mo_2",0,&nNstc);
	      g_GameSetTing.GetInteger("FuMoNpcSet","Mo_3",0,&nNstd);
	      g_GameSetTing.GetInteger("FuMoNpcSet","Mo_4",0,&nNste);
	  
	  if (m_NpcSettingIdx ==nNsta)
	  {//��
		 nEquipPlace=53;
	  }
	  else if (m_NpcSettingIdx ==nNstb)
	  {//ľ
		  nEquipPlace=58;
	  }
	  else if (m_NpcSettingIdx ==nNstc)
	  {//ˮ
		  nEquipPlace=63;
	  }
	  else if (m_NpcSettingIdx ==nNstd)
	  {//��
		  nEquipPlace=68;
	  }
	  else if (m_NpcSettingIdx ==nNste)
	  {//��
		  nEquipPlace=73;
	  }
  }
 //��53-57 ľ58-62 ˮ63-67 ��68-72 ��73-77
  int i;
  if (53<=nEquipPlace && nEquipPlace<=57)
  {//��
	for (i = 53; i < 58; ++i)
	{
		if (!UpMagicAttrib(i,nWonerIndex))
			  continue;
	}
  }
  else if (58<=nEquipPlace && nEquipPlace<=62)
  {//ľ
	  for (i = 58; i < 63; ++i)
	  {
		  if (!UpMagicAttrib(i,nWonerIndex))
			  continue;
	  }
  }
  else if (63<=nEquipPlace && nEquipPlace<=67)
  {//ˮ
	  for (i = 63; i < 68; ++i)
	  {
		  if (!UpMagicAttrib(i,nWonerIndex))
			  continue;
	  }
  }
  else if (68<=nEquipPlace && nEquipPlace<=72)
  {//��
	  for (i = 68; i < 73; ++i)
	  {
		  if (!UpMagicAttrib(i,nWonerIndex))
			  continue;
	  }
  }
  else if (73<=nEquipPlace && nEquipPlace<=77)
  {//��
	  for (i = 73; i < 78; ++i)
	  {
		  if (!UpMagicAttrib(i,nWonerIndex))
			  continue;
	  }
  }

}

BOOL KNpc::UpMagicAttrib(int i,int nWonerIndex)
{
	int nIdx =Player[nWonerIndex].m_ItemList.GetItemBox(i);
	
	if (!nIdx)
	   return FALSE;
	
	int nActive = 3;//m_ItemList.GetEquipEnhance(i);  //��ȡ���������
	
	//����ƥ
	//	if (i != itempart_horse || Npc[m_nIndex].m_bRideHorse)  //������Ĳ�λ �� ������
	{
		int nActiveInfo[2];
		
		ZeroMemory(nActiveInfo,sizeof(nActiveInfo));
		
		if (Item[nIdx].IsBlue())
		{//�������װ
			nActiveInfo[0] = 1;
			nActiveInfo[1] = Item[nIdx].GetIsWhere();
			
		} 
		else if  (Item[nIdx].IsPurple())
		{//�������װ
			nActiveInfo[0] = 2;
			nActiveInfo[1] = Item[nIdx].GetIsWhere();
		} 
		else if  (Item[nIdx].IsGold())
		{//����ǻ�װ
			nActiveInfo[0] = 3;
			nActiveInfo[1] = Item[nIdx].GetLevel();//�ȼ�
		} 
		else
		{
			nActiveInfo[0] = 0;
			nActiveInfo[1] = 0;
		}
		
		if (Item[nIdx].GetIsCanUse()==1)   //�ǿ���ʹ�õ�
		{     
			Item[nIdx].ApplyMagicAttribToNPC(&Npc[m_Index], nActive,nActiveInfo);
		}
		else if (Item[nIdx].GetIsCanUse()==0 && Item[nIdx].GetGenre()==item_equip && Item[nIdx].GetDetailType()==equip_mask) 
		{//���
			Item[nIdx].ApplyMagicAttribToNPC(&Npc[m_Index], nActive,nActiveInfo);
		}
		
		}
	return TRUE;
}

//����Ϊ�⻷����
void KNpc::SetAuraSkill(int nSkillID)
{
	int nCurLevel = 0;

	if (nSkillID>g_SkillManager.GetSkillCount())
	{
		m_ActiveAuraID = 0;
		m_ActiveAuraIndex =0;
		return;
	}

	if (nSkillID <= 0 || nSkillID >= MAX_SKILL) 
    {
        nSkillID = 0;
    }
	else
	{
		nCurLevel = m_SkillList.GetCurrentLevel(nSkillID);
		if (nCurLevel <= 0) 
        {
            nSkillID = 0;
        }
		else
		{
			//_ASSERT(nSkillID < MAX_SKILL && nCurLevel < MAX_SKILLLEVEL);
		   if (nSkillID < MAX_SKILL && nCurLevel < MAX_SKILLLEVEL)
		   {
			   KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nSkillID, nCurLevel);
               if (!pOrdinSkill || !pOrdinSkill->IsAura())
			   { 
				 nSkillID  = 0;
			   }
		   }
		}
	}

	m_ActiveAuraID = nSkillID;
	m_ActiveAuraIndex = m_SkillList.FindSame(m_ActiveAuraID);

	SKILL_CHANGEAURASKILL_COMMAND ChangeAuraMsg;
	ChangeAuraMsg.ProtocolType = c2s_changeauraskill;
	ChangeAuraMsg.m_nAuraSkill = m_ActiveAuraID;
	if (g_pClient)
		g_pClient->SendPackToServer(&ChangeAuraMsg, sizeof(SKILL_CHANGEAURASKILL_COMMAND));
}

BOOL KNpc::SetPlayerIdx(int nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_PLAYER)
		return FALSE;

	if (m_Kind != kind_player)
		return FALSE;

	m_nPlayerIdx = nIdx;
	return TRUE;
}

// -------------------------------------------------------------------------
// ����		: KNpc::TestMovePos
// ����		: �����Ƿ��ܹ�ֱ�ߵ���Ŀ���
// ����ֵ	: BOOL �Ƿ��ܵ���
// ����		: INT& nMpsX, INT& nMpsY	����Ŀ����ͼ���ꣻ	����ʵ�ʿ��Ե��������
// ����		: INT& nLength				����������󳤶ȣ�		����ʵ�ʳ���
// ����		: INT  nSpeed				�����ƶ��ٶ�
// ����		: BOOL bCanJumpOver			�����Ƿ����Խ��һ�����ϰ���������Npc֮��ģ�
// ����		: FanZai
// ��ע		: �Ӿɵ�NewJump()�޸Ķ���
// -------------------------------------------------------------------------

BOOL KNpc::TestMovePos(IN OUT INT& nMpsX, IN OUT INT& nMpsY, IN OUT INT& nLength, INT nSpeed, BOOL bCanJumpOver)
{
	if (nMpsX < 0 || nMpsY < 0)
	{
		return FALSE;
	}
	if (nSpeed <= 0 || nSpeed > defLOGIC_CELL_WIDTH)
	{
		//_ASSERT(FALSE);	// ��ô�������أ�
		return FALSE;
	}
	
	if (nLength <= 0)
	{
		//_ASSERT(FALSE);	// ������󳤶�0�������ƶ�
		return FALSE;
	}
	
	INT nX, nY,nMap;
	GetMpsPos(&nX, &nY,&nMap);
	
	INT	nTargetX	= nMpsX - nX;
	INT	nTargetY	= nMpsY - nY;	// Ŀ���ƫ������
	
	INT	nTargetLength =				// Ŀ���ƫ�ƾ���
		(INT)sqrt((FLOAT)(nTargetX * nTargetX + nTargetY * nTargetY));
	
	if (!nTargetLength)				// �������0�����Ҫ������
		return FALSE;
	
	INT	nDx	= (nTargetX * nSpeed << 10) / nTargetLength;
	INT	nDy	= (nTargetY * nSpeed << 10) / nTargetLength;	// ÿ��ƫ�����꣨�Ŵ�1024����
	
	if (nTargetLength > nLength)
		nTargetLength = nLength;	// ʵ����Ծ����
	
	INT	nBarrierTestCounts	= nTargetLength / nSpeed;		// Ҫ���ԵĲ���
	
	INT	nSuccessCounts = 0;			// �ɹ��Ĳ�����ֻ�����ͣ�ĵ㡣����Խ������������ͣ�ĵ㲻�㣩
	
	INT nTestEX = nX << 10;
	INT nTestEY = nY << 10;			// ��ǰ���Ե�Ŵ�1024������
	INT nTestX, nTestY;				// ��ǰ���Ե��ͼ����
	Obstacle_Kind eTestBarrier;		// ��ǰ���Ե��ϰ���Ϣ
	
	for (INT i = 1; i <= nBarrierTestCounts; i++)
	{
		nTestX	= (nTestEX += nDx) >> 10;
		nTestY	= (nTestEY += nDy) >> 10;
		eTestBarrier	= (Obstacle_Kind)SubWorld[m_SubWorldIndex].GetBarrier(nTestX, nTestY);
		switch (eTestBarrier)
		{
		case Obstacle_NULL:
			nSuccessCounts = i;
			continue;
		case Obstacle_Normal:
		case Obstacle_Fly:
			break;
		case Obstacle_Jump:
		case Obstacle_JumpFly:
			if (bCanJumpOver)
				continue;
			else
				break;
		case 0xff:
			//_ASSERT(FALSE);	// ���곬����ͼ��Χ����
			return FALSE;
			break;
		default:
			//_ASSERT(FALSE);	// �����ϰ������ˣ���
			return FALSE;
		}
		break;
	}
	nLength	= nSuccessCounts * nSpeed;
	nMpsX	= nX + ((nDx * nSuccessCounts) >> 10);
	nMpsY	= nY + ((nDy * nSuccessCounts) >> 10);
	
	return TRUE;
}

int	KNpc::CheckMaps(char *nKey,int nMapIdx)
{
	int nRow = g_ForbitMap.GetHeight()+1,nReg=0;
	
	for (int i=2;i<nRow;++i)
	{
		int nMaps =0;
		g_ForbitMap.GetInteger(i,nKey,0,&nMaps);
		if (nMapIdx==nMaps)
		{
			nReg =1;
			break;
		}
	}
	return nReg;     
}


int	KNpc::CheckAllItem(int nKeyCol,int nGen,int nDetail,int nPart)
{
	int nRow = g_ForbitMap.GetHeight()+1,nReg=0;
	
	for (int i=2;i<nRow;++i)
	{
		int nMaps[3];
		nMaps[0]=0;
		nMaps[1]=0;
		nMaps[2]=0;

		g_ForbitMap.GetInt3(i,nKeyCol,nMaps);

		if (nMaps[2]==-1)
			continue;

		if (nMaps[0]==nGen && nMaps[1]==nDetail && nMaps[2]==nPart)
		{
			nReg =1;
			break;
		}
	}
	return nReg;     
}
//#include "scene/KScenePlaceC.h"
//�������� Ѫ�� ��
int KNpc::PaintInfo(int nHeightOffset, bool bSelect, int nFontSize, DWORD dwBorderColor)  //������Ϣ
{
	int nMpsX, nMpsY,nmMap;
	    GetMpsPos(&nMpsX, &nMpsY,&nmMap);
	///nFontSize=12;
	DWORD	dwColor;
	int nHeightOff = nHeightOffset + nFontSize + 1;
	int  nHigthOff = 0;
	
	if (relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
		(m_Kind == kind_player || m_Kind == kind_partner) && m_Hide.nTime > 0 )//������ежԹ�ϵ��������ʱ����ڵ� ����ʾ
	{
		return 0;
	}

	if (m_Kind == kind_player) //�����Ӫ��ɫ��ʾ
	{
		switch(m_CurrentCamp)
		{
		case camp_begin: 
			dwColor = 0xffffffff;   //����
			break;
		case camp_justice:
			dwColor = 0xff000000 | (255 << 16) | (168 << 8) | 94;
			break;
		case camp_evil:
			dwColor = 0xff000000 | (255 << 16) | (146 << 8) | 255;
			break;
		case camp_balance:
			dwColor = 0xff000000 | (85 << 16) | (255 << 8) | 145;
			break;
		case camp_free:
			dwColor = 0xff000000 | (255 << 16);  //ɱ��
			break;
		case camp_blue:
			dwColor = TGetColor("74,74,255");
			break;
		case camp_green:
			dwColor = TGetColor("0,249,0");  //ɱ��
			break;
		default:
			dwColor = 0xffff00ff;
			break;
		}
//		char  xinxi[128];
//      t_sprintf(xinxi,"[CQ:%d,SH:%d,PE:%d,LU:%d,PF:%d,JY:%d,JQ:%d,����:%d,JN:%d,dwID:%d,TYPE:%d]",m_BtnFlag,m_WarShuishou,m_nPeopleIdx,IsLuKey,m_PifengType,IsJinYan,IsJinQian,m_CurrentLife,m_ActiveSkillID,m_dwID,this->m_cGold.GetGoldType());  
//		g_pRepresent->OutputText(nFontSize, xinxi, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(xinxi)/4, nMpsY-24, dwColor, 0, nHeightOff, dwBorderColor);
//		nHeightOffset += nFontSize + 1;
	
		char	szString[128]={0};		
        t_sprintf(szString,"%s",Name); 

		if (m_IsInCity==1)
		{
           strcat(szString, "���");
		   //strcat(szString, m_ScriptPicPath);
		}
		else if (m_IsInCity==11)
		{
			strcat(szString, "�");
			//strcat(szString, m_ScriptPicPath);
		}
		else if (m_IsInCity==162)
		{
			strcat(szString, "����");
			//strcat(szString, m_ScriptPicPath);
		}
		else if (m_IsInCity==37)
		{
			strcat(szString, "�");
			//strcat(szString, m_ScriptPicPath);
		}
		else if (m_IsInCity==78)
		{
			strcat(szString, "����");
			//strcat(szString, m_ScriptPicPath);
		}
		else if (m_IsInCity==80)
		{
			strcat(szString, "����");
			//strcat(szString, m_ScriptPicPath);
		}
		else if (m_IsInCity==176)
		{
			strcat(szString, "�");
			//strcat(szString, m_ScriptPicPath);
		}
		/*else
		{	char nMsg[32];
		    t_sprintf(nMsg,"����:%d",m_nPeopleIdx);
			strcat(szString, nMsg);	 //m_WeaponType
		}*/

		//if (m_FreezeState.nTime || m_PoisonState.nTime || m_ConfuseState.nTime || m_StunState.nTime || m_Hide.nTime || m_randmove.nTime)
		 if (m_FreezeState.nTime || m_PoisonState.nTime || m_ConfuseState.nTime || m_StunState.nTime || m_Hide.nTime || m_randmove.nTime)
		{
			strcat(szString, "(");
			if (m_FreezeState.nTime)
				strcat(szString, "��");
			//if (m_PoisonState.nTime)
			if (m_PoisonState.nTime)
				strcat(szString, "��");
			if (m_ConfuseState.nTime)
				strcat(szString, "��");
			if (m_StunState.nTime)
				strcat(szString, "��");
			if (m_randmove.nTime)
				strcat(szString, "��");
			if (m_Hide.nTime)
			   strcat(szString, "��");
			strcat(szString, ")");
		}

  /*    if ( m_PifengType>0)	// ����ƺ�
		g_pRepresent->OutputText(nFontSize, szString, KRF_ZERO_END, nMpsX +20- nFontSize * g_StrLen(Name)/4, nMpsY-10, dwColor, 0, nHeightOff, dwBorderColor); //+100
      else
        g_pRepresent->OutputText(nFontSize, szString, KRF_ZERO_END, nMpsX- nFontSize * g_StrLen(Name)/4, nMpsY-10, dwColor, 0, nHeightOff, dwBorderColor); //+100   		
 */

	   nMpsY=nMpsY-10;

       nHeightOffset += nFontSize + 1;

       char Livename[40]={0}; 

       /*if (m_GameliveTime>0)
	   { 
		  dwColor=TGetColor("255,255,255");
		  dwBorderColor=TGetColor("0,0,0");
          int nHour=0,nMin=0,nSec=0;
		  
		  nHour = m_GameliveTime/3600;//TakeTrader(m_GameliveTime,3600);  //Сʱ
		  
		  nMin  = (m_GameliveTime-nHour*3600)/60;//TakeTrader(m_GameliveTime-nHour*3600,60); //ʣ��������60�룬�ͼ����
		  
		  if (nMin>0)
		  {
			  nSec  = (m_GameliveTime-nHour*3600-nMin*60);  //��
			  
		  }
		  else
			  nSec  = m_GameliveTime-nHour*3600;
		  	  
		  wt_sprintf(Livename,"(ʣ��ʱ��:%dСʱ%d��%d��)",nHour,nMin,nSec);
		  g_pRepresent->OutputText(nFontSize,Livename, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name)/4-50, nMpsY+150, dwColor, 0, nHeightOff, dwBorderColor);
		  nHeightOffset += nFontSize + 1;
	   }
	   else	*/
	 /* if (m_MapUseModel.nTime>0)
	   {
		   dwColor=TGetColor("255,255,255");
		   dwBorderColor=TGetColor("0,0,0");
		   if (m_MapUseModel.nTime==1)
			   wt_sprintf(Livename,"(ʣ����:%d��)",Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentXu());
		   else	if (m_MapUseModel.nTime==2)
			   wt_sprintf(Livename,"(ʣ������:%d��)",Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney());
		   else	if (m_MapUseModel.nTime==3)
			   wt_sprintf(Livename,"(ʣ�ྭ��:%d��)",Player[CLIENT_PLAYER_INDEX].m_nExp);
		   else	if (m_MapUseModel.nTime==4)
			   wt_sprintf(Livename,"(ʣ�฻Դ:%d��)",Player[CLIENT_PLAYER_INDEX].m_cFuYuan.GetFuYuanValue());
		   else	if (m_MapUseModel.nTime==5)
			   wt_sprintf(Livename,"(ʣ������:%d��)",Player[CLIENT_PLAYER_INDEX].m_cRepute.GetReputeValue());
		   
		   g_pRepresent->OutputText(nFontSize,Livename, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(Name)/4-25, nMpsY+150, dwColor, 0, nHeightOff, dwBorderColor);
		   nHeightOffset += nFontSize + 1;
	   }
	   else
	   { 
		    ZeroMemory(Livename,sizeof(Livename));
	   }*/ 

		//------------------------------------------
		if ( m_PifengType>0)	// ����ƺ�
		{	
			nHeightOffset +=8;
		    if (m_Doing == do_sit && !m_MaskType)
				nHeightOffset +=6;
			else if	(m_MaskType)
				nHeightOffset +=-6;
			
			PaintPifeng(m_PifengType,nMpsX -40- nFontSize * g_StrLen(Name)/4,nMpsY-GetNpcPate()-nHeightOffset,7);	//��ʾspr�ƺ�  nMpsX-30	
		}
		//-------------------------------------------
      //  int bb= Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ExBoxId;
		char *nPath=NULL;
	 	if(m_btRankFFId > 0 && m_btRankFFId!=88 && m_btRankFFId!=99)	//��ʾspr�ƺ�
		{	
			PaintHonor(m_btRankFFId,nMpsX,nMpsY,6);	         //��ʾspr�ƺ�	
			nPath="\\spr\\skill\\special\\skill_heart.spr";  //��������ת
			SetClientSpr(nPath,nMpsX,nMpsY+50,0,8);
		}

		if (m_AutoplayId ==1)
		{
			//this->SetFrameSpr("\\spr\\Ui3\\��������\\state_autofight.spr",0, 120,GetNpcPate(),100);
			nPath="\\spr\\Ui3\\��������\\state_autofight.spr";
			SetClientSpr(nPath,nMpsX, nMpsY-GetNpcPate()-nHeightOff*1.8,0,0);
		}
		//ת���Ĺ⻷
		if (nReBorn>0)
		{
			//nPath="\\spr\\skill\\others\\�ػ�.spr";
			//SetClientSpr(nPath,nMpsX - nFontSize * g_StrLen(Name)/4+30, nMpsY-GetNpcPate()+80,0,6);
			//lanyue.spr  zq_sl_003.spr  
			//nPath="\\spr\\skill\\zhongqiu\\zq_sl_003.spr";  \\spr\\skill\\special\\skill_heart.spr
			nPath="\\spr\\skill\\others\\totempole_phoenix.spr"; //���ӡ
			//nPath="\\spr\\skill\\special\\skill_heart.spr";  //��������ת
			//nPath="\\spr\\skill\\others\\totempole_mask.spr";
			//nPath="\\spr\\skill\\task\\randomtask.spr"; //��̾��
			SetClientSpr(nPath,nMpsX,nMpsY-GetNpcPate(),0,7);	 //
		}
		//�񽫵�״̬ --����
		/*if (m_nFuMoNum[0].nNpcSetings >0 || m_nFuMoNum[1].nNpcSetings >0||m_nFuMoNum[2].nNpcSetings >0||m_nFuMoNum[3].nNpcSetings >0||m_nFuMoNum[4].nNpcSetings >0)
		{	
			nPath="\\spr\\skill\\zhongqiu\\zq_sl_003.spr";
			SetClientSpr(nPath,nMpsX,nMpsY-GetNpcPate(),0,7);	 //-nHeightOff
		}  */

		if (TongName[0])  //���� ��ʾ���ɳƺ�
		{
			char szTong[64];
			//dwColor=GetColor("255,255,255");
            //dwBorderColor=GetColor("255,255,0");
			dwBorderColor=TGetColor("0,0,0");

            strcpy(szTong,TongName);

			switch(m_nFigure)  //���ְλ
			{
			case enumTONG_FIGURE_MEMBER://����
				{
                if (m_IsWarCity==78)
                {
				   ZeroMemory(szTong,sizeof(szTong));
				   strcpy(szTong,"������");
		           strcat(szTong,TongName);
				}
				else if (m_IsWarCity==1)
				{
					ZeroMemory(szTong,sizeof(szTong));
					strcpy(szTong,"���衤");
		            strcat(szTong,TongName);
				}
				else if (m_IsWarCity==162)
				{
					ZeroMemory(szTong,sizeof(szTong));
					strcpy(szTong,"����");
					strcat(szTong,TongName);
				}
				else if (m_IsWarCity==11)
				{
					ZeroMemory(szTong,sizeof(szTong));
					strcpy(szTong,"�ɶ���");
					strcat(szTong,TongName);
				}
				else if (m_IsWarCity==53)
				{
					ZeroMemory(szTong,sizeof(szTong));
					strcpy(szTong,"���ꡤ");
					strcat(szTong,TongName);
				}
				else if (m_IsWarCity==176)
				{
					ZeroMemory(szTong,sizeof(szTong));
					strcpy(szTong,"�ٰ���");
					strcat(szTong,TongName);
				}
				else if (m_IsWarCity==80)
				{
					ZeroMemory(szTong,sizeof(szTong));
					strcpy(szTong,"���ݡ�");
					strcat(szTong,TongName);
				}
				else if (m_IsWarCity==28)
				{
					ZeroMemory(szTong,sizeof(szTong));
					strcpy(szTong,"�꾩��");
					strcat(szTong,TongName);
				}
			/*	if (m_WarFucheng==1)
                    strcat(szTong,"(������)");
				else if (m_WarZuoHu==2)
					strcat(szTong,"(�󻤷�)");
				else if (m_WarYouHu==3)
					strcat(szTong,"(�һ���)");
				else if (m_WarTaishi==4)
					strcat(szTong,"(̫ʷ��)");
				else if (m_WarZhongShu==5)
					strcat(szTong,"(������)");
				else if (m_WarShangShu==6)
					strcat(szTong,"(������)");
				else if (m_WarMiShu==7)
					strcat(szTong,"(������)");
				else if (m_WarTaiLe==8)
					strcat(szTong,"(̫����)");
                else */
				strcat(szTong,"(����)");

				//dwColor=GetColor("255,255,0");
                //dwBorderColor=GetColor("255,0,0");
				dwBorderColor=TGetColor("0,0,0");
				}
				break;
			case enumTONG_FIGURE_MANAGER:  // �ӳ�
				{
				 if (m_IsWarCity==78)
				 {
					ZeroMemory(szTong,sizeof(szTong));
					strcpy(szTong,"������");
					strcat(szTong,TongName);
				 }
				 else if (m_IsWarCity==1)
				 {
					ZeroMemory(szTong,sizeof(szTong));
					strcpy(szTong,"���衤");
					strcat(szTong,TongName);
				 }
				 else if (m_IsWarCity==162)
				 { 
				   ZeroMemory(szTong,sizeof(szTong));
				   strcpy(szTong,"����");
				   strcat(szTong,TongName);
				 } 
				 else if(m_IsWarCity==11)
				 { 
				  ZeroMemory(szTong,sizeof(szTong));
				  strcpy(szTong,"�ɶ���");
				  strcat(szTong,TongName);
				 } 
				 else if (m_IsWarCity==53)
				 {   
				  ZeroMemory(szTong,sizeof(szTong));
				  strcpy(szTong,"���ꡤ");
				  strcat(szTong,TongName);
				 } 
				 else if (m_IsWarCity==176)
				 {  
				  ZeroMemory(szTong,sizeof(szTong));
				  strcpy(szTong,"�ٰ���");
				  strcat(szTong,TongName);
				 } 
			     else if (m_IsWarCity==80)
				 { 
				  ZeroMemory(szTong,sizeof(szTong));
				  strcpy(szTong,"���ݡ�");
				  strcat(szTong,TongName);
				 }
				 else if (m_IsWarCity==28)
				 { 
				  ZeroMemory(szTong,sizeof(szTong));
				  strcpy(szTong,"�꾩��");
				  strcat(szTong,TongName);
				 }
				/*if (m_WarFucheng==1)
                    strcat(szTong,"(������)");
				else if (m_WarZuoHu==2)
					strcat(szTong,"(�󻤷�)");
				else if (m_WarYouHu==3)
					strcat(szTong,"(�һ���)");
				else if (m_WarTaishi==4)
					strcat(szTong,"(̫ʷ��)");
				else if (m_WarZhongShu==5)
					strcat(szTong,"(������)");
				else if (m_WarShangShu==6)
					strcat(szTong,"(������)");
				else if (m_WarMiShu==7)
					strcat(szTong,"(������)");
				else if (m_WarTaiLe==8)
					strcat(szTong,"(̫����)");
                else */
				strcat(szTong,"(�ӳ�)");

				//dwColor=GetColor("255,255,0");
                //dwBorderColor=GetColor("255,0,0");
				dwBorderColor=TGetColor("0,0,0");
				}
				break;
			case enumTONG_FIGURE_DIRECTOR:  // ����
				{
					if (m_IsWarCity==78)
					{
						ZeroMemory(szTong,sizeof(szTong));
						strcpy(szTong,"������");
						strcat(szTong,TongName);
					}
					else if (m_IsWarCity==1)
					{
						ZeroMemory(szTong,sizeof(szTong));
						strcpy(szTong,"���衤");
						strcat(szTong,TongName);
					}
					else if (m_IsWarCity==162)
					{ 
						ZeroMemory(szTong,sizeof(szTong));
						strcpy(szTong,"����");
						strcat(szTong,TongName);
					} 
					else if(m_IsWarCity==11)
					{ 
						ZeroMemory(szTong,sizeof(szTong));
						strcpy(szTong,"�ɶ���");
						strcat(szTong,TongName);
					} 
					else if (m_IsWarCity==53)
					{   
						ZeroMemory(szTong,sizeof(szTong));
						strcpy(szTong,"���ꡤ");
						strcat(szTong,TongName);
					} 
					else if (m_IsWarCity==176)
					{  
						ZeroMemory(szTong,sizeof(szTong));
						strcpy(szTong,"�ٰ���");
						strcat(szTong,TongName);
					} 
					else if (m_IsWarCity==80)
					{ 
						ZeroMemory(szTong,sizeof(szTong));
						strcpy(szTong,"���ݡ�");
						strcat(szTong,TongName);
					}
					else if (m_IsWarCity==28)
					{ 
						ZeroMemory(szTong,sizeof(szTong));
						strcpy(szTong,"�꾩��");
						strcat(szTong,TongName);
					}
				/*if (m_WarFucheng==1)
                    strcat(szTong,"(������)");
				else if (m_WarZuoHu==2)
					strcat(szTong,"(�󻤷�)");
				else if (m_WarYouHu==3)
					strcat(szTong,"(�һ���)");
				else if (m_WarTaishi==4)
					strcat(szTong,"(̫ʷ��)");
				else if (m_WarZhongShu==5)
					strcat(szTong,"(������)");
				else if (m_WarShangShu==6)
					strcat(szTong,"(������)");
				else if (m_WarMiShu==7)
					strcat(szTong,"(������)");
				else if (m_WarTaiLe==8)
					strcat(szTong,"(̫����)");
                else */
				     strcat(szTong,"(����)");
				//dwColor=GetColor("255,255,0");
                //dwBorderColor=GetColor("255,0,0");
				dwBorderColor=TGetColor("0,0,0");
				}
				break;
			case enumTONG_FIGURE_MASTER:  // ����
				{
                if (m_IsWarCity==78)
				{
					ZeroMemory(szTong,sizeof(szTong));
					strcpy(szTong,"������");
				 	strcat(szTong,TongName);
					strcat(szTong,"(����)");
				}  
                else if (m_IsWarCity==1)
				{ 
	             ZeroMemory(szTong,sizeof(szTong));
	             strcpy(szTong,"���衤");
	             strcat(szTong,TongName);
				 strcat(szTong,"(����)");
				} 
                else if (m_IsWarCity==162)
				{  
	             ZeroMemory(szTong,sizeof(szTong));
	             strcpy(szTong,"����");
	             strcat(szTong,TongName);
				 strcat(szTong,"(����)");
				}  
                else if(m_IsWarCity==11)
				{  
	             ZeroMemory(szTong,sizeof(szTong));
	             strcpy(szTong,"�ɶ���");
	             strcat(szTong,TongName);
				 strcat(szTong,"(����)");
				}  
                else if (m_IsWarCity==53)
				{    
	             ZeroMemory(szTong,sizeof(szTong));
	             strcpy(szTong,"���ꡤ");
	             strcat(szTong,TongName);
				 strcat(szTong,"(����)");
				}  
                else if (m_IsWarCity==176)
				{   
	            ZeroMemory(szTong,sizeof(szTong));
	             strcpy(szTong,"�ٰ���");
	             strcat(szTong,TongName);
				 strcat(szTong,"(����)");
				}  
                else if (m_IsWarCity==80)
				{  
	             ZeroMemory(szTong,sizeof(szTong));
	             strcpy(szTong,"���ݡ�");
	             strcat(szTong,TongName);
				 strcat(szTong,"(����)");
				} 
                else if (m_IsWarCity==28)
				{  
	             ZeroMemory(szTong,sizeof(szTong));
	             strcpy(szTong,"�꾩��");
	             strcat(szTong,TongName);
				 strcat(szTong,"(����)");
				}
				else
				{
				  strcat(szTong,"(����)");
				}
				//dwColor=GetColor("255,255,0");
                //dwBorderColor=GetColor("255,0,0");
				dwBorderColor=TGetColor("0,0,0");
				}
				break;
			default:
				dwColor=TGetColor("255,255,255");
				break;
			}

			if (!m_bIsHideTong)
			{
//			   g_pRepresent->OutputText(nFontSize, szTong, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(szTong) / 4, nMpsY-24, dwColor, 0, nHeightOff, dwBorderColor);
			   nHeightOffset += nFontSize + 1;
			}
			else
			{
				ZeroMemory(TongName,sizeof(TongName));
			}
		}

		char PlayerRank[32]={0};

		if  (m_CurNpcTitle>0)
		{
			int nVal=0;
			char nClorVal[32]={0};
			dwColor = 0xffff00ff;
			dwBorderColor=TGetColor("0,0,0");
			g_PlayerTitle.GetString(m_CurNpcTitle+1,"Color","",nClorVal,sizeof(nClorVal));
			if (strstr(nClorVal,","))
				dwColor=TGetColor(nClorVal);
			else
				dwColor=atoi(nClorVal);
			
			if (dwColor<=0)
				dwColor = 0xffff00ff;
			
			g_PlayerTitle.GetString(m_CurNpcTitle+1,"TitleName","",PlayerRank,sizeof(PlayerRank));
			
			int nYY = nMpsY;

			if (TongName[0])
			{
				nYY -= 50;
			}
			else
				nYY -= 26;
			
//			g_pRepresent->OutputText(nFontSize, PlayerRank, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(PlayerRank) / 4, nYY, dwColor,0, nHeightOff, dwBorderColor);
			nHeightOffset += nFontSize + 1;
		}
		else if  (m_NpcTitle>0)
		{

			int nVal=0;
			char nClorVal[32]={0};
			dwColor = 0xffff00ff;
			dwBorderColor=TGetColor("0,0,0");
			g_PlayerTitle.GetString(m_NpcTitle+1,"Color","",nClorVal,sizeof(nClorVal));
			if (strstr(nClorVal,","))
				dwColor=TGetColor(nClorVal);
			else
				dwColor=atoi(nClorVal);
			
			if (dwColor<=0)
				dwColor = 0xffff00ff;

			g_PlayerTitle.GetString(m_NpcTitle+1,"TitleName","",PlayerRank,sizeof(PlayerRank));

			int nYY = nMpsY;
			if (TongName[0])
			{
				nYY -= 50;
			}
			else
				nYY -= 26;
			//���Ե���
			/*char nMsg[16]={0};
			t_sprintf(nMsg,"��%d:%d",m_ChiBangType,m_Camp);
			strcat(PlayerRank,nMsg);*/
		
//			g_pRepresent->OutputText(nFontSize, PlayerRank, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(PlayerRank) / 4, nYY, dwColor,0, nHeightOff, dwBorderColor);
			nHeightOffset += nFontSize + 1;

			/*if(m_NpcTitle>0 && m_btRankFFId <=0)//û�����Գƺŵ�ʱ�����ʾ
			{	
				int nStateID=0;
				g_PlayerTitle.GetInteger(m_NpcTitle+1,"SpeicalGraphic",0,&nStateID);
				if (nStateID>0)
				{
					//int sprMpsX, sprMpsY,sprmMap;
					//GetMpsPos(&sprMpsX, &sprMpsY,&sprmMap);
					
					char nSpePath[128]={0},ncPart[32]={0};
					cTabFile.GetString(nStateID+1,"�ļ���","",nSpePath,sizeof(nSpePath));
					cTabFile.GetString(nStateID+1,"����","",ncPart,sizeof(ncPart));
					if (nSpePath)
						m_DataRes.SetClientSpr(nSpePath,nMpsX,nMpsY,0,0,ncPart,nStateID);
				}
				
			}*/ 
	
		}
		else if (m_btRankId>0) //��ʾ�ϵĳƺ�
		{
			//char szRankId[5];
			//itoa(m_btRankId, szRankId, 10); //int ת char
			g_RankTabSetting.GetString(m_btRankId, "RANKSTR", "", PlayerRank, sizeof(PlayerRank));
			dwColor = 0xffff00ff;
			dwBorderColor=TGetColor("0,0,0");
			int nYY = nMpsY;

			if (TongName[0])
			{
				nYY -= 50;
			}
			else
				nYY -= 26;


//			g_pRepresent->OutputText(nFontSize, PlayerRank, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(PlayerRank) / 4, nYY, dwColor,0, nHeightOff, dwBorderColor);
			nHeightOffset += nFontSize + 1;
		} 
		else
		{
			t_sprintf(PlayerRank,"������Ե");
			dwColor = TGetColor("15,249,68");//0xffff00ff;
			dwBorderColor=TGetColor("0,0,0");
			int nYY = nMpsY;
			if (TongName[0])
			{
				nYY -= 50;
			}
			else
				nYY -= 26;

//			g_pRepresent->OutputText(nFontSize, PlayerRank, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(PlayerRank) / 4, nYY, dwColor,0, nHeightOff, dwBorderColor);
			nHeightOffset += nFontSize + 1;
		}

		if (m_MaskType > 0)	//����ߵ�ʱ�����ʾ
		{
			if (m_bRideHorse)  //��������� ���ƴ���
			{
				char* sOut = "\\Spr\\UI3\\Npc\\ActionIcon_RideHorse.spr";//Spr
				SetClientSpr(sOut,nMpsX + (nFontSize * g_StrLen(Name) / 4),nMpsY-nHeightOff*1.8,0,1);
			}
			else if (m_Doing == do_sit)
			{
				char* sOut = "\\Spr\\UI3\\Npc\\ActionIcon_Sit.spr";
				SetClientSpr(sOut,nMpsX + (nFontSize * g_StrLen(Name) / 4),nMpsY-nHeightOff*1.7,0,2);
				
			}
		}
		if (m_BaiTan) //��̯
		{
			int	nMpsX, nMpsY,nMmap;
			    GetMpsPos(&nMpsX, &nMpsY,&nMmap);
				char nstrName[32]={0};
		        GetstrInfo(STR_SHOP_NAME,nstrName);
			int nWid = nFontSize * g_StrLen(nstrName)/2 + 10;
			int nHei = nFontSize + 12;
			char*sOutm = "\\Spr\\Ui3\\��̯\\��̯ͷ��������.spr";
			KRUImage RUIconImageR;
			RUIconImageR.nType = ISI_T_SPR;
			RUIconImageR.Color.Color_b.a = 255;
			RUIconImageR.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
			RUIconImageR.uImage = 0;
			RUIconImageR.nISPosition = IMAGE_IS_POSITION_INIT;
			RUIconImageR.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
			strcpy(RUIconImageR.szImage, sOutm);
			RUIconImageR.oPosition.nX = nMpsX - nWid / 2;
			RUIconImageR.oPosition.nY = nMpsY - 35;
			RUIconImageR.oPosition.nZ = nHeightOffset;
			RUIconImageR.nFrame = 0;
			
			for (int i = 0; i < nWid;++i)
			{
				RUIconImageR.oPosition.nX = nMpsX - nWid / 2 + i;
//				g_pRepresent->DrawPrimitives(1, &RUIconImageR, RU_T_IMAGE, FALSE);
			}

			if (PlayerRank[0] || TongName[0])
			{
				dwColor = 0x00c3a94e;
				dwBorderColor=TGetColor("0,0,0");

				/*if (TongName[0] && PlayerRank[0]) //��ʾ ��̯���  �� �ǳƺ� Ϊ�յĻ�
					g_pRepresent->OutputText(nFontSize, nstrName, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(nstrName) / 4, nMpsY - 76, dwColor, 0, nHeightOff, dwBorderColor);
				else if (TongName[0]||PlayerRank[0])
					g_pRepresent->OutputText(nFontSize, nstrName, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(nstrName) / 4, nMpsY - 50, dwColor, 0, nHeightOff, dwBorderColor);
			*/
			}
			else
			{
				dwColor = 0x00c3a94e;
				dwBorderColor=TGetColor("0,0,0");
//				g_pRepresent->OutputText(nFontSize, nstrName, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(nstrName) / 4, nMpsY - 34, dwColor, 0, nHeightOff, dwBorderColor);
			}

			char*   sOut = "\\Spr\\Ui3\\��̯\\��̯ͷ��������.spr";
			SetClientSpr(sOut,nMpsX - nWid / 2 - 10,nMpsY+6,nHeightOffset + 23,4);
		            sOut = "\\Spr\\Ui3\\��̯\\��̯ͷ��������.spr";
			SetClientSpr(sOut,nMpsX + nWid / 2,nMpsY+6,nHeightOffset + 23,3);

			nHeightOffset += nFontSize + 1;
			//----------------------------------------------------------------------------------------------------------------------------

		}
	}
	else if (m_Kind == kind_dialoger)  //�Ի�NPC
	{ 
        char outname[64]="";           //��������

		if (m_GameliveTime>0)
		{
			dwColor=TGetColor("251,84,4");
            dwBorderColor=TGetColor("0,0,0");

			int nHour=0,nMin=0,nSec=0;
			
			nHour = m_GameliveTime/3600;//TakeTrader(m_GameliveTime,3600);  //Сʱ
		  
		    nMin  = (m_GameliveTime-nHour*3600)/60;//TakeTrader(m_GameliveTime-nHour*3600,60); //ʣ��������60�룬�ͼ����
			
			if (nMin>0)
			{
				nSec  = (m_GameliveTime-nHour*3600-nMin*60);  //��
				
			}
			else
				nSec  = m_GameliveTime-nHour*3600;
			char nstrName[64]={0};
		    GetstrInfo(STR_GUISHU_NAME,nstrName);

			t_sprintf(outname,nstrName);
//			g_pRepresent->OutputText(nFontSize,outname, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(outname)/4, nMpsY-57, dwColor, 0, nHeightOff, dwBorderColor);
			nHeightOffset += nFontSize + 1;

			t_sprintf(outname,"(���ʱ��:%dСʱ%d��%d��)",nHour,nMin,nSec);
//			g_pRepresent->OutputText(nFontSize,outname, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(outname)/4, nMpsY-30, dwColor, 0, nHeightOff, dwBorderColor);
			nHeightOffset += nFontSize + 1;
		}

		   dwColor=TGetColor("255,255,255");
           dwBorderColor=TGetColor("0,0,0");
	       t_sprintf(outname,"%s",Name);//m_NpcSettingIdx
//	       g_pRepresent->OutputText(nFontSize,outname, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(outname)/4, nMpsY-5, dwColor, 0, nHeightOff, dwBorderColor);
	 	   nHeightOffset += nFontSize + 1;
	}
	else if (m_Kind == kind_bird || m_Kind == kind_mouse )  //
	{  

	}
	else if (m_Kind == kind_partner)  //
	{   
		char outname[64]=""; //��������
		if (m_btRankFFId==99) 
		{//����  
			dwColor=TGetColor("255,255,255");
			dwBorderColor=TGetColor("0,0,0");	

			if (m_GameliveTime>0)
			{//��ʱͬ��
                int nHour=0,nMin=0,nSec=0;

                //nHour = TakeTrader(m_GameliveTime,3600);  //Сʱ
		        //nMin  = TakeTrader(m_GameliveTime-nHour*3600,60); //ʣ��������60�룬�ͼ����
				nHour = m_GameliveTime/3600;//TakeTrader(m_GameliveTime,3600);  //Сʱ	
				nMin  = (m_GameliveTime-nHour*3600)/60;//TakeTrader(m_GameliveTime-nHour*3600,60); //ʣ��������60�룬�ͼ����


					if (nMin>0)
					{
                         nSec  = (m_GameliveTime-nHour*3600-nMin*60);  //��

					}
					else
                         nSec  = m_GameliveTime-nHour*3600;
			
				
                t_sprintf(outname,"(���ʱ��:%dСʱ%d��%d��)",nHour,nMin,nSec);
//			    g_pRepresent->OutputText(nFontSize,outname, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(outname)/4, nMpsY-30, dwColor, 0, nHeightOff, dwBorderColor);
		        nHeightOffset += nFontSize + 1;

				char nFileName[128];
				t_sprintf(nFileName,"\\spr\\skill\\others\\������.spr");
				SetClientSpr(nFileName,nMpsX - (nFontSize * g_StrLen(outname)/4+10),nMpsY-nHeightOff*2.7,0,7);

			}

            dwColor=TGetColor("237,157,24");
			dwBorderColor=TGetColor("0,0,0");
			t_sprintf(outname,"%s",Name);
		    strcat(outname,"(����)");
		   //strcat(outname,m_GuishuName);  //ͬ�����
		   //t_sprintf(outnamea,"[��Ӫ:%d,name:%s,M_dwID:%d,M_Index:%d,PeopleId:%d]",m_Camp);             
//			g_pRepresent->OutputText(nFontSize,outname, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(outname)/4, nMpsY-6, dwColor, 0, nHeightOff, dwBorderColor);
		    nHeightOffset += nFontSize + 1;

			//PaintHonor(30,nMpsX,nMpsY,6);	//��ʾspr�ƺ�
			
		}
		else
		{//����	�ڳ�
			dwColor      = TGetColor("234,289,11");
			dwBorderColor= TGetColor("0,0,0");
			if (m_GameliveTime>0)
			{	
                int nHour=0,nMin=0,nSec=0;

                //nHour = TakeTrader(m_GameliveTime,3600);  //Сʱ
		        //nMin  = TakeTrader(m_GameliveTime-nHour*3600,60); //ʣ��������60�룬�ͼ����
				nHour = m_GameliveTime/3600;//TakeTrader(m_GameliveTime,3600);  //Сʱ	
				nMin  = (m_GameliveTime-nHour*3600)/60;//TakeTrader(m_GameliveTime-nHour*3600,60); //ʣ��������60�룬�ͼ����

				
				if (nMin>0)
				{
					nSec  = (m_GameliveTime-nHour*3600-nMin*60);  //��
					
				}
				else
                    nSec  = m_GameliveTime-nHour*3600;

                t_sprintf(outname,"(���ʱ��:%dСʱ%d��%d��)",nHour,nMin,nSec);
//				g_pRepresent->OutputText(nFontSize,outname, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(outname)/4, nMpsY-30, dwColor, 0, nHeightOff, dwBorderColor);
				nHeightOffset += nFontSize + 1;
			}

			t_sprintf(outname,"%s",Name);
//		    g_pRepresent->OutputText(nFontSize,outname, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(outname)/4, nMpsY-6, dwColor, 0, nHeightOff, dwBorderColor);
		    nHeightOffset += nFontSize + 1;
		}

		   
	}
    else if (bSelect) //�Ƿ� 
	{//����	
		 char *nPath=NULL;
		char outname[64]={0}; //��������
		char outnamea[64]={0}; //��������
		char* series;
		if  (m_IsbeSel)
		{//��ѡ����
			nPath="\\spr\\skill\\task\\randomtask.spr"; //��̾��
			SetClientSpr(nPath,nMpsX,nMpsY-GetNpcPate(),0,7);//
		}

		if (m_GameliveTime>0)
		{//��ʱNPC
			    dwColor=TGetColor("255,255,255");
			    int nHour=0,nMin=0,nSec=0;
				nHour = m_GameliveTime/3600;//TakeTrader(m_GameliveTime,3600);  //Сʱ	
				nMin  = (m_GameliveTime-nHour*3600)/60;//TakeTrader(m_GameliveTime-nHour*3600,60); //ʣ��������60�룬�ͼ����
				if (nMin>0)
				{
					nSec  = (m_GameliveTime-nHour*3600-nMin*60);  //��
					
				}
				else
                    nSec  = m_GameliveTime-nHour*3600;

            t_sprintf(outname,"(���ʱ��:%dСʱ%d��%d��)",nHour,nMin,nSec);
//			g_pRepresent->OutputText(nFontSize,outname, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(outname)/4, nMpsY-30, dwColor, 0, nHeightOff, dwBorderColor);
			nHeightOffset += nFontSize + 1;

			/*wt_sprintf(outname,"����:%s",m_GuishuName);
			g_pRepresent->OutputText(nFontSize,outname, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(outname)/4, nMpsY+100, dwColor, 0, nHeightOff, dwBorderColor);
			nHeightOffset += nFontSize + 1;*/

		 }
		 
		  int nType = this->m_cGold.GetGoldType();  //��ȡ�Ƿ�ƽ���0Ϊ�ǻƽ�
		  int nVal=0;
		      g_NpcSetting.GetInteger(m_NpcSettingIdx+2, "AuraSkillId", 0, &nVal);

		        if (nType == 0 && nVal==0)
					dwColor = 0xffffffff;           //��ɫ���ֹ���
				else if (nType ==1) 
					dwColor = 0xff00ff00;	        // �̹�  ��Ǯ
				else if (nType >=2 && nType <= 4)
					dwColor=TGetColor("100,100,255");//ǳ��100,100,255 ���� 0,0,160
				else if (nType >=5 && nType <= 11)
                    dwColor=TGetColor("234,189,11"); //GetColor(" 255,255,0");
				else if (nType >=12 && nType <= 16)
					dwColor=TGetColor("255,62,62");	//   ���"210,0,0"
				else 
				{
				   if (nVal>0)
				   {
					  dwColor=TGetColor("234,189,11");
				   }

				   //g_NpcSetting.GetInteger(m_NpcSettingIdx+2, "PasstSkillId", 0, &nVal)
				}

           if (m_ZhuaVal>0)
		   {//��ץ��״̬
			   dwColor=TGetColor("255,217,78");
			   dwBorderColor=TGetColor("0,0,0");
               t_sprintf(outname,"%s(��ץ����..)",Name);
		   }
		   else
		   {
                 t_sprintf(outname,"%s",Name);
		   }
           
           if (m_FreezeState.nTime || m_PoisonState.nTime/*m_PoisonState.nTime*/ || m_ConfuseState.nTime || m_StunState.nTime || m_randmove.nTime)
           {
		       strcat(outname, "(");
			if (m_FreezeState.nTime)
				strcat(outname, "��");
			//if (m_PoisonState.nTime)
			if  (m_PoisonState.nTime)
			   strcat(outname, "��");
			if (m_ConfuseState.nTime)
				strcat(outname, "��");
			if (m_StunState.nTime)
				strcat(outname, "��");
			if (m_randmove.nTime)
			    strcat(outname, "��");
			strcat(outname, ")");
		   }

		      t_sprintf(outnamea,"%s,Lv:%d",outname,m_Level);  // m_NpcSettingIdx      
//		   g_pRepresent->OutputText(nFontSize,outnamea, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(outname)/4, nMpsY-6, dwColor, 0, nHeightOff, dwBorderColor);
		   nHeightOffset += nFontSize + 1;
		  

		switch(m_Series) 
		{
		case 0:
			series= "\\Spr\\Ui3\\Npc\\series0.spr";
			break;
		case 1:
			series= "\\Spr\\Ui3\\Npc\\series1.spr";
			break;
		case 2:
			series= "\\Spr\\Ui3\\Npc\\series2.spr";
			break;
		case 3:
			series= "\\Spr\\Ui3\\Npc\\series3.spr";
			break;
		case 4:
			series= "\\Spr\\Ui3\\Npc\\series4.spr";
			break;
		}

		if  (m_Series >= 0  && m_Series<= 4)
		   SetClientSpr(series,nMpsX - (nFontSize * g_StrLen(outname)/4+20),nMpsY-nHeightOff*1.8+1,0,5);
		 
/*
		KRUImage RUIconImage;
		RUIconImage.nType = ISI_T_SPR;
		RUIconImage.Color.Color_b.a = 255;
		RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		RUIconImage.uImage = 0;
		RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
		RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		strcpy(RUIconImage.szImage, series);
		RUIconImage.oPosition.nX = nMpsX - (nFontSize * g_StrLen(outname)/4+20); //ͼƬ��λ��
		RUIconImage.oPosition.nY = nMpsY-nHeightOff*1.75;
	//	RUIconImage.oPosition.nY = nMpsY;
		RUIconImage.oPosition.nZ = 0;
		RUIconImage.nFrame = 0;
		g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, FALSE);
*/

	  
		dwColor=TGetColor("255,255,255");
		dwBorderColor=TGetColor("0,0,0");
		ZeroMemory(outnamea,sizeof(outnamea));

		if (!m_bIsHideLife && !Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bIsHideLife)
		{ 
		   t_sprintf(outnamea,"%d/%d",m_CurrentLife,m_CurrentLifeMax);
		} 
		else
		  ZeroMemory(outnamea,sizeof(outnamea));

//		g_pRepresent->OutputText(nFontSize,outnamea,KRF_ZERO_END,nMpsX-nFontSize*g_StrLen(outnamea)/4, nMpsY+30, dwColor, 0, nHeightOff, dwBorderColor);
		nHeightOffset += nFontSize + 1;
	 
	}
/*	
#ifdef SWORDONLINE_SHOW_DBUG_INFO
	if (Player[CLIENT_PLAYER_INDEX].m_DebugMode)
	{
		char szNameID[50];
		t_sprintf(szNameID,"[%d]", m_dwID);
		g_pRepresent->OutputText(nFontSize, szNameID, KRF_ZERO_END, nMpsX, nMpsY + 20, 0xfff0fff0, 0, m_Height);
	}
	
	if (Player[CLIENT_PLAYER_INDEX].m_nIndex == m_Index && Player[CLIENT_PLAYER_INDEX].m_DebugMode)
	{
		char	szMsg[256];
		int nCount[9];
		for (int i = 0; i < 9; ++i)
			nCount[i] = 0;
		if (LEFTUPREGIONIDX >= 0)
			nCount[0] = LEFTUPREGION.m_NpcList.GetNodeCount();
		if (UPREGIONIDX >= 0)
			nCount[1] = UPREGION.m_NpcList.GetNodeCount();
		if (RIGHTUPREGIONIDX >= 0)
			nCount[2] = RIGHTUPREGION.m_NpcList.GetNodeCount();
		if (LEFTREGIONIDX >= 0)
			nCount[3] = LEFTREGION.m_NpcList.GetNodeCount();
		if (m_RegionIndex >= 0)
			nCount[4] = CURREGION.m_NpcList.GetNodeCount();
		if (RIGHTREGIONIDX >= 0)
			nCount[5] = RIGHTREGION.m_NpcList.GetNodeCount();
		if (LEFTDOWNREGIONIDX >= 0)
			nCount[6] = LEFTDOWNREGION.m_NpcList.GetNodeCount();
		if (DOWNREGIONIDX >= 0)
			nCount[7] = DOWNREGION.m_NpcList.GetNodeCount();
		if (RIGHTDOWNREGIONIDX >= 0)
			nCount[8] = RIGHTDOWNREGION.m_NpcList.GetNodeCount();
		
		int nPosX, nPosY;
		GetMpsPos(&nPosX, &nPosY);
		t_sprintf(szMsg,
			"NpcID:%d  Life:%d\nRegionIndex:%d Pos:%d,%d\nPlayerNumber:%d\n"
			"NpcNumber:\n%02d,%02d,%02d\n%02d,%02d,%02d\n%02d,%02d,%02d",
			m_dwID,
			m_CurrentLife,			
			m_RegionIndex,
			m_MapX,
			m_MapY,
			CURREGION.m_PlayerList.GetNodeCount(),
			nCount[0], nCount[1], nCount[2],
			nCount[3], nCount[4], nCount[5],
			nCount[6], nCount[7], nCount[8]			
			);
		
		g_pRepresent->OutputText(14, szMsg, -1, 320, 40, 0xffffffff);

	}
#endif
	*/	   
	

	return nHeightOffset;
}

void KNpc::SetClientSpr(char *nSprPath,int nxLeft,int nyTop,int nzPos,int i,char *ncPart)
{
				KRUImage RUIconImage;
				RUIconImage.nType = ISI_T_SPR;
				RUIconImage.Color.Color_b.a = 255;
				RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
				RUIconImage.uImage = 0;
				RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
				RUIconImage.bRenderFlag =RUIMAGE_RENDER_FLAG_REF_SPOT;

				strcpy(RUIconImage.szImage, nSprPath);  //�ı�����

				if (ncPart)
				{
					if (strstr(ncPart,"Head")) //ͷ��
					{
						RUIconImage.oPosition.nX = nxLeft;
						RUIconImage.oPosition.nY = nyTop-10;
					}
					else if (strstr(ncPart,"Body"))	//�в�
					{
						RUIconImage.oPosition.nX = nxLeft;
						RUIconImage.oPosition.nY = nyTop;
					}
					else
					{//�Ų�
						RUIconImage.oPosition.nX = nxLeft;
						RUIconImage.oPosition.nY = nyTop;
					}
				    
				}
				else
				{
					RUIconImage.oPosition.nX = nxLeft;
					RUIconImage.oPosition.nY = nyTop;
				}
				RUIconImage.oPosition.nZ = nzPos; 

				/*
				KRUImage RUIconImageR;
				RUIconImageR.nType = ISI_T_SPR;
				RUIconImageR.Color.Color_b.a = 255;
				RUIconImageR.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
				RUIconImageR.uImage = 0;
				RUIconImageR.nISPosition = IMAGE_IS_POSITION_INIT;
				RUIconImageR.bRenderFlag = 0;
				strcpy(RUIconImageR.szImage,TaskBg);  //����ͼ
				RUIconImageR.oPosition.nX = nYx;//nMpsX - nWid / 2;
				RUIconImageR.oPosition.nY = nYy;//nMpsY - 35;
				RUIconImageR.oPosition.nZ = 0;
				RUIconImageR.nFrame = 0;
				g_pRepresentShell->DrawPrimitives(1,&RUIconImageR, RU_T_IMAGE,TRUE);

				*/
				KImageParam	 imgParam; 
				imgParam.nNumFrames = 0;
    			m_ImageStore.GetImageParam(nSprPath,ISI_T_SPR,&imgParam);
				
				if (imgParam.nNumFrames > 0 && m_DurFrame[i] < imgParam.nNumFrames)
				{			
					IR_NextFrame(m_DurFrame[i], imgParam.nNumFrames,imgParam.nInterval,uFlipTime[i]);
					RUIconImage.nFrame =m_DurFrame[i];
				}
				else
				{
					m_DurFrame[i]=0;
					RUIconImage.nFrame =0;		
				}	
//				if (g_GameWorld)
//				g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, FALSE);	 //RU_T_IMAGE
}

//--------------------------------------------------------------------------
//	���ܣ����ַ�����ʾ����ɫ��ϢתΪ��ֵ��ʾ
//--------------------------------------------------------------------------
/*uint32_t KNpc::GetColor(LPCTSTR pString)
{
	if (pString == NULL)
		return 0;
	
	uint32_t Color = 0xFF000000;  //��ɫ
	
	char Buf[16] = "";
	int  i = 0;
	int  n = 0;
	while (pString[i] != ',')
	{
		if (pString[i] == 0 || n >= 15)
			return Color;
		Buf[n++] = pString[i++];
	}
	
	Buf[n] = 0;
	Color += ((atoi(Buf) & 0xFF) << 16);
	n = 0;
	i++;
	while (pString[i] != ',')
	{
		if (pString[i] == 0 || n >= 15)
			return Color;
		Buf[n++] = pString[i++];
	}
	Buf[n] = 0;
	Color += ((atoi(Buf) & 0xFF) << 8);
	n = 0;
	i++;
	while (pString[i] != 0)
	{
		if (n >= 15)
			return Color;
		Buf[n++] = pString[i++];
	}
	Buf[n] = 0;
	Color += (atoi(Buf) & 0xFF);
	return Color;
}
//ת�����ַ��� ��ɫ
const char* KNpc::GetColorString(uint32_t nColor)
{
	static char szColor[12];
	KRColor c;
	c.Color_dw = nColor;
	t_sprintf(szColor, "%d,%d,%d", c.Color_b.r, c.Color_b.g, c.Color_b.b);
	szColor[11] = 0;
	
	return szColor;
}
*/
//�����ͷ����Ϣ����
int	KNpc::PaintChat(int nHeightOffset)
{
	//if (m_Kind != kind_player)  //�����ͷ��������
	//	return nHeightOffset;
	/*
	if (m_nChatContentLen <= 0)
		return nHeightOffset;
	if (m_nChatNumLine <= 0)
		return nHeightOffset;

	int nFontSize = 12;
	int					nWidth, nHeight;
	int					nMpsX, nMpsY,nMap;
	KRUShadow			sShadow;
	//KOutputTextParam	sParam;
	sParam.BorderColor = 0XFF000000; //�����Ե

	sParam.nNumLine = m_nChatNumLine;

	nWidth = m_nChatFontWidth * nFontSize / 2;//
	nHeight = sParam.nNumLine * (nFontSize + 1);

	nWidth += 10;	//Ϊ�˺ÿ�
	nHeight += 5;	//Ϊ�˺ÿ�


	GetMpsPos(&nMpsX, &nMpsY,&nMap);
	sParam.nX = nMpsX - nWidth / 2;
	sParam.nY = nMpsY;
	sParam.nZ = nHeightOffset + nHeight;
	sParam.Color = SHOW_CHAT_COLOR;
	sParam.nSkipLine = 0;
	sParam.nVertAlign = 0;

	sShadow.oPosition.nX = sParam.nX;
	sShadow.oPosition.nX -= 3;	//Ϊ�˺ÿ�
	sShadow.oPosition.nY = sParam.nY;
	sShadow.oPosition.nZ = sParam.nZ;
	sShadow.oEndPos.nX = sParam.nX + nWidth;
	sShadow.oEndPos.nX += 2;	//Ϊ�˺ÿ�
	sShadow.oEndPos.nY = sParam.nY;
	sShadow.oEndPos.nZ = sParam.nZ - nHeight;
	//sShadow.Color.Color_dw = 0x00FFFF00;
	sShadow.Color.Color_dw =0x14000000; // 0XFF000000
	sParam.bPicPackInSingleLine = true;

	//g_pRepresent->OutputRichText(nFontSize, &sParam, m_szChatBuffer, m_nChatContentLen, nWidth);

	return sParam.nZ;*/
	return 0;
}

#include "engine/Text.h"
//�Լ�����ͷ����ʾ
int	KNpc::SetChatInfo(const char* Name, const char* pMsgBuff, unsigned short nMsgLength)
{
	int nFontSize = 12;
	int nMaxStrLen;

    if (m_Kind == kind_player)
       nMaxStrLen = 32;	// ��ʾ�ĳ���
    else
       nMaxStrLen = 64;	// ��ʾ�ĳ���

	char szChatBuffer[MAX_SENTENCE_LENGTH];

	memset(szChatBuffer, 0, sizeof(szChatBuffer));
  //  m_nChatContentLen = TEncodeText(szChatBuffer, strlen(szChatBuffer));
	
	if (nMsgLength)
	{
		int nOffset = 0;
		if (pMsgBuff[0] != KTC_TAB) //�ո�
		{
			szChatBuffer[nOffset] = (char)KTC_COLOR;
			nOffset++;
			szChatBuffer[nOffset] = (char)0xFF;	  //������ɫ
			nOffset++;
			szChatBuffer[nOffset] = (char)0xFF;
			nOffset++;
			szChatBuffer[nOffset] = (char)0x00;
			nOffset++;
			strncpy(szChatBuffer + nOffset, Name, 32);
			nOffset += strlen(Name);
			szChatBuffer[nOffset] = ':';
			nOffset++;
			szChatBuffer[nOffset] = (char)KTC_COLOR_RESTORE;
			nOffset++;
		}
		else
		{
			pMsgBuff ++;
			nMsgLength --;
		}

		if (nMsgLength)
		{
			memcpy(szChatBuffer + nOffset, pMsgBuff, nMsgLength); //��������
			nOffset += nMsgLength;

			memset(m_szChatBuffer, 0, sizeof(m_szChatBuffer));
           // TEncodeText(szChatBuffer, strlen(szChatBuffer));
			m_nChatContentLen = MAX_SENTENCE_LENGTH; //ͷ����Ϣ��ʾ


			TGetLimitLenEncodedString(szChatBuffer, nOffset, nFontSize, nMaxStrLen,m_szChatBuffer, m_nChatContentLen, 2, true);

			m_nChatNumLine = TGetEncodedTextLineCount(m_szChatBuffer, m_nChatContentLen, nMaxStrLen, m_nChatFontWidth,nFontSize, 0, 0, true);//nFontSize
			//SetstrInfo(STR_CHATBUFF_CLIENT,m_szChatBuffer);

			if (m_nChatNumLine >= 2)
				m_nChatNumLine = 2;
			m_nCurChatTime = IR_GetCurrentTime();

			return true;
		}
	}
	return false;
}
//����Ѫ��
int	KNpc::PaintLife(int nHeightOffset, bool bSelect)
{
	if (!bSelect && (m_Kind != kind_player && m_Kind != kind_partner))
		return nHeightOffset;

	if (m_Kind == kind_bird || m_Kind == kind_mouse )  //
	{  
		return nHeightOffset;
	}

	if (m_CurrentLifeMax <= 0)
		return nHeightOffset;

	if (m_Hide.nTime > 0 && relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
		return nHeightOffset; //���غ͵жԹ�ϵ

	if (relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
		(m_Kind == kind_player /*|| m_Kind == kind_partner*/) && m_nPKFlag != 2)
		return nHeightOffset;		//�ежԹ�ϵ����Ҳ���ʾ����

	int	nMpsX, nMpsY,nMmap;
	GetMpsPos(&nMpsX, &nMpsY,&nMmap);
	int nWid = SHOW_LIFE_WIDTH;
	int nHei = SHOW_LIFE_HEIGHT;
	KRUShadow	Blood;
	//DWORD nX = m_CurrentLife*100/m_CurrentLifeMax; //Ѫ����Χ��ɫ��ʾ
	int nCols=0;
	if (m_CurrentLifeMax>=300)
	   nCols=m_CurrentLifeMax/100;	                //����100��
	else
	{
	   nCols=m_CurrentLife*100/m_CurrentLifeMax;
	}

	int nCurCols=0;
	if (nCols>0)
	   nCurCols=m_CurrentLife/nCols;                //��ǰ��ռ���˶��ٷ�

	if (m_CurrentLifeMax<300)
			nCurCols= nCols;


	if (nCurCols >= 50)
	{
		Blood.Color.Color_b.r = 0;
		Blood.Color.Color_b.g = 255;
		Blood.Color.Color_b.b = 0;
	}
	else if (nCurCols >= 25)
	{
		//Blood.Color.Color_b.r = 255;
		//Blood.Color.Color_b.g = 255;
		//Blood.Color.Color_b.b = 0;
		Blood.Color.Color_b.r = 255;//45;
		Blood.Color.Color_b.g = 217;//45;
		Blood.Color.Color_b.b = 78;//255;

	}
	else
	{
		Blood.Color.Color_b.r = 255;
		Blood.Color.Color_b.g = 0;
		Blood.Color.Color_b.b = 0;
	}

	if (m_nPKFlag == 2)
	{
		Blood.Color.Color_b.r = 255;
		Blood.Color.Color_b.g = 0;
		Blood.Color.Color_b.b = 204;
	}

	Blood.Color.Color_b.a = 0;
	Blood.oPosition.nX = nMpsX - nWid / 2;
	Blood.oPosition.nY = nMpsY;
	Blood.oPosition.nZ = nHeightOffset + nHei;
	Blood.oEndPos.nX = Blood.oPosition.nX + nWid * nCurCols / 100;
	Blood.oEndPos.nY = nMpsY;
	Blood.oEndPos.nZ = nHeightOffset;
//	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);

	Blood.Color.Color_b.r = 128;
	Blood.Color.Color_b.g = 128;
	Blood.Color.Color_b.b = 128;
	Blood.oPosition.nX = Blood.oEndPos.nX;
	Blood.oEndPos.nX = nMpsX + nWid / 2;
//	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);

	if (m_Kind == kind_player)
	   nHei = nHei - 2;

	return nHeightOffset + nHei;
}
//����������
int	KNpc::PaintMana(int nHeightOffset)
{
	if (m_Kind != kind_player && m_Kind != kind_partner)
		return nHeightOffset;

	if (m_CurrentManaMax <= 0)
		return nHeightOffset;

     if (m_Hide.nTime > 0 && relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
		return nHeightOffset; //���غ͵жԹ�ϵ

	 if (relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&
		(m_Kind == kind_player || m_Kind == kind_partner) && m_nPKFlag != 2)
		return nHeightOffset;		//�ежԹ�ϵ����Ҳ���ʾ����

	int	nMpsX, nMpsY,nMmap;
	GetMpsPos(&nMpsX, &nMpsY,&nMmap);
	int nWid = 38;
	int nHei = 5;
	KRUShadow	Blood;
	DWORD nX = m_CurrentMana*100 /m_CurrentManaMax ;  //255,217,78
	if (nX >= 50)
	{
		Blood.Color.Color_b.r = 10;
		Blood.Color.Color_b.g = 10;
		Blood.Color.Color_b.b = 255;
	}
	else if (nX >= 25)
	{
		Blood.Color.Color_b.r = 255;//45;
		Blood.Color.Color_b.g = 217;//45;
		Blood.Color.Color_b.b = 78;//255;
	}
	else
	{
		Blood.Color.Color_b.r = 100;
		Blood.Color.Color_b.g = 100;
		Blood.Color.Color_b.b = 255;
	}

	if (m_nPKFlag == 2)
	{
		Blood.Color.Color_b.r = 45;
		Blood.Color.Color_b.g = 45;
		Blood.Color.Color_b.b = 255;
	}

	Blood.Color.Color_b.a = 0;
	Blood.oPosition.nX = nMpsX - nWid / 2;
	Blood.oPosition.nY = nMpsY;
	Blood.oPosition.nZ = nHeightOffset + nHei;
	Blood.oEndPos.nX = Blood.oPosition.nX + nWid * nX / 100;
	Blood.oEndPos.nY = nMpsY;
	Blood.oEndPos.nZ = nHeightOffset;
//	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);

	Blood.Color.Color_b.r = 128;
	Blood.Color.Color_b.g = 128;
	Blood.Color.Color_b.b = 128;
	Blood.oPosition.nX = Blood.oEndPos.nX;
	Blood.oEndPos.nX = nMpsX + nWid / 2;
//	g_pRepresent->DrawPrimitives(1, &Blood, RU_T_SHADOW, FALSE);
	
	return nHeightOffset + nHei;
}

void KNpc::Paint()  // �滭�����NPC
{

	if (/*relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex) &&*/
		m_Index!=Player[CLIENT_PLAYER_INDEX].m_nIndex &&
		(m_Kind == kind_player || m_Kind == kind_partner) && m_Hide.nTime > 0)
	{
		if (g_GameWorld)
			g_GameWorld->MoveObject(OBJ_NODE_NPC,m_Index,true);  //

		return;
	}

	if (m_ResDir != m_Dir)
	{
		int nDirOff = m_Dir - m_ResDir;
		if (nDirOff > 32)
			nDirOff -= 64;
		else if (nDirOff < - 32)
			nDirOff += 64;
		m_ResDir += nDirOff / 2;
		if (m_ResDir >= 64)
			m_ResDir -= 64;
		if (m_ResDir < 0)
			m_ResDir += 64;
	}
	
	int nHeight = GetNpcPate() + GetNpcPatePeopleInfo();
	if (m_btRankId || TongName[0])
		nHeight += 10;


	DrawMenuState(nHeight);		 //���ͷ�� ���� ˯�� ���� ��SPR
	
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bIsHidePlayer)  //�Ƿ�����NPC   �����
    {
    	if (m_Kind == kind_player && !IsPlayer())
		{	
			if (g_GameWorld)
				g_GameWorld->MoveObject(OBJ_NODE_NPC,m_Index,true);
			return;			 
		}
    }
	
  if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bIsHideNpc)  // �������NPC
  {
	if (m_Kind == kind_bird || m_Kind == kind_mouse || m_Kind == kind_normal)   // ���� �� �Ի���
	{
		if (g_GameWorld)
			g_GameWorld->MoveObject(OBJ_NODE_NPC,m_Index,true);
	     return;
	}
  }
   m_DataRes.Draw(m_Index, m_ResDir, m_Frames.nTotalFrame, m_Frames.nCurrentFrame); //�ͻ��˻滭�������
}

//--------------------------------------------------------------------------
//	���ܣ����ӻ������������
//--------------------------------------------------------------------------
void	KNpc::AddBaseLifeMax(int nLife)
{
	m_LifeMax += nLife;
	m_CurrentLifeMax = m_LifeMax;
}
//--------------------------------------//
void	KNpc::SetBaseLifeMax(int nLifeMax) 
{
	m_LifeMax = nLifeMax;
	m_CurrentLifeMax = m_LifeMax;
}
//------------------------------------//
//--------------------------------------------------------------------------
//	���ܣ����ӵ�ǰ���������
//--------------------------------------------------------------------------
void	KNpc::AddCurLifeMax(int nLife)
{
	m_CurrentLifeMax += nLife;
}

//--------------------------------------------------------------------------
//	���ܣ����ӻ������������
//--------------------------------------------------------------------------
void	KNpc::AddBaseStaminaMax(int nStamina)
{
	m_StaminaMax += nStamina;
	m_CurrentStaminaMax = m_StaminaMax;
}
//------------------------------------//
void	KNpc::SetBaseStaminaMax(int nStamina)
{
	m_StaminaMax = nStamina;
	m_CurrentStaminaMax = m_StaminaMax;
}
//------------------------------------//

//--------------------------------------------------------------------------
//	���ܣ����ӵ�ǰ���������
//--------------------------------------------------------------------------
void	KNpc::AddCurStaminaMax(int nStamina)
{
	m_CurrentStaminaMax += nStamina;
}

//--------------------------------------------------------------------------
//	���ܣ����ӻ������������
//--------------------------------------------------------------------------
void	KNpc::AddBaseManaMax(int nMana)
{
	m_ManaMax += nMana;
	m_CurrentManaMax = m_ManaMax;
}
//---------------------------------------//
void	KNpc::SetBaseManaMax(int nMana)
{
	m_ManaMax = nMana;
	m_CurrentManaMax = m_ManaMax;
}
//---------------------------------------//
//--------------------------------------------------------------------------
//	���ܣ����ӵ�ǰ���������
//--------------------------------------------------------------------------
void	KNpc::AddCurManaMax(int nMana)
{
	m_CurrentManaMax += nMana;
}

/*
//--------------------------------------------------------------------------
//	���ܣ����¼��������ظ��ٶ�
//--------------------------------------------------------------------------
void	KNpc::ResetLifeReplenish()
{
	m_LifeReplenish = (m_Level + 5) / 6;
	m_CurrentLifeReplenish = m_LifeReplenish;
}

/*
//--------------------------------------------------------------------------
//	���ܣ����㵱ǰ���������
//--------------------------------------------------------------------------
void	KNpc::CalcCurLifeMax()
{
}
*/

/*
//--------------------------------------------------------------------------
//	���ܣ����㵱ǰ���������
//--------------------------------------------------------------------------
void	KNpc::CalcCurStaminaMax()
{
	m_CurrentStaminaMax = m_StaminaMax;		// ����Ҫ���� װ�������ܡ�ҩ���ʱ���ȵ�Ӱ��
}
*/

/*
//--------------------------------------------------------------------------
//	���ܣ����㵱ǰ���������
//--------------------------------------------------------------------------
void	KNpc::CalcCurManaMax()
{
	m_CurrentManaMax = m_ManaMax;			// ����Ҫ���� װ�������ܡ�ҩ���ʱ���ȵ�Ӱ��
}
*/

//--------------------------------------------------------------------------
//	���ܣ����㵱ǰ�����ظ��ٶ�
//--------------------------------------------------------------------------
void	KNpc::CalcCurLifeReplenish()
{
	m_CurrentLifeReplenish = m_LifeReplenish;	// ���ɫϵ�𡢽�ɫ�ȼ����Ƿ�ʹ��ҩ�������ܺ�ħ��װ���й�
}

void	KNpc::Remove()
{
/*	m_LoopFrames = 0;
	m_Index = 0;
	m_PlayerIdx = -1;
	m_Kind = 0;
	m_dwID = 0;
	Name[0] = 0;*/
	m_DataRes.Remove(m_Index);  //��Ӱ���ɾ��
	Init();
}

void	KNpc::RemoveRes()
{
	m_DataRes.Remove(m_Index);//��Ӱ���ɾ��  m_Index
}

//�ͻ�����ʾ������Ϣ
#ifndef	_SERVER
void	KNpc::ClientShowMsg(const char *Msg)
{
	//strCoreInfo[MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE].c_str()
	KSystemMessage	sMsg;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.eType = SMT_NORMAL;
	sMsg.byParamSize = 0;
	sMsg.byPriority = 0;	

	if (sizeof(Msg)<125)
	{
		t_sprintf(sMsg.szMessage,Msg);
	}
	else
	{
		t_sprintf(sMsg.szMessage,"����:<colro=gyellow>��Ϣ����,ȡ������<color>");
	}
	//t_sprintf(sMsg.szMessage,Msg);
	sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
}
#endif
//--------------------------------------------------------------------------
//	���ܣ��趨�� npc ���������ԣ����ݻ�û��ɣ�not end
//--------------------------------------------------------------------------
void	KNpc::SetSeries(int nSeries)
{
	if (nSeries >= series_metal && nSeries < series_num)
		m_Series = nSeries;
	else
		m_Series = series_metal; //��ϵ��
}


void	KNpc::SetBaseDamage(int nMin,int nMax,int nType)
{
  if (nType==0)
  {//ֱ������
   	   m_PhysicsDamage.nValue[0]              = nMin;
       m_PhysicsDamage.nValue[2]              = nMax;
  }
  else 
  {//����
       m_PhysicsDamage.nValue[0]              *= nMin;
       m_PhysicsDamage.nValue[2]              *= nMax;
  }

}

//�ı䵱ǰMPC������
void	KNpc::SetFangAndHai(KChanelBaseInfo *nNpcInfo)
{

	m_PhysicsDamage.nValue[0]              = nNpcInfo->PhysicsDamage/5;
    m_PhysicsDamage.nValue[2]              = nNpcInfo->PhysicsDamage;

	m_CurrentPhysicsMagicDamageV.nValue[0] = nNpcInfo->m_PhysicsDamage/5;
	m_CurrentPhysicsMagicDamageV.nValue[2] = nNpcInfo->m_PhysicsDamage;

	m_CurrentMagicFireDamage.nValue[0]     = nNpcInfo->m_FireDamage/5;
    m_CurrentMagicFireDamage.nValue[2]     = nNpcInfo->m_FireDamage;

	m_CurrentMagicColdDamage.nValue[0]     = nNpcInfo->m_ColdDamage/5;
    m_CurrentMagicColdDamage.nValue[2]     = nNpcInfo->m_ColdDamage;

	m_CurrentMagicLightDamage.nValue[0]    = nNpcInfo->m_LightDamage/5;
    m_CurrentMagicLightDamage.nValue[2]    = nNpcInfo->m_LightDamage;

	m_CurrentMagicPoisonDamage.nValue[0]   = nNpcInfo->m_PoisonDamage/5;
    m_CurrentMagicPoisonDamage.nValue[2]   = nNpcInfo->m_PoisonDamage;

	m_CurrentFireResist    = nNpcInfo->m_FireResist;	         // Npc�ĵ�ǰ����
	m_CurrentColdResist    = nNpcInfo->m_ColdResist;	         // Npc�ĵ�ǰ������
	m_CurrentPoisonResist  = nNpcInfo->m_PoisonResist;	         // Npc�ĵ�ǰ������
	m_CurrentLightResist   = nNpcInfo->m_LightResist;	         // Npc�ĵ�ǰ�翹��
	m_CurrentPhysicsResist = nNpcInfo->m_PhysicsResist;	         // Npc�ĵ�ǰ������
/*
	int		m_FireDamage;	           // Npc�ĵ�ǰ���˺�
	int		m_ColdDamage;	           // Npc�ĵ�ǰ���˺�
	int		m_LightDamage;	           // Npc�ĵ�ǰ���˺�
	int		m_PoisonDamage;	           // Npc�ĵ�ǰ���˺�
*/

}

//��ȡ��ǰMPC������
void	KNpc::GetFangAndHai(KChanelBaseInfo *nNpcInfo)
{
    nNpcInfo->PhysicsDamage=m_PhysicsDamage.nValue[2];

	nNpcInfo->m_PhysicsDamage=m_PhysicsDamage.nValue[2] + m_CurrentAddPhysicsDamage;//m_MagicPhysicsDamage.nValue[2];

    nNpcInfo->m_FireDamage  =m_CurrentMagicFireDamage.nValue[2];

    nNpcInfo->m_ColdDamage  =m_CurrentMagicColdDamage.nValue[2];

    nNpcInfo->m_LightDamage =m_CurrentMagicLightDamage.nValue[2];

    nNpcInfo->m_PoisonDamage=m_CurrentMagicPoisonDamage.nValue[2];

	nNpcInfo->m_FireResist   =m_CurrentFireResist;	         // Npc�ĵ�ǰ����
	nNpcInfo->m_ColdResist   =m_CurrentColdResist;	         // Npc�ĵ�ǰ������
	nNpcInfo->m_PoisonResist =m_CurrentPoisonResist;	     // Npc�ĵ�ǰ������
	nNpcInfo->m_LightResist  =m_CurrentLightResist;	         // Npc�ĵ�ǰ�翹��
	nNpcInfo->m_PhysicsResist=m_CurrentPhysicsResist;	     // Npc�ĵ�ǰ������
/*
	int		m_FireDamage;	           // Npc�ĵ�ǰ���˺�
	int		m_ColdDamage;	           // Npc�ĵ�ǰ���˺�
	int		m_LightDamage;	           // Npc�ĵ�ǰ���˺�
	int		m_PoisonDamage;	           // Npc�ĵ�ǰ���˺�
	int nMinNpcDamage = Npc[m_nIndex].m_PhysicsDamage.nValue[0] + Npc[m_nIndex].m_CurrentAddPhysicsDamage;
	int nMaxNpcDamage = Npc[m_nIndex].m_PhysicsDamage.nValue[2] + Npc[m_nIndex].m_CurrentAddPhysicsDamage;
*/
}

/*!*****************************************************************************
// Function		: KNpc::SetStateSkill���ͻ��˺ͷ�������ͬʱ���У�
// Purpose		: 
// Return		: void 
// Argumant		: int nSkillID
// Argumant		: int nLevel
// Argumant		: void *pData
// Argumant		: int nDataNum
// Argumant		: int nTime -1��ʾ�������ܣ�ʱ������   û��״̬�������ݵ� ֱ��ʹ��ͬ��Ч��
// Comments		:
// Author		: Spe
*****************************************************************************/
void KNpc::SetToolNoStateEffect(int nLauncher, int nSkillID, int nLevel,int nTime)
{
	if (nLevel <= 0|| nSkillID <= 0) 
		return ;

	KStateNode* pNode;

	pNode = (KStateNode *)m_StateSkillList.GetHead(); //��һ���ڵ�� ����
	int nMun=0;
	while(pNode)
	{
		if (pNode->m_SkillID == nSkillID)
		{//ID��ͬ

			if (pNode->m_Level == nLevel)
			{//�ȼ���ͬ,����������
					pNode->m_LeftTime  = nTime;
			}
			else
			{//�ȼ�����ͬ�� �͸�������
				for (int i = 0; i < 2; ++i)
				{//nDataNum Ϊÿ�����ܵ�����״̬������
                    //���·�ֵ״̬��������
					pNode->m_State[i].nAttribType =0;
					pNode->m_State[i].nValue[0]   =0;
					pNode->m_State[i].nValue[1]   =0;
					pNode->m_State[i].nValue[2]   =0;
				}

			}
			return;
		}
		nMun++;
		pNode = (KStateNode *)pNode->GetNext();
	}
	// û����ѭ���з��أ�˵�����¼���
	//_ASSERT(nSkillID < MAX_SKILL && nLevel < MAX_SKILLLEVEL);
	if (nSkillID >= MAX_SKILL)
	{
		return;
	}

	pNode = new KStateNode;
	pNode->m_SkillID  = nSkillID;
	pNode->m_Level    = nLevel;
    pNode->m_LeftTime = nTime;

	pNode->m_IsClientState =0; //�Լ���״̬

	if (nLevel>=MAX_SKILLLEVEL)
		nLevel = MAX_SKILLLEVEL-1;


	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nSkillID, nLevel);  
	if (pOrdinSkill)
        pNode->m_StateGraphics = pOrdinSkill->GetStateSpecailId();  //��ȡ���ܵ�״̬���õ�id
	else
	{
		pNode->m_StateGraphics = 0;
	}

	if (IsPlayer() && pNode->m_StateGraphics)  
	{//�ͻ���
		m_btStateInfo[m_nNextStatePos] = pNode->m_StateGraphics;
		if ((++m_nNextStatePos) >= MAX_NPC_RECORDER_STATE)
			m_nNextStatePos = 0;
	}
	for (int i = 0; i < 2; ++i)  //ÿ�����ܵ� ������
	{//��������������ȫ��״̬����
		//���෴ֵ�����������Թ��Ƴ�ʱʹ��
		pNode->m_State[i].nAttribType = 0;
		pNode->m_State[i].nValue[0]   = 0;
		pNode->m_State[i].nValue[1]   = 0;
		pNode->m_State[i].nValue[2]   = 0;
	}

	m_StateSkillList.AddTail(pNode);  //��ÿһ�������ļ��ܣ�����״̬�����ݼ���ڵ�
}

/*!*****************************************************************************
// Function		: KNpc::SetStateSkill���ͻ��˺ͷ�������ͬʱ���У�
// Purpose		: 
// Return		: void 
// Argumant		: int nSkillID
// Argumant		: int nLevel
// Argumant		: void *pData
// Argumant		: int nDataNum
// Argumant		: int nTime -1��ʾ�������ܣ�ʱ������
// Comments		:
// Author		: Spe
*****************************************************************************/
void KNpc::SetToolStateSkillEffect(int nLauncher, int nSkillID, int nLevel, void *pData, int nDataNum, int nTime/* = -1*/)
{
	if (nLevel <= 0|| nSkillID <= 0) 
		return ;
	//_ASSERT(nDataNum < MAX_SKILL_STATE);
	if (nDataNum >= MAX_SKILL_STATE)
		nDataNum = MAX_SKILL_STATE;
	KStateNode* pNode;
	KMagicAttrib* pTemp = NULL;
	pTemp = (KMagicAttrib *)pData;    //����������ܵ���������

	pNode = (KStateNode *)m_StateSkillList.GetHead(); //��һ���ڵ�� ����
	//KStateNode *mNode =(KStateNode *)m_StateSkillList.GetStatusNode(5);  //��5���ڵ�ļ���
	int nMun=0;
	while(pNode)
	{
		if (pNode->m_SkillID == nSkillID)
		{//ID��ͬ

			if (pNode->m_Level == nLevel)
			{//�ȼ���ͬ,����������
			///	pNode->m_LeftTime = pTemp->nValue[1];      //������ʱ�� 
				if (nTime)
					pNode->m_LeftTime      = nTime;
				else
					pNode->m_LeftTime      = pTemp->nValue[1];      //������ʱ��

			}
			else
			{//�ȼ�����ͬ�� �͸�������
				for (int i = 0; i < nDataNum; ++i)
				{//nDataNum Ϊÿ�����ܵ�����״̬������
					//���ԭ���ܵ�Ӱ��
					ModifyAttrib(nLauncher, &pNode->m_State[i]);  //���Դ����
					//���µȼ��¼��ܵ�Ӱ����㵽NPC����
					ModifyAttrib(nLauncher, pTemp);
                    //���·�ֵ״̬��������
					pNode->m_State[i].nAttribType = pTemp->nAttribType;
					pNode->m_State[i].nValue[0]   = -pTemp->nValue[0];
					pNode->m_State[i].nValue[1]   = -pTemp->nValue[1];
					pNode->m_State[i].nValue[2]   = -pTemp->nValue[2];
					++pTemp;
				}
			}
			return;
		}
		nMun++;
		pNode = (KStateNode *)pNode->GetNext();
	}
	// û����ѭ���з��أ�˵�����¼���
	//_ASSERT(nSkillID < MAX_SKILL && nLevel < MAX_SKILLLEVEL);
	if (nSkillID >= MAX_SKILL)
	{
		return;
	}

	pNode = new KStateNode;
	pNode->m_SkillID  = nSkillID;
	pNode->m_Level    = nLevel;

	if (nTime)
		pNode->m_LeftTime      = nTime;
	else
		pNode->m_LeftTime      = pTemp->nValue[1];      //������ʱ��

	//pNode->m_LeftTime = nTime;
	pNode->m_IsClientState =0; //�Լ���״̬

	if (nLevel>=MAX_SKILLLEVEL)
		nLevel = MAX_SKILLLEVEL-1;


	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nSkillID, nLevel);  
	if (pOrdinSkill)
        pNode->m_StateGraphics = pOrdinSkill->GetStateSpecailId();  //��ȡ���ܵ�״̬���õ�id
	else
	{
		//g_OrdinSkillsSetting.GetInteger()
		pNode->m_StateGraphics = 0;
	}

	if (IsPlayer() && pNode->m_StateGraphics)  
	{//�ͻ���
		m_btStateInfo[m_nNextStatePos] = pNode->m_StateGraphics;
		if ((++m_nNextStatePos) >= MAX_NPC_RECORDER_STATE)
			m_nNextStatePos = 0;
	}

	pTemp = (KMagicAttrib *)pData;
	for (int i = 0; i < nDataNum; ++i)  //ÿ�����ܵ� ������
	{//��������������ȫ��״̬����
		// ����NPC����
		ModifyAttrib(nLauncher, pTemp);
		//���෴ֵ�����������Թ��Ƴ�ʱʹ��
		pNode->m_State[i].nAttribType = pTemp->nAttribType;
		pNode->m_State[i].nValue[0]   = -pTemp->nValue[0];
		pNode->m_State[i].nValue[1]   = -pTemp->nValue[1];
		pNode->m_State[i].nValue[2]   = -pTemp->nValue[2];
		++pTemp;
	}

	m_StateSkillList.AddTail(pNode);  //��ÿһ�������ļ��ܣ�����״̬�����ݼ���ڵ�
}

/*!*****************************************************************************
// Function		: KNpc::SetStateSkill���ͻ��˺ͷ�������ͬʱ���У�
// Purpose		: 
// Return		: void 
// Argumant		: int nSkillID
// Argumant		: int nLevel
// Argumant		: void *pData
// Argumant		: int nDataNum
// Argumant		: int nTime -1��ʾ�������ܣ�ʱ������
// Comments		:
// Author		: Spe
*****************************************************************************/
void KNpc::SetStateSkillEffect(int nLauncher, int nSkillID, int nLevel, void *pData, int nDataNum, int nTime,int nIsEuq)
{
	if (!pData || nDataNum<=0 || nLevel <= 0|| nSkillID <= 0) 
		return ;

	if (nDataNum >= MAX_SKILL_STATE)
		nDataNum = MAX_SKILL_STATE;

	KStateNode* pNode;
	KMagicAttrib* pTemp = NULL;
	pNode = (KStateNode *)m_StateSkillList.GetHead(); //��һ���ڵ�� ����
	//KStateNode *mNode =(KStateNode *)m_StateSkillList.GetStatusNode(5);  //��5���ڵ�ļ���
	int nMun=0;
	while(pNode)
	{
		if (pNode->m_SkillID == nSkillID)
		{//ID��ͬ
/*#ifdef _SERVER
			printf("�Ͽ�ʼͬ���ͻ��˼���(%d),�ڵȼ�(%d),���ȼ�(%d),ʱ��(%d)\n",nSkillID,pNode->m_Level,nLevel,nTime);
#endif*/
			if (pNode->m_Level == nLevel && nIsEuq==0)
			{//�ȼ���ͬ,����������
				pNode->m_LeftTime = nTime;      //������ʱ��   
			}
			else
			{//�ȼ�����ͬ�� �͸�������
				//pNode->m_LeftTime = nTime;      //������ʱ��
				pTemp = (KMagicAttrib *)pData;    //����������ܵ���������
				for (int i = 0; i < nDataNum; ++i)
				{//nDataNum Ϊÿ�����ܵ�����״̬������
					//���ԭ���ܵ�Ӱ��
					ModifyAttrib(nLauncher, &pNode->m_State[i]);  //���Դ����
					//���µȼ��¼��ܵ�Ӱ����㵽NPC����
					ModifyAttrib(nLauncher, pTemp);
                    //���·�ֵ״̬��������
					pNode->m_State[i].nAttribType = pTemp->nAttribType;
					pNode->m_State[i].nValue[0]   = -pTemp->nValue[0];
					pNode->m_State[i].nValue[1]   = -pTemp->nValue[1];
					pNode->m_State[i].nValue[2]   = -pTemp->nValue[2];
					++pTemp;
					//ccMessageBox("ͬ��״̬�ɹ�","ͬ��״̬�ɹ�");
				}
			}
			return;
		}
		nMun++;
		pNode = (KStateNode *)pNode->GetNext();
	}
	// û����ѭ���з��أ�˵�����¼���
	//_ASSERT(nSkillID < MAX_SKILL && nLevel < MAX_SKILLLEVEL);
	if (nSkillID >= MAX_SKILL)
	{
		return;
	}

	pNode = new KStateNode;
	pNode->m_SkillID  = nSkillID;
	pNode->m_Level    = nLevel;
	pNode->m_LeftTime = nTime;
	pNode->m_IsClientState =0; //�Լ���״̬

	if (nLevel>=MAX_SKILLLEVEL)
		nLevel = MAX_SKILLLEVEL-1;

	KSkill * pOrdinSkill = (KSkill *)g_SkillManager.GetSkill(nSkillID, nLevel);  
	if (pOrdinSkill)
        pNode->m_StateGraphics = pOrdinSkill->GetStateSpecailId();  //��ȡ���ܵ�״̬���õ�id
	else
		pNode->m_StateGraphics = 0;

	/*if (IsPlayer()&& pOrdinSkill)
	{
		char testInfo[128];
		t_sprintf(testInfo,"�յ�״̬ͬ��:%s,%d,%d",pOrdinSkill->GetSkillName(),nTime,nLevel);
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(testInfo);
	}*/

	if (IsPlayer() && pNode->m_StateGraphics)  
	{//�ͻ���
		m_btStateInfo[m_nNextStatePos] = pNode->m_StateGraphics;
		if ((++m_nNextStatePos) >= MAX_NPC_RECORDER_STATE)
			m_nNextStatePos = 0;
	}
	//ccMessageBox("����״̬�ɹ�","����״̬�ɹ�");
	pTemp = (KMagicAttrib *)pData;
	for (int i = 0; i < nDataNum; ++i)  //ÿ�����ܵ� ������
	{//��������������ȫ��״̬����
		// ����NPC����
		ModifyAttrib(nLauncher, pTemp);
		//���෴ֵ�����������Թ��Ƴ�ʱʹ��
		pNode->m_State[i].nAttribType = pTemp->nAttribType;
		pNode->m_State[i].nValue[0]   = -pTemp->nValue[0];
		pNode->m_State[i].nValue[1]   = -pTemp->nValue[1];
		pNode->m_State[i].nValue[2]   = -pTemp->nValue[2];
		++pTemp;
	}

	m_StateSkillList.AddTail(pNode);  //��ÿһ�������ļ��ܣ�����״̬�����ݼ���ڵ�
}

//ͬ���ͻ���ʹ�ü���
void KNpc::SysnCastSkillEffect(int nLauncher,int nSkillID,int nParam1,int nParam2,int nLevel, int nTime,int mMaxBei)
{
    if (nLevel <= 0|| nSkillID <= 0) 
		return;
} 
/*!*****************************************************************************
// Function		: KNpc::ModifyMissleCollsion
// Purpose		: 
// Return		: BOOL 
// Argumant		: BOOL bCollsion
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KNpc::ModifyMissleCollsion(BOOL bCollsion)
{
	if (bCollsion)
		return TRUE;

	//if (g_RandPercent(m_CurrentPiercePercent))
	//	return TRUE;
	else
		return FALSE;
}

int KNpc::ModifyMissleLifeTime(int nLifeTime)
{
	if (IsPlayer())
	{
		//return Player[m_PlayerIdx].GetWeapon().GetRange();
		return nLifeTime;
	}
	else
	{
		return nLifeTime;
	}
}

int	KNpc::ModifyMissleSpeed(int nSpeed)
{
	if (m_CurrentSlowMissle)  //�ӵ��ٶȼ���
	{
		return nSpeed / 2;
	}
	return nSpeed;
}

BOOL KNpc::DoManyAttack()
{
	m_ProcessAI = 0;
	
	KSkill * pSkill =(KSkill*) GetActiveSkill();
	if (!pSkill) 
        return FALSE;
	
	int nEnChance = m_SkillList.GetEnChance(m_ActiveSkListIndex);//m_SkillList.FindSame(pSkill->GetSkillId())
	pSkill->m_nEnChance = nEnChance;
	if (pSkill->GetChildSkillNum() <= m_SpecialSkillStep) 		
        goto ExitManyAttack;


    //m_DataRes.SetBlur(TRUE);

	
	m_Frames.nTotalFrame = pSkill->GetMissleGenerateTime(m_SpecialSkillStep);
	
	int x, y;
	SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
//	m_DesX = x;
//	m_DesY = y;

	if (m_nPlayerIdx > 0)
		pSkill->PlayPreCastSound(m_nSex, x ,y);
	if (g_Random(2))
		m_ClientDoing = cdo_attack;
	else 
		m_ClientDoing = cdo_attack1;

	
	m_Doing = do_manyattack;
	
	m_Frames.nCurrentFrame = 0;

	return TRUE;

ExitManyAttack:

    //m_DataRes.SetBlur(FALSE);
	DoStand();
	m_ProcessAI = 1;
	m_SpecialSkillStep = 0;

	return TRUE;
}

void KNpc::OnManyAttack()
{
	if (WaitForFrame())
	{

		//m_DataRes.SetBlur(FALSE);

		KSkill * pSkill = (KSkill*)GetActiveSkill();
		if (!pSkill) 
            return ;

		int nPhySkillId =  pSkill->GetChildSkillId();//GetCurActiveWeaponSkill(); Changed 
		int nEnChance = m_SkillList.GetEnChance(m_ActiveSkListIndex);//m_SkillList.FindSame(pSkill->GetSkillId())

		if (nPhySkillId > 0)
		{
			KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkillB(nPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
			if (pOrdinSkill)
            {
				pOrdinSkill->m_nEnChance = nEnChance;
				pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
            }
		}
		NpcFuMoCastSkll(m_Index, m_SkillParam1, m_SkillParam2);
		m_SpecialSkillStep ++;
		DoManyAttack();

	}	
}

BOOL	KNpc::DoRunAttack()
{
	m_ProcessAI = 0;

	switch(m_SpecialSkillStep)
	{
	case 0:
		m_Frames.nTotalFrame = m_RunSpeed;
		m_ProcessAI = 0;
		
		//m_DataRes.SetBlur(TRUE);

		if (m_FightMode)
		{
			m_ClientDoing = cdo_fightrun;
		}
		else
		{
			m_ClientDoing = cdo_run;
		}
		
		if (m_DesX < 0 && m_DesY > 0) 
		{
			int x, y;
			SubWorld[m_SubWorldIndex].NewMap2Mps
				(
				Npc[m_DesY].m_RegionIndex,
				Npc[m_DesY].m_MapX, 
				Npc[m_DesY].m_MapY, 
				Npc[m_DesY].m_OffX, 
				Npc[m_DesY].m_OffY, 
				&x,
				&y
				);

		m_DesX = x;
		m_DesY = y;
		}

		m_Frames.nCurrentFrame = 0;
		m_Doing = do_runattack;
		break;

	case 1:
#ifndef _SERVER
		if (g_Random(2))	
			m_ClientDoing = cdo_attack;
		else
			m_ClientDoing = cdo_attack1;

		int x, y, tx, ty,tMap;
		SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
		if (m_SkillParam1 == -1)
		{
			Npc[m_SkillParam2].GetMpsPos(&tx, &ty,&tMap);
		}
		else
		{
			tx = m_SkillParam1;
			ty = m_SkillParam2;
		}
		m_Dir = g_GetDirIndex(x, y, tx, ty);
#endif
		//����Ϊ�����ܲ�������
		m_Frames.nTotalFrame = 0;//m_AttackFrame * 100 / (100 + m_CurrentAttackSpeed);
		m_Frames.nCurrentFrame = 0;
		m_Doing = do_runattack;
		break;

	case 2:
	case 3:
		//m_DataRes.SetBlur(FALSE);
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
		break;
	}

	m_Frames.nCurrentFrame = 0;
		
	return TRUE;

}

void	KNpc::OnRunAttack()
{
				
	if (m_SpecialSkillStep == 0)
	{
		OnRun();
		KSkill * pSkill = (KSkill*)GetActiveSkill();
		if (!pSkill) 
            return ;
		int nEnChance = m_SkillList.GetEnChance(m_ActiveSkListIndex); //m_SkillList.FindSame(pSkill->GetSkillId())
		pSkill->m_nEnChance = nEnChance;
        if (m_Doing == do_stand || (DWORD)m_nCurrentMeleeTime > pSkill->GetMissleGenerateTime(0)) 
		{
			m_SpecialSkillStep ++;
			m_nCurrentMeleeTime = 0;

			DoRunAttack();
		
		}
		else
			m_nCurrentMeleeTime ++;

		m_ProcessAI = 0;
	}
	else if (m_SpecialSkillStep == 1)
	{
		if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
		{
			DoStand();
			m_ProcessAI = 1;	
		}
		else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
		{
			KSkill * pSkill = (KSkill*)GetActiveSkill();
			if (!pSkill) 
                return ;
			
            int nCurPhySkillId = pSkill->GetChildSkillId();//GetCurActiveWeaponSkill();
			int nEnChance = m_SkillList.GetEnChance(m_ActiveSkListIndex);
			pSkill->m_nEnChance = nEnChance;
			if (nCurPhySkillId > 0)
			{
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkillB(nCurPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
				if (pOrdinSkill)
                {
					pOrdinSkill->m_nEnChance = nEnChance;
				    pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
                }
			}
            NpcFuMoCastSkll(m_Index, m_SkillParam1, m_SkillParam2);
			DoStand();
			m_ProcessAI = 1;
			m_SpecialSkillStep = 0;
		}
		//m_DataRes.SetBlur(FALSE);
	}
	else
	{
		//m_DataRes.SetBlur(FALSE);
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
	}
}

BOOL KNpc::DoJumpAttack()
{
	m_ProcessAI = 0;
	int naAttackSpeed=100,nbAttackSpeed=100;
	    g_GameSetTing.GetInteger2("SYSTEM","NpcAttackSpeed",&naAttackSpeed,&nbAttackSpeed);

	switch(m_SpecialSkillStep)
	{
	case 0:
		DoJump();

		//m_DataRes.SetBlur(TRUE);
		m_ClientDoing = cdo_jump;
		m_Doing = do_jumpattack;
		break;
		
	case 1:
		if (g_Random(2))	
			m_ClientDoing = cdo_attack;
		else
			m_ClientDoing = cdo_attack1;
		int x, y, tx, ty,tMap;
		SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
		if (m_SkillParam1 == -1)
		{
			Npc[m_SkillParam2].GetMpsPos(&tx, &ty,&tMap);
		}
		else
		{
			tx = m_SkillParam1;
			ty = m_SkillParam2;
		}
		m_Dir = g_GetDirIndex(x, y, tx, ty);
		if  (nbAttackSpeed + m_CurrentAttackSpeed!=0)
			m_Frames.nTotalFrame = m_AttackFrame * naAttackSpeed / (nbAttackSpeed + m_CurrentAttackSpeed);
		else
			m_Frames.nTotalFrame = m_AttackFrame;

		m_Frames.nCurrentFrame = 0;
		m_Doing = do_jumpattack;
		break;
		
	case 2:
	case 3:
		//m_DataRes.SetBlur(FALSE);
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
		break;
	}
	
	m_Frames.nCurrentFrame = 0;
	
	return TRUE;
	
}

BOOL KNpc::OnJumpAttack()
{
	if (m_SpecialSkillStep == 0)
	{
		if (!OnJump())
		{
			m_SpecialSkillStep ++;
			m_nCurrentMeleeTime = 0;
			DoJumpAttack();
		}
		m_ProcessAI = 0;
	}
	else if (m_SpecialSkillStep == 1)
	{
		//m_DataRes.SetBlur(FALSE);
		if (WaitForFrame() &&m_Frames.nTotalFrame != 0)
		{
			DoStand();
			m_ProcessAI = 1;	
		}
		else if (IsReachFrame(ATTACKACTION_EFFECT_PERCENT))
		{
			KSkill * pSkill =(KSkill*) GetActiveSkill();
			if (!pSkill) 
                return FALSE;
			
            int nCurPhySkillId = pSkill->GetChildSkillId();//GetCurActiveWeaponSkill();
			int nEnChance = m_SkillList.GetEnChance(m_ActiveSkListIndex);
			pSkill->m_nEnChance = nEnChance;
			if (nCurPhySkillId > 0)
			{
				KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkillB(nCurPhySkillId, pSkill->m_ulLevel, SKILL_SS_Missles);
				if (pOrdinSkill)
                {
					pOrdinSkill->m_nEnChance = nEnChance;
					pOrdinSkill->Cast(m_Index, m_SkillParam1, m_SkillParam2);
                }
			}
			NpcFuMoCastSkll(m_Index, m_SkillParam1, m_SkillParam2);
			DoStand();
			m_ProcessAI = 1;
			m_SpecialSkillStep = 0;
		}
	}
	else
	{
		//m_DataRes.SetBlur(FALSE);
		DoStand();
		m_ProcessAI = 1;
		m_SpecialSkillStep = 0;
		return FALSE;
	}
	return TRUE;
}
//������(�����ܷ�) ������(��������) ���ԶԷ���������(�����ܷ�) ���ԶԷ�������(��������)	
BOOL KNpc::CheckHitTarget(int nAR, int nDf, int nIngore,int nInHuLieAr)
{
	int nDefense = nDf * (100 - nIngore) / 100;  //���˵�������
	int bRet=0;

    int nCurAr   = nAR - nInHuLieAr;  //���ԶԷ����е���

	if  (nCurAr<=0) // ������ʣ�������
	{//�����ǰ������������Ϊ С�� 0 �� ����0ʱ,����Ĭ��������Ϊ 10
	    nCurAr = MIN_HIT_PERCENT;
	}

	if  (nDefense<=0)// (����������ʣ��������)
	{//������˵�������С�� �� ����0  ����
        bRet = 1;
		return bRet;
	}
	else
	{//����Ա�  ������ �� ���˵� ���ܵ�  ���ֵ
        if (g_Random(nCurAr) >=g_Random(nDefense))
		{//������
			bRet = 1;
		}
	}

	return bRet;
}

void KNpc::GetNpcCopyFromTemplate(int nNpcTemplateId, int nLevel)
{
	if (nNpcTemplateId < 0 || nLevel < 1 ) 
		return ;

	KNpcTemplate* pNpcTemp = NULL;
	if (pNpcTemp = NpcSet.GetTemplate(nNpcTemplateId, nLevel))
	//if (g_pNpcTemplate[nNpcTemplateId][nLevel]) //������Ч�򿽱���������������
		LoadDataFromTemplate(pNpcTemp);//(nNpcTemplateId, nLevel);

	/*else
	{
		if (!g_pNpcTemplate[nNpcTemplateId][0])
		{//�����������,û�еȼ�֮��
			g_pNpcTemplate[nNpcTemplateId][0] = new KNpcTemplate;
			g_pNpcTemplate[nNpcTemplateId][0]->InitNpcBaseData(nNpcTemplateId);  //��ʼ���������ݺͱ����ļ�
			g_pNpcTemplate[nNpcTemplateId][0]->m_NpcSettingIdx = nNpcTemplateId;
			g_pNpcTemplate[nNpcTemplateId][0]->m_bHaveLoadedFromTemplate = TRUE;
		}
	

//Ȼ����ؽű��ȼ�����		
KLuaScript * pLevelScript = NULL;		

#ifdef _SERVER //g_pNpcTemplate[nNpcTemplateId][0]
			pLevelScript = (KLuaScript*)g_GetScript(g_pNpcTemplate[nNpcTemplateId][0]->m_dwLevelSettingScript);
		
		if (pLevelScript == NULL)  //���û�нű��� ������Ĭ�ϵĽű�
			pLevelScript = g_pNpcLevelScript;   //npcLevelscript.lua
#else
		KLuaScript LevelScript;
		if (!g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript[0])
			pLevelScript = g_pNpcLevelScript;
		else
		{
			    LevelScript.Init();   //��ʼ�� �ű�����
			if (!LevelScript.Load(g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript))
			{
				//g_DebugLog ("[error]��������,�޷���ȷ��ȡ%s", g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript);
				//_ASSERT(0);
				pLevelScript = g_pNpcLevelScript;
				LevelScript.Exit();
			}
			else
				pLevelScript = &LevelScript;
		}

#endif
		g_pNpcTemplate[nNpcTemplateId][nLevel] = new KNpcTemplate; //�������ɵ�ǰ�ȼ��µ� NPC����
		*g_pNpcTemplate[nNpcTemplateId][nLevel] = *g_pNpcTemplate[nNpcTemplateId][0]; //�ѻ������ݸ�ֵ
		g_pNpcTemplate[nNpcTemplateId][nLevel]->m_nLevel = nLevel; //�ı�ȼ�
		//���ظĵȼ��µĽű����� 
		g_pNpcTemplate[nNpcTemplateId][nLevel]->InitNpcLevelData(nNpcTemplateId, pLevelScript, nLevel);
		g_pNpcTemplate[nNpcTemplateId][nLevel]->m_bHaveLoadedFromTemplate = TRUE;  //�����Ѿ������� ģ��NPC����
		LoadDataFromTemplate(nNpcTemplateId,nLevel);       //���ظ��º�����ݻ�������
	} */

}
//��NPCģ�嵼������
void	KNpc::LoadDataFromTemplate(VOID * nNpcTemp)//(int nNpcTemplateId, int nLevel)  //��NPCģ�嵼������
{
	/*if (nNpcTemplateId < 0 || nNpcTemplateId > MAX_NPCSTYLE-1 || nLevel<0 || nLevel > MAX_NPC_LEVEL-1)
	{
		return ;
	} */
	
	//KNpcTemplate* pNpcTemp = NULL;
	//if (pNpcTemp = NpcSet.GetTemplate(nNpcTemplateId, nLevel))

	KNpcTemplate * pNpcTemp = (KNpcTemplate *)nNpcTemp;//NpcSet.GetTemplate(nNpcTemplateId, nLevel);//
	//KNpcTemplate * pNpcTemp = g_pNpcTemplate[nNpcTemplateId][nLevel];

	if (!pNpcTemp)
		return;

	strcpy(Name,pNpcTemp->Name);
	m_Kind   = pNpcTemp->m_Kind;
	m_Camp   = pNpcTemp->m_Camp;
	m_CurrentCamp  = pNpcTemp->m_Camp;

	if (pNpcTemp->m_Series<0 && pNpcTemp->m_Series>4)
	   m_Series = GetRandomNumber(0,4);
	else
	   m_Series = (BYTE)pNpcTemp->m_Series;

	m_Level  = pNpcTemp->m_nLevel;

	if  (m_Level<=0)
		m_Level =1;

	m_HeadImage   =	pNpcTemp->m_HeadImage;
	m_bClientOnly = pNpcTemp->m_bClientOnly;   //�Ƿ�ֻ�ǿͻ��˵�NPC
	m_CorpseSettingIdx = pNpcTemp->m_CorpseSettingIdx;
	m_DeathFrame =	pNpcTemp->m_DeathFrame;
	m_WalkFrame  =	pNpcTemp->m_WalkFrame;
	m_RunFrame   =	pNpcTemp->m_RunFrame;
	m_HurtFrame   =	pNpcTemp->m_HurtFrame;
	m_WalkSpeed   = pNpcTemp->m_WalkSpeed;
	m_AttackFrame =	pNpcTemp->m_AttackFrame;  //�⹥�����ٶ�
	m_CastFrame   = pNpcTemp->m_CastFrame;    //�ڹ������ٶ�
	m_RunSpeed =	pNpcTemp->m_RunSpeed;
	m_StandFrame =  pNpcTemp->m_StandFrame;
	m_StandFrame1 = pNpcTemp->m_StandFrame1;
	m_NpcSettingIdx = pNpcTemp->m_NpcSettingIdx;
	m_nStature		= pNpcTemp->m_nStature;

	m_LifeMax				= pNpcTemp->m_nLifeMax;
	m_ArmorType				= pNpcTemp->m_ArmorType;
	m_HelmType				= pNpcTemp->m_HelmType;
//	m_PifengType		    = pNpcTemp->m_PifengType;
	m_WeaponType			= pNpcTemp->m_WeaponType;
	m_HorseType				= pNpcTemp->m_HorseType;
	m_bRideHorse			= pNpcTemp->m_bRideHorse;
	//strcpy(ActionScript, pNpcTemp->ActionScript);  //��Ϊ�ű���ֵ
	SetstrInfo(STR_ACTION_SCRIPT,pNpcTemp->ActionScript);

	/*for (int j  = 0; j < 4; j ++)
		m_SkillList.Add()
	int		m_nSkillID[4];
	int		m_nSkillLevel[4];*/

	//BOOL	m_bHaveLoadedFromTemplate;
	//int		m_nNpcTemplateId;
	
	RestoreNpcBaseInfo();  //����NPC������Ϣ
	
}

//-----------------------------------------------------------------------
//	���ܣ��趨�������������Сֵ not end ��Ҫ���ǵ��õĵط�
//-----------------------------------------------------------------------
void	KNpc::SetPhysicsDamage(int nMinDamage, int nMaxDamage)
{
	m_PhysicsDamage.nValue[0] = nMinDamage;
	m_PhysicsDamage.nValue[2] = nMaxDamage;
}

//-----------------------------------------------------------------------
//	���ܣ��趨����������
//-----------------------------------------------------------------------
void	KNpc::SetBaseAttackRating(int nAttackRating)
{
	m_AttackRating = nAttackRating;
	// �˴�����Ҫ����װ�������ܵ�Ӱ�죬�������ǰֵ
	m_CurrentAttackRating += m_AttackRating;
}

//-----------------------------------------------------------------------
//	���ܣ��趨������
//-----------------------------------------------------------------------
void	KNpc::SetBaseDefence(int nDefence)
{
	m_Defend = nDefence;
	// �˴�����Ҫ����װ�������ܵ�Ӱ�죬�������ǰֵ
	m_CurrentDefend = m_Defend;
}

/*
//-----------------------------------------------------------------------
//	���ܣ��趨�����ٶ�
//-----------------------------------------------------------------------
void	KNpc::SetBaseWalkSpeed(int nSpeed)
{
	m_WalkSpeed = nSpeed;
	// �˴�����Ҫ����װ�������ܵ�Ӱ�죬�������ǰֵ (not end)
	m_CurrentWalkSpeed = m_WalkSpeed;
}
*/

/*
//-----------------------------------------------------------------------
//	���ܣ��趨�ܲ��ٶ�
//-----------------------------------------------------------------------
void	KNpc::SetBaseRunSpeed(int nSpeed)
{
	m_RunSpeed = nSpeed;
	// �˴�����Ҫ����װ�������ܵ�Ӱ�죬�������ǰֵ (not end)
	m_CurrentRunSpeed = m_RunSpeed;
}
*/

#ifdef _SERVER

void KNpc::ScriptDropItem(int nBelongPlayer,int nLastDamageIdx,int nItemNum,int nSeries)
{
	if (nBelongPlayer > 0 && m_pDropRate) // �Ƿ��б���  �Ƿ��� ������ ���� �������� �б�������
	{
	
		int nNum=0,nLuckyRate=0;
		int nLuckBeiShu = Npc[Player[nBelongPlayer].m_nIndex].IsLuKey;    //����˱���

		int nGoldLuck=g_GlobalMissionArray.GetMissionValue(28);

		if (nGoldLuck<=0)
            nGoldLuck=1;

        int nLuck       = Player[nBelongPlayer].m_nCurLucky*nLuckBeiShu*nGoldLuck;  //�����ߵ�����ֵ
		//int nSysZcount  = 2;
		//    nSysZcount  = m_pDropRate->nCount;   //������
        int nSysBcount  = 0;
		    nSysBcount  = m_pDropRate->nTypeNum;//GetItemParm(ITEM_PARM_TYPENUM); //Ҫ���µ�����

		g_GameSetTing.GetInteger("ServerConfig","LuckyRate",100,&nLuckyRate);

		 nNum=nSysBcount;

		 if (nItemNum >0)
			 nNum = nItemNum;

       //Ǯֻ��һ��	

		 /*
		 {
		 int		nGenre;
		 int		nDetailType;
		 int		nParticulType;
		 int		nRate;              //����
		 int     nQuality;           //�Ƿ�ƽ�
		 int 	nMinItemLevel;      //��Ʒ����С�ȼ�
		 int     nMaxItemLevel;      //��Ʒ�����ȼ�
		 int     nHour;              //��ʱ
		 int     nIsBang;            //�Ƿ��
		 int     nIsNoBian;          //�Ƿ��Ǳ�ʶ
		 int     nStackNum;          //���ӵ�����
		 //int     nDropType;        //����
		 };  //��Ʒ����
		 
		   int			nCount;
		   int			nMagicRate;
		   int			nMaxRandRate;
		   int			nMoneyRate;
		   int         nMoneyNum;
		   int			nMoneyScale;
		   int			nMinItemLevelScale;   //��С�ȼ�����
		   int			nMaxItemLevelScale;
		   int		    nMinItemLevel;
		   int		    nMaxItemLevel;
		   int         nTypeNum;             //���µ�����
    int         nIsBianShi;           //�Ƿ���Ա� �ɱ�ʶװ��
			 
		*/

	   g_GameSetTing.GetInteger("ServerConfig","GoldMoneyRate",100,&nSysBcount);

	   if (g_RandPercent(nSysBcount) && g_RandPercent(m_pDropRate->nMoneyRate))//GetItemParm(ITEM_PARM_MONEYRATE)))         //���ȡ���ʷ�Χ�� return ((int)g_Random(100) < nPercent);
	   {  
		   LoseMoney(nBelongPlayer,m_pDropRate->nMoneyNum);//GetItemParm(ITEM_PARM_MONEYNUM));  //Ǯ
	   } 
//----------------------------------------------------------ʱ�������
	   	int nIsCloseAllItem=0,nIsBreak=0;

        if (/*nBelongPlayer>0 && */Npc[Player[nBelongPlayer].m_nIndex].m_AutoplayId>0)
		{//�һ���
			int nIsCloseGuaItem=0,nTimeStart=0,nTimeEnd=9,nIsOpenFullForbit=0;
			g_GameSetTing.GetInteger("ServerConfig","IsOpenFullForbit",0,&nIsOpenFullForbit);

			if (nIsOpenFullForbit>=1 && Player[nBelongPlayer].m_nRoomFull==1)	  //�������� ��������Ʒ
			{
				nIsBreak=1;
				return;
			}

            g_GameSetTing.GetInteger2("ServerConfig","IsCloseItem",&nIsCloseGuaItem,&nIsCloseAllItem);
			g_GameSetTing.GetInteger2("ServerConfig","TimeCloseItem",&nTimeStart,&nTimeEnd);

			if (nIsCloseAllItem>=1)
			{
                return;
			}

			time_t rawtime;      //����һ��long �ʹ������
			struct tm * timeinfo;	
			time (&rawtime);     //�������ڵ�ʱ�䣨�룩
			timeinfo = localtime(&rawtime);  //�Ѵ�1970-1-1�����ֵ���ǰʱ��ϵͳ��ƫ�Ƶ�����ʱ��ת��Ϊ����ʱ��
			int	bYear=0,bMonth=0,bDay=0,bHour=0,bMin=0;			
			bYear   = timeinfo->tm_year+1900;
			bMonth  = timeinfo->tm_mon+1;
			bDay    = timeinfo->tm_mday;
			bHour   = timeinfo->tm_hour;
	        bMin    = timeinfo->tm_min;
            
            if (nIsCloseGuaItem>=1 && bHour>=nTimeStart && bHour<nTimeEnd)
			{
				nIsBreak=1;
				return;
			}
		}

		/*if (Player[nBelongPlayer].m_ItemList.GetPlayerItemCount()>=MAX_ITEMLIST_CONUT)
		{
			//Player[nBelongPlayer].m_ItemList.msgshow("��ʾ:<color=yollow>��Ʒ����,��������Ʒ�ռ�<color>!");
			nIsBreak=1;
			return;
		}*/
//----------------------------------------------------------
		//------------------------------------------------�µ�
		//CONST KItemDropRate::KItemParam* pItemDropParam = NULL;  //��Ʒ��Ϣ����

		int  nCurIdx=0;
		for (int i = 0; i < nNum; ++i) 
		{  			
            if (nIsCloseAllItem>=1)
				break;
            if (nIsBreak>=1)
				break;
			//	CONST KItemDropRate::KItemParam* pItemDropParam = NULL;  //��Ʒ��Ϣ����
			//pItemDropParam = m_pDropRate->GetRandItem(nLuck); //���ľ��������Ʒ
			
			//if (!pItemDropParam)
			//   continue;
			char nstrName[64]={0};
                 GetstrInfo(STR_GUISHU_NAME,nstrName);
           if (strstr(nstrName,"ϵͳ"))    
		   {//����boss
			     nCurIdx = nBelongPlayer;
			     LoseSingleItem(nBelongPlayer,NULL,nLuckyRate,nLastDamageIdx,nSeries);   //NPC����װ�� m_nLastDamageIdx	 
		   }
		   else 
		   {//˽��BOSS
			 //  int nNpcidx=NpcSet.SearchGSName(m_GuishuName);
			    int mPlayerIdx = PlayerSet.FindNameID(nstrName);
                if (mPlayerIdx>0)
				{//����
					nCurIdx = mPlayerIdx;
				    LoseSingleItem(mPlayerIdx,NULL,nLuckyRate,nLastDamageIdx,nSeries);   //NPC����װ��
				}
				else
				{//������
					LoseSingleItem(nBelongPlayer,NULL,nLuckyRate,nLastDamageIdx,nSeries);   //NPC����װ��
				}
		   }
		}

		if (g_RandPercent(m_pDropRate->nMoneyRate))//GetItemParm(ITEM_PARM_MONEYRATE)))  //���ȡ���ʷ�Χ�� return ((int)g_Random(100) < nPercent);
		{   
			LoseMoney(nCurIdx);  //Ǯ
		}  
	}

}
//  ��������--NPC����װ��
void KNpc::DeathPunish(int nMode, int nBelongPlayer,int nLastDamageIdx)
{
	#define	LOSE_EXP_SCALE	10

	if (IsPlayer())
	{//�������
		int nExpMin=0,nExpMax=0,nGoldExpMin=0,nGoldExpMax=0,nSubExp=0,nDeathLevelpro;
				g_GameSetTing.GetInteger2("SYSTEM","DeathExpData",&nExpMin,&nExpMax);
				g_GameSetTing.GetInteger("SYSTEM","DeathLevelpro",10,&nDeathLevelpro);
				g_GameSetTing.GetInteger2("SYSTEM","GoldDeathExpData",&nGoldExpMin,&nGoldExpMax);
		

		if (nMode == DEATH_MODE_NPC_KILL)
		{//��npc kill
			nSubExp=0;
			if (Player[m_nPlayerIdx].m_nExp > 0)
			{//��ǰ�ľ���
				
				if (m_Level <= 10)
					nSubExp = Player[m_nPlayerIdx].m_nExp/50;//TakeTrader(Player[m_nPlayerIdx].m_nExp,50);	 //PlayerSet.m_cLevelAdd.GetLevelExp(m_Level)
				else
					nSubExp = GetRandomNumber(nGoldExpMin*m_Level,nGoldExpMax*m_Level);//TakeTrader(Player[m_nPlayerIdx].m_nExp,25);  	 //4��֮һ
                
				if (nSubExp>0)
				{
			        Player[m_nPlayerIdx].DirectAddExp(-nSubExp);
				}
				else
				{
					if (nSubExp==0)
						Player[m_nPlayerIdx].DirectAddExp(-Player[m_nPlayerIdx].m_nExp);
					else
						Player[m_nPlayerIdx].DirectAddExp(nSubExp);//Player[m_nPlayerIdx].m_nExp
				}

			}
			else
			{//��ǰ����Ϊ��ʱ,����ȫ����������
				if (m_Level > nDeathLevelpro)
				    Player[m_nPlayerIdx].DirectAddExp(-GetRandomNumber(nExpMin,nExpMax));
			}

			// Ǯ����
			int nMoney = (Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney())/2;//TakeTrader(Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney(),2);  //����һ���Ǯ
			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostMoney(nMoney);
				// ��ʧ��Ǯ��Ϣ
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType =s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_DEC_MONEY;
				sMsg.m_lpBuf = (void *)(nMoney);
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);//
				sMsg.m_lpBuf = 0;

				//if (TakeTrader(nMoney,2) > 0)
				if ((nMoney/2) > 0)
					PlayerDeadCreateMoneyObj(nMoney/2);  //����1/4 ��Ǯ
			}
		}
		else if (nMode == DEATH_MODE_PLAYER_NO_PUNISH)
		{//�д裬û�гͷ�
			return;
		}
		else if (nMode == DEATH_MODE_EXP_PUNISH)
		{//����ͷ�
			 nSubExp=0;
			 if (Player[m_nPlayerIdx].m_nExp > 0)
			 {//��ǰ�ľ���

				if (m_Level <= 10)
					nSubExp = (Player[m_nPlayerIdx].m_nExp)/50;//TakeTrader(Player[m_nPlayerIdx].m_nExp,50);
				else
					nSubExp = GetRandomNumber(nGoldExpMin*m_Level,nGoldExpMax*m_Level);//TakeTrader(Player[m_nPlayerIdx].m_nExp,25);  
                
				if (nSubExp>0)
				{
					Player[m_nPlayerIdx].DirectAddExp(-nSubExp);
				}
				else
				{
					if (nSubExp==0)
						Player[m_nPlayerIdx].DirectAddExp(-Player[m_nPlayerIdx].m_nExp);
					else
						Player[m_nPlayerIdx].DirectAddExp(nSubExp);//Player[m_nPlayerIdx].m_nExp
				}

			 } 
			 else
			 {//��ǰ����Ϊ��ʱ,����ȫ����������
				    nSubExp= GetRandomNumber(nExpMin,nExpMax);
					Player[m_nPlayerIdx].DirectAddExp(-nSubExp);
			 }
				if (Npc[nLastDamageIdx].m_Kind==kind_player)
				{//ȫ�ֳͷ��ű�
					int nTarPlayer=0;
					char szNpcIndex[30];

					if (nLastDamageIdx>0)
					{
				        t_sprintf(szNpcIndex, "%d", nLastDamageIdx);
						nTarPlayer = Npc[nLastDamageIdx].m_nPlayerIdx;  //Npc[m_nLastDamageIdx].m_nPlayerIdx
					} 
					if (nTarPlayer>0)    //�Ƿ�ִ��ȫ�ֽű�
						Player[nTarPlayer].ExecuteScriptC("\\script\\global\\goldpunish.lua", "nGoldPunish", szNpcIndex,Name,m_Index,m_dwID,nSubExp);
				}
		}
		else if (nMode == DEATH_MODE_MONEY_PUNISH)
		{//�����ͷ�
			int nMoney = Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney()/2;//TakeTrader(Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney(),2);  //����һ���Ǯ
			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostMoney(nMoney);
				// ��ʧ��Ǯ��Ϣ
				char msg[64];
				t_sprintf(msg,"��ʾ:����ʧ�� %d ����!",nMoney);
				Player[m_nPlayerIdx].m_ItemList.msgshow(msg);
				
				if ((nMoney/2) > 0)
					PlayerDeadCreateMoneyObj(nMoney/2);  //����1/4 ��Ǯ

			}

			if (Npc[nLastDamageIdx].m_Kind==kind_player)
			{//ȫ�ֳͷ��ű�
				int nTarPlayer=0;
				char szNpcIndex[30];
				if (nLastDamageIdx>0)
				{
					t_sprintf(szNpcIndex, "%d", nLastDamageIdx);
					nTarPlayer = Npc[nLastDamageIdx].m_nPlayerIdx;  //Npc[m_nLastDamageIdx].m_nPlayerIdx
				} 
				if (nTarPlayer>0)    //�Ƿ�ִ��ȫ�ֽű�
					Player[nTarPlayer].ExecuteScriptC("\\script\\global\\goldpunish.lua", "nGoldPunish", szNpcIndex,Name,m_Index,m_dwID,nMoney);
			}
		}
		else if (nMode == DEATH_MODE_EQUIP_PUNISH)
		{//װ���ͷ�
			int  nLostRate;

            nLostRate=g_Random(100)+Player[m_nPlayerIdx].m_nCurLucky;
			// ��ʧ��Ʒ
			Player[m_nPlayerIdx].m_ItemList.AutoLoseItemFromEquipmentRoom(nLostRate);
			
			// ��ʧ�������ϵ�װ��
			if (g_Random(100) > g_Random(100)+Player[m_nPlayerIdx].m_nCurLucky)
			{//����Խ���Խ�ѵ���
				Player[m_nPlayerIdx].m_ItemList.AutoLoseEquip();
			}

		}
		else if (nMode == DEATH_MODE_JINBI_PUNISH)
		{//��ҳͷ�
			int nMoney = (Player[m_nPlayerIdx].m_ItemList.GetEquipmentXu())/10;//TakeTrader(Player[m_nPlayerIdx].m_ItemList.GetEquipmentXu(),10);  //10��֮һ
			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostXu(nMoney);
				// ��ʧ��Ǯ��Ϣ
                char msg[64];
				t_sprintf(msg,"��ʾ:����ʧ�� %d ���!",nMoney);
				Player[m_nPlayerIdx].m_ItemList.msgshow(msg);
			}

			if (Npc[nLastDamageIdx].m_Kind==kind_player)
			{//ȫ�ֳͷ��ű�
				int nTarPlayer=0;
				char szNpcIndex[30];
				if (nLastDamageIdx>0)
				{
					t_sprintf(szNpcIndex, "%d", nLastDamageIdx);
					nTarPlayer = Npc[nLastDamageIdx].m_nPlayerIdx;  //Npc[m_nLastDamageIdx].m_nPlayerIdx
				} 
				if (nTarPlayer>0)    //�Ƿ�ִ��ȫ�ֽű�
					Player[nTarPlayer].ExecuteScriptC("\\script\\global\\goldpunish.lua", "nGoldPunish", szNpcIndex,Name,m_Index,m_dwID,nMoney);
			}

		}	
		else if (nMode == DEATH_MODE_PKBATTLE_PUNISH)
		{//������� ,��װ��  ��ս
			nSubExp=0;
			if (Player[m_nPlayerIdx].m_nExp > 0)
			{
				
				if (m_Level <= 10)
					nSubExp = Player[m_nPlayerIdx].m_nExp/50;//TakeTrader(Player[m_nPlayerIdx].m_nExp,50);
				else
					nSubExp = GetRandomNumber(nGoldExpMin*m_Level,nGoldExpMax*m_Level);//TakeTrader(Player[m_nPlayerIdx].m_nExp,25);  
                
				if (nSubExp>0)
					Player[m_nPlayerIdx].DirectAddExp(-nSubExp);
				else
				{
					if (nSubExp==0)
						Player[m_nPlayerIdx].DirectAddExp(-Player[m_nPlayerIdx].m_nExp);
					 else
					    Player[m_nPlayerIdx].DirectAddExp(nSubExp);//Player[m_nPlayerIdx].m_nExp
				}
			}
			else
			{//��ǰ����Ϊ��ʱ,����ȫ����������
				if (m_Level > nDeathLevelpro)
				   Player[m_nPlayerIdx].DirectAddExp(-GetRandomNumber(nExpMin,nExpMax));
			}

			// Ǯ����
			int nMoney = Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney()/2;//TakeTrader(Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney(),2);  //����һ���Ǯ
			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostMoney(nMoney);
				// ��ʧ��Ǯ��Ϣ
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType =s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_DEC_MONEY;
				sMsg.m_lpBuf = (void *)(nMoney);
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);//
				sMsg.m_lpBuf = 0;
				
				if (nMoney/2 > 0)
					PlayerDeadCreateMoneyObj(nMoney/2);  //����1/4 ��Ǯ
			}

			int  nLostRate;
                 nLostRate=g_Random(100)+Player[m_nPlayerIdx].m_nCurLucky;
			// ��ʧ��Ʒ
			Player[m_nPlayerIdx].m_ItemList.AutoLoseItemFromEquipmentRoom(nLostRate);
			
			// ��ʧ�������ϵ�װ��
			if (g_Random(100) > g_Random(100)+Player[m_nPlayerIdx].m_nCurLucky)
			{
				Player[m_nPlayerIdx].m_ItemList.AutoLoseEquip();
			}
		}
		// PK��������PKֵ����ͷ� 2
		else
		{
			int	nPKValue;
			    nPKValue = Player[this->m_nPlayerIdx].m_cPK.GetPKValue();
			if (nPKValue < 0)
				nPKValue = 0;
			if (nPKValue > MAX_DEATH_PUNISH_PK_VALUE)
				nPKValue = MAX_DEATH_PUNISH_PK_VALUE;

			// �������
			//int	nLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(m_Level);  //��ȡ��ǰ�ȼ��ľ���
			//    Player[m_nPlayerIdx].DirectAddExp(-(nLevelExp/PlayerSet.m_sPKPunishParam[nPKValue].m_nExp));

             nSubExp=0;
			 if (Player[m_nPlayerIdx].m_nExp > 0)
			 {
				if (m_Level <= 10)
					nSubExp = (PlayerSet.m_cLevelAdd.GetLevelExp(m_Level))/50;//TakeTrader(PlayerSet.m_cLevelAdd.GetLevelExp(m_Level),50);
				else
					nSubExp = GetRandomNumber(nGoldExpMin*m_Level,nGoldExpMax*m_Level);//TakeTrader(PlayerSet.m_cLevelAdd.GetLevelExp(m_Level),25);  
                
				if (nSubExp>0)
			        Player[m_nPlayerIdx].DirectAddExp(-nSubExp);
				else
				{
					if (nSubExp==0)
						Player[m_nPlayerIdx].DirectAddExp(-Player[m_nPlayerIdx].m_nExp);
					else
						Player[m_nPlayerIdx].DirectAddExp(nSubExp);//Player[m_nPlayerIdx].m_nExp
				}
			 }
			 else
			 {//��ǰ����Ϊ��ʱ,����ȫ����������
				 Player[m_nPlayerIdx].DirectAddExp(-GetRandomNumber(nExpMin,nExpMax));
			 } 

			// Ǯ����
			int nMoney = Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney()/50;//TakeTrader(Player[m_nPlayerIdx].m_ItemList.GetEquipmentMoney(),50);

			if (nMoney > 0)
			{
				Player[m_nPlayerIdx].m_ItemList.CostMoney(nMoney);
				// ��ʧ��Ǯ��Ϣ
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_DEC_MONEY;
				sMsg.m_lpBuf = (void *)(nMoney);
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				g_pServer->PackDataToClient(Player[m_nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				sMsg.m_lpBuf = 0;
				
				if (nMoney/2 > 0)
					PlayerDeadCreateMoneyObj(nMoney/2);
			}
          int sPKValue=0;
          g_GameSetTing.GetInteger("ServerConfig","PKValue",5,&sPKValue);
			// ��ʧ��ƷGetPKValue()
          if (nPKValue>=sPKValue)
          {//PKֵ����5�ŵ���װ��
			Player[m_nPlayerIdx].m_ItemList.AutoLoseItemFromEquipmentRoom(PlayerSet.m_sPKPunishParam[nPKValue].m_nItem);
			// ��ʧ�������ϵ�װ��
			if (g_Random(100) < PlayerSet.m_sPKPunishParam[nPKValue].m_nEquip)  //����PKֵ�� �ͷ�����
			{
				//Player[m_nPlayerIdx].m_ItemList.AutoLoseEquip();
			}
          }
            //����ɱ�� PKֵ����һ��
			Player[m_nPlayerIdx].m_cPK.AddPKValue(NpcSet.m_nBeKilledAddPKValue); //����PKֵ  m_Currentnopkvalue
			
			if (nLastDamageIdx)
			{
				if (Npc[nLastDamageIdx].IsPlayer())
				{
					KPlayerChat::MakeEnemy(Name,Npc[nLastDamageIdx].Name);  //��Ϊ����
				}
			}
		}
	}
//��������
	else if (nBelongPlayer > 0 && m_pDropRate) // �Ƿ��б���  �Ƿ��� ������ ���� �������� �б�������
	{
	
		int nNum=0,nLuckyRate=0;

		int nLuckBeiShu = Npc[Player[nBelongPlayer].m_nIndex].IsLuKey;    //����˱���

		int nGoldLuck=g_GlobalMissionArray.GetMissionValue(28);

		if (nGoldLuck<=0)
            nGoldLuck=1;

        int nLuck       = Player[nBelongPlayer].m_nCurLucky*nLuckBeiShu*nGoldLuck;  //�����ߵ�����ֵ
		//int nSysZcount  = 2;
		//    nSysZcount  = m_pDropRate->nCount;   //������
        int nSysBcount  = 0;
		   // nSysBcount  = m_pDropRate->nTypeNum; //Ҫ���µ�����

		g_GameSetTing.GetInteger("ServerConfig","LuckyRate",100,&nLuckyRate);

		nNum = m_pDropRate->nTypeNum;//GetItemParm(ITEM_PARM_TYPENUM);

		if (m_CurrentTreasure >0)
		{
		    nNum = m_CurrentTreasure;
		}

		if (m_pDropRate->nCount<=0)
		   return;


      /* if  (nLuck>100)
	   { 
		   if (nLuck>100 && nLuck <=200)
              nNum=nSysBcount+GetRandomNumber(0,3);
			else if (nLuck>200 && nLuck <=500)
              nNum=nSysBcount+GetRandomNumber(1,3);
			else if (nLuck>500 && nLuck <=700)
              nNum=nSysBcount+GetRandomNumber(1,5);
			else if (nLuck>700)
			  nNum=nSysBcount+GetRandomNumber(1,6);
	   } 
       else
	   { 
		if (nSysBcount<=1)  //�����ļ��ĵ�װ������
          nNum=m_CurrentTreasure;   //ģ��� ��װ������
		else
          nNum=nSysBcount; 
	   } 
	   */
       //Ǯֻ��һ��

	   g_GameSetTing.GetInteger("ServerConfig","GoldMoneyRate",100,&nSysBcount);

	   if (g_RandPercent(nSysBcount) && g_RandPercent(m_pDropRate->nMoneyRate))//GetItemParm(ITEM_PARM_MONEYRATE)))         //���ȡ���ʷ�Χ�� return ((int)g_Random(100) < nPercent);
	   {  
		   LoseMoney(nBelongPlayer,m_pDropRate->nMoneyNum);//GetItemParm(ITEM_PARM_MONEYNUM));  //Ǯ
	   } 
//----------------------------------------------------------ʱ�������
	   	int nIsCloseAllItem=0,nIsBreak=0;

        if (/*nBelongPlayer>0 && */Npc[Player[nBelongPlayer].m_nIndex].m_AutoplayId>0)
		{//�һ���
			int nIsCloseGuaItem=0,nTimeStart=0,nTimeEnd=9,nIsOpenFullForbit=0;
			g_GameSetTing.GetInteger("ServerConfig","IsOpenFullForbit",0,&nIsOpenFullForbit);

			 /*char mag[64];
			 t_sprintf(mag,"S�Ƿ��ֹ����Ʒ:%d,�Ƿ���:%d",Player[nBelongPlayer].m_nRoomFull,nIsOpenFullForbit);
			 Player[nBelongPlayer].m_ItemList.msgshow(mag);*/

			if (nIsOpenFullForbit>=1 && Player[nBelongPlayer].m_nRoomFull==1)	  //�������� ��������Ʒ
			{
				nIsBreak=1;
				return;
			}

            g_GameSetTing.GetInteger2("ServerConfig","IsCloseItem",&nIsCloseGuaItem,&nIsCloseAllItem);
			g_GameSetTing.GetInteger2("ServerConfig","TimeCloseItem",&nTimeStart,&nTimeEnd);

			if (nIsCloseAllItem>=1)
			{
                return;
			}

			time_t rawtime;      //����һ��long �ʹ������
			struct tm * timeinfo;	
			time (&rawtime);     //�������ڵ�ʱ�䣨�룩
			timeinfo = localtime(&rawtime);  //�Ѵ�1970-1-1�����ֵ���ǰʱ��ϵͳ��ƫ�Ƶ�����ʱ��ת��Ϊ����ʱ��
			int	bYear=0,bMonth=0,bDay=0,bHour=0,bMin=0;			
			bYear   = timeinfo->tm_year+1900;
			bMonth  = timeinfo->tm_mon+1;
			bDay    = timeinfo->tm_mday;
			bHour   = timeinfo->tm_hour;
	        bMin    = timeinfo->tm_min;
            
            if (nIsCloseGuaItem>=1 && bHour>=nTimeStart && bHour<nTimeEnd)
			{
				nIsBreak=1;
				return;
			}
		}

		/*if (Player[nBelongPlayer].m_ItemList.GetPlayerItemCount()>=MAX_ITEMLIST_CONUT)
		{
			//Player[nBelongPlayer].m_ItemList.msgshow("��ʾ:<color=yollow>��Ʒ����,��������Ʒ�ռ�<color>!");
			nIsBreak=1;
			return;
		}*/
//----------------------------------------------------------
		//------------------------------------------------�µ�
		//CONST KItemDropRate::KItemParam* pItemDropParam = NULL;  //��Ʒ��Ϣ����
		if (nNum<=0)
		    nNum = 5;

		int nCurIdx = 0;
		for (int i = 0; i < nNum; ++i) 
		{  			
            if (nIsCloseAllItem>=1)
				return;
            if (nIsBreak>=1)
				return;

		/*	pItemDropParam = m_pDropRate->GetRandItem(nLuck); //���ľ��������Ʒ
			
			if (!pItemDropParam)
			   continue;*/

		   //printf("--ȡ����Ʒ:%d,%d,%d -- \n",pItemDropParam->nGenre,pItemDropParam->nDetailType,pItemDropParam->nParticulType);
			char nstrName[64]={0};
                 GetstrInfo(STR_GUISHU_NAME,nstrName);
		   if (strstr(nstrName,"ϵͳ"))    
		   {//����boss
			     nCurIdx = nBelongPlayer;
			     LoseSingleItem(nBelongPlayer,NULL,nLuckyRate,nLastDamageIdx);   //NPC����װ�� m_nLastDamageIdx	 
		   }
		   else 
		   {//˽��BOSS
			 //  int nNpcidx=NpcSet.SearchGSName(m_GuishuName);
			 
			    int mPlayerIdx = PlayerSet.FindNameID(nstrName);
				    nCurIdx = nBelongPlayer;

                if (mPlayerIdx>0)
				{//���� 
					    nCurIdx = mPlayerIdx;
				        LoseSingleItem(mPlayerIdx,NULL,nLuckyRate,nLastDamageIdx);   //NPC����װ��
				}
				else
				{//������
						LoseSingleItem(nBelongPlayer,NULL,nLuckyRate,nLastDamageIdx);   //NPC����װ��
				} 
		   }  
		}

		
		if (g_RandPercent(m_pDropRate->nMoneyRate));//GetItemParm(ITEM_PARM_MONEYRATE)))  //���ȡ���ʷ�Χ�� return ((int)g_Random(100) < nPercent);
		{   
			LoseMoney(nCurIdx);  //Ǯ
		} 
	}
}

// �������ʱ���������Ǯ����һ��object
void	KNpc::PlayerDeadCreateMoneyObj(int nMoneyNum)
{
	int		nX, nY,nMap;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY,&nMap);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);

	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);
	
	int nObjIdx = ObjSet.AddMoneyObj(Pos, nMoneyNum);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
		Object[nObjIdx].SetItemBelong(-1);
	}
}
//���µ�Ǯ��
void KNpc::LoseMoney(int nBelongPlayer,int nMoneyNum)
{

	if  (nBelongPlayer<=0)
		return ;

	int nX, nY,nMap;
	POINT	ptLocal;
	KMapPos	Pos;
    int  nJinQian=Npc[Player[nBelongPlayer].m_nIndex].IsJinQian;
	int  nMoneyRate[3];
         nMoneyRate[0]=1;
		 nMoneyRate[1]=0;
		 nMoneyRate[2]=100;
		g_GameSetTing.GetInt3("ServerConfig","MoneyRate",nMoneyRate);
//m_CurrentExperience
	int nMoney = (nMoneyRate[2]*Npc[Player[nBelongPlayer].m_nIndex].m_Level*nMoneyRate[0]* m_pDropRate->nMoneyScale*nJinQian)/100;	//GetItemParm(ITEM_PARM_MONEYSCALE)
	//TakeTrader(nMoneyRate[2]*Npc[Player[nBelongPlayer].m_nIndex].m_Level*nMoneyRate[0]* m_pDropRate->nMoneyScale*nJinQian,100);   //��Ǯ�ı���
	
	if (nMoney <= 0)
		return;

    if (nMoney>=nMoneyRate[1])
        nMoney=nMoneyRate[1]/2+GetRandomNumber(1,nMoneyRate[1]/2);
	//TakeTrader(nMoneyRate[1],2)+GetRandomNumber(1,TakeTrader(nMoneyRate[1],2));

	if (nMoneyNum>0)
		nMoney=GetRandomNumber(nMoneyNum/2,nMoneyNum);

	GetMpsPos(&nX, &nY,&nMap);
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);
	
	int nObjIdx = ObjSet.AddMoneyObj(Pos, nMoney);
	if (nObjIdx > 0 && nObjIdx < MAX_OBJECT)
	{
		if (nBelongPlayer > 0)
			Object[nObjIdx].SetItemBelong(nBelongPlayer);
		else
			Object[nObjIdx].SetItemBelong(-1);
	}

}

//�������ĳ��װ��
int  KNpc::DropOneItem(int nBelongPlayer,int nItemGenre,int nDetailType,int nParticulType,\
					   int nItemLevel, int nItemSeries,int nLuckey,int nRate,int nStackNum,\
					   int nMgCount,int nMgLevel,int IsBang,int IsHour,int IsQuality)
{
 /*   
	int nGoldLuck=g_GlobalMissionArray.GetMissionValue(28);//ȫ������
	if (nGoldLuck<=0)
        nGoldLuck=1;
*/
	if (nItemSeries<=-1 || nItemSeries>=5)
	      nItemSeries   = GetRandomNumber(0,4);//m_Series;
	
//==============���ľ��������Ʒ��==================
	time_t rawtime;
	struct tm * timeinfo;	
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	int bYear,bMonth,bDay,bHour,bMin,pnMagicLevel[6],nSLevel=0;
	ZeroMemory(pnMagicLevel,sizeof(pnMagicLevel));

	if (IsHour>0)
	{
      bYear  =timeinfo->tm_year+1900;
      bMonth =timeinfo->tm_mon+1;
      bDay   =timeinfo->tm_mday;
      bHour  =timeinfo->tm_hour+IsHour;
	  bMin   =timeinfo->tm_min;
	}
	else
	{
		bYear  =0;
		bMonth =0;
		bDay   =0;
		bHour  =0;
    	bMin   =0;
	}

	if (nMgCount<=0)
		nMgCount=GetRandomNumber(1,6);

	if (nStackNum<=0)
	   nStackNum=1;
	
	for (int j = 0; j <nMgCount; ++j)    // ���µ���������
	{
		if (!nItemGenre) 
		{//��װ��
			if (nMgLevel<=0)
			    nSLevel=GetRandomNumber(1,10);
			else
				nSLevel=nMgLevel;

				pnMagicLevel[j] =nSLevel;  //ħ�����Ե����� ħ���������������  ����  ���Ե�Ʒ�� 1-10��Χ
		}
		else
		{//Ϊ��װ  �������� 	 
			pnMagicLevel[j] = 0;  //������
		}
	}
//--------------------------------------------------------------------
	if (nItemLevel<=0)
        nItemLevel = 1;

	int nItemIdx=0;

    if (IsQuality==0)
		nItemIdx = ItemSet.AddOther(nItemGenre,nItemSeries,nItemLevel,nLuckey,\
		                  nDetailType, nParticulType, pnMagicLevel,\
		                  g_SubWorldSet.GetGameVersion(),0,GetRandomNumber(1,nStackNum),\
		                  0,0,bYear,bMonth,bDay,bHour,bMin,0,\
                            NULL,NULL,NULL,0);
	else if(IsQuality==1) //��װ�Ĳ���
	    nItemIdx = ItemSet.AddGold(nDetailType,pnMagicLevel,nItemSeries,0,0,\
		                   bYear,bMonth,bDay,bHour,bMin,NULL,NULL,nItemLevel,nLuckey);

	if (nItemIdx <= 0)
		return 0;

    if (IsBang)
        Item[nItemIdx].SetBang(IsBang);

	int		nX, nY,nMap;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY,&nMap);                         // ���������=ת���������

	ptLocal.x = nX;
	ptLocal.y = nY;

	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);  //��ȡ�ܵ��¶���������
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y,\
		              &Pos.nRegion, &Pos.nMapX, &Pos.nMapY,\
		                           &Pos.nOffX, &Pos.nOffY);

	int nObj;
	//////����װ��������KObjItemInfo//////�ṹ���еĲ������Ա����
	KObjItemInfo sInfo;
	memset(&sInfo, 0, sizeof(sInfo));
	sInfo.m_nItemID     = nItemIdx;                   //��Ʒ����
	sInfo.m_nItemWidth  = Item[nItemIdx].GetWidth();
	sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
	sInfo.m_nMoneyNum   = 0;
	strcpy(sInfo.m_szName, Item[nItemIdx].GetName()); //��Ʒ������
	sInfo.m_nColorID    = 0;                      //��ɫ
	sInfo.m_nMovieFlag  = 1;
	sInfo.m_nSoundFlag  = 1;
    sInfo.m_sHaveAttack = 0;   //�Ƿ�����Ϊ������Ч��
	//sInfo.m_AttackerDwid=0;   //�ϴι����ŵ�DWID
	nObj = ObjSet.Add(Item[nItemIdx].GetObjIdx(),Pos,sInfo);   //��ʼͬ��������Ʒ

	if (nObj == -1)
	{
		ItemSet.Remove(nItemIdx);
		return 0;
	}
	else
	{
		if (nBelongPlayer > 0)
		{
		   
              Object[nObj].SetItemBelong(nBelongPlayer);
		} 
		else
		{
			  Object[nObj].SetItemBelong(-1);
		}
	}

		return nItemIdx;

}

//NPC����װ��   NPC��װ��  NPC����װ�� ���PK����װ��
void KNpc::LoseSingleItem(int nBelongPlayer,CONST KItemDropRate::KItemParam* pItem,int LuckyRate,int LastDamageIdx,int IsWhere,int Series)
{
	if (!m_pDropRate /*|| m_IsClearDropRank!=0*/)  //���û�б��� �Ͳ�����
		return;

//	if  (!pItem)
//		return;

	if (m_pDropRate->nMaxItemLevelScale <= 0 || m_pDropRate->nMinItemLevelScale <= 0)
		return;

	int nRand = g_Random(m_pDropRate->nMaxRandRate);  //�ܱ���  ȡ����

	int nCheckRand  = 0;	// �ۼӸ��ʣ�ȷ���Ƿ�����������
    //int nIsThisItem =-1;
	int i;
	for (i = 0; i < m_pDropRate->nCount; ++i)   //ѭ����װ��  װ����������
	{
		if (nRand >= nCheckRand && nRand < nCheckRand + m_pDropRate->pItemParam[i].nRate)
		{//��������ڵ����ϸ���Ʒ���ۼƱ��ʲ�С�ڵ�ʱ�����Ʒ���ۼӱ���
			//nIsThisItem = i;
			break;
		}

		nCheckRand += m_pDropRate->pItemParam[i].nRate;   //ÿ����Ʒ���ʼ������ۼ�
	}

	if (i >= m_pDropRate->nCount)  //û�ҵ�������Ʒ�� �ͷ�����
		return;	
  
//==============���ľ��������Ʒ��=================
	int nGenre, nLuck=0, nDetail, nParticular, nLevel=1,nSLevel=0, pnMagicLevel[6],nQuality,nwMinItemLevel,nwMaxItemLevel,nIsBang=0,nHour=0,bYear=0,bMonth=0,bDay=0,bHour=0,bMin=0,n_Wlevel;  //Quality=1�ǻ�װ 2Ϊ�׽�3 Ϊ��װ ��
	int pnRMagicLevel[6],nIsNoBian=0,nMaxStackNum=1,nNpcCurLucky=0;
	ZeroMemory(pnRMagicLevel,sizeof(pnRMagicLevel));

	nGenre         = m_pDropRate->pItemParam[i].nGenre;
	nDetail        = m_pDropRate->pItemParam[i].nDetailType;
	nParticular    = m_pDropRate->pItemParam[i].nParticulType;

	if 	(nGenre<0)
		return;

	int nGoldLuck=g_GlobalMissionArray.GetMissionValue(28);//ȫ������
	if (nGoldLuck<=0)
        nGoldLuck=1;

	char nstrName[64]={0};
			Npc[LastDamageIdx].GetstrInfo(STR_GUISHU_NAME,nstrName);

	if (LastDamageIdx>0 && Npc[LastDamageIdx].m_Kind==kind_partner)
	{//ͬ��
	   if (strstr(nstrName,Npc[Player[nBelongPlayer].m_nIndex].Name))
	   {//���������˵Ļ� 
		   nNpcCurLucky= Npc[LastDamageIdx].m_nCurNpcLucky;
	   }
	}

	/*if (nNpcCurLucky==0 && nBelongPlayer>0 && Npc[Player[nBelongPlayer].m_nIndex].m_TongBanNum)
	{//�Ѿ��ٻ��˳����
		
		int nNpcIdx=0;
		    nNpcIdx =NpcSet.SearchID(Npc[Player[nBelongPlayer].m_nIndex].m_TongBanNum);
		if (nNpcIdx>0)
		{
			nNpcCurLucky=Npc[nNpcIdx].m_nCurNpcLucky;
		} 
	}*/

   if (nBelongPlayer>0)
	    nLuck= Player[nBelongPlayer].m_nCurLucky*Npc[Player[nBelongPlayer].m_nIndex].IsLuKey*nGoldLuck+nNpcCurLucky;  //��ҵ�����ֵ
	
	//if ((m_pDropRate->pItemParam[i].nRate+nLuck) < g_Random(nRand))
   //    	return;

	if  (Series<0 || Series>4)
	      Series   = GetRandomNumber(0,4);//m_Series;
	
    n_Wlevel       = Npc[Player[nBelongPlayer].m_nIndex].m_Level; //��ҵȼ�
	nQuality       = m_pDropRate->pItemParam[i].nQuality;         //�Ƿ�ƽ� ����װ
    nwMinItemLevel = m_pDropRate->pItemParam[i].nMinItemLevel;    //��Ʒ���ֵĵȼ�
    nwMaxItemLevel = m_pDropRate->pItemParam[i].nMaxItemLevel;    //��Ʒ�ĵȼ�
    nIsBang        = m_pDropRate->pItemParam[i].nIsBang;
    nHour          = m_pDropRate->pItemParam[i].nHour;
    nIsNoBian      = m_pDropRate->pItemParam[i].nIsNoBian;
    nMaxStackNum   = m_pDropRate->pItemParam[i].nStackNum;
//==============���ľ��������Ʒ��==================
	time_t rawtime;
	struct tm * timeinfo;	
	time (&rawtime);
	timeinfo = localtime (&rawtime);

	if (nHour>0)
	{
      bYear  =timeinfo->tm_year+1900;
      bMonth =timeinfo->tm_mon+1;
      bDay   =timeinfo->tm_mday;
      bHour  =timeinfo->tm_hour+nHour;
	  bMin   =timeinfo->tm_min;
	}
	else
	{
		bYear  =0;
		bMonth =0;
		bDay   =0;
		bHour  =0;
    	bMin   =0;
	}
//    printf("[���](%s)����(%d) OK...\n",Npc[Player[nBelongPlayer].m_nIndex].Name,nLuck);
    int nMaxLevel,nMinLevel,mCount=0,mAllLuck=nLuck/*+n_Wlevel+m_Level*/,nBossType=4,nIsNoRandomGoald=0;
    int nLuckItem[5];
	    nLuckItem[0]=0;nLuckItem[1]=0;nLuckItem[2]=0;nLuckItem[3]=0;nLuckItem[4]=0;
		
		//g_GameSetTing.GetInteger("SYSTEM","IsRandomGold",0,&nIsRandomGoald);
		nIsNoRandomGoald = m_pDropRate->pItemParam[i].nIsNoRandGold;
	    g_GameSetTing.GetInt5("SYSTEM","BlueMagciLuckyCount",nLuckItem);

	if (mAllLuck>=nLuckItem[0] && mAllLuck <=nLuckItem[1])
		mCount=GetRandomNumber(1,2);
	else if (mAllLuck>nLuckItem[1] && mAllLuck <=nLuckItem[2])
		mCount=GetRandomNumber(1,4);
	else if  (mAllLuck>nLuckItem[2] && mAllLuck<=nLuckItem[3])
		mCount=GetRandomNumber(1,5);
	else if (mAllLuck>nLuckItem[3] && mAllLuck <=nLuckItem[4])
	    mCount=GetRandomNumber(1,6);
	else if (mAllLuck>nLuckItem[4])
	    mCount=GetRandomNumber(1,12);
	else
        mCount=1;

	if (mCount>6)
		mCount=6;

	g_GameSetTing.GetInteger("SYSTEM","MagciCountFull",-1,&nBossType);

	if (nBossType>=0 && this->m_cGold.GetGoldType() >= nBossType)
	{
	   mCount=6;
	}

	//nLuck *=IsLuKey;

	BOOL bSkip = FALSE;

	int j;
	//printf("----���ľ��������Ʒ(%d,%d,%d)---\n",nGenre,nDetail,nParticular);
	for (j = 0; j <mCount; ++j)    // ���µ���������
	{
		if (!bSkip)
		{//����Ture ��ִ�е� GetItemParm(ITEM_PARM_MAGRATE)
			 if (g_Random(m_pDropRate->nMagicRate + nLuck) >= g_Random(LuckyRate))   // ȡ���� ����+��װ�ı���  ħ�����Եı���
			 {
				 if (nwMinItemLevel>0 && nwMaxItemLevel>0)
				 {//��Ʒָ���ĵȼ�	 
					 nLevel = GetRandomNumber(nwMinItemLevel,nwMaxItemLevel);  
				 }
				 else
				 {//NPC�ȼ� / �ȼ����� ==��Ʒ�ĵȼ�
					 nMaxLevel = m_Level/m_pDropRate->nMaxItemLevelScale;//GetItemParm(ITEM_PARM_MAXLEVELSCALE);
					 nMinLevel = m_Level/m_pDropRate->nMinItemLevelScale;//GetItemParm(ITEM_PARM_MINLEVELSCALE);
					 
					 if (nMaxLevel > m_pDropRate->nMaxItemLevel)  //ȫ���ȼ�����
						 nMaxLevel = m_pDropRate->nMaxItemLevel;
					 
					 if (nMinLevel < m_pDropRate->nMinItemLevel)
						 nMinLevel = m_pDropRate->nMinItemLevel;
					 
					 if (nMaxLevel < m_pDropRate->nMinItemLevel)
						 nMaxLevel = m_pDropRate->nMinItemLevel;
					 
					 if (nMinLevel > m_pDropRate->nMaxItemLevel)
						 nMinLevel = m_pDropRate->nMaxItemLevel;
					 
					 if (nMaxLevel < nMinLevel)
					 {
						 int nTemp;
						 nTemp     = nMinLevel;
						 nMinLevel = nMaxLevel;
						 nMaxLevel = nTemp;
					 }

					 nLevel = g_Random(nMaxLevel - nMinLevel) + nMinLevel; //*������� Ȼ��ȡ ȡ���� װ���ȼ�
				 } 


				 if (nLevel>=10)
					 nLevel = 10;

//////////////////////////����ĵȼ�Ӱ�����Եĵȼ�///////////////////////////////////////
				 g_GameSetTing.GetInt5("SYSTEM","BlueMagciLuckyLevel",nLuckItem);
				 //m_IsNoLevel
				 if (m_IsNoLevel<=0)
				 {//��������ȼ�
					 int nCha=0;
					 if ((n_Wlevel-m_Level)>0)
						 nCha=n_Wlevel-m_Level;
					 else
						 nCha=m_Level-n_Wlevel;                //�ȼ���

					 if (nCha<GetRandomNumber(1,nLuckItem[0])) //�Աȵĵȼ�
					 {//��Ʒ��Ʒ����������ǰ�װ
						  nSLevel =GetRandomNumber(1,10);
					 } 
					 else
						  nSLevel =GetRandomNumber(0,nLuckItem[1]); 
				 }
				 else
				 {//��������ȼ��Ĳ�� 
					 nSLevel = nLevel;
				 }
				 		 
				 pnMagicLevel[j] = nSLevel;  //ħ�����Ե����� ħ���������������  ����  ���Ե�Ʒ�� 1-10��Χ

				 if (nSLevel==0)
					bSkip = TRUE;
			 }
			 else
			 {//���ʲ��������Χ�� ���ٵ��������Ե�
				     pnMagicLevel[j] = 0; // ��װ��
				     bSkip = TRUE;        // ���ٵ���װ����
			 }
		}
		else
		{//Ϊ��װ  �������� 
			if (nwMinItemLevel>0 && nwMaxItemLevel>0)
			{//��Ʒָ���ĵȼ�	 
				nLevel = GetRandomNumber(nwMinItemLevel,nwMaxItemLevel);  
			}
			pnMagicLevel[j] = 0;  //������
		}
	}

	int nItemIdx=0,nRpiont=0,nMagicNum=0; 

	//-------��װnLevel��Ʒ�Ĳ���Ӱ��װ������۵ȼ�---��ʶ��������----	
	ZeroMemory(pnRMagicLevel,sizeof(pnRMagicLevel));

	g_GameSetTing.GetInteger("ServerConfig","MagicNum",0,&nMagicNum);

    if (nGenre==0 && m_pDropRate->nIsBianShi && pnMagicLevel[nMagicNum] && nIsNoBian!=1)
	{//�ж��ٸ��������ϵĲſ��Ա���ʶװ��
		g_GameSetTing.GetInt5("SYSTEM","BianShiLuckyCount",nLuckItem);

		if (nLuck>=nLuckItem[0] && nLuck <=nLuckItem[1])
			nRpiont = GetRandomNumber(0,1);
		else if (nLuck>nLuckItem[1] && nLuck <=nLuckItem[2])
			nRpiont = GetRandomNumber(0,2);
		else if  (nLuck>nLuckItem[2] && nLuck <=nLuckItem[3])
			nRpiont = GetRandomNumber(0,3);
		else if (nLuck>nLuckItem[3]&& nLuck <=nLuckItem[4])
			nRpiont = GetRandomNumber(0,4);
		else if (nLuck>nLuckItem[4])
			nRpiont = GetRandomNumber(0,6);
		else
            nRpiont = 0;

        if (nRpiont>0)
		{ //�Ƿ������ʶ����
		/*	KTabFile nRongt;

		nRongt.Load(TABFILE_FUSION_PATH);
		int nRows = nRongt.GetHeight(),nAddRows=0;

	    g_GameSetTing.GetInt5("SYSTEM","BianShiLuckyLevel",nLuckItem);

		if (nLuck>=nLuckItem[0] && nLuck <=nLuckItem[1])
			nAddRows=GetRandomNumber(0,2);
		else if (nLuck>nLuckItem[1] && nLuck <=nLuckItem[2])
			nAddRows=GetRandomNumber(0,3);
		else if  (nLuck>nLuckItem[2] && nLuck <=nLuckItem[3])
			nAddRows=GetRandomNumber(0,4);
		else if (nLuck>nLuckItem[3] && nLuck <=nLuckItem[4])
			nAddRows=GetRandomNumber(0,5);
		else if (nLuck >nLuckItem[4])
			nAddRows=GetRandomNumber(0,8);
		else
			nAddRows=GetRandomNumber(0,1);*/
		
		  for (j=0;j<nRpiont;++j)
		  { //�������Ը�ֵ == ����
			  pnRMagicLevel[j] = 0;//isHave; //�趨����			
		  } 
		 
		  IsWhere = -1;                      //����δ��ʶ

          int nTime = 1;
		    g_GameSetTing.GetInteger("ServerConfig","BianShiTime",1,&nTime);

          if (pnRMagicLevel[0])
		  {  
            bYear  =timeinfo->tm_year+1900;
			bMonth =timeinfo->tm_mon+1;
			bDay   =timeinfo->tm_mday;
			bHour  =timeinfo->tm_hour+nTime;   //Ĭ����һ��Сʱ����ʶ
	        bMin   =timeinfo->tm_min;
		  } 
		}
        else
		{  
            nRpiont = 0;
		}   

	}   

    nLuckItem[0]=0;nLuckItem[1]=0;nLuckItem[2]=0;nLuckItem[3]=0;nLuckItem[4]=0;
//--------------------------------------------------------------------
	if (nLevel<=0)
        nLevel = 1;
	int nForBitItem[3];
	    g_GameSetTing.GetInt3("ServerConfig","AutoPlayForBitItem",nForBitItem);

	if (nBelongPlayer>0 && Npc[Player[nBelongPlayer].m_nIndex].m_AutoplayId>0 && nForBitItem[0] && timeinfo->tm_hour>=nForBitItem[1] && timeinfo->tm_hour<nForBitItem[2])
	{//�һ��� ������������ʱ��ε� ��ʼ��� ���ܵ������Ʒ
		if (CheckAllItem(3,nGenre,nDetail,nParticular))
			return;
	}
	//nLuck *=IsLuKey
	int nSerLucky = 0;

	if (nLuck*IsLuKey>2000000000)
		nSerLucky = 2000000000;
	else
		nSerLucky = nLuck*IsLuKey;

    if (nQuality==0)
		nItemIdx = ItemSet.AddOther(nGenre,Series,nLevel,nSerLucky,\
		                  nDetail, nParticular, pnMagicLevel,\
		                  g_SubWorldSet.GetGameVersion(),0,GetRandomNumber(1,nMaxStackNum),\
		            0,0,bYear,bMonth,bDay,bHour,bMin,nRpiont,\
                            pnRMagicLevel,NULL,NULL,IsWhere);
	else if(nQuality>0) //��װ�Ĳ���
	{
		if (nIsNoRandomGoald<=0)
		  nItemIdx = ItemSet.AddRondomGold(nQuality,pnMagicLevel,Series,0,0,bYear,bMonth,bDay,bHour,bMin,NULL,nLevel,nSerLucky,nDetail);
		else
          nItemIdx = ItemSet.AddGold(nQuality,pnMagicLevel,Series,0,0,bYear,bMonth,bDay,bHour,bMin,NULL,NULL,nLevel,nSerLucky);
	}  
	//char msg[80];
	//t_sprintf(msg,"S����:%s(%d,%d,%d,%d)",Item[nItemIdx].GetName(),nGenre,nDetail,nParticular,nSLevel);
    //Player[nBelongPlayer].m_ItemList.msgshow(msg);//m_PlayerIdx

	if (nItemIdx <= 0)
		return;

    if (nIsBang)
          Item[nItemIdx].SetBang(nIsBang);

	int		nX, nY,nMap;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY,&nMap);                         // ���������=ת���������

	ptLocal.x = nX;
	ptLocal.y = nY;

	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);  //��ȡ�ܵ��¶���������
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y,\
		              &Pos.nRegion, &Pos.nMapX, &Pos.nMapY,\
		                           &Pos.nOffX, &Pos.nOffY);

	int nObj;
	//////����װ��������KObjItemInfo//////�ṹ���еĲ������Ա����
	KObjItemInfo sInfo;
	memset(&sInfo, 0, sizeof(sInfo));
	sInfo.m_nItemID     = nItemIdx;                   //��Ʒ����
	sInfo.m_nItemWidth  = Item[nItemIdx].GetWidth();
	sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
	sInfo.m_nMoneyNum   = 0;
	strcpy(sInfo.m_szName, Item[nItemIdx].GetName()); //��Ʒ������
	sInfo.m_nColorID    = 0;                      //��ɫ
	sInfo.m_nMovieFlag  = 1;
	sInfo.m_nSoundFlag  = 1;
    sInfo.m_sHaveAttack = 0;   //�Ƿ�����Ϊ������Ч��
	//sInfo.m_AttackerDwid=0;   //�ϴι����ŵ�DWID
	nObj = ObjSet.Add(Item[nItemIdx].GetObjIdx(),Pos,sInfo);   //��ʼͬ��������Ʒ

	if (nObj == -1)
	{
		ItemSet.Remove(nItemIdx);
	}
	else
	{
		if (nBelongPlayer > 0)
		{
		    //int nmIdx=0;
            //nmIdx=NpcSet.SearchName(Npc[nAttacker].m_GuishuName);   //���ҹ����ߵ�����
            //m_cDeathCalcExp.AddDamage(Npc[nmIdx].m_nPlayerIdx, (m_CurrentLife - nDamage > 0 ? nDamage : m_CurrentLife));
	     	if (Npc[LastDamageIdx].m_Kind==kind_partner)
			{//�����ͬ��Ļ�
			   	int nmIdx=0;
				char nstrName[64]={0};
			         Npc[LastDamageIdx].GetstrInfo(STR_GUISHU_NAME,nstrName);
                    nmIdx=NpcSet.SearchName(nstrName);   //���ҹ����ߵ�����
					//if (strstr(Npc[Player[nBelongPlayer].m_nIndex].Name,Npc[LastDamageIdx].m_GuishuName))
					if (nmIdx)
					{
					    Object[nObj].SetItemBelong(Npc[nmIdx].GetPlayerIdx());
					}
					else
					    Object[nObj].SetItemBelong(-1);
			}
			else
              Object[nObj].SetItemBelong(nBelongPlayer);
		} 
		else
		{
			if (Npc[LastDamageIdx].m_Kind==kind_partner)
			{//�����ͬ��Ļ�
				int nmIdx=0;
			         Npc[LastDamageIdx].GetstrInfo(STR_GUISHU_NAME,nstrName);
				nmIdx=NpcSet.SearchName(nstrName);   //���ҹ����ߵ�����
				//if (strstr(Npc[Player[nBelongPlayer].m_nIndex].Name,Npc[LastDamageIdx].m_GuishuName))
				if (nmIdx)
				{
					Object[nObj].SetItemBelong(Npc[nmIdx].GetPlayerIdx());
				}
				else
					Object[nObj].SetItemBelong(-1);
			}
			else
			  Object[nObj].SetItemBelong(-1);
		}
	}
}

//NPC�ű�����װ��   NPC��װ��  NPC����װ�� ���PK����װ��
int KNpc::DropSingleItem(int nBelongPlayer,int nItemGenre,int nDetailType,int nParticulType,int nRate,int nItemMinLevel,int nItemMaxLevel, int nItemSeries,int nStackNum,int *JBlevel,int nMgCount,int nMgLevel,int IsBang,int IsHour,int IsQuality)
{
	if (!nRate)  //���û�б��� �Ͳ�����
		return FALSE;

	if (g_Random(nRate) < g_Random(2000)) //���ʼ��
		return FALSE;

    if (nItemSeries==-1)
        nItemSeries=GetRandomNumber(0,4);

	int LuckyRate;
      g_GameSetTing.GetInteger("ServerConfig","LuckyRate",5000,&LuckyRate);

	int nGenre, nSeries, nLuck, nDetail, nParticular, nLevel=1,nSLevel=0, pnMagicLevel[6],nQuality,nwMinItemLevel,nwMaxItemLevel,nIsBang,nHour,bYear,bMonth,bDay,bHour,bMin,n_Wlevel;  //Quality=1�ǻ�װ 2Ϊ�׽�3 Ϊ��װ ��
	nGenre         = nItemGenre;
	nDetail        = nDetailType;
	nParticular    = nParticulType;
	nSeries        = nItemSeries;//m_Series;
	nLuck          = Player[nBelongPlayer].m_nCurLucky*Npc[Player[nBelongPlayer].m_nIndex].IsLuKey;  //��ҵ�����ֵ
    n_Wlevel       = Npc[Player[nBelongPlayer].m_nIndex].m_Level; //��ҵȼ�
	nQuality       = IsQuality;  //�Ƿ�ƽ�
    nwMinItemLevel = nItemMinLevel;  //��Ʒ���ֵĵȼ�
    nwMaxItemLevel = nItemMaxLevel;
    nIsBang        = IsBang;
    nHour          = IsHour;

	time_t rawtime;
	struct tm * timeinfo;	
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	if (nHour>0)
	{
       bYear   =timeinfo->tm_year+1900;
       bMonth  =timeinfo->tm_mon+1;
       bDay    =timeinfo->tm_mday;
       bHour   =timeinfo->tm_hour+nHour;
	   bMin    =timeinfo->tm_min;
	}
	else
	{
        bYear   =0;
		bMonth  =0;
		bDay    =0;
		bHour   =0;
		bMin    =0;
	}
//   printf("[���](%s)����(%d) OK...\n",Npc[Player[nBelongPlayer].m_nIndex].Name,nLuck);
    int nMaxLevel,nMinLevel,mCount=0;

	int nLuckItem[5];
	    nLuckItem[0]=0;nLuckItem[1]=0;nLuckItem[2]=0;nLuckItem[3]=0;nLuckItem[4]=0;

	    g_GameSetTing.GetInt5("SYSTEM","BlueMagciLuckyCount",nLuckItem);

	if (nLuck>=nLuckItem[0] && nLuck <=nLuckItem[1])
		mCount=GetRandomNumber(1,2);
	else if (nLuck>nLuckItem[1] && nLuck <=nLuckItem[2])
		mCount=GetRandomNumber(1,4);
	else if  (nLuck>nLuckItem[2] && nLuck <=nLuckItem[3])
		mCount=GetRandomNumber(1,5);
	else if (nLuck>nLuckItem[3] && nLuck <=nLuckItem[4])
		mCount=GetRandomNumber(1,6);
	else if (nLuck>nLuckItem[4])
		mCount=GetRandomNumber(2,6);
	else
        mCount=1;
/////////////////////////////////////////////////////////////////////////////////////////////////
	if (nwMinItemLevel>0 &&  nwMaxItemLevel>0)
	{
		nLevel = GetRandomNumber(nwMinItemLevel,nwMaxItemLevel);  //��Ʒָ���ĵȼ�	 
	}
	else
	{ 
		nLevel = GetRandomNumber(1,7);
	} 
/////////////////////////////////////////////////////////////////////
	BOOL	bSkip = FALSE;
	for (int j = 0; j <mCount; ++j)    // ���µ���������
	{
		if (!bSkip)
		{
			 if (g_Random(nRate + nLuck) >= g_Random(LuckyRate))   // ȡ���� ����+��װ�ı���  ħ�����Եı���
			 {
//////////////////////////����ĵȼ�Ӱ�����Եĵȼ�////////////////////////////////////////////////////////////////
				if (nMgLevel>0)
				{
                  nSLevel=nMgLevel;
				}
				else
				{
				 int nCha=0;
				 if ((n_Wlevel-m_Level)>0)
					 nCha=n_Wlevel-m_Level;
				 else
					 nCha=m_Level-n_Wlevel;  //�ȼ���
				 
				 int mBiJIAO=GetRandomNumber(1,250);

				  g_GameSetTing.GetInt5("SYSTEM","BlueMagciLuckyLevel",nLuckItem);

					 if (nCha<mBiJIAO)
					 {
					   if (nLuck>=nLuckItem[0] && nLuck <=nLuckItem[1])
						 nSLevel=GetRandomNumber(1,2);
					   else if (nLuck>nLuckItem[1] && nLuck <=nLuckItem[2])
						 nSLevel=GetRandomNumber(1,3);
					   else if  (nLuck>nLuckItem[2] && nLuck <=nLuckItem[3])
						 nSLevel=GetRandomNumber(1,4);
					   else if (nLuck>nLuckItem[3] && nLuck <=nLuckItem[4])
						 nSLevel=GetRandomNumber(1,5);
					   else if (nLuck >nLuckItem[4])
						   nSLevel=GetRandomNumber(1,10);
					   else
                         nSLevel=GetRandomNumber(0,1);		 
					 } 
				     else
					     nSLevel=GetRandomNumber(0,1);
				}
				     pnMagicLevel[j] =nSLevel;  //ħ�����Ե����� ħ���������������  ����
			 }
			 else
			 {
				     pnMagicLevel[j] = 0; //��װ��
				     bSkip = TRUE;
			 }
		}
		else
		{
			if (nwMinItemLevel>0 &&  nwMaxItemLevel>0)
			{//��Ʒָ���ĵȼ�	 
				nLevel = GetRandomNumber(nwMinItemLevel,nwMaxItemLevel);  
			}

			pnMagicLevel[j] = 0;  //������
		}
	}


    nLuckItem[0]=0;nLuckItem[1]=0;nLuckItem[2]=0;nLuckItem[3]=0;nLuckItem[4]=0;

	int nIdx=0; 
	if (nLevel<=0)
        nLevel = 1;

	if (nQuality==0)
         nIdx = ItemSet.AddOther(nGenre,nSeries, nLevel,nLuck, nDetail,\
	        nParticular, pnMagicLevel, g_SubWorldSet.GetGameVersion(),\
			0,nStackNum,0,0,bYear,bMonth,bDay,bHour,bMin,0,NULL,JBlevel,NULL,0);
	/*

  bYear   =0;
		bMonth  =0;
		bDay    =0;
		bHour   =0;
		bMin    =0;
	AddOther(IN int nItemGenre, IN int nSeries, IN int nLevel=1, IN int nLuck=0, IN int nDetail = -1, \
		                   IN int nParticular = -1, IN int* pnMagicLevel = NULL, IN int nVersion = 0,\
			    IN UINT nRandomSeed = 0,IN int nStackNum = 1, IN int nEnChance = 0,IN int nPoint = 0, \
			  IN int nYear = 0, IN int nMonth = 0, IN int nDay = 0, IN int nHour = 0,IN int nMin = 0,\
						                               IN int nRongpiont=0,IN int* pnRMagicLevel = NULL,\
						           IN int* pnJBLevel = NULL,IN int* pnBsLevel=NULL,IN int IsWhere=0); 
	*/
//��װ�Ĳ���
	else if(nQuality==1)
	{
		for (int i=0;i<6;++i)
			pnMagicLevel[i]=JBlevel[i];

	     nIdx = ItemSet.AddGold(nDetail,pnMagicLevel,nSeries,0,0,
	                                bYear,bMonth,bDay,bHour,bMin,
						                  NULL,NULL,nLevel,nLuck);
	}
	
	if (nIdx <= 0)
		return FALSE;

	if (nIsBang)
		Item[nIdx].SetBang(nIsBang);

	int		nX, nY,nMap;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY,&nMap);  // ת���������
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);

	int nObj;
	//////����װ��������KObjItemInfo//////�ṹ���еĲ������Ա����
	KObjItemInfo sInfo;
    memset(&sInfo, 0, sizeof(sInfo));
	sInfo.m_nItemID = nIdx;
	sInfo.m_nItemWidth = Item[nIdx].GetWidth();
	sInfo.m_nItemHeight = Item[nIdx].GetHeight();
	sInfo.m_nMoneyNum = 0;
	t_sprintf(sInfo.m_szName, Item[nIdx].GetName()); //��Ʒ������
	sInfo.m_nColorID = 0;
	sInfo.m_nMovieFlag = 1;
	sInfo.m_nSoundFlag = 1;
    sInfo.m_sHaveAttack=0;   //�Ƿ�����Ϊ������Ч��
	//sInfo.m_AttackerDwid=0;   //�ϴι����ŵ�DWID

	nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), Pos, sInfo);
	if (nObj == -1)
	{
		ItemSet.Remove(nIdx);
	}
	else
	{
		if (nBelongPlayer > 0)
			Object[nObj].SetItemBelong(nBelongPlayer);
		else
			Object[nObj].SetItemBelong(-1);
	}

	return nIdx;
}
//NPC�ű�������ʶװ��
int KNpc::DropBianShiItem(int nBelongPlayer,int nItemGenre,int nDetailType,int nParticulType,int nRate,int nItemMinLevel,int nItemMaxLevel, int nItemSeries,int nRpiont,int IsWhere,int nMgCount,int nMgLevel,int IsBang,int IsHour,int IsMin,int IsQuality)
{
	if (!nRate)  //���û�б��� �Ͳ�����
		return FALSE;
	if (g_Random(nRate) < g_Random(2000)) //���ʼ��
		return FALSE;

	int LuckyRate;
      g_GameSetTing.GetInteger("ServerConfig","LuckyRate",5000,&LuckyRate);

	int nGenre, nSeries, nLuck, nDetail, nParticular, nLevel=1,nSLevel=0, pnMagicLevel[6],nQuality,nwMinItemLevel,nwMaxItemLevel,nIsBang,nHour,nMin,bYear,bMonth,bDay,bHour,bMin,n_Wlevel;  //Quality=1�ǻ�װ 2Ϊ�׽�3 Ϊ��װ ��
	int pnRMagicLevel[6];

	ZeroMemory(pnRMagicLevel,sizeof(pnRMagicLevel));

	nGenre         = nItemGenre;
	nDetail        = nDetailType;
	nParticular    = nParticulType;
	nSeries        = nItemSeries;//m_Series;
	nLuck          = Player[nBelongPlayer].m_nCurLucky*Npc[Player[nBelongPlayer].m_nIndex].IsLuKey;  //��ҵ�����ֵ
    n_Wlevel       = Npc[Player[nBelongPlayer].m_nIndex].m_Level; //��ҵȼ�
	nQuality       = IsQuality;      //�Ƿ�ƽ�
    nwMinItemLevel = nItemMinLevel;  //��Ʒ���ֵĵȼ�
    nwMaxItemLevel = nItemMaxLevel;
    nIsBang        = IsBang;
    nHour          = IsHour;
	nMin           = IsMin;
 //   nMin=m_pDropRate->pItemParam[i].nMin;
	time_t rawtime;
	struct tm * timeinfo;	
	time (&rawtime);
	timeinfo = localtime ( &rawtime );

	if (nHour>0 || nMin>0)
	{
		bYear   =timeinfo->tm_year+1900;
		bMonth  =timeinfo->tm_mon+1;
		bDay    =timeinfo->tm_mday;
		bHour   =timeinfo->tm_hour+nHour;
		bMin    =timeinfo->tm_min+nMin;
	}
	else
	{
		bYear   = 0;
		bMonth  = 0;
		bDay    = 0;
		bHour   = 0;
		bMin    = 0;
	}

    int nMaxLevel,nMinLevel,mCount=0;
	int nLuckItem[5];
	    nLuckItem[0]=0;nLuckItem[1]=0;nLuckItem[2]=0;nLuckItem[3]=0;nLuckItem[4]=0;

	 g_GameSetTing.GetInt5("SYSTEM","BlueMagciLuckyCount",nLuckItem);

	if (nLuck>=nLuckItem[0] && nLuck <=nLuckItem[1])
		mCount=GetRandomNumber(1,2);
	else if (nLuck>nLuckItem[1] && nLuck <=nLuckItem[2])
		mCount=GetRandomNumber(1,4);
	else if  (nLuck>nLuckItem[2] && nLuck <=nLuckItem[3])
		mCount=GetRandomNumber(1,5);
	else if (nLuck>nLuckItem[3] && nLuck <=nLuckItem[4])
		mCount=GetRandomNumber(1,6);
	else if (nLuck>nLuckItem[4])
		mCount=GetRandomNumber(2,6);
	else
        mCount=1;
/////////////////////////////////////////////////////////////////////////////////////////////////
	if (nwMinItemLevel>0 &&  nwMaxItemLevel>0)
	{
		nLevel = GetRandomNumber(nwMinItemLevel,nwMaxItemLevel);  //��Ʒָ���ĵȼ�	 
	}
	else
	{ 
		nLevel = GetRandomNumber(1,7);
	} 
/////////////////////////////////////////////////////////////////////
	BOOL	bSkip = FALSE;
    int j;
	for (j = 0; j <mCount; ++j)    // ���µ���������
	{
		if (!bSkip)
		{
			 if (g_Random(nRate + nLuck) >= g_Random(LuckyRate))   // ȡ���� ����+��װ�ı���  ħ�����Եı���
			 {
//////////////////////////����ĵȼ�Ӱ�����Եĵȼ�////////////////////////////////////////////////////////////////
				if (nMgLevel>0)
				{
                  nSLevel=nMgLevel;
				}
				else
				{
				 int nCha=0;
				 if ((n_Wlevel-m_Level)>0)
					 nCha=n_Wlevel-m_Level;
				 else
					 nCha=m_Level-n_Wlevel;  //�ȼ���
				 
				 int mBiJIAO=GetRandomNumber(1,250);

                 g_GameSetTing.GetInt5("SYSTEM","BlueMagciLuckyLevel",nLuckItem);

				if (nCha<mBiJIAO)
				{
					   if (nLuck>=nLuckItem[0] && nLuck <=nLuckItem[1])
						 nSLevel=GetRandomNumber(1,2);
					   else if (nLuck>nLuckItem[1] && nLuck <=nLuckItem[2])
						 nSLevel=GetRandomNumber(1,3);
					   else if  (nLuck>nLuckItem[2] && nLuck <=nLuckItem[3])
						 nSLevel=GetRandomNumber(1,4);
					   else if (nLuck>nLuckItem[3] && nLuck <=nLuckItem[4])
						 nSLevel=GetRandomNumber(1,5);
					   else if (nLuck >nLuckItem[4])
						   nSLevel=GetRandomNumber(1,10);
					   else
                         nSLevel=GetRandomNumber(0,1);		 
				 }
				 else
                         nSLevel=GetRandomNumber(0,1);		 
				}
				     pnMagicLevel[j] =nSLevel;  //ħ�����Ե����� ħ���������������  ����
			 }
			 else
			 {
				     pnMagicLevel[j] = 0;       //��װ��
				     bSkip = TRUE;
			 }
		}
		else
		{
			if (nwMinItemLevel>0 &&  nwMaxItemLevel>0)
			{//��Ʒָ���ĵȼ�	 
				nLevel = GetRandomNumber(nwMinItemLevel,nwMaxItemLevel);  
			}
			pnMagicLevel[j] = 0;  //������
		}
	}
	int nIdx=0,nMagicNum=0; ; 
//-------��װnLevel��Ʒ�Ĳ���Ӱ��װ������۵ȼ�---------------------

	g_GameSetTing.GetInteger("ServerConfig","MagicNum",0,&nMagicNum);

	if (nRpiont>0 && pnMagicLevel[nMagicNum] && nGenre==0)
	{//����װ
	   /* KTabFile nRongt;
       nRongt.Load(TABFILE_FUSION_PATH);
	   int nRows = nRongt.GetHeight(),nAddRows;

	  g_GameSetTing.GetInt5("SYSTEM","BianShiLuckyLevel",nLuckItem);

		if (nLuck>=nLuckItem[0] && nLuck <=nLuckItem[1])
			nAddRows=GetRandomNumber(0,2);
		else if (nLuck>nLuckItem[1] && nLuck <=nLuckItem[2])
			nAddRows=GetRandomNumber(0,3);
		else if  (nLuck>nLuckItem[2] && nLuck <=nLuckItem[3])
			nAddRows=GetRandomNumber(0,4);
		else if (nLuck>nLuckItem[3] && nLuck <=nLuckItem[4])
			nAddRows=GetRandomNumber(0,5);
		else if (nLuck >nLuckItem[4])
			nAddRows=GetRandomNumber(0,8);
		else
			nAddRows=GetRandomNumber(0,1);*/

       for (j=0;j<nRpiont;++j)
	   {//�������Ը�ֵ == ����
	  /*	 
	    KTabFile nRongt;
       nRongt.Load(TABFILE_FUSION_PATH);
	   int nRows = nRongt.GetHeight(),nAddRows;
	
	     // int bRows = GetRandomNumber(3,nRows);
		 // int isHave=0;
		 //	  nRongt.GetInteger(bRows,"ħ����������1",0,&isHave);
		*/

         // if (pnRMagicLevel[j]==0)
		  //{
              pnRMagicLevel[j] = 0;//bRows; //�趨����
			  //IsWhere++;
		 // }

	   }
       IsWhere = -1;                  //����δ��ʶ
       int nTime = 1;
	   g_GameSetTing.GetInteger("ServerConfig","BianShiTime",1,&nTime);

	   if (pnRMagicLevel[0])
	   {//����Ϊδ��ʶ
		   bYear  =timeinfo->tm_year+1900;
		   bMonth =timeinfo->tm_mon+1;
		   bDay   =timeinfo->tm_mday;
		   bHour  =timeinfo->tm_hour+nTime;   //Ĭ����һ��Сʱ����ʶ
		   bMin   =timeinfo->tm_min;
//		   IsWhere = -1;                  //����δ��ʶ
		}
//	    nRongt.Clear();
}
	nLuckItem[0]=0;nLuckItem[1]=0;nLuckItem[2]=0;nLuckItem[3]=0;nLuckItem[4]=0;
//----------------------------------------------------------------
//��װ ��װ ��װ �����������Ĳ���
	if (nLevel<=0)
        nLevel = 1;

    if (nQuality==0)
	   nIdx = ItemSet.AddOther(nGenre,nSeries, nLevel,nLuck,\
	                           nDetail, nParticular, pnMagicLevel, \
							   g_SubWorldSet.GetGameVersion(),0,1,\
                               0,0,bYear,bMonth,bDay,bHour,bMin,nRpiont,\
                               pnRMagicLevel,NULL,NULL,IsWhere);
//��װ�Ĳ���
	else if(nQuality==1)
	    nIdx = ItemSet.AddGold(nDetail,pnMagicLevel,nSeries,0,0,\
		                       bYear,bMonth,bDay,bHour,bMin,\
							   NULL,NULL,nLevel,nLuck);
	
	if (nIdx <= 0)
		return FALSE;

	int		nX, nY,nMap;
	POINT	ptLocal;
	KMapPos	Pos;

	GetMpsPos(&nX, &nY,&nMap);  // ת���������
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);

	int nObj;
	//////����װ��������KObjItemInfo//////�ṹ���еĲ������Ա����
	KObjItemInfo sInfo;
	memset(&sInfo, 0, sizeof(sInfo));
	sInfo.m_nItemID = nIdx;
	sInfo.m_nItemWidth = Item[nIdx].GetWidth();
	sInfo.m_nItemHeight = Item[nIdx].GetHeight();
	sInfo.m_nMoneyNum = 0;
	t_sprintf(sInfo.m_szName, Item[nIdx].GetName()); //��Ʒ������
	sInfo.m_nColorID = 0;
	sInfo.m_nMovieFlag = 1;
	sInfo.m_nSoundFlag = 1;
    sInfo.m_sHaveAttack=0;   //�Ƿ�����Ϊ������Ч��
	//sInfo.m_AttackerDwid=0;   //�ϴι����ŵ�DWID

	nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), Pos, sInfo);
	if (nObj == -1)
	{
		ItemSet.Remove(nIdx);
	}
	else
	{
		if (nBelongPlayer > 0)
			Object[nObj].SetItemBelong(nBelongPlayer);
		else
			Object[nObj].SetItemBelong(-1);
	}

	return nIdx;
}



int KNpc::GiveSingleItem(int nBelongPlayer,int nItemGenre,int nDetailType,int nParticulType,int nRate,int nItemMinLevel,int nItemMaxLevel, int nItemSeries,int nMgCount,int nMgLevel,int IsBang,int IsHour,int IsQuality)
{
	if (!nRate)  //���û�б��� �Ͳ�����
		return FALSE;

	if (g_Random(nRate) < g_Random(2000)) //���ʼ��
		return FALSE;

    if (nItemSeries==-1)
        nItemSeries=GetRandomNumber(0,4);

	int LuckyRate;
      g_GameSetTing.GetInteger("ServerConfig","DuanZaoLuckyRate",5000,&LuckyRate);

	int nGenre, nSeries, nLuck, nDetail, nParticular, nLevel=1,nSLevel=0, pnMagicLevel[6],nQuality,nwMinItemLevel,nwMaxItemLevel,nIsBang,nHour,bYear,bMonth,bDay,bHour,bMin,n_Wlevel;  //Quality=1�ǻ�װ 2Ϊ�׽�3 Ϊ��װ ��
	nGenre         = nItemGenre;
	nDetail        = nDetailType;
	nParticular    = nParticulType;
	nSeries        = nItemSeries;//m_Series;
	nLuck          = Player[nBelongPlayer].m_nCurLucky*Npc[Player[nBelongPlayer].m_nIndex].IsLuKey;  //��ҵ�����ֵ
    n_Wlevel       = Npc[Player[nBelongPlayer].m_nIndex].m_Level; //��ҵȼ�
	nQuality       = IsQuality;  //�Ƿ�ƽ�
    nwMinItemLevel = nItemMinLevel;  //��Ʒ���ֵĵȼ�
    nwMaxItemLevel = nItemMaxLevel;
    nIsBang        = IsBang;
    nHour          = IsHour;

	time_t rawtime;
	struct tm * timeinfo;	
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	if (nHour>0)
	{
       bYear   =timeinfo->tm_year+1900;
       bMonth  =timeinfo->tm_mon+1;
       bDay    =timeinfo->tm_mday;
       bHour   =timeinfo->tm_hour+nHour;
	   bMin    =timeinfo->tm_min;
	}
	else
	{
        bYear   =0;
		bMonth  =0;
		bDay    =0;
		bHour   =0;
		bMin    =0;
	}

    int nMaxLevel,nMinLevel,mCount=0;

	int nLuckItem[5];
	    nLuckItem[0]=0;nLuckItem[1]=0;nLuckItem[2]=0;nLuckItem[3]=0;nLuckItem[4]=0;

	    g_GameSetTing.GetInt5("ServerConfig","DuanZaoBlueMagciLucky",nLuckItem);

	if (nLuck>=nLuckItem[0] && nLuck <=nLuckItem[1])
		mCount=GetRandomNumber(1,2);
	else if (nLuck>nLuckItem[1] && nLuck <=nLuckItem[2])
		mCount=GetRandomNumber(1,4);
	else if  (nLuck>nLuckItem[2] && nLuck <=nLuckItem[3])
		mCount=GetRandomNumber(1,5);
	else if (nLuck>nLuckItem[3] && nLuck <=nLuckItem[4])
		mCount=GetRandomNumber(1,6);
	else if (nLuck>nLuckItem[4])
		mCount=GetRandomNumber(2,6);  //��������
	else
        mCount=1;
/////////////////////////////////////////////////////////////////////////////////////////////////
	if (nwMinItemLevel>0 &&  nwMaxItemLevel>0)
	{
		nLevel = GetRandomNumber(nwMinItemLevel,nwMaxItemLevel);  //��Ʒָ���ĵȼ�	 
	}
	else
	{ 
		nLevel = GetRandomNumber(1,7);
	} 
/////////////////////////////////////////////////////////////////////
	BOOL	bSkip = FALSE;
	for (int j = 0; j <mCount; ++j)    // ���µ���������
	{
		if (!bSkip)
		{
			 if (g_Random(nRate + nLuck) >= g_Random(LuckyRate))   // ȡ���� ����+��װ�ı���  ħ�����Եı���
			 {
//////////////////////////����ĵȼ�Ӱ�����Եĵȼ�////////////////////////////////////////////////////////////////
				if (nMgLevel>0)
				{
                    nSLevel=nMgLevel;
				}
				else
				{
				  g_GameSetTing.GetInt5("ServerConfig","DuanZaoMagciLuckyLevel",nLuckItem);

					   if (nLuck>=nLuckItem[0] && nLuck <=nLuckItem[1])
						 nSLevel=GetRandomNumber(1,2);
					   else if (nLuck>nLuckItem[1] && nLuck <=nLuckItem[2])
						 nSLevel=GetRandomNumber(1,3);
					   else if  (nLuck>nLuckItem[2] && nLuck <=nLuckItem[3])
						 nSLevel=GetRandomNumber(1,4);
					   else if (nLuck>nLuckItem[3] && nLuck <=nLuckItem[4])
						 nSLevel=GetRandomNumber(1,5);
					   else if (nLuck >nLuckItem[4])
						   nSLevel=GetRandomNumber(1,10);
					   else
                           nSLevel=GetRandomNumber(0,1);		 
				}
				     pnMagicLevel[j] =nSLevel;  //ħ�����Ե����� ħ���������������  Ʒ��
			 }
			 else
			 {
				     pnMagicLevel[j] = 0; //��װ��
				     bSkip = TRUE;
			 }
		}
		else
		{
			if (nwMinItemLevel>0 &&  nwMaxItemLevel>0)
			{//��Ʒָ���ĵȼ�	 
				nLevel = GetRandomNumber(nwMinItemLevel,nwMaxItemLevel);  
			}
			pnMagicLevel[j] = 0;  //������
		}
	}

    nLuckItem[0]=0;nLuckItem[1]=0;nLuckItem[2]=0;nLuckItem[3]=0;nLuckItem[4]=0;

	int nIdx=0; 

	if (nLevel<=0)
        nLevel = 1;
//nStackNum,nEnChance,nPoint
	if (nQuality==0)
         nIdx = ItemSet.AddOther(nGenre,nSeries,nLevel,nLuck, nDetail,\
	        nParticular, pnMagicLevel, g_SubWorldSet.GetGameVersion(),\
			0,1,0,0,bYear,bMonth,bDay,bHour,bMin,0,NULL,NULL,NULL,0);
//��װ�Ĳ���
	else if(nQuality==1)
	     nIdx = ItemSet.AddGold(nDetail,pnMagicLevel,nSeries,0,0,\
	                                bYear,bMonth,bDay,bHour,bMin,\
						                  NULL,NULL,nLevel,nLuck);
	if (nIdx <= 0)
		return FALSE;

	if (nIsBang)
		Item[nIdx].SetBang(nIsBang);


	return nIdx;
}


int KNpc::GiveZhiDingItem(int nBelongPlayer,int nItemGenre,int nDetailType,\
           int nParticulType,int nItemMinLevel,int nItemMaxLevel,\
                       int nItemSeries,int IsBang,int IsHour,int IsQuality,\
                       int nStackNum,int nEnChance,int nPoint,int nIsWhere,\
         int* pnMagicLevel,int* pnRMagicLevel,int* pnJbLevel, int* pnBsLevel)
{

    if (nItemSeries==-1)
        nItemSeries=GetRandomNumber(0,4);

	//int LuckyRate;
    //  g_GameSetTing.GetInteger("ServerConfig","ZhiDingoLuckyRate",5000,&LuckyRate);

	int nGenre, nSeries, nLuck, nDetail, nParticular, nLevel=1,nSLevel=0,nQuality,nwMinItemLevel,nwMaxItemLevel,nIsBang,nHour,bYear,bMonth,bDay,bHour,bMin;  //Quality=1�ǻ�װ 2Ϊ�׽�3 Ϊ��װ ��
	nGenre         = nItemGenre;
	nDetail        = nDetailType;
	nParticular    = nParticulType;
	nSeries        = nItemSeries;//m_Series;
	nLuck          = Player[nBelongPlayer].m_nCurLucky*Npc[Player[nBelongPlayer].m_nIndex].IsLuKey;  //��ҵ�����ֵ
    //n_Wlevel       = Npc[Player[nBelongPlayer].m_nIndex].m_Level; //��ҵȼ�
	nQuality       = IsQuality;  //�Ƿ�ƽ�
    nwMinItemLevel = nItemMinLevel;  //��Ʒ���ֵĵȼ�
    nwMaxItemLevel = nItemMaxLevel;
    nIsBang        = IsBang;
    nHour          = IsHour;

	time_t rawtime;
	struct tm * timeinfo;	
	time (&rawtime);
	timeinfo = localtime (&rawtime);

	if (nHour>0)
	{
       bYear   =timeinfo->tm_year+1900;
       bMonth  =timeinfo->tm_mon+1;
       bDay    =timeinfo->tm_mday;
       bHour   =timeinfo->tm_hour+nHour;
	   bMin    =timeinfo->tm_min;
	}
	else
	{
        bYear   =0;
		bMonth  =0;
		bDay    =0;
		bHour   =0;
		bMin    =0;
	}

    int nMaxLevel,nMinLevel,mCount=0;

/////////////////////////////////////////////////////////////////////////////////////////////////
	if (nwMinItemLevel>0 &&  nwMaxItemLevel>0)
	{
		nLevel = GetRandomNumber(nwMinItemLevel,nwMaxItemLevel);  //��Ʒָ���ĵȼ�	 
	}
	else
	{ 
		nLevel = GetRandomNumber(1,7);
	} 
////////////////////////////////////////////////////////////////////

	int nIdx=0; 

	if (nLevel<=0)
        nLevel = 1;

	if (nQuality==0)
         nIdx = ItemSet.AddOther(nGenre,nSeries,nLevel,nLuck,nDetail,\
	         nParticular,pnMagicLevel,g_SubWorldSet.GetGameVersion(),\
			0,nStackNum,nEnChance,nPoint,bYear,bMonth,bDay,bHour,bMin,\
			           0,pnRMagicLevel,pnJbLevel,pnBsLevel,nIsWhere);
//��װ�Ĳ���
	else if(nQuality==1)
	     nIdx = ItemSet.AddGold(nDetail,pnMagicLevel,nSeries,0,nPoint,\
	                                     bYear,bMonth,bDay,bHour,bMin,\
						          pnRMagicLevel,pnBsLevel,nLevel,nLuck);
	if (nIdx <= 0)
		return FALSE;

	if (nIsBang)
		Item[nIdx].SetBang(nIsBang);


    int		nX, nY,nMap;
	POINT	ptLocal;
	KMapPos	Pos;
	        GetMpsPos(&nX,&nY,&nMap);  // ת���������
	ptLocal.x = nX;
	ptLocal.y = nY;
	SubWorld[m_SubWorldIndex].GetFreeObjPos(ptLocal);
	
	Pos.nSubWorld = m_SubWorldIndex;
	SubWorld[m_SubWorldIndex].Mps2Map(ptLocal.x, ptLocal.y, 
		&Pos.nRegion, &Pos.nMapX, &Pos.nMapY, 
		&Pos.nOffX, &Pos.nOffY);

	int nObj;
	//////����װ��������KObjItemInfo//////�ṹ���еĲ������Ա����
	KObjItemInfo sInfo;
	memset(&sInfo, 0, sizeof(sInfo));
	sInfo.m_nItemID = nIdx;
	sInfo.m_nItemWidth = Item[nIdx].GetWidth();
	sInfo.m_nItemHeight = Item[nIdx].GetHeight();
	sInfo.m_nMoneyNum = 0;
	t_sprintf(sInfo.m_szName, Item[nIdx].GetName()); //��Ʒ������
	sInfo.m_nColorID = 0;
	sInfo.m_nMovieFlag = 1;
	sInfo.m_nSoundFlag = 1;
    sInfo.m_sHaveAttack=0;      //�Ƿ�����Ϊ������Ч��
	//sInfo.m_AttackerDwid=0;   //�ϴι����ŵ�DWID

	nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), Pos, sInfo);
	if (nObj == -1)
	{
		ItemSet.Remove(nIdx);
	}
	else
	{
		if (nBelongPlayer > 0)
			Object[nObj].SetItemBelong(nBelongPlayer);
		else
			Object[nObj].SetItemBelong(-1);
	}


	return nIdx;
}

//NPC���� ��������
void KNpc::Revive()  //NPC����
{	
	//RestoreLiveData();
	RestoreNpcBaseInfo();  //���û�����Ϣ(�ͻ��˺� ������ ������)
	int nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[m_SubWorldIndex].Mps2Map(m_OriginX, m_OriginY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	m_RegionIndex = nRegion;
	m_MapX = nMapX;
	m_MapY = nMapY;
	m_MapZ = 0;
	m_OffX = nOffX;
	m_OffY = nOffY;

	if (m_RegionIndex < 0)
		return;

	//SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);

	SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].AddNpcRef(m_MapX,m_MapY);
	SubWorld[0].NpcChangeRegion(MAP_REGION, SubWorld[0].m_Region[nRegion].m_RegionID, m_Index);
	DoStand();
	m_ProcessAI = 1;
	m_ProcessState = 1;
	m_AiAddLifeTime = 0;
}

void KNpc::RestoreLiveData()
{

}
/////////////////////////////��Ѫ ���� ��תѪ///////////////////////////////////
/*void KNpc::RestoreDamage2Life(int nDamage)
{	
	m_CurrentLife = m_CurrentLife + (nDamage*m_CurrentLifeStolen/100);
	if(m_CurrentLife >= m_CurrentLifeMax)
		m_CurrentLife = m_CurrentLifeMax;
}

void KNpc::RestoreDamage2Mana(int nDamage)
{	
	m_CurrentMana = m_CurrentMana + (nDamage*m_CurrentManaStolen/100);
	if(m_CurrentMana >= m_CurrentManaMax)
		m_CurrentMana = m_CurrentManaMax;
}

void KNpc::RestoreDamage2Stamina(int nDamage)
{	
	m_CurrentStamina = m_CurrentStamina + (nDamage*m_CurrentStaminaStolen/100);
	if(m_CurrentStamina >= m_CurrentStaminaMax)
		m_CurrentStamina = m_CurrentStaminaMax;
}*/
////////////////////////////////////////////////////////////////////////////////
#endif

//-------------------------------------------------------------------------
//	���ܣ��趨ͷ��״̬
//-------------------------------------------------------------------------
void	KNpc::SetMenuState(int nState, char *lpszSentence, int nLength)
{
	this->m_DataRes.SetMenuState(nState, lpszSentence, nLength);
}

//-------------------------------------------------------------------------
//	���ܣ����ͷ��״̬
//-------------------------------------------------------------------------
int		KNpc::GetMenuState()
{
	return this->m_DataRes.GetMenuState();
}

//-------------------------------------------------------------------------
//	���ܣ�������Χ9��Region���Ƿ���ָ�� ID �� npc  ����NPC����
//-------------------------------------------------------------------------
DWORD	KNpc::SearchAroundID(DWORD dwID)
{
	int		nIdx, nRegionNo;
	nIdx = SubWorld[0].m_Region[m_RegionIndex].SearchNpc(dwID);
	if (nIdx)
		return nIdx;
	for (int i = 0; i < 8; ++i)
	{
		nRegionNo = SubWorld[0].m_Region[m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nIdx = SubWorld[0].m_Region[nRegionNo].SearchNpc(dwID);
		if (nIdx)
			return nIdx;
	}
	return 0;
}

//-------------------------------------------------------------------------
//	���ܣ��趨�����ֻ����һ�������spr�ļ�
//-------------------------------------------------------------------------
void	KNpc::SetSpecialSpr(char *lpszSprName)
{
	//m_DataRes.SetSpecialSpr(lpszSprName);
}
//����ѭ����̬SPR ����ͬʱ��ʾ��SPR
void	KNpc::SetFrameSpr(char *lpszSprName, int nX, int nY, int nHeight,int mInterval) // ve danh hieu ff
{
	//m_DataRes.SetFrameSpr(lpszSprName, nX, nY,nHeight,mInterval);
}

//-------------------------------------------------------------------------
//	���ܣ��趨˲����Ч
//-------------------------------------------------------------------------
void	KNpc::SetInstantSpr(int nNo)
{
	char	szName[FILE_NAME_LENGTH];
	szName[0] = 0;
	NpcSet.m_cInstantSpecial.GetSprName(nNo, szName, sizeof(szName));
	if (szName[0])
		this->SetSpecialSpr(szName);
}

int		KNpc::GetNormalNpcStandDir(int nFrame)
{
	return m_DataRes.GetNormalNpcStandDir(nFrame);
}
//����
void	KNpc::SetNpcState(int* pNpcState)
{
    ClearNpcState();  //�ͷ��ڴ�

	if (!pNpcState)
		return ;

	for (int i = 0; i < MAX_NPC_RECORDER_STATE;++i)
	{
		if (*(pNpcState + i) != 0)//(*pNpcState > 0)  //*(pNpcState + i) != 0	 *pNpcState > 0
		{
			KStateNode * pNewNode = new KStateNode;      //�����ӵ�״̬
			pNewNode->m_StateGraphics = *(pNpcState + i);//*pNpcState;
			pNewNode->m_IsClientState = 1;               //���˵�״̬ ����Ϊ�ͻ���
			m_StateSkillList.AddTail(pNewNode);          //��ĩβ��ӽڵ����ͻ��˵�����
			//*pNpcState++;	                             //��ֵ������
		}
	}

   //m_DataRes.SetState(&m_StateSkillList, &g_NpcResList);  //�⻷����״̬	
}

//������пͻ���״̬
void KNpc::ClearNpcState()
{
	KStateNode * pNode = (KStateNode*)m_StateSkillList.GetHead();
	KStateNode * pTempNode = NULL;
	
	while(pNode)
	{
		int nIdx = pNode->m_StateGraphics;
		if (g_GameWorld)//ɾ��ȫ��״̬����
			g_GameWorld->removespriteByIdx(m_Index,nIdx);

		pTempNode = pNode;
		pNode = (KStateNode*) pNode->GetNext();
		pTempNode->Remove();
		delete pTempNode;
        pTempNode=NULL;
	}
	return;
}

//���ûָ�NPC�Ļ�����Ϣ  ���� �� ������Ϸʱ����	������� ����
void	KNpc::RestoreNpcBaseInfo()
{

   int nmPlayerIdx=0;

   if  (m_Kind==kind_player)
   {//��������
	nmPlayerIdx		= CLIENT_PLAYER_INDEX;
   }
	m_CurrentCamp = m_Camp;
	m_ActiveSkillID = 0;
	m_ActiveSkListIndex=0;
	m_IsMoreAura=false;

	m_ActiveAuraID = 0;
	m_ActiveAuraIndex= 0;
	m_ActiveXinYunXingID = 0;

	m_nPeopleIdx = 0;
	m_nLastDamageIdx = 0;
	m_nLastPoisonDamageIdx = 0;
	m_nLastBurnDamageIdx = 0;
	m_nObjectIdx = 0;

	m_TempFireResist = 0;	                       // Npc�ĵ�ǰ����
	m_TempColdResist = 0;	                       // Npc�ĵ�ǰ������
	m_TempPoisonResist = 0;	                       // Npc�ĵ�ǰ������
	m_TempLightResist = 0;	                       // Npc�ĵ�ǰ�翹��
	m_TempPhysicsResist = 0;	                   // Npc�ĵ�ǰ������

	m_CurrentFireResist		= m_FireResist;		   //ԭʼ�����
	m_CurrentColdResist		= m_ColdResist;
	m_CurrentPoisonResist	= m_PoisonResist;
	m_CurrentLightResist	= m_LightResist;
	m_CurrentPhysicsResist	= m_PhysicsResist;

  if (m_Kind==kind_player)
  {//��������
	int nCurbei=0;

	/*int nFomoidx=GetCurFuMoIdx();

	if 	(nFomoidx>-1)
	{
		if (GetCurFoMoSkllLevel(nFomoidx)>=5) //��ȡ�񽫵�ǰ�ĵȼ�
		{
			nCurbei=GetCurFoMoSkllLevel(nFomoidx)/5;
		}
	}
	*/
	m_CurrentLifeMax		 = m_LifeMax*(100+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*Player[nmPlayerIdx].m_cReBorn.GetReBornLifeMaxVal()+nCurbei*50)/100;
	//TakeTrader(m_LifeMax*(100+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*50+nCurbei*50),100);
	//m_LifeMax*(100+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*50+nCurbei*50)/100;
    m_CurrentManaMax		 = m_ManaMax*(100+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*Player[nmPlayerIdx].m_cReBorn.GetReBornLifeMaxVal()+nCurbei*50)/100;
	//TakeTrader(m_ManaMax*(100+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*50+nCurbei*50),100);
	//m_ManaMax*(100+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*50+nCurbei*50)/100;//�ű��������������ֵ��ֵ
	m_CurrentFireResistMax	 = m_FireResistMax+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*Player[nmPlayerIdx].m_cReBorn.GetReBornFanYuMaxVal(); //200
	m_CurrentColdResistMax	 = m_ColdResistMax+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*Player[nmPlayerIdx].m_cReBorn.GetReBornFanYuMaxVal();
	m_CurrentPoisonResistMax = m_PoisonResistMax+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*Player[nmPlayerIdx].m_cReBorn.GetReBornFanYuMaxVal();
	m_CurrentLightResistMax	 = m_LightResistMax+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*Player[nmPlayerIdx].m_cReBorn.GetReBornFanYuMaxVal();
	m_CurrentPhysicsResistMax= m_PhysicsResistMax+Player[nmPlayerIdx].m_cReBorn.GetReBornNum()*Player[nmPlayerIdx].m_cReBorn.GetReBornFanYuMaxVal();
  }
  else
  {
	  m_CurrentLifeMax		     = m_LifeMax;
	  m_CurrentManaMax		     = m_ManaMax;//�ű��������������ֵ��ֵ
	  m_CurrentFireResistMax	 = m_FireResistMax; //200
	  m_CurrentColdResistMax	 = m_ColdResistMax;
	  m_CurrentPoisonResistMax   = m_PoisonResistMax;
	  m_CurrentLightResistMax	 = m_LightResistMax;
	  m_CurrentPhysicsResistMax  = m_PhysicsResistMax;
  }
    m_CurrentLife			= m_LifeMax;    //�ű��������������ֵ��ֵ
	m_CurrentLifeReplenish	= m_LifeReplenish;
	m_CurrentNuQi			= 0;    //����Ϊ��
	m_CurrentLifeDamage     = 0;
	m_CurPoisonDamage       = 0;
	m_CurFireDamage         = 0;
	m_CurrentNuQiMax		= m_NuqiMax;
	m_CurrentNuQiReplenish	= m_NuqiReplenish;

	m_CurrentMana			= m_ManaMax;

	m_CurrentManaReplenish	= m_ManaReplenish;
	m_CurrentStamina		= m_StaminaMax;
	m_CurrentStaminaMax		= m_StaminaMax;
	m_CurrentStaminaGain	= m_StaminaGain;
	m_CurrentStaminaLoss	= m_StaminaLoss;
	
//	memset(&m_CurrentJinMai, 0, sizeof(m_CurrentJinMai));
//	memset(&m_WaiPhysicsDamage, 0, sizeof(m_WaiPhysicsDamage));
//	memset(&m_CurrentFireDamage, 0, sizeof(m_CurrentFireDamage));
//	memset(&m_CurrentColdDamage, 0, sizeof(m_CurrentColdDamage));
//	memset(&m_CurrentLightDamage, 0, sizeof(m_CurrentLightDamage));
//	memset(&m_CurrentPoisonDamage, 0, sizeof(m_CurrentPoisonDamage));

	memset(&m_CurrentPhysicsMagicDamageV, 0, sizeof(m_CurrentPhysicsMagicDamageV));
	memset(&m_CurrentPhysicsMagicDamageP, 0, sizeof(m_CurrentPhysicsMagicDamageP));	
	memset(&m_CurrentMagicFireDamage, 0, sizeof(m_CurrentMagicFireDamage));
	memset(&m_CurrentMagicColdDamage, 0, sizeof(m_CurrentMagicColdDamage));
	memset(&m_CurrentMagicLightDamage, 0, sizeof(m_CurrentMagicLightDamage));
	memset(&m_CurrentMagicPoisonDamage, 0, sizeof(m_CurrentMagicPoisonDamage));

	m_CurrentAttackRating	= m_AttackRating;
	m_CurrentDefend			= m_Defend;

	m_CurrentWalkSpeed		= m_WalkSpeed;
	m_CurrentRunSpeed		= m_RunSpeed;
	m_CurrentAttackSpeed	= m_AttackSpeed;
	m_CurrentCastSpeed		= m_CastSpeed;
	m_CurrentVisionRadius	= m_VisionRadius;
	m_CurrentActiveRadius	= m_ActiveRadius;
	m_CurrentHitRecover		= m_HitRecover;
	m_CurrentTreasure		= m_Treasure;   //����װ��������

	m_CurrentDamage2Mana	= 0;
	//m_CurrentManaPerEnemy	= 0;
	m_CurrentLifeStolen		= 0;
	m_CurrentManaStolen		= 0;
	m_CurrentStaminaStolen	= 0;
	m_CurrentKnockBack		= 0;	
	m_CurrentDeadlyStrike	= 0;
//	m_CurrentBlindEnemy		= 0;
//	m_CurrentPiercePercent	= 0;
	m_CurrentFreezeTimeReducePercent	= 0;
	m_CurrentPoisonTimeReducePercent	= 0;
	m_EnemyPoisonTimeReducePercent      = 0;
	m_CurrentStunTimeReducePercent		= 0;
	m_EnemyStunTimeReducePercent        = 0;
	m_CurrentFireEnhance	= 0;
	m_CurrentColdEnhance	= 0;
	m_CurrentPoisonEnhance	= 0;
	m_CurrentLightEnhance	= 0;
	m_CurrentRangeEnhance	= 0;
	m_CurrentHandEnhance	= 0;
	m_CurrentSerisesEnhance = 0;
	m_CurrentdanggeRate		= 0;  //����
	m_CurrentzhongjiRate	= 0;  //
    m_CurrentcjdanggeRate	= 0;                 //��⵵��
    m_CurrentcjzhongjiRate	= 0;                //����ػ�
    m_Currentsorbdamage	 = 0;                   //�����˺�
	m_Currentsorbdamage_v=0;
	m_Currenadddamagev   = 0;
	m_Currenadddamagep   = 0;
    m_Currentpoisonres	= 0;                    //=���ԶԷ�����:#d1-%
    m_Currentfireres	= 0;                      //=���ԶԷ����:#d1-%
    m_Currentlightingres	= 0;                  //=���ԶԷ��׷�:#d1-%
    m_Currentphysicsres	= 0;                   //=���ԶԷ��շ�:#d1-%
    m_Currentcoldres	= 0;                   //=���ԶԷ�����:#d1-%
	m_Currentallres     =0;                    //=���ԶԷ�ȫ��:#d1-%      
    m_Currentnopkvalue	= 0;                    //=������PKֵ����:#d1+%
    m_Currentbossdamage	= 0;                   //=�Իƽ�boss�����˺�<color=orange>#d1+%<color>
    m_Currentelementsenhance	= 0;              //=����ǿ��ֵ��#d1-�㡣ǿ����������еĿ���Ч��
    m_Currentelementsresist	= 0;               //=��������ֵ��#d1-�㡣������������еĿ���Ч��
	m_Currentskillenhance=0;
//	ZeroMemory(m_CurrentSkillEnhance,sizeof(m_CurrentSkillEnhance));
	/*_EnhanceInfo::iterator it;
	for( it = nEnhanceInfo.begin(); it != nEnhanceInfo.end(); ++it)
	{
		it->second.nSkillIdx = 0;
		it->second.nEnhance = 0;
	}*/
	nEnhanceInfo.clear();
	m_CurrentFullManaskillenhance=0;
	m_CurrentUpExp=0;
	m_CurrentautoReviverate=0;                 //�������
    m_CurrentCreatnpcv=0;
    m_CurrentAllJiHuo=0;                       //�Ƿ�ȫ����
	m_CurrentCreatStatus=0;
    m_CurrentAddPhysicsDamageP= 0;     // ��ǰ�������հٷֱ�
    m_CurrentAddFireDamagev= 0;        // ��ǰ��������
    m_CurrentAddColdDamagev= 0;        // ��ǰ��������� 
    m_CurrentAddLighDamagev= 0;        // ��ǰ�������׵�
    m_CurrentAddPoisonDamagev= 0;      // ��ǰ�����ⶾ��
	
    m_CurrentAddmagicphysicsDamage= 0; // ��ǰ�������յ�
    m_CurrentAddmagicphysicsDamageP= 0;// ��ǰ�������հٷֱ�
    m_CurrentAddmagicColdDamagicv= 0;  // ��ǰ�����ڱ���
    m_CurrentAddmagicFireDamagicv= 0;  // ��ǰ�����ڻ��  
	m_CurrentAddmagicLightDamagicv= 0; // ��ǰ�������׵�
    m_CurrentAddmagicPoisonDamagicv= 0;// ��ǰ�����ڶ���
	m_Currentbaopoisondmax_p=0;
	m_CurrentPoisondamagereturnV=0;
	m_CurrentPoisondamagereturnP=0;
	m_CurrentReturnskillp=0;
	m_CurrentIgnoreskillp=0;
	m_CurrentReturnresp=0;
    m_CurrentAttackRatingEnhancep=0;
	m_CurrentAttackRatingEnhancev=0;
	m_CurrentIgnorenAttacRating  =0;

	m_Me2metaldamage_p=0;              //=�Խ�ϵ�˺����ӣ�#d1+%
	m_Metal2medamage_p=0;              //=�������Խ�ϵ���˺���#d1-%
	m_Me2wooddamage_p=0;              //=��ľϵ�˺����ӣ�#d1+%
	m_Wood2medamage_p=0;              //=��������ľϵ���˺���#d1-%
	m_Me2waterdamage_p=0;              //=��ˮϵ�˺����ӣ�#d1+%
	m_Water2medamage_p=0;              //=��������ˮϵ���˺���#d1-%
	m_Me2firedamage_p=0;              //=�Ի�ϵ�˺����ӣ�#d1+%
	m_Fire2medamage_p=0;              //=�������Ի�ϵ���˺���#d1-%
	m_Me2earthdamage_p=0;              //=����ϵ�˺����ӣ�#d1+%
	m_Earth2medamage_p=0;              //=����������ϵ���˺���#d1-%
	m_CurrentStunRank_p=0;
	m_Staticmagicshield_p=0;
	ZeroMemory(m_CurrentMeleeEnhance, sizeof(m_CurrentMeleeEnhance));
	ClearStateSkillEffect();
	ClearNormalState();

}
//���NPC�߿�
void KNpc::DrawBorder()
{
	if (m_Index <= 0)
		return;
	//m_DataRes.DrawBorder();
}

int KNpc::DrawMenuState(int n)
{
	if (m_Index <= 0) //��״̬�Ż滭)
		return n;

	if (GetMenuState())
	{  
		isRemoveMenu = false;
	   return  m_DataRes.DrawMenuState(n,m_Index);
	}
	else
	{//û��״̬
		if (g_GameWorld && !isRemoveMenu)
		{
			isRemoveMenu = true; //�����Ѿ�ɾ������
			g_GameWorld->removeMenuByIdx(m_Index,0);
		}

		return n;
	}
}
//���� ðѪ����  ����Ѫ��
void KNpc::DrawBlood()
{
	if (m_Kind != kind_normal) 
		return;

	int nFontSize = 12;
	int nHeightOff = GetNpcPate();
	//if (NpcSet.CheckShowLife())
	{
		nHeightOff = PaintLife(nHeightOff, true);
		nHeightOff += SHOW_SPACE_HEIGHT;
	}
	//if (NpcSet.CheckShowName())
    nHeightOff = PaintInfo(nHeightOff,true,nFontSize,0XFF000000);  //ѡ ���� NPC�к� ����ʾ

}

 //��ʼ�������
/*#ifdef _SERVER
void KNpc::TobeExchangeServer(DWORD dwMapID, int nX, int nY)
{
	if (!IsPlayer())
	{
		return;
	}
	m_OldFightMode = m_FightMode;
	m_bExchangeServer = TRUE;  //���ڿ��
	if (m_nPlayerIdx > 0 && m_nPlayerIdx <= MAX_PLAYER)
	{
		Player[m_nPlayerIdx].TobeExchangeServer(dwMapID, nX, nY);
	}
}
#endif */
/*
int KNpc::GetCurFuMoIdx()
{//��ǰʹ���񽫱��
	for (int i=0;i<MAX_FUMO_COUNT;++i)
	{
	    if (m_nFuMoNum[i].nNpcSetings>1)
			return i;
	} 
	return -1;
} 

int KNpc::GetCurFuMoNpcNo(int idx)
{//��ǰʹ���񽫱��
	if (idx<=-1 || idx>MAX_FUMO_COUNT)
		return 0;

	return	m_nFuMoNum[idx].nNpcSetings;
}


int KNpc::GetCurFoMoSkllLevel(int idx)
{//��ǰ���񽫼��ܵȼ�
    if (idx<=-1 || idx>MAX_FUMO_COUNT)
		return 0;

	return m_nFuMoNum[idx].nSkillLevel;
}
*/

//��ȡ����������С�����˺�
//#ifndef _SERVER
void KNpc::GetTongBanDamage(int* nMin, int* nMax)
{
	int nSkillId;
	*nMin = 0;
	*nMax = 0;
	
	if (NULL == nMin || NULL == nMax)
		return;
	//���Ҽ���ѡ��

    nSkillId = m_ActiveSkillID;
	int nListidx = m_SkillList.FindSame(nSkillId);
	int nLevel = m_SkillList.GetCurrentLevelByIdx(nListidx);
	int nEnChance = m_SkillList.GetEnChance(nListidx);

	if (nSkillId <= 0 || nLevel <= 0 || nLevel >= MAX_SKILLLEVEL)
		return;
	
	KMagicAttrib*	pMagicData = NULL;
	
	KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillId, nLevel);
	if (!pOrdinSkill) 
        return ;
 /*
	switch(pOrdinSkill->GetSkillStyle())
	{
	case SKILL_SS_Missles:		        	//	�ӵ���		���������ڷ����ӵ���
        break;
	case SKILL_SS_Melee:                    //
	    break;
	case SKILL_SS_InitiativeNpcState:	    //	������		���������ڸı䵱ǰNpc������״̬
	    break;
	case SKILL_SS_PassivityNpcState:		//	������		���������ڸı�Npc�ı���״̬
		{

		}
		break;
	default:
		return;
	} */
	
	BOOL	bIsPhysical = pOrdinSkill->IsPhysical();  //������
	int 	bIsMagic = pOrdinSkill->IsMagic();        //1�ڹ�ϵ ��0�⹥ϵ

	pMagicData = pOrdinSkill->GetDamageAttribs();     //������������
	
	if (!pMagicData)
		return;
	//�����˺�+���������˺�+�յ�=�������˺�
	int nMinNpcDamage = m_PhysicsDamage.nValue[0]+m_CurrentPhysicsMagicDamageV.nValue[0];
	int nMaxNpcDamage = m_PhysicsDamage.nValue[2]+m_CurrentPhysicsMagicDamageV.nValue[2];
 	
	int nEnhancew=0;
/*
	if (equip_meleeweapon == m_ItemList.GetWeaponType())
	{
		nEnhancew = m_CurrentMeleeEnhance[m_ItemList.GetWeaponParticular()];
	}
	else if (equip_rangeweapon == m_ItemList.GetWeaponType())
	{
		nEnhancew = m_CurrentRangeEnhance;
	}
	else 
	{
		nEnhancew = m_CurrentHandEnhance;
	} */
//�����˺�
	*nMin += nMinNpcDamage +(nMinNpcDamage*nEnhancew)/100;
	*nMax += nMaxNpcDamage +(nMinNpcDamage*nEnhancew)/100;
	
	//������
	pMagicData++;//1 //������
	pMagicData++; //2 �յ��˺�+�հٷֱ��˺�
	if (magic_physicsenhance_p == pMagicData->nAttribType)
	{
		int Vmin=0,Vmax=0;

		    Vmin = (*nMin)*pMagicData->nValue[0]/100;
            Vmax = (*nMax)*pMagicData->nValue[0]/100;

		*nMin += Vmin;
		*nMax += Vmax;	

	//*nMin += nMinNpcDamage * nEnhancew / 100;
	//*nMax += nMaxNpcDamage * nEnhancew / 100;

	if (bIsMagic)
	{//�ű���������+����״̬(װ��+���ܵ�״̬)
		*nMin = (*nMin+m_CurrentAddmagicphysicsDamage)*(100+m_CurrentAddmagicphysicsDamageP)/100;
		*nMax = (*nMax+m_CurrentAddmagicphysicsDamage)*(100+m_CurrentAddmagicphysicsDamageP)/100;
	}
	else
	{//�ű���������+����״̬(װ��+���ܵ�״̬)
		*nMin = (*nMin+m_CurrentAddPhysicsDamage)*(100+m_CurrentAddPhysicsDamageP)/100;
		*nMax = (*nMax+m_CurrentAddPhysicsDamage)*(100+m_CurrentAddPhysicsDamageP)/100;
	} 
	}
	pMagicData++; // 3 ���˺�
	if (magic_colddamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2];//*(100+ m_CurrentColdEnhance)/100;	

	if (bIsMagic)  //�ڱ�
	{
		*nMin += m_CurrentMagicColdDamage.nValue[0]+m_CurrentAddmagicColdDamagicv;
		*nMax += m_CurrentMagicColdDamage.nValue[2]+m_CurrentAddmagicColdDamagicv;
	}
	else
	{ 
		*nMin += m_CurrentMagicColdDamage.nValue[0]+m_CurrentAddColdDamagev;
		*nMax += m_CurrentMagicColdDamage.nValue[2]+m_CurrentAddColdDamagev;
	} 
	}
	pMagicData++; //4  ���˺�
	if (magic_firedamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2]*(100 + m_CurrentFireEnhance)/ 100;
	if (bIsMagic)
	{
		*nMin += m_CurrentMagicFireDamage.nValue[0]+m_CurrentAddmagicFireDamagicv;
		*nMax += m_CurrentMagicFireDamage.nValue[2]+m_CurrentAddmagicFireDamagicv;
	}
	else
	{ 
		*nMin += m_CurrentMagicFireDamage.nValue[0]+m_CurrentAddFireDamagev;
		*nMax += m_CurrentMagicFireDamage.nValue[2]+m_CurrentAddFireDamagev;
	} 
	}
	pMagicData++;// 5 ���˺�
	if (magic_lightingdamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2]; 

	if (bIsMagic)
	{
		*nMin += m_CurrentMagicLightDamage.nValue[0]+m_CurrentAddmagicLightDamagicv;
		*nMax += m_CurrentMagicLightDamage.nValue[2]+m_CurrentAddmagicLightDamagicv;
	}
	else
	{ 
		*nMin += m_CurrentMagicLightDamage.nValue[0]+m_CurrentAddLighDamagev;
		*nMax += m_CurrentMagicLightDamage.nValue[2]+m_CurrentAddLighDamagev;
	}
	}
	pMagicData++; // 6 ���˺�
	if (magic_poisondamage_v == pMagicData->nAttribType)
	{
			*nMin += pMagicData->nValue[0];//* (100 +m_CurrentPoisonEnhance) / 100;//���ܻ�������
			*nMax += pMagicData->nValue[0]; 

	  if (bIsMagic)
	  { 
		*nMin += m_CurrentMagicPoisonDamage.nValue[0]+m_CurrentAddmagicPoisonDamagicv;
		*nMax += m_CurrentMagicPoisonDamage.nValue[0]+m_CurrentAddmagicPoisonDamagicv;
	  } 
	  else
	  {  
		int nPoisonDamage = m_CurrentMagicPoisonDamage.nValue[0]+m_CurrentAddPoisonDamagev;
		*nMin += nPoisonDamage;
		*nMax += nPoisonDamage;
	  } 
	}
	pMagicData++;// 7 �����˺������ӷ�����

	if (magic_magicdamage_v == pMagicData->nAttribType)
	{ 
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2];
	}
	pMagicData++;//8
	pMagicData++;//9
	pMagicData++;//10
	pMagicData++;//11
	pMagicData++;//12
	pMagicData++;//13
	pMagicData++;//14
	pMagicData++;//15
	if (magic_physicsdamage_v == pMagicData->nAttribType) //�յ��˺� ��
	{
		//*nMin += pMagicData->nValue[0];
		//*nMax += pMagicData->nValue[2];
      if (bIsMagic)
	  {//�ű���������+����״̬(װ��+���ܵ�״̬)
		*nMin += (pMagicData->nValue[0]+m_CurrentAddmagicphysicsDamage)*(100+m_CurrentAddmagicphysicsDamageP)/100;
		*nMax += (pMagicData->nValue[2]+m_CurrentAddmagicphysicsDamage)*(100+m_CurrentAddmagicphysicsDamageP)/100;
	  }
	  else
	  {//�ű���������+����״̬(װ��+���ܵ�״̬)
		*nMin += (pMagicData->nValue[0]+m_CurrentAddPhysicsDamage)*(100+m_CurrentAddPhysicsDamageP)/100;
		*nMax += (pMagicData->nValue[2]+m_CurrentAddPhysicsDamage)*(100+m_CurrentAddPhysicsDamageP)/100;
	  } 
    }
	pMagicData++;//16
	pMagicData++;//17
	pMagicData++;//18
	
/*	int nFomoidx=GetCurFuMoIdx(),nCurbei=0;
	
	if 	(nFomoidx>-1)
	{
		if (GetCurFoMoSkllLevel(nFomoidx)>=5) //��ȡ�񽫵�ǰ�ĵȼ�
		{
			nCurbei=GetCurFoMoSkllLevel(nFomoidx)/5;
		}
	}
	*/
	*nMin += *nMin * nEnChance/100;
	*nMax += *nMax * nEnChance/100; 

//	*nMin = *nMin * (nCurbei*5+100)/100;
//  *nMax = *nMax * (nCurbei*5+100)/100;
}





BOOL KNpc::IsPlayer()
{
	return m_Index == Player[CLIENT_PLAYER_INDEX].m_nIndex; //�ͻ��� NPC����
}

// ���NPC���ϵķǱ�����ļ���״̬
void KNpc::ClearStateSkillEffect()
{
	KStateNode* pNode;
	          pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
		KStateNode* pTempNode = pNode;
		pNode = (KStateNode *)pNode->GetNext();

		if (pTempNode->m_LeftTime == -1)	// ��ʱ�����Ƽ���
			continue;

		if (pTempNode->m_LeftTime > 0)
		//if (pTempNode->m_LeftTime == 0 && pTempNode->m_IsClientState==0)    //�ڼ���״̬ʱ��
		{

			if  (nExpSkillIdx!=0 && pTempNode->m_SkillID == nExpSkillIdx)
			{//����˫������ʱ��
                 continue;
			}

		    if  (nLuckySkillIdx!=0 && pTempNode->m_SkillID == nLuckySkillIdx)
			{//����˫������ʱ��
                continue;
			}

			if  (nKangkillIdx!=0 && pTempNode->m_SkillID == nKangkillIdx)
			{//����ȫ��ʱ��
                continue;
			}
			if  (nYaoPinSkillIdx!=0 && pTempNode->m_SkillID == nYaoPinSkillIdx)
			{//���ݳ�ЧҩƷʱ��
                continue;
			}
	
			for (int i = 0; i < MAX_SKILL_STATE; ++i)
			{
				if (pTempNode->m_State[i].nAttribType)
				{
					ModifyAttrib(m_Index, &pTempNode->m_State[i]);
				}
			}
			int nIdx = pTempNode->m_StateGraphics;
			//_ASSERT(pTempNode != NULL);
			if (pTempNode != NULL)
			{
		       pTempNode->Remove();
			   delete pTempNode;
			   pTempNode = NULL;
			   if (g_GameWorld)//ɾ��״̬����
				   g_GameWorld->removespriteByIdx(m_Index,nIdx);
			}
			//pTempNode = NULL;
			continue;
		}
	}
}

void KNpc::ClearNormalState() //�ͷ��ڴ����� ����ڴ�����
{

	/*_KStateNote::iterator it;
	for( it = nKStateNote.begin(); it != nKStateNote.end(); ++it)
	{
		it->second.nMagicAttrib = 0;
		it->second.nTime = 0;
		it->second.nValue[0] = 0;
		it->second.nValue[1] = 0;
	 }*/
	//nKStateNote.clear();

	ZeroMemory(&m_PhysicsArmor, sizeof(m_PhysicsArmor));
	ZeroMemory(&m_ColdArmor, sizeof(m_ColdArmor));
	ZeroMemory(&m_FireArmor, sizeof(m_FireArmor));
	ZeroMemory(&m_PoisonArmor, sizeof(m_PoisonArmor));
	ZeroMemory(&m_LightArmor, sizeof(m_LightArmor));
	ZeroMemory(&m_ManaShield, sizeof(m_ManaShield));
	ZeroMemory(&m_PoisonState, sizeof(m_PoisonState));
	ZeroMemory(&m_FreezeState, sizeof(m_FreezeState));
	ZeroMemory(&m_BurnState, sizeof(m_BurnState));
	ZeroMemory(&m_ConfuseState, sizeof(m_ConfuseState));
	ZeroMemory(&m_StunState, sizeof(m_StunState));
	ZeroMemory(&m_LifeState, sizeof(m_LifeState));
	ZeroMemory(&m_ManaState, sizeof(m_ManaState));
	ZeroMemory(&m_DrunkState, sizeof(m_DrunkState));
	ZeroMemory(&m_Hide,sizeof(m_Hide));
	ZeroMemory(&m_LoseMana,sizeof(m_LoseMana));
	ZeroMemory(&m_ZhuaState,sizeof(m_ZhuaState));
	ZeroMemory(&m_Returnskill,sizeof(m_Returnskill));
	ZeroMemory(&m_ExpState, sizeof(m_ExpState));
	ZeroMemory(&m_DoScriptState, sizeof(m_DoScriptState));
    ZeroMemory(&m_randmove, sizeof(m_randmove));
	ZeroMemory(&m_Deathkill, sizeof(m_Deathkill));
	ZeroMemory(&m_Rescueskill, sizeof(m_Rescueskill));
	ZeroMemory(&m_Replyskill, sizeof(m_Replyskill));
}
//��������ִ��
void KNpc::CheckTrap()
{
	if (m_Kind != kind_player)
		return;
	
	if (m_Index <= 0)
		return;

	if (m_SubWorldIndex < 0 || m_RegionIndex < 0)
		return;

	DWORD	dwTrap = SubWorld[m_SubWorldIndex].m_Region[m_RegionIndex].GetTrap(m_MapX, m_MapY);
/*
#ifdef _SERVER
	char msg[64];
	     t_sprintf(msg,"��ǰ����:R:%d,X:%d,Y:%d,Tr:%d",m_RegionIndex,m_MapX,m_MapY,dwTrap);
    Player[m_nPlayerIdx].m_ItemList.msgshow(msg);
#endif
*/
	if (m_TrapScriptID == dwTrap)
	{
		return;
	}
	else
	{
		m_TrapScriptID = dwTrap;
	}

	if (!m_TrapScriptID)
	{
		return;
	}

	Player[m_nPlayerIdx].ExecuteScriptD(m_TrapScriptID, "main",0,"",0,0,0,0,0,FALSE);
}

void KNpc::SetFightMode(BOOL bFightMode)
{
	m_FightMode = bFightMode;

}

void KNpc::SetBayBan(BOOL bBayBan)
{
	m_BayBan = bBayBan;
}

void KNpc::setNpcDir(int nX2,int nY2)
{
	if (!m_Index)
		return;
	int nX1, nY1,nMap1;
	GetMpsPos(&nX1, &nY1,&nMap1);
	m_Dir = g_GetDirIndex(nX1, nY1, nX2, nY2); //��ȡ����
}

void KNpc::TurnTo(int nIdx)
{
	if (!Npc[nIdx].m_Index || !m_Index)
		return;

	int nX1, nY1,nMap1, nX2, nY2,nMap2;

	GetMpsPos(&nX1, &nY1,&nMap1);
	Npc[nIdx].GetMpsPos(&nX2, &nY2,&nMap2);

	m_Dir = g_GetDirIndex(nX1, nY1, nX2, nY2); //��ȡ����
}
//״̬�ļ���
void KNpc::ReCalcStateEffect()
{
	KStateNode* pNode;
	pNode = (KStateNode *)m_StateSkillList.GetHead();
	while(pNode)
	{
		if (pNode->m_LeftTime != 0)	// ��������(-1)������(>0)
		{
			int i;
			for (i = 0; i < MAX_SKILL_STATE; ++i)
			{
				if (pNode->m_State[i].nAttribType)
				{
					KMagicAttrib	nMagicAttrib;
					nMagicAttrib.nAttribType = pNode->m_State[i].nAttribType;
					nMagicAttrib.nValue[0]   = -pNode->m_State[i].nValue[0];
					nMagicAttrib.nValue[1]   = -pNode->m_State[i].nValue[1];
					nMagicAttrib.nValue[2]   = -pNode->m_State[i].nValue[2];
					//�ı�NPC����
					ModifyAttrib(m_Index, &nMagicAttrib);
				}
			}
		}
		pNode = (KStateNode *)pNode->GetNext();
	}
}

extern KTabFile g_ClientWeaponSkillTabFile;

int		KNpc::GetCurActiveWeaponSkill()
{
	int nSkillId = 0;
	if (IsPlayer())
	{//���
		
		int nDetailType = Player[m_nPlayerIdx].m_ItemList.GetWeaponType();
		int nParticularType = Player[m_nPlayerIdx].m_ItemList.GetWeaponParticular();
		
		//��������
		if (nDetailType == 0)
		{
			nSkillId = g_nMeleeWeaponSkill[nParticularType];
		}//Զ������
		else if (nDetailType == 1)
		{
			nSkillId = g_nRangeWeaponSkill[nParticularType];
		}//����
		else if (nDetailType == -1)
		{
			nSkillId = g_nHandSkill;
		}
	}
	else
	{//����
		if (m_Kind == kind_player)
		{
			g_ClientWeaponSkillTabFile.GetInteger(m_WeaponType + 1, "SkillId", 0, &nSkillId);
		}
		else						//Real Npc
		{
			return 0;//
		}
	}
	return nSkillId;
}
//�ϻ�̣�˲��
void	KNpc::HurtAutoMove()
{
	if (this->m_Index != Player[CLIENT_PLAYER_INDEX].m_nIndex)
		return;
	if (this->m_Doing != do_hurt)
		return;
	if (m_sSyncPos.m_nDoing != do_hurt && m_sSyncPos.m_nDoing != do_stand)
		return;

	int	nFrames, nRegionIdx;

	nFrames = m_Frames.nTotalFrame - m_Frames.nCurrentFrame;
	if (nFrames <= 1)
	{
		if ((DWORD)SubWorld[0].m_Region[m_RegionIndex].m_RegionID == m_sSyncPos.m_dwRegionID)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY);
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
			//SubWorld[0].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].m_Region[m_RegionIndex].AddNpcRef(m_MapX, m_MapY);
		}
		else
		{
			nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
			if (nRegionIdx < 0)
				return;
			SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = m_sSyncPos.m_nMapX;
			m_MapY = m_sSyncPos.m_nMapY;
			m_OffX = m_sSyncPos.m_nOffX;
			m_OffY = m_sSyncPos.m_nOffY;
			memset(&m_sSyncPos, 0, sizeof(m_sSyncPos));
		}
	}
	else
	{
		nRegionIdx = SubWorld[0].FindRegion(m_sSyncPos.m_dwRegionID);
		if (nRegionIdx < 0)
			return;
		int		nNpcX, nNpcY, nSyncX, nSyncY;
		int		nNewX, nNewY, nMapX, nMapY, nOffX, nOffY;
		SubWorld[0].NewMap2Mps(m_RegionIndex, 
			m_MapX, m_MapY,
			m_OffX, m_OffY,
			&nNpcX, &nNpcY);
		SubWorld[0].NewMap2Mps(nRegionIdx, 
			m_sSyncPos.m_nMapX, m_sSyncPos.m_nMapY,
			m_sSyncPos.m_nOffX, m_sSyncPos.m_nOffY,
			&nSyncX, &nSyncY);
		nNewX = nNpcX + (nSyncX - nNpcX) / nFrames;
		nNewY = nNpcY + (nSyncY - nNpcY) / nFrames;
		SubWorld[0].Mps2Map(nNewX, nNewY, &nRegionIdx, &nMapX, &nMapY, &nOffX, &nOffY);
		//_ASSERT(nRegionIdx >= 0);
		if (nRegionIdx < 0)
			return;
		if (nRegionIdx != m_RegionIndex)
		{
			SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY);
			SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegionIdx].m_RegionID, m_Index);
			m_RegionIndex = nRegionIdx;
			m_dwRegionID = m_sSyncPos.m_dwRegionID;
			m_MapX = nMapX;
			m_MapY = nMapY;
			m_OffX = nOffX;
			m_OffY = nOffY;
		}
		else
		{
			SubWorld[0].m_Region[m_RegionIndex].DecNpcRef(m_MapX, m_MapY);
			m_MapX = nMapX;
			m_MapY = nMapY;
			m_OffX = nOffX;
			m_OffY = nOffY;
			//SubWorld[0].m_Region[m_RegionIndex].AddRef(m_MapX, m_MapY, obj_npc);
			SubWorld[0].m_Region[m_RegionIndex].AddNpcRef(m_MapX, m_MapY);
		}
	}
}

//�ͻ���
void KNpc::ProcNetCommand(NPCCMD cmd, int x /* = 0 */, int y /* = 0 */, int z /* = 0 */)
{
	switch (cmd)
	{
	case do_death:   
		{
		DoDeath();
		//SetInstantSpr(enumINSTANT_STATE_REVIVE); //�ͷ�һ��������˲����Ч
		}
		break;
	case do_hurt:
		DoHurt(x, y, z);
		break;
	case do_revive:  //վ�ţ�
		{
		DoStand();
		m_ProcessAI    = 1;
		m_ProcessState = 1;
		//IsDeath=0;
		SetInstantSpr(enumINSTANT_STATE_REVIVE); //�ͷ�һ��������˲����Ч	
		}
		break;
	case do_stand:
		{
		DoStand();
		m_ProcessAI = 1;
		m_ProcessState = 1;
		}
	    break;
	default:
		break;
	}
}
/*
#ifndef _SERVER
void	KNpc::ClearBlood(int i)
{
	m_nBloodNo[i]		= 0;
	m_nBloodAlpha[i]	= 0;
	m_nBloodTime[i]		= 0;
	m_szBloodNo[i][0]	= 0;
}
#endif


#ifndef _SERVER
void	KNpc::SetBlood(int nNo)
{
	if (nNo <= 0)
		return;
	for (int i = 0;i < defMAX_SHOW_BLOOD_NUM;i++)
	{
		if (!m_szBloodNo[i][0])
			break;
	}
	if (i == defMAX_SHOW_BLOOD_NUM) i = 0;
	m_nBloodNo[i]		= nNo;
	m_nBloodAlpha[i]	= 0;
	m_nBloodTime[i]		= defMAX_SHOW_BLOOD_TIME;
	t_sprintf(m_szBloodNo[i], "%d", nNo);
}
#endif

#ifndef _SERVER
int	KNpc::PaintBlood(int nHeightOffset)
{
	for (int i = 0;i < defMAX_SHOW_BLOOD_NUM;i++)
	{
		if (!m_szBloodNo[i][0])
			continue;
		
		int	nHeightOff = nHeightOffset + (defMAX_SHOW_BLOOD_TIME - m_nBloodTime[i]) * defSHOW_BLOOD_MOVE_SPEED;
		int nFontSize = 14;
		DWORD	dwColor = SHOW_BLOOD_COLOR | (m_nBloodAlpha[i] << 24);
		int		nMpsX, nMpsY,nMapID;
		GetMpsPos(&nMpsX,&nMpsY,&nMapID);
		g_pRepresent->OutputText(nFontSize, m_szBloodNo[i], KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(m_szBloodNo[i]) / 4, nMpsY, dwColor, 0, nHeightOff);
		
		m_nBloodTime[i]--;
		if (m_nBloodTime[i] <= 0)
		{
			ClearBlood(i);
			continue;
		}
		m_nBloodAlpha[i]++;
		if (m_nBloodAlpha[i] > 31)
			m_nBloodAlpha[i] = 31;
	}
	
	return nHeightOffset;
}
#endif
*/

void	KNpc::SetBlood(int nNo)
{	
		if (nNo <= 0)
			return;

		BOOL flag = FALSE;
		int i = 0;
		for (i; i< defMAX_SHOW_BLOOD_NUM;++i)
		{
			 if (m_nBloodNo[i][0] == 0) 
			{//���˺�ֵ
				 flag = TRUE;
				 //m_nBloodNo[i][0] = nNo;                   //��ֵ�� ����������ֵ��
			     //m_nBloodNo[i][1] = defMAX_SHOW_BLOOD_TIME;//���ʱ�� ���Ǿ���/?	
				 break;
			}
		}
		
		if (flag == TRUE)
		{//�пյľ���ʾ���
			m_nBloodNo[i][0] = nNo;                   //��ֵ�� ����������ֵ��
			m_nBloodNo[i][1] = defMAX_SHOW_BLOOD_TIME;//���ʱ�� ���Ǿ���/?	
		}
		else
		{//���� ����ʾ�ڵ�һ��
			 m_nBloodNo[0][0] = nNo;
			 m_nBloodNo[0][1] = defMAX_SHOW_BLOOD_TIME;
		}
}

int	KNpc::PaintOther()
{
	if (g_GameWorld)
		g_GameWorld->DrawPrimitives_NpcOther(m_Index,m_Kind,false);
	return true;
}

//ðѪ������ʾ
int	KNpc::PaintBlood(int nHeightOffset)
{	
	int nHeight = GetNpcPate();

	if  (IsPlayer() || m_Kind==kind_dialoger)
		return false;

	nHeightOffset = nHeight/2;

	 BOOL _flag = FALSE;

	 for (int j = 0; j< defMAX_SHOW_BLOOD_NUM; ++j)
	 {
		 if (m_nBloodNo[j][0] > 0)
		{
			_flag = TRUE;
			break;
		}
	 }

	 if (_flag == FALSE )
	 {//���û��ðѪ�� ��Ҫɾ�����׽ڵ㾫��
		 if (g_GameWorld)
			 g_GameWorld->DrawPrimitives_NpcBlood(m_Index,-1,NULL,0,0,0,true);

		return nHeightOffset;
	 }
	int nFontSize = 16;
	int  nMpsX, nMpsY,nMmap;
	      GetMpsPos(&nMpsX, &nMpsY,&nMmap);

	int nHeightOff = nHeightOffset;

	for (int i = 0; i < defMAX_SHOW_BLOOD_NUM; ++i)
	{
		if (m_nBloodNo[i][0] > 0)
		{//Ѫ������0
			 nHeightOff = nHeightOffset + (defMAX_SHOW_BLOOD_TIME - m_nBloodNo[i][1]) * defSHOW_BLOOD_MOVE_SPEED;
			 t_sprintf(m_szBloodNo,"%d",m_nBloodNo[i][0]); //ðѪ������ʾ
			 if (g_GameWorld)
				 g_GameWorld->DrawPrimitives_NpcBlood(m_Index,i,m_szBloodNo,nMpsX,nMpsY,nHeightOff,false); // - nFontSize * g_StrLen(m_szBloodNo)/4

			 m_nBloodNo[i][1]--; //ʱ���1

			 if (m_nBloodNo[i][1] <=0)
			 {//��վ���
				 if (g_GameWorld)
					 g_GameWorld->DrawPrimitives_NpcBlood(m_Index,i,NULL,0,0,0,true);

				 m_nBloodNo[i][0] = 0;
				 m_nBloodNo[i][1] = 0;
				// m_nBloodAlpha	= 0;
				 m_szBloodNo[0]	= 0;
			 }
		 }
	}

	return nHeightOff;
}

//Ҫ��ͬ��NPC��Ϣ
void KNpc::ApplySynNpcInfo(DWORD nNpcDwid)
{
	NPC_SYN_INFO	sApplyInfo;
	sApplyInfo.ProtocolType = (BYTE)c2s_synnpcinfo;
	sApplyInfo.nNpcDwid=nNpcDwid;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApplyInfo, sizeof(NPC_SYN_INFO));
}


int	KNpc::PaintNewBlood(int nHeightOffset,int Val)
{	
	int nFontSize = 12;
	//DWORD dwColor = SHOW_BLOOD_COLOR | (m_nBloodAlpha << 24); //��ɫ
	//DWORD dwColor = SHOW_BLOOD_COLOR;
     DWORD	dwColor=TGetColor("255,255,255");
	 int  nMpsX, nMpsY,nMmap;
	      GetMpsPos(&nMpsX, &nMpsY,&nMmap);
	 char nNewBlood[32];
	int nHeightOff = nHeightOffset;
			 t_sprintf(nNewBlood, "%d",Val); //ðѪ������ʾ 
//			 g_pRepresent->OutputText(nFontSize, nNewBlood, KRF_ZERO_END, nMpsX - nFontSize * g_StrLen(nNewBlood) / 4, nMpsY, dwColor, 0, nHeightOff,0XFF000000);
			 
	//m_nBloodAlpha++;
	//if (m_nBloodAlpha > 31)
	//	m_nBloodAlpha = 31;

	return nHeightOff;
}
//�߶ȵ���
int	KNpc::GetNpcPate()
{
	int nHeight = m_Height + m_nStature;

	if (m_Kind == kind_player)
	{
		if (m_nSex)
			nHeight += 80;	//Ů
		else
			nHeight += 80;	//��

		if (m_MaskType == 0)
		{//û����ߵ�
			if (m_Doing == do_sit &&(10*m_Frames.nCurrentFrame/m_Frames.nTotalFrame) >= 8)
				nHeight -= (30*m_Frames.nCurrentFrame/m_Frames.nTotalFrame);
			
			if (m_bRideHorse)
				nHeight += 20;	//����
		}
		else
		{//�����
			nHeight += 20;
		}
		
	}
	return nHeight;
}
//��������ʾ
int	KNpc::GetNpcPatePeopleInfo()
{
	int nFontSize = 18;
	if (m_nChatContentLen > 0 && m_nChatNumLine > 0)
		return m_nChatNumLine * (nFontSize + 1);

	int nHeight = 0;
	if (NpcSet.CheckShowLife())
	{
		if (m_Kind == kind_player || m_Kind == kind_partner)
		{
			if (m_CurrentLifeMax > 0 &&\
				(relation_enemy == NpcSet.GetRelation(m_Index, Player[CLIENT_PLAYER_INDEX].m_nIndex))
				)
				nHeight += SHOW_LIFE_HEIGHT;
		}
	}
	if (NpcSet.CheckShowName())
	{
		if (nHeight != 0)
			nHeight += SHOW_SPACE_HEIGHT;//�ÿ�

		if (m_Kind == kind_player || m_Kind == kind_dialoger)
			nHeight += nFontSize + 1;
	}
	return nHeight;
}

void KNpc::GetFrameCopyFromTemplate(int nNpcTemplateId, int nLevel)
{
	if (nNpcTemplateId < 0 || nLevel < 1 ) 
		return ;
	KNpcTemplate* pNpcTemp = NULL;
	if (pNpcTemp = NpcSet.GetTemplate(nNpcTemplateId, nLevel))
	//if (g_pNpcTemplate[nNpcTemplateId][nLevel])        //������Ч�򿽱���������������
		LoadFrameFromTemplate(pNpcTemp);//(nNpcTemplateId, nLevel); //��ģ���ȡ����
	/*else
	{
		if (!g_pNpcTemplate[nNpcTemplateId][0])
		{
			g_pNpcTemplate[nNpcTemplateId][0] = new KNpcTemplate;
			g_pNpcTemplate[nNpcTemplateId][0]->InitNpcBaseData(nNpcTemplateId);  //NPC����
			g_pNpcTemplate[nNpcTemplateId][0]->m_NpcSettingIdx = nNpcTemplateId;
			g_pNpcTemplate[nNpcTemplateId][0]->m_bHaveLoadedFromTemplate = TRUE;
		}

		KLuaScript * pLevelScript = NULL;		
		
#ifdef _SERVER	 //g_pNpcTemplate[nNpcTemplateId][0]
		pLevelScript = (KLuaScript*)g_GetScript(
			g_pNpcTemplate[nNpcTemplateId][0]->m_dwLevelSettingScript
			);
		
		if (pLevelScript == NULL)
			pLevelScript = g_pNpcLevelScript;
#else
		KLuaScript LevelScript;
		if (!g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript[0])
			pLevelScript = g_pNpcLevelScript;
		else
		{
			LevelScript.Init();
			if (!LevelScript.Load(g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript))
			{
				//g_DebugLog ("[error]��������,�޷���ȷ��ȡ[%s]", g_pNpcTemplate[nNpcTemplateId][0]->m_szLevelSettingScript);
				//_ASSERT(0);
				pLevelScript = g_pNpcLevelScript;
				LevelScript.Exit();
			}
			else
				pLevelScript = &LevelScript;
		}
		
#endif
		g_pNpcTemplate[nNpcTemplateId][nLevel] = new KNpcTemplate;
		*g_pNpcTemplate[nNpcTemplateId][nLevel] = *g_pNpcTemplate[nNpcTemplateId][0];
		g_pNpcTemplate[nNpcTemplateId][nLevel]->m_nLevel = nLevel;
		g_pNpcTemplate[nNpcTemplateId][nLevel]->InitNpcLevelData(nNpcTemplateId, pLevelScript, nLevel);
		g_pNpcTemplate[nNpcTemplateId][nLevel]->m_bHaveLoadedFromTemplate = TRUE;
		LoadFrameFromTemplate(nNpcTemplateId,nLevel);
	   }
	 */

}
//��߱���
void	KNpc::ReSetRes(int nMark)
{
	char	szNpcTypeName[32];
	if (nMark == 1)
	{//�ָ�ԭ��������
	  if (m_NpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID || m_NpcSettingIdx == PLAYER_FEMALE_NPCTEMPLATEID)
	  { //���������Ļ�
		if (m_NpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID)
		{
			strcpy(szNpcTypeName, "������");  //"������"
			/*strcpy(szNpcTypeName, "ani018");*/
			m_StandFrame = NpcSet.GetPlayerStandFrame(TRUE);
			m_WalkFrame  = NpcSet.GetPlayerWalkFrame(TRUE);
			m_RunFrame   = NpcSet.GetPlayerRunFrame(TRUE);
		}
		else
		{
			strcpy(szNpcTypeName, "Ů����"); //"Ů����"
			m_StandFrame = NpcSet.GetPlayerStandFrame(FALSE);
			m_WalkFrame  = NpcSet.GetPlayerWalkFrame(FALSE);
			m_RunFrame   = NpcSet.GetPlayerRunFrame(FALSE);
		}
		m_WalkSpeed   = NpcSet.GetPlayerWalkSpeed();
		m_RunSpeed    = NpcSet.GetPlayerRunSpeed();
		m_AttackFrame = NpcSet.GetPlayerAttackFrame();
		m_HurtFrame	  = NpcSet.GetPlayerHurtFrame();

	  }
	  else
	  {//��ͨnpc ����
			GetNpcCopyFromTemplate(m_NpcSettingIdx,m_Level);  //���� NPCs.TXT ����Ļ�����Ϣ����			
			g_NpcSetting.GetString(m_NpcSettingIdx + 2, "NpcResType", "enemy003", szNpcTypeName, sizeof(szNpcTypeName));
			if (!szNpcTypeName[0])
			{ //�������ͱ� npcres
				g_NpcKindFile.GetString(2, "��������", "enemy003", szNpcTypeName, sizeof(szNpcTypeName));//���û�ҵ����õ�һ��npc����
			} 
	  }
	  m_DataRes.Remove(m_Index);                //�����Ӱ  m_Index
	  m_DataRes.Init(szNpcTypeName,&g_NpcResList,m_Index);
	  m_DataRes.SetAction(m_ClientDoing);
	  m_DataRes.SetRideHorse(m_bRideHorse);
	  m_DataRes.SetArmor(m_ArmorType);
	  m_DataRes.SetHelm(m_HelmType);
	  m_DataRes.SetHorse(m_HorseType);
	  m_DataRes.SetWeapon(m_WeaponType);
	  m_DataRes.SetPifeng(m_PifengType);
	  m_DataRes.SetChiBang(m_ChiBangType);
	  if (g_GameWorld)
		  g_GameWorld->setBuWeiHide(m_Index,true,true);
	}
	else
	{//����ͱ���
		GetFrameCopyFromTemplate(m_MaskType,m_Level);	//��ȡNPC������ ���� =m_MaskType
		g_NpcSetting.GetString(m_MaskType + 2, "NpcResType", "", szNpcTypeName, sizeof(szNpcTypeName));	// ���
		
		if (!szNpcTypeName[0])
		{
			g_NpcKindFile.GetString(4, "��������", "enemy003", szNpcTypeName, sizeof(szNpcTypeName));//���û�ҵ����õ�һ��npc����
		}
		m_DataRes.Remove(m_Index);               //�����Ӱ m_Index
		m_DataRes.Init(szNpcTypeName,&g_NpcResList,m_Index);
		m_DataRes.SetAction(m_ClientDoing);

		if (g_GameWorld)
			g_GameWorld->setBuWeiHide(m_Index,false,true);
		/*m_DataRes.SetRideHorse(0);
		m_DataRes.SetArmor(m_ArmorType);
		m_DataRes.SetHelm(m_HelmType);
		m_DataRes.SetHorse(0);
		m_DataRes.SetWeapon(0);
		m_DataRes.SetPifeng(0);
		m_DataRes.SetChiBang(0);*/
	}
}
//---------------

void	KNpc::LoadFrameFromTemplate(VOID* nNpcTemp)//(int nNpcTemplateId, int nLevel)
{
	/*if (nNpcTemplateId < 0 )
	{
		return ;
	}
	
	KNpcTemplate* pNpcTemp = NULL;
	if (pNpcTemp = NpcSet.GetTemplate(nNpcTemplateId, nLevel))
   */
	//KNpcTemplate * pNpcTemp = g_pNpcTemplate[nNpcTemplateId][nLevel];	  //NpcSet.GetTemplate(nNpcTemplateId, nLevel);
	KNpcTemplate* pNpcTemp =NULL;
	              pNpcTemp  =  (KNpcTemplate *)nNpcTemp;

	if (!pNpcTemp)
		return ;

	m_HeadImage        =	pNpcTemp->m_HeadImage;
	m_CorpseSettingIdx =	pNpcTemp->m_CorpseSettingIdx;
	m_DeathFrame       =	pNpcTemp->m_DeathFrame;
	m_WalkFrame        =	pNpcTemp->m_WalkFrame;
	m_RunFrame         =	pNpcTemp->m_RunFrame;
	m_HurtFrame        =	pNpcTemp->m_HurtFrame;
	//m_AttackFrame      =	pNpcTemp->m_AttackFrame;
	//m_CastFrame        =	pNpcTemp->m_CastFrame;
	m_StandFrame       =    pNpcTemp->m_StandFrame;
	m_StandFrame1      =    pNpcTemp->m_StandFrame1;
	m_ArmorType				= pNpcTemp->m_ArmorType;
	m_HelmType				= pNpcTemp->m_HelmType;
	m_WeaponType			= pNpcTemp->m_WeaponType;
 // m_PifengType		    = pNpcTemp->m_PifengType; //����
//	m_HorseType				= pNpcTemp->m_HorseType;
//	m_bRideHorse			= pNpcTemp->m_bRideHorse;	
}
//�ƶ�Npc 
BOOL KNpc::DoSecMove(int nDistance)
{
	WaitForFrame();
	if (nDistance <= 0)
		return FALSE;

	if (m_RegionIndex < 0 || m_RegionIndex >= 9)
	{
		//g_DebugLog("[zroc]Npc(%d)ServerMove RegionIdx = %d", m_Index, m_RegionIndex);
		DoStand();
		return FALSE;
	}
	int x, y;

	SubWorld[m_SubWorldIndex].NewMap2Mps(m_RegionIndex, m_MapX, m_MapY, m_OffX, m_OffY, &x, &y);
	int nDis = SubWorld[m_SubWorldIndex].GetDistance(x,y,m_DesX,m_DesY);

	if (nDis > nDistance)
	{
		if (m_JumpDir >= MaxMissleDir)
			m_JumpDir -= MaxMissleDir;

		x = m_DesX - (g_DirCos(m_JumpDir, MaxMissleDir) * (nDis - nDistance) >> 10);
		y = m_DesY - (g_DirSin(m_JumpDir, MaxMissleDir) * (nDis - nDistance) >> 10);
	}
	else
	{
		x = m_DesX;
		y = m_DesY;
	}

	int nOldRegion = m_RegionIndex;
	int nOldMapX = m_MapX;
	int nOldMapY = m_MapY;
	int nOldOffX = m_OffX;
	int nOldOffY = m_OffY;
	
	//	����NPC��������
	//	CELLWIDTH��CELLHEIGHT��OffX��OffY���ǷŴ���1024��
	
	if (!m_bClientOnly)
		CURREGION.DecNpcRef(m_MapX, m_MapY);

	SubWorld[m_SubWorldIndex].Mps2Map(x,y,&m_RegionIndex,&m_MapX,&m_MapY,&m_OffX,&m_OffY);

	if (!m_bClientOnly && m_RegionIndex >= 0)
		CURREGION.AddNpcRef(m_MapX,m_MapY);
		//CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
	
	
	if (m_RegionIndex == -1)	// �������ƶ���-1 Region�������������������ָ�ԭ����
	{
		m_RegionIndex = nOldRegion;
		m_MapX = nOldMapX;
		m_MapY = nOldMapY;
		m_OffX = nOldOffX;
		m_OffY = nOldOffY;
		//CURREGION.AddRef(m_MapX, m_MapY, obj_npc);
		CURREGION.AddNpcRef(m_MapX,m_MapY);
		return FALSE;
	}
	
	if (nOldRegion != m_RegionIndex)
	{
		SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[nOldRegion].m_RegionID, SubWorld[0].m_Region[m_RegionIndex].m_RegionID, m_Index);
		m_dwRegionID = SubWorld[0].m_Region[m_RegionIndex].m_RegionID;
	}
	return FALSE;
}

//============================
void KNpc::SetRankFF(int nRankFF)
{	
	m_btRankFFId = nRankFF;
}

//spr״̬�ƺ�==========
void KNpc::PaintHonor(int nbtRankFFId,int nMpsX,int nMpsY,int i)
{
	KTabFile nBank;
   nBank.Load("\\Settings\\HonorSetting.txt");
   int nRows=nBank.GetHeight(); //����
   int nLies=nBank.GetWidth();  //����
	char szImageName[128], nLuaField[64];
	int n_X, n_Y,mInterval=100;
	ZeroMemory(szImageName,strlen(szImageName));
    ZeroMemory(nLuaField,strlen(nLuaField));
     if (nbtRankFFId<=nRows)
	 {
          nBank.GetString(nbtRankFFId,"HONORLINK","",nLuaField,sizeof(nLuaField)); 
		  t_sprintf(szImageName,"\\spr\\skill\\others\\%s",nLuaField);
		  nBank.GetInteger(nbtRankFFId,"HONORX",10,&n_X);
		  nBank.GetInteger(nbtRankFFId,"HONORY",10,&n_Y);
		  nBank.GetInteger(nbtRankFFId,"nInterval",100,&mInterval);  
	 }
	 else
	 {//����
		 t_sprintf(szImageName,"\\spr\\skill\\others\\������.spr");

	 }
	 this->SetFrameSpr(szImageName, n_X, n_Y+6,GetNpcPate(),mInterval);
	 //SetClientSpr(szImageName,nMpsX+n_X,nMpsY+n_Y,0,i);
	 ZeroMemory(szImageName,strlen(szImageName));
     ZeroMemory(nLuaField,strlen(nLuaField));
     nBank.Clear();
}
//����=================
void KNpc::PaintPifeng(int m_PifengType,int nMpsX,int nMpsY,int i)
{	
	KIniFile nBank;
	nBank.Load("\\Ui\\npcbobo.ini");
	int nCount=0,nTime=20;
	char szImageName[128]={0},nDir[8]={0};
	      nBank.GetInteger("FortuneRank","count",11,&nCount);
		  nBank.GetInteger("FortuneRank","nTime",20,&nTime);
		 if (m_PifengType>nCount)
				 m_PifengType=nCount;
		 /*if (m_PiFenLoop%18*nTime==0)
		 {
            m_PifengType=GetRandomNumber(0,nCount);
			if  (m_PiFenLoop>50000)
			     m_PiFenLoop=0;
		 }*/
        t_sprintf(nDir,"Spr_%d",m_PifengType);
		nBank.GetString("FortuneRank",nDir,"",szImageName,sizeof(szImageName)); 		
	    SetClientSpr(szImageName,nMpsX,nMpsY,0,i); //szImageName
	    nBank.Clear();
}


//#ifdef _SERVER
//������ʼ��(�����ǳ�ʼ��)
int KNpc::NpcNewInitJinMaiVal()
{

    int nPlayeridx=0;

    nPlayeridx=CLIENT_PLAYER_INDEX;
    KTabFile nMagFile;
	  if(!nMagFile.Load(TABFILE_MAGICAGOLD_PATH)) //"\\Settings\\item\\004\\GoldMagic.txt"
	  {
		Player[nPlayeridx].m_ItemList.ClientShowMsg("�����ļ�ʧ��C!");
		return 0;
	  }
	//int nLeftTime = Player[nPlayeridx].m_cTask.GetSaveVal(TASKVALUE_LIXIAN); //����ʱ��


	BYTE nDuMaiStaus=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3501),1); //�������� ���� 0 ��ʾ�ر��� 1 ��ʾ������ 2 ��ʾ�����
	BYTE nDuMaiRow=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3501),2); //��ʼ������
	//BYTE nDuMaiCurXue=Player[nPlayeridx].m_cTask.GetTaskByte(Player[nPlayeridx].m_cTask.GetSaveVal(3501),3); //��ǰѨ��
		 //Player[nPlayeridx].m_cTask.GetTaskByte(Player[nPlayeridx].m_cTask.GetSaveVal(3501),4);  
	BYTE nRenMaiStaus=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3502),1);
	BYTE nRenMaiRow=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3502),2);
	//BYTE nRenMaiCurXue=Player[nPlayeridx].m_cTask.GetTaskByte(Player[nPlayeridx].m_cTask.GetSaveVal(3502),3);

	BYTE nChoMaiStaus=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3503),1);
	BYTE nChoMaiRow=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3503),2);
	//BYTE nChoMaiCurXue=Player[nPlayeridx].m_cTask.GetTaskByte(Player[nPlayeridx].m_cTask.GetSaveVal(3503),3);

	BYTE nDaiMaiStaus=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3504),1);
	BYTE nDaiMaiRow=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3504),2);
	//BYTE nDaiMaiCurXue=Player[nPlayeridx].m_cTask.GetTaskByte(Player[nPlayeridx].m_cTask.GetSaveVal(3504),3);

	BYTE nYinWMaiStaus=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3505),1);
	BYTE nYinWMaiRow=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3505),2);
	//BYTE nYinWMaiCurXue=Player[nPlayeridx].m_cTask.GetTaskByte(Player[nPlayeridx].m_cTask.GetSaveVal(3505),3);

	BYTE nYanWMaiStaus=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3506),1);
	BYTE nYanWMaiRow=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3506),2);
	//BYTE nYanWMaiCurXue=Player[nPlayeridx].m_cTask.GetTaskByte(Player[nPlayeridx].m_cTask.GetSaveVal(3506),3);

	BYTE nYinQMaiStaus=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3507),1);
	BYTE nYinQMaiRow=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3507),2);
	//BYTE nYinQMaiCurXue=Player[nPlayeridx].m_cTask.GetTaskByte(Player[nPlayeridx].m_cTask.GetSaveVal(3507),3);
	//BYTE nYinQMaiStaus=Player[nPlayeridx].m_cTask.GetTaskByte(Player[nPlayeridx].m_cTask.GetSaveVal(3507),4);

	BYTE nYanQMaiStaus=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3508),1);
	BYTE nYanQMaiRow=EGetByte(Player[nPlayeridx].m_cTask.GetSaveVal(3508),2);
	//BYTE nYanQMaiCurXue=Player[nPlayeridx].m_cTask.GetTaskByte(Player[nPlayeridx].m_cTask.GetSaveVal(3508),3);


	int nNpcIdx = m_Index;
    int nAllStaus=0;
	if (nNpcIdx>0)
	{	
		if (nDuMaiStaus==2&&  //�������
			nRenMaiStaus==2&&    
			nChoMaiStaus==2&&
			nDaiMaiStaus==2&&
			nYinWMaiStaus==2&&
			nYanWMaiStaus==2&&
			nYinQMaiStaus==2&&
			nYanQMaiStaus==2
			)
		{//����ȫ������������ɴ���  ����ȫ�� �� ���ܼӳ�
            /*  int nMacLevel[2];
			g_FsJinMai.GetInt2(114,6,nMacLevel);
			UseSkills(nMacLevel[0],nMacLevel[1],nNpcIdx);*/ //���ü��ܹ⻷Ч��
			nAllStaus=1;
		}
      //�����Ǵ�����������ķ���״̬
	 KMagicAttrib m_CurrentJinMai;	    // Npc�ĵ�ǰ��������
	 ZeroStruct(m_CurrentJinMai);

     for (int i=1;i<9;++i)
	 {
		//char mMagcid[16];
		//int mMgLevel=0,nIfMagic=1,nTime=-1;
		if (nAllStaus==1)
		{//����ȫ����ɴ��� ��֧״̬
		    int nRows=g_FsJinMai.GetHeight();
            for (int k=1;k<nRows;++k)  //13��������������
			{   
			    /*if (k>nDuMaiCurXue)
				    break;*/
                int nMacidx;
                
				g_FsJinMai.GetInteger(k+1,7,0,&nMacidx);
                int mAttribType,mValue;
				nMagFile.GetInteger(nMacidx+1,5,0,&mAttribType) ;
				nMagFile.GetInteger(nMacidx+1,6,0,&mValue) ;
				//UseSkills(nMacLevel[0],nMacLevel[1],nNpcIdx); //���ü��ܹ⻷Ч��
				
			  //const  KItemNormalAttrib* pAttrib;
		      //pAttrib = &(m_aryMagicAttrib[i]);
              /*
			    	KMagicAttrib* pTemp = NULL;
		            pTemp = (KMagicAttrib *)pData;  //���µȼ�����
			  */
				m_CurrentJinMai.nAttribType=mAttribType;
                m_CurrentJinMai.nValue[0]=mValue;
                m_CurrentJinMai.nValue[1]=-1;
				m_CurrentJinMai.nValue[2]=0;
                // pAttrib=&(m_CurrentJinMai);
		       if (-1 != m_CurrentJinMai.nAttribType)
			   {//������Ӧ����NPC����
					ModifyAttrib(nNpcIdx, &m_CurrentJinMai);
			   }	
			} 
			//UseSkills(533,1,nNpcIdx); //���ü��ܹ⻷Ч��
		}
		else 	
		{//�������
		 //int nMacLevel[2];//nMacLevel[0]=����ID,nMacLevel[1]=���ܵȼ�
			if (i==1 && nDuMaiStaus>0) //����״̬Ϊ������
			{
			   
			  for (int k=1;k<17;++k)  //13��������������
			  {   
			    if (k>nDuMaiRow)
				    break;

                int nMacidx;

				g_FsJinMai.GetInteger(k+1,7,0,&nMacidx);
                int mAttribType=0,mValue=0;
				nMagFile.GetInteger(nMacidx+1,"���Ե������",0,&mAttribType) ;
				nMagFile.GetInteger(nMacidx+1,"����1��Сֵ",0,&mValue) ;
				//UseSkills(nMacLevel[0],nMacLevel[1],nNpcIdx); //���ü��ܹ⻷Ч��
				
                //char msg[64];

			    /*KItemNormalAttrib* pAttrib=NULL;
		        //pAttrib = &(m_aryMagicAttrib[i]);
				pAttrib->nAttribType=mAttribType;
                pAttrib->nValue[0]=mValue;
                pAttrib->nValue[1]=-1;
				pAttrib->nValue[2]=0;

		        if (-1 != pAttrib->nAttribType)
	            {//������Ӧ����NPC����
					ModifyAttrib(nNpcIdx, (void *)pAttrib);
	            }*/
				m_CurrentJinMai.nAttribType=mAttribType;
                m_CurrentJinMai.nValue[0]=mValue;
                m_CurrentJinMai.nValue[1]=-1;
				m_CurrentJinMai.nValue[2]=0;

                // pAttrib=&(m_CurrentJinMai);

		        if (m_CurrentJinMai.nAttribType)
				{//������Ӧ����NPC����
					ModifyAttrib(nNpcIdx, &m_CurrentJinMai);
				}
                /*#ifdef _SERVER
				  t_sprintf(msg,"����(%d)״̬S:%d,Ѩ��:%d,����:%d,ֵ:%d||%d",nMacidx,nDuMaiStaus,nDuMaiRow,mAttribType,mValue,m_CurrentJinMai.nValue[0]);
                  Player[nPlayeridx].m_ItemList.msgshow(msg);
	            #else
				  t_sprintf(msg,"����(%d)״̬C:%d,Ѩ��:%d,����:%d,ֵ:%d||%d",nMacidx,nDuMaiStaus,nDuMaiRow,mAttribType,mValue,m_CurrentJinMai.nValue[0]);
				  Player[nPlayeridx].m_ItemList.ClientShowMsg(msg);
				#endif*/
					
			  } 
			    
			//	UseSkills(533,1,nNpcIdx); //���ü��ܹ⻷Ч��
			}
			else if (i==2 && nRenMaiStaus>0) //��������	
			{
			  for (int k=1;k<17;++k)  //13��������������
			  {   
			    if (k>nRenMaiRow)
				    break;

                int nMacidx;

				g_FsJinMai.GetInteger(k+17,7,0,&nMacidx);
                int mAttribType,mValue;
				nMagFile.GetInteger(nMacidx+1,5,0,&mAttribType) ;
				nMagFile.GetInteger(nMacidx+1,6,0,&mValue) ;
				//UseSkills(nMacLevel[0],nMacLevel[1],nNpcIdx); //���ü��ܹ⻷Ч��
				
			    m_CurrentJinMai.nAttribType=mAttribType;
                m_CurrentJinMai.nValue[0]=mValue;
                m_CurrentJinMai.nValue[1]=-1;
				m_CurrentJinMai.nValue[2]=0;

                // pAttrib=&(m_CurrentJinMai);

		        if (m_CurrentJinMai.nAttribType)
				{//������Ӧ����NPC����
					ModifyAttrib(nNpcIdx, &m_CurrentJinMai);
				}
			  	
			  } 			
				//UseSkills(533,1,nNpcIdx); //���ü��ܹ⻷Ч��
			}
			else if (i==3 && nChoMaiStaus>0) //��������
			{
			  for (int k=1;k<17;++k)  //13��������������
			  {   
			    if (k>nChoMaiRow)
				    break;

                int nMacidx;

				g_FsJinMai.GetInteger(k+33,7,0,&nMacidx);
                int mAttribType,mValue;
				nMagFile.GetInteger(nMacidx+1,5,0,&mAttribType) ;
				nMagFile.GetInteger(nMacidx+1,6,0,&mValue) ;
				//UseSkills(nMacLevel[0],nMacLevel[1],nNpcIdx); //���ü��ܹ⻷Ч��
				
			    m_CurrentJinMai.nAttribType=mAttribType;
                m_CurrentJinMai.nValue[0]=mValue;
                m_CurrentJinMai.nValue[1]=-1;
				m_CurrentJinMai.nValue[2]=0;

                // pAttrib=&(m_CurrentJinMai);

		        if (m_CurrentJinMai.nAttribType)
				{//������Ӧ����NPC����
					ModifyAttrib(nNpcIdx, &m_CurrentJinMai);
				}	
			  } 							
				//UseSkills(533,1,nNpcIdx); //���ü��ܹ⻷Ч��
			}	
 	
			else if (i==4 && nDaiMaiStaus>0) //��������	
			{
			  for (int k=1;k<17;++k)  //13��������������
			  {   
			    if (k>nDaiMaiRow)
				    break;

                int nMacidx;

				g_FsJinMai.GetInteger(k+49,7,0,&nMacidx);
                int mAttribType,mValue;
				nMagFile.GetInteger(nMacidx+1,5,0,&mAttribType) ;
				nMagFile.GetInteger(nMacidx+1,6,0,&mValue) ;
				//UseSkills(nMacLevel[0],nMacLevel[1],nNpcIdx); //���ü��ܹ⻷Ч��
				
			    m_CurrentJinMai.nAttribType=mAttribType;
                m_CurrentJinMai.nValue[0]=mValue;
                m_CurrentJinMai.nValue[1]=-1;
				m_CurrentJinMai.nValue[2]=0;

                // pAttrib=&(m_CurrentJinMai);

		        if (m_CurrentJinMai.nAttribType)
				{//������Ӧ����NPC����
					ModifyAttrib(nNpcIdx, &m_CurrentJinMai);
				}	
			  } 											
				//UseSkills(533,1,nNpcIdx); //���ü��ܹ⻷Ч��
			}
			else if (i==5 && nYinWMaiStaus>0) //��ά����	
			{
			  for (int k=1;k<17;++k)  //13��������������
			  {   
			    if (k>nYinWMaiRow)
				    break;

                int nMacidx;

				g_FsJinMai.GetInteger(k+65,7,0,&nMacidx);
                int mAttribType,mValue;
				nMagFile.GetInteger(nMacidx+1,5,0,&mAttribType) ;
				nMagFile.GetInteger(nMacidx+1,6,0,&mValue) ;
				//UseSkills(nMacLevel[0],nMacLevel[1],nNpcIdx); //���ü��ܹ⻷Ч��
				
			    m_CurrentJinMai.nAttribType=mAttribType;
                m_CurrentJinMai.nValue[0]=mValue;
                m_CurrentJinMai.nValue[1]=-1;
				m_CurrentJinMai.nValue[2]=0;

                // pAttrib=&(m_CurrentJinMai);

		        if (m_CurrentJinMai.nAttribType)
				{ //������Ӧ����NPC����
					ModifyAttrib(nNpcIdx, &m_CurrentJinMai);
				}	 
			  } 
			  //UseSkills(533,1,nNpcIdx); //���ü��ܹ⻷Ч��												
			}
			else if (i==6 && nYanWMaiStaus>0) //��ά����	
			{
			  for (int k=1;k<17;++k)  //13��������������
			  {   
			    if (k>nYanWMaiRow)
				    break;

                int nMacidx;

				g_FsJinMai.GetInteger(k+81,7,0,&nMacidx);
                int mAttribType,mValue;
				nMagFile.GetInteger(nMacidx+1,5,0,&mAttribType) ;
				nMagFile.GetInteger(nMacidx+1,6,0,&mValue) ;
				//UseSkills(nMacLevel[0],nMacLevel[1],nNpcIdx); //���ü��ܹ⻷Ч��
				
			    m_CurrentJinMai.nAttribType=mAttribType;
                m_CurrentJinMai.nValue[0]=mValue;
                m_CurrentJinMai.nValue[1]=-1;
				m_CurrentJinMai.nValue[2]=0;

                // pAttrib=&(m_CurrentJinMai);

		        if (m_CurrentJinMai.nAttribType)
				{ //������Ӧ����NPC����
					ModifyAttrib(nNpcIdx, &m_CurrentJinMai);
				}	 
			  } 										
				//UseSkills(533,1,nNpcIdx); //���ü��ܹ⻷Ч��
			} 
			else if (i==7 && nYinQMaiStaus>0) //���η���	
			{
			  for (int k=1;k<17;++k)  //13��������������
			  {   
			    if (k>nYinQMaiRow)
				    break;

                int nMacidx;

				g_FsJinMai.GetInteger(k+97,7,0,&nMacidx);
                int mAttribType,mValue;
				nMagFile.GetInteger(nMacidx+1,5,0,&mAttribType) ;
				nMagFile.GetInteger(nMacidx+1,6,0,&mValue) ;
				//UseSkills(nMacLevel[0],nMacLevel[1],nNpcIdx); //���ü��ܹ⻷Ч��
				
			   m_CurrentJinMai.nAttribType=mAttribType;
                m_CurrentJinMai.nValue[0]=mValue;
                m_CurrentJinMai.nValue[1]=-1;
				m_CurrentJinMai.nValue[2]=0;

                // pAttrib=&(m_CurrentJinMai);

		        if (m_CurrentJinMai.nAttribType)
				{ //������Ӧ����NPC����
					ModifyAttrib(nNpcIdx, &m_CurrentJinMai);
				}	 
			  } 
			  //UseSkills(533,1,nNpcIdx); //���ü��ܹ⻷Ч��												
			} 
			else if (i==8 && nYanQMaiStaus>0) //���η���	
			{
			  for (int k=1;k<17;++k)  //13��������������
			  {   
			    if (k>nYanQMaiRow)
				    break;

                int nMacidx;

				g_FsJinMai.GetInteger(k+113,7,0,&nMacidx);
                int mAttribType,mValue;
				nMagFile.GetInteger(nMacidx+1,5,0,&mAttribType) ;
				nMagFile.GetInteger(nMacidx+1,6,0,&mValue) ;
				//UseSkills(nMacLevel[0],nMacLevel[1],nNpcIdx); //���ü��ܹ⻷Ч��
				
			    m_CurrentJinMai.nAttribType=mAttribType;
                m_CurrentJinMai.nValue[0]=mValue;
                m_CurrentJinMai.nValue[1]=-1;
				m_CurrentJinMai.nValue[2]=0;

                // pAttrib=&(m_CurrentJinMai);

		        if (m_CurrentJinMai.nAttribType)
				{ //������Ӧ����NPC����
					ModifyAttrib(nNpcIdx, &m_CurrentJinMai);
				}	 
			  } 										
				//UseSkills(533,1,nNpcIdx); //���ü��ܹ⻷Ч��
			} 
							
		}
	 }

	} 
	else
	{
	  nMagFile.Clear();
      return 0;
	}

	nMagFile.Clear();
    return 1;
}

void KNpc::UseSkills(int sKillID,int sLevel,int nNpcIdx,KMagicAttrib *pData,BOOL nIfMagic,int nTime)
{										   
	      KSkill *pSkill = (KSkill*)g_SkillManager.GetSkill(sKillID,sLevel);
		  if (nIfMagic)
		  {	   if (pSkill)
			      pSkill->CastStateSkill(nNpcIdx,0,0,nTime);  //�Ƿ�
		  }
		  else
		     Npc[nNpcIdx].SetStateSkillEffect(nNpcIdx,sKillID,sLevel,pData,0,nTime);  //���ü��ܹ⻷Ч��	
}

//#endif

//����������Թ������ܵļӳ�
void KNpc::ReFullManaSkillEnhance(int nLeftSkill,int nLeftListidx)
{
	int nActiveSkillID,nCurSkillEnchance=0;
	nActiveSkillID = nLeftSkill;
	//�ͻ�����Ҫ��װ�����޸����յ�½ʱ��ɵļӳ��������
	if (IsPlayer())
	{
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax <= Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana)
			nCurSkillEnchance = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Currentskillenhance+Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentFullManaskillenhance;
		else
			nCurSkillEnchance = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Currentskillenhance;
	}

	if (nCurSkillEnchance!= 0)
	{
		int nListIndex = 0;
		if  (nLeftListidx>0)
			nListIndex  = nLeftListidx;
		else
			nListIndex = m_SkillList.FindSame(nActiveSkillID);

		if (nListIndex)                 //���Ҽ���ĵ�ǰ��������ID
		{
			int nEnchance = m_SkillList.GetTempEnChance(nListIndex);  //��ȡԭʼ�ļ��ܼӳ� 

			if (m_SkillList.GetCurrentLevelByIdx(nListIndex) > 0)  //�ȼ�����0�Ĳ� ����
			{
				//printf("----�������:%d ԭʼ�ӳ�:%d -----\n",nListIndex,nEnchance);
				//char Infostr[64]={0};
				//t_sprintf(Infostr,"----�������:%d ԭʼ�ӳ�:%d -----",nListIndex,nEnchance);
				//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(Infostr);
				m_SkillList.SetEnChance(nListIndex,nEnchance+nCurSkillEnchance);
				//printf("----�������: �ܼӳ�:%d������:%d -----\n",nEnchance+nCurSkillEnchance,nCurSkillEnchance);
				//t_sprintf(Infostr,"----�������: �ܼӳ�:%d������:%d -----",nEnchance+nCurSkillEnchance,nCurSkillEnchance);
				//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(Infostr);
			}			
		}
	}

}

//���ù������ܵļӳ�
void KNpc::ReSkillEnhance(int nLeftSkill,int nIsAdd)
{
	int nActiveSkillID,nCurSkillEnchance=0;
	
	nActiveSkillID = nLeftSkill;
//�ͻ�����Ҫ��װ�����޸����յ�½ʱ��ɵļӳ��������
	if (IsPlayer())
	{
     if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax <= Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana)
		nCurSkillEnchance = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Currentskillenhance+Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentFullManaskillenhance;
	 else
        nCurSkillEnchance = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Currentskillenhance;
	}

	if (nCurSkillEnchance!= 0)
	{   
		int nListIndex = m_SkillList.FindSame(nActiveSkillID);
		if (nListIndex)                 //���Ҽ���ĵ�ǰ��������ID
		{
			if (m_SkillList.GetCurrentLevelByIdx(nListIndex) > 0)  //�ȼ�����0�Ĳ� ����
			{
				if (nIsAdd==0)
				    m_SkillList.AddEnChance(nListIndex,-nCurSkillEnchance);
				else if (nIsAdd > 0)
					m_SkillList.AddEnChance(nListIndex,nCurSkillEnchance);
			}			
		}
	}
/*
#ifdef _SERVER
	char msg[64];
	t_sprintf(msg,"S�����ӳ�:%d",nCurSkillEnchance);
	Player[m_nsPlayerIdx].m_ItemList.msgshow(msg);
#else
	char msg[64];
	t_sprintf(msg,"C�����ӳ�:%d",nCurSkillEnchance);
	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(msg);	
#endif
*/
}

//����ĳ���ܵļӳ�
void KNpc::ReWhereSkillEnhance(int nActiveSkillID,int nCurSkillEnchance,int nIsAdd)
{

/*
#ifdef _SERVER
				char Msg[64];
				t_sprintf(Msg,"S:��ĳ����(%d)(%d)�ӳɳɹ�",nActiveSkillID,nCurSkillEnchance);
				Player[GetPlayerIdx()].m_ItemList.msgshow(Msg);	
#else
				char Msg[64];
				t_sprintf(Msg,"C:��ĳ����(%d)(%d)�ӳɳɹ�",nActiveSkillID,nCurSkillEnchance);
				Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(Msg);
#endif;
*/				
	if (nCurSkillEnchance!= 0)
	{
		int nListIndex = m_SkillList.FindSame(nActiveSkillID);
		if (nListIndex)                 //���Ҽ���ĵ�ǰ��������ID
		{
		  if (m_SkillList.GetCurrentLevelByIdx(nListIndex) > 0)    //�ȼ�����0�Ĳ� ����
		  {
			 if (nIsAdd==0)
				m_SkillList.AddEnChance(nListIndex,-nCurSkillEnchance);
			 else if (nIsAdd > 0)
				m_SkillList.AddEnChance(nListIndex,nCurSkillEnchance);
		  }	 		
		}
	}
}

void KNpc::NewSetNpcEnChance()
{
	int j;
	for (j = 0 ; j< MAX_NPCSKILL;++j)
	{	
		m_SkillList.m_Skills[j].EnChance = 0;  // ���ܵļӳ�����
		m_SkillList.m_Skills[j].nTempEnChance = 0;
	} 
	m_SkillList.ReEnChance();                  //���üӳ�
	ReSkillEnhance(Player[CLIENT_PLAYER_INDEX].GetLeftSkill(),1);	
	_EnhanceInfo::iterator it;
	for( it = nEnhanceInfo.begin(); it != nEnhanceInfo.end(); ++it)
	{
		if (it->second.nSkillIdx>0)
		{////����ĳ���ܵļӳ�
		   ReWhereSkillEnhance(it->second.nSkillIdx,it->second.nEnhance,1);	 		
		}
	}
}
