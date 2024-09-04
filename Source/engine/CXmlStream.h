#ifndef __CXMLSTREAM_H__
#define __CXMLSTREAM_H__

#include "cocos2d.h"

USING_NS_AX;

class CXmlStream
{
private:
    ax::ValueMap m_dictionary;

public:
	CXmlStream(const char* dicFilename);
	~CXmlStream();

	const  char* GetStringByKey(const char* keyword);
	static const char* GetStringByKeyFromFile(const char* dicFilename, const char* keyword);  //
};

#endif
