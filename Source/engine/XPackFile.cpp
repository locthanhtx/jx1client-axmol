/*****************************************************************************************
//	��ȡ����ļ�
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-9-16
*****************************************************************************************/
#include "XPackFile.h"
#include "stdlib.h"
//#include "file.h"
#include "ucl/ucl.h"
//#include <crtdbg.h>

//һ��Pack�ļ����е�ͷ�ṹ:
/*
struct XPackFileHeader
{
	unsigned char cSignature[4];		//�ĸ��ֽڵ��ļ���ͷ��־���̶�Ϊ�ַ���'PACK'
	uint32_t uCount;				//�ļ����ݵ���Ŀ��
	uint32_t uIndexTableOffset;	//������ƫ����
	uint32_t uDataOffset;			//���ݵ�ƫ����
	uint32_t uCrc32;				//У���
	unsigned char cReserved[12];		//�������ֽ�
};

#define	XPACKFILE_SIGNATURE_FLAG		0x4b434150	//'PACK'

//Pack�ж�Ӧÿ�����ļ���������Ϣ��
struct XPackIndexInfo
{
	uint32_t	uId;				//���ļ�id
	uint32_t	uOffset;			//���ļ��ڰ��е�ƫ��λ��
	int32_t			lSize;				//���ļ���ԭʼ��С
	int32_t			lCompressSizeFlag;	//���ļ�ѹ����Ĵ�С��ѹ������
										//����ֽڱ�ʾѹ����������XPACK_METHOD
										//�͵������ֽڱ�ʾ���ļ�ѹ����Ĵ�С
};

//���ļ���ѹ����ʽ
enum XPACK_METHOD
{
	TYPE_NONE	= 0x00000000,			//û��ѹ��
	TYPE_UCL	= 0x01000000,			//UCLѹ��
	TYPE_BZIP2	= 0x02000000,			//bzip2ѹ��
	TYPE_FRAME	= 0x10000000,			//ʹ���˶���֡ѹ��,���ļ�Ϊspr����ʱ�ſ����õ�
	TYPE_METHOD_FILTER = 0x0f000000,	//���˱��
	TYPE_FILTER = 0xff000000,			//���˱��
};

//pak���б����spr֡��Ϣ��
struct XPackSprFrameInfo
{
	int32_t lCompressSize;
	int32_t lSize;
} ;
*/
XPackFile::XPackElemFileCache	XPackFile::ms_ElemFileCache[MAX_XPACKFILE_CACHE];
int32_t								XPackFile::ms_nNumElemFileCache = 0;

XPackFile::XPackFile()
{
	m_hFile = NULL;
	m_uFileSize = 0;
	m_pIndexList = NULL;
	m_nElemFileCount = 0;  // ���ļ�����
	m_nSelfIndex     = 0;
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	   InitializeCriticalSection(&m_ReadCritical);
    #else
	   pthread_mutex_init(&m_ReadCritical,NULL);
    #endif
	ZeroMemory(ms_ElemFileCache,sizeof(XPackElemFileCache)*MAX_XPACKFILE_CACHE);
}

XPackFile::~XPackFile()
{
	Close();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	    DeleteCriticalSection(&m_ReadCritical);
    #else
	    pthread_mutex_destroy(&m_ReadCritical);
    #endif
}

//-------------------------------------------------
//���ܣ��򿪰��ļ�
//���أ��ɹ����
//-------------------------------------------------
bool XPackFile::Open(const char* pszPackFileName, int32_t nSelfIndex)
{
	bool bResult = false;
	Close();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	EnterCriticalSection(&m_ReadCritical);
#else
	pthread_mutex_lock(&m_ReadCritical);
#endif
	m_nSelfIndex = nSelfIndex;
	//m_hFile = ::CreateFile(pszPackFileName, GENERIC_READ, FILE_SHARE_READ, NULL,OPEN_EXISTING , 0, NULL);
	//std::string fullPath = fullPathForFilename(pszFileName);
	m_hFile = fopen(pszPackFileName, "rb");

	//ccMessageBox(pszPackFileName,"bool XPackFile::Open start");
	while (m_hFile)
	{
		//m_uFileSize = ::GetFileSize(m_hFile, NULL);
		//m_uFileSize = m_hFile->size();
		
		fseek(m_hFile,0,SEEK_END);
		m_uFileSize = ftell(m_hFile);  //�ļ��Ĵ�С
		fseek(m_hFile,0,SEEK_SET);     //ָ��������ͷ

		if (m_uFileSize == 0 || m_uFileSize == INVALID_FILE_SIZE || m_uFileSize <= sizeof(XPackFileHeader))
		{
			break;
		}

		XPackFileHeader	Header;
        int32_t size = sizeof(XPackFileHeader);
	   // ZeroMemory(&Header,sizeof(XPackFileHeader));
		memset(&Header,0,sizeof(XPackFileHeader));
		size_t dwListSize, dwReaded=0;
		//--��ȡ���ļ�ͷ-- size = fread(pData,sizeof(unsigned char), size,fp);
		char nFileInfo[64]={0};
		//sprintf(nFileInfo,"�ļ���С:%d",m_uFileSize); fread(lpBuffer, 1, dwReadBytes, m_hFile);
		//fwrite(&header, 1, sizeof(header), output); //д��ͷ�ṹ
		if (!(dwReaded = fread(&Header,1,sizeof(Header),m_hFile)))
		//if (::ReadFile(m_hFile, &Header, sizeof(Header), &dwReaded, NULL) == FALSE)
		{
//          if (::ReadFile(m_hFile, &xHeader, sizeof(xHeader), &dwReaded, NULL) == FALSE)
			break;
		}
		//--���ļ���������ݵĺϷ����ж�--
		
		if (dwReaded != sizeof(Header) ||
			*(int32_t*)(&Header.cSignature) != XPACKFILE_SIGNATURE_FLAG ||
			Header.uCount == 0 ||
			Header.uIndexTableOffset < sizeof(XPackFileHeader) ||
			Header.uIndexTableOffset >= m_uFileSize ||
			Header.uDataOffset < sizeof(XPackFileHeader) ||
			Header.uDataOffset >= m_uFileSize)
		{
			
			break;
		}
		//--��ȡ��������Ϣ��--
		dwListSize = sizeof(XPackIndexInfo) * Header.uCount;
		m_pIndexList = (XPackIndexInfo*)malloc(dwListSize);   //�����ڴ档������
		//printf("---PAK(%s)�����ڴ�: %u Mb,��%d���ļ�--\n",pszPackFileName,dwListSize/(1024*1024),Header.uCount);
		//if (m_pIndexList == NULL ||
		//	::SetFilePointer(m_hFile, Header.uIndexTableOffset, NULL, FILE_BEGIN) != Header.uIndexTableOffset)
		int32_t  nCurOffset = 0;
		//if (m_pIndexList == NULL || (nCurOffset = fseek(m_hFile,Header.uIndexTableOffset,FILE_BEGIN)) != Header.uIndexTableOffset)
		if (m_pIndexList == NULL)
		{
			//sprintf(nFileInfo,"ƫ��:%d==%d �ļ���С:%d %d |%d = %d",nCurOffset,Header.uIndexTableOffset,m_uFileSize,Header.uCount,dwReaded,sizeof(Header));
			//MessageBox(NULL, nFileInfo,"Information",MB_OK | MB_ICONEXCLAMATION);
			break;
		}
		fseek(m_hFile,Header.uIndexTableOffset,FILE_BEGIN);//����ָ��Ŀ�ʼλ��
		//if (::ReadFile(m_hFile, m_pIndexList, dwListSize, &dwReaded, NULL) == FALSE)
		if (!(dwReaded = fread(m_pIndexList,1,dwListSize,m_hFile))) //m_uFileSize
		   break;

		if (dwReaded != dwListSize)
			break;

		m_nElemFileCount = Header.uCount;  //���ļ�����
		bResult = true;
		break;
	};
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	LeaveCriticalSection(&m_ReadCritical);
#else
	pthread_mutex_unlock(&m_ReadCritical);
#endif

	if (bResult == false)
		Close();

	//ccMessageBox(pszPackFileName,"bool XPackFile::Open return");
	return bResult;
}
//-------------------------------------------------
//���ܣ����°��ļ�
//���أ��ɹ����
//-------------------------------------------------
/*bool XPackFile::OpenX(const char* pszPackFileName, int32_t nSelfIndex)
{
	bool bResult = false;
	Close();
	EnterCriticalSection(&m_ReadCritical);
	m_nSelfIndex = nSelfIndex;
	m_hFile = ::CreateFile(pszPackFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	while (m_hFile != INVALID_HANDLE_VALUE)
	{
		m_uFileSize = ::GetFileSize(m_hFile, NULL);
		if (m_uFileSize == 0 || m_uFileSize == INVALID_FILE_SIZE || m_uFileSize <= sizeof(XPackFileHeaderX))
		{
			break;
		}
		XPackFileHeaderX xHeader;
		DWORD dwListSize, dwReaded;
		//--��ȡ���ļ�ͷ--
		if (::ReadFile(m_hFile, &xHeader, sizeof(xHeader), &dwReaded, NULL) == FALSE)
		{
			break;
		}
		//--���ļ���������ݵĺϷ����ж�--
		if (dwReaded != sizeof(xHeader) ||
			*(int32_t*)(&xHeader.cSignature) != XPACKFILE_SIGNATURE_FLAG ||
			xHeader.uCount == 0 ||
			xHeader.SubHeader[0].uIndexTableOffset < sizeof(XPackFileHeaderX) ||
			xHeader.SubHeader[0].uIndexTableOffset >= m_uFileSize ||
			xHeader.SubHeader[0].uDataOffset < sizeof(XPackFileHeaderX) ||
			xHeader.SubHeader[0].uDataOffset >= m_uFileSize)
		{
			break;
		}
		
		//--��ȡ��������Ϣ��--
		dwListSize = sizeof(XPackIndexInfoX) * xHeader.SubHeader[0].uCount;
		m_pIndexListX = (XPackIndexInfoX*)malloc(dwListSize);   //�����ڴ档������
		if (m_pIndexListX == NULL ||
			::SetFilePointer(m_hFile, xHeader.SubHeader[0].uIndexTableOffset, NULL, FILE_BEGIN) != xHeader.SubHeader[0].uIndexTableOffset)
		{//���õ�ǰ�ļ���ȡλ��
			break;
		}
		//ֱ�Ӷ�ȡ���ļ�
		if (::ReadFile(m_hFile, m_pIndexListX, dwListSize, &dwReaded, NULL) == FALSE)
			break;
		if (dwReaded != dwListSize)
			break;		
		m_nElemFileCount= xHeader.SubHeader[0].uCount;  //���ļ�����
		bResult = true;
		break;
	};
	if (bResult == false)
		Close();
	LeaveCriticalSection(&m_ReadCritical);
	return bResult;
}	*/
//-------------------------------------------------
//���ܣ��رհ���
//-------------------------------------------------
void XPackFile::Close()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	EnterCriticalSection(&m_ReadCritical);
#else
	pthread_mutex_lock(&m_ReadCritical);
#endif

	if (m_pIndexList)
	{
		//----���cache�л��浽�ģ����ܣ��Ǵ˰��е����ļ�----
		for (int32_t i = ms_nNumElemFileCache - 1; i >=0; i--)
		{
			if (ms_ElemFileCache[i].nPackIndex == m_nSelfIndex)
			{
				FreeElemCache(i);
				ms_nNumElemFileCache --;
				for (int32_t j = i; j < ms_nNumElemFileCache; ++j)
					ms_ElemFileCache[j] = ms_ElemFileCache[j + 1];
			}
		}
		free (m_pIndexList);
		m_pIndexList = NULL;
	}
	m_nElemFileCount = 0;

	if (m_hFile)
	{
		fclose(m_hFile);
		m_hFile = NULL;
	}
	m_uFileSize = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	LeaveCriticalSection(&m_ReadCritical);
#else
	pthread_mutex_unlock(&m_ReadCritical);
#endif
}

//-------------------------------------------------
//���ܣ��ͷ�һ��cache��������
//���أ��ɹ����
//-------------------------------------------------
void XPackFile::FreeElemCache(int32_t nCacheIndex)
{
	//_ASSERT(nCacheIndex >= 0 && nCacheIndex < ms_nNumElemFileCache);
	if (nCacheIndex<0 || nCacheIndex >= ms_nNumElemFileCache) return;
	if (ms_ElemFileCache[nCacheIndex].pBuffer)
	{
		///free(ms_ElemFileCache[nCacheIndex].pBuffer);��
		ms_ElemFileCache[nCacheIndex].pBuffer = NULL;
	}
	ms_ElemFileCache[nCacheIndex].uId = 0;
	ms_ElemFileCache[nCacheIndex].lSize = 0;
	ms_ElemFileCache[nCacheIndex].uRefFlag = 0;
	ms_ElemFileCache[nCacheIndex].nPackIndex = -1;
}

//-------------------------------------------------
//���ܣ�ֱ�Ӷ�ȡ���ļ������е����ݵ�������
//���أ��ɹ����
//-------------------------------------------------
bool XPackFile::DirectRead(void* pBuffer, uint32_t uOffset, uint32_t uSize) const
{
	bool bResult = false;
	DWORD dwReaded = 0;
	//_ASSERT(pBuffer && m_hFile != INVALID_HANDLE_VALUE);
    if  (pBuffer==NULL || !m_hFile)
		return bResult;

	//if (uOffset + uSize <= m_uFileSize && ::SetFilePointer(m_hFile, uOffset, 0, FILE_BEGIN) == uOffset) //���õ�ǰ�Ķ�дλ��
    //dwBytesRead = fread(lpBuffer, 1, dwReadBytes, m_hFile);	fseek(m_hFile, lDistance, dwMoveMethod);
    if (uOffset + uSize <= m_uFileSize /*&& fseek(m_hFile, 0, FILE_BEGIN) == uOffset*/) //���õ�ǰ�Ķ�дλ��
    {
		//if (ReadFile(m_hFile, pBuffer, uSize, &dwReaded, NULL))
		fseek(m_hFile,uOffset,FILE_BEGIN); //����ָ��Ŀ�ʼλ��

    	if (dwReaded = fread(pBuffer,1,uSize,m_hFile))
		{
			if (dwReaded == uSize)
				bResult = true;
		}
	}
	return bResult;
}

//-------------------------------------------------
//���ܣ�����ѹ�ض�ȡ���ļ���������
//������pBuffer --> ������ָ��
//		uExtractSize  --> ���ݣ���������ѹ��Ĵ�С��pBuffer�������Ĵ�С��С�ڴ���
//		lCompressType --> ֱ�ӴӰ��ж�ȡ��ԭʼ��/ѹ������С
//		uOffset  --> �Ӱ��еĴ�ƫ��λ�ÿ�ʼ��ȡ
//		uSize    --> �Ӱ���ֱ�Ӷ�ȡ�ã�ѹ�������ݵĴ�С
//���أ��ɹ����
//-------------------------------------------------
bool XPackFile::ExtractRead(void* pBuffer, uint32_t uExtractSize,
	int32_t lCompressType, uint32_t uOffset, uint32_t uSize) const
{
	if (!pBuffer)return false;
	bool bResult = false;
	if (lCompressType == TYPE_NONE)
	{//û��ѹ��  ֱ�Ӷ�ȡ
		if (uExtractSize == uSize)
			bResult = DirectRead(pBuffer, uOffset, uSize);

		//ccMessageBox("DirectRead","ExtractRead");
	}
	else
	{
		//ccMessageBox("ucl_nrv2e_decompress_8_1","ExtractRead");
		void*	pReadBuffer = malloc(uSize);  //�����ڴ�
		//|| lCompressType == TYPE_BZIP2
		if (pReadBuffer)
		{
			if ((lCompressType == TYPE_UCL || lCompressType == TYPE_UCL_OLD/* || lCompressType == TYPE_BZIP2_OLD || lCompressType == TYPE_BZIP2*/) && DirectRead(pReadBuffer, uOffset, uSize))
			{//0x01000000
				uint32_t uDestLength = 0;
				ucl_nrv2b_decompress_8((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength, NULL);  //��ѹ��ȡ
				//ucl_nrv2e_decompress_8_1((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength);
				bResult =  (uDestLength == uExtractSize); 
			}
			else if ((/*lCompressType == TYPE_FRAGMENT || */lCompressType ==TYPE_FRAGMENT_OLD) && DirectRead(pReadBuffer, uOffset, uSize))
			{//0x03000000
				uint32_t uDestLength;
				ucl_nrv2b_decompress_8((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength, NULL);  //��ѹ��ȡ
				//ucl_nrv2e_decompress_8_2((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength);
				//ucl_nrv2e_decompress_8_fs((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength); 
				bResult =  (uDestLength == uExtractSize);
			}
			else if ((/*lCompressType == TYPE_FRAGMENTA || */lCompressType == TYPE_FRAGMENTA_OLD)  && DirectRead(pReadBuffer, uOffset, uSize))
			{//0x04000000
				uint32_t uDestLength;
				ucl_nrv2b_decompress_8((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength, NULL);  //��ѹ��ȡ
				//ucl_nrv2e_decompress_8_3((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength);
				//ucl_nrv2e_decompress_8_fs((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength); 
				bResult =  (uDestLength == uExtractSize);
			}
			else 
			{//�°� 
				if (DirectRead(pReadBuffer, uOffset, uSize))  //����ԭʼ����
				{ 
					uint32_t uDestLength;
					//ucl_nrv2e_decompress_8_fs((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength); 
					//ucl_nrv2b_decompress_8((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength, NULL);  //��ѹ��ȡ
					//ucl_nrv2b_decompress_safe_8((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength, NULL);  //��ѹ��ȡ
					ucl_decompress_8_New((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength); 

					bResult =  (uDestLength == uExtractSize);                                            //��ѹ����
				} 
			}  
			free (pReadBuffer);
		}
	}
	return bResult;
}


typedef char                int8;
typedef short               int16;
typedef int32_t	                int32;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef uint32_t        uint32;
//typedef unsigned __int64    uint64;

#define _BYTE  uint8
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define BYTE1(x)      BYTEn(x,  1)  
//
int32_t  XPackFile::ucl_decompress_8_New(unsigned char * a1, uint32_t a2, unsigned char * a3, uint32_t *a4) const
	//(_BYTE *a1, int32_t a2, int32_t a3, _DWORD *a4)
{
	BYTE *v4; // ecx@1
	int32_t v5; // eax@1
	int32_t v6; // ebx@1
	int32_t v7; // eax@4
	int32_t v8; // edx@7
	int32_t v9; // eax@9
	int32_t v10; // eax@10
	int32_t v11; // eax@13
	uint32_t v12; // ebp@16
	int32_t v13; // ebp@17
	int32_t v14; // eax@20
	int32_t v15; // eax@21
	int32_t v16; // edx@22
	int32_t v17; // eax@24
	int32_t v18; // esi@25
	int32_t v19; // esi@26
	int32_t v20; // eax@28
	int32_t v21; // edx@29
	int32_t v22; // eax@32
	int32_t v23; // edi@37
	BYTE *v24; // edi@37
	int32_t v25; // ecx@40
	int32_t result; // eax@41
	int32_t v27; // [sp+10h] [bp-4h]@1

	v4 = a1;
	v5 = 0;
	v27 = 1;
	v6 = 0;
	while (1)
	{
		while (1)
		{
			if (v5&0x7F)
			{
				v5 *= 2;
			}
			else
			{
				v7 = *v4++;
				v5 = 2 * v7 + 1;
			}
			if (!(v5&0x0100)) //(v5&0x100)
				break;
			*(v6++ + a3) = *v4++;
		}
		v8 = 1;
		do
		{
			if (v5&0x7F)
			{
				v9 = 2 * v5;
			}
			else
			{
				v10 = *v4++;
				v9 = 2 * v10 + 1;
			}
			v8 = ((v9&0x0100) >> 8) + 2 * v8;
			if (v9&0x7F)
			{
				v5 = 2 * v9;
			}
			else
			{
				v11 = *v4++;
				v5 = 2 * v11 + 1;
			}
		}
		while (!(v5&0x0100));
		if ( v8 == 2 )
		{
			v12 = v27;
			goto LABEL_19;
		}
		v13 = *v4++ + ((v8 + 0x00fffffd) << 8);
		if ( v13 == 0xffffffff )
			break;
		v12 = v13 + 1;
		v27 = v12;
LABEL_19:
		if ( v5 & 0x7F )
		{
			v14 = 2 * v5;
		}
		else
		{
			v15 = *v4++;
			v14 = 2 * v15 + 1;
		}
		v16 = (v14&0x0100)>> 8;
		if ( v14 & 0x7F )
		{
			v5 = 2 * v14;
		}
		else
		{
			v17 = *v4++;
			v5 = 2 * v17 + 1;
		}
		v18 = ((v5&0x0100) >> 8) + 2 * v16;
		if ( !v18 )
		{
			v19 = 1;
			do
			{
				if ( v5 & 0x7F ) //һ���ֽ�?
				{
					v20 = 2 * v5;
				}
				else
				{
					v21 = *v4++;
					v20 = 2 * v21 + 1;
				}
				v19 = ((v20&0x0100) >> 8) + 2 * v19;
				if ( v20 & 0x7F )
				{
					v5 = 2 * v20;
				}
				else
				{
					v22 = *v4++;
					v5 = 2 * v22 + 1;
				}
			}
			while (!(v5&0x0100));
			v18 = v19 + 2;
		}
		if ( v12 > 0x0D00 )
			++v18;
		v23 = v6++ - v12;
		*(v6 + a3 - 1) = *(v23 + a3);
		v24 = (v23 + a3 + 1);
		do
		{
			*(v6++ + a3) = *v24++;
			--v18;
		}
		while ( v18 );
	}
	v25 = v4 - a1;
	*a4 = v6;
	if ( a2 <= v25 )
	{
		if ( a2 == v25 )
			result = 0;
		else
			result = (a2 >= v25) - 1;
	}
	else
	{
		result = 1;
	}
	return result;
}

//int32_t __cdecl sub_10025D00(int32_t a1, uint32_t a2, int32_t a3, int32_t *a4)
int32_t  XPackFile::ucl_nrv2e_decompress_8_2(unsigned char * a1, uint32_t a2, unsigned char *a3, uint32_t *a4) const
{
	BYTE *v4; // ebp@1
	int32_t v5; // eax@1
	uint32_t v6; // ecx@1
	int32_t v7; // edi@1
	int32_t v8; // eax@4
	int32_t v9; // edx@7
	int32_t v10; // eax@9
	int32_t v11; // eax@10
	int32_t v12; // esi@11
	int32_t v13; // ebx@12
	int32_t v14; // edx@13
	int32_t v15; // eax@17
	int32_t v16; // edx@20
	int32_t v17; // edx@21
	uint32_t v18; // eax@22
	int32_t v19; // eax@26
	int32_t v20; // edi@27
	int32_t v21; // edi@28
	int32_t v22; // eax@30
	int32_t v23; // edx@31
	int32_t v24; // eax@34
	int32_t v25; // edi@37
	BYTE *v26; // esi@37
	int32_t v27; // ebp@37
	char v28; // bl@37
	BYTE *v29; // esi@37
	int32_t v30; // ST10_4@39
	int32_t result; // eax@41
	int32_t v32; // eax@42
	int32_t v33; // [sp+10h] [bp-Ch]@7
	uint32_t v34; // [sp+14h] [bp-8h]@19
	int32_t v35; // [sp+18h] [bp-4h]@1

	v4 = a1;
	v5 = 0;
	v6 = 0;
	v7 = 0;
	v35 = 1;
	while ( 1 )
	{
		while ( 1 )
		{
			if ( v5 & 0x7F )
			{
				v5 *= 2;
			}
			else
			{
				v8 = *(v6++ + v4);
				v5 = 2 * v8 + 1;
			}
			if ( !(v5&0x100) )  //(v5&0x100)
				break;
			*(v7++ + a3) = *(v6++ + v4);
		}
		v33 = v7;
		v9 = 1;
		while ( 1 )
		{
			if ( v5 & 0x7F )
			{
				v10 = 2 * v5;
			}
			else
			{
				v11 = *(v6++ + v4);
				v10 = 2 * v11 + 1;
			}
			v12 = ((v10&0x100) >> 8) + 2 * v9;
			if ( v10 & 0x7F )
			{
				v13 = 2 * v10;
			}
			else
			{
				v14 = *(v6++ + v4);
				v13 = 2 * v14 + 1;
			}
			if (v13&0x100)
				break;
			if ( v13 & 0x7F )
			{
				v5 = 2 * v13;
				v9 = (((uint32_t)(2 * v13) >> 8) & 1) + 2 * v12 - 2;
			}
			else
			{
				v15 = *(v6++ + v4);
				v5 = 2 * v15 + 1;
				v9 = ((v5&0x100) >> 8) + 2 * v12 - 2;
			}
		}
		if ( v12 == 2 )
		{
			v34 = v35;
			if ( v13 & 0x7F )
			{
				v13 *= 2;
				v16 = (v13&0x100) >> 8;
			}
			else
			{
				v17 = *(v6++ + v4);
				v13 = 2 * v17 + 1;
				v16 = (v13&0x100) >> 8;
			}
			goto LABEL_24;
		}
		v18 = ((v12 + 0x00fffffd) << 8) + *(v6++ + v4);
		if ( v18 == 0xffffffff)
			break;
		v16 = ~(BYTE)v18 & 0x100;
		v34 = (v18 >> 1) + 1;
		v35 = (v18 >> 1) + 1;
LABEL_24:
		if ( v13 & 0x7F )
		{
			v5 = 2 * v13;
		}
		else
		{
			v19 = *(v6++ + v4);
			v5 = 2 * v19 + 1;
		}
		v20 = ((v5&0x100) >> 8) + 2 * v16;
		if ( !v20 )
		{
			v21 = 1;
			do
			{
				if ( v5 & 0x7F )
				{
					v22 = 2 * v5;
				}
				else
				{
					v23 = *(v6++ + v4);
					v22 = 2 * v23 + 1;
				}
				v21 = ((v22&0x100) >> 8) + 2 * v21;
				if ( v22 & 0x7F )
				{
					v5 = 2 * v22;
				}
				else
				{
					v24 = *(v6++ + v4);
					v5 = 2 * v24 + 1;
				}
			}
			while ( !(v5&0x100) );
			v20 = v21 + 2;
		}
		v25 = (v34 > 0x500) + v20;
		v26 = (a3 + v33 - v34);
		v27 = v33 + 1;
		v28 = *v26;
		v29 = v26 + 1;
		*(a3 + v33 + 1 - 1) = v28;
		do
		{
			*(a3 + v27++) = *v29++;
			--v25;
		}
		while ( v25 );
		v30 = v27;
		v4 = a1;
		v7 = v30;
	}
	*a4 = v7;
	if ( v6 == a2 )
	{
		result = 0;
	}
	else
	{
		v32 = -(v6 < a2);
		//LOBYTE(v32) = v32 & 0xFC;
		result = v32;// - 201;
	}
	return result;
}
//int32_t __cdecl ucl_nrv2e_decompress_8_3(int32_t a1, uint32_t a2, int32_t a3, int32_t *a4)
int32_t  XPackFile::ucl_nrv2e_decompress_8_1(unsigned char * a1, uint32_t a2, unsigned char *a3, uint32_t *a4) const
{
	uint32_t v4; // ecx@1
	int32_t v5; // eax@1
	int32_t v6; // eax@4
	int32_t v7; // esi@7
	int32_t v8; // eax@9
	int32_t v9; // eax@10
	int32_t v10; // eax@13
	uint32_t v11; // ebp@16
	int32_t v12; // ebp@17
	int32_t v13; // eax@20
	int32_t v14; // eax@21
	int32_t v15; // edx@22
	int32_t v16; // eax@24
	int32_t v17; // edi@25
	int32_t v18; // edi@26
	int32_t v19; // eax@28
	int32_t v20; // edx@29
	int32_t v21; // eax@32
	int32_t v22; // edi@35
	BYTE *v23; // esi@35
	int32_t v24; // edx@35
	char v25; // bl@35
	BYTE *v26; // esi@35
	int32_t result; // eax@39
	int32_t v28; // eax@40
	int32_t v29; // [sp+10h] [bp-8h]@1
	int32_t v30; // [sp+14h] [bp-4h]@1

	v4 = 0;
	v5 = 0;
	v29 = 0;
	v30 = 1;
	while ( 1 )
	{
		while ( 1 )
		{
			if ( v5 & 0x7F )
			{
				v5 *= 2;
			}
			else
			{
				v6 = *(v4++ + a1);
				v5 = 2 * v6 + 1;
			}
			if ( !(v5&0x100) ) //(v5&0x100)
				break;
			*(v29++ + a3) = *(v4++ + a1);
		}
		v7 = 1;
		do
		{
			if ( v5 & 0x7F )
			{
				v8 = 2 * v5;
			}
			else
			{
				v9 = *(v4++ + a1);
				v8 = 2 * v9 + 1;
			}
			v7 = ((v8&0x100) >> 8) + 2 * v7;
			if ( v8 & 0x7F )
			{
				v5 = 2 * v8;
			}
			else
			{
				v10 = *(v4++ + a1);
				v5 = 2 * v10 + 1;
			}
		}
		while ( !(v5&0x100) );
		if ( v7 == 2 )
		{
			v11 = v30;
			goto LABEL_19;
		}
		v12 = ((v7 + 16777213) << 8) + *(v4++ + a1);
		if ( v12 == -1 )
			break;
		v11 = v12 + 1;
		v30 = v11;
LABEL_19:
		if ( v5 & 0x7F )
		{
			v13 = 2 * v5;
		}
		else
		{
			v14 = *(v4++ + a1);
			v13 = 2 * v14 + 1;
		}
		v15 = (v13&0x100) >> 8;
		if ( v13 & 0x7F )
		{
			v5 = 2 * v13;
		}
		else
		{
			v16 = *(v4++ + a1);
			v5 = 2 * v16 + 1;
		}
		v17 = ((v5&0x100) >> 8) + 2 * v15;
		if ( !v17 )
		{
			v18 = 1;
			do
			{
				if ( v5 & 0x7F )
				{
					v19 = 2 * v5;
				}
				else
				{
					v20 = *(v4++ + a1);
					v19 = 2 * v20 + 1;
				}
				v18 = ((v19&0x100) >> 8) + 2 * v18;
				if ( v19 & 0x7F )
				{
					v5 = 2 * v19;
				}
				else
				{
					v21 = *(v4++ + a1);
					v5 = 2 * v21 + 1;
				}
			}
			while ( !(v5&0x100) );
			v17 = v18 + 2;
		}
		v22 = (v11 > 0xD00) + v17;
		v23 = (a3 + v29 - v11);
		v24 = v29 + 1;
		v25 = *v23;
		v26 = v23 + 1;
		*(v29 + 1 + a3 - 1) = v25;
		do
		{
			*(v24++ + a3) = *v26++;
			--v22;
		}
		while ( v22 );
		v29 = v24;
	}
	*a4 = v29;
	if ( v4 == a2 )
	{
		result = 0;
	}
	else
	{
		v28 = -(v4 < a2);
		//LOBYTE(v28) = v28 & 0xFC;
		result = v28;// - 201;
	}
	return result;
}
int32_t  XPackFile::ucl_nrv2e_decompress_8_3(unsigned char * a1, uint32_t a2, unsigned char *a3, uint32_t *a4) const
{
	int32_t v4; // eax@1
	uint32_t v5; // ecx@1
	int32_t v6; // ebp@1
	int32_t v7; // eax@4
	int32_t v8; // edx@7
	int32_t v9; // eax@9
	int32_t v10; // eax@10
	int32_t v11; // esi@11
	int32_t v12; // edx@12
	int32_t v13; // edx@13
	int32_t v14; // eax@17
	uint32_t v15; // ebx@19
	int32_t v16; // esi@20
	int32_t v17; // eax@21
	uint32_t v18; // eax@22
	int32_t v19; // esi@26
	int32_t v20; // edx@27
	int32_t v21; // eax@30
	int32_t v22; // esi@33
	int32_t v23; // edx@34
	int32_t v24; // eax@37
	int32_t v25; // eax@38
	int32_t v26; // eax@41
	int32_t v27; // esi@44
	BYTE *v28; // edi@44
	char v29; // bl@44
	BYTE *v30; // edi@44
	int32_t result; // eax@48
	int32_t v32; // eax@49
	int32_t v33; // [sp+10h] [bp-4h]@1

	v4 = 0;
	v5 = 0;
	v6 = 0;
	v33 = 1;
	while ( 1 )
	{
		while ( 1 )
		{
			if ( v4 & 0x7F )
			{
				v4 *= 2;
			}
			else
			{
				v7 = *(v5++ + a1);
				v4 = 2 * v7 + 1;
			}
			if (!(v4&0x100)) //ch & 0x100
				break;
			*(a3 + v6++) = *(v5++ + a1);
		}
		v8 = 1;
		while ( 1 )
		{
			if ( v4 & 0x7F )
			{
				v9 = 2 * v4;
			}
			else
			{
				v10 = *(v5++ + a1);
				v9 = 2 * v10 + 1;
			}
			v11 = ((v9&0x100) >> 8) + 2 * v8;
			if ( v9 & 0x7F )
			{
				v12 = 2 * v9;
			}
			else
			{
				v13 = *(v5++ + a1);
				v12 = 2 * v13 + 1;
			}
			if (v12&0x100)
				break;
			if ( v12 & 0x7F )
			{
				v4 = 2 * v12;
				v8 = (((uint32_t)(2 * v12) >> 8) & 0x100) + 2 * v11 - 2;
			}
			else
			{
				v14 = *(v5++ + a1);
				v4 = 2 * v14 + 1;
				v8 = ((v4&0x100) >> 8) + 2 * v11 - 2;
			}
		}
		if ( v11 != 2 )
			break;
		v15 = v33;
		if ( v12 & 0x7F )
		{
			v12 *= 2;
			v16 = (v12&0x100) >> 8;
		}
		else
		{
			v17 = *(v5++ + a1);
			v12 = 2 * v17 + 1;
			v16 = (v12&0x100) >> 8;
		}
LABEL_24:
		if ( v16 )
		{
			if ( v12 & 0x7F )
			{
				v4 = 2 * v12;
				v19 = (((uint32_t)(2 * v12) >> 8) & 0x100) + 1;
			}
			else
			{
				v20 = *(v5++ + a1);
				v4 = 2 * v20 + 1;
				v19 = ((v4&0x100) >> 8) + 1;
			}
		}
		else
		{
			if ( v12 & 0x7F )
			{
				v4 = 2 * v12;
			}
			else
			{
				v21 = *(v5++ + a1);
				v4 = 2 * v21 + 1;
			}
			if (v4&0x100)
			{
				if ( v4 & 0x7F )
				{
					v4 *= 2;
					v22 = (v4&0x100)>> 8;
				}
				else
				{
					v23 = *(v5++ + a1);
					v4 = 2 * v23 + 1;
					v22 = (v4&0x100) >> 8;
				}
			}
			else
			{
				v22 = 1;
				do
				{
					if ( v4 & 0x7F )
					{
						v24 = 2 * v4;
					}
					else
					{
						v25 = *(v5++ + a1);
						v24 = 2 * v25 + 1;
					}
					v22 = ((v24&0x100) >> 8) + 2 * v22;
					if ( v24 & 0x7F )
					{
						v4 = 2 * v24;
					}
					else
					{
						v26 = *(v5++ + a1);
						v4 = 2 * v26 + 1;
					}
				}
				while (!(v4&0x100));
			}
			v19 = v22 + 3;
		}
		v27 = (v15 > 0x500) + v19;
		v28 = (a3 + v6++ - v15);
		v29 = *v28;
		v30 = v28 + 1;
		*(a3 + v6 - 1) = v29;
		do
		{
			*(a3 + v6++) = *v30++;
			--v27;
		}
		while ( v27 );
	}
	v18 = ((v11 + 0x00fffffd) << 8) + *(v5++ + a1);
	if ( v18 != 0xffffffff)
	{
		v16 = ~v18 & 0x100;
		v15 = (v18 >> 1) + 1;
		v33 = (v18 >> 1) + 1;
		goto LABEL_24;
	}
	*a4 = v6;
	if ( v5 == a2 )
	{
		result = 0;
	}
	else
	{
		v32 = -(v5 < a2);
		//LOBYTE(v32) = v32 & 0xFC;
		result = v32;// - 201;
	}
	return result;
}

//��ѹ����
int32_t XPackFile::ucl_nrv2e_decompress_8_fs(unsigned char * s_buf,int32_t s_size,unsigned char * d_buf,uint32_t *d_size)  const
{
	uint32_t ch=0;
	uint32_t repeat_pri=1;		    //��һ�ε� REPEAT VALUE
	uint32_t repeat_cur;			//���ε� REPEAT VALUE
	uint32_t repeat_cnt;			// repeat count
	uint32_t d_cnt=0;
	unsigned char * bak_s_buf;		    //���ݵ�����
	
	bak_s_buf=s_buf;
	do
	{
		do
		{
			if((ch & 0x7f)!=0)
				ch<<=1;
			else
				ch=((*(s_buf++))<<1)+1;
			if((ch & 0x100)==0) break;
			*(d_buf+d_cnt++)=*(s_buf++);
		}while(1);
		repeat_cur=1;

		do
		{
			if((ch & 0x7f)!=0)
				ch<<=1;
			else
				ch=((*(s_buf++))<<1)+1;
			repeat_cur<<=1;
			repeat_cur +=(ch & 0x100)>>8;
			if((ch & 0x7f)!=0)
				ch<<=1;
			else
				ch=((*(s_buf++))<<1)+1;
		}while((ch & 0x100)==0);

		if(repeat_cur==2)
			repeat_cur=repeat_pri;
		else
		{
			repeat_cur +=0x00fffffd;
			repeat_cur<<=8;
			repeat_cur +=*(s_buf++);
			if(repeat_cur==0xffffffff) break;
			  repeat_cur++;
			repeat_pri=repeat_cur;
		}
		if((ch & 0x7f)!=0)
			ch<<=1;
		else
			ch=((*(s_buf++))<<1)+1;
		repeat_cnt=(ch & 0x100)>>8;
		if((ch & 0x7f)!=0)
			ch<<=1;
		else
			ch=((*(s_buf++))<<1)+1;
		repeat_cnt<<=1;
		repeat_cnt+=(ch & 0x100)>>8;
		if(repeat_cnt==0)
		{
			repeat_cnt=1;
			do
			{
				if((ch & 0x7f)!=0)
					ch<<=1;
				else
					ch=((*(s_buf++))<<1)+1;
				repeat_cnt<<=1;
				repeat_cnt+=(ch & 0x100)>>8;
				if((ch & 0x7f)!=0) //ѹ������
					ch<<=1;
				else
					ch=((*(s_buf++))<<1)+1;
			}while((ch & 0x100)==0);
			repeat_cnt+=2;
		}
		if(repeat_cur>0x0d00) repeat_cnt++;
		repeat_cur=d_cnt-repeat_cur;
		*(d_buf+d_cnt++)=*(d_buf+repeat_cur++);
		do
		{
			*(d_buf+d_cnt++)=*(d_buf+repeat_cur++);
			repeat_cnt--;
		}while(repeat_cnt!=0);

	}while(1);
	*d_size=d_cnt;
	if(s_size>(s_buf-bak_s_buf))
		return 1;
	if(s_size==(s_buf-bak_s_buf))
		return 0;
	if(s_size<(s_buf-bak_s_buf))
		return -1;
	
	return 0;
}
//-------------------------------------------------
//���ܣ����������в������ļ���(���ַ���)
//���أ����ҵ��������������е�λ��(>=0)����δ�ҵ�����-1
//-------------------------------------------------
int32_t XPackFile::XFindElemFileA(uint32_t ulId) const
{
	int32_t nBegin, nEnd, nMid;
	nBegin = 0;
	nEnd = m_nElemFileCount - 1;
	while (nBegin <= nEnd)
	{
		nMid = (nBegin + nEnd) / 2;
		if (ulId < m_pIndexList[nMid].uId)
			nEnd = nMid - 1;
		else if (ulId > m_pIndexList[nMid].uId)
			nBegin = nMid + 1;
		else
			break;
	}
	return ((nBegin <= nEnd) ? nMid : -1);
}

//-------------------------------------------------
//���ܣ����Ұ��ڵ����ļ�
//������uId --> ���ļ���id
//		ElemRef -->����ҵ����ڴ˽ṹ���������ļ��������Ϣ
//���أ��Ƿ��ҵ�
//-------------------------------------------------
bool XPackFile::XFindElemFile(uint32_t uId, XPackElemFileRef& ElemRef)
{
	ElemRef.nElemIndex = -1;
	if (uId)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		EnterCriticalSection(&m_ReadCritical);
#else
		pthread_mutex_lock(&m_ReadCritical);
#endif
		ElemRef.nCacheIndex = FindElemFileInCache(uId, -1);  //�����Ƿ��ڻ�����
		if (ElemRef.nCacheIndex >= 0)
		{
			ElemRef.uId = uId;
			ElemRef.nPackIndex = ms_ElemFileCache[ElemRef.nCacheIndex].nPackIndex;
			ElemRef.nElemIndex = ms_ElemFileCache[ElemRef.nCacheIndex].nElemIndex;
			ElemRef.nSize = ms_ElemFileCache[ElemRef.nCacheIndex].lSize;
			ElemRef.nOffset = 0;
		}
		else
		{
			ElemRef.nElemIndex = XFindElemFileA(uId);
			if (ElemRef.nElemIndex >= 0)
			{
				ElemRef.uId = uId;
				ElemRef.nPackIndex = m_nSelfIndex;
				ElemRef.nOffset = 0;
				ElemRef.nSize = m_pIndexList[ElemRef.nElemIndex].lSize;
			}
		}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	    LeaveCriticalSection(&m_ReadCritical);
#else
		pthread_mutex_unlock(&m_ReadCritical);
#endif
	}
	return (ElemRef.nElemIndex >= 0);
}

//-------------------------------------------------
//���ܣ����仺�������������ڵ����ļ������ݵ�����
//���������ļ��ڰ��ڵ�����
//���أ��ɹ��򷵻ػ�������ָ�룬���򷵻ؿ�ָ��
//-------------------------------------------------
void* XPackFile::ReadElemFile(int32_t nElemIndex) const
{
	//_ASSERT(nElemIndex >= 0 && nElemIndex < m_nElemFileCount);
	if (nElemIndex<0 || nElemIndex >= m_nElemFileCount) 
	{
		//MessageBox(NULL,"�����ļ�ʧ��","Information",MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	void*	pDataBuffer = malloc(m_pIndexList[nElemIndex].lSize);
	if (pDataBuffer)
	{//��ѹ����ȡ�ļ�
		/*
		   ~ΪC++�е�һ�����������˼Ϊ�����������ֵ��λȡ����
		   ����Ҫʹ~x����0����Ҫʹx������λ����������λ����Ϊ1.

		   1���������е���������֮ǰ����ʾ�ú�������������������A����������
		   2���������ֻ������α���֮ǰ����ʾ�Ը���ȡ��������
		      �������~0=1, ~1=0, �������0101 0101ȡ�������1010 1010
		*/
		int32_t lCompressType = m_pIndexList[nElemIndex].lCompressSizeFlag & TYPE_FILTER_OLD;
		uint32_t uSize = m_pIndexList[nElemIndex].lCompressSizeFlag & (~TYPE_FILTER_OLD);
		//uint32_t uSize = m_pIndexList[nElemIndex].lCompressSizeFlag & (~TYPE_FILTER_SIZE);

		if (ExtractRead(pDataBuffer,
				m_pIndexList[nElemIndex].lSize,
				lCompressType,  //�����ڹ��˱�־
				m_pIndexList[nElemIndex].uOffset,
				uSize) == false)
		  {
			//MessageBox(NULL,"�����ļ�ʧ��A","Information",MB_OK | MB_ICONEXCLAMATION);
			free (pDataBuffer);
			pDataBuffer = NULL;
		  }
	}
	return pDataBuffer;
}

//-------------------------------------------------
//���ܣ���cache��������ļ�
//������uId --> ���ļ�id
//		nDesireIndex --> ��cache�еĿ���λ��
//���أ��ɹ��򷵻�cache�ڵ�����(>=0),ʧ���򷵻�-1
//-------------------------------------------------
int32_t XPackFile::FindElemFileInCache(uint32_t uId, int32_t nDesireIndex)
{
	if (nDesireIndex >= 0 && nDesireIndex < ms_nNumElemFileCache &&
		uId == ms_ElemFileCache[nDesireIndex].uId)
	{
		ms_ElemFileCache[nDesireIndex].uRefFlag = 0xffffffff;
		return nDesireIndex;
	}

	nDesireIndex = -1;
	for (int32_t i = 0; i < ms_nNumElemFileCache; ++i)
	{
		if (uId == ms_ElemFileCache[i].uId)
		{
			ms_ElemFileCache[i].uRefFlag = 0xffffffff;
			nDesireIndex = i;
			break;
		}
	}
	return nDesireIndex;
}

//-------------------------------------------------
//���ܣ������ļ�������ӵ�cache
//������pBuffer --> �������ļ����ݵĻ�����
//		nElemIndex --> ���ļ��ڰ��е�����
//���أ���ӵ�cache������λ��
//-------------------------------------------------
int32_t XPackFile::AddElemFileToCache(void* pBuffer, int32_t nElemIndex)
{
	//_ASSERT(pBuffer && nElemIndex >= 0 && nElemIndex < m_nElemFileCount);
	if (!pBuffer || nElemIndex<0 || nElemIndex >= m_nElemFileCount) return 0;

	int32_t nCacheIndex;
	if (ms_nNumElemFileCache < MAX_XPACKFILE_CACHE)
	{	//�ҵ�һ����λ��
		nCacheIndex = ms_nNumElemFileCache++;
	}
	else
	{	//�ͷ�һ���ɵ�cache�ڵ�
		nCacheIndex = 0;
		if (ms_ElemFileCache[0].uRefFlag)
			ms_ElemFileCache[0].uRefFlag --;
		for (int32_t i = 1; i < MAX_XPACKFILE_CACHE; ++i)
		{
			if (ms_ElemFileCache[i].uRefFlag)
				ms_ElemFileCache[i].uRefFlag --;
			if (ms_ElemFileCache[i].uRefFlag < ms_ElemFileCache[nCacheIndex].uRefFlag)
				nCacheIndex = i;

		}
		FreeElemCache(nCacheIndex);
	}
	ms_ElemFileCache[nCacheIndex].pBuffer = pBuffer;
	ms_ElemFileCache[nCacheIndex].uId = m_pIndexList[nElemIndex].uId;
	ms_ElemFileCache[nCacheIndex].lSize = m_pIndexList[nElemIndex].lSize;
	ms_ElemFileCache[nCacheIndex].nPackIndex = m_nSelfIndex;
	ms_ElemFileCache[nCacheIndex].nElemIndex = nElemIndex;
	ms_ElemFileCache[nCacheIndex].uRefFlag = 0xffffffff;
	return nCacheIndex;
}

//-------------------------------------------------
//���ܣ���ȡ���ļ�һ�����ȵ����ݵ�������
//������pBuffer --> ������ȡ���ݵĻ�����
//		uSize --> Ҫ��ȡ�����ݵĳ���
//���أ��ɹ���ȡ���ֽ���
//-------------------------------------------------
int32_t XPackFile::XElemFileRead(XPackElemFileRef& ElemRef, void* pBuffer, unsigned uSize)
{
	int32_t nResult = 0;
	if (pBuffer && ElemRef.uId &&ElemRef.nElemIndex >= 0)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		EnterCriticalSection(&m_ReadCritical);
#else
		pthread_mutex_lock(&m_ReadCritical);
#endif
		//--�ȿ��Ƿ��Ѿ���cache����---
		ElemRef.nCacheIndex = FindElemFileInCache(ElemRef.uId, ElemRef.nCacheIndex);

		if (ElemRef.nCacheIndex < 0 &&								//��cache��δ�ҵ�
			ElemRef.nElemIndex < m_nElemFileCount &&
			m_pIndexList[ElemRef.nElemIndex].uId == ElemRef.uId)
		{
			//MessageBox(NULL,"���д����ļ�B","Information",MB_OK | MB_ICONEXCLAMATION);
			void*	pDataBuffer = ReadElemFile(ElemRef.nElemIndex); //�����ļ�

			if (pDataBuffer) //�����ļ����棬�����������Ա���÷���
			{
				ElemRef.nCacheIndex = AddElemFileToCache(pDataBuffer, ElemRef.nElemIndex);
				//MessageBox(NULL,"���ӻ���ɹ�","Information",MB_OK | MB_ICONEXCLAMATION);
			}
		}

		if (ElemRef.nCacheIndex >= 0 &&
			//����������Ӧ��չ����飬��ֹ��ģ���ⲿ�ı䣬�������
			//ΪЧ�ʿɿ���ʡ�ԣ������ⲿ���չ������ı�ElemRef�����ݡ�
			ElemRef.nPackIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nPackIndex &&
			ElemRef.nElemIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nElemIndex &&
			ElemRef.nSize == ms_ElemFileCache[ElemRef.nCacheIndex].lSize
			)
		{
			//_ASSERT(ElemRef.nPackIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nPackIndex);
			//_ASSERT(ElemRef.nElemIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nElemIndex);
			//_ASSERT(ElemRef.nSize == ms_ElemFileCache[ElemRef.nCacheIndex].lSize);
			//MessageBox(NULL,"���д����ļ�C","Information",MB_OK | MB_ICONEXCLAMATION);
			if (ElemRef.nOffset < 0)
				ElemRef.nOffset = 0;
			if (ElemRef.nOffset < ElemRef.nSize)
			{
				if (ElemRef.nOffset + (int32_t)uSize <= ElemRef.nSize)
					nResult = uSize;
				else
					nResult = ElemRef.nSize - ElemRef.nOffset;
				memcpy(pBuffer, (char*)ms_ElemFileCache[ElemRef.nCacheIndex].pBuffer + ElemRef.nOffset, nResult);
				ElemRef.nOffset += nResult;
			}
			else
			{
				ElemRef.nOffset = ElemRef.nSize;
			}
		}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		LeaveCriticalSection(&m_ReadCritical);
#else
		pthread_mutex_unlock(&m_ReadCritical);
#endif
	}
	return nResult;
}

#define	NODE_INDEX_STORE_IN_RESERVED	2

SPRHEAD* XPackFile::GetSprHeader(XPackElemFileRef& ElemRef, SPROFFS*& pOffsetTable)
{
	SPRHEAD*		pSpr = NULL;
	bool			bOk = false;

	pOffsetTable = NULL;
	if (ElemRef.uId == 0 || ElemRef.nElemIndex < 0)
		return NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	EnterCriticalSection(&m_ReadCritical);
#else
	pthread_mutex_lock(&m_ReadCritical);
#endif
	if(ElemRef.nElemIndex < m_nElemFileCount && m_pIndexList[ElemRef.nElemIndex].uId == ElemRef.uId)
	{
        //���ȼ�����id��ʲô����ѹ����ʽ �Ƿ�ֿ�ѹ��
		if ((m_pIndexList[ElemRef.nElemIndex].lCompressSizeFlag & TYPE_FRAME) == 0)//������Ƿֿ�ѹ��
		{//ѹ������SPR��ȡ
			pSpr = (SPRHEAD*)ReadElemFile(ElemRef.nElemIndex);
			if (pSpr)
			{
				if ((*(int32_t*)&pSpr->Comment[0]) == SPR_COMMENT_FLAG) //����� SPR �ļ�
				{
					pOffsetTable = (SPROFFS*)(((char*)pSpr) + sizeof(SPRHEAD) + pSpr->Colors * 3);
					bOk = true;
				}
			}
		}
		else
		{//����Ƿֿ�ѹ�� ֱ�Ӷ�ȡ
			SPRHEAD Header;
			if (DirectRead(&Header, m_pIndexList[ElemRef.nElemIndex].uOffset, sizeof(SPRHEAD)))
			{//�����ļ�ͷ��Ϣ
				if (*(int32_t*)&(Header.Comment[0]) == SPR_COMMENT_FLAG)  //spr
				{
					uint32_t	u2ListSize = Header.Colors * 3 + Header.Frames * sizeof(XPackSprFrameInfo);
					pSpr = (SPRHEAD*)malloc(sizeof(SPRHEAD) + u2ListSize);
					if (pSpr)
					{//Ҳ�Ǽ�����ȫ��������
						if (DirectRead((char*)(&pSpr[1]),m_pIndexList[ElemRef.nElemIndex].uOffset + sizeof(SPRHEAD),u2ListSize))
						{//Ȼ��ѹ���ļ�ͷ����
							memcpy(pSpr, &Header, sizeof(SPRHEAD));
							bOk = true;
						}
					}
				}
			}
		}

		if (pSpr)
		{
			if (bOk)
			{
				*((int32_t*)&pSpr->Reserved[NODE_INDEX_STORE_IN_RESERVED]) = ElemRef.nElemIndex;
			}
			else
			{
				free (pSpr);
				pSpr = NULL;
			}
		}
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	LeaveCriticalSection(&m_ReadCritical);
#else
	pthread_mutex_unlock(&m_ReadCritical);
#endif
    return pSpr;
}

SPRFRAME* XPackFile::GetSprFrame(SPRHEAD* pSprHeader, int32_t nFrame,uint32_t &nSingFrameSize)
{
	SPRFRAME*	pFrame = NULL;
	if (pSprHeader && nFrame >= 0 && nFrame < pSprHeader->Frames)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		EnterCriticalSection(&m_ReadCritical);
#else
		pthread_mutex_lock(&m_ReadCritical);
#endif
		int32_t nNodeIndex = *((int32_t*)&pSprHeader->Reserved[NODE_INDEX_STORE_IN_RESERVED]);
		if (nNodeIndex >= 0 && nNodeIndex < m_nElemFileCount)
		{
			int32_t lCompressType = m_pIndexList[nNodeIndex].lCompressSizeFlag;
			if ((lCompressType & TYPE_FRAME) != 0) //����Ƕ���֡ѹ����
			{
				bool bOk = false;
				lCompressType &= TYPE_METHOD_FILTER_OLD;  //�ϰ�
				int32_t lTempValue = sizeof(SPRHEAD) + pSprHeader->Colors * 3;
				//uint32_t	u2ListSize = Header.Colors * 3 + Header.Frames * sizeof(XPackSprFrameInfo);
				//����ָ��֡����Ϣ
				XPackSprFrameInfo* pFrameList = (XPackSprFrameInfo *)((char*)pSprHeader + lTempValue);
				uint32_t	uSrcOffset    = m_pIndexList[nNodeIndex].uOffset + lTempValue +pSprHeader->Frames * sizeof(XPackSprFrameInfo);
			    //nFrame +=1; //0��Ч �������ˡ�����
				while(nFrame > 0)
				{//ƫ���ƶ���ָ��֡��λ��
					uSrcOffset += (pFrameList++)->lCompressSize; //�Լ�ǰִ��
					nFrame--;
				};
				lTempValue = pFrameList->lSize;

				/*if  (nFrame==0)
				{
					char ddd[64];
					sprintf(ddd,"pFrameList->lSize:%d",lTempValue);
					ccMessageBox(ddd,"pFrameList->lSize");
				}*/

				if (lTempValue < 0)
				{
					lTempValue = -lTempValue;
					if (pFrame = (SPRFRAME*)malloc(lTempValue))
						bOk = DirectRead(pFrame,uSrcOffset,lTempValue);

					if (bOk)
						nSingFrameSize = lTempValue;
				}
				else
				{//��ѹ��ȡ
					if (pFrame = (SPRFRAME*)malloc(lTempValue))
					{
						bOk = ExtractRead(pFrame,lTempValue,lCompressType,uSrcOffset,pFrameList->lCompressSize);
					    
						if (bOk)
							nSingFrameSize = lTempValue;//pFrameList->lCompressSize;
					}
				}

				if (bOk == false && pFrame != NULL)
				{
					free(pFrame);
					pFrame = NULL;
					ccMessageBox("GetSprFrame--filed","ExtractRead");
				}
			}
		}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		LeaveCriticalSection(&m_ReadCritical);
#else
		pthread_mutex_unlock(&m_ReadCritical);
#endif
	}

	if (pFrame==NULL) 
		ccMessageBox("pFrame IS NULL","GetSprFrame");

	return pFrame;	
}
