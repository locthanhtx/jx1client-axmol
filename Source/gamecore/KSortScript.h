#ifndef KSORTSCRIPT_H
#define KSORTSCRIPT_H

#include "KCore.h"
#include "engine/KLuaScript.h"
//��������������澭���ļ���ת���ֵĽű���id,��ʵ�ֿ��ٲ��Ҷ�Ӧ�ű�

//#include "KBinTreeNode.h"
//#include "KBinsTree.h"
//extern char g_szCurScriptDir[MAX_PATH];

#define MAX_SCRIPT_IN_SET 5000   //�ű���������

extern KLuaScript g_ScriptSet[MAX_SCRIPT_IN_SET];
/*
class KSortScriptNode
{
	unsigned long m_dwScriptIndex;
	unsigned long m_dwScriptID;
public:



	unsigned long GetScriptIndex(){return m_dwScriptIndex;};
	unsigned long GetScriptID(){return m_dwScriptID;};

	void SetScriptIndex(unsigned long dwScriptIndex){m_dwScriptIndex = dwScriptIndex;};
	void SetScriptID(unsigned long dwScriptID){m_dwScriptID = dwScriptID;};
	const KScript * GetScript()
	{
		if (m_dwScriptIndex >= 0 && m_dwScriptIndex <MAX_SCRIPT_IN_SET)
			return &g_ScriptSet[m_dwScriptIndex];
		else
			return NULL;
	};
};

typedef	BinSTree<KSortScriptNode> KScriptBinTree;
extern KScriptBinTree g_ScriptBinTree;
*/
extern unsigned int g_IniScriptEngine(const char* nScriptDir, int nIsSer = TRUE, int nIsClear = TRUE);
extern const KScript * g_GetScript(unsigned long dwScriptId);//
extern const KScript * g_GetScript(const char * szRelativeScriptFile);
extern int ReLoadScript(const char * szRelativePathScript);
extern unsigned int  ReLoadAllScript();
extern void  ClearAllScript();

#endif
