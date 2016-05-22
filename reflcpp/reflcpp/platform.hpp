/******************************************************************************\
*                                                                              *
*   platform.h                                                                 *
*                                                                              *
*   These defines are intended to provide a simple way to #ifdef code.         *
*   They are currenly very simple.  I may need to provide additional           *
*   information like the OS version and compiler version and name.             *
*                                                                              *
\******************************************************************************/

// $Id: platform.hpp,v 1.1 2007/10/25 14:05:28 tdevadit Exp $



#ifndef REFLCPP_PLATFORM_H
#define REFLCPP_PLATFORM_H



// Operating system

#if defined(_AIX)
    #define IXCR_AIX
#elif defined(__sgi)
    #define IXCR_IRIX
#elif defined(__linux)
    #define IXCR_LINUX
#elif defined(__sun)
    #define IXCR_SOLARIS
#elif defined(_WIN32)
    #define IXCR_WIN32
#else
    #error  Unknown OS.
#endif



// Compiler

#if defined(__GNUC__)
    #define IXCR_GNUC
#elif defined (mips) && defined(_COMPILER_VERSION)
    #define IXCR_MIPSPRO
#elif defined (__SUNPRO_CC)
    #define IXCR_SUNPRO
#elif defined (_MSC_VER)
    #if _MSC_VER < 1310
        #error The C++ reflection library is unlikely to compile with this version of Visual C++.
    #endif
    #define IXCR_VCPP
#else
    #error Unknown compiler.
#endif



// Template handling

#if defined(IXCR_GNUC) || defined(IXCR_MIPSPRO)
    #define IXCR_INCLUDE_TEMPLATE
#endif



namespace reflcpp {
static const char platform_h_id[] =
"$Id: platform.hpp,v 1.1 2007/10/25 14:05:28 tdevadit Exp $";
}



#endif
