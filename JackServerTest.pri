QT       += gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32{
DEFINES += CONFIG_PORTAUDIO
INCLUDEPATH += "C:\Program Files\JACK2\include"

LIBS += "C:\Program Files\JACK2\lib\libjack64.lib"
LIBS += "C:\Program Files\JACK2\lib\libjackserver64.lib"
LIBS += "C:\msys64\mingw64\lib\libportaudio.dll.a"

HEADERS += portaudio.h
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

SOURCES += \
    jackinterface.cpp \
    jackparameterform.cpp \
    jackwindow.cpp \
    qjackctlInterfaceComboBox.cpp

HEADERS += \
    jackinterface.h \
    jackparameterform.h \
    jackwindow.h \
    qjackctlInterfaceComboBox.h

FORMS += \
    jackparameterform.ui \
    jackwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
