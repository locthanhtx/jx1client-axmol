#ifndef	KDrawGameWorldH
#define	KDrawGameWorldH
#include "network/Thread.h"
#include "engine/KTimer.h"
/*
#ifndef _SERVER
#include "KMapMusic.h"
#else

#include "KSubWorld.h"

#endif
*/

class  KDrawGameWorld: public ax::Layer,protected CThread
{
public:
	using CThread::Start;
	void StartUp();
	void ClearUp();
	virtual void  Run();
	void  SetGameWorlNode(Node * ngameNode,int nKind);
	void  DrawPrimitives(int nNpcIndex,int nPos, KRUImage* pPrimitives, unsigned int uGenre, int bpartNo,int nOrDer);

	Node   *ParentNode_ui;
	Node   *ParentNode_map;//
	Node   *ParentNode_npc;
	Node   *ParentNode_player;
	Node   *ParentNode_miss;
	Node   *ParentNode_item;
private:

public:
	KDrawGameWorld();
	~KDrawGameWorld();
	//int	LoadSerMaps(char* szFileName);
};
extern  KDrawGameWorld g_DrawGameWorld;
#endif
