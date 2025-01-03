#ifndef KRegionH
#define KRegionH

//-----------------------------------------------------------------------
#include "engine/KEngine.h"
#include "KNpc.h"
#include "KNpcSet.h"
#include "KObj.h"
// #include "KWorldMsg.h"
// �����ӵ�
// #include "Scene/SceneDataDef.h"
#include <vector>
#include <list>
#include <map>
//-----------------------------------------------------------------------
#define MAX_REGION         9
#define REGION_GRID_WIDTH  16
#define REGION_GRID_HEIGHT 32
#define REGION_WIDTH       16
#define REGION_HEIGHT      32

/*enum MOVE_OBJ_KIND
{
        obj_npc,
        obj_object,
        //obj_missle,
}; */
class KRegion
{
    friend class KSubWorld;

public:
    typedef std::vector<INT> VEC_LIST;
    int m_nIndex;  // ��ͼ����
    int m_nSubMapidx;
    int m_RegionID;
    KList m_NpcList;  // �����б�
    KList m_ObjList;  // ����б�
    // VEC_LIST	m_ObjList;							// ����б�
    KList m_MissleList;        // �ӵ��б�
    KList m_PlayerList;        // ����б�
    int m_nConnectRegion[8];   // ���ڵĵ�ͼ����
    int m_nConRegionID[8];     // ���ڵĵ�ͼID
    KRegion* m_pConRegion[8];  // ���ڵ�Region
    int m_nRegionX;            // �������е�λ��X�����ص㣩
    int m_nRegionY;            // �������е�λ��Y�����ص㣩
    int m_nWidth;
    int m_nHeight;

private:
    //--------------------------------------------------------------------------
    enum _KEACT_TYPE
    {
        emACT_TYPE_NONE = 0,  // һ�л��ֹ
        emACT_TYPE_WAIT,      // �ȴ���ֹ����������Missile��Objδ��ʧ
        emACT_TYPE_ALL,       // ȫ�������Χ�����
    };

    class _KGridData
    {
    public:
        _KGridData()
        {
            // ZeroStruct(m_adwDataFlag);
            memset(&m_adwDataFlag, 0, sizeof(m_adwDataFlag));
        }

        int HasData(INT nX, INT nY) const { return (m_adwDataFlag[nX] & (1 << nY)) != 0; }
        INT GetData(INT nX, INT nY) const
        {
            INT nResult = 0;
            // KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
            // KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
            if (nX < 0 || nX >= REGION_WIDTH)
                goto Exit0;
            if (nY < 0 || nY >= REGION_HEIGHT)
                goto Exit0;

            if (m_adwDataFlag[nX] & (1 << nY))
            {
                nResult = m_mapData.find(MAKELONG(nX, nY))->second;
            }
        Exit0:
            return nResult;
        }
        int SetData(INT nX, INT nY, INT nData)
        {
            unsigned long dwId = MAKELONG(nX, nY);
            // KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
            // KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
            if (nX < 0 || nX >= REGION_WIDTH)
                goto Exit0;
            if (nY < 0 || nY >= REGION_HEIGHT)
                goto Exit0;

            if (nData)
            {
                m_adwDataFlag[nX] |= 1 << nY;
                m_mapData[dwId] = nData;
            }
            else
            {
                m_adwDataFlag[nX] &= ~(1 << nY);
                m_mapData.erase(dwId);
            }
        Exit0:
            return TRUE;
        }
        int IncData(INT nX, INT nY)
        {
            unsigned long dwId = MAKELONG(nX, nY);
            // KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
            // KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
            if (nX < 0 || nX >= REGION_WIDTH)
                goto Exit0;
            if (nY < 0 || nY >= REGION_HEIGHT)
                goto Exit0;

            if (m_adwDataFlag[nX] & (1 << nY))  // �Ѿ��еģ�+1
            {
                m_mapData[dwId]++;
            }
            else  // �µģ�ֱ����1
            {
                m_adwDataFlag[nX] |= 1 << nY;
                m_mapData[dwId] = 1;
            }
        Exit0:
            return TRUE;
        }
        int DecData(INT nX, INT nY)
        {
            int bResult = FALSE;
            unsigned long dwId   = MAKELONG(nX, nY);
            // KGLOG_PROCESS_ERROR(nX >= 0 && nX < REGION_WIDTH);
            // KGLOG_PROCESS_ERROR(nY >= 0 && nY < REGION_HEIGHT);
            if (nX < 0 || nX >= REGION_WIDTH)
                goto Exit0;
            if (nY < 0 || nY >= REGION_HEIGHT)
                goto Exit0;

            // KGLOG_PROCESS_ERROR(m_adwDataFlag[nX] & (1 << nY));
            if (m_adwDataFlag[nX] & (1 << nY))
            {
                if (--m_mapData[dwId] == 0)  // ��Ϊ0��
                {
                    m_mapData.erase(dwId);
                    m_adwDataFlag[nX] &= ~(1 << nY);
                }
            }
        Exit0:
            return bResult;
        }
        int Clear()
        {
            // ZeroStruct(m_adwDataFlag);
            memset(&m_adwDataFlag, 0, sizeof(m_adwDataFlag));
            // m_mapData::iterator it;
            // for(it = m_mapData.begin(); it != m_mapData.end(); ++it)
            //    m_mapData.erase(it);
            m_mapData.clear();
            return TRUE;
        }

    private:
        unsigned long m_adwDataFlag[REGION_WIDTH];  // ��λ�����ĳ���Ƿ������ݣ��պ�REGION_HEIGHT == 32����һ��unsigned long��
        typedef std::map<unsigned long, INT> _mapData;
        _mapData m_mapData;
    };
    //---------------------------------------------------------
    int m_nNpcSyncCounter;  // ͬ��������
    // int		m_nLoopCounter;					// ͬ��������
    int m_nObjSyncCounter;
    int m_nActive;  // �Ƿ񼤻�Ƿ�������ڸ�����
    // BYTE*		m_pNpcRef;							// ÿ�������ϵ�NPC��Ŀ
    // BYTE*		m_pObjRef;							// ÿ�������ϵ�OBJ��Ŀ
    // BYTE*		m_pMslRef;							// ÿ�������ϵ�MISSLE��Ŀ
    _KGridData m_cNpcRef;  // ÿ�������ϵ�NPC��Ŀ
    _KGridData m_cObjRef;  // ÿ�������ϵ�OBJ��Ŀ
public:
    KRegion();
    ~KRegion();
    int Init(int nWidth, int nHeight);
    int Load(int nX, int nY);

    // ����ͻ��˵�ͼ�ϱ�region �� object���ݣ�����npc��box�ȣ�
    // ��� bLoadNpcFlag == TRUE ��Ҫ���� clientonly npc else ������
    int LoadObjectC(int nSubWorld, int nX, int nY, char* lpszPath);
    // ����ͻ��˵�ͼ�ϱ� region �� clientonlynpc ����
    int LoadClientNpc(KPakFile* pFile, unsigned long dwDataSize);
    // ����ͻ��˵�ͼ�ϱ� region �� clientonlyobj ����
    int LoadClientObj(KPakFile* pFile, unsigned long dwDataSize);
    // �����ϰ����ݸ�С��ͼ
    static void LoadLittleMapData(int nX, int nY, char* lpszPath, INT* lpbtObstacle);

    static void GetFliePath(char* nName, char* nPath, int nMaxRows);

    void Close();
    void Activate(int mSubWorldID);
    void NewRegActivate();
    void ActivateObj(int mSubWorldID);
    BYTE GetBarrier(int MapX, int MapY, int nDx, int nDy);  //	��ͼ�߶�
    BYTE GetNewBarrier(int nMapX, int nMapY, int nDx, int nDy, int nIsCheckNpc = FALSE);
    // �� ���ص����� * 1024 �ľ����ж�ĳ��λ���Ƿ��ϰ�
    // ������nGridX nGirdY ����region��������
    // ������nOffX nOffY �������ڵ�ƫ����(���ص� * 1024 ����)
    // ������bCheckNpc ���Ƿ��ж�npc�γɵ��ϰ�
    // ����ֵ���ϰ�����(if ���� == Obstacle_NULL ���ϰ�)
    BYTE GetBarrierMin(int nGridX, int nGridY, int nOffX, int nOffY, int bCheckNpc);
    BYTE GetBarrierNewMin(int nGridX, int nGridY, int nOffX, int nOffY, int bCheckNpc);

    unsigned long GetTrap(int MapX, int MapY);  //	�õ�Trap���
    int SetTrap(int MapX, int MapY, int nCellNum = 1, unsigned long uTrapScriptId = 0);

    inline int IsActiveAll() { return TRUE; }

    inline int IsActive() { return TRUE; };

    int HasNpc(INT nMapX, INT nMapY) const { return m_cNpcRef.HasData(nMapX, nMapY); }
    INT GetNpcRef(INT nMapX, INT nMapY) const { return m_cNpcRef.HasData(nMapX, nMapY); }
    int AddNpcRef(INT nMapX, INT nMapY) { return m_cNpcRef.IncData(nMapX, nMapY); }
    int DecNpcRef(INT nMapX, INT nMapY) { return m_cNpcRef.DecData(nMapX, nMapY); }

    int HasObj(INT nMapX, INT nMapY) const { return m_cObjRef.HasData(nMapX, nMapY); }
    INT GetObjRef(INT nMapX, INT nMapY) const { return m_cObjRef.GetData(nMapX, nMapY); }
    int AddObjRef(INT nMapX, INT nMapY) { return m_cObjRef.IncData(nMapX, nMapY); }
    int DecObjRef(INT nMapX, INT nMapY) { return m_cObjRef.DecData(nMapX, nMapY); }
    /*
           int			GetRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType);
           int		AddRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType);
           int		DecRef(int nMapX, int nMapY, MOVE_OBJ_KIND nType);
      */
    int FindNpc(int nMapX, int nMapY, int nNpcIdx, unsigned long nRelation, int nType = 0);
    int CheckNpcInDisten(int nScyNpcIdx, int nTarNpcIdx, int nDis);
    int FindNpcAuto(int nMapX, int nMapY, int nNpcIdx, unsigned long nRelation, int nType = 0, int nDis = 0);
    // int		FindEquip(int nMapX, int nMapY);
    int FindObjectAuto(int nMapX, int nMapY, int nNpcIdx = 0, void* pData = NULL, int* m_CurState = 0);
    int FindObject(int nObjID);
    void* GetObjNode(int nIdx);
    // VEC_LIST::iterator	GetObjNode(INT nIdx);
    int SearchNpc(unsigned long dwNpcID);  // Ѱ�ұ��������Ƿ���ĳ��ָ�� id �� npc (zroc add)

    void Paint();
    void AddNpc(int nIdx);
    void RemoveNpc(int nIdx);
    // void	    RemoveNpcwor(int nIdx);
    void AddMissle(int nIdx);
    void RemoveMissle(int nIdx);
    void AddObj(int nIdx);
    void RemoveObj(int nIdx);
    int AddPlayer(int nIdx);
    int RemovePlayer(int nIdx);
};

inline int KRegion::CheckNpcInDisten(int nScyNpcIdx, int nTarNpcIdx, int nDis)
{
    if (nScyNpcIdx <= 0 || nTarNpcIdx <= 0)
        return FALSE;

    int nScyX, nScyY, nTarX, nTarY, nScymap, nTarmap;
    Npc[nScyNpcIdx].GetMpsPos(&nScyX, &nScyY, &nScymap);
    Npc[nTarNpcIdx].GetMpsPos(&nTarX, &nTarY, &nTarmap);

    if (nScymap != nTarmap)
        return FALSE;

    int nCurDis = 0;
    nCurDis     = (int)sqrt(double((nScyX - nTarX)) * (nScyX - nTarX) + (nScyY - nTarY) * (nScyY - nTarY));

    if (nCurDis <= nDis)
        return TRUE;
    else
        return FALSE;
}
//--------------------------------------------------------------------------
//	Find Npc
//--------------------------------------------------------------------------
inline int KRegion::FindNpc(int nMapX, int nMapY, int nNpcIdx, unsigned long nRelation, int nType)
{
    // if (m_pNpcRef[nMapY * m_nWidth + nMapX] == 0)
    //	return 0;
    if (GetNpcRef(nMapX, nMapY) <= 0)
        return 0;

    KIndexNode* pNode = NULL;

    pNode = (KIndexNode*)m_NpcList.GetHead();

    while (pNode)
    {
        if (Npc[pNode->m_nIndex].m_MapX == nMapX && Npc[pNode->m_nIndex].m_MapY == nMapY)
        {
            if (nType == 0)
            {
                if (NpcSet.GetRelation(nNpcIdx, pNode->m_nIndex) & nRelation)  // λ�ƶԱ� �ǵ��˵Ļ�
                {
                    return pNode->m_nIndex;
                }
            }
            else if (nType == 1)
            {
                if (nRelation == Npc[pNode->m_nIndex].m_NpcSettingIdx)
                {
                    return pNode->m_nIndex;
                }
            }
            else if (nType == 2)
            {
                if (nRelation == Npc[pNode->m_nIndex].m_dwID)
                {
                    return pNode->m_nIndex;
                }
            }
        }
        pNode = (KIndexNode*)pNode->GetNext();
    }
    return 0;
}

// �һ���ʹ�õĲ���NPC
inline int KRegion::FindNpcAuto(int nMapX, int nMapY, int nNpcIdx, unsigned long nRelation, int nType, int nDis)
{
    // if (m_pNpcRef[nMapY * m_nWidth + nMapX] == 0)
    //	return 0;
    if (GetNpcRef(nMapX, nMapY) <= 0)
        return 0;

    KIndexNode* pNode = NULL;

    pNode = (KIndexNode*)m_NpcList.GetHead();

    while (pNode)
    {
        if (Npc[pNode->m_nIndex].m_MapX == nMapX && Npc[pNode->m_nIndex].m_MapY == nMapY)
        {
            if (nType == 0)
            {
                if (NpcSet.GetRelation(nNpcIdx, pNode->m_nIndex) & nRelation)  // λ�ƶԱ� �ǵ��˵Ļ�
                {
                    if (Npc[pNode->m_nIndex].m_AttackerDwid != Npc[nNpcIdx].m_dwID)
                    {
                        if (nDis > 0)
                        {
                            if (CheckNpcInDisten(nNpcIdx, pNode->m_nIndex, nDis))
                                return pNode->m_nIndex;
                        }
                        else
                            return pNode->m_nIndex;
                    }
                }
            }
            else if (nType == 1)
            {
                if (nRelation == Npc[pNode->m_nIndex].m_NpcSettingIdx)
                {
                    if (Npc[pNode->m_nIndex].m_AttackerDwid != Npc[nNpcIdx].m_dwID)
                    {
                        if (nDis > 0)
                        {
                            if (CheckNpcInDisten(nNpcIdx, pNode->m_nIndex, nDis))
                                return pNode->m_nIndex;
                        }
                        else
                            return pNode->m_nIndex;
                    }
                }
            }
            else if (nType == 2)
            {
                if (nRelation == Npc[pNode->m_nIndex].m_dwID)
                {
                    if (Npc[pNode->m_nIndex].m_AttackerDwid != Npc[nNpcIdx].m_dwID)
                    {
                        if (nDis > 0)
                        {
                            if (CheckNpcInDisten(nNpcIdx, pNode->m_nIndex, nDis))
                                return pNode->m_nIndex;
                        }
                        else
                            return pNode->m_nIndex;
                    }
                }
            }
        }
        pNode = (KIndexNode*)pNode->GetNext();
    }
    return 0;
}

inline int KRegion::FindObjectAuto(int nMapX, int nMapY, int nNpcIdx, void* pData, int* m_CurState)
{
    if (!pData)
        return 0;
    gameAutoplaerInfo* pAutoInfo = (gameAutoplaerInfo*)pData;
    KIndexNode* pNode            = NULL;
    pNode                        = (KIndexNode*)m_ObjList.GetHead();
    char nTempItemName[128];
    while (pNode)
    {
        if (KObject[pNode->m_nIndex].m_nMapX == nMapX && KObject[pNode->m_nIndex].m_nMapY == nMapY)
        {
            if (KObject[pNode->m_nIndex].m_AttackerDwid != Npc[nNpcIdx].m_dwID)
            {  // û��ȡ��ʰȡ�ġ�
                // sprintf(nTempItemName,UTEXT(KObject[pNode->m_nIndex].m_szName,1).c_str());
                // if (nItemName && strstr(nTempItemName,nItemName))
                //	return pNode->m_nIndex;
                // ����
                if (pAutoInfo->isopenmoney && KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Money)
                {                     // �������ʰȡǮ
                    *m_CurState = 3;  // PICCK_ITEM_MONEY;
                    return pNode->m_nIndex;
                }
                // ����
                if (pAutoInfo->isopendaoju && KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Item)
                {  // ��ʼѭ����⡡�Ƿ����б��ڡ�����ڷ��� Npc[m_Index].m_AutoplayId==1
                    if (Npc[nNpcIdx].m_AutoplayId == 1)
                    {  // �һ�״̬
                        char _nItemName[64] = {0};
                        for (int i = 0; i < 16; ++i)
                        {
                            nToolItem.GetString(i + 1, 1, (char*)"������", _nItemName, sizeof(_nItemName));
                            sprintf(nTempItemName, "%s", UTEXT(KObject[pNode->m_nIndex].m_szName, 1).c_str());
                            if (strstr(nTempItemName, _nItemName))
                            {
                                *m_CurState = 5;  // PICCK_TOOL;
                                return pNode->m_nIndex;
                            }
                        }
                    }
                    else
                    {  // �ֶ�ʰȡ��
                        if (KObject[pNode->m_nIndex].m_nColorID == 8)
                        {
                            *m_CurState = 5;  // PICCK_TOOL;
                            return pNode->m_nIndex;
                        }
                    }
                }
                // װ��
                if (pAutoInfo->insopenzhuang && KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Item)
                {  // ��ʼѭ����⡡�Ƿ����б��ڡ�����ڷ���
                    if (KObject[pNode->m_nIndex].m_nColorID == 1 || KObject[pNode->m_nIndex].m_nColorID == 3 ||
                        KObject[pNode->m_nIndex].m_nColorID == 4)
                    {
                        *m_CurState = 2;  // PICCK_ITEM;
                        return pNode->m_nIndex;
                    }
                }
                // װ����Ǯ
                if (pAutoInfo->ismonyeandzhuang)
                {  // ��ʼѭ����⡡�Ƿ����б��ڡ�����ڷ���
                    if (KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Item &&
                        (KObject[pNode->m_nIndex].m_nColorID == 1 || KObject[pNode->m_nIndex].m_nColorID == 3 ||
                         KObject[pNode->m_nIndex].m_nColorID == 4))
                    {
                        *m_CurState = 2;  // PICCK_ITEM;
                        return pNode->m_nIndex;
                    }

                    if (KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Money)
                    {                     // �������ʰȡǮ
                        *m_CurState = 3;  // PICCK_ITEM_MONEY;
                        return pNode->m_nIndex;
                    }
                }
            }
        }
        pNode = (KIndexNode*)pNode->GetNext();
    }
    *m_CurState = 0;
    return 0;

    //----------------------------------------------------
    /*if (nKind==-1)
    {
            while(pNode)
            {//ֱ�Ӽ������
                    if (KObject[pNode->m_nIndex].m_nMapX == nMapX && KObject[pNode->m_nIndex].m_nMapY == nMapY)
                    {
                            if (KObject[pNode->m_nIndex].m_AttackerDwid!=Npc[nNpcIdx].m_dwID)
                            {
                                    sprintf(nTempItemName,UTEXT(KObject[pNode->m_nIndex].m_szName,1).c_str());
                                    if (nItemName && strstr(nTempItemName,nItemName))
                                            return pNode->m_nIndex;
                            }
                    }
                    pNode = (KIndexNode *)pNode->GetNext();
            }
            return 0;

    }
    else
    {
            while(pNode)
            {
                    if (KObject[pNode->m_nIndex].m_nMapX == nMapX && KObject[pNode->m_nIndex].m_nMapY == nMapY)
                    {
                            if (nKind==1 && KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Money &&
    KObject[pNode->m_nIndex].m_AttackerDwid!=Npc[nNpcIdx].m_dwID) return pNode->m_nIndex; else if (nKind==2 &&
    KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Item && KObject[pNode->m_nIndex].m_AttackerDwid!=Npc[nNpcIdx].m_dwID)
                            {//�����Ե�װ��
                                    if (KObject[pNode->m_nIndex].m_nColorID==1 || KObject[pNode->m_nIndex].m_nColorID==3
    || KObject[pNode->m_nIndex].m_nColorID==4) return pNode->m_nIndex;
                            }
                            else  if (nKind==3 && KObject[pNode->m_nIndex].m_AttackerDwid!=Npc[nNpcIdx].m_dwID)
                            {
                                    if  (KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Money)
                                            return pNode->m_nIndex;
                                    else if (KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Item)
                                    {
                                            if (KObject[pNode->m_nIndex].m_nColorID==1 ||
    KObject[pNode->m_nIndex].m_nColorID==3 || KObject[pNode->m_nIndex].m_nColorID==4) return pNode->m_nIndex;
                                    }
                            }
                            else if (nKind==4 && nItemName &&
    KObject[pNode->m_nIndex].m_AttackerDwid!=Npc[nNpcIdx].m_dwID)
                            {//ͼ��
                                    sprintf(nTempItemName,UTEXT(KObject[pNode->m_nIndex].m_szName,1).c_str());
                                    if (strstr(nTempItemName,nItemName))
                                            return pNode->m_nIndex;
                            }
                    }
                    pNode = (KIndexNode *)pNode->GetNext();
            }
            return 0;

    }
    return 0;
    */
}

/*
inline int KRegion::FindObjectAuto(int nMapX, int nMapY,int nKind,char *nItemName,int nNpcIdx,void *pData,int
*m_CurState)
{
KIndexNode *pNode = NULL;
pNode = (KIndexNode *)m_ObjList.GetHead();
char nTempItemName[128];
if (nKind==-1)
{
while(pNode)
{
if (KObject[pNode->m_nIndex].m_nMapX == nMapX && KObject[pNode->m_nIndex].m_nMapY == nMapY)
{
if (KObject[pNode->m_nIndex].m_AttackerDwid!=Npc[nNpcIdx].m_dwID)
{
sprintf(nTempItemName,UTEXT(KObject[pNode->m_nIndex].m_szName,1).c_str());
if (nItemName && strstr(nTempItemName,nItemName))
return pNode->m_nIndex;
}
}
pNode = (KIndexNode *)pNode->GetNext();
}
return 0;

}
else
{
while(pNode)
{
if (KObject[pNode->m_nIndex].m_nMapX == nMapX && KObject[pNode->m_nIndex].m_nMapY == nMapY)
{
if (nKind==1 && KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Money &&
KObject[pNode->m_nIndex].m_AttackerDwid!=Npc[nNpcIdx].m_dwID) return pNode->m_nIndex; else if (nKind==2 &&
KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Item && KObject[pNode->m_nIndex].m_AttackerDwid!=Npc[nNpcIdx].m_dwID)
{
if (KObject[pNode->m_nIndex].m_nColorID==1 || KObject[pNode->m_nIndex].m_nColorID==3 ||
KObject[pNode->m_nIndex].m_nColorID==4) return pNode->m_nIndex;
}
else  if (nKind==3 && KObject[pNode->m_nIndex].m_AttackerDwid!=Npc[nNpcIdx].m_dwID)
{
if  (KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Money)
return pNode->m_nIndex;
else if (KObject[pNode->m_nIndex].m_nKind == Obj_Kind_Item)
{
if (KObject[pNode->m_nIndex].m_nColorID==1 || KObject[pNode->m_nIndex].m_nColorID==3 ||
KObject[pNode->m_nIndex].m_nColorID==4) return pNode->m_nIndex;
}
}
else if (nKind==4 && nItemName && KObject[pNode->m_nIndex].m_AttackerDwid!=Npc[nNpcIdx].m_dwID)
{//ͼ��
sprintf(nTempItemName,UTEXT(KObject[pNode->m_nIndex].m_szName,1).c_str());
if (strstr(nTempItemName,nItemName))
return pNode->m_nIndex;
}
}
pNode = (KIndexNode *)pNode->GetNext();
}
return 0;

}
return 0;
}

*/
#endif
