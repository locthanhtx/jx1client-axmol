#ifndef __KTABFILECTRL_H__
#define __KTABFILECTRL_H__
#include "KEngine.h"
#include "KNode.h"
/////////////////////////////////////
#include "KITabFile.h"
////////////////////////////////////
class TTabColNode: public KNode
{
	
public :
	char * m_Str;
	~TTabColNode(){ if (m_Str){ delete m_Str;m_Str=NULL;}};
	
};

class TTabLineNode: public KNode
{
public:
	KList * pList;
	~TTabLineNode()
	{
		while(pList->GetHead())	
		{
			TTabColNode * pDelNode = (TTabColNode*) pList->GetHead();
			pList->RemoveHead();
			if (pDelNode)
			{
				delete pDelNode;
				pDelNode=NULL;
			}
		}
		//delete pList;pList=NULL;
	};
};

class KTabFileCtrl:public KITabFile
{
private:
	
	BOOL		SetValue(int32_t nRow, int32_t nColumn, LPSTR lpRString, DWORD dwSize, BOOL bCanCreateNew = TRUE);
	BOOL		GetValue(int32_t nRow, int32_t nColumn, LPSTR& lpRString, DWORD dwSize);
	BOOL		GetValue(LPSTR RowName, LPSTR ColumnName, LPSTR szValue, DWORD dwSize)
	{return GetValue(FindRow(RowName),FindColumn(ColumnName), szValue, dwSize);};
	int32_t			Str2Col(LPSTR);
	//KMemClass   Meme;
public:
	KTabFileCtrl();
	~KTabFileCtrl();
	KList		m_RowList;
	BOOL		Load(LPSTR FileName/*,LPSTR mMemKey=NULL*/);//�����ļ�
	BOOL	    openfile(LPSTR FileName);
	BOOL		Save(LPSTR FileName);//�����ļ�
	bool		CreatFile(LPSTR FileName);//����ļ�
	BOOL		LoadPack(LPSTR FileName);
	int32_t			FindRow(LPSTR szRow);//������1Ϊ����ֵ
	int32_t			FindColumn(LPSTR szColumn);//������1Ϊ����ֵ
	LPSTR		GetRowName(int32_t nRow);
	LPSTR		GetColName(int32_t nCol);
	int32_t			InsertNewCol(LPSTR strNewCol);//���������ɵ�Col��ţ������򷵻�-1


	BOOL		InsertAfter(int32_t nRow);
	
	BOOL		InsertAfter(LPSTR szRow, BOOL bColumnLab = TRUE)
	{
		if (!bColumnLab)
			return InsertAfter(Str2Col(szRow)); 
		else
			return InsertAfter(FindRow(szRow));
	}

	BOOL		InsertBefore(int32_t nRow);
	
	BOOL		InsertBefore(LPSTR szRow, BOOL bColumnLab = TRUE)
	{
		if (!bColumnLab)
			return InsertBefore(Str2Col(szRow));
		else
			return InsertBefore(FindRow(szRow));
	}
	
	
	
	int32_t			GetWidth()//��õ�һ�еĿ��  �ж�����
	{
		TTabLineNode * pLineNode = (TTabLineNode*)m_RowList.GetHead();
		if (!pLineNode) return 0;
		return pLineNode->pList->GetNodeCount();
	};
	int32_t			GetHeight() { return m_RowList.GetNodeCount();};//����ܳ���
	
	BOOL		GetString(int32_t nRow, int32_t nColumn, LPSTR lpDefault, LPSTR lpRString, DWORD dwSize);
	
	BOOL		GetString(int32_t nRow, LPSTR szColumn, LPSTR lpDefault, LPSTR lpRString, DWORD dwSize, BOOL bColumnLab = TRUE)
	{	
		if (!bColumnLab) //�ö���exl�ļ���a1,b1,c1�ַ���Ϊ
			return GetString(nRow, Str2Col(szColumn), lpDefault, lpRString, dwSize);	
		else
			return GetString(nRow, FindColumn(szColumn), lpDefault, lpRString, dwSize);	
	};
	
	BOOL		GetString(LPSTR szRow, LPSTR szColumn, LPSTR lpDefault, LPSTR lpRString, DWORD dwSize)
				{			return GetString(FindRow(szRow), FindColumn(szColumn), lpDefault, lpRString, dwSize);	};
	
	BOOL		GetInteger(int32_t nRow, int32_t nColumn, int32_t nDefault, int32_t *pnValue);
	
	BOOL		GetInteger(int32_t nRow, LPSTR szColumn, int32_t nDefault, int32_t *pnValue, BOOL bColumnLab = TRUE)
				{			if (!bColumnLab)
				return GetInteger( nRow, Str2Col(szColumn),	nDefault, pnValue);
				else
					return GetInteger( nRow, FindColumn(szColumn),	nDefault, pnValue);
				};
	
	BOOL		GetInteger(LPSTR szRow, LPSTR szColumn, int32_t nDefault, int32_t *pnValue)
	{
		return GetInteger(FindRow(szRow), FindColumn(szColumn), nDefault, pnValue);
	}
	
	
	BOOL		GetFloat(int32_t nRow, int32_t nColumn, float fDefault, float *pfValue);
	BOOL		GetFloat(int32_t nRow, LPSTR szColumn, float fDefault, float *pfValue, BOOL bColumnLab = TRUE)
	{
		if (!bColumnLab)
			return GetFloat( nRow, Str2Col(szColumn),	fDefault, pfValue);
		else
			return GetFloat( nRow, FindColumn(szColumn),	fDefault, pfValue);
	};
	
	BOOL		GetFloat(LPSTR szRow, LPSTR szColumn, float fDefault, float *pfValue)
	{
		return GetFloat( FindRow(szRow), FindColumn(szColumn),	fDefault, pfValue);
	}
	
	
	
	BOOL		WriteString(int32_t nRow, int32_t nColumn,  LPSTR lpString,  DWORD dwSize = 0);
	
	BOOL		WriteString(int32_t nRow, LPSTR szColumn, LPSTR lpString,  BOOL bColumnLab = TRUE)
	{	
		if (!bColumnLab) //�ö���exl�ļ���a1,b1,c1�ַ���Ϊ
			return WriteString(nRow, Str2Col(szColumn), lpString);	
		else
			return WriteString(nRow, FindColumn(szColumn), lpString);	
	};
	
	BOOL		WriteString(LPSTR szRow, LPSTR szColumn,  LPSTR lpRString)
	{			return WriteString(FindRow(szRow), FindColumn(szColumn), lpRString);};
	
	BOOL		WriteInteger(int32_t nRow, int32_t nColumn, int32_t nValue);
	
	BOOL		WriteInteger(int32_t nRow, LPSTR szColumn, int32_t nValue, BOOL bColumnLab = TRUE)
	{			if (!bColumnLab)
	return WriteInteger( nRow, Str2Col(szColumn),	 nValue);
				else
					return WriteInteger( nRow, FindColumn(szColumn), nValue);
	};
	
	BOOL		WriteInteger(LPSTR szRow, LPSTR szColumn, int32_t nValue)
	{
		return WriteInteger(FindRow(szRow), FindColumn(szColumn), nValue);
	}
	
	
	BOOL		WriteFloat(int32_t nRow, int32_t nColumn,float fValue);
	BOOL		WriteFloat(int32_t nRow, LPSTR szColumn, float fValue, BOOL bColumnLab = TRUE)
	{
		if (!bColumnLab)
			return WriteFloat( nRow, Str2Col(szColumn),	fValue);
		else
			return WriteFloat( nRow, FindColumn(szColumn), fValue);
	};
	
	BOOL		WriteFloat(LPSTR szRow, LPSTR szColumn,  float fValue)
	{
		return WriteFloat( FindRow(szRow), FindColumn(szColumn),	fValue);
	}
	
	BOOL		Remove(int32_t nRow);
	BOOL		Remove(LPSTR szRow, BOOL bColumnLab = TRUE)
	{
		if (!bColumnLab)
			return Remove(Str2Col(szRow));
		else 
			return Remove(FindRow(szRow));
	};
	void		Clear();
	bool        isload;

};


#endif //__KTABFILECTRL_H__
