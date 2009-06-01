#ifndef STDIO_HPP
#define STDIO_HPP
/*
  StdIO.hpp
  Copyright (C) 2007 David M. Anderson
  
  Deal with variations in <cstdio>.
*/


#include <cstdio>

#if defined(__GNUC__)

#elif defined(_MSC_VER)
#define snprintf    sprintf_s

#endif //compiler


#endif //STDIO_HPP
