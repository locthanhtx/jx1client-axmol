/*****************************************************************************************
//  ����ͼ��ͼ����Դ����
//	Copyright : Kingsoft 2002-2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-11-11
------------------------------------------------------------------------------------------
*****************************************************************************************/
#ifndef KIMAGESTORE2_H_HEADER_INCLUDED_C23109DC
#define KIMAGESTORE2_H_HEADER_INCLUDED_C23109DC

//#include "../iRepresent/KRepresentUnit.h"
#include "gamecore/KRUImage.h" 
#include "engine/KCriticalSection.h"
//#include <ddraw.h>
struct KImageParam;

//## ����ͼ��ͼ����Դ����
//## ������ͬid��ͼ�ζ���ֻ��һ����
class KImageStore2
{
public:
    KImageStore2();
    ~KImageStore2();

//====���ImageStore����Ĳ���====

	//## ��ʼ��iResStore�ӿڶ���
    bool Init();

    //## ����ͷ�ȫ����ͼ�ζ��󣬼Ȱ�����̬���صģ�Ҳ����ͨ������CreateImage���ɵġ�
    void Free();

    //## ����ͼ�ζ�̬����ƽ�������
    void SetBalanceParam(
        //## ����ͼ�ε���Ŀ��ƽ��ֵ��
        int nNumImage, 
        //## ÿ���ٴ�����ͼ�ζ������һ��ƽ���顣
		uint32_t uCheckPoint = 10000);

//====��Ե���ͼ�εĲ���====

	//## ����ڴ�ͼ�ζ���ʧ�ܣ��򷵻�0�����򷵻ظ���ͼ������ת�����ɵ���ֵid��
    /*uint32_t CreateImage(
        //## ͼ�ε����֡�
        const char* pszName, 
        //## ͼ�κ��
        int nWidth, 
        //## ͼ���ݿ�
        int nHeight, 
        //## ͼ������
        int nType);
	*/
    //## ��ȡ�Ѿ����ڵ�ͨ��CreateImage������ͼ��
    /*void* GetExistedCreateBitmap(
		//## ͼ������
        const char* pszImage,
		//## ͼ��id
		uint32_t uImage,
		//## ͼ��ԭ����iImageStore�ڵ�λ�á�
        short& nImagePosition);
	*/
	/*bool CreateBitmapSurface(
		//## ͼ������
        const char* pszImage,
		//## ͼ��id
		uint32_t& uImage,
		//## ͼ��ԭ����iImageStore�ڵ�λ�á�
        short& nImagePosition);
	*/
	//## ָ���ͷ�ĳ��ͼ��Դ��
    void FreeImage(
        //## ͼ���ļ���/ͼ������
        char* pszImage);
	void* GetSprFrameData(char* pszImage, uint32_t& uImage,
		short& nImagePosition, int nFrame,uint32_t& nSingFrameSize);
    //## ��ȡͼ�ε����ݻ�������
    void* GetImage(
        //## ͼ��Դ�ļ�����/ͼ������
         char* pszImage, 
        //## ͼ��id
        uint32_t& uImage, 
        //## ͼ��ԭ����iImageStore�ڵ�λ�á�
        short& nImagePosition, 
        //## Ҫ��ȡͼ�ε�֡��������
        int nFrame,
        //## ͼ��Դ����
        int nType,
		//������Ҫ��֡����buffer
		void*& pFrameData,
		void*& pOffsetTable,
		int& bSingleFrameLoad,uint32_t &nSingFrameSize);

    //## ��ȡĳ��ͼ����Դ�������Ϣ��
    bool GetImageParam(
        //## ָ�򱣴�ͼ����Դ�ļ���/ͼ�����Ļ�����
        char* pszImage,
        //## ͼ��Դ����
		int nType,
		//## ͼ����Ϣ�洢�ṹ��ָ��
        KImageParam* pImageData);
	
	//## ��ȡĳ��ͼ����Դĳ֡����Ϣ��
	bool GetImageFrameParam(
        //## ָ�򱣴�ͼ����Դ�ļ���/ͼ�����Ļ�����
        char* pszImage,
        //## ͼ��Դ����
		int nType,
		//ͼ��֡����
		int		nFrame,
        //## ֡ͼ�����������ͼ�ε�ƫ��
		KRPosition2* pOffset,
		//## ֡ͼ�δ�С
		KRPosition2* pSize);

    //## ��ȡͼ��ָ��֡��ĳ������alphaֵ��
    /*int GetImagePixelAlpha(
        //## ͼ����Դ�ļ���/ͼ����
        const char* pszImage, 
        //## ͼ��Դ����
        int nType,
		//## ͼ�ε�֡������
        int nFrame, 
        //## �����ͼ�к�����
        int nX, 
        //## �����ͼ��������
        int nY);
	*/
    //## ����ͼ�ε��ļ��������ļ�ֻ��ISI_T_BITMAP16��ʽ��ͼ����Ч������ָ����ͼ���Ƿ�ɹ����浽�ļ���
    bool SaveImage(
        //## �����ļ������֡�
        const char* pszFile, 
        //## ͼ����
        const char* pszImage, 
        //## Ŀ��ͼ���ļ�������
        int nFileType);

	/*uint32_t SetAdjustColorList(uint32_t* puColorList, uint32_t uCount);

	char* GetAdjustColorPalette(
		int nISPosition,	//ͼ����iImageStore�ڵ�λ�á�
		unsigned uColor);//ƫɫ��
	*/
private:

	struct _KISImageFrameObj
	{        
		bool			bRef;	//�ڱ��λ�ͼѭ�����Ƿ����õ�
		bool			bReserved;
		unsigned short	sOffTableSize;
		uint32_t 	pFrameSize;
		union
		{//ͬʱֻ��ʹ��һ��
			//ͼ��֡����ƫ�Ʊ������Ӹ�֡����
			void*			pOffsetTable;
			//֡���ݻ�����
			void*			pFrameData;
		};
	};

    //## �洢����ͼ��id�������Լ�����ָ��Ľṹ
    struct _KISImageObj
    {
        //## ͼ�ε�id
        uint32_t uId;
		//## ����Ϊ���ڴ��д�����ͼ�Σ����ܴӴ��̶�̬���أ�����cache����
        bool bNotCacheable;
		//�Ƿ񰴵�֡����
        bool bSingleFrameLoad;
		//�ڱ��λ�ͼѭ�����Ƿ����õ�
		bool bRef;
        //## ͼ�ε�����
        unsigned char bType;
        //## ͼ�����ݶ���ָ�롣
        void* pObject;
		/*union
		{
			//ƫɫɫ��, ISI_T_SPR��ʽ��ͼ��
			char* pcAdjustColorPalettes;
			//DirectDraw Surface, ISI_T_BITMAP16��ʽ��ͼ��
            LPDIRECTDRAWSURFACE pSurface;
		};*/
		//ͼ�ε�֡����
		_KISImageFrameObj* pFrames;
    };

private:

	//## ��չm_pObjectList�Ŀռ䣬ʹ�ô洢����ͼ��Դ���ñ�������
    bool ExpandSpace();

    //## ���ͼ��Դ�洢����ƽ��״����
    void CheckBalance();

    //## ��ø���id�ڼ�¼���е�λ�á�
    //## ����ֵ���壺
    //##  ����ֵ >= 0 --> ������id�ڼ�¼���е�λ�ã���0��ʼ������ֵ��
    //##  ����ֵ < 0  --> ����û�и�����id�������Ҫ�����id�Ļ�������λ��Ӧ��Ϊ(-����ֵ-1)
    int FindImage(
        //##Documentation
        //## ͼ�ε�id
        uint32_t uImage, 
        //##Documentation
        //## ͼ�μ�¼��m_pObjectList�п��ܵ�λ�á�
        int nPossiblePosition);

	//��ȡͼ�ζ����ĳһ֡�����ݻ�����
	void* GetSprFrame(
		 char* pszImageFile,
		_KISImageObj& ImgObject,
		int nFrame,uint32_t& nSingFrameSize);

    //## �ͷ�ָ����ͼ�ζ���
    void FreeImageObject(
        //## ͼ�ζ���ָ�롣
        _KISImageObj& ImgObject,
		//�ͷŴ�ͼ�ε���֡���ݣ��������ֵΪ��������ʾ�ͷ�����ͼ��
		int nFrame = -1);

    //## ����ͼ�ζ�������ָ��ͼ����Դ������ʧ�����
    void* LoadImage(
        //## ͼ���ļ���
        char* pszImageFile, 
        //## ͼ������
        _KISImageObj& ImgObj,
		//����ͼ�ε���һ֡����
		int nFrame,
		//���ڻ�ȡ֡���ݵĻ�����ָ��
		void*& pFrameData,uint32_t& nSingFrameSize);

	//�ļ���ת��Ϊ�ַ���
	uint32_t ImageNameToId(char* pszName);

	//char* CreateAdjustColorPalette(const char* pOrigPalette, int nNumColor);

	//���ȫ����ƫɫɫ��
	//void ClearAllAdjustColorPalette();

private:

    enum IS_BALANCE_PARAM
	{
        //## ��չm_pObjectList��ÿ�β������ӵ�Ԫ�ص���Ŀ
        ISBP_EXPAND_SPACE_STEP = 128,
        //## ÿ���ٴ�����ͼ�ζ������һ��ƽ�����Ĭ��ֵ��
        ISBP_CHECK_POINT_DEF = 256 + 128, 
        //## ͼ����Ŀƽ���Ĭ��ֵ
        ISBP_BALANCE_NUM_DEF = 256 + 128,// + 128 + 512, 
        //## ������Χ���Ȳ��ҵķ�Χ��С��
        ISBP_TRY_RANGE_DEF = 8,
	};

private:

    //## ����ͼ�ζ���ṹ�����Ա���������ͼ�ε�id�����������С�
    _KISImageObj* m_pObjectList;

    //## m_pObjectList�����а���_KISImageObjԪ�ص���Ŀ���ߵ�ǰ��������ɴ洢ͼ����Ϣ����Ŀ��m_pObjectList�ռ��С�ı�ʱͬʱ������ֵ��
    int m_nNumReserved;

    //## ���ص�ͼ����Դ����Ŀ����Щͼ�ε���Ϣ�����Ŵ洢��m_pObjectList��ͷ��ʼ��λ�ã�����ͼ�ε�id�������С�
    int m_nNumImages;

    //## ��ͼ�ζ�������ܴ������ۼӼ�¼ֵ��
    uint32_t m_uImageAccessCounter; 

    //## ����ƽ���ʱ�����á�
    uint32_t m_uCheckPoint;

    //## �������ڴ���ͼ����Ŀ��ƽ��ֵ
	int m_nBalanceNum;

	#define	MAX_ADJUSTABLE_COLOR_NUM	16 //8
	uint32_t	m_uSprAdjustColorList[MAX_ADJUSTABLE_COLOR_NUM];
	uint32_t	m_uNumSprAdustColor;

    KCriticalSection    m_ImageProcessLock;

};


#endif /* KIMAGESTORE2_H_HEADER_INCLUDED_C23109DC */
