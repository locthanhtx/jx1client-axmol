#ifndef KSCRIPTLIST_H
#define KSCRIPTLIST_H
#include "KEngine.h"
#include "KLuaScript.h"
#include "KList.h"
#include "KStepLuaScript.h"

class  KScriptNode : public KNode
{
public:
	 KLuaScript * pScript;
	unsigned long			 nId;
	~KScriptNode()
	{
		delete pScript;
		pScript=NULL;
	}
};

class  KScriptList:public  KList
{
public:
	KLuaScript * GetScript(Lua_State * L);
	void Activate();
	void SendMessage(char * szMessageName, char * szData);
};
#endif
