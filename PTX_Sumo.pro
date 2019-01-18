TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += debug

SOURCES += \
        main.cpp \
    PTX_System/PTX_AreaCore.cpp \
    PTX_System/PTX_Area.cpp \
    PTX_System/PTX_Cam.cpp \
    PTX_System/PTX_Color.cpp \
    PTX_System/PTX_HUE_Zone.cpp \
    PTX_System/PTX_Core.cpp \
    PTX_System/PTX_System.cpp


win32
{
    debug
    {
        CONFIG += console
    }

    INCLUDEPATH += E:/Programmation/Librairies/SFML-2.5.1-build-debug/include
    INCLUDEPATH += E:/Programmation/Librairies/Box2D-2.3.0-build-debug/include
    INCLUDEPATH += E:/Programmation/Librairies/OpenCV-4.0.1-build-debug/include

    LIBS += -LE:/Programmation/Librairies/Box2D-2.3.0-build-debug/bin/x86/Debug/ -lBox2D

    LIBS += -LE:/Programmation/Librairies/SFML-2.5.1-build-debug/lib/ -lsfml-audio-d
    LIBS += -LE:/Programmation/Librairies/SFML-2.5.1-build-debug/lib/ -lsfml-graphics-d
    LIBS += -LE:/Programmation/Librairies/SFML-2.5.1-build-debug/lib/ -lsfml-main-d
    LIBS += -LE:/Programmation/Librairies/SFML-2.5.1-build-debug/lib/ -lsfml-system-d
    LIBS += -LE:/Programmation/Librairies/SFML-2.5.1-build-debug/lib/ -lsfml-window-d

    LIBS += -LE:\Programmation\Librairies\OpenCV-4.0.1-build-debug\lib/ -lopencv_core401
    LIBS += -LE:\Programmation\Librairies\OpenCV-4.0.1-build-debug\lib/ -lopencv_highgui401
    LIBS += -LE:\Programmation\Librairies\OpenCV-4.0.1-build-debug\lib/ -lopencv_imgproc401
    LIBS += -LE:\Programmation\Librairies\OpenCV-4.0.1-build-debug\lib/ -lopencv_imgcodecs401
    LIBS += -LE:\Programmation\Librairies\OpenCV-4.0.1-build-debug\lib/ -lopencv_videoio401
    LIBS += -LE:\Programmation\Librairies\OpenCV-4.0.1-build-debug\lib/ -lopencv_features2d401

}

HEADERS += \
    PTX_System/PTX_AreaCore.h \
    PTX_System/PTX_Area.h \
    PTX_System/PTX_Cam.h \
    PTX_System/PTX_Color.h \
    PTX_System/PTX_HUE_Zone.h \
    PTX_System/PTX_Core.h \
    PTX_System/PTX_System.h
