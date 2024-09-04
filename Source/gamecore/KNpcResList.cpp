//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcResList.cpp
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------
#include "KCore.h"

#include	"CoreUseNameDef.h"
#include	"KNpcResList.h"
KNpcResList	g_NpcResList;

KNpcResList::KNpcResList()
{
	
}

KNpcResList::~KNpcResList()
{
	KNpcResNode		*pTempNode;
	KNpcResNode		*pTempNextNode;

    // �ͷ����������еĽڵ�
   	pTempNode = (KNpcResNode *)this->GetHead();
	while (pTempNode)
    {
		pTempNextNode = (KNpcResNode *)pTempNode->GetNext();

        delete pTempNode;
        pTempNode = NULL;
        pTempNode = pTempNextNode;
	}
}

//-------------------------------------------------------------------------
//	���ܣ�	��ʼ��
//-------------------------------------------------------------------------
BOOL	KNpcResList::Init()
{
	if (!m_cActionName.Init(ACTION_FILE_NAME))
		return FALSE;
	if (!m_cNpcAction.Init(NPC_ACTION_NAME) )
		return FALSE;
	
	if (!m_cStateTable.Init() )
		return FALSE;
	if (!m_cMenuState.Init() )
		return FALSE;
	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ�	����ͬ���� NpcResNode �����ؽڵ�
//-------------------------------------------------------------------------
KNpcResNode*	KNpcResList::FindNpcRes(char *lpszNpcName)
{
	if ( !lpszNpcName || !lpszNpcName[0])
		return NULL;

	KNpcResNode		*pTempNode;

	pTempNode = (KNpcResNode *)this->GetHead();
	if (pTempNode == NULL)
		return NULL;
	if (strcmp(pTempNode->m_szNpcName, lpszNpcName) == 0)
	{
		return pTempNode;
	}

	while (1)
	{
		pTempNode = (KNpcResNode *)pTempNode->GetNext();
		if (pTempNode == NULL)
			return NULL;
		if (strcmp(pTempNode->m_szNpcName, lpszNpcName) == 0)
		{
			return pTempNode;
		}
	}

	return NULL;
}

//-------------------------------------------------------------------------
//	���ܣ�	���һ�� NpcResNode �����ؽڵ㣬����Ѵ��ڣ�ֱ�ӷ��ؽڵ�
//-------------------------------------------------------------------------
KNpcResNode*	KNpcResList::AddNpcRes(char *lpszNpcName)
{
	if ( !lpszNpcName || !lpszNpcName[0])
		return NULL;

	KNpcResNode		*pTempNode;
	pTempNode = FindNpcRes(lpszNpcName);  //�ҵ�����ļ�
	if (pTempNode !=NULL)
		return pTempNode;

	pTempNode = new KNpcResNode;
	if (!pTempNode->Init(lpszNpcName, &m_cActionName, &m_cNpcAction) )  //��ʼ������ļ�
	{
		delete pTempNode;
		pTempNode = NULL;
		return NULL;
	}
	this->AddTail(pTempNode);
	return pTempNode;
}


KNpcResNode*	KNpcResList::ClearOneNpcRes(char *lpszNpcName)
{
	if ( !lpszNpcName || !lpszNpcName[0])
		return NULL;

	KNpcResNode		*pTempNode;
	KNpcResNode		*pTempNextNode;
	
	// �ͷ����������еĽڵ�
	pTempNode = (KNpcResNode *)this->GetHead();
	while (pTempNode)
	{ 
		pTempNextNode = (KNpcResNode *)pTempNode->GetNext();

		if (strcmp(pTempNode->m_szNpcName, lpszNpcName) == 0)
		{
			delete pTempNode;
			pTempNode = NULL;
			break;
		}

		// pTempNode = pTempNextNode;		
	}

	return NULL;
} 


KNpcResNode*	KNpcResList::ClearAllNpcRes()
{
  KNpcResNode		*pTempNode;
  KNpcResNode		*pTempNextNode;

// �ͷ����������еĽڵ�
  pTempNode = (KNpcResNode *)this->GetHead();
  while (pTempNode)
  { 
	pTempNextNode = (KNpcResNode *)pTempNode->GetNext();
	
	delete pTempNode;
	pTempNode = NULL;
	pTempNode = pTempNextNode;
  }
   return NULL;
} 