//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KSG_StringProcess.h
//  Version     :   1.0
//  Creater     :   Freeeway Chen
//  Date        :   2003-8-1 12:43:32
//  Comment     :   Process String to Int, Skip Symbol
//   �ַ�������
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _KSG_STRING_PROCESS_H
#define _KSG_STRING_PROCESS_H   1
#include "cocos2d.h"

USING_NS_AX;
#include "KbugInfo.h"

int32_t KSG_StringGetInt(const char **ppcszString, int32_t nDefaultValue);
bool KSG_StringSkipSymbol(const char **ppcszString, int32_t nSymbol);

inline int32_t KSG_StringGetInt(char **ppszString, int32_t nDefaultValue)
{
    return KSG_StringGetInt((const char **)ppszString, nDefaultValue);
}

inline bool KSG_StringSkipSymbol(char **ppszString, int32_t nSymbol)
{
    return KSG_StringSkipSymbol((const char **)ppszString, nSymbol);
}


#endif

