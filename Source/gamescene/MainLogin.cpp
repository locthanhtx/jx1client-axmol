//
// Created by Kull on 10/4/2024.
//

#include "MainLogin.h"
#include "MainMenu.h"
#include "gamecore/KCore.h"
#include "engine/KIniFile.h"
#include "gamecore/CoreShell.h"
#include "gameui/KSelPlayer.h"

bool MainLogin::init()
{
    if (!Scene::init())
    {
        return false;
    }

    m_Status = LL_S_WAIT_INPUT_ACCOUNT;

    _groot = GRoot::create(this);
    _groot->retain();

    UIPackage::addPackage("FGUI/Login");
//    UIConfig::registerFont(UIConfig::defaultFont, "fonts/UVNButLong2.TTF");
    _view = UIPackage::createObject("Login", "Main")->as<GComponent>();
    _groot->addChild(_view);

    _view->getChild("bt_xacnhan")->addClickListener([this](EventContext*) {
        this->mianLoginCallback(this);
    });

    _view->getChild("bt_huy")->addClickListener([this](EventContext*) {
        this->mianCancelCallback(this);
    });

    char nTempStr[64];
    ZeroMemory(nTempStr,sizeof(nTempStr));
    KIniFile accinfo;
    accinfo.Load("accinfo.ini",true);
    accinfo.GetString("info","acc","",nTempStr,sizeof(nTempStr));

    GTextInput* username = _view->getChild("username")->as<GTextInput>();
    username->setText(nTempStr);

    accinfo.GetString("info","pas","",nTempStr,sizeof(nTempStr));
    GTextInput* password = _view->getChild("password")->as<GTextInput>();
    password->setText(nTempStr);

    accinfo.Clear();

    this->scheduleUpdate();
    return true;
}

MainLogin::MainLogin() : _groot(nullptr)
{
}

MainLogin::~MainLogin()
{
    AX_SAFE_RELEASE(_groot);
}

void MainLogin::mianCancelCallback(Object* pSender)
{
    m_ClientChoices.nServerRegionIndex = -1;
    if (nAcclient)
    {
        m_bIsClientConnecting = false;
        m_bIsPlayerInGame = false;
        nAcclient->Cleanup();

    }
    Director::getInstance()->replaceScene(MainMenu::create());
}

void MainLogin::mianLoginCallback(Object* pSender)
{
    GTextInput* username = _view->getChild("username")->as<GTextInput>();
    GTextInput* password = _view->getChild("password")->as<GTextInput>();
    std::string strAcc;
    std::string strPas;

    if (username && password)
    {
        strAcc = username->getText();
        strPas = password->getText();
        if (strAcc.length()>16 || strAcc.length()<=0 || strPas.length()>16 || strPas.length()<=0)
        {
            messageBox("Sai tài khoản hoặc mật khẩu!","Cảnh báo");
            return;
        }

        std::string strcheck="check";
        strcheck+=strAcc;
        strcheck+=strPas;
        int count = 0,spaceNum=0,chineseNum=0,other=0,letter=0,xiaoxie=0;
        for(int i = 0; i <strcheck.length();i++)
        {
            if(isdigit(strcheck[i])) //判断是否是数字
                count++;
            else if(isspace(strcheck[i]))//判断是否是空格
                ++spaceNum;
            else if(isalpha(strcheck[i])==1)//判断是否是字母	 1 为大写 2 为 小写  0 为不是字母
            {
                ++letter;
            }
            else if(isalpha(strcheck[i])==2)//判断是否是字母	 1 为大写 2 为 小写  0 为不是字母
            {
                ++xiaoxie;
            }
            else if(strcheck[i]&0x80)//if(strcheck[0]>=0xB0)
                ++chineseNum;    //判断是否是中文
            else
            {//开放合区注册 不检测特殊符号
                ++other;
            }
        }

        if (spaceNum>0 || chineseNum >0 || other>0)
        { 	//system("pause");
            messageBox("Invalid character,please check your name!\n Cannot contain special symbols such as chinese, spaces, etc.!","Warning");
            return;
        }

        KIniFile accinfo;
        if (accinfo.Load("accinfo.ini",true))
        {
            char nTempStr[64];
            t_sprintf(nTempStr,strAcc.c_str());
            accinfo.WriteString("info","acc",nTempStr);

            t_sprintf(nTempStr,strPas.c_str());
            accinfo.WriteString("info","pas",nTempStr);
            accinfo.Save("accinfo.ini");
            accinfo.Clear();
        }

        if (m_bIsClientConnecting && nAcclient)
        {
            if  (m_ClientChoices.nServerRegionIndex <= 0)
            {
                messageBox("Vui lòng chọn máy chủ đăng nhập", "Cảnh báo");
                return;
            }
            //模拟帐号登陆
            m_nNumRole = 0;                          //角色数量设置为0
            memset(&m_RoleList,0,sizeof(m_RoleList));//清空角色清单数据
            ClearAccountPassword(true, true);        //清空备份的帐号密码

            unsigned char Buff[sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE];
            RandMemSet(sizeof(Buff),(unsigned char*)Buff);		// random memory for make a cipher
            char pszAccount[32]={0};
            t_sprintf(pszAccount,strAcc.c_str());

            (*(PROTOCOL_MSG_TYPE*)Buff) = c2s_login_fhgyrinhkmvnsheypo;
            KLoginAccountInfo* pInfo = (KLoginAccountInfo*)&Buff[PROTOCOL_MSG_SIZE];

            pInfo->Size  = sizeof(KLoginAccountInfo);
            pInfo->Param = LOGIN_A_LOGIN | LOGIN_R_REQUEST;
            strncpy(pInfo->Account,pszAccount,sizeof(pInfo->Account));

            pInfo->Account[sizeof(pInfo->Account) - 1] = '\0';
            //KSG_PASSWORD pcPassword;
            KSG_StringToMD5String(pInfo->Password.szPassword,strPas.c_str());
            //pInfo->Password      = pcPassword;
            pInfo->nSelSererIdx    = m_ClientChoices.nServerRegionIndex;//KUiSelServer::GetCurServer();
            pInfo->ProtocolVersion = KPROTOCOL_VERSION;       //  传输协议版本，以便校验是否兼容
            m_Status               = LL_S_ACCOUNT_CONFIRMING; //  等待验证帐号密码
            SetAccountPassword(pszAccount, &pInfo->Password);
            if (!nAcclient->SendMsg(Buff, sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE))
            {
                if (nAcclient)
                {
                    nAcclient->Cleanup();
                    m_bIsClientConnecting = false;
                }
                Director::getInstance()->replaceScene(MainMenu::create());
            }
            //Director::getInstance()->replaceScene(KSelPlayer::scene());
            //messageBox("send suss","send suss");
        }
    }

}

void MainLogin::ClearAccountPassword(bool bAccount, bool bPassword)
{
    if (bAccount)
        memset(m_ClientChoices.Account, 0xff, sizeof(m_ClientChoices.Account));
    if (bPassword)
        memset(&m_ClientChoices.Password, 0xff, sizeof(m_ClientChoices.Password));
}

void MainLogin::SetAccountPassword(const char* pszAccount, const KSG_PASSWORD* pcPassword)
{
    int i;
    if (pszAccount)
    {
        strncpy(m_ClientChoices.Account, pszAccount, sizeof(m_ClientChoices.Account));
        for (i = 0; i < 32; ++i)
            m_ClientChoices.Account[i] = ~m_ClientChoices.Account[i];
    }

    if (pcPassword)
    {
        m_ClientChoices.Password = *pcPassword;
        for (i = 0; i < KSG_PASSWORD_MAX_SIZE; ++i)
            m_ClientChoices.Password.szPassword[i] = ~m_ClientChoices.Password.szPassword[i];
    }
}

void MainLogin::GetAccountPassword(char* pszAccount, KSG_PASSWORD* pPassword)
{
    int i;
    if (pszAccount)
    {
        memcpy(pszAccount, m_ClientChoices.Account, sizeof(m_ClientChoices.Account));
        for (i = 0; i < 32; ++i)
            pszAccount[i] = ~pszAccount[i];
    }
    if (pPassword)
    {
        *pPassword = m_ClientChoices.Password;
        for (i = 0; i < KSG_PASSWORD_MAX_SIZE; ++i)
            pPassword->szPassword[i] = ~pPassword->szPassword[i];
    }
}

void MainLogin::AcceptNetMsg(void* pMsgData) //循环接受处理消息
{
    if (pMsgData == NULL)  //数据包为空 则返回
        return;
    //messageBox("回调成功","GetPackFromServer");
    switch(m_Status)       //发送数据包标识变量
    {
        case LL_S_ACCOUNT_CONFIRMING:  //账号验证
            ProcessAccountLoginResponse((KLoginStructHead*) (((char*)pMsgData) + PROTOCOL_MSG_SIZE));
            break;
        case LL_S_WAIT_ROLE_LIST:      //等待角色清单
            ProcessRoleListResponse((TProcessData*)pMsgData);   //接收协议角色信息
            break;
        case LL_S_CREATING_ROLE:       //创建角色
            //ProcessCreateRoleResponse((tagNewDelRoleResponse*)pMsgData); //创建角色
            break;
        case LL_S_DELETING_ROLE:       //删除角色
            //ProcessDeleteRoleResponse((tagNewDelRoleResponse*)pMsgData);
            break;
        case LL_S_WAIT_TO_DEL:         //已经取消
            //ProcessGetRoleResponse((tagNotifyPlayerLogin*)pMsgData);
            break;
        case LL_S_WAIT_TO_LOGIN_GAMESERVER:  //等待链接到服务器
            //ProcessToLoginGameServResponse((tagNotifyPlayerLogin*)pMsgData); //调用链接到服务器的函数
            break;
    }
}

void MainLogin::ProcessAccountLoginResponse(KLoginStructHead* pResponse)
{
    if (((pResponse->Param & LOGIN_ACTION_FILTER) == LOGIN_A_LOGIN) &&	//操作性为要匹配
        pResponse->Size >= sizeof(KLoginAccountInfo))				//数据内容的大小也要匹配
    {
        KLoginAccountInfo* pInfo = (KLoginAccountInfo*)pResponse;
        char	szAccount[32]={0};
        KSG_PASSWORD Password;
        GetAccountPassword(szAccount, &Password);  //获取数据库的账号和密码
        if (strcmp(pInfo->Account,  szAccount)  == 0 &&
            strcmp(pInfo->Password.szPassword, Password.szPassword) == 0)
        {//返回的结果
            KIniFile nBVer;
            double mBver,mBishopver;
            if (!nBVer.Load("\\Settings\\Ver\\Xfsverjx.ini"))//补丁包里面的版本号 GetDouble
            {
                isthisVer = false;
                m_Result = LL_R_INVALID_PROTOCOLVERSION;  //版本问题
                //ReturnToIdle();//断开网络
                nBVer.Clear();
                messageBox("Phiên bản sai","Nhắc:");
                return;
            }
            nBVer.GetDouble("CilentVer","ver",0.1,&mBver);  //补丁包版本号 GetDouble
//            CcharToDouble(pInfo->nBishopver, mBishopver)	;  //bishiop发过来的版本
            sscanf(pInfo->nBishopver,"%lf",&mBishopver);
            if  (pInfo->nSelSererIdx<=0)
            {//所有服务器客户端都检测
                if (mBishopver!=mBver)
                    isthisVer = false;
                else
                    isthisVer = true;
            }
            else
            {
                if (m_ClientChoices.nServerRegionIndex==pInfo->nSelSererIdx)
                {//等于这个服务器才检测
                    if (mBishopver!=mBver)
                        isthisVer = false;
                    else
                        isthisVer = true;
                }
                else //否则就不检测 直接通过
                    isthisVer = true;
            }
            //------------多开检测--------------
            if (pInfo->nForbitopennum)
            {
            }
            nBVer.Clear();
            //预建立角色检测
            if (pInfo->ProtocolVersion!=KPROTOCOL_VERSION)
            {
                messageBox("Phiên bản không tương thích!","Nhắc:");
                m_Result = LL_R_INVALID_PROTOCOLVERSION;  //版本问题
                return;
            }
            //----------------------------------
            int nResult = ((pResponse->Param) & ~LOGIN_ACTION_FILTER);
            if (nResult == LOGIN_R_SUCCESS)
            {
                m_Status = LL_S_WAIT_ROLE_LIST;			  //正在获取角色信息
            }
            else
            {
                LOGIN_LOGIC_RESULT_INFO eResult = LL_R_NOTHING;
                switch(nResult)
                {
                    case LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR:  //密码错误
                        //eResult = LL_R_ACCOUNT_PWD_ERROR;
                        m_Status = LL_S_WAIT_INPUT_ACCOUNT;
                        messageBox("Sai mật khẩu","Nhắc:");
                        break;
                    case LOGIN_R_ACCOUNT_EXIST:   //账号给锁定  --账号正在使用
                        //eResult = LL_R_ACCOUNT_LOCKED;
                        messageBox("Tài khoản đang được sử dụng","Nhắc:");
                        m_Status = LL_S_WAIT_INPUT_ACCOUNT;
                        break;
                    case LOGIN_R_FREEZE:  // 账号被冻结
                        //eResult = LL_R_ACCOUNT_FREEZE;
                        messageBox("Tài khoản bị đóng băng","Nhắc:");
                        m_Status = LL_S_WAIT_INPUT_ACCOUNT;
                        break;
                    case LOGIN_R_INVALID_PROTOCOLVERSION: // 版本不兼容
                        //eResult = LL_R_INVALID_PROTOCOLVERSION
                        messageBox("Phiên bản không tương thích", "Nhắc:");
                        break;
                    case LOGIN_R_FAILED:  //链接失败
                        //eResult = LL_R_CONNECT_SERV_BUSY; // 提示服务器忙
                        messageBox("Kết nối không thành công","Nhắc:");
                        break;
                    case LOGIN_R_TIMEOUT: // 账号使用时间到
                        //eResult = LL_R_ACCOUNT_NOT_ENOUGH_POINT;
                        messageBox("Tài khoản đã hết hạn","Nhắc:");
                        m_Status = LL_S_WAIT_INPUT_ACCOUNT;
                        break;
                    default:
                        messageBox("Kết nối không thành công","Nhắc:");
                        break;
                }

                if (m_Status != LL_S_WAIT_INPUT_ACCOUNT)
                {//断开连接
                    //ReturnToIdle();
                }
                m_Result = eResult;
            }
        }
        memset(szAccount, 0, sizeof(szAccount));
        memset(&Password, 0, sizeof(Password));
    }
}


//--------------------------------------------------------------------------
//	功能：角色列表返回
//	状态切换：成功 LL_S_WAIT_ROLE_LIST -> LL_S_ROLE_LIST_READY
//			  失败 状态保持
//--------------------------------------------------------------------------
void MainLogin::ProcessRoleListResponse(TProcessData* pResponse)
{
    if (pResponse->nProtoId == s2c_roleserver_getrolelist_result)
    {
        m_nNumRole = pResponse->pDataBuffer[0];
        if (m_nNumRole > MAX_PLAYER_PER_ACCOUNT)
            m_nNumRole = MAX_PLAYER_PER_ACCOUNT;
        else if (m_nNumRole < 0)
            m_nNumRole = 0;

        RoleBaseInfo* pList = (RoleBaseInfo*)&pResponse->pDataBuffer[1];
        for (int i = 0; i < m_nNumRole; ++i)
        {
            if (pList->szName[0])
            {
                strcpy(m_RoleList[i].Name, pList->szName);
                m_RoleList[i].Attribute = pList->Series;
                m_RoleList[i].Gender = pList->Sex;
                m_RoleList[i].nLevel = pList->Level;
                pList ++;
            }
            else
            {
                m_nNumRole = i;
                break;
            }
        }
        m_Status = LL_S_ROLE_LIST_READY;
    }
}

void MainLogin::update(float delta)
{
    if  (m_Status == LL_S_ROLE_LIST_READY)
    {//如果已经获取到角色数据了 就进入下一个场景
        m_Status = LL_S_IDLE;
        Director::getInstance()->replaceScene(KSelPlayer::create());
        return;
    }
    unsigned int nSize;
    const char* pBuffer = NULL;
    if (m_bIsClientConnecting)
    { //账号服务器
        while (true)
        {
            if (!nAcclient)
                break;
            //无限循环从服务器获取数据小包
            pBuffer = (const char*)nAcclient->GetPackFromServer(nSize);

            if (!(pBuffer && nSize))
                break;

            PROTOCOL_MSG_TYPE*	pMsg = (PROTOCOL_MSG_TYPE*)pBuffer;
            PROTOCOL_MSG_TYPE	Msg  = *pMsg;  //协议头

            if  (Msg == s2c_login || Msg == s2c_roleserver_getrolelist_result ||Msg == s2c_notifypl_ayerlogin_hfkqotivbhd || Msg == s2c_rolenewdelresponse)
                AcceptNetMsg(pMsg);
        }
    }
    int newX = 0;
    int newY = 0;
    switch(newY)
    {
        case 1:
            break;
        case  2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
    }
}
