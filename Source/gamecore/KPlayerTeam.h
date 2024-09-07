//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerTeam.h
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Team Class
//---------------------------------------------------------------------------

#ifndef KPLAYERTEAM_H
#define KPLAYERTEAM_H

#include	"KPlayerDef.h"

#define		MAX_TEAM  MAX_PLAYER //

class KTeamApplyList	// �������������б��е���������Ϣ
{
public:
	unsigned long	m_dwNpcID;			// ������ npc id
	unsigned long	m_dwTimer;			// ����ʱ�������
	int		m_nLevel;			// �����˵ȼ�
	char	m_szName[32];		// ����������
public:
	KTeamApplyList() {Release();};
	void	Release() {m_dwNpcID = 0; m_dwTimer = 0; m_nLevel = 0; m_szName[0] = 0;};
};				// �ͻ��˱����ڶӳ� player ���ϵĶ����������б���Ϣ

class KPlayerTeam	// �ͻ�����ҵ������Ϣ
{
public:
	int		m_nFlag;			// ��־�Ƿ��Ѿ����
	int		m_nFigure;			// ����Ѿ���ӣ�player �����ݣ�TEAM_CAPTAIN TEAM_MEMBER

	int		m_nApplyCaptainID;	// ��������Ŀ�����Ķӳ��� npc id
	unsigned long	m_dwApplyTimer;		// ����ʱ����������������ˣ�����ʱ��ȡ�����룩
	int	m_bAutoRefuseInviteFlag;// �Ƿ��Զ��ܾ����˵ļ����������� TRUE �Զ��ܾ�   FALSE �ֶ�
	KTeamApplyList	m_sApplyList[MAX_TEAM_APPLY_LIST];	// ���Ϊ�ӳ���������������б�
public:
	KPlayerTeam();
	void	Release();
	void	ReleaseList();
	int	ApplyCreate();//char *lpszTeamName);			// ���봴��һ֧����
	void	InviteAdd(unsigned long dwNpcID);						// ����������
	void	ReceiveInvite(TEAM_INVITE_ADD_SYNC *pInvite);	// �յ�����
	void	ReplyInvite(int nIdx, int nResult);				// �ظ�����
	void	SetAutoRefuseInvite(int bFlag);				// �趨�Ƿ��Զ��ܾ����˵ļ�����������
	int	GetAutoRefuseState();							// ����Ƿ��Զ��ܾ����˵ļ�����������״̬
	int		GetInfo(KUiPlayerTeam *pTeam);					// �������������Ϣ�������棩
	void	UpdateInterface();								// ���½�����ʾ
	void    UpdateamUI();
	void	DeleteOneFromApplyList(unsigned long dwNpcID);			// ���������б���ɾ��ĳ��������
};
// �˶����Ƿ�Ϊ��ͨ���ж϶ӳ� id ����������Ϊ -1 ʱΪ�գ�ÿ������ض���һ���ӳ���
class KTeam
{
private:
	int		m_nIndex;									// �� Team �� g_Team �е�λ��
public:
	int		m_nState;									// ����״̬��Team_S_Open Team_S_Close
	int		m_nCaptain;									// �ӳ� id ������������ player index ���ͻ����� npc dwid ��-1 Ϊ��
	int		m_nMember[MAX_TEAM_MEMBER];					// ���ж�Ա id ������������ player index ���ͻ����� npc dwid ��-1 Ϊ��
	int		m_nMemNum;									// ���ж�Ա����(�������ӳ�)
	int		m_nMemLevel[MAX_TEAM_MEMBER + 1];			// ��Ա�ȼ� (��һ����ԱΪ�ӳ�) (��Ա������ʱ��Ҫ֪ͨ����������Ա not end)
	char	m_szMemName[MAX_TEAM_MEMBER + 1][32];		// ��Ա���� (��һ����ԱΪ�ӳ�)
	unsigned long	m_nTeamServerID;							// �����ڷ������ϵ�Ψһ��ʶ

public:
	KTeam();											// ���캯��
	void	Release();									// ���
	void	SetIndex(int nIndex);						// �趨 Team �� g_Team �е�λ��
	int	SetTeamOpen();								// �趨����״̬���򿪣����������³�Ա��
	int	SetTeamClose();								// �趨����״̬���رգ������������³�Ա��
	int		CalcCaptainPower();							// ����ӳ���ͳ˧��Ա������
	int		FindFree();									// Ѱ�Ҷ�Ա��λ
	int		FindMemberID(unsigned long dwNpcID);				// Ѱ�Ҿ���ָ��npc id�Ķ�Ա���������ӳ���

	// �ͻ��˴���һ֧���飨�ͻ���ֻ���ܴ���һ֧���飬���ڱ�����ң�
	void	CreateTeam(int nCaptainNpcID, char *lpszCaptainName, int nCaptainLevel, unsigned long nTeamServerID);
	int	AddMember(unsigned long dwNpcID, int nLevel, char *lpszNpcName);// ����һ�������Ա
	void	DeleteMember(unsigned long dwNpcID);				// �ͻ���ɾ��һ�������Ա
	int		GetMemberInfo(KUiPlayerItem *pList, int nCount);	// �����Ա��Ϣ(�������Ҫ�󣬸�����)
};

extern	KTeam	g_Team[MAX_TEAM];
#endif
