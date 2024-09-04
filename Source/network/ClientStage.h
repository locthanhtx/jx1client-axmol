/********************************************************************
	created:	2003/03/24
	file base:	ClientStage
	file ext:	h
	author:		liupeng
	
	purpose:	
*********************************************************************/
#ifndef __INCLUDE_CLIENTSTAGE_H__
#define __INCLUDE_CLIENTSTAGE_H__
#pragma once
#include "engine/KbugInfo.h"
#include "SocketClient.h"
//#include "tstring.h"
//#include "Event.h"
#include "IOBuffer.h"
/*
 * class CGameClient
 */
class CGameClient : public CSocketClient
{
public:
	bool Startup();
	bool Cleanup(int32_t isCleartheThread=false);
	bool FsGameServerConnectTo(const char * const &pAddressToConnectServer,
		unsigned short usPortToConnectServer);
	//bool RegisterMsgFilter(void * lpParam,
		//CALLBACK_CLIENT_EVENT pfnEventNotify);
	//������Ϣ
	int32_t	 SendMsg(const void *pBuffer, int32_t nSize);
	bool SendPackToServer(const void * const pData,
		const uint32_t &datalength);
	const void * GetPackFromServer(uint32_t &datalength);
	bool Shutdown();
	void setBufferEmpty();
	/*
     *  IUnknown COM Interface Methods
     */
    bool QueryInterface(void** ppv);
    uint32_t AddRef();
    uint32_t Release();
	bool getSocketStates(){return __SocketStates;};
	void setSocketStates(bool ver){__SocketStates=ver;};
public:
	CGameClient( uint32_t maxFreeBuffers,
		uint32_t maxFreeBuffers_Cache,
		uint32_t bufferSize_Cache = 8192,
		uint32_t bufferSize = 1024,
		int32_t    mSocketNo=0);
	virtual ~CGameClient();
	uint32_t GetMessageSize(const CIOBuffer *pBuffer) const;
protected:
	uint32_t GetMinimumMessageSize() const;

	CIOBuffer *ProcessDataStream(CIOBuffer *pBuffer);
	void ProcessCommand(const CIOBuffer *pBuffer);
private:
	bool __SocketStates;
	virtual void OnStartConnections();
	virtual void OnStopConnections();
	virtual void OnShutdownComplete();
	virtual void OnShutdownInitiated();
	virtual void ReadCompleted(CIOBuffer *pBuffer);
private:
	int32_t m_lRefCount;
	CIOBuffer::Allocator	m_theCacheAllocator;
	void *					m_lpCallBackParam;
	//CALLBACK_CLIENT_EVENT	m_pfnCallbackClientEvent;
	CCriticalSection m_csReadAction;
	CIOBuffer		*m_pRecvBuffer;
	CIOBuffer		*m_pReadBuffer;
	/*
	 * No copies do not implement
	 */
	CGameClient(const CGameClient &rhs);
	CGameClient &operator=(const CGameClient &rhs);
};

/*
 * CClientFactory
 */
class CClientFactory //: public IClientFactory
{
public:
	/*
     *  IUnknown COM Interface Methods
     */
   uint32_t AddRef();
   uint32_t Release();
   CGameClient *pObject;
public:
	CClientFactory();
	virtual ~CClientFactory();
private:
	int32_t	m_lRefCount;
	uint32_t m_bufferSize;
	uint32_t m_MaxPlauerCount;
	uint32_t m_nPrecision;
};

#endif // __INCLUDE_CLIENTSTAGE_H__
