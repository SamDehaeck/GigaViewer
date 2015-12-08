#-------------------------------------------------
#
# Project created by QtCreator 2012-01-04T15:26:37
#
#-------------------------------------------------
QT       += core gui opengl widgets


TARGET = GigaViewer
TEMPLATE = app

CONFIG += HDF5       # enable HDF5 format for storing and reading files
CONFIG += TRACKING   # enable tracking of Marangoni-driven particles
#CONFIG += IDS        # use GigE and USB3 cameras from IDS: https://en.ids-imaging.com/
#CONFIG += PVAPI     # use GigE cameras from Prosilica (now AVT). Available on Windows/Mac/Linux: https://www.alliedvision.com
#CONFIG += VIMBA     # use GigE cameras from AVT (newer version of above). For now only Windows/Linux: https://www.alliedvision.com
                     # on Windows also support for Firewire cameras
#CONFIG += IDS PVAPI VIMBA HDF5
# uncomment the CONFIG lines for the camera modules you want compiled, available options: IDS PVAPI VIMBA
# when you want no cameras, comment al camera related CONFIG lines above (IDS PVAPI VIMBA)

IDS {
    DEFINES *= IDS
}
PVAPI {
    DEFINES *= PVAPI
}
VIMBA {
    DEFINES *= VIMBA
}
HDF5 {
    DEFINES *= ENABLE_HDF5
}
TRACKING {
    DEFINES *= TRACKING
}

#message(The Defines are $$DEFINES)

win32 {
    message(Compiling for windows)
    INCLUDEPATH += C:\opencv\build\include
    QMAKE_LIBDIR += "C:\opencv\build\x86\vc12\lib"
    HDF5 {
        QMAKE_INCDIR += "C:\HDF5\1.8.15\include"  #this cannot have a space => copy installed hdf5 folder to the root
        QMAKE_LIBDIR += "C:\HDF5\1.8.15\lib"
    }

    PVAPI {
        LIBS +=  "C:\Program Files\Allied Vision Technologies\GigESDK\lib-pc\PvAPI.lib" \
             "C:\Program Files\Allied Vision Technologies\GigESDK\lib-pc\ImageLib.lib" \
    }
    VIMBA {
        LIBS += "C:\Program Files\Allied Vision Technologies\AVTVimba_1.3\VimbaCPP\Lib\Win32\VimbaCPP.lib"
    }
    IDS {
        LIBS += "C:\Program Files\IDS\uEye\Develop\Lib\uEye_api.lib"
    }
    LIBS += -lopengl32 -lhdf5 -lhdf5_cpp -lopencv_core2411 -lopencv_imgproc2411 -lopencv_highgui2411 -lopencv_video2411
}

unix:!macx {
    message(Compiling for Linux)
    LIBS += -L /usr/local/lib   # store PvAPI and VimbaCPP libraries here

    HDF5 {
        QMAKE_INCDIR += /usr/include/hdf5/serial
        QMAKE_LIBDIR += /usr/lib/x86_64-linux-gnu/hdf5/serial
        LIBS += -lhdf5 -lhdf5_hl -lhdf5_cpp
    }
    LIBS += -pthread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video
#        LIBS += -lopencv_imgcodecs -lopencv_videoio  # for opencv 3.0 these packages are necessary

    PVAPI {
        LIBS += -lPvAPI
    }
    VIMBA {
        LIBS += -lVimbaCPP
    }
    IDS {
        LIBS += -lueye_api
    }
}
macx {
    message(Compiling for Mac) # No camera modules supported so far!
    VIMBA|IDS|PVAPI {
        message(No camera modules support so far!!! Change configuration in GigaViewer.pro.)
    }
    HDF5 {
        LIBS += -lhdf5 -lhdf5_hl -lhdf5_cpp
    }
    LIBS += -pthread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video
}

PVAPI {
    message(Including PVAPI libraries)
    SOURCES += avtsourcesink.cpp
    HEADERS += avtsourcesink.h
}

IDS {
    message(Including IDS libraries)
    SOURCES += idssourcesink.cpp
    HEADERS += idssourcesink.h
}

VIMBA {
    message(Including VIMBA libraries)
    SOURCES +=  vimbasourcesink.cpp vimbaframeobserver.cpp
    HEADERS +=  vimbasourcesink.h \
    vimbaframeobserver.h
}

HDF5 {
    SOURCES += hdf5sourcesink.cpp
    HEADERS += hdf5sourcesink.h
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
    cameracontrolsdialog.cpp

TRACKING {
    SOURCES += marangonitracking.cpp \
        marangonitrackingdialog.cpp

    HEADERS += marangonitracking.h \
        marangonitrackingdialog.h

    FORMS += marangonitrackingdialog.ui
}

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
    regexsourcesink.h


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

DISTFILES += \
    TODO.txt
