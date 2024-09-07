//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KMemClass.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KMemClass_H
#define KMemClass_H
#include "cocos2d.h"

USING_NS_AX;
#include "KbugInfo.h"
//---------------------------------------------------------------------------
class KMemClass
{
private:
	PVOID		m_lpMemPtr;
	unsigned long		m_lpMemLen;
public:
	KMemClass();
	~KMemClass();
	PVOID		Alloc(unsigned long dwSize);
	void		Free();
	void		Zero();
	void		Fill(BYTE byFill);
	void		Fill(unsigned short wFill);
	void		Fill(unsigned long dwFill);
	PVOID		GetMemPtr() { return m_lpMemPtr; };
	unsigned long		GetMemLen() { return m_lpMemLen; };
};
//---------------------------------------------------------------------------
#endif
