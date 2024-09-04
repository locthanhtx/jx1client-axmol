//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCodecLzo.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	LZO Encode and Decode Algorithm
//---------------------------------------------------------------------------
#include "KMemBase.h"
#include "KCodecLzo.h"
//---------------------------------------------------------------------------
// ����:	Compress
// ����:	ѹ������ΪdwInLen������pIn��pOut������ѹ����ĳ���pOutLen
// ����:	pIn			��ѹ������ָ��
//			dwInLen		��ѹ�����ݳ���
//			pOut		ѹ��������ָ��
//			pOutLen		ѹ�������ݳ���
// ����:	ʣ��û��ѹ�����ֽ��� (PBYTE pIn, DWORD dwInLen, PBYTE pOut, PDWORD pOutLen)
//---------------------------------------------------------------------------
DWORD KCodecLzo::Compress(
	 PBYTE	pIn, 		// ��ѹ������
	 DWORD	dwInLen, 	// ��ѹ�����ݳ���
	 PBYTE	pOut,		// ѹ������
	 PDWORD	pOutLen		// ѹ�����ݳ���
	 )
{
	BYTE *ip;
	BYTE *op;
	BYTE *in_end = pIn + dwInLen;
	BYTE *ip_end = pIn + dwInLen - 13;
	BYTE *ii;
	BYTE **dict = (BYTE **)m_WorkMem.GetMemPtr();

	op = pOut;
	ip = pIn;
	ii = ip;
	ip += 4;

	while (TRUE)
	{
		PBYTE m_pos;
		unsigned m_off;
		unsigned m_len;
		unsigned dindex;

		dindex = ((0x21*(((((((unsigned)(ip[3])<<6)^ip[2])<<5)^ip[1])<<5)^ip[0]))>>5) & 0x3fff;
		m_pos = dict [dindex];

		if	(((uintptr_t)m_pos < (uintptr_t)pIn)  ||
			((m_off = (uintptr_t)((uintptr_t)ip-(uintptr_t)m_pos) ) <= 0) ||
			(m_off > 0xbfff))
			goto literal;

		if ((m_off <= 0x0800) || (m_pos[3] == ip[3]))
			goto try_match;

		dindex = (dindex & 0x7ff) ^ 0x201f;

		m_pos  =  dict [dindex];

		if ((uintptr_t)(m_pos)  < (uintptr_t)(pIn)   ||
			((m_off = (uintptr_t)( (int32_t)((uintptr_t)ip-(uintptr_t)m_pos))) <= 0)||
			(m_off > 0xbfff))
			goto literal;

		if ((m_off <= 0x0800) || (m_pos[3] == ip[3]))
			goto try_match;

		goto literal;

try_match:

		if ((*(unsigned short*)m_pos == *(unsigned short*)ip) &&
			(m_pos[2] == ip[2]))
			goto match;

literal:

		dict[dindex] = ip;
		++ip;
		if (ip >= ip_end)
			break;
		continue;

match:

		dict[dindex] = ip;
		if (ip - ii > 0)
		{
			unsigned t = ip - ii;

			if (t <= 3)
				op[-2] |= (BYTE)t;
			else if (t <= 18)
				*op++ = (BYTE) ( t - 3 );
			else
			{
				unsigned tt = t - 18;
				*op++ = 0;
				while (tt > 255)
				{
					tt -= 255;
					*op++ = 0;
				}
				*op++ = (BYTE)tt;
			}
			do { *op++ = *ii++; } while (--t > 0);
		}

		ip += 3;

		if (m_pos[3] != *ip++ || m_pos[4] != *ip++ || m_pos[5] != *ip++ ||
			m_pos[6] != *ip++ || m_pos[7] != *ip++ || m_pos[8] != *ip++ )
		{
			--ip;
			m_len = ip - ii;

			if (m_off <= 0x0800)
			{
				--m_off;
				*op++ = (BYTE) ( ((m_len - 1) << 5) | ((m_off & 7) << 2) );
				*op++ = (BYTE) ( m_off >> 3 );
			}
			else if (m_off <= 0x4000)
			{
				-- m_off;
				*op++ = (BYTE) ( 32  | (m_len - 2) );
				goto m3_m4_offset;
			}
			else
			{
				m_off -= 0x4000;
				*op++ = (BYTE) (16 | ((m_off & 0x4000) >> 11) | (m_len - 2) );
				goto m3_m4_offset;
			}
		}
		else
		{
			{
				BYTE *end = in_end;
				BYTE *m = m_pos + 9;
				while ((ip < end) && (*m == *ip))
				{
					m++, ip++;
				}
				m_len = (ip - ii);
			}

			if (m_off <= 0x4000)
			{
				--m_off;
				if (m_len <= 33)
				{
					*op++ = (BYTE) ( 32  | (m_len - 2));
				}
				else
				{
					m_len -= 33;
					*op++ = 32;
					goto m3_m4_len;
				}
			}
			else
			{
				m_off -= 0x4000;
				if (m_len <= 9 )
				{
					*op++ = (BYTE)(16|((m_off & 0x4000) >> 11) | (m_len - 2));
				}
				else
				{
					m_len -= 9;
					*op++ = (BYTE) (16 | ((m_off & 0x4000) >> 11));
m3_m4_len:
					while (m_len > 255)
					{
						m_len -= 255;
						*op++ = 0;
					}
					*op++ = (BYTE)m_len;
				}
			}

m3_m4_offset:

			*op++ = (BYTE) ( (m_off & 63) << 2 );
			*op++ = (BYTE) ( m_off >> 6 );
		}

		ii = ip;
		if (ip >= ip_end)
			break;
	} // while(TRUE)

	*pOutLen = op - pOut;
	return (DWORD)(in_end - ii);
}
//---------------------------------------------------------------------------
// ����:	Encode
// ����:	ѹ��
// ����:	pCodeInfo	��ѹ������ָ��
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
bool KCodecLzo::Encode(TCodeInfo* pCodeInfo)
{
	PBYTE	op = pCodeInfo->lpPack;
	DWORD	t;
	DWORD	dwPackLen;

	if (!m_WorkMem.Alloc(65536))
		return FALSE;

	if (pCodeInfo->dwDataLen <= 13)
	{
		t = pCodeInfo->dwDataLen;
	}
	else
	{
		t = Compress(pCodeInfo->lpData, pCodeInfo->dwDataLen, op, &dwPackLen);
		op += dwPackLen;
	}

	if (t > 0)
	{
		BYTE *ii = pCodeInfo->lpData + pCodeInfo->dwDataLen - t;
		if (op == pCodeInfo->lpPack && t <= 238)
			*op++ = (BYTE) (17 + t);
		else if (t <= 3)
			op[-2] |= (BYTE)t;
		else if (t <= 18)
			*op++ = (BYTE)(t - 3);
		else
		{
			unsigned tt = t - 18;
			*op++ = 0;
			while (tt > 255)
			{
				tt -= 255;
				*op++ = 0;
			}
			*op++ = (BYTE)tt;
		}
		do { *op++ = *ii++; } while (--t > 0);
	}

	*op++ = 17;
	*op++ = 0;
	*op++ = 0;

	pCodeInfo->dwPackLen = (DWORD)(op - pCodeInfo->lpPack);
	return TRUE;
}
//---------------------------------------------------------------------------
// ����:	Decode
// ����:	��ѹ��
// ����:	pCodeInfo	ѹ������ָ��
// ����:	TRUE		�ɹ�
//			FALSE		ʧ��
//---------------------------------------------------------------------------
bool KCodecLzo::Decode(TCodeInfo* pCodeInfo)
{
	PBYTE op;
	PBYTE ip;
	DWORD t;
	PBYTE m_pos;

	op = pCodeInfo->lpData;
	ip = pCodeInfo->lpPack;

	if (*ip > 17)
	{
		t = *ip++ - 17;
		if (t < 4)
			goto match_next;
		do { *op++ = *ip++; } while (--t > 0);
		goto first_literal_run;
	}

	while (TRUE)
	{
		t = *ip++;
		if (t >= 16) goto match;
		if (t == 0)
		{
			while (*ip == 0)
			{
				t += 255;
				ip++;
			}
			t += 15 + *ip++;
		}

		* (unsigned *) op = * (unsigned *) ip;
		op += 4;
		ip += 4;
		if (--t > 0)
		{
			if (t >= 4)
			{
				do
				{
					* (unsigned *) op = * (unsigned *) ip;
					op += 4;
					ip += 4;
					t -= 4;
				}
				while (t >= 4);
				if (t > 0) do { *op++ = *ip++; } while (--t > 0);
			}
			else do { *op++ = *ip++; } while (--t > 0);
		}

first_literal_run:

		t = *ip++;
		if (t >= 16)
			goto match;

		m_pos = op - 0x0801;
		m_pos -= t >> 2;
		m_pos -= *ip++ << 2;

		*op++ = *m_pos++;
		*op++ = *m_pos++;
		*op++ = *m_pos;

		goto match_done;

		while (TRUE)
		{

match:

		if (t >= 64)
		{
			m_pos = op - 1;
			m_pos -= (t >> 2) & 7;
			m_pos -= *ip++ << 3;
			t = (t >> 5) - 1;
			goto copy_match;
		}
		else if (t >= 32)
		{
			t &= 31;
			if (t == 0)
			{
				while (*ip == 0)
				{
					t += 255;
					ip++;
				}
				t += 31 + *ip++;
			}
			m_pos = op - 1;
			m_pos -= (* (PWORD) ip) >> 2;
			ip += 2;
		}
		else if (t >= 16)
		{
			m_pos = op;
			m_pos -= (t & 8) << 11;
			t &= 7;
			if (t == 0)
			{
				while (*ip == 0)
				{
					t += 255;
					ip++;
				}
				t += 7 + *ip++;
			}
			m_pos -= (* (PWORD) ip) >> 2;
			ip += 2;
			if (m_pos == op)
				goto eof_found;
			m_pos -= 0x4000;
		}
		else
		{
			m_pos = op - 1;
			m_pos -= t >> 2;
			m_pos -= *ip++ << 2;
			*op++ = *m_pos++; *op++ = *m_pos;
			goto match_done;
		}

		if (t >= 6 && (op - m_pos) >= 4)
		{
			* (unsigned *) op = * (unsigned *) m_pos;
			op += 4;
			m_pos += 4;
			t -= 2;
			do
			{
				* (unsigned *) op = * (unsigned *) m_pos;
				op += 4;
				m_pos += 4;
				t -= 4;
			}
			while (t >= 4);
			if (t > 0) do { *op++ = *m_pos++; } while (--t > 0);
		}
		else
		{

copy_match:

		*op++ = *m_pos++;
		*op++ = *m_pos++;
		do { *op++ = *m_pos++; } while (--t > 0);
		}

match_done:

		t = ip[-2] & 3;
		if (t == 0) break;

match_next:

		do { *op++ = *ip++; } while (--t > 0);
		t = *ip++;

		}// while(TRUE)

	}// while(TRUE)

eof_found:

	if (ip != (pCodeInfo->lpPack + pCodeInfo->dwPackLen))
	{
		pCodeInfo->dwDataLen = 0;
		return FALSE;
	}

	pCodeInfo->dwDataLen = (DWORD)(op - pCodeInfo->lpData);
	return TRUE;
}
//---------------------------------------------------------------------------

