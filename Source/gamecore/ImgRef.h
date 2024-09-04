#pragma once
#include "cocos2d.h"
USING_NS_AX;
//extern struct iRepresentShell*	g_pRepresent;

void		 IR_UpdateTime();
uint32_t IR_GetCurrentTime();
void		 IR_NextFrame(int& nFrame, int nTotalFrame, uint32_t uInterval, uint32_t& uFlipTime);
