//#include "stdafx.h"
#include "gamecore/KCore.h"
#include "SocketClient.h"
//#include "gamecore/KCore.h"
#include "gamecore/CoreShell.h"
//#include "Win32Exception.h"
//#include "Utils.h"
#include "Socket.h"
//#include "Macro.h"
#include <vector>
//#include "event.h"
#ifndef WIN32
#include "unistd.h"
#endif
//CEvent *m_ReadEvent = NULL;
extern iCoreShell*	g_pCoreShell; //ȫ�ֵ���
using std::vector;
/*
CEvent *_CreateEvent(bool bManualReset, bool bInitialState)
{
	CEvent *pEvent = new CEvent(bManualReset, bInitialState);
	assert(pEvent);

	bool bRet = pEvent->CreateEvent();
	assert(bRet);

	return pEvent;
}

unsigned int WaitForSingleObject(CEvent *pEvent, int cms)
{
	assert(pEvent);
	if( pEvent->Wait(cms) )
	{
		return 0;
	}

	return 1;
}
*/
CSocketClient::CSocketClient(
		const std::string &addressToConnectServer,
		unsigned short portToConnectServer,
		unsigned int maxFreeBuffers,
		unsigned int bufferSize, /* = 1024 */
		int mSocketNo)
	  : CIOBuffer::Allocator(bufferSize, maxFreeBuffers),
		m_address(addressToConnectServer),
		m_port(portToConnectServer),
		m_connectSocket(INVALID_SOCKET),
		m_SocketState(STATE_NONE),
		m_SocketNo(mSocketNo)
		//m_ReadEvent(_CreateEvent(true, true))
{
	 // m_ReadEvent = _CreateEvent(true, true);
#ifdef WIN32
	unsigned short wVersionRequested = 0x202;
	if ( 0 != ::WSAStartup(wVersionRequested, &m_data ))
	{
		messageBox("socket is fail..","socket");
		//throw CWin32Exception( _T("CUsesWinsock::CUsesWinsock()"), ::WSAGetLastError() );
	}
#endif
}

CSocketClient::CSocketClient(
		unsigned int maxFreeBuffers,
		unsigned int bufferSize, /* = 1024 */
		int   mSocketNo
		)
	: CIOBuffer::Allocator(bufferSize, maxFreeBuffers),
	m_port(0),
	m_connectSocket(INVALID_SOCKET),
	m_SocketNo(mSocketNo)
{
	//m_ReadEvent = _CreateEvent(true, true);
#ifdef WIN32
	unsigned short wVersionRequested = 0x202;
	if ( 0 != ::WSAStartup(wVersionRequested, &m_data ))
	{
		messageBox("socket is fail..","socket");
		//throw CWin32Exception( _T("CUsesWinsock::CUsesWinsock()"), ::WSAGetLastError() );
	}
#endif

}
void CSocketClient::Connect(
			   const std::string &addressToConnectServer,
			   unsigned short portToConnectServer
			   )
{
	m_address = addressToConnectServer;
	m_port = portToConnectServer;
}

CSocketClient::~CSocketClient()
{
	//if (m_ReadEvent)
	  // CloseHandle(m_ReadEvent);
	try
	{
		StopConnections();
	}
	catch(...)
	{
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	::WSACleanup();
#endif
}

bool CSocketClient::StartConnections()
{
	CCriticalSection::Owner lock(m_criticalSection_s);

	if (INVALID_SOCKET == m_connectSocket)
	{//����ǿ����ӵĻ� �ʹ���һ������
		/*
		 * Call to unqualified virtual function
		 */
		  // OnStartConnections();
		/*
		 * call to unqualified virtual function
		 */
		//messageBox("CreateConnectionSocket","Socket");
		m_connectSocket = CreateConnectionSocket(m_address, m_port);

		if (!WaitAndVerifyCipher())
		{//�ȴ���֤����
			//messageBox("WaitAndVerifyCipher false","Socket");
			return false;
		}

		//m_eventSelect.AssociateEvent( m_connectSocket, FD_CONNECT | FD_CLOSE | FD_READ );
		//m_ReadEvent->Set();
		m_SocketState   =STATE_START;
		return true;
	}
	return false;
}

void CSocketClient::StopConnections()
{
	CCriticalSection::Owner lock(m_criticalSection_s);

	if (INVALID_SOCKET != m_connectSocket)
	{
		/*
		 * Change the sockH to be blocking
		 */
		/*
		 * Force an abortive close.
		 */
		LINGER lingerStruct;

		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0;

		if (SOCKET_ERROR == ::setsockopt(m_connectSocket,
								SOL_SOCKET,
								SO_LINGER,
								(char *)&lingerStruct,
								sizeof(lingerStruct)))
		{
		}
		//�������ź� �Ͽ���
		//m_ReadEvent->Reset();
		//m_eventSelect.DissociateEvent();

#ifdef WIN32
	if (0!= ::closesocket(m_connectSocket))
		{
		}
#else
      close(m_connectSocket);
#endif

		m_connectSocket =INVALID_SOCKET;
		m_SocketState   =STATE_STOP;
		/*
		 * Call to unqualified virtual function
		 */
		OnStopConnections();	  //֪ͨ�������Ѿ��Ͽ�����
	}
}

SOCKET CSocketClient::CreateConnectionSocket(
					  const std::string &addressToConnectServer,
					  unsigned short port)
{
#ifdef WIN32
	//unsigned short wVersionRequested = 0x202;
	//WSAStartup(wVersionRequested, &wsaData);
	SOCKET s = ::WSASocket(AF_INET,SOCK_STREAM,IPPROTO_IP,NULL,0,0);
	unsigned int ul=1;
	int ret;
	ret=ioctlsocket(s,FIONBIO,(u_long *)&ul);//���óɷ�����ģʽ��
	if(ret==SOCKET_ERROR)//����ʧ�ܡ�
	{
	  messageBox("ioctlsocket is fail..","ioctlsocket");
	}
#else
	SOCKET s = socket(AF_INET, SOCK_STREAM,IPPROTO_IP);
	int flags = fcntl(s,F_GETFL,0);                        //��ȡ�ļ���flagsֵ��
	fcntl(s,F_SETFL,flags|O_NONBLOCK);                     //���óɷ�����ģʽ��
#endif
	char ninfo[64]={0};
	if ( INVALID_SOCKET == s )
	{
		sprintf(ninfo,"INVALID_SOCKET:%d",s);
		messageBox(ninfo,"CreateConnectionSocket");
		return INVALID_SOCKET;
	}

	//sprintf(ninfo,"SOCKET:%d",s);
	//messageBox(ninfo,"CreateConnectionSocket");

	CSocket connectionSocket(s);
#ifdef WIN32
	CSocket::InternetAddress localAddress(addressToConnectServer,port);
	//connectionSocket.NewConnect(localAddress);
	connectionSocket.Connect(localAddress);
#else
	connectionSocket.Connect_(addressToConnectServer,port);
#endif

	return connectionSocket.Detatch();
}

void CSocketClient::InitiateShutdown()
{
	/*
	 * Signal that the dispatch thread should shut down all worker threads and then exit
	 */
	//m_ReadEvent->Reset();
	StopConnections();
	/*
	 * Call to unqualified virtual function
	 */
	OnShutdownInitiated();
}

void CSocketClient::WaitForShutdownToComplete(int isCleartheThread)
{
	/*
	 * If we havent already started a shut down, do so...
	 */
#ifdef WIN32
	if (isCleartheThread)
		Terminate();
	else
	{
		InitiateShutdown();
		Terminate();
	}
#else
	if (isCleartheThread)
		Terminate(1);
	else
	{
		InitiateShutdown();
		//Wait();
		Terminate(1);
	}
#endif
}
/*
void CSocketClient::Run()
{//�������������� sk ��������
	//return;
	try
	{
		timeval pstTime = {0,0};
		unsigned long g_nServiceLoop = 0;
		while (true)
		{
		 if (++g_nServiceLoop & 0x80000000)
		 {
			g_nServiceLoop = 0;
		 }
		 if (g_nServiceLoop & 0x1)
		 {
#ifdef WIN32
			Sleep(1);     //����1����
#else
			usleep(1000); //΢�� ����=1000΢��  sleep Ϊ��
#endif
		 }
				//��½�������� �ʺŷ��������� ����������
		 if  (m_bIsGameSevConnecting && m_SocketNo==0)
			 continue;
		 CIOBuffer *pReadContext = Allocate();

		 if (m_SocketNo==1 && m_bIsGameSevConnecting)
	     {//��Ϸ������
				unsigned long dwNumBytes = 0;
	            unsigned long dwFlags = 0;
				if (!pReadContext)
					break;
	            pReadContext->SetupRead(); //���ýṹ������ݵĵ�ǰָ��
	            dwNumBytes = recv(m_connectSocket,pReadContext->GetWSABUF()->buf,pReadContext->GetWSABUF()->len,dwFlags);
	//���յ����ֽ�����
	            if (SOCKET_ERROR == dwNumBytes || 0 == dwNumBytes)
	            {//������� ���������Ѿ��Ͽ�
					//pReadContext->Release();
	            }
	            else if (dwNumBytes>0)
	            {//��ʼִ�пͻ��˺���
					pReadContext->Use(dwNumBytes);//���ó��������˶���
					//pReadContext->Use(dwNumBytes);//���ó��������˶���
					//������ݽ��� ��仺����
					//ReadCompleted(pReadContext);

					//const unsigned char *pPackData = pBuffer->GetBuffer();
					//unsigned int used = pBuffer->GetUsed();
					unsigned int nSize          = pReadContext->GetUsed();
					const unsigned char* pBuffer = pReadContext->GetBuffer();

					PROTOCOL_MSG_TYPE*	pMsg = (PROTOCOL_MSG_TYPE*)pBuffer; //��ǰ��

					while(pMsg < (PROTOCOL_MSG_TYPE*)(pBuffer + nSize))
					{//˧ѡ���ݰ�
						PROTOCOL_MSG_TYPE Msg = *pMsg;
						// ���������Э��
						if (Msg == s2c_notifyplayerexchange)
						{//ת����ͼ��ת��
							//ProcessSwitchGameSvrMsg(pMsg);
							break;
						}
						else if (g_pCoreShell)
						{
							char xieyi[64]={0};
							sprintf(xieyi,"%d:%d",Msg,dwNumBytes);

							messageBox(xieyi,"g_pCoreShell");
							if  (Msg <= s2c_clientbegin ||  Msg >= s2c_end || g_pCoreShell->GetProtocolSize(Msg)==0)
								break;
							if (Msg == s2c_syncworld)
								messageBox("s2c_syncworld...","s2c_syncworld");
							if (Msg == s2c_syncnpc)
								messageBox("s2c_syncnpc...","s2c_syncnpc");
							if (Msg == s2c_syncplayer)
								messageBox("s2c_syncplayer...","s2c_syncplayer");
							if (Msg == s2c_synonlineplayer) //�ղ���
								messageBox("s2c_synonlineplayer...","s2c_synonlineplayer");
							if (Msg == s2c_syncclientend)
								messageBox("s2c_syncclientend....","s2c_syncclientend");

							g_pCoreShell->NetMsgCallbackFunc(pMsg);          //���ܴӷ�����������Э����Ϣ

							if (g_pCoreShell->GetProtocolSize(Msg) > 0)
								pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + g_pCoreShell->GetProtocolSize(Msg));
							else //û�����Ƴ��ȵ� -1
								pMsg = (PROTOCOL_MSG_TYPE*)(((char*)pMsg) + PROTOCOL_MSG_SIZE + (*(unsigned short*)(((char*)pMsg) + PROTOCOL_MSG_SIZE)));
						}
	                 }//END WIHEL
			  }
		 }
		else
		{//�ʺŷ�������
			OnRead(pReadContext);
		}
		pReadContext->Release();
	  } // while ( ...
	}
	catch(...)
	{
		messageBox("run is catch Error","Run");
		StopConnections();
		//m_ReadEvent->Reset();
	}
	OnShutdownComplete();
	//return 1;
}
*/
void CSocketClient::Run()
{//�������������� sk ��������
	//return;
	try
	{
		timeval pstTime = {0,0};
		unsigned long g_nServiceLoop = 0;
		//fd_set fdRead={0};
		//fd_set fdRead;
		 while (true)
		 {

			//��½�������� �ʺŷ��������� ����������
			if (m_SocketNo==0 && m_bIsGameSevConnecting)
				continue;

			//FD_ZERO(&fdRead);
			//FD_SET(m_connectSocket,&fdRead);
			//memset(&fdRead,0,sizeof(fdRead));
			//fdRead.fd_count=0;
			//fdRead.fd_array[0] = m_connectSocket;
			//�Ƿ������ݿ���
			//int ret = select(m_connectSocket+1,&fdRead,NULL,NULL,0); //&pstTime
			//if (ret== SOCKET_ERROR || ret==0)
			//{//����
				//break;
			//}
			//else if (ret>0)
			//{
			   //if(FD_ISSET(m_connectSocket, &fdRead))
			   //{
		       //}
			//}
			if  (INVALID_SOCKET == m_connectSocket)
				break;

			CIOBuffer *pReadContext = Allocate();
			if (pReadContext)
			{
				OnRead(pReadContext);
				pReadContext->Release();
			}
#ifdef WIN32
			if (++g_nServiceLoop & 0x80000000)
			{
				g_nServiceLoop = 0;
			}
			if (g_nServiceLoop & 0x1)
				Sleep(10);       //����1����
#else
			usleep(10000); //΢�� ����=1000΢��  sleep Ϊ��
			//sleep(1);
#endif
		} // while ( ...
	}
	catch(...)
	{
		//messageBox("run is catch Error","Run");
		StopConnections();
		//m_ReadEvent->Reset();
	}
	//OnShutdownComplete();
	//return 1;
}

void CSocketClient::ReleaseBuffers()
{
	Flush();
}

void CSocketClient::OnError( const std::string &message )
{
}

void CSocketClient::OnRead( CIOBuffer *pBuffer )
{//���հ�
	unsigned long dwNumBytes = 0;
	unsigned long dwFlags = 0;
	pBuffer->SetupRead(); //���ýṹ������ݵĵ�ǰָ��
	dwNumBytes = recv(m_connectSocket,pBuffer->GetWSABUF()->buf,pBuffer->GetWSABUF()->len,dwFlags);
	//���յ����ֽ�����
	/*if (SOCKET_ERROR == dwNumBytes || 0 == dwNumBytes)
	{//������� ���������Ѿ��Ͽ�
		//StopConnections();
		return;
		unsigned long lastError = ::WSAGetLastError();
		if ( ERROR_IO_PENDING != lastError)
		{
			if (lastError == WSAECONNABORTED || //��������ʧ��
				lastError == WSAECONNRESET ||
				lastError == WSAEDISCON)        //Զ�˽��������ӡ�
			{

				//ECONNRESET��linux���������̲����Ĵ��󣬴�����Ϊ104��
				//WSAECONNRESET��windows���������̲����Ĵ��󣬴�����Ϊ10054
				//���߲�����ԭ��һ���������¼��������
				//- ���ն�recv����read�� �Զ��Ѿ��ر����ӣ�recv/read���ظô���
				//- �Զ��������ӣ���δ��������
				//- ���Ͷ��Ѿ��Ͽ����ӣ����ǵ���send�ᴥ���������

				//messageBox("read is  Error and stop sk","read");
				//StopConnections();
			}
		}
	}
	*/
	if (dwNumBytes>0)
	{
		pBuffer->Use(dwNumBytes);//���ó��������˶���
		//������ݽ��� ��仺����
		ReadCompleted(pBuffer);
	}
	else
	{
		if (SOCKET_ERROR == dwNumBytes || 0 == dwNumBytes)
		{//�������Ѿ��Ͽ�����
			//StopConnections();
			OnStopConnections();	  //֪ͨ�������Ѿ��Ͽ�����
			return;
		}
	}
}

void CSocketClient::Write( const char *pData, unsigned int dataLength )
{
	if ( INVALID_SOCKET != m_connectSocket &&
		dataLength > 0 &&
		pData )
	{
		CIOBuffer *pBuffer = Allocate();

		pBuffer->AddData( pData, dataLength );

		Write( pBuffer );

		pBuffer->Release();
	}
}

//static struct timeval gs_CheckRW_timeout = {0,0};
//��ʼ����
bool CSocketClient::Write( CIOBuffer *pBuffer )
{
	unsigned int uDataLength = 0;

	if (NULL== pBuffer ||\
		(0 == (uDataLength = pBuffer->GetUsed())) ||\
		INVALID_SOCKET == m_connectSocket)
	{
		return false;
	}

	/*
	 * Begin to send data
	 */

	int nError = 0;
	unsigned long lastError= 0;

	unsigned long dwFlags = 0;//MSG_NOSIGNAL;
	unsigned long dwSendNumBytes = 0;
	timeval gs_CheckRW_timeout = {0,0};
	pBuffer->SetupWrite();
	WSABUF &wsa = *(pBuffer->GetWSABUF());
		//fd_set writefds;
		//memset(&writefds, 0, sizeof(writefds));
		//writefds.fd_count = 1;
		//writefds.fd_array[0] = m_connectSocket;
		do
		{   //ѭ����� ָ�����Է���
			/*
			 * Check socket status //��� sk ״̬ ������Է��;ͷ���
			 */
/*
#ifdef WIN32
			FD_ZERO(&writefds);
			//FD_SET(m_connectSocket,&writefds);
			//memset( &writefds, 0, sizeof( writefds ) );
			writefds.fd_count = 1;
			writefds.fd_array[0] = m_connectSocket;   //
			nError = select(1,NULL,&writefds,NULL,0); //������������m_connectSocket+1
			//messageBox("select is end","send");
			if (SOCKET_ERROR==nError || 0 ==nError)
			{//����������� ֱ�ӷ�����
				//StopConnections();
				//messageBox("select is error","send");
				break;
			}

			if (nError>0)
#endif*/
			{//���Է�������
				//messageBox("send is ok","send");
				wsa.len		-= dwSendNumBytes;
				wsa.buf		+= dwSendNumBytes;//���͵��ӽ�
				uDataLength	-= dwSendNumBytes;
				dwSendNumBytes =send(m_connectSocket,wsa.buf,wsa.len,dwFlags);
				if (dwSendNumBytes == -1 || dwSendNumBytes == 0)
				{//�������Ͽ����򡡳���
					break;
				}
			   /*
		       * Continue to send buffer when 'dwSendNumBytes < uDataLength'
		       */
				if (SOCKET_ERROR != nError && dwSendNumBytes >= uDataLength)
				{//��������,ֱ�ӷ�����
					return true;
				}
			}
		} while (true);
	return false;
}

static const unsigned long g_dwTimeout = 5;//��

bool CSocketClient::WaitAndVerifyCipher()
{
	fd_set	fdRead  = { 0 };
	timeval stTime;
	timeval	*pstTime = NULL;
	if (-1 != g_dwTimeout)
	{
		stTime.tv_sec = g_dwTimeout; //��
		stTime.tv_usec = 0;
		pstTime = &stTime;
	}

	///m_ReadEvent->Set();
	/*
	 * Select function set read timeout
	 */
	SOCKET s = ( SOCKET )m_connectSocket;
	unsigned long dwTotalLength = 0;
	while (true)
	{
		unsigned long dwBytesRead = 0L;
		/*
		 * Set Descriptor
		 */
	    //fd_set fdRead;
		//memset(&fdRead,0,sizeof(fdRead));
		//fdRead.fd_count    = 0;
		//fdRead.fd_array[0] = s;
		FD_ZERO(&fdRead);
		FD_SET(s,&fdRead);
		int res;
#ifdef WIN32
		res = select(0,&fdRead,NULL,NULL,pstTime); //FD_SETSIZE
#else
		res = select(s+1,&fdRead,NULL,NULL,pstTime);
#endif
		if (res > 0)
		{
			res = recv(s,(char*)&m_theSendAccountBegin + dwTotalLength, sizeof( m_theSendAccountBegin ) - dwTotalLength,0);
			if (res > 0)
			{
				dwBytesRead = res;
				dwTotalLength += dwBytesRead;
			}
		}

		if (res <= 0)
		{
			/*
			 * Timeout and exit
			 */
			//m_ReadEvent->Reset();
			//char ninfo[64]={0};
			//sprintf(ninfo,"Timeout and exit:%d",s);
			//messageBox(ninfo,"WaitAndVerifyCipher");
			break;
		}

		if (dwTotalLength == sizeof(m_theSendAccountBegin))
		{
			ACCOUNT_BEGIN *pAccountBegin = ( ACCOUNT_BEGIN * )(&m_theSendAccountBegin.AccountBegin);

			if (pAccountBegin->ProtocolType == CIPHER_PROTOCOL_TYPE)
			{
				if (pAccountBegin->nMode != 0)
				{
					messageBox("Server send cliper mode error","cliper");
				/*
				* Write this message and then shutdown the sending side of the socket.
					*/

                #ifdef WIN32
					if (0 != ::closesocket(m_connectSocket))
					{
					}
                #else
                     close(m_connectSocket);
                #endif

					m_connectSocket = INVALID_SOCKET;
					return false;
				}
				m_uKeyMode   = pAccountBegin->nMode;
				m_uServerKey = ~(pAccountBegin->ServerKey);
				m_uClientKey = ~(pAccountBegin->ClientKey);
				//m_ReadEvent->Reset();
				return true;
			}
		}
	}
	//m_ReadEvent->Reset();
	return false;
}
