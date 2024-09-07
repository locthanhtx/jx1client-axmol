//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNode.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Simple double linked node class
//---------------------------------------------------------------------------
//#include "KWin32.h"
//#include "KDebug.h"
#include "KNode.h"
////---------------------------------------------------------------------------
//// ����:	Knode
//// ����:	����
//// ����:	void
//// ����:	void
////---------------------------------------------------------------------------
//inline KNode::KNode(void)
//{
//	m_pNext = NULL;
//	m_pPrev = NULL;
//}
////---------------------------------------------------------------------------
//// ����:	GetNext
//// ����:	��һ�����
//// ����:	void
//// ����:	KNode*
////---------------------------------------------------------------------------
//inline KNode* KNode::GetNext(void)
//{
//	if (m_pNext->m_pNext)
//		return m_pNext;
//	return NULL;
//}
////---------------------------------------------------------------------------
//// ����:	GetPrev
//// ����:	ǰһ�����
//// ����:	void
//// ����:	KNode*
////---------------------------------------------------------------------------
//inline KNode* KNode::GetPrev(void)
//{
//	if (m_pPrev->m_pPrev)
//		return m_pPrev;
//	return NULL;
//}
////----------------------------------------------------------------------------
//// ����:	InsertBefore
//// ����:	��ǰ�����һ�����
//// ����:	KNode*
//// ����:	void
////---------------------------------------------------------------------------
//inline void KNode::InsertBefore(KNode *pNode)
//{
//	KASSERT(m_pPrev);
//	pNode->m_pPrev = m_pPrev;
//	pNode->m_pNext = this;
//	m_pPrev->m_pNext = pNode;
//	m_pPrev = pNode;
//}
////---------------------------------------------------------------------------
//// ����:	InsertAfter
//// ����:	�ں������һ�����
//// ����:	KNode*
//// ����:	void
////---------------------------------------------------------------------------
//inline void KNode::InsertAfter(KNode *pNode)
//{
//	KASSERT(m_pNext);
//	pNode->m_pPrev = this;
//	pNode->m_pNext = m_pNext;
//	m_pNext->m_pPrev = pNode;
//	m_pNext = pNode;
//}
////---------------------------------------------------------------------------
//// ����:	Remove the node
//// ����:	ɾ��������
//// ����:	void
//// ����:	KNode*
////---------------------------------------------------------------------------
//inline void KNode::Remove(void)
//{
//	KASSERT(m_pPrev);
//	KASSERT(m_pNext);
//	m_pPrev->m_pNext = m_pNext;
//	m_pNext->m_pPrev = m_pPrev;
//	m_pPrev = NULL;
//	m_pNext = NULL;
//}
////---------------------------------------------------------------------------
//// ����:	IsLinked
//// ����:	����ڵ��Ƿ�����
//// ����:	void
//// ����:	bool
////---------------------------------------------------------------------------
//inline int KNode::IsLinked(void)
//{
//	return (m_pPrev && m_pNext);
//}
////--------------------------------------------------------------------------------
