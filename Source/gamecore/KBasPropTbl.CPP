//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KBasPropTbl.CPP
// Date:	2002.08.14
// Code:	DongBo
// Desc:    cpp file. ±¾ÎÄ¼þÊµÏÖµÄÀàÓÃÓÚ´Ótab fileÖÐ¶Á³öµÀ¾ßµÄ³õÊ¼ÊôÐÔ,
//			²¢Éú³É¶ÔÓ¦µÄÊôÐÔ±í
//---------------------------------------------------------------------------
#include "KCore.h"
#include "engine/KTabFile.h"
//#include "MyAssert.H"
#include "KBasPropTbl.h"
#include "KNpcTemplate.h"  //ºóÀ´¼ÓµÄ

#define		TABFILE_PATH				"\\settings\\item\\004"
#define		TABFILE_MINE				"mine.txt"
#define		TABFILE_TASK				"questkey.txt"
#define		TABFILE_FUSION				"fusion.txt"    //ÎÆ¸Ù
#define		TABFILE_MEDICINE			"potion.txt"
#define		TABFILE_MEDMATERIAL			"medmaterialbase.txt"
#define		TABFILE_MELEEWEAPON			"MeleeWeapon.txt"
#define		TABFILE_RANGEWEAPON			"rangeweapon.txt"
#define		TABFILE_ARMOR				"Armor.txt"   //ÒÂ·þ
#define		TABFILE_HELM				"Helm.txt"    //Ã±×Ó
#define		TABFILE_BOOT				"Boot.txt"
#define		TABFILE_BELT				"Belt.txt"
#define		TABFILE_AMULET				"Amulet.txt"
#define		TABFILE_RING				"Ring.txt"
#define		TABFILE_CUFF				"Cuff.txt"
#define		TABFILE_PENDANT				"Pendant.txt"
#define		TABFILE_HORSE				"Horse.txt"
#define		TABFILE_TOWNPORTAL			"TownPortal.txt"
#define		TABFILE_EQUIPMENT_UNIQUE	"unique.txt"      //À¶×°£¿
#define		TABFILE_MAGICATTRIB			"magicattrib.txt" //Ä§·¨ÊôÐÔ
#define		TABFILE_GOLDITEM			"GoldItem.txt"
#define		TABFILE_MASK				"Mask.txt"	    // mat na
#define		TABFILE_PIFENG				"Pifeng.txt"	// Åû·ç
#define		TABFILE_YINJIAAN			"Yinjian.txt"	// Ó¡¼ø
#define		TABFILE_SHIPIN				"Shipin.txt"	// ÊÎÆ·
#define		TABFILE_PLATINA				"platinaequip.txt" // °×½ð

// ÒÔÏÂ¶¨ÒåµÄ½á¹¹ÓÃÓÚ¸¨Öú´ÓtabfileÖÐ¶Á³öÊôÐÔµÄ³õÊ¼Öµ
typedef struct tagPROPINFO
{
	int		m_nType;		// ÊôÐÔµÄÀàÐÍ. Ïê¼û PI_VARTYPE_...ÏµÁÐ¶¨Òå
	union
	{
	char*	m_pszBuf;		// Ö¸Ïò×Ö·û´®»º³åÇøµÄÖ¸Õë
	int*	m_pnData;		// Ö¸Ïòint±äÁ¿µÄÖ¸Õë
	}m_pData;
	int		m_nBufSize;		// »º³åÇøµÄ³¤¶È
} PROPINFO;
#define		PI_VARTYPE_CHAR		0
#define		PI_VARTYPE_INT		1
//×°±¸Çåµ¥
const char*	TABFILE_EQUIPMENT[] =
{
	TABFILE_MELEEWEAPON,	//"MeleeWeapon.txt",
	TABFILE_RANGEWEAPON,	//"RangeWeapon.txt",
	TABFILE_ARMOR,			//"Armor.txt",
	TABFILE_HELM,			//"Helm.txt",
	TABFILE_BOOT,			//"Boot.txt",
	TABFILE_BELT,			//"Belt.txt",
	TABFILE_AMULET,			//"Amulet.txt",
	TABFILE_RING,			//"Ring.txt",
	TABFILE_CUFF,			//"Cuff.txt",
	TABFILE_PENDANT,		//"Pendant.txt",
	TABFILE_HORSE,			//"Horse.txt",
	TABFILE_MASK,			//"Mask.txt"
	TABFILE_PIFENG,         //"Pifeng.txt"
	TABFILE_YINJIAAN,
	TABFILE_SHIPIN,
};

//int GetRandomNumber(int nMin, int nMax);

//=============================================================================

/******************************************************************************
	¹¦ÄÜ:	´Ótab fileÖÐ¶ÁÈëÌØ¶¨µÄÊý¾Ý¼ÇÂ¼
	Èë¿Ú:	pTF: ¹¤¾ßÀàÖ¸Õë, ÓÃ´Ë¹¤¾ßÀà¶ÁÈ¡tab file
			nRow: ¶ÁµÚnRowÏî¼ÇÂ¼
			pPI[i].m_nType: ¸ø³öÓû¶ÁµÄ¼ÇÂ¼ÖÐµÚi¸öÓòµÄÀàÐÍ, ¿ÉÄÜÊÇÕûÐÍ»ò×Ö·û´®
			pPI[i].m_pData: ½«Êý¾Ý¶Áµ½´Ë»º³åÇøÖÐ
			cbFields: Ã¿Ïî¼ÇÂ¼°üº¬ÕâÃ´¶àµÄÓò
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, m_pBuf Ö¸Ïò·ÖÅäµÄÄÚ´æ
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
int LoadRecord(IN KTabFile* pTF, IN int nRow,
				IN OUT const PROPINFO* pPI, IN int cbFields)
{
	int bEC = TRUE;

	nRow += 2;	// ¼Ó1: Ìø¹ýtabfileµÄµÚÒ»ÐÐ. ¸ÃÐÐ¸ø³öµÄÊÇ¸÷ÁÐµÄÃû³Æ
				// ÔÙ¼Ó1: KTabFile::GetInteger()º¯ÊýÒªÇónRow´Ó1¿ªÊ¼ËãÆð

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	for (int n = 0; n < cbFields; ++n)
	{
		if (PI_VARTYPE_INT == (pPI+n)->m_nType)
		{	// ¶ÁÈë int ÐÍÊý¾Ý
			if (FALSE == pTF->GetInteger(nRow, n+1, -1, (pPI+n)->m_pData.m_pnData))
			{
				bEC = FALSE;
				break;
			}
				//{ _ASSERT(FALSE); bEC = FALSE; break; }
		}
		else
		{	// ¶ÁÈë×Ö·û´®ÐÍÊý¾Ý
			//_ASSERT(PI_VARTYPE_CHAR == (pPI+n)->m_nType);
            if  (PI_VARTYPE_CHAR != (pPI+n)->m_nType)
			{
				bEC = FALSE;
			      break;
			}
			if (FALSE == pTF->GetString(nRow, n+1, (char*)"",(pPI+n)->m_pData.m_pszBuf,(pPI+n)->m_nBufSize)) //(pPI+n)->m_pData.m_pszBuf ,(pPI+n)->m_nBufSize)
			{
			     //_ASSERT(FALSE);
			      bEC = FALSE;
			      break;
			}
		}
	}
	return bEC;
}

//=============================================================================

KLibOfBPT::KLibOfBPT()
{
}

KLibOfBPT::~KLibOfBPT()
{
}

/******************************************************************************
	¹¦ÄÜ£º	×Ü¿Ø£¬´Ótab fileÖÐ¶ÁÈëµÀ¾ß,Ä§·¨µÈÔ­Ê¼Êý¾Ý
	³ö¿Ú:	Ïà¹ØÊý¾Ý´æÔÚm_BPTWeapon,m_BPTWeaponDirtyµÈ³ÉÔ±±äÁ¿ÖÐ
******************************************************************************/
int KLibOfBPT::Init()
{
	// ³õÊ¼»¯
	KBasicPropertyTable*	paryBPT[] = {
											&m_BPTMeleeWeapon,  //0 ÎäÆ÷
											&m_BPTRangeWeapon,  //°µÆ÷
											&m_BPTArmor,   //ÒÂ·þ
											&m_BPTHelm,    //Ã±×Ó
											&m_BPTBoot,    //Ñ¥×Ó
											&m_BPTBelt,    //Ñü´ø
											&m_BPTAmulet,  //
											&m_BPTRing,    //½ä×Ó
											&m_BPTCuff,    //ÊÖïí
											&m_BPTPendant, //ÓñÅå
											&m_BPTHorse,   //ÂíÆ¥
											&m_BPTMask,	   //11
											&m_BPTPifeng,  //12
											&m_BPTYinjian, //13
											&m_BPTShipin,  //14
											&m_GoldItem,   //15 »Æ½ð×°
											&m_BPTMedicine,//16Ò©Æ·
											&m_BPTQuest,   //17ÎïÆ·
											&m_BPTTownPortal, //18»Ø³Ç
											&m_BPTMine,       //19ÎïÆ·
											&m_BPTMagicAttrib,//20Ä§·¨ÊôÐÔ±í
											&m_BPTFusion,     //21ÎÆ¸Ù
											&m_Platina,       //22°×½ð
										};
	// ½«tab fileÖð¸ö¶ÁÈë
	const int cbNumOfTables = sizeof(paryBPT)/sizeof(paryBPT[0]);

	for (int i = 0; i < cbNumOfTables; ++i)
	{
		if (i < 15)	         //×°±¸ÌØÊâ´¦Àí  °××°
		{
			KBPT_Equipment* pTemp = (KBPT_Equipment *)paryBPT[i];
			pTemp->Init(i);  //³õÊ¼»¯ÎÄ¼þÃû¸³Öµ
		}
		else if (i == 15)    //»Æ½ð
		{
			KBPT_Equipment_Gold* pGTemp = (KBPT_Equipment_Gold *)paryBPT[i];
			pGTemp->Init();
		}
		else if (i ==22)	 //°×½ð
		{
			KBPT_Equipment_Platina* pPTemp = (KBPT_Equipment_Platina *)paryBPT[i];
			pPTemp->Init();
		}

		if (FALSE == paryBPT[i]->Load())  //¼ÓÔØ¿Í»§¶Ë×°±¸ÎïÆ·ÎÄ¼þ£¨³ö´í µ¼ÖÂ¿Í»§¶ËÉÁÍË£©
		{
			//_ASSERT(FALSE);
		    return FALSE;
		}
	}

	// ¹¹ÔìÄ§·¨ÊôÐÔ±í
	InitMALib();

    // ¹¹ÔìÄ§·¨ÊôÐÔË÷Òý±í
    // add by Freeway Chen in 2003.5.30
    InitMAIT();

	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ£º	½¨Á¢Ò»¸öËÄÎ¬µÄÊý×é£¬ÀûÓÃ(Ç°ºó×º¡¢ÎïÆ·ÀàÐÍ¡¢ÎåÐÐ¡¢¼¶±ð)
            È·¶¨Ò»¸öÄ§·¨ÊôÐÔµÄË÷ÒýÖµÁÐ±í
	Èë¿Ú:	m_BPTMagicAttrib: ÄÚº¬´Ótab file¶ÁÈëµÄÈ«²¿Ä§·¨ÊôÐÔ
	³ö¿Ú:	ËÄÎ¬Êý×é m_CMAIT ·Ö±ðÌî³äÏàÓ¦µÄË÷ÒýÖµÁÐ±í
******************************************************************************/
// Add by Freeway Chen in 2003.5.30
int KLibOfBPT::InitMAIT()
{
    int nResult = false;
    int i = 0;
    int nPrefixPostfix = 0;
    int nType = 0;
    int nSeries = 0;
    int nSeriesMin = 0;
    int nSeriesMax = 0;

    int nLevel  = 0;
    int nLevelMin = 0;
    int nLevelMax = 0;

    int nTotalCount = 0;

    for (i = 0; i < m_BPTMagicAttrib.NumOfEntries(); ++i)
    {
        KMAGICATTRIB_TABFILE *pItem = (KMAGICATTRIB_TABFILE *)m_BPTMagicAttrib.GetRecord(i);
        if (!pItem)
        {
            //_ASSERT(pItem);
            continue;
        }

        pItem->m_nUseFlag = false;

        nPrefixPostfix = pItem->m_nPos;

       // _ASSERT((nPrefixPostfix >= 0) && (nPrefixPostfix < MATF_PREFIXPOSFIX));
        if  (nPrefixPostfix< 0 || nPrefixPostfix>=MATF_PREFIXPOSFIX)
		   continue;

        for (nType = 0; nType < MATF_CBDR; ++nType)   //×î´óµÄ¿É³öÏÖµÄ²¿Î»ÏÞÖÆ
        {
            if ((pItem->m_DropRate[nType]) <= 1)      //¸ÅÂÊÐ¡ÓÚ1µÄ½«²»»áÌí¼ÓÊôÐÔ
                continue; // Èç¹ûÃ»ÓÐ¸ÅÂÊ³öÏÖÕâ¸öÀàÐÍ¾ÍÌøµ½ÏÂÒ»¸ö

            nSeriesMin = nSeriesMax = pItem->m_nClass;  // ÎåÐÐ

            if ((pItem->m_nClass) == -1)
            {
                nSeriesMin = 0;
                nSeriesMax = MATF_SERIES - 1;
            }
            else
            {
                //_ASSERT(((pItem->m_nClass) >= 0) && ((pItem->m_nClass) < MATF_SERIES));
                if ((pItem->m_nClass) <0 || (pItem->m_nClass) >= MATF_SERIES)
          		  continue;
            }

            for (nSeries = nSeriesMin; nSeries <= nSeriesMax; ++nSeries)
            {
                nLevelMin = pItem->m_nLevel;
                nLevelMax = MATF_LEVEL;

                for (nLevel = nLevelMin; nLevel <= nLevelMax; ++nLevel)
                {
                    m_CMAIT[nPrefixPostfix][nType][nSeries][nLevel - 1].Insert(i);
                    nTotalCount++;
                }
            }
        }
    }

    //#ifdef _DEBUG
 /*   #if 0
    //g_DebugLog("[Ä§·¨ÊôÐÔ]%sÎå·À¿¹ÐÔÉÏÏÞÔö¼Ó%d", pNpc->Name, pMagic->nValue[0]);
    printf("[Ä§·¨ÊôÐÔ]m_CMAIT[Ç°×ººó×º][ÀàÐÍ][ÎåÐÐ][¼¶±ð]\n");
    for (nPrefixPostfix = 0; nPrefixPostfix < MATF_PREFIXPOSFIX; nPrefixPostfix++)
    {
        for (nType = 0; nType < MATF_CBDR; nType++)
        {
            for (nSeries = 0; nSeries < MATF_SERIES; nSeries++)
            {
                for (nLevel = 1; nLevel < (MATF_LEVEL + 1); nLevel++) //×î¸ß¼¶±ð Îª10
                {
                    KBPT_ClassMAIT *pMAITItem = &m_CMAIT[nPrefixPostfix][nType][nSeries][nLevel - 1];

                    char szOutputString[8192];
                    char szStringContent[4096];
                    szStringContent[0] = '\0';

                    for (i = 0; i < pMAITItem->GetCount(); ++i)
                    {
                        KMAGICATTRIB_TABFILE *pItem = (KMAGICATTRIB_TABFILE *)m_BPTMagicAttrib.GetRecord(pMAITItem->Get(i));
                        char szTemp[1024];
                        sprintf(szTemp, " %3d(%-8s) ", pMAITItem->Get(i) + 2, pItem->m_szName);

                        strcat(szStringContent, szTemp);
                    }

                    sprintf(szOutputString,
                        "[Ä§·¨ÊôÐÔ]m_CMAIT[%d][%d][%d][%d]: Count = %3d, %s  \n",
                        nPrefixPostfix,
                        nType,
                        nSeries,
                        nLevel - 1,
                        pMAITItem->GetCount(),
                        szStringContent
                    );
                    printf("%s", szOutputString);
                    //g_DebugLog("%s", szOutputString);
                    //OutputDebugString(szOutputString);

                }
            }
        }
    }
    ExitProcess(0); // for redirect to File save

    #endif // _DEBUG
*/

    nResult = true;
//Exit0:
    return nResult;
}


/******************************************************************************
	¹¦ÄÜ£º	¸ù¾ÝÔ­Ê¼µÄÄ§·¨ÊôÐÔ±í£¬Í³¼Æ³öÃ¿ÖÖ×°±¸¸÷ÓÐÄÄÐ©¿ÉÄÜµÄÄ§·¨ÊôÐÔ
	Èë¿Ú:	m_BPTMagicAttrib: ÄÚº¬´Ótab file¶ÁÈëµÄÈ«²¿Ä§·¨ÊôÐÔ
	³ö¿Ú:	Ô­Ê¼µÄÄ§·¨ÊôÐÔ±í±»·ÖÀà, ·ÖÀàºóµÄÊý¾Ý´æÈëm_CMATÊý×éÖÐ
			m_CMAT[i]ÖÐ¸ø³öÁËÊÊÓÃÓÚµÚiÖÖ×°±¸µÄÈ«²¿Ä§·¨ÊôÐÔ
******************************************************************************/
int KLibOfBPT::InitMALib()
{
	int bEC = FALSE;

	// È·¶¨Ã¿ÖÖ×°±¸¸÷ÓÐ¶àÉÙÖÖ¿ÉÄÜµÄÄ§·¨ÊôÐÔ
    int naryMACount[2][MATF_CBDR];	// µÚiÖÖ×°±¸¹²ÓÐnaryMACount[0][i]ÖÖ¿ÉÄÜµÄÇ°×º
									//			  ºÍnaryMACount[1][i]ÖÖ¿ÉÄÜµÄºó×º
	m_BPTMagicAttrib.GetMACount((int*)naryMACount);

	// ¸ù¾ÝÈ¡»ØµÄÊýÖµ£¬Îª¸÷×°±¸µÄÄ§·¨ÊôÐÔË÷Òý±í·ÖÅäÄÚÈÝ
	for (int i = 0; i < MATF_CBDR; ++i)	// -1 because of horse
	{
		//_ASSERT(naryMACount[0][i] > 0);	// ²»¿ÉÄÜÃ»ÓÐÒ»¸öÄ§·¨Ç°×ºÊÊÓÃÓÚ¸Ã×°±¸
		//_ASSERT(naryMACount[1][i] > 0);	// ²»¿ÉÄÜÃ»ÓÐÒ»¸öÄ§·¨ºó×ºÊÊÓÃÓÚ¸Ã×°±¸
		if  (naryMACount[0][i]<=0 || naryMACount[1][i]<=0)
			continue;

		if (FALSE == m_CMAT[0][i].GetMemory(naryMACount[0][i]))
			return bEC;
		if (FALSE == m_CMAT[1][i].GetMemory(naryMACount[1][i]))
			return bEC;
	}

	// ±éÀúÄ§·¨ÊôÐÔ±í£¬½¨Á¢Æð¸÷×°±¸µÄÄ§·¨ÊôÐÔË÷Òý±í
	const int nNumOfMA = m_BPTMagicAttrib.NumOfEntries();	// Ä§·¨ÊôÐÔµÄ×ÜÊý
	//_ASSERT(nNumOfMA > 0);
	if (nNumOfMA<=0)
		return false;

	int m, n;		// µÚmÏîÄ§·¨ÊôÐÔÊÊÓÃÓÚµÚnÖÖ×°±¸
	for (m = 0; m < nNumOfMA; ++m)				// ±éÀúËùÓÐÄ§·¨ÊôÐÔ
	{
		const KMAGICATTRIB_TABFILE* pRec;
		pRec = m_BPTMagicAttrib.GetRecord(m);
		for (n = 0; n < MATF_CBDR; ++n)			// ¹²ÓÐMATF_CBDRÖÖ×°±¸´øÄ§·¨
		{// È·ÈÏÄ§·¨ÊôÐÔÊÊÓÃÓÚÄÄÐ©×°±¸
			if (pRec->m_DropRate[n]>1)
			{// ÔËÐÐÖÁ´Ë, ËµÃ÷µÚmÏîÄ§·¨ÊôÐÔÊÊÓÃÓÚµÚnÖÖ×°±¸
				int nPos;
				nPos = (0 == pRec->m_nPos) ? 1 : 0;   //Ç°×º ºó×º?
				m_CMAT[nPos][n].Set(m);
			}
		}
	}
	bEC = TRUE;
	return bEC;
}

/******************************************************************************
	¹¦ÄÜ£º	»ñÈ¡°×½ðµÄ»ù±¾Êý¾Ý
******************************************************************************/
const KBASICPROP_EQUIPMENT_PLATINA* KLibOfBPT::GetPlatinaRecord(IN int nIndex) const
{
	return (KBASICPROP_EQUIPMENT_PLATINA*)m_Platina.GetRecord(nIndex);
}

const int KLibOfBPT::GetPlatinaNumber() const
{
	return m_Platina.GetRecordCount();
}

/******************************************************************************
	¹¦ÄÜ£º	»ñÈ¡»Æ½ðµÄ»ù±¾Êý¾Ý
******************************************************************************/
const KBASICPROP_EQUIPMENT_GOLD* KLibOfBPT::GetGoldItemRecord(IN int nIndex) const
{
	return (KBASICPROP_EQUIPMENT_GOLD*)m_GoldItem.GetRecord(nIndex);
}

const int KLibOfBPT::GetGoldItemNumber() const
{
	return m_GoldItem.GetRecordCount();
}
/******************************************************************************
	¹¦ÄÜ£º	»ñÈ¡Ö¸¶¨µÄCMAT
******************************************************************************/
const KBPT_ClassifiedMAT* KLibOfBPT::GetCMAT(int nPos, int i) const
{
	//_ASSERT(this != NULL);
	if (this==NULL)
		return NULL;

	if (nPos != 0 && nPos != 1)
		{  return NULL; }
	if (i < 0 || i >= MATF_CBDR)
		{ return NULL; }
	return &(m_CMAT[nPos][i]);
}

/******************************************************************************
	¹¦ÄÜ£º	»ñÈ¡Ö¸¶¨µÄCMAIT
******************************************************************************/
// Add by Freeway Chen in 2003.5.30
const KBPT_ClassMAIT*       KLibOfBPT::GetCMIT(IN int nPrefixPostfix, IN int nType, IN int nSeries, int nLevel) const
{
   // _ASSERT((nPrefixPostfix >= 0) && (nPrefixPostfix < MATF_PREFIXPOSFIX));
    if (!((nPrefixPostfix >= 0) && (nPrefixPostfix < MATF_PREFIXPOSFIX)))
        return NULL;

    //_ASSERT((nType >= 0) && (nType < MATF_CBDR));  //°××°×°±¸ÖÖÀàÊý
    if (!((nType >= 0) && (nType < MATF_CBDR)))
        return NULL;

    //_ASSERT((nSeries >= 0) && (nSeries < MATF_SERIES)); //5ÐÐ
    if (!((nSeries >= 0) && (nSeries < MATF_SERIES)))
        return NULL;

    //_ASSERT(((nLevel - 1) >= 0) && ((nLevel - 1) < MATF_LEVEL));    // µÈ¼¶¼¶±ðnLevel is from 1..MATF_LEVEL
    if (!(((nLevel - 1) >= 0) && ((nLevel - 1) < MATF_LEVEL)))
        return NULL;

    return &m_CMAIT[nPrefixPostfix][nType][nSeries][nLevel - 1];
}

const KMAGICATTRIB_TABFILE* KLibOfBPT::GetMARecord(IN int i) const
{
	return m_BPTMagicAttrib.GetRecord(i);
}

const int KLibOfBPT::GetMARecordNumber() const
{
	return m_BPTMagicAttrib.NumOfEntries();
}

const KBASICPROP_EQUIPMENT* KLibOfBPT::GetMeleeWeaponRecord(IN int i) const
{
	return m_BPTMeleeWeapon.GetRecord(i);
}

const int KLibOfBPT::GetMeleeWeaponRecordNumber() const
{
	return m_BPTMeleeWeapon.NumOfEntries();
}

const KBASICPROP_EQUIPMENT*	KLibOfBPT::GetRangeWeaponRecord(IN int i) const
{
	return m_BPTRangeWeapon.GetRecord(i);
}

const int KLibOfBPT::GetRangeWeaponRecordNumber() const
{
	return m_BPTRangeWeapon.NumOfEntries();
}

const KBASICPROP_EQUIPMENT*	KLibOfBPT::GetArmorRecord(IN int i) const
{
	return m_BPTArmor.GetRecord(i);
}

const int KLibOfBPT::GetArmorRecordNumber() const
{
	return m_BPTArmor.NumOfEntries();
}

const KBASICPROP_EQUIPMENT*	KLibOfBPT::GetHelmRecord(IN int i) const
{
	return m_BPTHelm.GetRecord(i);
}

const int KLibOfBPT::GetHelmRecordNumber() const
{
	return m_BPTHelm.NumOfEntries();  //ÊôÐÔµÄÊýÁ¿
}

const KBASICPROP_EQUIPMENT*	KLibOfBPT::GetBootRecord(IN int i) const
{
	return m_BPTBoot.GetRecord(i);
}

const int KLibOfBPT::GetBootRecordNumber() const
{
	return m_BPTBoot.NumOfEntries();
}

const KBASICPROP_EQUIPMENT*	KLibOfBPT::GetBeltRecord(IN int i) const
{
	return m_BPTBelt.GetRecord(i);
}

const int KLibOfBPT::GetBeltRecordNumber() const
{
	return m_BPTBelt.NumOfEntries();
}

const KBASICPROP_EQUIPMENT*	KLibOfBPT::GetAmuletRecord(IN int i) const
{
	return m_BPTAmulet.GetRecord(i);
}

const int KLibOfBPT::GetAmuletRecordNumber() const
{
	return m_BPTAmulet.NumOfEntries();
}

const KBASICPROP_EQUIPMENT*	KLibOfBPT::GetRingRecord(IN int i) const
{
	return m_BPTRing.GetRecord(i);
}

const int KLibOfBPT::GetRingRecordNumber() const
{
	return m_BPTRing.NumOfEntries();
}

const KBASICPROP_EQUIPMENT*	KLibOfBPT::GetCuffRecord(IN int i) const
{
	return m_BPTCuff.GetRecord(i);
}

const int KLibOfBPT::GetCuffRecordNumber() const
{
	return m_BPTCuff.NumOfEntries();
}

const KBASICPROP_EQUIPMENT*	KLibOfBPT::GetPendantRecord(IN int i) const
{
	return m_BPTPendant.GetRecord(i);
}

const int KLibOfBPT::GetPendantRecordNumber() const
{
	return m_BPTPendant.NumOfEntries();
}

const KBASICPROP_EQUIPMENT* KLibOfBPT::GetHorseRecord(IN int i) const
{
	return m_BPTHorse.GetRecord(i);
}

const int KLibOfBPT::GetHorseRecordNumber() const
{
	return m_BPTHorse.NumOfEntries();
}
//-----------------------------------------------------------------
const KBASICPROP_MEDICINE* KLibOfBPT::GetMedicineRecord(IN int i) const
{
	return m_BPTMedicine.GetRecord(i);
}

const int KLibOfBPT::GetMedicineRecordNumber() const
{
	return m_BPTMedicine.NumOfEntries();
}
//------------------------------------------------------------------
const KBASICPROP_QUEST* KLibOfBPT::GetQuestRecord(IN int i) const
{
	return m_BPTQuest.GetRecord(i);
}

const int KLibOfBPT::GetQuestRecordNumber() const
{
	return m_BPTQuest.NumOfEntries();
}
//----------------------ÎÆ¸Ù-------------------------------------
const KBASICPROP_FUSION* KLibOfBPT::GetFusionRecord(IN int i) const
{
	return m_BPTFusion.GetRecord(i);
}

const int KLibOfBPT::GetFusionRecordNumber() const
{
	return m_BPTFusion.NumOfEntries();
}
//-------------------------------------------------------------------
const KBASICPROP_TOWNPORTAL* KLibOfBPT::GetTownPortalRecord(IN int i) const
{
	return m_BPTTownPortal.GetRecord(i);
}

const int KLibOfBPT::GetTownPortalRecordNumber() const
{
	return m_BPTTownPortal.NumOfEntries();
}
//--------------------------------------------------------------------
const KBASICPROP_EQUIPMENT* KLibOfBPT::GetMaskRecord(IN int i) const
{
	return m_BPTMask.GetRecord(i);
}

const int KLibOfBPT::GetMaskRecordNumber() const
{
	return m_BPTMask.NumOfEntries();
}
//
//-----------------------------Åû·ç------------------------------------
const KBASICPROP_EQUIPMENT* KLibOfBPT::GetPifengRecord(IN int i) const
{
	return m_BPTPifeng.GetRecord(i);
}

const int KLibOfBPT::GetPifengRecordNumber() const
{
	return m_BPTPifeng.NumOfEntries();
}
//-----------------------------Ó¡¼ø------------------------------------
const KBASICPROP_EQUIPMENT* KLibOfBPT::GetYinjianRecord(IN int i) const
{
	return m_BPTYinjian.GetRecord(i);
}

const int KLibOfBPT::GetYinjianRecordNumber() const
{
	return m_BPTYinjian.NumOfEntries();
}
//------------------------------ÊÎÆ·-----------------------------------
const KBASICPROP_EQUIPMENT* KLibOfBPT::GetShipinRecord(IN int i) const
{
	return m_BPTShipin.GetRecord(i);
}

const int KLibOfBPT::GetShipinRecordNumber() const
{
	return m_BPTShipin.NumOfEntries();
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄ¿óÊ¯µÄÊôÐÔ¼ÇÂ¼
	Èë¿Ú:	i: Ö¸¶¨¿óÊ¯
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã¼ÇÂ¼µÄÖ¸Õë
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_MINE* KLibOfBPT::GetMine(IN int i) const
{
	return m_BPTMine.GetRecord(i);
}

const int KLibOfBPT::GetMineRecordNumber() const
{
	return m_BPTMine.NumOfEntries();
}
/*
const KBASICPROP_EQUIPMENT* KLibOfBPT::FindEquipment(IN int a, IN int b, IN int c) const
{
	return m_BPTEquipment.FindRecord(a, b, c);
}
*/

const KBASICPROP_EQUIPMENT_UNIQUE* KLibOfBPT::FindEquipmentUnique(IN int a, IN int b, IN int c) const
{
	return 0;// TODO:m_BPTEquipment_Unique.FindRecord(a, b, c);
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄÒ©²ÄµÄÊôÐÔ¼ÇÂ¼
	Èë¿Ú:	i: Ö¸¶¨Ò©²Ä
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã¼ÇÂ¼µÄÖ¸Õë
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_MEDMATERIAL* KLibOfBPT::GetMedMaterial(int i) const
{
	return 0;// TODO:m_BPTMedMaterial.GetRecord(i);
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄÒ©Æ·µÄÊôÐÔ¼ÇÂ¼
	Èë¿Ú:	nType: Ò©Æ·µÄÀàÐÍ
			nLevel: Ò©Æ·µÄ¼¶±ð
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã¼ÇÂ¼µÄÖ¸Õë
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_MEDICINE* KLibOfBPT::FindMedicine(IN int nType, IN int nLevel) const
{
	return 0;//TODO: m_BPTMedicine.FindRecord(nType, nLevel);
}

/////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
// const KBASICPROP_EQUIPMENT_GOLD* KLibOfBPT::GetGoldRecord( IN int i) const
// {
// 	return m_GoldItem.GetRecord(i);
// }
//
// const int KLibOfBPT::GetGoldRecordNumber(IN int i) const
// {
// 	return m_GoldItem.NumOfEntries();
// }
//=============================================================================

KBasicPropertyTable::KBasicPropertyTable()
{
	m_pBuf = NULL;
	m_nNumOfEntries = 0;
	m_nSizeOfEntry = 0;
	m_szTabFile[0] = 0;
}

KBasicPropertyTable::~KBasicPropertyTable()
{
	ReleaseMemory();
}

/******************************************************************************
	¹¦ÄÜ:	¼ÇÂ¼tab fileÖÐ¹²ÓÐ¶àÉÙÏîÊý¾Ý¼ÇÂ¼
******************************************************************************/
void KBasicPropertyTable::SetCount(int cbCount)
{
	//_ASSERT(cbCount>0);
	//_ASSERT(0==m_nNumOfEntries);	// this function is supposed to be called only once
	 if (cbCount >0 &&  0==m_nNumOfEntries)
	    m_nNumOfEntries = cbCount;
}

/******************************************************************************
	¹¦ÄÜ:	·ÖÅäÄÚ´æ,ÓÃÓÚ±£´æ´Ótab fileÖÐ¶ÁÈëµÄÊý¾Ý
	Èë¿Ú:	m_nNumOfEntries: ¹²ÓÐÕâÃ´¶àÏîÊý¾Ý¼ÇÂ¼
			m_nSizeOfEntry: Ã¿ÏîÊý¾Ý¼ÇÂ¼µÄ´óÐ¡(×Ö½Ú)
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, m_pBuf Ö¸Ïò·ÖÅäµÄÄÚ´æ
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
int KBasicPropertyTable::GetMemory()
{
	//_ASSERT(NULL == m_pBuf);
	if (m_pBuf)
		return false;

	//_ASSERT(m_nNumOfEntries > 0 && m_nSizeOfEntry > 0);
    if (m_nNumOfEntries<=0 ||  m_nSizeOfEntry<=0)
    	return false;

	int bEC = FALSE;
	const int nMemSize = m_nSizeOfEntry * m_nNumOfEntries;
	void* pBuf = new BYTE[nMemSize]; //·ÖÅäÄÚ´æ
	//_ASSERT(pBuf != NULL);
	if (pBuf != NULL)
	{
		m_pBuf = pBuf;
		bEC = TRUE;
	}
	return bEC;
}

/******************************************************************************
	¹¦ÄÜ:	ÊÍ·ÅÄÚ´æ
******************************************************************************/
void KBasicPropertyTable::ReleaseMemory()
{
	if (m_pBuf)
	{
		delete []m_pBuf;
		m_pBuf = NULL;
		m_nNumOfEntries = 0;
	}
}

/******************************************************************************
	¹¦ÄÜ:	¶ÁÈëtab fileÖÐµÄÈ«²¿Êý¾Ý
	Èë¿Ú:	m_szTabFile: ÎÄ¼þÃû
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, È«²¿Êý¾Ý¶ÁÈëm_pBufËùÖ¸»º³åÇøÖÐ.
				m_nNumOfEntries ¸ø³ö¹²¶ÁÈë¶àÉÙÏîÊý¾Ý
			Ê§°ÜÊ±·µ»ØÁã  ---×¢ÒâÇø±ð ±êµã µÄÈ«½Ç ºÍ°ë½Ç ÎÊÌâ
******************************************************************************/
int KBasicPropertyTable::Load()             //¶ÁÈë×°±¸ÎïÆ·ÎÄ¼þ
{
	int bEC = FALSE;
	KTabFile	theLoader;

	// ¼ÓÔØtab file
	g_SetRootPath(NULL);  //ÉèÖÃ¸ùÄ¿Â¼

//	::g_SetFilePath(TABFILE_PATH);
	char	szFileName[FILE_NAME_LENGTH]={0};
            //szFileName[FILE_NAME_LENGTH]=0;
	g_UnitePathAndName((char*)TABFILE_PATH, m_szTabFile, szFileName); //ºÏ²¢ÍêÕûÂ·¾¶
	if (FALSE == theLoader.Load(szFileName))        //¶ÁÈëÎÄ¼þ
	{
	//	_ASSERT(FALSE);
		theLoader.Clear();
	    return bEC;
	}                 //½ÓÊÜÊý¾Ý   ---³ö´íµ¼ÖÂ ¿Í»§¶ËÉÁÍË

	// È·¶¨fileÄÚ¸ø³öÁË¶àÉÙÏî¼ÇÂ¼
	const int cbItems = theLoader.GetHeight() - 1;	// µÚÒ»ÐÐ¸ø³ö¸÷ÁÐÃû³Æ,
	if (cbItems <= 0)							    // Êµ¼ÊÊý¾Ý´ÓµÚ2ÐÐ¿ªÊ¼¸ø³ö.
	{
		//_ASSERT(FALSE);
		theLoader.Clear();
		return bEC;
	}                                               // ---³ö´íµ¼ÖÂ ¿Í»§¶ËÉÁÍË

	SetCount(cbItems);                              // Éè¶¨×ÜÐÐÊý

	// ·ÖÅäÄÚ´æ£¬¹¹½¨ÊôÐÔ±í
	if (FALSE == GetMemory())
	{
		//_ASSERT(FALSE);
		theLoader.Clear();
	    return bEC;
	}                 // ---³ö´íµ¼ÖÂ ¿Í»§¶ËÉÁÍË

	// ½«ÊôÐÔ¼ÇÂ¼ÖðÌõ¶ÁÈë
	int i;
	for ( i = 0; i < cbItems; ++i)  //ÓÐ¶àÉÙÁÐ
	{
		if (FALSE == LoadRecord(i, &theLoader))     // ---³ö´íµ¼ÖÂ ¿Í»§¶ËÉÁÍË
		{
			//_ASSERT(FALSE);
			theLoader.Clear();
			return bEC;
		}
	}

	bEC = TRUE;
	theLoader.Clear();
	return bEC;
}

//============================================================================

KBPT_Mine::KBPT_Mine()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_MINE);
	::strcpy(m_szTabFile, TABFILE_MINE);
}

KBPT_Mine::~KBPT_Mine()
{
}

int KBPT_Mine::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (pTF==NULL || i<0 || i>=m_nNumOfEntries)
		return false;

	// ³õÊ¼»¯
	KBASICPROP_MINE* pBuf = (KBASICPROP_MINE*)m_pBuf;
	pBuf = pBuf + i;	// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO	aryPI[] =
	{
		{ PI_VARTYPE_CHAR,			pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nItemGenre), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDetailType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nParticularType), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szImageName, sizeof(pBuf->m_szImageName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nObjIdx), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWidth), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nHeight), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},  //ËµÃ÷
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSeries), 0},   //ÎåÐÐ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPrice), 0},    //¼Û¸ñ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 0},    //µÈ¼¶
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_bStack), 0},    //ÊÇ·ñÄÜµþ¼Ó
		{ PI_VARTYPE_CHAR,			pBuf->m_szScript, sizeof(pBuf->m_szScript)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[0]), 0},  //²ÎÊý  ¼¼ÄÜID
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[1]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[2]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[3]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[4]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDelet), 0},     //Ê¹ÓÃºóÊÇ·ñÏûÊ§
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPriceXu), 0},   //½ð±Ò
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_IsKuaiJie), 0},  //ÊÇ·ñ¿ì½ÝÀ¸
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_IsMagic), 0},    //ÊÇ·ñÓÐÊôÐÔ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_IsUse), 0},      //ÊÇ·ñÁ¢¼´Ê¹ÓÃ
	};

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	return ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0]));

}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄ¿óÊ¯µÄÊôÐÔ
	Èë¿Ú:	i: ÒªÇó»ñÈ¡µÚiÏî¿óÊ¯µÄÊôÐÔ. ÈôÎª-1±íÊ¾Ëæ»úÈ¡Ò»¸ö
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã¿óÊ¯ÊôÐÔµÄÖ¸Õë(Ò»¸öKBASICPROP_MINE½á¹¹)
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_MINE* KBPT_Mine::GetRecord(int i) const
{
	//_ASSERT(this != NULL);

	if (this==NULL)
		return NULL;

	if (-1 == i)
		i = ::GetRandomNumber(0, m_nNumOfEntries-1);

	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	return (i >= 0 && i < m_nNumOfEntries) ?
		(((KBASICPROP_MINE*)m_pBuf) + i) : NULL;
}


const KBASICPROP_MINE* KBPT_Mine::FindRecord(IN int nType,IN int nLevel) const
{
	const KBASICPROP_MINE* pData = NULL;
	// ÒÔÏÂÊ¹ÓÃË³Ðò²éÕÒµÄËã·¨. ÈôÔ­Ê¼Êý¾ÝÊÇ°´m_nDetailType¼°m_nLevelÅÅÐòµÄ,
	// Ôò¿É½øÐÐËã·¨ÓÅ»¯
	for (int i = 0; i < m_nNumOfEntries; ++i)
	{
		const KBASICPROP_MINE* pMed;
		pMed = GetRecord(i);

		//_ASSERT(NULL != pMed);
		if (pMed == NULL)
			continue;

		if (nType == pMed->m_nDetailType && nLevel == pMed->m_nLevel)
		{
			// ¸ù¾Ý²ß»®µÄÉè¼Æ, ÊôÐÔ1ÀàÐÍ == ¾ßÌåÀà±ð. ½øÐÐÑéÖ¤
			//			_ASSERT(nType == pMed->m_nAttrib1_Type);
			pData = pMed;
			break;
		}
	}
	return pData;
}

//============================================================================
KBPT_TownPortal::KBPT_TownPortal()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_TOWNPORTAL);
	::strcpy(m_szTabFile, TABFILE_TOWNPORTAL);
}

KBPT_TownPortal::~KBPT_TownPortal()
{
}

int KBPT_TownPortal::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (pTF==NULL || i<0 || i >=m_nNumOfEntries)
		return false;

	KBASICPROP_TOWNPORTAL* pBuf = (KBASICPROP_TOWNPORTAL*)m_pBuf;
	pBuf = pBuf + i;			// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO aryPI[] =
	{
		{ PI_VARTYPE_CHAR,			pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nItemGenre), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szImageName, sizeof(pBuf->m_szImageName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nObjIdx), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWidth), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nHeight), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPrice), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},
//		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_IsKuaiJie), 0}, //ÊÇ·ñ¿ì½ÝÀ¸
	};

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	return ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0]));
}

const KBASICPROP_TOWNPORTAL* KBPT_TownPortal::GetRecord(IN int i) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (this == NULL)
		return NULL;

	return (i >= 0 && i < m_nNumOfEntries) ? (((KBASICPROP_TOWNPORTAL*)m_pBuf) + i) : NULL;
}

//============================================================================
KBPT_Quest::KBPT_Quest()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_QUEST);
	::strcpy(m_szTabFile, TABFILE_TASK);
}

KBPT_Quest::~KBPT_Quest()
{
}

int KBPT_Quest::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (pTF==NULL || i<0 || i >=m_nNumOfEntries)
		return false;

	KBASICPROP_QUEST* pBuf = (KBASICPROP_QUEST*)m_pBuf;
	pBuf = pBuf + i;			// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO aryPI[] =
	{
		{ PI_VARTYPE_CHAR,			pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nItemGenre), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDetailType), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szImageName, sizeof(pBuf->m_szImageName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nObjIdx), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWidth), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nHeight), 0},

		{ PI_VARTYPE_CHAR,			pBuf->m_szScript, sizeof(pBuf->m_szScript)},   //½Å±¾
		{ PI_VARTYPE_CHAR,			pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPrice), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPriceXu), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDelet), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nIsSell), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nIsTrade), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nIsDrop), 0},    //ºÏ³É²ÄÁÏÖÖÀà
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_IsKuaiJie), 0},  //ÊÇ·ñ¿ì½ÝÀ¸
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_SkillType), 0},  //ÊÇ¼¼ÄÜÄ§·¨ÐÐºÅ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSeries), -1},   //ÊôÐÔ/ÎåÐÐ
	    { PI_VARTYPE_INT,  (char*)&(pBuf->m_ISMagic), 0},    //ÊÇ·ñÓÐÊôÐÔ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 1},     //µÈ¼¶
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_bStack), 0},     //ËÀ·ñµþ·Å
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_MagicID), 0},    //¼¼ÄÜID
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_IsUse), 0},      //ÊÇ·ñÁ¢¼´Ê¹ÓÃ

	};

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	return ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0]));

}

const KBASICPROP_QUEST* KBPT_Quest::GetRecord(IN int i) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (this==NULL || i<0 || i >=m_nNumOfEntries)
		return NULL;
	return (i >= 0 && i < m_nNumOfEntries) ? (((KBASICPROP_QUEST*)m_pBuf) + i) : NULL;

}

const KBASICPROP_QUEST* KBPT_Quest::FindRecord(IN int nDetailType) const
{
	const KBASICPROP_QUEST* pData = NULL;
	// ÒÔÏÂÊ¹ÓÃË³Ðò²éÕÒµÄËã·¨. ÈôÔ­Ê¼Êý¾ÝÊÇ°´m_nDetailTypeÅÅÐòµÄ,
	// Ôò¿É½øÐÐËã·¨ÓÅ»¯
	for (int i = 0; i < m_nNumOfEntries; ++i)
	{
		const KBASICPROP_QUEST* pQst;
		pQst = GetRecord(i);
		//_ASSERT(NULL != pQst);

		if (pQst==NULL)
			continue;

		if (nDetailType == pQst->m_nDetailType)
		{
			// ¸ù¾Ý²ß»®µÄÉè¼Æ, ÊôÐÔ1ÀàÐÍ == ¾ßÌåÀà±ð. ½øÐÐÑéÖ¤
//			_ASSERT(nType == pMed->m_nAttrib1_Type);
			pData = pQst;
			break;
		}
	}
	return pData;
}


//============================================================================//ÎÆ¸Ö
KBPT_Fusion::KBPT_Fusion()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_FUSION);
	::strcpy(m_szTabFile, TABFILE_FUSION);
}

KBPT_Fusion::~KBPT_Fusion()
{
}

int KBPT_Fusion::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);

	if (pTF==NULL || i<0 || i >=m_nNumOfEntries)
			return NULL;

	KBASICPROP_FUSION* pBuf = (KBASICPROP_FUSION*)m_pBuf;
	pBuf = pBuf + i;			// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO aryPI[] =
	{
		{ PI_VARTYPE_CHAR, pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nItemGenre), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDetailType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nParticularType), 0},
		{ PI_VARTYPE_CHAR, pBuf->m_szImageName, sizeof(pBuf->m_szImageName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nObjIdx), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWidth), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nHeight), 0},
		{ PI_VARTYPE_CHAR,	pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},//ËµÃ÷
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSeries),-1},   //ÊôÐÔ/ÎåÐÐ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPrice),0},      //Ó¢Á½
        { PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 1},     //µÈ¼¶
        { PI_VARTYPE_INT,  (char*)&(pBuf->m_bStack), 0},     //ËÀ·ñµþ·Å
        { PI_VARTYPE_INT,  (char*)&(pBuf->m_inPin), 1},      //Æ·ÖÊ
        { PI_VARTYPE_INT,  (char*)&(pBuf->m_MagIndex), 0},   //Ä§·¨Ë÷Òý
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[0]), -1},   //¿ÉÈÛÁ¶²¿Î»
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[1]), -1},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[2]), -1},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[3]), -1},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[4]), -1},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_Magic[5]), -1},
        { PI_VARTYPE_INT,  (char*)&(pBuf->m_IsBang), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPriceXu), 0},
	};
	//	{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDelet), 0},
	//	{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nIsSell), 0},
	//	{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nIsTrade), 0},
	//	{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nIsDrop), 0},    //ºÏ³É²ÄÁÏÖÖÀà
	//	{ PI_VARTYPE_INT,  (char*)&(pBuf->m_IsKuaiJie), 0},  //ÊÇ·ñ¿ì½ÝÀ¸

	//	{ PI_VARTYPE_INT,  (char*)&(pBuf->m_ISMagic), 0},    //ÊÇ·ñÓÐÊôÐÔ
	//	{ PI_VARTYPE_INT,  (char*)&(pBuf->m_MagicID), 0},    //¼¼ÄÜID
	//{ PI_VARTYPE_INT,  (char*)&(pBuf->m_IsUse), 0},      //ÊÇ·ñÁ¢¼´Ê¹ÓÃ
	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	return ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0]));

}

const KBASICPROP_FUSION* KBPT_Fusion::GetRecord(IN int i) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (this==NULL || i<0 || i >=m_nNumOfEntries)
				return NULL;
	return (i >= 0 && i < m_nNumOfEntries) ? (((KBASICPROP_FUSION*)m_pBuf) + i) : NULL;
}

const KBASICPROP_FUSION* KBPT_Fusion::FindRecord(IN int nDetailType,IN int nLevel) const
{
	const KBASICPROP_FUSION* pData = NULL;
	// ÒÔÏÂÊ¹ÓÃË³Ðò²éÕÒµÄËã·¨. ÈôÔ­Ê¼Êý¾ÝÊÇ°´m_nDetailTypeÅÅÐòµÄ,
	// Ôò¿É½øÐÐËã·¨ÓÅ»¯
	for (int i = 0; i < m_nNumOfEntries; ++i)
	{
		const KBASICPROP_FUSION* pQst;

		      pQst = GetRecord(i);

		//_ASSERT(NULL != pQst);
		if (pQst==NULL) continue;


		if (nDetailType == pQst->m_nParticularType)  //ÏêÏ¸Àà±ð
		{
			// ¸ù¾Ý²ß»®µÄÉè¼Æ, ÊôÐÔ1ÀàÐÍ == ¾ßÌåÀà±ð. ½øÐÐÑéÖ¤
			//			_ASSERT(nType == pMed->m_nAttrib1_Type);
			pData = pQst;
			break;
		}
	}
	return pData;
}

//============================================================================

KBPT_Medicine::KBPT_Medicine()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_MEDICINE);
	::strcpy(m_szTabFile,TABFILE_MEDICINE);
}

KBPT_Medicine::~KBPT_Medicine()
{
}

int KBPT_Medicine::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (pTF==NULL || i<0 || i >=m_nNumOfEntries)
	   return NULL;

	// ³õÊ¼»¯
	KBASICPROP_MEDICINE* pBuf = (KBASICPROP_MEDICINE*)m_pBuf;
	pBuf = pBuf + i;	// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO	aryPI[] =
	{
		{ PI_VARTYPE_CHAR, pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nItemGenre), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDetailType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nParticularType), 0},
		{ PI_VARTYPE_CHAR, pBuf->m_szImageName, sizeof(pBuf->m_szImageName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nObjIdx), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWidth), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nHeight), 0},
		{ PI_VARTYPE_CHAR, pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSeries), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPrice), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_bStack), 0},
        { PI_VARTYPE_INT,  (char*)&(pBuf->m_IsKuaiJie), 0}, //ÊÇ·ñ¿ì½ÝÀ¸
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[0].nAttrib), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[0].nValue), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[0].nTime), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[1].nAttrib), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[1].nValue), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[1].nTime), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[2].nAttrib), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[2].nValue), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[2].nTime), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[3].nAttrib), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[3].nValue), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[3].nTime), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[4].nAttrib), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[4].nValue), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[4].nTime), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[5].nAttrib), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[5].nValue), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryAttrib[5].nTime), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_UseMap), 0},
	};

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	return ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0]));
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄÒ©Æ·µÄÊôÐÔ
	Èë¿Ú:	i: ÒªÇó»ñÈ¡µÚiÏîÒ©Æ·µÄÊôÐÔ
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸ÃÒ©²ÄÊôÐÔµÄÖ¸Õë(Ò»¸öKBASICPROP_MEDICINE½á¹¹)
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_MEDICINE* KBPT_Medicine::GetRecord(int i) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (this==NULL || i<0 || i >=m_nNumOfEntries)
						return NULL;
	return (i >= 0 && i < m_nNumOfEntries) ? (((KBASICPROP_MEDICINE*)m_pBuf) + i) : NULL;
}

const KBASICPROP_MEDICINE* KBPT_Medicine::FindRecord(IN int nType,IN int nLevel) const
{
	const KBASICPROP_MEDICINE* pData = NULL;
	// ÒÔÏÂÊ¹ÓÃË³Ðò²éÕÒµÄËã·¨. ÈôÔ­Ê¼Êý¾ÝÊÇ°´m_nDetailType¼°m_nLevelÅÅÐòµÄ,
	// Ôò¿É½øÐÐËã·¨ÓÅ»¯
	for (int i = 0; i < m_nNumOfEntries; ++i)
	{
		const KBASICPROP_MEDICINE* pMed;
		pMed = GetRecord(i);
		//_ASSERT(NULL != pMed);
		if (pMed == NULL) continue;
		if (nType == pMed->m_nDetailType && nLevel == pMed->m_nLevel)
		{
			// ¸ù¾Ý²ß»®µÄÉè¼Æ, ÊôÐÔ1ÀàÐÍ == ¾ßÌåÀà±ð. ½øÐÐÑéÖ¤
//			_ASSERT(nType == pMed->m_nAttrib1_Type);
			pData = pMed;
			break;
		}
	}
	return pData;
}

//============================================================================

KBPT_MedMaterial::KBPT_MedMaterial()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_MEDMATERIAL);
	::strcpy(m_szTabFile, TABFILE_MEDMATERIAL);
}

KBPT_MedMaterial::~KBPT_MedMaterial()
{
}

int KBPT_MedMaterial::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (pTF==NULL || i<0 || i >=m_nNumOfEntries)
						return NULL;

	// ³õÊ¼»¯
	KBASICPROP_MEDMATERIAL* pBuf = (KBASICPROP_MEDMATERIAL*)m_pBuf;
	pBuf = pBuf + i;	// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO	aryPI[] =
	{
/*ver1****************************************************************
		{ PI_VARTYPE_CHAR,			pBuf->m_szSerialNum, sizeof(pBuf->m_szSerialNum)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nStyle), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_CHAR,			pBuf->m_szImage1, sizeof(pBuf->m_szImage1)},
		{ PI_VARTYPE_CHAR,			pBuf->m_szImage2, sizeof(pBuf->m_szImage2)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_bRepeatable), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSort1), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSort2), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nValue), 0},
*********************************************************************/
		{ PI_VARTYPE_CHAR,			pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nItemGenre), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDetailType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nParticularType), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szImageName, sizeof(pBuf->m_szImageName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nObjIdx), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWidth), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nHeight), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSeries), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPrice), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_bStack), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nAttrib1_Type), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nAttrib1_Para), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nAttrib2_Type), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nAttrib2_Para), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nAttrib3_Type), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nAttrib3_Para), 0},
	};

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	return ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0]));
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄÒ©²ÄµÄÊôÐÔ
	Èë¿Ú:	i: ÒªÇó»ñÈ¡µÚiÏîÒ©²ÄµÄÊôÐÔ. ÈôÎª-1±íÊ¾Ëæ»úÈ¡Ò»¸ö
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸ÃÒ©²ÄÊôÐÔµÄÖ¸Õë(Ò»¸öKBASICPROP_MEDMATERIAL½á¹¹)
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_MEDMATERIAL* KBPT_MedMaterial::GetRecord(int i) const
{
	//_ASSERT(this != NULL);
	if (this==NULL) return NULL;

	if (-1 == i)
		i = ::GetRandomNumber(0, m_nNumOfEntries-1);

	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	return (i >= 0 && i < m_nNumOfEntries) ?
		(((KBASICPROP_MEDMATERIAL*)m_pBuf) + i) : NULL;
}

KBPT_Equipment::KBPT_Equipment()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_EQUIPMENT);
}

KBPT_Equipment::~KBPT_Equipment()
{
}

void KBPT_Equipment::Init(IN int i)  //¶ÁÈë°××°ÊôÐÔ
{
	::strcpy(m_szTabFile, TABFILE_EQUIPMENT[i]);
}
//°××°¶ÁÈë°××°ÊôÐÔ
int KBPT_Equipment::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (pTF==NULL || i<0 || i >=m_nNumOfEntries)
							return NULL;

	// ³õÊ¼»¯
	KBASICPROP_EQUIPMENT* pBuf = (KBASICPROP_EQUIPMENT*)m_pBuf;
	pBuf = pBuf + i;	// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO	aryPI[] =
	{
		{ PI_VARTYPE_CHAR,			pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nItemGenre), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDetailType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nParticularType), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szImageName, sizeof(pBuf->m_szImageName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nObjIdx), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWidth), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nHeight), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSeries), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPrice), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_bStack), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[0].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[0].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[0].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[1].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[1].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[1].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[2].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[2].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[2].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[3].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[3].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[3].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[4].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[4].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[4].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[5].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[5].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[5].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[6].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[6].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[6].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[0].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[0].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[1].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[1].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[2].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[2].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[3].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[3].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[4].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[4].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[5].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[5].nPara), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nYingNuma), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nYingNumb), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nRongNum), 0}, //¿ÉÈÛÁ¶ÊýÁ¿
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWengangPin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nBinfujiazhi), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_ChiBangRes), 0},      //³á°òµÄÍâ¹ÛÀàÐÍ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nParticularTypea), 0},//ÓÃÓÚÌí¼ÓÆäËûÀ¶×°
	};

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	return ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0]));
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄ×°±¸µÄÊôÐÔ
	Èë¿Ú:	i: ÒªÇó»ñÈ¡µÚiÏî×°±¸µÄÊôÐÔ
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã×°±¸ÊôÐÔµÄÖ¸Õë(Ò»¸öKBASICPROP_EQUIPMENT½á¹¹)
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_EQUIPMENT* KBPT_Equipment::GetRecord(int i) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (this==NULL || i<0 || i >=m_nNumOfEntries)
							return NULL;
	return (i >= 0 && i < m_nNumOfEntries) ? (((KBASICPROP_EQUIPMENT*)m_pBuf) + i) : NULL;
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄ×°±¸ÊôÐÔ¼ÇÂ¼
	Èë¿Ú:	nDetailType: ¾ßÌåÀà±ð
			nParticularType: ÏêÏ¸Àà±ð
			nLevel: µÈ¼¶
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã¼ÇÂ¼µÄÖ¸Õë
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_EQUIPMENT* KBPT_Equipment::FindRecord( IN int nDetailType,
														IN int nParticularType,
														IN int nLevel) const
{
	//_ASSERT(this != NULL);
	if (this == NULL) return NULL;

	const KBASICPROP_EQUIPMENT* pData = NULL;
	// ÒÔÏÂÊ¹ÓÃË³Ðò²éÕÒµÄËã·¨. ÈôÔ­Ê¼Êý¾ÝÊÇ°´m_nDetailType¼°m_nLevelÅÅÐòµÄ,
	// Ôò¿É½øÐÐËã·¨ÓÅ»¯
	for (int i = 0; i < m_nNumOfEntries; ++i)
	{
		const KBASICPROP_EQUIPMENT* pEqu;
		pEqu = GetRecord(i);
		//_ASSERT(NULL != pEqu);
		if (pEqu==NULL)
			continue;

		if (nDetailType == pEqu->m_nDetailType &&
			nParticularType == pEqu->m_nParticularType &&
			nLevel == pEqu->m_nLevel)
		{
			pData = pEqu;
			break;
		}
	}
	return pData;
}

//============================================================================
// »Æ½ð×°±¸µÄÊµÏÖ
// flying
KBPT_Equipment_Gold::KBPT_Equipment_Gold()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_EQUIPMENT_GOLD);
	// Copy the gold item information file's name into the buffer
}

KBPT_Equipment_Gold::~KBPT_Equipment_Gold()
{
	return;
}

int KBPT_Equipment_Gold::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (pTF==NULL || i<0 || i >=m_nNumOfEntries)
								return NULL;

	// ³õÊ¼»¯
	KBASICPROP_EQUIPMENT_GOLD* pBuf = (KBASICPROP_EQUIPMENT_GOLD*)m_pBuf;
	pBuf = pBuf + i;	// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO	aryPI[] =
	{
		{ PI_VARTYPE_CHAR,			pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nItemGenre), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDetailType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nParticularType), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szImageName, sizeof(pBuf->m_szImageName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nObjIdx), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWidth), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nHeight), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSeries), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPrice), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_bStack), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[0].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[0].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[0].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[1].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[1].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[1].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[2].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[2].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[2].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[3].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[3].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[3].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[4].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[4].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[4].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[5].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[5].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[5].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[6].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[6].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[6].sRange.nMax), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[0].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[0].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[1].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[1].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[2].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[2].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[3].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[3].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[4].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[4].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[5].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[5].nPara), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[0]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[1]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[2]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[3]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[4]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[5]), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nId), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSet), 0},  //ËùÔÚÌ××°
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSetNum), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nUpSet), 0}, //À©Õ¹Ì××°
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSetId), 0}, //À©Õ¹Ì××°ÐòºÅ

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_yinMagicAttribs[0]), 0}, //À©Õ¹Ì××°Òþ²ØÊôÐÔ1
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_yinMagicAttribs[1]), 0}, //À©Õ¹Ì××°Òþ²ØÊôÐÔ2
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nRongNum), 0},  //¿ÉÈÛÁ¶ÊýÁ¿
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWengangPin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nBinfujiazhi), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_ChiBangRes), 0},//³á°òµÄÍâ¹ÛÀàÐÍ
	};

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	return ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0]));
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄ×°±¸µÄÊôÐÔ
	Èë¿Ú:	i: ÒªÇó»ñÈ¡µÚiÏî×°±¸µÄÊôÐÔ
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã×°±¸ÊôÐÔµÄÖ¸Õë(Ò»¸öKBASICPROP_EQUIPMENT½á¹¹)
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_EQUIPMENT_GOLD* KBPT_Equipment_Gold::GetRecord(int i) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (this==NULL || i<0 || i >=m_nNumOfEntries)
									return NULL;
	return (i >= 0 && i < m_nNumOfEntries) ? (((KBASICPROP_EQUIPMENT_GOLD*)m_pBuf) + i) : NULL;
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄÎ¨Ò»×°±¸ÊôÐÔ¼ÇÂ¼
	Èë¿Ú:	nDetailType: ¾ßÌåÀà±ð
			nParticularType: ÏêÏ¸Àà±ð
			nLevel: µÈ¼¶
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã¼ÇÂ¼µÄÖ¸Õë
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_EQUIPMENT_GOLD*
		KBPT_Equipment_Gold::FindRecord(IN int nDetailType,
										  IN int nParticularType,
										  IN int nLevel) const
{
	//_ASSERT(this != NULL);
	if (this==NULL) return NULL;

	const KBASICPROP_EQUIPMENT_GOLD* pData = NULL;
	for (int i = 0; i < m_nNumOfEntries; ++i)
	{
		const KBASICPROP_EQUIPMENT_GOLD* pEqu;
		pEqu = GetRecord(i);
		//_ASSERT(NULL != pEqu);
		if (NULL == pEqu)
			continue;

		if (nDetailType == pEqu->m_nDetailType &&
			nParticularType == pEqu->m_nParticularType &&
			nLevel == pEqu->m_nLevel)
		{
			pData = pEqu;
			break;
		}
	}
	return pData;
}

void KBPT_Equipment_Gold::Init()
{
	::strcpy(m_szTabFile, TABFILE_GOLDITEM);
}
//============================================================================
//============================================================================
// °×½ð×°±¸µÄÊµÏÖ  PLATINA
// flying
KBPT_Equipment_Platina::KBPT_Equipment_Platina()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_EQUIPMENT_PLATINA);
	// Copy the gold item information file's name into the buffer
}

KBPT_Equipment_Platina::~KBPT_Equipment_Platina()
{
	return;
}

int KBPT_Equipment_Platina::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (pTF==NULL || i<0 || i >=m_nNumOfEntries)
		return NULL;
	// ³õÊ¼»¯
	KBASICPROP_EQUIPMENT_PLATINA* pBuf = (KBASICPROP_EQUIPMENT_PLATINA*)m_pBuf;
	pBuf = pBuf + i;	// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO	aryPI[] =
	{
		{ PI_VARTYPE_CHAR,	pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nItemGenre), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDetailType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nParticularType), 0},
		{ PI_VARTYPE_CHAR,	pBuf->m_szImageName, sizeof(pBuf->m_szImageName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nObjIdx), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWidth), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nHeight), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSeries), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPrice), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_bStack), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[0].nType), 0},	   //»ù´¡ÊôÐÔ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[0].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[0].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[1].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[1].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[1].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[2].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[2].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[2].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[3].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[3].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[3].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[4].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[4].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[4].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[5].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[5].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[5].sRange.nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[6].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[6].sRange.nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropBasic[6].sRange.nMax), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[0].nType), 0},	//ÐèÇóÊôÐÔ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[0].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[1].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[1].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[2].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[2].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[3].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[3].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[4].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[4].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[5].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[5].nPara), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[0]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_TempPlatinaAttrib[0]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[1]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_TempPlatinaAttrib[1]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[2]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_TempPlatinaAttrib[2]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[3]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_TempPlatinaAttrib[3]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[4]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_TempPlatinaAttrib[4]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[5]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_TempPlatinaAttrib[5]), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSet), 0},       //ËùÔÚÌ××°
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSetNum), 0},    //Ì××°ÊýÁ¿
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSixSkill), 0},	 //6¼¶¼¼ÄÜID
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nTenSkill), 0},	 //10¼¶¼¼ÄÜID
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nUpSet), 0}, //À©Õ¹Ì××°
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSetId), 0}, //À©Õ¹Ì××°ÐòºÅ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_yinMagicAttribs[0]), 0}, //À©Õ¹Ì××°Òþ²ØÊôÐÔ1
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_yinMagicAttribs[1]), 0}, //À©Õ¹Ì××°Òþ²ØÊôÐÔ2
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nRongNum), 0},  //¿ÉÈÛÁ¶ÊýÁ¿
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nWengangPin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nBinfujiazhi), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_ChiBangRes), 0},//³á°òµÄÍâ¹ÛÀàÐÍ
	};

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	return ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0]));
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄ×°±¸µÄÊôÐÔ
	Èë¿Ú:	i: ÒªÇó»ñÈ¡µÚiÏî×°±¸µÄÊôÐÔ
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã×°±¸ÊôÐÔµÄÖ¸Õë(Ò»¸öKBASICPROP_EQUIPMENT½á¹¹)
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_EQUIPMENT_PLATINA* KBPT_Equipment_Platina::GetRecord(int i) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (this==NULL || i<0 || i >=m_nNumOfEntries)
			return NULL;
	return (i >= 0 && i < m_nNumOfEntries) ? (((KBASICPROP_EQUIPMENT_PLATINA*)m_pBuf) + i) : NULL;
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄÎ¨Ò»×°±¸ÊôÐÔ¼ÇÂ¼
	Èë¿Ú:	nDetailType: ¾ßÌåÀà±ð
			nParticularType: ÏêÏ¸Àà±ð
			nLevel: µÈ¼¶
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã¼ÇÂ¼µÄÖ¸Õë
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_EQUIPMENT_PLATINA*
		KBPT_Equipment_Platina::FindRecord(IN int nDetailType,
										  IN int nParticularType,
										  IN int nLevel) const
{
	//_ASSERT(this != NULL);
	if (this == NULL) return NULL;
	const KBASICPROP_EQUIPMENT_PLATINA* pData = NULL;
	for (int i = 0; i < m_nNumOfEntries; ++i)
	{
		const KBASICPROP_EQUIPMENT_PLATINA* pEqu;
		pEqu = GetRecord(i);
		//_ASSERT(NULL != pEqu);
		if (NULL == pEqu)
			continue;
		if (nDetailType == pEqu->m_nDetailType && nParticularType == pEqu->m_nParticularType && nLevel == pEqu->m_nLevel)
		{
			pData = pEqu;
			break;
		}
	}
	return pData;
}

void KBPT_Equipment_Platina::Init()
{
	::strcpy(m_szTabFile, TABFILE_PLATINA);
}
//============================================================================
KBPT_Equipment_Unique::KBPT_Equipment_Unique()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_EQUIPMENT_UNIQUE);
	::strcpy(m_szTabFile, TABFILE_EQUIPMENT_UNIQUE);
}

KBPT_Equipment_Unique::~KBPT_Equipment_Unique()
{
}

int KBPT_Equipment_Unique::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (pTF==NULL || i<0 || i >=m_nNumOfEntries)
		return NULL;
	// ³õÊ¼»¯
	KBASICPROP_EQUIPMENT_UNIQUE* pBuf = (KBASICPROP_EQUIPMENT_UNIQUE*)m_pBuf;
	pBuf = pBuf + i;	// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO	aryPI[] =
	{
		{ PI_VARTYPE_CHAR,			pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nItemGenre), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nDetailType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nParticularType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szImageName, sizeof(pBuf->m_szImageName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nObjIdx), 0},
		{ PI_VARTYPE_CHAR,			pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nSeries), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPrice), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nRarity), 0},
// TODO: ÒÔÉÏ¸÷ÏîµÄË³Ðò¿ÉÄÜ»¹»áµ÷Õû.

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[0].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[0].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[1].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[1].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[2].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[2].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[3].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[3].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[4].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[4].nPara), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[5].nType), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryPropReq[5].nPara), 0},

		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[0].nPropKind), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[0].aryRange[0].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[0].aryRange[0].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[0].aryRange[1].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[0].aryRange[1].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[0].aryRange[2].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[0].aryRange[2].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[1].nPropKind), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[1].aryRange[0].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[1].aryRange[0].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[1].aryRange[1].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[1].aryRange[1].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[1].aryRange[2].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[1].aryRange[2].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[2].nPropKind), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[2].aryRange[0].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[2].aryRange[0].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[2].aryRange[1].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[2].aryRange[1].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[2].aryRange[2].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[2].aryRange[2].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[3].nPropKind), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[3].aryRange[0].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[3].aryRange[0].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[3].aryRange[1].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[3].aryRange[1].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[3].aryRange[2].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[3].aryRange[2].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[4].nPropKind), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[4].aryRange[0].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[4].aryRange[0].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[4].aryRange[1].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[4].aryRange[1].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[4].aryRange[2].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[4].aryRange[2].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[5].nPropKind), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[5].aryRange[0].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[5].aryRange[0].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[5].aryRange[1].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[5].aryRange[1].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[5].aryRange[2].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_aryMagicAttribs[5].aryRange[2].nMax), 0},
	};

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	return ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0]));
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄ×°±¸µÄÊôÐÔ
	Èë¿Ú:	i: ÒªÇó»ñÈ¡µÚiÏî×°±¸µÄÊôÐÔ
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã×°±¸ÊôÐÔµÄÖ¸Õë(Ò»¸öKBASICPROP_EQUIPMENT½á¹¹)
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_EQUIPMENT_UNIQUE* KBPT_Equipment_Unique::GetRecord(int i) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (this==NULL || i<0 || i >=m_nNumOfEntries)
			return NULL;
	return (i >= 0 && i < m_nNumOfEntries) ?
		(((KBASICPROP_EQUIPMENT_UNIQUE*)m_pBuf) + i) : NULL;
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄÎ¨Ò»×°±¸ÊôÐÔ¼ÇÂ¼
	Èë¿Ú:	nDetailType: ¾ßÌåÀà±ð
			nParticularType: ÏêÏ¸Àà±ð
			nLevel: µÈ¼¶
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸Ã¼ÇÂ¼µÄÖ¸Õë
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KBASICPROP_EQUIPMENT_UNIQUE*
		KBPT_Equipment_Unique::FindRecord(IN int nDetailType,
										  IN int nParticularType,
										  IN int nLevel) const
{
	//_ASSERT(this != NULL);
	if (this == NULL) return NULL;
	const KBASICPROP_EQUIPMENT_UNIQUE* pData = NULL;
	// ÒÔÏÂÊ¹ÓÃË³Ðò²éÕÒµÄËã·¨. ÈôÔ­Ê¼Êý¾ÝÊÇ°´m_nDetailType¼°m_nLevelÅÅÐòµÄ,
	// Ôò¿É½øÐÐËã·¨ÓÅ»¯
	for (int i = 0; i < m_nNumOfEntries; ++i)
	{
		const KBASICPROP_EQUIPMENT_UNIQUE* pEqu;
		pEqu = GetRecord(i);
		//_ASSERT(NULL != pEqu);
		if (NULL == pEqu)
			continue;
		if (nDetailType == pEqu->m_nDetailType &&
			nParticularType == pEqu->m_nParticularType &&
			nLevel == pEqu->m_nLevel)
		{
			pData = pEqu;
			break;
		}
	}
	return pData;
}

//============================================================================

KBPT_MagicAttrib_TF::KBPT_MagicAttrib_TF()
{
	m_nSizeOfEntry = sizeof(KMAGICATTRIB_TABFILE);
	::strcpy(m_szTabFile, TABFILE_MAGICATTRIB);
	::memset(&m_naryMACount, 0, sizeof(m_naryMACount));
}

KBPT_MagicAttrib_TF::~KBPT_MagicAttrib_TF()
{

}
//Ä§·¨ÊôÐÔ±í
int KBPT_MagicAttrib_TF::LoadRecord(int i, KTabFile* pTF)
{
	//_ASSERT(pTF != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (pTF==NULL || i<0 || i >=m_nNumOfEntries)
		return NULL;
	// ³õÊ¼»¯
	KMAGICATTRIB_TABFILE* pBuf = (KMAGICATTRIB_TABFILE*)m_pBuf;
	pBuf = pBuf + i;	// ¶ÁÈëµÄÊôÐÔ¼ÇÔÚ pBuf ËùÖ¸½á¹¹ÖÐ
	const PROPINFO	aryPI[] =
	{
		{ PI_VARTYPE_CHAR,	pBuf->m_szName, sizeof(pBuf->m_szName)},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nPos), 0},    //ÊÇ·ñÇ°×º£¿
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nClass), 0},  //ÎåÐÐ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_nLevel), 0},  //µÈ¼¶
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_MagicAttrib.nPropKind), 0},  // Ä§·¨ÊôÐÔÖÖÀà
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_MagicAttrib.aryRange[0].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_MagicAttrib.aryRange[0].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_MagicAttrib.aryRange[1].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_MagicAttrib.aryRange[1].nMax), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_MagicAttrib.aryRange[2].nMin), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_MagicAttrib.aryRange[2].nMax), 0},
		{ PI_VARTYPE_CHAR, pBuf->m_szIntro, sizeof(pBuf->m_szIntro)},  //ÃèÊö
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[0]), 0}, //¸÷¸ö²¿Î»µÄ³öÏÖ¸ÅÂÊ
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[1]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[2]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[3]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[4]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[5]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[6]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[7]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[8]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[9]), 0},
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[10]), 0},   //ÂíÆ¥
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[11]), 0},   //Ãæ¾ß
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[12]), 0},   //Åû·ç
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[13]), 0},   //Ó¡¼ø
		{ PI_VARTYPE_INT,  (char*)&(pBuf->m_DropRate[14]), 0},   //ÊÎÆ·
	};

	// Öð¸ö¶ÁÈë¸÷ÏîÊôÐÔ
	if (FALSE == ::LoadRecord(pTF, i, aryPI, sizeof(aryPI)/ sizeof(aryPI[0])))
		return FALSE;

    pBuf->m_nUseFlag = false;       // ³õÊ¼»¯ÎªÃ»ÓÐÊ¹ÓÃ

	// Êý¾ÝÍ³¼Æ£ºÃ¿ÖÖ×°±¸¿ÉÊÊÓÃµÄÄ§·¨ÊýÄ¿
	for (int n = 0; n < MATF_CBDR; ++n)
	{
		if (0 != pBuf->m_DropRate[n])
		{
			int nPos;
			nPos = (0 == pBuf->m_nPos) ? 1 : 0;
			m_naryMACount[nPos][n]++;
		}
	}
	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ:	»ñÈ¡Ö¸¶¨µÄÄ§·¨ÊôÐÔ
	Èë¿Ú:	i: ÒªÇó»ñÈ¡µÚiÏîÄ§·¨ÊôÐÔ
	³ö¿Ú:	³É¹¦Ê±·µ»ØÖ¸Ïò¸ÃÄ§·¨ÊôÐÔµÄÖ¸Õë(Ò»¸öKMAGICATTRIB_TABFILE½á¹¹)
			Ê§°ÜÊ±·µ»ØNULL
******************************************************************************/
const KMAGICATTRIB_TABFILE* KBPT_MagicAttrib_TF::GetRecord(int i) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfEntries);
	if (this==NULL || i<0 || i >=m_nNumOfEntries)
			return NULL;
	return (i >= 0 && i < m_nNumOfEntries) ? (((KMAGICATTRIB_TABFILE*)m_pBuf) + i) : NULL;
}

void KBPT_MagicAttrib_TF::GetMACount(int* pnCount) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(pnCount != NULL);
	if (this == NULL) return;
	if (pnCount == NULL) return;

	::memcpy(pnCount, m_naryMACount, sizeof(m_naryMACount));
}

//============================================================================
// Magic Item Index Table Class
KBPT_ClassMAIT::KBPT_ClassMAIT()
{
	m_pnTable = NULL;
	m_nSize = 0;
	m_nNumOfValidData = 0;
}

KBPT_ClassMAIT::~KBPT_ClassMAIT()
{
    m_nSize = 0;
	m_nNumOfValidData = 0;

	if (m_pnTable)
    {
        delete []m_pnTable;
        m_pnTable = NULL;
    }
}

/******************************************************************************
	¹¦ÄÜ:	½«Ë÷Òý±íÇå¿Õ£¬²¢²»ÊÍ·ÅÄÚ´æ
	Èë¿Ú:
	³ö¿Ú:	³É¹¦·µ»Ø·ÇÁã£¬Ê§°Ü·µ»ØÁã
	ËµÃ÷:
******************************************************************************/
int KBPT_ClassMAIT::Clear()
{
    m_nNumOfValidData = 0;

    return true;
}


/******************************************************************************
	¹¦ÄÜ:	´ÓË÷Òý±íÖÐÈ¡Öµ
	Èë¿Ú:	i: ´ÓË÷Òý±íÖÐ¸ÃÎ»ÖÃÈ¡Öµ
	³ö¿Ú:	·µ»ØËùÈ¡µÄÖµ
	ËµÃ÷:	Èô·µ»ØÖµÎªnItemIndex, Ôòµ÷ÓÃKLibOfBPT::m_BPTMagicAttrib.GetRecord(nItemIndex)
            ¿É»ñµÃ¶ÔÓ¦µÄÄ§·¨ÊôÐÔ
******************************************************************************/
int KBPT_ClassMAIT::Get(int i) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(i >= 0 && i < m_nNumOfValidData);
	if (this==NULL || i<0 || i >=m_nNumOfValidData)
		return NULL;

    if (!m_pnTable) return NULL;

	return m_pnTable[i];
}


/******************************************************************************
	¹¦ÄÜ:	¸øË÷Òý±í²åÈëÒ»¸öÐÂµÄÖµ
	Èë¿Ú:	nItemIndex: Óû¸³Ö®Öµ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, m_pnTable[m_nNumOfValidData]ÎªËù¸³Ö®Öµ
			m_nNumOfValidDataÖ¸ÏòÏÂÒ»¿ÕÎ»
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
int KBPT_ClassMAIT::Insert(int nItemIndex)
{
    int nResult = false;

	//_ASSERT(this != NULL);
	//_ASSERT(nItemIndex >= 0);	// nData ÎªÊý×éÏÂ±ê
    if (this==NULL || nItemIndex<0)
      return false;

    if (!m_pnTable)
    {
        m_pnTable = new int [4];
        if (!m_pnTable)
            goto Exit0;

        m_nNumOfValidData = 0;
        m_nSize = 4;
    }

    if (m_nNumOfValidData >= m_nSize)
    {
        int *pnaryTemp = new int [m_nSize + 8]; //Ô­À´ÊÇ 8
        if (!pnaryTemp)
            goto Exit0;

        memcpy(pnaryTemp, m_pnTable, m_nNumOfValidData * sizeof(int));

        m_nSize += 8;//Ô­À´ÊÇ 8
        delete []m_pnTable;
        m_pnTable = pnaryTemp;
        pnaryTemp = NULL;
    }

	m_pnTable[m_nNumOfValidData++] = nItemIndex;

    nResult = true;
Exit0:
    return nResult;
}



//============================================================================

KBPT_ClassifiedMAT::KBPT_ClassifiedMAT()
{
	m_pnTable = NULL;
	m_nSize = 0;
	m_nNumOfValidData = 0;
}

KBPT_ClassifiedMAT::~KBPT_ClassifiedMAT()
{
	ReleaseMemory();
}

/******************************************************************************
	¹¦ÄÜ:	ÎªË÷Òý±í·ÖÅäÄÚ´æ
	Èë¿Ú:	nCount: Ë÷Òý±í´óÐ¡(Êý¾ÝÏîµÄÊýÄ¿)
	³ö¿Ú:	m_pnTable Ö¸Ïò·ÖÅäµÄÄÚ´æ
			m_nSize = nCount
******************************************************************************/
int KBPT_ClassifiedMAT::GetMemory(int nCount)
{
	//_ASSERT(this != NULL);
	//_ASSERT(nCount > 0);
	//_ASSERT(NULL == m_pnTable);
	//_ASSERT(0 == m_nSize);

	 if (this==NULL || nCount<0 || m_pnTable==NULL ||m_nSize<=0)
	     return false;

	int bEC = FALSE;
	int* pnBuf = new int[nCount];
	if (pnBuf)
	{
		m_pnTable = pnBuf;
		m_nSize = nCount;
		bEC = TRUE;
	}
	return bEC;
}

/******************************************************************************
	¹¦ÄÜ:	ÊÍ·ÅË÷Òý±íËùÓÃµÄÄÚ´æ
******************************************************************************/
void KBPT_ClassifiedMAT::ReleaseMemory()
{
	if (m_pnTable)
	{
		delete []m_pnTable;
		m_pnTable = NULL;
		m_nSize = 0;
	}
}

/******************************************************************************
	¹¦ÄÜ:	¸øË÷Òý±í¸³Öµ
	Èë¿Ú:	nData: Óû¸³Ö®Öµ
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, m_pnTable[m_nNumOfValidData]ÎªËù¸³Ö®Öµ
			m_nNumOfValidDataÖ¸ÏòÏÂÒ»¿ÕÎ»
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
int KBPT_ClassifiedMAT::Set(int nData)
{
	//_ASSERT(this != NULL);
	//_ASSERT(nData >= 0);	// nData ÎªÊý×éÏÂ±ê
	//_ASSERT(m_nNumOfValidData >= 0 && m_nNumOfValidData < m_nSize);
	if (this==NULL || nData<0 || m_nNumOfValidData<0 ||m_nNumOfValidData>=m_nSize)
		return false;


	m_pnTable[m_nNumOfValidData++] = nData;
	return TRUE;
}

/******************************************************************************
	¹¦ÄÜ:	´ÓË÷Òý±íÖÐÈ¡Öµ
	Èë¿Ú:	nIndex: ´ÓË÷Òý±íÖÐ¸ÃÎ»ÖÃÈ¡Öµ
	³ö¿Ú:	·µ»ØËùÈ¡µÄÖµ
	ËµÃ÷:	Èô·µ»ØÖµÎªi, Ôòµ÷ÓÃKLibOfBPT::m_BPTMagicAttrib.GetRecord(i)¿É»ñµÃ
			¶ÔÓ¦µÄÄ§·¨ÊôÐÔ
******************************************************************************/
int KBPT_ClassifiedMAT::Get(int nIndex) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(nIndex >= 0 && nIndex < m_nSize);
	//if (this==NULL || nIndex<0 || nIndex>=m_nSize)
	  // return false;

	return (nIndex >= 0 && nIndex < m_nSize) ? m_pnTable[nIndex] : 0;
}

/******************************************************************************
	¹¦ÄÜ:	½«Êý¾Ýµ¼³öµ½¸ø¶¨µÄ»º³åÇøÖÐ
	Èë¿Ú:	pnaryBuf: ¼¶³åÇøÖ¸Õë
			*pnCount: »º³åÇø¿ÉÈÝÄÉ¶àÉÙ¸öintÐÍÊý¾Ý
	³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, Êý¾Ý±»µ¼³öµ½»º³åÇøÖÐ, *pnCount ¸ø³öÊý¾Ý¸öÊý
			Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
int KBPT_ClassifiedMAT::GetAll(int* pnaryBuf, int* pnCount) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(pnaryBuf != NULL);
	//_ASSERT(pnCount != NULL);

	if (this==NULL || pnaryBuf==NULL || pnCount==NULL)
		  return false;

	int bEC = FALSE;
	if (pnaryBuf && pnCount && *pnCount > 0)
	{
		int nLength = *pnCount;
		if(m_nSize< nLength) nLength = m_nSize;
		::memcpy(pnaryBuf, m_pnTable, nLength*sizeof(int));
		*pnCount = nLength;
		bEC = TRUE;
	}
	return bEC;
}

