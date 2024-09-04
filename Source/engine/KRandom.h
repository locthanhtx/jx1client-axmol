//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KRandom.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KRandom_H
#define KRandom_H
#include <cstdint>
//---------------------------------------------------------------------------
//ENGINE_API unsigned __int64 g_Random(UINT nMax);
//ENGINE_API  unsigned __int64 g_GetRandomSeed();
void g_RandomSeed(uint32_t nSeed);
uint32_t g_GetRandomSeed();
uint32_t g_Random(uint32_t nMax);

//---------------------------------------------------------------------------
#endif
