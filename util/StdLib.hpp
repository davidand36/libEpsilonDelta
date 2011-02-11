#ifndef STDLIB_HPP
#define STDLIB_HPP
/*
  StdLib.hpp
  Copyright (C) 2007 David M. Anderson
  
  Deal with variations in <cstdlib>.
*/


#include "Platform.hpp"
#include <cstdlib>

#if defined(COMPILER_GNU)
#if defined(OS_ANDROID)
#define strtold     strtod
#endif

#elif defined(COMPILER_MSC)
#define strtoll     _strtoi64
#define strtoull    _strtoui64
#define strtof      strtod
#define strtold     strtod

#endif //compiler


#endif //STDLIB_HPP
