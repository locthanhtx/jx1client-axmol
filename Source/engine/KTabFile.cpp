//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KTabFile.cpp
// Date:	2002.02.20
// Code:	Huyi(Spe)
// Desc:	Tab File Operation Class
//---------------------------------------------------------------------------
#include "KStrBase.h"
#include "KFile.h"
#include "KFilePath.h"
#include "KPakFile.h"
#ifndef _SERVER
#include "KCodec.h"
#endif
#include "KTabFile.h"
#include <string.h>
//---------------------------------------------------------------------------
// ����:	KTabFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KTabFile::KTabFile()
{
    m_Width		= 0;
	m_Height	= 0;
	dwSize      = 0;
	//dwFileIdx   = 0;

	///ZeroMemory(nMemKey,sizeof(nMemKey));
}
//---------------------------------------------------------------------------
// ����:	~KTabFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KTabFile::~KTabFile()
{
	Clear();
}

bool  KTabFile::CreatFile(char * FileName)
{
	if  (!FileName) return false;
	KFile cFile;
	if (cFile.Create(FileName))  //����һ���ļ�
	{
		cFile.Close();
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
// ����:	Load
// ����:	����һ��Tab�ļ�
// ����:	FileName	�ļ���
// ����:	true		�ɹ�
//			false		ʧ��
//---------------------------------------------------------------------------
BOOL KTabFile::Load(char * FileName/*,char * mMemKey*/)
{

	KPakFile	File;
	//uint32_t		dwSize;
	void *		Buffer;

	// check file name
	if (FileName[0] == 0)
		return false;


	if (!File.Open(FileName))
	{
	//	printf("Can't open tab file : %s", FileName);
		return false;
	}

	dwSize = File.Size(); //�ļ��Ĵ�С

	/*char nFileName[256]={0};
	sprintf(nFileName,FileName);
	g_StrLower(nFileName);
	dwFileIdx = g_FileName2Id(nFileName);*/

	if (dwSize<=0)
	{
		File.Close();
		return false;
	}

	//if  (mMemKey)
	//     Buffer = m_Memory.Alloc(dwSize,g_FileName2Id(mMemKey));  //�����ڴ�  �������ڴ��־
	//else
	Buffer = m_Memory.Alloc(dwSize);

	//sprintf(nMemKey,mMemKey);

	File.Read(Buffer, dwSize);

	CreateTabOffset();

	File.Close();
	return true;
}

//---------------------------------------------------------------------------
// ����:	CreateTabOffset
// ����:	������������ָ��ļ���ƫ�Ʊ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KTabFile::CreateTabOffset()
{
	int32_t		nWidth, nHeight, nOffset, nSize;
	unsigned char	*Buffer;
	TABOFFSET *TabBuffer;

	nWidth	= 1;
	nHeight	= 1;
	nOffset = 0;

	Buffer	= (unsigned char *)m_Memory.GetMemPtr();	//��ȡ�ڴ�ָ��
	nSize	= m_Memory.GetMemLen();

	// ����һ�о����ж�����
	while (*Buffer != 0x0d && *Buffer != 0x0a)
	{
		if (*Buffer == 0x09)
		{
			nWidth++;
		}
		Buffer++;
		nOffset++;
	}
	if (*Buffer == 0x0d && *(Buffer + 1) == 0x0a)
	{
		Buffer += 2;	// 0x0a����
		nOffset += 2;	// 0x0a����
	}
	else
	{
		Buffer += 1;	// 0x0a����
		nOffset += 1;	// 0x0a����
	}
	while(nOffset < nSize)
	{
		while (*Buffer != 0x0d && *Buffer != 0x0a)
		{
			Buffer++;
			nOffset++;
			if (nOffset >= nSize)
				break;
		}
		nHeight++;
		if (*Buffer == 0x0d && *(Buffer + 1) == 0x0a)
		{
			Buffer += 2;	// 0x0a����
			nOffset += 2;	// 0x0a����
		}
		else
		{
			Buffer += 1;	// 0x0a����
			nOffset += 1;	// 0x0a����
		}
	}
	m_Width		= nWidth;
	m_Height	= nHeight;

	TabBuffer = (TABOFFSET *)m_OffsetTable.Alloc(m_Width * m_Height * sizeof (TABOFFSET));
	Buffer = (unsigned char *)m_Memory.GetMemPtr();

	nOffset = 0;
	int32_t nLength;
	for (int32_t i = 0; i < nHeight; ++i)
	{
		for (int32_t j = 0; j < nWidth; ++j)
		{
			TabBuffer->dwOffset = nOffset;
			nLength = 0;
			while(*Buffer != 0x09 && *Buffer != 0x0d && *Buffer != 0x0a && nOffset < nSize)
			{
				Buffer++;
				nOffset++;
				nLength++;
			}
			Buffer++;	// 0x09��0x0d��0x0a(linux)����
			nOffset++;
			TabBuffer->dwLength = nLength;
			TabBuffer++;
			if (*(Buffer - 1) == 0x0a || *(Buffer - 1) == 0x0d)	//	�����Ѿ������ˣ���Ȼ����û��nWidth //for linux modified [wxb 2003-7-29]
			{
				for (int32_t k = j+1; k < nWidth; ++k)
				{
					TabBuffer->dwOffset = nOffset;
					TabBuffer->dwLength = 0;
					TabBuffer++;
				}
				break;
			}
		}

		//modified for linux [wxb 2003-7-29]
		if (*(Buffer - 1) == 0x0d && *Buffer == 0x0a)
		{
			Buffer++;				// 0x0a����
			nOffset++;				// 0x0a����
		}
	}
}

//---------------------------------------------------------------------------
// ����:	Str2Column
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	szColumn
// ����:	�ڼ���
//---------------------------------------------------------------------------
int32_t KTabFile::Str2Col(char * szColumn)
{
	int32_t	nStrLen = g_StrLen(szColumn);
	char	szTemp[4];

	g_StrCpy(szTemp, szColumn);
	g_StrUpper(szTemp);

	//toupper(szTemp);
    // _strupr(szTemp);
	if (nStrLen == 1)
	{
		return (szTemp[0] - 'A');
	}
	return ((szTemp[0] - 'A' + 1) * 26 + szTemp[1] - 'A') + 1;
}

//---------------------------------------------------------------------------
// ����:	GetString
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			nColomn			��
//			lpDefault		ȱʡֵ
//			lpRString		����ֵ
//			dwSize			�����ַ�������󳤶�
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetString(int32_t nRow, char * szColumn, char * lpDefault, char * lpRString, uint32_t dwSize, BOOL bColumnLab)
{
	int32_t nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, lpRString, dwSize))
		return true;
	g_StrCpyLen(lpRString, lpDefault, dwSize); //Ĭ��ֵ
	    return false;
}
//---------------------------------------------------------------------------
// ����:	GetString
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			nColomn			��
//			lpDefault		ȱʡֵ
//			lpRString		����ֵ
//			dwSize			�����ַ�������󳤶�
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetString(char * szRow, int32_t szColumn, char * lpDefault, char * lpRString, uint32_t dwSize, BOOL bColumnLab)
{
	int32_t nRow, nColumn;

	   nRow = FindRow(szRow);

	if (bColumnLab)
		nColumn = szColumn;
	else
		nColumn = szColumn;

	if (GetValue(nRow - 1, nColumn - 1, lpRString, dwSize))
		return true;
	g_StrCpyLen(lpRString, lpDefault, dwSize);
	return false;
}
//---------------------------------------------------------------------------
// ����:	GetString
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	szRow			��	���ؼ��֣�
//			szColomn		��	���ؼ��֣�
//			lpDefault		ȱʡֵ
//			lpRString		����ֵ
//			dwSize			�����ַ�������󳤶�
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetString(char * szRow, char * szColumn, char * lpDefault, char * lpRString, uint32_t dwSize)
{
	int32_t nRow, nColumn;

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, lpRString, dwSize))
		return true;
	g_StrCpyLen(lpRString, lpDefault, dwSize);
	return false;
}
//---------------------------------------------------------------------------
// ����:	GetString
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��		��1��ʼ
//			nColomn			��		��1��ʼ
//			lpDefault		ȱʡֵ
//			lpRString		����ֵ
//			dwSize			�����ַ�������󳤶�
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetString(int32_t nRow, int32_t nColumn, char * lpDefault, char * lpRString, uint32_t dwSize)
{
	if (GetValue(nRow - 1, nColumn - 1,  lpRString, dwSize))
		return true;
	g_StrCpyLen(lpRString, lpDefault, dwSize);
	return false;
}
//---------------------------------------------------------------------------
// ����:	GetInteger
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetInteger(int32_t nRow, char * szColumn, int32_t nDefault, int32_t *pnValue, BOOL bColumnLab)
{
	char	Buffer[64];
	int32_t		nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return true;
	}
	else
	{
		*pnValue = nDefault;
		return false;
	}
}

//---------------------------------------------------------------------------
// ����:	GetInteger6  6�ֶ���ȡint�ַ�
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetInteger6(int32_t nRow, char * szColumna,char * szColumnb,char * szColumnc,char * szColumnd,char * szColumne,char * szColumnf,  int32_t nDefault, int32_t *pnValue, BOOL bColumnLab)
{
	char	Buffer[64];
	int32_t		nColumna,nColumnb,nColumnc,nColumnd,nColumne,nColumnf;
	if (bColumnLab)
	{
		nColumna = FindColumn(szColumna);
		nColumnb = FindColumn(szColumnb);
		nColumnc = FindColumn(szColumnc);
		nColumnd = FindColumn(szColumnd);
		nColumne = FindColumn(szColumne);
		nColumnf = FindColumn(szColumnf);
	}
	else
	{
		nColumna = Str2Col(szColumna);
		nColumnb = Str2Col(szColumnb);
		nColumnc = Str2Col(szColumnc);
		nColumnd = Str2Col(szColumnd);
		nColumne = Str2Col(szColumne);
		nColumnf = Str2Col(szColumnf);
	}

	if (GetValue(nRow - 1, nColumna - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		//return true;
	}
	else
	{
		*pnValue = nDefault;
	}
     pnValue++;
    if(GetValue(nRow - 1, nColumnb - 1, Buffer, sizeof(Buffer)))
	{

		*pnValue = atoi(Buffer);
		//return true;
	}
	else
	{
		*pnValue = nDefault;
	}
     pnValue++;
    if(GetValue(nRow - 1, nColumnc - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		//return true;
	}
	else
	{
		*pnValue = nDefault;
	}
     pnValue++;
	if(GetValue(nRow - 1, nColumnd - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		//return true;
	}
	else
	{
		*pnValue = nDefault;
	}
     pnValue++;
	if(GetValue(nRow - 1, nColumne - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		//return true;
	}
	else
	{
		*pnValue = nDefault;
	}
     pnValue++;
	if(GetValue(nRow - 1, nColumnf - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		//return true;
	}
	else
	{
		*pnValue = nDefault;
	}

	if (pnValue)
	{
      return true;
	}

	 return false;
}
//---------------------------------------------------------------------------
// ����:	GetInteger3  3�ֶ���ȡint�ַ�
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetInteger3(int32_t nRow, char * szColumna,char * szColumnb,char * szColumnc,int32_t nDefault, int32_t *pnValue, BOOL bColumnLab)
{
	char	Buffer[64];
	int32_t		nColumna,nColumnb,nColumnc;
	if (bColumnLab)
	{
		nColumna = FindColumn(szColumna);
		nColumnb = FindColumn(szColumnb);
		nColumnc = FindColumn(szColumnc);
	}
	else
	{
		nColumna = Str2Col(szColumna);
		nColumnb = Str2Col(szColumnb);
		nColumnc = Str2Col(szColumnc);
	}

	if (GetValue(nRow - 1, nColumna - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		//return true;
	}
	else
	{
		*pnValue = nDefault;
	}
	pnValue++;
    if(GetValue(nRow - 1, nColumnb - 1, Buffer, sizeof(Buffer)))
	{

		*pnValue = atoi(Buffer);
		//return true;
	}
	else
	{
		*pnValue = nDefault;
	}
	pnValue++;
    if(GetValue(nRow - 1, nColumnc - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		//return true;
	}
	else
	{
		*pnValue = nDefault;
	}


	if (pnValue)
	{
	   return true;
	}
	   return false;
}




//ͬ���ֶηָ���� |
//void KTabFile::GetInt2(LPCSTR lpSection, LPCSTR lpKeyName, int32_t* pRect)

void KTabFile::GetInt2(int32_t nRow, int32_t szColumn, int32_t *pRect)
{
	char  Buffer[256];

	if (GetValue(nRow - 1, szColumn - 1, Buffer, sizeof(Buffer)))
	{
		sscanf(Buffer, "%d|%d", &pRect[0], &pRect[1]);
	}
}

void KTabFile::GetInt15(int32_t nRow, int32_t szColumn, int32_t *pRect)
{
	char  Buffer[256];

	if (GetValue(nRow - 1, szColumn - 1, Buffer, sizeof(Buffer)))
	{
		sscanf(Buffer, "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d", &pRect[0],&pRect[1],&pRect[2],&pRect[3],&pRect[4],&pRect[5],&pRect[6],&pRect[7],&pRect[8],&pRect[9],&pRect[10],&pRect[11],&pRect[12],&pRect[13],&pRect[14]);
	}
}

void KTabFile::GetInt15s(int32_t nRow, char * szColumn, int32_t *pRect)
{
	char  Buffer[256];

	int32_t	nColumn = FindColumn(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		sscanf(Buffer, "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d", &pRect[0],&pRect[1],&pRect[2],&pRect[3],&pRect[4],&pRect[5],&pRect[6],&pRect[7],&pRect[8],&pRect[9],&pRect[10],&pRect[11],&pRect[12],&pRect[13],&pRect[14]);
	}
}

void KTabFile::GetInt3(int32_t nRow, int32_t szColumn, int32_t *pRect)
{
	char  Buffer[256];

	if (GetValue(nRow - 1, szColumn - 1, Buffer, sizeof(Buffer)))
	{
		sscanf(Buffer, "%d|%d|%d", &pRect[0], &pRect[1], &pRect[2]);
	}
}
//------�ַ�����-----
void KTabFile::GetInt2s(int32_t nRow, char * szColumn, int32_t *pRect)
{
	char  Buffer[256];

	int32_t	nColumn = FindColumn(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		sscanf(Buffer, "%d|%d", &pRect[0], &pRect[1]);
	}
}

void KTabFile::GetInt3s(int32_t nRow, char * szColumn, int32_t *pRect)
{
	char  Buffer[256];
	int32_t	nColumn = FindColumn(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		sscanf(Buffer, "%d|%d|%d", &pRect[0], &pRect[1], &pRect[2]);
	}
}
//---------------------------------------------------------------------------
// ����:	GetInteger
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	szRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetInteger(char * szRow, char * szColumn, int32_t nDefault, int32_t *pnValue)
{
	int32_t		nRow, nColumn;
	char	Buffer[64];

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return true;
	}
	else
	{
		*pnValue = nDefault;
		return false;
	}
}
//---------------------------------------------------------------------------
// ����:	GetInteger
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��		��1��ʼ
//			nColomn			��		��1��ʼ
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetInteger(int32_t nRow, int32_t nColumn, int32_t nDefault, int32_t *pnValue)
{
	char	Buffer[64]={0};

	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return true;
	}
	else
	{
		*pnValue = nDefault;
		return true;
	}
}

BOOL KTabFile::GetIntegerDW(int32_t nRow, char * szColumn, int32_t nDefault,uint32_t *pnValue, BOOL bColumnLab)
{
	char	Buffer[64];
	int32_t		nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return true;
	}
	else
	{
		*pnValue = nDefault;
		return false;
	}
}

BOOL KTabFile::GetDword(int32_t nRow, char * szColumn, uint32_t fDefault, uint32_t *pfValue, BOOL bColumnLab)
{
	char	Buffer[64];
	int32_t		nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (uint32_t)atof(Buffer);
		return true;
	}
	else
	{
		*pfValue = fDefault;
		return false;
	}
}


//---------------------------------------------------------------------------
// ����:	GetDouble
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetDouble(int32_t nRow, char * szColumn, double fDefault, double *pfValue, BOOL bColumnLab)
{
	char	Buffer[64];
	int32_t		nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (double)atof(Buffer);
		return true;
	}
	else
	{
		*pfValue = fDefault;
		return false;
	}
}

//---------------------------------------------------------------------------
// ����:	GetFloat
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetFloat(int32_t nRow, char * szColumn, float fDefault, float *pfValue, BOOL bColumnLab)
{
	char	Buffer[64];
	int32_t		nColumn;
	if (bColumnLab)
		nColumn = FindColumn(szColumn);
	else
		nColumn = Str2Col(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (float)atof(Buffer);
		return true;
	}
	else
	{
		*pfValue = fDefault;
		return false;
	}
}
//---------------------------------------------------------------------------
// ����:	GetFloat
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	szRow			��
//			szColomn		��
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetFloat(char * szRow, char * szColumn, float fDefault, float *pfValue)
{
	int32_t		nRow, nColumn;
	char	Buffer[64];

	nRow = FindRow(szRow);
	nColumn = FindColumn(szColumn);
	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (float)atof(Buffer);
		return true;
	}
	else
	{
		*pfValue = fDefault;
		return false;
	}
}
//---------------------------------------------------------------------------
// ����:	GetFloat
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��		��1��ʼ
//			nColomn			��		��1��ʼ
//			nDefault		ȱʡֵ
//			pnValue			����ֵ
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetFloat(int32_t nRow, int32_t nColumn, float fDefault, float *pfValue)
{
	char	Buffer[64];

	if (GetValue(nRow - 1, nColumn - 1, Buffer, sizeof(Buffer)))
	{
		*pfValue = (float)atof(Buffer);
		return true;
	}
	else
	{
		*pfValue = fDefault;
		return false;
	}
}
//---------------------------------------------------------------------------
// ����:	GetValue
// ����:	ȡ��ĳ��ĳ���ַ�����ֵ
// ����:	nRow			��
//			nColomn			��
//			lpDefault		ȱʡֵ
//			lpRString		����ֵ
//			dwSize			�����ַ�������󳤶�
// ����:	�Ƿ�ɹ�
//---------------------------------------------------------------------------
BOOL KTabFile::GetValue(int32_t nRow, int32_t nColumn, char * lpRString, uint32_t dwSize)
{
	if (nRow >= m_Height || nColumn >= m_Width || nRow < 0 || nColumn < 0)
		return false;

	TABOFFSET	*TempOffset;
	char *		Buffer;

	Buffer = (char *)m_Memory.GetMemPtr();
	TempOffset = (TABOFFSET *)m_OffsetTable.GetMemPtr();
	TempOffset += nRow * m_Width + nColumn;

	//ZeroMemory(lpRString, dwSize);
	memset(lpRString, 0, dwSize);
	Buffer += TempOffset->dwOffset;
	if (TempOffset->dwLength == 0)
	{
		TempOffset=NULL;
		return false;
	}
	if (dwSize > TempOffset->dwLength)
	{
		memcpy(lpRString, Buffer, TempOffset->dwLength);
		lpRString[TempOffset->dwLength] ='\0';
	}
	else
	{
		memcpy(lpRString, Buffer, dwSize);
		lpRString[dwSize] = '\0';
	}
    TempOffset=NULL;
	return true;
}
//---------------------------------------------------------------------------
// ����:	Clear
// ����:	���TAB�ļ�������
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KTabFile::Clear()
{
/*	if (m_Memory)
	{
		g_MemFree(m_Memory);
		m_Memory = NULL;
	}
	*/
	//ZeroMemory(nMemKey,sizeof(nMemKey));
	//dwFileIdx = 0;
	m_OffsetTable.Free();
	m_Memory.Free();
}
//---------------------------------------------------------------------------
// ����:	FindRow
// ����:	�����йؼ���
// ����:	szRow���йؼ��֣�
// ����:	int32_t
//---------------------------------------------------------------------------
int32_t KTabFile::FindRow(char * szRow)
{
	char	szTemp[128];
	for (int32_t i = 0; i < m_Height; ++i)	// ��1��ʼ��������һ�е��ֶ���
	{
		GetValue(i, 0, szTemp, g_StrLen(szRow));
        std::string str(szTemp);
        std::string str1(U2G(szTemp));
        std::string str2(G2U(szTemp));
		if (g_StrCmp(szTemp, szRow))
			return i + 1; //�Ķ��˴�Ϊ��һ by Romandou,��������1Ϊ���ı��
	}
	return -1;
}
//---------------------------------------------------------------------------
// ����:	FindColumn
// ����:	�����йؼ���
// ����:	szColumn���йؼ��֣�
// ����:	int32_t
//---------------------------------------------------------------------------
int32_t KTabFile::FindColumn(char * szColumn)
{
	char	szTemp[128];
	for (int32_t i = 0; i < m_Width; ++i)	// ��1��ʼ��������һ�е��ֶ���
	{
		GetValue(0, i, szTemp, g_StrLen(szColumn));
		if (g_StrCmp(szTemp, szColumn))
			return i + 1;//�Ķ��˴�Ϊ��һ by Romandou,��������1Ϊ���ı��
	}
	return -1;
}

//---------------------------------------------------------------------------
// ����:	Col2Str
// ����:	������ת���ַ���
// ����:	szColumn
// ����:	�ڼ���
//---------------------------------------------------------------------------
void KTabFile::Col2Str(int32_t nCol, char * szColumn)
{

	if (nCol < 26)
	{
		szColumn[0] = 'A' + nCol;
		szColumn[1]	= 0;
	}
	else
	{
		szColumn[0] = 'A' + (nCol / 26 - 1);
		szColumn[1] = 'A' + nCol % 26;
		szColumn[2] = 0;
	}
}
