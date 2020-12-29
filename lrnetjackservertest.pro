include (JackServerTest.pri)
include (../lrnet.pri)

DESTDIR=$$DLLDESTDIR

DEFINES += LRNETJACKSERVERTEST_LIBRARY
TEMPLATE = lib

HEADERS += \
    lrnetjackservertest.h
