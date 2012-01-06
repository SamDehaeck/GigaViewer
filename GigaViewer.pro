#-------------------------------------------------
#
# Project created by QtCreator 2012-01-04T15:26:37
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = GigaViewer
TEMPLATE = app

LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video

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
