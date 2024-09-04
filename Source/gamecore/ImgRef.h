#pragma once
#include "cocos2d.h"
USING_NS_AX;
//extern struct iRepresentShell*	g_pRepresent;

void		 IR_UpdateTime();
unsigned int IR_GetCurrentTime();
void		 IR_NextFrame(int& nFrame, int nTotalFrame, unsigned int uInterval, unsigned int& uFlipTime);
