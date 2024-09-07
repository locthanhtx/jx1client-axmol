//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerPK.h
// Date:	2003.07.15
// Code:	�߳�����
// Desc:	PlayerPK Class
//---------------------------------------------------------------------------

#ifndef KPLAYERPK_H
#define KPLAYERPK_H

#define		PK_ANMITY_TIME					200
#define		PK_ANMITY_CLOSE_TIME			10800

enum
{
	enumPK_ENMITY_STATE_CLOSE = 0,
	enumPK_ENMITY_STATE_TIME,
	enumPK_ENMITY_STATE_PKING,
	enumPK_ENMITY_STATE_NUM,
};

class KPlayerPK
{
	friend class KPlayer;
private:
	int		m_nNormalPKFlag;				// ����PK���� TRUE �򿪣����Կ���  FALSE �رգ������Կ���
	int		m_nEnmityPKState;				// ��ɱPK״̬ 0 �ر� 1 ���ڼ�ʱ 2 ��ɱ���ڽ��У���Ӧ enumPK_ANMITY_STATE
	int		m_nEnmityPKAim;					// ��ɱPKĿ�꣬npc id
	int		m_nEnmityPKTime;				// ��ɱPK״̬Ϊ1���ڼ�ʱ������ʱ����
	char	m_szEnmityAimName[32];			// ��ɱĿ������
	int		m_nExercisePKFlag;				// �д�ģʽPK���� TRUE �򿪣����Կ���  FALSE �رգ������Կ���
	int		m_nExercisePKAim;				// �д�ģʽPKĿ�꣬npc id
	char	m_szExerciseAimName[32];		// �д�Ŀ������
	int		m_nPKValue;						// PKֵ

public:
	void	Init();							// ��ʼ��
	void	ApplySetNormalPKState(BYTE bFlag);// �����������򿪡��ر�����PK״̬
	void	SetNormalPKState(int bFlag, int bShowMsg = TRUE);	// �趨����PK״̬
	int		GetNormalPKState();				// �������PK״̬
	void	ApplyEnmityPK(char *lpszName);	// ������������ɱĳ��
	void	ApplyEnmityPK(int nNpcID);		// ������������ɱĳ��
	void	SetEnmityPKState(int nState, int nNpcID = 0, char *lpszName = NULL);	// �趨��ɱPK״̬
	int		GetEnmityPKState()	{return m_nEnmityPKState;}		// ��ó�ɱPK״̬
	int		GetEnmityPKAimNpcID()	{return m_nEnmityPKAim;}	// ��ó�ɱPKĿ��npc id
	int		GetEnmityPKTime()	{return m_nEnmityPKTime;}		// ��ó�ɱPK׼��ʱ��
	char*	GetEnmityPKAimName()	{return m_szEnmityAimName;}	// ��ó�ɱPKĿ������
	void	EnmityPKCountDown();			// ��ɱ����ʱ
	void	SetExercisePKState(int nState, int nNpcID = 0, char *lpszName = NULL);	// �趨�д�״̬
	int		GetExercisePKState()	{return m_nExercisePKFlag;}	// ����д�״̬
	int		GetExercisePKAim()	{return m_nExercisePKAim;}		// ����д�Ŀ��npc dwid
	char*	GetExercisePKName()	{return m_szExerciseAimName;}	// ����д�Ŀ������
	void	SetPKValue(int nValue);			// �趨PKֵ
	int		GetPKValue()	{return m_nPKValue;}	// ���PKֵ
	void	Active();
};
#endif
