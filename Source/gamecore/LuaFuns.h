#ifndef LUAFUNS_H
#define LUAFUNS_H
#include "KCore.h"
#include "engine/KScriptList.h"
#include "engine/KScriptCache.h"

extern  TLua_Funcs GameScriptFuns[];
extern  int	g_GetGameScriptFunNum();

extern  TLua_Funcs WorldScriptFuns[] ;
extern  int	g_GetWorldScriptFunNum();

//�ű��б�ʵ��
//Questions:���������ű��б������������.
extern  KScriptList		g_StoryScriptList;	//���������ű��б�
#endif

