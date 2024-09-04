#include "KCore.h"
#include "KNpc.h"
#include "KNpcSet.h"
#include "KMissleSet.h"
#include "KObjSet.h"
#include "KPlayerSet.h"
#include "KPlayer.h"
#include "KMissle.h"
//#include "KObj.h"
//#include "KWorldMsg.h"
#include "KSubWorld.h"
#include "KItem.h"   //--������ӵ�
#include "gamescene/ObstacleDef.h"
#include "engine/KTabFileCtrl.h"
//ԭ���е�
#include "gamescene/SceneDataDef.h"
#include "KRegion.h"
//#include "MyAssert.H"
//#pragma pack(1);

KRegion::KRegion()
{
	m_nIndex		= -1;
	m_RegionID		= -1;
	m_nActive		= 0;
	m_nNpcSyncCounter = 0;
	m_nObjSyncCounter = 0;
	//m_nLoopCounter    = 0;
	m_nWidth		= 0;
	m_nHeight		= 0;
	ZeroMemory(m_nConnectRegion, 8 * sizeof(int));
	memset(m_pConRegion, 0, sizeof(m_pConRegion));
//	m_pNpcRef		= NULL;
//	m_pObjRef		= NULL;
//	m_pMslRef		= NULL;

}

KRegion::~KRegion()
{

	m_cNpcRef.Clear();
	m_cObjRef.Clear();

	/*if (m_pMslRef)
	{
		delete [] m_pMslRef;
		m_pMslRef=NULL;
	} 
	if (m_pNpcRef)
	{
		delete [] m_pNpcRef;
	    m_pNpcRef=NULL;
	}
	if (m_pObjRef)
	{
		delete [] m_pObjRef;
	    m_pObjRef=NULL;
}*/
 
}

BOOL KRegion::Init(int nWidth, int nHeight)
{
	m_nWidth	= nWidth;	//16
	m_nHeight	= nHeight;	//32

	m_cNpcRef.Clear();
	m_cObjRef.Clear();
	
	return TRUE;
}

BOOL KRegion::Load(int nX, int nY)
{

	Close();
	m_RegionID = MAKELONG(nX, nY);

	m_nRegionX = nX * 512;
	m_nRegionY = nY * 1024;

	// �·�
	m_nConRegionID[0] = MAKELONG(nX, nY + 1);
	// ���·�
	m_nConRegionID[1] = MAKELONG(nX - 1, nY + 1);
	// ��
	m_nConRegionID[2] = MAKELONG(nX - 1, nY);
	// ���Ϸ�
	m_nConRegionID[3] = MAKELONG(nX - 1, nY - 1);
	// �Ϸ�
	m_nConRegionID[4] = MAKELONG(nX, nY - 1);
	// ���Ϸ�
	m_nConRegionID[5] = MAKELONG(nX + 1, nY - 1);
	// �ҷ�
	m_nConRegionID[6] = MAKELONG(nX + 1, nY);
	// ���·�
	m_nConRegionID[7] = MAKELONG(nX + 1, nY + 1);

	return TRUE;
}
//----------------------------------------------------------------------
//	���ܣ�����ͻ��˵�ͼ�ϱ�region �� object���ݣ�����npc��box�ȣ�
//	��� bLoadNpcFlag == TRUE ��Ҫ���� clientonly npc else ������
//----------------------------------------------------------------------
BOOL KRegion::LoadObjectC(int nSubWorld, int nX, int nY, char *lpszPath)
{
	char szPath[FILE_NAME_LENGTH], szFile[FILE_NAME_LENGTH];

	if (!lpszPath || !lpszPath[0] || strlen(lpszPath) >= FILE_NAME_LENGTH)
		return FALSE;

	t_sprintf(szPath, "\\%s\\v_%03d", lpszPath, nY);
//	g_SetFilePath(szPath);
	// ����npc������λ�ڱ��ص� client npc
	NpcSet.InsertNpcToRegion(this->m_nIndex);

	KPakFile	cData;
	t_sprintf(szFile, "%s\\%03d_%s", szPath, nX, REGION_COMBIN_FILE_NAME_CLIENT);
	if (cData.Open(szFile))
	{//���Ұ��е� ��ͼ�ļ�
		DWORD	dwHeadSize;
		DWORD	dwMaxElemFile = 0;
		KCombinFileSection	sElemFile[REGION_ELEM_FILE_COUNT];

		if (cData.Size() < sizeof(DWORD) + sizeof(KCombinFileSection) * REGION_ELEM_FILE_COUNT)
			goto gotoCLOSE;
		cData.Read(&dwMaxElemFile, sizeof(DWORD));
		if (dwMaxElemFile > REGION_ELEM_FILE_COUNT)
		{
			cData.Read(sElemFile, sizeof(sElemFile));
			cData.Seek(sizeof(KCombinFileSection) * (dwMaxElemFile - REGION_ELEM_FILE_COUNT), FILE_CURRENT);
		}
		else
		{
			cData.Read(sElemFile, sizeof(sElemFile));
		}
		dwHeadSize = sizeof(DWORD) + sizeof(KCombinFileSection) * dwMaxElemFile;

		// ����npc����
		 cData.Seek(dwHeadSize + sElemFile[REGION_NPC_FILE_INDEX].uOffset, FILE_BEGIN);
		LoadClientNpc(&cData, sElemFile[REGION_NPC_FILE_INDEX].uLength);

		// ����obj����
		cData.Seek(dwHeadSize + sElemFile[REGION_OBJ_FILE_INDEX].uOffset, FILE_BEGIN);
		LoadClientObj(&cData, sElemFile[REGION_OBJ_FILE_INDEX].uLength);
		

gotoCLOSE:
		cData.Close();
	}
	else
	{
		// ���� Client npc ����
		/*KPakFile		cNpcData;
		KNpcFileHead	sNpcFileHead;
		KSPNpc			sNpcCell;
		DWORD			i;
		KClientNpcID	sTempID;
		int				nNpcNo;

		// �����ͼ�ļ���� client npc
//		g_SetFilePath(szPath);
		t_sprintf(szFile, "%s\\%03d_%s", szPath, nX, REGION_NPC_FILE_CLIENT);
		if (!cNpcData.Open(szFile))
			return false;//goto NPC_CLOSE;
		if (cNpcData.Size() < sizeof(KNpcFileHead))
			goto NPC_CLOSE;
		cNpcData.Read(&sNpcFileHead, sizeof(KNpcFileHead));
		for (i = 0; i < sNpcFileHead.uNumNpc; ++i)
		{
			cNpcData.Read(&sNpcCell, sizeof(KSPNpc) - sizeof(sNpcCell.szScript)-sizeof(sNpcCell.nNDropFile)-sizeof(sNpcCell.nGDropFile));
			//CCAssert(sNpcCell.nScriptNameLen < sizeof(sNpcCell.szScript),"");
			//if (sNpcCell.nScriptNameLen>=sizeof(sNpcCell.szScript))

			if (sNpcCell.nScriptNameLen > 0 && sNpcCell.nScriptNameLen < sizeof(sNpcCell.szScript))
			{
				cNpcData.Read(sNpcCell.szScript, sNpcCell.nScriptNameLen);
				sNpcCell.szScript[sNpcCell.nScriptNameLen] = 0;
			}
			else
			{
				sNpcCell.szScript[0] = 0;
			}
			sTempID.m_dwRegionID = MAKELONG(nX, nY);
			sTempID.m_nNo = i;
			nNpcNo = NpcSet.SearchClientID(sTempID);
			if (nNpcNo == 0)
			{
				int nIdx = NpcSet.AddClientNpc(sNpcCell.nTemplateID, nX, nY, sNpcCell.nPositionX, sNpcCell.nPositionY, i);
				if (nIdx > 0)
				{
					if (sNpcCell.shKind<0)
						sNpcCell.shKind = 3;

					Npc[nIdx].m_Kind = sNpcCell.shKind;
					Npc[nIdx].SendSerCommand(do_stand);
					Npc[nIdx].m_Dir = Npc[nIdx].GetNormalNpcStandDir(sNpcCell.nCurFrame);
				}
			} 
		}

NPC_CLOSE:
		cNpcData.Close(); 
		*/
		// ����ClientObject����
		ObjSet.ClientLoadRegionObj(szPath, nX, nY, nSubWorld, this->m_nIndex);
	}

	return TRUE;
}

//----------------------------------------------------------------------
//	���ܣ�����ͻ��˵�ͼ�ϱ� region �� clientonlynpc ����
//----------------------------------------------------------------------
BOOL	KRegion::LoadClientNpc(KPakFile *pFile, DWORD dwDataSize)
{
	return FALSE;

	if (!pFile || dwDataSize < sizeof(KNpcFileHead))
		return FALSE;

	KNpcFileHead	sNpcFileHead;
	KSPNpc			sNpcCell;
	DWORD			i;
	KClientNpcID	sTempID;
	int				nNpcNo;

	pFile->Read(&sNpcFileHead, sizeof(KNpcFileHead));

	for (i = 0; i < sNpcFileHead.uNumNpc; ++i)
	{
		pFile->Read(&sNpcCell, sizeof(KSPNpc) - sizeof(sNpcCell.szScript)-sizeof(sNpcCell.nNDropFile)-sizeof(sNpcCell.nGDropFile));
		if (sNpcCell.nScriptNameLen < sizeof(sNpcCell.szScript))
		{
			pFile->Read(sNpcCell.szScript, sNpcCell.nScriptNameLen);
			sNpcCell.szScript[sNpcCell.nScriptNameLen] = 0;
		}
		else
		{
			sNpcCell.szScript[0] = 0;
			pFile->Seek(sNpcCell.nScriptNameLen, FILE_CURRENT);
		}
		sTempID.m_dwRegionID = m_RegionID;
		sTempID.m_nNo = i;
		nNpcNo = NpcSet.SearchClientID(sTempID);
		if (nNpcNo == 0)
		{
			int nIdx = NpcSet.AddClientNpc(sNpcCell.nTemplateID, LOWORD(m_RegionID), HIWORD(m_RegionID), sNpcCell.nPositionX, sNpcCell.nPositionY, i);
			if (nIdx > 0)
			{
				if (sNpcCell.shKind<0)
					sNpcCell.shKind = 3;

				Npc[nIdx].m_Kind = sNpcCell.shKind;
				Npc[nIdx].SendSerCommand(do_stand);
				Npc[nIdx].m_Dir = Npc[nIdx].GetNormalNpcStandDir(sNpcCell.nCurFrame);
			}
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------
//	���ܣ�����ͻ��˵�ͼ�ϱ� region �� clientonlyobj ����
//----------------------------------------------------------------------
BOOL	KRegion::LoadClientObj(KPakFile *pFile, DWORD dwDataSize)
{
	return ObjSet.ClientAddRegionObj(pFile, dwDataSize);
}


//----------------------------------------------------------------------
//	���ܣ������ϰ����ݸ�С��ͼ	(��ȡ���С������ϰ�����Ϣ)
//----------------------------------------------------------------------
void	KRegion::LoadLittleMapData(int nX, int nY, char *lpszPath, INT *lpbtObstacle)
{
	if (!lpbtObstacle)
		return;

	char  szPath[FILE_NAME_LENGTH], szFile[FILE_NAME_LENGTH];
	int	  i, j;

	if (!lpszPath || !lpszPath[0] || strlen(lpszPath) >= FILE_NAME_LENGTH)
		return ;
	t_sprintf(szPath, "%s\\v_%03d", lpszPath, nY);

	KPakFile	cData;
	INT		    nTempTable[REGION_GRID_WIDTH][REGION_GRID_HEIGHT];	// ��ͼ�ϰ���Ϣ��
	ZeroStruct(nTempTable);
//	g_SetFilePath(szPath);
	t_sprintf(szFile, "%s\\%03d_%s", szPath, nX, REGION_COMBIN_FILE_NAME_CLIENT);
//	GetFliePath("����·��:",szFile,1000);

	if (cData.Open(szFile))
	{
		DWORD	dwHeadSize;
		DWORD	dwMaxElemFile = 0;
		KCombinFileSection	sElemFile[REGION_ELEM_FILE_COUNT];

		if (cData.Size() < sizeof(DWORD) + sizeof(KCombinFileSection) * REGION_ELEM_FILE_COUNT)
		{
			ZeroMemory(nTempTable, sizeof(nTempTable));
		}
		else
		{
			cData.Read(&dwMaxElemFile, sizeof(DWORD));
			if (dwMaxElemFile > REGION_ELEM_FILE_COUNT)
			{
				cData.Read(sElemFile, sizeof(sElemFile));
				cData.Seek(sizeof(KCombinFileSection) * (dwMaxElemFile - REGION_ELEM_FILE_COUNT), FILE_CURRENT);
			}
			else
			{
				cData.Read(sElemFile, sizeof(sElemFile));
			}

			dwHeadSize = sizeof(DWORD) + sizeof(KCombinFileSection) * dwMaxElemFile;

			if (sElemFile[REGION_OBSTACLE_FILE_INDEX].uLength >= sizeof(nTempTable))
			{//����ȫ�ϰ� �� �����ϰ�
				cData.Seek(dwHeadSize + sElemFile[REGION_OBSTACLE_FILE_INDEX].uOffset, FILE_BEGIN);
				cData.Read((LPVOID)nTempTable, sizeof(nTempTable));
					for (i = 0; i < REGION_GRID_HEIGHT; ++i)
					{    
		              for (j = 0; j < REGION_GRID_WIDTH; ++j)
					  {    
		                 if ((BYTE)nTempTable[j][i]) 
		              	   lpbtObstacle[i * REGION_GRID_WIDTH + j] = (BYTE)nTempTable[j][i];
		                 else
			               lpbtObstacle[i * REGION_GRID_WIDTH + j] = 0;
						/*char nMsg[64]={0};
						t_sprintf(nMsg,"���:%d,ֵ:%d",i * REGION_GRID_WIDTH + j,(BYTE)nTempTable[j][i]);
						GetFliePath(nMsg,"��",2000); */
					  }   
					}    
			}
			else
			{//Ϊ ��������Ϊ ���ϰ�  -2��
				//ZeroMemory(nTempTable, sizeof(nTempTable));
				for (i = 0; i < REGION_GRID_HEIGHT; ++i)
				{   
		            for (j = 0; j < REGION_GRID_WIDTH; ++j)
					{   
			            lpbtObstacle[i * REGION_GRID_WIDTH + j] = -2;
					}  
				}  
			}
		}
		cData.Close();
	}
	else
	{//��ͼ��Ե û�������Ϊ -1��
		/*t_sprintf(szFile, "%03d_%s", nX, REGION_OBSTACLE_FILE);
		if (cData.Open(szFile))
			cData.Read((LPVOID)nTempTable, sizeof(nTempTable));
		else
			ZeroMemory(nTempTable, sizeof(nTempTable));

		cData.Close(); */
	  for (i = 0; i < REGION_GRID_HEIGHT; ++i)
	  {  
		for (j = 0; j < REGION_GRID_WIDTH; ++j)
		{
			  lpbtObstacle[i * REGION_GRID_WIDTH + j] = -1;
		}
	  }  
	}	
}

void KRegion::NewRegActivate()
{
	KIndexNode* pNode	 = NULL;
	KIndexNode* pTmpNode = NULL;
	uint32_t nCounter = 0;

	if (IsActiveAll())	// ֻ���ڸ��������ʱ��Npc�Ŵ��ڻ״̬��
	{
		pNode = (KIndexNode*)m_NpcList.GetHead();
		while (pNode)
		{
			pTmpNode = (KIndexNode*)pNode->GetNext();
			int nNpcIdx = pNode->m_nIndex;
			if (nNpcIdx>0 || nNpcIdx<MAX_NPC)
			{
			   Npc[nNpcIdx].Activate();
			   /*if(g_GameWorld) 
			   {//��� ���ӽڵ�
				   Npc[nNpcIdx].Paint();
			   }*/
			}
			pNode = pTmpNode;
		}
		m_nNpcSyncCounter++;
		if (m_nNpcSyncCounter > m_NpcList.GetNodeCount() << 1)
			m_nNpcSyncCounter = 0;
	}
	/*nCounter = 0;
	INT nObjIdx = 0;
	// ���ڼ�ʱɾ����Ҫ��obj����Active��������Ϊ���ԭ���ж�
	VEC_LIST::const_iterator it = m_ObjList.begin();
	for(; it != m_ObjList.end(); ++it)
	{
		nObjIdx = *it;
		Object[nObjIdx].Activate();

#ifdef _SERVER
		if ((nCounter >> 1) == m_nObjSyncCounter)
			Object[nObjIdx].SyncState();
		nCounter ++;
#endif // #ifdef _SERVER
	}

	m_nObjSyncCounter++;
	if (m_nObjSyncCounter > (INT)m_ObjList.size() <<1)
		m_nObjSyncCounter = 0;*/
	nCounter = 0;
	KIndexNode *pObjNode = NULL;
	KIndexNode *pObjTmpNode = NULL;
	pObjNode = (KIndexNode *)m_ObjList.GetHead();

	while(pObjNode)
	{
		pObjTmpNode = (KIndexNode *)pObjNode->GetNext();
		if (pObjNode->m_nIndex>0)
		{
			try
			{
				Object[pObjNode->m_nIndex].Activate();  //��Ʒ�����ѭ������ ���ϵ�
			}
			catch (...)
			{
				//printf("--��Ʒ��������ɾ��:����:%u --\n",pObjNode->m_nIndex);
				if (Object[pObjNode->m_nIndex].m_nSubWorldID>=0 && Object[pObjNode->m_nIndex].m_nRegionIdx >= 0)
					SubWorld[Object[pObjNode->m_nIndex].m_nSubWorldID].m_Region[Object[pObjNode->m_nIndex].m_nRegionIdx].RemoveObj(pObjNode->m_nIndex);

				ObjSet.Remove(pObjNode->m_nIndex);
			}

		}
		pObjNode = pObjTmpNode;
	}

	++m_nObjSyncCounter;

	if (m_nObjSyncCounter > m_ObjList.GetNodeCount()<<1)
	{
		m_nObjSyncCounter = 0;
	}
	//�ӵ���ѭ��
	pNode = (KIndexNode *)m_MissleList.GetHead();
	while(pNode)
	{
		pTmpNode = (KIndexNode *)pNode->GetNext();
		Missle[pNode->m_nIndex].Activate();		
		pNode = pTmpNode;
	}
	
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex == m_nIndex)	// ��Player���ڵ�Region
		Player[CLIENT_PLAYER_INDEX].Active();
}

//�����ѭ��
void KRegion::Activate(int mSubWorldID)
{
	KIndexNode *pNode = NULL;
	KIndexNode *pTmpNode = NULL;
	KIndexNode *pPlayerNode = NULL;
	KIndexNode *pPlayerTmpNode = NULL;
	int	nCounter = 0;
//------Npc
	pNode = (KIndexNode *)m_NpcList.GetHead();

	while(pNode)
	{
		pTmpNode = (KIndexNode *)pNode->GetNext();
		int nNpcIdx = pNode->m_nIndex;
	    if (nNpcIdx>0)
		{ 
		   if (IsActive() || Npc[nNpcIdx].m_GameliveTime >0)
			  Npc[nNpcIdx].Activate();                         //NPC״̬�� AI,��۵�--����ʾ���� ����ʾ core mainloop ����
     
/*#ifndef _SERVER
		  if (Npc[nNpcIdx].IsPlayer())
		     Npc[nNpcIdx].ActivateWaiGua();	                //ѭ��������
#endif*/
		}
		pNode = pTmpNode;
	}

	++ m_nNpcSyncCounter;

	if (m_nNpcSyncCounter > m_NpcList.GetNodeCount()<<1)
	{
		m_nNpcSyncCounter = 0;
	}

//nCounter = 0;
//----------��Ʒ
	/*pNode = (KIndexNode *)m_ObjList.GetHead();

	while(pNode)
	{
		pTmpNode = (KIndexNode *)pNode->GetNext();
#ifdef _SERVER
		if ((nCounter == m_nObjSyncCounter / 2) && (m_nObjSyncCounter & 1))
		{
			Object[pNode->m_nIndex].SyncState();
		}
		nCounter++;
#endif
		Object[pNode->m_nIndex].Activate();  //��Ʒ�����ѭ������ ���ϵ�
		pNode = pTmpNode;
	}
	m_nObjSyncCounter++;

	if (m_nObjSyncCounter > m_ObjList.GetNodeCount() * 2)
	{
		m_nObjSyncCounter = 0;
	}
//----�ӵ�
	pNode = (KIndexNode *)m_MissleList.GetHead();
	while(pNode)
	{
		pTmpNode = (KIndexNode *)pNode->GetNext();
		Missle[pNode->m_nIndex].Activate();		 //�ӵ������
		pNode = pTmpNode;
	}
 */
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_RegionIndex == m_nIndex)	// ��Player���ڵ�Region
	{//�ͻ�����ҵ�ѭ��
		Player[CLIENT_PLAYER_INDEX].Active();
	}
}

void KRegion::ActivateObj(int mSubWorldID)
{

	KIndexNode *pNode = NULL;
	KIndexNode *pTmpNode = NULL;

	int	nCounter = 0;
	//----�ӵ�ѭ��
	pNode = (KIndexNode *)m_MissleList.GetHead();
	while(pNode)
	{
		pTmpNode = (KIndexNode *)pNode->GetNext();

		if (pNode->m_nIndex>0 && pNode->m_nIndex < MAX_MISSLE)
		{
			try
			{
				Missle[pNode->m_nIndex].Activate();		 //�ӵ������
			}
			catch (...)
			{
				//printf("--�ӵ���������ɾ��:����:%u --\n",pNode->m_nIndex);
				if (Missle[pNode->m_nIndex].m_nSubWorldId>=0 && Missle[pNode->m_nIndex].m_nRegionId >= 0)
					SubWorld[Missle[pNode->m_nIndex].m_nSubWorldId].m_Region[Missle[pNode->m_nIndex].m_nRegionId].RemoveMissle(pNode->m_nIndex);

				MissleSet.Remove(pNode->m_nIndex);
			}
		}
		pNode = pTmpNode;
	}
//----��Ʒ
	nCounter = 0;
	/*INT nObjIdx = 0;
	// ���ڼ�ʱɾ����Ҫ��obj����Active��������Ϊ���ԭ���ж�
	VEC_LIST::const_iterator it = m_ObjList.begin();
	for(; it != m_ObjList.end(); ++it)
	{
		nObjIdx = *it;
		Object[nObjIdx].Activate();
	}

	m_nObjSyncCounter++;
	if (m_nObjSyncCounter > (INT)m_ObjList.size() * 2)
		m_nObjSyncCounter = 0;*/

	KIndexNode *pObjNode = NULL;
	KIndexNode *pObjTmpNode = NULL;

   	pObjNode = (KIndexNode *)m_ObjList.GetHead();

	while(pObjNode)
	{
	  pObjTmpNode = (KIndexNode *)pObjNode->GetNext();
		if (pObjNode->m_nIndex>0)
		{
		  try
		  {
			 Object[pObjNode->m_nIndex].Activate();  //��Ʒ�����ѭ������ ���ϵ�
		  }
		  catch (...)
		  {
			 //printf("--��Ʒ��������ɾ��:����:%u --\n",pObjNode->m_nIndex);
			  if (Object[pObjNode->m_nIndex].m_nSubWorldID>=0 && Object[pObjNode->m_nIndex].m_nRegionIdx >= 0)
				  SubWorld[Object[pObjNode->m_nIndex].m_nSubWorldID].m_Region[Object[pObjNode->m_nIndex].m_nRegionIdx].RemoveObj(pObjNode->m_nIndex);
			  
		        ObjSet.Remove(pObjNode->m_nIndex);
		  }
		}
		pObjNode = pObjTmpNode;
	}

	++m_nObjSyncCounter;

	if (m_nObjSyncCounter > m_ObjList.GetNodeCount()<<1)
	{
		m_nObjSyncCounter = 0;
	}
}


//��ͼ��������NPC�ڵ�
void KRegion::AddNpc(int nIdx)
{
	if (nIdx > 0 && nIdx < MAX_NPC)
	{
		//CCAssert(Npc[nIdx].m_Node.m_Ref == 0,"");
		if (Npc[nIdx].m_Node.m_Ref <= 0)
		{
			Npc[nIdx].m_Node.m_Ref = 0;
			m_NpcList.AddTail(&Npc[nIdx].m_Node);
			Npc[nIdx].m_Node.AddRef();
		}
	}
}

void KRegion::RemoveNpc(int nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_NPC)
		return;

	//CCAssert(Npc[nIdx].m_Node.m_Ref > 0,"");

	if (Npc[nIdx].m_Node.m_Ref > 0)
	{
		Npc[nIdx].m_Node.Remove();
		Npc[nIdx].m_Node.Release();
		Npc[nIdx].m_Node.m_Ref = 0;
	}
	//ɾ�����
	Npc[nIdx].RemoveRes();
}

/*void KRegion::RemoveNpcwor(int nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_NPC)
		return;
	
	//_ASSERT(Npc[nIdx].m_Node.m_Ref > 0);
	
	if (Npc[nIdx].m_Node.m_Ref > 0)
	{
		Npc[nIdx].m_Node.Remove();
		Npc[nIdx].m_Node.Release();
	}
    else
      return;
}*/

void KRegion::AddMissle(int nId)
{
	if (nId > 0 && nId < MAX_MISSLE)  //�ӵ�����
	{
		//CCAssert(Missle[nId].m_Node.m_Ref == 0,"");

		if (Missle[nId].m_Node.m_Ref <= 0)
		{
			Missle[nId].m_Node.m_Ref = 0;
			m_MissleList.AddTail(&Missle[nId].m_Node);
			Missle[nId].m_Node.AddRef();
		}
	}
}

void KRegion::RemoveMissle(int nId)
{
	if (nId > 0 && nId < MAX_MISSLE)
	{
//		_ASSERT(Missle[nId].m_Node.m_Ref > 0);
		if (Missle[nId].m_Node.m_Ref > 0)
		{
			Missle[nId].m_Node.Remove();
			Missle[nId].m_Node.Release();
			//Missle[nId].m_Node.m_Ref =0;
		}
	}
}

void KRegion::AddObj(int nIdx)
{

	if (nIdx <= 0 || nIdx >= MAX_OBJECT)
		return;

	KIndexNode *pNode = NULL;
	
	pNode = new KIndexNode;
	pNode->m_nIndex = nIdx;

	m_ObjList.AddTail(pNode);
	//m_ObjList.push_back(nIdx);	
	if (Object[nIdx].m_nMapX >= 0 && Object[nIdx].m_nMapY >= 0)
	{
	  AddObjRef(Object[nIdx].m_nMapX,Object[nIdx].m_nMapY);
	} 
	

	/*if (nIdx > 0 && nIdx < MAX_OBJECT)
	{
		if (Object[nIdx].m_Node.m_Ref <= 0)
		{
			m_ObjList.AddTail(&Object[nIdx].m_Node);
			Object[nIdx].m_Node.m_Ref =0;
			Object[nIdx].m_Node.AddRef();
		}

		if (Object[nIdx].m_nMapX >= 0 && Object[nIdx].m_nMapY >= 0)
		{
			AddObjRef(Object[nIdx].m_nMapX,Object[nIdx].m_nMapY);
		} 
	}*/
}

void KRegion::RemoveObj(int nIdx)
{

	if (nIdx <= 0 || nIdx >= MAX_OBJECT)
		return;

	KIndexNode *pNode = NULL;
	
	pNode = (KIndexNode *)m_ObjList.GetHead();
	
	while(pNode)
	{
		if (pNode->m_nIndex == nIdx)
		{
			pNode->Remove();
			delete pNode;
			pNode=NULL;
			break;
		}
		pNode = (KIndexNode *)pNode->GetNext();
	}

	if (Object[nIdx].m_nMapX > 0 && Object[nIdx].m_nMapY > 0)
	{
	  DecObjRef(Object[nIdx].m_nMapX, Object[nIdx].m_nMapY);
	}
	
	/*if (nIdx > 0 && nIdx < MAX_OBJECT)
	{
		if (Object[nIdx].m_Node.m_Ref > 0)
		{
			Object[nIdx].m_Node.Remove();
			Object[nIdx].m_Node.m_Ref =0;
			Object[nIdx].m_Node.Release();
		}

		if (Object[nIdx].m_nMapX > 0 && Object[nIdx].m_nMapY > 0)
		{
			DecObjRef(Object[nIdx].m_nMapX, Object[nIdx].m_nMapY);
		}
	}*/
}


BOOL KRegion::SetTrap(int nMapX, int nMapY,int nCellNum,DWORD uTrapScriptId)
{
	    return FALSE;
}

DWORD KRegion::GetTrap(int nMapX, int nMapY)
{
	return 0;
}
//��ȡ�ϰ�������(��������)
BYTE KRegion::GetBarrier(int nMapX, int nMapY, int nDx, int nDy)
{
	if (nMapX < 0  || nMapX >= REGION_GRID_WIDTH || nMapY< 0 ||  nMapY >= REGION_GRID_HEIGHT)
		return	Obstacle_NULL;

	/*if (m_pNpcRef)
	{
		if (m_pNpcRef[nMapY * m_nWidth + nMapX] > 0)
			return Obstacle_JumpFly;
	}*/
	return Obstacle_NULL;
}



//��ȡ�ϰ�������(��������)
BYTE KRegion::GetNewBarrier(int nMapX, int nMapY, int nDx, int nDy,BOOL nIsCheckNpc)
{
    if ( nMapX<0 || nMapX >=REGION_GRID_WIDTH ||  nMapY<0 || nMapY >=REGION_GRID_HEIGHT)
	{
	   return Obstacle_NULL;
	}

	if (nIsCheckNpc)
	{
		//if (m_pNpcRef[nMapY * m_nWidth + nMapX] > 0)
		if (GetNpcRef(nMapX,nMapY))
			return Obstacle_JumpFly;
	}
	return Obstacle_NULL;
}



//----------------------------------------------------------------------------
//	���ܣ��� ���ص����� * 1024 �ľ����ж�ĳ��λ���Ƿ��ϰ�
//	������nGridX nGirdY ����region��������
//	������nOffX nOffY �������ڵ�ƫ����(���ص� * 1024 ����)
//	������bCheckNpc ���Ƿ��ж�npc�γɵ��ϰ�
//	����ֵ���ϰ�����(if ���� == Obstacle_NULL ���ϰ�)
//----------------------------------------------------------------------------
BYTE	KRegion::GetBarrierNewMin(int nGridX, int nGridY, int nOffX, int nOffY, BOOL bCheckNpc)
{
	//_ASSERT(0 <= nGridX && nGridX < REGION_GRID_WIDTH && 0 <= nGridY && nGridY < REGION_GRID_HEIGHT);
	if ( nGridX<0 || nGridX >=REGION_GRID_WIDTH ||  nGridY<0 || nGridY >=REGION_GRID_HEIGHT)
	{
	   return Obstacle_NULL;
	}

	if (bCheckNpc)
	{
		//if (m_pNpcRef[nGridY * m_nWidth + nGridX] > 0)
		if (GetNpcRef(nGridX,nGridY))
			return Obstacle_JumpFly;
	}
	return Obstacle_NULL;
}






//----------------------------------------------------------------------------
//	���ܣ��� ���ص����� * 1024 �ľ����ж�ĳ��λ���Ƿ��ϰ�
//	������nGridX nGirdY ����region��������
//	������nOffX nOffY �������ڵ�ƫ����(���ص� * 1024 ����)
//	������bCheckNpc ���Ƿ��ж�npc�γɵ��ϰ�
//	����ֵ���ϰ�����(if ���� == Obstacle_NULL ���ϰ�)
//----------------------------------------------------------------------------
BYTE	KRegion::GetBarrierMin(int nGridX, int nGridY, int nOffX, int nOffY, BOOL bCheckNpc)
{
	//_ASSERT(0 <= nGridX && nGridX < REGION_GRID_WIDTH && 0 <= nGridY && nGridY < REGION_GRID_HEIGHT);
	if ( nGridX<0 || nGridX >=REGION_GRID_WIDTH ||  nGridY<0 || nGridY >=REGION_GRID_HEIGHT)
	{
		return Obstacle_NULL;
	}

	if (bCheckNpc)
	{
		//if (m_pNpcRef[nGridY * m_nWidth + nGridX] > 0)
		if (GetNpcRef(nGridX,nGridY))
			return Obstacle_JumpFly;
	}
	return Obstacle_NULL;
}
/*
int KRegion::GetRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType)
{
	int nRet = 0;
	if (nMapX >= m_nWidth || nMapY >= m_nHeight)
		return 0;

	switch(nType)
	{
	case obj_npc:	 //NPC
		nRet = m_pNpcRef[nMapY * m_nWidth + nMapX];
		break;
	case obj_object: //��Ʒ
		nRet = m_pObjRef[nMapY * m_nWidth + nMapX];
		break;
	case obj_missle: //�ӵ�
		nRet = m_pMslRef[nMapY * m_nWidth + nMapX];
		break;
	default:
		break;
	} 
	return nRet;
}

BOOL KRegion::AddRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType)
{
	BYTE* pBuffer = NULL;
	int nRef = 0;

	if (nMapX >= m_nWidth || nMapY >= m_nHeight)
		return FALSE;
	
	switch(nType)
	{
	case obj_npc:
		pBuffer = m_pNpcRef;
		break;
	case obj_object:
		pBuffer = m_pObjRef;
		break;
	case obj_missle:
		pBuffer = m_pMslRef;
		break;
	default:
		break;
	}
	if (pBuffer)
	{
		nRef = (int)pBuffer[nMapY * m_nWidth + nMapX];
		if (nRef >= m_nWidth * m_nWidth)
			return FALSE;
		else
		{
			pBuffer[nMapY * m_nWidth + nMapX]++;  //��һ����Ʒ
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
}

BOOL KRegion::DecRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType)
{
	BYTE* pBuffer = NULL;
	int nRef = 0;
	
	if (nMapX >= m_nWidth || nMapY >= m_nHeight)
		return FALSE;
	
	switch(nType)
	{
	case obj_npc:
		pBuffer = m_pNpcRef;
		break;
	case obj_object:
		pBuffer = m_pObjRef;
		break;
	case obj_missle:
		pBuffer = m_pMslRef;
		break;
	default:
		break;
	}

	if (pBuffer)
	{
		nRef = (int)pBuffer[nMapY * m_nWidth + nMapX];
		if (nRef == 0)
		{
//			_ASSERT(0);
			return FALSE;
		}
		else
		{
			pBuffer[nMapY * m_nWidth + nMapX]--; //����һ����������
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
}
*/
BOOL KRegion::AddPlayer(int nIdx)
{
	if (nIdx > 0 && nIdx < MAX_PLAYER)
	{
		//_ASSERT(Player[nIdx].m_Node.m_Ref == 0);
		if (Player[nIdx].m_Node.m_Ref <= 0)
		{
			m_PlayerList.AddTail(&Player[nIdx].m_Node);  //����б�����һ����ҽڵ�λ��
			Player[nIdx].m_Node.AddRef();                // ��������
			
			return TRUE;
		}
	}
	return FALSE;
}

BOOL KRegion::RemovePlayer(int nIdx)
{
	if (nIdx > 0 && nIdx < MAX_PLAYER)
	{
		if (Player[nIdx].m_Node.m_Ref > 0)
		{
			Player[nIdx].m_Node.Remove();
			Player[nIdx].m_Node.Release();


			return TRUE;
		}
	}
	return FALSE;
}

//-------------------------------------------------------------------------
//	���ܣ�Ѱ�ұ��������Ƿ���ĳ��ָ�� id �� npc
//-------------------------------------------------------------------------
int		KRegion::SearchNpc(DWORD dwNpcID)
{
	KIndexNode *pNode = NULL;

	pNode = (KIndexNode *)m_NpcList.GetHead();
	while(pNode)
	{
		if (Npc[pNode->m_nIndex].m_dwID == dwNpcID)
			return pNode->m_nIndex;
		pNode = (KIndexNode *)pNode->GetNext();
	}	

	return 0;
}

int KRegion::FindObject(int nObjID)
{
	KIndexNode *pNode = NULL;
	
	pNode = (KIndexNode *)m_ObjList.GetHead();
	
	while(pNode)
	{
		if (Object[pNode->m_nIndex].m_nID == nObjID)
		{
			return pNode->m_nIndex;
		}
		pNode = (KIndexNode *)pNode->GetNext();
	}	
	/*VEC_LIST::iterator it = m_ObjList.begin();
	for(; it != m_ObjList.end(); ++it)
	{
		if (Object[*it].m_nID == nObjID)
		{
			return *it;
		}
	}*/
	return 0;
}

/*int KRegion::FindEquip(int nMapX, int nMapY)
{
	KIndexNode *pNode = NULL;
	
	pNode = (KIndexNode *)m_ObjList.GetHead();
	
	while(pNode)
	{
		if (Object[pNode->m_nIndex].m_nMapX == nMapX && Object[pNode->m_nIndex].m_nMapY == nMapY)
		{
			if (Object[pNode->m_nIndex].m_nKind == Obj_Kind_Item)
			{
				return pNode->m_nIndex;
			}
		}
		pNode = (KIndexNode *)pNode->GetNext();
	}	
	return 0;
}*/

//�ͻ��˻��� NPC �ӵ� ��Ʒ
void   KRegion::GetFliePath(char* nName,char * nPath,int nMaxRows)
{
	KFile nFile;
	KTabFileCtrl nScirptFile;
	char nTongApplyPath[125]={0},szCol[128]={0};
	t_sprintf(nTongApplyPath,"%s","\\regjxpath.log");
	if (!g_FileExists(nTongApplyPath))	//�Ƿ����
	{
		nFile.Create(nTongApplyPath);
		//t_sprintf(szCol,"��ͼ\t����\tԴ�ű�\15\n");
		t_sprintf(szCol,"����\15\n");
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
		
		t_sprintf(szCol,"--��ͼ%s:(%s)--",nName,nPath);
		nScirptFile.InsertAfter(nRows);
		nScirptFile.WriteString(nRows,1, szCol);                                    //�к�
		//nTong.WriteString(nRows,2,Npc[Player[m_nPlayerIndex].m_nIndex].Name);     //����
		//nTong.WriteInteger(nRows,3,Npc[Player[m_nPlayerIndex].m_nIndex].m_Level); //�ȼ�
		nScirptFile.Save(nTongApplyPath);	
		//nFile.Write(szCol, sizeof(szCol));
	}
	nScirptFile.Clear();
}

void KRegion::Paint()
{
	KIndexNode *pNode = NULL;

	pNode = (KIndexNode *)m_NpcList.GetHead();
	while(pNode)
	{
		Npc[pNode->m_nIndex].Paint();     //�滭NPC ���
		pNode = (KIndexNode *)pNode->GetNext();
	}
/*
	pNode = (KIndexNode *)m_MissleList.GetHead();
	while(pNode)
	{
		Missle[pNode->m_nIndex].Paint();  //�滭 �ӵ�
		pNode = (KIndexNode *)pNode->GetNext();
	}

	pNode = (KIndexNode *)m_ObjList.GetHead();
	while(pNode)
	{
//		Object[pNode->m_nIndex].Paint();		need add -spe
		pNode = (KIndexNode *)pNode->GetNext();
	}*/
}

//KRegion::VEC_LIST::iterator KRegion::GetObjNode(int nIdx)
void* KRegion::GetObjNode(int nIdx)
{
	KIndexNode *pNode = NULL;

	pNode = (KIndexNode *)m_ObjList.GetHead();

	while(pNode)
	{
		if (pNode->m_nIndex == nIdx)
		{
			break;
		}
		pNode = (KIndexNode *)pNode->GetNext();
	}
	return pNode;

	/*VEC_LIST::iterator it = m_ObjList.begin();
	for(; it != m_ObjList.end(); ++it)
	{
		if (*it == nIdx)
			break;
	}
	return it;*/
}

/*void* KRegion::GetObjNode(int nIdx)
{
	KIndexNode *pNode = NULL;

	pNode = (KIndexNode *)m_ObjList.GetHead();

	while(pNode)
	{
		if (pNode->m_nIndex == nIdx)
		{
			break;
		}
		pNode = (KIndexNode *)pNode->GetNext();
	}
	return pNode;
}*/

void KRegion::Close()		// ���Region�еļ���������ָ�������û�б������
{
	KIndexNode* pNode = NULL;
	KIndexNode* pTempNode = NULL;

	if (!m_nWidth || !m_nHeight)
		return;

	m_cNpcRef.Clear();
	m_cObjRef.Clear();

	pNode = (KIndexNode *)m_NpcList.GetHead();

	while(pNode)
	{
		pTempNode = pNode;
		pNode = (KIndexNode *)pNode->GetNext();
	    if  (pTempNode->m_nIndex>0)
		{
			RemoveNpc(pTempNode->m_nIndex);		
			Npc[pTempNode->m_nIndex].m_RegionIndex = -1;
		}
	}

	KIndexNode* pMissNode = NULL;
	KIndexNode* pMissTempNode = NULL;

	pMissNode = (KIndexNode *)m_MissleList.GetHead();
	while(pMissNode)
	{
		pMissTempNode = pMissNode;
		pMissNode = (KIndexNode *)pMissNode->GetNext();
		if (pMissTempNode->m_nIndex>0)
		{
			MissleSet.Remove(pMissTempNode->m_nIndex);
			Missle[pMissTempNode->m_nIndex].m_nRegionId = -1;
		}

		pMissTempNode->Remove();
		pMissTempNode->Release();
	}

	// ͬʱ��� clientonly ���͵� obj ---- zroc add
	/*VEC_LIST::iterator it = m_ObjList.begin();
	for(; it != m_ObjList.end(); ++it)
	{
#ifndef _SERVER
		//Object[*it].Remove(FALSE);
		Object[*it].Remove(FALSE,TRUE);
#endif
		ObjSet.Remove(*it); 
		//g_cObjMgr.Remove(*it);        // TODO:
		Object[*it].m_nRegionIdx = -1;
	}
	m_ObjList.clear();*/

	KIndexNode* pObjNode = NULL;
	KIndexNode* pObjTempNode = NULL;

	pObjNode = (KIndexNode *)m_ObjList.GetHead();
	while(pObjNode)
	{
		pObjTempNode = pObjNode;
		pObjNode     = (KIndexNode *)pObjNode->GetNext();
		
	  if (pObjTempNode->m_nIndex>0)
	  {
		  //ObjSet.RemoveIfClientOnly(pObjTempNode->m_nIndex);//ԭ����ע������
		  //ObjSet.Remove(pObjTempNode->m_nIndex);            //ԭ����ע������
		  //Object[pObjTempNode->m_nIndex].m_nRegionIdx = -1;
		  pObjTempNode->Remove();
		  pObjTempNode->Release();
		  Object[pObjTempNode->m_nIndex].Remove(FALSE,false);  //�ͻ����Ѿ�ɾ���ڴ�		  
		  //ObjSet.Remove(pObjTempNode->m_nIndex);             // TODO:
	  }
	  else
	  {
		pObjTempNode->Remove();
		pObjTempNode->Release();
		if (pObjTempNode)
		{
		  delete pObjTempNode;
		  pObjTempNode=NULL;
		}
	  }
	}
	
	KIndexNode* ppNode = NULL;
	KIndexNode* ppTempNode = NULL;

	ppNode = (KIndexNode *)m_PlayerList.GetHead();
	while(ppNode)
	{
		ppTempNode = ppNode;
		ppNode = (KIndexNode *)ppNode->GetNext();

		ppTempNode->Remove();
		ppTempNode->Release();
	}

	m_RegionID	= -1;
	m_nIndex	= -1;
	m_nActive	= 0;
	memset(m_nConnectRegion, -1, sizeof(m_nConnectRegion));
	memset(m_nConRegionID, -1, sizeof(m_nConRegionID));

}

