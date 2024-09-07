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
	virtual int Init();
	virtual void Exit();
	virtual int Load(char* FileName);
	//virtual int Load(char* FileName,TLua_Funcs Funcs[]=0, int n = 0);
	virtual	int Compile(char* FileName);
	virtual	int Execute();
	virtual	int CallFunction( char * cFuncName, int nResults,  char * cFormat,...);
	virtual int RegisterFunction( char * FuncName, void* Func);

};
//---------------------------------------------------------------------------
#endif  //KScript_H
