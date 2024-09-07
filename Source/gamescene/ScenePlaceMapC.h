// ***************************************************************************************
// ������ͼ��С��ͼ
// Copyright : Kingsoft 2003
// Author    : wooy(wu yue)
// CreateTime: 2003-7-8
// ---------------------------------------------------------------------------------------
// ����Ч�����أ������ϰ�С��ͼ��ʾ�ķ�ΧҪ�����ڳ�����ͼ���صķ�Χ֮�С�����ģ����ص�ͼ�ķ�Χ��
// 7*7=49�������ϰ�С��ͼ�������ʾ��Χ��ӦΪ6*6=36�����򡣵�С��ͼ����С����Ϊ16��֮һ�����
// �£����Ӧ��С��ͼ��ʾͼ�εĴ�С���޶��ڲ�����192*192���ء�
// ***************************************************************************************
#pragma once
//#include	"KLittleMap.h"
#include "cocos2d.h"
using namespace ax;
using namespace std;
#define	MAX_NUM_CHARACTER	40 //40

class KScenePlaceMapC
{
public:
	//��������ʱ��һЩ����
	enum SCENE_PLACE_MAP_WORK_PARAM_PUB
	{
        MAP_SCALE_H = 16,				//��ͼ�ں����ϵı���
		MAP_SCALE_V = 32,				//��ͼ�������ϵı���
	};

public:
	KScenePlaceMapC();
	~KScenePlaceMapC();

	void	Terminate();	//���������ܡ��ͷŶ����ȫ�������붯̬�������Դ��

	//��ȡ����
	void Load(KIniFile* pSetting,int nSubWoleID,const char* pszScenePlaceRootPath );//const char* pszScenePlaceRootPath
	//���
	void Free();

	//���ó�����ͼ�Ľ���
	void SetFocusPosition(int nX, int nY, bool bChangedRegion);
	//���ó�����ͼ�İ�����Ԫ��
	void SetShowElemsFlag(unsigned int uShowElemsFlag);

	//����С��ͼ�Ĵ�С����λ�����ص㣩
	void SetSize(int cx, int cy);
	//����
	void PaintMap(int nx, int ny);


	//KLittleMap m_LittleMap;

	//�����ӵ�
	//////////////////////////////////////////////////////////////////////////
	/*void PaintSymbol(int nX);
	void LoadSymbol(int nSubWorldID);
	void PaintFindPos(int nX, int nY, int nMpsX, int nMpsY);
	void DirectFindPos(int nX, int nY, int bSync, int bPaintLine);
	int OnDirectMap(int nX, int nY);
	void DoDirectMap(int nX, int nY);
	void AutoRunTo(int nX, int nY);	*/
	//////////////////////////////////////////////////////////////////////////


	//��ȡ��ͼ���ǵķ�Χ,����ֵ��ʾ�Ƿ��е�ͼ
	int	 GetMapRect(RECT* pRect);
	void SetGreenLine(int nX, int nY, int bSearch);
	//void DrawGreenLine(int nX, int nY, int nX2, int nY2,const char* pString,int nLineNum=2); // ������
	void DelGreenLine();//ȡ��������
	void GetMapSizePos(KSceneMapSizePos* pInfo);
    void PaintPointYellow(int nX, int nY, int nNpcX, int nNpcY,char szName[64],int nType=0);
	void PaintSymbol(int nX ,int nPosX,int nPosY,char *nContent,int Kind);

	void MapGoTo();
    void PaintPointQiZi(int nX, int nY, int nNpcX, int nNpcY,char szName[64]);
	//-----------------
	//INT	*GetBarrierBuf(int nX, int nY);	// ���ĳ��region���ϰ����ݵĴ洢��ַ

	void	SetHaveLoad(int nX, int nY);	// �趨������ĳ��region���ϰ�����

	void	Draw(int nX, int nY);


	//INT ** GetbtBarrier(return **m_ppbtBarrier;);

    void	GetMapBarrierBuf(INT **lpbtObstacle);


private:
	bool	Initialize();	//��ʼ��
	void	FillCellsPicInfo();
	void	PaintMapPic(int nX, int nY);
	void	PaintCharacters(int nX, int nY);
	void	CalcPicLayout();
	void    SetClientSpr(char *nSprPath,int nxLeft,int nyTop,int nzPos,int i);
	unsigned int GetColor(const char* pString); //���ַ�����ɫת����16����
    ax::Texture2D* createTextureFromRawData(const unsigned char* sBuf, ssize_t dataSize);
private:

	//��������ʱ��һЩ����(���ڲ�ʹ��)
	enum SCENE_PLACE_MAP_WORK_PARAM
	{
		MAP_CELL_MAP_WIDTH = 128,		//��ͼ���ڵ�ͼ�к���Ŀ�ȣ���λ�����ص㣩
		MAP_CELL_MAP_HEIGHT = 128,		//��ͼ���ڵ�ͼ������Ŀ�ȣ���λ�����ص㣩
		MAP_CELL_SCENE_WIDTH = 2048,	//��ͼ����ʵ�ʳ����к���Ŀ�ȣ���λ�����ص㣩= MAP_SCALE_H * MAP_CELL_MAP_WIDTH
		MAP_CELL_SCENE_HEIGHT = 4096,	//��ͼ����ʵ�ʳ���������Ŀ�ȣ���λ�����ص㣩= MAP_SCALE_V * MAP_CELL_MAP_HEIGHT
		MAP_CELL_NUM_REGION_H = 4,		//ÿ����ͼ���ں�������ĳ����������Ŀ
		MAP_CELL_NUM_REGION_V = 4,		//ÿ����ͼ������������ĳ����������Ŀ

 		MAP_CELL_MAX_RANGE = 5,			//ģ����ͬʱ���Ƶĵ�ͼ��ĺ�/��������ȣ���λ���飩
		MAP_CELL_CENTRE_INDEX = 2,		//�������ڵĵ�ͼ��ĺ����������

//		MAP_CELL_FOCUS_INDEX_MIN = 2,	//�������ڵĵ�ͼ��ĺ���������������Сֵ
//		MAP_CELL_FOCUS_INDEX_MAX = 2,	//�������ڵĵ�ͼ��ĺ����������������ֵ

		MAP_MAX_SUPPORT_WIDTH  = (MAP_CELL_MAX_RANGE - 1) * MAP_CELL_MAP_WIDTH,		//С��ͼ�����֧����ʾ����λ�����ص㣩
		MAP_MAX_SUPPORT_HEIGHT = (MAP_CELL_MAX_RANGE - 1) * MAP_CELL_MAP_HEIGHT,	//С��ͼ�����֧����ʾ�ߣ���λ�����ص㣩

		MAP_A_REGION_NUM_MAP_PIXEL_H = 32,	//����һ�������ڵ�ͼ�ϵĺ����ȣ���λ�����ص㣩
		MAP_A_REGION_NUM_MAP_PIXEL_V = 32,	//����һ�������ڵ�ͼ�ϵ������ȣ���λ�����ص㣩
	};

	struct	MAP_CELL
	{
		 char			szImageName[128];
		 unsigned int	uImageId;
		 short			sISPosition;
		 short			sReserved;
	};

private:
	int		m_bHavePicMap;			//�˳����Ƿ�������ͼ��ʽ��С��ͼ
	int     m_bPicMapType;			//�˳����Ƿ�������ͼ��ʽ��С��ͼ
	bool	m_bInited;				//�Ƿ�ִ�й���ʼ��
    int     bFlag;
	int    bSearch;
	int     m_SubWorld;
	RECT    m_MapPos;
	POINT m_GreenLineDes;           //����Ŀ������
	POINT m_GreenLineYsc;           //���ߵ�ǰ����
	int   m_CurPicWidth;            //��ǰjpgͼƬ�Ŀ��
	int   m_CurPicHeight;           //��ǰjpgͼƬ�ĸ߶�
    int   m_DurFrame[10];
	unsigned int        uFlipTime[10];
	char	m_szEntireMapFile[128];	 //��ͼͼ�ļ�����
	//KSGImageContent*	m_pEntireMap;//���������Ե�ͼ

	POINT	m_EntireMapLTPosition;	//������ͼ�����Ͻǵĳ�������


	POINT	m_EndMapRBPosition;	    //���½ǳ�������

	POINT	m_FocusPosition;		//��������
	POINT	m_PicLoadedLTPosition;	//���Ե�ͼ���ز������ϵ�(�����������еĶ�Ӧ)����
	RECT	m_FocusLimit;			//����λ�õ��޶���Χ

	SIZE			m_Size;			//��ͼ����ʾ��С
	unsigned int	m_uMapShowElems;//��ͼ��Ҫ��ʾ������
	//MAP_CELL		m_ElemsList[MAP_CELL_MAX_RANGE][MAP_CELL_MAX_RANGE];

	//--������ʾʱ����Ԥ�ȼ����뱣����--
	RECT		m_MapCoverArea;		    //��ͼ���Ʋ��ָ��ǵķ�Χ���Ե�ͼ���ص�Ϊ��λ�����������0,0��Ӧ��Ϊԭ�㣩
	RECT		m_PaintCell;			//Ҫ���Ƶĵ�ͼ��������Χ
	SIZE		m_FirstCellSkipWidth;	//���Ƶĺ�/��ͷһ��Ҫ�����Ŀ�/�߶�
	SIZE		m_LastCellSkipHeight;	//���Ƶĺ�/����ĩ�ǿ�Ľ������Ŀ�/�߶�

	unsigned int	m_uSelfColor;			// ������ɫ
	unsigned int	m_uTeammateColor;		// ������ɫ
	unsigned int	m_uPlayerColor;			// ���������ɫ
	unsigned int	m_uFightNpcColor;		// ս��npc��ɫ
	unsigned int	m_uNormalNpcColor;		// ��ͨnpc��ɫ

	//------------------------
	//BYTE		*m_pbyLoadFlag;			    // ĳ��region���ϰ������������
	//INT 		**m_ppbtBarrier;		    // ����region���ϰ�����
	int			m_nMapWidth;			// ��ͼ��(region, m_sMapRect.right - m_sMapRect.left + 1)
	int			m_nMapHeight;			// ��ͼ��(region, m_sMapRect.bottom - m_sMapRect.top + 1)
	int			m_nShowWidth;			// ��ʾ����ٸ�region���ϰ�
	int			m_nShowHeight;			// ��ʾ�߶��ٸ�region���ϰ�

	RECT        m_MapRect;

	int         m_DestRegIdx_X;		    //  Ŀ�����ڵ����� ����
    int         m_DestRegIdx_Y;


	int         m_YscRegIdx_X;		    //  ���ﵱǰ���ڵ����� ����
    int         m_YscRegIdx_Y;

	int         m_YscTempRegIdx_X;		//  ���￪ʼѰ·�����ڵ����� ����
    int         m_YscTempRegIdx_Y;
	int         CreateFolder(char * nPath);
	XPackElemFileRef m_PackRef;
};
