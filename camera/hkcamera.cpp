#include "hkcamera.h"



hkCamera* hkCamera::singleInstance = nullptr;

hkCamera::hkCamera(QObject *parent) : QObject(parent)
{
    QString current = QDir::current().absolutePath();
    QString libety_parh = current+"/lib/libcrypto.so.1.1";
    QString ssleay_path = current+"/lib/libssl.so.1.1";
    QString sdk_path  = current+"/lib";

//    const char * path = current.toStdString().c_str();

    //---------------------------------------
    // 初始化
    char cryptoPath[2048]={0};
    sprintf(cryptoPath,libety_parh.toStdString().c_str()); ///home/stfz/build/FZ001xface
    NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_LIBEAY_PATH, cryptoPath);

    char sslPath[2048]= {0};
    sprintf(sslPath, ssleay_path.toStdString().c_str());
    NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_SSLEAY_PATH, sslPath);

    NET_DVR_LOCAL_SDK_PATH struComPath={0};
    sprintf(struComPath.sPath,sdk_path.toStdString().c_str());
    NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_SDK_PATH, &struComPath);

    NET_DVR_Init();
    NET_DVR_SetLogToFile(3,"hksdklog");
    DWORD ver = NET_DVR_GetSDKBuildVersion();
    uint8_t mainVer = ver>>24&0x00ff;
    uint8_t subVer = ver>>16&0x00ff;
    qWarning("hk SDK mainVer:%d,subVer:%d",mainVer,subVer);
    //设置连接时间与重连时间
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    //---------------------------------------
}

hkCamera::~hkCamera()
{

    //撤销布防上传通道
    if (!NET_DVR_CloseAlarmChan_V30(lHandle))
    {
        printf("NET_DVR_CloseAlarmChan_V30 error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
    }
    //注销用户
    NET_DVR_Logout(lUserID);
    //释放 SDK 资源
    NET_DVR_Cleanup();
}



hkCamera *hkCamera::getInstance(QObject* obj)
{
    if(hkCamera::singleInstance == nullptr)
    {
        hkCamera::singleInstance = new hkCamera(obj);
        return hkCamera::singleInstance;
    }
    return hkCamera::singleInstance;
}

//接收异常消息的回调函数的外部实现
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256];
    memset(tempbuf,0,256);
    qWarning("ExceptionCallBack dwType:0x%x",dwType);
    switch(dwType)
    {
        case EXCEPTION_AUDIOEXCHANGE: //语音对讲时网络异常
            sprintf(tempbuf,"语音对讲时网络异常!!!");
            qWarning() << tempbuf;
            //TODO: 关闭语音对讲
            break;
        case EXCEPTION_ALARM: //报警上传时网络异常
            sprintf(tempbuf,"报警上传时网络异常!!!");
            qWarning() << tempbuf;
            //TODO: 关闭报警上传
            break;
        case EXCEPTION_PREVIEW: //网络预览时异常
            sprintf(tempbuf,"网络预览时网络异常!!!");
            qWarning() << tempbuf;
            //TODO: 关闭网络预览
            break;
        case EXCEPTION_SERIAL: //透明通道传输时异常
            sprintf(tempbuf,"透明通道传输时网络异常!!!");
            qWarning() << tempbuf;
            //TODO: 关闭透明通道
            break;
        case EXCEPTION_RECONNECT: //预览时重连
            qDebug()<<"reconnect";
            break;
        default:
            break;
    }
}

int hkCamera::openLoginCamera(QStringList ipStrList)
{
//    this->loginParams = ipStrList;

    QString userName = ipStrList.at(0);
    QString password = ipStrList.at(1);
    QString ip = ipStrList.at(2);

    bool ret;
    // 注册设备

    //登录参数，包括设备地址、登录用户、密码等
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    struLoginInfo.bUseAsynLogin = 0; //同步登录方式
    strcpy(struLoginInfo.sDeviceAddress, ip.toLocal8Bit().data()); //设备IP地址
    struLoginInfo.wPort = 8000; //设备服务端口

    qWarning()<<"hk username:"<<userName;
    qWarning()<<"hk password:"<<password;
    qWarning()<<"hk ip:"<<ip;


    strcpy(struLoginInfo.sUserName, userName.toLocal8Bit().data()); //设备登录用户名
    strcpy(struLoginInfo.sPassword, password.toLocal8Bit().data()); //设备登录密码


    //设备信息, 输出参数
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
    qWarning("注册设备");
    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    qWarning("lUserID=%d",lUserID);
    if (lUserID < 0)
    {
        qCritical("Login snap face camrea error, %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return -1;
    }
    else
    {
        qWarning()<<"Login snap face camrea with ip:"<<ipStrList << " succussfully";
    }
    //设置报警回调函数
    qWarning("set callback");
    ret = NET_DVR_SetDVRMessageCallBack_V31(MessageCallback, NULL);
    if (ret)
    {
        qWarning()<<"hk face CallBack succussfully";
    }
    //启用布防
    NET_DVR_SETUPALARM_PARAM  struAlarmParam={0};
    struAlarmParam.dwSize=sizeof(struAlarmParam);
    struAlarmParam.byFaceAlarmDetection = 0; //人脸抓拍报警，上传COMM_UPLOAD_FACESNAP_RESULT类型报警信息
    //其他报警布防参数不需要设置，不支持

    lHandle = NET_DVR_SetupAlarmChan_V41(lUserID, & struAlarmParam);
    if (lHandle < 0)
    {
        qCritical("NET_DVR_SetupAlarmChan_V41 error, %d\n", NET_DVR_GetLastError());
    }

//    //注册接收异常消息的回调函数
    ret = NET_DVR_SetExceptionCallBack_V30(NULL, NULL, g_ExceptionCallBack, NULL);
    if (ret)
    {
        qWarning()<<"hk  ExceptionCallBack succussfully";
    }
    onlineStatus = true;
    return 0;

}

void hkCamera::emitImage()
{
    emit sendaFaceImageSig(hkCamera::singleInstance->imageList);
}


BOOL hkCamera::CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
    static int localId=0;
    NET_VCA_FACESNAP_RESULT faceSnapInfo;
    NET_VCA_FACESNAP_ADDINFO faceSnapAddInfo;
    memcpy(&faceSnapInfo, pAlarmInfo, sizeof(NET_VCA_FACESNAP_RESULT));
    printf("lCommand:0x%x\n",lCommand);
    switch(lCommand)
    {
        case COMM_UPLOAD_FACESNAP_RESULT:
        {

            if (faceSnapInfo.dwFacePicLen > 0)
            {
                if(faceSnapInfo.dwFaceScore<70)
                {
                    break;
                }
                localId++;

                qWarning() << "dwFacePicID:" << QString::number(faceSnapInfo.dwFacePicID) <<",dwFaceScore:" << QString::number(faceSnapInfo.dwFaceScore);
                QString  tempDirStr = "pic/hkpic/"+QDateTime::currentDateTime().toString("yyyy-MM-dd");
                QDir picDir;
                if (!picDir.exists(tempDirStr))
                {
                    picDir.mkpath(tempDirStr);
                }

                QString picName = tempDirStr + "/" + QString::number(faceSnapInfo.dwFacePicID) + "_"+QString::number(localId) +".jpg";

                QByteArray ba=picName.toLatin1();
                char *filename=ba.data();
                FILE *fSnapPic = NULL;
                fSnapPic = fopen(filename, "wb");
                fwrite(faceSnapInfo.pBuffer1, faceSnapInfo.dwFacePicLen, 1, fSnapPic);
                fclose(fSnapPic);
                hkCamera::singleInstance->imageList.push_front(picName);
                if(hkCamera::singleInstance->imageList.length()>4){
                    hkCamera::singleInstance->imageList.pop_back();
                }
                hkCamera::singleInstance->emitImage();
            }
        }
        break;
    default:
        break;
    }
}




