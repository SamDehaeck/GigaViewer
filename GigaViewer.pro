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
    QMAKE_INCDIR += "C:\HDF5\1.8.15\include"  #this cannot have a space => copy installed hdf5 folder to the root
    QMAKE_LIBDIR += "C:\HDF5\1.8.15\lib"
    QMAKE_LIBDIR += "C:\opencv\build\x86\vc12\lib"

    LIBS +=  "C:\Program Files\Allied Vision Technologies\GigESDK\lib-pc\PvAPI.lib" \
         "C:\Program Files\Allied Vision Technologies\GigESDK\lib-pc\ImageLib.lib" \
         "C:\Program Files\Allied Vision Technologies\AVTVimba_1.3\VimbaCPP\Lib\Win32\VimbaCPP.lib"
    LIBS += -lopengl32 -lhdf5 -lhdf5_cpp -lopencv_core2411 -lopencv_imgproc2411 -lopencv_highgui2411 -lopencv_video2411
} else {
    LIBS += -L /usr/local/lib   # store PvAPI and VimbaCPP libraries here
    QMAKE_INCDIR += /usr/include/hdf5/serial
    QMAKE_LIBDIR += /usr/lib/x86_64-linux-gnu/hdf5/serial
    LIBS += -lhdf5 -lhdf5_hl -lhdf5_cpp -pthread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lPvAPI -lVimbaCPP
#    LIBS += -lopencv_imgcodecs -lopencv_videoio
    LIBS += -lueye_api
}

    SOURCES += idssourcesink.cpp
    HEADERS += idssourcesink.h \
    ueye.h

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
    hdf5sourcesink.h \


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
