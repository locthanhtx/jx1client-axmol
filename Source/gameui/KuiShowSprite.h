//
//  KuiShowSprite.h
//  KuiShowSprite
//
//  Created by �ᵶ����Ц QQ:25557432
//
//
#ifndef __KuiShowSprite__
#define __KuiShowSprite__

#include <iostream>
#include "cocos2d.h"

#include "gamescene/KScenePlaceC.h"
USING_NS_AX;

using namespace ui;
//#include "Kuiplayerfun.h"
//#define FIGHT_SKILL_COUNT_PER_PAGE	25

class KuiShowSprite:public Layer{
public:
	KuiShowSprite();
	~KuiShowSprite();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiShowSprite);

	static KuiShowSprite* create(const char* nFilePath,
                                     int nXpos,
                                     int nYpos,
                                     Ref* callbackListener,
                                     const std::function<void(ax::Node*)>& callfun,
                                     bool nTouchesModel = true,
                                     int nRenderFlag    = 0);
	//���Ӽ���ͼ��
	void addDialogData();
        void mianDialogSet(const char* nFilePath, int nXpos, int nYpos, bool nTouchesModel = true, int nRenderFlag = 0);
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	virtual void update(float delta);
	virtual void draw(ax::Renderer* renderer, const ax::Mat4& transform, unsigned int flags);
	bool    isOpen;
	Size getShowSize();
	void   removeShowSprite();
	void   setShowPosition(int nPosX,int nPosY);
	void   setRegisterWithTouchDispatcher(int nVer);
	//void _setPlayerFun(Kuiplayerfun * ver){nPlayerFun=ver;};
private:
	//��д����ע�ắ�������¸�����������
	//void registerWithTouchDispatcher();
	//��������ccTouchBegan������true
	Texture2D * ccgetTxtData_s(char *sprPath,int nFramIndex,int nMpsX,int nMpsY,int nMpsZ,int *mScreeX,int *mScreeY,int bRenderFlag,int bSinglePlaneCoord,int *mHeadw,int *mHeadh);
	bool ccTouchBegan(Touch *pTouch, Event * pevent);
	void ccTouchMoved(Touch *pTouch, Event *pEvent);
	void ccTouchEnded(Touch *pTouch, Event *pEvent);

	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);
	//�ϲ����
	Ref * m_callbackListener;

	//�ص�����
	std::function<void(ax::Node*)> m_callfun;
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	Size winSize;
	//�Ի��򱳾�����
	LayerColor *ParentNode_Team;
	Rect getRect(Node* pNode,int i);
	Label* ptestLabel;
	Action* red;

	//Layer *m_pUiLayer;
	//EditBox *pstrEditBox_d;
	Size bgSize;
	Size nTextureSize;
	bool m_bScrolling;
    float nRoleDisX;
    float nRoleDisY;
	Point m_lastPoint;
	Sprite *sFarmsSprite;
	Texture2D *sFarmsTexture;
	unsigned int __loopTime;
	Label *pPointLabel;
	int    _nFarms;
	int    _curFarms;
	int    _nModel;
	char   nSprNamePath[128];
	int		_nInterval;		        //֡���
	unsigned int _nFlipTime;		//��ǰ֡�л�������ʱ��
	int   _nXpos;
	int   _nYpos;
	int   _pHwidth;
	int   _pHheight;
	int   _bRenderFlag;
	//KUiImageRef m_Image;

};

#endif /* defined(__MyGame__PopLayer__) */
