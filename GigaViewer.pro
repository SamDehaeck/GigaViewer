#-------------------------------------------------
#
# Project created by QtCreator 2012-01-04T15:26:37
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = GigaViewer
TEMPLATE = app

win32 {
    INCLUDEPATH += C:\opencv\include
    LIBS += C:\opencv\bin\libopencv_core231d.dll \
        C:\opencv\bin\libopencv_imgproc231d.dll \
        C:\opencv\bin\libopencv_highgui231d.dll \
        C:\opencv\bin\libopencv_video231d.dll
} else {
    LIBS += -L/usr/local/lib
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lPvAPI
}

SOURCES += main.cpp \
    imagepacket.cpp \
    videoglscene.cpp \
    fileinputdialog.cpp \
    coordinator.cpp \
    maingui.cpp \
    picbackend.cpp \
    cambackend.cpp \
    playbackdialog.cpp \
    mainwindow.cpp \
    opencvsourcesink.cpp \
    fmfsourcesink.cpp \
    avtsourcesink.cpp

HEADERS  += \
    videoglscene.h \
    fileinputdialog.h \
    coordinator.h \
    maingui.h \
    picbackend.h \
    cambackend.h \
    imagepacket.h \
    playbackdialog.h \
    mainwindow.h \
    opencvsourcesink.h \
    fmfsourcesink.h \
    imagesourcesink.h \
    avtsourcesink.h \
    PvApi.h

FORMS += \
    fileinputdialog.ui \
    playbackdialog.ui

OTHER_FILES += \
    README.txt

RESOURCES += \
    icons.qrc
