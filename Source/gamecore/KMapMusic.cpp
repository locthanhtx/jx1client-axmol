//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KMapMusic.cpp
// Date:	2003-5-18 17:22
// Code:	�߳�����
//---------------------------------------------------------------------------
#include "KCore.h"
#include "KMapMusic.h"
#include "engine/KPakList.h"
#ifdef WIN32
#include <direct.h>
#else
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

#define	GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS		\
	(-10000 + (m_nGameVolume * m_pMusicInfo[m_nCurInfoNo].m_sMusic[m_nCurMusicNo].m_nVolume * m_nWeatherVolume) / 100)

//---------------------------------------------------------------------------
//	���ܣ����캯��
//---------------------------------------------------------------------------
KMapMusic::KMapMusic()
{
	__pMusic            = NULL;
	nSdcardPath         ="";
	nIsDown             =false;
	m_nState			= enumMUSIC_STATE_STOP;

	m_nCurMapID			= -1;
	m_nCurInfoNo		= 0;
	m_nCurMusicNo		= 0;
	m_nGameVolume		= 100;
	m_nWeatherVolume	= 100;
	m_szCurName[0]		= 0;

	m_pMusicInfo		= NULL;
	m_nInfoSize			= 0;
	m_nLoadFlag			= FALSE;

	m_bFightMode		= FALSE;
	m_nCurStage			= -1;

	memset(m_sFightMusic, 0, sizeof(m_sFightMusic));
	int		i, j;
	for (i = 0; i < defMUSIC_MAX_STAGE; i++)
	{
		for (j = 0; j < defMUSIC_STATE_MAX_MAP; j++)
			this->m_sFightMusic[i].m_nMapID[j] = -1;
	}
}

//---------------------------------------------------------------------------
//	���ܣ���������
//---------------------------------------------------------------------------
KMapMusic::~KMapMusic()
{
	Release();
}

//---------------------------------------------------------------------------
//	���ܣ����
//---------------------------------------------------------------------------
void	KMapMusic::Release()
{
	m_nState			= enumMUSIC_STATE_STOP;

	m_nCurMapID			= -1;
	m_nCurMusicNo		= 0;
	m_nGameVolume		= 100;
	m_nWeatherVolume	= 100;
	m_szCurName[0]		= 0;

	if (m_pMusicInfo)
	{
		delete []m_pMusicInfo;
		m_pMusicInfo = NULL;
	}
	m_nInfoSize = 0;
	m_nLoadFlag = FALSE;
	__pMusic = NULL;
	nSdcardPath ="";
	memset(m_sFightMusic, 0, sizeof(m_sFightMusic));
}

//---------------------------------------------------------------------------
//	���ܣ������ͼ������Ϣ
//---------------------------------------------------------------------------
void	KMapMusic::LoadSetFile()
{
	KTabFile	cFile;
	int			nHeight, i, j;

	Release();
	nSdcardPath = ax::FileUtils::getInstance()->getWritablePath();
//	g_SetFilePath("\\");
	if (!cFile.Load(defMUSIC_SET_FILE) )
		return;
	nHeight = cFile.GetHeight();
	if (nHeight <= 1)
		return;
	m_nInfoSize = nHeight - 1;
	m_pMusicInfo = (KMapAllMusic*)new KMapAllMusic[m_nInfoSize];
	for (i = 0; i < m_nInfoSize; i++)
	{
		cFile.GetInteger(i + 2, 1, 0, &m_pMusicInfo[i].m_nMapID);
		for (j = 0; j < defONE_MAP_MAX_MUSIC; j++)
		{
			cFile.GetString(i + 2, 2 + j * 5, "", m_pMusicInfo[i].m_sMusic[j].m_szFileName, FILE_NAME_LENGTH);
			cFile.GetInteger(i + 2, 2 + j * 5 + 1, 100, &m_pMusicInfo[i].m_sMusic[j].m_nVolume);
			cFile.GetInteger(i + 2, 2 + j * 5 + 2, 0, &m_pMusicInfo[i].m_sMusic[j].m_nStartTime);
			cFile.GetInteger(i + 2, 2 + j * 5 + 3, defGAME_TIME_ONE_DAY - 1, &m_pMusicInfo[i].m_sMusic[j].m_nEndTime);
			if (m_pMusicInfo[i].m_sMusic[j].m_szFileName[0])
				downLoadAllMapmusic(m_pMusicInfo[i].m_sMusic[j].m_szFileName);
		}
	}
	cFile.Clear();
	int			nStage, nMapNum, nMusicNum;
	char		szSection[32], szKey[32];
	KIniFile	cFightFile;
//	g_SetFilePath("\\");
	if ( !cFightFile.Load(defMUSIC_FIGHT_SET_FILE))
		return;
	cFightFile.GetInteger("Stage", "StageNum", 0, &nStage);
	if (nStage > defMUSIC_MAX_STAGE)
		nStage = defMUSIC_MAX_STAGE;
	for (i = 0; i < nStage; i++)
	{
		sprintf(szSection, "Stage%d", i);

		cFightFile.GetInteger(szSection, "MapNum", 0, &nMapNum);
		if (nMapNum > defMUSIC_STATE_MAX_MAP)
			nMapNum = defMUSIC_STATE_MAX_MAP;
		for (j = 0; j < nMapNum; j++)
		{
			sprintf(szKey, "Map%03d", j);
			cFightFile.GetInteger(szSection, szKey, 0, &m_sFightMusic[i].m_nMapID[j]);
		}

		cFightFile.GetInteger(szSection, "MusicNum", 0, &nMusicNum);
		if (nMusicNum > defMUSIC_STATE_MAX_MUSIC)
			nMusicNum = defMUSIC_STATE_MAX_MUSIC;
		m_sFightMusic[i].m_nMusicNum = nMusicNum;
		for (j = 0; j < nMusicNum; j++)
		{
			sprintf(szKey, "Music%03d", j);
			cFightFile.GetString(szSection, szKey, NULL, m_sFightMusic[i].m_szMusicName[j], FILE_NAME_LENGTH);
			if (!m_sFightMusic[i].m_szMusicName[j][0])
			{
				m_sFightMusic[i].m_nMusicNum = j;
				break;
			}
			downLoadAllMapmusic(m_sFightMusic[i].m_szMusicName[j]);
		}
	}
	cFightFile.Clear();
	m_nLoadFlag = TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ���ʼ���������ͼ������Ϣ
//---------------------------------------------------------------------------
void	KMapMusic::Init()
{
	LoadSetFile();
	SetGameVolume(100);
	SetWeatherVolume(100);
}

//---------------------------------------------------------------------------
//	���ܣ���Ϸ�������ֲ���
//	������nMapID ��ͼid    nGameTime ��Ϸʱ��(0 -- 1440)
//---------------------------------------------------------------------------
void	KMapMusic::Play(int nMapID, int nGameTime, BOOL bFightMode)
{
	if (m_nState == enumMUSIC_STATE_STOP)
		return;

	if ( !m_pMusicInfo || m_nInfoSize <= 0)
		return;
	if (nMapID <= 0)
		return;
	if (!__pMusic)
		return;

	int		i, j;

	if (m_nState == enumMUSIC_STATE_SCRIPT)
	{//�ű����ŵ���Ч
		if (__pMusic->isBackgroundMusicPlaying())
		{
			__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
		}
		else
		{
			__pMusic->stopBackgroundMusic(true);
			m_nState = enumMUSIC_STATE_AUTO;
			Start(nMapID, nGameTime, bFightMode);
		}
		return;
	}

	// 	if (m_nState == enumMUSIC_STATE_AUTO)

	// ��ս��ģʽ
	if (!bFightMode)
	{
		nGameTime %= defGAME_TIME_ONE_DAY;
		// ������˵�ͼ�����ߴ�ս��ģʽ��ɷ�ս��ģʽ
		if (nMapID != m_nCurMapID || m_bFightMode)
		{
			m_nCurMapID = nMapID;
			m_bFightMode = bFightMode;
			// ���ҵ�ǰӦ�ò�����һ������
			for (i = 0; i < m_nInfoSize; i++)
			{
				if (this->m_pMusicInfo[i].m_nMapID == nMapID)
					break;
			}
			if (i >= m_nInfoSize)
				return;
			m_nCurInfoNo = i;
			m_nCurMusicNo = 0;
			for (i = 0; i < defONE_MAP_MAX_MUSIC; i++)
			{
				if (m_pMusicInfo[m_nCurInfoNo].m_sMusic[i].m_nStartTime <= nGameTime &&
					nGameTime <= m_pMusicInfo[m_nCurInfoNo].m_sMusic[i].m_nEndTime)
				{
					this->m_nCurMusicNo = i;
					break;
				}
			}

			// �ҵ��˵�ǰӦ�ò�����һ�����֣�����֮
			// �����ǰ���ڲ���Ӧ�ò��ŵ����֣�����
			if (__pMusic->isBackgroundMusicPlaying() &&
				strcmp(m_szCurName, m_pMusicInfo[m_nCurInfoNo].m_sMusic[m_nCurMusicNo].m_szFileName) == 0)
			{
				__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
				return;
			}
			// ����Ӧ�ò��ŵ�����
			strcpy(m_szCurName, m_pMusicInfo[m_nCurInfoNo].m_sMusic[m_nCurMusicNo].m_szFileName);
			if (!m_szCurName[0])
				return;
			__pMusic->stopBackgroundMusic(true);

			if (m_szCurName[0])
			{
				__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
				__pMusic->playBackgroundMusic(m_szCurName);
			}

			return;
		}

		// �������ţ�ѭ��֮
		if (m_pMusicInfo[m_nCurInfoNo].m_sMusic[m_nCurMusicNo].m_nStartTime <= nGameTime &&
			nGameTime <= m_pMusicInfo[m_nCurInfoNo].m_sMusic[m_nCurMusicNo].m_nEndTime)
		{
			if (__pMusic->isBackgroundMusicPlaying())
			{
				__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
			}
			else
			{
				__pMusic->stopBackgroundMusic(true);
				if (m_szCurName[0])
				{
					//messageBox(m_szCurName,"11111");
					__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
					__pMusic->playBackgroundMusic(m_szCurName);
				}
			}
			return;
		}

		// ʱ��������ˣ�������
		m_nCurMusicNo = 0;
		for (i = 0; i < defONE_MAP_MAX_MUSIC; i++)
		{
			if (m_pMusicInfo[m_nCurInfoNo].m_sMusic[i].m_nStartTime <= nGameTime &&
				nGameTime <= m_pMusicInfo[m_nCurInfoNo].m_sMusic[i].m_nEndTime)
			{
				this->m_nCurMusicNo = i;
				break;
			}
		}

		// ���ͬ������������
		if (strcmp(m_szCurName, m_pMusicInfo[m_nCurInfoNo].m_sMusic[m_nCurMusicNo].m_szFileName) == 0)
		{
			__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
			return;
		}

		// ��ͬ����������
		strcpy(m_szCurName, m_pMusicInfo[m_nCurInfoNo].m_sMusic[m_nCurMusicNo].m_szFileName);
		__pMusic->stopBackgroundMusic(true);
		if (m_szCurName[0])
		{
			//messageBox(m_szCurName,"22222");
			__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
			__pMusic->playBackgroundMusic(m_szCurName);
		}
	}
	// ս��ģʽ
	else
	{
		int		nFind;
		// ������˵�ͼ�����ߴӷ�ս��ģʽ���ս��ģʽ
		if (nMapID != m_nCurMapID || !m_bFightMode)
		{
			m_bFightMode = bFightMode;
			m_nCurMapID = nMapID;
			// ���ҵ�ǰ��ͼ���ĸ�����
			nFind = 0;
			for (i = 0; i < defMUSIC_MAX_STAGE; i++)
			{
				if (m_sFightMusic[i].m_nMapID[0] == -1)
					break;
				for (j = 0; j < defMUSIC_STATE_MAX_MAP; j++)
				{
					if (m_sFightMusic[i].m_nMapID[j] == -1)
						break;
					if (m_sFightMusic[i].m_nMapID[j] == nMapID)
					{
						nFind = 1;
						break;
					}
				}
				if (nFind)
					break;
			}
			// �ҵ��˵�ǰ��ͼ���ĸ����򣬴���֮
			if (nFind)
			{
				m_nCurStage = i;
				// �жϵ�ǰ���ŵ������Ƿ��Ǳ�����ģ�����ǣ����жϣ�����������һ������
				nFind = 0;
				for (i = 0; i < defMUSIC_STATE_MAX_MUSIC; i++)
				{
					if (m_sFightMusic[m_nCurStage].m_szMusicName[i][0] &&
						strcmp(m_sFightMusic[m_nCurStage].m_szMusicName[i], m_szCurName) == 0)
					{
						nFind = 1;
						m_nCurMusicNo = i;
						break;
					}
				}
				// ��ǰ���ŵ��������Ǳ�����ģ���������
				if (nFind)
					return;
				// ��ǰ���ŵ����ֲ��Ǳ�����ģ��жϣ�������һ������
				__pMusic->stopBackgroundMusic(true);
				m_nCurMusicNo = g_Random(m_sFightMusic[m_nCurStage].m_nMusicNum);
				strcpy(m_szCurName, m_sFightMusic[m_nCurStage].m_szMusicName[m_nCurMusicNo]);

				if (m_szCurName[0])
				{
					__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
					__pMusic->playBackgroundMusic(m_szCurName);
				}
			}
			return;
		}

		// ����������ڲ��ţ�ֻ��Ҫ�趨������С
		if (__pMusic->isBackgroundMusicPlaying())
		{
			__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
			return;
		}
		// ������ֲ��Ž�����������һ�����֣�����֮
		else
		{
			if (m_nCurStage < 0 || m_sFightMusic[m_nCurStage].m_nMusicNum <= 0)
				return;
			m_nCurMusicNo = g_Random(m_sFightMusic[m_nCurStage].m_nMusicNum);
			if (!m_sFightMusic[m_nCurStage].m_szMusicName[m_nCurMusicNo][0])
				return;
			strcpy(m_szCurName, m_sFightMusic[m_nCurStage].m_szMusicName[m_nCurMusicNo]);
			__pMusic->stopBackgroundMusic(true);
			if (m_szCurName[0])
			{
				__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
				__pMusic->playBackgroundMusic(m_szCurName);
			}
			return;
		}
	}
}

void	KMapMusic::Stop()
{
	if (__pMusic)
		__pMusic->stopBackgroundMusic(true);
	m_nState = enumMUSIC_STATE_STOP;
}

//---------------------------------------------------------------------------
//	���ܣ��趨��Ϸ��������������С(0(����) -- 100(����))
//---------------------------------------------------------------------------
void	KMapMusic::SetGameVolume(int nVolume)
{
	if (nVolume <= 0)
		nVolume = 0;

	if (nVolume > 100)
		nVolume = 100;

	m_nGameVolume = nVolume;

	if (__pMusic)
		__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
}

//---------------------------------------------------------------------------
//	���ܣ��趨����������������С��Ӱ��(0(����) -- 100(����))
//---------------------------------------------------------------------------
void	KMapMusic::SetWeatherVolume(int nVolume)
{
	if (nVolume < 0)
		m_nWeatherVolume = 0;
	if (nVolume > 100)
		m_nWeatherVolume = 100;
	m_nWeatherVolume = nVolume;
	if (__pMusic)
		__pMusic->setBackgroundMusicVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
}

//---------------------------------------------------------------------------
//	���ܣ�������Ϸ���������趨�򿪵�ʱ�򣬲�������
//---------------------------------------------------------------------------
void	KMapMusic::Start(int nMapID, int nGameTime, BOOL bFightMode)
{
	this->m_nCurMapID = -1;
	m_nState = enumMUSIC_STATE_AUTO;
	Play(nMapID, nGameTime, bFightMode);
}

void	KMapMusic::ScriptPlay(char *lpszMusicName)
{
	if (!lpszMusicName || !lpszMusicName[0])
		return;

	m_nState = enumMUSIC_STATE_SCRIPT;
	strcpy(m_szCurName, lpszMusicName);
	__pMusic->stopAllEffects();
	if (m_szCurName[0])
	{
		__pMusic->setEffectsVolume(GAMEVALUE_TO_HUNDREDTHS_OF_DECIBELS);
		__pMusic->playEffect(m_szCurName);
		__pMusic->unloadEffect(m_szCurName);
	}
}


//�����ļ���
int KMapMusic::CreateFolder(char * nPath)
{
	/* ��ʽ��·��������Ŀ¼ */
	/*for (int i = 0; i < (int)strlen(nPath); i++)
	{
		if (nPath[i] == '/')
		{
			nPath[i] = '\\';
		}
	}
	if(nPath[0]=='\\'||nPath[0]=='/')
	{
		memcpy(oPath,nPath+1,strlen(nPath)-1);
	}
	else
	{
		memcpy(oPath, nPath, strlen(nPath));
	}*/
	char tPath[255];
	if (nPath[0]=='/'|| nPath[0]=='\\')
	{
		nPath[0]='/';
		nPath++;
	}

	for (size_t i = 1; i < strlen(nPath); i++)
	{
		if (nPath[i] == '\\') nPath[i] = '/';
		if (nPath[i] == '/')
		{
			memcpy(tPath, nPath, i );
			tPath[i] = 0;
			char nTempDir[256];
			ZeroMemory(nTempDir,sizeof(nTempDir));
			sprintf(nTempDir,"%s%s",nSdcardPath.c_str(),tPath);
#ifdef WIN32 //����·��
			_mkdir(nTempDir);//tPath
#else
			//DIR *pDir = NULL;
			//pDir = opendir(nTempDir);
			//if (!pDir)
			mkdir(nTempDir,S_IRWXU|S_IRWXG|S_IRWXO);
			//else
			//closedir(pDir);
#endif
		}
	}

	return true;

}

void KMapMusic::downLoadAllMapmusic(char *pMusicName)
{
	if (!pMusicName || nSdcardPath=="" || !g_pPakList) return;
	//��ʼ��ѹPAK�����ļ�
	char m_szEntireMapFile[256];
	ZeroMemory(m_szEntireMapFile,sizeof(m_szEntireMapFile));
	XPackElemFileRef m_PackRef;
	ZeroMemory(&m_PackRef,sizeof(XPackElemFileRef));
	int nIsHave = 0;
	    //messageBox(pMusicName,nSdcardPath.c_str());
		nIsHave  = g_pPakList->pGetFilePath(pMusicName,m_PackRef);
	if (nIsHave)
	{//��������������
		//messageBox(pMusicName,"�ҵ��ļ�");
		char * sBuf=NULL;
		sBuf = (char *)malloc(m_PackRef.nSize+1);
		if (sBuf)
		{
			CreateFolder(pMusicName);
			g_pPakList->ElemFileRead(m_PackRef,sBuf,m_PackRef.nSize);
			//sprintf(m_szEntireMapFile,"%s%s",nSdcardPath.c_str(),pMusicName);
#ifdef WIN32
			sprintf(m_szEntireMapFile,"%s%s",nSdcardPath.c_str(),pMusicName);
#else
			sprintf(m_szEntireMapFile,"%s%s",nSdcardPath.c_str(),G2U(pMusicName).c_str());
#endif
			FILE *oFp = NULL;
			if((oFp=fopen(m_szEntireMapFile,"wb+"))==0)
			   return;

			fseek(oFp,0,0);
			fwrite(sBuf,m_PackRef.nSize,1,oFp);
			fclose(oFp);
			oFp = NULL;

			free(sBuf);
			sBuf = NULL;
		}
	}
	nIsDown = true;
}
