//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KObjSet.cpp
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------
#include "KCore.h"
#include "KSubWorld.h"
#include "KObj.h"
/*#ifndef _SERVER
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "scene/KScenePlaceC.h"
#endif*/
#include "KObjSet.h"
#include "CoreUseNameDef.h"
#include "KItem.h"
#include "gamescene/SceneDataDef.h"

#define		OBJ_WORLD_ID_START		1000

enum
{
	ObjDataField_Name = 1,
	ObjDataField_DataID,
	ObjDataField_Kind,
	ObjDataField_ScriptName,
	ObjDataField_ImageName,
	ObjDataField_SoundName,
	ObjDataField_LifeTime,
	ObjDataField_Layer,
	ObjDataField_Height,

	ObjDataFiled_SkillKind,
	ObjDataFiled_SkillCamp,
	ObjDataFiled_SkillRange,
	ObjDataField_SkillCastTime,
	ObjDataField_SkillID,
	ObjDataField_SkillLevel,

	ObjDataField_LightRadius,
	ObjDataField_LightRed,
	ObjDataField_LightGreen,
	ObjDataField_LightBlue,
	ObjDataField_LightAlpha,
	ObjDataField_LightReflectType,

	ObjDataField_ImageTotalFrame,
	ObjDataField_ImageCurFrame,
	ObjDataField_ImageTotalDir,
	ObjDataField_ImageCurDir,
	ObjDataField_ImageInterval,
	ObjDataField_ImageCgXpos,
	ObjDataField_ImageCgYpos,

	ObjDataField_Bar0,
	ObjDataField_Bar1,
	ObjDataField_Bar2,
	ObjDataField_Bar3,
	ObjDataField_Bar4,
	ObjDataField_Bar5,
	ObjDataField_Bar6,
	ObjDataField_Bar7,
	ObjDataField_Bar8,
	ObjDataField_Bar9,
	ObjDataField_Bar10,
	ObjDataField_Bar11,
	ObjDataField_Bar12,
	ObjDataField_Bar13,
	ObjDataField_Bar14,

	ObjDataField_ImageDropName,
	ObjDataField_ImageDropTotalFrame,
	ObjDataField_ImageDropCurFrame,
	ObjDataField_ImageDropTotalDir,
	ObjDataField_ImageDropCurDir,
	ObjDataField_ImageDropInterval,
	ObjDataField_ImageDropCgXpos,
	ObjDataField_ImageDropCgYpos,

	ObjDataField_DrawFlag,

	ObjDataField_Num,
};

KObjSet	ObjSet;

char	g_szObjKind[Obj_Kind_Num][32] =
{
	"MapObj",
	"Body",
	"Box",
	"Item",
	"Money",
	"LoopSound",
	"RandSound",
	"Light",
	"Door",
	"Trap",
	"Prop"
};

unsigned long	g_dwObjKindNum[Obj_Kind_Num] =
{
	0x4f70614d,
	0x79646f42,
	0x00786f42,
	0x6d657449,
	0x656e6f4d,
	0x706f6f4c,
	0x646e6152,
	0x6867694c,
	0x726f6f44,
	0x70617254,
	0x706f7250
};

KObjSet::KObjSet()
{
	m_nObjID = OBJ_WORLD_ID_START;
}

KObjSet::~KObjSet()
{
	m_nObjID = 0;
}

//---------------------------------------------------------------------------
//	���ܣ���ʼ���������ͼobj����ģ��
//---------------------------------------------------------------------------
int	KObjSet::Init()
{
	int	bRet = TRUE;
	int		i;

	m_FreeIdxObjSet.Init(MAX_OBJECT);
	m_UseIdxObjSet.Init(MAX_OBJECT);

	// ��ʼʱ���е�����Ԫ�ض�Ϊ��
	for (i = MAX_OBJECT - 1; i > 0; i--)
	{
		m_FreeIdxObjSet.Insert(i);
		//KObject[i].m_Node.m_nIndex=i;
	}

//	g_SetFilePath("");
	if ( !m_cTabFile.Load(OBJ_DATA_FILE_NAME) )
		bRet = FALSE;

//	g_SetFilePath("");
	if ( !m_cMoneyFile.Load(MONEY_OBJ_FILE_NAME))
		bRet = FALSE;

	int			nA, nR, nG, nB, nColorCount;
	char		szTemp[32];
	KIniFile	cColorIni;

//	g_SetFilePath("");
	if (!cColorIni.Load(OBJ_NAME_COLOR_FILE)) //��Ʒ��ɫ����
	{
		for (i = 0; i < MAX_OBJ_NAME_COLOR; ++i)  //��ɫ��������
			m_dwNameColor[i] = ax::Color3B::WHITE;
	}
	else
	{
		cColorIni.GetInteger("List", "Count", 0, &nColorCount);
		if (nColorCount > MAX_OBJ_NAME_COLOR)
			nColorCount = MAX_OBJ_NAME_COLOR;
		for (i = 0; i < nColorCount; ++i)
		{
			sprintf(szTemp, "%d", i);
			cColorIni.GetInteger(szTemp, "A", 0, &nA);
			cColorIni.GetInteger(szTemp, "R", 0, &nR);
			cColorIni.GetInteger(szTemp, "G", 0, &nG);
			cColorIni.GetInteger(szTemp, "B", 0, &nG);
			m_dwNameColor[i].r = nR;
			m_dwNameColor[i].g = nG;
			m_dwNameColor[i].b = nG;
			//m_dwNameColor[i] = (nA << 24) | (nR << 16) | (nG << 8) | nB;
		}
		for (i = nColorCount; i < MAX_OBJ_NAME_COLOR; ++i)
			m_dwNameColor[i] = ax::Color3B::WHITE;
	}
	this->m_nShowNameFlag = 0;

	cColorIni.Clear();
	return bRet;
}

//---------------------------------------------------------------------------
//	���ܣ��ͻ������һ��obj
//---------------------------------------------------------------------------
int		KObjSet::ClientAdd(int nID, int nDataID, int nState, int nDir, int nCurFrame, int nXpos, int nYpos, KCObjItemInfo sInfo,unsigned int nObjDwidx)
{
	if (nDataID <= 0 || nDataID >= m_cTabFile.GetHeight())
		return -1;

	int		nAddIndex;
	int		nRegion, nMapX, nMapY, nOffX, nOffY;
	SubWorld[0].Mps2Map(nXpos, nYpos, &nRegion, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nRegion < 0)
		return -1;
	nAddIndex = AddData(nDataID, 0, nRegion, nMapX, nMapY, nOffX, nOffY);


	if (nAddIndex < 0)
		return -1;
	KObject[nAddIndex].SetWorldID(nID);
	if (nDir >= 0)
	{
		KObject[nAddIndex].SetDir(nDir);
	}
	else
	{
		if (nCurFrame > 0)
			KObject[nAddIndex].m_cImage.SetCurFrame(nCurFrame);
	}
	if (nState >= 0)
	{
		KObject[nAddIndex].SetState(nState, sInfo.m_nSoundFlag);
	}
	if (KObject[nAddIndex].m_nKind == Obj_Kind_Item && sInfo.m_nMovieFlag)
	{
		KObject[nAddIndex].m_nDropState = 1;	// ��Ϊ��������
		if (nCurFrame > 0)
		{
			KObject[nAddIndex].m_cImageDrop.SetCurFrame(nCurFrame);

		}
	}
	else
	{
		KObject[nAddIndex].m_nDropState = 0;
	}

	KObject[nAddIndex].m_nMoneyNum   = sInfo.m_nMoneyNum;
	KObject[nAddIndex].m_nItemDataID = sInfo.m_nItemID;
	KObject[nAddIndex].m_nItemWidth  = sInfo.m_nItemWidth;
	KObject[nAddIndex].m_nItemHeight = sInfo.m_nItemHeight;
	KObject[nAddIndex].m_nColorID    = sInfo.m_nColorID;
	//KObject[nAddIndex].m_dwNameColor = 0x00ffffff ;//this->GetNameColor(sInfo.m_nColorID);
	KObject[nAddIndex].m_nameColor   = this->GetNameColor(sInfo.m_nColorID);
    KObject[nAddIndex].m_AttackerDwid= nObjDwidx;
    KObject[nAddIndex].m_IsHaveAttack=sInfo.m_cHaveAttack;

	if (KObject[nAddIndex].m_IsHaveAttack)
	   KObject[nAddIndex].m_AttackerTime=100800;
	else
	   KObject[nAddIndex].m_AttackerTime=0;

	if (_clientlanguage!=1)
	{//Խ�ϰ汾
		if (KObject[nAddIndex].m_nKind == Obj_Kind_Item)
		{
			if (sInfo.m_GoldId>0)
			{//�ƽ�
				sprintf(KObject[nAddIndex]._clientObjName,"����Խ������-�ƽ�");
			}
			else
			{//������ͨ��Ʒ

				sprintf(KObject[nAddIndex]._clientObjName,"����Խ������-������Ʒ");
			}
		}
		else if (KObject[nAddIndex].m_nKind == Obj_Kind_Money)
		{//Ǯ
			sprintf(KObject[nAddIndex]._clientObjName,"����Խ������-Ǯ");
		}
		else
		{//objdata
			sprintf(KObject[nAddIndex]._clientObjName,"ϵͳ����");

		}
	}
	else//���İ汾
       ZeroMemory(KObject[nAddIndex]._clientObjName,sizeof(KObject[nAddIndex]._clientObjName));

	if (sInfo.m_szName[0])
		strcpy(KObject[nAddIndex].m_szName,sInfo.m_szName);

	SubWorld[0].m_Region[nRegion].AddObj(nAddIndex);   // m_WorldMessage.Send(GWM_OBJ_ADD, nRegion, nAddIndex);
	return nAddIndex;
}

//---------------------------------------------------------------------------
//	���ܣ��ͻ�������һ��Region������Obj
//---------------------------------------------------------------------------
int	KObjSet::ClientLoadRegionObj(char *lpszMapPath, int nRegionX, int nRegionY, int nSubWorld, int nRegion)
{
	if (lpszMapPath==NULL || !lpszMapPath[0])
		return FALSE;

	char	szFile[256]={0};

//	sprintf(szPath, "%s\\v_%03d", lpszMapPath, nRegionY);
//	g_SetFilePath(szPath);

	KPakFile			cDataFile;
	KObjFileHead		sHead;

	sprintf(szFile, "%s\\v_%03d\\%03d_%s", lpszMapPath, nRegionY, nRegionX, REGION_OBJ_FILE_CLIENT);
	if (!cDataFile.Open(szFile))
		return FALSE;
	if (cDataFile.Size() < sizeof(KObjFileHead))
	{
		cDataFile.Close();
		return FALSE;
	}
	cDataFile.Read(&sHead, sizeof(KObjFileHead));

	KSPObj	sData;
	int		nKind;
	KCObjItemInfo	sInfo;
	for (int i = 0; (unsigned long)i < sHead.uNumObj; ++i)
	{
		memset(sData.szScript, 0, sizeof(sData.szScript));
		cDataFile.Read(&sData, sizeof(KSPObj) - sizeof(sData.szScript));
		cDataFile.Read(sData.szScript, sData.nScriptNameLen);
		nKind = GetDataIDKind(sData.nTemplateID);
		if (CheckClientKind(nKind) != 1)
			continue;

		sInfo.m_nItemID = 0;
		sInfo.m_nItemWidth = 0;
		sInfo.m_nItemHeight = 0;
		sInfo.m_nMoneyNum = 0;
		sInfo.m_nColorID = 0;
		sInfo.m_szName[0] = 0;
		sInfo.m_nMovieFlag = 0;
		sInfo.m_nSoundFlag = 0;
		sInfo.m_cHaveAttack=0;
		sInfo.m_Genre=-1;
		sInfo.m_DetailType=-1;
		sInfo.m_ParticularType=-1;
		sInfo.m_GoldId=0;
		sInfo.m_ItemLevel=0;
		sInfo.m_StackNum=0;
		//sInfo.m_cAttackerDwid=0;
		ClientAdd(0, sData.nTemplateID, sData.nState, sData.nDir, 0, sData.Pos.x, sData.Pos.y, sInfo);
	}
	cDataFile.Close();

	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ��ͻ�������һ��Region������Obj
//---------------------------------------------------------------------------
int	KObjSet::ClientAddRegionObj(KPakFile *pFile, unsigned int dwDataSize)
{
	if (!pFile || dwDataSize < sizeof(sizeof(KObjFileHead)))
		return FALSE;

	KObjFileHead	sHead;
	KSPObj			sData;
	int				nKind;
	KCObjItemInfo	sInfo;

	pFile->Read(&sHead, sizeof(sHead));
	for (int i = 0; (unsigned long)i < sHead.uNumObj; ++i)
	{
		pFile->Read(&sData, sizeof(KSPObj) - sizeof(sData.szScript));
		if (sData.nScriptNameLen < sizeof(sData.szScript))
		{
			pFile->Read(sData.szScript, sData.nScriptNameLen);
			sData.szScript[sData.nScriptNameLen] = 0;
		}
		else
		{
			sData.szScript[0] = 0;
			pFile->Seek(sData.nScriptNameLen, FILE_CURRENT);
		}
		nKind = GetDataIDKind(sData.nTemplateID);
		if (CheckClientKind(nKind) != 1)
			continue;

		sInfo.m_nItemID = 0;
		sInfo.m_nItemWidth = 0;
		sInfo.m_nItemHeight = 0;
		sInfo.m_nMoneyNum = 0;
		sInfo.m_nColorID = 0;
		sInfo.m_szName[0] = 0;
		sInfo.m_nMovieFlag = 0;
		sInfo.m_nSoundFlag = 0;
		sInfo.m_cHaveAttack=0;
		sInfo.m_Genre=-1;
		sInfo.m_DetailType=-1;
		sInfo.m_ParticularType=-1;
		sInfo.m_GoldId=0;
		sInfo.m_ItemLevel=0;
		sInfo.m_StackNum=0;
		//sInfo.m_cAttackerDwid=0;

		ClientAdd(0, sData.nTemplateID, sData.nState, sData.nDir, 0, sData.Pos.x, sData.Pos.y, sInfo);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
//	���ܣ�ͨ��obj��dataid���һ��obj
//---------------------------------------------------------------------------
int		KObjSet::AddData(int nDataID, int nSubWorld, int nRegion, int nMapX, int nMapY, int nOffX, int nOffY)
{
	if (nDataID <= 0 || nDataID >= m_cTabFile.GetHeight())
		return -1;
	if (nSubWorld < 0 || nSubWorld >= MAX_SUBWORLD)
		return -1;
	if (nRegion < 0)
		return -1;

	int		nFreeNo, i, nTemp;
	char	szBuffer[80];

	nFreeNo = FindFree();
	if (nFreeNo <= 0)
		return -1;

	KObject[nFreeNo].Release();
	KObject[nFreeNo].m_nDataID = nDataID;
	m_cTabFile.GetString(nDataID + 1, ObjDataField_Kind, g_szObjKind[0], szBuffer, sizeof(szBuffer));
	for (i = 0; i < Obj_Kind_Num; ++i)
	{
		if (*(unsigned long*)(&szBuffer) == g_dwObjKindNum[i])
			break;
	}
	if (i >= Obj_Kind_Num)
		i = 0;
	KObject[nFreeNo].m_nKind = i;
	// �� ObjData �ļ��ж�ȡ����
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_Layer, 1, &KObject[nFreeNo].m_nLayer);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_Height, 0, &KObject[nFreeNo].m_nHeight);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LifeTime, 0, &KObject[nFreeNo].m_nLifeTime);
	m_cTabFile.GetString(nDataID + 1, ObjDataField_Name, "", KObject[nFreeNo].m_szName, sizeof(KObject[nFreeNo].m_szName));
	g_ObjChangeName.GetString(nDataID + 1,2,"", KObject[nFreeNo]._clientObjName,sizeof(KObject[nFreeNo]._clientObjName));
	m_cTabFile.GetString(nDataID + 1, ObjDataField_ScriptName, "", szBuffer, sizeof(szBuffer));
	KObject[nFreeNo].SetScriptFile(szBuffer);
	m_cTabFile.GetString(nDataID + 1, ObjDataField_SoundName, "", KObject[nFreeNo].m_szSoundName, sizeof(KObject[nFreeNo].m_szSoundName));

	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightRadius, 0, &KObject[nFreeNo].m_sObjLight.m_nRadius);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightRed, 255, &KObject[nFreeNo].m_sObjLight.m_nRed);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightGreen, 255, &KObject[nFreeNo].m_sObjLight.m_nGreen);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightBlue, 255, &KObject[nFreeNo].m_sObjLight.m_nBlue);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightAlpha, 255, &KObject[nFreeNo].m_sObjLight.m_nAlpha);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_LightReflectType, 0, &KObject[nFreeNo].m_sObjLight.m_nReflectType);

	int nTotalFrame, nTotalDir, nCurFrame, nCurDir;
	m_cTabFile.GetString(nDataID + 1, ObjDataField_ImageName, "", KObject[nFreeNo].m_szImageName, sizeof(KObject[nFreeNo].m_szImageName));
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageTotalFrame, 1, &nTotalFrame);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageTotalDir, 1, &nTotalDir);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageCurFrame, 0, &nCurFrame);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageCurDir, 0, &nCurDir);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageInterval, 0, (int *)(&KObject[nFreeNo].m_cImage.m_dwInterval));
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageCgXpos, 0, &KObject[nFreeNo].m_cImage.m_nCgXpos);
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_ImageCgYpos, 0, &KObject[nFreeNo].m_cImage.m_nCgYpos);
	KObject[nFreeNo].m_cImage.SetFileName(KObject[nFreeNo].m_szImageName);
	KObject[nFreeNo].m_cImage.SetTotalFrame(nTotalFrame);
	KObject[nFreeNo].m_cImage.SetTotalDir(nTotalDir);
	KObject[nFreeNo].m_cImage.SetCurFrame(nCurFrame);
	KObject[nFreeNo].m_cImage.SetCurDir(nCurDir);

	if  (KObject[nFreeNo].m_cImage.m_nTotalDir!=0)
		KObject[nFreeNo].SetDir(KObject[nFreeNo].m_cImage.m_nCurDir * 64 / KObject[nFreeNo].m_cImage.m_nTotalDir);
	else
		KObject[nFreeNo].SetDir(0);

	int nDropTotalFrame, nDropTotalDir, nDropCurFrame, nDropCurDir;
	m_cTabFile.GetString(
		nDataID + 1,
		ObjDataField_ImageDropName,
		KObject[nFreeNo].m_szImageName,
		KObject[nFreeNo].m_szImageDropName,
		sizeof(KObject[nFreeNo].m_szImageDropName));
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropTotalFrame,
		nTotalFrame,
		&nDropTotalFrame);
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropTotalDir,
		nTotalDir,
		&nDropTotalDir);
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropCurFrame,
		nCurFrame,
		&nDropCurFrame);
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropCurDir,
		nCurDir,
		&nDropCurDir);
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropInterval,
		KObject[nFreeNo].m_cImage.m_dwInterval,
		(int *)(&KObject[nFreeNo].m_cImageDrop.m_dwInterval));
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropCgXpos,
		KObject[nFreeNo].m_cImage.m_nCgXpos,
		&KObject[nFreeNo].m_cImageDrop.m_nCgXpos);
	m_cTabFile.GetInteger(
		nDataID + 1,
		ObjDataField_ImageDropCgYpos,
		KObject[nFreeNo].m_cImage.m_nCgYpos,
		&KObject[nFreeNo].m_cImageDrop.m_nCgYpos);
	KObject[nFreeNo].m_cImageDrop.SetFileName(KObject[nFreeNo].m_szImageDropName);
	KObject[nFreeNo].m_cImageDrop.SetTotalFrame(nDropTotalFrame);
	KObject[nFreeNo].m_cImageDrop.SetTotalDir(nDropTotalDir);
	KObject[nFreeNo].m_cImageDrop.SetCurFrame(nDropCurFrame);
	KObject[nFreeNo].m_cImageDrop.SetCurDir(nDropCurDir);

	for (i = 0; i < OBJ_BAR_SIZE; ++i)
	{
		m_cTabFile.GetInteger(nDataID + 1, ObjDataField_Bar0 + i, 0, &nTemp);
		KObject[nFreeNo].m_btBar[i] = (BYTE)nTemp;
	}
	int nCurVal = 0;
	m_cTabFile.GetInteger(nDataID + 1, ObjDataField_DrawFlag, 0, &nCurVal);
	KObject[nFreeNo].m_bDrawFlag = nCurVal>0?true:false;
	KObject[nFreeNo].m_nIndex = nFreeNo;
	KObject[nFreeNo].m_nSubWorldID = nSubWorld;
	KObject[nFreeNo].m_nRegionIdx  = nRegion;
	KObject[nFreeNo].m_nMapX = nMapX; //��������
	KObject[nFreeNo].m_nMapY = nMapY; //��������
	KObject[nFreeNo].m_nOffX = nOffX;
	KObject[nFreeNo].m_nOffY = nOffY;



	KObject[nFreeNo].m_nBelongRegion = SubWorld[0].m_Region[nRegion].m_RegionID;

	// if (��������ϰ���) �趨����ϰ�

	m_UseIdxObjSet.Insert(nFreeNo);
	m_FreeIdxObjSet.Remove(nFreeNo);
	return nFreeNo;
}

//---------------------------------------------------------------------------
//	���ܣ���û��ʹ�õĿ���obj
//---------------------------------------------------------------------------
int		KObjSet::FindFree()
{
	return m_FreeIdxObjSet.GetNext(0);
}

//---------------------------------------------------------------------------
//	���ܣ��ж�ĳ�����͵�obj�ǲ���clientonly����
//---------------------------------------------------------------------------
int 	KObjSet::CheckClientKind(int nKind)
{
	switch (nKind)
	{
	case Obj_Kind_MapObj:
	case Obj_Kind_Body:
	case Obj_Kind_LoopSound:
	case Obj_Kind_RandSound:
	case Obj_Kind_Light:
		return 1;
	case Obj_Kind_Door:
	case Obj_Kind_Trap:
	case Obj_Kind_Prop:
	case Obj_Kind_Box:
	case Obj_Kind_Item:
	case Obj_Kind_Money:
		return 0;
	default:
		return -1;
	}
	return -1;
}
//---------------------------------------------------------------------------
//	���ܣ���ĳ��id��obj
//---------------------------------------------------------------------------
int		KObjSet::FindID(int nID)
{
	if (nID < OBJ_WORLD_ID_START)
		return 0;
	/*
	for (int i = 1; i < MAX_OBJECT; ++i)
	{
		if ( KObject[i].m_nIndex > 0 && KObject[i].m_nID == nID)
			return i;
	}*/
	int i = m_UseIdxObjSet.GetNext(0);
	while (i != 0)
	{
		if (KObject[i].m_nIndex > 0 && KObject[i].m_nID == nID)
			return i;
		i = m_UseIdxObjSet.GetNext(i);
	}
	return 0;
}

//---------------------------------------------------------------------------
//	���ܣ���ĳ�����ֵ�obj
//---------------------------------------------------------------------------
int		KObjSet::FindName(char *lpszObjName)
{
	if (!lpszObjName || !lpszObjName[0])
		return 0;

	//for (int i = 1; i < MAX_OBJECT; ++i)
	int i = m_UseIdxObjSet.GetNext(0);
	while(i != 0)
	{
		if (KObject[i].m_nIndex > 0 && strcmp(KObject[i].m_szName, lpszObjName) == 0)
			return i;
		i = m_UseIdxObjSet.GetNext(i);
	}
	return 0;
}

//---------------------------------------------------------------------------
//	���ܣ�ֻ����Obj �� SetWorldID ��ʱ����ã�����ʱ��ֱ��ʹ��m_nObjID
//---------------------------------------------------------------------------
int		KObjSet::GetID()
{
	m_nObjID++;
	return (m_nObjID - 1);
}

void	KObjSet::Remove(int nIdx)
{
	KObject[nIdx].Release();
	m_FreeIdxObjSet.Insert(nIdx);
	m_UseIdxObjSet.Remove(nIdx);
}

void	KObjSet::RemoveIfClientOnly(int nIdx)
{
	if (CheckClientKind(KObject[nIdx].GetKind()) != 1)
		return;
	KObject[nIdx].Release();
	m_FreeIdxObjSet.Insert(nIdx);
	//m_FreeIdxObjSet.Remove(nIdx);
	m_UseIdxObjSet.Remove(nIdx);
}

int		KObjSet::GetDataIDKind(int nDataID)
{
	if (nDataID <= 0 || nDataID >= m_cTabFile.GetHeight())
		return -1;
	char	szBuffer[80];
	m_cTabFile.GetString(nDataID + 1, ObjDataField_Kind, g_szObjKind[0], szBuffer, sizeof(szBuffer));
	int i;
	for (i = 0; i < Obj_Kind_Num; ++i)
	{
		if (strcmp(szBuffer, g_szObjKind[i]) == 0)
			break;
	}
	if (i >= Obj_Kind_Num)
		return -1;
	return i;
}

int	KObjSet::SearchObjAt(int nX, int nY, int nRange)
{
	int nIdx = 0;
	int	nMin = nRange;
	int nMinIdx = 0;
	int	nLength = 0;
	int nSrcX[2];
	int	nSrcY[2];

	nSrcX[0] = nX;
	nSrcY[0] = nY;
	//g_ScenePlace.ViewPortCoordToSpaceCoord(nSrcX[0], nSrcY[0], 0);
	SubWorld[0].GetMpsByLocalPosition(nSrcX[0], nSrcY[0], 0);

	nSrcX[1] = nX;
	nSrcY[1] = nY;
	//g_ScenePlace.ViewPortCoordToSpaceCoord(nSrcX[1], nSrcY[1], 120);
	SubWorld[0].GetMpsByLocalPosition(nSrcX[1], nSrcY[1], 120);

	int nDx = nSrcX[0] - nSrcX[1];
	int nDy = nSrcY[0] - nSrcY[1];

	while(1)
	{
		nIdx = m_UseIdxObjSet.GetNext(nIdx);
		if (!nIdx)
			break;

		if (KObject[nIdx].m_nRegionIdx < 0)
			continue;

		if (Obj_Kind_Box != KObject[nIdx].m_nKind
			&& Obj_Kind_Item != KObject[nIdx].m_nKind
			&& Obj_Kind_Money != KObject[nIdx].m_nKind
			&& Obj_Kind_Prop != KObject[nIdx].m_nKind
			&& Obj_Kind_Door != KObject[nIdx].m_nKind)
			continue;

		int x, y;
		SubWorld[0].NewMap2Mps(KObject[nIdx].m_nRegionIdx, KObject[nIdx].m_nMapX, KObject[nIdx].m_nMapY,
			KObject[nIdx].m_nOffX, KObject[nIdx].m_nOffY, &x, &y);

		if (nSrcY[0] > y)
			continue;

		if (nSrcY[0] < y - 40 - KObject[nIdx].m_nHeight)
			continue;

		nLength = abs(nDx * (nSrcY[0] - y) / nDy + nSrcX[0] - x);
		if (nLength < nMin)
		{
			nMin = nLength;
			nMinIdx = nIdx;
		}
	}

	return nMinIdx;
}

ax::Color3B	KObjSet::GetNameColor(int nColorID)
{
	if (nColorID < 0 || nColorID >= MAX_OBJ_NAME_COLOR)
		return this->m_dwNameColor[0];
	return this->m_dwNameColor[nColorID];
}

//-------------------------------------------------------------------------
//	���ܣ��趨�Ƿ�ȫ����ʾ item �� money ��� object ������
//			bFlag ==	TRUE ��ʾ��bFlag == FALSE ����ʾ zroc add
//-------------------------------------------------------------------------
void	KObjSet::SetShowNameFlag(int bFlag)
{
	this->m_nShowNameFlag = bFlag;
}
//-------------------------------------------------------------------------
//	���ܣ��ж��Ƿ�ȫ����ʾ item �� money ��� object ������  ����ֵ TRUE ��ʾ��FALSE ����ʾ
//-------------------------------------------------------------------------
int	KObjSet::CheckShowName()
{
	return m_nShowNameFlag;
}
