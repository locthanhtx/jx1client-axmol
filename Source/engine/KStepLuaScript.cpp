//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KStepLuaScript.cpp
// Date:	2001-9-13 10:33:29
// Code:	Romandou
// Desc:
//---------------------------------------------------------------------------
#include "KEngine.h"
#include "KStepLuaScript.h"
#include "lualib/LuaLib.h"
#include "string.h"
#include "KScriptList.h"

#define CANCOMPILETOLUB //�Ƿ�֧��lub�ļ�Ҳ����Ϊ���������Ľ�ĳЩָ��IF ��任��GOTO�Ĺ���
//---------------------------------------------------------------------------
// ����:	KStepLuaScript::KStepLuaScript
// ����:
// ����:	void
// ����:
//---------------------------------------------------------------------------
KStepLuaScript::KStepLuaScript(void)
{
	m_CurLine			= 0;
	m_BufLen			= 0;
	m_CurPos			= 0;
	m_FirstExecuteLine	= 0;
	m_EndExecuteLine	= 0;
	m_pMsgQueue			= NULL;
	m_Status            = ssRunIdle;
	m_ScriptStyle       = false;
	strcpy(m_szWaitingMsg, "");
}

//---------------------------------------------------------------------------
// ����:	KStepLuaScript::KStepLuaScript
// ����:
// ����:	int32_t StackSize
// ����:
//---------------------------------------------------------------------------
KStepLuaScript::KStepLuaScript(int32_t StackSize)
{
	m_CurLine			= 0;
	m_BufLen			= 0;
	m_CurPos			= 0;
	m_FirstExecuteLine	= 0;
	m_EndExecuteLine	= 0;
	m_pMsgQueue			= NULL;
}


//---------------------------------------------------------------------------
// ����:	KStepLuaScript::~KStepLuaScript
// ����:
// ����:	void
// ����:
//---------------------------------------------------------------------------
KStepLuaScript::~KStepLuaScript(void)
{
	m_Memory.Free();
}

BOOL KStepLuaScript::GetNextLine(LPBYTE lpByte, char * szLine)
{
	int32_t nCurPos;
	int32_t i = 0;

	nCurPos = m_CurPos;
	while(1)
	{
		if (nCurPos >= m_BufLen)
		{
			szLine[i ++] = 13;
			szLine[i ++] = '\n';
			break;
		}

		if (lpByte[nCurPos] == '\n')
		{
			if (i == 0) {nCurPos++; continue;}
			szLine[i] = '\n';
			break;

		}

		szLine[i ++] = lpByte[nCurPos++];
	}
	if (i)
	{
		szLine[i] = '\0'; // i - 1 old
		m_CurPos = nCurPos + 1;
		return TRUE;
	}
	return FALSE;
}
//����lua�ű�
BOOL KStepLuaScript::Load(LPSTR szFileName)
{

	if (!szFileName)
		return FALSE;

	strcpy(m_szFilename,szFileName);
	SetScriptName( szFileName);

#ifdef CANCOMPILETOLUB
	KLubCmpl_Blocker blocker;
	KLineNode * pIfLine = blocker.Load(szFileName);

	KLineNode * pExitLine = new KLineNode;
	char  szExit[10];
	strcpy(szExit,"Exit();\n");
	pExitLine->m_pLineMem = new KMemClass1;
	pExitLine->m_pLineMem->Alloc(strlen(szExit));
	strcpy((char *)pExitLine->m_pLineMem->GetMemPtr(), szExit);
	blocker.m_Lines.AddTail(pExitLine);

	blocker.ScanIf(pIfLine);
	KMemClass1 * pMem = NULL;

	int32_t len = blocker.GetBuffer(pMem);
	if (len == 0) return FALSE;
	if (!GetExeBuffer(pMem->GetMemPtr(), len))	return FALSE;
	if (!KLuaScript::LoadBuffer((PBYTE)m_Memory.GetMemPtr(), len ))		return FALSE;
	if (!ExecuteCode()) return FALSE;
	delete pExitLine;
	pExitLine=NULL;
	delete pMem;
	pMem=NULL;
#else
	if (!GetExeBufferFromFile(szFileName)) 		return FALSE;
	if (!KLuaScript::LoadBuffer((PBYTE)m_Memory.GetMemPtr(), m_BufLen - 1))
		if (!ExecuteCode()) return FALSE;
#endif
	return TRUE;
}

//ִ�и������
BOOL KStepLuaScript::ExeLine(LPSTR szLine)
{
	if (szLine)
	{
	if (lua_dostring(m_LuaState, szLine) == 0)	return TRUE;
	}
	return FALSE;
}

BOOL KStepLuaScript::CheckLine(LPSTR szLine)//��齫ִ�е�Lua����Ƿ�����������粻����for goto
{
	return TRUE;
}

BOOL KStepLuaScript::GetExeBufferFromFile(char * FileName)//��õ�ǰ�ļ���ִ�жε����
{
	KPakFile	File;
	DWORD		Size;

	// open file
	if (!File.Open(FileName))
		return FALSE;

	// get file size
	Size = File.Size();

	// alloc memory
	if (! m_Memory.Alloc(Size))
	{
		File.Close();
		return FALSE;
	}


	// read file
	if (File.Read(m_Memory.GetMemPtr(), Size) != Size)
	{
		File.Close();
		return FALSE;
	}


	File.Close();

	((char*)m_Memory.GetMemPtr())[Size] = 13;
    ((char*)m_Memory.GetMemPtr())[Size + 1] = '\n';
	((char*)m_Memory.GetMemPtr())[Size + 2] = 0;

	// set buffer length
	m_BufLen = Size + 3;//m_Memory.GetMemLen();

	// set cursor position
	m_CurPos = 0;

	char szLine[100];
	int32_t nCurPos = 0;

	LPBYTE lpByte;
	lpByte = (LPBYTE) m_Memory.GetMemPtr();

	while(1)
	{
		nCurPos = m_CurPos;
		if (!GetNextLine(lpByte, szLine))
			break;

		if(strstr(szLine,MainBlockBeginString))
		{
			m_FirstExecuteLine = m_CurPos;
			break;
		}

	}

	while(1)
	{
		nCurPos = m_CurPos;
		if (!GetNextLine(lpByte, szLine))
			break;
		if(strstr(szLine,MainBlockEndString))
		{
			m_EndExecuteLine = nCurPos;
			break;
		}
	}

	if ((m_FirstExecuteLine * m_EndExecuteLine) == 0)
		return FALSE;

	return TRUE;

}
//��Buffer�л�ô���
BOOL KStepLuaScript::GetExeBuffer(void * szScriptBuffer, int32_t nLen)//���ִ�жε����
{
	char szLine[100];
	int32_t nCurPos = 0;

	LPBYTE lpByte;

	int32_t Size = nLen;
	m_BufLen = Size + 3;//m_Memory.GetMemLen();
	if (!m_Memory.Alloc(Size+16))
		return FALSE;

	g_MemCopy(m_Memory.GetMemPtr(), szScriptBuffer, nLen);

	lpByte = (LPBYTE)m_Memory.GetMemPtr();
	m_CurPos = 0;

	while(1)
	{
		nCurPos = m_CurPos;
		if (!GetNextLine(lpByte, szLine))
			break;
		if(strstr(szLine,MainBlockBeginString))
		{
			m_FirstExecuteLine = m_CurPos;
			break;
		}

	}

	while(1)
	{
		nCurPos = m_CurPos;
		if (!GetNextLine(lpByte, szLine))
			break;
		if(strstr(szLine,MainBlockEndString))
		{
			m_EndExecuteLine = nCurPos;
			break;
		}

	}

	if ((m_FirstExecuteLine * m_EndExecuteLine) == 0)
		return FALSE;

	return TRUE;

}


int32_t KStepLuaScript::Active()
{
	char szLine[100];
	int32_t nCurPos = 0;
	int32_t index = 0;

	TScriptMsg * pNode = m_pMsgQueue;

	while(pNode)
	{
		char MsgFuncName[40];
		sprintf(MsgFuncName, "On%s", (char *)pNode->szMessage);
		if (!CallFunction(MsgFuncName, 0, "ds", (uintptr_t)pNode->StateAddr,  pNode->szMsgData))
		{
		}

		if (IsRunWaitMsg())
		{
			if (!strcmp(pNode->szMessage, m_szWaitingMsg))
				RunMain();
		}

		TScriptMsg * pNode1 = pNode;
		pNode = pNode->NextMsg;
		delete pNode1;
		pNode1=NULL;
	}

	m_pMsgQueue = NULL;
	if (IsRunIdle()) return 1;
	static int32_t ii = 0;

	while(m_CurPos < m_EndExecuteLine && m_CurPos >= m_FirstExecuteLine)
	{
		nCurPos = m_CurPos;

		//�ȴ���Ϣ��....
		if (IsRunWaitMsg())	return 0;

		if (GetNextLine((LPBYTE )m_Memory.GetMemPtr(), szLine))
		{
			lua_dostring(m_LuaState, szLine);
			lua_settop(m_LuaState, 0);
		}

		//����ִ��ĳ�����ʱ��npcgo��
		if (IsRunFunc())
		{
			m_CurPos = nCurPos;
			return 0;
		}

		if (IsRunResume())
		{
			RunMain();
			return 0;
		}

		if (IsRunIdle())	return 1;

	}
	RunIdle();
	return 1 ;
}

//��һ��
void	KStepLuaScript::PosUp()
{
	int32_t nCurPos = m_CurPos - 2;

	int32_t i = 0;
	if (nCurPos <= 0)  return;
	LPBYTE lpByte = (LPBYTE )m_Memory.GetMemPtr();

	while(nCurPos > 0)
	{
		if (lpByte[nCurPos --] == '\n')
			break;
	}
	m_CurPos = nCurPos + 2;
}



void	KStepLuaScript::GotoLabel( LPSTR szLabelName)
{
	int32_t nCurPos = m_FirstExecuteLine;
	LPSTR lpByte = (LPSTR )m_Memory.GetMemPtr();
	char szLabel[50];
	sprintf(szLabel, "Label(\"%s\")",szLabelName);

	char * szindex = strstr(lpByte,szLabel);

	if (szindex == NULL)
	{
//		g_MessageBox("�ű�����: GotoLabel() = %s", szLabelName);
		return;
	}

	m_CurPos = szindex - lpByte;
	char  szLine[50];
	GetNextLine((LPBYTE)lpByte, szLine);
}

BOOL    KStepLuaScript::AddMessage(Lua_State * L, char * MessageName, char * szData)
{

	if (strlen(MessageName) == 0)
		return FALSE;

	TScriptMsg * pMsg = new TScriptMsg;
	pMsg->szMessage = MessageName ;
	pMsg->szMsgData = szData;
	pMsg->StateAddr = L;
	pMsg->NextMsg =	NULL;


	if (m_pMsgQueue == NULL)
	{
		m_pMsgQueue = pMsg;
	}
	else
	{
		TScriptMsg * pNode = m_pMsgQueue;

		while(pNode)
		{
			TScriptMsg * pNode1 = pNode;
			pNode = pNode->NextMsg;
			if (pNode == NULL)
			{
				pNode1->NextMsg = pMsg;
				break;
			}

		}

	}

	return TRUE;

}

BOOL	KStepLuaScript::SendMessage(KStepLuaScript * pSendedScript, char * szMessageName, char * szData)
{

	if (pSendedScript == NULL)
		return FALSE;
	return pSendedScript->AddMessage( m_LuaState ,szMessageName, szData);

}
