#include "KCore.h"
//#include "MyAssert.H"
#include "engine/KTabFile.h"
#include "engine/KRandom.h"
#include "KNpc.h"
#include "KItem.h"
#include "KItemSet.h"
#include "KNpcTemplate.h"    //�����ӵ�
//#include "../../Engine/src/Text.h"
/*#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif*/


#include "ImgRef.h"
#include "KPlayer.h"
#include "KRUImage.h"
//#include "../../Represent/iRepresent/iRepresentshell.h"
#include "KMagicDesc.h"
#include <time.h>
/////////////////////////////////
//#include "../../Headers/IClient.h"
#include "CoreShell.h"
////////////////////////////////
//#endif

KItem	*Item = NULL;//Item[MAX_ITEM];

int GetRandomNumber(int nMin, int nMax);
//extern  const KScript * g_GetScript(unsigned long dwScriptId);  //ԭ��û�е�
KItem::KItem()
{   //�ڴ��ʼ��
	::memset(&m_CommonAttrib,    0, sizeof(m_CommonAttrib));
	::memset(m_aryBaseAttrib,    0, sizeof(m_aryBaseAttrib));
	::memset(m_aryRequireAttrib, 0, sizeof(m_aryRequireAttrib));
	::memset(m_aryMagicAttrib,   0, sizeof(m_aryMagicAttrib));
	::memset(m_ronMagicAttrib,   0, sizeof(m_ronMagicAttrib));       //����
//	::memset(m_BaoShiMagicAttrib,   0, sizeof(m_BaoShiMagicAttrib)); //��ʯ
	::memset(&m_GeneratorParam,	 0, sizeof(m_GeneratorParam));
    ::memset(&m_TempMagicAttrib,	 0, sizeof(m_TempMagicAttrib));  //(��ʱ�����ħ������ֵ,����ǿ��)
	::memset(&m_TempRMagicAttrib,	 0, sizeof(m_TempRMagicAttrib)); //
//	::memset(&m_TempBMagicAttrib,	 0, sizeof(m_TempBMagicAttrib));
	::memset(&m_TempPlatinaAttrib,	 0, sizeof(m_TempPlatinaAttrib));
	::memset(&m_yinMagicAttrib,	 0, sizeof(m_yinMagicAttrib));

	m_nCurrentDur = -1;

//	nstrNoteInfo.clear();

//#ifndef _SERVER
	::memset(&m_Image,   0, sizeof(KRUImage));
//#endif
	m_nIndex = 0;
}

KItem::~KItem()
{
}

void* KItem::GetRequirement(IN int nReq)
{
	int i = sizeof(m_aryRequireAttrib)/sizeof(m_aryRequireAttrib[0]);
	if (nReq >= i)
		return NULL;
	return &m_aryRequireAttrib[nReq];
}

int KItem::CheckRequirement(IN int nReq)
{
	int i = sizeof(m_aryRequireAttrib)/sizeof(m_aryRequireAttrib[0]);
	int nReg=FALSE;
//	m_aryRequireAttrib[nReq];

	for (int k=0;k<i;++k)
	{
		if (m_aryRequireAttrib[k].nAttribType==nReq)
		{
		   nReg=TRUE;
		   break;
		}
	}

	return nReg;
}

/******************************************************************************
����:	��item�ϵ�ħ��Ӧ�õ�NPC����
��ڣ�	pNPC: ָ��NPC��ָ�룬nMagicAcive���򿪵�����������Ŀ
����:	ħ����Ӧ�á�
		���幤����KNpc�ĳ�Ա������ɡ�
		KItem ������û�г�Ա�������޸�
******************************************************************************/
void KItem::ApplyMagicAttribToNPC(IN KNpc* pNPC, IN int nMagicActive,int *nType) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(nMagicActive >= 0);
     if (!pNPC || nMagicActive<0)
		return;

	int nCount = nMagicActive;
	int i;

	// �������Ե���NPC
	for (i = 0; i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); ++i)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryBaseAttrib[i]);
		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
		}
	}
	// ħ�����Ե���NPC
	for (i = 0; i < sizeof(m_aryMagicAttrib)/sizeof(m_aryMagicAttrib[0]); ++i)
	{//��װ ��װ ��װ �׽�װ
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			if (i & 1)						// Ϊ�������Ǻ�׺��i���㿪ʼ��
			{// 1 3 5
				if (nCount >0)
				{
					pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
					nCount--;
				}
			}
			else
			{// 2 4 6
				    pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
			}
		}
	}

	//for (i = 0; i < sizeof(m_yinMagicAttrib)/sizeof(m_yinMagicAttrib[0]); ++i)//sizeof(m_ronMagicAttrib)/sizeof(m_ronMagicAttrib[0])
	//���ص�����
	if (nType[0]==3)
	{//�ƽ��׽�
			const KItemNormalAttrib* pRAttrib;
			 if (nType[1] >=5 && nType[1] < 10)
			 {
			    pRAttrib = &(m_yinMagicAttrib[0]);	//��һ������
			    if (INVALID_ATTRIB != pRAttrib->nAttribType)
				{
				   pNPC->ModifyAttrib(pNPC->m_Index, (void *)pRAttrib);
				}
			 }
			 else  if (nType[1]>=10)
			 {
				 pRAttrib = &(m_yinMagicAttrib[0]);	//��һ������
				 if (INVALID_ATTRIB != pRAttrib->nAttribType)
				 {
					 pNPC->ModifyAttrib(pNPC->m_Index, (void *)pRAttrib);
				 }
				 pRAttrib = &(m_yinMagicAttrib[1]);	//�ڶ�������
				 if (INVALID_ATTRIB != pRAttrib->nAttribType)
				 {
					 pNPC->ModifyAttrib(pNPC->m_Index, (void *)pRAttrib);
				 }
			 }
	}

 //������ʶ
	for (i = 0; i < sizeof(m_ronMagicAttrib)/sizeof(m_ronMagicAttrib[0]); ++i)//sizeof(m_ronMagicAttrib)/sizeof(m_ronMagicAttrib[0])
	{
		  const KItemNormalAttrib* pRAttrib;
		  pRAttrib = &(m_ronMagicAttrib[i]);

		  if (nType[0]==1)
		  {//����װ
			  // �������Ե���NPC
			  if (nType[1]>-1)
			  {//�Ѿ���ʶ �� �Ƹ�����
		          if (INVALID_ATTRIB != pRAttrib->nAttribType)
				  {
			        pNPC->ModifyAttrib(pNPC->m_Index, (void *)pRAttrib);
				  }
			  }
		  }
		  else
		  {//������ֱ������
			  if (INVALID_ATTRIB != pRAttrib->nAttribType)
			  {
				  pNPC->ModifyAttrib(pNPC->m_Index, (void *)pRAttrib);
			  }
		  }
	}
	// ��ʯ���Ե���NPC
/*	for (i = 0; i < sizeof(m_BaoShiMagicAttrib)/sizeof(m_BaoShiMagicAttrib[0]); ++i)//sizeof(m_ronMagicAttrib)/sizeof(m_ronMagicAttrib[0])
	{
		const KItemNormalAttrib* pRAttrib;
		pRAttrib = &(m_BaoShiMagicAttrib[i]);
		if (INVALID_ATTRIB != pRAttrib->nAttribType)
		{
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)pRAttrib);
		}
	} */
}

/******************************************************************************
����:	��item�ϵ�ħ����NPC�����Ƴ�
��ڣ�	pNPC: ָ��NPC��ָ�룬nMagicAcive���򿪵�����������Ŀ
����:	ħ����Ӧ�á�
		���幤����KNpc�ĳ�Ա������ɡ�
		KItem ������û�г�Ա�������޸�
******************************************************************************/
void KItem::RemoveMagicAttribFromNPC(IN KNpc* pNPC, IN int nMagicActive,IN int *nType) const
{
	//_ASSERT(this != NULL);
	//_ASSERT(nMagicActive >= 0);
    if (!pNPC || nMagicActive<0)
    	return;

	int nCount = nMagicActive;
	int	i;

	// �������Ե���NPC
	for (i = 0; i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); ++i)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryBaseAttrib[i]);
		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			KItemNormalAttrib RemoveAttrib;
			RemoveAttrib.nAttribType = pAttrib->nAttribType;
			RemoveAttrib.nValue[0]   = -pAttrib->nValue[0];
			RemoveAttrib.nValue[1]   = -pAttrib->nValue[1];
			RemoveAttrib.nValue[2]   = -pAttrib->nValue[2];
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
		}
	}

	for (i = 0; i < sizeof(m_aryMagicAttrib)/sizeof(m_aryMagicAttrib[0]); ++i)// ħ������
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)		// TODO: Ϊ -1 ����һ������?
		{
			KItemNormalAttrib RemoveAttrib;
			if (i & 1)						            //Ϊ�������Ǻ�׺��i���㿪ʼ��������
			{// 1 3 5
				if (nCount >0)
				{
					RemoveAttrib.nAttribType = pAttrib->nAttribType;
					RemoveAttrib.nValue[0]   = -pAttrib->nValue[0];
					RemoveAttrib.nValue[1]   = -pAttrib->nValue[1];
					RemoveAttrib.nValue[2]   = -pAttrib->nValue[2];
					pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
					nCount--;
				}
			}
			else
			{// 2 4 6
				RemoveAttrib.nAttribType = pAttrib->nAttribType;
				RemoveAttrib.nValue[0]   = -pAttrib->nValue[0];
				RemoveAttrib.nValue[1]   = -pAttrib->nValue[1];
				RemoveAttrib.nValue[2]   = -pAttrib->nValue[2];
				pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
			}
		}
	}

//-----------------�ƽ� ��׽�� ��������--------------------------------
		if  (nType[0]==3 && nType[1]>5 && nType[1]<10)
		{
			const KItemNormalAttrib* pAttrib;
		    pAttrib = &(m_yinMagicAttrib[0]);
		    if (INVALID_ATTRIB != pAttrib->nAttribType)		// TODO: Ϊ -1 ����һ������?
			{
				KItemNormalAttrib RemoveAttrib;
				RemoveAttrib.nAttribType = pAttrib->nAttribType;
				RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
				RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
				RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
				pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
			}
		}
		else if  (nType[0]==3 && nType[1]>=10)
		{
			for (i = 0; i < sizeof(m_yinMagicAttrib)/sizeof(m_yinMagicAttrib[0]); ++i)//��������
			{
				const KItemNormalAttrib* pAttrib;
				pAttrib = &(m_yinMagicAttrib[i]);
				if (INVALID_ATTRIB != pAttrib->nAttribType)		// TODO: Ϊ -1 ����һ������?
				{
					KItemNormalAttrib RemoveAttrib;
					RemoveAttrib.nAttribType = pAttrib->nAttribType;
					RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
					RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
					RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
					pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
				}

			}
		}
//-----------------------------------------------------------------------------------------
	for (i = 0; i < sizeof(m_ronMagicAttrib)/sizeof(m_ronMagicAttrib[0]); ++i)// ����ħ������
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_ronMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)		// TODO: Ϊ -1 ����һ������?
		{
			        KItemNormalAttrib RemoveAttrib;
				    RemoveAttrib.nAttribType = pAttrib->nAttribType;
				    RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
				    RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
				    RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
				    pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
		}
	}

/*	for (i = 0; i < sizeof(m_BaoShiMagicAttrib)/sizeof(m_BaoShiMagicAttrib[0]); ++i)// ��ʯħ������
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_BaoShiMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)		// TODO: Ϊ -1 ����һ������?
		{
			KItemNormalAttrib RemoveAttrib;
			RemoveAttrib.nAttribType = pAttrib->nAttribType;
			RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
			RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
			RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
		}
	} */
}

/******************************************************************************
����:	��item�ϵĵ�N������ħ������Ӧ�õ�NPC����
��ڣ�	pNPC: ָ��NPC��ָ��
����:	ħ����Ӧ�á�
		���幤����KNpc�ĳ�Ա������ɡ�
		KItem ������û�г�Ա�������޸�   ������  δ���������
******************************************************************************/
void KItem::ApplyHiddenMagicAttribToNPC(IN KNpc* pNPC, IN int nMagicActive) const
{
	//_ASSERT(this != NULL);

	if (!pNPC || nMagicActive < 0)
		return;

	const KItemNormalAttrib* pAttrib; //��ͨ����
	pAttrib = &(m_aryMagicAttrib[(nMagicActive << 1) - 1]);	// ��׺Ϊ�����������Գ�2��һ
	if (-1 != pAttrib->nAttribType)
	{
		pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
	}
}

/******************************************************************************
����:	��item�ϵĵ�N������ħ�����Դ�NPC�����Ƴ�
��ڣ�	pNPC: ָ��NPC��ָ�룬nMagicActive����n��ħ������
����:	ħ�����Ƴ���
		���幤����KNpc�ĳ�Ա������ɡ�
		KItem ������û�г�Ա�������޸�
******************************************************************************/
void KItem::RemoveHiddenMagicAttribFromNPC(IN KNpc* pNPC, IN int nMagicActive) const
{
	//_ASSERT(this != NULL);
	if (!pNPC || nMagicActive <0)
		return;

	const KItemNormalAttrib* pAttrib;
	pAttrib = &(m_aryMagicAttrib[(nMagicActive << 1) - 1]);	// ��׺Ϊ�����������Գ�2��һ
	if (-1 != pAttrib->nAttribType)
	{
		KItemNormalAttrib RemoveAttrib;
		RemoveAttrib.nAttribType = pAttrib->nAttribType;
		RemoveAttrib.nValue[0]   = -pAttrib->nValue[0];
		RemoveAttrib.nValue[1]   = -pAttrib->nValue[1];
		RemoveAttrib.nValue[2]   = -pAttrib->nValue[2];
		pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
	}
}

/******************************************************************************
����:	���������ļ��е�����,Ϊitem�ĸ����ֵ
��ڣ�	pData: �������������ļ�������
����:	�ɹ�ʱ���ط���, ���³�Ա������ֵ:
			m_CommonAttrib,m_aryBaseAttrib,m_aryRequireAttrib
		ʧ��ʱ������
˵��:	CBR: Common,Base,Require
******************************************************************************/
int KItem::SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT* pData)
{
	//_ASSERT(pData != NULL);

	int bEC = FALSE;

	if (!pData)
		return bEC;


	if (pData)
	{
		//SetAttrib_Common(pData);
		*this = *pData;		// ���������
		SetAttrib_Base(pData->m_aryPropBasic); //��������
		SetAttrib_Req(pData->m_aryPropReq);    //��������
		bEC = TRUE;
	}
	return bEC;
}

int KItem::SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT_GOLD* pData)  //�ƽ�װ���� ������ �������� ����
{
	//_ASSERT(pData != NULL);

	int bEC = FALSE;
	if (!pData)
			return bEC;

	if (pData)
	{
		//SetAttrib_Common(pData);
		*this = *pData;		                   // ���������
		SetAttrib_Base(pData->m_aryPropBasic); //����
		SetAttrib_Req(pData->m_aryPropReq);    //����
		bEC = TRUE;
	}
	return bEC;
}

int KItem::SetAttrib_BCBR(IN const KBASICPROP_EQUIPMENT_PLATINA* pData)
{
	//_ASSERT(pData != NULL);

	int bEC = FALSE;
	if (!pData)
			return bEC;


	if (pData)
	{
		*this = *pData;		// ���������
		SetAttrib_Base(pData->m_aryPropBasic); //��������
		SetAttrib_Req(pData->m_aryPropReq);    //��������
		bEC = TRUE;
	}
	return bEC;
}

int KItem::SetAttrib_RCBR(IN const KBASICPROP_EQUIPMENT_GOLD* pData,int nIsDel)
{
	//_ASSERT(pData != NULL);

	int bEC = FALSE;
	if (!pData)
			return bEC;

	if (pData)
	{
		//SetAttrib_Common(pData);
		*this = *pData;		// ���������
		SetAttrib_Base(pData->m_aryPropBasic); //��������
		SetAttrib_Req(pData->m_aryPropReq,nIsDel);    //��������

       // memcpy(pData->m_aryMagicAttribs, bb, sizeof(int) * 6);
        ///  =bb;

      //  SetAttrib_Mof(pData->m_aryMagicAttribs); //ħ������
		bEC = TRUE;
	}
	return bEC;
}

int KItem::SetAttrib_Base(const KEQCP_BASIC* pBasic)
{
	for (int i = 0;
		 i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); ++i)
	{
		KItemNormalAttrib* pDst;
		const KEQCP_BASIC* pSrc;
		pDst = &(m_aryBaseAttrib[i]);
		pSrc = &(pBasic[i]);
		pDst->nAttribType = pSrc->nType;  //0
		pDst->nValue[0] = ::GetRandomNumber(pSrc->sRange.nMin, pSrc->sRange.nMax);  //�������ֵ
		pDst->nValue[1] = 0;	// RESERVED
		pDst->nValue[2] = 0;	// RESERVED
		if (pDst->nAttribType == magic_durability_v)
			SetDurability(pDst->nValue[0]);
	}

	if (m_nCurrentDur == 0)	// ˵��û���;ö�����
		m_nCurrentDur = -1;

	return TRUE;
}
//��������
int KItem::SetAttrib_Req(const KEQCP_REQ* pReq,int inDel)
{
	for (int i = 0;
	i < sizeof(m_aryRequireAttrib)/sizeof(m_aryRequireAttrib[0]); ++i)
	{
		KItemNormalAttrib* pDst;
		pDst = &(m_aryRequireAttrib[i]);
		if (inDel && pReq[i].nType==39)
		{//ȥ����������
			pDst->nAttribType =0;
			pDst->nValue[0] = 0;
			pDst->nValue[1] = 0;	// RESERVED
			pDst->nValue[2] = 0;	// RESERVED
		}
		else
		{
			pDst->nAttribType = pReq[i].nType;
			pDst->nValue[0] = pReq[i].nPara;
			pDst->nValue[1] = 0;	// RESERVED
			pDst->nValue[2] = 0;	// RESERVED
		}

	}
	return TRUE;
}


//��������

int KItem::SetAttrib_RON(IN const KItemNormalAttrib* pMA)
{
	if (NULL == pMA)
	{ return FALSE;}

	for (int i = 0;i < sizeof(m_ronMagicAttrib)/sizeof(m_ronMagicAttrib[0]); ++i)
	{

			m_ronMagicAttrib[i] = pMA[i]; //�Ǽ��ܱ��  ��ֵ�����ܱ������
			m_TempRMagicAttrib[i] = pMA[i];
			// ����ĥ��
			if (m_ronMagicAttrib[i].nAttribType == magic_indestructible_b)
			{
				SetDurability(-1);
			}

	}
	return 1;
}
//��ʯ����
int KItem::SetAttrib_Bao(IN const KItemNormalAttrib* pMA)
{
/*	if (NULL == pMA)
	{ _ASSERT(FALSE); return FALSE; }
	for (int i = 0;i < sizeof(m_BaoShiMagicAttrib)/sizeof(m_BaoShiMagicAttrib[0]); ++i)
	{

		m_BaoShiMagicAttrib[i] = pMA[i]; //�Ǽ��ܱ��  ��ֵ�����ܱ������
		m_TempBMagicAttrib[i] = pMA[i];  //����������Ե�ԭʼֵ(����ǿ��)
		// ����ĥ��
		if (m_BaoShiMagicAttrib[i].nAttribType == magic_indestructible_b)
		{
			SetDurability(-1);
		}

	}  */
	return TRUE;
}


//��������
int KItem::SetAttrib_Yin(IN const KItemNormalAttrib* pMA)
{
	if (NULL == pMA)
	{ return FALSE; }
	for (int i = 0;i < sizeof(m_yinMagicAttrib)/sizeof(m_yinMagicAttrib[0]); ++i)
	{

		m_yinMagicAttrib[i] = pMA[i];    //�Ǽ��ܱ��  ��ֵ�����ܱ������
		//m_TempBMagicAttrib[i] = pMA[i];  //����������Ե�ԭʼֵ(����ǿ��)
		// ����ĥ��
		if (m_yinMagicAttrib[i].nAttribType == magic_indestructible_b)
		{
			SetDurability(-1);
		}

	}
	return 1;
}

/******************************************************************************
����:	���ݴ��������, Ϊitem��ħ�����Ը���ֵ
��ڣ�	pMA: ��������
����:	�ɹ�ʱ���ط���, ���³�Ա������ֵ:
			m_aryMagicAttrib
		ʧ��ʱ������
	//	KMagicAttrib* pAttrib = (KMagicAttrib *)pData
******************************************************************************/
int KItem::SetAttrib_MA(IN const KItemNormalAttrib* pMA)
{
	if (NULL == pMA)
		{return FALSE; }
    for (int i = 0; i < sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); ++i)
//	for (int i = 0; i <6; ++i)
	{
		m_aryMagicAttrib[i]  = pMA[i]; //ħ����ţ�����ֵû������
		m_TempMagicAttrib[i] = pMA[i];//�������ħ�����Ե�ԭʼֵ������ǿ���ӳɵģ�
		// ����ĥ��
		if (m_aryMagicAttrib[i].nAttribType == magic_indestructible_b)
		{
			SetDurability(-1);
		}
	}
	return TRUE;
}

/******************************************************************************
����:	���ݴ��������, Ϊitem��ħ�����Ը���ֵ
��ڣ�	pMA: ��������
����:	�ɹ�ʱ���ط���, ���³�Ա������ֵ:
			m_aryMagicAttrib
		ʧ��ʱ������
******************************************************************************/
int KItem::SetAttrib_MAB(IN const KMACP* pMA)  // ����ħ������
{
	if (NULL == pMA)
		{return FALSE; }

	for (int i = 0; i < sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); ++i)
	{
		const KMACP* pSrc;
		KItemNormalAttrib* pDst;
		pSrc = &(pMA[i]);
		pDst = &(m_aryMagicAttrib[i]);

		pDst->nAttribType = pSrc->nPropKind;
		pDst->nValue[0] =  ::GetRandomNumber(pSrc->aryRange[0].nMin, pSrc->aryRange[0].nMax);
		pDst->nValue[1] =  ::GetRandomNumber(pSrc->aryRange[1].nMin, pSrc->aryRange[1].nMax);
		pDst->nValue[2] =  ::GetRandomNumber(pSrc->aryRange[2].nMin, pSrc->aryRange[2].nMax);
	}
	return TRUE;
}
//����ָ���� ħ���ƽ�����
int KItem::SetAttrib_MAC(IN const int* pMA)
{
	if (NULL == pMA)
	   {return FALSE; }

	KTabFile MagicTab;
	MagicTab.Load(TABFILE_MAGICAGOLD_PATH);
	for (int i = 0; i < sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); ++i)
	{
		const int* pSrc;
		KItemNormalAttrib* pDst;
		pSrc = &(pMA[i]);
		pDst = &(m_aryMagicAttrib[i]);

		int nType,nLow,nHigh;
		MagicTab.GetInteger(*pSrc + 1,"属性调整类别",0,&nType);
		pDst->nAttribType = nType;
		MagicTab.GetInteger(*pSrc + 1,"参数1最小值",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"参数1最大值",0,&nHigh);
		pDst->nValue[0] =  ::GetRandomNumber(nLow, nHigh);
		MagicTab.GetInteger(*pSrc + 1,"参数2最小值",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"参数2最大值",0,&nHigh);
		pDst->nValue[1] =  ::GetRandomNumber(nLow, nHigh);
		MagicTab.GetInteger(*pSrc + 1,"参数3最小值",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"参数3最大值",0,&nHigh);
		pDst->nValue[2] =  ::GetRandomNumber(nLow, nHigh);
	}
	MagicTab.Clear();
	return TRUE;
}
//��װ
void KItem::operator = (const KBASICPROP_EQUIPMENT& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nParticularTypea= sData.m_nParticularTypea; //����2
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = 0;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nPriceXu		 = sData.m_nPriceXu;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nUseMap         = 0;
	pCA->nUseKind	     = 0;
	pCA->nRes            = 0;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->nSixSkill		 = 0;
	pCA->nTenSkill		 = 0;
	pCA->nGoldId		 = 0;
	pCA->nIsPlatina	     = 0;	         //�׽�
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsWhere        = 0;
//	pCA->nYingNuma       = sData.m_nYingNuma;               //��������1
//    pCA->nYingNumb       = sData.m_nYingNumb;               //��������2
	pCA->nRongNum        = sData.m_nRongNum;               //������������
    pCA->nWengangPin     = sData.m_nWengangPin;          //�����Ƹ�Ʒ��
    pCA->nBinfujiazhi    = sData.m_nBinfujiazhi;         //������ֵ
	pCA->nChiBangRes     =0;              //����������
	pCA->nRongpiont      =0;
	pCA->nIsBang         =0;
    pCA->nIsKuaiJie      =0;
	pCA->nISMagic        =0;     //�Ƿ�������
	pCA->nSkillType      =0;     //����ħ���к�
	pCA->nMagicID        =0;     //����ID
    pCA->nIsUse          =0;
	pCA->nLianjieFlg     =0;
	pCA->nCanUse         =1;     //�Ƿ����ʹ��
	pCA->nSellModel      =0;
	::strcpy(pCA->szItemName, sData.m_szName);
	//::strcpy(pCA->szScript,"");
	pCA->szScript = NULL;
	::strcpy(pCA->szWonName,"系统");

	/*pCA->szItemName = sData.m_szName;
	pCA->szScript = NULL;
	pCA->szWonName = "ϵͳ";*/

	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->LimitTime.bMin = 0;

	/*char szIntro[516];
	ZeroMemory(szIntro,sizeof(szIntro));
	sprintf(szIntro,"%s", sData.m_szIntro);
	sprintf(pCA->ItmeInfo,szIntro);*/
	pCA->ItmeInfo = sData.m_szIntro;
#ifndef _SERVER
	pCA->uPrice = 0;
	pCA->uLianjieFlg =0;
	pCA->ncSellModel =0;
	pCA->szImageName = sData.m_szImageName;
	//::strcpy(pCA->szImageName, sData.m_szImageName);
	sprintf(pCA->szIntro, "%s", sData.m_szIntro);
	//pCA->szIntro = sData.m_szIntro;
    ///////////////////////////////////////////
    m_Image.Color.Color_b.a = 255;
    m_Image.nFrame = 0;
    m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
    m_Image.nType = ISI_T_SPR;
    //::strcpy(m_Image.szImage, pCA->szImageName);
	sprintf(m_Image.szImage,"%s", pCA->szImageName);
    m_Image.uImage = 0;
	/*KImageParam	Param;
	if (g_pRepresent)
	{
	  if (!g_pRepresent->GetImageParam(m_Image.szImage, &Param, ISI_T_SPR))
	  {//�������û�����SPR ������Ĭ�ϵ�SPR·��
		 sprintf(m_Image.szImage,"\\spr\\others\\�ʺ�.spr");
		 sprintf(pCA->szIntro,"��ʾ:�ͻ�����Դ����,�����Ϊ���¿ͻ���!");
	  }
	}*/

#endif
}

void KItem::operator = (const KBASICPROP_MEDMATERIAL& sData)
{
	// ��ֵ: ��ͬ���Բ���
	KItemCommonAttrib* pCA = &(m_CommonAttrib);
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nPriceXu		 = sData.m_nPriceXu;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nUseMap         = 0;
	pCA->nUseKind	     = 0;
	pCA->nRes            = 0;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->nGoldId		 = 0;
	pCA->nIsPlatina	     = 0;	         //�׽�
	pCA->nSixSkill		 = 0;
	pCA->nTenSkill		 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsWhere        = 0;
//	pCA->nYingNuma        = 0;               //��������1
//    pCA->nYingNumb        = 0; //��������2
	pCA->nRongNum        = 0;               //������������
    pCA->nWengangPin     = 0;            //�����Ƹ�Ʒ��
    pCA->nBinfujiazhi    = 0;            //������ֵ
	pCA->nChiBangRes     = 0;            //����������
	pCA->nRongpiont      =0;
	pCA->nIsBang         =0;
	pCA->nIsKuaiJie      =1;
	pCA->nISMagic        =0;     //�Ƿ�������
	pCA->nSkillType      =0;     //����ID
	pCA->nMagicID        =0;     //����ID
	pCA->nIsUse          =0;
	pCA->nLianjieFlg     =0;
	pCA->nSellModel      =0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//::strcpy(pCA->szScript,	   "");
	pCA->szScript = NULL;
	::strcpy(pCA->szWonName,"系统");

	/*pCA->szItemName = sData.m_szName;
	pCA->szScript = NULL;
	pCA->szWonName = "ϵͳ"; */

	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->LimitTime.bMin = 0;
	/*char szIntro[516];
	ZeroMemory(szIntro,sizeof(szIntro));
	sprintf(szIntro,"%s", sData.m_szIntro);
	sprintf(pCA->ItmeInfo,szIntro);*/
	pCA->ItmeInfo = sData.m_szIntro;

	pCA->uPrice = 0;
	pCA->uLianjieFlg =0;
	pCA->ncSellModel =0;

	pCA->szImageName = sData.m_szImageName;
	//pCA->szIntro     = sData.m_szIntro;
//	::strcpy(pCA->szImageName, sData.m_szImageName);
	sprintf(pCA->szIntro, "%s", sData.m_szIntro);

	// ��ֵ: �������Բ���
	KItemNormalAttrib* pBA = m_aryBaseAttrib;
	pBA[0].nAttribType = sData.m_nAttrib1_Type;
	pBA[0].nValue[0]   = sData.m_nAttrib1_Para;
	pBA[1].nAttribType = sData.m_nAttrib2_Type;
	pBA[1].nValue[0]   = sData.m_nAttrib2_Para;
	pBA[2].nAttribType = sData.m_nAttrib3_Type;
	pBA[2].nValue[0]   = sData.m_nAttrib3_Para;

	// ��ֵ: �������Բ���: ��
	// ��ֵ: ħ�����Բ���: ��

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
	/*KImageParam	Param;
	if (g_pRepresent)
	{
		if (!g_pRepresent->GetImageParam(m_Image.szImage, &Param, ISI_T_SPR))
		{//�������û�����SPR ������Ĭ�ϵ�SPR·��
			sprintf(m_Image.szImage,"\\spr\\others\\�ʺ�.spr");
			sprintf(pCA->szIntro,"��ʾ:�ͻ�����Դ����,�����Ϊ���¿ͻ���!");
		}
	}*/
}
//��ʯ����Ʒ
void KItem::operator = (const KBASICPROP_MINE& sData)
{
	// ��ֵ: ��ͬ���Բ���
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nPriceXu		 = sData.m_nPriceXu;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nUseMap         = 0;
	pCA->nUseKind	     = 0;
	pCA->nRes            = 0;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nSixSkill		 = 0;
	pCA->nTenSkill		 = 0;
	pCA->nBigSet		 = sData.m_nDelet;   //�Ƿ�ʹ����ɾ��
	pCA->nGoldId		 = 0;
	pCA->nIsPlatina	     = 0;	         //�׽�
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsWhere        = 0;
//	pCA->nYingNuma       = 0;                //��������1
//    pCA->nYingNumb       = 0;                //��������2
	pCA->nRongNum        = 0;                //������������
    pCA->nWengangPin     = 0;                //�����Ƹ�Ʒ��
    pCA->nBinfujiazhi    = 0;                //������ֵ
	pCA->nChiBangRes     = 0;            //����������
	pCA->nRongpiont      = 0;
	pCA->nIsBang         = 0;
	pCA->nIsKuaiJie      = sData.m_IsKuaiJie; //�Ƿ�����
	pCA->nISMagic        = 0;                //�Ƿ�������
	pCA->nSkillType      = 0;                //����ħ������
	pCA->nMagicID        = sData.m_Magic[0]; //����ID
	pCA->nIsUse          = sData.m_IsUse;    //�Ƿ�����ʹ��
	pCA->nLianjieFlg     =0;
	pCA->nSellModel      =0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//::strcpy(pCA->szScript,	   sData.m_szScript);
	pCA->szScript = sData.m_szScript;
	::strcpy(pCA->szWonName,"系统");

	/*pCA->szItemName = sData.m_szName;
	pCA->szScript   = sData.m_szScript;
	pCA->szWonName  = "ϵͳ";*/

	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->LimitTime.bMin = 0;
	//::strcpy(pCA->ItmeInfo,sData.m_szIntro);
    /*char szIntro[516];
	ZeroMemory(szIntro,sizeof(szIntro));
	sprintf(szIntro,"%s", sData.m_szIntro);
	sprintf(pCA->ItmeInfo,szIntro);*/
	pCA->ItmeInfo = sData.m_szIntro;

	pCA->uPrice = 0;
    pCA->uLianjieFlg =0;
	pCA->ncSellModel =0;
//	::strcpy(pCA->szImageName, sData.m_szImageName);
	sprintf(pCA->szIntro, "%s", sData.m_szIntro);

	pCA->szImageName = sData.m_szImageName;
	//pCA->szIntro     = sData.m_szIntro;

	// ��ֵ: �������Բ���: ��
	// ��ֵ: ħ�����Բ���: ��
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));

	ZeroMemory(m_ronMagicAttrib, sizeof(m_ronMagicAttrib));
//	ZeroMemory(m_BaoShiMagicAttrib, sizeof(m_BaoShiMagicAttrib));
	ZeroMemory(m_yinMagicAttrib, sizeof(m_yinMagicAttrib));
	ZeroMemory(m_TempMagicAttrib, sizeof(m_TempMagicAttrib));
	ZeroMemory(m_TempRMagicAttrib, sizeof(m_TempRMagicAttrib));
//	ZeroMemory(m_TempBMagicAttrib, sizeof(m_TempBMagicAttrib));


	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
	/*KImageParam	Param;
	if (g_pRepresent)
	{
		if (!g_pRepresent->GetImageParam(m_Image.szImage, &Param, ISI_T_SPR))
		{//�������û�����SPR ������Ĭ�ϵ�SPR·��
			sprintf(m_Image.szImage,"\\spr\\others\\�ʺ�.spr");
			sprintf(pCA->szIntro,"��ʾ:�ͻ�����Դ����,�����Ϊ���¿ͻ���!");
		}
	}*/

}

//---------------------------------------------------�Ƹ�
void KItem::operator = (const KBASICPROP_FUSION& sData)
{
	// ��ֵ: ��ͬ���Բ���
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nPriceXu		 = sData.m_nPriceXu;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nUseMap         = 0;
	pCA->nUseKind	     = 0;
	pCA->nRes            = 0;
	pCA->nSixSkill		 = 0;
	pCA->nTenSkill		 = 0;
//-----------------------��¼�ſ������Ĳ�λ-
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 1;
	pCA->nBigSet		 = 0;
	pCA->nISMagic        = 0;
	pCA->nMagicID        = 0;
//-------------------------------------------
	pCA->nGoldId		 = 0;
	pCA->nIsPlatina	     = 0;	         //�׽�
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsWhere        = 0;
//	pCA->nYingNuma       = 0;                  //��������1
//    pCA->nYingNumb       = 0;                  //��������2
	pCA->nRongNum        = 0;                  //������������
    pCA->nWengangPin     = sData.m_inPin;      //�����Ƹ�Ʒ��
    pCA->nBinfujiazhi    = 0;     //������ֵ
	pCA->nIsSell		 = 1;     //�Ƿ�����
	pCA->nIsTrade		 = 1;     //�Ƿ���
	pCA->nIsDrop		 = 0;     //��������
	pCA->nRongpiont      = 0;
	pCA->nIsBang         = 0;
	pCA->nIsKuaiJie      = 0;
	pCA->nSkillType      = sData.m_MagIndex; //�Ƹֵ�ħ��ID�ڿ��Ƹ��б���к�
	pCA->nIsUse          = 0;
	pCA->nLianjieFlg     = 0;
	pCA->nChiBangRes     = 0;                //����������
	pCA->nSellModel      = 0;
	::strcpy(pCA->szItemName,sData.m_szName);
	//::strcpy(pCA->szScript,"");
	pCA->szScript = NULL;
	::strcpy(pCA->szWonName,"系统");
	/*pCA->szItemName = sData.m_szName;
	pCA->szScript   = NULL;
	pCA->szWonName  = "ϵͳ";*/


	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->LimitTime.bMin = 0;
	//::strcpy(pCA->ItmeInfo,sData.m_szIntro);
	/*char szIntro[516];
	ZeroMemory(szIntro,sizeof(szIntro));
	sprintf(szIntro,"%s", sData.m_szIntro);
	sprintf(pCA->ItmeInfo,szIntro);*/
	pCA->ItmeInfo = sData.m_szIntro;

	pCA->uPrice = 0;
	pCA->uLianjieFlg =0;
	pCA->ncSellModel =0;
	//::strcpy(pCA->szImageName,sData.m_szImageName);
	sprintf(pCA->szIntro,"%s", sData.m_szIntro);
	pCA->szImageName = sData.m_szImageName;
	//pCA->szIntro     = sData.m_szIntro;

	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	// ��ֵ: �������Բ���: ��
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	// ��ֵ: ħ�����Բ���: ��
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));
/*
	pCA->nSet			 = sData.m_Magic[0];
	pCA->nSetId			 = sData.m_Magic[1];
	pCA->nSetNum		 = sData.m_Magic[2];
	pCA->nBigSet		 = sData.m_Magic[3];
	pCA->nISMagic        = sData.m_Magic[4];
	pCA->nMagicID        = sData.m_Magic[5];
*/
    KItemNormalAttrib* pBA = m_aryMagicAttrib;
	pBA[0].nAttribType = 0;
	pBA[0].nValue[0]   = sData.m_Magic[0];
	pBA[1].nAttribType = 0;
	pBA[1].nValue[0]   = sData.m_Magic[1];
	pBA[2].nAttribType = 0;
	pBA[2].nValue[0]   = sData.m_Magic[2];
	pBA[3].nAttribType = 0;
	pBA[3].nValue[0]   = sData.m_Magic[3];
	pBA[4].nAttribType = 0;
	pBA[4].nValue[0]   = sData.m_Magic[4];
	pBA[5].nAttribType = 0;
	pBA[5].nValue[0]   = sData.m_Magic[5];

	ZeroMemory(m_ronMagicAttrib, sizeof(m_ronMagicAttrib));
//	ZeroMemory(m_BaoShiMagicAttrib, sizeof(m_BaoShiMagicAttrib));
	ZeroMemory(m_yinMagicAttrib, sizeof(m_yinMagicAttrib));
	ZeroMemory(m_TempMagicAttrib, sizeof(m_TempMagicAttrib));
	ZeroMemory(m_TempRMagicAttrib, sizeof(m_TempRMagicAttrib));
//	ZeroMemory(m_TempBMagicAttrib, sizeof(m_TempBMagicAttrib));

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	//::strcpy(m_Image.szImage, pCA->szImageName);
	sprintf(m_Image.szImage,"%s", pCA->szImageName);
	m_Image.uImage = 0;
	/*KImageParam	Param;
	if (g_pRepresent)
	{
		if (!g_pRepresent->GetImageParam(m_Image.szImage, &Param, ISI_T_SPR))
		{//�������û�����SPR ������Ĭ�ϵ�SPR·��
			sprintf(m_Image.szImage,"\\spr\\others\\�ʺ�.spr");
			sprintf(pCA->szIntro,"��ʾ:�ͻ�����Դ����,�����Ϊ���¿ͻ���!");
		}
	}*/

}

//----------------------------------------------------
void KItem::operator = (const KBASICPROP_QUEST& sData)
{
	// ��ֵ: ��ͬ���Բ���
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = 0;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nPriceXu		 = sData.m_nPriceXu;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSixSkill		 = 0;
	pCA->nTenSkill		 = 0;
	pCA->nSeries		 = -1;
	pCA->nUseMap         = 0;
	pCA->nUseKind	     = 0;
	pCA->nRes            = 0;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = sData.m_nDelet;
	pCA->nGoldId		 = 0;
	pCA->nIsPlatina	     = 0;	         //�׽�
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsWhere        = 0;
//	pCA->nYingNuma       = 0;                  //��������1
//    pCA->nYingNumb       = 0;                  //��������2
	pCA->nRongNum        = 0;                  //������������
    pCA->nWengangPin     = 0;                  //�����Ƹ�Ʒ��
    pCA->nBinfujiazhi    = 0;                  //������ֵ
	pCA->nIsSell		 = sData.m_nIsSell;    //�Ƿ�����
	pCA->nIsTrade		 = sData.m_nIsTrade;   //�Ƿ���
	pCA->nIsDrop		 = sData.m_nIsDrop;    //��������
	pCA->nRongpiont      =0;
	pCA->nIsBang         =0;
	pCA->nIsKuaiJie      =sData.m_IsKuaiJie;
	pCA->nSkillType      =sData.m_SkillType;   //�Ƹֵ�ħ��ID�ڿ��Ƹ��б���к�
	pCA->nISMagic        =sData.m_ISMagic;     //�Ƿ�������
	pCA->nMagicID        =sData.m_MagicID;     //����ID
	pCA->nIsUse          =sData.m_IsUse;
	pCA->nLianjieFlg     =0;
	pCA->nChiBangRes     =0;            //����������
	pCA->nSellModel      =0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//::strcpy(pCA->szScript,	   sData.m_szScript);
	pCA->szScript = sData.m_szScript;
	::strcpy(pCA->szWonName,"系统");

	/*pCA->szItemName=  sData.m_szName;
	pCA->szScript  = sData.m_szScript;
	pCA->szWonName = "ϵͳ";*/

	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->LimitTime.bMin = 0;
	//::strcpy(pCA->ItmeInfo,sData.m_szIntro);
	/*char szIntro[516];
	ZeroMemory(szIntro,sizeof(szIntro));
	sprintf(szIntro,"%s", sData.m_szIntro);
	sprintf(pCA->ItmeInfo,szIntro);*/
	pCA->ItmeInfo = sData.m_szIntro;

	pCA->uPrice = 0;
	pCA->uLianjieFlg =0;
	pCA->ncSellModel =0;
	//::strcpy(pCA->szImageName, sData.m_szImageName);
	sprintf(pCA->szIntro,"%s", sData.m_szIntro);

	pCA->szImageName = sData.m_szImageName;
    //pCA->szIntro     = sData.m_szIntro;
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	// ��ֵ: �������Բ���: ��
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	// ��ֵ: ħ�����Բ���: ��
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));

	ZeroMemory(m_ronMagicAttrib, sizeof(m_ronMagicAttrib));
//	ZeroMemory(m_BaoShiMagicAttrib, sizeof(m_BaoShiMagicAttrib));
	ZeroMemory(m_yinMagicAttrib, sizeof(m_yinMagicAttrib));
	ZeroMemory(m_TempMagicAttrib, sizeof(m_TempMagicAttrib));
	ZeroMemory(m_TempRMagicAttrib, sizeof(m_TempRMagicAttrib));
//	ZeroMemory(m_TempBMagicAttrib, sizeof(m_TempBMagicAttrib));

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	//::strcpy(m_Image.szImage, pCA->szImageName);
	sprintf(m_Image.szImage,"%s", pCA->szImageName);
	m_Image.uImage = 0;
	/*KImageParam	Param;
	if (g_pRepresent)
	{
		if (!g_pRepresent->GetImageParam(m_Image.szImage, &Param, ISI_T_SPR))
		{//�������û�����SPR ������Ĭ�ϵ�SPR·��
			sprintf(m_Image.szImage,"\\spr\\others\\�ʺ�.spr");
			sprintf(pCA->szIntro,"��ʾ:�ͻ�����Դ����,�����Ϊ���¿ͻ���!");
		}
	}*/

}
//���ط�
void KItem::operator = (const KBASICPROP_TOWNPORTAL& sData)
{
	// ��ֵ: ��ͬ���Բ���
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = 0;
	pCA->nParticularType = 0;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = 0;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nUseMap         = 0;
	pCA->nUseKind	     = 0;
	pCA->nRes            = 0;
	pCA->nPriceXu		 = 0;  //���
	pCA->nLevel			 = 1;
	pCA->nSeries		 = -1;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->nGoldId		 = 0;
	pCA->nIsPlatina	     = 0;	         //�׽�
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsWhere        = 0;
//	pCA->nYingNuma        = 0;               //��������1
//    pCA->nYingNumb        = 0;               //��������2
	pCA->nRongNum        = 0;               //������������
    pCA->nWengangPin     = 0;            //�����Ƹ�Ʒ��
    pCA->nBinfujiazhi    = 0;            //������ֵ
	pCA->nChiBangRes     = 0;            //����������
	pCA->nRongpiont      =0;
	pCA->nIsBang         =0;
	pCA->nIsKuaiJie      =1;
	pCA->nISMagic        =0;     //�Ƿ�������
	pCA->nSkillType      =0;   //����ID
	pCA->nMagicID        =0;     //����ID
	pCA->nIsUse          =0;
	pCA->nLianjieFlg     =0;
	pCA->nSellModel      =0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//::strcpy(pCA->szScript,	   "");
	pCA->szScript = NULL;

	::strcpy(pCA->szWonName,"系统");

	 /*pCA->szItemName = sData.m_szName;
	 pCA->szScript = NULL;
	 pCA->szWonName = "ϵͳ";*/

	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->LimitTime.bMin = 0;
	//::strcpy(pCA->ItmeInfo,sData.m_szIntro);
	/*char szIntro[516];
	ZeroMemory(szIntro,sizeof(szIntro));
	sprintf(szIntro,"%s", sData.m_szIntro);
	sprintf(pCA->ItmeInfo,szIntro);*/
	pCA->ItmeInfo = sData.m_szIntro;
	pCA->uPrice = 0;
    pCA->uLianjieFlg =0;
	pCA->ncSellModel =0;
	//::strcpy(pCA->szImageName, sData.m_szImageName);
	sprintf(pCA->szIntro, "%s", sData.m_szIntro);
	pCA->szImageName = sData.m_szImageName;
	//pCA->szIntro = sData.m_szIntro;
	// ��ֵ: �������Բ���: ��
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	// ��ֵ: �������Բ���: ��
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	// ��ֵ: ħ�����Բ���: ��
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));

	ZeroMemory(m_ronMagicAttrib, sizeof(m_ronMagicAttrib));
//	ZeroMemory(m_BaoShiMagicAttrib, sizeof(m_BaoShiMagicAttrib));
	ZeroMemory(m_yinMagicAttrib, sizeof(m_yinMagicAttrib));
	ZeroMemory(m_TempMagicAttrib, sizeof(m_TempMagicAttrib));
	ZeroMemory(m_TempRMagicAttrib, sizeof(m_TempRMagicAttrib));
//	ZeroMemory(m_TempBMagicAttrib, sizeof(m_TempBMagicAttrib));

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	//::strcpy(m_Image.szImage, pCA->szImageName);
	sprintf(m_Image.szImage, "%s", pCA->szImageName);
	m_Image.uImage = 0;
	/*KImageParam	Param;
	if (g_pRepresent)
	{
		if (!g_pRepresent->GetImageParam(m_Image.szImage, &Param, ISI_T_SPR))
		{//�������û�����SPR ������Ĭ�ϵ�SPR·��
			sprintf(m_Image.szImage,"\\spr\\others\\�ʺ�.spr");
			sprintf(pCA->szIntro,"��ʾ:�ͻ�����Դ����,�����Ϊ���¿ͻ���!");
		}
	}*/

}
//ҩƷ
void KItem::operator = (const KBASICPROP_MEDICINE& sData)
{
	// ��ֵ: ��ͬ���Բ���
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nPriceXu		 = sData.m_nPriceXu;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nUseMap         = sData.m_UseMap;
	pCA->nUseKind	     = 0;
	pCA->nRes            = 0;
	pCA->nSixSkill		 = 0;
	pCA->nTenSkill		 = 0;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->nGoldId		 = 0;
	pCA->nIsPlatina	     = 0;	         //�׽�
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsWhere        = 0;
//	pCA->nYingNuma        = 0;               //��������1
//    pCA->nYingNumb        = 0;               //��������2
	pCA->nRongNum        = 0;               //������������
    pCA->nWengangPin     = 0;            //�����Ƹ�Ʒ��
    pCA->nBinfujiazhi    = 0;            //������ֵ
	pCA->nChiBangRes     = 0;            //����������
	pCA->nRongpiont      =0;
	pCA->nIsBang         =0;
	pCA->nIsKuaiJie      =1;
	pCA->nISMagic        =0;     //�Ƿ�������
	pCA->nSkillType      =0;   //����ID
	pCA->nMagicID        =0;     //����ID
	pCA->nIsUse          =0;
	pCA->nLianjieFlg     =0;
	pCA->nSellModel      =0;
	::strcpy(pCA->szItemName,sData.m_szName);
	//::strcpy(pCA->szScript,"");
	pCA->szScript = NULL;
	::strcpy(pCA->szWonName,"系统");
	/*pCA->szItemName	=  sData.m_szName;
	pCA->szItemName	= NULL;
    pCA->szWonName  = "ϵͳ";
	*/
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->LimitTime.bMin = 0;
	//::strcpy(pCA->ItmeInfo,sData.m_szIntro);
	/*char szIntro[516];
	ZeroMemory(szIntro,sizeof(szIntro));
	sprintf(szIntro,"%s", sData.m_szIntro);
	sprintf(pCA->ItmeInfo,szIntro);*/
	pCA->ItmeInfo = sData.m_szIntro;

	pCA->uPrice = 0;
    pCA->uLianjieFlg =0;
	pCA->ncSellModel =0;
	//::strcpy(pCA->szImageName, sData.m_szImageName);
	sprintf(pCA->szIntro, "%s", sData.m_szIntro);
	pCA->szImageName = sData.m_szImageName;
	//pCA->szIntro = sData.m_szIntro;

	// ��ֵ: �������Բ���
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	KItemNormalAttrib* pBA = m_aryBaseAttrib;
	pBA[0].nAttribType = sData.m_aryAttrib[0].nAttrib;
	pBA[0].nValue[0]   = sData.m_aryAttrib[0].nValue;
	pBA[0].nValue[1]   = sData.m_aryAttrib[0].nTime;

	pBA[1].nAttribType = sData.m_aryAttrib[1].nAttrib;
	pBA[1].nValue[0]   = sData.m_aryAttrib[1].nValue;
	pBA[1].nValue[1]   = sData.m_aryAttrib[1].nTime;

	pBA[2].nAttribType = sData.m_aryAttrib[2].nAttrib;
	pBA[2].nValue[0]   = sData.m_aryAttrib[2].nValue;
	pBA[2].nValue[1]   = sData.m_aryAttrib[2].nTime;

	pBA[3].nAttribType = sData.m_aryAttrib[3].nAttrib;
	pBA[3].nValue[0]   = sData.m_aryAttrib[3].nValue;
	pBA[3].nValue[1]   = sData.m_aryAttrib[3].nTime;

	pBA[4].nAttribType = sData.m_aryAttrib[4].nAttrib;
	pBA[4].nValue[0]   = sData.m_aryAttrib[4].nValue;
	pBA[4].nValue[1]   = sData.m_aryAttrib[4].nTime;

	pBA[5].nAttribType = sData.m_aryAttrib[5].nAttrib;
	pBA[5].nValue[0]   = sData.m_aryAttrib[5].nValue;
	pBA[5].nValue[1]   = sData.m_aryAttrib[5].nTime;



	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));

	ZeroMemory(m_ronMagicAttrib, sizeof(m_ronMagicAttrib));
//	ZeroMemory(m_BaoShiMagicAttrib, sizeof(m_BaoShiMagicAttrib));
	ZeroMemory(m_yinMagicAttrib, sizeof(m_yinMagicAttrib));
	ZeroMemory(m_TempMagicAttrib, sizeof(m_TempMagicAttrib));
	ZeroMemory(m_TempRMagicAttrib, sizeof(m_TempRMagicAttrib));

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	//::strcpy(m_Image.szImage, pCA->szImageName);
	sprintf(m_Image.szImage,"%s", pCA->szImageName);
	m_Image.uImage = 0;
	/*KImageParam	Param;
	if (g_pRepresent)
	{
		if (!g_pRepresent->GetImageParam(m_Image.szImage, &Param, ISI_T_SPR))
		{//�������û�����SPR ������Ĭ�ϵ�SPR·��
			sprintf(m_Image.szImage,"\\spr\\others\\�ʺ�.spr");
			sprintf(pCA->szIntro,"��ʾ:�ͻ�����Դ����,�����Ϊ���¿ͻ���!");
		}
	}*/
}

void KItem::operator = (const KBASICPROP_EQUIPMENT_UNIQUE& sData)
{
	// ��ֵ: ��ͬ���Բ���
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nPriceXu		 = sData.m_nPriceXu;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nSixSkill		 = 0;
	pCA->nTenSkill		 = 0;
	pCA->nUseMap         = 0;
	pCA->nUseKind	     = 0;
	pCA->nRes            = 0;
	pCA->bStack			 = 0;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->nGoldId		 = 0;
	pCA->nIsPlatina	     = 0;	         //�׽�
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsWhere        = 0;
//	pCA->nYingNuma       = 0;               //��������1
//    pCA->nYingNumb       = 0;              //��������2
	pCA->nRongNum        = 0;               //������������
    pCA->nWengangPin     = 0;            //�����Ƹ�Ʒ��
    pCA->nBinfujiazhi    = 0;            //������ֵ
	pCA->nChiBangRes     = 0;            //����������
	pCA->nRongpiont      = 0;
	pCA->nIsBang         =0;
	pCA->nIsKuaiJie      =0;
	pCA->nISMagic        =0;     //�Ƿ�������
	pCA->nSkillType      =0;   //����ID
	pCA->nMagicID        =0;     //����ID
	pCA->nIsUse          =0;
	pCA->nLianjieFlg     =0;
	pCA->nCanUse         =1;     //�Ƿ����ʹ��
	pCA->nSellModel      =0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//::strcpy(pCA->szScript,"");
	pCA->szScript = NULL;
	::strcpy(pCA->szWonName,"系统");

	/*pCA->szItemName	=  sData.m_szName;
	pCA->szItemName	= NULL;
    pCA->szWonName  = "ϵͳ";*/

	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->LimitTime.bMin = 0;
	//::strcpy(pCA->ItmeInfo,sData.m_szIntro);
	/*char szIntro[516];
	ZeroMemory(szIntro,sizeof(szIntro));
	sprintf(szIntro,"%s", sData.m_szIntro);
	sprintf(pCA->ItmeInfo,szIntro);*/
	pCA->ItmeInfo = sData.m_szIntro;

	pCA->uPrice = 0;
	pCA->uLianjieFlg =0;
	pCA->ncSellModel =0;
	//::strcpy(pCA->szImageName, sData.m_szImageName);
	sprintf(pCA->szIntro, "%s", sData.m_szIntro);

	pCA->szImageName = sData.m_szImageName;
    //pCA->szIntro = sData.m_szIntro;

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	//::strcpy(m_Image.szImage, pCA->szImageName);
	sprintf(m_Image.szImage, "%s", pCA->szImageName);
	m_Image.uImage = 0;
	/*KImageParam	Param;
	if (g_pRepresent)
	{
		if (!g_pRepresent->GetImageParam(m_Image.szImage, &Param, ISI_T_SPR))
		{//�������û�����SPR ������Ĭ�ϵ�SPR·��
			sprintf(m_Image.szImage,"\\spr\\others\\�ʺ�.spr");
			sprintf(pCA->szIntro,"��ʾ:�ͻ�����Դ����,�����Ϊ���¿ͻ���!");
		}
	}*/
}
//�׽�
void KItem::operator = (const KBASICPROP_EQUIPMENT_PLATINA& sData) //const
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nPriceXu		 = sData.m_nPriceXu;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nSet			 = sData.m_nSet;
	pCA->nSetId			 = sData.m_nSetId;  //��װ��ID
	pCA->nSetNum		 = sData.m_nSetNum; //
	pCA->nBigSet		 = sData.m_nUpSet;
	pCA->nSixSkill		 = sData.m_nSixSkill;
	pCA->nTenSkill		 = sData.m_nTenSkill;
	pCA->nGoldId		 = 0;
	pCA->nIsPlatina	     = 1;	         //�׽�
	pCA->nUseMap         = 0;
	pCA->nUseKind	     = 0;
	pCA->nRes            = 0;
	pCA->bStack			 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsWhere        = 0;
//	pCA->nYingNuma       = sData.m_nYingNuma;            //��������1
//  pCA->nYingNumb       = sData.m_nYingNumb;            //��������2
	pCA->nRongNum        = sData.m_nRongNum;             //������������
    pCA->nWengangPin     = sData.m_nWengangPin;          //�����Ƹ�Ʒ��
    pCA->nBinfujiazhi    = sData.m_nBinfujiazhi;         //������ֵ
    pCA->nChiBangRes     = 0;           //����������
	pCA->nRongpiont      =0;
	pCA->nIsBang         =0;
	pCA->nIsKuaiJie      =0;
	pCA->nISMagic        =0;     //�Ƿ�������
	pCA->nSkillType      =0;     //����ID
	pCA->nMagicID        =0;     //����ID
	pCA->nIsUse          =0;
	pCA->nLianjieFlg     =0;
	pCA->nCanUse         =1;     //�Ƿ����ʹ��
	pCA->nSellModel      =0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//::strcpy(pCA->szScript,"");
	pCA->szScript = NULL;
	::strcpy(pCA->szWonName,"系统");
	/*pCA->szItemName	=  sData.m_szName;
	pCA->szItemName	= NULL;
    pCA->szWonName  = "ϵͳ";
	*/
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->LimitTime.bMin = 0;
	/*char szIntro[516];
	ZeroMemory(szIntro,sizeof(szIntro));
	sprintf(szIntro,"%s", sData.m_szIntro);
	sprintf(pCA->ItmeInfo,szIntro);*/
	pCA->ItmeInfo = sData.m_szIntro;

	pCA->uPrice = 0;
	pCA->uLianjieFlg =0;
	pCA->ncSellModel =0;
//	::strcpy(pCA->szImageName, sData.m_szImageName);
	sprintf(pCA->szIntro, "%s", sData.m_szIntro);
	pCA->szImageName =  sData.m_szImageName;
	//pCA->szIntro	 =  sData.m_szIntro;

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	//��Ϸ��Ʒͼ����ʾ
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
	/*KImageParam	Param;
	if (g_pRepresent)
	{
		if (!g_pRepresent->GetImageParam(m_Image.szImage, &Param, ISI_T_SPR))
		{//�������û�����SPR ������Ĭ�ϵ�SPR·��
			sprintf(m_Image.szImage,"\\spr\\others\\�ʺ�.spr");
			sprintf(pCA->szIntro,"��ʾ:�ͻ�����Դ����,�����Ϊ���¿ͻ���!");
		}
	}*/
}


//�ƽ��������
void KItem::operator = (const KBASICPROP_EQUIPMENT_GOLD& sData) //const
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nPriceXu		 = sData.m_nPriceXu;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nSet			 = sData.m_nSet;
	pCA->nSetId			 = sData.m_nSetId;  // ��װ��ID
	pCA->nSetNum		 = sData.m_nSetNum;
	pCA->nBigSet		 = sData.m_nUpSet;
	pCA->nGoldId		 = sData.m_nId;
	pCA->nIsPlatina	     = 0;	         //�׽�
	pCA->nSixSkill		 = 0;
	pCA->nTenSkill		 = 0;
	pCA->nUseMap         = 0;
	pCA->nUseKind	     = 0;
	pCA->nRes            = 0;	         //�ı�����
	pCA->bStack			 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsWhere        = 0;
//	pCA->nYingNuma       = sData.m_nYingNuma;              //��������1
//  pCA->nYingNumb       = sData.m_nYingNumb;              //��������2
	pCA->nRongNum        = sData.m_nRongNum;               //������������
    pCA->nWengangPin     = sData.m_nWengangPin;            //�����Ƹ�Ʒ��
    pCA->nBinfujiazhi    = sData.m_nBinfujiazhi;         //������ֵ
    pCA->nChiBangRes     = 0;              //����������
	pCA->nRongpiont      =0;
	pCA->nIsBang         =0;
	pCA->nIsKuaiJie      =0;
	pCA->nISMagic        =0;     //�Ƿ�������
	pCA->nSkillType      =0;   //����ID
	pCA->nMagicID        =0;     //����ID
	pCA->nIsUse          =0;
	pCA->nLianjieFlg     =0;
	pCA->nCanUse         =1;     //�Ƿ����ʹ��
	pCA->nSellModel      =0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	//::strcpy(pCA->szScript,"");
	pCA->szScript = NULL;
	::strcpy(pCA->szWonName,"系统");
	/*
	pCA->szItemName	=  sData.m_szName;
	pCA->szItemName	= NULL;
    pCA->szWonName  = "ϵͳ";
	*/
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->LimitTime.bMin = 0;
	//::strcpy(pCA->ItmeInfo,sData.m_szIntro);
	/*char szIntro[516];
	ZeroMemory(szIntro,sizeof(szIntro));
	sprintf(szIntro,"%s", sData.m_szIntro);
	sprintf(pCA->ItmeInfo,szIntro);*/
	pCA->ItmeInfo = sData.m_szIntro;

	pCA->uPrice = 0;
	pCA->uLianjieFlg =0;
	pCA->ncSellModel =0;
//	::strcpy(pCA->szImageName, sData.m_szImageName);
	sprintf(pCA->szIntro, "%s", sData.m_szIntro);

	pCA->szImageName	=  sData.m_szImageName;
	//pCA->szIntro	=  sData.m_szIntro;

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
	/*KImageParam	Param;
	if (g_pRepresent)
	{
		if (!g_pRepresent->GetImageParam(m_Image.szImage, &Param, ISI_T_SPR))
		{//�������û�����SPR ������Ĭ�ϵ�SPR·��
			sprintf(m_Image.szImage,"\\spr\\others\\�ʺ�.spr");
			sprintf(pCA->szIntro,"��ʾ:�ͻ�����Դ����,�����Ϊ���¿ͻ���!");
		}
	}*/
}
//��װ
int KItem::Gen_Equipment_Unique(const KBASICPROP_EQUIPMENT* pEqu,
								 const KBASICPROP_EQUIPMENT_UNIQUE* pUni)
{
	//_ASSERT(this != NULL);
	//_ASSERT(pEqu != NULL);
	//_ASSERT(pUni != NULL);
	if (NULL == pEqu || NULL == pUni)
		{ return FALSE;}

	// ��ֵ: ��ͬ���Բ���
	*this = *pUni;		// ���������
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->bStack  = pEqu->m_bStack;
	pCA->nWidth  = pEqu->m_nWidth;
	pCA->nHeight = pEqu->m_nHeight;

	SetAttrib_Base(pEqu->m_aryPropBasic);		// ��ֵ: �������Բ���
	SetAttrib_Req(pUni->m_aryPropReq);			// ��ֵ: �������Բ���
	SetAttrib_MAB(pUni->m_aryMagicAttribs);		// ��ֵ: ħ�����Բ���

	return TRUE;
}

void KItem::Remove()
{//��������Ϊ0
	m_nIndex = 0;
}

void KItem::SetIndex(int i)
{//��������Ϊ0
	m_nIndex = i;
}


int KItem::SetBaseAttrib(IN const KItemNormalAttrib* pAttrib)
{
	if (!pAttrib)
		return FALSE;

	for (int i = 0; i < sizeof(m_aryBaseAttrib) / sizeof(m_aryBaseAttrib[0]); ++i)
	{
		m_aryBaseAttrib[i] = pAttrib[i];
	}
	return TRUE;
}

int KItem::SetRequireAttrib(IN const KItemNormalAttrib* pAttrib)
{
	if (!pAttrib)
		return FALSE;

	for (int i = 0; i < sizeof(m_aryRequireAttrib) / sizeof(m_aryRequireAttrib[0]); ++i)
	{
		m_aryRequireAttrib[i] = pAttrib[i];
	}
	return TRUE;
}

int KItem::SetMagicAttrib(IN const KItemNormalAttrib* pAttrib)
{
	return SetAttrib_MA(pAttrib);
}

//------------------------------------------------------------------
//	ĥ�𣬷���ֵ��ʾʣ���;ö�
//------------------------------------------------------------------
int KItem::Abrade(IN const int nRandRange)
{
	if (m_nCurrentDur == -1 || nRandRange == 0)	// ����ĥ��
		return -1;

     if (nRandRange<0)
	 {//С��0�� �ض�ĥ��
	      m_nCurrentDur--;
	      if (m_nCurrentDur == 0)
		  {
		    return 0;
		  }
	 }
     else if (g_Random(nRandRange) == 0)	// nRandRange��֮һ�ĸ���
	 {
		m_nCurrentDur--;
		if (m_nCurrentDur == 0)
		{
			return 0;
		}
	 }
	return m_nCurrentDur;
}

void KItem::SetItemInfo(int nVale)
{
	char strVal[516]={0};
	sprintf(strVal, "%s", GetSItmeInfo());

 	if (strrchr(strVal,'.lua'))
	{
		KLuaScript * pScript = NULL;
		char szResult[256];
		ZeroMemory(szResult,sizeof(szResult));
		if (nVale<=0)
             nVale=GetBaseMagic();
		KLuaScript nClentLua;
        pScript = (KLuaScript*)(nClentLua.Load(strVal));

		if (pScript)
		{
			int nSafeIndex = 0;
			nSafeIndex=pScript->SafeCallBegin();
			//ִ������ű��ĺ���

			pScript->CallFunction("GetItemDescription", 1, "sds",GetName(),nVale,"");
			//	sprintf(Npc[Player[nIndex].m_nIndex].m_ItmeInfo,"����˵��C:%s,%s,%d,%s",JView->nCallFun,JView->nCanShuA,JView->nCanShuB,JView->nCanShuC);
			const char * szType = lua_typename(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			if (Lua_IsNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
			{//Lua_PushNumber
				int nResult = (int)Lua_ValueToNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
				sprintf(szResult, "%d", nResult);  // int ����
			}
			else if (Lua_IsString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)  // �ַ�������
			{
				sprintf(szResult, "%s", (char *)Lua_ValueToString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)));
				//szResult[strlen(szResult)]='\0';
				//sprintf(Npc[Player[nIndex].m_nIndex].m_ItmeInfo,szResult);
			}
			else
				sprintf(szResult,"<color=Red>数据错误,请联系GM处理!<color>"); //JView->nCallFun,JView->nCanShuA,JView->nCanShuB,JView->nCanShuC
			//	ParseString2MagicAttrib(nLevel, szSettingNameValue, szResult);  //�����ű�����
			nSafeIndex=pScript->SafeCallBegin();
			pScript->SafeCallEnd(nSafeIndex);
		}
		else
		{
				 pScript->Exit();
			     sprintf(szResult,"数据有误:%s,%s,%d,%s","GetItemDescription",GetName(),nVale,"暂无数据");
			//       printf("[说明]:函数:%s,参数1:%s,参数2:%d,参数3:%s,返回:%s\n",pView->nCallFun,pView->nCanShuA,pView->nCanShuB,pView->nCanShuC,Npc[Player[nIndex].m_nIndex].m_ItmeInfo);
		}
		SetSItmeInfo(szResult);
	}
}
////////�����ʾͼ���������///////////////////////////////////////////
void KItem::Paint(int nX, int nY,int bStack,int nzStackNum,int nKind)  //������Ʒ����ɫ
{
	m_Image.oPosition.nX = nX;
	m_Image.oPosition.nY = nY;

	if (nKind==CGOG_NPCSELLITEM_PAI)
	{//������
		if (m_CommonAttrib.nWidth==1 && m_CommonAttrib.nHeight>1)
		{//
			m_Image.oPosition.nY -= (m_CommonAttrib.nHeight>>1)*25;
		}
		if (m_CommonAttrib.nWidth>1 && m_CommonAttrib.nHeight==1)
		{//
			m_Image.oPosition.nX -= (m_CommonAttrib.nWidth>>1)*25;
		}
		if (m_CommonAttrib.nWidth>1 && m_CommonAttrib.nHeight>1)
		{//
			m_Image.oPosition.nX -= (m_CommonAttrib.nWidth>>1)*25;
			m_Image.oPosition.nY -= (m_CommonAttrib.nHeight>>1)*25;
		}
	}

	m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;//IMAGE_RENDER_STYLE_BORDER;
	//g_pRepresent->DrawPrimitives(1, &m_Image, RU_T_IMAGE, TRUE);  //RU_T_IMAGE4 ��������

	if (nKind!=CGOG_NPCSELLITEM_PAI && m_CommonAttrib.bStack && bStack)
	{//�Ƿ�滭��������
		int nNum = m_CommonAttrib.nStackNum+nzStackNum;

		if (nNum >= 1 && nNum < 5000)   //С��10000
		{
			int nFontSize = 14;
			char szNum[16];
			int nLen = sprintf(szNum, "%d", nNum);  //����
			    szNum[15] = 0;
			/*g_pRepresent->OutputText(nFontSize, szNum, KRF_ZERO_END,
				nX + (m_CommonAttrib.nWidth * 27) - nLen * (nFontSize ) / 2,
				nY + (m_CommonAttrib.nHeight) + nFontSize + 1, TGetColor("255,255,0")); //��ɫ
			*/
		}
	}
}

//char SubBuWei[15][24]={"����","����","�·�","����","����","ѥ��","����","ñ��","������","��������","��ƥ","���","����","ӡ��","��Ʒ"};
char SubBuWei[15][24]={"武器","暗器","衣服","戒子","项链","靴子","腰带","帽子","手镯护腕","玉佩香囊","马匹","面具","披风","印鉴","饰品"};

void KItem::GetDesc(char* pszMsg, bool bShowPrice, int nPriceScale, int nActiveAttrib,int nType) //�õ�װ��������
{
	char	szColor[item_number][32] =
	{
		"",
		"<color=White>",
		"",
		"",
		"<color=Yellow>",
	};

	if (m_CommonAttrib.nItemGenre == item_equip)
	{
		if (m_CommonAttrib.nGoldId)	// �ƽ�װ��
		{
			if (m_CommonAttrib.nIsPlatina==0)
			    strcpy(szColor[item_equip], "<color=Yellow>");	   //0xffdcdc00	 Yellow
			else
			    strcpy(szColor[item_equip], "<bclr=blue><color=white>");
		}

		else if (m_CommonAttrib.nPoint > 0 && m_CommonAttrib.nPoint<=6 ) //��ɫ
		{
			strcpy(szColor[item_equip], "<color=Purple>");
		}
		else if (m_aryMagicAttrib[0].nAttribType)	// ��ɫװ��
		{
			strcpy(szColor[item_equip], "<color=Blue>");
		}
		else
		{
			strcpy(szColor[item_equip], "<color=White>");
		}
	}
	else
	{
	   strcpy(szColor[m_CommonAttrib.nItemGenre], "<color=White>");
	}



	if (m_CommonAttrib.nEnChance)
	{
		if (m_CommonAttrib.nItemGenre == 0)
		{
			strcpy(pszMsg, szColor[m_CommonAttrib.nItemGenre]);

			char sItemName[64]={0};
		   if (nType==1)
		   {
			   if (m_CommonAttrib.nIsPlatina==0)
		          sprintf(sItemName,"%s(已装备)<bclr=White><color=blue>+%d<color><bclr>",m_CommonAttrib.szItemName,m_CommonAttrib.nEnChance);//强化
		   	   else
				  sprintf(sItemName,"%s(已装备)+%d",m_CommonAttrib.szItemName,m_CommonAttrib.nEnChance);//强化
		   }
		   else
		   {
			   if (m_CommonAttrib.nIsPlatina==0)
		          sprintf(sItemName,"%s<bclr=White><color=blue>+%d<color><bclr>",m_CommonAttrib.szItemName,m_CommonAttrib.nEnChance);//强化
		   		else
				  sprintf(sItemName,"%s+%d",m_CommonAttrib.szItemName,m_CommonAttrib.nEnChance);//ǿ��

		   }
		   char    TextLevel[128]={0};
			int     LevelItem = m_CommonAttrib.nLevel;

			if(LevelItem > 10)
			{
				if (LevelItem < 100)
				{
					if (LevelItem%10 == 0)
					{
						sprintf(TextLevel, "%s[等级%d]", sItemName, LevelItem/(LevelItem/10));
					}
					else
					{
						sprintf(TextLevel, "%s[等级%d]", sItemName, LevelItem%10);
					}
				}
				else if (LevelItem < 1000)
				{
					if (LevelItem%100 == 0)
					{
						sprintf(TextLevel, "%s[等级%d]", sItemName, LevelItem/(LevelItem/100));
					}
					else
					{
						sprintf(TextLevel, "%s[等级%d]", sItemName, LevelItem%100);
					}
				}
			}
			else
			{
				sprintf(TextLevel, "%s[等级%d]", sItemName, LevelItem);
			}
			strcat(pszMsg, TextLevel);

		    if (m_CommonAttrib.nIsPlatina==1)
				strcat(pszMsg, "<color><bclr>\n");
			else
			    strcat(pszMsg, "<color>\n");
		}
		else
		{
			strcpy(pszMsg, szColor[m_CommonAttrib.nItemGenre]);
			strcat(pszMsg, m_CommonAttrib.szItemName);
			if (m_CommonAttrib.nIsPlatina==1)
				strcat(pszMsg, "<color><bclr>\n");
			else
			    strcat(pszMsg, "<color>\n");
		}

	}
    else
	{
		if (m_CommonAttrib.nItemGenre == 0)
		{
		  strcpy(pszMsg, szColor[m_CommonAttrib.nItemGenre]);

		  char sItemName[64]={0};

		if (nType==1)
		   sprintf(sItemName,"%s(已装备)",m_CommonAttrib.szItemName);
		else
		   sprintf(sItemName,"%s",m_CommonAttrib.szItemName);


		char    TextLevel[128]={0};
			int        LevelItem = m_CommonAttrib.nLevel;

			if(LevelItem > 10)
			{
				if (LevelItem < 100)
				{
					if (LevelItem%10 == 0)
					{

						sprintf(TextLevel, "%s[等级%d]", sItemName, LevelItem/(LevelItem/10));
					}
					else
					{
						sprintf(TextLevel, "%s[等级%d]", sItemName, LevelItem%10);
					}
				}
				else if (LevelItem < 1000)
				{
					if (LevelItem%100 == 0)
					{
						sprintf(TextLevel, "%s[等级%d]", sItemName, LevelItem/(LevelItem/100));
					}
					else
					{
						sprintf(TextLevel, "%s[等级%d]", sItemName, LevelItem%100);
					}
				}
			}
			else
			{
				sprintf(TextLevel, "%s[等级%d]", sItemName, LevelItem);
			}
			strcat(pszMsg, TextLevel);
			if (m_CommonAttrib.nIsPlatina==1)
				strcat(pszMsg, "<color><bclr>\n");
			else
			    strcat(pszMsg, "<color>\n");
		}
		else
		{//��װ��
			strcpy(pszMsg, szColor[m_CommonAttrib.nItemGenre]);
			strcat(pszMsg, m_CommonAttrib.szItemName);  //��Ʒ����

			if (m_CommonAttrib.nLevel>10)
			{
				char nMsg[32]={0};
				sprintf(nMsg,"[%d]级",m_CommonAttrib.nLevel);
				strcat(pszMsg, nMsg);
			}

			if (m_CommonAttrib.nIsPlatina==1)
				strcat(pszMsg, "<color><bclr>\n");
			else
			    strcat(pszMsg, "<color>\n");
		}

	}

/*   if  (m_CommonAttrib.nPoint==7)
    strcat(pszMsg, "[<color=green>VIP<color><color=pink>������:");
   else
    strcat(pszMsg, "[<color=pink>������:");
    strcat(pszMsg,m_CommonAttrib.szWonName);  //������
    strcat(pszMsg, "<color>]\n");
*/
    strcat(pszMsg, "<color=green>");

	if (m_CommonAttrib.nIsBang<=0)
       strcat(pszMsg,"该物品还未绑定");
	else
       strcat(pszMsg,"该物品已永久绑定");

    strcat(pszMsg, "<color>\n");


	 if (m_CommonAttrib.nUseMap>0)
	 {
		 char nMapKey[32]={0},nMapName[64]={0};
		 ZeroStruct(nMapName);
		 sprintf(nMapKey,"%d_name",m_CommonAttrib.nUseMap);

		 g_NpcMapDropRate.GetString("List",nMapKey,"暂无数据",nMapName,sizeof(nMapName));

		 if (m_CommonAttrib.nItemGenre == item_equip)
		 {//锁魂
			 strcat(pszMsg, "<color=yellow>该装备已被锁魂：");
		 }
		 else
			 strcat(pszMsg, "<color=yellow>该物品已被锁魂：");

		  if (m_CommonAttrib.nUseKind==0)  //锁魂地图
			     strcat(pszMsg, nMapName);
		  else
		  {
			  sprintf(nMapName,"等级<%d级",m_CommonAttrib.nUseMap);
			  strcat(pszMsg, nMapName);
		  }
		  strcat(pszMsg, "<color>\n");
	 }

	 if (m_CommonAttrib.nIsSell == 1 && m_CommonAttrib.nItemGenre != item_equip) //可交易
	 {
		strcat(pszMsg, "<color=Green>该物品可以交易<color>\n");
	 }
	 else if (m_CommonAttrib.nItemGenre != item_equip)
	 {
		strcat(pszMsg, "<color=Red>该物品不可以交易<color>\n");
	 }

	 //if (m_CommonAttrib.nUseMap >0 && m_CommonAttrib.nItemGenre == item_medicine) //ҩƷ
	 //{
	 //	 strcat(pszMsg, "<color=Red>��ҩƷ���Ƶ�ͼʹ��<color>\n");
	 //}

	int qianghua=m_CommonAttrib.nEnChance;

    if (qianghua>0)
	{
		if (qianghua==1)
			strcat(pszMsg,"强化星级：<bclr=blue><color=White>★<color><bclr>\n");
		else if (qianghua==2)
            strcat(pszMsg,"强化星级：<bclr=blue><color=White>★★<color><bclr>\n");
		else if (qianghua==3)
            strcat(pszMsg,"强化星级：<bclr=blue><color=White>★★★<color><bclr>\n");
		else if (qianghua==4)
            strcat(pszMsg,"强化星级：<bclr=blue><color=White>★★★★<color><bclr>\n");
		else if (qianghua==5)
            strcat(pszMsg,"强化星级：<bclr=blue><color=White>★★★★★<color><bclr>\n");
		else if (qianghua==6)
            strcat(pszMsg,"强化星级：<bclr=blue><color=White>★★★★★★<color><bclr>\n");
		else if (qianghua==7)
            strcat(pszMsg,"强化星级：<bclr=blue><color=White>★★★★★★★<color><bclr>\n");
		else if (qianghua==8)
            strcat(pszMsg,"强化星级：<bclr=blue><color=White>★★★★★★★★<color><bclr>\n");
		else if (qianghua==9)
            strcat(pszMsg,"强化星级：<bclr=blue><color=White>★★★★★★★★★<color><bclr>\n");
		else if (qianghua==10)
            strcat(pszMsg,"强化星级：<bclr=blue><color=White>★★★★★★★★★★<color><bclr>\n");
		else
		{
			char nInfo[64]={0};
            sprintf(nInfo,"强化星级：<bclr=blue><color=White>%d ★<color><bclr>\n",qianghua);
		    strcat(pszMsg,nInfo);
		}

	}

	if (bShowPrice && nPriceScale > 0)
	{
		/*if (m_CommonAttrib.nPriceXu>0)//(m_CommonAttrib.nItemGenre == item_task || m_CommonAttrib.nItemGenre == item_mine)
		{//���
			char szPrice[32];
			sprintf(szPrice, "价值：%d金币", m_CommonAttrib.nPriceXu);
			strcat(pszMsg, szPrice);
			strcat(pszMsg, "\n");
		}
		else*/
		{
			//	else if (m_PriceInfo.nPriceType==moneyunit_repute
			char szPrice[32]={0},nInfo[4]={0};
			//if (TakeTrader(m_CommonAttrib.nPrice,nPriceScale) < 10000)
			if (m_CommonAttrib.ncSellModel==moneyunit_money || m_CommonAttrib.ncSellModel==moneyunit_moneya)
				sprintf(nInfo,"两");
			else if (m_CommonAttrib.ncSellModel==moneyunit_xu)
				sprintf(nInfo,"金");
			else
				sprintf(nInfo,"点");

			if (m_CommonAttrib.nPrice/nPriceScale < 10000)
			{//万
			        //sprintf(szPrice, "价值：%d两",TakeTrader(m_CommonAttrib.nPrice,nPriceScale));
					sprintf(szPrice, "价值：%d%s",m_CommonAttrib.nPrice/nPriceScale,nInfo);
			}
			else
			{
					int vPrice = (m_CommonAttrib.nPrice/nPriceScale)/10000;//TakeTrader(TakeTrader(m_CommonAttrib.nPrice,nPriceScale), 10000);
					int lPrice = (m_CommonAttrib.nPrice/nPriceScale)%10000;//TakeRemainder(TakeTrader(m_CommonAttrib.nPrice, nPriceScale),10000);

					if (lPrice == 0)
					{
						sprintf(szPrice, "价值：%d万%s", vPrice,nInfo);
					}
					else
					{
						sprintf(szPrice, "价值：%d万%d%s", vPrice,lPrice,nInfo);
					}
			}
		strcat(pszMsg, szPrice);
		strcat(pszMsg, "\n");
		}

	}


/*     char  nMagid[6];
        sprintf(nMagid,"<color=yellow>%d<color>",m_CommonAttrib.nIsUse);
	    strcat(pszMsg,nMagid); //�Ƿ�����ʹ��
        strcat(pszMsg, "\n");
*/

//---------------------------------TASK
	if (m_CommonAttrib.nItemGenre == item_task)
	{
          char nSkillType[4];
		//	strcat(pszMsg, _itoa(m_CommonAttrib.nSkillType,nSkillType,10));
		  if (m_CommonAttrib.nDetailType == 371)  //������ʯ
		  {
			strcat(pszMsg, "\n");
			char szLevel[32];
			int  mLeve;
			if (m_CommonAttrib.nLevel>0 && m_CommonAttrib.nLevel<=6)
				mLeve=m_CommonAttrib.nLevel;
			else if (m_CommonAttrib.nLevel>6)
			    mLeve= 6;
			sprintf(szLevel, "<color=Blue>(可熔炼:%d个属性)<color>", mLeve);
		    strcat(pszMsg, szLevel);
		  }
		  else if 	(m_CommonAttrib.nDetailType == 374 || m_CommonAttrib.nDetailType == 375)  //������ʯ
		  {
			  strcat(pszMsg, "\n");
			  char szLevel[32];
			  sprintf(szLevel, "<color=Blue>(品质等级:%d品)<color>", m_CommonAttrib.nLevel);
		      strcat(pszMsg, szLevel);
		  }

			strcat(pszMsg, "\n");
//	}

        if (strstr(m_CommonAttrib.szItemName,"玄火炭"))
		{
			/*if (m_CommonAttrib.nLevel==1)
			{
				strcat(pszMsg, "<color=Yellow>说明:<color><color=Blue>可以提炼第一属性纹钢<color>\n");
				//strcat(pszMsg, "\n");
			}
			else if (m_CommonAttrib.nLevel==2)
			{
				strcat(pszMsg, "<color=Yellow>说明:<color><color=Blue>可以提炼第二属性纹钢<color>\n");
				//strcat(pszMsg, "\n");
			}
			else if (m_CommonAttrib.nLevel==3)
			{
				strcat(pszMsg, "<color=Yellow>说明:<color><color=Blue>可以提炼第三属性纹钢<color>\n");
				//strcat(pszMsg, "\n");
			}
			else if (m_CommonAttrib.nLevel==4)
			{
				strcat(pszMsg, "<color=Yellow>说明:<color><color=Blue>可以提炼第四属性纹钢<color>\n");
				//strcat(pszMsg, "\n");
			}
			else if (m_CommonAttrib.nLevel==5)
			{
				strcat(pszMsg, "<color=Yellow>说明:<color><color=Blue>可以提炼第五属性纹钢<color>\n");
				//strcat(pszMsg, "\n");
			}
			else if (m_CommonAttrib.nLevel>5)
			{
				strcat(pszMsg, "<color=Yellow>说明:<color><color=Blue>可以提炼第六属性纹钢<color>\n");
				//strcat(pszMsg, "\n");
			} */
			strcat(pszMsg, "<color=Blue>可以提炼所有纹钢<color>\n");
		}

		if (m_CommonAttrib.nSkillType>0)
		{//��ʾ�Ƿ�����õ��Ƹ�
			char szIntor[64];
			int szRows,nMin=0;
			KTabFile MagicTab,MagicTabA;
		    MagicTab.Load(TABFILE_MAGICALEVEL_PATH);
            MagicTab.GetString(m_CommonAttrib.nSkillType+1,"DESC","",szIntor,sizeof(szIntor));
			MagicTab.GetInteger(m_CommonAttrib.nSkillType+1,"MAGATTRLVL_START",1,&szRows);
            MagicTabA.Load(TABFILE_MAGICATTRIB_PATH);
            MagicTabA.GetInteger(szRows+m_CommonAttrib.nLevel,6,1,&nMin);
         /* if (strstr(m_CommonAttrib.szItemName,"�Ƹ�"))
		  {
            strcat(pszMsg, "<color=Yellow>�Ƹ�����:<color><color=Cyan>");
			strcat(pszMsg, szIntor);
			strcat(pszMsg, "<color>\n");
			MagicTab.GetString(m_CommonAttrib.nSkillType+1,"FIT_EQUIP","",szIntor,sizeof(szIntor));
			strcat(pszMsg, "<color=Yellow>�Ƹ�λ��:<color><color=Cyan>");
			strcat(pszMsg, szIntor);
		    strcat(pszMsg, "<color>\n");
			char szLevel[32];
			sprintf(szLevel, "<color=Blue>Ʒ�ʵȼ�:%dƷ(����:%d��)<color>", m_CommonAttrib.nLevel,nMin);
			strcat(pszMsg, szLevel);
		  }
		  else*/
		 if (strstr(m_CommonAttrib.szItemName,"宝石"))
		  {
			  strcat(pszMsg, "<color=Yellow>灵合属性:<color><color=Cyan>");
			  strcat(pszMsg, szIntor);
			  strcat(pszMsg, "<color>\n");
			  MagicTab.GetString(m_CommonAttrib.nSkillType+1,"FIT_EQUIP","",szIntor,sizeof(szIntor));
			  strcat(pszMsg, "<color=Yellow>灵合位置:<color><color=Cyan>");
			  strcat(pszMsg, szIntor);
			  strcat(pszMsg, "<color>\n");
			  char szLevel[64]={0};
			  sprintf(szLevel, "<color=Blue>品质等级:%d品(增加:%d点)<color>", m_CommonAttrib.nLevel,nMin);
			  strcat(pszMsg, szLevel);
		  }

			MagicTabA.Clear();
			MagicTab.Clear();
		    strcat(pszMsg, "\n");
        //  strcat(pszMsg,)
		}

	}

//---------------------------------װ�� �� MINE
	if (m_CommonAttrib.nItemGenre == item_equip ||
		(m_CommonAttrib.nItemGenre == item_mine &&
		(m_CommonAttrib.nParticularType == 150 ||
		m_CommonAttrib.nParticularType == 152 ||
		m_CommonAttrib.nParticularType == 154 ||
		(m_CommonAttrib.nParticularType >= 200 &&
		m_CommonAttrib.nParticularType <= 205)) //װ���� ��ȸԭʯ����ȸʯ ��ܽ��ʯ
		))
	{
		switch(m_CommonAttrib.nSeries)
		{
		case series_metal:
			strcat(pszMsg, "<color=White>五行属性：<color><color=Metal>金<color>");
			break;
		case series_wood:
			strcat(pszMsg, "<color=White>五行属性：<color><color=Wood>木<color>");
			break;
		case series_water:
			strcat(pszMsg, "<color=White>五行属性：<color><color=Water>水<color>");
			break;
		case series_fire:
			strcat(pszMsg, "<color=White>五行属性：<color><color=Fire>火<color>");
			break;
		case series_earth:
			strcat(pszMsg, "<color=White>五行属性：<color><color=Earth>土<color>");
			break;
		}
			strcat(pszMsg, "\n");
	}
//----------------------------------����----
	/*if (strstr(m_CommonAttrib.szItemName,"�������"))
	{
		strcat(pszMsg, "<color=Orange>����������<color>\n");
	} */
	strcat(pszMsg, m_CommonAttrib.szIntro);  //˵������  ��Ʒ��˵������
	strcat(pszMsg, "\n"); //<color>

	/*if (strstr(m_CommonAttrib.szItemName,"�������"))
	{
		strcat(pszMsg, "<color=yellow>��ʾ���Ҽ�ʹ�ÿɽ�����<color>\n");
	} */
//---------------------------------------------�Ƹ�
    if (m_CommonAttrib.nItemGenre == item_fusion) //������Ƹ�
	{
		    char szIntor[64];
		    int szRows,nMin=0;
		    KTabFile MagicTab;

		    MagicTab.Load(TABFILE_MAGICAGOLD_PATH);
            MagicTab.GetString(m_CommonAttrib.nSkillType,1,"暂无数据",szIntor,sizeof(szIntor));
            MagicTab.GetInteger(m_CommonAttrib.nSkillType,6,1,&nMin);   //魔法的索引

            strcat(pszMsg, "<color=Yellow>纹钢属性：<color><color=Cyan>");
			strcat(pszMsg, szIntor);
			strcat(pszMsg, "\n");

			if (strstr(m_CommonAttrib.szItemName,"专属"))
			    sprintf(szIntor, "<color=Blue>品质等级：%d品(增加:%d%s)<color>\n", m_CommonAttrib.nWengangPin,nMin,"%%");
			else
		        sprintf(szIntor, "<color=Blue>品质等级：%d品(增加:%d点)<color>\n", m_CommonAttrib.nWengangPin,nMin);

		    strcat(pszMsg, szIntor);

            MagicTab.Clear();

	  for (int i = 0; i < 6; ++i)  //��װ����װ����װ
	  {
		if (m_aryMagicAttrib[i].nValue[0]>-1) //�������λ�õ�������û�����ݵ� // nAttribType�������ͱ��루int���֣�
		{
             sprintf(szIntor, "<bclr=blue><color=White>可纹纲的部位：%s<color><bclr>",SubBuWei[m_aryMagicAttrib[i].nValue[0]]);
		     strcat(pszMsg, szIntor);
			 strcat(pszMsg, "\n");
		}
	  }
	}
//----------------------------------------------------MINE
	if (m_CommonAttrib.nItemGenre == item_mine &&
		m_CommonAttrib.nParticularType >= 200 &&
		m_CommonAttrib.nParticularType <= 205)  //������----����ʯ
	{
		char szIntor[64];

		KTabFile MagicTab;

		MagicTab.Load(TABFILE_MAGICALEVEL_PATH);

		char szTmp[8];
		int nskillidx,nRows;
		//sprintf(szTmp,"%d",m_aryBaseAttrib[0].nValue[0]); //ֻ���ǵ�һ���� ���� Ϊ ���������ֵ
		nRows=MagicTab.GetHeight();
        nskillidx=m_aryBaseAttrib[0].nValue[0];
      if (nskillidx>0)
        for (int k=0;k<nRows;++k)
		{
			int nDestidx,nMagcType;
           MagicTab.GetInteger(k+1,"MAGIC_ID",0,&nDestidx);
		   MagicTab.GetInteger(k+1,"MAGIC_TTPE",0,&nMagcType);
		   if (nDestidx==nskillidx && nMagcType==0)
		   {
			   MagicTab.GetString(k+1,"DESC","",szIntor,64);
			   strcat(pszMsg, "<color=Yellow>镶嵌属性: <color><color=Cyan>");
			   strcat(pszMsg, szIntor);
			   strcat(pszMsg, "<color>\n");
			   MagicTab.GetString(k+1,"FIT_EQUIP","",szIntor,64);
			   strcat(pszMsg, "<color=Yellow>镶嵌位置: <color><color=Cyan>");
			   strcat(pszMsg, szIntor);
		       strcat(pszMsg, "<color>\n");
			   break;
		   }

		}

		MagicTab.Clear();
		/*
		MagicTab.GetString(szTmp,"DESC","",szIntor,64);
		strcat(pszMsg, "<color=Yellow>��Ƕ����˵��: <color><color=Cyan>");
		strcat(pszMsg, szIntor);
		strcat(pszMsg, "<color>\n");
		MagicTab.GetString(szTmp,"FIT_EQUIP","",szIntor,64);
		strcat(pszMsg, "<color=Yellow>������Ƕ�Ĳ���λ��: <color><color=Cyan>");
		strcat(pszMsg, szIntor);
		strcat(pszMsg, "<color>\n");*/
	//	strcat(pszMsg, "\n");
	}

	if (m_CommonAttrib.nItemGenre == item_mine && (m_CommonAttrib.nParticularType == 147 ||
		(m_CommonAttrib.nParticularType >= 200 && m_CommonAttrib.nParticularType <= 205) ||
		(m_CommonAttrib.nParticularType >= 149 && m_CommonAttrib.nParticularType <= 154)
		))
	{
		char szLevel[32];
		sprintf(szLevel, "<color=Blue>品质等级: %d<color>", m_CommonAttrib.nLevel);
		strcat(pszMsg, szLevel);
		strcat(pszMsg, "\n");
	}

	int i;
	for ( i = 0; i < 7; i++)  //��������
	{
		if (!m_aryBaseAttrib[i].nAttribType)
		{
			continue;
		}

		if (m_aryBaseAttrib[i].nAttribType == magic_durability_v)
		{
			char	szDurInfo[64]={0};
			if (m_nCurrentDur == -1)
			{
				   sprintf(szDurInfo, "<color=Yellow>永不磨损<color>\n");
			}
			else
			{
				if  (GetDurability()<=3 && GetDurability()>0)
				{
					if (GetGenre()==item_equip && GetDetailType()==equip_mask)
						sprintf(szDurInfo, "<color=blue>剩余使用次数：%d/%d<color>\n", GetDurability(), GetMaxDurability());
					else
                        sprintf(szDurInfo, "<color=red>耐久度(需要修理)：%d/%d<color>\n", GetDurability(), GetMaxDurability());
				}
				else
				{
					if (GetGenre()==item_equip && GetDetailType()==equip_mask)
						sprintf(szDurInfo, "<color=blue>剩余使用次数：%d/%d<color>\n", GetDurability(), GetMaxDurability());
					else
				        sprintf(szDurInfo, "<color=White>耐久度：%d/%d<color>\n", GetDurability(), GetMaxDurability());
				}
			}
			strcat(pszMsg, szDurInfo);
		}
		else
		{//��������

			char* pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryBaseAttrib[i]);
			if (!pszInfo || !pszInfo[0])
				continue;
            strcat(pszMsg, "<color=White>");
			strcat(pszMsg, pszInfo);
			strcat(pszMsg, "<color>\n");
		}
	}
	////��װ��������nAttribType
	for (i = 0; i < 6; ++i)
	{
		if (!m_aryRequireAttrib[i].nAttribType)
		{
			continue;
		}
		char* pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryRequireAttrib[i]);
		if (!pszInfo || !pszInfo[0])
			continue;
		if (Player[CLIENT_PLAYER_INDEX].m_ItemList.EnoughAttrib(&m_aryRequireAttrib[i]))
		{
			strcat(pszMsg, "<color=White>");
			strcat(pszMsg, pszInfo);
			strcat(pszMsg, "<color>\n");
		}
		else
		{
			strcat(pszMsg, "<color=Red>");
			strcat(pszMsg, pszInfo);
			strcat(pszMsg, "<color>\n");
		}
	}

	for (i = 0; i < 6; ++i)  //��װ����װ����װ
	{
		if (m_CommonAttrib.nItemGenre != item_equip)
			break;

		if (!m_aryMagicAttrib[i].nAttribType) //�������λ�õ�������û�����ݵ� // nAttribType�������ͱ��루int���֣�
		{
			if (IsPurple() && i< m_CommonAttrib.nPoint)  //打孔了
			{//紫装
				strcat(pszMsg, "<color=GYellow>未镶嵌<color=white>\n");
			}
		//	else if (IsBlue())
		//	{
		//		  strcat(pszMsg, "<color=Yellow>�ɱ�ʶ����:<color>");
		//		  strcat(pszMsg,"<color=DYellow>δ��ʶ<color=white>");
		//		  strcat(pszMsg, "\n");
		//	}
			continue;
		}
		char* pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryMagicAttrib[i]);
		if (!pszInfo||!pszInfo[0])
			continue;
		//char pPin[16];
		//int smLevel=CheckBluePin(m_aryMagicAttrib[i].nAttribType,m_aryMagicAttrib[i].nValue[0]); //��ȡ��װƷ��
		if ((i & 1) == 0) //i=1 Ĭ���Ǽ��� �����ж�
		{//1 3 5 ����
			if (m_CommonAttrib.nGoldId > 0)  //�ƽ�װ
			{
				strcat(pszMsg, "<color=HYellow>");
			    strcat(pszMsg, pszInfo);
                strcat(pszMsg, "<color>");
			}
			else if (IsPurple())   //��װ
			{
				strcat(pszMsg, "<color=Purple>");
			    strcat(pszMsg, pszInfo);
                strcat(pszMsg, "<color>");
			}
			else
			{//��װ
				  strcat(pszMsg, "<color=HBlue>");  //��װ
			      strcat(pszMsg, pszInfo);
                  strcat(pszMsg, "<color>");
			}
		}
		else
		{//2 4 6 ż��
			if ((i>>1) < nActiveAttrib) // ����Ǳ������
			{
				if (m_CommonAttrib.nGoldId > 0)
				{
					strcat(pszMsg, "<color=HYellow>");
				    strcat(pszMsg, pszInfo);
                    strcat(pszMsg, "<color>");
				}
				else if (IsPurple())
				{
					strcat(pszMsg, "<color=Purple>");
				    strcat(pszMsg, pszInfo);
                    strcat(pszMsg, "<color>");
				}
				else
				{
					strcat(pszMsg, "<color=HBlue>");
				    strcat(pszMsg, pszInfo);
                    strcat(pszMsg, "<color>");
				}
			}
			else
			{//û�б�����
				if (m_CommonAttrib.nGoldId > 0)
				{
					strcat(pszMsg, "<color=DYellow>"); //  û�б������
				    strcat(pszMsg, pszInfo);
                    strcat(pszMsg, "<color>");
				}
				else
				{
					strcat(pszMsg, "<color=DBlue>");
				    strcat(pszMsg, pszInfo);
                    strcat(pszMsg, "<color>");
				}
			}
		}
    //�� �� ��ɫ ����ħ��������ʾ
	    strcat(pszMsg,"\n");
	} //end for

	//strcat(pszMsg,"\n");
//-----------------�������ص�����---------------------------
	for (i = 0; i < 2; ++i)  //��װ����װ����װ
	{
		if (m_CommonAttrib.nItemGenre != item_equip)
			break;

		if (!m_yinMagicAttrib[i].nAttribType)
           continue;

		char* pszInfo = (char *)g_MagicDesc.GetDesc(&m_yinMagicAttrib[i]);
		if (!pszInfo||!pszInfo[0])
		   continue;

		if (m_CommonAttrib.nGoldId > 0)
		{
		   if (m_CommonAttrib.nLevel>= 5 && m_CommonAttrib.nLevel<10)
		   {//��һ����
			 if(i==0)
			 {
		       strcat(pszMsg, "<color=HYellow>"); //  �����
		       strcat(pszMsg, pszInfo);
		       strcat(pszMsg, "<color>");
		       strcat(pszMsg,"\n");
			 }
			 if(i==1)
			 {
				 strcat(pszMsg, "<color=DYellow>");
				 strcat(pszMsg, pszInfo);
				 strcat(pszMsg, "<color>");
				 strcat(pszMsg,"\n");
			 }
		   }
		   else if (m_CommonAttrib.nLevel>=10)
		   {//������
			   strcat(pszMsg, "<color=HYellow>"); //  �����
			   strcat(pszMsg, pszInfo);
			   strcat(pszMsg, "<color>");
			   strcat(pszMsg,"\n");
		   }
		   else
		   {
			   strcat(pszMsg, "<color=DYellow>");
			   strcat(pszMsg, pszInfo);
			   strcat(pszMsg, "<color>");
			   strcat(pszMsg,"\n");
		   }
		}
	}

//==================��ʾ�ƽ���������=========================
	int RongID;
	char* pszInfor;
          RongID=m_CommonAttrib.nRongNum;

 if (RongID>0 && m_CommonAttrib.nRongpiont>0 && (m_CommonAttrib.nGoldId>0 || IsPurple()))
 {
   for (i = 0; i < m_CommonAttrib.nRongpiont; ++i)
   {
	 if (!m_ronMagicAttrib[i].nAttribType) //这个属性位置的数组中没有数据的 // nAttribType属性类型编码（int数字）
	 {
		 strcat(pszMsg,"<bclr=Blue><color=White>[未纹钢]<color><bclr>\n");
	     continue;
	 }
	 pszInfor = (char *)g_MagicDesc.GetDesc(&m_ronMagicAttrib[i]);
	 if (!pszInfor || !pszInfor[0])
		continue;
	//char pPin[32];
	//sprintf(pPin,"<color=pink>%d<color>",m_CommonAttrib.nRongpiont);
	//strcat(pszMsg,pPin);
	///int smLevel=CheckWenPin(m_ronMagicAttrib[i].nAttribType,m_ronMagicAttrib[i].nValue[0]); //��ȡƷ��
	//char pPin[32];
	// sprintf(pPin,"<color=pink>%dƷ<color>",smLevel);
    //strcat(pszMsg,pPin);
   	strcat(pszMsg, "<bclr=Blue><color=White>");
	strcat(pszMsg, pszInfor); //�������� ��ʾ
	strcat(pszMsg, "<color><bclr>\n");
   }
 }
//=================��ʾ�������Խ���======================
//==================��ʾ��װ����=========================
char* szBlueInfo=NULL;

 if (RongID>0 && m_CommonAttrib.nRongpiont>0 && IsBlue())
 {
	 char pPin[32]={0};

	 if (m_CommonAttrib.nIsWhere==-1)
	 {
		 sprintf(pPin, "%s","<color=yellow>该装备可以洗练出随机特殊属性");
		 strcat(pszMsg,pPin);
		 strcat(pszMsg, "<color>\n");
	 }

	 if (m_CommonAttrib.nIsWhere==-2)
	 {
		 sprintf(pPin, "%s","<color=yellow>该装备洗练属性不可见");
		 strcat(pszMsg,pPin);
		 strcat(pszMsg, "<color>\n");
	 }

    if (m_CommonAttrib.nIsWhere==0)
	{ //����

	 for (i = 0; i < m_CommonAttrib.nRongpiont; ++i)
	 {
	   if (!m_ronMagicAttrib[i].nAttribType) //这个属性位置的数组中没有数据的 // nAttribType属性类型编码（int数字）
	   {
		   strcat(pszMsg,"<bclr=Blue><color=White>[未纹钢]<color><bclr>\n");
		   continue;
	   }
	   szBlueInfo = (char *)g_MagicDesc.GetDesc(&m_ronMagicAttrib[i]);
	   if (!szBlueInfo || !szBlueInfo[0])
		   continue;
	   //int smLevel=CheckWenPin(m_ronMagicAttrib[i].nAttribType,m_ronMagicAttrib[i].nValue[0]); //��ȡƷ��
	   //char pPin[32];
	   //sprintf(pPin,"<color=Pink>%dƷ<color>",smLevel);
	   //strcat(pszMsg,pPin);
	   strcat(pszMsg, "<bclr=Blue><color=White>");
	   strcat(pszMsg, szBlueInfo); //�������� ��ʾ
	   strcat(pszMsg, "<color><bclr>\n");
	 }
   }
   else if (m_CommonAttrib.nIsWhere>0)
   {//��ʶ����
	 for (i = 0; i < m_CommonAttrib.nRongpiont; ++i)
	 {
		if (!m_ronMagicAttrib[i].nAttribType) //这个属性位置的数组中没有数据的 // nAttribType属性类型编码（int数字）
		{
			strcat(pszMsg,"<bclr=Blue><color=White>[未洗练]<color><bclr>\n");
			continue;
		}
		szBlueInfo = (char *)g_MagicDesc.GetDesc(&m_ronMagicAttrib[i]); //BaoShiGetDesc
		if (!szBlueInfo || !szBlueInfo[0])
			continue;
		//int smLevel=CheckWenPin(m_ronMagicAttrib[i].nAttribType,m_ronMagicAttrib[i].nValue[0]); //��ȡƷ��
		//char pPin[32];
		//sprintf(pPin,"<color=Cyan>%dƷ",smLevel);
		//strcat(pszMsg,pPin);
		strcat(pszMsg, "<color=Cyan>");
		strcat(pszMsg, szBlueInfo); //��ʶ���� ��ʾ
		strcat(pszMsg, "<color>\n");
	 }
   }
 }
//=================��ʾ��װ��ʶ���Խ���

//=================��ʾ��ʯ����
/* if  (m_CommonAttrib.nItemGenre==item_equip && m_CommonAttrib.nGoldId>0)
 {
	 if (GetBaoShiCount())
	    strcat(pszMsg, "<color=green>��ʯ���:<color>\n");
	 //else
     //   strcat(pszMsg, "<color=green>��ʯ���:<color>�������\n");

 //if (m_CommonAttrib.nGoldId > 0 && (m_CommonAttrib.nDetailType==equip_meleeweapon || m_CommonAttrib.nDetailType==equip_rangeweapon))
 //ֻ����������ʾ
	for (i = 0; i < 6; ++i)
	{
		if (!m_BaoShiMagicAttrib[i].nAttribType) //�������λ�õ�������û�����ݵ� // nAttribType�������ͱ��루int���֣�
		{
			//if (m_CommonAttrib.nGoldId>0 && m_CommonAttrib.nRongpiont>0)  //�����
				//strcat(pszMsg,"<color=yellow>[δ��Ƕ��ʯ]<color>");
				//strcat(pszMsg, "\n");
			continue;
		}
		pszInfor = (char *)g_MagicDesc.BaoShiGetDesc(&m_BaoShiMagicAttrib[i]);
		if (!pszInfor || !pszInfor[0])
			continue;
		//int smLevel=CheckWenPin(m_BaoShiMagicAttrib[i].nAttribType,m_BaoShiMagicAttrib[i].nValue[0]); //��ȡƷ��
		//char pPin[32];
		//sprintf(pPin,"<color=pink>%dƷ<color>",smLevel);
		//strcat(pszMsg,pPin);
		strcat(pszMsg, "<bclr=red><color=yellow>");
		strcat(pszMsg, pszInfor); //��ʯ���� ��ʾ
		strcat(pszMsg, "<color><bclr>\n");
	}
 } 	*/
///////////////////��ʾ��ʯ���Խ���//////////////////////////
/*//////////////////����������ʾ/////////////////////////////
	int YingIDA,YingIDB;
	char AszInfoy[16], BszInfoy[16];
    YingIDA=m_CommonAttrib.nYingNuma;
	YingIDB=m_CommonAttrib.nYingNumb;
 if (YingIDA>0)
 {
        strcat(pszMsg, "<color=White>");
	    sprintf(AszInfoy,"[��������1ID:%d]",YingIDA);
	    strcat(pszMsg, AszInfoy); //ħ������1 ��ʾ
	    strcat(pszMsg, "<color>\n");
 }
 if (YingIDB>0)
 {      strcat(pszMsg, "<color=White>");
	    sprintf(BszInfoy,"[��������2ID:%d]",YingIDB);
		strcat(pszMsg, BszInfoy); //ħ������2 ��ʾ
		strcat(pszMsg, "<color>\n");
 }
*///////////////////�������Խ���/////////////////////////////
	if (m_CommonAttrib.LimitTime.bYear>0) // ʱ������װ��
	{
		time_t rawtime;
		struct tm * timeinfo;
		time (&rawtime);
		timeinfo = localtime (&rawtime);
		//timeinfo->tm_year
		char sTmp[64];
		if (m_CommonAttrib.nIsWhere>0 && m_CommonAttrib.nCanUse==0)
		    sprintf(sTmp,"<color=Red>已到期:%d年%d月%d日%d时%d分<color>\n",m_CommonAttrib.LimitTime.bYear,m_CommonAttrib.LimitTime.bMonth,m_CommonAttrib.LimitTime.bDay,m_CommonAttrib.LimitTime.bHour,m_CommonAttrib.LimitTime.bMin);
		else if (m_CommonAttrib.nCanUse==0)
		{
			sprintf(sTmp,"<color=Green>使用期限至:%d年%d月%d日%d时%d分<color>\n",m_CommonAttrib.LimitTime.bYear,m_CommonAttrib.LimitTime.bMonth,m_CommonAttrib.LimitTime.bDay,m_CommonAttrib.LimitTime.bHour,m_CommonAttrib.LimitTime.bMin);
		    strcat(sTmp,"<color=red>不可装备<color>\n");
		}
		else
		    sprintf(sTmp,"<color=Green>使用期限至:%d年%d月%d日%d时%d分<color>\n",m_CommonAttrib.LimitTime.bYear,m_CommonAttrib.LimitTime.bMonth,m_CommonAttrib.LimitTime.bDay,m_CommonAttrib.LimitTime.bHour,m_CommonAttrib.LimitTime.bMin);

		strcat(pszMsg,sTmp);
/*        int mYear = m_CommonAttrib.LimitTime.bYear-1900-timeinfo->tm_year;
		int mMon  = m_CommonAttrib.LimitTime.bMonth-1-timeinfo->tm_mon;
		int mDay  = m_CommonAttrib.LimitTime.bDay-timeinfo->tm_mday;
		int mHour = m_CommonAttrib.LimitTime.bHour-timeinfo->tm_hour;
		int mMin  = m_CommonAttrib.LimitTime.bMin-timeinfo->tm_min;
        sprintf(sTmp,"<color=Green>ʣ��ʱ��:");
        strcat(pszMsg, sTmp);
        if (mYear>0)
		{
           sprintf(sTmp,"%d��",mYear);
		   strcat(pszMsg, sTmp);
		}
		if (mMon>0)
		{
		   sprintf(sTmp,"%d��",mMon);
		   strcat(pszMsg, sTmp);
		}
		if (mDay>0)
		{
			sprintf(sTmp,"%d��",mDay);
			strcat(pszMsg, sTmp);
		}

		if (mHour>0)
		{
			sprintf(sTmp,"%dСʱ",mHour);
			strcat(pszMsg, sTmp);
		}

        if (mMin>0)
		{
			sprintf(sTmp,"%d��",mMin);
			strcat(pszMsg, sTmp);
		}
*/
		strcat(pszMsg, " \n");
	}

    int IsShowThis=1;
	    g_GameSetTing.GetInteger("SYSTEM","IsShowDesc",1,&IsShowThis);   //�Ƿ���ʾ

	if (nType==0 && (m_CommonAttrib.nColor==2 || m_aryMagicAttrib[0].nAttribType) && IsShowThis) //װ������
	{
		strcat(pszMsg, " \n");
        switch(m_CommonAttrib.nDetailType)
        {
        case equip_meleeweapon:  //����
            {
             switch(m_CommonAttrib.nSeries)
                {
                  case series_metal:
					  {
			            strcat(pszMsg, "<color=Metal>");
			            strcat(pszMsg, "需要<color=Cyan>土系<color><color=Earth>衣服<color>和<color=Earth>项链<color>激活暗属性<color>\n");
					  }
			      break;
	         	  case series_wood:
					  {
			            strcat(pszMsg, "<color=Wood>");
			            strcat(pszMsg, "需要<color=Cyan>水系<color><color=Water>衣服<color>和<color=Water>项链<color>激活暗属性<color>\n");
					  }
			      break;
		          case series_water:
					  {
			            strcat(pszMsg, "<color=Water>");
		                strcat(pszMsg, "需要<color=Cyan>金系<color><color=Metal>衣服<color>和<color=Metal>项链<color>激活暗属性<color>\n");
					  }
			      break;
	        	  case series_fire:
					  {
			            strcat(pszMsg, "<color=Fire>");
			            strcat(pszMsg, "需要<color=Cyan>木系<color><color=Wood>衣服<color>和<color=Wood>项链<color>激活暗属性<color>\n");
					  }
			      break;
	        	  case series_earth:
					  {
			            strcat(pszMsg, "<color=Earth>");
			            strcat(pszMsg, "需要<color=Cyan>火系<color><color=Fire>衣服<color>和<color=Fire>项链<color>激活暗属性<color>\n");
					  }
			      break;
			 }
             break;
            }
        case equip_rangeweapon:  // ����
            {
                 switch(m_CommonAttrib.nSeries)
				 {
				    case series_metal:
						{
						   strcat(pszMsg, "<color=Metal>");
						   strcat(pszMsg, "需要<color=Cyan>土系<color><color=Earth>衣服<color>和<color=Earth>项链<color>激活暗属性<color>\n");
						}
					break;
				   case series_wood:
					   {
						   strcat(pszMsg, "<color=Wood>");
						   strcat(pszMsg, "需要<color=Cyan>水系<color><color=Water>衣服<color>和<color=Water>项链<color>激活暗属性<color>\n");
					   }
					break;
				   case series_water:
					   {
						   strcat(pszMsg, "<color=Water>");
						   strcat(pszMsg, "需要<color=Cyan>金系<color><color=Metal>衣服<color>和<color=Metal>项链<color>激活暗属性<color>\n");
					   }
					break;
				   case series_fire:
					   {
						   strcat(pszMsg, "<color=Fire>");
						   strcat(pszMsg, "需要<color=Cyan>木系<color><color=Wood>衣服<color>和<color=Wood>项链<color>激活暗属性<color>\n");
					   }
					break;
				   case series_earth:
					   {
						  strcat(pszMsg, "<color=Earth>");
						  strcat(pszMsg, "需要<color=Cyan>火系<color><color=Fire>衣服<color>和<color=Fire>项链<color>激活暗属性<color>\n");
					   }
					break;
				 }
             break;
            }
        case  equip_armor:
            {
                switch(m_CommonAttrib.nSeries)
                {
              	   case series_metal:
					   {
			             strcat(pszMsg, "<color=Metal>");
			             strcat(pszMsg, "需要<color=Cyan>土系<color><color=Earth>下戒<color>和<color=Earth>腰带<color>激活暗属性<color>\n");
					   }
			       break;
	         	   case series_wood:
					  {
			             strcat(pszMsg, "<color=Wood>");
			             strcat(pszMsg, "需要<color=Cyan>水系<color><color=Water>下戒<color>和<color=Water>腰带<color>激活暗属性<color>\n");
					  }
			       break;
		           case series_water:
					  {
			             strcat(pszMsg, "<color=Water>");
			             strcat(pszMsg, "需要<color=Cyan>金系<color><color=Metal>下戒<color>和<color=Metal>腰带<color>激活暗属性<color>\n");
					  }
			       break;
	        	   case series_fire:
					  {
			             strcat(pszMsg, "<color=Fire>");
			             strcat(pszMsg, "需要<color=Cyan>木系<color><color=Wood>下戒<color>和<color=Wood>腰带<color>激活暗属性<color>\n");
					  }
			       break;
	        	   case series_earth:
					  {
			            strcat(pszMsg, "<color=Earth>");
			            strcat(pszMsg, "需要<color=Cyan>火系<color><color=Fire>下戒<color>和<color=Fire>腰带<color>激活暗属性<color>\n");
					  }
                   break;
				}
				break;
            }
        case equip_ring:
            {
                switch(m_CommonAttrib.nSeries)
                {
              	  case series_metal:
			          strcat(pszMsg, "<color=Metal>");
			       break;
	         	  case series_wood:
			          strcat(pszMsg, "<color=Wood>");
			       break;
		          case series_water:
		              strcat(pszMsg, "<color=Water>");
			       break;
	        	  case series_fire:
			           strcat(pszMsg, "<color=Fire>");
		          break;
	        	  case series_earth:
		              strcat(pszMsg, "<color=Earth>");
			     //     strcat(pszMsg, "��Ҫ<color=Cyan>��ϵ<color><color=Fire>�½�<color>��<color=Fire>����<color>�������<color>\n");
			      break;
				  default:
					  strcat(pszMsg, "<color=White>");
					  break;
                }
				strcat(pszMsg, "上戒需要<color=Cyan>衣服和头盔,<color><color=Cyan>下戒<color>需要<color=Cyan>护腕和玉佩<color>激活暗属性<color>\n");
			    break;
            }
        case equip_amulet:
            {
                switch(m_CommonAttrib.nSeries)
                {
              	   case series_metal:
						{
			                strcat(pszMsg, "<color=Metal>");
			                strcat(pszMsg, "需要<color=Cyan>土系<color><color=Earth>下戒<color>和<color=Earth>腰带<color>激活暗属性<color>\n");
						}
			       break;
	         	   case series_wood:
					   {
			                strcat(pszMsg, "<color=Wood>");
			                strcat(pszMsg, "需要<color=Cyan>水系<color><color=Water>下戒<color>和<color=Water>腰带<color>激活暗属性<color>\n");
					   }
			       break;
		           case series_water:
					   {
			                strcat(pszMsg, "<color=Water>");
			                strcat(pszMsg, "需要<color=Cyan>金系<color><color=Metal>下戒<color>和<color=Metal>腰带<color>激活暗属性<color>\n");
					   }
			       break;
	        	   case series_fire:
					   {
			                strcat(pszMsg, "<color=Fire>");
			                strcat(pszMsg, "需要<color=Cyan>木系<color><color=Wood>下戒<color>和<color=Wood>腰带<color>激活暗属性<color>\n");
					   }
			       break;
	        	   case series_earth:
					   {
			                strcat(pszMsg, "<color=Earth>");
		                    strcat(pszMsg, "需要<color=Cyan>火系<color><color=Fire>下戒<color>和<color=Fire>腰带<color>激活暗属性<color>\n");
					   }
			       break;
                }
			break;
            }
         case equip_boots:
			 {
                switch(m_CommonAttrib.nSeries)
				{
              	   case series_metal:
					   {
			               strcat(pszMsg, "<color=Metal>");
			               strcat(pszMsg, "需要<color=Cyan>土系<color><color=Earth>头盔<color>和<color=Earth>武器<color>激活暗属性<color>\n");
					   }
			       break;
	               case series_wood:
					   {
			              strcat(pszMsg, "<color=Wood>");
			              strcat(pszMsg, "需要<color=Cyan>水系<color><color=Water>头盔<color>和<color=Water>武器<color>激活暗属性<color>\n");
					   }
			       break;
		           case series_water:
					   {
			              strcat(pszMsg, "<color=Water>");
			              strcat(pszMsg, "需要<color=Cyan>金系<color><color=Metal>头盔<color>和<color=Metal>武器<color>激活暗属性<color>\n");
					   }
			      break;
	        	  case series_fire:
					  {
			              strcat(pszMsg, "<color=Fire>");
			              strcat(pszMsg, "需要<color=Cyan>木系<color><color=Wood>头盔<color>和<color=Wood>武器<color>激活暗属性<color>\n");
					  }
		 	      break;
	        	  case series_earth:
					  {
			              strcat(pszMsg, "<color=Earth>");
			              strcat(pszMsg, "需要<color=Cyan>火系<color><color=Fire>头盔<color>和<color=Fire>武器<color>激活暗属性<color>\n");
					  }
			      break;
				}
            break;
			 }
           case equip_belt:
                    {
                       switch(m_CommonAttrib.nSeries)
                        {
              	          case series_metal:
							  {
			                     strcat(pszMsg, "<color=Metal>");
			                     strcat(pszMsg, "需要<color=Cyan>土系<color><color=Earth>护腕<color>和<color=Earth>玉佩<color>激活暗属性<color>\n");
							  }
			              break;
	         	          case series_wood:
							  {
			                     strcat(pszMsg, "<color=Wood>");
			                     strcat(pszMsg, "需要<color=Cyan>水系<color><color=Water>护腕<color>和<color=Water>玉佩<color>激活暗属性<color>\n");
							  }
			              break;
		                  case series_water:
							  {
			                     strcat(pszMsg, "<color=Water>");
			                     strcat(pszMsg, "需要<color=Cyan>金系<color><color=Metal>护腕<color>和<color=Metal>玉佩<color>激活暗属性<color>\n");
							  }
			              break;
	        	          case series_fire:
							  {
			                     strcat(pszMsg, "<color=Fire>");
			                     strcat(pszMsg, "需要<color=Cyan>木系<color><color=Wood>护腕<color>和<color=Wood>玉佩<color>激活暗属性<color>\n");
							  }
			              break;
	        	          case series_earth:
							  {
		                         strcat(pszMsg, "<color=Earth>");
			                     strcat(pszMsg, "需要<color=Cyan>火系<color><color=Fire>护腕<color>和<color=Fire>玉佩<color>激活暗属性<color>\n");
							  }
			              break;
					   }
                     break;
                    }
            case equip_helm:
                    {
                        switch(m_CommonAttrib.nSeries)
                        {
              	          case series_metal:
							   {
			                      strcat(pszMsg, "<color=Metal>");
			                      strcat(pszMsg, "需要<color=Cyan>土系<color><color=Earth>衣服<color>和<color=Earth>项链<color>激活暗属性<color>\n");
							   }
			              break;
	         	          case series_wood:
							  {
			                      strcat(pszMsg, "<color=Wood>");
			                      strcat(pszMsg, "需要<color=Cyan>水系<color><color=Water>衣服<color>和<color=Water>项链<color>激活暗属性<color>\n");
							  }
			              break;
		                  case series_water:
							  {
			                      strcat(pszMsg, "<color=Water>");
			                      strcat(pszMsg, "需要<color=Cyan>金系<color><color=Metal>衣服<color>和<color=Metal>项链<color>激活暗属性<color>\n");
							  }
			              break;
	        	          case series_fire:
							  {
			                      strcat(pszMsg, "<color=Fire>");
			                      strcat(pszMsg, "需要<color=Cyan>木系<color><color=Wood>衣服<color>和<color=Wood>项链<color>激活暗属性<color>\n");
							  }
			              break;
	        	          case series_earth:
							  {
			                      strcat(pszMsg, "<color=Earth>");
			                      strcat(pszMsg, "需要<color=Cyan>火系<color><color=Fire>衣服<color>和<color=Fire>项链<color>激活暗属性<color>\n");
							  }
			              break;
						}
                   break;
					}
            case equip_cuff:
                    {
                        switch(m_CommonAttrib.nSeries)
                        {
              	           case series_metal:
							   {
			                       strcat(pszMsg, "<color=Metal>");
			                       strcat(pszMsg, "需要<color=Cyan>土系<color><color=Earth>上戒<color>和<color=Earth>靴子<color>激活暗属性<color>\n");
							   }
			                break;
	         	           case series_wood:
							   {
			                       strcat(pszMsg, "<color=Wood>");
			                       strcat(pszMsg, "需要<color=Cyan>水系<color><color=Water>上戒<color>和<color=Water>靴子<color>激活暗属性<color>\n");
							   }
			                break;
		                   case series_water:
							   {
			                       strcat(pszMsg, "<color=Water>");
			                       strcat(pszMsg, "需要<color=Cyan>金系<color><color=Metal>上戒<color>和<color=Metal>靴子<color>激活暗属性<color>\n");
							   }
			                break;
	        	           case series_fire:
							   {
			                       strcat(pszMsg, "<color=Fire>");
			                       strcat(pszMsg, "需要<color=Cyan>木系<color><color=Wood>上戒<color>和<color=Wood>靴子<color>激活暗属性<color>\n");
							   }
			                break;
	        	           case series_earth:
							   {
			                       strcat(pszMsg, "<color=Earth>");
			                       strcat(pszMsg, "需要<color=Cyan>火系<color><color=Fire>上戒<color>和<color=Fire>靴子<color>激活暗属性<color>\n");
							   }
			                break;
						}
                    break;
					}
           case equip_pendant:
                    {
                        switch(m_CommonAttrib.nSeries)
						{
              	           case series_metal:
							   {
			                      strcat(pszMsg, "<color=Metal>");
			                      strcat(pszMsg, "需要<color=Cyan>土系<color><color=Earth>上戒<color>和<color=Earth>靴子<color>激活暗属性<color>\n");
							   }
			               break;
	         	           case series_wood:
							  {
			                      strcat(pszMsg, "<color=Wood>");
			                      strcat(pszMsg, "需要<color=Cyan>水系<color><color=Water>上戒<color>和<color=Water>靴子<color>激活暗属性<color>\n");
							  }
			               break;
		                   case series_water:
							  {
			                      strcat(pszMsg, "<color=Water>");
			                      strcat(pszMsg, "需要<color=Cyan>金系<color><color=Metal>上戒<color>和<color=Metal>靴子<color>激活暗属性<color>\n");
							  }
			               break;
			               case series_fire:
							   {
			                      strcat(pszMsg, "<color=Fire>");
			                      strcat(pszMsg, "需要<color=Cyan>木系<color><color=Wood>上戒<color>和<color=Wood>靴子<color>激活暗属性<color>\n");
							   }
			               break;
	        	           case series_earth:
							   {
			                      strcat(pszMsg, "<color=Earth>");
			                      strcat(pszMsg, "需要<color=Cyan>火系<color><color=Fire>上戒<color>和<color=Fire>靴子<color>激活暗属性<color>\n");
							   }
			               break;
						}
                       break;
					}
		   default:
			    strcat(pszMsg, "<color=Cyan>请看F1帮助五行激活图<color>\n");
                break;
				}
        }

/*	if (m_CommonAttrib.nGoldId>0)
	{
    	strcat(pszMsg, "\n");
		KTabFile GoldTab;
		char szGoldName[80]={0};
		GoldTab.Load("\\Settings\\item\\004\\GoldItem.txt");

		for (int k = 0;k < m_CommonAttrib.nSetNum;++k)  //��ʾ��װ�������
		{
			GoldTab.GetString(m_CommonAttrib.nGoldId + 3 - m_CommonAttrib.nSetId + k,1,"",szGoldName,sizeof(szGoldName));

			int ncolor = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetGoldColor(m_CommonAttrib.nSet,k + 1);

			//if (m_CommonAttrib.nSetId == k + 1)  //��ǰװ�� //��չ��װ���
			//{
			//	strcat(pszMsg, "<color=Yellow>");
			//	strcat(pszMsg, szGoldName);
			//	strcat(pszMsg, "<color>\n");
			//}

		    if (!szGoldName[0])
			   continue;

			if (ncolor == 0)
			{
				strcat(pszMsg, "<color=DGreen>");
			}
			else if (ncolor == 1)
			{//������Ʒ
				strcat(pszMsg, "<color=HGreen>");
			}
			else if (ncolor == 2)
			{//װ���ŵ�
				strcat(pszMsg, "<color=Green>");
			}
			else
				strcat(pszMsg, "<color=White>");

			strcat(pszMsg, szGoldName);
			strcat(pszMsg, "<color>\n");

		}	  //end for

		GoldTab.Clear();
	}


		if (m_CommonAttrib.nIsWhere==-1)
		{
		sprintf(pPin,"<color=yellow>��װ������ϴ���������������");
		strcat(pszMsg,pPin);
		strcat(pszMsg, "<color>\n");
	 }

	*/

	if (nType==0 && m_CommonAttrib.nRongNum>0)
	{
		 char sRong[32]={0};
		 char sDong[32]={0};

         if (m_CommonAttrib.nIsWhere==-1 && (IsBlue() || IsPurple()))
		 {
			strcat(pszMsg,"\n");
			strcat(pszMsg, "<color=Green>");
			int mRmun=0;
			for (int nRong=0;nRong<6;++nRong)
			{
				if (m_ronMagicAttrib[nRong].nAttribType)
					mRmun++;
			}

			if (GetIsWhere()>0)
			{
			   sprintf(sRong,"已洗练属性：%d/%d个",mRmun,m_CommonAttrib.nRongNum);
			}
			else
			{
               sprintf(sRong,"已洗练属性：0/%d个",m_CommonAttrib.nRongNum);
			}

			strcat(pszMsg,sRong);
			strcat(pszMsg,"\n");

			sprintf(sDong,"最大洗练属性数量为%d",m_CommonAttrib.nRongNum);
			strcat(pszMsg,sDong);
			strcat(pszMsg,"<color>\n");
		 }
		 else if (m_CommonAttrib.nGoldId>0)
		 {
			 strcat(pszMsg,"\n");
			 strcat(pszMsg, "<color=Green>");
			 int mRmun=0;
			 for (int nRong=0;nRong<6;++nRong)
			 {
				 if (m_ronMagicAttrib[nRong].nAttribType>0)
					 mRmun++;
			 }

			 sprintf(sRong,"已熔炼纹钢数量：%d/%d个",mRmun,m_CommonAttrib.nRongNum);
			 strcat(pszMsg,sRong);
			 strcat(pszMsg,"\n");

			 sprintf(sDong,"最大可熔炼纹钢等级为%d",m_CommonAttrib.nWengangPin);
			 strcat(pszMsg,sDong);
			 strcat(pszMsg,"<color>\n");
		 }
	}


	if (nType==0 && m_CommonAttrib.nBinfujiazhi>0)
	{
		strcat(pszMsg, "<color=HGREEN>");
		char sBong[32]={0};
		sprintf(sBong,"基础兵甲值：%d",m_CommonAttrib.nBinfujiazhi);
		strcat(pszMsg,sBong);
		strcat(pszMsg,"\n");
		strcat(pszMsg,VER_INFO);
		strcat(pszMsg,"<color>\n");
	}
/*	else
	{
		strcat(pszMsg, "<color=HGREEN>");
		strcat(pszMsg,"��������ֵ��0");
		strcat(pszMsg,"\n");
		strcat(pszMsg,VER_INFO);
		strcat(pszMsg,"<color>\n");
	}*/


	if (m_CommonAttrib.uPrice > 0)  //��̯���
	{
	    char nD[8];
        if (m_CommonAttrib.ncSellModel==2)
	      sprintf(nD,"金");
	    else
          sprintf(nD,"两");

		strcat(pszMsg,"\n");
		strcat(pszMsg, "<color=Yellow>");
		char sOrice[32];

		int nN = m_CommonAttrib.uPrice/100000000;//TakeTrader(m_CommonAttrib.uPrice,100000000);
		int nW = (m_CommonAttrib.uPrice%100000000)/10000;//TakeTrader(TakeRemainder(m_CommonAttrib.uPrice,100000000),10000);
		int nG = m_CommonAttrib.uPrice%10000;//TakeRemainder(m_CommonAttrib.uPrice,10000);

	    if (nN > 0)
		{
		   sprintf(sOrice,"摆摊价值：%d亿%d万%d%s",nN,nW,nG,nD);
		}
	    else
		{
		   if (nW == 0)
		      sprintf(sOrice,"摆摊价值：%d%s",nG,nD);
		   else
		      sprintf(sOrice,"摆摊价值：%d万%d%s",nW,nG,nD);
		}
		strcat(pszMsg,sOrice);
		strcat(pszMsg,"<color>\n");
	}

    if (m_CommonAttrib.nGoldId>0 /*&& m_CommonAttrib.nSet>0*/)
	{
    	strcat(pszMsg, "\n");
		KTabFile GoldTab;
		char szGoldName[80]={0};

		if (m_CommonAttrib.nIsPlatina ==0 )
		    GoldTab.Load("\\Settings\\item\\004\\GoldItem.txt");
		else
			GoldTab.Load("\\Settings\\item\\004\\platinaequip.txt");

		for (int k = 0;k < m_CommonAttrib.nSetNum;++k)  //��ʾ��װ�������
		{
			GoldTab.GetString(m_CommonAttrib.nGoldId + 3 - m_CommonAttrib.nSetId + k,1," ",szGoldName,sizeof(szGoldName));

			int ncolor = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetGoldColor(m_CommonAttrib.nSet,k + 1);

		    if (!szGoldName[0])
			   continue;

			if (ncolor == 0)
			{
				strcat(pszMsg, "<color=DGreen>");
			}
			else if (ncolor == 1)
			{//������Ʒ
				strcat(pszMsg, "<color=HGreen>");
			}
			else if (ncolor == 2)
			{//װ���ŵ�
				strcat(pszMsg, "<color=Fire>");		  //Yellow
			}
			else
				strcat(pszMsg, "<color=White>");

		   /*	if (m_CommonAttrib.nSetId == k + 1)  //��ǰװ�� //��չ��װ���
			{
				strcat(pszMsg, "<color=Orange>");
				strcat(pszMsg, szGoldName);
				strcat(pszMsg, "<color>\n");
			}*/

			strcat(pszMsg, szGoldName);
			strcat(pszMsg, "<color>\n");

		}	  //end for

		GoldTab.Clear();
	}

	//strcat(pszMsg,"\0");
}

int KItem::ExtractChars(const char *inStr,int scrLen,char *outStr)
{
	char *tmp=NULL;         //����һ����ʱ����ռ䣬����ַ���
	tmp=outStr;
	int nLen=0;
	while(*inStr!='\0')
	{//һֱǰ��,ֱ���н������Ž�ֹͣ��
		if(nLen==scrLen) //���������ȵ�ʱ��
		{
			//*inStr='|';   //�滻����ʼ�ַ�
			*tmp++;         //�¸��ַ�
            *tmp='\n';      //�Ӹ����з���
			//inStr++;             //ȥ����������
		}
		if (*inStr=='\0')          //���û���ҵ��������������Ѿ����˽�β���������ˣ���
			break;
		*tmp=*inStr;               //Ԫ������ͬ�����ŵ�tmp�С�
		tmp++;                     //������һ��Ԫ���ıȽ�
		inStr++;
		nLen++;
	}
	*tmp='\0';              //Դ�ַ��� ��ȡ��� �Ӹ������� ��ֹ����
    return nLen;
}


//���־ö�
int KItem::GetMaxDurability()
{
	for (int i = 0; i < 7; ++i)
	{
		if (m_aryBaseAttrib[i].nAttribType == magic_durability_v)
		{
			return m_aryBaseAttrib[i].nValue[0];
		}
	}
	return -1;
}
//ȡ��װ�������Եȼ�����
int KItem::GetTotalMagicLevel()
{
	int nRet = 0;
	for (int i = 0; i < 6; ++i)
	{
		//nRet += m_GeneratorParam.nGeneratorLevel[i];
		if (m_GeneratorParam.nGeneratorLevel[i])
			nRet++;
		if (m_GeneratorParam.nRGeneratorLevel[i])
			nRet++;
		//if (m_GeneratorParam.nBGeneratorLevel[i])
		//	nRet++;
		//if (m_GeneratorParam.nBGeneratorLevel[i])
		//	nRet++;
	}

	return nRet;
}
//��ȡ����۸�
int KItem::GetRepairPrice()
{
	if (ItemSet.m_sRepairParam.nMagicScale == 0)
		return 0;

	if (GetGenre() != item_equip)          //����װ��
		return 0;

	if (m_nCurrentDur == -1)               //����ĥ��
		return 0;

	int nMaxDur   = GetMaxDurability();
	int nSumMagic = GetTotalMagicLevel();  //ħ�����Ե��ܵȼ�

	if (nMaxDur <= 0)                      //���־ö�û��
		return 0;

	int nRepairPrice=0;
        nRepairPrice = m_CommonAttrib.nLevel*ItemSet.m_sRepairParam.nLevelScale + m_CommonAttrib.nPrice * ItemSet.m_sRepairParam.nPriceScale/100 * (nMaxDur - m_nCurrentDur)/nMaxDur * (ItemSet.m_sRepairParam.nMagicScale + nSumMagic)/ItemSet.m_sRepairParam.nMagicScale;
    //����۸�=��Ʒ�ȼ�*�ȼ�����+��Ʒ�۸�*�۸���/100*(���־�-��ǰ�־�)/���־�*(ħ������+ħ������)/ħ������
	//����Ʒ�ļ۸�Xά�޼۸�ı���/100
	return nRepairPrice;
}

//�Ƿ����޸�
int KItem::CanBeRepaired()
{
	if (GetGenre() != item_equip)
		return FALSE;

	//if (GetIsWhere()>0 && GetIsUse()<=0) //��ʶװ �����ǲ���ʹ�õ� ��������
	//   return FALSE;

	if (m_nCurrentDur == -1)  //��Զ��ĥ��
		return FALSE;

	int nMaxDur = GetMaxDurability();
	if (m_nCurrentDur == nMaxDur)
		return FALSE;

	return TRUE;
}

int KItem::CheckItemInfo(int nGen,int nDetail,int nParticular,const char *KeyName)
{
	int nItemInfo[3];
	int nRows = g_ForbitMap.GetHeight();
	bool nReturn = false;

	for (int i = 2;i<nRows+1;i++)
	{
		g_ForbitMap.GetInt3s(i,KeyName,nItemInfo);
		if (nItemInfo[0] == nGen && nItemInfo[1]==nDetail && nItemInfo[2]==nParticular)
		{
			nReturn = true;
			break;
		}
	}
	return nReturn;
}

//�Ƿ���Ե���
int KItem::CanStack(int nOldIdx)
{
	if (m_CommonAttrib.bStack)
	{
		int nGen        = Item[nOldIdx].GetGenre();
		int nDetail     = Item[nOldIdx].GetDetailType();
		int nParticular = Item[nOldIdx].GetParticular();
		int nSeries     = Item[nOldIdx].GetSeries();
		int m_nLevel    = Item[nOldIdx].GetLevel();

		if (m_CommonAttrib.nItemGenre == nGen && m_CommonAttrib.nDetailType == nDetail && m_CommonAttrib.nParticularType == nParticular && Item[nOldIdx].GetStackNum() < MAX_STACKNUM)   //����С��50 �Ϳ��Ե���
		{
			if (CheckItemInfo(nGen,nDetail,nParticular,"NeedLevel"))
			{//���ȼ�
				if (m_CommonAttrib.nLevel != m_nLevel)
					return FALSE;
			}

			if (CheckItemInfo(nGen,nDetail,nParticular,"NeedSeries"))
			{//���ȼ�
				if (m_CommonAttrib.nSeries != nSeries)
					return FALSE;
			}

			return TRUE;
		}
	}
	return FALSE;
}
//�Ƿ�����ٵ���
int KItem::Stack( int nIdx )
{
	m_CommonAttrib.nStackNum += nIdx;

	if (m_CommonAttrib.nStackNum > MAX_STACKNUM)  //�ɵ�����Ʒ����������
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void KItem::SetBackUpMagicAttrib()
{
  for (int i = 0;i < 6;++i)
  {
    m_TempMagicAttrib[i].nValue[0]=m_aryMagicAttrib[i].nValue[0];
  }
}

void KItem::SetBackUpRmagicAttrib()
{
  for (int i = 0;i < 6;++i)
  {
    m_TempRMagicAttrib[i].nValue[0]=m_ronMagicAttrib[i].nValue[0];
  }
}

void KItem::SetBackUpBmagicAttrib()
{
/*  for (int i = 0;i < 6;++i)
  {
    m_TempBMagicAttrib[i].nValue[0]=m_BaoShiMagicAttrib[i].nValue[0];
  }*/
}

//ħ��װ����ǿ��
int KItem::EnChance( int nEnChance /*= 1*/ )
{
	    int nCanEn=0,i;

		if (nEnChance < 31)
		{
		   //int nOldEnChance = m_CommonAttrib.nEnChance;  //ԭ���ļӳ�

			for (i = 0;i < 6;++i)
			{
				nCanEn=CheckEnChance("ForBitEn",m_aryMagicAttrib[i].nAttribType);

				if (m_aryMagicAttrib[i].nAttribType && nCanEn==1)
				{
					if (m_aryMagicAttrib[i].nValue[0] >= (JIACHENG_VAL<<1))
					{
						   int nTempVal=m_TempMagicAttrib[i].nValue[0]; //ԭʼֵ

                            for (int j=0;j<nEnChance;++j)	//nEnChance+1
							{
							     nTempVal +=nTempVal>>JIACHENG_VAL;//*j
						        //if (nTempVal%JIACHENG_VAL>=2)
						        if (nTempVal-(nTempVal>>JIACHENG_VAL<<JIACHENG_VAL)>2)
						           nTempVal +=1;
							}

						    m_aryMagicAttrib[i].nValue[0] = nTempVal;
					}
				}
			}

			m_CommonAttrib.nEnChance = nEnChance;
		}
		else
		{
			m_CommonAttrib.nEnChance = 30;
		}

	return 1;
}
//ǿ����������
int KItem::EnChanceRong( int nEnChance /*= 1*/ )
{
	int nCanEn=0;
	if (nEnChance < 31)
	{
		int nOldEnChance = m_CommonAttrib.nEnChance;//ԭ���ļӳ�

		for (int i = 0;i < 6;++i)
		{
			nCanEn=CheckEnChance("ForBitEn",m_aryMagicAttrib[i].nAttribType);

		    if (m_aryMagicAttrib[i].nAttribType && nCanEn==1)
			{//��ɫ����
			    if (m_aryMagicAttrib[i].nValue[0] >= (JIACHENG_VAL<<1))
				{
				    int nTempVal=m_TempMagicAttrib[i].nValue[0]; //ԭʼֵ

				    for (int j=0;j<nEnChance;++j)
					{
					     nTempVal +=nTempVal>>JIACHENG_VAL;//*j
						        //if (nTempVal%JIACHENG_VAL>=2)
						 if (nTempVal-(nTempVal>>JIACHENG_VAL<<JIACHENG_VAL)>2)
						     nTempVal +=1;
					}

				    m_aryMagicAttrib[i].nValue[0] = nTempVal;
				}
			}

			nCanEn=CheckEnChance("ForBitEn",m_ronMagicAttrib[i].nAttribType);
			if (m_ronMagicAttrib[i].nAttribType && nCanEn==1)
			{//��ʶ�� �Ƹ�����
				if (m_ronMagicAttrib[i].nValue[0] >= (JIACHENG_VAL<<1))
				{
						/* if (nEnChance>nOldEnChance)      //�ɹ���
						 {
						    m_TempRMagicAttrib[i].nValue[0]=m_ronMagicAttrib[i].nValue[0]; //�����������ԭ���� ����ֵ������ǿ��ʧ���õģ�
						    m_ronMagicAttrib[i].nValue[0] += m_ronMagicAttrib[i].nValue[0] * nEnChance/JIACHENG_VAL;
						  }
						 else if (nEnChance< nOldEnChance) //ʧ�ܵ�
						 {
						    m_ronMagicAttrib[i].nValue[0] = m_TempRMagicAttrib[i].nValue[0];//m_ronMagicAttrib[i].nValue[0] * (nEnChance+1)/JIACHENG_VAL;
						 }*/
						 int nTempVal=m_TempRMagicAttrib[i].nValue[0]; //ԭʼֵ

                            for (int j=0;j<nEnChance;++j)
							{
							     nTempVal +=nTempVal>>JIACHENG_VAL;//*j
						        //if (nTempVal%JIACHENG_VAL>=2)
						        if (nTempVal-(nTempVal>>JIACHENG_VAL<<JIACHENG_VAL)>2)
						           nTempVal +=1;
							}

						    m_ronMagicAttrib[i].nValue[0] = nTempVal;

				}
			}
		}
	   m_CommonAttrib.nEnChance = nEnChance;
	}
	else
	{
		m_CommonAttrib.nEnChance = 30;

		/*for (int i = 0;i < 6;++i)
		{
			nCanEn=CheckEnChance("ForBitEn",m_ronMagicAttrib[i].nAttribType);
			if (m_ronMagicAttrib[i].nAttribType && nCanEn==0)
			{
				if (m_ronMagicAttrib[i].nValue[0] >= 5)
				{
					m_ronMagicAttrib[i].nValue[0] += m_ronMagicAttrib[i].nValue[0] * nEnChance / 18;
				}
			}
		}*/
	}

	return 1;
}

//ǿ����ʯ����
int KItem::EnChanceBao( int nEnChance /*= 1*/)
{
/*	int nCanEn=0;
	if (nEnChance < 31)
	{
	    int nOldEnChance = m_CommonAttrib.nEnChance;//ԭ���ļӳ�

		for (int i = 0;i < 6;++i)
		{
			nCanEn=CheckEnChance("ForBitEn",m_BaoShiMagicAttrib[i].nAttribType);

			if (m_BaoShiMagicAttrib[i].nAttribType && nCanEn==0)
			{
				if (m_BaoShiMagicAttrib[i].nValue[0] >= JIACHENG_VAL)
				{
					if (nEnChance>nOldEnChance)      //�ɹ���
					{
					    m_TempBMagicAttrib[i].nValue[0]=m_BaoShiMagicAttrib[i].nValue[0]; //�����������ԭ���� ����ֵ������ǿ��ʧ���õģ�
					    m_BaoShiMagicAttrib[i].nValue[0] += m_BaoShiMagicAttrib[i].nValue[0] * nEnChance/JIACHENG_VAL;
					}
					else if (nEnChance< nOldEnChance) //ʧ�ܵ�
					{//ʧ�ܾͻ�ԭԭ����ֵ
						m_BaoShiMagicAttrib[i].nValue[0] = m_TempBMagicAttrib[i].nValue[0];//m_BaoShiMagicAttrib[i].nValue[0] * (nEnChance+1)/JIACHENG_VAL;
				    }
					int nTempVal=m_TempBMagicAttrib[i].nValue[0]; //ԭʼֵ
                            for (int j=0;j<nEnChance+1;++j)
							     nTempVal +=nTempVal*j/JIACHENG_VAL;

						    m_BaoShiMagicAttrib[i].nValue[0] = nTempVal;
				}
			}
		}

	  m_CommonAttrib.nEnChance = nEnChance;
	}
	else
	{
		m_CommonAttrib.nEnChance = 30;
	}
	*/
	return 1;
}


int	KItem::CheckEnChance(const char *nKey,int nAttribType)
{
	int nRow = g_ForbitMap.GetHeight()+1,nReg=0;

	for (int i=2;i<nRow;++i)
	{
		int nMaps =0;

		g_ForbitMap.GetInteger(i,nKey,0,&nMaps);

		if (nAttribType==nMaps)
		{
			nReg =1;
			break;
		}
	}
	return nReg;
}


int KItem::IsReceptacle()
{
	return 1;
}


int KItem::HoldItem(int nIdx, int nWidth, int nHeight)
{
//	int i, j;
	//for (i = 0; i < m_nWidth - nWidth + 1; ++i)
//	{
//		for (j = 0; j < m_nHeight - nHeight + 1; ++j)
//		{
	//		if (PlaceItem(i, j, nIdx, nWidth, nHeight))
				return TRUE;
	//	}
//	}
//	return FALSE;
}

int KItem::IsPurple()  //����װ
{
	if (m_CommonAttrib.nPoint>0 && m_CommonAttrib.nPoint<=6)
	return m_CommonAttrib.nPoint;

	return 0;

}

int KItem::GetPoint()
{

   return m_CommonAttrib.nPoint;

}

int KItem::CheckReqability(int n)
{
	for (int i=0;i<6;++i)
	{
		if (m_aryRequireAttrib[i].nAttribType==n)
	     return TRUE;
	}
	return FALSE;
}

int KItem::GetIsWhere()
{

   return m_CommonAttrib.nIsWhere;

}

int KItem::getItemKind()
{
	if  (m_CommonAttrib.nGoldId>0)
		return 1; //TB Hoang Kim
	else if ((m_CommonAttrib.nPoint==7 || m_CommonAttrib.nPoint==0) && m_aryMagicAttrib[0].nAttribType>0)
		return 2; //TB XANH
	else if (m_CommonAttrib.nPoint>0 && m_CommonAttrib.nPoint<=6)
	    return 3; //TB TIM
	else
		return 0;//TB THUONG
}

int KItem::IsBlue()  //TB XANH
{
	if ((m_CommonAttrib.nPoint==7 || m_CommonAttrib.nPoint==0) && m_CommonAttrib.nGoldId<=0 && m_aryMagicAttrib[0].nAttribType>0)
    return 1;

    return 0;
}

int KItem::IsPlatina()  //TB BACH KIM

{
	if (m_CommonAttrib.nGoldId>0 && m_CommonAttrib.nIsPlatina>0)
		return 1;

    return 0;
}


int KItem::IsRong()  //������
{
	return m_CommonAttrib.nRongpiont;
}

int KItem::IsGold()  //������
{
	return m_CommonAttrib.nGoldId;
}

//������Ʒ��ʹ��ʱ��
void KItem::SetTime( int bYear,int bMonth,int bDay,int bHour,int bMin)
{
	if (bYear>0)
	{
		while (bMin >= 60)
		{
			bHour++;
			bMin -= 60;
		}

		while (bHour >= 24)
		{
			bDay++;
			bHour -= 24;
		}

		int nIsYear=FALSE;

		int  nAllDay=30;

		if (bYear%4==0)	   //����
		{//����
		   nIsYear=TRUE;
		   //if (bMonth==2)  //����
		//	   nAllDay=29;
		}
		else
		{//ƽ��
 			nIsYear=FALSE;
			//if (bMonth==2)	//����
			 //  nAllDay=28;
		}
		//-----����ʱ��ĵ���
		if (bMonth<=12)
		{//С��12����
			if (bMonth==2 && nIsYear)
			{//�����2�·�
				nAllDay=29;
			}
			else if (bMonth==2 && !nIsYear)
			{//ƽ���2�·�
				nAllDay= 28;
			}
			else
			{
				if (bMonth==1 || bMonth==3 || bMonth==5|| bMonth==7|| bMonth==8|| bMonth==10 || bMonth==12)
				{//����
					nAllDay=31;
				}
				else if (bMonth==4 || bMonth==6 || bMonth==9|| bMonth==11)
				{//����
					nAllDay=30;
				}
			}
		}
		else
		{//���·ݴ���12���µ�
			if (bMonth%12==2 && nIsYear)
			{//�����2�·�
				nAllDay=29;
			}
			else if (bMonth%12==2 && !nIsYear)
			{//ƽ���2�·�
				nAllDay= 28;
			}
			else
			{
				if (bMonth%12==0 || bMonth%12==1 || bMonth%12==3 || bMonth%12==5|| bMonth%12==7|| bMonth%12==8|| bMonth%12==10)
				{//����
					nAllDay=31;
				}
				else if (bMonth%12==4 || bMonth%12==6 || bMonth%12==9|| bMonth%12==11)
				{//����
					nAllDay=30;
				}
			}
		}

	 while (bDay > nAllDay)
	 {

	/*		if (((bMonth <= 7) && !(bMonth % 2)) || ((bMonth > 7) && !(bMonth % 2)))
			{//˫�� 2 4 6 8 10 12
				bMonth++;
				bDay -= 30;
			}
			else if (((bMonth <= 7) && !(bMonth % 2) && bDay > 31 )||((bMonth > 7) && (bMonth % 2) && bDay > 31))
			{//˫�� 2 4 6  ��   ���� 9 11  ������31
				bMonth++;
				bDay -= 31;
			}
			else if (((bMonth <= 7) && !(bMonth % 2) && bDay == 31 )||((bMonth > 7) && (bMonth % 2) && bDay == 31))
			{//���� 1 3 5 7  ����=31 �� ˫�� 8 10 12 ����=31
				break;
			}*/

			bMonth++;
			bDay -= nAllDay;

			//-----------��һ����----------
		 if (bMonth<=12)
		 {//С��12����
			if (bMonth==2 && nIsYear)
			{//�����2�·�
				nAllDay=29;
			}
			else if (bMonth==2 && !nIsYear)
			{//ƽ���2�·�
				nAllDay= 28;
			}
			else
			{
			   	if (bMonth==1 || bMonth==3 || bMonth==5|| bMonth==7|| bMonth==8|| bMonth==10 || bMonth==12)
				{//����
					nAllDay=31;
				}
				else if (bMonth==4 || bMonth==6 || bMonth==9|| bMonth==11)
				{//����
					nAllDay=30;
				}
			}
		 }
		 else
		 {//���·ݴ���12���µ�
				if (bMonth%12==2 && nIsYear)
				{//�����2�·�
					nAllDay=29;
				}
				else if (bMonth%12==2 && !nIsYear)
				{//ƽ���2�·�
					nAllDay= 28;
				}
				else
				{
					if (bMonth%12==0 || bMonth%12==1 || bMonth%12==3 || bMonth%12==5|| bMonth%12==7|| bMonth%12==8|| bMonth%12==10)
					{//����
						nAllDay=31;
					}
					else if (bMonth%12==4 || bMonth%12==6 || bMonth%12==9|| bMonth%12==11)
					{//����
						nAllDay=30;
					}
			}

		 }

	 }

		while (bMonth > 12)
		{
			bYear++;
			bMonth -= 12;
		}

		if (bDay<=0)
		{
			bDay=1;
        }

		if (bMonth<=0)
			bMonth=1;

		if (bHour<0)
			bHour=1;

        if (bMin<0)
			bMin=0;

		m_CommonAttrib.LimitTime.bYear  = bYear;
		m_CommonAttrib.LimitTime.bMonth = bMonth;
		m_CommonAttrib.LimitTime.bDay   = bDay;
		m_CommonAttrib.LimitTime.bHour  = bHour;
		m_CommonAttrib.LimitTime.bMin   = bMin;

	}
	else
	{
	    m_CommonAttrib.LimitTime.bYear  = 0;
		m_CommonAttrib.LimitTime.bMonth = 0;
		m_CommonAttrib.LimitTime.bDay   = 0;
		m_CommonAttrib.LimitTime.bHour  = 0;
		m_CommonAttrib.LimitTime.bMin   = 0;
	}
}

//����Ƹֵ�Ʒ��
int  KItem::CheckBluePin(int pShuXingID,int pVal)

{
	KTabFile MagicTab,MagicTabA;
	MagicTab.Load(TABFILE_MAGICALEVEL_PATH);
	int nRows=MagicTab.GetHeight();
	int nReg=0,nStart=0,nEnd=0;

	for (int i=0;i<nRows;++i)
	{
		int nMagid=0;
		MagicTab.GetInteger(i+1,"MAGIC_ID",1,&nMagid);
		if (nMagid==pShuXingID) //�Ƿ�����Ƹ���ȡ������
		{
			MagicTab.GetInteger(i+1,"MAGATTRLVL_START",0,&nStart);
			MagicTab.GetInteger(i+1,"MAGATTRLVL_END",0,&nEnd);
			nReg=i;
			break;
		}
	}
    MagicTab.Clear();

	if (nReg==0)  //û�п�����ȡ������
            return 0;

    if (nReg&&nStart&&nEnd)  //�õ�����
	{
		MagicTabA.Load(TABFILE_MAGICATTRIB_PATH);
		int nRowsA=MagicTabA.GetHeight(),nAValmin=0,nAValmax=0,nLevel=0;
		for (nStart;nStart<nEnd;++nStart)
		{
			MagicTabA.GetInteger(nStart+1,6,1,&nAValmin);
			MagicTabA.GetInteger(nStart+1,7,1,&nAValmax);
			if (pVal>=nAValmin && pVal<=nAValmax) //�������Χ��
			{
				MagicTabA.GetInteger(nStart+1,4,1,&nLevel);
				break;
			}

		}
		MagicTabA.Clear();
		return nLevel;
	}
    return 0;
}

//����Ƹֵ�Ʒ��
int  KItem::CheckWenPin(int pShuXingID,int pVal)

{
	KTabFile MagicTab,MagicTabA;
	MagicTab.Load(TABFILE_MAGICALEVEL_PATH);
	int nRows=MagicTab.GetHeight();
	int nReg=0,nStart=0,nEnd=0;

	for (int i=0;i<nRows;++i)
	{
		int nMagid=0;
		MagicTab.GetInteger(i+1,"MAGIC_ID",1,&nMagid);
		if (nMagid==pShuXingID)
		{
			MagicTab.GetInteger(i+1,"MAGATTRLVL_START",0,&nStart);
			MagicTab.GetInteger(i+1,"MAGATTRLVL_END",0,&nEnd);
			nReg=i;
			break;
		}
	}
    MagicTab.Clear();

    if (nReg&&nStart&&nEnd)  //�õ�����
	{
		MagicTabA.Load(TABFILE_MAGICATTRIB_PATH);
		int nRowsA=MagicTabA.GetHeight(),nAVal=0,nLevel=0;
		for (nStart;nStart<nEnd;++nStart)
		{
			MagicTabA.GetInteger(nStart+1,6,1,&nAVal);
			if (nAVal==pVal)
			{
				MagicTabA.GetInteger(nStart+1,4,1,&nLevel);
				break;
			}

		}
		MagicTabA.Clear();
		return nLevel;
	}
    return 0;
}

//�����Ƹ�����ID��ȡ��Ʒ�� eventid ��
int  KItem::GeteventID(int pShuXingID)

{
	KTabFile MagicTab;
	MagicTab.Load(TABFILE_MAGICALEVEL_PATH);
	int nRows=MagicTab.GetHeight();
	int nReg=0;
	for (int i=0;i<nRows;++i)
	{
		int nMagid=0;
		MagicTab.GetInteger(i+1,"MAGIC_ID",1,&nMagid);
		if (nMagid==pShuXingID)
		{
			nReg=i;
			break;
		}
	}
    MagicTab.Clear();

		return nReg;
}

void KItem::SetItemMagicLevel(int i,int nRow)
{
	m_aryBaseAttrib[i].nValue[0]=nRow;
}

int KItem::GetItemMagicLevel(int i)
{
   return m_aryBaseAttrib[i].nValue[0];
}
//�Ƿ���������� װ��
int KItem::HaveMaigc( int nAttribe,int nValue1Min,int nValue1Max,int nValue2Min,int nValue2Max,int nValue3Min,int nValue3Max )
{
	for (int i = 0;i < 6;++i)  //ħ��
	{
		if (m_aryMagicAttrib[i].nAttribType == nAttribe)
		{
			if (nValue1Min != -1)
			{
				if (m_aryMagicAttrib[i].nValue[0] < nValue1Min)
					return FALSE;
			}
			if (nValue1Max != -1)
			{
				if (m_aryMagicAttrib[i].nValue[0] > nValue1Max)
					return FALSE;
			}
			if (nValue2Min != -1)
			{
				if (m_aryMagicAttrib[i].nValue[1] < nValue2Min)
					return FALSE;
			}
			if (nValue2Max != -1)
			{
				if (m_aryMagicAttrib[i].nValue[1] > nValue2Max)
					return FALSE;
			}
			if (nValue3Min != -1)
			{
				if (m_aryMagicAttrib[i].nValue[2] < nValue3Min)
					return FALSE;
			}
			if (nValue3Max != -1)
			{
				if (m_aryMagicAttrib[i].nValue[2] > nValue3Max)
					return FALSE;
			}
			return TRUE;
		}
	   else if (m_ronMagicAttrib[i].nAttribType == nAttribe)
		{
			if (nValue1Min != -1)
			{
				if (m_ronMagicAttrib[i].nValue[0] < nValue1Min)
					return FALSE;
			}
			if (nValue1Max != -1)
			{
				if (m_ronMagicAttrib[i].nValue[0] > nValue1Max)
					return FALSE;
			}
			if (nValue2Min != -1)
			{
				if (m_ronMagicAttrib[i].nValue[1] < nValue2Min)
					return FALSE;
			}
			if (nValue2Max != -1)
			{
				if (m_ronMagicAttrib[i].nValue[1] > nValue2Max)
					return FALSE;
			}
			if (nValue3Min != -1)
			{
				if (m_ronMagicAttrib[i].nValue[2] < nValue3Min)
					return FALSE;
			}
			if (nValue3Max != -1)
			{
				if (m_ronMagicAttrib[i].nValue[2] > nValue3Max)
					return FALSE;
			}
			return TRUE;
		}
	}
    return FALSE;
}
