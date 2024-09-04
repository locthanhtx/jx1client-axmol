#include "KCore.h"
//#include "../../Represent/iRepresent/iRepresentShell.h"
#include "KSubWorldSet.h"

//iRepresentShell*	g_pRepresent = 0;

uint32_t	l_Time = 0;

uint32_t IR_GetCurrentTime()
{
	return l_Time;
}

//--------------------------------------------------------------------------
//	���ܣ�����ͼ�λ�֡������ʱ��
//--------------------------------------------------------------------------
void IR_UpdateTime()
{//gettimeofday(m_nTimeStop.QuadPart,NULL);
	timeval m_pStartUpdate;
	gettimeofday(&m_pStartUpdate,NULL);
	l_Time = m_pStartUpdate.tv_sec*1000+m_pStartUpdate.tv_usec/1000;//����timeGetTime();���ϵͳ���е�ʱ��
}

int	IR_IsTimePassed(uint32_t uInterval, uint32_t& uLastTimer)
{
	if ((l_Time - uLastTimer) >= uInterval)
	{
		uLastTimer += uInterval;
		return 1;
	}
	return 0;
}
//��ȡʣ��ʱ�䣬���ʱ���Ѿ�����/���ˣ�����ֵ��Ϊ0
uint32_t IR_GetRemainTime(uint32_t uInterval, uint32_t uLastTimer)
{
	uint32_t uRemain;
	if ((uRemain = l_Time - uLastTimer) < uInterval)
		return (uInterval - uRemain);
	return 0;
}
//--------------------------------------------------------------------------
//	���ܣ���֡����
//--------------------------------------------------------------------------
void IR_NextFrame(int& nFrame, int nTotalFrame, uint32_t uInterval, uint32_t& uFlipTime)
{
	if (nTotalFrame > 1 && uInterval)
	{
		while ((l_Time - uFlipTime) >= uInterval)
		{
			uFlipTime += uInterval;
			if ((++nFrame) >= nTotalFrame)
			{
				uFlipTime = l_Time;
				nFrame = 0;
				break;
			}
		}
	}
}
