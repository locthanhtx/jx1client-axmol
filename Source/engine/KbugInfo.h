#ifndef KstrFile_H
#define KstrFile_H

#include "cocos2d.h"
//#include "tchar.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#pragma warning (disable: 4005)
USING_NS_AX;

typedef struct _SECURITY_ATTRIBUTES_S {
	unsigned int nLength;
	void * lpSecurityDescriptor;
	bool bInheritHandle;
} SECURITY_ATTRIBUTES_S, *PSECURITY_ATTRIBUTES_S, *LPSECURITY_ATTRIBUTES_S;

//typedef unsigned short *PWORD;
//#define unsigned long* * unsigned long
//typedef unsigned int  *unsigned long*;
//typedef unsigned int  *LPDWORD;

#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2
#define IN
#define OUT
//#define BYTE unsigned char
#define MAX_PATH 1200
//#define UINT unsigned int
//#define INT  int
//#define FALSE false
//#define TRUE  true
//
//#define PVOID void *
//
//#define int  int
//#define PBYTE unsigned char *
//#define LPBYTE PBYTE
////typedef unsigned char BYTE;
//
////UINT_PTR
//#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
//#define INVALID_FILE_SIZE ((unsigned long)0xFFFFFFFF)

#define MAX_CHUNK	250		//����ջ��   250
#define HANDLE void *

#ifndef WIN32
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
enum enumServerConnectInfo
{
	enumServerConnectCreate = 0x100,
	enumServerConnectClose
};
#define CP_UTF8 65001
typedef int* LPINT;
#define FLOAT float
#define LPCTSTR const char *
#define CONST const
#define SOCKET_ERROR -1
#define  VOID void
#define CALLBACK __stdcall
#define  CHAR char
#define  ULONG unsigned int
typedef int SOCKET;
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL;}}
#define RELEASE(a) if(a){a->Release();a=NULL;}
#define SAFE_DELETE(a) if(a){delete a;a=NULL;}
#define SAFE_CLOSEHANDLE(x)		try{ if (x) {close(x); (x) = NULL; } } catch(...) { }
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
#define INVALID_SOCKET -1
#define ZeroMemory(x,y)  memset((x),0,(y))
//typedef struct fd_set FD_SET;
//typedef struct fd_set FD_SET;
//#define FD_ZERO(set) (((fd_set FAR *)(set))->fd_count=0)

typedef struct timeval TIMEVAL;
typedef struct linger LINGER;
typedef struct sockaddr_in SOCKADDR_IN;

typedef struct {
    ULONG len;     /* the length of the buffer */
    CHAR *buf; /* the pointer to the buffer __field_bcount(len) */
}WSABUF, * LPWSABUF;

//typedef void (CALLBACK *CALLBACK_CLIENT_EVENT )(void* lpParam,const unsigned int &ulnEventType);
typedef struct
{
	uint32_t   Internal;
	uint32_t   InternalHigh;
	union
	{
		struct
		{
			unsigned int Offset;
			unsigned int OffsetHigh;
		};
		void * Pointer;
	};
	 void * hEvent;
}OVERLAPPED;

typedef struct tagSIZE
{
    int        cx;
    int        cy;
} SIZE, *PSIZE, *LPSIZE;

typedef struct
{
	int  x;
	int  y;
}POINT;

typedef struct tagRECT
{
	int    left;
	int    top;
	int    right;
	int    bottom;
}RECT;
//#define MAKELONG(a, b) ((int)(((unsigned short)(((DWORD_PTR)(a)) & 0xffff)) | ((unsigned long)((unsigned short)(((DWORD_PTR)(b)) & 0xffff))) << 16))
//#define LOWORD(l)      ((unsigned short)(((DWORD_PTR)(l)) & 0xffff))
//#define HIWORD(l)      ((unsigned short)((((DWORD_PTR)(l)) >> 16) & 0xffff))
//#define LOBYTE(w)      ((BYTE)(((DWORD_PTR)(w)) & 0xff))
//#define HIBYTE(w)      ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

#    define MAKEWORD(a, b) \
        ((uint16_t)(((uint8_t)((uintptr_t)(a) & 0xff)) | ((uint16_t)((uint8_t)((uintptr_t)(b) & 0xff))) << 8))
#    define MAKELONG(a, b) \
        ((int32_t)(((uint16_t)((uintptr_t)(a) & 0xffff)) | ((uint32_t)((uint16_t)((uintptr_t)(b) & 0xffff))) << 16))
#    define LOWORD(l) ((uint16_t)(((uintptr_t)(l)) & 0xffff))
#    define HIWORD(l) ((uint16_t)((((uintptr_t)(l)) >> 16) & 0xffff))
#    define LOBYTE(w) ((uint8_t)(((uintptr_t)(w)) & 0xff))
#    define HIBYTE(w) ((uint8_t)((((uintptr_t)(w)) >> 8) & 0xff))

// cần chỉnh sửa lại để đồng bộ từ win32 và GNU (x64)
typedef uint32_t WPARAM;
typedef int32_t LPARAM;
typedef int32_t LRESULT;

typedef unsigned short* PWORD;
typedef uint32_t* PDWORD;
typedef uint32_t* LPDWORD;

#    define BYTE unsigned char
#    define PBYTE unsigned char*
#    define UINT  uint32_t
#    define INT    int32_t
#    define FALSE false
#    define TRUE  true
#    define DWORD  uint32_t
#    define WORD   unsigned short
#    define BOOL   int32_t
#    define LPCSTR const char*
#    define PVOID  void*
#    define LPVOID PVOID
#    define LPSTR  char*
#    define LONG   int32_t
#    define PBYTE  unsigned char*
#    define LPBYTE PBYTE
#    define LPTSTR LPSTR
// typedef unsigned char BYTE;
typedef int32_t LONG_PTR;
typedef uint32_t DWORD_PTR;
typedef uint32_t DWORD_PTRD;
typedef uint32_t UINT_PTR;
typedef uint32_t ULONG_PTR;
#    define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#    define INVALID_FILE_SIZE    ((unsigned long)0xFFFFFFFF)
#endif //end #ifndef WIN32 #ifndef WIN32 #ifndef WIN32 #ifndef WIN32

typedef struct {
	BYTE		Blue;		// ��ɫ����
	BYTE		Green;		// ��ɫ����
	BYTE		Red;		// ��ɫ����
	BYTE		Alpha;		// ͸������
} KPAL32;					// 32λɫ��
//---------------------------------------------------------------------------
typedef struct {
	BYTE		Red;		// ��ɫ����
	BYTE		Green;		// ��ɫ����
	BYTE		Blue;		// ��ɫ����
} KPAL24;					// 24λɫ��
//---------------------------------------------------------------------------
typedef unsigned short	KPAL16;		// 16λɫ��

enum eImageFormat
{
	IMAGE_FORMAT_NULL,
	IMAGE_8888,
	IMAGE_888,
	IMAGE_565,
	IMAGE_4444,
	IMAGE_256,
};

struct stImageInfo    //ͼƬ��������
{
	unsigned int			nDirections;
	unsigned int			nFrames;
	int				bytes_per_pixel;
	int			size;
	BYTE*			buffer;
	int			width,height;
	int				x,y;
};
typedef struct
{
	unsigned short bfType; // λͼ�ļ������ͣ�����ΪBM(1-2�ֽڣ�
	unsigned long bfSize; // λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ��3-6�ֽڣ�
	unsigned short bfReserved1; // λͼ�ļ������֣�����Ϊ0(7-8�ֽڣ�
	unsigned short bfReserved2; // λͼ�ļ������֣�����Ϊ0(9-10�ֽڣ�
	unsigned long bfOffBits; // λͼ���ݵ���ʼλ�ã��������λͼ��11-14�ֽڣ�
	// �ļ�ͷ��ƫ������ʾ�����ֽ�Ϊ��λ
} BITMAPFILEHEADERS;

typedef struct
{
	unsigned long biSize; // ���ṹ��ռ���ֽ�����15-18�ֽڣ�
	int biWidth; // λͼ�Ŀ�ȣ�������Ϊ��λ��19-22�ֽڣ�
	int biHeight; // λͼ�ĸ߶ȣ�������Ϊ��λ��23-26�ֽڣ�
	unsigned short biPlanes; // Ŀ���豸�ļ��𣬱���Ϊ1(27-28�ֽڣ�
	unsigned short biBitCount;// ÿ�����������λ����������1��˫ɫ������29-30�ֽڣ�
	// 4(16ɫ����8(256ɫ����24�����ɫ��֮һ
	unsigned long biCompression;  // λͼѹ�����ͣ������� 0����ѹ��������31-34�ֽڣ�
	// 1(BI_RLE8ѹ�����ͣ���2(BI_RLE4ѹ�����ͣ�֮һ
	unsigned long biSizeImage;    // λͼ�Ĵ�С�����ֽ�Ϊ��λ��35-38�ֽڣ�
	int biXPelsPerMeter; // λͼˮƽ�ֱ��ʣ�ÿ����������39-42�ֽڣ�
	int biYPelsPerMeter; // λͼ��ֱ�ֱ��ʣ�ÿ����������43-46�ֽ�)
	unsigned long biClrUsed;      // λͼʵ��ʹ�õ���ɫ���е���ɫ����47-50�ֽڣ�
	unsigned long biClrImportant; // λͼ��ʾ��������Ҫ����ɫ����51-54�ֽڣ�
} BITMAPINFOHEADERS;

typedef struct {
	BYTE		Red;		// ��ɫ����
	BYTE		Green;		// ��ɫ����
	BYTE		Blue;		// ��ɫ����
} KPAL244;					// 24λɫ��
typedef struct
{
	BYTE rgbBlue;    // ��ɫ�����ȣ�ֵ��ΧΪ0-255)
	BYTE rgbGreen;   // ��ɫ�����ȣ�ֵ��ΧΪ0-255)
	BYTE rgbRed;     // ��ɫ�����ȣ�ֵ��ΧΪ0-255)
	BYTE rgbReserved;// ����������Ϊ0
} RGBQUADS;

typedef struct
{
	BITMAPINFOHEADERS bmiHeader;    // λͼ��Ϣͷ
	KPAL244          bmiColors[1]; // ��ɫ��
} BITMAPINFOS;

typedef struct
{
	/* //����
	BYTE	Comment[4];	// ע������(SPR\0)
	unsigned short	Width;		// ͼƬ���
	unsigned short	Height;		// ͼƬ�߶�
	int 	CenterX;	// ���ĵ�ˮƽλ��
	int 	CenterY;	// ���ĵĴ�ֱλ��
	unsigned short	Frames;		// ��֡��
	unsigned short	Colors;		// ��ɫ��
	unsigned short	Directions;	// ������
	unsigned short	nInterval;	// ÿ֡���������Ϸ֡Ϊ��λ��
	unsigned short	Reserved[6];// �����ֶΣ����Ժ�ʹ�ã�*/
	BYTE	Comment[4];	// ע������(SPR\0)
	unsigned short	Width;		// ͼƬ���
	unsigned short	Height;		// ͼƬ�߶�
	unsigned short	CenterX;	// ���ĵ�ˮƽλ��
	unsigned short	CenterY;	// ���ĵĴ�ֱλ��
	unsigned short	Frames;		// ��֡��
	unsigned short	Colors;		// ��ɫ��
	unsigned short	Directions;	// ������
	unsigned short	nInterval;	// ÿ֡���������Ϸ֡Ϊ��λ��
	unsigned short	Reserved[6];// �����ֶΣ����Ժ�ʹ�ã�
} SPRHEAD;

#define	SPR_COMMENT_FLAG				0x525053	//'SPR'

//---------------------------------------------------------------------------
typedef struct
{
	unsigned long	Offset;		// ÿһ֡��ƫ��
	unsigned long	Length;		// ÿһ֡�ĳ���
} SPROFFS;

//---------------------------------------------------------------------------
typedef struct
{
	unsigned short	Width;		// ֡��С���
	unsigned short	Height;		// ֡��С�߶�
	unsigned short	OffsetX;	// ˮƽλ�ƣ������ԭͼ���Ͻǣ�
	unsigned short	OffsetY;	// ��ֱλ�ƣ������ԭͼ���Ͻǣ�
	BYTE	Sprite[1];	// RLEѹ��ͼ������
} SPRFRAME;

enum enumSubDir{sdFont=0,sdMaps,sdScript,sdResource,sdSetting,sdSound,sdSpr,sdUi,sdLastDir};
//һ��Pack�ļ����е�ͷ�ṹ:��PAKͷ
struct XPackFileHeader
{
	unsigned char cSignature[4];		//�ĸ��ֽڵ��ļ���ͷ��־���̶�Ϊ�ַ���'PACK'
	unsigned int uCount;				//�ļ����ݵ���Ŀ��
	unsigned int uIndexTableOffset;	//������ƫ����
	unsigned int uDataOffset;			//���ݵ�ƫ����
	unsigned int uCrc32;				//У���
	unsigned int  uPakTime;			    //����ļ�����ʱ��ʱ�䣬��Ϊ��λtime()
	unsigned char cReserved[8];		    //�������ֽ�
};

#define	XPACKFILE_SIGNATURE_FLAG		0x4b434150	//'PACK'
//Pack�ж�Ӧÿ�����ļ���������Ϣ��
struct XPackIndexInfo
{
	unsigned int	uId;				//���ļ�id
	unsigned int	uOffset;			//���ļ��ڰ��е�ƫ��λ��
	int			lSize;				//���ļ���ԭʼ��С
	int			lCompressSizeFlag;	//���ļ�ѹ����Ĵ�С��ѹ������
//	FILETIME        UpdateTime;
	//����ֽڱ�ʾѹ����������XPACK_METHOD
	//�͵������ֽڱ�ʾ���ļ�ѹ����Ĵ�С
};


//pak���б����spr֡��Ϣ��
struct XPackSprFrameInfo
{
	int lCompressSize;
	int lSize;
} ;

//�ֿ�ѹ�����ļ����ļ����ݴ洢���洢��������˳��һ��Ϊ��
// [XPackFileFragmentElemHeader] + [��һ������] + [�ڶ�������] + .. + [��n������] + [n��XPackFileFragmentInfo������]

//�ֿ�ѹ�����ļ��ڸ��ļ����ݴ洢����ʼλ�õ�ͷ���ݽṹ
struct XPackFileFragmentElemHeader
{
	int				nNumFragment;		//�ֿ����Ŀ
	int				nFragmentInfoOffest;//�ֿ���Ϣ���ƫ��λ��,�����XPackFileFragmentElemHeader��ʼλ�õ�ƫ��
};

//�ֿ�ѹ���ļ��ķֿ���Ϣ
struct XPackFileFragmentInfo
{
	unsigned int	uOffset;			//�˷ֿ����ݵ���ʼ�������ļ��������е�ƫ��λ��,�����XPackFileFragmentElemHeader��ʼλ�õ�ƫ��
	unsigned int	uSize;				//�˷ֿ��ԭʼ��С
	unsigned int	uCompressSizeFlag;	//�˷ֿ�ѹ����Ĵ�С��ѹ��������������XPackIndexInfo�ṹ�е�uCompressSizeFlag
};

//���ļ���ѹ����ʽ
enum XPACK_METHOD
{
	    TYPE_NONE	           = 0x00000000,			//û��ѹ��
		TYPE_UCL	           = 0x10000000,			//���ļ��ֿ�ѹ�� UCLѹ��
		TYPE_BZIP2	           = 0x20000000,	        //�°�, ��ʾ������UCL�㷨ѹ��
		TYPE_FRAGMENT          = 0x30000000,            //�ֿ�ѹ��
		TYPE_FRAGMENTA         = 0x40000000,            //�ֿ�ѹ��
		TYPE_FRAME	           = 0x10000000,			//ʹ���˶���֡ѹ��,���ļ�Ϊspr����ʱ�ſ����õ�
		TYPE_METHOD_FILTER     = 0xf0000000,	        //�°���ѹ�����
		TYPE_FILTER            = 0xf0000000,			//�°������˱��
		TYPE_FILTER_SIZE       = 0x07ffffff,            //�°���ѹ�����С���˱��
		TYPE_UCL_OLD	       = 0x01000000,			//�ϰ�,��ʾ���ļ������Ǿ����ֿ�ѹ�� UCLѹ��
		TYPE_BZIP2_OLD	       = 0x02000000,	        //�ϰ�,��ʾ������UCL�㷨ѹ��
		TYPE_METHOD_FILTER_OLD = 0x0f000000,            //�ϰ�, ѹ�����
		TYPE_FILTER_OLD        = 0xff000000,			//�ϰ�, ���˱��
		TYPE_FRAGMENT_OLD      = 0x03000000,            //�ϰ�,�ֿ�ѹ�� ��
		TYPE_FRAGMENTA_OLD     = 0x04000000,            //�ϰ�,�ֿ�ѹ��
		TYPE_FILTER_SIZE_OLD   = 0x00ffffff,            //�ϰ�,ѹ�����С���˱��
		XPACK_COMPRESS_SIZE_BIT	= 27,
};

//--------------------------------------------------
//ʹ��XPackFile�����ļ����з��ʲ������õ��ĸ����ṹ
//--------------------------------------------------
struct	XPackElemFileRef
{
	unsigned int	uId;			//PAK�ļ�id
	int				nPackIndex;		//PAK������
	int				nElemIndex;		//���ڵ����ļ�����
	int				nCacheIndex;	//��������
	int				nOffset;		//���ļ��Ĳ������ƶ�
	int				nSize;			//���ļ��Ĵ�С
};



/*typedef struct tagRECT
{
    int    left;
    int    top;
    int    right;
    int    bottom;
}RECT,*PRECT,*NPRECT,*LPRECT;
*/

/*void g_MessageBox(char* lpMsg, ...)
{
#ifndef __linux
	char szMsg[256]={0};
	va_list va;
	va_start(va, lpMsg);
	vsprintf(szMsg, lpMsg, va);
	va_end(va);
//	g_DebugLog(szMsg);
	//	MessageBox(g_GetMainHWnd(), szMsg, 0, MB_OK);
	MessageBox(NULL, (LPCTSTR)(szMsg), 0, MB_OK);
#endif

}*/

class KstrFile
{
private:

public:
	KstrFile();
	~KstrFile();

	char *_strupr(char *s)
	{

	};

	char *strlwr(char *s)
	{
	 char *str;
	 str = s;
	 while(*str != '\0')
	 {
	  if(*str >= 'A' && *str <= 'Z') {
	     *str += 'a'-'A';
	 }
	 str++;
	 }
	   return s;
	 }
};
//---------------------------------------------------------------------------
#endif


