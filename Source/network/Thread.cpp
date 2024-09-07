//#include "stdafx.h"
#include "Thread.h"
//
#ifdef WIN32
//#include "Macro.h"
#include <process.h>		//Thread define
CThread::CThread()
	: m_hThread( NULL )
{

}
CThread::~CThread()
{

#ifdef WIN32
	try{ if (m_hThread) {CloseHandle(m_hThread); (m_hThread) = NULL; } } catch(...) { }
#else
	SAFE_CLOSEHANDLE(m_hThread);
#endif
}

void * CThread::GetHandle() const
{
	return m_hThread;
}

void CThread::Start()
{
	if (m_hThread ==NULL)
	{
		unsigned int threadIDc = 0;
		//??????????
		m_hThread = (void *)::_beginthreadex(0,
			0,
			ThreadFunction,
			(void *)this,
			0,
			&threadIDc);

		char ninfo[64]={0};
		if (m_hThread == NULL)
		{
			return;
		}
	}
}

void CThread::Wait() const
{
	if (!Wait(INFINITE))
	{
//		throw CException(_T("CThread::Wait()"), _T("Unexpected timeout on infinite wait"));
	}
}

bool CThread::Wait(unsigned int timeoutMillis) const
{
	bool ok;

	if ( !m_hThread )
	{
		return true;
	}

	unsigned long result = ::WaitForSingleObject( m_hThread, timeoutMillis );

	if ( result == WAIT_TIMEOUT )
	{
		ok = false;
	}
	else if ( result == WAIT_OBJECT_0 )
	{
		ok = true;
	}
	else
	{
		//throw CWin32Exception( _T( "CThread::Wait() - WaitForSingleObject" ), ::GetLastError() );
	}

	return ok;
}
//????????? __stdcall
unsigned int __stdcall  CThread::ThreadFunction( void *pV )
{
	int result = 0;

	CThread* pThis = ( CThread * )pV;

	if (pThis)
	{
		try
		{
			//result =
			pThis->Run();
		}
		catch(...)
		{
		}
	}

	return result;
}
//??????
void CThread::Terminate(unsigned int exitCode /* = 0 */)
{
	if (m_hThread && !::TerminateThread(m_hThread,exitCode))
	{
		//TRACE( "CThread::Terminate error!" );
	}

	//SAFE_CLOSEHANDLE(m_hThread);
	try{ if (m_hThread) {CloseHandle(m_hThread); (m_hThread) = NULL; } } catch(...) { }
}
#else
//linux
#include <pthread.h>
#include <iostream>
#include <stdexcept>

CThread::CThread() : autoDelete_(false) {
    if (pthread_attr_init(&thread_attr) != 0) {
        throw std::runtime_error("Failed to initialize thread attributes");
    }
}

CThread::~CThread() {
    // Ensure the thread is properly joined if needed
    pthread_attr_destroy(&thread_attr);
}

pthread_t CThread::GetHandle() const {
    return a_thread;
}

void CThread::Wait() const {
    if (pthread_join(a_thread, nullptr) != 0) {
        throw std::runtime_error("Failed to join thread");
    }
}

void CThread::Start() {
    int result = pthread_create(&a_thread, &thread_attr, ThreadFunction, this);
    if (result != 0) {
        throw std::runtime_error("Failed to create thread");
    }
}

void CThread::setAutoDelete(bool autoDelete) {
    autoDelete_ = autoDelete;
}

void CThread::Terminate(unsigned int exitCode) {
    if (exitCode)
        pthread_attr_destroy(&thread_attr);
}

void* CThread::ThreadFunction(void* pV) {
    CThread* pThis = static_cast<CThread*>(pV);
    pThis->Run();
    if (pThis->autoDelete_) {
        delete pThis;
    }
    return nullptr;
}
#endif  //end linxu
