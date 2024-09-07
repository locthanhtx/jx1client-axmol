//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KStrBase.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	String Utility Functions
//---------------------------------------------------------------------------
#include "KMemBase.h"
#include "KStrBase.h"
#include "KPaklist.h"
#ifndef WIN32
#    include "KGbktoUtf8.h"
#    include <ctype.h>
#    include <sys/stat.h>
#    include <sys/types.h>
#    include <dirent.h>
#endif
#ifdef WIN32
#    include <direct.h>
#endif
#include <string.h>
#include <stdarg.h>
// isgraph ����ַ��Ƿ�Ϊ����ʾ�ַ���������
// ispunct ����ַ��Ƿ�Ϊ�����ţ�������
// isalnum ����ַ��Ƿ�����ĸ�������֣�������
// isspace ����ַ��Ƿ�Ϊ�հף�������
// isprint �ж��Ƿ��ӡ�ַ� isdigit
// isxdigit ������c�Ƿ�Ϊ16 �������֣�ֻҪcΪ��������һ������ͼ��ɹ� 0123456789ABCDEF
// isalnum,isalpha,isdigit,isxdigit,iscntrl,isgraph,isprint,ispunct

int GetBig5Count(char* str)
{
    int lnBIG5    = 0;  // ���ͳ�ƿ����Ƿ����ֵĺ��ָ���
    int lnGB      = 0;  // ���ͳ�ƿ����Ǽ����ֵĺ��ָ���
    int liTranLen = strlen(str);

    for (int liT = 0; liT < liTranLen - 1; liT++)
    {
        // β�ֽ�40-7E��BGI5�����е�,���ɨ�赽���ֱ���˵������Ԫ���Ƿ���(������:������,�����Ǻ��ֵ�����һ��������Ӣ�ı�����϶��ɵ�)
        if ((BYTE)(BYTE)str[liT] >= 161 && (BYTE)(BYTE)str[liT] <= 254 && (BYTE)(BYTE)str[liT + 1] >= 64 &&
            (BYTE)(BYTE)str[liT + 1] <= 126)
            lnBIG5++;

        if ((BYTE)(BYTE)str[liT] >= 129 && (BYTE)(BYTE)str[liT] <= 160 && (BYTE)(BYTE)str[liT + 1] >= 64 &&
            (BYTE)(BYTE)str[liT + 1] <= 126)
            lnBIG5++;

        if ((BYTE)(BYTE)str[liT] >= 129 && (BYTE)(BYTE)str[liT] <= 160 && (BYTE)(BYTE)str[liT + 1] >= 161 &&
            (BYTE)(BYTE)str[liT + 1] <= 254)
            lnBIG5++;

        // ���ֽ�A4-A9��GB��Ϊ���ļ���,ϣ����ĸ,������ĸ���Ʊ��,�����ı��к��ٳ���,�������Χ��BIG5�ĳ��ú���,������Ϊ����BIG5��
        if ((BYTE)(BYTE)str[liT] >= 164 && (BYTE)(BYTE)str[liT] <= 169 && (BYTE)(BYTE)str[liT + 1] >= 161 &&
            (BYTE)(BYTE)str[liT + 1] <= 254)
            lnBIG5++;

        // GB�����ֽ�AA-AFû�ж���,�������ֽ�λ�AA-AF֮��,β�ֽ�λ�A1-FE�ı��뼸��100%��BIG5(������:û��100%),��Ϊ��BIG5��
        /*if((BYTE)(BYTE)str[liT]>=170 && (BYTE)(BYTE)str[liT]<=175 && (BYTE)(BYTE)str[liT+1]>=161 &&
           (BYTE)(BYTE)str[liT+1]<=254) lnBIG5 ++;
        */
        // ���ֽ�C6-D7,β�ֽ�A1-FE��GB�����һ���ֿ�,�ǳ��ú���,����BIG5��,C6-C7û����ȷ����,��ͨ�����������ļ��������,C8-D7��춺��ú�����,���Կ���Ϊ��GB��
        // if((BYTE)(BYTE)str[liT]>=196 && (BYTE)(BYTE)str[liT]<=215 && (BYTE)(BYTE)str[liT+1]>=161 &&
        // (BYTE)(BYTE)str[liT+1]<=254) 	lnGB ++;
    }

    // ���ɨ����������Ԫ��,�����Ǽ����ֵ���Ŀ�ȿ����Ƿ����ֵ���Ŀ�����Ϊ�Ǽ����ֲ�ת��(��һ��׼ȷ)
    // return lnBIG5-lnGB;
    return lnBIG5;
}

UINT g_Atoui(char* str)
{
    UINT result = 0, i = 0;

    char* tmp = NULL;

    for (i = 0; isspace(str[i]) && i < strlen(str); i++)  // �����հ׷�;
        ;
    tmp = str + i;

    while (*tmp)
    {
        result = result * 10 + *tmp - '0';
        tmp++;
    }

    return result;
}

int g_Round2Int(double d)
{
    return static_cast<int>(d + (d > 0.0 ? 0.5 : -0.5));
}

float g_Int2Round(int d)
{
    return static_cast<float>(0.0 + d);
}

int g_ExtractChar(const char* inStr, char scrchar, char destchar, char* outStr, char* outStra, int nMoedel)
{
    char *tmp = NULL, *tmpa = NULL;  // ����һ����ʱ����ռ䣬����ַ���
    tmp = outStr, tmpa = outStra;
    int nLen = 0;
    while (*inStr != '\0')
    {  // һֱǰ��,ֱ���н������Ž�ֹͣ��
        if (nMoedel == 0 && *inStr == scrchar)
        {             // ��ȡ�����ַ�֮������� ����� tmpa��
            inStr++;  // ���˵���ʼ�ַ���ָ���¸������бȽϡ�
            while (*inStr != '\0')
            {                            // ����ǰ��
                if (*inStr == destchar)  // �����еĵڶ���Ԫ��������ַ���ͬ
                {
                    //*inStr='|';       //�滻�������ַ�
                    inStr++;  // ���˵���������ָ���¸�����
                    break;    // �ҵ��������� ���˵� ����ѭ����
                }
                *tmpa = *inStr;  // ��ȡ�ַ��Ž�tempa��
                tmpa++;          // ��һ��Ԫ��
                inStr++;         // ��һ��Ԫ��
            }
            *tmpa = '\0';  // ��ȡ��󣬼Ӹ���������
        }
        else if (nMoedel == 1 && *inStr == scrchar)
        {  // ��ȡ�ַ�ǰ������� ����� tmp��
            break;
        }
        else if (nMoedel == 2 && *inStr == scrchar)
        {             // ��ȡ�ַ���������� ����� tmpa��
            inStr++;  // ���˵���ʼ�ַ���ָ���¸������бȽϡ�
            while (*inStr != '\0')
            {  // ����ǰ��,������ȡ���������
                /*if (*inStr==destchar)   //�����еĵڶ���Ԫ��������ַ���ͬ
                                {
                                        //*inStr='|';       //�滻�������ַ�
                    inStr++;            //���˵���������ָ���¸�����
                                        break;              //�ҵ��������� ���˵� ����ѭ����
                                }*/
                *tmpa = *inStr;  // ��ȡ�ַ��Ž�tempa��
                tmpa++;          // ��һ��Ԫ��
                inStr++;         // ��һ��Ԫ��
            }
            *tmpa = '\0';  // ��ȡ��󣬼Ӹ���������
        }

        if (*inStr == '\0')  // ���û���ҵ��������������Ѿ����˽�β���������ˣ���
            break;

        *tmp = *inStr;  // Ԫ������ͬ�����ŵ�tmp�С�
        tmp++;          // ������һ��Ԫ���ıȽ�
        inStr++;
        nLen++;
    }
    *tmp = '\0';  // Դ�ַ��� ��ȡ��� �Ӹ������� ��ֹ����
    return nLen;
}

/*UINT g_Atoui(char* str)
{
        UINT result = 0, i = 0;

        char *tmp = NULL;

        for (i = 0; isspace(str[i]) && i < strlen(str); i++)//�����հ׷�;
                ;
        tmp = str+i;

        while (*tmp)
        {
                result = result * 10 + *tmp - '0';
                tmp++;
        }

        return result;
}*/

int g_StrLen(const char* lpStr)
{
    /*#ifdef WIN32
            int nLen;

            __asm
            {
                    mov		edi, lpStr
                    mov		ecx, 0xffffffff
                    xor		al, al
                    repne	scasb
                    not		ecx
                    dec		ecx
                    mov		nLen, ecx
            }
            return nLen;
    #else*/
    return strlen(lpStr);
    // #endif
}
//---------------------------------------------------------------------------
// ����:	StrEnd
// ����:	�����ַ�����βָ��
// ����:	lpStr	:	�ַ�����ͷ��ָ��
// ����:	lpEnd	:	�ַ���ĩβ��ָ��
//---------------------------------------------------------------------------
char* g_StrEnd(const char* lpStr)
{
    /*#ifdef WIN32
            char* lpEnd;

            __asm
            {
                    mov		edi, lpStr
                    mov		ecx, 0xffffffff
                    xor		al, al
                    repne	scasb
                    lea		eax, [edi - 1]
                    mov		lpEnd, eax
            }
            return lpEnd;
    #else*/
    return (char*)lpStr + strlen(lpStr);
    // #endif
}
//---------------------------------------------------------------------------
// ����:	StrCpy
// ����:	�ַ�������
// ����:	lpDest	:	Ŀ���ַ���
//			lpSrc	:	Դ�ַ���
// ����:	void
//---------------------------------------------------------------------------
void g_StrCpy(char* lpDest, const char* lpSrc)
{

    //	sprintf(lpDest,"%s",lpSrc);
    sprintf(lpDest, "%s", lpSrc);

}
//---------------------------------------------------------------------------
// ����:	StrCpyLen
// ����:	�ַ�������,����󳤶�����
// ����:	lpDest	:	Ŀ���ַ���
//			lpSrc	:	Դ�ַ���
//			nMaxLen	:	��󳤶�
// ����:	void
//---------------------------------------------------------------------------
void g_StrCpyLen(char* lpDest, const char* lpSrc, int nMaxLen)
{
    // strncpy(lpDest, lpSrc, nMaxLen);
    if (nMaxLen > 0)
        sprintf(lpDest, "%s", lpSrc);
    else
        sprintf(lpDest, "%s", lpSrc);
}
//---------------------------------------------------------------------------
// ����:	StrCat
// ����:	�ַ���ĩβ׷����һ���ַ���
// ����:	lpDest	:	Ŀ���ַ���
//			lpSrc	:	Դ�ַ���
// ����:	void
//---------------------------------------------------------------------------
void g_StrCat(char* lpDest, const char* lpSrc)
{
    char* lpEnd;

    lpEnd = g_StrEnd(lpDest);
    g_StrCpy(lpEnd, lpSrc);
}
//---------------------------------------------------------------------------
// ����:	StrCatLen
// ����:	�ַ���ĩβ׷����һ���ַ���,����󳤶�����
// ����:	lpDest	:	Ŀ���ַ���
//			lpSrc	:	Դ�ַ���
//			nMaxLen	:	��󳤶�
// ����:	void û�е���
//---------------------------------------------------------------------------
void g_StrCatLen(char* lpDest, const char* lpSrc, int nMaxLen)
{
    char* lpEnd;

    lpEnd = g_StrEnd(lpDest);
    g_StrCpyLen(lpEnd, lpSrc, nMaxLen);
}
//---------------------------------------------------------------------------
// ����:	StrCmp
// ����:	�ַ����Ƚ�
// ����:	lpDest	:	�ַ���1
//			lpSrc	:	�ַ���2
// ����:	TRUE	:	��ͬ
//			FALSE	:	��ͬ
//---------------------------------------------------------------------------
int g_StrCmp(const char* lpDest, const char* lpSrc)
{
    int nLen1, nLen2;

    nLen1 = g_StrLen(lpDest);
    nLen2 = g_StrLen(lpSrc);
    if (nLen1 != nLen2)
        return FALSE;
    return g_MemComp((void*)lpDest, (void*)lpSrc, nLen1);
}
//---------------------------------------------------------------------------
// ����:	StrCmpLen
// ����:	�ַ����Ƚ�,�޶�����
// ����:	lpDest	:	�ַ���1
//			lpSrc	:	�ַ���2
//			nLen	:	����
// ����:	TRUE	:	��ͬ
//			FALSE	:	��ͬ
//---------------------------------------------------------------------------
int g_StrCmpLen(const char* lpDest, const char* lpSrc, int nMaxLen)
{
    int nLen1, nLen2;

    nLen1 = g_StrLen(lpDest);
    nLen2 = g_StrLen(lpSrc);
    if (nMaxLen > nLen1)
        nMaxLen = nLen1;
    if (nMaxLen > nLen2)
        nMaxLen = nLen2;
    return g_MemComp((void*)lpDest, (void*)lpSrc, nMaxLen);
}
//---------------------------------------------------------------------------
// ����:	StrUpper
// ����:	Сд��ĸת��д��ĸ
// ����:	lpDest	:	�ַ���
// ����:	void
//---------------------------------------------------------------------------
void g_StrUpper(char* lpDest)
{
    /*#ifdef WIN32
            __asm
            {
                    mov		esi, lpDest
    loc_lodsb:
                    lodsb
                    or		al, al
                    je		loc_exit
                    cmp		al, 'a'
                    jb		loc_lodsb
                    cmp		al, 'z'
                    ja		loc_lodsb
                    sub		al, 0x20
                    mov		[esi - 1], al
                    jmp		loc_lodsb
    loc_exit:
            }
    #else*/
    char* ptr = lpDest;
    while (*ptr)
    {
        if (*ptr >= 'a' && *ptr <= 'z')  //*ptr += 'A' - 'a';
            *ptr = toupper(*ptr);
        ptr++;
    }
    // #endif
}
//---------------------------------------------------------------------------
// ����:	StrLower
// ����:	��д��ĸתСд��ĸ
// ����:	lpDest	:	�ַ���
// ����:	void
//---------------------------------------------------------------------------
void g_StrLower(char* lpDest)
{

    char* ptr = lpDest;
    while (*ptr)
    {
        if (*ptr >= 'A' && *ptr <= 'Z')
            //*ptr += 'a' - 'A';
            *ptr = tolower(*ptr);
        ++ptr;
    }
    // #endif
}
//---------------------------------------------------------------------------û�е���
void g_StrRep(char* lpDest, const char* lpSrc, const char* lpRep)
{
    int nSrcLen  = g_StrLen(lpSrc);
    int nDestLen = g_StrLen(lpDest);
    int nMaxLen  = nDestLen - nSrcLen + g_StrLen(lpRep) + 1;
    char* pStart     = NULL;
    int i;
    for (i = 0; i < nDestLen - nSrcLen; i++)
    {
        if (g_StrCmpLen(&lpDest[i], lpSrc, nSrcLen))
            break;
    }
    if (i == nDestLen - nSrcLen)
        return;

    pStart = new char[nMaxLen];

    if (i != 0)
    {
        g_StrCpyLen(pStart, lpDest, i);
        g_StrCat(pStart, lpRep);
        g_StrCat(pStart, &lpDest[i + nSrcLen]);
    }
    else
    {
        g_StrCpy(pStart, lpRep);
        g_StrCat(pStart, &lpDest[nSrcLen]);
    }
    g_StrCpy(lpDest, pStart);
    if (pStart)
    {
        delete[] pStart;
        pStart = NULL;
    }
}

static void t_printchar(char** str, int c)
{
    /*#ifndef WIN32
            extern int putchar(int c);
    #endif*/
    if (str)
    {
        **str = c;
        ++(*str);
    }
    // else (void)putchar(c);
}

#define PAD_RIGHT 1
#define PAD_ZERO  2

static int t_prints(char** out, const char* string, int width, int pad)
{
    int pc = 0, padchar = ' ';

    if (width > 0)
    {
        int len = 0;
        const char* ptr;
        for (ptr = string; *ptr; ++ptr)
            ++len;
        if (len >= width)
            width = 0;
        else
            width -= len;
        if (pad & PAD_ZERO)
            padchar = '0';
    }
    if (!(pad & PAD_RIGHT))
    {
        for (; width > 0; --width)
        {
            t_printchar(out, padchar);
            ++pc;
        }
    }
    for (; *string; ++string)
    {
        t_printchar(out, *string);
        ++pc;
    }
    for (; width > 0; --width)
    {
        t_printchar(out, padchar);
        ++pc;
    }

    return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int t_printi(char** out, int i, int b, int sg, int width, int pad, int letbase)
{
    char print_buf[PRINT_BUF_LEN];
    char* s;
    int t, neg = 0, pc = 0;
    unsigned int u = i;

    if (i == 0)
    {
        print_buf[0] = '0';
        print_buf[1] = '\0';
        return t_prints(out, print_buf, width, pad);
    }

    if (sg && b == 10 && i < 0)
    {
        neg = 1;
        u   = -i;
    }

    s  = print_buf + PRINT_BUF_LEN - 1;
    *s = '\0';

    while (u)
    {
        t = u % b;
        if (t >= 10)
            t += letbase - '0' - 10;
        *--s = t + '0';
        u /= b;
    }

    if (neg)
    {
        if (width && (pad & PAD_ZERO))
        {
            t_printchar(out, '-');
            ++pc;
            --width;
        }
        else
        {
            *--s = '-';
        }
    }

    return pc + t_prints(out, s, width, pad);
}

static int t_print(char** out, const char* format, va_list args)
{
    int width, pad;
    int pc = 0;
    char scr[2];

    for (; *format != 0; ++format)
    {
        if (*format == '%')
        {
            ++format;
            width = pad = 0;
            if (*format == '\0')
                break;
            if (*format == '%')
                goto out_t;
            if (*format == '-')
            {
                ++format;
                pad = PAD_RIGHT;
            }
            while (*format == '0')
            {
                ++format;
                pad |= PAD_ZERO;
            }
            for (; *format >= '0' && *format <= '9'; ++format)
            {
                width *= 10;
                width += *format - '0';
            }
            if (*format == 's')
            {
                char* s = (char*)va_arg(args, int);
                pc += t_prints(out, s ? s : "(null)", width, pad);
                continue;
            }
            if (*format == 'd')
            {
                pc += t_printi(out, va_arg(args, int), 10, 1, width, pad, 'a');
                continue;
            }
            if (*format == 'x')
            {
                pc += t_printi(out, va_arg(args, int), 16, 0, width, pad, 'a');
                continue;
            }
            if (*format == 'X')
            {
                pc += t_printi(out, va_arg(args, int), 16, 0, width, pad, 'A');
                continue;
            }
            if (*format == 'u')
            {
                pc += t_printi(out, va_arg(args, int), 10, 0, width, pad, 'a');
                continue;
            }
            if (*format == 'c')
            {
                /* char are converted to int then pushed on the stack */
                scr[0] = (char)va_arg(args, int);
                scr[1] = '\0';
                pc += t_prints(out, scr, width, pad);
                continue;
            }
        }
        else
        {
        out_t:
            t_printchar(out, *format);
            ++pc;
        }
    }
    if (out)
        **out = '\0';
    va_end(args);
    return pc;
}

int t_printf(const char* format, ...)
{
    va_list args;

    va_start(args, format);
    return t_print(0, format, args);
}

int t_sprintf(char* out, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    return t_print(&out, format, args);
}

int t_snprintf(char* buf, unsigned int count, const char* format, ...)
{
    va_list args;

    (void)count;

    va_start(args, format);
    return t_print(&buf, format, args);
}

/*
��ҫ7(PLK-CL00)���޷��������ģ����ı��ضϣ�����Ӣ�����������������ڲ�ʵ�ֵ���UTF-8
����5360�ﴦ�����������������ڲ�ʹ�õ���ANSIC
�޸ķ�����ȫ���ĳ�std::string�ļӺŲ�����������std::ostringstream��<<�������������ֻ���������
---------------------
*/
static bool _mysprintf(char* szDst, size_t nSize, const char* szFmt, ...)
{
    assert(NULL != szDst && 0 < nSize || NULL != szFmt);
    va_list args;
    va_start(args, szFmt);
    const bool bRet = (0 != vsnprintf(szDst, nSize, szFmt, args));  //  2013-7-26 ����1:19:29
    va_end(args);
    szDst[nSize - 1] = '\0';
    return bRet;
}

// �����ļ���
bool __CreateFolder(std::string nSdcardPath, char* nPath)
{
    if (!nPath || nSdcardPath == "")
        return false;
    /* ��ʽ��·��������Ŀ¼ */
    /*for (int i = 0; i < (int)strlen(nPath); i++)
    {
            if (nPath[i] == '/')
            {
                    nPath[i] = '\\';
            }
    }
    if(nPath[0]=='\\'||nPath[0]=='/')
    {
            memcpy(oPath,nPath+1,strlen(nPath)-1);
    }
    else
    {
            memcpy(oPath, nPath, strlen(nPath));
    }*/

    char tPath[255];
    if (nPath[0] == '/' || nPath[0] == '\\')
    {
        nPath[0] = '/';
        nPath++;
    }

    for (size_t i = 1; i < strlen(nPath); i++)
    {
        if (nPath[i] == '\\')
            nPath[i] = '/';
        if (nPath[i] == '/')
        {
            memcpy(tPath, nPath, i);
            tPath[i] = 0;
            char nTempDir[256];
            ZeroMemory(nTempDir, sizeof(nTempDir));
            sprintf(nTempDir, "%s%s", nSdcardPath.c_str(), tPath);
#ifdef WIN32                   // ����·��
            _mkdir(nTempDir);  // tPath
#else
            // DIR *pDir = NULL;
            // pDir = opendir(nTempDir);
            // if (!pDir)
            mkdir(nTempDir, S_IRWXU | S_IRWXG | S_IRWXO);
            // else
            // closedir(pDir);
#endif
        }
    }

    return true;
}

void g_StrCopy(std::string nSdcardPath, char* pMusicName)
{
    if (!pMusicName || nSdcardPath == "" || !g_pPakList)
        return;
    // ��ʼ��ѹPAK�����ļ�
    char m_szEntireMapFile[256];
    ZeroMemory(m_szEntireMapFile, sizeof(m_szEntireMapFile));
    XPackElemFileRef m_PackRef;
    ZeroMemory(&m_PackRef, sizeof(XPackElemFileRef));
    int nIsHave = 0;
    // messageBox(pMusicName,nSdcardPath.c_str());
    nIsHave = g_pPakList->pGetFilePath(pMusicName, m_PackRef);
    if (nIsHave)
    {  // ��������������
        // messageBox(pMusicName,"�ҵ��ļ�");
        char* sBuf = NULL;
        sBuf       = (char*)malloc(m_PackRef.nSize + 1);
        if (sBuf)
        {
            __CreateFolder(nSdcardPath, pMusicName);
            g_pPakList->ElemFileRead(m_PackRef, sBuf, m_PackRef.nSize);
            // messageBox(m_szEntireMapFile,"��ʼ����");
#ifdef WIN32
            sprintf(m_szEntireMapFile, "%s%s", nSdcardPath.c_str(), pMusicName);
#else
            sprintf(m_szEntireMapFile, "%s%s", nSdcardPath.c_str(), G2U(pMusicName).c_str());
#endif
            FILE* oFp = NULL;
            if ((oFp = fopen(m_szEntireMapFile, "wb+")) == 0)
                return;

            fseek(oFp, 0, 0);
            fwrite(sBuf, m_PackRef.nSize, 1, oFp);
            fclose(oFp);
            oFp = NULL;

            free(sBuf);
            sBuf = NULL;
        }
    }
}

unsigned int _ccHash(const char* key)
{
    unsigned int len    = strlen(key);
    const char* end = key + len;
    unsigned int hash;

    for (hash = 0; key < end; key++)
    {
        hash *= 16777619;
        hash ^= (unsigned int)(unsigned char)toupper(*key);
    }
    return (hash);
}
