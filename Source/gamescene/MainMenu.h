//
// Created by Kull on 10/3/2024.
//

#ifndef JX1CLIENT_MAINMENU_H
#define JX1CLIENT_MAINMENU_H

#include "cocos2d.h"
#include "FairyGUI.h"

USING_NS_FGUI;

class MainMenu : public ax::Scene{
public:
    // implement the "static create()" method manually
    bool init();
    MainMenu();
    virtual ~MainMenu();
    CREATE_FUNC(MainMenu);

protected:
    GRoot* _groot;

private:
    GComponent* _view;
    void onClose(EventContext* context);
    void mianLoginCallback(Object* pSender);
    void mianCancelCallback(Object* pSender);
};


#endif //JX1CLIENT_MAINMENU_H
