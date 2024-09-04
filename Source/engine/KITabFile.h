#ifndef __KITABFILE_H__
#define __KITABFILE_H__
#include "KEngine.h"

class  KITabFile//TabFile�ӿ���
{
public:
	KITabFile(){};
	virtual  ~KITabFile() {};

	virtual BOOL		Load(char * FileName/*,char * mMemKey=NULL*/) = 0;
	virtual BOOL		Save(char * FileName) = 0;
	virtual bool        CreatFile(char * FileName)=0;
	virtual int32_t			FindRow(char * szRow) = 0;
	virtual int32_t			FindColumn(char * szColumn)	=0;
	virtual int32_t			GetWidth() 	=0;
	virtual int32_t			GetHeight() =0;
	virtual BOOL		GetString(int32_t nRow, char * szColumn, char * lpDefault, char * lpRString, uint32_t dwSize, BOOL bColumnLab = true)	=0;
	virtual BOOL		GetString(int32_t nRow, int32_t nColumn, char * lpDefault, char * lpRString, uint32_t dwSize)	=0;
	virtual BOOL		GetString(char * szRow, char * szColumn, char * lpDefault, char * lpRString, uint32_t dwSize)	=0;
	virtual BOOL		GetInteger(int32_t nRow, char * szColumn, int32_t nDefault, int32_t *pnValue, BOOL bColumnLab = true)	=0;
	virtual BOOL		GetInteger(int32_t nRow, int32_t nColumn, int32_t nDefault, int32_t *pnValue)	=0;
	virtual BOOL		GetInteger(char * szRow, char * szColumn, int32_t nDefault, int32_t *pnValue)	=0;
	virtual BOOL		GetFloat(int32_t nRow, char * szColumn, float fDefault, float *pfValue, BOOL bColumnLab = true)	=0;
	virtual BOOL		GetFloat(int32_t nRow, int32_t nColumn, float fDefault, float *pfValue)	=0;
	virtual BOOL		GetFloat(char * szRow, char * szColumn, float fDefault, float *pfValue)	=0;
//	virtual void        GetInt2(int32_t nRow, int32_t szColumn, int32_t *pRect)=0;
//	virtual	void        GetInt3(int32_t nRow, int32_t szColumn, int32_t *pRect)=0;
//	virtual	void        GetInt2(int32_t nRow, char * szColumn, int32_t *pRect)=0;
//	virtual	void        GetInt3(int32_t nRow, char * szColumn, int32_t *pRect)=0;
	virtual void		Clear()	=0;


};

#endif //__KITABFILE_H__
