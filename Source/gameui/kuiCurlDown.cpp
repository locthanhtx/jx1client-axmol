//
//  kuiCurlDown.cpp
//  muli_download
//
//  Created by vin on 14-3-24.
//
//
#include "KuiCurlDown.h"
#ifdef WIN32
#    include <pthreadwin32.h>
#else
#    include <pthread.h>
#endif
#ifdef _WIN32
#    include <io.h>
#    define access _access
#else
#    include <unistd.h>
#endif
static pthread_mutex_t g_downloadMutex_1;

kuiCurlDown::~kuiCurlDown(){
    mFileLenth = 0;
}

kuiCurlDown::kuiCurlDown():isStop(false),mDownloadUrl(""),timeout(2){ // test timeout 2 seconds. if release timeout 20 seconds
    mFileLenth = 0;
    mFilePath = "";
    pthread_mutex_init(&g_downloadMutex_1, NULL);
}

kuiCurlDown::kuiCurlDown(string downUrl,string filePath):mFileLenth(0),isStop(false),mDownloadUrl(downUrl),timeout(2),mFilePath(filePath){  // test timeout 2 seconds. if release timeout 20 seconds
    mDownloadUrl = downUrl;
    pthread_mutex_init(&g_downloadMutex_1, NULL);
}

void kuiCurlDown::setDelegate(CurlDownDelegate * delegate) {
    mDelegate = delegate;
}

//#pragma mark 进行文件写入本地回调函数
static unsigned int my_write_func(void *ptr, unsigned int size, unsigned int nmemb, void *userdata) {
    FILE *fp = (FILE*)userdata;
    unsigned int written = fwrite(ptr, size, nmemb, fp);
    return written;
}
//#pragma mark 下载进度函数 - 每次下载大小 不是总的大小
static int my_progress_func(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded) {
    //    double curpercent = nowDownloaded / totalToDownload *100;//2001619
    //    if (totalToDownload == 0) {
    //        return 0;
    //    }
    //    CCLOG("nowDd:%d; totalDown:%d; downProgress:%.2f%%",(int)nowDownloaded,(int)totalToDownload,curpercent);
    // 下载总值 上面注释的如果进入续传就不准确了
    kuiCurlDown* curlDown = (kuiCurlDown*)ptr;
    if(!curlDown || curlDown->mFileLenth == 0 || nowDownloaded == 0) return 0;
    double nowDown = (curlDown->mFileLenth - totalToDownload + nowDownloaded);
    double curpercent = nowDown / curlDown->mFileLenth * 100; // 2001619

    curlDown->mDelegate->onProgress(curpercent,ptr, curlDown->mFilePath);
    //    CCLOG("nowDd:%d; totalDown:%d; downProgress:%.2f%%",(int)(curlDown->mFileLenth - totalToDownload + nowDownloaded),(int)curlDown->mFileLenth , curpercent);
    return 0;
}

//#pragma mark 进行下载
bool kuiCurlDown::download() 
{
	FILE *fp = NULL;
	if(access(mFilePath.c_str(), 0)==0) { // 以二进制形式追加
		fp = fopen(mFilePath.c_str(), "ab+");
	} else { // 二进制写
		fp = fopen(mFilePath.c_str(), "wb");
	}

	if (fp == NULL) {// 如果文件初始化失败进行返回
		return false;
	}

	// 读取本地文件下载大小
	long localFileLenth = getLocalFileLength(); //已经下载的大小
	CCLOG("filePath:%s；leng:%ld",mFilePath.c_str() , localFileLenth ); //4397779 //3377875

	CURL *handle = curl_easy_init();
	std::string packageUrl = mDownloadUrl; //下载地址+下载文件名
	curl_easy_setopt(handle, CURLOPT_URL, packageUrl.c_str()); // http://curl.haxx.se/libcurl/c/fopen.html
	curl_easy_setopt(handle, CURLOPT_TIMEOUT, timeout);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, my_write_func);   //写文件回调方法
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp); // 写入文件对象
	curl_easy_setopt(handle, CURLOPT_RESUME_FROM, localFileLenth);  // 从本地大小位置进行请求数据
	//    curl_easy_setopt(handle, CURLOPT_RESUME_FROM_LARGE, localFileLenth); // 坑
	curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(handle, CURLOPT_PROGRESSFUNCTION, my_progress_func ); //下载进度回调方法
	curl_easy_setopt(handle, CURLOPT_PROGRESSDATA, this); // 传入本类对象

	CURLcode res = curl_easy_perform(handle);
	fclose(fp);
	return res == CURLE_OK;
}


//#pragma mark- 控制方法
void kuiCurlDown::downloadControler() {
    CCLOG("--1-");
    mFileLenth = getDownloadFileLenth(); // 获取远程文件大小
    if (mFileLenth <= 0) {
        cout << "download file fail..." << endl;
        mDelegate->onError(kNetwork);
        return;
    }
    vector<string> searchPaths = ax::FileUtils::getInstance()->getSearchPaths();
    vector<string>::iterator iter = searchPaths.begin();
    searchPaths.insert(iter, mFilePath);
    ax::FileUtils::getInstance()->setSearchPaths(searchPaths);
    
    CCLOG("--2-mFileLenth:%f",mFileLenth);
	mFileName = mDownloadUrl.substr(mDownloadUrl.rfind('/') + 1);
    CCLOG("--3-");
    CCLOG("mFileName:%s;",mFileName.c_str());
//    mFilePath = ax::FileUtils::getInstance()->getWritablePath();
//    CCLOG("--5-");
    mFilePath = mFilePath + mFileName;
    CCLOG("mFilePath:%s",mFilePath.c_str());
    CCLOG("--6-");
    bool ret = false;
    while (true){ // 循环下载 每30秒进行下载 避免断网情况
        ret = download(); //直接下载 进行堵塞线程
        CCLOG("----stop---%s------",isStop?"true":"false");
        if (isStop) { // 如果进行停止 break
            CCLOG("----stop---------");
            break;
        }
        if (ret ){ //下载完成
            break;
        }
#ifndef WIN32
        sleep(0.5); //每次下载中间间隔0.5秒
#else
		Sleep(500);
#endif
    }
    
    if (ret) {
        CCLOG("download ok");
        mDelegate->onSuccess(mFilePath);
    } else {
        CCLOG("download fail");
        mDelegate->onError(kUncompress);
    }
}

void kuiCurlDown::setStopDown(){
    pthread_mutex_lock(&g_downloadMutex_1);
    isStop = true;
    pthread_mutex_unlock(&g_downloadMutex_1);
    CCLOG("----stop-lll--%s------",isStop?"true":"false");
}

/*
//fopen函数调用如下：
//fopen（“文件名”，“使用文件方式”）；
//r：只读；
//w：只写；
//r+：允许读写；
//w+：允许读写；
//a：向文本文件末添加数据；
//a+：向文本文件末添加数据，允许读；
//rb：只读二进制文件；
//wb：只写二进制文件；
//rb+：只读二进制文件，允许写；
//wb+：只写二进制文件，允许读；
//ab：向二进制文件末添加数据；
//ab+：向二进制文件末添加数据，允许读；
*/
long kuiCurlDown::getLocalFileLength() 
{
    FILE *fp = fopen(mFilePath.c_str(), "r");
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    fclose(fp);
    return length;
}

/* 得到远程文件的大小, 要下载的文件大小 */ // 参考的那个不对 获取不到大小
long kuiCurlDown::getDownloadFileLenth(){
    //	double filesize = 0.0;
	CURL *handle = curl_easy_init();
    //    http://curl.haxx.se/libcurl/c/ftpgetinfo.html
    //    curl_easy_setopt(curl, CURLOPT_URL, ftpurl);
    //    /* No download if the file */
    //    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    //    /* Ask for filetime */
    //    curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
    //    /* No header output: TODO 14.1 http-style HEAD output for ftp */
    //    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
    //    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
    //    /* Switch on full protocol/debug output */
    //    /* curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); */
    
	curl_easy_setopt(handle, CURLOPT_URL, mDownloadUrl.c_str());
    /* No download if the file */
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1L);
    /* Ask for filetime */
    curl_easy_setopt(handle, CURLOPT_HEADER, 0L); // 0 不打印日志 1打印日志
    
	if (curl_easy_perform(handle) == CURLE_OK) {
		curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &mFileLenth);
        printf("filesize : %0.0f bytes\n", mFileLenth);
    } else {
		mFileLenth = -1;
	}
	return mFileLenth;
}
