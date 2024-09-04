/*****************************************************************************************
//  iRepresentShell���Ƶ�ͼԪ�����������
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-11-11
------------------------------------------------------------------------------------------
*****************************************************************************************/

#ifndef KREPRESENTUNIT_H_HEADER_INCLUDED_C23172D7
#define KREPRESENTUNIT_H_HEADER_INCLUDED_C23172D7
//##ModelId=3DCA20D1035C
//##Documentation
//## ͼԪ�ĸ�������ö�ٶ���
enum REPRESENT_UNIT_TYPE
{
    //##Documentation
    //## ���	KRUPoint
    RU_T_POINT,
    //##Documentation
    //## �߶�	KRULine
    RU_T_LINE,
	//## ���α߿�	KRURect
	RU_T_RECT,
    //##Documentation
    //## ͼ��(����ͼ�Σ�ֻ��������ͼԪ�����)	KRUImage
    RU_T_IMAGE,
    //##Documentation
    //## ͼ�ξֲ�	KRUImagePart
    RU_T_IMAGE_PART,
	//##Documentation	KRUImage4
	//## ͼ��(�ı���ͼ�Σ������ĸ�ͼԪ�����)
	RU_T_IMAGE_4,
	//���ŵػ���ͼ��	KRUImageStretch
	//ֻ�ڵ�ƽ���ϻ��ƣ���ͼ��ΪISI_T_BITMAP16 ��Ч
	RU_T_IMAGE_STRETCH,
    //##Documentation
    //## �����ֵĹ���ͼ�Ρ�
    RU_T_DIVIDED_LIT_IMAGE,
    //##Documentation
    //## ��Ӱ
    RU_T_SHADOW,
	//##Documentation
    //## ���ݻ���������ͼ��
	RU_T_BUFFER,
};

//������ʾ�Ķ�ά������
struct KRPosition2
{
	int nX;
	int nY;
};

//������ʾ����ά������
struct KRPosition3
{
	int nX;
	int nY;
	int nZ;
};

//##ModelId=3DB0C4F20145
//##Documentation
//## ����ͼԪ
struct KRepresentUnit
{
    KRPosition3	oPosition;
};


//##ModelId=3DD2107D01C5
//##Documentation
//## ������ɫ�Ľṹ
union KRColor
{
	struct {unsigned char b, g, r, a; } Color_b;
	unsigned int	Color_dw;
};

//##ModelId=3DB617B20036
//##Documentation
//## ���
struct KRUPoint : public KRepresentUnit
{
    //##ModelId=3DB617D2032B
	//##Documentation
	//## �����ɫ
    KRColor	Color;
};

//##ModelId=3DB3C020008B
//##Documentation
//## �߶�
struct KRULine : public KRepresentUnit
{
	KRPosition3	oEndPos;
    //##ModelId=3DB3C0B80278
	//##Documentation
	//## �ߵ���ɫ
    KRColor	Color;
};

typedef KRULine	KRURect;

//##ModelId=3DB3C020008B
//##Documentation
//## ����ͼ�εĻ�����
struct KRUBuffer : public KRepresentUnit
{
	KRPosition2	BufferEntireSize;	//�������Ĵ�С
	KRPosition2	BufferUpdateSize;	//�������Ĵ�С��������ͼ�εķ�Χ��С
	KRPosition2	oImgLTPos;			//Ŀ��ͼ�ε����Ͻǵ�����(��λ:��ͼ���ص�)
    KRColor		Color;				//��ɫ����
	const unsigned char* pBuffer;	//�������ڴ��ָ��
};

enum RUIMAGE_RENDER_STYLE
{
	IMAGE_RENDER_STYLE_ALPHA = 0,		//��alpha����
	IMAGE_RENDER_STYLE_OPACITY,			//ֻ��ȫͨ͸����ȫ��͸����KRUImage::Alphaֵ�����ԡ�
	IMAGE_RENDER_STYLE_3LEVEL,			//����alpha���ƣ�KRUImage::Alphaֵ�����ԡ�
	IMAGE_RENDER_STYLE_BORDER,          //�滭�߿�
	IMAGE_RENDER_STYLE_ALPHA_NOT_BE_LIT,  //��alpha���Ƶ��ǲ�������
	IMAGE_RENDER_STYLE_ALPHA_COLOR_ADJUST,//��alpha���ƣ��Ҵ�ƫɫ����
	IMAGE_RENDER_STYLE_ALPHA_COLOR_3D,    //��alpha���ƣ�H�滭3D��Ӱ
};

enum RUIMAGE_RENDER_FLAG
{
	RUIMAGE_RENDER_FLAG_REF_SPOT = 1,	//����������Ƿ�Ҫ��ͼ�εĲο���(���ĵ�)�Ǻϡ�
			                            //ʹ�ô˱��ʱ,nEndX,nEndY,nEndZ��ֵ�����ԡ�������Ļƽ�е����ϻ���ͼ�Σ���ʹ��ͼ�εĲο���(���ĵ�)��nX,nY,nZ�����ĵ����Ǻϡ�
	RUIMAGE_RENDER_FLAG_FRAME_DRAW = 2,	//֡���Ʒ�ʽ�������ǻ���֡���������ͼ�ε�λ��ƫ��
};

#define	IMAGE_IS_POSITION_INIT	-1

//##ModelId=3DB60D73035F
//##Documentation
//## ͼ��
struct KRUImage : public KRepresentUnit
{
	KRPosition3	oEndPos;

	//��ɫָ���������Ƿ���������ã�������bRenderStyle��ȡֵ��
	//����alpha���ֵ�ȡֵΪ0-255��256�ס�
	KRColor	Color;

    //##ModelId=3DCF64DA029D
    //##Documentation
    //## ���Ʒ�ʽ ȡֵΪRUIMAGE_RENDER_STYLEö��ֵ֮һ
    unsigned char bRenderStyle;

	//���Ʊ��,ȡֵΪRUIMAGE_RENDER_FLAGö��ֵ����ϡ�
	unsigned char bRenderFlag;

    //##ModelId=3DCF5BB001B7
    //##Documentation
    //## ���Ƶ�ͼ�ε�����
    short nType;

	//##ModelId=3DCBEAD0014F
    //##Documentation
    //## ͼ����Դ���ļ���/����ͼ����
    char szImage[256];

    //##ModelId=3DCBEAF50291
    //##Documentation
    //## ͼ��id
    unsigned int uImage;

    //##ModelId=3DCBEB170189
    //##Documentation
    //## ͼ��ԭ����iImageStore�ڵ�λ�á�
    short nISPosition;

    //##ModelId=3DCF5B9C0196
    //##Documentation
    //## ���Ƶ�ͼ��֡����
    short nFrame;

   //short	bMatchReferenceSpot;

   ///BOOL bFrameDraw;

};

//##ModelId=3DB618EC02CC
//##Documentation
//## ͼ�ξֲ�
struct KRUImagePart : public KRUImage
{
	KRPosition2	oImgLTPos;	//��ͼ���Ͻǵ�����(��λ:��ͼ���ص�)
	KRPosition2	oImgRBPos;	//��ͼ���½ǵ�����(��λ:��ͼ���ص�)
};

struct KRUImage4 : public KRUImagePart
{
	//(�㰴˳ʱ��������)
	//��һ��������Ϊ oPosition
	KRPosition3	oSecondPos;	//ͼԪ�ı��εĵڶ����������
	KRPosition3	oThirdPos;	//ͼԪ�ı��εĵ������������
	//���ĸ�������Ϊ oEndPos
};

typedef KRUImage	KRUImageStretch;


//##ModelId=3DBFF195012B
//##Documentation
//## �����ֵĹ���ͼ�Ρ�
struct KRUDividedLitImage : public KRUImage
{
	KRPosition2 oImgLTPos;	//ͼ�α����÷�Χ�����Ͻǵ������(��λ:��ͼ���ص�)

    //##ModelId=3DBFF2A5013A
    //##Documentation
    //## ���ջ��ּ��㣬���ֵĸ��ӵĺ����λ�����ص㣩
    int nCellWidth;

    //##ModelId=3DBFF2B000C0
    //##Documentation
    //## ���ջ��ּ��㣬���ֵĸ��ӵ��ݿ���λ�����ص㣩
    int nCellHeight;

    //##ModelId=3DBFF2B5032B
    //##Documentation
    //## ˮƽ���򻮷ֵĹ��ռ�����ӵ���Ŀ
    int nNumCellH;

    //##ModelId=3DBFF2D800B1
    //##Documentation
    //## ��ֱ���򻮷ֵĹ��ռ�����ӵ���Ŀ
    int nNumCellV;

    //##ModelId=3DBFF2DD03A9
    //##Documentation
    //## �洢���ռ��㻮�ֵĸ��Ӹ����ڵ�Ĺ���ǿ�ȵ����顣ÿ���ڵ�Ĺ���ǿ�ȱ���Ϊһ��256�׵�����ֵ���洢�ռ�Ϊһ���ֽڡ�
    unsigned char* pLightIntensityData;

};


//##ModelId=3DB61A8C0298
//##Documentation
//## ��Ӱ
struct KRUShadow : public KRepresentUnit
{
    KRPosition3	oEndPos;

    //##ModelId=3DB61ADA0024
	//##Documentation
	//## ��Ӱ����ɫ
    KRColor	Color;

};


//##ModelId=3DC0F495038B
//##Documentation
//## ͼ�����Ͷ���
enum KIS_IMAGE_TYPE
{
    //##Documentation
    //## 16λ��֡λͼ��
    //## ISI_T_BITMAP16�ĸ�ʽΪ D3DFMT_R5G6B5 ���� D3DFMT_X1R5G5B5
    //## ������Ϊ���ָ�ʽ��iImageStore�ڲ�ȷ��������ͨ������iImageStore::IsBitmapFormat565��֪���������ָ�ʽ��
    ISI_T_BITMAP16,
    //##Documentation
    //##  spr��ʽ�Ĵ�alphaѹ��ͼ�Σ�����֡
    ISI_T_SPR,
};


//##ModelId=3DB511F30242
//##Documentation
//## ͼ�ε���Ϣ��
struct KImageParam
{
    //##ModelId=3DB512190144
    //##Documentation
    //## ͼ�ε��ܵ�֡��Ŀ
    short nNumFrames;

    //##ModelId=3DB51226012F
    //##Documentation
    //## ͼ�ε�֡�����
    short nInterval;

    //##ModelId=3DB5123B0158
    //##Documentation
    //## ͼ�κ����λ�����ص㣩��
    short nWidth;

    //##ModelId=3DB5123E036F
    //##Documentation
    //## ͼ���ݿ���λ�����ص㣩
    short nHeight;

    //##ModelId=3DB512900118
    //##Documentation
    //## ͼ�βο��㣨���ģ��ĺ�����ֵ��
    short nReferenceSpotX;

    //##ModelId=3DB512A70306
    //##Documentation
    //## ͼ�βο��㣨���ģ���������ֵ��
    short nReferenceSpotY;

    //##ModelId=3DB512CD0280
    //##Documentation
    //## ͼ�ε�֡������Ŀ��ͨ��Ӧ��Ϊͼ�η�����Ŀ����
    short nNumFramesGroup; //������Ŀ

};

struct SPRFRAMSINFO
{
	unsigned short mFrams;
	unsigned short pHwidth;
	unsigned short pHheight;
	unsigned short pHCenterX;
	unsigned short pHCenterY;
	unsigned short pFwidth;
	unsigned short pFheight;
	unsigned short pFoffsetX;
	unsigned short pFoffsetY;
	unsigned short nInterval;
};
#endif /* KREPRESENTUNIT_H_HEADER_INCLUDED_C23172D7 */
