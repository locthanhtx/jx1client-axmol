#ifndef	KWorldH
#define	KWorldH

#define	MAX_SUBWORLD	1  //�ͻ��˵ĵ�ͼ����
#include <map>
#include <vector>

#define	MAP_REGION		-2
//-------------------------------------------------------------
#include "engine/KEngine.h"
#include "KRegion.h"
//#include "KWeatherMgr.h"
#include "GameDataDef.h"
//-------------------------------------------------------------

class  KSubWorld
{
public:
	int			m_nIndex;
	int			m_SubWorldID;        //��ͼ���
	int			m_SubWhereTeam;      //��ǰ��ͼ�����Ǹ���������õ�
	int        m_IsShowLoop;        //�Ƿ���ʾ��ͼ��Ϣ

	int         m_WargKillNum;	     //������������	 ��
	int         m_WarsKillNum;		 //�ط���������	 ��

	BYTE        m_IsWhereCity;        //�������ĸ����б����� 1,2.....7 ����д���ս�ļ�
    BYTE        m_WarIsWho;           //�����ķ��� �� ���ɵ�ͼ--����  1 Ϊ���� 2 Ϊ�ط�
	int         m_ShuiShouType;       //˰�յ�����  ������ ���Ǽ���

    BYTE        m_IswarCity;          //�Ƿ�ռ��

	int         m_IsLuKey;	          //��ͼ�����˱���
	int         m_IsJinYan;		      //��ͼ�ľ��鱶��
	int         m_IsJinQian;		  //��ͼ����������

	int         m_WarCityManey; //���н����ʽ�
	int         m_WarCityJB;    //���
	int         m_WarCityGX;    //����
	int         m_GuziGameStat;

	int         m_nWPKFlag;  //�Ƿ�����PK
	int         m_nWFight;   //�Ƿ�ս��ģʽ
	int         m_nWcomp;    //�õ�ͼ����Ӫ		 Npc[nNpcIndex].SetCurrentCamp(nValue);
    int         m_nMapTime;  //�ܶ�����ʱ��
    int         m_nSleepTime; //����ʱ��ʱ��

	int         m_nNpcZongNum;  //�ܲ���
	int         m_nCurNpcNum;	//��ǰ����
	int         m_nCurNum;      //ÿ���Ĺ�����

	int         m_nPleyerZongNum;  //������
	int         m_nCurPleyerNum;   //��ǰ����
	KRegion     *m_Region;

	int			m_ClientRegionIdx[MAX_REGION];
	char		m_szMapPath[FILE_NAME_LENGTH];

	int         m_nLeft;
	int         m_nTop;

int			m_nWorldRegionWidth;			//	��ͼ��ʵ�ʿ���
int			m_nWorldRegionHeight;			//  ��ͼ��ʵ�ʸ߶�
int			m_nTotalRegion;					//	��ͼ��ʵ�������С
int			m_nRegionWidth;					//	��ͼ��ÿ��Region�ĸ��ӿ��ȣ��̶�16��
int			m_nRegionHeight;				//	��ͼ��ÿ��Region�ĸ��Ӹ߶ȣ��̶�32��
int			m_nCellWidth;					//	Cell�����ؿ��ȣ��̶�32��
int			m_nCellHeight;					//	Cell�����ظ߶ȣ��̶�32��
int			m_nRegionBeginX;		        //  ��ͼ�Ŀ�ʼλ��X LEFT
int			m_nRegionBeginY;                //  ��ͼ�Ŀ�ʼλ��Y TOP
int	        m_nRegionEndX;                  //  ��ͼ�Ľ���λ��X Rigth
int         m_nRegionEndY;                  //  ��ͼ�Ľ���λ��Y Buttom
int			m_nWeather;						//	�����仯
unsigned long		m_dwCurrentTime;				//	��ǰ֡
//KWorldMsg	m_WorldMessage;					//	��Ϣ
KList		m_NoneRegionNpcList;			//	���ڵ�ͼ�ϵ�NPC

typedef std::map<INT, std::string> _WorldMapInfo;                 //SkillId,Echance
_WorldMapInfo nWorldMapInfo;

private:
	//	BYTE*		m_pNpcRef;							// ÿ�������ϵ�NPC��Ŀ
	//  BYTE*		m_pObjRef;							// ÿ�������ϵ�OBJ��Ŀ
public:
	KSubWorld();
	~KSubWorld();
	void		Activate();
	void		GetFreeObjPos(POINT& pos);
	void        GetFreePos(POINT& pos);
	void        GetFreeNewObjPos(POINT& pos);

	int		CanPutObj(POINT pos,int nModle=0,int nIsCheckNpc=FALSE);
	void		ObjChangeRegion(int nSrcRegionIdx, int nDesRegionIdx, int nObjIdx);
	void		MissleChangeRegion(int nSrcRegionIdx, int nDesRegionIdx, int nObjIdx);
	void		AddPlayer(int nRegion, int nIdx);
	void		RemovePlayer(int nRegion, int nIdx);
	void		Close();
	int			GetDistance(int nRx1, int nRy1, int nRx2, int nRy2);// ���ؼ�����
	void        LookAt(int nX, int nY, int nZ);
	void        GetLocalPositionByMps(int& nX, int& nY, int nZ);
	void        GetMpsByLocalPosition(int& nX, int& nY, int nZ);  //��Ļ����ת����������
	void		NewMap2Mps(int nR, int nX, int nY, int nDx, int nDy, int *nRx, int *nRy);		// ��������ת��������
	//static void Map2Mps(int nRx, int nRy, int nX, int nY, int nDx, int nDy, int *pnX, int *pnY);// ��������ת��������
	void		Mps2MapnRodom(int * nR, int * nX, int * nY, int *nDx, int * nDy,int *Rx,int*Ry,int nOffwminx=1,int nOffwmaxx=1,int nOffhminy=1,int nOffhmaxy=1);	// ��������ת��������
	void		Mps2Map(int Rx, int Ry, int * nR, int * nX, int * nY, int *nDx, int * nDy);	    // ��������ת��������
	void		GetMps(int *nX, int *nY, int nSpeed, int nDir, int nMaxDir = 64);			    // ȡ��ĳ����ĳ�ٶ���һ�������
	BYTE		TestBarrier(int nMpsX, int nMpsY);
	BYTE		TestBarrier(int nRegion, int nMapX, int nMapY, int nDx, int nDy, int nChangeX, int nChangeY);	// �����һ���Ƿ�Ϊ�ϰ�
	BYTE		TestBarrierMin(int nRegion, int nMapX, int nMapY, int nDx, int nDy, int nChangeX, int nChangeY);	// �����һ���Ƿ�Ϊ�ϰ�
	BYTE		GetBarrier(int nMpsX, int nMpsY);											// ȡ��ĳ����ϰ���Ϣ
	unsigned long		GetTrap(int nMpsX, int nMpsY);
	int		SetTrap(int nMpsX, int nMpsY,int nCellNum=1,unsigned long uTrapScriptId=0);
	void		MessageLoop();
	int			FindRegion(int RegionID);													// �ҵ�ĳID��Region������
	int			FindFreeRegion(int nX = 0, int nY = 0);
	void		RemoveNpc(int nIdx);
	void        SetFocusPosition(int nX, int nY, int nZ);	  //���ý�������
	//�ͻ���
	int		LoadMapC(int nIdx, int nRegion,bool isClearAll=false);
	void		NpcChangeRegion(int nSrcRegionIdx, int nDesRegionIdx, int nNpcIdx,bool isClearAll=false);
	void		Paint();
	void		Mps2Screen(int *Rx, int *Ry);
	void		Screen2Mps(int *Rx, int *Ry);
	void        Mps2MapClient(int Rx, int Ry, int * nR, int * nX, int * nY, int *nDx, int * nDy);

private:
	void		LoadTrap();
	//void		ProcessMsg(KWorldMsgNode *pMsg);
	void		LoadCell();
};

extern  KSubWorld	*SubWorld;//SubWorld[MAX_SUBWORLD];

#endif
