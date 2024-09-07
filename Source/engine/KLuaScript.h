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

#define MAINFUNCTIONNAME            "main"
#define LUA_CREATE_ERROR            1   // ��������
#define LUA_SCRIPT_LEN_ERROR        2   // ��С����
#define LUA_SCRIPT_COMPILE_ERROR    3   // �������
#define LUA_SCRIPT_EXECUTE_ERROR    4   // ִ�г���
#define LUA_SCRIPT_NOT_NUMBER_ERROR 10  // �������ֳ���
#define LUA_SCRIPT_NOT_STRING_ERROR 11  // �����ַ���
#define LUA_SCRIPT_NOT_TABLE_ERROR  12  // ���Ǳ��
#define LUA_SCRIPT_STATES_IS_NULL   20  // �ű�Ϊ��

typedef struct
{
    const char* name;
    lua_CFunction func;

} TLua_Funcs;

//---------------------------------------------------------------------------
class KLuaScript : public KScript
{
public:
    /*
    KLuaScript();
    virtual ~KLuaScript();
    KLuaScript(int StackSize);*/
    KLuaScript(void);
    KLuaScript(int StackSize);
    virtual ~KLuaScript();

    int Activate()
    {
        Execute();
        return 1;
    };

    virtual int Init();
    virtual void Exit();
    virtual int Load(const char* FileName);
    virtual int Compile(char* FileName);
    virtual int Execute();
    virtual int CallFunction(const char* cFuncName, int nResults, const char* cFormat, ...);
    virtual int RegisterFunction(char* FuncName, void* Func);
    virtual void SetScriptName(char* scpname) { strcpy(m_szScriptName, scpname); };

    int GetUserTag() { return m_UserTag; };
    int RegisterFunctions(TLua_Funcs Funcs[], int n = 0);
    void SetInIt(int nStackSize, TLua_Funcs Funcs[], int n = 0);
    int LoadBuffer(PBYTE pBuffer, unsigned long dwLen);
    int SafeCallBegin();  // ��¼��ǰ��ջջ��λ�ã������浽pIndexָ���������
    void SafeCallEnd(int nIndex);  // �ָ�������֮ǰջ��λ�á�
    int GetValuesFromStack(char* cFormat, ...);

    int Stop();
    int Resume();
    unsigned long CreateTable();
    unsigned long ModifyTable(char* szTableName);
    void SetGlobalName(const char* szName);
    int GetStackSpace();
    inline void SetTableMember(int nIndex, int Id, const char* szString)
    {
        Lua_SetTable_StringFromId(m_LuaState, nIndex, Id, szString);
    };
    inline void SetTableMember(int nIndex, int Id, double nNumber)
    {
        Lua_SetTable_DoubleFromId(m_LuaState, nIndex, Id, nNumber);
    };
    inline void SetTableMember(int nIndex, int Id, int nNumber)
    {
        Lua_SetTable_IntFromId(m_LuaState, nIndex, Id, nNumber);
    };
    inline void SetTableMember(int nIndex, const char* szMemberName, Lua_CFunction CFun)
    {
        Lua_SetTable_CFunFromName(m_LuaState, nIndex, szMemberName, CFun);
    };
    inline void SetTableMember(int nIndex, int nId, Lua_CFunction CFun)
    {
        Lua_SetTable_CFunFromId(m_LuaState, nIndex, nId, CFun);
    };
    inline void SetTableMember(int nIndex, const char* szMemberName, char* szString)
    {
        Lua_SetTable_StringFromName(m_LuaState, nIndex, szMemberName, szString);
    };
    inline void SetTableMember(int nIndex, const char* szMemberName, int Number)
    {
        Lua_SetTable_IntFromName(m_LuaState, nIndex, szMemberName, Number);
    };
    inline void SetTableMember(int nIndex, const char* szMemberName, double Number)
    {
        Lua_SetTable_DoubleFromName(m_LuaState, nIndex, szMemberName, Number);
    };
    void ScriptError(int Error);
    void ScriptError(int, int);
    int ExecuteCode();

    // private:
    void RegisterStandardFunctions();
    int m_IsRuning;            // �Ƿ�ýű���Ч
    char m_szScriptName[256];  // �ű�·����
    int m_UserTag;
    Lua_State* m_LuaState;

private:
    int CallFunctionS(const char* cFuncName, int nResults, const char* cFormat, va_list vlist);
};
//---------------------------------------------------------------------------
#endif  // KLuaScript_H
