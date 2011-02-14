#ifndef PLATFORM_HPP
#define PLATFORM_HPP
/*
  Platform.hpp
  Copyright (C) 2007 David M. Anderson

  Macros for indentifying compiler, operating system, processor, etc.
*/


//*****************************************************************************


#if defined(__GNUC__)
#   define COMPILER_GNU    __GNUC__
#endif
#if defined(__MSC_VER)
#   define COMPILER_MSC    __MSC_VER
#elif defined(_MSC_VER)
#   define COMPILER_MSC    _MSC_VER
#endif


#if defined(__unix__) || defined(__unix) || defined(unix)
#   define OS_UNIX
#endif
#if defined(__linux__) || defined(__linux) || defined(linux)
#   define OS_UNIX
#   define OS_LINUX
#endif
#if defined(_WIN32) || defined(WIN32)
#   define OS_WINDOWS
#   define OS_WIN32
#endif
#if defined(_WIN64)
#   define OS_WINDOWS
#   define OS_WIN64
#endif
#if defined(ANDROID) || defined(__ANDROID) || defined(__ANDROID__)
#   define OS_ANDROID
#endif


#if defined(__i386) || defined(__i386__) || defined(i386) || defined(_M_IX86)
#   define CPU_X86
#endif
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__) || defined(_M_IA64)
#   define CPU_X86
#   define CPU_X86_64
#endif
#if defined(__ARM_ARCH_5__)
#   define CPU_ARM
#   define CPU_ARM_5
#endif


#define CPP_EXCEPTIONS  1
#define CPP_RTTI  1
#if defined(COMPILER_GNU)
#   if ! defined(__EXCEPTIONS)
#       undef CPP_EXCEPTIONS
#   endif
#   if ! defined(__GXX_RTTI)
#       undef CPP_RTTI
#   endif
#endif


#if defined(COMPILER_GNU)
#   define SIZEOF_SHORT         __SIZEOF_SHORT__
#   define SIZEOF_INT           __SIZEOF_INT__
#   define SIZEOF_LONG          __SIZEOF_LONG__
#   define SIZEOF_LONG_LONG     __SIZEOF_LONG_LONG__

#elif defined(COMPILER_MSC)
#   define SIZEOF_SHORT         2
#   define SIZEOF_INT           4
#   define SIZEOF_LONG          4
#   define SIZEOF_LONG_LONG     8

#endif


#undef LITTLE_ENDIAN
#undef BIG_ENDIAN
#if defined(COMPILER_GNU)
#   include <endian.h>
#   if (__BYTE_ORDER == __BIG_ENDIAN)
#       define BIG_ENDIAN __BIG_ENDIAN
#   else
#       define LITTLE_ENDIAN __LITTLE_ENDIAN
#   endif

#elif defined(COMPILER_MSC)
#   if defined(CPU_X86)
#       define LITTLE_ENDIAN 1234
#   else
#       define BIG_ENDIAN 4321
#   endif

#endif


//*****************************************************************************

#endif //PLATFORM_HPP
