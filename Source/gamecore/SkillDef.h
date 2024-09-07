#ifndef __SKILLDEF_H__
#define __SKILLDEF_H__

#define MAX_SKILL  2000                   //������������ 1000
#define MAX_SKILLLEVEL 64				  //����ܵȼ�
#define MaxMissleDir	64
#define MAXSKILLLEVELSETTINGNUM	20        //����д��������״̬ʱ�����������20  ÿ������������������
#define MAX_SKILLVEDATA_COUNT	20
#define MAX_MISSLESTYLE  1000             //ϵͳ���ӵ��������� txt
#define MISSLE_MIN_COLLISION_ZHEIGHT 0	  //�ӵ������ײ�ĸ߶ȡ�
#define MISSLE_MAX_COLLISION_ZHEIGHT 20   //�ӵ����ڸø߶�ʱ,��������ײ

//---------------------------------------------------------------------------
// MoveKind �˶�����
//---------------------------------------------------------------------------
enum eMissleMoveKind
{
	    MISSLE_MMK_Stand,							//	ԭ��
		MISSLE_MMK_Line,							//	ֱ�߷���
		MISSLE_MMK_Random,							//	������У����ڶ�Ů�׵�Charged Bolt��
		MISSLE_MMK_Circle,							//	���з��У�Χ�������ߣ����ڶ��̿͵ļ�����
		MISSLE_MMK_Helix,							//	�����׵������ߣ����ڶ�������Bless Hammer��
		MISSLE_MMK_Follow,							//	����Ŀ�����
		MISSLE_MMK_Motion,							//	��Ҷ�����
		MISSLE_MMK_Parabola,						//	������
		MISSLE_MMK_SingleLine,						//	���еĵ�һֱ�߷���ħ��
		MISSLE_MMK_RollBack = 100,					//  �ӵ����ط���
		MISSLE_MMK_Toss		,						//	������
};

//---------------------------------------------------------------------------
// FollowKind ��������	(��Ҫ�����ԭ�ء������������߷���������)
//---------------------------------------------------------------------------
enum eMissleFollowKind
{
	MISSLE_MFK_None,							//	�������κ����
	MISSLE_MFK_NPC,								//	����NPC�����
	MISSLE_MFK_Missle,							//	�����ӵ�
};

#define	MAX_MISSLE_STATUS 4                      //�����ӵ�״̬����

enum eMissleStatus
{
	MS_DoWait,  //0�ȴ�״̬
	MS_DoFly,   //1����״̬
	MS_DoVanish, //2����״̬
	MS_DoCollision,//3��ײ
};


enum eSkillLRInfo
{
	BothSkill,          //���Ҽ��Կ�0
	leftOnlySkill,		//���1
	RightOnlySkill,		//�Ҽ�2
	NoneSkill,			//������3
};

//--------------------------------------------------------Skill.h

//���ܷ����ߵ�����
/*enum eGameActorType
{
	Actor_Npc,
	Actor_Obj,
	Actor_Missle,
	Actor_Sound,
	Actor_None,
}; */
enum eSkillLauncherType
{
	SKILL_SLT_Npc = 0, //NPC��������Ч
	SKILL_SLT_Obj ,    //��Ʒ��������Ч
	SKILL_SLT_Missle,  //�ӵ���������Ч
};


/*#ifndef _SERVER

struct	TOrginSkill
{
	int		nNpcIndex;				//	Npc��index
	unsigned long	nSkillId;				//	���͵�skillid
};

#endif*/


enum eSkillParamType
{
	SKILL_SPT_TargetIndex	= -1,
	SKILL_SPT_Direction		= -2,
};

//���ܵ�����
enum eSKillStyle
{
	    SKILL_SS_Missles = 0,			//	�ӵ���0		���������ڷ����ӵ���
		SKILL_SS_Melee,                 //  �⹦ϵ1     Զ���ӵ��������ܣ�
		SKILL_SS_InitiativeNpcState,	//	������2		���������ڸı䵱ǰNpc������״̬
		SKILL_SS_PassivityNpcState,		//	������3		���������ڸı�Npc�ı���״̬
		SKILL_SS_CreateNpc,				//	����Npc��4	��������������һ���µ�Npc
		SKILL_SS_BuildPoison,			//	������5		��������������
		SKILL_SS_AddPoison,				//	�Ӷ���6		���������ڸ������Ӷ���
		SKILL_SS_GetObjDirectly,		//	ȡ����7		���������ڸ���ȡ��
		SKILL_SS_StrideObstacle ,		//	��Խ��8		���������ڿ�Խ�ϰ�
		SKILL_SS_BodyToObject,			//	������9		���������ڽ�ʬ���ɱ���
		SKILL_SS_Mining,				//	�ɿ���10		���������ڲɿ�������ɿ�ʯ
		SKILL_SS_RepairWeapon,			//	�޸���11		�����������޸�װ��
		SKILL_SS_Capture,				//	��׽��12		���������ڲ�׽����Npc
		SKILL_SS_Thief,				    //	͵����13
//		SKILL_SS_NewMissles,			//	���ӵ�14
};

//ͬʱ�����Ķ���ӵ��ķ�����ʼ��ʽ
enum eMisslesForm
{
	    SKILL_MF_Wall	= 0,			//0ǽ��	����ӵ��ʴ�ֱ�������У���ʽ��ǽ״
		SKILL_MF_Line,					//1����	����ӵ���ƽ������ҷ�������
		SKILL_MF_Spread,				//2ɢ��	����ӵ���һ���ĽǶȵķ�ɢ״
		SKILL_MF_Circle,				//3Բ��	����ӵ�Χ��һ��Ȧ
		SKILL_MF_Random,				//4���	����ӵ�����ŷ�
		SKILL_MF_Zone,					//5����	����ӵ�������ĳ����Χ��
		SKILL_MF_AtTarget,				//6����	����ӵ�����
		SKILL_MF_AtFirer,				//7����	����ӵ�ͣ����ҵ�ǰλ��
		SKILL_MF_COUNT,
};

enum eMeleeForm
{
	Melee_AttackWithBlur = SKILL_MF_COUNT,
	Melee_Jump,
	Melee_JumpAndAttack,
	Melee_RunAndAttack, //�ϻ��
	Melee_ManyAttack,
	Melee_Move,
};

/*
enum eSKillCostType
{
	SKILL_SCT_MANA		= 1,
		SKILL_SCT_LIFE		= 2,
		SKILL_SCT_STAMINA	= 8,
		SKILL_SCT_MONEY		= 16,
};
*/
enum eMisslesGenerateStyle
{
	    SKILL_MGS_NULL		= 0,
		SKILL_MGS_SAMETIME	,    //ͬʱ
		SKILL_MGS_ORDER		,	 //��˳��
		SKILL_MGS_RANDONORDER,
		SKILL_MGS_RANDONSAME,
		SKILL_MGS_CENTEREXTENDLINE,  //���м���������ɢ
};
/*
typedef struct
{
	int dx;
	int dy;
}TCollisionOffset;

typedef struct
{
	int nRegion;
	int nMapX;
	int nMapY;
}
TMisslePos;

typedef struct
{
	TCollisionOffset m_Offset [4];
}
TCollisionMatrix;

extern TCollisionMatrix g_CollisionMatrix[64];
 */

typedef struct
{
	int nLauncher;
	unsigned long dwLauncherID;
	eSkillLauncherType eLauncherType; //�����ߣ�һ��ΪNpc


	int nParent;
	eSkillLauncherType eParentType;	  //ĸ
	unsigned long dwParentID;

	int nParam1;
	int nParam2;
	int nWaitTime;
	int nTargetId;
	unsigned long dwTargetNpcID;
}
TOrdinSkillParam, * LPOrdinSkillParam;

#endif //__SKILLDEF_H__
