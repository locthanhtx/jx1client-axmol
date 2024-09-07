//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KStrBase.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KStrBase_H
#define KStrBase_H
#include "cocos2d.h"

USING_NS_AX;
#include "KbugInfo.h"

//---------------------------------------------------------------------------
int		g_StrLen(const char* lpStr);
char*	g_StrEnd(const char* lpStr);
void	g_StrCpy(char* lpDest, const char* lpSrc);
void	g_StrCpyLen(char* lpDest, const char* lpSrc, int nMaxLen);
void	g_StrCat(char* lpDest, const char* lpSrc);
void	g_StrCatLen(char* lpDest, const char* lpSrc, int nMaxLen);
int	g_StrCmp(const char* lpDest, const char* lpSrc);
int	g_StrCmpLen(const char* lpDest, const char* lpSrc, int nMaxLen);
void	g_StrUpper(char* lpDest);
void	g_StrLower(char* lpDest);
void g_StrRep(char* lpDest, const char* lpSrc, const char* lpRep);
UINT    g_Atoui(char* str);  //���ַ���ר��UINT
void    g_StrCopy(std::string nSdcardPath, char *pMusicName);
int     g_ExtractChar(const char *inStr,char scrchar,char destchar,char *outStr=NULL,char *outStra=NULL,int nMoedel=0);
int     g_Round2Int(double d);
float   g_Int2Round(int d);
int     GetBig5Count(char *str);

int t_snprintf(char *buf, unsigned int count, const char *format, ... );
int t_sprintf(char *out, const char *format, ...);
int t_printf(const char *format, ...);
bool __CreateFolder(std::string nSdcardPath,char * nPath);
unsigned int _ccHash(const char *key);
//---------------------------------------------------------------------------
#endif
