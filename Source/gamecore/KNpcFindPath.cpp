//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcFindPath.cpp
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------

#include	"KCore.h"
#include	<math.h>
#include	"KMath.h"
#include	"KNpcFindPath.h"
#include	"KSubWorld.h"
#include	"KNpc.h"
#include	"KPlayer.h"
#define	MAX_FIND_TIMER	30

//-------------------------------------------------------------------------
//	���ܣ����캯��
//-------------------------------------------------------------------------
KNpcFindPath::KNpcFindPath()
{
	m_nDestX = 0;
	m_nDestY = 0;
	m_nFindTimer = 0;
	m_nMaxTimeLong = MAX_FIND_TIMER;
	m_nFindState = 0;
	m_nPathSide = 0;
	m_nFindTimes = 0;
	m_NpcIdx = 0;
};

//-------------------------------------------------------------------------
//	���ܣ���ʼ��
//	������nNpc :���Ѱ·�������ĸ� npc ��
//-------------------------------------------------------------------------
void KNpcFindPath::Init(int nNpc)
{
	m_NpcIdx = nNpc;
	m_nDestX = 0;
	m_nDestY = 0;
	m_nFindTimer = 0;
	m_nMaxTimeLong = MAX_FIND_TIMER;
	m_nFindState = 0;
	m_nPathSide = 0;
	m_nFindTimes = 0;
}
//-------------------------------------------------------------------------
//	���ܣ����뵱ǰ���ꡢ����Ŀ������ꡢ�ٶȣ�Ѱ·�ҵ���һ��Ӧ���ߵķ���
//	����ֵ���������0�����ϰ�����������;1���ҵ�һ�����򣬷���ֵ����pnGetDir (��64����)��
//			-1������ͼ����ȥ��
//-------------------------------------------------------------------------
#define		defFIND_PATH_STOP_DISTANCE		64
#define		defFIND_PATH_DISTANCE		1

int	KNpcFindPath::GetDir(int nXpos,int nYpos, int nDir, int nDestX, int nDestY, int nMoveSpeed, int *pnGetDir)
{
	if (Npc[m_NpcIdx].m_AutoplayId<=0 && Npc[m_NpcIdx].m_nIsOver)
	{//�һ�״̬������
		m_nFindTimer = 0;
		m_nFindState = 0;
		m_nFindTimes = 0;
	    Npc[m_NpcIdx].m_nIsOver=FALSE;
		return 0;

	}

	// �������ӽ�����Ϊ�Ѿ��ߵ���
	if (!CheckDistance(nXpos>>10, nYpos>>10,nDestX,nDestY,nMoveSpeed)) // 15 m_CurrentRunSpeed	  nMoveSpeed
	{
		if (Npc[m_NpcIdx].m_AutoplayId>0 && Npc[m_NpcIdx].IsPlayer())
		{//�һ�״̬������
		  Player[CLIENT_PLAYER_INDEX].m_Autoplay.nRunLoop=0;
		}
		m_nFindTimer = 0;
		m_nFindState = 0;
		m_nFindTimes = 0;
		return 0;
	}
	if (Npc[m_NpcIdx].m_AutoplayId>0 && Npc[m_NpcIdx].IsPlayer())
	{//�һ�״̬
		Player[CLIENT_PLAYER_INDEX].m_Autoplay.nRunLoop ++;
		int nRunTime   = Player[CLIENT_PLAYER_INDEX].m_Autoplay.nRunLoop;
		int nForBitTime= Player[CLIENT_PLAYER_INDEX].m_Autoplay.nAutoTime;

		//if  (nForBitTime<=0)
		nForBitTime =10;

		if (nRunTime>nForBitTime*18 && nRunTime%(nForBitTime*18)==0)
		{//�ܵ�̫����,��û���ܵ�.������,��ֱ�ӷɵ�������
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("����:�ƶ���ʱ,ģʽ����ɹ�!");
			if  (Npc[m_NpcIdx].m_FightMode>0)
			    SendClientCmdSunyi(SubWorld[0].m_SubWorldID,nDestX,nDestY,0); //����˲������

			Player[CLIENT_PLAYER_INDEX].m_Autoplay.nRunLoop=0;
			m_nFindTimer = 0;
			m_nFindState = 0;
			m_nFindTimes = 0;
			return 0;
		}
	}
	// Ŀ�������б仯��ȡ��ԭ������·״̬
	if (m_nDestX != nDestX || m_nDestY != nDestY)
	{
		m_nFindTimer = 0;
		m_nFindState = 0;
		m_nFindTimes = 0;
		m_nDestX = nDestX;
		m_nDestY = nDestY;
	}

	int	x, y, nWantDir,n;
	       nWantDir = g_GetDirIndex(nXpos >> 10, nYpos >> 10, nDestX, nDestY);	  //��ȡҪ�ߵķ���


    if (nWantDir < 0 )
	{
	  nWantDir = MaxMissleDir + nWantDir;
	}
    if (nWantDir >= MaxMissleDir)
	{
		nWantDir=nWantDir%MaxMissleDir;
	}
	x = g_DirCos(nWantDir,MaxMissleDir) * nMoveSpeed*defFIND_PATH_DISTANCE; //�¸�Ҫ�ߵĵ�
	y = g_DirSin(nWantDir,MaxMissleDir) * nMoveSpeed*defFIND_PATH_DISTANCE; //�¸�Ҫ�ߵĵ�
	// �����·��ֱ����
	int nCheckBarrier = CheckBarrierMin(x, y);

	if (nCheckBarrier == Obstacle_NULL)
	{//���û���ϰ�
		m_nFindState = 0;
		*pnGetDir = nWantDir;	//�������������
		return 1;
	}
	// ��ͼ��Ե
	else if (nCheckBarrier == 0xff)
	{
		return -1;
	}

	int		i;
	// �ӷ���·״̬������·״̬
	if (m_nFindState == 0)
	{
		if (Npc[m_NpcIdx].m_RegionIndex >= 0)
			nCheckBarrier = SubWorld[0].TestBarrier(nDestX, nDestY);//�����ж�
		else
			nCheckBarrier = 0xff;

		if (nCheckBarrier != 0 && !CheckDistance(nXpos >> 10, nYpos >> 10, nDestX, nDestY, defFIND_PATH_STOP_DISTANCE))
		{// ���Ŀ������ϰ����Ҿ��������������
			m_nFindTimes = 0;
			return 0;
		}

	// ����ڶ��ν������״̬�������ˣ�ֻ��һ���䣩
	//	m_nFindTimes++;
	//	if (m_nFindTimes > 1)
	//	{
	//		m_nFindTimes = 0;
	//		return 0;
	//	}
		int	nTempDir8, nTempDir64;
		    nTempDir8 = Dir64To8(nWantDir) + 8;
		// ת���� 8 �����ǰ�����Ƿ����
		    nTempDir64 = Dir8To64(nTempDir8 & 0x07);
		if (nTempDir64 < 0 )
		{
			nWantDir = MaxMissleDir+ nWantDir;
		}

		if (nTempDir64 >= MaxMissleDir)
		{
			nTempDir64 = nTempDir64%MaxMissleDir;
		}
		x = g_DirCos( nTempDir64, MaxMissleDir ) * nMoveSpeed;
		y = g_DirSin( nTempDir64, MaxMissleDir ) * nMoveSpeed;

		if (CheckBarrierMin(x, y) == Obstacle_NULL)
		{
			m_nFindState = 1;
			m_nFindTimer = 0;
			if ((nTempDir64 < nWantDir && nWantDir - nTempDir64 <= 4) || (nTempDir64 > nWantDir && nTempDir64 - nWantDir >= 60))
				m_nPathSide = 0;
			else
				m_nPathSide = 1;
			*pnGetDir = nTempDir64;
			return 1;
		}
		// �� 8 ����Ѱ�ң�����ȥ����ͱ�������� 6 ������
		for (i = 1; i < 8; ++i)	   // 32
		{
			nTempDir64 = Dir8To64((nTempDir8 + i) & 0x07);
			if (nTempDir64 < 0 )
			{
				nWantDir = MaxMissleDir+ nWantDir;
			}

			if (nTempDir64 >= MaxMissleDir)
			{
				nTempDir64 = nTempDir64%MaxMissleDir;
			}

			x = g_DirCos( nTempDir64, MaxMissleDir ) * nMoveSpeed*defFIND_PATH_DISTANCE;
			y = g_DirSin( nTempDir64, MaxMissleDir ) * nMoveSpeed*defFIND_PATH_DISTANCE;


			if (CheckBarrierMin(x, y) == Obstacle_NULL)
			{
				m_nFindState = 1;
				m_nFindTimer = 0;
				m_nPathSide = 1;
				*pnGetDir = nTempDir64;
				return 1;
			}

			nTempDir64 = Dir8To64((nTempDir8 - i) & 0x07);

			if (nTempDir64 < 0 )
			{
				nWantDir = MaxMissleDir+ nWantDir;
			}

			if (nTempDir64 >= MaxMissleDir)
			{
				nTempDir64 = nTempDir64%MaxMissleDir;
			}

			x = g_DirCos( nTempDir64, MaxMissleDir ) * nMoveSpeed*defFIND_PATH_DISTANCE;
			y = g_DirSin( nTempDir64, MaxMissleDir ) * nMoveSpeed*defFIND_PATH_DISTANCE;

			if (CheckBarrierMin(x, y) == Obstacle_NULL )
			{
				m_nFindState = 1;
				m_nFindTimer = 0;
				m_nPathSide = 0;
				*pnGetDir = nTempDir64;
				return 1;
			}
		}
		return 0;
	}
	// ԭ������·״̬��������·
	else
	{
		// �����·ʱ�������������
	//	if (m_nFindTimer >= m_nMaxTimeLong)
	//	{
	//		m_nFindState = 0;
	//		return 0;
	//	}
		m_nFindTimer++;
		int	nWantDir8, nTempDir64;
		    nWantDir8 = Dir64To8(nWantDir) + 8;
		// ��ǰ����λ��Ŀ�귽����Ҳ�
		if (m_nPathSide == 1)
		{
			// �ж��Ƿ���Ҫ��⵱ǰĿ�곯���Ӧ�� 8 ������
			nTempDir64 = Dir8To64(nWantDir8 & 0x07);
			if ((nTempDir64 < nWantDir && nWantDir - nTempDir64 <= 4) || (nTempDir64 > nWantDir && nTempDir64 - nWantDir >= 60))
				i = 1;
			else
				i = 0;
			// �������
			for (; i < 8; ++i)
			{
				nTempDir64 = Dir8To64((nWantDir8 + i) & 0x07);

				if (nTempDir64 < 0 )
				{
					nWantDir = MaxMissleDir+ nWantDir;
				}

				if (nTempDir64 >= MaxMissleDir)
				{
					nTempDir64 = nTempDir64%MaxMissleDir;
				}

				x = g_DirCos( nTempDir64, MaxMissleDir ) * nMoveSpeed*defFIND_PATH_DISTANCE;
				y = g_DirSin( nTempDir64, MaxMissleDir ) * nMoveSpeed*defFIND_PATH_DISTANCE;
				if (CheckBarrierMin(x, y) == Obstacle_NULL)
				{
					*pnGetDir = nTempDir64;
					return 1;
				}
			}
			m_nFindState = 1;
			m_nFindTimer = 0;
			return 0;
		}
		// ��ǰ����λ��Ŀ�귽������
		else
		{
			// �ж��Ƿ���Ҫ��⵱ǰĿ�곯���Ӧ�� 8 ������
			nTempDir64 = Dir8To64(nWantDir8 & 0x07);
			if ((nTempDir64 < nWantDir && nWantDir - nTempDir64 <= 4) || (nTempDir64 > nWantDir && nTempDir64 - nWantDir >= 60))
				i = 0;
			else
				i = 1;
			// �������
			for (; i < 8; ++i)
			{
				nTempDir64 = Dir8To64((nWantDir8 - i) & 0x07);
				if (nTempDir64 < 0 )
				{
					nWantDir = MaxMissleDir+ nWantDir;
				}

				if (nTempDir64 >= MaxMissleDir)
				{
					nTempDir64 = nTempDir64%MaxMissleDir;
				}

				x = g_DirCos(nTempDir64,MaxMissleDir) * nMoveSpeed*defFIND_PATH_DISTANCE;
				y = g_DirSin(nTempDir64,MaxMissleDir) * nMoveSpeed*defFIND_PATH_DISTANCE;
				if (CheckBarrierMin(x, y) == Obstacle_NULL )
				{
					*pnGetDir = nTempDir64;
					return 1;
				}
			}
			m_nFindState = 1;
			m_nFindTimer = 0;
			return 0;
		}
	}

	m_nFindState = 0;
	m_nFindTimer = 0;
	return 0;
}

//-------------------------------------------------------------------------
//	���ܣ�	64 ����ת��Ϊ 8 ����
//-------------------------------------------------------------------------
int		KNpcFindPath::Dir64To8(int nDir)
{
	return ((nDir + 4) >> 3) & 0x07;
}

//-------------------------------------------------------------------------
//	���ܣ�	8 ����ת��Ϊ 64 ����
//-------------------------------------------------------------------------
int		KNpcFindPath::Dir8To64(int nDir)
{
	return nDir << 3;  // *8
}

//-------------------------------------------------------------------------
//	���ܣ�	�ж�������ֱ�߾����Ƿ���ڻ���ڸ�������
//	���أ�	����С�� nDistance ���� FALSE �����򷵻� TRUE
//-------------------------------------------------------------------------
int	KNpcFindPath::CheckDistance(int x1, int y1, int x2, int y2, int nDistance)
{
	return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) >= nDistance * nDistance );
}

//-------------------------------------------------------------------------
//	���ܣ�	�ж�ĳ�����Ƿ����ϰ�
//-------------------------------------------------------------------------
/*
int	KNpcFindPath::CheckBarrier(int nChangeX, int nChangeY)
{
#ifdef _SERVER
	return SubWorld[Npc[m_NpcIdx].m_SubWorldIndex].TestBarrierMin(Npc[m_NpcIdx].m_RegionIndex, Npc[m_NpcIdx].m_MapX, Npc[m_NpcIdx].m_MapY, Npc[m_NpcIdx].m_OffX, Npc[m_NpcIdx].m_OffY, nChangeX, nChangeY);
#else
	if (Npc[m_NpcIdx].m_RegionIndex >= 0)
		return SubWorld[0].TestBarrierMin(Npc[m_NpcIdx].m_RegionIndex, Npc[m_NpcIdx].m_MapX, Npc[m_NpcIdx].m_MapY, Npc[m_NpcIdx].m_OffX, Npc[m_NpcIdx].m_OffY, nChangeX, nChangeY);
	else
		return 0xff;
#endif
}
*/
//������������ȷ����
int	KNpcFindPath::CheckBarrierMin(int nChangeX, int nChangeY)
{
	if (Npc[m_NpcIdx].m_RegionIndex >= 0)
  //return SubWorld[0].m_Region[Npc[m_NpcIdx].m_RegionIndex].GetBarrierMin(nChangeX, nChangeY, Npc[m_NpcIdx].m_OffX, Npc[m_NpcIdx].m_OffY, TRUE);  //���ϰ�ʱ
	    return SubWorld[0].TestBarrierMin(Npc[m_NpcIdx].m_RegionIndex, Npc[m_NpcIdx].m_MapX, Npc[m_NpcIdx].m_MapY, Npc[m_NpcIdx].m_OffX, Npc[m_NpcIdx].m_OffY, nChangeX, nChangeY);
	else
		return 0xff;
}
/*//�����ڴ��Բ���
int	KNpcFindPath::CheckBarrier(int nChangeX, int nChangeY)
{
#ifdef _SERVER
   // return	SubWorld[Npc[m_NpcIdx].m_SubWorldIndex].m_Region[Npc[m_NpcIdx].m_RegionIndex].GetBarrierMin(nChangeX, nChangeY, Npc[m_NpcIdx].m_OffX, Npc[m_NpcIdx].m_OffY, TRUE);  //���ϰ�ʱ
	    return SubWorld[Npc[m_NpcIdx].m_SubWorldIndex].TestBarrier(Npc[m_NpcIdx].m_RegionIndex, Npc[m_NpcIdx].m_MapX, Npc[m_NpcIdx].m_MapY, Npc[m_NpcIdx].m_OffX, Npc[m_NpcIdx].m_OffY, nChangeX, nChangeY);
#else
	if (Npc[m_NpcIdx].m_RegionIndex >= 0)
  //return SubWorld[0].m_Region[Npc[m_NpcIdx].m_RegionIndex].GetBarrierMin(nChangeX, nChangeY, Npc[m_NpcIdx].m_OffX, Npc[m_NpcIdx].m_OffY, TRUE);  //���ϰ�ʱ
	    return SubWorld[0].TestBarrier(Npc[m_NpcIdx].m_RegionIndex, Npc[m_NpcIdx].m_MapX, Npc[m_NpcIdx].m_MapY, Npc[m_NpcIdx].m_OffX, Npc[m_NpcIdx].m_OffY, nChangeX, nChangeY);
	else
		return 0xff;
#endif
}
*/
