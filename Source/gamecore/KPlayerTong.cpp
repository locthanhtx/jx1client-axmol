//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerTong.cpp
// Date:	2003.08.12
// Code:	�߳�����
// Desc:	KPlayerTong Class
//---------------------------------------------------------------------------

#include	"KCore.h"
#include	"KNpc.h"
#include	"KPlayer.h"
#include	"KPlayerSet.h"
#include	"KPlayerTong.h"
#include    "engine/Text.h"
//#include "../../Engine/src/KFilePath.h"
#include     "engine/KFilePath.h"
#include     "engine/KTabFile.h"
#include     "engine/KTabFileCtrl.h"

#include "KProtocolDef.h"
#include "KProtocol.h"
#include "KRelayProtocol.h"
#include "KTongProtocol.h"
#include	"CoreShell.h"

#define     TONG_DATA_FILE             "\\TongSet\\"
#define     TONG_SET_FILE             "\\TongSet\\TongSet.ini"
//#define		defTONG_NAME_LENGTH			8
#define		defFuncShowNormalMsg(str,nKind)		\
	{										\
		KSystemMessage	sMsg;				\
		sMsg.eType = SMT_NORMAL;			\
		sMsg.byConfirmType = SMCT_NONE;		\
		sMsg.byPriority = 0;				\
		sMsg.byParamSize = 0;				\
		sprintf(sMsg.szMessage, "%s", str);		\
		if (nKind==0)\
		   sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));\
		else\
		   sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));\
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);\
	}

//-------------------------------------------------------------------------
//	���ܣ���ʼ��
//-------------------------------------------------------------------------
void	KPlayerTong::Init(int nPlayerIdx)
{
	nAttAckInfo.clear();
	m_nPlayerIndex = nPlayerIdx;

	ActiveLoop=0;

	Clear();
}

//-------------------------------------------------------------------------
//	���ܣ����
//-------------------------------------------------------------------------
void	KPlayerTong::Clear()
{
	m_nFlag				= 0;
	m_nFigure			= enumTONG_FIGURE_MEMBER;
	m_nCamp				= 0;
	m_dwTongNameID		= 0;
	m_szName[0]			= 0;
	m_szTitle[0]		= 0;
	m_BMasterName[0]	= 0;
	m_nApplyTo			= 0;
	m_nMoney			= 0;
	m_AttackState       = 0;
	m_AttackCamp        = 0;
	m_nDeathCount       = 0;
    m_AttackTime        = 0;
    m_AttackName[0]     = 0;
	m_Mapid             = 0;
	m_CurRenShu			= 0;
	m_nLevel            = 1;

	m_nAttackNum		= 0;	                            // ��ս����
    m_nWinNum		    = 0;								// ʤ������
    m_nLoseNum		    = 0;
    sprintf(m_AttackName,"����");
}

//-------------------------------------------------------------------------
//	���ܣ����봴�����
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyCreateTong(int nCamp, char *lpszTongName)
{
	defFuncShowNormalMsg(strCoreInfo[MSG_TONG_APPLY_CREATE].c_str(),1);
	// ���������
	if (!lpszTongName || !lpszTongName[0] || strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_CREATE_ERROR01].c_str(),1);
		return FALSE;
	}
	// �����Ӫ����
	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_CREATE_ERROR06].c_str(),1);
		return FALSE;
	}
	// �Ѿ��ǰ���Ա
	if (m_nFlag)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_CREATE_ERROR05].c_str(),1);
		return FALSE;
	}
	// �Լ�����Ӫ����
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp != camp_free)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_CREATE_ERROR04].c_str(),1);
		return FALSE;
	}
	// �ȼ�����
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Level < PlayerSet.m_sTongParam.m_nLevel)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_CREATE_ERROR07].c_str(),1);
		return FALSE;
	}
	if ((int)Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel < PlayerSet.m_sTongParam.m_nLeadLevel)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_CREATE_ERROR06].c_str(),1);
		return FALSE;
	}
	// ���׹�����
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
	{
		return FALSE;
	}
	// Ǯ����
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoneyAmount() < PlayerSet.m_sTongParam.m_nMoney)
	{
		char	szBuf[80];
		sprintf(szBuf, strCoreInfo[MSG_TONG_CREATE_ERROR08].c_str(), PlayerSet.m_sTongParam.m_nMoney);
		defFuncShowNormalMsg(szBuf,1);
		return FALSE;
	}
	// ��Ӳ��ܽ����
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_CREATE_ERROR09].c_str(),1);
		return FALSE;
	}

	TONG_APPLY_CREATE_COMMAND	sApply;
	sApply.ProtocolType = c2s_extendtong;
	sApply.m_wLength = sizeof(TONG_APPLY_CREATE_COMMAND) - 1;
	sApply.m_btMsgId = enumTONG_COMMAND_ID_APPLY_CREATE;
	sApply.m_btCamp = (BYTE)nCamp;
	strcpy(sApply.m_szName, lpszTongName);

	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sApply.m_wLength + 1);

	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ����������
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyAddTong(DWORD dwNpcID)
{
	defFuncShowNormalMsg(strCoreInfo[MSG_TONG_APPLY_ADD].c_str(),1);
	// �Ѿ��ǰ���Ա
	if (m_nFlag)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_APPLY_ADD_ERROR1].c_str(),1);
		return FALSE;
	}
	// �Լ�����Ӫ����
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp != camp_free)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_APPLY_ADD_ERROR2].c_str(),1);
		return FALSE;
	}
	// ���׹�����
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
	{
		return FALSE;
	}
	// ��Ӳ��ܽ����
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_APPLY_ADD_ERROR3].c_str(),1);
		return FALSE;
	}

	TONG_APPLY_ADD_COMMAND	sApply;
	sApply.ProtocolType = c2s_extendtong;
	sApply.m_wLength = sizeof(TONG_APPLY_ADD_COMMAND) - 1;
	sApply.m_btMsgId = enumTONG_COMMAND_ID_APPLY_ADD;
	sApply.m_dwNpcID = dwNpcID;

	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(TONG_APPLY_ADD_COMMAND));

	return TRUE;
}

//�ͻ���
//-------------------------------------------------------------------------
//	���ܣ��õ�������֪ͨ�������
//-------------------------------------------------------------------------
void	KPlayerTong::Create(TONG_CREATE_SYNC *psCreate)
{
	if (!psCreate)
		return;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = psCreate->m_btCamp;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = psCreate->m_btCamp;

	m_nFlag			= 1;
	m_nFigure		= enumTONG_FIGURE_MASTER;
	m_nCamp			= psCreate->m_btCamp;
	m_AttackState   = 0;
	m_szTitle[0]	= 0;
	m_AttackCamp        = 0;
    m_AttackTime        = 0;
    m_AttackName[0]     = 0;
	m_nAttackNum		= 0;	                            // ��ս����
    m_nWinNum		    = 0;								// ʤ������
    m_nLoseNum		    = 0;

	memset(m_szName, 0, sizeof(m_szName)); //�����
	memcpy(m_szName, psCreate->m_szName, sizeof(psCreate->m_szName));
	strcpy(m_BMasterName,Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);//Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name)
	//strcpy(m_szMasterName,Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);//Npc[P
	m_dwTongNameID	= g_FileName2Id(psCreate->m_szName);

	// ����֪ͨ��Ὠ���ɹ�
	defFuncShowNormalMsg(strCoreInfo[MSG_TONG_CREATE_SUCCESS].c_str(),1);

	// ֪ͨ���Ƶ�� ���»�����Ϣ
	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);

}

void KPlayerTong::Active()
{

/*	ActiveLoop++;
	//if (!m_nFlag)
	//	return;
	if (m_nFigure == enumTONG_FIGURE_MASTER)
	{
		//defFuncShowNormalMsg(MSG_TONG_CANNOT_LEAVE1);
		//return FALSE;
	}
	if (m_nFigure == enumTONG_FIGURE_DIRECTOR)
	{
		//defFuncShowNormalMsg(MSG_TONG_CANNOT_LEAVE2);
		//return FALSE;
	}

   if (ActiveLoop>18 && ActiveLoop%18==0)
   {
	TONG_APPLY_GET_ATTACK	sAttackGet;
	sAttackGet.ProtocolType		= c2s_extendtong;
	sAttackGet.m_wLength		= sizeof(sAttackGet) - 1;
	sAttackGet.m_btMsgId		= enumTONG_COMMAND_ID_APPLY_ATTACK_GET;
	if (m_nFlag)
	   sAttackGet.m_dwTongNameID= Player[CLIENT_PLAYER_INDEX].m_cTong.m_dwTongNameID; //g_FileName2Id(this->m_szName);
	else
	   sAttackGet.m_dwTongNameID= 0;

	sAttackGet.m_NpcDwid        = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAttackGet, sAttackGet.m_wLength + 1);
   }

   if (ActiveLoop>30000000)
	   ActiveLoop=0; */
}

void KPlayerTong::SendAttack(char *lpszTongName)
{
	if (!m_nFlag)
	{
		defFuncShowNormalMsg("��ʾ:ֻ�� ������� �ſ��Է�������ս!",0);
		return;
	}
	if (m_nFigure== enumTONG_FIGURE_MASTER || m_nFigure == enumTONG_FIGURE_DIRECTOR)
	{
		TONG_APPLY_SEND_ATTACK	sAttackSend;
		sAttackSend.ProtocolType	= c2s_extendtong;
		sAttackSend.m_wLength		= sizeof(sAttackSend) - 1;
		sAttackSend.m_btMsgId		= enumTONG_COMMAND_ID_APPLY_ATTACK_SEND;
		sAttackSend.m_dwYTongNameID	= Player[CLIENT_PLAYER_INDEX].m_cTong.m_dwTongNameID;        //g_FileName2Id(this->m_szName);
		sAttackSend.m_dwDTongNameID = g_FileName2Id(lpszTongName);
		sAttackSend.m_NpcDwid       = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwID;
		if (g_pClient)
			g_pClient->SendPackToServer(&sAttackSend, sAttackSend.m_wLength + 1);
	}
    else
	{
		defFuncShowNormalMsg("��ʾ:ֻ�� ���� �� ���� �ſ��Է�������ս!",0);
	}
	return;
}

void    KPlayerTong::SetTongLevel(char *lpszTongName,int nLevel)
{

}

void    KPlayerTong::SetTongMap(char *lpszTongName,int nMap)
{

}

void	KPlayerTong::SetTongCredit(char *lpszTongName,int nCredit)        //�����ʽ�
{

}
void	KPlayerTong::SetTongMasterTitle(char *lpszTongName,char *nTitle)  //�����ƺ�
{

}
void	KPlayerTong::SetTongDirectorTitle(char *lpszTongName,char *nTitle)//���ϳƺ�
{

}
void	KPlayerTong::SetTongManagerTitle(char *lpszTongName,char *nTitle)//�ӳ��ƺ�
{

}
void	KPlayerTong::SetTongNormalTitle(char *lpszTongName,char *nTitle)  //���ڳƺ�
{

}

//-------------------------------------------------------------------------
//	���ܣ��Ƿ���ܳ�Ա bFlag == TRUE ���� == FALSE ������
//-------------------------------------------------------------------------
void	KPlayerTong::AcceptMember(int nPlayerIdx, DWORD dwNameID, BOOL bFlag)
{
	if (nPlayerIdx <= 0)
		return;

	TONG_ACCEPT_MEMBER_COMMAND	sAccept;
	sAccept.ProtocolType	= c2s_extendtong;
	sAccept.m_wLength		= sizeof(TONG_ACCEPT_MEMBER_COMMAND) - 1;
	sAccept.m_btMsgId		= enumTONG_COMMAND_ID_ACCEPT_ADD;
	sAccept.m_nPlayerIdx	= nPlayerIdx;   // �����Playerid
	sAccept.m_dwNameID		= dwNameID;     // �����˵�����ID  Npc[Player[nPlayerIdx].m_nIndex].m_dwID;//
	sAccept.m_btFlag		= (bFlag != 0);

	if (g_pClient)
		g_pClient->SendPackToServer(&sAccept, sAccept.m_wLength + 1);
}


void	KPlayerTong::GetTongName(char *lpszGetName)
{
	//if (!lpszGetName)
	//	return;
	if (!m_nFlag)
	{
		lpszGetName[0] = 0;
		return;
	}

	strcpy(lpszGetName, m_szName);
}

//-------------------------------------------------------------------------
//	���ܣ������ᣬ��Ϊ��ͨ����
//-------------------------------------------------------------------------
BOOL	KPlayerTong::AddTong(int nCamp, char *lpszTongName, char *lpszTitle, char *lpszMaster)
{
	if (!lpszTongName || !lpszTongName[0]/* || strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH*/)
		return FALSE;
	// �����Ӫ����
	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
		return FALSE;

	m_nFlag		= 1;
	m_nFigure	= enumTONG_FIGURE_MEMBER;
	m_nCamp		= nCamp;
	strcpy(m_szName, lpszTongName);
	if (!lpszTitle)
		this->m_szTitle[0] = 0;
	else
		strcpy(m_szTitle, lpszTitle);
	if (!lpszMaster)
		this->m_BMasterName[0] = 0;
	else
		strcpy(m_BMasterName, lpszMaster);
	m_dwTongNameID	= g_FileName2Id(lpszTongName);

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = m_nCamp;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = m_nCamp;

	defFuncShowNormalMsg(strCoreInfo[MSG_TONG_ADD_SUCCESS].c_str(),1);

	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ���������
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyInstate(int nCurFigure, int nCurPos, int nNewFigure, int nNewPos, char *lpszName)
{
	if (!lpszName)
		return FALSE;
	if (!m_nFlag)
		return FALSE;
	if (nCurFigure < 0 || nCurFigure >= enumTONG_FIGURE_NUM ||
		nNewFigure < 0 || nNewFigure >= enumTONG_FIGURE_NUM)
		return FALSE;
	if (nCurPos < 0 || nNewPos < 0)
		return FALSE;
	if (nCurFigure == nNewFigure && nCurPos == nNewPos)
		return FALSE;

	switch (m_nFigure)
	{
	case enumTONG_FIGURE_MEMBER:
		// ����û������Ȩ��
		return FALSE;
	case enumTONG_FIGURE_MANAGER:
		// �ӳ�û������Ȩ��
		return FALSE;
	case enumTONG_FIGURE_DIRECTOR:
		// ����ֻ�жԶӳ������ڵ�����Ȩ��
		if ((nCurFigure != enumTONG_FIGURE_MANAGER && nCurFigure != enumTONG_FIGURE_MEMBER) ||
			(nNewFigure != enumTONG_FIGURE_MANAGER && nNewFigure != enumTONG_FIGURE_MEMBER))
			return FALSE;
		if (nCurFigure == enumTONG_FIGURE_MANAGER && nCurPos >= defTONG_MAX_MANAGER)
			return FALSE;
		if (nNewFigure == enumTONG_FIGURE_MANAGER && nNewPos >= defTONG_MAX_MANAGER)
			return FALSE;
		break;
	case enumTONG_FIGURE_MASTER:
		// �����жԳ��ϡ��ӳ������ڵ�����Ȩ��
		if (nCurFigure == enumTONG_FIGURE_MASTER || nNewFigure == enumTONG_FIGURE_MASTER)
			return FALSE;
		if (nCurFigure == enumTONG_FIGURE_MANAGER && nCurPos >= defTONG_MAX_MANAGER)
			return FALSE;
		if (nCurFigure == enumTONG_FIGURE_DIRECTOR && nCurPos >= defTONG_MAX_DIRECTOR)
			return FALSE;
		if (nNewFigure == enumTONG_FIGURE_MANAGER && nNewPos >= defTONG_MAX_MANAGER)
			return FALSE;
		if (nNewFigure == enumTONG_FIGURE_DIRECTOR && nNewPos >= defTONG_MAX_DIRECTOR)
			return FALSE;
		break;
	default:
		return FALSE;
	}

	TONG_APPLY_INSTATE_COMMAND	sApply;

	if (strlen(lpszName) >= sizeof(sApply.m_szName))
		return FALSE;
	sApply.ProtocolType   = c2s_extendtong;
	sApply.m_btMsgId      = enumTONG_COMMAND_ID_APPLY_INSTATE;
	sApply.m_dwTongNameID = g_FileName2Id(this->m_szName);
	sApply.m_btCurFigure  = nCurFigure;
	sApply.m_btCurPos     = nCurPos;
	sApply.m_btNewFigure  = nNewFigure;
	sApply.m_btNewPos     = nNewPos;
	memset(sApply.m_szName, 0, sizeof(sApply.m_szName));
	strcpy(sApply.m_szName, lpszName);
	sApply.m_wLength      = sizeof(sApply) - 1;

	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sApply.m_wLength + 1);

	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ���������
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyKick(int nCurFigure, int nCurPos, char *lpszName)
{
	if (!lpszName || !lpszName[0] || strlen(lpszName) >= 32)
		return FALSE;
	if (!m_nFlag)
		return FALSE;

	if (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR)   //ִ�����˵�ְλ
	{
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("��ʾ:���ܽ������˲���!");
		return FALSE;
	}

	if (nCurFigure != enumTONG_FIGURE_MANAGER && nCurFigure != enumTONG_FIGURE_MEMBER)  //���ߵ��˵�ְλ
	{
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("��ʾ:���ܶԳ��Ͻ��в��� !");
		return FALSE;
	}

	if (nCurFigure == enumTONG_FIGURE_MANAGER /*&& (nCurPos < 0 || nCurPos >= defTONG_MAX_MANAGER)*/)
	{
        Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("��ʾ:���ܶԶӳ����в��� !");
		return FALSE;//�����˵�ְλ
	}

	TONG_APPLY_KICK_COMMAND	sKick;
	sKick.ProtocolType		= c2s_extendtong;
	sKick.m_wLength			= sizeof(sKick) - 1;
	sKick.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_KICK;
	sKick.m_btFigure		= nCurFigure;
	sKick.m_btPos			= nCurPos;
	sKick.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	strcpy(sKick.m_szName, lpszName);

	if (g_pClient)
		g_pClient->SendPackToServer(&sKick, sKick.m_wLength + 1);
	char msg[64];
	sprintf(msg,"��ʾ:�ѽ�(%s),�߳����!",lpszName);
    Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(msg);
	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ����봫λ
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyChangeMaster(int nCurFigure, int nPos, char *lpszName)
{
	if (!lpszName || !lpszName[0] || strlen(lpszName) >= 32)
		return FALSE;
	if (!m_nFlag)
		return FALSE;
	if (m_nFigure != enumTONG_FIGURE_MASTER)
		return FALSE;

	TONG_APPLY_CHANGE_MASTER_COMMAND	sChange;
	sChange.ProtocolType	= c2s_extendtong;
	sChange.m_wLength		= sizeof(sChange) - 1;
	sChange.m_btMsgId		= enumTONG_COMMAND_ID_APPLY_CHANGE_MASTER;
	sChange.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	sChange.m_btFigure		= nCurFigure;
	sChange.m_btPos			= nPos;
	strcpy(sChange.m_szName, lpszName);
	if (g_pClient)
		g_pClient->SendPackToServer(&sChange, sChange.m_wLength + 1);

	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ������뿪��� �Ѱ�
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyLeave()
{
	if (!m_nFlag)
		return FALSE;
	if (m_nFigure == enumTONG_FIGURE_MASTER)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_CANNOT_LEAVE1].c_str(),1);
		return FALSE;
	}
	if (m_nFigure == enumTONG_FIGURE_DIRECTOR)
	{
		defFuncShowNormalMsg(strCoreInfo[MSG_TONG_CANNOT_LEAVE2].c_str(),1);
		return FALSE;
	}

	TONG_APPLY_LEAVE_COMMAND	sLeave;
	sLeave.ProtocolType		= c2s_extendtong;
	sLeave.m_wLength		= sizeof(sLeave) - 1;
	sLeave.m_btMsgId		= enumTONG_COMMAND_ID_APPLY_LEAVE;
	sLeave.m_btFigure		= m_nFigure;
	sLeave.m_btPos			= 0;
	sLeave.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	strcpy(sLeave.m_szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	if (g_pClient)
		g_pClient->SendPackToServer(&sLeave, sLeave.m_wLength + 1);

	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ������ð����Ϣ
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyInfo(int nInfoID, int nParam1, int nParam2, int nParam3, char *lpszName/*=NULL*/)
{
	if (nInfoID < 0 || nInfoID >= enumTONG_APPLY_INFO_ID_NUM)
		return FALSE;

	TONG_APPLY_INFO_COMMAND	sInfo;
	sInfo.ProtocolType = c2s_extendtong;
	sInfo.m_btMsgId = enumTONG_COMMAND_ID_APPLY_INFO;
	sInfo.m_btInfoID = nInfoID;
	sInfo.m_nParam1 = nParam1;	  //TONGID
	sInfo.m_nParam2 = nParam2;	  //��ʼλ��
	sInfo.m_nParam3 = nParam3;    //ÿ�λ�ȡ����

	switch (nInfoID)
	{
	case enumTONG_APPLY_INFO_ID_SELF:  //�Լ���ѯ�Լ��İ����Ϣ
		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)   //gs
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);
		break;

	case enumTONG_APPLY_INFO_ID_MASTER: //����
		break;

	case enumTONG_APPLY_INFO_ID_DIRECTOR: //����
		break;

	case enumTONG_APPLY_INFO_ID_MANAGER:  // �ӳ�
		if (!m_nFlag)
			break;
		/*if (this->m_nFigure != enumTONG_FIGURE_MASTER &&
			this->m_nFigure != enumTONG_FIGURE_DIRECTOR)
			break;*///ֻ���ǰ����ͳ��ϲŸ���

		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);

		break;
	case enumTONG_APPLY_INFO_ID_LIST:
		{
			sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
			if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);
		}
		break;
	case enumTONG_APPLY_INFO_ID_MEMBER:   //����
		if (!m_nFlag)
			break;
		/*if (this->m_nFigure != enumTONG_FIGURE_MASTER &&
			this->m_nFigure != enumTONG_FIGURE_DIRECTOR)
			break;*///ֻ���ǰ����ͳ��ϲŸ���

		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);

		break;

	case enumTONG_APPLY_INFO_ID_ONE:  //ĳ�˰���Ա����Ϣ
		if (!lpszName || !lpszName[0])
			break;
		if (strlen(lpszName) >= 32)
			break;
		strcpy(sInfo.m_szBuf, lpszName);
		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf) + strlen(lpszName);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);
		break;
	case enumTONG_APPLY_INFO_ID_TONG_HEAD:  //��ȡ���������ϵ���Ϣ
		if (nParam1 <= 0 || nParam1 >= MAX_NPC)
			break;
		if (nParam1 != Player[CLIENT_PLAYER_INDEX].m_nIndex && Npc[nParam1].m_nTongFlag == 0)
			break;
		sInfo.m_nParam1 = Npc[nParam1].m_dwID;
		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);
		break;
	}
	return TRUE;
}

DWORD	KPlayerTong::GetTongNameID()
{
///	return (m_nFlag ? m_dwTongNameID : 0);
	return m_dwTongNameID;
//	if (!m_nFlag)
//		return 0;
//	return g_FileName2Id(m_szName);
}

//-------------------------------------------------------------------------
//	���ܣ��Ƿ���Ȩ����ѯ�ӳ���Ϣ
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CanGetManagerInfo(DWORD dwTongNameID)
{
	if (!m_nFlag)
		return FALSE;
	if (dwTongNameID != g_FileName2Id(this->m_szName))
		return FALSE;
	//if (m_nFigure == enumTONG_FIGURE_MASTER || m_nFigure == enumTONG_FIGURE_DIRECTOR)
	//	return TRUE;
	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ��Ƿ���Ȩ����ѯ������Ϣ
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CanGetMemberInfo(DWORD dwTongNameID)
{
	if (!m_nFlag)
		return FALSE;
	if (dwTongNameID != g_FileName2Id(this->m_szName))
		return FALSE;
	//if (m_nFigure == enumTONG_FIGURE_MASTER || m_nFigure == enumTONG_FIGURE_DIRECTOR)
	//	return TRUE;
	return TRUE;
}
//�������� ͬ�����ͻ���
#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��趨��������Ϣ
//-------------------------------------------------------------------------
void	KPlayerTong::SetSelfInfo(TONG_SELF_INFO_SYNC *pInfo)
{
	if (pInfo->m_btJoinFlag == 0)
	{//û������
		if (m_nFlag)
		{//�ͻ���ԭ���� ����Ա
			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
			sUi.nData = TONG_ACTION_LEAVE;
			sUi.nParam = m_nFigure;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (uintptr_t)&sUi, 1);

			// ���߳����
			defFuncShowNormalMsg("��ʾ:�����߳����!",0);
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = camp_free;
			if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = camp_free;
		}

		Clear();

		CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);

		return;
	}

	if (m_nFlag == 1 && m_nFigure != pInfo->m_btFigure)
	{	// ������
		defFuncShowNormalMsg("��ʾ:���ڰ�����ְλ�����仯!",0);
	}

	this->m_nFlag = 1;                   //�ͻ����Ƿ����
	this->m_nFigure = pInfo->m_btFigure; //�ͻ��˰��ɵ����
	this->m_nCamp = pInfo->m_btCamp;     //�ͻ��˰��ɵ���Ӫ
	this->m_nMoney = pInfo->m_dwMoney;   //�ͻ������ɵ�Ǯ
	this->m_AttackState= pInfo->m_nAttackState;	  //������ս״̬
	this->m_nDeathCount= pInfo->m_nDeathCount;
	this->m_nLevel=pInfo->m_nLevel;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = m_nCamp;

	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag == 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = m_nCamp;

	memcpy(this->m_BMasterName, pInfo->m_szMaster, sizeof(pInfo->m_szMaster)); //���������˰���������
	memcpy(this->m_szName, pInfo->m_szTongName, sizeof(pInfo->m_szTongName));  //�����
	memcpy(this->m_szTitle, pInfo->m_szTitle, sizeof(pInfo->m_szTitle));       //����е�ְλ
	m_dwTongNameID	= g_FileName2Id(pInfo->m_szTongName);                      //���Ĵ���
    //���½�����Ϣ
	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
}
#endif

//�򿪽���UI
void    KPlayerTong::OpenCreateInterface()
{
	CoreDataChanged(GDCNI_OPEN_TONG_CREATE_SHEET, 1, 0);
}
//�ﻧ��Ǯ
BOOL KPlayerTong::ApplySaveMoney(DWORD nMoney)
{
	if (!m_nFlag)
		return FALSE;
	if (nMoney <= 0)
		return FALSE;
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetsMon(room_equipment) < nMoney)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage,"������û����ô��Ǯ��!");
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		return FALSE;
	}

	TONG_APPLY_SAVE_COMMAND	sSave;
	sSave.ProtocolType		= c2s_extendtong;
	sSave.m_wLength			= sizeof(sSave) - 1;
	sSave.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_SAVE;
	sSave.m_dwTongNameID	= Player[CLIENT_PLAYER_INDEX].m_cTong.m_dwTongNameID;//g_FileName2Id(Player[CLIENT_PLAYER_INDEX].m_cTong.m_szName);  //this->m_szName
	sSave.m_dwMoney			= nMoney;
	strcpy(sSave.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	if (g_pClient)
		g_pClient->SendPackToServer(&sSave, sSave.m_wLength + 1);

	return TRUE;
}
//���ȡǮ
BOOL KPlayerTong::ApplyGetMoney(DWORD nMoney)
{
	if (!m_nFlag)
		return FALSE;
	if (nMoney <= 0)
		return FALSE;
	if (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	if (this->m_nMoney < nMoney)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, "��ʾ:��Ǯ����!");
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
        sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		return FALSE;
	}

	TONG_APPLY_SAVE_COMMAND	sSave;
	sSave.ProtocolType		= c2s_extendtong;
	sSave.m_wLength			= sizeof(sSave) - 1;
	sSave.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_GET;
	sSave.m_dwTongNameID	=  Player[CLIENT_PLAYER_INDEX].m_cTong.m_dwTongNameID;//g_FileName2Id(this->m_szName);
	sSave.m_dwMoney			= nMoney;
	strcpy(sSave.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	if (g_pClient)
		g_pClient->SendPackToServer(&sSave, sSave.m_wLength + 1);

	return TRUE;
}

//�ͻ���
void    KPlayerTong::SetAttAckCityInfo(BYTE *Msg)
{
	  CTONG_CITY_INFO_SYNC	*pAinfo = (CTONG_CITY_INFO_SYNC*)Msg;

	  for (int i =1;i<pAinfo->m_WarCityCount+1;++i)
	  {

		  nAttAckInfo[i].m_idx = pAinfo->snWarInfo[i-1].m_idx;
		  nAttAckInfo[i].m_levle=pAinfo->snWarInfo[i-1].m_levle;
		  nAttAckInfo[i].m_mapidx=pAinfo->snWarInfo[i-1].m_mapidx;
		  nAttAckInfo[i].m_shushou=pAinfo->snWarInfo[i-1].m_shushou;
		  nAttAckInfo[i].m_Mastername = pAinfo->snWarInfo[i-1].m_Mastername;
		  nAttAckInfo[i].m_szMapName = pAinfo->snWarInfo[i-1].m_szMapName; //��ͼ��
		  nAttAckInfo[i].m_Tongmaster = pAinfo->snWarInfo[i-1].m_Tongmaster;

		/*AttAck[i].m_idx=pAinfo->snWarInfo[i].m_idx;
		AttAck[i].m_levle=pAinfo->snWarInfo[i].m_levle;
		AttAck[i].m_mapidx=pAinfo->snWarInfo[i].m_mapidx;
		AttAck[i].m_shushou=pAinfo->snWarInfo[i].m_shushou;
		sprintf(AttAck[i].m_Mastername,pAinfo->snWarInfo[i].m_Mastername);
		sprintf(AttAck[i].m_szMapName,pAinfo->snWarInfo[i].m_szMapName); //��ͼ��
		sprintf(AttAck[i].m_Tongmaster,pAinfo->snWarInfo[i].m_Tongmaster);
		*/
		// Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(AttAck[i].m_Tongmaster);
		/*if (m_nFlag && strstr(m_szName,AttAck[i].m_Tongmaster)) //��������
		   //��������ͬ,��������������еĳ�Ա
		  Npc[Player[m_nPlayerIndex].m_nIndex].m_IsWarCity=AttAck[i].m_mapidx;
		else
		  Npc[Player[m_nPlayerIndex].m_nIndex].m_IsWarCity=-1; */

	  }
}


void    KPlayerTong::SetAttAckInfo(BYTE *Msg)
{
	CTONG_ATTTACK_INFO_SYNC	*pAinfo = (CTONG_ATTTACK_INFO_SYNC*)Msg;
   	/*
	for (int i =0;i<pAinfo->m_WarCityCount;i++)
	{
		AttAck[i].m_idx=pAinfo->snWarInfo[i].m_idx;
		AttAck[i].m_levle=pAinfo->snWarInfo[i].m_levle;
		AttAck[i].m_mapidx=pAinfo->snWarInfo[i].m_mapidx;
		AttAck[i].m_shushou=pAinfo->snWarInfo[i].m_shushou;
		sprintf(AttAck[i].m_Mastername,pAinfo->snWarInfo[i].m_Mastername);
		sprintf(AttAck[i].m_szMapName,pAinfo->snWarInfo[i].m_szMapName);
		sprintf(AttAck[i].m_Tongmaster,pAinfo->snWarInfo[i].m_Tongmaster);
	}
	*/
	m_AttackState=pAinfo->m_AttackState;
	m_AttackCamp=pAinfo->m_nTempCamp;
	m_AttackTime=pAinfo->m_AttackTime;
	m_nLevel=pAinfo->m_nLevel;
	m_nDeathCount=pAinfo->m_YDeathCount;
	m_nDDeathCount=pAinfo->m_DDeathCount;
	m_Mapid= pAinfo->m_nMapidx;
	m_nAttackNum=pAinfo->m_nAttackNum;	                            // ��ս����
	m_nWinNum=pAinfo->m_nWinNum;								// ʤ������
	m_nLoseNum=pAinfo->m_nLoseNum;
	sprintf(m_AttackName, "%s", pAinfo->m_szAttackName); //��ս�İ��

	if (pAinfo->m_nMapidx>0)
	{//�ͻ��� ����ͼ������
		//sprintf(SubWorld[pAinfo->m_nMapidx].m_WarSubTongname,m_szName);
		SubWorld[pAinfo->m_nMapidx].nWorldMapInfo[STR_MAP_ZTONGNAME] = m_szName;
	}
}

//��ᷢǮ
BOOL KPlayerTong::ApplySndMoney(DWORD nMoney)
{
	if (!m_nFlag)
		return FALSE;
	if (nMoney <= 0)
		return FALSE;
	if (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	if (this->m_nMoney < nMoney)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, "��ʾ:��Ǯ����!");
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		return FALSE;
	}

	TONG_APPLY_SAVE_COMMAND	sSave;
	sSave.ProtocolType		= c2s_extendtong;
	sSave.m_wLength			= sizeof(sSave) - 1;
	sSave.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_SND;
	sSave.m_dwTongNameID	= Player[CLIENT_PLAYER_INDEX].m_cTong.m_dwTongNameID;//g_FileName2Id(this->m_szName);
	sSave.m_dwMoney			= nMoney;
	strcpy(sSave.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	if (g_pClient)
		g_pClient->SendPackToServer(&sSave, sSave.m_wLength + 1);

	return TRUE;
}
