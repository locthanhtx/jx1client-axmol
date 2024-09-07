#ifndef __KITABFILE_H__
#define __KITABFILE_H__
#include "KEngine.h"

class  KITabFile//TabFile�ӿ���
{
public:
	KITabFile(){};
	virtual  ~KITabFile() {};

	virtual int		Load(char * FileName/*,char * mMemKey=NULL*/) = 0;
	virtual int		Save(char * FileName) = 0;
	virtual bool        CreatFile(char * FileName)=0;
	virtual int			FindRow(char * szRow) = 0;
	virtual int			FindColumn(char * szColumn)	=0;
	virtual int			GetWidth() 	=0;
	virtual int			GetHeight() =0;
	virtual int		GetString(int nRow, char * szColumn, char * lpDefault, char * lpRString, unsigned int dwSize, int bColumnLab = true)	=0;
	virtual int		GetString(int nRow, int nColumn, char * lpDefault, char * lpRString, unsigned int dwSize)	=0;
	virtual int		GetString(char * szRow, char * szColumn, char * lpDefault, char * lpRString, unsigned int dwSize)	=0;
	virtual int		GetInteger(int nRow, char * szColumn, int nDefault, int *pnValue, int bColumnLab = true)	=0;
	virtual int		GetInteger(int nRow, int nColumn, int nDefault, int *pnValue)	=0;
	virtual int		GetInteger(char * szRow, char * szColumn, int nDefault, int *pnValue)	=0;
	virtual int		GetFloat(int nRow, char * szColumn, float fDefault, float *pfValue, int bColumnLab = true)	=0;
	virtual int		GetFloat(int nRow, int nColumn, float fDefault, float *pfValue)	=0;
	virtual int		GetFloat(char * szRow, char * szColumn, float fDefault, float *pfValue)	=0;
//	virtual void        GetInt2(int nRow, int szColumn, int *pRect)=0;
//	virtual	void        GetInt3(int nRow, int szColumn, int *pRect)=0;
//	virtual	void        GetInt2(int nRow, char * szColumn, int *pRect)=0;
//	virtual	void        GetInt3(int nRow, char * szColumn, int *pRect)=0;
	virtual void		Clear()	=0;


};

#endif //__KITABFILE_H__
