//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KObj.cpp
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------
#include "KCore.h"
#include "KNpc.h"
#include "KPlayer.h"
#include "KSubWorld.h"
#include "KItemSet.h"
#include "LuaFuns.h"
#include "KSortScript.h"
//#include "../../Represent/iRepresent/iRepresentshell.h"
//#include "scene/KScenePlaceC.h"
#include "ImgRef.h"
//#include "KOption.h"
#include "KObj.h"
#include "KObjSet.h"
#include "engine/Text.h"

#define		OBJ_SHOW_NAME_Y_OFF	30	 //48 20

KObj	*KObject = NULL;//KObject[MAX_OBJECT];

//-------------------------------------------------------------------------
//	���ܣ�	��ʼ��
//-------------------------------------------------------------------------
KObj::KObj()
{
	Release();
}

//-------------------------------------------------------------------------
//	���ܣ�	������
//-------------------------------------------------------------------------
void	KObj::Release()
{
	m_nID				= -1;
	m_nKind				= 0;
	m_nBelongRegion		= -1;

	m_nIndex			= 0;
	m_nSubWorldID		= 0;
	m_nRegionIdx		= -1;
	m_nMapX				= 0;
	m_nMapY				= 0;
	m_nOffX				= 0;
	m_nOffY				= 0;
	m_nDir				= 0;
	m_IsHaveAttack      = 0;  //�Ƿ�����Ϊ������Ч��
	m_AttackerDwid      = 0;  //�ϴι����ŵ�DWID
	m_AttackerTime      = 0;
	m_nLayer			= 1;
	m_nHeight			= 0;

	m_sObjLight.Release();
	//m_Image.uImage = 0;
	m_SceneID = 0;
	m_nState			= 0;
	m_nLifeTime			= 0;
	m_nBornTime			= 0;
	m_nWaitTime			= 0;
	m_cSkill.Release();
	m_nItemDataID		= 0;
	m_nItemWidth		= 0;
	m_nItemHeight		= 0;
	m_nMoneyNum			= 0;
    m_nKeDieNum         = 0;   //�ɵ�����
	m_szName[0]			= 0;
	_clientObjName[0]        = 0;
	m_dwScriptID		= 0;
	this->m_nColorID	= 0;
	m_szImageName[0]	= 0;
	m_szImageDropName[0]= 0;
	m_szSoundName[0]	= 0;
	m_nDropState		= 0;
	m_cImage.Release();
	m_cImageDrop.Release();
	//this->m_dwNameColor = 0x00ffffff;
	m_nameColor         = ax::Color3B::WHITE;
	m_dwTimer			= 0;
	memset(m_btBar, 0, sizeof(m_btBar));
	Polygon.Clear();
	//m_pSoundNode		= NULL;
	//m_pWave				= NULL;
}

/*
//-------------------------------------------------------------------------
//	���ܣ�	���������Ϣ�� ini �ļ�
//-------------------------------------------------------------------------
void KObj::Save(KIniFile *IniFile, LPSTR Section)
{
	IniFile->WriteString(Section, "ObjName", m_szName);

	IniFile->WriteInteger(Section, "DataID", m_nDataID);
	IniFile->WriteInteger(Section, "Kind", m_nKind);
	IniFile->WriteInteger(Section, "Dir", m_nDir);
	IniFile->WriteInteger(Section, "State", m_nState);
	IniFile->WriteInteger(Section, "LifeTime", m_nLifeTime);
	IniFile->WriteInteger(Section, "BornTime", m_nBornTime);
	IniFile->WriteInteger(Section, "WaitTime", m_nWaitTime);
	IniFile->WriteInteger(Section, "SkillKind", m_cSkill.m_nKind);
	IniFile->WriteInteger(Section, "SkillCamp", m_cSkill.m_nCamp);
	IniFile->WriteInteger(Section, "SkillRange", m_cSkill.m_nRange);
	IniFile->WriteInteger(Section, "SkillCastTime", m_cSkill.m_nCastTime);
	IniFile->WriteInteger(Section, "SkillID", m_cSkill.m_nID);
	IniFile->WriteInteger(Section, "SkillLevel", m_cSkill.m_nLevel);
	IniFile->WriteString(Section, "ScriptFile", m_szScriptName);

	IniFile->WriteInteger(Section, "ItemDataID", m_nItemDataID);
	IniFile->WriteInteger(Section, "ItemWidth", m_nItemWidth);
	IniFile->WriteInteger(Section, "ItemHeight", m_nItemHeight);
	IniFile->WriteInteger(Section, "Money", m_nMoneyNum);

#ifndef _SERVER
	IniFile->WriteInteger(Section, "Layer", m_nLayer);
	IniFile->WriteInteger(Section, "Height", m_nHeight);

	IniFile->WriteInteger(Section, "LightRadius", m_sObjLight.m_nRadius);
	IniFile->WriteInteger(Section, "Red", m_sObjLight.m_nRed);
	IniFile->WriteInteger(Section, "Green", m_sObjLight.m_nGreen);
	IniFile->WriteInteger(Section, "Blue", m_sObjLight.m_nBlue);
	IniFile->WriteInteger(Section, "Alpha", m_sObjLight.m_nAlpha);
	IniFile->WriteInteger(Section, "Reflect", m_sObjLight.m_nReflectType);
#endif

#ifdef _SERVER
	IniFile->WriteInteger(Section, "TotalFrmae", m_cImage.m_nTotalFrame);
	IniFile->WriteInteger(Section, "CurFrmae", m_cImage.m_nCurFrame);
	IniFile->WriteInteger(Section, "TotalDir", m_cImage.m_nTotalDir);
	IniFile->WriteInteger(Section, "CurDir", m_cImage.m_nCurDir);
	IniFile->WriteInteger(Section, "Interval", m_cImage.m_dwInterval);
#endif

#ifndef _SERVER
	IniFile->WriteString(Section, "ImageFile", m_szImageName);
	IniFile->WriteString(Section, "SoundFile", m_szSoundName);
	IniFile->WriteInteger(Section, "TotalFrmae", m_cImage.m_nTotalFrame);
	IniFile->WriteInteger(Section, "CurFrmae", m_cImage.m_nCurFrame);
	IniFile->WriteInteger(Section, "TotalDir", m_cImage.m_nTotalDir);
	IniFile->WriteInteger(Section, "CurDir", m_cImage.m_nCurDir);
	IniFile->WriteInteger(Section, "Interval", (int)m_cImage.m_dwInterval);
	IniFile->WriteInteger(Section, "CenterX", m_cImage.m_nCgXpos);
	IniFile->WriteInteger(Section, "CenterY", m_cImage.m_nCgYpos);
#endif

	int		x, y;
	SubWorld[m_nSubWorldID].Map2Mps(m_nRegionIdx, m_nMapX, m_nMapY, m_nOffX, m_nOffY, &x, &y);
	IniFile->WriteInteger(Section, "PosX", x);
	IniFile->WriteInteger(Section, "PosY", y);

	IniFile->WriteStruct(Section, "ObjBar", m_btBar, sizeof(m_btBar));
	IniFile->WriteStruct(Section, "Polygon", Polygon.GetPolygonPtr(), sizeof(TPolygon));
}

//-------------------------------------------------------------------------
//	���ܣ�	�� ini �ļ��ж�ȡ�����Ϣ
//-------------------------------------------------------------------------
void KObj::Load(int nObjIndex, int nSubWorldID, KIniFile *IniFile, LPSTR Section)
{
	IniFile->GetString(Section, "ObjName", "", m_szName, sizeof(m_szName));

	IniFile->GetInteger(Section, "DataID", 0, &m_nDataID);
	IniFile->GetInteger(Section, "Kind", Obj_Kind_MapObj, &m_nKind);
	IniFile->GetInteger(Section, "Dir", 0, &m_nDir);
	IniFile->GetInteger(Section, "State", 0, &m_nState);
	IniFile->GetInteger(Section, "LifeTime", 0, &m_nLifeTime);
	IniFile->GetInteger(Section, "BornTime", 0, &m_nBornTime);
	IniFile->GetInteger(Section, "WaitTime", 0, &m_nWaitTime);
	IniFile->GetInteger(Section, "SkillKind", 0, &m_cSkill.m_nKind);
	IniFile->GetInteger(Section, "SkillCamp", camp_animal, &m_cSkill.m_nCamp);
	IniFile->GetInteger(Section, "SkillRange", 0, &m_cSkill.m_nRange);
	IniFile->GetInteger(Section, "SkillCastTime", 0, &m_cSkill.m_nCastTime);
	IniFile->GetInteger(Section, "SkillID", 0, &m_cSkill.m_nID);
	IniFile->GetInteger(Section, "SkillLevel", 0, &m_cSkill.m_nLevel);
	IniFile->GetString(Section, "ScriptFile", "", m_szScriptName, sizeof(m_szScriptName));

	IniFile->GetInteger(Section, "ItemDataID", 0, &m_nItemDataID);
	IniFile->GetInteger(Section, "ItemWidth", 0, &m_nItemWidth);
	IniFile->GetInteger(Section, "ItemHeight", 0, &m_nItemHeight);
	IniFile->GetInteger(Section, "Money", 0, &m_nMoneyNum);

#ifndef _SERVER
	IniFile->GetInteger(Section, "Layer", 1, &m_nLayer);
	IniFile->GetInteger(Section, "Height", 0, &m_nHeight);

	IniFile->GetInteger(Section, "LightRadius", 0, &m_sObjLight.m_nRadius);
	IniFile->GetInteger(Section, "Red", 0, &m_sObjLight.m_nRed);
	IniFile->GetInteger(Section, "Green", 0, &m_sObjLight.m_nGreen);
	IniFile->GetInteger(Section, "Blue", 0, &m_sObjLight.m_nBlue);
	IniFile->GetInteger(Section, "Alpha", 0, &m_sObjLight.m_nAlpha);
	IniFile->GetInteger(Section, "Reflect", 0, &m_sObjLight.m_nReflectType);
#endif

#ifdef _SERVER
	int		nTotalFrame, nTotalDir, nInterval, nCurFrame, nCurDir;
	IniFile->GetInteger(Section, "TotalFrmae", 1, &nTotalFrame);
	IniFile->GetInteger(Section, "CurFrmae", 0, &nCurFrame);
	IniFile->GetInteger(Section, "TotalDir", 1, &nTotalDir);
	IniFile->GetInteger(Section, "CurDir", 0, &nCurDir);
	IniFile->GetInteger(Section, "Interval", 1, &nInterval);

	m_cImage.SetTotalFrame(nTotalFrame);
	m_cImage.SetTotalDir(nTotalDir);
	m_cImage.SetCurFrame(nCurFrame);
	m_cImage.SetCurDir(nCurDir);
	m_cImage.SetInterVal(nInterval);
#endif

#ifndef _SERVER
	int		nCgX, nCgY, nTotalFrame, nTotalDir, nInterval, nCurFrame, nCurDir;
	IniFile->GetString(Section, "ImageFile", "", m_szImageName, sizeof(m_szImageName));
	IniFile->GetString(Section, "SoundFile", "", m_szSoundName, sizeof(m_szSoundName));
	IniFile->GetInteger(Section, "TotalFrmae", 1, &nTotalFrame);
	IniFile->GetInteger(Section, "CurFrmae", 0, &nCurFrame);
	IniFile->GetInteger(Section, "TotalDir", 1, &nTotalDir);
	IniFile->GetInteger(Section, "CurDir", 0, &nCurDir);
	IniFile->GetInteger(Section, "Interval", 1, &nInterval);
	IniFile->GetInteger(Section, "CenterX", 0, &nCgX);
	IniFile->GetInteger(Section, "CenterY", 0, &nCgY);

	m_cImage.SetFileName(m_szImageName);
	m_cImage.SetTotalFrame(nTotalFrame);
	m_cImage.SetTotalDir(nTotalDir);
	m_cImage.SetCurFrame(nCurFrame);
	m_cImage.SetCurDir(nCurDir);
	m_cImage.SetInterVal((DWORD)nInterval);
	m_cImage.SetCenterPos(nCgX, nCgY);
#endif

	int		x, y;
	IniFile->GetInteger(Section, "PosX", 0, &x);
	IniFile->GetInteger(Section, "PosY", 0, &y);
	m_nSubWorldID = nSubWorldID;
	SubWorld[m_nSubWorldID].Mps2Map(x, y, &m_nRegionIdx, &m_nMapX, &m_nMapY,&m_nOffX, &m_nOffY);

	IniFile->GetStruct(Section, "ObjBar", m_btBar, sizeof(m_btBar));
	IniFile->GetStruct(Section, "Polygon", Polygon.GetPolygonPtr(), sizeof(TPolygon));

	SetIndex(nObjIndex);
	SetDir(m_nDir);
	SetState(m_nState);
}
*/

//-------------------------------------------------------------------------
//	���ܣ�	�趨���������ֵ
//-------------------------------------------------------------------------
void	KObj::SetIndex(int nIndex)
{
	if (nIndex >= 0)
		m_nIndex = nIndex;
	else
		m_nIndex = 0;
}

//-------------------------------------------------------------------------
//	���ܣ�	�趨�������Ψһ ID ��ע��ֻ�ڿͻ��˴��ڵ������ ID ͳһΪ 0��
//-------------------------------------------------------------------------
void	KObj::SetWorldID(int nID)
{
	if (nID < 0)
		m_nID = 0;
	else
		m_nID = nID;
}

//-------------------------------------------------------------------------
//	���ܣ�	�趨���������ʱ�� ��ע��ֻ��ʬ�塢װ�������������Ч��
//-------------------------------------------------------------------------
void	KObj::SetLifeTime(int nLifeTime)
{
	if (nLifeTime < 0)
	{
		m_nLifeTime=0;
		return;
	}
	m_nLifeTime = nLifeTime;
}

//-------------------------------------------------------------------------
//	���ܣ�	�趨���״̬
//-------------------------------------------------------------------------
void	KObj::SetState(int nState, int nPlaySoundFlag/* = 0*/)
{
	if (nState < 0)
		return;
	m_nState = nState;
	switch (m_nKind)
	{
	case Obj_Kind_Box:
		if (nState == OBJ_BOX_STATE_CLOSE)
			BoxClose();
		else if (nState == OBJ_BOX_STATE_OPEN)
			BoxOpen();
		break;
	case Obj_Kind_Door:
		if (nState == OBJ_DOOR_STATE_CLOSE)
			DoorClose();
		else if (nState == OBJ_DOOR_STATE_OPEN)
			DoorOpen();
		break;
	case Obj_Kind_Prop:
		if (nState == OBJ_PROP_STATE_HIDE)
		{
			m_nBornTime = m_nLifeTime;
		}
		break;
	}

/*#ifndef _SERVER
	if (nPlaySoundFlag)
		PlaySound();
#endif*/
}

//-------------------------------------------------------------------------
//	���ܣ�	�����
//-------------------------------------------------------------------------
void	KObj::BoxOpen()
{
/*#ifndef _SERVER
	if (m_nState != 1)
		PlaySound();
#endif*/
	m_nState = 1;
	m_nBornTime = m_nLifeTime;
}

//-------------------------------------------------------------------------
//	���ܣ�	����ر�
//-------------------------------------------------------------------------
void	KObj::BoxClose()
{
/*#ifndef _SERVER
	if (m_nState != 0)
		PlaySound();
#endif*/
	m_nState = 0;
	m_nBornTime = 0;
}

//-------------------------------------------------------------------------
//	���ܣ�	�Ŵ�
//-------------------------------------------------------------------------
void	KObj::DoorOpen()
{
/*#ifndef _SERVER
	if (m_nState != 1)
		PlaySound();
#endif*/
	m_nState = 1;

	// ȱ�ٴ�������ϰ�
}

//-------------------------------------------------------------------------
//	���ܣ�	�Źر�
//-------------------------------------------------------------------------
void	KObj::DoorClose()
{
/*#ifndef _SERVER
	if (m_nState != 0)
		PlaySound();
#endif*/
	m_nState = 0;

	// ȱ�ٴ�������ϰ�
}

BOOL	KObj::SetDir(int n64Dir)
{
	if (n64Dir < 0 || n64Dir >= 64)
	{
		n64Dir = 0;
		//_ASSERT(0);
		return FALSE;
	}

	if (m_nDir == n64Dir)
		return FALSE;

	m_nDir = n64Dir;
	m_cImage.SetCurDir64(n64Dir);
	m_cImageDrop.SetCurDir64(n64Dir);
	return TRUE;
}

void	KObj::SetScriptFile(char *lpszScriptFile)
{
	char	szScript[80];
	if (!lpszScriptFile || strlen(lpszScriptFile) >= sizeof(szScript))
	{
//		g_DebugLog("[error]Script FileName Error!!!");
	}
	else
	{
		if (lpszScriptFile[0])
		{
			if (lpszScriptFile[0] == '.')
				g_StrCpyLen(szScript, &lpszScriptFile[1], sizeof(szScript));
			else
				g_StrCpyLen(szScript, lpszScriptFile, sizeof(szScript));

			g_StrLower(szScript);

			if (strstr(szScript,".lua"))
			{//���������Ļ� ���ǽű� �����
				m_dwScriptID = g_CheckFileExist(szScript);
			}
			else
				m_dwScriptID=0;

		}
	}
}

void	KObj::SetImageDir(int nDir)
{
	if  (m_cImage.m_nTotalDir<=0)
	m_cImage.m_nTotalDir = 1;

	m_cImage.SetCurDir(nDir);
	SetDir(m_cImage.m_nCurDir * 64 / m_cImage.m_nTotalDir);
}

//�滭������Ʒ����
void KObj::DrawInfo()
{
	if (m_nKind != Obj_Kind_Item && m_nKind != Obj_Kind_Money)
		return;

	int		nMpsX, nMpsY, nHeightOff;
	//DWORD	dwColor;

	GetMpsPos(&nMpsX, &nMpsY);
	nHeightOff = OBJ_SHOW_NAME_Y_OFF;
	//dwColor = this->m_dwNameColor;  //������Ʒ���ֵ���ɫ
  //  int nMsgLen = TEncodeText(m_szName, strlen(m_szName));
//    g_pRepresent->OutputText(12,m_szName, KRF_ZERO_END, nMpsX - 12*g_StrLen(m_szName)/4, nMpsY, dwColor, 0, nHeightOff, 0XFF000000);
}

//-------------------------------------------------------------------------
//	���ܣ�	�������
//-------------------------------------------------------------------------
void KObj::Draw()
{
	if ( m_nIndex <= 0 )
		return;
	if (!m_cImage.CheckExist())
		return;
	if (m_bDrawFlag)
		return;
	if (m_nRegionIdx < 0 || m_nRegionIdx >= 9)
		return;
	if (!g_GameWorld||g_GameWorld==NULL)
	    return;
	int	x, y;
	SubWorld[m_nSubWorldID].NewMap2Mps(m_nRegionIdx, m_nMapX, m_nMapY, m_nOffX, m_nOffY, &x, &y);
//	SubWorld[m_nSubWorldID].Mps2Screen(&x, &y);
	m_Image.Color.Color_b.a = 255;
	m_Image.bRenderFlag  = RUIMAGE_RENDER_FLAG_REF_SPOT;
	m_Image.bRenderStyle =IMAGE_RENDER_STYLE_ALPHA;    //IMAGE_RENDER_STYLE_BORDER;//
	m_Image.nISPosition  = IMAGE_IS_POSITION_INIT;
	m_Image.nType        = ISI_T_SPR;
	m_Image.oPosition.nZ = 0;
	if (m_nKind == Obj_Kind_Item && m_nDropState == 1)	// ��Ʒ��������
	{
		m_Image.nFrame = m_cImageDrop.m_nCurFrame;
		m_Image.oPosition.nX = x;// - m_cImageDrop.m_nCgXpos;
		m_Image.oPosition.nY = y;// - m_cImageDrop.m_nCgYpos * 2;
		strcpy(m_Image.szImage, m_cImageDrop.m_szName);
	}
	else
	{
		m_Image.nFrame = m_cImage.m_nCurFrame;
		m_Image.oPosition.nX = x;// - m_cImage.m_nCgXpos;
		m_Image.oPosition.nY = y;// - m_cImage.m_nCgYpos * 2;
		strcpy(m_Image.szImage, m_cImage.m_szName);
	}

	if (m_nColorID<=0)
	   m_nameColor   = ax::Color3B::WHITE;
	else if (m_nColorID==1)
	{

		//m_nameColor  = {100,100,255};//ax::Color3B::BLUE;
		m_nameColor.r = 100;
		m_nameColor.g = 100;
		m_nameColor.b = 255;

	}
	else if (m_nColorID==2)
	{
		m_nameColor  = ax::Color3B::MAGENTA;
		/*m_nameColor.r = 188;
		m_nameColor.g = 64;
		m_nameColor.b = 255;*/
	}
	else if (m_nColorID==3)
		m_nameColor  = ax::Color3B::YELLOW;

	switch(m_nKind)
	{
	case Obj_Kind_MapObj:
		g_GameWorld->DrawPrimitives_obj(m_nIndex,0,&m_Image,0,0,1,0,m_nameColor,m_szName,_clientObjName);
		break;
	case Obj_Kind_Prop:
		if (m_nState == OBJ_PROP_STATE_DISPLAY )
		    g_GameWorld->DrawPrimitives_obj(m_nIndex,0,&m_Image,0,0,1,0,m_nameColor,m_szName,_clientObjName);
		break;
	default:
		    g_GameWorld->DrawPrimitives_obj(m_nIndex,0,&m_Image,0,0,1,0,m_nameColor,m_szName,_clientObjName);
		break;
	}
}



//-------------------------------------------------------------------------
//	���ܣ�	��������Ĵ���
//-------------------------------------------------------------------------
void KObj::Activate()
{
	if ( m_nIndex <= 0 )
		return;

   if (m_IsHaveAttack==1)
   {
      if (m_AttackerTime>0)
	     m_AttackerTime--;
	  if (m_AttackerTime<=0)
	  {
         //m_IsHaveAttack=0;
		 //m_AttackerDwid=0;
		 m_AttackerTime=0;
	  }
   }

	int	nMask = IPOT_RL_OBJECT | IPOT_RL_INFRONTOF_ALL;	// mat na
	switch(m_nKind)
	{
	case Obj_Kind_MapObj:					// ��ͼ�������
		if (m_nState == 0)					// ��ͼ�������ѭ������
			m_cImage.GetNextFrame();
		nMask = IPOT_RL_OBJECT | IPOT_RL_INFRONTOF_ALL;		// mat na
		break;
	case Obj_Kind_Light:					// ��Դ
		if (m_nState == 0)
			m_cImage.GetNextFrame();
		nMask = IPOT_RL_OBJECT | IPOT_RL_INFRONTOF_ALL;	// mat na
		break;
	case Obj_Kind_LoopSound:				// ѭ����Ч
		//PlayLoopSound();
		break;
	case Obj_Kind_RandSound:				// �����Ч
		//PlayRandSound();
		break;
	case Obj_Kind_Body:						// ʬ������ʧ
		m_cImage.GetNextFrame(FALSE);		// ʬ�嶯�������򲥷�
		m_nLifeTime--;
		if (m_nLifeTime <= 0)
			Remove(FALSE);
		nMask = IPOT_RL_COVER_GROUND | IPOT_RL_INFRONTOF_ALL;		// mat na
		break;
	case Obj_Kind_Box:						// ����
		if (m_nState == OBJ_BOX_STATE_CLOSE)// ����ر�״̬
			m_cImage.GetPrevFrame(FALSE);
		else// if (m_nState == OBJ_BOX_STATE_OPEN)// �����״̬
			m_cImage.GetNextFrame(FALSE);
		nMask = IPOT_RL_OBJECT | IPOT_RL_INFRONTOF_ALL;		// mat na
		break;
	case Obj_Kind_Door:						// ��
	{
		if (m_nState == OBJ_DOOR_STATE_CLOSE)// �Źر�״̬
			m_cImage.GetPrevFrame(FALSE);
		else// if (m_nState == OBJ_DOOR_STATE_OPEN)// �Ŵ�״̬
			m_cImage.GetNextFrame(FALSE);
		nMask = IPOT_RL_OBJECT | IPOT_RL_INFRONTOF_ALL;	// mat na

     }
	break;
	case Obj_Kind_Item:						// װ������ѭ������
	{
		if (this->m_nDropState == 1)		// ��Ʒ��������
		{
			if (m_cImageDrop.GetNextFrame(FALSE))
			{
				if (m_cImageDrop.CheckEnd())
				{
					m_nDropState = 0;			// ��Ʒ���������������ˣ���Ϊ����ѭ������
					m_Image.uImage = 0;
				}
			}
			nMask = IPOT_RL_OBJECT | IPOT_RL_INFRONTOF_ALL;		// mat na
		}
		else
		{
			if (this->m_nDropState == 2)
			{
				if (m_cImage.GetNextFrame(FALSE))
				{
					if (m_cImage.CheckEnd())
						this->m_nDropState = 0;
				}
			}
			else
			{
				if (g_Random(40) == 0)
				{
					m_nDropState = 2;
				}
				else
				{
					m_cImage.SetDirStart();
				}
			}
			nMask = IPOT_RL_COVER_GROUND | IPOT_RL_INFRONTOF_ALL;		// mat na
		}
		m_nLifeTime--;
		if (m_nLifeTime <=-54)
			Remove(FALSE);
    }
    break;
	case Obj_Kind_Money:
		m_cImage.GetNextFrame();
		nMask = IPOT_RL_COVER_GROUND | IPOT_RL_INFRONTOF_ALL;		// mat na
		m_nLifeTime--;
		if (m_nLifeTime <= -54)
			Remove(FALSE);
		break;
	case Obj_Kind_Prop:
		if (m_nState == OBJ_PROP_STATE_DISPLAY)
		{
			m_cImage.GetNextFrame();
			nMask = IPOT_RL_OBJECT | IPOT_RL_INFRONTOF_ALL;		// mat na
		}
		break;
	default:
		break;
	}
	int nMpsX, nMpsY;
	SubWorld[m_nSubWorldID].NewMap2Mps(m_nRegionIdx, m_nMapX, m_nMapY, m_nOffX, m_nOffY, &nMpsX, &nMpsY);
	if (g_GameWorld)
	    g_GameWorld->MoveObject(OBJ_NODE_OBJSET,m_nIndex,false,nMpsX,nMpsY,0);//�滭��Ʒ  m_SceneID, nMask
}

//-------------------------------------------------------------------------
//	���ܣ�	����ѭ����Ч
//-------------------------------------------------------------------------
void	KObj::PlayLoopSound()
{
	/*if (!m_szSoundName[0])
		return;

	m_pSoundNode = (KCacheNode*) g_SoundCache.GetNode(m_szSoundName, (KCacheNode * )m_pSoundNode);
	m_pWave = (KWavSound*)m_pSoundNode->m_lpData;
	if (m_pWave)
	{
		if (!m_pWave->IsPlaying())
		{
			m_pWave->Play(GetSoundPan(), GetSoundVolume(), 0);
		}
		else
		{
			m_pWave->SetPan(GetSoundPan());
			m_pWave->SetVolume(GetSoundVolume());
		}
	}*/
}

//-------------------------------------------------------------------------
//	���ܣ�	���������Ч
//-------------------------------------------------------------------------
void	KObj::PlayRandSound()
{
	/*if (!m_szSoundName[0])
		return;

	if (g_Random(500) != 0)
		return;

	m_pSoundNode = (KCacheNode*) g_SoundCache.GetNode(m_szSoundName, (KCacheNode * )m_pSoundNode);
	m_pWave = (KWavSound*)m_pSoundNode->m_lpData;
	if (m_pWave)
	{
		if (m_pWave->IsPlaying())
			return;
		m_pWave->Play(GetSoundPan(), GetSoundVolume(), 0);
	}*/
}

//-------------------------------------------------------------------------
//	���ܣ�	�õ����������С
//-------------------------------------------------------------------------
int		KObj::GetSoundPan()
{
	int		nNpcX, nNpcY, nObjX, nObjY;

	SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].NewMap2Mps(
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex,
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_MapX,
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_MapY,
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_OffX,
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_OffY,
		&nNpcX,
		&nNpcY);
	SubWorld[m_nSubWorldID].NewMap2Mps(
		m_nRegionIdx,
		m_nMapX,
		m_nMapY,
		m_nOffX,
		m_nOffY,
		&nObjX,
		&nObjY);

	return (nObjX - nNpcX) * 5;
}

//-------------------------------------------------------------------------
//	���ܣ�	�õ�����������С
//-------------------------------------------------------------------------
int		KObj::GetSoundVolume()
{
	int		nNpcX, nNpcY, nObjX, nObjY;

	SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].NewMap2Mps(
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex,
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_MapX,
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_MapY,
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_OffX,
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_OffY,
		&nNpcX,
		&nNpcY);
	SubWorld[m_nSubWorldID].NewMap2Mps(
		m_nRegionIdx,
		m_nMapX,
		m_nMapY,
		m_nOffX,
		m_nOffY,
		&nObjX,
		&nObjY);

	//return ((10000 - (abs(nObjX - nNpcX) + abs(nObjY - nNpcY) * 2)) * Option.GetSndVolume() / 100) - 10000;
	return 0;
}

//-------------------------------------------------------------------------
//	���ܣ�	��������
//-------------------------------------------------------------------------
void	KObj::PlaySound()
{
	/*if (!m_szSoundName[0])
		return;

	m_pSoundNode = (KCacheNode*) g_SoundCache.GetNode(m_szSoundName, (KCacheNode * )m_pSoundNode);
	m_pWave = (KWavSound*)m_pSoundNode->m_lpData;
	if (m_pWave)
	{
		if (m_pWave->IsPlaying())
			return;
		m_pWave->Play(GetSoundPan(), GetSoundVolume(), 0);
	}*/
}


//-------------------------------------------------------------------------
//	���ܣ�	��������ű�
//-------------------------------------------------------------------------
void	KObj::ExecScript(int nPlayerIdx,int nObjIdx,int nOgjWorlID)
{
	if (!m_dwScriptID)
		return;
	if (nPlayerIdx < 0)
		return;
	DWORD dwScriptId = m_dwScriptID;//g_FileName2Id(m_szScriptName);
	bool bExecuteScriptMistake = TRUE;
	KLuaScript * pScript = (KLuaScript*)g_GetScript(dwScriptId);

	if (pScript==NULL)
	{
		printf("-------OBJ�ű�������,ִ��ʧ��!-------- \n");
		return;
	}

	int nTopIndex = 0;
	try
	{
		if (pScript)
		{
			if (Player[nPlayerIdx].m_nIndex < 0) return ;

			Npc[Player[nPlayerIdx].m_nIndex].m_ActionScriptID = dwScriptId;

			Lua_PushNumber(pScript->m_LuaState, Player[nPlayerIdx].GetPlayerIndex());
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);

			Lua_PushNumber(pScript->m_LuaState, Player[nPlayerIdx].GetPlayerID());
			pScript->SetGlobalName(SCRIPT_PLAYERID);

			Lua_PushNumber(pScript->m_LuaState, m_nIndex);
			pScript->SetGlobalName(SCRIPT_OBJINDEX);

			nTopIndex=pScript->SafeCallBegin();

			BOOL bResult = FALSE;
			     bResult = pScript->CallFunction("main",0,"%d%d",nObjIdx,nOgjWorlID);
			if (bResult)
			{//ִ�гɹ��� �ָ���ǰ�Ľű�ID
				bExecuteScriptMistake = FALSE;
			}
			nTopIndex=pScript->SafeCallBegin();
			pScript->SafeCallEnd(nTopIndex);
			//lua_pop(pScript->m_LuaState, -1); //��ջ������1��Ԫ�� -1���ջ
		}
           if (bExecuteScriptMistake)
		   {
//			    Player[nPlayerIdx].m_bWaitingPlayerFeedBack = false;
	 		 	Npc[Player[nPlayerIdx].m_nIndex].m_ActionScriptID=0;
				return;
		   }

	}
	catch(...)
	{
		if (pScript)
		{
			nTopIndex=pScript->SafeCallBegin();
			pScript->SafeCallEnd(nTopIndex);
		}

		printf("-----ִ����Ʒ�ű��������� Obj-----!\n");
//	    Player[nPlayerIdx].m_bWaitingPlayerFeedBack = false;
		Npc[Player[nPlayerIdx].m_nIndex].m_ActionScriptID=0;
	}
	return;
}
//-------------------------------------------------------------------------
//	���ܣ�	��������ű�
//-------------------------------------------------------------------------
BOOL KObj::ExecScriptFiled(int nPlayerIdx,char *m_szScriptName,char *m_szcallfuns,int nParm1,int nParm2,char *sSubName,char *gsName,int nParm3)
{
	if (!m_szScriptName)
		return 1;
	if (nPlayerIdx < 0)
		return 1;
	DWORD dwScriptId = g_CheckFileExist(m_szScriptName);//g_FileName2Id(m_szScriptName);
    bool bExecuteScriptMistake = true;
	int bResult = 0;
	KLuaScript * pScript = (KLuaScript*)g_GetScript(dwScriptId);

	if (pScript==NULL)
	{
		printf("-------�ű�������,ִ��[%s][%s]ʧ��!-------- \n",m_szScriptName,m_szcallfuns);
		return FALSE;
	}

	if (pScript)
		{

			if (Player[nPlayerIdx].m_nIndex < 0) return -1;

			//Npc[Player[nPlayerIdx].m_nIndex].m_ActionScriptID = dwScriptId;

			Lua_PushNumber(pScript->m_LuaState, Player[nPlayerIdx].GetPlayerIndex());
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);

			Lua_PushNumber(pScript->m_LuaState, Player[nPlayerIdx].GetPlayerID());
			pScript->SetGlobalName(SCRIPT_PLAYERID);

			Lua_PushNumber(pScript->m_LuaState, m_nIndex);
			pScript->SetGlobalName(SCRIPT_OBJINDEX);

			int nTopIndex = 0;

			nTopIndex=pScript->SafeCallBegin();

			char *bResultchar=NULL;
			if (pScript->CallFunction(m_szcallfuns,1, "ddssdd", nParm1,nParm2,sSubName,gsName,nParm3,0))
			{
               bExecuteScriptMistake=false;
			}

			if (bExecuteScriptMistake==false)
			{
		 	   const char * szType = lua_typename(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			   if (Lua_IsNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)
			   {//Lua_PushNumber
			       bResult = (int)Lua_ValueToNumber(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			   }
			   else if (Lua_IsString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState)) == 1)  // �ַ�������
			   {
			       bResultchar = (char *)Lua_ValueToString(pScript->m_LuaState, Lua_GetTopIndex(pScript->m_LuaState));
			       bResult     = atoi(bResultchar);
			   }

			   nTopIndex=pScript->SafeCallBegin();
			   pScript->SafeCallEnd(nTopIndex);
			  // printf("Obj����Ʒ�ű��ɹ� [%d:%s]!\n",bResult,m_szScriptName);
		       return bResult;
			}
			  nTopIndex=pScript->SafeCallBegin();
			  pScript->SafeCallEnd(nTopIndex);
			  //lua_pop(pScript->m_LuaState, -1); //��ջ������1��Ԫ�� -1���ջ
		}
		 if (bExecuteScriptMistake)
		 {
//			Player[nPlayerIdx].m_bWaitingPlayerFeedBack = false;
			Npc[Player[nPlayerIdx].m_nIndex].m_ActionScriptID=0;
//			printf("Obj����Ʒ�ű�ʧ�� [%d:%s]!\n",bResult,m_szScriptName);
			return 0;
		 }

	return 0;
}
//-------------------------------------------------------------------------
//	���ܣ�	���������ӵ�
//-------------------------------------------------------------------------
void	KObj::CastSkill(int nDir)
{
	if (m_cSkill.m_nID <= 0 || m_cSkill.m_nLevel < 0)
		return;
//	Skill[m_nID][m_nLevel].cast();
}

//-------------------------------------------------------------------------
//	���ܣ�	��Ŀ��㷢���ӵ�
//-------------------------------------------------------------------------
void	KObj::CastSkill(int nXpos, int nYpos)
{
	if (m_cSkill.m_nID <= 0 || m_cSkill.m_nLevel < 0)
		return;
//	Skill[m_nID][m_nLevel].cast();
}

void KObj::Remove(BOOL bSoundFlag,BOOL nIsClear)
{
	/*if (bSoundFlag)
	{
		switch (m_nKind)
		{
		case Obj_Kind_Money:
			PlaySound();
			break;
		case Obj_Kind_Box:
		case Obj_Kind_Item:
		case Obj_Kind_Door:
		case Obj_Kind_Prop:
			break;
		}
	}*/
	m_Image.uImage = 0;

	if (g_GameWorld)
	    g_GameWorld->MoveObject(OBJ_NODE_OBJSET,m_nIndex,true,0,0,0);//

	if (m_nIndex >0 && m_nIndex<MAX_OBJECT)
	{
	  if (!nIsClear)
	  {
		if (KObject[m_nIndex].m_nSubWorldID>=0 && KObject[m_nIndex].m_nRegionIdx >= 0)
			SubWorld[KObject[m_nIndex].m_nSubWorldID].m_Region[KObject[m_nIndex].m_nRegionIdx].RemoveObj(m_nIndex);
		ObjSet.Remove(m_nIndex);
	  }

	}
}

int		KObj::GetKind()
{
	return m_nKind;
}

//-------------------------------------------------------------------------
//	���ܣ�	͹�����ת��Ϊ�ϰ���Ϣ
//-------------------------------------------------------------------------
void	KObj::PolygonChangeToBar(
								 KPolygon Polygon,	// ͹�����
								 int nGridWidth,	// ���ӳ�
								 int nGridHeight,	// ���ӿ�
								 int nTableWidth,	// ���
								 int nTableHeight,	// ����
								 BYTE *lpbBarTable)	// �������
{
	if ( !lpbBarTable )
		return;
	if (nGridWidth <= 0 || nGridHeight <= 0 || nTableWidth <= 0 || nTableHeight <= 0)
		return;

	int		nTemp, nTempLT, nTempRT, nTempLB, nTempRB, nFlag = 0;
	POINT	TempPos;
	for (int i = 0; i < nTableWidth * nTableHeight; ++i)
	{
		Polygon.GetCenterPos(&TempPos);
		// ����
		TempPos.x += ((i % nTableWidth) * nGridWidth) - ((nTableWidth / 2) * nGridWidth + nGridWidth / 2);
		TempPos.y += ((i / nTableWidth) * nGridHeight) - ((nTableHeight / 2) * nGridHeight + nGridHeight / 2);
		nTempLT = Polygon.IsPointInPolygon(TempPos);
		// ����
		TempPos.x += nGridWidth;
		nTempRT = Polygon.IsPointInPolygon(TempPos);
		// ����
		TempPos.x -= nGridWidth;
		TempPos.y += nGridHeight;
		nTempLB = Polygon.IsPointInPolygon(TempPos);
		// ����
		TempPos.x += nGridWidth;
		nTempRB = Polygon.IsPointInPolygon(TempPos);

		nTemp = nTempLT + nTempRT + nTempLB + nTempRB;
		if (nTemp == 0)
			lpbBarTable[i] = Obj_Bar_Empty;
		else if (nTemp > 1)
		{
			lpbBarTable[i] = Obj_Bar_Full;
			nFlag = 1;
		}
		else
		{
			if (nTempLT)
				lpbBarTable[i] = Obj_Bar_LT;
			else if (nTempRT)
				lpbBarTable[i] = Obj_Bar_RT;
			else if (nTempLB)
				lpbBarTable[i] = Obj_Bar_LB;
			else if (nTempRB)
				lpbBarTable[i] = Obj_Bar_RB;
		}
	}

	lpbBarTable[(nTableHeight / 2) * nTableWidth + nTableWidth / 2] = Obj_Bar_Full;
}

void KObj::GetMpsPos(int *pX, int *pY)
{
	SubWorld[m_nSubWorldID].NewMap2Mps(m_nRegionIdx, m_nMapX, m_nMapY, m_nOffX, m_nOffY, pX, pY);
}
//�滭��Ʒ�߿�
void KObj::DrawBorder()
{
	if (m_bDrawFlag)
		return;
	//m_Image.bRenderStyle = IMAGE_RENDER_STYLE_BORDER;  //��߻滭
	//m_Image.nType = ISI_T_SPR;

	switch(m_nKind)
	{
	case Obj_Kind_MapObj:
		//g_pRepresent->DrawPrimitives(1, &m_Image, RU_T_IMAGE, 0);
		break;
	case Obj_Kind_Prop:
		//if (m_nState == OBJ_PROP_STATE_DISPLAY )
			//g_pRepresent->DrawPrimitives(1, &m_Image, RU_T_IMAGE, 0);
		break;
	default:
		//g_pRepresent->DrawPrimitives(1, &m_Image, RU_T_IMAGE, 0);
		break;
	}
	//m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
}
