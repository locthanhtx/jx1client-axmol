
#pragma once

//#include "KWavSound.h"
#include "engine/KList.h"
#include "KNpcResNode.h"
#include "KNpcResList.h"
#include "KSprControl.h"
#include "KRUImage.h"
//#include "../../Represent/iRepresent/KRepresentUnit.h"

class KCacheNode;

class KStateSpr
{

public:
	int				m_nID;						// �ڱ����ļ��е�λ�ã��� 1 ��ʼ��0Ϊ�գ�
	int				m_nType;					// ���ͣ�ͷ�����ŵס�����
	int				m_nPlayType;				// ��������
	int				m_nBackStart;				// �������� ����ʼ֡
	int				m_nBackEnd;					// �������� �������֡
	KSprControl		m_SprContrul;				// spr ����
public:
	KStateSpr();
	void			Release();
};

#define		MAX_BLUR_FRAME		7
#define		START_BLUR_ALPHA	128
#define		BLUR_ALPHA_CHANGE	16

class KNpcBlur
{
public:
	int				m_nActive;							// ��ǰ��Ӱ�����Ƿ��ڼ���״̬
	int				m_nCurNo;							// ��ǰָ֡��
	unsigned long			m_dwTimer;							// ʱ�������
	unsigned long			m_dwInterval;						// ����֡ȡһ�β�Ӱ
	int				m_nMapXpos[MAX_BLUR_FRAME];			// ��Ӧ�ĵ�ͼ���� x
	int				m_nMapYpos[MAX_BLUR_FRAME];			// ��Ӧ�ĵ�ͼ���� y
	int				m_nMapZpos[MAX_BLUR_FRAME];			// ��Ӧ�ĵ�ͼ���� z
	unsigned int	m_SceneIDNpcIdx[MAX_BLUR_FRAME];
	unsigned int	m_SceneID[MAX_BLUR_FRAME];			//
	KRUImage		m_Blur[MAX_BLUR_FRAME][MAX_PART];	// ��Ӱ�����б�
public:
	KNpcBlur();
	~KNpcBlur();
	int			Init();
	void			Remove();
	void			SetNextNo();
	void			SetMapPos(int x, int y, int z, int nNpcIdx);
	void			ChangeAlpha();
	void			ClearCurNo();
	void			SetFile(int nNo, char *lpszFileName, int nSprID, int nFrameNo, int nXpos, int nYpos, int nZpos);
	void			Draw(int nIdx);
	int			NowGetBlur();
	void			AddObj();
	void			RemoveObj();
};

class KNpcRes
{
	enum
	{
		SHADOW_BEGIN	= 0,
		STATE_BEGIN		= 1,
		PART_BEGIN		= 1 + 6,
		SPEC_BEGIN		= 1 + MAX_PART + 6,
		MENUSTATE_BEGIN = 1 + MAX_PART + 6 + 1,
	};
private:
	int				m_nDoing;							// Npc�Ķ���
	int				m_nAction;							// Npc��ʵ�ʶ������������������йأ�
	int				m_nNpcKind;							// ���� ��ͨ
	int				m_nXpos;							// ���� x
	int				m_nYpos;							// ���� y
	int				m_nZpos;							// ���� z
	unsigned int 	m_SceneID_NPCIdx;                   // �ڳ����е�ID ��Ӧ��NPCidx
	unsigned int	m_SceneID;							// �ڳ����е�ID
	int				m_nHelmType;						// ��ǰͷ������
	int				m_nArmorType;						// ��ǰ��������
	int				m_nWeaponType;						// ��ǰ��������
	int				m_nHorseType;						// ��ǰ��ƥ����
	int             m_nPifengType;                      //��������
	int             m_nChiBangType;                      //�������
	int			m_bRideHorse;						// ��ǰ�Ƿ�����
	int				m_nBlurState;
	char			m_szSoundName[80];					// ��ǰ��Ч�ļ���
//	KCacheNode		*m_pSoundNode;						// ��Чָ��
//	KWavSound		*m_pWave;							// ��Чwavָ��
	int				m_nHeightOff;
//	int             m_ChiBangCurFrameNo;
//	int             m_TempDir;
public:
	enum
	{
		    adjustcolor_physics = 0,	// �����˺�
			adjustcolor_poison,         // �����˺�
			adjustcolor_freeze,			// �����˺�
			adjustcolor_burn,			// �����˺�
			adjustcolor_confuse,		// �����˺�
			adjustcolor_stun,			// ѣ���˺���ɫ
			adjustcolor_red=16,			// ��ɫ
	};
	KSprControl		m_cNpcImage[MAX_PART];				 // ���ж���������spr�ļ���
	KSprControl		m_cNpcShadow;					     // npc��Ӱ
	KStateSpr		m_cStateSpr[36];                    //��Ч��0 1 2 3 Ϊͷ�� 4 5 6 7 Ϊ�ŵ�  8 9 10 11Ϊ����
	//KStateSpr		m_ClientStateSpr[20];               //�ͻ������ͬ��������
	//KRUImage		m_ClientDrawFile[20];               //�ͻ���״̬�ṹ
	//KSprControl	m_cSpecialSpr;						// �����ֻ����һ�������spr�ļ�
	//KSprControl	m_cFrameSpr;
	unsigned int	m_ulAdjustColorId;   //״̬��ɫƫ�Ʊ�ID
	KSprControl  	m_cMenuStateSpr;
	int				m_nMenuState;
	int				m_nBackMenuState;
	int				m_nSleepState;
	//char			m_szSentence[MAX_SENTENCE_LENGTH];
	//char			m_szBackSentence[MAX_SENTENCE_LENGTH];
	int				m_nSortTable[MAX_PART];				// �����
	KRUImage		m_cDrawFile[MAX_NPC_IMAGE_NUM];     // �����б� ���岿�� + ��Ӱ + ħ��״̬ + ���⶯�� + ͷ��״̬
	//KRUImage		m_cStatrDrawFile[MAX_NPC_IMAGE_NUM];     // �����б� ���岿�� + ��Ӱ + ħ��״̬ + ���⶯�� + ͷ��״̬
	//KNpcBlur		m_cNpcBlur;							// npc ��Ӱ
	KNpcResNode		*m_pcResNode;      // npc ��Դ
	//KRUImage	    m_cDrawSprFile[3];
	KStateSpr		m_cSprStateSpr[3];
	//ClientSprInfo    uSprInfo[20];

private:
	// ��һ��ͼ����Դ�ļ����õ�������Ӱͼ���ļ���
	void			GetSoundName();						// ��õ�ǰ��������Ч�ļ���
	void			PlaySound(int nX, int nY);			// ���ŵ�ǰ��������Ч
	void			SetMenuStateSpr(int nMenuState);					// set menu state spr
public:
	KNpcRes();
	~KNpcRes();
	int			Init(char *lpszNpcName, KNpcResList *pNpcResList,unsigned int nNpcDwIdx);	// ��ʼ��
	void			Remove(int nNpcIdx);								// ���
	void			Draw(int nNpcIdx, int nDir, int nAllFrame, int nCurFrame, int bInMenu = FALSE);		// ����
	void			DrawBorder();
	int				DrawMenuState(int nHeightOffset,int nNpcIndex);
	int			SetHelm(int nHelmType);								// �趨ͷ������
	int			SetArmor(int nArmorType);							// �趨��������
	int			SetWeapon(int nWeaponType);							// �趨��������
	int			SetPifeng(int nPifengType);
	int            SetChiBang(int nChiBangType);                       //�������
	int			SetHorse(int nHorseType);							// �趨��ƥ����
	int			SetAction(int nDoing);								// �趨��������
    int	            GetChiBang(){return m_nChiBangType;};
	int	            GetPifeng(){return m_nPifengType;};                 //��ȡ��������
	int				GetHelm(){return m_nHelmType;};
	int				GetArmor(){return m_nArmorType;	};
	int				GetWeapon(){return m_nWeaponType;};
	int			SetRideHorse(int bRideHorse);						// �趨�Ƿ�����

	void			SetPos(int nNpcIdx, int x, int y, int z = 0, int bFocus = FALSE, int bMenu = FALSE);// �趨 npc λ��

	void			SetSprState(int nSprIdx,KNpcResList *pNpcResList);
	void			SetState(KList *pNpcStateList, KNpcResList *pNpcResList);	// �趨״̬��Ч
//	void	        SetClientState(KList *pNpcStateList, KNpcResList *pNpcResList);
	void			SetClientSpr(char *nSprPath,int nxLeft,int nyTop,int nzPos,int i,char *ncPart,int nSprIdx);
	void			SetSpecialSpr(char *lpszSprName);					// �趨�����ֻ����һ�������spr�ļ�
	void			SetFrameSpr(char *lpszSprName, int nX, int nY, int nHeight,int mInterval=100); //���ö�̬ѭ��SPR
	void			SetBlur(int bBlur);								// ��Ӱ�򿪹ر�
	void			SetAdjustColorId(unsigned long ulColorId){m_ulAdjustColorId = ulColorId;};			// ����ƫɫ��������Ϊ0��ʾ��ƫɫ.
	int				GetAction(){return m_nAction;};
	void			SetMenuState(int nState, char *lpszSentence = NULL, int nSentenceLength = 0);	// �趨ͷ��״̬
	int				GetMenuState();						// ���ͷ��״̬
	void			SetSleepState(int bFlag);			// �趨˯��״̬
	int			GetSleepState();						// ���˯��״̬
	void			StopSound();
	int				GetSndVolume(int nVol);
	static void		GetShadowName(char *lpszShadow, char *lpszSprName);
	int				GetNormalNpcStandDir(int nFrame);	// ����֡��ת�����߼�����(0 - 63)
};
