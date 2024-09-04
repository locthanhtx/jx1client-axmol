//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KFile.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KFile_H
#define KFile_H
#include "cocos2d.h"

USING_NS_AX;
//---------------------------------------------------------------------------
#define SEEK_ERROR		0xFFFFFFFF
//---------------------------------------------------------------------------
class KFile
{
private:
//#ifndef __linux
//	HANDLE		m_hFile;	// File Handle
//#else
	FILE *		m_hFile;	// File Handle
//#endif
	uint32_t		m_dwLen;	// File Size
	uint32_t		m_dwPos;	// File Pointer
public:
	KFile();
	~KFile();
	bool		Open(char *FileName);
	bool		Create(char *FileName);
	bool		Append(char * FileName);
	void		Close();
	uint32_t Read(void *lpBuffer,uint32_t dwReadBytes);
	uint32_t Write(void * lpBuffer, uint32_t dwWriteBytes);
	uint32_t Seek(int32_t lDistance, uint32_t dwMoveMethod);
	uint32_t Tell();
	uint32_t Size();
	FILE * getFilePtr(){return m_hFile;};
};
//---------------------------------------------------------------------------
#endif
