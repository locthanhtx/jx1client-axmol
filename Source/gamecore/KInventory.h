#ifndef KInventoryH
#define	KInventoryH

class  KInventory
{
	friend	class	KItemList;

private:
	int*	m_pArray;
	int		m_nMoney;
	int		m_nWidth;  //�������
	int		m_nHeight; //�����߶�
	int		m_nBakWidth;  //���ݵ��������
	int		m_nBakHeight; //���ݵ������߶�
	int		m_nXu;

public:
	KInventory();
    ~KInventory();

	BOOL	Init(int nWidth, int nHeight);
	void	Release();
	void	Clear();
	BOOL	PickUpItem(int nIdx, int nX, int nY, int nWidth, int nHeight);//���idx�Ƿ���λ���ϵı�ʾ��ͬ,��������Ϊλ�ñ�ʾΪ0 ������
	BOOL    SetRoomNull();
	BOOL    SetEmptyPlaceItemIdx(int nIdx, int nWidth, int nHeight,POINT *pPos);
	int		FindItem(int nX, int nY);
	BOOL	PlaceItem(int nXpos, int nYpos, int nIdx, int nWidth, int nHeight);//�����Ʒ�Ž�����ʱ��λ�����Ƿ�����Ʒ,�������ø�λ�õı�ʾΪidx
	void    DirectSetPosition(int nIdx, int nX, int nY,int nWidth,int nHeight);
	
	BOOL	HoldItem(int nIdx, int nWidth, int nHeight);  //ʣ�������ռ��Ƿ��ܷ��¸���Ʒ
	BOOL	FindRoom(int nWidth, int nHeight, POINT* pPos);
	BOOL	CheckRoom(int nXpos, int nYpos, int nWidth, int nHeight);
	int     GetFreeRoomCount(int nWidth, int nHeight);
	BOOL	AddmClient(int nMoney);
	int		GetmServer() { return m_nMoney; }
	int     GetWidth() { return m_nWidth; }
	int     GetHeight() { return m_nHeight; }
	// int     GetWidth() { return 1; }
	// int     GetHeight() { return 1; }
	BOOL	SetClientMon(int nMon) { if (nMon < 0) return FALSE; m_nMoney = nMon; return TRUE; }
	//Xu
	BOOL	AddxCliet(int ni);
	int		GetxCLient() { return m_nXu; }
	BOOL	SetxClient(int ni) { if (ni < 0) return FALSE; m_nXu = ni; return TRUE; }
	////////////////////
	int		GetNextItem(int nStartIdx, int nXpos, int nYpos, int *pX, int *pY);
	int		CalcSameDetailType(int nGenre, int nDetail);
	int		GetCurIndexItem(int nCurCount);
	BOOL	FindEmptyPlace(int nWidth, int nHeight, POINT *pPos);

	// ������Ʒ���ͺ;������ͣ��쿴Inventory������û����ͬ����Ʒ�����λ�úͱ��
	BOOL	FindSameDetailType(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY);
	// ������Ʒ���ͺ;������ͣ��쿴Inventory������û����ͬ����Ʒ
	BOOL	CheckSameDetailType(int nGenre, int nDetail,int ParticularType);
};
#endif //KInventoryH
