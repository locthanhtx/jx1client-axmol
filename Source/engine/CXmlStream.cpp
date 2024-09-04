
#include "CXmlStream.h"

CXmlStream::CXmlStream(const char* dicFilename)
{
	this->m_dictionary = ax::FileUtils::getInstance()->getValueMapFromFile(dicFilename);
}
 
CXmlStream::~CXmlStream()
{
 
}
 
const char* CXmlStream::GetStringByKey(const char* keyword)
{
    if (!keyword || m_dictionary.size() <= 0)
    {
        return nullptr;
    }

    std::string strKeyword(keyword);
    auto iter = m_dictionary.find(strKeyword);

    if (iter != m_dictionary.end())
    {
        std::string value = iter->second.asString();
        return value.c_str();
    }

    return nullptr;
}
 
const char* CXmlStream::GetStringByKeyFromFile(const char* dicFilename, const char* keyword)
{
	if (!dicFilename) return NULL;

    ax::ValueMap dictionary = ax::FileUtils::getInstance()->getValueMapFromFile(dicFilename);

	if  (dictionary.size() <= 0) return NULL;

    std::string strKeyword(keyword);
    auto iter = dictionary.find(strKeyword);

    if (iter != dictionary.end())
    {
        std::string value = iter->second.asString();
        return value.c_str();
    }

    return nullptr;
}

/*
// Insert objects to dictionary
*  CCString* pValue1 = CCString::create("100");
*  CCString* pValue2 = CCString::create("120");
*  CCInteger* pValue3 = CCInteger::create(200);
*  pDict->setObject(pValue1, "key1");
*  pDict->setObject(pValue2, "key2");
*  pDict->setObject(pValue3, "key3");

*/