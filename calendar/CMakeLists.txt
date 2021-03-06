#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta calendar library.
#
#

set( sources
     CalendarLibText.cpp
     JulianCalendar.cpp
     JulianDate.cpp
     ISO8601Calendar.cpp
     ISO8601Date.cpp
     PersianWeek.cpp
     PersianCalendar.cpp
     PersianDate.cpp
     IslamicCalendar.cpp
     IslamicWeek.cpp
     IslamicDate.cpp
     HebrewCalendar.cpp
     HebrewWeek.cpp
     HebrewDate.cpp
     ChineseCalendar.cpp
     ChineseDate.cpp
     HinduWeek.cpp
     HinduAstro.cpp
     HinduSolarCalendar.cpp
     HinduSolarDate.cpp
     HinduLunisolarCalendar.cpp
     HinduLunisolarDate.cpp
     CopticCalendar.cpp
     CopticWeek.cpp
     CopticDate.cpp
     EthiopianCalendar.cpp
     EthiopianWeek.cpp
     EthiopianDate.cpp
     BahaiCalendar.cpp
     BahaiWeek.cpp
     BahaiDate.cpp
     BadiCalendar.cpp
     BadiDate.cpp
     EgyptianCalendar.cpp
     EgyptianDate.cpp
     ArmenianCalendar.cpp
     ArmenianWeek.cpp
     ArmenianDate.cpp
     MayanLongCountCalendar.cpp
     MayanLongCountDate.cpp
     MayanHaabCalendar.cpp
     MayanHaabDate.cpp
     MayanTzolkinCalendar.cpp
     FrenchRevolutionaryCalendar.cpp
     FrenchRevolutionaryDate.cpp
   )

set( EpsilonDeltaLibs
     astro file time math util
   )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_calendar  STATIC  ${sources} )
