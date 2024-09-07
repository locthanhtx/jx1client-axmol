#ifndef KLUBCMPL_BLOCKER
#define KLUBCMPL_BLOCKER

#include "KEngine.h"
#include "KList.h"
#include "KNode.h"
//#include "assert.h"
#include "KMemClass1.h"
#include "KStrBase.h"

#define KEYIF  1
#define KEYELSEIF 2
#define KEYELSE  3
#define KEYEND  4

//extern int g_StrLen(char*);
//extern void g_StrCpy(char*,char*);
//extern void g_StrCpyLen(char*,char*,int);
//extern void g_StrRep(char* lpDest, char* lpSrc, char* lpRep);

class KLineNode: public KNode
{
public:
	KMemClass1  * m_pLineMem;
	void CreateNode(char * szLine){int nLen = g_StrLen(szLine);assert(nLen > 0); g_StrCpy((char *)m_pLineMem, szLine); };
};

class KStackNode:public KNode
{
public:
	KLineNode * m_pLine;
	int			nKey;
};

class KLubCmpl_Blocker
{
public:
	KList m_StackList;
	KList m_Lines;		//
	KList m_RestLines;	//end;--main()����Ĵ���
	KList  UnitsList;

	int  GetBuffer(KMemClass1 * &pMem); //ת���������յ�Buffer
	void PushKey(KStackNode * pStackNode);
	KStackNode * PopKey();
	void ScanIf(KLineNode * pFirstNode);
	int ExchangeCurLines();
	KLineNode* Load(char* FileName);
	int Write(char* FileName);
	void Print();
};

#endif
