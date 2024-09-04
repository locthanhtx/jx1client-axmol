//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KObjSet.h
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------
#ifndef	KObjSetH
#define	KObjSetH

#include "KObj.h"
#include "engine/KLinkArray.h"

#define		MAX_OBJ_NAME_COLOR		16
/*
typedef struct
{
	char	m_szCopyRight[31];
	BYTE	m_btVersion;
	DWORD	m_dwNum;
} KObjMapDataHead;

typedef struct
{
	int		m_nTemplateID;
	int		m_nState;
	int		m_nDir;
	int		m_nPos[3];
	DWORD	m_dwScriptLength;
	char	m_szScript[80];
} KObjMapData;	*/

typedef struct
{
	int		m_nItemID;
	int		m_nMoneyNum;
	int		m_nItemWidth;
	int		m_nItemHeight;
	int		m_nColorID;
	int		m_nMovieFlag;
	int		m_nSoundFlag;
	char	m_szName[80];
	char	m_gsName[80];
	int     m_sHaveAttack;  //�Ƿ�����Ϊ������Ч��
	//DWORD   m_AttackerDwid;  //�ϴι����ŵ�DWID
} KObjItemInfo;

typedef struct
{
	int		m_nItemID;
	int		m_nMoneyNum;
	int		m_nItemWidth;
	int		m_nItemHeight;
	int		m_nColorID;
	int		m_nMovieFlag;
	int		m_nSoundFlag;
	char	m_szName[80];
	char	m_gsName[80];
	int     m_cHaveAttack;  //�Ƿ�����Ϊ������Ч��
	int     m_Genre;
	int     m_DetailType;
	int     m_ParticularType;
	int     m_GoldId;
	int     m_ItemLevel;
	int     m_StackNum;
	//DWORD   m_cAttackerDwid;  //�ϴι����ŵ�DWID
} KCObjItemInfo;

class KObjSet
{
public:
	KTabFile	m_cTabFile;
	KTabFile	m_cMoneyFile;
	int			m_nObjID;
	//unsigned int	m_dwNameColor[MAX_OBJ_NAME_COLOR];  // ��Ʒ������ɫ����
	ax::Color3B	m_dwNameColor[MAX_OBJ_NAME_COLOR];
	int			m_nShowNameFlag;		// �Ƿ�ȫ����ʾ item �� money ��� object ��������ͷ����

private:
	KLinkArray	m_UseIdxObjSet;
	KLinkArray	m_FreeIdxObjSet;
public:
	KObjSet();
	~KObjSet();
	BOOL	Init();
	int		GetID();
	void	Remove(int nIdx);
//	int		AddMoneyObj(KMapPos MapPos, int nMoneyNum);

	int		AddData(int nDataID, int nSubWorld, int nRegion, int nMapX, int nMapY, int nOffX, int nOffY);
	int		ClientAdd(int nID, int nDataID, int nState, int nDir, int nCurFrame, int nXpos, int nYpos, KCObjItemInfo sInfo,unsigned int nObjDwidx=0);
	BOOL	ClientLoadRegionObj(char *lpszMapPath, int nRegionX, int nRegionY, int nSubWorld, int nRegion);
	BOOL	ClientAddRegionObj(KPakFile *pFile, unsigned int dwDataSize);
	void	RemoveIfClientOnly(int nIdx);
	int		SearchObjAt(int nX, int nY, int nRange);
	ax::Color3B	GetNameColor(int nColorID);
	void	SetShowNameFlag(BOOL bFlag);
	BOOL	CheckShowName();

	int		FindID(int nID);
	int		FindName(char *lpszObjName);
private:
	int		FindFree();
	int		CheckClientKind(int nKind);
	int		GetDataIDKind(int nDataID);
};

extern	KObjSet	ObjSet;
extern	char	g_szObjKind[Obj_Kind_Num][32];

#endif

/*
�����������յ���Ҫ����һ���µ������Ҫ��������ڵذ��Ϸ���һ��װ����Ϊ��ȷ��
���װ������ҪһЩ��Ϣ��
    1��λ����Ϣ������������ı������Ϣ
    2���������ݵ���Ϣ��DataID������Щ��Ϣ��������������ơ����͡��ϰ���Ϣ��ͬһ
       ��DataID��ͨ����Ϣ����Щ��Ϣ����� ObjData.txt �ļ����棬ͨ����ȡ��Щ��
       Ϣ���Գ���ȷ����������
    3�������ÿ��ʵ��֮�䲻ͬ����Ϣ�����磺��ǰ��״̬����ǰ�ķ�����Щ���͵���
       ��������Ҫ��������Ϣ������ǰ������ʱ�䣨��Щ���͵����������ʱ�估��Ӧ
       �Ĵ�������ǰ�����ű��ļ����ơ���ǰ��װ������ID��װ���ࣩ��������������
       �������Ƶĸ�����Ϣ����ͼ������ȣ�����ǰ�ϰ���Ϣ����������д򿪡��ر�״
       ̬�ȣ�
��ȷ�����㹻����Ϣ�󣬿��Ե��þ���ķ����ڷ�����������һ���µ���������嵽ʹ��
�ϣ����ȵ��� AddData() �������ѵ� 1 ���� 2 ����Ϣ���룬����������Զ��� KObject
������Ѱ��һ�������������Ӧ�Ļ������ݣ�����һ���µ�����������������������
��λ��xxx��Ȼ����������һ������ΨһID��֪ͨ�ͻ��˲���һ����Ӧ���������Ȼ���
����֪ͨ��ӦSubWorld����ӦRegion��ʼ��������������
    SubWorld[nSubWorld].m_WorldMessage.Send(GWM_OBJ_ADD, nRegion, xxx);
Ȼ���趨�������һЩ������Ϣ�����趨�� 3 ����Ϣ��������Ҫ���õķ����У�
    KObject[xxx].SetLifeTime()
    KObject[xxx].SetState()
    KObject[xxx].SetDir()
    KObject[xxx].SetItemDataID()
    KObject[xxx].SetScriptFile() �ȣ�
���ˣ�һ���µ�����ڷ��������Ѿ��������ˣ�
���ͻ��˵����յ��������˵Ĳ��������������󣬵��� ClientAdd() �����ڿͻ��˲���
һ����Ӧ��������������������Ϊ�������Զ��� KObject ������Ѱ��һ�����������
��Ӧ�Ļ������ݣ�����һ���µ����������������������е�λ��xxx���ѽ��յ�������Ψ
һ ID �������ϣ�Ȼ���趨���ֽ��յ��Ĳ��������֪ͨ��ӦSubWorld����ӦRegion��ʼ
�����������������ˣ�������Ѿ����������ˣ��������ĸ��������û�������趨��ɣ�
��Ҫ�Ժ�������˷�������ͬ����Ϣ�������յõ���ȷ�ı��֡�
*/







