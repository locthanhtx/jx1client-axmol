/*****************************************************************************************
//  ����ͼ��ͼ����Դ����
//	Copyright : Kingsoft 2002-2003
//	Author	: Wooy(Wu yue)
//	CreateTime:	2002-11-11
*****************************************************************************************/
#include "engine/KEngine.h"
#include "KImageStore2.h"
//#include "../iRepresent/KRepresentUnit.h"
//#include <crtdbg.h.>
//#include "../../Engine/Src/KColors.h"

//�ļ���ת��Ϊ�ַ���
uint32_t KImageStore2::ImageNameToId(char* pszName)
{
	//															-������....,ż��ʵ��ί�۸�-
	uint32_t uId = 0;
	if (pszName && pszName[0])
	{
		for (int i = 1; (*pszName); pszName++)
		{
			if ((*pszName) < 'A' || (*pszName) > 'Z')
				uId = (uId + i * (*pszName)) % 0x8000000b * 0xffffffef;
			else						//�����ַ��ĺ��ֽ�����ܱ�ת������Ϊ�ؼӴ�������ĸ��ʡ�
				uId = (uId + i * ((*pszName) + 0x20)) % 0x8000000b * 0xffffffef;
		}
        return (uint32_t)(uId^0x12345678);

//        int32_t len = strlen(pszName);
//        uint32_t dwID = 0;
//        for (int32_t i = 0;i<len; ++i) //lpFileName[i]
//        {
//            dwID =(dwID + (i + 1) * pszName[i])%0x8000000B*0xFFFFFFEF;//0x8000000B*0xFFFFFFEF;
//        }
//        return (uint32_t)(dwID^0x12345678);
    }
	return uId;
}
//bmp
/*
uint32_t KImageStore2::CreateImage(const char* pszName, int nWidth, int nHeight, int nType)
{
	uint32_t uImage = ImageNameToId(pszName);
	KSGImageContent* pBitmap = NULL;
	int nIdx;


    KAutoCriticalSection AutoLock(m_ImageProcessLock);

	if (nWidth > 0 && nHeight > 0 && uImage && nType == ISI_T_BITMAP16)
	{
		if ((nIdx = FindImage(uImage, 0)) < 0 &&	            //�����ǲ�����ͬid��ͼ��
			(m_nNumImages < m_nNumReserved || ExpandSpace()))	//�пռ���ͼ�ζ���
		{
			pBitmap = (KSGImageContent *)malloc(KSG_IMAGE_CONTENT_SIZE(nWidth, nHeight));
		}
	}
	
	if (pBitmap)
	{
		pBitmap->nWidth = nWidth;
		pBitmap->nHeight = nHeight;

		nIdx = - nIdx - 1;
		for (int i = m_nNumImages; i > nIdx; i--)
		{
			m_pObjectList[i] = m_pObjectList[i - 1];
		}
		m_pObjectList[nIdx].bNotCacheable = true;
		m_pObjectList[nIdx].bSingleFrameLoad = false;
		m_pObjectList[nIdx].bType = ISI_T_BITMAP16;
		m_pObjectList[nIdx].pObject = pBitmap;
		m_pObjectList[nIdx].pFrames = NULL;
		m_pObjectList[nIdx].uId = uImage;
		m_pObjectList[nIdx].pSurface = NULL;
		m_nNumImages++;
	}
	else
		uImage = 0;
	return uImage;
}
*/
void KImageStore2::Free()
{
    KAutoCriticalSection AutoLock(m_ImageProcessLock);

	if (m_pObjectList)
	{
        for (int i = 0; i < m_nNumImages; i++)
			FreeImageObject(m_pObjectList[i]);

		free(m_pObjectList);
		m_pObjectList = NULL;
	}
	m_nNumImages   = 0;
	m_nNumReserved = 0;

	m_uImageAccessCounter = 0;

	ax::Director::getInstance()->getTextureCache()->removeUnusedTextures();

}

void KImageStore2::FreeImage(char* pszImage)
{//�ͷ�ĳ��ͼƬ
    KAutoCriticalSection AutoLock(m_ImageProcessLock);

	uint32_t uImage = ImageNameToId(pszImage);
	int nIdx = FindImage(uImage, 0);
	if (nIdx >= 0)
	{
		FreeImageObject(m_pObjectList[nIdx]);
		m_nNumImages--;
		for (int i = nIdx; i < m_nNumImages; i++)
			m_pObjectList[i] = m_pObjectList[i + 1];
	}

	ax::Director::getInstance()->getTextureCache()->removeUnusedTextures();
}
/*
void* KImageStore2::GetExistedCreateBitmap(const char* pszImage, uint32_t uImage, short& nImagePosition)
{
	if (uImage == 0)
		uImage = ImageNameToId(pszImage);

    KAutoCriticalSection AutoLock(m_ImageProcessLock);

	nImagePosition = FindImage(uImage, nImagePosition);
	void* pObject = NULL;
	if (nImagePosition >= 0 &&
		m_pObjectList[nImagePosition].bType == ISI_T_BITMAP16 &&
		m_pObjectList[nImagePosition].bNotCacheable == true &&
		m_pObjectList[nImagePosition].pObject)
	{
		pObject = (m_pObjectList[nImagePosition].pObject);
	}
	return pObject;
}
*/

void* KImageStore2::GetSprFrameData(char* pszImage, uint32_t& uImage,
	short& nImagePosition, int nFrame,uint32_t& nSingFrameSize)
{
	if  (!pszImage)
		return NULL;

	void* pFrameData = NULL;

	if (uImage == 0)
	{
		if ((uImage = ImageNameToId(pszImage)) == 0)
			return NULL;
	}
    KAutoCriticalSection AutoLock(m_ImageProcessLock);
	if ((nImagePosition = FindImage(uImage, nImagePosition)) >= 0)
	{//�Ƿ���� �������
		_KISImageObj&	ImgObj = m_pObjectList[nImagePosition];
			if (ImgObj.pObject && ImgObj.bType == ISI_T_SPR)
			{
				/*pObject        = ImgObj.pObject;
				pOffsetTable     = ImgObj.pFrames->pOffsetTable;
				bSingleFrameLoad = ImgObj.bSingleFrameLoad;*/
			    //��ȡĳ֡������,�Զ����뻺��
				pFrameData = GetSprFrame(pszImage, ImgObj, nFrame,nSingFrameSize);
			}
	}
	return pFrameData;
}
//spr����
void* KImageStore2::GetImage(char* pszImage, uint32_t& uImage,
					short& nImagePosition, int nFrame, int nType, void*& pFrameData,void*&pOffsetTable,int& bSingleFrameLoad,uint32_t&nSingFrameSize)
{
	if  (!pszImage)
		return NULL;

	pFrameData = NULL;

	if (uImage == 0)
	{
		if ((uImage = ImageNameToId(pszImage)) == 0)
			return NULL;
	}

    KAutoCriticalSection AutoLock(m_ImageProcessLock);
	
	void* pObject = NULL;
	if ((nImagePosition = FindImage(uImage, nImagePosition)) >= 0)
	{//�Ƿ���� �������
		_KISImageObj&	ImgObj = m_pObjectList[nImagePosition];
		if (ImgObj.bType == (unsigned char)nType)
		{
			if (ImgObj.pObject && ImgObj.bType == ISI_T_SPR)
			{
				pObject          = ImgObj.pObject;
				pOffsetTable     = ImgObj.pFrames->pOffsetTable;
				bSingleFrameLoad = ImgObj.bSingleFrameLoad;
			    //��ȡĳ֡������
				//uint32_t nFramesize =0;
				if ((pFrameData = GetSprFrame(pszImage, ImgObj, nFrame,nSingFrameSize)) == NULL)
					pObject = NULL;
			}
			ImgObj.bRef = true;
		}
		//else ��ȥͬid���ǲ�ͬͼ����������Ĵ�����Ϊ�����������
		//Ϊ��ִ��Ч������δ��������жϷ���CheckBalance���������档��ͬ��
		if (m_nNumImages > m_nBalanceNum && (++m_uImageAccessCounter) > m_uCheckPoint)
			CheckBalance();

		return pObject;
	}

	// ��������ڻ����� �����¼��� �пռ�
	if (m_nNumImages < m_nNumReserved || ExpandSpace())
	{
		nImagePosition = - nImagePosition - 1;	// FindImageʱ�Ѿ��Һ�λ����
		for (int i = m_nNumImages; i > nImagePosition; i--)
		{//������������
			m_pObjectList[i] = m_pObjectList[i - 1];
		}
		
		_KISImageObj &ImgObj = m_pObjectList[nImagePosition];	
		
		ImgObj.bNotCacheable = false;
		ImgObj.bRef = true;
		ImgObj.bSingleFrameLoad = false;  //�Ƿ����֡�� ����
		ImgObj.bType = (unsigned char)nType;
		ImgObj.pFrames = NULL;
		ImgObj.pObject = NULL;
		ImgObj.uId = uImage;
		//ImgObj.pcAdjustColorPalettes = NULL;
		pObject      = LoadImage(pszImage, ImgObj, nFrame, pFrameData,nSingFrameSize);
		if (pObject)
		{
			pOffsetTable     = ImgObj.pFrames->pOffsetTable;
			bSingleFrameLoad = ImgObj.bSingleFrameLoad;
			m_nNumImages++;  //ͼƬ�ļ���+1
			//���ͼƬ����������486 �� ���ͷ��ڴ�
			if (m_nNumImages > m_nBalanceNum && (++m_uImageAccessCounter) > m_uCheckPoint)
				CheckBalance();
		}

	}
	return pObject;
}

void* KImageStore2::GetSprFrame(char* pszImageFile, _KISImageObj& ImgObject, int nFrame,uint32_t& nSingFrameSize)
{
	if  (!pszImageFile)
		return NULL;

	SPRHEAD* pSprHeader = (SPRHEAD*)ImgObject.pObject;
	void* pFrameData = NULL;
	if (!pSprHeader)
		return NULL;
	//_ASSERT(pszImageFile && pSprHeader);
	if (nFrame >= 0 && nFrame < pSprHeader->Frames)
	{
		_KISImageFrameObj * pFrame=NULL;
		if (ImgObject.bSingleFrameLoad == false)
		{//���Ƕ��������
			pFrame = ImgObject.pFrames;
			//_ASSERT(pFrame);
			//_ASSERT(pFrame->pOffsetTable);
			if (pFrame && pFrame->pOffsetTable)
			{
				pFrameData = (((char*)pFrame->pOffsetTable) + pFrame->sOffTableSize +
					((SPROFFS*)pFrame->pOffsetTable)[nFrame].Offset);
				nSingFrameSize = ((SPROFFS*)pFrame->pOffsetTable)[nFrame].Length;
				pFrame->bRef = true;
			}
		}
		else
		{//
			pFrame = &ImgObject.pFrames[nFrame];
			if (pFrame)
			{
				pFrame->bRef    = true;
				nSingFrameSize  = pFrame->pFrameSize;
				if ((pFrameData = pFrame->pFrameData) == NULL)
				{	//ָ����֡���ݻ�������
					/*char nRoleInfo[128]={0};
					sprintf(nRoleInfo,"��:%d index:%d",pSprHeader->Frames,nFrame);
					ccMessageBox(nRoleInfo,"����֡������");*/
					uint32_t nFrameSize = 0;
					pFrame->pFrameData = SprGetFrame((SPRHEAD*)ImgObject.pObject,nFrame,nFrameSize);
					pFrameData         = pFrame->pFrameData;
					nSingFrameSize     = nFrameSize;
					pFrame->pFrameSize = nFrameSize;
				}
			}
		}
	}
	return pFrameData;
}
//��ȡ����spr��������
bool KImageStore2::GetImageParam(char* pszImage, int nType, KImageParam* pImageData)
{
	uint32_t uImage = ImageNameToId(pszImage);
	
	if (uImage == 0)
		return false;
    //KAutoCriticalSection AutoLock(m_ImageProcessLock);
	void*  pOffsTable = NULL;
	int    bSingleFrameLoad = false;
	uint32_t    bSingleFrameSize=0;
	short nImagePosition = FindImage(uImage, -1);

	if (nImagePosition < 0)
	{//���û��
		void* pFrameData;
		if (GetImage(pszImage,uImage,nImagePosition,0,nType,pFrameData,pOffsTable,bSingleFrameLoad,bSingleFrameSize) == NULL)
			nImagePosition = -1;
	}

	if (nImagePosition >= 0)
	{//����Ѿ�������
		_KISImageObj&	ImgObj = m_pObjectList[nImagePosition];
		if (ImgObj.pObject && ImgObj.bType == (unsigned char)nType)
		{
			if (pImageData && ImgObj.bType == ISI_T_SPR)
			{
					SPRHEAD* pSprHeader         = (SPRHEAD*)ImgObj.pObject;
					pImageData->nWidth          = pSprHeader->Width;
					pImageData->nHeight         = pSprHeader->Height;
					pImageData->nInterval       = pSprHeader->nInterval;
					pImageData->nNumFrames      = pSprHeader->Frames;
					pImageData->nNumFramesGroup = pSprHeader->Directions;
					pImageData->nReferenceSpotX = pSprHeader->CenterX;
					pImageData->nReferenceSpotY = pSprHeader->CenterY;				
			}
			return true;
		}
	}
	return false;
}
//ĳ֡ ������ 
bool KImageStore2::GetImageFrameParam(char* pszImage, int nType,
		int nFrame, KRPosition2* pOffset, KRPosition2* pSize)
{	
	bool bRet = false;
	short	nPos = -1;
	uint32_t uImage = 0;
	void* pFrame;
	void* pOffsTable = NULL;
	int  bSingleFrameLoad=false;
	uint32_t    bSingleFrameSize=0;
	void* pImage = GetImage(pszImage, uImage, nPos, 0, nType, pFrame,pOffsTable,bSingleFrameLoad,bSingleFrameSize);
	if (pImage)
	{
		if (nType == ISI_T_SPR)
		{
			if (pOffset)
			{
				pOffset->nX = ((SPRFRAME*)pFrame)->OffsetX;
				pOffset->nY = ((SPRFRAME*)pFrame)->OffsetY;
			}
			if (pSize)
			{
				pSize->nX = ((SPRFRAME*)pFrame)->Width;
				pSize->nY = ((SPRFRAME*)pFrame)->Height;
			}
		}
		bRet = true;
	}
	return bRet;
}
/*
//��ȡĳ��� ͨ��
int KImageStore2::GetImagePixelAlpha(const char* pszImage, int nType, int nFrame, int nX, int nY)
{
	int nRet = 0;
	short	nPos = -1;
	uint32_t uImage = 0;
	SPRFRAME* pFrame;
	void* pImage = GetImage(pszImage, uImage, nPos, 0, nType, (void*&)pFrame);
	if (pImage)
	{
		if (nType == ISI_T_SPR)
		{
			nX -= pFrame->OffsetX;
			nY -= pFrame->OffsetY;
			if (nX >= 0  && nX < pFrame->Width && nY >= 0 && nY < pFrame->Height)
			{
				int	nNumPixels = pFrame->Width;
				void*	pSprite =  pFrame->Sprite;
				nY++;
				_asm
				{
					//ʹSDIָ��sprite�е�ͼ������λ��
					mov		esi, pSprite
				dec_line:
					dec		nY				//����һ��
					jz		last_line
					
					mov		edx, nNumPixels
				skip_line:
					movzx	eax, byte ptr[esi]
					inc		esi
					movzx	ebx, byte ptr[esi]
					inc		esi
					or		ebx, ebx
					jz		skip_line_continue
					add		esi, eax
				skip_line_continue:
					sub		edx, eax
					jg		skip_line
					jmp		dec_line

				last_line:
					mov		edx, nX
				last_line_alpha_block:
					movzx	eax, byte ptr[esi]
					inc		esi
					movzx	ebx, byte ptr[esi]
					inc		esi
					or		ebx, ebx
					jz		last_line_continue
					add		esi, eax
				last_line_continue:
					sub		edx, eax
					jg		last_line_alpha_block

					mov		nRet, ebx
				}
			}
		}
		else if (nType == ISI_T_BITMAP16)
		{
			if (nX >= 0 && nY >= 0 &&
				nX < ((KSGImageContent*)pImage)->nWidth &&
				nY < ((KSGImageContent*)pImage)->nHeight)
			{
				nRet = 255;
			}
		}
	}
	return nRet;
}
*/
bool KImageStore2::Init()
{
	Free();
	return ExpandSpace();
}

bool KImageStore2::SaveImage(const char* pszFile, const char* pszImage, int nFileType)
{
	return 0;
}

void KImageStore2::SetBalanceParam(int nNumImage, uint32_t uCheckPoint)
{
	m_nBalanceNum = nNumImage;
	m_uCheckPoint = uCheckPoint;
}

void KImageStore2::CheckBalance()
{
	m_uImageAccessCounter = 0;
	int i, j;
    int nNewNumImages = 0;
#ifdef WIN32
    MEMORYSTATUS MemStatus;
    GlobalMemoryStatus(&MemStatus);
    // ���ʣ��ɷ���������ڴ�������������ڴ��һ������������Ҫ����Cache����
    if (MemStatus.dwAvailPhys > (MemStatus.dwTotalPhys / 32))
    {
        if (m_nNumImages < 1024)    // Cache��ͼƬ�����ޣ�Cache�������1024��ͼƬ
            return;                 // ��������
    }
#else
	if (m_nNumImages < 1024)    // Cache��ͼƬ�����ޣ�Cache�������1024��ͼƬ
		return;                 // ��������
#endif

    KAutoCriticalSection AutoLock(m_ImageProcessLock);
    nNewNumImages = 0;
    for (i = 0; i < m_nNumImages; i++)
    {
        if (m_pObjectList[i].bNotCacheable == false)
        {//�����滺���
			if (m_pObjectList[i].bRef == false)
			{
				FreeImageObject(m_pObjectList[i]);
                continue;
            }

			if (//����֡�� ���ص� �ͷŵ�
                (m_pObjectList[i].bSingleFrameLoad == true) &&
				(m_pObjectList[i].pObject) &&
				(m_pObjectList[i].bType == ISI_T_SPR)
            )
			{
				int nFrameCount = ((SPRHEAD*)(m_pObjectList[i].pObject))->Frames;
				for (j = 0; j < nFrameCount; j++)
				{
					if (m_pObjectList[i].pFrames[j].bRef == false)
					{
						FreeImageObject(m_pObjectList[i],j);
					}
                    else
                    {//����õ��� ����Ϊfalse
                        m_pObjectList[i].pFrames[j].bRef = false;
                    }
				}
			}
        }

        m_pObjectList[nNewNumImages] = m_pObjectList[i];
        m_pObjectList[nNewNumImages].bRef = false;
        nNewNumImages++;
    }
    m_nNumImages =  nNewNumImages;

	ax::Director::getInstance()->getTextureCache()->removeUnusedTextures();

}

bool KImageStore2::ExpandSpace()
{
	_KISImageObj* pNewList = (_KISImageObj *)realloc(m_pObjectList,
						(m_nNumReserved + ISBP_EXPAND_SPACE_STEP) * sizeof(_KISImageObj));
	if (pNewList)
	{
		m_pObjectList = pNewList;
		m_nNumReserved += ISBP_EXPAND_SPACE_STEP;
		return true;
	}
	return false;
}

int KImageStore2::FindImage(uint32_t uImage, int nPossiblePosition)
{
	int nPP = nPossiblePosition;
	if (nPP < 0 || nPP >= m_nNumImages)
	{
		if (m_nNumImages <= 0)
		{
			return -1;
		}
		else
		{
			nPP = m_nNumImages / 2;
		}
	}
	if (m_pObjectList[nPP].uId == uImage)
		return nPP;
	int nFrom, nTo, nTryRange;
	nTryRange = ISBP_TRY_RANGE_DEF;
	if (m_pObjectList[nPP].uId > uImage)
	{
		nFrom = 0;
		nTo = nPP - 1;
		nPP -= nTryRange;
	}
	else
	{
		nFrom = nPP + 1;
		nTo = m_nNumImages - 1;
		nPP += nTryRange;
	}
	if (nFrom + nTryRange >= nTo)
		nPP = (nFrom + nTo) / 2;

	while (nFrom < nTo)
	{
		if (m_pObjectList[nPP].uId < uImage)
		{
			nFrom = nPP + 1;
		}
		else if (m_pObjectList[nPP].uId > uImage)
		{
			nTo = nPP - 1;
		}
		else
		{
			return nPP;
		}
		nPP = (nFrom + nTo) / 2;
	}
	if (nFrom == nTo)
	{
		if (m_pObjectList[nPP].uId > uImage)
		{
			nPP = - nPP - 1;
		}
		else if (m_pObjectList[nPP].uId < uImage)
		{
			nPP = - nPP - 2;
		}
	}
	else
	{
		nPP = - nFrom -1;
	}
	return nPP;
}

void KImageStore2::FreeImageObject(_KISImageObj& ImgObject, int nFrame/*=-1*/)
{
	if(ImgObject.pObject)
	{
		if (ImgObject.bType == ISI_T_SPR)
		{
			if (ImgObject.bSingleFrameLoad == false)
			{
				free (ImgObject.pFrames);
				ImgObject.pFrames = NULL;

				if (ImgObject.pObject)
				{
					SprReleaseHeader((SPRHEAD*)ImgObject.pObject);
					ImgObject.pObject = NULL;
				}
			}
			else
			{//����֡ �ͷ�
				int nNumFrame = ((SPRHEAD*)ImgObject.pObject)->Frames;
				if (nFrame >= 0 && nFrame < nNumFrame)
				{
					if (ImgObject.pFrames[nFrame].pFrameData)
					{
						SprReleaseFrame((SPRFRAME*)ImgObject.pFrames[nFrame].pFrameData);
						ImgObject.pFrames[nFrame].pFrameData = NULL;
					}
				}
				else if (nFrame < 0)
				{//
					for (nFrame = 0; nFrame < nNumFrame; nFrame++)
					{//0-95
						if (ImgObject.pFrames[nFrame].pFrameData)
						{
							SprReleaseFrame((SPRFRAME*)ImgObject.pFrames[nFrame].pFrameData);
							ImgObject.pFrames[nFrame].pFrameData =NULL;
						}
					}
					free (ImgObject.pFrames);
					ImgObject.pFrames = NULL;
					SprReleaseHeader((SPRHEAD*)(ImgObject.pObject));
					ImgObject.pObject = NULL;
				}
			}
		}
		else
			ImgObject.pObject = NULL;
	}
}
//spr����
void* KImageStore2::LoadImage(char* pszImageFile, _KISImageObj& ImgObj, int nFrame, void*& pFrameData,uint32_t& nSingFrameSize)
{
	void* pRet = NULL;
	_KISImageFrameObj* pFrameObj = NULL;
	if (ImgObj.bType == ISI_T_SPR)
	{
		SPROFFS*  pOffsTable = NULL;
		SPRHEAD*  pSprHeader = SprGetHeader(pszImageFile, pOffsTable);
		
		if (pSprHeader)
		{
			if (pOffsTable)	//һ�μ���������sprͼ
			{
				pFrameObj = (_KISImageFrameObj*)malloc(sizeof(_KISImageFrameObj));
				if (pFrameObj)
				{
					memset(pFrameObj, 0, sizeof(_KISImageFrameObj));
					ImgObj.pObject = pSprHeader;
					ImgObj.pFrames = pFrameObj;
					ImgObj.bNotCacheable = false;
					ImgObj.bSingleFrameLoad = false; //�Ƿ����֡����
					pFrameObj->pOffsetTable = pOffsTable;
					pFrameObj->sOffTableSize = sizeof(SPROFFS) * pSprHeader->Frames;
					pFrameObj->bRef = true;

					if (nFrame >= 0 && nFrame < pSprHeader->Frames)
					{//ĳ֡��������
						pFrameData     = ((char*)pOffsTable + pFrameObj->sOffTableSize + pOffsTable[nFrame].Offset);
						nSingFrameSize = pOffsTable[nFrame].Length;
						pRet = pSprHeader;
					}					
				}
			}
			else	//��֡���ص�ͼ
			{
				int nSize = sizeof(_KISImageFrameObj) * pSprHeader->Frames;
				pFrameObj = (_KISImageFrameObj*)malloc(nSize);//ȫ��֡���Ķ�������
				if (pFrameObj)
				{
					memset(pFrameObj, 0, nSize);
					ImgObj.pObject          = pSprHeader;
					ImgObj.pFrames          = pFrameObj;
					ImgObj.bNotCacheable    = false;
					ImgObj.bSingleFrameLoad = true; //�Ƕ���֡���ı����
					if (nFrame >= 0 && nFrame < pSprHeader->Frames)
					{
						uint32_t nFrameSize = 0;
						pFrameData = (SPRFRAME*)SprGetFrame(pSprHeader,nFrame,nFrameSize);
						if (pFrameData)
						{
							ImgObj.pFrames[nFrame].bRef       = true;
							ImgObj.pFrames[nFrame].pFrameData = (SPRFRAME*)pFrameData;
							ImgObj.pFrames[nFrame].pFrameSize = nFrameSize;//��һ֡�ĳ���
							nSingFrameSize                    = nFrameSize;
						    pRet = pSprHeader;
						}
					}
					
				}
			}
			if (!ImgObj.pObject)
			{
				SprReleaseHeader(pSprHeader);
				//ImgObj.pcAdjustColorPalettes = NULL;
			}
		}
	}
	return pRet;
}

KImageStore2::KImageStore2()
{
	m_pObjectList = NULL;
	m_nNumReserved= 0;
	m_nNumImages  = 0;
	m_nBalanceNum = ISBP_BALANCE_NUM_DEF; //484
	m_uCheckPoint = ISBP_CHECK_POINT_DEF; //484
	m_uImageAccessCounter = 0;
	Init();
}

KImageStore2::~KImageStore2()
{
	Free();
}
/*
void KImageStore2::ClearAllAdjustColorPalette()
{
    KAutoCriticalSection AutoLock(m_ImageProcessLock);

	for (int i = 0; i < m_nNumImages; i++)
	{
		if (m_pObjectList[i].pcAdjustColorPalettes)
		{
			free (m_pObjectList[i].pcAdjustColorPalettes);
			m_pObjectList[i].pcAdjustColorPalettes = NULL;
		}
	}
}*/
