//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KList.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef	KUPDATELAYER_H
#define	KUPDATELAYER_H
#pragma once
//---------------------------------------------------------------------------
#include "cocos2d.h"
#include "engine/KPakList.h"
#include "engine/KDownLoadFile.h"
USING_NS_AX;

#include "AudioEngine.h"

#include "platform/FileUtils.h"
//#include "network/ODSocket.h"
#include "../network/ClientStage.h"
#include "../network/Thread.h"
#include "LoginDef.h"
#include "assets-manager/AssetsManager.h"

////using namespace CocosDenshion;
// �ȸ���ʵ��ʾ��
struct Kpaklistdata
{
	char    pakName[128];
	int     _pakreload;
	bool    _isbreak;
};
class KUpdateLayer: public Layer, public ax::extension::AssetsManagerDelegateProtocol,protected CThread
{
public:
	KUpdateLayer();
	~KUpdateLayer();
	static Scene* scene(){
		Scene* scene = Scene::create();
		scene->addChild(KUpdateLayer::create());
		return scene;
	};

    CREATE_FUNC(KUpdateLayer);

//	static KUpdateLayer* create() {
//		KUpdateLayer* pLayer = new KUpdateLayer;
//		if (pLayer && pLayer->init())
//		{
//			pLayer->autorelease();
//			return pLayer;
//		}
//		delete pLayer;
//		return NULL;
//	};

	// ��ʼ��
	bool init();
	using CThread::Start;
	//CThread UpThread;
	//virtual void onEnter();
	// �˵��ص�����
	void reset(Ref* pSender);					// ���ð汾
	void getClientVersion(Ref* pSender);	    // ��ȡ��ǰ�ͻ��˰汾��
	void checkUpdate(Ref* pSender);		    // ����Ƿ��а汾����
	void updates(Ref* pSender);				// ���°汾
	void enterScene(Ref* pSender);				// ���볡�������δ������Ļ�м����ʾ̾�ŵ�ͼƬ�����º����ʾ��һ��ͼƬ
	//ODSocket * sock_client;
	virtual void update(float delta);
	virtual void Run();
protected:
	// ��ʼ������Ŀ¼
	void initDownloadDir();
	void creatDownloadDir(std::string nTempPath);
	// ɾ��Ŀ¼
	void deleteDir(std::string dir);
private:
	downloadFile *downPtr;
	//��ȡ��ʼʱ��Ľ�����
	Sprite *m_pLoadBarStart;
	//��ȡ���ʱ��Ľ�����
	ProgressTimer *m_pLoadBarEnd;
	//�߳���صĺ���
	//�ܵļ���ͼƬ��
	int m_nAllLoadResources;
	//��ǰ����ͼƬ��
	int m_nLoadedResources;
	int m_nLoadedFinish;
	//��ȡ����
	float m_fProgressIndex;
	Label *m_pLabelLoading;
	Label *m_pLabelPercent;
	void    startload();//������ɺ� ִ��ɾ������

	Label* m_label;
	std::string m_downloadDir;
	std::string m_WritablePath;
	void copyData(const char* pFileName);
	int isFileExist(const char* pFileName);
	void loadSuccess();
	//��ȡ�Ļص�����
	void loadingCallBack(Ref *obj);
	void loadingCallBack_new(const char * nFileNane,int nTalSize,int nCurSize);
	//CGameClient *nAcclient;
	//int m_bIsClientConnecting;
	//int m_bIsGameSevConnecting;
	enum UpdataState
	{//������״̬
		STATE_NONE_UP,//����
		STATE_RUN_UP,//��������
		STATE_FLS_UP,//���
		STATE_ON_FIEL_UP,
		STATE_ON_URLFIEL_UP,
		STATE_ON_NET_UP,
		STATE_LOAD_LISTDATA,
		STATE_DWON_LISTDATA,
	};
	size_t writelen;
	std::string nCurpFileName;
	int nPakCount;
    char downUrlmask[64];
	int nDownState;
	Kpaklistdata* pListData;
	int  pakDownCount;
	int  isOpenDown;
	//int  pakreload;
	unsigned int errorLoop;
	bool isHaveClient;
	bool isOver;
	bool m_IsInPutIpPadIsOpen;
	void startInPutCallback(Node *pNode);
	//std::vector<std::strging> searchPaths;

};

#endif
