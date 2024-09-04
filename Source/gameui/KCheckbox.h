#ifndef __MyGame__CCCheckbox__
#define __MyGame__CCCheckbox__

#include "cocos2d.h"
#include "engine/KGbktoUtf8.h"
using namespace ax;

class KCheckbox:public Sprite
{
public:
	//����checkbox
	static Sprite * create(const char * picNoSelect,const char * picSelect,char * content  ,int status = 0,int nKind=0);

	//�������
	int click();

private:
	//��ȡ��ǰ״̬
	int getStatus();
};

#endif /* defined(__MyGame__CCCheckbox__) */
