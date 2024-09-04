//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCache.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Cache class
//---------------------------------------------------------------------------
#include "KMemBase.h"
#include "KMemManager.h"
#include "KCache.h"
//---------------------------------------------------------------------------
// ����:	KCache
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KCache::KCache()
{
	m_nMaxNode = 256;
	m_nLimitNode = 256;
}


//---------------------------------------------------------------------------
// ����:	Release
// ����:    �ͷ�����Cache�еĽڵ�,�������������޷������麯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KCache::Release()
{
	KCacheNode* lpNode;
	while (lpNode = (KCacheNode*)m_HashList.RemoveHead())
	{
		FreeNode(lpNode);
		delete(lpNode);
		lpNode=NULL;
	}
}

//---------------------------------------------------------------------------
// ����:	~KCache
// ����:	��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KCache::~KCache()
{
    /*Release();*/
}
//---------------------------------------------------------------------------
// ����:	Init
// ����:	
// ����:	
// ����:	void
//---------------------------------------------------------------------------
void KCache::Init(int32_t nMaxNode)
{
	m_nMaxNode = nMaxNode;
	m_nLimitNode = m_nMaxNode;
	printf(" KCache::Init(%d) OK!!!..\n",nMaxNode);
}
//---------------------------------------------------------------------------
// ����:	NewNode
// ����:	
// ����:	
// ����:	
//---------------------------------------------------------------------------
KCacheNode* KCache::NewNode(char* lpName)
{
	//Ѱ����ͬ�Ľڵ�
	KCacheNode* lpNode = (KCacheNode*)m_HashList.Find(lpName);
	
	//���������
	if (NULL == lpNode)
	{
		if (m_HashList.GetNodeCount() < m_nMaxNode)
		{
			//���û�е����ڵ������Ͳ���һ���½ڵ�
			//�����������������ǰ��
			lpNode = new KCacheNode;
			lpNode->SetName(lpName);
			m_HashList.AddHead(lpNode);
			LoadNode(lpNode);
			lpNode->m_Ref = 1;
		}
		else
		{	
			//�Ѿ��ﵽ���ڵ������鿴�Ƿ��������㣬�У��Ͱ�����õ�һ��
			//�ڵ��Ϊ��ǰ�ڵ㣬�������ƶ�����ǰ�棬������������ڵ���
			lpNode = (KCacheNode *)m_HashList.GetTail();
			if (lpNode->m_Ref == 0)
			{
				lpNode = (KCacheNode*)m_HashList.RemoveTail();
				FreeNode(lpNode);
				lpNode->SetName(lpName);
				LoadNode(lpNode);
				m_HashList.AddHead(lpNode);
				lpNode->m_Ref = 1;
			}
			else
			{
				lpNode = new KCacheNode;
				lpNode->SetName(lpName);
				m_HashList.AddHead(lpNode);
				LoadNode(lpNode);
				lpNode->m_Ref = 1;
				m_nMaxNode++;
			}
		}
	}
	return lpNode;
}
//---------------------------------------------------------------------------
// ����:	GetNode
// ����:	
// ����:	
// ����:	
//---------------------------------------------------------------------------
KCacheNode* KCache::GetNode(char* lpName, KCacheNode* lpNode)
{
	//������ǵ�һ��ȡ����Ƚ�һ�����֣����ڵ��Ƿ�ı�
	if (lpNode)
	{	//����ڵ�û�иı䣬�Ͱ����ƶ�����ǰ��
		if (g_StrCmp(lpName, lpNode->GetName()))
		{
			lpNode->Remove();
			m_HashList.AddHead(lpNode);
			lpNode->m_Ref = 1;
			return lpNode;
		}
	}
	//����ǵ�һ��ȡ���󣬻��߶����Ѿ��ı䣬�Ͳ����¶���
	return NewNode(lpName);
}
//---------------------------------------------------------------------------
// ����:	Prepare
// ����:	
// ����:	
// ����:	
//---------------------------------------------------------------------------
void KCache::Prepare()
{
	// �ͷŵ����趨��С��������ڱ���ʹ�����ò��ϵĽڵ�����
	int32_t	i = m_nMaxNode;
	KCacheNode* lpOldNode = NULL;
	KCacheNode* lpNode = (KCacheNode *)m_HashList.GetTail();

	while(i > m_nLimitNode)
	{
		if (lpNode->m_Ref)
			break;
		FreeNode(lpNode);
		lpNode = (KCacheNode *)lpNode->GetPrev();
		i--;
	}
	// �ѱ���ʹ�ñ�������Ա��´�ѭ��ʹ��
	lpNode = (KCacheNode *)m_HashList.GetHead();
	while(lpNode)
	{
		lpNode->m_Ref = 0;
		lpNode = (KCacheNode *)lpNode->GetNext();
	}
}

void	KCache::ClearNode(char * lpName)
{
	if (NULL == lpName || NULL == lpName[0]) return;
	//Ѱ�Ҹýڵ�
	KCacheNode* lpNode = (KCacheNode*)m_HashList.Find(lpName);
	
	//�������,�����!
	if (NULL != lpNode)
	{
		lpNode->Remove();
		FreeNode(lpNode);
		delete lpNode;
		lpNode=NULL;
	}
}
