//
// Created by Kull on 3/10/24.
//

#ifndef JX1CLIENT_SELSERVER_H
#define JX1CLIENT_SELSERVER_H

#include "cocos2d.h"
#include "FairyGUI.h"
#include "engine/KIniFile.h"
#include "engine/Kgetinidata.h"

USING_NS_FGUI;

class SelServer : public ax::Scene {
public:
    bool init();
    SelServer();
    virtual ~SelServer();
    CREATE_FUNC(SelServer);

protected:
    GRoot* _groot;

private:
    GComponent* _view;
    void onClickItem(EventContext* context);
    void mianCancelCallback(Object* pSender);

    GList* _list;
    KIniFile nserlist;
};


#endif //JX1CLIENT_SELSERVER_H
