//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiAutoPlay_vn__
#define __KuiAutoPlay_vn__

#include <iostream>
#include "cocos2d.h"

#include "engine/KIniFile.h"
#include "engine/KTabFile.h"
#include "gameui/Uiglobaldata.h"
#include "extensions/GUI/src/GUI/ControlExtension/ControlSwitch.h"
//#include "network/Thread.h"
USING_NS_AX;

using namespace ui;

//#define FIGHT_SKILL_COUNT_PER_PAGE	25
#define MAX_SKILL_COUNT	4        //ÿ�����ӵĸ߶ȺͿ��
#define MAX_ITEMLIST_COUNT	16   //ÿ�����ӵĸ߶ȺͿ��

class KuiAutoPlay_vn:public Layer//,protected CThread
{
public:
	KuiAutoPlay_vn();
	~KuiAutoPlay_vn();
	//��ʼ��
	virtual bool init();
	//����
	CREATE_FUNC(KuiAutoPlay_vn);
	//using CThread::Start;
	static KuiAutoPlay_vn * create(char * title,Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	virtual void update(float delta);
    virtual void onDraw(const ax::Mat4 &transform, unsigned int flags);
	void  _removeDelegate(bool isThis);
	bool    isOpen;
	//virtual void Run();
private:
	//���Ӽ���ͼ��
	void addDialogData();
	void setcoloseButton(Ref * callbackListener,const std::function<void(ax::Node*)>& callfun);
	void selectedBoxStateEvent(Ref* pSender, CheckBox::EventType type);
	void checkBoxStateEvent(Ref* pSender, CheckBox::EventType type);
	void selectedItemEvent(Ref* pSender, ListView::EventType type);
	void valueChanged(Ref* sender/*, CCControlEvent controlEvent*/);
	//��д����ע�ắ�������¸�����������
	void registerWithTouchDispatcher();
	std::string creatAutoPlayDir(std::string nTempPath);
	//��������ccTouchBegan������true
	bool ccTouchBegan(Touch * touch,Event * pevent);
	void touchPageBtnEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
	void oktouchEvent(Ref *pSender, ax::ui::AbstractCheckButton::TouchEventType type);
	//�رյ�����
	void closePopLayer(Ref * pSender);

	//ִ���ϲ����Ļص��������رյ�����
	void buttonCallBackFunc(Ref * pSender);
	//�ϲ����
	Ref * m_callbackListener;

	int CreateFolder(char * nPath);
	//�ص�����
	std::function<void(ax::Node*)> m_callfun;

	void setBtnTouchEnble(int index,bool enble);
	void clearTempSelindex();
	//�Ի��򱳾���С
	Point m_origin;
	Size m_size;
	Size winSize;
	//�Ի��򱳾�����
	//Sprite * m_bgSprite;
	Sprite *ParentNode_Team;
	//KUiSkillData Skills[FIGHT_SKILL_COUNT];
	Rect getRect(Node* pNode,int i);
	void base_Breathe();
	void item_Breathe();
	//Label* pMoneyLabel;
	//Label* pXuLabel;
	//LayerColor * colorLayer;
	Action* red;

	//int  m_nMoney;
	//int  m_nXu;
	//POINT m_StartPos; //���ӿ�ʼ��ƫ�� ���ƫ�� ���Ͻ�(0,0)
	Layer *m_pUiLayer;
	Layout  *m_pWidget;
	//CCArray* m_array;
	ListView* listView_Item;
	ListView* listView_tool;
	ListView* listView_use;
	Size btnSize;
	Button* btnPageBox_1;
	Button* btnPageBox_2;
	Button* btnPageBox_3;
	Button* btnPageBox_4;
	Button* btnPageBox_5;

	//Layer * _PageLayer_1;
	Node  * _PageLayer_1;
	Layer * _PageLayer_2;
	Layer * _PageLayer_3;
	Layer * _PageLayer_4;
	Layer * _PageLayer_5;
	std::string clientSaveDir;

    CheckBox* pAuto_TDP_CheckBox_TDPSinhLuc;
    CheckBox* pAuto_TDP_CheckBox_HetBinhSinhLuc;
    CheckBox* pAuto_TDP_CheckBox_HetBinhNoiLuc;
    CheckBox* pAuto_TDP_CheckBox_DoBenTrangBi;
    CheckBox* checkBox_Auto_MANA;
    CheckBox* checkBox_Auto_HP;
	CheckBox* checkBox_baohu;
	CheckBox* checkBox_zhandou;
	CheckBox* checkBox_shiqu;
	CheckBox* checkBox_savem;

	ax::extension::ControlSwitch *switchControl_Auto_HP;
	ax::extension::ControlSwitch *switchControl_baohu;
	ax::extension::ControlSwitch *switchControl_team;
	ax::extension::ControlSwitch *switchControl_team_g;
	ax::extension::ControlSwitch *switchControl_team_d;
	ax::extension::ControlSwitch *switchControl_team_y;

	ax::extension::ControlSwitch *switchControl_zhandou;
	ax::extension::ControlSwitch *switchControl_shiqu;
	ax::extension::ControlSwitch *switchControl_savem;
	ax::extension::ControlSwitch *switchControl_fenjie;
	ax::extension::ControlSwitch *switchControl_yanshi;
    ax::extension::ControlSwitch *switchControl_skill[MAX_SKILL_COUNT];
	Label      *skillnameLabel[MAX_SKILL_COUNT];

	ax::extension::ControlSwitch *switchControl_item_m;
	ax::extension::ControlSwitch *switchControl_item_d;
	ax::extension::ControlSwitch *switchControl_item_z;
	ax::extension::ControlSwitch *switchControl_item_zm;
	ax::extension::ControlSwitch *switchControl_item_t;

	ax::extension::ControlSwitch *switchControl_yabiao;
	ax::extension::ControlSwitch *switchControl_baoliu;
	ax::extension::ControlSwitch *switchControl_useitem;

	ax::extension::ControlSwitch *switchControl_dingdian;
	ax::extension::ControlSwitch *switchControl_luxian;
	ax::extension::ControlSwitch *switchControl_luzhi;
	ax::extension::ControlSwitch *switchControl_miaosha;
	ax::extension::ControlSwitch *switchControl_other_s;
	ax::extension::ControlSwitch *switchControl_other_j;

    EditBox *pLifeEditBox_2;
	EditBox *pLifeEditBox;
	EditBox *pLifeEditBox_h;
	EditBox *pNeiliEditBox;
	EditBox *pshuxEditBox;
	EditBox *pEditBox_yanshi;

	EditBox *pXunguaiEditBox;
	EditBox *pGongjiEditBox;
	EditBox *pGongjiEditBox_m;
	EditBox *pmoneyEditBox;
	EditBox *pshiquEditBox_c;

	EditBox *pstrEditBox_s;
	EditBox *pstrEditBox_sz;
	EditBox *pstrEditBox_d;
	EditBox *pstrEditBox_u;

	char nMapName[64];
	int  nMapidx;
	int  nPosX;
	int  nPosY;
	char nMapKind[32];
	KIniFile nAutoNeiGua;
	KTabFile nAutoZhuang;
	KTabFile nAutoDaiju;
	KTabFile nAutoUseItem;
	KTabFile nAutoSkill;
	char nFilePath[128];
	int nFactionNo;
	Button* custom_button_item[MAX_ITEMLIST_COUNT];
	Button* custom_button_tool[MAX_ITEMLIST_COUNT];
	Button* custom_button_use[MAX_ITEMLIST_COUNT];

	Button* _button_1;
	Button* _button_2;
	Button* _button;
	int   nTempSelItemIndex;
	int   nTempSeltoolIndex;
	int   nTempSeluseIndex;

	unsigned int m_uLifeTime;
	unsigned int m_uManaTime,m_PortalTime;
	unsigned int m_uMoneyTime;
	unsigned int nUseItemLoopTime;
	unsigned int g_nServiceLoop;

};

#endif /* defined(__MyGame__PopLayer__) */
