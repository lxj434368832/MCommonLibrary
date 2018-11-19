#ifndef MQWT_GLOBAL_H
#define MQWT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MQWT_LIBRARY)
#  define MQWT_EXPORT Q_DECL_EXPORT
#else
#  define MQWT_EXPORT Q_DECL_IMPORT
#endif

#endif // MQWT_GLOBAL_H
