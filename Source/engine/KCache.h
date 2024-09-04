//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCache.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KCache_H
#define KCache_H
//---------------------------------------------------------------------------
#include "KStrNode.h"
#include "KStrList.h"
//---------------------------------------------------------------------------
class KCacheNode : public KStrNode
{
public:
	PVOID	m_lpData;			//����ָ��
	LONG	m_Ref;				//��֡�Ƿ�����
public:
	KCacheNode() {m_Ref = 0; m_lpData = NULL;};
};
//---------------------------------------------------------------------------
class  KCache
{
public:
	LONG m_nMaxNode;			//������ڵ���
	LONG m_nLimitNode;			//�趨�Ķ���ڵ���
	KStrList m_HashList;		//�����ϣ����
public:
	KCache();
	~KCache();
	virtual void Init(int32_t nMaxNode=256);
	virtual KCacheNode* NewNode(char* lpName);
	virtual KCacheNode* GetNode(char* lpName, KCacheNode* lpNode);
	virtual BOOL LoadNode(KCacheNode* lpNode){return FALSE;};
	virtual void FreeNode(KCacheNode* lpNode){};
	void	ClearNode(char * lpName);//���ĳ�����
	void	Prepare();
    virtual void Release();
};
//---------------------------------------------------------------------------
#endif
