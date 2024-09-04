//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KList.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef	KList_H
#define	KList_H
//---------------------------------------------------------------------------
#include "cocos2d.h"

USING_NS_AX;

#include "KNode.h"

//---------------------------------------------------------------------------

class KList
{
public:
	KNode m_ListHead; // ͷ�ڵ㣨���ǵ�һ���ڵ㣩
	KNode m_ListTail; // β�ڵ㣨�������Ľڵ㣩
public:
	KList(void);
	KNode* GetHead(void); // ȡ�õ�һ���ڵ�
	KNode* GetTail(void); // ȡ�����һ���ڵ�
	void AddHead(KNode *pNode); // ����ǰ������һ���ڵ�
	void AddTail(KNode *pNode); // �����������һ���ڵ�
	KNode* RemoveHead(void); // ɾ����һ���ڵ�
	KNode* RemoveTail(void); // ɾ�����һ���ڵ�
	KNode* GetStatusNode(int32_t n);
    KNode* RemoveOneTail(int32_t n);//ɾ��ĳ���ڵ�

	BOOL IsEmpty(void);      // �Ƿ��Ǹ��յ�����
	LONG GetNodeCount(void);
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����:	KList
// ����:	����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
inline KList::KList(void)
{
	m_ListHead.m_pNext = &m_ListTail;
	m_ListTail.m_pPrev = &m_ListHead;
}
//-------------------------------------------------------------------------
// ����:	IsEmpty
// ����:	�Ƿ�Ϊ��
// ����:	void
// ����:	BOOL
//---------------------------------------------------------------------------
inline BOOL KList::IsEmpty(void)
{
	return (m_ListHead.GetNext() == NULL);
}
//-------------------------------------------------------------------------
// ����:	GetHead
// ����:	ȡ�������ͷ
// ����:	void
// ����:	KNode*
//---------------------------------------------------------------------------
inline KNode* KList::GetHead(void)
{
	return m_ListHead.GetNext();
}
//-------------------------------------------------------------------------
// ����:	GetTail
// ����:	ȡ�������β
// ����:	void
// ����:	KNode*
//---------------------------------------------------------------------------
inline KNode* KList::GetTail(void)
{
	return m_ListTail.GetPrev();
}
//-------------------------------------------------------------------------
// ����:	AddHead
// ����:	��ͷ������һ���ڵ�
// ����:	KNode*
// ����:	BOOL
//---------------------------------------------------------------------------
inline void KList::AddHead(KNode *pNode)
{
	m_ListHead.InsertAfter(pNode);
}
//-------------------------------------------------------------------------
// ����:	AddTail
// ����:	��ĩβ����һ���ڵ�
// ����:	KNode*
// ����:	void
//---------------------------------------------------------------------------
inline void KList::AddTail(KNode *pNode)
{
	m_ListTail.InsertBefore(pNode);
}
//-------------------------------------------------------------------------
// ����:	RemoveHead
// ����:	ɾ����һ���ڵ�
// ����:	void
// ����:	KNode*
//---------------------------------------------------------------------------
inline KNode* KList::RemoveHead(void)
{
	KNode* pNode = m_ListHead.GetNext();
	if (pNode)
		pNode->Remove();
	return pNode;
}
//-------------------------------------------------------------------------
// ����:	RemoveTail
// ����:	ɾ�����һ���ڵ�
// ����:	void
// ����:	KNode*
//---------------------------------------------------------------------------
inline KNode* KList::RemoveTail(void)
{
	KNode* pNode = m_ListTail.GetPrev();
	if (pNode)
		pNode->Remove();
	return pNode;
}
//-------------------------------------------------------------------------
// ����:	GetNodeCount
// ����:	ȡ�ýڵ����
// ����:	void
// ����:	LONG
//---------------------------------------------------------------------------
inline LONG KList::GetNodeCount(void)
{
	int32_t nNode = 0;
	KNode* pNode = GetHead();
	while (pNode)
	{
		pNode = pNode->GetNext();
		nNode++;
	}
	return nNode;
}
//-------------------------------------------------------------------------
// ����:	RemoveTail
// ����:	��ȡ�ڼ����ڵ�
// ����:	void
// ����:	KNode*
//---------------------------------------------------------------------------
inline KNode* KList::GetStatusNode(int32_t n)
{
	int32_t nNode = 0;
	KNode* pNode = GetHead();
	while (pNode)
	{
		pNode = pNode->GetNext();
		if  (nNode==n)
			break;
		nNode++;
	}

	return pNode;
}


//-------------------------------------------------------------------------
// ����:	RemoveOneTail
// ����:	ɾ�����һ���ڵ�
// ����:	void
// ����:	KNode*
//---------------------------------------------------------------------------
inline KNode* KList::RemoveOneTail(int32_t n)
{
/*	KNode* pNode = m_ListTail.GetPrev();
	if (pNode)
		pNode->Remove();
	return pNode;*/


	int32_t nNode = 0;
	KNode* pNode = GetHead();
	while (pNode)
	{
		pNode = pNode->GetNext();
		if  (nNode==n)
		{
			pNode->Remove();
			break;
		}
		nNode++;
	}

	return pNode;
}




//-------------------------------------------------------------------------
#endif
