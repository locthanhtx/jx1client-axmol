#ifndef KMagicDescH
#define	KMagicDescH

#include "engine/KIniFile.h"

class KMagicDesc
{
private:
	KIniFile	m_MIniFile;
	char		m_szDesc[512];
public:
	KMagicDesc();
	~KMagicDesc();
	int		Init();
	void        scat(char* p,char const* q);
	const char*	GetDesc(void* pData,int nType=0) ;//0Ϊ��Ʒ 1 Ϊ����
	const char* GetDesc_(void *pData,int nType=0);
	const void 	GetDesc_New(char *strReturn,void* pData,int nType=0); //0Ϊ��Ʒ 1 Ϊ����
	const char* BaoShiGetDesc(void *pData);
};

extern KMagicDesc	g_MagicDesc;
#endif
