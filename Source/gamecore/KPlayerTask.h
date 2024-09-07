//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KPlayerTask.h
// Date:	2002.10.05
// Code:	�߳�����
// Desc:	PlayerTask Class
//---------------------------------------------------------------------------

#ifndef KPLAYERTASK_H
#define KPLAYERTASK_H

#define		MAX_TASK		4000   //���������
#define		MAX_TEMP_TASK	4000
//-------------�µ�----------------------------------------------
//#include <algorithm>
#include <map>
struct cTaskInfo
{
	char nTaskIconPath[128];
	int  nTaskidx;
	int  nTaskType;
	char nTaskName[32];
};
class KPlayerTask
{
public:
	//int			nSave[MAX_TASK];					// ���ڼ�¼�����Ƿ���ɣ��뱣�浽���ݿ�
	//int			nClear[MAX_TEMP_TASK];				// ���ڼ�¼��������е��м䲽����������������浽���ݿ⣬������ߺ���������

	typedef std::map<unsigned long, INT> _KTasklate,_KTaskTemplate;
	_KTasklate		m_Tasklate;
	_KTaskTemplate	m_TaskTemplate;

	cTaskInfo   nTaskInfo[20];                      // ���ڿͻ�������ϵͳ��¼
	int         nTaskGenre;
	char        nTaskDesc[512];
public:
	KPlayerTask();									// ���캯��
	void		Release();							// ���
	void		ClearTempVar();						// �����ʱ���̿��Ʊ���
	int			GetSaveVal(int nNo);				// �õ�����������
	void		SetSaveVal(int nNo, int bFlag);	    // �趨����������
	int			GetClearVal(int nNo);				// �õ���ʱ���̿��Ʊ���ֵ
	void		SetClearVal(int nNo, int nVal);		// �趨��ʱ���̿��Ʊ���ֵ
	//NewByte = SetByte(Value, ByteNo, ByteValue)
	//int         SetTaskByte(int nIntValue,int nByteNumber,int nByteValue);
	//ByteValue = GetByte(Value, ByteNo)
	//int         GetTaskByte(int nIntValue,int nByteNumber);
};

#endif
