//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KViewItem.cpp
// Date:	2003.07.28
// Code:	�߳�����---��̯ϵͳ
// Desc:	KViewItem Class
//---------------------------------------------------------------------------

#include	"KCore.h"

#include	"CoreShell.h"
#include    "KPlayer.h"
#include    "KPlayerSet.h"
#include	"KItemSet.h"
#include	"KItem.h"
#include	"KSellItem.h"
#include    "engine/Text.h"
KSellItem	g_cSellItem;

KSellItem::KSellItem()
{
	Init();
}

void	KSellItem::Init()
{
	m_dwNpcID	= 0;
	m_szName[0]	= 0;
	m_nProcess=0;
    m_nLProcess=0;
	nMum=0;
	memset(this->m_sItem, 0, sizeof(this->m_sItem));
	memset(this->m_sLItem, 0, sizeof(this->m_sLItem));
}

void	KSellItem::ApplyViewItem(unsigned int dwNpcID)
{
	VIEW_EQUIP_COMMAND	sView;
	sView.ProtocolType = c2s_viewitem;
	sView.m_dwNpcID    = dwNpcID;
	sView.m_bPrcess    = 0;
	if (g_pClient)
		g_pClient->SendPackToServer(&sView, sizeof(sView));
	m_nProcess = 0;
}
//������̯  ɾ����Ʒ
void	KSellItem::DeleteAll(int nUpdate)
{
	m_dwNpcID	= 0;
	m_szName[0]	= 0;
	m_nProcess = 0;
	m_nId = 0;
	ZeroMemory(m_nIdx,sizeof(m_nIdx));

	for (int i = 0; i < 60; ++i)
	{
		if (!nUpdate)
		{
			if (m_sItem[i].nIdx > 0)
				ItemSet.Remove(m_sItem[i].nIdx);  //ɾ������Ķ���
			m_nDorp[i] = 0;
		}
		else
		{
			if (m_nDorp[i])
				ItemSet.Remove(m_nDorp[i]);
			m_nDorp[i] = m_sItem[i].nIdx;
		}
		m_sItem[i].nIdx = 0;
		m_sItem[i].nPrice = 0;
		m_sItem[i].nX = 0;
		m_sItem[i].nY = 0;
		//m_sItem[i].nLianJieFlg = 0;
	}
}
//������̯  ɾ����Ʒ
void	KSellItem::DeleteLAll(int nUpdate)
{
	m_dwNpcID	= 0;
	m_szName[0]	= 0;
	m_nLProcess = 0;
	m_nLId = 0;
	ZeroMemory(m_nLIdx,sizeof(m_nLIdx));

	for (int i = 0; i < 60; ++i)
	{
		if (!nUpdate)
		{
			if (m_sLItem[i].nIdx > 0)
				ItemSet.Remove(m_sLItem[i].nIdx);  //ɾ������Ķ���

			m_nLDorp[i] = 0;
		}
		else
		{
			if (m_nLDorp[i])
				ItemSet.Remove(m_nLDorp[i]);

			m_nLDorp[i] = m_sLItem[i].nIdx;
		}

		m_sLItem[i].nIdx = 0;
		m_sLItem[i].nPrice = 0;
		m_sLItem[i].nX = 0;
		m_sLItem[i].nY = 0;
		m_sLItem[i].nLianJieFlg = 0;
		m_sLItem[i].nModel = 0;
	}
}

int		KSellItem::FindIdx(int uId)
{
	for (int i = 0; i < 60; ++i)
	{
		if (m_sItem[i].nIdx == uId)
		{
			return m_nIdx[i];
		}
	}
	return 0;
}

//�ͻ���������崦��
int  KSellItem::GetDataL(BYTE* pMsg ,int nUpdate)
{
	if (!pMsg)
		return FALSE;
	if (!nUpdate)
		DeleteLAll();
	else if (nUpdate)
		DeleteLAll(1);

	int	i,nItemIdx;
	VIEW_LIAN_ITEM_SYNC		*pItemSync = (VIEW_LIAN_ITEM_SYNC*)pMsg;
	KUiLianItem	             sLPlayer;
	memset(&sLPlayer, 0, sizeof(KUiLianItem));
	//m_dwNpcID       = pItemSync->m_dwNpcID;       //���鿴װ����DW��
	//sLPlayer.uId  = m_dwNpcID;               //�ָ�ֵ�� �ṹKUiPlayerItem  ����� Uid
	//nNpcIdx = NpcSet.SearchID(m_dwNpcID);    //��Ч(�����Ե�)
	//nNpcIdx         = NpcSet.SearchName(pItemSync->nShopName);
	//strcpy(sLPlayer.Name, pItemSync->nShopName);  //̯��������

	/*if (nNpcIdx > 0)
	{
		strcpy(m_szName, Npc[nNpcIdx].Name);
		//sLPlayer.nIndex = nNpcIdx;                       //̯��������
		//sLPlayer.nData = Npc[nNpcIdx].GetMenuState();    //̯����״̬
	} */

		if (pItemSync->m_ID == 0)
			return FALSE;

		int pnMagicParam[6],pnRMagicParam[6],pnJBParam[7],pnBMagicParam[6];

		for (i = 0; i < 6; ++i)
		{
			pnMagicParam[i]  = pItemSync->m_MagicLevel[i];
			pnRMagicParam[i] = pItemSync->m_RongMagicLev[i];
			pnBMagicParam[i] = 0;//pItemSync->m_BaoMagicLev[i];

		}
		for (i = 0; i < 7; ++i)
		{
			pnJBParam[i]     = pItemSync->m_JBLevel[i];
		}

		if (pItemSync->m_GoldId<=0)
			nItemIdx = ItemSet.AddCilentItem(\
			pItemSync->m_Genre,\
			pItemSync->m_Series,\
			pItemSync->m_Level,\
			pItemSync->m_Luck,\
			pItemSync->m_Detail,\
			pItemSync->m_Particur,\
			pnMagicParam,\
			pItemSync->m_Version,\
			pItemSync->m_RandomSeed,\
			pItemSync->m_StackNum,\
			pItemSync->m_EnChance,\
			pItemSync->m_Point,\
			pItemSync->m_Time.bYear,\
			pItemSync->m_Time.bMonth,\
			pItemSync->m_Time.bDay,\
			pItemSync->m_Time.bHour,\
			pItemSync->m_Time.bMin,\
			pItemSync->m_Rongpiont,\
			pnRMagicParam,\
			pnJBParam,\
			NULL,\
			NULL,\
			NULL,\
			NULL,pItemSync->m_IsWhere);
		else if (pItemSync->m_GoldId>0)
		{
			if (pItemSync->m_Point==8)
				nItemIdx = ItemSet.AddRondomGold(
				pItemSync->m_GoldId,\
				pnMagicParam,\
				pItemSync->m_Series,\
				pItemSync->m_EnChance,\
				pItemSync->m_Rongpiont,\
				pItemSync->m_Time.bYear,\
				pItemSync->m_Time.bMonth,\
				pItemSync->m_Time.bDay,\
				pItemSync->m_Time.bHour,\
				pItemSync->m_Time.bMin,\
				pnRMagicParam,\
				pItemSync->m_Level,pItemSync->m_Luck,pItemSync->m_Detail,\
				-1,1,0,pItemSync->m_IsBaijin,FALSE,pItemSync->m_RandomSeed);
			else

			nItemIdx = ItemSet.AddGold(\
			pItemSync->m_GoldId,\
			pnMagicParam,\
			pItemSync->m_Series,\
			pItemSync->m_EnChance,\
			pItemSync->m_Rongpiont,\
			pItemSync->m_Time.bYear,\
			pItemSync->m_Time.bMonth,\
			pItemSync->m_Time.bDay,\
			pItemSync->m_Time.bHour,\
			pItemSync->m_Time.bMin,\
			pnRMagicParam,\
			pnBMagicParam,pItemSync->m_Level,1,-1,-1,1,pItemSync->m_Version,pItemSync->m_IsBaijin,FALSE,pItemSync->m_RandomSeed);
		}
		if (nItemIdx <= 0)
			return FALSE;

		if (nItemIdx > 0)
		{//���ÿͻ��˸�����Ϣ
			Item[nItemIdx].SetStackNum(pItemSync->m_StackNum);         //������ �����ÿɵ�����
			/*pItemSync->m_IsBang     = Item[nItemIdx].GetIsBang();    //�Ƿ��
			pItemSync->m_IsKuaiJie  = Item[nItemIdx].GetIsKuaiJie();   //�Ƿ�����
			pItemSync->m_IsMagic    = Item[nItemIdx].GetIsMagic();
			pItemSync->m_SkillType  = Item[nItemIdx].GetSkillType();   //
			pItemSync->m_MagicID    = Item[nItemIdx].GetMagicID(); */
			Item[nItemIdx].SetUseMapIdx(pItemSync->m_UseMap);

			Item[nItemIdx].SetUseKind(pItemSync->m_UseKind);

			Item[nItemIdx].SetBang(pItemSync->m_IsBang);
			Item[nItemIdx].SetIsCanUse(pItemSync->m_IsCanUse); //�ͻ��������Ƿ����ʹ��
			if (pItemSync->m_ItmeInfo[0])
			{
				Item[nItemIdx].SetItmeInfo(pItemSync->m_ItmeInfo);
			}
			//sprintf(pItemSync->m_WonName,"%s",Item[nIndex].GetWonName());  //������
			Item[nItemIdx].SetWonName(pItemSync->m_WonName);//pItemSync->m_WonName
			Item[nItemIdx].SetID(pItemSync->m_ID);  //����DwID

			sLPlayer.nIndex = nItemIdx;
			sLPlayer.uId    = pItemSync->m_ID;
			sLPlayer.w      = Item[nItemIdx].GetWidth();
			sLPlayer.h      = Item[nItemIdx].GetHeight();
			sprintf(sLPlayer.Name, "%s", Item[nItemIdx].GetName());

		    Item[nItemIdx].SetDurability((short)pItemSync->m_Durability);  // ͬ���־ö�
			Item[nItemIdx].SetLianjieFlg(pItemSync->m_LianJieFlg);         //���ü۸�
            Item[nItemIdx].SetPrice(0);                                    //���ü۸�
				m_sLItem[0].nIdx   = /*pView->m_sInfo[i].m_nIdx*/nItemIdx;
				m_sLItem[0].nLianJieFlg = pItemSync->m_LianJieFlg;
				m_sLItem[0].nX     = pItemSync->m_btX;
				m_sLItem[0].nY     = pItemSync->m_btY;
                m_sLItem[0].nPrice = 0;
			   //m_nLIdx[0]        = pLView->m_sLInfo.m_nIdx;
        //���洦��
		//m_ItemList.ClientShowMsg(Item[nItemIdx].GetName());
		}
	CoreDataChanged(GDCNI_VIEW_LIAN_LITEM, (uintptr_t)&sLPlayer, 0);  //��������Ʒ���
    return TRUE;
}
//�ͻ���
void KSellItem::GetData(BYTE* pMsg ,int nUpdate)
{
	if (!pMsg)
		return;

	if (!m_nProcess && !nUpdate)
		DeleteAll();
	else if (!m_nProcess && nUpdate)
		DeleteAll(1);

	int	i, j;
	int	nNpcIdx, nItemIdx;
	VIEW_ITEM_SYNC	*pItemSync = (VIEW_ITEM_SYNC*)pMsg;
	KUiPlayerItem	sPlayer;

	m_dwNpcID = pItemSync->m_dwNpcID;
	sPlayer.uId = m_dwNpcID;
	nNpcIdx = NpcSet.SearchID(m_dwNpcID);
	if (nNpcIdx > 0)
	{
		strcpy(m_szName, Npc[nNpcIdx].Name);
		sPlayer.nIndex = nNpcIdx;
		strcpy(sPlayer.Name, m_szName);
		sPlayer.nShopType=0;
		sPlayer.nData = Npc[nNpcIdx].GetMenuState();
	}
	else
	{
		sPlayer.nIndex = 0;
		sPlayer.Name[0] = 0;
		sPlayer.nData = 0;
	}

	for (i = 0;i<6;++i)
	{
		if (m_nProcess != m_nId)
		{
			m_nProcess = 60;
			break;
		}

		m_nProcess++; //0123

		if (pItemSync->m_sInfo[i].m_ID == 0)
			continue;
        //-------------------------------------------------------------
		int pnMagicParam[6],pnRMagicParam[6],pnBMagicParam[6],pnJBParam[7];

		for (j = 0; j < 6; ++j)
		{
			pnMagicParam[j]  = pItemSync->m_sInfo[i].m_MagicLevel[j];
			pnRMagicParam[j] = pItemSync->m_sInfo[i].m_RongMagicLev[j];
			pnBMagicParam[j] = 0;//pItemSync->m_sInfo[i].m_BaoMagicLev[j];

		}
		for (j = 0; j < 7; ++j)
		{
			pnJBParam[j] = 0;//pItemSync->m_sInfo[i].m_JBLevel[j];
		}

		if (pItemSync->m_sInfo[i].m_GoldId<=0)
			nItemIdx = ItemSet.AddCilentItem(\
			pItemSync->m_sInfo[i].m_Genre,\
			pItemSync->m_sInfo[i].m_Series,\
			pItemSync->m_sInfo[i].m_Level,\
			pItemSync->m_sInfo[i].m_Luck,\
			pItemSync->m_sInfo[i].m_Detail,\
			pItemSync->m_sInfo[i].m_Particur,\
			pnMagicParam,\
			pItemSync->m_sInfo[i].m_Version,\
			pItemSync->m_sInfo[i].m_RandomSeed,\
			pItemSync->m_sInfo[i].m_StackNum,\
			pItemSync->m_sInfo[i].m_EnChance,\
			pItemSync->m_sInfo[i].m_Point,\
			pItemSync->m_sInfo[i].m_Time.bYear,\
			pItemSync->m_sInfo[i].m_Time.bMonth,\
			pItemSync->m_sInfo[i].m_Time.bDay,\
			pItemSync->m_sInfo[i].m_Time.bHour,\
			pItemSync->m_sInfo[i].m_Time.bMin,\
			pItemSync->m_sInfo[i].m_Rongpiont,\
			pnRMagicParam,\
			pnJBParam,\
			NULL,\
			NULL,\
			NULL,\
			NULL,pItemSync->m_sInfo[i].m_IsWhere);
		else if (pItemSync->m_sInfo[i].m_GoldId>0)
		{
			if (pItemSync->m_sInfo[i].m_Point==8)
				nItemIdx = ItemSet.AddRondomGold(
				pItemSync->m_sInfo[i].m_GoldId,\
				pnMagicParam,\
				pItemSync->m_sInfo[i].m_Series,\
				pItemSync->m_sInfo[i].m_EnChance,\
				pItemSync->m_sInfo[i].m_Rongpiont,\
				pItemSync->m_sInfo[i].m_Time.bYear,\
				pItemSync->m_sInfo[i].m_Time.bMonth,\
				pItemSync->m_sInfo[i].m_Time.bDay,\
				pItemSync->m_sInfo[i].m_Time.bHour,\
				pItemSync->m_sInfo[i].m_Time.bMin,\
				pnRMagicParam,\
	            pItemSync->m_sInfo[i].m_Level,pItemSync->m_sInfo[i].m_Luck,pItemSync->m_sInfo[i].m_Detail,\
				-1,1,0,pItemSync->m_sInfo[i].m_IsBaiJin,FALSE,pItemSync->m_sInfo[i].m_RandomSeed);
			else
			nItemIdx = ItemSet.AddGold(\
			pItemSync->m_sInfo[i].m_GoldId,\
			pnMagicParam,\
			pItemSync->m_sInfo[i].m_Series,\
			pItemSync->m_sInfo[i].m_EnChance,\
			pItemSync->m_sInfo[i].m_Rongpiont,\
			pItemSync->m_sInfo[i].m_Time.bYear,\
			pItemSync->m_sInfo[i].m_Time.bMonth,\
			pItemSync->m_sInfo[i].m_Time.bDay,\
			pItemSync->m_sInfo[i].m_Time.bHour,\
			pItemSync->m_sInfo[i].m_Time.bMin,\
			pnRMagicParam,\
			pnBMagicParam,pItemSync->m_sInfo[i].m_Level,1,-1,-1,1,0,pItemSync->m_sInfo[i].m_IsBaiJin,FALSE,pItemSync->m_sInfo[i].m_RandomSeed);
		}
		if (nItemIdx <= 0)
			continue;

		//if (nItemIdx > 0)
		//���ÿͻ��˸�����Ϣ
			Item[nItemIdx].SetStackNum(pItemSync->m_sInfo[i].m_StackNum);         //������ �����ÿɵ�����
			//pItemSync->m_sInfo[i].m_IsBang     = Item[nItemIdx].GetIsBang();      //�Ƿ��
			//pItemSync->m_sInfo[i].m_IsKuaiJie  = Item[nItemIdx].GetIsKuaiJie();   //�Ƿ�����
			//pItemSync->m_sInfo[i].m_IsMagic    = Item[nItemIdx].GetIsMagic();
			//pItemSync->m_sInfo[i].m_SkillType  = Item[nItemIdx].GetSkillType();
			//pItemSync->m_sInfo[i].m_MagicID    = Item[nItemIdx].GetMagicID();
			Item[nItemIdx].SetIsCanUse(pItemSync->m_sInfo[i].m_IsCanUse); //�ͻ��������Ƿ����ʹ��
		    Item[nItemIdx].SetModel(pItemSync->m_sInfo[i].m_Model);
			/*if (strrchr(Item[nIndex].GetItmeInfo(),'.lua'))
			{
				Item[nItemIdx].SetItmeInfo(pItemSync->m_ItmeInfo);
			}*/

			//sprintf(pItemSync->m_WonName,"%s",Item[nIndex].GetWonName());  //������
			//Item[nItemIdx].SetBang(pItemSync->m_sInfo[i].m_IsBang);
			Item[nItemIdx].SetWonName("ϵͳ");//pItemSync->m_sInfo[i].m_WonName
		    //Item[nItemIdx].SetDurability((short)pItemSync->m_sInfo[i].m_Durability);  // ͬ���־ö�

			Item[nItemIdx].SetID(pItemSync->m_sInfo[i].m_ID);
			Item[nItemIdx].SetPrice(pItemSync->m_sInfo[i].m_uPrice);
            //Item[nItemIdx].SetItmeInfo(pView->m_sInfo[i].m_DescInfo);
            //-----------------------------------------------------------��¼��Ʒ���������۸������е�λ��
				m_sItem[m_nId].nIdx   = nItemIdx;
				m_sItem[m_nId].nPrice = pItemSync->m_sInfo[i].m_uPrice;
				m_sItem[m_nId].nX     = pItemSync->m_sInfo[i].m_btX;
				m_sItem[m_nId].nY     = pItemSync->m_sInfo[i].m_btY;
                m_sItem[m_nId].nModel = pItemSync->m_sInfo[i].m_Model;
				m_nIdx[m_nId]         = pItemSync->m_sInfo[i].m_nIdx;
		        m_nId++;
	}

	if (m_nProcess < 60 && !nUpdate)  //ʱ�䣿
	{
		VIEW_EQUIP_COMMAND	sView;
		sView.ProtocolType = c2s_viewitem;
		sView.m_dwNpcID = pItemSync->m_dwNpcID;
		sView.m_bPrcess = (BYTE)m_nProcess;
		if (g_pClient)
			g_pClient->SendPackToServer(&sView, sizeof(sView));
		return;
	}
	else if (m_nProcess < 60 && nUpdate)
	{
		VIEW_EQUIP_COMMAND	sView;
		sView.ProtocolType = c2s_updateviewitem;
		sView.m_dwNpcID = pItemSync->m_dwNpcID;
		sView.m_bPrcess = (BYTE)m_nProcess;
		if (g_pClient)
			g_pClient->SendPackToServer(&sView, sizeof(sView));
		return;
	}
	// ֪ͨ����
	if (!nUpdate)
		CoreDataChanged(GDCNI_TASK_INFO, (uintptr_t)&sPlayer,0);  //�򿪰�̯��Ʒ���
	else
		CoreDataChanged(GDCNI_VIEW_PLAYERUPDATEITEM, 0,0);
}
//���͸�����Ʒ
void KSellItem::UpdateItem( unsigned int dwNpcID )
{
	if (m_nProcess >= 60)
	{
		VIEW_EQUIP_COMMAND	sView;
		sView.ProtocolType = c2s_updateviewitem;
		sView.m_dwNpcID = dwNpcID;
		sView.m_bPrcess = 0;
		if (g_pClient)
			g_pClient->SendPackToServer(&sView, sizeof(sView));
		m_nProcess = 0;
	}
}

//�ͻ���---������ --���½���
void KSellItem::UpdateLianItem( unsigned int dwNpcID,char *nName)
{
/*
	if ( m_nLProcess>= 60)
	{
		VIEW_LIANJIE_COMMAND	sView;
		sView.ProtocolType = c2s_updatelianitem;
		sView.m_dwNpcID = dwNpcID;
		sprintf(sView.m_PLname,nName);
		sView.m_bLPrcess = 0;
		if (g_pClient)
			g_pClient->SendPackToServer(&sView, sizeof(sView));
		m_nLProcess = 0;
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, "<color=yellow>�������ӳɹ�:DW:%d,����:%s<color>",sView.m_dwNpcID,sView.m_PLname);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
	    CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	}*/
}

int KSellItem::GetCount()
{
	if (m_nProcess < 60)
	{
		return -1;
	}
	else
	{
		return m_nId;
	}
}

int KSellItem::GetLCount()
{
//	if (m_nLProcess < 60)
//	{
//		return -1;
//	}
//	else
	{
		return nMum;
	}
}

void KSellItem::Check( int nCount, unsigned int dwId )
{
	if (nCount == 0)
		CoreDataChanged(GDCNI_CLOSE_BAITAN, 0 ,0);  //�رտ�̯����״̬

	if (nCount >0 && nCount != m_nId)
	{
		UpdateItem(dwId);
	}
}
//�ͻ���
void KSellItem::CheckLian( int nCount, unsigned int dwId ,char * nName)
{
	if (nCount == 0)
	{
//		CoreDataChanged(GDCNI_CLOSE_BAITAN, 0 ,1);  // ���û�ж��� �ر� ���
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, "����:�������Ѿ�ʧЧ");
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		sMsg.nMsgLen = TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
	    CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
	}
	if (nCount!=m_nLId)  //��������� ��ִ�и���
	{
		UpdateLianItem(dwId,nName);
	}
	else
	{

	}
}













