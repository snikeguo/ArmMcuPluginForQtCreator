#pragma once

#include <QtGlobal>

#if defined(ARMMCU_LIBRARY)
#  define ARMMCUSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ARMMCUSHARED_EXPORT Q_DECL_IMPORT
#endif
