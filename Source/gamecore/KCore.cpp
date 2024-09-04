//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCodec.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Data Compress / Decompress Base Class
//---------------------------------------------------------------------------

#include "KCore.h"
#include "engine/KEngine.h"
#include "KSortScript.h"
#include "gamescene/KScenePlaceC.h"
#include "KItemChangeRes.h"
#include "KNpcSet.h"
#include "KSkills.h"
#include "KPlayerSet.h"
#include "KPlayerTeam.h"
#include "KMissleSet.h"
#include "KFaction.h"
#include "KMath.h"
#include "KPlayerTask.h"
#include "KSubWorldSet.h"
#include "KItemGenerator.h"
#include "KObjSet.h"
#include "KItemSet.h"
#include "KNpc.h"
#include "KPlayer.h"
#include "KNpcTemplate.h"
#include "CoreUseNameDef.h"
#include "KBuySell.h"
#include "KTaskFuns.h"
#include "TaskDef.h"
#include "LuaFuns.h"
KLuaScript	*	g_pNpcLevelScript = NULL;
KLuaScript      g_WorldScript;

//�ͻ��˿�ʼԤ��������
#define CLIENTWEAPONSKILL_TABFILE		"\\settings\\ClientWeaponSkill.txt"
#define ADJUSTCOLOR_TABFILE				"\\settings\\AdjustColor.txt"
_ItemDataInfo  iItemDataInfo;
_StrNpcInfo    strNpcInfo;
_StrObjInfo    strObjInfo;
_StrCoreInfo   strCoreInfo;

Size visibleSize;  //�ɼ���Ļ���ڵĴ�С
Point origin;       //��ʼ��λ��?
Size kSize;         //
std::string m_SDcardDirPath = "";
std::string maxMapPicPath = "";
std::string m_MobileKey     ="NYc0bUz0z_JhCNAi0WUK8JK0MOlOQlkP";
AudioEngine *__pSound = NULL;
FileUtils *ccFileUtils = NULL;
bool _openSound =true;
unsigned int curMobileVer=30;
unsigned int curMobileKey=14302019;
//KSprCodec SprDecode;

KTimer    timerStart;
KTabFile  g_ClientWeaponSkillTabFile,g_ObjChangeName;
KTabFile  g_OrdinSkillsSetting, g_MisslesSetting,g_PlayerTitle;
KTabFile  g_SkillLevelSetting,g_FsJinMai,g_ForbitMap,g_CompEquip;
KTabFile  g_NpcSetting,g_NpcKindFile ,g_RankTabSetting;
KTabFile  cTabFile,g_CompAtlas,nToolItem;
KIniFile  g_GameSetTing,g_NpcMapDropRate,g_MagicBaoshiDesc;
KIniFile  g_MapTraffic;
KTabFile  g_GetMsg;
int		  g_nMeleeWeaponSkill[MAX_MELEEWEAPON_PARTICULARTYPE_NUM];
int		  g_nRangeWeaponSkill[MAX_RANGEWEAPON_PARTICULARTYPE_NUM];
int		  g_nHandSkill;
int       nGameVersion;//ʲô�汾��GS
int       nExpSkillIdx;
int       nLuckySkillIdx;
int       nKangkillIdx;
int       nYaoPinSkillIdx;
int       nIsGaldInit=false;
bool      nIsShowShadow=true;
bool      nIsShowTitle=true;
#ifdef WIN32
bool      nisgetinfo=true;
#else
bool      nisgetinfo=false;
#endif
KPakList g_pcPakList;
Kgetinidata _getinidata;
BOOL g_bUISelIntelActiveWithServer = FALSE;//��ǰѡ����Ƿ���������˽���
BOOL g_bUISpeakActiveWithServer = FALSE;
int	 g_bUISelLastSelCount = 0;

CGameClient *nAcclient;     //�ʺ�����
CGameClient *g_pClient;     //gs����
int m_bIsClientConnecting;  //�ʺŷ������Ƿ��Ѿ�����
int m_bIsGameSevConnecting; //gs�Ƿ��Ѿ�����
int m_bIsPlayerInGame    = false;//����Ƿ��Ѿ�������Ϸ��
int _clientlanguage=2;      //1 Ϊ�й� 2 ΪԽ�� 3 Ӣ��
bool _clientNoNetWork=false; //�Ƿ񵥻�
void g_InitProtocol();
bool InitSkillSetting()
{
	if (!g_SkillManager.Init())
       messageBox("load filed.","SkillManager");
	if (!g_ClientWeaponSkillTabFile.Load(CLIENTWEAPONSKILL_TABFILE))
		messageBox("load filed..","ClientWeapon");
	return true;
}

bool InitMissleSetting()
{
	int nMissleNum = g_MisslesSetting.GetHeight() - 1;
	for (int i = 0; i < nMissleNum; ++i)
	{
		int nMissleId = 0;
		g_MisslesSetting.GetInteger(i + 2, "MissleId", -1, &nMissleId);
		if (nMissleId>=MAX_MISSLESTYLE)
			continue;
		if (nMissleId > 0)
		{
			g_MisslesLib[nMissleId].GetInfoFromTabFile(i + 2);
			g_MisslesLib[nMissleId].m_nMissleId = nMissleId;
		}
	}
	return true;
}

bool InitTaskSetting()
{
	if (!g_StringResourseTabFile.Load(STRINGRESOURSE_TABFILE))
		messageBox("[TASK]CAN NOT LOAD.","TASK");
	if (!g_RankTabSetting.Load(PLAYER_RANK_SETTING_TABFILE))
		messageBox("[RANK]CAN NOT LOAD.","RANK");
	return true;
}

bool InitNpcSetting()
{
	int nNpcTemplateNum = g_NpcSetting.GetHeight() - 1;
	//g_pNpcTemplate = new KNpcTemplate[nNpcTemplateNum * MAX_NPC_LEVEL]; //0,0Ϊ���
	//memset(g_pNpcTemplate, 0, sizeof(void*) * MAX_NPCSTYLE * MAX_NPC_LEVEL);
	// �����ļ� ��������.txt
	//	g_SetFilePath(RES_INI_FILE_PATH);
	if (!g_NpcKindFile.Load(NPC_RES_KIND_FILE_NAME) )
		messageBox("[NpcKind]CAN NOT LOAD.","NpcKind");
	//����Npc�ȼ��趨�Ľű��ļ������ڽ�����Npcʱʹ��
	g_pNpcLevelScript = new KLuaScript;
	if (!g_pNpcLevelScript)
	{
		messageBox("g_pNpcLevelScript is Error","InitNpcSetting");
	}
	else
	{
	  g_pNpcLevelScript->Init();
	  if (!g_pNpcLevelScript->Load(NPC_LEVELSCRIPT_FILENAME))
	  {
		delete g_pNpcLevelScript;
		g_pNpcLevelScript = NULL;
		messageBox("[NpcLevel]CAN NOT LOAD.","InitNpcSetting");
	  }
	}

	if (!g_NpcResList.Init())   //�ͻ��˼�������ļ�������
		messageBox("[NpcResList]CAN NOT LOAD.","NpcResList");
	return true;
}

void g_ReleaseCore()
{
//#ifdef WIN32
	g_SubWorldSet.Close();
//#endif
	g_ScenePlace.ClosePlace();

	/*if (g_pNpcLevelScript)
	{
		delete g_pNpcLevelScript;
		g_pNpcLevelScript = NULL;
	}*/
	/*
	if (g_pAdjustColorTab)
	{
		delete []g_pAdjustColorTab;
		g_pAdjustColorTab = NULL;
		g_ulAdjustColorCount = 0;
	}*/
    //g_UnInitMath();
}
void g_InitCore()
{
	//�����ʺš��͡��������
	srand((unsigned)time(NULL));
	timerStart.Start();                           //������Ϸ�ļ�ʱ��
	nAcclient = NULL;
	g_pClient = NULL;
	const size_t bufferSize           = 1024;   //Scoket���������Ĵ�С ������ڴ�(m_bufferSize > 0) ? m_bufferSize : (1024*64);
	const size_t bufferSize_Cache     = 1024*512; //������ڴ� ���� �Ӱ��Ļ����С
	const size_t maxFreeBuffers	      = 2;        //Scoket����������
	const size_t maxFreeBuffers_Cache = 2;        //���� �Ӱ��Ļ��� ����������
	nAcclient     = new CGameClient(maxFreeBuffers,maxFreeBuffers_Cache,bufferSize_Cache,bufferSize,0); //2,2   8
	//g_pClient     = new CGameClient(maxFreeBuffers,maxFreeBuffers_Cache,bufferSize_Cache,bufferSize,1); //2,2   8

	srand((unsigned)time(NULL));
	if (g_GameSetTing.Load(GAME_SETTING_FILE))
	{
		g_GameSetTing.GetInteger("SkillsIcon","ExpSkillIdx",0,&nExpSkillIdx);
		g_GameSetTing.GetInteger("SkillsIcon","LuckySkillIdx",0,&nLuckySkillIdx);
		g_GameSetTing.GetInteger("SkillsIcon","KangkillIdx",0,&nKangkillIdx);
		g_GameSetTing.GetInteger("SkillsIcon","YaoPinSkillIdx",0,&nYaoPinSkillIdx);
		g_GameSetTing.GetInteger("Gameconfig","GameVersion",1,&nGameVersion);//1Ϊ���İ汾 2 �����汾 ��Ҫ�滻�ַ�����
	}

	if (!g_ObjChangeName.Load("\\settings\\vn\\ObjData.txt"))
		messageBox("load vn-ObjData.txt Error","Warning");
		//printf("--���������ļ�ʧ��:%s\n","\\settings\\vn\\ObjData.txt");
	KTabFile nNpcInfo;  //���������洦
	/*if (nNpcInfo.Load("\\settings\\lang\\zh\\replacename_npc.txt"))
	{
		int  nRows = nNpcInfo.GetHeight()+1;

		for(int i=2;i<nRows;i++)
		{
			char nyNpcName[64]={0},ndNpcName[64]={0};
			nNpcInfo.GetString(i,"sourcename","no npc name",nyNpcName,sizeof(nyNpcName));
			nNpcInfo.GetString(i,"targetname","no npc name",ndNpcName,sizeof(ndNpcName));
			strNpcInfo[nyNpcName]=ndNpcName;
		}
		nNpcInfo.Clear();
	}

	if (nNpcInfo.Load("\\settings\\lang\\zh\\replacename_obj.txt"))
	{
		int  nRows = nNpcInfo.GetHeight()+1;
		for(int i=2;i<nRows;i++)
		{
			char ndNpcName[64]={0};//nyNpcName[64]={0},
			//nNpcInfo.GetString(i,"sourcename","no npc name",nyNpcName,sizeof(nyNpcName));
			nNpcInfo.GetString(i,"targetname","no npc name",ndNpcName,sizeof(ndNpcName));
			strObjInfo[i]=ndNpcName;
		}
		nNpcInfo.Clear();
	}
	*/
	char nTempStr[256];
	ZeroMemory(nTempStr,sizeof(nTempStr));

	if (_clientlanguage!=1)
		sprintf(nTempStr,"\\settings\\lang\\vn\\stringtable_core.txt");
	else
	    sprintf(nTempStr,"\\settings\\lang\\zh\\stringtable_core.txt");

	if (nNpcInfo.Load(nTempStr))
	{
		int  nRows = nNpcInfo.GetHeight()+1;

		for(int i=2;i<nRows;i++)
		{
			char nyNpcName[64]={0},ndNpcName[256]={0};
			nNpcInfo.GetString(i,"key","no npc name",nyNpcName,sizeof(nyNpcName));
			nNpcInfo.GetString(i,"value","no npc name",ndNpcName,sizeof(ndNpcName));
			strCoreInfo[nyNpcName]=ndNpcName;
		}
		nNpcInfo.Clear();
	}

	char *szRandomMem1, *szRandomMem2, *szRandomMem3;	//��Щ����û��Ӧ�ü�ֵ����Ҫ�Ƿ�����ҹ̶��ڴ�
	SubWorld = new KSubWorld[MAX_SUBWORLD];
	szRandomMem1 = new char[((rand() % 64) + 6) * 1024];
	Player   = new KPlayer[MAX_PLAYER];
	szRandomMem2 = new char[((rand() % 64) + 6) * 1024];
	Npc      = new KNpc[MAX_NPC];
	szRandomMem3 = new char[((rand() % 64) + 6) * 1024];
	Item     = new KItem[MAX_ITEM];
	KObject   = new KObj[MAX_OBJECT];
	Missle   = new KMissle[MAX_MISSLE];

	if (szRandomMem1)
	{
		delete[] szRandomMem1;
		szRandomMem1 = NULL;
	}

	if (szRandomMem2)
	{
		delete[] szRandomMem2;
		szRandomMem2 = NULL;
	}

	if (szRandomMem3)
	{
		delete[] szRandomMem3;
		szRandomMem3 = NULL;
	}
	//_ASSERT(SubWorld && Player && Npc && Item && KObject && Missle);
	//---------------------------------------------------------------------------
	g_InitProtocol();
	g_RandomSeed(42);	                               //����һ���������	clock()
	//g_SoundCache.Init(256);

	//g_SubWorldSet.m_cMusic.Init();//VS 2010 �д���

	g_InitSeries();

	if (!g_InitMath())
		messageBox("load InitMath Error","InitMath");
	//return;
	ItemSet.Init();
	ItemGen.Init();
	//InitAdjustColorTab();                            //VS 2010 �д���״̬��ɫƫ��  �� ��״̬����ɫ����״̬����ɫ//����ƫɫ��......

	g_MagicDesc.Init();                              //��ʼ���ͻ���ħ����������
	g_ItemChangeRes.Init();                             //VS 2010 �д��� ��ʼ��װ������ļ�
	//if(!g_ForbitWaiGua.Load(GAME_FORBITWAIGUA_FILE))  //VS 2010 �д���
	//	messageBox("load ForbitMap Error","ForbitMap");

	if(!g_ForbitMap.Load(GAME_FORBITMAP_FILE))
		messageBox("load ForbitMap Error","ForbitMap");
	if(!g_FsJinMai.Load(GAME_FSJINMAI_FILE))
		messageBox("load JinMai Error","JinMai");
	//return;
	g_PlayerTitle.Load("\\Settings\\playertitle.txt");
	g_NpcMapDropRate.Load(MAP_DROPRATE_FILE);
	NpcSet.m_cGoldTemplate.Init();                       //�ƽ����ģ���ʼ��
	NpcSet.Init();
	ObjSet.Init();
	MissleSet.Init();

	g_IniScriptEngine("\\Ui",FALSE);                      //J���ؿͻ��˽ű�
	if (!g_OrdinSkillsSetting.Load(SKILL_SETTING_FILE))   //��ʼ�����ܻ�������
		messageBox("load skills error.","skills");

	g_MisslesSetting.Load(MISSLES_SETTING_FILE);     //�ӵ���ʼ��  �Ӽ���

	g_NpcSetting.Load(NPC_SETTING_FILE);             //����NPCģ��
	InitSkillSetting();                              //����   //��ʼ�����ܻ�������

	InitMissleSetting();                             //�ӵ�

	InitNpcSetting();                                //npcģ��
	InitTaskSetting();                               //����
	// ����漰���뼼����صĶ��������Ա�����ڼ��ܳ�ʼ��֮��
	if (!PlayerSet.Init())
	   messageBox("Init PlayerSet Error","PlayerSet");

	g_CompAtlas.Load(GAME_ATLAS_FILE);
	g_GetMsg.Load(GAME_MSG_FILE);
	g_CompEquip.Load(GAME_COMP_FILE);
	g_MapTraffic.Load("\\settings\\MapTraffic.ini");
	if (!g_ScenePlace.Initialize())//������ʼ��
		messageBox("Scene Load Error","Initialize");

	g_Faction.Init();
	memset(g_nMeleeWeaponSkill, 0, sizeof(g_nMeleeWeaponSkill));
	memset(g_nRangeWeaponSkill, 0, sizeof(g_nRangeWeaponSkill));
	KTabFile Weapon_PhysicsSkillIdFile;
	if (Weapon_PhysicsSkillIdFile.Load(WEAPON_PHYSICSSKILLFILE))
	{
		int nHeight = Weapon_PhysicsSkillIdFile.GetHeight() - 1;
		for (int i = 0; i < nHeight; ++i)
		{
			int nDetail = 0,nParticular  = 0,nPhysicsSkill = 0;
			Weapon_PhysicsSkillIdFile.GetInteger(i + 2,1, -1, &nDetail); //WEAPON_DETAILTYPE
			Weapon_PhysicsSkillIdFile.GetInteger(i + 2,2, -1, &nParticular);//WEAPON_PARTICULARTYPE
			Weapon_PhysicsSkillIdFile.GetInteger(i + 2,3, -1, &nPhysicsSkill); //WEAPON_SKILLID

			//��������
			if (nDetail == 0 )
			{
				if (nParticular >= 0 && nParticular < MAX_MELEEWEAPON_PARTICULARTYPE_NUM && nPhysicsSkill > 0 && nPhysicsSkill < MAX_SKILL)
					g_nMeleeWeaponSkill[nParticular] = nPhysicsSkill;
			}
			else if (nDetail == 1)
			{
				if (nParticular >= 0 && nParticular < MAX_RANGEWEAPON_PARTICULARTYPE_NUM && nPhysicsSkill > 0 && nPhysicsSkill < MAX_SKILL)
					g_nRangeWeaponSkill[nParticular] = nPhysicsSkill;
			}
			else if (nDetail == -1) //����
			{
				if (nPhysicsSkill > 0 && nPhysicsSkill < MAX_SKILL)
					g_nHandSkill = nPhysicsSkill;
			}
		}
	}
	else
		messageBox("PhysicsSkillIdFile load Error","PhysicsSkillIdFile");

	Weapon_PhysicsSkillIdFile.Clear();
	if (!BuySell.Init())  //�̵��ʼ��
		messageBox("Init BuySell Error","BuySell");

	//����һ���̴߳��� ��Ϸ������ѭ��
	//messageBox("Init Core is ok....","InitCore");
}

/*
KCore::KCore(void)
{

}*/
