//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerMenuState.h
// Date:	2002.12.10
// Code:	�߳�����
// Desc:	PlayerMenuState Class
//---------------------------------------------------------------------------

#ifndef KPLAYERMENUSTATE_H
#define KPLAYERMENUSTATE_H

#include "GameDataDef.h"
enum
{
	PLAYER_MENU_STATE_NORMAL = 0,
	PLAYER_MENU_STATE_TEAMOPEN,  //��������
	PLAYER_MENU_STATE_TRADEOPEN, //���״�
	PLAYER_MENU_STATE_TRADING,   //������
	PLAYER_MENU_STATE_IDLE,      //˯��״̬
	PLAYER_MENU_STATE_NUM,
};

class KPlayerMenuState
{
public:
	int			m_nState;
	int			m_nTradeDest;						// �������˼ǵ��� player index �ͻ��˼ǵ��� npc id
	int			m_nTradeState;						// �Ƿ��Ѿ�����ok 0 û�� 1 ����
	char		m_szSentence[MAX_SENTENCE_LENGTH];
	int			m_nTradeDestState;

public:
	KPlayerMenuState();
	void		Release();
	void		SetState(int nState);

};

class KPlayerMenuStateGraph
{
public:
	char		m_szName[PLAYER_MENU_STATE_NUM][80];

public:
	int		Init();
	void		GetStateSpr(int nState, char *lpszGetName);
};

#endif
