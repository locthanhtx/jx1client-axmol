//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerTask.cpp
// Date:	2002.10.05
// Code:	�߳�����
// Desc:	PlayerTask Class
//---------------------------------------------------------------------------

#include	"KCore.h"
#include	"KPlayerTask.h"

//---------------------------------------------------------------------------
//	���ܣ����캯��
//---------------------------------------------------------------------------
KPlayerTask::KPlayerTask()
{
	Release();
}

//---------------------------------------------------------------------------
//	���ܣ����
//---------------------------------------------------------------------------
void	KPlayerTask::Release()
{
	//memset(nSave, 0, sizeof(nSave));
	//memset(nClear, 0, sizeof(nClear));

	/*_KTaskTemplate::iterator it;
		for( it = m_TaskTemplate.begin(); it != m_TaskTemplate.end(); ++it )
		{
	         it->second = 0;
		}
	   */

       m_TaskTemplate.clear();
	/*_KTasklate::iterator its;
		for( its = m_Tasklate.begin(); its != m_Tasklate.end(); ++its )
		{
			its->second = 0;
		}
	*/	
       m_Tasklate.clear();
	
	ZeroMemory(nTaskInfo,sizeof(nTaskInfo));
}

//---------------------------------------------------------------------------
//	���ܣ������ʱ���̿��Ʊ���
//---------------------------------------------------------------------------
void	KPlayerTask::ClearTempVar()
{
	//memset(nClear, 0, sizeof(nClear));
	/*_KTaskTemplate::iterator it;
	for( it = m_TaskTemplate.begin(); it != m_TaskTemplate.end(); ++it )
	{
		it->second = 0;
	}*/
	
	m_TaskTemplate.clear();
}

//---------------------------------------------------------------------------
//	���ܣ��õ�����������
//---------------------------------------------------------------------------
int		KPlayerTask::GetSaveVal(int nNo)
{
	if (nNo < 0 || nNo >= MAX_TASK)
		return 0;
	if (m_Tasklate.count(nNo)>0)
		return m_Tasklate[nNo];
	else
		return 0;

	//return nSave[nNo];
}

//---------------------------------------------------------------------------
//	���ܣ��趨����������
//---------------------------------------------------------------------------
void	KPlayerTask::SetSaveVal(int nNo, int bFlag)
{
	if (nNo < 0 || nNo >= MAX_TASK)
		return;

	m_Tasklate[nNo] = bFlag;
	//nSave[nNo] = bFlag;  //ֵ
}






//---------------------------------------------------------------------------
//	���ܣ��õ���ʱ���̿��Ʊ���ֵ
//---------------------------------------------------------------------------
int		KPlayerTask::GetClearVal(int nNo)
{
	if (nNo < 0 || nNo >= MAX_TEMP_TASK)
		return 0;
	if (m_TaskTemplate.count(nNo)>0)
		return m_TaskTemplate[nNo];
	else
		return 0;
	//return nClear[nNo];
}

//---------------------------------------------------------------------------
//	���ܣ��趨��ʱ���̿��Ʊ���ֵ
//---------------------------------------------------------------------------
void	KPlayerTask::SetClearVal(int nNo, int nVal)
{
	if (nNo < 0 || nNo >= MAX_TEMP_TASK)
		return;

	m_TaskTemplate[nNo] = nVal; 
	//nClear[nNo] = nVal;
}

 /*
int  KPlayerTask::SetTaskByte(int nIntValue,int nByteNumber,int nByteValue)
{
	BYTE * pByte =	NULL;

	nByteValue = (nByteValue & 0xff);
	
	if (nByteNumber > 4 || nByteNumber <= 0) ///4
		return nIntValue;
	
	pByte = (BYTE*)&nIntValue;
	*(pByte + (nByteNumber -1)) = (BYTE)nByteValue;
	//nIntValue = (nIntValue | (0xff << ((nByteNumber - 1) * 8) )) ;
	//Lua_PushNumber(L, nIntValue);
	return nIntValue;
}

int  KPlayerTask::GetTaskByte(int nIntValue,int nByteNumber)
{
	int nByteValue = 0;

	if (nByteNumber > 4 || nByteNumber <= 0) 
		return nByteValue;

	nByteValue = (nIntValue & (0xff << ((nByteNumber - 1) * 8) )) >> ((nByteNumber - 1) * 8);
	//Lua_PushNumber(L, nByteValue);
	return nByteValue;
}
 */