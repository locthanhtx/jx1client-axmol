//
// Created by Administrator on 10/5/2024.
//

#ifndef JX1CLIENT_SELPLAYER_H
#define JX1CLIENT_SELPLAYER_H

#include "cocos2d.h"
#include "FairyGUI.h"

USING_NS_FGUI;

class SelPlayer : public ax::Scene{
public:
    // implement the "static create()" method manually
    bool init();
    SelPlayer();
    virtual ~SelPlayer();
    CREATE_FUNC(SelPlayer);
    GComponent* getView() {
        return _view;
    };

protected:
    GRoot* _groot;

private:
    GComponent* _view;
};


#endif //JX1CLIENT_SELPLAYER_H
