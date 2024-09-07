//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KSG_EncodeDecode.cpp
//  Version     :   1.0
//  Creater     :
//  Date        :   2003-6-3 10:28:57
//  Comment     :
//
//////////////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "KSG_EncodeDecode.h"

int KSG_DecodeEncode(unsigned int uSize, unsigned char *pbyBuf, unsigned int *puKey)
{
//#ifndef WIN32
//#pragma pack(push) //�������״̬
//#pragma pack(8)    //�趨Ϊ4�ֽڶ��� //64λ 8���ֽ� ָ��
//#endif
    unsigned int *puBuf = (unsigned int *)pbyBuf;
    unsigned int uKey = *puKey;
    unsigned int uRemainSize = uSize%4;//ʣ�¶��ٳ��� 4�ֽڶ���
	uSize /= 4;//�ж��ٸ�4�ı���
    while (uSize-- > 0)
	{
        *puBuf++^=uKey;//��ֵ ������� ����
	}
    pbyBuf = (unsigned char *)puBuf; //��ֵ

    while (uRemainSize-- > 0)
    {//ʣ���
        *pbyBuf++^=(unsigned char)(uKey&0xff); //255
        uKey >>= 8; //256
    }
    *puKey = (*puKey * 31) + 134775813L;//
//#ifndef WIN32
//#pragma pack(pop)
//#endif
return true;
}


int KSG_DecodeEncode_ASM(unsigned int uSize, unsigned char *pbyBuf, unsigned *puKey)
{
    /*__asm mov eax, [uSize]
    __asm mov esi, [puKey]
    __asm mov edi, [pbyBuf]
    __asm mov ecx, eax
    __asm mov edx, [esi]        // key

    __asm shr eax, 2
    __asm jz  Next1

    Loop1:
    __asm mov ebx, [edi]
    __asm add edi, 4
    __asm xor ebx, edx
    __asm dec eax
    __asm mov [edi - 4], ebx
    __asm jnz Loop1


    Next1:
    __asm and ecx, 3
    __asm mov eax, edx  // Key
    __asm jz Next2

    Loop2:
    __asm mov bl, [edi]
    __asm inc edi
    __asm xor bl, al
    __asm shr eax, 8
    __asm mov [edi - 1], bl
    __asm dec ecx
    __asm jnz Loop2
    Next2:

    __asm mov eax, edx          // NewKey = OldKey * 31 +  134775813L
    __asm shl edx, 5
    __asm sub edx, eax
    __asm add edx, 134775813    // 08088405H
    __asm mov [esi], edx*/

    return true;
}

int KSG_DecodeEncode_New(unsigned int uSize, unsigned char *pbyBuf, unsigned int *puKey)
{
	unsigned char *v3; // edi@1
	unsigned int v4; // edx@1
	unsigned int i; // eax@1
	int v6; // ebx@2
	unsigned int v7; // ecx@3
	unsigned int j; // eax@3
	char v9; // bl@4
	char v10; // bl@4

	v3 = pbyBuf;
	v4 = *puKey;
	for (i = uSize >> 2; i; *(v3 - 1) = v4 ^ v6 )
	{
		v6 = *v3;
		v3 += 4;
		--i;
	}
	v7 = uSize & 3;
	for (j = v4; v7; --v7)
	{
		v9 = *v3++;
		v10 = j ^ v9;
		j >>= 8;
		*(v3 - 1) = v10;
	}
	*puKey = 31 * v4 + 134775813;
	return 1;
}
