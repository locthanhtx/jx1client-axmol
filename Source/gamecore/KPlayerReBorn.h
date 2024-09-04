//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerReBorn.cpp
// Date:	2012.08.10
// Code:	SkyGold
// Desc:	PlayerRepute Class
//---------------------------------------------------------------------------

#ifndef KPLAYERREBORN_H
#define KPLAYERREBORN_H

//�ͻ���
class KPlayerReBorn
{
	friend class KPlayer;
private:
	int	   m_nReBornValue;						//ת��ֵ  ����¼ת������,�Լ�ת���ĵȼ�,�����ļ��ܵ�,Ǳ�ܵ㣩
	int    m_nReBornLevel;		       //��ǰת���ĵȼ�
	int    m_nReBornKeepQpiont;       //������Ǳ�ܵ�
	int	   m_nReBornKeepJpiont;       //�����ļ���
	int    m_nReBornSetSkillMaxLevel; //���ܵȼ���������
	int    m_nReBornSetFanYuMaxVal;   //������������
	int    m_nReBornLifeMaxVal;       //������������
	int    m_nCurPlyaerLvel;
	int    m_nCurReBornNum;



public:
	void	SetReBornValue(BYTE* pMsg);	  //int nValue
	void	AddReBornValue(int nAdd);	
	int		GetReBornValue()	{return m_nReBornValue;};
	int		GetReBornLevel()	{return m_nReBornLevel;};
	int		GetReBornKeepQpiont()	{return m_nReBornKeepQpiont;};
	int		GetReBornKeepJpiont()	{return m_nReBornKeepJpiont;};
	int		GetReBornSkillMaxLevel()	{return m_nReBornSetSkillMaxLevel;};
	int		GetReBornFanYuMaxVal()	{return m_nReBornSetFanYuMaxVal;};
	int		GetReBornLifeMaxVal()	{return m_nReBornLifeMaxVal;};
	int		GetReBornCurLevel()	{return m_nCurPlyaerLvel;};
	int		GetReBornNum()	{return m_nCurReBornNum;};
	//int     SetReBornByte(int nIntValue,int nByteNumber,int nByteValue);
	//int     GetReBornByte(int nIntValue,int nByteNumber);

};
#endif
