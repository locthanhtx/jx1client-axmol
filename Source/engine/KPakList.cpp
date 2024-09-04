//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPakList.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Pack Data List Class
//---------------------------------------------------------------------------
//#include <windows.h>
//#include "KWin32.h"
//#include "KDebug.h"
#include "KStrBase.h"
#include "KFilePath.h"
#include "KIniFile.h"
#include "KPakList.h"
#include "KGbktoUtf8.h"
//#include "crtdbg.h"
//---------------------------------------------------------------------------
KPakList* g_pPakList = NULL;

//---------------------------------------------------------------------------
// ����:	���캯��
//---------------------------------------------------------------------------
KPakList::KPakList()
{
	g_pPakList   = this;
	m_nPakNumber = 0;
}

//---------------------------------------------------------------------------
// ����:	���캯��
//---------------------------------------------------------------------------
KPakList::~KPakList()
{
	Close();
}

//---------------------------------------------------------------------------
// ����:	�ر������ļ�
//---------------------------------------------------------------------------
void KPakList::Close()
{
	for (int32_t i = 0; i < m_nPakNumber; ++i)
	{
		if (m_PakFilePtrList[i])
		{
			delete m_PakFilePtrList[i];
			m_PakFilePtrList[i]=NULL;
		}
	}
	m_nPakNumber = 0;
}

//---------------------------------------------------------------------------
// ����:	�����а���ɨ��ָ�����ļ�
// ����:	uId			�ļ���ID
//			ElemRef		���ڴ�ţ��������ļ���Ϣ
// ����:	�Ƿ�ɹ��ҵ�
//---------------------------------------------------------------------------
bool KPakList::pFindElemFileA(uint32_t uId, XPackElemFileRef& ElemRef)
{
	bool bFounded = false;
	for (int32_t i = 0; i < m_nPakNumber; ++i)
	{
		if (m_PakFilePtrList[i]->XFindElemFile(uId, ElemRef))  //���Ҷ�ȡ�����ļ�
		{
			bFounded = true;
			break;
		}
	}
	return bFounded;
}

//---------------------------------------------------------------------------
// ����:	���ļ���ת��Ϊ���е�id
// ����:	pszFileName	�ļ���
// ����:	�ļ�����Ӧ�İ��е�id
//---------------------------------------------------------------------------
uint32_t KPakList::FileNameToId(const char* pszFileName)
{
	//_ASSERT(pszFileName && pszFileName[0]);
	if  (!pszFileName && !pszFileName[0])
		return 0;

	uint32_t id = 0;
	const char *ptr = pszFileName;
	int32_t index = 0;
	while(*ptr)
	{
		if(*ptr >= 'A' && *ptr <= 'Z')
			id = (id + (++index) * (*ptr + 'a' - 'A')) % 0x8000000b * 0xffffffef;
		else
		    id = (id + (++index) * (*ptr)) % 0x8000000b * 0xffffffef;
		ptr++;
	}
	return (uint32_t)(id ^ 0x12345678);
}

//---------------------------------------------------------------------------
// ����:	�����а���ɨ��ָ���ļ�
// ����:	pszFileName	�ļ���
//			ElemRef	���ڴ�ţ��������ļ���Ϣ
// ����:	�Ƿ�ɹ��ҵ�
//---------------------------------------------------------------------------
bool KPakList::pFindElemFile(char* pszFileName, XPackElemFileRef& ElemRef)
{
	bool bFounded = false;
	if (pszFileName && pszFileName[0])
	{
		char szPackName[256];
		ZeroMemory(szPackName,sizeof(szPackName));
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			szPackName[0] = '\\';
		#else
			szPackName[0] = '\\';
		#endif
		g_GetPackPath(szPackName + 1, (char*)pszFileName);//�ϲ�·�� ȥ��.. ת��Сд
		uint32_t uId = 0;
		uId      = g_FileName2Id(szPackName);
		bFounded = pFindElemFileA(uId, ElemRef);
	}
	return bFounded;
}

//--------------------------------------------------------------------
// ����:	Open package ini file
// ����:	char* filename
// ����:	BOOL
//---------------------------------------------------------------------------
int32_t KPakList::Open(char* pPakListFile,int32_t nKind)
{
	Close();

	KIniFile IniFile;
	#define	SECTION "Package"
	bool bResult = false;
	if (IniFile.Load(pPakListFile)) //���ر��ص� packpage.ini
	{
		//messageBox(pPakListFile,"KPakList::Open::IniFile.Load");

		char szBuffer[32]={0}, szKey[32]={0}, szFile[MAX_PATH]={0};

		if (IniFile.GetString(SECTION, "Path","\\data", szBuffer, sizeof(szBuffer)))
		{
#ifndef WIN32
			strcat(szBuffer,"data");
#endif
			g_GetFullPath(szFile, szBuffer);
			//MessageBox(NULL, szFile,"Information",MB_OK | MB_ICONEXCLAMATION);
			//messageBox(szFile,"IniFile.GetString");

			int32_t nNameStartPos = strlen(szFile);
			if (szFile[nNameStartPos - 1] != '\\' || szFile[nNameStartPos - 1] != '/')
			{
				#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
					szFile[nNameStartPos++] = '\\';
				#else
					szFile[nNameStartPos++] = '/';
				#endif

				szFile[nNameStartPos] = 0;
			}

			for (int32_t i = 0; i < MAX_PAK; ++i)   //PAK��������
			{
				//itoa(i, szKey, 10);
				sprintf(szKey,"%d",i);

				if (!IniFile.GetString(SECTION,szKey,"", szBuffer, sizeof(szBuffer)))
					continue;

				if (szBuffer[0] == 0)
					continue;

				strcpy(szFile + nNameStartPos, szBuffer);
				m_PakFilePtrList[m_nPakNumber] = new XPackFile;  //�����ڴ�

				//printf("--(%s)PAK�������ڴ�:%u b..--\n",szBuffer,sizeof(XPackFile));

				if (m_PakFilePtrList[m_nPakNumber])
				{
					if (m_PakFilePtrList[m_nPakNumber]->Open(szFile, m_nPakNumber)) //����Ƿ��ܴ�PAK��
					{
						m_nPakNumber++;
					}
					else
					{
						delete (m_PakFilePtrList[m_nPakNumber]);
						m_PakFilePtrList[m_nPakNumber]=NULL;
					}
				}
			}
			bResult = true;
		}
	}
	 IniFile.Clear();
	return m_nPakNumber;
}

//��ȡ���ڵ����ļ�
int32_t KPakList::ElemFileRead(XPackElemFileRef& ElemRef,void* pBuffer, unsigned uSize)
{
	if (ElemRef.nPackIndex >= 0 && ElemRef.nPackIndex < m_nPakNumber)
		return m_PakFilePtrList[ElemRef.nPackIndex]->XElemFileRead(ElemRef, pBuffer, uSize);

	//MessageBox(NULL,"�����Ҳ����ļ�A","Information",MB_OK | MB_ICONEXCLAMATION);
	return 0;
}

//��ȡspr�ļ�ͷ��������spr
SPRHEAD* KPakList::GetSprHeader(XPackElemFileRef& ElemRef, SPROFFS*& pOffsetTable)
{
	if (ElemRef.nPackIndex >= 0 && ElemRef.nPackIndex < m_nPakNumber)
		return (m_PakFilePtrList[ElemRef.nPackIndex]->GetSprHeader(ElemRef, pOffsetTable));
	return NULL;
}

//��ȡ��֡ѹ����spr��һ֡������
SPRFRAME* KPakList::GetSprFrame(int32_t nPackIndex, SPRHEAD* pSprHeader, int32_t nFrame,uint32_t &nSingFrameSize)
{
	if (nPackIndex >= 0 && nPackIndex < m_nPakNumber)
		return m_PakFilePtrList[nPackIndex]->GetSprFrame(pSprHeader, nFrame,nSingFrameSize);
	return NULL;
}

bool KPakList::pGetFilePath(char* pszFileName, XPackElemFileRef& ElemRef)
{
	bool bFounded = false;
	if (pszFileName && pszFileName[0])
	{
		char szPackName[256];
		ZeroMemory(szPackName,sizeof(szPackName));
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			szPackName[0] = '\\';
		#else
			szPackName[0] = '\\';
		#endif
		g_GetPackPath(szPackName + 1, (char*)pszFileName);//�ϲ�·�� ȥ��.. ת��Сд
		//char BackStr[256];                       //����ԭ����·��
		//ZeroMemory(BackStr,sizeof(BackStr));
		//sprintf(BackStr,szPackName);
		uint32_t uId = 0;
		uId = g_CheckFileExist(U2G(szPackName).c_str());
		bFounded = pFindElemFileA(uId, ElemRef);
		if (!bFounded)
		{//����ٲ���һ�� ���Ҳ��� �ͷ�������
			uId = g_CheckFileExist(G2U(szPackName).c_str());//FileNameToId(szPackName);
			bFounded = pFindElemFileA(uId, ElemRef);
			if (!bFounded)
			{//��ת�ٲ���һ�� ���Ҳ��� ����û�е���
				uId = g_CheckFileExist(szPackName);//FileNameToId(szPackName);
				bFounded = pFindElemFileA(uId, ElemRef);
			}
		}
		if (!bFounded)
		{
		}
	}
	return bFounded;
}
