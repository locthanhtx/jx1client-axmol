//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KNpcResNode.cpp
// Date:	2002.01.06
// Code:	�߳�����
// Desc:	Obj Class
//---------------------------------------------------------------------------
#include "KCore.h"
#include "engine/KSG_StringProcess.h"
#include "KSprControl.h"
#include "engine/KTabFile.h"
#include "engine/KIniFile.h"
#include "engine/KFilePath.h"
// #include	"KStrBase.h"
#include "KNpcResNode.h"
#include "CoreUseNameDef.h"

#define MAX_SORT_DIR 16  // 16

KNpcResNode::KNpcResNode()
{
    m_nNpcKind     = NPC_RES_NORMAL;
    m_nPartNum     = 1;
    m_szNpcName[0] = 0;
    m_szResPath[0] = 0;
    memset(m_szSoundName, 0, sizeof(m_szSoundName));
}

KNpcResNode::~KNpcResNode() {}

int KNpcResNode::Init(char* lpszNpcName, CActionName* cActionName, CActionName* cNpcAction)
{
    if (lpszNpcName == NULL || lpszNpcName[0] == 0)
        return FALSE;

    KTabFile KindFile;
    int i, j, k, nFindNo;
    char szBuffer[FILE_NAME_LENGTH], szTemp[FILE_NAME_LENGTH], szBuf[FILE_NAME_LENGTH];
    const char* pcszTemp = NULL;
    strcpy(m_szNpcName, lpszNpcName);
    // �����ļ� ��������.txt
    //	g_SetFilePath(RES_INI_FILE_PATH);
    if (!KindFile.Load(NPC_RES_KIND_FILE_NAME))
    {
        messageBox("KindFile is Error", "KindFile");
        return FALSE;
    }

    nFindNo = KindFile.FindRow(lpszNpcName);  // ����ĳ��
    if (nFindNo < 0)
    {
        messageBox(lpszNpcName, "KNpcResNode::Init");
        return FALSE;
    }

    KindFile.GetString(nFindNo, KIND_NAME_SECT, "", szBuffer, sizeof(szBuffer));

    // �ж�npc����
    if (strcmp(szBuffer, KIND_NAME_SPECIAL) == 0)
        m_nNpcKind = NPC_RES_SPECIAL;  // ��������
    else
        m_nNpcKind = NPC_RES_NORMAL;  // ��ͨ����
    // �õ���Դ�ļ�·��
    KindFile.GetString(nFindNo, KIND_FILE_SECT5, "", m_szResPath, sizeof(m_szResPath));  // �л�Ů

    // ����npc  �������
    if (m_nNpcKind == NPC_RES_SPECIAL)
    {
        KTabFile PartFile, SoundName, ShadowName;
        // �õ�����˵���ļ���
        KindFile.GetString(nFindNo, KIND_FILE_SECT1, "", szBuffer, sizeof(szBuffer));
        if (!szBuffer[0])
            return FALSE;
        //		g_SetFilePath(RES_INI_FILE_PATH);
        g_UnitePathAndName(RES_INI_FILE_PATH, szBuffer, szBuf);
        if (!PartFile.Load(szBuf))  // ���� �л�Ů�Ĳ���.txt
            return FALSE;
        // �õ����������Ϣ
        m_nPartNum = 0;
        for (i = 0; i < MAX_BODY_PART; ++i)
        {  // i = 2 3 4 5 6 7
            for (j = 0; j < MAX_BODY_PART_SECT; ++j)
            {
                m_nSectInfo[i * MAX_BODY_PART_SECT + j].Clear();
                // �ڶ��� �����п�ʼ j =3 4 5 6
                PartFile.GetString(i + 2, j + 3, "", m_nSectInfo[i * MAX_BODY_PART_SECT + j].szSectName,
                                   sizeof(m_nSectInfo[i * MAX_BODY_PART_SECT + j].szSectName));
                if (m_nSectInfo[i * MAX_BODY_PART_SECT + j].szSectName[0])  // i=5,j=0 Ϊ���
                {  // i =1 Ϊװ�������+����+������ 4 + (0,1,2,3)= 4 5 6 7
                    m_nSectInfo[i * MAX_BODY_PART_SECT + j].nFlag = 1;  // m_nSectInfo[5 * MAX_BODY_PART_SECT + 0] 20
                    m_nPartNum++;                                       // ������������1
                }
            }
        }
        PartFile.Clear();
        // �õ�ÿ����������Դ˵���ļ���
        for (i = 0; i < MAX_PART; ++i)  // 24
        {
            if (m_nSectInfo[i].nFlag)
            {  // ���������λ����
                KindFile.GetString(nFindNo, m_nSectInfo[i].szSectName, "", m_nSectInfo[i].szSectResName,
                                   sizeof(m_nSectInfo[i].szSectResName));  // ��ֵ���������
                // ��Դ��Ϣ˵���ļ�������Դ˵���ļ�������SPR_INFO_NAME������Ϣ����
                if (m_nSectInfo[i].szSectResName[0])
                {
                    std::string Tempstr        = m_nSectInfo[i].szSectResName;
                    std::string::size_type pos = Tempstr.find(".txt");
                    Tempstr.replace(pos, Tempstr.length(), "��Ϣ.txt");
                    sprintf(m_nSectInfo[i].szSectSprInfoName, "%s", Tempstr.c_str());
                    /*g_StrCpyLen(
                            m_nSectInfo[i].szSectSprInfoName,
                            m_nSectInfo[i].szSectResName,
                            g_StrLen(m_nSectInfo[i].szSectResName) - 3
                            );//�����ȸ��� ȥ�� .TXT �����ַ�
                    */
                    // g_StrCat(m_nSectInfo[i].szSectSprInfoName,SPR_INFO_NAME); //�ں����� ��Ϣ ������
                    // g_StrCat(m_nSectInfo[i].szSectSprInfoName, ".txt");//�õ� *****��Ϣ.TXT �ļ��� SPR ��Ϣ��
                }
            }
        }
        // �õ�ÿ�������ľ������Դ�ļ���
        KTabFile SectFile, SectInfoFile;
        int nGetEquipNo, nActionCount;
        for (i = 0; i < MAX_PART; ++i)
        {
            m_cResInfo[i].AutoDelete();
            if (m_nSectInfo[i].nFlag)
            {  // ��λ�������
                //				g_SetFilePath(RES_INI_FILE_PATH);
                g_UnitePathAndName(RES_INI_FILE_PATH, m_nSectInfo[i].szSectResName,
                                   szBuf);  // ��Ӧ������������Ĳ�����SPR·��
                // sprintf(szBuf,"%s",U2G(szBuf).c_str());
                // CCMessageBox(szBuf,"MAX_PARTA");
                if (!SectFile.Load(szBuf))
                    continue;
                g_UnitePathAndName(RES_INI_FILE_PATH, m_nSectInfo[i].szSectSprInfoName, szBuf);  // ��Ů���� ��Ϣ ·��
                // CCMessageBox(szBuf,"MAX_PARTB.....");
                if (!SectInfoFile.Load(szBuf))
                    continue;
                nGetEquipNo = SectFile.GetHeight() - 1;
                if (nGetEquipNo <= 0)
                    continue;
                nActionCount = cActionName->GetActionCount();  // ����:	�õ�����������
                if (nActionCount <= 0)
                    continue;
                m_cResInfo[i].AutoNew(nActionCount, nGetEquipNo);  // �����ڴ���
                for (j = 0; j < nGetEquipNo; ++j)
                {
                    for (k = 0; k < nActionCount; ++k)  // k=	nActionCount-1+2+1
                    {
                        // ���ַ����Ƚ�̫��������ֱ���ñ�ţ����Ǳ�����뱣֤������
                        // cActionName->GetActionName(k, szBuffer, sizeof(szBuffer));
                        //						SectFile.GetString(
                        //							j + 2,
                        //							szBuffer,
                        //							"",
                        //							szTemp,
                        //							80);
                        int nIsMa = 0;
                        SectFile.GetString(j + 2, k + 2, "", szTemp, sizeof(szTemp));
                        SectFile.GetInteger(j + 2, "�ڲ�����", 0, &nIsMa);

                        if (nIsMa == 1 && strstr(lpszNpcName, "MainLady"))
                        {
                            ComposePathAndName(m_cResInfo[i].m_cSprInfo[j * nActionCount + k].szFileName,
                                               "spr\\npcres\\woman", szTemp);
                        }
                        else if (nIsMa == 1 && strstr(lpszNpcName, "MainMan"))
                        {
                            ComposePathAndName(m_cResInfo[i].m_cSprInfo[j * nActionCount + k].szFileName,
                                               "spr\\npcres\\man", szTemp);
                        }
                        else
                        {
                            if (strstr(lpszNpcName, "Ů����") && strstr(szBuf, "Ů������"))
                            {
                                ComposePathAndName(m_cResInfo[i].m_cSprInfo[j * nActionCount + k].szFileName,
                                                   "spr\\npcres", szTemp);
                                // CCMessageBox(m_cResInfo[i].m_cSprInfo[j * nActionCount + k].szFileName,szTemp);
                            }
                            else
                                ComposePathAndName(m_cResInfo[i].m_cSprInfo[j * nActionCount + k].szFileName,
                                                   m_szResPath, szTemp);

                            /// CCMessageBox(szBuf,"Ů����");
                        }

                        SectInfoFile.GetString(
                            j + 2, k + 2, "16,8,1", szTemp,
                            sizeof(szTemp));  // ��Ϣ�ļ� ֡�������򣬼��ʱ��

                        pcszTemp = szTemp;
                        m_cResInfo[i].m_cSprInfo[j * nActionCount + k].nTotalFrames =
                            KSG_StringGetInt(&pcszTemp, 16);   // ��֡��
                        KSG_StringSkipSymbol(&pcszTemp, ',');  // ������ ȡֵ��
                        m_cResInfo[i].m_cSprInfo[j * nActionCount + k].nTotalDirs =
                            KSG_StringGetInt(&pcszTemp, 8);  // �ܷ���
                        KSG_StringSkipSymbol(&pcszTemp, ',');
                        m_cResInfo[i].m_cSprInfo[j * nActionCount + k].nInterval = KSG_StringGetInt(
                            &pcszTemp, 1);  // ʱ����
                                            // sscanf(szTemp, "%d,%d,%d",
                                            //	&m_cResInfo[i].m_cSprInfo[j * nActionCount + k].nTotalFrames,
                                            //	&m_cResInfo[i].m_cSprInfo[j * nActionCount + k].nTotalDirs,
                                            //	&m_cResInfo[i].m_cSprInfo[j * nActionCount + k].nInterval
                        //);
                    }
                }
                SectInfoFile.Clear();
                SectFile.Clear();
            }
        }
        // �õ�������Ϊ�������ļ���
        KTabFile NoHorseFile, OnHorseFile;
        int nTableWidth, nTableHeight, nGetNo;
        char szNoHorseTableName[80], szOnHorseTableName[80];
        KindFile.GetString(nFindNo, KIND_FILE_SECT2, "", szNoHorseTableName, sizeof(szNoHorseTableName));
        KindFile.GetString(nFindNo, KIND_FILE_SECT3, "", szOnHorseTableName, sizeof(szOnHorseTableName));
        // δ������Ӧ��
        if (szNoHorseTableName[0])
        {
            //			g_SetFilePath(RES_INI_FILE_PATH);
            g_UnitePathAndName(RES_INI_FILE_PATH, szNoHorseTableName, szBuf);  // �ϲ�·��
            if (NoHorseFile.Load(szBuf))                                       // ���ض�Ӧ���ļ���
            {
                nTableWidth  = NoHorseFile.GetWidth() - 1;
                nTableHeight = NoHorseFile.GetHeight() - 1;
                m_NoHorseTable.AutoNew(nTableWidth, nTableHeight);
                for (i = 0; i < nTableHeight; ++i)
                {
                    for (j = 0; j < nTableWidth; ++j)
                    {
                        NoHorseFile.GetString(i + 2, j + 2, "", szBuffer, sizeof(szBuffer));
                        nGetNo = cActionName->GetActionNo(szBuffer);  // ����:�ɶ������Ƶõ��������
                        m_NoHorseTable.SetValue(j, i, nGetNo);  // ����:�趨������ĳһ���ֵ
                    }
                }
            }
        }
        NoHorseFile.Clear();
        // ������Ӧ��
        if (szOnHorseTableName[0])
        {
            //			g_SetFilePath(RES_INI_FILE_PATH);
            g_UnitePathAndName(RES_INI_FILE_PATH, szOnHorseTableName, szBuf);
            if (OnHorseFile.Load(szBuf))
            {
                nTableWidth  = OnHorseFile.GetWidth() - 1;
                nTableHeight = OnHorseFile.GetHeight() - 1;
                m_OnHorseTable.AutoNew(nTableWidth, nTableHeight);
                for (i = 0; i < nTableHeight; ++i)
                {
                    for (j = 0; j < nTableWidth; ++j)
                    {
                        OnHorseFile.GetString(i + 2, j + 2, "", szBuffer, sizeof(szBuffer));
                        nGetNo = cActionName->GetActionNo(szBuffer);
                        m_OnHorseTable.SetValue(j, i, nGetNo);
                    }
                }
            }
        }
        OnHorseFile.Clear();
        // �� ��������.TXT�� �õ���ͼ˳����ļ���
        if (KindFile.GetString(nFindNo, KIND_FILE_SECT4, "", szBuffer, sizeof(szBuffer)))  //
        {
            strcpy(m_cSortTable.m_sSortTableFileName, szBuffer);
            m_cSortTable.GetcTable(szBuffer, cActionName, m_nPartNum);  // ����:	�������ļ��а�������Ϣ��ȡ����
        }
        KindFile.Clear();

        // ��ö�����Ӱ�ļ���Ϣ
        //		g_SetFilePath(RES_INI_FILE_PATH);
        nActionCount = cActionName->GetActionCount();
        this->m_cShadowInfo.Init(nActionCount);
        ShadowName.Load(PLAYER_RES_SHADOW_FILE);
        nFindNo = ShadowName.FindRow(lpszNpcName);
        if (nFindNo > 0 && nActionCount > 0)
        {
            for (i = 0; i < nActionCount; ++i)
            {
                ShadowName.GetString(nFindNo, 2 + i * 2, "", szTemp, sizeof(szTemp));
                if (szTemp[0])
                    this->ComposePathAndName(m_cShadowInfo.m_psInfo[i].m_szName, m_szResPath, szTemp);
                ShadowName.GetString(nFindNo, 2 + i * 2 + 1, "16,8,1", szTemp, sizeof(szTemp));

                pcszTemp                                = szTemp;
                m_cShadowInfo.m_psInfo[i].m_nTotalFrame = KSG_StringGetInt(&pcszTemp, 16);
                KSG_StringSkipSymbol(&pcszTemp, ',');
                m_cShadowInfo.m_psInfo[i].m_nTotalDir = KSG_StringGetInt(&pcszTemp, 8);
                KSG_StringSkipSymbol(&pcszTemp, ',');
                m_cShadowInfo.m_psInfo[i].m_nInterval = KSG_StringGetInt(&pcszTemp, 1);
                // sscanf(szTemp, "%d,%d,%d", &m_cShadowInfo.m_psInfo[i].m_nTotalFrame,
                // &m_cShadowInfo.m_psInfo[i].m_nTotalDir, &m_cShadowInfo.m_psInfo[i].m_nInterval);

                m_cShadowInfo.m_psInfo[i].m_nCgX = SPR_X_OFF;
                m_cShadowInfo.m_psInfo[i].m_nCgY = SPR_Y_OFF;
            }
        }
        ShadowName.Clear();
        // ��ö�����Ч�ļ���
        //		g_SetFilePath(RES_INI_FILE_PATH);
        SoundName.Load(PLAYER_SOUND_FILE);
        nFindNo      = SoundName.FindColumn(lpszNpcName);
        nActionCount = cActionName->GetActionCount();
        if (nFindNo > 0 && nActionCount > 0)
        {
            for (i = 0; i < nActionCount; ++i)
            {
                SoundName.GetString(i + 2, nFindNo, "", szTemp, sizeof(szTemp));
                if (szTemp[0])
                    this->ComposePathAndName(this->m_szSoundName[i], RES_SOUND_FILE_PATH, szTemp);
            }
        }

        SoundName.Clear();
    }
    // ����
    else if (m_nNpcKind == NPC_RES_NORMAL)
    {
        int nActionCount;
        KTabFile NormalNpc, NormalNpcSprInfo, SoundName;

        for (i = 0; i < MAX_PART; ++i)
            m_nSectInfo[i].nFlag = 0;

        m_nSectInfo[NORMAL_NPC_PART_NO].nFlag = 1;

        m_nPartNum = 1;

        for (i = 0; i < MAX_PART; ++i)
            m_cResInfo[i].AutoDelete();
        nActionCount = cNpcAction->GetActionCount();
        if (nActionCount < 0)
            nActionCount = 0;
        m_cResInfo[NORMAL_NPC_PART_NO].AutoNew(nActionCount, 1);
        this->m_cShadowInfo.Init(nActionCount);

        //		g_SetFilePath(RES_INI_FILE_PATH);
        if (!NormalNpc.Load(NPC_NORMAL_RES_FILE))
            return FALSE;
        // ������Ϣ�ļ�û��û�б�Ҫ��ʼ�����ɹ�������û��If(!) return FALSE;
        NormalNpcSprInfo.Load(NPC_NORMAL_SPRINFO_FILE);
        SoundName.Load(NPC_SOUND_FILE);
        nFindNo = NormalNpc.FindRow(lpszNpcName);
        if (nFindNo < 0)
            return FALSE;

        for (i = 0; i < nActionCount; ++i)
        {
            cNpcAction->GetActionName(i, szBuffer, sizeof(szBuffer));  // Ӣ��
            NormalNpc.GetString(nFindNo, szBuffer, "", szTemp, sizeof(szTemp));
            ComposePathAndName(m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].szFileName, m_szResPath, szTemp);
            NormalNpcSprInfo.GetString(nFindNo, szBuffer, "16,8,0", szTemp, sizeof(szTemp));

            pcszTemp                                                  = szTemp;
            m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].nTotalFrames = KSG_StringGetInt(&pcszTemp, 16);
            KSG_StringSkipSymbol(&pcszTemp, ',');
            m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].nTotalDirs = KSG_StringGetInt(&pcszTemp, 8);
            KSG_StringSkipSymbol(&pcszTemp, ',');
            m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].nInterval = KSG_StringGetInt(&pcszTemp, 0);
            // sscanf(szTemp, "%d,%d,%d",
            //	&m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].nTotalFrames,
            //	&m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].nTotalDirs,
            //	&m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].nInterval
            //);

            KNpcResNode::GetShadowName(m_cShadowInfo.m_psInfo[i].m_szName,
                                       m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].szFileName);
            m_cShadowInfo.m_psInfo[i].m_nTotalFrame = m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].nTotalFrames;
            m_cShadowInfo.m_psInfo[i].m_nTotalDir   = m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].nTotalDirs;
            m_cShadowInfo.m_psInfo[i].m_nInterval   = m_cResInfo[NORMAL_NPC_PART_NO].m_cSprInfo[i].nInterval;
            m_cShadowInfo.m_psInfo[i].m_nCgX        = SPR_X_OFF;
            m_cShadowInfo.m_psInfo[i].m_nCgY        = SPR_Y_OFF;

            SoundName.GetString(nFindNo, szBuffer, "", szTemp, sizeof(szTemp));
            if (szTemp[0])
                ComposePathAndName(m_szSoundName[i], RES_SOUND_FILE_PATH, szTemp);
        }
        SoundName.Clear();
        NormalNpcSprInfo.Clear();
        NormalNpc.Clear();
    }

    return TRUE;
}

//---------------------------------------------------------------------------
// ����:	��·�����ļ����ϳ���һ���Ϊ��·�����ļ���
//---------------------------------------------------------------------------
void KNpcResNode::ComposePathAndName(char* lpszGet, const char* lpszPath, char* lpszName)
{
    if (strlen(lpszName) == 0)
    {
        lpszGet[0] = 0;
        return;
    }
    if (strlen(lpszPath) == 0)
    {
        strcpy(lpszGet, lpszName);
        return;
    }
    if (lpszPath[0] != '\\')
    {
        lpszGet[0] = '\\';
        lpszGet[1] = 0;
    }
    else
    {
        lpszGet[0] = 0;
    }
    strcat(lpszGet, lpszPath);
    if (lpszGet[strlen(lpszGet) - 1] != '\\')
        strcat(lpszGet, "\\");

    if (strstr(lpszName, "..\\"))
    {  //
        std::string nTempsr        = lpszName;
        std::string::size_type pos = nTempsr.find("..\\");  // ��ʼ��λ��
        nTempsr.replace(pos, 3, "");
        strcat(lpszGet, nTempsr.c_str());
        // sprintf(m_nSectInfo[i].szSectSprInfoName,Tempstr.c_str());
        // nTempsr.replace()
    }
    else
        strcat(lpszGet, lpszName);
}

//---------------------------------------------------------------------------
// ����:	�ж�ĳ�������Ƿ����
//---------------------------------------------------------------------------
int KNpcResNode::CheckPartExist(int nPartNo)
{
    if (nPartNo < 0 || nPartNo >= MAX_PART)
        return FALSE;
    if (m_nSectInfo[nPartNo].nFlag)
        return TRUE;
    return FALSE;
}

//---------------------------------------------------------------------------
// ����:	���ĳ��������ĳ��װ����ĳ�������µ� spr �ļ���
//---------------------------------------------------------------------------
int KNpcResNode::GetFileName(int nPartNo,
                             int nActionNo,
                             int nEquipNo,
                             const char* lpszDefault,
                             char* lpszGetName,
                             int nStrLen)
{
    if (nPartNo < 0 || nPartNo >= MAX_PART)
        return FALSE;
    if (m_cResInfo[nPartNo].GetName(nActionNo, nEquipNo, "", lpszGetName, nStrLen))
        return TRUE;
    return FALSE;
}

//---------------------------------------------------------------------------
// ����:	���ĳ��������ĳ��װ����ĳ�������µ� spr �ļ���Ϣ
//---------------------------------------------------------------------------
int KNpcResNode::GetInterval(int nPartNo, int nActionNo, int nEquipNo, int nDefault)
{
    if (nPartNo < 0 || nPartNo >= MAX_PART)
        return nDefault;

    return m_cResInfo[nPartNo].GetInterval(nActionNo, nEquipNo, nDefault);
}

//---------------------------------------------------------------------------
// ����:	���ĳ��������ĳ��װ����ĳ�������µ� spr �ļ���Ϣ
//---------------------------------------------------------------------------
int KNpcResNode::GetTotalFrames(int nPartNo, int nActionNo, int nEquipNo, int nDefault)
{
    if (nPartNo < 0 || nPartNo >= MAX_PART)
        return nDefault;

    return m_cResInfo[nPartNo].GetTotalFrames(nActionNo, nEquipNo, nDefault);
}

//---------------------------------------------------------------------------
// ����:	���ĳ��������ĳ��װ����ĳ�������µ� spr �ļ���Ϣ
//---------------------------------------------------------------------------
int KNpcResNode::GetTotalDirs(int nPartNo, int nActionNo, int nEquipNo, int nDefault)
{
    if (nPartNo < 0 || nPartNo >= MAX_PART)
        return nDefault;

    return m_cResInfo[nPartNo].GetTotalDirs(nActionNo, nEquipNo, nDefault);
}

//---------------------------------------------------------------------------
// ����:	���ĳ����ĳ״̬�µĶ������
//---------------------------------------------------------------------------
int KNpcResNode::GetActNo(int nDoing, int nEquipNo, int bRideHorse)
{
    // ��ͨnpc Doing �� Action ��ͬ
    if (m_nNpcKind == NPC_RES_NORMAL)
        return nDoing;

    // ���� npc�� ���
    if (bRideHorse == FALSE)
    {  // δ������
        return m_NoHorseTable.GetValue(nEquipNo, nDoing);
    }
    else
    {  // ������
        return m_OnHorseTable.GetValue(nEquipNo, nDoing);
    }
}

//---------------------------------------------------------------------------
// ����:	�ӱ����еõ�һ��������Ϣ
//---------------------------------------------------------------------------
int KNpcResNode::GetnSort(int nActNo, int nDir, int nFrameNo, int* lpnTable, int nTableLen)
{
    // ��ͨnpc
    if (m_nNpcKind == NPC_RES_NORMAL)
    {
        if (!lpnTable || nTableLen <= 0)
            return FALSE;
        for (int i = 0; i < nTableLen; ++i)
            lpnTable[i] = -1;

        lpnTable[0] = NORMAL_NPC_PART_NO;
        return TRUE;
    }

    // ����npc
    if (m_cSortTable.GetSort(nActNo, nDir, nFrameNo, lpnTable, nTableLen))
        return TRUE;

    return FALSE;
}

//---------------------------------------------------------------------------
// ���ܣ����ĳ��������Ӧ����Ч�ļ���
//---------------------------------------------------------------------------
void KNpcResNode::GetActionSoundName(int nAction, char* lpszSoundName)
{
    if (nAction < 0 || nAction >= MAX_ACTION)
        return;
    if (!lpszSoundName)
        return;
    g_StrCpy(lpszSoundName, m_szSoundName[nAction]);
}

//---------------------------------------------------------------------------
// ���ܣ���Ĭ�Ϲ����һ��npc��sprͼ�ļ���ת������Ӧ����Ӱ�ļ���
//---------------------------------------------------------------------------
void KNpcResNode::GetShadowName(char* lpszShadow, char* lpszSprName)
{
    if (lpszSprName[0] == 0)
    {
        lpszShadow[0] = 0;
        return;
    }
    strcpy(lpszShadow, lpszSprName);
    for (int i = strlen(lpszShadow) - 1; i >= 0; i--)
    {
        if (lpszShadow[i] == '.')
        {
            lpszShadow[i] = 0;
            break;
        }
    }
    strcat(lpszShadow, "b.spr");
}

//---------------------------- class CActionName ----------------------------
//---------------------------------------------------------------------------
// ����:	���캯��
//---------------------------------------------------------------------------
CActionName::CActionName()
{
    m_nMaxAction   = MAX_ACTION;
    m_nCurActionNo = 0;
    for (int i = 0; i < MAX_ACTION; ++i)
        m_szName[i][0] = 0;
}

//---------------------------------------------------------------------------
// ����:	��ȡ�������ࡢ���Ƶ���Ϣ  "npc������.txt" �� "������ű�.txt"
//---------------------------------------------------------------------------
int CActionName::Init(const char* lpszFileName)
{
    if (!lpszFileName || !lpszFileName[0])
        return FALSE;
    char szBuf[FILE_NAME_LENGTH] = {0};
    ZeroMemory(szBuf, sizeof(szBuf));
    KTabFile caTabFile;
    int i;

    // g_SetFilePath(RES_INI_FILE_PATH);
    g_UnitePathAndName(RES_INI_FILE_PATH, lpszFileName, szBuf);
    if (!caTabFile.Load(szBuf))
        return FALSE;
    m_nCurActionNo = caTabFile.GetHeight() - 1;
    if (m_nCurActionNo <= 0)
    {
        caTabFile.Clear();
        return FALSE;
    }
    if (m_nCurActionNo > m_nMaxAction)  // �������
        m_nCurActionNo = m_nMaxAction;
    for (i = 0; i < m_nCurActionNo; ++i)
    {
        caTabFile.GetString(i + 2, 1, "", m_szName[i], sizeof(m_szName[i]));
    }
    caTabFile.Clear();
    return TRUE;
}

//---------------------------------------------------------------------------
// ����:	�ɶ������Ƶõ��������
//---------------------------------------------------------------------------
int CActionName::GetActionNo(char* lpszName)
{
    if (!lpszName || !lpszName[0])
        return -1;
    for (int i = 0; i < m_nCurActionNo; ++i)
    {
        if (strcmp(lpszName, m_szName[i]) == 0)
            return i;
    }
    return -1;
}

//---------------------------------------------------------------------------
// ����:	�õ�����������
//---------------------------------------------------------------------------
int CActionName::GetActionCount()
{
    return m_nCurActionNo;
}

//---------------------------------------------------------------------------
// ����:	�ɶ�����ŵõ���������
//---------------------------------------------------------------------------
int CActionName::GetActionName(int nNo, char* lpszName, int nSize)
{
    if (!lpszName)
        return FALSE;
    if (nNo < 0 || nNo >= m_nCurActionNo)
        return FALSE;
    if (strlen(m_szName[nNo]) >= (unsigned long)nSize)
        return FALSE;
    strcpy(lpszName, m_szName[nNo]);

    return TRUE;
}
//-------------------------- class CActionName end --------------------------

//-------------------------- class CShadowFileName --------------------------

//---------------------------------------------------------------------------
// ���ܣ����캯��
//---------------------------------------------------------------------------
CShadowFileName::CShadowFileName()
{
    m_psInfo      = NULL;
    m_nInfoLength = 0;
}

//---------------------------------------------------------------------------
// ���ܣ���������
//---------------------------------------------------------------------------
CShadowFileName::~CShadowFileName()
{
    if (m_psInfo)
    {
        delete[] m_psInfo;
        m_psInfo = NULL;
    }
    m_nInfoLength = 0;
}

void CShadowFileName::Init(int nActionCount)
{
    if (m_psInfo)
    {
        delete[] m_psInfo;
        m_psInfo = NULL;
    }
    m_nInfoLength = 0;

    if (nActionCount <= 0)
        return;

    m_psInfo = (SShadowFileInfo*)new SShadowFileInfo[nActionCount];
    memset(m_psInfo, 0, sizeof(SShadowFileInfo) * nActionCount);
    m_nInfoLength = nActionCount;
}

//---------------------------------------------------------------------------
// ���ܣ��õ�ĳ����������Ӱ�ļ�����������ļ���Ϣ
//---------------------------------------------------------------------------
int CShadowFileName::GetFile(int nActionNo,
                             int* pnFrame,
                             int* pnDir,
                             int* pnInterval,
                             int* pnCgX,
                             int* pnCgY,
                             char* lpszName)
{
    if (nActionNo < 0 || nActionNo >= m_nInfoLength)
        return FALSE;
    if (!m_psInfo)
        return FALSE;

    *pnFrame    = m_psInfo[nActionNo].m_nTotalFrame;
    *pnDir      = m_psInfo[nActionNo].m_nTotalDir;
    *pnInterval = m_psInfo[nActionNo].m_nInterval;
    *pnCgX      = m_psInfo[nActionNo].m_nCgX;
    *pnCgY      = m_psInfo[nActionNo].m_nCgY;
    strcpy(lpszName, m_psInfo[nActionNo].m_szName);

    return TRUE;
}

//------------------------ class CShadowFileName end ------------------------

//------------------------- class CStateMagicTable --------------------------
//---------------------------------------------------------------------------
// ����:	���캯��
//---------------------------------------------------------------------------
CStateMagicTable::CStateMagicTable()
{
    for (int i = 0; i < MAX_RES_STATE; ++i)
    {
        m_nType[i]       = 0;
        m_nPlayType[i]   = 0;
        m_nBackStart[i]  = 0;
        m_nBackEnd[i]    = 0;
        m_nTotalFrame[i] = 1;
        m_nTotalDir[i]   = 1;
        m_nInterVal[i]   = 1;
        m_szName[i][0]   = 0;
    }
}

//---------------------------------------------------------------------------
// ����:	��ʼ��
//---------------------------------------------------------------------------
int CStateMagicTable::Init()
{
    //	KTabFile	cTabFile;
    int i;
    char szBuffer[80], szTemp[128];

    //	g_SetFilePath(RES_INI_FILE_PATH);
    if (!cTabFile.Load(STATE_MAGIC_TABLE_NAME))
        return FALSE;

    // KNpcResNode		Temp;
    for (i = 0; i < MAX_RES_STATE; ++i)
    {
        cTabFile.GetString(i + 2, 3, "", szBuffer, sizeof(szBuffer));  // ��λ
        if (strcmp(szBuffer, "Head") == 0)
            m_nType[i] = STATE_MAGIC_HEAD;
        else if (strcmp(szBuffer, "Foot") == 0)
            m_nType[i] = STATE_MAGIC_FOOT;
        else
            m_nType[i] = STATE_MAGIC_BODY;

        cTabFile.GetString(i + 2, 4, "", szBuffer, sizeof(szBuffer));
        if (strcmp(szBuffer, "Loop") == 0)
            m_nPlayType[i] = 0;
        else
            m_nPlayType[i] = 1;
        cTabFile.GetInteger(i + 2, 5, 0, &m_nBackStart[i]);
        cTabFile.GetInteger(i + 2, 6, 0, &m_nBackEnd[i]);
        cTabFile.GetInteger(i + 2, 7, 1, &m_nTotalFrame[i]);
        cTabFile.GetInteger(i + 2, 8, 1, &m_nTotalDir[i]);
        cTabFile.GetInteger(i + 2, 9, 1, &m_nInterVal[i]);
        cTabFile.GetString(i + 2, 2, "", szTemp, sizeof(szTemp));  // spr�ļ�·�� ���� �ļ���
        g_StrCpy(m_szName[i], szTemp);                             // ��ֵspr�ļ���
        //		Temp.ComposePathAndName(m_szName[i], RES_NPC_STYLE_PATH, szTemp);
    }
    // cTabFile.Clear();
    return TRUE;
}

//---------------------------------------------------------------------------
// ����:	�õ�ĳ��״̬�Ĺ�ӰЧ������Ϣ
// nNo �� 1 �� MAX_RES_STATE
//---------------------------------------------------------------------------
void CStateMagicTable::GetInfo(int nNo,
                               char* lpszGetName,
                               int* pnType,
                               int* pnPlayType,
                               int* pnBackStart,
                               int* pnBackEnd,
                               int* pnTotalFrame,
                               int* pnTotalDir,
                               int* pnInterVal,
                               int* MapPosX,
                               int* MapPosY)
{
    if (!lpszGetName)
        return;
    if (nNo <= 0 || nNo > MAX_RES_STATE)
    {
        lpszGetName[0] = 0;
        *pnType        = 0;
        *pnPlayType    = 0;
        *pnBackStart   = 0;
        *pnBackEnd     = 0;
        *pnTotalFrame  = 1;
        *pnTotalDir    = 1;
        *pnInterVal    = 1;
        return;
    }
    strcpy(lpszGetName, m_szName[nNo - 1]);
    *pnType       = m_nType[nNo - 1];
    *pnPlayType   = m_nPlayType[nNo - 1];
    *pnBackStart  = m_nBackStart[nNo - 1];
    *pnBackEnd    = m_nBackEnd[nNo - 1];
    *pnTotalFrame = m_nTotalFrame[nNo - 1];
    *pnTotalDir   = m_nTotalDir[nNo - 1];
    *pnInterVal   = m_nInterVal[nNo - 1];
    *MapPosX      = m_nMapPosX[nNo - 1];
    *MapPosY      = m_nMapPosY[nNo - 1];
}
//----------------------- class CStateMagicTable end ------------------------

//----------------------------- class CRESINFO ------------------------------
//---------------------------------------------------------------------------
// ����:	���캯��
//---------------------------------------------------------------------------
CRESINFO::CRESINFO()
{
    m_nActionKind = 0;
    m_nEquipKind  = 0;
    m_cSprInfo    = NULL;
}

//---------------------------------------------------------------------------
// ����:	��������
//---------------------------------------------------------------------------
CRESINFO::~CRESINFO()
{
    AutoDelete();
}

//---------------------------------------------------------------------------
// ����:	�������
//---------------------------------------------------------------------------
void CRESINFO::AutoDelete()
{
    if (m_cSprInfo)
    {
        delete[] m_cSprInfo;
        m_cSprInfo = NULL;
    }
    m_nActionKind = 0;
    m_nEquipKind  = 0;
}

//---------------------------------------------------------------------------
// ����:	�趨������������װ������������������ڴ滺����
//---------------------------------------------------------------------------
int CRESINFO::AutoNew(int nWidth, int nHeight)
{
    int i;

    AutoDelete();

    m_nActionKind = nWidth;
    if (m_nActionKind < 0)
        m_nActionKind = 0;
    m_nEquipKind = nHeight;
    if (m_nEquipKind < 0)
        m_nEquipKind = 0;

    if (m_nActionKind == 0 || m_nEquipKind == 0)
        return FALSE;

    m_cSprInfo = (CSPR_INFO*)new CSPR_INFO[m_nActionKind * m_nEquipKind];
    if (!m_cSprInfo)
        return FALSE;
    ZeroMemory(m_cSprInfo, sizeof(CSPR_INFO) * m_nActionKind * m_nEquipKind);

    for (i = 0; i < m_nActionKind * m_nEquipKind; ++i)
    {
        m_cSprInfo[i].szFileName[0] = 0;
    }

    return TRUE;
}

//---------------------------------------------------------------------------
// ����:	���ݶ�����š�װ����ŵõ���Դ�ļ���
//---------------------------------------------------------------------------
int CRESINFO::GetName(int nActionNo, int nEquipNo, const char* lpszDefault, char* lpszGetName, int nStrLen)
{
    if (!lpszGetName)
        return FALSE;
    if (!m_cSprInfo)  // ��BUG
    {
        // CCMessageBox("m_cSprInfo is NULL","GetName");
        goto FALSE_LAB;
    }
    if (nActionNo < 0 || nActionNo >= m_nActionKind || nEquipNo < 0 || nEquipNo >= m_nEquipKind)
    {
        // CCString* msg =
        // CCString::createWithFormat("aNo:%d,eNo:%d,aK:%d,eK:%d",nActionNo,nEquipNo,m_nActionKind,m_nEquipKind);
        // CCMessageBox(msg->getCString(),"GetName");
        goto FALSE_LAB;
    }
    strcpy(lpszGetName, m_cSprInfo[nEquipNo * m_nActionKind + nActionNo].szFileName);
    // CCMessageBox(lpszGetName,"GetName is suss");
    return TRUE;

FALSE_LAB:
    g_StrCpyLen(lpszGetName, lpszDefault, nStrLen);
    return FALSE;
}
// ���ż��ʱ��
int CRESINFO::GetInterval(int nActionNo, int nEquipNo, int nDefault)
{
    if (nActionNo < 0 || nActionNo >= m_nActionKind || nEquipNo < 0 || nEquipNo >= m_nEquipKind)
        return nDefault;

    return m_cSprInfo[nEquipNo * m_nActionKind + nActionNo].nInterval;
}
// ��֡��
int CRESINFO::GetTotalFrames(int nActionNo, int nEquipNo, int nDefault)
{
    if (nActionNo < 0 || nActionNo >= m_nActionKind || nEquipNo < 0 || nEquipNo >= m_nEquipKind)
        return nDefault;

    return m_cSprInfo[nEquipNo * m_nActionKind + nActionNo].nTotalFrames;
}
// �ܷ���
int CRESINFO::GetTotalDirs(int nActionNo, int nEquipNo, int nDefault)
{
    if (nActionNo < 0 || nActionNo >= m_nActionKind || nEquipNo < 0 || nEquipNo >= m_nEquipKind)
        return nDefault;

    return m_cSprInfo[nEquipNo * m_nActionKind + nActionNo].nTotalDirs;
}
//--------------------------- class CRESINFO end ----------------------------

//------------------------- class CEquipStyleTable --------------------------
//---------------------------------------------------------------------------
// ����:	���캯��
//---------------------------------------------------------------------------
CEquipStyleTable::CEquipStyleTable()
{
    m_nStyleKind  = 0;
    m_nEquipKind  = 0;
    m_lpnActionNo = NULL;
}

//---------------------------------------------------------------------------
// ����:	��������
//---------------------------------------------------------------------------
CEquipStyleTable::~CEquipStyleTable()
{
    AutoDelete();
}

//---------------------------------------------------------------------------
// ����:	�������
//---------------------------------------------------------------------------
void CEquipStyleTable::AutoDelete()
{
    if (m_lpnActionNo)
    {
        delete[] m_lpnActionNo;
        m_lpnActionNo = NULL;
    }
    m_nStyleKind = 0;
    m_nEquipKind = 0;
}

//---------------------------------------------------------------------------
// ����:	�趨���񳤿�����ʼ�����񻺳���
//---------------------------------------------------------------------------
int CEquipStyleTable::AutoNew(int nWidth, int nHeight)
{
    AutoDelete();

    if (nWidth < 0 || nHeight < 0)
        return FALSE;

    m_nStyleKind = nWidth;
    m_nEquipKind = nHeight;

    m_lpnActionNo = new int[nWidth * nHeight];
    if (m_lpnActionNo == NULL)
        return FALSE;
    for (int i = 0; i < nWidth * nHeight; ++i)
        m_lpnActionNo[i] = -1;

    return TRUE;
}

//---------------------------------------------------------------------------
// ����:	�趨������ĳһ���ֵ
//---------------------------------------------------------------------------
void CEquipStyleTable::SetValue(int nXpos, int nYpos, int nValue)
{
    if (nXpos < 0 || nXpos >= m_nStyleKind || nYpos < 0 || nYpos >= m_nEquipKind)
        return;
    m_lpnActionNo[nYpos * m_nStyleKind + nXpos] = nValue;
}

//---------------------------------------------------------------------------
// ����:	�õ�������ĳһ���ֵ
// ����:	nXpos : װ�����-��۱��  nYpos : ״̬���
// ����:	�������
//---------------------------------------------------------------------------
int CEquipStyleTable::GetValue(int nXpos, int nYpos)
{
    if (nXpos < 0 || nXpos >= m_nEquipKind || nYpos < 0 || nYpos >= m_nStyleKind)
        return -1;
    if (m_lpnActionNo == NULL)
        return -1;
    return m_lpnActionNo[nXpos * m_nStyleKind + nYpos];
}

//----------------------- class CEquipStyleTable end ------------------------

//---------------------------- class CSortTable -----------------------------
//---------------------------------------------------------------------------
// ����:	���캯��
//---------------------------------------------------------------------------
CSortTable::CSortTable()
{
    m_lpnSortTable   = NULL;
    m_nSortTableSize = 0;
    m_nPartNum       = 1;
    strcpy(m_sSortTableFileName, "");
}

//---------------------------------------------------------------------------
// ����:	��������
//---------------------------------------------------------------------------
CSortTable::~CSortTable()
{
    Release();
}

//---------------------------------------------------------------------------
// ����:	�������
//---------------------------------------------------------------------------
void CSortTable::Release()
{
    int i;

    if (m_lpnSortTable)
    {
        delete[] m_lpnSortTable;
        m_lpnSortTable = NULL;
    }
    m_nSortTableSize = 0;

    for (i = 0; i < MAX_ACTION; ++i)
        m_sActTableOff[i].Clear();
}

//---------------------------------------------------------------------------
// ����:	���ַ����а�һϵ������ȡ����
// ����:	lpszStr : �ַ���  lpnTable : �������ֵ�����  nTableSize : ����Ĵ�С
//---------------------------------------------------------------------------
void CSortTable::SortStrToNum(char* lpszStr, int* lpnTable, int nTableSize)
{
    if (!lpszStr || !lpszStr[0] || !lpnTable || nTableSize < 1)
        return;
    if (nTableSize > m_nPartNum + 1)
        nTableSize = m_nPartNum + 1;

    int nTablePos = 0, nNumPos = 0, nStrPos = 0;
    char szNum[32];

    while (1)
    {
        if (lpszStr[nStrPos] == '-' || ('0' <= lpszStr[nStrPos] && lpszStr[nStrPos] <= '9'))
        {
            szNum[nNumPos] = lpszStr[nStrPos];
            nNumPos++;
        }
        else
        {
            if (nNumPos > 0)
            {
                szNum[nNumPos] = 0;
                nNumPos        = 0;
                if (nTablePos < nTableSize)
                {
                    lpnTable[nTablePos] = atoi(szNum);
                    nTablePos++;
                }
            }
            if (lpszStr[nStrPos] == 0)
                break;
        }
        nStrPos++;
    }
    for (int i = nTablePos; i < nTableSize; ++i)
        lpnTable[i] = -1;
}

//---------------------------------------------------------------------------
// ����:	�������ļ��а�������Ϣ��ȡ����
// ����:
//			lpszFileName: �����ļ���
//			*cActionName: ���ڻ�ȡ�����ĸ�����Ϣ
//			nPartNum������������
// ����:	��ȡ�ɹ����
//---------------------------------------------------------------------------
int CSortTable::GetcTable(char* lpszFileName, CActionName* cActionName, int nPartNum)
{
    if (!lpszFileName || !lpszFileName[0])
        return FALSE;

    char szBuf[FILE_NAME_LENGTH];
    KIniFile SortIni;

    //	g_SetFilePath(RES_INI_FILE_PATH);
    g_UnitePathAndName(RES_INI_FILE_PATH, lpszFileName, szBuf);
    if (!SortIni.Load(szBuf))
        return FALSE;
    int nReg = FALSE;
    nReg     = GetkTable(&SortIni, cActionName, nPartNum);
    SortIni.Clear();
    return nReg;
}

int CSortTable::GetkTable(KIniFile* pIni, CActionName* cActionName, int nPartNum)
{
    //	if ( !lpszFileName || !lpszFileName[0])
    //		return FALSE;
    if (nPartNum > 1)
        m_nPartNum = nPartNum;

    //	KIniFile	SortIni;
    int i, j, nActNo;
    char szBuffer[256], szKey[80], szActName[80];

    //	g_SetFilePath(RES_INI_FILE_PATH);
    //	if ( !SortIni.Load(lpszFileName) )
    //		return FALSE;
    //	KIniFile	*pIni = &SortIni;

    // ----------------------- ͨ�������ļ����� Table �Ĵ�С --------------------------
    Release();
    // ȫ��Ĭ�ϲ��֣�16 ������
    for (i = 0; i < MAX_SORT_DIR; ++i)
    {
        sprintf(szKey, "Dir%d", i + 1);
        if (pIni->GetString("DEFAULT", szKey, "", szBuffer, sizeof(szBuffer)))
            m_nSortTableSize++;
    }

    // �������������
    nActNo = cActionName->GetActionCount();
    if (nActNo <= 0)
        return FALSE;
    for (i = 0; i < nActNo; ++i)
    {
        // �õ�������
        if (!cActionName->GetActionName(i, szActName, sizeof(szActName)))
            continue;

        if (!pIni->IsSectionExist(szActName))  // ���û���������
            continue;

        // �ж��Ƿ�ʹ��Ĭ�Ϸ���˳��
        for (j = 0; j < MAX_SORT_DIR; ++j)
        {  // ����������·�����
            sprintf(szKey, "Dir%d", j + 1);
            if (pIni->GetString(szActName, szKey, "", szBuffer, sizeof(szBuffer)))
            {  //
                m_sActTableOff[i].bUseDefault = FALSE;
                m_sActTableOff[i].nActOff     = m_nSortTableSize;
                m_sActTableOff[i].nLineNum    = MAX_SORT_DIR;
                break;
            }
        }
        // �ж��Ƿ�����Ҫ���������������
        j = 1;
        while (1)
        {
            sprintf(szKey, "Line%d", j);
            if (pIni->GetString(szActName, szKey, "", szBuffer, sizeof(szBuffer)))
            {
                if (m_sActTableOff[i].nActOff == 0)
                {
                    m_sActTableOff[i].nActOff  = m_nSortTableSize;
                    m_sActTableOff[i].nLineNum = 1;
                }
                else
                {
                    m_sActTableOff[i].nLineNum++;
                }
            }
            else
            {
                break;
            }

            j++;
        }
        m_nSortTableSize += m_sActTableOff[i].nLineNum;
    }

    if (m_nSortTableSize < MAX_SORT_DIR)
        return FALSE;
    // --------------------- ͨ�������ļ����� Table �Ĵ�С end ------------------------

    // ------------------------------ �������Ķ�Ӧ������ ----------------------------
    // �� m_lpnSortTable �����ڴ�
    m_lpnSortTable = (int*)new int[m_nSortTableSize * (m_nPartNum + 1)];

    if (m_lpnSortTable == NULL)
    {
        Release();
        return FALSE;
    }

    for (i = 0; i < m_nSortTableSize * (m_nPartNum + 1); ++i)
    {
        m_lpnSortTable[i] = -1;
    }

    // int nDirCount;
    //	pIni->GetInteger("DEFAULT","DirNum",MAX_SORT_DIR,&nDirCount);
    int nTablePos = 0;
    // default ���֣�16 ������
    for (i = 0; i < MAX_SORT_DIR; ++i)
    {
        sprintf(szKey, "Dir%d", i + 1);
        if (pIni->GetString("DEFAULT", szKey, "", szBuffer, sizeof(szBuffer)))
        {
            SortStrToNum(szBuffer, &m_lpnSortTable[nTablePos * (m_nPartNum + 1)], m_nPartNum + 1);
        }
        nTablePos++;  // �����λ���Լ�
    }

    // �������������
    for (i = 0; i < nActNo; ++i)
    {
        // �����ȫʹ��Ĭ��ֵ�������ٴ���
        if (m_sActTableOff[i].nActOff == 0)
            continue;
        // �õ�������
        if (!cActionName->GetActionName(i, szActName, sizeof(szActName)))
            continue;
        if (!pIni->IsSectionExist(szActName))
            continue;  // ���û���������

        // �������ʹ��Ĭ��ֵ�����뷽��˳��
        if (m_sActTableOff[i].bUseDefault == FALSE)
        {
            for (j = 0; j < MAX_SORT_DIR; ++j)
            {
                sprintf(szKey, "Dir%d", j + 1);
                if (pIni->GetString(szActName, szKey, "", szBuffer, sizeof(szBuffer)))
                {
                    SortStrToNum(szBuffer, &m_lpnSortTable[nTablePos * (m_nPartNum + 1)], m_nPartNum + 1);
                }
                else
                {
                    memcpy(&m_lpnSortTable[nTablePos * (m_nPartNum + 1)], &m_lpnSortTable[j * (m_nPartNum + 1)],
                           sizeof(int) * (m_nPartNum + 1));
                }
                nTablePos++;
            }
        }
        // ����������˳��
        if ((m_sActTableOff[i].bUseDefault == FALSE && m_sActTableOff[i].nLineNum > MAX_SORT_DIR) ||
            (m_sActTableOff[i].bUseDefault == TRUE && m_sActTableOff[i].nLineNum > 0))
        {
            j = 1;
            while (1)
            {
                sprintf(szKey, "Line%d", j);
                if (!pIni->GetString(szActName, szKey, "", szBuffer, sizeof(szBuffer)))
                    break;
                SortStrToNum(szBuffer, &m_lpnSortTable[nTablePos * (m_nPartNum + 1)], m_nPartNum + 1);
                nTablePos++;
                j++;
            }
        }
    }
    // ---------------------------- �������Ķ�Ӧ������ end --------------------------

    return TRUE;
}

//---------------------------------------------------------------------------
// ����:	�ӱ����еõ�һ��������Ϣ
// ����:
//			nActNo : �������
//			nDir : ��ǰ���� ( 0 -- 16 )
//			nFrameNo : ��ǰ�ڼ���
//			lpnTable : ����������Ϣ������
//			nTableLen : lpnTable�ĳ���
// ����:	��ȡ�ɹ����
//---------------------------------------------------------------------------
int CSortTable::GetSort(int nActNo, int nDir, int nFrameNo, int* lpnTable, int nTableLen)
{
    if (!lpnTable)
        return FALSE;
    if (nActNo < 0 || nActNo >= MAX_ACTION)
        return FALSE;
    if (nDir < 0 || nDir >= MAX_SORT_DIR)
        return FALSE;
    if (!m_lpnSortTable)
        return FALSE;

    int i;

    // ��ȫʹ��Ĭ��
    if (m_sActTableOff[nActNo].nActOff == 0)
    {
        if (nTableLen <= m_nPartNum)
            memcpy(lpnTable, &m_lpnSortTable[nDir * (m_nPartNum + 1) + 1], sizeof(int) * nTableLen);
        else
        {
            memcpy(lpnTable, &m_lpnSortTable[nDir * (m_nPartNum + 1) + 1], sizeof(int) * m_nPartNum);

            for (i = m_nPartNum; i < nTableLen; ++i)  // ���������λ�ľͲ��滭
                lpnTable[i] = -1;                     // ��λ���滭
        }
    }
    else
    {
        // ����ʹ��Ĭ�ϣ���������
        if (m_sActTableOff[nActNo].bUseDefault == TRUE)
        {
            // �ж��Ƿ���������
            for (i = 0; i < m_sActTableOff[nActNo].nLineNum; ++i)
            {
                // �ҵ�������
                if (nFrameNo == m_lpnSortTable[(m_sActTableOff[nActNo].nActOff + i) * (m_nPartNum + 1)])
                {
                    if (nTableLen <= m_nPartNum)
                        memcpy(lpnTable, &m_lpnSortTable[(m_sActTableOff[nActNo].nActOff + i) * (m_nPartNum + 1) + 1],
                               sizeof(int) * nTableLen);
                    else
                    {
                        memcpy(lpnTable, &m_lpnSortTable[(m_sActTableOff[nActNo].nActOff + i) * (m_nPartNum + 1) + 1],
                               sizeof(int) * m_nPartNum);
                        for (int j = m_nPartNum; j < nTableLen; ++j)
                            lpnTable[j] = -1;
                    }
                    return TRUE;
                }
            }
            // ���ˣ�û���ҵ�������
            if (nTableLen <= m_nPartNum)
                memcpy(lpnTable, &m_lpnSortTable[nDir * (m_nPartNum + 1) + 1], sizeof(int) * nTableLen);
            else
            {
                memcpy(lpnTable, &m_lpnSortTable[nDir * (m_nPartNum + 1) + 1], sizeof(int) * m_nPartNum);
                for (i = m_nPartNum; i < nTableLen; ++i)
                    lpnTable[i] = -1;
            }
        }
        // ����ʹ��Ĭ��
        else
        {
            // �ж��Ƿ���������
            for (i = MAX_SORT_DIR; i < m_sActTableOff[nActNo].nLineNum; ++i)
            {
                // �ҵ�������
                if (nFrameNo == m_lpnSortTable[(m_sActTableOff[nActNo].nActOff + i) * (m_nPartNum + 1)])
                {
                    if (nTableLen <= m_nPartNum)
                        memcpy(lpnTable, &m_lpnSortTable[(m_sActTableOff[nActNo].nActOff + i) * (m_nPartNum + 1) + 1],
                               sizeof(int) * nTableLen);
                    else
                    {
                        memcpy(lpnTable, &m_lpnSortTable[(m_sActTableOff[nActNo].nActOff + i) * (m_nPartNum + 1) + 1],
                               sizeof(int) * m_nPartNum);
                        for (int j = m_nPartNum; j < nTableLen; ++j)
                            lpnTable[j] = -1;
                    }
                    return TRUE;
                }
            }
            // ���ˣ�û���ҵ�������
            if (nTableLen <= m_nPartNum)
                memcpy(lpnTable, &m_lpnSortTable[(m_sActTableOff[nActNo].nActOff + nDir) * (m_nPartNum + 1) + 1],
                       sizeof(int) * nTableLen);
            else
            {
                memcpy(lpnTable, &m_lpnSortTable[(m_sActTableOff[nActNo].nActOff + nDir) * (m_nPartNum + 1) + 1],
                       sizeof(int) * m_nPartNum);
                for (i = m_nPartNum; i < nTableLen; ++i)
                    lpnTable[i] = -1;
            }
        }
    }

    return TRUE;
}

//-------------------------- class CSortTable end ---------------------------
