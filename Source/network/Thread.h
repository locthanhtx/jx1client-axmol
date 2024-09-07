/********************************************************************
	created:	2003/02/14
	file base:	Thread
	file ext:	h
	author:		liupeng

	purpose:
*********************************************************************/
#ifndef __INCLUDE_THREAD_H__
#define __INCLUDE_THREAD_H__
#include "cocos2d.h"
#include "engine/KbugInfo.h"
USING_NS_AX;
#pragma once

/*
 * namespace OnlineGameLib::Win32
 */

/*
 * CThread
 */
#ifdef WIN32
class CThread
{
public:

	CThread();

	virtual ~CThread();

	void * GetHandle() const;

	void Wait() const;

	bool Wait(unsigned int timeoutMillis) const;

	void Start();

	void Terminate( unsigned int exitCode = 0 );

	virtual void Run() = 0;

private:
	static unsigned int __stdcall  ThreadFunction( void *pV );
	void * m_hThread;
	/*
	 * No copies do not implement
	 */
	CThread( const CThread &rhs );
	CThread &operator=( const CThread &rhs );

};

#else
//linux
#include <pthread.h>
#include <iostream>
#include <stdexcept>

class CThread {
public:
    CThread();
    ~CThread();

    pthread_t GetHandle() const;
    void Wait() const;
    void Start();
    void setAutoDelete(bool autoDelete);
    void Terminate(unsigned int exitCode);

protected:
    virtual void Run() = 0; // Virtual method to be implemented by derived classes

private:
    static void* ThreadFunction(void* pV);

    pthread_t a_thread;
    pthread_attr_t thread_attr;
    bool autoDelete_;
};
#endif //end linux
#endif //__INCLUDE_THREAD_H__
