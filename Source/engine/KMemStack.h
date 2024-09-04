//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMemStack.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef _KMem_Stack_H
#define _KMem_Stack_H

#include "cocos2d.h"
USING_NS_AX;
#include "KbugInfo.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class KMemStack
{
private:
	PBYTE		m_pStack[MAX_CHUNK];
	int32_t			m_nStackTop;
	int32_t 		m_nStackEnd;
	int32_t			m_nChunkTop;
	int32_t			m_nChunkSize;
public:
	KMemStack();
	~KMemStack();
	void		Init(int32_t nChunkSize = 65536);
	PVOID		Push(int32_t nSize);
	void		Free(LPVOID pMem);
	BOOL		AllocNewChunk();
	void		FreeAllChunks();
	int32_t			GetChunkSize();
	int32_t			GetStackSize();
};
//---------------------------------------------------------------------------
#endif
