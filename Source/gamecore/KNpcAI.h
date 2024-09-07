#ifndef	KNpcAIH
#define	KNpcAIH
#include "KNpc.h"
#include "KMath.h"
#include "KItem.h"
//#include "KBasPropTbl.h"
//#include "KMagicAttrib.h"

enum AutoPickUp  //�ű�֪ͨ��ʾ�Ľ�������
{
	PICCK_NONE,	 //����
	PICCK_MONEY,
    PICCK_ITEM,
	PICCK_ITEM_MONEY,
	PICCK_TUPU,
	PICCK_TOOL,
};

class	KNpcAI
{
public:
	int				 m_nIndex;
	int			 m_bActivate;
private:
public:
	KNpcAI();
	~KNpcAI();
	void			 Activate(int nIndex);
	void			 Enable(){ m_bActivate = TRUE; }
	void			 Disable(){ m_bActivate = FALSE; }

    void			 FollowCharacter(int m_Index,int fanwei,int juli,int nActiveNum=3);
	void             mainSkillattack(int m_Index,int m_Kind=0);  //������ť
	void             mainPickItem(int m_Index);  //������ť
	void             Setscpos(int m_nIndex,int x=0,int y=0);
	void			 MoveYaBiao(int m_Index);
	//KNpcFindPath	 ai_PathFinder;
	void             setPickStateNone(){nPickState = PICCK_NONE;};

private:
	int          	 GetNearestCharacter(int nRelation,int indexid,int nType=0,int nDis=0);
	int             AiCheckDistance(int nYMpsX=0 ,int nYMpsY=0, int nDMpsX=0,int nDMpsY=0,int nDisten=0);
	void             ReMoveCommonAction(int nNpcIdx);


	void	         MoveJiaoBen(int m_Index);
	void             GotoWhere(int x, int y, int mode);

	void             SetCurpos(int m_nIndex,int x,int y);
	int             KeepActiveCharacter(int fanwei,int indexid,int xx,int yy) ;
	int             KeepActiveJiaoBen(int fanwei,int indexid,int xx,int yy);
	void			 FollowAttackCharacter(int i,int r,int l,int nActiveNum);
	void			 KeepAttackCharacter(int nEnemy, int nRange,int r);
	AutoPickUp       nPickState;
	int              AutoMovePickUp(int nIndex ,int nObjIdx,int xx,int yy, AutoPickUp nPickState);
	int				GetNearestNpc(int nRelation);
//	int          	GetNearestNpcA(int nRelation,int indexid);
	int             GetNearestObject(int indexid,void *pData=NULL,int *CurState=NULL);
	int				GetNpcNumber(int nRelation);
	void			ProcessPlayer();

	void			FollowPeople(int nIdx);
	void			FollowObject(int nIdx);

	void	        ProcessChargedart();    // Ѻ��AI
	void	        ProcessCompanion();     // ͬ����AI
	void	        ProcessLuXianAi();      // ָ��·��AI

	//void			FleeEnemy(int nEnemyIdx);  //�������
	void			ProcessAIType01();		// ��ͨ������1
	void			ProcessAIType02();		// ��ͨ������2
	void			ProcessAIType03();		// ��ͨ������3
	void			ProcessAIType04();		// ��ͨ������1
	void			ProcessAIType05();		// ��ͨ������2
	void			ProcessAIType06();		// ��ͨ������3

	void			TriggerObjectTrap();
	void			TriggerMapTrap();
	void			FollowAttack(int nIdx);
	void			FollowAttackCompanion(int nIdx,int juli);
    void            FollowChargedart(int nIdx);
    void            FollowCompanion(int nIdx);
	int			InEyeshot(int nIdx);
	int			InEyeshota(int nIdx,int r);
	void			CommonAction();
	void			CommonActionWithPlayer();
	void			CommonActionCompanion(int fanwei);
	int			KeepActiveRange();
	void			KeepAttackRange(int nEnemy, int nRange);
	void			Flee(int nIdx);  //�������
	friend class KNpc;
	// װ������NPC�˶�����ϵ��
	// flying add these on Jun.4.2003
	// ����װ����NPC�˶�������ڣ���Activate(int)�ڲ�����
	int				ProcessShowNpc();
	int  			ShowNpcType11();
	int				ShowNpcType12();
	int				ShowNpcType13();
	int				ShowNpcType14();
	int				ShowNpcType15();
	int				ShowNpcType16();
	int				ShowNpcType17();
	int             GetNpcMoveOffset(int nDir, int nDistance, int *pnX, int *pnY);

	// �ж��Ƿ񳬳���Χ
	//int KeepActiveShowRange();
	// 16/17 AiMode NPC�����ݶ���
	int				DoShowFlee(int nIdx);
	// �ж��Ƿ����֡�ڿ��Ը���NPC��ָ��
	int			CanShowNpc();
	// flying add the function to get nearest player.
	int			IsPlayerCome();
};

inline int KNpcAI::GetNpcMoveOffset(int nDir, int nDistance, int *pnX, int *pnY)
{
    //_ASSERT(pnX);
    //_ASSERT(pnY);
	if (!pnX && !pnY)
		return false;

	if (nDir >= MaxMissleDir)
		nDir -= MaxMissleDir;

    *pnX = -nDistance * g_DirSin(nDir, MaxMissleDir);
    *pnY = -nDistance * g_DirCos(nDir, MaxMissleDir);

    return true;
}

inline int KNpcAI::CanShowNpc()
{
	int bResult = TRUE;
	if (Npc[m_nIndex].m_AiParam[5] < Npc[m_nIndex].m_AiParam[4])
		bResult = FALSE;
	Npc[m_nIndex].m_AiParam[5]++;
	return bResult;
}

extern KNpcAI NpcAI;
#endif

