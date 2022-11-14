#include "StringUtils.h"
#include "definefeatures.h"
#include "qimage.h"



StringUtils::StringUtils(QObject *parent) : QObject(parent)
{

}

QString StringUtils::GetCurrentLogPath()
{
    QString current = QDir::current().absolutePath();
    QDir logsDir(current + "/Logs");
    if(!logsDir.exists())
    {
        qDebug()<<"--------------";
        qDebug()<<current + "/logs";
        logsDir.mkdir(current + "/Logs");
    }

    QString dateString = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString destPath = current + "/Logs/"+dateString + "/";
    QDir dir(destPath);
    if(!dir.exists())
    {
        dir.mkdir(destPath);
    }
    return destPath;
}

QString StringUtils::GetCurrentPicturePath()
{
    FUNCTION_BEGIN;
    QString current = QDir::current().absolutePath();
    QDir imagesDir(current + "/images");
    if(!imagesDir.exists())
    {
        qDebug()<<"--------------";
        qDebug()<<current + "/images";
        imagesDir.mkdir(current + "/images");
    }

    QString dateString = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString destPath = current + "/images/"+dateString + "/";
    QDir dir(destPath);
    if(!dir.exists())
    {
        dir.mkdir(destPath);
    }
    FUNCTION_END;

    return destPath;
}





















