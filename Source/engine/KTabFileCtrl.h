#ifndef __KTABFILECTRL_H__
#define __KTABFILECTRL_H__
#include "KEngine.h"
#include "KNode.h"
/////////////////////////////////////
#include "KITabFile.h"
////////////////////////////////////
class TTabColNode : public KNode
{

public:
    char* m_Str;
    ~TTabColNode()
    {
        if (m_Str)
        {
            delete m_Str;
            m_Str = NULL;
        }
    };
};

class TTabLineNode : public KNode
{
public:
    KList* pList;
    ~TTabLineNode()
    {
        while (pList->GetHead())
        {
            TTabColNode* pDelNode = (TTabColNode*)pList->GetHead();
            pList->RemoveHead();
            if (pDelNode)
            {
                delete pDelNode;
                pDelNode = NULL;
            }
        }
        // delete pList;pList=NULL;
    };
};

class KTabFileCtrl
{
private:
    int SetValue(int nRow, int nColumn, char* lpRString, unsigned long dwSize, int bCanCreateNew = TRUE);
    int GetValue(int nRow, int nColumn, char*& lpRString, unsigned long dwSize);
    int GetValue(char* RowName, char* ColumnName, char* szValue, unsigned long dwSize)
    {
        return GetValue(FindRow(RowName), FindColumn(ColumnName), szValue, dwSize);
    };
    int Str2Col(const char*);
    // KMemClass   Meme;
public:
    KTabFileCtrl();
    ~KTabFileCtrl();
    KList m_RowList;
    int Load(const char* FileName /*,char* mMemKey=NULL*/);  // �����ļ�
    int openfile(char* FileName);
    int Save(const char* FileName);        // �����ļ�
    bool CreatFile(const char* FileName);  // ����ļ�
    int LoadPack(char* FileName);
    int FindRow(char* szRow);              // ������1Ϊ����ֵ
    int FindColumn(const char* szColumn);  // ������1Ϊ����ֵ
    char* GetRowName(int nRow);
    char* GetColName(int nCol);
    int InsertNewCol(char* strNewCol);  // ���������ɵ�Col��ţ������򷵻�-1

    int InsertAfter(int nRow);

    int InsertAfter(char* szRow, int bColumnLab = TRUE)
    {
        if (!bColumnLab)
            return InsertAfter(Str2Col(szRow));
        else
            return InsertAfter(FindRow(szRow));
    }

    int InsertBefore(int nRow);

    int InsertBefore(char* szRow, int bColumnLab = TRUE)
    {
        if (!bColumnLab)
            return InsertBefore(Str2Col(szRow));
        else
            return InsertBefore(FindRow(szRow));
    }

    int GetWidth()  // ��õ�һ�еĿ��  �ж�����
    {
        TTabLineNode* pLineNode = (TTabLineNode*)m_RowList.GetHead();
        if (!pLineNode)
            return 0;
        return pLineNode->pList->GetNodeCount();
    };
    int GetHeight() { return m_RowList.GetNodeCount(); };  // ����ܳ���

    int GetString(int nRow, int nColumn, char* lpDefault, char* lpRString, unsigned long dwSize);

    int GetString(int nRow,
                  char* szColumn,
                  char* lpDefault,
                  char* lpRString,
                  unsigned long dwSize,
                  int bColumnLab = TRUE)
    {
        if (!bColumnLab)  // �ö���exl�ļ���a1,b1,c1�ַ���Ϊ
            return GetString(nRow, Str2Col(szColumn), lpDefault, lpRString, dwSize);
        else
            return GetString(nRow, FindColumn(szColumn), lpDefault, lpRString, dwSize);
    };

    int GetString(char* szRow, char* szColumn, char* lpDefault, char* lpRString, unsigned long dwSize)
    {
        return GetString(FindRow(szRow), FindColumn(szColumn), lpDefault, lpRString, dwSize);
    };

    int GetInteger(int nRow, int nColumn, int nDefault, int* pnValue);

    int GetInteger(int nRow, const char* szColumn, int nDefault, int* pnValue, int bColumnLab = TRUE)
    {
        if (!bColumnLab)
            return GetInteger(nRow, Str2Col(szColumn), nDefault, pnValue);
        else
            return GetInteger(nRow, FindColumn(szColumn), nDefault, pnValue);
    };

    int GetInteger(char* szRow, char* szColumn, int nDefault, int* pnValue)
    {
        return GetInteger(FindRow(szRow), FindColumn(szColumn), nDefault, pnValue);
    }

    int GetFloat(int nRow, int nColumn, float fDefault, float* pfValue);
    int GetFloat(int nRow, char* szColumn, float fDefault, float* pfValue, int bColumnLab = TRUE)
    {
        if (!bColumnLab)
            return GetFloat(nRow, Str2Col(szColumn), fDefault, pfValue);
        else
            return GetFloat(nRow, FindColumn(szColumn), fDefault, pfValue);
    };

    int GetFloat(char* szRow, char* szColumn, float fDefault, float* pfValue)
    {
        return GetFloat(FindRow(szRow), FindColumn(szColumn), fDefault, pfValue);
    }

    int WriteString(int nRow, int nColumn, char* lpString, unsigned long dwSize = 0);

    int WriteString(int nRow, char* szColumn, char* lpString, int bColumnLab = TRUE)
    {
        if (!bColumnLab)  // �ö���exl�ļ���a1,b1,c1�ַ���Ϊ
            return WriteString(nRow, Str2Col(szColumn), lpString);
        else
            return WriteString(nRow, FindColumn(szColumn), lpString);
    };

    int WriteString(char* szRow, char* szColumn, char* lpRString)
    {
        return WriteString(FindRow(szRow), FindColumn(szColumn), lpRString);
    };

    int WriteInteger(int nRow, int nColumn, int nValue);

    int WriteInteger(int nRow, char* szColumn, int nValue, int bColumnLab = TRUE)
    {
        if (!bColumnLab)
            return WriteInteger(nRow, Str2Col(szColumn), nValue);
        else
            return WriteInteger(nRow, FindColumn(szColumn), nValue);
    };

    int WriteInteger(char* szRow, char* szColumn, int nValue)
    {
        return WriteInteger(FindRow(szRow), FindColumn(szColumn), nValue);
    }

    int WriteFloat(int nRow, int nColumn, float fValue);
    int WriteFloat(int nRow, char* szColumn, float fValue, int bColumnLab = TRUE)
    {
        if (!bColumnLab)
            return WriteFloat(nRow, Str2Col(szColumn), fValue);
        else
            return WriteFloat(nRow, FindColumn(szColumn), fValue);
    };

    int WriteFloat(char* szRow, char* szColumn, float fValue)
    {
        return WriteFloat(FindRow(szRow), FindColumn(szColumn), fValue);
    }

    int Remove(int nRow);
    int Remove(char* szRow, int bColumnLab = TRUE)
    {
        if (!bColumnLab)
            return Remove(Str2Col(szRow));
        else
            return Remove(FindRow(szRow));
    };
    void Clear();
    bool isload;
};

#endif  //__KTABFILECTRL_H__
