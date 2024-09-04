#pragma once

#ifdef WIN32 //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define UTEXT(str,need) gbk2utf8(str,need)
#define G2U(str)   gbk2utf8(str,0)
#define U2G(str)   utf2gbk(str)
#include "platform/third_party/win32/iconv/iconv.h"
#pragma comment(lib,"libiconv.lib")
#else //if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define UTEXT(str, need) gbk2utf8(str,need)
#define G2U(str)   gbk2utf8(str,1)
#define U2G(str)   utf2gbk(str)
#include "./libiconv/include/iconv.h"
//#include "D:\android-ndk-r9d\sources\android\support\include\iconv.h"
#endif
#include <string>
using namespace std;

static std::string gbk2utf8(const char *inbuf,int32_t need=0);

static int32_t code_convert(const char *from_charset, const char *to_charset,const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	const char *temp = inbuf;
	//char **pin = &temp;
	const char **pin = &temp;

	char **pout = &outbuf;
	memset(outbuf, 0, outlen);
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	if (iconv(cd, pin,&inlen,pout,&outlen) == -1) return -1;
	iconv_close(cd);
	return 0;
}

/*UTF8 to GB2312*/
static std::string utf2gbk(const char *inbuf)
{
	std::string strRet = "";

    size_t inlen = strlen(inbuf);  //size_t
	char * outbuf = new char[inlen * 2 + 2];
	//char outbuf[255]={0};gb2312
	int32_t ret=-1;
	if (ret = code_convert("utf-8","gb2312",inbuf,inlen,outbuf,inlen * 2 + 2)==0)
	{
		strRet = outbuf;
	}

	if (ret==-1 ||strRet=="")
		strRet =inbuf;

	delete [] outbuf;
	return strRet;
}

/*GB2312 to UTF8*/
static std::string gbk2utf8(const char *inbuf,int32_t need)
{
	std::string strRet="";

	if (!need)
	{
		strRet = inbuf;
		return strRet;
	}
	//gb2312
    size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	int32_t ret=-1;
	if (ret = code_convert("gb2312", "utf-8", inbuf, inlen,outbuf,inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}

	if (ret==-1 || strRet=="")
		strRet =inbuf;
	delete [] outbuf;
	return strRet;
}

