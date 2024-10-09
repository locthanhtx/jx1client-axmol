//
// Created by Kull on 10/4/2024.
//

#ifndef JX1CLIENT_MAINLOGIN_H
#define JX1CLIENT_MAINLOGIN_H

#include "cocos2d.h"
#include "FairyGUI.h"
#include "gameui/Uiglobaldata.h"

USING_NS_FGUI;

class MainLogin : public ax::Scene{
public:
    bool init();
    MainLogin();
    virtual ~MainLogin();
    CREATE_FUNC(MainLogin);
    virtual void update(float delta);
protected:
    GRoot* _groot;
    void	GetAccountPassword(char* pszAccount, KSG_PASSWORD* pPassword);
    void	SetAccountPassword(const char* pszAccount, const KSG_PASSWORD* pcPassword);
    void	ClearAccountPassword(bool bAccount, bool bPassword);

private:
    GComponent* _view;
    void mianLoginCallback(Object* pSender);
    void mianCancelCallback(Object* pSender);

    void	AcceptNetMsg(void* pMsgData);
    LOGIN_LOGIC_STATUS			m_Status;  //发送标识变量
    LOGIN_LOGIC_RESULT_INFO		m_Result; //接受标识变量

    //====各操作的网络响应的返回处理====
    void	ProcessAccountLoginResponse(KLoginStructHead* pResponse);		//处理账号登陆的响应
    void	ProcessRoleListResponse(TProcessData* pResponse);
};


#endif //JX1CLIENT_MAINLOGIN_H
