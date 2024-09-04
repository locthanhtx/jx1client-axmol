/*****************************************************************************************
//	���ļ���ȡ
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-9-16
------------------------------------------------------------------------------------------
	XPackFile֧�ֶ��̵߳�ͬʱ���ʡ�
	XPackFile�ⲿͨ��XPackElemFileRef���������õĶ԰������ļ������á�
	�ⲿ����������Ķ��������ṹ�ڵ����ݣ��Ի�������İ����ʹ��ܡ�
*****************************************************************************************/
#ifndef _XPACK_FILE_H_
#define	_XPACK_FILE_H_

#include "cocos2d.h"
USING_NS_AX;

#include "KbugInfo.h"
#include <pthread.h>
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#define CRITICAL_SECTION pthread_mutex_t
#endif


//#include "KSprite.h"

class XPackFile
{
public:
	XPackFile();
	~XPackFile();
	//�򿪰��ļ�
	bool		Open(const char* pszPackFileName, int32_t nSelfIndex);
	//bool		OpenX(const char* pszPackFileName, int32_t nSelfIndex);
	//�رհ��ļ�
	void		Close();
	//���Ұ��ڵ����ļ�
	bool		XFindElemFile(uint32_t uId, XPackElemFileRef& ElemRef);
	//��ȡ���ڵ����ļ�
	int32_t			XElemFileRead(XPackElemFileRef& ElemRef, void* pBuffer, unsigned uSize);

	//��ȡspr�ļ�ͷ��������spr
	SPRHEAD*	GetSprHeader(XPackElemFileRef& ElemRef, SPROFFS*& pOffsetTable);
	//��ȡ��֡ѹ����spr��һ֡������
	SPRFRAME*	GetSprFrame(SPRHEAD* pSprHeader, int32_t nFrame,uint32_t &nSingFrameSize);

private:
	//ֱ�Ӷ�ȡ���ļ������е����ݵ�������
	bool		DirectRead(void* pBuffer, uint32_t uOffset, uint32_t uSize) const;
	//����ѹ�ض�ȡ���ļ���������
	bool		ExtractRead(void* pBuffer, uint32_t uExtractSize,
						int32_t lCompressType, uint32_t uOffset, uint32_t uSize) const;
	//���������в������ļ���
	int32_t			XFindElemFileA(uint32_t ulId) const;
	//��cache��������ļ�
	int32_t         ucl_nrv2e_decompress_8_fs(unsigned char * s_buf,int32_t s_size,unsigned char * d_buf,uint32_t *d_size) const;
	int32_t         ucl_nrv2e_decompress_8_3(unsigned char * a1, uint32_t a2, unsigned char * a3, uint32_t *a4) const;
	int32_t         ucl_nrv2e_decompress_8_2(unsigned char * a1, uint32_t a2, unsigned char *a3, uint32_t *a4) const;
	int32_t         ucl_nrv2e_decompress_8_1(unsigned char * a1, uint32_t a2, unsigned char *a3, uint32_t *a4) const;
	int32_t         ucl_decompress_8_New(unsigned char * a1, uint32_t a2, unsigned char * a3, uint32_t *a4) const;
	int32_t			FindElemFileInCache(uint32_t uId, int32_t nDesireIndex);
	//�����ļ�������ӵ�cache
	int32_t			AddElemFileToCache(void* pBuffer, int32_t nElemIndex);
	//����һ��������������ָ�������ļ����ݶ�������
	void*		ReadElemFile(int32_t nElemIndex) const;
	//�ͷ�һ��cache��������
	static void	FreeElemCache(int32_t nCacheIndex);

private:
	//HANDLE					m_hFile;			//���ļ����
	FILE *                  m_hFile;
	uint32_t			m_uFileSize;		//���ļ���С
	int32_t						m_nElemFileCount;	//���ļ��ĸ���
	int32_t						m_nSelfIndex;		//���ļ��Լ��ڰ������е�����
	struct XPackIndexInfo*	m_pIndexList;		//���ļ������б�
  //struct XPackIndexInfoX*	m_pIndexListX;		//���ļ������б�
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	 pthread_mutex_t      m_ReadCritical;
#else
	 CRITICAL_SECTION     m_ReadCritical;		//�������ļ�ʱ���ٽ�������
#endif
	//----���ļ�����cache----
	struct XPackElemFileCache
	{
		void*			pBuffer;			//�������ļ����ݵĻ�����
		uint32_t	uId;				//���ļ�id
		int32_t			lSize;				//���ļ���С
		int32_t				nPackIndex;			//�����ĸ����ļ�
		int32_t				nElemIndex;			//���ļ��������б��е�λ��
		uint32_t	uRefFlag;			//�������ñ��
	};

	#define	MAX_XPACKFILE_CACHE			10
	//���ļ���cache����
	static	XPackElemFileCache	ms_ElemFileCache[MAX_XPACKFILE_CACHE];
	//���ļ���cache����Ŀ
	static	int32_t					ms_nNumElemFileCache;
};


//====UCLѹ���㷨����====
/*extern "C"
{
	ENGINE_API int32_t CD_LCU_I();
	ENGINE_API int32_t CD_LCU_C(const unsigned char* pSrcBuffer, uint32_t nSrcLen,
		unsigned char* pDestBuffer, uint32_t* pDestLen, int32_t nCompressLevel);
	ENGINE_API int32_t CD_LCU_D(const unsigned char* pSrcBuffer, unsigned nSrcLen,
		unsigned char* pDestBuffer, uint32_t uExtractSize);
}*/

#endif
