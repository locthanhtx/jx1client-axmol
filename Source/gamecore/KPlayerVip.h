//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerRoleVip.cpp
// Date:	2012.08.10
// Code:	SkyGold
// Desc:	PlayerRepute Class
//---------------------------------------------------------------------------

#ifndef KPLAYERVIP_H
#define KPLAYERVIP_H
//�ͻ���
class KPlayerVip
{
	friend class KPlayer;
private:
	int	   m_nRoleVipValue;			   //ת��ֵ  ����¼ת������,�Լ�ת���ĵȼ�,�����ļ��ܵ�,Ǳ�ܵ㣩
	int    m_nRoleVipLevel;		       //��ǰת���ĵȼ�
	int    m_nRoleVipKeepQpiont;       //������Ǳ�ܵ�
	int	   m_nRoleVipKeepJpiont;       //�����ļ���
	int    m_nRoleVipSetSkillMaxLevel; //���ܵȼ���������
	int    m_nRoleVipSetFanYuMaxVal;   //������������
	int    m_nRoleVipLifeMaxVal;       //������������
	int    m_nCurPlyaerLvel;
	int    m_nCurRoleVipNum;
	int    m_nRoleVipDamageMaxVal;
public:
	void	SetRoleVipValue(BYTE* pMsg);	  //int nValue
	void	AddRoleVipValue(int nAdd);	
	int		GetRoleVipValue()	{return m_nRoleVipValue;};
	int		GetRoleVipLevel()	{return m_nRoleVipLevel;};
	int		GetRoleVipKeepQpiont()	{return m_nRoleVipKeepQpiont;};
	int		GetRoleVipKeepJpiont()	{return m_nRoleVipKeepJpiont;};
	int		GetRoleVipSkillMaxLevel()	{return m_nRoleVipSetSkillMaxLevel;};
	int		GetRoleVipFanYuMaxVal()	{return m_nRoleVipSetFanYuMaxVal;};
	int		GetRoleVipLifeMaxVal()	{return m_nRoleVipLifeMaxVal;};
	int		GetRoleVipCurLevel()	{return m_nCurPlyaerLvel;};
	int		GetRoleVipNum()	{return m_nCurRoleVipNum;};
	int		GetRoleVipDamageMaxVal()	{return m_nRoleVipDamageMaxVal;};
	//int     SetRoleVipByte(int nIntValue,int nByteNumber,int nByteValue);
    //int     GetRoleVipByte(int nIntValue,int nByteNumber);

};
#endif
