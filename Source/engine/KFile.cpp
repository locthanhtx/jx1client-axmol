//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KFile.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Win32 File Operation Class
//---------------------------------------------------------------------------
//#include "KWin32.h"
#include "KFilePath.h"
#include "KFile.h"
//---------------------------------------------------------------------------
// ����:	KFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KFile::KFile()
{
	m_hFile	= NULL;
	m_dwLen	= 0;
	m_dwPos	= 0;
}
//---------------------------------------------------------------------------
// ����:	~KFile
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KFile::~KFile()
{
	Close();
}
//---------------------------------------------------------------------------
// ����:	Open
// ����:	��һ���ļ���׼����ȡ
// ����:	FileName	�ļ���
// ����:	�ɹ�����TRUE��ʧ�ܷ���FALSE��
//---------------------------------------------------------------------------
bool KFile::Open(char * FileName)
{
	char PathName[MAXPATH]={0};

	// close prior file handle
	//if (m_hFile != INVALID_HANDLE_VALUE)
	//	Close();

	if (m_hFile != NULL)
		Close();
	//messageBox(FileName,"KFile::Open a");
	// get full path name
	g_GetFullPath(PathName, FileName);
	//messageBox(PathName,"KFile::Open a");
/*#ifndef WIN32
	char *name_ptr = PathName;
	while(*name_ptr) {
		if(*name_ptr == '\\') *name_ptr = '/';
		name_ptr++;
	}
#endif
	// Open the file for read
	m_hFile = CreateFile(
		PathName,		// pointer to name of the file with path
		GENERIC_READ,	// access (read-write) mode
		FILE_SHARE_READ,// share mode
		NULL,			// pointer to security attributes
		OPEN_EXISTING,	// how to create
		FILE_ATTRIBUTE_NORMAL,// file attributes
		NULL);			// template file*/
/*#ifndef WIN32
        char *ptr = PathName;
        while(*ptr) {
          if(*ptr == '\\') *ptr = '/';
          ptr++;
        }

		//open lower case file for linux
		char lcasePathName[MAXPATH];
		char szRootPath[MAXPATH];
		g_GetRootPath(szRootPath);
		strcpy(lcasePathName, PathName);
		if (memcmp(lcasePathName, szRootPath, strlen(szRootPath)) == 0)
			strlwr(lcasePathName + strlen(szRootPath));
		else
			strlwr(lcasePathName);
		if (NULL == (m_hFile = fopen(lcasePathName, "rb")))
#endif*/
	 char *ptr = PathName;
	        while(*ptr) {
	          if(*ptr == '\\') *ptr = '/';
	          ptr++;
	        }

	m_hFile = fopen(PathName, "rb");
	// check file handle
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return FALSE;

	if (m_hFile == NULL)
	{
		//messageBox(PathName,"PATHE");
		return FALSE;
	}
	//messageBox(PathName,"KFile::Open b");
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Create
// ����:	����һ���ļ���׼��д�롣
// ����:	FileName	�ļ���
// ����:	�ɹ�����TRUE��ʧ�ܷ���FALSE��
//---------------------------------------------------------------------------
bool KFile::Create(char * FileName)
{
	char PathName[MAXPATH]={0};

	// close prior file handle
	//if (m_hFile != INVALID_HANDLE_VALUE)
	//	Close();

	if (m_hFile != NULL) //������ھ͹ر�
		Close();

	// get full path name
	g_GetFullPath(PathName, FileName);

	// change file attribute for write
//	SetFileAttributes(PathName, FILE_ATTRIBUTE_NORMAL);

	// create file for write
/*	m_hFile = CreateFile(
		PathName,		// pointer to name of the file with path
		GENERIC_WRITE,	// access (read-write) mode
		FILE_SHARE_READ,// share mode
		NULL,			// pointer to security attributes
		CREATE_ALWAYS,	// create or over write
		FILE_ATTRIBUTE_NORMAL, // file attributes
		NULL);			// template file
*/
	m_hFile = fopen(PathName,"wb+");
	// check file handle
	if (m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	if (m_hFile == NULL)
		return FALSE;
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	Append
// ����:	��һ���ļ���׼���ں�������ݣ�������ļ������ڣ����������ļ���
// ����:	FileName	�ļ���
// ����:	�ɹ�����TRUE��ʧ�ܷ���FALSE��
//---------------------------------------------------------------------------
bool KFile::Append(char * FileName)
{
	char PathName[MAXPATH]={0};

	// close prior file handle
	//if (m_hFile != INVALID_HANDLE_VALUE)
	//	Close();

	if (m_hFile != NULL)
		Close();

	// get full path name
	g_GetFullPath(PathName, FileName);

	// change file attribute for write
//	SetFileAttributes(PathName, FILE_ATTRIBUTE_NORMAL);

	// create file for write
/*	m_hFile = CreateFile(
		PathName,		// pointer to name of the file with path
		GENERIC_WRITE,	// access (read-write) mode
		FILE_SHARE_READ,// share mode
		NULL,			// pointer to security attributes
		OPEN_ALWAYS,	// Opens the file, if it exists. If the file does not exist, the function creates the file as if dwCreationDisposition were CREATE_NEW
		FILE_ATTRIBUTE_NORMAL, // file attributes
		NULL);			// template file*/
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return FALSE;

	// check file handle
	m_hFile = fopen(PathName, "ab");
	if (m_hFile == NULL)
		return FALSE;

	//fseek(0, FILE_END);
	fseek(m_hFile,0,SEEK_END);
	return TRUE;
}

//---------------------------------------------------------------------------
// ����:	Close
// ����:	�رմ򿪵��ļ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KFile::Close()
{
	//if (m_hFile != INVALID_HANDLE_VALUE)
		//CloseHandle(m_hFile);

	if (m_hFile)
	{
		fclose((FILE*)m_hFile);
	}

	//m_hFile	= (FILE *)INVALID_HANDLE_VALUE;
	m_hFile	= NULL;
	m_dwLen	= 0;
	m_dwPos	= 0;
}
//---------------------------------------------------------------------------
// ����:	Read
// ����:	��ȡ�ļ�����
// ����:	lpBuffer	��ȡ���ݴ�ŵ��ڴ�����
//			dwReadBytes	��ȡ���ݵ��ֽ���
// ����:	�ɹ����ض�ȡ���ֽ�����ʧ�ܷ���0��
//---------------------------------------------------------------------------
uint32_t KFile::Read(void *lpBuffer,uint32_t dwReadBytes)
{
	uint32_t dwBytesRead;

	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return 0;
	if (m_hFile == NULL)
		return 0;

//	ReadFile(m_hFile, lpBuffer, dwReadBytes, &dwBytesRead, NULL);
	dwBytesRead = fread(lpBuffer, 1, dwReadBytes, m_hFile);
	m_dwPos += dwBytesRead;

	return dwBytesRead;

	/* FILE *fp = fopen(fullPath.c_str(), pszMode);
    AX_BREAK_IF(!fp);

    uint32_t size;
    fseek(fp,0,SEEK_END);
    size = ftell(fp);
    fseek(fp,0,SEEK_SET);
    pData = new unsigned char[size];
    size = fread(pData,sizeof(unsigned char), size,fp);
    fclose(fp);

    if (pSize)
    {
        *pSize = size;
    }*/
}
//---------------------------------------------------------------------------
// ����:	write
// ����:	д���ļ�����
// ����:	lpBuffer		д�����ݴ�ŵ��ڴ�����
//			dwWriteBytes	д�����ݵ��ֽ���
// ����:	�ɹ�����д����ֽ�����ʧ�ܷ���0��
//---------------------------------------------------------------------------
uint32_t KFile::Write(void * lpBuffer, uint32_t dwWriteBytes)
{
	uint32_t dwBytesWrite;

	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return 0;

	if (m_hFile == NULL)
		return 0;

	dwBytesWrite = fwrite(lpBuffer, 1, dwWriteBytes, m_hFile);
//	WriteFile(m_hFile, lpBuffer, dwWriteBytes, &dwBytesWrite, NULL);

	m_dwPos += dwBytesWrite;

	return dwBytesWrite;
}
//---------------------------------------------------------------------------
// ����:	Seek
// ����:	�ƶ��ļ�ָ��λ��
// ����:	lDistance		�ƶ�����
//			dwMoveMethod	�ƶ�������FILE_BEGIN��FILE_CURRENT��FILE_END
// ����:	�ɹ�����ָ��λ�ã�ʧ�ܷ���SEEK_ERROR��
//---------------------------------------------------------------------------
uint32_t KFile::Seek(int32_t lDistance, uint32_t dwMoveMethod)
{
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return SEEK_ERROR;

	if (m_hFile == NULL)
		return SEEK_ERROR;

//	m_dwPos = SetFilePointer(m_hFile, lDistance, NULL, dwMoveMethod);
	fseek(m_hFile, lDistance, dwMoveMethod);
	m_dwPos = ftell(m_hFile);
	return m_dwPos;
}
//---------------------------------------------------------------------------
// ����:	Tell
// ����:	ȡ���ļ�ָ��λ��
// ����:	void
// ����:	�ɹ�����ָ��λ�ã�ʧ�ܷ���SEEK_ERROR��
//---------------------------------------------------------------------------
uint32_t KFile::Tell()
{
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return SEEK_ERROR;

	if (m_hFile == NULL)
		return SEEK_ERROR;

	return m_dwPos;
}
//---------------------------------------------------------------------------
// ����:	Size
// ����:	ȡ���ļ�����
// ����:	void
// ����:	�ɹ������ļ����ȣ�ʧ�ܷ���0��
//---------------------------------------------------------------------------
uint32_t KFile::Size()
{
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//	return 0;

	if (m_hFile == NULL)
		return 0;

	if (m_dwLen == 0) {
		DWORD temp = m_dwPos;
		m_dwLen = Seek(0, FILE_END);
		Seek(temp, FILE_BEGIN);
		//m_dwLen = GetFileSize(m_hFile, NULL);
	}

	return m_dwLen;
}
//---------------------------------------------------------------------------
