//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerTrade.cpp
// Date:	2003.02.17
// Code:	�߳�����
// Desc:	Trade Class
//---------------------------------------------------------------------------

#include	"KCore.h"
#include	"KNpc.h"
#include	"KPlayer.h"
#include	"KPlayerSet.h"
#include	"KPlayerTrade.h"
#include    "engine/Text.h"
//#include	"MyAssert.h"
#include	"CoreShell.h"
KTrade::KTrade()
{
	Release();
}

void	KTrade::Release()
{
	m_nIsTrading			= 0;
	m_nTradeDest			= -1;
	m_nTradeState			= 0;
	m_nTradeLock			= 0;
	m_nBackEquipMoney		= 0;
	m_nBackRepositoryMoney	= 0;
	m_nTradeDestState		= 0;
	m_nTradeDestLock		= 0;
	m_szDestName[0]			= 0;
}

int	KTrade::StartTrade(int nDest, int nEquipMoney, int nRepositoryMoney)
{
	if (m_nIsTrading)
		return FALSE;
	if (nDest < 0)
		return FALSE;

	m_nIsTrading = 1;
	m_nTradeDest = nDest;
	m_nTradeState = 0;
	m_nTradeLock = 0;
	m_nBackEquipMoney = nEquipMoney;
	m_nBackRepositoryMoney = nRepositoryMoney;
	m_nTradeDestState = 0;
	m_nTradeDestLock = 0;
	int nIdx = NpcSet.SearchID(nDest);
	if (nIdx > 0)
		strcpy(m_szDestName, Npc[nIdx].Name);
	else
		m_szDestName[0] = 0;
	return TRUE;
}

void	KTrade::Lock(int bFlag)
{
	if (bFlag)
	{
		m_nTradeLock = 1;
	}
	else
	{
		m_nTradeLock = 0;
		m_nTradeDestLock = 0;
	}
}

void	KTrade::SetTradeState(int nSelfLock, int nDestLock, int nSelfStateOk, int nDestStateOk)
{
	if (nSelfLock)
	{
		if (m_nTradeLock == 0)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, "%s", strCoreInfo[MSG_TRADE_SELF_LOCK].c_str());
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
	}
	else
	{
		if (m_nTradeLock != 0)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, "%s", strCoreInfo[MSG_TRADE_SELF_UNLOCK].c_str());
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
	}

	if (nDestLock)
	{
		if (m_nTradeDestLock == 0)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, strCoreInfo[MSG_TRADE_DEST_LOCK].c_str(), m_szDestName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
	}
	else
	{
		if (m_nTradeDestLock != 0)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, strCoreInfo[MSG_TRADE_DEST_UNLOCK].c_str(), m_szDestName);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sMsg.nMsgLen = TEncodeText_(sMsg.szMessage, strlen(sMsg.szMessage));
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
		}
	}

	this->m_nTradeLock = (nSelfLock != 0);
	this->m_nTradeDestLock = (nDestLock != 0);
	this->m_nTradeState = (nSelfStateOk != 0);
	this->m_nTradeDestState = (nDestStateOk != 0);

	// ֪ͨ���水ť״̬�ı�
	CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
}

void	KTrade::ReplyInvite(int nDestIdx, int nReply)
{
	if (nDestIdx < 0)
		return;
	TRADE_REPLY_START_COMMAND	sReply;
	sReply.ProtocolType = c2s_tradereplystart;
	sReply.m_bDecision  = (nReply != 0);
	sReply.m_nDestIdx   = nDestIdx;
	if (g_pClient)
		g_pClient->SendPackToServer(&sReply, sizeof(TRADE_REPLY_START_COMMAND));
}
