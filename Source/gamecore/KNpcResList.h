//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcResList.h
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------

#pragma once

#include "engine/KList.h"
#include "KNpcResNode.h"
#include "KPlayerMenuState.h"

class KNpcResList : public KList
{
public:
	CActionName				m_cActionName;  //��Ϊ���� "������ű�.txt" Ӣ��
	CActionName				m_cNpcAction;   //�������� "npc������.txt"  Ӣ��
	CStateMagicTable		m_cStateTable;  //״̬��������Ч��Ӧ��
	KPlayerMenuStateGraph	m_cMenuState; //

private:

	// ����ͬ���� NpcResNode �����ؽڵ�
	KNpcResNode*		FindNpcRes(char *lpszNpcName);

public:
    KNpcResList();
    ~KNpcResList();

    // ��ʼ�� ActionName
	int				Init();
	// ����һ�� NpcResNode �����ؽڵ㣬����Ѵ��ڣ�ֱ�ӷ��ؽڵ�
	KNpcResNode*		AddNpcRes(char *lpszNpcName);
	KNpcResNode*	    ClearAllNpcRes();
	KNpcResNode*	    ClearOneNpcRes(char *lpszNpcName);
};

extern KNpcResList	g_NpcResList;
