//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KFilePath.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KFilePath_H
#define KFilePath_H
#include "cocos2d.h"
//
#include "KbugInfo.h"
USING_NS_AX;
using namespace std;
//---------------------------------------------------------------------------
#ifndef MAXPATH
#    define MAXPATH  260
#    define MAXDIR   256
#    define MAXFILE  256
#    define MAXEXT   256
#    define MAXDRIVE 3
#endif
//---------------------------------------------------------------------------
void g_SetRootPath(char* lpPathName = NULL, bool isChange = false);
void g_GetRootPath(char* lpPathName);
void g_SetFilePath(const char* lpPathName);
void g_GetFilePath(char* lpPathName);
void g_GetFullPath(char* lpPathName, const char* lpFileName);
void g_GetHalfPath(char* lpPathName, char* lpFileName);
void g_GetPackPath(char* lpPathName, char* lpFileName);
void g_GetDiskPath(char* lpPathName, char* lpFileName);
void g_CreatePath(char* lpPathName);
// һ��·����һ���ļ������ϲ���lpGet���γ�һ��������·���ļ���
void g_UnitePathAndName(const char* lpPath, const char* lpFile, char* lpGet);

//---------------------------------------------------------------------------
int g_FileExists(char* lpPathName);
unsigned int g_FileName2Id(char* lpFileName);  // unsigned long
void g_ChangeFileExt(char* lpFileName, char* lpNewExt);
void g_ExtractFileName(char* lpFileName, char* lpFilePath);
void g_ExtractFilePath(char* lpPathName, char* lpFilePath);
unsigned int g_CheckFileExist(const char* lpFileName);
//---------------------------------------------------------------------------
#endif
