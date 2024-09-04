//
//  KuiMylistView.h
//  KuiMylistView
//
//  Created by �ᵶ����Ц QQ:25557432
//
//
#ifndef __KuiMylistView__
#define __KuiMylistView__

#include <iostream>
#include "cocos2d.h"

USING_NS_AX;


using namespace ui;
class KuiMylistView:public ListView{
public:
	KuiMylistView();
	~KuiMylistView();
	//��ʼ��
	static KuiMylistView* create();

	Point getMoveEndPos(){return moveEndPoint;};
	void    setSpritePiont(Point aPoint);
	void    setSpriteVisible(bool ver);
	void    setSpriteOpacity(int ver);

	bool    getIsRun(){return isRun;};
	void    setIsRun(bool nisRun){isRun=nisRun;};
	void    setRoleIndex(int nIdx){nRoleIndex=nIdx;};
	void    setRolePoint(Point nPoint){nRolePoint=nPoint;};
private:
	//��д����ע�ắ�������¸�����������
	virtual void registerWithTouchDispatcher();
	float getAngleSigned();
	float getRad(Point p1,Point p2);
	KuiMylistView * initWithCenter();
	//��������ccTouchBegan������true
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

#endif /* defined(__KuiMylistView__) */
