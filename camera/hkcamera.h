#ifndef HKCAMERA_H
#define HKCAMERA_H

#include <QObject>
#include <hksdk/HCNetSDK.h>
#include <QDebug>
#include <QDir>
#include <QDateTime>

class hkCamera : public QObject
{
    Q_OBJECT
public:
    explicit hkCamera(QObject *parent = 0);
        ~hkCamera();
    static hkCamera* getInstance(QObject *obj);
    int openLoginCamera(QStringList ipStr);
    static BOOL CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void*pUser);
    bool onlineStatus = false;
    LONG lUserID = -1;
    LONG lHandle = -1;
    static hkCamera* singleInstance;
    void emitImage();
    QStringList imageList;

signals:
    void sendaFaceImageSig(QStringList imageList);
public slots:
};

#endif // HKCAMERA_H

