// ***************************************************************************************
// ������ͼ�����������ඨ��ʵ��
// Copyright : Kingsoft 2002
// Author    : wooy(wu yue)
// CreateTime: 2002-11-11
// ***************************************************************************************
#include "gamecore/KCore.h"
#include "engine/KEngine.h"
#include "engine/KTabFileCtrl.h"
#include "KScenePlaceRegionC.h"
//#include "crtdbg.h"
#include "gamecore/ImgRef.h"
//#include "../../Represent/iRepresent/iRepresentShell.h"
#include "KIpotLeaf.h"
#include "SceneDataDef.h"
#include "SceneMath.h"
#include "ObstacleDef.h"
#include "math.h"
/*#ifdef SWORDONLINE_SHOW_DBUG_INFO
#include "gamecore/GameDataDef.h"
#include "gamecore/KSubWorld.h"
#endif*/

#define	OBJ_IS_COPY_FROM_NEST_REGION	0xff
#define OBJ_IS_SELF_OWNED				4

#define OBJ_IMAGE_ID					oPos2.x
#define	OBJ_IMAGE_ISPOSITION			oPos2.y

#define	LOCAL_MAX_IMG_NUM	600 //320
#define	LOCAL_FULL_IMG_NUM  600
//int  g_iShowObstacle=FALSE;
//�Ƿ����log
//int  g_iShowPaint   =FALSE;
//#define	OUTPUT_PROCESS_TIME

//##ModelId=3DD7EA9200B2
KScenePlaceRegionC::KScenePlaceRegionC()
{
	m_Flag = 0;
	m_Status = REGION_S_IDLE;
	m_LeftTopCornerScenePos.x = m_LeftTopCornerScenePos.y = 0;
	m_RegionIndex.x = m_RegionIndex.y = 0;
	memset(&m_GroundLayerData, 0, sizeof(KGroundLayerData));
	memset(&m_BiosData, 0, sizeof(KBiosData));
	m_pPrerenderGroundImg = NULL;
	//m_BackNum = 0;
	m_BackTreeNum = 0;
	m_BackHouseNum= 0;
	m_ClearState  = 0;//δɾ��
	//memset(m_TrapInfo, 0, sizeof(m_TrapInfo));
}

//##ModelId=3DD7EA9200F8
KScenePlaceRegionC::~KScenePlaceRegionC()
{
	Clear();
}

//##ModelId=3DD3DD2C0141
void KScenePlaceRegionC::GetRegionIndex(int& nX, int& nY) const
{
	nX = m_RegionIndex.x;
	nY = m_RegionIndex.y;
}

//##ModelId=3DBDAC140299
bool KScenePlaceRegionC::ToLoad(int nIndexX, int nIndexY)
{
	//CCAssert (m_Status != REGION_S_LOADING,"");
	if (m_Status==REGION_S_LOADING) return false;
	Clear();
	m_RegionIndex.x = nIndexX;
	m_RegionIndex.y = nIndexY;
	m_Status = REGION_S_TO_LOAD;
	return true;
}

//##ModelId=3DB901F101CD
bool KScenePlaceRegionC::LoadRe(const char* pszBaseFolderName)
{//��Ϊ ����
	if (m_Status != REGION_S_TO_LOAD || !pszBaseFolderName || pszBaseFolderName[0] == 0)
		return false;

	m_Status = REGION_S_LOADING;
	/////////////////////////////////////////////////////////��Ϊ����
	char RegionPathPrefix[256];
	ZeroMemory(RegionPathPrefix,sizeof(RegionPathPrefix));
	sprintf(RegionPathPrefix,"%s\\v_%03d\\%03d%s",pszBaseFolderName,m_RegionIndex.y,m_RegionIndex.x,"_");
	//messageBox(UTEXT(RegionPathPrefix,1).c_str(),"TESE");;
	m_LeftTopCornerScenePos.x = m_RegionIndex.x * RWPP_AREGION_WIDTH;
	m_LeftTopCornerScenePos.y = m_RegionIndex.y * RWPP_AREGION_HEIGHT;
	char	File[256]={0};
	ZeroMemory(File,sizeof(File));
	sprintf(File,"%s%s",RegionPathPrefix,REGION_COMBIN_FILE_NAME_CLIENT);
	//messageBox(File,"TESE");
	////////////////////////////////////////////////////////
	//return false;
	KPakFile	Data;
	if (Data.Open(File))
	{
		//return false;
		unsigned int uMaxElemFile = 0;
		Data.Read(&uMaxElemFile, sizeof(unsigned int));
		KCombinFileSection	ElemFile[REGION_ELEM_FILE_COUNT] = { 0 };
		if (uMaxElemFile > REGION_ELEM_FILE_COUNT)
		{
			Data.Read(&ElemFile[0], sizeof(KCombinFileSection) * REGION_ELEM_FILE_COUNT);
			Data.Seek(sizeof(KCombinFileSection) * (uMaxElemFile - REGION_ELEM_FILE_COUNT), FILE_CURRENT);
		}
		else
		{
			Data.Read(&ElemFile[0], sizeof(KCombinFileSection) * uMaxElemFile);
		}

		unsigned int uOffsetAhead = sizeof(unsigned int) + sizeof(KCombinFileSection) * uMaxElemFile;

		//--�ر��--
		if (ElemFile[REGION_GROUND_LAYER_FILE_INDEX].uLength)
		{
			Data.Seek(uOffsetAhead + ElemFile[REGION_GROUND_LAYER_FILE_INDEX].uOffset, FILE_BEGIN);
			LoadGroundLayer(&Data, ElemFile[REGION_GROUND_LAYER_FILE_INDEX].uLength);
		}

		//--�ڽ�����--
		if (ElemFile[REGION_BUILDIN_OBJ_FILE_INDEX].uLength)
		{
			Data.Seek(uOffsetAhead + ElemFile[REGION_BUILDIN_OBJ_FILE_INDEX].uOffset, FILE_BEGIN);
			LoadAboveGroundObjects(&Data, ElemFile[REGION_BUILDIN_OBJ_FILE_INDEX].uLength);
		}


		//--�ϰ�--
		if (ElemFile[REGION_OBSTACLE_FILE_INDEX].uLength)
		{
			//GetFliePath("�ϰ�",File);
			Data.Seek(uOffsetAhead + ElemFile[REGION_OBSTACLE_FILE_INDEX].uOffset, FILE_BEGIN);
			LoadObstacle(&Data, ElemFile[REGION_OBSTACLE_FILE_INDEX].uLength);
		}
		else
		{
			//LoadObstacle(NULL, 0);
			m_cObstacle.Clear();
		}
		//--����--
		if (ElemFile[REGION_TRAP_FILE_INDEX].uLength)
		{
			Data.Seek(uOffsetAhead + ElemFile[REGION_TRAP_FILE_INDEX].uOffset, FILE_BEGIN);
			LoadTrap(&Data, ElemFile[REGION_TRAP_FILE_INDEX].uLength);
		}
		Data.Close();
	}
	else
	{
		unsigned int uSize;
		//--�ر��--
		sprintf(File, "%s%s",RegionPathPrefix,REGION_GROUND_LAYER_FILE);
		if (Data.Open(File))
		{
			uSize = Data.Size();
			LoadGroundLayer(&Data, uSize);
			Data.Close();
		}

		//--�ڽ�����--
		sprintf(File, "%s%s",RegionPathPrefix,REGION_BUILDIN_OBJ_FILE);
		if (Data.Open(File))
		{
			uSize = Data.Size();
			LoadAboveGroundObjects(&Data, uSize);

			Data.Close();
		}

		//--�ϰ�--
		sprintf(File, "%s%s",RegionPathPrefix,REGION_OBSTACLE_FILE);
		if (Data.Open(File))
		{
			//GetFliePath("�ϰ�",File);
			uSize = Data.Size();
			LoadObstacle(&Data, uSize);
			Data.Close();
		}
		else
			//LoadObstacle(NULL, 0);
			m_cObstacle.Clear();
		//--����--
		sprintf(File, "%s%s", RegionPathPrefix,REGION_TRAP_FILE);
		if (Data.Open(File))
		{//��trap��
			uSize = Data.Size();
			LoadTrap(&Data, uSize);
			Data.Close();
		}
	}
	m_Status = REGION_S_STANDBY;

	return true;
}


void KScenePlaceRegionC::GetFliePath(char* nName,char * nPath,int nMaxRows)
{
	KFile nFile;
	KTabFileCtrl nScirptFile;
	char nTongApplyPath[125]={0},szCol[128]={0};
	sprintf(nTongApplyPath,"%s","\\jxpath.log");
	if (!g_FileExists(nTongApplyPath))	//�Ƿ����
	{
		nFile.Create(nTongApplyPath);
		//sprintf(szCol,"��ͼ\t����\tԴ�ű�\15\n");
		sprintf(szCol,"��ͼ+����+Դ�ű�\15\n");
		nFile.Write(szCol, sizeof(szCol));
		//nFile.Save(nTongApplyPath);
		nFile.Close();
	}

	if (nScirptFile.Load(nTongApplyPath))
	{
		int nRows=nScirptFile.GetHeight();

		if  (nRows<=0)
			nRows=1;

		if (nRows>=nMaxRows)
		{

			//nScirptFile.Clear();
			//return;
		  	while(nScirptFile.GetHeight()>0)
			{
				nScirptFile.Remove(nScirptFile.GetHeight());
				nScirptFile.Save(nTongApplyPath);
			}

			nRows=nScirptFile.GetHeight();

			if  (nRows<=0)
				 nRows=1;

		}

		sprintf(szCol,"--��ͼ%s:(%s)--",nName,nPath);
		nScirptFile.InsertAfter(nRows);
		nScirptFile.WriteString(nRows,1, szCol);                                    //�к�
		//nTong.WriteString(nRows,2,Npc[Player[m_nPlayerIndex].m_nIndex].Name);     //����
		//nTong.WriteInteger(nRows,3,Npc[Player[m_nPlayerIndex].m_nIndex].m_Level); //�ȼ�
		nScirptFile.Save(nTongApplyPath);
		//nFile.Write(szCol, sizeof(szCol));
	}
	nScirptFile.Clear();
}
//##ModelId=3DBF876400B2
void KScenePlaceRegionC::Clear()
{
	if (m_Status != REGION_S_STANDBY)
		return;
	m_Flag = 0;
	if (m_pPrerenderGroundImg)
	{
		//��KRUImage::GROUND_IMG_OCCUPY_FLAG(bMatchReferenceSpot)����ʾKRUImage�����Ƿ�ռ��
		//��KRUImage::GROUND_IMG_OK_FLAG(bFrameDraw)����ʾKRUImage�����Ƿ��Ѿ�Ԥ��Ⱦ����
		m_pPrerenderGroundImg->GROUND_IMG_OCCUPY_FLAG = false;
		m_pPrerenderGroundImg->GROUND_IMG_OK_FLAG = false;
		m_pPrerenderGroundImg = NULL;
	}

	m_GroundLayerData.uNumGrunode = 0;
	m_GroundLayerData.uNumObject = 0;
	if (m_GroundLayerData.pGrunodes)
	{
		free(m_GroundLayerData.pGrunodes);
		m_GroundLayerData.pGrunodes = NULL;
	}
	if (m_GroundLayerData.pObjects)
	{
		free(m_GroundLayerData.pObjects);
		m_GroundLayerData.pObjects = NULL;
	}
	if (m_BiosData.pLights)
		free(m_BiosData.pLights);
	if (m_BiosData.pBios)
		free (m_BiosData.pBios);
	if (m_BiosData.pLeafs)
		free (m_BiosData.pLeafs);
	memset(&m_BiosData, 0, sizeof(KBiosData));
	m_Status = REGION_S_STANDBY;
}


//--------------------------------------------------------------------------
//	���ܣ����ַ�����ʾ����ɫ��ϢתΪ��ֵ��ʾ
//--------------------------------------------------------------------------
unsigned int KScenePlaceRegionC::GetColor(LPCTSTR pString)
{
	if (pString == NULL)
		return 0;

	unsigned int Color = 0xFF000000;  //��ɫ

	char Buf[16] = "";
	int  i = 0;
	int  n = 0;
	while (pString[i] != ',')
	{
		if (pString[i] == 0 || n >= 15)
			return Color;
		Buf[n++] = pString[i++];
	}

	Buf[n] = 0;
	Color += ((atoi(Buf) & 0xFF) << 16);
	n = 0;
	i++;
	while (pString[i] != ',')
	{
		if (pString[i] == 0 || n >= 15)
			return Color;
		Buf[n++] = pString[i++];
	}
	Buf[n] = 0;
	Color += ((atoi(Buf) & 0xFF) << 8);
	n = 0;
	i++;
	while (pString[i] != 0)
	{
		if (n >= 15)
			return Color;
		Buf[n++] = pString[i++];
	}
	Buf[n] = 0;
	Color += (atoi(Buf) & 0xFF);
	return Color;
}


//##ModelId=3DBF9582039A //�滭·�涫��
bool KScenePlaceRegionC::PrerenderGround(bool bForce)
{//g_pRepresent == NULL ||
	if ( m_pPrerenderGroundImg == NULL ||
		(bForce == false && m_pPrerenderGroundImg->GROUND_IMG_OK_FLAG))
		return true;

	m_pPrerenderGroundImg->GROUND_IMG_OK_FLAG = true;

//	g_pRepresent->ClearImageData(m_pPrerenderGroundImg->szImage,
	//	m_pPrerenderGroundImg->uImage, m_pPrerenderGroundImg->nISPosition);

	KRUImage	ImgList[LOCAL_MAX_IMG_NUM];
	KRUImage	*pGi;
	unsigned int nIndex;
	int			CellWidth  = RWPP_AREGION_WIDTH / RWP_NUM_GROUND_CELL_H;
	int			CellHeight = RWPP_AREGION_HEIGHT / 2 / RWP_NUM_GROUND_CELL_V;

	memset(&ImgList, 0, sizeof(ImgList));
	int			nNum = 0;
	pGi = &ImgList[0];

	//----����ױ���:�ݵء�ˮ��----
	char nTrapInfo[256]={0};
	KSPRCrunode* pGrunode = m_GroundLayerData.pGrunodes;
	for (nIndex = 0; nIndex < m_GroundLayerData.uNumGrunode; ++nIndex)
	{
		pGi->bRenderStyle = IMAGE_RENDER_STYLE_OPACITY;
		pGi->nType = ISI_T_SPR;
		pGi->oPosition.nX = pGrunode->Param.h * CellWidth;
		pGi->oPosition.nY = pGrunode->Param.v * CellHeight;
		memcpy(pGi->szImage, pGrunode->szImgName, pGrunode->Param.nFileNameLen);
		/*if (g_iShowPaint)
		{
		    sprintf(nTrapInfo,"�ײ�:%s",pGi->szImage);
		    g_LogFile.gTraceLogFile(nTrapInfo,sizeof(nTrapInfo));
		}*/
		pGi->nFrame = pGrunode->Param.nFrame;
		pGi->uImage = 0;
		pGi->nISPosition = IMAGE_IS_POSITION_INIT;
		pGrunode = (KSPRCrunode*)(((char*)pGrunode) +
			sizeof(KSPRCrunode::KSPRCrunodeParam) + pGrunode->Param.nFileNameLen);
		nNum++;
		if (nNum < LOCAL_MAX_IMG_NUM)
		{
			pGi++;
		}
		else
		{
//			g_pRepresent->DrawPrimitivesOnImage(LOCAL_MAX_IMG_NUM, &ImgList[0], RU_T_IMAGE,
	//			m_pPrerenderGroundImg->szImage, m_pPrerenderGroundImg->uImage,
//				m_pPrerenderGroundImg->nISPosition);
			//messageBox("��ʼ�滭��ͼ","PrerenderGround");
			nNum = 0;
			pGi = &ImgList[0];
		}
	}

	//----�����ر���Ķ���:·���----
	KSPRCoverGroundObj* pObj = m_GroundLayerData.pObjects;
	for (nIndex = 0; nIndex < m_GroundLayerData.uNumObject; ++nIndex, ++pObj)
	{
		pGi->bRenderStyle =IMAGE_RENDER_STYLE_3LEVEL;//IMAGE_RENDER_STYLE_ALPHA_COLOR_ADJUST;//IMAGE_RENDER_STYLE_ALPHA;// IMAGE_RENDER_STYLE_ALPHA_COLOR_ADJUST;//IMAGE_RENDER_STYLE_3LEVEL;
		pGi->nType = ISI_T_SPR;
		pGi->bRenderFlag = RUIMAGE_RENDER_FLAG_FRAME_DRAW;//RUIMAGE_RENDER_FLAG_REF_SPOT;//RUIMAGE_RENDER_FLAG_FRAME_DRAW;
		//pGi->Color.Color_dw = GetColor("250,250,250");//g_pAdjustColorTab[16];
		pGi->Color.Color_b.a = 255;
		pGi->oPosition.nX = pObj->nPositionX - m_LeftTopCornerScenePos.x;
		pGi->oPosition.nY = (pObj->nPositionY - m_LeftTopCornerScenePos.y) / 2;
		strcpy(pGi->szImage, pObj->szImage);
		/*if (g_iShowPaint)
		{
		 sprintf(nTrapInfo,"·��:%s",pGi->szImage);
		 g_LogFile.gTraceLogFile(nTrapInfo,sizeof(nTrapInfo));
		}*/
		pGi->nFrame = pObj->nFrame;
		pGi->uImage = 0;
		pGi->nISPosition = IMAGE_IS_POSITION_INIT;
		nNum++;
		if (nNum < LOCAL_MAX_IMG_NUM)
		{
			pGi++;
		}
		else
		{
//			g_pRepresent->DrawPrimitivesOnImage(LOCAL_MAX_IMG_NUM, &ImgList[0], RU_T_IMAGE,
	//			m_pPrerenderGroundImg->szImage, m_pPrerenderGroundImg->uImage,
//				m_pPrerenderGroundImg->nISPosition);
			nNum = 0;
			pGi = &ImgList[0];
		}
	}

	if (nNum)
	{
		//g_pRepresent->DrawPrimitivesOnImage(nNum, &ImgList[0], RU_T_IMAGE,
			//m_pPrerenderGroundImg->szImage, m_pPrerenderGroundImg->uImage,
			//m_pPrerenderGroundImg->nISPosition);
	}
	return true;
}

/*
//##ModelId=3DBF9582039A
bool KScenePlaceRegionC::PrerenderGround()
{
	if (g_pRepresent == NULL || m_pPrerenderGroundImg == NULL ||
		m_pPrerenderGroundImg->GROUND_IMG_OK_FLAG)
		return true;

	m_pPrerenderGroundImg->GROUND_IMG_OK_FLAG = true;

	g_pRepresent->ClearImageData(m_pPrerenderGroundImg->szImage,
		m_pPrerenderGroundImg->uImage, m_pPrerenderGroundImg->nImagePosition);

	KRUImage	gi;
	unsigned int nIndex;
	int			CellWidth  = RWPP_AREGION_WIDTH / RWP_NUM_GROUND_CELL_H;
	int			CellHeight = RWPP_AREGION_HEIGHT / 2 / RWP_NUM_GROUND_CELL_V;

	memset(&gi, 0, sizeof(gi));

	//----����ױ���:�ݵء�ˮ��----
	KSPRCrunode* pGrunode = m_GroundLayerData.pGrunodes;
	gi.bRenderStyle = IMAGE_RENDER_STYLE_OPACITY;
	gi.nType = ISI_T_SPR;
	for (nIndex = 0; nIndex < m_GroundLayerData.uNumGrunode; nIndex++)
	{
		gi.nX = pGrunode->Param.h * CellWidth;
		gi.nY = pGrunode->Param.v * CellHeight;
		memcpy(gi.szImage, pGrunode->szImgName, pGrunode->Param.nFileNameLen);
		gi.szImage[pGrunode->Param.nFileNameLen] = 0;
		gi.nFrame = pGrunode->Param.nFrame;
		g_pRepresent->DrawPrimitivesOnImage(1, &gi, RU_T_IMAGE,
			m_pPrerenderGroundImg->szImage, m_pPrerenderGroundImg->uImage,
			m_pPrerenderGroundImg->nImagePosition);
		gi.uImage = 0;
		gi.szImage[0] = 0;
		gi.nImagePosition = IMAGE_IS_POSITION_INIT;
		pGrunode = (KSPRCrunode*)(((char*)pGrunode) +
			sizeof(KSPRCrunode::KSPRCrunodeParam) + pGrunode->Param.nFileNameLen);
	}

	//----�����ر���Ķ���:·���----

	gi.bRenderStyle = IMAGE_RENDER_STYLE_3LEVEL;
	gi.bRenderFlag = RUIMAGE_RENDER_FLAG_FRAME_DRAW;
	gi.Color.Color_b.a = 255;
	KSPRCoverGroundObj* pObj = m_GroundLayerData.pObjects;
	for (nIndex = 0; nIndex < m_GroundLayerData.uNumObject; nIndex++, pObj++)
	{
		gi.nX = pObj->nPositionX - m_LeftTopCornerScenePos.x;
		gi.nY = (pObj->nPositionY - m_LeftTopCornerScenePos.y) / 2;
		strcpy(gi.szImage, pObj->szImage);
		gi.nFrame = pObj->nFrame;
		g_pRepresent->DrawPrimitivesOnImage(1, &gi, RU_T_IMAGE,
				m_pPrerenderGroundImg->szImage, m_pPrerenderGroundImg->uImage,
				m_pPrerenderGroundImg->nImagePosition);
		gi.uImage = 0;
		gi.szImage[0] = 0;
		gi.nImagePosition = IMAGE_IS_POSITION_INIT;
	}

	return true;
}*/

void KScenePlaceRegionC::SetNestRegion(KScenePlaceRegionC* pNest)
{
	if (pNest == NULL || pNest == this)
		return;

	int		nDestX = pNest->m_RegionIndex.x - m_RegionIndex.x;
	int		nDestY = pNest->m_RegionIndex.y - m_RegionIndex.y;

	if (nDestX * nDestX > 1 || nDestY * nDestY > 1)
		return;	//����������

	unsigned char cRelate = (nDestX * 3 + nDestY + 4);
	unsigned int  i = RWP_NEST_REGION_0 << cRelate;
	if (m_Flag & i)
		return;
	m_Flag |= i;

	RECT	rcthis;//ÿ������Ĵ�С �� ��ǰ��ƫ��
	rcthis.left  = m_LeftTopCornerScenePos.x;
	rcthis.right = m_LeftTopCornerScenePos.x + RWPP_AREGION_WIDTH;
	rcthis.top   = m_LeftTopCornerScenePos.y;
	rcthis.bottom = m_LeftTopCornerScenePos.y +RWPP_AREGION_HEIGHT;

	KSPRCoverGroundObj*	pObj = pNest->m_GroundLayerData.pObjects;
	int		nCount = 0;
	for (i = 0; i < pNest->m_GroundLayerData.uNumObject; ++i, ++pObj)
	{
		if (pObj->bRelateRegion == OBJ_IS_SELF_OWNED &&
			pObj->nPositionX < rcthis.right && pObj->nPositionX + pObj->nWidth > rcthis.left &&
			pObj->nPositionY < rcthis.bottom &&
			pObj->nPositionY + pObj->nHeight + pObj->nHeight > rcthis.top)
		{
			pObj->bRelateRegion = OBJ_IS_COPY_FROM_NEST_REGION;
			nCount++;
		}
	}
	if (nCount == 0)
		return;

	pObj = (KSPRCoverGroundObj*)realloc(m_GroundLayerData.pObjects,
		sizeof(KSPRCoverGroundObj) * (m_GroundLayerData.uNumObject + nCount));
	if (pObj == NULL)
		return;
	m_GroundLayerData.pObjects = pObj;
	pObj = pNest->m_GroundLayerData.pObjects;
	for (i = 0; i < pNest->m_GroundLayerData.uNumObject; ++i, ++pObj)
	{
		if (pObj->bRelateRegion == OBJ_IS_COPY_FROM_NEST_REGION)
		{
			pObj->bRelateRegion = cRelate;
			AddGrundCoverObj(pObj);
			pObj->bRelateRegion = OBJ_IS_SELF_OWNED;
		}
	}
	//�������ر�ͼΪδ����Ⱦ����Ҫ������Ⱦ
	if (m_pPrerenderGroundImg)
		m_pPrerenderGroundImg->GROUND_IMG_OK_FLAG = false;
}


//##ModelId=3DB9001D00BD
bool KScenePlaceRegionC::LoadGroundLayer(KPakFile* pDataFile, unsigned int uSize)
{
	//_ASSERT(pDataFile);
	if  (!pDataFile) return false;

	KGroundFileHead	Head = { 0 };
	unsigned int uStartPos = pDataFile->Tell();
	pDataFile->Read(&Head, sizeof(KGroundFileHead));
	if (Head.uNumGrunode)  //�ر�ͼ����Ŀ�������0 ��
	{
		int nSize = Head.uObjectDataOffset - sizeof(KGroundFileHead);
		m_GroundLayerData.pGrunodes = (KSPRCrunode*)malloc(nSize);
		if (m_GroundLayerData.pGrunodes)
		{
			pDataFile->Read(m_GroundLayerData.pGrunodes, nSize);
			m_GroundLayerData.uNumGrunode = Head.uNumGrunode;
		}
		else
			return false;
	}
	if (Head.uNumObject)
	{
		uSize -= Head.uObjectDataOffset;
		m_GroundLayerData.pObjects = (KSPRCoverGroundObj*)malloc(uSize);
		if (m_GroundLayerData.pObjects)
		{
			pDataFile->Seek(Head.uObjectDataOffset + uStartPos, FILE_BEGIN);
			pDataFile->Read(m_GroundLayerData.pObjects, uSize);
			m_GroundLayerData.uNumObject = Head.uNumObject;
			for (unsigned int i = 0; i < Head.uNumObject; ++i)
				m_GroundLayerData.pObjects[i].bRelateRegion = OBJ_IS_SELF_OWNED;
		}
		else
			return false;
	}
	return true;
}
//���� ����uNumObject ����
void KScenePlaceRegionC::AddGrundCoverObj(KSPRCoverGroundObj* pObj)
{
//	_ASSERT(pObj);
	if  (pObj==NULL) return;
	int nDest = m_GroundLayerData.uNumObject;//
	while(nDest > 0)
	{
		KSPRCoverGroundObj* pSelfObj = &m_GroundLayerData.pObjects[nDest - 1];
		if (pSelfObj->bOrder >  pObj->bOrder)
			nDest --;
		else if (pSelfObj->bOrder == pObj->bOrder)
		{
			if(pSelfObj->nLayer >  pObj->nLayer)
				nDest --;
			else if (pSelfObj->nLayer <  pObj->nLayer)
				break;
			else
			{
				while(nDest > 0)
				{
					pSelfObj = &m_GroundLayerData.pObjects[nDest - 1];
					if (pSelfObj->bOrder == pObj->bOrder &&
						pSelfObj->nLayer == pObj->nLayer &&
						(pSelfObj->nPositionY > pObj->nPositionY ||
							(pSelfObj->nPositionY == pObj->nPositionY &&
							 pSelfObj->bRelateRegion > pObj->bRelateRegion)))
					{
						nDest--;
					}
					else
						break;
				}
				break;
			}
		}
		else
			break;
	};
	for (int i = m_GroundLayerData.uNumObject; i > nDest; i--)
		m_GroundLayerData.pObjects[i] = m_GroundLayerData.pObjects[i - 1];
	m_GroundLayerData.pObjects[nDest] = *pObj;
	m_GroundLayerData.uNumObject++;
}

//##ModelId=3DB90015018D
void KScenePlaceRegionC::LoadObstacle(KPakFile* pDataFile, unsigned int uSize)
{//�����ϰ�����
	if (!pDataFile) return;

	long m_ObstacleInfo[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];

	ZeroStruct(m_ObstacleInfo);

	m_cObstacle.Clear();

	if (pDataFile && uSize >= sizeof(m_ObstacleInfo))
	{
		pDataFile->Read((LPVOID)m_ObstacleInfo, sizeof(m_ObstacleInfo));

		for (INT i = 0; i < REGION_GRID_WIDTH; ++i)
		{
			for (INT j = 0; j < REGION_GRID_HEIGHT; ++j)
			{
				if (m_ObstacleInfo[i][j])
					m_cObstacle.SetData(i, j, m_ObstacleInfo[i][j]);
			}
		}
	}
	else
		//ZeroMemory(m_ObstacleInfo, sizeof(m_ObstacleInfo));
		m_cObstacle.Clear();
}

void KScenePlaceRegionC::LoadTrap(KPakFile* pDataFile, unsigned int uSize)
{   //����trap����
	//_ASSERT(pDataFile);
	if  (!pDataFile) return;

	KTrapFileHead	sTrapFileHead = { 0 };
	KSPTrap			sTrapCell;

	pDataFile->Read(&sTrapFileHead, sizeof(KTrapFileHead));

	//long m_TrapInfo[RWP_NUM_GROUND_CELL_H][RWP_NUM_GROUND_CELL_V * 2];

	//ZeroStruct(m_TrapInfo);

	m_cTrap.Clear();

	for (DWORD i = 0; i < sTrapFileHead.uNumTrap; ++i)
	{
		pDataFile->Read(&sTrapCell, sizeof(KSPTrap));
		if (sTrapCell.cY >= RWP_NUM_GROUND_CELL_V * 2 || sTrapCell.cX + sTrapCell.cNumCell - 1 >= RWP_NUM_GROUND_CELL_H)
			continue;

		for (int j = 0; j < sTrapCell.cNumCell; ++j)
		{
			//m_TrapInfo[sTrapCell.cX + j][sTrapCell.cY] = sTrapCell.uTrapId;

			m_cTrap.SetData(sTrapCell.cX + j,sTrapCell.cY,sTrapCell.uTrapId);

		}
	}
}

//##ModelId=3DBDADE00001
bool KScenePlaceRegionC::LoadAboveGroundObjects(KPakFile* pDataFile, unsigned int uSize)
{
	//CCAssert(pDataFile,"");
	if  (!pDataFile) return false;
	pDataFile->Read(&m_BiosData.Numbers, sizeof(KBuildinObjFileHead));
	if (uSize < m_BiosData.Numbers.nNumBios * sizeof(KBuildinObj) +
		m_BiosData.Numbers.nNumBulidinLight * sizeof(KBuildInLightInfo) +
		sizeof(KBuildinObjFileHead))
	{
		memset(&m_BiosData.Numbers, 0, sizeof(KBuildinObjFileHead));
		return false;
	}

	int nSize, nRead;
	if (m_BiosData.Numbers.nNumBios)
	{
		nSize = sizeof(KBuildinObj) * m_BiosData.Numbers.nNumBios;
		m_BiosData.pBios = (KBuildinObj*)malloc(nSize);
		if (m_BiosData.pBios == NULL)
		{
			memset(&m_BiosData.Numbers, 0, sizeof(KBuildinObjFileHead));
			return false;
		}

		nRead = pDataFile->Read(m_BiosData.pBios, nSize);
		if (nRead < nSize)
			memset(((char*)m_BiosData.pBios) + nRead, 0, nSize - nRead);

		for (unsigned int i = m_BiosData.Numbers.nNumBios - m_BiosData.Numbers.nNumBiosAbove;
				i < m_BiosData.Numbers.nNumBios; ++i)
		{
			m_BiosData.pBios[i].OBJ_IMAGE_ID = 0;
			m_BiosData.pBios[i].OBJ_IMAGE_ISPOSITION = -1;
			if (m_BiosData.pBios[i].nAniSpeed)
				m_BiosData.pBios[i].nAniSpeed = 1;
//			if(!isLoading()) {
//				getSPR(m_BiosData.pBios[i].szImage, m_BiosData.pBios[i].nImgNumFrames, false);
//				releaseSPR();
//			}
		}
	}

	if (m_BiosData.Numbers.nNumBulidinLight)
	{
		nSize = sizeof(KBuildInLightInfo) * m_BiosData.Numbers.nNumBulidinLight;
		m_BiosData.pLights = (KBuildInLightInfo*)malloc(nSize);
		if (m_BiosData.pLights == NULL)
		{
			m_BiosData.Numbers.nNumBulidinLight = 0;
			return false;
		}

		nRead = pDataFile->Read(m_BiosData.pLights, nSize);
		if (nRead < nSize)
			memset(((char*)m_BiosData.pLights) + nRead, 0, nSize - nRead);
	}
	return true;
}


void KScenePlaceRegionC::PaintGroundDirect_Part(int bRegionIndex)
{//��Ͳ� ����
	if (g_GameWorld == NULL)
		return;

	unsigned int nIndex;
	int			CellWidth  = RWPP_AREGION_WIDTH/RWP_NUM_GROUND_CELL_H;   //32
	int			CellHeight = RWPP_AREGION_HEIGHT/2/RWP_NUM_GROUND_CELL_V;//32
	//----�����ر���Ķ���:·���----
	KRUImagePart	ImgPart[LOCAL_MAX_IMG_NUM];
	KRUImagePart	*pGp;

	memset(&ImgPart, 0, sizeof(ImgPart));
	int nPartNum = 0;
	pGp = &ImgPart[0];

	POINT RegionRBPos;//ʵ�ʻ滭��ÿ������Ĵ�СΪ(512x512) ��������(��������)
	RegionRBPos.x = m_LeftTopCornerScenePos.x + RWPP_AREGION_WIDTH;
	RegionRBPos.y = m_LeftTopCornerScenePos.y + RWPP_AREGION_HEIGHT;

	KSPRCoverGroundObj* pObj = m_GroundLayerData.pObjects;
	for (nIndex = 0; nIndex < m_GroundLayerData.uNumObject; ++nIndex, ++pObj)
	{
		//memset(&ImgList, 0, sizeof(ImgList));
		if (pObj->bRelateRegion == OBJ_IS_SELF_OWNED &&
			pObj->nPositionX >= m_LeftTopCornerScenePos.x &&
			pObj->nPositionY >= m_LeftTopCornerScenePos.y &&
			pObj->nPositionX + pObj->nWidth < RegionRBPos.x&&
			pObj->nPositionY + pObj->nHeight * 2 < RegionRBPos.y)
		{//�����Ĳ���
		}
		else
		{//��Ҫ���еĲ���
			pGp->Color.Color_dw = 0;
			pGp->bRenderFlag  = RUIMAGE_RENDER_FLAG_FRAME_DRAW;
			pGp->bRenderStyle = IMAGE_RENDER_STYLE_3LEVEL;//IMAGE_RENDER_STYLE_3LEVEL;//IMAGE_RENDER_STYLE_ALPHA;//IMAGE_RENDER_STYLE_3LEVEL;
			pGp->nType = ISI_T_SPR;
			pGp->oPosition.nZ = 0;
			pGp->oEndPos.nZ = 0;
			pGp->szImage[0] = 0;

			strcpy(pGp->szImage, pObj->szImage);
			pGp->nFrame = pObj->nFrame;
			pGp->uImage = 0;
			pGp->nISPosition = -1;

			if (pObj->nPositionX < m_LeftTopCornerScenePos.x)
				pGp->oImgLTPos.nX = m_LeftTopCornerScenePos.x - pObj->nPositionX;
			else
				pGp->oImgLTPos.nX = 0;

			if (pObj->nPositionX + pObj->nWidth >= RegionRBPos.x)
				pGp->oImgRBPos.nX = RegionRBPos.x - pObj->nPositionX;
			else
				pGp->oImgRBPos.nX = pObj->nWidth;

			if (pObj->nPositionY < m_LeftTopCornerScenePos.y)
				pGp->oImgLTPos.nY = (m_LeftTopCornerScenePos.y - pObj->nPositionY) / 2;
			else
				pGp->oImgLTPos.nY = 0;

			if (pObj->nPositionY + pObj->nHeight * 2 >= RegionRBPos.y)
				pGp->oImgRBPos.nY = (RegionRBPos.y - pObj->nPositionY) / 2;
			else
				pGp->oImgRBPos.nY = pObj->nHeight;

			pGp->oPosition.nX = pObj->nPositionX + pGp->oImgLTPos.nX;
			pGp->oPosition.nY = pObj->nPositionY + pGp->oImgLTPos.nY * 2;

			pGp->oEndPos.nX = pObj->nPositionX + pGp->oImgRBPos.nX;
			pGp->oEndPos.nY = pObj->nPositionY + pGp->oImgRBPos.nY * 2;

			if (pGp->szImage[0] && nPartNum<LOCAL_MAX_IMG_NUM)
			{
				nPartNum++;
				pGp++;
			}
		}
	}//end for

	POINT m_curRegPoint;
	m_curRegPoint.x = m_RegionIndex.x;//9������ı� ����ǲ��������
	m_curRegPoint.y = m_RegionIndex.y;//9������ı� ����ǲ��������

	if (nPartNum) //Draw_Part_map
		g_GameWorld->Draw_Part_map(bRegionIndex+1,nPartNum,&ImgPart[0],nIndex+1,MAP_NODE_PART,0,false,m_curRegPoint,RegionRBPos);

	char nKey[32];
	sprintf(nKey,"%d-%d-part",m_curRegPoint.x,m_curRegPoint.y);
	std::string nCurKey = nKey;
	if  (nPartNum<=0)
	{
		(g_GameWorld->ParentNode_map)->removeChildByName(nCurKey,true);
		/*Node *nRegNode         = (g_GameWorld->ParentNode_map)->getChildByName(nCurKey);
		if (!nRegNode)  return;
		Node *nOnePartNode     = nRegNode->getChildByTag(MAP_NODE_PART);

		if (nOnePartNode)
			nOnePartNode->removeAllChildren();*/
	}
}

void KScenePlaceRegionC::PaintGroundDirect_full(int bRegionIndex)
{//��Ͳ� ����
	if (g_GameWorld == NULL)
		return;

	KRUImage	ImgList[LOCAL_FULL_IMG_NUM];
	KRUImage	*pGi;
	unsigned int nIndex;
	int			CellWidth  = RWPP_AREGION_WIDTH/RWP_NUM_GROUND_CELL_H;   //32
	int			CellHeight = RWPP_AREGION_HEIGHT/2/RWP_NUM_GROUND_CELL_V;//32
	int m_isClear = 0;
	memset(&ImgList, 0, sizeof(ImgList));
	int nNum = 0;
	int nCaodiSprCount = 0;
	pGi = &ImgList[0];
	//----����ױ���:�ݵء�ˮ��----
	KSPRCrunode* pGrunode = m_GroundLayerData.pGrunodes;
	for (nIndex = 0; nIndex < m_GroundLayerData.uNumGrunode; ++nIndex)
	{//һ���Զ���ر�����
		//memset(&ImgList, 0, sizeof(ImgList));
		pGi->bRenderStyle =IMAGE_RENDER_STYLE_OPACITY;//IMAGE_RENDER_STYLE_3LEVEL;// IMAGE_RENDER_STYLE_OPACITY;
		pGi->nType = ISI_T_SPR;
		pGi->oPosition.nX = pGrunode->Param.h * CellWidth + m_LeftTopCornerScenePos.x;       //��������
		pGi->oPosition.nY = (pGrunode->Param.v * CellHeight) * 2 + m_LeftTopCornerScenePos.y;//��������
		memcpy(pGi->szImage, pGrunode->szImgName, pGrunode->Param.nFileNameLen);
		pGi->nFrame = pGrunode->Param.nFrame;//�ڼ�֡��
		pGi->uImage = 0;
		pGi->nISPosition = IMAGE_IS_POSITION_INIT;
		pGrunode = (KSPRCrunode*)(((char*)pGrunode) + sizeof(KSPRCrunode::KSPRCrunodeParam) + pGrunode->Param.nFileNameLen);//ƫ�Ƶ����λ��

		if  (pGi->szImage[0] && nNum < LOCAL_MAX_IMG_NUM)
		{
			nNum++;
			pGi++;
		}
	}//end for

	POINT RegionRBPos;//ʵ�ʻ滭��ÿ������Ĵ�СΪ(512x512) ��������(��������)
	RegionRBPos.x = m_LeftTopCornerScenePos.x + RWPP_AREGION_WIDTH;
	RegionRBPos.y = m_LeftTopCornerScenePos.y + RWPP_AREGION_HEIGHT;
	KSPRCoverGroundObj* pObj = m_GroundLayerData.pObjects;
	for (nIndex = 0; nIndex < m_GroundLayerData.uNumObject; ++nIndex, ++pObj)
	{
		//memset(&ImgList, 0, sizeof(ImgList));
		if (pObj->bRelateRegion == OBJ_IS_SELF_OWNED &&
			pObj->nPositionX >= m_LeftTopCornerScenePos.x &&
			pObj->nPositionY >= m_LeftTopCornerScenePos.y &&
			pObj->nPositionX + pObj->nWidth < RegionRBPos.x&&
			pObj->nPositionY + pObj->nHeight * 2 < RegionRBPos.y)
		{//�����Ĳ���
			pGi->bRenderStyle = IMAGE_RENDER_STYLE_3LEVEL;//;//IMAGE_RENDER_STYLE_3LEVEL;//IMAGE_RENDER_STYLE_ALPHA;//IMAGE_RENDER_STYLE_3LEVEL;
			pGi->nType = ISI_T_SPR;
			pGi->bRenderFlag = RUIMAGE_RENDER_FLAG_FRAME_DRAW;
			pGi->Color.Color_b.a = 125;  //125 pGi->Color.Color_b.a = 255;
			pGi->oPosition.nX = pObj->nPositionX;
			pGi->oPosition.nY = pObj->nPositionY;
			strcpy(pGi->szImage, pObj->szImage);
			pGi->nFrame = pObj->nFrame;
			pGi->uImage = 0;
			pGi->nISPosition = IMAGE_IS_POSITION_INIT;

			if (pGi->szImage[0] && nNum < LOCAL_MAX_IMG_NUM)
			{
				nNum++;
				pGi++;
			}
		}
	}//end for

	POINT m_curRegPoint;
	m_curRegPoint.x = m_RegionIndex.x;//9������ı� ����ǲ��������
	m_curRegPoint.y = m_RegionIndex.y;//9������ı� ����ǲ��������

	if (nNum)
		g_GameWorld->_DrawPrimitives_map(bRegionIndex+1,nNum,&ImgList[0],nIndex+1,MAP_NODE_LUMIAN_A,0,false,m_curRegPoint,RegionRBPos);

	char nKey[32];
	sprintf(nKey,"%d-%d-dimian",m_curRegPoint.x,m_curRegPoint.y);
	std::string nCurKey = nKey;

	if  (nNum<=0)
	{
		g_GameWorld->ParentNode_map->removeChildByName(nCurKey,true);
		/*Node *nRegNode    = (g_GameWorld->ParentNode_map)->getChildByName(nCurKey);
		if (!nRegNode)  return;

		Node *nPartNode   = nRegNode->getChildByTag(MAP_NODE_LUMIAN_A);
		if (nPartNode)
			nPartNode->removeAllChildren();*/
	}
	//if (nNum)
	//g_GameWorld->DrawPrimitives_map(bRegionIndex,nNum, &ImgList[0], RU_T_IMAGE,MAP_NODE_OTHER,0, false);
}

void KScenePlaceRegionC::PaintGroundDirect(int bRegionIndex)
{//��Ͳ� ����
	if (g_GameWorld == NULL)
		return;

	KRUImage	ImgList[LOCAL_MAX_IMG_NUM];
	KRUImage	*pGi;
	unsigned int nIndex;
	int			CellWidth  = RWPP_AREGION_WIDTH/RWP_NUM_GROUND_CELL_H;   //32
	int			CellHeight = RWPP_AREGION_HEIGHT/2/RWP_NUM_GROUND_CELL_V;//32
	int m_isClear = 0;
	memset(&ImgList, 0, sizeof(ImgList));
	int nNum = 0;
	int nCaodiSprCount = 0;
	pGi = &ImgList[0];
	//----����ױ���:�ݵء�ˮ��----
	KSPRCrunode* pGrunode = m_GroundLayerData.pGrunodes;
	for (nIndex = 0; nIndex < m_GroundLayerData.uNumGrunode; ++nIndex)
	{//һ���Զ���ر�����
		//memset(&ImgList, 0, sizeof(ImgList));
		pGi->bRenderStyle =IMAGE_RENDER_STYLE_OPACITY;//IMAGE_RENDER_STYLE_3LEVEL;// IMAGE_RENDER_STYLE_OPACITY;
		pGi->nType = ISI_T_SPR;
		pGi->oPosition.nX = pGrunode->Param.h * CellWidth + m_LeftTopCornerScenePos.x;       //��������
		pGi->oPosition.nY = (pGrunode->Param.v * CellHeight) * 2 + m_LeftTopCornerScenePos.y;//��������
		memcpy(pGi->szImage, pGrunode->szImgName, pGrunode->Param.nFileNameLen);
		pGi->nFrame = pGrunode->Param.nFrame;//�ڼ�֡��
		pGi->uImage = 0;
		pGi->nISPosition = IMAGE_IS_POSITION_INIT;
		pGrunode = (KSPRCrunode*)(((char*)pGrunode) + sizeof(KSPRCrunode::KSPRCrunodeParam) + pGrunode->Param.nFileNameLen);//ƫ�Ƶ����λ��

		if  (pGi->szImage[0] && nNum < LOCAL_MAX_IMG_NUM)
		{
		   nNum++;
		   pGi++;
	    }
	}//end for

	//----�����ر���Ķ���:·���----
	//memset(&ImgList, 0, sizeof(ImgList));
	KRUImagePart	ImgPart[LOCAL_MAX_IMG_NUM];
	KRUImagePart	*pGp;
	//KRUImagePart	ImgPart;
	memset(&ImgPart, 0, sizeof(ImgPart));
	int nPartNum = 0;
	pGp = &ImgPart[0];

	POINT RegionRBPos;//ʵ�ʻ滭��ÿ������Ĵ�СΪ(512x512) ��������(��������)
	RegionRBPos.x = m_LeftTopCornerScenePos.x + RWPP_AREGION_WIDTH;
	RegionRBPos.y = m_LeftTopCornerScenePos.y + RWPP_AREGION_HEIGHT;
	int nSprCount = 0;
	int nPartSprCount=0;
	KSPRCoverGroundObj* pObj = m_GroundLayerData.pObjects;
	for (nIndex = 0; nIndex < m_GroundLayerData.uNumObject; ++nIndex, ++pObj)
	{
		//memset(&ImgList, 0, sizeof(ImgList));
		if (pObj->bRelateRegion == OBJ_IS_SELF_OWNED &&
			pObj->nPositionX >= m_LeftTopCornerScenePos.x &&
			pObj->nPositionY >= m_LeftTopCornerScenePos.y &&
			pObj->nPositionX + pObj->nWidth < RegionRBPos.x&&
			pObj->nPositionY + pObj->nHeight * 2 < RegionRBPos.y)
		{//�����Ĳ���
			pGi->bRenderStyle = IMAGE_RENDER_STYLE_3LEVEL;//;//IMAGE_RENDER_STYLE_3LEVEL;//IMAGE_RENDER_STYLE_ALPHA;//IMAGE_RENDER_STYLE_3LEVEL;
			pGi->nType = ISI_T_SPR;
			pGi->bRenderFlag = RUIMAGE_RENDER_FLAG_FRAME_DRAW;
			pGi->Color.Color_b.a = 125;  //125 pGi->Color.Color_b.a = 255;
			pGi->oPosition.nX = pObj->nPositionX;
			pGi->oPosition.nY = pObj->nPositionY;
			strcpy(pGi->szImage, pObj->szImage);
			pGi->nFrame = pObj->nFrame;
			pGi->uImage = 0;
			pGi->nISPosition = IMAGE_IS_POSITION_INIT;

		    if (pGi->szImage[0] && nNum < LOCAL_MAX_IMG_NUM)
			{
			  nNum++;
			  pGi++;
		    }
		}
		else
		{//��Ҫ���еĲ���

			pGp->Color.Color_dw = 0;
			pGp->bRenderFlag = RUIMAGE_RENDER_FLAG_FRAME_DRAW;
			pGp->bRenderStyle = IMAGE_RENDER_STYLE_3LEVEL;//IMAGE_RENDER_STYLE_3LEVEL;//IMAGE_RENDER_STYLE_ALPHA;//IMAGE_RENDER_STYLE_3LEVEL;
			pGp->nType = ISI_T_SPR;
			pGp->oPosition.nZ = 0;
			pGp->oEndPos.nZ = 0;
			pGp->szImage[0] = 0;


			strcpy(pGp->szImage, pObj->szImage);
			pGp->nFrame = pObj->nFrame;
			pGp->uImage = 0;
			pGp->nISPosition = -1;

			if (pObj->nPositionX < m_LeftTopCornerScenePos.x)
				pGp->oImgLTPos.nX = m_LeftTopCornerScenePos.x - pObj->nPositionX;
			else
				pGp->oImgLTPos.nX = 0;

			if (pObj->nPositionX + pObj->nWidth >= RegionRBPos.x)
				pGp->oImgRBPos.nX = RegionRBPos.x - pObj->nPositionX;
			else
				pGp->oImgRBPos.nX = pObj->nWidth;

			if (pObj->nPositionY < m_LeftTopCornerScenePos.y)
				pGp->oImgLTPos.nY = (m_LeftTopCornerScenePos.y - pObj->nPositionY) / 2;
			else
				pGp->oImgLTPos.nY = 0;

			if (pObj->nPositionY + pObj->nHeight * 2 >= RegionRBPos.y)
				pGp->oImgRBPos.nY = (RegionRBPos.y - pObj->nPositionY) / 2;
			else
				pGp->oImgRBPos.nY = pObj->nHeight;

			pGp->oPosition.nX = pObj->nPositionX + pGp->oImgLTPos.nX;
			pGp->oPosition.nY = pObj->nPositionY + pGp->oImgLTPos.nY * 2;

			pGp->oEndPos.nX = pObj->nPositionX + pGp->oImgRBPos.nX;
			pGp->oEndPos.nY = pObj->nPositionY + pGp->oImgRBPos.nY * 2;

			if (pGp->szImage[0] && nPartNum < LOCAL_MAX_IMG_NUM)
			{
				nPartNum++;
				pGp++;
			}

		}//end if
	}//end for

	POINT m_curRegPoint;
	m_curRegPoint.x = m_RegionIndex.x;//9������ı� ����ǲ��������
	m_curRegPoint.y = m_RegionIndex.y;//9������ı� ����ǲ��������

	if (nNum)
	   g_GameWorld->_DrawPrimitives_map(bRegionIndex+1,nNum,&ImgList[0],nIndex+1,MAP_NODE_LUMIAN_A,0,false,m_curRegPoint,RegionRBPos);

	if (nPartNum) //Draw_Part_map
		g_GameWorld->Draw_Part_map(bRegionIndex+1,nPartNum,&ImgPart[0],nIndex+1,MAP_NODE_PART,0,false,m_curRegPoint,RegionRBPos);

	   char nKey[32];
	   sprintf(nKey,"%d-%d-dimian",m_curRegPoint.x,m_curRegPoint.y);
	   std::string nFullKey = nKey;
	   if  (nNum<=0)
	   {
		   (g_GameWorld->ParentNode_map)->removeChildByName(nFullKey,true);
		   /*Node *nRegNode    = (g_GameWorld->ParentNode_map)->getChildByName(nCurKey);
		   if (!nRegNode)  return;

		   Node *nPartNode   = nRegNode->getChildByTag(MAP_NODE_LUMIAN_A);
			if (nPartNode)
			    nPartNode->removeAllChildren();*/
	   }
	   sprintf(nKey,"%d-%d-part",m_curRegPoint.x,m_curRegPoint.y);
	   std::string nPartKey = nKey;
	   if  (nPartNum<=0)
	   {
		    (g_GameWorld->ParentNode_map)->removeChildByName(nPartKey,true);
		   /*Node *nRegNode         = (g_GameWorld->ParentNode_map)->getChildByName(nCurKey);
		   if (!nRegNode)  return;
		   Node *nOnePartNode     = nRegNode->getChildByTag(MAP_NODE_PART);

		   if (nOnePartNode)
			   nOnePartNode->removeAllChildren();*/
	   }
	//if (nNum)
		//g_GameWorld->DrawPrimitives_map(bRegionIndex,nNum, &ImgList[0], RU_T_IMAGE,MAP_NODE_OTHER,0, false);
}

void KScenePlaceRegionC::ReomeGroundObj(int bRegionIndex)
{
	if (g_GameWorld == NULL)
		return;
	Node *nRegNode    = (g_GameWorld->ParentNode_map)->getChildByTag(bRegionIndex+1);
	Node *nLuMianNode = NULL;
	int  nLuMianSprCount= 0;

	if (nRegNode)
	{
		nLuMianNode = nRegNode->getChildByTag(MAP_NODE_LUMIAN);
		if (nLuMianNode)
			nLuMianSprCount = nLuMianNode->getChildrenCount();
	}
	int  m_isClear=0;
	if  (m_isClear<=0)
	{
		//�պõ����� �ͼ�⵽ �Ѿ�����������Ѿ�����˽ڵ� ��������
		//if  (nLuMianNode && nLuMianSprCount)
			//nLuMianNode->removeAllChildren();
	}
}
//##ModelId=3DDBD8C80309
void KScenePlaceRegionC::PaintGround(int bRegionIndex,int fullorpart)
{
	//if (m_pPrerenderGroundImg && bPrerenderGroundImg)
		//g_pRepresent->DrawPrimitives(1, m_pPrerenderGroundImg, RU_T_IMAGE, false);
	//else
	    if  (fullorpart)
		    PaintGroundDirect_full(bRegionIndex);
		else
			PaintGroundDirect_Part(bRegionIndex);
}

void KScenePlaceRegionC::PaintBuildinObj(RECT* pRepresentArea,int bRegionIndex,bool isClear)
{
	    if (!g_GameWorld)
		   return;
	    if (pRepresentArea==NULL || isClear)
		{//ɾ���ڵ㾫��
			if (m_ClearState==0)
			{
				m_ClearState =1;//�����Ѿ�ɾ��
				char nRegKey[32]={0},nKey[32]={0};
				sprintf(nRegKey,"%d-%d",m_RegionIndex.x,m_RegionIndex.y);
				std::string nCurRegKey = nRegKey;
				(g_GameWorld->ParentNode_Bigmap)->removeChildByName(nCurRegKey,true);
				//Node *nRegNode = (g_GameWorld->ParentNode_Bigmap)->getChildByName(nCurRegKey);
				//if (nRegNode)
					//nRegNode->removeAllChildren();
			}

			return;
		}
		m_ClearState = 0;
	   //pLeaf = (KIpotLeaf*)m_BiosData.pLeafs;
	  // (m_BiosData.pLeafs)->PaintABuildinObject(pRepresentArea,bRegionIndex);
	   //���������ڽ���
		if  (m_BiosData.pBios==NULL || m_BiosData.pLeafs==NULL) return;

		KIpotBuildinObj* pLeaf = NULL;
		//KBuildinObj*	 pObj  = NULL;

		unsigned int nCount = m_BiosData.Numbers.nNumBios - m_BiosData.Numbers.nNumBiosAbove; //��ȥ�߿���Ŀ

		pLeaf = m_BiosData.pLeafs;
		//pObj  = m_BiosData.pBios;
		if  (nCount<0)
			return;

#define BIG_MAX_IMG_NUM 100

		KRUImage4	ImgListTrees[BIG_MAX_IMG_NUM];
		KRUImage4	*pGtree;
		memset(&ImgListTrees, 0, sizeof(ImgListTrees));
		int nTreeNum = 0;
		pGtree = &ImgListTrees[0];

		KRUImage4	ImgListHouse[BIG_MAX_IMG_NUM];
		KRUImage4	*pGhouse;
		memset(&ImgListHouse, 0, sizeof(ImgListHouse));
		int nHouseNum = 0;
		pGhouse = &ImgListHouse[0];

		int i;
		for (i = 0; i < nCount; ++i, ++pLeaf) //++pLeaf,
		{
			if (!pLeaf) continue;

			/*KRUImage4	Image;
			Image.Color.Color_dw = 0;
			Image.nType          = ISI_T_SPR;
			Image.nFrame         = (pLeaf->pBio)->nFrame;
			Image.nISPosition    = -1;     //(m_BiosData.pLeafs)->nISPosition;
			strcpy(Image.szImage, (pLeaf->pBio)->szImage);
			Image.uImage         = 0;      //(pLeaf)->uImage;
			*/
			pGtree->Color.Color_dw = 0;
			pGtree->nType          = ISI_T_SPR;
			pGtree->nFrame         = (pLeaf->pBio)->nFrame;
			pGtree->nISPosition    = pLeaf->nISPosition;
			strcpy(pGtree->szImage, (pLeaf->pBio)->szImage);
			pGtree->uImage         = pLeaf->uImage;

			pGhouse->Color.Color_dw = 0;
			pGhouse->nType          = ISI_T_SPR;
			pGhouse->nFrame         = (pLeaf->pBio)->nFrame;
			pGhouse->nISPosition    = pLeaf->nISPosition;
			strcpy(pGhouse->szImage, (pLeaf->pBio)->szImage);
			pGhouse->uImage         = pLeaf->uImage;


			if (((pLeaf->pBio)->Props & SPBIO_F_HIGHT_LIGHT) == 0)
			{
				if ((pLeaf->pBio)->nImgWidth > 128 && (pLeaf->pBio)->nImgHeight > 128)
				{
					//Image.bRenderStyle = IMAGE_RENDER_STYLE_3LEVEL;
					pGtree->bRenderStyle  = IMAGE_RENDER_STYLE_3LEVEL;
					pGhouse->bRenderStyle = IMAGE_RENDER_STYLE_3LEVEL;
				}
				else
				{
					//Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
					//Image.Color.Color_b.a = 255;
					pGtree->bRenderStyle     = IMAGE_RENDER_STYLE_ALPHA;
					pGtree->Color.Color_b.a  = 255;
					pGhouse->bRenderStyle    = IMAGE_RENDER_STYLE_ALPHA;
					pGhouse->Color.Color_b.a = 255;
				}
			}
			else
			{
				//Image.bRenderStyle = IMAGE_RENDER_STYLE_BORDER;
				pGtree->bRenderStyle  = IMAGE_RENDER_STYLE_BORDER;
				pGhouse->bRenderStyle = IMAGE_RENDER_STYLE_BORDER;
			}

			if ((pLeaf->pBio)->nAniSpeed == 0)
			{//��֡
				/*Image.bRenderFlag = RUIMAGE_RENDER_FLAG_FRAME_DRAW;
				Image.oPosition.nX = (pLeaf->pBio)->ImgPos1.x;
				Image.oPosition.nY = (pLeaf->pBio)->ImgPos1.y;
				Image.oPosition.nZ = (pLeaf->pBio)->ImgPos1.z;

				Image.oEndPos.nX = (pLeaf->pBio)->ImgPos3.x;
				Image.oEndPos.nY = (pLeaf->pBio)->ImgPos3.y;
				Image.oEndPos.nZ = (pLeaf->pBio)->ImgPos3.z;
				*/

				pGtree->bRenderFlag = RUIMAGE_RENDER_FLAG_FRAME_DRAW;
				pGtree->oPosition.nX = (pLeaf->pBio)->ImgPos1.x;
				pGtree->oPosition.nY = (pLeaf->pBio)->ImgPos1.y;
				pGtree->oPosition.nZ = (pLeaf->pBio)->ImgPos1.z;
				pGtree->oEndPos.nX = (pLeaf->pBio)->ImgPos3.x;
				pGtree->oEndPos.nY = (pLeaf->pBio)->ImgPos3.y;
				pGtree->oEndPos.nZ = (pLeaf->pBio)->ImgPos3.z;

				pGhouse->bRenderFlag = RUIMAGE_RENDER_FLAG_FRAME_DRAW;
				pGhouse->oPosition.nX = (pLeaf->pBio)->ImgPos1.x;
				pGhouse->oPosition.nY = (pLeaf->pBio)->ImgPos1.y;
				pGhouse->oPosition.nZ = (pLeaf->pBio)->ImgPos1.z;
				pGhouse->oEndPos.nX = (pLeaf->pBio)->ImgPos3.x;
				pGhouse->oEndPos.nY = (pLeaf->pBio)->ImgPos3.y;
				pGhouse->oEndPos.nZ = (pLeaf->pBio)->ImgPos3.z;

				if (((pLeaf->pBio)->Props & SPBIO_P_SORTMANNER_MASK) == SPBIO_P_SORTMANNER_POINT)
				{//��ľ���ݺ�һЩװ��Ʒ
					/*if (Image.oEndPos.nY >= pRepresentArea->top)
					{
						POINT nCurPoint;
						nCurPoint.x = m_RegionIndex.x;//Image.oPosition.nX/512;
						nCurPoint.y = m_RegionIndex.y;//(Image.oPosition.nY - 6)/1024;
						if (Image.szImage[0])
							g_GameWorld->DrawPrimitives_BigMap(bRegionIndex+1,1,&Image,0,MAP_NODE_TREES,0,false,nCurPoint);
					}*/
					if (pGtree->oEndPos.nY >= pRepresentArea->top)
					{
						if (pGtree->szImage[0])
						{
							nTreeNum++;
							pGtree++;
							//g_GameWorld->DrawPrimitives_BigMap(bRegionIndex+1,1,&Image,0,MAP_NODE_TREES,0,false,nCurPoint);
						}
					}
					//g_pRepresent->DrawPrimitives(1, &Image, RU_T_IMAGE, false)
				}
				else
				{
					if (pGhouse->oEndPos.nY > pRepresentArea->top && pLeaf->pBio->ImgPos3.x >= pRepresentArea->left && pLeaf->pBio->ImgPos4.x < pRepresentArea->right)
					{//���� ǽ��
						/*Image.oSecondPos.nX = (pLeaf->pBio)->ImgPos2.x;
						Image.oSecondPos.nY = (pLeaf->pBio)->ImgPos2.y;
						Image.oSecondPos.nZ = (pLeaf->pBio)->ImgPos2.z;

						Image.oThirdPos.nX = (pLeaf->pBio)->ImgPos4.x;
						Image.oThirdPos.nY = (pLeaf->pBio)->ImgPos4.y;
						Image.oThirdPos.nZ = (pLeaf->pBio)->ImgPos4.z;

						Image.oImgLTPos.nX = 0;
						Image.oImgLTPos.nY = 0;
						Image.oImgRBPos.nX = (pLeaf->pBio)->nImgWidth;
						Image.oImgRBPos.nY = (pLeaf->pBio)->nImgHeight;
						*/
						pGhouse->oSecondPos.nX = (pLeaf->pBio)->ImgPos2.x;
						pGhouse->oSecondPos.nY = (pLeaf->pBio)->ImgPos2.y;
						pGhouse->oSecondPos.nZ = (pLeaf->pBio)->ImgPos2.z;

						pGhouse->oThirdPos.nX = (pLeaf->pBio)->ImgPos4.x;
						pGhouse->oThirdPos.nY = (pLeaf->pBio)->ImgPos4.y;
						pGhouse->oThirdPos.nZ = (pLeaf->pBio)->ImgPos4.z;

						pGhouse->oImgLTPos.nX = 0;
						pGhouse->oImgLTPos.nY = 0;
						pGhouse->oImgRBPos.nX = (pLeaf->pBio)->nImgWidth;
						pGhouse->oImgRBPos.nY = (pLeaf->pBio)->nImgHeight;

						if (pGhouse->szImage[0])
						{
							nHouseNum++;
							pGhouse++;
							//g_GameWorld->DrawPrimitives_BigMap(bRegionIndex+1,1,&Image,0,MAP_NODE_HOUSE,0,false,nCurPoint);
						}
						//g_pRepresent->DrawPrimitives(1, &Image, RU_T_IMAGE_4, false);
					}
				}
			}
		}//end for
		POINT nCurPoint;
		nCurPoint.x = m_RegionIndex.x;//Image.oPosition.nX/512;
		nCurPoint.y = m_RegionIndex.y;//(Image.oPosition.nY - 6)/1024;

		//char msg[128];
		//sprintf(msg,"Reg:%d,nTNum:%d,nHNum:%d,RegX:%d,RegY:%d",bRegionIndex,nTreeNum,nHouseNum,nCurPoint.x,nCurPoint.y);
		//messageBox(msg,"PaintBuildinObj");

		if  (nTreeNum>0)//DrawPrimitives_BigMap_Tree
			g_GameWorld->DrawPrimitives_BigMap(bRegionIndex+1,nTreeNum,&ImgListTrees[0],0,MAP_NODE_TREES,0,false,nCurPoint);

		if  (nHouseNum>0)//DrawPrimitives_BigMap_house
			g_GameWorld->DrawPrimitives_BigMap(bRegionIndex+1,nHouseNum,&ImgListHouse[0],0,MAP_NODE_HOUSE,0,false,nCurPoint);

		char nRegKey[32]={0},nKey[32]={0};
		sprintf(nRegKey,"%d-%d",nCurPoint.x,nCurPoint.y);
		std::string nCurRegKey = nRegKey;

		Node *nRegNode = (g_GameWorld->ParentNode_Bigmap)->getChildByName(nCurRegKey);

		if (nRegNode && m_BackTreeNum>0 && nTreeNum>0 && m_BackTreeNum>nTreeNum)
		{

			Node *nPartNode = nRegNode->getChildByTag(MAP_NODE_TREES);
			if  (nPartNode)
			{
				for (int i=nTreeNum;i<m_BackTreeNum;i++)
				{
					sprintf(nKey,"tree-%d",i+1);
					std::string nCurKey = nKey;
					nPartNode->removeChildByName(nCurKey,true);
				}
			}
		}

		if (nRegNode &&  m_BackHouseNum>0 && nHouseNum>0 && m_BackHouseNum>nHouseNum)
		{
			Node *nPartNode = nRegNode->getChildByTag(MAP_NODE_HOUSE);
			if  (nPartNode)
			{
				for (int i=nHouseNum;i<m_BackHouseNum;i++)
				{
					sprintf(nKey,"house-%d",i+1);
					std::string nCurKey = nKey;
					nPartNode->removeChildByName(nCurKey,true);
				}
			}
		}
		m_BackTreeNum  = nTreeNum;
		m_BackHouseNum = nHouseNum;
}

//#ifdef SWORDONLINE_SHOW_DBUG_INFO
void KScenePlaceRegionC::PaintObstacle()
{//�滭�ϰ�
	/*KRULine	Line[4];
	int		nNumLine, i, j, nX, nY;

	for (i = 0; i < 4; ++i)
	{
		Line[i].Color.Color_dw = 0xffffffff;
		Line[i].oPosition.nZ = 0;
		Line[i].oEndPos.nZ = 0;
	}

	nX = m_LeftTopCornerScenePos.x;
	for (i = 0; i < 16; ++i, nX += RWP_OBSTACLE_WIDTH)
	{
		nY = m_LeftTopCornerScenePos.y;
		for (j = 0; j < 32; ++j, nY += RWP_OBSTACLE_HEIGHT)
		{
			long nTrap = m_cTrap.GetData(i,j);//m_TrapInfo[i][j];
			if (nTrap > 0)
			{//TRAP��
				char	szDebugTrap[32]={0};
				sprintf(szDebugTrap, "%x", nTrap);
				g_pRepresent->OutputText(14, szDebugTrap, KRF_ZERO_END, nX, nY, 0xffffffff, 0, 0);
			}

			int nIdx = SubWorld[0].FindRegion(MAKELONG(m_RegionIndex.x, m_RegionIndex.y));
			if (nIdx >= 0)
			{//�����ڵ�NPC
				int nRef = SubWorld[0].m_Region[nIdx].GetNpcRef(i,j);//SubWorld[0].m_Region[nIdx].GetRef(i,j,obj_npc);
				char szDebugNpcRef[64]={0};
				if (nRef)
				{
					sprintf(szDebugNpcRef, "NPC(%d/%d):%d,X:%d,Y:%d,��:%d,��:%d",nX/32/16,nY/32/32, nRef,i,j,nIdx,SubWorld[0].m_nTotalRegion);
					g_pRepresent->OutputText(14, szDebugNpcRef, KRF_ZERO_END, nX, nY, 0xffffffff, 0, 0);
				}

				nRef = SubWorld[0].m_Region[nIdx].GetObjRef(i,j);//SubWorld[0].m_Region[nIdx].GetRef(i,j,obj_object);
				//char	szDebugNpcRef[32];
				if (nRef)
				{
					sprintf(szDebugNpcRef, "object:%d,X:%d,Y:%d", nRef,i,j);
					g_pRepresent->OutputText(14, szDebugNpcRef, KRF_ZERO_END, nX, nY, 0xffffffff, 0, 0);

			}
		   if (g_iShowObstacle)
		   {//m_ObstacleInfo[i][j]
			long nType = (m_cObstacle.GetData(i,j) >> 4) & 0x0000000f;  //�ж��ϰ��� 16 32 * 16
			if (nType == Obstacle_Empty)
				continue;

			Line[2].oEndPos.nX = Line[1].oEndPos.nX = Line[0].oEndPos.nX =
				Line[2].oPosition.nX = Line[1].oPosition.nX = Line[0].oPosition.nX = nX;
			Line[2].oEndPos.nY = Line[1].oEndPos.nY = Line[0].oEndPos.nY =
				Line[2].oPosition.nY = Line[1].oPosition.nY = Line[0].oPosition.nY = nY;
			nNumLine = 0;
			switch(nType)
			{
			case Obstacle_Full:
				Line[0].oEndPos.nX      += 32;	//��
				Line[1].oEndPos.nY      += 32;	//��
				Line[2].oPosition.nX    += 32;	//��
				Line[2].oEndPos.nX      += 32;
				Line[2].oEndPos.nY      += 32;
				Line[3].oPosition.nX	= Line[0].oPosition.nX;	//��
				Line[3].oPosition.nY    = Line[2].oEndPos.nY;
				Line[3].oEndPos.nX      = Line[2].oEndPos.nX;
				Line[3].oEndPos.nY      = Line[2].oEndPos.nY;
				nNumLine = 4;
				break;
			case Obstacle_LT:
				Line[0].oEndPos.nX      += 32;	//��
				Line[1].oEndPos.nY      += 32;	//��
				Line[2].oPosition.nY    += 32;	//��б
				Line[2].oEndPos.nX      += 32;
				nNumLine = 3;
				break;
			case Obstacle_RT:
				Line[0].oEndPos.nX      += 32;	//��
				Line[1].oPosition.nX    += 32;	//��
				Line[1].oEndPos.nX      += 32;
				Line[1].oEndPos.nY      += 32;
				Line[2].oEndPos.nX      += 32;	//��б
				Line[2].oEndPos.nY      += 32;
				nNumLine = 3;
				break;
			case Obstacle_LB:
				Line[0].oEndPos.nY      += 32;	//��
				Line[1].oPosition.nY    += 32;	//��
				Line[1].oEndPos.nX      += 32;
				Line[1].oEndPos.nY      += 32;
				Line[2].oEndPos.nX      += 32;	//��б
				Line[2].oEndPos.nY      += 32;
				nNumLine = 3;
				break;
			case Obstacle_RB:
				Line[0].oPosition.nX    += 32;	//��
				Line[0].oEndPos.nX      += 32;
				Line[0].oEndPos.nY      += 32;
				Line[1].oPosition.nY	+= 32;	//��б
				Line[1].oEndPos.nX      += 32;
				Line[2].oPosition.nY    += 32;	//��
				Line[2].oEndPos.nX      += 32;
				Line[2].oEndPos.nY      += 32;
				nNumLine = 3;
				break;
			}
//			g_pRepresent->DrawPrimitives(nNumLine, Line, RU_T_LINE, false);
		   }
		}
	}
		*/
}
//#endif


//��ȡ��һ�������
BOOL KScenePlaceRegionC::GetNextMapPos()
{//�滭�ϰ�
	/*KRULine	Line[4];
	int		nNumLine, i, j, nX, nY,nReg=0;

	for (i = 0; i < 4; ++i)
	{
		Line[i].Color.Color_dw = 0xffffffff;
		Line[i].oPosition.nZ = 0;
		Line[i].oEndPos.nZ = 0;
	}

	nX = m_LeftTopCornerScenePos.x;
	for (i = 0; i < 16; ++i, nX += RWP_OBSTACLE_WIDTH)
	{
		nY = m_LeftTopCornerScenePos.y;
		for (j = 0; j < 32; ++j, nY += RWP_OBSTACLE_HEIGHT)
		{
			int nIdx = SubWorld[0].FindRegion(MAKELONG(m_RegionIndex.x, m_RegionIndex.y));
			if (nIdx >= 0)
			{//�����ڵ�NPC


			}
			//m_ObstacleInfo[i][j]
			long nType = (m_cObstacle.GetData(i,j) >> 4) & 0x0000000f;  //�ж��ϰ��� 16 32 * 16

			if (nType == Obstacle_Empty)
			{
				nReg=TRUE;
				break;
			}

			Line[2].oEndPos.nX = Line[1].oEndPos.nX = Line[0].oEndPos.nX =
				Line[2].oPosition.nX = Line[1].oPosition.nX = Line[0].oPosition.nX = nX;
			Line[2].oEndPos.nY = Line[1].oEndPos.nY = Line[0].oEndPos.nY =
				Line[2].oPosition.nY = Line[1].oPosition.nY = Line[0].oPosition.nY = nY;
			nNumLine = 0;

			switch(nType)
			{
			case Obstacle_Full:
				Line[0].oEndPos.nX += 32;	//��
				Line[1].oEndPos.nY += 32;	//��
				Line[2].oPosition.nX += 32;	//��
				Line[2].oEndPos.nX += 32;
				Line[2].oEndPos.nY += 32;
				Line[3].oPosition.nX = Line[0].oPosition.nX;	//��
				Line[3].oPosition.nY = Line[2].oEndPos.nY;
				Line[3].oEndPos.nX = Line[2].oEndPos.nX;
				Line[3].oEndPos.nY = Line[2].oEndPos.nY;
				nNumLine = 4;
				break;
			case Obstacle_LT:
				Line[0].oEndPos.nX += 32;	//��
				Line[1].oEndPos.nY += 32;	//��
				Line[2].oPosition.nY += 32;	//��б
				Line[2].oEndPos.nX += 32;
				nNumLine = 3;
				break;
			case Obstacle_RT:
				Line[0].oEndPos.nX += 32;	//��
				Line[1].oPosition.nX    += 32;	//��
				Line[1].oEndPos.nX += 32;
				Line[1].oEndPos.nY += 32;
				Line[2].oEndPos.nX += 32;	//��б
				Line[2].oEndPos.nY += 32;
				nNumLine = 3;
				break;
			case Obstacle_LB:
				Line[0].oEndPos.nY += 32;	//��
				Line[1].oPosition.nY += 32;	//��
				Line[1].oEndPos.nX += 32;
				Line[1].oEndPos.nY += 32;
				Line[2].oEndPos.nX += 32;	//��б
				Line[2].oEndPos.nY += 32;
				nNumLine = 3;
				break;
			case Obstacle_RB:
				Line[0].oPosition.nX += 32;	//��
				Line[0].oEndPos.nX += 32;
				Line[0].oEndPos.nY += 32;
				Line[1].oPosition.nY += 32;	//��б
				Line[1].oEndPos.nX += 32;
				Line[2].oPosition.nY += 32;	//��
				Line[2].oEndPos.nX += 32;
				Line[2].oEndPos.nY += 32;
				nNumLine = 3;
				break;
			}
			g_pRepresent->DrawPrimitives(nNumLine, Line, RU_T_LINE, false);

		}
	}
	*/
	return  0 ;//nReg;
}


//##ModelId=3DE29F360221
void KScenePlaceRegionC::PaintAboveHeadObj(KBuildinObj* pObj, RECT* pRepresentArea,int nindex)
{
	//_ASSERT(pObj);
	if  (pObj==NULL) return;

	if  (!g_GameWorld) return;

	//char nTrapInfo[256]={0};
	KRUImage4	Img;
	Img.Color.Color_dw = 0;
	Img.bRenderStyle = IMAGE_RENDER_STYLE_3LEVEL;
	Img.nType = ISI_T_SPR;
	strcpy(Img.szImage, pObj->szImage);

	Img.nFrame = pObj->nFrame;
	Img.uImage = pObj->OBJ_IMAGE_ID;
	Img.nISPosition = pObj->OBJ_IMAGE_ISPOSITION;

	POINT nCurPoint;
	nCurPoint.x = 0;
	nCurPoint.y = 0;

	if (pObj->nAniSpeed == 0)
	{//��֡��
		Img.bRenderFlag = RUIMAGE_RENDER_FLAG_FRAME_DRAW;
		if (SM_IsOutsideRepresentArea(pRepresentArea,
			pObj->ImgPos1.x, pObj->ImgPos1.y, pObj->ImgPos1.z,
			pObj->ImgPos2.x, pObj->ImgPos2.y, pObj->ImgPos2.z) == false)
		{//ÿ��Ҳֻ�ܻ�һ�� �������9��
			Img.oPosition.nX = pObj->ImgPos1.x;
			Img.oPosition.nY = pObj->ImgPos1.y;
			Img.oPosition.nZ = pObj->ImgPos1.z;
			Img.oEndPos.nX = pObj->ImgPos3.x;
			Img.oEndPos.nY = pObj->ImgPos3.y;
			Img.oEndPos.nZ = pObj->ImgPos3.z;

			if ((pObj->Props & SPBIO_P_SORTMANNER_MASK) == SPBIO_P_SORTMANNER_POINT)
			{//��ƫ�ƻ滭 ���� ��
//				g_pRepresent->DrawPrimitives(1, &Img, RU_T_IMAGE, false);
				g_GameWorld->DrawAboveHead(nindex,nCurPoint,1,&Img,BAOVE_NODE_FULL,false);
			}
			else
			{//��ƫ�ƻ滭
				Img.oSecondPos.nX = pObj->ImgPos2.x;
				Img.oSecondPos.nY = pObj->ImgPos2.y;
				Img.oSecondPos.nZ = pObj->ImgPos2.z;

				Img.oThirdPos.nX = pObj->ImgPos4.x;
				Img.oThirdPos.nY = pObj->ImgPos4.y;
				Img.oThirdPos.nZ = pObj->ImgPos4.z;

				Img.oImgLTPos.nX = 0;
				Img.oImgLTPos.nY = 0;
				Img.oImgRBPos.nX = pObj->nImgWidth;
				Img.oImgRBPos.nY = pObj->nImgHeight;
//				g_pRepresent->DrawPrimitives(1, &Img, RU_T_IMAGE_4, false);
				g_GameWorld->DrawAboveHead(nindex,nCurPoint,1,&Img,BAOVE_NODE_PART,false);
			}
		}
	}
	/*else
	{//��֡
		Img.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		Img.oPosition.nX = pObj->oPos1.x;
		Img.oPosition.nY = pObj->oPos1.y;
		Img.oPosition.nZ = pObj->oPos1.z;

		BuildinObjNextFrame(pObj);
	}*/

	pObj->OBJ_IMAGE_ID = Img.uImage;
	pObj->OBJ_IMAGE_ISPOSITION = Img.nISPosition;
}

//## ��ȡ�߿ն������Ļ�ͼ�����š�
unsigned int KScenePlaceRegionC::GetAboveHeadLayer(KBuildinObj*& pObjsAboveHead)
{
	pObjsAboveHead = NULL;
	unsigned int uNum = 0;
	if (m_BiosData.Numbers.nNumBiosAbove && m_BiosData.pBios)
	{
		uNum = m_BiosData.Numbers.nNumBiosAbove;
		pObjsAboveHead = &m_BiosData.pBios[m_BiosData.Numbers.nNumBios - uNum];

	}
	return uNum;
}

//����ڽ���Դ����Ϣ
unsigned int KScenePlaceRegionC::GetBuildinLights(KBuildInLightInfo*& pLights)
{
	unsigned int uNum = 0;
	if (m_BiosData.Numbers.nNumBulidinLight && m_BiosData.pLights)
	{
		pLights = m_BiosData.pLights;
		uNum = m_BiosData.Numbers.nNumBulidinLight;
	}
	return uNum;
}


//##ModelId=3DE33AB30318
void KScenePlaceRegionC::GetBuildinObjs(
		KIpotBuildinObj*& pObjsPointList, unsigned int& nNumObjsPoint,
		KIpotBuildinObj*& pObjsLineList, unsigned int& nNumObjsLine,
		KIpotBuildinObj*& pObjsTreeList, unsigned int& nNumObjsTree)
{
	pObjsPointList = NULL;
	pObjsLineList = NULL;
	pObjsTreeList = NULL;
	nNumObjsPoint = 0;
	nNumObjsLine = 0;
	nNumObjsTree = 0;
	if (m_BiosData.pLeafs)
	{
		int	nCount = 0;
		if (nNumObjsPoint = m_BiosData.Numbers.nNumBiosPoint)
		{
			pObjsPointList = m_BiosData.pLeafs;
			nCount += nNumObjsPoint;
		}
		if (nNumObjsLine = m_BiosData.Numbers.nNumBiosLine)
		{
			pObjsLineList = &m_BiosData.pLeafs[nCount];
			nCount += nNumObjsLine;
		}
		if (nNumObjsTree = m_BiosData.Numbers.nNumBiosTree)
			pObjsTreeList = &m_BiosData.pLeafs[nCount];
	}
}

//## ��ȡ�������ڽ�δ��ʼ���Ķ�����б�
void KScenePlaceRegionC::GetBIOSBuildinObjs(
	KBuildinObj*& pObjsList,
	unsigned int& nNumObjs
)
{
    pObjsList = NULL;

    nNumObjs = m_BiosData.Numbers.nNumBios;
    if (nNumObjs > 0)
        pObjsList = m_BiosData.pBios;
}


void KScenePlaceRegionC::LeaveProcessArea()
{
	//��KRUImage::GROUND_IMG_OCCUPY_FLAG(bMatchReferenceSpot)����ʾKRUImage�����Ƿ�ռ��
	//��KRUImage::GROUND_IMG_OK_FLAG(bFrameDraw)����ʾKRUImage�����Ƿ��Ѿ�Ԥ��Ⱦ����
	if (m_pPrerenderGroundImg)
	{
		m_pPrerenderGroundImg->GROUND_IMG_OCCUPY_FLAG = false;
		m_pPrerenderGroundImg->GROUND_IMG_OK_FLAG = false;
	}

	m_pPrerenderGroundImg = NULL;
	if (m_BiosData.pLeafs)
	{//�뿪�� ���� �ͷ���Դ��
		free(m_BiosData.pLeafs);
		m_BiosData.pLeafs = NULL;

	}
}

void KScenePlaceRegionC::EnterProcessArea(KRUImage *pImage)
{
	//��KRUImage::GROUND_IMG_OCCUPY_FLAG(bMatchReferenceSpot)����ʾKRUImage�����Ƿ�ռ��
	//��KRUImage::GROUND_IMG_OK_FLAG(bFrameDraw)����ʾKRUImage�����Ƿ��Ѿ�Ԥ��Ⱦ����
	if (pImage && m_pPrerenderGroundImg != pImage)
	{//�������ͬ
		if (m_pPrerenderGroundImg)
		{
			m_pPrerenderGroundImg->GROUND_IMG_OCCUPY_FLAG = false;
			m_pPrerenderGroundImg->GROUND_IMG_OK_FLAG = false;
		}
		if (m_pPrerenderGroundImg = pImage)
		{//����
		  if (m_pPrerenderGroundImg)
		  {
			m_pPrerenderGroundImg->GROUND_IMG_OCCUPY_FLAG = true;
			m_pPrerenderGroundImg->GROUND_IMG_OK_FLAG = false;
			m_pPrerenderGroundImg->oPosition.nX = m_LeftTopCornerScenePos.x;
			m_pPrerenderGroundImg->oPosition.nY = m_LeftTopCornerScenePos.y;
			m_pPrerenderGroundImg->oEndPos.nX = m_pPrerenderGroundImg->oPosition.nX + RWPP_AREGION_WIDTH;
			m_pPrerenderGroundImg->oEndPos.nY = m_pPrerenderGroundImg->oPosition.nY + RWPP_AREGION_HEIGHT;
			m_pPrerenderGroundImg->oEndPos.nZ = m_pPrerenderGroundImg->oPosition.nZ = 0;
		  }
		}
	}

	unsigned int i;
	unsigned int nCount = m_BiosData.Numbers.nNumBios
							- m_BiosData.Numbers.nNumBiosAbove; //��ȥ�߿���Ŀ
	KIpotBuildinObj* pLeaf = NULL;
	KBuildinObj*	 pObj  = NULL;

	if (m_BiosData.pLeafs == NULL && m_BiosData.pBios &&
		m_BiosData.Numbers.nNumBios > m_BiosData.Numbers.nNumBiosAbove)
	{//���������
		m_BiosData.pLeafs = (KIpotBuildinObj*) malloc(
			sizeof(KIpotBuildinObj) * nCount);
		if (m_BiosData.pLeafs)
		{
			pLeaf = m_BiosData.pLeafs;
			pObj  = m_BiosData.pBios;
			for (i = 0; i < nCount; ++i, ++pLeaf, ++pObj)
			{
				pLeaf->eLeafType = KIpotLeaf::IPOTL_T_BUILDIN_OBJ;

				pLeaf->uImage = 0;
				pLeaf->nISPosition = -1;
				pLeaf->pBio = pObj;

				pLeaf->oPosition.x = pObj->oPos1.x;
				pLeaf->oPosition.y = pObj->oPos1.y;
				pLeaf->oEndPos.x = pObj->oPos2.x;
				pLeaf->oEndPos.y = pObj->oPos2.y;

				pLeaf->fAngleXY  = pObj->fAngleXY;
				pLeaf->fNodicalY = pObj->fNodicalY;

				pLeaf->bClone = false;
				pLeaf->bImgPart = false;
				pLeaf->pBrother = NULL;
				pLeaf->pRChild = NULL;
				pLeaf->pLChild = NULL;
			}
			pLeaf  = m_BiosData.pLeafs;
			nCount = m_BiosData.Numbers.nNumBiosPoint;
			for (i = 0; i < nCount; ++i, ++pLeaf)
				pLeaf->oPosition.y += POINT_LEAF_Y_ADJUST_VALUE;
		}
	}
	else if(m_BiosData.pLeafs)
	{//��� ���ڵ�
		pLeaf = m_BiosData.pLeafs;
		pObj  = m_BiosData.pBios;
		for (i = 0; i < nCount; ++i, ++pLeaf, ++pObj)
		{
			pLeaf->oPosition.x = pObj->oPos1.x;
			pLeaf->oPosition.y = pObj->oPos1.y;
			pLeaf->oEndPos.x = pObj->oPos2.x;
			pLeaf->oEndPos.y = pObj->oPos2.y;
			pLeaf->fAngleXY  = pObj->fAngleXY;
			pLeaf->fNodicalY = pObj->fNodicalY;
			pLeaf->bImgPart = false;
		}
		pLeaf = m_BiosData.pLeafs;
		//������
		nCount = m_BiosData.Numbers.nNumBiosPoint;
		for (i = 0; i < nCount; ++i, ++pLeaf)
			pLeaf->oPosition.y += POINT_LEAF_Y_ADJUST_VALUE;
	}
}
//��ȡ�ϰ�����Ϣ
long KScenePlaceRegionC::GetObstacleInfo(int nX, int nY)
{
	int		nMpsX, nMpsY, nMapX, nMapY;
	long	lInfo, lRet, lType;

	nMpsX = nX - m_LeftTopCornerScenePos.x;
	nMpsY = nY - m_LeftTopCornerScenePos.y;

	nMapX = nMpsX / RWP_OBSTACLE_WIDTH;
	nMapY = nMpsY / RWP_OBSTACLE_HEIGHT;

	CCAssert(nMapX >= 0 && nMapX < RWP_NUM_GROUND_CELL_H && nMapY >= 0 && nMapY < RWP_NUM_GROUND_CELL_V * 2,"");
	lInfo = m_cObstacle.GetData(nMapX,nMapY);//m_ObstacleInfo[nMapX][nMapY];
	nMpsX -= nMapX * RWP_OBSTACLE_WIDTH;
	nMpsY -= nMapY * RWP_OBSTACLE_HEIGHT;
	lRet = lInfo & 0x0000000f;

	lType = (lInfo >> 4) & 0x0000000f;
	switch(lType)
	{
	case Obstacle_LT:
		if (nMpsX + nMpsY > RWP_OBSTACLE_WIDTH)
			lRet = Obstacle_NULL;
		break;
	case Obstacle_RT:
		if (nMpsX < nMpsY)
			lRet = Obstacle_NULL;
		break;
	case Obstacle_LB:
		if (nMpsX > nMpsY)
			lRet = Obstacle_NULL;
		break;
	case Obstacle_RB:
		if (nMpsX + nMpsY < RWP_OBSTACLE_WIDTH)
			lRet = Obstacle_NULL;
		break;
	default:
		break;
	}
	return lRet;
}
//��ȡ��������ϰ���Ϣ
long KScenePlaceRegionC::GetObstacleInfoMin(int nX, int nY, int nOffX, int nOffY)
{

	int		nMpsX, nMpsY, nMapX, nMapY;
	long	lRet, lType;

	nMpsX = nX - m_LeftTopCornerScenePos.x;
	nMpsY = nY - m_LeftTopCornerScenePos.y;
	nMapX = nMpsX / RWP_OBSTACLE_WIDTH;
	nMapY = nMpsY / RWP_OBSTACLE_HEIGHT;
	nMpsX = ((nMpsX - nMapX * RWP_OBSTACLE_WIDTH) << 10) + nOffX;
	nMpsY = ((nMpsY - nMapY * RWP_OBSTACLE_HEIGHT) << 10) + nOffY;

	CCAssert(nMapX >= 0 && nMapX < RWP_NUM_GROUND_CELL_H && nMapY >= 0 && nMapY < RWP_NUM_GROUND_CELL_V * 2,"");
	CCAssert(nOffX >= 0 && nOffX < 1024 && nOffY >= 0 && nOffY < 1024,"");

	lRet = /*m_ObstacleInfo[nMapX][nMapY]*/m_cObstacle.GetData(nMapX,nMapY) & 0x0000000f;
	lType = (/*m_ObstacleInfo[nMapX][nMapY]*/m_cObstacle.GetData(nMapX,nMapY) >> 4) & 0x0000000f;

	switch(lType)
	{
	case Obstacle_LT:
		if (nMpsX + nMpsY > RWP_OBSTACLE_WIDTH << 10)
			lRet = Obstacle_NULL;
		break;
	case Obstacle_RT:
		if (nMpsX < nMpsY)
			lRet = Obstacle_NULL;
		break;
	case Obstacle_LB:
		if (nMpsX > nMpsY)
			lRet = Obstacle_NULL;
		break;
	case Obstacle_RB:
		if (nMpsX + nMpsY < RWP_OBSTACLE_WIDTH << 10)
			lRet = Obstacle_NULL;
		break;
	default:
		break;
	}
	return lRet;
}

void KScenePlaceRegionC::SetHightLightSpecialObject(unsigned int uBioIndex)
{
	if (m_BiosData.pBios && uBioIndex < m_BiosData.Numbers.nNumBios)
		m_BiosData.pBios[uBioIndex].Props |= SPBIO_F_HIGHT_LIGHT;
}
void KScenePlaceRegionC::UnsetHightLightSpecialObject(unsigned int uBioIndex)
{
	if (m_BiosData.pBios && uBioIndex < m_BiosData.Numbers.nNumBios)
		m_BiosData.pBios[uBioIndex].Props &= ~SPBIO_F_HIGHT_LIGHT;
}
