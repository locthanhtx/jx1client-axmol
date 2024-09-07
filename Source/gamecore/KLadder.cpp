#include "KCore.h"
#include "KPlayerFaction.h"
#include "KLadder.h"

KLadder	Ladder;

KLadder::KLadder()
{
	ZeroMemory(&GameStatData, sizeof(TGAME_STAT_DATA));
}

KLadder::~KLadder()
{
	ZeroMemory(&GameStatData, sizeof(TGAME_STAT_DATA));
}

int KLadder::Init(void* pData, size_t uSize)
{
	if (uSize != sizeof(TGAME_STAT_DATA))
		return FALSE;
//��ȡ�������ݰ�
	memcpy(&GameStatData, pData, uSize);
	return TRUE;
}
//������ǰ10�ȼ�����
const TRoleList* KLadder::TopTenFacMasterHand(int nFac)
{
	if (nFac < - 1 || nFac >= series_num * FACTIONS_PRR_SERIES)
		return NULL;
	return GameStatData.LevelStatBySect[nFac + 1];   //0��Ұ    1-10
}
//������ǰ10��Ǯ����
const TRoleList* KLadder::TopTenFacRich(int nFac)
{
	if (nFac < - 1 || nFac >= series_num * FACTIONS_PRR_SERIES)  //10�����ɵ�����
		return NULL;
	return GameStatData.MoneyStatBySect[nFac + 1];   //0Ϊ��Ұ	 1-10
}
//������ǰ10�������
const TRoleList* KLadder::TopTenFacXu(int nFac)
{
	if (nFac < - 1 || nFac >= series_num * FACTIONS_PRR_SERIES)  //10�����ɵ�����
		return NULL;
	return GameStatData.XuStatBySect[nFac + 1];
}
//����ǰ10ɱ������
const TRoleList* KLadder::TopTenKiller()
{
	return GameStatData.KillerStat;
}
//����ǰ10�ȼ�����
const TRoleList* KLadder::TopTenMasterHand()
{
	return GameStatData.LevelStat;
}
//����ǰ10��Ǯ����
const TRoleList* KLadder::TopTenRich()
{
	return GameStatData.MoneyStat;
}
//����ǰ10�������
const TRoleList* KLadder::TopTenXu()
{
	return GameStatData.XuStat;
}

//�����еȼ�������ռ����
int KLadder::GetFacMasterHandPercent(int nFac)
{
	if (nFac < - 1 || nFac >= series_num * FACTIONS_PRR_SERIES)
		return 0;
	return GameStatData.SectLevelMost[nFac + 1];
}
//�����н�Ǯ������ռ����
int KLadder::GetFacMoneyPercent(int nFac)
{
	if (nFac < - 1 || nFac >= series_num * FACTIONS_PRR_SERIES)
		return NULL;
	return GameStatData.SectMoneyMost[nFac + 1];
}
//�������ɵ�����
int KLadder::GetFacMemberCount(int nFac)
{
	if (nFac < - 1 || nFac >= series_num * FACTIONS_PRR_SERIES)
		return NULL;
	return GameStatData.SectPlayerNum[nFac + 1];
}

const TRoleList* KLadder::GetTopTen(unsigned long dwLadderID)  //ԭ���� 1-26��
{
	if (dwLadderID <= enumLadderBegin || dwLadderID >= enumLadderEnd)  //0----38
	{
		return NULL;
	}

	if (dwLadderID == enumTopTenMasterHand)
	{
		return GameStatData.LevelStat;  //����ȼ�����
	}
	else if (dwLadderID == enumTopTenRicher)
	{
		return GameStatData.MoneyStat;  //�����Ǯ����
	}
	else if (dwLadderID == enumTopTenKiller)
	{
		return GameStatData.KillerStat;  //����ɱ������
	}
	else if (dwLadderID == enumTopTenXu)
	{
		return GameStatData.XuStat;       //����������
	}
	else if (dwLadderID < enumFacTopTenRicher)   //5-15  ���ɵȼ�����  5 Ϊ��Ұʮ������
	{
		return TopTenFacMasterHand(dwLadderID - enumFacTopTenMasterHand - 1);  //������ǰ10�ȼ�����(0 -- 9)
	}
	else //���ڵ���15 ����������ǰ10��Ǯ����  15Ϊ��Ұʮ�󸻺�	 16-26
	{
		return TopTenFacRich(dwLadderID - enumFacTopTenRicher - 1);  //���ɽ�Ǯ����	 (0-9)
	}
}
