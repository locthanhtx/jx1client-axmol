//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPakFile.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	File In Dat Class
//---------------------------------------------------------------------------
//#include "KWin32.h"
#include "KFilePath.h"
#include "KPakFile.h"
//#ifndef _SERVER
#include "KPakList.h"
//#endif

//---------------------------------------------------------------------------
// �ļ���ȡģʽ 0 = ���ȴӴ��̶�ȡ 1 = ���ȴ��ļ�����ȡ
#ifdef _SERVER
static int m_nPakFileMode = 1;
#else
static int m_nPakFileMode = 1;
#endif
//---------------------------------------------------------------------------
// ����:	SetFileMode
// ����:	�����ļ���ȡģʽ
// ����:	int
// ����:	void
//---------------------------------------------------------------------------
void g_SetPakFileMode(int nFileMode)
{
	m_nPakFileMode = nFileMode;
}

#define	PAK_INDEX_STORE_IN_RESERVED	0

//----modify by Wooy to add Adjust color palette and to get rid of #@$%^& ----2003.8.19
SPRHEAD* SprGetHeader(char* pszFileName, SPROFFS*& pOffsetTable)
{
	pOffsetTable = NULL;

	if(pszFileName == NULL || pszFileName[0] == 0)
		return NULL;

	KPakFile	File;
	if (!File.Open(pszFileName))  //��SPR�ļ�
	{
		//char springo[128]={0};
		//sprintf(springo,"frams:%d,w:%d,h:%d",pSprHeader->Frames,pSprHeader->Width,pSprHeader->Height);
		//messageBox(pszFileName,"SprGetHeader");
		return NULL;
	}

	SPRHEAD*		pSpr = NULL;
	if (File.IsFileInPak())      //�жϴ��ļ��Ƿ�Ӱ��д򿪵�
	{
		//====���ļ�����Ѱ�Ҷ�ȡͼ�ļ�=====
		XPackElemFileRef	PakRef;
		//_ASSERT(g_pPakList);
		if (g_pPakList->pGetFilePath(pszFileName, PakRef))
		{
			pSpr = g_pPakList->GetSprHeader(PakRef, pOffsetTable);
			if (pSpr)
				pSpr->Reserved[PAK_INDEX_STORE_IN_RESERVED] = (unsigned short)(short)PakRef.nPackIndex;
		}
	}
	else
	{
		//messageBox(pszFileName,"SprGetHeader");
		bool			bOk = false;
		SPRHEAD			Header;
		//---���ļ�ͷ�����ж��Ƿ�Ϊ�Ϸ���sprͼ�ļ�---
		while(File.Read(&Header, sizeof(SPRHEAD)) == sizeof(SPRHEAD))
		{
			if (*(int*)&Header.Comment[0] != SPR_COMMENT_FLAG || Header.Colors > 256)  //ԭ���е�
				break;
			//---Ϊ�������������ռ�---
			int uEntireSize = File.Size();
			pSpr = (SPRHEAD*)malloc(uEntireSize);
			if (pSpr == NULL)
				break;

			uEntireSize -= sizeof(SPRHEAD);
			//---��ȡsprʣ�µ�����---
			if (File.Read(&pSpr[1], uEntireSize) == uEntireSize)
			{
				//----���ͼ��֡������Ŀ�ʼλ��---
				pOffsetTable = (SPROFFS*)(((char*)(pSpr)) + sizeof(SPRHEAD) + Header.Colors * 3);
				Header.Reserved[PAK_INDEX_STORE_IN_RESERVED] = (unsigned short)(-1);
				memcpy(pSpr, &Header, sizeof(SPRHEAD));
				bOk = true;
			}
			break;
		};

		if (bOk == false && pSpr)
		{
			free (pSpr);
			pSpr = NULL;
		}
	}
	File.Close();
	return pSpr;
}

void SprReleaseHeader(SPRHEAD* pSprHeader)
{
    if (pSprHeader)
	{
		free(pSprHeader);
		//pSprHeader = NULL;
	}
}

SPRFRAME* SprGetFrame(SPRHEAD* pSprHeader, int nFrame,unsigned int &nSingFrameSize)
{
	SPRFRAME*	pFrame = NULL;
	if (pSprHeader && g_pPakList)
	{
		int nPakIndex = (short)pSprHeader->Reserved[PAK_INDEX_STORE_IN_RESERVED];
		if (nPakIndex >= 0)
		{
			pFrame = g_pPakList->GetSprFrame(nPakIndex, pSprHeader, nFrame,nSingFrameSize);
		}
	}
	return pFrame;
}

void SprReleaseFrame(SPRFRAME* pFrame)
{
    if (pFrame)
	{
		free(pFrame);
	}
}

KSGImageContent* get_jpg_image(const char cszName[], unsigned uRGBMask16)
{
    return NULL;	
}


void release_image(KSGImageContent *pImage)
{
    if (pImage)
        free (pImage);
}


//---------------------------------------------------------------------------
// ����:	���캯��
//---------------------------------------------------------------------------
KPakFile::KPakFile()
{
//#ifndef _SERVER
	m_PackRef.nPackIndex = -1;
	m_PackRef.uId = 0;
//#endif
}

//---------------------------------------------------------------------------
// ����:	���캯��
//---------------------------------------------------------------------------
KPakFile::~KPakFile()
{
	Close();
}

//---------------------------------------------------------------------------
// ����:	�жϴ��ļ��Ƿ�Ӱ��д򿪵�
//---------------------------------------------------------------------------
bool KPakFile::IsFileInPak()
{
	return (m_PackRef.nPackIndex >= 0 && m_PackRef.uId);
}

//---------------------------------------------------------------------------
// ����:	��һ���ļ�, ��Ѱ�ҵ�ǰĿ¼���Ƿ���ͬ���ĵ����ļ�,
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��  ����װ���ļ���Ϣ
//---------------------------------------------------------------------------
int KPakFile::Open(const char* pszFileName)
{
	if (pszFileName == NULL || pszFileName[0] == 0)
		return false;
	bool bOk = false;
	Close();
	if (m_nPakFileMode == 0)	
	{
		bOk = (m_File.Open((char*)pszFileName) != FALSE); 
		if (bOk == false && g_pPakList)  
		{
			bOk = g_pPakList->pGetFilePath(pszFileName, m_PackRef);
		}
	}
	else	
	{
		if (g_pPakList) 
			bOk = g_pPakList->pGetFilePath(pszFileName,m_PackRef);
		if (bOk == false)
			bOk = (m_File.Open((char*)pszFileName) != FALSE);
	}
	return bOk;
}

//---------------------------------------------------------------------------
// ����:	���ļ��ж�ȡ����
// ����:	pBuffer		������ָ��
//			dwSize		Ҫ��ȡ�ĳ���
// ����:	�������ֽڳ���
//---------------------------------------------------------------------------
unsigned long KPakFile::Read(void* pBuffer, unsigned int uSize)
{

//#ifndef _SERVER
	if (m_PackRef.nPackIndex >= 0)  //����  INI�ļ�
	{
		//MessageBox(NULL,"���д����ļ�","Information",MB_OK | MB_ICONEXCLAMATION);

		if (g_pPakList->ElemFileRead(m_PackRef, pBuffer, uSize) == false)
		{
			//MessageBox(NULL,"�Ҳ������ļ�","Information",MB_OK | MB_ICONEXCLAMATION);
			uSize = 0;
		}
	}
	else
//#endif
	{
		uSize = m_File.Read(pBuffer, uSize);
	}
	return uSize;
}

//---------------------------------------------------------------------------
// ����:	�ļ���ָ�붨λ
// ����:	lOffset			ƫ����
//			dwMethod		��λ����
// ����:	�ļ���ָ��
//---------------------------------------------------------------------------
unsigned long KPakFile::Seek(int nOffset, unsigned int uMethod)
{
//#ifndef _SERVER
	if (m_PackRef.nPackIndex >= 0)
	{
		if (uMethod == FILE_BEGIN)
			m_PackRef.nOffset = nOffset;
		else if (uMethod == FILE_END)
			m_PackRef.nOffset = m_PackRef.nSize + nOffset;
		else
			m_PackRef.nOffset += nOffset;
		if (m_PackRef.nOffset > m_PackRef.nSize)
			m_PackRef.nOffset =  m_PackRef.nSize;
		else if (m_PackRef.nOffset < 0)
			m_PackRef.nOffset = 0;
		nOffset = m_PackRef.nOffset;
	}
	else
//#endif
	{
		nOffset = m_File.Seek(nOffset, uMethod);
	}
	return nOffset;
}

//---------------------------------------------------------------------------
// ����:	�����ļ���ָ��
// ����:	�ļ���ָ��
//---------------------------------------------------------------------------
unsigned long KPakFile::Tell()
{
	int nOffset;
//#ifndef _SERVER
	if (m_PackRef.nPackIndex >= 0)
		nOffset = m_PackRef.nOffset;
	else
//#endif
	   nOffset = m_File.Tell();

	return nOffset;
}

//---------------------------------------------------------------------------
// ����:	�����ļ���С
// ����:	�ļ��Ĵ�С in bytes
//---------------------------------------------------------------------------
unsigned long KPakFile::Size()
{
	unsigned int uSize;
//#ifndef _SERVER
	if (m_PackRef.nPackIndex >= 0)
		uSize = m_PackRef.nSize;
	else
//#endif
		uSize = m_File.Size();

	return uSize;
}
//---------------------------------------------------------------------------
// ����:	�ر�һ���ļ�
//---------------------------------------------------------------------------
void KPakFile::Close()
{
	if (m_PackRef.nPackIndex >= 0)
	{
		m_PackRef.nPackIndex = -1;
		m_PackRef.uId = 0;
	}
	else
	{
		m_File.Close();
	}
}

//---------------------------------------------------------------------------
// ÿ�ζ�ȡ���ݿ�Ĵ�С
#define BLOCK_SIZE	(0x10000L)

//---------------------------------------------------------------------------
// ����:	��һ�����е��ļ�
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
/*int KPakFile::OpenPak(char* FileName)
{
	if (g_pPakList == NULL)
		return FALSE;

	KAutoMutex	AutoMutex(g_pPakList->GetMutexPtr());

	// �������ļ����в���Ҫ�򿪵��ļ��Ƿ����
	m_nPackage = g_pPakList->Search(FileName, &m_dwFileOfs, &m_dwFileLen);
	if (m_nPackage < 0)
		return FALSE;

	// m_nBlocks ����ĸ���, Դ�ļ�ÿ64K��Ϊһ����
	// m_nBlocks * 2 Ϊ�鳤�ȱ�Ĵ�С(ÿ��Ĵ�С��һ��unsigned short��¼)
	m_nBlocks = (m_dwFileLen + 0xffff) >> 16;

	// �� block buffer �����ڴ�
	if (!m_MemBlock.Alloc(m_nBlocks * 2))
		return FALSE;

	// �� file buffer �����ڴ�64K, Ϊ��ѹ��׼��
	if (!m_MemFile.Alloc(BLOCK_SIZE))
		return FALSE;

	// �� read buffer �����ڴ�64K, Ϊ��ѹ��׼��
	if (!m_MemRead.Alloc(BLOCK_SIZE))
		return FALSE;

	// �ļ�������ָ��
	m_pBuffer = (PBYTE)m_MemFile.GetMemPtr();

	// ÿ��ĳ��ȱ�
	m_pBlocks = (PWORD)m_MemBlock.GetMemPtr();

	// �ƶ����ļ���ʼ
	g_pPakList->Seek(m_dwFileOfs, FILE_BEGIN);

	// ����ÿ��Ĵ�С
	g_pPakList->Read(m_pBlocks, m_nBlocks * 2);

	// ��һ��ѹ�����ݵ�ƫ����
	m_dwFileOfs = m_dwFileOfs + m_nBlocks * 2;

	// ��ȡѹ��������ʼλ��
	m_dwDataPtr = m_dwFileOfs;

	// ��ָ���λ��(������λ��) = 0;
	m_dwFilePtr = 0;

	// �ɹ����ļ�
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	ReadPak
// ����:	���ļ��ж�ȡ����
// ����:	pBuffer		������ָ��
//			dwSize		Ҫ��ȡ�ĳ���
// ����:	�������ֽڳ���
//---------------------------------------------------------------------------
unsigned long KPakFile::ReadPak(PVOID pBuffer, unsigned long dwSize)
{
	KAutoMutex AutoMutex(g_pPakList->GetMutexPtr());

	// ����ǰ����ļ��ʹӰ��ж�
	UINT	nBlock = 0;
	unsigned long	dwReadSize = 0;
	unsigned long	dwBlockPos = 0;
	PBYTE	pOutBuf = (PBYTE)pBuffer;

	// �����ȡ���ȴ���ʣ���ļ�����
	if (m_dwFilePtr + dwSize > m_dwFileLen)
	{
		dwSize =  m_dwFileLen - m_dwFilePtr;
		dwReadSize = dwSize;
	}
	else
	{
		dwReadSize = dwSize;
	}

	// �Ѿ�����Ŀ���
	nBlock = m_dwFilePtr >> 16;

	// �Ѿ�����С��64K��ĳ���
	dwBlockPos = m_dwFilePtr & 0xffff;

	// ��ǰ����С��64K������
	if (dwBlockPos)
	{
		// Ҫ�������ݳ���С��64K
		if (dwBlockPos + dwSize <= BLOCK_SIZE)
		{
			// m_pBufferΪ64K��С, �ϴ��Ѷ�����
			g_MemCopyMmx(pOutBuf, m_pBuffer + dwBlockPos, dwSize);
			m_dwFilePtr += dwSize;

			// �����ʱ m_dwFilePtr Ϊ64K �ı���
			if ((m_dwFilePtr & 0xffff) == 0)
				m_dwDataPtr += (m_pBlocks[nBlock] == 0)? BLOCK_SIZE : m_pBlocks[nBlock];

			return dwSize;
		}

		// Ҫ�������ݳ��ȴ���64K
		g_MemCopyMmx(pOutBuf, m_pBuffer + dwBlockPos, BLOCK_SIZE - dwBlockPos);
		pOutBuf += BLOCK_SIZE - dwBlockPos;
		m_dwDataPtr += (m_pBlocks[nBlock] == 0)? BLOCK_SIZE : m_pBlocks[nBlock];
		m_dwFilePtr = (++nBlock) << 16;
		dwSize -= (BLOCK_SIZE - dwBlockPos);
	}

	// �������ಿ��
	while (dwSize > 0xffff) // ����64K
	{
		ReadBlock(pOutBuf, nBlock);
		pOutBuf += BLOCK_SIZE;
		m_dwDataPtr += (m_pBlocks[nBlock] == 0)? BLOCK_SIZE : m_pBlocks[nBlock];
		m_dwFilePtr = (++nBlock) << 16;
		dwSize -= BLOCK_SIZE;
	}

	// �պö����򷵻�
	if (dwSize == 0)
	{
		return dwReadSize;
	}

	// ��һ��64K���ݿ鵽������
	ReadBlock(m_pBuffer, nBlock);

	// �������������ݵ�Ŀ���ַ
	g_MemCopyMmx(pOutBuf, m_pBuffer, dwSize);

	// �����ļ�ָ��
	m_dwFilePtr += dwSize;

	// ���ض�ȡ���ֽڳ���
	return dwReadSize;
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	�ļ���ָ�붨λ
// ����:	lOffset			ƫ����
//			dwMethod		��λ����
// ����:	�ļ���ָ��
//---------------------------------------------------------------------------
unsigned long KPakFile::SeekPak(int lOffset, unsigned long dwMethod)
{
	KAutoMutex AutoMutex(g_pPakList->GetMutexPtr());

	if (m_nPackage < 0)
	{
		return m_File.Seek(lOffset, dwMethod);
	}

	int	nFilePtr = m_dwFilePtr;

	switch (dwMethod)
	{
	case FILE_BEGIN:
		nFilePtr = lOffset;
		break;

	case FILE_END:
		nFilePtr = m_dwFileLen + lOffset;
		break;

	case FILE_CURRENT:
		nFilePtr = m_dwFilePtr + lOffset;
		break;
	}

	if (nFilePtr < 0)
	{
		nFilePtr = 0;
	}
	else if (nFilePtr > (int)m_dwFileLen)
	{
		nFilePtr = m_dwFileLen;
	}

	m_dwFilePtr = nFilePtr;
	m_dwDataPtr = m_dwFileOfs;

	int nBlocks = nFilePtr >> 16;
	for (int i = 0; i < nBlocks; ++i)
	{
		m_dwDataPtr += (m_pBlocks[i] == 0)? BLOCK_SIZE : m_pBlocks[i];
	}
	if (nFilePtr & 0xffff)
	{
		ReadBlock(m_pBuffer, nBlocks);
	}

	return m_dwFilePtr;
}

//---------------------------------------------------------------------------
// ����:	Save
// ����:	�����ļ�
// ����:	FileName	�ļ���
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
*/
int KPakFile::Save(const char* pszFileName)
{
/*	if (m_nPackage < 0)
		return TRUE;

	if (!m_File.Create(pszFileName))
		return FALSE;

	unsigned long dwSize = m_dwFileLen;
	int nBlock = 0;

	// set data ptr
	m_dwDataPtr = m_dwFileOfs;

	// read blocks and write to file
	while (dwSize > BLOCK_SIZE)
	{
		ReadBlock(m_pBuffer, nBlock);
		dwSize -= BLOCK_SIZE;
		m_File.Write(m_pBuffer, BLOCK_SIZE);
		m_dwDataPtr += (m_pBlocks[nBlock] == 0)? BLOCK_SIZE : m_pBlocks[nBlock];
		nBlock++;
	}

	// read last block and write to file
	ReadBlock(m_pBuffer, nBlock);
	m_File.Write(m_pBuffer, dwSize);*/

	return FALSE;
}

/*
//---------------------------------------------------------------------------
// ����:	ReadBlock
// ����:	��һ��ѹ�����ݿ�
// ����:	pBuffer		������ָ��
//			nBlock		������
// ����:	void
//---------------------------------------------------------------------------
void KPakFile::ReadBlock(PBYTE pBuffer, int nBlock)
{
	TCodeInfo	CodeInfo;

	// ���õ�ǰʹ�õ��ļ���
	g_pPakList->SetActivePak(m_nPackage);

	// ����ѹ���ӿڽṹ
	CodeInfo.lpPack = (PBYTE)m_MemRead.GetMemPtr();
	CodeInfo.dwPackLen = m_pBlocks[nBlock];
	CodeInfo.lpData = pBuffer;
	CodeInfo.dwDataLen = BLOCK_SIZE;

	// ����Ƿ�ѹ����
	if (CodeInfo.dwPackLen == 0) // û��ѹ��
	{
		g_pPakList->Seek(m_dwDataPtr, FILE_BEGIN);
		g_pPakList->Read(CodeInfo.lpData, CodeInfo.dwDataLen);
		return;
	}

	// ���һ���ʵ�ʳ��ȣ�ֻ��LHA�ã�
	if (nBlock == (m_nBlocks - 1))
	{
		CodeInfo.dwDataLen = m_dwFileLen - nBlock * BLOCK_SIZE;
	}

	// �ƶ�ָ�룬��ȡѹ�����ݣ��ٽ�ѹ��
	g_pPakList->Seek(m_dwDataPtr, FILE_BEGIN);
	g_pPakList->Read(CodeInfo.lpPack, CodeInfo.dwPackLen);
	g_pPakList->Decode(&CodeInfo);
}
//---------------------------------------------------------------------------
*/
