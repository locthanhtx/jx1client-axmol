/*******************************************************************************
// FileName			:	ScriptFuns.cpp
// FileAuthor		:	RomanDou
// FileCreateDate	:	2002-11-19 15:58:20
// FileDescription	:	�ű�ָ�
// Revision Count	:
*******************************************************************************/
/*#ifndef WIN32
#include <string>
#endif*/
#include "engine/KEngine.h"
#include "engine/KTabFileCtrl.h"
#include "engine/KStepLuaScript.h"
#include "engine/lualib/LuaLib.h"
#include "engine/KScriptList.h"
#include "engine/Text.h"
#include <string>
#include "LuaFuns.h"
#include "KCore.h"
#include "KNpc.h"
#include "KSubWorld.h"
#include "KObjSet.h"
#include "KItemSet.h"
//#include "KNetClient.h"
#include "KScriptValueSet.h"
#include "KNpcSet.h"
#include "KPlayerSet.h"
#include "KPlayer.h"
//#include "KPlayerTong.h"
#include "KSubWorldSet.h"
//#include "KProtocolProcess.h"
#include "KBuySell.h"
#include "KTaskFuns.h"
#include "KPlayerDef.h"
//#include "KForBitGua.h"
////ԭ��û�е�//////////////////////
#include "KProtocolDef.h"
//#include "KProtocol.h"
#include "KRelayProtocol.h"
#include "KTongProtocol.h"
#include "KInventory.h"
#include "coreshell.h"
#include <vector>
#include <cassert>
#include "KNpcTemplate.h"
#include <time.h>
/////////////////////////////////////
#include "TaskDef.h"
#include "KSortScript.h"
#ifdef WIN32
#include <direct.h>
#else
#include "unistd.h"
#endif

/*#ifndef __linux
#include "Shlwapi.h"
#include "windows.h"
#include "winbase.h"
#include <direct.h>
#else
#include "unistd.h"
#endif*/

#include "engine/KSG_StringProcess.h"


/*#ifndef WIN32
typedef struct  _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}	SYSTEMTIME;
typedef struct  _FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
}	FILETIME;
#endif*/

#ifdef WIN32
inline const char* _ip2a(DWORD ip) { in_addr ia; ia.s_addr = ip; return inet_ntoa(ia); }
inline DWORD _a2ip(const char* cp) { return inet_addr(cp); }
#endif

KTabFile        g_MineItemTab;
KScriptList		g_StoryScriptList;
KStepLuaScript * LuaGetScript(Lua_State * L);
int	       GetPlayerIndex(Lua_State * L);
extern int g_GetPriceToStation(int,int);
extern int g_GetPriceToWayPoint(int,int);
extern int g_GetPriceToDock(int ,int );
//BitValue = GetBit(Value, BitNo)
void UseSkills(int sKillID,int sLevel,int nNpcIdx,KMagicAttrib *pData=NULL);
int  GetQuestSetIdx(int sVal);
static void KSceGetFliePathc(char* nName,char * nPath,int nMaxRows)
{
	KFile  nFile;
	KTabFileCtrl nScirptFile;
	char nTongApplyPath[125]={0},szCol[128]={0};
	sprintf(nTongApplyPath,"%s","\\clientscript.log");
	if (!g_FileExists(nTongApplyPath))	//�Ƿ����
	{
		nFile.Create(nTongApplyPath);
		sprintf(szCol,"�ű�log\15\n");
		nFile.Write(szCol, sizeof(szCol));
		//nFile.Save(nTongApplyPath);
		nFile.Close();
	}

	if (nScirptFile.Load(nTongApplyPath))
	{
		int nRows=nScirptFile.GetHeight();
		if  (nRows==0)
			nRows=1;

		if (nRows>=nMaxRows)
		{
			while(nScirptFile.GetHeight()>0)
			{
				nScirptFile.Remove(nScirptFile.GetHeight());
				nScirptFile.Save(nTongApplyPath);
			}
		//nScirptFile.Clear();
		//retrn;
		}

		sprintf(szCol,"--%s:(%s)--",nName,nPath);
		nScirptFile.InsertAfter(nRows);
		nScirptFile.WriteString(nRows,1, szCol);                                    //�к�
		//nTong.WriteString(nRows,2,Npc[Player[m_nPlayerIndex].m_nIndex].Name);     //����
		//nTong.WriteInteger(nRows,3,Npc[Player[m_nPlayerIndex].m_nIndex].m_Level); //�ȼ�
		nScirptFile.Save(nTongApplyPath);
		//nFile.Write(szCol, sizeof(szCol));
	}
	nScirptFile.Clear();
}

int GetPlayerIndex(Lua_State * L)
{
	Lua_GetGlobal(L, SCRIPT_PLAYERINDEX);
	if (lua_isnil(L,Lua_GetTopIndex(L)))
		return -1;
	int nIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	if (nIndex >= MAX_PLAYER || nIndex <= 0)
	{
		return -1;
	}
	if (Player[nIndex].m_nIndex >= MAX_NPC || Player[nIndex].m_nIndex < 0)
	{
		//_ASSERT(0);
		return -1;
	}
	return nIndex;
}

int GetObjIndex(Lua_State * L)
{
	Lua_GetGlobal(L, SCRIPT_OBJINDEX);
	if (lua_isnil(L,Lua_GetTopIndex(L)))
		return -1;
	int nIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	if (nIndex >= MAX_OBJECT || nIndex <= 0)
	{
		//	_ASSERT(0);
		return -1;
	}
	if (KObject[nIndex].m_nIndex != nIndex)
	{
		//	_ASSERT(0);
		return -1;
	}
	return nIndex;
}

int LuaGetBit(Lua_State * L)
{
	int nBitValue = 0;
	int nIntValue = (int)Lua_ValueToNumber(L, 1);
	int nBitNumber = (int)Lua_ValueToNumber(L, 2);

	if (nBitNumber >= 32 || nBitNumber <= 0)
		goto lab_getbit;
	nBitValue = (nIntValue & (1 << (nBitNumber - 1))) != 0;
lab_getbit:
	Lua_PushNumber(L, nBitValue);
	return 1;
}

//NewBit = SetBit(Value, BitNo, BitValue)
int LuaSetBit(Lua_State * L)
{
	int nIntValue = (int)Lua_ValueToNumber(L, 1);   //ֵ
	int nBitNumber = (int)Lua_ValueToNumber(L, 2);  //λ��
	int nBitValue = (int)Lua_ValueToNumber(L,3);

	nBitValue = (nBitValue == 1);

	if (nBitNumber > 32 || nBitNumber <= 0) //32λ
		goto lab_setbit;

	nIntValue = (nIntValue | (1 << (nBitNumber - 1)));
lab_setbit:
	Lua_PushNumber(L, nIntValue);
	return 1;
}
//NewByte = SetByte(Value, ByteNo, ByteValue)
int LuaSetByte(Lua_State * L)
{
	BYTE * pByte =	NULL;
	int nIntValue = (int)Lua_ValueToNumber(L, 1);
	int nByteNumber = (int)Lua_ValueToNumber(L, 2);
	int nByteValue = (int)Lua_ValueToNumber(L,3);

	nByteValue = (nByteValue & 0xff);	//0xff����ʮ����Ϊ255

	if (nByteNumber > 4 || nByteNumber <= 0) ///4
		goto lab_setByte;

	pByte =(BYTE*)&nIntValue;	 //
	*(pByte + (nByteNumber -1)) = (BYTE)nByteValue;//
	//nIntValue = (nIntValue | (0xff << ((nByteNumber - 1) * 8) )) ;
lab_setByte:
	Lua_PushNumber(L, nIntValue);
	return 1;
}

//ByteValue = GetByte(Value, ByteNo)
int LuaGetByte(Lua_State * L)
{
	int nByteValue = 0;
	int nIntValue = (int)Lua_ValueToNumber(L, 1);     //��Ҫת����ֵ
	int nByteNumber = (int)Lua_ValueToNumber(L, 2);   //���� 4��ƫ��

	if (nByteNumber > 4 || nByteNumber <= 0)
		goto lab_getByte;

	nByteValue = (nIntValue & (0xff << ((nByteNumber - 1) * 8) )) >> ((nByteNumber - 1) * 8);

lab_getByte:
	Lua_PushNumber(L, nByteValue);
	return 1;
}

int GetSubWorldIndex(Lua_State * L)
{
	Lua_GetGlobal(L, SCRIPT_SUBWORLDINDEX);
	if (lua_isnil(L,Lua_GetTopIndex(L)))
        return -1;
	int nIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
    if (nIndex >= MAX_SUBWORLD || nIndex <0)
	{
		//_ASSERT(0);
		return -1;
	}
	if (SubWorld[nIndex].m_nIndex >= MAX_SUBWORLD || SubWorld[nIndex].m_nIndex < 0)
	{
		//_ASSERT(0);
		return -1;
	}
    return nIndex;
}

//Idx = SubWorldID2Idx(dwID)
int LuaSubWorldIDToIndex(Lua_State * L)
{
	int nTargetSubWorld = -1;
	int nSubWorldID = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_subworldid2idx;

	nSubWorldID = (int)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = g_SubWorldSet.SearchWorld(nSubWorldID); // ��ǰ�ĵ�ͼ���ڷ�������ͼ�б�ĵڼ���(����)

lab_subworldid2idx:
	Lua_PushNumber(L, nTargetSubWorld);
	return 1;
}

int LuaIndexToSubWorldID(Lua_State * L)
{
	int nTargetSubWorld = -1;
	int nSubWorldindex = 0;
	if (Lua_GetTopIndex(L) < 1)
		goto lab_subworldid2idx;

	nSubWorldindex = (int)Lua_ValueToNumber(L, 1);
	nTargetSubWorld = g_SubWorldSet.SearchWorldRows(nSubWorldindex);

lab_subworldid2idx:
	Lua_PushNumber(L, nTargetSubWorld);
	return 1;
}


int LuaSetOrAddTrap(Lua_State * L)
{
    int nMapIdx,nXpos,nYpos,nCellNum,nReg=0;
	DWORD nScriptidx;
	      nMapIdx = (int)Lua_ValueToNumber(L,1);
		  nXpos = (int)Lua_ValueToNumber(L,2);
		  nYpos = (int)Lua_ValueToNumber(L,3);
		  nCellNum = (int)Lua_ValueToNumber(L,4);
		  nScriptidx = g_CheckFileExist((char *)Lua_ValueToString(L,5));

          if (nScriptidx==0)
		  {
			  Lua_PushNumber(L,nReg);
	          return 1;
		  }

		  int nRegion, nMapX, nMapY, nOffX, nOffY;

		  SubWorld[nMapIdx].Mps2Map(nXpos, nYpos, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);

		  if (nRegion == -1)
		  {
			  Lua_PushNumber(L,nReg);
			  return 1;
		  }

	      //return m_Region[nRegion].SetTrap(nMapX, nMapY,nCellNum,uTrapScriptId);
          //nReg = SubWorld[nMapIdx].SetTrap(nXpos,nYpos,nCellNum,nScriptidx);
		  nReg = SubWorld[nMapIdx].m_Region[nRegion].SetTrap(nMapX,nMapY,nCellNum,nScriptidx);
/*
          int nPlayerIndex   = GetPlayerIndex(L);

          if (nPlayerIndex>0)
		  {

#ifdef _SERVER
			  char msg[64];
			  sprintf(msg,"<color=yellow>��ǰ����:R:%d,X:%d,Y:%d,Tr:%d",nRegion,nMapX,nMapY,nScriptidx);
              //Player[nPlayerIndex].m_ItemList.msgshow(msg);
			  //char nMsg[250];
			  //g_StrCpyLen(nMsg,  Lua_ValueToString(L,1), sizeof(nMsg));
			  int nleg=TEncodeText(msg, strlen(msg));
			  const char * szMsg = msg;
			  if (szMsg)
			  {
				  KPlayerChat::SendSystemInfo(0, 0, "����:", (char *)szMsg, nleg);  //strlen(szMsg)
			  }

#endif
		  }
*/
          Lua_PushNumber(L,nReg);
	      return 1;
}

int LuaSubWorldName(Lua_State * L)
{
	int nMapId;
	nMapId = (int)Lua_ValueToNumber(L,1);
    Lua_PushString(L,SubWorld[nMapId].nWorldMapInfo[STR_MAP_NAME].c_str());
	return 1;
}


/*
Say(sMainInfo, nSelCount, sSel1, sSel2, sSel3, .....,sSeln)
Say(nMainInfo, nSelCount, sSel1, sSel2, sSel3, .....,sSeln)
Say(nMainInfo, nSelCount, SelTab)
����ǿͻ��˵��򲻻���������˷����κβ���

  Say(100, 3, 10, 23,43)
  Say("ѡ��ʲô��", 2, "��/yes", "��/no");
  Say("ѡʲôѽ", 2, SelTab);
*/
//**************************************************************************************************************************************************************
//												����ű�
//**************************************************************************************************************************************************************

int ExtractChars(const char *inStr,char scrchar,char destchar,char *outStr=NULL,char *outStra=NULL,int nMoedel=0)
{
	char *tmp=NULL,*tmpa=NULL;         //����һ����ʱ����ռ䣬����ַ���
	tmp=outStr,tmpa=outStra;
	int nLen=0;
	while(*inStr!='\0')
	{//һֱǰ��,ֱ���н������Ž�ֹͣ��
		if(*inStr==scrchar && nMoedel==0) //�����еĵ�һ��Ԫ���뿪ʼ�ַ���ͬ
		{
			//*inStr='|';                 //�滻����ʼ�ַ�
			inStr++;                      //���˵���ʼ�ַ���ָ���¸������бȽϡ�
           	while(*inStr!='\0')
			{//����ǰ��
                  if (*inStr==destchar)   //�����еĵڶ���Ԫ��������ַ���ͬ
				  {
					  //*inStr='|';       //�滻�������ַ�
                      inStr++;            //���˵���������ָ���¸�����
					  break;              //�ҵ��������� ���˵� ����ѭ����
				  }
				  *tmpa=*inStr;           //��ȡ�ַ��Ž�tempa��
				  tmpa++;	              //��һ��Ԫ��
	              inStr++;                //��һ��Ԫ��
			}
			//inStr++;                   //ȥ����������
			*tmpa='\0';                  //��ȡ��󣬼Ӹ���������
		}
		else if (*inStr==scrchar && nMoedel==1)  //����ĳ���ַ����������ַ���
		{
              break;
		}
		if (*inStr=='\0')                //���û���ҵ��������������Ѿ����˽�β���������ˣ���
			break;
		*tmp=*inStr;                     //Ԫ������ͬ�����ŵ�tmp�С�
		tmp++;                           //������һ��Ԫ���ıȽ�
		inStr++;
		nLen++;
	}
	*tmp='\0';                           //Դ�ַ��� ��ȡ��� �Ӹ������� ��ֹ����
    return nLen;
}

int LuaSayUI(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	char * pContent = NULL;

	int nPlayerIndex ;
       nPlayerIndex   = CLIENT_PLAYER_INDEX;
	if (nPlayerIndex < 0) return 0;

//	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;

	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 2) return 0;

	if (Lua_IsNumber(L,2))
	{
		nOptionNum = (int)Lua_ValueToNumber(L,2);
	}
	else
	{
	//	_ASSERT(0);
		return 0;
	}

	if  (Lua_IsNumber(L,1))
	{
		nMainInfo = (int)Lua_ValueToNumber(L,1);
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 1)) 	//�������Ϣ���ַ��������ַ�����ʶ��
	{
		strMain = (char *)Lua_ValueToString(L, 1);
		nDataType = 0 ;
	}
	else
		return 0;

	BOOL bStringTab = FALSE;//��ʶ��������ѡ�����ݴ����һ�������У���������ַ�����

	if (Lua_IsString(L,3))
		bStringTab = FALSE;
	else if (Lua_IsTable(L, 3))   // ��һ�����
	{
		bStringTab = TRUE;
	}
	else
	{if (nOptionNum > 0)  return 0;
	}

	if (bStringTab == FALSE)
	{
		//���ʵ�ʴ����ѡ��ĸ���
		if (nOptionNum > nParamNum - 2) nOptionNum = nParamNum - 2;
	}

	if (nOptionNum > MAX_ANSWERNUM) nOptionNum = MAX_ANSWERNUM;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId        = UI_SELECTDIALOG;
	UiInfo.m_bParam1      = nDataType;//����Ϣ�����ͣ��ַ���(0)������(1)
	UiInfo.m_bOptionNum   = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select       = 0;
//	Player[nPlayerIndex].m_nCheckWaiGua =0;

	//����ϢΪ�ַ���
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		pContent = UiInfo.m_pContent;
	}
	else if (nDataType == 1) //����ϢΪ���ֱ�ʶ
	{
		*(int *)UiInfo.m_pContent = nMainInfo;
		pContent = UiInfo.m_pContent + sizeof(int);
		*pContent = 0;
	}

	if (nOptionNum > MAX_ANSWERNUM)
		nOptionNum = MAX_ANSWERNUM;

	Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionNum;

	for (int i  = 0; i < nOptionNum; i ++)
	{
		char  pAnswer[64];
		ZeroMemory(pAnswer,sizeof(pAnswer));

		if (bStringTab)   //������������Table
		{
			Lua_PushNumber(L, i + 1);
			Lua_RawGet(L, 3);	 //��ջ����������
			char * pszString = (char *)Lua_ValueToString(L, Lua_GetTopIndex(L));
			if (pszString)
			{
				g_StrCpyLen(pAnswer, pszString, 64);
			}
		}
		else
		{
			char * pszString = (char *)Lua_ValueToString(L, i + 3);
			if (pszString)
				g_StrCpyLen(pAnswer, pszString, 64);
		}

		char * pFunName = strstr(pAnswer, "/"); //�ж� �ַ��� �״γ��֡�/�� ��λ��!

		/*if (pFunName)
		{
			char *mpFunName=strstr(pFunName, "#");  //  stristr() �Դ�Сд������  ȡ���沿��
			if (mpFunName)
			{ //ֻ�ǲ��Ҵ��������Ҫ�õ��ַ�������strposЧ����ߡ�
				char nmpFunName[32];
				char nmCanshu[32];
				//strpos();
				ExtractChars(mpFunName,'(',')',nmpFunName,nmCanshu);
				g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i], nmCanshu, sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
				g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i], nmpFunName+1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
				ExtractChars(pAnswer,'/',')',nmpFunName,nmCanshu,1);
				sprintf(pContent, "%s|%s", pContent, nmpFunName);
				ZeroMemory(nmpFunName,sizeof(nmpFunName));
			    ZeroMemory(nmCanshu,sizeof(nmCanshu));
			}
			else
			{
				g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i],"", sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
				g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
				*pFunName = 0;   //�������
				sprintf(pContent, "%s|%s", pContent, pAnswer);
			}
		}*/
		if (pFunName)
		{
			char *mpFunName=strstr(pFunName, "#");  //  stristr() �Դ�Сд������  ȡ���沿�֣��������ţ�
			if (mpFunName)
			{ //sscanf(nItemID, "%s(%s", &nTextA, &nTextB); �ַ����ָ�
				char nmpFunName[32];
				char nmCanshu[32];
				//nmpFunName=strtok(mpFunName,"-");  //�ַ����ָ�   ����
				//nmCanshu=strtok(NULL,"-");        //����
				ExtractChars(mpFunName,'(',')',nmpFunName,nmCanshu);
				//					 //���Я���� �������� �� ����
				g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i],nmpFunName+1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
				g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i],nmCanshu, sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
				//nmpFunName=strtok(pAnswer, "/"); //ȥ�� б��  //ȡǰ��Ĳ���
				ExtractChars(pAnswer,'/',')',nmpFunName,nmCanshu,1);
				sprintf(pContent, "%s|%s", pContent,nmpFunName);
				ZeroMemory(nmpFunName,sizeof(nmpFunName));
				ZeroMemory(nmCanshu,sizeof(nmCanshu));
			}
			else
			{
				g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i],"", sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
				g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
				*pFunName = 0;   //�������
				sprintf(pContent, "%s|%s", pContent, pAnswer);
			}
		}
		else
		{
			strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i], "main");
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
	}

	if (nDataType == 0)
		UiInfo.m_nBufferLen  = strlen(pContent);
	else
		UiInfo.m_nBufferLen = strlen(pContent) + sizeof(int);

	UiInfo.m_bParam2 = 0;

/*	if (nOptionNum == 0)
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	} */

	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}


//��������� �ض���ʾSPR�ĶԻ���
int LuaCreateSprSay(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	int nIsVal=0;
	int nIsTimeClose=0;
	char * pContent = NULL;
	char * pOptionStr=NULL;

	int nPlayerIndex ;
       nPlayerIndex   = CLIENT_PLAYER_INDEX;
	   if (nPlayerIndex < 0) return 0;

//	   Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;

	   int nParamNum = Lua_GetTopIndex(L);

	   if (nParamNum < 1)
		   return 0;


	   PLAYER_SCRIPTACTION_SYNC UiInfo;
	   ZeroStruct(UiInfo);

	   UiInfo.m_bUIId        = UI_SELECTDIALOG;
	   UiInfo.m_bParam1      = nDataType ;//����Ϣ�����ͣ��ַ���(0)������(1)
	   // UiInfo.m_bOptionNum   = nOptionNum;
	   UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	   UiInfo.m_Select       = 1;


	   if (Lua_IsTable(L, 1))  //�������
	   {
		       nOptionNum = Lua_GetN(L,1);       //�����ж��ٸ���

	         if (nOptionNum > MAX_ANSWERNUM+2)
		         nOptionNum = MAX_ANSWERNUM+2;

			   Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionNum-2;  //ѡ����	 ��ȥ��һ�� ����˵��

			   UiInfo.m_bOptionNum   = nOptionNum-2;

			   for (int i=0;i<nOptionNum+2;i++)  //�����table
			   {
				   char pAnswer[64];
				   ZeroMemory(pAnswer,sizeof(pAnswer));

				   Lua_PushNumber(L,i + 1); //ѹ��һ��Key
				   //lua_settable(L, -3);   //����KEYֵ,Ȼ���ջ
				   Lua_RawGet(L, 1);        //ȡ�������ű��������ĵ�һ�������������ӦTABLE��KEYֵ Ȼ���ջ
				   char * pszString = (char *)Lua_ValueToString(L, Lua_GetTopIndex(L));
				   //nCurNum=4;
				   if (pszString)
				   {
					   if (i==0)
					   {//NPC spr ·��
						   sprintf(UiInfo.m_szSprPath, "%s", pszString);
						   continue;
					   }
					   else if (i==1)
					   {//��Ҫ����
						   sprintf(UiInfo.m_pContent, "%s", pszString);
						   pContent = UiInfo.m_pContent;
						   continue;
					   }
					   else
					     g_StrCpyLen(pAnswer, pszString, 64);

					   //i=i-1; //�ָ����

					   char * pFunName = strstr(pAnswer, "/");  // �����״γ��� б�ܳ��ֵ�λ�� ����  �Դ�Сд����

					   if (pFunName)
					   {
						   char *mpFunName=strstr(pFunName, "#");  //  stristr() �Դ�Сд������  ȡ���沿�֣��������ţ�
						   if (mpFunName)
						   { //sscanf(nItemID, "%s(%s", &nTextA, &nTextB); �ַ����ָ�
							   char nmpFunName[32]={0};
							   char nmCanshu[32]={0};
							   //nmpFunName=strtok(mpFunName,"-");  //�ַ����ָ�   ����
							   //nmCanshu=strtok(NULL,"-");        //����
							   ExtractChars(mpFunName,'(',')',nmpFunName,nmCanshu);
							   //					 //���Я���� �������� �� ����
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i-2],nmpFunName+1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i-2],nmCanshu, sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
							   //nmpFunName=strtok(pAnswer, "/"); //ȥ�� б��  //ȡǰ��Ĳ���
							   ExtractChars(pAnswer,'/',')',nmpFunName,nmCanshu,1);
							   sprintf(pContent, "%s|%s", pContent,nmpFunName);
							   ZeroMemory(nmpFunName,sizeof(nmpFunName));
							   ZeroMemory(nmCanshu,sizeof(nmCanshu));
						   }
						   else
						   {
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i-2],"", sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i-2], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
							   *pFunName = 0;   //�������
							   sprintf(pContent, "%s|%s", pContent, pAnswer);
						   }
					   }
					   else
					   {
						   strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i-2], "main");  //ִ��Main����
						   sprintf(pContent, "%s|%s", pContent, pAnswer);
					   }
				   }

			   }



			   if (nDataType == 0)//����Ϣ�����ͣ��ַ���(0)������(1)
				   UiInfo.m_nBufferLen  = strlen(pContent);
			   else
				   UiInfo.m_nBufferLen = strlen(pContent) + sizeof(int);

			   UiInfo.m_bParam2 = 0;  //�ͻ���

/*			   if (nOptionNum == 0)
			   {
				   Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
			   }
			   else
			   {
				   Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
			   } */

			   Player[nPlayerIndex].DoScriptAction(&UiInfo);//Ҫ����ʾĳ��UI����

	   }

  return 0;

}



//�¶Ի���
int LuaCreateTaskSay(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	int nIsVal=0;
	int nIsTimeClose=0;
	char * pContent = NULL;
	char * pOptionStr=NULL;

	int nPlayerIndex ;

       nPlayerIndex   = CLIENT_PLAYER_INDEX;

	   if (nPlayerIndex < 0) return 0;

//	   Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;

	   int nParamNum = Lua_GetTopIndex(L);

	   if (nParamNum < 1)
		   return 0;


	   PLAYER_SCRIPTACTION_SYNC UiInfo;
	   ZeroStruct(UiInfo);
	   UiInfo.m_bUIId        = UI_SELECTDIALOG;
	   UiInfo.m_bParam1      = nDataType ;//����Ϣ�����ͣ��ַ���(0)������(1)
	   // UiInfo.m_bOptionNum   = nOptionNum;
	   UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	   UiInfo.m_Select       = 1;


	   if (Lua_IsTable(L, 1))  //�������
	   {
		       nOptionNum = Lua_GetN(L,1);       //�����ж��ٸ���

	         if (nOptionNum > MAX_ANSWERNUM+1)
		         nOptionNum = MAX_ANSWERNUM+1;


			   Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionNum-1;  //ѡ����	 ��ȥ��һ�� ����˵��

			   UiInfo.m_bOptionNum   = nOptionNum-1;

			   for (int i=0;i<nOptionNum+1;i++)  //�����table
			   {
				   char pAnswer[64];
				   ZeroMemory(pAnswer,sizeof(pAnswer));

				   Lua_PushNumber(L,i + 1); //ѹ��һ��Key
				   //lua_settable(L, -3);   //����KEYֵ,Ȼ���ջ
				   Lua_RawGet(L, 1);        //ȡ�������ű��������ĵ�һ�������������ӦTABLE��KEYֵ Ȼ���ջ
				   char * pszString = (char *)Lua_ValueToString(L, Lua_GetTopIndex(L));
				   //nCurNum=4;
				   if (pszString)
				   {
					   if (i==0)
					   {//����
						   sprintf(UiInfo.m_pContent, "%s", pszString);
						   pContent = UiInfo.m_pContent;
						   continue;
					   }
					   else
					     g_StrCpyLen(pAnswer, pszString, 64);

					   //i=i-1; //�ָ����

					   char * pFunName = strstr(pAnswer, "/");  // �����״γ��� б�ܳ��ֵ�λ�� ����  �Դ�Сд����

					   if (pFunName)
					   {
						   char *mpFunName=strstr(pFunName, "#");  //  stristr() �Դ�Сд������  ȡ���沿�֣��������ţ�
						   if (mpFunName)
						   { //sscanf(nItemID, "%s(%s", &nTextA, &nTextB); �ַ����ָ�
							   char nmpFunName[32]={0};
							   char nmCanshu[32]={0};
							   //nmpFunName=strtok(mpFunName,"-");  //�ַ����ָ�   ����
							   //nmCanshu=strtok(NULL,"-");        //����
							   ExtractChars(mpFunName,'(',')',nmpFunName,nmCanshu);
							   //					 //���Я���� �������� �� ����
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i-1],nmpFunName+1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i-1],nmCanshu, sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
							   //nmpFunName=strtok(pAnswer, "/"); //ȥ�� б��  //ȡǰ��Ĳ���
							   ExtractChars(pAnswer,'/',')',nmpFunName,nmCanshu,1);
							   sprintf(pContent, "%s|%s", pContent,nmpFunName);
							   ZeroMemory(nmpFunName,sizeof(nmpFunName));
							   ZeroMemory(nmCanshu,sizeof(nmCanshu));
						   }
						   else
						   {
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i-1],"", sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i-1], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
							   *pFunName = 0;   //�������
							   sprintf(pContent, "%s|%s", pContent, pAnswer);
						   }
					   }
					   else
					   {
						   strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i-1], "main");  //ִ��Main����
						   sprintf(pContent, "%s|%s", pContent, pAnswer);
					   }
				   }

			   }



			   if (nDataType == 0)//����Ϣ�����ͣ��ַ���(0)������(1)
				   UiInfo.m_nBufferLen  = strlen(pContent);
			   else
				   UiInfo.m_nBufferLen = strlen(pContent) + sizeof(int);
			   UiInfo.m_bParam2 = 0;  //�ͻ���

/*			   if (nOptionNum == 0)
			   {
				   Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
			   }
			   else
			   {
				   Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
			   }*/

			   Player[nPlayerIndex].DoScriptAction(&UiInfo);//Ҫ����ʾĳ��UI����

	   }

  return 0;
}


//--------------------------
int LuaCreateNewSayEx(Lua_State * L)
{
   char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	int nIsVal=0;
	int nIsTimeClose=0;
	char * pContent = NULL;
	char * pOptionStr=NULL;

	int nPlayerIndex ;
       nPlayerIndex   = CLIENT_PLAYER_INDEX;
	   if (nPlayerIndex < 0) return 0;

//	   Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;

	   int nParamNum = Lua_GetTopIndex(L);

	   if (nParamNum < 2)
		   return 0;

	   if  (Lua_IsNumber(L,1))                       //����
	   {
		   nMainInfo = (int)Lua_ValueToNumber(L,1);  //��Ҫ��Ϣ
		   nDataType = 1 ;
	   }
	   else if (Lua_IsString(L, 1)) 	//�������Ϣ���ַ��������ַ�����ʶ��  �ַ���
	   {
		   strMain = (char *)Lua_ValueToString(L, 1);
		   nDataType = 0 ;
	   }
	   else
		   return 0;


	   PLAYER_SCRIPTACTION_SYNC UiInfo;
	   ZeroStruct(UiInfo);
	   UiInfo.m_bUIId        = UI_SELECTDIALOG;
	   UiInfo.m_bParam1      = nDataType ;//����Ϣ�����ͣ��ַ���(0)������(1)
	   // UiInfo.m_bOptionNum   = nOptionNum;
	   UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	   UiInfo.m_Select       = 1;

	  	//����ϢΪ�ַ���
	  if (nDataType == 0)
	  {
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		pContent = UiInfo.m_pContent;
	  }
	  else if (nDataType == 1) //����ϢΪ���ֱ�ʶ
	  {
		*(int *)UiInfo.m_pContent = nMainInfo;
		pContent = UiInfo.m_pContent + sizeof(int);
		*pContent = 0;
	  }


	   if (Lua_IsTable(L, 2))  //�������
	   {
		       nOptionNum = Lua_GetN(L,2);       //�ж��ٸ�ѡ��

	         if (nOptionNum > MAX_ANSWERNUM)
		         nOptionNum = MAX_ANSWERNUM;


			   Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionNum;  //ѡ����	 ��ȥ��һ�� ����˵��

			   UiInfo.m_bOptionNum   = nOptionNum;

			   for (int i=0;i<nOptionNum;i++)  //�����table
			   {
				   char pAnswer[64];
				   ZeroMemory(pAnswer,sizeof(pAnswer));

				   Lua_PushNumber(L,i + 1); //ѹ��һ��Key
				   //lua_settable(L, -3);   //����KEYֵ,Ȼ���ջ
				   Lua_RawGet(L, 2);        //ȡ�������ű��������ĵ�һ�������������ӦTABLE��KEYֵ Ȼ���ջ
				   char * pszString = (char *)Lua_ValueToString(L, Lua_GetTopIndex(L));
				   //nCurNum=4;
				   if (pszString)
				   {

					   g_StrCpyLen(pAnswer, pszString, 64);

					   //i=i-1; //�ָ����

					   char * pFunName = strstr(pAnswer, "/");  // �����״γ��� б�ܳ��ֵ�λ�� ����  �Դ�Сд����

					   if (pFunName)
					   {
						   char *mpFunName=strstr(pFunName, "#");  //  stristr() �Դ�Сд������  ȡ���沿�֣��������ţ�
						   if (mpFunName)
						   { //sscanf(nItemID, "%s(%s", &nTextA, &nTextB); �ַ����ָ�
							   char nmpFunName[32]={0};
							   char nmCanshu[32]={0};
							   //nmpFunName=strtok(mpFunName,"-");  //�ַ����ָ�   ����
							   //nmCanshu=strtok(NULL,"-");        //����
							   ExtractChars(mpFunName,'(',')',nmpFunName,nmCanshu);
							   //					 //���Я���� �������� �� ����
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i-1],nmpFunName+1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i-1],nmCanshu, sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
							   //nmpFunName=strtok(pAnswer, "/"); //ȥ�� б��  //ȡǰ��Ĳ���
							   ExtractChars(pAnswer,'/',')',nmpFunName,nmCanshu,1);
							   sprintf(pContent, "%s|%s", pContent,nmpFunName);
							   ZeroMemory(nmpFunName,sizeof(nmpFunName));
							   ZeroMemory(nmCanshu,sizeof(nmCanshu));
						   }
						   else
						   {
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i-1],"", sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
							   g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i-1], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
							   *pFunName = 0;   //�������
							   sprintf(pContent, "%s|%s", pContent, pAnswer);
						   }
					   }
					   else
					   {
						   strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i-1], "main");  //ִ��Main����
						   sprintf(pContent, "%s|%s", pContent, pAnswer);
					   }
				   }

			   }

			   if (nDataType == 0)//����Ϣ�����ͣ��ַ���(0)������(1)
				   UiInfo.m_nBufferLen  = strlen(pContent);
			   else
				   UiInfo.m_nBufferLen = strlen(pContent) + sizeof(int);
			   UiInfo.m_bParam2 = 0;  //�ͻ���

/*			   if (nOptionNum == 0)
			   {
				   Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
			   }
			   else
			   {
				   Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
			   }*/

			   Player[nPlayerIndex].DoScriptAction(&UiInfo);//Ҫ����ʾĳ��UI����

	   }

  return 0;

}

//--------------------------
int LuaSayNew(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	int nIsVal=0;
	int nIsTimeClose=0;
	char * pContent = NULL;
	char * pOptionStr=NULL;

	int nPlayerIndex ;
       nPlayerIndex   = CLIENT_PLAYER_INDEX;
	if (nPlayerIndex < 0) return 0;
//	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;

	int nParamNum = Lua_GetTopIndex(L);

	if (nParamNum < 2)
		return 0;

	if (Lua_IsNumber(L,2))  //�ڶ�������Ϊ ����
	{
		nOptionNum = (int)Lua_ValueToNumber(L,2);  //�м���������Ϣ
//		Player[nPlayerIndex].m_nCheckWaiGua=0;
	}
	else
	{//����Ϊ�ַ���
	//
		 pOptionStr=(char *)Lua_ValueToString(L,2);
		 if (strstr(pOptionStr,"|"))
		 {
		    sscanf(pOptionStr,"%d|%d|%d",&nOptionNum,&nIsVal,&nIsTimeClose);
//		    Player[nPlayerIndex].m_nCheckWaiGua=nIsVal;//(BYTE)Lua_ValueToNumber(L,nCurNum);
		 }
		  else
		  {
			  //_ASSERT(0);
//			  Player[nPlayerIndex].m_nCheckWaiGua=0;
			  return 0;
		  }

	}

	if  (Lua_IsNumber(L,1))  //����
	{
		nMainInfo = (int)Lua_ValueToNumber(L,1);  //��Ҫ��Ϣ
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 1)) 	//�������Ϣ���ַ��������ַ�����ʶ��  �ַ���
	{
		strMain = (char *)Lua_ValueToString(L, 1);
		nDataType = 0 ;
	}
	else
		return 0;

	BOOL bStringTab = FALSE;//��ʶ��������ѡ�����ݴ����һ�������У���������ַ�����

	if (Lua_IsString(L,3)) //�����������ַ��� ���ؼ�
		bStringTab = FALSE;
	else if (Lua_IsTable(L, 3))  //�������
	{
		bStringTab = TRUE;
	}
	else
	{
		if (nOptionNum > 0)  return 0;
	}

	if (bStringTab == FALSE) //��table
	{
		//���ʵ�ʴ����ѡ��ĸ���
		if (nOptionNum > nParamNum - 2)   //��ȥǰ����������
			nOptionNum = nParamNum - 2;
	}

	if (nOptionNum > MAX_ANSWERNUM)
		nOptionNum = MAX_ANSWERNUM; //ѡ����ʾ����

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId        = UI_SELECTDIALOG;
	UiInfo.m_bParam1      = nDataType;//����Ϣ�����ͣ��ַ���(0)������(1)
	UiInfo.m_bOptionNum   = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	UiInfo.m_Select       = 1;

	//����ϢΪ�ַ���
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		pContent = UiInfo.m_pContent;

		//TEncodeText(UiInfo.m_pContent, strlen(UiInfo.m_pContent));

	}
	else if (nDataType == 1) //����ϢΪ���ֱ�ʶ
	{
		*(int *)UiInfo.m_pContent = nMainInfo;
		pContent = UiInfo.m_pContent + sizeof(int);
		*pContent = 0;
	}

	if (nOptionNum > MAX_ANSWERNUM)
		nOptionNum = MAX_ANSWERNUM;

	Player[nPlayerIndex].m_nAvailableAnswerNum = nOptionNum;  //ѡ����

	//int nCurNum=4;

	for (int i  = 0; i < nOptionNum; i ++)
	{
		char  pAnswer[64];
		ZeroMemory(pAnswer,sizeof(pAnswer));
		//pAnswer[0]=0;

		if (bStringTab)  //�����table
		{
			Lua_PushNumber(L,i + 1); //ѹ��һ��Key
			Lua_RawGet(L, 3);        //ȡ�������ű��������ĵ����������������ӦTABLE��KEYֵ Ȼ���ջ
			char * pszString = (char *)Lua_ValueToString(L, Lua_GetTopIndex(L));
			    // nCurNum=4;
			if (pszString)
			{
				g_StrCpyLen(pAnswer, pszString, 64);
			}
		}
		else  //������ǵ������ַ���
		{
			char * pszString = (char *)Lua_ValueToString(L, i + 3);	//�ӵ�����������ʼ
			       //nCurNum=i + 4;
			if (pszString)
				g_StrCpyLen(pAnswer, pszString, 64);
		}

		char * pFunName = strstr(pAnswer, "/");  // �����״γ��� б�ܳ��ֵ�λ�� ����  �Դ�Сд����

		if (pFunName)
		{
			char *mpFunName=strstr(pFunName, "#");  //  stristr() �Դ�Сд������  ȡ���沿�֣��������ţ�
			if (mpFunName)
			{ //sscanf(nItemID, "%s(%s", &nTextA, &nTextB); �ַ����ָ�
				char nmpFunName[32]={0};
				char nmCanshu[32]={0};
                     //nmpFunName=strtok(mpFunName,"-");  //�ַ����ָ�   ����
                     //nmCanshu=strtok(NULL,"-");        //����
				     ExtractChars(mpFunName,'(',')',nmpFunName,nmCanshu);
//					 //���Я���� �������� �� ����
					 g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i],nmpFunName+1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
					 g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i],nmCanshu, sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
                     //nmpFunName=strtok(pAnswer, "/"); //ȥ�� б��  //ȡǰ��Ĳ���
					 ExtractChars(pAnswer,'/',')',nmpFunName,nmCanshu,1);
			         sprintf(pContent, "%s|%s", pContent,nmpFunName);
					 ZeroMemory(nmpFunName,sizeof(nmpFunName));
					 ZeroMemory(nmCanshu,sizeof(nmCanshu));
			}
			else
			{
		    	g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerCanshu[i],"", sizeof(Player[nPlayerIndex].m_szTaskAnswerCanshu[0]));
			    g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[i], pFunName + 1, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
			    *pFunName = 0;   //�������
			    sprintf(pContent, "%s|%s", pContent, pAnswer);
				//printf("--%s--\n",pContent);
			}
		}
		else
		{
			strcpy(Player[nPlayerIndex].m_szTaskAnswerFun[i], "main");  //ִ��Main����
			sprintf(pContent, "%s|%s", pContent, pAnswer);
		}
	}

	   if (nDataType == 0)//����Ϣ�����ͣ��ַ���(0)������(1)
		UiInfo.m_nBufferLen  = strlen(pContent);
	   else
		UiInfo.m_nBufferLen = strlen(pContent) + sizeof(int);

	UiInfo.m_bParam2 = 0;  //�ͻ���

/*	if (nOptionNum == 0)
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else
	{
		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}*/

	Player[nPlayerIndex].DoScriptAction(&UiInfo);//Ҫ����ʾĳ��UI����

	return 0;
}

//PutMsg(szMsg/MsgId)

int LuaSendMessageInfo(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	int nPlayerIndex ;
       nPlayerIndex   = CLIENT_PLAYER_INDEX;
	if (nPlayerIndex < 0) return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId = UI_MSGINFO;
	UiInfo.m_bOptionNum = 1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
//	Player[nPlayerIndex].m_nCheckWaiGua =0;

	int nMsgId = 0;

	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else
	{

		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

//AddGlobalNews(Newsstr)
int LuaAddGlobalNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	int nPlayerIndex;
       nPlayerIndex   = CLIENT_PLAYER_INDEX;
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
//	Player[nPlayerIndex].m_nCheckWaiGua =0;

	int nMsgId = 0;

	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 128);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}

	//int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

//AddLocalNews(Newsstr)
int LuaAddLocalNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	int nPlayerIndex=0;
		nPlayerIndex = CLIENT_PLAYER_INDEX;
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_NORMAL;  //��ͨ��Ϣ ��ʾ������
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;  //��ʾUI����
//	Player[nPlayerIndex].m_nCheckWaiGua =0;
	int nMsgId = 0;

	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 128);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}

	//int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}
//AddLocalNews(Newsstr)  ,��ʾһ�ξ�����
int LuaAddSysNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	int nPlayerIndex=0;

	nPlayerIndex = CLIENT_PLAYER_INDEX;
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_SHUIJI;  //��ͨ��Ϣ ��ʾ������
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;  //��ʾUI����
//	Player[nPlayerIndex].m_nCheckWaiGua =0;

	int nMsgId = 0;

	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int);
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), MAX_SCIRPTACTION_BUFFERNUM);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		UiInfo.m_bParam1 = 0;
	}

	//int nPlayerIndex = CLIENT_PLAYER_INDEX;//GetPlayerIndex(L);
	if (nPlayerIndex < 0)
		return 0;
	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo); //�ͻ���ֱ����ʾ
	return 0;
}


//AddGlobalCountNews(strNew/newid, time)
int LuaAddGlobalCountNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;
	int nPlayerIndex=0;

	nPlayerIndex = CLIENT_PLAYER_INDEX;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING;  //����ʱ��Ϣ
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
//	Player[nPlayerIndex].m_nCheckWaiGua =0;

	int nMsgId = 0;

	int nTime = (int)Lua_ValueToNumber(L,2);

	if (nTime <= 0)
		nTime = 1;

	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(int *)((char *)UiInfo.m_pContent + sizeof(int)) = nTime;
		UiInfo.m_nBufferLen = sizeof(int) * 2;
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		*(int *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(int);
		UiInfo.m_bParam1 = 0;
	}
//�ͻ���
	//int nPlayerIndex = CLIENT_PLAYER_INDEX;//GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

//AddLocalCountNews(strNew/newid, time)
int LuaAddLocalCountNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 2)
		return 0;


	int nPlayerIndex=0;

	nPlayerIndex = CLIENT_PLAYER_INDEX;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_COUNTING;  //����ʱ��Ϣ
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
//	Player[nPlayerIndex].m_nCheckWaiGua =0;
	int nMsgId = 0;

	int nTime = (int)Lua_ValueToNumber(L,2);

	if (nTime <= 0)
		nTime = 1;

	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		*(int *)((char *)UiInfo.m_pContent + sizeof(int)) = nTime;
		UiInfo.m_nBufferLen = sizeof(int) * 2;
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 256);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
		*(int *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen) = nTime;
		UiInfo.m_nBufferLen += sizeof(int);
		UiInfo.m_bParam1 = 0;
	}
//�ͻ���
	//int nPlayerIndex = CLIENT_PLAYER_INDEX;//GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);

	return 0;
}

//AddGlobalTimeNews(strNew/newid, year,month,day,hour,mins)
int LuaAddGlobalTimeNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 6)
		return 0;

	int nPlayerIndex=0;

	nPlayerIndex = CLIENT_PLAYER_INDEX;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND;  //��ʱ��Ϣ
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
//	Player[nPlayerIndex].m_nCheckWaiGua =0;
	int nMsgId = 0;

	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(SYSTEMTIME);
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 64);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}

	SYSTEMTIME *pSystemTime = (SYSTEMTIME *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));

	SYSTEMTIME LocalTime ;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));

	LocalTime.wYear = (WORD)Lua_ValueToNumber(L,2);
	LocalTime.wMonth =(WORD)Lua_ValueToNumber(L,3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L,5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L,6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif

	//int nPlayerIndex = CLIENT_PLAYER_INDEX;//GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

//AddLocalTimeNews(strNew/newid, year,month,day,hour,mins)
int LuaAddLocalTimeNews(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 6)
		return 0;

	int nPlayerIndex=0;

	nPlayerIndex = CLIENT_PLAYER_INDEX;
	PLAYER_SCRIPTACTION_SYNC UiInfo;
	UiInfo.m_bUIId = UI_NEWSINFO;
	UiInfo.m_bOptionNum = NEWSMESSAGE_TIMEEND;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
//	Player[nPlayerIndex].m_nCheckWaiGua =0;
	int nMsgId = 0;

	if (Lua_IsNumber(L,1))
	{
		nMsgId = (int)Lua_ValueToNumber(L,1);
		*((int *)(UiInfo.m_pContent)) = nMsgId;
		UiInfo.m_bParam1 = 1;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(SYSTEMTIME);
	}
	else
	{
		g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), 64);
		UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent)) + sizeof(SYSTEMTIME);
		UiInfo.m_bParam1 = 0;
	}

	SYSTEMTIME *pSystemTime = 	(SYSTEMTIME *)((char *)UiInfo.m_pContent + UiInfo.m_nBufferLen - sizeof(SYSTEMTIME));
	memset(pSystemTime, 0, sizeof(SYSTEMTIME));

	SYSTEMTIME LocalTime ;
	memset(&LocalTime, 0, sizeof(SYSTEMTIME));

	LocalTime.wYear = (WORD)Lua_ValueToNumber(L,2);
	LocalTime.wMonth =(WORD)Lua_ValueToNumber(L,3);
	LocalTime.wDay = (WORD)Lua_ValueToNumber(L, 4);
	LocalTime.wHour = (WORD)Lua_ValueToNumber(L,5);
	LocalTime.wMinute = (WORD)Lua_ValueToNumber(L,6);
	FILETIME ft;
	FILETIME sysft;
#ifdef WIN32
	SystemTimeToFileTime(&LocalTime, &ft);
	LocalFileTimeToFileTime(&ft, &sysft);
	FileTimeToSystemTime(&sysft, pSystemTime);
#else
	memcpy(pSystemTime, &LocalTime, sizeof(LocalTime));
#endif

	//int nPlayerIndex = CLIENT_PLAYER_INDEX;
	if (nPlayerIndex < 0) return 0;

	UiInfo.m_bParam2 = 0;
	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

//AddNote(str/strid)
int LuaAddNote(Lua_State * L)
{
	char * strMain  = NULL;
	int nMainInfo = 0;
	int nDataType = 0;

	int nPlayerIndex = GetPlayerIndex(L);
	nPlayerIndex   = CLIENT_PLAYER_INDEX;
	if (nPlayerIndex < 0)
		return 0;

	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 1)
		return 0;

	int nParam2 = 0;


	if  (Lua_IsNumber(L,1))
	{
		nMainInfo = (int)Lua_ValueToNumber(L,1);
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 1)) 	//�������Ϣ���ַ��������ַ�����ʶ��
	{
		strMain = (char *)Lua_ValueToString(L, 1);
		nDataType = 0 ;
	}
	else
		return 0;

	if (nParamNum > 1)
	{
		nParam2 = (int)Lua_ValueToNumber(L, 2);
	}


	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId = UI_NOTEINFO;
	UiInfo.m_bParam1 = nDataType;//����Ϣ�����ͣ��ַ���(0)������(1)
//	Player[nPlayerIndex].m_nCheckWaiGua =0;
	UiInfo.m_bParam2 = 0;

	UiInfo.m_bOptionNum = 0;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;

	//����ϢΪ�ַ���
	if (nDataType == 0)
	{
		if (strMain)
			sprintf(UiInfo.m_pContent, "%s", strMain);
		int nLen = strlen(strMain);
		*(int*)(UiInfo.m_pContent + nLen) = nParam2;
		UiInfo.m_nBufferLen = nLen + sizeof(int);
	}
	else if (nDataType == 1) //����ϢΪ���ֱ�ʶ
	{
		*(int *)UiInfo.m_pContent = nMainInfo;
		*(int *)(UiInfo.m_pContent + sizeof(int)) = nParam2;
		UiInfo.m_nBufferLen = sizeof(int) + sizeof(int);
	}

	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

/*
**
**��ʽ1:Talk(SentenceNum, CallBack-Fun(������Ļص�����), sTalk1, sTalk2, sTalk3, sTalk4,...sTalkN);
Talk(SentenceNum, CallBack-Fun(������Ļص�����), nTalk1, nTalk2,nTalk3,nTalk4,...nTalkN);
**��ʽ2:Talk(SentenceNum, CallBack-Fun, SentenceTab);
**����:Talk(3,"EndTalk", "��ң��������ڼ������ˣ�", "��Ա������5������","̫лл���ˣ�");
**
*/

int LuaTalkUI(Lua_State * L)
{
	int nPlayerIndex ;
       nPlayerIndex   = CLIENT_PLAYER_INDEX;
	if (nPlayerIndex <= 0)
		return 0;
//	Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	int nMainInfo = 0;
	int nDataType = 0;
	int nOptionNum = 0;
	char * pContent = NULL;

	int nParamNum = Lua_GetTopIndex(L);
	if (nParamNum < 3)
		return 0;

	if (Lua_IsNumber(L,1))
	{
		nOptionNum = (int)Lua_ValueToNumber(L,1);
	}
	else
	{

		return 0;
	}

	const char * pCallBackFun = Lua_ValueToString(L,2);

	//�������Ϣ���ַ��������ַ�����ʶ��

	if  (Lua_IsNumber(L,3))
	{
		nDataType = 1 ;
	}
	else if (Lua_IsString(L, 3))
	{
		nDataType = 0 ;
	}
	else
		return 0;


	//���ʵ�ʴ����ѡ��ĸ���
	if (nOptionNum > nParamNum - 2)
		nOptionNum = nParamNum - 2;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId = UI_TALKDIALOG;
	UiInfo.m_bParam1 = nDataType;//����Ϣ�����ͣ��ַ���(0)������(1)
	UiInfo.m_bOptionNum = nOptionNum;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
	pContent = UiInfo.m_pContent;
//	Player[nPlayerIndex].m_nCheckWaiGua =0;
	pContent[0] = 0;
	size_t nContentLen = 0;
	for (int i  = 0; i < nOptionNum; i ++)
	{
		const char * pString = NULL;
		if (!nDataType)//StringInfo
		{
			pString = Lua_ValueToString(L, i + 3); //��������Ϊ�ַ�����
			if (nContentLen  + strlen(pString) >= MAX_SCIRPTACTION_BUFFERNUM)
			{
				nOptionNum = i;
				UiInfo.m_bOptionNum = nOptionNum;
				break;
			}
			nContentLen += strlen(pString);
			sprintf(pContent, "%s%s|", pContent, pString);
		}
		else
		{
			int j = (int)Lua_ValueToNumber(L, i + 3);
			sprintf(pContent, "%s%d|", pContent, j);
		}
	}
	UiInfo.m_nBufferLen  = strlen(pContent);

	if (!pCallBackFun || strlen(pCallBackFun) <= 0)
	{
		UiInfo.m_nParam = 0;
		Player[nPlayerIndex].m_nAvailableAnswerNum = 0;
//		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = false;
	}
	else   //�лص�����
	{
		UiInfo.m_nParam = 1;
		Player[nPlayerIndex].m_nAvailableAnswerNum = 1;
		g_StrCpyLen(Player[nPlayerIndex].m_szTaskAnswerFun[0], pCallBackFun, sizeof(Player[nPlayerIndex].m_szTaskAnswerFun[0]));
//		Player[nPlayerIndex].m_bWaitingPlayerFeedBack = true;
	}

	UiInfo.m_bParam2 = 0;

	Player[nPlayerIndex].DoScriptAction(&UiInfo);

	return 0;

}

int LuaIncludeLib(Lua_State * L)
{

	if (Lua_GetTopIndex(L) <= 0)
		return 0;

	if (Lua_IsString(L,1))
	{
       	const char * pFileDir = lua_tostring(L,1);
		//char lszCurrentDirectory[MAX_PATH];
	}
	return 0;
}


int LuaIncludeFile(Lua_State * L)
{
	if (Lua_GetTopIndex(L) <= 0)
		return 0;

	if (Lua_IsString(L,1))
	{
		//char * pFileName = lua_tostring(L,1);
		char pFileName[256];
		ZeroMemory(pFileName,sizeof(pFileName));
		sprintf(pFileName, "%s", lua_tostring(L,1));
		char lszCurrentDirectory[MAX_PATH];
		int nLen = 0;
		if (pFileName[0] != '\\' && pFileName[0] != '/')
		{
			getcwd(lszCurrentDirectory, MAX_PATH);
			nLen = strlen(lszCurrentDirectory);
			if (lszCurrentDirectory[nLen - 1] == '\\' || lszCurrentDirectory[nLen - 1] == '/')
				lszCurrentDirectory[nLen - 1] = 0;
#ifdef WIN32  //32λ��  "\\"
			g_StrCat(lszCurrentDirectory, "\\");
			g_StrCat(lszCurrentDirectory, (char*)pFileName);
#else
			g_StrCat(lszCurrentDirectory, "/");
			g_StrCat(lszCurrentDirectory, (char*)pFileName);
			for (int i = 0; lszCurrentDirectory[i]; ++i)
			{
				if (lszCurrentDirectory[i] == '\\')
					lszCurrentDirectory[i] = '/';
			}
#endif
		}
		else
		{
			g_GetRootPath(lszCurrentDirectory); //��ȡ������е� ����·��

			nLen = strlen(lszCurrentDirectory);
			if (lszCurrentDirectory[nLen - 1] == '\\' || lszCurrentDirectory[nLen - 1] == '/')
				lszCurrentDirectory[nLen - 1] = 0;
#ifdef WIN32
			g_StrCat(lszCurrentDirectory, "\\");
			g_StrCat(lszCurrentDirectory, (char*)pFileName + 1);
#else
			g_StrCat(lszCurrentDirectory, "/");
			g_StrCat(lszCurrentDirectory, (char*)pFileName + 1);
			for (int i = 0; lszCurrentDirectory[i]; ++i)
			{
				if (lszCurrentDirectory[i] == '\\')
					lszCurrentDirectory[i] = '/';
			}
#endif
		}

	    //g_StrLower(lszCurrentDirectory + nLen);  //��дתСд	 strlwr
		g_StrLower(lszCurrentDirectory + nLen);

        //�ͻ���ֻ�ܰ��� ���ļ�
		if (!pFileName)
			return 0;
		/*g_StrLower(pFileName);   //��дתСд  g_StrLower
		DWORD dwScriptId = g_FileName2Id(pFileName);
			 pScript = (KLuaScript* )g_GetScript(dwScriptId);
		*/
		//Lua_CompileFile();       //���ذ����ļ� �ͻ���
		 // char nMsg[128]={0};
          KPakFile	File;
          DWORD		Size;
	      if (!File.Open(pFileName))
		  {
		    // sprintf(nMsg,"��%sʧ��",pFileName);
		     //KSceGetFliePathc(nMsg,"--",100);
		     return 0;
		  }
		  Size = File.Size();
		  KMemClass Memory;
		  if (!Memory.Alloc(Size + 4))
		  {
		     File.Close();
			 //sprintf(nMsg,"�����ڴ�(%s):%u ����!!",pFileName,Size);
			 //KSceGetFliePathc(nMsg,"��",100);
		     return 0;
		  }
		  if (File.Read(Memory.GetMemPtr(),Size) != Size)
		  {
		     File.Close();
			// sprintf(nMsg,"��ȡ(%s):%u ����!!",pFileName,Size);
			 //KSceGetFliePathc(nMsg,"��",100);
			 Memory.Free();
		     return 0;
		  }

		  char * pszMem = (char *)Memory.GetMemPtr();
		  pszMem[Size + 1] = 0;

		  File.Close();

		  try
		  {
			  if (pszMem)
			  {
				  int state;
				  if (state = Lua_ExecuteBuffer(L,pszMem,Size,NULL)!=0)
				  {
					  //sprintf(nMsg,"----ִ�а����ű�(%s)����(%u)----",pFileName,state);
					  //KSceGetFliePathc(nMsg,"��",100);
					  Memory.Free();
					  return 0;
				  }
			  }
		  }
		  catch(...)
		  {
			  //sprintf(nMsg,"���� %s �����쳣,����!!",pFileName);
			  //KSceGetFliePathc(nMsg,"��",100);
		  }

		  //if (pszMem)
		  Memory.Free();

		return 0;
	}
	else
		return 0;
}

//**************************************************************************************************************************************************************
//												����ű�
//**************************************************************************************************************************************************************


//**************************************************************************************************************************************************************
#define MAX_TEMPVALUENUM_INCLIENT 4000
int g_TempValue[MAX_TEMPVALUENUM_INCLIENT];

int LuaGetTaskValue(Lua_State * L)
{
	int nPlayerIndex=0;

	int nTaskId= (int)Lua_ValueToNumber(L,1);
       nPlayerIndex   = CLIENT_PLAYER_INDEX;

       PLAYER_GET_TASKVAL_COMMAND GetTaskval;
	       GetTaskval.ProtocolType = c2s_gettaskval;
           GetTaskval.m_TaskId=nTaskId;
           GetTaskval.m_pDwid=Npc[Player[nPlayerIndex].m_nIndex].m_dwID;
	   if (g_pClient)
		   g_pClient->SendPackToServer((BYTE*)&GetTaskval, sizeof(PLAYER_GET_TASKVAL_COMMAND));
	      //Player[nPlayerIndex].m_ItemList.LockOperation();

    int nValue=0;

	if (nPlayerIndex > 0)
	{
		nValue = Player[nPlayerIndex].m_cTask.GetSaveVal(nTaskId);
		//Lua_PushNumber(L, nValue);
	}
	//else
	Lua_PushNumber(L, nValue);
	return 1;
}

int LuaSetTaskValue(Lua_State * L)
{
	int nPlayerIndex;
       nPlayerIndex   = CLIENT_PLAYER_INDEX;
	int nValueIndex = (int)Lua_ValueToNumber(L, 1);
	int nValue = (int)Lua_ValueToNumber(L, 2);
	if (nPlayerIndex <= 0) return 0;

	Player[nPlayerIndex].m_cTask.SetSaveVal(nValueIndex, nValue);  //��� �� ֵ

	 PLAYER_SET_TASKVAL_COMMAND SetTaskval;
	 SetTaskval.ProtocolType = c2s_settaskval;
	 SetTaskval.m_TaskId  = nValueIndex;
	 SetTaskval.m_TaskVal = nValue;
	 SetTaskval.m_pDwid=Npc[Player[nPlayerIndex].m_nIndex].m_dwID;
	 if (g_pClient)
	   g_pClient->SendPackToServer((BYTE*)&SetTaskval, sizeof(PLAYER_SET_TASKVAL_COMMAND));
	//	int nReg=Player[nPlayerIndex].Save();  //
	return 0;
}

int LuaSetDeathSkill(Lua_State * L)
{
	int nPlayerIndex;
       nPlayerIndex   = CLIENT_PLAYER_INDEX;
	   int nValueIndex = (int)Lua_ValueToNumber(L, 1);
	   int nValue = (int)Lua_ValueToNumber(L, 2);
	   if (nPlayerIndex <= 0) return 0;

	   Player[nPlayerIndex].m_cTask.SetSaveVal(nValueIndex, nValue);  //��� �� ֵ
	   //	int nReg=Player[nPlayerIndex].Save();  //
	   return 0;
}

int LuaGetDeathSkill(Lua_State * L)
{
	int nPlayerIndex=0;

	int nTaskId= (int)Lua_ValueToNumber(L,1);

       nPlayerIndex   = CLIENT_PLAYER_INDEX;

       PLAYER_GET_TASKVAL_COMMAND GetTaskval;
	   GetTaskval.ProtocolType = c2s_gettaskval;
	   GetTaskval.m_TaskId=nTaskId;
	   GetTaskval.m_pDwid=Npc[Player[nPlayerIndex].m_nIndex].m_dwID;
	   if (g_pClient)
		   g_pClient->SendPackToServer((BYTE*)&GetTaskval, sizeof(PLAYER_GET_TASKVAL_COMMAND));

	   int nValue=0;

	   if (nPlayerIndex > 0)
	   {
		   nValue = Player[nPlayerIndex].m_cTask.GetSaveVal(nTaskId);
		   //Lua_PushNumber(L, nValue);
	   }
	   //else
	   Lua_PushNumber(L, nValue);
	   return 1;
}

int LuaAddTaskValue(Lua_State * L)
{
	int nPlayerIndex;
       nPlayerIndex   = CLIENT_PLAYER_INDEX;
	int nValueIndex = (int)Lua_ValueToNumber(L,1);
	int nValue = (int)Lua_ValueToNumber(L, 2);

	int nValueOld = (int)Player[nPlayerIndex].m_cTask.GetSaveVal(nValueIndex);

	if (nPlayerIndex <= 0)
	    return 0;

	    Player[nPlayerIndex].m_cTask.SetSaveVal(nValueIndex,nValueOld + nValue);
	return 0;
}

int LuaMathematical(Lua_State * L)
{
	return 0;
}

int  LuaMessage(Lua_State * L)
{
	const char * szString;
	szString  = lua_tostring (L,1);
	printf("--%s-- \n",(char *)szString);
	return 0;
}
//***************************************************************************8***
int LuaGetTempTaskValue(Lua_State * L)
{
	int nTempIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	int nPlayerIndex;
	nPlayerIndex=CLIENT_PLAYER_INDEX;
	if (nTempIndex >= 0 && nTempIndex < MAX_TEMPVALUENUM_INCLIENT)
		Lua_PushNumber(L, g_TempValue[nTempIndex]);
	else
		Lua_PushNil(L);
	return 1;
}

int LuaSetTempTaskValue(Lua_State * L)
{
	int nTempIndex = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L) - 1);
	int nValue = (int)Lua_ValueToNumber(L, Lua_GetTopIndex(L));
	int nPlayerIndex;
	nPlayerIndex=CLIENT_PLAYER_INDEX;
	g_TempValue[nTempIndex] = nValue;
	return 0;
}

//��ȡ�� ��������
int LuaGetTabRowAndCol(Lua_State * L)
{
	KTabFile  TableFile;
	char *FiledName=NULL,*LieName=NULL;
	int nReg=0,mLie=0,mRow=0;
	FiledName = (char *)Lua_ValueToString(L, 1);
	nReg=TableFile.Load(FiledName);
    //TableFile.TabFileCtrl->WriteString(1,1,"����д��");
	if (nReg)
	{
		mLie= TableFile.GetWidth();        //������
		mRow= TableFile.GetHeight()-1;     //������
	}
	TableFile.Clear();
	Lua_PushNumber(L,mRow);
    Lua_PushNumber(L,mLie);
	return 2;
}


//��ȡINIĳ�� ĳ����ֵ
int LuaGetIniFileInfo(Lua_State * L)
{
	KIniFile  IniFile;
	char *FiledName=NULL,*JieName=NULL,*KeyName=NULL;
	    FiledName =(char *)Lua_ValueToString(L,1);
		JieName =(char *)Lua_ValueToString(L, 2);
		KeyName =(char *)Lua_ValueToString(L, 3);
     int nIsType=0;
		 nIsType=(int)Lua_ValueToNumber(L, 4);
	 int nInVal=0;
	 char nstrVal[256]={0};
	 if (!IniFile.Load(FiledName))
		 return 0;

	 if  (nIsType==0)
	 {
	      IniFile.GetInteger(JieName,KeyName,0,&nInVal);
		  IniFile.Clear();
		  Lua_PushNumber(L,nInVal);
          return 1;
	 }
	 else
	 {
		 IniFile.GetString(JieName,KeyName,"",nstrVal,sizeof(nstrVal));
		 IniFile.Clear();
		 Lua_PushString(L,nstrVal);
         return 1;
	 }

}

//��ȡĳ�� ĳ�е�ֵ
int LuaGetTabFileInfo(Lua_State * L)
{
	KTabFile  TableFile;
	char *FiledName=NULL,*LieName=NULL;
	char nVal[512]={0};
	ZeroMemory(nVal,sizeof(nVal));
	int mLiem;
	FiledName =(char *)Lua_ValueToString(L, 1);
	//int mRowm=(int)Lua_ValueToNumber(L, 2);
	if(!TableFile.Load(FiledName))
		return 0;

    if (Lua_IsNumber(L,2))
	{
         int mRowm=(int)Lua_ValueToNumber(L, 2);

		 if (Lua_IsNumber(L,3))
		 {
			 mLiem=(int)Lua_ValueToNumber(L, 3);
			 TableFile.GetString(mRowm,mLiem,"",nVal,sizeof(nVal));
		 }
		 else
		 {
			 LieName = (char *)Lua_ValueToString(L, 3);
			 TableFile.GetString(mRowm,LieName,"",nVal,sizeof(nVal));
		 }

	}
	else
	{
        char *mRowm=(char *)Lua_ValueToString(L, 2);

		if (Lua_IsNumber(L,3))
		{
			mLiem=(int)Lua_ValueToNumber(L, 3);
			TableFile.GetString(mRowm,mLiem,"",nVal,sizeof(nVal));
		}
		else
		{
			LieName = (char *)Lua_ValueToString(L, 3);
			TableFile.GetString(mRowm,LieName,"",nVal,sizeof(nVal));
		}
	}

	TableFile.Clear();
	Lua_PushString(L,nVal);

	return 1;
}

int LuaGetTaskAwardMatrix(Lua_State * L)
{
	KTabFile TableFile;
	//  KTabFile    TableFile;     // ��ȡTXT�ļ� ���ر��
	char *FiledName=NULL,*nKeyName=NULL,*nTxtName=NULL;
	int nRows=0,nReg=0,mLie=0,mRow=0;

	FiledName = (char *)Lua_ValueToString(L, 1);
    nKeyName  = (char *)Lua_ValueToString(L, 2);  //�е��ֶ�
	nTxtName  = (char *)Lua_ValueToString(L, 3);  //��ҪѰ�ҵ�����

    if (!TableFile.Load(FiledName))
		return 0;
    int i ,nStartPos=0,nEndPos=0;
    nRows = TableFile.GetHeight();
    mLie  = TableFile.GetWidth();
	char nTitle[64]={0};

	for (i=2;i<nRows+1;++i)
	{

		TableFile.GetString(i,nKeyName,"",nTitle,sizeof(nTitle));
		if (strstr(nTitle,nTxtName))
		{
			nStartPos = i;
			break;
		}
	}

	for (i=nStartPos;i<nRows+1;++i)
	{
		TableFile.GetString(i,nKeyName,"",nTitle,sizeof(nTitle));
		if (strstr(nTitle,nTxtName))
		{
			nEndPos = i;
			nReg++;	   //�ж�����
		}
		else
			break;     //��������  ֻ���� �������к�
	}

	TableFile.Clear();
	Lua_PushNumber(L,nReg);
	Lua_PushNumber(L,mLie);
	Lua_PushNumber(L,nStartPos);
	Lua_PushNumber(L,nEndPos);
	return 4;
}


int LuaCalcFreeItemCellCount(Lua_State * L)
{
int nPlayerIndex;
    nPlayerIndex   = CLIENT_PLAYER_INDEX;
	   int nCount = 0;
	   if (nPlayerIndex > 0)
	   {
		   int x,y;
		   for (x=0;x<6;x++)
			   for (y=0;y<10;y++)
			   {
		           if (!Player[nPlayerIndex].m_ItemList.m_Room[room_equipment].FindItem(x, y))
				   {//û�ж��� ������ �������Ŀռ�
					   nCount ++;
				   }
			   }
	   }
	   Lua_PushNumber(L, nCount);
 return 1;
}

int LuaGetPositionCount(Lua_State * L)
{
	int nPlayerIndex;

       nPlayerIndex   = CLIENT_PLAYER_INDEX;

	int nWpos  = (int)Lua_ValueToNumber(L,1);
	int nHpos  = (int)Lua_ValueToNumber(L,2);
    int nCount = 0;
    if (nPlayerIndex > 0)
    {
		ItemPos Pos;
		ZeroStruct(Pos);
		if (Player[nPlayerIndex].m_ItemList.SearchPosition(nWpos,nHpos,&Pos))
		{//����λ��	�����ռ�
			nCount=nWpos*nHpos;
		}
		ZeroStruct(Pos);
	}
	Lua_PushNumber(L, nCount);
    return 1;
}

//��ȡ��ǰʱ�䣨����ģʽ ��ʽ����
int LuaGetLocalDate(Lua_State * L)
{
    char * nTimeStr = (char *)Lua_ValueToString(L,1);

	time_t rawtime;                  //����һ��long �ʹ������
	struct tm * timeinfo;
	time (&rawtime);                 //�������ڵ�ʱ�䣨�룩
	timeinfo = localtime(&rawtime);  //�Ѵ�1970-1-1�����ֵ���ǰʱ��ϵͳ��ƫ�Ƶ�����ʱ��ת��Ϊ����ʱ��

	char strTime[80];
    strftime(strTime,sizeof(strTime),nTimeStr,timeinfo);

    Lua_PushString(L,strTime);
    return 1;
}
//�򿪽�����
int LuaOpenJinDuTiao(Lua_State * L)
{
	//int nPlayerIndex;

    const	char *mTitle=(char *)Lua_ValueToString(L,1);   //����
	int  nMsgIndex=(int )Lua_ValueToNumber(L,2);           //ģ��ID  ���Ǹ�������
	int  nTimes=(int )Lua_ValueToNumber(L,3);              //�������ٺ�ִ�лص�����
    const	char *mFun=(char *)Lua_ValueToString(L,4);     //�ص���������
    const   char *szParma=(char *)Lua_ValueToString(L,5);  //���ݲ���һ
    const	char *szParmb=(char *)Lua_ValueToString(L,6);  //���ݲ�����
    const	char *szParmc=(char *)Lua_ValueToString(L,7);  //���ݲ�����
    return 0;
}


int LuaGetRandomNum(Lua_State * L)
{

	int nMaxNum = (int)Lua_ValueToNumber(L, 1);
	if (nMaxNum <0)
		nMaxNum=0;

	char nsg[64]={0};
	sprintf(nsg,"%d",g_Random(nMaxNum));
	Lua_PushString(L,nsg);
	return 1;

}




int LuaSetRandomSeed(Lua_State * L)
{
	int nSeed = (int)Lua_ValueToNumber(L, 1);

	if  (nSeed<=0)
		 nSeed=1;

	g_RandomSeed(nSeed);

	return 0;

}

int LuaGetRandomSeed(Lua_State * L)
{
	Lua_PushNumber(L,g_GetRandomSeed());
	return 1;

}

int LuaNewRandom(Lua_State * L)
{

	int nMinNum = (int)Lua_ValueToNumber(L, 1);
	int nMaxNum = (int)Lua_ValueToNumber(L, 2);

	if (nMinNum<0 || nMaxNum<0) return 0;

	if (nMinNum>nMaxNum)
	{//����
	   int inTemp= nMinNum;
	   nMinNum=nMaxNum;
	   nMaxNum=inTemp;
	}

	srand((unsigned)time(0));

    Lua_PushNumber(L,rand()%(nMaxNum-nMinNum)+nMinNum);

	return 1;

}


int LuaTabFile_UnLoad(Lua_State * L)
{
	return 0;
}
/*
int LuaTabFile_GetRowCount(Lua_State * L)
{
   return 0;
}

int LuaTabFile_GetCell(Lua_State * L)
{
  return 0;
}
*/
int LuaTabFile_Load(Lua_State * L)
{
   KTabFile TableFile;

   char *FiledName=NULL,*nKeyName=NULL,*LieName=NULL;
   int nRows=0,nReg=0,mLie=0,mRow=0;

   FiledName = (char *)Lua_ValueToString(L, 1);
   nKeyName = (char *)Lua_ValueToString(L, 2);  //�е��ֶ�
   nReg= TableFile.Load(FiledName);

  /*
   int mRowm=(int)Lua_ValueToNumber(L, 3);
   if (Lua_IsString(L,4))
   {

        LieName = (char *)Lua_ValueToString(L, 4);
        nReg=1;
   }
   else if (Lua_IsNumber(L,4))
   {
       mLie=(int)Lua_ValueToNumber(L, 4);
       nReg=1;
   }
*/
   TableFile.Clear();
   Lua_PushNumber(L,nReg);
//Lua_PushUserTag(L,,TableFile);
   return 1;

}


int LuaFileName2Id(Lua_State * L)
{
	char *strScript = (char *)Lua_ValueToString(L ,1);

	if (strScript)
      Lua_PushNumber(L, g_FileName2Id(strScript));
	else
	  Lua_PushNumber(L, 0);

	return 1;
}



//���ð���Ϊ��--�޸�����
int LuaSetRoomNull(Lua_State * L)
{
   int nPlayerIndex,mreg=0;

       nPlayerIndex   = CLIENT_PLAYER_INDEX;
/*
  	room_equipment = 0,	//0 װ����-����
	room_repository,	//1 ������
	room_exbox1,		//2 ��չ��1
	room_exbox2,		//3 ��չ��2
	room_exbox3,		//4 ��չ��3
	room_equipmentex,	//5 ͬ�����
	room_trade,			//6 ������
	room_tradeback,		//7 ���׹�����װ�����ı���
	room_trade1,		//8 ���׹����жԷ��Ľ�����
	room_immediacy,		//9 �����Ʒ
	room_give,          //10 ����ռ�
	room_giveback,      //11 ����ص��ռ�
	room_dazao,         //12 �����
	room_cailiao,       //13 ���Ͽ�
	room_num,			// �ռ�����

*/
	   int  nRoomidx=(int )Lua_ValueToNumber(L,1);

            mreg=Player[nPlayerIndex].m_ItemList.m_Room[nRoomidx].SetRoomNull();
  //#include	"KItemList.h"
	Lua_PushNumber(L,mreg);
	return 1;
}

//��ȡĳ�������µ�NPC SPR·��
int LuaGetMaskActionSpr(Lua_State * L)
{

	 int  nNpcSetIdx=(int )Lua_ValueToNumber(L,1);
	 int  nAction   =(int )Lua_ValueToNumber(L,2);

	 char nNpcKind[32]={0},nNpcRePath[128],nNpcActSpr[64]={0},nNpcSprFullPath[256]={0};
	 g_NpcSetting.GetString(nNpcSetIdx,"NpcResType","enemy003",nNpcKind,sizeof(nNpcKind));

	 KTabFile nNpcType;

	 if (nNpcType.Load("\\settings\\npcres\\��������.txt"))
	 {
		 nNpcType.GetString(nNpcKind,3,"",nNpcRePath,sizeof(nNpcRePath));
	 }
	 nNpcType.Clear();

	 if (nNpcType.Load("\\settings\\npcres\\��ͨnpc��Դ.txt"))
	 {
		 nNpcType.GetString(nNpcKind,nAction+1,"",nNpcActSpr,sizeof(nNpcActSpr));
	 }
	 nNpcType.Clear();

	 if (nNpcRePath[0] && nNpcActSpr[0])
		sprintf(nNpcSprFullPath,"\\%s\\%s",nNpcRePath,nNpcActSpr);
	 else
		return 0;

	 Lua_PushString(L, nNpcSprFullPath);
	 return 1;
}


//SetPos(X,Y) //����
int LuaSetPos(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
	if (nParamCount != 2) return 0;
	int nPlayerIndex=0;

	nPlayerIndex = CLIENT_PLAYER_INDEX;
	if  (nPlayerIndex<=0) return 0;

	int nX = (int) Lua_ValueToNumber(L,1);
	int nY = (int) Lua_ValueToNumber(L,2);
	int nResult=0;

	if (nPlayerIndex > 0)
	{
		PLAYER_SYN_POS mPos;
		mPos.ProtocolType = c2s_playersetpos;
		mPos.nXpos = nX;
		mPos.nYpos = nY;
		if (g_pClient)
			g_pClient->SendPackToServer((BYTE*)&mPos, sizeof(PLAYER_SYN_POS));
		nResult = 1 ;

	}

	Lua_PushNumber(L, nResult);
	return 1;
}


int LuaCalcEquiproomItemCount(Lua_State * L)
{

int nPlayerIndex=0;

	nPlayerIndex = CLIENT_PLAYER_INDEX;
	if  (nPlayerIndex<=0) return 0;
	int nCout=0;
	int nItemGen = (int)Lua_ValueToNumber(L, 1);
    int nDetail  = (int)Lua_ValueToNumber(L, 2);
	int nParticular    = (int)Lua_ValueToNumber(L, 3);
	int nLevel   = (int)Lua_ValueToNumber(L, 4);


	nCout = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetItemNum(nItemGen, nDetail, nParticular, nLevel);


	Lua_PushNumber(L,nCout);
    return 1;
}


//����
int LuaGetPlayerFaction(Lua_State * L)
{
	int nPlayerIndex = GetPlayerIndex(L);
	nPlayerIndex = CLIENT_PLAYER_INDEX;

	if (nPlayerIndex > 0)
	{
		char FactionName[64]={0};
		Player[nPlayerIndex].GetFactionName(FactionName, 64);
		Lua_PushString(L, FactionName);
	}
	else
	{
		Lua_PushString(L,"");
	}
	return 1;
}


int LuaGetLastFactionNumber(Lua_State * L)
{
	int nPlayerIndex = CLIENT_PLAYER_INDEX;
		if (nPlayerIndex <= 0)
			Lua_PushNumber(L,-1);
		else
			Lua_PushNumber(L, Player[nPlayerIndex].m_cFaction.m_nFirstAddFaction);


	return 1;
}


int LuaGetLevel(Lua_State * L)
{

	if (Lua_GetTopIndex(L) >0)
	{
		int nNpcIdx = (int)Lua_ValueToNumber(L, 1);
		Lua_PushNumber(L, Npc[nNpcIdx].m_Level);
	}
	else
	{
		int nPlayerIndex =  CLIENT_PLAYER_INDEX;
		if (nPlayerIndex <= 0)		Lua_PushNumber(L,0);
		if (Player[nPlayerIndex].m_nIndex <= 0) Lua_PushNumber(L,0);
		Lua_PushNumber(L, Npc[Player[nPlayerIndex].m_nIndex].m_Level);
	}
	return 1;
}


//W,X,Y = GetWorldPos()
int LuaGetNewWorldPos(Lua_State * L)
{
	int nPlayerIndex = CLIENT_PLAYER_INDEX;

	if (nPlayerIndex > 0)
	{
		int nPosX = 0,nPosY = 0,nPmap=0;
		Npc[Player[nPlayerIndex].m_nIndex].GetMpsPos(&nPosX, &nPosY,&nPmap);

		int nSubWorldIndex = Npc[Player[nPlayerIndex].m_nIndex].m_SubWorldIndex;
		int nSubWorldID = 0;

		if (nSubWorldIndex >= 0 && nSubWorldIndex < MAX_SUBWORLD)
		{
			nSubWorldID = SubWorld[nSubWorldIndex].m_SubWorldID;
		}

		Lua_PushNumber(L, nSubWorldID);
		Lua_PushNumber(L, ((int)(nPosX / 32)));
		Lua_PushNumber(L, ((int)(nPosY / 32)));
	}
	else
	{
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		Lua_PushNumber(L, 0);
		return 3;
	}
	return 3;
}



//==============����Ϊͨ��ָ��===============�������˽ű�������ʼ=======================
int LuaPlayMusic(Lua_State * L)//PlayMusic(musicname,loop=1, vol );
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;

	int nPlayerIndex=0;

#ifdef _SERVER
	nPlayerIndex = GetPlayerIndex(L);
#else
	nPlayerIndex = CLIENT_PLAYER_INDEX;
#endif

	//int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex < 0) return 0;

	PLAYER_SCRIPTACTION_SYNC UiInfo;
	ZeroStruct(UiInfo);
	UiInfo.m_bUIId = UI_PLAYMUSIC;
	UiInfo.m_bOptionNum = 1;
	UiInfo.m_nOperateType = SCRIPTACTION_UISHOW;
//	Player[nPlayerIndex].m_nCheckWaiGua =0;
	int nMsgId = 0;

	g_StrCpyLen(UiInfo.m_pContent, Lua_ValueToString(L,1), sizeof(UiInfo.m_pContent));
	UiInfo.m_nBufferLen = strlen(((char *)UiInfo.m_pContent));
	UiInfo.m_bParam1 = 0;

#ifndef _SERVER
	UiInfo.m_bParam2 = 0;
#else
	UiInfo.m_bParam2 = 1;
#endif

	Player[nPlayerIndex].DoScriptAction(&UiInfo);
	return 0;
}

int LuaFadeInMusic(Lua_State * L)
{
	return 0;
}

int LuaFadeOutMusic(Lua_State * L)
{
	return 0;
}

//===========================�ͻ��˽ű�����========================
#ifndef _SERVER
int LuaPlaySound(Lua_State * L)
{
	return 0;
}

/*
int Luadostring(Lua_State * L)
{
    char *Fun=(char *)Lua_ValueToString(L,1);
	lua_dostring(L,Fun);
	return 0;
}
*/

int LuaNewTask_AddNpcFindPath(Lua_State * L)
{
    int nTaskGenre = (int)Lua_ValueToNumber(L,1);
    int nMsgIndex = (int)Lua_ValueToNumber(L,2);
	int nTaskMapId = (int)Lua_ValueToNumber(L,3);
	int nMapXpos = (int)Lua_ValueToNumber(L,4);
	int nMapYpos = (int)Lua_ValueToNumber(L,5);
	// Player[CLIENT_PLAYER_INDEX].m_cTask.nTaskGenre=nTaskGenre;
	//sprintf(Player[CLIENT_PLAYER_INDEX].m_cTask.nTaskDesc,nTaskDesc);
	KTaskPathInfo nInfo;
        nInfo.nTaskType=nTaskGenre;
        nInfo.nMsgIndex=nMsgIndex;
		nInfo.nMapIdx=nTaskMapId;
		nInfo.nMapXpos=nMapXpos;
		nInfo.nMapYpos=nMapYpos;
	CoreDataChanged(GDCNI_TASK_INFO, (uintptr_t)&nInfo,1,0);
   return 0;
}

int LuaNewTask_SetFinishedTask(Lua_State * L)
{
	return 0;
}

int LuaNewTask_DetailTextOut(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
    int nTaskGenre = (int)Lua_ValueToNumber(L,1);
    char *nTaskDesc=(char *)Lua_ValueToString(L,2);
	int nIsBtn=0;
	if  (nParamCount>=3)
		nIsBtn = (int)Lua_ValueToNumber(L,3);
	// Player[CLIENT_PLAYER_INDEX].m_cTask.nTaskGenre=nTaskGenre;
	//sprintf(Player[CLIENT_PLAYER_INDEX].m_cTask.nTaskDesc,nTaskDesc);
	KNewsMessage News;
	//����ϢΪ�ַ���
	//g_StrCpyLen(News.sMsg, nTaskDesc, sizeof(News.sMsg));
	News.nType =	nTaskGenre;
	sprintf(News.sMsg,"%s",nTaskDesc);
	// News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
	//SYSTEMTIME systime;
	//memset(&systime, 0, sizeof(SYSTEMTIME));
	CoreDataChanged(GDCNI_TASK_INFO, (uintptr_t)&News,0,nIsBtn);//;(uintptr_t)&News
	return 0;
}

int LuaNewTask_TaskTextOut(Lua_State * L)
{
	int nParamCount = Lua_GetTopIndex(L);
    int nTaskGenre = (int)Lua_ValueToNumber(L,1);
    char *nTaskDesc=(char *)Lua_ValueToString(L,2);
	int nIsBtn=0;
	if  (nParamCount>=3)
		nIsBtn = (int)Lua_ValueToNumber(L,3);

  // Player[CLIENT_PLAYER_INDEX].m_cTask.nTaskGenre=nTaskGenre;
   //sprintf(Player[CLIENT_PLAYER_INDEX].m_cTask.nTaskDesc,nTaskDesc);
   KNewsMessage News;
   //����ϢΪ�ַ���
   //g_StrCpyLen(News.sMsg, nTaskDesc, sizeof(News.sMsg));
   News.nType =	nTaskGenre;
   sprintf(News.sMsg,"%s",nTaskDesc);
  // News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
   //SYSTEMTIME systime;
   //memset(&systime, 0, sizeof(SYSTEMTIME));
   CoreDataChanged(GDCNI_TASK_INFO, (uintptr_t)&News,0,nIsBtn);//;(uintptr_t)&News
   return 0;
}

int LuaNewTask_AddIcon(Lua_State * L)
{
	int nTaskType = (int)Lua_ValueToNumber(L, 1);
    char *nTaskIcocPath = (char *)Lua_ValueToString(L, 2);
	int nTaskIdx = (int)Lua_ValueToNumber(L, 3);
	char *nTaskName = (char *)Lua_ValueToString(L, 4);
	if (nTaskIdx>20 || nTaskIdx<0)
		return 0;

	Player[CLIENT_PLAYER_INDEX].m_cTask.nTaskInfo[nTaskIdx].nTaskType=nTaskType;
    Player[CLIENT_PLAYER_INDEX].m_cTask.nTaskInfo[nTaskIdx].nTaskidx=nTaskIdx;
	sprintf(Player[CLIENT_PLAYER_INDEX].m_cTask.nTaskInfo[nTaskIdx].nTaskIconPath, "%s", nTaskIcocPath);
	sprintf(Player[CLIENT_PLAYER_INDEX].m_cTask.nTaskInfo[nTaskIdx].nTaskName, "%s", nTaskName);

    Lua_PushNumber(L,1);
	return 1;
}
//�ͻ���ִ���Զ���ҽű�
int LuaAutoForbitGua(Lua_State * L)
{
	//int nPlayerIndex = GetPlayerIndex(L);
/*	int nFReg=0;
 //////////////////�����ҳ���/////////////////////////
	int IsForBit[2];  //Ĭ�Ͽ������
	g_GameSetTing.GetInt2("SYSTEM","IsForBitGua",IsForBit);

	int nMun=0;
	int wgRows=g_ForbitWaiGua.GetHeight()+1;

	for (int i=2;i<wgRows;++i)
	{
		char nKey[16],nReg=0;
		g_ForbitWaiGua.GetString(i+2,"GuaNameKey","��",nKey,sizeof(nKey));
		nReg= g_ForBitGua.GetProcessList(nKey,wgRows,i);//CheckWaiGua(nKey);
		if (nReg>=1)
		{//��⵽���

			nFReg=1;
			break;
		}
		else if (nReg<0)
		{//��Ȩʧ��
			nFReg=-1;
			break;
		}
		else  if (nReg==0)
		{//û�����
			nMun++;
//			break;
		}
		g_ForBitGua.Colse();
	}
	if (nMun==wgRows) //�������ͨ����Ҽ��
	{//ͬ����Ҳ�������������
		SKILL_LEFT_SYNC	sLeftSkill;
		sLeftSkill.ProtocolType = c2s_skillsync;
		sLeftSkill.m_nLeftskill =0;
		sLeftSkill.m_Type=1;

//		if (g_pClient)
	//	g_pClient->SendPackToServer(&sLeftSkill, sizeof(SKILL_LEFT_SYNC));
		nFReg=0;
	}
    Lua_PushNumber(L,nFReg);*/
	return 0;
}

//˲�似����Ч������0 �Լ��ͷ� 1Ϊָ������NPC�ͷţ�
int LuaCastSkill(Lua_State * L)
{
	if (Lua_GetTopIndex(L) < 1)
		return 0;
	int nPlayerIndex = GetPlayerIndex(L);
	if (nPlayerIndex<0) return 0;
	int nIndexID=0;
	int nStateID=0;
    int nType = (int)Lua_ValueToNumber(L, 1);
	nIndexID = (int)Lua_ValueToNumber(L, 2);
	nStateID = (int)Lua_ValueToNumber(L, 3);
	if (nType==0)
        Npc[Player[nPlayerIndex].m_nIndex].SetInstantSpr(nStateID);
    else if (nType==1)
		Npc[nIndexID].SetInstantSpr(nStateID);     //�ͷ�һ��˲����Ч

	return 0;
}

int LuaPlaySprMovie(Lua_State * L)
{
	return 0;
}
#endif
//==============================================================
TLua_Funcs GameScriptFuns[] =
{
	//ͨ��ָ��
	{"Say", LuaSayUI},
	{"SayNew",LuaSayNew},
	{"Describe",LuaSayNew},
	{"CreateTaskSay",LuaCreateTaskSay},	 //֧�ֱ��
	{"CreateNewSayEx",LuaCreateNewSayEx},
	{"CreateSprSay",LuaCreateSprSay},

	{"Talk", LuaTalkUI},
	{"GetTaskTemp", LuaGetTempTaskValue},
	{"SetTaskTemp", LuaSetTempTaskValue},
	{"print", LuaMessage},	  //print	 printf
	{"Mathematical", LuaMathematical},
	{"GetBit",	LuaGetBit},
	{"GetByte",	LuaGetByte},
	{"SetBit",	LuaSetBit},
	{"SetByte",	LuaSetByte},
	{"Include",LuaIncludeFile},
	{"IncludeLib",LuaIncludeLib},
	{"IL",LuaIncludeLib},
	{"PutMessage", LuaSendMessageInfo},
	{"AddGlobalNews",LuaAddGlobalNews},
	{"AddGlobalTimeNews",LuaAddGlobalTimeNews},
	{"AddGlobalCountNews",LuaAddGlobalCountNews},
	{"AddLocalNews",LuaAddLocalNews},
	{"AddSysNews"  ,LuaAddSysNews},
	{"AddLocalTimeNews",LuaAddLocalTimeNews},
	{"AddLocalCountNews",LuaAddLocalCountNews},
	{"GetTask",			LuaGetTaskValue},	    //GetTask(�����):��õ�ǰ��Ҹ�����ŵ�ֵ
	{"SetTask",			LuaSetTaskValue},	    //SetTask(�����,ֵ):��������ֵ
	{"SetDeathSkill",			LuaSetDeathSkill},	    //SetTask(�����,ֵ):��������ֵ
	{"GetDeathSkill",			LuaGetDeathSkill},
	{"AddTask",			LuaAddTaskValue},	    //
	{"GetTabFileInfo",  LuaGetTabFileInfo},     //��ȡTXT�ļ� ĳ�� ĳ�� ������ �������ַ�������  3������
	{"GetIniFileInfo",  LuaGetIniFileInfo},     //��ȡini�ļ� ĳ�� ĳ�� ������

	{"GetTabRowAndCol", LuaGetTabRowAndCol},    //��ȡ�ļ��� ������������   1������

	{"IniFile_GetData",  LuaGetIniFileInfo},
	{"TaskAwardMatrix", LuaGetTaskAwardMatrix}, //��ȡ���� ͬһ�����Ƶ� �ж�����
    {"CalcFreeItemCellCount", LuaCalcFreeItemCellCount},
	{"GetPositionCount",        LuaGetPositionCount},

	{"GetLocalDate", LuaGetLocalDate},
	{"OpenJinDuTiao", LuaOpenJinDuTiao},
	{"SetRoomNull",	LuaSetRoomNull},
	{"FileName2Id",	LuaFileName2Id},

	{"TabFile_Load",             LuaTabFile_Load},         //����TXT�ļ�
	{"TabFile_GetCell",          LuaGetTabFileInfo},
	{"TabFile_GetRowCount",      LuaGetTabRowAndCol},
	{"TabFile_UnLoad",           LuaTabFile_UnLoad},
	{"GetRandomSeed",           LuaGetRandomSeed},
	{"GetRandomNum",           LuaGetRandomNum},
	{"NewRandom",           LuaNewRandom},
	{"SetRandomSeed",       LuaSetRandomSeed},

	{"GetWorldPos",		LuaGetNewWorldPos},	//W,X,Y = GetWorldPos()������NewWorld���������
	{"GetLevel",		LuaGetLevel},			        //GetLevel()GetPlayers Level
	{"GetLastFactionNumber", LuaGetLastFactionNumber},
	{"GetFaction",		LuaGetPlayerFaction},    //GetFaction()�����ҵ�������
	{"GetLastAddFaction",LuaGetPlayerFaction},
	{"GetMaskActionSpr", LuaGetMaskActionSpr},

	{"CalcEquiproomItemCount", LuaCalcEquiproomItemCount},
	{"SetPos",			LuaSetPos},			//SetPos(x,y)����ĳ��
	//�ͻ��˽ű�ָ��
	{"PlaySound",       LuaPlaySound},      //PlaySound(Sound);
	{"PlaySprMovie",    LuaPlaySprMovie},   //PlaySprMovie(npcindex, Movie, times)
	{"AutoForbitGua",   LuaAutoForbitGua},  //�ͻ���ִ���Զ����
	{"CastSkill",       LuaCastSkill},      //˲����Ч
	{"NewTask_AddIcon", LuaNewTask_AddIcon},
	{"NewTask_TaskTextOut", LuaNewTask_TaskTextOut},
	{"NewTask_DetailTextOut", LuaNewTask_DetailTextOut},

	{"NewTask_SetFinishedTask", LuaNewTask_SetFinishedTask},   //�����������

	{"NewTask_AddNpcFindPath",  LuaNewTask_AddNpcFindPath},     //������Ϣ����Ѱ·
	//{"dostring", Luadostring},
	{"PlayMusic",       LuaPlayMusic}, //PlayMusic(Music,Loop)
	{"FadeInMusic",     LuaFadeInMusic},
	{"FadeOutMusic",    LuaFadeOutMusic},
};


TLua_Funcs WorldScriptFuns[] =// ��ָ����ҵĽű�ָ�
{
	//ͨ��ָ��
	{"AddLocalNews",     LuaAddLocalNews},
	{"AddSysNews",       LuaAddSysNews},
	{"AddLoaclTimeNews", LuaAddLocalTimeNews},
	{"AddLocalCountNews",LuaAddLocalCountNews},
};

int g_GetGameScriptFunNum()
{
	return sizeof(GameScriptFuns)  / sizeof(GameScriptFuns[0]);
}

int g_GetWorldScriptFunNum()
{
	return sizeof(WorldScriptFuns)  / sizeof(WorldScriptFuns[0]);
}
