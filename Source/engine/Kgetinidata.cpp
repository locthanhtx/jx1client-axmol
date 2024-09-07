//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCodec.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Data Compress / Decompress Base Class
//---------------------------------------------------------------------------
#include "gamecore/KCore.h"
#include "KMemBase.h"
#include "KEngine.h"
#include "Kgetinidata.h"
#include "dataChecksum.h"
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//#define APP_STRINFO_1 "vvlmXQy0KkYOvN9gEi0IW4kkp602Lym0"
//#define APP_STRINFO_2 "IzOINKf_Rz1T0Lvdf106klw0HxpZ4pWh"
//#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
//#endif
KImageStore2 m_ImageStore; //ȫ�ֵ���

#define GET_SPR_PALETTE(pHeader) ( ((char*)pHeader) + sizeof(SPRHEAD))

Kgetinidata::Kgetinidata(void)
{
	nFrameData = NULL;
	pHeader=NULL;
	m_Palette=NULL;
	m_pPal16=NULL;
	m_pOffset=NULL;
	m_pSprite=NULL;
	m_nWidth=0;
	m_nHeight=0;
	m_nCenterX=0;
	m_nCenterY=0;
	m_nOffsetX=0;
	m_nOffsetY=0;
	m_nFrames=0;
	m_nColors=0;
	m_nDirections=1;
	m_nInterval=10;
	data = NULL;
	m_size = 0;
	//memset(pPalette,0,sizeof(pPalette));
	//m_ImageStore.Init(); //��ʼ���������
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	m_ReadModel = 1; //����
#else
	m_ReadModel = 1; //win32 ·����ȡ
#endif
}

//---------------------------------------------------------------------------
// ����:	GetPackLen
// ����:	ȡ��ѹ������������С����
// ����:	dwDataLen	��ѹ�����ݳ���
// ����:	ѹ������������С����(in Bytes)
// ע��:	�п���ѹ��������ݱ�Դ���ݶ�, ������Ҫ�õ��������������С����
//---------------------------------------------------------------------------
unsigned int Kgetinidata::GetLen(unsigned int dwDataLen)
{
	return (dwDataLen + dwDataLen / 10 + 1024);
}
//---------------------------------------------------------------------------
// ����:	Encode
// ����:	ѹ��
// ����:	pCodeInfo	ѹ�����ݽṹ
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
/*bool Kgetinidata::SprEncode(TSprCodeInfo* pCodeInfo)
{
	//g_MemCopyMmx(pCodeInfo->lpPack, pCodeInfo->lpData, pCodeInfo->dwDataLen);
	//pCodeInfo->dwPackLen = pCodeInfo->dwDataLen;
	return TRUE;
}
*/
/*
bool Kgetinidata::SaveBMPFile(const char *filename,  const stImageInfo& m_info)
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bi;
	bi.biSize   = sizeof(BITMAPINFOHEADER);
	bi.biWidth  = m_info.width;
	bi.biHeight = m_info.height;
	bi.biPlanes = 1;
	bi.biBitCount  = m_info.bytes_per_pixel * 8;
	bi.biCompression = 0;
	bi.biSizeImage   = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed       = 0;
	bi.biClrImportant  = 0;

	bfh.bfType=0x4d42;
	bfh.bfOffBits=sizeof(bfh)+sizeof(BITMAPINFOHEADER);
	bfh.bfSize=bfh.bfOffBits+m_info.width*m_info.height*3;
	bfh.bfReserved1=0;
	bfh.bfReserved2=0;
	FILE * file;
	//CFile file;
	file = fopen(filename, "wb+");

	if (file == NULL)
	{
		return FALSE;
	}
	//if(!file.Open(filename,CFile::modeCreate|CFile::modeWrite))
	//	return -1;
	fwrite(&bfh, 1, sizeof(bfh), file);
	fwrite(&bi, 1, sizeof(bi), file);
	fwrite(m_info.buffer,1,m_info.width*m_info.height*3,file);
	//file.Write(&bfh,sizeof(bfh));
	//file.Write(&bi,sizeof(bi));
	//file.Write(m_info.buffer,m_info.width*m_info.height*3);
	//file.Close();
	fclose((FILE*)file);

	return true;
}
*/
bool Kgetinidata::ReadFileData(char *nFileName,int nFrameIndex)
{
	//��Դ�ļ��е� spr
	 /*if (m_ReadModel)
	 {
		 //g_pPakList->GetSprHeader();
		 //g_pPakList->GetSprFrame()
		 SPROFFS*  pOffsTable = NULL;
		 SPRHEAD*  pSprHeader = NULL;
			 pSprHeader= SprGetHeader(nFileName, pOffsTable);
		 nFrameData = NULL;
		 if (pSprHeader)
		 {
			 if (pOffsTable)	//һ�μ���������sprͼ
			 {
					 if (nFrameIndex >= 0 && nFrameIndex < pSprHeader->Frames)
					 {
						 nFrameData = (SPRFRAME*)((char*)pOffsTable + sizeof(SPROFFS) * pSprHeader->Frames + pOffsTable[nFrameIndex].Offset);
					 }
				 //}
			 }
			 else
			 {
				 unsigned int nFrameSize = 0;
		       nFrameData = SprGetFrame(pSprHeader, nFrameIndex,nFrameSize); //��֡����
			 }
		 }
		 else
		 {
			 messageBox("pSprHeader is NULL","error");
			 return false;
		 }

		 if (nFrameData==NULL)
		 {
		    SprReleaseHeader(pSprHeader);
			messageBox("FrameFiledata is NULL","error");
			return false;
		 }

		 m_nWidth      = pSprHeader->Width;
		 m_nHeight     = pSprHeader->Height;
		 m_nCenterX    = pSprHeader->CenterX;
		 m_nCenterY    = pSprHeader->CenterY;
		 m_nFrames     = pSprHeader->Frames;
		 m_nColors     = pSprHeader->Colors;
		 m_nDirections = pSprHeader->Directions;
		 m_nInterval   = pSprHeader->nInterval;

		 m_pPal24  = (KPAL24*)GET_SPR_PALETTE(pSprHeader);//(&pSprHeader[1]);
		 m_pOffset = pOffsTable;
		 //m_pSprite = (LPBYTE)(m_pOffset + sizeof(SPROFFS) * pSprHeader->Frames);
		 for(int i=0;i<m_nColors;i++)
		 {
			 pPalette[i].Red   = m_pPal24->Red;
			 pPalette[i].Green = m_pPal24->Green;
			 pPalette[i].Blue  = m_pPal24->Blue;
			 //pPalette[i]
			 m_pPal24++;
		 }
		 SprReleaseHeader(pSprHeader);
		 return true;
	 }
	 else
	   data = ax::FileUtils::getInstance()->getFileData(nFileName, "rb", &m_size);
	//��ʼת����png ����

	if (!data)
	{
		messageBox("ReadFiledata is NULL","error");
		return false;
	}

	pHeader = (SPRHEAD*)data;
	//get sprite info   ��ȡspr����Ϣ
	m_nWidth      = pHeader->Width;
	m_nHeight     = pHeader->Height;
	m_nCenterX    = pHeader->CenterX;
	m_nCenterY    = pHeader->CenterY;
	m_nFrames     = pHeader->Frames;
	m_nColors     = pHeader->Colors;
	m_nDirections = pHeader->Directions;
	m_nInterval   = pHeader->nInterval;

	// setup palette pointer
	data += sizeof(SPRHEAD);  //ɫ�߿�ʼ��ָ��
	m_pPal24 = (KPAL24*)data; //24ɫ���ɫ�� ��¼��һ��rgb�ṹ ����

	// setup offset pointer
	data += m_nColors * sizeof(KPAL24);//ÿ֡ƫ�Ƽ�¼�Ŀ�ʼָ��
	m_pOffset = (SPROFFS*)data;

	// setup sprite pointer
	data += m_nFrames * sizeof(SPROFFS);//��֡�� ȫ��λͼ���ݿ�ʼָ��
	m_pSprite = (LPBYTE)data;           //Ϊͼ����
	// make color table
	//m_Palette = new BYTE[m_nColors * sizeof(KPAL16)]; //�յ�ɫ��

	for(int i=0;i<m_nColors;i++)
	{
		pPalette[i].Red   = m_pPal24->Red;
		pPalette[i].Green = m_pPal24->Green;
		pPalette[i].Blue  = m_pPal24->Blue;
		//pPalette[i]
		m_pPal24++;
	}
*/
	return true;
}


int Kgetinidata::GetGreaterNear2Fold(int d)
{
	int fd = 1;
	while(d > fd)
	{
		fd <<= 1;
	}
	return fd;
}

//free in data, return row reverted data.
PBYTE Kgetinidata::RevertRowRGBA(PBYTE data, int &width, int &height)
{
	if(!data)return NULL;
	int nearW = GetGreaterNear2Fold(width);
	int nearH = GetGreaterNear2Fold(height);
	int size = nearW*nearH*4;
	PBYTE toData = (PBYTE)malloc(size);
	if(!toData)
	{
		free(data);
		return NULL;
	}
	memset(toData, 0, size);
	int rowStep   = width*4;
	int toRowStep = nearW*4;
	int i = 0;
	for(int h = height - 1; h >=0; h--)
	{
		int begin = h * width * 4;
		memcpy(toData + i, data+begin, rowStep);
		i += toRowStep;
	}
	free(data);
	width  = nearW;
	height = nearH;
	return toData;
}

Texture2D *Kgetinidata::getpicPath(char *picPath)
{
  if (picPath==NULL)
	  return NULL;

  XPackElemFileRef	m_PackRef;
  ZeroMemory(&m_PackRef,sizeof(XPackElemFileRef));
  Texture2D* nPicTexture =NULL;
  char nSprFileKey[64]={0},m_szEntireMapFile[128];
  sprintf(m_szEntireMapFile, "%s", picPath);
  g_StrLower(m_szEntireMapFile);
  unsigned long nFielpahtdwid = g_FileName2Id(m_szEntireMapFile);
  sprintf(nSprFileKey,"%u-%d",nFielpahtdwid,0);
  nPicTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFileKey);
  int isHave = false;

  if  (nPicTexture)
	  return nPicTexture;

  if (nPicTexture==NULL)
  {
	if (g_pPakList)
		isHave  = g_pPakList->pGetFilePath(m_szEntireMapFile,m_PackRef);
	if (isHave)
	{
		unsigned char * sBuf=NULL;
		sBuf = (unsigned char *)malloc(m_PackRef.nSize+1);
		if (g_pPakList) //��ȡͼƬ������ addImageWihtData
			g_pPakList->ElemFileRead(m_PackRef,sBuf,m_PackRef.nSize);

		if (sBuf)
		{
//			nPicTexture = ax::TextureCache::getInstance()->addImage(nSprFileKey,sBuf,m_PackRef.nSize,0);
            auto image = new ax::Image();
            image->initWithImageData(sBuf, m_PackRef.nSize);
            nPicTexture = ax::Director::getInstance()->getTextureCache()->addImage(image, nSprFileKey);
			free(sBuf);
			sBuf = NULL;
		}
	}
  }
  return nPicTexture;
}

Texture2D *Kgetinidata::getinidata_one(char *nFileName,int nFrameIndex,int *mCurwidth, int *mCurheight,int *mFrams,SPRFRAMSINFO *nSprInfo)
{

	if  (!nFileName || !nFileName[0])
		return NULL;

	if (!strstr(nFileName,".spr"))
		return NULL;

	SPROFFS*  pOffsTable = NULL;
	SPRHEAD*  pSprHeader = NULL;
	SPRFRAME* pFrameData = NULL;
	Texture2D* nTextureOld=NULL;
	unsigned int nfleddwid = 0;
	short nImagePosition   = -1;
	int bSingleFrameLoad = false; //�Ƿ����֡����
	unsigned int nSingFrameSize = 0;
	//int nFarmIndex = 0;           //0Ϊ
	//��ȡͼƬ����Ϣ
	pSprHeader = (SPRHEAD*)m_ImageStore.GetImage(nFileName,nfleddwid,nImagePosition,nFrameIndex,ISI_T_SPR,(void*&)pFrameData,(void*&)pOffsTable,bSingleFrameLoad,nSingFrameSize);
	if (pSprHeader == NULL || pFrameData==NULL)
	{
		//messageBox(nFileName,"SprHeader is null");
		return NULL;
	}

	if  (!bSingleFrameLoad && pOffsTable==NULL)
	{
		//messageBox(nFileName,"OffsTable is null");
		return NULL;
	}

	if (nSprInfo)
	{
		nSprInfo->mFrams    = pSprHeader->Frames;
		nSprInfo->pHwidth   = pSprHeader->Width;
		nSprInfo->pHheight  = pSprHeader->Height;
		nSprInfo->pHCenterX = pSprHeader->CenterX;
		nSprInfo->pHCenterY = pSprHeader->CenterY;
		nSprInfo->nInterval = pSprHeader->nInterval;
		nSprInfo->pFwidth   = pFrameData->Width;
		nSprInfo->pFheight	= pFrameData->Height;
		nSprInfo->pFoffsetX	= pFrameData->OffsetX;
		nSprInfo->pFoffsetY = pFrameData->OffsetY;
	}
	*mFrams             = pSprHeader->Frames;
	char nSprFilePath[64]={0};
	unsigned long nFielpahtdwid = g_FileName2Id(nFileName);
	sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,nFrameIndex);
	if (nTextureOld = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))
	{//����д��� ��ֱ�ӷ�����
		if(mCurwidth)
		{
			*mCurwidth =(int)pFrameData->Width;//pSprHeader->Width;
		}
		if(mCurheight)
		{
			*mCurheight=(int)pFrameData->Height;//pSprHeader->Height;
		}

		return nTextureOld;
	}

	KPAL24* m_pPal24=NULL;
	Palette_Colour pPalette[256];
	memset(pPalette,0,sizeof(pPalette));
	if (m_ReadModel)
	{
		 m_pPal24      = (KPAL24*)GET_SPR_PALETTE(pSprHeader);//(&pSprHeader[1]);
		 //m_pOffset     = pOffsTable;
		 for(int i=0;i<pSprHeader->Colors;i++)
		 {
			 pPalette[i].Red   = m_pPal24->Red;
			 pPalette[i].Green = m_pPal24->Green;
			 pPalette[i].Blue  = m_pPal24->Blue;
			 //pPalette[i]
			 m_pPal24++;
		 }
	 }

	SPRFRAME* pFrame = NULL;
	unsigned char* decdata = NULL;

	if (!bSingleFrameLoad && !pOffsTable)
	{
		//messageBox("m_pOffset is Error","m_pOffset");
		return NULL;
	}

	//char nRoleInfo[256]={0};
	if (!bSingleFrameLoad)
	    pFrame       = (SPRFRAME*)pFrameData;//(SPRFRAME*)((char*)pOffsTable + sizeof(SPROFFS)*pSprHeader->Frames + pOffsTable[nFrameIndex].Offset);
	else
        pFrame       = (SPRFRAME*)pFrameData;//(SPRFRAME*)m_ImageStore.GetSprFrameData(nFileName,nfleddwid,nImagePosition,nFrameIndex,nSingFrameSize);

	if  (pFrame==NULL)
	{
		return NULL;
	}

	BYTE *pSrc	      = pFrame->Sprite; //ԭͼѹ������ //m_pOffset[0].Length;
	int height       = pFrame->Height;
	int width        = pFrame->Width;
	//unsigned short* mPalette  =(unsigned short*)m_Palette;//(unsigned short*)GetPalette();��ԭʼɫ Ϊ��24ɫ
	unsigned int datalength = 0;

	if (bSingleFrameLoad)
     //����֡
		datalength = nSingFrameSize; //����֡�ĳ���
	else
	    datalength = pOffsTable[nFrameIndex].Length;

	unsigned int curdecposition = 0;
	int decdatalen  = width * height*4;
	 decdata = (unsigned char*)malloc((size_t)decdatalen);
	if (!decdata)
	{
		//messageBox("men Error","for");
		return NULL;
	}

	for(int datidx = 0; datidx < decdatalen;)
	{
		decdata[datidx++] = 0x00;
		decdata[datidx++] = 0x00;
		decdata[datidx++] = 0x00;
		decdata[datidx++] = 0x00;
	}
	//��ʼ����
	int n=0,calpha;
	unsigned char alpha;
	unsigned int temppos = 0;

#ifndef WIN32
	dataChecksum nappInfo;
	char nstrps[64]={0},destStr[128];
	ZeroMemory(destStr,sizeof(destStr));
	ZeroMemory(nstrps,sizeof(nstrps));
	//njjj.SimplyDecrypt(bstrmak,nstrmak);
	nappInfo.SimplyDecrypt(nstrps,APP_STRINFO_1);
	strcat(destStr,nstrps);
	ZeroMemory(nstrps,sizeof(nstrps));
	nappInfo.SimplyDecrypt(nstrps,APP_STRINFO_2);
	strcat(destStr,nstrps);
//	if (strcmp(m_MobileKey.c_str(),destStr)!= 0)
//		goto _Return;
#endif

	for(unsigned int i = 0; i < datalength - 8;)
	{
		if(curdecposition > (unsigned int)decdatalen)
		{
			break;
		}
		if (i == 0)
		{//һ��ͼ��������
			n      = *pSrc++; //��ǰ�е�ԭʼ����ָ��λ��
			calpha = *pSrc++; //rgba
		}

		if(calpha == (unsigned char)0x00)
		{
			temppos = n;
			for(unsigned char j = 0; j < temppos; j++)
			{
				decdata[curdecposition++] = 0x0;
				decdata[curdecposition++] = 0x0;
				decdata[curdecposition++] = 0x0;
				decdata[curdecposition++] = 0x0;
			}
		}
		else
		{
			temppos = n;
			alpha   = calpha;
			for(unsigned char m = 0; m < temppos; m++)
			{
				int nTemp = *pSrc++;
				i++;
				decdata[curdecposition++] = pPalette[nTemp].Red;
				decdata[curdecposition++] = pPalette[nTemp].Green;
				decdata[curdecposition++] = pPalette[nTemp].Blue;
				decdata[curdecposition++] = alpha;
			}
		}
		n     =*pSrc++;
		calpha=*pSrc++;
		i += 2;
	}

	if (decdata){
//        nTextureOld = ax::TextureCache::getInstance()->addImage(nSprFilePath,decdata,width,height,ax::Size((float)(width), (float)(height)));
        auto image = new ax::Image();
        image->initWithRawData(decdata, width * height * 4, width, height, 4, false);
        nTextureOld = ax::Director::getInstance()->getTextureCache()->addImage(image, nSprFilePath);
    }


	if(mCurwidth)
	{
		*mCurwidth = (int)width;
	}
	if(mCurheight)
	{
		*mCurheight = (int)height;
	}

	if  (decdata)
	{
		free(decdata);
		decdata =NULL;
	}
	//sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,nFrameIndex);
_Return:
	return nTextureOld;//ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
}

Texture2D *Kgetinidata::getinidata_new(char *nFileName,int nFrameIndex,int *mCurwidth, int *mCurheight,int *mFrams)
//void Kgetinidata::RenderToSprDecode(unsigned short* m_pDest, BYTE* m_pSrc, int m_width, int m_height, BYTE* m_pPalette,unsigned int nPitch,int m_Mask32)
{
	if  (!nFileName || !nFileName[0])
		return NULL;

	if (!strstr(nFileName,".spr"))
		return NULL;

	SPROFFS*  pOffsTable = NULL;
	SPRHEAD*  pSprHeader = NULL;
	SPRFRAME* pFrameData = NULL;
	Texture2D* nTextureOld=NULL;
	unsigned int nfleddwid = 0;
	short nImagePosition   = -1;
	int bSingleFrameLoad = false; //�Ƿ����֡����
	unsigned int nSingFrameSize = 0;
	int nFarmIndex = 0;           //0Ϊ
	//��ȡͼƬ����Ϣ
	pSprHeader = (SPRHEAD*)m_ImageStore.GetImage(nFileName,nfleddwid,nImagePosition,nFarmIndex,ISI_T_SPR,(void*&)pFrameData,(void*&)pOffsTable,bSingleFrameLoad,nSingFrameSize);
	if (pSprHeader == NULL || pFrameData==NULL)
	{
		//messageBox(nFileName,"SprHeader is null");
		return NULL;
	}

	if  (!bSingleFrameLoad && pOffsTable==NULL)
	{
		//messageBox(nFileName,"OffsTable is null");
		return NULL;
	}

	*mFrams = pSprHeader->Frames;
	char nSprFilePath[64]={0};
	unsigned long nFielpahtdwid = g_FileName2Id(nFileName);
	sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
	if (nTextureOld = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))
	{//����д��� ��ֱ�ӷ�����
		if(mCurwidth)
		{
			*mCurwidth =(unsigned short)pSprHeader->Width;
		}
		if(mCurheight)
		{
			*mCurheight=(unsigned short)pSprHeader->Height;
		}

		return nTextureOld;
	}
	KPAL24* m_pPal24=NULL;
	Palette_Colour pPalette[256];
	memset(pPalette,0,sizeof(pPalette));
	if (m_ReadModel)
	{
		 m_pPal24      = (KPAL24*)GET_SPR_PALETTE(pSprHeader);//(&pSprHeader[1]);
		 //m_pOffset     = pOffsTable;
		 for(int i=0;i<pSprHeader->Colors;i++)
		 {
			 pPalette[i].Red   = m_pPal24->Red;
			 pPalette[i].Green = m_pPal24->Green;
			 pPalette[i].Blue  = m_pPal24->Blue;
			 //pPalette[i]
			 m_pPal24++;
		 }
	 }

	SPRFRAME* pFrame = NULL;
	unsigned char* decdata = NULL;

#ifndef WIN32
	dataChecksum nappInfo;
	char nstrps[64]={0},destStr[128];
	ZeroMemory(destStr,sizeof(destStr));
	ZeroMemory(nstrps,sizeof(nstrps));
	//njjj.SimplyDecrypt(bstrmak,nstrmak);
	nappInfo.SimplyDecrypt(nstrps,APP_STRINFO_1);
	strcat(destStr,nstrps);
	ZeroMemory(nstrps,sizeof(nstrps));
	nappInfo.SimplyDecrypt(nstrps,APP_STRINFO_2);
	strcat(destStr,nstrps);
//	if (strcmp(m_MobileKey.c_str(),destStr)!= 0)
//		return NULL;
#endif

 for (int k=0;k<pSprHeader->Frames+1;k++)
 {//96
	//nImagePosition = -1;

	if (k>=pSprHeader->Frames && decdata)
	{
	    free (decdata);  //ɾ�����һ��
	    decdata =NULL;
	    break;
     }

	if (decdata)
	{
		free (decdata);
		decdata =NULL;
	}

	if (!bSingleFrameLoad && !pOffsTable)
	{
		//messageBox("m_pOffset is Error","m_pOffset");
		break;
	}

	//char nRoleInfo[256]={0};
	if (!bSingleFrameLoad)
	    pFrame       = (SPRFRAME*)((char*)pOffsTable + sizeof(SPROFFS)*pSprHeader->Frames + pOffsTable[k].Offset);
	else
        pFrame       = (SPRFRAME*)m_ImageStore.GetSprFrameData(nFileName,nfleddwid,nImagePosition,k,nSingFrameSize);

	if  (pFrame==NULL)
	{
		continue;
	}

	sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,k);
	if (ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath))
		continue;

	BYTE *pSrc	      = pFrame->Sprite; //ԭͼѹ������ //m_pOffset[0].Length;
	int height       = pFrame->Height;
	int width        = pFrame->Width;
	//unsigned short* mPalette  =(unsigned short*)m_Palette;//(unsigned short*)GetPalette();��ԭʼɫ Ϊ��24ɫ
	unsigned int datalength = 0;

	if (bSingleFrameLoad)
     //����֡ ��1 ��ʼ û�е�0֡
		datalength = nSingFrameSize; //����֡�ĳ���
	else
	    datalength = pOffsTable[k].Length;

	unsigned int curdecposition = 0;
	int decdatalen  = width * height*4;
	 decdata = (unsigned char*)malloc((size_t)decdatalen);
	if (!decdata)
	{
		//messageBox("men Error","for");
		break;
	}

	for(int datidx = 0; datidx < decdatalen;)
	{
		decdata[datidx++] = 0x00;
		decdata[datidx++] = 0x00;
		decdata[datidx++] = 0x00;
		decdata[datidx++] = 0x00;
	}
	//��ʼ����
	int n=0,calpha;
	unsigned char alpha;
	unsigned int temppos = 0;
	for(unsigned int i = 0; i < datalength - 8;)
	{
		if(curdecposition > (unsigned int)decdatalen)
		{
			break;
		}
		if (i == 0)
		{//һ��ͼ��������
			n      = *pSrc++; //��ǰ�е�ԭʼ����ָ��λ��
			calpha = *pSrc++; //rgba
		}

		if(calpha == (unsigned char)0x00)
		{
			temppos = n;
			for(unsigned char j = 0; j < temppos; j++)
			{
				decdata[curdecposition++] = 0x0;
				decdata[curdecposition++] = 0x0;
				decdata[curdecposition++] = 0x0;
				decdata[curdecposition++] = 0x0;
			}
		}
		else
		{
			temppos = n;
			alpha   = calpha;
			for(unsigned char m = 0; m < temppos; m++)
			{
				/*if(file.fail())
				{
					file.close();
					free(decdata);
					return NULL;
				}*/
				//file.read((char*)temp,1);
				int nTemp = *pSrc++;
				//*pSrc++;
				i++;
				decdata[curdecposition++] = pPalette[nTemp].Red;
				decdata[curdecposition++] = pPalette[nTemp].Green;
				decdata[curdecposition++] = pPalette[nTemp].Blue;
				decdata[curdecposition++] = alpha;
			}
		}
		//file.read((char*)temp,2);
		n     =*pSrc++;
		calpha=*pSrc++;
		i += 2;
	}

	if (decdata) {
        ax::Image* image = new ax::Image();
        image->initWithRawData(decdata, decdatalen, width, height, 4);
        ax::Director::getInstance()->getTextureCache()->addImage(image, nSprFilePath);
        image->release();
    }
//	  ax::TextureCache::getInstance()->addImage(nSprFilePath,decdata,width,height,ax::Size((float)(width), (float)(height)));

	if(mCurwidth)
	{
		*mCurwidth = (int)width;
	}
	if(mCurheight)
	{
		*mCurheight = (int)height;
	}
/*#ifndef WIN32
	usleep(1000); //1����
#endif*/
  }//end for

//if (pSprHeader) //һ���� ȫ��spr�����ͷ�
	//SprReleaseHeader(pSprHeader);


//����֡�����ͷš�������
//SprReleaseFrame((SPRFRAME*)ImgObject.pFrames[nFrame].pFrameData);
	//PBYTE rData = NULL;
	/*if(decdata)
	{//������ int fcw, fch;
		rData = RevertRowRGBA(decdata,width,height); //�ó����յĸ߶ȿ��

		if(mCurwidth)
		{
			*mCurwidth = (int)width;
		}
		if(mCurheight)
		{
			*mCurheight = (int)height;
		}
	}*/
	sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
	return ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFilePath);
}
//---------------------------------------------------------------------------
// ����:	Decode
// ����:	��ѹ��
// ����:	pCodeInfo	ѹ�����ݽṹ
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
/*bool Kgetinidata::SprDecode(TSprCodeInfo* pCodeInfo)
{
	//g_MemCopyMmx(pCodeInfo->lpData, pCodeInfo->lpPack, pCodeInfo->dwPackLen);
	//pCodeInfo->dwDataLen = pCodeInfo->dwPackLen;
	return TRUE;
}*/
//---------------------------------------------------------------------------
// ����:	g_InitCodec
// ����:	��ʼ�����롢������
// ����:	ppCodec				���롢������ָ��
//			nCompressMethod		ѹ������
// ����:	void
//---------------------------------------------------------------------------
/*void g_InitCodec(KCodec** ppCodec, int nCompressMethod)
{
	*ppCodec = NULL;

	switch (nCompressMethod)
	{
	case CODEC_NONE:
		*ppCodec = new KCodec;
		break;
//
	case CODEC_LHA:
		*ppCodec = new KCodecLha;
		break;

	case CODEC_LZO:
		*ppCodec = new KCodecLzo;
		break;

	}
}
//---------------------------------------------------------------------------
// ����:	g_FreeCodec
// ����:	�ͷű��롢������
// ����:	ppCodec				���롢������ָ��
//			nCompressMethod		ѹ������
// ����:	void
//---------------------------------------------------------------------------
void g_FreeCodec(KCodec** ppCodec, int nCompressMethod)
{
	if (*ppCodec == NULL)
		return;

	switch (nCompressMethod)
	{
	case CODEC_NONE:
		{
		  delete (KCodec *)*ppCodec;
		  *ppCodec=NULL;
		}
		break;
//
//	case CODEC_LHA:
//		delete (KCodecLha *)*ppCodec;
//		break;

	case CODEC_LZO:
		{
        	delete (KCodecLzo *)*ppCodec;
			*ppCodec=NULL;
		}
		break;

	}
	*ppCodec = NULL;
}*/
//---------------------------------------------------------------------------

