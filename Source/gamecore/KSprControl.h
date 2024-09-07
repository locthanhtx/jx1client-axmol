
#pragma once

#define		SPR_X_OFF	160
#define		SPR_Y_OFF	190

class KSprControl
{
public:
	int			m_bChange;							// �Ƿ����仯
	int			m_nTotalFrame;						// ��֡��
	int			m_nCurFrame;						// ��ǰ֡
	int			m_nTotalDir;						// �ܷ�����
	int			m_nCurDir;							// ��ǰ����
	unsigned long		m_dwTimer;							// ʱ�������
	unsigned long		m_dwInterval;						// ֡����������ٶȣ�
	int			m_nXpos;							// ���� x
	int			m_nYpos;							// ���� y
	int			m_nCgXpos;							// �������� x
	int			m_nCgYpos;							// �������� y
	char		m_szName[256];						// �ļ���
	unsigned long		m_dwNameID;							// �ļ���id

public:
	KSprControl();									// ���캯��
	void		Release();							// ���
	void		SetFileName(char *lpszName);		// �趨 spr �ļ���
	void		SetTotalFrame(int nTotalFrame);		// �趨��֡��
	void		SetCurFrame(int nCurFrame);			// �趨��ǰ֡
	void		SetTotalDir(int nTotalDir);			// �趨�ܷ�����
	int		SetCurDir(int nDir);				// �趨��ǰ����
	void		SetInterVal(unsigned long dwInterval);		// �趨֡���
	void		SetPos(int x, int y);				// �趨����
	void		SetCenterPos(int x, int y);			// �趨����λ��
	void		SetDirFrame(int nDir, int nFrame);	// �趨ĳ����Ϊ�ڼ�֡
	void		Set64DirFrame(int nDir, int nFrame);// �趨ĳ����Ϊ�ڼ�֡(�������64����ת���������ķ���)
	int		SetCurDir64(int nDir);				// �趨��ǰ����(�������64����ת���������ķ���)
	int			GetOneDirFrames();					// ��õ�����֡��

	void		SetSprFile(							// ���һ�� spr �ļ��ĸ�����Ϣ
		char *lpszName,
		int nTotalFrame,
		int nTotalDir,
		int nInterVal);

	int		GetNextFrame(int bLoop = TRUE);	// �Զ����֡
	int		GetPrevFrame(int bLoop = TRUE);	// �Զ���ǰ��֡
	int		CheckExist();						// �ж��ļ��Ƿ����
	int		CheckEnd();							// �ж϶����Ƿ񲥷ŵ����
	void		SetDirStart();						// �趨��ǰ����ĵ�ǰ֡Ϊ��һ֡
	void		SetDirEnd();						// �趨��ǰ����ĵ�ǰ֡Ϊ���һ֡
	int			GetCurDirFrameNo();					// ��õ�ǰ����ڼ�֡

	// ����
/*	void		DrawAlpha(
						int nX,						// ��Ļ���� x
						int nY,						// ��Ļ���� y
						int nExAlpha = 32);			// alpha ��
	// ���Ʊ߿�
	void		DrawBorder(
						int nX,						// ��Ļ���� x
						int nY,						// ��Ļ���� y
						int nColor);	*/			// ��ɫ
};

//---------------------------------------------------------------------------
//	���ܣ�	�жϿ��ƶ����Ƿ����
//---------------------------------------------------------------------------
inline int KSprControl::CheckExist()
{
	if ((m_nTotalFrame > 0) && m_szName[0])
		return TRUE;

    return FALSE;
}
