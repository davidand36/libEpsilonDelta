#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta util library.
#
#

set( sources
     Exception.cpp
     Assert.cpp
     TestCheck.cpp
     Logger.cpp
     FixEndian.cpp
     CharType.cpp
     StringUtil.cpp
     UnicodeUtil.cpp
     CSV.cpp
     JSON.cpp
     JSONException.cpp
     SmartPtr.cpp
     Array.cpp
     VMap.cpp
     IndexedVector.cpp
     Algorithms.cpp
   )


add_library( EpsilonDelta_util  STATIC  ${sources} )
