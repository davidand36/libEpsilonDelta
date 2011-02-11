#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for EpsilonDelta astro library.
#
#

set( sources
     GeodeticEllipsoid.cpp
     GeodeticDatum.cpp
     GeodeticLocation.cpp
     SiderealTime.cpp
     Equatorial.cpp
     Horizontal.cpp
     AstroCoordTransformations.cpp
     JPLEphemeris.cpp
     Precession.cpp
     Obliquity.cpp
     Nutation.cpp
     CoordinateReduction.cpp
     ApparentEphemeris.cpp
     AstroPhenomena.cpp
     Seasons.cpp
     MoonPhases.cpp
     RiseSet.cpp
     LunarVisibility.cpp
     EquationOfTime.cpp
   )

set( EpsilonDeltaLibs
     file time math util
   )
foreach( lib ${EpsilonDeltaLibs} )
     include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
endforeach( lib )


add_library( EpsilonDelta_astro  STATIC  ${sources} )
