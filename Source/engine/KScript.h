//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
// 
// File:	KScript.h
// Date:	2001-9-11 10:33:35
// Code:	Romandou,Daphnis
// Desc:	�ű�����ӿ�
//---------------------------------------------------------------------------
#ifndef KScript_H
#define KScript_H
//---------------------------------------------------------------------------
#include "KbugInfo.h"

class  KScript
{
public:
	KScript();
	virtual ~KScript();
	virtual BOOL Init();
	virtual void Exit();
	virtual BOOL Load(char* FileName);
	//virtual BOOL Load(char* FileName,TLua_Funcs Funcs[]=0, int32_t n = 0);
	virtual	BOOL Compile(char* FileName);
	virtual	BOOL Execute();
	virtual	BOOL CallFunction( char * cFuncName, int32_t nResults,  char * cFormat,...);
	virtual BOOL RegisterFunction( char * FuncName, void* Func); 
	
};
//---------------------------------------------------------------------------
#endif  //KScript_H
