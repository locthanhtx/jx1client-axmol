#include "KCore.h"
#include "KSubWorld.h"
#include "KSubWorldSet.h"
#include "KSprControl.h"
//#include "MyAssert.h"
//---------------------------------------------------------------------------
//	���ܣ�	���캯��
//---------------------------------------------------------------------------
KSprControl::KSprControl()
{
	Release();
}

//---------------------------------------------------------------------------
//	���ܣ�	��գ���ʼ��
//---------------------------------------------------------------------------
void	KSprControl::Release()
{
	m_bChange = TRUE;
	m_nTotalFrame = 1;
	m_nCurFrame = 0;
	m_nTotalDir = 1;
	m_nCurDir = 0;
	m_dwTimer = 0;
	m_dwInterval = 1;
	m_nXpos = 0;
	m_nYpos = 0;
	m_nCgXpos = 0;
	m_nCgYpos = 0;
	m_szName[0] = 0;
	m_dwNameID = 0;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨���ƶ���
//---------------------------------------------------------------------------
void	KSprControl::SetSprFile(char *lpszName, int nTotalFrame, int nTotalDir, int nInterVal)
{
	if (!lpszName)
		return;

	if (!lpszName[0])
	{
		Release();
		return;
	}

	if (strcmp(m_szName, lpszName) == 0)
		return;

	strcpy(m_szName, lpszName);
	m_dwNameID = g_FileName2Id(m_szName);
	m_nTotalDir = nTotalDir;

	if (m_nTotalDir < 1)
		m_nTotalDir = 1;

	if (nTotalFrame < m_nTotalDir)
	{
		m_nTotalFrame = m_nTotalDir;
	}
	else
	{
		m_nTotalFrame = nTotalFrame;
	}

	if (m_nTotalFrame < 1)
		m_nTotalFrame = 1;

	m_dwInterval = nInterVal;
	m_bChange = TRUE;
	this->m_dwTimer = SubWorld[0].m_dwCurrentTime;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��ǰ֡
//---------------------------------------------------------------------------
void	KSprControl::SetCurFrame(int nCurFrame)
{
	if (nCurFrame < 0 || nCurFrame >= m_nTotalFrame)
		return;

	m_nCurFrame = nCurFrame;

	if  (m_nCurFrame<=0)
		m_nCurFrame =0;

	if (m_nTotalFrame && m_nTotalDir)
		m_nCurDir = nCurFrame / (m_nTotalFrame / m_nTotalDir);
	else
		m_nCurDir = 0;

	m_dwTimer = SubWorld[0].m_dwCurrentTime;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨ĳ����Ϊ�ڼ�֡
//---------------------------------------------------------------------------
void	KSprControl::SetDirFrame(int nDir, int nFrame)
{
	if (nDir < 0 || nDir >= m_nTotalDir)
		return;
	m_nCurDir = nDir;
	if (nFrame < 0 || nFrame >= m_nTotalFrame / m_nTotalDir)
		m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * nDir;
	else
		m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * nDir + nFrame;
	m_dwTimer = SubWorld[0].m_dwCurrentTime;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨ĳ����Ϊ�ڼ�֡(�������64����ת���������ķ���)
//---------------------------------------------------------------------------
void	KSprControl::Set64DirFrame(int nDir, int nFrame)
{
	if (nDir < 0 || nDir >= 64)
		return;
	m_nCurDir = (nDir + (32 / m_nTotalDir)) / (64 / m_nTotalDir);
	if (m_nCurDir >= m_nTotalDir)
		m_nCurDir -= m_nTotalDir;
	if (nFrame < 0 || nFrame >= m_nTotalFrame / m_nTotalDir) //12
		m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * nDir;
	else
		m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * nDir + nFrame;

	m_dwTimer = SubWorld[0].m_dwCurrentTime;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��ǰ����
//---------------------------------------------------------------------------
int	KSprControl::SetCurDir(int nDir)
{
	if (m_nCurDir == nDir)
		return TRUE;
	if (nDir < 0 || nDir >= m_nTotalDir)
		return FALSE;
	m_nCurDir = nDir;
	m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * nDir;
	m_dwTimer = SubWorld[0].m_dwCurrentTime;
	return FALSE;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��ǰ����(�������64����ת���������ķ���)
//---------------------------------------------------------------------------
int	KSprControl::SetCurDir64(int nDir)
{
	if (nDir < 0 || nDir >= 64)
		return FALSE;

	int nTempDir;

	 nTempDir = (nDir + (32 / m_nTotalDir)) / (64 / m_nTotalDir);
	if (nTempDir >= m_nTotalDir)
		nTempDir -= m_nTotalDir;
	if (m_nCurDir == nTempDir)
		return TRUE;
	m_nCurDir = nTempDir;
	m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * nTempDir;
	m_dwTimer = SubWorld[0].m_dwCurrentTime;
	return FALSE;

}

//---------------------------------------------------------------------------
//	���ܣ�	�趨����λ��
//---------------------------------------------------------------------------
void	KSprControl::SetCenterPos(int x, int y)
{
	m_nCgXpos = x, m_nCgYpos = y;
}

//---------------------------------------------------------------------------
//	���ܣ�	��õ�����֡��
//---------------------------------------------------------------------------
int		KSprControl::GetOneDirFrames()
{
	return m_nTotalFrame / m_nTotalDir;
}


//---------------------------------------------------------------------------
//	���ܣ�	�趨λ��
//---------------------------------------------------------------------------
void	KSprControl::SetPos(int x, int y)
{
	m_nXpos = x;
	m_nYpos = y;
}


//---------------------------------------------------------------------------
//	���ܣ�	�ж϶����Ƿ񲥷ŵ���󣬵�ǰ�ǵ� 0 ֡
//---------------------------------------------------------------------------
int	KSprControl::CheckEnd()
{
	if (m_nCurFrame == (m_nTotalFrame / m_nTotalDir) * (m_nCurDir + 1) - 1)
		return TRUE;
	return FALSE;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��ǰ����ĵ�ǰ֡Ϊ��һ֡
//---------------------------------------------------------------------------
void	KSprControl::SetDirStart()
{
	m_nCurFrame = m_nCurDir * (m_nTotalFrame / m_nTotalDir);   //(96/8=12*(0-63))
	this->m_dwTimer = SubWorld[0].m_dwCurrentTime;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��ǰ����ĵ�ǰ֡Ϊ���һ֡
//---------------------------------------------------------------------------
void	KSprControl::SetDirEnd()
{
	m_nCurFrame = (m_nCurDir + 1) * (m_nTotalFrame / m_nTotalDir) - 1;
}

//---------------------------------------------------------------------------
//	���ܣ�	��õ�ǰ����ڼ�֡
//---------------------------------------------------------------------------
int		KSprControl::GetCurDirFrameNo()
{
	return m_nCurFrame - m_nCurDir * (m_nTotalFrame / m_nTotalDir);
}

//---------------------------------------------------------------------------
//	���ܣ�	ȡ�õ�ǰ�������һ֡
//	������bLoop		�Ƿ�ѭ������ TRUE ѭ������ FALSE ���ŵ����һֹ֡ͣ
//	����ֵ��FALSE �ļ����������bLoop==FALSEʱ���ŵ����һ֡(����֪ͨ���������������������)
//			TRUE otherwise
//---------------------------------------------------------------------------
int	KSprControl::GetNextFrame(int bLoop)
{
	/*if ( !m_szName[0] )
		return FALSE;

	int bRetVal = TRUE;

	if (m_dwInterval <= 0)
		m_dwInterval = 1;

	_ASSERT(SubWorld[0].m_dwCurrentTime >= m_dwTimer);

	if (SubWorld[0].m_dwCurrentTime - m_dwTimer >= m_dwInterval)
	{
		if (bLoop)
		{
			m_dwTimer = SubWorld[0].m_dwCurrentTime;
			m_nCurFrame = (m_nTotalFrame/m_nTotalDir) * m_nCurDir;
			//m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * (m_nCurDir + 1) - 1;
			bRetVal = TRUE;
		}
		else
		{
			m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * (m_nCurDir + 1) - 1;
			bRetVal = TRUE;
		}
	}
	else
	{
		m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * m_nCurDir + (m_nTotalFrame / m_nTotalDir) * (SubWorld[0].m_dwCurrentTime - m_dwTimer) /m_dwInterval;
		bRetVal = TRUE;
	}

	return bRetVal;*/

	if ( !m_szName[0] )
		return FALSE;

	int bRetVal = TRUE;

	if (m_dwInterval <= 0)
		m_dwInterval = 1;


	if (SubWorld[0].m_dwCurrentTime != m_dwTimer)
	{
		m_dwTimer = SubWorld[0].m_dwCurrentTime;
		m_nCurFrame ++;
		INT nDirFrame = m_nTotalFrame / m_nTotalDir;	// ÿ���������	  12   ÿ�������֡�� �͸ı䷽��
		if (m_nCurFrame >= nDirFrame * (m_nCurDir + 1))
		{
			if (bLoop)
			{
				m_nCurFrame = nDirFrame * m_nCurDir;    // 12 *
			}
			else
			{
				m_nCurFrame = nDirFrame * (m_nCurDir + 1) - 1;
				bRetVal	= FALSE;
			}
		}
	}

	return bRetVal;
}

//---------------------------------------------------------------------------
//	���ܣ�	ȡ�õ�ǰ�����ǰһ֡
//---------------------------------------------------------------------------
int	KSprControl::GetPrevFrame(int bLoop)
{
	/*if (!m_szName[0])
		return FALSE;
	if (SubWorld[0].m_dwCurrentTime - m_dwTimer >= m_dwInterval)
	{
		m_dwTimer = SubWorld[0].m_dwCurrentTime;
		m_nCurFrame--;
		if (m_nCurFrame < (m_nTotalFrame / m_nTotalDir) * m_nCurDir)
		{
			if (bLoop)
				m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * (m_nCurDir + 1) - 1;
			else
				m_nCurFrame = (m_nTotalFrame / m_nTotalDir) * m_nCurDir;
		}
		return TRUE;
	}

	return FALSE; */
	if (!m_szName[0])
		return FALSE;

	int bRetVal = TRUE;

	if (SubWorld[0].m_dwCurrentTime != m_dwTimer)
	{
		m_dwTimer = SubWorld[0].m_dwCurrentTime;
		m_nCurFrame--;
		INT nDirFrame = m_nTotalFrame / m_nTotalDir;	// ÿ���������
		if (m_nCurFrame < nDirFrame * m_nCurDir)
		{
			if (bLoop)
			{
				m_nCurFrame	= nDirFrame * (m_nCurDir + 1) - 1;
			}
			else
			{
				m_nCurFrame	= nDirFrame * m_nCurDir;
				bRetVal = FALSE;
			}
		}
	}

	return bRetVal;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨 spr �ļ���
//---------------------------------------------------------------------------
void	KSprControl::SetFileName(char *lpszName)
{
	if (!lpszName)
		return;

	if (!lpszName[0])
		Release();
	else
	{
		strcpy(m_szName, lpszName);
		m_bChange = TRUE;
	}
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨��֡��
//---------------------------------------------------------------------------
void	KSprControl::SetTotalFrame(int nTotalFrame)
{
	if (nTotalFrame > 0)
		m_nTotalFrame = nTotalFrame;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨�ܷ�����
//---------------------------------------------------------------------------
void	KSprControl::SetTotalDir(int nTotalDir)
{
	if (nTotalDir > 0)
		m_nTotalDir = nTotalDir;
}

//---------------------------------------------------------------------------
//	���ܣ�	�趨֡���
//---------------------------------------------------------------------------
void	KSprControl::SetInterVal(unsigned long dwInterval)
{
	m_dwInterval = dwInterval;
}
/*
//---------------------------------------------------------------------------
//	���ܣ�	����  ---ԭ��û�е�
//---------------------------------------------------------------------------
void	KSprControl::DrawAlpha(
							   int nX,			// ��Ļ���� x
							   int nY,			// ��Ļ���� y
							   int nExAlpha)	// alpha ��
{
	KCacheNode	*pSprNode = NULL;
	KSprite		*pSprite;

	pSprNode = (KCacheNode *)g_SpriteCache.GetNode(m_szName, (KCacheNode *)pSprNode);
	pSprite = (KSprite*)pSprNode->m_lpData;
	if (pSprite)
		pSprite->DrawAlpha( nX - m_nCgXpos, nY - m_nCgYpos, m_nCurFrame, nExAlpha );
}

//---------------------------------------------------------------------------
//	���ܣ�	���Ʊ߿�
//---------------------------------------------------------------------------
void	KSprControl::DrawBorder(
								int nX,			// ��Ļ���� x
								int nY,			// ��Ļ���� y
								int nColor)		// ��ɫ
{
	KCacheNode	*pSprNode = NULL;
	KSprite		*pSprite;

	pSprNode = (KCacheNode *)g_SpriteCache.GetNode(m_szName, (KCacheNode *)pSprNode);
	pSprite = (KSprite*)pSprNode->m_lpData;
	if (pSprite)
		pSprite->DrawBorder( nX - m_nCgXpos, nY - m_nCgYpos, m_nCurFrame, nColor );
}
*/
