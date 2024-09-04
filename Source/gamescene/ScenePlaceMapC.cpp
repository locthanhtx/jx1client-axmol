// ***************************************************************************************
// ��ͼ���ඨ��ʵ��
// Copyright : Kingsoft 2003
// Author    : wooy(wu yue)
// CreateTime: 2003-7-8
// ***************************************************************************************
#include "gamecore/KCore.h"
#include "KScenePlaceC.h"  //�����ӵ�
#include "ScenePlaceMapC.h"
#include "gamecore/KPlayer.h"
#include "gamecore/ImgRef.h"
#include "gamecore/GameDataDef.h"
#include "KScenePlaceRegionC.h"
#include "SceneDataDef.h"
#include "engine/KSG_StringProcess.h"
//�����ӵ�
#include "gamecore/coreshell.h"
#include "engine/Text.h"
#include "engine/KIniFile.h"
#ifdef WIN32
#include <direct.h>
#endif

extern KImageStore2 m_ImageStore;
/////////////////////////////////
#define	PLACE_MAP_FILE_NAME_APPEND		"24.jpg"  //24.jpg
#define	PLACE_MAP_FILE_NAME_NEW		    ".jpg"  //24.jpg

#define	PLACE_MAP_SAVE_SECTION			"MAIN"
#define	MAP_SETTING_FILE				"\\Ui\\Setting.ini"
bool    g_ShowPaint    = FALSE;
#define	RIGHT_BOTTOM_NO_LIMIT			0x7fffffff

KScenePlaceMapC::KScenePlaceMapC()
{
	m_bHavePicMap = false;
	m_bInited = false;
	m_szEntireMapFile[0] = 0;
	m_EntireMapLTPosition.x = m_EntireMapLTPosition.y = 0;
	m_FocusPosition.x = m_FocusPosition.y = 0;
	m_FocusLimit.left = m_FocusLimit.right = m_FocusLimit.top = m_FocusLimit.bottom = 0;
	m_PicLoadedLTPosition.x = m_PicLoadedLTPosition.y = 0;
	m_Size.cx = m_Size.cy = 0;
	m_MapCoverArea.left = m_MapCoverArea.right = m_MapCoverArea.top = m_MapCoverArea.bottom = 0;
	m_PaintCell.left = m_PaintCell.right = m_PaintCell.top = m_PaintCell.bottom = 0;
	m_uMapShowElems = SCENE_PLACE_MAP_ELEM_NONE;
	//memset(&m_ElemsList, 0, sizeof(m_ElemsList));
	//m_pEntireMap = NULL;
	m_GreenLineDes.x = -123456;  //����Ŀ������
	m_GreenLineDes.y = -123456;
	m_nShowWidth  = 4;			// ��ʾ����ٸ�region���ϰ�
    m_nShowHeight = 4;			// ��ʾ�߶��ٸ�region���ϰ�

	m_DestRegIdx_X = 0;
	m_DestRegIdx_Y = 0;

	m_YscRegIdx_X  = 0;
	m_YscRegIdx_Y  = 0;

	m_SubWorld     = -1;
	m_YscTempRegIdx_X = 0;		//  ���￪ʼѰ·�����ڵ����� ����
    m_YscTempRegIdx_Y = 0;

	//m_ppbtBarrier = NULL;
	//m_pbyLoadFlag = NULL;
	m_nMapWidth   = 0;
	m_nMapHeight  = 0;
	//nPicTexture =NULL;
}

KScenePlaceMapC::~KScenePlaceMapC()
{
	Terminate();
}

//��ʼ��
bool KScenePlaceMapC::Initialize()
{
	if (m_bInited == false)
	{// && g_pRepresent
		int nCount = MAP_CELL_MAX_RANGE * MAP_CELL_MAX_RANGE;
//		int	nIndex, i;
		//MAP_CELL* pCell = &m_ElemsList[0][0];

		/*for (nIndex = i = 0; nIndex < nCount && i < 0x03335688; ++i)
		{
			sprintf(pCell->szImageName, "*PlaceMap_%d*", i);

//			pCell->uImageId = g_pRepresent->CreateImage(pCell->szImageName, MAP_CELL_MAP_WIDTH, MAP_CELL_MAP_HEIGHT, ISI_T_BITMAP16);

			if (pCell->uImageId)
			{
				pCell++;
				nIndex++;
			}
		}*/

		m_bInited = true;

		/*KIniFile	ColorSetting;
		char		szBuffer[64]={0};
		if (ColorSetting.Load(MAP_SETTING_FILE))
		{
			const char* pcszTemp;
			KRColor		Color;
			Color.Color_dw = 0;

			// ������ɫ
			ColorSetting.GetString("Map", "SelfColor", "255,255,255", szBuffer, sizeof(szBuffer));
            pcszTemp = szBuffer;
            Color.Color_b.r = KSG_StringGetInt(&pcszTemp, 255);
            KSG_StringSkipSymbol(&pcszTemp, ',');
			Color.Color_b.g = KSG_StringGetInt(&pcszTemp, 255);
            KSG_StringSkipSymbol(&pcszTemp, ',');
			Color.Color_b.b = KSG_StringGetInt(&pcszTemp, 255);
			m_uSelfColor = Color.Color_dw;

			// ������ɫ
			ColorSetting.GetString("Map", "TeammateColor", "255,255,255", szBuffer, sizeof(szBuffer));
            pcszTemp = szBuffer;
			Color.Color_b.r = KSG_StringGetInt(&pcszTemp, 255);
            KSG_StringSkipSymbol(&pcszTemp, ',');
			Color.Color_b.g = KSG_StringGetInt(&pcszTemp, 255);
            KSG_StringSkipSymbol(&pcszTemp, ',');
			Color.Color_b.b = KSG_StringGetInt(&pcszTemp, 255);
			m_uTeammateColor = Color.Color_dw;

			// ���������ɫ
			ColorSetting.GetString("Map", "PlayerColor", "255,255,255", szBuffer, sizeof(szBuffer));
            pcszTemp = szBuffer;
			Color.Color_b.r = KSG_StringGetInt(&pcszTemp, 255);
            KSG_StringSkipSymbol(&pcszTemp, ',');
			Color.Color_b.g = KSG_StringGetInt(&pcszTemp, 255);
            KSG_StringSkipSymbol(&pcszTemp, ',');
			Color.Color_b.b = KSG_StringGetInt(&pcszTemp, 255);
			m_uPlayerColor = Color.Color_dw;

			// ս��npc��ɫ
			ColorSetting.GetString("Map", "FightNpcColor", "255,255,255", szBuffer, sizeof(szBuffer));
            pcszTemp = szBuffer;
			Color.Color_b.r = KSG_StringGetInt(&pcszTemp, 255);
            KSG_StringSkipSymbol(&pcszTemp, ',');
			Color.Color_b.g = KSG_StringGetInt(&pcszTemp, 255);
            KSG_StringSkipSymbol(&pcszTemp, ',');
			Color.Color_b.b = KSG_StringGetInt(&pcszTemp, 255);
			m_uFightNpcColor = Color.Color_dw;

			// ��ͨnpc��ɫ
			ColorSetting.GetString("Map", "NormalNpcColor", "255,255,255", szBuffer, sizeof(szBuffer));
            pcszTemp = szBuffer;
			Color.Color_b.r = KSG_StringGetInt(&pcszTemp, 255);
            KSG_StringSkipSymbol(&pcszTemp, ',');
			Color.Color_b.g = KSG_StringGetInt(&pcszTemp, 255);
            KSG_StringSkipSymbol(&pcszTemp, ',');
			Color.Color_b.b = KSG_StringGetInt(&pcszTemp, 255);
			m_uNormalNpcColor = Color.Color_dw;
		}

		ColorSetting.Clear();*/
	}

	return m_bInited;
}

//���������ܡ��ͷŶ����ȫ�������붯̬�������Դ��
void KScenePlaceMapC::Terminate()
{
	/*if (m_bInited)
	{
		for (int v = 0; v < MAP_CELL_MAX_RANGE; ++v)
		{
			for (int h = 0; h < MAP_CELL_MAX_RANGE; ++h)
			{
				if (m_ElemsList[v][h].uImageId)
				{
					m_ImageStore.FreeImage(m_ElemsList[v][h].szImageName);
					m_ElemsList[v][h].uImageId = 0;
				}
				m_ElemsList[v][h].szImageName[0] = 0;
			}
		}
	}*/
}

//���ó�����ͼ�İ�����Ԫ��
void KScenePlaceMapC::SetShowElemsFlag(unsigned int uShowElemsFlag)
{
	m_uMapShowElems = uShowElemsFlag;
	if (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_PIC)
		SetFocusPosition(m_FocusPosition.x, m_FocusPosition.y, true);
}

//�����ļ���
int KScenePlaceMapC::CreateFolder(char * nPath)
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
#ifdef WIN32
	char tPath[255];
	if (nPath[0]=='/'||nPath[0]=='\\')
	{
		nPath++;
	}
	for (size_t i = 1; i < strlen(nPath); i++)
	{
		if (nPath[i] == '/')nPath[i] = '\\';
		if (nPath[i] == '\\')
		{
			memcpy(tPath, nPath, i );
			tPath[i] = 0;
			_mkdir(tPath);
		}
	}
#endif
	return 1;

}

ax::Texture2D* KScenePlaceMapC::createTextureFromRawData(const unsigned char* sBuf, ssize_t dataSize) {
    ax::Image* image = new ax::Image();
    if (image->initWithImageData(sBuf, dataSize)) {
        ax::Texture2D* texture = new ax::Texture2D();
        if (texture->initWithImage(image)) {
            return texture;
        } else {
            delete texture;
        }
    }

    delete image;
    return nullptr;
}

//��ȡ����
void KScenePlaceMapC::Load(KIniFile* pSetting,int nSubWoleID,const char* pszScenePlaceRootPath )//const char* pszScenePlaceRootPath
{
	if (Initialize() == false)
		return;

	Free();

	m_bHavePicMap = false;
	m_bPicMapType = 0;
	if (pSetting && nSubWoleID>-1)
	{
		m_SubWorld = nSubWoleID;
		char m_jpgMapFile[256],nCurSavePath[256];
		ZeroMemory(m_jpgMapFile,sizeof(m_jpgMapFile));
		ZeroMemory(nCurSavePath,sizeof(nCurSavePath));
		sprintf(m_jpgMapFile, "maps/%d" PLACE_MAP_FILE_NAME_NEW,nSubWoleID);
		sprintf(m_szEntireMapFile,"%s" PLACE_MAP_FILE_NAME_APPEND, pszScenePlaceRootPath);
		//m_bHavePicMap = g_FileExists(m_szEntireMapFile);           //�������ļ��Ƿ����
		ZeroMemory(&m_PackRef,sizeof(XPackElemFileRef));
		Texture2D* nPicTexture =NULL;
		char nSprFileKey[64]={0};
		DWORD nFielpahtdwid = g_FileName2Id(m_szEntireMapFile);
		sprintf(nSprFileKey,"%u-%d",nFielpahtdwid,0);
		nPicTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFileKey);
		if (nPicTexture==NULL)
		{
		   if (g_pPakList)
		       m_bHavePicMap  = g_pPakList->pGetFilePath(m_szEntireMapFile,m_PackRef);
	       if (m_bHavePicMap)
		   {
			   unsigned char * sBuf=NULL;
			   sBuf = (unsigned char *)malloc(m_PackRef.nSize+1);
			   if (g_pPakList) //��ȡͼƬ������ addImageWihtData
				  g_pPakList->ElemFileRead(m_PackRef,sBuf,m_PackRef.nSize);

			   if (sBuf)
			   {
//			     nPicTexture = CCTextureCache::sharedTextureCache()->addImageWihtData(nSprFileKey,sBuf,m_PackRef.nSize,0);
//                   nPicTexture = createTextureFromRawData(sBuf, m_PackRef.nSize);
                   ax::Image* image = new ax::Image();
                   image->initWithImageData(sBuf, m_PackRef.nSize);
                   nPicTexture = ax::Director::getInstance()->getTextureCache()->addImage(image, nSprFileKey);
                   free(sBuf);
			       sBuf = NULL;
			   }
			/*std::string nsavepath = ax::FileUtils::getInstance()->getWritablePath();
			sprintf(nCurSavePath,"%s%s",nsavepath.c_str(),m_jpgMapFile);
			CreateFolder(nCurSavePath);
			FILE *oFp ;
			if((oFp=fopen(nCurSavePath,"wb+"))==0)
				return;
			fseek(oFp,0,0);
			fwrite(sBuf,m_PackRef.nSize,1,oFp);
			fclose(oFp);
			*/
		  }
		}

		if (nPicTexture)
		{
			m_bHavePicMap = true;
			Size n =  nPicTexture->getContentSize();
			m_CurPicWidth  = (int)n.width; //����Ŀ��
			m_CurPicHeight = (int)n.height;//����ĸ߶�
		}
		std::string nCurFilePath = m_szEntireMapFile;
		//m_bHavePicMap = ax::FileUtils::getInstance()->isFileExist(nCurFilePath);
        if (!m_bHavePicMap)
		{
			//messageBox(nCurFilePath.c_str(),"������");
            sprintf(m_szEntireMapFile,"maps/tongyong.jpg");
			m_bHavePicMap =true;
			m_bPicMapType = 1;
		}

		if (m_bHavePicMap)
		{//�Ƿ���С��ͼ	 MapLTRegionIndex

			m_EntireMapLTPosition.x = -1;
			pSetting->GetInteger2(PLACE_MAP_SAVE_SECTION,"MapLTRegionIndex",(int*)&m_EntireMapLTPosition.x, (int*)&m_EntireMapLTPosition.y);

			if (m_EntireMapLTPosition.x == -1)
			{//С��ͼ�Ĵ�С
				//RECT	rc;
				pSetting->GetRect(PLACE_MAP_SAVE_SECTION, "rect", &m_MapRect);
				m_EntireMapLTPosition.x = m_MapRect.left;	 //1  Ϊ��Ӧ�����ļ�������� dat�ļ��� ��ʼ���
				m_EntireMapLTPosition.y = m_MapRect.top;	 //2  Ϊ������ļ��п�ʼ���
				m_EndMapRBPosition.x    = m_MapRect.right;   //3  Ϊ��Ӧ�����ļ�������� dat�ļ��� �������
				m_EndMapRBPosition.y    = m_MapRect.bottom;  //4  Ϊ������ļ��н������
				//���ر�С��ͼ�� �����ϰ��б� ����߶��ж��ٸ������Ŀ��
//			    int x,y;

			    //��ͼ��ʵ�ʴ�С
				//this->m_nMapWidth		= (m_MapRect.right  - m_MapRect.left+1);  // dat �ļ�  �����ͼ�ж��ٸ� �����(16)�Ŀ��
				//this->m_nMapHeight	    = (m_MapRect.bottom - m_MapRect.top  + 1);  // �ļ���	 �����ͼ�ж��ٸ� �����(32)�Ŀ��
				//char msg[64];
			    //sprintf(msg,"w:%d,h:%d,%d",m_nMapWidth,m_nMapHeight,m_PackRef.nSize);
			    //messageBox(msg,"��ͼ��С");
				//��ʼ��ʼ��С��ͼ����

				/*if (sBuf);
				{//��ʼ�������
					char nSprFilePath[64]={0};
					DWORD nFielpahtdwid = g_FileName2Id(m_szEntireMapFile);
					sprintf(nSprFilePath,"%u-%d",nFielpahtdwid,0);
					ax::TextureCache::getInstance()->addImage(nSprFilePath,sBuf,m_nMapWidth,m_nMapHeight,ax::Size((float)(m_nMapWidth), (float)(m_nMapHeight)));
				}*/
				//-------------------
			if (m_EntireMapLTPosition.x != -1)
			{//�Ŵ� ����   ת�� ��������
				m_EntireMapLTPosition.x *= KScenePlaceRegionC::RWPP_AREGION_WIDTH;   //ת����������
				m_EntireMapLTPosition.y *= KScenePlaceRegionC::RWPP_AREGION_HEIGHT;

				m_EndMapRBPosition.x    *= KScenePlaceRegionC::RWPP_AREGION_WIDTH;
				m_EndMapRBPosition.y    *= KScenePlaceRegionC::RWPP_AREGION_HEIGHT;

				m_PicLoadedLTPosition.x = 0;
				m_PicLoadedLTPosition.y = 0;
				//ͼƬ�Ŀ�ʼλ��?
				m_FocusLimit.left = m_EntireMapLTPosition.x + (m_Size.cx * MAP_SCALE_H / 2);//*8
				m_FocusLimit.top  = m_EntireMapLTPosition.y + (m_Size.cy * MAP_SCALE_V / 2);//*16
				m_FocusLimit.bottom = m_FocusLimit.right = RIGHT_BOTTOM_NO_LIMIT;

				m_bHavePicMap = true;
				//m_bPicMapType = 1;
			}
			else
			{
				m_bHavePicMap = false;
				m_bPicMapType =0;
			}
		}
	}
  //messageBox("���سɹ�","���سɹ�");
 }//end if

}
//���ó�����ͼ�Ľ���
void KScenePlaceMapC::SetFocusPosition(int nX, int nY, bool bChangedRegion)
{
	if (nX < m_FocusLimit.left)
		m_FocusPosition.x = m_FocusLimit.left;
	else if (nX > m_FocusLimit.right)
		m_FocusPosition.x = m_FocusLimit.right;
	else
		m_FocusPosition.x = nX;
	if (nY < m_FocusLimit.top)
		m_FocusPosition.y = m_FocusLimit.top;
	else if (nY > m_FocusLimit.bottom)
		m_FocusPosition.y = m_FocusLimit.bottom;
	else
		m_FocusPosition.y = nY;

	m_MapCoverArea.left = m_FocusPosition.x/MAP_SCALE_H - m_Size.cx / 2;//-64
	m_MapCoverArea.top  = m_FocusPosition.y/MAP_SCALE_V - m_Size.cy / 2;//-64
	m_MapCoverArea.right  = m_MapCoverArea.left + m_Size.cx;
	m_MapCoverArea.bottom = m_MapCoverArea.top  + m_Size.cy;
	//�ϰ�С��ͼ
	/*m_LittleMap.Init(m_MapCoverArea.left,m_MapCoverArea.top,m_MapCoverArea.right,m_MapCoverArea.bottom);
	m_LittleMap.SetShowSize(100,100);
	m_LittleMap.Show(TRUE);*/

	if (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_PIC)
	{
		if (bChangedRegion && m_bHavePicMap)
		{
			POINT	CellIndex;
			CellIndex.x = (m_FocusPosition.x - m_PicLoadedLTPosition.x) / MAP_CELL_SCENE_WIDTH;
			CellIndex.y = (m_FocusPosition.y - m_PicLoadedLTPosition.y) / MAP_CELL_SCENE_HEIGHT;
			if (CellIndex.x != MAP_CELL_CENTRE_INDEX || //MAP_CELL_FOCUS_INDEX_MIN || CellIndex.x > MAP_CELL_FOCUS_INDEX_MAX ||
				CellIndex.y != MAP_CELL_CENTRE_INDEX)	//MAP_CELL_FOCUS_INDEX_MIN || CellIndex.y > MAP_CELL_FOCUS_INDEX_MAX)
			{
				m_PicLoadedLTPosition.x += (CellIndex.x - MAP_CELL_CENTRE_INDEX) * MAP_CELL_SCENE_WIDTH;
				m_PicLoadedLTPosition.y += (CellIndex.y - MAP_CELL_CENTRE_INDEX) * MAP_CELL_SCENE_HEIGHT;
				FillCellsPicInfo();
			}
		}
		CalcPicLayout();
	}
}

void KScenePlaceMapC::FillCellsPicInfo()
{

	if (m_szEntireMapFile[0])
	{

		  //  nPicTexture = ax::Director::getInstance()->getTextureCache()->addImage(m_szEntireMapFile);
		/*char nSprFileKey[64]={0};
		DWORD nFielpahtdwid = g_FileName2Id(m_szEntireMapFile);
		sprintf(nSprFileKey,"%u-%d",nFielpahtdwid,0);
		 //messageBox(nSprFileKey,"FillCellsPicInfo");
		nPicTexture = ax::Director::getInstance()->getTextureCache()->getTextureForKey(nSprFileKey);
		*/
		//if (nPicTexture)
		{
			//messageBox(m_szEntireMapFile,"�������");
			//Size n =  nPicTexture->getContentSize();
			//m_CurPicWidth  = (int)n.width; //����Ŀ��
			//m_CurPicHeight = (int)n.height;//����ĸ߶�

			//m_pEntireMap->nWidth = m_CurPicWidth;
			//m_pEntireMap->nHeight= m_CurPicHeight;
			//m_pEntireMap->Data   = (unsigned short)nPicTexture;
			//char msg[32];
			//sprintf(msg,"w:%d,h:%d",m_CurPicWidth,m_CurPicHeight);
			//messageBox(msg,"FillCellsPicInfo");
		}

		m_FocusLimit.right  = m_FocusLimit.left + (m_CurPicWidth - m_Size.cx)* MAP_SCALE_H;
		m_FocusLimit.bottom = m_FocusLimit.top + (m_CurPicHeight - m_Size.cy)* MAP_SCALE_V;
		if (m_FocusLimit.right < m_FocusLimit.left)
			m_FocusLimit.right = m_FocusLimit.left;
		if (m_FocusLimit.bottom < m_FocusLimit.top)
			m_FocusLimit.bottom = m_FocusLimit.top;
	}
	else
	{
		m_bHavePicMap = false;
		return;
	}

	/*if (m_pEntireMap == NULL)
	{
		if (m_bHavePicMap)
		{
			KBitmapDataBuffInfo	Info;
			short *pBuff = (short*)g_pRepresent->GetBitmapDataBuffer(m_ElemsList[0][0].szImageName, &Info);
			if (pBuff)
			{
				g_pRepresent->ReleaseBitmapDataBuffer(m_ElemsList[0][0].szImageName, pBuff);
				unsigned int uMask16 = -1;
				if (Info.eFormat == BDBF_16BIT_555)
					uMask16 = RGB_555;
				else if (Info.eFormat == BDBF_16BIT_565)
					uMask16 = RGB_565;
				if (uMask16 >= 0)
					m_pEntireMap = get_jpg_image(m_szEntireMapFile, uMask16);
			}
		}
		if (m_pEntireMap)
		{
			m_FocusLimit.right = m_FocusLimit.left + (m_pEntireMap->nWidth - m_Size.cx)* MAP_SCALE_H;
			m_FocusLimit.bottom = m_FocusLimit.top + (m_pEntireMap->nHeight - m_Size.cy)* MAP_SCALE_V;
			if (m_FocusLimit.right < m_FocusLimit.left)
				m_FocusLimit.right = m_FocusLimit.left;
			if (m_FocusLimit.bottom < m_FocusLimit.top)
				m_FocusLimit.bottom = m_FocusLimit.top;
		}
		else
		{
			m_bHavePicMap = false;
			return;
		}
	}
	*/

	int	nStartX = (m_PicLoadedLTPosition.x - m_EntireMapLTPosition.x) / MAP_SCALE_H;
	int nStartY = (m_PicLoadedLTPosition.y - m_EntireMapLTPosition.y) / MAP_SCALE_V;

	//char msg[64];
	//sprintf(msg,"nStartX:%d,nStartY:%d",nStartX,nStartY);
	//messageBox(msg,"С��ͼ");
	return;

	SIZE	PicEntireSize;
	PicEntireSize.cx = m_CurPicWidth;//m_pEntireMap->nWidth;
	PicEntireSize.cy = m_CurPicHeight;//m_pEntireMap->nHeight;
	POINT	DestPos;

	int	h, v, x, y, nFromX, nFromY, nToX, nToY;
	for (v = 0, y = nStartY; v < MAP_CELL_MAX_RANGE; ++v, y += MAP_CELL_MAP_HEIGHT)
	{
		nToY = y + MAP_CELL_MAP_HEIGHT;
		bool bCleared = false;
		if (y < 0 || nToY > PicEntireSize.cy)
		{//����û��ȫ������ͼ��
			for (h = 0; h < MAP_CELL_MAX_RANGE; ++h)
			{
				//g_pRepresent->ClearImageData(m_ElemsList[v][h].szImageName,
					//m_ElemsList[v][h].uImageId, m_ElemsList[v][h].sISPosition);
			}
			bCleared = true;
		}
		if (y < PicEntireSize.cy && nToY > 0)
		{//�ڷ�Χ�ڵ�
			if (y >= 0)
			{
				nFromY = y;
				DestPos.y = 0;
			}
			else
			{
				nFromY = 0;
				DestPos.y = -y;
			}

			if (nToY > PicEntireSize.cy)//������ڸ߶�
				nToY = PicEntireSize.cy;

			for (h = 0, x = nStartX; h < MAP_CELL_MAX_RANGE; ++h, x += MAP_CELL_MAP_WIDTH)
			{
				nToX = x + MAP_CELL_MAP_WIDTH;
				if (bCleared == false && (x < 0 || nToX > PicEntireSize.cx))
				{//����û��ȫ������ͼ��
					//g_pRepresent->ClearImageData(m_ElemsList[v][h].szImageName,
						//m_ElemsList[v][h].uImageId, m_ElemsList[v][h].sISPosition);
				}

				if (x < PicEntireSize.cx && nToX > 0)
				{
					if (x >= 0)
					{
						nFromX = x;
						DestPos.x = 0;
					}
					else
					{
						nFromX = 0;
						DestPos.x = -x;
					}
					if (nToX > PicEntireSize.cx)
						nToX = PicEntireSize.cx;

					/*short *pBuf = (short*)g_pRepresent->GetBitmapDataBuffer(
						//m_ElemsList[v][h].szImageName, NULL);

					if (pBuf)
					{
						short* pEntire = (short*)(&m_pEntireMap->Data) +
							nFromY * PicEntireSize.cx + nFromX;
						short* pDest = pBuf + MAP_CELL_MAP_WIDTH * DestPos.y + DestPos.x;
						for (int i = nFromY; i < nToY; ++i)
						{
							memcpy(pDest, pEntire, (nToX - nFromX) * 2);
							pEntire += PicEntireSize.cx;
							pDest += MAP_CELL_MAP_WIDTH;
						}
//						g_pRepresent->ReleaseBitmapDataBuffer(
	//						m_ElemsList[v][h].szImageName, pBuf);
					}*/
				}
			}
		}
	}
}

void KScenePlaceMapC::CalcPicLayout()
{
	POINT	ShowLTPos;

	ShowLTPos.x = m_MapCoverArea.left - m_PicLoadedLTPosition.x / MAP_SCALE_H ;
	ShowLTPos.y = m_MapCoverArea.top  - m_PicLoadedLTPosition.y / MAP_SCALE_V ;
//	ShowLTPos.x = (m_FocusPosition.x - m_PicLoadedLTPosition.x) / MAP_SCALE_H - m_Size.cx / 2;
//	ShowLTPos.y = (m_FocusPosition.y - m_PicLoadedLTPosition.y) / MAP_SCALE_V - m_Size.cy / 2;

	m_PaintCell.left = ShowLTPos.x / MAP_CELL_MAP_WIDTH;
	m_PaintCell.top = ShowLTPos.y / MAP_CELL_MAP_HEIGHT;

	m_FirstCellSkipWidth.cx = ShowLTPos.x - m_PaintCell.left * MAP_CELL_MAP_WIDTH;
	m_FirstCellSkipWidth.cy = ShowLTPos.y - m_PaintCell.top * MAP_CELL_MAP_HEIGHT;

	int nTemp = m_Size.cx + m_FirstCellSkipWidth.cx + MAP_CELL_MAP_WIDTH - 1;
	m_PaintCell.right = nTemp / MAP_CELL_MAP_WIDTH;
	m_LastCellSkipHeight.cx = nTemp - MAP_CELL_MAP_WIDTH * m_PaintCell.right;
	m_PaintCell.right += m_PaintCell.left;

	nTemp = m_Size.cy + m_FirstCellSkipWidth.cy + MAP_CELL_MAP_HEIGHT - 1;
	m_PaintCell.bottom = nTemp / MAP_CELL_MAP_HEIGHT;
	m_LastCellSkipHeight.cy = nTemp - MAP_CELL_MAP_HEIGHT * m_PaintCell.bottom;
	m_PaintCell.bottom += m_PaintCell.top;
}

//���
void KScenePlaceMapC::Free()
{
	/*if (m_pEntireMap)
	{
		release_image(m_pEntireMap);
		m_pEntireMap = NULL;
	}*/
	//-------------------------------------------
	/*if (m_ppbtBarrier)
	{
		for (int i = 0; i < m_nMapWidth * m_nMapHeight; i++)
		{
			if (!m_ppbtBarrier[i])
				continue;
			delete []m_ppbtBarrier[i];
			m_ppbtBarrier[i] = NULL;
		}
		delete [] m_ppbtBarrier;
		m_ppbtBarrier = NULL;
	}
	*/
	/*if (m_pbyLoadFlag)
	{
		delete []m_pbyLoadFlag;
		m_pbyLoadFlag = NULL;
	}*/
	m_nMapWidth  = 0;
	m_nMapHeight = 0;

	Terminate();
}

//��̬����
void KScenePlaceMapC::PaintMap(int nX, int nY)
{
	m_MapPos.left = nX;
	m_MapPos.top  = nY;
	m_MapPos.right  = m_MapPos.left + m_Size.cx;
	m_MapPos.bottom = m_MapPos.top  + m_Size.cy;
	//g_pRepresent
	if (m_bHavePicMap  && m_bPicMapType==0)
	{
		//----��������ͼ----
		if (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_PIC)
			PaintMapPic(nX, nY);

		//---���ƶ���λ��----
		//PaintCharacters(nX, nY);
		//---�����Լ�λ��----
		int nNpcIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		if (nNpcIdx >= 0 && Npc[nNpcIdx].m_RegionIndex >= 0)
		{
			int nNpcX = LOWORD(Npc[nNpcIdx].m_dwRegionID) * MAP_A_REGION_NUM_MAP_PIXEL_H + Npc[nNpcIdx].m_MapX * 2;
			int nNpcY = HIWORD(Npc[nNpcIdx].m_dwRegionID) * MAP_A_REGION_NUM_MAP_PIXEL_V + Npc[nNpcIdx].m_MapY;
            //char msg[32];
			//sprintf(msg,"nNpcX:%dnNpcY:%d",nNpcX,nNpcY);
			//messageBox(msg,"FootSpot");
			KRUShadow	FootSpot;
			if (nNpcX >= m_MapCoverArea.left && nNpcX < m_MapCoverArea.right &&
				nNpcY >= m_MapCoverArea.top  && nNpcY < m_MapCoverArea.bottom)
			{//�滭�Լ�	 m_FocusPosition.x / MAP_SCALE_H - m_Size.cx / 2;//-64
				FootSpot.Color.Color_dw = m_uSelfColor;
				FootSpot.oPosition.nX   = nX + nNpcX - m_MapCoverArea.left - 1;
				FootSpot.oPosition.nY   = nY + nNpcY - m_MapCoverArea.top  - 1;
				FootSpot.oEndPos.nX     = FootSpot.oPosition.nX + 3;
				FootSpot.oEndPos.nY     = FootSpot.oPosition.nY + 3;
				//messageBox("�滭��","�滭��");
				if (g_GameWorld)
				   g_GameWorld->DrawPrimitives_RolePoint(0,1,&FootSpot,0,RU_T_SHADOW,2,true);
			}
		}
	}
	else
	{
		//----��������ͼ----
		if (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_PIC)
			PaintMapPic(nX, nY);
	}
}

#define	PLACE_MAP_SYMBOL_SIZE			5
#define NORMAL_FONTSIZE					12


void KScenePlaceMapC::PaintSymbol(int nX ,int nPosX,int nPosY,char *nContent,int Kind)
{
	//if (g_pRepresent)
	{
		int nNpcX = 0;
		int nNpcY = 0;

			if (Kind <= -1)
				return;

			nNpcX = nPosX + nX + (m_FocusLimit.left / MAP_SCALE_H - m_Size.cx / 2) - m_MapCoverArea.left - 5;
			nNpcY = nPosY + m_MapPos.top + (m_FocusLimit.top / MAP_SCALE_V - m_Size.cy / 2) - m_MapCoverArea.top - 5;

			switch (Kind)
			{
				case 0:
				break;
				case 1:	//�ַ���
				{
					POINT NamePoint;
					RECT NamePos;
					int NameLen = g_StrLen(nContent) * NORMAL_FONTSIZE / 2;
					NamePoint.x = NamePos.left = nNpcX - (NameLen / 2);
					NamePos.right = nNpcX + (NameLen / 2);
					NamePoint.y = NamePos.top = nNpcY;
					NamePos.bottom = nNpcY + (NORMAL_FONTSIZE + 1);

					if (NamePos.left < m_MapPos.left)
						NamePoint.x = m_MapPos.left;
					if (NamePos.right > m_MapPos.right - (NORMAL_FONTSIZE / 2))
						NamePoint.x -= (NamePos.right - m_MapPos.right + (NORMAL_FONTSIZE / 2));
					if (NamePos.top < m_MapPos.top)
						NamePoint.y = m_MapPos.top;
					if (NamePos.bottom > m_MapPos.bottom)
						NamePoint.y -= (NamePos.bottom - m_MapPos.bottom);

					if (NamePoint.x > nNpcX)
						return;
					if (NamePoint.y + (NORMAL_FONTSIZE + 1) < nNpcY)
						return;
					if (NamePoint.x < NamePos.left - (NameLen / 2))
						return;
					if (NamePoint.y > NamePos.bottom)
						return;

//					g_pRepresent->OutputText(NORMAL_FONTSIZE,nContent, KRF_ZERO_END, NamePoint.x, NamePoint.y, 0x00ffff00);
				}
				break;
				case 2:	//С�Ƶ�
				{
					KRUImage	RUSymbol;
					strcpy(RUSymbol.szImage, nContent);

					KImageParam	Param;
//					g_pRepresent->GetImageParam(nContent, &Param, ISI_T_SPR);

					BYTE btSymbolW = Param.nWidth;
					BYTE btSymbolH = Param.nHeight;

					RUSymbol.nType = ISI_T_SPR;
					RUSymbol.Color.Color_b.a = 255;
					RUSymbol.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
					RUSymbol.uImage = 0;
					RUSymbol.nISPosition = IMAGE_IS_POSITION_INIT;
					RUSymbol.bRenderFlag = 0;
					RUSymbol.nFrame = 0;
					RUSymbol.oPosition.nX = nNpcX;
					RUSymbol.oPosition.nY = nNpcY;
					RUSymbol.oPosition.nZ = 0;

					RECT PosSymbol;

					PosSymbol.left = RUSymbol.oPosition.nX + btSymbolW;
					PosSymbol.right = RUSymbol.oPosition.nX;
					PosSymbol.top = RUSymbol.oPosition.nY + btSymbolH;
					PosSymbol.bottom = RUSymbol.oPosition.nY + btSymbolH;

					if ((PosSymbol.left - btSymbolW) < m_MapPos.left)
						RUSymbol.oPosition.nX += m_MapPos.left - (PosSymbol.left - btSymbolW);
					if (PosSymbol.top - btSymbolH < m_MapPos.top)
						RUSymbol.oPosition.nY += m_MapPos.top - (PosSymbol.bottom - btSymbolH);
					if (PosSymbol.bottom > m_MapPos.bottom)
						RUSymbol.oPosition.nY -= PosSymbol.bottom - m_MapPos.bottom;
					if (PosSymbol.right + btSymbolW > m_MapPos.right)
						RUSymbol.oPosition.nX -= (PosSymbol.right + btSymbolW) - m_MapPos.right;

					if (PosSymbol.left < m_MapPos.left ||
						PosSymbol.top < m_MapPos.top ||
						PosSymbol.right > m_MapPos.right ||
						PosSymbol.bottom > m_MapPos.bottom + btSymbolH)
						break;
//					g_pRepresent->DrawPrimitives(1, &RUSymbol, RU_T_IMAGE, true);
				}
				break;
				default:
					break;
			}
	}
}




void KScenePlaceMapC::MapGoTo()
{

	/*
	  	int         m_DestRegIdx_X;		    //  Ŀ�����ڵ����� ����
		int         m_DestRegIdx_Y;


		int         m_YscRegIdx_X;		    //  �������ڵ����� ����
		int         m_YscRegIdx_Y;

	*/
	int nMovePox_X,nMovePox_Y,nMoveTempRegIdx_X,nMoveTempRegIdx_Y;
	if (m_GreenLineYsc.x<=0 && m_GreenLineYsc.y<=0)
		return;

	nMovePox_X = m_GreenLineYsc.x*16;
	nMovePox_Y = m_GreenLineYsc.y*32;

	nMoveTempRegIdx_X = m_YscTempRegIdx_X;
	nMoveTempRegIdx_Y = m_YscTempRegIdx_Y;

	int nMapID=SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_SubWorldID;

	/*int  nYx,nYy;
	int nRegidx =(m_YscRegIdx_X - m_MapRect.top) * m_nMapWidth +(m_YscRegIdx_Y - m_MapRect.left);
	//int	nRegion = (nY - m_sMapRect.top) * m_nMapWidth + nX - m_sMapRect.left;
	int nBarrier=0,lRet=0,lType=0;

	nYx  = m_FocusPosition.x%16;
	nYy  = m_FocusPosition.y%32;

	if (nRegidx>0 && nRegidx < m_nMapWidth*m_nMapHeight)
		nBarrier = m_ppbtBarrier[nRegidx][nYy * REGION_GRID_WIDTH + nYx];

	if (nBarrier>=0)
	{
		lRet = nBarrier & 0x0000000f;
		lType = (nBarrier >> 4) & 0x0000000f;
	}

	if  (lType>0)
	{
		return;
	} */

	//��������Ѱ·��---------------------------------
	 /*if (m_YscRegIdx_X == m_DestRegIdx_X && m_YscRegIdx_Y == m_DestRegIdx_Y)
	 {//ͬһ������
		 nMovePox_X = m_YscRegIdx_X*512;
		 nMovePox_Y = m_YscRegIdx_Y*1024;
		 //m_FocusPosition.x/8/32, m_FocusPosition.y/16/32); //�������ڵĸ�������
		 Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_BtnFindPath=FALSE;
		 m_GreenLineDes.x = -123456;                         //����Ŀ������
		 m_GreenLineDes.y = -123456;
		 m_GreenLineYsc.x = 0;
		 m_GreenLineYsc.y = 0;
		 m_DestRegIdx_X  = 0;
		 m_DestRegIdx_Y  = 0;
		 m_YscRegIdx_X   = 0;
		 m_YscRegIdx_Y   = 0;
	 }
	 else
	 {
		if (m_YscRegIdx_X > m_DestRegIdx_X)
		{//Ŀ��������� ���
			if (m_YscRegIdx_Y >= m_DestRegIdx_Y)
			{//Ŀ���� ���� �·�	 ���Գ��Եݼ����� ����

			}

		}
	 } */
	//-----------------------------------------------
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendSerCommand(do_run, nMovePox_X,nMovePox_Y);
	SendClientCmdRun(nMovePox_X,nMovePox_Y,nMapID);
}


void KScenePlaceMapC::PaintPointYellow(int nX, int nY,int nNpcX, int nNpcY,char szName[64],int nType) //���Ƶ�
{
	//if (m_bHavePicMap && g_pRepresent)
	{

		if (nNpcX >= m_MapCoverArea.left && nNpcX < (m_MapCoverArea.right - 5) &&
			nNpcY >= m_MapCoverArea.top  && nNpcY < (m_MapCoverArea.bottom - 5))
		{
		KRUImage YellowPoint;
		YellowPoint.nType = ISI_T_SPR;
		YellowPoint.Color.Color_b.a = 255;
		YellowPoint.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		YellowPoint.uImage = 0;
		YellowPoint.nISPosition = IMAGE_IS_POSITION_INIT;
		YellowPoint.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
		if (nType==2)
			strcpy(YellowPoint.szImage, szName);
		else
		    strcpy(YellowPoint.szImage, "\\Spr\\obj\\box\\YellowPoint.spr");

		YellowPoint.nFrame = 0;
		YellowPoint.oPosition.nX = nX;
		YellowPoint.oPosition.nY = nY;
		YellowPoint.oPosition.nZ = 0;
//		g_pRepresent->DrawPrimitives(1, &YellowPoint, RU_T_IMAGE, true);

		if (nType==2)
			return;

		if (nNpcX + strlen(szName) < m_MapCoverArea.left)
		{
			nNpcX = m_MapCoverArea.left;
		}
		else if (nNpcX +strlen(szName) > m_MapCoverArea.right)
		{
			nNpcX = m_MapCoverArea.right;
		}
//		    g_pRepresent->OutputText(12, szName, KRF_ZERO_END,nX-20, nY-12, 0xFFFF00, 0, TEXT_IN_SINGLE_PLANE_COORD,0XFF000000);
		}
	}
}
//���ƶ���λ��
void KScenePlaceMapC::PaintCharacters(int nX, int nY)
{
	bool bCharacters = (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_CHARACTER) != 0;
	bool bPartners = (m_uMapShowElems & SCENE_PLACE_MAP_ELEM_PARTNER) != 0;
	if (bCharacters == false || bPartners == false)
		return;

#define	MAX_NUM_PARTNER		8
	POINT		oPartnerPos[MAX_NUM_PARTNER];
	int			nNumPartner = 0;
	int			nIsInTeam = Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag;


	KRUShadow	FootSpot[MAX_NUM_CHARACTER];
	int			nNumSpot = 0;
	// ��ʾ������Һ���ͨnpc
	int nNpcIdx = 0;
	while (nNpcIdx = NpcSet.GetNextIdx(nNpcIdx))
	{
		if (Npc[nNpcIdx].m_RegionIndex == -1)
			continue;

		int nNpcX = LOWORD(Npc[nNpcIdx].m_dwRegionID) * MAP_A_REGION_NUM_MAP_PIXEL_H + Npc[nNpcIdx].m_MapX * 2;
		int nNpcY = HIWORD(Npc[nNpcIdx].m_dwRegionID) * MAP_A_REGION_NUM_MAP_PIXEL_V + Npc[nNpcIdx].m_MapY;
		if (nNpcX < m_MapCoverArea.left || nNpcX >= m_MapCoverArea.right ||
			nNpcY < m_MapCoverArea.top  || nNpcY >= m_MapCoverArea.bottom)
		{
			continue;
		}

		bool bValidNpc = false;
		unsigned int	uColor;

		if (Npc[nNpcIdx].m_Kind == kind_normal && bCharacters)
		{//����
			bValidNpc = true;
			uColor = m_uFightNpcColor;
			/*int nType=0;
		        nType =Npc[nNpcIdx].m_cGold.GetGoldType();

				if (nType ==1)
					uColor = 0xff00ff00;	// �̹�  ��Ǯ
				else if (nType >=2 && nType <= 4)
					uColor=GetColor("0,0,160");  //����
				else if (nType >=5 && nType <= 11)
                    uColor=GetColor("234,289,11");
				else if (nType >=12 && nType <= 16)
					uColor=GetColor("210,0,0");
			  */

		}
		else if (Npc[nNpcIdx].m_Kind == kind_dialoger && bCharacters)
		{//�Ի�
			bValidNpc = true;
			uColor = m_uNormalNpcColor;
		}
		else if (Npc[nNpcIdx].m_Kind == kind_player && nNpcIdx != Player[CLIENT_PLAYER_INDEX].m_nIndex)
		{
			if (bPartners && //Ҫ��ʾ����
				nIsInTeam && //���������״̬
				((DWORD)g_Team[0].m_nCaptain == Npc[nNpcIdx].m_dwID ||	    //���������ڶ���Ķӳ�
					g_Team[0].FindMemberID(Npc[nNpcIdx].m_dwID) >= 0) &&	//���������ڶ���Ķ�Ա
				nNumPartner < MAX_NUM_PARTNER)
			{
				oPartnerPos[nNumPartner].x = nNpcX;
				oPartnerPos[nNumPartner].y = nNpcY;
				nNumPartner ++;
			}
			else if (bCharacters)
			{
				bValidNpc = true;
				uColor = m_uPlayerColor;
			}
			if(NpcSet.GetRelation(nNpcIdx,Player[CLIENT_PLAYER_INDEX].m_nIndex) == relation_enemy && Npc[nNpcIdx].m_Hide.nTime > 0)
			{
				bValidNpc =false;
			}
		}

		if (bValidNpc)
		{
			FootSpot[nNumSpot].Color.Color_dw = uColor;
			FootSpot[nNumSpot].oPosition.nX = nX + nNpcX - m_MapCoverArea.left - 1;
			FootSpot[nNumSpot].oPosition.nY = nY + nNpcY - m_MapCoverArea.top  - 1;
			FootSpot[nNumSpot].oEndPos.nX = FootSpot[nNumSpot].oPosition.nX + 3;
			FootSpot[nNumSpot].oEndPos.nY = FootSpot[nNumSpot].oPosition.nY + 3;
			nNumSpot++;
			if (nNumSpot == MAX_NUM_CHARACTER) //�����ٸ���
			{
//				g_pRepresent->DrawPrimitives(MAX_NUM_CHARACTER, &FootSpot[0], RU_T_SHADOW, true);
				nNumSpot = 0;
			}
		}
	}

	if (nNumSpot)
	{
//		g_pRepresent->DrawPrimitives(nNumSpot, &FootSpot[0], RU_T_SHADOW, true);
		nNumSpot = 0;
	}
	if (nNumPartner)
	{//����
		//_ASSERT(nNumPartner <= MAX_NUM_CHARACTER);
		if (nNumPartner > MAX_NUM_CHARACTER)
			return;

		for (int i = 0; i < nNumPartner; ++i)
		{
			FootSpot[i].Color.Color_dw = m_uTeammateColor;
			FootSpot[i].oPosition.nX = nX + oPartnerPos[i].x - m_MapCoverArea.left - 1;
			FootSpot[i].oPosition.nY = nY + oPartnerPos[i].y - m_MapCoverArea.top  - 1;
			FootSpot[i].oEndPos.nX = FootSpot[i].oPosition.nX + 3;
			FootSpot[i].oEndPos.nY = FootSpot[i].oPosition.nY + 3;
		}
//		g_pRepresent->DrawPrimitives(nNumPartner, &FootSpot[0], RU_T_SHADOW, true);
	}
}

//��������ͼ
void KScenePlaceMapC::PaintMapPic(int nX, int nY)
{
	//_ASSERT(g_pRepresent);
	if  (g_GameWorld==NULL || !m_szEntireMapFile[0]) return;

	KRUImagePart	Img;
	Img.bRenderFlag = 0;
	Img.bRenderStyle = IMAGE_RENDER_STYLE_OPACITY;
	Img.Color.Color_dw = 0;
	Img.nFrame = 0;
	Img.nType = ISI_T_SPR;
	Img.oPosition.nY = nY;
	//TOP = 0
	//char msg[128];
	//sprintf(msg,"MapW:%d,H:%d,fx:%d,fy:%d,Lx:%d,Ly:%d",m_nMapWidth,m_nMapHeight,m_FirstCellSkipWidth.cx,m_FirstCellSkipWidth.cy,m_LastCellSkipHeight.cx,m_LastCellSkipHeight.cy);
    //messageBox(msg,"PaintMapPic");
    /*if (m_bPicMapType==0)
	for (int v = m_PaintCell.top; v < m_PaintCell.bottom; ++v)
	{//�滭25��?
		if (v != m_PaintCell.top)
			Img.oImgLTPos.nY = 0;
		else
			Img.oImgLTPos.nY = m_FirstCellSkipWidth.cy;//��ȡ��top����
		if (v != m_PaintCell.bottom - 1)
			Img.oImgRBPos.nY = MAP_CELL_MAP_HEIGHT;
		else
			Img.oImgRBPos.nY = m_LastCellSkipHeight.cy; //��ȡ�ĸ߶�

		Img.oPosition.nX = nX;//ĳ��λ�û滭

		for (int h = m_PaintCell.left; h < m_PaintCell.right; ++h)
		{
			if (h != m_PaintCell.left)
				Img.oImgLTPos.nX = 0;
			else
				Img.oImgLTPos.nX = m_FirstCellSkipWidth.cx; //ͼ�ڵ�ƫ��

			if (h != m_PaintCell.right - 1)
				Img.oImgRBPos.nX = MAP_CELL_MAP_WIDTH;      //��ȡ�Ŀ��
			else
				Img.oImgRBPos.nX = m_LastCellSkipHeight.cx; //��ȡ�Ŀ��

			Img.nISPosition = m_ElemsList[v][h].sISPosition;
			strcpy(Img.szImage, m_ElemsList[v][h].szImageName);//�������ͼ��
			Img.uImage      = m_ElemsList[v][h].uImageId;
//			g_pRepresent->DrawPrimitives(1, &Img, RU_T_IMAGE_PART, true);
			m_ElemsList[v][h].sISPosition = Img.nISPosition;
			m_ElemsList[v][h].uImageId    = Img.uImage;

			Img.oPosition.nX += Img.oImgRBPos.nX - Img.oImgLTPos.nX;
		}

		Img.oPosition.nY += Img.oImgRBPos.nY - Img.oImgLTPos.nY;

	}*/
	if (m_bPicMapType==0)
    {
		std::string nCurFilePath = m_szEntireMapFile;
		if (g_GameWorld)
		    g_GameWorld->DrawPrimitives_minemap(nX,nY,m_MapRect,nCurFilePath);
    }
	else if (m_bPicMapType==1)
	{
		Img.nType = ISI_T_SPR;
        Img.oPosition.nX = nX;
        strcpy(Img.szImage,"\\maps\\С��ͼ\\С��ͼ.spr");
        SetClientSpr(Img.szImage,Img.oPosition.nX,Img.oPosition.nY,0,1);
		//g_pRepresent->DrawPrimitives(1, &Img,RU_T_IMAGE,true);
	}
}
//�������ϵ�
void KScenePlaceMapC::SetClientSpr(char *nSprPath,int nxLeft,int nyTop,int nzPos,int i)
{
	            if (!g_GameWorld) return;
				KRUImage RUIconImage;
				RUIconImage.nType = ISI_T_SPR;
				RUIconImage.Color.Color_b.a = 255;
				RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
				RUIconImage.uImage = 0;
				RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
				RUIconImage.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
				strcpy(RUIconImage.szImage, nSprPath);  //�ı�����
				RUIconImage.oPosition.nX = nxLeft;
				RUIconImage.oPosition.nY = nyTop;
				RUIconImage.oPosition.nZ = nzPos;
				KImageParam	 imgParam;
				imgParam.nNumFrames = 0;

				//g_pRepresent->GetImageParam(nSprPath, &imgParam, ISI_T_SPR);

				if (imgParam.nNumFrames > 0 && m_DurFrame[i] < imgParam.nNumFrames)
				{
					IR_NextFrame(m_DurFrame[i], imgParam.nNumFrames,imgParam.nInterval,uFlipTime[i]);
					RUIconImage.nFrame = m_DurFrame[i];
				}
				else
				{
					m_DurFrame[i]=0;
					RUIconImage.nFrame = 0;
				}
				//g_GameWorld->DrawPrimitives_minemap(RUIconImage.oPosition.nX,RUIconImage.oPosition.nY,m_MapRect,nCurFilePath);
                //g_pRepresent->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, TRUE);
}

//����С��ͼ�Ĵ�С����λ�����ص㣩
void KScenePlaceMapC::SetSize(int cx, int cy)
{
	if (m_Size.cx != cx || m_Size.cy != cy)
	{
		if (cx > MAP_MAX_SUPPORT_WIDTH)
			cx = MAP_MAX_SUPPORT_WIDTH;
		else if (cx < 0)
			cx = 0;
		if (cy > MAP_MAX_SUPPORT_HEIGHT)
			cy = MAP_MAX_SUPPORT_HEIGHT;
		else if (cy < 0)
			cy = 0;
		int nDLimit = (cx - m_Size.cx) * MAP_SCALE_H / 2;
		m_FocusLimit.left += nDLimit;
		if (m_FocusLimit.right != RIGHT_BOTTOM_NO_LIMIT)
		{
			m_FocusLimit.right -= nDLimit;
			if (m_FocusLimit.right < m_FocusLimit.left)
				m_FocusLimit.right = m_FocusLimit.left;
		}
		nDLimit = (cy - m_Size.cy) * MAP_SCALE_V / 2;
		m_FocusLimit.top += nDLimit;
		if (m_FocusLimit.bottom != RIGHT_BOTTOM_NO_LIMIT)
		{
			m_FocusLimit.bottom -= nDLimit;
			if (m_FocusLimit.bottom < m_FocusLimit.top)
				m_FocusLimit.bottom = m_FocusLimit.top;
		}

		m_Size.cx = cx;
		m_Size.cy = cy;

		SetFocusPosition(m_FocusPosition.x, m_FocusPosition.y, true);
		//�Ƿ�����ϰ��� С��ͼ--------------------------------------
		/*int		i, j;
		int		x, y;
		x = LOWORD(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwRegionID) - m_nShowWidth / 2;
		y = HIWORD(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_dwRegionID) - m_nShowHeight / 2;
		for (i = x; i < x + m_nShowWidth; i++)
		{
			for (j = y; j < y + m_nShowHeight; j++)
			{
				KRegion::LoadLittleMapData(i, j, SubWorld[0].m_szMapPath, GetBarrierBuf(i, j));
				SetHaveLoad(i, j);
			}
		}*/
	   //-------------------------------------------------------------
	}
}

//����ֵ��ʾ�Ƿ��е�ͼ
int KScenePlaceMapC::GetMapRect(RECT* pRect)
{
	if (pRect)
	{
		pRect->left = m_EntireMapLTPosition.x;
		pRect->top  = m_EntireMapLTPosition.y;
		if (m_szEntireMapFile[0])
		{//m_pEntireMap->nWidth
			pRect->right  = m_EntireMapLTPosition.x + m_CurPicWidth * MAP_SCALE_H;
			pRect->bottom = m_EntireMapLTPosition.y + m_CurPicHeight * MAP_SCALE_V;
		}
		else
		{
			pRect->right  = m_EntireMapLTPosition.x;
			pRect->bottom = m_EntireMapLTPosition.y;
		}
	}
	return m_bHavePicMap;
}
//���С��ͼ��ִ�еĺ��� ���û��ߵ�λ�� nX,nYΪ��������
void KScenePlaceMapC::SetGreenLine(int nX, int nY, BOOL bSearch)
{
	char nDebugmsg[200];
	if (!bSearch)	//��С��ͼѰ· ��꽹�� ת���ɳ�������
	{

		if ( nX > m_MapPos.left && nY > m_MapPos.top && nX < m_MapPos.right && nY < m_MapPos.bottom)
		{//�ڷ�Χ��

		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_BtnFindPath	=TRUE;

		m_GreenLineDes.x = nX - m_MapPos.left + m_MapCoverArea.left + 1; // ����Ŀ������
		m_GreenLineDes.y = nY - m_MapPos.top + m_MapCoverArea.top + 1;

		m_GreenLineYsc.x = m_GreenLineDes.x;
		m_GreenLineYsc.y = m_GreenLineDes.y;
		sprintf(nDebugmsg,"m_GreenLineYsc:%d,m_GreenLineYsc.y:%d",m_GreenLineYsc,m_GreenLineYsc.y);
		Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg(nDebugmsg);


		   /*int nMapID=SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_SubWorldID;
			    Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendSerCommand(do_run, m_GreenLineDes.x*16, m_GreenLineDes.y*32);
			    SendClientCmdRun(m_GreenLineDes.x*16, m_GreenLineDes.y*32,nMapID);

                KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CAN_NOT_PK_SUNYI);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sMsg.nMsgLen=TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
				bFlag = 0; */
		}
		else
		{
			m_GreenLineDes.x = -123456;  //����Ŀ������
			m_GreenLineDes.y = -123456;
		}
	}
	else	// �����������Ѱ·
	{
		m_GreenLineDes.x = nX*MAP_SCALE_H;
		m_GreenLineDes.y = nY*MAP_SCALE_H;
//		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode == 0) //��ս��ģʽ�ſ���˲��
//		{
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ClientShowMsg("tim duong 2");
		    int nMapID=SubWorld[Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex].m_SubWorldID;
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendSerCommand(do_run, m_GreenLineDes.x*16, m_GreenLineDes.y*32);
			SendClientCmdRun(m_GreenLineDes.x*16, m_GreenLineDes.y*32,nMapID);
			// KSystemMessage	sMsg;
			// sprintf(sMsg.szMessage, strCoreInfo[MSG_CAN_NOT_PK_SUNYI].c_str());
			// sMsg.eType = SMT_NORMAL;
			// sMsg.byConfirmType = SMCT_NONE;
			// sMsg.byPriority = 0;
			// sMsg.byParamSize = 0;
			// sMsg.nMsgLen=TEncodeText(sMsg.szMessage, strlen(sMsg.szMessage));
			// CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (uintptr_t)&sMsg, 0);
			//bFlag = 0;
//		}
//		else
//		{
//			SendClientCmdSunyi(53,m_GreenLineDes.x*16,m_GreenLineDes.y*32); //����˲������
//		}

	}
}

//--------------------------------------------------------------------------
//	���ܣ����ַ�����ʾ����ɫ��ϢתΪ��ֵ��ʾ
//--------------------------------------------------------------------------
unsigned int KScenePlaceMapC::GetColor(LPCTSTR pString)
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

// void KScenePlaceMapC::DrawGreenLine(int nX, int nY, int nX2, int nY2,LPCTSTR pString,int nLineNum) // ������
// {
// 	if (nX < -100000 || nY < -100000)
// 		return;
// 	KRULine Line[2];
// 	Line[0].oPosition.nX = nX;  //���
// 	Line[0].oPosition.nY = nY;
// 	Line[0].oPosition.nZ = 0;
// 	Line[0].oEndPos.nX = nX2; //�յ�
// 	Line[0].oEndPos.nY = nY2;
// 	Line[0].oEndPos.nZ = 0;
// 	Line[0].Color.Color_dw = GetColor(pString);	  //��ɫ0,225,64

// 	Line[1].oPosition.nX = nX+1;  //���
// 	Line[1].oPosition.nY = nY;
// 	Line[1].oPosition.nZ = 0;
// 	Line[1].oEndPos.nX = nX2+1; //�յ�
// 	Line[1].oEndPos.nY = nY2;
// 	Line[1].oEndPos.nZ = 0;
// 	Line[1].Color.Color_dw = GetColor(pString);	  //��ɫ

// 	g_GameWorld->DrawPrimitives(nLineNum,Line, RU_T_LINE, TRUE);
// }

//�����ȡ��Ѱ·����״̬

void KScenePlaceMapC::DelGreenLine() // ȡ��������
{
	m_GreenLineDes.x = -123456;  //����Ŀ������
	m_GreenLineDes.y = -123456;

	m_GreenLineYsc.x = 0;
	m_GreenLineYsc.y = 0;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_BtnFindPath=FALSE;
}

void KScenePlaceMapC::GetMapSizePos(KSceneMapSizePos* pInfo) // ��ȡ�����Ĵ�С
{
	pInfo->Arealeft = m_MapCoverArea.left;  //��ȡ�����Ĵ�С
	pInfo->Areatop  = m_MapCoverArea.top;
}

//��С����
void KScenePlaceMapC::PaintPointQiZi(int nX, int nY,int nNpcX, int nNpcY,char szName[64]) //���Ƶ�
{
	if (m_bHavePicMap )
	{//&& g_pRepresent

		if (nNpcX >= m_MapCoverArea.left && nNpcX < (m_MapCoverArea.right - 5) &&
			nNpcY >= m_MapCoverArea.top  && nNpcY < (m_MapCoverArea.bottom - 5))
		{
			KRUImage YellowPoint;
			YellowPoint.nType = ISI_T_SPR;
			YellowPoint.Color.Color_b.a = 255;
			YellowPoint.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
			YellowPoint.uImage = 0;
			YellowPoint.nISPosition = IMAGE_IS_POSITION_INIT;
			YellowPoint.bRenderFlag = RUIMAGE_RENDER_FLAG_REF_SPOT;
			strcpy(YellowPoint.szImage, "\\Spr\\Ui3\\С��ͼ\\��ͼС����.spr");
			YellowPoint.nFrame = 0;
			YellowPoint.oPosition.nX = nX;
			YellowPoint.oPosition.nY = nY;
			YellowPoint.oPosition.nZ = 0;
//			g_pRepresent->DrawPrimitives(1, &YellowPoint, RU_T_IMAGE, true);
			if (nNpcX + strlen(szName) < m_MapCoverArea.left)
			{
				nNpcX = m_MapCoverArea.left;
			}
			else if (nNpcX +strlen(szName) > m_MapCoverArea.right)
			{
				nNpcX = m_MapCoverArea.right;
			}
//			g_pRepresent->OutputText(12, szName, KRF_ZERO_END,nX-20, nY-12, 0xFFFF00, 0, TEXT_IN_SINGLE_PLANE_COORD,0XFF000000);
		}
	}
}


//---------------------------------------------------------------------------
//	���ܣ����ĳ��region���ϰ����ݵĴ洢��ַ
//---------------------------------------------------------------------------
void KScenePlaceMapC::GetMapBarrierBuf(INT **lpbtObstacle)
{
	/*if (!lpbtObstacle)
		return;
	//int	nRegion  = nY *m_MapWidth  + nX;  //����߶� �ж��ٸ� ����Ŀ��(nY - m_MapCoverArea.top) * m_nMapWidth + nX - m_MapCoverArea.left;
	//return m_ppbtBarrier[nRegion];

//	INT *m_TempbtBarrier = m_ppbtBarrier;
	lpbtObstacle = m_ppbtBarrier;
	*/
}


//---------------------------------------------------------------------------
//	���ܣ����ĳ��region���ϰ����ݵĴ洢��ַ
//---------------------------------------------------------------------------
/*INT*	KScenePlaceMapC::GetBarrierBuf(int nX, int nY)
{
	if (!m_ppbtBarrier)
		return NULL;
	int	nRegion  = nY * m_nMapWidth + nX;  //����߶� �ж��ٸ� ����Ŀ��(nY - m_MapCoverArea.top) * m_nMapWidth + nX - m_MapCoverArea.left;
	//if (m_pbyLoadFlag[nRegion])
	//	return NULL;
	if (!m_ppbtBarrier[nRegion])
	{
    	m_ppbtBarrier[nRegion] = (INT*)new INT[REGION_GRID_WIDTH * REGION_GRID_HEIGHT];
		memset(m_ppbtBarrier[nRegion],0, sizeof(INT) * REGION_GRID_WIDTH * REGION_GRID_HEIGHT);
	}
	return m_ppbtBarrier[nRegion];

}	*/
//---------------------------------------------------------------------------
//	���ܣ��趨������ĳ��region���ϰ�����
//---------------------------------------------------------------------------
void	KScenePlaceMapC::SetHaveLoad(int nX, int nY)
{
	if (nX < m_MapCoverArea.left ||
		nX > m_MapCoverArea.right ||
		nY < m_MapCoverArea.top ||
		nY > m_MapCoverArea.bottom)
		return;
	//if (m_pbyLoadFlag)
	//	m_pbyLoadFlag[(nY - m_MapCoverArea.top) * m_nMapWidth + nX - m_MapCoverArea.left] = 1;
}


//---------------------------------------------------------------------------
//	���ܣ�����
//---------------------------------------------------------------------------
void	KScenePlaceMapC::Draw(int nX, int nY)
{
	//if (!m_nShowFlag)
	//	return;
	/*if (!this->m_pbyLoadFlag || !this->m_ppbtBarrier) //���û���ϰ� ��û�м����ϰ��Ͳ���ʾ
		return;

	int		nRx, nRy, nOx, nOy, nSx, nSy, nTx, nTy;

	nSx = nX - REGION_GRID_WIDTH * m_nShowWidth / 2;
	nSy = nY - REGION_GRID_HEIGHT * m_nShowHeight / 2;

	// ��ʾ�ϰ�
	int m_nPointPos = 0;
	KRUPoint	m_sPoint[MAX_NUM_CHARACTER];	// ���ڻ���
	for (int i = 0; i < m_nShowWidth * REGION_GRID_WIDTH; i++)
	{
		for (int j = 0; j < m_nShowHeight * REGION_GRID_HEIGHT; j++)
		{
			nRx = (nSx + i) / REGION_GRID_WIDTH;
			nRy = (nSy + j) / REGION_GRID_HEIGHT;
			if (nRx < m_EntireMapLTPosition.x ||
				nRx > m_EndMapRBPosition.x ||
				nRy < m_EntireMapLTPosition.y ||
				nRy > m_EndMapRBPosition.y)
				continue;
			nRx -= m_EntireMapLTPosition.x;
			nRy -= m_EntireMapLTPosition.y;
			if (!m_pbyLoadFlag[nRy * m_nMapWidth + nRx] || !m_ppbtBarrier[nRy * m_nMapWidth + nRx])
				continue;
			nOx = (nSx + i) % REGION_GRID_WIDTH;
			nOy = (nSy + j) % REGION_GRID_HEIGHT;

			if (m_ppbtBarrier[nRy * m_nMapWidth + nRx][nOy * REGION_GRID_WIDTH + nOx])
			{//������ϰ�
				m_sPoint[m_nPointPos].Color.Color_dw = GetColor("0,225,64");	  //��ɫm_sBarrierColor.Color_dw;
				m_sPoint[m_nPointPos].oPosition.nZ = 0;
				m_sPoint[m_nPointPos].oPosition.nX = 300 + i * 2;
				m_sPoint[m_nPointPos].oPosition.nY = 200 + j;
				m_nPointPos++;
				if (m_nPointPos == MAX_NUM_CHARACTER)
				{
//					g_pRepresent->DrawPrimitives(m_nPointPos, m_sPoint, RU_T_POINT, true);
					m_nPointPos = 0;
				}
			}
		}
	}

	if (m_nPointPos > 0 && m_nPointPos < MAX_NUM_CHARACTER)
	{
//		g_pRepresent->DrawPrimitives(m_nPointPos, m_sPoint, RU_T_POINT, true);
		m_nPointPos = 0;
	}*/
}

