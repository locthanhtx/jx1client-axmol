//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNode.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef	KNode_H
#define	KNode_H
//---------------------------------------------------------------------------
#include "cocos2d.h"

USING_NS_AX;
//#include "KDebug.h"
#include "KbugInfo.h"
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)

class KNode
{
public:
	KNode* m_pNext;
	KNode* m_pPrev;

public:
	KNode(void);
	virtual ~KNode(){};
	KNode* GetNext(void);
	KNode* GetPrev(void);
	void InsertBefore(KNode* pNode);
	void InsertAfter(KNode* pNode);
	void Remove(void);
	int IsLinked(void);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����:	Knode
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
inline KNode::KNode(void)
{
	m_pNext = NULL;
	m_pPrev = NULL;
}
//---------------------------------------------------------------------------
// ����:	GetNext
// ����:	��һ�����
// ����:	void
// ����:	KNode*
//---------------------------------------------------------------------------
inline KNode* KNode::GetNext(void)
{
	if (m_pNext->m_pNext)
		return m_pNext;
	return NULL;
}
//---------------------------------------------------------------------------
// ����:	GetPrev
// ����:	ǰһ�����
// ����:	void
// ����:	KNode*
//---------------------------------------------------------------------------
inline KNode* KNode::GetPrev(void)
{
	if (m_pPrev->m_pPrev)
		return m_pPrev;
	return NULL;
}
//----------------------------------------------------------------------------
// ����:	InsertBefore
// ����:	��ǰ�����һ�����
// ����:	KNode*
// ����:	void
//---------------------------------------------------------------------------
inline void KNode::InsertBefore(KNode *pNode)
{
	//KASSERT(m_pPrev);
	if (!pNode || !m_pPrev)
		return;

	pNode->m_pPrev = m_pPrev;
	pNode->m_pNext = this;
	m_pPrev->m_pNext = pNode;
	m_pPrev = pNode;
}
//---------------------------------------------------------------------------
// ����:	InsertAfter
// ����:	�ں������һ�����
// ����:	KNode*
// ����:	void
//---------------------------------------------------------------------------
inline void KNode::InsertAfter(KNode *pNode)
{
//	KASSERT(m_pNext);
	if (!pNode || !m_pNext)
		return;
	pNode->m_pPrev = this;
	pNode->m_pNext = m_pNext;
	m_pNext->m_pPrev = pNode;
	m_pNext = pNode;
}
//---------------------------------------------------------------------------
// ����:	Remove the node
// ����:	ɾ��������
// ����:	void
// ����:	KNode*
//---------------------------------------------------------------------------
inline void KNode::Remove(void)
{
//	KASSERT(m_pPrev);
//	KASSERT(m_pNext);
	if (m_pPrev)
		m_pPrev->m_pNext = m_pNext;
	if (m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	m_pPrev = NULL;
	m_pNext = NULL;
}
//---------------------------------------------------------------------------
// ����:	IsLinked
// ����:	����ڵ��Ƿ�����
// ����:	void
// ����:	bool
//---------------------------------------------------------------------------
inline int KNode::IsLinked(void)
{
	return (m_pPrev && m_pNext);
}
//--------------------------------------------------------------------------------

#endif
