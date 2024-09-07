#include "KDownLoadFile.h"

//#include "..\deprecated\CCDeprecated.h"
#include <string>
#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

//using namespace ax;

downloadFile* downloadFile::m_inst = NULL;

downloadFile* downloadFile::GetInst(std::string ndownloadUrl,std::string WritablePath)
{
	if (!m_inst)
	{
		m_inst = new downloadFile();
		if (m_inst)
		{
			//m_inst->_curl      = NULL;
			m_inst->curpercent = 0;
			m_inst->setdownloadUrl(ndownloadUrl);
			m_inst->m_WritablePath= WritablePath;
			m_inst->m_DownState =STATE_OK_NET;
		}
		return m_inst;
	}
	return m_inst;
}

void downloadFile::Replease()
{

	if (m_inst)
	{
		/*if (m_inst->_curl)
		{
			curl_easy_cleanup(m_inst->_curl);
			curl_global_cleanup();
		}*/

		delete m_inst;
		m_inst = NULL;
	}
}
//std::string fullPath,std::filename,
bool downloadFile::DownLoadFile(std::string nfullPath,std::string filename,bool reload)
{
    std::string fullpath = nfullPath + filename;
    FILE* fp = createFile(fullpath.c_str(), reload);
    if (fp == nullptr) {
        m_DownState = STATE_NO_FILE;
        return false;
    }

    int localFileLength = 0;
    if (!reload) {
        localFileLength = getLocalFileLenth(filename.c_str());
    }

    int timeout = 20;
    std::string packageUrl = m_downloadUrl + filename;
    double fileSize = getDownloadFileLenth(packageUrl.c_str());

    if (fileSize <= 0) {
        fclose(fp);
        m_DownState = STATE_NO_URLFILE;
        return false;
    }

    if (!reload && localFileLength > 0 && localFileLength >= fileSize) {
        fclose(fp);
        m_DownState = STATE_WANCHENG_FILE;
        return true;
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        fclose(fp);
        curl_global_cleanup();
        m_DownState = STATE_NO_NET;
        return false;
    }

    m_DownState = STATE_OK_NET;
    curl_easy_setopt(curl, CURLOPT_URL, packageUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_RESUME_FROM, localFileLength);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressFunc);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    fclose(fp);

    if (res != CURLE_OK) {
        m_DownState = STATE_NO_NET; // Đặt trạng thái lỗi phù hợp
        return false;
    } else {
        m_DownState = STATE_NEXT_FILE;
        return true;
    }
}

FILE* downloadFile::createFile(const char *file,bool reload)
{
	if (!file)
		return NULL;

	/*char buf[260];
	const char *pStart = file + 1;
	const char *pEnd = NULL;

	do
	{
		pEnd = strchr(pStart, '/');
		if (pEnd)
		{
			memcpy(buf, file, pEnd - file);
			buf[pEnd - file] = 0;
			pStart = ++pEnd;

			if (chdir(buf) == -1)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				//if (mkdir(buf))
				if ((GetFileAttributesA(buf)) == INVALID_FILE_ATTRIBUTES)
				{
					if  (!CreateDirectoryA(buf, 0))
						return NULL;
				}
				else
					return NULL;
#else
				//mkdir(m_downloadDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
				if (mkdir(buf, S_IRWXU))
					return NULL;
#endif

			}

		}
		else*/
		//{//wb+
			FILE *fp = NULL;
			if (reload)
			{//�������д
				if ((fp = fopen(file,"wb+")) == NULL)
					return NULL;
			}
			else
			{//δ�����д
				if ((fp = fopen(file,"ab+")) == NULL)
					return NULL;
			}

			return fp;
		//}
	//} while (1);

	//return NULL;
}

int downloadFile::getLocalFileLenth(const char* filename)
{
	std::string fullPath = m_WritablePath+filename;//ax::FileUtils::getInstance()->getWritablePath() + filename;

	FILE *fp = fopen(fullPath.c_str(),"r");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		int length = ftell(fp);
		fclose(fp);
		fp =NULL;
		return length;
	}
	return 0;
}
int downloadFile::progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	//��ǰ�����ļ��������СtotalToDownload
	//��ǰ���صĴ�СnowDownloaded
	//��ǰ���ص��ļ�����
	if (m_inst)
	    m_inst->curpercent = nowDownloaded/totalToDownload * 100;
	return 0;
}

size_t downloadFile::downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE* fp = static_cast<FILE*>(userdata);

    char * xxx = static_cast<char*>(ptr);
	size_t written = fwrite(ptr, size, nmemb, fp);
	return written;
}

double downloadFile::getDownloadFileLenth(const char *url){

	double len = 0.0;
	CURL *handle = curl_easy_init();
	if (!handle)
	{
		curl_easy_cleanup(handle);
		curl_global_cleanup();
		return 0;
	}

	curl_easy_setopt(handle, CURLOPT_URL, url);
//���ó�ʱ CURLOPT_TIMEOUT_MS

	curl_easy_setopt(handle, CURLOPT_TIMEOUT,5);

	curl_easy_setopt(handle, CURLOPT_HEADER, 1);    //ֻҪ��headerͷ

	curl_easy_setopt(handle, CURLOPT_NOBODY, 1);    //������body

	curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, save_header);

	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writefunc);

	if (curl_easy_perform(handle) == CURLE_OK) {

		if (CURLE_OK == curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &len)){


		}
		else {

			//curl_easy_getinfo failed!
			len = -1;
		}

	}
	else {

		len = -1;

	}
	curl_easy_cleanup(handle);
	curl_global_cleanup();
	return len;

}
size_t downloadFile::save_header(void *ptr, size_t size, size_t nmemb, void *data)
{
	return (size_t)(size * nmemb);
}

size_t downloadFile::writefunc(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	return (size_t)(size * nmemb);
}
