
#ifndef _dataChecksum_H
#define	_dataChecksum_H

#include "cocos2d.h"
USING_NS_AX;
#include "KbugInfo.h"

#include "string.h"

//public use
#define PG_MAXBUFFER			64
#define PG_MAXPASSWORDLEN		20
#define PG_RESULTLENSTD			32

//private use
#define PG_MINPKEYLEN			(PG_RESULTLENSTD - PG_MAXPASSWORDLEN - 2)
#define PG_MAXPKEYLEN			(PG_RESULTLENSTD - 2)
#define PG_PKEYMASK				151
#define PG_PKEYOFF				7
#define PG_ENOFF				5
#define PG_MINCHAR				0x20
#define PG_MAXCHAR				0x7E
#define PG_CHARCOUNT			(PG_MAXCHAR - PG_MINCHAR + 1)

#define PG_VALIDCHAR(c)			((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c >= '0' && c <= '9'))
#define PG_INVALIDCHAR(c)		!PG_VALIDCHAR(c)

class  dataChecksum
{
public:
	//interface functions for the RSA MD5 calculation
	 int32_t SimplyDecrypt(char* szPass, const char* szEncrypted);   //����
	 //int32_t SimplyEncrypt(char* szResult, const char* szPass);    //����
protected:

private:
     int32_t  pgChar2Int(char b);
	 char __rol(char c, uint32_t count);
	 int32_t  pgStrLen(const char* sz);
	 void pgSwapChars(char* sz);
	 void pgDecrypt(char* szKey, int32_t nKeyLen, char* szBuffer, const char* szEnc, int32_t nStrLen);

//	 int32_t  SimplyDecrypt(char* szPass, const char* szEncrypted);
	 //����
	 //char pgInt2Char(char i, DWORD dwTickCount = 0);
	 //int32_t  pgSameString(const char* s1, const char* s2);
	 //int32_t  pgEncrypt(char* szKey, int32_t nKeyLen, char* szBuffer, const char* szPass, int32_t nStrLen);
	 //int32_t  pgEncrypt_(char* szResult, const char* szPass, DWORD dwTickCount, int32_t& nLevel);
//	 int32_t  SimplyEncryptPassword(char* szResult, const char* szPass);
};
#endif // !defined(AFX_dataChecksum_H__2BC7928E_4C15_11D3_B2EE_A4A60E20D2C3__INCLUDED_)
