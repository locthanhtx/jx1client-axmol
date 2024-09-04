//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMemManager.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Memory Opration Class Using Heap Memory Functions
//---------------------------------------------------------------------------
#include "KMemBase.h"
#include "KMemManager.h"
//---------------------------------------------------------------------------
 KMemManager g_MemManager;
//---------------------------------------------------------------------------
// ����:	KMemManager
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KMemManager::KMemManager()
{
    int32_t i;
    for (i = 0; i<NUM_BLOCK; i++)
        m_block_size[i] = (1<<(i + MIN_BLOCK)); //��Ĵ�С
	g_MemZero(m_blocks, sizeof(m_blocks));
}
//---------------------------------------------------------------------------
// ����:	~KMemManager
// ����:	��������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KMemManager::~KMemManager()
{
    KChunkHeader* ch;
	KBlockHeader* bh;
	char* bp;
	int32_t i;
	// ����ڴ�й©
    while (ch = (KChunkHeader *)m_chunks.GetHead())
	{
        bp = ((char *)ch) + sizeof(KChunkHeader);
        for (i = 0; i < ch->block_num; i++)
		{
            bh = (KBlockHeader *)bp;
            //if (bh->size != 0)// ��û�ͷŵ��ڴ�飿
			//	g_DebugLog("KMemManager::Leak Detected, Size = %d", bh->size);
			bp += ch->block_size;
        }
        FreeChunk(ch);
    }
}
//---------------------------------------------------------------------------
// ����:	NewChunk()
// ����:	�����µ��ڴ����飬CHUNK_SIZEΪ��λ
// ����:	block size, block number
// ����:	block header
//---------------------------------------------------------------------------
void* KMemManager::NewChunk(int32_t block_size, int32_t block_num)
{
	// ��Ĵ�СҪ���Ͽ�ͷ�Ϳ�β
    block_size = block_size + sizeof(KBlockHeader) + sizeof(KBlockTailer);
	// chunk�Ĵ�СҪ����chunk header
    int32_t chunk_size = sizeof(KChunkHeader) + (block_size * block_num);
    // ����һ����ڴ�
	char* c = (char*)g_MemAlloc(chunk_size);
    if (c)
	{
        // ChunkHeader initialization
        KChunkHeader* ch = (KChunkHeader *)c;
		ch->node.m_pNext = NULL;
		ch->node.m_pPrev = NULL;
        ch->block_num  = block_num;
        ch->block_size = block_size;
        m_chunks.AddTail((KNode *)ch);

        // blocks im Chunk initializtion
        char *f = c + sizeof(KChunkHeader); // first Block
        char *p = c + chunk_size; // last Block + 1
        char *next = NULL;
        while (p > f)
		{
            p -= block_size;
            KBlockHeader *bh = (KBlockHeader *)p;
            bh->next = (void *)next;
            bh->size = 0;
            bh->magic = HEAD_MAGIC;
            next = p;
        }
        return f;
    }
    return NULL;
}
//---------------------------------------------------------------------------
// ����:	FreeChunk()
// ����:	�����µ��ڴ����飬CHUNK_SIZEΪ��λ
// ����:	block size, block number,
// ����:	block header
//---------------------------------------------------------------------------
void KMemManager::FreeChunk(KChunkHeader *ch)
{
	ch->node.Remove();
	g_MemFree((void *)ch);
    ch=NULL;
}
//---------------------------------------------------------------------------
// ����:	Malloc()
// ����:	�����ڴ�
// ����:	size in bytes
// ����:	void*
//---------------------------------------------------------------------------
void* KMemManager::Malloc(int32_t size)
{
    char* p = NULL;

	if (size > (1<<MAX_BLOCK))
	{
        p = (char *)NewChunk(size, 1);
        if (p)
		{
            KBlockHeader *bh = (KBlockHeader *)p;
            KBlockTailer *bp = (KBlockTailer *)(p + size + sizeof(KBlockHeader));
            bh->next = (void *) -1L;// ��ʾֻ��һ��
            bh->size = size;
            bh->magic = HEAD_MAGIC;
            bp->magic = TAIL_MAGIC;
            p += sizeof(KBlockHeader);
        }
    }
	else
	{
        // ��һ����С���ʵ�chunk
        int32_t i, mask;
        for (i=0; i<NUM_BLOCK-1; i++)
		{
            mask = ~(m_block_size[i]-1);
            if ((size & mask) == 0)
                break;
        }
        if (NULL == m_blocks[i])
		{
			m_blocks[i] = NewChunk(m_block_size[i],
				CHUNK_SIZE / m_block_size[i]);
		}
        p = (char *)m_blocks[i];
		KBlockHeader *bh = (KBlockHeader *)p;
		KBlockTailer *bp = (KBlockTailer *)(p + size + sizeof(KBlockHeader));
		m_blocks[i] = bh->next;
		bh->next = (void *)i;
		bh->size = size;
		bh->magic = HEAD_MAGIC;
		bp->magic = TAIL_MAGIC;
		p += sizeof(KBlockHeader);
    }
    return p;
}
//---------------------------------------------------------------------------
// ����:	Calloc()
// ����:	�����ڴ棬���������
// ����:	size in bytes
// ����:	void*
//---------------------------------------------------------------------------
void* KMemManager::Calloc(int32_t size)
{
	void* p = Malloc(size);
	g_MemZero(p, size);
	return p;
}
//---------------------------------------------------------------------------
// ����:	Free()
// ����:	�ͷ��ڴ�
// ����:	void*
// ����:	void
//---------------------------------------------------------------------------
void KMemManager::Free(void* p)
{
	if (NULL == p)
	{
//		g_DebugLog("KMemManager::Free(p); p = NULL!");
		return;
	}
    char* pc = (char *)p;
    KBlockHeader *bh = (KBlockHeader *)(pc - sizeof(KBlockHeader));
    KBlockTailer *bp = (KBlockTailer *)(pc + bh->size);

    // ����Ƿ�����ڴ�Խ�����
    if ((bh->magic != HEAD_MAGIC) || (bp->magic != TAIL_MAGIC))
		messageBox("Memory Corrupted","Free");
        //ccMs("Memory Corrupted(Խ��) : Size = %d", bh->size);

	// ����������ڴ��
    if (((intptr_t)bh->next) == -1L)
	{
        KChunkHeader *ch = (KChunkHeader *)(((char *)bh) - sizeof(KChunkHeader));
        bh->size = 0;
        FreeChunk(ch);
    }
	else
	{
        int32_t i = (intptr_t)bh->next;
        KBlockHeader* next = (KBlockHeader *)m_blocks[i];
        m_blocks[i] = bh;
        bh->next = next;
        bh->size = 0;
    }
}
//---------------------------------------------------------------------------
