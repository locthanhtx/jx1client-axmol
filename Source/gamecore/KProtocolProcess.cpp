#include "engine/KEngine.h"
#include "KCore.h"
#include "engine/Text.h"
#include "engine/KFilePath.h"
#include "engine/KTabFile.h"   //�����ӵ�
#include "CoreShell.h"
#include "KViewItem.h"
#include "KSellItem.h"
#include "KMagicDesc.h"
#include "KObjSet.h"
#include "KNpcSet.h"
#include "KPlayer.h"
#include "KPlayerSet.h"
#include "KPlayerTeam.h"
#include "KPlayerFaction.h"
#include "KNpc.h"
#include "KSubWorld.h"
#include "LuaFuns.h"
//#include "KProtocolProcess.h"
#include "KSkills.h"
#include "KItemSet.h"
#include "KBuySell.h"
#include "KSubWorldSet.h"
#include "gamescene/ObstacleDef.h"
#include "gamescene/KScenePlaceC.h"
#include "KMath.h"
#include "KTongProtocol.h"
#include "KLadder.h"
//#include "KNpcTemplate.h"
#ifdef __linux
#include <sys/time.h>
#endif
//#define WAIGUA_ZROC
//KSubWorldSet   *M_SubWorldSetB;
//KPlayerSet     *G_PlayerSet;
const char * g_MagicID2String(int nAttrib);
KProtocolProcess g_ProtocolProcess;
KProtocolProcess::KProtocolProcess()
{
	ZeroMemory(ProcessFunc, sizeof(ProcessFunc));
	ProcessFunc[s2c_login]         = NULL;
	ProcessFunc[s2c_logout]        = NULL;
	ProcessFunc[s2c_syncclientend] =&KProtocolProcess::SyncClientEnd;  //ͬ�����
	ProcessFunc[s2c_synonlineplayer] =&KProtocolProcess::SyncCurPlayer;//����ͬ����
	ProcessFunc[s2c_synccurplayerskill] =&KProtocolProcess::s2cSyncAllSkill;  //ͬ������
	ProcessFunc[s2c_synccurplayernormal] =&KProtocolProcess::SyncCurNormalData;  //ͬ�� ��������--����ѭ��
	ProcessFunc[s2c_newplayer]    = NULL;
	ProcessFunc[s2c_removeplayer] = NULL;
	ProcessFunc[s2c_syncworld] =&KProtocolProcess::SyncWorld;    //����ͼʱͬ��
	ProcessFunc[s2c_syncplayer] =&KProtocolProcess::SyncPlayer;  //һ��һ���� �ͻ���Ҫ��ͬ����NPC����
	ProcessFunc[s2c_syncplayermin] =&KProtocolProcess::SyncPlayerMin;////ʵʱͬ������
	ProcessFunc[s2c_syncnpc] =&KProtocolProcess::SyncNpc;       //�ͻ���Ҫ��ͬ����NPC����
	ProcessFunc[s2c_syncnpcmin] =&KProtocolProcess::SyncNpcMin; //ʵʱͬ��NPC
	ProcessFunc[s2c_syncnpcminplayer] =&KProtocolProcess::SyncNpcMinPlayer;//ʵʱͬ������
	ProcessFunc[s2c_objadd] =&KProtocolProcess::SyncObjectAdd;
	ProcessFunc[s2c_syncobjstate] =&KProtocolProcess::SyncObjectState;
	ProcessFunc[s2c_syncobjdir] =&KProtocolProcess::SyncObjectDir;
	ProcessFunc[s2c_objremove] =&KProtocolProcess::SyncObjectRemove;
	ProcessFunc[s2c_objTrapAct] =&KProtocolProcess::SyncObjectTrap;
	ProcessFunc[s2c_npcremove] =&KProtocolProcess::NetCommandRemoveNpc;
	ProcessFunc[s2c_npcwalk] =&KProtocolProcess::NetCommandWalk;
	ProcessFunc[s2c_npcrun] =&KProtocolProcess::NetCommandRun;
	ProcessFunc[s2c_npcattack] = NULL;
	ProcessFunc[s2c_npcmagic] = NULL;
	ProcessFunc[s2c_npcjump] =&KProtocolProcess::NetCommandJump;
	ProcessFunc[s2c_npctalk] =&KProtocolProcess::NetCommandChat;
	ProcessFunc[s2c_npchurt] =&KProtocolProcess::NetCommandHurt;
	ProcessFunc[s2c_npcdeath] =&KProtocolProcess::NetCommandDeath;
	ProcessFunc[s2c_npcchgcurcamp] =&KProtocolProcess::NetCommandChgCurCamp;
	ProcessFunc[s2c_npcchgcamp] =&KProtocolProcess::NetCommandChgCamp;
	ProcessFunc[s2c_skillcast] =&KProtocolProcess::NetCommandSkill;
	ProcessFunc[s2c_playertalk] = NULL;
	ProcessFunc[s2c_playerexp] =&KProtocolProcess::s2cPlayerExp;
	ProcessFunc[s2c_teaminfo] =&KProtocolProcess::s2cShowTeamInfo;
	ProcessFunc[s2c_teamselfinfo] =&KProtocolProcess::s2cUpdataSelfTeamInfo;
	ProcessFunc[s2c_teamapplyinfofalse] =&KProtocolProcess::s2cApplyTeamInfoFalse;
	ProcessFunc[s2c_teamcreatesuccess] =&KProtocolProcess::s2cCreateTeam;
	ProcessFunc[s2c_teamcreatefalse] =&KProtocolProcess::s2cApplyCreateTeamFalse;
	ProcessFunc[s2c_teamopenclose] =&KProtocolProcess::s2cSetTeamState;
	ProcessFunc[s2c_teamgetapply] =&KProtocolProcess::s2cApplyAddTeam;
	ProcessFunc[s2c_teamaddmember] =&KProtocolProcess::s2cTeamAddMember;
	ProcessFunc[s2c_teamleave] =&KProtocolProcess::s2cLeaveTeam;
	ProcessFunc[s2c_teamchangecaptain] =&KProtocolProcess::s2cTeamChangeCaptain;
	ProcessFunc[s2c_playerfactiondata] =&KProtocolProcess::s2cSetFactionData;
	ProcessFunc[s2c_playerleavefaction] =&KProtocolProcess::s2cLeaveFaction;
	ProcessFunc[s2c_playerfactionskilllevel] =&KProtocolProcess::s2cFactionSkillOpen;
	ProcessFunc[s2c_playersendchat] =&KProtocolProcess::s2cGetChat;
	ProcessFunc[s2c_playersyncleadexp] =&KProtocolProcess::s2cGetLeadExp;
	ProcessFunc[s2c_playerlevelup] =&KProtocolProcess::s2cLevelUp;
	ProcessFunc[s2c_teammatelevel] =&KProtocolProcess::s2cGetTeammateLevel;
	ProcessFunc[s2c_playersyncattribute] =&KProtocolProcess::s2cGetCurAttribute;
	ProcessFunc[s2c_playerskilllevel] =&KProtocolProcess::s2cGetSkillLevel;
	ProcessFunc[s2c_syncitem] =&KProtocolProcess::s2cSyncItem;
	ProcessFunc[s2c_removeitem] =&KProtocolProcess::s2cRemoveItem;
	ProcessFunc[s2c_syncmonfs] =&KProtocolProcess::s2cSyncMoney;
	ProcessFunc[s2c_syncqianfs] =&KProtocolProcess::s2cSyncXu;//xu
	ProcessFunc[s2c_playermoveitem] =&KProtocolProcess::s2cMoveItem;
	ProcessFunc[s2c_scriptaction] =&KProtocolProcess::SyncScriptAction;
	ProcessFunc[s2c_chatapplyaddfriend] =&KProtocolProcess::s2cChatGetApplyAddFriend;
	ProcessFunc[s2c_chataddfriend] =&KProtocolProcess::s2cChatAddFriend;
	ProcessFunc[s2c_chatrefusefriend] =&KProtocolProcess::s2cChatBeRefusedAddFriend;
	ProcessFunc[s2c_chataddfriendfail] =&KProtocolProcess::s2cChatAddFriendFail;
	ProcessFunc[s2c_chatloginfriendnoname] =&KProtocolProcess::s2cChatLoginFriendNoName;
	ProcessFunc[s2c_chatloginfriendname] =&KProtocolProcess::s2cChatLoginFriendName;
	ProcessFunc[s2c_chatonefrienddata] =&KProtocolProcess::s2cChatAddOneFriend;
	ProcessFunc[s2c_chatfriendonline] =&KProtocolProcess::s2cChatFriendOnline;
	ProcessFunc[s2c_chatdeletefriend] =&KProtocolProcess::s2cChatDeleteFriend;
	ProcessFunc[s2c_chatfriendoffline] =&KProtocolProcess::s2cChatFriendOffLine;
	ProcessFunc[s2c_syncrolelist] =&KProtocolProcess::s2cSyncRoleList;
	ProcessFunc[s2c_tradechangestate] =&KProtocolProcess::s2cTradeChangeState;
	ProcessFunc[s2c_npcsetmenustate] =&KProtocolProcess::s2cNpcSetMenuState;
	ProcessFunc[s2c_trademoneysync] =&KProtocolProcess::s2cTradeMoneySync;
	ProcessFunc[s2c_tradedecision] =&KProtocolProcess::s2cTradeDecision;
	ProcessFunc[s2c_chatscreensingleerror] =&KProtocolProcess::s2cChatScreenSingleError;
	ProcessFunc[s2c_syncnpcstate] =&KProtocolProcess::s2cUpdateNpcState;
	ProcessFunc[s2c_teaminviteadd] =&KProtocolProcess::s2cTeamInviteAdd;
	ProcessFunc[s2c_tradepressoksync] =&KProtocolProcess::s2cTradePressOkSync;
	ProcessFunc[s2c_ping] =&KProtocolProcess::s2cPing;
	ProcessFunc[s2c_npcsit] =&KProtocolProcess::NetCommandSit;
	ProcessFunc[s2c_opensalebox] =&KProtocolProcess::OpenSaleBox;
	ProcessFunc[s2c_castskilldirectly] =&KProtocolProcess::s2cDirectlyCastSkill;
	ProcessFunc[s2c_msgshow] =&KProtocolProcess::s2cShowMsg;
	ProcessFunc[s2c_syncstateeffect] =&KProtocolProcess::SyncStateEffect;
	ProcessFunc[s2c_openstorebox] =&KProtocolProcess::OpenStoreBox;
	ProcessFunc[s2c_openresetpass] =&KProtocolProcess::OpenResetPass;
	ProcessFunc[s2c_openstringbox] =&KProtocolProcess::OpenStringUI; //string
	ProcessFunc[s2c_opendataubox] =&KProtocolProcess::GiftDT;
	ProcessFunc[s2c_playerrevive] =&KProtocolProcess::PlayerRevive;
	ProcessFunc[s2c_requestnpcfail] =&KProtocolProcess::RequestNpcFail;
	ProcessFunc[s2c_tradeapplystart] =&KProtocolProcess::s2cTradeApplyStart;
	ProcessFunc[s2c_rolenewdelresponse] =NULL;
	ProcessFunc[s2c_ItemAutoMove] =&KProtocolProcess::s2cItemAutoMove;
	ProcessFunc[s2c_itemexchangefinish] =&KProtocolProcess::FinishedItemExchange;
	ProcessFunc[s2c_changeweather] =&KProtocolProcess::s2cChangeWeather;
	ProcessFunc[s2c_pksyncnormalflag] =&KProtocolProcess::s2cPKSyncNormalFlag;
	ProcessFunc[s2c_pksyncenmitystate] =&KProtocolProcess::s2cPKSyncEnmityState;
	ProcessFunc[s2c_pksyncexercisestate] =&KProtocolProcess::s2cPKSyncExerciseState;
	ProcessFunc[s2c_pksyncpkvalue] =&KProtocolProcess::s2cPKValueSync;
	ProcessFunc[s2c_reputesyncreputevalue] =&KProtocolProcess::s2cReputeValueSync;
	ProcessFunc[s2c_fuyuansyncfuyuanvalue] =&KProtocolProcess::s2cFuYuanValueSync;
	ProcessFunc[s2c_rebornsyncrebornvalue] =&KProtocolProcess::s2cReBornValueSync;
	ProcessFunc[s2c_openmarketbox] =&KProtocolProcess::OpenMarketBox;
	ProcessFunc[s2c_npcsleepmode] =&KProtocolProcess::NpcSleepSync;
	ProcessFunc[s2c_viewequip] =&KProtocolProcess::s2cViewEquip;
	ProcessFunc[s2c_ladderresult] =&KProtocolProcess::LadderResult;
	ProcessFunc[s2c_ladderlist] =&KProtocolProcess::LadderList;
	ProcessFunc[s2c_tongcreate] =&KProtocolProcess::s2cTongCreate;
	ProcessFunc[s2c_replyclientping] =&KProtocolProcess::ServerReplyClientPing;
	ProcessFunc[s2c_npcgoldchange] =&KProtocolProcess::s2cNpcGoldChange;
	ProcessFunc[s2c_itemdurabilitychange] =&KProtocolProcess::ItemChangeDurability;
	ProcessFunc[s2c_opentremble] =&KProtocolProcess::OpenTremble;
	ProcessFunc[s2c_rankname] =&KProtocolProcess::NetCommandSetRankFF;
	ProcessFunc[s2c_viewsellitem] =&KProtocolProcess::s2cViewSellItem;
	ProcessFunc[s2c_viewupdateitem] =&KProtocolProcess::s2cViewUpdateItem;
	ProcessFunc[s2c_playergetcount] =&KProtocolProcess::s2cGetCouunt;
	ProcessFunc[s2c_shopname] =&KProtocolProcess::s2cShopName;
	ProcessFunc[s2c_syncmasklock] =&KProtocolProcess::SyncMaskLock;	// mat na
	ProcessFunc[s2c_opengive] =&KProtocolProcess::SyncGive;
	ProcessFunc[s2c_npcsecmove] =&KProtocolProcess::SyncSecMov;
	ProcessFunc[s2c_pkvalue] =&KProtocolProcess::SyncPKValue;
	ProcessFunc[s2c_viewlianitem]=&KProtocolProcess::SyncViewlianitem; //����
	ProcessFunc[s2c_openwengang] =&KProtocolProcess::OpenWenGang;
	ProcessFunc[s2c_openstone] =&KProtocolProcess::Openstone;
	ProcessFunc[s2c_updateviewequip] =&KProtocolProcess::s2cViewUpdateEquip;
	ProcessFunc[s2c_gettongback] =&KProtocolProcess::s2cgettongback;
	ProcessFunc[s2c_syncequipitem] =&KProtocolProcess::s2csyncequipitem;
	ProcessFunc[s2c_taskcallback] =&KProtocolProcess::s2ctaskcallback;
	ProcessFunc[s2c_openjindutiao] =&KProtocolProcess::s2copenjindutiao;
	ProcessFunc[s2c_syncitemposition] =&KProtocolProcess::s2csyncitemposition;
	ProcessFunc[s2c_openduanzhao] =&KProtocolProcess::s2copenduanzhao;	//������װ
	ProcessFunc[s2c_syncurupdata] =&KProtocolProcess::s2csyncurupdata;	//ͬ����ǰ����
	ProcessFunc[s2c_JinMaicallback] =&KProtocolProcess::s2cJinMaicallback;	//ͬ����ǰ����
    ProcessFunc[s2c_openvipdazao] =&KProtocolProcess::s2copenvipdazao;
	ProcessFunc[s2c_synnpcinfocallback] =&KProtocolProcess::s2csynnpcinfocallback;
	ProcessFunc[s2c_pautoMovecallback] =&KProtocolProcess::s2cpautoMovecallback;
	ProcessFunc[s2c_plyerguzicallback] =&KProtocolProcess::s2cplyerguzicallback;
	ProcessFunc[s2c_syncplayermap] =&KProtocolProcess::s2csyncplayermap;
	ProcessFunc[s2c_synconestate] =&KProtocolProcess::s2csynconeclearstate;
	ProcessFunc[s2c_syncnodataeffect] =&KProtocolProcess::s2csyncnodataeffect;
	ProcessFunc[s2c_npcsetpos] =&KProtocolProcess::s2cnpcsetpos;
	ProcessFunc[s2c_playermissiondata] =&KProtocolProcess::s2cplayermissiondata;
	ProcessFunc[s2c_syncrankdata] =&KProtocolProcess::s2csyncrankdata;
	ProcessFunc[s2c_syncsupershop] = &KProtocolProcess::s2cSyncSuperShop;
	ProcessFunc[s2c_syncitem_shop] = &KProtocolProcess::s2csyncitemshop;
	ProcessFunc[s2c_itemdata_shop] = &KProtocolProcess::s2citemdatashop;
	ProcessFunc[s2c_kickoutgame] = &KProtocolProcess::s2ckickoutgame;
	ProcessFunc[s2c_objadd_vn] = &KProtocolProcess::s2cobjadd_vn;
	ProcessFunc[s2c_rolesyncvipvalue] = &KProtocolProcess::s2crolesyncvipvalue;

	//ProcessFunc[s2c_rolecheckname_result] =NULL;
	ProcessFunc[s2c_extend] =&KProtocolProcess::s2cExtend;
	ProcessFunc[s2c_extendchat]   =&KProtocolProcess::s2cExtendChat;
	ProcessFunc[s2c_extendfriend] =&KProtocolProcess::s2cExtendFriend;
	ProcessFunc[s2c_extendtong]   =&KProtocolProcess::s2cExtendTong;
}

KProtocolProcess::~KProtocolProcess()
{
}

void KProtocolProcess::ProcessNetMsg(BYTE* pMsg)
{
	if (NULL==pMsg||NULL==this || !this || NULL==this ||NULL==ProcessFunc) return;
	if (pMsg[0] <= s2c_clientbegin || pMsg[0] >= s2c_end || NULL==ProcessFunc[pMsg[0]])
		return;
	//if (ProcessFunc[pMsg[0]])
	 (this->*ProcessFunc[pMsg[0]])(pMsg);
}

//ͬ���ƶ�
void	KProtocolProcess::SyncSecMov(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_JUMP_SYNC* pNetCommandJump = (NPC_JUMP_SYNC *)pMsg;
	DWORD dwNpcId = pNetCommandJump->ID;
	int nIdx = NpcSet.SearchID(dwNpcId);

	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{//�ͻ���ʹ�õ�
		Npc[nIdx].SendSerCommand(do_special4, pNetCommandJump->nMpsX, pNetCommandJump->nMpsY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

//֪ͨ�򿪸���UI

void	KProtocolProcess::SyncGive(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return;

	PLAYER_GIVE *pGive = (PLAYER_GIVE *)pMsg;

	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClearRoom(room_give);

    PLAYER_GIVEUI pGiveUI;
    sprintf(pGiveUI.m_szName,"%s",pGive->m_szName);
    sprintf(pGiveUI.m_Callback,"%s",pGive->m_Callback);
    sprintf(pGiveUI.m_Error,"%s",pGive->m_Error);
	sprintf(pGiveUI.m_szTitle,"%s",pGive->m_szTitle);
	CoreDataChanged(GDCNI_GIVE, (uintptr_t)&pGiveUI,pGive->m_nNum);  //֪ͨ��UI

}
//֪ͨ���ַ���UI
void KProtocolProcess::OpenStringUI(BYTE* pMsg) //stringbox
{
	if  (NULL==pMsg||NULL==this) return;
    PLAYER_STRING *pSring = (PLAYER_STRING *)pMsg;

	PLAYER_STRINGUI pStringUI;

    sprintf(pStringUI.m_szTitle,"%s",pSring->m_szTitle);
    sprintf(pStringUI.m_SCallback,"%s",pSring->m_SCallback);
    sprintf(pStringUI.m_SError,"%s",pSring->m_SError);
    pStringUI.m_nNum=pSring->m_nNum;
	CoreDataChanged(GDCNI_OPEN_STRING_BOX, (uintptr_t)&pStringUI, pSring->m_MaxLen);
}

void	KProtocolProcess::SyncPKValue(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CoreDataChanged(GDCNI_PKVALUE,NULL, NULL);
}
//����  �ͻ���
void	KProtocolProcess::SyncViewlianitem(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	if (!g_cSellItem.GetDataL(pMsg))
        Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("����:��������,����ϵGM����!");
}

/*
void	KProtocolProcess::s2cLianUpdateItem(BYTE* pMsg)
{
	g_cSellItem.GetDataL(pMsg,1);
}

*/
void	KProtocolProcess::s2cViewEquip(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
//	    Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("�鿴װ��:��������,����ϵGM����!");
	    g_cViewItem.GetData(pMsg);
}

void	KProtocolProcess::s2cgettongback(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
         PLAYER_TONGINFO_CALLBACK *nTongBack=(PLAYER_TONGINFO_CALLBACK *)pMsg;
         if (nTongBack->inType==1)
		 {//���°�����ú͹�����Ϣ
			 KClientZhaoMuInfo nTongInfo;
			 memset(&nTongInfo, 0, sizeof(nTongInfo));

             sprintf(nTongInfo.szQingXiang, "%s", nTongBack->szQingXiang);
			 sprintf(nTongInfo.szHuoDong1, "%s", nTongBack->szHuoDong1);
			 sprintf(nTongInfo.szHuoDong2, "%s", nTongBack->szHuoDong2);
			 sprintf(nTongInfo.szHuoDong3, "%s", nTongBack->szHuoDong3);
			 sprintf(nTongInfo.szHuoDong4, "%s", nTongBack->szHuoDong4);
			 sprintf(nTongInfo.szJiYu, "%s", nTongBack->szJiYu);
			 sprintf(nTongInfo.szGongGao, "%s", nTongBack->szGongGao);
			 nTongInfo.inType=nTongBack->inType;
             nTongInfo.inAcceptLevel=nTongBack->inAcceptLevel;
			 nTongInfo.inRefuseLevel=nTongBack->inRefuseLevel;

			 CoreDataChanged(GDCNI_GETTONG_INFO, (uintptr_t)&nTongInfo, 0);
             nTongBack=NULL;
		 }
		 else if (nTongBack->inType==5)
		 {//������������ɵ������б�
               KClientApplyList mTongApplyList;
               memset(&mTongApplyList, 0, sizeof(mTongApplyList));
			   for (int i=0;i<nTongBack->inAcceptLevel;i++)
			   {
                 sprintf(mTongApplyList.nInfo[i].szName, "%s", nTongBack->m_sMember[i].m_szName);
			     mTongApplyList.nInfo[i].inLevel = nTongBack->m_sMember[i].m_inLevel;
				 mTongApplyList.inType           = nTongBack->inType;
				 mTongApplyList.nCurNum          = nTongBack->inAcceptLevel;     //��ǰ������
				 mTongApplyList.nStartPos        = nTongBack->inRefuseLevel;     //��ʼ��ȡ��λ��
			   }
            //  Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("��ȡ��������б�ɹ�!");
			   CoreDataChanged(GDCNI_GETTONG_INFO, (uintptr_t)&mTongApplyList, nTongBack->inType);
		 }

}

void	KProtocolProcess::s2cViewUpdateEquip(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	    g_cViewItem.GetData(pMsg,1);
}

void	KProtocolProcess::s2cViewSellItem(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	g_cSellItem.GetData(pMsg);
}

void	KProtocolProcess::s2cViewUpdateItem(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	g_cSellItem.GetData(pMsg,1);
}

//��ȡ����
void	KProtocolProcess::s2cGetCouunt(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_GET_COUNT *pGetCount = (PLAYER_GET_COUNT *)pMsg;
	g_cSellItem.Check(pGetCount->nCount,pGetCount->dwId);
}

void	KProtocolProcess::s2cShopName(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_PLAYER_SHOPNAME *pShopname = (PLAYER_PLAYER_SHOPNAME *)pMsg;
	int nIndex = NpcSet.SearchID(pShopname->m_dwNpcID);

	//char nstrName[32]={0};
	if (nIndex>0)
	   Npc[nIndex].SetstrInfo(STR_SHOP_NAME,pShopname->m_Name);
	//strcpy(Npc[nIndex].ShopName,pShopname->m_Name);
}

//void KProtocolProcess::SyncInfo(BYTE* pMsg)
//{
//û�����

//}

// mat na
void KProtocolProcess::SyncMaskLock(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_SIT_SYNC *pInfo = (NPC_SIT_SYNC *)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_ItemList.SetMaskLock(pInfo->ID);
}

void KProtocolProcess::NpcSleepSync(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_SLEEP_SYNC*	pSync = (NPC_SLEEP_SYNC *)pMsg;
	DWORD	dwNpcId = pSync->NpcID;

	int nIdx = NpcSet.SearchID(dwNpcId);

	if (nIdx > 0)
	{
		Npc[nIdx].SetSleepMode((BOOL)pSync->bSleep);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

//�ͻ���
void KProtocolProcess::s2cPing(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
//	DWORD	dwTimer = GetTickCount();
//	PING_COMMAND* PingCmd = (PING_COMMAND *)pMsg;
//
//	dwTimer -= PingCmd->m_dwTime;
//	dwTimer >>= 1;
//	g_SubWorldSet.SetPing(dwTimer);
//	g_bPingReply = TRUE;
	DWORD	dwTimer;
#ifdef WIN32
	dwTimer = GetTickCount(); //��ȡϵͳ���е�ʱ��
#else
	timeval  m_pStartUpdate;
	gettimeofday(&m_pStartUpdate,NULL);
	dwTimer = m_pStartUpdate.tv_sec*1000+m_pStartUpdate.tv_usec/1000;
#endif

	PING_COMMAND*	PingCmd = (PING_COMMAND *)pMsg;

	/*if  (curMobileVer!=PingCmd->m_serVer)
	{
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("app�汾����,�����app!");
		return;
	}*/


	PING_CLIENTREPLY_COMMAND	pcc;
	pcc.ProtocolType = c2s_ping;
	pcc.m_dwReplyServerTime = PingCmd->m_dwTime;  //���ظ��������� У��ʱ��
	pcc.m_dwClientTime      = dwTimer;            //��ǰ�ͻ��˵�ʱ��
	if (g_pClient)
	    g_pClient->SendPackToServer(&pcc, sizeof(PING_CLIENTREPLY_COMMAND));
	//g_SubWorldSet.SetPing(PingCmd->m_dwTime);
}

void KProtocolProcess::ServerReplyClientPing(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	DWORD	dwTimer;
#ifdef WIN32
	dwTimer = GetTickCount(); //��ȡϵͳ���е�ʱ��
#else
	timeval  m_pStartUpdate;
	gettimeofday(&m_pStartUpdate,NULL);
	dwTimer = m_pStartUpdate.tv_sec*1000+m_pStartUpdate.tv_usec/1000;
#endif
	//DWORD	dwTimer = GetTickCount();
	PING_COMMAND* pPc = (PING_COMMAND *)pMsg;
	if (g_GameWorld)
		g_GameWorld->setPingTime(pPc->m_dwTime);

	dwTimer -= pPc->m_dwTime;
	dwTimer >>= 1;
	g_SubWorldSet.SetPing(dwTimer);  //�趨�ϴ� �ͷ�����ͨ����ʱ��
}

void KProtocolProcess::s2cPlayerExp(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_EXP_SYNC	*pExp = (PLAYER_EXP_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].s2cSetExp(pExp->m_nExp);
}

void	KProtocolProcess::s2cChatScreenSingleError(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CHAT_SCREENSINGLE_ERROR_SYNC	*pError = (CHAT_SCREENSINGLE_ERROR_SYNC*)pMsg;

	// ֪ͨ������Ļ���Ĳ��ɹ�
	/*char	szName[32];
	memset(szName, 0, sizeof(szName));
	memcpy(szName, pError->m_szName, pError->m_wLength + 1 + sizeof(pError->m_szName) - sizeof(CHAT_SCREENSINGLE_ERROR_SYNC));

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, strCoreInfo[MSG_CHAT_TAR_REFUSE_SINGLE_TALK].c_str(), szName);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);*/
}

void KProtocolProcess::NetCommandChgCamp(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	DWORD	dwNpcId;

	NPC_CHGCAMP_SYNC *nComp = (NPC_CHGCAMP_SYNC *)pMsg;
	//dwNpcId = *(DWORD *)&pMsg[1];
	dwNpcId  = nComp->ID;
	int nIdx = NpcSet.SearchID(dwNpcId);

	if (nIdx > 0)
	{
		Npc[nIdx].m_Camp = (int)nComp->Camp;//(int)pMsg[5];
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandChgCurCamp(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	DWORD	dwNpcId;
	NPC_CHGCURCAMP_SYNC	*NetCommand = (NPC_CHGCURCAMP_SYNC *)pMsg;
	dwNpcId = NetCommand->ID;//*(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcId);

	if (nIdx > 0)
	{
		Npc[nIdx].m_CurrentCamp = (int)NetCommand->Camp;//pMsg[5];
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}
//�ͻ���
void KProtocolProcess::NetCommandDeath(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_DEATH_SYNC	*pDeath = (NPC_DEATH_SYNC*)pMsg;

	int nIdx = NpcSet.SearchID(pDeath->ID);
    //int nIdxR= NpcSet.SearchName(pDeath->nDeathName);  //�����ߵ�����
	if (nIdx > 0)
	{
		Npc[nIdx].ProcNetCommand(do_death);
		Npc[nIdx].m_CurrentLife = 0;
		Npc[nIdx].m_CurrentNuQi = 0;
	    Npc[nIdx].m_Height      = 0;  //�߶�Ϊ��
		Npc[nIdx].m_SyncSignal  = SubWorld[0].m_dwCurrentTime;
	}

}

void KProtocolProcess::NetCommandChat(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_CHAT_SYNC *pSync = (NPC_CHAT_SYNC*)pMsg;

	int nIdx = NpcSet.SearchID(pSync->ID);

	if (nIdx > 0)
	{
		//pSync->nMsgLen = TEncodeText(pSync->szMsg, strlen(pSync->szMsg));
		//Npc[nIdx].SetChatInfo(Npc[nIdx].Name, pSync->szMsg, pSync->nMsgLen);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandJump(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_JUMP_SYNC* pNetCommandJump = (NPC_JUMP_SYNC *)pMsg;
	DWORD dwNpcId = pNetCommandJump->ID;
	int nIdx = NpcSet.SearchID(dwNpcId);

	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		Npc[nIdx].SendSerCommand(do_jump, pNetCommandJump->nMpsX, pNetCommandJump->nMpsY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandHurt(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_HURT_SYNC*	pSync = (NPC_HURT_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pSync->ID);
	if (nIdx > 0)
	{
		//Npc[nIdx].SendCommand(do_hurt, pSync->nFrames, pSync->nX, pSync->nY);
		Npc[nIdx].ProcNetCommand(do_hurt, pSync->nFrames, pSync->nX, pSync->nY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}
//ͬ��ɾ���ͻ���NPC
void KProtocolProcess::NetCommandRemoveNpc(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	DWORD	dwNpcID;
	NPC_REMOVE_SYNC	*RemoveSync = (NPC_REMOVE_SYNC*)pMsg;;
	dwNpcID = RemoveSync->ID;//*(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcID);

	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{//������ǿͻ��˱���
		if (Npc[nIdx].m_RegionIndex >= 0)
		{
			if(RemoveSync->Rv)//(*(BOOL *)&pMsg[5])
				SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].RemoveNpc(nIdx);
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecNpcRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY);
		}
		NpcSet.Remove(nIdx);
	}
	else
	{//����Ǳ��� ��ɾ�����еķ��Ӻ� ��ľ�ľ���
			if (g_GameWorld)
				g_GameWorld->FreeAllSprite();
			//g_ScenePlace.ClearProcessArea();
	}
}

void KProtocolProcess::NetCommandRun(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	DWORD	dwNpcID;
	DWORD	MapX, MapY;
	NPC_RUN_SYNC *NetCommand = (NPC_RUN_SYNC *)pMsg;
	dwNpcID = NetCommand->ID;//*(DWORD *)&pMsg[1];
	MapX = NetCommand->nMpsX;//*(int *)&pMsg[5];
	MapY = NetCommand->nMpsY;//*(int *)&pMsg[9];

	int nIdx = NpcSet.SearchID(dwNpcID);
	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))  //�����ڿͻ���
	{
		Npc[nIdx].SendSerCommand(do_run, MapX, MapY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

void KProtocolProcess::NetCommandSit(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_SIT_SYNC*	pSitSync;

	pSitSync = (NPC_SIT_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pSitSync->ID);

	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		Npc[nIdx].SendSerCommand(do_sit);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}


//֪ͨ�ͻ����˳���Ϸ
void 	KProtocolProcess::s2ckickoutgame(BYTE* pMsg)
{
	    if  (NULL==pMsg||NULL==this) return;

		KICK_OUT_GAME_COMMAND* pOut = (KICK_OUT_GAME_COMMAND*)pMsg;
		if (pOut->m_isout && g_GameWorld)
           g_GameWorld->KickOutGame();
}

void 	KProtocolProcess::s2citemdatashop(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	//BuySell.SetShopItemdata(pMsg);
}


void 	KProtocolProcess::s2csyncitemshop(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	//S2C_SUPERSHOP* pInfo = (S2C_SUPERSHOP*)pMsg;
	//BuySell.OpenCSuPerShop(pInfo->m_nSaleType, (BuySellInfo*)&pInfo->m_BuySellInfo);
	BuySell.SetShopItemInfo(pMsg);
}


//�ͻ���
void 	KProtocolProcess::s2cSyncSuperShop(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	S2C_SUPERSHOP* pInfo = (S2C_SUPERSHOP*)pMsg;
	//BuySell.OpenCSuPerShop(pInfo->m_nSaleType, (BuySellInfo*)&pInfo->m_BuySellInfo);
}


void KProtocolProcess::OpenSaleBox(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	SALE_BOX_SYNC* pSale = (SALE_BOX_SYNC *)pMsg;

	BuySell.OpenCSale(pSale->nShopIndex);
}

void KProtocolProcess::OpenStoreBox(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CoreDataChanged(GDCNI_OPEN_STORE_BOX,NULL,NULL);
}

void KProtocolProcess::OpenResetPass(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CoreDataChanged(GDCNI_OPEN_RESET_PASS, NULL, NULL);
}

void KProtocolProcess::GiftDT(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CoreDataChanged(GDCNI_OPEN_DATAU_BOX, NULL, NULL);
}
//�������  �ͻ���
void KProtocolProcess::PlayerRevive(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_REVIVE_SYNC* pSync = (NPC_REVIVE_SYNC*)pMsg;

	int nIdx = NpcSet.SearchID(pSync->ID);
	if (nIdx > 0 && nIdx<MAX_NPC)
	{
		if (!Npc[nIdx].IsPlayer() && (pSync->mType == 0 || pSync->mType == 1))
		{//������� �� ͬ�� ɾ���ͻ���NPC
			//SubWorld[0].m_WorldMessage.NewSend(GWM_NPC_DEL, nIdx);
			if (Npc[nIdx].m_RegionIndex >= 0)
			{
				int nSubWorld = Npc[nIdx].m_SubWorldIndex;
				int nRegion = Npc[nIdx].m_RegionIndex;

				if (nSubWorld>=0 && nRegion>=0)
				{
					SubWorld[nSubWorld].m_Region[nRegion].RemoveNpc(nIdx);
					SubWorld[nSubWorld].m_Region[nRegion].DecNpcRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY);
				}

			}
		    NpcSet.Remove(nIdx);
			//messageBox("ɾ����npc","PlayerRevive");
			return;
		}
		else
		{//�Ǳ��� �Ϳ�ʼ����
			Npc[nIdx].ProcNetCommand(do_revive);
		}
	}
}

void KProtocolProcess::RequestNpcFail(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_REQUEST_FAIL* pNpcSync = (NPC_REQUEST_FAIL *)pMsg;

	if (NpcSet.IsNpcRequestExist(pNpcSync->ID))
		NpcSet.RemoveNpcRequest(pNpcSync->ID);
}
//�ͻ���  ����ͬ��
void KProtocolProcess::NetCommandSkill(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	/*DWORD	dwNpcID;
	int		nSkillID, nSkillLevel, nSkillEnChance;
	int		MapX, MapY;

	dwNpcID = *(DWORD *)&pMsg[1];
	nSkillID = *(int *)&pMsg[5];
	nSkillLevel = *(int *)&pMsg[9];
	MapX = *(int *)&pMsg[13];
	MapY = *(int *)&pMsg[17];
	nSkillEnChance = *(int *)&pMsg[21];
	*/
	DWORD	dwNpcID;
	int		nSkillID, nSkillLevel, nSkillEnChance,nWaitTimer,mIsEnChance,mMaxBeiLv=1;
	int 	MapX, MapY; //DWORD
	NPC_SKILL_SYNC	*pPress = (NPC_SKILL_SYNC*)pMsg;

	dwNpcID      = pPress->ID;
	nSkillID     = pPress->nSkillID;          //�Ӽ���
	nSkillLevel  = pPress->nSkillLevel;
	MapX         = pPress->nMpsX;
	MapY         = pPress->nMpsY;
	nSkillEnChance = pPress->nSkillEnChance;  //���ܼӳ�
	nWaitTimer   = pPress->nWaitTime;
	mIsEnChance  = pPress->nIsEnChance;
	mMaxBeiLv    = pPress->nMaxBei;           // ��������

	//if (MapY < 0)
	//	return ;

	//��ָ��ĳ��Ŀ��ʱ(MapX == -1),MapYΪĿ���NpcdwID����Ҫת���ɱ��ص�NpcIndex����
	if (MapX < 0)
	{
		if (MapX != -1)
			return;

		MapY = NpcSet.SearchID(MapY);
		if (MapY == 0)
			return;

		if (Npc[MapY].m_RegionIndex < 0)
			return;

	}

	int nIdx = NpcSet.SearchID(dwNpcID);

	if (nIdx <= 0)
	{
		//messageBox("nIdx <= 0","NetCommandSkill");
		return;
	}
	//char msg[128]={0};
	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{//���ǿͻ��˱���ʹ�ü���

		Npc[nIdx].m_SkillList.SetSkillLevel(nSkillID, nSkillLevel);
		Npc[nIdx].m_SkillList.SetSkillEnChance(nSkillID, nSkillEnChance);   //���ÿͻ��˵ļ��ܼӳ�
		Npc[nIdx].SendSerCommand(do_skill, nSkillID, MapX, MapY);           //��ʼ����ʹ�ü���
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}

}

void KProtocolProcess::NetCommandWalk(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	DWORD	dwNpcID;
	DWORD	MapX, MapY;
	NPC_WALK_SYNC *NetCommand =(NPC_WALK_SYNC *)pMsg;
	dwNpcID = NetCommand->ID;//*(DWORD *)&pMsg[1];
	MapX = NetCommand->nMpsX;//*(int *)&pMsg[5];
	MapY =  NetCommand->nMpsY;//*(int *)&pMsg[9];
	int nIdx = NpcSet.SearchID(dwNpcID);
	if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))
	{
		Npc[nIdx].SendSerCommand(do_walk, MapX, MapY);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ���鴴��ʧ��
//-------------------------------------------------------------------------
void KProtocolProcess::s2cApplyCreateTeamFalse(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_SEND_CREATE_TEAM_FALSE *pCreateFalse = (PLAYER_SEND_CREATE_TEAM_FALSE*)pMsg;
//	KSystemMessage	sMsg;

	switch (pCreateFalse->m_btErrorID)
	{
	// �Ѿ��ڶ����У�˵���ͻ��˶��������д����������»�ö�������
	case Team_Create_Error_InTeam:	//Team_Creae_Error_InTeam
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		break;

	// ��ǰ���ڲ������״̬
	case Team_Create_Error_CannotCreate:
		/*sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_CANNOT_CREATE].c_str());
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);*/
		break;


	default:
		/*sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_CREATE_FAIL].c_str());
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);*/
		break;
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ��ѯĳ������������ʧ��
//-------------------------------------------------------------------------
void KProtocolProcess::s2cApplyTeamInfoFalse(BYTE* pMsg)
{
	// ������ʾ�����ѯʧ��(not end)
}

//-------------------------------------------------------------------------
//	���ܣ����һ������
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatAddFriend(BYTE* pMsg)
{
	return;
	if  (NULL==pMsg||NULL==this) return;
	CHAT_ADD_FRIEND_SYNC	*pFriend = (CHAT_ADD_FRIEND_SYNC*)pMsg;
	//Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pFriend->m_dwID, pFriend->m_nIdx, pFriend->m_szName, 0);
	//Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	KSystemMessage	sMsg;
	KUiPlayerItem	sPlayer;

	memset(&sPlayer, 0, sizeof(KUiPlayerItem));
	strcpy(sPlayer.Name, pFriend->m_szName);
	sPlayer.uId = pFriend->m_dwID;
	sPlayer.nIndex = pFriend->m_nIdx;

	sprintf(sMsg.szMessage, MSG_CHAT_ADD_FRIEND_SUCCESS, pFriend->m_szName);
	sMsg.eType = SMT_FRIEND;
	sMsg.byConfirmType = SMCT_UI_INTERVIEW;
	sMsg.byPriority = 2;
	sMsg.byParamSize = sizeof(KUiPlayerItem);
	sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, (intptr_t)&sPlayer);
}

//-------------------------------------------------------------------------
//	���ܣ���Ӻ���ʧ��
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatAddFriendFail(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CHAT_ADD_FRIEND_FAIL_SYNC	*pFail = (CHAT_ADD_FRIEND_FAIL_SYNC*)pMsg;

	/*char	szName[32];
	CChatApplyListNode	*pList;
	pList = (CChatApplyListNode*)Player[CLIENT_PLAYER_INDEX].m_cChat.m_cApplyAddList.GetHead();
	while (pList)
	{
		if (pList->m_nPlayerIdx == pFail->m_nTargetPlayerIdx)
		{
			g_StrCpy(szName, pList->m_szName);

			// ֪ͨ�������ĳ��Ϊ���Ѳ���ʧ��
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_CHAT_ADD_FRIEND_FAIL, szName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);

			return;
		}
		pList = (CChatApplyListNode*)pList->GetNext();
	}*/
}

//-------------------------------------------------------------------------
//	���ܣ��յ����������������ĳ��������ѵ����ݣ������֣�
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatAddOneFriend(BYTE* pMsg)
{

	/*
	if  (NULL==pMsg||NULL==this) return;
	CHAT_ONE_FRIEND_DATA_SYNC	*pData = (CHAT_ONE_FRIEND_DATA_SYNC*)pMsg;
	int		nTeamNo;

	nTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.CheckTeamNo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, pData->m_szName);
	if (nTeamNo < 0)
		nTeamNo = 0;
	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pData->m_dwID, pData->m_nPlayerIdx, pData->m_szName, nTeamNo);
	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
	*/
	// ֪ͨ����õ��³�Ա not end
}

//-------------------------------------------------------------------------
//	���ܣ��������Ҿܾ�����
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatBeRefusedAddFriend(BYTE* pMsg)
{
	return;//ȡ��
	if  (NULL==pMsg||NULL==this) return;
	CHAT_REFUSE_FRIEND_SYNC	*pRefuse = (CHAT_REFUSE_FRIEND_SYNC*)pMsg;
	char	szName[64];

	memset(szName, 0, sizeof(szName));
	memcpy(szName, pRefuse->m_szName, pRefuse->m_wLength + 1 + sizeof(pRefuse->m_szName) - sizeof(CHAT_REFUSE_FRIEND_SYNC));

	// ֪ͨ����pRefuse->m_szName�ܾ������Ľ�������
	KSystemMessage	sMsg;

	sprintf(sMsg.szMessage, MSG_CHAT_REFUSE_FRIEND, szName);
	sMsg.eType = SMT_FRIEND;
	sMsg.byConfirmType = SMCT_CLICK;
	sMsg.byPriority = 1;
	sMsg.byParamSize = 0;
	sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
}

//-------------------------------------------------------------------------
//	���ܣ��õ�֪ͨ��������
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatFriendOnline(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;

	CHAT_FRIEND_ONLINE_SYNC	*pFriend = (CHAT_FRIEND_ONLINE_SYNC*)pMsg;

	Player[CLIENT_PLAYER_INDEX].ChatFriendOnLine(pFriend->m_dwID, pFriend->m_nPlayerIdx);
}

//-------------------------------------------------------------------------
// �յ�������ת���ı������������ѵ�����
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatGetApplyAddFriend(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CHAT_APPLY_ADD_FRIEND_SYNC	*pApply = (CHAT_APPLY_ADD_FRIEND_SYNC*)pMsg;

	/*if ( Player[CLIENT_PLAYER_INDEX].m_cChat.CheckIsFriend(pApply->m_nSrcPlayerIdx) )
		return;

	CChatApplyListNode	*pNode = NULL;
	pNode = new CChatApplyListNode;
	if (pNode == NULL)
		return;
	pNode->m_nPlayerIdx = pApply->m_nSrcPlayerIdx;
	strcpy(pNode->m_szName, pApply->m_szSourceName);
	strcpy(pNode->m_szInfo, pApply->m_szInfo);

	Player[CLIENT_PLAYER_INDEX].m_cChat.m_cApplyAddList.AddTail(pNode);

	// ֪ͨ����������������������
	KSystemMessage	sMsg;
	KUiPlayerItem	sPlayer;

	strcpy(sPlayer.Name, pApply->m_szSourceName);
	sPlayer.nIndex = pApply->m_nSrcPlayerIdx;
	sPlayer.uId = 0;
	sPlayer.nData = 0;

	sprintf(sMsg.szMessage, MSG_CHAT_GET_FRIEND_APPLY, pNode->m_szName);
	sMsg.eType = SMT_FRIEND;
	sMsg.byConfirmType = SMCT_UI_FRIEND_INVITE;
	sMsg.byPriority = 3;
	sMsg.byParamSize = sizeof(KUiPlayerItem);
	sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, (intptr_t)&sPlayer);
	*/
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������������ҵ�¼ʱ�������������ݣ������֣�
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatLoginFriendName(BYTE* pMsg)
{
	/*
	if  (NULL==pMsg||NULL==this) return;
	CHAT_LOGIN_FRIEND_NAME_SYNC	*pFriend = (CHAT_LOGIN_FRIEND_NAME_SYNC*)pMsg;
	int		nTeamNo;
	char	szName[64];

	memset(szName, 0, sizeof(szName));
	memcpy(szName, pFriend->m_szName, pFriend->m_wLength + 1 + sizeof(pFriend->m_szName) - sizeof(CHAT_LOGIN_FRIEND_NAME_SYNC));
	nTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.CheckTeamNo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, szName);
	if (nTeamNo < 0)
		nTeamNo = 0;
	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pFriend->m_dwID, pFriend->m_nPlayerIdx, szName, nTeamNo);
	*/
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������������ҵ�¼ʱ�������������ݣ��������֣�
//-------------------------------------------------------------------------
void KProtocolProcess::s2cChatLoginFriendNoName(BYTE* pMsg)
{
	/*
	if  (NULL==pMsg||NULL==this) return;
	CHAT_LOGIN_FRIEND_NONAME_SYNC	*pFriend = (CHAT_LOGIN_FRIEND_NONAME_SYNC*)pMsg;
	char	szName[32];
	int		nTeamNo;

	szName[0] = 0;
	nTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.CheckTeamNo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, NULL);
	if (nTeamNo < 0)
	{
		if (Player[CLIENT_PLAYER_INDEX].m_cChat.m_nLoginGetFirstOneFriendFlag == 0)		// �������·������к������ݣ�������
		{
			Player[CLIENT_PLAYER_INDEX].m_cChat.m_nLoginGetFirstOneFriendFlag = 1;

			CHAT_APPLY_RESEND_ALL_FRIEND_NAME_COMMAND	sApply;
			sApply.ProtocolType = c2s_chatapplyresendallfriendname;

//			if (g_pClient)
	//			g_pClient->SendPackToServer((BYTE*)&sApply, sizeof(CHAT_APPLY_RESEND_ALL_FRIEND_NAME_COMMAND));
		}
		else												// �������·�������������ݣ�������
		{
			CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND	sApply;
			sApply.ProtocolType = c2s_chatapplysendonefriendname;
			sApply.m_dwID = pFriend->m_dwID;

			//if (g_pClient)
				//g_pClient->SendPackToServer((BYTE*)&sApply, sizeof(CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND));
		}
		return;
	}
	Player[CLIENT_PLAYER_INDEX].m_cChat.AddOne(pFriend->m_dwID, pFriend->m_nPlayerIdx, szName, nTeamNo);
	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
	Player[CLIENT_PLAYER_INDEX].m_cChat.m_nLoginGetFirstOneFriendFlag = 1;
	*/

}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ����һ֧����
//-------------------------------------------------------------------------
void KProtocolProcess::s2cCreateTeam(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_SEND_CREATE_TEAM_SUCCESS	*pCreateSuccess = (PLAYER_SEND_CREATE_TEAM_SUCCESS*)pMsg;

	g_Team[0].CreateTeam(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Level, pCreateSuccess->nTeamServerID);
	Player[CLIENT_PLAYER_INDEX].m_cTeam.Release();
	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nApplyCaptainID = 0;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].RestoreCurrentCamp();

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, "%s",  strCoreInfo[MSG_TEAM_CREATE].c_str());
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);

	sprintf(sMsg.szMessage, "%s", strCoreInfo[MSG_TEAM_CREATE].c_str());
	sMsg.eType = SMT_TEAM;
	sMsg.byConfirmType = SMCT_CLICK;
	sMsg.byPriority = 1;
	sMsg.byParamSize = 0;
	sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetInstantSpr(enumINSTANT_STATE_CREATE_TEAM); ////�ͷ�һ����ӵ�˲����Ч
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ���ŵ�ǰ���ɼ���ĳ�ȼ�
//-------------------------------------------------------------------------
void KProtocolProcess::s2cFactionSkillOpen(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_FACTION_SKILL_LEVEL	*pSkill = (PLAYER_FACTION_SKILL_LEVEL*)pMsg;

	if (Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction < 0 || Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction != pSkill->m_btCurFactionID)
	{
		// �������������Ϣ
		Player[CLIENT_PLAYER_INDEX].ApplyFactionData();
		return;
	}

//	for (int i = 0; i < FACTIONS_PRR_SERIES; i++)
//	{
//		if (Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[i].m_nID == Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction)
//		{
//			Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[i].m_nOpenLevel = pSkill->m_btLevel;
//			Player[CLIENT_PLAYER_INDEX].m_cFaction.OpenCurSkillLevel(Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[i].m_nOpenLevel, &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList);
//			break;
//		}
//	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������������������Ϣ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cGetChat(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_SEND_CHAT_SYNC	*pChat = (PLAYER_SEND_CHAT_SYNC*)pMsg;

//	Player[CLIENT_PLAYER_INDEX].m_cChat.GetChat(pChat);
}

//-------------------------------------------------------------------------
//	���ܣ��յ��������������ĵ�ǰĳ���Եĵ���
//-------------------------------------------------------------------------
void KProtocolProcess::s2cGetCurAttribute(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].s2cGetCurAttribute(pMsg);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������������ͳ��������ֵ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cGetLeadExp(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	DWORD	dwLevel = Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel;

	PLAYER_LEAD_EXP_SYNC	*pLeadExp = (PLAYER_LEAD_EXP_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadExp = pLeadExp->m_dwLeadExp;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(Player[CLIENT_PLAYER_INDEX].m_dwLeadExp, Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);
	Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);

	if (dwLevel < Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel)
	{
		/*KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, strCoreInfo[MSG_LEADER_LEVEL_UP].c_str(), Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);
		sMsg.eType = SMT_PLAYER;
		sMsg.byConfirmType = SMCT_CLICK;
		sMsg.byPriority = 1;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);*/
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������������ĳ���ܵ���
//-------------------------------------------------------------------------
void KProtocolProcess::s2cGetSkillLevel(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	//messageBox("���¼��ܡ�������","���¼���");
	int	nSkillIndex=0;
	PLAYER_SKILL_LEVEL_SYNC	*pSkill = (PLAYER_SKILL_LEVEL_SYNC*)pMsg;
	if (pSkill->m_nSkillLevel >=MAX_SKILLLEVEL)
		return;
	nSkillIndex = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(pSkill->m_nSkillID);
	Player[CLIENT_PLAYER_INDEX].m_nSkillPoint = pSkill->m_nLeavePoint;
	//messageBox("���¼���","���¼���");
	if (nSkillIndex<=0)
	{//������¼���
		//����ü��ܵĵȼ�Ϊ�㣬��ʾ��ѧ��ֱ����ͻ������
		if (pSkill->m_nSkillLevel <= 0)
		{
			//messageBox("�����Ӽ���","�����Ӽ���");
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.Add(pSkill->m_nSkillID, 0) == 0 )
				return;
		}
		else
		{
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.Add(pSkill->m_nSkillID, pSkill->m_nSkillLevel) == 0 )
				return;
		}
	}
	else
	{//�ɼ��ܾ�ֱ�����õȼ�
		    Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.SetLevel(nSkillIndex, pSkill->m_nSkillLevel);
	}

	//���ö���ļ��ܵ�
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.SetAddPoint(nSkillIndex, pSkill->m_nAddPoint);

	// ֪ͨ������¼�������
	//CoreDataChanged(GDCNI_FIGHT_SKILL_POINT, 0, Player[CLIENT_PLAYER_INDEX].m_nSkillPoint);

	KUiSkillData SkillData;
	SkillData.uGenre = CGOG_SKILL_FIGHT;
	SkillData.uId    = pSkill->m_nSkillID;
	SkillData.nLevel = pSkill->m_nSkillLevel;
	int nSkillPosition = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetSkillPosition(pSkill->m_nSkillID);
	if(nSkillPosition < 0)
	   return;

	CoreDataChanged(GDCNI_SKILL_CHANGE, (uintptr_t)&SkillData,nSkillPosition-4); //��ȥǰ��3�� Ĭ�ϼ���λ��
}

//-------------------------------------------------------------------------
//	���ܣ��յ������������Ķ��ѵȼ��仯֪ͨ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cGetTeammateLevel(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_TEAMMATE_LEVEL_SYNC	*pLevel = (PLAYER_TEAMMATE_LEVEL_SYNC*)pMsg;
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	if ((DWORD)g_Team[0].m_nCaptain == pLevel->m_dwTeammateID)
	{
		g_Team[0].m_nMemLevel[0] = (WORD)pLevel->m_btLevel;
		return;
	}

	for (int i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if ((DWORD)g_Team[0].m_nMember[i] == pLevel->m_dwTeammateID)
		{
			g_Team[0].m_nMemLevel[i + 1] = (WORD)pLevel->m_btLevel;
			break;
		}
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ�뿪����
//-------------------------------------------------------------------------
void KProtocolProcess::s2cLeaveFaction(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].m_cFaction.LeaveFaction();

	// �ı���Ӫ
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetCamp(camp_free);

	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ�г�Ա�뿪(�����Լ��뿪)
//-------------------------------------------------------------------------
void KProtocolProcess::s2cLeaveTeam(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
		Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateamUI(); //��ʾ�����������Ϣ
		return;
	}

	PLAYER_LEAVE_TEAM	*pLeaveTeam = (PLAYER_LEAVE_TEAM*)pMsg;

	// �Լ��뿪
	if (pLeaveTeam->m_dwNpcID == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 0;
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nApplyCaptainID = 0;
		g_Team[0].Release();
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMenuState() == PLAYER_MENU_STATE_TEAMOPEN)
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetMenuState(PLAYER_MENU_STATE_NORMAL);
	}
	// �����뿪
	else
	{
		g_Team[0].DeleteMember(pLeaveTeam->m_dwNpcID);
	}

	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateamUI(); //��ʾ�����������Ϣ
	return;
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ����
//-------------------------------------------------------------------------
void KProtocolProcess::s2cLevelUp(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].s2cLevelUp(pMsg);
}
//�ͻ����ƶ���Ʒ
void KProtocolProcess::s2cMoveItem(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_MOVE_ITEM_SYNC	*pMove = (PLAYER_MOVE_ITEM_SYNC*)pMsg;

	ItemPos	DownPos,UpPos;
	int     nIsMianBan;
	DownPos.nPlace    = pMove->m_btDownPos;
	DownPos.nX        = pMove->m_btDownX;
	DownPos.nY        = pMove->m_btDownY;
    DownPos.nDIsRongqi = pMove->m_DownRongqi;
	UpPos.nPlace      = pMove->m_btUpPos;
	UpPos.nX          = pMove->m_btUpX;
	UpPos.nY          = pMove->m_btUpY;
	UpPos.nSIsRongqi  = pMove->m_UpRongqi;
    nIsMianBan        = pMove->m_IsMianBan;

	ItemPos	sSrc, sDest;
	sDest.nPlace     = pMove->m_btDownPos;    //Դ����
	sDest.nX         = pMove->m_btDownX;
	sDest.nY         = pMove->m_btDownY;
    sDest.nSIsRongqi = pMove->m_DownRongqi;

	sSrc.nPlace     = pMove->m_btUpPos;  //Ŀ������
	sSrc.nX         = pMove->m_btUpX;
	sSrc.nY         = pMove->m_btUpY;
	sSrc.nDIsRongqi = pMove->m_UpRongqi;


    // char msg[128];
	// sprintf(msg,"mb:%d,Cli:cpy:%d,cxy:%d,cyy:%d||cpd:%d,cxd:%d,cyd:%d",nIsMianBan,DownPos.nPlace,DownPos.nX,DownPos.nY,UpPos.nPlace,UpPos.nX,UpPos.nY);
	// Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(msg);
	Player[CLIENT_PLAYER_INDEX].m_ItemList.AutoMoveItem(sDest, sSrc,nIsMianBan);
	//Player[CLIENT_PLAYER_INDEX].m_ItemList.ExchangeItem(&UpPos, &DownPos,nIsMianBan);
}

void KProtocolProcess::s2cRemoveItem(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	ITEM_REMOVE_SYNC	*pRemove = (ITEM_REMOVE_SYNC*)pMsg;

	int		nIdx;
	nIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchID(pRemove->m_ID);
	if (nIdx > 0)
	{
		if (pRemove->m_model==1)
		{
		    Player[CLIENT_PLAYER_INDEX].m_ItemList.Remove(nIdx,pRemove->m_dx,pRemove->m_dy,1);
		}
		else
		{
            Player[CLIENT_PLAYER_INDEX].m_ItemList.Remove(nIdx,pRemove->m_dx,pRemove->m_dy);
		}

		Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������������������Ϣ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cSetFactionData(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_FACTION_DATA	*pData = (PLAYER_FACTION_DATA*)pMsg;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetCamp(pData->m_btCamp);
	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction = (char)pData->m_btCurFaction;
	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nFirstAddFaction = (char)pData->m_btFirstFaction;
	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nAddTimes = pData->m_nAddTimes;

	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);

//	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[0].m_nOpenLevel = pData->m_btNo1SkillOpenLevel;
//	Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[1].m_nOpenLevel = pData->m_btNo2SkillOpenLevel;
//	if (Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction == Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[0].m_nID)
//		Player[CLIENT_PLAYER_INDEX].m_cFaction.OpenCurSkillLevel(Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[0].m_nOpenLevel, &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList);
//	else
//		Player[CLIENT_PLAYER_INDEX].m_cFaction.OpenCurSkillLevel(Player[CLIENT_PLAYER_INDEX].m_cFaction.m_sSkillOpen[1].m_nOpenLevel, &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ����״̬(�ӳ����뿪�š��رն���״̬ʱ�Ż��յ�����Ϣ)
//-------------------------------------------------------------------------
void KProtocolProcess::s2cSetTeamState(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	// �ͻ����������������������
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag || Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	PLAYER_TEAM_OPEN_CLOSE	*pTeamState = (PLAYER_TEAM_OPEN_CLOSE*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.ReleaseList();
	if (pTeamState->m_btOpenClose)
	{
		g_Team[0].SetTeamOpen();

		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, "%s", strCoreInfo[MSG_TEAM_OPEN].c_str());
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	}
	else
	{
		g_Team[0].SetTeamClose();

		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, "%s", strCoreInfo[MSG_TEAM_CLOSE].c_str());
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ����״̬(�ӳ����뿪�š��رն���״̬ʱ�Ż��յ�����Ϣ)
//-------------------------------------------------------------------------
void	KProtocolProcess::s2cApplyAddTeam(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].s2cApplyAddTeam(pMsg);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������������ĳ�����������������ʾ֮
//-------------------------------------------------------------------------
void KProtocolProcess::s2cShowTeamInfo(BYTE* pMsg)
{
	//if  (NULL==pMsg||NULL==this) return;
}
//ֱ�����ÿͻ��˿ռ�
void	KProtocolProcess::s2csyncitemposition(BYTE* pMsg)
{
	//if  (NULL==pMsg||NULL==this) return;
}


//�ͻ��˴򿪽���������
void KProtocolProcess::s2copenjindutiao(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
    PLAYER_OPEN_JINDUTIAO *pJindu = (PLAYER_OPEN_JINDUTIAO*)pMsg;
/*
	char mag[64];
	sprintf(mag,"���Դ򿪽������ɹ�");
    Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(mag);
*/
	KUiJinDuTiaoInfo JinDuData;
	sprintf(JinDuData.nTitle, "%s", pJindu->m_szTitle);
	sprintf(JinDuData.nCallFuc, "%s", pJindu->m_Callback);
	sprintf(JinDuData.szParma, "%s", pJindu->m_szParma);
	sprintf(JinDuData.szParmb, "%s", pJindu->m_szParmb);
	sprintf(JinDuData.szParmc, "%s", pJindu->m_szParmc);
	JinDuData.nMsgIndex    = pJindu->m_MsgIndex;
	JinDuData.nTimes       = pJindu->m_inTime;

	CoreDataChanged(GDCNI_OPEN_JINDUTIAO, (uintptr_t)&JinDuData,0);
}

//�ͻ��˽��� ���������ص� ����� ��ֵ
void KProtocolProcess::s2ctaskcallback(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
    	PLAYER_GETBACK_TASKVAL	*pTaskback = (PLAYER_GETBACK_TASKVAL*)pMsg;

        Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(pTaskback->m_TaskId,pTaskback->m_TaskVal);

}

//ͬ��װ������ �ͻ���  --�Ƹֱ�ʶ,�������װ��
void KProtocolProcess::s2csyncequipitem(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;

	ITEM_SYNC *pItemSync = (ITEM_SYNC*)pMsg;

    int nIdx = Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchID(pItemSync->m_ID);
	int i;
    if (nIdx>0)
	{
	     for (i = 0; i < 6; i++)
		 {
		   Item[nIdx].GetItemParam()->nRGeneratorLevel[i] = pItemSync->m_RongMagicLev[i];
//           Item[nIdx].GetItemParam()->nBGeneratorLevel[i] = pItemSync->m_BaoMagicLev[i];

		 }

		  if (pItemSync->m_Luck > 10)  //����ֵ
		  {
			pItemSync->m_Luck = 10;
		  }

		  if (pItemSync->m_Luck < 0)
		  {
			pItemSync->m_Luck = 0;
		  }

		   ///����Ϊ�����Ƹ�����
		   KItemNormalAttrib	sMAR[6];// ħ��
		   ZeroMemory(sMAR,sizeof(sMAR)); //�����ڴ�
		   KTabFile MagicTabR;
		   MagicTabR.Load(TABFILE_MAGICAGOLD_PATH);  //������װ��ħ�������ļ�

		   int nTypeR,nMaxR,nMinR;

		   g_RandomSeed(pItemSync->m_RandomSeed);

		   for (i = 0;i < 6;i++)
		   {//�������� �� ��ʶ����
			   if (Item[nIdx].GetItemParam()->nRGeneratorLevel[i] > 0 && i < pItemSync->m_Rongpiont)
			   {
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nRGeneratorLevel[i],5,0,&nTypeR);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nRGeneratorLevel[i],6,0,&nMaxR);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nRGeneratorLevel[i],7,0,&nMinR);
				   sMAR[i].nAttribType = nTypeR;  //��������
				   //sMAR[i].nValue[0] = nMaxR + ((nMinR - nMaxR) * pItemSync->m_Luck / 10);
				   sMAR[i].nValue[0] = GetRandomNumber(nMaxR,nMinR);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nRGeneratorLevel[i],8,0,&nMaxR);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nRGeneratorLevel[i],9,0,&nMinR);
				   //sMAR[i].nValue[1] = nMaxR + ((nMinR - nMaxR) * pItemSync->m_Luck / 10);
				   sMAR[i].nValue[1] = GetRandomNumber(nMaxR,nMinR);

				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nRGeneratorLevel[i],10,0,&nMaxR);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nRGeneratorLevel[i],11,0,&nMinR);
				   //sMAR[i].nValue[2] = nMaxR + ((nMinR - nMaxR) * pItemSync->m_Luck / 10);
				   sMAR[i].nValue[2] = GetRandomNumber(nMaxR,nMinR);

				   continue;
			   }
			   sMAR[i].nAttribType = 0;
			   sMAR[i].nValue[0] = 0;
			   sMAR[i].nValue[1] = 0;
			   sMAR[i].nValue[2] = 0;
		   }
		   Item[nIdx].SetAttrib_RON(sMAR);   //��������ħ����
		   ZeroMemory(sMAR,sizeof(sMAR));    //����

          /* for (i = 0;i < 6;i++)
		   {//��ʯ����
			   if (Item[nIdx].GetItemParam()->nBGeneratorLevel[i] > 0)
			   {
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nBGeneratorLevel[i],5,0,&nTypeR);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nBGeneratorLevel[i],6,0,&nMaxR);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nBGeneratorLevel[i],7,0,&nMinR);
				   sMAR[i].nAttribType = nTypeR;  //��������
				   sMAR[i].nValue[0] = nMaxR + ((nMinR - nMaxR) * pItemSync->m_Luck / 10);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nBGeneratorLevel[i],8,0,&nMaxR);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nBGeneratorLevel[i],9,0,&nMinR);
				   sMAR[i].nValue[1] = nMaxR + ((nMinR - nMaxR) * pItemSync->m_Luck / 10);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nBGeneratorLevel[i],10,0,&nMaxR);
				   MagicTabR.GetInteger(Item[nIdx].GetItemParam()->nBGeneratorLevel[i],11,0,&nMinR);
				   sMAR[i].nValue[2] = nMaxR + ((nMinR - nMaxR) * pItemSync->m_Luck / 10);

				   continue;
			   }
			   sMAR[i].nAttribType = 0;
			   sMAR[i].nValue[0] = 0;
			   sMAR[i].nValue[1] = 0;
			   sMAR[i].nValue[2] = 0;
		   }
		   Item[nIdx].SetAttrib_Bao(sMAR);   //��������ħ����
		   */
		   Item[nIdx].SetPoint(pItemSync->m_Point);

           if (Item[nIdx].IsBlue() && (pItemSync->m_IsWhere>0 || pItemSync->m_Rongpiont>0))   //�Ѿ���ʶ����װ �����Ѿ��Ƹֵ���װ
		   {
                Item[nIdx].EnChanceRong(pItemSync->m_EnChance);  //��װǿ��
		   }
		   else if (Item[nIdx].IsBlue())   //ȫ������װ
                 Item[nIdx].EnChance(pItemSync->m_EnChance);      //��װǿ��
		   else if (Item[nIdx].IsPurple())
		   {//��װ
				   /*for (i = 0; i < 6; ++i)
				   {
					   Item[nIdx].GetItemParam()->nGeneratorLevel[i] = pItemSync->m_MagicLevel[i];
				   }

				   int nLucky=pItemSync->m_Luck;

				   KItemNormalAttrib sMz[6];
				   KTabFile tabMagicAttribe;
				   tabMagicAttribe.Load(TABFILE_MAGICATTRIB_PATH);
				   char nInfo[64]={0};
				   int nType,nMax,nMin;
				   for (i = 0;i < 6;++i)
				   {
					   if (Item[nIdx].GetItemParam()->nGeneratorLevel[i] > 0 && i < pItemSync->m_Point)
					   {
						   tabMagicAttribe.GetInteger(Item[nIdx].GetItemParam()->nGeneratorLevel[i],5,0,&nType);
						   tabMagicAttribe.GetInteger(Item[nIdx].GetItemParam()->nGeneratorLevel[i],6,0,&nMax);
						   tabMagicAttribe.GetInteger(Item[nIdx].GetItemParam()->nGeneratorLevel[i],7,0,&nMin);
						   sMz[i].nAttribType = nType;
						   //sMz[i].nValue[0] = nMax + ((nMin - nMax) * nLucky / 10);
						   sMz[i].nValue[0] = GetRandomNumber(nMax,nMin);
						   sprintf(nInfo,"���� %d ��һ��ֵ:%d",i,sMz[i].nValue[0]);
						   Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nInfo);
						   tabMagicAttribe.GetInteger(Item[nIdx].GetItemParam()->nGeneratorLevel[i],8,0,&nMax);
						   tabMagicAttribe.GetInteger(Item[nIdx].GetItemParam()->nGeneratorLevel[i],9,0,&nMin);
						   //sMz[i].nValue[1] = nMax + ((nMin - nMax) * nLucky / 10);
						   sMz[i].nValue[1] = GetRandomNumber(nMax,nMin);
						   sprintf(nInfo,"���� %d �ڶ���ֵ:%d",i,sMz[i].nValue[1]);
						   Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nInfo);
						   tabMagicAttribe.GetInteger(Item[nIdx].GetItemParam()->nGeneratorLevel[i],10,0,&nMax);
						   tabMagicAttribe.GetInteger(Item[nIdx].GetItemParam()->nGeneratorLevel[i],11,0,&nMin);
						   //sMz[i].nValue[2] = nMax + ((nMin - nMax) * nLucky / 10);
						   sMz[i].nValue[2] = GetRandomNumber(nMax,nMin);
						   sprintf(nInfo,"���� %d ������ֵ:%d",i,sMz[i].nValue[2]);
						   Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nInfo);
						   continue;
					   }


					   sMz[i].nAttribType = 0;
					   sMz[i].nValue[0] = 0;
					   sMz[i].nValue[1] = 0;
					   sMz[i].nValue[2] = 0;
				   }
				   Item[nIdx].SetAttrib_MA(sMz);
				   tabMagicAttribe.Clear();*/
				   Item[nIdx].EnChance(pItemSync->m_EnChance);
		   }

		   if (Item[nIdx].GetGoldId()/* && pItemSync->m_Rongpiont>0*/)
		   {
		       Item[nIdx].EnChanceRong(pItemSync->m_EnChance);  //����ǿ��
		       Item[nIdx].EnChanceBao(pItemSync->m_EnChance);   //��ʯǿ��
		   }

		   MagicTabR.Clear();

		   Item[nIdx].SetStackNum(pItemSync->m_StackNum);
		   Item[nIdx].SetRPoint(pItemSync->m_Rongpiont);
		   Item[nIdx].SetIsWhere(pItemSync->m_IsWhere);
		   Item[nIdx].SetSeries(pItemSync->m_Series);
		   Item[nIdx].SetBang(pItemSync->m_IsBang);
	       Item[nIdx].SetKuaiJie(pItemSync->m_IsKuaiJie);
		   Item[nIdx].SetIsCanUse(pItemSync->m_IsCanUse);
           Item[nIdx].SetTime(pItemSync->m_Time.bYear,pItemSync->m_Time.bMonth,pItemSync->m_Time.bDay,pItemSync->m_Time.bHour,pItemSync->m_Time.bMin);
           Item[nIdx].SetDurability(pItemSync->m_Durability);  // ͬ���־ö�

           Item[nIdx].SetMagic(pItemSync->m_IsMagic);          //�Ƿ���ħ������
	       Item[nIdx].SetSkillType(pItemSync->m_SkillType);
	       Item[nIdx].SetMagicID(pItemSync->m_MagicID);

		   Item[nIdx].SetUseMapIdx(pItemSync->m_UseMap);
		   Item[nIdx].SetUseKind(pItemSync->m_UseKind);
		   Item[nIdx].SetItemResIdx(pItemSync->m_Res);

		   Item[nIdx].SetID(pItemSync->m_ID);             //����DwID
		   if (pItemSync->m_ItmeInfo[0])
		      Item[nIdx].SetItmeInfo(pItemSync->m_ItmeInfo);
   }
	//�����ͻ���
            //char msg[64];
			//sprintf(msg,Item[nIdx].GetName());
            //messageBox(msg,"ͬ����Ʒ�ɹ�");
   Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
}

//ͬ��װ������ �ͻ��˲���װ��  --ˢװ��
void KProtocolProcess::s2cSyncItem(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;

	ITEM_SYNC	*pItemSync = (ITEM_SYNC*)pMsg;
	int pnMagicParam[6],nIndex,pnRMagicParam[6],pnJBParam[7];  //pnBMagicParam[6],
	int i;
	for (i = 0; i < 6; i++)
	{
		pnMagicParam[i]  = pItemSync->m_MagicLevel[i];
		pnRMagicParam[i] = pItemSync->m_RongMagicLev[i];
//      pnBMagicParam[i] = pItemSync->m_BaoMagicLev[i];

      if (pItemSync->m_Point==8)
	  {//����ƽ�

	  }
	}
	for (i = 0; i < 7; i++)
	{
        pnJBParam[i]     = pItemSync->m_JBLevel[i];
	}

	if (pItemSync->m_GoldId<=0)
		nIndex = ItemSet.AddCilentItem(\
			pItemSync->m_Genre,\
			pItemSync->m_Series,\
			pItemSync->m_Level,\
			pItemSync->m_Luck,\
			pItemSync->m_Detail,\
			pItemSync->m_Particur,\
			pnMagicParam,\
			pItemSync->m_Version,\
			pItemSync->m_RandomSeed,\
			pItemSync->m_StackNum,\
			pItemSync->m_EnChance,\
			pItemSync->m_Point,\
			pItemSync->m_Time.bYear,\
			pItemSync->m_Time.bMonth,\
			pItemSync->m_Time.bDay,\
			pItemSync->m_Time.bHour,\
			pItemSync->m_Time.bMin,\
			pItemSync->m_Rongpiont,\
			pnRMagicParam,\
			pnJBParam,\
			NULL,\
			NULL,\
			NULL,\
			NULL,pItemSync->m_IsWhere);
	else if (pItemSync->m_GoldId>0)
	{
	  if (pItemSync->m_Point==8)
	   nIndex = ItemSet.AddRondomGold(
	     pItemSync->m_GoldId,\
	     pnMagicParam,\
	     pItemSync->m_Series,\
	     pItemSync->m_EnChance,\
	     pItemSync->m_Rongpiont,\
	     pItemSync->m_Time.bYear,\
	     pItemSync->m_Time.bMonth,\
	     pItemSync->m_Time.bDay,\
	     pItemSync->m_Time.bHour,\
	     pItemSync->m_Time.bMin,\
	     pnRMagicParam,\
	     pItemSync->m_Level,pItemSync->m_Luck,pItemSync->m_Detail,-1,1,0,pItemSync->m_IsPlatima,FALSE,pItemSync->m_RandomSeed);
	  else
      nIndex = ItemSet.AddGold(
		pItemSync->m_GoldId,\
		pnMagicParam,\
		pItemSync->m_Series,\
		pItemSync->m_EnChance,\
        pItemSync->m_Rongpiont,\
		pItemSync->m_Time.bYear,\
		pItemSync->m_Time.bMonth,\
		pItemSync->m_Time.bDay,\
		pItemSync->m_Time.bHour,\
        pItemSync->m_Time.bMin,\
        pnRMagicParam,\
		NULL,\
		pItemSync->m_Level,1,-1,-1,1,0,pItemSync->m_IsPlatima,FALSE,pItemSync->m_RandomSeed);
	}
	/*
	   AddGold(IN int nId , IN int* pnMagicLevel , \
	   IN int nSeries,IN int nEnChance,\
	   IN int nRongpiont, \
	   int nYear, int nMonth, int nDay, int nHour,int nMin,\
	   IN int* pnRMagicLevel,IN int* pnBMagicLevel,\
	   IN int nLevel,IN int nLuck, IN int nDetailType, \
	   IN int nParticular,IN int nStackNum,IN int nVersion,int nIsPlatina)

	*/

	if (nIndex > 0)
	{//���ÿͻ��˸�����Ϣ
		Item[nIndex].SetStackNum(pItemSync->m_StackNum);         //������ �����ÿɵ�����
		//pItemSync->m_IsBang     = Item[nIndex].GetIsBang();      //�Ƿ��
		//pItemSync->m_IsKuaiJie  = Item[nIndex].GetIsKuaiJie();   //�Ƿ�����
		//pItemSync->m_IsMagic    = Item[nIndex].GetIsMagic();
		//pItemSync->m_SkillType  = Item[nIndex].GetSkillType();
		//pItemSync->m_MagicID    = Item[nIndex].GetMagicID();
		Item[nIndex].SetBang(pItemSync->m_IsBang);
		Item[nIndex].SetKuaiJie(pItemSync->m_IsKuaiJie);
		Item[nIndex].SetMagic(pItemSync->m_IsMagic);
		Item[nIndex].SetSkillType(pItemSync->m_SkillType);
		Item[nIndex].SetMagicID(pItemSync->m_MagicID);
        Item[nIndex].SetIsCanUse(pItemSync->m_IsCanUse); //�ͻ��������Ƿ����ʹ��
		Item[nIndex].SetUseMapIdx(pItemSync->m_UseMap);
		Item[nIndex].SetItemResIdx(pItemSync->m_Res);
		Item[nIndex].SetUseKind(pItemSync->m_UseKind);
		//if (strrchr(Item[nIndex].GetItmeInfo(),'.lua'))
		//{
		if (pItemSync->m_ItmeInfo[0])
		   Item[nIndex].SetItmeInfo(pItemSync->m_ItmeInfo);
		//}

        //sprintf(pItemSync->m_WonName,"%s",Item[nIndex].GetWonName());  //������
		Item[nIndex].SetWonName("系统");//pItemSync->m_WonName
		Item[nIndex].SetID(pItemSync->m_ID);  //����DwID
		Item[nIndex].SetDurability(pItemSync->m_Durability);  // ͬ���־ö�
		//�ͻ�������װ��
        Player[CLIENT_PLAYER_INDEX].m_Autoplay.nShiQuNum=0;
		Player[CLIENT_PLAYER_INDEX].m_ItemList.Add(nIndex, pItemSync->m_btPlace, pItemSync->m_btX, pItemSync->m_btY,0,0,pItemSync->m_StackNum,pItemSync->m_IsLoign);
        //----------------��ҹ���װ������----------------------------------------
		int k,nYval,nDvala,nDvalb,nDvalc;
		char nItemName[64]={0},nToolName[64]={0};

	     //���Ե�����
		int nSetNun=Player[CLIENT_PLAYER_INDEX].m_Autoplay.nShuXingNum;

        if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_AutoplayId==1 && nSetNun>0 && nSetNun<7)
        {//����ǹһ�
		   char szTempDesc[128]={0},nTempStrVer[128];
		   if (Item[nIndex].GetGenre()==item_equip)
		   {//��װװ������

		      int nSxNum=Item[nIndex].GetMagicCount();

			  if  (nSxNum<nSetNun)
			  {//���ʵ����������С�� ���õ����� �Ͷ���
				   Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
			       Player[CLIENT_PLAYER_INDEX].AutoAwayItem(pItemSync->m_ID);
				   return;
			  }

		      int nIsHave=0;
		      if ((Player[CLIENT_PLAYER_INDEX].m_Autoplay.nAutoEques==1 || Player[CLIENT_PLAYER_INDEX].m_Autoplay.nAutoMoneyAndEques==1))
			  {
	             for(i=0;i<16;i++)
	             {
					 //ZeroMemory(nTempVer,sizeof(nTempVer));
					 //sprintf(nsetionKey,"item_%d",i);
		             Player[CLIENT_PLAYER_INDEX].nEquipItem.GetString(i+1,1,"无数据",nItemName,sizeof(nItemName));  //���Թؼ���
					 Player[CLIENT_PLAYER_INDEX].nEquipItem.GetInteger(i+1,2,0,&nYval);  //���Թؼ���
					 //nYval = g_Atoui(nTempVer);
					 //Player[CLIENT_PLAYER_INDEX].nEquipItem.GetInteger(i+1,3,3,&nYval);
						 for (k=0;k<nSxNum;k++)
		                 {
		                   const char	*pszKeyName = g_MagicID2String(Item[nIndex].GetAttribType(k));
						   nDvala =  Item[nIndex].GetAttribTypeVala(k);
						   nDvalb =  Item[nIndex].GetAttribTypeValb(k);
						   nDvalc =  Item[nIndex].GetAttribTypeValc(k);

			                Player[CLIENT_PLAYER_INDEX].m_FileDesc.GetString("Descript",pszKeyName, "无数据", szTempDesc, sizeof(szTempDesc));
							sprintf(nTempStrVer, "%s", UTEXT(szTempDesc,1).c_str());
							if (nYval>0 && nDvala>=nYval && strstr(nTempStrVer,nItemName)/* || nDvalb>=nYval || nDvalc>=nYval*/)
							{//���������ؼ��ֵ�����
							    nIsHave++;
							}
							else if (nYval<=0 && strstr(nTempStrVer,nItemName))
							{
							    nIsHave++;
							}
	                     }
                 }
			}
            if (nIsHave==0)
			{//������������װ���� �Ͷ���
               Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
			   Player[CLIENT_PLAYER_INDEX].AutoAwayItem(pItemSync->m_ID);
			   return;
			}
		   }
/*
		   if (Player[CLIENT_PLAYER_INDEX].m_Autoplay.nAutoTool==1)
		   {//��������װ������
		      if (Item[nIndex].GetGenre()==item_equip)
			  {//��װ�� �Ϳ�ʼ���

			  }
		   }*/
		}
//---------------------------����Ϊ������Ϣ---------------------------
		char sMsg[125]={0};
        /*int nblue,ngold,nzise;
           nblue=Item[nIndex].IsBlue();
           nzise=Item[nIndex].IsPurple();
           ngold=Item[nIndex].GetGoldId();
		   if (ngold>0)
			   sprintf(sMsg, "��ʾ:���(%sx%d)!", Item[nIndex].GetName(),Item[nIndex].GetStackNum());
           else if (nblue==1)
			   sprintf(sMsg, "��ʾ:���(%sx%d)!", Item[nIndex].GetName(),Item[nIndex].GetStackNum());
		   else if (nzise>0)
			   sprintf(sMsg, "��ʾ:���(%sx%d)!", Item[nIndex].GetName(),Item[nIndex].GetStackNum());
		   else*/
		   sprintf(sMsg, "提示:获得(%sx%d)!", Item[nIndex].GetName(),Item[nIndex].GetStackNum());
		   Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(sMsg);
//------------------------------------------------------------------------
	}
	//�����ͻ���
	Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
}
//-------------------------------------------------------------------------
//	���ܣ��յ���������������ͬ��money����Ϣ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cSyncMoney(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].s2cSyncMoney(pMsg);
}
void KProtocolProcess::s2cSyncXu(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].s2cSyncXu(pMsg);
}

void KProtocolProcess::s2cSyncRoleList(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	ROLE_LIST_SYNC	*pSync = (ROLE_LIST_SYNC *)pMsg;
	// TODO: ͨ��CORESHELL�������棿
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ�����˼������
//-------------------------------------------------------------------------
void KProtocolProcess::s2cTeamAddMember(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;

	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	PLAYER_TEAM_ADD_MEMBER	*pAddMem = (PLAYER_TEAM_ADD_MEMBER*)pMsg;
	int	nMemNum;

	nMemNum = g_Team[0].FindFree();
	if (nMemNum < 0)
		return;
	g_Team[0].m_nMember[nMemNum] = pAddMem->m_dwNpcID;
	g_Team[0].m_nMemLevel[nMemNum + 1] = (WORD)pAddMem->m_btLevel;
	strcpy(g_Team[0].m_szMemName[nMemNum + 1], pAddMem->m_szName);
	g_Team[0].m_nMemNum++;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.DeleteOneFromApplyList(pAddMem->m_dwNpcID);
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
	//Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateamUI(); //��ʾ�����������Ϣ

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage,  strCoreInfo[MSG_TEAM_ADD_MEMBER].c_str(), pAddMem->m_szName);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority    = 0;
	sMsg.byParamSize   = 0;
	sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ�����ӳ�
//-------------------------------------------------------------------------
void KProtocolProcess::s2cTeamChangeCaptain(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	int		nLevel, nMemNo;
	char	szName[32]={0};
	PLAYER_TEAM_CHANGE_CAPTAIN	*pChange = (PLAYER_TEAM_CHANGE_CAPTAIN*)pMsg;

	nMemNo = g_Team[0].FindMemberID(pChange->m_dwCaptainID);
	// ��ǰ������������
	if ((DWORD)g_Team[0].m_nCaptain != pChange->m_dwMemberID || nMemNo < 0)
	{
		Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		return;
	}

	// �Լ�������Ϊ�ӳ�
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pChange->m_dwCaptainID)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_CHANGE_CAPTAIN_SELF].c_str(), g_Team[0].m_szMemName[0]);
		sMsg.eType = SMT_TEAM;
		sMsg.byConfirmType = SMCT_UI_TEAM;
		sMsg.byPriority = 3;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	}
	else	// ���˱�����Ϊ�ӳ�
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_CHANGE_CAPTAIN].c_str(), g_Team[0].m_szMemName[nMemNo + 1]);
		sMsg.eType = SMT_TEAM;
		sMsg.byConfirmType = SMCT_UI_TEAM;
		sMsg.byPriority = 3;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	}

	// ������Ա
	nLevel = g_Team[0].m_nMemLevel[0];
	strcpy(szName, g_Team[0].m_szMemName[0]);
	g_Team[0].m_nCaptain = pChange->m_dwCaptainID;
	g_Team[0].m_nMemLevel[0] = g_Team[0].m_nMemLevel[nMemNo + 1];
	strcpy(g_Team[0].m_szMemName[0], g_Team[0].m_szMemName[nMemNo + 1]);
	g_Team[0].m_nMember[nMemNo] = pChange->m_dwMemberID;
	g_Team[0].m_nMemLevel[nMemNo + 1] = nLevel;
	strcpy(g_Team[0].m_szMemName[nMemNo + 1], szName);

	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pChange->m_dwCaptainID)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
		Player[CLIENT_PLAYER_INDEX].m_cTeam.ReleaseList();
	}
	else if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pChange->m_dwMemberID)
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_MEMBER;
		Player[CLIENT_PLAYER_INDEX].m_cTeam.ReleaseList();
	}

	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateamUI(); //��ʾ�����������Ϣ
}

//-------------------------------------------------------------------------
//	���ܣ��յ��������������Լ��������������������Ӧ��Ϣ
//-------------------------------------------------------------------------
void KProtocolProcess::s2cUpdataSelfTeamInfo(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	int		i;
	PLAYER_SEND_SELF_TEAM_INFO	*pSelfInfo = (PLAYER_SEND_SELF_TEAM_INFO*)pMsg;

	// ���ݰ�����
	if (pSelfInfo->m_dwNpcID[0] == 0)
		return;

	Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadExp = pSelfInfo->m_dwLeadExp;
	Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(Player[CLIENT_PLAYER_INDEX].m_dwLeadExp);
	Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);
	if (Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp == 0)
		Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp = Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel;

	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == pSelfInfo->m_dwNpcID[0])	// �ӳ�
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
	}
	else													// ��Ա
	{
		Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_MEMBER;
		for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_sApplyList[i].Release();
	}
	g_Team[0].m_nCaptain = pSelfInfo->m_dwNpcID[0];
	if (pSelfInfo->m_btState == 0)
		g_Team[0].SetTeamClose();
	else
		g_Team[0].SetTeamOpen();

	g_Team[0].m_nTeamServerID = pSelfInfo->nTeamServerID;	  //�����ڷ������е� Ψһid
	g_Team[0].m_nMemLevel[0] = (WORD)pSelfInfo->m_btLevel[0];
	strcpy(g_Team[0].m_szMemName[0], pSelfInfo->m_szNpcName[0]);
	g_Team[0].m_nMemNum = 0;
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (pSelfInfo->m_dwNpcID[i + 1] > 0)
		{
			g_Team[0].m_nMember[i] = pSelfInfo->m_dwNpcID[i + 1];         //��Ա��Ϣ
			g_Team[0].m_nMemLevel[i + 1] = (WORD)pSelfInfo->m_btLevel[i + 1];
			strcpy(g_Team[0].m_szMemName[i + 1], pSelfInfo->m_szNpcName[i + 1]);
			g_Team[0].m_nMemNum++;
		}
		else
		{
			g_Team[0].m_nMember[i] = -1;
			g_Team[0].m_nMemLevel[i + 1] = 0;
			g_Team[0].m_szMemName[i + 1][0] = 0;
		}
	}
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateamUI(); //��ʾ�����������Ϣ
}
//ͬ����������
void KProtocolProcess::SyncCurNormalData(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CURPLAYER_NORMAL_SYNC	*pSync = (CURPLAYER_NORMAL_SYNC*)pMsg;

	if (Player[CLIENT_PLAYER_INDEX].m_nIndex<=0 || Player[CLIENT_PLAYER_INDEX].m_nIndex>=MAX_NPC)
		return;

	if (pSync->m_shLife > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife = pSync->m_shLife;
	else
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife = 0;
	if (pSync->m_shStamina > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina = pSync->m_shStamina;
	else
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina = 0;

	if (pSync->m_shMana > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana = pSync->m_shMana;
	else
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana = 0;

	//if (pSync->m_shAngry > 0)//��ǰŭֵ
	//	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentNuQi = pSync->m_shAngry;
	//else
	//	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentNuQi = 0; //ŭ��
///////////////////////////////
	if (pSync->m_shLifeMax > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax = pSync->m_shLifeMax;
	else
        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax = 100;

	if (pSync->m_shStaminaMax > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStaminaMax = pSync->m_shStaminaMax;

	if (pSync->m_shManaMax > 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax = pSync->m_shManaMax;
	else
        Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax = 100;

	if (pSync->m_shAngryMax > 0)  //ŭ�����ֵ
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentNuQiMax = pSync->m_shAngryMax;

//	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_sit)
//	{
//		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife >= Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax &&
//			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana >= Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax)
//		{
//			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
//		}
//	}
//��ǰ��ʱ��֡��
	Player[CLIENT_PLAYER_INDEX].m_nForbiddenFlag           = pSync->m_nForbiddenFlag;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	switch (pSync->m_btTeamData)
	{
	case 0x00:
		if ( Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag )
		{
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 0;
			Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		}
		break;
	case 0x03:
		if ( !Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag || Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure != TEAM_CAPTAIN)
		{
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_CAPTAIN;
			Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		}
		break;
	case 0x01:
		if ( !Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag || Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure != TEAM_MEMBER)
		{
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag = 1;
			Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure = TEAM_MEMBER;
			Player[CLIENT_PLAYER_INDEX].ApplySelfTeamInfo();
		}
		break;
	}
}
//����ͬ���ͻ���	���� ����
void KProtocolProcess::SyncCurPlayer(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	//messageBox("�յ�SyncCurPlayer����ͬ����","SyncCurPlayer");
	Player[CLIENT_PLAYER_INDEX].SyncCurPlayer(pMsg);
}
//����ͬ�����ݿ�����=--�ͻ���
void KProtocolProcess::SyncNpc(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;

	NPC_SYNC* NpcSync = (NPC_SYNC *)pMsg;

/*
	int nRegion = SubWorld[0].FindRegion(NpcSync->RegionID);
	// ���Region�����ڣ�˵����������������⣬�������ˡ�
	if (nRegion == -1)
		return;
*/
	int nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[0].Mps2Map(NpcSync->MapX, NpcSync->MapY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	if (nRegion == -1)
		return;

	int nIdx = NpcSet.SearchID(NpcSync->ID);

	if (!nIdx)
	{//2��ͬ����  ���û�� ��������
		nIdx = NpcSet.AddServerNpcA(NpcSync->NpcSettingIdx, 0, NpcSync->MapX, NpcSync->MapY,-1,0,0,0,0,0,NpcSync->m_btKind,NpcSync->ID);

		Npc[nIdx].m_dwID = NpcSync->ID;
		Npc[nIdx].m_Kind = NpcSync->m_btKind;

		Npc[nIdx].m_Height = 0;

		if (NpcSet.IsNpcRequestExist(NpcSync->ID))
			NpcSet.RemoveNpcRequest(NpcSync->ID);
	}
	else
	{
		if (Npc[nIdx].m_RegionIndex >= 0)
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecNpcRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY);

		Npc[nIdx].m_MapX = nMapX;
		Npc[nIdx].m_MapY = nMapY;
		Npc[nIdx].m_OffX = nOffX;
		Npc[nIdx].m_OffY = nOffY;

		Npc[nIdx].m_NpcSettingIdx = (short)HIWORD(NpcSync->NpcSettingIdx);
		Npc[nIdx].m_Level = LOWORD(NpcSync->NpcSettingIdx);

		if (Npc[nIdx].m_RegionIndex >= 0)
		{
			if (Npc[nIdx].m_RegionIndex != nRegion)
			{//��������ڷ������˵�����
				SubWorld[0].NpcChangeRegion(SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].m_RegionID, SubWorld[0].m_Region[nRegion].m_RegionID, nIdx);
				Npc[nIdx].m_RegionIndex = nRegion;
			}
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].AddNpcRef(Npc[nIdx].m_MapX,Npc[nIdx].m_MapY);
		}
		else
		{
			SubWorld[0].m_Region[nRegion].AddNpc(nIdx);
			Npc[nIdx].m_RegionIndex = nRegion;
			SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].AddNpcRef(Npc[nIdx].m_MapX,Npc[nIdx].m_MapY);
		}
		//g_ScenePlace.setIsPaint(true);
	}

	Npc[nIdx].m_Camp			= (NPCCAMP)NpcSync->Camp;
	Npc[nIdx].m_CurrentCamp		= (NPCCAMP)NpcSync->CurrentCamp;
	Npc[nIdx].m_Series			= NpcSync->m_bySeries;

     Npc[nIdx].m_CurrentdanggeRate		= NpcSync->Dangge; //����
     Npc[nIdx].m_CurrentzhongjiRate   	= NpcSync->Zhongji;

	  //Npc[nIdx].SetstrInfo(STR_GUISHU_NAME,NpcSync->GuishuName);
    //if (Npc[nIdx].m_Kind!=kind_player)
	  Npc[nIdx].m_cGold.SetGoldCurrentType(NpcSync->NpcEnchant);      //ͬ��BOSS��������

	/*if (NpcSync->LifePerCent <= 128)//���ƶ�7λ
		Npc[nIdx].m_CurrentLife	=(Npc[nIdx].m_CurrentLifeMax * NpcSync->LifePerCent) >> 7 ;
	else
		Npc[nIdx].m_CurrentLife	= 0;*/

	  if (NpcSync->nLifeYu >=0 && Npc[nIdx].m_CurrentLifeMax > 0)
	  {
		  Npc[nIdx].m_CurrentLife=Npc[nIdx].m_CurrentLifeMax-NpcSync->nLifeYu;//TakeRemainder(m_CurrentLifeMax,m_CurrentLife);
	  	  if (Npc[nIdx].m_CurrentLife<0)
			  Npc[nIdx].m_CurrentLife=0;
	  }

	if (Npc[nIdx].m_Doing != do_death || Npc[nIdx].m_Doing != do_revive) //need check later -- spe 03/05/27
		Npc[nIdx].SendSerCommand((NPCCMD)NpcSync->m_Doing, NpcSync->MapX, NpcSync->MapY);

	Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	//Npc[nIdx].SetMenuState(NpcSync->m_btMenuState);
	memset(Npc[nIdx].Name,0,sizeof(Npc[nIdx].Name));
	sprintf(Npc[nIdx].Name, "%s", NpcSync->m_szName);
}


void KProtocolProcess::s2cplayermissiondata(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;

	PLAYER_MISSION_DATA	*pData = (PLAYER_MISSION_DATA*)pMsg;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nMissionGroup = pData->m_nMissionGroup;

	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0); //����ɾ��Ƶ��

}

void KProtocolProcess::s2csyncrankdata(BYTE* pMsg)
{
	return;
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_MISSION_RANKDATA* pInfo = (PLAYER_MISSION_RANKDATA *)pMsg;
	//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("�յ���Ϣ");
	memcpy(&Player[CLIENT_PLAYER_INDEX].m_MissionData, &pInfo->SelfData, sizeof(pInfo->SelfData));
	for(int i = 0; i < MISSION_STATNUM;i++)	       //ʮ������
		memcpy(&Player[CLIENT_PLAYER_INDEX].m_MissionRank[i], &pInfo->MissionRank[i], sizeof(pInfo->MissionRank[i]));
}

void KProtocolProcess::s2cnpcsetpos(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_PLAYER_TYPE_NORMAL_SYNC *pSync = (NPC_PLAYER_TYPE_NORMAL_SYNC*)pMsg;

	int nIdx = NpcSet.SearchID(pSync->m_dwNpcID);

	if(nIdx > 0)
	{

	    int m_gubWorldIndex	= Npc[nIdx].m_SubWorldIndex;

		if  (m_gubWorldIndex<0)
			return;

		if (Npc[nIdx].m_RegionIndex>=0)
		   SubWorld[m_gubWorldIndex].m_Region[Npc[nIdx].m_RegionIndex].DecNpcRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY);

		int nRegionX = pSync->m_dwMapX / (SubWorld[m_gubWorldIndex].m_nCellWidth * SubWorld[m_gubWorldIndex].m_nRegionWidth);
		int nRegionY = pSync->m_dwMapY / (SubWorld[m_gubWorldIndex].m_nCellHeight * SubWorld[m_gubWorldIndex].m_nRegionHeight);
		int nRegion  = -1;
		DWORD	dwRegionID = MAKELONG(nRegionX, nRegionY);
		nRegion = SubWorld[m_gubWorldIndex].FindRegion(dwRegionID);
		if (Npc[nIdx].m_RegionIndex != nRegion || nRegion < 0)
		{
			if (Npc[nIdx].m_RegionIndex >= 0)
				SubWorld[m_gubWorldIndex].NpcChangeRegion(SubWorld[m_gubWorldIndex].m_Region[Npc[nIdx].m_RegionIndex].m_RegionID, dwRegionID, nIdx,true);
			else
				SubWorld[m_gubWorldIndex].NpcChangeRegion(-1, dwRegionID, nIdx,true);
		}

		int nMapX, nMapY, nOffX, nOffY;
		SubWorld[m_gubWorldIndex].Mps2Map(pSync->m_dwMapX, pSync->m_dwMapY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

		//CCAssert(nRegion == Npc[nIdx].m_RegionIndex,"");
		if (nRegion!=Npc[nIdx].m_RegionIndex)
			return;

		Npc[nIdx].m_MapX = nMapX;
		Npc[nIdx].m_MapY = nMapY;
		Npc[nIdx].m_OffX = nOffX;
		Npc[nIdx].m_OffY = nOffY;

		//SubWorld[m_gubWorldIndex].m_Region[Npc[nIdx].m_RegionIndex].AddRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY, obj_npc);
		SubWorld[m_gubWorldIndex].m_Region[Npc[nIdx].m_RegionIndex].AddNpcRef(Npc[nIdx].m_MapX,Npc[nIdx].m_MapY);
		Npc[nIdx].SendSerCommand(do_stand);
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
		//g_ScenePlace.setIsPaint(true);
	}

}


void KProtocolProcess::s2csyncnodataeffect(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	STATE_NODATA_EFFECT_SYNC* pNoState = (STATE_NODATA_EFFECT_SYNC *)pMsg;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetToolNoStateEffect(Player[CLIENT_PLAYER_INDEX].m_nIndex, pNoState->m_dwSkillID, pNoState->m_nLevel, pNoState->m_nTime);

	return;
}

void KProtocolProcess::s2csynconeclearstate(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_ONESTATE_SYNC* pPlayState = (PLAYER_ONESTATE_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pPlayState->ID);

	if (nIdx==Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
		//Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].ClearOneSkillState(pPlayState->m_Skillid);
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetSkillLeftTime(pPlayState->m_Skillid,pPlayState->m_Time);
	}
	return;
}

//��Ϸ ���� ��ʵʱͬ������--�ͻ���
void	KProtocolProcess::s2csyncplayermap(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_NORMALMAP_SYNC* pPlaySyncMap = (PLAYER_NORMALMAP_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pPlaySyncMap->ID);

	  if (nIdx==Player[CLIENT_PLAYER_INDEX].m_nIndex)
	  {
		Player[CLIENT_PLAYER_INDEX].m_nChestPW	= pPlaySyncMap->m_IsInCity;
	  }

}

//��Ϸ ���� ��ʵʱͬ������--�ͻ��� ����ѭ��
void KProtocolProcess::SyncNpcMin(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_NORMAL_SYNC* NpcSync = (NPC_NORMAL_SYNC *)pMsg;

//	if (NpcSync->ID == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
//		return;
	if (NpcSync->Doing == do_revive || NpcSync->Doing == do_death)  //�������� ����������
	{
		 return;
	}
	int nIdx = NpcSet.SearchID(NpcSync->ID);
	if (!nIdx)
	{//���û�����NPC������
		// �����������ͬ�����NPC��ȫ������
		if (!NpcSet.IsNpcRequestExist(NpcSync->ID))
		{
			SendClientCmdRequestNpc(NpcSync->ID);
			NpcSet.InsertNpcRequest(NpcSync->ID);
		}
	}
	else
	{
//		int nRegion = SubWorld[0].FindRegion(NpcSync->RegionID);

		//Npc[nIdx].m_isClearSpr = NpcSync->m_isClearSpr;

		int nRegion, nMapX, nMapY, nOffX, nOffY;
		SubWorld[0].Mps2Map(NpcSync->MapX, NpcSync->MapY,&nRegion,&nMapX, &nMapY, &nOffX, &nOffY);

		if (Npc[nIdx].m_RegionIndex == -1 && nIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)	 //�����
		{//�ǵ�ͼ��
			if (nRegion == -1)
			{
				return;
			}
			else
			{
				Npc[nIdx].m_MapX = nMapX;
				Npc[nIdx].m_MapY = nMapY;
				Npc[nIdx].m_OffX = nOffX;
				Npc[nIdx].m_OffY = nOffY;
				Npc[nIdx].m_RegionIndex = nRegion;
				Npc[nIdx].m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
				SubWorld[0].m_Region[nRegion].AddNpc(nIdx);
				SubWorld[0].m_Region[nRegion].AddNpcRef(Npc[nIdx].m_MapX,Npc[nIdx].m_MapY);
			}

			if (NpcSync->Doing == do_stand)
				Npc[nIdx].ProcNetCommand(do_stand);
		}
		else
		{
			if (Npc[nIdx].m_RegionIndex != nRegion && nIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)
			{//��ͬ��ͼ����Ĺ���
				SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].RemoveNpc(nIdx);
				SubWorld[0].m_Region[Npc[nIdx].m_RegionIndex].DecNpcRef(Npc[nIdx].m_MapX, Npc[nIdx].m_MapY);

				Npc[nIdx].m_MapX = nMapX;
				Npc[nIdx].m_MapY = nMapY;
				Npc[nIdx].m_OffX = nOffX;
				Npc[nIdx].m_OffY = nOffY;
				Npc[nIdx].m_RegionIndex = nRegion;

				if (nRegion >= 0)
				{
					SubWorld[0].m_Region[nRegion].AddNpc(nIdx);
					Npc[nIdx].m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
					SubWorld[0].m_Region[nRegion].AddNpcRef(Npc[nIdx].m_MapX,Npc[nIdx].m_MapY);
				}
			}
		}
        //����

		if (nIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)	// ����ұ���
		{
			Npc[nIdx].m_CurrentLifeMax = NpcSync->nCurrentLifeMax;

			LONG	nOldLife = Npc[nIdx].m_CurrentLife;         //��һ�ε�Ѫ��

			//Npc[nIdx].m_CurrentLife = (Npc[nIdx].m_CurrentLifeMax * NpcSync->LifePerCent) >> 7;

			if (NpcSync->nLifeYu >=0 && Npc[nIdx].m_CurrentLifeMax > 0)
			{
				Npc[nIdx].m_CurrentLife=Npc[nIdx].m_CurrentLifeMax-NpcSync->nLifeYu;//TakeRemainder(m_CurrentLifeMax,m_CurrentLife);

				if (Npc[nIdx].m_CurrentLife<0)
					Npc[nIdx].m_CurrentLife=0;
			}

			if  (Npc[nIdx].m_CurrentLife<0)
			{
				Npc[nIdx].m_CurrentLife = 0;
			}
               Npc[nIdx].m_CurrentLifeDamage  = NpcSync->nLifeDamagePerCent;
			// ðѪ����kind_partner
			if (Npc[nIdx].m_Kind == kind_normal || Npc[nIdx].m_Kind == kind_partner)  //����  ͬ��Ѫ��  ����ðѪ
			{
			     if (nOldLife - Npc[nIdx].m_CurrentLife>0)
				     Npc[nIdx].SetBlood(nOldLife - Npc[nIdx].m_CurrentLife);// nOldLife - Npc[nIdx].m_CurrentLife);NpcSync->nLifeDamagePerCent  //��ʾ���ٵ���������
			}

			Npc[nIdx].m_CurrentCamp = NpcSync->Camp;

			if  (Npc[nIdx].m_IsSynDir>=1)
			     Npc[nIdx].m_Dir=NpcSync->nDir;

		}

		Npc[nIdx].m_FreezeState.nTime  = NpcSync->nState & STATE_FREEZE;
		//��λ������� �Ƚ�������������Ӧ�Ķ�����λ��������������λ��Ϊ1ʱ����λ�Ľ��ȡ1������ȡ0��
		//Npc[nIdx].m_PoisonState.nTime  = NpcSync->State & STATE_POISON;
		Npc[nIdx].m_PoisonState.nTime  = NpcSync->nState & STATE_POISON;

		Npc[nIdx].m_ConfuseState.nTime = NpcSync->nState & STATE_CONFUSE;
		Npc[nIdx].m_StunState.nTime    = NpcSync->nState & STATE_STUN;
		Npc[nIdx].m_Hide.nTime         = NpcSync->nState & STATE_HIDE;

		Npc[nIdx].m_randmove.nTime     = NpcSync->nIsMove;

		Npc[nIdx].m_ZhuaVal            = NpcSync->nZhuaVal;
        Npc[nIdx].m_TongBanNum         = NpcSync->nTongBanNum;
		Npc[nIdx].m_GameliveTime       = NpcSync->mliveTime;

		if (Npc[nIdx].m_CurrentAttackSpeed != NpcSync->ASpeed)
			Npc[nIdx].m_CurrentAttackSpeed = NpcSync->ASpeed;   //�⹦����
		if (Npc[nIdx].m_CurrentCastSpeed != NpcSync->CSpeed)
			Npc[nIdx].m_CurrentCastSpeed = NpcSync->CSpeed;     //�ڹ�����


		if (NpcSync->nReForm)
			Npc[nIdx].SetNpcIsReFrame(1,NpcSync->nReForm);

		if (Npc[nIdx].GetAttackFrame() != NpcSync->nAttackFrame)
			Npc[nIdx].SetAttackFrame(NpcSync->nAttackFrame);     //�⹦����֡��
		if (Npc[nIdx].GetCastFrame() != NpcSync->nCastFrame)
			Npc[nIdx].SetCastFrame(NpcSync->nCastFrame);         //�ڹ�����֡��

	    if (Npc[nIdx].m_CurrentdanggeRate != NpcSync->Dangge)    //����
	    	Npc[nIdx].m_CurrentdanggeRate  = NpcSync->Dangge;

		if(Npc[nIdx].m_CurrentzhongjiRate  !=NpcSync->Zhongji)
            Npc[nIdx].m_CurrentzhongjiRate  =NpcSync->Zhongji;

		Npc[nIdx].m_btRankFFId=NpcSync->RankFFID;
		Npc[nIdx].m_NpcTitle=NpcSync->nNpcTitle;
		Npc[nIdx].m_CurNpcTitle=NpcSync->nCurNpcTitle;
		Npc[nIdx].IsLuKey=NpcSync->AIsLukey;               //�����
        Npc[nIdx].IsJinYan=NpcSync->AIsJinYan;             //�����
        Npc[nIdx].m_GoldLucky=NpcSync->gsLukey;
		Npc[nIdx].m_Experience=NpcSync->nExpPer;
		Npc[nIdx].IsJinQian=NpcSync->AIsJinQian;           //���Ǯ

        Npc[nIdx].m_IsXingYunXing=NpcSync->IsXingYunXing;  //������

         Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;

		// Npc[nIdx].SetstrInfo(STR_GUISHU_NAME,NpcSync->GuishuName);
		 sprintf(Npc[nIdx].Name,"%s",NpcSync->nName);             //����
         sprintf(Npc[nIdx]._clientName,"%s",NpcSync->_clientName);
         Npc[nIdx].m_GuiShuDwid=NpcSync->nGuiShuDwid;
		 Npc[nIdx].m_MaskType=NpcSync->nMaskType;
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣͬ�������npc���� ����ѭ���������
//-------------------------------------------------------------------------
void KProtocolProcess::SyncNpcMinPlayer(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_PLAYER_TYPE_NORMAL_SYNC	*pSync = (NPC_PLAYER_TYPE_NORMAL_SYNC*)pMsg;

	//_ASSERT(pSync->m_dwNpcID == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID);

	if (pSync->m_dwNpcID != Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
		return;

	int nRegion, nMapX, nMapY, nOffX, nOffY, nNpcIdx;

	SubWorld[0].Mps2Map(pSync->m_dwMapX, pSync->m_dwMapY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

	nNpcIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if  (nNpcIdx<=0 || nNpcIdx>=MAX_NPC)
		return;

	Player[CLIENT_PLAYER_INDEX].m_GameLeftTime=pSync->m_GameLeftTiem;

	// �л���ͼ�ˣ����ǵ�RegionIndexΪ-1
	if (Npc[nNpcIdx].m_RegionIndex == -1)
	{
		if (nRegion < 0)
		{
			// �ոջ��ĵ�ͼ�����ʱ��Ӧ�����ҵõ�region������Ҳ�����˵�����ݰ����ܳ�����
			return;
		}

		Npc[nNpcIdx].m_RegionIndex = nRegion;
		Npc[nNpcIdx].m_dwRegionID = SubWorld[0].m_Region[nRegion].m_RegionID;
		//pSync->m_nRegionID;
		SubWorld[0].NpcChangeRegion(-1, SubWorld[0].m_Region[nRegion].m_RegionID, nNpcIdx);
		SubWorld[0].m_Region[nRegion].AddNpcRef(nMapX,nMapY);

		Npc[nNpcIdx].m_MapX = nMapX;
		Npc[nNpcIdx].m_MapY = nMapY;
		Npc[nNpcIdx].m_OffX = pSync->m_wOffX;
		Npc[nNpcIdx].m_OffY = pSync->m_wOffY;
		memset(&Npc[nNpcIdx].m_sSyncPos, 0, sizeof(Npc[nNpcIdx].m_sSyncPos));
		Npc[nNpcIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
		g_ScenePlace.setIsPaint(true);
		return;
	}

	// ��̫Զ������9����Χ
	//nRegionIdx = SubWorld[0].FindRegion(pSync->m_nRegionID);
	if (nRegion == -1)
	{
		SubWorld[0].m_Region[Npc[nNpcIdx].m_RegionIndex].DecNpcRef(Npc[nNpcIdx].m_MapX, Npc[nNpcIdx].m_MapY);
		int nRegionX = pSync->m_dwMapX/(SubWorld[0].m_nCellWidth * SubWorld[0].m_nRegionWidth);
		int nRegionY = pSync->m_dwMapY/(SubWorld[0].m_nCellHeight * SubWorld[0].m_nRegionHeight);

		DWORD dwRegionID = MAKELONG(nRegionX, nRegionY);
		//messageBox("Player start add map:LoadMapC","SyncNpcMinPlayer");
		SubWorld[0].LoadMapC(SubWorld[0].m_SubWorldID, dwRegionID);
		nRegion = SubWorld[0].FindRegion(dwRegionID);
		if (nRegion<0)
		{
			return;
		}
		Npc[nNpcIdx].m_RegionIndex = nRegion;
		Npc[nNpcIdx].m_dwRegionID  = dwRegionID;//pSync->m_nRegionID;
		SubWorld[0].NpcChangeRegion(-1, SubWorld[0].m_Region[nRegion].m_RegionID, nNpcIdx);

		SubWorld[0].Mps2Map(pSync->m_dwMapX, pSync->m_dwMapY, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
		Npc[nNpcIdx].m_MapX = nMapX;
		Npc[nNpcIdx].m_MapY = nMapY;
		Npc[nNpcIdx].m_OffX = pSync->m_wOffX;
		Npc[nNpcIdx].m_OffY = pSync->m_wOffY;

		SubWorld[0].m_Region[Npc[nNpcIdx].m_RegionIndex].AddNpcRef(Npc[nNpcIdx].m_MapX,Npc[nNpcIdx].m_MapY);
		memset(&Npc[nNpcIdx].m_sSyncPos, 0, sizeof(Npc[nNpcIdx].m_sSyncPos));
		Npc[nNpcIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
		g_ScenePlace.setIsPaint(true);
		return;
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣ���һ��obj
//-------------------------------------------------------------------------
void KProtocolProcess::SyncObjectAdd(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	OBJ_ADD_SYNC	*pObjSyncAdd = (OBJ_ADD_SYNC*)pMsg;
	int				nObjIndex;

	KCObjItemInfo	sInfo;
    memset(&sInfo, 0, sizeof(sInfo));

	nObjIndex = ObjSet.FindID(pObjSyncAdd->m_nID);
	if (nObjIndex > 0)
		return;

	sInfo.m_nItemID     = pObjSyncAdd->m_nItemID;
	sInfo.m_nItemWidth  = pObjSyncAdd->m_btItemWidth;
	sInfo.m_nItemHeight = pObjSyncAdd->m_btItemHeight;
	sInfo.m_nMoneyNum   = pObjSyncAdd->m_nMoneyNum;
	sInfo.m_nColorID    = pObjSyncAdd->m_btColorID;
	sInfo.m_nMovieFlag  = ((pObjSyncAdd->m_btFlag & 0x02) > 0 ? 1 : 0);
	sInfo.m_nSoundFlag  = ((pObjSyncAdd->m_btFlag & 0x01) > 0 ? 1 : 0);

    sInfo.m_cHaveAttack=  ((pObjSyncAdd->m_btFlag & 0x04) > 0 ? 1 : 0);
	//sInfo.m_cAttackerDwid=0;// pObjSyncAdd->m_nAttackerDwid;
	DWORD m_cAttackerDwid=0;

    if (sInfo.m_cHaveAttack)
	{
       m_cAttackerDwid=Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID;
	}
	else
       m_cAttackerDwid=0;


	sInfo.m_Genre          =pObjSyncAdd->m_Genre;
	sInfo.m_DetailType     =pObjSyncAdd->m_DetailType;
	sInfo.m_ParticularType =pObjSyncAdd->m_ParticularType;
	sInfo.m_GoldId         =pObjSyncAdd->m_GoldId;
	sInfo.m_ItemLevel      =pObjSyncAdd->m_ItemLevel;
	sInfo.m_StackNum      =pObjSyncAdd->m_StackNum;

	memset(sInfo.m_szName, 0, sizeof(sInfo.m_szName));
	memset(sInfo.m_gsName, 0, sizeof(sInfo.m_gsName));
	//sprintf(sInfo.m_gsName,"%s",pObjSyncAdd->_clientOgjName);
	//sprintf(sInfo.m_szName,"%s",pObjSyncAdd->m_szName);
	int nTempLen=pObjSyncAdd->m_wLength + 1 + sizeof(pObjSyncAdd->m_szName) - sizeof(OBJ_ADD_SYNC);//+sizeof(pObjSyncAdd->_clientOgjName)-strlen(pObjSyncAdd->_clientOgjName)
	memcpy(sInfo.m_szName,pObjSyncAdd->m_szName,nTempLen);
//�ͻ��˲�����Ʒ
	nObjIndex = ObjSet.ClientAdd(
		pObjSyncAdd->m_nID,\
		pObjSyncAdd->m_nDataID,\
		pObjSyncAdd->m_btState,\
		pObjSyncAdd->m_btDir,\
		pObjSyncAdd->m_wCurFrame,\
		pObjSyncAdd->m_nXpos,\
		pObjSyncAdd->m_nYpos,\
		sInfo,m_cAttackerDwid);
}

//���θ��·��������
void KProtocolProcess::s2cobjadd_vn(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	OBJ_ADD_SYNC_VN	*pObjSyncAdd = (OBJ_ADD_SYNC_VN*)pMsg;
	int nObjIndex = ObjSet.FindID(pObjSyncAdd->m_nID);
	if (nObjIndex>0)
	{//��ʼ���÷��������memcpy
		//int nTempLen=pObjSyncAdd->m_wLength + 1 + sizeof(pObjSyncAdd->_clientOgjName) - sizeof(OBJ_ADD_SYNC_VN);//+sizeof(pObjSyncAdd->_clientOgjName)-strlen(pObjSyncAdd->_clientOgjName)
		sprintf(KObject[nObjIndex]._clientObjName,  "%s", pObjSyncAdd->_clientOgjName);
	}
}
//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣ����ĳ��obj����
//-------------------------------------------------------------------------
void KProtocolProcess::SyncObjectDir(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	OBJ_SYNC_DIR	*pObjSyncDir = (OBJ_SYNC_DIR*)pMsg;
	int				nObjIndex;
	nObjIndex = ObjSet.FindID(pObjSyncDir->m_nID);
	if (nObjIndex <= 0)
	{
		// ����������������
		OBJ_CLIENT_SYNC_ADD	sObjClientSyncAdd;
		sObjClientSyncAdd.ProtocolType = c2s_requestobj;
		sObjClientSyncAdd.m_nID = pObjSyncDir->m_nID;
		if (g_pClient)
			g_pClient->SendPackToServer(&sObjClientSyncAdd, sizeof(sObjClientSyncAdd));
	}
	else
	{	// ͬ������
		KObject[nObjIndex].SetDir(pObjSyncDir->m_btDir);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣɾ��ĳ��obj
//-------------------------------------------------------------------------
void KProtocolProcess::SyncObjectRemove(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	OBJ_SYNC_REMOVE	*pObjSyncRemove = (OBJ_SYNC_REMOVE*)pMsg;
	int				nObjIndex;
	nObjIndex = ObjSet.FindID(pObjSyncRemove->m_nID);
	if (nObjIndex > 0)
	{	// ɾ��
		KObject[nObjIndex].Remove(pObjSyncRemove->m_btSoundFlag);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣ����ĳ��obj״̬
//-------------------------------------------------------------------------
void KProtocolProcess::SyncObjectState(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	OBJ_SYNC_STATE	*pObjSyncState = (OBJ_SYNC_STATE*)pMsg;
	int				nObjIndex;
	nObjIndex = ObjSet.FindID(pObjSyncState->m_nID);
	if (nObjIndex <= 0)
	{
		// ����������������
		OBJ_CLIENT_SYNC_ADD	sObjClientSyncAdd;
		sObjClientSyncAdd.ProtocolType = c2s_requestobj;
		sObjClientSyncAdd.m_nID = pObjSyncState->m_nID;
		if (g_pClient)
			g_pClient->SendPackToServer(&sObjClientSyncAdd, sizeof(sObjClientSyncAdd));
	}
	else
	{	// ͬ��״̬
		if (KObject[nObjIndex].m_nRegionIdx == -1)
		{//����ǲ��������ڵĻ�,������һ��
			int	nRegion;
			nRegion = SubWorld[0].FindRegion(KObject[nObjIndex].m_nBelongRegion);

			if (nRegion >= 0)
			{
				KObject[nObjIndex].m_nRegionIdx = nRegion;
				SubWorld[0].m_Region[nRegion].AddObj(nObjIndex);
			}
		}

		KObject[nObjIndex].SetState(pObjSyncState->m_btState);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣͬ��ĳ�� trap �� obj ������
//-------------------------------------------------------------------------
void KProtocolProcess::SyncObjectTrap(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	OBJ_SYNC_TRAP_ACT *pObjTrapSync = (OBJ_SYNC_TRAP_ACT*)pMsg;
	int		nObjIndex;
	nObjIndex = ObjSet.FindID(pObjTrapSync->m_nID);
	if (nObjIndex <= 0)
	{
		// ����������������
		OBJ_CLIENT_SYNC_ADD	sObjClientSyncAdd;
		sObjClientSyncAdd.ProtocolType = c2s_requestobj;
		sObjClientSyncAdd.m_nID = pObjTrapSync->m_nID;
		if (g_pClient)
			g_pClient->SendPackToServer(&sObjClientSyncAdd, sizeof(sObjClientSyncAdd));
	}
	else
	{
		KObject[nObjIndex].m_nState = OBJ_TRAP_STATE_ACTING;
		KObject[nObjIndex].m_cImage.SetDirStart();
		KObject[nObjIndex].m_cSkill.m_nTarX = pObjTrapSync->m_nTarX;
		KObject[nObjIndex].m_cSkill.m_nTarY = pObjTrapSync->m_nTarY;
	}
}
//����ͬ�����--������Ϣ- ���ݿ�-�ͻ���
void KProtocolProcess::SyncPlayer(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	//messageBox("online sync server","SyncPlayer");
	PLAYER_SYNC*	pPlaySync = (PLAYER_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pPlaySync->ID);

	if  (nIdx<=0)
		return;

	Npc[nIdx].m_ArmorType			= pPlaySync->ArmorType;
	Npc[nIdx].m_HelmType			= pPlaySync->HelmType;
	Npc[nIdx].m_HorseType			= pPlaySync->HorseType;
	Npc[nIdx].m_CurrentRunSpeed		= pPlaySync->RunSpeed;
	Npc[nIdx].m_CurrentWalkSpeed	= pPlaySync->WalkSpeed;
	Npc[nIdx].m_WeaponType			= pPlaySync->WeaponType;
	Npc[nIdx].m_Kind				= kind_player;
	Npc[nIdx].m_btRankId			= pPlaySync->RankID;
	Npc[nIdx].m_btRankFFId			= pPlaySync->RankFFID;
    Npc[nIdx].m_CJtaskID            = pPlaySync->mCJtaskID;      //ת��
	Npc[nIdx].m_ExItemId			= pPlaySync->ExItemID; // hanh trang
	Npc[nIdx].m_ExBoxId				= pPlaySync->ExBoxID; // ruong mo rong
	Npc[nIdx].nRankInWorld			= pPlaySync->RankInWorld;
	Npc[nIdx].nRepute				= pPlaySync->Repute;
	Npc[nIdx].nPKValue				= pPlaySync->PKValue;
	Npc[nIdx].nReBorn				= pPlaySync->ReBorn;

	Npc[nIdx].m_MaskType            = pPlaySync->MaskType;	    // mat na
	Npc[nIdx].m_PifengType          = pPlaySync->PifengType;	// ����
	Npc[nIdx].m_nFigure				= pPlaySync->m_bFigure;

	//if (Npc[nIdx].m_nFigure != -1)
	{
		//strcpy(Npc[nIdx].TongName,pPlaySync->TongName);   //�����
		sprintf(Npc[nIdx].TongName, "%s", pPlaySync->TongName);
	}
	if (Npc[nIdx].m_BaiTan != pPlaySync->m_bBaiTan && pPlaySync->m_bBaiTan)
	{//�������Ҫ����°�̯����
		SendClientCmdShopName(Npc[nIdx].m_dwID);
	}
	Npc[nIdx].m_BaiTan	= pPlaySync->m_bBaiTan;

	BOOL bChange = FALSE;

	if (Npc[nIdx].m_HorseType == -1)
	{
		if (Npc[nIdx].m_bRideHorse)
			bChange = TRUE;
		Npc[nIdx].m_bRideHorse = FALSE;
	}
	else
	{
		if (!Npc[nIdx].m_bRideHorse)
			bChange = TRUE;
		Npc[nIdx].m_bRideHorse = TRUE;
	}

	if (pPlaySync->m_btSomeFlag & 0x01)
		Npc[nIdx].m_nPKFlag			= 0;
	else if (pPlaySync->m_btSomeFlag & 0x10)
		Npc[nIdx].m_nPKFlag			= 1;
	else
		Npc[nIdx].m_nPKFlag			= 2;

	if (pPlaySync->m_btSomeFlag & 0x02)
		Npc[nIdx].m_FightMode		= 1;
	else
		Npc[nIdx].m_FightMode		= 0;
	if (pPlaySync->m_btSomeFlag & 0x04)
		Npc[nIdx].SetSleepMode(1);
	else
		Npc[nIdx].SetSleepMode(0);

	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex && bChange)
	{
		int nActiveId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_horse);

		int nActiveInfo[2];

		if (nActiveId)
			if (Npc[nIdx].m_bRideHorse)
			{

				ZeroMemory(nActiveInfo,sizeof(nActiveInfo));

				if (Item[nIdx].IsBlue())
				{//�������װ
					nActiveInfo[0] = 1;
					nActiveInfo[1] = Item[nActiveId].GetIsWhere();

				}
				else if  (Item[nIdx].IsPurple())
				{//�������װ
					nActiveInfo[0] = 2;
					nActiveInfo[1] = Item[nActiveId].GetIsWhere();
				}
				else if  (Item[nIdx].IsGold())
				{//����ǻ�װ
					nActiveInfo[0] = 3;
					nActiveInfo[1] = Item[nActiveId].GetLevel();//�ȼ�

				}
				else
				{
					nActiveInfo[0] = 0;
					nActiveInfo[1] = 0;
				}

				Item[nActiveId].ApplyMagicAttribToNPC(&Npc[nIdx],0,nActiveInfo);  // ����ͼ�����
			}
			else if (!Npc[nIdx].m_bRideHorse)
			{
				if  (Item[nIdx].IsGold())
				{//����ǻ�װ
					nActiveInfo[0] = 3;
					nActiveInfo[1] = Item[nActiveId].GetLevel();//�ȼ�

				}
				else
				{
					nActiveInfo[0] = 0;
					nActiveInfo[1] = 0;
				}

				Item[nActiveId].RemoveMagicAttribFromNPC(&Npc[nIdx],0,nActiveInfo); //�����ȥ������
			}
	}
}
//ʵʱͬ�����--������ʱ����-�ͻ���
void KProtocolProcess::SyncPlayerMin(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_NORMAL_SYNC* pPlaySync = (PLAYER_NORMAL_SYNC *)pMsg;

	int nIdx = NpcSet.SearchID(pPlaySync->ID);
	//char mgs[64]={0};
	if (nIdx<=0)
	{
		//sprintf(mgs,"���ݴ���:%d....",pPlaySync->ID);
		//messageBox(mgs,"SyncPlayerMin");
		return;
	}
	Npc[nIdx].m_bLockNpcDwID	    = pPlaySync->m_LockNpcDwID;
	Npc[nIdx].m_CurrentRunSpeed		= pPlaySync->RunSpeed;
	Npc[nIdx].m_CurrentWalkSpeed	= pPlaySync->WalkSpeed;
	Npc[nIdx].m_HelmType			= pPlaySync->HelmType;
	Npc[nIdx].m_ArmorType			= pPlaySync->ArmorType;
	Npc[nIdx].m_HorseType			= pPlaySync->HorseType;
	Npc[nIdx].m_WeaponType			= pPlaySync->WeaponType;
	Npc[nIdx].m_Kind				= kind_player;
	Npc[nIdx].m_btRankId			= pPlaySync->RankID;
	Npc[nIdx].m_btRankFFId			= pPlaySync->RankFFID;
	Npc[nIdx].m_AutoplayId			= pPlaySync->AutoplayId;
	Npc[nIdx].m_IsWarCity			= pPlaySync->mIsWarCity;   // ʵʱͬ����ҳ��й���
    Npc[nIdx].m_nsPlayerIdx         = pPlaySync->msPlayerIdx;
    Npc[nIdx].m_CJtaskID            = pPlaySync->mCJtaskID;    //ת��
	Npc[nIdx].m_IsSerLock           = pPlaySync->nIsSerLock;
	Npc[nIdx].m_ExItemId			= pPlaySync->ExItemID;      // hanh trang
	Npc[nIdx].m_ExBoxId				= pPlaySync->ExBoxID;       // ruong mo rong
	Npc[nIdx].nRankInWorld			= pPlaySync->RankInWorld;
	Npc[nIdx].nRepute				= pPlaySync->Repute;
	Npc[nIdx].nPKValue				= pPlaySync->PKValue;        //PKֵ
	Npc[nIdx].nReBorn				= pPlaySync->ReBorn;
	Npc[nIdx].m_IsInCity	        = pPlaySync->m_IsInCity;

	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
	{
        Player[CLIENT_PLAYER_INDEX].m_nVipType=pPlaySync->nVipType;
		Npc[nIdx].m_MapUseModel.nTime=pPlaySync->nIsOpenMapType;
		//Player[CLIENT_PLAYER_INDEX].m_cTong.m_AttackState=pPlaySync->mAttackState;
		Player[CLIENT_PLAYER_INDEX].m_IsTranse = pPlaySync->nIsTranse;//�Ƿ�����
		Player[CLIENT_PLAYER_INDEX].m_CurGsSerIdx = pPlaySync->nCurGsSerIdx;
	}
	//Npc[nIdx].m_MaskType			= pPlaySync->MaskType;
    Npc[nIdx].m_PifengType          = pPlaySync->PifengType;	 // ����
	Npc[nIdx].m_ChiBangType         = pPlaySync->nChiBangType;

    Npc[nIdx].IsCreatTongBan        = pPlaySync->mIsCreatTongBan;//�Ƿ��Ѿ���ͬ��
    Npc[nIdx].m_IsVip               = pPlaySync->IsVip;
	Npc[nIdx].m_nFigure		        = pPlaySync->m_bFigure;
	strcpy(Npc[nIdx].TongName,pPlaySync->TongName); //�����

	if (Npc[nIdx].m_BaiTan != pPlaySync->m_bBaiTan && pPlaySync->m_bBaiTan)
	{//ֻͬ��һ�Σ�
		SendClientCmdShopName(Npc[nIdx].m_dwID);        //ʵʱͬ����̯��
	}

	Npc[nIdx].m_BaiTan	= pPlaySync->m_bBaiTan;

	if (Npc[nIdx].m_BaiTan)
	   Npc[nIdx].SetstrInfo(STR_SHOP_NAME,pPlaySync->ShopName);

	BOOL bChange = FALSE;
	Npc[nIdx].m_JinMaiBingJia=pPlaySync->mJinMaiBingJia;
    Npc[nIdx].m_ZhenYuan=pPlaySync->mZhenYuan;

	if (Npc[nIdx].m_HorseType == -1)
	{
		if (Npc[nIdx].m_bRideHorse)
			bChange = TRUE;
		Npc[nIdx].m_bRideHorse = FALSE;
	}
	else
	{
		if (!Npc[nIdx].m_bRideHorse)
			bChange = TRUE;
		Npc[nIdx].m_bRideHorse = TRUE;
	}

	if (pPlaySync->m_btSomeFlag & 0x01)
		Npc[nIdx].m_nPKFlag			= 0;
	else if (pPlaySync->m_btSomeFlag & 0x10)
		Npc[nIdx].m_nPKFlag			= 1;
	else
		Npc[nIdx].m_nPKFlag			= 2;

	if (pPlaySync->m_btSomeFlag & 0x02)
		Npc[nIdx].m_FightMode		= 1;  //ս��ģʽ
	else
		Npc[nIdx].m_FightMode		= 0;

	if (pPlaySync->m_btSomeFlag & 0x04)
		Npc[nIdx].SetSleepMode(1);
	else
		Npc[nIdx].SetSleepMode(0);

	if (pPlaySync->m_btSomeFlag & 0x08)
		Npc[nIdx].m_nTongFlag		= 1;  //������Ȩ��
	else
		Npc[nIdx].m_nTongFlag		= 0;  //������Ȩ��

	if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex && bChange)
	{
		int nActiveId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_horse);
		int nActiveInfo[2];

		if (nActiveId)

			if (Npc[nIdx].m_bRideHorse)
			{

				ZeroMemory(nActiveInfo,sizeof(nActiveInfo));

				if (Item[nIdx].IsBlue())
				{//�������װ
					nActiveInfo[0] = 1;
					nActiveInfo[1] = Item[nActiveId].GetIsWhere();

				}
				else if  (Item[nIdx].IsPurple())
				{//�������װ
					nActiveInfo[0] = 2;
					nActiveInfo[1] = Item[nActiveId].GetIsWhere();
				}
				else if  (Item[nIdx].IsGold())
				{//����ǻ�װ
					nActiveInfo[0] = 3;
					nActiveInfo[1] = Item[nActiveId].GetLevel();//�ȼ�
				}
				else
				{
					nActiveInfo[0] = 0;
					nActiveInfo[1] = 0;
				}

				Item[nActiveId].ApplyMagicAttribToNPC(&Npc[nIdx],0,nActiveInfo);  //Ӧ��ħ�����Ե� NPC����
			}
			else if (!Npc[nIdx].m_bRideHorse)
			{

				if  (Item[nIdx].IsGold())
				{//����ǻ�װ
					nActiveInfo[0] = 3;
					nActiveInfo[1] = Item[nActiveId].GetLevel();//�ȼ�
				}
				else
				{
					nActiveInfo[0] = 0;
					nActiveInfo[1] = 0;
				}
				Item[nActiveId].RemoveMagicAttribFromNPC(&Npc[nIdx],0,nActiveInfo); // �Ƴ�ħ������
			}
	}

	if  (Npc[nIdx].m_Kind ==kind_player)
	{//ͬ����� ��ʼ�滭
		//g_ScenePlace.setIsPaint(true);
	}
}

void KProtocolProcess::SyncScriptAction(BYTE* pMsg)
{//�ͻ��˴򿪽���
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].OnScriptAction((PLAYER_SCRIPTACTION_SYNC *)pMsg);
}
//ͬ�������ͼ��---�ͻ�����
void KProtocolProcess::SyncWorld(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	 WORLD_SYNC *WorldSync = (WORLD_SYNC *)pMsg;

	  if (Player[CLIENT_PLAYER_INDEX].m_nIndex > 0) //����ǿͻ���
	  {
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendSerCommand(do_stand);
	   }

	  if (SubWorld[0].m_SubWorldID != WorldSync->SubWorld)
	  {//��ͬ��ͼ,�����������ͼ
		//messageBox("add map LoadMapC","SyncWorld");
		SubWorld[0].LoadMapC(WorldSync->SubWorld, WorldSync->SRegion);
	  }
	  SubWorld[0].m_dwCurrentTime = WorldSync->Frame;
	  SubWorld[0].m_IsShowLoop    = WorldSync->m_IsShowLoop;
	  char nMapType[32]={0},mMapType[32]={0};

      sprintf(nMapType,"%d_MapType",WorldSync->SubWorld);
      g_NpcMapDropRate.GetString("List",nMapType,"��",mMapType,sizeof(mMapType));
	  SubWorld[0].nWorldMapInfo[STR_MAP_MASTER]=WorldSync->WarMaster;

	  if (strstr(mMapType,"City"))
	  {
        SubWorld[0].m_WarIsWho     =WorldSync->WarIsWho;
        SubWorld[0].m_IswarCity    =WorldSync->IsWarCity;      // �Ƿ�ռ��
	    SubWorld[0].m_ShuiShouType =WorldSync->mShuiType;
	    SubWorld[0].m_WarCityManey =WorldSync->WarCityManey;   //���н����ʽ�
        SubWorld[0].m_WarCityJB    =WorldSync->WarCityJB;      //���н����ʽ�
        SubWorld[0].m_WarCityGX    =WorldSync->WarCityGX;      //���н��蹱��
      }

	  SubWorld[0].m_nWPKFlag=WorldSync->WPKFlag; //��ͼ��pkģʽ����
	// ͬ������
	  SubWorld[0].m_nWeather = WorldSync->Weather;
//	  g_ScenePlace.ChangeWeather(WorldSync->Weather);
	  //messageBox("add finish...","SyncWorld");
}

//-------------------------------------------------------------------------
//	���ܣ��յ���������Ϣĳ�����ֹ���뱾�ͻ��˵ĺ��ѹ�ϵ
//-------------------------------------------------------------------------
void	KProtocolProcess::s2cChatDeleteFriend(BYTE* pMsg)
{
	/*
	if  (NULL==pMsg||NULL==this) return;
	CHAT_DELETE_FRIEND_SYNC	*pDelete = (CHAT_DELETE_FRIEND_SYNC*)pMsg;
	CChatFriend	*pFriend;
	for (int i = 0; i < MAX_FRIEND_TEAM; i++)
	{
		pFriend = (CChatFriend*)Player[CLIENT_PLAYER_INDEX].m_cChat.m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pFriend)
		{
			if (pFriend->m_dwID == pDelete->m_dwID)
			{
				pFriend->Remove();
				Player[CLIENT_PLAYER_INDEX].m_cChat.m_cFriendTeam[i].m_nFriendNo--;

				// ֪ͨ���� ĳ�����ֹ���뱾�ͻ��˵ĺ��ѹ�ϵ
//				CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
				CoreDataChanged(GDCNI_CHAT_FRIEND, 0, i);

				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CHAT_DELETED_FRIEND, pFriend->m_szName);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);

				delete pFriend;
					pFriend=NULL;
				return;
			}
			pFriend = (CChatFriend*)pFriend->GetNext();
		}
	}*/
}

//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨĳ�����������
//-------------------------------------------------------------------------
void	KProtocolProcess::s2cChatFriendOffLine(BYTE* pMsg)
{
	//if  (NULL==pMsg||NULL==this) return;
	//CHAT_FRIEND_OFFLINE_SYNC	*pSync = (CHAT_FRIEND_OFFLINE_SYNC*)pMsg;
	//Player[CLIENT_PLAYER_INDEX].m_cChat.GetMsgOffLine(pSync->m_dwID);
}
//�ͻ���ͬ������
void	KProtocolProcess::s2cSyncAllSkill(BYTE * pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	SKILL_SEND_ALL_SYNC	* pSync = (SKILL_SEND_ALL_SYNC*) pMsg;
	int nSkillCount = (pSync->m_wProtocolLong - 2) / sizeof(SKILL_SEND_ALL_SYNC_DATA);

	int nNpcIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if  (nNpcIndex<=0 || nNpcIndex>=MAX_NPC)
		return;

	KSkill * pOrdinSkill = NULL;

	Npc[nNpcIndex].m_SkillList.Clear();

	for (int i = 0; i < nSkillCount; i ++)
	{
		if (pSync->m_sAllSkill[i].SkillId)
		{

//			pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(pSync->m_sAllSkill[i].SkillId, pSync->m_sAllSkill[i].SkillLevel);
//			if (!pOrdinSkill)
//              continue;
			Npc[nNpcIndex].m_SkillList.Add(\
				pSync->m_sAllSkill[i].SkillId,\
				pSync->m_sAllSkill[i].SkillLevel,\
				0,\
				0,\
				pSync->m_sAllSkill[i].SkillAdd,\
                pSync->m_sAllSkill[i].CurSkillExp);
		}
	}
}

void KProtocolProcess::SyncClientEnd(BYTE* pMsg)
{
	//֪ͨ������ �Ѿ�ͬ��������
	if  (NULL==pMsg||NULL==this) return;
	CLIENT_SYNC_END* nNpcSync = (CLIENT_SYNC_END *)pMsg;

	if (!nNpcSync->m_IsLogin)
	{
		messageBox("Login gameworld Error","Login gameworld");
		return;
	}
	if  (curMobileKey!=nNpcSync->m_clientKey){

		CoreDataChanged(GDCNI_GAME_QUIT,0,0);
		return;
	}

	Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("��½�ɹ�!");
	CLIENT_SYNC_END nSerSync;
	nSerSync.ProtocolType = (BYTE)c2s_syncclientend;
	nSerSync.m_IsLogin    = nNpcSync->m_IsLogin;
	nSerSync.m_clientKey  = curMobileKey;

	/*BYTE SyncEnd = (BYTE)c2s_syncclientend;
	if (g_pClient)
		g_pClient->SendPackToServer(&SyncEnd, sizeof(BYTE));
	*/
	if (g_pClient)
		g_pClient->SendPackToServer(&nSerSync, sizeof(CLIENT_SYNC_END));

	m_bIsPlayerInGame = true;

	//����Ĭ�Ͽ�ݼ���
	Player[CLIENT_PLAYER_INDEX].SetDefaultImmedSkill();
	//ͬ�����ֻ��ͻ��˵�½
	PLAYER_REQUEST_CP_LOCK LockCmd;
	LockCmd.ProtocolType = (BYTE)c2s_cplock;
	LockCmd._isMobile =1; //�Ƿ��ֻ���
	LockCmd._clientVer=_clientlanguage;//�ͻ��˰汾 1 �й� 2Խ�� ���� Ӣ��
	if (g_pClient)
		g_pClient->SendPackToServer(&LockCmd, sizeof(PLAYER_REQUEST_CP_LOCK));
//	g_bPingReply = TRUE;
	// �趨���충��Ƶ��
//	Player[CLIENT_PLAYER_INDEX].m_cChat.LoadTeamName(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
//	Player[CLIENT_PLAYER_INDEX].m_cChat.SaveTeamInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
//	Player[CLIENT_PLAYER_INDEX].m_cChat.LoadTakeChannelInfo(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
    //����һ��״̬
	//Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetInstantSpr(enumINSTANT_STATE_LOGIN); //�ͷ�һ����½��˲����Ч
	// ��ʼ������
	//g_SubWorldSet.m_cMusic.Start(SubWorld[0].m_SubWorldID, SubWorld[0].m_dwCurrentTime, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode);
	// PK��ʼ��
	Player[CLIENT_PLAYER_INDEX].m_cPK.Init();

	CoreDataChanged(GDCNI_GAME_START,0,0);
}

void	KProtocolProcess::s2cTradeChangeState(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].s2cTradeChangeState(pMsg);
}

void	KProtocolProcess::s2cNpcSetMenuState(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_SET_MENU_STATE_SYNC	*pState = (NPC_SET_MENU_STATE_SYNC*)pMsg;
	int		nNpc;
	if (pState->m_dwID == Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID)
	{//�ǿͻ��˱���
		nNpc = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		if (pState->m_btState == PLAYER_MENU_STATE_TRADEOPEN)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, "%s",  strCoreInfo[MSG_TRADE_STATE_OPEN].c_str());
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		else if (pState->m_btState == PLAYER_MENU_STATE_NORMAL)
		{
			//messageBox("�ر�","tset");
			if (Npc[nNpc].GetMenuState() == PLAYER_MENU_STATE_TRADEOPEN)
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TRADE_STATE_CLOSE].c_str());
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
			}
		}
	}
	else
	{
		nNpc = NpcSet.SearchID(pState->m_dwID);
		if (nNpc == 0)
			return;
	}
	if (pState->m_wLength > sizeof(NPC_SET_MENU_STATE_SYNC) - 1 - sizeof(pState->m_szSentence))
		Npc[nNpc].SetMenuState(pState->m_btState, pState->m_szSentence, pState->m_wLength - (sizeof(NPC_SET_MENU_STATE_SYNC) - 1 - sizeof(pState->m_szSentence)));
	else
		Npc[nNpc].SetMenuState(pState->m_btState);
}

void	KProtocolProcess::s2cTradeMoneySync(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].s2cTradeMoneySync(pMsg);
}

void	KProtocolProcess::s2cTradeDecision(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].s2cTradeDecision(pMsg);

	Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
}
//�ͻ���״̬�㲥
void	KProtocolProcess::s2cUpdateNpcState(BYTE * pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_SYNC_STATEINFO * pInfo = (NPC_SYNC_STATEINFO*) pMsg;

	int nIndex = NpcSet.SearchID(pInfo->m_ID);	//״̬������

	if (nIndex > 0 && !Npc[nIndex].IsPlayer() && Npc[nIndex].m_RegionIndex >= 0)
	{// ���ǿͻ��˱��� �ͻ�������״̬����  //andriod����
		Npc[nIndex].SetNpcState(pInfo->m_btStateInfo);  //����NPC��״̬
	}
}

void	KProtocolProcess::s2cTeamInviteAdd(BYTE * pMsg)
{

	if  (NULL==pMsg||NULL==this) return;

    if (Player[CLIENT_PLAYER_INDEX].m_Autoplay.nCteam.nIsHuiDa==1 && Player[CLIENT_PLAYER_INDEX].m_Autoplay.nCteam.nIsDuiYuan==1)
    {//�Զ����
	   TEAM_INVITE_ADD_SYNC	*pPress = (TEAM_INVITE_ADD_SYNC*)pMsg;
	   Player[CLIENT_PLAYER_INDEX].m_cTeam.ReplyInvite(pPress->m_nIdx, 1);
	   return;
	}

	Player[CLIENT_PLAYER_INDEX].m_cTeam.ReceiveInvite((TEAM_INVITE_ADD_SYNC*)pMsg);
}

void	KProtocolProcess::s2cTradePressOkSync(BYTE * pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	TRADE_STATE_SYNC	*pPress = (TRADE_STATE_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cTrade.SetTradeState(pPress->m_btSelfLock, pPress->m_btDestLock, pPress->m_btSelfOk, pPress->m_btDestOk);
}
//�ͻ��˹⻷����ͬ�����ӵ����ܣ�
void	KProtocolProcess::s2cDirectlyCastSkill(BYTE * pMsg)
{
	if  (NULL==pMsg||NULL==this) return;;

	DWORD	dwNpcID;
	int		nSkillID, nSkillLevel, nSkillEnChance,nWaitTimer,mIsEnChance,mMaxBeiLv=1;
	int 	MapX, MapY; //DWORD
	NPC_SKILL_SYNC	*pPress = (NPC_SKILL_SYNC*)pMsg;
/*	dwNpcID = *(DWORD *)&pMsg[1];
	nSkillID = *(int *)&pMsg[5];
	nSkillLevel = *(int *)&pMsg[9];
	MapX = *(int *)&pMsg[13];
	MapY = *(int *)&pMsg[17];
	nSkillEnChance = *(int *)&pMsg[21];  //���ܼӳ�
	*/
	dwNpcID      = pPress->ID;
	nSkillID     = pPress->nSkillID;          //�Ӽ���
	nSkillLevel  = pPress->nSkillLevel;
	MapX         = pPress->nMpsX;
	MapY         = pPress->nMpsY;
	nSkillEnChance = pPress->nSkillEnChance;  //���ܼӳ�
	nWaitTimer   = pPress->nWaitTime;
	mIsEnChance  = pPress->nIsEnChance;
	mMaxBeiLv    = pPress->nMaxBei;           // ��������
	//��ָ��ĳ��Ŀ��ʱ(MapX == -1),MapYΪĿ���NpcdwID����Ҫת���ɱ��ص�NpcIndex����
	if (MapX == -1)
	{
		if (MapY < 0 )
			return;
        int nIndex=0;
		    nIndex = NpcSet.SearchID(MapY);

		if (nIndex <= 0)
			return;

		if (Npc[nIndex].m_RegionIndex < 0)
			return;

		int nX, nY;
		nX = Npc[nIndex].m_MapX;
		nY = Npc[nIndex].m_MapY;
		SubWorld[0].NewMap2Mps(Npc[nIndex].m_RegionIndex, nX, nY, 0, 0, (int *)&MapX, (int *)&MapY); //��������
	}

	    int nIdx = NpcSet.SearchID(dwNpcID);

		if (nIdx<=0)
			return;

		//if (!Npc[nIdx].IsPlayer())
		//   return;

        //if (Player[CLIENT_PLAYER_INDEX].ConformIdx(nIdx))//���Ǳ���
		//   return;

		if (nSkillID <= 0 || nSkillLevel <=0)
			return;

		KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillID,nSkillLevel);

		if (!pOrdinSkill)
            return ;

		pOrdinSkill->m_nEnChance= nSkillEnChance;
		//pOrdinSkill->Cast(nIdx,MapX,MapY,nWaitTimer,SKILL_SLT_Npc,mMaxBeiLv);  //������
		pOrdinSkill->Cast(nIdx, MapX, MapY);
		//char msg[64];
		//sprintf(msg,"%s %s",Npc[nIdx].Name,pOrdinSkill->GetSkillName());
		//messageBox(msg,"ͬ��������");
  }

void	KProtocolProcess::s2cShowMsg(BYTE *pMsg)
{
	//return;//ȡ��

	if (NULL==pMsg||NULL==this)
		return;
	SHOW_MSG_SYNC	*pShowMsg = (SHOW_MSG_SYNC*)pMsg;

	switch (pShowMsg->m_wMsgID)
	{
	case enumMSG_ID_TEAM_KICK_One:
		{
			char	szName[32];
			KSystemMessage	sMsg;

			memset(szName, 0, sizeof(szName));
			memcpy(szName, pMsg + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));
			if (strcmp(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, szName) == 0)
			{
				sprintf(sMsg.szMessage, "%s", strCoreInfo[MSG_TEAM_BE_KICKEN].c_str());
				sMsg.eType = SMT_TEAM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
			}
			else
			{
				sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_KICK_ONE].c_str(), szName);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
			}
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_DISMISS:
		{
			if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
				break;
			KSystemMessage	sMsg;
			if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFigure == TEAM_CAPTAIN)
			{
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TEAM_DISMISS_CAPTAIN].c_str());
			}
			else
			{
				sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_DISMISS_MEMBER].c_str(), g_Team[0].m_szMemName[0]);
			}
			sMsg.eType = SMT_TEAM;
			sMsg.byConfirmType = SMCT_CLICK;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_LEAVE:
		{
			if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
				break;
			KSystemMessage	sMsg;
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID == dwID)
			{
				sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_LEAVE_SELF_MSG].c_str(), g_Team[0].m_szMemName[0]);
			}
			else
			{
				sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_LEAVE].c_str(), "����");
				for (int i = 0; i < MAX_TEAM_MEMBER; i++)
				{
					if ((DWORD)g_Team[0].m_nMember[i] == dwID)
					{
						sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_LEAVE].c_str(), g_Team[0].m_szMemName[i + 1]);
						break;
					}
				}
			}
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
/*	case enumMSG_ID_TEAM_REFUSE_INVITE:
		{
			char	szName[32];
			memset(szName, 0, sizeof(szName));
			memcpy(szName, &pShowMsg->m_lpBuf, pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TEAM_REFUSE_INVITE, szName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;*/
	case enumMSG_ID_TEAM_SELF_ADD:
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_SELF_ADD].c_str(), g_Team[0].m_szMemName[0]);
			sMsg.eType = SMT_TEAM;
			sMsg.byConfirmType = SMCT_UI_TEAM;
			sMsg.byPriority = 3;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL:
		{
			int		nMember;
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			nMember = g_Team[0].FindMemberID(dwID);
			if (nMember < 0)
				break;
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			sprintf(sMsg.szMessage,   "%s", strCoreInfo[MSG_TEAM_CHANGE_CAPTAIN_FAIL1].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
			sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_CHANGE_CAPTAIN_FAIL2].c_str(), g_Team[0].m_szMemName[nMember + 1]);
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL2:
		{
			int		nMember;
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);
			nMember = g_Team[0].FindMemberID(dwID);
			if (nMember < 0)
				break;
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TEAM_CHANGE_CAPTAIN_FAIL1].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TEAM_CHANGE_CAPTAIN_FAIL3].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_OBJ_CANNOT_PICKUP:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			strcpy(sMsg.szMessage, strCoreInfo[MSG_OBJ_CANNOT_PICKUP].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_OBJ_TOO_FAR:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			strcpy(sMsg.szMessage, strCoreInfo[MSG_OBJ_TOO_FAR].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_DEC_MONEY:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, strCoreInfo[MSG_DEC_MONEY].c_str(), (int)pShowMsg->m_lpBuf);
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_SELF_ROOM_FULL:
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TRADE_SELF_ROOM_FULL].c_str());
			sMsg.eType = SMT_SYSTEM;
			sMsg.byConfirmType = SMCT_CLICK;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_DEST_ROOM_FULL:
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, strCoreInfo[MSG_TRADE_DEST_ROOM_FULL].c_str(), Player[CLIENT_PLAYER_INDEX].m_cTrade.m_szDestName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_REFUSE_APPLY:
		{
			int	nIdx = NpcSet.SearchID(*((DWORD*)&pShowMsg->m_lpBuf));
			if (nIdx <= 0)
				return;
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, strCoreInfo[MSG_TRADE_REFUSE_APPLY].c_str(), Npc[nIdx].Name);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TRADE_TASK_ITEM:
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TRADE_TASK_ITEM].c_str());
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_ITEM_DAMAGED:
		{
			int nItemID = (int)pShowMsg->m_lpBuf;
			int nIdx = ItemSet.SearchID(nItemID);
			if (!nIdx)
				break;

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 1;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, strCoreInfo[MSG_ITEM_DAMAGED].c_str(), Item[nIdx].GetName());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_GET_ITEM:
		{
			DWORD	dwID = *(DWORD*)(&pShowMsg->m_lpBuf);

			int nItemIdx = ItemSet.SearchID(dwID);
			if (nItemIdx <= 0 || nItemIdx >= MAX_ITEM)
				break;

			char	szName[128];
			KSystemMessage	sMsg;

			strcpy(szName, Item[nItemIdx].GetName());

			if (strlen(szName) >= sizeof(sMsg.szMessage) - strlen(MSG_ADD_ITEM))
				break;

			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, strCoreInfo[MSG_ADD_ITEM].c_str(), szName);  //�����Ʒ��ʾ
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_MONEY_CANNOT_PICKUP:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			strcpy(sMsg.szMessage, strCoreInfo[MSG_MONEY_CANNOT_PICKUP].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_CANNOT_ADD_TEAM:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TEAM_CANNOT_CREATE].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TARGET_CANNOT_ADD_TEAM:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TEAM_TARGET_CANNOT_ADD_TEAM].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_1:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_PK_ERROR_1].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_2:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, "%s", strCoreInfo[MSG_PK_ERROR_2].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_3:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_PK_ERROR_3].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_4:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_PK_ERROR_4].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_5:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_PK_ERROR_5].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_6:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_PK_ERROR_6].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_PK_ERROR_7:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_PK_ERROR_7].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_DEATH_LOSE_ITEM:
		{
			char	szName[32];
			memset(szName, 0, sizeof(szName));
			memcpy(szName, &pShowMsg->m_lpBuf, pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, strCoreInfo[MSG_DEATH_LOSE_ITEM].c_str(), szName);
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TONG_REFUSE_ADD:
		{
			//char szName[32];
			//memset(szName, 0, sizeof(szName));
			//memcpy(szName, pMsg + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), pShowMsg->m_wLength + 1 + sizeof(LPVOID) - sizeof(SHOW_MSG_SYNC));

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,strCoreInfo[MSG_TONG_REFUSE_ADD].c_str() , pShowMsg->m_Mmsgs);
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TONG_BE_KICK:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_BE_KICKED].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);

			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
			sUi.nData = TONG_ACTION_DISMISS;
			sUi.nParam = 0;
			sUi.uParam = 0;
			sUi.szString[0] = 0;

			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (uintptr_t)&sUi, 1);
		}
		break;
	case enumMSG_ID_TONG_LEAVE_SUCCESS:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_LEAVE_SUCCESS].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TONG_LEAVE_FAIL:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_LEAVE_FAIL].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_TONG_CHANGE_AS_MASTER:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CHANGE_AS_MASTER].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);

			/*KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Player[CLIENT_PLAYER_INDEX].m_cTong.m_BMasterName);
			sUi.nData = TONG_ACTION_DEMISE;
			sUi.nParam = 0;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (uintptr_t)&sUi, 1);*/
		}
		break;
	case enumMSG_ID_TONG_CHANGE_AS_MEMBER:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CHANGE_AS_MEMBER].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);

			/*KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Player[CLIENT_PLAYER_INDEX].m_cTong.m_BMasterName);
			sUi.nData = TONG_ACTION_DEMISE;
			sUi.nParam = 0;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (uintptr_t)&sUi, 1);*/
		}
		break;
	case enumMSG_ID_RIDE_CANNOT:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_NPC_CANNOT_RIDE].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_COMP_FAILED:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_COMP_FAILED].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_COMP_SUCCESS:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_COMP_SUCCESS].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_COMP_ERITEM:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_COMP_ERITEM].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
    case enumMSG_ID_COMP_MONEY:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_COMP_MONEY].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_COMP_NO:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_ID_COMP_NO].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_SUNYI_SUCCESS:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_SUNYI_SUCCESS].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_LIANJIE_FAILED: //����ʧЧ
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_ID_LIANJIE_FAILED].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_FUHUO_FAILED: //ԭ�ظ���
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, "%s", strCoreInfo[MSG_ID_FUHUO_YUANDIAN].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_ERROR_INFO: //ԭ�ظ���
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,strCoreInfo[L_NPC_15].c_str(),"��");
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
	case enumMSG_ID_SHUA_SHUA: //ˢװ����ʾ
		{
        	KSystemMessage	sMsg;
	        sMsg.eType = SMT_NORMAL;
	        sMsg.byConfirmType = SMCT_NONE;
	        sMsg.byPriority = 0;
	        sMsg.byParamSize = 0;
	        sprintf(sMsg.szMessage, "%s", strCoreInfo[MSG_ID_SHUA_SHUA].c_str());
	        sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	        CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
		break;
   	case enumMSG_ID_RENOVE_TIME_ITEM: //ɾ��������Ʒ
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", pShowMsg->m_Mmsgs);
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		//	ZeroMemory()
		}
		break;
	default:
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage,  "%s", pShowMsg->m_Mmsgs);
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
			//	ZeroMemory()
		}
		break;
	}
}

//ͬ���Լ�����״̬�⻷Ч��
void	KProtocolProcess::SyncStateEffect(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	STATE_EFFECT_SYNC*	pSync = (STATE_EFFECT_SYNC *)pMsg;
	int nDataNum = MAX_SKILL_STATE - (sizeof(STATE_EFFECT_SYNC) - pSync->m_wLength) / sizeof(KMagicAttrib);
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SetStateSkillEffect(Player[CLIENT_PLAYER_INDEX].m_nIndex, pSync->m_dwSkillID, pSync->m_nLevel, pSync->m_MagicAttrib, nDataNum, pSync->m_nTime,pSync->m_IsEuq);
}

void	KProtocolProcess::s2cTradeApplyStart(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].s2cTradeApplyStart(pMsg);
}
//�ͻ����Զ��ƶ����������������ͷ���
void	KProtocolProcess::s2cItemAutoMove(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	ITEM_AUTO_MOVE_SYNC	*pSync = (ITEM_AUTO_MOVE_SYNC*)pMsg;
	ItemPos	sSrc, sDest;
	sSrc.nPlace     = pSync->m_btSrcPos;    //Դ����
	sSrc.nX         = pSync->m_btSrcX;
	sSrc.nY         = pSync->m_btSrcY;
    sSrc.nSIsRongqi = pSync->m_SisRongqi;

	sDest.nPlace     = pSync->m_btDestPos;  //Ŀ������
	sDest.nX         = pSync->m_btDestX;
	sDest.nY         = pSync->m_btDestY;
	sDest.nDIsRongqi = pSync->m_DisRongqi;

	Player[CLIENT_PLAYER_INDEX].m_ItemList.AutoMoveItem(sSrc, sDest,pSync->m_ItenDwid);
}
//�����Ʒ���� �ͽ���
void KProtocolProcess::FinishedItemExchange(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].m_ItemList.UnlockOperation();
}

extern IClientCallback* l_pDataChangedNotifyFunc;

void KProtocolProcess::s2cExtend(BYTE* pMsg)
{
	//messageBox("Ƶ��������Ϣ�������","Ƶ��������Ϣ�����");
	if  (NULL==pMsg||NULL==this) return;
	EXTEND_HEADER* pHeader = (EXTEND_HEADER*)(pMsg + sizeof(tagExtendProtoHeader));

	if (pHeader->ProtocolFamily == pf_playercommunity)
	{//playercomm_c2s_querychannelid
		if (pHeader->ProtocolID == playercomm_s2c_notifychannelid)
		{
			PLAYERCOMM_NOTIFYCHANNELID* pNChann = (PLAYERCOMM_NOTIFYCHANNELID*)pHeader;
			if (g_GameWorld)
				g_GameWorld->NotifyChannelID(pNChann->channel, pNChann->channelid, pNChann->cost);
		}
	}
}
//�ͻ���
void KProtocolProcess::s2cExtendChat(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	tagExtendProtoHeader* pExHdr = (tagExtendProtoHeader*)pMsg;
	void* pExPckg = pExHdr + 1;
	BYTE protocol = *(BYTE*)(pExPckg);
	if (protocol == chat_someonechat)   //����
	{//�Է�
		CHAT_SOMEONECHAT_SYNC* pCscSync = (CHAT_SOMEONECHAT_SYNC*)pExPckg;
		if (g_GameWorld)
			g_GameWorld->MSNMessageArrival(
			pCscSync->someone, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name,
			(const char*)(pCscSync + 1), pCscSync->sentlen,true,pCscSync->nItemDwid);

		//Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid=pCscSync->nItemDwid;
		//char nMsg[64]={0};
		//sprintf(nMsg,"˽��Ƶ��:%s,��ƷID:%d",pCscSync->someone,pCscSync->nItemDwid);
		//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nMsg);

	}
	else if (protocol == chat_channelchat)   //���淢��ͨ��  ������
	{
		CHAT_CHANNELCHAT_SYNC* pCccSync = (CHAT_CHANNELCHAT_SYNC*)pExPckg;
		//int pSentlenA=0;
        //    pSentlenA =pCccSync->sentlen;// TEncodeText((char*)(pCccSync + 1), (int)pCccSync->sentlen);//���Ʊ�ǽ���ת��
	    if (g_GameWorld)
		   g_GameWorld->ChannelMessageArrival(pCccSync->channelid, pCccSync->someone,(char*)(pCccSync + 1),pCccSync->sentlen,true,pCccSync->nItemDwid,pCccSync->packageID);

		//Player[CLIENT_PLAYER_INDEX].m_ItemLinkDwid=pCccSync->nItemDwid;

		//char nMsg[64]={0};
		//sprintf(nMsg,"Ƶ��:%d,��ƷID:%d",pCccSync->channelid,pCccSync->nItemDwid);
		//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nMsg);
	}
	else if (protocol == chat_feedback)
	{//����
		////X
		CHAT_FEEDBACK* pCfb = (CHAT_FEEDBACK*)pExPckg;
		UINT* pChannelid = (UINT*)(pCfb + 1);

		if (*pChannelid == -1)
		{//someone   ˽��
			char* pDstName = (char*)(pChannelid + 1);
			BYTE* pSentlen = (BYTE*)(pDstName + _NAME_LEN);
			void* pSent    = pSentlen + 1;

			if (g_GameWorld)
				g_GameWorld->MSNMessageArrival(
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, pDstName,
				(const char*)pSent, *pSentlen, pCfb->code != codeFail);
		}
		else
		{//channel  Ƶ����Ϣ
			BYTE* pSentlen = (BYTE*)(pChannelid + 1);
			void* pSent = pSentlen + 1;
			if (g_GameWorld)
				g_GameWorld->ChannelMessageArrival(
				*pChannelid, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name,
				(char*)pSent, *pSentlen, pCfb->code != codeFail);
		}
		////X
	}
}

static BOOL sParseUGName(const std::string& name, std::string* pUnit, std::string* pGroup)
{
	static const char char_split = '\n';

	size_t pos = name.find(char_split);
	if (pos == name.npos)
	{
		if (pUnit)
			pUnit->resize(0);
		if (pGroup)
			pGroup->assign(name);
	}
	else
	{
		std::string::const_iterator itSplit = name.begin() + pos;

		if (pUnit)
			pUnit->assign(name.begin(), itSplit);
		if (pGroup)
			pGroup->assign(itSplit + 1, name.end());
	}

	return TRUE;
}

void KProtocolProcess::s2cExtendFriend(BYTE* pMsg)
{
	return;//��BUG ��ʱȡ��
	if  (NULL==pMsg||NULL==this) return;
	tagExtendProtoHeader* pExHdr = (tagExtendProtoHeader*)pMsg;
	void* pExPckg = pExHdr + 1;

	EXTEND_HEADER* pHeader = (EXTEND_HEADER*)(pExPckg);

	if (pHeader->ProtocolFamily == pf_tong)
	{
		//tong message
		//...
	}
	else if (pHeader->ProtocolFamily == pf_friend)
	{
		if (pHeader->ProtocolID == friend_c2c_askaddfriend)
		{
			ASK_ADDFRIEND_SYNC* pAafSync = (ASK_ADDFRIEND_SYNC*)pHeader;
			//l_pDataChangedNotifyFunc->FriendInvite(pAafSync->srcrole);
		}
		else if (pHeader->ProtocolID == friend_c2c_repaddfriend)
		{
			REP_ADDFRIEND_SYNC* pRafSync = (REP_ADDFRIEND_SYNC*)pHeader;
			//l_pDataChangedNotifyFunc->AddFriend(pRafSync->srcrole, pRafSync->answer);
		}
		else if (pHeader->ProtocolID == friend_s2c_repsyncfriendlist)
		{
			REP_SYNCFRIENDLIST* pRsfl = (REP_SYNCFRIENDLIST*)pHeader;
			char* pGroupTag = (char*)(pRsfl + 1);
			if (*pGroupTag != specGroup)
				goto on_error;
			{{
			char* pGroup = (char*)(pGroupTag + 1);
on_newgroup:
			std::string theUnit, theGroup;
			sParseUGName(std::string(pGroup), &theUnit, &theGroup);

			for (char* pRoleTag = pGroup + strlen(pGroup) + 1; ; )
			{
				if (*pRoleTag == specOver)
					goto on_over;
				else if (*pRoleTag == specGroup)
				{
					pGroup = pRoleTag + 1;
					goto on_newgroup;
				}
				else if (*pRoleTag == specRole)
				{
					char* pState = pRoleTag + 1;
					char* pRole = pState + 1;
					//l_pDataChangedNotifyFunc->FriendInfo(pRole, (char*)theUnit.c_str(), (char*)theGroup.c_str(), (BYTE)*pState);
					pRoleTag = pRole + strlen(pRole) + 1;
					continue;
				}
				else
					goto on_error;
			}
			}}
on_error:
			0;
on_over:
			0;
		}
		else if (pHeader->ProtocolID == friend_s2c_friendstate)
		{
			FRIEND_STATE* pFs = (FRIEND_STATE*)pHeader;
			//for (char* pRole = (char*)(pFs + 1); *pRole; pRole += strlen(pRole) + 1)
//				l_pDataChangedNotifyFunc->FriendStatus(pRole, pFs->state);
		}
		else if (pHeader->ProtocolID == friend_s2c_syncassociate)
		{
			FRIEND_SYNCASSOCIATE* pFsa = (FRIEND_SYNCASSOCIATE*)pHeader;

			char* szGroup = (char*)(pFsa + 1);
			std::string group(szGroup);

			std::string theUnit, theGroup;
			sParseUGName(group, &theUnit, &theGroup);

			//CCAssert(theGroup.empty(),"");

	//		for (char* szRole = szGroup + group.size() + 1; *szRole; szRole += strlen(szRole) + 1)
//				l_pDataChangedNotifyFunc->AddPeople((char*)theUnit.c_str(), szRole);
		}
	}
}

void KProtocolProcess::s2cExtendTong(BYTE* pMsg)
{
	//return;//ȡ��
	if  (NULL==pMsg||NULL==this) return;
	S2C_TONG_HEAD	*pHead = (S2C_TONG_HEAD*)pMsg;

	switch (pHead->m_btMsgId)
	{
	case enumTONG_SYNC_ID_CITY_INFO:
		{
//		   Player[CLIENT_PLAYER_INDEX].m_cTong.SetAttAckCityInfo(pMsg);
		  // Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("�����߳���Ϣ�ɹ�!!!");
		}
		break;
	case enumTONG_SYNC_ID_ATTACK_INFO:
		{
			//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("���հ�վ��Ϣ�ɹ�!!!");
//			Player[CLIENT_PLAYER_INDEX].m_cTong.SetAttAckInfo(pMsg);

		}
		break;
	case enumTONG_SYNC_ID_CREATE_FAIL:
		{
			TONG_CREATE_FAIL_SYNC *pFail = (TONG_CREATE_FAIL_SYNC*)pMsg;

			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;

			sprintf(sMsg.szMessage, "��ᴴ��ʧ��");
			sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);

			switch (pFail->m_btFailId)
			{
			case enumTONG_CREATE_ERROR_ID1:		// Player[m_nPlayerIndex].m_nIndex <= 0
				break;
			case enumTONG_CREATE_ERROR_ID2:		// ���׹�����
				break;
			case enumTONG_CREATE_ERROR_ID3:		// ���������
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR01].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID4:		// �����Ӫ����
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR04].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID5:		// �Ѿ��ǰ���Ա
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR05].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID6:		// �Լ�����Ӫ����
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR04].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID7:		// �ȼ�����
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR07].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID8:		// Ǯ����
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR08].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID9:		// ��Ӳ��ܽ����
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR09].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID10:	// ���ģ�����
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR10].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID11:	// �����ַ�������
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR11].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID12:	// �����ַ�������
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR12].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID13:	// ���ͬ������
				sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_TONG_CREATE_ERROR13].c_str());
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				break;
			case enumTONG_CREATE_ERROR_ID14:	// ������ʧ��
				break;
			}
		}
		break;
	case enumTONG_SYNC_ID_TRANSFER_ADD_APPLY:
		{//����������ͬ�� �ܾ������ ����  �ͻ���
			TONG_APPLY_ADD_SYNC	*pApply = (TONG_APPLY_ADD_SYNC*)pMsg;
			char	szName[32];
			DWORD	dwNameID;
			int		nPlayerIdx;

			memset(szName, 0, sizeof(szName));
			memcpy(szName, pApply->m_szName, pApply->m_wLength + 1 + sizeof(pApply->m_szName) - sizeof(TONG_APPLY_ADD_SYNC));
			dwNameID   = g_FileName2Id(szName);
			nPlayerIdx = pApply->m_nPlayerIdx;

            /*if (pApply->m_IsAuToAdd==2)    //�Ƿ��Զ�ͬ��������
			{
		     	Player[CLIENT_PLAYER_INDEX].m_cTong.AcceptMember(pApply->m_nPlayerIdx,dwNameID,1);
				char msg[64];
				sprintf(msg,"��ϲ:(%s)������ļҪ��,�Զ��������!",szName);
				Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(msg);
				break;
			}*/

			//�����淢��Ϣ���յ����룬�Ƿ�ͬ��
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage,"�յ�(%s)���������!", szName);
			sMsg.eType = SMT_CLIQUE;
			sMsg.byConfirmType = SMCT_UI_TONG_JOIN_APPLY;
			sMsg.byPriority = 3;
			sMsg.byParamSize = sizeof(KUiPlayerItem);
			sMsg.nMsgLen=TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
			KUiPlayerItem	player;
			//strcpy(player.Name, szName);
			sprintf(player.Name, "%s", szName);
			player.nIndex = pApply->m_nPlayerIdx;
			player.uId = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, (intptr_t)&player);
		}
		break;
	case enumTONG_SYNC_ID_ADD:
		{
			TONG_Add_SYNC	*pAdd = (TONG_Add_SYNC*)pMsg;
			char	szName[32], szTitle[32], szMaster[32];

			memcpy(szName, pAdd->m_szTongName,sizeof(szName));
			memcpy(szTitle, pAdd->m_szTitle, sizeof(szTitle));
			memcpy(szMaster, pAdd->m_szMaster, sizeof(szMaster));

			Player[CLIENT_PLAYER_INDEX].m_cTong.AddTong(pAdd->m_btCamp, szName, szTitle, szMaster);

			// ֪ͨ�����������
			/*KUiGameObjectWithName	sUi;
			memset(&sUi, 0, sizeof(sUi));
			strcpy(sUi.szName, szName);
			sUi.nData = TONG_ACTION_APPLY;
			strcpy(sUi.szString, szTitle);
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (uintptr_t)&sUi, 1);*/
		/*------------------------------*/
			CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
		}
		break;
	case enumTONG_SYNC_ID_HEAD_INFO: //��ȡ������Ϣ
		{
			TONG_HEAD_INFO_SYNC	*pInfo = (TONG_HEAD_INFO_SYNC*)pMsg;

			// ֪ͨ����õ�ĳ�����Ϣ
			int nIdx = NpcSet.SearchID(pInfo->m_dwNpcID);
			if (nIdx <= 0)
				break;

			KUiPlayerRelationWithOther	sUi;
			sUi.nIndex = nIdx;
			sUi.uId = Npc[nIdx].m_dwID;
			//if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex)
				//sUi.nRelation = Player[CLIENT_PLAYER_INDEX].m_cTong.GetFigure();
			//else
				sUi.nRelation = -1;
			sUi.nData = 0;
			sUi.nParam = 0;
			strcpy(sUi.Name, Npc[nIdx].Name);

	        KClientTongInfo	sInfo;
			memset(&sInfo, 0, sizeof(sInfo));
			sInfo.nFaction       = pInfo->m_btCamp;                   //��Ӫ
			sInfo.nMemberCount   = (int)pInfo->m_dwMemberNum;         //������
			sInfo.nManagerCount  = (int)pInfo->m_btManagerNum;        //�ӳ���
			sInfo.nDirectorCount = (int)pInfo->m_btDirectorNum;       //������
			sInfo.nZhaoMuCount   = pInfo->m_btZhaoMuNum;
            sInfo.nApplyCount    = pInfo->m_btApplyNum;
			sInfo.nMoney         = pInfo->m_dwMoney;  //Ǯ
			sInfo.nLevel         = pInfo->m_btLevel;  //�ȼ�

			//if (nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex && sUi.nRelation==1)
				//Player[CLIENT_PLAYER_INDEX].m_cTong.m_CurRenShu=(int)pInfo->m_dwMemberNum+(int)pInfo->m_btManagerNum+(int)pInfo->m_btDirectorNum;

			//strcpy(sInfo.szMasterName, pInfo->m_sMember[0].m_szName);  //������  �д���  ����pInfo->m_sMember[0].m_szName
			//strcpy(sInfo.szName, pInfo->m_szTongName);                 //�����

//			char msg[64];
			sprintf(sInfo.szMasterName,"%s",pInfo->m_sMember[0].m_szName);
            sprintf(sInfo.szName,"%s",pInfo->m_szTongName);
            //sprintf(msg,"���:%s ����:%s",pInfo->m_szTongName,pInfo->m_sMember[0].m_szName);
			sInfo.szName[8]='\0';
            //Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(msg);
			CoreDataChanged(GDCNI_TONG_INFO, (uintptr_t)&sUi, (uintptr_t)&sInfo);

			KUiGameObjectWithName	sObj;
			strcpy(sObj.szName, pInfo->m_szTongName); //�����
			sObj.nData  = enumTONG_FIGURE_DIRECTOR;
			sObj.nParam = 0;
			sObj.uParam = (uintptr_t)pInfo->m_btDirectorNum;//��������
			sObj.szString[0] = 0;

			KTongMemberItem	sItem[defTONG_MAX_DIRECTOR];
			memset(sItem, 0, sizeof(sItem));
			for (int i = 0; i < pInfo->m_btDirectorNum; i++)//����
			{//���Ͻ������
				sItem[i].nData = enumTONG_FIGURE_DIRECTOR;
				strcpy(sItem[i].Name, pInfo->m_sMember[i + 1].m_szName);      //��������
				strcpy(sItem[i].szAgname, pInfo->m_sMember[i + 1].m_szTitle); //�˺ųƺ�
			}

			CoreDataChanged(GDCNI_TONG_MEMBER_LIST, (uintptr_t)&sObj, (intptr_t)sItem);
		}
		break;
	case enumTONG_SYNC_ID_SELF_INFO:  //ͬ���ͻ��˵İ����Ϣ
		{
			TONG_SELF_INFO_SYNC	*pInfo = (TONG_SELF_INFO_SYNC*)pMsg;
			Player[CLIENT_PLAYER_INDEX].m_cTong.SetSelfInfo(pInfo);   //���ð����Ϣ
			// ֪ͨ�����������
			CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
		}
		break;
	case enumTONG_SYNC_ID_MANAGER_INFO:
		{
			TONG_MANAGER_INFO_SYNC	*pInfo = (TONG_MANAGER_INFO_SYNC*)pMsg;

			// ֪ͨ����õ�ĳ���ӳ���Ϣ
			KUiGameObjectWithName	sObj;
			strcpy(sObj.szName, pInfo->m_szTongName);
			sObj.nData = enumTONG_FIGURE_MANAGER;
			sObj.nParam = pInfo->m_btStateNo;
			sObj.uParam = pInfo->m_btCurNum;
			sObj.szString[0] = 0;

			KTongMemberItem	sItem[defTONG_ONE_PAGE_MAX_NUM];
			memset(sItem, 0, sizeof(sItem));
			for (int i = 0; i < pInfo->m_btCurNum; i++)
			{
				sItem[i].nData = enumTONG_FIGURE_MANAGER;
				strcpy(sItem[i].Name, pInfo->m_sMember[i].m_szName);
				strcpy(sItem[i].szAgname, pInfo->m_sMember[i].m_szTitle);
			}

			CoreDataChanged(GDCNI_TONG_MEMBER_LIST, (uintptr_t)&sObj, (intptr_t)sItem);
		}
		break;
	case enumTONG_SYNC_ID_LIST_INFO:
		{//ͬ������б���Ϣ
			TONG_LIST_INFO_SYNC	*pLInfo = (TONG_LIST_INFO_SYNC*)pMsg;
			// ֪ͨ����õ�ĳ��������Ϣ
			KUiGameObjectWithName	sLObj;
			strcpy(sLObj.szName, "ϵͳϵͳ"); //�����
			sLObj.nData = pLInfo->m_Count;	     //����������
			sLObj.nParam = pLInfo->m_btStateNo;  //��ʼ���
			sLObj.uParam = pLInfo->m_btCurNum;   //��ǰ������
			sLObj.szString[0] = 0;

			KTongMemberItem	sLItem[defTONG_ONE_PAGE_MAX_NUM];

			memset(sLItem, 0, sizeof(sLItem));
			for (int i = 0; i < pLInfo->m_btCurNum; i++)
			{
				sLItem[i].nData = enumTONG_FIGURE_LIST;
				strcpy(sLItem[i].Name, pLInfo->m_sTongList[i].m_szName); //�����
				strcpy(sLItem[i].szAgname,"ȫ��");         //ְλ
			}

			CoreDataChanged(GDCNI_TONG_LIST, (uintptr_t)&sLObj, (intptr_t)sLItem);
		}
		break;
	case enumTONG_SYNC_ID_MEMBER_INFO:
		{
			TONG_MEMBER_INFO_SYNC	*pInfo = (TONG_MEMBER_INFO_SYNC*)pMsg;

			// ֪ͨ����õ�ĳ��������Ϣ
			KUiGameObjectWithName	sObj;
			strcpy(sObj.szName, pInfo->m_szTongName); //�����
			sObj.nData = enumTONG_FIGURE_MEMBER;
			sObj.nParam = pInfo->m_btStateNo;  //��ʼ���
			sObj.uParam = pInfo->m_btCurNum;   //��ǰ������
			sObj.szString[0] = 0;

			KTongMemberItem	sItem[defTONG_ONE_PAGE_MAX_NUM];

			memset(sItem, 0, sizeof(sItem));
			for (int i = 0; i < pInfo->m_btCurNum; i++)
			{
				sItem[i].nData = enumTONG_FIGURE_MEMBER;
				strcpy(sItem[i].Name, pInfo->m_sMember[i].m_szName); //����
				strcpy(sItem[i].szAgname, pInfo->m_szTitle);         //ְλ
			}

			CoreDataChanged(GDCNI_TONG_MEMBER_LIST, (uintptr_t)&sObj, (intptr_t)sItem);
		}
		break;
	case enumTONG_SYNC_ID_INSTATE:
		{
			TONG_INSTATE_SYNC	*pInstate = (TONG_INSTATE_SYNC*)pMsg;
			// ֪ͨ���������Ƿ�ɹ�
			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, pInstate->m_szName);
			sUi.nData = TONG_ACTION_ASSIGN;
			sUi.nParam = pInstate->m_btNewFigure;
			sUi.uParam = pInstate->m_btOldFigure;
			strcpy(sUi.szString, pInstate->m_szTitle);
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (uintptr_t)&sUi, pInstate->m_btSuccessFlag);
		}
		break;
	case enumTONG_SYNC_ID_KICK:
		{
			TONG_KICK_SYNC	*pKick = (TONG_KICK_SYNC*)pMsg;
			// ֪ͨ���������Ƿ�ɹ�
			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, pKick->m_szName);
			sUi.nData = TONG_ACTION_DISMISS;
			sUi.nParam = pKick->m_btFigure;
			sUi.uParam = pKick->m_btPos;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (uintptr_t)&sUi, pKick->m_btSuccessFlag);
		}
		break;
	case enumTONG_SYNC_ID_CHANGE_MASTER_FAIL:
		{
			TONG_CHANGE_MASTER_FAIL_SYNC	*pFail = (TONG_CHANGE_MASTER_FAIL_SYNC*)pMsg;

			switch (pFail->m_btFailID)
			{
			case 0:		// �Է�������
				{
					KSystemMessage	sMsg;
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					sprintf(sMsg.szMessage, "��ʾ:�Է�������!");
					sMsg.nMsgLen=TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				}
				break;
			case 1:		// �Է�����������
				{
					KSystemMessage	sMsg;
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					sprintf(sMsg.szMessage, "�Է���������!!");
					sMsg.nMsgLen=TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				}
				break;
			case 2:		// ���ֲ��ԣ�
				{
					KSystemMessage	sMsg;
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					sprintf(sMsg.szMessage, "�Է����ֲ���!!");
					sMsg.nMsgLen=TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				}
				break;

			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}


void KProtocolProcess::s2cChangeWeather(BYTE* pMsg)
{
//	g_ScenePlace.ChangeWeather(((SYNC_WEATHER*)pMsg)->WeatherID);
}

void	KProtocolProcess::s2cPKSyncNormalFlag(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PK_NORMAL_FLAG_SYNC	*pFlag = (PK_NORMAL_FLAG_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetNormalPKState(pFlag->m_btFlag);
}

void	KProtocolProcess::s2cPKSyncEnmityState(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PK_ENMITY_STATE_SYNC	*pState = (PK_ENMITY_STATE_SYNC*)pMsg;
	char	szName[32];

	memset(szName, 0, sizeof(szName));
	//memcpy(szName, pState->m_szName, pState->m_wLength + 1 + sizeof(pState->m_szName) - sizeof(PK_ENMITY_STATE_SYNC));
	sprintf(szName, "%s", pState->m_szName);
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetEnmityPKState(pState->m_btState, pState->m_dwNpcID, szName);
}

void	KProtocolProcess::s2cPKSyncExerciseState(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PK_EXERCISE_STATE_SYNC	*pState = (PK_EXERCISE_STATE_SYNC*)pMsg;
	char	szName[32];
	memset(szName, 0, sizeof(szName));
	//memcpy(szName, pState->m_szName, pState->m_wLength + 1 + sizeof(pState->m_szName) - sizeof(PK_EXERCISE_STATE_SYNC));
	sprintf(szName, "%s", pState->m_szName);
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetExercisePKState(pState->m_btState, pState->m_dwNpcID, szName);
}

void	KProtocolProcess::s2cPKValueSync(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PK_VALUE_SYNC	*pValue = (PK_VALUE_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cPK.SetPKValue(pValue->m_nPKValue);
}

void	KProtocolProcess::s2cReputeValueSync(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	REPUTE_VALUE_SYNC	*pValue = (REPUTE_VALUE_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cRepute.SetReputeValue(pValue->m_nReputeValue);
}

void	KProtocolProcess::s2cFuYuanValueSync(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	FUYUAN_VALUE_SYNC	*pValue = (FUYUAN_VALUE_SYNC*)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cFuYuan.SetFuYuanValue(pValue->m_nFuYuanValue);
}

void KProtocolProcess::s2crolesyncvipvalue(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].m_cVip.SetRoleVipValue(pMsg);
}

void	KProtocolProcess::s2cReBornValueSync(BYTE* pMsg)
{
	// REBORN_VALUE_SYNC	*pValue = (REBORN_VALUE_SYNC*)pMsg;
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].m_cReBorn.SetReBornValue(pMsg);	   //pValue->m_nReBornValue
}

//�ͻ��˴��̳�
void KProtocolProcess::OpenMarketBox(BYTE* pMsg)//ϵͳ�̳�
{
	if  (NULL==pMsg||NULL==this) return;
	SALE_MARKET_SYNC* pSale = (SALE_MARKET_SYNC *)pMsg;

	if (pSale==NULL)
		return;

	BuySell.OpenCMarket(pSale->nShopID);
}

void	KProtocolProcess::s2cTongCreate(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
      Player[CLIENT_PLAYER_INDEX].m_cTong.Create((TONG_CREATE_SYNC*)pMsg);
}
//���ù�������
void	KProtocolProcess::s2cNpcGoldChange(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	NPC_GOLD_CHANGE_SYNC	*pSync = (NPC_GOLD_CHANGE_SYNC*)pMsg;

	int nIdx = NpcSet.SearchID(pSync->m_dwNpcID);

	if (nIdx && (Npc[nIdx].m_Kind == kind_normal || Npc[nIdx].m_Kind ==kind_mouse || Npc[nIdx].m_Kind ==kind_bird))  //Ұ������
	{
		Npc[nIdx].m_cGold.SetGoldCurrentType((int)pSync->m_wGoldFlag);
	}
}
//�ͻ���ĥ�𾯸�
void	KProtocolProcess::ItemChangeDurability(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	ITEM_DURABILITY_CHANGE	*pIDC = (ITEM_DURABILITY_CHANGE *)pMsg;

	int nIdx = ItemSet.SearchID(pIDC->dwItemID);

	if (nIdx)
	{
		Item[nIdx].SetDurability(Item[nIdx].GetDurability() + pIDC->nChange); //���õ�ǰ�־�

		if (pIDC->nCanUse==1)
             Item[nIdx].SetIsCanUse(pIDC->nCanUse);

		if (Item[nIdx].GetDurability() >=0 && Item[nIdx].GetDurability() <= 3)  //С����
		{//����ĥ�����
			/*KSystemMessage	sMsg;
			//sMsg.eType         = SMT_NORMAL;
			//sMsg.byConfirmType = SMCT_NONE;
			//sMsg.byPriority    = 0;
			//sMsg.byParamSize   = 0;

			sMsg.byConfirmType = SMCT_CLICK;
			sMsg.byParamSize = 0;
			sMsg.byPriority = 1;
	        sMsg.eType = SMT_PLAYER;

			sprintf(sMsg.szMessage, MSG_ITEM_NEARLY_DAMAGED, Item[nIdx].GetName());
			sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0); */
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(MSG_ITEM_NEARLY_DAMAGED);
		}
	}
}
//�򿪴������
void	KProtocolProcess::OpenTremble(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CoreDataChanged(GDCNI_VIEW_TREMBLEITEM, NULL, NULL);
}
//���Ƹֽ���
void	KProtocolProcess::OpenWenGang(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CoreDataChanged(GDCNI_VIEW_RONGLIANEITEM, NULL, NULL);
}
//�򿪱�ʯ��Ƕ����
void	KProtocolProcess::Openstone(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	CoreDataChanged(GDCNI_VIEW_STONES, NULL, NULL);
}
//�򿪶������
void	KProtocolProcess::s2copenduanzhao(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
    PLAYER_DUANZHAO*	pList = (PLAYER_DUANZHAO *)pMsg;
	CoreDataChanged(GDCNI_OPEN_DUANZHAO,pList->m_nType,TRUE);
}


//��Ϸ����
void KProtocolProcess::s2cplyerguzicallback(BYTE* pMsg)
{
	/*
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_SYNCALLBACK_GUZI* pGuZi = (PLAYER_SYNCALLBACK_GUZI *)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_GuziGame.nIsOpen=pGuZi->m_nIsOpen;
	Player[CLIENT_PLAYER_INDEX].m_GuziGame.nTpye=pGuZi->m_Type;
	Player[CLIENT_PLAYER_INDEX].m_GuziGame.nYingL=pGuZi->m_nYinL;
	Player[CLIENT_PLAYER_INDEX].m_GuziGame.nJinBi=pGuZi->m_nJinBi;
	Player[CLIENT_PLAYER_INDEX].m_GuziGame.nDianShu=pGuZi->m_nDianShu;
	Player[CLIENT_PLAYER_INDEX].m_GuziGame.nBeiLv=pGuZi->m_Beilv;*/
}


//�Զ��ƶ�װ��
void KProtocolProcess::s2cpautoMovecallback(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
    PLAYER_AUTOCALLBACK_EQUIP*	pEquip = (PLAYER_AUTOCALLBACK_EQUIP *)pMsg;

	ItemPos SrcPos,DesPos;

	SrcPos.nPlace=pEquip->m_Splace;
	SrcPos.nX=pEquip->m_btSrcX;
	SrcPos.nY=pEquip->m_btSrcY;

	DesPos.nPlace=pEquip->m_Dplace;
	DesPos.nX=pEquip->m_btDestX;
	DesPos.nY=pEquip->m_btDestY;

	int	nSrcItemIdx=ItemSet.SearchID(pEquip->m_ItemDwId);
	if (nSrcItemIdx<=0 || nSrcItemIdx>=MAX_ITEM){
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("��������C,��װʧ��!");
		return;
	}
	int nSplace=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquPlace(Item[nSrcItemIdx].GetDetailType()); //�����װ����λ��
	int nEuqIdx=Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(nSplace); //װ�����Ƿ���װ��

	if 	(nEuqIdx && nSplace == itempart_ring1)
	{//ԭ��λ���ж��� �Ͻ���
		if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(itempart_ring2))
			nSplace = itempart_ring2;
	}

	if  (pEquip->m_Kind>=1)
	{//�����ж��װ��
		nSplace = Player[CLIENT_PLAYER_INDEX].m_ItemList._getEquipPlaceByItemIdx(nSrcItemIdx);
	}

	Player[CLIENT_PLAYER_INDEX].m_ItemList.AutoEquip(SrcPos,DesPos,pEquip->m_ItemDwId,nSplace,pEquip->m_Kind);
}


//ͬ��NPC��Ϣ
void KProtocolProcess::s2csynnpcinfocallback(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
    NPC_SYN_INFO_GETBACK*	pInfo = (NPC_SYN_INFO_GETBACK *)pMsg;

	int nNpcIdx=NpcSet.SearchID(pInfo->m_NpcDwid);

	if (nNpcIdx<=0)
		return;
	 Npc[nNpcIdx].m_CurrentFireResist=pInfo->m_TaskVala;
	 Npc[nNpcIdx].m_CurrentColdResist=pInfo->m_TaskValb;
	 Npc[nNpcIdx].m_CurrentPoisonResist=pInfo->m_TaskValc;
	 Npc[nNpcIdx].m_CurrentLightResist=pInfo->m_TaskVald;
	 Npc[nNpcIdx].m_CurrentPhysicsResist=pInfo->m_TaskVale;
	 Npc[nNpcIdx].m_CurrentFireResistMax=pInfo->m_TaskValf;
	 Npc[nNpcIdx].m_CurrentColdResistMax=pInfo->m_TaskValg;
	 Npc[nNpcIdx].m_CurrentPoisonResistMax=pInfo->m_TaskValh;
	 Npc[nNpcIdx].m_CurrentLightResistMax=pInfo->m_TaskVali;
	 Npc[nNpcIdx].m_CurrentPhysicsResistMax=pInfo->m_TaskValj;
}

//�򿪶������
void	KProtocolProcess::s2copenvipdazao(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
    PLAYER_VIPDAZAO*	pList = (PLAYER_VIPDAZAO *)pMsg;
	CoreDataChanged(GDCNI_OPEN_VIPDAZAO,pList->m_nType,pList->m_nCurExtPiont);
}

void KProtocolProcess::s2cJinMaicallback(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	PLAYER_JINMAI_GETBACK*	sTaskBack= (PLAYER_JINMAI_GETBACK *)pMsg;
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(3501,sTaskBack->m_TaskVala);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(3502,sTaskBack->m_TaskValb);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(3503,sTaskBack->m_TaskValc);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(3504,sTaskBack->m_TaskVald);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(3505,sTaskBack->m_TaskVale);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(3506,sTaskBack->m_TaskValf);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(3507,sTaskBack->m_TaskValg);
	Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(3508,sTaskBack->m_TaskValh);

	Player[CLIENT_PLAYER_INDEX].UpdataCurData();
}


//ͬ����ǰ����
void	KProtocolProcess::s2csyncurupdata(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	Player[CLIENT_PLAYER_INDEX].UpdataCurData();
}


void KProtocolProcess::NetCommandSetRankFF(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	DWORD	dwNpcId;
	RANKFF_SYNC	*NetCommand = (RANKFF_SYNC *)pMsg;
	dwNpcId = NetCommand->ID;//' *(DWORD *)&pMsg[1];
	int nIdx = NpcSet.SearchID(dwNpcId);

	if (nIdx > 0)
	{
		Npc[nIdx].m_btRankFFId = (BYTE)NetCommand->RankFF;//(int)pMsg[5];
		Npc[nIdx].m_SyncSignal = SubWorld[0].m_dwCurrentTime;
	}
}
void KProtocolProcess::LadderList(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;

	LADDER_LIST*	pList = (LADDER_LIST *)pMsg;
	KRankIndex		*pLadderListIndex = NULL;

	if (pList->nCount > 0 && pList->nCount < enumLadderEnd)
	{
		pLadderListIndex = new KRankIndex[pList->nCount];
	}
	if (pLadderListIndex)
	{
		for (int i = 0; i < pList->nCount; i++)
		{
			pLadderListIndex[i].usIndexId = pList->dwLadderID[i];
			pLadderListIndex[i].bValueAppened = true;
			pLadderListIndex[i].bSortFlag = true;
		}
		CoreDataChanged(GDCNII_RANK_INDEX_LIST_ARRIVE, pList->nCount, (intptr_t)pLadderListIndex);
		delete [] pLadderListIndex;
		pLadderListIndex = NULL;
	}
}

void KProtocolProcess::LadderResult(BYTE* pMsg)
{
	if  (NULL==pMsg||NULL==this) return;
	LADDER_DATA*	pLadderData = (LADDER_DATA *)pMsg;
	KRankMessage*	pLadderMessage = NULL;

	pLadderMessage = new KRankMessage[10];
	if (pLadderMessage)
	{
		for (int i = 0; i < 10; i++)
		{
			pLadderMessage[i].usMsgLen = strlen(pLadderData->StatData[i].Name);
			strcpy(pLadderMessage[i].szMsg, pLadderData->StatData[i].Name);
			pLadderMessage[i].nValueAppend = pLadderData->StatData[i].nValue;
			pLadderMessage[i].cSortFlag = (char)pLadderData->StatData[i].bySort;
		}
		unsigned int uParam = 10 | (((WORD)pLadderData->dwLadderID) << 16);
		CoreDataChanged(GDCNII_RANK_INFORMATION_ARRIVE, uParam, (intptr_t)pLadderMessage);
		delete [] pLadderMessage;
		pLadderMessage = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
