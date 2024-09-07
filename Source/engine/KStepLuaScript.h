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
				KStepLuaScript( int StackSize);

	int			Active();
	int		GetNextLine(LPBYTE lpByte, char * szLin);
	int		ExeLine(char* szLine);
	int		CheckLine(char* szLine);//��齫ִ�е�Lua����Ƿ�����������粻����for goto
	int		GetExeBufferFromFile(char * filename);//��õ�ǰ�ļ���ִ�жε����
	int		GetExeBuffer(void *, int len);//���ִ�жε����
	int		Load(char* szFileName);
	void		SeekBegin(){m_CurPos = 0;	};
	void		SeekToExeBegin(){m_CurPos = m_FirstExecuteLine;};

	void		RunMain(){ m_Status = ssRunMain; };
	void		RunFunc(){ m_Status = ssRunFunc; };
	void		RunIdle(){	m_Status = ssRunIdle;};
	void		RunResume(){ m_Status = ssRunResume;};
	void		RunWaitMsg(){ m_Status = ssRunWaitMsg;	};
	int		IsRunMain(){ return m_Status == ssRunMain; };
	int		IsRunFunc(){ return m_Status == ssRunFunc; };
	int		IsRunIdle(){ return m_Status == ssRunIdle;};
	int		IsRunResume(){ return m_Status == ssRunResume;	};
	int		IsRunWaitMsg(){return m_Status == ssRunWaitMsg;};

	KMemClass	m_Memory;
	int		SendMessage(KStepLuaScript * pSendedScript, char * szMessageName, char * szData);
	int		AddMessage(Lua_State * L, char * szMessageName, char * szData);
	char*		GetWaitingMsg(){return m_szWaitingMsg;};

	void		PosUp();
	void		GotoLabel( char* szLabelName);
	int			GetStatus(){return m_Status;	};

private:

	TScriptMsg  *m_pMsgQueue;
	RunStatus	m_Status;       // �ű���״̬
	int		m_ScriptStyle; //�ýű����������Ƿ�Ϊ������   ��������
	int		m_CurLine;
	int		m_BufLen;
	int		m_CurPos;
	int		m_FirstExecuteLine;
	int		m_EndExecuteLine;
	char		m_szFilename[32];
	char		m_szWaitingMsg[40];
};
//---------------------------------------------------------------------------
#endif //KStepLuaScript_H
