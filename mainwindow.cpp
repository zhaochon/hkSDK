#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QWidget*> childWidgetList;

    this->childWidgetList<<ui->face0<<ui->face1<<ui->face2<<ui->face3;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QWidget *MainWindow::getChildWidgetWithName(QString objectName)
{
    for(int i=0; i< this->childWidgetList.length(); i++)
    {
        QWidget* w = this->childWidgetList.at(i);
        if(w->objectName() == objectName)
        {
            return w;
        }
    }
    qDebug()<<"Error: try to get child: "+objectName + " not exist";
    return nullptr;
}


void MainWindow::on_pushButton_clicked()
{
    QStringList cip1;

    QString ip = ui->ip->text();
    QString name = ui->name->text();
    QString password = ui->password->text();

    //    QString ip = "192.168.1.201";
    //    QString name = "admin";
    //    QString password = "stfz2430";

    cip1<<name<<password<<ip;

    hkCamera* cameraHelper = hkCamera::getInstance(this);
    connect(cameraHelper, &hkCamera::sendaFaceImageSig, this, &MainWindow::sendFaceImage);
    cameraHelper->openLoginCamera(cip1);
    if(cameraHelper->lUserID >= 0) {
        HWND hWnd = (HWND)ui->video->winId();
        NET_DVR_PREVIEWINFO struPlayInfo;
        long IRealPlayHandle = this->play(cameraHelper->lUserID,hWnd,struPlayInfo);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    hkCamera* cameraHelper = hkCamera::getInstance(this);
    if(cameraHelper->lUserID >= 0) {
        QDir tempDir;
        if (!tempDir.exists("pic"))
        {
            tempDir.mkdir("pic");
        }

        QDir picDir;
        if (!picDir.exists("./pic/" + QDateTime::currentDateTime().toString("yyyy-MM-dd")))
        {
            picDir.mkdir("./pic/" + QDateTime::currentDateTime().toString("yyyy-MM-dd"));
        }

        QString picName = "./pic/" + QDateTime::currentDateTime().toString("yyyy-MM-dd")+ "/"  + "cap.jpg";
        QByteArray picNameStr;
        picNameStr.append(picName);

        NET_DVR_JPEGPARA tempPara = {0};
        memset(&tempPara, 0x0, sizeof(tempPara));
        tempPara.wPicSize = 12;
        tempPara.wPicQuality = 0;

        bool result = NET_DVR_CaptureJPEGPicture(cameraHelper->lUserID,1,&tempPara,picNameStr.data());

        if (result)
        {
            //            QPixmap capturePic(picName);
            //            ui->videoImage->setPixmap(capturePic);
            cv::Mat cardMat = cv::imread(picName.toLocal8Bit().data());
            QImage image ((uchar*)cardMat.data, cardMat.cols, cardMat.rows, cardMat.step, QImage::Format_RGB888);
            QPixmap pixmap = QPixmap::fromImage(image.rgbSwapped());
            QPixmap p = pixmap.scaled(ui->videoImage->width(), ui->videoImage->height(), Qt::KeepAspectRatio);
            ui->videoImage->setPixmap(p);
        }
        else
        {
            qWarning() << "HKSDK cap error," << NET_DVR_GetLastError();
        }
    }
}


void MainWindow::sendFaceImage(QStringList imageList)
{
    for(int i=0;i<imageList.length();i++) {
        QLabel* face = (QLabel*)getChildWidgetWithName("face"+ QString::number(i));
        cv::Mat cardMat = cv::imread(imageList.at(i).toLocal8Bit().data());
        QImage image ((uchar*)cardMat.data, cardMat.cols, cardMat.rows, cardMat.step, QImage::Format_RGB888);
        QPixmap pixmap = QPixmap::fromImage(image.rgbSwapped());
        QPixmap p = pixmap.scaled(face->width(), face->height(), Qt::KeepAspectRatio);
        face->setPixmap(p);
    }
}

long MainWindow::play(long userID, HWND hWnd, NET_DVR_PREVIEWINFO struPlayInfo)
{
    struPlayInfo={0};
    struPlayInfo.hPlayWnd = hWnd;//(HWND)ui->video->winId();//需要 SDK 解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel = 1;//预览通道号
    struPlayInfo.dwStreamType = 0;//码流类型：0-主码流，1-子码流，2-三码流，3-虚拟码流，以此类推
    struPlayInfo.dwLinkMode = 0;//0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked = 1;//0- 非阻塞取流，1- 阻塞取流

    long IRealPlayHandle = NET_DVR_RealPlay_V40(userID,&struPlayInfo,NULL,NULL);

    if(IRealPlayHandle <0)
    {
        qWarning() << "NET_DVR_RealPlay_V40 error;error number " <<NET_DVR_GetLastError();
        NET_DVR_Logout(userID);
        return 0;
    }
    else
        return IRealPlayHandle;
}



