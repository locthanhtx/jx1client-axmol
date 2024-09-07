//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerTeam.cpp
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Team Class
//---------------------------------------------------------------------------

#include	"KCore.h"
#include	"KNpc.h"
//#ifdef _SERVER
//#include	"KNetServer.h"
//#include "../../Headers/IServer.h"
//#else
//#include	"KNetClient.h"
//#include "../../Headers/IClient.h"
#include	"CoreShell.h"

#include	"KPlayer.h"
#include	"KPlayerSet.h"
#include	"KPlayerTeam.h"
//#include	"MyAssert.h"
#include "engine/Text.h"

enum
{
	Team_S_Close = 0,
	Team_S_Open,
};

KTeam		g_Team[MAX_TEAM];
KPlayerTeam::KPlayerTeam()
{
	Release();
}

void	KPlayerTeam::Release()
{
	m_nFlag = 0;
	m_nFigure = TEAM_CAPTAIN;
	m_nApplyCaptainID = -1;
	m_nApplyCaptainID = 0;
	m_dwApplyTimer = 0;
	m_bAutoRefuseInviteFlag = FALSE;	 // TRUE �Զ��ܾ�   FALSE �ֶ�
	ReleaseList();
}

void	KPlayerTeam::ReleaseList()
{
	for (int i = 0; i < MAX_TEAM_APPLY_LIST; ++i)
		m_sApplyList[i].Release();
}


int	KPlayerTeam::ApplyCreate()//char *lpszTeamName)
{
//	if (!lpszTeamName || !lpszTeamName[0])
//		return FALSE;
//	if (strlen(lpszTeamName) >= 32)
//		return FALSE;
	if (m_nFlag)
		return FALSE;

	PLAYER_APPLY_CREATE_TEAM	sCreateTeam;
	sCreateTeam.ProtocolType = c2s_teamapplycreate;
//	memset(sCreateTeam.m_szTeamName, 0, sizeof(sCreateTeam.m_szTeamName));
//	strcpy(sCreateTeam.m_szTeamName, lpszTeamName);
	if (g_pClient)
		g_pClient->SendPackToServer(&sCreateTeam, sizeof(PLAYER_APPLY_CREATE_TEAM));

	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ�����������
//---------------------------------------------------------------------------
void	KPlayerTeam::InviteAdd(unsigned long dwNpcID)
{
	if (!this->m_nFlag || this->m_nFigure != TEAM_CAPTAIN || g_Team[0].m_nState != Team_S_Open)
		return;
	TEAM_INVITE_ADD_COMMAND	sAdd;
	sAdd.ProtocolType = c2s_teaminviteadd;
	sAdd.m_dwNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAdd, sizeof(TEAM_INVITE_ADD_COMMAND));
}

//---------------------------------------------------------------------------
//	���ܣ��յ�����
//---------------------------------------------------------------------------
void	KPlayerTeam::ReceiveInvite(TEAM_INVITE_ADD_SYNC *pInvite)
{
	if (!pInvite || pInvite==NULL)
		return;
	char	szName[32]={0};
	int		nIdx;
	memset(szName, 0, sizeof(szName));
	memcpy(szName, pInvite->m_szName, sizeof(pInvite->m_szName) - (sizeof(TEAM_INVITE_ADD_SYNC) - pInvite->m_wLength - 1));
	nIdx = pInvite->m_nIdx;

	if (m_bAutoRefuseInviteFlag)
	{
		ReplyInvite(nIdx, 0); //�ܾ�
		//if (uParam)
		//	Player[CLIENT_PLAYER_INDEX].m_cTeam.ReplyInvite(((KUiPlayerItem*)uParam)->nIndex, nParam);
	}
	else
	{
		// ֪ͨ��������������Ҽ���ĳ������
		KUiPlayerItem	sPlayer;
		KSystemMessage	sMsg;

		strcpy(sPlayer.Name, szName);
		sPlayer.nIndex = pInvite->m_nIdx;
		sPlayer.uId = 0;
		sPlayer.nData = 0;
		sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_GET_INVITE].c_str(), szName);
		sMsg.eType         = SMT_TEAM;
		sMsg.byConfirmType = SMCT_UI_TEAM_INVITE;
		sMsg.byPriority    = 3;
		sMsg.byParamSize   = sizeof(KUiPlayerItem);
		sMsg.nMsgLen       = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, (uintptr_t)&sPlayer);
	}
}

//---------------------------------------------------------------------------
//	���ܣ��ظ�����
//	������nResult  if == 0 �ܾ�  if == 1 ����
//---------------------------------------------------------------------------
void	KPlayerTeam::ReplyInvite(int nIdx, int nResult)
{

	if (nIdx < 0 || nResult < 0 || nResult > 1)
		return;
	TEAM_REPLY_INVITE_COMMAND	sReply;
	sReply.ProtocolType = c2s_teamreplyinvite;
	sReply.m_nIndex     = nIdx;
	sReply.m_btResult   = nResult;
	if (g_pClient)
		g_pClient->SendPackToServer(&sReply, sizeof(TEAM_REPLY_INVITE_COMMAND));
}

//---------------------------------------------------------------------------
//	���ܣ��趨�Ƿ��Զ��ܾ����˵ļ�����������
//---------------------------------------------------------------------------
void	KPlayerTeam::SetAutoRefuseInvite(int bFlag)
{
	KSystemMessage	sMsg;
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;

	if (bFlag)
	{
		m_bAutoRefuseInviteFlag = TRUE;
		//sprintf(sMsg.szMessage, strCoreInfo[MSG_TEAM_AUTO_REFUSE_INVITE].c_str());
	}
	else
	{
		m_bAutoRefuseInviteFlag = FALSE;
		//sprintf(sMsg.szMessage,  strCoreInfo[MSG_TEAM_NOT_AUTO_REFUSE_INVITE].c_str());
	}
    //sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
	//CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
}

//---------------------------------------------------------------------------
//	���ܣ�����Ƿ��Զ��ܾ����˵ļ�����������״̬
//---------------------------------------------------------------------------
int	KPlayerTeam::GetAutoRefuseState()
{
	return m_bAutoRefuseInviteFlag;
}

//---------------------------------------------------------------------------
//	���ܣ�������������Ϣ�������棩
//---------------------------------------------------------------------------
int		KPlayerTeam::GetInfo(KUiPlayerTeam *pTeam)
{
	if (!pTeam)
		return 0;
	pTeam->nCaptainPower = g_Team[0].CalcCaptainPower(); //�ӳ����������

	if (m_nFlag == 0)
		return 0;

	if (m_nFigure == TEAM_CAPTAIN)
	{
		pTeam->bTeamLeader = true;
		int nNo = 0;
		for (int i = 0; i < MAX_TEAM_APPLY_LIST; ++i)
		{
			if (this->m_sApplyList[i].m_dwNpcID > 0)
				nNo++;
		}
		pTeam->cNumTojoin = nNo;
	}
	else
	{
		pTeam->bTeamLeader = false;
		pTeam->cNumTojoin = 0;
	}
	pTeam->nTeamServerID = g_Team[0].m_nTeamServerID;
	pTeam->cNumMember = g_Team[0].m_nMemNum + 1;
	pTeam->bOpened = 0;
	if (g_Team[0].m_nState == Team_S_Open)
		pTeam->bOpened = 1;

	return 1;
}

//---------------------------------------------------------------------------
//	���ܣ����½�����ʾ
//---------------------------------------------------------------------------
void	KPlayerTeam::UpdateInterface()
{
	KUiPlayerTeam	sTeam;
	if (GetInfo(&sTeam) == 0)
	{
		CoreDataChanged(GDCNI_TEAM,NULL,0);
	}
	else
	{
		CoreDataChanged(GDCNI_TEAM, (uintptr_t)&sTeam,0);
	}
}

//---------------------------------------------------------------------------
//	���ܣ�ѭ�����µĶ��� ����¸��½�����ʾ
//---------------------------------------------------------------------------
void	KPlayerTeam::UpdateamUI()
{
	KUiPlayerTeam	sTeam;
	if (GetInfo(&sTeam) == 0)
	{
		//CoreDataChanged(GDCNI_TEAM_UI,NULL,0);  //���¶���UI��Ϣ
	}
	else
	{
		//CoreDataChanged(GDCNI_TEAM_UI, (uintptr_t)&sTeam,0);
	}
}

//---------------------------------------------------------------------------
//	���ܣ����������б���ɾ��ĳ��������
//---------------------------------------------------------------------------
void	KPlayerTeam::DeleteOneFromApplyList(unsigned long dwNpcID)
{
	for (int i = 0; i < MAX_TEAM_APPLY_LIST; ++i)
	{
		if (m_sApplyList[i].m_dwNpcID == dwNpcID)
		{
			m_sApplyList[i].Release();
			return;
		}
	}
}

//---------------------------------------------------------------------------
//	���ܣ����캯��
//---------------------------------------------------------------------------
KTeam::KTeam()
{
	Release();
}

//---------------------------------------------------------------------------
//	���ܣ����
//---------------------------------------------------------------------------
void	KTeam::Release()
{
	m_nCaptain = -1;
	m_nMemNum = 0;
	int		i;
	for (i = 0; i < MAX_TEAM_MEMBER; ++i)
	{
		m_nMember[i] = -1;
	}
	m_nState = Team_S_Close;
	for (i = 0; i < MAX_TEAM_MEMBER + 1; ++i)
	{
		m_nMemLevel[i] = 0;
		memset(m_szMemName, 0, 32 * (MAX_TEAM_MEMBER + 1));
	}
	m_nTeamServerID = -1;
}

//---------------------------------------------------------------------------
//	���ܣ��趨 Team �� g_Team �е�λ��
//---------------------------------------------------------------------------
void	KTeam::SetIndex(int nIndex)
{
	m_nIndex = nIndex;
	Release();
}

//---------------------------------------------------------------------------
//	���ܣ�Ѱ�Ҷ�Ա��λ
//---------------------------------------------------------------------------
int		KTeam::FindFree()
{
	for (int i = 0; i < MAX_TEAM_MEMBER; ++i)
	{
		if (m_nMember[i] < 0)
			return i;
	}
	return -1;
}

//---------------------------------------------------------------------------
//	���ܣ�Ѱ�Ҿ���ָ��npc id�Ķ�Ա���������ӳ���
//	����ֵ����Ա�� m_nMember �����е�λ��
//---------------------------------------------------------------------------
int		KTeam::FindMemberID(unsigned long dwNpcID)
{
	for (int i = 0; i <	MAX_TEAM_MEMBER; ++i)
	{
		if (m_nMember[i] > 0 && (unsigned long)m_nMember[i] == dwNpcID)
			return i;
	}
	return -1;
}
//---------------------------------------------------------------------------
//	���ܣ��趨����״̬���򿪣���������³�Ա��
//---------------------------------------------------------------------------
int	KTeam::SetTeamOpen()
{
	m_nState = Team_S_Open;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
	//Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateamUI(); //��ʾ�����������Ϣ
	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ��趨����״̬���رգ�����������³�Ա��
//---------------------------------------------------------------------------
int	KTeam::SetTeamClose()
{
	m_nState = Team_S_Close;
	Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ�����ӳ���ͳ˧��Ա������
//---------------------------------------------------------------------------
int	KTeam::CalcCaptainPower()
{
	return PlayerSet.m_cLeadExp.GetMemNumFromLevel(Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel);
}

//---------------------------------------------------------------------------
//	���ܣ��ͻ��˴���һ֧���飨�ͻ���ֻ���ܴ���һ֧�Ķ��飬���ڱ�����ң�
//---------------------------------------------------------------------------
void	KTeam::CreateTeam(int nCaptainNpcID, char *lpszCaptainName, int nCaptainLevel, unsigned long nTeamServerID)
{
	Release();
	m_nCaptain = nCaptainNpcID;
	m_nMemLevel[0] = nCaptainLevel;
	strcpy(m_szMemName[0], lpszCaptainName);
	m_nTeamServerID = nTeamServerID;		// �����ڷ������ϵ�Ψһ��ʶ
}

//---------------------------------------------------------------------------
//	���ܣ����һ�������Ա
//---------------------------------------------------------------------------
int	KTeam::AddMember(unsigned long dwNpcID, int nLevel, char *lpszNpcName)
{
	for (int i = 0; i < MAX_TEAM_MEMBER;++i)
	{
		if (m_nMember[i] == (int)dwNpcID)
			return TRUE;
	}

	int nFreeNo;
	nFreeNo = FindFree();
	if (nFreeNo < 0)
		return FALSE;
	m_nMember[nFreeNo] = dwNpcID;
	m_nMemLevel[nFreeNo + 1] = nLevel;
	strcpy(m_szMemName[nFreeNo + 1], lpszNpcName);
	m_nMemNum++;

	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ��ͻ���ɾ��һ�������Ա
//---------------------------------------------------------------------------
void	KTeam::DeleteMember(unsigned long dwNpcID)
{
	if (dwNpcID == (unsigned long)m_nCaptain)
	{
		Release();
		return;
	}

	for (int i = 0; i < MAX_TEAM_MEMBER; ++i)
	{
		if ((unsigned long)m_nMember[i] == dwNpcID)
		{
			m_nMemNum--;
			m_nMember[i] = -1;
			m_nMemLevel[i + 1] = 0;
			m_szMemName[i + 1][0] = 0;
			return;
		}
	}
}

//---------------------------------------------------------------------------
//	���ܣ������Ա��Ϣ(�������Ҫ�󣬸�����)
//---------------------------------------------------------------------------
int		KTeam::GetMemberInfo(KUiPlayerItem *pList, int nCount)
{
	if (this->m_nCaptain < 0)
		return 0;
	int		i, nNum = 0;
	if (!pList || nCount <= m_nMemNum)
	{
		return m_nMemNum + 1;
	}

	// �ӳ�����
	strcpy(pList[nNum].Name, m_szMemName[0]);
	pList[nNum].uId = this->m_nCaptain;
	pList[nNum].nIndex = 0;
	nNum++;

	for (i = 0; i < MAX_TEAM_MEMBER; ++i)
	{
		if (nNum >= nCount)
			break;
		if (this->m_nMember[i] <= 0)
			continue;
		strcpy(pList[nNum].Name, m_szMemName[i + 1]);
		pList[nNum].uId = m_nMember[i];
		pList[nNum].nIndex = 0;
		nNum++;
	}

	return nNum;
}
/*
#ifdef _SERVER
//---------------------------------------------------------------------------
//	���ܣ��ж϶����Ƿ����
//---------------------------------------------------------------------------
int	KTeamSet::CheckName(char *lpszName)
{
	if (!lpszName || !lpszName[0])
		return FALSE;

	for (int i = 0; i < MAX_TEAM; i++)
	{
		if (g_Team[i].m_nCaptain < 0)
			continue;
		if (strcmp(lpszName, g_Team[i].m_szName) == 0)
			return FALSE;
	}

	return TRUE;
}
#endif
*/
