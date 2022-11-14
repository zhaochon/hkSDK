#-------------------------------------------------
#
# Project created by QtCreator 2022-11-08T16:59:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hksdkDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        StringUtils.cpp \
    camera/hkcamera.cpp

HEADERS  += mainwindow.h \
    hksdk/HCNetSDK.h \
    StringUtils.h \
    quickcheckdefine.h \
    definefeatures.h \
    hksdk/DataType.h \
    hksdk/DecodeCardSdk.h \
    hksdk/plaympeg4.h \
    camera/hkcamera.h

FORMS    += mainwindow.ui

QMAKE_RPATHDIR += ./lib/

unix:!macx: LIBS += -L$$PWD/lib/ -lz -lanalyzedata -lSuperRender  -lPlayCtrl  -lNPQos  -lhpr  -lhcnetsdk -lHCCore -lAudioRender

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.


unix:!macx: LIBS += -L$$PWD/lib/HCNetSDKCom/ -lSystemTransform -lStreamTransClient -liconv2 -lHCVoiceTalk -lHCPreview -lHCPlayBack -lHCIndustry -lHCGeneralCfgMgr -lHCDisplay -lHCAlarm -lAudioIntercom -lanalyzedata

INCLUDEPATH += $$PWD/lib/HCNetSDKCom
DEPENDPATH += $$PWD/lib/HCNetSDKCom


unix:!macx: LIBS += -L$$PWD/usr/local/lib/ -lopencv_highgui
unix:!macx: LIBS += -L$$PWD/usr/local/lib/ -lopencv_core
unix:!macx: LIBS += -L$$PWD/usr/local/lib/ -lopencv_imgproc
unix:!macx: LIBS += -L$$PWD/usr/local/lib/ -lopencv_objdetect
unix:!macx: LIBS += -L$$PWD/usr/local/lib/ -lopencv_video
