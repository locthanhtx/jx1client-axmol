#ifndef KPLAYERDEF_H
#define KPLAYERDEF_H

//���ݿ�����б�����ʱÿ֡��������
#define DBLOADPERTIME_ITEM		50
#define DBLOADPERTIME_SKILL		20
#define DBLOADPERTIME_FRIEND	20
#define	DBLOADPERTIME_TASK		20

#define SCRIPT_PLAYERID		"PlayerId"		//�ű��е����Idȫ�ֱ�����      dwid
#define SCRIPT_PLAYERINDEX	"PlayerIndex"	//�ű��е���ҵı��ȫ�ֱ�����	plaueridx
#define SCRIPT_OBJINDEX		"ObjIndex"		//�����ű��������INDEX
#define SCRIPT_SUBWORLDINDEX "SubWorld"		//�ű��е�ǰ�����Index��ȫ�ֱ�����
#define SCRIPT_SUBWORLDID	"SubWorldID"	//�ű��е�ǰ�����ID��ȫ�ֱ�����	

#define	MAX_PLAYER		2    //Index 0 reserved 500
#define	CLIENT_PLAYER_INDEX	1   //�ͻ��˱�ʾ

#define	MAX_TEAM_APPLY_LIST	7	// ÿ����������ǰ����������� 32

//---------------------------------------------------------------------------
/*#define	QUEST_BEFORE	0x0000
#define	QUEST_START		0x0001
#define	QUEST_FINISHED	0xffff*/
//---------------------------------------------------------------------------
enum MOUSE_BUTTON
{
	button_left  = 0,
	button_right,
};

enum SCRIPTACTION  
{
	SCRIPTACTION_UISHOW,			//��ʾĳ��UI����
	SCRIPTACTION_EXESCRIPT,			//ִ�нű�
};

enum
{
	ATTRIBUTE_STRENGTH = 0,
	ATTRIBUTE_DEXTERITY,
	ATTRIBUTE_VITALITY,
	ATTRIBUTE_ENGERGY,
};

enum	// ���鴴��ʧ��ԭ��
{
	Team_Create_Error_InTeam = 0,	// �Ѿ��ڶ�����
	Team_Create_Error_SameName,		// ͬ������
	Team_Create_Error_Name,			// �������ַǷ�
	Team_Create_Error_TeamFull,		// ��������̫�࣬���ɲ���
	Team_Create_Error_CannotCreate,	// ��ǰ���ڲ������״̬
};

enum TEAM_FIGURE
{
	TEAM_CAPTAIN,				// �����Ա��� �ӳ�
	TEAM_MEMBER,				// �����Ա��� ��Ա
};

enum SYNC_STEP
{
	STEP_BASE_INFO = 0,
	STEP_FIGHT_SKILL_LIST,
	STEP_LIFE_SKILL_LIST,
	STEP_TASK_LIST,
	STEP_ITEM_LIST,  //װ���嵥
	STEP_FRIEND_LIST,  //����
	STEP_SYNC_END
};

enum SAVE_STATUS
{
	SAVE_IDLE = 0,
	SAVE_REQUEST,	 //�����浵
	SAVE_DOING,	     //��ʱ��浵��
};
#endif
