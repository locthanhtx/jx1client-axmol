//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KBasPropTbl.h
// Date:	2002.08.14
// Code:	DongBo
// Desc:    header file. ���ļ�����������ڴ�tab file�ж������ߵĳ�ʼ����,
//			�����ɶ�Ӧ�����Ա�
//---------------------------------------------------------------------------

#ifndef	KBasPropTblH
#define	KBasPropTblH

#define		SZBUFLEN_0	128		    // ���͵��ַ�������������
#define		SZBUFLEN_1	516		    // ���͵��ַ�������������
#define		SCBUFLEN_1	300		    // ���͵��ַ�������������


//#define		MAX_MAGIC_PREFIX	20
//#define		MAX_MAGIC_SUFFIX	20
// ���½ṹ����������ʯ�Ļ�������. ��������������ļ�(tab file)�ṩ
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nItemGenre;				// ��������
	int			m_nDetailType;				// �������
	int			m_nParticularType;			// ��ϸ���
	char		m_szImageName[SZBUFLEN_0];	// �����еĶ����ļ���
	int			m_nObjIdx;					// ��Ӧ�������
	int			m_nWidth;					// ����������ռ����
	int			m_nHeight;					// ����������ռ�߶�
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	int			m_nSeries;					// ��������
	int			m_nPrice;					// �۸�
	int			m_nPriceXu;	                //���
	int			m_nLevel;					// �ȼ�
	int		m_bStack;					// �Ƿ�ɵ���
	char		m_szScript[SCBUFLEN_1];		// ִ�нű�
	int			m_Magic[6];
	int			m_nDelet;
	int	        m_IsBang;
    int	        m_IsKuaiJie;
    int	        m_IsMagic;
    int	        m_IsUse;
} KBASICPROP_MINE;

// ���½ṹ��������ҩƷ���Ե����ԣ���ֵ��ʱ��
typedef struct
{
	int			nAttrib;
	int			nValue;
	int			nTime;
} KMEDATTRIB;

// ���½ṹ��������ҩƷ�Ļ�������. ��������������ļ�(tab file)�ṩ
// ����������ҩƷ: ����������,����������,����������,��ҩ��,�ⶾ��,
//					��ȼ����,�������
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nItemGenre;				// ��������
	int			m_nDetailType;				// �������
	int			m_nParticularType;			// ��ϸ���
	char		m_szImageName[SZBUFLEN_0];	// �����еĶ����ļ���
	int			m_nObjIdx;					// ��Ӧ�������
	int			m_nWidth;					// ����������ռ����
	int			m_nHeight;					// ����������ռ�߶�
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	int			m_nSeries;					// ��������
	int			m_nPrice;					// �۸�
	int			m_nPriceXu;
	int			m_nLevel;					// �ȼ�
	int		m_bStack;					// �Ƿ�ɵ���
	KMEDATTRIB	m_aryAttrib[6];				// ҩƷ������
	int	        m_IsBang;
	int	        m_IsKuaiJie;
	int	        m_IsMagic;
	int         m_UseMap;
} KBASICPROP_MEDICINE;

// ���½ṹ��������ҩ�ĵĻ�������. ��������������ļ�(tab file)�ṩ
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nItemGenre;				// ��������
	int			m_nDetailType;				// �������
	int			m_nParticularType;			// ��ϸ���
	char		m_szImageName[SZBUFLEN_0];	// �����еĶ����ļ���
	int			m_nObjIdx;					// ��Ӧ�������
	int			m_nWidth;					// ����������ռ����
	int			m_nHeight;					// ����������ռ�߶�
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	int			m_nSeries;					// ��������
	int			m_nPrice;					// �۸�
	int			m_nPriceXu;
	int			m_nLevel;					// �ȼ�
	int		m_bStack;					// �Ƿ�ɵ���
	int			m_nAttrib1_Type;			// ����1����
	int			m_nAttrib1_Para;			// ����1����
	int			m_nAttrib2_Type;			// ����2����
	int			m_nAttrib2_Para;			// ����2����
	int			m_nAttrib3_Type;			// ����2����
	int			m_nAttrib3_Para;			// ����2����
	int	        m_IsBang;
	int	        m_IsKuaiJie;
	int	        m_IsMagic;
} KBASICPROP_MEDMATERIAL;

// ���½ṹ��������һ�����,��Сֵ
typedef struct
{
	int			nMin;
	int			nMax;
} KMINMAXPAIR;

// ���½ṹ���ڸ���װ���ĺ��Ĳ���: ��������
typedef struct
{
	int			nType;						// ��������
	KMINMAXPAIR	sRange;						// ȡֵ��Χ
} KEQCP_BASIC;	// Equipment_CorePara_Basic

// ���½ṹ���ڸ���װ���ĺ��Ĳ���: ��������
typedef struct
{
	int			nType;						// ��������
	int			nPara;						// ��ֵ
} KEQCP_REQ;	// Equipment_CorePara_Requirment

// ���½ṹ���ڸ���ħ���ĺ��Ĳ���
typedef struct
{
	int			nPropKind;					// �޸ĵ��������ͣ���ͬһ����ֵ�ӰٷֱȺͼӵ�������Ϊ���������ԣ�
	KMINMAXPAIR	aryRange[3];				// �޸���������ļ�������
} KMACP;	// MagicAttrib_CorePara

// ���½ṹ�������������ļ��и�����ħ������. ��������������ļ�(tab file)�ṩ
// Add by Freeway Chen in 2003.5.30
#define			MATF_CBDR		    15      // ��Ʒ���� +1 �ݴ� type(���ڵ�ֵΪ equip_detailnum)  װ�����Կɳ��ֲ�λ��������
#define         MATF_PREFIXPOSFIX   2       // ǰ׺��׺
#define         MATF_SERIES         5       // ����
#define         MATF_LEVEL          10      // ħ�����������10������

typedef struct
{
	int			m_nPos;						// ǰ׺���Ǻ�׺
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nClass;					// ����Ҫ��
	int			m_nLevel;					// �ȼ�Ҫ��
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	KMACP		m_MagicAttrib;				// ���Ĳ���
	int			m_DropRate[MATF_CBDR];		// ���ָ���
    //add by Freeway Chen in 2003.5.30
    int         m_nUseFlag;                 // ��ħ���Ƿ�ʹ�ù�
} KMAGICATTRIB_TABFILE;

// ���½ṹ��������ħ������. ��������������ļ�(tab file)�ṩ
/*
typedef struct
{
	int			m_nPos;						// ǰ׺���Ǻ�׺
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nClass;					// ����Ҫ��
	int			m_nLevel;					// �ȼ�Ҫ��
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	KMACP		m_MagicAttrib;				// ���Ĳ���
	int			m_DropRate;					// ���ָ���
} KMAGICATTRIB;
*/
// ���½ṹ��������װ���ĳ�ʼ����. ��������������ļ�(tab file)�ṩ====����װ������װ��
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nItemGenre;				// �������� (����? ҩƷ? ��ʯ?)
	int			m_nDetailType;				// �������
	int			m_nParticularType;			// ��ϸ���
	int         m_nParticularTypea;
	char		m_szImageName[SZBUFLEN_0];	// �����еĶ����ļ���
	int			m_nObjIdx;					// ��Ӧ�������
	int			m_nWidth;					// ����������ռ����
	int			m_nHeight;					// ����������ռ�߶�
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	int			m_nSeries;					// ��������
	int			m_nPrice;					// �۸�
	int			m_nPriceXu;
	int			m_nLevel;					// �ȼ�
	int		m_bStack;					// �Ƿ�ɵ���
	KEQCP_BASIC	m_aryPropBasic[7];			// ��������
	KEQCP_REQ	m_aryPropReq[6];			// ��������

	int	        m_nYingNuma;               //��������1
    int	        m_nYingNumb;               //��������2
	int         m_nRongNum;                //������������
    int         m_nWengangPin;             //�����Ƹ�Ʒ��
	int         m_nBinfujiazhi;            //������ֵ
	int	        m_IsBang;
	int         m_IsKuaiJie;
	int	        m_IsMagic;
    int	        m_ChiBangRes;
} KBASICPROP_EQUIPMENT;

// ���½ṹ��������Ψһװ���ĳ�ʼ����. ��������������ļ�(tab file)�ṩ ----��װ
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nItemGenre;				// �������� (����? ҩƷ? ��ʯ?)
	int			m_nDetailType;				// �������
	int			m_nParticularType;			// ��ϸ���
	char		m_szImageName[SZBUFLEN_0];	// �����еĶ����ļ���
	int			m_nObjIdx;					// ��Ӧ�������
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	int			m_nSeries;					// ��������
	int			m_nPrice;					// �۸�
	int			m_nPriceXu;
	int			m_nLevel;					// �ȼ�
	int			m_nRarity;					// ϡ�г̶�
	KEQCP_REQ	m_aryPropReq[6];			// ��������
	KMACP		m_aryMagicAttribs[6];		// ħ������
	int	        m_IsBang;
	int         m_IsKuaiJie;
	int	        m_IsMagic;
} KBASICPROP_EQUIPMENT_UNIQUE;

// ���½ṹ���������ƽ�װ���ĳ�ʼ����. ��������������ļ�(tab file)�ṩ =====�׽�
typedef struct
 {
	 char		m_szName[SZBUFLEN_0];		// ����
	 int			m_nItemGenre;				// �������� (����? ҩƷ? ��ʯ?)
	 int			m_nDetailType;				// �������
	 int			m_nParticularType;			// ��ϸ���
	 //	int			m_nRarity;					// ϡ�г̶�
	 char		m_szImageName[SZBUFLEN_0];	// �����еĶ����ļ���
	 int		m_bStack;					// �Ƿ�ɵ���
	 int			m_nObjIdx;					// ��Ӧ�������
	 int			m_nWidth;					// ��Ʒ������
	 int			m_nHeight;					// ��Ʒ���߶�
	 char		m_szIntro[SZBUFLEN_1];		// ˵������
	 int			m_nSeries;					// ��������
	 int			m_nPrice;					// �۸�
	 int			m_nPriceXu;
	 int			m_nLevel;					// �ȼ�
	 KEQCP_BASIC	m_aryPropBasic[7];			// ��������
	 KEQCP_REQ	    m_aryPropReq[6];			// ��������
	 int			m_aryMagicAttribs[6];		// ħ������
	 int            m_yinMagicAttribs[2];
	 int            m_TempPlatinaAttrib[6];     // ��ʱ��ħ������
	 int			m_nId;						// �ƽ�Id
	 int			m_nSet;						// ������װ
	 int			m_nSetId;					// �������
	 int			m_nSetNum;					// ��װ����
	 int			m_nUpSet;					// ��չ��װ
//	 int	        m_nYingNuma;               //��������1
//	 int	        m_nYingNumb;               //��������2
	 int            m_nRongNum;               //������������
	 int            m_nWengangPin;            //�����Ƹ�Ʒ��
	 int            m_nBinfujiazhi;            //������ֵ
	 int	        m_IsBang;
	 int            m_IsKuaiJie;
	 int	        m_IsMagic;
	 int            m_ChiBangRes;              //�����������
	 int            m_nSixSkill;
	 int            m_nTenSkill;
 } KBASICPROP_EQUIPMENT_PLATINA;

// ���½ṹ���������ƽ�װ���ĳ�ʼ����. ��������������ļ�(tab file)�ṩ =====��װ
// flying ���ݲ߻������޸���KBASICPROP_EQUIPMENT_UNIQUE����
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nItemGenre;				// �������� (����? ҩƷ? ��ʯ?)
	int			m_nDetailType;				// �������
	int			m_nParticularType;			// ��ϸ���
//	int			m_nRarity;					// ϡ�г̶�
	char		m_szImageName[SZBUFLEN_0];	// �����еĶ����ļ���
	int		m_bStack;					// �Ƿ�ɵ���
	int			m_nObjIdx;					// ��Ӧ�������
	int			m_nWidth;					// ��Ʒ������
	int			m_nHeight;					// ��Ʒ���߶�
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	int			m_nSeries;					// ��������
	int			m_nPrice;					// �۸�
	int			m_nPriceXu;
	int			m_nLevel;					// �ȼ�
	KEQCP_BASIC	m_aryPropBasic[7];			// ��������
	KEQCP_REQ	m_aryPropReq[6];			// ��������
	int			m_aryMagicAttribs[6];		// ħ������
	int         m_yinMagicAttribs[2];       // ��������
	int			m_nId;						// �ƽ�Id
	int			m_nSet;						// ������װ
	int			m_nSetId;					// �������
	int			m_nSetNum;					// ��װ����
	int			m_nUpSet;					// ��չ��װ
//    int	        m_nYingNuma;            //��������1
//    int	        m_nYingNumb;            //��������2
	int         m_nRongNum;               //������������
    int         m_nWengangPin;            //�����Ƹ�Ʒ��
	int         m_nBinfujiazhi;            //������ֵ
	int	        m_IsBang;
	int         m_IsKuaiJie;
	int	        m_IsMagic;
	int         m_ChiBangRes;              //�����������
} KBASICPROP_EQUIPMENT_GOLD;

typedef struct
{
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nItemGenre;				// ��������
	int			m_nDetailType;				// �������
	char		m_szImageName[SZBUFLEN_0];	// �����еĶ����ļ���
	int			m_nObjIdx;					// ��Ӧ�������
	int			m_nWidth;					// ����������ռ����
	int			m_nHeight;					// ����������ռ�߶�
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	char		m_szScript[SCBUFLEN_1];		// ִ�нű�
	int			m_nPrice;
	int			m_nPriceXu;
	int			m_nDelet;
	int			m_nIsSell;
	int			m_nIsTrade;
	int			m_nIsDrop;  //���ϣ�
	int	        m_IsBang;
	int         m_IsKuaiJie;
	int         m_SkillType;                //�Ƹֵ�����ħ��ID
	int			m_nSeries;					//��������
	int			m_ISMagic;
	int         m_nLevel;
	int         m_bStack;                   //�Ƿ����
	int         m_MagicID; //����ID
	int	        m_IsUse;
} KBASICPROP_QUEST;

typedef struct
{
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nItemGenre;				// ��������
	int			m_nDetailType;				// �������
	int			m_nParticularType;			// ��ϸ���
	char		m_szImageName[SZBUFLEN_0];	// �����еĶ����ļ���
	int			m_nObjIdx;					// ��Ӧ�������
	int			m_nWidth;					// ����������ռ����
	int			m_nHeight;					// ����������ռ�߶�
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	char		m_szScript[SCBUFLEN_1];		// ִ�нű�
	int			m_nPrice;
	int			m_nPriceXu;
	int			m_nDelet;
	int			m_nIsSell;
	int			m_nIsTrade;
	int			m_nIsDrop;  //���ϣ�
	int	        m_IsBang;
	int         m_IsKuaiJie;
	int         m_SkillType;                //�Ƹֵ�����ħ��ID
	int			m_nSeries;					//��������
	int			m_ISMagic;
	int         m_nLevel;
	int         m_bStack;    //�Ƿ����
	int         m_MagicID;   //����ID
	int	        m_IsUse;
    int         m_inPin;     //Ʒ��
    int         m_MagIndex;  //ħ������
    int         m_Magic[6];  //��������λ
}KBASICPROP_FUSION;  //�Ƹ�

typedef struct
{
	char		m_szName[SZBUFLEN_0];		// ����
	int			m_nItemGenre;				// ��������
	char		m_szImageName[SZBUFLEN_0];	// �����еĶ����ļ���
	int			m_nObjIdx;					// ��Ӧ�������
	int			m_nWidth;					// ����������ռ����
	int			m_nHeight;					// ����������ռ�߶�
	int			m_nPrice;					// �۸�
	int			m_nPriceXu;
	char		m_szIntro[SZBUFLEN_1];		// ˵������
	int	        m_IsBang;
	int         m_IsKuaiJie;
	int	        m_IsMagic;
} KBASICPROP_TOWNPORTAL;
//=============================================================================

class KBasicPropertyTable			      // ��д: BPT,����������
{
public:
	KBasicPropertyTable();
	~KBasicPropertyTable();

// �����Ǻ��ĳ�Ա����
protected:
	void*		m_pBuf;						// ָ�����Ա���������ָ��
											// ���Ա���һ���ṹ����,
											// ��������������������
	int			m_nNumOfEntries;			// ���Ա����ж���������

// �����Ǹ����Եĳ�Ա����
    int         m_nSizeOfEntry;				// ÿ�����ݵĴ�С(���ṹ�Ĵ�С)
	char		m_szTabFile[MAX_PATH];		// tabfile���ļ���

// �����Ƕ���ӿ�
public:
	virtual int Load();					// ��tabfile�ж�����ʼ����ֵ, �������Ա�
	int NumOfEntries() const { return m_nNumOfEntries; }

// �����Ǹ�������
protected:
	int GetMemory();
	void ReleaseMemory();
	void SetCount(int);
	virtual int LoadRecord(int i, KTabFile* pTF) = 0;
};

class KBPT_Mine : public KBasicPropertyTable
{
public:
	KBPT_Mine();
	~KBPT_Mine();

// �����Ƕ���ӿ�
public:
	const KBASICPROP_MINE* GetRecord(IN int) const;
	const KBASICPROP_MINE* FindRecord(IN int, IN int) const;

// �����Ǹ�������
protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
};

// =====>ҩƷ<=====
class KBPT_Medicine : public KBasicPropertyTable
{
public:
	KBPT_Medicine();
	~KBPT_Medicine();

// �����Ƕ���ӿ�
public:
	const KBASICPROP_MEDICINE* GetRecord(IN int) const;
	const KBASICPROP_MEDICINE* FindRecord(IN int, IN int) const;

// �����Ǹ�������
protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
};

// =====>������Ʒ<=====
class KBPT_Quest : public KBasicPropertyTable
{
public:
	KBPT_Quest();
	~KBPT_Quest();

// �����Ƕ���ӿ�
public:
	const KBASICPROP_QUEST* GetRecord(IN int) const;
	const KBASICPROP_QUEST* FindRecord(IN int) const;

protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
};

// =====>�Ƹ���Ʒ<=====
class KBPT_Fusion : public KBasicPropertyTable
{
public:
	KBPT_Fusion();
	~KBPT_Fusion();

	// �����Ƕ���ӿ�
public:
	const KBASICPROP_FUSION* GetRecord(IN int) const;
	const KBASICPROP_FUSION* FindRecord(IN int,IN int) const;

protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
};
// =====>�س���Ʒ<=====
class KBPT_TownPortal : public KBasicPropertyTable
{
public:
	KBPT_TownPortal();
	~KBPT_TownPortal();

// �����Ƕ���ӿ�
public:
	const KBASICPROP_TOWNPORTAL* GetRecord(IN int) const;

protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
};

// =====>ҩ��<=====
class KBPT_MedMaterial : public KBasicPropertyTable
{
public:
	KBPT_MedMaterial();
	~KBPT_MedMaterial();

// �����Ƕ���ӿ�
public:
	const KBASICPROP_MEDMATERIAL* GetRecord(IN int) const;

// �����Ǹ�������
protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
};
//��װ
class KBPT_Equipment : public KBasicPropertyTable
{
public:
	KBPT_Equipment();
	~KBPT_Equipment();

// �����Ƕ���ӿ�
public:
	const KBASICPROP_EQUIPMENT* GetRecord(IN int) const;
	const KBASICPROP_EQUIPMENT* FindRecord(IN int, IN int, IN int) const;
	void Init(IN int);
// �����Ǹ�������
protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
};
//��װ
class KBPT_Equipment_Unique : public KBasicPropertyTable
{
public:
	KBPT_Equipment_Unique();
	~KBPT_Equipment_Unique();

// �����Ƕ���ӿ�
public:
	const KBASICPROP_EQUIPMENT_UNIQUE* GetRecord(IN int) const;
	const KBASICPROP_EQUIPMENT_UNIQUE* FindRecord(IN int, IN int, IN int) const;

// �����Ǹ�������
protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
};

// �׽�װ��
class KBPT_Equipment_Platina : public KBasicPropertyTable
{
public:
	KBPT_Equipment_Platina();
	virtual ~KBPT_Equipment_Platina();

	// �����Ƕ���ӿ�	 KBASICPROP_EQUIPMENT_PLATINA
public:
	const KBASICPROP_EQUIPMENT_PLATINA* GetRecord(IN int) const;
	const KBASICPROP_EQUIPMENT_PLATINA* FindRecord(IN int, IN int, IN int) const;
	int GetRecordCount() const {return KBasicPropertyTable::NumOfEntries();};
	void Init();
	// �����Ǹ�������
protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
};

// flying modify this class
// �ƽ�װ��
class KBPT_Equipment_Gold : public KBasicPropertyTable
{
public:
	KBPT_Equipment_Gold();
	virtual ~KBPT_Equipment_Gold();

// �����Ƕ���ӿ�
public:
	const KBASICPROP_EQUIPMENT_GOLD* GetRecord(IN int) const;
	const KBASICPROP_EQUIPMENT_GOLD* FindRecord(IN int, IN int, IN int) const;
	int GetRecordCount() const {return KBasicPropertyTable::NumOfEntries();};
	void Init();
// �����Ǹ�������
protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
};

class KBPT_MagicAttrib_TF : public KBasicPropertyTable
{
public:
	KBPT_MagicAttrib_TF();
	~KBPT_MagicAttrib_TF();

// �����Ǹ�����Ա����
protected:
	int m_naryMACount[2][MATF_CBDR];	// ÿ��װ�������õ�ħ����Ŀ,��ǰ��׺����ͳ��
										// ����MATF_CBDR��װ�����Ծ߱�ħ��
// �����Ƕ���ӿ�
public:
	void GetMACount(int*) const;
	const KMAGICATTRIB_TABFILE* GetRecord(IN int) const;

// �����Ǹ�������
protected:
	virtual int LoadRecord(int i, KTabFile* pTF);
	void Init();
};
/*
class KBPT_MagicAttrib : public KBasicPropertyTable
{
public:
	KBPT_MagicAttrib();
	~KBPT_MagicAttrib();

// �����Ǹ�������
protected:
};
*/

//============================================================================
class KBPT_ClassMAIT    // Magic Item Index Table
{
public:
	KBPT_ClassMAIT();
	~KBPT_ClassMAIT();

// �����Ǻ��ĳ�Ա����
protected:
	int*	m_pnTable;				// ������ָ��, ��������Ϊ
									// KBPT_MagicAttrib_TF::m_pBuf������±�
	int		m_nSize;				// �������ں�����������(�����ֽ���)

// �����Ǹ�����Ա����
	int		m_nNumOfValidData;		// ����������Ч���ݵĸ���
									// ��ʼ��������ɺ�m_nNumOfValidData < m_nSize
// �����Ƕ���ӿ�
public:
    int Clear();
	int Insert(int nItemIndex);
	int  Get(int i) const;
    int  GetCount() const { return m_nNumOfValidData; }
};

//============================================================================

class KBPT_ClassifiedMAT
{
public:
	KBPT_ClassifiedMAT();
	~KBPT_ClassifiedMAT();

// �����Ǻ��ĳ�Ա����
protected:
	int*	m_pnTable;				// ������ָ��, ��������Ϊ
									// KBPT_MagicAttrib_TF::m_pBuf������±�
	int		m_nSize;				// �������ں�����������(�����ֽ���)

// �����Ǹ�����Ա����
	int		m_nNumOfValidData;		// ����������Ч���ݵĸ���
									// ��ʼ��������ɺ�m_nNumOfValidData==m_nSize
// �����Ƕ���ӿ�
public:
	int GetMemory(int);
	int Set(int);
	int  Get(int) const;
	int GetAll(int*, int*) const;

// �����Ǹ�������
protected:
	void ReleaseMemory();
};

class KLibOfBPT
{
public:
	KLibOfBPT();
	~KLibOfBPT();

// �����Ǻ��ĳ�Ա����
protected:
	KBPT_Medicine			m_BPTMedicine;
	KBPT_TownPortal			m_BPTTownPortal;
	KBPT_Quest				m_BPTQuest;
	KBPT_Fusion				m_BPTFusion;  //�Ƹ�
	KBPT_Mine				m_BPTMine;
	KBPT_Equipment			m_BPTHorse;
	KBPT_Equipment			m_BPTMeleeWeapon;
	KBPT_Equipment			m_BPTRangeWeapon;
	KBPT_Equipment			m_BPTArmor;
	KBPT_Equipment			m_BPTHelm;
	KBPT_Equipment			m_BPTBoot;
	KBPT_Equipment			m_BPTBelt;
	KBPT_Equipment			m_BPTAmulet;
	KBPT_Equipment			m_BPTRing;
	KBPT_Equipment			m_BPTCuff;
	KBPT_Equipment			m_BPTPendant;
	KBPT_Equipment			m_BPTMask;
	KBPT_Equipment			m_BPTPifeng;
	KBPT_Equipment			m_BPTYinjian;
	KBPT_Equipment			m_BPTShipin;

    KBPT_MagicAttrib_TF		m_BPTMagicAttrib;  //ħ�������б�
	KBPT_Equipment_Gold		m_GoldItem;
	KBPT_Equipment_Platina	m_Platina;
	// ��ά�ֱ�Ϊǰ��׺����Ʒ���͡����С�����
    KBPT_ClassMAIT          m_CMAIT[MATF_PREFIXPOSFIX][MATF_CBDR][MATF_SERIES][MATF_LEVEL];

	KBPT_ClassifiedMAT		m_CMAT[2][MATF_CBDR];

// �����Ƕ���ӿ�
public:
	int Init();

	const KMAGICATTRIB_TABFILE* GetMARecord(IN int) const;
	const int					GetMARecordNumber() const;

    // Add by Freeway Chen in 2003.5.30
    const KBPT_ClassMAIT*       GetCMIT(IN int nPrefixPostfix, IN int nType, IN int nSeries, int nLevel) const;

	const KBPT_ClassifiedMAT*	GetCMAT(IN int, int) const;
	// Add by flying on 2003.6.2
	const KBASICPROP_EQUIPMENT_GOLD*	GetGoldItemRecord(IN int nIndex) const;
	const int							GetGoldItemNumber() const;

	const KBASICPROP_EQUIPMENT_PLATINA*	GetPlatinaRecord(IN int nIndex) const;
	const int							GetPlatinaNumber() const;

	const KBASICPROP_EQUIPMENT*	GetMeleeWeaponRecord(IN int) const;
	const int					GetMeleeWeaponRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetRangeWeaponRecord(IN int) const;
	const int					GetRangeWeaponRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetArmorRecord(IN int) const;
	const int					GetArmorRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetHelmRecord(IN int) const;
	const int					GetHelmRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetBootRecord(IN int) const;
	const int					GetBootRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetBeltRecord(IN int) const;
	const int					GetBeltRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetAmuletRecord(IN int) const;
	const int					GetAmuletRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetRingRecord(IN int) const;
	const int					GetRingRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetCuffRecord(IN int) const;
	const int					GetCuffRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetPendantRecord(IN int) const;
	const int					GetPendantRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetHorseRecord(IN int) const;
	const int					GetHorseRecordNumber() const;
	const KBASICPROP_EQUIPMENT_UNIQUE* FindEquipmentUnique(IN int, IN int, IN int) const;
	const KBASICPROP_MEDMATERIAL* GetMedMaterial(IN int) const;
	const KBASICPROP_MEDICINE*	GetMedicineRecord(IN int) const;
	const int					GetMedicineRecordNumber() const;
	const KBASICPROP_MEDICINE*	FindMedicine(IN int, IN int) const;
	const KBASICPROP_QUEST*		GetQuestRecord(IN int) const;
	const int					GetQuestRecordNumber() const;
	const KBASICPROP_TOWNPORTAL*	GetTownPortalRecord(IN int) const;
	const int					GetTownPortalRecordNumber() const;
	const KBASICPROP_MINE*		GetMine(IN int) const;
	const int					GetMineRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetMaskRecord(IN int) const;	// mat na
	const int					GetMaskRecordNumber() const;	// mat na
	const KBASICPROP_EQUIPMENT* GetPifengRecord(IN int) const;	// ����
	const int					GetPifengRecordNumber() const;	// ����
	const KBASICPROP_EQUIPMENT* GetYinjianRecord(IN int) const;	// ӡ��
	const int					GetYinjianRecordNumber() const;	// ӡ��
	const KBASICPROP_EQUIPMENT* GetShipinRecord(IN int) const;	// ��Ʒ
	const int					GetShipinRecordNumber() const;	// ��Ʒ
	const KBASICPROP_FUSION*	GetFusionRecord(IN int) const;  //�Ƹ�
	const int					GetFusionRecordNumber() const;  //�̸�

// 	const KBASICPROP_EQUIPMENT_GOLD*	GetGoldRecord(IN int) const;
// 	const int					GetGoldRecordNumber(IN int) const;
// �����Ǹ�������
protected:
	int InitMALib();

    // Add by Freeway Chen in 2003.5.30
    int InitMAIT();
};
#endif		// #ifndef KBasPropTblH
