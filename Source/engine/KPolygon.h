//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
// �Զ�Ѱ·��
// File:	KPolygon.h
// Date:	2002.01.10
// Code:	Spe
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KPolygon_H
#define KPolygon_H
#include "cocos2d.h"
#include "KbugInfo.h"
USING_NS_AX;

#define	defMaxVertex	8   //������
typedef struct
{
	int		nVertex;
	POINT	Pos[defMaxVertex];
}
TPolygon, *LPPolygon;

class  KPolygon
{
private:
	TPolygon	m_Polygon;
public:
	KPolygon();
	bool		IsPointInPolygon(POINT pos);					//	��鵱ǰ���Ƿ��ڶ������
	bool		IsPointInPolygon(int x, int y);					//	��鵱ǰ���Ƿ��ڶ������
	bool		AddPointToVertex(POINT pos);					//	���Ӷ���
	bool		AddPointToVertex(int x, int y);					//	���Ӷ���
	int			GetNearVertex(POINT pos);						//	ȡ���뵱ǰ������Ķ�������
	int			GetVertexNumber() {return m_Polygon.nVertex;};	//	ȡ�ö���εĶ�����Ŀ
	bool		GetIndexVertex(int i, POINT* vertex);			//	ȡ�ö���εĵ�I����������
	bool		RemoveIndexVertex(int i);						//	�Ӷ������ȥ����һ������
	void		LoopVertex(int i);								//	�Ѷ���ζ���˳�����I��ѭ���仯
	void		Clear();										//	��ն������Ϣ
	void		GetCenterPos(POINT*	pos);						//	ȡ������λ��
	LPPolygon	GetPolygonPtr() {return &m_Polygon;};			//	ȡ�ö����ָ��
	bool		ShiftVertex(int nDir, int nDistance);			//	�����ⶥ����ʲô�����ƶ�
	int			GetLeftVertex();								//	ȡ�����󶥵������
	int			GetRightVertex();								//	ȡ�����Ҷ��������
};

#endif
