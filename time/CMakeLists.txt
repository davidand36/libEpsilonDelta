#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta time library.
#
#

set( sources
     TimeLibText.cpp
     Time.cpp
     TimeIncrement.cpp
     TimeStandards.cpp
     ModifiedJulianDay.cpp
     Epoch.cpp
     JDDate.cpp
     GregorianCalendar.cpp
     GregorianDate.cpp
     WesternWeek.cpp
     DateTime.cpp
     StdTime.cpp
     StdClock.cpp
     Timer.cpp
   )

set( EpsilonDeltaLibs
     math util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_time  STATIC  ${sources} )
