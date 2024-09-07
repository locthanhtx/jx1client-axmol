#include "KCore.h"
#include "KObjSet.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KNpc.h"
#include "engine/KIniFile.h"
#include "KSubWorldSet.h"
#include "KMissleSet.h"
#include "LuaFuns.h"
#include "KPlayerSet.h"
#include "KPlayer.h"
/////////////������/////////////////
#include  <time.h>
#include "engine/KTimer.h"
#include "KSortScript.h"
//#include "KScriptValueSet.h"
// #ifdef WIN32
// #include <ppl.h>
// #endif
//#include "KNpcTemplate.h"
//extern KNpcTemplate	* g_pNpcTemplate[MAX_NPCSTYLE][MAX_NPC_LEVEL]; //0,0Ϊ���
///////////////////////////////////
KSubWorldSet g_SubWorldSet;

KSubWorldSet::KSubWorldSet()
{
	//m_TaskTimer.Start();
	m_nLoopRate = 0;   //ѭ����֡�� 18֡Ϊһ��
	m_dwPing    = 0;
	//messageBox("m_nLoopRate is null","KSubWorldSet");
}

////////////////////////////////////��ͼִ�нű�//////////////////////////////////////

int KSubWorldSet::SubExecuteScriptA(char * ScriptFileName, char * szFunName, int nParam,char * szCanshu)
{//g_FileName2Id(ScriptFileName)
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	char nNewScriptName[128]={0};
	sprintf(nNewScriptName,"%s",ScriptFileName);
	g_StrLower(nNewScriptName);//g_StrLower
	return SubExecuteScriptB(nNewScriptName, szFunName, nParam,szCanshu);
}

int KSubWorldSet::SubExecuteScriptB(char* dwScriptIdA,  char * szFunName, int nParam,char * szCanshu)
{
	KSubWorld nIndex;

		bool bExecuteScriptMistake = true;
		//����һ����ջ
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptIdA); //dwScriptId
		int nTopIndex = 0;  //��¼λ�� ��ջ�ף�

		if (pScript==NULL)
		{
			printf("-------�ű�������,ִ��[%s][%s]ʧ��!-------- \n",dwScriptIdA,szFunName);
			return FALSE;
		}
	try
	{
		if (pScript)
		{	//��ջ��ѹ��һ��int����

		    nTopIndex=pScript->SafeCallBegin();

		    if (pScript->CallFunction(szFunName,0,"ds",nParam,szCanshu))
			{
				bExecuteScriptMistake = false;
			}

           // lua_pop(pScript->m_LuaState, -1); //��ջ������1��Ԫ�� -1���ջ
			nTopIndex=pScript->SafeCallBegin();
			pScript->SafeCallEnd(nTopIndex);
		    //lua_pop(pScript->m_LuaState, -1); //��ջ������1��Ԫ�� -1���ջ
		//	lua_close(pScript->m_LuaState);   //�ͷ��ڴ�
		}
		//lua_pop(pScript->m_LuaState, -1); //��ջ������1��Ԫ�� -1���ջ
		if (bExecuteScriptMistake)
		{
		    pScript=NULL;
			return FALSE;
		}

		pScript=NULL;
		return TRUE;
	}
	catch(...)
	{

		if (pScript)
		{
			nTopIndex=pScript->SafeCallBegin();
			pScript->SafeCallEnd(nTopIndex);
		}

		printf("ִ�нű���������E,Script[%s]!\n",szFunName);
		return FALSE;
	}
	return TRUE;
}

int	KSubWorldSet::SubExecuteScript(unsigned long dwScriptId, char * szFunName, char *  szParams,char * snParams,int inParams,int njb,int njxb )
{
	KSubWorld nIndex;
	KNpc   m_Index;

		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if (pScript==NULL)
		{
			printf("-------�ű�������,ִ��[%s]ʧ��!-------- \n",szFunName);
			return FALSE;
		}

		int nTopIndex = 0;
	 try
	 {
		if (pScript)
		{
		//	Lua_PushNumber(pScript->m_LuaState, nIndex.m_nIndex);
		//	pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
		     nTopIndex=pScript->SafeCallBegin();
			if ((!szParams) || !szParams[0])
			{//���û�в���	 �����һ��Ϊ�գ���ִ�пղ���
				 if (pScript->CallFunction(szFunName,0,""))
				 {
					 bExecuteScriptMistake = false;
				 }
			}
		    else if (pScript->CallFunction(szFunName,0,"ssdddd",szParams,snParams,inParams,njb,njxb,0))  //�в���
			{
				bExecuteScriptMistake = false;
			}
			//}
		   nTopIndex=pScript->SafeCallBegin();
		   pScript->SafeCallEnd(nTopIndex);
         //#define Lua_Release(L)					lua_close(L)
		//	lua_close(pScript->m_LuaState); //�ͷ��ڴ�
		//lua_pop(pScript->m_LuaState, -1); //��ջ������1��Ԫ�� -1���ջ
		}
		//lua_pop(pScript->m_LuaState, -1); //��ջ������1��Ԫ�� -1���ջ
		if (bExecuteScriptMistake)
		{//ִ�нű�ʧ�ܺ󣬾����
			pScript=NULL;
			return FALSE;
		}
		return TRUE;
	}
	catch(...)
	{
		if (pScript)
		{
			nTopIndex=pScript->SafeCallBegin();
			pScript->SafeCallEnd(nTopIndex);
			//pScript->Exit();
		}
		printf("ִ�нű���������D,Script[%s],[%d]!\n",szFunName);
		return FALSE;
	}
	return TRUE;
}

int	KSubWorldSet::SubExecuteScript(char * ScriptFileName, char * szFunName, char *  szParams,char *  snParams, int inParams,int njb,int njxb)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	unsigned long dwScriptId = g_CheckFileExist(ScriptFileName);

	KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
	if (pScript==NULL)
	{
		printf("-------�ű�������,ִ��[%s][%s]ʧ��!-------- \n",ScriptFileName,szFunName);
		return FALSE;
	}
	pScript=NULL ;

	return SubExecuteScript(dwScriptId, szFunName, szParams,snParams,inParams,njb,njxb);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int KSubWorldSet::SearchWorld(unsigned long dwID)
{
	for (int i = 0; i < MAX_SUBWORLD; ++i)
	{
		if ((unsigned long)SubWorld[i].m_SubWorldID == dwID)
			return i;
	}
	return -1;
}

int KSubWorldSet::SearchWorldRows(int dwID)
{
//	for (int i = 0; i < MAX_SUBWORLD; ++i)
//	{
	if (dwID<MAX_SUBWORLD)
		return (unsigned long)SubWorld[dwID].m_SubWorldID;
//	}
	return -1;
}
///////////////////////////////////////////////////////////////////////////////////////////
//static CLogFile gs_LogFile;
/*
int KSubWorldSet::LoadSerMaps(char* szFileName)
{
   KIniFile IniFile;
   char szKeyName[32];
   int nWorldID,nWorldCount = 0,nSucess = 0,nFail = 0;
   int nNpcmun=0,nZnNpcmun=0,nZnNpcmunA=0;
   IniFile.Load(szFileName);
   IniFile.GetInteger("Init", "Count", 1, &nWorldCount);
   if (nWorldCount > MAX_SUBWORLD)
   {
      printf("----��ͼ���س�������:��������:%d,��������:%d-----\n", nWorldCount,MAX_SUBWORLD);
      IniFile.Clear();
      return FALSE;
   }

    printf("Loading File Map: %s\n", szFileName);

    for (int i = 0; i < nWorldCount; ++i)
	{
      sprintf((char*)szKeyName, "World%02d", i);
      IniFile.GetInteger("World", szKeyName,5000, &nWorldID);
}
      printf("Map Load Sucessed: %d Map,��NPC����:%d\n", nSucess,nZnNpcmun);
      printf("Map Load Failed: %d Map \n", nFail);
	  IniFile.Clear();
      return TRUE;
}
*/
//int nActiveRegionCount;
//�ͻ��˺� �������� �������ú���---��Ϸ��ѭ��
void KSubWorldSet::MainLoop()
{

	++m_nLoopRate;
	// ��Ϸ���ֵĴ���
	//this->m_cMusic.Play(SubWorld[0].m_SubWorldID, SubWorld[0].m_dwCurrentTime, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode);

	for (int i = 0; i < MAX_SUBWORLD; ++i) //�������� ��ͼ�б�
	{
		SubWorld[i].Activate();
		NpcSet.CheckBalance();             //����ͬ����һ�µ�NPC ��ɾ����
	}
}

unsigned int KSubWorldSet::getCilentRunTime()
{
	unsigned int dwTimer=0;
#ifdef WIN32
	dwTimer = GetTickCount(); //��ȡϵͳ���е�ʱ��
#else
	timeval  m_pStartUpdate;
	gettimeofday(&m_pStartUpdate,NULL);
	dwTimer = m_pStartUpdate.tv_sec*1000+m_pStartUpdate.tv_usec/1000;
#endif
	//dwTimer >>= 1;
	return dwTimer;
}

//��ͼ��Ϣ����ѭ��
void KSubWorldSet::MessageLoop()
{

	for (int i = 0; i < MAX_SUBWORLD; ++i)
	{
		SubWorld[i].MessageLoop();
	}
}

int KSubWorldSet::SendMessage(int nSubWorldID, unsigned long dwMsgType, int nParam1, int nParam2, int nParam3)
{
	/*KWorldMsgNode *pNode = NULL;

	pNode = new KWorldMsgNode;
	if (!pNode)
		return FALSE;

	pNode->m_dwMsgType	= dwMsgType;
	pNode->m_nParam[0]	= nParam1;
	pNode->m_nParam[1]	= nParam2;
	pNode->m_nParam[2]	= nParam3;

	return SubWorld[nSubWorldID].m_WorldMessage.Send(pNode);*/
	return false;
}
//�뿪��Ϸʱ���õĺ���
void KSubWorldSet::Close()
{
	for (int i = 0; i < MAX_SUBWORLD; ++i)
	{
		SubWorld[i].Close();
	}
	NpcSet.RemoveAll(); //����ɾ��
	Player[CLIENT_PLAYER_INDEX].m_ItemList.RemoveAll();  // ɾ���ͻ���ȫ����Ʒ
	Player[CLIENT_PLAYER_INDEX].m_cTeam.Release();
	g_Team[0].Release();
	Player[CLIENT_PLAYER_INDEX].Release();
	//m_cMusic.Stop();
}

/*#ifndef _SERVER
void KSubWorldSet::Paint()
{
	SubWorld[0].Paint();
}
#endif*/
