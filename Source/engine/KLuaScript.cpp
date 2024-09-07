//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KLuaScript.cpp
// Date:	2001-9-13 10:33:29
// Code:	Romandou
// Desc:
//---------------------------------------------------------------------------
#include "KPakFile.h"
#include "KLuaScript.h"
#include "lualib/LuaLib.h"	 // LuaLib.h
#include "KMemClass.h"
#include "KStrBase.h"
//---------------------------------------------------------------------------
// ����:	KLuaScript::KLuaScript
// ����:
// ����:	void
// ����:
//---------------------------------------------------------------------------
 /*
static void *l_alloc (void *ud, void *ptr, size_t osize, size_t nsize)
{//5.3.4
	(void)ud; (void)osize;

	if (nsize == 0)
	{//�ͷ��ڴ�
		free(ptr);
		return NULL;
	}
	else
		return realloc(ptr, nsize);
}*/

KLuaScript::KLuaScript(void)
{
	//void *ptr;
	//void *ud;
	//m_LuaState	= lua_newstate(l_alloc,NULL); //100	 5.3.4
	m_LuaState	= lua_open(128); //100	����һ����С�Ķ�ջ

	if (m_LuaState == NULL)
	{
		ScriptError(LUA_CREATE_ERROR);
		m_IsRuning			= FALSE;
		return ;
	}

	m_IsRuning				= TRUE;
	m_szScriptName[0]		= '\0';
}


//---------------------------------------------------------------------------
// ����:	KLuaScript::KLuaScript
// ����:
// ����:	int StackSize
// ����:
//---------------------------------------------------------------------------
KLuaScript::KLuaScript(int StackSize)
{
	m_LuaState	= Lua_Create(StackSize);	 //

	if (m_LuaState == NULL )
	{
		ScriptError(LUA_CREATE_ERROR);
		m_IsRuning = FALSE;
		return ;
	}
	m_IsRuning				= TRUE;
	m_szScriptName[0]		= '\0';
}


//---------------------------------------------------------------------------
// ����:	KLuaScript::~KLuaScript
// ����:
// ����:	void
// ����:
//---------------------------------------------------------------------------
KLuaScript::~KLuaScript(void)
{
	Exit();
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::LoadBuffer()
// ����:
// ����:	PBYTE pBuffer
// ����:	unsigned long dwLen
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::LoadBuffer(PBYTE pBuffer, unsigned long dwLen )
{
	if (dwLen < 0)
	{
		ScriptError(LUA_SCRIPT_LEN_ERROR);
		return FALSE;
	}

	if (!m_LuaState)
	{
	   ScriptError(LUA_SCRIPT_COMPILE_ERROR);
	   return FALSE;
	}

	if (Lua_CompileBuffer(m_LuaState, (char *) pBuffer, dwLen, NULL) != 0)
	{//����ű�
		ScriptError(LUA_SCRIPT_COMPILE_ERROR);
		return FALSE;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	KLuaScript::Load
// ����:
// ����:	char* Filename
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::Load(const char* Filename)
{
	KPakFile	File;
	unsigned long		nSize;

//	printf("----��ʼ���ؽű�:%s----\n", Filename);

	// open file
	if (!File.Open(Filename))	return FALSE;

	// get file size
	nSize = File.Size();
	//KLuaScript(nSize);
	//SetInIt(nSize,Funcs,n);

	KMemClass Memory;
	// alloc memory
	if (!Memory.Alloc(nSize + 4))
	{
		File.Close();
		return FALSE;
	}
	// read file
	if (File.Read(Memory.GetMemPtr(),nSize) != nSize)
	{
		File.Close();
		Memory.Free();
		return FALSE;
	}
	char * pszMem = (char *)Memory.GetMemPtr();
	pszMem[nSize + 1] = 0;

	File.Close();

	try
	{
		if (!LoadBuffer((PBYTE)Memory.GetMemPtr(),nSize))
		{
			ScriptError(LUA_SCRIPT_COMPILE_ERROR);
			printf("Load Script %s ����A!!\n", Filename);
			Memory.Free();
			return FALSE;
		}
	}
	catch(...)
	{
		printf("Load Script %s �����쳣,����!!\n", Filename);
		Memory.Free();
		return FALSE;
	}

	if (!ExecuteCode())
	{
		printf("Load Script %s ����B!!\n",Filename);
		Memory.Free();
		return FALSE;
	}
	Memory.Free();

	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::Execute
// ����:
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::Execute()
{
	if (m_IsRuning && m_LuaState)
	return CallFunction(MAINFUNCTIONNAME,0,"");

	return FALSE;
}


//---------------------------------------------------------------------------
// ����:	KLuaScript::ExecuteCode
// ����:
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::ExecuteCode()
{
	if (!(m_IsRuning && m_LuaState))
	{
		ScriptError(LUA_SCRIPT_STATES_IS_NULL);
		printf("-------------m_LuaState Ϊ��---------------\n");
		return FALSE;
	}

	int state;
	if (state = Lua_Execute(m_LuaState) != 0)
	{
		printf("-------------Lua_Execute ����---------------\n");
		//ScriptError(LUA_SCRIPT_EXECUTE_ERROR, state);
		return FALSE;
	}

	return	TRUE;
}


//---------------------------------------------------------------------------
// ����:	KLuaScript::CallFunction
// ����:	����Lua�ű��ڵĺ���
// ����:	char* cFuncName
// ����:	int nResults
// ����:	char* cFormat  ����ʱ��������������
//			n:������(double) d:����(int) s:�ַ����� f:C������  n:Nil v:Value p:Point
//        v��ΪLua֧�ֵģ�����Ϊ���ε���index��ָ����index��ָ��ջ�ı�����Ϊ
//			 �ú����ĵ��ò�����
//	ע�⣺���ڸú����в���������,�������֣�ϵͳ����ȷ��������double������int
//  ���ڣ����ֱ�����ʽ�ǲ�ͬ�ġ������Ҫע�⵱�������������ʱ����ʽ��Ӧ��d
//  ��������n,����ǿ�иı�Ϊdouble�Ρ��������ּ���Ĵ���
//
// ����:	...
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::CallFunctionS(const char* cFuncName, int nResults, const char* cFormat, va_list vlist)
{

	double nNumber;
	char * cString	= NULL;
	void * pPoint	= NULL;
	Lua_CFunction CFunc;
	int i=0;
	int nArgnum = 0;    //����������
	int nIndex = 0;
	int nRetcode;		//���ýű�������ķ�����

	if (!(m_IsRuning && m_LuaState))
	{//�ű�ֹͣ������
		ScriptError(LUA_SCRIPT_STATES_IS_NULL);
		return FALSE;
	}

	{
		Lua_GetGlobal(m_LuaState,cFuncName); //�ڶ�ջ�м�����Ҫ���õĺ�����

		while (cFormat[i] != '\0')	//'\0
		{
			switch(cFormat[i])
			{
			case 'n'://�����������double�� NUMBER��Lua��˵��Double��
				{
					nNumber = va_arg(vlist, double);
					Lua_PushNumber(m_LuaState, nNumber);
					nArgnum ++;

				}
				break;

			case 'd'://���������Ϊ����
				{
					nNumber = (double)(va_arg(vlist, int));
					Lua_PushNumber(m_LuaState, (double) nNumber);
					nArgnum ++;
				}
				break;

			case 's'://�ַ�����
				{
					cString = va_arg(vlist, char *);
					Lua_PushString(m_LuaState, cString);
					nArgnum ++;
				}
				break;
			case 'N'://NULL
				{
					Lua_PushNil(m_LuaState);
					nArgnum ++;
				}
				break;

			case 'f'://�������CFun�Σ����ڲ�������   //����Ҫִ�еĺ���
				{
					CFunc = va_arg(vlist, Lua_CFunction);
					Lua_PushCFunction(m_LuaState, CFunc);
					nArgnum ++;
				}
				break;

			case 'v'://������Ƕ�ջ��IndexΪnIndex����������
				{
					nNumber = va_arg(vlist, int);
					int nIndex1 = (int) nNumber;
					Lua_PushValue(m_LuaState, nIndex1);
					nArgnum ++;
				}
				break;
			case 't'://����ΪһTable����
				{//�зָ����'t\' �Ʊ��



				}
				break;

			case 'p':
				{//����ָ��
					pPoint = va_arg(vlist, void *);
					Lua_PushUserTag(m_LuaState, pPoint,m_UserTag);
					nArgnum ++;
				}
				break;
			}

			i++;
		}

	}

	//printf("----Lua_Callǰ(%s)(%s),����:%d ��,����----\n",m_szScriptName,cFuncName,nArgnum);

	    nRetcode =Lua_Call(m_LuaState,nArgnum,nResults);	 //��ʼִ���������	Lua_Call

	if (nRetcode != 0)
	{
		//Exit(); // �ͷ���Դ
		int nTopIdx=0;
		    nTopIdx=Lua_SafeBegin(m_LuaState);
		if  (nTopIdx>1)
		   Lua_SafeEnd(m_LuaState, nTopIdx);//ƽ���ջ

		ScriptError(LUA_SCRIPT_EXECUTE_ERROR, nRetcode);
		//printf("----Lua_Call(%s),����:%d ��,ջԪ��:%d ��,�����쳣----\n", cFuncName,nArgnum,nTopIdx);
		return FALSE;
	}

	return	TRUE;
}


//---------------------------------------------------------------------------
// ����:	KLuaScript::CallFunction
// ����:
// ����:	char* cFuncName
// ����:	int nResults
// ����:	char* cFormat
// ����:	...
// ����:	int   ---��ȷ��������
//---------------------------------------------------------------------------
int KLuaScript::CallFunction(const char* cFuncName, int nResults, const char* cFormat, ...)
{
	int bResult  = FALSE;
	va_list vlist;
	va_start(vlist, cFormat);
	bResult = CallFunctionS(cFuncName, nResults, cFormat, vlist);
	va_end(vlist);
	return bResult;
}


//---------------------------------------------------------------------------
// ����:	KLuaScript::GetValuesFromStack
// ����:	�Ӷ�ջ�л�ñ���
// ����:	char * cFormat
// ����:	...
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::GetValuesFromStack(char * cFormat, ...)
{
	va_list vlist;
	double* pNumber = NULL;
	const char **   pString ;
	int * pInt = NULL;
	int i = 0;
	int nTopIndex = 0;
	int nIndex = 0;
	int nValueNum = 0;//cFormat���ַ����ȣ���ʾ��Ҫȡ�Ĳ�������

	if (! m_LuaState)
		return FALSE;

	nTopIndex = Lua_GetTopIndex(m_LuaState);
	nValueNum = strlen(cFormat);

	if (nTopIndex == 0 || nValueNum == 0)//����ջ�������ݻ�ȡ�����Ƿ���FALSE
		return FALSE;

	if (nTopIndex < nValueNum)
		return FALSE;

	nIndex = nTopIndex - nValueNum +1;

	{
		va_start(vlist, cFormat);

		while (cFormat[i] != '\0')
		{

			switch(cFormat[i])
			{
			case 'n'://����ֵΪ��ֵ��,Number,��ʱLuaֻ����double�ε�ֵ
				{
					pNumber = va_arg(vlist, double *);

					if (pNumber == NULL)
						return FALSE;

					if (Lua_IsNumber(m_LuaState, nIndex ))
					{
						* pNumber = Lua_ValueToNumber(m_LuaState, nIndex ++ );

					}
					else
					{
						ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
						return FALSE;
					}


				}
				break;
			case 'd':
				{
					pInt = va_arg(vlist, int *);
					if (pInt == NULL)
						return FALSE;
					if ( Lua_IsNumber(m_LuaState, nIndex))
					{
						* pInt = (int ) Lua_ValueToNumber(m_LuaState, nIndex ++);
					}
					else
					{
						ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
						return FALSE;
					}

				}
				break;
			case 's'://�ַ�����
				{
					pString = va_arg(vlist, const char **);

					if (pString == NULL)
						return FALSE;

					if (Lua_IsString(m_LuaState, nIndex))
					{
						(*pString) = (const char *)Lua_ValueToString(m_LuaState, nIndex++);

					}
					else
					{
						ScriptError(LUA_SCRIPT_NOT_STRING_ERROR);
						return FALSE;
					}
				}
				break;

			}


		i ++;
		}
		va_end(vlist);     		/* Reset variable arguments.      */

	}
	return	TRUE;
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::Init
// ����:	��ʼ���ű�����ע��ϵͳ��׼������
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::Init()
{
	if (!m_LuaState)
	{
		m_LuaState	= Lua_Create(0);	 //0

		if (m_LuaState == NULL)
		{
			ScriptError(LUA_CREATE_ERROR);
			m_IsRuning			= FALSE;
			return FALSE;
		}

		m_IsRuning				= TRUE;
		m_szScriptName[0]		= '\0';
		m_UserTag =TRUE;
	    lua_newtag(m_LuaState);
	}

	RegisterStandardFunctions();
	//printf(" KLuaScript::Init() OKB!!!..\n");
	return	TRUE;
}

void KLuaScript::SetInIt(int nStackSize,TLua_Funcs Funcs[], int n)

{
	if (m_LuaState)
	   Exit();

    //if (m_LuaState)
	//m_LuaState->stacksize = nStackSize;

	m_LuaState	= lua_open(nStackSize);                             //100	����һ����С�Ķ�ջ
	printf("----�ű������ڴ�:%u----\n", nStackSize);
	if (m_LuaState == NULL)
	{
		ScriptError(LUA_CREATE_ERROR);
		m_IsRuning			= FALSE;
		return ;
	}

	m_IsRuning				= TRUE;
	m_szScriptName[0]		= '\0';

    RegisterStandardFunctions();
    RegisterFunctions(Funcs,n); //ע�ắ��
}


//---------------------------------------------------------------------------
// ����:	KLuaScript::RegisterFunction
// ����:	ע��ĳ�ڲ�C�������ű���
// ����:	char* FuncName  �ڽű���ʹ�õĺ�����
// ����:	void* Func      ʵ����Ӧ��C����ָ��
// ����:	int Args = 0    //��KScript�ӿ����ݣ�����
// ����:	int Flag = 0    //��KScript�ӿ�����, ����
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::RegisterFunction(char* FuncName , void* Func)
{
	if (! m_LuaState)
		return FALSE;
	Lua_Register(m_LuaState, FuncName, (Lua_CFunction)Func);
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::Compile
// ����:
// ����:	char *
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::Compile(char *)
{
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::RegisterFunctions
// ����:	����ע��Lua���ڲ�C������������������Ϣ������TLua_Funcs��������
// ����:	TLua_Funcs *Funcs �����ָ��
// ����:	int n ��������������Ϊ�㣬��ϵͳ����õ���
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::RegisterFunctions(TLua_Funcs Funcs[], int n)
{
	if (! m_LuaState)
		return FALSE;
	if (n == 0)
		n = sizeof(Funcs) / sizeof(Funcs[0]);
	for (int i = 0; i < n; i ++)
		Lua_Register(m_LuaState, Funcs[i].name, Funcs[i].func);

	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::RegisterStandardFunctions
// ����:	ע��Luaϵͳ��׼�ĺ�����
// ����:	void
//---------------------------------------------------------------------------
void KLuaScript::RegisterStandardFunctions()
{
	//printf("��ʼע�ắ�� OKB!!!..\n");
	if (!m_LuaState)
	{
		printf("m_LuaState Ϊ��!!!..\n");
		return ;

	}

//	Lua_OpenAllLib(m_LuaState);		//��ʼ�����л�����
 try
 {
	 Lua_OpenBaseLib(m_LuaState);  //Lua������  ����lualibdll
	 Lua_OpenIOLib(m_LuaState);    //���������
	 Lua_OpenStrLib(m_LuaState);   //�ַ��������
	 Lua_OpenMathLib(m_LuaState);  //��ֵ�����
	 Lua_OpenDBLib(m_LuaState);    //���Կ�
 }
 catch (...)
 {
	 printf("----ע��ű����������----\n");
	 printf("----ע��ű����������----\n");
	 printf("----ע��ű����������----\n");

 }

	return;
}


//---------------------------------------------------------------------------
// ����:	KLuaScript::ReleaseScript
// ����:	�ͷŸýű���Դ��
// ����:	int
//---------------------------------------------------------------------------
void KLuaScript::Exit()
{
	if (!m_LuaState)
		return;
	Lua_Release(m_LuaState);
	m_LuaState = NULL;
	m_IsRuning = FALSE;
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::ScriptError
// ����:
// ����:	int Error
// ����:	void
//---------------------------------------------------------------------------
void KLuaScript::ScriptError(int Error)
{
	char lszErrMsg[200];
	sprintf(lszErrMsg, "ScriptError %d. (%s) \n", Error, m_szScriptName);
	lua_outerrmsg(lszErrMsg);
	//Exit();
	return;
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::ScriptError
// ����:
// ����:	int Error1
// ����:	int Error2
// ����:	void
//---------------------------------------------------------------------------
void KLuaScript::ScriptError(int Error1 ,int Error2)
{
	char lszErrMsg[200]={0};
	sprintf(lszErrMsg, "ScriptError %d:[%d] (%s) \n", Error1, Error2, m_szScriptName);
	lua_outerrmsg(lszErrMsg);
	//Exit();
	return;
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::SafeCallBegin
// ����:
// ����:	int * pIndex
// ����:	void
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// SafeCallBegin��SafeCallEnd������Ӧ����ʹ�ã��Է�ֹ�ڵ���Lua���ⲿ����֮��
//�ж��������ڶ�ջ��δ��������ﵽ����ǰ����ú��ջ��ռ�ô�С���䡣
//�������ֻ�����ڵ����ⲿ����ʱ���ڲ�����������˴���
//																	Romandou
//---------------------------------------------------------------------------
int KLuaScript::SafeCallBegin()
{
	if (!m_LuaState)
		return 0;
  return Lua_SafeBegin(m_LuaState);
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::SafeCallEnd
// ����:
// ����:	int nIndex
// ����:	void
//---------------------------------------------------------------------------
void KLuaScript::SafeCallEnd(int nIndex)
{
	if (!m_LuaState)
		return;
	Lua_SafeEnd(m_LuaState, nIndex);
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::StopScript
// ����:	��ֹ�ű�
// ����:	void
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::Stop(void)
{
	if (!m_IsRuning)		return TRUE;
	if (!m_LuaState)		return FALSE;
	m_IsRuning =  FALSE;
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::ResumeScript
// ����:	�ָ�����ֹ�Ľű�
// ����:	void
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::Resume(void)
{
	if ((!m_IsRuning) && (m_LuaState))
	{
		m_IsRuning = TRUE;
		return TRUE;
	}
	return FALSE;
}
//---------------------------------------------------------------------------
// ����:	KLuaScript::CreateTable
// ����:	����һ��Lua��Table���ڵ��øú���������Table������Ա֮�󣬱������
//			SetGlobalName()�������Tableָ��һ�����֡�
// ����:	unsigned long
//---------------------------------------------------------------------------
unsigned long KLuaScript::CreateTable()
{
	 int nIndex = 0;

	nIndex = Lua_GetTopIndex(m_LuaState) ;
	Lua_NewTable(m_LuaState);
	if (Lua_GetTopIndex(m_LuaState) != ++nIndex )
		return -1;

	return nIndex;
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::SetGlobalName
// ����:	����Lua��ջ����������һ������
// ����:	char* szName
// ����:	void
//---------------------------------------------------------------------------
void KLuaScript::SetGlobalName(const char* szName)
{
	if (!szName) return ;
	Lua_SetGlobal(m_LuaState, szName);
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::GetStackSpace
// ����:	��ȡ��ջ�Ŀռ�
// ����:	m_LuaState
// ����:	int
//---------------------------------------------------------------------------
int KLuaScript::GetStackSpace()
{
	if (!m_LuaState) return 0;
	 return Lua_GetStackSpace(m_LuaState);
}

//---------------------------------------------------------------------------
// ����:	KLuaScript::ModifyTable
// ����:	��ָ�����Ƶ�LuaTable�ö�ջ���ˣ������ض���Index
// ����:	char* szTableName
// ����:	unsigned long ��Lua�в����ڸ�Table�򷵻�-1
//---------------------------------------------------------------------------
unsigned long KLuaScript::ModifyTable(char* szTableName)
{
	if (! szTableName[0])		return -1;

	int nIndex = Lua_GetTopIndex(m_LuaState);

	Lua_GetGlobal(m_LuaState, szTableName);

	if (Lua_GetTopIndex(m_LuaState) != ++nIndex)
		return -1;

	return nIndex;
}
