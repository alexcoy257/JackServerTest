QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = lrnetjackserver

CONFIG += file_copies
#DEFINES += LINUX_KLUDGE

!isEmpty(jack){
message(Using JACK at $$jack)
INCLUDEPATH+=$$jack/include
LIBS += -L$$jack/lib
}

# isEmpty(PREFIX) will allow path to be changed during the command line
# call to qmake, e.g. qmake PREFIX=/usr
isEmpty(PREFIX) {
 PREFIX = /usr/local
}

message(Installing to $$PREFIX)

DESTDIR=./bin

shared{
CONFIG += create_prl
COPIES += libraryHeaders
libraryHeaders.files = jackinterface.h jackparameterform.h lrnetjackservertest.h
libraryHeaders.path = $$PREFIX/include
INSTALLS += libraryHeaders

win32{
#CONFIG += staticlib
}
DESTDIR=./lib
TEMPLATE = lib
target.path = $$PREFIX/lib/
}





# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32{
#INCLUDEPATH += "C:\Program Files\JACK2\include"
#LIBS += "C:\Program Files\JACK2\lib\libjack64.dll.a"
#LIBS += "C:\Program Files\JACK2\lib\libjackserver64.dll.a"

#DEFINES += __WIN_32__

DEFINES += CONFIG_PORTAUDIO
INCLUDEPATH += "C:\msys64\mingw64\include"
#CONFIG += static
#DEFINES += STATIC_LRLIBJACKSERVER
LIBS += "C:\msys64\mingw64\lib\libjack64.dll.a"
LIBS += "C:\msys64\mingw64\lib\libjackserver64.dll.a"
#LIBS += -Bstatic "C:\msys64\mingw64\lib\libportaudio.a" -Bdynamic
LIBS += -mthreads -IC:/msys64/mingw64/include -LC:/msys64/mingw64/lib
LIBS += -lportaudio -lwinmm -lm -luuid -lsetupapi -lole32

#LIBS += -lsetupapi -lwinmm
}

linux{
DEFINES += CONFIG_ALSA_SEQ

LIBS += -ljack -ljackserver -lasound
CONFIG += link_pkgconfig
PKGCONFIG += dbus-1

SOURCES += audio_reserve.c reserve.c
}

macx{
DEFINES += CONFIG_COREAUDIO __MAC_OSX__

#INCLUDEPATH +=/Volumes/Alex_Coy_Projects_2/jack2/include

LIBS += -framework CoreAudio -framework CoreFoundation
LIBS += -ljack -ljackserver
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
    qjackctlInterfaceComboBox.h \
    lrnetjackservertest.h

FORMS += \
    jackparameterform.ui \
    jackwindow.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
