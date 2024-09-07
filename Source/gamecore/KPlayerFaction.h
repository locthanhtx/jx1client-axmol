//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerFaction.h
// Date:	2002.09.26
// Code:	�߳�����
// Desc:	PlayerFaction Class
//---------------------------------------------------------------------------

#ifndef KPLAYERFACTION_H
#define KPLAYERFACTION_H

#include	"KFaction.h"
#include	"KSkillList.h"

// Player �Դ����й����ɵ�ģ��
class KPlayerFaction
{
public:
	int		m_nCurFaction;						// ��ǰ�������� id �����Ϊ -1 ����ǰû����������
	int		m_nFirstAddFaction;					// ��һ�μ�������� id
	int		m_nAddTimes;						// ��ǰ�������ɵĴ���
//	struct	SSkillOpen
//	{
//		int		m_nID;							// ��ϵ�����У�ĳ������ id
//		int		m_nOpenLevel;					// ��ϵĳ�����ɼ��ܿ��ŵ��ڼ���
//	}		m_sSkillOpen[FACTIONS_PRR_SERIES];	// ��ϵĳ�����ɵļ��ܿ��Ŷ�
public:
	KPlayerFaction();							// ���캯��
	void	Release();
	void	SetSeries(int nSeries);				// ������ҵ���������ȷ�������������
	int	AddFaction(int nSeries, char *lpszFactionName);// ��������
	int	AddFaction(int nSeries, int nFactionID);// ��������
	void	LeaveFaction();						// �뿪��ǰ����
	int	OpenCurSkillLevel(int nLevel, KSkillList *pSkillList);// ���ŵ�ǰ����ĳ���ȼ��ļ���
	int		GetGurFactionCamp();				// ��õ�ǰ������Ӫ
	int		GetCurFactionNo();					// ��õ�ǰ����
	void	GetCurFactionName(char *lpszGetName);
};

/*
ע��ÿ�����һ�����������е�ĳ��ϵ����ľˮ��������ÿ��ϵ�����������ɣ�
���ֻ���ܼ������������ɡ�ÿ�����ɶ���һЩ���ܿɹ�ѧϰ����Щ�����Ƿ�
�ȼ��ģ�ֻ������ȼ��Դ���ҿ����ˣ�����Ҳ���ѧϰ����ȼ��ļ��ܡ���
�ȼ����ܿ�����ζ�űȴ˵ȼ��͵������ȼ�Ҳһ�����ţ�����ֻ���¼��ǰ��
�ŵ��ڼ����ȼ������ˡ���ҵ�һ�μ�������ɵĵȼ����ܿ����������Ժ��
������ɵĿ��������ǲ�һ���ģ����Ա����¼��ǰ�������ɵĴ�����
*/
#endif
