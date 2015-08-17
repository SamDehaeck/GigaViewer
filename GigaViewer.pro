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

    LIBS +=  C:\opencv\build\x86\vc12\lib\opencv_core2411.lib \
         C:\opencv\build\x86\vc12\lib\opencv_imgproc2411.lib \
         C:\opencv\build\x86\vc12\lib\opencv_highgui2411.lib \
         C:\opencv\build\x86\vc12\lib\opencv_video2411.lib \
         "C:\Program Files\Allied Vision Technologies\GigESDK\lib-pc\PvAPI.lib" \
         "C:\Program Files\Allied Vision Technologies\GigESDK\lib-pc\ImageLib.lib" \
         "C:\Program Files\Allied Vision Technologies\AVTVimba_1.3\VimbaCPP\Lib\Win32\VimbaCPP.lib"
    LIBS += -lopengl32
} else {
    LIBS += -L /usr/local/lib
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lPvAPI -lVimbaCPP
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
    vimbasourcesink.cpp \
    vimbaframeobserver.cpp \
    hdf5sourcesink.cpp
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
    VimbaC/Include/VimbaC.h \
    VimbaC/Include/VmbCommonTypes.h \
    VimbaCPP/Include/AncillaryData.h \
    VimbaCPP/Include/BasicLockable.h \
    VimbaCPP/Include/Camera.h \
    VimbaCPP/Include/Camera.hpp \
    VimbaCPP/Include/EnumEntry.h \
    VimbaCPP/Include/EnumEntry.hpp \
    VimbaCPP/Include/Feature.h \
    VimbaCPP/Include/Feature.hpp \
    VimbaCPP/Include/FeatureContainer.h \
    VimbaCPP/Include/FeatureContainer.hpp \
    VimbaCPP/Include/FileLogger.h \
    VimbaCPP/Include/Frame.h \
    VimbaCPP/Include/ICameraFactory.h \
    VimbaCPP/Include/ICameraListObserver.h \
    VimbaCPP/Include/IFeatureObserver.h \
    VimbaCPP/Include/IFrameObserver.h \
    VimbaCPP/Include/IInterfaceListObserver.h \
    VimbaCPP/Include/Interface.h \
    VimbaCPP/Include/Interface.hpp \
    VimbaCPP/Include/IRegisterDevice.h \
    VimbaCPP/Include/LoggerDefines.h \
    VimbaCPP/Include/Mutex.h \
    VimbaCPP/Include/SharedPointer.h \
    VimbaCPP/Include/SharedPointer_impl.h \
    VimbaCPP/Include/SharedPointerDefines.h \
    VimbaCPP/Include/UserLoggerDefines.h \
    VimbaCPP/Include/UserSharedPointerDefines.h \
    VimbaCPP/Include/VimbaCPP.h \
    VimbaCPP/Include/VimbaCPPCommon.h \
    VimbaCPP/Include/VimbaSystem.h \
    VimbaCPP/Include/VimbaSystem.hpp \
    vimbaframeobserver.h \
    hdf5sourcesink.h

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
