/*******************************************************************************
// FileName			:	KScriptCache.cpp
// FileAuthor		:	RomanDou
// FileCreateDate	:	2002-7-23 10:25:56
// FileDescription	:
// Revision Count	:
// Questions		:   �ű���Cache�������ʱ�Ƿ����Cache�����ˣ�Cacheͨ�������ַ�������Ч�ʻή��һЩ�����Ż������
*******************************************************************************/

#include "KScriptCache.h"

KScriptCache::KScriptCache()
{

}

KScriptCache::~KScriptCache()
{
    Release();      // ���û����еĺ���,�ͷ����нڵ�
}

int KScriptCache::LoadNode(KCacheNode* lpNode)
{
	KLuaScript* pScript = new KLuaScript;
	pScript->Init();
	if (pScript->Load(lpNode->GetName()))
		lpNode->m_lpData = pScript;
	else
		lpNode->m_lpData = NULL;
	return lpNode->m_lpData != NULL;
}
//---------------------------------------------------------------------------
// ����:	FreeNode
// ����:
// ����:
// ����:	void
//---------------------------------------------------------------------------
void KScriptCache::FreeNode(KCacheNode* lpNode)
{
	KLuaScript* pLuaScript = (KLuaScript*)lpNode->m_lpData;
	if (pLuaScript)
	{
		pLuaScript->Exit();
		delete pLuaScript;
		pLuaScript=NULL;
	}
	lpNode->m_lpData = NULL;
}
