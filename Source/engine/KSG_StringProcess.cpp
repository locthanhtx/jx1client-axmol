//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KSG_StringProcess.cpp
//  Version     :   1.0
//  Creater     :   Freeway Chen
//  Date        :   2003-8-1 12:45:11
//  Comment     :   Process String to Int, Skip Symbol
//
//////////////////////////////////////////////////////////////////////////////////////
#include <ctype.h>
#include "KSG_StringProcess.h"


int32_t KSG_StringGetInt(const char **ppcszString, int32_t nDefaultValue)
{
    int32_t nResult = false;
    int32_t nRetValue = 0;
    int32_t nNegSignFlag = false;
    int32_t nRetValueValidFlag = false;
    const char *pcszString = NULL;

    if (!ppcszString)
        goto Exit0;
    
    pcszString = *ppcszString;

    if (!pcszString)
        goto Exit0;

    while (isspace(*pcszString))
        pcszString++;
    
    if ((*pcszString) == '\0')
        goto Exit0;

    if ((*pcszString) == '-')
    {
        nNegSignFlag = true;
        pcszString++;

        // Skip Prev Space
        while (isspace(*pcszString))
            pcszString++;

        if ((*pcszString) == '\0')
            goto Exit0;
    }

    
    while (isdigit(*pcszString))
    {
        nRetValueValidFlag = true;

        nRetValue = nRetValue * 10 +  ((int32_t)(*pcszString - '0'));

        pcszString++;
    }

    nResult = true;
Exit0:

    if (pcszString)
    {
        if (ppcszString)
            *ppcszString = pcszString;
    }

    if (nNegSignFlag)
        nRetValue = -nRetValue;

    if (
        (!nResult) ||
        (!nRetValueValidFlag)
    )
        nRetValue = nDefaultValue; 


    return nRetValue;
}


bool KSG_StringSkipSymbol(const char **ppcszString, int32_t nSymbol)
{
    bool bResult = false;
    const char *pcszString = NULL;

    if (!ppcszString)
        goto Exit0;
    
    pcszString = *ppcszString;

    if (!pcszString)
        goto Exit0;

    while (isspace(*pcszString))
        pcszString++;
    
    if (((unsigned)(*pcszString)) != (unsigned)nSymbol)
        goto Exit0;

    pcszString++;   // Skip Symbol

    bResult = true;
Exit0:

    if (pcszString)
    {
        if (ppcszString)
            *ppcszString = pcszString;
    }

    return bResult;
}

