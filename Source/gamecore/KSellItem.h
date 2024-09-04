//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KViewItem.h
// Date:	2003.07.28
// Code:	�߳�����
// Desc:	KViewItem Class
//---------------------------------------------------------------------------

class KSellItem
{
private:
	uint32_t	m_dwNpcID;			// �Է���ҵ� npc �� id
	char		m_szName[32];			// �Է���ҵ�����
	int			m_nProcess;
	int         m_nLProcess;
public:
	PlayerItem	m_sItem[60];	        // �Է���Ҵ������ϵ�װ���ڿͻ��� Item �����е�λ����Ϣ
	PlayerItem	m_sLItem[60];	        // �Է���Ҵ������ϵ�װ���ڿͻ��� Item �����е�λ����Ϣ
	int			m_nIdx[60];
	int			m_nLIdx[60];
	int			m_nId;
	int         m_nLId;
	int			m_nDorp[60];
	int			m_nLDorp[60];
	int         nMum;

public:
	KSellItem();						// ����
	void		Init();					// ��ʼ��
	void		ApplyViewItem(uint32_t dwNpcID);
	void		UpdateItem(uint32_t dwNpcID);
	void		UpdateLianItem(uint32_t dwNpcID,char * nName);
	int         GetLCount();
	void		Check(int nCount, uint32_t dwId);
	int			GetCount();
	void        CheckLian( int nCount, uint32_t dwId,char * nName);
	void		DeleteAll(int nUpdate = 0);
	void	    DeleteLAll(int nUpdate= 0);
	void		GetData(BYTE* pMsg ,int nUpdate = 0);
	int 	    GetDataL(BYTE* pMsg ,int nUpdate=0);
	int			FindIdx(int uId);
};

extern	KSellItem	g_cSellItem;


