#ifndef __COREDRAWGAMEOBJ__H__
#define __COREDRAWGAMEOBJ__H__

struct KLightInfo;

//绘制游戏对象
BOOL	CoreDrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam,int inParam=-1);

//查询对象的光源属性
void	CoreGetGameObjLightInfo(unsigned int uObjGenre, unsigned int uId, KLightInfo *pLightInfo);

#endif //#ifndef __COREDRAWGAMEOBJ__H__
