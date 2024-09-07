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
	unsigned int		m_dwLen;	// File Size
	unsigned int		m_dwPos;	// File Pointer
public:
	KFile();
	~KFile();
	bool		Open(char *FileName);
	bool		Create(const char *FileName);
	bool		Append(char * FileName);
	void		Close();
	unsigned int Read(void *lpBuffer,unsigned int dwReadBytes);
	unsigned int Write(void * lpBuffer, unsigned int dwWriteBytes);
	unsigned int Seek(int lDistance, unsigned int dwMoveMethod);
	unsigned int Tell();
	unsigned int Size();
	FILE * getFilePtr(){return m_hFile;};
};
//---------------------------------------------------------------------------
#endif
