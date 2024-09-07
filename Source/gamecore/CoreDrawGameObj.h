#ifndef __COREDRAWGAMEOBJ__H__
#define __COREDRAWGAMEOBJ__H__

struct KLightInfo;

//������Ϸ����
int	CoreDrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam,int inParam=-1);

//��ѯ����Ĺ�Դ����
void	CoreGetGameObjLightInfo(unsigned int uObjGenre, unsigned int uId, KLightInfo *pLightInfo);

#endif //#ifndef __COREDRAWGAMEOBJ__H__
