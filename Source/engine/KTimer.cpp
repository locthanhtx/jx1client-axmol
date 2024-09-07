//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KTimer.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Timer Class
//---------------------------------------------------------------------------

//#include <Winbase.h>
//#include "KMemBase.h"
#include "KTimer.h"
#include <time.h>
// flying add this macro.
// must pay more attention in the platform porting job.
#ifdef __linux
#include <sys/time.h>
#endif


//#include <time.h>
// ��1601��1��1��0:0:0:000��1970��1��1��0:0:0:000��ʱ��(��λ100ns)
#define EPOCHFILETIME (116444736000000000UL)

KTimer::KTimer()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//m_nFrequency.QuadPart = 200 * 1024 * 1024;   //ʱ�ӵ�Ƶ��
	//m_nTimeStart.QuadPart = 0;
	//m_nTimeStop.QuadPart  = 0;
	m_nFPS = 0;
	//SetThreadAffinityMask(GetCurrentThread(),0x00000001);
	//QueryPerformanceFrequency(&m_nFrequency);    //���ڶ�˳��ִ���  ����Ӳ��֧�ֵĸ߾��ȼ�������Ƶ�ʡ���ȡ cpuƵ��
#else
    //m_nFrequency = CLOCKS_PER_SEC; //����ԭ��û�е�
#endif
}
//---------------------------------------------------------------------------
// ����:	Start
// ����:	��ʼ��ʱ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------
void KTimer::Start()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//	 CCTimer::
    //FILETIME ft;
    //LARGE_INTEGER li;
    //int64_t tt = 0;
    /*::GetSystemTimeAsFileTime(&ft);
    m_nTimeStart.LowPart  = ft.dwLowDateTime;
    m_nTimeStart.HighPart = ft.dwHighDateTime;
    // ��1970��1��1��0:0:0:000�����ڵ�΢����(UTCʱ��)
    m_nTimeStart.QuadPart = (m_nTimeStart.QuadPart - EPOCHFILETIME)/10UL;
    //return tt;
	//DWORD_PTR oldmask =::SetThreadAffinityMask(::GetCurrentThread(),0);
	//QueryPerformanceCounter(&m_nTimeStart);
	//::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
    //time_t rawtime;      //����һ��int �ʹ������
	//struct tm * timeinfo;
	//m_nTimeStart.QuadPart = time(NULL);     //�������ڵ�ʱ�䣨�룩
    //m_nTimeStart.QuadPart = m_nTimeStart.sQuadPart*1000; */
	//timeval *m_pStartUpdate = NULL;
	gettimeofday(&m_nTimeStart,NULL);
   // if (m_pStartUpdate)
	 //   m_nTimeStart.QuadPart = m_pStartUpdate->tv_usec/1000;
	//m_nTimeStart.QuadPart=//GetTickCount();// ��ȷ�Ǻ���

#else
    gettimeofday(&m_nTimeStart, NULL); //ԭ��
//	QueryPerformanceCounter(&m_nTimeStart);
#endif
}
//---------------------------------------------------------------------------
// ����:	Stop
// ����:	ֹͣ��ʱ
// ����:	void
// ����:	void
//---------------------------------------------------------------------------

void KTimer::Stop()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    //FILETIME ft;
    //LARGE_INTEGER li;
    //int64_t tt = 0;
    /*::GetSystemTimeAsFileTime(&ft);
    m_nTimeStop.LowPart  = ft.dwLowDateTime;
    m_nTimeStop.HighPart = ft.dwHighDateTime;
    // ��1970��1��1��0:0:0:000�����ڵ�΢����(UTCʱ��)
    m_nTimeStop.QuadPart = (m_nTimeStop.QuadPart - EPOCHFILETIME)/10UL;	//����
	//DWORD_PTR oldmask =::SetThreadAffinityMask(::GetCurrentThread(),0);
	//QueryPerformanceCounter(&m_nTimeStop);
	//::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
    //m_nTimeStop.QuadPart = time(NULL);
	*/

    //m_nTimeStop.QuadPart=GetTickCount();// ��ȷ�Ǻ���
	//timeval *m_pStopUpdate = NULL;
	gettimeofday(&m_nTimeStop,NULL);
	//if (m_pStopUpdate)
	  // m_nTimeStop.QuadPart = m_pStopUpdate->tv_usec/1000;

#else
	gettimeofday(&m_nTimeStop, NULL); //ԭ��
//	QueryPerformanceCounter(&m_nTimeStop);
#endif
}
//---------------------------------------------------------------------------
// ����:	GetElapse
// ����:	����ӿ�ʼ��ʱ�������Ѿ�����ʱ��
// ����:	void
// ����:	unsigned long in ms		 GetTickCount() ��ȷ�Ǻ���
//---------------------------------------------------------------------------
long long KTimer::GetElapse()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//LARGE_INTEGERS nTime;
	/*
    FILETIME ft;
    //int64_t tt = 0;
    ::GetSystemTimeAsFileTime(&ft);	   //��ȡ���ڵ�ʱ��
    nTime.LowPart  = ft.dwLowDateTime;
    nTime.HighPart = ft.dwHighDateTime;
    // ��1970��1��1��0:0:0:000�����ڵ�΢����(UTCʱ��)
    nTime.QuadPart = (nTime.QuadPart - EPOCHFILETIME)/10UL;	//����
   	*/
	timeval  m_pEndtime;
	gettimeofday(&m_pEndtime,NULL);
	//if (m_pStopUpdate)
		//m_nTimeStop.QuadPart = m_pStopUpdate->tv_usec/1000;
	return (m_pEndtime.tv_sec - m_nTimeStart.tv_sec) * 1000 + m_pEndtime.tv_usec/1000;  //����
	    //nTime.QuadPart= m_pEndtime->tv_usec/1000;//GetTickCount();// ��ȷ�Ǻ���
	//return (unsigned long)((nTime.QuadPart - m_nTimeStart.QuadPart)/1000UL);  //����
	//return  (nTime.QuadPart - m_nTimeStart.QuadPart);
	//LARGE_INTEGER nTime;
	//DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	//QueryPerformanceCounter(&nTime);
	//::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
	//return (unsigned long)((nTime.QuadPart - m_nTimeStart.QuadPart) * 1000 / m_nFrequency.QuadPart);

	/* nTime.QuadPart = time(NULL);
    unsigned long nValTime;
    nValTime =(unsigned long)((nTime.QuadPart - m_nTimeStart.QuadPart)*1000);
	return nValTime;*/  //������
#else
	timeval tv;
	gettimeofday(&tv, NULL); //ԭ��
    return (tv.tv_sec - m_nTimeStart.tv_sec) * 1000 + tv.tv_usec / 1000;  //����
#endif
}
//---------------------------------------------------------------------------
// ����:	GetElapseFrequency
// ����:	����ӿ�ʼ��ʱ�������Ѿ�����ʱ��
// ����:	void
// ����:	unsigned long in frequency
//---------------------------------------------------------------------------
long long KTimer::GetElapseFrequency()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	LARGE_INTEGERS nTime;
   /* FILETIME ft;
    LARGE_INTEGER nTime;
    //int64_t tt = 0;
    ::GetSystemTimeAsFileTime(&ft);
    nTime.LowPart  = ft.dwLowDateTime;
    nTime.HighPart = ft.dwHighDateTime;
	nTime.QuadPart = (nTime.QuadPart - EPOCHFILETIME)/10UL;
	//DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	//QueryPerformanceCounter(&nTime);
    //::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
	//nTime.QuadPart = time(NULL);
	return (unsigned long)((nTime.QuadPart - m_nTimeStart.QuadPart)/1000UL); */
	//nTime.QuadPart=GetTickCount();// ��ȷ�Ǻ���
	return 0;//(nTime.QuadPart - m_nTimeStart.QuadPart);

#endif
	return 0;
}
//---------------------------------------------------------------------------
// ����:	GetInterval
// ����:	ȡ�ôӿ�ʼ��ֹ֮ͣ���ʱ�������Ժ���Ϊ��λ
// ����:	void
// ����:	����ֵ
//---------------------------------------------------------------------------
long long KTimer::GetInterval()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//return (unsigned long)((m_nTimeStop.QuadPart - m_nTimeStart.QuadPart) * 1000 / m_nFrequency.QuadPart); //����Ϊ��λ
    return 0;//(m_nTimeStop.QuadPart - m_nTimeStart.QuadPart);
#endif
  // return (unsigned long)((m_nTimeStop.QuadPart - m_nTimeStart.QuadPart) * 1000 / m_nFrequency.QuadPart);
	return 0; //ԭ��
}
//---------------------------------------------------------------------------
// ����:	Passed
// ����:	���Ƿ����nTime����
// ����:	nTime	����
// ����:	TRUE	�Ѿ�����
//			FALSE	��û�й�
//---------------------------------------------------------------------------
bool KTimer::Passed(unsigned int nTime)
{

	if (GetElapse() >= (unsigned int)nTime)
	{
		Start();    //�����趨��ʼʱ��
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
// ����:	GetFPS
// ����:	ȡ��֡����
// ����:	nFPS	֡����
// ����:	TRUE	�ɹ�
//			FALSE	ʧ��
//---------------------------------------------------------------------------
bool KTimer::GetFPS(int *nFPS)
{
	if (GetElapse() >= 1000UL)
	{
		*nFPS = m_nFPS;
		m_nFPS = 0;
		Start();     //�������ÿ�ʼʱ��
		return true;
	}
	m_nFPS++;
	return false;
}
