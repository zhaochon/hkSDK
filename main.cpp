#include "mainwindow.h"
#include <QApplication>
#include "quickcheckdefine.h"
//#include <QFile>
//#include <QMutex>
#include <StringUtils.h>


void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.tryLock(20);

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    {
        QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
        QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss-zzz");
        QString current_date = QString("(%1)").arg(current_date_time);
        QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(current_date).arg(msg);

        QFile file(StringUtils::GetCurrentLogPath() + "applog.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << message << "\r\n";
        file.flush();
        file.close();
    }
    mutex.unlock();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(outputMessage);
    MainWindow w;
    w.show();

    return a.exec();
}
