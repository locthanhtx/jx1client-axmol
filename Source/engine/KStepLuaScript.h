//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
// 
// File:	KStepLuaScript.h
// Date:	2001-9-11 11:11:09
// Code:	Romandou
// Desc:	
//---------------------------------------------------------------------------
#ifndef KStepLuaScript_H
#define KStepLuaScript_H
//---------------------------------------------------------------------------
#include "KScript.h"
#include "KLuaScript.h"
#include "KMemClass.h"
//#include "KMemClass1.h"
#include "KLubCmpl_Blocker.h"

#define MainBlockBeginString		"function main()"
#define MainBlockEndString			"end;--main"

struct TScriptMsg
{
	char * szMessage;
	char * szMsgData;
	Lua_State* StateAddr;
	TScriptMsg * NextMsg;
	
};

typedef enum
{
	ssRunIdle,	// ��δ���ؽű�
	ssRunMain,	// �ű�����ִ��
	ssRunFunc,	// ��������ִ��
	ssRunResume, //��ʱ����
	ssRunWaitMsg, //�ȴ�ĳ����Ϣ����
}
RunStatus;


//---------------------------------------------------------------------------
class  KStepLuaScript : public KLuaScript
{

public:
				KStepLuaScript();
	virtual		~KStepLuaScript();
				KStepLuaScript( int32_t StackSize);
	
	int32_t			Active();
	BOOL		GetNextLine(LPBYTE lpByte, char * szLin);
	BOOL		ExeLine(LPSTR szLine);
	BOOL		CheckLine(LPSTR szLine);//��齫ִ�е�Lua����Ƿ�����������粻����for goto 
	BOOL		GetExeBufferFromFile(char * filename);//��õ�ǰ�ļ���ִ�жε����
	BOOL		GetExeBuffer(void *, int32_t len);//���ִ�жε����
	BOOL		Load(LPSTR szFileName);
	void		SeekBegin(){m_CurPos = 0;	};
	void		SeekToExeBegin(){m_CurPos = m_FirstExecuteLine;};

	void		RunMain(){ m_Status = ssRunMain; };
	void		RunFunc(){ m_Status = ssRunFunc; };
	void		RunIdle(){	m_Status = ssRunIdle;};
	void		RunResume(){ m_Status = ssRunResume;};
	void		RunWaitMsg(){ m_Status = ssRunWaitMsg;	};
	BOOL		IsRunMain(){ return m_Status == ssRunMain; };
	BOOL		IsRunFunc(){ return m_Status == ssRunFunc; };
	BOOL		IsRunIdle(){ return m_Status == ssRunIdle;};
	BOOL		IsRunResume(){ return m_Status == ssRunResume;	};
	BOOL		IsRunWaitMsg(){return m_Status == ssRunWaitMsg;};
	
	KMemClass	m_Memory;
	BOOL		SendMessage(KStepLuaScript * pSendedScript, char * szMessageName, char * szData);
	BOOL		AddMessage(Lua_State * L, char * szMessageName, char * szData);
	LPSTR		GetWaitingMsg(){return m_szWaitingMsg;};
	
	void		PosUp();
	void		GotoLabel( LPSTR szLabelName);
	int32_t			GetStatus(){return m_Status;	};

private:

	TScriptMsg  *m_pMsgQueue;
	RunStatus	m_Status;       // �ű���״̬
	BOOL		m_ScriptStyle; //�ýű����������Ƿ�Ϊ������   ��������
	int32_t		m_CurLine;
	int32_t		m_BufLen;
	int32_t		m_CurPos;
	int32_t		m_FirstExecuteLine;
	int32_t		m_EndExecuteLine;
	char		m_szFilename[32];
	char		m_szWaitingMsg[40];
};
//---------------------------------------------------------------------------
#endif //KStepLuaScript_H
