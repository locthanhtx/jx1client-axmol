#include "KMemClass1.h"
PVOID KMemClass1::Alloc(unsigned long dwSize)
{
	this->m_lpMemPtr = new BYTE[dwSize];
	this->m_lpMemLen = dwSize;
	return m_lpMemPtr;
}

void KMemClass1::Free()
{
	delete []this->m_lpMemPtr;
	this->m_lpMemPtr=NULL;
}
