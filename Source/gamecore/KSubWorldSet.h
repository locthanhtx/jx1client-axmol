#ifndef	KSubWorldSetH
#define	KSubWorldSetH

#include "engine/KTimer.h"
#include "KMapMusic.h"
/*
#ifndef _SERVER
#include "KMapMusic.h"
#else

#include "KSubWorld.h"

#endif
*/
#define  MAX_TASK_COUNT 50

class  KSubWorldSet
{
public:
	unsigned long  m_nLoopRate;		// ѭ��֡��
	//KMapMusic m_cMusic;
private:
	int		m_nGameVersion;
	unsigned long	m_dwPing;
public:
	KSubWorldSet();
	//int	LoadSerMaps(char* szFileName);
	int		SearchWorld(unsigned long dwID);
	int     SearchWorldRows(int dwID);
	void	MainLoop();
	void	MessageLoop();
	//int  	TaskActivate(); //�����Զ�����ϵͳ
	int 	TaskMainLoop();
	int	SendMessage(int nSubWorldID, unsigned long	dwMsgType, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0);
	unsigned long	GetGameTime(){return m_nLoopRate;};
	int		GetGameVersion() { return m_nGameVersion; }
	void	Close();
	int	SubExecuteScriptA(char * ScriptFileName, char * szFunName, int nParam = 0,char * szCanshu=NULL);
	int	SubExecuteScript(char * ScriptFileName, char * szFunName, char * szParams=NULL,char * snParams=NULL,int inParam = 0,int njb=0,int njxb=0);
	int	SubExecuteScript(unsigned long dwScriptId, char * szFunName, char *  szParams=NULL,char * snParams=NULL,int inParam = 0,int njb=0,int njxb=0);
	int	SubExecuteScriptB(char* dwScriptIdA,  char * szFunName, int nParam=0,char * szCanshu=NULL);
// Client�ϲ��еļ�������
	void	SetPing(unsigned long dwTimer) { m_dwPing = dwTimer; }
	unsigned long	GetPing() { return m_dwPing; }
	unsigned int getCilentRunTime();
	//void	Paint();
};
extern  KSubWorldSet g_SubWorldSet;
#endif
