//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerTrade.h
// Date:	2003.02.17
// Code:	�߳�����
// Desc:	Trade Class
//---------------------------------------------------------------------------

#ifndef KPLAYERTRADE_H
#define KPLAYERTRADE_H

#include	"KPlayerDef.h"

class KTrade
{
	friend class KPlayer;
public:
	int			m_nIsTrading;						// �Ƿ����ڽ��� 0 û�� 1 ���ڽ���
	int			m_nTradeDest;						// �������˼ǵ��� player index �ͻ��˼ǵ��� npc id
	int			m_nTradeState;						// �Ƿ��Ѿ�����ok 0 û�� 1 ����
	int			m_nTradeLock;						// �Ƿ����� 0 û�� 1 �Ѿ�����

	int			m_nBackEquipMoney;					// ���׿�ʼʱ������Ʒ��money
	int			m_nBackRepositoryMoney;				// ���׿�ʼʱ���ݴ�����money
	int			m_nTradeDestState;					// �ͻ��˼�¼�Է��Ƿ�ok
	int			m_nTradeDestLock;					// �ͻ��˼�¼�Է��Ƿ�����
	char		m_szDestName[32];					// �Է�����
public:
	KTrade();
	void		Release();
	int		StartTrade(int nDest, int nEquipMoney = 0, int nRepositoryMoney = 0);
	void		Lock(int bFlag);

	void		SetTradeState(int nSelfLock, int nDestLock, int nSelfStateOk, int nDestStateOk);
	static void		ReplyInvite(int nDestIdx, int nReply);

};
#endif
