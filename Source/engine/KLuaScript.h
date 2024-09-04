//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
// 
// File:	KLuaScript.h
// Date:	2001-9-11 11:11:09
// Code:	Romandou
// Desc:	
//---------------------------------------------------------------------------
#ifndef KLuaScript_H
#define KLuaScript_H
//---------------------------------------------------------------------------
#include "KScript.h"
#include "lualib/LuaLib.h"
#include <string.h>

#define MAINFUNCTIONNAME "main"
#define LUA_CREATE_ERROR 1			 //��������
#define LUA_SCRIPT_LEN_ERROR 2		 //��С����
#define LUA_SCRIPT_COMPILE_ERROR 3	 //�������
#define LUA_SCRIPT_EXECUTE_ERROR 4	 //ִ�г���
#define LUA_SCRIPT_NOT_NUMBER_ERROR 10	//�������ֳ���
#define LUA_SCRIPT_NOT_STRING_ERROR 11	//�����ַ���
#define LUA_SCRIPT_NOT_TABLE_ERROR 12	//���Ǳ��
#define LUA_SCRIPT_STATES_IS_NULL 20	//�ű�Ϊ��

typedef struct  {
	const char *name;
	lua_CFunction func;
	
}TLua_Funcs;

//---------------------------------------------------------------------------
class  KLuaScript : public KScript
{
public:
	/*
	KLuaScript();
	virtual ~KLuaScript();
	KLuaScript(int32_t StackSize);*/
	KLuaScript(void);
	KLuaScript(int32_t StackSize);
	virtual ~KLuaScript();
	
	int32_t		Activate(){Execute(); return 1;};

	
	virtual BOOL Init();
	virtual void Exit();
	virtual BOOL Load(char* FileName);
	virtual	BOOL Compile(char* FileName);
	virtual	BOOL Execute();
	virtual	BOOL CallFunction(LPSTR cFuncName, int32_t nResults, LPSTR cFormat, ...);
	virtual BOOL RegisterFunction(LPSTR FuncName, void* Func);
	virtual void SetScriptName(LPSTR scpname){strcpy(m_szScriptName, scpname);};

	int32_t	 GetUserTag() { return m_UserTag; };
	BOOL RegisterFunctions( TLua_Funcs Funcs[], int32_t n = 0 );
	void SetInIt(int32_t nStackSize,TLua_Funcs Funcs[], int32_t n = 0);
	BOOL LoadBuffer(PBYTE pBuffer, DWORD dwLen );
	int32_t  SafeCallBegin();//��¼��ǰ��ջջ��λ�ã������浽pIndexָ���������
	void SafeCallEnd (int32_t nIndex);   //�ָ�������֮ǰջ��λ�á�
	BOOL GetValuesFromStack(char * cFormat , ...);			
	
	BOOL  Stop();
	BOOL  Resume();
	DWORD CreateTable();
	DWORD ModifyTable(LPSTR szTableName); 
	void  SetGlobalName(LPSTR szName);
	int32_t   GetStackSpace();
	inline void SetTableMember(int32_t nIndex, int32_t Id, const char * szString){Lua_SetTable_StringFromId(m_LuaState,  nIndex, Id, szString);};
	inline void SetTableMember(int32_t nIndex, int32_t Id, double nNumber){Lua_SetTable_DoubleFromId(m_LuaState,  nIndex, Id, nNumber);};
	inline void SetTableMember(int32_t nIndex, int32_t Id, int32_t nNumber){Lua_SetTable_IntFromId(m_LuaState,  nIndex, Id, nNumber);};
	inline void SetTableMember(int32_t nIndex, const char * szMemberName, Lua_CFunction CFun){Lua_SetTable_CFunFromName(m_LuaState,  nIndex, szMemberName, CFun);};
	inline void SetTableMember(int32_t nIndex, int32_t nId, Lua_CFunction CFun){Lua_SetTable_CFunFromId(m_LuaState,  nIndex, nId, CFun);};
	inline void SetTableMember(int32_t nIndex, const char * szMemberName, char * szString){Lua_SetTable_StringFromName(m_LuaState,  nIndex, szMemberName, szString);};
	inline void SetTableMember(int32_t nIndex, const char * szMemberName, int32_t Number){Lua_SetTable_IntFromName(m_LuaState,  nIndex, szMemberName, Number);};
	inline void SetTableMember(int32_t nIndex, const char * szMemberName, double Number){Lua_SetTable_DoubleFromName(m_LuaState,  nIndex, szMemberName, Number);};
	void ScriptError(int32_t Error);
	void ScriptError(int32_t , int32_t);
	BOOL ExecuteCode();

//private:
	void RegisterStandardFunctions();
	BOOL m_IsRuning;//�Ƿ�ýű���Ч
	char m_szScriptName[256]; //�ű�·����
	int32_t	 m_UserTag;
	Lua_State * m_LuaState;
private:
	BOOL CallFunctionS(LPSTR cFuncName, int32_t nResults, LPSTR cFormat, va_list vlist);

};
//---------------------------------------------------------------------------
#endif //KLuaScript_H
