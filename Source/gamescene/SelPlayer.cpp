//
// Created by Administrator on 10/5/2024.
//

#include "SelPlayer.h"

bool SelPlayer::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _groot = GRoot::create(this, 0);
    _groot->retain();

    UIPackage::addPackage("FGUI/SelPlayer");
    _view = UIPackage::createObject("SelPlayer", "Main")->as<GComponent>();
    _groot->addChild(_view);
    _view->getChild("bt_tao")->addClickListener([this](EventContext*) {
        CCLOG("Button clicked");
        // Không ngăn chặn sự kiện lan tỏa
    });

    return true;
}

SelPlayer::SelPlayer() : _groot(nullptr), _view(nullptr)
{
}

SelPlayer::~SelPlayer()
{
    AX_SAFE_RELEASE(_groot);
}
