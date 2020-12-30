include (JackServerTest.pri)
include (../lrnet.pri)

CONFIG += file_copies

COPIES += libraryHeaders

libraryHeaders.files = jackparameterform.h lrnetjackservertest.h jackinterface.h
libraryHeaders.path = $$LIBINCDIR

DESTDIR=$$DLLDESTDIR

DEFINES += LRNETJACKSERVERTEST_LIBRARY
TEMPLATE = lib

HEADERS += \
    lrnetjackservertest.h
