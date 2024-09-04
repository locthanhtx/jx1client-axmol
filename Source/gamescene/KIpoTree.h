// ***************************************************************************************
// ������ͼ�ж��󼯺���
// Copyright : Kingsoft 2002
// Author    : wooy(wu yue)
// CreateTime: 2002-11-19
// ---------------------------------------------------------------------------------------
// ***************************************************************************************

#ifndef KIPOTREE_H_HEADER_INCLUDED_C22523F9
#define KIPOTREE_H_HEADER_INCLUDED_C22523F9

#include "SceneDataDef.h"
#include "KIpotLeaf.h"
#include "KIpotBranch.h"
#include "gamecore/GameDataDef.h"
//#include "../../Represent/iRepresent/iRepresentshell.h"

#pragma warning(disable : 4786)
#include <list>
#ifndef __CODEWARRIOR
using namespace std;
#endif

#define TEXT_IN_SINGLE_PLANE_COORD -32767
#define LIGHTING_GRID_SIZEX			32							// ���������Ϣ���ӵĴ�С������������512
#define LIGHTING_GRID_SIZEY			32							// ���������Ϣ���ӵĴ�С������������512
//RWPP_AREGION_WIDTH��RWPP_AREGION_HEIGHTҪ��KScenePlaceRegionC.h�е�ֵ����һ��
#define AREGION_WIDTH	512
#define AREGION_HEIGHT	1024
#define LIGHTING_GRID_WIDTH	(AREGION_WIDTH / LIGHTING_GRID_SIZEX * 3)	// �Ÿ��������򰲸��Ӽ���Ŀ��
#define LIGHTING_GRID_HEIGHT (AREGION_HEIGHT / LIGHTING_GRID_SIZEY * 3)	// �Ÿ��������򰲸��Ӽ���ĸ߶�

struct KIpotLeaf;
struct KIpotRuntimeObj;
class  KIpotBranch;

// ���ڴ洢�ڵ���Ϣ�Ľṹ
struct KVector2
{
	float fX;
	float fY;
};

struct KLObstacle
{
	int nObstacle;						// �ڵ�ID
	KVector2 vDir;						// �ϰ�����
    unsigned nReserveForAlign;
};

class KLightBase
{
public:
	KIpotLeaf* m_pParent;				// ��Դ��������ָ��
	KPosition3 m_oPosition;				// ��Դλ��
	DWORD m_dwColor;					// ��Դ��ɫ������
	long  m_nRadius;					// ���ð뾶
public:
	virtual void Breath(){}
};

class KSelfBreathLight : public KLightBase
{
public:
	float	fMinRange;					// ��С�뾶
	float	fMaxRange;					// ���뾶
	float	fRadius;					// �Ը�������ʾ�Ĺ�Դ�뾶
	float	fCycle;						// ѭ������,����
	int		nDir;						// �뾶�仯����
	DWORD	dwLastBreathTime;			// �ϴκ���ʱ��
public:
	KSelfBreathLight();
	virtual void Breath();
};

//##ModelId=3DD95FC001F8
//##Documentation
//## �����еĶ�����ݿռ��໥λ�ã�����Ϊ��������νṹ�ļ��ϡ�
class KIpoTree
{
  public:

  	//##ModelId=3DDA92EE0346
	KIpoTree();

	//##ModelId=3DDA92EE0350
	~KIpoTree();

	//##ModelId=3DD9ECFD00E6
	//##Documentation
	//## ������ö��󼯺���
	void Paint(RECT* pRepresentArea, enum IPOT_RENDER_LAYER eLayer);

	//##ModelId=3DDA96360240
	//##Documentation
	//## ����������е����ݡ�������֦�����ڡ��Ա��ⲿ���ö�����ɾ����Ҷ�ӣ���ת��m_PermanetBranch֦�ϡ�
	void Clear();

	//##ModelId=3DE1D38A0173
	//##Documentation
	//## ����������е����ݡ�������֦�����ڡ��������ϵ�KIpotRuntimeObj���͵�Ҷ��ȫ��ժ��,ת��m_PermanetBranch֦�ϡ�
	void Fell();

	//m_PermanentLeaf�����ӵ�KIpotRuntimeObj���͵�Ҷ�ӣ�����KeepRtoArea��Χ��ȫ���ƶ�����֦�ϣ������Ķ�������
	void StrewRtoLeafs(RECT& KeepRtoArea);

	//##ModelId=3DDAB2E20352
	//##Documentation
	//## �����֦��
	//## �ɹ����ط�0ֵ�����򷵻�0ֵ��
	void AddBranch(
		//##Documentation
		//## ��֦�������Ķ����ָ��
		KIpotBuildinObj* pBranchObj);

	//##ModelId=3DDA96780082
	//##Documentation
	//## ɾ���������б�������������ʱ�������Ҷ��
//	void RemoveAllRtoLeafs();

	//##ModelId=3DE2A05601A3
	//##Documentation
	//## ɾ��ȫ��������Χ��rtoҶ�ӡ�
/*	void RemoveOutsideRtoLeafs(
		//##Documentation
		//## ָ���ķ�Χ
		RECT& rc);
*/
	//##ModelId=3DE3D4AB036F
	//##Documentation
	//## ������߷�ʽ������ڽ�����Ҷ��
	void AddLeafLine(
		//##Documentation
		//## ���߷�ʽ������ڽ�����
		KIpotBuildinObj* pLeaf);

	//##ModelId=3DE3D4AB0371
	//##Documentation
	//## ����Ե㷽ʽ������ڽ�����Ҷ��
	void AddLeafPoint(
		//##Documentation
		//## �Ե㷽ʽ������ڽ�����
		KIpotLeaf* pLeaf);

	//����������֦��λ��
	void SetPermanentBranchPos(int nLeftX, int nRightX, int y);

	// ɾ��һ��KIpotRuntimeObj��ͬʱɾ�������Ĺ�Դ
	void PluckRto(KIpotRuntimeObj* pLeaf);

	// ���þŸ������������Ͻ�����
	void SetLightenAreaLeftTopPos(int nX, int nY)
	{
		m_nLeftTopX = nX;
		m_nLeftTopY = nY;
	}

	void Breathe();

	void AddBuildinLight(KBuildInLightInfo* pLights, int nCount);

	// ���õ�ǰʱ�䣬nCurrentTimeΪ���賿��ʼ�ķ�����
	void SetCurrentTime(int nCurrentTime){m_nCurrentTime = nCurrentTime;}
	// �����Ƿ�������
	void SetIsIndoor(bool bIsIndoor){m_bIsIndoor = bIsIndoor;}
	
	// ���û��������ɫ������
	void SetAmbient(DWORD dwAmbient){m_dwAmbient = dwAmbient;}

	// ��Ⱦ����ͼ
	void RenderLightMap();

	// �����Ƿ����ͼ�ڽ���Դ�����ڱ��ְ��������
	void EnableBioLights(bool bEnable);
	// �����Ƿ���̬����
	void EnableDynamicLights(bool bEnable);

  private:

	//ɾ������Զ����Ҷһ���rtoҶ��
	void RemoveRtoGroupWithPermanentLeaf();

  private:

	//##ModelId=3DDABBB80020
	enum PARAM
	{
		//##Documentation
		//## Ҷ�ӱ����ֵ��Ҷ�Ӱ�ָ�����ݵ�mask
		LEAF_IDVALUE_HALF_POINTER_MASK = 0xFFFF, 
		//##Documentation
		//## Ҷ�ӱ����ֵ��������ֵ֦��mask
		LEAF_IDVALUE_BRANCH_VALUE_MASK = 0xFF0000
	};

 private:
	
	//##ModelId=3DDA93C50327
	//##Documentation
	//## ָ��������֦�ṹ��
	KIpotBranch* m_pMainBranch;

	//##ModelId=3DE1D266030D
	//##Documentation
	//## ��Զ����֦������֦�ڣ�����֦����
	KIpotBranch m_DefaultBranch;

	//��Զ��rto��Ҷ��������֯û�м�����֦��rtoҶ��
	KIpotLeaf	m_PermanentLeaf;

	// �Ÿ������������Ͻ�X����
	int m_nLeftTopX;
	// �Ÿ������������Ͻ�Y����
	int m_nLeftTopY;
	
	int m_nCurrentTime;
	bool m_bIsIndoor;

	// �Ƿ����ͼ�ڽ���Դ
	bool m_bProcessBioLights;
	// �Ƿ���̬��
	bool m_bDynamicLighting;

	// ���������ɫ��ǿ��
	DWORD m_dwAmbient;
	KLObstacle pObstacle[LIGHTING_GRID_WIDTH * LIGHTING_GRID_HEIGHT];	// ������Ϣ����
	//KLColor pLColor[LIGHTING_GRID_WIDTH * LIGHTING_GRID_HEIGHT];		// ������ռ�����м���
	KLColor *pLColor;                                           		// ������ռ�����м���
    unsigned char *m_pbyLColorBase;
	KLColor *ptp;                                                       // ���澭��ƽ�����ɵĹ��ռ�����м���
	unsigned char *m_pby_ptpBase;


	DWORD pLightingArray[LIGHTING_GRID_WIDTH * LIGHTING_GRID_HEIGHT];	// ����ͼ��Ϣ����
	list<KLightBase*> m_LightList;

	// ��������赲�Ļص�����
	static void ObjectsCallback(void* p, KIpotLeaf* pLeaf);
	void FillLineObstacle(int nX1, int nY1, int nX2, int nY2, int nObstacleID);
	bool CanLighting(int x, int y, int nLightPosX, int nLightPosY);
};

#endif /* KIPOTREE_H_HEADER_INCLUDED_C22523F9 */
