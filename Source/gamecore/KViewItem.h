//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KViewItem.h
// Date:	2003.07.28
// Code:	�߳�����
// Desc:	KViewItem Class
//---------------------------------------------------------------------------

#ifndef _KVIEWITEM_H
#define _KVIEWITEM_H

class KViewItem
{
private:
	unsigned long		m_dwNpcID;				// �Է���ҵ� npc �� id
	int			m_nLevel;				// �Է���ҵĵȼ�
	char		m_szName[32];			// �Է���ҵ�����
    int	        m_nProcess;

public:
	PlayerItem	m_sItem[itempart_num];	// �Է���Ҵ������ϵ�װ���ڿͻ��� Item �����е�λ����Ϣ
	int			m_nIdx[itempart_num];
	int			m_nDorp[itempart_num];
	int			m_nId;

public:
	KViewItem();						// ����
	void		Init();					// ��ʼ��
	void		ApplyViewEquip(int npcIndex);
	void		UpdateItem(unsigned long dwNpcID);
	void		DeleteAll();
	void		DeleteNewAll(int nUpdate=0);
	void		GetData(BYTE* pMsg,int nUpdate = 0);
	void		Check(int nCount, unsigned long dwId);
	int			GetCount();
	int		    FindIdx(int uId);
};

extern	KViewItem	g_cViewItem;

#endif
