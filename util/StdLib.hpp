#ifndef STDLIB_HPP
#define STDLIB_HPP
/*
  StdLib.hpp
  Copyright (C) 2007 David M. Anderson
  
  Deal with variations in <cstdlib>.
*/


#include <cstdlib>

#if defined(__GNUC__)

#elif defined(_MSC_VER)
#define strtoll     _strtoi64
#define strtoull    _strtoui64
#define strtof      strtod
#define strtold     strtod

#endif //compiler


#endif //STDLIB_HPP
