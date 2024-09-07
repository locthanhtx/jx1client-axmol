/*******************************************************************************
// FileName			:	KMissle.cpp
// FileAuthor		:	RomanDou
// FileCreateDate	:	2002-6-10 15:32:22
// FileDescription	:
// Revision Count	:
*******************************************************************************/

#include "KCore.h"
#include "KMissle.h"
#include "KSubWorld.h"
#include "KSubWorldSet.h"
#include "KRegion.h"
#include "KNpc.h"
#include "KNpcSet.h"
#include "KMath.h"
#include <math.h>
#include "KSkillSpecial.h"
// #include "myassert.h"
// #include "../../Represent/iRepresent/iRepresentshell.h"
// #include "Scene\KScenePlaceC.h"
#include "ImgRef.h"
#include "gamescene/ObstacleDef.h"
#include "KPlayer.h"
#include "KMissleSet.h"

#include "engine/KSG_StringProcess.h"

/*TCollisionMatrix g_CollisionMatrix[64] =
{
        {0,	0, -1, 1, 0, 1, 1, 1}, // 0--------
        {0,	0, -1, 1, 0, 1, 1, 1}, // 1
        {0,	0, -1, 1, 0, 1, 1, 1}, // 2
        {0,	0, -1, 1, 0, 1, 1, 1}, // 3

        {0,	0, -1, 0, -1, 1, 0, 1}, // 4
        {0,	0, -1, 0, -1, 1, 0, 1}, // 5
        {0,	0, -1, 0, -1, 1, 0, 1}, // 6
        {0,	0, -1, 0, -1, 1, 0, 1}, // 7
        {0,	0, -1, 0, -1, 1, 0, 1}, // 8--------
        {0,	0, -1, 0, -1, 1, 0, 1}, // 9
        {0,	0, -1, 0, -1, 1, 0, 1}, // 10
        {0,	0, -1, 0, -1, 1, 0, 1}, // 11

        {0,	0, -1, -1, -1, 0, -1, 1}, // 12
        {0,	0, -1, -1, -1, 0, -1, 1}, // 13
        {0,	0, -1, -1, -1, 0, -1, 1}, // 14
        {0,	0, -1, -1, -1, 0, -1, 1}, // 15
        {0,	0, -1, -1, -1, 0, -1, 1}, // 16--------
        {0,	0, -1, -1, -1, 0, -1, 1}, // 17
        {0,	0, -1, -1, -1, 0, -1, 1}, // 18
        {0,	0, -1, -1, -1, 0, -1, 1}, // 19

        {0,	0, 0, -1, -1, -1, -1, 0}, // 20
        {0,	0, 0, -1, -1, -1, -1, 0}, // 21
        {0,	0, 0, -1, -1, -1, -1, 0}, // 22
        {0,	0, 0, -1, -1, -1, -1, 0}, // 23
        {0,	0, 0, -1, -1, -1, -1, 0}, // 24--------
        {0,	0, 0, -1, -1, -1, -1, 0}, // 25
        {0,	0, 0, -1, -1, -1, -1, 0}, // 26
        {0,	0, 0, -1, -1, -1, -1, 0}, // 27

        {0,	0, -1, -1, 0, -1, 1, -1}, // 28
        {0,	0, -1, -1, 0, -1, 1, -1}, // 29
        {0,	0, -1, -1, 0, -1, 1, -1}, // 30
        {0,	0, -1, -1, 0, -1, 1, -1}, // 31
        {0,	0, -1, -1, 0, -1, 1, -1}, // 32--------
        {0,	0, -1, -1, 0, -1, 1, -1}, // 33
        {0,	0, -1, -1, 0, -1, 1, -1}, // 34
        {0,	0, -1, -1, 0, -1, 1, -1}, // 35

        {0,	0, 0, -1, 1, -1, 1, 0}, // 36
        {0,	0, 0, -1, 1, -1, 1, 0}, // 37
        {0,	0, 0, -1, 1, -1, 1, 0}, // 38
        {0,	0, 0, -1, 1, -1, 1, 0}, // 39
        {0,	0, 0, -1, 1, -1, 1, 0}, // 40--------
        {0,	0, 0, -1, 1, -1, 1, 0}, // 41
        {0,	0, 0, -1, 1, -1, 1, 0}, // 42
        {0,	0, 0, -1, 1, -1, 1, 0}, // 43

        {0,	0, 1, -1, 1, 0, 1, 1}, // 44
        {0,	0, 1, -1, 1, 0, 1, 1}, // 45
        {0,	0, 1, -1, 1, 0, 1, 1}, // 46
        {0,	0, 1, -1, 1, 0, 1, 1}, // 47
        {0,	0, 1, -1, 1, 0, 1, 1}, // 48--------
        {0,	0, 1, -1, 1, 0, 1, 1}, // 49
        {0,	0, 1, -1, 1, 0, 1, 1}, // 50
        {0,	0, 1, -1, 1, 0, 1, 1}, // 51

        {0,	0, 1, 0, 1, 1, 0, 1}, // 52
        {0,	0, 1, 0, 1, 1, 0, 1}, // 53
        {0,	0, 1, 0, 1, 1, 0, 1}, // 54
        {0,	0, 1, 0, 1, 1, 0, 1}, // 55
        {0,	0, 1, 0, 1, 1, 0, 1}, // 56---------
        {0,	0, 1, 0, 1, 1, 0, 1}, // 57
        {0,	0, 1, 0, 1, 1, 0, 1}, // 58
        {0,	0, 1, 0, 1, 1, 0, 1}, // 59

        {0,	0, -1, 1, 0, 1, 1, 1}, // 60
        {0,	0, -1, 1, 0, 1, 1, 1}, // 61
        {0,	0, -1, 1, 0, 1, 1, 1}, // 62
        {0,	0, -1, 1, 0, 1, 1, 1}, // 63
};
*/
KMissle g_MisslesLib[MAX_MISSLESTYLE];  // KMissle *g_MisslesLib;//

// extern KMissleRes m_MissleRes;

// ÿ�����ӵ����س���
#define CellWidth  (SubWorld[m_nSubWorldId].m_nCellWidth << 10)
#define CellHeight (SubWorld[m_nSubWorldId].m_nCellHeight << 10)

// ÿ��region��㳤��
#define RegionWidth            (SubWorld[m_nSubWorldId].m_nRegionWidth)
#define RegionHeight           (SubWorld[m_nSubWorldId].m_nRegionHeight)

#define CurRegion              SubWorld[m_nSubWorldId].m_Region[m_nRegionId]
#define CurSubWorld            SubWorld[m_nSubWorldId]

#define LeftRegion(nRegionId)  SubWorld[m_nSubWorldId].m_Region[nRegionId].m_nConnectRegion[2]
#define RightRegion(nRegionId) SubWorld[m_nSubWorldId].m_Region[nRegionId].m_nConnectRegion[6]
#define UpRegion(nRegionId)    SubWorld[m_nSubWorldId].m_Region[nRegionId].m_nConnectRegion[4]
#define DownRegion(nRegionId)  SubWorld[m_nSubWorldId].m_Region[nRegionId].m_nConnectRegion[0]

// ����ƶ�ħ��������ƫ�Ʊ�
// int g_nRandMissleTab[100] = {0};

KMissle* Missle = NULL;  // Missle[MAX_MISSLE];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
KMissle::KMissle()
{
    m_nMissleId            = -1;
    m_nCollideOrVanishTime = 0;
    m_ulDamageInterval     = 0;
    m_nTempParam1          = 0;
    m_nTempParam2          = 0;
    m_nFirstReclaimTime    = 0;
    m_nEndReclaimTime      = 0;
    m_bFollowNpcWhenCollid = 1;
    m_bRemoving            = FALSE;
    m_btRedLum = m_btGreenLum = m_btBlueLum = 0xff;
    m_usLightRadius                         = 50;  // �ƹⷶΧ
    m_SceneID                               = 0;
}

void KMissle::Release()
{
    /// #pragma	message(ATTENTION("�ӵ�����ʱ������·�����ʹ�øü���ʱ�ĵ�ǰʹ�ô�����ʹ֮��һ"))

    // g_ScenePlace.RemoveObject(CGOG_MISSLE, m_nMissleId,m_SceneID);	 //
    if (g_GameWorld)
        g_GameWorld->MoveObject(OBJ_NODE_MISS, m_nMissleId, true, 0, 0, 0);

    // SubWorld[m_nSubWorldId].m_WorldMessage.Send(GWM_MISSLE_DEL, m_nMissleId);
    m_MissleRes.Clear();
    m_nMissleId     = -1;
    m_nFollowNpcIdx = 0;
}

KMissle::~KMissle() {}
/*!*****************************************************************************
// Function		: KMissle::GetInfoFromTabFile
// Purpose		: ���TabFile�й��ӵ��Ļ�����Ϣ
// Return		: int
// Argumant		: int nMissleId
// Comments		:
// Author		: RomanDou
*****************************************************************************/
int KMissle::GetInfoFromTabFile(int nMissleId)
{
    if (nMissleId <= 0)
        return FALSE;
    KTabFile* pITabFile = &g_MisslesSetting;
    return GetInfoFromTabFile(pITabFile, nMissleId);
}

int KMissle::GetInfoFromTabFile(KTabFile* pMisslesSetting, int nMissleId)
{
    if (nMissleId <= 0)
        return FALSE;
    m_nMissleId = nMissleId;
    int nRow    = nMissleId;
    pMisslesSetting->GetString(nRow, "MissleName", "", m_szMissleName, sizeof(m_szMissleName), TRUE);

    int nHeightOld;
    pMisslesSetting->GetInteger(nRow, "MissleHeight", 0, &nHeightOld, TRUE);
    m_nHeight   = nHeightOld << 10;
    int ccUrval = 0;
    pMisslesSetting->GetInteger(nRow, "LifeTime", 0, &m_nLifeTime, TRUE);
    pMisslesSetting->GetInteger(nRow, "Speed", 0, &m_nSpeed, TRUE);  // �ӵ��ٶ�
    pMisslesSetting->GetInteger(nRow, "ResponseSkill", 0, &m_nSkillId, TRUE);
    pMisslesSetting->GetInteger(nRow, "CollidRange", 0, &m_nCollideRange, TRUE);  // ��ײ��Χ
    pMisslesSetting->GetInteger(nRow, "ColVanish", 0, &ccUrval, TRUE);            // ��ײ������
    m_bCollideVanish = (ccUrval > 0) ? true : false;
    pMisslesSetting->GetInteger(nRow, "CanColFriend", 0, &ccUrval, TRUE);
    m_bCollideFriend = (ccUrval > 0) ? true : false;
    pMisslesSetting->GetInteger(nRow, "CanSlow", 0, &ccUrval, TRUE);
    m_bCanSlow = (ccUrval > 0) ? true : false;
    pMisslesSetting->GetInteger(nRow, "IsRangeDmg", 0, &ccUrval, TRUE);  // �Ƿ����˺���Χ
    m_bRangeDamage = (ccUrval > 0) ? true : false;
    pMisslesSetting->GetInteger(nRow, "DmgRange", 0, &m_nDamageRange, TRUE);
    pMisslesSetting->GetInteger(nRow, "MoveKind", 0, (int*)&m_eMoveKind, TRUE);
    pMisslesSetting->GetInteger(nRow, "FollowKind", 0, (int*)&m_eFollowKind, TRUE);
    pMisslesSetting->GetInteger(nRow, "Zacc", 0, (int*)&m_nZAcceleration, TRUE);  // Z����
    pMisslesSetting->GetInteger(nRow, "Zspeed", 0, (int*)&m_nHeightSpeed, TRUE);
    pMisslesSetting->GetInteger(nRow, "Param1", 0, &m_nParam1, TRUE);
    pMisslesSetting->GetInteger(nRow, "Param2", 0, &m_nParam2, TRUE);
    pMisslesSetting->GetInteger(nRow, "Param3", 0, &m_nParam3, TRUE);

    int bAutoExplode = 0;

    m_nRate     = 0;
    m_nHitCount = 0;

    pMisslesSetting->GetInteger(nRow, "AutoExplode", 0, (int*)&ccUrval, TRUE);
    bAutoExplode   = (ccUrval > 0) ? true : false;
    m_bAutoExplode = bAutoExplode;

    pMisslesSetting->GetInteger(nRow, "DmgInterval", 0, (int*)&m_ulDamageInterval, TRUE);
    char AnimFileCol[64]      = {0};
    char SndFileCol[64]       = {0};
    char AnimFileInfoCol[100] = {0};
    char szAnimFileInfo[100]  = {0};

    const char* pcszTemp = NULL;

    pMisslesSetting->GetInteger(nRow, "RedLum", 255, (int*)&m_btRedLum, TRUE);
    pMisslesSetting->GetInteger(nRow, "GreenLum", 255, (int*)&m_btGreenLum, TRUE);
    pMisslesSetting->GetInteger(nRow, "BlueLum", 255, (int*)&m_btBlueLum, TRUE);

    int nLightRadius = 0;
    pMisslesSetting->GetInteger(nRow, "LightRadius", 50, (int*)&nLightRadius, TRUE);
    m_usLightRadius = nLightRadius;

    pMisslesSetting->GetInteger(nRow, "MultiShow", 0, &ccUrval, TRUE);
    m_bMultiShow = (ccUrval > 0) ? true : false;

    for (int i = 0; i < MAX_MISSLE_STATUS; ++i)  // �ӵ�״̬
    {                                            // 0 1 2 3 456
        sprintf(AnimFileCol, "AnimFile%d", i + 1);
        sprintf(SndFileCol, "SndFile%d", i + 1);
        sprintf(AnimFileInfoCol, "AnimFileInfo%d", i + 1);

        pMisslesSetting->GetString(nRow, AnimFileCol, "", m_MissleRes.m_MissleRes[i].AnimFileName,
                                   sizeof(m_MissleRes.m_MissleRes[i].AnimFileName), TRUE);
        pMisslesSetting->GetString(nRow, SndFileCol, "", m_MissleRes.m_MissleRes[i].SndFileName,
                                   sizeof(m_MissleRes.m_MissleRes[i].SndFileName), TRUE);
        pMisslesSetting->GetString(nRow, AnimFileInfoCol, "", szAnimFileInfo, sizeof(szAnimFileInfo), TRUE);

        if (m_MissleRes.m_MissleRes[i].SndFileName[0])
            g_StrCopy(m_SDcardDirPath, m_MissleRes.m_MissleRes[i].SndFileName);

        pcszTemp = szAnimFileInfo;

        m_MissleRes.m_MissleRes[i].nTotalFrame = KSG_StringGetInt(&pcszTemp, 100);
        KSG_StringSkipSymbol(&pcszTemp, ',');
        m_MissleRes.m_MissleRes[i].nDir = KSG_StringGetInt(&pcszTemp, 16);
        KSG_StringSkipSymbol(&pcszTemp, ',');
        m_MissleRes.m_MissleRes[i].nInterval = KSG_StringGetInt(&pcszTemp, 1);

        sprintf(AnimFileCol, "AnimFileB%d", i + 1);
        sprintf(SndFileCol, "SndFileB%d", i + 1);
        sprintf(AnimFileInfoCol, "AnimFileInfoB%d", i + 1);

        pMisslesSetting->GetString(nRow, AnimFileCol, "", m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].AnimFileName,
                                   sizeof(m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].AnimFileName), TRUE);
        pMisslesSetting->GetString(nRow, SndFileCol, "", m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].SndFileName,
                                   sizeof(m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].SndFileName), TRUE);
        pMisslesSetting->GetString(nRow, AnimFileInfoCol, "", szAnimFileInfo, sizeof(szAnimFileInfo), TRUE);

        if (m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].SndFileName[0])
            g_StrCopy(m_SDcardDirPath, m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].SndFileName);

        pcszTemp                                                   = szAnimFileInfo;
        m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].nTotalFrame = KSG_StringGetInt(&pcszTemp, 100);
        KSG_StringSkipSymbol(&pcszTemp, ',');
        m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].nDir = KSG_StringGetInt(&pcszTemp, 16);
        KSG_StringSkipSymbol(&pcszTemp, ',');
        m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].nInterval = KSG_StringGetInt(&pcszTemp, 1);

        // sscanf(szAnimFileInfo, "%d,%d,%d",
        //	&m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].nTotalFrame,
        //	&m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].nDir,
        //	&m_MissleRes.m_MissleRes[i + MAX_MISSLE_STATUS].nInterval
        //);
    }
    int ninVal = 0;
    pMisslesSetting->GetInteger(nRow, "LoopPlay", 0, &ninVal, TRUE);
    m_MissleRes.m_bLoopAnim = (ninVal > 0) ? true : false;
    pMisslesSetting->GetInteger(nRow, "SubLoop", 0, &ninVal, TRUE);
    m_MissleRes.m_bSubLoop = (ninVal > 0) > 0 ? true : false;
    pMisslesSetting->GetInteger(nRow, "SubStart", 0, &ninVal, TRUE);
    m_MissleRes.m_nSubStart = ninVal;
    pMisslesSetting->GetInteger(nRow, "SubStop", 0, &ninVal, TRUE);
    m_MissleRes.m_nSubStop = ninVal;
    pMisslesSetting->GetInteger(nRow, "ColFollowTarget", 0, (int*)&m_bFollowNpcWhenCollid, TRUE);
    return TRUE;
}

int KMissle::Init(int nLauncher, int nMissleId, int nXFactor, int nYFactor, int nLevel)
{
    m_MissleRes.Init();
    return TRUE;
}

/*!*****************************************************************************
// Function		: KMissle::Activate
// Purpose		:
// Return		: void
// Comments		:
// Author		: RomanDou
*****************************************************************************/
// �����ӵ����� ����ѭ�����ӵ���ײ�˺�Ч��
int KMissle::Activate()
{

    if (m_nMissleId <= 0 || m_nRegionId < 0)
    {
        return 0;
    }

    if (m_nLauncher <= 0)
    {
        DoVanish();
        return 0;
    }

    // �ӵ��������Ѿ��뿪��So �ӵ�����
    if (!Npc[m_nLauncher].IsMatch(m_dwLauncherId) || Npc[m_nLauncher].m_SubWorldIndex != m_nSubWorldId ||
        Npc[m_nLauncher].m_RegionIndex < 0)
    {
        DoVanish();
        return 0;
    }

    // ���ٵ�Ŀ�������Ѿ����ڸõ�ͼ��ʱ���Զ����
    if (m_nFollowNpcIdx > 0 && m_nFollowNpcIdx < MAX_NPC)
    {
        // 		if (!Npc[m_nFollowNpcIdx].IsMatch(m_dwFollowNpcID) || Npc[m_nFollowNpcIdx].m_SubWorldIndex !=
        // m_nSubWorldId)
        // 		{
        // 			m_nFollowNpcIdx = 0;
        // 		}
        if (Npc[m_nFollowNpcIdx].m_SubWorldIndex != m_nSubWorldId)
        {
            m_nFollowNpcIdx = 0;  // ���ӵ����ٵ� NPC����
        }
    }

    eMissleStatus eLastStatus = m_eMissleStatus;

    // �����ǰ״̬���ӵ���������������׼������״̬ʱ�������������л�������ײ��
    if (m_nCurrentLife >= m_nLifeTime && m_eMissleStatus != MS_DoVanish && m_eMissleStatus != MS_DoCollision)
    {
        if (m_bAutoExplode)
        {                        // ����ǰ�Ƿ��ٴ���ײ
            ProcessCollision();  // ������ײ
        }

        DoVanish();
    }
    //--------------
    if (m_nCurrentLife == m_nStartLifeTime && m_eMissleStatus != MS_DoVanish)
    {
        if (PrePareFly())
        {
            int nSrcX2 = 0;
            int nSrcY2 = 0;
            SubWorld[0].NewMap2Mps(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, &nSrcX2,
                                   &nSrcY2);
            m_MissleRes.PlaySound(MS_DoFly, nSrcX2, nSrcY2, 0);
            // CreateSpecialEffect(MS_DoFly, nSrcX2, nSrcY2, m_nCurrentMapZ); //��������Ч��
            DoFly();  // ���÷���״̬
        }
        else
            DoVanish();
    }
    //--------
    switch (m_eMissleStatus)
    {
    case MS_DoWait:
    {
        // OnWait();
    }
    break;
    case MS_DoFly:
    {
        OnFly();

        if (m_bFlyEvent)
        {
            if (m_nFlyEventTime == 0)
            {
                DoVanish();
                break;
            }

            if ((m_nCurrentLife - m_nStartLifeTime) % m_nFlyEventTime == 0)
            {
                //_ASSERT(m_nSkillId < MAX_SKILL && m_nLevel < MAX_SKILLLEVEL);
                // if (m_nLevel  <= 0 ) return 0;

                if (m_nSkillId >= MAX_SKILL || m_nLevel >= MAX_SKILLLEVEL || m_nLevel <= 0)
                    return 0;

                KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_nSkillId, m_nLevel);
                if (pOrdinSkill)
                {
                    pOrdinSkill->FlyEvent(this);
                }
            }
        }
    }
    break;
    case MS_DoCollision:
    {
        OnCollision();
    }
    break;
    case MS_DoVanish:
    {
        OnVanish();
    }
    break;
    }

    // �ӵ�δ������ �滭 �п��ܵ���ʹ��õ�廨 �ͻ��� ����
    if (m_nMissleId > 0 && m_eMissleStatus != MS_DoVanish)
    {
        int nSrcX;
        int nSrcY;
        SubWorld[0].NewMap2Mps(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, &nSrcX, &nSrcY);
        /*if (m_nLauncher>0 && m_nLauncher<MAX_NPC)
        {
                char msg[128]={0};
                sprintf(msg,"������:%s,%s",Npc[m_nLauncher].Name,m_szMissleName);
                messageBox(msg,"Activate");
        }*/
        if (m_usLightRadius && m_eMissleStatus != MS_DoWait)  // �滭�ӵ�
        {                                                     // �й��շ�Χ��
            // g_ScenePlace.MoveObject(CGOG_MISSLE, m_nMissleId, nSrcX, nSrcY, m_nCurrentMapZ, m_SceneID, IPOT_RL_OBJECT
            // | IPOT_RL_LIGHT_PROP );
            if (g_GameWorld)
                g_GameWorld->MoveObject(OBJ_NODE_MISS, m_nMissleId, false, nSrcX, nSrcY,
                                        m_nCurrentMapZ);  //, m_SceneID, IPOT_RL_OBJECT | IPOT_RL_LIGHT_PROP
        }
        else
        // g_ScenePlace.MoveObject(CGOG_MISSLE, m_nMissleId, nSrcX, nSrcY, m_nCurrentMapZ, m_SceneID, IPOT_RL_OBJECT);
        {
            if (g_GameWorld)
                g_GameWorld->MoveObject(OBJ_NODE_MISS, m_nMissleId, false, nSrcX, nSrcY, m_nCurrentMapZ);
        }
    }

    m_nCurrentLife++;
    return 1;
}

/*!*****************************************************************************
// Function		: KMissle::OnWait
// Purpose		:
// Return		: void
// Comments		:
// Author		: RomanDou
********************************************************************************/
void KMissle::OnWait()
{
    return;
}
/*!*****************************************************************************
// Function		: KMissle::OnCollision
// Purpose		:
// Return		: void
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void KMissle::OnCollision()
{
    return;
}

// 1��ʾ������ײ�����壬0��ʾδ��ײ���κ�����, -1��ʾ���
int KMissle::FsCheckCollision()
{
    if (m_nLauncher <= 0 || m_nLauncher >= MAX_NPC || m_nSubWorldId < 0 || m_nSubWorldId >= MAX_SUBWORLD)
        return FALSE;

    if (m_nCurrentMapZ <= MISSLE_MIN_COLLISION_ZHEIGHT)
    {
        return -1;
    }

    // �ӵ��ڸ���һ���߶�ʱ��������Խ����ײ����
    if (m_nCurrentMapZ > MISSLE_MAX_COLLISION_ZHEIGHT)
    {
        return 0;
    }

    if (m_nRegionId < 0)
    {
        return -1;
    }

    int nAbsX       = 0;
    int nAbsY       = 0;
    int nCellWidth  = CellWidth;
    int nCellHeight = CellHeight;
    //_ASSERT(nCellWidth > 0 && nCellHeight > 0);
    if (nCellWidth <= 0 || nCellHeight <= 0)
        return FALSE;

    int nRMx          = 0;
    int nRMy          = 0;
    int nSearchRegion = 0;
    int nNpcIdx       = 0;
    int nDX           = 0;
    int nDY           = 0;
    int nNpcOffsetX   = 0;
    int nNpcOffsetY   = 0;
    int bCollision    = FALSE;

    int nColRegion = m_nRegionId;
    int nColMapX   = m_nCurrentMapX;  // �ӵ��ĵ�ǰ��������X:0-16 Y:0-32
    int nColMapY   = m_nCurrentMapY;

    if (m_nCollideRange == 1)  // ��ײ��Χ
    {                          // ��������
        if (m_bNeedReclaim && m_nCurrentLife >= m_nFirstReclaimTime && m_nCurrentLife <= m_nEndReclaimTime)
        {  // �ڴ���ʱ���� ������ʱ��Χ�Ƿ���NPC
            if (m_nCurrentLife == m_nEndReclaimTime)
                m_bNeedReclaim = FALSE;

            // nNpcIdx = SubWorld[m_nSubWorldId].m_Region[nColRegion].FindNpc(nColMapX, nColMapY, m_nLauncher,
            // m_eRelation);
            nNpcIdx = CheckNearestCollision();
        }
        else
        {
            // nNpcIdx = SubWorld[m_nSubWorldId].m_Region[nColRegion].FindNpc(nColMapX, nColMapY, m_nLauncher,
            // m_eRelation);
            nNpcIdx = CheckNearestCollision();
        }
        int nIsOut = 0;

        if (nNpcIdx > 0 && nNpcIdx < MAX_NPC)
        {                             // ��NPC
            if (m_nDamageRange == 1)  // ��Ŀ��Npc����ײ  �˺���Χ
            {                         // һ��һ����ײ
                if (ProcessCollision(m_nLauncher, Npc[nNpcIdx].m_RegionIndex, Npc[nNpcIdx].m_MapX, Npc[nNpcIdx].m_MapY,
                                     m_nDamageRange, m_eRelation, nNpcIdx))
                    nIsOut = 1;
            }
            else
            {
                if (ProcessCollision())
                    nIsOut = 2;  // ���ӵ�λ�ô�����
            }
        }
        else
        {
            if (ProcessCollision())
                nIsOut = 3;  // ���ӵ�λ�ô�����ײ
        }

        if (nIsOut)
        {
            DoCollision();  // �ӵ�����ײ���Ч��
            return 1;
        }
    }
    else
    {  // Ⱥ��
        for (int i = -m_nCollideRange; i <= m_nCollideRange; ++i)
        {
            for (int j = -m_nCollideRange; j <= m_nCollideRange; ++j)
            {
                // ��ȡ�����Χ�ڵ����� �����ڵ�����
                if (!GetOffsetAxis(m_nSubWorldId, m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, i, j, nSearchRegion,
                                   nRMx, nRMy))
                    continue;

                //_ASSERT(nSearchRegion >= 0);
                if (nSearchRegion < 0)
                    continue;
                // ��������Ƿ���NPC
                nNpcIdx = SubWorld[m_nSubWorldId].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nLauncher, m_eRelation);
                int nIsOut = 0;
                if (nNpcIdx > 0 && nNpcIdx < MAX_NPC)
                {  // ����о� ��ײ
                    if (ProcessCollision(m_nLauncher, Npc[nNpcIdx].m_RegionIndex, Npc[nNpcIdx].m_MapX,
                                         Npc[nNpcIdx].m_MapY, m_nDamageRange, m_eRelation, nNpcIdx))
                        nIsOut = 1;
                }
                else
                {
                    if (ProcessCollision())  // ������ײ ���й�����Ҳ���Դ��е���
                        nIsOut = 2;
                }

                if (nIsOut)
                {
                    DoCollision();  // �ӵ�����ײ���Ч��
                    return 1;
                }
                /*if (nNpcIdx > 0)
                {
                        ProcessCollision();//������ײ
                        DoCollision();//�ӵ�����ײ���Ч��
                        return 1;
                } */
            }
        }
    }
    return 0;
}

inline unsigned long KMissle::GetCurrentSubWorldTime()
{
    return SubWorld[m_nSubWorldId].m_dwCurrentTime;
}
// �����ӵ�����
void KMissle::OnFly()
{

    if (m_nInteruptTypeWhenMove)
    {
        // ��������λ���ƶ��ˣ���������do_wait״̬��do_fly״̬�����ӵ�����ʧ��
        // �����ѽ���dofly״̬�ľɵ������ӵ�ҲҪǿ����ʧ��
        if (m_nInteruptTypeWhenMove == Interupt_EndOldMissleLifeWhenMove)
        {
            int nPX, nPY, nMp;
            Npc[m_nLauncher].GetMpsPos(&nPX, &nPY, &nMp);
            if (nPX != m_nLauncherSrcPX || nPY != m_nLauncherSrcPY)
            {
                int nSrcX2 = 0;
                int nSrcY2 = 0;
                SubWorld[0].NewMap2Mps(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, &nSrcX2,
                                       &nSrcY2);
                CreateSpecialEffect(MS_DoVanish, nSrcX2, nSrcY2, m_nCurrentMapZ);
                DoVanish();
                return;
            }
        }
    }

    // ���м�⵱ǰλ���Ƿ����ϰ�
    /*if (TestBarrier())
    {//���ϰ���
            int nSrcX3 = 0 ;
            int nSrcY3 = 0 ;
            SubWorld[0].NewMap2Mps(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY,m_nXOffset, m_nYOffset, &nSrcX3,
&nSrcY3); CreateSpecialEffect(MS_DoVanish, nSrcX3, nSrcY3, m_nCurrentMapZ);
//#else
//		if (Npc[m_nLauncher].IsPlayer())
//	     	printf("--����ϰ���:��X:%d,Y:%d--\n",m_nCurrentMapX,m_nCurrentMapY);
            DoVanish(); //�ӵ�����
            return;
    }*/

    if (m_nSubWorldId < 0)
    {
        DoVanish();  // �ӵ�����
        return;
    }

    int nDOffsetX = 0;
    int nDOffsetY = 0;

    if (m_nSpeed > 32)
        m_nSpeed = 32;

    ZAxisMove();  // Z�����

    /*#ifdef _SERVER
            //����м���˺���һ����ӵ����ڴ�͸�ࣩ
            if (m_ulDamageInterval)
            {
                    //�����һ���˺�ʱ�仹δ�����򱾴β��ô�����ײ
                    if (m_ulNextCalDamageTime > (unsigned long)g_SubWorldSet.GetGameTime())
                    {
    //			bCheckColide = FALSE;
    //			bNeverColide = TRUE;
                    }
            }

    #endif*/

    switch (this->m_eMoveKind)  // �ƶ�����
    {
    case MISSLE_MMK_Stand:  //	ԭ��
        /*{
                nDOffsetX = g_Random(32);
                nDOffsetY = g_Random(32);
        }*/
        break;
    case MISSLE_MMK_Parabola:  //	������
    case MISSLE_MMK_Line:      //	ֱ�߷���
    {
        if (this->m_nFollowNpcIdx > 0 && this->m_nFollowNpcIdx < MAX_NPC && this->m_eFollowKind == 2)  // ؤ�＼��
        {  // ���湥��
            // m_nFollowNpcIdx = 2;
            if (m_nTempParam1 > 5)
            {
                int nDistance = 0;
                int nSrcMpsX  = 0;
                int nSrcMpsY  = 0;
                int nDesMpsX  = 0;
                int nDesMpsY  = 0;

                int nDmap = 0;
                GetMpsPos(&nSrcMpsX, &nSrcMpsY);
                Npc[m_nFollowNpcIdx].GetMpsPos(&nDesMpsX, &nDesMpsY, &nDmap);
                nDistance = SubWorld[m_nSubWorldId].GetDistance(nSrcMpsX, nSrcMpsY, nDesMpsX, nDesMpsY);
                if (nDistance > 20)
                {
                    int nXFactor = ((nDesMpsX - nSrcMpsX) << 10) / nDistance;
                    int nYFactor = ((nDesMpsY - nSrcMpsY) << 10) / nDistance;
                    int dx       = nXFactor * m_nSpeed / 1.3;
                    int dy       = nYFactor * m_nSpeed / 1.3;
                    nDOffsetX    = dx;
                    nDOffsetY    = dy;
                    m_nDirIndex = g_GetDirIndex(nSrcMpsX, nSrcMpsY, nDesMpsX, nDesMpsY);  // �ı䷽�� ���ع���
                    m_nDir = g_DirIndex2Dir(m_nDirIndex, MaxMissleDir);
                }
                else
                {
                    ProcessDamage(m_nFollowNpcIdx, m_nRate);  ////���������˺�����
                    DoVanish();                               // �ӵ�����
                    return;
                }
            }
            else
            {
                nDOffsetX = (m_nSpeed * m_nXFactor);
                nDOffsetY = (m_nSpeed * m_nYFactor);
                m_nTempParam1++;
            }
        }
        else
        {
            nDOffsetX = (m_nSpeed * m_nXFactor);
            nDOffsetY = (m_nSpeed * m_nYFactor);
        }
    }
    break;
    case MISSLE_MMK_RollBack:
    {
        if (!m_nTempParam1)
        {
            if (m_nTempParam2 <= m_nCurrentLife)
            {
                m_nXFactor    = -m_nXFactor;
                m_nYFactor    = -m_nYFactor;
                m_nTempParam1 = 1;
                m_nDir        = m_nDir - (MaxMissleDir >> 1);
                if (m_nDir < 0)
                    m_nDir += MaxMissleDir;
            }
        }

        nDOffsetX = (m_nSpeed * m_nXFactor);
        nDOffsetY = (m_nSpeed * m_nYFactor);
    }
    break;
        // ������Ʒ�������������޷��ﵽ�ͷ����˵�ͬ��
    case MISSLE_MMK_Random:  //	������У����ڶ�Ů�׵�Charged Bolt��
    {
    }
    break;
        // ����һ��ʾ˳ʱ�뻹����ʱ��ת��
        // ��������ʾ�̶�ԭ�Ļ���Χ�ķ�����
        // dx = SinA * R
        // dy = Ctg(90-A/2).R = SinA*SinA / (1 + CosA) * R
    case MISSLE_MMK_Circle:  //	���з��У�Χ������ߣ����ڶ��̿͵ļ����� �ٶ�����
    {
        int nPreAngle = m_nAngle - 1;
        if (nPreAngle < 0)
            nPreAngle = MaxMissleDir - 1;
        m_nDir = m_nAngle + (MaxMissleDir >> 2);
        if (m_nDir >= MaxMissleDir)
            m_nDir = m_nDir - MaxMissleDir;
        // int dx = (m_nSpeed + 250)  * (g_DirCos(m_nAngle,MaxMissleDir) - g_DirCos(nPreAngle,MaxMissleDir)) ;
        // int dy = (m_nSpeed + 50)  * (g_DirSin(m_nAngle,MaxMissleDir) - g_DirSin(nPreAngle, MaxMissleDir)) ;
        int dx = (m_nSpeed + 30) * (g_DirCos(m_nAngle, MaxMissleDir) - g_DirCos(nPreAngle, MaxMissleDir));
        int dy = (m_nSpeed + 30) * (g_DirSin(m_nAngle, MaxMissleDir) - g_DirSin(nPreAngle, MaxMissleDir));

        if (m_nParam2)  // ԭ��ת
        {
            nDOffsetX = dx;
            nDOffsetY = dy;
        }
        else  // Χ���ŷ�����ת
        {
            int nOldRegion = m_nRegionId;
            // CurRegion.DecRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);
            m_nRegionId    = Npc[m_nLauncher].m_RegionIndex;
            m_nCurrentMapX = Npc[m_nLauncher].m_MapX;
            m_nCurrentMapY = Npc[m_nLauncher].m_MapY;
            // m_nXOffset		= Npc[m_nLauncher].m_OffX;
            // m_nYOffset		= Npc[m_nLauncher].m_OffY;
            m_nXOffset =
                Npc[m_nLauncher].m_OffX + 30 * (g_DirCos(m_nAngle, MaxMissleDir) + g_DirCos(nPreAngle, MaxMissleDir));
            m_nYOffset =
                Npc[m_nLauncher].m_OffY + 30 * (g_DirSin(m_nAngle, MaxMissleDir) + g_DirSin(nPreAngle, MaxMissleDir));

            // CurRegion.AddRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);

            if (nOldRegion != m_nRegionId)
            {
                // SubWorld[m_nSubWorldId].m_WorldMessage.NewSend(GWM_MISSLE_CHANGE_REGION,nOldRegion,m_nRegionId,m_nMissleId);
                SubWorld[m_nSubWorldId].MissleChangeRegion(nOldRegion, m_nRegionId, m_nMissleId);
            }
            nDOffsetX = dx;
            nDOffsetY = dy;
            // CurRegion.DecRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);
        }

        // ˳ʱ�뻹����ʱ��
        if (m_nParam1)
        {
            m_nAngle++;
            if (m_nAngle >= MaxMissleDir)
                m_nAngle = 0;
        }
        else
        {
            m_nAngle--;
            if (m_nAngle < 0)
                m_nAngle = MaxMissleDir - 1;
        }
    }
    break;
        /*	{
                INT nPreAngle = m_nAngle - 1;
                if (nPreAngle < 0) nPreAngle = MaxMissleDir - 1;
                m_nDir = m_nAngle + (MaxMissleDir / 4);
                if (m_nDir >= MaxMissleDir) m_nDir = m_nDir - MaxMissleDir;
                INT dx = (m_nSpeed + 50)  * (g_DirCos(m_nAngle,MaxMissleDir) - g_DirCos(nPreAngle,MaxMissleDir)) ;
                INT dy = (m_nSpeed + 50)  * (g_DirSin(m_nAngle,MaxMissleDir) - g_DirSin(nPreAngle, MaxMissleDir)) ;

                nDOffsetX = dx;
                nDOffsetY = dy;

                //˳ʱ�뻹����ʱ��
                if (m_nParam1)
                {
                        m_nAngle ++;
                        if (m_nAngle >= MaxMissleDir)
                                m_nAngle = 0;
                }
                else
                {
                        m_nAngle --;
                        if (m_nAngle < 0 )
                                m_nAngle = MaxMissleDir - 1;
                }

                }
                break;*/

        // ����һ��ʾ˳ʱ�뻹����ʱ��ת��
        // ��������ʾ�̶�ԭ�Ļ���Χ�ķ�����
    case MISSLE_MMK_Helix:  //	�����׵������ߣ����ڶ�������Bless
                            //Hammer��
    {
        INT nPreAngle = m_nAngle - 1;
        if (nPreAngle < 0)
        {
            nPreAngle = MaxMissleDir - 1;
        }
        m_nDir = m_nAngle + (MaxMissleDir >> 2);
        if (m_nDir >= MaxMissleDir)
            m_nDir = m_nDir - MaxMissleDir;

        INT dx =
            (m_nSpeed + m_nCurrentLife + 50) * (g_DirCos(m_nAngle, MaxMissleDir) - g_DirCos(nPreAngle, MaxMissleDir));
        INT dy =
            (m_nSpeed + m_nCurrentLife + 50) * (g_DirSin(m_nAngle, MaxMissleDir) - g_DirSin(nPreAngle, MaxMissleDir));

        if (m_nParam2)  // ԭ��ת
        {
            nDOffsetX = dx;
            nDOffsetY = dy;
        }
        else  // Χ���ŷ�����ת
        {
            INT nOldRegion = m_nRegionId;
            //				CurRegion.DecRef(m_nCurrentMapX, m_nCurrentMapY, obj_missile);
            m_nRegionId    = Npc[m_nLauncher].m_RegionIndex;
            m_nCurrentMapX = Npc[m_nLauncher].m_MapX;
            m_nCurrentMapY = Npc[m_nLauncher].m_MapY;
            // m_nXOffset		= Npc[m_nLauncher].m_OffX;
            // m_nYOffset		= Npc[m_nLauncher].m_OffY;
            m_nXOffset =
                Npc[m_nLauncher].m_OffX + 50 * (g_DirCos(m_nAngle, MaxMissleDir) + g_DirCos(nPreAngle, MaxMissleDir));
            m_nYOffset =
                Npc[m_nLauncher].m_OffY + 50 * (g_DirSin(m_nAngle, MaxMissleDir) + g_DirSin(nPreAngle, MaxMissleDir));

            if (nOldRegion != m_nRegionId)
            {
                // SubWorld[m_nSubWorldId].m_WorldMessage.Send(GWM_MISSILE_CHANGE_REGION, nOldRegion, m_nRegionId,
                // m_nMissileIdx);
                SubWorld[m_nSubWorldId].MissleChangeRegion(nOldRegion, m_nRegionId, m_nMissleId);
            }
            nDOffsetX = dx;
            nDOffsetY = dy;
        }

        if (m_nParam1)
        {
            m_nAngle++;
            if (m_nAngle >= MaxMissleDir)
                m_nAngle = 0;
        }
        else
        {
            m_nAngle--;
            if (m_nAngle < 0)
                m_nAngle = MaxMissleDir - 1;
        }
    }
    break;
        /*{
                int nPreAngle;
                if (m_nTempParam1 == 0)
                {
                        m_nParam1 = 0;
                        m_nAngle = Npc[m_nLauncher].m_Dir;
                        m_nTempParam1 = 1;
                }

                nPreAngle = m_nAngle + 1;

                if (nPreAngle > MaxMissleDir)
                {
                        nPreAngle = 0;
                }

                m_nDir = m_nAngle + (MaxMissleDir/4);

                if (m_nDir >= MaxMissleDir) m_nDir = m_nDir - MaxMissleDir;

                int dx = (m_nSpeed + m_nCurrentLife + 275)  * (g_DirCos(m_nAngle,MaxMissleDir) - g_DirCos(nPreAngle,
        MaxMissleDir)) ; int dy = (m_nSpeed + m_nCurrentLife + 275)  * (g_DirSin(m_nAngle,MaxMissleDir) -
        g_DirSin(nPreAngle,MaxMissleDir)) ;


                if (m_nParam2) //ԭ��ת
                {
                        nDOffsetX = dx;
                        nDOffsetY = dy;
                }
                else			// Χ���ŷ�����ת
                {
                        int nOldRegion = m_nRegionId;
                        //CurRegion.DecRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);
                        m_nRegionId		= Npc[m_nLauncher].m_RegionIndex;
                        m_nCurrentMapX	= Npc[m_nLauncher].m_MapX;
                        m_nCurrentMapY	= Npc[m_nLauncher].m_MapY;
                        m_nXOffset		= Npc[m_nLauncher].m_OffX + (35 * g_DirCos(Npc[m_nLauncher].m_Dir,64) +
        100); m_nYOffset		= Npc[m_nLauncher].m_OffY + (35 * g_DirSin(Npc[m_nLauncher].m_Dir,64) + 100);
                        //CurRegion.AddRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);

                        if (nOldRegion != m_nRegionId)
                        {
                                //SubWorld[m_nSubWorldId].m_WorldMessage.NewSend(GWM_MISSLE_CHANGE_REGION,nOldRegion,m_nRegionId,m_nMissleId);
                                SubWorld[m_nSubWorldId].MissleChangeRegion(nOldRegion, m_nRegionId, m_nMissleId);
                        }

                        nDOffsetX = dx;
                        nDOffsetY = dy;
                        //CurRegion.DecRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);
                }

                        m_nAngle ++;
                        if (m_nAngle >= MaxMissleDir)
                                m_nAngle = 0;
        }
        break; */
    case MISSLE_MMK_Follow:  //	����Ŀ�����
    {
        if (this->m_nFollowNpcIdx > 0 && this->m_nFollowNpcIdx < MAX_NPC)
        {
            // m_nFollowNpcIdx = 2;
            if (m_nTempParam1 > 5)
            {
                int nDistance = 0;
                int nSrcMpsX  = 0;
                int nSrcMpsY  = 0;
                int nDesMpsX  = 0;
                int nDesMpsY  = 0;

                int nDmap = 0;

                GetMpsPos(&nSrcMpsX, &nSrcMpsY);
                Npc[m_nFollowNpcIdx].GetMpsPos(&nDesMpsX, &nDesMpsY, &nDmap);

                nDistance = SubWorld[m_nSubWorldId].GetDistance(nSrcMpsX, nSrcMpsY, nDesMpsX, nDesMpsY);
                if (nDistance > 20)
                {
                    int nXFactor = ((nDesMpsX - nSrcMpsX) << 10) / nDistance;
                    int nYFactor = ((nDesMpsY - nSrcMpsY) << 10) / nDistance;
                    int dx       = nXFactor * m_nSpeed / 1.3;
                    int dy       = nYFactor * m_nSpeed / 1.3;
                    nDOffsetX    = dx;
                    nDOffsetY    = dy;
                    m_nDirIndex  = g_GetDirIndex(nSrcMpsX, nSrcMpsY, nDesMpsX, nDesMpsY);
                    m_nDir       = g_DirIndex2Dir(m_nDirIndex, MaxMissleDir);
                }
                else
                {
                    ProcessDamage(m_nFollowNpcIdx, m_nRate);  // ���������˺�����
                    DoVanish();
                    return;
                }
            }
            else
            {
                nDOffsetX = (m_nSpeed * m_nXFactor);
                nDOffsetY = (m_nSpeed * m_nYFactor);
                m_nTempParam1++;
            }
        }
        else
        {
            nDOffsetX = (m_nSpeed * m_nXFactor);
            nDOffsetY = (m_nSpeed * m_nYFactor);
        }
    }
    break;
    case MISSLE_MMK_Motion:  //	��Ҷ�����
    {
    }
    break;

    case MISSLE_MMK_SingleLine:  //	���еĵ�һֱ�߷���ħ��
    {
        // ��һ�������ӵ�����ʽ�ڴ����Լ�������ͬ��������Ϸ�еĻ���ֱ��ħ��
        int x     = m_nXOffset;
        int y     = m_nYOffset;
        int dx    = (m_nSpeed * m_nXFactor);
        int dy    = (m_nSpeed * m_nYFactor);
        nDOffsetX = dx;  //* m_nCurrentLife;
        nDOffsetY = dy;  //* m_nCurrentLife;
    }
    break;
    default:
        break;
    }

    //
    if (CheckBeyondRegion(nDOffsetX, nDOffsetY))
    {
        if (FsCheckCollision() == -1)
        {  // �ӵ������
            if (m_bAutoExplode)
            {                        // �Ƿ��ٴ�	��ײ
                ProcessCollision();  // ������ײ
            }
            int nSrcX4 = 0;
            int nSrcY4 = 0;
            SubWorld[0].NewMap2Mps(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, &nSrcX4,
                                   &nSrcY4);
            CreateSpecialEffect(MS_DoVanish, nSrcX4, nSrcY4, m_nCurrentMapZ);
            DoVanish();
            return;
        }
    }
    else  // ����ӵ����й����н�����һ����Ч��Region���ӵ��Զ�����
    {
        DoVanish();
    }
}
/*!*****************************************************************************
// Function		: KMissle::OnVanish
// Purpose		:
// Return		: void
// Comments		:
// Author		: RomanDou
*****************************************************************************/
void KMissle::OnVanish()
{
    /*if (m_nMissleId>0 && m_nMissleId<MAX_MISSLE)
    {
       //SubWorld[m_nSubWorldId].m_WorldMessage.NewSend(GWM_MISSLE_DEL, m_nMissleId);
            if (Missle[m_nMissleId].m_nSubWorldId>=0 && Missle[m_nMissleId].m_nRegionId >= 0)
                    SubWorld[Missle[m_nMissleId].m_nSubWorldId].m_Region[Missle[m_nMissleId].m_nRegionId].RemoveMissle(m_nMissleId);
            MissleSet.Remove(m_nMissleId);
    } */
}

// �滭�ӵ�
void KMissle::Paint()
{  // �滭õ�廨����
    if (m_nMissleId <= 0)
        return;

    int nSrcX;
    int nSrcY;

    SubWorld[0].NewMap2Mps(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, &nSrcX, &nSrcY);

    if (!m_nZAcceleration)
    {
        m_MissleRes.Draw(m_eMissleStatus, nSrcX, nSrcY, m_nCurrentMapZ, m_nDir, m_nLifeTime - m_nStartLifeTime,
                         m_nCurrentLife - m_nStartLifeTime);
    }
    else
    {
        int nDirIndex = g_GetDirIndex(0, 0, m_nXFactor, m_nYFactor);
        int nDir      = g_DirIndex2Dir(nDirIndex, MaxMissleDir);

        m_MissleRes.Draw(m_eMissleStatus, nSrcX, nSrcY, m_nCurrentMapZ, nDir, m_nLifeTime - m_nStartLifeTime,
                         m_nCurrentLife - m_nStartLifeTime);
    }
    // ���ڿͻ��ˣ�ֱ���ӵ����������Ч��ȫ�����������ֹ��ɾ����!
    if (m_MissleRes.m_bHaveEnd && (m_MissleRes.SpecialMovieIsAllEnd()))  // ��ͷ�Ѿ�û���� ��ɾ��
    {
        if (m_nMissleId > 0 && m_nMissleId < MAX_MISSLE)
        {
            // SubWorld[m_nSubWorldId].m_WorldMessage.NewSend(GWM_MISSLE_DEL, m_nMissleId);
            if (Missle[m_nMissleId].m_nSubWorldId >= 0 && Missle[m_nMissleId].m_nRegionId >= 0)
                SubWorld[Missle[m_nMissleId].m_nSubWorldId].m_Region[Missle[m_nMissleId].m_nRegionId].RemoveMissle(
                    m_nMissleId);

            MissleSet.Remove(m_nMissleId);
        }
    }
}

int KMissle::CheckBeyondRegion(int nDOffsetX, int nDOffsetY)
{
    if (m_nRegionId < 0)
        return FALSE;
    // δ��
    if (nDOffsetX == 0 && nDOffsetY == 0)
        return TRUE;

    if (abs(nDOffsetX) > CellWidth)
    {
        return FALSE;
    }

    if (abs(nDOffsetY) > CellHeight)
    {
        return FALSE;
    }

    int nOldRegion  = m_nRegionId;
    int nNewXOffset = m_nXOffset + nDOffsetX;
    int nNewYOffset = m_nYOffset + nDOffsetY;
    int nNewMapX    = m_nCurrentMapX;
    int nNewMapY    = m_nCurrentMapY;
    int nNewRegion  = m_nRegionId;

    unsigned long nRegionWidth  = RegionWidth;
    unsigned long nRegionHeight = RegionHeight;

    // CCAssert(abs(nNewXOffset) <= CellWidth * 2,"");
    // CCAssert(abs(nNewYOffset) <= CellHeight * 2,"");
    if (abs(nNewXOffset) > CellWidth * 2)
    {
        return FALSE;
    }

    if (abs(nNewYOffset) > CellHeight * 2)
    {
        return FALSE;
    }

    //	����NPC��������
    //	CELLWIDTH��CELLHEIGHT��OffX��OffY���ǷŴ���1024��

    if (nNewXOffset < 0)
    {
        nNewMapX--;
        nNewXOffset += CellWidth;
    }
    else if (nNewXOffset > CellWidth)
    {
        nNewMapX++;
        nNewXOffset -= CellWidth;
    }

    if (nNewYOffset < 0)
    {
        nNewMapY--;
        nNewYOffset += CellHeight;
    }
    else if (nNewYOffset > CellHeight)
    {
        nNewMapY++;
        nNewYOffset -= CellHeight;
    }

    if (nNewMapX < 0)
    {
        nNewRegion = LeftRegion(m_nRegionId);
        nNewMapX += nRegionWidth;
    }
    else if ((unsigned long)nNewMapX >= nRegionWidth)
    {
        nNewRegion = RightRegion(m_nRegionId);
        nNewMapX -= nRegionWidth;
    }

    if (nNewRegion < 0)
    {
        return FALSE;
    }

    if (nNewMapY < 0)
    {
        nNewRegion = UpRegion(nNewRegion);
        nNewMapY += nRegionHeight;
    }
    else if (nNewMapY >= RegionHeight)
    {
        nNewRegion = DownRegion(nNewRegion);
        nNewMapY -= nRegionHeight;
    }

    // ��һ��λ��Ϊ���Ϸ�λ�ã�������
    if (nNewRegion < 0)
    {
        return FALSE;
    }
    else
    {
        // CurRegion.DecRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);
        // CCAssert(m_nCurrentMapX >= 0  &&  m_nCurrentMapY >= 0,"");
        m_nRegionId    = nNewRegion;
        m_nCurrentMapX = nNewMapX;
        m_nCurrentMapY = nNewMapY;
        m_nXOffset     = nNewXOffset;
        m_nYOffset     = nNewYOffset;
        // CurRegion.AddRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);

        if (nOldRegion != m_nRegionId)
        {
            // SubWorld[m_nSubWorldId].m_WorldMessage.NewSend(GWM_MISSLE_CHANGE_REGION, nOldRegion, m_nRegionId,
            // m_nMissleId);
            SubWorld[m_nSubWorldId].MissleChangeRegion(nOldRegion, m_nRegionId, m_nMissleId);
        }
        // CurRegion.DecRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);
    }
    return TRUE;
}

KMissle& KMissle::operator=(KMissle& Missle)
{
    Missle.m_nTempParam1       = 0;
    Missle.m_nTempParam2       = 0;
    Missle.m_nDesMapX          = 0;
    Missle.m_nDesMapY          = 0;
    Missle.m_nDesRegion        = 0;
    Missle.m_bNeedReclaim      = FALSE;
    Missle.m_nFirstReclaimTime = 0;
    Missle.m_nEndReclaimTime   = 0;
    // memset(Missle.m_NeedReclaimPos, 0, sizeof(m_NeedReclaimPos));

    Missle.m_bCanSlow         = m_bCanSlow;
    Missle.m_bCollideEvent    = m_bCollideEvent;
    Missle.m_bCollideFriend   = m_bCollideFriend;
    Missle.m_bCollideVanish   = m_bCollideVanish;
    Missle.m_bRangeDamage     = m_bRangeDamage;
    Missle.m_eFollowKind      = m_eFollowKind;
    Missle.m_eMoveKind        = m_eMoveKind;
    Missle.m_nAction          = m_nAction;
    Missle.m_nAngle           = m_nAngle;
    Missle.m_nCollideRange    = m_nCollideRange;
    Missle.m_nCurrentLife     = 0;
    Missle.m_nDamageRange     = m_nDamageRange;
    Missle.m_nHeight          = m_nHeight;
    Missle.m_nLifeTime        = m_nLifeTime;
    Missle.m_nSpeed           = m_nSpeed;
    Missle.m_nParam1          = m_nParam1;
    Missle.m_nParam2          = m_nParam2;
    Missle.m_nParam3          = m_nParam3;
    Missle.m_nCurrentMapZ     = m_nHeight >> 10;
    Missle.m_bFlyEvent        = m_bFlyEvent;
    Missle.m_nFlyEventTime    = m_nFlyEventTime;
    Missle.m_nZAcceleration   = m_nZAcceleration;
    Missle.m_nHeightSpeed     = m_nHeightSpeed;
    Missle.m_bAutoExplode     = m_bAutoExplode;
    Missle.m_ulDamageInterval = m_ulDamageInterval;
    // strcpy(Missle.m_szMissleName	,	m_szMissleName);
    strcpy(Missle.m_szMissleName, m_szMissleName);
    Missle.m_bMultiShow            = m_bMultiShow;
    Missle.m_MissleRes.m_bLoopAnim = m_MissleRes.m_bLoopAnim;
    Missle.m_MissleRes.m_bHaveEnd  = FALSE;
    Missle.m_btRedLum              = m_btRedLum;
    Missle.m_btGreenLum            = m_btGreenLum;
    Missle.m_btBlueLum             = m_btBlueLum;
    Missle.m_usLightRadius         = m_usLightRadius;
    int nOffset                    = 0;

    // �������ͬ���ӵ������Բ�ͬ��ʽ��ʾʱ�����������
    if (m_bMultiShow)
    {
        if (g_Random(2) == 0)
        {
            nOffset = 0;
        }
        else
            nOffset = MAX_MISSLE_STATUS;
    }

    for (int t = 0; t < MAX_MISSLE_STATUS; ++t)
    {
        strcpy(Missle.m_MissleRes.m_MissleRes[t].AnimFileName, m_MissleRes.m_MissleRes[t + nOffset].AnimFileName);

        Missle.m_MissleRes.m_MissleRes[t].nTotalFrame = m_MissleRes.m_MissleRes[t + nOffset].nTotalFrame;
        Missle.m_MissleRes.m_MissleRes[t].nDir        = m_MissleRes.m_MissleRes[t + nOffset].nDir;
        Missle.m_MissleRes.m_MissleRes[t].nInterval   = m_MissleRes.m_MissleRes[t + nOffset].nInterval;

        strcpy(Missle.m_MissleRes.m_MissleRes[t].SndFileName, m_MissleRes.m_MissleRes[t + nOffset].SndFileName);
    }
    Missle.m_MissleRes.m_bSubLoop  = m_MissleRes.m_bSubLoop;
    Missle.m_MissleRes.m_nSubStart = m_MissleRes.m_nSubStart;
    Missle.m_MissleRes.m_nSubStop  = m_MissleRes.m_nSubStop;

    return (Missle);
}

/*!*****************************************************************************
// Function		: KMissle::ProcessDamage
// Purpose		:
// Return		: int
// Argumant		: int nNpcId
// Comments		:
// Author		: RomanDou   ���޳������ӵ��˺�
�ӵ�����--�˺���������
*****************************************************************************/
int KMissle::ProcessDamage(int nNpcId, int nRata)
{
    return FALSE;
}
// �ӵ�����
void KMissle::DoVanish()
{
    if (m_eMissleStatus == MS_DoVanish)
        return;

    m_MissleRes.m_bHaveEnd = TRUE;  // ���ÿͻ��� �Ѿ��������
    m_nCollideOrVanishTime = m_nCurrentLife;
    if (m_bVanishedEvent)
    {  // �ӵ�����ʱ �ͷŵĽ���ʱ�ļ���
        // CCAssert(m_nSkillId < MAX_SKILL && m_nLevel < MAX_SKILLLEVEL,"");
        KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_nSkillId, m_nLevel);
        if (pOrdinSkill)
        {
            pOrdinSkill->Vanish(this);
        }
    }
    m_eMissleStatus = MS_DoVanish;
    // SubWorld[0].m_WorldMessage.Send(GWM_MISSLE_DEL, m_nMissleId);
    if (m_nMissleId > 0 && m_nMissleId < MAX_MISSLE)
    {
        // SubWorld[m_nSubWorldId].m_WorldMessage.NewSend(GWM_MISSLE_DEL, m_nMissleId);
        if (Missle[m_nMissleId].m_nSubWorldId >= 0 && Missle[m_nMissleId].m_nRegionId >= 0)
        {
            SubWorld[Missle[m_nMissleId].m_nSubWorldId].m_Region[Missle[m_nMissleId].m_nRegionId].RemoveMissle(
                m_nMissleId);
        }
        MissleSet.Remove(m_nMissleId);
    }

    if (m_nRegionId < 0)
    {
        //_ASSERT(0);
        m_bRemoving = TRUE;
        return;
    }
}

void KMissle::DoCollision()
{

    if (m_eMissleStatus == MS_DoCollision)
        return;

    int nSrcX = 0;
    int nSrcY = 0;
    SubWorld[0].NewMap2Mps(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, &nSrcX, &nSrcY);

    if (m_bCollideEvent)
    {  // ��ײ����
        // CCAssert(m_nSkillId < MAX_SKILL && m_nLevel < MAX_SKILLLEVEL,"");
        KSkill* pOrdinSkill = (KSkill*)g_SkillManager.GetSkill(m_nSkillId, m_nLevel);
        if (pOrdinSkill)
        {
            pOrdinSkill->Collidsion(this);
        }
    }
    // ����ײ���Ч��
    if (m_MissleRes.SpecialMovieIsAllEnd())  // ����������� �ʹ�����ײЧ��
        CreateSpecialEffect(MS_DoCollision, nSrcX, nSrcY, m_nCurrentMapZ);

    if (m_bCollideVanish)
    {  // ��ײ������
        m_MissleRes.m_bHaveEnd = TRUE;
        int nSrcX5             = 0;
        int nSrcY5             = 0;
        SubWorld[0].NewMap2Mps(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, &nSrcX5, &nSrcY5);
        CreateSpecialEffect(MS_DoVanish, nSrcX5, nSrcY5, m_nCurrentMapZ);
        DoVanish();
    }
    else
    {
        // ����ײ���Ч��
        // if (m_MissleRes.SpecialMovieIsAllEnd())//����������� �ʹ�����ײЧ��
        //	CreateSpecialEffect(MS_DoCollision, nSrcX, nSrcY, m_nCurrentMapZ);

        m_eMissleStatus = MS_DoFly;  // MS_DoCollision;//
    }
}

void KMissle::DoFly()
{
    if (m_eMissleStatus == MS_DoFly)
        return;
    // ��ʼ����ͼ
    m_eMissleStatus = MS_DoFly;
}

int KMissle::GetOffsetAxis(int nSubWorld,
                           int nSrcRegionId,
                           int nSrcMapX,
                           int nSrcMapY,
                           int nOffsetMapX,
                           int nOffsetMapY,
                           int& nDesRegionId,
                           int& nDesMapX,
                           int& nDesMapY)
{
    if (nSubWorld < 0 || nSubWorld >= MAX_SUBWORLD)
        return 0;
    if (nSrcRegionId < 0 || nSrcRegionId > SubWorld[nSubWorld].m_nTotalRegion)
        return false;
    nDesRegionId = -1;
    // ȷ��Ŀ�����ʵ�ʵ�REGION������ȷ��
    nDesMapX = nSrcMapX + nOffsetMapX;
    nDesMapY = nSrcMapY + nOffsetMapY;

    if (nSrcRegionId < 0)
        return FALSE;

    int nSearchRegion = nSrcRegionId;

    if (nDesMapX < 0)
    {
        nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[2];
        nDesMapX += SubWorld[nSubWorld].m_nRegionWidth;
    }
    else if (nDesMapX >= SubWorld[nSubWorld].m_nRegionWidth)
    {
        nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[6];
        nDesMapX -= SubWorld[nSubWorld].m_nRegionWidth;
    }

    if (nSearchRegion < 0 || nSearchRegion > SubWorld[nSubWorld].m_nTotalRegion)
        return FALSE;
    //-------------------
    if (nDesMapY < 0)
    {
        nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[4];
        nDesMapY += SubWorld[nSubWorld].m_nRegionHeight;
    }
    else if (nDesMapY >= SubWorld[nSubWorld].m_nRegionHeight)
    {
        nSearchRegion = SubWorld[nSubWorld].m_Region[nSearchRegion].m_nConnectRegion[0];
        nDesMapY -= SubWorld[nSubWorld].m_nRegionHeight;
    }

    if (nSearchRegion < 0 || nSearchRegion > SubWorld[nSubWorld].m_nTotalRegion)
        return FALSE;

    nDesRegionId = nSearchRegion;

    return TRUE;
    // ��REGION��NPC�б��в�������������NPC
    // int nNpcIdx = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nDesMapX, nDesMapY, nLauncherIdx,
    // relation_all);
}

/*!*****************************************************************************
// Function		: KMissle::ProcessCollision
// Purpose		:
// Return		: int
// Argumant		: int nLauncherIdx
// Argumant		: int nRegionId
// Argumant		: int nMapX
// Argumant		: int nMapY
// Argumant		: int nRange
// Argumant		: MISSLE_RELATION eRelation
// Comments		:
// Author		: RomanDou  �ӵ���ײ�˺�����
*****************************************************************************/
int KMissle::ProcessCollision(int nLauncherIdx,
                              int nRegionId,
                              int nMapX,
                              int nMapY,
                              int nRange,
                              int eRelation,
                              int mNpcIdx)
{

    // #ifdef TOOLVERSION
    //	return 0;
    // #endif
    if (nLauncherIdx <= 0 || nLauncherIdx >= MAX_NPC)
        return 0;

    if (nRange <= 0)
        return 0;

    int nRangeX = nRange;  // һ��������	/2
    int nRangeY = nRangeX;

    int nSubWorld = Npc[nLauncherIdx].m_SubWorldIndex;

    // CCAssert(Npc[nLauncherIdx].m_SubWorldIndex >= 0,"");
    // CCAssert(nRegionId >= 0,"");
    if (nSubWorld < 0 || nSubWorld >= MAX_SUBWORLD || nRegionId < 0)
        return 0;

    int nRegion = nRegionId;
    int nRet    = 0;
    int nRMx, nRMy, nSearchRegion;

    if (mNpcIdx > 0 && mNpcIdx < MAX_NPC)
    {  // ָ��NPC ������  �˺���Χ Ϊ1�� ֱ��������
        if (NpcSet.GetRelation(Npc[nLauncherIdx].m_Index, mNpcIdx) == eRelation)
        {  // ��ϵ��ͬ�� ��ִ�й���  m_RegionIndex
            int nSrcX = 0;
            int nSrcY = 0;
            SubWorld[0].NewMap2Mps(Npc[mNpcIdx].m_RegionIndex, Npc[mNpcIdx].m_MapX, Npc[mNpcIdx].m_MapY,
                                   Npc[mNpcIdx].m_OffX, Npc[mNpcIdx].m_OffY, &nSrcX, &nSrcY);

            if (m_bFollowNpcWhenCollid)  // �Ƿ����
                CreateSpecialEffect(MS_DoCollision, nSrcX, nSrcY, m_nCurrentMapZ, mNpcIdx);
            else
                CreateSpecialEffect(MS_DoCollision, nSrcX, nSrcY, m_nCurrentMapZ);

            // m_eMissleStatus = MS_DoCollision;
        }

        return TRUE;
    }

    // ��鷶Χ�ڵĸ������NPC
    for (int i = -nRangeX; i <= nRangeX; ++i)
    {  // �˺��ķ�Χ
        for (int j = -nRangeY; j <= nRangeY; ++j)
        {
            // ȥ���߽Ǽ������ӣ���֤��Ұ����Բ��
            if ((i * i + j * j) > nRangeX * nRangeX)
                continue;

            if (!GetOffsetAxis(nSubWorld, nRegionId, nMapX, nMapY, i, j, nSearchRegion, nRMx, nRMy))
                continue;

            //_ASSERT(nSearchRegion >= 0);
            if (nSearchRegion < 0)
                continue;

            int nNpcIdx = 0;
            // ��REGION��NPC�б��в�������������NPC
            nNpcIdx = SubWorld[nSubWorld].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, nLauncherIdx, eRelation);

            if (nNpcIdx > 0 && nNpcIdx < MAX_NPC)
            {
                nRet++;  // ����NPC�ĸ���
                int nSrcX = 0;
                int nSrcY = 0;
                SubWorld[0].NewMap2Mps(nSearchRegion, Npc[nNpcIdx].m_MapX, Npc[nNpcIdx].m_MapY, Npc[nNpcIdx].m_OffX,
                                       Npc[nNpcIdx].m_OffY, &nSrcX, &nSrcY);

                if (m_bFollowNpcWhenCollid)  // �Ƿ����
                    CreateSpecialEffect(MS_DoCollision, nSrcX, nSrcY, m_nCurrentMapZ, nNpcIdx);
                else
                    CreateSpecialEffect(MS_DoCollision, nSrcX, nSrcY, m_nCurrentMapZ);

                // m_eMissleStatus = MS_DoCollision;
                if (m_nHitCount > 0 && nRet >= m_nHitCount)  // ����趨�� ����������
                    return nRet;
            }
        }
    }

    return nRet;
}

int KMissle::ProcessCollision()
{
    /// #ifdef TOOLVERSION
    // return 0;
    // #endif
    if (m_bClientSend)
        return 0;  // �ǿͻ��˼��� �����˺�

    return ProcessCollision(m_nLauncher, m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nDamageRange, m_eRelation);
}

// ����ĳ����Ч���
#define MISSLE_Y_OFFSET 1
int KMissle::CreateSpecialEffect(eMissleStatus eStatus, int nPX, int nPY, int nPZ, int nNpcIndex)
{
    if (!m_MissleRes.m_MissleRes[eStatus].AnimFileName[0])
    {
        // if (m_MissleRes.m_MissleRes[eStatus+MAX_MISSLE_STATUS].AnimFileName[0])
        //	sprintf(m_MissleRes.m_MissleRes[eStatus].AnimFileName,m_MissleRes.m_MissleRes[eStatus+MAX_MISSLE_STATUS].AnimFileName);
        // else
        return FALSE;
    }

    KSkillSpecialNode* pNode = NULL;
    // ͬһ���ӵ������м�����ըЧ����һ��Npc����
    if (nNpcIndex > 0)
    {  // ����Ǹ����NPC
        pNode = (KSkillSpecialNode*)m_MissleRes.m_SkillSpecialList.GetHead();
        while (pNode)
        {
            if (pNode->m_pSkillSpecial->m_dwMatchID == Npc[nNpcIndex].m_dwID)
                return FALSE;

            pNode = (KSkillSpecialNode*)pNode->GetNext();
        }
    }

    m_MissleRes.PlaySound(eStatus, nPX, nPY, 0);
    // if pSkillSpecial->m_pMissleRes->nDir

    pNode = new KSkillSpecialNode;

    if (pNode == NULL)
        return FALSE;

    KSkillSpecial* pSkillSpecial = new KSkillSpecial;

    if (pSkillSpecial == NULL)
    {
        delete pNode;
        pNode = NULL;
        return FALSE;
    }

    /*if (eStatus == MS_DoCollision && nNpcIndex > 0)
    {
            // ���X������Ϊ��ɫ���Ϊ40
            if (m_nHurtRandX > 0)
                    pSkillSpecial->m_nXOffset	= ((INT)KSysService::Engine_Random(m_nHurtRandX) - m_nHurtRandX / 2) *
    40 / 100;

            // ���Y������Ϊһ���ɫ�߶�Ϊ100
            INT nNpcStature = Npc[nNpcIndex].m_DataRes.GetStature();
            if (m_nHurtRandY > 0 && nNpcStature > 0)
                    pSkillSpecial->m_nYOffset	= ((INT)KSysService::Engine_Random(m_nHurtRandY) - m_nHurtRandY / 2 +
    50) * nNpcStature / 100 - 50;
    }*/

    pNode->m_pSkillSpecial = pSkillSpecial;

    int nSrcX = nPX;
    int nSrcY = nPY;

    pSkillSpecial->m_nMissleId = m_nMissleId;
    pSkillSpecial->m_nPX       = nSrcX;
    pSkillSpecial->m_nPY       = nSrcY - 5;  // MISSLE_Y_OFFSET;
    pSkillSpecial->m_nPZ       = nPZ;
    if (nNpcIndex > 0)
    {
        pSkillSpecial->m_nNpcIndex = nNpcIndex;
        pSkillSpecial->m_dwMatchID = Npc[nNpcIndex].m_dwID;
    }
    else
    {
        pSkillSpecial->m_nNpcIndex = 0;
        pSkillSpecial->m_dwMatchID = 0;
    }

    pSkillSpecial->m_pMissleRes = &m_MissleRes.m_MissleRes[eStatus];
    pSkillSpecial->m_eStatus    = eStatus;
    pSkillSpecial->m_nBeginTime = g_SubWorldSet.GetGameTime();
    pSkillSpecial->m_nEndTime =
        g_SubWorldSet.GetGameTime() + (pSkillSpecial->m_pMissleRes->nInterval *
                                       pSkillSpecial->m_pMissleRes->nTotalFrame / pSkillSpecial->m_pMissleRes->nDir);
    pSkillSpecial->m_nCurDir = g_DirIndex2Dir(m_nDirIndex, m_MissleRes.m_MissleRes[eStatus].nDir);
    pSkillSpecial->Init();
    m_MissleRes.m_SkillSpecialList.AddTail(pNode);

    return TRUE;
}

int KMissle::CreateMissleForShow(char* szMovie, char* szFormat, char* szSound, TMissleForShow* pShowParam)
{
    if (!pShowParam || !szMovie || !szMovie[0])
        return FALSE;
    int nPX   = 0;
    int nPY   = 0;
    int nMmap = 0;
    int nPZ   = 0;

    if (pShowParam->nNpcIndex > 0)
    {
        Npc[pShowParam->nNpcIndex].GetMpsPos(&nPX, &nPY, &nMmap);
    }
    else
    {
        nPX = pShowParam->nPX;
        nPY = pShowParam->nPY;
    }

    int nSubWorldId  = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SubWorldIndex;
    int nMissleIndex = MissleSet.Add(nSubWorldId, nPX, nPY);
    if (nMissleIndex < 0)
        return FALSE;

    Missle[nMissleIndex].m_nDir      = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Dir;
    Missle[nMissleIndex].m_nDirIndex = g_Dir2DirIndex(Missle[nMissleIndex].m_nDir, MaxMissleDir);

    Missle[nMissleIndex].m_nFollowNpcIdx = 0;

    Missle[nMissleIndex].m_dwBornTime   = SubWorld[nSubWorldId].m_dwCurrentTime;
    Missle[nMissleIndex].m_nSubWorldId  = nSubWorldId;
    Missle[nMissleIndex].m_nLauncher    = pShowParam->nLauncherIndex;
    Missle[nMissleIndex].m_dwLauncherId = Npc[pShowParam->nLauncherIndex].m_dwID;

    Missle[nMissleIndex].m_nParentMissleIndex = 0;
    Missle[nMissleIndex].m_nShangBei          = 0;  // �˺��ı���
    Missle[nMissleIndex].m_nEnChance          = 0;
    Missle[nMissleIndex].m_nSkillId           = 0;
    Missle[nMissleIndex].m_nStartLifeTime     = 0;
    Missle[nMissleIndex].m_nLifeTime          = 1;
    Missle[nMissleIndex].m_nRefPX             = 0;
    Missle[nMissleIndex].m_nRefPY             = 0;
    Missle[nMissleIndex].m_MissleRes.Clear();

    Missle[nMissleIndex].m_MissleRes.LoadResource(MS_DoWait, szMovie, szSound);
    char* pcszTemp                                                      = szFormat;
    Missle[nMissleIndex].m_MissleRes.m_MissleRes[MS_DoWait].nTotalFrame = KSG_StringGetInt(&pcszTemp, 100);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    Missle[nMissleIndex].m_MissleRes.m_MissleRes[MS_DoWait].nDir = KSG_StringGetInt(&pcszTemp, 16);
    KSG_StringSkipSymbol(&pcszTemp, ',');
    Missle[nMissleIndex].m_MissleRes.m_MissleRes[MS_DoWait].nInterval = KSG_StringGetInt(&pcszTemp, 1);

    Missle[nMissleIndex].CreateSpecialEffect(MS_DoWait, nPX, nPY, nPZ, pShowParam->nNpcIndex);
    return TRUE;
}

void KMissle::GetLightInfo(KLightInfo* pLightInfo)
{
    if (!pLightInfo)
    {
        return;
    }

    int nPX, nPY, nPZ;
    GetMpsPos(&nPX, &nPY);
    nPZ = m_nCurrentMapZ;

    pLightInfo->oPosition.nX = nPX;
    pLightInfo->oPosition.nY = nPY;
    pLightInfo->oPosition.nZ = nPZ;
    pLightInfo->dwColor      = 0xff000000 | m_btRedLum << 16 | m_btGreenLum << 8 | m_btBlueLum;
    pLightInfo->nRadius      = m_usLightRadius;
}

void KMissle::DoWait()
{
    //	if (m_eMissleStatus == MS_DoWait) return;
    m_eMissleStatus = MS_DoWait;
    int nSrcX       = 0;
    int nSrcY       = 0;
    SubWorld[0].NewMap2Mps(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, &nSrcX, &nSrcY);
    CreateSpecialEffect(MS_DoWait, nSrcX, nSrcY, m_nCurrentMapZ);
}
// ���ӵ�����fly״̬ʱ����Ҫ��������䶯
int KMissle::PrePareFly()
{
    if (m_eMoveKind == MISSLE_MMK_RollBack)
        m_nTempParam2 = m_nStartLifeTime + (m_nLifeTime - m_nStartLifeTime) / 2;

    // �Ƿ���淢���ߵ��ƶ����жϣ���ʽħ��3�д��ͷ���
    if (m_nInteruptTypeWhenMove)
    {
        int nPX, nPY, nMp;
        Npc[m_nLauncher].GetMpsPos(&nPX, &nPY, &nMp);
        if (nPX != m_nLauncherSrcPX || nPY != m_nLauncherSrcPY)
        {
            return false;
        }
    }

    // �ӵ�λ����Ҫ����Ϊ���ʵ���λ�ã��ӵ��ĳ���������ĳ������λ���ڲ��ϱ仯������Ϊ�����
    if (m_bHeelAtParent)
    {
        int nNewPX = 0;
        int nNewPY = 0;

        if (m_nParentMissleIndex)  // �ο���Ϊĸ�ӵ�
        {
            if (Missle[m_nParentMissleIndex].m_dwLauncherId != m_dwLauncherId)
            {
                return false;
            }
            else
            {
                int nParentPX, nParentPY;
                int nSrcPX, nSrcPY;
                Missle[m_nParentMissleIndex].GetMpsPos(&nParentPX, &nParentPY);
                GetMpsPos(&nSrcPX, &nSrcPY);
                nNewPX = nSrcPX + (nParentPX - m_nRefPX);
                nNewPY = nSrcPY + (nParentPY - m_nRefPY);
            }
        }
        else
        // �ο���Ϊ������
        {
            //_ASSERT(m_nLauncher > 0);

            if (m_nLauncher <= 0)
                return false;

            int nParentPX, nParentPY, nPmap;
            int nSrcPX, nSrcPY;

            Npc[m_nLauncher].GetMpsPos(&nParentPX, &nParentPY, &nPmap);
            GetMpsPos(&nSrcPX, &nSrcPY);

            nNewPX = nSrcPX + (nParentPX - m_nRefPX);
            nNewPY = nSrcPY + (nParentPY - m_nRefPY);
        }

        int nOldRegion = m_nRegionId;

        // CurRegion.DecRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);
        SubWorld[m_nSubWorldId].Mps2Map(nNewPX, nNewPY, &m_nRegionId, &m_nCurrentMapX, &m_nCurrentMapY, &m_nXOffset,
                                        &m_nYOffset);
        // CurRegion.AddRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);
        if (m_nSubWorldId < 0)
            return false;

        if (nOldRegion != m_nRegionId)
        {
            // SubWorld[m_nSubWorldId].m_WorldMessage.NewSend(GWM_MISSLE_CHANGE_REGION, nOldRegion, m_nRegionId,
            // m_nMissleId);
            SubWorld[m_nSubWorldId].MissleChangeRegion(nOldRegion, m_nRegionId, m_nMissleId);
        }
        // CurRegion.DecRef(m_nCurrentMapX, m_nCurrentMapY, obj_missle);
    }

    return true;
}

int KMissle::CheckNearestCollision()
{
    if (m_nLauncher < 0 && m_nLauncher >= MAX_NPC)
        return 0;

    if (m_nSubWorldId < 0 && m_nSubWorldId >= MAX_SUBWORLD)
        return 0;

    int nSearchRegion = 0;
    int nRMx          = 0;
    int nRMy          = 0;
    int bCollision    = TRUE;
    int nNpcIdx       = 0;
    int nDX           = 0;
    int nDY           = 0;
    int nNpcOffsetX   = 0;
    int nNpcOffsetY   = 0;
    int nAbsX         = 0;
    int nAbsY         = 0;
    int nCellWidth    = CellWidth;
    int nCellHeight   = CellHeight;

    if (nCellWidth <= 0 && nCellHeight <= 0)
        return 0;

    /*for (int i = -1; i <= 1; ++i)
            for (int j = -1; j <= 1; ++j)
            {
                    if
       (!KMissle::GetOffsetAxis(m_nSubWorldId,m_nRegionId,m_nCurrentMapX,m_nCurrentMapY,i,j,nSearchRegion,nRMx,nRMy))
                            continue;
                    //_ASSERT(nSearchRegion >= 0);
                    if (nSearchRegion<0)
                            continue;

                    int i;

                    switch (nRMx)
                    {
                    case 0:
                            {
                                    for (i=0;i<=nRMx+2;i++)
                                    {//nRMx
                                            nNpcIdx = SubWorld[m_nSubWorldId].m_Region[nSearchRegion].FindNpc(i, nRMy,
       m_nLauncher, m_eRelation); if  (nNpcIdx>0) break;
                                    }
                            }
                            break;
                    case 15:
                            {
                                    for (i=nRMx-2;i<=nRMx;i++)
                                    {//nRMx
                                            nNpcIdx = SubWorld[m_nSubWorldId].m_Region[nSearchRegion].FindNpc(i, nRMy,
       m_nLauncher, m_eRelation); if  (nNpcIdx>0) break;
                                    }
                            }
                            break;
                    default:
                            {
                                    if (nRMx>0 && nRMx<15)
                                    {
                                            for ( i=nRMx-1;i<=nRMx+1;i++)
                                            {//nRMx
                                                    nNpcIdx = SubWorld[m_nSubWorldId].m_Region[nSearchRegion].FindNpc(i,
       nRMy, m_nLauncher, m_eRelation); if  (nNpcIdx>0) break;
                                            }
                                    }
                                    else
                  nNpcIdx = SubWorld[m_nSubWorldId].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nLauncher,
       m_eRelation);
                            }
                            break;
                    }

                    if (nNpcIdx > 0 && nNpcIdx<MAX_NPC)
                            return nNpcIdx;
            }
            return 0;*/
    //--------------------------------------------------
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
        {
            if (!KMissle::GetOffsetAxis(m_nSubWorldId, m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, i, j, nSearchRegion,
                                        nRMx, nRMy))
                continue;

            //_ASSERT(nSearchRegion >= 0);
            if (nSearchRegion < 0)
                continue;

            nNpcIdx = SubWorld[m_nSubWorldId].m_Region[nSearchRegion].FindNpc(nRMx, nRMy, m_nLauncher, m_eRelation);

            if (nNpcIdx > 0 && nNpcIdx < MAX_NPC)
            {
                bCollision  = TRUE;
                nDX         = m_nCurrentMapX - Npc[nNpcIdx].m_MapX;
                nDY         = m_nCurrentMapY - Npc[nNpcIdx].m_MapY;
                nNpcOffsetX = Npc[nNpcIdx].m_OffX;
                nNpcOffsetY = Npc[nNpcIdx].m_OffY;
                nAbsX       = abs(nDX);
                nAbsY       = abs(nDY);

                if (nAbsX)
                {
                    if (nDX < 0)
                    {
                        if (nCellWidth - m_nXOffset + nNpcOffsetX > nCellWidth)
                        {
                            bCollision = FALSE;
                            goto CheckCollision;
                        }
                    }
                    else if (nDX > 0)
                    {
                        if (nCellWidth - nNpcOffsetX + m_nXOffset > nCellWidth)
                        {
                            bCollision = FALSE;
                            goto CheckCollision;
                        }
                    }
                }

                if (nAbsY)
                {
                    if (nDY < 0)
                    {
                        if (nCellHeight - m_nYOffset + nNpcOffsetY > nCellHeight)
                        {
                            bCollision = FALSE;
                            goto CheckCollision;
                        }
                    }
                    else if (nDY > 0)
                    {
                        if (nCellHeight - nNpcOffsetY + m_nYOffset > nCellHeight)
                        {
                            bCollision = FALSE;
                            goto CheckCollision;
                        }
                    }
                }

            CheckCollision:
                if (bCollision)
                    return nNpcIdx;
            }
        }

    return 0;
}

void KMissle::GetMpsPos(int* pPosX, int* pPosY)
{
    SubWorld[m_nSubWorldId].NewMap2Mps(m_nRegionId, m_nCurrentMapX, m_nCurrentMapY, m_nXOffset, m_nYOffset, pPosX,
                                       pPosY);
};

int KMissle::Remove()
{
    if (m_nMissleId < 0)
        return FALSE;
    // SubWorld[m_nSubWorldId].m_WorldMessage.Send(GWM_MISSILE_DEL, m_nMissileIdx);
    SubWorld[Missle[m_nMissleId].m_nSubWorldId].m_Region[Missle[m_nMissleId].m_nRegionId].RemoveMissle(m_nMissleId);
    MissleSet.Remove(m_nMissleId);

    return TRUE;
}
