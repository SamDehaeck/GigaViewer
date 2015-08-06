#-------------------------------------------------
#
# Project created by QtCreator 2012-01-04T15:26:37
#
#-------------------------------------------------

QT       += core gui opengl widgets

TARGET = GigaViewer
TEMPLATE = app

win32 {
    INCLUDEPATH += C:\opencv\build\include

    LIBS +=  C:\opencv\build\x86\vc10\lib\opencv_core2410.lib \
         C:\opencv\build\x86\vc10\lib\opencv_imgproc2410.lib \
         C:\opencv\build\x86\vc10\lib\opencv_highgui2410.lib \
         C:\opencv\build\x86\vc10\lib\opencv_video2410.lib \
         "C:\Program Files\Allied Vision Technologies\GigESDK\lib-pc\PvAPI.lib" \
         "C:\Program Files\Allied Vision Technologies\GigESDK\lib-pc\ImageLib.lib"
} else {
    LIBS += -L /usr/local/lib
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
    imagesourcesink.cpp \
    regexsourcesink.cpp \
    cameracontrolsdialog.cpp \
    vimbasourcesink.cpp
#unix {
    SOURCES += avtsourcesink.cpp
#}


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
    cameracontrolsdialog.h \
    regexsourcesink.h \
    vimbasourcesink.h \
    VimbaCpp/Feature.hpp \
    VimbaCpp/FeatureContainer.h \
    VimbaCpp/FeatureContainer.hpp \
    VimbaCpp/FileLogger.h \
    VimbaCpp/Frame.h \
    VimbaCpp/ICameraFactory.h \
    VimbaCpp/ICameraListObserver.h \
    VimbaCpp/IFeatureObserver.h \
    VimbaCpp/IFrameObserver.h \
    VimbaCpp/IInterfaceListObserver.h \
    VimbaCpp/Interface.h \
    VimbaCpp/Interface.hpp \
    VimbaCpp/IRegisterDevice.h \
    VimbaCpp/LoggerDefines.h \
    VimbaCpp/Mutex.h \
    VimbaCpp/SharedPointer.h \
    VimbaCpp/SharedPointer_impl.h \
    VimbaCpp/SharedPointerDefines.h \
    VimbaCpp/UserLoggerDefines.h \
    VimbaCpp/UserSharedPointerDefines.h \
    VimbaCpp/VimbaCPP.h \
    VimbaCpp/VimbaCPPCommon.h \
    VimbaCpp/VimbaSystem.h \
    VimbaCpp/VimbaSystem.hpp

#unix {
    HEADERS +=  avtsourcesink.h \
                PvApi.h
#}

FORMS += \
    fileinputdialog.ui \
    playbackdialog.ui \
    cameracontrolsdialog.ui

OTHER_FILES += \
    README.txt \
    gigaviewer.desktop \
    GigaViewer.svg \
    video-fmf.xml

RESOURCES += \
    icons.qrc
