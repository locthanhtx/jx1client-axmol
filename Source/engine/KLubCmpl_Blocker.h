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

//extern int32_t g_StrLen(LPSTR);
//extern void g_StrCpy(LPSTR,LPSTR);
//extern void g_StrCpyLen(LPSTR,LPSTR,int32_t);
//extern void g_StrRep(LPSTR lpDest, LPSTR lpSrc, LPSTR lpRep);

class KLineNode: public KNode
{
public:
	KMemClass1  * m_pLineMem;
	void CreateNode(char * szLine){int32_t nLen = g_StrLen(szLine);assert(nLen > 0); g_StrCpy((char *)m_pLineMem, szLine); };
};

class KStackNode:public KNode
{
public:
	KLineNode * m_pLine;
	int32_t			nKey;
};

class KLubCmpl_Blocker 
{
public:
	KList m_StackList;
	KList m_Lines;		//
	KList m_RestLines;	//end;--main()����Ĵ���
	KList  UnitsList;

	int32_t  GetBuffer(KMemClass1 * &pMem); //ת���������յ�Buffer
	void PushKey(KStackNode * pStackNode);
	KStackNode * PopKey();
	void ScanIf(KLineNode * pFirstNode);
	BOOL ExchangeCurLines();
	KLineNode* Load(LPSTR FileName);
	BOOL Write(LPSTR FileName);
	void Print();
};

#endif
