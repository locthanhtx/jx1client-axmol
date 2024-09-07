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
	bool Cleanup(int isCleartheThread=false);
	bool FsGameServerConnectTo(const char * const &pAddressToConnectServer,
		unsigned short usPortToConnectServer);
	//bool RegisterMsgFilter(void * lpParam,
		//CALLBACK_CLIENT_EVENT pfnEventNotify);
	//������Ϣ
	int	 SendMsg(const void *pBuffer, int nSize);
	bool SendPackToServer(const void * const pData,
		const unsigned int &datalength);
	const void * GetPackFromServer(unsigned int &datalength);
	bool Shutdown();
	void setBufferEmpty();
	/*
     *  IUnknown COM Interface Methods
     */
    bool QueryInterface(void** ppv);
    unsigned int AddRef();
    unsigned int Release();
	bool getSocketStates(){return __SocketStates;};
	void setSocketStates(bool ver){__SocketStates=ver;};
public:
	CGameClient( unsigned int maxFreeBuffers,
		unsigned int maxFreeBuffers_Cache,
		unsigned int bufferSize_Cache = 8192,
		unsigned int bufferSize = 1024,
		int    mSocketNo=0);
	virtual ~CGameClient();
	unsigned int GetMessageSize(const CIOBuffer *pBuffer) const;
protected:
	unsigned int GetMinimumMessageSize() const;

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
	long m_lRefCount;
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
   unsigned int AddRef();
   unsigned int Release();
   CGameClient *pObject;
public:
	CClientFactory();
	virtual ~CClientFactory();
private:
	long	m_lRefCount;
	unsigned int m_bufferSize;
	unsigned int m_MaxPlauerCount;
	unsigned int m_nPrecision;
};

#endif // __INCLUDE_CLIENTSTAGE_H__
