/********************************************************************
	created:	2003/02/14
	file base:	IOBuffer
	file ext:	h
	author:		liupeng
	
	purpose:	Header file for CIOBuffer class
*********************************************************************/
#ifndef __INCLUDE_IOBUFFER_H__
#define __INCLUDE_IOBUFFER_H__

#pragma once
/*
 * Identifier was truncated to '255' characters 
 * in the debug information
 */
#include "engine/KbugInfo.h"
#pragma warning(disable : 4786)                               
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <winsock2.h>
#endif

#include "CriticalSection.h" 
//#include "tstring.h"

#include "cocos2d.h"
#include "NodeList.h"
//#include "OpaqueUserData.h"
//#include "Macro.h"

#include <map>

/*
 * Nonstandard extension used : zero-sized array in struct/union
 */
#pragma warning(disable: 4200)

/*
 * namespace OnlineGameLib::Win32
 */

//namespace OnlineGameLib {
//namespace Win32 {

/*
 * CIOBuffer
 */
class CIOBuffer : 
	public OVERLAPPED, 
	public CNodeList::Node
	//public COpaqueUserData
{
public:
	
	class Allocator;
	friend class Allocator;
	
	class InOrderBufferList;
	
	WSABUF *GetWSABUF() const { return const_cast<WSABUF *>(&m_wsabuf);};
	
	uint32_t GetUsed() const { return m_used; };
	uint32_t GetSize() const { return m_size; };
	
	const BYTE *GetBuffer() const {return m_buffer_ptr;};
	
	void SetupZeroByteRead();
	void SetupRead();
	void SetupWrite();
	
	void AddData( const char * const pData, uint32_t dataLength );
	void AddData( const BYTE * const pData, uint32_t dataLength );
	void AddData( BYTE data );
	
	void Use( uint32_t dataUsed ) { m_used += dataUsed; };
	
	CIOBuffer *SplitBuffer( uint32_t bytesToRemove );
	void RemoveBuffer( uint32_t bytesToRemove );
	CIOBuffer *AllocateNewBuffer() const;
	
	void ConsumeAndRemove( uint32_t bytesToRemove );
	
	void Empty();
	
	void AddRef() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		::InterlockedIncrement(&m_ref);
#else
		//__sync_fetch_and_add(&m_ref, 1);
		m_ref++;
#endif
	};

	void Release();
	
	uint32_t GetOperation() const { return m_operation; };
	void SetOperation( uint32_t operation ) { m_operation = operation; };
	
	uint32_t GetSequenceNumber() const { return m_sequenceNumber; };
	void SetSequenceNumber( uint32_t sequenceNumber ) { m_sequenceNumber = sequenceNumber; };

private:

	uint32_t m_operation;
	uint32_t m_sequenceNumber;
	
	WSABUF m_wsabuf;
	
	Allocator &m_allocator;
	
	int32_t m_ref;
	const uint32_t m_size;
	uint32_t m_used;

	/*
	 * Start of the actual buffer, must remain the last
	 * data member in the class.
	 */
	BYTE *m_buffer_ptr;			// four bytes aligned
	
	//BYTE m_buffer_base_addr[0];

private:

	static void *operator new(size_t ojSize, size_t bufferSize );
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	static void operator delete( void *pObj, uint32_t bufferSize );
#else
	static void operator delete(void *pObject);
#endif

	
	CIOBuffer( Allocator &m_allocator, uint32_t size );
	~CIOBuffer();
	
	/*
     * No copies do not implement
     */
	CIOBuffer( const CIOBuffer &rhs );
	CIOBuffer &operator=( const CIOBuffer &rhs );
};

/*
 * CIOBuffer::Allocator
 */

class CIOBuffer::Allocator
{
public:

      friend class CIOBuffer;

      explicit Allocator( uint32_t bufferSize, uint32_t maxFreeBuffers );

      virtual ~Allocator();

      CIOBuffer *Allocate();

      uint32_t GetBufferSize() const { return m_bufferSize; };

protected:

      void Flush();

private:

      void Release( CIOBuffer *pBuffer );

      virtual void OnBufferCreated() {}
      virtual void OnBufferAllocated() {}
      virtual void OnBufferReleased() {}

      void DestroyBuffer( CIOBuffer *pBuffer );

      const uint32_t m_bufferSize;

      typedef TNodeList< CIOBuffer > BufferList;
      
      BufferList m_freeList;
      BufferList m_activeList;
      
      const uint32_t m_maxFreeBuffers;

      CCriticalSection m_criticalSection;

      /*
	   * No copies do not implement
	   */
      Allocator( const Allocator &rhs );
      Allocator &operator=( const Allocator &rhs );
};

/*
 * CIOBuffer::InOrderBufferList
 */

class CIOBuffer::InOrderBufferList
{
public:

      explicit InOrderBufferList( CCriticalSection &lock );

      void AddBuffer( CIOBuffer *pBuffer );

      void ProcessBuffer();

      CIOBuffer *ProcessAndGetNext();

      CIOBuffer *GetNext();
      CIOBuffer *GetNext( CIOBuffer *pBuffer );

      void Reset();

      bool Empty() const { return m_list.empty(); };

private:

      uint32_t m_next;
   
      typedef std::map< uint32_t, CIOBuffer * > BufferSequence;

      BufferSequence m_list;

      CCriticalSection &m_criticalSection;
};

inline void CIOBuffer::ConsumeAndRemove( uint32_t bytesToRemove )
{
	m_used -= bytesToRemove;

	memmove( m_buffer_ptr, m_buffer_ptr + bytesToRemove, m_used );
}

inline void CIOBuffer::SetupZeroByteRead()
{
	m_wsabuf.buf = reinterpret_cast< char * >( m_buffer_ptr );

	m_wsabuf.len = 0; 
}

inline void CIOBuffer::SetupRead()
{//���������ṹ��  WSABUF  ����
	if ( m_used == 0 )
	{
		m_wsabuf.buf = reinterpret_cast< char * >(m_buffer_ptr);
		m_wsabuf.len = m_size; 
	}
	else
	{
		m_wsabuf.buf = reinterpret_cast< char * >(m_buffer_ptr) + m_used;
		m_wsabuf.len = m_size - m_used;
   }
}

inline void CIOBuffer::SetupWrite()
{//����
	m_wsabuf.buf = reinterpret_cast< char * >(m_buffer_ptr);
	m_wsabuf.len = m_used;

	m_used = 0;
}

//} // End of namespace OnlineGameLib
//} // End of namespace Win32


#endif //__INCLUDE_IOBUFFER_H__
