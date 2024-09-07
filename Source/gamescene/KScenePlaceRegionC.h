// ***************************************************************************************
// ������ͼ����������������
// Copyright : Kingsoft 2002
// Author    : wooy(wu yue)
// CreateTime: 2002-11-11
// ---------------------------------------------------------------------------------------
// ***************************************************************************************

#ifndef KSCENEPLACEREGIONC_H_HEADER_INCLUDED_C22EB91D
#define KSCENEPLACEREGIONC_H_HEADER_INCLUDED_C22EB91D
#include "engine/KbugInfo.h"
#include "SceneDataDef.h"
#include "gamecore/KRegion.h"
#include "KIpotLeaf.h"
#include <map>

struct KIpotRuntimeObj;
struct KIpotBuildinObj;
struct KRUImage;
class KPakFile;

//##ModelId=3DB8F89F02D5
//##Documentation
//## ������ͼ�е�һ�����򡣣��ͻ��˰棩
class KScenePlaceRegionC
{
  public:

	//##ModelId=3DD7E5A80351
	//##Documentation
	//## �����������ʱ��һЩ����(�ⲿ�������)
	enum KREGION_WORK_PARAM_PUB {
		//##Documentation
		//## ��������ĺ�����ȣ���λ����㣩
		RWPP_AREGION_WIDTH = 512,
		//##Documentation
		//## ���������������ȣ���λ����㣩
		RWPP_AREGION_HEIGHT = 1024 };

  public:

	//##ModelId=3DD7EA9200B2
	KScenePlaceRegionC();

	//##ModelId=3DD7EA9200F8
	~KScenePlaceRegionC();

	//##ModelId=3DBDAC140299
	//##Documentation
	//## ָʾ�������Ӧ�ü��ص��������ݵ�������������
	bool ToLoad(
		//##Documentation
		//## ����ĺ�������
		int nIndexX,
		//##Documentation
		//## �������������
		int nIndexY);

	int  getStatus(){return m_Status;};
	//##ModelId=3DB901F101CD
	//##Documentation
	//## ����������Ϣ��
	//## �������̲ο�[Use Case View\����\�ͻ���\��������ļ���]��
	bool LoadRe(
		//##Documentation
		//## ָ��洢������Ϣ��'����Ŀ¼'���ڵĸ�Ŀ¼�����ƵĻ�������
		const char* pszBaseFolderName);


	//bool KScenePlaceRegionC::FindPathInit(const char* pszBaseFolderName,int nRegX,int nRegY);

	//##ModelId=3DD3DD2C0141
	//##Documentation
	//## ��ȡ�������������
	void GetRegionIndex(
		//##Documentation
		//## ��������
		int& nX,
		//##Documentation
		//## ��������
		int& nY) const;

	//##ModelId=3DBF876400B2
	//##Documentation
	//## ��մ˶��������
	void Clear();

    void GetFliePath(char* nName,char * nPath,int nMaxRows=100);

	//##ModelId=3DBF9582039A
	//##Documentation
	//## Ԥ��Ⱦ�ر���ͼ��
	bool PrerenderGround(bool bForce);
	unsigned int GetColor(LPCTSTR pString);


	//##ModelId=3DDBD8C80309
	//##Documentation
	//## ���Ƴ�������ر���
	void PaintGround(int bRegionIndex,int fullorpart=1);
	void PaintBuildinObj(RECT* pRepresentArea,int bRegionIndex,bool isClear);
	void ReomeGroundObj(int bRegionIndex);
	void PaintGroundDirect(int bRegionIndex);
	void PaintGroundDirect_Part(int bRegionIndex);
	void PaintGroundDirect_full(int bRegionIndex);

	//##ModelId=3DE29E9102B0
	//##Documentation
	//## ��ȡ�߿ն������Ļ�ͼ�����š�
	unsigned int GetAboveHeadLayer(
		//## ָ��߿ն�����б�
		KBuildinObj*& pObjsAboveHead);

	//##ModelId=3DE29F360221
	//##Documentation
	//## ���Ƹ߿�����
	static void PaintAboveHeadObj(
		KBuildinObj* pObj,
		RECT* pRepresentArea,int nindex);

	//##ModelId=3DE33AB30318
	//##Documentation
	//## ��ȡ�������ڽ�������б���
	void GetBuildinObjs(
		//##Documentation
		//## ָ�򰴵㷽ʽ���������б�
		KIpotBuildinObj*& pObjsPointList,
		//##Documentation
		//## ���㷽ʽ����������Ŀ
		unsigned int& nNumObjsPoint,
		//##Documentation
		//## ָ���߷�ʽ���������б�
		KIpotBuildinObj*& pObjsLineList,
		//##Documentation
		//## ���߷�ʽ����������Ŀ
		unsigned int& nNumObjsLine,
		//##Documentation
		//## ָ������ʽ���������б�
		KIpotBuildinObj*& pObjsTreeList,
		//##Documentation
		//## ������ʽ����������Ŀ
		unsigned int& nNumObjsTree);

	//## ��ȡ�������ڽ�δ��ʼ���Ķ�����б���
	void GetBIOSBuildinObjs(
		KBuildinObj*& pObjsList,
		unsigned int& nNumObjs
    );

	//����ڽ���Դ����Ϣ
	unsigned int GetBuildinLights(
		KBuildInLightInfo*& pLights);

	void SetNestRegion(KScenePlaceRegionC* pNest);

	void EnterProcessArea(KRUImage* pImage);
	void LeaveProcessArea();
	KRUImage* GetPrerenderGroundImage() { return m_pPrerenderGroundImg; }

	long GetObstacleInfo(int nX, int nY);
	long GetObstacleInfoMin(int nX, int nY, int nOffX, int nOffY);
	void SetHightLightSpecialObject(unsigned int uBioIndex);
	void UnsetHightLightSpecialObject(unsigned int uBioIndex);

//#ifdef SWORDONLINE_SHOW_DBUG_INFO
	void PaintObstacle();
	int GetNextMapPos();

//#endif

  private:

	//##Documentation
	//## �����������ʱ��һЩ����(���ڲ�ʹ��)
	//##ModelId=3DD7E4C101E9
	enum KREGION_WORK_PARAM
	{
		//##Documentation
		//## �ر����򻮷ָ��ӵ���Ŀ
		RWP_NUM_GROUND_CELL_H = 16,
		//##Documentation
		//## �ر����򻮷ָ��ӵ���Ŀ
		RWP_NUM_GROUND_CELL_V = 16,

		RWP_NEST_REGION_0 = 1,
		//##Documentation
		//## Ĭ�ϵ�ͼ��ĺ����
		//## ��ͼ��ָ������ͼΪ���ϰ��ȼ��㻮�ֵ���С���ӵ�Ԫ��
		RWP_OBSTACLE_WIDTH = 32,
		//##Documentation
		//## Ĭ�ϵ�ͼ����ݿ���
		//## ��ͼ��ָ������ͼΪ���ϰ��ȼ��㻮�ֵ���С���ӵ�Ԫ��
		RWP_OBSTACLE_HEIGHT = 32,
	};

	//##ModelId=3DCE75E2003D
	//##Documentation
	//## ��������״̬ö��
	enum KREGION_STATUS {
		//##Documentation
		//## ����������״̬��
		REGION_S_IDLE,
		//##Documentation
		//## �ȴ�ִ�м���
		REGION_S_TO_LOAD,
		//##Documentation
		//## ������������
		REGION_S_LOADING,
		//##Documentation
		//## ����������ϣ����Ա�Ӧ��״̬��
		REGION_S_STANDBY,
		//##Documentation
		//## ���ݴ�����
		REGION_S_PROCESSING };

	struct KGroundLayerData
	{
		unsigned int uNumGrunode;	//�ر���ͼ�ε���Ŀ
		unsigned int uNumObject;	//�������������·��֮��Ķ������Ŀ
		KSPRCrunode* pGrunodes;
		KSPRCoverGroundObj* pObjects;
	};

	//##ModelId=3DE1311803A3
	//##Documentation
	//## �ڽ���������
	struct KBiosData
	{
		KBuildinObjFileHead	Numbers;
		KBuildinObj*		pBios;
		KIpotBuildinObj*	pLeafs;
		KBuildInLightInfo*	pLights;
	};
  private:

	//##ModelId=3DB90015018D
	//##Documentation
	//## �����ϰ���Ϣ��
	void LoadObstacle(
		//## �����ļ������ָ��
		KPakFile* pDataFile,
		//## �����ļ��Ĵ�С
		unsigned int uSize);
	//##ModelId=3DBDADE00001
	//##Documentation
	//## ������Ʒ��Ϣ��
	bool LoadAboveGroundObjects(
		//## �����ļ������ָ��
		KPakFile* pDataFile,
		//## �����ļ��Ĵ�С
		unsigned int uSize);
	//##ModelId=3DB9001D00BD
	//##Documentation
	//## ����ر���Ϣ
	bool LoadGroundLayer(
		//## �����ļ������ָ��
		KPakFile* pDataFile,
		//## �����ļ��Ĵ�С
		unsigned int uSize);
	// �����¼�����Ϣ
	void LoadTrap(
		//## �����ļ������ָ��
		KPakFile* pDataFile,
		//## �����ļ��Ĵ�С
		unsigned int uSize);

	void AddGrundCoverObj(KSPRCoverGroundObj* pObj);

  private:

	unsigned int	m_Flag;
	//int m_isClear ;
	//##ModelId=3DBD531C02B2
	//##Documentation
	//## ״̬���
	KREGION_STATUS m_Status;

	//##ModelId=3DDC11C400A3
	//##Documentation
	//## Region���Ͻǵ�������������ͼ�е����꣨��λ�����ص㣩
	POINT m_LeftTopCornerScenePos;
	//##ModelId=3DBE392600F3
	//##Documentation
	//## ����������������������
	POINT m_RegionIndex;
	//##ModelId=3DE131AE01A0
	//##Documentation
	//## �ڽ���������
	KBiosData m_BiosData; //�������� ��ľ ���ݵ�

//-----�ر�������-----
	KGroundLayerData	m_GroundLayerData;

	//##ModelId=3DD7F09402D4
	//##Documentation
	//## Ԥ����Ⱦ�ĵر���ͼ�Ρ�
	KRUImage* m_pPrerenderGroundImg;

	int      m_BackTreeNum;
	int      m_BackHouseNum;
	int      m_ClearState;
//--------------------------------------------------------------------------
	class _KGridRegData
	{
	public:
		_KGridRegData()
		{
#ifdef WIN32
			//ZeroStruct(m_adwDataFlag);
			memset(m_adwDataFlag, 0, sizeof(m_adwDataFlag));
#else
			//memset(m_adwDataFlag);
			memset(m_adwDataFlag, 0, sizeof(m_adwDataFlag));
#endif
		}

		int HasData(INT nX, INT nY) CONST
		{
			return (m_adwDataFlag[nX] & (1 << nY)) != 0;
		}
		INT GetData(INT nX, INT nY) CONST
		{
			INT nResult	= 0;
			//KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
			//KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
			if (nX<0 || nX>=REGION_WIDTH)
				goto Exit0;
			if (nY<0 || nY>=REGION_HEIGHT)
				goto Exit0;

			if (m_adwDataFlag[nX] & (1 << nY))
			{
				nResult	= m_mapRegData.find(MAKELONG(nX, nY))->second;
			}
Exit0:
			return nResult;
		}
		int SetData(INT nX, INT nY, INT nData)
		{
			unsigned long dwId	= MAKELONG(nX, nY);
			//KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
			//KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
			if (nX<0 || nX>=REGION_WIDTH)
				goto Exit0;
			if (nY<0 || nY>=REGION_HEIGHT)
				goto Exit0;

			if (nData)
			{
				m_adwDataFlag[nX] |= 1 << nY;
				m_mapRegData[dwId]	= nData;
			}
			else
			{
				m_adwDataFlag[nX] &= ~(1 << nY);
				m_mapRegData.erase(dwId);
			}
Exit0:
			return TRUE;
		}
		int IncData(INT nX, INT nY)
		{
			unsigned long dwId	= MAKELONG(nX, nY);
			//KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
			//KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
			if (nX<0 || nX>=REGION_WIDTH)
			  goto Exit0;
			if (nY<0 || nY>=REGION_HEIGHT)
				goto Exit0;

			if (m_adwDataFlag[nX] & (1 << nY))	// �Ѿ��еģ�+1
			{
				m_mapRegData[dwId]++;
			}
			else								// �µģ�ֱ����1
			{
				m_adwDataFlag[nX] |= 1 << nY;
				m_mapRegData[dwId]	= 1;
			}
Exit0:
			return TRUE;
		}
		int DecData(INT nX, INT nY)
		{
			int bResult	= FALSE;
			unsigned long dwId		= MAKELONG(nX, nY);
			//KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
			//KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
			if (nX<0 || nX>=REGION_WIDTH)
				goto Exit0;
			if (nY<0 || nY>=REGION_HEIGHT)
				goto Exit0;

			//KGLOG_PROCESS_ERROR(m_adwDataFlag[nX] & (1 << nY));
			if (m_adwDataFlag[nX] & (1 << nY))
			{
				if (--m_mapRegData[dwId] == 0)	// ��Ϊ0��
				{
					m_mapRegData.erase(dwId);
					m_adwDataFlag[nX] &= ~(1 << nY);
				}
			}
Exit0:
			return bResult;
		}

		int Clear()
		{
			//ZeroStruct(m_adwDataFlag);
#ifdef WIN32
			//ZeroStruct(m_adwDataFlag);
			memset(m_adwDataFlag, 0, sizeof(m_adwDataFlag));
#else
			//memset(m_adwDataFlag);
			memset(m_adwDataFlag, 0, sizeof(m_adwDataFlag));
#endif
			//_mapRegData::iterator it;
			//for( it = m_mapRegData.begin(); it != m_mapRegData.end(); ++it)
			//  m_mapRegData.erase(it);
			m_mapRegData.clear();
			return TRUE;
		}
	private:
		unsigned long m_adwDataFlag[REGION_WIDTH];	// ��λ�����ĳ���Ƿ������ݣ�unsigned longGION_HEIGHT == 32����һ��unsigned long��
		typedef std::map<unsigned long, INT> _mapRegData;
		_mapRegData m_mapRegData;
	};
//---------------------------------------------------------


	//long		m_ObstacleInfo[RWP_NUM_GROUND_CELL_H][RWP_NUM_GROUND_CELL_V * 2];
	//long		m_TrapInfo[RWP_NUM_GROUND_CELL_H][RWP_NUM_GROUND_CELL_V * 2];

	_KGridRegData	m_cObstacle;						                // ÿ�������ϵ��ϰ���Ϣ
//	_KGridData	m_cFindPath;										// Ѱ·��
	_KGridRegData	m_cTrap;							                // ÿ�������ϵ�Trap��Ϣ

};

#define	GROUND_IMG_OCCUPY_FLAG			Color.Color_b.b
#define	GROUND_IMG_OK_FLAG				Color.Color_b.g

#endif /* KSCENEPLACEREGIONC_H_HEADER_INCLUDED_C22EB91D */
