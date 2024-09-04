#ifndef __COREDRAWGAMEOBJ__H__
#define __COREDRAWGAMEOBJ__H__

struct KLightInfo;

//������Ϸ����
BOOL	CoreDrawGameObj(uint32_t uObjGenre, uint32_t uId, int x, int y, int Width, int Height, int nParam,int inParam=-1);

//��ѯ����Ĺ�Դ����
void	CoreGetGameObjLightInfo(uint32_t uObjGenre, uint32_t uId, KLightInfo *pLightInfo);

#endif //#ifndef __COREDRAWGAMEOBJ__H__