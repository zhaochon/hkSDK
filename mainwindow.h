#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camera/hkcamera.h"
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <QDebug>
//#include <QDir>
//#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QList<QWidget*> childWidgetList;
    QWidget* getChildWidgetWithName(QString objectName);
    long play(long userID, HWND hWnd, NET_DVR_PREVIEWINFO struPlayInfo);


private slots:
    void on_pushButton_clicked();
    void sendFaceImage(QStringList imageList);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
