//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerPK.cpp
// Date:	2003.07.15
// Code:	�߳�����
// Desc:	PlayerPK Class
//---------------------------------------------------------------------------
#include	"KCore.h"
#include	"KPlayer.h"
#include	"KPlayerDef.h"
#include	"KPlayerPK.h"
#include "engine/Text.h"
#include "CoreShell.h"
//-------------------------------------------------------------------------
//	���ܣ���ʼ��
//-------------------------------------------------------------------------
void	KPlayerPK::Init()
{
//	m_nNormalPKFlag			= 1;
	m_nEnmityPKState		= enumPK_ENMITY_STATE_CLOSE;
	m_nEnmityPKAim			= 0;
	m_nEnmityPKTime			= 0;
	m_szEnmityAimName[0]	= 0;
	m_nExercisePKFlag		= 0;
	m_nExercisePKAim		= 0;
	m_szExerciseAimName[0]	= 0;
//	m_nPKValue				= 0;
}

void	KPlayerPK::Active()
{
	EnmityPKCountDown();
}

//-------------------------------------------------------------------------
//	���ܣ��趨����PK״̬ TRUE �򿪣����Կ���  FALSE �رգ������Կ���
//-------------------------------------------------------------------------
void	KPlayerPK::SetNormalPKState(int bFlag, BOOL bShowMsg/* = TRUE*/)
{
	if (m_nNormalPKFlag == bFlag)
		return;

	m_nNormalPKFlag = bFlag;

	//CoreDataChanged(GDCNI_PK_SETTING, 0, bFlag);

	if (bShowMsg)
	{
		KSystemMessage	sMsg;
		if (bFlag == 1)
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_PK_NORMAL_FLAG_OPEN].c_str());//KPģʽ����
		else if (bFlag == 2)
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_PK_NORMAL_FLAG_DS].c_str());  //��ɱ
		else
			sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_PK_NORMAL_FLAG_CLOSE].c_str());//pkģʽ�ر�

		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen=TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	}
}

//-------------------------------------------------------------------------
//	���ܣ������������򿪡��ر�����PK״̬
//-------------------------------------------------------------------------
void	KPlayerPK::ApplySetNormalPKState(BYTE bFlag)
{
	//SetNormalPKState(bFlag, FALSE);

	PK_APPLY_NORMAL_FLAG_COMMAND	sApply;
	sApply.ProtocolType = c2s_pkapplychangenormalflag;
	sApply.m_btFlag = bFlag;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(PK_APPLY_NORMAL_FLAG_COMMAND));

}

//-------------------------------------------------------------------------
//	���ܣ��������PK״̬ TRUE �򿪣����Կ���  FALSE �رգ������Կ���
//-------------------------------------------------------------------------
int	KPlayerPK::GetNormalPKState()
{
	return this->m_nNormalPKFlag;
}



//-------------------------------------------------------------------------
//	���ܣ�������������ɱĳ��
//-------------------------------------------------------------------------
void	KPlayerPK::ApplyEnmityPK(char *lpszName)
{
	if (!lpszName || !lpszName[0])
		return;
	int		nNpcIdx;
	nNpcIdx = NpcSet.SearchName(lpszName);
	if (nNpcIdx <= 0)
		return;
	ApplyEnmityPK(Npc[nNpcIdx].m_dwID);
}

//-------------------------------------------------------------------------
//	���ܣ�������������ɱĳ��
//-------------------------------------------------------------------------
void	KPlayerPK::ApplyEnmityPK(int nNpcID)
{
	if (m_nEnmityPKState != enumPK_ENMITY_STATE_CLOSE)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage,  "%s", strCoreInfo[MSG_PK_ERROR_4].c_str());
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		return;
	}
	if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, "%s",  strCoreInfo[MSG_PK_ERROR_1].c_str());
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		return;
	}
	int		nIdx = NpcSet.SearchID(nNpcID);
	if (nIdx == 0 || Npc[nIdx].m_Kind != kind_player)
		return;

	PK_APPLY_ENMITY_COMMAND	sApply;
	sApply.ProtocolType = c2s_pkapplyenmity;
	sApply.m_dwNpcID = nNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(PK_APPLY_ENMITY_COMMAND));
}

//-------------------------------------------------------------------------
//	���ܣ��趨��ɱPK״̬
//-------------------------------------------------------------------------
void	KPlayerPK::SetEnmityPKState(int nState, int nNpcID/* = 0*/, char *lpszName/* = NULL*/)
{
	if (nState == enumPK_ENMITY_STATE_CLOSE)
	{
		if (m_nEnmityPKState != enumPK_ENMITY_STATE_CLOSE)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, "%s",  strCoreInfo[MSG_PK_ENMITY_CLOSE].c_str());
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}

		m_nEnmityPKState = enumPK_ENMITY_STATE_CLOSE;
		m_nEnmityPKAim = 0;
		m_nEnmityPKTime = 0;
		m_szEnmityAimName[0] = 0;
	}
	else if (nState == enumPK_ENMITY_STATE_TIME)
	{
		m_nEnmityPKState = enumPK_ENMITY_STATE_CLOSE;
		m_nEnmityPKAim = nNpcID;
		m_nEnmityPKTime = PK_ANMITY_TIME;
		m_szEnmityAimName[0] = 0;
		if (lpszName)
			strcpy(m_szEnmityAimName, lpszName);

		KSystemMessage	sMsg;
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sprintf(sMsg.szMessage, strCoreInfo[MSG_PK_ENMITY_SUCCESS_1].c_str(), m_szEnmityAimName);
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		sprintf(sMsg.szMessage, "%s",  strCoreInfo[MSG_PK_ENMITY_SUCCESS_2].c_str());
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	}
	else	// if (nState == enumPK_ENMITY_STATE_PKING)
	{
		m_nEnmityPKState = enumPK_ENMITY_STATE_PKING;
		m_nEnmityPKTime = 0;
		if (nNpcID > 0)
			m_nEnmityPKAim = nNpcID;
		if (lpszName)
			strcpy(m_szEnmityAimName, lpszName);

		KSystemMessage	sMsg;
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sprintf(sMsg.szMessage, "%s",  strCoreInfo[MSG_PK_ENMITY_OPEN].c_str());
		sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	}
}

//-------------------------------------------------------------------------
//	���ܣ���ɱ����ʱ
//-------------------------------------------------------------------------
void	KPlayerPK::EnmityPKCountDown()
{
	if (m_nEnmityPKState == enumPK_ENMITY_STATE_TIME)
	{
		m_nEnmityPKTime--;
		if (m_nEnmityPKTime < 0)
			m_nEnmityPKTime = 0;
	}
}

//-------------------------------------------------------------------------
//	���ܣ��趨�д�״̬
//-------------------------------------------------------------------------
void	KPlayerPK::SetExercisePKState(int nState, int nNpcID/* = 0*/, char *lpszName/* = NULL*/)
{
	SetEnmityPKState(enumPK_ENMITY_STATE_CLOSE);

	if (nState)
	{
		m_nExercisePKFlag = 1;
		m_nExercisePKAim = nNpcID;
		m_szExerciseAimName[0] = 0;
		if (lpszName)
			strcpy(m_szExerciseAimName, lpszName);
	}
	else
	{
		m_nExercisePKFlag = 0;
		m_nExercisePKAim = 0;
		m_szExerciseAimName[0] = 0;
	}
}

//-------------------------------------------------------------------------
//	���ܣ��趨PKֵ
//-------------------------------------------------------------------------
void	KPlayerPK::SetPKValue(int nValue)
{
	if (m_nPKValue == nValue)
		return;

	this->m_nPKValue = (nValue < 0 ? 0 : nValue);
	if (m_nPKValue > MAX_DEATH_PUNISH_PK_VALUE)
		m_nPKValue = MAX_DEATH_PUNISH_PK_VALUE;

	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, strCoreInfo[MSG_PK_VALUE].c_str(), m_nPKValue);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);

}














