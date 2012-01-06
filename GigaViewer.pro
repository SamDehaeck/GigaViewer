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
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video
}

SOURCES += main.cpp \
    videoglscene.cpp \
    fileinputdialog.cpp

HEADERS  += \
    videoglscene.h \
    fileinputdialog.h

FORMS += \
    fileinputdialog.ui

OTHER_FILES += \
    README.txt
