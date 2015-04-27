#-------------------------------------------------
#
# Project created by QtCreator 2015-04-08T18:30:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Unitracker
TEMPLATE = app
unix{
    INCLUDEPATH += /usr/include/opencv2
    LIBS += -L/usr/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_calib3d
}

win32{
    INCLUDEPATH += C:\opencv\install\include
    LIBS += -L"C:/opencv/install/x64/mingw/bin"
    LIBS += -lopencv_core2410 -lopencv_highgui2410 -lopencv_imgproc2410 -lopencv_calib3d24102410
}

SOURCES += main.cpp\
        mainwindow.cpp \
    tracking.cpp \
    display.cpp \
    calibdialog.cpp \
    prefsdialog.cpp \
    udpcom.cpp

HEADERS  += mainwindow.h \
    tracking.h \
    display.h \
    calibdialog.h \
    prefsdialog.h \
    udpcom.h

FORMS    += mainwindow.ui \
    calibdialog.ui \
    prefsdialog.ui

CONFIG += c++11
