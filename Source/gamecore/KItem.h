//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KItem.h
// Date:	2002.08
// Code:	LiXueWu, DongBo
// Desc:	Header File
//---------------------------------------------------------------------------

#ifndef KItemH
#define KItemH

#include "KBasPropTbl.h"
#include "KMagicAttrib.h"
#include "GameDataDef.h"
#include "engine/KTabFile.h"
#include "KRUImage.h"

class KPlayer;
class KIniFile;

#define IN
#define OUT

#define MAX_ITEM                 1024  // 150000

#define KItemNormalAttrib        KMagicAttrib

#define DESCMAXLEN               516  // ���͵��ַ�������������

#define TABFILE_MAGICATTRIB_PATH "\\settings\\item\\004\\magicattrib.txt"  // ��װħ������
#define TABFILE_MAGICAGOLD_PATH  "\\settings\\item\\004\\GoldMagic.txt"    // �ƽ�ħ������
#define TABFILE_MAGICALEVEL_PATH "\\Settings\\item\\004\\magicattriblevel_index.txt"
#define TABFILE_FUSION_PATH      "\\Settings\\item\\004\\fusion.txt"
#define TABFILE_BIANSHI_PATH     "\\Settings\\item\\004\\bianshi.txt"
#define JIACHENG_VAL             2    // ǿ������Сֵ	18
#define MAX_STACKNUM             200  // ���͵��ַ�������������  nStackNum

enum ITEMGENRE
{
    item_equip = 0,   // װ��
    item_medicine,    // ҩƷ
    item_mine,        // ��ʯ
    item_materials,   // ҩ��
    item_task,        // ����
    item_townportal,  // ������
    item_fusion,      // �Ƹ�
    item_number,      // ������Ŀ
};

enum EQUIPDETAILTYPE
{
    equip_meleeweapon = 0,
    equip_rangeweapon,
    equip_armor,
    equip_ring,
    equip_amulet,
    equip_boots,
    equip_belt,
    equip_helm,
    equip_cuff,
    equip_pendant,
    equip_horse,
    equip_mask,  // mat na
    equip_pifeng,
    equip_yinjian,
    equip_shiping,
    equip_detailnum,
};

enum MEDICINEDETAILTYPE
{
    medicine_blood = 0,   // ��ҩ������  Ѫ
    medicine_mana,        // ��
    medicine_both,        // Ѫ����
    medicine_stamina,     // ����
    medicine_antipoison,  //
    medicine_allboth = 8,
    medicine_detailnum,
};
// װ���Ĺ�ͬ����
typedef struct
{
    int nItemGenre;       // �������� (����? ҩƷ? ��ʯ?)
    int nDetailType;      // �ڸ������еľ������
    int nParticularType;  // ��ϸ���
    int nObjIdx;          // ��ͼ�ϰڷ�ʱ��Ӧ��������ݱ��
    int bStack;           // �Ƿ�ɵ���
    int nWidth;           // ����������ռ���
    int nHeight;          // ����������ռ�߶�
    int nPrice;           // �̵깺��۸�
    int nLevel;           // �ȼ�
    int nSeries;          // ��������
    char szItemName[80];  // ����
    // const char*	szItemName;
    char szWonName[32];  // ����������
    // const char*	szWonName;
    int nColor;       // ��ɫ
    int nLianjieFlg;  // ����״̬
    int nCanUse;      // �Ƿ����ʹ��
    BYTE nSellModel;  // ��̯���׵�ģʽ
    // �ͻ���
    // char	szImageName[80];		 // �����еĶ����ļ���
    const char* szImageName;
    char szIntro[DESCMAXLEN];  // ˵������  ��Ʒ��˵������
    // const char* 	szIntro;
    int uPrice;        // ��̯����۸�
    BYTE uLianjieFlg;  // �Ƿ���������
                       //	int     uCanUse;                 // �Ƿ����ʹ��
    BYTE ncSellModel;  // ��̯���׵�ģʽ

    // char    ItmeInfo[DESCMAXLEN];    // ˵������
    const char* ItmeInfo;
    // char	szScript[256];			 // ִ�нű�
    const char* szScript;
    int nSet;         // ��װ
    int nSetId;       // ��ǰ ������װ��Id
    int nSetNum;      // ��װ����
    int nBigSet;      // ��װ��չ
    int nGoldId;      // �ƽ�Id
    int nIsPlatina;   // �Ƿ�׽�װ
    int nStackNum;    // ��������
    int nEnChance;    // ǿ��
    int nPoint;       // ��װ��
    KTime LimitTime;  // ��ʱ
    int nIsSell;      // �Ƿ��������
    int nPriceXu;     // ���
    int nJiFen;       // ����
    int nIsTrade;     // �Ƿ���Խ���
    int nIsDrop;      // �Ƿ���Զ���
    //    int	    nYingNuma;                //��������1
    //    int	    nYingNumb;                //��������2
    int nRongNum;          // ������������
    int nWengangPin;       // �����Ƹ�Ʒ��
    int nBinfujiazhi;      // ������ֵ
    int nRongpiont;        // ������
    int nIsBang;           // �Ƿ��
    int nIsKuaiJie;        // �Ƿ�����
    int nSkillType;        // ��������
    int nISMagic;          // �Ƿ������
    int nMagicID;          // �Ƹֵļ���ID
    int nIsUse;            // �Ƿ������ʹ��
    int nIsWhere;          // �ǵڼ���
    int nChiBangRes;       // ����������
    int nParticularTypea;  // ����2
    int nUseMap;
    int nSixSkill;
    int nTenSkill;
    int nRes;      // �ı��װ�������
    int nUseKind;  // ʹ�õ�����

} KItemCommonAttrib;
// װ��ħ���������ɵĺ��Ĳ���
typedef struct
{
    UINT uRandomSeed;         // �������
    int nGeneratorLevel[6];   // ħ��
    int nRGeneratorLevel[6];  // ����
    // int	nBGeneratorLevel[6]; // ��ʯ
    int nJGeneratorLevel[7];  // ��������
    int nVersion;             // �汾
    int nLuck;                // ����ֵ

    //	UINT   uItemRandomSeed[12];  //����ÿ�����Ե� ������� 0-5 Ϊ ��װ,5-10,����
} KItemGeneratorParam;

class KNpc;

class KItem
{
public:
    KItem();
    ~KItem();

    // �����Ǻ��ĳ�Ա����
private:
    KItemCommonAttrib m_CommonAttrib;         // ���ֵ��ߵĹ�ͬ����
    KItemNormalAttrib m_aryBaseAttrib[7];     // ���ߵĻ�������
    KItemNormalAttrib m_aryRequireAttrib[6];  // ���ߵ���������
    KItemNormalAttrib m_aryMagicAttrib[6];  // ���ߵ�ħ������  //��������Ե��ַ���
    KItemNormalAttrib m_ronMagicAttrib[6];  // ���ߵ���������
    //	KItemNormalAttrib	m_BaoShiMagicAttrib[6];	// ���ߵı�ʯ����
    KItemNormalAttrib m_yinMagicAttrib[2];  // ���ߵ���������
    KItemNormalAttrib
        m_TempPlatinaAttrib[6];  // ��ʱ�İ׽�ħ������ֵ�����ڸı����ԣ�
    KItemNormalAttrib m_TempMagicAttrib[6];   // ��ʱ��ħ������ֵ������ǿ����
    KItemNormalAttrib m_TempRMagicAttrib[6];  // ��ʱ�ı�ʶ�Ƹ�����ֵ������ǿ����
    //	KItemNormalAttrib	m_TempBMagicAttrib[6];  // ��ʱ�ı�ʯ����ֵ������ǿ����
    // �����Ǹ����Եĳ�Ա����
public:
    KItemGeneratorParam m_GeneratorParam;  // ���ߵ����ɲ���
private:
    int m_nIndex;          // �����ڵ��������е�����
    unsigned long m_dwID;  // ������ID�����ڿͻ�����������˵Ľ���
    int m_nCurrentDur;     // ��ǰ�;ö�
    KRUImage m_Image;
    // �����Ƕ���ӿ�
public:
    void ApplyMagicAttribToNPC(IN KNpc*, IN int = 0, IN int* nType = NULL) const;
    void RemoveMagicAttribFromNPC(IN KNpc*, IN int = 0, IN int* nType = NULL) const;
    void ApplyHiddenMagicAttribToNPC(IN KNpc*, IN int) const;
    void RemoveHiddenMagicAttribFromNPC(IN KNpc*, IN int) const;
    KItemGeneratorParam* GetItemParam() { return &m_GeneratorParam; };
    void SetID(unsigned long dwID) { m_dwID = dwID; };
    unsigned long GetID() const { return m_dwID; };                    // unsigned long CHECK O DAY
    int GetLianjieFlg() const { return m_CommonAttrib.nLianjieFlg; };  // ���ӱ�ʾ
    int GetIsCanUse() const { return m_CommonAttrib.nCanUse; };        // ��ȡ�Ƿ���ʹ��
    void SetIsCanUse(int uCanUse) { m_CommonAttrib.nCanUse = uCanUse; };
    int ExtractChars(const char* inStr, int scrLen, char* outStr);
    int GetDetailType() const { return m_CommonAttrib.nDetailType; };
    int GetGenre() const { return m_CommonAttrib.nItemGenre; };
    int GetUseMapIdx() const { return m_CommonAttrib.nUseMap; };
    void SetUseMapIdx(int nUseMap) { m_CommonAttrib.nUseMap = nUseMap; };

    int GetUseKind() const { return m_CommonAttrib.nUseKind; };
    void SetUseKind(int nUseKind)
    {
        if (nUseKind < 0)
            nUseKind = 0;
        if (nUseKind > 1)
            nUseKind = 1;
        m_CommonAttrib.nUseKind = nUseKind;
    }

    int GetItemResIdx() const { return m_CommonAttrib.nRes; };
    void SetItemResIdx(int nResIdx) { m_CommonAttrib.nRes = nResIdx; };

    int GetIsUse() const { return m_CommonAttrib.nIsUse; };
    int GetIsSell() const { return m_CommonAttrib.nIsSell; };
    int GetIsTrade() const { return m_CommonAttrib.nIsTrade; };
    int GetIsDrop() const { return m_CommonAttrib.nIsDrop; };

    int GetParticular() { return m_CommonAttrib.nParticularType; };
    int GetParticular2() { return m_CommonAttrib.nParticularTypea; };
    int GetChiBangRes() { return m_CommonAttrib.nChiBangRes; };
    void SetChiBangRes(int nRes) { m_CommonAttrib.nChiBangRes = nRes; };
    // int	GetLParticular() { return m_CommonAttrib.nParticularType; };
    int GetLevel() { return m_CommonAttrib.nLevel; };
    void SetSeries(int nSeries) { m_CommonAttrib.nSeries = nSeries; };
    int GetSeries() const { return m_CommonAttrib.nSeries; };
    // int		GetWidth() const { return m_CommonAttrib.nWidth; };
    // int		GetHeight() const { return m_CommonAttrib.nHeight; };
    int GetWidth() const { return 1; };
    int GetHeight() const { return 1; };
    int GetPrice() const { return m_CommonAttrib.nPrice; };
    int GetPriceXu() const { return m_CommonAttrib.nPriceXu; };
    char* GetName() const { return (char*)m_CommonAttrib.szItemName; };
    char* GetImagePath() const { return (char*)m_CommonAttrib.szImageName; };
    char* GetWonName() const { return (char*)m_CommonAttrib.szWonName; };
    void SetWonName(const char* strWonName) { sprintf(m_CommonAttrib.szWonName, "%s", strWonName); };  // �����ù���
    int GetObjIdx() { return m_CommonAttrib.nObjIdx; };
    void* GetRequirement(IN int);
    int CheckRequirement(IN int nReq);
    int GetMaxDurability();
    int GetTotalMagicLevel();
    int GetRepairPrice();  // ��ȡ������ܼ۸�
    void Remove();
    void SetIndex(int i);
    int GetIndex() { return m_nIndex; };
    int SetBaseAttrib(IN const KItemNormalAttrib*);
    int SetRequireAttrib(IN const KItemNormalAttrib*);
    int SetMagicAttrib(IN const KItemNormalAttrib*);
    void SetDurability(IN const int nDur) { m_nCurrentDur = nDur; };  // ���ó־ö�
    int GetDurability() { return m_nCurrentDur; };                    // ���ص�ǰ�־ö�
    // KItemGeneratorParam*	GetGeneratorParam() { return &m_GeneratorParam; }
    int Abrade(IN const int nRange);
    int CanBeRepaired();                                                  // ����Ƿ����޸��־ö�
    int GetIsBang() const { return m_CommonAttrib.nIsBang; };             // �Ƿ��
    void SetBang(int nVal = 0) { m_CommonAttrib.nIsBang = nVal; };        // ���ð�
    int GetIsKuaiJie() const { return m_CommonAttrib.nIsKuaiJie; };       // �Ƿ�����
    void SetKuaiJie(int nVal = 0) { m_CommonAttrib.nIsKuaiJie = nVal; };  // ���ÿ����
    int GetIsMagic() const { return m_CommonAttrib.nISMagic; };           // �Ƿ�ħ������
    void SetMagic(int nVal = 0)
    {
        m_CommonAttrib.nISMagic = nVal;
    };  //() const { return m_CommonAttrib.nISMagic; };      //�Ƿ�ħ������

    int GetSkillType() const { return m_CommonAttrib.nSkillType; };  // �Ƹ��ڿ��Ƹ������б���к�m_SkillType
    void SetSkillType(int nVal = 0) { m_CommonAttrib.nSkillType = nVal; };

    int GetMagicID() const { return m_CommonAttrib.nMagicID; };  // ��Ʒ�����ļ���id
    void SetMagicID(int nVal = 0) { m_CommonAttrib.nMagicID = nVal; };

    int GetSet() const { return m_CommonAttrib.nSet; };        // �Ƿ���װװ��
    int GetSetId() const { return m_CommonAttrib.nSetId; };    // ��װID
    int GetSetNum() const { return m_CommonAttrib.nSetNum; };  // ��װ����
    int GetGoldId() const { return m_CommonAttrib.nGoldId; };
    void SetGoldId(int nId) { m_CommonAttrib.nGoldId = nId; };

    int GetIsPlatima() const { return m_CommonAttrib.nIsPlatina; };
    void SetPlatima(int nId) { m_CommonAttrib.nIsPlatina = nId; };

    // void    SetSItmeInfo(char *SItemInfo)
    // {ZeroMemory(m_CommonAttrib.ItmeInfo,sizeof(m_CommonAttrib.ItmeInfo));_snprintf(m_CommonAttrib.ItmeInfo,sizeof(m_CommonAttrib.ItmeInfo)-1,"%s",SItemInfo);};
    // //������˵��
    void SetSItmeInfo(char* SItemInfo) { m_CommonAttrib.ItmeInfo = SItemInfo; };
    char* GetSItmeInfo() const { return (char*)m_CommonAttrib.ItmeInfo; };
    void SetItemInfo(int nVale);
    int GetColor() const { return m_CommonAttrib.nColor; };
    int GetBigSet() const { return m_CommonAttrib.nBigSet; };
    int GetRongNum() const { return m_CommonAttrib.nRongNum; };        // ����������
    int GetWengangPin() const { return m_CommonAttrib.nWengangPin; };  // ��ȡ�Ƹٵ�Ʒ��
    int GetBinfujiazhi() const { return m_CommonAttrib.nBinfujiazhi; };
    int GetUsedRong()
    {
        int mRmun = 0;
        for (int i = 0; i < 6; i++)
        {
            if (m_ronMagicAttrib[i].nAttribType > 0)
                mRmun++;
        }
        return mRmun;
    };

    int GetMagicCount()
    {
        int mRmun = 0;
        for (int i = 0; i < sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); i++)
        {
            if (m_aryMagicAttrib[i].nAttribType > 0)
                mRmun++;
        }
        return mRmun;
    };

    int getBaseCount()
    {
        int ncount = 0;
        for (int i = 0; i < sizeof(m_aryBaseAttrib) / sizeof(m_aryBaseAttrib[0]); i++)
        {
            if (m_aryBaseAttrib[i].nAttribType > 0)
                ncount++;
        }
        return ncount;
    };
    int getReqCount()
    {
        int ncount = 0;
        for (int i = 0; i < sizeof(m_aryRequireAttrib) / sizeof(m_aryRequireAttrib[0]); i++)
        {
            if (m_aryRequireAttrib[i].nAttribType != 0)
                ncount++;
        }
        return ncount;
    };
    int IsPurple();
    int GetPoint();
    int GetIsWhere();
    int CheckReqability(int n);
    int IsRong();
    int IsBlue();
    int getItemKind();
    int IsGold();
    int IsPlatina();
    int IsReceptacle();  // ����Ƿ�����
    int HoldItem(int nIdx, int nWidth, int nHeight);
    int Stack(int nIdx);  // ����
    int CheckItemInfo(int nGen, int nDetail, int nParticular, const char* KeyName);
    int CanStack(int nOldIdx);
    int IsStack() const { return m_CommonAttrib.bStack; };  // �Ƿ���Ե���
    int GetStackNum() const { return m_CommonAttrib.nStackNum; };
    void SetStackNum(int nNum) { m_CommonAttrib.nStackNum = nNum; };  // �ɵ��ӵ�����
    void SetPerXu(int nPerXu) { m_CommonAttrib.nPriceXu = nPerXu; };
    void SetPerPrice(int nmPrice) { m_CommonAttrib.nPrice = nmPrice; };

    int GetEnChance() const { return m_CommonAttrib.nEnChance; };
    int EnChance(int nEnChance = 0);
    int EnChanceRong(int nEnChance = 0);
    int EnChanceBao(int nEnChance = 0);  // ��ʯ�ӳ�
    void SetBackUpMagicAttrib();
    void SetBackUpBmagicAttrib();
    void SetBackUpRmagicAttrib();
    int CheckEnChance(const char* nKey, int nAttribType);
    KMagicAttrib getaryRequireAttrib(int i) { return m_aryRequireAttrib[i]; };
    int GetItemReqType(int i = 0) const { return m_aryRequireAttrib[i].nAttribType; };
    int GetItemReqMagic(int i) const { return m_aryRequireAttrib[i].nValue[0]; };  // ��ȡ��������
    KMagicAttrib getaryBaseAttrib(int i) { return m_aryBaseAttrib[i]; };
    int GetItemBaseType(int i = 0) const { return m_aryBaseAttrib[i].nAttribType; };
    int GetItemBaseMagic(int i) const { return m_aryBaseAttrib[i].nValue[0]; };   // ��ȡ��������
    int GetItemBaseMagicA(int i) const { return m_aryBaseAttrib[i].nValue[1]; };  // ��ȡ��������
    int GetItemBaseMagicB(int i) const { return m_aryBaseAttrib[i].nValue[2]; };  // ��ȡ��������
    int GetBaseMagic() const { return m_aryBaseAttrib[0].nValue[0]; };            // ��ȡ��������

    char* GetScript() const { return (char*)m_CommonAttrib.szScript; };  // ��ȡִ�еĽű�
    void SetLevel(int i) { m_CommonAttrib.nLevel = i; };
    KMagicAttrib getaryMagicAttrib(int i) { return m_aryMagicAttrib[i]; };

    int getmagicAttribCount() { return sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); };
    int getbaseAttribCount() { return sizeof(m_aryBaseAttrib) / sizeof(m_aryBaseAttrib[0]); };
    int getrequireAttribCount() { return sizeof(m_aryRequireAttrib) / sizeof(m_aryRequireAttrib[0]); };

    int GetAttribType(int i = 0) const { return m_aryMagicAttrib[i].nAttribType; };  // ��ȡħ����������
    int GetAttribTypeVala(int i = 0) const { return m_aryMagicAttrib[i].nValue[0]; };  // ��ȡħ����������
    int GetAttribTypeValb(int i = 0) const { return m_aryMagicAttrib[i].nValue[1]; };  // ��ȡħ����������
    int GetAttribTypeValc(int i = 0) const { return m_aryMagicAttrib[i].nValue[2]; };  // ��ȡħ����������

    void SetAttribType(int i, int nType) { m_aryMagicAttrib[i].nAttribType = nType; };  // ��ȡħ����������
    void SetAttribTypeAndVal(int i, int nType, int nVal)
    {
        m_aryMagicAttrib[i].nAttribType = nType;
        m_aryMagicAttrib[i].nValue[0]   = nVal;
    };
    int GetRongAttribType(int i = 0) const { return m_ronMagicAttrib[i].nAttribType; };  // ���͵õ��������Ե�
    int GetRongAttribTypeVala(int i = 0) const { return m_ronMagicAttrib[i].nValue[0]; };
    int GetRongAttribTypeValb(int i = 0) const { return m_ronMagicAttrib[i].nValue[1]; };
    int GetRongAttribTypeValc(int i = 0) const { return m_ronMagicAttrib[i].nValue[2]; };

    void SetRongAttribType(int i, int nType) { m_ronMagicAttrib[i].nAttribType = nType; };  // ���͵õ��������Ե�
    void SetRongTypeAndVal(int i, int nType, int nVal)
    {
        m_ronMagicAttrib[i].nAttribType = nType;
        m_ronMagicAttrib[i].nValue[0]   = nVal;
    };
    int GetRongTypeVal(int i = 0) const { return m_ronMagicAttrib[i].nValue[0]; };
    //	int		GetBaoShiAttribType( int i = 0) const { return m_BaoShiMagicAttrib[i].nAttribType; }; //
    //���͵õ��������Ե�
    void SetPoint(int i) { m_CommonAttrib.nPoint = i; };
    void SetIsWhere(int i) { m_CommonAttrib.nIsWhere = i; };
    void SetRPoint(int i) { m_CommonAttrib.nRongpiont = i; };
    void SetGoodPrice(int uPrice) { m_CommonAttrib.nPrice = uPrice; };
    void SetTime(int bYear = 0, int bMonth = 0, int bDay = 0, int bHour = 0, int bMin = 0);
    void SetItemMagicLevel(int i, int nRow);  // ���õ�һ�����Ե�ֵ ��Ҫ������������
    int GetItemMagicLevel(int i);
    KTime* GetTime() { return &m_CommonAttrib.LimitTime; };
    int HaveMaigc(int nAttribe,
                  int nValue1Min,
                  int nValue1Max,
                  int nValue2Min,
                  int nValue2Max,
                  int nValue3Min,
                  int nValue3Max);
    int CheckWenPin(int pShuXingID = 0, int pVal = 0);
    int CheckBluePin(int pShuXingID = 0, int pVal = 0);
    int GeteventID(int pShuXingID = 0);
    int SetAttrib_RON(IN const KItemNormalAttrib*);  // ��������
    int SetAttrib_Bao(IN const KItemNormalAttrib*);  // ��ʯ����
    int SetAttrib_MA(IN const KItemNormalAttrib*);
    int SetAttrib_Yin(IN const KItemNormalAttrib*);
    // �ͻ���
    void SetPrice(int uPrice) { m_CommonAttrib.uPrice = uPrice; };                       // ��̯�۸�
    void SetModel(BYTE uPrice) { m_CommonAttrib.ncSellModel = uPrice; };                 // ���������
    BYTE GetcModel() { return m_CommonAttrib.ncSellModel; };                             // ���������
    void SetLianjieFlg(BYTE uLianjieFlg) { m_CommonAttrib.uLianjieFlg = uLianjieFlg; };  // ���ӱ�ʾ
    // void	SetIsCanUse(int uCanUse) {m_CommonAttrib.nCanUse = uCanUse;};               //������ʱ���Ƿ���ʹ��
    void SetItmeInfo(char* ItemInfo)
    {
        memset(m_CommonAttrib.szIntro, 0, sizeof(m_CommonAttrib.szIntro));
        sprintf(m_CommonAttrib.szIntro, "%s", ItemInfo);
    };  // ������˵��
    //	void    SetItmeInfo(char *ItemInfo) {m_CommonAttrib.szIntro = ItemInfo;};
    char* GetItmeInfo() const { return (char*)m_CommonAttrib.szIntro; };
    int GetSetPrice() { return m_CommonAttrib.uPrice; };
    void Paint(int nX, int nY, int bStack = TRUE, int nzStackNum = 0, int nKind = -1);  // Ĭ�������
    // ��ȡ����
    void GetDesc(char* pszMsg, bool bShowPrice = false, int nPriceScale = 1, int nActiveAttrib = 0, int nType = 0);

    // �����Ǹ����ӿ�
    friend class KItemGenerator;
    friend class KPlayer;
    friend class KItemList;

private:
    int SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT*);
    int SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT_GOLD*);
    int SetAttrib_RCBR(IN const KBASICPROP_EQUIPMENT_GOLD*, int nIsDel = FALSE);
    int SetAttrib_BCBR(IN const KBASICPROP_EQUIPMENT_PLATINA*);

    //	int SetAttrib_MA(IN const KItemNormalAttrib*);
    //	int SetAttrib_RON(IN const KItemNormalAttrib*); //��������
    //	int SetAttrib_Bao(IN const KItemNormalAttrib*); //��ʯ����
    int SetAttrib_MAB(IN const KMACP*);
    int SetAttrib_MAC(IN const int*);
    void operator=(const KBASICPROP_EQUIPMENT&);  //
    void operator=(const KBASICPROP_MEDMATERIAL&);
    void operator=(const KBASICPROP_MINE&);
    void operator=(const KBASICPROP_MEDICINE&);
    void operator=(const KBASICPROP_QUEST&);
    void operator=(const KBASICPROP_TOWNPORTAL&);
    void operator=(const KBASICPROP_EQUIPMENT_UNIQUE&);

    void operator=(const KBASICPROP_EQUIPMENT_PLATINA&);
    void operator=(const KBASICPROP_EQUIPMENT_GOLD&);
    void operator=(const KBASICPROP_FUSION&);
    int Gen_Equipment_Unique(const KBASICPROP_EQUIPMENT*, const KBASICPROP_EQUIPMENT_UNIQUE*);

    // �����Ǹ�������
private:
    int SetAttrib_Base(const KEQCP_BASIC*);
    int SetAttrib_Req(const KEQCP_REQ*, int inDel = FALSE);
};

extern KItem* Item;  // Item[MAX_ITEM];

#endif
