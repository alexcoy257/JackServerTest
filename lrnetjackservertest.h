#ifndef LRNETJACKSERVERTEST_H
#define LRNETJACKSERVERTEST_H

#include <QtCore/QtGlobal>

#ifndef STATIC_LRLIBJACKSERVER
#if defined(LIBJACKSERVERTEST_LIBRARY)
#  define LRNETJACKSERVERTEST_EXPORT Q_DECL_EXPORT
#else
#  define LRNETJACKSERVERTEST_EXPORT Q_DECL_IMPORT
#endif
#else
#  define LRNETJACKSERVERTEST_EXPORT
#endif

#endif // LIBJACKSERVERTEST_H
