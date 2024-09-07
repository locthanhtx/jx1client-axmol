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
#ifdef WIN32
#include <pthreadwin32.h>
#else
#include <pthread.h>
#endif
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
	bool		Open(const char* pszPackFileName, int nSelfIndex);
	//bool		OpenX(const char* pszPackFileName, int nSelfIndex);
	//�رհ��ļ�
	void		Close();
	//���Ұ��ڵ����ļ�
	bool		XFindElemFile(unsigned int uId, XPackElemFileRef& ElemRef);
	//��ȡ���ڵ����ļ�
	int			XElemFileRead(XPackElemFileRef& ElemRef, void* pBuffer, unsigned uSize);

	//��ȡspr�ļ�ͷ��������spr
	SPRHEAD*	GetSprHeader(XPackElemFileRef& ElemRef, SPROFFS*& pOffsetTable);
	//��ȡ��֡ѹ����spr��һ֡������
	SPRFRAME*	GetSprFrame(SPRHEAD* pSprHeader, int nFrame,unsigned int &nSingFrameSize);
    std::string fileName;

private:
	//ֱ�Ӷ�ȡ���ļ������е����ݵ�������
	bool		DirectRead(void* pBuffer, unsigned int uOffset, unsigned int uSize) const;
	//����ѹ�ض�ȡ���ļ���������
	bool		ExtractRead(void* pBuffer, unsigned int uExtractSize,
						int lCompressType, unsigned int uOffset, unsigned int uSize) const;
	//���������в������ļ���
	int			XFindElemFileA(unsigned int ulId) const;
	//��cache��������ļ�
	int         ucl_nrv2e_decompress_8_fs(unsigned char * s_buf,int s_size,unsigned char * d_buf,unsigned int *d_size) const;
	int         ucl_nrv2e_decompress_8_3(unsigned char * a1, unsigned int a2, unsigned char * a3, unsigned int *a4) const;
	int         ucl_nrv2e_decompress_8_2(unsigned char * a1, unsigned int a2, unsigned char *a3, unsigned int *a4) const;
	int         ucl_nrv2e_decompress_8_1(unsigned char * a1, unsigned int a2, unsigned char *a3, unsigned int *a4) const;
	int         ucl_decompress_8_New(unsigned char * a1, unsigned int a2, unsigned char * a3, unsigned int *a4) const;
	int			FindElemFileInCache(unsigned int uId, int nDesireIndex);
	//�����ļ�������ӵ�cache
	int			AddElemFileToCache(void* pBuffer, int nElemIndex);
	//����һ��������������ָ�������ļ����ݶ�������
	void*		ReadElemFile(int nElemIndex) const;
	//�ͷ�һ��cache��������
	static void	FreeElemCache(int nCacheIndex);

private:
	//HANDLE					m_hFile;			//���ļ����
	FILE *                  m_hFile;
	unsigned int			m_uFileSize;		//���ļ���С
	int						m_nElemFileCount;	//���ļ��ĸ���
	int						m_nSelfIndex;		//���ļ��Լ��ڰ������е�����
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
		unsigned int	uId;				//���ļ�id
		int			lSize;				//���ļ���С
		int				nPackIndex;			//�����ĸ����ļ�
		int				nElemIndex;			//���ļ��������б��е�λ��
		unsigned int	uRefFlag;			//�������ñ��
	};

	#define	MAX_XPACKFILE_CACHE			10
	//���ļ���cache����
	static	XPackElemFileCache	ms_ElemFileCache[MAX_XPACKFILE_CACHE];
	//���ļ���cache����Ŀ
	static	int					ms_nNumElemFileCache;
};


//====UCLѹ���㷨����====
/*extern "C"
{
	ENGINE_API int CD_LCU_I();
	ENGINE_API int CD_LCU_C(const unsigned char* pSrcBuffer, unsigned int nSrcLen,
		unsigned char* pDestBuffer, unsigned int* pDestLen, int nCompressLevel);
	ENGINE_API int CD_LCU_D(const unsigned char* pSrcBuffer, unsigned nSrcLen,
		unsigned char* pDestBuffer, unsigned int uExtractSize);
}*/

#endif
