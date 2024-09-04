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
	int32_t		nVertex;
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
	bool		IsPointInPolygon(int32_t x, int32_t y);					//	��鵱ǰ���Ƿ��ڶ������
	bool		AddPointToVertex(POINT pos);					//	���Ӷ���
	bool		AddPointToVertex(int32_t x, int32_t y);					//	���Ӷ���
	int32_t			GetNearVertex(POINT pos);						//	ȡ���뵱ǰ������Ķ�������
	int32_t			GetVertexNumber() {return m_Polygon.nVertex;};	//	ȡ�ö���εĶ�����Ŀ
	bool		GetIndexVertex(int32_t i, POINT* vertex);			//	ȡ�ö���εĵ�I����������
	bool		RemoveIndexVertex(int32_t i);						//	�Ӷ������ȥ����һ������
	void		LoopVertex(int32_t i);								//	�Ѷ���ζ���˳�����I��ѭ���仯
	void		Clear();										//	��ն������Ϣ
	void		GetCenterPos(POINT*	pos);						//	ȡ������λ��
	LPPolygon	GetPolygonPtr() {return &m_Polygon;};			//	ȡ�ö����ָ��
	bool		ShiftVertex(int32_t nDir, int32_t nDistance);			//	�����ⶥ����ʲô�����ƶ�
	int32_t			GetLeftVertex();								//	ȡ�����󶥵������
	int32_t			GetRightVertex();								//	ȡ�����Ҷ��������
};

#endif
