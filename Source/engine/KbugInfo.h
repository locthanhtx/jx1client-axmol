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
	uint32_t nLength;
	void * lpSecurityDescriptor;
	bool bInheritHandle;
} SECURITY_ATTRIBUTES_S, *PSECURITY_ATTRIBUTES_S, *LPSECURITY_ATTRIBUTES_S;

typedef unsigned short *PWORD;
//#define PDWORD * DWORD
typedef uint32_t  *PDWORD;
typedef uint32_t  *LPDWORD;

#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2
#define IN
#define OUT
#define BYTE unsigned char
#define MAX_PATH 1200
#define UINT uint32_t
#define INT  int32_t
#define FALSE false
#define TRUE  true
#define DWORD uint32_t
#define WORD  unsigned short

#define BOOL int32_t
#define LPCSTR const char *

#define PVOID void *
#define LPVOID PVOID
#define LPSTR char *
#define LONG  int32_t
#define PBYTE unsigned char *
#define LPBYTE PBYTE
#define LPTSTR LPSTR
//typedef unsigned char BYTE;
typedef int32_t   LONG_PTR;
typedef uint32_t  DWORD_PTR;
typedef uint32_t  DWORD_PTRD;
typedef uint32_t   UINT_PTR;
typedef uint32_t  ULONG_PTR;
//UINT_PTR
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)

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
typedef int32_t* LPINT;
#define FLOAT float
#define LPCTSTR const char *
#define CONST const
#define SOCKET_ERROR -1
#define  VOID void
#define CALLBACK __stdcall
#define  CHAR char
#define  ULONG uint32_t
typedef int32_t SOCKET;
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

//typedef void (CALLBACK *CALLBACK_CLIENT_EVENT )(LPVOID lpParam,const uint32_t &ulnEventType);
typedef struct
{
	ULONG_PTR Internal;
	ULONG_PTR InternalHigh;
	union
	{
		struct
		{
			uint32_t Offset;
			uint32_t OffsetHigh;
		};
		void * Pointer;
	};
	 void * hEvent;
}OVERLAPPED;

typedef struct tagSIZE
{
    int32_t        cx;
    int32_t        cy;
} SIZE, *PSIZE, *LPSIZE;

typedef struct
{
	int32_t  x;
	int32_t  y;
}POINT;

typedef struct tagRECT
{
	int32_t    left;
	int32_t    top;
	int32_t    right;
	int32_t    bottom;
}RECT;
#define MAKELONG(a, b) ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)      ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)      ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)      ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)      ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;

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
typedef WORD	KPAL16;		// 16λɫ��

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
	uint32_t			nDirections;
	uint32_t			nFrames;
	int32_t				bytes_per_pixel;
	LONG			size;
	BYTE*			buffer;
	LONG			width,height;
	int32_t				x,y;
};
typedef struct
{
	WORD bfType; // λͼ�ļ������ͣ�����ΪBM(1-2�ֽڣ�
	DWORD bfSize; // λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ��3-6�ֽڣ�
	WORD bfReserved1; // λͼ�ļ������֣�����Ϊ0(7-8�ֽڣ�
	WORD bfReserved2; // λͼ�ļ������֣�����Ϊ0(9-10�ֽڣ�
	DWORD bfOffBits; // λͼ���ݵ���ʼλ�ã��������λͼ��11-14�ֽڣ�
	// �ļ�ͷ��ƫ������ʾ�����ֽ�Ϊ��λ
} BITMAPFILEHEADERS;

typedef struct
{
	DWORD biSize; // ���ṹ��ռ���ֽ�����15-18�ֽڣ�
	LONG biWidth; // λͼ�Ŀ�ȣ�������Ϊ��λ��19-22�ֽڣ�
	LONG biHeight; // λͼ�ĸ߶ȣ�������Ϊ��λ��23-26�ֽڣ�
	WORD biPlanes; // Ŀ���豸�ļ��𣬱���Ϊ1(27-28�ֽڣ�
	WORD biBitCount;// ÿ�����������λ����������1��˫ɫ������29-30�ֽڣ�
	// 4(16ɫ����8(256ɫ����24�����ɫ��֮һ
	DWORD biCompression;  // λͼѹ�����ͣ������� 0����ѹ��������31-34�ֽڣ�
	// 1(BI_RLE8ѹ�����ͣ���2(BI_RLE4ѹ�����ͣ�֮һ
	DWORD biSizeImage;    // λͼ�Ĵ�С�����ֽ�Ϊ��λ��35-38�ֽڣ�
	LONG biXPelsPerMeter; // λͼˮƽ�ֱ��ʣ�ÿ����������39-42�ֽڣ�
	LONG biYPelsPerMeter; // λͼ��ֱ�ֱ��ʣ�ÿ����������43-46�ֽ�)
	DWORD biClrUsed;      // λͼʵ��ʹ�õ���ɫ���е���ɫ����47-50�ֽڣ�
	DWORD biClrImportant; // λͼ��ʾ��������Ҫ����ɫ����51-54�ֽڣ�
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
	WORD	Width;		// ͼƬ���
	WORD	Height;		// ͼƬ�߶�
	int32_t 	CenterX;	// ���ĵ�ˮƽλ��
	int32_t 	CenterY;	// ���ĵĴ�ֱλ��
	WORD	Frames;		// ��֡��
	WORD	Colors;		// ��ɫ��
	WORD	Directions;	// ������
	WORD	nInterval;	// ÿ֡���������Ϸ֡Ϊ��λ��
	WORD	Reserved[6];// �����ֶΣ����Ժ�ʹ�ã�*/
	BYTE	Comment[4];	// ע������(SPR\0)
	WORD	Width;		// ͼƬ���
	WORD	Height;		// ͼƬ�߶�
	WORD	CenterX;	// ���ĵ�ˮƽλ��
	WORD	CenterY;	// ���ĵĴ�ֱλ��
	WORD	Frames;		// ��֡��
	WORD	Colors;		// ��ɫ��
	WORD	Directions;	// ������
	WORD	nInterval;	// ÿ֡���������Ϸ֡Ϊ��λ��
	WORD	Reserved[6];// �����ֶΣ����Ժ�ʹ�ã�
} SPRHEAD;

#define	SPR_COMMENT_FLAG				0x525053	//'SPR'

//---------------------------------------------------------------------------
typedef struct
{
	DWORD	Offset;		// ÿһ֡��ƫ��
	DWORD	Length;		// ÿһ֡�ĳ���
} SPROFFS;

//---------------------------------------------------------------------------
typedef struct
{
	WORD	Width;		// ֡��С���
	WORD	Height;		// ֡��С�߶�
	WORD	OffsetX;	// ˮƽλ�ƣ������ԭͼ���Ͻǣ�
	WORD	OffsetY;	// ��ֱλ�ƣ������ԭͼ���Ͻǣ�
	BYTE	Sprite[1];	// RLEѹ��ͼ������
} SPRFRAME;

enum enumSubDir{sdFont=0,sdMaps,sdScript,sdResource,sdSetting,sdSound,sdSpr,sdUi,sdLastDir};
//һ��Pack�ļ����е�ͷ�ṹ:��PAKͷ
struct XPackFileHeader
{
	unsigned char cSignature[4];		//�ĸ��ֽڵ��ļ���ͷ��־���̶�Ϊ�ַ���'PACK'
	uint32_t uCount;				//�ļ����ݵ���Ŀ��
	uint32_t uIndexTableOffset;	//������ƫ����
	uint32_t uDataOffset;			//���ݵ�ƫ����
	uint32_t uCrc32;				//У���
	uint32_t  uPakTime;			    //����ļ�����ʱ��ʱ�䣬��Ϊ��λtime()
	unsigned char cReserved[8];		    //�������ֽ�
};

#define	XPACKFILE_SIGNATURE_FLAG		0x4b434150	//'PACK'
//Pack�ж�Ӧÿ�����ļ���������Ϣ��
struct XPackIndexInfo
{
	uint32_t	uId;				//���ļ�id
	uint32_t	uOffset;			//���ļ��ڰ��е�ƫ��λ��
	int32_t			lSize;				//���ļ���ԭʼ��С
	int32_t			lCompressSizeFlag;	//���ļ�ѹ����Ĵ�С��ѹ������
//	FILETIME        UpdateTime;
	//����ֽڱ�ʾѹ����������XPACK_METHOD
	//�͵������ֽڱ�ʾ���ļ�ѹ����Ĵ�С
};


//pak���б����spr֡��Ϣ��
struct XPackSprFrameInfo
{
	int32_t lCompressSize;
	int32_t lSize;
} ;

//�ֿ�ѹ�����ļ����ļ����ݴ洢���洢��������˳��һ��Ϊ��
// [XPackFileFragmentElemHeader] + [��һ������] + [�ڶ�������] + .. + [��n������] + [n��XPackFileFragmentInfo������]

//�ֿ�ѹ�����ļ��ڸ��ļ����ݴ洢����ʼλ�õ�ͷ���ݽṹ
struct XPackFileFragmentElemHeader
{
	int32_t				nNumFragment;		//�ֿ����Ŀ
	int32_t				nFragmentInfoOffest;//�ֿ���Ϣ���ƫ��λ��,�����XPackFileFragmentElemHeader��ʼλ�õ�ƫ��
};

//�ֿ�ѹ���ļ��ķֿ���Ϣ
struct XPackFileFragmentInfo
{
	uint32_t	uOffset;			//�˷ֿ����ݵ���ʼ�������ļ��������е�ƫ��λ��,�����XPackFileFragmentElemHeader��ʼλ�õ�ƫ��
	uint32_t	uSize;				//�˷ֿ��ԭʼ��С
	uint32_t	uCompressSizeFlag;	//�˷ֿ�ѹ����Ĵ�С��ѹ��������������XPackIndexInfo�ṹ�е�uCompressSizeFlag
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
	uint32_t	uId;			//PAK�ļ�id
	int32_t				nPackIndex;		//PAK������
	int32_t				nElemIndex;		//���ڵ����ļ�����
	int32_t				nCacheIndex;	//��������
	int32_t				nOffset;		//���ļ��Ĳ������ƶ�
	int32_t				nSize;			//���ļ��Ĵ�С
};



/*typedef struct tagRECT
{
    int32_t    left;
    int32_t    top;
    int32_t    right;
    int32_t    bottom;
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


