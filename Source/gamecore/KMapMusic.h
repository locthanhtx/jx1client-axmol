//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KMapMusic.h
// Date:	2003-5-18 17:22
// Code:	�߳�����
// ˵������Ϸ��������֣�����߻������趨�����
//			sourcesafe�߻���\Setting\GAMESETTING\����ϵͳ�趨.doc
//---------------------------------------------------------------------------
#include "cocos2d.h"

#include "AudioEngine.h"
#include "GameDataDef.h"
#include "cocostudio/ComAudio.h"
#pragma once
USING_NS_AX;

//using namespace CocosDenshion;

#define		defONE_MAP_MAX_MUSIC			4				// ÿ�ŵ�ͼ������������
#define		defGAME_TIME_ONE_DAY			(1440 * 120)	// ��Ϸÿ��ʱ��ĳ���
#define		defMUSIC_MAX_STAGE				32
#define		defMUSIC_STATE_MAX_MAP			64
#define		defMUSIC_STATE_MAX_MUSIC		16

enum
{
	enumMUSIC_STATE_STOP = 0,
	enumMUSIC_STATE_AUTO,
	enumMUSIC_STATE_SCRIPT,
	enumMUSIC_STATE_NUM,
};

// ÿ�����ֵĲ���
struct	KMapMusicInfo
{
	int		m_nVolume;						// ����������С
	int		m_nStartTime;					// ��������Ϸʱ���ʲô�׶β��ţ���ʼʱ��
	int		m_nEndTime;						// ��������Ϸʱ���ʲô�׶β��ţ�����ʱ��
	char	m_szFileName[FILE_NAME_LENGTH];	// �����ļ���(������Ϸ��Ŀ¼��ʼ��������Ϸ·��)
};

// ÿ�ŵ�ͼ�����ֲ���
struct	KMapAllMusic
{
	int				m_nMapID;				        // ��ͼid
	KMapMusicInfo	m_sMusic[defONE_MAP_MAX_MUSIC];	// ����ͼ�������ֵĲ�������

	KMapAllMusic() {m_nMapID = 0; ZeroMemory(m_sMusic, sizeof(m_sMusic));};	// ���캯������ʼ��
};

struct	KMapFightMusic
{
	int		m_nMusicNum;
	int		m_nMapID[defMUSIC_STATE_MAX_MAP];
	char	m_szMusicName[defMUSIC_STATE_MAX_MUSIC][FILE_NAME_LENGTH];
};

// ��Ϸ���������
class KMapMusic
{
private:
	int				m_nState;			// ��ǰ״̬

	int				m_nCurMapID;		// ��ǰ��ͼid
	int				m_nCurInfoNo;		// ��ǰ��ͼid����Ϣ���ݵ�λ��
	int				m_nCurMusicNo;		// ��ǰ���ڲ��ű���ͼ�ڼ�������
	int				m_nGameVolume;		// ��ǰ��Ϸ�趨����������С(0(����) -- 100(����))
	int				m_nWeatherVolume;	// ��ǰ�������µ�����������С�ı仯(0(����) -- 100(����))
	char			m_szCurName[FILE_NAME_LENGTH];	// ��ǰ�����ļ���

	KMapAllMusic	*m_pMusicInfo;		// ���е�ͼ��������Ϣ
	int				m_nInfoSize;		// ���е�ͼ������Ϣ���ݵĴ�С(��λ:sizeof(KMapAllMusic))
	int				m_nLoadFlag;		// ��Ϸ������Ϣ�Ƿ��Ѿ�����

	KMapFightMusic	m_sFightMusic[defMUSIC_MAX_STAGE];

	int			m_bFightMode;
	int				m_nCurStage;

public:
	KMapMusic();
	~KMapMusic();
	void			LoadSetFile();		// �����ͼ������Ϣ
	void			Init();				// ��ʼ���������ͼ������Ϣ

	// ��Ϸ�������ֲ���
	// ������nMapID ��ͼid
    //		nGameTime ��Ϸʱ��(0 -- 1440)
	void			Play(int nMapID, int nGameTime, int bFightMode);
	void			Stop();

	void			Start(int nMapID, int nGameTime, int bFightMode);// ������Ϸ���������趨�򿪵�ʱ�򣬲�������
	void			SetGameVolume(int nVolume);	// �趨��Ϸ��������������С(0 -- 100)
	void			SetWeatherVolume(int nVolume);	// �趨����������������С��Ӱ��(0 -- 100)
	void			ScriptPlay(char *lpszMusicName);// �ýű���������
	void			downLoadAllMapmusic(char *pMusicName);          // ����ȫ����ͼ���ֵ�sd��
	bool 			getisDwon(){return nIsDown;};    // �Ƿ��Ѿ�����
private:
	void			Release();			// ���
	int             CreateFolder(char * nPath);
    cocostudio::ComAudio *__pMusic;
	std::string     nSdcardPath;
	bool            nIsDown;
};

