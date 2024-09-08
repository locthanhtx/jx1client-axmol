// ***************************************************************************************
// Chỉnh sửa lại bởi duccom0123 07/09/2024 6:42:22 CH
// ***************************************************************************************
#include "KFilePath.h"
#include "KPakFile.h"
#include "KPakList.h"

#ifdef _SERVER
static int m_nPakFileMode = 1;
#else
static int m_nPakFileMode = 1;
#endif
void g_SetPakFileMode(int nFileMode)
{
    m_nPakFileMode = nFileMode;
}

#define PAK_INDEX_STORE_IN_RESERVED 0

SPRHEAD* SprGetHeader(char* pszFileName, SPROFFS*& pOffsetTable)
{
    pOffsetTable = NULL;

    if (pszFileName == NULL || pszFileName[0] == 0)
        return NULL;

    KPakFile File;
    if (!File.Open(pszFileName))  
    {
        
        
        
        return NULL;
    }

    SPRHEAD* pSpr = NULL;
    if (File.IsFileInPak())  
    {
        
        XPackElemFileRef PakRef;
        
        if (g_pPakList->pGetFilePath(pszFileName, PakRef))
        {
            pSpr = g_pPakList->GetSprHeader(PakRef, pOffsetTable);
            if (pSpr)
                pSpr->Reserved[PAK_INDEX_STORE_IN_RESERVED] = (unsigned short)(short)PakRef.nPackIndex;
        }
    }
    else
    {
        
        bool bOk = false;
        SPRHEAD Header;
        
        while (File.Read(&Header, sizeof(SPRHEAD)) == sizeof(SPRHEAD))
        {
            if (*(int*)&Header.Comment[0] != SPR_COMMENT_FLAG || Header.Colors > 256)  
                break;
            
            int uEntireSize = File.Size();
            pSpr            = (SPRHEAD*)malloc(uEntireSize);
            if (pSpr == NULL)
                break;

            uEntireSize -= sizeof(SPRHEAD);
            
            if (File.Read(&pSpr[1], uEntireSize) == uEntireSize)
            {
                
                pOffsetTable = (SPROFFS*)(((char*)(pSpr)) + sizeof(SPRHEAD) + Header.Colors * 3);
                Header.Reserved[PAK_INDEX_STORE_IN_RESERVED] = (unsigned short)(-1);
                memcpy(pSpr, &Header, sizeof(SPRHEAD));
                bOk = true;
            }
            break;
        };

        if (bOk == false && pSpr)
        {
            free(pSpr);
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
        
    }
}

SPRFRAME* SprGetFrame(SPRHEAD* pSprHeader, int nFrame, unsigned int& nSingFrameSize)
{
    SPRFRAME* pFrame = NULL;
    if (pSprHeader && g_pPakList)
    {
        int nPakIndex = (short)pSprHeader->Reserved[PAK_INDEX_STORE_IN_RESERVED];
        if (nPakIndex >= 0)
        {
            pFrame = g_pPakList->GetSprFrame(nPakIndex, pSprHeader, nFrame, nSingFrameSize);
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

void release_image(KSGImageContent* pImage)
{
    if (pImage)
        free(pImage);
}

KPakFile::KPakFile()
{
    
    m_PackRef.nPackIndex = -1;
    m_PackRef.uId        = 0;
    
}

KPakFile::~KPakFile()
{
    Close();
}

bool KPakFile::IsFileInPak()
{
    return (m_PackRef.nPackIndex >= 0 && m_PackRef.uId);
}

int KPakFile::Open(const char* pszFileName)
{
    if (pszFileName == NULL || pszFileName[0] == 0)
        return false;
    bool bOk = false;
    Close();
    // duccom0123 chỉnh để luôn load file ở ngoài trước rồi mới load trong pak nếu không có
    bOk = (m_File.Open((char*)pszFileName) != FALSE);
    if (bOk == false && g_pPakList)
    {
        bOk = g_pPakList->pGetFilePath(pszFileName, m_PackRef);
    }

    return bOk;
}

unsigned long KPakFile::Read(void* pBuffer, unsigned int uSize)
{

    
    if (m_PackRef.nPackIndex >= 0)  
    {
        

        if (g_pPakList->ElemFileRead(m_PackRef, pBuffer, uSize) == false)
        {
            
            uSize = 0;
        }
    }
    else
    
    {
        uSize = m_File.Read(pBuffer, uSize);
    }
    return uSize;
}

unsigned long KPakFile::Seek(int nOffset, unsigned int uMethod)
{
    
    if (m_PackRef.nPackIndex >= 0)
    {
        if (uMethod == FILE_BEGIN)
            m_PackRef.nOffset = nOffset;
        else if (uMethod == FILE_END)
            m_PackRef.nOffset = m_PackRef.nSize + nOffset;
        else
            m_PackRef.nOffset += nOffset;
        if (m_PackRef.nOffset > m_PackRef.nSize)
            m_PackRef.nOffset = m_PackRef.nSize;
        else if (m_PackRef.nOffset < 0)
            m_PackRef.nOffset = 0;
        nOffset = m_PackRef.nOffset;
    }
    else
    
    {
        nOffset = m_File.Seek(nOffset, uMethod);
    }
    return nOffset;
}

unsigned long KPakFile::Tell()
{
    int nOffset;
    
    if (m_PackRef.nPackIndex >= 0)
        nOffset = m_PackRef.nOffset;
    else
        
        nOffset = m_File.Tell();

    return nOffset;
}

unsigned long KPakFile::Size()
{
    unsigned int uSize;
    
    if (m_PackRef.nPackIndex >= 0)
        uSize = m_PackRef.nSize;
    else
        
        uSize = m_File.Size();

    return uSize;
}
void KPakFile::Close()
{
    if (m_PackRef.nPackIndex >= 0)
    {
        m_PackRef.nPackIndex = -1;
        m_PackRef.uId        = 0;
    }
    else
    {
        m_File.Close();
    }
}

#define BLOCK_SIZE (0x10000L)

/*int KPakFile::OpenPak(char* FileName)
{
        if (g_pPakList == NULL)
                return FALSE;

        KAutoMutex	AutoMutex(g_pPakList->GetMutexPtr());

        
        m_nPackage = g_pPakList->Search(FileName, &m_dwFileOfs, &m_dwFileLen);
        if (m_nPackage < 0)
                return FALSE;

        
        
        m_nBlocks = (m_dwFileLen + 0xffff) >> 16;

        
        if (!m_MemBlock.Alloc(m_nBlocks * 2))
                return FALSE;

        
        if (!m_MemFile.Alloc(BLOCK_SIZE))
                return FALSE;

        
        if (!m_MemRead.Alloc(BLOCK_SIZE))
                return FALSE;

        
        m_pBuffer = (PBYTE)m_MemFile.GetMemPtr();

        
        m_pBlocks = (PWORD)m_MemBlock.GetMemPtr();

        
        g_pPakList->Seek(m_dwFileOfs, FILE_BEGIN);

        
        g_pPakList->Read(m_pBlocks, m_nBlocks * 2);

        
        m_dwFileOfs = m_dwFileOfs + m_nBlocks * 2;

        
        m_dwDataPtr = m_dwFileOfs;

        
        m_dwFilePtr = 0;

        
        return TRUE;
}
unsigned long KPakFile::ReadPak(PVOID pBuffer, unsigned long dwSize)
{
        KAutoMutex AutoMutex(g_pPakList->GetMutexPtr());

        
        UINT	nBlock = 0;
        unsigned long	dwReadSize = 0;
        unsigned long	dwBlockPos = 0;
        PBYTE	pOutBuf = (PBYTE)pBuffer;

        
        if (m_dwFilePtr + dwSize > m_dwFileLen)
        {
                dwSize =  m_dwFileLen - m_dwFilePtr;
                dwReadSize = dwSize;
        }
        else
        {
                dwReadSize = dwSize;
        }

        
        nBlock = m_dwFilePtr >> 16;

        
        dwBlockPos = m_dwFilePtr & 0xffff;

        
        if (dwBlockPos)
        {
                
                if (dwBlockPos + dwSize <= BLOCK_SIZE)
                {
                        
                        g_MemCopyMmx(pOutBuf, m_pBuffer + dwBlockPos, dwSize);
                        m_dwFilePtr += dwSize;

                        
                        if ((m_dwFilePtr & 0xffff) == 0)
                                m_dwDataPtr += (m_pBlocks[nBlock] == 0)? BLOCK_SIZE : m_pBlocks[nBlock];

                        return dwSize;
                }

                
                g_MemCopyMmx(pOutBuf, m_pBuffer + dwBlockPos, BLOCK_SIZE - dwBlockPos);
                pOutBuf += BLOCK_SIZE - dwBlockPos;
                m_dwDataPtr += (m_pBlocks[nBlock] == 0)? BLOCK_SIZE : m_pBlocks[nBlock];
                m_dwFilePtr = (++nBlock) << 16;
                dwSize -= (BLOCK_SIZE - dwBlockPos);
        }

        
        while (dwSize > 0xffff) 
        {
                ReadBlock(pOutBuf, nBlock);
                pOutBuf += BLOCK_SIZE;
                m_dwDataPtr += (m_pBlocks[nBlock] == 0)? BLOCK_SIZE : m_pBlocks[nBlock];
                m_dwFilePtr = (++nBlock) << 16;
                dwSize -= BLOCK_SIZE;
        }

        
        if (dwSize == 0)
        {
                return dwReadSize;
        }

        
        ReadBlock(m_pBuffer, nBlock);

        
        g_MemCopyMmx(pOutBuf, m_pBuffer, dwSize);

        
        m_dwFilePtr += dwSize;

        
        return dwReadSize;
}
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

*/
int KPakFile::Save(const char* pszFileName)
{
    /*	if (m_nPackage < 0)
                    return TRUE;

            if (!m_File.Create(pszFileName))
                    return FALSE;

            unsigned long dwSize = m_dwFileLen;
            int nBlock = 0;

            
            m_dwDataPtr = m_dwFileOfs;

            
            while (dwSize > BLOCK_SIZE)
            {
                    ReadBlock(m_pBuffer, nBlock);
                    dwSize -= BLOCK_SIZE;
                    m_File.Write(m_pBuffer, BLOCK_SIZE);
                    m_dwDataPtr += (m_pBlocks[nBlock] == 0)? BLOCK_SIZE : m_pBlocks[nBlock];
                    nBlock++;
            }

            
            ReadBlock(m_pBuffer, nBlock);
            m_File.Write(m_pBuffer, dwSize);*/

    return FALSE;
}

/*
void KPakFile::ReadBlock(PBYTE pBuffer, int nBlock)
{
        TCodeInfo	CodeInfo;

        
        g_pPakList->SetActivePak(m_nPackage);

        
        CodeInfo.lpPack = (PBYTE)m_MemRead.GetMemPtr();
        CodeInfo.dwPackLen = m_pBlocks[nBlock];
        CodeInfo.lpData = pBuffer;
        CodeInfo.dwDataLen = BLOCK_SIZE;

        
        if (CodeInfo.dwPackLen == 0) 
        {
                g_pPakList->Seek(m_dwDataPtr, FILE_BEGIN);
                g_pPakList->Read(CodeInfo.lpData, CodeInfo.dwDataLen);
                return;
        }

        
        if (nBlock == (m_nBlocks - 1))
        {
                CodeInfo.dwDataLen = m_dwFileLen - nBlock * BLOCK_SIZE;
        }

        
        g_pPakList->Seek(m_dwDataPtr, FILE_BEGIN);
        g_pPakList->Read(CodeInfo.lpPack, CodeInfo.dwPackLen);
        g_pPakList->Decode(&CodeInfo);
}
*/
