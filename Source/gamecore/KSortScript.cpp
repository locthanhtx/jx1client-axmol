#include "KCore.h"
#include "KSortScript.h"
#include "LuaFuns.h"
#include "engine/KFilePath.h"
#include "engine/KTabFile.h"
#include "engine/KTabFileCtrl.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#else 
#include <io.h>
#include <direct.h>
#endif

KLuaScript g_ScriptSet[MAX_SCRIPT_IN_SET];   //�ű���������
//KScriptBinTree g_ScriptBinTree;
uint32_t   nCurrentScriptNum;

void	LoadScriptInDirectory(LPSTR lpszRootDir, LPSTR lpszSubDir);
//���ز�������
/*int	operator<(KSortScriptNode ScriptLeft, KSortScriptNode ScriptRight)
{
	return ScriptLeft.GetScriptID() < ScriptRight.GetScriptID();
};
	
int operator==(KSortScriptNode ScriptLeft, KSortScriptNode ScriptRight)
{
	return ScriptLeft.GetScriptID() == ScriptRight.GetScriptID();
};
 */
//-------------�µ�----------------------------------------------
#include <algorithm>

class KLuaScriptRateMap : public std::map<DWORD, KLuaScript *>
{
private:
	static VOID _FreeNode(value_type &cValue)
	{
		//SAFE_DELETE(cValue.second);
		if (cValue.second)
		{//KEYֵ
			delete (cValue.second);
			(cValue.second) = NULL;
		} 
	}
	
public:
	~KLuaScriptRateMap()
	{//ɾ��ȫ���ڵ�
/*#ifdef _SERVER
		std::for_each(begin(), end(), _FreeNode);
#endif*/

	}
};

static KLuaScriptRateMap g_ScriptRateMap;	// Ϊ�˱�֤�ڴ����İ�ȫ���ñ������ٶ���


const KScript * g_GetScript(DWORD dwScriptId)
{//�Ѿ����ؽű� ����ջ�еĽڵ�
	//KSortScriptNode ScriptNode;
	 //               ScriptNode.SetScriptID(dwScriptId);
	/*if (g_ScriptBinTree.Find(ScriptNode))
	{
		return ScriptNode.GetScript();
	} */

	if (g_ScriptRateMap.count(dwScriptId) > 0)
	{
		return g_ScriptRateMap[dwScriptId];
	} 

	return NULL;
}

const KScript * g_GetScript(const char * szRelativeScriptFile)
{
	DWORD dwScriptId = g_CheckFileExist((LPSTR)szRelativeScriptFile);
	return g_GetScript(dwScriptId);
}
///////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
extern int LuaIncludeFile(Lua_State * L);
extern int LuaIncludeLib(Lua_State * L);

static void KSceGetFliePath(char* nName,char * nPath,int nMaxRows)
{
	KFile nFile;
	KTabFileCtrl nScirptFile;
	char nTongApplyPath[125]={0},szCol[128]={0};
	sprintf(nTongApplyPath,"%s","\\clientscript.log");
	if (!g_FileExists(nTongApplyPath))	//�Ƿ����
	{
		nFile.Create(nTongApplyPath);
		//sprintf(szCol,"��ͼ\t����\tԴ�ű�\15\n");
		sprintf(szCol,"�ű�Log\15\n");
		nFile.Write(szCol, sizeof(szCol));
		//nFile.Save(nTongApplyPath);
		nFile.Close();
	}
	
	if (nScirptFile.Load(nTongApplyPath))
	{
		int nRows=nScirptFile.GetHeight();
		if  (nRows==0)
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
		}
		
		sprintf(szCol,"--%s:(%s)--",nName,nPath);
		nScirptFile.InsertAfter(nRows);
		nScirptFile.WriteString(nRows,1, szCol);                                    //�к�
		//nTong.WriteString(nRows,2,Npc[Player[m_nPlayerIndex].m_nIndex].Name);     //����
		//nTong.WriteInteger(nRows,3,Npc[Player[m_nPlayerIndex].m_nIndex].m_Level); //�ȼ�
		nScirptFile.Save(nTongApplyPath);	
		//nFile.Write(szCol, sizeof(szCol));
	}
	nScirptFile.Clear();
}
//���ؽű������ļ�������Ϊ���Ŀ¼
static BOOL LoadScriptToSortListA(char * szRelativeFile)
{
	if (!szRelativeFile || !szRelativeFile[0]) return FALSE;
	//KSortScriptNode ScriptNode ;
	//ScriptNode.SetScriptID(g_FileName2Id(szRelativeFile));  //���ýڵ�����
	int t  =strlen(szRelativeFile);

	if (t >= 256)
		ccMessageBox("path is to long ","script");
		//printf("[�ű�]���ؽű�[%s]���ȹ���,�ýű����ܳ���!����! \n", szRelativeFile);
	
	if (nCurrentScriptNum < MAX_SCRIPT_IN_SET)  //�ű����ƣ�
	{
		g_ScriptSet[nCurrentScriptNum].Init();  //��ʼ���ű�������
		g_ScriptSet[nCurrentScriptNum].RegisterFunctions(GameScriptFuns, g_GetGameScriptFunNum()); //ע����Ϸ����
		g_StrCpyLen(g_ScriptSet[nCurrentScriptNum].m_szScriptName, szRelativeFile, 256);
		if (!g_ScriptSet[nCurrentScriptNum].Load(szRelativeFile))  //���Լ���PAK�ļ���
		{
			//printf("-[�ű�]����[%s]����,����!-\n", szRelativeFile);
			g_ScriptSet[nCurrentScriptNum].Exit();
			return FALSE;
		}		
	}
	else
	{
		//printf("-�ű�]�ű�����������[%d]!���������!-\n", MAX_SCRIPT_IN_SET);
		return FALSE;
	}

	 g_ScriptRateMap[g_CheckFileExist(szRelativeFile)] = &g_ScriptSet[nCurrentScriptNum];
     nCurrentScriptNum ++;
	//ScriptNode.SetScriptIndex(nCurrentScriptNum++);	//���ýű�����
	//g_ScriptBinTree.Insert(ScriptNode);             //����ڵ�
	return TRUE;
}

//��szFilePathĿ¼�µ����нű��ļ����ؽ�ScriptBinTree��������
static uint32_t LoadAllScript(char * szFilePath,BOOL nIsSer)
{
	g_SetFilePath("\\");

	char szFileRootPath[256]={0};
	char szOldRootPath[MAX_PATH]={0};
	//	GetCurrentDirectory(MAX_PATH, szOldRootPath);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_getcwd(szOldRootPath, MAXPATH);//��ȡ������Ŀ¼
	//����˵����getcwd()�Ὣ��ǰ����Ŀ¼�ľ���·�����Ƶ�����buffer��ָ���ڴ�ռ���
	//����maxlenΪbuffer�Ŀռ��С��
#else
	std::string nPath =  ax::FileUtils::getInstance()->getWritablePath();
	sprintf(szOldRootPath,"%s",nPath.c_str());
#endif
	g_GetFullPath(szFileRootPath,szFilePath);

	if (nIsSer)
	   LoadScriptInDirectory(szFileRootPath,"");  //Ӳ���ļ�:ȫ·��; ���ļ�:���·��
	//-------------------��ʼ����PAK�Ľű�----------------
	char szRealDir[128]={0};
	KTabFile nMapScript;
	sprintf(szRealDir,"%s\\%s",szFilePath,"script.txt");
	//KSceGetFliePath("���سɹ�",szRealDir);
	if (nMapScript.Load(szRealDir))	//���ļ�������ļ�
	{
		int nRows=nMapScript.GetHeight();
		for (int i=0;i<nRows;++i)
		{
			nMapScript.GetString(i+1,1,"",szRealDir,sizeof(szRealDir));
			g_StrUpper(szRealDir); //ת�ɴ�д _strupr g_StrUpper
			if (strstr(szRealDir,".LUA"))
			{
				g_StrLower(szRealDir);   //��дתСд g_StrLower
				LoadScriptToSortListA(szRealDir);
			}
		}
		nMapScript.Clear();
	}
	//-----------------------------------------------------------------
	chdir(szOldRootPath);   
	//chdir ��C�����е�һ��ϵͳ���ú�����ͬcd����
	//���ڸı䵱ǰ����Ŀ¼�������ΪPath Ŀ��Ŀ¼�������Ǿ���Ŀ¼�����Ŀ¼��
	//SetCurrentDirectory(szOldRootPath);
	return nCurrentScriptNum;
}

uint32_t g_IniScriptEngine(char * nScriptDir,BOOL nIsSer,BOOL nIsClear)
{
	//g_szCurScriptDir[0] = 0;
	if (nIsClear)
	{
		nCurrentScriptNum = 0;
		//g_ScriptBinTree.ClearList();
       ClearAllScript();
	}
	return LoadAllScript(nScriptDir,nIsSer);
	
}

//���ؽű������ļ�������Ϊʵ��Ŀ¼
static BOOL LoadScriptToSortList(char * szFileName)
{
	if (!szFileName || !szFileName[0]) return FALSE;
	if (nCurrentScriptNum>= MAX_SCRIPT_IN_SET)
	{
		//printf("[Script]�ű�����������%d,���ش�������!\n",MAX_SCRIPT_IN_SET);
		return FALSE;
	}

	//int nFileNameLen = strlen(szFileName);
	
	char szRootPath[MAX_PATH];	
	g_GetRootPath(szRootPath);
//	char szRelativePath[MAX_PATH];
	char *szRelativePath;
	char szCurrentDirectory[MAX_PATH];

//	GetCurrentDirectory(MAX_PATH, szCurrentDirectory);
	getcwd(szCurrentDirectory, MAX_PATH);
	szRelativePath = szCurrentDirectory + strlen(szRootPath);
//	PathRelativePathTo(szRelativePath,szRootPath, FILE_ATTRIBUTE_DIRECTORY,szCurrentDirectory , FILE_ATTRIBUTE_NORMAL );
	char szRelativeFile[MAX_PATH];
	if (szRelativePath[0] == '.' && szRelativePath[1] == '\\')
		sprintf(szRelativeFile, "%s\\%s", szRelativePath + 1, szFileName);
	else
		sprintf(szRelativeFile, "%s\\%s", szRelativePath, szFileName);

	g_StrLower(szRelativeFile);  //ת��Сд
//	printf("[Script]Loading Script %s %d\n", szRelativeFile, g_FileName2Id(szRelativeFile));
//
	return LoadScriptToSortListA(szRelativeFile);
//	return FALSE;
}


void	LoadScriptInDirectory(LPSTR lpszRootDir, LPSTR lpszSubDir)
{
	int				nFlag;
	char			szRealDir[MAX_PATH];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	sprintf(szRealDir, "%s\\%s", lpszRootDir, lpszSubDir);
#else
	sprintf(szRealDir, "%s/%s", lpszRootDir, lpszSubDir);
        char *ptr = szRealDir;
        while(*ptr) { if(*ptr == '\\') *ptr = '/';  ptr++;  }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if(chdir(szRealDir)) return;
	_finddata_t FindData;
	long dir = _findfirst("*.*", &FindData);
	while(dir != -1)
	 {
		if(strcmp(FindData.name, ".") == 0 || strcmp(FindData.name, "..") == 0)	{
			if(_findnext(dir, &FindData)) break;
			continue;
		}
		if(FindData.attrib == _A_SUBDIR)
		{//����
			LoadScriptInDirectory(szRealDir, FindData.name);
		}
		else
		{	
			nFlag = 0;
			for (int i = 0; i < (int)strlen(FindData.name);  i++)
			{
				if (FindData.name[i] == '.')
					break;
				if (FindData.name[i] == '\\')
				{
					nFlag = 1;
					break;
				}
			}
			if (nFlag == 1)
			{
				LoadScriptInDirectory(szRealDir, FindData.name);
			}
			else
			{
				char szExt[50];
				if (strlen(FindData.name) >= 4) 
				{
					strcpy(szExt, FindData.name + strlen(FindData.name) - 4);
					g_StrUpper(szExt); //ת�ɴ�д
					if ((!strcmp(szExt, ".LUA"))/* || (!strcmp(szExt, ".TXT"))*/)
						if (!LoadScriptToSortList(FindData.name))
							printf("-------����%s�ļ�����------\n", FindData.name);
				}
			}
		}
		if(_findnext(dir, &FindData)) break;
	} 
	_findclose(dir);
	chdir(lpszRootDir);
#else
     DIR *dp;
     int i;
     struct dirent *ep;
     if(chdir(szRealDir)) return;
     dp = opendir(".");
     if(dp) {
          while(ep = readdir(dp)) {
            if(strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;
            
            if(ep->d_type ==4) {
                LoadScriptInDirectory(szRealDir, ep->d_name);
            }
            else {
			nFlag = 0;
			for (i = 0; i < (int)strlen(ep->d_name);  i++)
			{
				if (ep->d_name[i] == '.')
					break;
				if (ep->d_name[i] == '\\')
				{
					nFlag = 1;
					break;
				}
			}
			if (nFlag == 1)
			{
				LoadScriptInDirectory(szRealDir,ep->d_name);
			}
			else
			{
				char szExt[50];
				if (strlen(ep->d_name) >= 4)
				{
					strcpy(szExt, ep->d_name + strlen(ep->d_name) - 4);
					g_StrUpper(szExt);  //ת�ɴ�дg_StrUpper
///					_strupr(szExt);
					if ( (!strcmp(szExt, ".LUA"))/* || (!strcmp(szExt, ".TXT"))*/)
						if (!LoadScriptToSortList(ep->d_name))
							printf("-----����%s�ļ�����------\n", ep->d_name);
				}
			}
		}
	          }
          closedir(dp);
     }
	chdir(lpszRootDir);
#endif
}

void UnLoadScript(DWORD dwScriptID)
{
	/*KSortScriptNode ScriptNode;
	ScriptNode.SetScriptID(dwScriptID);
	g_ScriptBinTree.Delete(ScriptNode);
	*/
	if (g_ScriptRateMap.count(dwScriptID) > 0)
	{//������ھ� ɾ��
		//return g_ScriptRateMap[dwScriptID];
		//printf("-- �ű�����,����ɹ� --\n");
		g_ScriptRateMap.erase(dwScriptID);
	}

}
//���ؽű�
int ReLoadScript(const char * szRelativePathScript)
{
	if (!szRelativePathScript || !szRelativePathScript[0])
		return FALSE;
	char script[MAX_PATH];
	strcpy(script, szRelativePathScript);
//	_strlwr(script);
	g_StrLower(script);   //��дתСд
	UnLoadScript(g_CheckFileExist(script));  //��ɾ�� �ڼ���
	LoadScriptToSortListA(script);
	return nCurrentScriptNum;
}

uint32_t  ReLoadAllScript()
{
//	g_ScriptBinTree.ClearList();
	
	ClearAllScript();
	return g_IniScriptEngine("\\script");
}

void  ClearAllScript()
{
	for (KLuaScriptRateMap::iterator it = g_ScriptRateMap.begin(); it != g_ScriptRateMap.end(); ++it)
	{
		if (it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	} 

	g_ScriptRateMap.clear();

}

 
