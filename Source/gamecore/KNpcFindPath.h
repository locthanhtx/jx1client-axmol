//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcFindPath.h
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------

#ifndef KNPCFINDPATH_H
#define KNPCFINDPATH_H

class KNpcFindPath
{
public:
	int				m_NpcIdx;			// ���Ѱ·�������ĸ� npc ��
	int				m_nDestX;			// Ŀ������� x
	int				m_nDestY;			// Ŀ������� y
	int				m_nFindTimer;		// ��·��һ��ʱ���Ժ�û�ҵ��Ͳ�����
	int				m_nMaxTimeLong;		// �Ҷ೤ʱ���·
	int				m_nFindState;		// ��ǰ�Ƿ�����·״̬
	int				m_nPathSide;		// ���ĸ�������
	int				m_nFindTimes;		// ��һ��Ŀ�����·�Ĵ���
public:
	KNpcFindPath();
	//	���ܣ���ʼ��	nNpc :���Ѱ·�������ĸ� npc ��
	void			Init(int nNpc);
	//	���ܣ����뵱ǰ���ꡢ����Ŀ������ꡢ�ٶȣ�Ѱ·�ҵ���һ��Ӧ���ߵķ���
	//	����ֵ���������false����������;true ���ҵ�һ�����򣬷���ֵ����pnGetDir (��64����)
	int				GetDir(int nXpos,int nYpos, int nDir, int nDestX, int nDestY, int nMoveSpeed, int *pnGetDir);
	// 64 ����ת��Ϊ 8 ����
	int				Dir64To8(int nDir);
	// 8 ����ת��Ϊ 64 ����
	int				Dir8To64(int nDir);
	// �ж�������ֱ�߾����Ƿ���ڻ���ڸ�������
	int			CheckDistance(int x1, int y1, int x2, int y2, int nDistance);
	// �ж�ĳ�����Ƿ����ϰ�
	int				CheckBarrierMin(int nChangeX, int nChangeY);
//	int				CheckBarrier(int nChangeX, int nChangeY);
};

#endif
