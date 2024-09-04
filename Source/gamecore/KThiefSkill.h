#pragma once
#include "KCore.h"
#include "Skill.h"
#include "KNpc.h"
#include "KPlayer.h"
#define THIEFSKILL_SETTINGFILE "\\SETTINGS\\THIEFSKILL.TXT"	
enum 
{
	thief_redress, //�ٸ���ƶ
	thief_showlife,
	thief_showmana,
	thief_showmoney,
	thief_showxu,
	thief_runscript,	//ʹ���ִ��ĳ���ű�
};

//----------------------------------------------------------------------
/*
							�ٸ���ƶ
  һ������ϵͳ��
  1��  �������ƣ��ٸ���ƶ��
  2��  �������ã���һ������͵����������������Ǯ�Ƶ�Y������͵��Ǯ��ϵͳ�Ե���
  3��  ���ܷ�Χ�������޷�ѧϰ��ʹ�ã����ܲ��������������������֮������໥���ã�����ս����ϵ���ƣ�
  4��  �������ģ�ʹ�ô˼������Ľ�ɫ������X�㣻
  5��  �������У��˼������������������뱻�����ȼ�������������ʣ�30��|Lv1��Lv2|��0.3��
  ��������ϵͳ��
  1��  ����ؤ�Ի���ѡ���Ƿ����ؤǮ��
  2��  ��ؤ��N���ĸ��ʽ̽�ɫ�˼��ܣ�NӦ��Ϊһ����С�ı�����
  ����ϵͳ��Ϣ��
  1��  ��͵�ɹ�����������������͵�������������Ӿ��ǰ�ߣ�
  2��  ��͵δ�죺��������ͼ͵���Ǯ��
  3��  ͵�Գɹ�����ɹ��Ĵӣ���������͵�������������Ӿ��ǰ�ߣ�
  4��  ͵��δ�죺��͵������ʧ�ܡ�
*/
//----------------------------------------------------------------------
class KThiefSkill :public ISkill
{
public:
	KThiefSkill();
    virtual ~KThiefSkill();
	int				GetSkillId();
	const char *	GetSkillName();
	int				GetSkillStyle();
	void			LoadSkillLevelData(uint32_t  ulLevel, int nParam);
	int				DoSkill(KNpc * pNpc, int nX, int nY);
	int				OnSkill(KNpc * pNpc);
	BOOL			CanCastSkill  (int nLauncher, int &nParam1, int &nParam2)  const ;
	int  			NewCanCastSkill  (int nLauncher, int nParam1, int nParam2);
	int          	GetDelayPerCast(){return m_ulDelayPerCast;};
	int				Cast(KPlayer * pLauncherPlayer, KPlayer * pTargetPlayer);
	NPCATTRIB		GetSkillCostType()const{return m_nSkillCostType;};
	int				GetSkillCost(void * pParam)const;
	int				GetAttackRadius() const{	return m_ulAttackRadius;};
	int				ChangeAttackRadius(int nDis) { m_ulAttackRadius +=nDis;if (m_ulAttackRadius<30) m_ulAttackRadius=30;return m_ulAttackRadius;};
	int				getBackAttackRadius(){return  m_ulAttackRadius;};
	BOOL			m_bTargetEnemy;
	BOOL			m_bTargetAlly;
	BOOL			m_bTargetObj;
	char			m_szSkillName[50];
	BOOL			m_bUseCostPercent;//ʹ�øü���ʱ������ֵ���ٷֱȼ���/���߾�����ֵ

	BOOL			IsTargetOnly()const{return TRUE;};
	BOOL			IsTargetEnemy()const{return TRUE;};
	BOOL			IsTargetAlly()const{return TRUE;};
	BOOL			IsTargetObj()const{return FALSE;};
	BOOL			IsTargetOther()const{return FALSE;};
	BOOL			IsTargetSelf()const{return FALSE;};
	BOOL			LoadSetting(char * szSettingFile);
	
	char			m_szSkillIcon[100];
	KRUImage		m_RUIconImage;
	char			m_szSkillDesc[300];//���ܵ�����
	char			m_szTargetMovie[200];
	char			m_szTargetMovieParam[20];
	char			m_szSkillSound[200];
	std::string		getIconPath(){return m_szSkillIcon;};
	void			DrawSkillIcon(int x, int y, int Width, int Height,int nParam=-1)  ;
	char			m_szSkillSpecialFile[200];
	void			GetDesc(uint32_t ulSkillId, uint32_t ulCurLevel, char * pszMsg, int nOwnerIndex,  bool bGetNextLevelDesc);
	uint32_t	m_ulAttackRadius;
	uint32_t	m_ulDelayPerCast;
	NPCATTRIB		m_nSkillCostType;
	uint32_t	m_nThiefPercent;
	int				m_nCost;
private:
	int				m_nThiefStyle;
	DWORD			m_dwParam1;
	DWORD			m_dwParam2;
};

