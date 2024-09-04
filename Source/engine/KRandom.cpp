//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KRandom.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Random Number Generate Functions
//---------------------------------------------------------------------------
#include "KRandom.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
//---------------------------------------------------------------------------
#define IM 139968
#define IA 3877
#define IC 29573
//---------------------------------------------------------------------------
static uint32_t nRandomSeed = 42;  //long long		UINT
//---------------------------------------------------------------------------
// ����:	RandomnSeed
// ����:	�������������
// ����:	nRandomSeed	:	���������
// ����:	void
//---------------------------------------------------------------------------
void g_RandomSeed(uint32_t nSeed)
{
	nRandomSeed = nSeed;

	//if (nRandomSeed>500000)
	//	nRandomSeed=42; 
}

//---------------------------------------------------------------------------
// ����:	Random
// ����:	����һ��С��nMax���������
// ����:	nMax	:	���ֵ
// ����:	һ��С��nMax�������
//---------------------------------------------------------------------------
uint32_t g_Random(uint32_t nMax)
{
	if (nMax)
	{
		nRandomSeed = nRandomSeed * IA + IC;	  //19W

		if ((nRandomSeed%nMax)<0)	//�����
		{
			/* ׼�����������,�Ե�ǰʱ��Ϊ���� */
			srand((unsigned)time(NULL));
			nRandomSeed= rand()%nMax;
			return nRandomSeed%nMax;
		}

		return nRandomSeed%nMax; //ȡ����				  
	}
	else
	{
		return 0;
	}
}
//---------------------------------------------------------------------------
// ����:	GetRandomSeed
// ����:	ȡ�õ�ʱ��α�������
// ����:	���ص�ǰ��α�������
//---------------------------------------------------------------------------
uint32_t g_GetRandomSeed()
{
	if  (nRandomSeed<0)
	{
		srand((unsigned)time(NULL));
		nRandomSeed= rand()%100+42;
		return nRandomSeed;
	}

	return nRandomSeed;
}

/*//---------------------------------------------------------------------------
// ����:	Random
// ����:	����һ��С��nMax���������
// ����:	nMax	:	���ֵ
// ����:	һ��С��nMax�������
//---------------------------------------------------------------------------
unsigned __int64 g_Random(UINT nMax)
{
	if (nMax)
	{
		//if (nRandomSeed>500000)	 //���������ж� 20��
		//    nRandomSeed=42;
		nRandomSeed = nRandomSeed * IA + IC;	  //19W
		return nRandomSeed%nMax; //ȡ����				  
	}
	else
	{
		return 0;
	}
}
*/