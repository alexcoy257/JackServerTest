QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = lrnetjackserver

CONFIG += file_copies
#CONFIG += build_lib

build_lib{
COPIES += libraryHeaders
libraryHeaders.files = jackinterface.h jackparameterform.h lrnetjackservertest.h
libraryHeaders.path = ./include
win32{
CONFIG += staticlib
}
DESTDIR=./lib
TEMPLATE = lib
}

!build_lib{
DESTDIR=./bin
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32{
DEFINES += CONFIG_PORTAUDIO
INCLUDEPATH += "C:\Program Files\JACK2\include"
CONFIG += static
DEFINES += STATIC_LRLIBJACKSERVER
LIBS += "C:\Program Files\JACK2\lib\libjack64.lib"
LIBS += "C:\Program Files\JACK2\lib\libjackserver64.lib"
LIBS += "C:\msys64\mingw64\lib\libportaudio.a"
LIBS += -lsetupapi -lwinmm
}

linux{
DEFINES += CONFIG_ALSA_SEQ

LIBS += -ljack -ljackserver -lasound
}

macx{
DEFINES += CONFIG_COREAUDIO __MAC_OSX__

INCLUDEPATH +=/Volumes/Alex_Coy_Projects_2/jack2/include

LIBS += -framework CoreAudio -framework CoreFoundation
LIBS += -L/Volumes/Alex_Coy_Projects_2/jack2/lib -ljack -ljackserver
}

DEFINES += LIBJACKSERVERTEST_LIBRARY

SOURCES += \
    jackinterface.cpp \
    jackparameterform.cpp \
    main.cpp \
    jackwindow.cpp \
    qjackctlInterfaceComboBox.cpp

HEADERS += \
    jackinterface.h \
    jackparameterform.h \
    jackwindow.h \
    portaudio.h \
    qjackctlInterfaceComboBox.h \
    lrnetjackservertest.h

FORMS += \
    jackparameterform.ui \
    jackwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
