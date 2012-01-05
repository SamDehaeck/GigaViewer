#-------------------------------------------------
#
# Project created by QtCreator 2012-01-04T15:26:37
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = GigaViewer
TEMPLATE = app

INCLUDEPATH += /usr/local/include \
             /usr/local/include/opencv

LIBS += -L/usr/local/lib
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann

SOURCES += main.cpp \
    videoglscene.cpp \
    fileinputdialog.cpp

HEADERS  += \
    videoglscene.h \
    fileinputdialog.h

FORMS += \
    fileinputdialog.ui
