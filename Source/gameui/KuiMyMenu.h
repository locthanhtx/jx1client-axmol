//
//  PopupLayer.h
//  PopupDemo
//
//  Created by IDEA-MAC03 on 13-10-10.
//
//
#ifndef __KuiMyMenu__
#define __KuiMyMenu__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;


class KuiMyMenu:public Menu{
public:
	KuiMyMenu();
	~KuiMyMenu();
	//��ʼ��
	//virtual bool init();
	//����
	//CREATE_FUNC(KuiMyMenu);
	/** creates a Menu with it's items */
	static KuiMyMenu* create(MenuItem* item, ...);
	static KuiMyMenu* menuWithItem(MenuItem* item);
	// ��дregisterWithTouchDispatcher
	virtual void registerWithTouchDispatcher();

	Point getMoveEndPos(){return moveEndPoint;};
	void    setSpritePiont(Point aPoint);
	void    setSpriteVisible(bool ver);
	void    setSpriteOpacity(int ver);

	bool    getIsRun(){return isRun;};
	void    setIsRun(bool nisRun){isRun=nisRun;};
	void    setRoleIndex(int nIdx){nRoleIndex=nIdx;};
	void    setRolePoint(Point nPoint){nRolePoint=nPoint;};
	void    setSpriteOrder(int ver);
private:
	//����ҡ��
	void Active();
	//���ҡ��
	void Inactive();
	void updatePos(float dt);  //ax::Time

	Point getDirection();
	float   getAngleSigned();
	float   getRad(Point p1,Point p2);
	float   getVelocity();
	//��д����ע�ắ�������¸�����������
	//void registerWithTouchDispatcher();
	KuiMyMenu * initWithCenter();
	//��������ccTouchBegan������true
	/*bool ccTouchBegan(Touch * touch,Event * pevent);
	void ccTouchEnded(Touch* touch, Event* pevent);
	void ccTouchCancelled(Touch *touch, Event* pevent);
	void ccTouchMoved(Touch* touch, Event* pevent);*/
	bool ccTouchBegan(Touch* touch, Event* event);
	void ccTouchEnded(Touch* touch, Event* event);
	void ccTouchCancelled(Touch *touch, Event* event);
	void ccTouchMoved(Touch* touch, Event* event);

	Point nRolePoint;
	int     nRoleIndex;

	Point moveEndPoint;
	Point centerPoint; //ҡ������
	Point currentPoint;//ҡ�˵�ǰλ��
	bool    isRun;
	bool    active;//�Ƿ񼤻�ҡ��
	float   radius;//ҡ�˰뾶
	Sprite *jsSprite;
	Sprite *bgSprite;
	bool    isFollowRole;//�Ƿ�����û����

	int     _BackAttackRadius;
	int     _CurSkillId;
	float   _Beilv;
	int     _useSkillModel;
};

#endif /* defined(__MyGame__PopLayer__) */
