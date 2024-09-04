#include "KCore.h"
#include "KNpc.h"    //ԭ��û�е�
#include "KNpcTemplate.h"

//#include "KDropScript.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#else
#include <io.h>
#include <direct.h>
#endif
//-------------�µ�----------------------------------------------
#include <algorithm>

#define MAX_VALUE_LEN 300

BOOL KNpcTemplate::InitNpcBaseData(int nNpcTemplateId)
{
	if (nNpcTemplateId < 0 )
		return FALSE;
	int nNpcTempRow = nNpcTemplateId + 2;

	g_NpcSetting.GetString(nNpcTempRow, "Name", "", Name, sizeof(Name));

	if (nGameVersion!=1)
	{//����汾����1�Ļ� ��ʼ�滻����
		if (strNpcInfo.count(Name))
		{//������ھ��滻��
			sprintf(Name, "%s", strNpcInfo[Name].c_str());
		}
		//sprintf(KObject[nFreeNo].m_szName,strObjInfo[nDataID + 1].c_str());
	}

	g_NpcSetting.GetInteger(nNpcTempRow, "Kind", 0, (int *)&m_Kind);
	g_NpcSetting.GetInteger(nNpcTempRow, "Camp", 0, &m_Camp);
	g_NpcSetting.GetInteger(nNpcTempRow, "Series", 0, &m_Series);

	g_NpcSetting.GetInteger(nNpcTempRow, "HeadImage",	0, &m_HeadImage);
	g_NpcSetting.GetInteger(nNpcTempRow, "ClientOnly",	0, &m_bClientOnly);
	g_NpcSetting.GetInteger(nNpcTempRow, "CorpseIdx",	0, &m_CorpseSettingIdx);  //ʬ����

	g_NpcSetting.GetInteger(nNpcTempRow, "DeathFrame",	12, &m_DeathFrame); //����֡��
	g_NpcSetting.GetInteger(nNpcTempRow, "WalkFrame",	15, &m_WalkFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "RunFrame",	15, &m_RunFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "HurtFrame",	10, &m_HurtFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "WalkSpeed",	5, &m_WalkSpeed);
	g_NpcSetting.GetInteger(nNpcTempRow, "AttackSpeed",	20, &m_AttackFrame); //�⹦�����ٶ�
	g_NpcSetting.GetInteger(nNpcTempRow, "CastSpeed",	20, &m_CastFrame);   //�ڹ������ٶ�
	g_NpcSetting.GetInteger(nNpcTempRow, "RunSpeed",	10, &m_RunSpeed);
	g_NpcSetting.GetInteger(nNpcTempRow, "StandFrame",	15, &m_StandFrame);
	g_NpcSetting.GetInteger(nNpcTempRow, "StandFrame1", 15, &m_StandFrame1);
	g_NpcSetting.GetInteger(nNpcTempRow, "Stature",		0,  &m_nStature);
	m_PifengType = 0;
	g_NpcSetting.GetInteger(nNpcTempRow, "ArmorType", 0, &m_ArmorType);
	g_NpcSetting.GetInteger(nNpcTempRow, "HelmType", 0, &m_HelmType);
	g_NpcSetting.GetInteger(nNpcTempRow, "WeaponType", 0, &m_WeaponType);
	g_NpcSetting.GetInteger(nNpcTempRow, "HorseType", -1, &m_HorseType);
	g_NpcSetting.GetInteger(nNpcTempRow, "RideHorse",0, &m_bRideHorse);
	g_NpcSetting.GetString(nNpcTempRow, "ActionScript", "", ActionScript,sizeof(ActionScript));
	g_NpcSetting.GetString(nNpcTempRow, "LevelScript", "", m_szLevelSettingScript, 100);
	return TRUE;

}
//�ӽű��л�ȡNPC����ԭʼ����
BOOL KNpcTemplate::InitNpcLevelData(int nNpcTemplateId, KLuaScript * pLevelScript, int nLevel)
{
	if (nNpcTemplateId < 0 || nLevel <= 0 || (!pLevelScript))
		return FALSE;
	int nNpcTempRow = nNpcTemplateId + 2;

	int	 nTopIndex = 0;

	m_nLevel = nLevel;

    nTopIndex=pLevelScript->SafeCallBegin();

	{
		m_NpcSettingIdx = nNpcTemplateId;
		double ncParam1, ncParam2,ncParam3, ncParam = 100;
		g_NpcSetting.GetDouble(nNpcTempRow, "LifeParam1", 0, &ncParam1);
		g_NpcSetting.GetDouble(nNpcTempRow, "LifeParam2", 0, &ncParam2);
		g_NpcSetting.GetDouble(nNpcTempRow, "LifeParam3", 0, &ncParam3);

		m_nLifeMax =(int) (GetNpcKeyDataFromScript(pLevelScript, "Life", nLevel, ncParam1, ncParam2, ncParam3,m_Series));

		if (m_nLifeMax <= 0) m_nLifeMax = 100;
	}
	nTopIndex=pLevelScript->SafeCallBegin();
	pLevelScript->SafeCallEnd(nTopIndex);

	return TRUE;
}
//��ȡ�ű� �ӽű���ȡ����ֵ
int KNpcTemplate::GetNpcLevelDataFromScript(KLuaScript * pScript, char * szDataName, int nLevel, char * szParam,int nSeries)
{
	int nTopIndex = 0;
	int nReturn = 0;
	if (pScript==NULL || szParam == NULL|| szParam[0] == 0 /*|| strlen(szParam) < 3*/)
	{
		return 0;
	}

	nTopIndex=pScript->SafeCallBegin();
	pScript->CallFunction("GetNpcLevelData",1, "ddss",nSeries,nLevel,szDataName,szParam);   // ��������
	nTopIndex = Lua_GetTopIndex(pScript->m_LuaState);
	nReturn = (int) Lua_ValueToNumber(pScript->m_LuaState, nTopIndex);  //��������
	nTopIndex=pScript->SafeCallBegin();
	pScript->SafeCallEnd(nTopIndex);
	return nReturn;
}
//��ȡ�ű�NPC ���� �ȼ� ����������
int KNpcTemplate::GetNpcKeyDataFromScript(KLuaScript * pScript, char * szDataName, int nLevel,double nParam1, double nParam2,double nParam3,int nSeries)
{
	int nTopIndex = 0;
	int nReturn = 0;
	if (pScript==NULL || nLevel<=0)
	{
		return 0;
	}
	nTopIndex=pScript->SafeCallBegin();
	pScript->CallFunction("GetNpcKeyData", 1, "ddsnnn",nSeries,nLevel,szDataName,nParam1,nParam2,nParam3);
	nTopIndex = Lua_GetTopIndex(pScript->m_LuaState);
	nReturn = (int)Lua_ValueToNumber(pScript->m_LuaState, nTopIndex);
	nTopIndex=pScript->SafeCallBegin();
	pScript->SafeCallEnd(nTopIndex);
	return nReturn;
}

//�������ļ����� �ڼ���SKILLS �ı��е�λ�� ��ȡ���ܵ�ID
int KNpcTemplate::SkillString2Id(char * szSkillString)
{
	if (!szSkillString[0]) return 0;
	int nSkillNum = g_OrdinSkillsSetting.GetHeight();
	char szSkillName[100];
	for (int i = 0 ;  i < nSkillNum; ++i)
	{
		g_OrdinSkillsSetting.GetString(i + 2, "SkillName", "", szSkillName, sizeof(szSkillName));
	//	if (g_StrCmp(szSkillString, szSkillName))  //������ȵ� ��ͬ��  ԭ����
		if (strcmp(szSkillString, szSkillName) == 0)
		{
			int nSkillId = 0;
			g_OrdinSkillsSetting.GetInteger(i + 2, "SkillId", 0, &nSkillId);
			return nSkillId;
		}
	}

	//printf("---�޷�����Npc.txt���õļ���[%s]ħ����Id,����!--- \n", szSkillString);
	return 0;
}



