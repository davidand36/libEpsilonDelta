#ifndef PLATFORM_HPP
#define PLATFORM_HPP
/*
  Platform.hpp
  Copyright (C) 2007 David M. Anderson

  Macros for indentifying compiler, operating system, processor, etc.
*/


//*****************************************************************************


#if defined(__GNUC__)
#define COMPILER_GNU    __GNUC__
#endif
#if defined(__MSC_VER)
#define COMPILER_MSC    __MSC_VER
#elif defined(_MSC_VER)
#define COMPILER_MSC    _MSC_VER
#endif

#if defined(__unix__) || defined(__unix) || defined(unix)
#define OS_UNIX
#endif
#if defined(__linux__) || defined(__linux) || defined(linux)
#define OS_UNIX
#define OS_LINUX
#endif
#if defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
#define OS_WIN32
#endif
#if defined(_WIN64)
#define OS_WINDOWS
#define OS_WIN64
#endif

#if defined(__i386) || defined(__i386__) || defined(i386) || defined(_M_IX86)
#define CPU_X86
#endif
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__)
#define CPU_X86
#define CPU_X86_64
#endif


//*****************************************************************************

#endif //PLATFORM_HPP
