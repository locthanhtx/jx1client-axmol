//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMemManager.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KMemManager_H
#define KMemManager_H
//---------------------------------------------------------------------------
#include "KList.h"
#include "KNode.h"
//---------------------------------------------------------------------------
class  KMemManager
{
private:
	struct KChunkHeader
	{
		KNode node;// node for link
		int32_t block_num;// Block number
		int32_t block_size;// Block size
	};
	struct KBlockHeader
	{
		void* next;// next block
		int32_t size;// block size
		int32_t magic;// header magic code
	};
	struct KBlockTailer
	{
		int32_t magic;// tail magic code
	};
    enum
	{
        MIN_BLOCK = 4,  // min block size is 2^4
		MAX_BLOCK = 10, // max block size is 2^10
		NUM_BLOCK = ((MAX_BLOCK-MIN_BLOCK)+1),
		HEAD_MAGIC = 0xDEADBEEF, // header magic code
		TAIL_MAGIC = 0xFEEBDAED, // tailer magic code
		CHUNK_SIZE = 16 * 1024, // chunk size is 16kb
    };
    KList m_chunks;
    void* m_blocks[NUM_BLOCK];
    int32_t m_block_size[NUM_BLOCK];
    void* NewChunk(int32_t block_size, int32_t block_num);
    void FreeChunk(KChunkHeader *ch);
	void InitBlock();
	void CheckLeak();
public:
    KMemManager();
    ~KMemManager();
    void* Malloc(int32_t size);// ������ڴ棨���ó�ʼ����
    void* Calloc(int32_t size);// ������ڴ棨�����ʼ����
    void Free(void* p);
};
extern  KMemManager g_MemManager;
//---------------------------------------------------------------------------
#endif
