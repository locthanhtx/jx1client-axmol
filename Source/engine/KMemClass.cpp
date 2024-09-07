//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMemClass.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Memory Opration Class Using Heap Memory Functions
//---------------------------------------------------------------------------
//#include "KWin32.h"
//#include "KDebug.h"
#include "KMemBase.h"
#include "KMemClass.h"
//---------------------------------------------------------------------------
// ����:	KMemClass
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KMemClass::KMemClass()
{
	m_lpMemPtr = NULL;
	m_lpMemLen = 0;
}
//---------------------------------------------------------------------------
// ����:	~KMemClass
// ����:	���캯��
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
KMemClass::~KMemClass()
{
	Free();
}
//---------------------------------------------------------------------------
// ����:	Alloc
// ����:	�����ڴ�
// ����:	dwSize		�ڴ���С
// ����:	PVOID		�ڴ��ָ��
//---------------------------------------------------------------------------
PVOID KMemClass::Alloc(unsigned long dwSize)
{
	// �Ѿ�����ľͲ�Ҫ�ٷ�����
	if (m_lpMemLen == dwSize)
		return m_lpMemPtr;

	// �ͷ��Ѿ�������ڴ�
	if (m_lpMemPtr)
		Free();

	// �����ڴ�
	m_lpMemPtr = g_MemAlloc(dwSize);

	if (m_lpMemPtr != NULL)
		m_lpMemLen = dwSize;
	return m_lpMemPtr;

}
//---------------------------------------------------------------------------
// ����:	Free
// ����:	�ͷ��ڴ�
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMemClass::Free()
{
	if (m_lpMemPtr)
		g_MemFree(m_lpMemPtr);
	m_lpMemPtr = NULL;
	m_lpMemLen = 0;
}
//---------------------------------------------------------------------------
// ����:	Zero
// ����:	�ڴ�����
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KMemClass::Zero()
{
	if (m_lpMemPtr)
		g_MemZero(m_lpMemPtr, m_lpMemLen);
}
//---------------------------------------------------------------------------
// ����:	Fill
// ����:	�ڴ����
// ����:	byFill	���������ֽ�
// ����:	void
//---------------------------------------------------------------------------
void KMemClass::Fill(BYTE byFill)
{
	if (m_lpMemPtr)
		g_MemFill(m_lpMemPtr, m_lpMemLen, byFill);
}
//---------------------------------------------------------------------------
// ����:	Fill
// ����:	�ڴ����
// ����:	byFill	���������ֽ�
// ����:	void
//---------------------------------------------------------------------------
void KMemClass::Fill(unsigned short wFill)
{
	if (m_lpMemPtr)
		g_MemFill(m_lpMemPtr, m_lpMemLen, wFill);
}
//---------------------------------------------------------------------------
// ����:	Fill
// ����:	�ڴ����
// ����:	byFill	���������ֽ�
// ����:	void
//---------------------------------------------------------------------------
void KMemClass::Fill(unsigned long dwFill)
{
	if (m_lpMemPtr)
		g_MemFill(m_lpMemPtr, m_lpMemLen, dwFill);
}
//---------------------------------------------------------------------------
