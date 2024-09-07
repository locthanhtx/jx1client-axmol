#include "KCore.h"
#include "KItem.h"
#include "KInventory.h"

KInventory::KInventory()
{
	m_pArray = NULL;
	m_nWidth = 0;
	m_nHeight= 0;
	m_nBakWidth = 0;
	m_nBakHeight = 0;
}

KInventory::~KInventory()
{
    Release();
}

int KInventory::Init(int nWidth, int nHeight)
{
	if (m_pArray)
	{
		delete [] m_pArray;
		m_pArray = NULL;
	}
	m_pArray = new int[nWidth * nHeight];

	if (!m_pArray)
		return FALSE;

	//ZeroMemory(m_pArray, sizeof(int) * nWidth * nHeight);
	memset(m_pArray, 0,sizeof(int) * nWidth * nHeight);

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBakWidth = nWidth;
	m_nBakHeight = nHeight;
	return TRUE;
}

void KInventory::Release()
{
	if (m_pArray)
	{
		delete []m_pArray;
		m_pArray = NULL;
	}
	this->m_nWidth = 0;
	this->m_nHeight = 0;
	this->m_nBakWidth = 0;
	this->m_nBakHeight = 0;
	this->m_nMoney = 0;
	this->m_nXu = 0;
}

void KInventory::Clear()
{
	if (m_pArray)
		memset(m_pArray, 0, sizeof(int) * m_nWidth * m_nHeight);
	m_nMoney = 0;
	m_nXu    = 0;
}

#pragma optimize( "y", off)
//�����Ʒ�Ƿ��ܷŽ�������λ�����Ƿ�����Ʒ,�������ø�λ�õı�ʾΪidx
int KInventory::PlaceItem(int nX, int nY, int nIdx, int nWidth, int nHeight)
{//m_nWidth m_nHeight Ϊ�����Ĵ�С
	if (!m_pArray)//�������
		return FALSE;

	if (nX < 0 || nY < 0 || nWidth <= 0 || nHeight <= 0 || nX + nWidth > m_nWidth || nY + nHeight > m_nHeight)
		return FALSE;

	if (nIdx <= 0)
	{
		//_ASSERT(0);
		return FALSE;
	}

	int i, j;
	/*int nOldIdx = 0;

	for (i = nX; i < nX + nWidth; i++)  //��������
	{//��� �Ƿ��ܰѶ����Ž�ȥ
		for (j = nY; j < nY + nHeight; j++)  //�����߶�
		{
			//if (!nOldIdx) //Ϊ0
			nOldIdx = m_pArray[j * m_nWidth + i]; //��λ���ϵı�ʾidx

			if (nOldIdx)
			{//λ�����ж���
				 return FALSE;
			}
		}
	}
	*/
	//������Ʒ�����������idx ��ʾ
	for (i = nX; i < nX + nWidth; i++)
	{
		for (j = nY; j < nY + nHeight; j++)
		{
			m_pArray[j * m_nWidth + i] = nIdx; //������Ʒ�����������idx ��ʾ
		}
	}

	return TRUE;
}


void KInventory::DirectSetPosition(int nIdx, int nX, int nY,int nWidth,int nHeight)
{
     //������Ʒ�����������idx ��ʾ
	int i, j;
	for (i = nX; i < nX + nWidth; i++)
	{
		for (j = nY; j < nY + nHeight; j++)
		{
			m_pArray[j * m_nWidth + i] = nIdx; //������Ʒ�����������idx ��ʾ
		}
	}
}





//�����Ʒ�Ž�����ʱ�����ø�λ�õı�ʾΪidx
int KInventory::SetEmptyPlaceItemIdx(int nIdx, int nWidth, int nHeight,POINT *pPos)
{//m_nWidth m_nHeight Ϊ�����Ĵ�С
	if (!m_pArray)//�������
		return FALSE;
	if (nWidth < 1 || nHeight < 1)
		return FALSE;
	if (nIdx <= 0)
	{
		//_ASSERT(0);
		return FALSE;
	}

	int i, j;

	if (FindEmptyPlace(nWidth,nHeight,pPos)) //���ҿ�λ��
	{
		//������Ʒ�����������idx ��ʾ
		for (i = pPos->x; i < pPos->x + nWidth; i++)
		{
			for (j = pPos->y; j < pPos->y + nHeight; j++)
			{
				m_pArray[j * m_nWidth + i] = nIdx; //������Ʒ�����������idx ��ʾ
			}
		}
		return TRUE;
	}

	return FALSE;
}

#pragma optimize( "", on)

int KInventory::HoldItem(int nIdx, int nWidth, int nHeight)
{//ʣ�������ռ��Ƿ��ܷ��¸���Ʒ
	int i, j;
	for (i = 0; i < m_nWidth - nWidth + 1; i++)
	{
		for (j = 0; j < m_nHeight - nHeight + 1; j++)
		{
			if (PlaceItem(i, j, nIdx, nWidth, nHeight))
				return TRUE;
		}
	}
	return FALSE;
}
//���idx�Ƿ���λ���ϵı�ʾ��ͬ������Ƿ����������Ʒ
int	KInventory::PickUpItem(int nIdx, int nX, int nY, int nWidth, int nHeight)
{
	if (nX < 0 || nY < 0 || nWidth < 1  || nHeight < 1 || nX + nWidth > this->m_nWidth || nY + nHeight > this->m_nHeight)
		return FALSE;

	int	i;
/*	for (i = nX; i < nX + nWidth; i++) //λ�ü�+��Ʒ�Ŀ���
	{//����Ƿ����������Ʒ ���ֲ���Խ��
		for (int j = nY; j < nY + nHeight; j++)  //λ��+��Ʒ�ĸ߶�
		{
			if (m_pArray[j * m_nWidth + i] != nIdx)  //��������Ʒ��ʾ �Ƿ���ͬ
			{
				//_ASSERT(0);
				return FALSE;
			}
		}
	}*/
//��������ø�λ��Ϊ��
	for (i = nX; i < nX + nWidth; i++)
	{
		for (int j = nY; j < nY + nHeight; j++)
		{
			m_pArray[j * m_nWidth + i] = 0;
		}
	}

	return TRUE;
}

//ȫ������Ϊ�� ����
int KInventory::SetRoomNull()
{
	if ( this->m_nWidth <=0 || this->m_nHeight <=0)
		return FALSE;

	int		i;

//��������ø�λ��Ϊ��
	for (i = 0; i < m_nWidth; i++)
	{
		for (int j = 0; j < m_nHeight; j++)
		{
			m_pArray[j * m_nWidth + i] = 0;
		}
	}

	return TRUE;
}


int	KInventory::FindItem(int nX, int nY)
{
	if (!m_pArray)
		return -1;
	if (nX < 0 || nX >= this->m_nWidth || nY < 0 || nY >= this->m_nHeight)
		return -1;

	int	nPos = nY * m_nWidth + nX; //=m_pArray[j * m_nWidth + i]
	int	*pArray = &m_pArray[nPos];  //���λ���ϵı�ʾ
	if (*pArray <= 0)
		return 0;
	int	nIdx = *pArray;

	//if ((nPos > 0 && *(pArray - 1) == nIdx) || (nPos >= m_nWidth && *(pArray - m_nWidth) == nIdx))
	//	return -1;

	return nIdx;

/*
	for (int i = 0; i < nY * m_nWidth + nX; i++)
	{
		if (m_pArray[i] == nIdx)
			return -1;
	}

	return nIdx;
*/

}

//��ȡ ĳ������ �� �߶ȿ��еİ�������
int KInventory::GetFreeRoomCount(int nWidth, int nHeight)
{
	int nCount =0;
	if (nWidth <= 0 || nWidth > m_nWidth || nHeight <= 0 || nHeight > m_nHeight)
		return FALSE;

	int i, j;
	for (i = 0; i < m_nWidth - nWidth + 1; i++)
	{//���������Ŀ���
		for (j = 0; j < m_nHeight - nHeight + 1; j++)
		{//���������ĸ߶�
			if (CheckRoom(i,j,nWidth,nHeight))
			{
				nCount ++;
				//return TRUE;
			}
		}
	}

	return nCount;
}

int KInventory::FindRoom(int nWidth, int nHeight, POINT* pPos)
{
	if (!pPos)
		return FALSE;
	if (nWidth <= 0 || nWidth > m_nWidth || nHeight <= 0 || nHeight > m_nHeight)
		return FALSE;

	int i, j;
	for (i = 0; i < m_nWidth - nWidth + 1; i++)
	{//���������Ŀ���
		for (j = 0; j < m_nHeight - nHeight + 1; j++)
		{//���������ĸ߶�
			if (CheckRoom(i,j,nWidth,nHeight))
			{
				pPos->x = i;
				pPos->y = j;
				return TRUE;
			}
		}
	}
	pPos->x = 0;
	pPos->y = 0;
	return FALSE;
}
//������λ���Ƿ��ж���
int KInventory::CheckRoom(int nX, int nY, int nWidth, int nHeight)
{
	if (!m_pArray)
	{
		//_ASSERT(0);
		return FALSE;
	}
//��Ʒ��СΪ 1 ��
	if (nX < 0 || nY < 0 || nWidth <=0 || nHeight <=0 || nX + nWidth > m_nWidth || nY + nHeight > m_nHeight)
		return FALSE;

	int i, j;
	int nOldIdx = 0;

	for (i = nX; i < nX + nWidth; i++)
	{
		for (j = nY; j < nY + nHeight; j++)
		{
			if (m_pArray[j * m_nWidth + i])//�������Ʒ �ͷ��ؼ�
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

int KInventory::AddmClient(int nMoney)
{
	if (m_nMoney + nMoney < 0)
		return FALSE;
	m_nMoney += nMoney;
	return TRUE;
}

int KInventory::AddxCliet(int ni)
{
	if (m_nXu + ni < 0)
		return FALSE;
	m_nXu += ni;
	return TRUE;
}

int		KInventory::GetNextItem(int nStartIdx, int nXpos, int nYpos, int *pX, int *pY)
{
	if (!m_pArray)
		return 0;

	if  (m_nWidth<=0)
		return 0;

	if (nXpos < 0 || nYpos < 0 || nXpos >= m_nWidth || nYpos >= m_nHeight || !pX || !pY)
		return 0;
	int		nSize = m_nWidth * m_nHeight;
	int		i = nYpos * m_nWidth + nXpos;
	int		*pArray = &m_pArray[i];
	for ( ; i < nSize; i++, pArray++)
	{
		if (*pArray <= 0)
			continue;
		if (*pArray == nStartIdx)
			continue;
		if (i < m_nWidth || pArray[-m_nWidth] != *pArray)
		{
			*pX = i % m_nWidth;
			*pY = i / m_nWidth;
			return *pArray;
		}
	}
	return 0;
}


int		KInventory::GetCurIndexItem(int nCurCount)
{
	if (!m_pArray)
		return 0;

	int		nNum = 0;
	int		nCurIdx = 0;
	int		nSize = m_nWidth * m_nHeight;
	int		*pArray = m_pArray;
	int     nItemCount =0;
	for (int i = 0; i < nSize; i++)
	{
		if (*pArray <= 0)
		{//û�ж���
			pArray++;
			continue;
		}

		if (nCurIdx == *pArray)
		{
			pArray++;
			continue;
		}

		if (i < m_nWidth || pArray[-m_nWidth] != *pArray)
		{
			nItemCount ++;
			nCurIdx = *pArray;
			if  (nItemCount==nCurCount)
				break;
			//if (Item[nCurIdx].GetGenre() == nGenre && Item[nCurIdx].GetDetailType() == nDetail)
			//	nNum += Item[nCurIdx].GetStackNum();
		}
		pArray++;
	}

	return nCurIdx;
}

//���������͵� ��������
int		KInventory::CalcSameDetailType(int nGenre, int nDetail)
{
	if (!m_pArray)
		return 0;
	int		nNum = 0;
	int		nCurIdx = 0;
	int		nSize = m_nWidth * m_nHeight;
	int		*pArray = m_pArray;
	for (int i = 0; i < nSize; i++)
	{
		if (*pArray <= 0)
		{
			pArray++;
			continue;
		}
		if (nCurIdx == *pArray)
		{
			pArray++;
			continue;
		}
		if (i < m_nWidth || pArray[-m_nWidth] != *pArray)
		{
			nCurIdx = *pArray;
			if (Item[nCurIdx].GetGenre() == nGenre && Item[nCurIdx].GetDetailType() == nDetail)
				nNum += Item[nCurIdx].GetStackNum();
		}
		pArray++;
	}

	return nNum;
}
//���ҿ��пռ�
int	KInventory::FindEmptyPlace(int nWidth, int nHeight, POINT *pPos)
{
	if (!m_pArray)
		return FALSE;
	if (!pPos || nWidth <= 0 || nHeight <= 0 || nWidth > m_nWidth || nHeight > m_nHeight)
		return FALSE;

	int		i, j, nIdx, a, b, nFind;
	for (i = 0; i < m_nHeight - nHeight + 1; i++)
	{
		for (j = 0; j < m_nWidth - nWidth + 1; )
		{
			nIdx = m_pArray[i * m_nWidth + j];  //�������Ʒ
			if (nIdx)
			{
				//_ASSERT(Item[nIdx].GetWidth() > 0);
				if (Item[nIdx].GetWidth()>0)
				   j += Item[nIdx].GetWidth();   //��������Ʒ��λ��
			}
			else
			{
				nFind = 1;
				for (a = i; a < i + nHeight; a++) //�߶�
				{
					for (b = j; b < j + nWidth; b++)
					{
						if (m_pArray[a * m_nWidth + b]) //����ж���
						{
							nFind = 0;
							break;
						}
					}
					if (nFind == 0)
						break;
				}
				//������Ʒ�� ���Ⱥͳ��ȼ����� �к��ʾ�����λ��
				if (nFind)
				{
					pPos->x = b;
					pPos->y = a;
					return TRUE;
				}
				j++;
			}
		}
	}

	return FALSE;
}
//---------------------------------------------------------------------------------
// ���ܣ�������Ʒ���ͺ;������ͣ��쿴Inventory������û����ͬ����Ʒ�����λ�úͱ��
//---------------------------------------------------------------------------------
int	KInventory::FindSameDetailType(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY)
{
	if (!m_pArray)
		return FALSE;

	if  (m_nWidth<=0)
		return FALSE;

	if (!pnIdx || !pnX || !pnY)
		return FALSE;

	int		*pArray = m_pArray;
	int		i, nSize = m_nWidth * m_nHeight;

	for (i = 0; i < nSize; i++, pArray++)
	{
		if (*pArray <= 0)
			continue;
		if (Item[*pArray].GetGenre() == nGenre && Item[*pArray].GetDetailType() == nDetail)
		{
			*pnIdx = *pArray;
			*pnX = i % m_nWidth;
			*pnY = i / m_nWidth;
			return TRUE;
		}
	}

	return FALSE;

/*
	int j;

	for (i = 0; i < this->m_nHeight; i++)
	{
		for (j = 0; j < this->m_nWidth; j++)
		{
			if (!m_pArray[i * m_nWidth + j])
				continue;
			if (Item[m_pArray[i * m_nWidth + j]].GetGenre() != nGenre)
				continue;
			if (Item[m_pArray[i * m_nWidth + j]].GetDetailType() != nDetail)
				continue;
			*pnIdx = m_pArray[i * m_nWidth + j];
			*pnX = j;
			*pnY = i;
			return TRUE;
		}
	}

	return FALSE;
*/
}

//---------------------------------------------------------------------------------
// ��������ܣ�������Ʒ���ͺ;������ͣ��쿴Inventory������û����ͬ����Ʒ
//---------------------------------------------------------------------------------
int	KInventory::CheckSameDetailType(int nGenre, int nDetail,int ParticularType)
{
	if (!m_pArray)
		return FALSE;

	int		*pArray = m_pArray;
	int		i, nSize = m_nWidth * m_nHeight;

	for (i = 0; i < nSize; i++, pArray++)
	{
		if (*pArray <= 0)
			continue;
		if (Item[*pArray].GetGenre() == nGenre && Item[*pArray].GetDetailType() == nDetail && Item[*pArray].GetParticular()==ParticularType)
			return TRUE;
	}

	return FALSE;
}
