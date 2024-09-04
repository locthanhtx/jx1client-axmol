//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KTabFile.h
// Date:	2002.02.18
// Code:	Spe
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KTabFile_H
#define KTabFile_H
#include "cocos2d.h"
#include "KMemClass.h"
#include "KITabFile.h"
//#include "KTabFileCtrl.h"
//---------------------------------------------------------------------------
typedef struct tagTabOffset
{
	uint32_t		dwOffset;
	uint32_t		dwLength;
} TABOFFSET;

class  KTabFile:public KITabFile
{
private:
	int32_t			m_Width;
	int32_t			m_Height;
	KMemClass	m_Memory;
	KMemClass	m_OffsetTable;
	uint32_t	dwSize;
	//uint32_t		dwFileIdx;
	//void *		Buffer;
	//char        nMemKey[32];
private:
	void		CreateTabOffset();
	BOOL		GetValue(int32_t nRow, int32_t nColumn, char * lpRString, uint32_t dwSize);
	int32_t			Str2Col(char * szColumn);
public:
	KTabFile();
	~KTabFile();
//	KTabFileCtrl *TabFileCtrl;
	BOOL		Load(char * FileName/*,char * mMemKey=NULL*/);
	bool        CreatFile(char * FileName);
	BOOL		Save(char * FileName){return false;}; //�޷�����
	BOOL		LoadPack( char * FileName);
	//uint32_t       GetFileDwIdx(){ return dwFileIdx;}

	uint32_t		GetSize(){ return dwSize;};
	int32_t			FindRow( char * szRow);//������1Ϊ����ֵ
	int32_t			FindColumn( char * szColumn);//������1Ϊ����ֵ
	void		Col2Str(int32_t nCol,  char * szColumn);
	int32_t			GetWidth() { return m_Width;};
	int32_t			GetHeight() { return m_Height;};
	// int32_t			GetWidth() { return 1;};
	// int32_t			GetHeight() { return 1;};
	BOOL		GetString(int32_t nRow, char * szColumn, char * lpDefault, char * lpRString, uint32_t dwSize, BOOL bColumnLab = TRUE);
	BOOL		GetString(char * szRow, int32_t szColumn, char * lpDefault, char * lpRString, uint32_t dwSize, BOOL bColumnLab = TRUE);
	BOOL		GetString(int32_t nRow, int32_t nColumn, char * lpDefault, char * lpRString, uint32_t dwSize);
	BOOL		GetString(char * szRow, char * szColumn, char * lpDefault, char * lpRString, uint32_t dwSize);
	BOOL		GetInteger(int32_t nRow, char * szColumn, int32_t nDefault, int32_t *pnValue, BOOL bColumnLab = TRUE);
	BOOL		GetInteger6(int32_t nRow, char * szColumna,char * szColumnb,char * szColumnc,char * szColumnd,char * szColumne,char * szColumnf,int32_t nDefault, int32_t *pnValue, BOOL bColumnLab= TRUE);
	BOOL		GetInteger3(int32_t nRow, char * szColumna,char * szColumnb,char * szColumnc,int32_t nDefault, int32_t *pnValue, BOOL bColumnLab= TRUE);
	BOOL		GetInteger(int32_t nRow, int32_t nColumn, int32_t nDefault, int32_t *pnValue);
	BOOL        GetIntegerDW(int32_t nRow, char * szColumn, int32_t nDefault,uint32_t *pnValue, BOOL bColumnLab = TRUE);
	BOOL		GetInteger(char * szRow, char * szColumn, int32_t nDefault, int32_t *pnValue);
	BOOL		GetFloat(int32_t nRow, char * szColumn, float fDefault, float *pfValue, BOOL bColumnLab = TRUE);
	BOOL		GetDouble(int32_t nRow, char * szColumn, double fDefault, double *pfValue, BOOL bColumnLab = TRUE);
	BOOL		GetDword(int32_t nRow, char * szColumn, uint32_t fDefault, uint32_t *pfValue, BOOL bColumnLab = TRUE);
//	BOOL		GetFloat(char * szRow, char * szColumn, float fDefault, float *pfValue);
	BOOL		GetFloat(int32_t nRow, int32_t nColumn, float fDefault, float *pfValue);
	BOOL		GetFloat(char * szRow, char * szColumn, float fDefault, float *pfValue);
	void        GetInt2(int32_t nRow, int32_t szColumn, int32_t *pRect);
	void        GetInt15(int32_t nRow, int32_t szColumn, int32_t *pRect);
	void        GetInt15s(int32_t nRow, char * szColumn, int32_t *pRect);
	void        GetInt3(int32_t nRow, int32_t szColumn, int32_t *pRect);
	void        GetInt2s(int32_t nRow, char * szColumn, int32_t *pRect);
	void        GetInt3s(int32_t nRow, char * szColumn, int32_t *pRect);
	void		Clear();
};
//---------------------------------------------------------------------------
#endif
