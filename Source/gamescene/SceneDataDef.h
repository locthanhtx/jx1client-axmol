// ***************************************************************************************
// ����ģ���һЩ���ݶ���
// Copyright : Kingsoft 2002
// Author    : wooy(wu yue)
// CreateTime: 2002-11-11
// ---------------------------------------------------------------------------------------
// ***************************************************************************************

#ifndef SCENEDATADEF_H_HEADER_INCLUDED_C225572F
#define SCENEDATADEF_H_HEADER_INCLUDED_C225572F

#define	REGION_COMBIN_FILE_NAME_SERVER "region_s.dat"		//������ͼ�������ݵķ������˰�ϲ��ļ����ļ���
#define	REGION_COMBIN_FILE_NAME_CLIENT "Region_C.dat"		//������ͼ�������ݵĿͻ��˰�ϲ��ļ����ļ���

//������ͼ�����ļ���ÿ��������֯�����ɸ��洢��ͬ���ݵ��ļ�����Щ�ļ���������Ϊ��λ���ֱ�ϲ���һ�������ļ�
//�����ļ��ĸ�ʽ���£�
//1.�ȴ洢һ�����ֽڵ�unsigned int���ݣ���ʾ�˺ϳ��ļ��й������������ļ�
//2.�洢���ļ���С���Լ���ʼ��ʼƫ��λ�õ���Ϣ�ṹ���б����м������ļ����������м���KCombinFileSection
//					�����ļ���ƫ��λ���Ǵ���Ϣ�ṹ���б�֮���λ�ÿ�ʼ����.
struct KCombinFileSection
{
	unsigned int	uOffset;	//��λ���ֽ�
	unsigned int	uLength;	//��λ���ֽ�
};
//3.���ļ��Ĵ洢����ÿ�����ļ���Ӧ��ʲô����SCENE_FILE_INDEX��ö�ٸ������͡�ÿ��ö��ֵ��ʾ���ļ��ںϲ��ļ��еĴ���������
enum SCENE_FILE_INDEX
{
	REGION_OBSTACLE_FILE_INDEX	= 0,//"OBSTACLE.DAT"�ϰ��ļ�
	REGION_TRAP_FILE_INDEX,			//"Trap.dat"
	REGION_NPC_FILE_INDEX,			//"Npc_S.dat" or "Npc_C.dat"
	REGION_OBJ_FILE_INDEX,			//"Obj_S.dat" or "Obj_C.dat"
	REGION_GROUND_LAYER_FILE_INDEX,	//"Ground.dat"
	REGION_BUILDIN_OBJ_FILE_INDEX,	//"BuildinObj.Dat"

	REGION_ELEM_FILE_COUNT
};
//4.�洢���ļ������ݡ�


#define REGION_GROUND_LAYER_FILE	"Ground.dat"			//�ر�����Ϣ�����ƴ洢�ļ�
#define	REGION_BUILDIN_OBJ_FILE		"BuildinObj.Dat"		//������ͼ�ڽ����󼯺���Ϣ�ļ�
#define	REGION_OBSTACLE_FILE		"OBSTACLE.DAT"			//�ϰ��ļ�
#define REGION_NPC_FILE_SERVER		"Npc_S.dat"
#define REGION_NPC_FILE_CLIENT		"Npc_C.dat"
#define REGION_OBJ_FILE_SERVER		"Obj_S.dat"
#define REGION_OBJ_FILE_CLIENT		"Obj_C.dat"
#define REGION_TRAP_FILE			"Trap.dat"				//�¼����ļ�

#define	MAX_RESOURCE_FILE_NAME_LEN	128
#define NOT_ABOVE_HEAD_OBJ			0xFFFF	//���Ǹ߿ն���


enum IPOT_RENDER_LAYER
{
	IPOT_RL_COVER_GROUND	= 0x01,		//ƽ���ڵ����ϣ���ȫ������֮��
	IPOT_RL_OBJECT			= 0x02,		//��ǰ�����
	IPOT_RL_INFRONTOF_ALL	= 0x04,		//��һ�е��ϲ�
	IPOT_RL_LIGHT_PROP		= 0x08,		//�˶�����й�Դ����
};

//## ��ά����
struct KTriDimensionCoord
{
	int x, y, z;
};

//������Ground.Dat�ر�����Ϣ�����ƴ洢�ļ�������
//������洢�������������ݣ�
//1.�ļ�ͷ�ṹ
struct KGroundFileHead
{
	unsigned int uNumGrunode;	//�ر���ͼ�ε���Ŀ
	unsigned int uNumObject;	//�������������·��֮��Ķ������Ŀ
	unsigned int uObjectDataOffset;
};
//2.�洢KGroundFileHead::nNumGrunode����KSPRCrunode�ṹ�����ĵر���ͼ��
struct KSPRCrunode//## �ر���ͼ����Ϣ��
{
	struct KSPRCrunodeParam
	{
		unsigned short h, v;		//��ͼ�����ڵģ������ڣ��ر�������
		unsigned short nFrame;		//����ͼ�εĵڼ�֡
		unsigned short nFileNameLen;//ͼ���ļ������ȣ���������������ȡֵ���ɴ���MAX_RESOURCE_FILE_NAME_LEN
	}	Param;

	//## ͼ���ļ���
	char szImgName[1];	//�˽ṹΪ�䳤��ʵ�ʳ���Ϊ�˽ṹ�е�nFileNameLen��ֵ
};
//3.�洢KGroundFileHead::nNumObject����KSPRCoverGroundObj�ṹ�����Ľ����������
//Ҫ���ջ��Ƶ��Ⱥ�˳�����к�
#pragma pack(push, 2)
struct KSPRCoverGroundObj//## �����������������·��֮��Ķ���ṹ��
{
	//## ����λ���ڳ�����ͼ�����ꡣ��λ�����������꣩
	int nPositionX;			//��������
	int nPositionY;			//��������

	char szImage[MAX_RESOURCE_FILE_NAME_LEN];	//���õ�ͼ�ε��ļ�����

	unsigned short	nWidth;	//ͼ�κ��
	unsigned short	nHeight;//ͼ���ݿ�

	//## ����ͼ�εĵڼ�֡
	unsigned short nFrame;

	//������Ϊ��ֵ0
	union
	{
		unsigned char	bReserved;
		unsigned char	bRelateRegion;
	};

	//## ���ƴ�������
	unsigned char	bOrder;

	//## ���ƴ��򣨴Σ�
	short	nLayer;

};
#pragma pack(pop)
//������Ground.Dat�ر�����Ϣ�����ƴ洢�ļ�--�洢���ݽ���---������


//������BuildinObj.Dat�����ڽ���������ƴ洢�ļ�������
//������洢�������������ݣ�
//1.�ļ�ͷ�ṹ
struct KBuildinObjFileHead
{
	//## �������ڽ�������ܵ���Ŀ
	unsigned int nNumBios;		//nNumBios = nNumBiosTree + nNumBiosLine + nNumBiosPoint + nNumBiosAbove
	//## ������ʽ������ڽ��������Ŀ
	unsigned short nNumBiosTree;
	//## �Եױ߷�ʽ������ڽ��������Ŀ
	unsigned short nNumBiosLine;
	//## �Ե㷽ʽ������ڽ��������Ŀ
	unsigned short nNumBiosPoint;
	//## �߿��ڽ��������Ŀ
	unsigned short nNumBiosAbove;

	//## �߿ն������Ļ�ͼ�����š�
	unsigned short nMaxAboveHeadObjOrder;
	unsigned short nNumBulidinLight;
};
//2.KBuildinObjFileHead::nNumBios����KBuildinObj�ṹ�����ĳ����ڽ�����
//�洢��˳��Ϊ:�洢�Ե㷽ʽ�����ȫ������;
//�ٴ洢�Եױ߷�ʽ�����ȫ������;
//�ٴ洢��������ʽ�����ȫ������;
//�ٴ洢ȫ���ĸ߿ն���
struct KBuildinObj//## ����������ͼ�ϵ��ڽ�����Ľṹ
{//���½ṹ,���ܵ���λ��
	//## ͼ�ε�����
	unsigned int Props;//��ֵΪö��SPBIO_PROPSȡֵ�����

	//====ͼ�����====
	//�ĸ�ͼ�ε㰴��ʱ�뷽�����
	KTriDimensionCoord ImgPos1;	//ͼ�ε�һ�����Ӧ�����
	KTriDimensionCoord ImgPos2;
	KTriDimensionCoord ImgPos3;
	KTriDimensionCoord ImgPos4; //ͼ�ε��ĸ����Ӧ�����

	short	nImgWidth;		//ͼ����ͼ�Ŀ�(��λ�����ص�)
	short	nImgHeight;		//ͼ����ͼ�ĸ�(��λ�����ص�)

	char  szImage[MAX_RESOURCE_FILE_NAME_LEN];	//ͼ����Դ���ļ���
	unsigned int   uFlipTime;	//��ǰ֡�л�������ʱ��
    unsigned short nFrame;		//���Ƶ�ͼ��֡����
	unsigned short nImgNumFrames;//�˶������ͼ��֡����Ŀ
	unsigned short nAniSpeed;	//�����Ĳ����ٶȣ�ֵ���ʾΪ��֡ͼ

	//====�߿ն���Ļ�ͼ˳��====
	unsigned short nOrder;
	//====��Ƕ���λ��====
	KTriDimensionCoord oPos1;
	KTriDimensionCoord oPos2;

	float fAngleXY;
	float fNodicalY;
};
//������BuildinObj.Dat�����ڽ���������ƴ洢�ļ�--�洢���ݽ���---������


//## spbio��������Զ���
enum SPBIO_PROPS_LIST
{
	//##Documentation
	//## ƽ���ڵر��������ܹ��з֡�
	SPBIO_P_PLANETYPE_H = 0x00,
	//##Documentation
	//## ƽ���ڵر��������ܹ��з֡�
	SPBIO_P_PLANETYPE_H_D = 0x01,
	//##Documentation
	//## ��ֱ�ڵر��������ܹ��з֡�
	SPBIO_P_PLANETYPE_V = 0x02,
	//##Documentation
	//## ��ֱ�ڵر��������ܹ��з֡�
	SPBIO_P_PLANETYPE_V_D = 0x03,
	//##Documentation
	//## ͼ�����Ͷ���ֵmask
	SPBIO_P_PLANETYPE_MASK = 0x03,
	//##Documentation
	//## ���ܹ�
	SPBIO_P_LIT_NONE = 0x00,
	//##Documentation
	//## �����з��ܹ�
	SPBIO_P_LIT_DIV = 0x04,
	//##Documentation
	//## ���������ܹ�
	SPBIO_P_LIT_CENTRE = 0x08,
	//##Documentation
	//## ƽ���ܹ�
	SPBIO_P_LIT_PARALLEL = 0x0C,
	//##Documentation
	//## �����зַ�ʽ�����mask
	SPBIO_P_LIT_MASK = 0x0C,
	//##Documentation
	//## ������
	SPBIO_P_BLOCK_LIGHT_NONE = 0x00,
	//##Documentation
	//## ���յױߵ���
	SPBIO_P_BLOCK_LIGHT_BOTTOM = 0x10,
	//##Documentation
	//## ����һ��Բ����
	SPBIO_P_BLOCK_LIGHT_CIRCLE = 0x20,
	//##Documentation
	//## ���ⷽʽ����ֵmask
	SPBIO_P_BLOCK_LIGHT_MASK = 0x30,
	//##Documentation
	//## ����뾶Ϊ0.2
	SPBIO_P_BLOCK_L_RADIUS_2 = 0x00,
	//##Documentation
	//## ����뾶Ϊ0.3
	SPBIO_P_BLOCK_L_RADIUS_3 = 0x40,
	//##Documentation
	//## ����뾶Ϊ0.4
	SPBIO_P_BLOCK_L_RADIUS_4 = 0x80,
	//##Documentation
	//## ����뾶Ϊ0.5
	SPBIO_P_BLOCK_L_RADIUS_5 = 0xC0,
	//##Documentation
	//## ����뾶Ϊ��ֵmask
	SPBIO_P_BLOCK_L_RADIUS_MASK = 0xC0,
	//##Documentation
	//## ���յ�����
	SPBIO_P_SORTMANNER_POINT = 0x0000,
	//##Documentation
	//## ���ձ�����
	SPBIO_P_SORTMANNER_LINE = 0x0100,
	//##Documentation
	//## ����������
	SPBIO_P_SORTMANNER_TREE = 0x0200,
	//##Documentation
	//## ����ʽ��mask
	SPBIO_P_SORTMANNER_MASK = 0x0300,

	//##Documentation
	//## ��ҪҪ������ʾ
	SPBIO_F_HIGHT_LIGHT = 0x0400,
};


//������Trap.dat�����¼�����Ϣ�����ƴ洢�ļ�������
//������洢�������������ݣ�
//1.�ļ�ͷ�ṹ
struct KTrapFileHead
{
	unsigned int uNumTrap;		//�¼������Ŀ
	unsigned int uReserved[2];	//�̶�ֵΪ0
};
//2.�洢KTrapFileHead::nNumTrap����KSPTrap�ṹ�����ĵر��¼���
struct KSPTrap
{
	unsigned char	cX;			//�����Xλ�ã���Χ0-16��
	unsigned char	cY;			//�����Yλ�� ��Χ0-32��
	unsigned char	cNumCell;	//���������˼���С����
	unsigned char	cReserved;	//�������̶�ֵΪ0
	unsigned int	uTrapId;	//�����¼������ļ���ת�����õ����¼�id��ʶ(�ű�ID)
};
//������Trap.Dat�����¼�����Ϣ�����ƴ洢�ļ�--�洢���ݽ���---������

//���ڴ����ͼ�����ļ�����
struct KMapList
{
	char				nMapName[64];
	char				nMapType[32];
	int                 nSubWorldID;
	int					nNpcSeriesAuto;
	int         	    nNpcSeriesMetal;
	int 	            nNpcSeriesWood;
	int	                nNpcSeriesWater;
	int	                nNpcSeriesFire;
	int	                nNpcSeriesEarth;
	int	                nAutoGoldenNpc;
	int	                nGoldenType;
	int                 nNpcNum;
	char	            nGoldenDropRate[MAX_RESOURCE_FILE_NAME_LEN];
	char 	            nNormalDropRate[MAX_RESOURCE_FILE_NAME_LEN];
	char	            nNewWorldScript[MAX_RESOURCE_FILE_NAME_LEN];
	char                nNewWorldParam[MAX_RESOURCE_FILE_NAME_LEN];
	unsigned char		nCSeries;		// npc ����
	int                 nMinLevel;
	int                 nMaxLevel;
	unsigned short		nScriptNameLen;	// Npc�ű��ļ������ȣ���������������ȡֵ���ɴ���MAX_RESOURCE_FILE_NAME_LEN
	char			    sZScript[MAX_RESOURCE_FILE_NAME_LEN];

};
//������Npc_S.dat, Npc_C.dat��ͼ�ϵ�npc�洢�ļ�������
//������洢�������������ݣ�
//1.�ļ�ͷ�ṹ
struct KNpcFileHead
{
	unsigned int uNumNpc;		//�¼������Ŀ
	unsigned int uReserved[2];	//�̶�ֵΪ0
};
//2.�洢KNpcFileHead::nNumNpc����KSPNpc�ṹ������npc
struct KSPNpc
{
	int					nTemplateID;	// ģ����(��Ӧģ���ļ�"\Settings\NpcS.txt"����Ŵ� 0 ��ʼ)
	int					nPositionX;
	int					nPositionY;
	bool				bSpecialNpc;	// ����npc  ���֣�
	char				cReserved[3];	// ����
	char				szName[32];		// npc ����
	short				nLevel;			// �ȼ�
	short				nCurFrame;		// ����ʱͼ���ǵڼ�֡
	short				nHeadImageNo;	// ͷ��ͼ�α��	 NpcSettings
	short				shKind;			// npc����
	unsigned char		cCamp;			// npc ��Ӫ
	unsigned char		cSeries;		// npc ����
	unsigned short		nScriptNameLen;	// Npc�ű��ļ������ȣ���������������ȡֵ���ɴ���MAX_RESOURCE_FILE_NAME_LEN
	char				szScript[MAX_RESOURCE_FILE_NAME_LEN];	//Npc�ű��ļ���//�˽ṹʵ�ʴ洢ʱΪ�䳤��ʵ�ʳ���Ϊ�˽ṹ�е�nScriptNameLen��ֵ
    char                nNDropFile[MAX_RESOURCE_FILE_NAME_LEN];
    char                nGDropFile[MAX_RESOURCE_FILE_NAME_LEN];
	//KTriDimensionCoord	Pos;
};
//������Npc_S.dat, Npc_C.dat��ͼ�ϵ�npc�洢�ļ�--�洢���ݽ���---������

//������Obj_S.dat, Obj_C.dat��ͼ�ϵ�Obj�洢�ļ�������
//������洢�������������ݣ�
//1.�ļ�ͷ�ṹ
struct KObjFileHead
{
	unsigned int uNumObj;		//�¼������Ŀ
	unsigned int uReserved[2];	//�̶�ֵΪ0
};
//2.�洢KObjFileHead::nNumObj����KSPObj�ṹ������Obj
struct KSPObj
{
	int					nTemplateID;	//ģ����(��Ӧģ���ļ�"\Settings\ObjData.txt"����Ŵ�1��ʼ)
	short				nState;
	unsigned short		nBioIndex;		//��bSkipPaintΪ��ֵʱ����������Ч������Ϊ��ֵ0
	KTriDimensionCoord	Pos;
	char				nDir;
	bool				bSkipPaint;		//�Ƿ���Դ�obj�Ļ���
	unsigned short		nScriptNameLen;	//Obj�ű��ļ������ȣ���������������ȡֵ���ɴ���MAX_RESOURCE_FILE_NAME_LEN
	char				szScript[MAX_RESOURCE_FILE_NAME_LEN];	//Obj�ű��ļ���//ʵ�ʴ洢ʱΪ�䳤��ʵ�ʳ���Ϊ�˽ṹ�е�nScriptNameLen��ֵ
};
//������Obj_S.dat, Obj_C.dat��ͼ�ϵ�npc�洢�ļ�--�洢���ݽ���---������

// �����ڽ���Դ���Խṹ
struct KBuildInLightInfo
{
	KTriDimensionCoord	oPos;			// ��Դ����
	unsigned long				dwColor;		// ��Դ��ɫ������
	int					nMinRange;		// ��С�뾶
	int					nMaxRange;		// ���뾶
	int					nCycle;			// ѭ������,����
};

#pragma pack(1)
// ���ڹ��ռ������ɫ�ṹ
struct KLColor
{
	//unsigned int r;
	//unsigned int g;
	//unsigned int b;
    //unsigned int ReserveForAlign;
	unsigned short r;
	unsigned short g;
	unsigned short b;
    unsigned short ReserveForAlign;

    // ����Ĵ��뵼�ºܶ��������ʧ
//	KLColor()
//	{
//        r = 0, g = 0, b = 0;
//	}
//
//	KLColor(int rr, int gg, int bb)
//	{
//		r = rr, g = gg, b = bb;
//	}

	void SetColor(int rr, int gg, int bb)
	{
		r = rr, g = gg, b = bb;
	}

	void SetColor(unsigned long dwColor)
	{
		r = (unsigned short)((dwColor>>16) & 0xff);
		g = (unsigned short)((dwColor>>8) & 0xff);
		b = (unsigned short)(dwColor & 0xff);
	}
	void Scale(float f)
	{
		r = (int)(r * f);
		g = (int)(g * f);
		b = (int)(b * f);
	}
	unsigned long GetColor()
	{
		return 0xff000000 | (r<<16) | (g<<8) | b;
	}

	const KLColor& operator+=(KLColor& color)
	{
		r += color.r, g += color.g, b += color.b;
		return *this;
	}

	const KLColor operator+(KLColor& color)
	{
		KLColor c;
		c.r += r + color.r, c.g += g + color.g, c.b += b + color.b;
		return c;
	}
};

#pragma pack()


#endif /* SCENEDATADEF_H_HEADER_INCLUDED_C225572F */
