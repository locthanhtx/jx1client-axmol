//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KIniFile.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KIniFile_H
#define KIniFile_H
//---------------------------------------------------------------------------
#include "cocos2d.h"
USING_NS_AX;
#include "KbugInfo.h"
#include "KMemStack.h"
#include "KMemClass.h"

//---------------------------------------------------------------------------
/*typedef struct tagRECT
{
    int    left;
    int    top;
    int    right;
    int    bottom;
} RECT, *PRECT,*NPRECT,*LPRECT;*/
//---------------------------------------------------------------------------
typedef struct tagKeyNode {
	unsigned int	dwID;
	char *			pKey;
	char *			pValue;
	tagKeyNode*		pNextNode;
} KEYNODE;
//---------------------------------------------------------------------------
typedef struct tagSecNode {
	unsigned int	dwID;
	char *			pSection;
	tagKeyNode		pKeyNode;
	tagSecNode*		pNextNode;
} SECNODE;
//---------------------------------------------------------------------------
 void g_SetIniCodec(int nCodec);
//---------------------------------------------------------------------------
class  KIniFile
{
private:
	SECNODE		m_Header;
	int		m_Offset;
	KMemStack	m_MemStack;

	KMemClass	m_MemClass;
	//unsigned int       dwFileIdx;
    //const char *    const char *   char * == char *
	//void far *void*
private:
	void		CreateIniLink(void * Buffer,int Size);
	void		ReleaseIniLink();
	unsigned int		String2Id(const char * pString);
	int		ReadLine(char * Buffer,int Size);
	int		IsKeyChar(char ch);
	char * 		SplitKeyValue(char * pString);
	int		SetKeyValue(const char *  pSection,const char *  pKey,const char *  pValue);
	int		GetKeyValue(const char *  pSection,const char *  pKey,char *  pValue,unsigned int dwSize);
public:
	KIniFile();
	~KIniFile();
	int        Load(const char* FileName, int nIsCreat=FALSE); //
	int		Save(const char * FileName);
	int		LoadPack(const char * FileName);
	int		SavePack(const char * FileName);
	void		Clear();
	int		IsSectionExist(char * lpSection);         //�Ƿ�����������
	void		EraseSection(char * lpSection);           //ɾ���������� ���������ͼ�ֵ
	void		EraseKey(char * lpSection,char * lpKey); //ɾ��һ�������ͼ�ֵ
	//unsigned int       GetFileDwIdx(){ return dwFileIdx;}
	int		GetString(
				const char *	lpSection,		// points to section name
				const char *	lpKeyName,		// points to key name
				const char *	lpDefault,		// points to default string
				char *	lpRString,		// points to destination buffer
				unsigned int	dwSize			// size of string buffer
				);
	int		GetInteger(
				const char *	lpSection,		// points to section name
				const char *	lpKeyName,		// points to key name
				int		nDefault,		// default value
				int		*pnValue        // return value
				);
	void		GetInteger2(
				const char *	lpSection,		// points to section name
				const char *	lpKeyName,		// points to key name
				int		*pnValue1,		// value 1
				int		*pnValue2       // value 2
				);
	int		GetFloat(
				const char *	lpSection,		// points to section name
				const char *	lpKeyName,		// points to key name
				float	fDefault,		// default value
				float	*pfValue        // return value
				);
	int		GetDouble(
		        const char *	lpSection,		// points to section name
		        const char *	lpKeyName,		// points to key name
		        double	fDefault,		// default value
		        double	*pfValue        // return value
				);
	void		GetFloat2(
				const char *	lpSection,		// points to section name
				const char *	lpKeyName,		// points to key name
				float	*pfValue1,		// value 1
				float	*pfValue2       // value 2
				);
	void		GetStruct(
				const char *	lpSection,		// pointer to section name
				const char *	lpKeyName,		// pointer to key name
				void *	lpStruct,		// pointer to buffer that contains data to add
				unsigned int 	dwSize			// size, in bytes, of the buffer
				);
	void		GetRect(
				const char *	lpSection,
				const char *	lpKeyName,
				RECT	*pRect
				);
	void		GetFloat3(
				const char *	lpSection,
				const char *	lpKeyName,
				float	*pFloat
				);
	void		GetFloat4(
				const char *	lpSection,
				const char *	lpKeyName,
				float	*pRect
				);
	void		GetFloat8(
		        const char *	lpSection,
		        const char *	lpKeyName,
		        float	*pRect
				);
	void        GetInt5(
		        const char * lpSection,
				const char * lpKeyName,
				int* pRect
				);
	void        GetInt2(
		        const char * lpSection,
		        const char * lpKeyName,
		        int* pRect
				);
	void        GetInt3(
		        const char * lpSection,
		        const char * lpKeyName,
		        int* pRect
				);
	void        GetInt8(
		        const char * lpSection,
		        const char * lpKeyName,
		        int* pRect
		        );

	void		GetBool(
				const char *	lpSection,
				const char *	lpKeyName,
				int	*pBool
				);
	void		WriteString(
				const char *	lpSection,		// pointer to section name
				const char *	lpKeyName,		// pointer to key name
				const char *	lpString		// pointer to string to add
				);
	void		WriteInteger(
				const char *	lpSection,		// pointer to section name
				const char *	lpKeyName,		// pointer to key name
				int	    Value			// value to write
				);
	void		WriteInteger2(
				const char *	lpSection,		// pointer to section name
				const char *	lpKeyName,		// pointer to key name
				int	    Value1,			// value 1 to write
				int		Value2			// value 2 to write
				);
	void		WriteFloat(
				const char *	lpSection,		// pointer to section name
				const char *	lpKeyName,		// pointer to key name
				float	fValue			// value to write
				);
	void		WriteFloat2(
				const char *	lpSection,		// pointer to section name
				const char *	lpKeyName,		// pointer to key name
				float	fValue1,		// value 1 to write
				float	fValue2			// value 2 to write
				);
	void		WriteStruct(
				const char *	lpSection,		// pointer to section name
				const char *	lpKeyName,		// pointer to key name
				void *	lpStruct,		// pointer to buffer that contains data to add
				unsigned int 	dwSize			// size, in bytes, of the buffer
				);
	int		GetNextSection(const char * pSection, char * pNextSection);
	int		GetNextKey(const char * pSection, const char * pKey, char * pNextKey);
	int			GetSectionCount();

};
//---------------------------------------------------------------------------
#endif
