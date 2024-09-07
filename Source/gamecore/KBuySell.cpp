#include "engine/KEngine.h"
#include "KCore.h"
#include "engine/KTabFile.h"
#include "KPlayer.h"
#include "KNpc.h"
#include "KItemGenerator.h"
#include "KSubWorldSet.h"
#include "KItemSet.h"
#include "KBuySell.h"
#include "engine/Text.h" //ºóÀ´¼ÓµÄ
#include "CoreShell.h"
//#include "crtdbg.h"
#include "CoreUseNameDef.h"
//#ifdef _SERVER
//#include "../MultiServer/Heaven/Interface/iServer.h"
//#endif
KSubWorldSet   *M_SubWorldSetA;
#define	SHOP_BOX_WIDTH		6
#define	SHOP_BOX_HEIGHT		10
#define	SSHOP_BOX_WIDTH		12
#define	SSHOP_BOX_HEIGHT	10
KBuySell	BuySell;

KBuySell::KBuySell()
{
	m_pShopRoom = NULL;
	m_pSShopRoom = NULL;
	m_Item = NULL;
	m_SellItem = NULL;
	m_Width = 0;
	m_Height = 0;
	m_MaxItem = 0;
}

KBuySell::~KBuySell()
{
	if (m_pShopRoom)
	{
		delete m_pShopRoom;
		m_pShopRoom = NULL;
	}
	if (m_pSShopRoom)
	{
		delete m_pSShopRoom;
		m_pSShopRoom = NULL;
	}

	if (m_Item)
	{
		delete [] m_Item;
		m_Item = NULL;
	}
	if (m_SellItem)
	{
		for (int i = 0; i < m_Height; i++)
		{
			if (m_SellItem[i])
			{
				delete m_SellItem[i];
				m_SellItem[i] = NULL;
			}
		}
		delete m_SellItem;
		m_SellItem = NULL;
	}
	m_Width = 0;
	m_Height = 0;
	m_MaxItem = 0;
}
//³õÊ¼»¯ÉÌÆ·
int KBuySell::Init()
{
	KTabFile		GoodsFile;
	KTabFile		BuySellFile;
	m_paiItem = (KItem *)new KItem[250];   //·ÖÅäÄÚ´æ
//	g_SetFilePath("\\");
        if (!BuySellFile.Load((char*)BUYSELL_FILE) || !GoodsFile.Load((char*)GOODS_FILE))
		return FALSE;

	int nHeight = GoodsFile.GetHeight() - 1;
	int nWidth = GoodsFile.GetWidth() - 1;
	if (nWidth == 0 || nHeight == 0)
	{
		GoodsFile.Clear();
	    BuySellFile.Clear();
		return FALSE;
	}

	m_Item = (KItem *)new KItem[nHeight];   //·ÖÅäÄÚ´æ

	if (!m_Item)
	{
		GoodsFile.Clear();
	    BuySellFile.Clear();
		return FALSE;
	}

	ItemGenerate ItemGenerator;
	for (int k = 0; k < nHeight; ++k)
	{
		GoodsFile.GetInteger(k + 2, 1, -1, &ItemGenerator.nGenre); //ÖÖÀà
		GoodsFile.GetInteger(k + 2, 2, -1, &ItemGenerator.nDetailType); //¾ßÌåÀàÐÍ
		GoodsFile.GetInteger(k + 2, 3, -1, &ItemGenerator.nParticularType); //ÏêÏ¸Àà±ð
		GoodsFile.GetInteger(k + 2, 4, -1, &ItemGenerator.nSeriesReq); //ÎåÐÐ
		GoodsFile.GetInteger(k + 2, 5, 1, &ItemGenerator.nLevel);//µÈ¼¶
		GoodsFile.GetInteger(k + 2, 6, 0, &ItemGenerator.nPrice);//ÒøÁ½
		int nGolden = 0,nPriceType = 0,nEquipType=0,nIsBang=0;
		GoodsFile.GetInteger(k + 2, 7, 0, &ItemGenerator.nPriceXu);//½ð±Ò
		ItemGenerator.nPriceXu = ItemGenerator.nPrice;
		GoodsFile.GetInteger(k + 2, 8, 0, &nPriceType);            //»õ±ÒÀàÐÍ
		//GoodsFile.GetInteger(k + 2, 9, 0, &nJiFen);              //Í­Ç®
		GoodsFile.GetInteger(k + 2, 10, 0, &nEquipType);           //×°±¸ÀàÐÍ
		GoodsFile.GetInteger(k + 2, 21, 0,&nIsBang);	    	   //ÊÇ·ñ°ó¶¨

		int nItemMagLevel[6];

		if (nEquipType>0)
		{
		   GoodsFile.GetInteger(k + 2, 22, 0, &nItemMagLevel[0]);
		   GoodsFile.GetInteger(k + 2, 23, 0, &nItemMagLevel[1]);
		   GoodsFile.GetInteger(k + 2, 24, 0, &nItemMagLevel[2]);
		   GoodsFile.GetInteger(k + 2, 25, 0, &nItemMagLevel[3]);
		   GoodsFile.GetInteger(k + 2, 26, 0, &nItemMagLevel[4]);
		   GoodsFile.GetInteger(k + 2, 27, 0, &nItemMagLevel[5]);
		}
		else
		{
			nItemMagLevel[0]=0;
			nItemMagLevel[1]=0;
			nItemMagLevel[2]=0;
			nItemMagLevel[3]=0;
			nItemMagLevel[4]=0;
			nItemMagLevel[5]=0;
		}

		switch(ItemGenerator.nGenre)
		{
		case item_equip:  //·þÎñÆ÷ÉÌµêµÄ×°±¸²úÉú
			ItemGen.Gen_Equipment(ItemGenerator.nDetailType,\
				              ItemGenerator.nParticularType,\
				                   ItemGenerator.nSeriesReq,\
				                ItemGenerator.nLevel,nItemMagLevel,0,\
				             g_SubWorldSet.GetGameVersion(),\
				             &m_Item[k],0,nEquipType,0,0,TRUE,\
							 ItemGenerator.nPrice,ItemGenerator.nPriceXu,nIsBang,nPriceType);

			break;
		case item_medicine:  //Ò©Æ·
			ItemGen.Gen_MedicineShop(ItemGenerator.nDetailType,\
				ItemGenerator.nLevel, \
				g_SubWorldSet.GetGameVersion(),\
				&m_Item[k],1,ItemGenerator.nPrice,nIsBang,nPriceType);
			break;
		case item_mine:  // Ä¬ÈÏÊÇ-ÉÌµêÀà
			ItemGen.Gen_MineShop(ItemGenerator.nParticularType, &m_Item[k], 1,\
				                ItemGenerator.nLevel, ItemGenerator.nSeriesReq,\
								ItemGenerator.nDetailType,ItemGenerator.nPriceXu,\
								ItemGenerator.nPrice,nIsBang,nPriceType);
			break;
		case item_townportal:
			ItemGen.Gen_TownPortal(&m_Item[k]);
			break;
		case item_task:				// Ä¬ÈÏÊÇÈÎÎñÀà-ÉÌµêÀà
			ItemGen.Gen_QuestShop(ItemGenerator.nDetailType,&m_Item[k],1,ItemGenerator.nPriceXu,\
				               ItemGenerator.nPrice,ItemGenerator.nLevel,nIsBang,nPriceType);
			break;
		case item_fusion:			// ÎÆ¸Ö
			//ItemGen.Gen_QuestShop(ItemGenerator.nDetailType,&m_Item[k],1,ItemGenerator.nPriceXu,ItemGenerator.nPrice,ItemGenerator.nLevel);
			break;
		default:
			break;
		}


	//	m_Item->SetBang(nIsBang);


		m_MaxItem++;
	}

	m_Height = BuySellFile.GetHeight() - 1;
	m_Width  = BuySellFile.GetWidth();

	if (m_Width == 0 || m_Height == 0)
	{
		GoodsFile.Clear();
	    BuySellFile.Clear();
		return FALSE;
	}

	m_SellItem = (int **)new int*[m_Height];
	if (!m_SellItem)
	{
		GoodsFile.Clear();
	    BuySellFile.Clear();
		return FALSE;
	}

	for (int i = 0; i < m_Height; ++i)
	{
		m_SellItem[i] = NULL;
		m_SellItem[i] = (int *)new int[m_Width];
		if (!m_SellItem[i])
		{
			GoodsFile.Clear();
	        BuySellFile.Clear();
			return FALSE;
		}
		for (int j = 0; j < m_Width; ++j)
		{
			BuySellFile.GetInteger(i + 2, j + 1, -1, &m_SellItem[i][j]);//m_SellItem[0][0]  ¿ªÊ¼[ÉÌµêID][ÁÐ]
			if (m_SellItem[i][j] == -1)
			{
				continue;
			}

			//_ASSERT(m_SellItem[i][j] > 0);		// ²ß»®ÊÇ´Ó1¿ªÊ¼µÄ
			if (m_SellItem[i][j] > 0)
				m_SellItem[i][j] -= 1;			// ÎªÁË²ß»®´Ó1¿ªÊ¼Ìî±í
		}
	}
	if (!m_pShopRoom)
	{
		m_pShopRoom = new KInventory;  //ÈÝÆ÷ÄÚ´æ
		m_pShopRoom->Init(SHOP_BOX_WIDTH, SHOP_BOX_HEIGHT);
	}

	if (!m_pSShopRoom)
	{
		m_pSShopRoom = new KInventory;
		m_pSShopRoom->Init(SSHOP_BOX_WIDTH, SSHOP_BOX_HEIGHT);
	}

	//mItemPtr.clear();

	GoodsFile.Clear();
	BuySellFile.Clear();
	return TRUE;
}

KItem* KBuySell::GetItemDataByidx(int nIndex)
{
	if (nIndex < 0 || nIndex >= 250 || !m_paiItem)
		return NULL;

	if (m_paiItem[nIndex].GetIndex()>0)
	{//´æÔÚµÄ
		return &m_paiItem[nIndex];
	}
	return NULL;
}

KItem* KBuySell::GetItemidx(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_MaxItem || !m_Item)
		return NULL;

	return &m_Item[nIndex];
}

int KBuySell::GetItemIndex(int nShop, int nIndex)
{
	if (!m_SellItem || nShop < 0 || nShop >= m_Height || nIndex < 0 || nIndex >= m_Width)
		return -1;

	if (!m_SellItem[nShop])
		return -1;

	return m_SellItem[nShop][nIndex];
}

//ÉÌµêÃè»æ¿ÉµþÊýÁ¿
void KBuySell::PaintItem(int nIdx, int nX, int nY)
{
	int nShop = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
	if (nShop < 0 || nShop >= m_Height)
		return;
	int nItemIdx = GetItemIndex(nShop, nIdx);

	int x = nX;
	int y = nY;

	KItem* pItem = GetItemidx(nItemIdx);

	if (pItem)
	{
		pItem->Paint(x, y);  //Ãè»æ¿Éµþ·ÅÊýÁ¿
	}
}

void KBuySell::PaintItemPai(int nIdx, int nX, int nY,int nKind)
{
	int x = nX;
	int y = nY;

	KItem* pItem = GetItemDataByidx(nIdx);

	if (pItem)
	{
		pItem->Paint(x, y,TRUE,0,nKind);  //Ãè»æ¿Éµþ·ÅÊýÁ¿
	}
}


//ÆæÕä¸ó
void KBuySell::OpenCSuPerShop(int nSaleType, BuySellInfo *pInfo)
{
	if(nSaleType < 0 || nSaleType > 1)	 //0 »ò 1
		return;

	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.Clear();
//	Player[CLIENT_PLAYER_INDEX].m_NewBuyInfo.Clear();

	Player[CLIENT_PLAYER_INDEX].m_BuyInfo = *pInfo;
	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx = 0;
	CoreDataChanged(GDCNI_SUPERSHOP, (uintptr_t)pInfo, nSaleType);
}

void KBuySell::OpenCSale(int nShop)  //ÏµÍ³NPCÉÌµê
{
	if (nShop < 0 || nShop >= m_Height)
		return;
	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.Clear();
	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx = nShop;
	CoreDataChanged(GDCNI_NPC_TRADE,nShop,TRUE);   //Í¨Öª¿Í»§¶Ë½çÃæ´ò¿ªÂòÂôÃæ°å
}
//¿Í»§¶Ë´ò¿ªÉÌ³Ç
void KBuySell::OpenCMarket(int nShop) //ÆæÕä¸ó
{
	if (nShop < 0 || nShop >= m_Height)
		return;

	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.Clear();

	Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx =0;

	CoreDataChanged(GDCNI_OPEN_MARKET, nShop, TRUE);  //Í¨Öª¿Í»§¶Ë½çÃæ´ò¿ªÆæÕä¸ó
}

void  KBuySell::SetShopItemdata(BYTE* pMsg)
{//¸üÐÂÅÄÂôÐÐ
	ITEMDATA_SYNC_END *nSynInfo = (ITEMDATA_SYNC_END *)pMsg;
	//¿Í»§¶Ë¿ªÊ¼²úÉúÎïÆ·

	if  (!nSynInfo)
		return;

	if  (nSynInfo->iid<=0)
		return;

	//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("----Í¬²½³É¹¦----");
	//iItemDataInfo
	int nItemMagLevel[6],nRItemMagLevel[6];

	ZeroMemory(&nItemMagLevel,sizeof(nItemMagLevel));
	ZeroMemory(&nRItemMagLevel,sizeof(nRItemMagLevel));

	/*
	int iparam2;    //À¶×°
	int iparam3;
	int iparam5;
	int iparam4;
	int iparam6;
	int iparam1;
	int iparamr2;   //ÈÛÁ¶
	int iparamr3;
	int iparamr5;
	int iparamr4;
	int iparamr6;
	int iparamr1;
	*/
	nItemMagLevel[0] = nSynInfo->iparam1;
	nItemMagLevel[1] = nSynInfo->iparam2;
	nItemMagLevel[2] = nSynInfo->iparam3;
	nItemMagLevel[3] = nSynInfo->iparam4;
	nItemMagLevel[4] = nSynInfo->iparam5;
	nItemMagLevel[5] = nSynInfo->iparam6;

	nRItemMagLevel[0] = nSynInfo->iparamr1;
	nRItemMagLevel[1] = nSynInfo->iparamr2;
	nRItemMagLevel[2] = nSynInfo->iparamr3;
	nRItemMagLevel[3] = nSynInfo->iparamr4;
	nRItemMagLevel[4] = nSynInfo->iparamr5;
	nRItemMagLevel[5] = nSynInfo->iparamr6;
	/*
	IN int nDetailType,\
	IN int nParticularType,\
	IN int nSeriesReq,\
	IN int nLevel,\
	IN const int* pnaryMALevel,\
	IN int nLucky,\
	IN int nVersion,\
	IN OUT KItem* pItem, \
	IN int nEnChance,\
	IN int nPoint,\
	IN int nRongpoint,\
	IN int IsWhere,IN int nIsShop,\
	IN int nPrice,IN int nFsxu,int nIsBang,int nPriceType)

	*/
	ZeroMemory(&m_paiItem[nSynInfo->iid],sizeof(m_paiItem[nSynInfo->iid]));
	//m_paiItem[nSynInfo->iid].SetIndex(-1);

	//char nMsgInfo[128]={0};

	//sprintf(nMsgInfo,"----<color=gyellow>%u,²úÉúÎïÆ·:gen:%d de:%d np:$d lv:%d<color>----",nSynInfo->irandseed,nSynInfo->iequipgen,nSynInfo->idetailtype,nSynInfo->iparticulartype,nSynInfo->ilevel);
	//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nMsgInfo);

	switch(nSynInfo->iequipgen)
	{
	case item_equip:  //ÅÄÂôÐÐµÄ×°±¸²úÉú
		ItemGen.Gen_paiEquipment(nSynInfo->idetailtype,\
			nSynInfo->iparticulartype,\
			nSynInfo->iseries,\
			nSynInfo->ilevel,nItemMagLevel,nRItemMagLevel,\
			nSynInfo->irandseed,\
			&m_paiItem[nSynInfo->iid],nSynInfo->ienchance,nSynInfo->ipoint,nSynInfo->iRongpiont,nSynInfo->iswhere,TRUE,\
			nSynInfo->iSellPrice,nSynInfo->iSellPrice,0,nSynInfo->iPriceKind,nSynInfo->ilucky);

		m_paiItem[nSynInfo->iid].SetIndex(nSynInfo->iid);
		//sprintf(nMsgInfo,"----<color=gyellow>²úÉúÎïÆ·:%s %u<color>----",m_paiItem[nSynInfo->iid].GetName(),m_paiItem[nSynInfo->iid].GetItemParam()->uRandomSeed);
		//Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nMsgInfo);
		break;
	/*case item_medicine:
		ItemGen.Gen_MedicineShop(ItemGenerator.nDetailType,\
			ItemGenerator.nLevel, \
			g_SubWorldSet.GetGameVersion(),\
			&m_Item[k],1,ItemGenerator.nPriceXu,nIsBang,nPriceType);
		break;
	case item_mine:  // Ä¬ÈÏÊÇ-ÉÌµêÀà
		ItemGen.Gen_MineShop(ItemGenerator.nParticularType, &m_Item[k], 1,\
			ItemGenerator.nLevel, ItemGenerator.nSeriesReq,\
			ItemGenerator.nDetailType,ItemGenerator.nPriceXu,\
			ItemGenerator.nPrice,nIsBang,nPriceType);
		break;
	case item_townportal:
		ItemGen.Gen_TownPortal(&m_Item[k]);
		break;
	case item_task:				// Ä¬ÈÏÊÇÈÎÎñÀà-ÉÌµêÀà
		ItemGen.Gen_QuestShop(ItemGenerator.nDetailType,&m_Item[k],1,ItemGenerator.nPriceXu,\
			ItemGenerator.nPrice,ItemGenerator.nLevel,nIsBang,nPriceType);
		break;
	case item_fusion:			// ÎÆ¸Ö
		//ItemGen.Gen_QuestShop(ItemGenerator.nDetailType,&m_Item[k],1,ItemGenerator.nPriceXu,ItemGenerator.nPrice,ItemGenerator.nLevel);
		break;*/
	default:
		break;
	}


}
//ÉèÖÃ¿Í»§¶ËÉÌ³ÇÐÅÏ¢
void  KBuySell::SetShopItemInfo(BYTE* pMsg)
{
	  ITEM_SYNC_SHOP *nSynInfo = (ITEM_SYNC_SHOP *)pMsg;

	 /* char nSmg[64]={0};
	  sprintf(nSmg,"ÊÕµ½ÐÅÏ¢:%d,%d",nSynInfo->nShopIdx,nSynInfo->nBuyIdx);
	  Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nSmg);
	  */
	 /* if (nSynInfo->nShopIdx != Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx)
	  {//ÊÇÕâ¸öÉÌµê
		  return;
	  }
	  if (Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx<0)
	  {
		  return;
	  }*/

	  if (NULL==m_SellItem || nSynInfo->nBuyIdx > m_Width)
	  {
		  return;
	  }

	  if (m_SellItem[nSynInfo->nShopIdx][nSynInfo->nBuyIdx] < 0 || m_SellItem[nSynInfo->nShopIdx][nSynInfo->nBuyIdx] >= m_MaxItem)
	  {
		  return;
	  }

	  int nIdx = m_SellItem[nSynInfo->nShopIdx][nSynInfo->nBuyIdx];

	  if (nSynInfo->m_ItmeInfo[0])
		 m_Item[nIdx].SetItmeInfo(nSynInfo->m_ItmeInfo);
}
