#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QTSCREENCAPTURE_LIB)
#  define QTSCREENCAPTURE_EXPORT Q_DECL_EXPORT
# else
#  define QTSCREENCAPTURE_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTSCREENCAPTURE_EXPORT
#endif
